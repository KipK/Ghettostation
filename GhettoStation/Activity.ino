//ACTIVITY

void check_activity() {
    
    if (activityMetro.check() == 1) 
      if (uav_fix_type == 3) { gps_fix = true; }
    
      {
 
        
   	  if (current_activity == 0) { //MENU
		display_menu();
                if (enter_button.holdTime() >= 1000 && enter_button.held()) { //long press 
                   displaymenu.back();
                }


	  }
          if (current_activity == 1 ) { //TRACK
           if ((!home_pos) || (!home_bear)) {  // check if home is set before start tracking
             current_activity = 2;
           } else if (home_bear) {
             antenna_tracking();
             lcddisp_tracking();
                if (enter_button.holdTime() >= 1000 && enter_button.held()) { //long press 
                   current_activity = 0;
                }
           }
          }
          
          if (current_activity == 2) { //SET HOME
            if (!home_pos) { lcddisp_sethome(); }
            else if (home_pos) {

              if (!home_bear) { 
              #ifndef BEARING_METHOD_3
              lcddisp_setbearing();   
              #else
              home_bearing = 0;
              #endif
            }
              else { lcddisp_homeok(); }
            }
            if (enter_button.holdTime() >= 1000 && enter_button.held()) { //long press 
                   current_activity = 0;
                }
          }
          
          if (current_activity == 3) { //PAN_MINPWM
            servoconf_tmp[0] = config_servo(1, 1, servoconf_tmp[0] );
            //if (servoconf_tmp[0]<configuration.pan_minpwm) {
                  attach_servo(pan_servo, PAN_SERVOPIN, servoconf_tmp[0], configuration.pan_maxpwm);
            //      } 
            //pan_servo.writeMicroseconds(servoconf_tmp[0]);
            pan_servo.write(0);
            
            if (enter_button.holdTime() >= 1000 && enter_button.held()) {//long press 
               configuration.pan_minpwm = servoconf_tmp[0];
               EEPROM_write(0, configuration);
               attach_servo(pan_servo, PAN_SERVOPIN, configuration.pan_minpwm, configuration.pan_maxpwm);
               move_servo(pan_servo, 1, 0, configuration.pan_minangle, configuration.pan_maxangle);
               current_activity=0;
                }
          }
          if (current_activity == 4) { //PAN_MINANGLE
             configuration.pan_minangle = config_servo(1, 2, configuration.pan_minangle );
            //pan_servo.writeMicroseconds(configuration.pan_minpwm);
            pan_servo.write(0);
             if (enter_button.holdTime() >= 1000 && enter_button.held()) {//long press
               EEPROM_write(0, configuration);
               move_servo(pan_servo, 1, 0, configuration.pan_minangle, configuration.pan_maxangle);
               current_activity=0;
                }
             
          }
          if (current_activity == 5) { //PAN_MAXPWM
             servoconf_tmp[1] = config_servo(1, 3, servoconf_tmp[1] );
            // if (servoconf_tmp[1]>configuration.pan_maxpwm) {
                  attach_servo(pan_servo,PAN_SERVOPIN, configuration.pan_minpwm, servoconf_tmp[1]);
            //      } 
            // pan_servo.writeMicroseconds(servoconf_tmp[1]);
            pan_servo.write(180);
             if (enter_button.holdTime() >= 1000 && enter_button.held()) {//long press
               configuration.pan_maxpwm = servoconf_tmp[1];
               EEPROM_write(0, configuration);
               attach_servo(pan_servo, PAN_SERVOPIN, configuration.pan_minpwm, configuration.pan_maxpwm);
               move_servo(pan_servo, 1, 0, configuration.pan_minangle, configuration.pan_maxangle);
               current_activity=0;
                }
          }
          
          if (current_activity == 6) { //PAN_MAXANGLE
             configuration.pan_maxangle = config_servo(1, 4, configuration.pan_maxangle );
            //pan_servo.writeMicroseconds(configuration.pan_maxpwm);
             pan_servo.write(180);
             if (enter_button.holdTime() >= 1000 && enter_button.held()) {//long press
               EEPROM_write(0, configuration);
               move_servo(pan_servo, 1, 0, configuration.pan_minangle, configuration.pan_maxangle);
               current_activity=0;
                }
          }
          
          if (current_activity == 7) { //"TILT_MINPWM"
	     servoconf_tmp[2] = config_servo(2, 1, servoconf_tmp[2] );
             //if (servoconf_tmp[2]<configuration.tilt_minpwm) {
              attach_servo(tilt_servo, TILT_SERVOPIN, servoconf_tmp[2], configuration.tilt_maxpwm); 
              //}
             //tilt_servo.writeMicroseconds(servoconf_tmp[2]); 
             tilt_servo.write(0);
             if (enter_button.holdTime() >= 1000 && enter_button.held()) {//long press
               configuration.tilt_minpwm = servoconf_tmp[2];
               EEPROM_write(0, configuration);
	       attach_servo(tilt_servo,TILT_SERVOPIN, configuration.tilt_minpwm, configuration.tilt_maxpwm);
               move_servo(tilt_servo, 2, 0, configuration.tilt_minangle, configuration.tilt_maxangle);;
               current_activity=0;
                }
          }
          
          if (current_activity == 8) { //TILT_MINANGLE
             configuration.tilt_minangle = config_servo(2, 2, configuration.tilt_minangle ); 
             //tilt_servo.writeMicroseconds(configuration.tilt_minpwm);
             tilt_servo.write(0);
             if (enter_button.holdTime() >= 1000 && enter_button.held()) {//long press
               EEPROM_write(0, configuration);
               move_servo(tilt_servo, 2, 0, configuration.tilt_minangle, configuration.tilt_maxangle);
               current_activity=0;
                }
          }
          
          if (current_activity == 9) { //"TILT_MAXPWM"
             servoconf_tmp[3] = config_servo(2, 3, servoconf_tmp[3] );
             //if (servoconf_tmp[3]>configuration.tilt_maxpwm) {
              attach_servo(tilt_servo, TILT_SERVOPIN, configuration.tilt_minpwm, servoconf_tmp[3]); 
              //}
             //tilt_servo.writeMicroseconds(servoconf_tmp[3]);
             tilt_servo.write(180);
             if (enter_button.holdTime() >= 1000 && enter_button.held()) {//long press
               configuration.tilt_maxpwm = servoconf_tmp[3];
               EEPROM_write(0, configuration);
	       attach_servo(tilt_servo,TILT_SERVOPIN, configuration.tilt_minpwm, configuration.tilt_maxpwm);
               move_servo(tilt_servo, 2, 0, configuration.tilt_minangle, configuration.tilt_maxangle);
               current_activity=0;
                }
          }
          
          if (current_activity == 10) { //TILT_MAXANGLE
             configuration.tilt_maxangle = config_servo(2, 4, configuration.tilt_maxangle );
             //tilt_servo.writeMicroseconds(configuration.tilt_maxpwm);
             tilt_servo.write(180);
             if (enter_button.holdTime() >= 1000 && enter_button.held()) {//long press
               EEPROM_write(0, configuration);
               move_servo(tilt_servo, 2, 0, configuration.tilt_minangle, configuration.tilt_maxangle);
               current_activity=0;
                }
          }
          if (current_activity == 11) { //TEST_SERVO
             test_servos();
             current_activity = 0; 
          }
      }
}
