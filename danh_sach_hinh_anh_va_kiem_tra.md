# DANH SÁCH RÀ SOÁT HÌNH ẢNH, SƠ ĐỒ VÀ CÁC HẠNG MỤC CẦN HOÀN THIỆN

Tài liệu này tổng hợp toàn bộ các vị trí cần chèn hình ảnh, sơ đồ nguyên lý, lưu đồ thuật toán và các nội dung cần bổ sung/hiệu chỉnh trong từng chương của đồ án tốt nghiệp **"Hệ thống điểm danh và giám sát hành trình xe buýt học sinh thông minh sử dụng công nghệ IoT"**.

---

## 1. Danh sách các Hình ảnh & Sơ đồ cần chèn vào Word

Dưới đây là danh sách các hình ảnh và sơ đồ kỹ thuật bạn cần xuất/chụp thực tế và chèn vào các chương tương ứng:

### CHƯƠNG 2. THIẾT KẾ TỔNG QUAN HỆ THỐNG
*   **[ ] Hình 2.1: Sơ đồ kiến trúc tổng quan hệ thống**
    *   *Vị trí:* Ngay sau mục **2.1 Mô hình tổng quan về hệ thống**.
    *   *Nguồn:* Vẽ bằng Mermaid (đã có mã nguồn ở mục 3.1 trong file [Bieu_do_UML_Mermaid.md](file:///c:/Users/buidu/OneDrive/Máy%20tính/datn/Bieu_do_UML_Mermaid.md)). Bạn có thể xuất ra ảnh PNG tại [Mermaid Live Editor](https://mermaid.live/).

### CHƯƠNG 3. THIẾT KẾ VÀ TRIỂN KHAI PHẦN CỨNG
*   **[ ] Hình 3.1: Sơ đồ khối phần cứng của thiết bị**
    *   *Vị trí:* Mục **3.1 Sơ đồ khối tổng quan của phần cứng thiết bị (Device)**.
    *   *Nội dung:* Vẽ sơ đồ khối thể hiện ESP32 ở trung tâm kết nối với các khối: Nguồn (Type-C), GPS, RFID, OLED, Buzzer.
*   **[ ] Hình 3.2: Sơ đồ nguyên lý (Schematic) khối vi điều khiển trung tâm ESP32**
    *   *Vị trí:* Mục **3.2.2**. Xuất từ phần mềm Altium Designer.
*   **[ ] Hình 3.3: Sơ đồ nguyên lý khối định vị GPS ATGM336H**
    *   *Vị trí:* Mục **3.2.3**. Xuất từ Altium Designer.
*   **[ ] Hình 3.4: Sơ đồ nguyên lý khối RFID RC522 và màn hình OLED**
    *   *Vị trí:* Mục **3.2.4**. Xuất từ Altium Designer.
*   **[ ] Hình 3.5: Sơ đồ nguyên lý mạch còi cảnh báo (Buzzer)**
    *   *Vị trí:* Mục **3.2.5** (thể hiện rõ transistor C1815 đệm dòng).
*   **[ ] Hình 3.6: Sơ đồ nguyên lý mạch nguồn cấp (Cổng Type-C và các tụ lọc nhiễu)**
    *   *Vị trí:* Mục **3.2.6**.
*   **[ ] Hình 3.7: Thiết kế bảng mạch in (PCB Layout) ở chế độ 2D**
    *   *Vị trí:* Mục **3.3.1** (chụp rõ đường đi dây của cả lớp Top và Bottom, vùng phủ đồng GND).
*   **[ ] Hình 3.8: Bản vẽ mô phỏng mạch in 3D (3D PCB View)**
    *   *Vị trí:* Mục **3.3.2** (Altium hỗ trợ xuất ảnh 3D chân thực kèm linh kiện).
*   **[ ] Hình 3.9: Hình ảnh mạch in thực tế sau khi hàn lắp linh kiện**
    *   *Vị trí:* Mục **3.4**. Ảnh chụp thực tế thiết bị thật của bạn khi đã hàn hoàn thiện.
*   **[ ] Hình 3.10: Thiết bị hoàn thiện sau khi lắp vào vỏ hộp bảo vệ**
    *   *Vị trí:* Mục **3.4**. Ảnh chụp thực tế hộp thiết bị hoàn chỉnh có anten GPS đi kèm.

### CHƯƠNG 4. THIẾT KẾ VÀ XÂY DỰNG PHẦN MỀM
*   **[ ] Hình 4.1: Biểu đồ Use Case tổng quát của hệ thống**
    *   *Vị trí:* Mục **4.1**. Vẽ bằng PlantUML (Mã nguồn đã cập nhật chuẩn ở mục 1 trong file [Bieu_do_UML_Mermaid.md](file:///c:/Users/buidu/OneDrive/Máy%20tính/datn/Bieu_do_UML_Mermaid.md)).
*   **[ ] Hình 4.2: Lưu đồ thuật toán tổng quát hoạt động của ESP32**
    *   *Vị trí:* Mục **4.2**. Vẽ bằng Mermaid (Mã nguồn có sẵn ở mục 3.2 trong file [Bieu_do_UML_Mermaid.md](file:///c:/Users/buidu/OneDrive/Máy%20tính/datn/Bieu_do_UML_Mermaid.md)).
*   **[ ] Hình 4.3: Biểu đồ tuần tự (Sequence Diagram) ca sử dụng quẹt thẻ điểm danh và gửi cảnh báo (UC1 & UC2)**
    *   *Vị trí:* Mục **4.4.2** hoặc **4.4.3**. (Mã nguồn PlantUML ở mục 2.1 trong file [Bieu_do_UML_Mermaid.md](file:///c:/Users/buidu/OneDrive/Máy%20tính/datn/Bieu_do_UML_Mermaid.md)).
*   **[ ] Hình 4.4: Biểu đồ tuần tự ca sử dụng phụ huynh truy vấn vị trí xe buýt (UC3)**
    *   *Vị trí:* Mục **4.5**. (Mã nguồn PlantUML ở mục 2.2 trong file [Bieu_do_UML_Mermaid.md](file:///c:/Users/buidu/OneDrive/Máy%20tính/datn/Bieu_do_UML_Mermaid.md)).
*   **[ ] Hình 4.5: Biểu đồ tuần tự ca sử dụng quản lý học sinh và giám sát trực quan (UC4 & UC5)**
    *   *Vị trí:* Mục **4.6** hoặc **4.1.3**. (Mã nguồn PlantUML ở mục 2.3 trong file [Bieu_do_UML_Mermaid.md](file:///c:/Users/buidu/OneDrive/Máy%20tính/datn/Bieu_do_UML_Mermaid.md)).

### CHƯƠNG 5. KIỂM THỬ VÀ ĐÁNH GIÁ KẾT QUẢ
*   **[ ] Hình 5.1: Ảnh chụp thực tế thiết bị hoạt động quét thẻ RFID**
    *   *Vị trí:* Mục **5.3.1**. Chụp tay đang quẹt thẻ vào module và màn hình OLED hiển thị tên học sinh.
*   **[ ] Hình 5.2: Ảnh chụp giao diện Firebase Realtime Database cập nhật trạng thái thời gian thực**
    *   *Vị trí:* Mục **5.3.1**. Chụp màn hình máy tính hiển thị nhánh dữ liệu JSON đổi trạng thái từ "in" sang "out".
*   **[ ] Hình 5.3: Ảnh chụp màn hình tin nhắn thông báo đẩy về điện thoại phụ huynh qua Telegram**
    *   *Vị trí:* Mục **5.3.3**. Chụp màn hình điện thoại nhận được tin nhắn từ Bot (gồm tên, giờ, trạng thái và link bản đồ).
*   **[ ] Hình 5.4: Giao diện định vị vị trí xe trên Google Maps khi phụ huynh ấn vào liên kết**
    *   *Vị trí:* Mục **5.4.1**. Chụp màn hình Google Maps mở ra vị trí pin định vị của xe buýt.

---

## 2. Các hạng mục công việc cần làm tiếp theo

Để hoàn thiện cuốn báo cáo đạt kết quả cao nhất, bạn cần thực hiện các bước sau:

### Bước 1: Thêm định nghĩa các Module cứng (Chương 3)
Bạn cần bổ sung các đoạn văn giới thiệu chi tiết/định nghĩa kỹ thuật cho các linh kiện chính:
1.  **Vi điều khiển ESP32 DevKit V1:** (Giới thiệu về nhân xử lý Tensilica Xtensa 32-bit LX6 dual-core, tích hợp Wi-Fi 802.11 b/g/n và Bluetooth v4.2 BR/EDR/BLE, tốc độ xung nhịp lên tới 240MHz).
2.  **Module định vị GPS ATGM336H:** (Module định vị đa hệ thống BDS/GPS hiệu suất cao, độ nhạy cực tốt -162dBm, hỗ trợ 99 kênh thu sóng, giao tiếp UART, sai số định vị dân sự < 10m).
3.  **Module đọc thẻ RFID RC522:** (Sử dụng chip MFRC522 giao tiếp không tiếp xúc tần số 13.56 MHz, hỗ trợ các chuẩn ISO/IEC 14443 A, giao tiếp SPI tốc độ cao với vi điều khiển).
4.  **Màn hình OLED 1.3 inch (Driver SH1106):** (Công nghệ màn hình OLED tự phát sáng không cần đèn nền, độ phân giải 128x64 pixel, giao tiếp I2C tiết kiệm chân GPIO, độ tương phản cao dưới ánh sáng mặt trời).

> [!TIP]
> Các định nghĩa này nên viết ngắn gọn (khoảng 0.5 - 1 trang cho mỗi linh kiện), tập trung vào các thông số kỹ thuật ảnh hưởng trực tiếp đến hệ thống (ví dụ: dòng tiêu thụ, điện áp hoạt động, giao thức truyền thông).

### Bước 2: Thiết lập Tài liệu tham khảo (References)
Bổ sung các tài liệu tham khảo chính thống vào cuối cuốn đồ án:
1.  **Datasheet của nhà sản xuất:**
    *   Espressif Systems, *ESP32 WROOM-32 Datasheet*.
    *   NXP Semiconductors, *MFRC522 Contactless Reader IC Datasheet*.
    *   Hangzhou Zhongke Microelectronics Co., Ltd, *ATGM336H GPS/BDS Module Datasheet*.
2.  **Tài liệu hệ thống / API:**
    *   Google Firebase, *Firebase Realtime Database REST API Documentation*.
    *   Telegram Messenger LLP, *Telegram Bot API Reference*.
3.  **Sách & Nghiên cứu liên quan:**
    *   Richard Barry, *Using the FreeRTOS Real Time Kernel - A Practical Guide*, Real Time Engineers Ltd.
    *   Các bài báo khoa học về ứng dụng RFID và định vị trong quản lý phương tiện giao thông trường học.

### Bước 3: Đồng bộ hóa định dạng Word
*   Sau khi copy nội dung từ các file Markdown (`.md`) vào Word, hãy định dạng lại Style của tiêu đề: Heading 1 cho các chương, Heading 2 cho các mục lớn (X.1), Heading 3 cho các mục nhỏ (X.1.1).
*   Tất cả các bảng biểu phải được tạo bằng chức năng **Table** của Word (không dán ảnh bảng) để dễ dàng căn chỉnh cột và font chữ.
*   Cài đặt tính năng tự động tạo **Danh mục hình ảnh (List of Figures)** và **Danh mục bảng biểu (List of Tables)** từ các caption tương ứng để tạo độ chuyên nghiệp.
