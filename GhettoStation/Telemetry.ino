void init_serial() {
	//disable previous serial in case of baudrate change
	Serial.end();
	//enable it
	Serial.begin(57600);
}

//Preparing adding other protocol
void get_telemetry() {
      if (telemetryMetro.check() == 1) {
        
#if defined(PROTOCOL_UAVTALK)
         uavtalk_read(); // uavtalk serial reading
#endif
#if defined(PROTOCOL_MSP)
         msp_read(); // uavtalk serial reading
#endif

      }
}
