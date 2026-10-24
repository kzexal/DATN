#include <TinyGPS++.h>

// =====================================================
// ĐỊNH NGHĨA CHÂN KẾT NỐI
// =====================================================
#define GPS_RX_PIN    16
#define GPS_TX_PIN    4

HardwareSerial SerialGPS(2);
TinyGPSPlus gps;

unsigned long lastPrint = 0;

void setup() {
  Serial.begin(115200);
  
  // Khởi tạo giao tiếp với module GPS
  SerialGPS.begin(9600, SERIAL_8N1, GPS_RX_PIN, GPS_TX_PIN);

  Serial.println("=========================================");
  Serial.println("=== KHOI DONG TEST GPS CHUYEN SAU =======");
  Serial.println("=========================================");
}

void loop() {
  // Đọc dữ liệu từ GPS và đẩy vào bộ giải mã
  while (SerialGPS.available() > 0) {
    gps.encode(SerialGPS.read());
  }

  // Cứ mỗi 2 giây, in ra báo cáo thống kê chi tiết một lần
  if (millis() - lastPrint > 2000) {
    Serial.println("\n--- BAO CAO DEBUG DIAGNOSTIC ---");
    
    // 1. Thống kê đường truyền vật lý (UART)
    Serial.print("Tong so byte da nhan : "); 
    Serial.println(gps.charsProcessed());
    
    Serial.print("Cau NMEA hop le      : "); 
    Serial.println(gps.passedChecksum());
    
    Serial.print("Cau NMEA loi (Failed): "); 
    Serial.println(gps.failedChecksum());

    // 2. Thống kê kết nối vệ tinh
    Serial.print("So ve tinh dang thay : "); 
    Serial.println(gps.satellites.value());
    
    // HDOP (Horizontal Dilution of Precision) - Độ suy giảm độ chính xác theo phương ngang
    // Giá trị < 1: Lý tưởng | 1-2: Tuyệt vời | 2-5: Tốt | > 5: Kém (sẽ sai số nhiều)
    Serial.print("Do nhieu tin hieu HDOP: "); 
    if (gps.hdop.isValid()) {
      Serial.println(gps.hdop.hdop());
    } else {
      Serial.println("Chua xac dinh");
    }

    // 3. Trạng thái chốt tọa độ
    if (gps.location.isValid()) {
      Serial.print(">> TOA DO HIEN TAI   : Lat: ");
      Serial.print(gps.location.lat(), 6);
      Serial.print(" | Lng: ");
      Serial.println(gps.location.lng(), 6);
    } else {
      Serial.println(">> TRANG THAI        : Dang tim kiem vi tri (No Fix)...");
    }
    
    lastPrint = millis();
  }
}