//LCD 



void init_lcdscreen() {
// init LCD
	LCD.Clear();
	LCD.Backlight_On();
	LCD.PrintChar(1, 1, "  [GHETTOSTATION]   ");
	LCD.PrintChar(1, 2, " __________________ ");
	LCD.PrintChar(1, 4, "      Welcome       ");
	delay(1500); //delay to init lcd in time.
}

void store_lcdline( int i, char string_buffer[21] ) {
	
	switch (i) {
		case 1: 
				strcpy(lcd_line1,string_buffer);
				break;
		case 2: 
				strcpy(lcd_line2,string_buffer);
				break;
		case 3: 
				strcpy(lcd_line3,string_buffer);
				break;
		case 4: 
				strcpy(lcd_line4,string_buffer);
				break;
                default: 
                                break;
           
	}

}

void refresh_lcd() {
   // refreshing lcd at defined update.
    if (lcdMetro.check() == 1) {
        // update lines
	LCD.PrintChar(1, 1, lcd_line1);
        LCD.PrintChar(1, 2, lcd_line2);
        LCD.PrintChar(1, 3, lcd_line3);
        LCD.PrintChar(1, 4, lcd_line4);
    }
   
}



// SET_HOME SCREEN
void lcddisp_sethome() {
   for ( int i = 1 ; i<5; i++ ) {
     char string_buffer[21];
     String currentline="";
     if (i=1) {
        //line1
        if (gcstelemetrystatus!=3) String c="NO";
        else String c=protocol;
        currentline = "L:" + protocol;
        currentline += "SATS:";
        currentline += String(uav_sats);
        currentline += " FIX:";
        currentline += String(uav_fix);
        }
     else if (i=2) {
       //line 2
       if (gcstelemetrystatus!=3) currentline =  "  Waiting for Data  ";
       else {
         if (!gps_fix) currentline = "   No GPS 3D FIX    ";
         else currentline = "     3D FIX OK      ";
       }
      }
      else if (i=3) {
        if (!gps_fix) currentline = "    Please Wait.    ";
        else {
             char buffer[10];
             currentline = String(dtostrf(uav_lat, 5, 5, buffer));
             currentline += " ";
             currentline += String(dtostrf(uav_lon, 5, 5, buffer));
        }
      }
      else {
         if (!gps_fix) currentline = "(long press to quit)";
         else currentline = "   Save Home pos now?   ";
      }
      
    for ( int l = currentline.length()-1 ; l<20 ; l++ ) {
	 currentline = currentline + " ";
	 }
    currentline.toCharArray(string_buffer,21);
    store_lcdline(i,string_buffer);
   } 
}


void lcddisp_setbearing() {
    for ( int i = 1 ; i<5; i++ ) {
       char string_buffer[21];
       String currentline="";
       switch (i) {
           case 1: 
                        if (gcstelemetrystatus!=3) String c="NO";
                        else String c=protocol;
                        currentline = "L:" + protocol;
                        currentline += "SATS:";
                        currentline += String(uav_sats);
                        currentline += " FIX:";
                        currentline += String(uav_fix);
                        break;
           case 2:
                        currentline = "                    "; break;
           case 3:
                        currentline = " Move UAV 20m ahead "; break;
           case 4:      
                        currentline = " & press enter      ";break;

       }
       for ( int l = currentline.length()-1 ; l<20 ; l++ ) {
	 currentline = currentline + " ";
	 }
       currentline.toCharArray(string_buffer,21);
       store_lcdline(i,string_buffer);
    }
}

void lcddisp_homeok() {
    for ( int i = 1 ; i<5; i++ ) {
       char string_buffer[21];
       String currentline="";
       switch (i) {
           case 1: 
                        if (gcstelemetrystatus!=3) String c="NO";
                        else String c=protocol;
                        currentline = "L:" + protocol;
                        currentline += "SATS:";
                        currentline += String(uav_sats);
                        currentline += " FIX:";
                        currentline += String(uav_fix);
                        break;
           case 2:
                        currentline = "      HOME IS SET      "; break;
           case 3:
                        currentline = "Enter: Start Tracking"; break;

           case 4:      currentline = " << Menu     Reset >>"; break;
       
           }
       for ( int l = currentline.length()-1 ; l<20 ; l++ ) {
         currentline = currentline + " ";
         }
       currentline.toCharArray(string_buffer,21);
       store_lcdline(i,string_buffer);
       }
}
