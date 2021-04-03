#include <Wire.h>
#include <Ticker.h>
#include <LiquidCrystal_I2C.h>
#include <ESP8266WiFi.h> // ESP8266WiFi.h library
Ticker x;
const char* ssid     = "Rohana";// replace subscribe with your WiFi SSID(Name)

const char* password = "rohana1964";//replace with Your Wifi Password name

const char* host = "api.thingspeak.com";

const char* writeAPIKey = "W00WNVAFM9EP99B2"; //copy yout ThingSpeak channel API Key.
#define trig 01
#define echo 02

LiquidCrystal_I2C lcd(0x27, 16, 2);
int G_sensorValue = 0;
double duration;
double cm;
double inch;
void setup() {
ThingSpeak_Setup();
  pinMode(OUTPUT, 3);
  pinMode(OUTPUT, 12);
  pinMode(OUTPUT, 13);
  pinMode(OUTPUT, 15);
  digitalWrite(3, LOW);
  digitalWrite(12, HIGH);
  digitalWrite(13, HIGH);
  digitalWrite(15, HIGH);
  lcd.begin();
 
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  x.attach_ms(1000, Gas_sensor);

}

void loop() {

  Ultra();
   Gas_range_led();

}
void Ultra() {
  digitalWrite(trig, 0);
  delayMicroseconds(10);
  digitalWrite(trig, 1);
  delayMicroseconds(10);
  digitalWrite(trig, 0);

  duration = pulseIn(echo, 1);
  cm = duration / 58;
  inch = duration / 148;
 
  lcd.clear();
  lcd.print("Distance");
  lcd.print(cm);
  lcd.setCursor(01, 02);
  lcd.print("sensorValue= ");
  lcd.print(sensorValue);
  delay(200);
}
void Gas_sensor() {
  G_sensorValue = analogRead(A0);

}
void Gas_range_led() {
  if (G_sensorValue > 0 && G_sensorValue < 200) {
    digitalWrite(14, HIGH);
  } else if (G_sensorValue > 200 && G_sensorValue < 500) {
    digitalWrite(12, HIGH);
  } else if (G_sensorValue > 500 && G_sensorValue < 1050) {
    digitalWrite(13, HIGH);
  }

}
void ThingSpeak_Setup(){
  // Initialize sensor

 Serial.begin(115200);

 

  delay(1000);

 Serial.println("Connecting to ");

       Serial.println(ssid);

//  Connect to WiFi network

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {

    delay(500);

    Serial.print(".");

  }

   Serial.println("");

   Serial.println("WiFi connected");
}
void Thing_Speak(){
  float humidity=33.33;

 float temperature=50.55;



  if (isnan(humidity) || isnan(sensorValue)) {

    return;

  }



// make TCP connections

  WiFiClient client;

  const int httpPort = 80;

  if (!client.connect(host, httpPort)) {

    return;

  }



  String url = "/update?key=";

  url+=writeAPIKey;

  url+="&field1=";

  url+=String(sensorValue);

  url+="&field2=";

  url+=String(humidity);

  url+="\r\n";

  

  // Request to the server

  client.print(String("GET ") + url + " HTTP/1.1\r\n" +

               "Host: " + host + "\r\n" + 

               "Connection: close\r\n\r\n");

  Serial.print("Temperature:");

  Serial.print(sensorValue);

  Serial.print("\n");

  Serial.print("Humidity:");

  Serial.println(humidity);

  Serial.println("Send to ThingSpeak.\n");

client.stop();

  Serial.println("Wait for 15 sec to update next datapack in thingSpeak");

    delay(1000);
}
