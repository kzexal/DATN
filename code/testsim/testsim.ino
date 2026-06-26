#define SIM_RX_PIN 27
#define SIM_TX_PIN 26
const String SO_DIEN_THOAI = "+84704160313";
// --- Thêm vào đầu file cùng chỗ với SO_DIEN_THOAI ---
const String TG_TOKEN  = "8456520629:AAFm9tTdNCjeenby2lVW_gg0mKjxD_5-3vE"; // token bot
const String TG_CHATID = "8779465788"; // chat_id của bạn
HardwareSerial SerialSIM(1);

// =====================================================
// HÀM LÕI: Gửi lệnh AT và đọc response đã lọc rác
// =====================================================
String sendAT(String cmd, int waitMs = 2000, bool debug = true) {
  while (SerialSIM.available()) SerialSIM.read();

  SerialSIM.println(cmd);

  String raw = "";
  long start = millis();

  while (millis() - start < waitMs) {
    while (SerialSIM.available()) {
      raw += (char)SerialSIM.read();
    }
    if (raw.indexOf("OK") != -1 || raw.indexOf("ERROR") != -1) {
      delay(50);
      while (SerialSIM.available()) raw += (char)SerialSIM.read();
      break;
    }
  }

  String clean = "";
  for (char c : raw) {
    if ((c >= 32 && c <= 126) || c == '\r' || c == '\n') {
      clean += c;
    }
  }

  if (debug) {
    Serial.print("[");
    Serial.print(cmd);
    Serial.print("] -> ");
    Serial.println(clean);
  }

  return clean;
}

// =====================================================
// HÀM TIỆN ÍCH
// =====================================================
String extractValue(String response, String prefix) {
  int idx = response.indexOf(prefix);
  if (idx == -1) return "";
  idx += prefix.length();
  int end = response.indexOf('\n', idx);
  if (end == -1) end = response.length();
  String val = response.substring(idx, end);
  val.trim();
  return val;
}

// =====================================================
// ĐO SÓNG — parse linh hoạt chống nhiễu
// =====================================================
int getCSQ() {
  String res = sendAT("AT+CSQ", 1000);

  int idx = res.indexOf("+CSQ:");
  if (idx != -1) {
    idx += 5;
    while (idx < res.length() && res[idx] == ' ') idx++;
  } else {
    int cmdEnd = res.indexOf("AT+CSQ");
    if (cmdEnd != -1) cmdEnd += 6;
    else cmdEnd = 0;
    idx = cmdEnd;
    while (idx < res.length() && !isDigit(res[idx])) idx++;
  }

  String num = "";
  while (idx < res.length() && isDigit(res[idx])) {
    num += res[idx++];
  }
  num.trim();

  Serial.print("[DEBUG CSQ raw]: '");
  Serial.print(num);
  Serial.println("'");

  if (num.length() == 0) return 99;
  return num.toInt();
}

// =====================================================
// KIỂM TRA MẠNG
// =====================================================
bool isNetworkReady() {
  String res = sendAT("AT+CEREG?", 1000);
  return (res.indexOf(",1") != -1 || res.indexOf(",5") != -1);
}

// =====================================================
// KIỂM TRA 4G
// =====================================================
String getNetworkType() {
  String res = sendAT("AT+CPSI?", 2000);
  if (res.indexOf("LTE") != -1)   return "4G LTE";
  if (res.indexOf("WCDMA") != -1) return "3G";
  if (res.indexOf("GSM") != -1)   return "2G";
  return "Khong xac dinh";
}

bool activateData(String apn = "v-internet") {
  sendAT("AT+CGDCONT=1,\"IP\",\"" + apn + "\"", 2000);

  String res = sendAT("AT+CGACT=1,1", 10000);
  // Chấp nhận cả "OK" bị nhiễu 1 ký tự
  if (res.indexOf("OK") == -1 && res.length() < 3) {
    Serial.println("[4G] Kich hoat PDP that bai!");
    return false;
  }

  String ipRes = sendAT("AT+CGPADDR=1", 2000);
  
  // Tìm IP trong response — tìm pattern "1," rồi đọc IP
  int idx = ipRes.indexOf("1,");
  if (idx == -1) {
    Serial.println("[4G] Khong doc duoc IP!");
    return false;
  }
  String ip = ipRes.substring(idx + 2);
  ip.trim();
  // Cắt tại CR/LF nếu có
  int cut = ip.indexOf('\r');
  if (cut != -1) ip = ip.substring(0, cut);
  cut = ip.indexOf('\n');
  if (cut != -1) ip = ip.substring(0, cut);

  Serial.print("[4G] IP duoc cap: ");
  Serial.println(ip);

  return ip.length() > 0 && ip != "0.0.0.0";
}

bool pingTest() {
  // Xả buffer
  while (SerialSIM.available()) SerialSIM.read();

  // Gửi HTTP request
  sendAT("AT+HTTPINIT", 2000);
  sendAT("AT+HTTPPARA=\"URL\",\"http://httpbin.org/ip\"", 2000);
  SerialSIM.println("AT+HTTPACTION=0");

  // Chờ URC "+HTTPACTION:" tối đa 20s
  // Module gửi URC này khi nhận xong response
  String result = "";
  long start = millis();
  while (millis() - start < 20000) {
    while (SerialSIM.available()) {
      result += (char)SerialSIM.read();
    }
    if (result.indexOf("+HTTPACTION:") != -1) break;
  }

  sendAT("AT+HTTPTERM", 1000);

  Serial.print("[4G] HTTP result: ");
  Serial.println(result);

  // +HTTPACTION: 0,200,xxx = HTTP 200 OK
  if (result.indexOf("+HTTPACTION:") != -1) {
    // Lấy status code
    int idx = result.indexOf("+HTTPACTION:");
    String after = result.substring(idx + 12); // bỏ "+HTTPACTION:"
    // Format: method,status,size → tìm status ở giữa
    int c1 = after.indexOf(',');
    int c2 = after.indexOf(',', c1 + 1);
    if (c1 != -1 && c2 != -1) {
      int status = after.substring(c1 + 1, c2).toInt();
      Serial.print("[4G] HTTP status: ");
      Serial.println(status);
      return status >= 200 && status < 400;
    }
    return true; // Có +HTTPACTION là đủ
  }

  return false;
}
// =====================================================
// GỬI TELEGRAM
// =====================================================
bool sendTelegram(String botToken, String chatId, String message) {
  Serial.println("[TG] Bat dau gui Telegram...");

  // Bước 1: Khởi tạo HTTP
  String r = sendAT("AT+HTTPINIT", 3000);
  if (r.indexOf("OK") == -1 && r.length() > 3) {
    // Có thể đang còn session cũ — term rồi init lại
    sendAT("AT+HTTPTERM", 2000);
    delay(500);
    sendAT("AT+HTTPINIT", 3000);
  }

  // Bước 2: Set URL
  String url = "https://api.telegram.org/bot" + botToken 
             + "/sendMessage?chat_id=" + chatId 
             + "&text=" + urlEncode(message);
  sendAT("AT+HTTPPARA=\"URL\",\"" + url + "\"", 3000);

  // Bước 3: Bật SSL (Telegram bắt buộc HTTPS)
  sendAT("AT+HTTPSSL=1", 2000);

  // Bước 4: Gửi GET request
  SerialSIM.println("AT+HTTPACTION=0");

  // Bước 5: Chờ URC +HTTPACTION (tối đa 30s)
  String result = "";
  long start = millis();
  while (millis() - start < 30000) {
    while (SerialSIM.available()) {
      result += (char)SerialSIM.read();
    }
    if (result.indexOf("+HTTPACTION:") != -1) break;
  }

  sendAT("AT+HTTPTERM", 2000);

  Serial.print("[TG] Raw result: ");
  Serial.println(result);

  // Kiểm tra HTTP 200
  if (result.indexOf("+HTTPACTION:") != -1) {
    int idx = result.indexOf("+HTTPACTION:");
    String after = result.substring(idx + 12);
    int c1 = after.indexOf(',');
    int c2 = after.indexOf(',', c1 + 1);
    if (c1 != -1 && c2 != -1) {
      int status = after.substring(c1 + 1, c2).toInt();
      Serial.print("[TG] HTTP status: ");
      Serial.println(status);
      if (status == 200) {
        Serial.println("[TG] Gui thanh cong!");
        return true;
      }
    }
  }

  Serial.println("[TG] Gui that bai!");
  return false;
}

// URL encode ký tự đặc biệt (space, dấu tiếng Việt sẽ bị lỗi nếu không encode)
String urlEncode(String str) {
  String encoded = "";
  for (int i = 0; i < str.length(); i++) {
    char c = str[i];
    if (c == ' ')       encoded += "%20";
    else if (c == '!')  encoded += "%21";
    else if (c == '"')  encoded += "%22";
    else if (c == '#')  encoded += "%23";
    else if (c == '&')  encoded += "%26";
    else if (c == '\'') encoded += "%27";
    else if (c == '(')  encoded += "%28";
    else if (c == ')')  encoded += "%29";
    else if (c == '+')  encoded += "%2B";
    else if (c == ',')  encoded += "%2C";
    else if (c == '/')  encoded += "%2F";
    else if (c == ':')  encoded += "%3A";
    else if (c == ';')  encoded += "%3B";
    else if (c == '=')  encoded += "%3D";
    else if (c == '?')  encoded += "%3F";
    else if (c == '@')  encoded += "%40";
    else                encoded += c;
  }
  return encoded;
}
void check4G(String apn = "v-internet") {
  Serial.println("\n=== KIEM TRA KET NOI 4G ===");

  String netType = getNetworkType();
  Serial.print("Loai mang: ");
  Serial.println(netType);
  if (netType != "4G LTE") {
    Serial.println("[CANH BAO] Khong phai 4G — toc do se cham hon!");
  }

  Serial.println("Dang kich hoat data...");
  if (!activateData(apn)) {
    Serial.println("[LOI] Khong kich hoat duoc data. Kiem tra APN!");
    return;
  }
  Serial.println("Da kich hoat data!");

  Serial.println("Dang kiem tra internet qua HTTP...");
  if (pingTest()) {
    Serial.println("[OK] Co internet! Ket noi 4G thanh cong.");
  } else {
    Serial.println("[LOI] Khong ra duoc internet.");
  }
}

// =====================================================
// GỬI SMS
// =====================================================
bool sendSMS(String number, String text) {
  String r1 = sendAT("AT+CMGF=1", 1000);
  if (r1.indexOf("OK") == -1) {
    Serial.println("[SMS] Loi set CMGF");
    return false;
  }

  SerialSIM.println("AT+CMGS=\"" + number + "\"");
  delay(1000);

  String prompt = "";
  long start = millis();
  while (millis() - start < 2000) {
    while (SerialSIM.available()) prompt += (char)SerialSIM.read();
    if (prompt.indexOf('>') != -1) break;
  }

  if (prompt.indexOf('>') == -1) {
    Serial.println("[SMS] Khong nhan duoc dau nhac >");
    SerialSIM.write(27);
    return false;
  }

  SerialSIM.print(text);
  delay(100);
  SerialSIM.write(26);

  String result = "";
  start = millis();
  while (millis() - start < 20000) {
    while (SerialSIM.available()) result += (char)SerialSIM.read();
    if (result.indexOf("+CMGS:") != -1 || result.indexOf("ERROR") != -1) break;
  }

  bool ok = result.indexOf("+CMGS:") != -1;
  Serial.println(ok ? "[SMS] Gui thanh cong!" : "[SMS] Gui that bai!");
  return ok;
}

// =====================================================
// GỌI ĐIỆN
// =====================================================
bool makeCall(String number) {
  String res = sendAT("ATD" + number + ";", 5000);
  return res.indexOf("OK") != -1;
}

void hangUp() {
  sendAT("ATH", 1000);
}

// =====================================================
// SETUP
// =====================================================
void setup() {
  Serial.begin(115200);
  SerialSIM.begin(9600, SERIAL_8N1, SIM_RX_PIN, SIM_TX_PIN);
  delay(3000);

  Serial.println("=== KHOI DONG ===");

  sendAT("ATE0", 500, false);

 // Kiểm tra kết nối — retry mỗi 5s nếu fail
  Serial.println("Dang ket noi module SIM...");
  while (true) {
    String at = sendAT("AT", 1000);
    if (at.indexOf("OK") != -1) {
      Serial.println("Ket noi thanh cong!");
      break;
    }
    Serial.println("[LOI] Khong ket noi duoc module SIM! Thu lai sau 5s...");
    delay(5000);
  }

  Serial.println("Cho dang ky mang...");
  for (int i = 0; i < 60; i++) {
    if (isNetworkReady()) {
      Serial.println("Da co mang!");
      break;
    }
    delay(1000);
  }

  int csq = getCSQ();
  Serial.print("CSQ: ");
  Serial.print(csq);
  if      (csq == 99) Serial.println(" (mat song)");
  else if (csq >= 20) Serial.println(" (rat tot)");
  else if (csq >= 15) Serial.println(" (tot)");
  else if (csq >= 10) Serial.println(" (trung binh)");
  else                Serial.println(" (yeu)");

  // Đổi APN theo nhà mạng: Viettel="v-internet" | Vinaphone="m3-world" | Mobifone="m-wap"
  check4G("v-internet");

  Serial.println("\nLenh: S=SMS | C=Goi | E=Ngat may | 4=Kiem tra 4G | T=Telegram");
}

// =====================================================
// LOOP
// =====================================================
void loop() {
  if (Serial.available()) {
    char lenh = Serial.read();
    if (lenh == '\n' || lenh == '\r') return;

    if (lenh == 'S' || lenh == 's') {
      sendSMS(SO_DIEN_THOAI, "Test SMS tu ESP32!");
    }
    else if (lenh == 'C' || lenh == 'c') {
      bool ok = makeCall(SO_DIEN_THOAI);
      Serial.println(ok ? "Dang goi..." : "Goi that bai!");
    }
    else if (lenh == 'E' || lenh == 'e') {
      hangUp();
      Serial.println("Da ngat may.");
    }
    else if (lenh == '4') {
      check4G("v-internet");
    }
    // --- Thêm vào loop() ---
else if (lenh == 'T' || lenh == 't') {
  sendTelegram(TG_TOKEN, TG_CHATID, "Hello from ESP32!");
}
  }

  while (SerialSIM.available()) {
    String urc = SerialSIM.readStringUntil('\n');
    urc.trim();
    if (urc.length() > 0) {
      Serial.print("[URC] ");
      Serial.println(urc);
    }
  }
}