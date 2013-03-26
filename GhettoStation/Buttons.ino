//BUTTONS FUNCTIONS 


// Buttons Callback functions

// enter button
void enterButtonClickEvents(Button &btn)
{
  Serial.println("enter click");
  //button action depends activity state
  if ( current_activity == "MENU" ) {
      if ( !btholdstate ) {
        Serial.println("MENU SELECT");
        displaymenu.select();
      }
      else {
        Serial.println("MENU BACK");
       displaymenu.back();
       btholdstate = false;
      }
    }
  
}

void enterButtonHoldEvents(Button &btn)   //Hold
{
  //Serial.println("enter hold");
  //button action depends activity state
    if (current_activity=="MENU") {
        btholdstate = true;   
    }
    
}

// left button
void leftButtonClickEvents(Button &btn)
{
  Serial.println("left click");
  //button action depends activity state
    if (current_activity=="MENU") {
        Serial.println("MENU LEFT");
        displaymenu.prev();
    }
}

void leftButtonHoldEvents(Button &btn)   //Hold
{
  Serial.println("left hold");
  //button action depends activity state

}


//right button
void rightButtonClickEvents(Button &btn)
{
    if (current_activity=="MENU") {
        Serial.println("MENU RIGHT");
        displaymenu.next();
    }
  Serial.println("right click");
  
}

void rightButtonHoldEvents(Button &btn)   //Hold
{
  Serial.println("right hold");
}


