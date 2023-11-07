#include <DHT.h>

#define DHTPIN 4 // Chọn chân số 2 cho kết nối với DHT11
#define DHTTYPE DHT11 // Loại cảm biến DHT11
#define LEDPIN 3 // Chọn chân số 13 cho kết nối với đèn LED

DHT dht(DHTPIN, DHTTYPE);
bool temperatureAlert = false;
bool hotAlert = false;
const int buzzer = 2;


void setup() {
  Serial.begin(9600);
  dht.begin();
  pinMode(LEDPIN, OUTPUT); // Đặt chân LEDPIN làm OUTPUT
  pinMode(buzzer, OUTPUT);

}

void loop() {
  // Đo nhiệt độ
  float temperature = dht.readTemperature();

  // Kiểm tra xem đọc dữ liệu thành công hay không
  if (isnan(temperature)) {
    Serial.println("Lỗi đọc nhiệt độ từ cảm biến DHT11");
  } else {
    Serial.print("Nhiệt độ: ");
    Serial.print(temperature);
    Serial.println(" °C");

    // Kiểm tra nhiệt độ và bật đèn LED nếu nhiệt độ cao hơn 30°C
    if (temperature > 25.0) {
      temperatureAlert = true;
      hotAlert = true; // Đặt cờ báo nhiệt độ
      digitalWrite(LEDPIN, HIGH); // Bật đèn LED
      tone(buzzer, 1000, 500); // Send 1KHz sound signal...
      //delay(1000);  
    }

    // if (hotAlert) {
    //   delay(500);
    //   tone(buzzer, 0);
    //   hotAlert = false; // Đặt lại cờ báo nhiệt độ
    // }
    // Tắt đèn LED sau 5 giây nếu có cảnh báo nhiệt độ
    if (temperatureAlert) {
      delay(5000);
      digitalWrite(LEDPIN, LOW); // Tắt đèn LED
      temperatureAlert = false; // Đặt lại cờ báo nhiệt độ
    }
  }

  // Chờ  trước khi đo lại
  delay(30000);
}
