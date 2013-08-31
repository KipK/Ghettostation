
#if defined(PROTOCOL_MSP)
#include "MSP.h"

#define MWCSERIALBUFFERSIZE 256
static uint8_t serialBuffer[MWCSERIALBUFFERSIZE]; // this hold the imcoming string from serial O string
static uint8_t receiverIndex;
static uint8_t dataSize;
static uint8_t cmdMSP;
static uint8_t rcvChecksum;
static uint8_t readIndex;
static int msp_baroalt;
static int msp_gpsalt;

uint8_t read8()  {
  return serialBuffer[readIndex++];
}

uint16_t read16() {
  uint16_t t = read8();
  t |= (uint16_t)read8()<<8;
  return t;
}

uint32_t read32() {
  uint32_t t = read16();
  t |= (uint32_t)read16()<<16;
  return t;
}

void msp_read() {
  uint8_t c;

  static enum _serial_state {
    IDLE,
    HEADER_START,
    HEADER_M,
    HEADER_ARROW,
    HEADER_SIZE,
    HEADER_CMD,
  }
  c_state = IDLE;
  
#ifndef TEENSYPLUS2

  while(Serial.available()) {
    c = Serial.read();
#else

while (Uart.available()) {
    c = Uart.read();
  
#endif

    if (c_state == IDLE) {
      c_state = (c=='$') ? HEADER_START : IDLE;
    }
    else if (c_state == HEADER_START) {
      c_state = (c=='M') ? HEADER_M : IDLE;
    }
    else if (c_state == HEADER_M) {
      c_state = (c=='>') ? HEADER_ARROW : IDLE;
    }
    else if (c_state == HEADER_ARROW) {
      if (c > MWCSERIALBUFFERSIZE)  {  // now we are expecting the payload size
        c_state = IDLE;
      }
      else {
        dataSize = c;
        c_state = HEADER_SIZE;
        rcvChecksum = c;
      }
    }
    else if (c_state == HEADER_SIZE) {
      c_state = HEADER_CMD;
      cmdMSP = c;
      rcvChecksum ^= c;
      receiverIndex=0;
    }
    else if (c_state == HEADER_CMD) {
      rcvChecksum ^= c;
      if(receiverIndex == dataSize) { // received checksum byte
        if(rcvChecksum == 0) {
            msp_check();
        }
        c_state = IDLE;
      }
      else serialBuffer[receiverIndex++]=c;
    }
  }
}

// --------------------------------------------------------------------------------------
// Here are decoded received commands from MultiWii
void msp_check() {
  readIndex = 0;
                    
  if (cmdMSP==MSP_IDENT)
  {
    // possible use later
   // MwVersion= read8();                             // MultiWii Firmware version
   // modeMSPRequests &=~ REQ_MSP_IDENT;
  }

  if (cmdMSP==MSP_STATUS)
  {
  //possible use later
  }
  if (cmdMSP==MSP_RAW_GPS)
  {
    uav_fix_type=read8();
    uav_satellites_visible=read8();
    uav_lat = read32() / 10000000.0;
    uav_lon = read32() / 10000000.0;

    msp_gpsalt = read16();

    uav_groundspeed = read16();
  }

  if (cmdMSP==MSP_COMP_GPS)
  {
//    GPS_distanceToHome=read16();
//    GPS_directionToHome=read16();
  }

  if (cmdMSP==MSP_ATTITUDE)
  {
//  possible use later
//    for(uint8_t i=0;i<2;i++)
//      MwAngle[i] = read16();
//    MwHeading = read16();
//    read16();
  }

  if (cmdMSP==MSP_ALTITUDE)
  {

    msp_baroalt = read32();

    //uav_vario = read16();
  }

  if (cmdMSP==MSP_ANALOG)
  {
// for later
//    MwVBat=read8();
//    pMeterSum=read16();
//    MwRssi = read16();
  }
#ifdef BARO_ALT
uav_alt = msp_baroalt / 10;
#else
uav_alt = msp_gpsalt * 10;
#endif
}


// End of decoded received commands from MultiWii
// --------------------------------------------------------------------------------------


#endif
