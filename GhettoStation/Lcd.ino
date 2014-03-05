//LCD 

void init_lcdscreen() {
#ifdef DEBUG
    Serial.println("starting lcd"); 
#endif


  char extract[20];
// init LCD
	LCD.begin(20,4);
        delay(20);
        LCD.backlight(); 		
 	delay(250); 		
 	LCD.noBacklight(); 		
 	delay(250);
        LCD.backlight(); 
        delay(250);
        LCD.setCursor(0,0);
	LCD.print(string_load1.copy(extract));
        LCD.setCursor(0,1);
        LCD.print(string_load2.copy(extract));
        LCD.setCursor(0,2);
	LCD.print(string_load3.copy(extract));
        LCD.setCursor(0,3);
	LCD.print(string_load4.copy(extract));
	delay(1500); //delay to init lcd in time.
}

void store_lcdline( int i, char sbuffer[20] ) {
	
	switch (i) {
		case 1: 
				strcpy(lcd_line1,sbuffer);
				break;
		case 2: 
				strcpy(lcd_line2,sbuffer);
				break;
		case 3: 
				strcpy(lcd_line3,sbuffer);
				break;
		case 4: 
				strcpy(lcd_line4,sbuffer);
				break;
                default: 
                                break;
           
	}

}

void refresh_lcd() {
   // refreshing lcd at defined update.
        // update lines
        //LCD.clear();
        LCD.setCursor(0,0);
	LCD.print(lcd_line1);
        LCD.setCursor(0,1);
	LCD.print(lcd_line2);
        LCD.setCursor(0,2);
	LCD.print(lcd_line3);
        LCD.setCursor(0,3);
    	LCD.print(lcd_line4);
}

void lcddisp_menu() {
        Menu const* displaymenu_current = displaymenu.get_current_menu();
	MenuComponent const* displaymenu_sel = displaymenu_current->get_selected();

        for (int n = 1; n < 5; ++n) {
          char currentline[21];
	//
            if ( (displaymenu_current->get_num_menu_components()) >= n ) {
                
      		  MenuComponent const* displaymenu_comp = displaymenu_current->get_menu_component(n-1);
      		  sprintf(currentline,displaymenu_comp->get_name());
                          for ( int l = strlen(currentline); l<19 ; l++ ) {
	                        strcat(currentline," ");
				}			  
      		  if (displaymenu_sel == displaymenu_comp) strcat(currentline,"<");
                  else strcat(currentline," ");
            }
            else {
               //empty_line.toCharArray(string_buffer,21);
               string_load2.copy(currentline);
            }
		store_lcdline(n, currentline);
		
	};
		
}

// SET_HOME SCREEN
void lcddisp_sethome() {
   for ( int i = 1 ; i<5; i++ ) {
     char currentline[21] = "";
     char extract[21];
     switch (i) {
           case 1:
                //line1
                if (!telemetry_ok) { strcpy(currentline, "P:NO TELEMETRY"); }
                else if (telemetry_ok) 
                { 
                  sprintf(currentline,"P:%s SATS:%d FIX:%d", protocol, uav_satellites_visible, uav_fix_type);
                }
                break;
            case 2:
                 //line 2
                 if (!telemetry_ok) string_shome1.copy(currentline); // waiting for data
                 else 
                 {
                   if (!gps_fix) string_shome2.copy(currentline);  // waiting for gps fix
                   else {
                         sprintf(currentline, "%s%dm",string_shome3.copy(extract),round((uav_alt)/10));    
                        }
                    }
                 break;
      
             case 3:
                    if (!gps_fix) strcpy(currentline, string_shome4.copy(extract));
                    else {
                         char bufferl[10];
                         char bufferL[10];
                         sprintf(currentline,"%s %s", dtostrf(uav_lat, 5, 5, bufferl),dtostrf(uav_lon, 5, 5, bufferL));
                    }
                    break;
      
             case 4:
                   if (!gps_fix) strcpy(currentline,string_shome5.copy(extract));
                   else string_shome6.copy(currentline);
                   break;
              }
     
    for ( int l = strlen(currentline); l<20 ; l++ ) {
	 strcat(currentline," ");
	 }
    store_lcdline(i,currentline);
   } 
}

#ifdef BEARING_METHOD_1
void lcddisp_setbearing() {
    for ( int i = 1 ; i<5; i++ ) {
       char currentline[21] = "";
       switch (i) {
           case 1: 
                if (!telemetry_ok) { strcpy(currentline,"P:NO TELEMETRY"); }
                else if (telemetry_ok) sprintf(currentline,"P:%s SATS:%d FIX:%d", protocol, uav_satellites_visible, uav_fix_type); 
                break;
           case 2:
                string_load2.copy(currentline);  break;
           case 3:
                string_shome8.copy(currentline); break;
           case 4:      
                string_shome9.copy(currentline); break;

       }
       for ( int l = strlen(currentline); l<20 ; l++ ) {
	 strcat(currentline," ");
	 }
       store_lcdline(i,currentline);
    }
}
#endif
#if defined (BEARING_METHOD_2)
void lcddisp_setbearing() {
     if (right_button.holdTime() >= 700 && right_button.isPressed() ) {
    home_bearing+=10;
    if (home_bearing>360) home_bearing = 360;
    else if (home_bearing<0) home_bearing = 0;
    delay(500);
    }
    else if ( left_button.holdTime() >= 700 && left_button.isPressed() ) {
    home_bearing-=10;
    delay(500);
    }
    for ( int i = 1 ; i<5; i++ ) {
       char currentline[21] = "";
       switch (i) {
           case 1: 
                if (!telemetry_ok) { strcpy(currentline, "P:NO TELEMETRY"); }
                else if (telemetry_ok) sprintf(currentline,"P:%s SATS:%d FIX:%d", protocol, uav_satellites_visible, uav_fix_type);
                break;
           case 2:
                string_shome7.copy(currentline);  break;
           case 3:
                sprintf(currentline, "     << %3d >>", home_bearing); break;
           case 4:      
                string_load2.copy(currentline); break;
       }
       
       for ( int l = strlen(currentline); l<20 ; l++ ) {
	 strcat(currentline," ");
	 }
       store_lcdline(i,currentline);
       
       //checking long press left right

 }
}
#endif

#if defined (BEARING_METHOD_3) || defined (BEARING_METHOD_4)
void lcddisp_setbearing() {
    #if defined (BEARING_METHOD_4)
    retrieve_mag();
    #else
    home_bearing = uav_heading;  // use compass data from the uav.
    #endif
    for ( int i = 1 ; i<5; i++ ) {
       char currentline[21]="";
       switch (i) {
           case 1: 
                if (!telemetry_ok) { strcpy(currentline, "P:NO TELEMETRY"); }
                else if (telemetry_ok) sprintf(currentline,"P:%s SATS:%d FIX:%d", protocol, uav_satellites_visible, uav_fix_type);
                break;
           case 2:
                string_shome7.copy(currentline);  break;
           case 3:
                sprintf(currentline, "        %3d", home_bearing); break;
           case 4:      
                string_load2.copy(currentline); break;

       }
       for ( int l = strlen(currentline); l<20 ; l++ ) {
	 strcat(currentline," ");
	 }
       store_lcdline(i,currentline);
 }
}
#endif

void lcddisp_homeok() {
    for ( int i = 1 ; i<5; i++ ) {
       char currentline[21] = "";
       switch (i) {
           case 1: 
                if (!telemetry_ok) { strcpy(currentline, "P:NO TELEMETRY"); }
                else if (telemetry_ok) sprintf(currentline,"P:%s SATS:%d FIX:%d", protocol, uav_satellites_visible, uav_fix_type);
                break;
           case 2:
                string_shome10.copy(currentline); break;
           case 3:
                string_shome11.copy(currentline); break;                
           case 4:
                string_shome12.copy(currentline); break;
           }
       for ( int l = strlen(currentline); l<20 ; l++ ) {
	 strcat(currentline," ");
         }
       store_lcdline(i,currentline);
       }
}

void lcddisp_tracking(){
    for ( int i = 1 ; i<5; i++ ) {
       char currentline[21]="";
       switch (i) {
           case 1: 
                if (!telemetry_ok) { strcpy(currentline, "P:NO TELEMETRY"); }
                else if (telemetry_ok) sprintf(currentline,"P:%s SATS:%d FIX:%d", protocol, uav_satellites_visible, uav_fix_type);
                break;
           case 2:
                sprintf(currentline, "Alt:%dm Spd:%dkmh", round((uav_alt - home_alt)/10), uav_groundspeed);
                break;
           case 3:
                sprintf(currentline, "Dist:%dm Hdg:%d°", home_dist, uav_heading);
                break;
           case 4:   
                char bufferl[10];
                char bufferL[10];   
                sprintf(currentline, "%s %s", dtostrf(uav_lat, 5, 6,bufferl), dtostrf(uav_lon, 5, 6,bufferL));
                break;
       }
       for ( int l = strlen(currentline); l<20 ; l++ ) {
	 strcat(currentline," ");
	 }
       store_lcdline(i,currentline);
    }
}

void lcddisp_telemetry() {
    for ( int i = 1 ; i<5; i++ ) {
       char currentline[21]="";
       char extract[21];
       switch (i) {
           case 1: 
                        string_telemetry1.copy(currentline);  break;
           case 2:
                        string_load2.copy(currentline);  break;
           case 3:
                     switch (configuration.telemetry) {
                       
                        case 0:
                                 // currentline = "UAVTalk"; break;
                                string_telemetry2.copy(currentline); break;
                        case 1:
                                 //currentline = "MSP"; break;
                                string_telemetry3.copy(currentline); break;
                        case 2:
                                //currentline = "AudioModem"; break;
                                string_telemetry4.copy(currentline); break;
                        case 3:
                                //currentline = "MavLink"; break;
                                string_telemetry5.copy(currentline); break;
                        
                     }
                     break;
           case 4:      
                        strcpy(currentline, string_shome5.copy(extract)); break;

       }
       for ( int l = strlen(currentline); l<20 ; l++ ) {
	 strcat(currentline," ");
       }
       store_lcdline(i,currentline);
    }
  
}

void lcddisp_baudrate() {
    for ( int i = 1 ; i<5; i++ ) {
       char currentline[21]="";
       char extract[21];
       switch (i) {
           case 1: 
                     string_baudrate.copy(currentline);  break;
           case 2:
                     strcpy(currentline, string_load2.copy(extract)); break;
           case 3:
                     switch (configuration.baudrate) {
                       
                        case 0:
                                 // 1200
                                string_baudrate0.copy(currentline);  break;
                        case 1:
                                 //2400
                                string_baudrate1.copy(currentline);  break;
                        case 2:
                                 //4800
                                string_baudrate2.copy(currentline); break;
                        case 3:
                                 //9600
                                string_baudrate3.copy(currentline);  break;
                        case 4:
                                 //19200
                                string_baudrate4.copy(currentline);  break;
                        case 5:
                                 //38400
                                string_baudrate5.copy(currentline);  break;
                        case 6:
                                 //57600
                                string_baudrate6.copy(currentline);  break;
                        case 7:
                                 //115200
                                string_baudrate7.copy(currentline);  break;                            
                                              
                     }
                     break;
       
           case 4:      
                     string_shome5.copy(currentline); break;
           }
       for ( int l = strlen(currentline); l<20 ; l++ ) {
	 strcat(currentline," ");
       }
       store_lcdline(i,currentline);
    }
}

// Settings Bank config
void lcddisp_bank() {
    for ( int i = 1 ; i<5; i++ ) {
       char currentline[21]="";
       char extract[21];
       switch (i) {
           case 1: 
                     string_bank.copy(currentline);  break;
           case 2:
                     string_load2.copy(currentline); break;
           case 3:
                     switch (current_bank+1) {
                        case 1: sprintf(currentline,"> %s", string_bank1.copy(extract));break;
                        case 2: sprintf(currentline,"> %s", string_bank2.copy(extract));break;
                        case 3: sprintf(currentline,"> %s", string_bank3.copy(extract));break;
                        case 4: sprintf(currentline,"> %s", string_bank4.copy(extract));break;
                     }
                     
                     break;
       
           case 4:      
                     string_shome5.copy(currentline); break;
           }
       for ( int l = strlen(currentline); l<20 ; l++ ) {
	 strcat(currentline," ");
	 }
       store_lcdline(i,currentline);
    }
}


// SERVO CONFIGURATION

int config_servo(int servotype, int valuetype, int value ) {
	// servo configuration screen function return configured value
        //check long press left right
        if (right_button.holdTime() >= 700 && right_button.isPressed() ) {
              value+=20;
              delay(500);
        }
        else if ( left_button.holdTime() >= 700 && left_button.isPressed() ) {
              value-=20;
              delay(500);
        }
        char currentline[21];
        char extract[21];
	if (servotype==1) {
	  string_servos1.copy(currentline);                              // Pan servo
          store_lcdline(1, currentline);
	}
	else if (servotype==2) {
          string_servos2.copy(currentline);                              // Tilt servo
          store_lcdline(1, currentline);
	}
	string_load2.copy(currentline);
        store_lcdline(2, currentline);
	switch (valuetype) 
       {	        
              case 1: sprintf(currentline, "min endpoint: <%4d>",  value); break;          //minpwm
    	      case 2: sprintf(currentline, "min angle: <%3d>    ", value); break;         //minangle
	      case 3: sprintf(currentline, "max endpoint: <%4d>",  value); break;          //maxpwm
              case 4: sprintf(currentline, "max angle: <%3d>    ", value); break;         //maxangle

	}
	store_lcdline(3, currentline);
        string_shome5.copy(currentline);
	store_lcdline(4, currentline); 
return value;
           
}
