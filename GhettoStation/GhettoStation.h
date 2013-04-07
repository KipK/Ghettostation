// VARIABLES DEFINITIONS

//Telemetry protocol used variables
float        uav_lat = 0;                    // latitude
float        uav_lon = 0;                    // longitude
uint8_t      uav_satellites_visible = 0;     // number of satelites
uint8_t      uav_fix_type = 0;               // GPS lock 0-1=no fix, 2=2D, 3=3D
//float        uav_heading = 0;                // ground course heading from GPS
int        uav_alt = 0;                    // altitude
int        uav_groundspeed = 0;            // ground speed

String protocol = "";
//Specific protocol variables
//UAVTalk
uint8_t op_uavtalk_mode = 1;            // OP UAVTalk Active/Passive mode ( 1 = Passive )

//servo variables
//int pan_middlepoint;
//int tilt_middlepoint;

//home variables
float home_lon;
float home_lat;
int home_alt;
float home_bearing;
int home_dist;




//tracking variables
int Bearing;
int Azimuth;

//lcd
char lcd_line1[21];
char lcd_line2[21];
char lcd_line3[21];
char lcd_line4[21];
//String empty_line = "                    ";
String cur_string;


//status variables
int current_activity = 0; // acctivity status 0: Menu , 1: Track, 2: SET_HOME, 3: PAN_MINPWM, 4: PAN_MINANGLE, 5: PAN_MAXPWM, 6: PAN_MAXANGLE, 7: TILT_MINPWM, 8: TILT_MINANGLE, 9: TILT_MAXPWM, 10: TILT_MAXANGLE, 11: TEST_SERVO, 12: SET_RATE
//String current_activity = "MENU"; //default activity 
boolean gps_fix;
boolean btholdstate = false;
boolean telemetry_ok = false;
boolean home_pos = false;
boolean home_bear = false;


//servo temp configuration before saving
int servoconf_tmp[4];



//Menu variables
MenuSystem displaymenu;
Menu rootMenu("");
MenuItem m1i1Item("START");
MenuItem m1i2Item("SET HOME");
Menu m1m3Menu("CONFIG");
	Menu m1m3m1Menu("SERVOS");	
		Menu m1m3m1m1Menu("PAN");
				MenuItem m1m3m1m1l1Item("MINPWM");
				MenuItem m1m3m1m1l2Item("MAXPWM");
				MenuItem m1m3m1m1l3Item("MINANGLE");
				MenuItem m1m3m1m1l4Item("MAXANGLE");			
		Menu m1m3m1m2Menu("TILT");
				MenuItem m1m3m1m2l1Item("MINPWM");
				MenuItem m1m3m1m2l2Item("MAXPWM");
				MenuItem m1m3m1m2l3Item("MINANGLE");
				MenuItem m1m3m1m2l4Item("MAXANGLE");			
		MenuItem m1m3m1i3Item("TEST");
