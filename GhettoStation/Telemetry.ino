void init_serial() {
	//disable previous serial in case of baudrate change
#if !defined(TEENSYPLUS2) && !defined(SOFT_MODEM)
	Serial.end();
        delay(10);
	Serial.begin(TELEMETRY_BAUD);

#endif
#if defined(TEENSYPLUS2)  && !defined(SOFT_MODEM)
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
      if (uavtalk_read()) {
         protocol = "UAVT";
         //uavtalk_read(); // uavtalk serial reading
      }
#endif
#if defined(PROTOCOL_MSP)
      msp_read(); //msp_read(); // uavtalk serial reading
#endif

#if defined(SOFT_MODEM)
      modem_read();
#endif

     // }
}
