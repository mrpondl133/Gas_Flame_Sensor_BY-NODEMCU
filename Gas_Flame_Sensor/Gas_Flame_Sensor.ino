#include <LiquidCrystal_I2C.h>
#include <TridentTD_LineNotify.h>
#include <Wire.h>


  int FiresensorPin = D0; //ตั้งขา D0 เป็นขารับข้อมูลเซ็นเซ็อร์ตรวจจับประกายไฟ
  int SensorState = 0; //สถานะของเซ็นเซอร์ตรวจจับประกายไฟ
  int BUZZER_PIN = D7; //ตั้งขา D3 เป็นขา BUZZER
  int GAS_SENSOR = A0; //ตั้งขา A0 เป็นขารับข้อมูลเซ็นเซ็อร์ตรวจจับแก๊ส
  int sensorThres = 400;  //ตั้งค่าความหนาของควันแก๊สที่ 400 (ทดสอบ)
  LiquidCrystal_I2C lcd(0x27, 16, 2);
  
#define SSID        "NB_2.4GHz"    //Name Wifi สำหรับ ESP8266 
#define PASSWORD    "085225698"    //Pass Wifi สำหรับ ESP8266
#define LINE_TOKEN  "TIxEmdvJFivGJwinfoqirgr0EX53wMf21GhEU3NVOv3"    //LINE TOKEN สำหรับแจ้งเตือนผ่านไลน์


void setup() {
  Serial.begin(115200); Serial.println(); // ตั้งค่ามอนิเตอร์ของ ARDUINO
  pinMode(FiresensorPin, INPUT); //ตั้งค่า SensorPin เป็น INPUT
  pinMode(BUZZER_PIN, OUTPUT); //ตั้งค่า BUZZER_PIN เป็น OUTPUT
  pinMode(GAS_SENSOR, INPUT); //ตั้งค่า GAS_SENSOR เป็น INPUT
  Serial.println(LINE.getVersion()); 
  lcd.begin();
  lcd.backlight();

//ส่วนของการเชื่อมต่อ WIFI
  WiFi.begin(SSID, PASSWORD);
  Serial.printf("WiFi connecting to %s\n",  SSID);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".\t");
    delay(400);
  }
  Serial.printf("\nWiFi connected\nIP : ");
  Serial.println(WiFi.localIP());


  // กำหนด Line Token
  LINE.setToken(LINE_TOKEN);

  // ตัวอย่างส่งข้อความ
  LINE.notify("เชื่อมต่อ สำเร็จ");
}

void loop() {
  //ส่วนของเซ็นเซอร์ตรวจสอบแก๊ส
  int analogSensor = analogRead(GAS_SENSOR);
  Serial.print("GAS: ");
  Serial.println(analogSensor);
  lcd.setCursor(0, 0);
  lcd.print("GAS: ");
  lcd.setCursor(6, 0); 
  lcd.println(analogSensor);
  digitalWrite(BUZZER_PIN, HIGH);
   if (analogSensor > sensorThres)  {
    LINE.notify("ตรวจพบแก๊สรั่วไหล");
    lcd.print(analogSensor);
    lcd.setCursor(0, 1);
    lcd.print("GAS DETECTION");
    delay(500);
    digitalWrite(BUZZER_PIN, LOW);
    delay(500);
    }
  
  //ส่วนการแจ้งเตือนเซ็นเซอร์ตรวจจับไฟไหม้
  if (digitalRead(FiresensorPin) == LOW) {
    Serial.println("Enter !");
    LINE.notify("ตรวจพบประกายไฟ");
    lcd.setCursor(0, 1);
    lcd.print("FIRE DETECTION");
    delay(500);
    digitalWrite(BUZZER_PIN, LOW);
    delay(500);
        digitalWrite(BUZZER_PIN, HIGH);
  }
  delay(1000);
}