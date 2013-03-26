//
// ServoEaser3Callbacks.ino -- showing use of ServoEaser callbak functions
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

int myServoMovesCount = 3;
ServoMove myServoMoves[] = {
// angle, duration
  { 90, 1000},
  { 45, 1000},
  {135, 1000},
};

// a bunch of different easingFunc examples

// from Easing::linearTween()
float linearTween (float t, float b, float c, float d) {
	return c*t/d + b;
}
// from Easing::easeInOutQuad()
float easeInOutQuad (float t, float b, float c, float d) {
	if ((t/=d/2) < 1) return c/2*t*t + b;
	return -c/2 * ((--t)*(t-2) - 1) + b;
}
// from Easing::easeInOutCubic()
float easeInOutCubic (float t, float b, float c, float d) {
	if ((t/=d/2) < 1) return c/2*t*t*t + b;
	return c/2*((t-=2)*t*t + 2) + b;
}
// from Easing::easeInOutQuart()
float easeInOutQuart (float t, float b, float c, float d) {
	if ((t/=d/2) < 1) return c/2*t*t*t*t + b;
	return -c/2 * ((t-=2)*t*t*t - 2) + b;
}
// from Easing::easeInOutQuint()
float easeInOutQuint (float t, float b, float c, float d) {
	if ((t/=d/2) < 1) return c/2*t*t*t*t*t + b;
	return c/2*((t-=2)*t*t*t*t + 2) + b;
}
// bounce easing out
float easeOutBounce (float t, float b, float c, float d) {
	if ((t/=d) < (1/2.75)) {
		return c*(7.5625*t*t) + b;
	} else if (t < (2/2.75)) {
		return c*(7.5625*(t-=(1.5/2.75))*t + .75) + b;
	} else if (t < (2.5/2.75)) {
		return c*(7.5625*(t-=(2.25/2.75))*t + .9375) + b;
	} else {
		return c*(7.5625*(t-=(2.625/2.75))*t + .984375) + b;
	}
}


int easingFuncCount = 6;
EasingFunc easingFuncs[] = { 
    linearTween, 
    easeInOutCubic, 
    easeInOutQuad, 
    easeInOutQuart, 
    easeInOutQuint, 
    easeOutBounce, 
};
char* easingFuncNames[] = {
    "linearTween",
    "easeInOutCubic", 
    "easeInOutQuad", 
    "easeInOutQuart", 
    "easeInOutQuint", 
    "easeOutBounce", 
};

int i = 0;

// called by servoeaser when servo hits destination
void myArrivedFunc( int currPos, int movesIndex )
{
  Serial.print("myArrivedFunc: currPos=");
  Serial.print(currPos);
  Serial.print(", movesIndex=");
  Serial.print(movesIndex);
  if( movesIndex == 0 ) { // begining of loop
    Serial.print("  switching to easing func: ");
    Serial.print( easingFuncNames[i] );
    servoEaser.setEasingFunc( easingFuncs[i] );
    i++;
    if( i == easingFuncCount ) i = 0;
  } 
  Serial.println();
}

//
void setup()
{
  Serial.begin(19200);
  Serial.println("ServoEaser3Callbacks");

  servo1.attach( servoPin );
  servoEaser.begin( servo1, servoFrameMillis);
  servoEaser.useMicroseconds(true);

  servoEaser.setArrivedFunc( myArrivedFunc );
  servoEaser.play( myServoMoves, myServoMovesCount );
}

//
void loop()
{
  servoEaser.update();
}


