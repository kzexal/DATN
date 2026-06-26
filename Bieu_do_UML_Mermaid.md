# Tổng hợp Code Vẽ Biểu Đồ (PlantUML & Mermaid)

File này chứa các đoạn mã nguồn để vẽ biểu đồ cho đồ án. 
- Code **PlantUML** (`plantuml`): copy và dán vào [PlantText](https://www.planttext.com/) hoặc [PlantUML Web](https://plantuml.com/plantuml).
- Code **Mermaid** (`mermaid`): copy và dán vào [Mermaid Live Editor](https://mermaid.live/).

---

## 1. Biểu đồ Use Case (Ca sử dụng) - PlantUML

*Ghi chú: PlantUML hỗ trợ vẽ biểu đồ Use Case trực quan và chuẩn mực hơn Mermaid.*

```plantuml
@startuml
skinparam packageStyle rectangle

actor "Học Sinh" as HS
actor "Phụ Huynh" as PH
actor "Quản Trị Viên" as Admin

rectangle "Hệ thống Điểm danh Xe buýt IoT" {
  usecase "UC1: Quẹt thẻ điểm danh" as UC1
  usecase "UC2: Nhận thông báo qua Telegram" as UC2
  usecase "UC3: Truy vấn vị trí xe (lệnh 'vitri')" as UC3
  usecase "UC4: Quản lý CSDL Học sinh" as UC4
  usecase "UC5: Giám sát trạng thái In/Out" as UC5
  
  ' Sắp xếp các Use Case nằm ngang từ trái qua phải
  UC1 -[hidden]right-> UC2
  UC2 -[hidden]right-> UC3
  UC3 -[hidden]right-> UC4
  UC4 -[hidden]right-> UC5
}

' Đẩy các Actor xuống dưới các Use Case tương ứng
HS -up-> UC1
PH -up-> UC2
PH -up-> UC3
Admin -up-> UC4
Admin -up-> UC5

UC1 .> UC2 : <<include>>
@enduml
```

---

## 2. Biểu đồ Sequence (Tuần tự) - PlantUML

### 2.1 Sequence Diagram: Quẹt thẻ và nhận thông báo (UC1 & UC2)

Mô tả quá trình từ lúc học sinh quẹt thẻ trên xe buýt cho đến khi phụ huynh nhận được thông báo qua Telegram.

```plantuml
@startuml
autonumber
actor "Học sinh" as Student
participant "ESP32 (Trên xe)" as ESP32
participant "RFID RC522" as RFID
participant "Màn hình OLED" as OLED
database "Firebase RTDB" as Firebase
participant "Telegram API" as Telegram
actor "Phụ huynh" as Parent

Student -> RFID: Quẹt thẻ
activate RFID
RFID --> ESP32: Trả về mã thẻ (UID)
deactivate RFID

activate ESP32
ESP32 -> OLED: Hiển thị "Đang xử lý..."
ESP32 -> Firebase: HTTP GET (Truy vấn UID)
activate Firebase
Firebase --> ESP32: Trả về JSON (chat_id, name, status)
deactivate Firebase

alt Thẻ hợp lệ (Có dữ liệu trên Firebase)
    ESP32 -> ESP32: Đảo ngược trạng thái (in <-> out)
    ESP32 -> Firebase: HTTP PUT (Cập nhật trạng thái mới)
    
    ESP32 -> Telegram: Gọi API Bot (Gửi tin nhắn + link GPS)
    activate Telegram
    Telegram --> Parent: Hiển thị thông báo (Push Notification)
    deactivate Telegram
    
    ESP32 -> OLED: Hiển thị Tên + Trạng thái mới, phát tiếng Bíp
else Thẻ không hợp lệ
    ESP32 -> OLED: Hiển thị "The chua dang ky!"
end
deactivate ESP32
@enduml
```

### 2.2 Sequence Diagram: Truy vấn vị trí xe buýt (UC3)

Mô tả quá trình phụ huynh chủ động nhắn tin cho Bot để hỏi vị trí.

```plantuml
@startuml
autonumber
actor "Phụ huynh" as Parent
participant "Telegram Bot" as Telegram
participant "ESP32 (Trên xe)" as ESP32
participant "Module GPS" as GPS

Parent -> Telegram: Nhắn lệnh "vitri"
activate Telegram

activate ESP32
ESP32 -> Telegram: Gọi hàm getUpdates (Polling mỗi 10s)
Telegram --> ESP32: Trả về nội dung tin nhắn mới nhất
deactivate Telegram

opt Nội dung tin nhắn chứa "vitri"
    ESP32 -> GPS: Yêu cầu dữ liệu tọa độ mới nhất
    activate GPS
    GPS --> ESP32: Trả về Kinh độ, Vĩ độ
    deactivate GPS
    
    ESP32 -> ESP32: Khởi tạo liên kết Google Maps
    
    ESP32 -> Telegram: Gửi API trả lời tin nhắn kèm link
    activate Telegram
    Telegram --> Parent: Nhận tin nhắn chứa vị trí bản đồ
    deactivate Telegram
end
deactivate ESP32
@enduml
```

### 2.3 Sequence Diagram: Quản lý thông tin học sinh (UC4, UC5)

```plantuml
@startuml
autonumber
actor "Quản trị viên" as Admin
participant "Giao diện Web/App" as Web
database "Firebase RTDB" as Firebase

Admin -> Web: Nhập thông tin Học sinh (UID, Tên, Chat_ID)
activate Web
Web -> Firebase: POST/PUT (Lưu trữ dữ liệu)
activate Firebase
Firebase --> Web: Trả về kết quả thành công
deactivate Firebase
Web --> Admin: Thông báo "Thêm/Sửa thành công"
deactivate Web

Admin -> Web: Yêu cầu xem trạng thái hiện tại
activate Web
Web -> Firebase: GET (Danh sách user)
activate Firebase
Firebase --> Web: Trả về JSON danh sách trạng thái
deactivate Firebase
Web --> Admin: Hiển thị bảng giám sát In/Out
deactivate Web
@enduml
```

---

## 3. Các Sơ đồ Tổng quan - Mermaid

### 3.1 Sơ đồ Khối Kiến Trúc Hệ Thống (System Architecture Diagram)

Sơ đồ này biểu diễn các khối phần cứng kết nối với ESP32 và tương tác với hệ thống Cloud/App.

```mermaid
graph TD
    subgraph Hardware ["Khối Phần Cứng (Trên xe buýt)"]
        Power["Khối Nguồn\n(Cổng Type-C 5V & Công tắc)"]
        ESP[("Vi điều khiển\nESP32\n(Core 0 & Core 1)")]
        RFID["Đầu đọc thẻ RFID\n(RC522)"]
        GPS["Module định vị GPS\n(ATGM336H)"]
        OLED["Màn hình hiển thị\n(OLED 1.3 inch I2C)"]
        Buzzer["Còi báo\n(Active Buzzer 5V)"]
        
        Power -->|"Cấp nguồn 5V"| ESP
        RFID <-->|"SPI"| ESP
        GPS -->|"UART"| ESP
        ESP -->|"I2C"| OLED
        ESP -->|"GPIO"| Buzzer
    end

    subgraph Cloud ["Khối Đám mây (Cloud Server)"]
        Firebase[("Firebase\nRealtime Database")]
        TelegramAPI["Telegram Bot API"]
        Firebase ~~~ TelegramAPI
    end

    subgraph Client ["Khối Người Dùng (Client)"]
        Admin["Quản trị viên\n(Nhà trường)"]
        TelegramApp["Ứng dụng Telegram\n(Phụ huynh)"]
    end

    ESP -->|"1. Truy vấn & Cập nhật\nWi-Fi (HTTPS)"| Firebase
    Firebase -->|"2. Trả về thông tin (JSON)"| ESP
    ESP -->|"3. Gửi lệnh cảnh báo\nWi-Fi (HTTPS)"| TelegramAPI
    
    Firebase <-->|"Xem trực tiếp Console"| Admin
    TelegramAPI <-->|"Nhận thông báo"| TelegramApp
    
    style Hardware fill:#f9f2f4,stroke:#333,stroke-width:2px
    style Cloud fill:#e1f5fe,stroke:#333,stroke-width:2px
    style Client fill:#e8f5e9,stroke:#333,stroke-width:2px
```

### 3.2 Sơ đồ Luồng Thuật Toán ESP32 (Flowchart)

Lưu đồ thuật toán chạy song song (Multi-tasking) trên 2 nhân của ESP32.

```mermaid
flowchart TD
    Start([Bắt đầu khởi động ESP32]) --> Setup[1. Thiết lập: Wi-Fi, OLED, I2C, SPI, UART]
    Setup --> Split{Phân chia Luồng\n(Dual Core)}

    %% Core 0 - Xử lý GPS
    Split --> |Core 0| TaskGPS[Task: gpsTask]
    TaskGPS --> ReadGPS[Liên tục đọc dữ liệu tọa độ từ GPS ATGM336H]
    ReadGPS --> ParseGPS[Giải mã NMEA bằng TinyGPS++]
    ParseGPS --> DelayGPS[Delay 10ms]
    DelayGPS --> ReadGPS

    %% Core 1 - Xử lý Chính
    Split --> |Core 1| LoopMain[Vòng lặp chính: loop]
    LoopMain --> CheckWifi{2. Kiểm tra kết nối Wi-Fi}
    CheckWifi -- Mất kết nối --> Reconnect[Thử kết nối lại Wi-Fi]
    Reconnect --> CheckWifi
    
    CheckWifi -- Đã kết nối --> CheckCard{3. Quét thẻ RFID RC522?}
    CheckCard -- Có --> GetUID[Chuyển đổi sang chuỗi UID HEX]
    GetUID --> FirebaseGet[4. Gửi HTTP GET Firebase lấy thông tin]
    FirebaseGet --> ParseJSON[Trích xuất Tên, Chat_ID, Trạng thái In/Out]
    ParseJSON --> FirebasePut[Gửi HTTP PUT cập nhật trạng thái đảo ngược]
    FirebasePut --> TeleSend[5. Gửi tin nhắn Telegram kèm Link Map]
    TeleSend --> Beep[Báo còi Buzzer & Cập nhật OLED]
    Beep --> TelePoll

    CheckCard -- Không --> TelePoll
    
    TelePoll[6. Polling API Telegram\nmỗi 10 giây] --> CheckMsg{Tin nhắn có lệnh 'vitri'?}
    CheckMsg -- Có --> SendLoc[Lấy tọa độ mới nhất từ Core 0\nGửi trả link Google Maps]
    SendLoc --> LoopMain
    CheckMsg -- Không --> LoopMain
```
