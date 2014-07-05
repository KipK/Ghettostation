/*
 * BigNums
 *
 * This sketch demonstrates how to use text areas, some of the large number
 * fonts, and the Printf() formatting function.
 *
 * Note: Printf() uses about 1.8k of code space. It can be expensive
 * if simple strings are being printed relative to using other simpler/smaller
 * functions such as Puts() or the Arduino Print class.
 * However, when doing doing lots of formatting or using hex number output the printf
 * function can be smaller.
 *
 * To save code space, try not to use both the arduino print class and
 * Printf().
 *
 * To save RAM space,
 * keep in mind that if using Printf() the formatting string will consum RAM space.
 * (Normal strings get copied to RAM so you can modify them)
 * Same is true when using Puts()
 * To avoid this, you can delcare your strings to only be in AVR progrmm memory by using the PSTR macro.
 * but then you have to also use the Print_P() or Puts_P() functions instead.
 * Example:
 *
 * GLCD.Printf_P(PSTR("format string"), arg1, arg2, ...);
 * It can also be used for simple strings too:
 * GLCD.Printf_P(PSTR("Hello world"));
 * GLCD.Puts_P(PSTR("Hello world"));
 *
 * The fixnums fonts are not complete fonts. The font headers only contain
 * the characters: '+', ',', '-', '.', '/', '0', '1', '2', '3', '4', '5', '6', '7', '9'
 * There are no other characters other than those listed above, so if you try to draw
 * something else like say an 'V', 'F', 'C', or anything not listed above,
 * it will not draw anything for that character.
 *
 * This is intentional to create the possiblity of having large numbers but not eat
 * up lots of flash space. This should be enough to do things like a clock or numbers
 * for things like counters, displaying voltage or temperature, etc...
 *
 * 2011-04-23 Created by Bill Perry
 */

#include "glcd.h"

/*
 * individual fonts don't have to be included
 * if you include the allFonts.h header
 */
//#include "fonts/fixednums15x31.h"
//#include "fonts/fixednums8x16.h"
//#include "fonts/fixednums7x15.h"
//#include "fonts/SystemFont5x7.h"

#include "fonts/allFonts.h"

void setup() {
	GLCD.Init(NON_INVERTED);
}

/*
 * Define some text areas
 * Text areas can be defined when declared or can be defined
 * runtime.
 *
 * Note:
 * The GLCD object contains a built in text area that is pre-defined
 * to be then entire glcd display.
 * All the same printing functions exist for the GLCD object and user
 * define text areas.
 */

gText t1; // will define runtime later

#if DISPLAY_HEIGHT > 32
gText t2; // will define runtime later

/*
* define t3 to be half way down starting in the middle (plus a pixel) and
* 8 columns by 1 row tall using the system font.
*/
gText t3 = gText(GLCD.CenterX+1, GLCD.CenterY, 8, 1, SystemFont5x7);

gText t4; // will define runtime later.
#endif

void loop()
{
int hr, min, sec;
int counter = 0;;

	/*
	 * Create 4 text areas in different ways
	 * for demonstration.
	 *
	 * Text areas can be declared using DefineArea()
	 *  in different ways:
	 * - predefined area
	 * - x1, y1, columns, rows
	 * - x1,y1 to x2,y2
	 *
	 */

	/*
	 * define t1 area based on a predefined area of the display
	 */

	t1.DefineArea(textAreaTOP);
	t1.SelectFont(fixednums15x31);

#if DISPLAY_HEIGHT > 32
	/*
 	 * define t2 to be half way down on the left and
	 * 8 columns by 2 rows tall
	 */
	t2.DefineArea(0, GLCD.CenterY, 8, 2, fixednums7x15);

	/*
	 * For demonstration, we will set the font color to white
	 * and clear the entire text area so it is visible.
	 */
	t2.SetFontColor(WHITE); // set font color 
	t2.ClearArea(); // Clear entire text area so you can see the full boundaries.


	/*
 	 * t3  text area was defined when declared.
	 */

	/*
	 * t4 is a 1 line 8 colum text area that starts in the middle
	 * and is 3/4 of the display height down.
	 */

	t4.DefineArea(GLCD.CenterX, GLCD.Height * 3/4, 8, 1, fixednums7x15);
#endif

	hr = 0;
	min = 0;
	sec = 0;
	while(1)
	{

#if DISPLAY_WIDTH > 127
		t1.CursorToXY(0,0); // column & row is relative to text area
		/*
		 * use a formatting string that is only in FLASH/(program memory) and *not* in RAM.
		 */
		t1.Printf_P(PSTR("%02d:%02d.%02d"), hr, min, sec);

#if DISPLAY_HEIGHT > 32
		t2.CursorTo(0,0); // column & row is relative to text area
		t2.Printf("%02d:%02d.%02d", hr, min, sec);

		/*
		 * because text area 3 is exactly 8x1 characters in size
		 * and that is what we print every time,
		 * there is no need to position or clear the area
		 * The new text will force out the old text.
		 */
		t3.Printf("%02d:%02d.%02d", hr, min, sec);

		/*
		 * You can also use the Arduino print class stuff as well.
		 * If you do a println the library defers the newline until the next character.
		 * So if you have a sigle line text area, it is automatically cleared when
		 * the next character is printed.
		 */

		/*
		 * This creates the same formatted output as the printf above but
		 * uses the Arduino Print class functions instead.
		 */
		if(hr < 9)
			t4.print(0);
		t4.print(hr);
		t4.print(":");
		if(min < 9)
			t4.print(0);
		t4.print(min);
		t4.print(".");
		if(sec < 9)
			t4.print(0);
		t4.println(sec);
#endif
		sec++; // not really seconds but animates display
		if(sec > 59)
		{
			sec = 0;
			min++;
			if(min>59)
			{
				min = 0;
				hr++;
				if(hr>11)
					hr = 0;
			}
		}
#else
		/*
		 * For small displays, print a simple 0 filled counter
 		 */
		t1.CursorTo(0,0);
		t1.Printf("%04d", counter);
		counter++;
		delay(100);
#endif
	}
}
