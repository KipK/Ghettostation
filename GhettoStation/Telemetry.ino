void init_serial() {
	//disable previous serial in case of baudrate change
#if !defined(TEENSYPLUS2) 
	Serial.end();
        delay(10);
	Serial.begin(TELEMETRY_BAUD);

#endif
#if defined(TEENSYPLUS2) 
      Uart.end();
      delay(10);
      Uart.begin(TELEMETRY_BAUD);
 
#endif

#ifdef DEBUG
    Serial.println("Serial initialised"); 
#endif

}

//Preparing adding other protocol
void get_telemetry() {
      //if (telemetryMetro.check() == 1) {
        
#if defined(PROTOCOL_UAVTALK)
 #ifdef TEENSYPLUS2
   if (configuration.telemetry==0) {
 #endif
      if (uavtalk_read()) {
         protocol = "UAVT";
         //uavtalk_read(); // uavtalk serial reading
      }
  #ifdef TEENSYPLUS2
   }
  #endif
#endif
#if defined(PROTOCOL_MSP)
 #ifdef TEENSYPLUS2
    if (configuration.telemetry==1) {
 #endif
      msp_read(); //msp_read(); // uavtalk serial reading
 #ifdef TEENSYPLUS2
    }
 #endif
#endif

#if defined(SOFT_MODEM)
 #ifdef TEENSYPLUS2
   if (configuration.telemetry==2) {
 #endif
      modem_read();
      
 #ifdef TEENSYPLUS2
   }
   
 #endif
#endif

#if defined(PROTOCOL_MAVLINK)
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
