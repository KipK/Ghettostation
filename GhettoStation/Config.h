//CONFIGURATION                        
//#define DEBUG

#define TEENSYPLUS2 // Teensy++2 support. Default is 328P.


//#define EXTERNALPULLUP // disable arduino internal pullup for I2C communication

#define BARO_ALT // Use Baro for Altitude instead of GPS.

#define MAGDEC -1.454  // your local Magnetic Declination in radian. Get it from here: http://magnetic-declination.com/  then convert it in milliradian: http://www.wolframalpha.com/input/?i=%280%C2%B0+5%27%29+in+radians 

//########### LCD ########################################################################################


//#define I2CADDRESS 0xc6 // LCD03 i2C from Robot Electronics 
#define I2CADDRESS 0x27 // IIC/I2C/TWI Serial 2004 20x4 lcd.
//#define I2CADDRESS 020 // Arduino-IIC-LCD GY-LCD-V1




//########### TELEMETRY PROTOCOL ####################################################################################
//for 328p choose one of the protocol below and comment others.. For Teensy++2 you can enable all, config is done at runtime.

//### UAVTALK OPENPILOT
#define PROTOCOL_UAVTALK      
#define UAVTALK_MODE_PASSIVE  // If you already have GCS or OSD running. Arduino won't send anything on the Tx line. If undefined, it will send UAVTalk queries.


//### MSP MULTIWII
#define PROTOCOL_MSP        // MSP from Multiwii , only passive for now ( ie doesn't send request to Multiwii so you need a multiwii OSD or ground station client running at the same time )

//### Audio MODEM AFK Telemetry (SoftModem)
#define SOFT_MODEM // 1200 bauds audio telemetry

//### MAVLINK
#define PROTOCOL_MAVLINK  // Mavlink for Ardupilot / Autoquad / PixHawk / Taulabs ( in mavlink mode ) 


//###################################################################################################################
#define TELEMETRY_BAUD  19200		//Telemetry baudrate used

//Minimum distance in meters where it will stop moving servos. 
#define DONTTRACKUNDER  3

#define PAN_SERVOPIN 26		//PWM Pin for pan servo
#define TILT_SERVOPIN 25		//PWM Pin for tilt servo
//#define PAN_SERVOREVERSED      // Depending of your hardware configuration, servo pwm need to be reversed. 
//#define TILT_SERVOREVERSED   // same for tilt


#define LEFT_BUTTON_PIN 10     // select a free digital pin for those 3 buttons
#define RIGHT_BUTTON_PIN 9
#define ENTER_BUTTON_PIN 8
//#define LED_PIN 13    // using pin13 already have a resistor for led on arduino mini & others
				

//Default servos configuration for first startup 
//You will be able to edit them from the configuration menu later
//Put conservatives values to not overload your servo forcing endpoints when configuring them

		
//#define PAN_MAXPWM 1500	//max pan servo pwm value
#define PAN_MAXPWM 1271  //max pan servo pwm for ServoCity heavy duty pan

//Max angle clockwise (on the right) defined by PAN_MAXPWM.
//#define PAN_MAXANGLE 90	/180° pan can go 90° left & right. 
#define PAN_MAXANGLE 180  // 360° pan can go 180° left & right.
	
//Max angle counter clockwise (on the left) defined by PAN_MINPWM.					
//#define PAN_MINPWM 1200		//min pan servo pwm value
#define PAN_MINPWM 1980
//#define PAN_MINANGLE 90		//180° pan can go 90° left & right
#define PAN_MINANGLE 180 //360° pan can go 180° left & right.

//#define TILT_MAXPWM 800	//max tilt pwm value 
#define TILT_MAXPWM 1200     
#define TILT_MAXANGLE 90	//max tilt angle considering 0° is facing toward.
																	
//#define TILT_MINPWM 500		//min tilt pwm value
#define TILT_MINPWM 1950
#define TILT_MINANGLE 1 	//Means minimum tilt position is parallel to ground. Considering 0 is facing toward, a -10 value would means we can tilt 10° down.

//#define OVERLAP_FACTOR 1.1	// TODO, Unused yet
				//the factor that determines how much the heading/tilt must go over the servo limits before the opposite axis  switch to the other side. I.e.
				// 1.1 as default setting gives you an 18 degree "gray on 180
				// area" where the pan/tilt doesn't switch sides even
				//if the servo goes to its limit.
				// If we have 360/90++ config, this means if you go over your head behind you, but less than (tilt_topcourseangle + tilt_overlap_factor) , it won't switch the pan axis to other side.
				//If we have 180/180 config, this is used the opposite way, if your heading goes more than 90° on your side, but less than 108°, it won't switch the Tilt axis ( & inverse pan ) to track behind.

// BEARING REFERENCE TO NORTH METHOD
//#define BEARING_METHOD_1  // 1=calculation from home pos & uav pos heading 20m away straight to neutral pan servo pos.	
#define BEARING_METHOD_2   // 2=manually enter the heading reference from a compass	
//#define BEARING_METHOD_3   // 3=don't bother, just point your tracker to the north.		
//#define BEARING_METHOD_4  // 4=Use HMC5883 compass reading                          
          
//END OF CONFIG
