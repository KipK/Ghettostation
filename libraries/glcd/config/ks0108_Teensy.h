/*
 * ks0108_Teensy.h - User specific configuration for Arduino GLCD library
 *
 * Use this file to set io pins
 * This version is for a standard ks0108 display
 * connected to a Teensy or Teensy++
 *
 * Note that the Teensy and Teensy++ use different pin numbers 
 * so make sure that your wiring matches the device you are using
 *
*/

#ifndef GLCD_PIN_CONFIG_H
#define GLCD_PIN_CONFIG_H

/**********************************************************/
/*  Configuration for assigning LCD bits to Teensy++ Pins */
/**********************************************************/

/*
 * Pins can be assigned using Arduino pin numbers 0-n
 * Pins can also be assigned using PIN_Pb 
 *    where P is port A-L and b is bit 0-7
 *    Example: port D bit 3 is PIN_D3
 *
 */

 
#if defined(__AVR_AT90USB646__) || defined(__AVR_AT90USB1286__) // Teensy++

#define glcd_PinConfigName "ks0108-Teensy++"   // define name for configuration
/*
 *                Arduino pin    AVR pin
 */ 
#define glcdCSEL1       18      // PIN_E6 
#define glcdCSEL2       19      // PIN_E7
#define glcdRW          8       // PIN_E0
#define glcdDI          9       // PIN_E1
#define glcdEN          7       // PIN_D7
//#define glcdRES         5       // PIN_D5

#if NBR_CHIP_SELECT_PINS > 2
#error Missing glcdCSEL3 define
//#define glcdCSEL3    ?   // third chip select if needed
#endif

#if NBR_CHIP_SELECT_PINS > 3
#error Missing glcdCSEL4 define
//#define glcdCSEL4    ?   // fourth chip select if needed
#endif


#define glcdData0Pin    10      // PIN_C0
#define glcdData1Pin    11      // PIN_C1
#define glcdData2Pin    12      // PIN_C2
#define glcdData3Pin    13      // PIN_C3
#define glcdData4Pin    14      // PIN_C4
#define glcdData5Pin    15      // PIN_C5
#define glcdData6Pin    16      // PIN_C6
#define glcdData7Pin    17      // PIN_C7

#warning "KS0108 using pins for teensy++"

/*********************************************************/
/*  Configuration for assigning LCD bits to Teensy Pins  */
/*********************************************************/
#elif defined(__AVR_ATmega32U4__)   // Teensy 2.0

#define glcd_PinConfigName "ks0108-Teensy"   // define name for configuration
/*
 *                         Arduino pin    AVR pin
 */ 
#define glcdCSEL1       7        // PIN_D2    // normal connection for control signals
#define glcdCSEL2       8        // PIN_D3
#define glcdRW          6        // PIN_D1
#define glcdDI          5        // PIN_D0
#define glcdEN          9        // PIN_C6
//#define glcdRES         17        //PIN_F6    // Reset Bit 

#if NBR_CHIP_SELECT_PINS > 2
#error Missing glcdCSEL3 define
//#define glcdCSEL3    ?   // third chip select if needed
#endif

#if NBR_CHIP_SELECT_PINS > 3
#error Missing glcdCSEL4 define
//#define glcdCSEL4    ?   // fourth chip select if needed
#endif

#define glcdData0Pin    0        // PIN_B0
#define glcdData1Pin    1        // PIN_B1
#define glcdData2Pin    2        // PIN_B2
#define glcdData3Pin    3        // PIN_B3
#define glcdData4Pin    13       // PIN_B4
#define glcdData5Pin    14       // PIN_B5
#define glcdData6Pin    15       // PIN_B6
#define glcdData7Pin    4        // PIN_B7

#warning "KS0108 using pins for teensy 2.0"
#endif

#endif //GLCD_PIN_CONFIG_H
