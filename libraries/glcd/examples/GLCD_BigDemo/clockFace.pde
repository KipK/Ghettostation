/*
 * clockAnalog.pde
 *
 * analog clock using the GLCD library
 *
 * Sets time using Pot (analog pin 0) and pushbutton (analog pin 1, digital pin 15)
 * Pressing the button when clock is displayed moves hands forward or backwards
 * direction and speed of hands is determined by pot position 
 *
 */
 
#include <Time.h>    // download from: http://www.arduino.cc/playground/Code/Time
#include <glcd.h>    // Graphics LCD library   
#include "fonts/SystemFont5x7.h"       // system font
#include "AnalogClock.h"  // header file for the analog clock class

#include "BigDemo.h"


AnalogClock analogClock = AnalogClock();  // this creates an instance of the analog clock display. 

void clockBegin(){
#ifdef buttonPin
  digitalWrite(buttonPin, PULL_UP);  // enable internal pull-up resistor
#endif  

  setTime(4,37,0,2,1,10); // set time to 4:37 am Jan 2 2010  
}

//show clock for the given number of seconds
void  clock( int duration)
{  

  GLCD.SelectFont(System5x7);
  GLCD.ClearScreen();
  analogClock.Init(GLCD.CenterX,GLCD.CenterY,GLCD.CenterY-4);  // draw the clock face  
  unsigned long startTime = now() ;   
  unsigned long  prevtime;
  while( now() < startTime + duration )
  {
    analogClock.DisplayTime(hour(), minute(), second() ); // update analog clock  
    prevtime = now(); 
    while( prevtime == now() )
    {
      // check if set buttons pressed while waiting for second to rollover
#ifdef buttonPin      
      if(checkSetButton(buttonPin, potPin))
          startTime = now();   //  time changed so reset start time
#endif#      
    }    
  }
}


boolean checkSetButton(int button, int pot)
{
int step;
boolean ret = false;

  while(digitalRead(button)== LOW)
  {
     int value = analogRead(pot);
     value = map(value, 0,1023, 10, -10);  // change the output range sense if the pot needs reversing
     int absVal = abs(value);
     if(absVal > 0)
     {    
        for(step=1; --absVal; step*=2)
           ;
        if(value < 0)
           step = -step;   
        adjustTime(step);
        ret = true; // flag to indicate the the time has been changed
        analogClock.DisplayTime(hour(), minute(), second() ); // update analog clock          
     } 
     delay(100); 
  }   
  return ret;
}






