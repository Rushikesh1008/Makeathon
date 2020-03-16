#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#include <ESP8266HTTPClient.h>
#include <FirebaseArduino.h>
#include <ArduinoJson.h>
#include "HX711.h"

HX711 cell(D7, D8);
// Set these to run example.
#define FIREBASE_HOST "makerswave-86fc5.firebaseio.com"
#define FIREBASE_AUTH "koqxfhEj3rsp3zyhNpaELdHJbf19fRFoqkl0BTKv"
#define WIFI_SSID "DS LAB"
#define WIFI_PASSWORD "12345678"
 
void setup()
{
  // Debug console
  Serial.begin(9600);
  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED)
      {
    Serial.print(".");
    delay(500);
      }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.setString("SIH/Patient Calling","Yes");
  Firebase.setInt("SIH/Bottles to be applied",2);
  Firebase.setString("Patient Calling","Yes");
  Firebase.setInt("Bottles to be applied",2);
}

float sdata = 0;
long val =0;
int i =0;
float rate = 0;
float w1 = 0;

void loop()
{
val = cell.read(); // most recent reading 
sdata = (val-8324056)/12434.0f*18;
Serial.println(sdata);
Firebase.setFloat("Fluid Remaining",sdata);
Firebase.setFloat("SIH/Fluid Remaining",sdata);
if(i==0)
{
  w1 = sdata;
}
i = i+1;
if(i==10)
{
  rate = (sdata-w1)/10;
  i=0; 
  Serial.print("Rate: ");
  Serial.println(rate);
  Firebase.setFloat("SIH/Flow Rate",rate);
  Firebase.setFloat("Flow Rate",rate);
}
delay(1000);            
}
