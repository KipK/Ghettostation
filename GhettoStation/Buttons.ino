//BUTTONS FUNCTIONS 


// Buttons Callback functions

// enter button
void enterButtonReleaseEvents(Button &btn)
 {
     //Serial.print("enter click");
     //Serial.println(current_activity);  
     if ( enter_button.holdTime() < 1000 ) { // norman press
       
        if ( current_activity == "MENU" ) { //button action depends activity state;
            displaymenu.select();
           }

        else if ( current_activity == "SET_HOME" ) {
            if (gps_fix && !home_pos) {
              //saving home position
              home_lat = uav_lat;
              home_lon = uav_lon;
              home_alt = uav_alt;
            }
            
            else if (gps_fix && home_pos && (!home_bear)) {
             // saving home bearing 
             home_bearing = calc_bearing(home_lon, home_lat, uav_lon, uav_lat); // storing bearing relative to north
              
            }
            else if (gps_fix && home_pos && (home_bear)) {
              // START TRACKING 

            }
        } 
     }
     else {    //Long Press Enter

           }
           
}


// left button
void leftButtonReleaseEvents(Button &btn)
{
  if ( left_button.holdTime() < 1000 ) {

  //Serial.println("left click");
  //button action depends activity state
    if (current_activity=="MENU") {
        Serial.println("MENU LEFT");
        displaymenu.prev();
    }
    
    else if ( current_activity != "MENU" && current_activity != "TRACKING" && current_activity != "SET_HOME" ) {
              //We're in a setting area: Left button decrase current value.
          if (current_activity == "PAN_MINPWM") servoconf_tmp[0]--;		 
          if (current_activity == "PAN_MINANGLE") configuration.pan_minangle--;
          if (current_activity == "PAN_MAXPWM") servoconf_tmp[1]--;
          if (current_activity == "PAN_MAXANGLE") configuration.pan_maxangle--;
          if (current_activity == "TILT_MINPWM") servoconf_tmp[2]--;
          if (current_activity == "TILT_MINANGLE") configuration.tilt_minangle--;        
          if (current_activity == "TILT_MAXPWM") servoconf_tmp[3]--;
          if (current_activity == "TILT_MAXANGLE") configuration.tilt_maxangle--;
    }
    
   else if (current_activity=="SET_HOME") {
               if (gps_fix && home_pos && (home_bear)) {
                  current_activity = "MENU";
                }
          }
    }
}


//right button
void rightButtonReleaseEvents(Button &btn)
{
  if ( right_button.holdTime() < 1000 ) {
     
    if (current_activity=="MENU") {
        Serial.println("MENU RIGHT");
        displaymenu.next();
    }
    else if ( current_activity != "MENU" && current_activity != "TRACKING" && current_activity != "SET_HOME" ) {
              //We're in a setting area: Right button decrase current value.
          if (current_activity == "PAN_MINPWM") servoconf_tmp[0]++;		 
          if (current_activity == "PAN_MINANGLE") configuration.pan_minangle++;
          if (current_activity == "PAN_MAXPWM") servoconf_tmp[1]++;
          if (current_activity == "PAN_MAXANGLE") configuration.pan_maxangle++;
          if (current_activity == "TILT_MINPWM") servoconf_tmp[2]++;
          if (current_activity == "TILT_MINANGLE") configuration.tilt_minangle++;        
          if (current_activity == "TILT_MAXPWM") servoconf_tmp[3]++;
          if (current_activity == "TILT_MAXANGLE") configuration.tilt_maxangle++;
    }
    
    if (current_activity=="SET_HOME") {
           if (gps_fix && home_pos && (home_bear)) {
              // reset home pos
              home_pos = false;
              home_bearing == false; 
           }
    }
  }
}



