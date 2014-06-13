/*
 *  AnalogClock.cpp- Analog clock display for Graphics LCD libraries
 *  coded by Michael Margolis 2010
*/

#include "AnalogClock.h"
#include <glcd.h>  // this clock uses the Graphics LCD library for display

/* Look-up sine table for integer math */
byte byteSine[16] = {0,  27,  54,  79, 104, 128, 150, 171, 190, 201, 221, 233, 243, 250, 254, 255} ;

AnalogClock:: AnalogClock( )
{ 
}

void AnalogClock::Init(byte x, byte y, byte r)
{
  	/* calculate sizes and position of clock */
	x_centre = x ;
	y_centre = y ;
	radius = r ;
	l_hour =  r / 2 ; // hour hand is half radius
	l_minute = (r*3) / 4 ;  // minute hand is 3/4 radius
	l_second  = l_minute + 1;  // second hand is 2 pixels larger than minute


	/* Initialise previous positions of hour and minute hand */
	PX_Hour = PX_Minute = PX_Second = x_centre ;
	PY_Hour = PY_Minute = PY_Second = y_centre ;
        GLCD.DrawCircle(x,y, r + 3,BLACK); // draw the surrounding circle
	this->DrawFace() ; /* draw the clock dial       */

}

void AnalogClock::DisplayTime( byte hours, byte minutes, byte seconds )
/* draw the hands at the specified positions */
{
	byte angle ;

	if( hours == 0 )
           hours = 12 ;

	/* erase previous hands */
	GLCD.DrawLine( x_centre, y_centre, PX_Hour, PY_Hour, WHITE ) ;
	GLCD.DrawLine( x_centre, y_centre, PX_Minute, PY_Minute, WHITE ) ;
       // GLCD.SetDot(PX_Second, PY_Second, WHITE); 
        GLCD.DrawCircle(PX_Second, PY_Second,1, WHITE);

	/* calculate new position of minute hand and draw it */
	angle = minutes ;
	this->CalcHands( angle, l_minute, &PX_Minute, &PY_Minute ) ;
	GLCD.DrawLine( x_centre, y_centre, PX_Minute, PY_Minute, BLACK ) ;

	/* calculate new position of hour hand and draw it */
	angle = ( ( 5*hours ) + ( minutes/12 ) ) % 60 ;
	this->CalcHands( angle, l_hour, &PX_Hour, &PY_Hour ) ;
	GLCD.DrawLine( x_centre, y_centre, PX_Hour, PY_Hour, BLACK ) ;

	/* calculate new position of second hand and draw if requested */
        if( seconds != -1)   
        {
  	  this->CalcHands( seconds, l_second, &PX_Second, &PY_Second ) ;
	  //GLCD.SetDot( PX_Second, PY_Second, BLACK ) ;
          GLCD.DrawCircle(PX_Second, PY_Second,1, BLACK);
        }
        
	/* re-draw clock centre */
	this->Box( x_centre, y_centre ) ;
}

void AnalogClock::CalcHands( byte angle, byte radius, byte *x, byte *y )
/* angle is location of hand on dial (0-60)          */
/* radius is length of hand                           */
/* *x   return x-coordinate of hand on dial face */
/* *y   return y-coordinate of hand on dial face */
{
	short quadrant, x_flip, y_flip ;

	/* calculate which quadrant the hand lies in */
	quadrant = angle/15 ;

	/* setup for reflection or rotation */
	switch ( quadrant ) {
	  case 0 : x_flip = 1 ; y_flip = -1 ; break ;
	  case 1 : angle = abs(angle-30) ; x_flip = y_flip = 1 ; break ;
	  case 2 : angle = angle-30 ; x_flip = -1 ; y_flip = 1 ; break ;
	  case 3 : angle = abs(angle-60) ; x_flip = y_flip = -1 ; break ; 
	  default:  x_flip = y_flip =1; // this should not happen
	}
	*x = x_centre ;
	*y = y_centre ;
	*x += ( x_flip*(( byteSine[angle]*radius ) >> 8 )) ;
	*y += ( y_flip*(( byteSine[15-angle]*radius ) >> 8 )) ;
}

void AnalogClock::DrawFace()
/* draw clock face */
{
	byte hr, x, y ;

	this->Box( x_centre, y_centre ) ; // draw center box

	/* draw hour marks or numerals around the clock face */
	for( hr = 0; hr < 60; hr += 5 ) {
	  if ( !( hr % 15 ) )
		 this->SegBox( hr ) ; /* draw quarter hour shapes */
	  else{
		 this->CalcHands( hr, radius, &x, &y ) ;
	     this->Box( x, y ) ;
	  }
	}
}
void AnalogClock::Box( byte x, byte y  )
/* draw a 3 x 3 pixel box centered at x,y */
{
	byte color = BLACK;
	GLCD.DrawLine( x-1, y-1, x+1, y-1,color ) ;
	GLCD.DrawLine( x-1, y,   x+1, y, color ) ;
	GLCD.DrawLine( x-1, y+1, x+1, y+1,color ) ;	
}

void AnalogClock::SegBox( byte FaceAngle )
/* draw quarter hour dial markers */
/* FaceAngle is position of marker around face (0-55) */
{
	byte quadrant;
	byte hour; 

	/* find the nearest quadrant to the marker position */
	quadrant = ( ( FaceAngle + 7 ) / 15 ) ;
	hour =  FaceAngle == 0 ? 12 : FaceAngle  / 5;

	switch( quadrant ) {
	  case 0 :
	  case 4 :  // 12 oclock
		      GLCD.GotoXY(x_centre-6,  y_centre - radius -1 );		       
			   break;
	  case 1 : // 3 oclock
		      GLCD.GotoXY(x_centre + radius - 3,  y_centre -3 );
			   break;
	  case 2 : // 6 oclock
		       GLCD.GotoXY(x_centre-2,  y_centre + radius -5);
			   break;
	  case 3 : // 9 oclock
		     GLCD.GotoXY(x_centre- radius ,  y_centre -3 );
			   break;
	}
	 GLCD.PrintNumber(hour);
}
