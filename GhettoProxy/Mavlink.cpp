#ifdef PROTOCOL_MAVLINK
//#include "../mavlink/include/mavlink.h"
#include "../GCS_MAVLink/include/mavlink/v1.0/mavlink_types.h"
#include "../GCS_MAVLink/include/mavlink/v1.0/ardupilotmega/mavlink.h"

// true when we have received at least 1 MAVLink packet
static bool mavlink_active;
static uint8_t crlf_count = 0;
static boolean      mavbeat = 0;
static uint8_t apm_mav_type;

static int packet_drops = 0;
static int parse_error = 0;
mavlink_message_t msg; 
mavlink_status_t status;
void request_mavlink_rates()
{
    const int  maxStreams = 6;
    const uint8_t MAVStreams[maxStreams] = {
        MAV_DATA_STREAM_RAW_SENSORS,
        MAV_DATA_STREAM_EXTENDED_STATUS,
        MAV_DATA_STREAM_RC_CHANNELS,
        MAV_DATA_STREAM_POSITION,
        MAV_DATA_STREAM_EXTRA1, 
        MAV_DATA_STREAM_EXTRA2};
    const uint16_t MAVRates[maxStreams] = {0x02, 0x02, 0x05, 0x02, 0x05, 0x02};
    for (int i=0; i < maxStreams; i++) {
        mavlink_msg_request_data_stream_pack(127, 0, &msg, 7, 1, MAVStreams[i], MAVRates[i], 1);
        uint8_t buf[MAVLINK_MAX_PACKET_LEN];
        uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);
        Serial.write(buf, len);
    }
}

void read_mavlink(){
    mavlink_message_t msg; 
    mavlink_status_t status;

    //grabing data 
    while(Serial.available() > 0) { 
        uint8_t c = Serial.read();
        //Serial.print(c,HEX);Serial.print(" ");

        //trying to grab msg  
        //if(mavlink_parse_char(MAVLINK_COMM_0, c, &msg, &status)) {
        if(mavlink_parse_char(MAVLINK_COMM_0, c, &msg, &status)) {
            mavlink_active = 1;
            lastpacketreceived = millis();
            //handle msg
            switch(msg.msgid) {
            case MAVLINK_MSG_ID_HEARTBEAT:
                {
                    mavbeat = 1;
                    //apm_mav_system    = msg.sysid;
                    //apm_mav_component = msg.compid;
   // TODO there's different flightmodes value depending of arducopter or arduplane. 
   // Need to check first vehicle type, then we will apply correct flightmode map.
   // for now only arducopter is supported.
                    apm_mav_type      = mavlink_msg_heartbeat_get_type(&msg);
                    uav_flightmode = (uint8_t)mavlink_msg_heartbeat_get_custom_mode(&msg);
                    if (apm_mav_type == 2)    //ArduCopter MultiRotor or ArduCopter Heli
                    {                         
                        switch (uav_flightmode) {
                            case 0: uav_flightmode = 2;   break;      //Stabilize 
                            case 1: uav_flightmode = 1;   break;      //Acro 
                            case 2: uav_flightmode = 8;   break;      //Alt Hold
                            case 3: uav_flightmode = 10;  break;      //Auto
                            case 4: uav_flightmode = 10;  break;      //Guided -> Auto
                            case 5: uav_flightmode = 9;   break;      //Loiter
                            case 6: uav_flightmode = 13;  break;      //RTL
                            case 7: uav_flightmode = 12;  break;      //Circle
                            case 8: uav_flightmode = 9 ;  break;      //Position -> Loiter
                            case 9: uav_flightmode = 15;  break;      //Land
                            case 10: uav_flightmode = 9;  break;      //OF LOiter
                            case 11: uav_flightmode = 5;  break;      //Drift -> Stabilize1
                            case 12: uav_flightmode = 6;  break;      //Sport -> Stabilize2
                            default: uav_flightmode = 19; break;      //Unknown                      
                        }
                    }
                    else if (apm_mav_type == 1)    //ArduPlane
                    {
                        switch (uav_flightmode) {
                            case 0:                       break;       //Manual
                            case 1:  uav_flightmode = 12; break;       //Circle
                            case 2:                       break;       //Stabilize
                            case 5:  uav_flightmode = 16; break;       //FlyByWire A
                            case 6:  uav_flightmode = 17; break;       //FlyByWire B
                            case 10:                      break;       //Auto
                            case 11: uav_flightmode = 13; break;       //RTH
                            case 12: uav_flightmode = 9;  break;       //Loiter
                            default: uav_flightmode = 19; break;       //Unknown
                        }
                    }
                    //Mode (arducoper armed/disarmed)
                    uav_arm = mavlink_msg_heartbeat_get_base_mode(&msg);
                    if (getBit(uav_arm,7)) 
                        uav_arm = 1;
                    else 
                        uav_arm = 0;
                }
                break;
            case MAVLINK_MSG_ID_SYS_STATUS:
                {
                    uav_bat = mavlink_msg_sys_status_get_voltage_battery(&msg);
                    uav_current = mavlink_msg_sys_status_get_current_battery(&  msg)/10;  
                    uav_amp = (int16_t)mavlink_msg_sys_status_get_battery_remaining(  &msg); //Mavlink send battery remaining % , will use this instead
                }
                break;

            case MAVLINK_MSG_ID_GPS_RAW_INT:
                {
                    uav_lat =  mavlink_msg_gps_raw_int_get_lat(&msg) ;
                    uav_lon =  mavlink_msg_gps_raw_int_get_lon(&msg);
                   #ifndef BARO_ALT
                    uav_alt = (int32_t)round(mavlink_msg_gps_raw_int_get_alt(&msg)/10.0f); // from mm to cm
                   #endif
                    uav_fix_type = (uint8_t) mavlink_msg_gps_raw_int_get_fix_type(&msg);
                    uav_satellites_visible = (uint8_t) mavlink_msg_gps_raw_int_get_satellites_visible(&msg);
                    uav_gpsheading = (int16_t) mavlink_msg_gps_raw_int_get_cog(&msg);
                }
                break; 
            case MAVLINK_MSG_ID_VFR_HUD:
                {
                    uav_groundspeed = (int)round(mavlink_msg_vfr_hud_get_groundspeed(&msg));
                    uav_airspeed = (uint8_t)round(mavlink_msg_vfr_hud_get_airspeed(&msg));
                  #ifdef BARO_ALT
                    uav_alt = (int32_t)round(mavlink_msg_vfr_hud_get_alt(&msg) * 100.0f);  // from m to cm
                  #endif
                }
                break;
            case MAVLINK_MSG_ID_ATTITUDE:
                {
                    uav_roll = (int16_t)round(toDeg(mavlink_msg_attitude_get_roll(&msg)));
                    uav_pitch = (int16_t)round(toDeg(mavlink_msg_attitude_get_pitch(&msg)));
                    uav_heading = (int16_t)round(toDeg(mavlink_msg_attitude_get_yaw(&msg)));
                    //if (uav_heading >= 180 ) uav_heading = -360+uav_heading; //convert from 0-360 to -180/180°
                }
                break;

            case MAVLINK_MSG_ID_RC_CHANNELS_RAW:
                {

                   uav_rssi      = mavlink_msg_rc_channels_raw_get_rssi(&msg);
//                   uav_chan5_raw = mavlink_msg_rc_channels_raw_get_chan5_raw(&msg);
//                   uav_chan6_raw = mavlink_msg_rc_channels_raw_get_chan6_raw(&msg);
//                   uav_chan7_raw = mavlink_msg_rc_channels_raw_get_chan7_raw(&msg);
//                   uav_chan8_raw = mavlink_msg_rc_channels_raw_get_chan8_raw(&msg);
                }
                break;           
            }
        }
        //delayMicroseconds(138);
    }
    // Update global packet drops counter
    packet_drops += status.packet_rx_drop_count;
    parse_error += status.parse_error;

}

#endif
