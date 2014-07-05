/*
 * ks0108_Manual_Config.h - User specific configuration for Arduino GLCD library
 *
 * Use this file to set io pins and LCD panel parameters
 * This version is for a standard ks0108 display
 * connected using the default Arduino wiring
 *
*/

#ifndef GLCD_PANEL_CONFIG_H
#define GLCD_PANEL_CONFIG_H

/*
 * define name for panel configuration
 */
#define glcd_PanelConfigName "ks0108-Manual"

/*********************************************************/
/*  Configuration for LCD panel specific configuration   */
/*********************************************************/
#define DISPLAY_WIDTH 128
#define DISPLAY_HEIGHT 64

// panel controller chips
#define CHIP_WIDTH     64  // pixels per chip
#define CHIP_HEIGHT    64  // pixels per chip

/*********************************************************/
/*  Configuration for assigning LCD bits to Arduino Pins */
/*********************************************************/

/*
 * define name for pin configuration
 */
#define glcd_PinConfigName "ks0108-Manual"

/*
 * Pins can be assigned using Arduino pin numbers 0-n
 * Pins can also be assigned using PIN_Pb
 *   where P is port A-L and b is bit 0-7
 *   Example: port D pin 3 is PIN_D3
 *
 */


/* Data pin definitions
 */
#define glcdData0Pin        8
#define glcdData1Pin        9
#define glcdData2Pin        10
#define glcdData3Pin        11
#define glcdData4Pin        4
#define glcdData5Pin        5
#define glcdData6Pin        6
#define glcdData7Pin        7

/* Arduino pins used for Commands
 * default assignment uses the first five analog pins
 */
#define glcdCSEL1        14
#define glcdCSEL2        15
// uncomment the following if more than two chip select pins are needed
//#define glcdCSEL3        3   // third chip select line on pin 3 if needed
//#define glcdCSEL4        2   // fourth chip select if needed

#define glcdRW            16
#define glcdDI            17
#define glcdEN            18
// Reset Bit  - uncomment the next line if reset is connected to an output pin
//#define glcdRES            19    // Reset Bit

/*
 * the following is the calculation of the number of chips - do not change
 */
#define glcd_CHIP_COUNT (((DISPLAY_WIDTH + CHIP_WIDTH - 1)  / CHIP_WIDTH) * ((DISPLAY_HEIGHT + CHIP_HEIGHT -1) / CHIP_HEIGHT))

/*********************************************************/
/*  Chip Select Configuration                            */
/*********************************************************/

/*
 * Change the following define to match the number of Chip Select pins for this panel
 * Most panels use two pins for chip select,
 * but check your datasheet to see if a different number is required
 */
#define NBR_CHIP_SELECT_PINS   2 // the number of chip select pins required for this panel 

/*
 * The following conditional statements determine the relationship between the chip select
 * pins and the physical chips.
 * If the chips are displayed in the wrong order, you can swap the glcd_CHIPx defines 
 */  

/* 
 * Defines for Panels using two Chip Select pins
 */  
#if  NBR_CHIP_SELECT_PINS == 2

/*
 * Two Chip panels using two select pins (the most common panel type)
 */
#if glcd_CHIP_COUNT == 2
#define glcd_CHIP0 glcdCSEL1,HIGH,   glcdCSEL2,LOW
#define glcd_CHIP1 glcdCSEL1,LOW,    glcdCSEL2,HIGH    

/*
 * Three Chip panel using two select pins
 */
#elif  glcd_CHIP_COUNT == 3 

#define glcd_CHIP0  glcdCSEL1,LOW,  glcdCSEL2,LOW
#define glcd_CHIP1  glcdCSEL1,LOW,  glcdCSEL2,HIGH
#define glcd_CHIP2  glcdCSEL1,HIGH, glcdCSEL2,LOW

/*
 * Four Chip panel using two select pins
 */
#elif  glcd_CHIP_COUNT == 4 
#define glcd_CHIP0  glcdCSEL1,LOW,  glcdCSEL2,LOW
#define glcd_CHIP1  glcdCSEL1,HIGH, glcdCSEL2,LOW
#define glcd_CHIP2  glcdCSEL1,HIGH, glcdCSEL2,HIGH    
#define glcd_CHIP3  glcdCSEL1,LOW,  glcdCSEL2,HIGH    
#endif

/*
 * Defines for Two Chip panels using one Chip Select pin 
 */
#elif  (NBR_CHIP_SELECT_PINS == 1 && glcd_CHIP_COUNT == 2)  
#define glcd_CHIP0  glcdCSEL1,LOW
#define glcd_CHIP1  glcdCSEL1,HIGH    

/*
 * Defines for Three Chip panels using three select pins
 */
#elif (NBR_CHIP_SELECT_PINS == 3 && glcd_CHIP_COUNT == 3)  
#define glcd_CHIP0  glcdCSEL1,HIGH, glcdCSEL2,LOW,  glcdCSEL3,LOW
#define glcd_CHIP1  glcdCSEL1,LOW,  glcdCSEL2,HIGH, glcdCSEL3,LOW
#define glcd_CHIP2  glcdCSEL1,LOW,  glcdCSEL2,LOW,  glcdCSEL3,HIGH    

/*
 * Defines for Four Chip panel using four select pins
 */
#elif  (NBR_CHIP_SELECT_PINS == 4 && glcd_CHIP_COUNT == 4) 
#define glcd_CHIP0  glcdCSEL1,HIGH, glcdCSEL2,LOW,  glcdCSEL3,LOW,  glcdCSEL4,LOW
#define glcd_CHIP1  glcdCSEL1,LOW,  glcdCSEL2,HIGH, glcdCSEL3,LOW,  glcdCSEL4,LOW
#define glcd_CHIP2  glcdCSEL1,LOW,  glcdCSEL2,LOW,  glcdCSEL3,HIGH, glcdCSEL4,LOW
#define glcd_CHIP3  glcdCSEL1,LOW,  glcdCSEL2,LOW,  glcdCSEL3,LOW,  glcdCSEL4,HIGH    

/*
 * Here if the Number of Chip Selects is not supported for the selected panel size and chip size
 */
#else
#error "The number of Chips and Chip Select pins does not match an option in ks0108_Panel.h"
#error "Check that the number of Chip Select pins is correct for the configured panel size"
#endif

/*********************************************************/
/*  End of Chip Select Configuration                     */
/*********************************************************/

/*
 * The following defines are for panel specific low level timing.
 *
 * See your data sheet for the exact timing and waveforms.
 * All defines below are in nanoseconds.
 */

#define GLCD_tDDR   320    /* Data Delay time (E high to valid read data)        */
#define GLCD_tAS    140    /* Address setup time (ctrl line changes to E high)   */
#define GLCD_tDSW   200    /* Data setup time (data lines setup to dropping E)   */
#define GLCD_tWH    450    /* E hi level width (minimum E hi pulse width)        */
#define GLCD_tWL    450    /* E lo level width (minimum E lo pulse width)        */

#include "device/ks0108_Device.h"
#endif //GLCD_PANEL_CONFIG_H
