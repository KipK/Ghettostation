//CONFIGURATION                        
#define DEBUG

#define TEENSYPLUS2 // Teensy++2 support. Default is 328P.


#define EXTERNALPULLUP // disable arduino internal pullup for I2C communication




//########### LCD ########################################################################################


//#define I2CADDRESS 0xc6 // LCD03 i2C from Robot Electronics 
#define I2CADDRESS 0x27 // IIC/I2C/TWI Serial 2004 20x4 lcd.
//#define I2CADDRESS 020 // Arduino-IIC-LCD GY-LCD-V1




//########### TELEMETRY PROTOCOL ####################################################################################
//Choose one of the protocol below and comment others:

//### UAVTALK OPENPILOT
//#define PROTOCOL_UAVTALK      
#define UAVTALK_MODE_PASSIVE  // If you already have GCS or OSD running. Arduino won't send anything on the Tx line. If undefined, it will send UAVTalk queries.
#define BARO_ALT // Use Baro for Altitude instead of GPS.

//### MSP MULTIWII
#define PROTOCOL_MSP        // MSP from Multiwii , only passive for now ( ie doesn't send request to Multiwii so you need a multiwii OSD or ground station client running at the same time )

//### Audio MODEM AFK Telemetry (SoftModem)
//#define SOFT_MODEM

//###################################################################################################################
#define TELEMETRY_BAUD  19200		//Telemetry baudrate used

//Minimum distance in meters where it will stop moving servos. 
#define DONTTRACKUNDER  3

#define PAN_SERVOPIN 26		//PWM Pin for pan servo
#define TILT_SERVOPIN 25		//PWM Pin for tilt servo
//#define PAN_SERVOREVERSED      // Depending of your hardware configuration, servo pwm need to be reversed. 
//#define TILT_SERVOREVERSED   // same for tilt


#define LEFT_BUTTON_PIN 7     // select a free digital pin for those 3 buttons
#define RIGHT_BUTTON_PIN 9
#define ENTER_BUTTON_PIN 8
//#define LED_PIN 13    // using pin13 already have a resistor for led on arduino mini & others
				

//Default servos configuration for first startup 
//You will be able to edit them from the configuration menu later
//Put conservatives values to not overload your servo forcing endpoints when configuring them

		
#define PAN_MAXPWM 2420	//max pan servo pwm value
#define PAN_MAXANGLE 90	//max angle clockwise (on the right) defined by PAN_MAXPWM.
						//Considering 0° is  heading toward level to the ground,
						//for 180° pan conf, a 110 value means we can go 20° further before reversing pan & tilt.						
						//A 190° value means for a 360° pan setup we can go 10° further before inversing pan.
						
#define PAN_MINPWM 632		//min pan servo pwm value
#define PAN_MINANGLE 90		//max angle counter-clockwise (on the left).
							//With those pan_maxangle & pan_minangle we can know the pan type ( 360° or 180° pan )
							// & we have the overlap range.

#define TILT_MAXPWM 2227	//max tilt pwm value 
#define TILT_MAXANGLE 155	//max tilt angle considering 0° is facing toward.
							//ie for 180° pan config, 160 means we have a 20° deadband behind on Tilt axis
							//a 200 value for example would indicate we can even tilt down backward 20°.
							//120 for 360° pan config, means tilt can go 30° more further your head 
							//before switching pan side.
							// IF YOU HAVE A 90° Tilt setup , DON'T FORGET TO CHANGE THIS VALUE TO 90 
							//HERE TO PROTECT YOUR SERVO
							
							
#define TILT_MINPWM 774		//min tilt pwm value
#define TILT_MINANGLE 3		//Means minimum tilt position is parallel to ground. Considering 0 is facing toward, a -10 value would means we can tilt 10° down.
//#define OVERLAP_FACTOR 1.1	// Unused yet
				//the factor that determines how much the heading/tilt must go over the servo limits before the opposite axis  switch to the other side. I.e.
				// 1.1 as default setting gives you an 18 degree "gray on 180
				// area" where the pan/tilt doesn't switch sides even
				//if the servo goes to its limit.
				// If we have 360/90++ config, this means if you go over your head behind you, but less than (tilt_topcourseangle + tilt_overlap_factor) , it won't switch the pan axis to other side.
				//If we have 180/180 config, this is used the opposite way, if your heading goes more than 90° on your side, but less than 108°, it won't switch the Tilt axis ( & inverse pan ) to track behind.

// BEARING REFERENCE TO NORTH METHOD
//#define BEARING_METHOD_1  // 1=calculation from home pos & uav pos heading 20m away straight to neutral pan servo pos.	
//#define BEARING_METHOD_2   // 2=manually enter the heading reference from a compass	
//#define BEARING_METHOD_3   // 3=don't bother, just point your tracker to the north.		
#define BEARING_METHOD_4  // 4=Use HMC5883 compass reading                          
          
//END OF CONFIG
