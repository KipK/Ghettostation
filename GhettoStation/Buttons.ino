//BUTTONS FUNCTIONS 


// Buttons Callback functions

// enter button
void enterButtonReleaseEvents(Button &btn)
 {
     Serial.print("enter click");
     //Serial.println(current_activity);  
     if ( enter_button.holdTime() < 1000 ) { // norman press
       
        if ( current_activity == "MENU" ) { //button action depends activity state
            Serial.println("MENU SELECT");
            displaymenu.select();
           }
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
          if (current_activity == "PAN_MINPWM") configuration.pan_minpwm--;		 
          if (current_activity == "PAN_MINANGLE") configuration.pan_minangle--;
          if (current_activity == "PAN_MAXPWM") configuration.pan_maxpwm--;
          if (current_activity == "PAN_MAXANGLE") configuration.pan_maxangle--;
          if (current_activity == "TILT_MINPWM") configuration.tilt_minpwm--;
          if (current_activity == "TILT_MINANGLE") configuration.tilt_minangle--;        
          if (current_activity == "TILT_MAXPWM") configuration.tilt_maxpwm--;
          if (current_activity == "TILT_MAXANGLE") configuration.tilt_maxangle--;
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
          if (current_activity == "PAN_MINPWM") configuration.pan_minpwm++;		 
          if (current_activity == "PAN_MINANGLE") configuration.pan_minangle++;
          if (current_activity == "PAN_MAXPWM") configuration.pan_maxpwm++;
          if (current_activity == "PAN_MAXANGLE") configuration.pan_maxangle++;
          if (current_activity == "TILT_MINPWM") configuration.tilt_minpwm++;
          if (current_activity == "TILT_MINANGLE") configuration.tilt_minangle++;        
          if (current_activity == "TILT_MAXPWM") configuration.tilt_maxpwm++;
          if (current_activity == "TILT_MAXANGLE") configuration.tilt_maxangle++;
    }
  }
}



