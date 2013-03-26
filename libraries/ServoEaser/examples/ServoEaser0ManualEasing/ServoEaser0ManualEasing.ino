//
// ServoEaser0ManualEasing.ino - show how to send manual easing commands
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

//
void setup()
{
  Serial.begin(19200);
  Serial.println("ServoEaser0ManualEasing");

  // first, get the servo ready
  servo1.attach( servoPin );

  //Serial.println("moving to 90 degrees immediately");
  // begin with a framerate, no starting position, we don't know
  servoEaser.begin( servo1, servoFrameMillis ); 

  // do manual easing
  Serial.println("moving to 20 degrees over 2 seconds");
  servoEaser.easeTo( 20, 2000);
}

//
void loop()
{
  servoEaser.update();

  printCurrPos();

  if( servoEaser.hasArrived() ) { 
    Serial.println("ServoEaser arrived at 20 degrees,");
    Serial.println("going to 160 in 2 seconds");
    servoEaser.easeTo( 160, 2000 );
  }
    
}

void printCurrPos()
{
    static long nextPrintTime;
    
    if( (long)(millis() - nextPrintTime) >= 0 ) {
        nextPrintTime += 200; // 100 millisecs between print statements
        Serial.print("currPos: ");
        Serial.println( servoEaser.getCurrPos() );
    }
}

