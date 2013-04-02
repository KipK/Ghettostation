#if defined(PROTOCOL_MSP)
#include "MSP.h"


uint32_t read32() {
  uint32_t t = read16();
  t+= (uint32_t)read16()<<16;
  return t;
}
uint16_t read16() {
  uint16_t t = read8();
  t+= (uint16_t)read8()<<8;
  return t;
}
uint8_t read8()  {
  return serialBuffer[p++]&0xff;
}

// --------------------------------------------------------------------------------------
// Here are decoded received commands from MultiWii
void serialMSPCheck()

{
  

  if (cmdMSP==MSP_IDENT)
  {
    MwVersion= read8();                             // MultiWii Firmware version

  }


  if (cmdMSP==MSP_RAW_GPS)
  {
	telemetry_ok = true;
    uav_fix_type=read8();
		if (uav_fix_type==1) uav_fix_type=3;
    uav_satellites_visible=read8();
    uav_lat = ((float)read32()) / 10000000;
    uav_lon = ((float)read32()) / 10000000;

    uav_alt = read16();
    uav_groundspeed = read16();
  }



  serialMSPStringOK=0;

}
// End of decoded received commands from MultiWii
// --------------------------------------------------------------------------------------

void msp_read
{
  static uint8_t dataSize;
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

  if(Serial.available())
  {
    c = Serial.read();

    if (c_state == IDLE)
    {
      c_state = (c=='$') ? HEADER_START : IDLE;
    }
    else if (c_state == HEADER_START)
    {
      c_state = (c=='M') ? HEADER_M : IDLE;
    }
    else if (c_state == HEADER_M)
    {
      c_state = (c=='>') ? HEADER_ARROW : IDLE;
    }
    else if (c_state == HEADER_ARROW)
    {
      if (c > SERIALBUFFERSIZE)
      {  // now we are expecting the payload size
        c_state = IDLE;
      }
      else
      {
        dataSize = c;
        c_state = HEADER_SIZE;
      }
    }
    else if (c_state == HEADER_SIZE)
    {
      c_state = HEADER_CMD;
      cmdMSP = c;
    }
    else if (c_state == HEADER_CMD)
    {
      serialBuffer[receiverIndex++]=c;
      if(receiverIndex>=dataSize)
      {
        receiverIndex=0;
        serialMSPStringOK=1;
        c_state = IDLE;
      }
      if(serialMSPStringOK) serialMSPCheck();
    }
  }
}


void blankserialRequest(char requestMSP)
{
  Serial.write('$');
  Serial.write('M');
  Serial.write('<');
  Serial.write((byte)0x00);
  Serial.write(requestMSP);
  Serial.write(requestMSP);
}

#endif
