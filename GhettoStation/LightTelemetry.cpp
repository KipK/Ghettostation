#include <Arduino.h>
#include "LightTelemetry.h"

  static uint8_t LTMserialBuffer[LIGHTTELEMETRY_GFRAMELENGTH-4];
  static uint8_t LTMreceiverIndex;
  static uint8_t LTMcmd;
  static uint8_t LTMrcvChecksum;
  static uint8_t LTMreadIndex;



uint8_t ltmread8()  {
  return LTMserialBuffer[LTMreadIndex++];
}

uint16_t ltmread16() {
  uint16_t t = ltmread8();
  t |= (uint16_t)ltmread8()<<8;
  return t;
}

uint32_t ltmread32() {
  uint32_t t = ltmread16();
  t |= (uint32_t)ltmread16()<<16;
  return t;
}

void ltm_read() {
  uint8_t c;

  static enum _serial_state {
    IDLE,
    HEADER_START1,
    HEADER_START2,
    HEADER_MSGTYPE,
    HEADER_DATA
  }
  c_state = IDLE;
  
#if defined(MEGA)
  while (Serial1.available()) {
    c = char(Serial1.read());
#else
  while (Uart.available()) {
    c = char(Uart.read());
#endif

    if (c_state == IDLE) {
      c_state = (c=='$') ? HEADER_START1 : IDLE;
        //Serial.println("header $" );
    }
    else if (c_state == HEADER_START1) {
      c_state = (c=='T') ? HEADER_START2 : IDLE;
        //Serial.println("header T" );
    }
    else if (c_state == HEADER_START2) {
      c_state = (c=='G') ? HEADER_MSGTYPE : IDLE;
        //Serial.println("header G" );
	  LTMcmd = c;
	  LTMreceiverIndex=0;
    }
    else if (c_state == HEADER_MSGTYPE) {
	  if(LTMreceiverIndex == 0) {
	  LTMrcvChecksum = c;
	  } 
	  else {
	  LTMrcvChecksum ^= c;
	  }
      if(LTMreceiverIndex == LIGHTTELEMETRY_GFRAMELENGTH-4) { // received checksum byte
        if(LTMrcvChecksum == 0) {
	    telemetry_ok = true;
            lastpacketreceived = millis();
	        protocol = "LTM"; 
            ltm_check();
        }
        else {
        c_state = IDLE;
        
        }
      }
      else LTMserialBuffer[LTMreceiverIndex++]=c;
    }
  }
}

// --------------------------------------------------------------------------------------
// Here are decoded received commands from MultiWii
void ltm_check() {
  LTMreadIndex = 0;
                    
  if (LTMcmd==LIGHTTELEMETRY_GFRAME)
  {
    
    uav_lat = ltmread32() / 10000000.0;
    uav_lon = ltmread32() / 10000000.0;
    uav_groundspeed = ltmread8()*3600/1000;
    uav_alt = (int32_t)ltmread32() / 10;
    uint8_t ltm_satsfix = ltmread8();
    uav_satellites_visible         = (int)((ltm_satsfix >> 2) & 0xFF);
    uav_fix_type                   = (int)(ltm_satsfix & 0b00000011);
    memset(LTMserialBuffer, 0, LIGHTTELEMETRY_GFRAMELENGTH-4);
    //Serial.println("lat : lon : speed: alt : sats : fix" );
    //Serial.print(uav_lat,10);Serial.print("  ");Serial.print(uav_lon,10);Serial.print("  ");Serial.print(uav_groundspeed);Serial.print("  ");Serial.print(uav_alt);Serial.print("  ");Serial.print(uav_satellites_visible);Serial.print("  ");Serial.println(uav_fix_type);
  }
}


// End of decoded received commands from MultiWii
// --------------------------------------------------------------------------------------



