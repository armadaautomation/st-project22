#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

/* Put your SSID & Password */
const char* ssid = "drainage";  // Enter SSID here
const char* password = "12345678";  //Enter Password here

/* Put IP Address details */
IPAddress local_ip(192,168,43,1);
IPAddress gateway(192,168,43,1);
IPAddress subnet(255,255,255,0);

ESP8266WebServer server(80);

uint8_t Relay4 = 5;
uint8_t Relay3 = 4;

uint8_t Relay2 = 0;
uint8_t Relay1 = 14;

uint8_t Relay5 = 12;

bool Leftstatus = LOW;
bool Rightstatus = LOW;
bool Straightstatus = LOW;
bool cleaning = LOW;
bool watering = LOW;
bool freetlizer = LOW;
bool ploughing = LOW;


void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);

  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  WiFi.softAP(ssid, password);
  Serial.println(WiFi.softAPIP());
  delay(100);
  
  server.on("/", handle_OnConnect);
  
  server.on("/Lefton", handle_Lefton);
  server.on("/Leftoff", handle_Leftoff);
  
  server.on("/Righton", handle_Righton);
  server.on("/Rightoff", handle_Rightoff);
  
  server.on("/Straighton", handle_Straighton);
  server.on("/Straightoff", handle_Straightoff);
  
  server.on("/cleanOn", handle_cleanon);
  server.on("/cleanOff", handle_cleanoff);

  server.on("/wateringon", handle_wateringon);
  server.on("/wateringoff", handle_wateringoff);

     
  server.begin();
  Serial.println("HTTP server started");

  pinMode(Relay1, OUTPUT);
  digitalWrite(Relay1, HIGH);
  
  pinMode(Relay2, OUTPUT);
  digitalWrite(Relay2, HIGH);
  
  pinMode(Relay3, OUTPUT);
  digitalWrite(Relay3, HIGH);
  
  pinMode(Relay4, OUTPUT);
  digitalWrite(Relay4, HIGH);

  pinMode(Relay5, OUTPUT);
  digitalWrite(Relay5, HIGH);

  delay(1000);

}


void loop() {
  // put your main code here, to run repeatedly:
  server.handleClient();


  if( Leftstatus || Straightstatus )
  {
      digitalWrite(Relay1, LOW);
      Serial.println("Relay 1 On");
          
  }
  else 
  {
      digitalWrite(Relay1, HIGH);
      Serial.println("Relay 1 Off");
  }
  
  if ( Rightstatus || Straightstatus  )
  {
     digitalWrite(Relay2, LOW);
     Serial.println("Relay 2 On");
  }
  else 
  {
    digitalWrite(Relay2, HIGH);
    Serial.println("Relay 2 Off");
  }
  

  if( cleaning  )
  {
      digitalWrite(Relay3, LOW);
      Serial.println("Relay 3 On");
  }
  else
  {
      digitalWrite(Relay3, HIGH);
      Serial.println("Relay 3 Off");
  }

  if( watering )
  {
      digitalWrite(Relay4, LOW);
      Serial.println("Relay 4 On");
  }
  else
  {
       digitalWrite(Relay4, HIGH);
       Serial.println("Relay 4 Off");
  }


  delay(1000);

}

void handle_OnConnect() {
  
  Leftstatus = LOW;
  Rightstatus = LOW;
  Straightstatus = LOW;

  
  Serial.println("GPIO7 Status: OFF | GPIO6 Status: OFF");
  server.send(200, "text/html", SendHTML(Leftstatus, Rightstatus, Straightstatus, cleaning, watering )); 
}

void handle_Lefton() {
  Leftstatus = HIGH;
  Rightstatus = LOW;
  Straightstatus = LOW;
  
  Serial.println("GPIO7 Status: ON");
  server.send(200, "text/html", SendHTML(Leftstatus, Rightstatus, Straightstatus, cleaning, watering)); 
}

void handle_Leftoff() {
  
  Leftstatus = LOW;
  Rightstatus = LOW;
  Straightstatus = LOW;
  
  Serial.println("GPIO7 Status: OFF");
  server.send(200, "text/html", SendHTML(Leftstatus, Rightstatus, Straightstatus, cleaning, watering )); 
  
}

void handle_Righton() {
  Leftstatus = LOW;
  Rightstatus = HIGH;
  Straightstatus = LOW;
  
  Serial.println("GPIO7 Status: ON");
  server.send(200, "text/html", SendHTML(Leftstatus, Rightstatus, Straightstatus, cleaning, watering )); 
}

void handle_Rightoff() {
  
  Leftstatus = LOW;
  Rightstatus = LOW;
  Straightstatus = LOW;
  
  Serial.println("GPIO7 Status: OFF");
  server.send(200, "text/html", SendHTML(Leftstatus, Rightstatus, Straightstatus, cleaning, watering )); 
  
}

void handle_Straighton() {
  Leftstatus = LOW;
  Rightstatus = LOW;
  Straightstatus = HIGH;
  
  Serial.println("GPIO7 Status: ON");
  server.send(200, "text/html", SendHTML(Leftstatus, Rightstatus, Straightstatus, cleaning, watering )); 
}

void handle_Straightoff() {
  
  Leftstatus = LOW;
  Rightstatus = LOW;
  Straightstatus = LOW;
  
  Serial.println("GPIO7 Status: OFF");
  server.send(200, "text/html", SendHTML(Leftstatus, Rightstatus, Straightstatus, cleaning, watering )); 
  
}

void handle_cleanon() {
  
  
  cleaning = HIGH;
  
  Serial.println("GPIO7 Status: ON");
  server.send(200, "text/html", SendHTML(Leftstatus, Rightstatus, Straightstatus, cleaning, watering)); 
}

void handle_cleanoff() {

  cleaning = LOW;
  
  Serial.println("GPIO7 Status: OFF");
  server.send(200, "text/html", SendHTML(Leftstatus, Rightstatus, Straightstatus, cleaning, watering )); 
  
}

void handle_wateringon() {

  watering = HIGH;
  
  Serial.println("GPIO7 Status: OFF");
  server.send(200, "text/html", SendHTML(Leftstatus, Rightstatus, Straightstatus, cleaning, watering )); 
  
}

void handle_wateringoff() {

  watering = LOW;
  
  Serial.println("GPIO7 Status: OFF");
  server.send(200, "text/html", SendHTML(Leftstatus, Rightstatus, Straightstatus, cleaning, watering )); 
  
}

bool blntemp = false;
bool blnhum = false;

String SendHTML(uint8_t Leftstat,uint8_t Rightstat, uint8_t Straight,uint8_t cleaning, uint8_t watering ){
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

  if( blnhum )
  {
      blnhum = false;
      ptr +="<h3> HUMIDITY : 45 % </h1>\n";
  }
  else
  {
      blnhum = true;
      ptr +="<h3> HUMIDITY : 35 % </h1>\n";
  }

  if( blntemp )
  {
      blntemp = false;
      ptr +="<h3> TEMP : 35 C </h1>\n";
  }
  else
  {
       blntemp = true;
       ptr +="<h3> TEMP : 36 C </h1>\n";
  }
  
  
   if(Leftstat)
  {ptr +="<p>LEFT: ON</p><a class=\"button button-off\" href=\"/Leftoff\">OFF</a>\n";}
  else
  {ptr +="<p>LEFT: OFF</p><a class=\"button button-on\" href=\"/Lefton\">ON</a>\n";}

  if(Rightstat)
  {ptr +="<p>RIGHT: ON</p><a class=\"button button-off\" href=\"/Rightoff\">OFF</a>\n";}
  else
  {ptr +="<p>RIGHT: OFF</p><a class=\"button button-on\" href=\"/Righton\">ON</a>\n";}

   if(Straight)
  {ptr +="<p>STRIGHT: ON</p><a class=\"button button-off\" href=\"/Straightoff\">OFF</a>\n";}
  else
  {ptr +="<p>STRIGHT: OFF</p><a class=\"button button-on\" href=\"/Straighton\">ON</a>\n";}

   if(cleaning)
  {ptr +="<p>cleaning: ON</p><a class=\"button button-off\" href=\"/cleanOff\">OFF</a>\n";}
  else
  {ptr +="<p>cleaning: OFF</p><a class=\"button button-on\" href=\"/cleanOn\">ON</a>\n";}

   if(watering)
  {ptr +="<p>watering : ON </p><a class=\"button button-off\" href=\"/wateringoff\">OFF</a>\n";}
  else
  {ptr +="<p>watering: OFF</p><a class=\"button button-on\" href=\"/wateringon\">ON</a>\n";}

  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}
