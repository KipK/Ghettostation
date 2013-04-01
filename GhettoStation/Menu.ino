



//create menu
void init_menu() {
	rootMenu.add_item(&m1i1Item, &screen_tracking); //start track
	rootMenu.add_item(&m1i2Item, &screen_sethome); //set home position
	rootMenu.add_menu(&m1m3Menu); //configure
		m1m3Menu.add_menu(&m1m3m1Menu); //config servos
			m1m3m1Menu.add_menu(&m1m3m1m1Menu); //config pan
				m1m3m1m1Menu.add_item(&m1m3m1m1l1Item, &configure_pan_minpwm); // pan min pwm
				m1m3m1m1Menu.add_item(&m1m3m1m1l2Item, &configure_pan_maxpwm); // pan max pwm
                                m1m3m1m1Menu.add_item(&m1m3m1m1l3Item, &configure_pan_minangle); // pan min angle
				m1m3m1m1Menu.add_item(&m1m3m1m1l4Item, &configure_pan_maxangle); // pan max angle
			m1m3m1Menu.add_menu(&m1m3m1m2Menu); //config tilt
				m1m3m1m2Menu.add_item(&m1m3m1m2l1Item, &configure_tilt_minpwm); // tilt min pwm
				m1m3m1m2Menu.add_item(&m1m3m1m2l2Item, &configure_tilt_maxpwm); // tilt max pwm
                                m1m3m1m2Menu.add_item(&m1m3m1m2l3Item, &configure_tilt_minangle); // tilt min angle
				m1m3m1m2Menu.add_item(&m1m3m1m2l4Item, &configure_tilt_maxangle); // tilt max angle
                        m1m3m1Menu.add_item(&m1m3m1i3Item, &configure_test_servo);
		m1m3Menu.add_item(&m1m3l2Item, &configure_baudrate); //config baudrate
	displaymenu.set_root_menu(&rootMenu);
}


void display_menu() {
        Serial.print("displaymenu");
        Menu const* displaymenu_current = displaymenu.get_current_menu();
	MenuComponent const* displaymenu_sel = displaymenu_current->get_selected();

        for (int n = 1; n < 5; ++n) {
          char string_buffer[21];
	//
            if ( (displaymenu_current->get_num_menu_components()) >= n ) {
                
      		  MenuComponent const* displaymenu_comp = displaymenu_current->get_menu_component(n-1);
      		  String getname = displaymenu_comp->get_name();
			  for ( int l = getname.length()-1 ; l<20 ; l++ ) {
				getname = getname + " ";
				}			  

      		  if (displaymenu_sel == displaymenu_comp) {
 
      				getname.setCharAt(19,'<');
      		  } else {
      		  		getname.setCharAt(19, ' ');
      		  }
      	
      		  getname.toCharArray(string_buffer,21);
                
            }
            else {
               //empty_line.toCharArray(string_buffer,21);
               string_load2.copy(string_buffer);
            }
		store_lcdline(n, string_buffer);
		
	};
		
}

//menu item callback functions

void screen_tracking(MenuItem* p_menu_item) {
	
	current_activity = 1;
}

void screen_sethome(MenuItem* p_menu_item) {
	current_activity = 2;
}

void configure_pan_minpwm(MenuItem* p_menu_item) {
	current_activity = 3;
}

void configure_pan_minangle(MenuItem* p_menu_item) {
	current_activity = 4;
}

void configure_pan_maxpwm(MenuItem* p_menu_item) {
	current_activity = 5;
}

void configure_pan_maxangle(MenuItem* p_menu_item) {
	current_activity = 6;
}

void configure_tilt_minpwm(MenuItem* p_menu_item) {
	current_activity = 7;
}

void configure_tilt_minangle(MenuItem* p_menu_item) {
	current_activity = 8;
}

void configure_tilt_maxpwm(MenuItem* p_menu_item) {
	current_activity = 9;
}

void configure_tilt_maxangle(MenuItem* p_menu_item) {
	current_activity = 10;
}

void configure_test_servo(MenuItem* p_menu_item) {
  
       current_activity = 11;
}

void configure_baudrate(MenuItem* p_menu_item) {
	current_activity = 12;
}


