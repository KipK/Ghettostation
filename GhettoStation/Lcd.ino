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


