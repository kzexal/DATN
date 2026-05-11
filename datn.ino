#include <SPI.h>
#include <MFRC522.h>
#include <TinyGPS++.h>
#include <HardwareSerial.h>
#include <U8g2lib.h>
#include <Wire.h>
#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include <HTTPClient.h>

// 1. CẤU HÌNH THÔNG TIN KẾT NỐI (Bạn điền vào đây)
#define WIFI_SSID "17_501"
#define WIFI_PASSWORD "nopassdau101"
#define FIREBASE_API_KEY "" // Lấy từ Firebase Project Settings
#define FIREBASE_DATABASE_URL ""

// 2. CẤU HÌNH TELEGRAM (Đã điền theo yêu cầu của bạn)
const String botToken = "";
const String chatID = "";

// 3. KHAI BÁO CHÂN LINH KIỆN
#define RST_PIN    4
#define SS_PIN     5
#define BUZZER_PIN 25
#define GPS_RX     16
#define GPS_TX     17

// 4. KHỞI TẠO ĐỐI TƯỢNG
MFRC522 mfrc522(SS_PIN, RST_PIN);
TinyGPSPlus gps;
HardwareSerial SerialGPS(2);
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE, /* clock=*/ 22, /* data=*/ 21);

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

// Biến lưu trữ trạng thái GPS
double currentLat = 0;
double currentLng = 0;

void setup() {
  Serial.begin(115200);
  SerialGPS.begin(9600, SERIAL_8N1, GPS_RX, GPS_TX);
  
  // Khởi tạo phần cứng
  SPI.begin();
  mfrc522.PCD_Init();
  pinMode(BUZZER_PIN, OUTPUT);
  u8g2.begin();
  
  hienThiManHinh("DANG KET NOI...", "Vui long cho");

  // Kết nối WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");

  // Cấu hình Firebase
  config.api_key = FIREBASE_API_KEY;
  config.database_url = FIREBASE_DATABASE_URL;
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  hienThiManHinh("HUST - SAN SANG", "Hay quet the...");
  beep(100);
}

void loop() {
  // Cập nhật dữ liệu GPS liên tục
  while (SerialGPS.available() > 0) {
    gps.encode(SerialGPS.read());
  }

  // Lưu tọa độ nếu GPS bắt được tín hiệu
  if (gps.location.isValid()) {
    currentLat = gps.location.lat();
    currentLng = gps.location.lng();
  }

  // Kiểm tra quẹt thẻ RFID
  if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  // 1. Lấy mã UID
  String uidStr = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    uidStr += String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : "");
    uidStr += String(mfrc522.uid.uidByte[i], HEX);
  }
  uidStr.toUpperCase();

  // 2. Phản hồi tức thì (Còi + OLED)
  beep(200);
  hienThiManHinh("QUET THANH CONG", uidStr);

  // 3. Gửi dữ liệu lên Cloud
  guiDuLieuCloud(uidStr);

  // Dừng đọc thẻ
  mfrc522.PICC_HaltA();
  delay(2000);
  hienThiManHinh("HUST - SAN SANG", "Hay quet the...");
}

// --- HÀM HỖ TRỢ ---

void beep(int duration) {
  tone(BUZZER_PIN, 2500);
  delay(duration);
  noTone(BUZZER_PIN);
}

void hienThiManHinh(String line1, String line2) {
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.drawStr(0, 20, line1.c_str());
  u8g2.setFont(u8g2_font_6x10_tf);
  u8g2.drawStr(0, 45, line2.c_str());
  u8g2.sendBuffer();
}

void guiDuLieuCloud(String uid) {
  // Đẩy lên Firebase
  String path = "/Log_QuetThe/" + String(millis());
  FirebaseJson json;
  json.add("UID", uid);
  json.add("Lat", currentLat);
  json.add("Lng", currentLng);
  json.add("Status", "Success");
  
  if (Firebase.RTDB.setJSON(&fbdo, path.c_str(), &json)) {
    Serial.println("Firebase: OK");
  }

  // Gửi Telegram
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String message = "Thong bao tu ESP32:\n- Co nguoi quet the!\n- UID: " + uid;
    if (currentLat != 0) {
      message += "\n- Vi tri: https://www.google.com/maps?q=" + String(currentLat, 6) + "," + String(currentLng, 6);
    } else {
      message += "\n- Vi tri: Dang cap nhat GPS...";
    }

    String url = "https://api.telegram.org/bot" + botToken + "/sendMessage?chat_id=" + chatID + "&text=" + urlEncode(message);
    http.begin(url);
    int httpCode = http.GET();
    if (httpCode > 0) Serial.println("Telegram: OK");
    http.end();
  }
}

// Hàm mã hóa URL để gửi tin nhắn Telegram không bị lỗi ký tự đặc biệt
String urlEncode(String str) {
  String encodedString = "";
  char c;
  char code0;
  char code1;
  for (int i = 0; i < str.length(); i++) {
    c = str.charAt(i);
    if (isalnum(c)) {
      encodedString += c;
    } else {
      code1 = (c & 0xf) + '0';
      if ((c & 0xf) > 9) code1 = (c & 0xf) - 10 + 'A';
      c = (c >> 4) & 0xf;
      code0 = c + '0';
      if (c > 9) code0 = c - 10 + 'A';
      encodedString += '%';
      encodedString += code0;
      encodedString += code1;
    }
  }
  return encodedString;
}