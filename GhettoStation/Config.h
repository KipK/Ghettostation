/*############################################## CONFIGURATION ####################################################
 # Comment/uncomment/edit according to your needs.
 ##################################################################################################################*/
 #define CONFIG_VERSION 2011 // Edit only if you want to reset eeprom
//########## BOARD ################################################################################################
 #define TEENSYPLUS2 // Teensy++2 support. 
//#define MEGA // Arduino Mega boards

/* If you have communication problem at 56K , set this on. ( ie APM 2/2.5/2.6/AIO )
   Most Arduino have a +2.18% error at 57600 bd, Teensyduino has a -0.74% error. Booth cumulated are too much.
   Successfull com between Teensy & arduino requires 58824 bauds for Teensy.*/
//#define BAUDRATE56K 57600
  #define BAUDRATE56K 58824 


//########## OPTIONS ###############################################################################################
 #define BARO_ALT // Use Baro for Altitude. Comment for using GPS altitude instead.
 
//Use Mag+imu for heading or GPS heading if not set.
  #define MAGHEADING 1

// BEARING REFERENCE TO NORTH METHOD
//#define BEARING_METHOD_1  // 1=calculation from home pos & uav pos heading 20m away straight to neutral pan servo pos.        
  #define BEARING_METHOD_2   // 2=manually enter the heading reference from a compass        
//#define BEARING_METHOD_3   // 3=don't bother, just point your tracker to the north.                
//#define BEARING_METHOD_4  // 4=Use HMC5883 compass reading                          
               
 #define MAGDEC -600  // Your local Magnetic Declination in radian. Get it from here: http://magnetic-declination.com/  then convert it in milliradian: http://www.wolframalpha.com/input/?i=%280%C2%B0+5%27%29+in+radians 
                     // only uncomment if using compass.

//Minimum distance in meters where it will stop moving servos.
 #define DONTTRACKUNDER  2

// Prevent Ghettostation to send packets to the flightcontroler
// Usefull if you're using OSD or a GCS at the same time.
 #define PASSIVEMODE 1



//Memory bank name to display on LCD (18 char max)
 #define BANK1  "1.2 GHZ"
 #define BANK2  "5.8 Ghz"
 #define BANK3  "Bank 3"
 #define BANK4  "Bank 4"

//########### LCD ##################################################################################################


//#define I2CADDRESS 0xc6 // LCD03 i2C from Robot Electronics 
 #define I2CADDRESS 0x27 // IIC/I2C/TWI Serial 2004 20x4 lcd ( Hobbyking )
//#define I2CADDRESS 020 // Arduino-IIC-LCD GY-LCD-V1


//#################################### SERVOS ENDPOINTS #############################################################
//. Those are just default values when not configured yet. 
// To prevent burning servo they boot starts at neutral for all values. Adjust them directly from the menu.

 #define PAN_MAXPWM 1500     //max pan servo pwm value
 #define PAN_MAXANGLE 90     //Max angle clockwise (on the right) relative to PAN_MAXPWM. 
 #define PAN_MINPWM 1500     //min pan servo pwm value
 #define PAN_MINANGLE 90	    //Max angle counter clockwise (on the left) relative to PAN_MINPWM.

 #define TILT_MAXPWM 1500    //max tilt pwm value 
 #define TILT_MAXANGLE 90    //max tilt angle considering 0° is facing toward.
 #define TILT_MINPWM 1500    //min tilt pwm value
 #define TILT_MINANGLE 0     //minimum tilt angle. Considering 0 is facing toward, a -10 value would means we can tilt 10° down.



//########################################### BOARDS PINOUTS #########################################################
// Change them with other compatible pins according to your needs. 


//pinout for TEENSY++ 2
#ifdef TEENSYPLUS2
  #define PAN_SERVOPIN 26	//PWM Pin for pan servo ( Timer 1, do not use with altsoftserial )
  #define TILT_SERVOPIN 25	//PWM Pin for tilt servo ( Timer 1, do not use with altsoftserial )
  #define LEFT_BUTTON_PIN 10    //Any Digital pin
  #define RIGHT_BUTTON_PIN 9    //Any Digital pin
  #define ENTER_BUTTON_PIN 8    //Any Digital pin
  #define SOFTSERIAL_TX 14      //Digital pin used by SoftSerial for sending telemetry to ground osd.
#endif

//pinout for Arduino Mega 1280/2560
#ifdef MEGA
  #define PAN_SERVOPIN 3	//PWM Pin for pan servo
  #define TILT_SERVOPIN 4       //PWM Pin for tilt ervo
  #define LEFT_BUTTON_PIN 32    //Any Digital pin
  #define RIGHT_BUTTON_PIN 34   //Any Digital pin
  #define ENTER_BUTTON_PIN 36   //Any Digital pin
#endif		

//################################################## DEBUG ##########################################################
//#define DEBUG
//#define SIMUGPS // Simulate gps feed, it will disable all other telemetry

//###############################################END OF CONFIG#######################################################

