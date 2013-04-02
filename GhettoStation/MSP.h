
//General use variables

#define SERIALBUFFERSIZE 128
static uint8_t serialBuffer[SERIALBUFFERSIZE]; // this hold the imcoming string from serial O string
uint8_t serialMSPStringOK=0;
uint8_t receiverIndex=0;
int32_t  MwAltitude=0;                         // This hold barometric value

uint8_t MwVersion=0;
uint8_t MwVBat=0;

uint8_t GPS_fix=0;
int32_t GPS_latitude;
int32_t GPS_longitude;
int16_t GPS_altitude;
int16_t GPS_speed=0;

uint8_t GPS_numSat=0;


// For Altitude

int altitude =1;
static uint8_t cmdMSP;
static uint8_t checksum;




// ---------------------------------------------------------------------------------------
// Defines 
#define MSP_VERSION        	 0

#define MSP_IDENT                100   //out message         multitype + multiwii version + protocol version + capability variable
#define MSP_STATUS               101   //out message         cycletime & errors_count & sensor present & box activation

#define MSP_RAW_GPS              106   //out message         fix, numsat, lat, lon, alt, speed
#define MSP_COMP_GPS             107   //out message         distance home, direction home





