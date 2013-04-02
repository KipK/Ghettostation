//SERVO FUNCTIONS

void attach_servo(Servo &s, int p, int min, int max) {
// called at setup() or after a servo configuration change in the menu
	if (s.attached()) {
	s.detach();
	}
	s.attach(p,min,max);
        //e.begin( s, SERVO_REFRESH_INTERVAL );
        //e.useMicroseconds(true);
        //e.setMinMaxMicroseconds( min, max );
        //#if defined(PAN_SERVOREVERSED) || defined(TILT_SERVOREVERSED)
        //e.setFlipped(true);
        //#endif
        // make ServoEaser use microsecond
        
}


void move_servo(Servo &s, int stype, float a, int mina, int maxa) {

 
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
				#ifdef PAN_SERVOREVERSED
				new_angle = 180 - new_angle
				#endif
                
	 }
  else if (stype == 2){
                
		new_angle = map(a, configuration.tilt_minangle, configuration.tilt_maxangle, 0, 180); //map configured tilt settings to default arduino lib 0-180° servo range
		#ifdef PAN_SERVOREVERSED
		new_angle = 180 - new_angle
		#endif
	}
	
	s.write( new_angle);
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

	move_servo(pan_servo, 1, angle_b, configuration.pan_minangle, configuration.pan_maxangle);
	move_servo(tilt_servo, 2, angle_a, configuration.tilt_minangle, configuration.tilt_maxangle);
}



// SERVO CONFIGURATION

int config_servo(int servotype, int valuetype, int value ) {
	// servo configuration screen function return configured value
        String currentline;
        String param = String(value);
        char string_buffer[21];
        
	if (servotype==1) {
	  store_lcdline(1,"    [PAN SERVO]     ");
	}
	else if (servotype==2) {
	  store_lcdline(1,"    [TILT SERVO]    ");
	}
	store_lcdline(2,"                    ");
	if (valuetype==1) {	//minpwm
          currentline = "min endpoint: <" + param;
          currentline += ">";
	}
	else if (valuetype==2) { //minangle
          currentline = "min angle: <" + param;
          currentline += ">    ";
	}
	else if (valuetype==3) {	//maxpwm
	  currentline = "max endpoint: <" + param;
          currentline += ">";
	}
	else if (valuetype==4) { //maxangle
	  currentline = "max angle: <" + param;
          currentline += ">";
	}
        currentline.toCharArray(string_buffer,21);
	store_lcdline(3,string_buffer);
	store_lcdline(4," Long press to quit ");
        //checking long press left righthere
           if (right_button.heldFor(800)) {
            value++;
            }
            else if (left_button.heldFor(800)) {
            value--;
            }
return value;
           
}

