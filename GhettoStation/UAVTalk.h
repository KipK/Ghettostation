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






//#define VERSION_RELEASE_12_10_1	// OpenPilot-RELEASE 12.10.1	Release from 2012.10.26
//#define VERSION_RELEASE_12_10_2		// OpenPilot-RELEASE 12.10.2	Release from 2012.12.21	'Mayan Apocalypse'


// TODO enhancement:
// Generate the following automatically out of the XML files.
//
// These object constants are version dependent!
//
// Short hints getting them manually:
// http://wiki.openpilot.org/display/Doc/Windows%3A+Building+and+Packaging
// git clone git://git.openpilot.org/OpenPilot.git OpenPilot
// QT Creator > Datei oder Projekt �ffnen... uavobjgenerator
// generate release and copy exe to <top>\ground\uavobjgenerator
// uavobjgenerator -flight ..\..\shared\uavobjectdefinition ..\..\

//#if defined VERSION_RELEASE_12_10_1 || defined VERSION_RELEASE_12_10_2

#define	FLIGHTTELEMETRYSTATS_OBJID			0x2F7E2902
#define	GCSTELEMETRYSTATS_OBJID				0xABC72744
#define	ATTITUDEACTUAL_OBJID				0x33DAD5E6
#define	FLIGHTSTATUS_OBJID				0x9B6A127E
#define	MANUALCONTROLCOMMAND_OBJID			0x1E82C2D2
#define GPSPOSITION_OBJID				0xE2A323B6
#define GPSTIME_OBJID					0xD4478084
#define GPSVELOCITY_OBJID				0x8245DC80
#define SYSTEMALARMS_OBJID				0x7BD9C77A

#define	FLIGHTTELEMETRYSTATS_OBJ_STATUS			20

#define	GCSTELEMETRYSTATS_OBJ_LEN			29
#define	GCSTELEMETRYSTATS_OBJ_STATUS			20

#define	ATTITUDEACTUAL_OBJ_ROLL				16
#define	ATTITUDEACTUAL_OBJ_PITCH			20
#define	ATTITUDEACTUAL_OBJ_YAW				24

#define	FLIGHTSTATUS_OBJ_ARMED				0
#define	FLIGHTSTATUS_OBJ_FLIGHTMODE			1

#define	MANUALCONTROLCOMMAND_OBJ_THROTTLE		0
#define	MANUALCONTROLCOMMAND_OBJ_CHANNEL_0		20
#define	MANUALCONTROLCOMMAND_OBJ_CHANNEL_1		22
#define	MANUALCONTROLCOMMAND_OBJ_CHANNEL_2		24
#define	MANUALCONTROLCOMMAND_OBJ_CHANNEL_3		26
#define	MANUALCONTROLCOMMAND_OBJ_CHANNEL_4		28
#define	MANUALCONTROLCOMMAND_OBJ_CHANNEL_5		30
#define	MANUALCONTROLCOMMAND_OBJ_CHANNEL_6		32
#define	MANUALCONTROLCOMMAND_OBJ_CHANNEL_7		34
#define	MANUALCONTROLCOMMAND_OBJ_CHANNEL_8		36

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

#define	GPSTIME_OBJ_YEAR				0
#define	GPSTIME_OBJ_MONTH				2
#define	GPSTIME_OBJ_DAY					3
#define	GPSTIME_OBJ_HOUR				4
#define	GPSTIME_OBJ_MINUTE				5
#define	GPSTIME_OBJ_SECOND				6

#define	GPSVELOCITY_OBJ_NORTH				0
#define	GPSVELOCITY_OBJ_EAST				4
#define	GPSVELOCITY_OBJ_DOWN				8

#define	SYSTEMALARMS_ALARM_OUTOFMEMORY			0
#define	SYSTEMALARMS_ALARM_STACKOVERFLOW		1
#define	SYSTEMALARMS_ALARM_CPUOVERLOAD			2
#define	SYSTEMALARMS_ALARM_EVENTSYSTEM			3
#define	SYSTEMALARMS_ALARM_TELEMETRY			4
#define	SYSTEMALARMS_ALARM_MANUALCONTROL		5
#define	SYSTEMALARMS_ALARM_ACTUATOR			6
#define	SYSTEMALARMS_ALARM_ATTITUDE			7
#define	SYSTEMALARMS_ALARM_SENSORS			8
#define	SYSTEMALARMS_ALARM_STABILIZATION		9
#define	SYSTEMALARMS_ALARM_GUIDANCE			10
#define	SYSTEMALARMS_ALARM_BATTERY			11
#define	SYSTEMALARMS_ALARM_FLIGHTTIME			12
#define	SYSTEMALARMS_ALARM_I2C				13
#define	SYSTEMALARMS_ALARM_GPS				14
#define	SYSTEMALARMS_ALARM_BOOTFAULT			15

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
  FLIGHTSTATUS_FLIGHTMODE_POSITIONHOLD=7 
} FlightStatusFlightModeOptions;

//#endif // defined VERSION_RELEASE_12_10_1 || defined VERSION_RELEASE_12_10_2




#define	UAVTALK_MODE_PASSIVE				0x01	// do not send any UAVTalk packets

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


