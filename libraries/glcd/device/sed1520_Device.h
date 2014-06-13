/*
  sed1520_Device.h - Arduino library support for graphic LCDs 

 vi:ts=4
  
*/

#ifndef GLCD_PANEL_DEVICE_H
#define GLCD_PANEL_DEVICE_H

/*
 * define name for Device
 */
#define glcd_DeviceName "sed1520"

/*
 * Sanity check SED1520 config info
 */

#if (CHIP_WIDTH < DISPLAY_WIDTH) && (CHIP_HEIGHT !=  DISPLAY_HEIGHT)
#error "SED1520 CHIP_WIDTH < DISPLAY_WITDH but CHIP_HEIGHT != DISPLAY_HEIGHT"
#endif

#if (CHIP_HEIGHT < DISPLAY_HEIGHT) && (CHIP_WIDTH !=  DISPLAY_WIDTH)
#error "SED1520 CHIP_HEIGHT < DISPLAY_HEIGHT but CHIP_WIDTH != DISPLAY_WIDTH"
#endif

#if (CHIP_HEIGHT == DISPLAY_HEIGHT) && (DISPLAY_WIDTH != 2 * CHIP_WIDTH)
#error "SED1520 CHIP_HEIGHT == DISPLAY_HEIGHT but DISPLAY_WIDTH != 2 * CHIP_WIDTH"
#endif

#ifndef glcdE1
#error "SED1520 configuration missing glcdE1"
#endif
#ifndef glcdE2
#error "SED1520 configuration missing glcdE2"
#endif
#ifdef glcdEN
#error "SED1520 configuration does not use glcdEN"
#endif

#ifdef glcd_CHIP0
#error "SED1520 configuration does not use glcd_CHIP0"
#endif

#ifdef glcd_CHIP2
#error "SED1520 configuration does not use glcd_CHIP0"
#endif

/*
 * LCD commands -------------------------------------------------------------
 */

#define LCD_ON				0xAF
#define LCD_OFF            	0xAE
#define LCD_DISP_START		0xC0	// set display START line
#define LCD_SET_ADD        	0x00	// set column (Segment) address
#define LCD_SET_PAGE		0xB8
#define LCD_RESET			0xE2	// Reset is a command not a signal
#define LCD_ADC_RIGHTWWARD	0xA0		
#define LCD_ADC_LEFTWARD	0xA1		
#define LCD_STATICDRIVE_OFF	0xA4	// Normal operation
#define LCD_STATICDRIVE_ON	0xA5	// static drive all segments lit (power save)
#define LCD_DUTY_16			0xA8	// 1/16 duty factor for driving LCD cells
#define LCD_DUTY_32			0xA9	// 1/32 duty factor for driving LCD cells

/*
 * Status register bits/flags -----------------------------------------------
 */

#define LCD_BUSY_FLAG		0x80 
#define LCD_BUSY_BIT		7
#define LCD_ADC_FLAG		0x40	// ADC status 1 = nomral /forward 0=leftward/reverse
#define LCD_ADC_BIT			6		// ADC status 1 = nomral /forward 0=leftward/reverse

#define LCD_RESET_BIT		4
#define LCD_RESET_FLAG		0x10

/*
 * Define primitives used by glcd_Device.cpp --------------------------------
 */

/*
 * The SED1520/VK5121 doesn't use chip selects
 */
#define glcd_DevSelectChip(chip)

/*
 * SED1520/VK5121 has seperate chipselect for each chip
 *	For now Assume only 2 chips as that is what
 *	is currently defined for the VK5121
 *
 *	This assumption will also keep from doing a
 *	true modulo function to detect chip crossover
 *	as the chipwidth is not a power of two which
 *	means that masking cannot be used in place
 *  of real division.
 */
#define glcd_DevENstrobeHi(chip) 	\
do									\
{									\
	if(chip == 0)					\
       lcdfastWrite(glcdE1, HIGH);	\
	else							\
       lcdfastWrite(glcdE2, HIGH);	\
}while(0)

#define glcd_DevENstrobeLo(chip) 	\
do									\
{									\
	if(chip == 0)					\
	   lcdfastWrite(glcdE1, LOW);	\
	else							\ 
	   lcdfastWrite(glcdE2, LOW);	\
}while(0)

/*
 * Convert X & Y coordinate to chip values
 */

#if CHIP_HEIGHT < DISPLAY_HEIGHT
#define glcd_DevXYval2Chip(x, y)	(((y) < CHIP_HEIGHT) ? 0 : 1)
#else
#define glcd_DevXYval2Chip(x, y)	(((x) < CHIP_WIDTH) ? 0 : 1)
#endif

#define glcd_DevXval2ChipCol(x)		((x) < CHIP_WIDTH ? x : (x - CHIP_WIDTH))

#endif //GLCD_PANEL_DEVICE_H
