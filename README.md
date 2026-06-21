# Đồ án Tốt nghiệp: Thiết kế thiết bị giám sát học sinh lên xuống xe buýt đưa đón

Tài liệu này tổng hợp toàn bộ thông tin về phần cứng, phần mềm, cấu trúc thư mục của dự án và đề cương chi tiết báo cáo tốt nghiệp. Đây là cẩm nang giúp bạn nắm vững kiến thức hệ thống phục vụ cho quá trình viết báo cáo và bảo vệ trước hội đồng.

---

## 📌 1. Giới thiệu Tổng quan Đề tài

Đề tài **"Thiết kế thiết bị giám sát học sinh lên xuống xe buýt đưa đón"** là một giải pháp IoT toàn diện nhằm đảm bảo an toàn cho học sinh và hỗ trợ nhà trường, phụ huynh quản lý hành trình đưa đón học sinh một cách tự động, minh bạch.

### 🌟 Tính năng cốt lõi của thiết bị:
- **Giám sát lên/xuống xe (RFID):** Nhận diện từng học sinh qua thẻ RFID cá nhân khi lên và xuống xe buýt.
- **Xác định vị trí và hành trình (GPS):** Module GPS liên tục cập nhật kinh độ, vĩ độ, tốc độ di chuyển và hướng đi của xe buýt đưa đón.
- **Cảnh báo tức thời qua 4G LTE:** Sử dụng module SIM 4G gửi thông báo trạng thái điểm danh kèm tọa độ vị trí (dưới dạng liên kết Google Maps) về máy điện thoại của phụ huynh/nhà trường thông qua Telegram Bot hoặc tin nhắn SMS, thực hiện cuộc gọi khẩn cấp khi cần thiết.
- **Hiển thị giao diện trực quan:** OLED hiển thị danh tính học sinh vừa quẹt thẻ, trạng thái sóng mạng 4G, trạng thái định vị GPS và chỉ số vệ tinh.
- **Báo động bằng âm thanh:** Còi Buzzer phát âm thanh phản hồi xác nhận quẹt thẻ thành công hoặc cảnh báo khi phát hiện thẻ không hợp lệ.

---

## 🛠️ 2. Kiến trúc Phần cứng (Hardware Architecture)

Dự án được thiết kế chuyên nghiệp trên phần mềm **Altium Designer** (nằm trong thư mục [datn_Project](file:///c:/Users/buidu/OneDrive/M%C3%A1y%20t%C3%ADnh/datn/datn_Project)).

### 🔌 Sơ đồ khối các linh kiện chính
1. **Khối điều khiển trung tâm (MCU):** ESP32-WROOM-32D chịu trách nhiệm xử lý logic chính, đọc thẻ RFID, giao tiếp UART với module SIM/GPS, điều khiển hiển thị OLED và còi báo.
2. **Khối định vị (GPS):** Module GPS (NEO-6M hoặc NEO-M8N) giao tiếp qua UART để lấy dữ liệu tọa độ Latitude, Longitude thời gian thực.
3. **Khối truyền thông di động (SIM 4G):** Module SIM 4G LTE (như dòng SIM7600G / SIM7600CE hoặc A7670C) hỗ trợ tập lệnh AT, kết nối Internet di động, tích hợp giao thức SSL/HTTPS để truyền tin nhắn Telegram qua HTTPS.
4. **Khối nhận dạng học sinh (RFID):** Đầu đọc thẻ RFID RC522 (tần số 13.56MHz) giao tiếp qua chuẩn SPI để đọc mã định danh (UID) của thẻ học sinh.
5. **Khối hiển thị (Display):** Màn hình OLED SH1106 (1.3 inch, I2C) để hiển thị thông tin học sinh, trạng thái kết nối và số vệ tinh bắt được.
6. **Khối cảnh báo (Alarm):** Còi Buzzer (Active Buzzer) phát tín hiệu âm thanh báo hiệu trạng thái quẹt thẻ.
7. **Khối nguồn (Power Supply):** Mạch hạ áp Buck (sử dụng IC nguồn chuyên dụng như MP1584/LM2596) hạ từ điện áp 9V - 24V (nguồn ắc quy xe) xuống 5V và 3.3V cấp cho vi điều khiển và các module. Module SIM 4G yêu cầu dòng đỉnh lớn (lên tới 2A), do đó mạch nguồn cần được thiết kế tản nhiệt tốt và lọc nhiễu tụ điện đầy đủ.
8. **Khối nạp chương trình:** Cổng nạp USB Type-C tích hợp chip chuyển đổi USB-to-UART (CP2102 hoặc CH340).

### 📍 Sơ đồ kết nối chân (Pin Mapping) trên ESP32

Dựa trên cấu hình trong mã nguồn kiểm tra:

| Linh kiện | Chân trên Module | Chân kết nối ESP32 | Loại giao tiếp / Ghi chú |
| :--- | :--- | :--- | :--- |
| **Màn hình OLED** | SDA | **GPIO 22** | I2C (SH1106, Địa chỉ `0x3C`) |
| | SCL | **GPIO 23** | I2C |
| **Module GPS** | TX (GPS) | **GPIO 16 (RX2)** | UART (Khởi tạo `HardwareSerial SerialGPS(2)`) |
| | RX (GPS) | **GPIO 4 (TX2)** | UART (Nối qua trở đệm) |
| **Module SIM 4G**| TX (SIM) | **GPIO 27 (RX1)** | UART (Khởi tạo `HardwareSerial SerialSIM(1)`) |
| | RX (SIM) | **GPIO 26 (TX1)** | UART |
| **Đầu đọc RFID** | MISO | **GPIO 19** | SPI tiêu chuẩn (Dành cho RC522) |
| *(Dự kiến)* | MOSI | **GPIO 23** | SPI tiêu chuẩn |
| | SCK | **GPIO 18** | SPI tiêu chuẩn |
| | SDA (SS) | **GPIO 5** | SPI / Chip Select |
| | RST | **GPIO 15** | Reset Pin |
| **Khối Còi / Buzzer**| Chân tín hiệu | **GPIO 12 (hoặc tùy chọn)**| Xuất mức HIGH kích còi |

---

## 📂 3. Cấu trúc Thư mục Dự án

Dưới đây là sơ đồ tổ chức file trong thư mục làm việc của bạn:

```text
datn/
├── data1.json                      # Cơ sở dữ liệu danh sách thẻ RFID và người dùng
├── testgps/
│   └── testgps.ino                 # Code kiểm tra định vị GPS và hiển thị OLED SH1106
├── testsim/
│   └── testsim.ino                 # Code kiểm tra module SIM 4G (SMS, Call, Telegram API)
├── code/
│   └── testgps/                    # Thư mục chứa mã nguồn dự phòng
└── datn_Project/                   # Thiết kế phần cứng trên Altium Designer
    ├── datn_Project.PrjPcb         # File quản lý project Altium
    ├── Datn.SchDoc                 # Sơ đồ nguyên lý mạch (Schematic)
    ├── Datn.PcbDoc                 # Sơ đồ mạch in PCB (Layout)
    ├── datn_Project.pdf            # Bản vẽ Schematic/PCB xuất PDF phục vụ báo cáo
    ├── *.PcbLib & *.SchLib         # Thư viện chân linh kiện (Sim, GPS, Coi, TypeC, Buck...)
    └── ...
```

---

## 💻 4. Phân tích Chi tiết Mã nguồn (Software Analysis)

- **Mã nguồn kiểm tra GPS (`testgps.ino`):** Đọc luồng dữ liệu thô (raw data NMEA) từ cổng `SerialGPS` (cổng UART2, baudrate `9600`) đưa vào giải mã `gps.encode()`. Khi tọa độ hợp lệ, hiển thị Vĩ độ (Latitude), Kinh độ (Longitude), số lượng vệ tinh lên màn hình OLED và gửi lên cổng Serial Monitor.
- **Mã nguồn kiểm tra SIM 4G (`testsim.ino`):** Điều khiển module SIM qua tập lệnh AT nhằm thực hiện cuộc gọi, gửi SMS và kết nối mạng di động LTE để gửi thông tin về Telegram Bot qua HTTPS. Tích hợp thuật toán đo sóng di động `getCSQ()`, lấy loại mạng `getNetworkType()`, kích hoạt PDP data context `activateData()`, ping test `pingTest()`, và mã hóa dữ liệu HTTPS gửi Telegram Bot `sendTelegram()`.

---

## 🗃️ 5. Cơ sở Dữ liệu Học sinh (`data1.json`)

Thiết bị quản lý danh sách học sinh thông qua mã định danh thẻ RFID (UID). File `data1.json` mô tả cách tổ chức dữ liệu:
- **Key (UID thẻ):** Dạng Hex 8 ký tự (ví dụ: `AA953207`, `9A22F906`).
- **Value (Thông tin học sinh):**
  - `name`: Tên học sinh.
  - `chat_id`: Chat ID Telegram của phụ huynh học sinh để gửi thông báo riêng biệt.
  - `phone`: Số điện thoại phụ huynh khi cần gửi SMS hoặc thực hiện cuộc gọi khẩn cấp.
  - `status`: Trạng thái lên/xuống xe hiện tại của học sinh (`in` - đang ở trên xe / `out` - đã xuống xe).

---

## 🔄 6. Sơ đồ Hoạt động của Thiết bị (Flowchart)

```mermaid
flowchart TD
    A([Khởi động thiết bị]) --> B[Khởi tạo OLED, GPS, SIM 4G, RFID]
    B --> C[Kiểm tra kết nối thẻ SIM & Đăng ký mạng di động]
    C --> D[Kiểm tra sóng GPS & Chờ chốt tọa độ vị trí]
    D --> E[Sẵn sàng: Hiển thị màn hình chờ trên OLED]
    E --> F{Có học sinh quẹt thẻ RFID?}
    F -- Không --> G[Cập nhật liên tục tọa độ vị trí xe buýt]
    G --> E
    F -- Có --> H[Đọc UID thẻ RFID]
    H --> I{UID học sinh tồn tại trong database JSON?}
    I -- Không --> J[Còi kêu cảnh báo dồn dập & OLED hiển thị: Thẻ không hợp lệ]
    J --> E
    I -- Có --> K[Cập nhật trạng thái Lên/Xuống xe (In/Out) trong database]
    K --> L[Lấy tọa độ GPS thời gian thực hiện tại của xe]
    L --> M[Hiển thị thông tin học sinh & trạng thái Lên/Xuống lên OLED]
    M --> N[Phát âm thanh bíp ngắn thành công qua còi]
    N --> O[Tạo tin nhắn kèm link vị trí Google Maps của xe buýt]
    O --> P[Gửi tin nhắn điểm danh (Lên/Xuống xe) đến phụ huynh qua Telegram Bot (4G LTE)]
    P --> Q{Truyền dữ liệu 4G thất bại / Mất mạng?}
    Q -- Có --> R[Chuyển hướng gửi tin nhắn SMS truyền thống]
    Q -- Không --> S[Hoàn thành quy trình quẹt thẻ]
    R --> S
    S --> E
```

---

## 📄 7. Đề cương Chi tiết Báo cáo Tốt nghiệp

*Dưới đây là cấu trúc đề cương phân bổ chi tiết các mục (1.1, 2.1, 3.1...) khớp hoàn hảo với danh mục chương yêu cầu của bạn, được tối ưu hóa cho đề tài giám sát xe buýt học sinh:*

### 📑 CHƯƠNG 1. TỔNG QUAN ĐỀ TÀI
* **1.1. Đặt vấn đề**
* **1.2. Đối tượng khách hàng**
* **1.3. Phương pháp thực hiện**
* **1.4. Mục tiêu của đề tài**
* **1.5. Các tiêu chí đánh giá hệ thống**
* **1.6. Kết luận chương**

---

### 📑 CHƯƠNG 2. THIẾT KẾ TỔNG QUAN HỆ THỐNG
* **2.1. Mô hình tổng quan về hệ thống**
* **2.2. Một số yêu cầu chung của hệ thống**
* **2.3. Yêu cầu chức năng các phân hệ chính**
  * *2.3.1. Phân hệ Device*
  * *2.3.2. Phân hệ Server – Backend*
  * *2.3.3. Phân hệ Client App*
* **2.4. Yêu cầu chức năng các phân hệ phụ**
  * *2.4.1. Phân hệ Định vị toàn cầu GPS*
  * *2.4.2. Phân hệ Cơ sở dữ liệu cục bộ*
* **2.5. Kết luận chương**

---

### 📑 CHƯƠNG 3. THIẾT KẾ VÀ TRIỂN KHAI PHẦN CỨNG
* **3.1. Sơ đồ khối tổng quan của phần cứng thiết bị (Device)**
* **3.2. Thiết kế sơ đồ nguyên lý mạch in (Schematic)**
  * *3.2.1. Cấu trúc dự án schematic trên Altium Designer*
  * *3.2.2. Thiết kế nguyên lý khối điều khiển trung tâm (MCU ESP32)*
  * *3.2.3. Thiết kế nguyên lý khối định vị toàn cầu (GPS ATGM336H)*
  * *3.2.4. Thiết kế nguyên lý khối truyền thông mạng di động (SIM A7680C 4G)*
  * *3.2.5. Thiết kế nguyên lý khối nhận dạng RFID và hiển thị OLED*
  * *3.2.6. Thiết kế nguyên lý khối cảnh báo (Buzzer)*
  * *3.2.7. Thiết kế nguyên lý khối nguồn cấp và hạ áp (Type-C & QS-1205CME-3A)*
* **3.3. Thiết kế bảng mạch in (PCB Layout)**
  * *3.3.1. Mạch PCB thiết kế ở chế độ 2D*
  * *3.3.2. Mạch PCB thiết kế ở chế độ 3D*
* **3.4. Thi công, hàn mạch in và hoàn thiện phần cứng thực tế**

### 📑 CHƯƠNG 4. THIẾT KẾ VÀ XÂY DỰNG PHẦN MỀM
* **4.1. Lưu đồ thuật toán tổng quát của hệ thống**
* **4.2. Khởi tạo môi trường lập trình và các thư viện sử dụng**
* **4.3. Triển khai các thuật toán xử lý chính**
  * *4.3.1. Thuật toán xử lý đa luồng (Multi-tasking) cho GPS*
  * *4.3.2. Thuật toán xử lý thẻ RFID và cơ sở dữ liệu Firebase*
  * *4.3.3. Thuật toán gửi thông báo kép dự phòng (Dual-Fallback)*
* **4.4. Triển khai cơ chế tương tác hai chiều (Telegram Bot)**
* **4.5. Giao diện người dùng trên Client App**
* **4.6. Kết luận chương**

---

### 📑 CHƯƠNG 5. KIỂM THỬ VÀ ĐÁNH GIÁ KẾT QUẢ
* **5.1. Kịch bản thực nghiệm và quy trình kiểm thử hệ thống**
* **5.2. Kết quả kiểm thử chế tạo phần cứng**
  * *5.2.1. Kết quả kiểm tra hoạt động của khối nguồn hạ áp dưới tải.*
  * *5.2.2. Đánh giá chất lượng gia công mạch in PCB thực tế.*
* **5.3. Kết quả thử nghiệm chức năng hệ thống**
  * *5.3.1. Kết quả nhận diện thẻ RFID và cập nhật trạng thái học sinh.*
  * *5.3.2. Kết quả bắt sóng vệ tinh định vị vị trí xe buýt trên đường.*
* **5.3.3. Kết quả thực tế gửi tin nhắn báo động qua Telegram và SMS Fallback.**
  * *5.3.4. Kết quả tương tác truy vấn vị trí từ xa (Lệnh "vitri").*
* **5.4. Đánh giá hiệu năng và phân tích sai số hệ thống**
  * *5.4.1. Đánh giá sai số định vị GPS (so sánh với GPS điện thoại).*
  * *5.4.2. Đánh giá độ trễ truyền dữ liệu điểm danh qua mạng di động 4G LTE.*
* **5.5. Nhận xét ưu điểm và nhược điểm của thiết bị**

---

### 📑 KẾT LUẬN VÀ HƯỚNG PHÁT TRIỂN
* **Kết luận:** Đánh giá mức độ hoàn thành so với mục tiêu ban đầu đặt ra.
* **Hướng phát triển đề tài:**
  * *Tích hợp khe cắm thẻ nhớ SD lưu trữ offline dữ liệu hành trình khi xe vào vùng mất sóng.*
  * *Ứng dụng thuật toán tối ưu hóa năng lượng của ESP32 (Deep Sleep).*
  * *Thiết kế ứng dụng di động riêng (App Mobile) quản lý trực quan thay vì qua Telegram Bot.*

---

## 📈 8. Kế hoạch Chuẩn bị trước khi Bảo vệ Đồ án
1. **Kiểm tra phần cứng:** Đảm bảo các mối hàn chắc chắn, không bị ngắn mạch đường nguồn. Cố định anten GPS và anten 4G hướng lên trên.
2. **Chuẩn bị SIM 4G:** Sử dụng SIM có đăng ký sẵn gói cước data tốc độ cao. Kiểm tra tài khoản thuê bao trước giờ demo.
3. **Cài đặt Telegram Bot:** Tạo sẵn 1 Group Telegram, thêm Bot vào và cấp quyền quản trị để thầy cô trong hội đồng có thể quét mã QR tham gia nhóm kiểm chứng tin nhắn nhảy về thời gian thực.
4. **Quay video demo dự phòng (Backup Video):** Hãy quay sẵn một clip chạy thực tế mạch chạy ngoài trời (định vị, quẹt thẻ và nhận tin nhắn Telegram). Đề phòng trường hợp phòng bảo vệ của trường bị che khuất sóng GPS/4G không thể demo trực tiếp được.
