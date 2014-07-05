/*
  glcd_io.h
  Copyright (c) 2009 Bill Perry
  
  vi:ts=4

  This file is part of the Arduino GLCD library.

  GLCD is free software: you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as published by
  the Free Software Foundation, either version 2.1 of the License, or
  (at your option) any later version.

  GLCD is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public License
  along with GLCD.  If not, see <http://www.gnu.org/licenses/>.

  This file maps abstract io requests from glcd_Device to AVR port and pin abstractions
  arduino_avrio.h maps arduino pins to avr ports and pins.
  The physical io is handled by macros in avrio.h
 
 */

#ifndef	GLCD_IO_H
#define GLCD_IO_H

#if ARDUINO < 100
#include "wiring.h"
#else
#include "Arduino.h"
#endif
#include "include/arduino_io.h"    // these macros map arduino pins

/*
 * Must set AVRIO modes before including avrio
 */

#ifdef GLCD_ATOMIC_IO
#define AVRIO_NO4BIT // for now disable nibble mode
#endif

#include "include/avrio.h"         // these macros do direct port io    

 
/*
 * Map a Busy status bit to a pin.
 * In order to do this properly it takes concatenation
 * to convert the LCD status bit value to a pin
 */

#define xGLCD_STATUS_BIT2PIN(bit)    glcdData ## bit  ## Pin
#define GLCD_STATUS_BIT2PIN(bit)    xGLCD_STATUS_BIT2PIN(bit)    


#ifdef  _AVRIO_AVRIO_

// lcdfastWrite Macro may be replaced by Paul's new Arduino macro 
#define lcdfastWrite(pin, pinval) avrio_WritePin(pin, pinval)

#ifndef OUTPUT
#define OUTPUT 1
#endif

#ifndef LOW
#define LOW 0
#endif

#ifndef HIGH
#define HIGH 1
#endif

#define lcdPinMode(pin, mode)  avrio_PinMode(pin, mode) 


/*
 * Set up the configured LCD data lines 
 */

#define lcd_avrWriteByte(data) 					\
	avrio_Write8Bits(AVRIO_PORTREG,				\
			glcdData0Pin, glcdData1Pin,		\
			glcdData2Pin, glcdData3Pin,		\
			glcdData4Pin, glcdData5Pin,		\
			glcdData6Pin, glcdData7Pin, data)


/*
 * Read the configured LCD data lines and return data as 8 bit byte
 */
#define lcd_avrReadByte() 					\
	avrio_Read8Bits(AVRIO_PINREG,				\
			glcdData0Pin, glcdData1Pin,		\
			glcdData2Pin, glcdData3Pin,		\
			glcdData4Pin, glcdData5Pin,		\
			glcdData6Pin, glcdData7Pin)


/*
 * Configure the direction of the data pins.
 *	0x00 is for input and 0xff is for output.
 */
#define lcdDataDir(dirbits)					\
	avrio_Write8Bits(AVRIO_DDRREG, 				\
			glcdData0Pin, glcdData1Pin,		\
			glcdData2Pin, glcdData3Pin,		\
			glcdData4Pin, glcdData5Pin,		\
			glcdData6Pin, glcdData7Pin, dirbits)

/*
 * alias to setup LCD data lines.
 */
#define lcdDataOut(data)	lcd_avrWriteByte(data)

/*
 * alias to Read LCD data lines.
 */
#define lcdDataIn()		lcd_avrReadByte()

/*
 * alias to read status bits
 */
#define lcdRdBusystatus()		(avrio_ReadPin(GLCD_STATUS_BIT2PIN(LCD_BUSY_BIT)))
#define lcdRdResetstatus()		(avrio_ReadPin(GLCD_STATUS_BIT2PIN(LCD_RESET_BIT)))

/*
 * alias to check status bits
 */

#define lcdIsBusyStatus(status) (status & LCD_BUSY_FLAG)
#define lcdIsResetStatus(status) (status & LCD_RESET_FLAG)

#ifdef glcdRES
#define lcdReset()		avrio_WritePin(glcdRES, 0)
#define lcdUnReset()	avrio_WritePin(glcdRES, 1)
#else
#define lcdReset()		
#define lcdUnReset()		
#endif

#endif // _AVRIO_AVRIO_

/*
 * Delay functions
 */

/*
 * For nano second delays there are two real options:
 * The AVR libC supplied <util/delay.h> is not good eough,
 * there is too much error in their calculations.
 *
 * - Hans Heinrichs _delay_cycles()
 * - avr-gcc __builtin_avr_delay_cycles() routine.
 *
 * The two generate very similar code, however the gcc
 * builtin function is only avialable in newer gcc revisions
 * (some older linux avr-gcc revisions don't have it) and Hans' code
 * is sometimes smaller and doesn't use loops which require a
 * a register when the number cycles is less than 12.
 */
#include "include/delay.h" // Hans' Heirichs delay macros

#define lcdDelayNanoseconds(__ns) _delay_cycles( (double)(F_CPU)*((double)__ns)/1.0e9 + 0.5 ) // Hans Heinrichs delay cycle routine

#define lcdDelayMilliseconds(__ms) delay(__ms)	// Arduino delay function


/*
 * functions to perform chip selects on panel configurations
 * that use them.
 */

#ifdef glcd_CHIP0 // check to see if panel uses chip selects


/*
 * First some sanity checks
 */

#ifndef glcd_CHIP_COUNT
#error glcd_CHIP_COUNT not defined
#endif

/*
 * The actual functions/macros that do the chip selects.
 * each macro is passed a chipselect string defined
 * in the config file. This string has 1, 2, 3, 4, ....
 * pairs of data that define pins that need to be set to
 * a high/low value.
 * These macros rip those strings apart and generate the
 * functions to set the pin values.
 */

#if (glcd_CHIP_COUNT > 4)
#error GLCD chip count beyond max of 4
#endif

#if defined(glcdCSEL4)
#define lcdChipSelect(cselstr) lcdChipSelect4(cselstr)
#elif defined(glcdCSEL3)
#define lcdChipSelect(cselstr) lcdChipSelect3(cselstr)
#elif defined(glcdCSEL2)
#define lcdChipSelect(cselstr) lcdChipSelect2(cselstr)
#elif defined(glcdCSEL1)
#define lcdChipSelect(cselstr) lcdChipSelect1(cselstr)
#endif

#define lcdChipSelect1(p,v) lcdfastWrite(p,v)

#define lcdChipSelect2(p1,v1, p2,v2) \
 do {lcdChipSelect1(p1, v1); lcdChipSelect1(p2,v2);} while(0)

#define lcdChipSelect3(p1,v1, p2,v2, p3,v3) \
 do {lcdChipSelect1(p1, v1); lcdChipSelect1(p2,v2); lcdChipSelect1(p3,v3);} while(0)

#define lcdChipSelect4(p1,v1, p2,v2, p3,v3, p4,v4) \
 do {lcdChipSelect1(p1, v1); lcdChipSelect1(p2,v2); lcdChipSelect1(p3,v3); lcdChipSelect1(p4,v4);} while(0)

#endif // glcd_CHIP0

#endif // GLCD_IO_H
