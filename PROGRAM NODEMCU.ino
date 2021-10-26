#define BLYNK_PRINT Serial //Pendeklarasian Library
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

#define DHTPIN D4
#define DHTTYPE DHT11
#define KIPAS D6
#define MQ135 A0
#define LAMPU D7
#define LAMPUDARURAT D8
#define BUZZER D3
#define LDR D0

char auth[] = "9_3b-k8udXhjfoXGq7g7SBmQ2KWsmgLP";
char ssid[] = "C2";  
char pass[] = "1223334444"; 

LiquidCrystal_I2C lcd(0x27,16,2);
DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;

void sendSensor(){ 
  
  int h = dht.readHumidity();
  int t = dht.readTemperature(); 
  int KUALITASUDARA  = analogRead(MQ135);
  int DARURAT = digitalRead(LDR);
  Serial.println(t);

  Blynk.virtualWrite(V5, h);
  Blynk.virtualWrite(V6, t);  
  Blynk.virtualWrite(V7, KUALITASUDARA);
  

  lcd.setCursor(0,0);
  lcd.print("suhu:");
  lcd.print(t);
  lcd.print("C");
  lcd.setCursor(0,1);
  lcd.print("k.udara=");
  lcd.print(KUALITASUDARA/(10));
  lcd.print("%");
  
  if (t > 30){
    digitalWrite(KIPAS, HIGH);
    digitalWrite(LAMPU, LOW);
    }
  if (t == 30){
    digitalWrite(KIPAS, LOW);
    digitalWrite(LAMPU, LOW);
    digitalWrite(LAMPUDARURAT, LOW);
    }
  if (t < 30){
    digitalWrite(KIPAS, LOW);
    digitalWrite(LAMPU, HIGH);
    if (DARURAT > 0){
      digitalWrite(LAMPUDARURAT, HIGH);
    }
    if (DARURAT < 1){
      digitalWrite(LAMPUDARURAT, LOW);
    }  
    }
  if (KUALITASUDARA > 400){
    digitalWrite(BUZZER,1);
    delay(100);
    digitalWrite(BUZZER,0);
    delay(100); 
   }
  if (KUALITASUDARA < 400)
   {
     noTone(BUZZER);
   }
   }

void setup() {
  Serial.begin(9600);
  lcd.begin();
  lcd.backlight();
  lcd.setCursor(2,0);
  lcd.print("KANDANG AYAM");
  lcd.setCursor(1,1);
  lcd.print("BERBASIS IOT");
  delay(5000);
  lcd.clear();
  dht.begin();
  pinMode(KIPAS,OUTPUT);
  pinMode(LAMPU,OUTPUT);
  pinMode(MQ135,INPUT);
  pinMode(BUZZER,OUTPUT);
  pinMode(LDR,INPUT);
  pinMode(LAMPUDARURAT,OUTPUT);
  Blynk.begin(auth, ssid, pass);
  timer.setInterval(1000L, sendSensor);
  
}

void loop() {
  Blynk.run();
  timer.run(); 
}
