#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>
#include <SPI.h>
#include <MFRC522.h>
#include <TinyGPS++.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <time.h>

// =====================================================
// ĐỊNH NGHĨA CHÂN KẾT NỐI
// =====================================================
#define SIM_RX_PIN    27
#define SIM_TX_PIN    26
#define GPS_RX_PIN    16
#define GPS_TX_PIN    4
#define I2C_SDA_PIN   22
#define I2C_SCL_PIN   23
#define RFID_SCK_PIN  19
#define RFID_MISO_PIN 5
#define RFID_MOSI_PIN 18
#define RFID_SS_PIN   21
#define RFID_RST_PIN  17
#define BUZZER_PIN    13

// =====================================================
// CẤU HÌNH MẠNG & API
// =====================================================
#include "secrets.h"
const char* WIFI_SSID = SECRET_WIFI_SSID;
const char* WIFI_PASS = SECRET_WIFI_PASS;

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 7 * 3600; // GMT+7 cho Viet Nam
const int   daylightOffset_sec = 0;

const String BOT_TOKEN = SECRET_BOT_TOKEN;
const String FIREBASE_HOST = SECRET_FIREBASE_HOST;
const String FIREBASE_AUTH = SECRET_FIREBASE_AUTH; 

// =====================================================
// KHỞI TẠO ĐỐI TƯỢNG
// =====================================================
HardwareSerial SerialSIM(1);
HardwareSerial SerialGPS(2);

MFRC522 rfid(RFID_SS_PIN, RFID_RST_PIN);
TinyGPSPlus gps;
Adafruit_SH1106G display = Adafruit_SH1106G(128, 64, &Wire, -1);

TaskHandle_t TaskGPS;
unsigned long lastBotCheck = 0;
long lastUpdateId = 0;

WiFiClientSecure secureClient;

// =====================================================
// LUỒNG GPS ĐỘC LẬP
// =====================================================
void gpsTask(void * pvParameters) {
  for(;;) {
    while (SerialGPS.available() > 0) {
      gps.encode(SerialGPS.read());
    }
    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}

// =====================================================
// HÀM TIỆN ÍCH
// =====================================================
String urlEncode(String str) {
  String encoded = "";
  for (int i = 0; i < str.length(); i++) {
    char c = str[i];
    if (isalnum(c)) {
      encoded += c;
    } else {
      char buf[4];
      sprintf(buf, "%%%02X", c);
      encoded += buf;
    }
  }
  return encoded;
}

String getNTPFormattedTime() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    return "Chua cap nhat";
  }
  char timeBuf[12];
  strftime(timeBuf, sizeof(timeBuf), "%H:%M:%S", &timeinfo);
  return String(timeBuf);
}

String sendAT(String cmd, int waitMs = 2000) {
  while (SerialSIM.available()) SerialSIM.read();
  SerialSIM.println(cmd);

  String raw = "";
  long start = millis();

  while (millis() - start < waitMs) {
    while (SerialSIM.available()) {
      raw += (char)SerialSIM.read();
    }
    if (raw.indexOf("OK") != -1 || raw.indexOf("ERROR") != -1) {
      delay(50);
      while (SerialSIM.available()) raw += (char)SerialSIM.read();
      break;
    }
  }

  String clean = "";
  for (char c : raw) {
    if ((c >= 32 && c <= 126) || c == '\r' || c == '\n') clean += c;
  }
  
  Serial.print("[AT] "); Serial.print(cmd); Serial.print(" -> "); Serial.println(clean);
  return clean;
}

// =====================================================
// MÀN HÌNH OLED
// =====================================================
void inOLED(String dong1, String dong2 = "", String dong3 = "") {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(0, 5);
  display.println(dong1);
  if(dong2 != "") { display.setCursor(0, 25); display.println(dong2); }
  if(dong3 != "") { display.setCursor(0, 45); display.println(dong3); }
  display.display();
}

void showIdleScreen() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(15, 5);
  display.println("HE THONG SAN SANG");
  display.drawLine(0, 18, 128, 18, SH110X_WHITE);
  display.setTextSize(2);
  display.setCursor(15, 35);
  display.println("QUET THE");
  display.display();
}

// =====================================================
// TƯƠNG TÁC FIREBASE
// =====================================================
bool getParentData(String uid, String &chatId, String &phone, String &currentStatus, String &studentName) {
  if (WiFi.status() != WL_CONNECTED) return false;

  Serial.println("[FB] Dang lay du lieu...");
  HTTPClient http;
  String url = "https://" + FIREBASE_HOST + "/users/" + uid + ".json?auth=" + FIREBASE_AUTH;
  
  http.begin(secureClient, url);
  int httpCode = http.GET();
  bool success = false;

  if (httpCode == HTTP_CODE_OK) {
    String jsonStr = http.getString();
    DynamicJsonDocument doc(1024);
    DeserializationError error = deserializeJson(doc, jsonStr);
    
    if (!error) {
      phone = doc["phone"].as<String>();
      
      // ĐIỀU KIỆN MỚI: Chỉ cần có số điện thoại hợp lệ là cho qua bài test dữ liệu
      if (phone.length() >= 9) {
        chatId = doc["chat_id"].as<String>(); // Lấy chat_id (chấp nhận cả chuỗi ngắn hoặc lỗi)
        
        if (phone.startsWith("84") && !phone.startsWith("+")) {
          phone = "+" + phone;
        }
        
        if (doc.containsKey("name")) {
          studentName = doc["name"].as<String>();
        } else {
          studentName = "Hoc sinh";
        }
        
        if (doc.containsKey("status")) {
          currentStatus = doc["status"].is<JsonObject>() ? 
                          doc["status"].as<JsonObject>().begin()->value().as<String>() : 
                          doc["status"].as<String>();
        } else {
          currentStatus = "out";
        }
        success = true;
      }
    }
  }
  http.end();
  return success;
}

void updateStatus(String uid, String newStatus) {
  if (WiFi.status() != WL_CONNECTED) return;
  HTTPClient http;
  String url = "https://" + FIREBASE_HOST + "/users/" + uid + "/status.json?auth=" + FIREBASE_AUTH;
  http.begin(secureClient, url);
  http.addHeader("Content-Type", "application/json");
  String body = "\"" + newStatus + "\"";
  http.PUT(body); 
  http.end();
}

// =====================================================
// TRUYỀN TIN: TELEGRAM & SMS
// =====================================================
bool sendTelegram(String chatId, String message) {
  if (WiFi.status() != WL_CONNECTED) return false;
  HTTPClient http;
  String url = "https://api.telegram.org/bot" + BOT_TOKEN + "/sendMessage?chat_id=" + chatId + "&text=" + urlEncode(message);
  http.begin(secureClient, url);
  int httpCode = http.GET();
  bool success = (httpCode == HTTP_CODE_OK);
  http.end();
  return success;
}
int checkSignalStrength() {
  String res = sendAT("AT+CSQ", 1000);
  int start = res.indexOf(":");
  int comma = res.indexOf(",");
  if (start != -1 && comma != -1) {
    String csq = res.substring(start + 1, comma);
    csq.trim();
    return csq.toInt();
  }
  return 0;
}
bool sendSMS(String number, String text) {
  int retryCount = 0;
  const int maxRetries = 3;
  bool smsSuccess = false;

  while (retryCount < maxRetries && !smsSuccess) {
    Serial.println("\n--- [DEBUG SMS START] ---");
    Serial.println("[SMS] Lan thu gui: " + String(retryCount + 1));
    Serial.println("[SMS] So: " + number);

    // 1. Ngắt Wi-Fi để dồn điện và chống nhiễu
    WiFi.disconnect(true);
    delay(1000);
    
    // Xóa sạch bộ đệm trước khi làm việc
    while(SerialSIM.available()) SerialSIM.read();

    // 2. Check sóng
    int csq = checkSignalStrength();
    Serial.print("[SMS DEBUG] Chat luong song (CSQ): "); Serial.println(csq);
    
    if (csq < 10 || csq == 99) {
      Serial.println("[SMS] LOI: Song qua yeu, huy lan gui nay...");
      delay(5000); // Đợi 5 giây trước khi retry
      retryCount++;
      continue; 
    }

    // 3. Reset module SIM
    sendAT("AT", 1000); 
    sendAT("AT+CMGF=1", 1000); 

    // 4. Gửi lệnh CMGS
    SerialSIM.print("AT+CMGS=\"");
    SerialSIM.print(number);
    SerialSIM.println("\"");
    
    // Đợi dấu '>'
    long startWait = millis();
    bool promptFound = false;
    String debugBuffer = "";
    while (millis() - startWait < 5000) {
      while (SerialSIM.available()) {
        char c = SerialSIM.read();
        debugBuffer += c;
        if (c == '>') promptFound = true;
      }
    }
    
    Serial.println("[SMS DEBUG] Response sau CMGS: " + debugBuffer);

    if (!promptFound) {
      Serial.println("[SMS] LOI: Khong nhan duoc dau '>'. SIM bi treo.");
      SerialSIM.write(27); 
      delay(2000);
    } else {
      Serial.println("[SMS] Da thay dau '>', gui tin...");
      SerialSIM.print(text);
      delay(100);
      SerialSIM.write(26); 
      delay(3000); // Đợi module xử lý gửi đi

      // Đợi kết quả
      long startResult = millis();
      String result = "";
      while (millis() - startResult < 20000) { 
        while (SerialSIM.available()) {
          result += (char)SerialSIM.read();
        }
        if (result.indexOf("OK") != -1 || result.indexOf("ERROR") != -1 || result.indexOf("+CMGS:") != -1) break;
      }
      Serial.println("[SMS DEBUG] Response ket thuc: " + result);
      
      if (result.indexOf("+CMGS:") != -1) {
        smsSuccess = true;
      }
    }

    // Kết nối lại Wi-Fi sau mỗi lần thử (thành công hay thất bại đều cần reconnect)
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    if (!smsSuccess) {
      delay(2000); 
      retryCount++;
    }
  }

  return smsSuccess;
}

void checkTelegramUpdates() {
  if (WiFi.status() != WL_CONNECTED) return;

  HTTPClient http;
  // Bot Telegram sẽ gửi lệnh, ta lấy message mới nhất
  String url = "https://api.telegram.org/bot" + BOT_TOKEN + "/getUpdates?offset=-1";
  
  http.begin(secureClient, url);
  int httpCode = http.GET();

  if (httpCode == HTTP_CODE_OK) {
    String jsonStr = http.getString();
    DynamicJsonDocument doc(1024);
    if (!deserializeJson(doc, jsonStr)) {
      if (doc["result"].size() > 0) {
        long update_id = doc["result"][0]["update_id"];
        
        // Kiểm tra xem có text message không
        if (doc["result"][0]["message"].containsKey("text")) {
          String text = doc["result"][0]["message"]["text"].as<String>();
          String chat_id = doc["result"][0]["message"]["chat"]["id"].as<String>();
          text.toLowerCase();

          // Chỉ xử lý nếu là tin nhắn mới
          if (update_id > lastUpdateId) {
            lastUpdateId = update_id;
            
            // Xử lý từ khóa "vitri" hoặc "/vitri"
            if (text.indexOf("vitri") != -1) {
              if (gps.location.isValid()) {
                String thoiGian = getNTPFormattedTime();
                String mapLink = "Vị trí xe buýt hiện tại:\nThời gian: " + thoiGian + "\nhttp://maps.google.com/?q=" + String(gps.location.lat(), 6) + "," + String(gps.location.lng(), 6);
                
                sendTelegram(chat_id, mapLink);
                Serial.println("[TG] Da gui vi tri cho chat_id: " + chat_id);
              } else {
                sendTelegram(chat_id, "Hệ thống đang dò tín hiệu GPS, vui lòng thử lại sau");
              }
            }
          }
        }
      }
    }
  }
  http.end();
}

// =====================================================
// SETUP
// =====================================================
void setup() {
  Serial.begin(115200);

  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);

  Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN); 
  display.begin(0x3c, true);
  
  inOLED("HE THONG", "Dang khoi dong...");
  delay(1000);

  SPI.begin(RFID_SCK_PIN, RFID_MISO_PIN, RFID_MOSI_PIN, RFID_SS_PIN);
  rfid.PCD_Init();

  SerialGPS.begin(9600, SERIAL_8N1, GPS_RX_PIN, GPS_TX_PIN);
  xTaskCreatePinnedToCore(gpsTask, "TaskGPS", 2048, NULL, 1, &TaskGPS, 0);

  SerialSIM.begin(9600, SERIAL_8N1, SIM_RX_PIN, SIM_TX_PIN);
  delay(2000);
  sendAT("ATE0", 500);
  sendAT("AT+CMGF=1", 1000);

  // ---------------------------------------------------
  // CẤU HÌNH KẾT NỐI WI-FI SIÊU TỐC
  // ---------------------------------------------------
  WiFi.mode(WIFI_STA);
  WiFi.setSleep(WIFI_PS_NONE);  // Tắt chế độ ngủ để chip thu phát sóng tối đa công suất
  WiFi.persistent(true);        // Cho phép lưu thông tin AP vào bộ nhớ Flash NVS    // Tự động kết nối ở tầng phần cứng khi thấy sóng
  WiFi.setAutoReconnect(true);

  inOLED("KET NOI WI-FI", WIFI_SSID, "Dang thuc hien...");
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(100); // Giảm thời gian delay vòng lặp để bắt bắt tay nhanh hơn
    Serial.print(".");
  }
  
  secureClient.setInsecure();
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  inOLED("WI-FI THANH CONG!", WiFi.localIP().toString());
  delay(1500);
  showIdleScreen();
}

// =====================================================
// LOOP
// =====================================================
void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    inOLED("MAT KET NOI WI-FI", "Dang thu lai...");
    WiFi.reconnect();
    delay(2000);
    if (WiFi.status() == WL_CONNECTED) showIdleScreen();
    return;
  }

  if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
    String uid = "";
    for (byte i = 0; i < rfid.uid.size; i++) {
      uid += String(rfid.uid.uidByte[i] < 0x10 ? "0" : "");
      uid += String(rfid.uid.uidByte[i], HEX);
    }
    uid.toUpperCase();

    digitalWrite(BUZZER_PIN, HIGH);
    delay(150);
    digitalWrite(BUZZER_PIN, LOW);

    inOLED("DA NHAN THE", "UID: " + uid, "Dang truy xuat...");

    String chatId, phone, currentStatus, studentName;
    if (getParentData(uid, chatId, phone, currentStatus, studentName)) {
      String newStatus = (currentStatus == "in") ? "out" : "in";
      updateStatus(uid, newStatus); 
      
      String thoiGian = getNTPFormattedTime(); 
      String tgMessage = "";
      String smsMessage = "";
      
      if (newStatus == "in") {
        tgMessage = "Thông báo: Học sinh [" + studentName + "] da LEN xe buyt.\nThời gian: " + thoiGian + "\n";
        smsMessage = "Hoc sinh [" + studentName + "] da LEN xe buyt vao luc " + thoiGian + ". ";
        inOLED(studentName, "LEN XE", thoiGian);
      } else {
        tgMessage = "Thông báo: Học sinh [" + studentName + "] da XUONG xe buyt.\nThời gian: " + thoiGian + "\n";
        smsMessage = "Hoc sinh [" + studentName + "] da XUONG xe buyt vao luc " + thoiGian + ". ";
        inOLED(studentName, "XUONG XE", thoiGian);
      }
      
      if (gps.location.isValid()) {
        String mapsLink = "https://maps.google.com/?q=" + String(gps.location.lat(), 6) + "," + String(gps.location.lng(), 6);
        tgMessage += "Vi tri: " + mapsLink;
        smsMessage += "Vi tri: " + mapsLink;
      }

      // ---------------------------------------------------
      // LOGIC XỬ LÝ CHAT ID DƯỚI 10 KÝ TỰ
      // ---------------------------------------------------
      if (chatId.length() < 10) {
        Serial.println("[TG] Chat ID khong hop le (duoi 10 ky tu). Chuyen thang sang SMS...");
        inOLED("KHONG CO CHAT ID", "Dang gui SMS...");
        if (sendSMS(phone, smsMessage)) {
          inOLED("HOAN TAT", "Da gui SMS");
        } else {
          inOLED("THAT BAI", "Loi gui SMS");
        }
      } else {
        // Nếu Chat ID đủ độ dài, tiến hành gửi Telegram bình thường
        if (!sendTelegram(chatId, tgMessage)) {
          Serial.println("[TG] Loi gui Telegram, chuyen sang SMS du phong...");
          inOLED("TG THAT BAI", "Chuyen sang SMS...");
          sendSMS(phone, smsMessage);
        }
        inOLED("HOAN TAT", "Da xu ly xong");
      }
      
      delay(2500); 
    } else {
      inOLED("LOI DU LIEU", "The chua dang ky");
      delay(2000);
    }
    
    rfid.PICC_HaltA();
    showIdleScreen();
  }

  if (millis() - lastBotCheck > 10000) {
    checkTelegramUpdates();
    lastBotCheck = millis();
  }
}