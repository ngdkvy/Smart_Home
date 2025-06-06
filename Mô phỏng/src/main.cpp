#include <WiFi.h>                // Thư viện WiFi cho ESP32
#include <FirebaseESP32.h>       // Thư viện Firebase cho ESP32
#include <DHT.h>

const char* ssid = "Wokwi-GUEST";
const char* password = "";

#define FIREBASE_HOST "iot-lt-2cecc-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "k9jcxvvKO4inaBCp9V34P0mWesTSO9r8htijyMg0"

// Cấu hình chân
#define DHTPIN 5          // Chân DHT22 DATA
#define DHTTYPE DHT22
#define TRIG_PIN 16
#define ECHO_PIN 35
#define BUZZER_PIN 27
#define LED_RED 4
#define LED_YELLOW 2
#define mq2Pin 34

DHT dht(DHTPIN, DHTTYPE);

FirebaseData firebaseData;
FirebaseConfig config;
FirebaseAuth auth;

unsigned long lastSendTime = 0;
const unsigned long sendInterval = 20000;  // 20 giây

void setup() {
  Serial.begin(115200);
  dht.begin();

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(mq2Pin, INPUT);
  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_YELLOW, LOW);
  Serial.println("Đang kết nối WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  Serial.println("\nWiFi đã kết nối!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
  config.host = FIREBASE_HOST;
  config.signer.tokens.legacy_token = FIREBASE_AUTH;

  // Bắt đầu kết nối Firebase
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
  Serial.println("Firebase đã sẵn sàng!");
}

float readDistanceCM() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  long duration = pulseIn(ECHO_PIN, HIGH);
  float distance = duration * 0.034 / 2;
  return distance;
}
void sendDataToFirebase(float temperature, float humidity, float distance, int gasLevel) {
  // Gửi nhiệt độ
  Firebase.setFloat(firebaseData, "/sensor/temperature", temperature);
  Firebase.setFloat(firebaseData, "/sensor/humidity", humidity);
  Firebase.setFloat(firebaseData, "/sensor/distance", distance);
  Firebase.setFloat(firebaseData, "/sensor/gas", gasLevel);
  Serial.println("\nGửi thông tin hoàn tất!");
  analogReadResolution(10); 

}
void loop() {
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  float distance = readDistanceCM();
  int gasLevel = analogRead(mq2Pin);

  sendDataToFirebase(temperature, humidity, distance, gasLevel);
  Serial.printf("Temp: %.1f°C | Hum: %.1f%% | Dist: %.1f cm | Gas: %d", temperature, humidity, distance, gasLevel);

  // Điều kiện bật LED/buzzer nếu quá ngưỡng
    if ((temperature > 40) && (humidity < 40)) 
  {
    digitalWrite(LED_RED, HIGH);
    Firebase.setFloat(firebaseData, "/thietbicanhbao/led_red", 1);
  } 
  else 
  {
    digitalWrite(LED_RED, LOW);
    Firebase.setFloat(firebaseData, "/thietbicanhbao/led_red", 0);
  }

  if (distance < 40) {
    digitalWrite(LED_YELLOW, HIGH);
    digitalWrite(BUZZER_PIN, HIGH);
    Firebase.setFloat(firebaseData, "/thietbicanhbao/led_yellow", 1);
    Firebase.setFloat(firebaseData, "/thietbicanhbao/buzzer", 1);
  } else {
    digitalWrite(LED_YELLOW, LOW);
    digitalWrite(BUZZER_PIN, LOW);
    Firebase.setFloat(firebaseData, "/thietbicanhbao/led_yellow", 0);
    Firebase.setFloat(firebaseData, "/thietbicanhbao/buzzer", 0);
  }
  
  delay(1000);
}
