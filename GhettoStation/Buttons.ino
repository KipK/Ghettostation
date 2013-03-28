//BUTTONS FUNCTIONS 


// Buttons Callback functions

// enter button
void enterButtonReleaseEvents(Button &btn)
 {
     Serial.print("enter click");
     //Serial.println(current_activity);  
     if ( !btholdstate ) { // normal press
       
        if ( current_activity == "MENU" ) { //button action depends activity state
            Serial.println("MENU SELECT");
            displaymenu.select();
           }
        else {
              //do nothing yet
          }
        }
      else  if ( btholdstate ){          //long press
            if ( current_activity == "MENU" ) {          
               Serial.println("MENU BACK");
               btholdstate = false;
               displaymenu.back();
               
            }
            else if ( current_activity != "MENU" && current_activity != "TRACKING" && current_activity != "SET_HOME" ) {
              //We're in a setting area so ENTER & ENTER long press means save setting & get back to menu.
               Serial.println("SAVE SETTINGS");   
       
               EEPROM_write(0, configuration); 
               btholdstate = false;
               current_activity="MENU";
               
            }
 
           else if (current_activity == "TRACKING" || current_activity == "SET_HOME") {
                btholdstate = false;
                current_activity="MENU";
                
           }
           else btholdstate = false;
           
      }

 }

void enterButtonHoldEvents(Button &btn)   //Hold
{
  Serial.println("enter hold");
  //button action depends activity state
   // if (current_activity=="MENU" ) {
        btholdstate = true;

   // } 
    
}

// left button
void leftButtonClickEvents(Button &btn)
{

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


void leftButtonHoldEvents(Button &btn)   //Hold
{
//  if ( current_activity != "MENU" && current_activity != "TRACKING" && current_activity != "SET_HOME" ) {
//           while (!digitalRead(LEFT_BUTTON_PIN)) {
//              if (current_activity == "PAN_MINPWM") configuration.pan_minpwm--;		 
//              if (current_activity == "PAN_MINANGLE") configuration.pan_minangle--;
//              if (current_activity == "PAN_MAXPWM") configuration.pan_maxpwm--;
//              if (current_activity == "PAN_MAXANGLE") configuration.pan_maxangle--;
//              if (current_activity == "TILT_MINPWM") configuration.tilt_minpwm--;
//              if (current_activity == "TILT_MINANGLE") configuration.tilt_minangle--;        
//              if (current_activity == "TILT_MAXPWM") configuration.tilt_maxpwm--;
//              if (current_activity == "TILT_MAXANGLE") configuration.tilt_maxangle--;
//              d_refresh_lcd();
//              delay(200);
//            }	
//    }
}


//right button
void rightButtonClickEvents(Button &btn)
{

     
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


void rightButtonHoldEvents(Button &btn)   //Hold
{
//  if ( current_activity != "MENU" && current_activity != "TRACKING" && current_activity != "SET_HOME" ) {
//           while (!digitalRead(RIGHT_BUTTON_PIN)) {
//              if (current_activity == "PAN_MINPWM") configuration.pan_minpwm++;		 
//              if (current_activity == "PAN_MINANGLE") configuration.pan_minangle++;
//              if (current_activity == "PAN_MAXPWM") configuration.pan_maxpwm++;
//              if (current_activity == "PAN_MAXANGLE") configuration.pan_maxangle++;
//              if (current_activity == "TILT_MINPWM") configuration.tilt_minpwm++;
//              if (current_activity == "TILT_MINANGLE") configuration.tilt_minangle++;        
//              if (current_activity == "TILT_MAXPWM") configuration.tilt_maxpwm++;
//              if (current_activity == "TILT_MAXANGLE") configuration.tilt_maxangle++;
//              d_refresh_lcd();
//              delay(200);
//            }	
//  }
}


