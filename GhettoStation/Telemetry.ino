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
      //if (telemetryMetro.check() == 1) {
        
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
      msp_read(); //msp_read(); // uavtalk serial reading
 #ifdef TEENSYPLUS2
    }
 #endif
#endif

#if defined(LIGHTTELEMETRY) // Taulabs 
 #ifdef TEENSYPLUS2
   if (configuration.telemetry==2) {
 #endif
      if (lighttelemetry_read()) {
        protocol = "LTM"; 
      }
      
 #ifdef TEENSYPLUS2
   }
 #endif
#endif

#if defined(PROTOCOL_MAVLINK) // Ardupilot / PixHawk / Taulabs / Other
 #ifdef TEENSYPLUS2
    if (configuration.telemetry==3) {
 #endif
      mavlink_read(); //msp_read(); // uavtalk serial reading
 #ifdef TEENSYPLUS2
    }
 #endif
#endif
     // }
}



