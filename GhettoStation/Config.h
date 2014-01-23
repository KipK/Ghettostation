//################################################################# CONFIGURATION #############################################################################
//# Comment/uncomment/edit according to your needs.
//#############################################################################################################################################################


//########## BOARD ################################################################################################
#define TEENSYPLUS2 // Teensy++2 support. 
//#define MEGA // Arduino Mega boards


//########## OPTIONS ###############################################################################################
#define BARO_ALT // Use Baro for Altitude. Comment for using GPS altitude instead.

// BEARING REFERENCE TO NORTH METHOD
//#define BEARING_METHOD_1  // 1=calculation from home pos & uav pos heading 20m away straight to neutral pan servo pos.        
#define BEARING_METHOD_2   // 2=manually enter the heading reference from a compass        
//#define BEARING_METHOD_3   // 3=don't bother, just point your tracker to the north.                
//#define BEARING_METHOD_4  // 4=Use HMC5883 compass reading                          
         
         
#define MAGDEC -600  // Your local Magnetic Declination in radian. Get it from here: http://magnetic-declination.com/  then convert it in milliradian: http://www.wolframalpha.com/input/?i=%280%C2%B0+5%27%29+in+radians 
                     // only uncomment if using compass.
                     
//#define DEBUG
//#define SIMUGPS // Simulate gps feed, it will disable all other telemetry


//Minimum distance in meters where it will stop moving servos.
#define DONTTRACKUNDER  2


//########### LCD ##################################################################################################


//#define I2CADDRESS 0xc6 // LCD03 i2C from Robot Electronics 
#define I2CADDRESS 0x27 // IIC/I2C/TWI Serial 2004 20x4 lcd.
//#define I2CADDRESS 020 // Arduino-IIC-LCD GY-LCD-V1



//########### TELEMETRY OPTIONS####################################################################################
//### UAVTALK OPENPILOT    
#define UAVTALK_MODE_PASSIVE  // If you already have GCS or OSD running. Arduino won't send anything on the Tx line. If undefined, it will send UAVTalk queries.


//#################################### SERVOS ENDPOINTS ################################################################
//. Those are just default values when not configured yet. They need to be adjusted from the menu.
// They are quiet conservatives values to not overload your servo forcing endpoints when configuring them

//#define PAN_MAXPWM 1500	//max pan servo pwm value
#define PAN_MAXPWM 1099  // max pan servo pwm for ServoCity heavy duty pan

//Max angle clockwise (on the right) defined by PAN_MAXPWM.
//#define PAN_MAXANGLE 90	/180° pan can go 90° left & right. 
#define PAN_MAXANGLE 180  // 360° pan can go 180° left & right.
	
//Max angle counter clockwise (on the left) defined by PAN_MINPWM.					
//#define PAN_MINPWM 1200		//min pan servo pwm value
#define PAN_MINPWM 1812

//#define PAN_MINANGLE 90		//180° pan can go 90° left & right
#define PAN_MINANGLE 180 //360° pan can go 180° left & right.

//#define TILT_MAXPWM 1500	//max tilt pwm value 
#define TILT_MAXPWM 1128     

#define TILT_MAXANGLE 90	//max tilt angle considering 0° is facing toward.
																	
//#define TILT_MINPWM 1000		//min tilt pwm value
#define TILT_MINPWM 1888
#define TILT_MINANGLE 0 	//Means minimum tilt position is parallel to ground. Considering 0 is facing toward, a -10 value would means we can tilt 10° down.



//########################################### BOARDS PINOUTS ####################################################
//you can change them with other pins according to your needs. A


//pintout for TEENSY++ 2
#ifdef TEENSYPLUS2
  #define PAN_SERVOPIN 26	//PWM Pin for pan servo
  #define TILT_SERVOPIN 25	//PWM Pin for tilt servo
  #define LEFT_BUTTON_PIN 10    //Any Digital pin
  #define RIGHT_BUTTON_PIN 9    //Any Digital pin
  #define ENTER_BUTTON_PIN 8    //Any Digital pin
#endif

//pinout for Arduino Mega 1280/2560
#ifdef MEGA
  #define PAN_SERVOPIN 3	//PWM Pin for pan servo
  #define TILT_SERVOPIN 4       //PWM Pin for tilt ervo
  #define LEFT_BUTTON_PIN 32    //Any Digital pin
  #define RIGHT_BUTTON_PIN 34   //Any Digital pin
  #define ENTER_BUTTON_PIN 36   //Any Digital pin
#endif


		

//END OF CONFIG
