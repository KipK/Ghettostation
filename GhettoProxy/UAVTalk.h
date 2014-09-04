/**
 ******************************************************************************
 *
 * @file       UAVTalk.h
 * @author     Joerg-D. Rothfuchs
 * @brief      Implements a subset of the telemetry communication between
 * 	       OpenPilot CC, CC3D, Revolution and Ardupilot Mega MinimOSD
 * 	       with code from OpenPilot and MinimOSD.
 * @see        The GNU Public License (GPL) Version 3
 *
 *****************************************************************************/
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
 * for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, see <http://www.gnu.org/licenses/> or write to the 
 * Free Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#define	FLIGHTTELEMETRYSTATS_OBJID          0x2F7E2902
#define FLIGHTTELEMETRYSTATS_OBJID_001      0x6737BB5A
#define	GCSTELEMETRYSTATS_OBJID             0xABC72744
#define GCSTELEMETRYSTATS_OBJID_001         0xCAD1DC0A              // different ID for 14.02.01
#define	ATTITUDEACTUAL_OBJID                0x33DAD5E6
#define ATTITUDESTATE_OBJID                 0xD7E0D964              // new name since OP VERSION_RELEASE_14_02_1
#define	FLIGHTSTATUS_OBJID                  0x9B6A127E              // Op
#define	FLIGHTSTATUS_OBJID_001              0x0ED79A04      // Op next
#define	FLIGHTSTATUS_OBJID_002              0x1B7AEB74      // OP next
#define FLIGHTSTATUS_OBJID_003              0x0B37AA16      // OP
#define FLIGHTSTATUS_OBJID_004              0x6CECFBC2      // Taulabs 
#define FLIGHTSTATUS_OBJID_005              0xC2E431BA      //Taulabs current next.
#define	MANUALCONTROLCOMMAND_OBJID          0x5C2F58AC              // Taulabs ( only used for RSSI )
#define MANUALCONTROLCOMMAND_OBJID_001      0xB8C7F78A              // different ID for 14.02.01
#define GPSPOSITION_OBJID                   0xE2A323B6
#define GPSPOSITION_OBJID_001               0x40BCC84E              //new Taulabs next ID.
#define GPSPOSITIONSENSOR_OBJID             0x1A5748CE              // new name since VERSION_RELEASE_14_02_1
#define AIRSPEEDACTUAL_OBJID                0x133A3280
#define FLIGHTBATTERYSTATE_OBJID            0xD2083596
#define BAROALTITUDE_OBJID                  0x99622E6A
#define BAROSENSOR_OBJID                    0x48120EA6      // new name since VERSION_RELEASE_14_02_1

#define OPLINKSTATUS_OBJID                  0x669C55E2
#define OPLINKSTATUS_OBJID_001              0xBE2584BA


#define FLIGHTTELEMETRYSTATS_OBJ_LEN                    21
#define	FLIGHTTELEMETRYSTATS_OBJ_STATUS			20
#define FLIGHTTELEMETRYSTATS_OBJ_LEN_001                37              // different since VERSION_RELEASE_14_02_1
#define FLIGHTTELEMETRYSTATS_OBJ_STATUS_001             36              // different since VERSION_RELEASE_14_02_1

#define BAROALTITUDE_OBJ_ALTITUDE			0
#define BAROALTITUDE_OBJ_TEMPERATURE			4
#define BAROALTITUDE_OBJ_PRESSURE			8

#define FLIGHTBATTERYSTATE_OBJ_VOLTAGE			0
#define FLIGHTBATTERYSTATE_OBJ_CURRENT			4
#define FLIGHTBATTERYSTATE_OBJ_BOARD_SUPPLY_VOLTAGE	8
#define FLIGHTBATTERYSTATE_OBJ_PEAK_CURRENT		12
#define FLIGHTBATTERYSTATE_OBJ_AVG_CURRENT		16
#define FLIGHTBATTERYSTATE_OBJ_CONSUMED_ENERGY		20
#define FLIGHTBATTERYSTATE_OBJ_ESTIMATED_FLIGHT_TIME	24

#define GCSTELEMETRYSTATS_OBJ_LEN                       21
#define GCSTELEMETRYSTATS_OBJ_STATUS                    20
#define GCSTELEMETRYSTATS_OBJ_LEN_001                   37              // different since VERSION_RELEASE_14_02_1
#define GCSTELEMETRYSTATS_OBJ_STATUS_001                36              // different since VERSION_RELEASE_14_02_1

#define	ATTITUDEACTUAL_OBJ_ROLL				16
#define	ATTITUDEACTUAL_OBJ_PITCH			20
#define	ATTITUDEACTUAL_OBJ_YAW				24

#define	FLIGHTSTATUS_OBJ_ARMED              0
#define	FLIGHTSTATUS_OBJ_FLIGHTMODE         1
#define FLIGHTSTATUS_OBJ_CONTROLSOURCE      2               //Taulabs only

#define	GPSPOSITION_OBJ_LAT				0
#define	GPSPOSITION_OBJ_LON				4
#define	GPSPOSITION_OBJ_ALTITUDE			8
#define	GPSPOSITION_OBJ_GEOIDSEPARATION			12
#define	GPSPOSITION_OBJ_HEADING				16
#define	GPSPOSITION_OBJ_GROUNDSPEED			20
#define	GPSPOSITION_OBJ_PDOP				24
#define	GPSPOSITION_OBJ_HDOP				28
#define	GPSPOSITION_OBJ_VDOP				32
#define	GPSPOSITION_OBJ_STATUS				36
#define	GPSPOSITION_OBJ_SATELLITES			37

#define	OPLINKSTATUS_OBJ_RSSI				85
#define	OPLINKSTATUS_OBJ_LINKQUALITY			86

#define MANUALCONTROLCOMMAND_OBJ_THROTTLE               0
#define MANUALCONTROLCOMMAND_OBJ_ROLL                   4
#define MANUALCONTROLCOMMAND_OBJ_PITCH                  8
#define MANUALCONTROLCOMMAND_OBJ_YAW                    12
#define MANUALCONTROLCOMMAND_OBJ_COLLECTIVE             16
#define MANUALCONTROLCOMMAND_OBJ_CHANNEL_0              20
#define MANUALCONTROLCOMMAND_OBJ_CHANNEL_1              22
#define MANUALCONTROLCOMMAND_OBJ_CHANNEL_2              24
#define MANUALCONTROLCOMMAND_OBJ_CHANNEL_3              26
#define MANUALCONTROLCOMMAND_OBJ_CHANNEL_4              28
#define MANUALCONTROLCOMMAND_OBJ_CHANNEL_5              30
#define MANUALCONTROLCOMMAND_OBJ_CHANNEL_6              32
#define MANUALCONTROLCOMMAND_OBJ_CHANNEL_7              34
#define MANUALCONTROLCOMMAND_OBJ_CHANNEL_8              36

#define MANUALCONTROLCOMMAND_OBJ_001_THROTTLE               0
#define MANUALCONTROLCOMMAND_OBJ_001_ROLL                   4
#define MANUALCONTROLCOMMAND_OBJ_001_PITCH                  8
#define MANUALCONTROLCOMMAND_OBJ_001_YAW                    12
#define MANUALCONTROLCOMMAND_OBJ_001_RAWRSSI                16
#define MANUALCONTROLCOMMAND_OBJ_001_COLLECTIVE             20
#define MANUALCONTROLCOMMAND_OBJ_001_RSSI                   24
#define MANUALCONTROLCOMMAND_OBJ_001_CHANNEL_0              26
#define MANUALCONTROLCOMMAND_OBJ_001_CHANNEL_1              28
#define MANUALCONTROLCOMMAND_OBJ_001_CHANNEL_2              30
#define MANUALCONTROLCOMMAND_OBJ_001_CHANNEL_3              32
#define MANUALCONTROLCOMMAND_OBJ_001_CHANNEL_4              34
#define MANUALCONTROLCOMMAND_OBJ_001_CHANNEL_5              36
#define MANUALCONTROLCOMMAND_OBJ_001_CHANNEL_6              38
#define MANUALCONTROLCOMMAND_OBJ_001_CHANNEL_7              40
#define MANUALCONTROLCOMMAND_OBJ_001_CHANNEL_8              42

// Enumeration options for field FlightMode 
// From build/uavobject-synthetics/flight/flightstatus.h
typedef enum { 
  FLIGHTSTATUS_FLIGHTMODE_MANUAL=0, 
  FLIGHTSTATUS_FLIGHTMODE_STABILIZED1=1, 
  FLIGHTSTATUS_FLIGHTMODE_STABILIZED2=2, 
  FLIGHTSTATUS_FLIGHTMODE_STABILIZED3=3, 
  FLIGHTSTATUS_FLIGHTMODE_AUTOTUNE=4, 
  FLIGHTSTATUS_FLIGHTMODE_ALTITUDEHOLD=5, 
  FLIGHTSTATUS_FLIGHTMODE_VELOCITYCONTROL=6, 
  FLIGHTSTATUS_FLIGHTMODE_POSITIONHOLD=7,
  FLIGHTSTATUS_FLIGHTMODE_RETURNTOHOME=8,
  FLIGHTSTATUS_FLIGHTMODE_PATHPLANNER=9
} FlightStatusFlightModeOptions;


// Enumeration options for field ControlSource 
// From build/uavobject-synthetics/flight/flightstatus.h
typedef enum { 
  FLIGHTSTATUS_CONTROLSOURCE_GEOFENCE=0, 
  FLIGHTSTATUS_CONTROLSOURCE_FAILSAFE=1, 
  FLIGHTSTATUS_CONTROLSOURCE_TRANSMITTER=2, 
  FLIGHTSTATUS_CONTROLSOURCE_TABLET=3 
} FlightStatusControlSourceOptions;



#define	FLIGHTTELEMETRYSTATS_CONNECT_TIMEOUT		10000
#define	GCSTELEMETRYSTATS_SEND_PERIOD			1000

#define	RESPOND_OBJ_LEN					8
#define	REQUEST_OBJ_LEN					8

#define UAVTALK_SYNC_VAL				0x3C

#define UAVTALK_TYPE_MASK				0xF8
#define UAVTALK_TYPE_VER				0x20

#define UAVTALK_TYPE_OBJ				(UAVTALK_TYPE_VER | 0x00)
#define UAVTALK_TYPE_OBJ_REQ				(UAVTALK_TYPE_VER | 0x01)
#define UAVTALK_TYPE_OBJ_ACK				(UAVTALK_TYPE_VER | 0x02)
#define UAVTALK_TYPE_ACK				(UAVTALK_TYPE_VER | 0x03)
#define UAVTALK_TYPE_NACK				(UAVTALK_TYPE_VER | 0x04)


typedef enum {
	UAVTALK_PARSE_STATE_WAIT_SYNC = 0,
	UAVTALK_PARSE_STATE_GOT_SYNC,
	UAVTALK_PARSE_STATE_GOT_MSG_TYPE,
	UAVTALK_PARSE_STATE_GOT_LENGTH,
	UAVTALK_PARSE_STATE_GOT_OBJID,
	UAVTALK_PARSE_STATE_GOT_INSTID,
	UAVTALK_PARSE_STATE_GOT_DATA,
	UAVTALK_PARSE_STATE_GOT_CRC
} uavtalk_parse_state_t;


typedef enum {
	TELEMETRYSTATS_STATE_DISCONNECTED = 0,
	TELEMETRYSTATS_STATE_HANDSHAKEREQ,
	TELEMETRYSTATS_STATE_HANDSHAKEACK,
	TELEMETRYSTATS_STATE_CONNECTED
} telemetrystats_state_t;


typedef struct __uavtalk_message {
	uint8_t Sync;
	uint8_t MsgType;
	uint16_t Length;
	uint32_t ObjID;
	uint16_t InstID;
	uint8_t Data[255];
	uint8_t Crc;
} uavtalk_message_t;


int uavtalk_read(void);
int uavtalk_state(void);


