//ACTIVITY

void check_activity() {
    if (activityMetro.check() == 1) 
      {
 
        
   	  if (current_activity == "MENU") {
		display_menu();
          


	  }
          if (current_activity == "TRACK") {
             //TO DO 
          }
          
          if (current_activity == "SET_HOME") {
             //TO DO 
          }
          
          if (current_activity == "PAN_MINPWM") {
             config_servo(1, 1, configuration.pan_minpwm );	 
          }
          if (current_activity == "PAN_MINANGLE") {
             config_servo(1, 2, configuration.pan_minangle );
             
          }
          if (current_activity == "PAN_MAXPWM") {
             config_servo(1, 3, configuration.pan_maxpwm );
          }
          
          if (current_activity == "PAN_MAXANGLE") {
             config_servo(1, 4, configuration.pan_maxangle );
          }
          
          if (current_activity == "TILT_MINPWM") {
	     config_servo(2, 1, configuration.tilt_minpwm );
          }
          
          if (current_activity == "TILT_MINANGLE") {
             config_servo(2, 2, configuration.tilt_minangle ); 
          }
          
          if (current_activity == "TILT_MAXPWM") {
             config_servo(2, 3, configuration.tilt_maxpwm );
          }
          
          if (current_activity == "TILT_MAXANGLE") {
             config_servo(2, 4, configuration.tilt_maxangle );
          }
          if (current_activity == "TEST_SERVO") {
             //TO DO 
          }
          if (current_activity == "SET_RATE") {
             //TO DO 
          }
      }
}
