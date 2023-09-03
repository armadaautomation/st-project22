#include<AFMotor.h>

AF_DCMotor motor(4);

void setup()
{
  Serial.begin(9600);
   motor.setSpeed(255);
}

void loop()
{
  motor.run(FORWARD);
}
