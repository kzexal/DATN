# CHƯƠNG 2. THIẾT KẾ TỔNG QUAN HỆ THỐNG

Nội dung chương 2 sẽ trình bày tổng quan về kiến trúc IoT của hệ thống giám sát xe buýt; những yêu cầu chức năng, phi chức năng của toàn hệ thống cũng như riêng từng phân hệ trong hệ thống, dựa trên thiết kế thực tế tích hợp giữa Firebase, Telegram và hệ thống viễn thông SMS.

## 2.1 Mô hình tổng quan về hệ thống

Hệ thống được xây dựng dựa vào kiến trúc IoT (Internet of Things) đám mây, với 3 phân hệ chính: Device (Thiết bị trạm), Cloud Server - Backend (Firebase RTDB & Telegram API), Client App (Ứng dụng di động người dùng); cùng 2 phân hệ phụ trợ là Hệ thống định vị GPS và Hệ thống Viễn thông (SIM). Các phân hệ được ứng dụng vào từng lớp (layer) của kiến trúc IoT như sau:

- **Perception Layer (Lớp nhận thức):** chứa Device (vi điều khiển ESP32) được gắn trên xe buýt đưa đón. Tại đây, dữ liệu được thu thập (quét thẻ RFID của học sinh, nhận chuỗi tọa độ NMEA từ module GPS) và xử lý logic ngay trên thiết bị. Lớp này đóng gói dữ liệu và đưa ra quyết định cảnh báo.
- **Network Layer (Lớp mạng):** sử dụng song song hai chuẩn kết nối để đảm bảo tính sẵn sàng cao (High Availability):
  - **Mạng Wi-Fi:** Đóng vai trò truyền dẫn Internet chính (thường thông qua bộ phát Wi-Fi 4G trên xe buýt) để thiết lập kết nối mã hóa SSL (HTTPS) tốc độ cao.
  - **Mạng Viễn thông (Module SIM):** Đóng vai trò là phương thức dự phòng (Fallback), sử dụng giao tiếp AT Command để truyền tín hiệu dạng tin nhắn SMS khi lớp mạng Internet gặp sự cố.
- **Processing Layer (Lớp xử lý đám mây):** gồm hai thành phần máy chủ độc lập nhưng liên kết chặt chẽ:
  - **Firebase Realtime Database:** Hoạt động như một Cơ sở dữ liệu đám mây (Cloud Database), cho phép ESP32 truy vấn thông tin học sinh và cập nhật trạng thái theo thời gian thực bằng API RESTful.
  - **Telegram Bot API:** Nhận các bản tin báo cáo từ ESP32, sau đó thực hiện phân phối tin nhắn cảnh báo Push Notification đến đúng đích nhận (Chat ID) của phụ huynh.
- **Application Layer (Lớp ứng dụng):** chứa Client App (Ứng dụng Telegram và Hộp thư SMS mặc định) được cài đặt trên thiết bị di động của phụ huynh, hiển thị tin nhắn điểm danh, cảnh báo và tự động mở bản đồ Google Maps từ tọa độ cung cấp.

## 2.2 Một số yêu cầu chung của hệ thống

- Có khả năng quản lý danh sách thông tin học sinh từ xa linh hoạt: Dữ liệu (gồm UID, Tên, Số điện thoại, Chat ID, Trạng thái In/Out) được cấu trúc hóa dưới dạng JSON (`data1.json`) và lưu trữ trực tuyến trên Firebase, giúp người quản trị dễ dàng thêm, sửa, xóa mà không cần nạp lại mã nguồn cho phần cứng.
- Hệ thống hoạt động theo cơ chế **Đa luồng (Multi-tasking)** trên ESP32 (FreeRTOS): Đảm bảo việc cập nhật vị trí vệ tinh GPS diễn ra liên tục trên một nhân xử lý (Core) riêng biệt, không làm treo hoặc trễ quá trình quét thẻ RFID và gửi tin nhắn mạng ở nhân xử lý còn lại.
- Thiết kế **Hệ thống cảnh báo kép dự phòng**: Mọi thao tác điểm danh ưu tiên gửi qua đường truyền Internet (Telegram). Nếu Chat ID không hợp lệ (độ dài < 10 ký tự) hoặc mạng Wi-Fi/Internet mất kết nối, hệ thống phải tự động chuyển hướng (fallback) qua gửi tin nhắn SMS để phụ huynh luôn nhận được thông báo.
- Giao diện nhận thông báo (qua Telegram/SMS) phải thân thiện, ngắn gọn, bao gồm trạng thái (LÊN/XUỐNG xe), tên học sinh, thời gian chính xác, và kèm theo liên kết vị trí bản đồ số.

## 2.3 Yêu cầu chức năng các phân hệ chính

### 2.3.1 Phân hệ Device (Thiết bị trung tâm)

Với phân hệ Device (vi điều khiển ESP32 và các module ngoại vi), hệ thống yêu cầu các chức năng sau:
- Tự động duy trì và kết nối lại (Auto-reconnect) mạng Wi-Fi.
- Phát tín hiệu vô tuyến tần số 13.56MHz để quét mã định danh (UID) của thẻ RFID.
- Dựa vào mã UID thu thập được, gửi HTTP GET tới Firebase để lấy thông tin phụ huynh và trạng thái In/Out hiện tại. Sau đó:
  - Gửi HTTP PUT tới Firebase để cập nhật đảo ngược trạng thái (VD: đang "in" thì chuyển thành "out").
  - Kích hoạt còi kêu 1 tiếng "bíp" ngắn xác nhận, đồng thời hiển thị thông tin lên màn hình OLED SH1106.
- Thực hiện đóng gói tin nhắn và gọi API Telegram (`sendMessage`) để thông báo.
- Nếu gửi Telegram thất bại, Device kích hoạt module SIM, phân tích chất lượng sóng vô tuyến (`AT+CSQ`) và thực thi chuỗi lệnh SMS (`AT+CMGS`).
- Định kỳ tự động quét máy chủ Telegram (sử dụng tính năng `getUpdates` API) để lắng nghe lệnh từ người dùng. Nếu nhận được từ khóa *"vitri"*, Device ngay lập tức trả về liên kết Google Maps vị trí thực tại của xe.

### 2.3.2 Phân hệ Server – Backend (Firebase & Telegram API)

Hệ thống không sử dụng máy chủ Backend tự xây dựng mà tận dụng các dịch vụ Cloud mạnh mẽ:
- **Dịch vụ Firebase RTDB:**
  - Nhận luồng HTTP GET/PUT từ thiết bị để trả về và cập nhật cơ sở dữ liệu dạng cây JSON.
  - Phải có khả năng xác thực bảo mật thông qua cấu hình `auth_token` tránh truy cập trái phép.
- **Dịch vụ Telegram Bot API:**
  - Nhận gói tin HTTP POST từ Device (bao gồm: Chat ID, nội dung cảnh báo, URL bản đồ).
  - Đảm bảo tính bảo mật (Mã hóa đường truyền SSL/TLS) và tốc độ truyền tải độ trễ cực thấp (< 2 giây).
  - Cung cấp API lịch sử tin nhắn để ESP32 có thể lấy lệnh truy vấn ngược từ người dùng.

### 2.3.3 Phân hệ Client App (Ứng dụng người dùng)

Tận dụng ứng dụng OTT Telegram, yêu cầu đối với người dùng (phụ huynh):
- Không cần cài đặt phần mềm bên thứ 3 phức tạp.
- Nhận và hiển thị các thông báo Push Notification ngay khi học sinh quẹt thẻ.
- Phân biệt rõ ràng các tình huống thông báo thông qua văn bản chi tiết (LÊN XE, XUỐNG XE).
- Có thể chủ động tương tác với thiết bị bằng cách gõ lệnh "vitri" hoặc "/vitri" vào Bot để kiểm tra vị trí xe buýt theo ý muốn.

## 2.4 Yêu cầu chức năng các phân hệ phụ

### 2.4.1 Phân hệ định vị toàn cầu GPS (NEO-6M/7M/8M)

- Thu thập thông số định vị từ các vệ tinh không gian qua anten.
- Truyền dữ liệu định dạng NMEA qua giao tiếp UART (`HardwareSerial 2`).
- Phân tích và chuyển đổi chuỗi tọa độ (Lat, Lng) kết hợp tính toán thời gian chuẩn múi giờ UTC+7.

### 2.4.2 Phân hệ Viễn thông (Module SIM)

- Giao tiếp với vi điều khiển bằng tập lệnh AT qua `HardwareSerial 1`.
- Yêu cầu xử lý ngắt nguồn Wi-Fi tạm thời khi gửi tin nhắn SMS để dồn điện áp và tránh nhiễu tín hiệu RF, đồng thời loại bỏ các kí tự rác trong bộ đệm nối tiếp (Serial buffer) nhằm đảm bảo lệnh thực thi chính xác.

## 2.5 Kết luận chương

Trên đây là tổng quan về thiết kế kiến trúc hệ thống giám sát học sinh xe buýt dựa trên kết nối Wi-Fi, Firebase và mạng viễn thông. Các chức năng, nhiệm vụ của thiết bị trạm (Device), cơ chế lưu trữ đám mây, cũng như giải pháp cảnh báo kép thông minh (Telegram kết hợp SMS fallback) đã được phân tách rõ ràng. Dựa vào mô hình này, các chương tiếp theo sẽ đi sâu vào thiết kế sơ đồ mạch phần cứng, cách triển khai thư viện phần mềm và đánh giá thực nghiệm.
