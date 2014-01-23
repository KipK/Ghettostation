#include "Config.h"
void telemetry_off(void);
extern boolean telemetry_ok;
extern int lastpacketreceived;
extern String protocol;
extern float uav_lat;
extern float uav_lon;
extern int uav_satellites_visible;
extern int uav_fix_type;
extern long uav_alt;
extern int uav_groundspeed;

#if defined(TEENSYPLUS2)
extern HardwareSerial Uart;
#endif
