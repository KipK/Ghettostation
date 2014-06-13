/*
 * Modvk5121_Manual_Config.h - User specific Custom configuration for Arduino GLCD library
 *
 * Use this file to set io pins and LCD panel parameters
 *
 *    This configuration file is for the vk5121 module manufactured by VIKAY.
 *    This module uses 2 sed1520 chips each controlling one half of the display.
 *    It also contains an onboard contrast pot along with a high voltage
 *    blue elctroluminescent backlight with onboard high voltage power supply.
 *    The backlight can be turned on/off via EL control pin.
 *
 *    This Display is very easy to hook up as it needs no additional components.
 *    The backlight can be hardwired on, off, or can even be controlled by a AVR Pin.
 *
 *    Pixels are slightly taller than they are wide (0.42mm x 0.53mm) so that things
 *    like circles may look slightly squished.
 *
 *    The datasheet can be downloaded from here:
 *    http://www.carriernetglobal.com/Graphic/VK5121.pdf
 *    
 *
 *
 *    <-------------------------------- 120 Pixels -------------------------------->
 *    +--------------------------------------+-------------------------------------+ 
 *    |                                      |                                     | ^
 *    |<------------ 60 Pixels ------------->|                                     | |
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
 *    |       | 1| 2| 3| 4| 5| 6| 7| 8| 9|10|11|12|13|14|15|16|                    |
 *    +----------------------------------------------------------------------------+
 *
 *    +-------------------------------------------+
 *    |Pin|Symbol|        Function                | Hook To
 *    +-------------------------------------------+
 *    | 1 | Vss  | Ground                         | Ground
 *    +-------------------------------------------+
 *    | 2 | Vdd  | +5v (4ma max)                  | +5v
 *    +-------------------------------------------+
 *    | 3 | DB0  | Data Bit 0                     | AVR Pin
 *    +-------------------------------------------+
 *    | 4 | DB1  | Data Bit 1                     | AVR Pin
 *    +-------------------------------------------+
 *    | 5 | DB2  | Data Bit 2                     | AVR Pin
 *    +-------------------------------------------+
 *    | 6 | DB3  | Data Bit 3                     | AVR Pin
 *    +-------------------------------------------+
 *    | 7 | DB4  | Data Bit 4                     | AVR Pin
 *    +-------------------------------------------+
 *    | 8 | DB5  | Data Bit 5                     | AVR Pin
 *    +-------------------------------------------+
 *    | 9 | DB6  | Data Bit 6                     | AVR Pin
 *    +-------------------------------------------+
 *    |10 | DB7  | Data Bit 7                     | AVR Pin
 *    +-------------------------------------------+
 *    |11 | RW   | H = Read, L = Write            | AVR Pin
 *    +-------------------------------------------+
 *    |12 | E1   | chip 0 enable strobe           | AVR Pin
 *    +-------------------------------------------+
 *    |13 | A0/DI| H = Data, L=instruction/Status | AVR Pin
 *    +-------------------------------------------+
 *    |14 | E2   | chip 1 enable strobe           | AVR Pin
 *    +-------------------------------------------+
 *    |15 | ELCNT| H=Backlight on, L=Off          | gnd=off, +5=on, AVR Pin=Control
 *    +-------------------------------------------+
 *    |16 | EL+  | Backlight +5v (50ma max)       | Direct to +5v
 *    +-------------------------------------------+
 *
 */

#ifndef GLCD_PANEL_CONFIG_H
#define GLCD_PANEL_CONFIG_H

/*
 * define name for panel configuration
 */
#define glcd_ConfigName "vk5121-Manual"

/*********************************************************/
/*  Configuration for LCD panel specific configuration   */
/*********************************************************/
#define DISPLAY_WIDTH 120
#define DISPLAY_HEIGHT 32

// panel controller chips
#define CHIP_WIDTH     60  // pixels per chip 
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
#define glcdData2Pin    10
#define glcdData3Pin    11
#define glcdData4Pin    4
#define glcdData5Pin    5
#define glcdData6Pin    6
#define glcdData7Pin    7

#define glcdRW         14

#define glcdE1         15

#define glcdDI         16    // Ao glcd pin

#define glcdE2         17


// defines for panel specific timing 
#define GLCD_tDDR    290    /* tACC6: Data Delay time (E high to valid read data)             */
#define GLCD_tAS      20    /* tAW6:  Address setup time (ctrl line changes to E high)        */
#define GLCD_tDSW    200    /* tDS6:  Data setup time (Write data lines setup to dropping E)  */
#define GLCD_tWH     450    /* tEW:   E hi level width (minimum E hi pulse width)             */ 
#define GLCD_tWL     450    /* E lo level width (minimum E lo pulse width)                    */


// calculate number of chips & round up if width is not evenly divisable
#define glcd_CHIP_COUNT (((DISPLAY_WIDTH + CHIP_WIDTH - 1)  / CHIP_WIDTH) * ((DISPLAY_HEIGHT + CHIP_HEIGHT -1) / CHIP_HEIGHT))

#include "device/sed1520_Device.h"
#endif //GLCD_PANEL_CONFIG_H
