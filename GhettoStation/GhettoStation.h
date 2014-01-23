// VARIABLES DEFINITIONS


//Telemetry protocol used variables
float   uav_lat = 0;                    // latitude
float   uav_lon = 0;                    // longitude
int     uav_satellites_visible = 0;     // number of satelites
int     uav_fix_type = 0;               // GPS lock 0-1=no fix, 2=2D, 3=3D
long    uav_alt = 0;                    // altitude (dm)
int     uav_groundspeed = 0;            // ground speed

String  protocol = "";
long    lastpacketreceived;

void mavlink_read(void);
boolean uavtalk_read(void);
void msp_read(void);
void ltm_read(void);

//home variables
float   home_lon;
float   home_lat;
int     home_alt;
int     home_bearing = 0;
int     home_dist;

//tracking variables
int     Bearing;
int     Elevation;
int     servoBearing=0;
int     servoElevation=0;

//lcd
char    lcd_line1[21];
char    lcd_line2[21];
char    lcd_line3[21];
char    lcd_line4[21];
//String empty_line = "                    ";
String  cur_string;


//status variables
int     current_activity = 0; // acctivity status 0: Menu , 1: Track, 2: SET_HOME, 3: PAN_MINPWM, 4: PAN_MINANGLE, 5: PAN_MAXPWM, 6: PAN_MAXANGLE, 7: TILT_MINPWM, 8: TILT_MINANGLE, 9: TILT_MAXPWM, 10: TILT_MAXANGLE, 11: TEST_SERVO, 12: SET_RATE
//String current_activity = "MENU"; //default activity 
boolean gps_fix;
boolean btholdstate = false;
boolean telemetry_ok = false;
boolean home_pos = false;
boolean home_bear = false;


//servo temp configuration before saving
int     servoconf_tmp[4];

//baudrate selection
int     baudrates[9]= {1200, 2400, 4800, 9600, 19200, 38400, 57600, 115200};

MenuSystem displaymenu;

        



