void init_serial() {
	//disable previous serial in case of baudrate change
	Serial.end();
	//enable it
	Serial.begin(TELEMETRY_BAUD);
}

//Preparing adding other protocol
void get_telemetry() {
      if (telemetryMetro.check() == 1) {
        
#if defined(PROTOCOL_UAVTALK)
         protocol = "UAVT";
         uavtalk_read(); // uavtalk serial reading
#endif
#if defined(PROTOCOL_MSP)
         protocol = "MSP";
         msp_read(); // uavtalk serial reading
#endif

      }
}
