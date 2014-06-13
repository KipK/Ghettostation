/*
 * AnalogClock.h - Analog clock display for Graphics LCD libraries
*/
#ifndef AnalogClock_h
#define AnalogClock_h

#if ARDUINO < 100
#include <WProgram.h>
#else
#include <Arduino.h>
#endif


class AnalogClock
{
private:
/* Sizes and positions of clock */
byte x_centre      ; /* x-coordinate of clock centre                  */
byte y_centre      ; /* y-coordinate of clock centre                  */
byte radius        ; /* radius of clock face                          */
byte radius_aspect ; /* radius of clock face adjuted for aspect ratio */
byte l_hour        ; /* length of hour hand                           */
byte l_minute      ; /* length of minute hand                         */
byte l_second      ; /* length of second hand                         */

/* Previous positions of hour and minute hands */
byte PX_Hour   ; /* previous x-coordinate of hour hand   */
byte PY_Hour   ; /* previous y-coordinate of hour hand   */
byte PX_Minute ; /* previous x-coordinate of minute hand */
byte PY_Minute ; /* previous y-coordinate of minute hand */
byte PX_Second ; /* previous x-coordinate of second hand */
byte PY_Second ; /* previous y-coordinate of second hand */

/* Positions of digital clock and date, and time offset */
byte DigRow     ; /* row position of digital clock    */
byte DigColumn  ; /* column position of digital clock */
byte Offset     ; /* time offset from system clock    */
byte Hours      ; /* current hours                    */
byte Minutes    ; /* current minutes                  */
byte Seconds    ; /* current seconds                  */ 
byte DateRow    ; /* row position of date string      */
byte DateColumn ; /* column position of date string   */

  void CalcHands( byte angle, byte radius, byte *x, byte *y );
  void DrawFace();
  void Box( byte x, byte y );
  void SegBox( byte FaceAngle );
public:		// Public Methods
  AnalogClock();
  void Init(byte x, byte y, byte r); //  x & y are the coordinates of the center of the clock, r is the radius of the clock face
  void DisplayTime( byte hours, byte minutes, byte seconds = -1 ); // draw hands in the position for the given hours and minutes 
};

#endif
