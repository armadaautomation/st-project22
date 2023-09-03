#include <ESP8266WiFi.h>
#include <WiFiUdp.h>


WiFiUDP Udp;
unsigned int localUdpPort = 2210;
char incomingPacket[256];
char replyPacket[256];

/* Put your SSID & Password */
const char* ssid = "landslider";  // Enter SSID here
const char* password = "12345678";  //Enter Password here

#define PIN_INN_1             5  // GPIO 5 OR D1
#define PIN_INN_2             4  // GPIO 4 OR D2

void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);
  Serial.println();

  WiFi.begin(ssid, password);

  pinMode(2, OUTPUT); 

  digitalWrite(2, LOW); 
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  digitalWrite(2, HIGH);

  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());

  Udp.begin(localUdpPort);

//  pinMode(PIN_INN_1, INPUT);
//  pinMode(PIN_INN_2, INPUT);

 pinMode(PIN_INN_1, INPUT_PULLUP);    //Pull up the pin, so by default the input will be high.
 pinMode(PIN_INN_2, INPUT_PULLUP);    //Pull up the pin, so by default the input will be high.

}

void loop() {
  // put your main code here, to run repeatedly:

  int inn1 = digitalRead(PIN_INN_1);
  int inn2 = digitalRead(PIN_INN_2);

  memset(replyPacket, 0, sizeof(replyPacket));
  strcpy(replyPacket, "WORKING");

     // if( inn1 == 1 || inn2 == 1 )
    if( inn1 == 0 || inn2 == 0 )            // If ever any one of the input gives low, the if condition will become true and execute commands inside the if.

  {
      memset(replyPacket, 0, sizeof(replyPacket));
      strcpy(replyPacket, "land");
      delay(2000);
//      digitalWrite(PIN_INN_1, LOW);
//      digitalWrite(PIN_INN_2, LOW);
      
//      digitalWrite(inn1, LOW);
//      digitalWrite(inn2,LOW);
  }

   
  Serial.println(replyPacket);
   

 Udp.beginPacket("192.168.43.1", localUdpPort);
 Udp.write(replyPacket);
 Udp.endPacket();
 delay(10);
  

}
