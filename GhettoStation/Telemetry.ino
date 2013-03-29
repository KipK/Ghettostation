void init_serial() {
	//disable previous serial in case of baudrate change
	//Serial.end();
	//enable it
	//Serial.begin(configuration.baudrate);
	Serial.begin(TELEMETRY_BAUD);
}

//Preparing adding other protocol
void get_telemetry() {
      if (telemetryMetro.check() == 1) {
        
#if defined(PROTOCOL_UAVTALK)
         uavtalk_read(); // uavtalk serial reading
         if (uav_fix == 3) gps_fix = true;
#endif

      }
}
