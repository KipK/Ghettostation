//MENU FUNCTIONS

//create menu
void init_menu() {
	rootMenu.add_item(&m1i1Item, &screen_tracking); //start track
	rootMenu.add_item(&m1i2Item, &screen_sethome); //set home position
	rootMenu.add_menu(&m1m3Menu); //configure
		m1m3Menu.add_menu(&m1m3m1Menu); //config servos
			m1m3m1Menu.add_menu(&m1m3m1m1Menu); //config pan
				m1m3m1m1Menu.add_item(&m1m3m1m1l1Item, &configure_pan_minpwm); // pan min pwm
				m1m3m1m1Menu.add_item(&m1m3m1m1l2Item, &configure_pan_minangle); // pan min angle
				m1m3m1m1Menu.add_item(&m1m3m1m1l3Item, &configure_pan_maxpwm); // pan max pwm
				m1m3m1m1Menu.add_item(&m1m3m1m1l4Item, &configure_pan_maxangle); // pan max angle
			m1m3m1Menu.add_menu(&m1m3m1m2Menu); //config tilt
				m1m3m1m2Menu.add_item(&m1m3m1m2l1Item, &configure_pan_minpwm); // tilt min pwm
				m1m3m1m2Menu.add_item(&m1m3m1m2l2Item, &configure_pan_minangle); // tilt min angle
				m1m3m1m2Menu.add_item(&m1m3m1m2l3Item, &configure_pan_maxpwm); // tilt max pwm
				m1m3m1m2Menu.add_item(&m1m3m1m2l4Item, &configure_pan_maxangle); // tilt max angle
                        m1m3m1Menu.add_item(&m1m3m1i3Item, &configure_test_servo);
		m1m3Menu.add_item(&m1m3l2Item, &configure_baudrate); //config baudrate
	displaymenu.set_root_menu(&rootMenu);
}

//display menu
void display_menu() {
        Menu const* displaymenu_current = displaymenu.get_current_menu();
		//Serial.print("current menu = ");
		//Serial.println(displaymenu_current->get_cur_menu_component_num());
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
               empty_line.toCharArray(string_buffer,21);
            }
		store_lcdline(n, string_buffer);
		
	};
		
}

//navigate menu
void navigate_menu() {;
//        //differenciate long press from unique press
//	if (islong_press(enter_button) == 1) {
//		displaymenu.select();
//	}
//        else if (islong_press(enter_button) == 2) {
//          //long press means exit
//                displaymenu.back();
//        }
//	if (left_button.uniquePress()) {
//		displaymenu.prev();
//	
//	}
//	if (right_button.uniquePress()) {
//		displaymenu.next();
//	}

}
//menu item callback functions

void screen_tracking(MenuItem* p_menu_item) {
	
	current_activity = "TRACK";
}

void screen_sethome(MenuItem* p_menu_item) {
	current_activity = "SET_HOME";
}

void configure_pan_minpwm(MenuItem* p_menu_item) {
	current_activity = "PAN_MINPWM";
}

void configure_pan_minangle(MenuItem* p_menu_item) {
	current_activity = "PAN_MINANGLE";
}

void configure_pan_maxpwm(MenuItem* p_menu_item) {
	current_activity = "PAN_MAXPWM";
}

void configure_pan_maxangle(MenuItem* p_menu_item) {
	current_activity = "PAN_MAXANGLE";
}

void configure_tilt_minpwm(MenuItem* p_menu_item) {
	current_activity = "TILT_MINPWM";
}

void configure_tilt_minangle(MenuItem* p_menu_item) {
	current_activity = "TILT_MINANGLE";
}

void configure_tilt_maxpwm(MenuItem* p_menu_item) {
	current_activity = "TILT_MAXPWM";
}

void configure_tilt_maxangle(MenuItem* p_menu_item) {
	current_activity = "TILT_MAXANGLE";
}

void configure_test_servo(MenuItem* p_menu_item) {
  
       current_activity = "TEST_SERVO";
}

void configure_baudrate(MenuItem* p_menu_item) {
	current_activity = "SET_RATE";
}


