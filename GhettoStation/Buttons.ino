//BUTTONS FUNCTIONS 


// Buttons Callback functions

// enter button
void enterButtonReleaseEvents(Button &btn)
 {
     //Serial.println(current_activity);  
     if ( enter_button.holdTime() < 1000 ) { // norman press
       
        if ( current_activity == 0 ) { //button action depends activity state
            displaymenu.select();
           }
         else if ( current_activity == 2 ) {
            if ((gps_fix) && (!home_pos)) {
              //saving home position
              home_lat = uav_lat;
              home_lon = uav_lon;
              home_alt = uav_alt;
              home_pos = true;
            }
            
            else if ((gps_fix) && (home_pos) && (!home_bear)) {
             // saving home bearing 
             home_bearing = calc_bearing(home_lon, home_lat, uav_lon, uav_lat); // storing bearing relative to north
             
             home_bear = true;
              
            }
            else if ((gps_fix) && (home_pos) && (home_bear)) {
              // START TRACKING 
              current_activity = 1;

            }
        }
     }
     
 }


// left button
void leftButtonReleaseEvents(Button &btn)
{
  if ( left_button.holdTime() < 1000 ) {

    if (current_activity==0) {
        displaymenu.prev();
    }
    
    else if ( current_activity != 0 && current_activity != 1 && current_activity != 2 ) {
              //We're in a setting area: Left button decrase current value.
          if (current_activity == 3) servoconf_tmp[0]--;		 
          if (current_activity == 4) configuration.pan_minangle--;
          if (current_activity == 5) servoconf_tmp[1]--;
          if (current_activity == 6) configuration.pan_maxangle--;
          if (current_activity == 7) servoconf_tmp[2]--;
          if (current_activity == 8) configuration.tilt_minangle--;        
          if (current_activity == 9) servoconf_tmp[3]--;
          if (current_activity == 10) configuration.tilt_maxangle--;
    }
    else if (current_activity==2) {
               if (gps_fix && home_pos && home_bear) {
                  current_activity = 0;
                }
    }
  }
}


//right button
void rightButtonReleaseEvents(Button &btn)
{
  if ( right_button.holdTime() < 1000 ) {
     
    if (current_activity==0) {
        displaymenu.next();
    }
    else if ( current_activity != 0 && current_activity != 1 && current_activity != 2 ) {
              //We're in a setting area: Right button decrase current value.
          if (current_activity == 3) servoconf_tmp[0]++;		 
          if (current_activity == 4) configuration.pan_minangle++;
          if (current_activity == 5) servoconf_tmp[1]++;
          if (current_activity == 6) configuration.pan_maxangle++;
          if (current_activity == 7) servoconf_tmp[2]++;
          if (current_activity == 8) configuration.tilt_minangle++;        
          if (current_activity == 9) servoconf_tmp[3]++;
          if (current_activity == 10) configuration.tilt_maxangle++;
    }
    else if (current_activity==2) {
           if (gps_fix && home_pos && (home_bear)) {
              // reset home pos
              home_pos = false;
              home_bearing = false; 
           }
    }
  }
}



