/*
 * Modmt12232d_Manual_Config.h - User specific Custom configuration for Arduino GLCD library
 *
 * Use this file to set io pins and LCD panel parameters
 *
 *    This configuration file is for the Russian mt-12232d module.
 *	It is similar to a sed1520DOA but instead of using dual E strobes,
 *	it uses a single E strobe (inverted) and a single chipselect line.
 *
 *	This display is funky!
 * 	The left side (chip 0) by default addresses from right to left instead of right to left.
 *	The chip can address up to 80 pixels across so when the addressing is reversed, pixel 79
 *	is now where pixel 0 would be and pixel 0 is where pixel 79 used to be.
 *	Since this display is only 61 pixels across, pixels 0 to 18 would be off the screen just
 *	like pixels 61 to 79 normally would be.
 *
 *	What all this means is that the left chip must be addressed backwards
 *	(set ADC = 1 for left chip [chip 0] ) and then you must add 0x13 to all chip addresses
 *	for the left chip (chip 0) to address the columns properly.
 *
 *    The datasheet can be downloaded from here:
 *    http://www.melt.com.ru/docs/MT-12232D.pdf
 *
 *
 *    <-------------------------------- 122 Pixels -------------------------------->
 *    +--------------------------------------+-------------------------------------+ 
 *    |                                      |                                     | ^
 *    |<------------ 61 Pixels ------------->|                                     | |
 *    |                                      |                                     | |
 *    |                                      |                                     | |
 *    |                                      |                                     | |
 *    |                                      |                                     | 32 pixels
 *    |    Chip 0 controls these pixels      |     Chip 1 Controls these pixels    | |
 *    |                                      |                                     | |
 *    |                                      |                                     | |
 *    |                                      |                                     | |
 *    |                                      |                                     | |
 *    |                                      |                                     | |
 *    |                                      |                                     | v
 *    +--------------------------------------+-------------------------------------+
 *    |            | 1| 2| 3| 4| 5| 6| 7| 8| 9|10|11|12|13|14|15|16|17|18|         |
 *    +----------------------------------------------------------------------------+
 *
 *    +-------------------------------------------+
 *    |Pin|Symbol|        Function                | Hook To
 *    +-------------------------------------------+
 *    | 1 | DB4  | Data Bit 4                     | AVR Pin
 *    +-------------------------------------------+
 *    | 2 | DB5  | Data Bit 5                     | AVR Pin
 *    +-------------------------------------------+
 *    | 3 | DB6  | Data Bit 6                     | AVR Pin
 *    +-------------------------------------------+
 *    | 4 | DB7  | Data Bit 7                     | AVR Pin
 *    +-------------------------------------------+
 *    | 5 | A0/DI| H = Data, L=instruction/Status | AVR Pin
 *    +-------------------------------------------+
 *    | 6 | RW   | H = Read, L = Write            | AVR Pin
 *    +-------------------------------------------+
 *    | 7 | E    | enable strobe (active low)     | AVR Pin
 *    +-------------------------------------------+
 *    | 8 | DB0  | Data Bit 0                     | AVR Pin
 *    +-------------------------------------------+
 *    | 9 | DB1  | Data Bit 1                     | AVR Pin
 *    +-------------------------------------------+
 *    |10 | DB2  | Data Bit 2                     | AVR Pin
 *    +-------------------------------------------+
 *    |11 | DB3  | Data Bit 3                     | AVR Pin
 *    +-------------------------------------------+
 *    |12 | Vss  | Ground                         | Ground --------------------- /
 *    +-------------------------------------------+                              \
 *    |13 | Vo   | Contrast control input         | wiper of contrast pot ------ / (10k-100k pot)
 *    +-------------------------------------------+                              \
 *    |14 | Vcc  | +5v (4ma max)                  | +5v -------------------------/
 *    +-------------------------------------------+
 *    |15 | K    | Backlight cathode (-)          | gnd
 *    +-------------------------------------------+
 *    |16 | A    | Backlight anode (+)            | +5v (no limiting resistor needed)
 *    +-------------------------------------------+
 *    |17 | RES  | Reset signal                   | Direct to +5v (or AVR Pin for s/w control)
 *    +-------------------------------------------+
 *    |18 | CS  | Chip Select H=chip0, L=chip1    | Direct to +5v
 *    +-------------------------------------------+
 *
 * Note:
 * While this shows how to hook up a proper contrast potentiometer, testing has shown that
 * the display will default to a readible display when using the display at room temperature and 
 * not using a contrast pot. (leave Vo unconnected)
 */

#ifndef GLCD_PANEL_CONFIG_H
#define GLCD_PANEL_CONFIG_H

/*
 * define name for configuration file
 */
#define glcd_ConfigName "mt2132d-Manual"

/*********************************************************/
/*  Configuration for LCD panel specific configuration   */
/*********************************************************/
#define DISPLAY_WIDTH 122
#define DISPLAY_HEIGHT 32

// panel controller chips
#define CHIP_WIDTH     61  // pixels per chip 
#define CHIP_HEIGHT    32  // pixels per chip 

/*********************************************************/
/*  Configuration for assigning LCD bits to Arduino Pins */
/*********************************************************/

/*
 * Pins can be assigned using arduino pin numbers 0-n
 * Pins can also be assigned using PIN_Pb 
 *    where P is port A-L and b is bit 0-7
 *     Example: port D pin 3 is PIN_D3
 *
 */


#define glcdData0Pin    8
#define glcdData1Pin    9
#define glcdData2Pin   10
#define glcdData3Pin   11
#define glcdData4Pin    4
#define glcdData5Pin    5
#define glcdData6Pin    6
#define glcdData7Pin    7

#define glcdRW         2
#define glcdDI         3     // Ao glcd pin 

#define glcdEN         12
#define glcdCSEL1      13


// Reset Bit  - uncomment the next line if glcd module reset is to be driven in s/w by the library
//#define glcdRES        14    // Reset Bit (14 is same as analog 0)


/*********************************************************/
/*  Chip Select pin assignments                          */
/*********************************************************/

/*
 * Two Chip panel using one select pin
 */

#define glcd_CHIP0 glcdCSEL1,HIGH
#define glcd_CHIP1 glcdCSEL1,LOW



// defines for panel specific timing 
#define GLCD_tDDR    400    // Tacc: Read Data Delay time (E active to valid read data)
#define GLCD_tAS      20    // Taw:  Address setup time (ctrl line changes to E active)
#define GLCD_tDSW    400    // Tds:  Data setup time (Write data lines setup to deactivate E)
#define GLCD_tWH    1000    // Tcyc/2: E active level width (minimum E active pulse width)
#define GLCD_tWL    1000    // Tcyc/2: E non-active level width (minimum E in-active pulse width)


// calculate number of chips & round up if width is not evenly divisable
#define glcd_CHIP_COUNT (((DISPLAY_WIDTH + CHIP_WIDTH - 1)  / CHIP_WIDTH) * ((DISPLAY_HEIGHT + CHIP_HEIGHT -1) / CHIP_HEIGHT))

#include "device/mt12232d_Device.h"
#endif //GLCD_PANEL_CONFIG_H
