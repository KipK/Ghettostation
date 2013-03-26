//
// ServoEaser1RandEasing.ino -- simple example showing use of ServoEaser
//
//
// 2011, TeamPneumo, Tod E. Kurt, http://todbot.com/blog/
//
//

#include <Servo.h>
#include "ServoEaser.h"

const int ledPin   = 13; 
const int servoPin = 7;

int servoFrameMillis = 20;  // minimum time between servo updates

Servo servo1; 
ServoEaser servoEaser;

unsigned long lastMillis;

//
void setup()
{
  Serial.begin(19200);
  Serial.println("ServoEaser1RandEasing");

  servo1.attach( servoPin );

  servoEaser.begin( servo1, servoFrameMillis);
  servoEaser.useMicroseconds( true );  // fine-control mode

  servoEaser.easeTo( 90, 5000);

}

//
void loop()
{
  servoEaser.update();

  if( servoEaser.hasArrived() ) {
    lastMillis = millis();
    int angle    = random(180);
    int duration = random(200,15000);
    Serial.print("easing to angle:");
    Serial.print(angle);
    Serial.print("\t duration:");
    Serial.println(duration);
    servoEaser.easeTo( angle, duration );
  }
}


