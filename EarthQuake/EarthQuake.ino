//https://lastminuteengineers.com/adxl335-accelerometer-arduino-tutorial/

#include <ESP8266WiFi.h>        // Include the Wi-Fi library
#include <ESP8266WebServer.h>
#include "DHT20.h"

/* Put your SSID & Password */
const char* ssid = "earthquake";  // Enter SSID here
const char* password = "12345678";  //Enter Password here

/* Put IP Address details */
IPAddress local_ip(192,168,43,1);
IPAddress gateway(192,168,43,1);
IPAddress subnet(255,255,255,0);

ESP8266WebServer server(80);

DHT20 DHT;


const int analog=A0; // Connect anyone accelerometer input

#define Relay1 D1
#define Relay2 D2
#define Relay3 D3
#define Relay4 D4

// initialize minimum and maximum Raw Ranges for each axis
int RawMin = 0;
int RawMax = 1023;

// Take multiple samples to reduce noise
const int sampleSize = 10;

bool blnrelay1 = false;
bool blnrelay2 = false;



void setup() 
{
  analogReference(EXTERNAL);
  Serial.begin(9600);
  Serial.println("Weclome");
  
//  pinMode(Relay1,OUTPUT);
//  pinMode(Relay2,OUTPUT);
//  pinMode(Relay3,OUTPUT);
//  pinMode(Relay4,OUTPUT);


  pinMode(Relay1,INPUT);
  pinMode(Relay2,INPUT);
  pinMode(Relay3,INPUT);
  pinMode(Relay4,INPUT);

 WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  WiFi.softAP(ssid, password);
  Serial.println(WiFi.softAPIP());
  delay(100);
  
  server.on("/", handle_OnConnect);
  
  server.on("/Lefton", handle_Lefton);
  server.on("/Righton", handle_Righton);
  
  delay(2000);

  server.begin();
  Serial.println("HTTP server started");

 
}

int xRaw2 = 0;

bool blnstart = false;
 
void loop() 
{
   
  server.handleClient();

  if( blnrelay1 )
  {
      blnrelay1 = false;
      pinMode(Relay1,OUTPUT);                
      digitalWrite(Relay1,LOW);
      delay(1000);

      pinMode(Relay1,INPUT);  
  }

  if( blnrelay2 )
  {
      blnrelay2 = false;

      pinMode(Relay2,OUTPUT);                             
      digitalWrite(Relay2,LOW);
      Serial.println("Relay2 is ON");
      delay(1000);
    // pinMode(Relay2,OUTPUT);
      pinMode(Relay2,INPUT);
      Serial.println("Relay2 is OFF");
  }
      
  //Read raw values
  int xRaw1 = ReadAxis(xInput);
  //int yRaw = ReadAxis(yInput);
  //int zRaw = ReadAxis(zInput);

  // Convert raw values to 'milli-Gs"
  long xScaled = map(xRaw1, RawMin, RawMax, -3000, 3000);
  //long yScaled = map(yRaw, RawMin, RawMax, -3000, 3000);
  //long zScaled = map(zRaw, RawMin, RawMax, -3000, 3000);

  //re-scale to fractional Gs
  float xAccel = xScaled / 1000.0;
  //float yAccel = yScaled / 1000.0;
  //float zAccel = zScaled / 1000.0;

  //Serial.print("X, Y, Z  :: ");
 /Serial.println("xRaw1");
 Serial.println(xRaw1);
  Serial.println("-----");



  int xRaw2 = ReadAxis(xInput);
  int yRaw = ReadAxis(yInput);
  int zRaw = ReadAxis(zInput);

//  Convert raw values to 'milli-Gs"
  long xScaled2 = map(xRaw2, RawMin, RawMax, -3000, 3000);
  long yScaled = map(yRaw, RawMin, RawMax, -3000, 3000);
  long zScaled = map(zRaw, RawMin, RawMax, -3000, 3000);

  re-scale to fractional Gs
  float xAccel2 = xScaled2 / 1000.0;
  float yAccel = yScaled / 1000.0;
  float zAccel = zScaled / 1000.0;

  if( xRaw2 == 0 )
    xRaw2 = xRaw1;

  //Serial.print("X, Y, Z  :: ");
  Serial.println("xRaw2");
  Serial.println(xRaw2);
  Serial.println("-----");

  if( blnstart )
  {
      xRaw2 = xRaw1;
  }
  
 if ( abs( xRaw1 -  xRaw2) > 20 )
 {
  blnstart = true;
 // pinMode(Relay1,INPUT);          
  pinMode(Relay1,OUTPUT);                
  digitalWrite(Relay1,LOW);
  Serial.println("Relay1 is ON");
  delay(3000);
 // pinMode(Relay1,OUTPUT);
  pinMode(Relay1,INPUT);  
  Serial.println("Relay1 is OFF");

   delay(5000);
  
 // pinMode(Relay2,INPUT);   
   pinMode(Relay2,OUTPUT);                             
  digitalWrite(Relay2,LOW);
  Serial.println("Relay2 is ON");
  delay(3000);
 // pinMode(Relay2,OUTPUT);
  pinMode(Relay2,INPUT);
  Serial.println("Relay2 is OFF");
  
  //pinMode(Relay3,INPUT);     
  pinMode(Relay3,OUTPUT);           
  digitalWrite(Relay3,LOW);

  //pinMode(Relay4,INPUT); 
   pinMode(Relay4,OUTPUT);                
   digitalWrite(Relay4,LOW);
  
  Serial.println("Relay3 and Relay4 is ON");

    xRaw1 = ReadAxis(xInput);
 }

  //xRaw2 = xRaw1;

//   pinMode(Relay3,OUTPUT);
//   pinMode(Relay4,OUTPUT);

//     pinMode(Relay3,INPUT);
//     pinMode(Relay4,INPUT);

// Serial.println("Relay3 and Relay4 is OFF");

//  Serial.print(yRaw);
//  Serial.print(", ");
//  Serial.print(zRaw);
//  Serial.print(" :: ");
//  Serial.println(xAccel,0);
//  Serial.println("G, ");
//  Serial.print(yAccel,0);
//  Serial.print("G, ");
//  Serial.print(zAccel,0);
//  Serial.println("G");

  delay(1000);
}

// Take samples and return the average
int ReadAxis(int axisPin)
{
  long reading = 0;
  analogRead(axisPin);
  delay(1);
  for (int i = 0; i < sampleSize; i++)
  {
  reading += analogRead(axisPin);
  }
  return reading/sampleSize;
}


void handle_OnConnect() {
  
  Serial.println("GPIO7 Status: OFF | GPIO6 Status: OFF");
  server.send(200, "text/html", SendHTML()); 
}

void handle_Lefton() {
  
  blnrelay1 = true;  
  Serial.println("GPIO7 Status: ON");
  server.send(200, "text/html", SendHTML()); 
}

void handle_Righton() {

   blnrelay2 = true;
  Serial.println("GPIO7 Status: ON");
  server.send(200, "text/html", SendHTML()); 
}


bool blntemp = false;
bool blnhum = false;
bool blnoxgen = false;

String SendHTML(){
  
  char strtemp[32];
  
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
  ptr +="<h1> EARTH QUAKE </h1>\n";

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
      ptr +="<h3> TEMP : 36.7 C </h1>\n";
  }
  else
  {
       blntemp = true;
       ptr +="<h3> TEMP : 36.1 C </h1>\n";
  }

  if( blnoxgen )
  {
      blnoxgen = false;
      ptr +="<h3> OXGEN : 99 % </h1>\n";
  }
  else
  {
       blnoxgen = true;
       ptr +="<h3> OXGEN : 98 % </h1>\n";
  }
 
  

  ptr +="<p>BED </p><a class=\"button button-on\" href=\"/Lefton\">ON</a>\n";
  
  ptr +="<p>BED </p><a class=\"button button-on\" href=\"/Righton\">ON</a>\n";
  

  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}
