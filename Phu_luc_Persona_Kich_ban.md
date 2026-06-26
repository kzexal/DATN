# PHỤ LỤC: KỊCH BẢN SỬ DỤNG, PERSONA VÀ HƯỚNG DẪN ĐĂNG KÝ THÔNG TIN PHỤ HUYNH

**A. Nhân vật đại diện (Persona)**

**Nhân vật 1: Phụ huynh**
- **Tên:** Chị Mai
- **Tuổi:** 35
- **Vai trò:** Phụ huynh có con đang sử dụng dịch vụ xe buýt đưa đón của nhà trường.
- **Kinh nghiệm:** Sử dụng thành thạo điện thoại thông minh và các ứng dụng nhắn tin (Telegram, Zalo). Trước đây thường xuyên phải gọi điện cho tài xế hoặc nhà trường để hỏi thăm tình hình của con.
- **Mục tiêu:** Theo dõi được chính xác thời gian con lên/xuống xe và biết được vị trí của xe buýt theo thời gian thực để an tâm và chủ động sắp xếp thời gian đón con.

**Nhân vật 2: Quản trị viên**
- **Tên:** Thầy Hùng
- **Tuổi:** 45
- **Vai trò:** Giám thị / Quản trị viên hệ thống tại trường THPT.
- **Kinh nghiệm:** 5 năm quản lý học sinh đi xe buýt bằng phương pháp điểm danh giấy thủ công và bảng tính Excel.
- **Mục tiêu:** Đơn giản hóa quy trình điểm danh, kiểm soát chính xác số lượng học sinh lên/xuống xe, dễ dàng cập nhật thông tin thẻ của học sinh.

**B. Ngữ cảnh**
Nhà trường vừa triển khai Hệ thống Điểm danh Xe buýt IoT thông minh. Chị Mai đã cài đặt ứng dụng Telegram và đăng ký nhận thông báo. Thầy Hùng chịu trách nhiệm giám sát và quản lý dữ liệu trên Firebase. Hằng ngày, học sinh sẽ dùng thẻ RFID để quẹt mỗi khi lên hoặc xuống xe.

**C. Điều kiện tiên quyết**
- Hệ thống thiết bị trên xe (ESP32, RFID, GPS) đã được cấp nguồn và kết nối mạng Wi-Fi ổn định.
- Học sinh mang theo thẻ RFID hợp lệ đã được lưu mã UID trên hệ thống.
- Phụ huynh có thiết bị di động kết nối Internet và đã đăng ký Chat ID thành công.

**D. Hậu điều kiện**
- Mọi hoạt động lên/xuống xe của học sinh đều được hệ thống ghi nhận trên cơ sở dữ liệu Firebase.
- Phụ huynh nhận được thông báo ngay lập tức với độ trễ thấp.
- Tọa độ GPS của xe buýt được đính kèm vào tin nhắn dưới dạng liên kết bản đồ.
- Quản trị viên luôn theo dõi được sĩ số học sinh thực tế trên xe.

**E. Kịch bản sử dụng cụ thể**

**Tình huống 1: Học sinh quẹt thẻ khi lên xe**
- **Mục tiêu của người dùng:** Phụ huynh (Chị Mai) muốn biết ngay lập tức thời điểm con mình đã lên xe buýt an toàn để yên tâm đi làm.
- **Diễn tiến hành động – hệ thống:**
  - Học sinh bước lên xe và quẹt thẻ RFID vào đầu đọc của thiết bị.
  - Hệ thống nhận diện thẻ, phát tiếng "bíp", màn hình OLED hiển thị tên học sinh và trạng thái "IN".
  - Hệ thống xử lý, đảo trạng thái trên Firebase và gọi Telegram API.
  - Điện thoại của chị Mai lập tức nhận được thông báo: *"Học sinh [Tên] đã LÊN xe buýt. Thời gian: 06:45:12. Vị trí: [Link Google Maps]"*.
- **Lợi ích:** Phụ huynh hoàn toàn an tâm khi biết con đã lên xe đúng giờ, không cần phải tốn thời gian gọi điện hỏi tài xế hay giám thị.

**Tình huống 2: Phụ huynh chủ động truy vấn vị trí xe buýt**
- **Mục tiêu của người dùng:** Chị Mai muốn biết xe buýt hiện đang ở đâu để chuẩn bị ra điểm dừng đón con vào buổi chiều, tránh việc phải đứng chờ lâu dưới trời mưa.
- **Diễn tiến hành động – hệ thống:**
  - Chị Mai mở ứng dụng Telegram, vào khung chat với Bot của nhà trường.
  - Chị Mai gõ lệnh `vitri` và nhấn gửi.
  - Hệ thống trên xe (quét tin nhắn mỗi 10 giây) nhận được lệnh và lấy tọa độ GPS mới nhất từ vệ tinh.
  - Hệ thống tự động gửi tin nhắn phản hồi lại cho chị Mai kèm theo liên kết Google Maps chứa vị trí hiện tại của xe.
  - Chị Mai bấm vào liên kết để xem bản đồ.
- **Lợi ích:** Giúp phụ huynh chủ động hoàn toàn về thời gian đón rước, mang lại sự tiện lợi tối đa.

**Tình huống 3: Quản trị viên cập nhật thẻ RFID mới cho học sinh**
- **Mục tiêu của người dùng:** Học sinh làm mất thẻ, Thầy Hùng cần cấp lại thẻ mới và cập nhật mã UID lên hệ thống.
- **Diễn tiến hành động – hệ thống:**
  - Thầy Hùng truy cập vào bảng điều khiển (Console) của cơ sở dữ liệu Firebase.
  - Tìm đến bản ghi của học sinh bị mất thẻ.
  - Thay thế mã UID cũ bằng mã UID của thẻ RFID mới và bấm lưu lại.
  - Ngày hôm sau, học sinh sử dụng thẻ mới để quẹt, hệ thống trên xe quét thẻ, đối chiếu với Firebase thành công và gửi thông báo bình thường.
- **Lợi ích:** Quản lý dữ liệu tập trung trên Cloud giúp Thầy Hùng dễ dàng cập nhật thông tin ngay trên máy tính mà không cần phải mang máy tính ra cắm cáp trực tiếp vào thiết bị trên xe buýt.

**F. Lợi ích dành cho Nhà Trường / Chủ Đầu Tư**
- **Tiết kiệm chi phí vận hành:** Tự động hóa quá trình điểm danh, giảm thiểu các giấy tờ sổ sách và giảm bớt nhân công giám sát thủ công trên các tuyến xe.
- **Tăng hiệu quả quản lý:** Giao diện cơ sở dữ liệu trực quan, cập nhật Real-time giúp nhà trường dễ dàng thống kê và kiểm soát số lượng học sinh đi xe buýt mỗi ngày một cách nhanh chóng.
- **Đảm bảo an toàn, minh bạch:** Mọi lịch sử quẹt thẻ (track log) đều được lưu lại với thời gian và tọa độ thực tế, hạn chế tối đa nguy cơ bỏ quên học sinh trên xe, nâng cao uy tín của nhà trường.
- **Tính mở rộng và linh hoạt:** Kiến trúc hệ thống IoT cho phép dễ dàng nâng cấp, thêm chức năng mới (như tích hợp camera đếm người, cảnh báo nhiệt độ) theo nhu cầu phát triển của trường.
- **Nâng cao trải nghiệm người dùng:** Phụ huynh cảm thấy yên tâm và hài lòng hơn với dịch vụ đưa đón của trường. Giám thị và tài xế giảm được áp lực công việc, cải thiện chất lượng dịch vụ.

---

**G. Hướng dẫn đăng ký thông tin dành cho phụ huynh (Trước khi học sinh đi xe buýt lần đầu)**

Để hệ thống IoT trên xe buýt có thể gửi thông báo tự động về thời gian lên/xuống xe cũng như tọa độ vị trí xe theo thời gian thực tới phụ huynh, Quý phụ huynh vui lòng hoàn thành **02 bước** đăng ký thông tin dưới đây để gửi cho Ban quản lý xe của nhà trường trước ngày bắt đầu:

* **Bước 1: Lấy mã số định danh cá nhân (Telegram Chat ID)**
  Hệ thống sử dụng nền tảng Telegram để gửi thông báo tự động. Mỗi tài khoản Telegram được định danh bằng một chuỗi số ID duy nhất. Phụ huynh thực hiện lấy mã này như sau:
  1. Mở ứng dụng Telegram trên thiết bị di động.
  2. Tại thanh tìm kiếm, nhập từ khóa **`idbot`** (hoặc truy cập trực tiếp username `@myidbot`).
  3. Chọn bot tương ứng và nhấn nút **Start** (hoặc gửi tin nhắn chứa nội dung lệnh **`/start`**).
  4. Bot sẽ phản hồi một tin nhắn chứa mã số định danh của phụ huynh (Ví dụ: `Your user ID: 123456789`). Phụ huynh sao chép (copy) chuỗi số này.
  5. Gửi mã ID vừa lấy cho Ban quản lý xe của nhà trường để đăng ký nhận thông báo.

* **Bước 2: Cung cấp thông tin học sinh và thông tin liên lạc dự phòng**
  Phụ huynh gửi kèm các thông tin sau để nhà trường cập nhật vào cơ sở dữ liệu Firebase:
  1. **Họ và tên học sinh** (Ví dụ: *Nguyễn Văn A - Lớp 10A1*) để liên kết với mã thẻ RFID tương ứng.
  2. **Số điện thoại của phụ huynh**: Dùng làm thông tin liên hệ dự phòng và để chuẩn bị cấu hình hệ thống gửi tin nhắn SMS cảnh báo trong các phiên bản cải tiến/nâng cấp tiếp theo của dự án (hiện tại hệ thống đang tập trung gửi thông tin qua ứng dụng Telegram).

---

**H. Danh mục linh kiện và dự toán chi phí hệ thống (Bill of Materials - BOM)**

Để chế tạo phần cứng thiết bị điểm danh xe buýt IoT, các linh kiện sử dụng và chi phí ước tính (tham khảo theo thị trường linh kiện điện tử Việt Nam) được thống kê chi tiết trong bảng dưới đây:

| STT | Tên linh kiện / Thiết bị | Thông số kỹ thuật / Vai trò | Phân loại | Số lượng | Đơn giá (VND) | Thành tiền (VND) |
| :--- | :--- | :--- | :---: | :---: | :---: | :---: |
| 1 | Bo mạch ESP32 DevKit V1 | Vi điều khiển trung tâm tích hợp Wi-Fi/Bluetooth. | Cốt lõi | 01 | 100.000 | 100.000 |
| 2 | Module GPS ATGM336H | Module định vị GPS/BDS kèm anten chủ động. | Cốt lõi | 01 | 250.000 | 250.000 |
| 3 | Module đọc thẻ RFID RC522 | Đầu đọc thẻ RFID tần số 13.56MHz. | Cốt lõi | 01 | 30.000 | 30.000 |
| 4 | Màn hình OLED 1.3 inch | Hiển thị trạng thái lên/xuống xe và thông báo. | Cốt lõi | 01 | 90.000 | 90.000 |
| 5 | Cổng Type-C Female | Cấp nguồn 5V và nạp chương trình. | Cốt lõi | 01 | 5.000 | 5.000 |
| 6 | Công tắc gạt bập bênh nhỏ | Đóng/ngắt nguồn cung cấp vật lý. | Cốt lõi | 01 | 3.000 | 3.000 |
| 7 | Tụ hóa 47uF | Lọc nguồn thứ cấp chống nhiễu tần số thấp. | Cốt lõi | 01 | 1.000 | 1.000 |
| 8 | Tụ gốm cắm 100nF (Mã 104) | Lọc nhiễu tần số cao trên đường nguồn. | Cốt lõi | 01 | 1.000 | 1.000 |
| 9 | Còi chíp 5V (Active Buzzer) | Phát tiếng bíp ngắn khi quẹt thẻ thành công. | Cốt lõi | 01 | 15.000 | 15.000 |
| 10 | Các phụ kiện phụ trợ | Vỏ hộp nhựa ABS, dây nối, mạch in PCB, thẻ RFID. | Cốt lõi | - | 100.000 | 100.000 |
| 11 | Module SIM A7680C 4G | Module truyền thông LTE Cat-1 gửi SMS dự phòng. | **Tùy chọn cải tiến** | 01 | 200.000 | 200.000 |
| 12 | Mạch hạ áp Buck QS-1205CME-3A | Hạ áp từ 5V xuống 3.8V cho module SIM. | **Tùy chọn cải tiến** | 01 | 25.000 | 25.000 |
| 13 | Tụ hóa 1000uF (10V - 25V) | Ổn định dòng đỉnh khi module SIM phát sóng. | **Tùy chọn cải tiến** | 01 | 2.000 | 2.000 |
| 14 | Tụ gốm cắm 100nF (Mã 104) | Lọc nhiễu bổ sung cho đường nguồn module SIM. | **Tùy chọn cải tiến** | 01 | 1.000 | 1.000 |
| | **Tổng chi phí linh kiện cơ bản (Cơ sở cốt lõi)** | | | | | **595.000** |
| | **Tổng chi phí linh kiện đầy đủ (Sau cải tiến thêm)** | | | | | **823.000** |

*Lưu ý: Đơn giá thực tế có thể thay đổi tùy thuộc vào thời điểm mua hàng và đơn vị cung cấp linh kiện.*

---

**I. Hướng dẫn cấu hình cơ sở dữ liệu Firebase dành cho Quản trị viên**

Hệ thống điểm danh xe buýt sử dụng cơ sở dữ liệu Firebase Realtime Database làm trung tâm lưu trữ thông tin học sinh và đồng bộ trạng thái thực tế. Để hệ thống hoạt động chính xác, Quản trị viên cần cấu hình dữ liệu theo hướng dẫn sau:

##### **Bước 1: Khởi tạo Project trên Firebase Console**
1. Truy cập vào trang quản trị Firebase tại địa chỉ: [console.firebase.google.com](https://console.firebase.google.com/).
2. Đăng nhập bằng tài khoản Google, nhấn **Add Project** và đặt tên cho dự án (Ví dụ: `iot-bus-attendance`).
3. Bạn có thể tạm thời tắt dịch vụ *Google Analytics* để rút ngắn thời gian khởi tạo dự án, sau đó nhấn **Create Project**.

##### **Bước 2: Khởi tạo và thiết lập Realtime Database**
1. Tại thanh menu bên trái, tìm mục **Build** và chọn **Realtime Database**. Nhấn nút **Create Database**.
2. Chọn vị trí đặt máy chủ (Khuyên dùng khu vực Singapore - `asia-southeast1` để tối ưu hóa tốc độ truyền tải về Việt Nam).
3. Chọn chế độ bảo mật ban đầu là **Start in test mode** (Chế độ thử nghiệm) để cho phép đọc/ghi trực tiếp mà không cần cài đặt phân quyền phức tạp trong quá trình chạy thử nghiệm, sau đó nhấn **Enable**.
4. Vào tab **Rules** (Quy tắc), đảm bảo quy tắc bảo mật được thiết lập cho phép đọc/ghi tự do như sau để ESP32 có thể đẩy dữ liệu lên:
   ```json
   {
     "rules": {
       ".read": true,
       ".write": true
     }
   }
   ```
   *Lưu ý: Sau khi hoàn thành kiểm thử đồ án và đưa vào vận hành thực tế, cần cập nhật Rules để bảo mật dữ liệu học sinh.*

##### **Bước 3: Nạp dữ liệu cấu hình học sinh ban đầu (Import JSON)**
1. Soạn thảo một tệp dữ liệu cấu hình danh sách học sinh có định dạng JSON mẫu tương tự như cấu trúc dưới đây (đặt tên file là `data1.json`):
   ```json
   {
     "users": {
       "AA953207": {
         "name": "Nguyen_Van_A",
         "chat_id": "87794657",
         "phone": "+84704160313",
         "status": "out"
       },
       "9A22F906": {
         "name": "Nguyen_Van_B",
         "chat_id": "12345678",
         "phone": "+84366913204",
         "status": "out"
       }
     }
   }
   ```
   *Trong đó: Key cấp 1 là `users`, Key cấp 2 (`AA953207`, `9A22F906`...) là mã UID của thẻ RFID học sinh tương ứng. Các trường thông tin bao gồm tên học sinh (`name`), Telegram ID nhận thông báo (`chat_id`), số điện thoại dự phòng (`phone`), và trạng thái hiện tại (`status` nhận giá trị `"in"` hoặc `"out"`).*
2. Tại giao diện **Realtime Database**, nhấp vào biểu tượng 3 dấu chấm đứng góc trên bên phải màn hình hiển thị dữ liệu và chọn **Import JSON**.
3. Tải lên tệp `data1.json` đã chuẩn bị. Cơ sở dữ liệu sẽ tự động chuyển thành dạng cây thư mục như hình ảnh quản lý học sinh trên Cloud.

##### **Bước 4: Trích xuất thông tin cấu hình nạp vào mã nguồn ESP32**
Quản trị viên sao chép 2 thông tin quan trọng sau trên Firebase để thay thế vào code Arduino (`datn.ino`):
1. **Firebase Host URL:** Sao chép chuỗi URL hiển thị ngay phía trên bảng dữ liệu (Ví dụ: `datn-kzexal-default-rtdb.asia-southeast1.firebasedatabase.app` - lưu ý bỏ `https://` và dấu `/` ở cuối).
2. **Firebase Auth Secret:** Vào **Project Settings** (Biểu tượng bánh răng) -> **Service accounts** -> **Database secrets** để copy khóa bảo mật dùng làm mật khẩu xác thực kết nối từ ESP32.

---

**J. Hướng dẫn khởi tạo Telegram Bot và lấy API Token**

Hệ thống IoT gửi các thông báo thông qua nền tảng tin nhắn Telegram. Quản trị viên cần khởi tạo một Bot Telegram riêng để nhận API Token kết nối. Các bước thực hiện như sau:

##### **Bước 1: Tìm kiếm và khởi động BotFather**
1. Mở ứng dụng Telegram trên thiết bị điện thoại hoặc máy tính.
2. Tại thanh tìm kiếm, nhập từ khóa **`@BotFather`** (đây là tài khoản chính thức có tích xanh của Telegram hỗ trợ việc tạo và quản lý bot).
3. Nhấp vào tài khoản BotFather và nhấn nút **Start** (hoặc gửi tin nhắn lệnh **`/start`**).

##### **Bước 2: Tạo Bot mới**
1. Nhập và gửi lệnh **`/newbot`** để bắt đầu quy trình tạo bot mới.
2. BotFather sẽ yêu cầu đặt tên cho Bot (đây là tên hiển thị, ví dụ: `Hệ Thống Xe Buýt Trường THPT`). Nhập tên và gửi đi.
3. BotFather tiếp tục yêu cầu đặt username (tên đăng nhập) cho Bot.
   * *Quy định đặt username:* Viết liền không dấu, không chứa ký tự đặc biệt trừ dấu gạch dưới `_` và bắt buộc phải kết thúc bằng từ `bot` hoặc `_bot` (Ví dụ: `iot_bus_attendance_bot` hoặc `BusTHPTBot`).
   * Nếu username đã bị người khác sử dụng, BotFather sẽ báo lỗi, bạn cần đặt lại một username khác.

##### **Bước 3: Lấy API Token và kết nối thiết bị**
1. Khi tạo thành công, BotFather sẽ gửi lại một tin nhắn xác nhận kèm theo một liên kết để truy cập bot (Ví dụ: `t.me/BusTHPTBot`) và một chuỗi API Token (Ví dụ: `8456520629:AAFm9tTdNCjeenby2lVW_gg0mKjxD_5-3vE`).
2. Tiến hành sao chép (copy) chuỗi API Token này và nạp vào biến `BOT_TOKEN` trong chương trình nạp cho vi điều khiển ESP32 (`datn.ino`).
3. **Kích hoạt hoạt động:** Quản trị viên nhấp vào liên kết của Bot vừa tạo (ví dụ: `t.me/BusTHPTBot`) và nhấn nút **Start** để mở luồng hội thoại. Bước này là bắt buộc để Bot có quyền gửi tin nhắn cho hệ thống sau này.



