#include <WiFi.h>
#include <WiFiUdp.h>
#include <WebServer.h>

/* Put your SSID & Password */
const char* ssid = "landslider";  // Enter SSID here
const char* password = "12345678";  //Enter Password here

/* Put IP Address details */
IPAddress local_ip(192,168,43,1);
IPAddress gateway(192,168,43,1);
IPAddress subnet(255,255,255,0);

WiFiUDP Udp;

unsigned int localUdpPort = 2210;
char incomingPacket[256];
char replyPacket[] = "Hi there! Got the message :-)";

#define RELAY_1            17

 WebServer server(80);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  WiFi.softAP(ssid, password);
  Serial.println(WiFi.softAPIP());
  delay(100);

  Udp.begin(localUdpPort);

  pinMode(RELAY_1, OUTPUT );
  //digitalWrite(RELAY_1, LOW );


  server.on("/", handle_OnConnect);
  

}


bool blnland = false;

void loop() {
  // put your main code here, to run repeatedly:
  server.handleClient();
  
  int packetSize = Udp.parsePacket();
  if( packetSize > 0 )
  {
      Serial.printf("Received %d bytes from %s, port %d\n", packetSize, Udp.remoteIP().toString().c_str(), Udp.remotePort());
      
      int len = Udp.read(incomingPacket, 255);
      if (len > 0)
      {
        incomingPacket[len] = '\0';
      }
      Serial.printf("UDP packet contents: %s\n", incomingPacket);

    if( strcmp(incomingPacket, "land") == 0 )
//      if( strcmp(incomingPacket, "Hello") == 0 )

      {
          pinMode(RELAY_1, OUTPUT );
          blnland = true;
          digitalWrite(RELAY_1, HIGH );
          delay(1000);
          digitalWrite(RELAY_1, LOW );
      }
      else
      {
          blnland = false;
      }

     
  }

  delay(100);

}


void handle_OnConnect() {
  
  Serial.println("GPIO7 Status: OFF | GPIO6 Status: OFF");
  server.send(200, "text/html", SendHTML()); 
}


String SendHTML( ){
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<title>LED Control</title>\n";
  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
  ptr +=".button {display: block;width: 80px;background-color: #1abc9c;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  ptr +=".button-on {background-color: #1abc9c;}\n";
  ptr +=".button-on:active {background-color: #16a085;}\n";
  ptr +=".button-off {background-color: #34495e;}\n";
  ptr +=".button-off:active {background-color: #2c3e50;}\n";
  ptr +="p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<h1> AAV ROBO </h1>\n";

  if( blnland )
  {
      ptr +="<h3> LAND SLIDE DETECTED </h1>\n";
  }
  
  
  
  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}
