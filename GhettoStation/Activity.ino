//ACTIVITY

void check_activity() {
    if (activityMetro.check() == 1) 
      {
 
        
   	  if (current_activity == "MENU") {
		display_menu();
                if (enter_button.holdTime() > 1000 && enter_button.held()) {//long press handled here for menu because of unkown race condition
                   displaymenu.back();
                }


	  }
          if (current_activity == "TRACK") {
             //TO DO 
          }
          
          if (current_activity == "SET_HOME") {
             //TO DO 
          }
          
          if (current_activity == "PAN_MINPWM") {
            servoconf_tmp[0] = config_servo(1, 1, servoconf_tmp[0] );
            if (servoconf_tmp[0]<configuration.pan_minpwm) {
                  attach_servo(pan_servo, pan_servoEaser,PAN_SERVOPIN, servoconf_tmp[0], configuration.pan_maxpwm);
                  } 
            pan_servo.writeMicroseconds(servoconf_tmp[0]);
            
            if (enter_button.holdTime() >= 1000 && enter_button.held()) {//long press handled here for menu because of unkown race condition
               configuration.pan_minpwm = servoconf_tmp[0];
               EEPROM_write(0, configuration);
               attach_servo(pan_servo,pan_servoEaser, PAN_SERVOPIN, configuration.pan_minpwm, configuration.pan_maxpwm);
               btholdstate = false;
               current_activity="MENU";
                }
          }
          if (current_activity == "PAN_MINANGLE") {
             config_servo(1, 2, configuration.pan_minangle );
             pan_servoEaser.easeTo(0,100);
             if (enter_button.holdTime() >= 1000 && enter_button.held()) {//long press handled here for menu because of unkown race condition
               EEPROM_write(0, configuration);
               btholdstate = false;
               current_activity="MENU";
                }
             
          }
          if (current_activity == "PAN_MAXPWM") {
             servoconf_tmp[1] = config_servo(1, 3, servoconf_tmp[1] );
             if (servoconf_tmp[1]>configuration.pan_maxpwm) {
                  attach_servo(pan_servo, pan_servoEaser,PAN_SERVOPIN, configuration.pan_minpwm, servoconf_tmp[1]);
                  } 
             pan_servo.writeMicroseconds(servoconf_tmp[1]);
             if (enter_button.holdTime() >= 1000 && enter_button.held()) {//long press handled here for menu because of unkown race condition
               configuration.pan_maxpwm = servoconf_tmp[1];
               EEPROM_write(0, configuration);
               attach_servo(pan_servo,pan_servoEaser, PAN_SERVOPIN, configuration.pan_minpwm, configuration.pan_maxpwm);
               btholdstate = false;
               current_activity="MENU";
                }
          }
          
          if (current_activity == "PAN_MAXANGLE") {
             config_servo(1, 4, configuration.pan_maxangle );
             pan_servoEaser.easeTo(180,100);
             if (enter_button.holdTime() >= 1000 && enter_button.held()) {//long press handled here for menu because of unkown race condition
               EEPROM_write(0, configuration);
               btholdstate = false;
               current_activity="MENU";
                }
          }
          
          if (current_activity == "TILT_MINPWM") {
	     servoconf_tmp[2] = config_servo(2, 1, servoconf_tmp[2] );
             if (servoconf_tmp[2]<configuration.tilt_minpwm) {
              attach_servo(tilt_servo,tilt_servoEaser,TILT_SERVOPIN, servoconf_tmp[2], configuration.tilt_maxpwm); 
              }
             tilt_servo.writeMicroseconds(servoconf_tmp[2]); 
             if (enter_button.holdTime() >= 1000 && enter_button.held()) {//long press handled here for menu because of unkown race condition
               configuration.tilt_minpwm = servoconf_tmp[2];
               EEPROM_write(0, configuration);
	       attach_servo(tilt_servo,tilt_servoEaser, TILT_SERVOPIN, configuration.tilt_minpwm, configuration.tilt_maxpwm);
               btholdstate = false;
               current_activity="MENU";
                }
          }
          
          if (current_activity == "TILT_MINANGLE") {
             config_servo(2, 2, configuration.tilt_minangle ); 
             tilt_servoEaser.easeTo(0,100);
             if (enter_button.holdTime() >= 1000 && enter_button.held()) {//long press handled here for menu because of unkown race condition
               EEPROM_write(0, configuration);
               btholdstate = false;
               current_activity="MENU";
                }
          }
          
          if (current_activity == "TILT_MAXPWM") {
             servoconf_tmp[3] = config_servo(2, 3, servoconf_tmp[3] );
             if (servoconf_tmp[3]>configuration.tilt_maxpwm) {
              attach_servo(tilt_servo,tilt_servoEaser,TILT_SERVOPIN, configuration.tilt_minpwm, servoconf_tmp[3]); 
              }
             tilt_servo.writeMicroseconds(servoconf_tmp[3]);
             if (enter_button.holdTime() >= 1000 && enter_button.held()) {//long press handled here for menu because of unkown race condition
               configuration.tilt_maxpwm = servoconf_tmp[3];
               EEPROM_write(0, configuration);
	       attach_servo(tilt_servo,tilt_servoEaser, TILT_SERVOPIN, configuration.tilt_minpwm, configuration.tilt_maxpwm);
               btholdstate = false;
               current_activity="MENU";
                }
          }
          
          if (current_activity == "TILT_MAXANGLE") {
             config_servo(2, 4, configuration.tilt_maxangle );
             tilt_servoEaser.easeTo(180,100);
             if (enter_button.holdTime() >= 1000 && enter_button.held()) {//long press handled here for menu because of unkown race condition
               EEPROM_write(0, configuration);
               btholdstate = false;
               current_activity="MENU";
                }
          }
          if (current_activity == "TEST_SERVO") {
             //TO DO 
          }
          if (current_activity == "SET_RATE") {
             //TO DO 
          }
      }
}
