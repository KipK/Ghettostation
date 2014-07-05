/*
  gText.h - Support for Text output on a graphical device
  Copyright (c) 2009,2010  Bill Perry and Michael Margolis

  vi:ts=4

  This file is part of the Arduino GLCD library.

  GLCD is free software: you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as published by
  the Free Software Foundation, either version 2.1 of the License, or
  (at your option) any later version.

  GLCD is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public License
  along with GLCD.  If not, see <http://www.gnu.org/licenses/>.
  
*/

#ifndef	GTEXT_H
#define GTEXT_H

#include <inttypes.h>
#include <avr/pgmspace.h>

#include "WString.h"
#include "include/Streaming.h" 
#include "include/glcd_Device.h"


#define GTEXT_VERSION 1 // software version of this code

#ifndef USE_ARDUINO_FLASHSTR
// these should be replaced when Arduino supports FLASH strings in the base print class
#include <avr/pgmspace.h>
typedef class _FlashString {} *FLASHSTRING;
#define flashStr(x) ((FLASHSTRING)(PSTR((x))))
#endif

// Font Indices
#define FONT_LENGTH			0
#define FONT_FIXED_WIDTH	2
#define FONT_HEIGHT			3
#define FONT_FIRST_CHAR		4
#define FONT_CHAR_COUNT		5
#define FONT_WIDTH_TABLE	6

// the following returns true if the given font is fixed width
// zero length is flag indicating fixed width font (array does not contain width data entries)
#define isFixedWidthFont(font)  (FontRead(font+FONT_LENGTH) == 0 && FontRead(font+FONT_LENGTH+1) == 0))

/*
 * Coodinates for predefined areas are compressed into a single 32 bit token.
 *
 * This works as the coordinates are cuurenly only 8 bit values.
 *
 * This allows the creatation of an unlmited number of predefined areas with zero code or
 * data space overhead.
 * 
 * A macro is used to create the tokens from a set of x1,y1 x2,y2 coordinates.
 *
 * A union is used to extract the coordinates from the 32 bit token.
 *
 * WARNING:
 *	This is non portable code in that it will only work on little endian processors.
 *	If you use big endian you have to switch the byte ordering in the union.
 */

#define MK_TareaToken(x1, y1, x2, y2) \
	(uint32_t) (((uint32_t) (x1) << 24) | ((uint32_t)(y1) << 16) | ((uint32_t)(x2) << 8) | (uint32_t)(y2))
        
/// @cond hide_from_doxygen
typedef union
{       
 struct
 {      
        uint8_t y2;
        uint8_t x2;
        uint8_t y1;
        uint8_t x1;
 }coord;
        
 uint32_t token; // swap byte order above for big endian
        
}TareaToken;
/// @endcond

typedef uint8_t textMode;  // type holding mode for scrolling and future attributes like padding etc.
// the only textMode supported in the current release is scrolling

const textMode SCROLL_UP = 0;
const textMode SCROLL_DOWN = 1; // this was changed from -1 so it can used in a bitmask 
const textMode DEFAULT_SCROLLDIR = SCROLL_UP;

/**
 * @defgroup glcd_enum GLCD enumerations
 */

/**
 * @ingroup glcd_enum
 * @hideinitializer
 * @brief Pre Defined Text areas
 *
 * These enumerations are used to easily define text areas 
 * using predefined display areas.\n
 * They are used with the
 * \ref gText::DefineArea(predefinedArea selection, textMode mode) "DefineArea()" function call.
 */
typedef enum  {

	textAreaFULL         = MK_TareaToken( 0,               0,                DISPLAY_WIDTH -1,   DISPLAY_HEIGHT -1   ),
	/**<Entire GLCD display */

	textAreaTOP          = MK_TareaToken( 0,               0,                DISPLAY_WIDTH -1,   DISPLAY_HEIGHT/2 -1 ),
	/**<Top half of GLCD display */

	textAreaBOTTOM       = MK_TareaToken( 0,               DISPLAY_HEIGHT/2, DISPLAY_WIDTH -1,   DISPLAY_HEIGHT -1   ),
	/**<Bottom half of GLCD display */

	textAreaLEFT         = MK_TareaToken( 0,               0,                DISPLAY_WIDTH/2 -1, DISPLAY_HEIGHT -1   ),
	/**<Left side of GLCD display */

	textAreaRIGHT        = MK_TareaToken( DISPLAY_WIDTH/2, 0,                DISPLAY_WIDTH -1,   DISPLAY_HEIGHT -1   ),
	/**<Right side of GLCD display */

	textAreaTOPLEFT      = MK_TareaToken( 0,               0,                DISPLAY_WIDTH/2 -1, DISPLAY_HEIGHT/2 -1 ),
	/**<Upper left quarter of GLCD display */

	textAreaTOPRIGHT     = MK_TareaToken( DISPLAY_WIDTH/2, 0,                DISPLAY_WIDTH -1,   DISPLAY_HEIGHT/2 -1 ),
	/**<Upper right quarter of GLCD display */

	textAreaBOTTOMLEFT   = MK_TareaToken( 0,               DISPLAY_HEIGHT/2, DISPLAY_WIDTH/2 -1, DISPLAY_HEIGHT -1   ),
	/**<Buttom left quarter of GLCD display */

	textAreaBOTTOMRIGHT  = MK_TareaToken( DISPLAY_WIDTH/2, DISPLAY_HEIGHT/2, DISPLAY_WIDTH -1,   DISPLAY_HEIGHT -1   )
	/**<Bottom right quarter of GLCD display */

} predefinedArea;



/*
  * enums for ansi style erase function
  * These values match the ANSI EraseInLine terminal primitive: CSI n K
*/  
/**
 * @ingroup glcd_enum
 * @hideinitializer
 * @brief Erase line options
 * @details
 * These enumerations are used with the
 * \ref gText::EraseTextLine( eraseLine_t type) "EraseTextLine()" function call.\n
 * They provide similar functionality to the
 * ANSI EraseInLine terminal primitive: CSI n K
 *
 */
enum eraseLine_t {
	eraseTO_EOL, 	/**< Erase From cursor to end of Line */
	eraseFROM_BOL,	/**< Erase From Begining of Line to Cursor*/
	eraseFULL_LINE	/**< Erase Entire line */
	};

typedef const uint8_t* Font_t;  	
typedef uint8_t (*FontCallback)(Font_t);

uint8_t ReadPgmData(const uint8_t* ptr);	//Standard Read Callback
static FontCallback	FontRead;               // font callback shared across all instances
//static glcd_Device    *device;              // static pointer to the device instance

/// @cond hide_from_doxygen
struct tarea
{
	uint8_t x1;
	uint8_t y1;
	uint8_t	x2;
	uint8_t y2;
	int8_t  mode;
};
/// @endcond

/**
 * @class gText
 * @brief Functions for Text Areas
 * @details
 * A text area acts like a terminal monitor and text output is displayed 
 * within the confines of a rectangle given in the DefineArea command.
 * All of the following text area functions operate on a user defined text area.
 */
  
 // graphical device text routines
class gText : public glcd_Device
{
  private:
    //FontCallback	FontRead;     // now static, move back here if each instance needs its own callback
	uint8_t			FontColor;
	Font_t			Font;
	struct tarea tarea;
	uint8_t			x;
	uint8_t			y;
#ifndef GLCD_NODEFER_SCROLL
	uint8_t			need_scroll; // set when text scroll has been defered
#endif

	void SpecialChar(uint8_t c);

	// Scroll routines are private for now
	void ScrollUp(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t pixels, uint8_t color);
	void ScrollDown(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t pixels, uint8_t color);

  public:
	gText(); // default - uses the entire display
	gText(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, textMode mode=DEFAULT_SCROLLDIR);
	// 4 Feb - added two constuctors (and SetFontColor below) 
	gText(predefinedArea selection, textMode mode=DEFAULT_SCROLLDIR);
	gText(uint8_t x1, uint8_t y1, uint8_t columns, uint8_t rows, Font_t font, textMode mode=DEFAULT_SCROLLDIR);

	//void Init(glcd_Device* _device); // no longer used

/** @name TEXT FUNCTIONS
 * The following text functions are available
 */
/*@{*/
	// Text area functions

	uint8_t DefineArea(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, textMode mode=DEFAULT_SCROLLDIR);
	uint8_t DefineArea(uint8_t x1, uint8_t y1, uint8_t columns, uint8_t rows, Font_t font, textMode mode=DEFAULT_SCROLLDIR);
	uint8_t DefineArea(predefinedArea selection, textMode mode=DEFAULT_SCROLLDIR);
	void SetTextMode(textMode mode); // change to the given text mode
	void ClearArea(void);

	// Font Functions
	void SelectFont(Font_t font, uint8_t color=BLACK, FontCallback callback=ReadPgmData); // default arguments added, callback now last arg
	void SetFontColor(uint8_t color); // new method
	int PutChar(uint8_t c);
	void Puts(char *str);
	void Puts(const String &str); // for Arduino String Class
	void Puts_P(PGM_P str);
	void DrawString(char *str, uint8_t x, uint8_t y);
	void DrawString(String &str, uint8_t x, uint8_t y); // for Arduino String class
	void DrawString_P(PGM_P str, uint8_t x, uint8_t y);

#if ARDUINO < 100
	void write(uint8_t c);  // character output for print base class
#else
	size_t write(uint8_t c);  // character output for print base class
#endif

	void CursorTo( uint8_t column, uint8_t row); // 0 based coordinates for character columns and rows
	void CursorTo( int8_t column); // move cursor on the current row
	void CursorToXY( uint8_t x, uint8_t y); // coordinates relative to active text area
	uint8_t CharWidth(uint8_t c);
	uint16_t StringWidth(const char* str);
	uint16_t StringWidth_P(PGM_P str);
	uint16_t StringWidth_P(String &str);

	void EraseTextLine( eraseLine_t type=eraseTO_EOL); //ansi like line erase function 
	void EraseTextLine( uint8_t row); // erase the entire text line in the given row and move cursor to left position

    // legacy text output functions 
	void PrintNumber(long n);

#ifndef USE_ARDUINO_FLASHSTR	
	// when the following function is supported in arduino it will be removed from this library
	void printFlash(FLASHSTRING str); //this overrides the Arduino print function to implicilty store the string in flash (progmem)
    void printFlashln(FLASHSTRING str);
#endif
	
#ifndef GLCD_NO_PRINTF
	void Printf(const char *format, ...);
	void Printf_P(const char *format, ...);
#endif
/*@}*/

};

#endif
