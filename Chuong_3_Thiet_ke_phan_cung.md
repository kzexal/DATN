# CHƯƠNG 3. THIẾT KẾ VÀ TRIỂN KHAI PHẦN CỨNG

Chương 3 trình bày chi tiết về quá trình thiết kế, triển khai phân hệ phần cứng (Device) từ việc xây dựng sơ đồ nguyên lý (Schematic), thiết kế mạch in (PCB Layout) trên phần mềm Altium Designer, cho đến quá trình gia công và hoàn thiện thiết bị trong thực tế.

## 3.1 Sơ đồ khối tổng quan của phần cứng thiết bị (Device)

 Phần cứng của thiết bị giám sát (Device) bao gồm các khối chức năng chính sau:
- **Khối điều khiển trung tâm (MCU):** Đóng vai trò đầu não, thu thập, tính toán logic và điều khiển mọi hoạt động của các khối ngoại vi. Ở đây sử dụng vi điều khiển ESP32 (module ESP32 DevKit V1 loại 30 chân) [8].
- **Khối nguồn (POWER) & Công tắc:** Chịu trách nhiệm cung cấp dòng điện ổn định 5V cho toàn hệ thống. Hệ thống linh hoạt sử dụng cổng Type-C Female để nhận nguồn 5V trực tiếp từ pin sạc dự phòng.
- **Khối định vị (GPS):** Thu thập dữ liệu vị trí không gian thông qua module vệ tinh ATGM336H [10].
- **Khối nhận dạng & Giao diện (RFID & OLED):** Sử dụng đầu đọc thẻ RFID RC522 (loại chân cắm đứng) [9] để nhận diện học sinh và màn hình OLED 1.3 inch (giao tiếp I2C) để hiển thị thông tin phản hồi tại chỗ.
- **Khối cảnh báo (ALERT):** Bao gồm còi chíp chủ động (Active Buzzer 5V) dùng để phát âm thanh cảnh báo trạng thái.

## 3.2 Thiết kế sơ đồ nguyên lý mạch in (Schematic)

Nguyên lý mạch in của phần cứng được thiết kế trên phần mềm thiết kế mạch điện tử chuyên dụng Altium Designer.

### 3.2.1 Cấu trúc dự án schematic trên Altium Designer

Dự án phần cứng được tổ chức trong thư mục `datn_Project` với tệp quản lý chính là `datn_Project.PrjPcb`. 
Mạch nguyên lý được thiết kế tập trung trên tệp `Datn.SchDoc`. Để tối ưu hóa việc tái sử dụng và quản lý linh kiện, mỗi linh kiện ngoại vi đều được phân tách thành các thư viện Schematic Library (`.SchLib`) và PCB Library (`.PcbLib`) riêng biệt như: `Coi.SchLib`, `GPS.SchLib`, `TypeC.SchLib`...

### 3.2.2 Thiết kế nguyên lý khối điều khiển trung tâm (MCU ESP32)

Module ESP32 DevKit V1 (30 chân) được sử dụng làm vi điều khiển chính [8]. Nguồn điện cấp cho ESP32 là 5V (vào chân Vin) và 3.3V (cấp trực tiếp vào nhân xử lý). 
Khối MCU giao tiếp với các thiết bị ngoại vi thông qua việc phân bổ các chân GPIO chức năng như sau:
- **Chuẩn giao tiếp I2C (Màn hình OLED 1.3 inch):** Sử dụng GPIO 22 (SDA) và GPIO 23 (SCL).
- **Chuẩn giao tiếp SPI (Module RFID RC522):** Sử dụng GPIO 19 (SCK), GPIO 5 (MISO), GPIO 18 (MOSI), GPIO 21 (SS/SDA), và GPIO 17 (RST).
- **Chuẩn giao tiếp UART:**
  - UART2 (Module GPS ATGM336H): Sử dụng GPIO 16 (RX) và GPIO 4 (TX).
- **Điều khiển GPIO mức Logic:** Chân GPIO 13 được cấu hình làm ngõ ra tín hiệu số để điều khiển còi Buzzer.

Để trực quan hóa toàn bộ sơ đồ đấu nối giữa ESP32 và các module chức năng của hệ thống, chi tiết phân bổ chân được tổng hợp trong bảng dưới đây:

#### **Bảng 3.1: Sơ đồ kết nối chân giữa ESP32 và các module chức năng**

| STT | Chân trên ESP32 | Kết nối với Module | Chân tương ứng trên Module | Giao thức / Chức năng |
| :---: | :--- | :--- | :--- | :--- |
| 1 | **3.3V** | Module RFID RC522 | VCC (3.3V) | Cấp nguồn điện áp 3.3V (Bắt buộc) |
| 2 | **GND** | Tất cả các Module | GND | Nối đất chung (Common Ground) |
| 3 | **GPIO 22** | Màn hình OLED 1.3 inch | SDA | Đường truyền dữ liệu I2C |
| 4 | **GPIO 23** | Màn hình OLED 1.3 inch | SCL | Đường cấp xung nhịp I2C |
| 5 | **GPIO 19** | Module RFID RC522 | SCK | Đường cấp xung nhịp SPI |
| 6 | **GPIO 5** | Module RFID RC522 | MISO | Giao tiếp SPI (Master In Slave Out) |
| 7 | **GPIO 18** | Module RFID RC522 | MOSI | Giao tiếp SPI (Master Out Slave In) |
| 8 | **GPIO 21** | Module RFID RC522 | SDA (SS) | Chọn chip ngoại vi SPI (Slave Select) |
| 9 | **GPIO 17** | Module RFID RC522 | RST | Thiết lập lại module (Reset) |
| 10 | **GPIO 16 (RX2)** | Module GPS ATGM336H | TXD (TX) | Nhận dữ liệu tọa độ GPS (UART) |
| 11 | **GPIO 4 (TX2)** | Module GPS ATGM336H | RXD (RX) | Gửi lệnh cấu hình định vị (UART) |
| 12 | **GPIO 27 (RX1)** | Module SIM A7680C | TXD (TX) | Nhận dữ liệu phản hồi từ module SIM (UART) |
| 13 | **GPIO 26 (TX1)** | Module SIM A7680C | RXD (RX) | Gửi lệnh AT điều khiển SIM (UART) |
| 14 | **GPIO 13** | Khối cảnh báo (Buzzer) | Chân I/O | Xuất mức logic điều khiển còi (kích hoạt mức thấp - Active Low) |


### 3.2.3 Thiết kế nguyên lý khối định vị toàn cầu (GPS ATGM336H)

Mô-đun GPS ATGM336H có hiệu năng cao, tích hợp sẵn ăng-ten để thu tín hiệu vệ tinh [10].
- **Giao tiếp MCU:** Thông qua giao thức UART nối chéo (RX của GPS nối với TX2 GPIO4 của ESP32, TX của GPS nối với RX2 GPIO16 của ESP32).



### 3.2.4 Thiết kế nguyên lý khối nhận dạng RFID và hiển thị OLED

- **Module RFID RC522:** Chân cấp nguồn của RC522 nối vào đường điện áp 3.3V của ESP32. Tín hiệu được truyền tải tốc độ cao qua chuẩn SPI [9].
- **Màn hình OLED 1.3 inch:** Yêu cầu 4 chân kết nối (VCC, GND, SCL, SDA), sử dụng chuẩn giao tiếp I2C tiện lợi.

### 3.2.5 Thiết kế nguyên lý khối cảnh báo (Buzzer)

Còi chíp loại chủ động được sử dụng để phát âm thanh cảnh báo. Do dòng điện cấp trực tiếp từ chân GPIO của ESP32 (mức 3.3V) thường không đủ để đảm bảo còi hoạt động ổn định và bền bỉ, module còi được tích hợp sẵn một mạch đệm khuếch đại dòng bên trong, giúp còi kêu ổn định ngay khi nhận tín hiệu điều khiển từ GPIO 13 mà không cần bổ sung thêm linh kiện khuếch đại trên mạch chính.

### 3.2.6 Thiết kế nguyên lý khối nguồn cấp

Phần nguồn cấp được thiết kế linh hoạt cho thiết bị.
- **Nguồn cấp chính:** Sử dụng cổng **Type-C Female** cắm trực tiếp cáp sạc 5V từ sạc dự phòng hoặc nguồn 5V trên xe. Thiết kế này cung cấp tính cơ động rất cao khi mang thiết bị ra ngoài đo đạc tín hiệu vệ tinh GPS.
- **Lọc nhiễu:** Đường nguồn 5V chính được bố trí thêm các tụ điện để lọc nhiễu tần số cao. Nguồn 5V này sau đó phân phối tới ESP32, GPS và OLED.

## 3.3 Thiết kế bảng mạch in (PCB Layout)

Sau khi thiết kế xong sơ đồ nguyên lý mạch (Schematic), chuyển đổi sang PCB layout trên phần mềm Altium Designer thông qua tệp `Datn.PcbDoc`.

### 3.3.1 Mạch PCB thiết kế ở chế độ 2D

- **Sắp xếp linh kiện (Placement):** Vi điều khiển ESP32 được đặt ở vị trí trung tâm. Module GPS ATGM336H được bố trí hợp lý để tối ưu thu phát tín hiệu. Cổng Type-C và công tắc gạt được đưa ra viền bo mạch để dễ thao tác cắm rút.
- **Quy tắc đi dây (Routing):** 
  - Đi dây 2 lớp (Top/Bottom).
  - Các đường mạch điện tín hiệu I2C, SPI, UART kích thước nhỏ.
  - Phủ mass (Copper Pour GND) kín bo mạch để hấp thụ nhiễu tần số cao (EMI).

### 3.3.2 Mạch PCB thiết kế ở chế độ 3D

Trên Altium Designer, các linh kiện cắm (THT) và các module dán (như ESP32 30 chân, GPS, OLED) được áp các mô hình 3D thực tế (File `.STEP`). Bản vẽ 3D giúp kiểm tra trực quan khoảng cách thò ra của cổng Type-C, góc cắm thẻ RFID RC522, đảm bảo thiết bị có thể lắp ráp hoàn hảo vào vỏ hộp bảo vệ.

## 3.4 Thi công, hàn mạch in và hoàn thiện phần cứng thực tế

**Gia công mạch in (Fabrication):**
Xuất tệp tin Gerber từ Altium và gửi xưởng gia công PCB (chất liệu FR4, phủ sơn Solder Mask).

- **Quá trình hàn linh kiện (Soldering & Assembly):**
- **Trình tự hàn:** Hàn các tụ gốm 104, điện trở trước. Sau đó hàn cổng Type-C Female và công tắc gạt. Cuối cùng, hàn rào cắm (Header) để cắm các module ESP32, GPS, OLED, RFID RC522 và module còi chíp.
- Việc sử dụng đế cắm rào cái (Female Header) cho các module giúp dễ dàng tháo lắp, thay thế khi có hỏng hóc trong quá trình thử nghiệm đồ án.

**Đóng vỏ hộp và hoàn thiện (Casing):**
- Trong quá trình bảo vệ đồ án trước hội đồng và thử nghiệm, hệ thống được cấp nguồn rất nhanh chóng và an toàn từ một viên pin sạc dự phòng thông qua cáp USB Type-C.
- Mạch thành phẩm được lắp cố định vào hộp bảo vệ. Anten GPS được đi dây gọn gàng, hướng bắt sóng tốt nhất để đạt độ ổn định cao nhất khi thao tác thực tế.

---

## 3.5 Kết luận chương

Chương 3 đã trình bày toàn bộ quá trình hiện thực hóa phần cứng của thiết bị trạm (Device). Từ việc xác định sơ đồ khối hệ thống, lựa chọn linh kiện, phân bổ chân GPIO trên vi điều khiển ESP32, thiết kế sơ đồ nguyên lý và layout mạch in 2 lớp trên phần mềm Altium Designer, cho đến các giai đoạn thi công cơ khí như gia công PCB, hàn lắp linh kiện và đóng hộp bảo vệ. Bo mạch thành phẩm sau khi hoàn thiện đảm bảo các tiêu chuẩn về mặt thẩm mỹ, kích thước nhỏ gọn, chống can nhiễu điện từ và tối ưu nguồn điện cấp qua cổng Type-C. Đây là nền tảng vật lý vững chắc để nhóm nghiên cứu tiếp tục xây dựng các thuật toán nhúng điều khiển thông minh và thiết lập truyền thông thời gian thực ở chương tiếp theo.

