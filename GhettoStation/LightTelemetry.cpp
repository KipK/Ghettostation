/* #################################################################################################################
 * LightTelemetry protocol (LTM)
 *
 * Ghettostation one way telemetry protocol for really low bitrates (1200/2400 bauds). 
 *			   
 * Protocol details: 3 different frames, little endian.
 *   G Frame (GPS position) (2hz @ 1200 bauds , 5hz >= 2400 bauds): 18BYTES
 *    0x24 0x54 0x47 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF  0xFF   0xC0   
 *     $     T    G  --------LAT-------- -------LON---------  SPD --------ALT-------- SAT/FIX  CRC
 *   A Frame (Attitude) (5hz @ 1200bauds , 10hz >= 2400bauds): 10BYTES
 *     0x24 0x54 0x41 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF 0xC0   
 *      $     T   A   --PITCH-- --ROLL--- -HEADING-  CRC
 *   S Frame (Sensors) (2hz @ 1200bauds, 5hz >= 2400bauds): 11BYTES
 *     0x24 0x54 0x53 0xFF 0xFF  0xFF 0xFF    0xFF    0xFF      0xFF       0xC0     
 *      $     T   S   VBAT(mv)  Current(ma)   RSSI  AIRSPEED  ARM/FS/FMOD   CRC
 * ################################################################################################################# */

#if defined(PROTOCOL_LIGHTTELEMETRY)
#include "LightTelemetry.h"
  static uint8_t LTMserialBuffer[LIGHTTELEMETRY_GFRAMELENGTH-4];
  static uint8_t LTMreceiverIndex;
  static uint8_t LTMcmd;
  static uint8_t LTMrcvChecksum;
  static uint8_t LTMreadIndex;
  static uint8_t LTMframelength;



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
  
  while (SerialPort1.available()) {
    c = char(SerialPort1.read());

    if (c_state == IDLE) {
      c_state = (c=='$') ? HEADER_START1 : IDLE;
        //Serial.println("header $" );
    }
    else if (c_state == HEADER_START1) {
      c_state = (c=='T') ? HEADER_START2 : IDLE;
        //Serial.println("header T" );
    }
    else if (c_state == HEADER_START2) {
      switch (c) {
         case 'G':
           LTMframelength = LIGHTTELEMETRY_GFRAMELENGTH;
           c_state = HEADER_MSGTYPE;
           break;
         case 'A':
           LTMframelength = LIGHTTELEMETRY_AFRAMELENGTH;
           c_state = HEADER_MSGTYPE;
           break;
         case 'S':
           LTMframelength = LIGHTTELEMETRY_SFRAMELENGTH;
           c_state = HEADER_MSGTYPE;
           break;
         default:
           c_state = IDLE;
      }
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
      if(LTMreceiverIndex == LTMframelength-4) {   // received checksum byte
        if(LTMrcvChecksum == 0) {
	    telemetry_ok = true;
            lastpacketreceived = millis();
	    protocol = "LTM"; 
            ltm_check();
            c_state = IDLE;
        }
        else {                                                   // wrong checksum, drop packet
        c_state = IDLE; 
        
        }
      }
      else LTMserialBuffer[LTMreceiverIndex++]=c;
    }
  }
}

// --------------------------------------------------------------------------------------
// Decoded received commands 
void ltm_check() {
  LTMreadIndex = 0;
                    
  if (LTMcmd==LIGHTTELEMETRY_GFRAME)
  {
    
    uav_lat = ltmread32() / 10000000.0;
    uav_lon = ltmread32() / 10000000.0;
    uav_groundspeed = ltmread8()*3600/1000;
    uav_alt = (int32_t)ltmread32() / 10;
    uint8_t ltm_satsfix = ltmread8();
    uav_satellites_visible         = (ltm_satsfix >> 2) & 0xFF;
    uav_fix_type                   = ltm_satsfix & 0b00000011;
    //memset(LTMserialBuffer, 0, LIGHTTELEMETRY_GFRAMELENGTH-4); //still wondering what's it doing here

  }
  
  if (LTMcmd==LIGHTTELEMETRY_AFRAME)
  {
    uav_pitch = ltmread16();
    uav_roll = ltmread16();
    uav_heading = ltmread16();
  }
  if (LTMcmd==LIGHTTELEMETRY_SFRAME)
  {
    uav_bat = ltmread16();
    uav_amp = ltmread16();
    uav_rssi = ltmread8();
    uav_airspeed = ltmread8();
    uint8_t ltm_armfsmode = ltmread8();
    uav_flightmode = (ltm_armfsmode >> 2) & 0xFF; 
    uav_failsafe = (ltm_armfsmode >> 1) & 0b00000001;
    uav_arm = ltm_armfsmode & 0b00000001;
  }
}


 


/* ########################################################################################
/*                            LightTelemetry OSD output 
/* ########################################################################################*/
#ifdef OSD_OUTPUT

#define LTM_GFRAME_SIZE 18
#define LTM_AFRAME_SIZE 10
#define LTM_SFRAME_SIZE 11


static void send_LTM_Packet(uint8_t *LTPacket, uint8_t LTPacket_size)
{
    //calculate Checksum
    uint8_t LTCrc = 0x00;
    int i;
    for (i = 3; i < LTPacket_size-1; i++) {
        LTCrc ^= LTPacket[i];
    }
    LTPacket[LTPacket_size-1]=LTCrc;
    for (i = 0; i<LTPacket_size; i++) {
        SerialPort2.write(LTPacket[i]);
    }
}


// GPS frame
void send_LTM_Gframe()
{

    uint8_t LTBuff[LTM_GFRAME_SIZE];
    //protocol: START(2 bytes)FRAMEID(1byte)LAT(cm,4 bytes)LON(cm,4bytes)SPEED(m/s,1bytes)ALT(cm,4bytes)SATS(6bits)FIX(2bits)CRC(xor,1byte)
    //START
    LTBuff[0]=0x24; //$
    LTBuff[1]=0x54; //T
    //FRAMEID
    LTBuff[2]=0x47; // G ( gps frame at 5hz )
    //PAYLOAD
    int32_t uav_intlat = (int32_t) uav_lat * 10000000;
    int32_t uav_intlon = (int32_t) uav_lon * 10000000;
    LTBuff[3]=(uav_intlat >> 8*0) & 0xFF;
    LTBuff[4]=(uav_intlat >> 8*1) & 0xFF;
    LTBuff[5]=(uav_intlat >> 8*2) & 0xFF;
    LTBuff[6]=(uav_intlat >> 8*3) & 0xFF;
    LTBuff[7]=(uav_intlon >> 8*0) & 0xFF;
    LTBuff[8]=(uav_intlon >> 8*1) & 0xFF;
    LTBuff[9]=(uav_intlon >> 8*2) & 0xFF;
    LTBuff[10]=(uav_intlon >> 8*3) & 0xFF;
    LTBuff[11]=( (uint8_t) round( (uav_groundspeed * 1000) / 3600) >> 8*0) & 0xFF;
    uint32_t uav_alt_cm = (uint32_t) uav_alt * 10;
    LTBuff[12]=(uav_alt_cm >> 8*0) & 0xFF;
    LTBuff[13]=(uav_alt_cm >> 8*1) & 0xFF;
    LTBuff[14]=(uav_alt_cm >> 8*2) & 0xFF;
    LTBuff[15]=(uav_alt_cm >> 8*3) & 0xFF;
    LTBuff[16]= ((uav_satellites_visible << 2 )& 0xFF) | (uav_fix_type & 0b00000011) ; // last 6 bits: sats number, first 2:fix type (0,1,2,3)
    send_LTM_Packet(LTBuff,LTM_GFRAME_SIZE);
}

//Sensors frame
static void send_LTM_Sframe() 
{
    uint8_t LTBuff[LTM_SFRAME_SIZE];
    
    //START
    LTBuff[0]=0x24; //$
    LTBuff[1]=0x54; //T
    //FRAMEID
    LTBuff[2]=0x53; //S 
    //PAYLOAD
    LTBuff[3]=(uav_bat >> 8*0) & 0xFF;                                                    //vbat converted in mv
    LTBuff[4]=(uav_bat >> 8*1) & 0xFF;
    LTBuff[5]=(uav_amp >> 8*0) & 0xFF;                                                    //consumed current in ma.
    LTBuff[6]=(uav_amp >> 8*1) & 0xFF;
    LTBuff[7]=(uav_rssi >> 8*0) & 0xFF;                                                   // rouding RSSI to 1 byte resolution.
    LTBuff[8]=(uav_airspeed >> 8*0) & 0xFF;                                               // no airspeed in multiwii/baseflight
    LTBuff[9]= ((uav_flightmode << 2)& 0xFF ) | ((uav_failsafe << 1)& 0b00000010 ) | (uav_arm & 0b00000001) ; // last 6 bits: flight mode, 2nd bit: failsafe, 1st bit: Arm status.
    // Flight mode(0-19): 0: Manual, 1: Rate, 2: Attitude/Angle, 3: Horizon, 4: Acro, 5: Stabilized1, 6: Stabilized2, 7: Stabilized3,
    // 8: Altitude Hold, 9: Loiter/GPS Hold, 10: Auto/Waypoints, 11: Heading Hold / headFree, 
    // 12: Circle, 13: RTH, 14: FollowMe, 15: LAND, 16:FlybyWireA, 17: FlybywireB, 18: Cruise, 19: Unknown
    send_LTM_Packet(LTBuff,LTM_SFRAME_SIZE);
}

// Attitude frame
static void send_LTM_Aframe() 
{
    uint8_t LTBuff[LTM_AFRAME_SIZE];
    
    //A Frame: $T(2 bytes)A(1byte)PITCH(2 bytes)ROLL(2bytes)HEADING(2bytes)CRC(xor,1byte)
    //START
    LTBuff[0]=0x24; //$
    LTBuff[1]=0x54; //T
    //FRAMEID
    LTBuff[2]=0x41; //A 
    //PAYLOAD
    LTBuff[3]=(uav_pitch >> 8*0) & 0xFF;
    LTBuff[4]=(uav_pitch >> 8*1) & 0xFF;
    LTBuff[5]=(uav_roll >> 8*0) & 0xFF;
    LTBuff[6]=(uav_roll >> 8*1) & 0xFF;
    LTBuff[7]=(uav_heading >> 8*0) & 0xFF;
    LTBuff[8]=(uav_heading >> 8*1) & 0xFF;
    send_LTM_Packet(LTBuff,LTM_AFRAME_SIZE);
}

void ltm_write() {
        send_LTM_Aframe();
        send_LTM_Sframe();
        send_LTM_Gframe();
}
#endif
#endif
