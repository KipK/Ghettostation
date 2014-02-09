#include <EEPROM.h>
#include <Arduino.h>  // for type definitions

int config_bank[]= {1, 51, 101,151}; // 50 bytes reserved per bank. 
uint8_t current_bank;

template <class T> int EEPROM_write(int ee, const T& value)
{
    const byte* p = (const byte*)(const void*)&value;
    unsigned int i;
    for (i = 0; i < sizeof(value); i++)
          EEPROM.write(ee++, *p++);
    return i;
}

template <class T> int EEPROM_read(int ee, T& value)
{
    byte* p = (byte*)(void*)&value;
    unsigned int i;
    for (i = 0; i < sizeof(value); i++)
          *p++ = EEPROM.read(ee++);
    return i;
}


//Configuration stored in EEprom
struct config_t
{
  int config_crc;
  int pan_minpwm;
  int pan_minangle;
  int pan_maxpwm;
  int pan_maxangle;
  int tilt_minpwm;
  int tilt_minangle;
  int tilt_maxpwm;
  int tilt_maxangle;
  int baudrate;
  int telemetry;
} configuration;



void clear_eeprom() {
	// clearing eeprom
	for (int i = 0; i < 1025; i++)
		EEPROM.write(i, 0);
		// eeprom is clear  we can write default config
        //writing 4 setting banks.
        for (int j = 0; j < 4; j++) {          
	  configuration.config_crc = CONFIG_VERSION;  // config version check
	  configuration.pan_minpwm = PAN_MINPWM;
	  configuration.pan_minangle = PAN_MINANGLE;
	  configuration.pan_maxpwm = PAN_MAXPWM;
	  configuration.pan_maxangle = PAN_MAXANGLE;
	  configuration.tilt_minpwm = TILT_MINPWM;
	  configuration.tilt_minangle = TILT_MINANGLE;
	  configuration.tilt_maxpwm = TILT_MAXPWM;
          configuration.tilt_maxangle = TILT_MAXANGLE;
	  configuration.baudrate = 6;
          configuration.telemetry = 0;
	  EEPROM_write(config_bank[j], configuration);
        }       
}

