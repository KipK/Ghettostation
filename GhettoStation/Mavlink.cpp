/*/*

 code borrowed from Andy Little DIY antenna Tracker: https://github.com/kwikius/mavlink_to_frsky/tree/master/arduino

(
  Some parts of this work are based on: 
  http://code.google.com/p/arducam-osd/source/browse/trunk/ArduCAM_OSD/MAVLink.ino
  Copyright (c) 2011. Sandro Benigno
)

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program. If not, see <http://www.gnu.org/licenses/>
 
*/

#ifdef PROTOCOL_MAVLINK

#include <Arduino.h>
#include <cstdint>
#include <mavlink.h>


mavlink_system_t mavlink_system = {12,1,0,0};

void comm_send_ch(mavlink_channel_t chan, uint8_t ch)
{
#if defined __AVR_ATmega32U4_
  Serial1.write(ch);
#elseif defined TEENSYPLUS2_
	Uart.write(ch);
#else
  Serial.write(ch);
#endif
}

namespace {

  
   uint32_t  num_heartbeats = 0U;
   
}

// return number of heartbeats since boot
uint32_t get_num_heartbeats()
{
   return  num_heartbeats;
}

namespace{

   uint8_t  apm_mav_type;
   uint8_t  apm_mav_system; 
   uint8_t  apm_mav_component;

   struct mav_sr_t{
      uint8_t stream_number;
      uint16_t rate;
   };

// The same as the minimosd

   mav_sr_t MAVStreams[] = {
      {MAV_DATA_STREAM_RAW_SENSORS,0x02},
      {MAV_DATA_STREAM_EXTENDED_STATUS,0x02},
      {MAV_DATA_STREAM_RC_CHANNELS,0x05},
      {MAV_DATA_STREAM_POSITION,0x02},
      {MAV_DATA_STREAM_EXTRA1, 0x05},
      {MAV_DATA_STREAM_EXTRA2,0x02}
   };

   void request_mavlink_rate(uint8_t mav_system, uint8_t mav_component, uint8_t stream_number, uint16_t rate );

   void request_mavlink_rate(uint8_t system, uint8_t component, uint8_t stream_number, uint16_t rate)
   {
        mavlink_msg_request_data_stream_send(MAVLINK_COMM_0,system, component, stream_number, rate, 1);
   }

   bool rate_request_sent_flag = false;

   void  request_mavlink_rates()
   {
     static const size_t  NumStreams = sizeof(MAVStreams) / sizeof(mav_sr_t);
     for (size_t i=0; i < NumStreams; ++i) {
         request_mavlink_rate(apm_mav_system, apm_mav_component,MAVStreams[i].stream_number, MAVStreams[i].rate);
     }
     rate_request_sent_flag = true;
   }

     void do_mavlink_heartbeat(mavlink_message_t* pmsg);
     void do_mavlink_sys_status(mavlink_message_t *pmsg);
   #ifndef MAVLINK10
     void do_mavlink_gps_raw(mavlink_message_t * pmsg);
     void do_mavlink_gps_status(mavlink_message_t * pmsg);
   #else
     void do_mavlink_gps_raw_int(mavlink_message_t * pmsg);
   #endif
     void do_mavlink_vfr_hud(mavlink_message_t * pmsg);

} // ~namespace

void mavlink_read()
{

   static bool mavlink_active = false;
   static int packet_drops = 0;
   static int  parse_error = 0;
  
   mavlink_message_t msg; 
   mavlink_status_t status;

#if defined __AVR_ATmega32U4_  
   while (Serial1.available() > 0) {
            uint8_t ch = Serial1.read();
			
#elseif defined TEENSYPLUS2
   while (Uart.available() > 0) {
            uint8_t ch = Uart.read();
#else
   while (Serial.available() > 0) {
            uint8_t ch = Serial.read();
#endif

      if(mavlink_parse_char(MAVLINK_COMM_0, ch, &msg, &status)) {
         mavlink_active = true;
         protocol = "MAV";
         switch(msg.msgid) {
            case MAVLINK_MSG_ID_HEARTBEAT:
               do_mavlink_heartbeat(&msg);
               if ( rate_request_sent_flag == false){
                  request_mavlink_rates();
               }
            break;
#ifndef MAVLINK10 
            case MAVLINK_MSG_ID_GPS_RAW:
               do_mavlink_gps_raw(&msg);
            break;
            case MAVLINK_MSG_ID_GPS_STATUS:
               do_mavlink_gps_status(&msg);
            break;
#else
            case MAVLINK_MSG_ID_GPS_RAW_INT:
               do_mavlink_gps_raw_int(&msg);
            break;
#endif          
            case MAVLINK_MSG_ID_VFR_HUD:
               do_mavlink_vfr_hud(&msg);
            break;
            default:
            break;
         }
      }
   }
 
  packet_drops += status.packet_rx_drop_count;
  parse_error += status.parse_error;
}

namespace {

  void do_mavlink_heartbeat(mavlink_message_t* pmsg)
  {    
      apm_mav_system    = pmsg->sysid;
      apm_mav_component = pmsg->compid;
      apm_mav_type      = mavlink_msg_heartbeat_get_type(pmsg);
        
     ++num_heartbeats;
   }


#ifndef MAVLINK10
#error wrong version
   void do_mavlink_gps_raw(mavlink_message_t * pmsg)
   {
      uav_lat = mavlink_msg_gps_raw_get_lat(pmsg);
      uav_lon = mavlink_msg_gps_raw_get_lon(pmsg);
      uav_fix_type = mavlink_msg_gps_raw_get_fix_type(pmsg);
   }
   void do_mavlink_gps_status(mavlink_message_t * pmsg)
   {
      uav_satellites_visible = mavlink_msg_gps_status_get_satellites_visible(pmsg);
   }
#else

  void do_mavlink_gps_raw_int(mavlink_message_t * pmsg)
   {

      uav_lat =  (float)mavlink_msg_gps_raw_int_get_lat(pmsg)/10000000 ;
      uav_lon =  (float)mavlink_msg_gps_raw_int_get_lon(pmsg)/10000000;
#ifndef BARO_ALT
      uav_alt = (int)round(mavlink_msg_gps_raw_int_get_alt(pmsg)/10000);
#endif
      uav_fix_type = mavlink_msg_gps_raw_int_get_fix_type(pmsg);
      uav_satellites_visible = mavlink_msg_gps_raw_int_get_satellites_visible(pmsg);
   }
#endif

   void do_mavlink_vfr_hud(mavlink_message_t * pmsg)
   {
      // mavlink_msg_vfr_hud_get_groundspeed retirns val in m/s
      uav_groundspeed = (int)round(mavlink_msg_vfr_hud_get_groundspeed(pmsg));

      // baroalt
#ifdef BARO_ALT
      uav_alt = (int)round(mavlink_msg_vfr_hud_get_alt(pmsg));
#endif
   }

}// ~namespace


#endif
