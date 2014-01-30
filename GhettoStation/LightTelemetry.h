void ltm_read();
void ltm_check();

#define LIGHTTELEMETRY_START1 0x24 //$
#define LIGHTTELEMETRY_START2 0x54 //T
#define LIGHTTELEMETRY_GFRAME 0x47 //G for GPS + Baro altitude data
#define LIGHTTELEMETRY_GFRAMELENGTH 18
	
void telemetry_off(void);
