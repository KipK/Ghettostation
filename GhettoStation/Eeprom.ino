void clear_eeprom() {
	// clearing eeprom
	for (int i = 0; i < 1024; i++)
		EEPROM.write(i, 0);
		// eeprom is clear  we can write default config
	configuration.config_crc = CONFIG_VERSION;  // config version check
	configuration.pan_minpwm = PAN_MINPWM;
	configuration.pan_minangle = PAN_MINANGLE;
	configuration.pan_maxpwm = PAN_MAXPWM;
	configuration.pan_maxangle = PAN_MAXANGLE;
	configuration.tilt_minpwm = TILT_MINPWM;
	configuration.tilt_minangle = TILT_MINANGLE;
	configuration.tilt_maxpwm = TILT_MAXPWM;
    configuration.tilt_maxangle = TILT_MAXANGLE;
	configuration.overlap_factor = OVERLAP_FACTOR;
	configuration.baudrate = TELEMETRY_BAUD;
	EEPROM_write(0, configuration);

        
}