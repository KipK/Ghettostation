/*
  hd44102.h - Arduino library support for graphic LCDs 

 vi:ts=4
  
*/

#ifndef GLCD_PANEL_DEVICE_H
#define GLCD_PANEL_DEVICE_H

/*
 * define name for Device
 */
#define glcd_DeviceName "hd44102"


/*
 * Place holder file - NEEDS to FIXED 
 */

/*
 * Sanity check KS0108 config pins
 */

#ifndef glcdCSEL1
#error "HD44102 configuration missing glcdCSEL1"
#endif
#ifndef glcdCSEL2
#error "HD44102 configuration missing glcdCSEL2"
#endif

#ifndef glcdEN
#error "HD44102 configuration missing glcdEN"
#endif

#ifdef glcdE1
#error "HD44102 configuration does not use glcdE1"
#endif
#ifdef glcdE2
#error "HD44102 configuration does not use glcdE2"
#endif

/*
 * LCD commands -------------------------------------------------------------
 */
#define LCD_ON				0x39
#define LCD_OFF				0x38
#define LCD_SET_ADD			0x40
#define LCD_DISP_START		0x3E   // Display start page 0
#define LCD_SET_PAGE		0xB8

/*
 * Status register bits/flags -----------------------------------------------
 */
#define LCD_BUSY_FLAG		0x80 
#define LCD_BUSY_BIT		7

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

/*
 * Convert X coordinate to chip values
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
