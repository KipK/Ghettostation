/*
  xxxk_Device.h - Arduino library support for graphic LCDs 

 vi:ts=4

	This is a device template file that shows what needs to be
	in a device header for a glcd.

The following lcd primitives must be defined

	glcd_DevSelectChip(chip)
	glcd_DevENstrobeHi(chip)
	glcd_DevENstrobeLo(chip)
	glcd_DevXval2Chip(x)
	glcd_DevXval2ChipCol(x)

	For devices that split set column into to 2 commands of hi/lo: 

	glcd_DevCol2addrlo(x)		(x & 0xf)	// lo nibble
	glcd_DevCol2addrhi(x)		(x & 0xf0)	// hi nibble
  
*/

#ifndef GLCD_PANEL_DEVICE_H
#define GLCD_PANEL_DEVICE_H

/*
 * define name for Device
 */
#define glcd_DeviceName "xxx"

/*
 * Sanity check XXX config pins
 *
 *	Help the user detect lipin configuration errors by
 *	detecting when defines are missing or are incorrect.
 *
 *	Check for existence/non-existence of:
 *		glcdCSEL1
 *		glcdCSEL2
 *		glcdEN
 *		glcdE1
 *		glcdE2
 */

#ifndef glcdCSEL1
#error "XXXX configuration missing glcdCSEL1"
#endif
#ifndef glcdCSEL2
#error "XXXX configuration missing glcdCSEL2"
#endif

#ifdef glcdEN
#error "XXXX configuration missing glcdEN"
#endif

#ifdef glcdE1
#error "XXXX configuration does not use glcdE1"
#endif
#ifdef glcdE2
#error "XXXX configuration does not use glcdE2"
#endif


/*
 * LCD commands -------------------------------------------------------------
 */

#define LCD_ON
#define LCD_OFF
#define LCD_SET_ADD
#define LCD_DISP_START
#define LCD_SET_PAGE


/*
 * Status register bits/flags -----------------------------------------------
 */

#define LCD_BUSY_FLAG
#define LCD_BUSY_BIT



/*
 * Define primitives used by glcd_Device.cpp --------------------------------
 */

#define glcd_DevSelectChip(chip)

/*
 * Define how to strob EN signals
 */
#define glcd_DevENstrobeHi(chip)
#define glcd_DevENstrobeLo(chip)

/*
 * Define how to get low and how addresses
 * (if needed)
 */

 * Convert X & Y coordinates to chip values
 */
#define	glcd_DevXYval2Chip(x,y)
#define	glcd_DevXval2ChipCol(x)

/*
 * Convert from chip column value to hi/lo address value
 * for LCD commands.
 *	(If Needed)
 */

#define glcd_DevCol2addrlo(x)
#define glcd_DevCol2addrhi(x)

#endif //GLCD_PANEL_DEVICE_H
