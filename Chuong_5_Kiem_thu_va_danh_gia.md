# CHƯƠNG 5. KIỂM THỬ VÀ ĐÁNH GIÁ KẾT QUẢ

Chương 5 trình bày kịch bản thực nghiệm, kết quả chạy thử các chức năng phần cứng lẫn phần mềm trong điều kiện thực tế, từ đó đưa ra những đánh giá khách quan về hiệu năng, sai số và tổng kết ưu/nhược điểm của hệ thống.

## 5.1 Kịch bản thực nghiệm và quy trình kiểm thử hệ thống

- **Mục đích:** Xác minh tính chính xác của thuật toán đọc thẻ RFID, tốc độ phản hồi của cơ sở dữ liệu Firebase và độ tin cậy của tính năng cảnh báo qua Telegram Bot.
- **Điều kiện thử nghiệm:** 
  - Mạch thiết bị được cấp nguồn qua cổng Type-C (5V) từ pin sạc dự phòng để tạo tính cơ động.
  - Thiết bị được đặt mô phỏng trên xe máy/ô tô di chuyển ngoài trời để đo bắt sóng GPS.
  - Sử dụng mạng Wi-Fi phát từ điện thoại di động để mô phỏng mạng Internet không dây trên xe buýt.
  - Chuẩn bị các thẻ RFID đại diện cho học sinh, thông tin UID đã được khai báo trước trong tập tin `data1.json` trên nền tảng Firebase RTDB kèm theo Chat ID Telegram tương ứng của phụ huynh.

## 5.2 Kết quả kiểm thử chế tạo phần cứng

### 5.2.1 Kết quả kiểm tra hoạt động của khối nguồn dưới tải
- Mạch nhận nguồn 5V trực tiếp từ cổng Type-C hoạt động rất ổn định, nguồn điện cấp cho vi điều khiển ESP32 và các module ngoại vi (GPS, RFID, OLED) được duy trì liên tục, không xảy ra hiện tượng sụt áp hay sập nguồn (Brownout Reset).
- Nhiệt độ hoạt động của mạch ổn định, các linh kiện phân phối nguồn không bị nóng, đảm bảo an toàn khi đóng trong hộp kín.

### 5.2.2 Đánh giá chất lượng gia công mạch in PCB
- Mạch in thực tế do xưởng gia công đúng chuẩn kích thước, các chân hàn linh kiện dán (SMD) và cắm (THT) không bị chạm chập.
- Khoảng cách giữa các khối linh kiện đảm bảo an toàn về can nhiễu điện từ. Màn hình OLED cắm trên Header khít, chắc chắn và hiển thị rõ nét ngoài trời sáng.

## 5.3 Kết quả thử nghiệm chức năng hệ thống

### 5.3.1 Kết quả nhận diện thẻ RFID và cập nhật trạng thái học sinh
- Khi quẹt thẻ RFID vào đầu đọc RC522 (khoảng cách 1-3cm), còi báo kêu 1 tiếng "bíp" ngắn lập tức.
- ESP32 đẩy tín hiệu HTTP GET/PUT lên Firebase. Firebase lật trạng thái cực nhanh (từ "in" sang "out" và ngược lại). Màn hình OLED hiển thị chính xác tên học sinh và dòng chữ "LÊN XE" hoặc "XUỐNG XE".
- **Kết quả:** Đạt yêu cầu 100%.

### 5.3.2 Kết quả bắt sóng vệ tinh định vị vị trí xe buýt
- Trong lần khởi động đầu tiên (Cold Start) ngoài trời, module ATGM336H mất khoảng 1-2 phút để bắt đủ số lượng vệ tinh (Fix 3D). Các lần sau mất chưa tới 5 giây.
- Sau khi có tín hiệu, mạch đi vào chế độ "HỆ THỐNG SẴN SÀNG". Tọa độ thu được liên tục chạy ngầm nhờ cơ chế đa luồng FreeRTOS trên Core 0.
- **Kết quả:** Đạt yêu cầu.

### 5.3.3 Kết quả gửi tin nhắn báo động qua Telegram
- Khi học sinh quét thẻ, thiết bị lấy Chat ID từ Firebase và gửi thông báo tức thời (gồm Tên học sinh, thời gian lên/xuống xe, và Link Google Maps vị trí xe) về ứng dụng Telegram của phụ huynh.
- Thời gian gửi và nhận tin nhắn qua Telegram diễn ra gần như ngay lập tức (độ trễ khoảng 1 giây), màn hình OLED báo "HOÀN TẤT".
- **Kết quả:** Tính năng thông báo thời gian thực hoạt động chính xác và ổn định.

### 5.3.4 Kết quả tương tác truy vấn vị trí từ xa (Lệnh "vitri")
- Phụ huynh nhắn tin `"vitri"` vào luồng chat của Bot Telegram.
- Thiết bị (do cứ 10 giây dùng hàm Polling `getUpdates` 1 lần) đã bắt được lệnh. Thiết bị đọc tọa độ GPS ngay lúc đó, đóng gói và phản hồi về khung chat một link bản đồ chính xác.
- **Kết quả:** Thành công. Tính năng tương tác hai chiều hoạt động trơn tru.

## 5.4 Đánh giá hiệu năng và phân tích sai số hệ thống

### 5.4.1 Đánh giá sai số định vị GPS
- Thử nghiệm bấm vào link Google Maps do thiết bị gửi và so sánh với vị trí định vị trên điện thoại iPhone. 
- **Đánh giá:** Sai số dao động dưới 10 mét trong điều kiện trời quang mây. Đây là mức sai số hoàn toàn có thể chấp nhận được đối với ứng dụng theo dõi xe buýt dân sự, đủ để phụ huynh biết xe đang ở khu vực tuyến đường nào.

### 5.4.2 Đánh giá độ trễ truyền dữ liệu qua mạng Wi-Fi và Telegram Bot
- **Truyền qua Internet (Wi-Fi/Firebase/Telegram):** Rất ưu việt, độ trễ trung bình < 2 giây. Đặc biệt hệ thống có thể xử lý lượng lớn học sinh lên xe liên tục mà không bị nghẽn mạng.
- **Đánh giá chung:** Tốc độ kết nối Wi-Fi phát từ bộ phát di động ổn định, Telegram Bot cho thời gian phản hồi rất nhanh, đáp ứng tốt nhu cầu giám sát thời gian thực.

## 5.5 Nhận xét ưu điểm và nhược điểm của thiết bị

### 5.5.1 Ưu điểm
- **Thông báo nhanh chóng:** Hệ thống gửi tin báo tức thời kèm vị trí thực tế của xe về ứng dụng Telegram của phụ huynh với độ trễ tối thiểu.
- **Quản lý thông minh:** Firebase RTDB giúp nhà trường dễ dàng chỉnh sửa thông tin học sinh trên máy tính mà không cần phải mang thiết bị về nạp lại code.
- **Hiệu năng cao:** Việc sử dụng đa luồng (FreeRTOS) tách biệt mạch xử lý GPS và xử lý RFID giúp loại bỏ hoàn toàn độ trễ phần cứng khi quẹt thẻ.
- **Chi phí thấp, tính ứng dụng cao:** Giá thành vật tư rẻ, tận dụng hạ tầng Telegram miễn phí giúp thiết bị hoàn toàn có thể thương mại hóa nhân rộng.

### 5.5.2 Nhược điểm
- Phụ thuộc vào kết nối Wi-Fi trên xe buýt. Nếu xe đi vào vùng mất sóng di động hoặc bộ phát Wi-Fi gặp sự cố, hệ thống tạm thời không thể đồng bộ dữ liệu và gửi cảnh báo.
- Thiết bị hiện tại chưa tích hợp khe cắm thẻ nhớ MicroSD. Do đó, nếu mất mạng diện rộng, dữ liệu quét thẻ tại thời điểm đó chưa thể lưu trữ ngoại tuyến (Offline) để đồng bộ lại sau.

---

## 5.6 Kết luận chương

Chương 5 đã trình bày chi tiết về quy trình và kết quả thử nghiệm thực tế của hệ thống giám sát xe buýt học sinh thông minh. Thông qua các kịch bản thực nghiệm, hệ thống đã chứng minh được tính ổn định và chính xác cao trong việc đọc thẻ RFID, định vị tọa độ vệ tinh GPS, cập nhật dữ liệu đám mây Firebase Realtime Database và gửi thông báo tức thời tới phụ huynh qua Telegram. Các phân tích về sai số định vị và độ trễ truyền tin đã khẳng định tính khả thi vượt trội của hệ thống dựa trên nền tảng vi điều khiển ESP32 kết hợp hệ điều hành thời gian thực FreeRTOS. Những ưu điểm và nhược điểm được chỉ ra trong chương này sẽ là tiền đề quan trọng để đưa ra đánh giá tổng thể và định hướng nâng cấp hệ thống ở chương tiếp theo.
