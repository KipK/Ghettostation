// VARIABLES DEFINITIONS

//Telemetry protocol used variables
float        uav_lat = 0;                    // latidude
float        uav_lon = 0;                    // longitude
uint8_t      uav_satellites_visible = 0;     // number of satelites
uint8_t      uav_fix_type = 0;               // GPS lock 0-1=no fix, 2=2D, 3=3D
float        uav_heading = 0;                // ground course heading from GPS
float        uav_alt = 0;                    // altitude
float        uav_groundspeed = 0;            // ground speed

//Specific protocol variables
//UAVTalk
uint8_t 		op_uavtalk_mode = 1;            // OP UAVTalk Active/Passive mode ( 1 = Passive )
String protocol = "UAVTk";

//servo variables
//int pan_middlepoint;
//int tilt_middlepoint;

//home variables
float home_lon;
float home_lat;
float home_alt;
float home_bearing;
int home_dist;




//tracking variables
float Bearing;
float Azimuth;

//lcd
char lcd_line1[21];
char lcd_line2[21];
char lcd_line3[21];
char lcd_line4[21];
String empty_line = "                    ";


//status variables
String current_activity = "MENU"; //default activity 
boolean gps_fix;
boolean btholdstate = false;
boolean telemetry_ok = false;
boolean home_pos = false;
boolean home_bear = false;

//servo temp configuration before saving
int servoconf_tmp[4];



//Configuration stored in EEprom
struct config_t
{
  int config_crc;
  int pan_minpwm;
  int pan_minangle;
  int pan_maxpwm;
  int pan_maxangle;
  int tilt_minpwm;
  int tilt_minangle;
  int tilt_maxpwm;
  int tilt_maxangle;
  float overlap_factor;
  int baudrate;
} configuration;

//Menu variables
MenuSystem displaymenu;
Menu rootMenu("");
MenuItem m1i1Item("START");
MenuItem m1i2Item("SET HOME");
Menu m1m3Menu("CONFIGURE");
	Menu m1m3m1Menu("SERVOS");	
		Menu m1m3m1m1Menu("PAN");
				MenuItem m1m3m1m1l1Item("PAN MINPWM");
				MenuItem m1m3m1m1l2Item("PAN MAXPWM");
				MenuItem m1m3m1m1l3Item("PAN MINANGLE");
				MenuItem m1m3m1m1l4Item("PAN MAXANGLE");			
		Menu m1m3m1m2Menu("TILT");
				MenuItem m1m3m1m2l1Item("TILT MINPWM");
				MenuItem m1m3m1m2l2Item("TILT MAXPWM");
				MenuItem m1m3m1m2l3Item("TILT MINANGLE");
				MenuItem m1m3m1m2l4Item("TILT MAXANGLE");			
		MenuItem m1m3m1i3Item("TEST");
	MenuItem m1m3l2Item("BAUDRATE");
