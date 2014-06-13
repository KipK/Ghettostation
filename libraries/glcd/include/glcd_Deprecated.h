/*
  glcd_deprecated.h - GLCD functions supported for backward compatibility only
  Include this file if you are migrating from the ks0108 library and want to use the old style function names
  See the documentation for more information on migrating from the old library
  vi:ts=4

*/	

#ifndef	GLCD_DEPRECATED_H
#define GLCD_DEPRECATED_H

// the following line drawing functions have been renamed
// the new function will use the default color if the color parameter is omitted
/**
 * @defgroup glcd_deprecated Deprecated GLCD functions
 * The following functions have been deprecated from the GLCD inerface.
 * They are temporarily available by adding the following include to your sketch:
 * @code
 * #include "include/glcd_Deprecated.h"
 * @endcode
 */

/** @addtogroup glcd_deprecated */
/*@{*/


/**
 * @hideinitializer
 * @brief Draw a Vertical line
 * @details
 * This functions has been replaced by glcd::DrawVLine()
 * @deprecated
 *
 * @see glcd::DrawVLine()
 * @see glcd::DrawHLine()
 */

#define DrawVertLine(x, y, height, color)   DrawVLine(x, y, height, color)

/**
 * @hideinitializer
 * @brief Draw a Horizontal line
 * @details
 * This functions has been replaced by glcd::DrawHLine()
 * @deprecated
 * @see glcd::DrawHLine()
 * @see glcd::DrawVLine()
 */
#define DrawHoriLine(x, y, width, color)   DrawHLine(x, y, width, color)

/**
 * @hideinitializer
 * @brief Clear a line of system text
 * @details
 * This function should no longer be used.\n
 * It has been replaced by the text area function gText::EraseTextLine(uint8_t row)
 * and gText::EraseTextLine(eraseLine_t type).
 * @deprecated
 * @see gText::EraseTextLine(uint8_t row)
 * @see gText::EraseTextLine(eraseLine_t type)
 */
#define ClearSysTextLine(line) FillRect(0, (line*8), (DISPLAY_WIDTH-1), ((line*8)+ 7), WHITE )

/**
 * @hideinitializer
 * @brief Clear an 8 pixel high row
 * @details
 * This function should no longer be used.\n
 * It has been replaced by glcd::FillRect()
 * @deprecated
 * @see glcd::FillRect()
 * @see gText::EraseTextLine(eraseLine_t type)
 * @see gText::EraseTextLine(uint8_t row)
 */
#define ClearPage(line, color) FillRect(0, (line*8), (DISPLAY_WIDTH-1), ((line*8)+ 7), color )

/**
 * @hideinitializer
 * @brief Set LCD display mode
 * @details
 * This functions has been replaced by glcd::SetDisplayMode()
 * @deprecated
 * @see glcd::SetDisplayMode()
 */
#define SetInverted(invert)	SetDisplayMode(invert)  // function renamed
/*@}*/

#endif
