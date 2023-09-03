#include <WiFi.h>     /////clint webserver with ajax
#include <WebServer.h>
#include <WiFiClient.h>

#include "bike.h"

#define LED 27

const char* ssid = "Office 4G";

const char* password = "3dotsinnovations";

WebServer server(80);


void handleRoot() 

{

 String s = webpage;

 server.send(200, "text/html", s);

}


void sensor_data() 

{

 int a = analogRead(A0);

 int temp= a/4.35;

 String sensor_value = String(temp);

 server.send(200, "text/plane", sensor_value);

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


void setup(void)

{

  Serial.begin(115200);

  WiFi.begin(ssid, password);

  Serial.println("");

  pinMode(LED,OUTPUT); 

  

  while (WiFi.status() != WL_CONNECTED)

  {

    Serial.print("Connecting...");

  }

  Serial.println("");

  Serial.print("Connected to ");

  Serial.println(ssid);

  Serial.print("IP address: ");

  Serial.println(WiFi.localIP());

 

  server.on("/", handleRoot);

  server.on("/led_set", led_control);

  server.on("/adcread", sensor_data);

  server.begin();

}


void loop(void)

{

  server.handleClient();

}
