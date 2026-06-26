# CHƯƠNG 4. THIẾT KẾ VÀ XÂY DỰNG PHẦN MỀM

Chương 4 tập trung vào việc mô tả các giải pháp phần mềm, lưu đồ thuật toán điều khiển của vi điều khiển ESP32, cũng như cách thức thiết lập giao tiếp với cơ sở dữ liệu đám mây Firebase và nền tảng nhắn tin Telegram.

## 4.1 Biểu đồ Use Case (Ca sử dụng)

Đối với một hệ thống nhúng và IoT phục vụ người dùng cuối, việc thiết lập các Use Case giúp định hình rõ ràng cách các đối tượng tương tác với hệ thống. Sơ đồ các ca sử dụng tổng quát của hệ thống được thể hiện qua hình vẽ dưới đây:

*(Chèn ảnh biểu đồ Use Case bạn vừa vẽ ở đây)*
**Hình 4.1: Biểu đồ Use Case tổng quát của hệ thống**

Dựa trên luồng hoạt động thực tế, hệ thống có 3 tác nhân (Actors) chính và các Use case tương ứng được tổng hợp trong bảng dưới đây:

**Bảng 4.0: Tổng hợp các Use Case trong hệ thống**

| Mã UC | Tên Use Case | Tác nhân chính | Ý nghĩa cơ bản |
| :--- | :--- | :--- | :--- |
| **UC1** | Quẹt thẻ điểm danh | Học sinh | Xác nhận việc lên/xuống xe buýt qua thẻ RFID. |
| **UC2** | Nhận thông báo điểm danh qua Internet | Phụ huynh | Tự động nhận tin nhắn Telegram khi học sinh lên/xuống xe. |
| **UC3** | Truy vấn vị trí xe buýt theo yêu cầu | Phụ huynh | Chủ động hỏi vị trí hiện tại của xe buýt qua lệnh Telegram. |
| **UC4** | Quản lý thông tin học sinh | Quản trị viên / Nhà trường | Thêm, sửa, xóa dữ liệu thẻ RFID, Chat ID trên Firebase. |
| **UC5** | Giám sát trạng thái toàn hệ thống | Quản trị viên / Nhà trường | Theo dõi trạng thái In/Out của học sinh theo thời gian thực. |

### 4.1.1 Tác nhân: Học sinh (Student)
Học sinh là người trực tiếp tương tác vật lý với thiết bị phần cứng trên xe buýt.

*   **UC1 - Quẹt thẻ điểm danh:** Học sinh sử dụng thẻ RFID (được cấp phát) để quẹt vào thiết bị khi lên hoặc xuống xe buýt. Hệ thống phần cứng sẽ đọc UID, phát còi "bíp" và hiển thị thông tin lên màn hình OLED.

### 4.1.2 Tác nhân: Phụ huynh (Parent)
Phụ huynh là người dùng đầu cuối (End-user) tiếp nhận thông tin từ hệ thống thông qua ứng dụng di động (Telegram).

*   **UC2 - Nhận thông báo điểm danh qua Internet:** Phụ huynh tự động nhận tin nhắn Push Notification qua Telegram khi con em mình lên/xuống xe (kèm thời gian thực và link Google Maps).
*   **UC3 - Truy vấn vị trí xe buýt theo yêu cầu:** Phụ huynh chủ động mở chat bot Telegram, gõ lệnh `vitri` để yêu cầu hệ thống trả về tọa độ chính xác của xe buýt ngay tại thời điểm đó.

### 4.1.3 Tác nhân: Quản trị viên / Nhà trường (Administrator)
Người chịu trách nhiệm thiết lập và quản lý dữ liệu gốc trên máy chủ đám mây (Firebase).

*   **UC4 - Quản lý thông tin học sinh:** Quản trị viên thao tác trên cơ sở dữ liệu Firebase để Thêm, Sửa, Xóa thông tin học sinh bao gồm: mã thẻ UID, Tên học sinh, Chat ID (để gửi Telegram).
*   **UC5 - Giám sát trạng thái toàn hệ thống:** Xem trực tiếp trạng thái hiện tại của toàn bộ học sinh (đang ở trạng thái in hay out) trên Realtime Database.

### 4.1.4 Đặc tả các Use Case

Dưới đây là bảng đặc tả chi tiết cho từng Use Case trong hệ thống, mô tả cụ thể luồng sự kiện từ lúc bắt đầu đến khi kết thúc.

**Bảng 4.1: Đặc tả Use Case "Quẹt thẻ điểm danh"**

| Thuộc tính | Nội dung |
| :--- | :--- |
| **Tên Use Case** | Quẹt thẻ điểm danh (UC1) |
| **Tác nhân** | Học sinh |
| **Mô tả** | Học sinh sử dụng thẻ RFID để quẹt vào thiết bị nhằm xác nhận việc lên hoặc xuống xe buýt. Hệ thống ghi nhận và chuyển trạng thái của học sinh. |
| **Tiền điều kiện** | Hệ thống (ESP32) đã khởi động, kết nối thành công Wi-Fi và Firebase. Thẻ RFID đã được đăng ký mã UID trên hệ thống. |
| **Hậu điều kiện** | Trạng thái (In/Out) của học sinh được cập nhật trên cơ sở dữ liệu Firebase. Hệ thống phát còi và hiển thị thông tin lên màn hình OLED. |
| **Luồng chính** | 1. Học sinh đưa thẻ RFID vào vùng quét của đầu đọc RC522.<br>2. Hệ thống đọc mã UID của thẻ.<br>3. Hệ thống hiển thị "Đang xử lý..." trên màn hình OLED.<br>4. Hệ thống truy vấn thông tin từ Firebase dựa trên UID (nếu hợp lệ thì sang bước 5, không hợp lệ thì sang luồng phụ).<br>5. Hệ thống đảo ngược trạng thái hiện tại (In -> Out hoặc ngược lại) và cập nhật lên Firebase.<br>6. Hệ thống hiển thị tên và trạng thái mới lên màn hình OLED, đồng thời phát tiếng "bíp" xác nhận. |
| **Luồng phụ** | **Thẻ không hợp lệ (Không tìm thấy UID):** Hệ thống hiển thị thông báo "The chua dang ky!" lên màn hình OLED, phát âm thanh cảnh báo và không thực hiện cập nhật dữ liệu. |

**Bảng 4.2: Đặc tả Use Case "Nhận thông báo điểm danh qua Internet"**

| Thuộc tính | Nội dung |
| :--- | :--- |
| **Tên Use Case** | Nhận thông báo điểm danh qua Internet (UC2) |
| **Tác nhân** | Phụ huynh |
| **Mô tả** | Hệ thống tự động gửi tin nhắn báo cáo thời gian thực về việc học sinh lên/xuống xe kèm vị trí GPS qua ứng dụng Telegram cho phụ huynh. |
| **Tiền điều kiện** | Phụ huynh đã sử dụng Telegram và Chat ID được lưu đúng trên Firebase. Học sinh vừa thực hiện UC1 thành công. |
| **Hậu điều kiện** | Phụ huynh nhận được thông báo qua Telegram với đầy đủ thông tin (Tên học sinh, trạng thái lên/xuống, thời gian, liên kết Google Maps). |
| **Luồng chính** | 1. Hệ thống lấy Chat ID và trạng thái mới của học sinh từ UC1.<br>2. Hệ thống lấy tọa độ GPS hiện tại từ module định vị.<br>3. Hệ thống tạo nội dung tin nhắn báo cáo và liên kết Google Maps.<br>4. Hệ thống gọi API của Telegram Bot để gửi tin nhắn đến Chat ID của phụ huynh (nếu thành công thì sang bước 5, lỗi thì sang luồng phụ).<br>5. Phụ huynh nhận được thông báo (Push Notification) trên ứng dụng Telegram. |
| **Luồng phụ** | **Chat ID không hợp lệ hoặc lỗi mạng:** Hệ thống ghi nhận lỗi qua Serial Monitor, bỏ qua bước gửi Telegram để giải phóng tài nguyên và tiếp tục chờ quét thẻ tiếp theo. |

**Bảng 4.3: Đặc tả Use Case "Truy vấn vị trí xe buýt theo yêu cầu"**

| Thuộc tính | Nội dung |
| :--- | :--- |
| **Tên Use Case** | Truy vấn vị trí xe buýt theo yêu cầu (UC3) |
| **Tác nhân** | Phụ huynh |
| **Mô tả** | Phụ huynh chủ động nhắn tin cho hệ thống qua Telegram để hỏi tọa độ hiện tại của xe buýt. |
| **Tiền điều kiện** | Hệ thống đang hoạt động, có kết nối Wi-Fi. Module GPS đã chốt được tọa độ vệ tinh. |
| **Hậu điều kiện** | Hệ thống phản hồi lại tin nhắn chứa liên kết bản đồ vị trí xe buýt ngay tại thời điểm đó. |
| **Luồng chính** | 1. Phụ huynh mở ứng dụng Telegram và nhắn từ khóa `vitri` cho Telegram Bot.<br>2. Hệ thống (được thiết lập polling mỗi 10 giây) đọc tin nhắn mới nhất từ API Telegram.<br>3. Hệ thống kiểm tra nội dung tin nhắn (nếu chứa từ khóa hợp lệ thì sang bước 4, không thì sang luồng phụ).<br>4. Hệ thống đọc tọa độ GPS mới nhất từ luồng xử lý định vị.<br>5. Hệ thống tạo liên kết bản đồ và gửi tin nhắn phản hồi cho phụ huynh.<br>6. Phụ huynh nhận được tin nhắn chứa vị trí xe buýt. |
| **Luồng phụ** | **Tin nhắn không hợp lệ:** Hệ thống bỏ qua tin nhắn và không phản hồi.<br>**Chưa có tín hiệu GPS:** Hệ thống trả về tọa độ cũ nhất hoặc thông báo đang chờ tín hiệu vệ tinh. |

**Bảng 4.4: Đặc tả Use Case "Quản lý thông tin học sinh"**

| Thuộc tính | Nội dung |
| :--- | :--- |
| **Tên Use Case** | Quản lý thông tin học sinh (UC4) |
| **Tác nhân** | Quản trị viên / Nhà trường |
| **Mô tả** | Quản trị viên thêm, sửa, hoặc xóa dữ liệu của học sinh trên cơ sở dữ liệu. |
| **Tiền điều kiện** | Quản trị viên có quyền truy cập vào bảng điều khiển Firebase hoặc hệ thống Web quản lý. |
| **Hậu điều kiện** | Thông tin học sinh (UID, Tên, Chat ID, trạng thái) được lưu trữ và cập nhật thành công trên hệ thống. |
| **Luồng chính** | 1. Quản trị viên đăng nhập vào hệ thống quản lý.<br>2. Chọn chức năng Thêm / Sửa / Xóa thông tin học sinh.<br>3. Nhập hoặc thay đổi các thông tin cần thiết (UID, Tên, Chat ID).<br>4. Nhấn Xác nhận / Lưu.<br>5. Hệ thống kiểm tra tính hợp lệ của dữ liệu (nếu hợp lệ sang bước 6, lỗi sang luồng phụ).<br>6. Cập nhật dữ liệu vào cơ sở dữ liệu Firebase và thông báo thành công. |
| **Luồng phụ** | **Dữ liệu thiếu hoặc sai định dạng:** Hệ thống hiện thông báo lỗi và yêu cầu quản trị viên kiểm tra lại thông tin vừa nhập. |

**Bảng 4.5: Đặc tả Use Case "Giám sát trạng thái toàn hệ thống"**

| Thuộc tính | Nội dung |
| :--- | :--- |
| **Tên Use Case** | Giám sát trạng thái toàn hệ thống (UC5) |
| **Tác nhân** | Quản trị viên / Nhà trường |
| **Mô tả** | Quản trị viên xem và theo dõi trạng thái In/Out của tất cả học sinh theo thời gian thực. |
| **Tiền điều kiện** | Quản trị viên có kết nối Internet và đã truy cập vào hệ thống giám sát. |
| **Hậu điều kiện** | Quản trị viên nhìn thấy được cái nhìn tổng quan về số lượng học sinh đang trên xe và đã xuống xe. |
| **Luồng chính** | 1. Quản trị viên mở giao diện giám sát hoặc Console của Firebase.<br>2. Hệ thống tải toàn bộ dữ liệu từ Node "users".<br>3. Giao diện hiển thị danh sách học sinh kèm trạng thái hiện tại (In/Out).<br>4. Bất cứ khi nào có học sinh quẹt thẻ, dữ liệu trên màn hình giám sát sẽ tự động nhảy (cập nhật) theo thời gian thực mà không cần tải lại trang. |
| **Luồng phụ** | **Mất kết nối mạng:** Giao diện hiển thị trạng thái offline hoặc thông báo không thể lấy dữ liệu mới nhất. |

## 4.2 Lưu đồ thuật toán tổng quát của hệ thống

Luồng xử lý phần mềm của hệ thống được thiết kế theo mô hình vòng lặp liên tục kết hợp với đa nhiệm (Multi-tasking). Quá trình hoạt động được chia thành các bước cơ bản sau:
1. **Khởi tạo (Setup):** Thiết lập các chuẩn giao tiếp (I2C, SPI, UART), khởi tạo màn hình OLED, kết nối Wi-Fi, và cấp phát một luồng (Task) độc lập để liên tục cập nhật tọa độ GPS.
2. **Kiểm tra kết nối mạng:** Vòng lặp chính luôn kiểm tra trạng thái Wi-Fi. Nếu mất kết nối, hệ thống sẽ cố gắng kết nối lại (Auto-reconnect).
3. **Quét thẻ RFID:** Hệ thống liên tục thăm dò đầu đọc thẻ RC522. Khi phát hiện thẻ hợp lệ, thiết bị sẽ chuyển đổi mã định danh sang chuỗi UID dạng HEX và gọi hàm truy xuất dữ liệu từ Firebase.
4. **Xử lý dữ liệu Firebase:** Dựa vào UID, thiết bị gửi lệnh HTTP GET để lấy thông tin (Chat ID, Tên, Trạng thái In/Out). Sau đó, gửi lệnh HTTP PUT để cập nhật trạng thái mới.
5. **Định tuyến thông báo (Telegram):** Dựa vào dữ liệu thu được, thiết bị gửi tin nhắn qua Telegram Bot (qua mạng Wi-Fi). Màn hình OLED sẽ cập nhật trạng thái tương ứng.
6. **Lắng nghe người dùng:** Cứ mỗi 10 giây, thiết bị sẽ gọi API Telegram một lần để xem có phụ huynh nào nhắn lệnh `"vitri"` hay không nhằm phản hồi vị trí tức thời.

## 4.3 Khởi tạo môi trường lập trình và các thư viện sử dụng

Hệ thống được lập trình bằng ngôn ngữ C++ trên nền tảng Arduino IDE. Phần mềm sử dụng các thư viện mã nguồn mở mạnh mẽ để tối ưu hóa thời gian phát triển:
- `WiFi.h` và `WiFiClientSecure.h`: Quản lý kết nối mạng và thiết lập kênh truyền tải HTTPS (mã hóa SSL) an toàn khi giao tiếp với Firebase và Telegram.
- `HTTPClient.h`: Thực hiện các phương thức HTTP như GET, PUT, POST.
- `ArduinoJson.h`: Phân tích cú pháp (Parsing) các chuỗi dữ liệu JSON phức tạp trả về từ Firebase và Telegram API.
- `TinyGPS++.h`: Giải mã các bản tin chuẩn NMEA từ vệ tinh thành dữ liệu Kinh độ, Vĩ độ, và Thời gian dễ đọc [13].
- `MFRC522.h`: Điều khiển module nhận diện RFID qua giao tiếp SPI.
- `Adafruit_GFX.h` và `Adafruit_SH110X.h`: Hiển thị font chữ và đồ họa lên màn hình OLED thông qua chuẩn I2C.

## 4.4 Triển khai các thuật toán xử lý chính

### 4.4.1 Thuật toán xử lý đa luồng (Multi-tasking) cho GPS

Việc phân tích chuỗi tín hiệu Serial từ module GPS mất rất nhiều thời gian và có thể làm treo (blocking) quá trình quét thẻ RFID. Do ESP32 là chip lõi kép (Dual-Core), thuật toán giải quyết bằng cách sử dụng hệ điều hành thời gian thực FreeRTOS [2]:
- Nhân xử lý Core 1 (Vòng lặp `loop()`) đảm nhiệm chức năng mạng, OLED và RFID.
- Nhân xử lý Core 0 được cấp phát hàm `xTaskCreatePinnedToCore` để chạy một luồng vô tận `gpsTask`. Nhiệm vụ duy nhất của luồng này là liên tục đọc dữ liệu `SerialGPS` đưa vào bộ giải mã của `TinyGPSPlus` với một độ trễ rất nhỏ (`vTaskDelay(10)`).
Sự phân chia này đảm bảo dữ liệu tọa độ vệ tinh luôn ở trạng thái mới nhất (real-time) mà không ảnh hưởng tới tốc độ phản hồi quét thẻ của học sinh.

### 4.4.2 Thuật toán xử lý thẻ RFID và cơ sở dữ liệu Firebase

Firebase Realtime Database (RTDB) được thiết kế lưu trữ theo cấu trúc cây JSON [11].
Khi học sinh quét thẻ, hàm `getParentData()` sẽ được kích hoạt:
1. Nối chuỗi URL: `https://[FIREBASE_HOST]/users/[UID].json?auth=[FIREBASE_AUTH]`.
2. Gửi yêu cầu HTTP GET bằng thư viện `HTTPClient`.
3. Nhận về chuỗi JSON và dùng `DynamicJsonDocument` để trích xuất các trường: `chat_id`, `name`, `status`.
4. Sau khi trích xuất thành công, hàm `updateStatus()` sẽ gửi yêu cầu HTTP PUT với nội dung được đảo ngược (Ví dụ: trạng thái cũ là "in" thì trạng thái mới đẩy lên sẽ là "out").

### 4.4.3 Thuật toán gửi thông báo qua Telegram

Đây là thuật toán quan trạng giúp đảm bảo thông tin được gửi đến phụ huynh một cách nhanh chóng và chính xác. Nếu biến `chat_id` lấy từ Firebase hợp lệ (có độ dài > 10 ký tự), hệ thống lập tức gọi API Telegram để gửi thông báo [12]. Ưu điểm của việc sử dụng Telegram là phản hồi cực nhanh (dưới 2 giây), hoàn toàn miễn phí, và hỗ trợ đính kèm nhiều thông tin văn bản cũng như liên kết Google Maps trực quan.

## 4.5 Triển khai cơ chế tương tác hai chiều (Telegram Bot)

Không chỉ báo cáo thụ động, hệ thống còn cung cấp chức năng lắng nghe chủ động thông qua Telegram Bot.
- Do ESP32 không có địa chỉ IP tĩnh nên không thể dùng Webhook. Giải pháp được chọn là "Polling".
- Một bộ đếm thời gian (Timer) bằng hàm `millis()` được thiết lập. Cứ 10 giây một lần, hệ thống gọi hàm `checkTelegramUpdates()`.
- Hàm này truy cập URL: `api.telegram.org/bot[TOKEN]/getUpdates?offset=-1` để kéo về tin nhắn mới nhất trong Bot.
- Nếu tin nhắn có nội dung chứa từ khóa "vitri", hệ thống sẽ đọc tọa độ từ thư viện GPS, khởi tạo đường dẫn Google Maps và gửi ngược lại vào khung chat của phụ huynh.

## 4.6 Giao diện người dùng trên Client App

Ứng dụng Telegram được chọn làm giao diện người dùng chính thay vì phải lập trình một ứng dụng riêng biệt, đem lại sự thân thiện và bảo mật.
- **Báo cáo trạng thái:** Phụ huynh nhận tin nhắn với nội dung rõ ràng: *"Học sinh [Tên] đã LÊN/XUỐNG xe buýt. Thời gian: [Giờ:Phút:Giây]"*. 
- **Báo cáo định vị:** Kèm theo nội dung trên luôn là dòng chữ xanh chứa đường dẫn Google Maps tọa độ GPS chính xác lúc quẹt thẻ. Chỉ cần chạm vào liên kết, ứng dụng Bản đồ trên điện thoại phụ huynh sẽ tự bật ra.

## 4.7 Kết luận chương

Chương 4 đã giải quyết trọn vẹn việc thiết kế và xây dựng giải pháp phần mềm cho hệ thống giám sát xe buýt học sinh thông minh. Bằng cách xây dựng chi tiết các biểu đồ đặc tả Use Case, thiết lập lưu đồ thuật toán tổng quát và ứng dụng cơ chế đa luồng (Multi-tasking) của hệ điều hành FreeRTOS trên nhân kép ESP32, chương này đã chứng minh được tính tối ưu và hiệu quả trong việc phân tách luồng dữ liệu GPS độc lập với tác vụ quét RFID và xử lý các API mạng. Đồng thời, việc tích hợp thành công giao tiếp thời gian thực với Firebase Realtime Database và cơ chế tương tác hai chiều thông qua Telegram Bot đã hoàn thiện phần điều khiển cốt lõi của thiết bị. Đây là cơ sở kỹ thuật quan trọng để hệ thống có thể chuyển sang giai đoạn kiểm thử thực tế và đánh giá hiệu năng chi tiết ở Chương 5.
