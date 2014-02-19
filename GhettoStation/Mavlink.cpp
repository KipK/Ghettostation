#ifdef PROTOCOL_MAVLINK

#include "../mavlink/include/mavlink.h"

// true when we have received at least 1 MAVLink packet
static bool mavlink_active;
static uint8_t crlf_count = 0;

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
        SerialPort1.write(buf, len);
    }
}

void read_mavlink(){
    mavlink_message_t msg; 
    mavlink_status_t status;

    //grabing data 
    while(SerialPort1.available() > 0) { 
        uint8_t c = SerialPort1.read();
        //Serial.print(c,HEX);Serial.print(" ");

        //trying to grab msg  
        if(mavlink_parse_char(MAVLINK_COMM_0, c, &msg, &status)) {
            mavlink_active = 1;
            telemetry_ok = true;
            protocol = "MAV";
            lastpacketreceived = millis();
            //handle msg
            switch(msg.msgid) {
            case MAVLINK_MSG_ID_HEARTBEAT:
                {
                    mavbeat = 1;
                    //apm_mav_system    = msg.sysid;
                    //apm_mav_component = msg.compid;

                    uav_flightmode = (uint8_t)mavlink_msg_heartbeat_get_custom_mode(&msg);
                    if (uav_flightmode == 0) uav_flightmode = 2; //Stabilize 
                    if (uav_flightmode == 1) uav_flightmode = 1; //Acro 
                    if (uav_flightmode == 2) uav_flightmode = 8; //Alt Hold
                    if (uav_flightmode == 3) uav_flightmode = 10; //Auto
                    if (uav_flightmode == 4) uav_flightmode = 10; //Guided -> Auto
                    if (uav_flightmode == 5) uav_flightmode = 9; //Loiter
                    if (uav_flightmode == 6) uav_flightmode = 13; //RTL 
                    if (uav_flightmode == 7) uav_flightmode = 12; //Circle
                    if (uav_flightmode == 8) uav_flightmode = 9; //Position -> Loiter
                    if (uav_flightmode == 9) uav_flightmode = 15; //Land 
                    if (uav_flightmode == 10) uav_flightmode = 9; //OF Loiter
                    if (uav_flightmode == 11) uav_flightmode = 5; //Drift -> Stabilize1
                    if (uav_flightmode == 12) uav_flightmode = 6; //Sport -> Stabilize2
                    
                    //Mode (arducoper armed/disarmed)
                    uav_arm = mavlink_msg_heartbeat_get_base_mode(&msg);
                    if(getBit(uav_arm,7)) uav_arm = 1;
                    else uav_arm = 0;
       
                    lastMAVBeat = millis();
                    if(waitingMAVBeats == 1){
                        enable_mav_request = 1;
                    }
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
                    uav_lat =  (float)mavlink_msg_gps_raw_int_get_lat(&msg)/10000000.0f ;
                    uav_lon =  (float)mavlink_msg_gps_raw_int_get_lon(&msg)/10000000.0f;
                   #ifndef BARO_ALT
                    uav_alt = (int)round(mavlink_msg_gps_raw_int_get_alt(&msg)/100.0f); // to decimeters
                   #endif
                    uav_fix_type = mavlink_msg_gps_raw_int_get_fix_type(&msg);
                    uav_satellites_visible = mavlink_msg_gps_raw_int_get_satellites_visible(&msg);
                    uav_gpsheading = (int16_t) mavlink_msg_gps_raw_int_get_cog(&msg);
                }
                break; 
            case MAVLINK_MSG_ID_VFR_HUD:
                {
                    uav_groundspeed = (int)round(mavlink_msg_vfr_hud_get_groundspeed(&msg));
                    uav_airspeed = (uint8_t)round(mavlink_msg_vfr_hud_get_airspeed(&msg));
                  #ifdef BARO_ALT
                    uav_alt = (int)round(mavlink_msg_vfr_hud_get_alt(&msg) * 10);  // to decimeters
                  #endif
                }
                break;
            case MAVLINK_MSG_ID_ATTITUDE:
                {
                    uav_roll = (int16_t)round(toDeg(mavlink_msg_attitude_get_roll(&msg)));
                    uav_pitch = (int16_t)round(toDeg(mavlink_msg_attitude_get_pitch(&msg)));
                    uav_heading = (int16_t)round(toDeg(mavlink_msg_attitude_get_yaw(&msg)));
                }
                break;

            case MAVLINK_MSG_ID_RC_CHANNELS_RAW:
                {

                   uav_rssi      = mavlink_msg_rc_channels_raw_get_rssi(&msg);
                   uav_chan5_raw = mavlink_msg_rc_channels_raw_get_chan5_raw(&msg);
                   uav_chan6_raw = mavlink_msg_rc_channels_raw_get_chan6_raw(&msg);
                   uav_chan7_raw = mavlink_msg_rc_channels_raw_get_chan7_raw(&msg);
                   uav_chan8_raw = mavlink_msg_rc_channels_raw_get_chan8_raw(&msg);
                }
                break;           
            }
        }
        delayMicroseconds(138);
        //next one
    }
    // Update global packet drops counter
    packet_drops += status.packet_rx_drop_count;
    parse_error += status.parse_error;

}

#endif
