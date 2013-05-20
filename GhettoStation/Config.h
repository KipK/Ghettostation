//CONFIGURATION                        
//#define DEBUG

//########### SOFT MODEM TELEMETRY########################################################################################
//Use Softmodem ( audio telemetry )
//#define SOFT_MODEM


//########### TELEMETRY PROTOCOL ####################################################################################
//Choose one of the protocol below and comment others:
//UAVTALK OPENPILOT
#define PROTOCOL_UAVTALK      
#define UAVTALK_MODE_PASSIVE  // define if you already have GCS or OSD running. Arduino won't send anything on the Tx line. If undefined, it will send UAVTalk queries.

//MSP MULTIWII
//#define PROTOCOL_MSP        // MSP from Multiwii , only passive for now ( ie doesn't send request to Multiwii so you need a multiwii OSD or ground station client running at the same time )

//###################################################################################################################
#define TELEMETRY_BAUD  9600		//Telemetry baudrate used

//Minimum distance in meters where it will stop moving servos. 
#define DONTTRACKUNDER  6

#define PAN_SERVOPIN 6		//Pin used on arduino for pan servo
#define TILT_SERVOPIN 9		//Pin used on arduino for tilt servo
//#define PAN_SERVOREVERSED      // Depending of your hardware configuration, servo pwm need to be reversed. 
//#define TILT_SERVOREVERSED   // same for tilt


#define LEFT_BUTTON_PIN 4
#define RIGHT_BUTTON_PIN 3
#define ENTER_BUTTON_PIN 5
#define LED_PIN 13    // using pin13 already have a resistor for led on arduino mini & others
				

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



#define BEARING_METHOD  2      //bearing reference to north method
                               // 1=calculation from home pos & uav pos heading 20m away straight to neutral pan servo pos.	
                               // 2=manually enter the heading reference from a compass							
//END OF CONFIG
