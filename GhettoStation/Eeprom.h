#include <EEPROM.h>
#include <Arduino.h>  // for type definitions

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
  float overlap_factor;
  int baudrate;
} configuration;


//FLASH arrays stored in EEPROM

//FLASH_STRING_ARRAY(boot_string, PSTR("  [GHETTOSTATION]   "), PSTR("                    "), PSTR(" __________________ "));
//FLASH_STRING_ARRAY(menu_string, PSTR("START"), PSTR("SET HOME"), PSTR("CONFIGURE"), PSTR("SERVOS"), PSTR("PAN"), PSTR("PAN MINPWM"), PSTR("PAN MAXPWM"), PSTR("PAN MINANGLE"), PSTR("PAN MAXANGLE"), PSTR("TILT"), PSTR("TILT MINPWM"), PSTR("TILT MAXPWM"), PSTR("TILT MINANGLE"), PSTR("TILT MAXANGLE"), PSTR("TEST SERVOS"), PSTR("BAUDRATE"), PSTR("    [PAN SERVO]     "), PSTR("    [TILT SERVO]    "), PSTR("min endpoint: <"), PSTR("min angle: <"), PSTR("max endpoint: <"), PSTR("max angle: <"), PSTR(" Long press to quit "), PSTR("  Waiting for Data  "), PSTR("   No GPS 3D FIX    "), PSTR("     3D FIX OK      "), PSTR("    Please Wait.    "), PSTR("  Save Home pos now?") , PSTR(" Move UAV 20m ahead "), PSTR(" & press enter      "), PSTR("    HOME IS SET     "), PSTR("Enter:Start Tracking"), PSTR("<< Menu     Reset >>") );
FLASH_STRING(string_load1, "  [GHETTOSTATION]   ");
FLASH_STRING(string_load2, "                    ");
FLASH_STRING(string_load3, " __________________ ");
FLASH_STRING(string_load4, "      Welcome       ");
//LCD SET HOME
FLASH_STRING(string_shome1, "  Waiting for Data  ");
FLASH_STRING(string_shome2, "   No GPS 3D FIX    ");
FLASH_STRING(string_shome3, "     3D FIX OK      ");
FLASH_STRING(string_shome4, "    Please Wait.    ");
FLASH_STRING(string_shome5, "(long press to quit)");
FLASH_STRING(string_shome6, " Save Home pos now? ");
//FLASH_STRING(string_shome7, "                    ")
FLASH_STRING(string_shome8, " Move UAV 20m ahead ");
FLASH_STRING(string_shome9, " & press enter      ");
FLASH_STRING(string_shome10, "    HOME IS SET     ");
FLASH_STRING(string_shome11, "Enter:Start Tracking");
FLASH_STRING(string_shome12, "<< Menu     Reset >>");






//PROGMEM menu variables
//Menu
//prog_char string_1[] PROGMEM = "START";
//prog_char string_2[] PROGMEM = "SET HOME";
//prog_char string_3[] PROGMEM = "CONFIGURE";
//prog_char string_4[] PROGMEM = "SERVOS";
//prog_char string_5[] PROGMEM = "PAN";
//prog_char string_6[] PROGMEM = "PAN MINPWM";
//prog_char string_7[] PROGMEM = "PAN MAXPWM";
//prog_char string_8[] PROGMEM = "PAN MINANGLE";
//prog_char string_9[] PROGMEM = "PAN MAXANGLE";
//prog_char string_10[] PROGMEM = "TILT";
//prog_char string_11[] PROGMEM = "TILT MINPWM";
//prog_char string_12[] PROGMEM = "TILT MAXPWM";
//prog_char string_13[] PROGMEM = "TILT MINANGLE";
//prog_char string_14[] PROGMEM = "TILT MAXANGLE";
//prog_char string_15[] PROGMEM = "TEST SERVOS";
//prog_char string_16[] PROGMEM = "BAUDRATE";

    


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


//PROGMEM const char *string_table[] = 	  
//{   
//  string_0,
//  string_1,
//  string_2,
//  string_3
//  string_4,
//  string_5,
//  string_6,
//  string_7,
//  string_8,
//  string_9,
//  string_10,
//  string_11,
//  string_12,
//  string_13,
//  string_14,
//  string_15,
//  string_16
//};
//
//char* printString(int i) {
//  char buffer[21];
//  strcpy_P( buffer, (char*)pgm_read_word(&(string_table[i])));
//  return buffer;
//}
