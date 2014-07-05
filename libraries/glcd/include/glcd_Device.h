/*
  glcd_Device.h - support for specific graphical LCDs
  Copyright (c) 2009, 2010 Michael Margolis and Bill Perry

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

  the glcd_Device class impliments the protocol for sending and receiving data and commands to a GLCD device.

  
*/

#ifndef	GLCD_DEVICE_H
#define GLCD_DEVICE_H

#include "glcd_Config.h"

#if defined WIRING
#include <WPrint.h> // used when deriving this class in Wiring
#else
#include "Print.h" // used when deriving this class in Arduino 
#endif


#define GLCD_Device 1 // software version of this class


// useful user constants
#define NON_INVERTED false
#define INVERTED     true

// Colors
#define BLACK				0xFF
#define WHITE				0x00

/// @cond hide_from_doxygen
typedef struct {
	uint8_t x;
	uint8_t y;
	struct {
#ifdef GLCD_XCOL_SUPPORT
		uint8_t col;
#endif
		uint8_t page;
	} chip[glcd_CHIP_COUNT];
} lcdCoord;
/// @endcond
	
/*
 * Note that all data in glcd_Device is static so that all derived instances  
 * (gText instances for example) share the same device state.
 * Any added data fields should also be static unless there is explicit reason
 * to not share the field among the instances 
 */
 
/**
 * @class glcd_Device
 * @brief Low level device functions
 * 
 */
class glcd_Device : public Print   
{
  private:
  // Control functions
	uint8_t DoReadData(void);
	void WriteCommand(uint8_t cmd, uint8_t chip);
	inline void Enable(void);
	inline void SelectChip(uint8_t chip); 
	void WaitReady(uint8_t chip);
	uint8_t GetStatus(uint8_t chip);
#if ARDUINO < 100
	void write(uint8_t); // for Print base class
#else
	size_t write(uint8_t); // for Print base class
#endif
	
  public:
    glcd_Device();
	protected: 
    int Init(uint8_t invert = false);      // now public, default is non-inverted
	void SetDot(uint8_t x, uint8_t y, uint8_t color);
	void SetPixels(uint8_t x, uint8_t y,uint8_t x1, uint8_t y1, uint8_t color);
    uint8_t ReadData(void);        // now public
    void WriteData(uint8_t data); 

  	void GotoXY(uint8_t x, uint8_t y);   
    static lcdCoord	  	Coord;  
	static uint8_t	 	Inverted; 
};
  
#endif
