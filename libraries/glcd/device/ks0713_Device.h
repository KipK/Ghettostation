/*
  ks0713_Device.h - Arduino library support for graphic LCDs 

 vi:ts=4

*/

#ifndef GLCD_PANEL_DEVICE_H
#define GLCD_PANEL_DEVICE_H

/*
 * define name for Device
 */
#define glcd_DeviceName "ks0713"

/*
 * Sanity check KS0713 config pins
 *
 *	Help the user detect pin configuration errors by
 *	detecting when defines are missing or are incorrect.
 *
 *	Check for existence/non-existence of:
 *		glcdCSEL1
 *		glcdCSEL2
 *		glcdEN
 *		glcdE1
 *		glcdE2
 */

#ifdef glcdCSEL1
#error "KS0713 configuration does not use glcdCSEL1"
#endif
#ifdef glcdCSEL2
#error "KS0713 configuration does not use glcdCSEL2"
#endif

#ifndef glcdEN
#error "KS0713 configuration missing glcdEN"
#endif

#ifdef glcdE1
#error "KS0713 configuration does not use glcdE1"
#endif
#ifdef glcdE2
#error "KS0713 configuration does not use glcdE2"
#endif


/*
 * LCD commands -------------------------------------------------------------
 */
#define LCD_ON				0xAF	// Turn on LCD panel
#define LCD_OFF            	0xAE	// Turn off LCD panel
#define LCD_DISP_START		0x40	// Initial Disply Line

#define LCD_SET_PAGE		0xB0
#define LCD_SET_ADDLO     	0x00	// set column (Segment) address lo nibble
#define LCD_SET_ADDHI     	0x10	// set column (Segment) address hi nibble

#define LCD_ADC_RIGHTWWARD	0xA0		
#define LCD_ADC_LEFTWARD	0xA1		
#define LCD_REVERSELCD		0xa7	// white dots on black background.
#define LCD_NORMALLCD		0xa6	// black dots on white backbround

#define LCD_RESET			0xE2	// Reset command not signal reset

/*
 * Status register bits/flags -----------------------------------------------
 */

#define LCD_BUSY_FLAG		0x80 
#define LCD_BUSY_BIT		7

/*
 * Define primitives used by glcd_Device.cpp --------------------------------
 */


/*
 * The current ks0713 doesn't use chip selects
 * As it is currently only set up for a single chip
 */
#define glcd_DevSelectChip(chip)

/*
 * ks0713 currently doesn't use/need a chip # to strobe EN
 * as there is only a single EN line, so ignore the chip parameter.
 */
#define glcd_DevENstrobeHi(chip) lcdfastWrite(glcdEN, 1)
#define glcd_DevENstrobeLo(chip) lcdfastWrite(glcdEN, 0)


/*
 * Convert X coordinate to chip values
 */
#define glcd_DevXYval2Chip(x,y)		(0)	// no multi chip support yet.

#define glcd_DevXval2ChipCol(x)	(x)	// no multi chip support yet

/*
 * Convert from chip column value to hi/lo address value
 * for LCD commands.
 */

#define glcd_DevCol2addrlo(x)		(x & 0xf)	// lo nibble
#define glcd_DevCol2addrhi(x)		(x & 0xf0)	// hi nibble

#endif //GLCD_PANEL_DEVICE_H
