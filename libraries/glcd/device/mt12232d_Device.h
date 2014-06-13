/*
  mt12232d_Device.h - Arduino library support for graphic LCDs 

 vi:ts=4
  
*/

#ifndef GLCD_PANEL_DEVICE_H
#define GLCD_PANEL_DEVICE_H

/*
 * define name for Device
 */
#define glcd_DeviceName "mt12232d"

/*
 * Sanity check mt-12232d config info
 */

#if (CHIP_WIDTH < DISPLAY_WIDTH) && (CHIP_HEIGHT !=  DISPLAY_HEIGHT)
#error "MT12232d CHIP_WIDTH < DISPLAY_WITDH but CHIP_HEIGHT != DISPLAY_HEIGHT"
#endif

#if (CHIP_HEIGHT < DISPLAY_HEIGHT) && (CHIP_WIDTH !=  DISPLAY_WIDTH)
#error "MT12232d CHIP_HEIGHT < DISPLAY_HEIGHT but CHIP_WIDTH != DISPLAY_WIDTH"
#endif

#if (CHIP_HEIGHT == DISPLAY_HEIGHT) && (DISPLAY_WIDTH != 2 * CHIP_WIDTH)
#error "MT12232d CHIP_HEIGHT == DISPLAY_HEIGHT but DISPLAY_WIDTH != 2 * CHIP_WIDTH"
#endif

#ifndef glcdEN
#error "MT12232d configuration missing glcdEN"
#endif

#ifdef glcdE1
#error "MT12232d configuration does not use glcdE1"
#endif

#ifdef glcdE1
#error "MT12232d configuration does not use glcdE1"
#endif

#ifndef glcd_CHIP0
#error "MT12232d configuration missing glcd_CHIP0"
#endif
#ifndef glcd_CHIP1
#error "MT12232d configuration missing glcd_CHIP1"
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
#define LCD_ADC_RIGHTWARD	0xA0		
#define LCD_ADC_LEFTWARD	0xA1		
#define LCD_STATICDRIVE_OFF	0xA4	// Normal operation
#define LCD_STATICDRIVE_ON	0xA5	// static drive all segments lit (power save)
#define LCD_DUTY_16			0xA8	// 1/16 duty factor for driving LCD cells
#define LCD_DUTY_32			0xA9	// 1/32 duty factor for driving LCD cells
#define LCD_RMW				0xE0	// start RMW mode
#define LCD_RMW_END			0xEE	// end RMW mode

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
 * The MT12232d chip select
 */

#define glcd_DevSelectChip(chip) SelectChip(chip)

/*
 * MT12232d doesn't use/need a chip # to strobe EN
 * as there is only a single EN line, so ignore the chip parameter.
 * However, E is acive low so strobe is "backwards" with respect to levels.
 */

#define glcd_DevENstrobeHi(chip) lcdfastWrite(glcdEN, 0)
#define glcd_DevENstrobeLo(chip) lcdfastWrite(glcdEN, 1)


/*
 * Convert X & Y coordinate to chip values
 *
 *	For now Assume only 2 chips as that is what
 *	is currently defined for the MT12232d
 *
 *	This assumption will also keep from doing a
 *	true modulo function to detect chip crossover
 *	as the chipwidth is not a power of two which
 *	means that masking cannot be used in place
 *  of real division.
 */

#if CHIP_HEIGHT < DISPLAY_HEIGHT
#define glcd_DevXYval2Chip(x, y)	(((y) < CHIP_HEIGHT) ? 0 : 1)
#else
#define glcd_DevXYval2Chip(x, y)	(((x) < CHIP_WIDTH) ? 0 : 1)
#endif

/*
 * NOTE:
 * This Module is VERY funky!
 * The left chip is wired backwards but when corrected by using ADC=1 the
 * column must be offset by 0x13. So that is why the column is offset by 0x13 for chip 0 below.
 */

#define glcd_DevXval2ChipCol(x)		((x) < CHIP_WIDTH ? (x + 0x13) : (x - CHIP_WIDTH))

/*
 * Custom init routine
 * This module is VERY funky!
 * The left side is mapped backwards so you have to set ADC=1 for chip 0
 * Then when using ADC=1 there is a 0x13 column offset (handled by glcd_DevXval2ChipCol() above )
 */
#define glcd_DeviceInit(chip)					\
({												\
uint8_t status;									\
	do												\
	{												\
		WriteCommand(LCD_RMW_END, chip);			\
		WriteCommand(LCD_ON, chip);					\
		WriteCommand(LCD_DISP_START, chip);			\
		if(chip == 0)								\
		   WriteCommand(LCD_ADC_LEFTWARD, chip);	\
		else										\
		   WriteCommand(LCD_ADC_RIGHTWARD, chip);	\
	}while(0);										\
	status = GLCD_ENOERR;							\
	status;	/* return value of macro */				\
})


#endif //GLCD_PANEL_DEVICE_H
