# CHƯƠNG 3. THIẾT KẾ VÀ TRIỂN KHAI PHẦN CỨNG

Chương 3 trình bày chi tiết về quá trình thiết kế, triển khai phân hệ phần cứng (Device) từ việc xây dựng sơ đồ nguyên lý (Schematic), thiết kế mạch in (PCB Layout) trên phần mềm Altium Designer, cho đến quá trình gia công và hoàn thiện thiết bị trong thực tế.

## 3.1 Sơ đồ khối tổng quan của phần cứng thiết bị (Device)

Phần cứng của thiết bị giám sát (Device) bao gồm các khối chức năng chính sau:
- **Khối điều khiển trung tâm (MCU):** Đóng vai trò đầu não, thu thập, tính toán logic và điều khiển mọi hoạt động của các khối ngoại vi. Ở đây sử dụng vi điều khiển ESP32 (module ESP32 DevKit V1 loại 30 chân).
- **Khối nguồn (POWER) & Công tắc:** Chịu trách nhiệm cung cấp dòng điện ổn định 5V cho toàn hệ thống. Trong giai đoạn thử nghiệm đồ án, hệ thống linh hoạt sử dụng cổng Type-C Female để nhận nguồn 5V trực tiếp từ pin sạc dự phòng. Trong điều kiện triển khai thực tế trên xe buýt, hệ thống lấy điện 12V-24V từ bình ắc-quy đi qua công tắc gạt, sau đó qua mạch hạ áp mini QS-1205CME-3A để chuyển đổi thành 5V.
- **Khối định vị (GPS):** Thu thập dữ liệu vị trí không gian thông qua module vệ tinh ATGM336H.
- **Khối truyền thông (COMMUNICATE):** Sử dụng module SIM A7680C 4G để kết nối mạng viễn thông băng thông rộng, thực hiện tác vụ gửi tin nhắn SMS khẩn cấp.
- **Khối nhận dạng & Giao diện (RFID & OLED):** Sử dụng đầu đọc thẻ RFID RC522 (loại chân cắm đứng) để nhận diện học sinh và màn hình OLED 1.3 inch (giao tiếp I2C) để hiển thị thông tin phản hồi tại chỗ.
- **Khối cảnh báo (ALERT):** Bao gồm còi chíp chủ động (Active Buzzer 5V) dùng để phát âm thanh cảnh báo trạng thái.

## 3.2 Thiết kế sơ đồ nguyên lý mạch in (Schematic)

Nguyên lý mạch in của phần cứng được thiết kế trên phần mềm thiết kế mạch điện tử chuyên dụng Altium Designer.

### 3.2.1 Cấu trúc dự án schematic trên Altium Designer

Dự án phần cứng được tổ chức trong thư mục `datn_Project` với tệp quản lý chính là `datn_Project.PrjPcb`. 
Mạch nguyên lý được thiết kế tập trung trên tệp `Datn.SchDoc`. Để tối ưu hóa việc tái sử dụng và quản lý linh kiện, mỗi linh kiện ngoại vi đều được phân tách thành các thư viện Schematic Library (`.SchLib`) và PCB Library (`.PcbLib`) riêng biệt như: `Coi.SchLib`, `GPS.SchLib`, `Mach_ha_ap.SchLib`, `Sim.SchLib`, `TypeC.SchLib`... 

### 3.2.2 Thiết kế nguyên lý khối điều khiển trung tâm (MCU ESP32)

Module ESP32 DevKit V1 (30 chân) được sử dụng làm vi điều khiển chính. Nguồn điện cấp cho ESP32 là 5V (vào chân Vin) và 3.3V (cấp trực tiếp vào nhân xử lý). 
Khối MCU giao tiếp với các thiết bị ngoại vi thông qua việc phân bổ các chân GPIO chức năng như sau:
- **Chuẩn giao tiếp I2C (Màn hình OLED 1.3 inch):** Sử dụng GPIO 22 (SDA) và GPIO 23 (SCL).
- **Chuẩn giao tiếp SPI (Module RFID RC522):** Sử dụng GPIO 19 (SCK), GPIO 5 (MISO), GPIO 18 (MOSI), GPIO 21 (SS/SDA), và GPIO 17 (RST).
- **Chuẩn giao tiếp UART:**
  - UART1 (Module SIM A7680C): Sử dụng GPIO 27 (RX) và GPIO 26 (TX).
  - UART2 (Module GPS ATGM336H): Sử dụng GPIO 16 (RX) và GPIO 4 (TX).
- **Điều khiển GPIO mức Logic:** Chân GPIO 13 được cấu hình làm ngõ ra tín hiệu số để điều khiển còi Buzzer.

### 3.2.3 Thiết kế nguyên lý khối định vị toàn cầu (GPS ATGM336H)

Mô-đun GPS ATGM336H có hiệu năng cao, tích hợp sẵn ăng-ten để thu tín hiệu vệ tinh.
- **Giao tiếp MCU:** Thông qua giao thức UART nối chéo (RX của GPS nối với TX2 GPIO4 của ESP32, TX của GPS nối với RX2 GPIO16 của ESP32).

### 3.2.4 Thiết kế nguyên lý khối truyền thông di động (Module SIM A7680C 4G)

Mô-đun SIM A7680C hỗ trợ mạng 4G LTE tốc độ cao, được kết nối để làm nhiệm vụ viễn thông dự phòng (Gửi SMS).
- **Điện áp cấp:** +5V. Module SIM có đặc thù tiêu thụ dòng điện đỉnh rất lớn khi phát sóng RF, do đó đường nguồn 5V cấp cho module SIM được bố trí một tụ hóa lớn **1000uF (10V-25V)** mắc song song để ổn định dòng điện đỉnh tức thời.
- **Giao tiếp MCU:** Kết nối chéo UART (RX nối TX1 GPIO26, TX nối RX1 GPIO27 của ESP32).

### 3.2.5 Thiết kế nguyên lý khối nhận dạng RFID và hiển thị OLED

- **Module RFID RC522:** Chân cấp nguồn của RC522 nối vào đường điện áp 3.3V của ESP32. Tín hiệu được truyền tải tốc độ cao qua chuẩn SPI.
- **Màn hình OLED 1.3 inch:** Yêu cầu 4 chân kết nối (VCC, GND, SCL, SDA), sử dụng chuẩn giao tiếp I2C tiện lợi.

### 3.2.6 Thiết kế nguyên lý khối cảnh báo (Buzzer)

Còi chíp loại chủ động (Active Buzzer 5V) được sử dụng để phát âm thanh. Do cổng GPIO 13 của ESP32 chỉ xuất mức áp 3.3V không đủ khả năng kích trực tiếp còi Buzzer, mạch nguyên lý sử dụng một mạch đệm khuếch đại dòng bằng Transistor NPN (như C1815) để kích hoạt còi kêu khi GPIO 13 xuất mức HIGH.

### 3.2.7 Thiết kế nguyên lý khối nguồn cấp và hạ áp

Phần nguồn cấp được thiết kế tối ưu linh hoạt cho cả hai môi trường: môi trường thử nghiệm và môi trường thực tế.
- **Nguồn thử nghiệm (Test mode):** Sử dụng cổng **Type-C Female** cắm trực tiếp cáp sạc 5V từ sạc dự phòng. Thiết kế này cung cấp tính cơ động rất cao khi mang thiết bị ra ngoài trời đo đạc tín hiệu vệ tinh GPS và sóng 4G trong giai đoạn làm đồ án.
- **Nguồn thực tế (Real-world mode):** Đầu nối lấy điện từ bình ắc quy xe buýt đi qua một **Công tắc gạt bập bênh**, sau đó đi vào mô-đun **Mạch hạ áp mini 3A QS-1205CME-3A**. Mạch này sẽ hạ dải điện áp cao xuống mức 5V DC chuẩn.
- **Lọc nhiễu:** Đường nguồn 5V chính được bố trí thêm các tụ điện để lọc nhiễu, bao gồm **01 tụ hóa 47uF** lọc nguồn thứ cấp và **02 tụ gốm 104 (100nF)** để lọc nhiễu tần số cao phát ra từ các module viễn thông. Nguồn 5V này sau đó phân phối tới ESP32, SIM, GPS và OLED.

## 3.3 Thiết kế bảng mạch in (PCB Layout)

Sau khi thiết kế xong sơ đồ nguyên lý mạch (Schematic), chuyển đổi sang PCB layout trên phần mềm Altium Designer thông qua tệp `Datn.PcbDoc`.

### 3.3.1 Mạch PCB thiết kế ở chế độ 2D

- **Sắp xếp linh kiện (Placement):** Vi điều khiển ESP32 được đặt ở vị trí trung tâm. Module SIM A7680C và mạch nguồn hạ áp QS-1205CME-3A được bố trí cách xa module GPS ATGM336H để giảm thiểu can nhiễu điện từ. Cổng Type-C và công tắc gạt được đưa ra viền bo mạch để dễ thao tác cắm rút.
- **Quy tắc đi dây (Routing):** 
  - Đi dây 2 lớp (Top/Bottom).
  - Các đường mạch điện tín hiệu I2C, SPI, UART kích thước nhỏ.
  - Phủ mass (Copper Pour GND) kín bo mạch để hấp thụ nhiễu tần số cao (EMI).

### 3.3.2 Mạch PCB thiết kế ở chế độ 3D

Trên Altium Designer, các linh kiện cắm (THT) và các module dán (như ESP32 30 chân, SIM A7680C, GPS, OLED) được áp các mô hình 3D thực tế (File `.STEP`). Bản vẽ 3D giúp kiểm tra trực quan khoảng cách thò ra của cổng Type-C, góc cắm thẻ RFID RC522, đảm bảo thiết bị có thể lắp ráp hoàn hảo vào vỏ hộp bảo vệ.

## 3.4 Thi công, hàn mạch in và hoàn thiện phần cứng thực tế

**Gia công mạch in (Fabrication):**
Xuất tệp tin Gerber từ Altium và gửi xưởng gia công PCB (chất liệu FR4, phủ sơn Solder Mask).

**Quá trình hàn linh kiện (Soldering & Assembly):**
- **Trình tự hàn:** Hàn các tụ gốm 104, điện trở, transistor đệm còi trước. Sau đó hàn cổng Type-C Female và công tắc gạt. Tiếp tục hàn mạch hạ áp QS-1205CME-3A, các tụ hóa 47uF và 1000uF. Cuối cùng, hàn rào cắm (Header) để cắm các module ESP32, SIM A7680C, GPS, OLED và RFID RC522.
- Việc sử dụng đế cắm rào cái (Female Header) cho các module đắt tiền giúp dễ dàng tháo lắp, thay thế khi có hỏng hóc trong quá trình thử nghiệm đồ án.

**Đóng vỏ hộp và hoàn thiện (Casing):**
- Trong quá trình bảo vệ đồ án trước hội đồng và thử nghiệm, hệ thống được cấp nguồn rất nhanh chóng và an toàn từ một viên pin sạc dự phòng thông qua cáp USB Type-C.
- Mạch thành phẩm được lắp cố định vào hộp bảo vệ. Anten GPS và anten 4G được đi dây gọn gàng, hướng bắt sóng tốt nhất để đạt độ ổn định cao nhất khi thao tác thực tế.
