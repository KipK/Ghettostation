/*
 * ks0108_Panel.h - User specific configuration for Arduino GLCD library
 *
 * Use this file to set LCD panel parameters
 * This version is for a standard ks0108 display
 * This file uses a board specific pin assignment file based on the board selected in the IDE
 *
*/

#ifndef GLCD_PANEL_CONFIG_H
#define GLCD_PANEL_CONFIG_H

/*
 * define name for panel configuration
 */
#define glcd_PanelConfigName "ks0108"

/*********************************************************/
/*  Configuration for LCD panel specific configuration   */
/*********************************************************/
#define DISPLAY_WIDTH 128
#define DISPLAY_HEIGHT 64

// panel controller chips
#define CHIP_WIDTH     64  // pixels per chip
#define CHIP_HEIGHT    64  // pixels per chip

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
#define GLCD_tAS    140    /* Address setup time (ctrl line changes to E HIGH   */
#define GLCD_tDSW   200    /* Data setup time (data lines setup to dropping E)   */
#define GLCD_tWH    450    /* E hi level width (minimum E hi pulse width)        */
#define GLCD_tWL    450    /* E lo level width (minimum E lo pulse width)        */


 /*
  * The code below selects a configuration file for pin assignment based on the board selected in the IDE 
  * These configurations are compatible with wiring used in earlier versions of the library
  * WARNING: When adding new board types it is not as simple as just editing these lines.
  * There is also a dependency on the file glcd/include/arduino_io.h which does the arduino pin mappings
  */
 
#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
#include "config/ks0108_Mega.h"      // config for mega 1280/2560 board
#elif defined(__AVR_ATmega644P__)  || defined(__AVR_ATmega644__)           
#include "config/ks0108_Sanguino.h"  // config for Sanguino or other ATmega644/p board
#elif defined(__AVR_AT90USB646__) || defined(__AVR_AT90USB1286__) || defined(__AVR_ATmega32U4__)// Teensy
#include "config/ks0108_Teensy.h"    // config for Teensy and Teensy++  
#else
#include "config/ks0108_Arduino.h"   // config file for standard Arduino using documented wiring 
#endif

#include "device/ks0108_Device.h"
#endif //GLCD_PANEL_CONFIG_H
