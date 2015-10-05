/**
 ******************************************************************************
 *
 * @file       UAVTalk.ino
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

#include <Arduino.h>
#include "UAVTalk.h"


#ifdef PROTOCOL_UAVTALK

//#define DEBUG


static unsigned long last_gcstelemetrystats_send = 0;
static unsigned long last_flighttelemetry_connect = 0;
static uint8_t gcstelemetrystatus = TELEMETRYSTATS_STATE_DISCONNECTED;
static uint32_t gcstelemetrystats_objid = 0;
static uint8_t gcstelemetrystats_obj_len = 0;
static uint8_t gcstelemetrystats_obj_status = 0;
static uint8_t flighttelemetrystats_obj_status = 0;


// CRC lookup table
static const uint8_t crc_table[256] = {
	0x00, 0x07, 0x0e, 0x09, 0x1c, 0x1b, 0x12, 0x15, 0x38, 0x3f, 0x36, 0x31, 0x24, 0x23, 0x2a, 0x2d,
	0x70, 0x77, 0x7e, 0x79, 0x6c, 0x6b, 0x62, 0x65, 0x48, 0x4f, 0x46, 0x41, 0x54, 0x53, 0x5a, 0x5d,
	0xe0, 0xe7, 0xee, 0xe9, 0xfc, 0xfb, 0xf2, 0xf5, 0xd8, 0xdf, 0xd6, 0xd1, 0xc4, 0xc3, 0xca, 0xcd,
	0x90, 0x97, 0x9e, 0x99, 0x8c, 0x8b, 0x82, 0x85, 0xa8, 0xaf, 0xa6, 0xa1, 0xb4, 0xb3, 0xba, 0xbd,
	0xc7, 0xc0, 0xc9, 0xce, 0xdb, 0xdc, 0xd5, 0xd2, 0xff, 0xf8, 0xf1, 0xf6, 0xe3, 0xe4, 0xed, 0xea,
	0xb7, 0xb0, 0xb9, 0xbe, 0xab, 0xac, 0xa5, 0xa2, 0x8f, 0x88, 0x81, 0x86, 0x93, 0x94, 0x9d, 0x9a,
	0x27, 0x20, 0x29, 0x2e, 0x3b, 0x3c, 0x35, 0x32, 0x1f, 0x18, 0x11, 0x16, 0x03, 0x04, 0x0d, 0x0a,
	0x57, 0x50, 0x59, 0x5e, 0x4b, 0x4c, 0x45, 0x42, 0x6f, 0x68, 0x61, 0x66, 0x73, 0x74, 0x7d, 0x7a,
	0x89, 0x8e, 0x87, 0x80, 0x95, 0x92, 0x9b, 0x9c, 0xb1, 0xb6, 0xbf, 0xb8, 0xad, 0xaa, 0xa3, 0xa4,
	0xf9, 0xfe, 0xf7, 0xf0, 0xe5, 0xe2, 0xeb, 0xec, 0xc1, 0xc6, 0xcf, 0xc8, 0xdd, 0xda, 0xd3, 0xd4,
	0x69, 0x6e, 0x67, 0x60, 0x75, 0x72, 0x7b, 0x7c, 0x51, 0x56, 0x5f, 0x58, 0x4d, 0x4a, 0x43, 0x44,
	0x19, 0x1e, 0x17, 0x10, 0x05, 0x02, 0x0b, 0x0c, 0x21, 0x26, 0x2f, 0x28, 0x3d, 0x3a, 0x33, 0x34,
	0x4e, 0x49, 0x40, 0x47, 0x52, 0x55, 0x5c, 0x5b, 0x76, 0x71, 0x78, 0x7f, 0x6a, 0x6d, 0x64, 0x63,
	0x3e, 0x39, 0x30, 0x37, 0x22, 0x25, 0x2c, 0x2b, 0x06, 0x01, 0x08, 0x0f, 0x1a, 0x1d, 0x14, 0x13,
	0xae, 0xa9, 0xa0, 0xa7, 0xb2, 0xb5, 0xbc, 0xbb, 0x96, 0x91, 0x98, 0x9f, 0x8a, 0x8d, 0x84, 0x83,
	0xde, 0xd9, 0xd0, 0xd7, 0xc2, 0xc5, 0xcc, 0xcb, 0xe6, 0xe1, 0xe8, 0xef, 0xfa, 0xfd, 0xf4, 0xf3
};


static inline int8_t uavtalk_get_int8(uavtalk_message_t *msg, int pos) {
	return msg->Data[pos];
}


static inline int16_t uavtalk_get_int16(uavtalk_message_t *msg, int pos) {
	int16_t i;
	memcpy(&i, msg->Data+pos, sizeof(int16_t));
	return i;
}


static inline int32_t uavtalk_get_int32(uavtalk_message_t *msg, int pos) {
	int32_t i;
	memcpy(&i, msg->Data+pos, sizeof(int32_t));
	return i;
}


static inline float uavtalk_get_float(uavtalk_message_t *msg, int pos) {
	float f;
	memcpy(&f, msg->Data+pos, sizeof(float));
	return f;
}


void uavtalk_send_msg(uavtalk_message_t *msg) {
	uint8_t *d;
	uint8_t i;
	uint8_t c;
	
	if (PASSIVEMODE)
		return;
	
	c = (uint8_t) (msg->Sync);
	SerialPort1.write(c);
	msg->Crc = crc_table[0 ^ c];
	c = (uint8_t) (msg->MsgType);
	SerialPort1.write(c);
	msg->Crc = crc_table[msg->Crc ^ c];
	c = (uint8_t) (msg->Length & 0xff);
	SerialPort1.write(c);
	msg->Crc = crc_table[msg->Crc ^ c];
	c = (uint8_t) ((msg->Length >> 8) & 0xff);
	SerialPort1.write(c);
	msg->Crc = crc_table[msg->Crc ^ c];
	c = (uint8_t) (msg->ObjID & 0xff);
	SerialPort1.write(c);
	msg->Crc = crc_table[msg->Crc ^ c];
	c = (uint8_t) ((msg->ObjID >> 8) & 0xff);
	SerialPort1.write(c);
	msg->Crc = crc_table[msg->Crc ^ c];
	c = (uint8_t) ((msg->ObjID >> 16) & 0xff);
	SerialPort1.write(c);
	msg->Crc = crc_table[msg->Crc ^ c];
	c = (uint8_t) ((msg->ObjID >> 24) & 0xff);
	SerialPort1.write(c);
	msg->Crc = crc_table[msg->Crc ^ c];
	if (msg->Length > 8) {
	  d = msg->Data;
	  for (i=0; i<msg->Length-8; i++) {
		c = *d++;
		SerialPort1.write(c);
		msg->Crc = crc_table[msg->Crc ^ c];
          }
	}
	SerialPort1.write(msg->Crc);
}


void uavtalk_respond_object(uavtalk_message_t *msg_to_respond, uint8_t type) {
	uavtalk_message_t msg;
	
	msg.Sync	= UAVTALK_SYNC_VAL;
	msg.MsgType	= type;
	msg.Length	= RESPOND_OBJ_LEN;
	msg.ObjID	= msg_to_respond->ObjID;
	
	uavtalk_send_msg(&msg);
}


#if 0 // currently unused
void uavtalk_request_object(uint8_t id) {
	uavtalk_message_t msg;
	
	msg.Sync	= UAVTALK_SYNC_VAL;
	msg.MsgType	= UAVTALK_TYPE_OBJ_REQ;
	msg.Length	= REQUEST_OBJ_LEN;
	msg.ObjID	= id;
	
	uavtalk_send_msg(&msg);
}
#endif


void uavtalk_send_gcstelemetrystats(void) {
	uint8_t *d;
	uint8_t i;
	uavtalk_message_t msg;

	if (gcstelemetrystats_obj_len == 0) return;	// we didn't know yet with which version we are talking, so be quiet

	msg.Sync	= UAVTALK_SYNC_VAL;
	msg.MsgType	= UAVTALK_TYPE_OBJ_ACK;
	msg.Length	= gcstelemetrystats_obj_len + 8;
	msg.ObjID	= gcstelemetrystats_objid;

	d = msg.Data;
	for (i=0; i<gcstelemetrystats_obj_len; i++) {
		*d++ = 0;
	}

	msg.Data[gcstelemetrystats_obj_status] = gcstelemetrystatus;
	// remaining data unused and unset
	
	uavtalk_send_msg(&msg);
	last_gcstelemetrystats_send = millis();
}


void set_telemetrystats_values(uint32_t ObjID) {

	if (gcstelemetrystats_obj_len != 0) return;	// we already know yet with which version we are talking, so do nothing

	switch (ObjID) {
		case FLIGHTTELEMETRYSTATS_OBJID:
			gcstelemetrystats_objid = GCSTELEMETRYSTATS_OBJID;
			gcstelemetrystats_obj_len = GCSTELEMETRYSTATS_OBJ_LEN;
			gcstelemetrystats_obj_status = GCSTELEMETRYSTATS_OBJ_STATUS;
			flighttelemetrystats_obj_status = FLIGHTTELEMETRYSTATS_OBJ_STATUS;
		break;
		case FLIGHTTELEMETRYSTATS_OBJID_001:
			gcstelemetrystats_objid = GCSTELEMETRYSTATS_OBJID_001;
			gcstelemetrystats_obj_len = GCSTELEMETRYSTATS_OBJ_LEN_001;
			gcstelemetrystats_obj_status = GCSTELEMETRYSTATS_OBJ_STATUS_001;
			flighttelemetrystats_obj_status = FLIGHTTELEMETRYSTATS_OBJ_STATUS_001;
		break;
	}
}


uint8_t uavtalk_parse_char(uint8_t c, uavtalk_message_t *msg) {
	static uint8_t status = UAVTALK_PARSE_STATE_WAIT_SYNC;
	static uint8_t crc = 0;
	static uint8_t cnt = 0;

	switch (status) {
		case UAVTALK_PARSE_STATE_WAIT_SYNC:
			if (c == UAVTALK_SYNC_VAL) {
				status = UAVTALK_PARSE_STATE_GOT_SYNC;
				msg->Sync = c;
				crc = crc_table[0 ^ c];
			}
		break;
		case UAVTALK_PARSE_STATE_GOT_SYNC:
			crc = crc_table[crc ^ c];
			if ((c & UAVTALK_TYPE_MASK) == UAVTALK_TYPE_VER) {
				status = UAVTALK_PARSE_STATE_GOT_MSG_TYPE;
				msg->MsgType = c;
				cnt = 0;
			}
			else {
				status = UAVTALK_PARSE_STATE_WAIT_SYNC;
			}
		break;
		case UAVTALK_PARSE_STATE_GOT_MSG_TYPE:
			crc = crc_table[crc ^ c];
			cnt++;
			if (cnt < 2) {
				msg->Length = ((uint16_t) c);
			}
			else {
				msg->Length += ((uint16_t) c) << 8;
                                if ((msg->Length < 8) || (msg->Length > 255 + 8)) {
                                       // Drop corrupted messages:
                                       // Minimal length is 8 (headers)
                                       // Maximum is 8 (headers) + 255 (Data) + 2 (Optional Instance Id)
                                       // As we are not parsing Instance Id, 255 is a hard maximum. 
				       status = UAVTALK_PARSE_STATE_WAIT_SYNC;
                                } else {
				       status = UAVTALK_PARSE_STATE_GOT_LENGTH;
				       cnt = 0;
                                }
			}
		break;
		case UAVTALK_PARSE_STATE_GOT_LENGTH:
			crc = crc_table[crc ^ c];
			cnt++;
			switch (cnt) {
				case 1:
					msg->ObjID = ((uint32_t) c);
				break;
				case 2:
					msg->ObjID += ((uint32_t) c) << 8;
				break;
				case 3:
					msg->ObjID += ((uint32_t) c) << 16;
				break;
				case 4:
					msg->ObjID += ((uint32_t) c) << 24;
					if (msg->Length == 8) { // no data exists
						status = UAVTALK_PARSE_STATE_GOT_DATA;
					} else {
						status = UAVTALK_PARSE_STATE_GOT_OBJID;
					}
					cnt = 0;
				break;
			}
		break;
		// TODO enhancement: we don't need/use INSTID and so we currently don't parse it
		case UAVTALK_PARSE_STATE_GOT_OBJID:
		case UAVTALK_PARSE_STATE_GOT_INSTID:
			crc = crc_table[crc ^ c];
			cnt++;
			msg->Data[cnt - 1] = c;
			if (cnt >= msg->Length - 8) {
				status = UAVTALK_PARSE_STATE_GOT_DATA;
				cnt = 0;
			}
		break;
		case UAVTALK_PARSE_STATE_GOT_DATA:
			msg->Crc = c;
			status = UAVTALK_PARSE_STATE_GOT_CRC;
		break;
	}
	
	if (status == UAVTALK_PARSE_STATE_GOT_CRC) {
		status = UAVTALK_PARSE_STATE_WAIT_SYNC;
		if (crc == msg->Crc) {
			return msg->Length;
		} else {
			return 0;
		}
	} else {
		return 0;
	}
}


int uavtalk_read(void) {
	static uint8_t crlf_count = 0;
	static uavtalk_message_t msg;
	uint8_t show_prio_info = 0;
	
	// grabbing data
	while (SerialPort1.available() > 0) {
		uint8_t c = SerialPort1.read();
		
		// parse data to msg
		if (uavtalk_parse_char(c, &msg)) {
            telemetry_ok = true;
            lastpacketreceived = millis();
			// consume msg
			switch (msg.ObjID) {
				case FLIGHTTELEMETRYSTATS_OBJID:
				case FLIGHTTELEMETRYSTATS_OBJID_001:
					set_telemetrystats_values(msg.ObjID);
					switch (msg.Data[flighttelemetrystats_obj_status]) {
						case TELEMETRYSTATS_STATE_DISCONNECTED:
							gcstelemetrystatus = TELEMETRYSTATS_STATE_HANDSHAKEREQ;
							uavtalk_send_gcstelemetrystats();
						break;
						case TELEMETRYSTATS_STATE_HANDSHAKEACK:
							gcstelemetrystatus = TELEMETRYSTATS_STATE_CONNECTED;
							uavtalk_send_gcstelemetrystats();
						break;
						case TELEMETRYSTATS_STATE_CONNECTED:
							gcstelemetrystatus = TELEMETRYSTATS_STATE_CONNECTED;
							last_flighttelemetry_connect = millis();
						break;
					}
				break;
				case ATTITUDEACTUAL_OBJID:
				case ATTITUDESTATE_OBJID:
					last_flighttelemetry_connect = millis();
					show_prio_info = 1;
        			uav_roll		= (int16_t) uavtalk_get_float(&msg, ATTITUDEACTUAL_OBJ_ROLL);
        			uav_pitch		= (int16_t) uavtalk_get_float(&msg, ATTITUDEACTUAL_OBJ_PITCH);
        			uav_heading			= (int16_t) uavtalk_get_float(&msg, ATTITUDEACTUAL_OBJ_YAW);
				break;
				case FLIGHTSTATUS_OBJID:
				case FLIGHTSTATUS_OBJID_001:
				case FLIGHTSTATUS_OBJID_002:
				case FLIGHTSTATUS_OBJID_003:
                case FLIGHTSTATUS_OBJID_004:
                case FLIGHTSTATUS_OBJID_005:
                                        uav_arm = uavtalk_get_int8(&msg, FLIGHTSTATUS_OBJ_ARMED);
                                        //remap flight modes id to Ghettostation ones
                                        switch (uavtalk_get_int8(&msg, FLIGHTSTATUS_OBJ_FLIGHTMODE)) {
                                            case 0: uav_flightmode = 0;  break;   //manual
                                            case 1: uav_flightmode = 5;  break;   //stabilized 1
                                            case 2: uav_flightmode = 6;  break;   //stabilized 2
                                            case 3: uav_flightmode = 7;  break;   //stabilized 3
                                            case 4: uav_flightmode = 16; break;   //autotune (unknown)
                                            case 5: uav_flightmode = 8;  break;   //altitude hold
                                            case 6: uav_flightmode = 16; break;   //velocity control (unknown)
                                            case 7: uav_flightmode = 9;  break;   //pos hold
                                            case 8: uav_flightmode = 13; break;   //RTH
                                            case 9: uav_flightmode = 10; break;   //pathplanner (auto)
                                        }

        
                                        //if (msg.ObjID==FLIGHTSTATUS_OBJID_004) {
                                          uav_failsafe = (uavtalk_get_int8(&msg, FLIGHTSTATUS_OBJ_CONTROLSOURCE) == 1) ? 1 : 0; // Taulabs only
                                        //}
                                break;
                                case MANUALCONTROLCOMMAND_OBJID: // OP
//                	                uav_chan5_raw		= uavtalk_get_int16(&msg, MANUALCONTROLCOMMAND_OBJ_CHANNEL_4);
//					uav_chan6_raw		= uavtalk_get_int16(&msg, MANUALCONTROLCOMMAND_OBJ_CHANNEL_6);
//					uav_chan7_raw		= uavtalk_get_int16(&msg, MANUALCONTROLCOMMAND_OBJ_CHANNEL_7);
//					uav_chan8_raw		= uavtalk_get_int16(&msg, MANUALCONTROLCOMMAND_OBJ_CHANNEL_8);
                                break;
				case MANUALCONTROLCOMMAND_OBJID_001: //Taulabs
                                        uav_rssi            = (uint8_t) uavtalk_get_int16( &msg, MANUALCONTROLCOMMAND_OBJ_001_RSSI);
                                        if ((uav_rssi < 0 ) || uav_rssi > 100 ) uav_rssi = 0; // rssi > 100 means it entered taulabs failsafe mode
//                                        uav_chan5_raw		= uavtalk_get_int16(&msg, MANUALCONTROLCOMMAND_OBJ_001_CHANNEL_4);
//					uav_chan6_raw		= uavtalk_get_int16(&msg, MANUALCONTROLCOMMAND_OBJ_001_CHANNEL_6);
//					uav_chan7_raw		= uavtalk_get_int16(&msg, MANUALCONTROLCOMMAND_OBJ_001_CHANNEL_7);
//					uav_chan8_raw		= uavtalk_get_int16(&msg, MANUALCONTROLCOMMAND_OBJ_001_CHANNEL_8);

				break;
				case GPSPOSITION_OBJID:
                case GPSPOSITION_OBJID_001:
				case GPSPOSITIONSENSOR_OBJID:
					uav_lat			= uavtalk_get_int32(&msg, GPSPOSITION_OBJ_LAT);
					uav_lon			= uavtalk_get_int32(&msg, GPSPOSITION_OBJ_LON);
					uav_satellites_visible	= (uint8_t) uavtalk_get_int8(&msg, GPSPOSITION_OBJ_SATELLITES);
					uav_fix_type		= (uint8_t) uavtalk_get_int8(&msg, GPSPOSITION_OBJ_STATUS);
					uav_gpsheading		= (int16_t) uavtalk_get_float(&msg, GPSPOSITION_OBJ_HEADING);
                                        #ifndef BARO_ALT
					uav_alt			= (int32_t) round (uavtalk_get_float(&msg, GPSPOSITION_OBJ_ALTITUDE) * 100.0f);
                                        #endif
					uav_groundspeed		= (uint16_t)uavtalk_get_float(&msg, GPSPOSITION_OBJ_GROUNDSPEED);
				break;

				case FLIGHTBATTERYSTATE_OBJID:
        			        uav_bat		= (int16_t) (1000.0 * uavtalk_get_float(&msg, FLIGHTBATTERYSTATE_OBJ_VOLTAGE));
					uav_current	= (int16_t) (100.0 * uavtalk_get_float(&msg, FLIGHTBATTERYSTATE_OBJ_CURRENT));
					uav_amp		= (int16_t) uavtalk_get_float(&msg, FLIGHTBATTERYSTATE_OBJ_CONSUMED_ENERGY);
				break;

				case BAROALTITUDE_OBJID:
				case BAROSENSOR_OBJID:
                                        #ifdef BARO_ALT
					uav_alt		= (int32_t) round (uavtalk_get_float(&msg, BAROALTITUDE_OBJ_ALTITUDE) * 100.0f);
                                        #endif
				break;
				case OPLINKSTATUS_OBJID:
				case OPLINKSTATUS_OBJID_001:
        			        uav_rssi        = uavtalk_get_int8(&msg, OPLINKSTATUS_OBJ_RSSI);
        			        uav_linkquality	= uavtalk_get_int8(&msg, OPLINKSTATUS_OBJ_LINKQUALITY);
				break;
				
			}
			if (msg.MsgType == UAVTALK_TYPE_OBJ_ACK) {
				uavtalk_respond_object(&msg, UAVTALK_TYPE_ACK);
			}
		}

		//delayMicroseconds(190);  // wait at least 1 byte
	}
	
	// check connect timeout
	if (last_flighttelemetry_connect + FLIGHTTELEMETRYSTATS_CONNECT_TIMEOUT < millis()) {
		gcstelemetrystatus = TELEMETRYSTATS_STATE_DISCONNECTED;
		show_prio_info = 1;
	}
	
	// periodically send gcstelemetrystats
	if (last_gcstelemetrystats_send + GCSTELEMETRYSTATS_SEND_PERIOD < millis()) {
		uavtalk_send_gcstelemetrystats();
	}

        return show_prio_info;
}


int uavtalk_state(void)
{
	return gcstelemetrystatus;
}

#endif // PROTOCOL_UAVTALK

