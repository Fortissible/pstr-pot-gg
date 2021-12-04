/* Pot GG
https://thingspeak.com/channels/1560863
C0 = soilmoister int
C1 = phmeter ???
c2 = Photoresistor int
*/
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#define SSID "ZahraFiber"
#define PASS "w2742064"
StaticJsonDocument<128> doc;
SoftwareSerial Wemos(D6,D5);
String dat,val;
float ph_meter, SoilMoist, photo, Tc, RH;

void setup() {
    pinMode(D2,INPUT);
    pinMode(D3,OUTPUT);
    WiFi.begin(SSID, PASS);
    while (WiFi.status() != WL_CONNECTED) delay(500);
    Serial.begin(9600);
    Wemos.begin(4800);
}
  
void loop() {
  DeserializationError error = deserializeJson(doc,Wemos);
  JsonObject buff = doc.as<JsonObject>();
  
  if(error){
    Serial.println("Packet Loss\n");
    delay(100);
  }

  else{
    ph_meter = buff["ph"];
    SoilMoist = buff["soilmoist"];
    photo = buff["photo"];
    Tc = buff["temp"];
    RH = buff["humid"];
    
    Serial.print("temp  : ");
    Serial.println(Tc);
    Serial.print("humid : ");
    Serial.println(RH);
    Serial.print("ph    : ");
    Serial.println(ph_meter);
    Serial.print("moist : ");
    Serial.println(SoilMoist);
    Serial.print("photo : ");
    Serial.println(photo);
    Serial.println("");

    if (ph_meter != 0 && photo != 0 && Tc != 0 && RH != 0)
      if (WiFi.status() == WL_CONNECTED){
        send_data();
        Serial.println("------ Uploading ------");
        delay(20000);
      }
    else
      delay(100);
  }
  doc.clear();
}

void send_data() {
  WiFiClient wifi;
  HTTPClient http;
  http.begin(wifi, "http://api.thingspeak.com/update");
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  int status = http.POST("api_key=4ERW3EXA2RPHWYFS&field1=" + String(Tc) +
  "&field2=" + RH +"&field3=" + SoilMoist +"&field4=" + ph_meter + "&field5=" + photo);
  http.end();
}
