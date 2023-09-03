#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include "tbike.h"
#define LED  27
WebServer server (80);
/* Put your SSID & Password */
const char* ssid = "SMART SIDDIQ";  // Enter SSID here
const char* password = "12345678";  //Enter Password here

/* Put IP Address details */
IPAddress local_ip(192,168,43,1);
IPAddress gateway(192,168,43,1);
IPAddress subnet(255,255,255,0);



void handleRoot() {
 String s = MAIN_page; //Read HTML contents
 server.send(200, "text/html", s); //Send web page
}

void led_control() 
{
 String state = "OFF";
 String act_state = server.arg("state");
 if(act_state == "1")
 {
  digitalWrite(LED,HIGH); //LED ON
  state = "ON";
 }
 else
 {
  digitalWrite(LED,LOW); //LED OFF
  state = "OFF";
 }

 server.send(200, "text/plane", state);

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
  server.on("/led_set", led_control);
  
  server.begin();
  Serial.println("HTTP server started");


  
  pinMode(LED, OUTPUT);
  digitalWrite(LED,LOW);
  
}


void loop() {
  
  server.handleClient();
 
}
