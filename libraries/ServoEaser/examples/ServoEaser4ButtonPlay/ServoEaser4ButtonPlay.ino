//
// ServoEaser4ButtonPlay.ino -- trigger a move list with a button
//
// 2011, TeamPneumo, Tod E. Kurt, http://todbot.com/blog/
//
//

#include <Servo.h>
#include "ServoEaser.h"

const int ledPin   = 13; 
const int servoPin  = 7;
const int buttonPin = 2;

int servoFrameMillis = 20;  // minimum time between servo updates

Servo servo1; 
ServoEaser servoEaser;

int myServoMovesCount = 3;
// configurable list of servo moves
ServoMove myServoMoves[] = {
// angle, duration
    {45,   500},
    {135,  500},
    {90,  1000},
};


//
void setup()
{
  Serial.begin(19200);
  Serial.println("ServoEaser4ButtonPlay");

  pinMode( buttonPin, INPUT_PULLUP); // turn on internal pullup resistor

  servo1.attach( servoPin );
  servoEaser.begin( servo1, servoFrameMillis);

  Serial.println("waiting for button press.");
}

//
void loop()
{
  servoEaser.update();

  // on button press, play moves once
  if( digitalRead(buttonPin) == LOW   // button press
  //   && !servoEaser.isRunning()   // to trigger only if not running
  ) {
      Serial.println("playing servo moves...");
      servoEaser.play( myServoMoves, myServoMovesCount, 1 );
  }

}
