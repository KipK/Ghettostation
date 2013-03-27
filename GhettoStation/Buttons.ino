//BUTTONS FUNCTIONS 


// Buttons Callback functions

// enter button
void enterButtonClickEvents(Button &btn)
 {
     Serial.print("cur_activity: ");
     Serial.println(current_activity);
     Menu const* displaymenu_current = displaymenu.get_current_menu();  
     Serial.print("cur_menu_comp_num: ");
     Serial.println(displaymenu_current->get_cur_menu_component_num());  
     if ( !btholdstate ) { // normal press
       
        if ( current_activity == "MENU" ) { //button action depends activity state
            Serial.println("MENU SELECT");
            displaymenu.select();
           }
        else {
              //do nothing
          }
        }
      else  {          //long press
            if ( current_activity == "MENU" ) {          
               Serial.println("MENU BACK");
               displaymenu.back();
               btholdstate = false;
            }
            else if ( current_activity != "MENU" && current_activity != "TRACKING" && current_activity != "SET_HOME" ) {
              //We're in a setting area so ENTER & ENTER long press means save setting & get back to menu.
               Serial.println("SAVE SETTINGS");   
       
               EEPROM_write(0, configuration); 
               //displaymenu.back();
               current_activity="MENU";
               //display_menu();
               btholdstate = false;
            }
 
           else if (current_activity == "TRACKING" || current_activity == "SET_HOME") {
                //displaymenu.back();
                current_activity="MENU";
                btholdstate = false;
           }
           else btholdstate = false;
      }  
 }

void enterButtonHoldEvents(Button &btn)   //Hold
{
  //Serial.println("enter hold");
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
     Menu const* displaymenu_current = displaymenu.get_current_menu();  
     Serial.print("cur_menu_comp_num: ");
     Serial.println(displaymenu_current->get_cur_menu_component_num()); 
    }
}

void leftButtonHoldEvents(Button &btn)   //Hold
{
  //Serial.println("left hold");
  //button action depends activity state

}


//right button
void rightButtonClickEvents(Button &btn)
{

     
    if (current_activity=="MENU") {
        Serial.println("MENU RIGHT");
        displaymenu.next();
      Menu const* displaymenu_current = displaymenu.get_current_menu();  
     Serial.print("cur_menu_comp_num: ");
     Serial.println(displaymenu_current->get_cur_menu_component_num()); 
    }
  //Serial.println("right click");
  
}

void rightButtonHoldEvents(Button &btn)   //Hold
{
  //Serial.println("right hold");
}


