# CHƯƠNG 6. KẾT LUẬN VÀ HƯỚNG PHÁT TRIỂN

Chương 6 tổng kết lại toàn bộ kết quả nghiên cứu và thực nghiệm đạt được của đề tài "Hệ thống điểm danh học sinh trên xe buýt thông minh sử dụng công nghệ IoT và RFID", đồng thời chỉ ra các hạn chế còn tồn tại và đề xuất định hướng phát triển nhằm nâng cao tính ứng dụng của thiết bị trong tương lai.

## 6.1 Kết luận

Qua quá trình nghiên cứu lý thuyết, thiết kế phần cứng, lập trình giải thuật nhúng và thử nghiệm vận hành thực tế, đề tài đã hoàn thành xuất sắc các mục tiêu nghiên cứu đặt ra ban đầu. Các kết quả cụ thể đạt được bao gồm:

### 6.1.1 Kết quả về mặt lý thuyết và thiết kế hệ thống
* **Nghiên cứu công nghệ:** Làm chủ giải pháp định vị vệ tinh GPS kết hợp truyền thông không dây Wi-Fi trong việc giám sát hành trình thời gian thực.
* **Hệ điều hành nhúng:** Áp dụng thành công cơ chế đa nhiệm thời gian thực (FreeRTOS) trên vi điều khiển lõi kép ESP32, phân tách luồng xử lý dữ liệu GPS (Core 0) độc lập với luồng quét thẻ RFID và truyền nhận API (Core 1), giải quyết triệt để hiện tượng nghẽn lệnh và trễ phần cứng.
* **Thiết kế mạch nguyên lý & PCB:** Làm chủ quy trình thiết kế mạch nguyên lý (Schematic) và layout mạch in (PCB) 2 lớp trên phần mềm Altium Designer, bố trí linh kiện khoa học để hạn chế tối đa can nhiễu điện từ giữa các khối định vị GPS và truyền nhận dữ liệu.

### 6.1.2 Kết quả về chế tạo phần cứng thực tế (Device)
* **Thi công mạch in:** Chế tạo thành công board mạch thành phẩm nhỏ gọn, hoạt động bền bỉ, sử dụng nguồn cấp trực tiếp 5V qua cổng Type-C an toàn, cơ động và tương thích tốt với các nguồn sạc dự phòng thông dụng.
* **Tích hợp ngoại vi:** Đồng bộ hóa hoạt động của đầu đọc RFID RC522 để nhận diện thẻ học sinh cực nhạy (khoảng cách 1-3cm), định vị GPS ổn định với sai số thấp và màn hình hiển thị OLED 1.3 inch hiển thị trực quan thông tin lên/xuống xe tại chỗ.
* **Tính mở của thiết kế:** Thiết kế bo mạch được chừa sẵn các phân khu và đường mạch chờ để dễ dàng tích hợp thêm module SIM 4G, mạch nguồn hạ áp và các linh kiện phụ trợ khác như một tùy chọn nâng cấp về sau mà không cần thiết kế lại bo mạch từ đầu.

### 6.1.3 Kết quả về mặt phần mềm và truyền thông
* **Quản trị dữ liệu đám mây:** Kết nối thành công vi điều khiển ESP32 với cơ sở dữ liệu thời gian thực Firebase Realtime Database. Dữ liệu học sinh được đồng bộ hóa và lưu trữ tập trung trên Cloud, cho phép quản trị viên cập nhật thông tin thẻ từ xa mà không cần can thiệp vật lý vào thiết bị.
* **Cảnh báo thời gian thực:** Triển khai thành công giải thuật cảnh báo tức thời: gửi thông báo trạng thái kèm liên kết vị trí Google Maps của xe về ứng dụng Telegram của phụ huynh qua kết nối Wi-Fi với tốc độ cao (độ trễ dưới 2 giây, chi phí vận hành bằng không).
* **Tương tác hai chiều:** Tích hợp thành công tính năng truy vấn vị trí từ xa. Phụ huynh có thể nhắn tin lệnh `vitri` đến Bot Telegram của hệ thống, thiết bị trên xe sẽ tự động phản hồi tọa độ thời gian thực dưới dạng liên kết bản đồ chỉ sau vài giây.

### 6.1.4 Hạn chế còn tồn tại của đề tài
Dù đạt được nhiều kết quả khả quan, hệ thống vẫn còn một số hạn chế cần khắc phục:
* **Phụ thuộc kết nối mạng:** Hệ thống hoàn toàn phụ thuộc vào kết nối Wi-Fi trên xe buýt. Nếu xe di chuyển vào vùng mất sóng di động diện rộng hoặc bộ phát Wi-Fi gặp sự cố, hệ thống không thể gửi cảnh báo thời gian thực.
* **Lưu trữ Offline:** Thiết bị chưa được tích hợp bộ nhớ lưu trữ ngoài (thẻ SD) để lưu tạm lịch sử quẹt thẻ ngoại tuyến khi mất mạng, nhằm đồng bộ bù khi có kết nối trở lại.
* **Kích thước phần cứng:** Kích thước mạch in thử nghiệm vẫn còn tương đối lớn, chưa được tối ưu hóa bằng các linh kiện dán SMD siêu nhỏ và vỏ hộp bảo vệ chưa đạt chuẩn công nghiệp chống nước, chống va đập.

---

## 6.2 Hướng phát triển của đề tài

Để thiết bị có thể nhân rộng thực tế và thương mại hóa chuyên nghiệp, đề tài đề xuất tập trung nghiên cứu phát triển các hướng sau trong tương lai:

### 6.2.1 Tích hợp module truyền thông di động GSM/LTE và cảnh báo SMS dự phòng
* **Giải pháp kỹ thuật:** Lắp đặt thêm module SIM 4G (như A7680C), mạch hạ áp chuyên dụng và tụ hóa lọc nguồn dung lượng lớn (1000uF) vào các chân chờ sẵn trên bo mạch.
* **Cơ chế hoạt động:** Phát triển thuật toán cảnh báo kép. Khi thiết bị mất kết nối Wi-Fi, hệ thống tự động chuyển sang sử dụng mạng di động qua module SIM để gửi tin nhắn SMS truyền thống tới số điện thoại phụ huynh, đảm bảo thông tin luôn được thông suốt ngay cả khi không có Internet.

### 6.2.2 Tích hợp bộ nhớ lưu trữ ngoại tuyến (Offline Storage & Sync)
* **Giải pháp kỹ thuật:** Thiết kế thêm khe cắm thẻ nhớ MicroSD trên bo mạch giao tiếp qua chuẩn SPI hoặc SDIO.
* **Cơ chế hoạt động:** Khi thiết bị phát hiện mất mạng, toàn bộ dữ liệu lịch sử quẹt thẻ (UID, thời gian, tọa độ GPS) sẽ được mã hóa và lưu vào thẻ nhớ. Ngay khi có mạng trở lại, hệ thống sẽ tự động đồng bộ toàn bộ dữ liệu này lên Firebase.

### 6.2.3 Tích hợp các cảm biến an toàn chủ động trong khoang xe
* **Cảm biến chuyển động (PIR/Radar vi sóng):** Lắp đặt các cảm biến quét chuyển động trên trần xe buýt. Khi tài xế tắt máy và khóa cửa xe, nếu phát hiện có học sinh bị bỏ quên di chuyển trong xe, hệ thống sẽ kích hoạt còi báo động và gửi thông báo khẩn cấp đến ban giám hiệu.
* **Hệ thống Camera AI đếm người:** Lắp đặt camera ở cửa lên xuống kết hợp mạch xử lý Edge AI để đếm số lượng người lên/xuống thực tế, đối chiếu chéo với số lượng thẻ RFID đã quẹt nhằm phát hiện học sinh quên quẹt thẻ hoặc bị sót lại trên xe.

### 6.2.4 Phát triển hệ sinh thái phần mềm quản lý chuyên dụng (Mobile App)
* **Ứng dụng dành cho phụ huynh:** Phát triển ứng dụng di động chuyên dụng (Android/iOS) hiển thị bản đồ trực tuyến vị trí xe di chuyển, thời gian ước tính xe đến điểm đón (ETA) và lịch sử đi xe của con.
* **Ứng dụng dành cho tài xế và giám thị:** Hiển thị danh sách học sinh cần đón/trả tại mỗi điểm dừng, tích hợp nút nhấn khẩn cấp SOS để báo về trung tâm khi xe gặp sự cố.

### 6.2.5 Tối ưu hóa thiết kế phần cứng công nghiệp
* **Thiết kế mạch in 4 lớp:** Thu nhỏ tối đa kích thước bo mạch bằng cách chuyển đổi toàn bộ linh kiện cắm sang linh kiện dán SMD, đi dây mạch in 4 lớp để chống nhiễu tín hiệu tốt hơn.
* **Tích hợp nguồn pin dự phòng:** Thiết kế thêm mạch quản lý sạc pin Lithium và tích hợp pin dự phòng trực tiếp trên bo mạch để duy trì hoạt động từ 4-6 tiếng khi nguồn điện chính của xe bị ngắt đột ngột.

---

## 6.3 Kết luận chương

Chương 6 đã tổng kết toàn diện những kết quả nghiên cứu lý thuyết và chế tạo thực nghiệm đạt được của đề tài "Hệ thống điểm danh học sinh trên xe buýt thông minh sử dụng công nghệ IoT và RFID". Mặc dù hệ thống đã hoàn thành xuất sắc các mục tiêu ban đầu về định danh, định vị và cảnh báo thời gian thực qua Wi-Fi/Telegram, báo cáo cũng thẳng thắn chỉ ra các điểm hạn chế cần khắc phục. Những định hướng phát triển chi tiết như tích hợp module SIM 4G gửi SMS dự phòng, tích hợp thẻ nhớ SD, bổ sung camera AI, cảm biến PIR và phát triển ứng dụng di động chuyên dụng đã vạch ra lộ trình rõ ràng để nâng cấp thiết bị thành một sản phẩm thương mại hoàn chỉnh, đóng góp thiết thực cho giải pháp an toàn học đường trong kỷ nguyên số.
