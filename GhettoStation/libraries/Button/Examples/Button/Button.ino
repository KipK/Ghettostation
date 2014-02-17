/*
||
|| @file Button.pde
|| @version 1.1
|| @author Alexander Brevig
|| @contact alexanderbrevig@gmail.com
||
|| @description
|| | Display the intuitive way of using a button when using this Hardware Abstraction class
|| #
||
|| @license
|| | Copyright (c) 2009 Alexander Brevig. All rights reserved.
|| | This code is subject to AlphaLicence.txt
|| | alphabeta.alexanderbrevig.com/AlphaLicense.txt
|| #
||
*/

#include <Button.h>

//create a Button object at pin 12
/*
|| Wiring:
|| GND -----/ ------ pin 12
*/
Button button = Button(12,PULLUP);

void setup(){
  pinMode(13,OUTPUT); //debug to led 13
}

void loop(){
  if(button.isPressed()){
	digitalWrite(13,HIGH);
  }else{
	digitalWrite(13,LOW);
  }
}
