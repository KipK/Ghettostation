/* ########################################  DEFINES ######################################################*/
#define PROTOCOL_UAVTALK                        // OpenPilot / Taulabs protocol
#define PROTOCOL_MSP                            // MSP from Multiwii 
#define PROTOCOL_LIGHTTELEMETRY                 // Ghettostation internal protocol. 
#define PROTOCOL_MAVLINK                        // Mavlink for Ardupilot / Autoquad / PixHawk / Taulabs (UAVOmavlinkBridge)
/* ######################################## SERIAL HAL ####################################################*/
#ifdef TEENSYPLUS2
// This line defines a "Uart" object to access the serial port
HardwareSerial SerialPort1 = HardwareSerial();
 #ifdef OSD_OUTPUT
 SoftwareSerial SerialPort2(SOFTSERIAL_TX,SOFTSERIAL_TX);
 #endif
#endif
#ifdef MEGA
HardwareSerial SerialPort1(Serial1);
 #ifdef OSD_OUTPUT
  HardwareSerial SerialPort2(Serial2);
 #endif
#endif
/* ########################################  VARIABLES #####################################################*/


//Telemetry variables
float        uav_lat = 0;                    // latitude
float        uav_lon = 0;                    // longitude
uint8_t      uav_satellites_visible = 0;     // number of satelites
uint8_t      uav_fix_type = 0;               // GPS lock 0-1=no fix, 2=2D, 3=3D
int16_t      uav_alt = 0;                    // altitude (dm)
int          uav_groundspeed = 0;            // ground speed
int16_t      uav_pitch = 0;                  // attitude pitch
int16_t      uav_roll = 0;                   // attitude roll
int16_t      uav_heading = 0;                // attitude heading
int16_t      uav_gpsheading=0;               // gps heading
uint16_t     uav_bat = 0;                    // battery voltage (mv)
uint16_t     uav_amp = 0;                    // consumed mah.
uint16_t     uav_current = 0;                // actual current
uint8_t      uav_rssi = 0;                   // radio RSSI (%)
uint8_t      uav_linkquality = 0;            // radio link quality
uint8_t      uav_airspeed = 0;               // Airspeed sensor (m/s)
uint8_t      uav_arm = 0;                    // 0: disarmed, 1: armed
uint8_t      uav_failsafe = 0;               // 0: normal,   1: failsafe 
uint8_t      uav_flightmode = 16;            // Flight mode(0-19): 0: Manual, 1: Rate, 2: Attitude/Angle, 3: Horizon, 4: Acro, 5: Stabilized1, 6: Stabilized2, 7: Stabilized3,
                                             // 8: Altitude Hold, 9: Loiter/GPS Hold, 10: Auto/Waypoints, 11: Heading Hold / headFree, 12: Circle, 13: RTH, 14: FollowMe, 15: LAND, 
                                             // 16:FlybyWireA, 17: FlybywireB, 18: Cruise, 19: Unknown
int16_t      uav_chan5_raw;      
int16_t      uav_chan6_raw; 
int16_t      uav_chan7_raw; 
int16_t      uav_chan8_raw; 

char* protocol = "";
long lastpacketreceived;
static boolean      enable_frame_request = 0;
//MAVLink session control
static boolean      mavbeat = 0;
//static boolean      landing = 0;
static float        lastMAVBeat = 0;
static boolean      waitingMAVBeats = 1;
static uint8_t      apm_mav_system; 
static uint8_t      apm_mav_component;


//home 
float home_lon;
float home_lat;
int home_alt;
int home_bearing = 0;
int home_dist;

//tracking 
int Bearing;
int Elevation;
int servoBearing=0;
int servoElevation=0;

//lcd
char lcd_line1[21];
char lcd_line2[21];
char lcd_line3[21];
char lcd_line4[21];


//status 
int current_activity = 0; // Activity status 0: Menu , 1: Track, 2: SET_HOME, 3: PAN_MINPWM, 4: PAN_MINANGLE, 5: PAN_MAXPWM, 
                          // 6: PAN_MAXANGLE, 7: TILT_MINPWM, 8: TILT_MINANGLE, 9: TILT_MAXPWM, 10: TILT_MAXANGLE, 11: TEST_SERVO, 12: SET_RATE
boolean gps_fix      = false;
boolean btholdstate  = false;
boolean telemetry_ok = false;
boolean home_pos     = false;
boolean home_bear    = false;

//servo temp configuration before saving
int servoconf_tmp[4];

//baudrate selection
long baudrates[8]= {1200, 2400, 4800, 9600, 19200, 38400, BAUDRATE56K, 115200};

/*##################################### STRINGS STORED IN FLASH #############################################*/

FLASH_STRING(string_load1,      "  [GHETTOSTATION]   ");
FLASH_STRING(string_load2,      "                    ");
FLASH_STRING(string_load3,      " __________________ ");
FLASH_STRING(string_load4,      "      Welcome       ");
FLASH_STRING(string_shome1,     "  Waiting for Data  ");
FLASH_STRING(string_shome2,     "   No GPS 3D FIX    ");
FLASH_STRING(string_shome3,     "3D FIX! Alt:");
FLASH_STRING(string_shome4,     "    Please Wait.    ");
FLASH_STRING(string_shome5,     "(long press to quit)");
FLASH_STRING(string_shome6,     " Save Home pos now? ");
FLASH_STRING(string_shome7,     " Set Bearing:       ");
FLASH_STRING(string_shome8,     " Move UAV 20m ahead ");
FLASH_STRING(string_shome9,     " & press enter      ");
FLASH_STRING(string_shome10,    "    HOME IS SET     ");
FLASH_STRING(string_shome11,    "Enter:Start Tracking");
FLASH_STRING(string_shome12,    "<< Menu     Reset >>");
FLASH_STRING(string_servos1,    "    [PAN SERVO]     ");
FLASH_STRING(string_servos2,    "    [TILT SERVO]    ");
FLASH_STRING(string_telemetry1, "  SELECT PROTOCOL:  ");
FLASH_STRING(string_telemetry2, "      UAVTALK     >>");
FLASH_STRING(string_telemetry3, "<<    MULTIWII    >>");
FLASH_STRING(string_telemetry4, "<< LIGHTTELEMETRY >>");
FLASH_STRING(string_telemetry5, "<<     MAVLINK      ");
FLASH_STRING(string_baudrate,   "  SELECT BAUDRATE:  ");
FLASH_STRING(string_baudrate0,  "        1200      >>");
FLASH_STRING(string_baudrate1,  "<<      2400      >>");
FLASH_STRING(string_baudrate2,  "<<      4800      >>");
FLASH_STRING(string_baudrate3,  "<<      9600      >>");
FLASH_STRING(string_baudrate4,  "<<     19200      >>");
FLASH_STRING(string_baudrate5,  "<<     38400      >>");
FLASH_STRING(string_baudrate6,  "<<     57600      >>");
FLASH_STRING(string_baudrate7,  "<<    115200        ");
FLASH_STRING(string_bank,       " LOAD SETTINGS BANK ");
FLASH_STRING(string_bank1,      BANK1);
FLASH_STRING(string_bank2,      BANK2);
FLASH_STRING(string_bank3,      BANK3);
FLASH_STRING(string_bank4,      BANK4);

/*########################################### MENU ##################################################*/
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
        MenuItem m1m3i2Item("TELEMETRY");
        MenuItem m1m3i3Item("BAUDRATE");
MenuItem m1i4Item("SWITCH SETTINGS");

/*##################################### COMMON FUNCTIONS #############################################*/
boolean getBit(byte Reg, byte whichBit) {
    boolean State;
    State = Reg & (1 << whichBit);
    return State;
}

byte setBit(byte &Reg, byte whichBit, boolean stat) {
    if (stat) {
        Reg = Reg | (1 << whichBit);
    } 
    else {
        Reg = Reg & ~(1 << whichBit);
    }
    return Reg;
}

float toRad(float angle) {
// convert degrees to radians
	angle = angle*0.01745329; // (angle/180)*pi
	return angle;
}

float toDeg(float angle) {
// convert radians to degrees.
	angle = angle*57.29577951;   // (angle*180)/pi
        return angle;
}



int config_bank[]= {1, 51, 101,151}; // 50 bytes reserved per bank. 
uint8_t current_bank;

template <class T> int EEPROM_write(int ee, const T& value)
{
    const byte* p = (const byte*)(const void*)&value;
    unsigned int i;
    for (i = 0; i < sizeof(value); i++)
          EEPROM.write(ee++, *p++);
    return i;
}

template <class T> int EEPROM_read(int ee, T& value)
{
    byte* p = (byte*)(void*)&value;
    unsigned int i;
    for (i = 0; i < sizeof(value); i++)
          *p++ = EEPROM.read(ee++);
    return i;
}


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
  int baudrate;
  int telemetry;
  int bearing;
} configuration;



void clear_eeprom() {
	// clearing eeprom
	for (int i = 0; i < 1025; i++)
		EEPROM.write(i, 0);
		// eeprom is clear  we can write default config
        //writing 4 setting banks.
        for (int j = 0; j < 4; j++) {          
	  configuration.config_crc = CONFIG_VERSION;  // config version check
	  configuration.pan_minpwm = PAN_MINPWM;
	  configuration.pan_minangle = PAN_MINANGLE;
	  configuration.pan_maxpwm = PAN_MAXPWM;
	  configuration.pan_maxangle = PAN_MAXANGLE;
	  configuration.tilt_minpwm = TILT_MINPWM;
	  configuration.tilt_minangle = TILT_MINANGLE;
	  configuration.tilt_maxpwm = TILT_MAXPWM;
          configuration.tilt_maxangle = TILT_MAXANGLE;
	  configuration.baudrate = 6;
          configuration.telemetry = 0;
          configuration.bearing = 0;
	  EEPROM_write(config_bank[j], configuration);
        }       
}


