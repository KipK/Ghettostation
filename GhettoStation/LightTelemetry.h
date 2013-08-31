#define LIGHTTELEMETRY_START1 0x24 //$
#define LIGHTTELEMETRY_START2 0x54 //T
#define LIGHTTELEMETRY_GFRAME 0x47 //G for GPS + Baro altitude data
#define LIGHTTELEMETRY_GFRAMELENGTH 14

#define	LIGHTTELEMETRY_LAT				0
#define	LIGHTTELEMETRY_LON				4
#define	LIGHTTELEMETRY_GROUNDSPEED			8
#define	LIGHTTELEMETRY_ALTITUDE			        10
#define	LIGHTTELEMETRY_SATSFIX			        14


typedef enum {
	LIGHTTELEMETRY_PARSE_STATE_WAIT_START1 = 0,
        LIGHTTELEMETRY_PARSE_STATE_GOT_START1,
	LIGHTTELEMETRY_PARSE_STATE_GOT_START2,
	LIGHTTELEMETRY_PARSE_STATE_GOT_MSG_TYPE,
	LIGHTTELEMETRY_PARSE_STATE_GOT_DATA,
	LIGHTTELEMETRY_PARSE_STATE_GOT_CRC,
        LIGHTTELEMETRY_PARSE_STATE_CRC_OK
} lighttelemetry_parse_state_t;

typedef struct __lighttelemetry_message {
	uint8_t Start1;
        uint8_t Start2;
	uint8_t MsgType;
	uint8_t Data[64];
	uint8_t Crc;
} lighttelemetry_message_t;

int lighttelemetry_read(void);



