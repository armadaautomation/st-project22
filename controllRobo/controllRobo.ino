#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Servo.h>

/* Put your SSID & Password */
const char* ssid = "robo";  // Enter SSID here
const char* password = "12345678";  //Enter Password here

/* Put IP Address details */
IPAddress local_ip(192,168,43,1);
IPAddress gateway(192,168,43,1);
IPAddress subnet(255,255,255,0);

ESP8266WebServer server(80);

uint8_t Relay4 = 4;
uint8_t Relay3 = 0;

uint8_t Relay2 = 2;
uint8_t Relay1 = 14;

uint8_t micin = A0;
uint8_t touchin = 12;

uint8_t motionin = 13;

uint8_t IR1in = 10;
uint8_t IR2in = 9;

uint8_t Servomotor = 5;

bool Leftstatus = LOW;
bool Rightstatus = LOW;
bool Straightstatus = LOW;
bool Buzzerstatus = LOW;
bool robostatus = LOW;
bool servostatus = LOW;

Servo servo;

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
  
  server.on("/Buzzeron", handle_Buzzeron);
  server.on("/Buzzeroff", handle_Buzzeroff);

  server.on("/Servoon", handle_Servoon);
  server.on("/Servooff", handle_Servooff);

  server.on("/roboon", handle_roboon);
  server.on("/robooff", handle_robooff);
    
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

  pinMode(micin, INPUT);
  pinMode(touchin, INPUT);

  pinMode(motionin, INPUT);

  pinMode(IR1in, INPUT);
  pinMode(IR2in, INPUT);

  servo.attach(Servomotor); //D1

  servo.write(90);

  delay(2000);

}

uint8_t motioncntr = 0;
uint8_t toucncntr = 0;

void loop() {
  // put your main code here, to run repeatedly:
  server.handleClient();

  uint8_t adcval = analogRead(micin);

  uint8_t touchval = digitalRead(touchin);
  uint8_t motionval = digitalRead(motionin);
  uint8_t IR1val = digitalRead(IR1in);
  uint8_t IR2val = digitalRead(IR2in);

  Serial.print("adcval : ");
  Serial.println(adcval);

  Serial.print("motionval : ");
  Serial.println(motionval);

  Serial.print("touchval : ");
  Serial.println(touchval);

  Serial.print("IR1val : ");
  Serial.println(IR1val);

  Serial.print("IR2val : ");
  Serial.println(IR2val);

  if( motionval == 0 )
  {
      motioncntr = 10;
  }

  if( motioncntr > 0 )
  {
      if( (motioncntr % 2) == 0 )
      {
          digitalWrite(Relay3, LOW);
          digitalWrite(Relay4, LOW);

          Serial.println("Relay 3 On");
          Serial.println("Relay 4 On");
      }
      else
      {
          digitalWrite(Relay3, HIGH);
          digitalWrite(Relay4, HIGH);

          Serial.println("Relay 3 Off");
          Serial.println("Relay 4 Off");
          
      }
      --motioncntr;
  }
  if( motioncntr == 0 )
  {
      digitalWrite(Relay3, HIGH);
      digitalWrite(Relay4, HIGH);

     Serial.println("Relay 3 Off");
     Serial.println("Relay 4 Off");
  }

  if( touchval == 1 || servostatus )
  {
      servo.write(0);
      toucncntr = 5;
      servostatus = LOW;
      
      Serial.println("Server ON");
  }

  if( toucncntr > 0 )
  {
      --toucncntr;

      if( toucncntr == 0 )
      {
          servo.write(90);
          Serial.println("Server Off");
      }
  }


  if( Leftstatus || IR1val == 0 || Straightstatus )
  {
      digitalWrite(Relay1, LOW);
      Serial.println("Relay 1 On");
          
  }
  else 
  {
      digitalWrite(Relay1, HIGH);
      Serial.println("Relay 1 Off");
  }
  
  if ( Rightstatus || IR2val == 0 || Straightstatus  )
  {
     digitalWrite(Relay2, LOW);
     Serial.println("Relay 2 On");
  }
  else 
  {
    digitalWrite(Relay2, HIGH);
    Serial.println("Relay 2 Off");
  }
  

  if( Buzzerstatus || ( IR1val == 0 && IR2val == 0 ) )
  {
      digitalWrite(Relay3, LOW);
      Serial.println("Relay 3 On");
  }
  else
  {
      digitalWrite(Relay3, HIGH);
      Serial.println("Relay 3 Off");
  }

  if( robostatus )
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
  server.send(200, "text/html", SendHTML(Leftstatus, Rightstatus, Straightstatus, Buzzerstatus, servostatus, robostatus  )); 
}

void handle_Lefton() {
  Leftstatus = HIGH;
  Rightstatus = LOW;
  Straightstatus = LOW;
  
  Serial.println("GPIO7 Status: ON");
  server.send(200, "text/html", SendHTML(Leftstatus, Rightstatus, Straightstatus, Buzzerstatus, servostatus, robostatus ));  
}

void handle_Leftoff() {
  
  Leftstatus = LOW;
  Rightstatus = LOW;
  Straightstatus = LOW;
  
  Serial.println("GPIO7 Status: OFF");
  server.send(200, "text/html", SendHTML(Leftstatus, Rightstatus, Straightstatus, Buzzerstatus, servostatus, robostatus ));  
  
}

void handle_Righton() {
  Leftstatus = LOW;
  Rightstatus = HIGH;
  Straightstatus = LOW;
  
  Serial.println("GPIO7 Status: ON");
  server.send(200, "text/html", SendHTML(Leftstatus, Rightstatus, Straightstatus, Buzzerstatus, servostatus, robostatus ));  
}

void handle_Rightoff() {
  
  Leftstatus = LOW;
  Rightstatus = LOW;
  Straightstatus = LOW;
  Buzzerstatus = LOW;
  
  Serial.println("GPIO7 Status: OFF");
  server.send(200, "text/html", SendHTML(Leftstatus, Rightstatus, Straightstatus, Buzzerstatus, servostatus, robostatus ));  
  
}

void handle_Straighton() {
  Leftstatus = LOW;
  Rightstatus = LOW;
  Straightstatus = HIGH;
  
  Serial.println("GPIO7 Status: ON");
  server.send(200, "text/html", SendHTML(Leftstatus, Rightstatus, Straightstatus, Buzzerstatus, servostatus, robostatus ));  
}

void handle_Straightoff() {
  
  Leftstatus = LOW;
  Rightstatus = LOW;
  Straightstatus = LOW;
  
  Serial.println("GPIO7 Status: OFF");
  server.send(200, "text/html", SendHTML(Leftstatus, Rightstatus, Straightstatus, Buzzerstatus, servostatus, robostatus ));  
  
}


void handle_Buzzeron() {
  
  
  Buzzerstatus = HIGH;
  
  Serial.println("GPIO7 Status: ON");
  server.send(200, "text/html", SendHTML(Leftstatus, Rightstatus, Straightstatus, Buzzerstatus, servostatus, robostatus ));  
}

void handle_Buzzeroff() {

  Buzzerstatus = LOW;
  
  Serial.println("GPIO7 Status: OFF");
  server.send(200, "text/html", SendHTML(Leftstatus, Rightstatus, Straightstatus, Buzzerstatus, servostatus, robostatus ));  
  
}

void handle_Servoon() {

  servostatus = HIGH;
  
  Serial.println("GPIO7 Status: OFF");
  server.send(200, "text/html", SendHTML(Leftstatus, Rightstatus, Straightstatus, Buzzerstatus, servostatus, robostatus ));  
  
}

void handle_Servooff() {

  servostatus = LOW;
  
  Serial.println("GPIO7 Status: OFF");
  server.send(200, "text/html", SendHTML(Leftstatus, Rightstatus, Straightstatus, Buzzerstatus, servostatus, robostatus ));  
  
}


void handle_roboon() {

  robostatus = HIGH;
  
  Serial.println("GPIO7 Status: OFF");
  server.send(200, "text/html", SendHTML(Leftstatus, Rightstatus, Straightstatus, Buzzerstatus, servostatus, robostatus ));  
  
}


void handle_robooff() {

  robostatus = LOW;
  
  Serial.println("GPIO7 Status: OFF");
  server.send(200, "text/html", SendHTML(Leftstatus, Rightstatus, Straightstatus, Buzzerstatus, servostatus, robostatus ));  
  
}



String SendHTML(uint8_t Leftstat,uint8_t Rightstat, uint8_t Straight,uint8_t Buzzer, uint8_t servo, uint8_t robo ){
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
  ptr +="<h1> ROBO </h1>\n";
  ptr +="<h3>Using Access Point(AP) Mode</h3>\n";
  
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

   if(Buzzer)
  {ptr +="<p>BUZZER: ON</p><a class=\"button button-off\" href=\"/Buzzeroff\">OFF</a>\n";}
  else
  {ptr +="<p>BUZZER: OFF</p><a class=\"button button-on\" href=\"/Buzzeron\">ON</a>\n";}

   if(servo)
  {ptr +="<p>Medical Kit : ON </p><a class=\"button button-off\" href=\"/Servooff\">CLOSE</a>\n";}
  else
  {ptr +="<p>Medical Kit: OFF</p><a class=\"button button-on\" href=\"/Servoon\">OPEN</a>\n";}

   if(robo)
  {ptr +="<p>ROBO : ON </p><a class=\"button button-off\" href=\"/robooff\">OFF</a>\n";}
  else
  {ptr +="<p>ROBO : OFF</p><a class=\"button button-on\" href=\"/roboon\">ON</a>\n";}


  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}
