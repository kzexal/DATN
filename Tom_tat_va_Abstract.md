# TÓM TẮT ĐỒ ÁN

Trong những năm gần đây, tình trạng bỏ quên học sinh trên các phương tiện đưa đón đã trở thành một vấn đề nhức nhối, gây ra những hậu quả nghiêm trọng về mặt an toàn và tâm lý. Nhằm giải quyết triệt để bài toán này, đồ án tiến hành nghiên cứu, thiết kế và chế tạo **"Hệ thống điểm danh và giám sát hành trình xe buýt học sinh thông minh sử dụng công nghệ IoT"**.

Hệ thống được xây dựng dựa trên vi điều khiển trung tâm lõi kép ESP32 kết hợp với hệ điều hành nhúng thời gian thực FreeRTOS, cho phép xử lý đa luồng dữ liệu liên tục mà không gây trễ phần cứng. Về mặt thiết bị, sản phẩm tích hợp đầu đọc thẻ RFID RC522 để định danh học sinh khi lên/xuống xe, module định vị vệ tinh ATGM336H thu thập tọa độ địa lý, cùng màn hình OLED 1.3 inch hiển thị thông tin trực quan. Toàn bộ dữ liệu thu thập được đồng bộ hóa tự động lên cơ sở dữ liệu đám mây Firebase Realtime Database.

Điểm nổi bật của đề tài là khả năng đồng bộ dữ liệu nhanh chóng và gửi cảnh báo tức thì. Hệ thống sử dụng kết nối Wi-Fi để đẩy thông báo trạng thái kèm vị trí chính xác (thông qua liên kết Google Maps) đến điện thoại của phụ huynh thông qua nền tảng Telegram Bot. Phụ huynh cũng có thể chủ động theo dõi và nắm bắt trạng thái an toàn của con em mình một cách dễ dàng và trực quan. Kết quả thực nghiệm thực tế cho thấy hệ thống hoạt động ổn định, độ sai số định vị thấp và thời gian phản hồi thông báo đạt dưới 2 giây, mở ra một giải pháp quản lý toàn diện, tiết kiệm chi phí và có tính khả thi cao để triển khai tại các trường học.

---

# ABSTRACT

In recent years, the issue of leaving students behind on school buses has become a pressing concern, leading to severe safety and psychological consequences. To comprehensively address this problem, this thesis proposes, designs, and develops a **"Smart School Bus Attendance and Route Monitoring System using IoT Technology"**.

The system is built upon the ESP32 dual-core microcontroller integrated with the FreeRTOS real-time operating system, enabling continuous multithreaded data processing without hardware latency. Regarding the physical device, it incorporates an RFID RC522 reader to identify students during boarding and alighting, an ATGM336H GPS module to collect geographic coordinates, and a 1.3-inch OLED display for intuitive on-site feedback. All collected data is instantaneously synchronized to the Firebase Realtime Database cloud platform.

A standout feature of this project is its rapid data synchronization and instant alert capabilities. The system utilizes a Wi-Fi connection to push real-time status notifications along with precise locations (via Google Maps links) directly to parents' smartphones through a Telegram Bot. This enables parents to proactively monitor and grasp their children's safety status easily and intuitively. Field experimental results demonstrate that the system operates stably, maintains low positioning error, and achieves notification response times of under 2 seconds, offering a comprehensive, cost-effective, and highly feasible management solution for schools.
