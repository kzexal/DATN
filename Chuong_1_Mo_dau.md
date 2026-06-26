# CHƯƠNG 1. TỔNG QUAN ĐỀ TÀI

Chương 1 sẽ trình bày một cách tổng quan đề tài về: đặt vấn đề, đối tượng khách hàng, phương pháp thực hiện, đưa ra mục tiêu của đề tài cũng như các tiêu chí đánh giá hệ thống v.v.

## 1.1 Đặt vấn đề

Trong bối cảnh đô thị hóa và nhịp sống hiện đại, dịch vụ xe buýt đưa đón học sinh đang trở thành một phương tiện thiết yếu tại các trường học ở Việt Nam. Dịch vụ này không chỉ giúp giảm tải áp lực giao thông vào giờ cao điểm mà còn mang lại sự thuận tiện lớn cho các bậc phụ huynh bận rộn. Tuy nhiên, đi kèm với sự phát triển và tiện lợi đó là những mối lo ngại sâu sắc về sự an toàn của học sinh trong quá trình di chuyển. 

Sự phổ biến của xe đưa đón học sinh cũng dẫn tới nhiều vấn đề nghiêm trọng, trong đó có 2 vấn đề liên quan trực tiếp tới sự an toàn của trẻ và tâm lý của phụ huynh:
- Vấn đề 1: bị bỏ quên trên xe buýt.
- Vấn đề 2: thiếu thông tin kiểm soát về thời gian, vị trí học sinh lên và xuống xe.

Nạn bỏ quên trẻ em trên xe đưa đón đã và đang diễn ra, để lại những hậu quả thương tâm và sự bức xúc lớn trong dư luận. Tình trạng này diễn ra một phần do sự chủ quan của nhân viên phụ trách, một phần do các phương pháp điểm danh thủ công (gọi tên, ghi sổ) dễ xảy ra sai sót khi số lượng học sinh đông. Bên cạnh đó, phụ huynh thường xuyên rơi vào tình trạng lo âu, không an tâm khi không biết chính xác con em mình đã lên xe an toàn hay chưa, và hiện tại xe đang di chuyển ở khu vực nào.

Vấn đề 1 không chỉ gây nguy hiểm trực tiếp đến tính mạng và sức khỏe của học sinh mà còn gây ảnh hưởng nghiêm trọng tới uy tín của nhà trường. Trong khi đó, vấn đề 2 khiến công tác quản lý của nhà trường trở nên bị động, rất cần có sự ứng dụng công nghệ để tự động hóa quy trình giám sát, giảm thiểu tối đa rủi ro do yếu tố con người gây ra.

Với vấn đề điểm danh và giám sát, nhà trường có thể bảo vệ học sinh bằng cách tăng cường nhân sự kiểm tra chéo. Tuy nhiên, để tự động hóa việc phát hiện và cảnh báo vị trí tức thời, trên thị trường Việt Nam hiện nay các thiết bị đáp ứng đồng thời cả việc điểm danh thẻ và định vị cảnh báo qua mạng 4G/Telegram vẫn còn hạn chế và chi phí triển khai cao.

Trong bối cảnh trên, đề tài **“Thiết kế thiết bị giám sát học sinh lên xuống xe buýt đưa đón”** ra đời nhằm giải quyết đồng thời 2 vấn đề trên trên cùng một thiết bị.

## 1.2 Đối tượng khách hàng

1. Nhà trường và các đơn vị cung cấp dịch vụ đưa đón học sinh
Đây là nhóm đối tượng chính mà hệ thống hướng đến. Hệ thống giúp nhà trường quản lý chặt chẽ số lượng học sinh lên/xuống xe, tự động hóa quy trình điểm danh, đảm bảo an toàn tuyệt đối cho học sinh và nâng cao chất lượng dịch vụ giáo dục.

2. Phụ huynh học sinh
Hệ thống cung cấp cho phụ huynh sự an tâm tuyệt đối bằng cách tự động gửi thông báo (kèm vị trí GPS) ngay khi con em họ lên hoặc xuống xe, giúp phụ huynh nắm bắt thông tin kịp thời.

3. Các cơ quan nghiên cứu và đào tạo
Các tổ chức nghiên cứu và đào tạo có thể quan tâm đến đề tài này để phát triển công nghệ IoT trong giao thông thông minh và quản lý trường học; đào tạo nhân lực có chuyên môn về lĩnh vực này.

## 1.3 Phương pháp thực hiện

1. Nghiên cứu và phân tích
Tiến hành nghiên cứu về tình hình mất an toàn trên xe đưa đón học sinh hiện tại, xác định các yếu tố nguyên nhân và tầm ảnh hưởng của chúng. Đánh giá các công nghệ IoT (RFID, GPS, 4G LTE) và các thiết bị cảm biến có thể được áp dụng để giải quyết vấn đề này.

2. Thiết kế hệ thống
Xác định các yêu cầu chức năng và phi chức năng của hệ thống IoT. Thiết kế kiến trúc hệ thống bao gồm thiết bị nhận dạng, mạng truyền thông, cơ sở dữ liệu và giao diện cảnh báo tới người dùng. Xác định các phương pháp xử lý dữ liệu để tự động điểm danh và định vị vị trí xe.

3. Xây dựng phân hệ phần cứng
Dựa trên yêu cầu của hệ thống, lựa chọn các thiết bị cảm biến (RFID RC522, GPS ATGM336H) và vi điều khiển có tích hợp kết nối mạng phù hợp. Thiết kế thiết bị nhỏ gọn có thể gắn vào xe buýt với khả năng tính toán, đưa cảnh báo và trao đổi dữ liệu với hệ thống phần mềm.

4. Xây dựng phân hệ phần mềm
Phát triển phần mềm nhúng (Firmware) phân tích dữ liệu và thiết lập giao tiếp với người dùng (thông qua Telegram Bot). Xác định các cơ chế cảnh báo và cách thức gửi thông báo đến phụ huynh hoặc cơ quan chức năng khi có học sinh quẹt thẻ hoặc khi có tình huống khẩn cấp.

5. Kiểm thử và đánh giá hiệu quả
Triển khai hệ thống vào môi trường thực tế và tiến hành thử nghiệm hoạt động của thiết bị trên xe di chuyển. Đánh giá hiệu quả của hệ thống trong việc nhận diện thẻ, tốc độ gửi tin nhắn và độ chính xác của GPS. Đo lường khả năng hoạt động ổn định và thời gian phản ứng của hệ thống.

6. Đưa ra hướng tối ưu, cải tiến
Dựa trên kết quả đánh giá, tiến hành đưa ra hướng tối ưu, cải tiến hệ thống trong tương lai để nâng cao hiệu quả và độ tin cậy.

## 1.4 Mục tiêu của đề tài

Mục tiêu chính của đề tài là xây dựng một hệ thống IoT thông minh, nhằm phát hiện và cảnh báo kịp thời hai tình huống:
- Học sinh lên/xuống xe: thiết bị tự động nhận diện danh tính học sinh qua thẻ RFID, lưu trữ trạng thái và gửi thông báo trực tiếp tới điện thoại phụ huynh.
- Giám sát hành trình: hệ thống liên tục định vị vị trí của xe buýt đang ở đâu (tọa độ GPS), đính kèm vào thông báo để phụ huynh và nhà trường có thể theo dõi trực tuyến.

## 1.5 Các tiêu chí đánh giá hệ thống

1. Tính chính xác
Mức độ chính xác của hệ thống trong việc nhận diện đúng thẻ RFID của học sinh và lấy tọa độ GPS thực tế. Hệ thống cần đảm bảo rằng cảnh báo và thông tin gửi đi là chuẩn xác, tránh việc nhận diện sai thẻ gây nhầm lẫn thông tin cho phụ huynh.

2. Tính khả dụng
Thời gian hoạt động của hệ thống trong một khoảng thời gian nhất định. Tính khả dụng cao đảm bảo hệ thống luôn sẵn sàng hoạt động trong suốt các chuyến đi và cung cấp thông báo kịp thời ngay khi học sinh lên/xuống xe.

3. Tính tin cậy, bảo mật
Đảm bảo rằng dữ liệu điểm danh và thông tin cá nhân của học sinh được lưu trữ một cách an toàn, tin nhắn thông báo chỉ được gửi đến đúng số điện thoại/tài khoản Telegram của phụ huynh, tránh rò rỉ thông tin.

4. Hiệu suất
Tốc độ phản hồi và xử lý dữ liệu của hệ thống. Hệ thống cần có hiệu suất cao, thời gian từ lúc quẹt thẻ đến lúc phụ huynh nhận được tin nhắn qua mạng 4G phải được tính bằng giây.

5. Tính linh hoạt
Khả năng thích nghi và mở rộng của hệ thống để có thể tích hợp số lượng lớn thẻ RFID và lắp đặt trên nhiều loại phương tiện xe đưa đón khác nhau.

6. Dễ dàng sử dụng
Hệ thống cần thiết kế phần cứng đơn giản (cắm điện là chạy), giao diện nhận thông báo (Telegram/SMS) quen thuộc, rõ ràng và dễ dàng thao tác cho phụ huynh mà không cần cài đặt ứng dụng phức tạp.

7. Chi phí
Đánh giá chi phí triển khai và vận hành hệ thống. Đảm bảo rằng hệ thống đáp ứng hiệu quả về chi phí và có tính khả thi cao để các trường học có thể trang bị đồng loạt.

8. Hiệu quả
Đánh giá khả năng của hệ thống trong việc giảm thiểu triệt để tình trạng bỏ quên học sinh; giúp đỡ kịp thời trong khâu quản lý. Hiệu quả của hệ thống là mục tiêu chính để nâng cao an toàn học đường.

## 1.6 Kết luận chương

Như vậy, nội dung chương đã cung cấp cái nhìn tổng quan, hướng phát triển của đề tài và hệ thống. Các chương tiếp theo sẽ tiếp tục đào sâu vào cách thiết kế và triển khai từng phân hệ cần có của đề tài.
