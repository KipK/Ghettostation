void init_serial() {
//#if !defined(TEENSYPLUS2) 
        
      Serial.begin(TELEMETRY_BAUD);

//#endif
#if defined(TEENSYPLUS2) 

      Uart.begin(TELEMETRY_BAUD);
 
#endif

#ifdef DEBUG
    Serial.println("Serial initialised"); 
#endif

}

//Preparing adding other protocol
void get_telemetry() {
 if (telemetryMetro.check() == 1) {
   if (millis() - lastpacketreceived > 1000) {
      telemetry_ok = false;
      
     
   }
        
#if defined(PROTOCOL_UAVTALK) // OpenPilot / Taulabs
 #ifdef TEENSYPLUS2
   if (configuration.telemetry==0) {
 #endif
      if (uavtalk_read()) {
         protocol = "UAVT";
      }
  #ifdef TEENSYPLUS2
   }
 #endif
#endif

#if defined(PROTOCOL_MSP) // Multiwii
 #ifdef TEENSYPLUS2
    if (configuration.telemetry==1) {
 #endif
      msp_read(); 
 #ifdef TEENSYPLUS2
    }
 #endif
#endif

#if defined(PROTOCOL_LIGHTTELEMETRY) // Open protocol. 
 #ifdef TEENSYPLUS2
   if (configuration.telemetry==2) {
 #endif
      ltm_read();
 #ifdef TEENSYPLUS2
   }
 #endif
#endif

#if defined(PROTOCOL_MAVLINK) // Ardupilot / PixHawk / Taulabs / Other
 #ifdef TEENSYPLUS2
    if (configuration.telemetry==3) {
 #endif
      mavlink_read(); 
 #ifdef TEENSYPLUS2
    }
 #endif
#endif
  }
}

void telemetry_off() {
  //reset uav data
  uav_lat = 0;
  uav_lon = 0;                    
  uav_satellites_visible = 0;     
  uav_fix_type = 0;               
  uav_alt = 0;                    
  uav_groundspeed = 0;
  protocol = "";
  telemetry_ok = false;
  }


