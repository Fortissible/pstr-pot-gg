/* Pot GG
https://thingspeak.com/channels/1560863
C0 = soilmoister int
C1 = phmeter ???
c2 = Photoresistor int
*/

#include <DHT.h>
#include <ArduinoJson.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SoftwareSerial.h>
StaticJsonDocument<128> doc;
SoftwareSerial Uno(5,6);
//------------------------------------------------------------------------
float calibration_value = 21.34 + 0.1;
int phval = 0; 
unsigned long int avgval; 
int buffer_arr[10],temp;
float ph_act;
int pH_Value; 
float Voltage;
int friendlyValue;
//------------------------------------------------------------------------
int level;
DHT dht(3, DHT11);
int Tc, RH;
int sensorValue;   
float nilaiph,suhu;

int dryValue = 1023;
int wetValue = 0;
int friendlyDryValue = 0;
int friendlyWetValue = 100;

int sensorPin,tegangan;
float Tc_float;
int photo;
float datamoisture;

//Mux control pins
int s0 = 0;
int s1 = 1;
int s2 = 2;
int s3 = 3;

//Mux in “SIG” pin
int SIG_pin = 0;

void setup() {
    dht.begin();
    /*
    pinMode(s0, OUTPUT);
    pinMode(s1, OUTPUT);
    pinMode(s2, OUTPUT);
    pinMode(s3, OUTPUT);
    
    digitalWrite(s0, LOW);
    digitalWrite(s1, LOW);
    digitalWrite(s2, LOW);
    digitalWrite(s3, LOW); 
    */
    
    Serial.begin(9600);
    Uno.begin(4800);
}
  
void loop() {    
    JsonObject buff = doc.to<JsonObject>();
    
    Serial.println("------- Arduino -------");
    Tc = dht.readTemperature();
    buff["temp"]=Tc;
    delay(100);
    
    RH = dht.readHumidity();
    buff["humid"]=RH;
    delay(100);
    
    Serial.print("Temp : ");
    Serial.print(Tc);
    Serial.print(" | ");
    Serial.print("Humid :");
    Serial.println(RH);
    
    sensorPin = analogRead(0);
    datamoisture = analogRead(1);
    photo = analogRead(2);
    
    /*
    for (int i = 0; i < 3; i++) {
    if (i==2){
      photo = readMux(i);
      int level = map(photo, 0, 1023, 0, 255); 
      Serial.println("Photoresistor Mentah, Mapping:");
      Serial.println(photo);
      Serial.println(level);
    }
    else if(i==0){
        datamoisture = readMux(i);
        Serial.println("Soil Moisture:");
        Serial.println(datamoisture);
    }
    else{
      sensorPin = readMux(i);}
    delay(1000);}   
    */
    
    // soil moisture -----------------------------
    int rawValue = datamoisture;
    Serial.print("Raw: ");
    Serial.print(rawValue);
    Serial.print(" | ");
    
    friendlyValue = map(rawValue, dryValue, wetValue, friendlyDryValue, friendlyWetValue);
    
    Serial.print("Friendly: ");
    Serial.print(friendlyValue);
    Serial.println("%");
    buff["soilmoist"]=friendlyValue;
    delay(100);

    // Photoresistor -----------------------------
    level = map(photo, 0, 1023, 0, 255);      // map 0--1023 to 0--255
    Serial.print("Photoresistor : ");
    Serial.print(photo);
    Serial.print(", ");
    Serial.println(level);
    buff["photo"] = level;
    delay(100);
    
    pH_Value = sensorPin; 
    Voltage = pH_Value * (5.0 / 1024.0);
    Serial.print("Voltage: "); 
    Serial.print(Voltage); 
    Serial.print(" | ");
    delay(100); 
    ph_act = -5.70 * Voltage + calibration_value;
    
    Serial.print("pH Val: ");
    Serial.println(ph_act);
    Serial.println("");
    buff["ph"] = ph_act;

    serializeJson(buff,Uno);
    doc.clear();
    delay(5000);
}

/*
void send_data() {
  WiFiClient wifi;
  HTTPClient http;
  http.begin(wifi, "http://api.thingspeak.com/update");
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  int status = http.POST("api_key=4ERW3EXA2RPHWYFS&field1=" + String(Tc) +
  "&field2=" + RH +"&field3=" + friendlyValue +"&field4=" + ph_act + "&field5=" + level);
  http.end();
}*/

/*
int readMux(int channel) {
    int controlPin[] = {
      s0,s1,s2,s3};
      
    int muxChannel[3][4] = {
      {0,0,0,0},
      {1,0,0,0}
    };
    
    //loop through the 4 sig 
    for(int i = 0; i < 4; i ++){ 
    digitalWrite(controlPin[i], muxChannel[channel][i]); 
    } 
    
    //read the value at the SIG pin 
    int val = analogRead(SIG_pin); 
    
    //return the value 
    return val; 
} */
