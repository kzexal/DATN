---
title: "Báo Cáo Thực Tập Kỹ Thuật"
author: "Bùi Duy Khánh"
date: "Tháng 6/2026"
---

# ĐẠI HỌC BÁCH KHOA HÀ NỘI
## TRƯỜNG ĐIỆN - ĐIỆN TỬ

<br><br>

# BÁO CÁO THỰC TẬP KỸ THUẬT

**Đơn vị thực tập: Công Ty TNHH Điện Tử Meiko Việt Nam**

<br>

**Sinh viên thực hiện:** Bùi Duy Khánh  
**MSSV:** 20224017  
**Lớp:** Điện tử 03 - K67  
**Giảng viên hướng dẫn:** PGS TS. Nguyễn Hữu Phát  
**Người trực tiếp hướng dẫn tại doanh nghiệp:** Nguyễn Quang Sang  

<br>
<p align="center">Hà Nội, 6/2026</p>

---

## LỜI CẢM ƠN

Thực tập doanh nghiệp là một phần quan trọng trong chương trình đào tạo, giúp sinh viên có cơ hội tiếp cận môi trường làm việc thực tế, vận dụng những kiến thức đã được học vào công việc chuyên môn, đồng thời rèn luyện các kỹ năng nghề nghiệp cần thiết. Đây cũng là cơ hội để sinh viên làm quen với quy trình làm việc chuyên nghiệp, tác phong công nghiệp và văn hóa doanh nghiệp.

Em xin gửi lời cảm ơn chân thành tới Ban lãnh đạo cùng toàn thể cán bộ, nhân viên Công ty TNHH Điện tử Meiko Việt Nam (đặc biệt là các anh/chị trong phòng E-EN) và thầy Nguyễn Hữu Phát, anh Nguyễn Quang Sang đã tạo điều kiện thuận lợi, hướng dẫn và hỗ trợ em trong suốt thời gian thực tập. Đồng thời, em xin chân thành cảm ơn quý thầy cô trong Khoa Điện tử viễn thông, Trường Điện - Điện Tử đã trang bị cho em những kiến thức nền tảng và tạo điều kiện để em hoàn thành đợt thực tập này.

---

## TÓM TẮT

Trong thời gian thực tập từ ngày 30/3/2026 đến 30/5/2026 tại Công ty TNHH Điện tử Meiko Việt Nam, em đã được phân công thực tập tại phòng E-EN, bộ phận sau SMT. Tại đây, em có cơ hội tìm hiểu về cơ cấu tổ chức, quy trình sản xuất và kiểm thử các thiết bị điện tử. Đặc biệt, em được trực tiếp tham gia vào công tác test, bảo trì các dây chuyền máy RF và OTA, tìm hiểu các thiết bị kiểm tra quang học như AOI, SPI, cũng như chạy thử nghiệm và đánh giá độ ổn định của dây chuyền FCT mới. 

Báo cáo này được thực hiện nhằm tổng kết những kiến thức, kỹ năng và kinh nghiệm mà em đã tiếp thu trong thời gian thực tập. Nội dung báo cáo bao gồm phần giới thiệu về đơn vị thực tập, công việc đã thực hiện, kết quả đạt được cũng như những nhận xét, đánh giá và bài học kinh nghiệm rút ra trong quá trình thực tập.

---

## MỤC LỤC
1. LỜI CẢM ƠN
2. TÓM TẮT
3. CHƯƠNG 1: GIỚI THIỆU SƠ LƯỢC VỀ CÔNG TY
4. CHƯƠNG 2: TỔNG QUAN VỀ CÔNG NGHỆ KIỂM TRA SAU SMT
5. CHƯƠNG 3: QUÁ TRÌNH THỰC TẬP
6. CHƯƠNG 4: KẾT QUẢ THỰC TẬP
7. TÀI LIỆU THAM KHẢO

---

## CHƯƠNG 1: GIỚI THIỆU SƠ LƯỢC VỀ CÔNG TY

Từ một công ty chuyên sản xuất bản mạch in điện tử (Printed Circuit Board – PCB) được thành lập năm 1975 tại tỉnh Kanagawa, Nhật Bản, sau hơn 30 năm phát triển, Tập đoàn Điện tử Meiko đã trở thành một trong những tập đoàn đứng đầu thế giới về sản xuất bản mạch in điện tử và lắp ráp linh kiện điện tử. Hiện nay tập đoàn có 4 nhà máy tại Nhật Bản, 2 nhà máy tại Trung Quốc, 1 nhà máy tại Việt Nam (Hà Nội) và nhiều trung tâm nghiên cứu, văn phòng đại diện trên toàn cầu.

Ngày 14 tháng 12 năm 2006, Công ty TNHH Điện tử Meiko Việt Nam chính thức được trao giấy chứng nhận đầu tư vào Khu Công nghiệp Thạch Thất – Quốc Oai, huyện Thạch Thất, Hà Nội. Lĩnh vực hoạt động chính gồm: Thiết kế, sản xuất và chế tạo các loại bảng mạch in điện tử (PCB); lắp ráp các linh kiện lên PCB, lắp ráp các sản phẩm điện tử hoàn chỉnh (EMS).

Với tổng diện tích 170,000m2 (17 hec-ta), gồm các nhà máy sản xuất bản mạch in điện tử (PCB), nhà máy lắp ráp linh kiện (EMS), khu kí túc xá cho CBCNV, khi hoàn thành và đi vào hoạt động ổn định, Meiko Việt Nam thu hút hàng ngàn lao động và mang lại doanh thu xuất khẩu lớn.

---

## CHƯƠNG 2: TỔNG QUAN VỀ CÔNG NGHỆ KIỂM TRA SAU SMT

Trong sản xuất bo mạch điện tử, sau khi các linh kiện SMD được gắn lên mạch qua dây chuyền SMT, quá trình kiểm tra (Inspection) và kiểm thử chức năng (Testing) đóng vai trò sống còn để đảm bảo chất lượng sản phẩm trước khi xuất xưởng. Bộ phận sau SMT chịu trách nhiệm phát hiện lỗi lắp ráp, nạp phần mềm và kiểm tra toàn diện các chức năng của bo mạch.

### 2.1 Kiểm tra in kem hàn (SPI - Solder Paste Inspection)
Tuy thường được đặt ngay trong dây chuyền SMT (sau máy in kem hàn), kỹ thuật viên phòng E-EN vẫn cần nắm vững hoạt động của SPI. Máy SPI sử dụng công nghệ quang học 3D để quét và đo lường thể tích, diện tích, chiều cao và độ lệch của lớp kem hàn. Việc kiểm soát tốt ở bước này giúp giảm thiểu đến 70% lỗi hàn ở các công đoạn sau.

### 2.2 Kiểm tra quang học tự động (AOI - Automated Optical Inspection)
Sau lò hàn Reflow, bo mạch sẽ đi qua máy AOI. Sử dụng hệ thống camera độ phân giải cao và thuật toán xử lý ảnh phức tạp, AOI so sánh hình ảnh thực tế của bo mạch với bo mạch chuẩn để phát hiện các lỗi như: thiếu linh kiện, sai linh kiện, lệch linh kiện, ngược cực, và các lỗi về mối hàn (thiếu thiếc, chạm chập, cầu thiếc).

### 2.3 Quá trình Nạp ROM (IC Programming)
Trước khi kiểm tra chức năng, vi điều khiển (MCU) hoặc bộ nhớ flash trên bo mạch cần được nạp chương trình phần mềm (firmware/ROM). Quá trình này có thể được thực hiện thông qua các thiết bị nạp tự động, hoặc nạp trực tiếp trên bo (In-System Programming - ISP). Đảm bảo tính toàn vẹn dữ liệu trong quá trình nạp là yêu cầu bắt buộc để bo mạch có thể hoạt động đúng.

### 2.4 Kiểm tra chức năng (FCT - Functional Circuit Test)
FCT là công đoạn mô phỏng môi trường hoạt động thực tế của bo mạch để kiểm tra xem nó có thực hiện đúng các chức năng thiết kế hay không.
- Các máy FCT kết nối với bo mạch thông qua hệ thống kim đo (test probes) chạm vào các điểm test point trên PCB.
- Hệ thống FCT thực hiện cấp nguồn, đưa tín hiệu đầu vào và đo lường các tín hiệu đầu ra, phân tích log để phát hiện các bất thường.
- Trong các hệ thống giao tiếp, việc đo suy hao đường truyền (Pathloss) là một chỉ tiêu quan trọng để đảm bảo tín hiệu truyền tải không bị suy giảm quá mức cho phép.

### 2.5 Kiểm tra RF (Radio Frequency) và OTA (Over-The-Air)
Đối với các thiết bị có tính năng kết nối không dây (Wi-Fi, Bluetooth, 5G, IoT...), kiểm tra RF và OTA là bước kiểm thử cấp cao:
- **RF Test (Conducted Test):** Kiểm tra tín hiệu cao tần qua cáp dẫn trực tiếp từ antenna port của bo mạch đến máy đo (như Spectrum Analyzer, Network Analyzer) để đo công suất phát, độ nhạy thu, tần số...
- **OTA Test (Radiated Test):** Kiểm tra hiệu suất bức xạ ăng-ten toàn vẹn trong môi trường buồng chống nhiễu (Anechoic Chamber). Kiểm tra OTA mô phỏng điều kiện sóng không gian thực tế để đánh giá công suất bức xạ đẳng hướng tương đương (EIRP) và độ nhạy thu toàn hướng (TIS).

---

## CHƯƠNG 3: QUÁ TRÌNH THỰC TẬP

Trong thời gian 2 tháng thực tập (30/3/2026 - 30/5/2026) tại Công ty TNHH Điện tử Meiko Việt Nam, em được phân công vào bộ phận E-EN, làm việc trực tiếp tại khu vực sau SMT. Dưới sự hướng dẫn của anh Nguyễn Quang Sang, em đã trực tiếp tham gia học hỏi và thực hiện các nhiệm vụ chuyên môn. 

Lịch trình công việc và thời gian cụ thể như sau:

| Thời gian | Nội dung công việc |
| :--- | :--- |
| **Tuần 1-2** | **Đào tạo hội nhập và an toàn lao động**<br> - Học các quy định, nội quy của xưởng sản xuất.<br> - Học các tiêu chuẩn về an toàn, tĩnh điện (ESD) trong công ty và các quy định khi làm việc trong phòng sạch.<br> - Tổng quan về dây chuyền sản xuất và vai trò của phòng E-EN, bộ phận sau SMT. |
| **Tuần 3** | **Tìm hiểu thiết bị kiểm tra quang học**<br> - Học cấu tạo, nguyên lý hoạt động của máy SPI và máy AOI.<br> - Quan sát quá trình xử lý ảnh và cách thiết lập thông số nhận diện lỗi trên máy AOI để giảm thiểu tỷ lệ báo lỗi giả (False Alarm). |
| **Tuần 4** | **Tìm hiểu quy trình Nạp ROM**<br> - Học cách sử dụng máy nạp ROM cho các IC và vi điều khiển.<br> - Tìm hiểu các giao thức nạp dữ liệu và xử lý các lỗi thường gặp khi nạp firmware. |
| **Tuần 5-6** | **Thực hành với hệ thống FCT (Functional Circuit Test)**<br> - Tham gia chạy thử nghiệm, đánh giá độ ổn định của dây chuyền FCT mới.<br> - Học cách đo suy hao (Pathloss) của các máy FCT theo tiêu chuẩn trong hướng dẫn thao tác (SOP).<br> - Thực hành đọc và phân tích log từ máy FCT, qua đó chẩn đoán và xử lý các sự cố phát sinh trong quá trình vận hành dây chuyền. |
| **Tuần 7-8** | **Bảo trì và Test dây chuyền máy RF, OTA**<br> - Tìm hiểu nguyên lý đo lường tín hiệu vô tuyến (RF).<br> - Tham gia test và bảo trì các thiết bị đo kiểm trong dây chuyền RF và OTA.<br> - Hỗ trợ các kỹ sư setup thông số đo, kiểm tra sự ổn định của kết nối cáp cao tần và môi trường buồng OTA.<br> - Tổng kết, viết báo cáo thực tập và xin xác nhận từ công ty. |

---

## CHƯƠNG 4: KẾT QUẢ THỰC TẬP

Đợt thực tập tại Công ty TNHH Điện tử Meiko Việt Nam là một trải nghiệm thực tế quý báu, mang lại ý nghĩa to lớn đối với hành trang kiến thức của em. Thông qua quá trình tìm hiểu và thực hành tại phòng E-EN (bộ phận sau SMT), em đã đạt được những kết quả sau:

1. **Về kiến thức chuyên môn:** 
   - Hiểu rõ quy trình kiểm soát chất lượng sau khi bo mạch rời khỏi dây chuyền lắp ráp (SMT).
   - Nắm được nguyên lý và cách vận hành cơ bản của các thiết bị kiểm tra quang học (SPI, AOI), quy trình nạp ROM.
   - Có cơ hội tiếp xúc thực tế và thực hành chuyên sâu với các hệ thống đo kiểm điện tử phức tạp như FCT, hệ thống đo cao tần RF và buồng đo OTA. Biết cách đo đạc Pathloss và đọc log phân tích lỗi, đây là những kiến thức hiếm khi được thực hành trên ghế nhà trường.

2. **Về kỹ năng mềm và tác phong công nghiệp:**
   - Quen thuộc với môi trường làm việc phòng sạch, ý thức nghiêm ngặt về các tiêu chuẩn an toàn tĩnh điện (ESD) và kỷ luật lao động của một công ty sản xuất điện tử hàng đầu Nhật Bản.
   - Rèn luyện kỹ năng làm việc nhóm, kỹ năng xử lý sự cố thực tế (troubleshooting) trên dây chuyền và báo cáo kỹ thuật.

Mặc dù thời gian thực tập chỉ kéo dài 2 tháng và kiến thức của bản thân còn nhiều hạn chế, nhưng nhờ sự tận tình chỉ bảo của anh Nguyễn Quang Sang và các anh chị kỹ sư trong phòng E-EN, em đã hoàn thành tốt các mục tiêu đề ra ban đầu. 

Một lần nữa, em xin chân thành cảm ơn Ban lãnh đạo công ty, phòng E-EN và Trường Điện - Điện Tử (ĐHBK HN) đã tạo điều kiện tốt nhất cho em trong đợt thực tập này.

---

## TÀI LIỆU THAM KHẢO

[1] “Giới Thiệu”, Meiko Electronics Vietnam. [Online]. Có tại: https://meiko-elec.com.vn/gioi-thieu/  
[2] Tài liệu nội bộ hướng dẫn vận hành máy FCT, AOI, SPI tại phòng E-EN, Công ty TNHH Điện tử Meiko Việt Nam.  
[3] Các tiêu chuẩn đo kiểm RF/OTA cơ bản, Tài liệu đào tạo nội bộ Meiko Việt Nam.
