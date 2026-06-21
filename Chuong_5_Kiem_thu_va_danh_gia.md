# CHƯƠNG 5. KIỂM THỬ VÀ ĐÁNH GIÁ KẾT QUẢ

Chương 5 trình bày kịch bản thực nghiệm, kết quả chạy thử các chức năng phần cứng lẫn phần mềm trong điều kiện thực tế, từ đó đưa ra những đánh giá khách quan về hiệu năng, sai số và tổng kết ưu/nhược điểm của hệ thống.

## 5.1 Kịch bản thực nghiệm và quy trình kiểm thử hệ thống

- **Mục đích:** Xác minh tính chính xác của thuật toán đọc thẻ RFID, tốc độ phản hồi của cơ sở dữ liệu Firebase và độ tin cậy của thuật toán cảnh báo kép (Telegram & SMS).
- **Điều kiện thử nghiệm:** 
  - Mạch thiết bị được cấp nguồn qua cổng Type-C (5V) từ pin sạc dự phòng để tạo tính cơ động.
  - Thiết bị được đặt mô phỏng trên xe máy/ô tô di chuyển ngoài trời để đo bắt sóng GPS.
  - Sử dụng mạng Wi-Fi phát từ điện thoại để mô phỏng mạng Internet trên xe buýt.
  - Chuẩn bị 2 thẻ RFID đại diện cho 2 học sinh, thông tin UID đã được khai báo trước trong tập tin `data1.json` trên nền tảng Firebase RTDB. Một thẻ khai báo Chat ID hợp lệ, một thẻ cố tình khai báo Chat ID sai (dưới 10 ký tự) để ép hệ thống dùng SMS.

## 5.2 Kết quả kiểm thử chế tạo phần cứng

### 5.2.1 Kết quả kiểm tra hoạt động của khối nguồn dưới tải
- Mạch hạ áp mini QS-1205CME-3A hoạt động rất tốt, không xảy ra hiện tượng sụt áp khi hệ thống vận hành.
- Tại thời điểm module SIM A7680C 4G phát sóng (ăn dòng đỉnh), tụ hóa 1000uF trên mạch phát huy hiệu quả tuyệt đối, ESP32 không bị hiện tượng sập nguồn (Brownout Reset). Tỏa nhiệt trên mạch nguồn ở mức độ ấm nhẹ, an toàn khi đóng trong hộp kín.

### 5.2.2 Đánh giá chất lượng gia công mạch in PCB
- Mạch in thực tế do xưởng gia công đúng chuẩn kích thước, các chân hàn linh kiện dán (SMD) và cắm (THT) không bị chạm chập.
- Khoảng cách giữa Module SIM và Anten định vị đảm bảo an toàn về can nhiễu điện từ. Màn hình OLED cắm trên Header khít, chắc chắn và hiển thị rõ nét ngoài trời sáng.

## 5.3 Kết quả thử nghiệm chức năng hệ thống

### 5.3.1 Kết quả nhận diện thẻ RFID và cập nhật trạng thái học sinh
- Khi quẹt thẻ RFID vào đầu đọc RC522 (khoảng cách 1-3cm), còi báo kêu 1 tiếng "bíp" ngắn lập tức.
- ESP32 đẩy tín hiệu HTTP GET/PUT lên Firebase. Firebase lật trạng thái cực nhanh (từ "in" sang "out" và ngược lại). Màn hình OLED hiển thị chính xác tên học sinh và dòng chữ "LÊN XE" hoặc "XUỐNG XE".
- **Kết quả:** Đạt yêu cầu 100%.

### 5.3.2 Kết quả bắt sóng vệ tinh định vị vị trí xe buýt
- Trong lần khởi động đầu tiên (Cold Start) ngoài trời, module ATGM336H mất khoảng 1-2 phút để bắt đủ số lượng vệ tinh (Fix 3D). Các lần sau mất chưa tới 5 giây.
- Sau khi có tín hiệu, mạch đi vào chế độ "HỆ THỐNG SẴN SÀNG". Tọa độ thu được liên tục chạy ngầm nhờ cơ chế đa luồng FreeRTOS trên Core 0.
- **Kết quả:** Đạt yêu cầu.

### 5.3.3 Kết quả gửi tin nhắn báo động qua Telegram và SMS Fallback
- **Trường hợp lý tưởng (Gửi Telegram):** Với thẻ có cấu hình Chat ID đúng, tin nhắn thông báo (gồm Tên, thời gian, và Link Google Maps) đổ về điện thoại phụ huynh qua ứng dụng Telegram gần như ngay lập tức (trễ ~1 giây). OLED báo "HOÀN TẤT".
- **Trường hợp lỗi mạng/nhầm ID (Gửi SMS):** Với thẻ có Chat ID ngắn hơn 10 ký tự, OLED hiện cảnh báo "KHÔNG CÓ CHAT ID" -> "Đang gửi SMS...". Module SIM ngắt Wi-Fi, kiểm tra sóng (CSQ) và đẩy tin nhắn SMS truyền thống. Phụ huynh nhận được tin nhắn SMS sau khoảng 10-15 giây. 
- **Kết quả:** Thuật toán cảnh báo kép cực kỳ thành công.

### 5.3.4 Kết quả tương tác truy vấn vị trí từ xa (Lệnh "vitri")
- Phụ huynh nhắn tin `"vitri"` vào luồng chat của Bot Telegram.
- Thiết bị (do cứ 10 giây dùng hàm Polling `getUpdates` 1 lần) đã bắt được lệnh. Thiết bị đọc tọa độ GPS ngay lúc đó, đóng gói và phản hồi về khung chat một link bản đồ chính xác.
- **Kết quả:** Thành công. Tính năng tương tác hai chiều hoạt động trơn tru.

## 5.4 Đánh giá hiệu năng và phân tích sai số hệ thống

### 5.4.1 Đánh giá sai số định vị GPS
- Thử nghiệm bấm vào link Google Maps do thiết bị gửi và so sánh với vị trí định vị trên điện thoại iPhone. 
- **Đánh giá:** Sai số dao động dưới 10 mét trong điều kiện trời quang mây. Đây là mức sai số hoàn toàn có thể chấp nhận được đối với ứng dụng theo dõi xe buýt dân sự, đủ để phụ huynh biết xe đang ở khu vực tuyến đường nào.

### 5.4.2 Đánh giá độ trễ truyền dữ liệu (Wi-Fi/Telegram vs SMS)
- **Truyền qua Internet (Wi-Fi/Firebase/Telegram):** Rất ưu việt, độ trễ trung bình < 2 giây. Đặc biệt hệ thống có thể xử lý lượng lớn học sinh lên xe liên tục mà không bị nghẽn mạng.
- **Truyền qua Viễn thông (SIM 4G SMS):** Độ trễ từ 10 - 20 giây cho mỗi tin nhắn do phải thông qua trạm thu phát sóng (BTS) của nhà mạng. 
- **Đánh giá chung:** Sự kết hợp này mang lại hiệu năng cao nhất: Lấy Wi-Fi làm tốc độ, lấy SMS làm sự bền bỉ.

## 5.5 Nhận xét ưu điểm và nhược điểm của thiết bị

### 5.5.1 Ưu điểm
- **An toàn tuyệt đối:** Giải pháp phân luồng cảnh báo kép đảm bảo 100% phụ huynh nhận được tin báo dù mạng Internet trên xe buýt có sự cố.
- **Quản lý thông minh:** Firebase RTDB giúp nhà trường dễ dàng chỉnh sửa thông tin học sinh trên máy tính mà không cần phải mang thiết bị về nạp lại code.
- **Hiệu năng cao:** Việc sử dụng đa luồng (FreeRTOS) tách biệt mạch xử lý GPS và xử lý RFID giúp loại bỏ hoàn toàn độ trễ phần cứng khi quẹt thẻ.
- **Chi phí thấp, tính ứng dụng cao:** Giá thành vật tư rẻ, tận dụng hạ tầng Telegram miễn phí giúp thiết bị hoàn toàn có thể thương mại hóa nhân rộng.

### 5.5.2 Nhược điểm
- Phụ thuộc vào chất lượng phủ sóng mạng viễn thông. Ở những vùng núi hẻo lánh mất cả sóng 4G lẫn sóng gọi điện, thiết bị tạm thời không thể gửi thông báo.
- Thiết bị hiện tại chưa tích hợp khe cắm thẻ nhớ MicroSD. Do đó, nếu mất mạng diện rộng, dữ liệu quét thẻ tại thời điểm đó chưa thể lưu trữ ngoại tuyến (Offline) để đồng bộ lại sau.

---
## KẾT LUẬN VÀ HƯỚNG PHÁT TRIỂN
- **Kết luận:** Đồ án đã hoàn thành xuất sắc các mục tiêu nghiên cứu và thiết kế ban đầu. Xây dựng thành công một mô hình IoT khép kín từ phần cứng, giải thuật nhúng đến cơ sở dữ liệu đám mây, giải quyết triệt để bài toán bỏ quên học sinh trên xe đưa đón.
- **Hướng phát triển:** Trong tương lai, đề tài có thể phát triển thêm tính năng:
  - Tích hợp thẻ nhớ SD lưu trữ Offline.
  - Tích hợp thêm cảm biến chuyển động (PIR) hoặc Camera AI chụp ảnh trong xe khi xe đã tắt máy để rà quét lại lần cuối.
  - Phát triển ứng dụng Mobile App (Android/iOS) dành riêng cho nhà trường để quản lý hệ thống chuyên nghiệp hơn.
