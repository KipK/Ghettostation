/*
  ReadKeypad.ino - Arduino library for I2C LCD03 display from Robot Electronics
  see http://www.robot-electronics.co.uk/htm/Lcd03tech.htm
  Copyright (c) 2013 Ben Arblaster.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/
#include <Wire.h>
#include <LCD03.h>

// Create new LCD03 instance
LCD03 lcd;

uint16_t keystate;

void setup() {
  // Initialise a 20x4 LCD
  lcd.begin(20, 4);
}

// Check they keypad state once a second and print to the LCD
void loop() {
  // Clear the display and move the cursor to the top-left
  lcd.clear();
  
  // Read the keypad state
  keystate = lcd.readKeypad();
  
  // Check if key 1 is pressed
  if(keystate & KEYPAD_1 == KEYPAD_1) {
    lcd.print("Key 1 is pressed!");
  }
   
  // Check if key 1 is pressed exclusively
  if(keystate == KEYPAD_1) {
   lcd.print("Only key 1 is pressed!");
  }
   
  // Check if keys 1 and * are pressed simultaneously
  if(keystate & (KEYPAD_1 | KEYPAD_STAR) == (KEYPAD_1 | KEYPAD_2)) {
    lcd.print("Keys 1 and * are pressed!");
  }
   
  // Check if keys 1 and * are pressed simultaneously exclusively
  if(keystate == (KEYPAD_1 | KEYPAD_STAR)) {
    lcd.print("Only keys 1 and * are pressed!");
  }

  // Wait for 1 second
  delay(1000);
}
