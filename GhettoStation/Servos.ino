//SERVO FUNCTIONS

void attach_servo(Servo &s, int p) {
// called at setup() or after a servo configuration change in the menu
	if (s.attached()) {
	s.detach();
	}
	s.attach(p);
        
}


void move_servo(ServoEaser &s, int stype, float a, int minp, int mina, int maxp, int maxa) {

 
 float new_angle;
 int t;
 //map servo range according to configured min max angle values
 if (stype == 1) {
		//convert angle for pan to pan servo reference point: 0° is pan_minangle
		if (a<=180) {
			a = mina + a;
		} else if ((a>180) && (a<360-mina)) {
                        //relevant only for 360° configs
			a = a - mina;
                }
                else if ((a>180) && (a>360-mina)) {
                          a = mina - (360-a);
                     
		}
                // map configured pan settings to default arduino lib 0-180° servo range
                new_angle = map (a, 0, mina+maxa, 0, 180); 
                
	 }
  else if (stype == 2){
                
		new_angle = map(a, configuration.tilt_minangle, configuration.tilt_maxangle, 0, 180); //map configured tilt settings to default arduino lib 0-180° servo range

	}
 //assign servo move duration to be proportional to requested course travel ( from 20ms to 800ms )
	 if ((new_angle - s.getCurrPos()) >= 0) {
		t = map(new_angle - s.getCurrPos(), 0, 180, 20,800);
	} else {
		t = map(s.getCurrPos() - new_angle, 0, 180, 20,800);
		}
	s.easeTo( new_angle, t);
}

void servoPathfinder(float angle_b, float angle_a){
//find the best way to move pan servo considering 0° reference face toward
	if (angle_b<=180) {
			if ( configuration.pan_maxangle >= angle_b ) {
			//works for all config
                                        //defining limits
					if (angle_a < configuration.tilt_minangle) {
					 // checking if we reach the min tilt limit
						angle_a = configuration.tilt_minangle;
					}
                                        else if (angle_a >configuration.tilt_maxangle) {
                                         //shouldn't happend but just in case
                                              angle_a = configuration.tilt_maxangle; 
                                        }

			}
			 else if ( configuration.pan_maxangle < angle_b ) {
                         //relevant for 180° tilt config only, in case bearing is superior to pan_maxangle
				
				angle_b = 360-(180-angle_b);
                                if (angle_b >= 360) {
                                   angle_b = angle_b - 360;
                                }


                                // invert pan axis 
				if ( configuration.tilt_maxangle >= ( 180-angle_a )) {
					// invert pan & tilt for 180° Pan 180° Tilt config

					angle_a = 180-angle_a;
					
				}
				
				else if (configuration.tilt_maxangle < ( 180-angle_a )) {
				 // staying at nearest max pos
				 angle_a = configuration.tilt_maxangle;
				}
				
				
			 }
	}
	
	else if ( angle_b > 180 )
		if( configuration.pan_minangle > 360-angle_b ) {
		//works for all config
			if (angle_a < configuration.tilt_minangle) {
			// checking if we reach the min tilt limit
						angle_a = configuration.tilt_minangle;
			}
		}
		else if ( configuration.pan_minangle <= 360-angle_b ) {
			angle_b = angle_b - 180;
			if ( configuration.tilt_maxangle >= ( 180-angle_a )) {
				// invert pan & tilt for 180/180 conf
				angle_a = 180-angle_a;
				}
			else if (configuration.tilt_maxangle < ( 180-angle_a)) {
				// staying at nearest max pos
				angle_a = configuration.tilt_maxangle;
		}
	}

	move_servo(pan_servoEaser, 1, angle_b, configuration.pan_minpwm, configuration.pan_minangle, configuration.pan_maxpwm, configuration.pan_maxangle);
	move_servo(tilt_servoEaser, 2, angle_a, configuration.tilt_minpwm, configuration.tilt_minangle, configuration.tilt_maxpwm, configuration.tilt_maxangle);
}


