/*
  gText.cpp - Support for Text output on a graphical device 
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

#include <avr/pgmspace.h>
#include "include/gText.h"
#include "glcd_Config.h" 

#ifndef GLCD_NO_PRINTF
extern "C"
{
#include <stdio.h>
}
#endif

/*
 * Experimental defines
 */

//#define GLCD_OLD_FONTDRAW    // uncomment this define to get old font rendering (not recommended)

	
//extern glcd_Device GLCD; // this is the global GLCD instance, here upcast to the base glcd_Device class 

// This constructor creates a text area using the entire display
// The device pointer is initialized using the global GLCD instance
// New constuctors can be added to take an exlicit glcd instance pointer
// if multiple glcd instances need to be supported
gText::gText()
{
   // device = (glcd_Device*)&GLCD; 
    this->DefineArea(0,0,DISPLAY_WIDTH -1,DISPLAY_HEIGHT -1, DEFAULT_SCROLLDIR); // this should never fail
}

// This constructor creates a text area with the given coordinates
// full display area is used if any coordinate is invalid
gText::gText(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, textMode mode) 
{
   //device = (glcd_Device*)&GLCD; 
   if( ! this->DefineArea(x1,y1,x2,y2,mode))
       this->DefineArea(0,0,DISPLAY_WIDTH -1,DISPLAY_HEIGHT -1,mode); // this should never fail
}

gText::gText(predefinedArea selection, textMode mode)
{
   //device = (glcd_Device*)&GLCD; 
   if( ! this->DefineArea(selection,mode))
       this->DefineArea(0,0,DISPLAY_WIDTH -1,DISPLAY_HEIGHT -1,mode); // this should never fail

}

gText::gText(uint8_t x1, uint8_t y1, uint8_t columns, uint8_t rows, Font_t font, textMode mode)
{
   //device = (glcd_Device*)&GLCD; 
   if( ! this->DefineArea(x1,y1,columns,rows,font, mode))
   {
       this->DefineArea(0,0,DISPLAY_WIDTH -1,DISPLAY_HEIGHT -1,mode); // this should never fail
	  this->SelectFont(font);
   }
}

/**
 * Clear text area with the current font background color
 * and home the cursor to upper left corner of the text area.
 *
 * @see DefineArea()
 */
void gText::ClearArea(void)
{
	/*
	 * fill the area with font background color
	 */

	glcd_Device::SetPixels(this->tarea.x1, this->tarea.y1, 
		this->tarea.x2, this->tarea.y2, 
			this->FontColor == BLACK ? WHITE : BLACK);
	/*
	 * put cursor at home position of text area to ensure we are always inside area.
	 */

	 this->CursorToXY(0,0);
}

/**
 * Define a Text area by columns and rows
 *
 * @param x X coordinate of upper left corner
 * @param y Y coordinate of upper left corner
 * @param columns number of text columns
 * @param rows number of text rows
 * @param font a font definition
 * @param mode constants SCROLL_DOWN and SCROLL_UP control scroll direction
 *
 *
 * Defines a text area sized to hold columns characters across and rows characters tall.
 * It is properly sized for the specified font.
 * 
 * The area within the newly defined text area is intentionally not cleared.
 *
 * While intended for fixed width fonts, sizing will work for variable
 * width fonts.
 *
 * When variable width fonts are used, the column is based on assuming a width
 * of the widest character.
 *
 * x,y is an absolute coordinate and is relateive to the 0,0 origin of the
 * display.
 *
 * mode is an optional parameter and defaults to normal/up scrolling
 *
 * @note
 * Upon defining the text area, the cursor position for the text area will be set to x,y
 *
 * @see ClearArea()
 */

uint8_t
gText::DefineArea(uint8_t x, uint8_t y, uint8_t columns, uint8_t rows, Font_t font, textMode mode)
{
uint8_t x2,y2;

	this->SelectFont(font);

	x2 = x + columns * (FontRead(this->Font+FONT_FIXED_WIDTH)+1) -1;
	y2 = y + rows * (FontRead(this->Font+FONT_HEIGHT)+1) -1;

	return this->DefineArea(x, y, x2, y2, mode);
}

/**
 * Define a text area by absolute coordinates
 *
 * @param x1 X coordinate of upper left corner
 * @param y1 Y coordinate of upper left corner
 * @param x2 X coordinate of lower right corner
 * @param y2 Y coordinate of lower right corner
 * @param mode constants SCROLL_DOWN and SCROLL_UP control scroll direction
 *
 * Defines a text area based on absolute coordinates.
 * The pixel coordinates for the text area are inclusive so x2,y2 is the lower right
 * pixel of the text area.
 *
 * x1,y1 and x2,y2 are an absolute coordinates and are relateive to the 0,0 origin of the
 * display.
 *
 * The area within the newly defined text area is intentionally not cleared.
 *
 * mode is an optional parameter and defaults to normal/up scrolling
 *
 * @returns true with the given area selected if all the coordinates are valid,
 * otherwise returns returns false with the area set to the full display
 *
 * @note
 * Upon creation of the text area, the cursor position for the text area will be set to x1, y1
 *
 * @see ClearArea()
 *
 */

uint8_t
gText::DefineArea(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, textMode mode)
{
uint8_t ret = false;
	if(		(x1 >= x2)
		||	(y1 >= y2)
		||	(x1 >= DISPLAY_WIDTH)
		||	(y1 >= DISPLAY_HEIGHT)
		||	(x2 >= DISPLAY_WIDTH)
		||	(y2 >= DISPLAY_WIDTH)
	)
	{
	    // failed sanity check so set defaults and return false 
		this->tarea.x1 = 0;
		this->tarea.y1 = 0;
		this->tarea.x2 = DISPLAY_WIDTH -1;
		this->tarea.y2 = DISPLAY_HEIGHT -1;
		this->tarea.mode = DEFAULT_SCROLLDIR;
	} 		
	else
	{  
	    this->tarea.x1 = x1; 
	    this->tarea.y1 = y1; 
		this->tarea.x2 = x2; 
	    this->tarea.y2 = y2; 		
		this->tarea.mode = mode; // not yet sanity checked
		ret = true;
	}		
	/*
	 * set cursor position for the area
	 */
	this->x = x1;
	this->y = y1;	
	
#ifndef GLCD_NODEFER_SCROLL
	/*
	 * Make sure to clear a deferred scroll operation when re defining areas.
	 */
	this->need_scroll = 0;
#endif
    return ret;
}

/**
 * Define a predefined generic text area
 *
 * @param selection a value from @ref predefinedArea
 * @param mode constants SCROLL_DOWN and SCROLL_UP control scroll direction
 *
 * Defines a text area using a selection form a set of predefined areas.
 *
 * The area within the newly defined text area is intentionally not cleared.
 *
 * mode is an optional parameter and defaults to normal/up scrolling
 *
 * @return returns @em true if successful.
 *
 *
 * @note
 * Upon defining the text area, the cursor position for the text area will be set to 
 * the upper left coordinate of the given predefined area
 *
 * @see ClearArea()
 * @see predefinedArea
 *
 */

uint8_t
gText::DefineArea(predefinedArea selection, textMode mode)
{
uint8_t x1,y1,x2,y2;
TareaToken tok;

	tok.token = selection;

	x1 =  tok.coord.x1;
	y1 =  tok.coord.y1;
	x2 =  tok.coord.x2;
	y2 =  tok.coord.y2;

	return this->DefineArea(x1,y1,x2,y2, mode);
}

/*
 * Scroll a pixel region up.
 * 	Area scrolled is defined by x1,y1 through x2,y2 inclusive.
 *  x1,y1 is upper left corder, x2,y2 is lower right corner.
 *
 *	color is the color to be used for the created space along the
 *	bottom.
 *
 *	pixels is the *exact* pixels to scroll. 1 is 1 and 9 is 9 it is
 *  not 1 less or 1 more than what you want. It is *exact*.
 */

void gText::ScrollUp(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, 
	uint8_t pixels, uint8_t color)
{
uint8_t dy;
uint8_t dbyte;
uint8_t sy;
uint8_t sbyte;
uint8_t col;

	/*
	 * Scrolling up more than area height?
	 */
	if(y1 + pixels > y2)
	{
		/*
		 * fill the region with "whitespace" because
		 * it is being totally scrolled out.
		 */
		glcd_Device::SetPixels(x1, y1, x2, y2, color);
		return;
	}

	for(col = x1; col <= x2; col++)
	{
		dy = y1;
		glcd_Device::GotoXY(col, dy & ~7);
		dbyte = glcd_Device::ReadData();


		/*
		 * preserve bits outside/above scroll region
		 */

		dbyte &= (_BV((dy & 7)) - 1);

		sy = dy + pixels;
		glcd_Device::GotoXY(col, sy & ~7);
		sbyte = glcd_Device::ReadData();

		while(sy <= y2)
		{
			if(sbyte & _BV(sy & 7))
			{
				dbyte |= _BV(dy & 7);
			}

			sy++;
			if((sy & 7) == 0)
			{
				/*
				 * If we just crossed over, then we should be done.
				 */
				if(sy < DISPLAY_HEIGHT)
				{
					glcd_Device::GotoXY(col, sy & ~7);
					sbyte = glcd_Device::ReadData();
				}
			}

			if((dy & 7) == 7)
			{
				glcd_Device::GotoXY(col, dy & ~7);	// Should be able to remove this
				glcd_Device::WriteData(dbyte);
				dbyte = 0;
			}
			dy++;
		}

		/*
		 * Handle the new area at the bottom of the region
		 */

		for(uint8_t p = pixels; p; p--)
		{
			if(color == BLACK)
			{
				dbyte |= _BV(dy & 7);
			}
			else
			{
				dbyte &= ~_BV(dy & 7);
			}

			if((dy & 7) == 7)
			{
				glcd_Device::GotoXY(col, dy & ~7); // should be able to remove this.
				glcd_Device::WriteData(dbyte);
				dbyte = 0;
			}
			dy++;
		}

		/*
		 * Flush out the final destination byte
		 */


		if(dy & 7)
		{
			dy--;

			glcd_Device::GotoXY(col, dy & ~7);
			sbyte = glcd_Device::ReadData();
			/*
			 * Preserver bits outside/below region
			 */

			dy++;
			sbyte &= ~(_BV((dy & 7)) - 1);
			dbyte |= sbyte;

			glcd_Device::WriteData(dbyte);
		}
	}

}

#ifndef GLCD_NO_SCROLLDOWN

void gText::ScrollDown(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, 
	uint8_t pixels, uint8_t color)
{
uint8_t dy;
uint8_t dbyte;
uint8_t sy;
uint8_t sbyte;
uint8_t col;

	/*
	 * Scrolling up more than area height?
	 */
	if(y1 + pixels > y2)
	{
		/*
		 * fill the region with "whitespace" because
		 * it is being totally scrolled out.
		 */
		glcd_Device::SetPixels(x1, y1, x2, y2, color);
		return;
	}

	/*
	 * Process region from left to right
	 */
	for(col = x1; col <= x2; col++)
	{
		dy = y2;
		glcd_Device::GotoXY(col, dy & ~7);
		dbyte = glcd_Device::ReadData();

		/*
		 * preserve bits outside/below scroll region
		 */

		dbyte &= ~(_BV(((dy & 7)+1)) - 1);
		sy = dy - pixels;
		glcd_Device::GotoXY(col, sy & ~7);
		sbyte = glcd_Device::ReadData();

		while(sy >= y1)
		{
			if(sbyte & _BV(sy & 7))
			{
				dbyte |= _BV(dy & 7);
			}
			if((dy & 7) == 0)
			{
				glcd_Device::GotoXY(col, dy & ~7);	// Should be able to remove this
				glcd_Device::WriteData(dbyte);
				dbyte = 0;
			}
			dy--;

			if(!sy)
				break; /* if we bottomed out, we are done */
			sy--;
			if((sy & 7) == 7)
			{
				glcd_Device::GotoXY(col, sy & ~7);
				sbyte = glcd_Device::ReadData();
			}

		}

		/*
		 * Handle the new area at the top of the column
		 */

		for(uint8_t p = pixels; p; p--)
		{
			if(color == BLACK)
			{
				dbyte |= _BV(dy & 7);
			}
			else
			{
				dbyte &= ~_BV(dy & 7);
			}

			if((dy & 7) == 0)
			{
				glcd_Device::GotoXY(col, dy & ~7); // should be able to remove this.
				glcd_Device::WriteData(dbyte);
				dbyte = 0;
			}
			dy--;
		}

		dy++; /* point dy back to last destination row */

		/*
		 * Flush out the final destination byte
		 */

		if(dy & 7)
		{
			glcd_Device::GotoXY(col, dy & ~7);
			sbyte = glcd_Device::ReadData();
			/*
			 * Preserve bits outside/above region
			 */

			sbyte &= (_BV((dy & 7)) - 1);
			dbyte |= sbyte;
			glcd_Device::WriteData(dbyte);
		}

	}

}
#endif //GLCD_NO_SCROLLDOWN


/*
 * Handle all special processing characters
 */
void gText::SpecialChar(uint8_t c)
{


	if(c == '\n')
	{
		uint8_t height = FontRead(this->Font+FONT_HEIGHT);

		/*
		 * Erase all pixels remaining to edge of text area.on all wraps
		 * It looks better when using inverted (WHITE) text, on proportional fonts, and
		 * doing WHITE scroll fills.
		 *
		 */


		if(this->x < this->tarea.x2)
			glcd_Device::SetPixels(this->x, this->y, this->tarea.x2, this->y+height, this->FontColor == BLACK ? WHITE : BLACK);

		/*
		 * Check for scroll up vs scroll down (scrollup is normal)
		 */
#ifndef GLCD_NO_SCROLLDOWN
		if(this->tarea.mode == SCROLL_UP)
#endif
		{

			/*
			 * Normal/up scroll
			 */

			/*
			 * Note this comparison and the pixel calcuation below takes into 
			 * consideration that fonts
			 * are atually 1 pixel taller when rendered. 
			 * This extra pixel is along the bottom for a "gap" between the character below.
			 */
			if(this->y + 2*height >= this->tarea.y2)
			{
#ifndef GLCD_NODEFER_SCROLL
					if(!this->need_scroll)
					{
						this->need_scroll = 1;
						return;
					}
#endif

				/*
				 * forumula for pixels to scroll is:
				 *	(assumes "height" is one less than rendered height)
				 *
				 *		pixels = height - ((this->tarea.y2 - this->y)  - height) +1;
				 *
				 *		The forumala below is unchanged 
				 *		But has been re-written/simplified in hopes of better code
				 *
				 */

				uint8_t pixels = 2*height + this->y - this->tarea.y2 +1;
		
				/*
				 * Scroll everything to make room
				 * * NOTE: (FIXME, slight "bug")
				 * When less than the full character height of pixels is scrolled,
				 * There can be an issue with the newly created empty line.
				 * This is because only the # of pixels scrolled will be colored.
				 * What it means is that if the area starts off as white and the text
				 * color is also white, the newly created empty text line after a scroll 
				 * operation will not be colored BLACK for the full height of the character.
				 * The only way to fix this would be alter the code use a "move pixels"
				 * rather than a scroll pixels, and then do a clear to end line immediately
				 * after the move and wrap.
				 *
				 * Currently this only shows up when
				 * there are are less than 2xheight pixels below the current Y coordinate to
				 * the bottom of the text area
				 * and the current background of the pixels below the current text line
				 * matches the text color
				 * and  a wrap was just completed.
				 *
				 * After a full row of text is printed, the issue will resolve itself.
				 * 
				 * 
				 */
				this->ScrollUp(this->tarea.x1, this->tarea.y1, 
					this->tarea.x2, this->tarea.y2, pixels, this->FontColor == BLACK ? WHITE : BLACK);

				this->x = this->tarea.x1;
				this->y = this->tarea.y2 - height;
			}
			else
			{
				/*
				 * Room for simple wrap
				 */

				this->x = this->tarea.x1;
				this->y = this->y+height+1;
			}
		}
#ifndef GLCD_NO_SCROLLDOWN
		else
		{
			/*
			 * Reverse/Down scroll
			 */

			/*
			 * Check for Wrap vs scroll.
			 *
			 * Note this comparison and the pixel calcuation below takes into 
			 * consideration that fonts
			 * are atually 1 pixel taller when rendered. 
			 *
			 */
			if(this->y > this->tarea.y1 + height)
			{
				/*
				 * There is room so just do a simple wrap
				 */
				this->x = this->tarea.x1;
				this->y = this->y - (height+1);
			}
			else
			{
#ifndef GLCD_NODEFER_SCROLL
					if(!this->need_scroll)
					{
						this->need_scroll = 1;
						return;
					}
#endif

				/*
				 * Scroll down everything to make room for new line
				 *	(assumes "height" is one less than rendered height)
				 */

				uint8_t pixels = height+1 - (this->tarea.y1 - this->y);

				this->ScrollDown(this->tarea.x1, this->tarea.y1, 
					this->tarea.x2, this->tarea.y2, pixels, this->FontColor == BLACK ? WHITE : BLACK);

				this->x = this->tarea.x1;
				this->y = this->tarea.y1;
			}
		}
#endif
	}

}

/**
 * output a character
 *
 * @param c the character to output
 *
 * If the character will not fit on the current text line
 * inside the text area,
 * the text position is wrapped to the next line. This might be
 * the next lower or the next higher line depending on the
 * scroll direction.
 *
 * If there is not enough room to fit a full line of new text after
 * wrapping, the entire text area will be scrolled to make room for a new
 * line of text. The scroll direction will be up or down
 * depending on the scroll direction for the text area.
 * 
 * @see Puts()
 * @see Puts_P()
 * @see write()
 */

int gText::PutChar(uint8_t c)
{
    if(this->Font == 0)
	  return 0; // no font selected

	/*
	 * check for special character processing
	 */

	if(c < 0x20)
	{
		SpecialChar(c);
		return 1;
	}
	   
	uint8_t width = 0;
	uint8_t height = FontRead(this->Font+FONT_HEIGHT);
	uint8_t bytes = (height+7)/8; /* calculates height in rounded up bytes */
	
	uint8_t firstChar = FontRead(this->Font+FONT_FIRST_CHAR);
	uint8_t charCount = FontRead(this->Font+FONT_CHAR_COUNT);
	
	uint16_t index = 0;
	uint8_t thielefont;

	if(c < firstChar || c >= (firstChar+charCount)) {
		return 0; // invalid char
	}
	c-= firstChar;

	if( isFixedWidthFont(this->Font) {
		thielefont = 0;
		width = FontRead(this->Font+FONT_FIXED_WIDTH); 
		index = c*bytes*width+FONT_WIDTH_TABLE;
	}
	else{
	// variable width font, read width data, to get the index
		thielefont = 1;
		/*
		 * Because there is no table for the offset of where the data
		 * for each character glyph starts, run the table and add up all the
		 * widths of all the characters prior to the character we
		 * need to locate.
		 */
	   for(uint8_t i=0; i<c; i++) {  
		 index += FontRead(this->Font+FONT_WIDTH_TABLE+i);
	   }
		/*
		 * Calculate the offset of where the font data
		 * for our character starts.
		 * The index value from above has to be adjusted because
		 * there is potentialy more than 1 byte per column in the glyph,
		 * when the characgter is taller than 8 bits.
		 * To account for this, index has to be multiplied
		 * by the height in bytes because there is one byte of font
		 * data for each vertical 8 pixels.
		 * The index is then adjusted to skip over the font width data
		 * and the font header information.
		 */

	   index = index*bytes+charCount+FONT_WIDTH_TABLE;

		/*
		 * Finally, fetch the width of our character
		 */
	   width = FontRead(this->Font+FONT_WIDTH_TABLE+c);
    }

#ifndef GLCD_NODEFER_SCROLL
	/*
	 * check for a defered scroll
	 * If there is a deferred scroll,
	 * Fake a newline to complete it.
	 */

	if(this->need_scroll)
	{
		this->PutChar('\n'); // fake a newline to cause wrap/scroll
		this->need_scroll = 0;
	}
#endif

	/*
	 * If the character won't fit in the text area,
	 * fake a newline to get the text area to wrap and 
	 * scroll if necessary.
	 * NOTE/WARNING: the below calculation assumes a 1 pixel pad.
	 * This will need to be changed if/when configurable pixel padding is supported.
	 */
	if(this->x + width > this->tarea.x2)
	{
		this->PutChar('\n'); // fake a newline to cause wrap/scroll
#ifndef GLCD_NODEFER_SCROLL
		/*
		 * We can't defer a scroll at this point since we need to ouput
		 * a character right now.
		 */
		if(this->need_scroll)
		{
			this->PutChar('\n'); // fake a newline to cause wrap/scroll
			this->need_scroll = 0;
		}
#endif
	}

	// last but not least, draw the character

#ifdef GLCD_OLD_FONTDRAW
/*================== OLD FONT DRAWING ============================*/
	glcd_Device::GotoXY(this->x, this->y);

	/*
	 * Draw each column of the glyph (character) horizontally
	 * 8 bits (1 page) at a time.
	 * i.e. if a font is taller than 8 bits, draw upper 8 bits first,
	 * Then drop down and draw next 8 bits and so on, until done.
	 * This code depends on WriteData() doing writes that span LCD
	 * memory pages, which has issues because the font data isn't
	 * always a multiple of 8 bits.
	 */

	for(uint8_t i=0; i<bytes; i++)	/* each vertical byte */
	{
		uint16_t page = i*width; // page must be 16 bit to prevent overflow
		for(uint8_t j=0; j<width; j++) /* each column */
		{
			uint8_t data = FontRead(this->Font+index+page+j);
		
			/*
			 * This funkyness is because when the character glyph is not a
			 * multiple of 8 in height, the residual bits in the font data
			 * were aligned to the incorrect end of the byte with respect 
			 * to the GLCD. I believe that this was an initial oversight (bug)
			 * in Thieles font creator program. It is easily fixed
			 * in the font program but then creates a potential backward
			 * compatiblity problem.
			 *	--- bperrybap
			 */

			if(height > 8 && height < (i+1)*8)	/* is it last byte of multibyte tall font? */
			{
				data >>= (i+1)*8-height;
			}
			
			if(this->FontColor == BLACK) {
				glcd_Device::WriteData(data);
			} else {
				glcd_Device::WriteData(~data);
			}
		}
		// 1px gap between chars
		if(this->FontColor == BLACK) {
			glcd_Device::WriteData(0x00);
		} else {
			glcd_Device::WriteData(0xFF);
		}
		glcd_Device::GotoXY(this->x, glcd_Device::Coord.y+8);
	}
	this->x = this->x+width+1;

/*================== END of OLD FONT DRAWING ============================*/
#else

/*================== NEW FONT DRAWING ===================================*/

	/*
	 * Paint font data bits and write them to LCD memory 1 LCD page at a time.
	 * This is very different from simply reading 1 byte of font data
	 * and writing all 8 bits to LCD memory and expecting the write data routine
	 * to fragement the 8 bits across LCD 2 memory pages when necessary.
	 * That method (really doesn't work) and reads and writes the same LCD page 
	 * more than once as well as not do sequential writes to memory.
	 *
	 * This method of rendering while much more complicated, somewhat scrambles the font 
	 * data reads to ensure that all writes to LCD pages are always sequential and a given LCD
	 * memory page is never read or written more than once.
	 * And reads of LCD pages are only done at the top or bottom of the font data rendering
	 * when necessary. 
	 * i.e it ensures the absolute minimum number of LCD page accesses
	 * as well as does the sequential writes as much as possible.
	 *
	 */

	uint8_t pixels = height +1; /* 1 for gap below character*/
	uint8_t p;
	uint8_t dy;
	uint8_t tfp;
	uint8_t dp;
	uint8_t dbyte;
	uint8_t fdata;

	for(p = 0; p < pixels;)
	{
		dy = this->y + p;

		/*
		 * Align to proper Column and page in LCD memory
		 */

		glcd_Device::GotoXY(this->x, (dy & ~7));

		uint16_t page = p/8 * width; // page must be 16 bit to prevent overflow

		for(uint8_t j=0; j<width; j++) /* each column of font data */
		{
			
			/*
			 * Fetch proper byte of font data.
			 * Note:
			 * This code "cheats" to add the horizontal space/pixel row
			 * below the font.
			 * It essentially creates a font pixel of 0 when the pixels are
			 * out of the defined pixel map.
			 *
			 * fake a fondata read read when we are on the very last
			 * bottom "pixel". This lets the loop logic continue to run
			 * with the extra fake pixel. If the loop is not the
			 * the last pixel the pixel will come from the actual
			 * font data, but that is ok as it is 0 padded.
			 *
			 */

			if(p >= height)
			{
				/*
				 * fake a font data read for padding below character.
				 */
				fdata = 0;
			}
			else
			{
				fdata = FontRead(this->Font+index+page+j);

				/*
				 * Have to shift font data because Thiele shifted residual
				 * font bits the wrong direction for LCD memory.
				 *
				 * The real solution to this is to fix the variable width font format to
				 * not shift the residual bits the wrong direction!!!!
				 */
				if(thielefont && (height - (p&~7)) < 8)
				{
					fdata >>= 8 - (height & 7);
				}
			}

			if(this->FontColor == WHITE)
				fdata ^= 0xff;	/* inverted data for "white" font color	*/


			/*
			 * Check to see if a quick full byte write of font
			 * data can be done.
			 */

			if(!(dy & 7) && !(p & 7) && ((pixels -p) >= 8))
			{
				/*
				 * destination pixel is on a page boundary
				 * Font data is on byte boundary
				 * And there are 8 or more pixels left
				 * to paint so a full byte write can be done.
				 */
					
					glcd_Device::WriteData(fdata);
					continue;
			}
			else
			{
					/*
					 * No, so must fetch byte from LCD memory.
					 */
					dbyte = glcd_Device::ReadData();
			}

			/*
			 * At this point there is either not a full page of data
			 * left to be painted  or the font data spans multiple font
			 * data bytes. (or both) So, the font data bits will be painted
			 * into a byte and then written to the LCD memory.page.
			 */


			tfp = p;		/* font pixel bit position 		*/
			dp = dy & 7;	/* data byte pixel bit position */

			/*
			 * paint bits until we hit bottom of page/byte
			 * or run out of pixels to paint.
			 */
			while((dp <= 7) && (tfp) < pixels)
			{
				if(fdata & _BV(tfp & 7))
				{
					dbyte |= _BV(dp);
				}
				else
				{
					dbyte &= ~_BV(dp);
				}

				/*
				 * Check for crossing font data bytes
				 */
				if((tfp & 7)== 7)
				{
					fdata = FontRead(this->Font+index+page+j+width);

					/*
					 * Have to shift font data because Thiele shifted residual
					 * font bits the wrong direction for LCD memory.
					 *
					 */

					if((thielefont) && ((height - tfp) < 8))
					{
						fdata >>= (8 - (height & 7));
					}

					if(this->FontColor == WHITE)
						fdata ^= 0xff;	/* inverted data for "white" color	*/
				}
				tfp++;
				dp++;
			}

			/*
			 * Now flush out the painted byte.
			 */
			glcd_Device::WriteData(dbyte);
		}

		/*
		 * now create a horizontal gap (vertical line of pixels) between characters.
		 * Since this gap is "white space", the pixels painted are oposite of the
		 * font color.
		 *
		 * Since full LCD pages are being written, there are 4 combinations of filling
		 * in the this gap page.
		 *	- pixels start at bit 0 and go down less than 8 bits
		 *	- pixels don't start at 0 but go down through bit 7
		 *	- pixels don't start at 0 and don't go down through bit 7 (fonts shorter than 6 hi)
		 *	- pixels start at bit 0 and go down through bit 7 (full byte)
		 *
		 * The code below creates a mask of the bits that should not be painted.
		 *
		 * Then it is easy to paint the desired bits since if the color is WHITE,
		 * the paint bits are set, and if the coloer is not WHITE the paint bits are stripped.
		 * and the paint bits are the inverse of the desired bits mask.
		 */


		
		if((dy & 7) || (pixels - p < 8))
		{
		uint8_t mask = 0;

			dbyte = glcd_Device::ReadData();

			if(dy & 7)
				mask |= _BV(dy & 7) -1;

			if((pixels-p) < 8)
				mask |= ~(_BV(pixels - p) -1);


			if(this->FontColor == WHITE)
				dbyte |= ~mask;	
			else
				dbyte &= mask;

		}
		else
		{
			if(this->FontColor == WHITE)
				dbyte = 0xff;
			else
				dbyte = 0;
		}

		glcd_Device::WriteData(dbyte);

		/*
		 * advance the font pixel for the pixels
		 * just painted.
		 */

		p += 8 - (dy & 7);
	}


	/*
	 * Since this rendering code always starts off with a GotoXY() it really isn't necessary
	 * to do a real GotoXY() to set the h/w location after rendering a character.
	 * We can get away with only setting the s/w version of X & Y.
	 *
	 * Since y didn't change while rendering, it is still correct.
	 * But update x for the pixels rendered.
	 *
	 */

	this->x = this->x+width+1;

/*================== END of NEW FONT DRAWING ============================*/

#endif // NEW_FONTDRAW

	return 1; // valid char
}


/**
 * output a character string
 *
 * @param str pointer to a null terminated character string.
 *
 * Outputs all the characters in the string to the text area. 
 * See PutChar() for a full description of how characters are
 * written to the text area.
 *
 * @see PutChar()
 * @see Puts_P()
 * @see DrawString()
 * @see DrawString_P()
 * @see write()
 */


void gText::Puts(char *str)
{
    while(*str)
	{
        this->PutChar((uint8_t)*str);
        str++;
    }
}

/**
 * output a String class string
 *
 * @param str String class string
 *
 * Outputs all the characters in the string to the text area. 
 * See PutChar() for a full description of how characters are
 * written to the text area.
 *
 * @see PutChar()
 * @see Puts_P()
 * @see DrawString()
 * @see DrawString_P()
 * @see write()
 */
void gText::Puts(const String &str)
{
	for (int i = 0; i < str.length(); i++)
	{
		write(str[i]);
	}
}

/**
 * output a program memory character string
 *
 * @param str pointer to a null terminated character string stored in program memory
 *
 * Outputs all the characters in the string to the text area. 
 * See PutChar() for a full description of how characters are
 * written to the text area.
 *
 * @see PutChar()
 * @see Puts()
 * @see DrawString()
 * @see DrawString_P()
 * @see write()
 */

void gText::Puts_P(PGM_P str)
{
uint8_t c;

    while((c = pgm_read_byte(str)) != 0)
	{
        this->PutChar(c);
        str++;
    }
}

/**
 * output a character string at x,y coordinate
 *
 * @param str String class string
 * @param x specifies the horizontal location
 * @param y specifies the vertical location
 *
 *
 * Outputs all the characters in the string to the text area. 
 * X & Y are zero based pixel coordinates and are relative to 
 * the upper left corner of the text area.
 *
 * See PutChar() for a full description of how characters are
 * written to the text area.
 *
 *
 * @see PutChar()
 * @see Puts()
 * @see Puts_P()
 * @see DrawString_P()
 * @see write()
 */

void gText::DrawString(char *str, uint8_t x, uint8_t y)
{
	this->CursorToXY(x,y);
	this->Puts(str);
}

/**
 * output a String class string at x,y coordinate
 *
 * @param str pointer to a null terminated character string
 * @param x specifies the horizontal location
 * @param y specifies the vertical location
 *
 *
 * Outputs all the characters in the string to the text area. 
 * X & Y are zero based pixel coordinates and are relative to 
 * the upper left corner of the text area.
 *
 * See PutChar() for a full description of how characters are
 * written to the text area.
 *
 *
 * @see PutChar()
 * @see Puts()
 * @see Puts_P()
 * @see DrawString_P()
 * @see write()
 */
void gText::DrawString(String &str, uint8_t x, uint8_t y)
{
	this->CursorToXY(x,y);
	this->Puts(str);
}

/**
 * output a program memory character string at x,y coordinate
 *
 * @param str pointer to a null terminated character string stored in program memory
 * @param x specifies the horizontal location
 * @param y specifies the vertical location
 *
 *
 * Outputs all the characters in the string to the text area. 
 * X & Y are zero based pixel coordinates and are relative to 
 * the upper left corner of the text area.
 *
 * See PutChar() for a full description of how characters are
 * written to the text area.
 *
 *
 * @see PutChar()
 * @see Puts()
 * @see Puts_P()
 * @see DrawString()
 * @see write()
 */

void gText::DrawString_P(PGM_P str, uint8_t x, uint8_t y)
{
	this->CursorToXY(x,y);
	this->Puts_P(str);
}

/**
 * Positions cursor to a character based column and row.
 *
 * @param column specifies the horizontal position 
 * @param row  specifies the vertical position
 *
 *	Column and Row are zero based character positions
 *	and are relative the the upper left corner of the
 *	text area base on the size of the currently selected font.
 *
 * While intended for fixed width fonts, positioning will work for variable
 * width fonts.
 *
 * When variable width fonts are used, the column is based on assuming a width
 * of the widest character.
 *
 * @see CursorToXY()
 */

void gText::CursorTo( uint8_t column, uint8_t row)
{
	if(this->Font == 0)
		return; // no font selected

	/*
	 * Text position is relative to current text area
	 */

	this->x = column * (FontRead(this->Font+FONT_FIXED_WIDTH)+1) + this->tarea.x1;
	this->y = row * (FontRead(this->Font+FONT_HEIGHT)+1) + this->tarea.y1;

#ifndef GLCD_NODEFER_SCROLL
	/*
	 * Make sure to clear a deferred scroll operation when repositioning
	 */
	this->need_scroll = 0;
#endif
}

// Bill, I think the following would be a useful addition to the API
// Should we add a sanity check to these?
/**
 * Positions cursor to a character based column on the current row.
 *
 * @param column specifies the horizontal position 
 *
 *	Column is a 0 based character position
 *	based on the size of the currently selected font.
 *
 * If column is negative then the column position is relative to the current cursor
 * position.
 *
 * @warning
 * While intended only for fixed width fonts, cursor repositioning will be done for variable
 * width fonts.
 * When variable width fonts are used, the column is based on assuming a width
 * of the widest character in the font.
 * Because the widest character is used for the amount of cursor movement, the amount
 * of cursor movement when using relative positioning will often not be consistent with
 * the number characters previously rendered. For example, if a letter "l" was written
 * and the cursor was reposisitioned with a -1, the amount backed up will be much larger
 * than the width of the "l".
 *
 *
 * @see CursorToXY()
 */
void gText::CursorTo( int8_t column)
{
	if(this->Font == 0)
		return; // no font selected
	/*
	 * Text position is relative to current text area
	 * negative value moves the cursor backwards
	 */
    if(column >= 0) 
	  this->x = column * (FontRead(this->Font+FONT_FIXED_WIDTH)+1) + this->tarea.x1;
	else
   	  this->x -= column * (FontRead(this->Font+FONT_FIXED_WIDTH)+1);   	

#ifndef GLCD_NODEFER_SCROLL
	/*
	 * Make sure to clear a deferred scroll operation when repositioning
	 */
	this->need_scroll = 0;
#endif
}


/**
 * Positions cursor to a X,Y position
 *
 * @param x specifies the horizontal location
 * @param y specifies the vertical location
 *
 *	X & Y are zero based pixel coordinates and are relative to 
 *	the upper left corner of the text area.
 *
 * @see CursorTo()
 */

void gText::CursorToXY( uint8_t x, uint8_t y)
{

	/*
	 * Text position is relative to current text area
	 */
	this->x = this->tarea.x1 + x;
	this->y = this->tarea.y1 + y;

#ifndef GLCD_NODEFER_SCROLL
	/*
	 * Make sure to clear a deferred scroll operation when repositioning
	 */
	this->need_scroll = 0;
#endif
}

/**
 * Erase in Line
 *
 * @param type type of line erase
 *
 * @arg \ref eraseTO_EOL Erase from cursor to end of line
 * @arg \ref eraseFROM_BOL Erase from beginning of line to cursor
 * @arg \ref eraseFULL_LINE Erase entire line
 *
 * Erases all or part of a line of text depending on the type
 * of erase specified.
 *
 * If type is not specified it is assumed to be \ref eraseTO_EOL
 *
 * The cursor position does not change.
 *
 * @see ClearArea()
 * @see eraseLine_t
 */

void gText::EraseTextLine( eraseLine_t type) 
{

	uint8_t x = this->x;
	uint8_t y = this->y;
	uint8_t height = FontRead(this->Font+FONT_HEIGHT);
	uint8_t color = (this->FontColor == BLACK) ? WHITE : BLACK;

	switch(type)
	{
		case eraseTO_EOL:
				glcd_Device::SetPixels(x, y, this->tarea.x2, y+height, color);
				break;
		case eraseFROM_BOL:
				glcd_Device::SetPixels(this->tarea.x1, y, x, y+height, color);
				break;
		case eraseFULL_LINE:
				glcd_Device::SetPixels(this->tarea.x1, y, this->tarea.x2, y+height, color);
				break;
	}

	/*
	 * restore cursor position
	 */
	this->CursorToXY(x,y);
}

/**
 * Erase Text Line
 *
 * @param row row # of text to earase
 *
 * Erases a line of text and moves the cursor
 * to the begining of the line. Rows are zero based so
 * the top line/row of a text area is 0.
 *
 * @see ClearArea()
 */

void gText::EraseTextLine( uint8_t row)
{
   this->CursorTo(0, row);
   EraseTextLine(eraseTO_EOL);	
}


/**
 * Select a Font and font color
 *
 * @param font a font definition
 * @param color  can be WHITE or BLACK and defaults to black
 * @param callback optional font read routine
 *
 *
 * Selects the font definition as the current font for the text area.
 *
 * All subsequent printing functions will use this font. 
 *
 * Font definitions from included font definition files are stored in program memory 
 * You can have as many fonts defines as will fit in program memory up to 64k and can
 * switch between them with this function.
 *
 * If the optional callback argument is ommitted, a default routine
 * is selected that assumes that the font is in program memory (flash).
 *
 * @note
 * When the display is initilized in normal mode, BLACK renders dark 
 * pixels on a white background and WHITE renders white pixels on 
 * black background; however, if the display is set to INVERTED mode
 * all colors are inverted. 
 *
 * @see SetFontColor()
 * @see SetTextMode()
 */

void gText::SelectFont(Font_t font,uint8_t color, FontCallback callback)
{
	this->Font = font;
	FontRead = callback;  // this sets the callback that will be used by all instances of gText
	this->FontColor = color;
}

/**
 * Select a font color
 *
 * @param color  can be WHITE or BLACK
 *
 *
 * @see SelectFont()
 * @see SetTextMode()
 */

void gText::SetFontColor(uint8_t color)
{
   	this->FontColor = color;
}

/**
 * Set TextArea mode
 *
 * @param mode  text area mode
 *
 * Currently mode is a scroll direction
 * @arg SCROLL_UP
 * @arg SCROLL_DOWN
 *
 * @see SelectFont()
 * @see SetFontColor()
 * @see DefineArea()
 */
/*
 */
void gText::SetTextMode(textMode mode)
{

/*
 * when other modes are added the tarea.mode variable will hold a bitmask or enum for the modde and should be renamed
 */
   this->tarea.mode = mode; 
} 
	
/**
 * Returns the pixel width of a character
 *
 * @param c character to be sized
 *
 * @return The width in pixels of the given character
 * including any inter-character gap pixels following the character when the character is
 * rendered on the display.
 *
 * @note The font for the character is the most recently selected font.
 *
 * @see StringWidth()
 * @see StringWidth_P()
 */

uint8_t gText::CharWidth(uint8_t c)
{
	uint8_t width = 0;
	
    if(isFixedWidthFont(this->Font){
		width = FontRead(this->Font+FONT_FIXED_WIDTH)+1;  // there is 1 pixel pad here
	} 
    else{ 
	    // variable width font 
		uint8_t firstChar = FontRead(this->Font+FONT_FIRST_CHAR);
		uint8_t charCount = FontRead(this->Font+FONT_CHAR_COUNT);
	
		// read width data
		if(c >= firstChar && c < (firstChar+charCount)) {
			c -= firstChar;
			width = FontRead(this->Font+FONT_WIDTH_TABLE+c)+1;
		}
	}	
	return width;
}

/**
 * Returns the pixel width of a string
 *
 * @param str pointer to string stored in RAM
 *
 * @return the width in pixels of the sum of all the characters in the
 * the string pointed to by str. 
 *
 * @see CharWidth()
 * @see StringWidth_P()
 */

uint16_t gText::StringWidth(const char* str)
{
	uint16_t width = 0;
	
	while(*str != 0) {
		width += this->CharWidth(*str++);
	}
	
	return width;
}

/**
 * Returns the pixel width of a character
 *
 * @param str pointer to string stored in program memory
 *
 * @return the width in pixels of the sum of all the characters in the
 * the string pointed to by str. 
 *
 * @see CharWidth()
 * @see StringWidth()
 */

uint16_t gText::StringWidth_P(PGM_P str)
{
	uint16_t width = 0;
	
	while(pgm_read_byte(str) != 0) {
		width += this->CharWidth(pgm_read_byte(str++));
	}
	
	return width;
}

/**
 * Returns the pixel width of a character
 *
		 * @param str String class string
 *
 * @return the width in pixels of the sum of all the characters in the
 * the string pointed to by str. 
 *
 * @see CharWidth()
 * @see StringWidth()
 */

uint16_t gText::StringWidth_P(String &str)
{
	uint16_t width = 0;

	for (int i = 0; i < str.length(); i++)
	{
		width += this->CharWidth(str[i]);
	}
	
	return width;
}


/**
 * Legacy function to print a number
 *
 * @param n is the number to print
 *
 *
 * @see print(n)
 */
void gText::PrintNumber(long n)
{
   uint8_t buf[10];  // prints up to 10 digits  
   uint8_t i=0;
   if(n==0)
	   PutChar('0');
   else{
	 if(n < 0){
        PutChar('-');
		n = -n;
	 }
     while(n>0 && i <= 10){
	   buf[i++] = n % 10;  // n % base
	   n /= 10;   // n/= base
	 }
	 for(; i >0; i--)
		 PutChar((char) (buf[i-1] < 10 ? '0' + buf[i-1] : 'A' + buf[i-1] - 10));	  
   }
}

/**
 * output a character to the text area
 * @param c the character to output
 *
 * This method is needed for the Print base class
 * @see PutChar()
 */

#if ARDUINO < 100
void gText::write(uint8_t c) 
{
	this->PutChar(c);
} 
#else
size_t gText::write(uint8_t c) 
{
	return(this->PutChar(c));
} 
#endif

#ifndef USE_ARDUINO_FLASHSTR
// functions to store and print strings in Progmem
// these should be removed when Arduino supports FLASH strings in the base print class
/**
 * print a flash based string
 * @param str pointer to a null terminated character string stored in program memory
 *
 * @see Puts_P()
 */
void gText::printFlash(FLASHSTRING str)
{
  Puts_P((PGM_P) str);
}

/**
 * print a flash based string
 * @param str pointer to a null terminated character string stored in program memory
 *
 * The string is output followed by a newline.
 *
 * @see Puts_P()
 */
void gText::printFlashln(FLASHSTRING str)
{
  printFlash(str);
  write('\n');
}
#endif


#ifndef GLCD_NO_PRINTF
/*
 * Support for printf().
 * This code plays a few games with the AVR stdio routines.
 *
 * The Printf() functions fudge up a STDIO stream to point back to a C callable function
 * which recovers the C++ text area object (this) and then prints the character
 * using the C++ text area object.
 */

extern "C"
{
  int glcdputc(char c, FILE *fp)
  {
  gText *gtp;

	gtp = (gText *) fdev_get_udata(fp);
	gtp->write((uint8_t) c);
	return(0);
  }
}

/**
 * print formatted data
 *
 * @param format string that contains text or optional embedded format tags
 * @param ... Depending on the format string, the function may expect a sequence of additional arguments.
 *
 * Writes a sequence of data formatted as the @em format argument specifies.
 * After the @em format parameter, the function expects at least as many additional
 * arguments as specified in @em format.
 * The format string supports all standard @em printf() formating % tags.
 *
 * @note
 *	By default @em printf() has no floating support in AVR enviornments.
 *	In order to enable this, a linker option must be changed. Currenly,
 *	the Arduino IDE does not support modifying the linker options.
 *
 * @see Printf_P()
 */ 


void gText::Printf(const char *format, ...)
{
static FILE stdiostr;

	va_list ap;

	fdev_setup_stream(&stdiostr, glcdputc, NULL, _FDEV_SETUP_WRITE);
	fdev_set_udata(&stdiostr, this);

	va_start(ap, format);
	vfprintf(&stdiostr, format, ap);
	va_end(ap);
}

/**
 * print formatted data
 *
 * @param format string in AVR progmem that contains text or optional embedded format tags
 * @param ... Depending on the format string, the function may expect a sequence of additional arguments.
 *
 * See gText::Printf() for full details.
 * @see Printf()
 */ 


void gText::Printf_P(const char *format, ...)
{
static FILE stdiostr;

	va_list ap;

	fdev_setup_stream(&stdiostr, glcdputc, NULL, _FDEV_SETUP_WRITE);
	fdev_set_udata(&stdiostr, this);

	va_start(ap, format);
	vfprintf_P(&stdiostr, format, ap);
	va_end(ap);
}

#endif
