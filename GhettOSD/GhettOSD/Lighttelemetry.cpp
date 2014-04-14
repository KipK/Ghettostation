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

#ifdef PROTOCOL_LIGHTTELEMETRY

#define LIGHTTELEMETRY_START1 0x24 //$
#define LIGHTTELEMETRY_START2 0x54 //T
#define LIGHTTELEMETRY_GFRAME 0x47 //G GPS + Baro altitude data ( Lat, Lon, Speed, Alt, Sats, Sat fix)
#define LIGHTTELEMETRY_AFRAME 0x41 //A Attitude data ( Roll,Pitch, Heading )
#define LIGHTTELEMETRY_SFRAME 0x53 //S Sensors/Status data ( VBat, Consumed current, Rssi, Airspeed, Arm status, Failsafe status, Flight mode )
#define LIGHTTELEMETRY_OFRAME 0x4F  //O OSD additionals data ( home pos, home alt, ddirection to home )
#define LIGHTTELEMETRY_GFRAMELENGTH 18
#define LIGHTTELEMETRY_AFRAMELENGTH 10
#define LIGHTTELEMETRY_SFRAMELENGTH 11
#define LIGHTTELEMETRY_OFRAMELENGTH 18

  static uint8_t LTMserialBuffer[LIGHTTELEMETRY_GFRAMELENGTH-4];
  static uint8_t LTMreceiverIndex;
  static uint8_t LTMcmd;
  static uint8_t LTMrcvChecksum;
  static uint8_t LTMreadIndex;
  static uint8_t LTMframelength;
  static uint8_t LTMpassed= 0;
  static uint8_t crlf_count = 0;
  void uploadFont();

uint8_t ltmread_u8()  {
  return LTMserialBuffer[LTMreadIndex++];
}

uint16_t ltmread_u16() {
  uint16_t t = ltmread_u8();
  t |= (uint16_t)ltmread_u8()<<8;
  return t;
}

uint32_t ltmread_u32() {
  uint32_t t = ltmread_u16();
  t |= (uint32_t)ltmread_u16()<<16;
  return t;
}


// --------------------------------------------------------------------------------------
// Decoded received commands 
void ltm_check() {
  LTMreadIndex = 0;
  LTM_ok = 1;
  lastLTMpacket = millis();
  
                    
  if (LTMcmd==LIGHTTELEMETRY_GFRAME)
  {
    
    osd_lat = (int32_t)ltmread_u32() / 10000000.0;
    osd_lon = (int32_t)ltmread_u32() / 10000000.0;
    osd_groundspeed = (float)(ltmread_u8()); 
    osd_alt = ((int32_t)ltmread_u32()) / 100.0f;      // altitude from cm to m.
    uint8_t ltm_satsfix = ltmread_u8();
    osd_satellites_visible         = (ltm_satsfix >> 2) & 0xFF;
    osd_fix_type                   = ltm_satsfix & 0b00000011;
    //memset(LTMserialBuffer, 0, LIGHTTELEMETRY_GFRAMELENGTH-4); 
    LTMpassed = 1;

  }
  
  if (LTMcmd==LIGHTTELEMETRY_AFRAME)
  {
    osd_pitch = (int16_t)ltmread_u16();
    osd_roll = (int16_t)ltmread_u16();
    osd_heading = (float)(int16_t)ltmread_u16();
    if (osd_heading < 0 ) osd_heading = osd_heading + 360.0f; //convert from -180/180 to 0/360°
    LTMpassed = 1;
  }
  if (LTMcmd==LIGHTTELEMETRY_SFRAME)
  {
    static int frametick = 0; 
    static uint16_t osd_curr_A_prev= 0;
    
    osd_vbat_A = (float) ltmread_u16() / 1000.0f;
    
    mah_used = ltmread_u16(); 
    osd_curr_A   = mah_used *360000 / (millis() - frametick);
    osd_battery_remaining_A = BAT_CAPACITY - mah_used;      // To do: send battery capacity configured from ghettostation menu
    
    osd_rssi = ltmread_u8();
    
    osd_airspeed = ltmread_u8();
    
    uint8_t ltm_armfsmode = ltmread_u8();
    motor_armed = (bool)(ltm_armfsmode & 0b00000001);
    osd_failsafe = (ltm_armfsmode >> 1) & 0b00000001;
    osd_mode = (ltm_armfsmode >> 2) & 0b00111111;     
    
    frametick = millis();
    LTMpassed = 1;
  }
  
    if (LTMcmd==LIGHTTELEMETRY_OFRAME)
  {
    osd_home_lat = (int32_t)ltmread_u32() / 10000000.0;
    osd_home_lon = (int32_t)ltmread_u32() / 10000000.0;
    osd_home_alt = (int32_t)(ltmread_u32()) / 100.0f; // altitude from cm to m.
    osd_enabled  = ltmread_u8();
    osd_got_home = ltmread_u8();
    if (osd_enabled == 0) osd_clear = 1;
    LTMpassed = 1;
  }
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
  
  while (Serial.available()) {
    c = char(Serial.read());
    /* allow CLI to be started by hitting enter 3 times, if no
    packets have been received */
        if (LTM_ok == 0 && millis() < 20000 && millis() > 5000) {
            if (c == '\n' || c == '\r') {
                crlf_count++;
            } else {
                crlf_count = 0;
            }
            if (crlf_count == 3) {
                uploadFont();
            }
        }
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
         case 'O':
           LTMframelength = LIGHTTELEMETRY_OFRAMELENGTH;
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



#endif
