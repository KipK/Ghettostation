/*
 * ks0108_Sanguino.h - ATmega644 specific configuration for Arduino GLCD library
 *
 * Use this file to set io pins
 *
 * The configuration below uses a single port for data 
 * and has the same wiring as the Sanguino config from the previous ks0108 library 
 *
*/

#ifndef GLCD_PIN_CONFIG_H
#define GLCD_PIN_CONFIG_H

/*
 * define name for pin configuration
 */
#define glcd_PinConfigName "ks0108-Sanguino"

/*********************************************************/
/*  Configuration for assigning LCD bits to Arduino Pins */
/*********************************************************/
/*
 * pins used for Commands
 */

#define glcdCSEL1    24     // CS1 Bit   // swap pin assignments with CSEL2 if left/right image is reversed
#define glcdCSEL2    25     // CS2 Bit
#define glcdRW       26     // R/W Bit
#define glcdDI       27     // D/I Bit 
#define glcdEN       28     // EN Bit

#if NBR_CHIP_SELECT_PINS > 2
#define glcdCSEL3    29   // third chip select if needed
#endif

#if NBR_CHIP_SELECT_PINS > 3
#define glcdCSEL4    30   // fourth chip select if needed
#endif

/*
 * Data pin definitions
 * This version uses pins 0-7 for LCD Data 
 */
#define glcdData0Pin    0
#define glcdData1Pin    1
#define glcdData2Pin    2
#define glcdData3Pin    3
#define glcdData4Pin    4
#define glcdData5Pin    5
#define glcdData6Pin    6
#define glcdData7Pin    7


#endif //GLCD_PANEL_CONFIG_H
