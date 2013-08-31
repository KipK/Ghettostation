#ifdef LIGHTTELEMETRY
#include "LightTelemetry.h"
  
static inline int8_t lighttelemetry_get_int8(lighttelemetry_message_t *msg, int pos) {
  	return msg->Data[pos];
  }
  
  
static inline int16_t lighttelemetry_get_int16(lighttelemetry_message_t *msg, int pos) {
  	int16_t i;
  	memcpy(&i, msg->Data+pos, sizeof(int16_t));
  	return i;
  }
  
  
static inline int32_t lighttelemetry_get_int32(lighttelemetry_message_t *msg, int pos) {
  	int32_t i;
  	memcpy(&i, msg->Data+pos, sizeof(int32_t));
  	return i;
  }


uint8_t lighttelemetry_parse_char(uint8_t c, lighttelemetry_message_t *msg) {
      static uint8_t status = LIGHTTELEMETRY_PARSE_STATE_WAIT_START1;
      static uint8_t crc = 0;
      static uint8_t cnt = 0;
      
      
      switch (status) {
  		case LIGHTTELEMETRY_PARSE_STATE_WAIT_START1:
  			if (c == LIGHTTELEMETRY_START1) {
  				status = LIGHTTELEMETRY_PARSE_STATE_GOT_START1;
  				msg->Start1 = c;
  			}
  		break;
                case LIGHTTELEMETRY_PARSE_STATE_GOT_START1:
  			if (c == LIGHTTELEMETRY_START2) {
  				status = LIGHTTELEMETRY_PARSE_STATE_GOT_START2;
  				msg->Start2 = c;
  			}
  		break;
  		case LIGHTTELEMETRY_PARSE_STATE_GOT_START2:

  			if (c == LIGHTTELEMETRY_GFRAME) {
  				status = LIGHTTELEMETRY_PARSE_STATE_GOT_MSG_TYPE;
  				msg->MsgType = c;
  				cnt = 0;
  			}
  			else {
  				status = LIGHTTELEMETRY_PARSE_STATE_WAIT_START1;
  			}
  		break;
  		case LIGHTTELEMETRY_PARSE_STATE_GOT_MSG_TYPE:
                        cnt++;
                        msg->Data[cnt - 1] = c; // feeds data buffer.
                        if (cnt >= LIGHTTELEMETRY_GFRAMELENGTH) {
  				status = LIGHTTELEMETRY_PARSE_STATE_GOT_DATA; 
  				cnt = 0;
  			}
  		break;
  

  		case LIGHTTELEMETRY_PARSE_STATE_GOT_DATA:
  			msg->Crc = c;                   
  			status = LIGHTTELEMETRY_PARSE_STATE_GOT_CRC;
  		break;
  	}
  	
  	if (status == LIGHTTELEMETRY_PARSE_STATE_GOT_CRC) {
  		status = LIGHTTELEMETRY_PARSE_STATE_WAIT_START1;
                //calculate crc
                for (int i=3; i < 18; i++) {
                  crc ^= msg->Data[i];  
                }
                if (crc == msg->Crc) {
                  //crc ok we can parse data
                  status = LIGHTTELEMETRY_PARSE_STATE_CRC_OK;
                  return 1;
                }
                else {
                  status = LIGHTTELEMETRY_PARSE_STATE_WAIT_START1;
                  return 0;
                }
        }
  
}

int lighttelemetry_read() {
      static lighttelemetry_message_t msg;
#ifndef TEENSYPLUS2
      while ( Serial.available() > 0) {
      uint8_t c = Serial.read();
#else
      while ( Uart.available() > 0) {
      uint8_t c = Uart.read();
#endif
      // parse data to msg
      if (lighttelemetry_parse_char(c, &msg)) {
          telemetry_ok = true;
          
          uav_lat			 = lighttelemetry_get_int32(&msg, LIGHTTELEMETRY_LAT) / 10000000.0;
          uav_lon			 = lighttelemetry_get_int32(&msg, LIGHTTELEMETRY_LON) / 10000000.0;
          uav_groundspeed		 = (int)round((float)lighttelemetry_get_int16(&msg, LIGHTTELEMETRY_GROUNDSPEED) * 3.6); // convert m/s to km/h
          uav_alt                        = (int)(lighttelemetry_get_int32(&msg, LIGHTTELEMETRY_ALTITUDE) / 10); // decimeter     
          uint8_t lighttelemetry_satsfix = lighttelemetry_get_int8(&msg, LIGHTTELEMETRY_SATSFIX);
          uav_satellites_visible         = (lighttelemetry_satsfix >> 2) & 0xFF;
          uav_fix_type                   = lighttelemetry_satsfix & 0b00000011;
   
         return 1;  
      }
      delayMicroseconds(900);  // wait at least 1 byte ( @ 1200 bauds should gives 1 bytes each 833.3us )
   }
}


#endif
