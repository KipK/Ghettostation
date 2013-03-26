// CONFIGURATION FUNCTIONS

int config_servo(int servotype, int valuetype, int &param ) {
	// servo configuration screen function return configured value
        String currentline;
        char string_buffer[21];
        
	if (servotype=1) {
	  store_lcdline(1,"    [PAN SERVO]    ");
	}
	else if (servotype==2) {
	  store_lcdline(1,"    [TILT SERVO]    ");
	}
	store_lcdline(2,"                    ");
	if (valuetype=1) {	//minpwm
          currentline = "min endpoint: " + param;
          currentline += "  ";
	}
	else if (valuetype=2) { //minangle
          currentline = "min angle: " + param;
          currentline += "        ";
	}
	else if (valuetype=3) {	//maxpwm
	  currentline = "max endpoint: " + param;
          currentline += "  ";
	}
	else if (valuetype=4) { //maxangle
	  currentline = "max angle: " + param;
          currentline += "        ";
	}
        currentline.toCharArray(string_buffer,21);
	store_lcdline(3,string_buffer);
	store_lcdline(4,"Press ENTER to quit ");
	
	//navigate
//	if (enter_button.uniquePress()) {
//		EEPROM_writeAnything(0, configuration); //Store config
//		current_activity = "MENU";
//	}
//	if (left_button.isPressed()) {
//		param = param - 1;
//	}
//	if (right_button.isPressed()) {
//		param = param + 1;
//	}
}
