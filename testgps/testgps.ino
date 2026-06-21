#include <TinyGPS++.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

// =====================================================
// ĐỊNH NGHĨA CHÂN KẾT NỐI
// =====================================================
#define GPS_RX_PIN    16
#define GPS_TX_PIN    4
#define I2C_SDA_PIN   22
#define I2C_SCL_PIN   23

HardwareSerial SerialGPS(2);
TinyGPSPlus gps;
Adafruit_SH1106G display = Adafruit_SH1106G(128, 64, &Wire, -1);

unsigned long lastPrint = 0;

// =====================================================
// HÀM HIỂN THỊ OLED
// =====================================================
void inOLED(String dong1, String dong2 = "", String dong3 = "", String dong4 = "") {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(0, 5);
  display.println(dong1);
  if(dong2 != "") { display.setCursor(0, 20); display.println(dong2); }
  if(dong3 != "") { display.setCursor(0, 35); display.println(dong3); }
  if(dong4 != "") { display.setCursor(0, 50); display.println(dong4); }
  display.display();
}

void setup() {
  Serial.begin(115200);
  
  // Khởi tạo GPS
  SerialGPS.begin(9600, SERIAL_8N1, GPS_RX_PIN, GPS_TX_PIN);

  // Khởi tạo OLED
  Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN); 
  display.begin(0x3c, true);

  Serial.println("=== KHOI DONG TEST GPS ===");
  inOLED("TEST MODULE GPS", "Dang cho tin hieu...");
}

void loop() {
  // Đọc dữ liệu thô từ GPS
  while (SerialGPS.available() > 0) {
    gps.encode(SerialGPS.read());
  }

  // Nếu chốt được tọa độ, cập nhật lên Serial và OLED mỗi giây
  if (gps.location.isValid()) {
    if (millis() - lastPrint > 1000) {
      String latStr = String(gps.location.lat(), 6);
      String lngStr = String(gps.location.lng(), 6);
      String satStr = "Ve tinh: " + String(gps.satellites.value());

      Serial.print("Lat: "); Serial.print(latStr);
      Serial.print(" | Lng: "); Serial.print(lngStr);
      Serial.print(" | "); Serial.println(satStr);

      inOLED("DA CHOT TOA DO!", "Lat: " + latStr, "Lng: " + lngStr, satStr);
      lastPrint = millis();
    }
  } else {
    // Nếu chưa có tọa độ hợp lệ nhưng có nhận được vệ tinh
    if (millis() - lastPrint > 2000) {
      Serial.print("Dang do tim... Ve tinh nhin thay: ");
      Serial.println(gps.satellites.value());
      inOLED("DANG DO TIM...", "Chua co toa do", "Ve tinh: " + String(gps.satellites.value()), "Mang mach ra ngoai troi!");
      lastPrint = millis();
    }
  }

  // Cảnh báo nếu mạch không giao tiếp được với module GPS (cắm sai dây RX/TX)
  if (millis() > 5000 && gps.charsProcessed() < 10) {
    Serial.println("LOI: Khong nhan duoc du lieu tu GPS. Kiem tra day RX/TX!");
    inOLED("LOI GIAO TIEP GPS", "Kiem tra day cap:", "TX (GPS) -> Chan 16", "RX (GPS) -> Chan 4");
    delay(2000);
  }
}