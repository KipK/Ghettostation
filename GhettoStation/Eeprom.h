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
  int baudrate;
  int telemetry;
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
FLASH_STRING(string_shome3, "3D FIX! Alt:");
FLASH_STRING(string_shome4, "    Please Wait.    ");
FLASH_STRING(string_shome5, "(long press to quit)");
FLASH_STRING(string_shome6, " Save Home pos now? ");
FLASH_STRING(string_shome7, " Set Bearing:       ");
FLASH_STRING(string_shome8, " Move UAV 20m ahead ");
FLASH_STRING(string_shome9, " & press enter      ");
FLASH_STRING(string_shome10, "    HOME IS SET     ");
FLASH_STRING(string_shome11, "Enter:Start Tracking");
FLASH_STRING(string_shome12, "<< Menu     Reset >>");
FLASH_STRING(string_telemetry1, "  SELECT PROTOCOL:  ");
FLASH_STRING(string_telemetry2, "      UAVTALK     >>");
FLASH_STRING(string_telemetry3, "<<    MULTIWII    >>");
FLASH_STRING(string_telemetry4, "<< LIGHTTELEMETRY >>");
FLASH_STRING(string_telemetry5, "<<     MAVLINK      ");
FLASH_STRING(string_baudrate, "  SELECT BAUDRATE:  ");
FLASH_STRING(string_baudrate0, "        1200      >>");
FLASH_STRING(string_baudrate1, "<<      2400      >>");
FLASH_STRING(string_baudrate2, "<<      4800      >>");
FLASH_STRING(string_baudrate3, "<<      9600      >>");
FLASH_STRING(string_baudrate4, "<<     19200      >>");
FLASH_STRING(string_baudrate5, "<<     38400      >>");
FLASH_STRING(string_baudrate6, "<<     57600      >>");
FLASH_STRING(string_baudrate7, "<<    115200        ");


void clear_eeprom() {
	// clearing eeprom
	for (int i = 0; i < 1025; i++)
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
	configuration.baudrate = 6;
        configuration.telemetry = 0;
	EEPROM_write(0, configuration);

        
}

