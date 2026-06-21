# CHƯƠNG 4. THIẾT KẾ VÀ XÂY DỰNG PHẦN MỀM

Chương 4 tập trung vào việc mô tả các giải pháp phần mềm, lưu đồ thuật toán điều khiển của vi điều khiển ESP32, cũng như cách thức thiết lập giao tiếp với cơ sở dữ liệu đám mây Firebase và nền tảng nhắn tin Telegram.

## 4.1 Lưu đồ thuật toán tổng quát của hệ thống

Luồng xử lý phần mềm của hệ thống được thiết kế theo mô hình vòng lặp liên tục kết hợp với đa nhiệm (Multi-tasking). Quá trình hoạt động được chia thành các bước cơ bản sau:
1. **Khởi tạo (Setup):** Thiết lập các chuẩn giao tiếp (I2C, SPI, UART), khởi tạo màn hình OLED, kết nối Wi-Fi, và cấp phát một luồng (Task) độc lập để liên tục cập nhật tọa độ GPS.
2. **Kiểm tra kết nối mạng:** Vòng lặp chính luôn kiểm tra trạng thái Wi-Fi. Nếu mất kết nối, hệ thống sẽ cố gắng kết nối lại (Auto-reconnect).
3. **Quét thẻ RFID:** Hệ thống liên tục thăm dò đầu đọc thẻ RC522. Khi phát hiện thẻ hợp lệ, thiết bị sẽ chuyển đổi mã định danh sang chuỗi UID dạng HEX và gọi hàm truy xuất dữ liệu từ Firebase.
4. **Xử lý dữ liệu Firebase:** Dựa vào UID, thiết bị gửi lệnh HTTP GET để lấy thông tin (Số điện thoại, Chat ID, Tên, Trạng thái In/Out). Sau đó, gửi lệnh HTTP PUT để cập nhật trạng thái mới.
5. **Định tuyến thông báo (Telegram/SMS):** Dựa vào dữ liệu thu được, thiết bị ưu tiên gửi tin nhắn qua Telegram Bot (qua mạng Wi-Fi). Nếu độ dài Chat ID không hợp lệ hoặc Telegram báo lỗi, hệ thống tự động kích hoạt module SIM 4G thực thi lệnh gửi SMS để dự phòng. Màn hình OLED sẽ cập nhật trạng thái tương ứng.
6. **Lắng nghe người dùng:** Cứ mỗi 10 giây, thiết bị sẽ gọi API Telegram một lần để xem có phụ huynh nào nhắn lệnh `"vitri"` hay không nhằm phản hồi vị trí tức thời.

## 4.2 Khởi tạo môi trường lập trình và các thư viện sử dụng

Hệ thống được lập trình bằng ngôn ngữ C++ trên nền tảng Arduino IDE. Phần mềm sử dụng các thư viện mã nguồn mở mạnh mẽ để tối ưu hóa thời gian phát triển:
- `WiFi.h` và `WiFiClientSecure.h`: Quản lý kết nối mạng và thiết lập kênh truyền tải HTTPS (mã hóa SSL) an toàn khi giao tiếp với Firebase và Telegram.
- `HTTPClient.h`: Thực hiện các phương thức HTTP như GET, PUT, POST.
- `ArduinoJson.h`: Phân tích cú pháp (Parsing) các chuỗi dữ liệu JSON phức tạp trả về từ Firebase và Telegram API.
- `TinyGPS++.h`: Giải mã các bản tin chuẩn NMEA từ vệ tinh thành dữ liệu Kinh độ, Vĩ độ, và Thời gian dễ đọc.
- `MFRC522.h`: Điều khiển module nhận diện RFID qua giao tiếp SPI.
- `Adafruit_GFX.h` và `Adafruit_SH110X.h`: Hiển thị font chữ và đồ họa lên màn hình OLED thông qua chuẩn I2C.

## 4.3 Triển khai các thuật toán xử lý chính

### 4.3.1 Thuật toán xử lý đa luồng (Multi-tasking) cho GPS

Việc phân tích chuỗi tín hiệu Serial từ module GPS mất rất nhiều thời gian và có thể làm treo (blocking) quá trình quét thẻ RFID. Do ESP32 là chip lõi kép (Dual-Core), thuật toán giải quyết bằng cách sử dụng FreeRTOS:
- Nhân xử lý Core 1 (Vòng lặp `loop()`) đảm nhiệm chức năng mạng, OLED và RFID.
- Nhân xử lý Core 0 được cấp phát hàm `xTaskCreatePinnedToCore` để chạy một luồng vô tận `gpsTask`. Nhiệm vụ duy nhất của luồng này là liên tục đọc dữ liệu `SerialGPS` đưa vào bộ giải mã của `TinyGPSPlus` với một độ trễ rất nhỏ (`vTaskDelay(10)`).
Sự phân chia này đảm bảo dữ liệu tọa độ vệ tinh luôn ở trạng thái mới nhất (real-time) mà không ảnh hưởng tới tốc độ phản hồi quét thẻ của học sinh.

### 4.3.2 Thuật toán xử lý thẻ RFID và cơ sở dữ liệu Firebase

Firebase Realtime Database (RTDB) được thiết kế lưu trữ theo cấu trúc cây JSON.
Khi học sinh quét thẻ, hàm `getParentData()` sẽ được kích hoạt:
1. Nối chuỗi URL: `https://[FIREBASE_HOST]/users/[UID].json?auth=[FIREBASE_AUTH]`.
2. Gửi yêu cầu HTTP GET bằng thư viện `HTTPClient`.
3. Nhận về chuỗi JSON và dùng `DynamicJsonDocument` để trích xuất các trường: `phone`, `chat_id`, `name`, `status`.
4. Sau khi trích xuất thành công, hàm `updateStatus()` sẽ gửi yêu cầu HTTP PUT với nội dung được đảo ngược (Ví dụ: trạng thái cũ là "in" thì trạng thái mới đẩy lên sẽ là "out").

### 4.3.3 Thuật toán gửi thông báo kép dự phòng (Dual-Fallback)

Đây là thuật toán quan trọng nhất đảm bảo tin nhắn luôn đến được tay phụ huynh, bao gồm 2 lớp:
- **Lớp 1 (Ưu tiên - Telegram):** Nếu biến `chat_id` lấy từ Firebase có độ dài > 10 ký tự, hệ thống lập tức gọi API Telegram. Ưu điểm: Phản hồi cực nhanh (dưới 2 giây), không tốn phí, đính kèm được nhiều thông tin văn bản và liên kết Google Maps.
- **Lớp 2 (Dự phòng - SMS):** Nếu hệ thống phát hiện `chat_id` dưới 10 ký tự (lỗi hoặc chưa cập nhật) HOẶC lớp 1 gửi Telegram thất bại, hàm `sendSMS()` được kích hoạt.
  - *Chống nhiễu sóng:* Tạm thời ngắt Wi-Fi (`WiFi.disconnect`) để dồn toàn bộ nguồn điện cho module SIM A7680C 4G và tránh nhiễu điện từ.
  - *Kiểm tra sóng:* Dùng lệnh `AT+CSQ`. Nếu chất lượng sóng quá thấp (<10), bỏ qua để không treo module.
  - *Thực thi:* Chuyển module về chế độ văn bản (`AT+CMGF=1`), gửi lệnh `AT+CMGS="[Số điện thoại]"`, chờ phản hồi dấu `>` và nạp nội dung tin nhắn, kết thúc bằng mã ASCII 26 (Ctrl+Z).
  - Kết thúc quá trình SMS, ESP32 sẽ tự động bật lại Wi-Fi để đón các học sinh tiếp theo.

## 4.4 Triển khai cơ chế tương tác hai chiều (Telegram Bot)

Không chỉ báo cáo thụ động, hệ thống còn cung cấp chức năng lắng nghe chủ động thông qua Telegram Bot.
- Do ESP32 không có địa chỉ IP tĩnh nên không thể dùng Webhook. Giải pháp được chọn là "Polling".
- Một bộ đếm thời gian (Timer) bằng hàm `millis()` được thiết lập. Cứ 10 giây một lần, hệ thống gọi hàm `checkTelegramUpdates()`.
- Hàm này truy cập URL: `api.telegram.org/bot[TOKEN]/getUpdates?offset=-1` để kéo về tin nhắn mới nhất trong Bot.
- Nếu tin nhắn có nội dung chứa từ khóa "vitri", hệ thống sẽ đọc tọa độ từ thư viện GPS, khởi tạo đường dẫn Google Maps và gửi ngược lại vào khung chat của phụ huynh.

## 4.5 Giao diện người dùng trên Client App

Ứng dụng Telegram được chọn làm giao diện người dùng chính thay vì phải lập trình một ứng dụng riêng biệt, đem lại sự thân thiện và bảo mật.
- **Báo cáo trạng thái:** Phụ huynh nhận tin nhắn với nội dung rõ ràng: *"Học sinh [Tên] đã LÊN/XUỐNG xe buýt. Thời gian: [Giờ:Phút:Giây]"*. 
- **Báo cáo định vị:** Kèm theo nội dung trên luôn là dòng chữ xanh chứa đường dẫn Google Maps tọa độ GPS chính xác lúc quẹt thẻ. Chỉ cần chạm vào liên kết, ứng dụng Bản đồ trên điện thoại phụ huynh sẽ tự bật ra.
- Trong trường hợp nhận bằng SMS (do rớt mạng), giao diện nội dung vẫn được giữ nguyên dạng văn bản kèm liên kết, hiển thị trong hộp thư nhắn tin mặc định của smartphone.

## 4.6 Kết luận chương
Chương 4 đã giải quyết trọn vẹn toàn bộ các thuật toán cần thiết để thu thập dữ liệu (RFID, GPS), xử lý logic (FreeRTOS đa luồng) và điều hướng luồng thông báo cực kỳ thông minh giữa mạng Wi-Fi (Telegram) và mạng viễn thông (SMS). Qua đó chứng minh được phần mềm điều khiển thiết bị hoạt động rất trơn tru, linh hoạt và luôn đảm bảo khả năng báo cáo cao nhất trong mọi hoàn cảnh thực tế.
