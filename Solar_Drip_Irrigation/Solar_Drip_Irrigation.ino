#include <WiFi.h>  ///// webserver with ajax
#include <WiFiClient.h>
#include <WebServer.h>
#include "index.h"
#define Relay4  27
#define soil_sensor       36
WebServer server (80);
/* Put your SSID & Password */
const char* ssid = "DRIP IRRIGATION";  // Enter SSID here
const char* password = "12345678";  //Enter Password here

/* Put IP Address details */
IPAddress local_ip(192,168,43,1);
IPAddress gateway(192,168,43,1);
IPAddress subnet(255,255,255,0);


float Soil;
void handleRoot() {
 String s = MAIN_page; //Read HTML contents
 server.send(200, "text/html", s); //Send web page
}

void handleADC() {
  int a = analogRead(soil_sensor);
   Soil = ( 100 - ( (a/4096.00) * 100 ) );
   Serial.println(Soil);
   delay(200);
  String adcValue = String(Soil);
  
  server.send(200,"text/plane",adcValue);
  
}


void setup() {
  

  Serial.begin(115200);
  Serial.println();
  Serial.println("Booting Sketch...");
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  WiFi.softAP(ssid, password);
  Serial.println(WiFi.softAPIP());
  delay(100);
  
  
  server.on("/", handleRoot);
  server.on("/readADC",handleADC);
  
  server.begin();
  Serial.println("HTTP server started");


  
  pinMode(Relay4, OUTPUT);
  digitalWrite(Relay4,LOW);
  
}


void loop() {
  
  server.handleClient();
 if (Soil <=50)
 {
  Serial.println(Soil);
  digitalWrite(Relay4,HIGH);
 }
 else
 {
  digitalWrite(Relay4,LOW); 
 }
 
}
