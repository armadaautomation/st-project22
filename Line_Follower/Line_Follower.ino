#include <WiFi.h>
#include <WebServer.h>

 const char* ssid     = "linefollower";
const char* password = "123456789";

IPAddress local_IP(192,168,43,1);
IPAddress gateway(192,168,43,1);
IPAddress subnet(255,255,255,0);
  

// Relay connection
  #define RELAY_1               23
  #define RELAY_2               22
  #define RELAY_3               21
  #define RELAY_4               19
  #define RELAY_5               18
  #define RELAY_6               5
  #define RELAY_7               17
  #define RELAY_8               16

  #define RELAY_MP3             14

  #define INPUT_1               4
  #define INPUT_2               15
  #define INPUT_3               13


  bool blnstart = false;

  WebServer server(80);

void setup() {

    Serial.begin(9600);

    Serial.print("Setting soft-AP configuration ... ");
    
    Serial.println(WiFi.softAPConfig(local_IP, gateway, subnet) ? "Ready" : "Failed!");

    Serial.print("Setting soft-AP ... ");
    Serial.println(WiFi.softAP(ssid,password) ? "Ready" : "Failed!");

    delay(100);
    
    pinMode(INPUT_1, INPUT_PULLUP );
    pinMode(INPUT_2, INPUT_PULLUP );
    pinMode(INPUT_3, INPUT_PULLUP );

    pinMode(RELAY_1, INPUT );
    pinMode(RELAY_2, INPUT );
    pinMode(RELAY_3, INPUT );
    pinMode(RELAY_4, INPUT );
    pinMode(RELAY_5, INPUT );
    pinMode(RELAY_6, INPUT );
    pinMode(RELAY_7, INPUT );
    pinMode(RELAY_8, INPUT );

    pinMode(RELAY_MP3, OUTPUT );
    digitalWrite(RELAY_MP3, HIGH );

    server.on("/", handle_OnConnect);
    server.on("/Patient1", handle_Patient1);
    server.on("/Patinet2", handle_Patinet2);
    server.on("/Patient3", handle_Patient3);
    server.on("/AllPatient", handle_AllPatient);
  
    server.begin();
    Serial.println("HTTP server started");
        
    
}


int homecounter = 0;
int homewaittimeout = 0;
bool blnpatient1 = false;
bool blnpatient2 = false;
bool blnpatient3 = false;
bool blnpatientall = false;

void loop() {

  server.handleClient();
  
  int Relay1info = -1;
  int Relay2Info = -1;

  int homeinfo = digitalRead(INPUT_1);

  int Motorleft  = digitalRead( INPUT_2 );
  int Motorright = digitalRead( INPUT_3 );

  Serial.print("input :");  
  Serial.print(homeinfo);
  Serial.print(",");  
  Serial.print(Motorleft);
  Serial.print(",");  
  Serial.print(Motorright);
  Serial.print(",");  
  Serial.println(blnstart);

  if( blnstart )
  {

      pinMode(RELAY_7, OUTPUT);
      pinMode(RELAY_8, OUTPUT);
      
      if( Motorleft == 0 )
      {
          pinMode(RELAY_7, OUTPUT);
          pinMode(RELAY_8, INPUT);
          
          Serial.print("Motor Left");  

          homeinfo = 1;

      }

      if( Motorright == 0 )
      {
          pinMode(RELAY_7, INPUT);
          pinMode(RELAY_8, OUTPUT);
          Serial.print("Motor Right");  

          homeinfo = 1;
      }

      if( homewaittimeout > 0 )
        --homewaittimeout;
        
      if( homewaittimeout <= 0 )
      {
        if( homeinfo == 0 )
        {
            homewaittimeout = 10;
            
            ++homecounter;
             Serial.print("Home count :");  
             Serial.println(homecounter);  
             
            if( homecounter <=  3 )
            {
                if( homecounter == 1 )
                {
                    if( blnpatient1 )
                    {
                      Relay1info = RELAY_1;
                      Relay2Info = RELAY_2;
                    }
                }
                else if( homecounter == 2 )
                {
                    if( blnpatient2 )
                    {
                      Relay1info = RELAY_3;
                      Relay2Info = RELAY_4;
                    }
                }
                else if( homecounter == 3 )
                {
                    if( blnpatient3 )
                    {
                      Relay1info = RELAY_5;
                      Relay2Info = RELAY_6;
                    }
                }

                if( Relay1info > 0 )
                {
                    pinMode(RELAY_7, INPUT);
                    pinMode(RELAY_8, INPUT);
    
                    digitalWrite(RELAY_MP3, LOW );
                    delay(100);
                    digitalWrite(RELAY_MP3, HIGH );
                    
                    pinMode(Relay1info, OUTPUT);
                    delay(3000);
                    pinMode(Relay1info, INPUT);
                    delay(1000);
                    pinMode(Relay2Info, OUTPUT);
                    delay(3000);
                    pinMode(Relay2Info, INPUT);
    
                    pinMode(RELAY_7, OUTPUT);
                    pinMode(RELAY_8, OUTPUT);
                }

                
            }
  
            if( homecounter >= 4 )
            {
                blnstart = false;
            }
            
        }
      }

  }
  else
  {
      homecounter = 0;
      homewaittimeout = 0;
      
      pinMode(RELAY_7, INPUT);
      pinMode(RELAY_8, INPUT);
  }

  

  delay(500);
 
}


void handle_OnConnect() {
  Serial.println("GPIO4 Status: OFF | GPIO5 Status: OFF");
  server.send(200, "text/html", SendHTML()); 
}


void handle_Patient1() {
  blnpatient1 = true;
  blnpatient2 = false;
  blnpatient3 = false;
  blnpatientall = false;
  blnstart = !blnstart;
  
  Serial.println("Patient 1 Status: ON");
  server.send(200, "text/html", SendHTML()); 
}

void handle_Patinet2() {
  blnpatient1 = false;
  blnpatient2 = true;
  blnpatient3 = false;
  blnpatientall = false;
  blnstart = !blnstart;
  
  Serial.println("Patient 2 Status: ON");
  server.send(200, "text/html", SendHTML()); 
}

void handle_Patient3() {
  blnpatient1 = false;
  blnpatient2 = false;
  blnpatient3 = true;
  blnpatientall = false;
  blnstart = !blnstart;
  
  Serial.println("Patient 3 Status: ON");
  server.send(200, "text/html", SendHTML()); 
}

void handle_AllPatient() {
  blnpatient1 = true;
  blnpatient2 = true;
  blnpatient3 = true;
  blnpatientall = true;
  blnstart = !blnstart;
  
  Serial.println("Patient All Status: ON");
  server.send(200, "text/html", SendHTML()); 
}

String SendHTML(  )
{
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<title>Line Follow Robot</title>\n";
  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
  ptr +=".button {display: block;width: 80px;background-color: #3498db;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  ptr +=".button-on {background-color: #3498db;}\n";
  ptr +=".button-on:active {background-color: #2980b9;}\n";
  ptr +=".button-off {background-color: #34495e;}\n";
  ptr +=".button-off:active {background-color: #2c3e50;}\n";
  ptr +="p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<h1>Line Follow Robot</h1>\n";
  if( homecounter == 0 )
  {
      ptr +="<h3> Check Me </h3>\n";
  }
    
   if(blnpatient1)
  {ptr +="<p>Status: PATINET 1 </p><a class=\"button button-off\" href=\"/Patient1\">STOP</a>\n";}
  else
  {ptr +="<p>Status: PATINET 1</p><a class=\"button button-on\" href=\"/Patient1\">START</a>\n";}

   if(blnpatient2)
  {ptr +="<p>Status: PATINET 2</p><a class=\"button button-off\" href=\"/Patinet2\">STOP</a>\n";}
  else
  {ptr +="<p>Status: PATINET 2</p><a class=\"button button-on\" href=\"/Patinet2\">START</a>\n";}

   if(blnpatient3)
  {ptr +="<p>Status: PATINET 3</p><a class=\"button button-off\" href=\"/Patient3\">STOP</a>\n";}
  else
  {ptr +="<p>Status: PATINET 3</p><a class=\"button button-on\" href=\"/Patient3\">START</a>\n";}

   if(blnpatientall)
  {ptr +="<p>Status: PATINET ALL</p><a class=\"button button-off\" href=\"/AllPatient\">STOP</a>\n";}
  else
  {ptr +="<p>Status: PATINET ALL</p><a class=\"button button-on\" href=\"/AllPatient\">START</a>\n";}

  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}







 
