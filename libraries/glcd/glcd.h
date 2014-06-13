/*
  glcd.h - Arduino library support for graphic LCDs 
  Copyright (c)2008,2009,2010 Michael Margolis and Bill Perry
   
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

  This file contains high level functions based on the previous ks0108 library.
  The functions were inspired from code written and copyright by Fabian Maximilian Thiele.
  you can obtain a copy of his original work here:
  http://www.scienceprog.com/wp-content/uploads/2007/07/glcd_ks0108.zip
 
  The glcd class impliments high level graphics routines. 
  It is derived from the glcd_Device class that impliments the protocol for sending and
  receiving data and commands to a GLCD device

*/


#ifndef	GLCD_H
#define GLCD_H

#include <inttypes.h>
#include <avr/pgmspace.h>

#include "include/gText.h" 

#define GLCD_VERSION 3 // software version of this library


/*
 * Note that while for now all these typedefs are the same they
 * may not be in the future since each is used to point to a different type of data
 */
typedef const uint8_t* Image_t; // a glcd format bitmap (includes width & height)
typedef const uint8_t* ImageXBM_t; // a "xbm" format bitmap (includes width & height)

// the first two bytes of bitmap data are the width and height
#define bitmapWidth(bitmap)  (*bitmap)  
#define bitmapHeight(bitmap)  (*(bitmap+1))  


/**
 * @class glcd
 * @brief Functions for GLCD
 */
class glcd : public gText  
{
  private:
  public:
	glcd();
	
/** @name CONTROL FUNCTIONS
 * The following control functions are available
 */
/*@{*/
	// Control functions
	int Init(uint8_t invert = NON_INVERTED);
	void SetDisplayMode(uint8_t mode); //NON_INVERTED or INVERTED,   was SetInverted(uint8_t invert);
/*@}*/
	
/** @name DRAWING FUNCTIONS
 * The following graphic functions are available
 */
/*@{*/
	// Graphic Functions
	void ClearScreen(uint8_t color = WHITE);
	void DrawVLine(uint8_t x, uint8_t y, uint8_t height, uint8_t color= BLACK);
	void DrawHLine(uint8_t x, uint8_t y, uint8_t width, uint8_t color= BLACK);
	void DrawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t color= BLACK);
	void DrawRect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t color= BLACK);
	void DrawRoundRect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t radius, uint8_t color= BLACK);
	void FillRect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t color= BLACK);
	void InvertRect(uint8_t x, uint8_t y, uint8_t width, uint8_t height);
	void DrawCircle(uint8_t xCenter, uint8_t yCenter, uint8_t radius, uint8_t color= BLACK);	
	void FillCircle(uint8_t xCenter, uint8_t yCenter, uint8_t radius, uint8_t color= BLACK);	
	void DrawBitmap(Image_t bitmap, uint8_t x, uint8_t y, uint8_t color= BLACK);
#ifdef NOTYET
	void DrawBitmapXBM(ImageXBM_t bitmapxbm, uint8_t x, uint8_t y, uint8_t color= BLACK);
	void DrawBitmapXBM_P(uint8_t width, uint8_t height, uint8_t *xbmbits, uint8_t x, uint8_t y, 
		uint8_t fg_color, uint8_t bg_color);
#endif

#ifdef DOXYGEN
	/*
	 * Define functions to get them to show up properly
	 * in doxygen
	 */
	void SetDot(uint8_t x, uint8_t y, uint8_t color);
	void SetPixels(uint8_t x, uint8_t y,uint8_t x1, uint8_t y1, uint8_t color);
	uint8_t ReadData(void);        // now public
    void WriteData(uint8_t data); 
#else
	using glcd_Device::SetDot;
	using glcd_Device::SetPixels;
	using glcd_Device::ReadData; 
	using glcd_Device::WriteData; 
#endif



	void GotoXY(uint8_t x, uint8_t y);  // overrride for GotoXY in device class


/*@}*/

	//Device Properties - these are read only constants	 
	static const uint8_t Width = DISPLAY_WIDTH; 	/**< Display width in pixels */
	static const uint8_t Height = DISPLAY_HEIGHT;	/**< Display height in pixels */
	static const uint8_t Right = DISPLAY_WIDTH-1;	/**< Right most pixel on Display (equals Width -1)*/
	static const uint8_t Bottom = DISPLAY_HEIGHT-1; /**< Bottom most pixel on Display (equals Height -1)*/
	static const uint8_t CenterX = DISPLAY_WIDTH/2;	/**< Horizontal center pixel on Display (equals Width/2)*/
	static const uint8_t CenterY = DISPLAY_HEIGHT/2;/**< Vertical center pixel on Display (equals Height/2)*/
	
};

extern glcd GLCD;   
#endif
