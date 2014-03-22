HardwareSerial SerialPort1(Serial);
//SoftwareSerial SerialPort2(SOFTSERIAL_RX,SOFTSERIAL_TX);
AltSoftSerial SerialPort2;

int          softserial_delay = (int)round(10000000.0f/(OUTPUT_BAUD)); // time to wait between each byte sent.
//boolean      ltm_counter = false;
uint8_t      ltm_scheduler = 0;
int32_t      uav_lat = 0;                    // latitude
int32_t      uav_lon = 0;                    // longitude
uint8_t      uav_satellites_visible = 0;     // number of satelites
uint8_t      uav_fix_type = 0;               // GPS lock 0-1=no fix, 2=2D, 3=3D
int32_t      uav_alt = 0;                    // altitude (cm)
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

long lastpacketreceived;
static boolean      enable_frame_request = 0;
//MAVLink session control
static boolean      mavbeat = 0;
//static boolean      landing = 0;
static float        lastMAVBeat = 0;
static boolean      waitingMAVBeats = 1;
static uint8_t      apm_mav_system; 
static uint8_t      apm_mav_component;

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

#define PROTOCOL_LIGHTTELEMETRY
