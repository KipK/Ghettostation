/*############################################## CONFIGURATION ####################################################
 # Comment/uncomment/edit according to your needs.
 ##################################################################################################################*/
 #define CONFIG_VERSION 1003 // Changing this will reset eeprom to default values
//########## BOARD ################################################################################################
 #define TEENSYPLUS2 // Teensy++2 support. 
//#define MEGA // Arduino Mega board

//########## OPTIONS ###############################################################################################

/* If you have communication problem at 56K , set this on. ( ie APM 2/2.5/2.6/AIO )
   Most Arduino have a +2.18% error at 57600 bd, Teensyduino has a -0.74% error. Booth cumulated are too much.
   Successfull com between Teensy & arduino requires 58824 bauds for Teensy.*/
//#define BAUDRATE56K 57600
 #define BAUDRATE56K 58824 
 
 #define BARO_ALT // Use Baro for Altitude. Comment for using GPS altitude instead.
 
//Use Mag+imu for heading or GPS heading if not set ( not used for tracker only osd relay )
 #define MAGHEADING 1
                         
               
 #define MAGDEC -600  // Your local Magnetic Declination in radian. Get it from here: http://magnetic-declination.com/  then convert it in milliradian: http://www.wolframalpha.com/input/?i=%280%C2%B0+5%27%29+in+radians 
                     // only needed if using internal compass.

//Minimum distance in meters where it will stop moving servos.
 #define DONTTRACKUNDER  5

// Prevent Ghettostation to send packets to the flightcontroler
// Usefull if you're using OSD or a GCS at the same time.
 #define PASSIVEMODE 0

// Default tilt angle used when not tracking.
 #define DEFAULTELEVATION  15

// Filter out gps reading if too far from last result (in meters)
#define GPS_DISTANCE_DELTA_MAX 500

//Memory bank name to display on LCD (18 char max)
 #define BANK1  "1.2 GHZ"
 #define BANK2  "5.8 Ghz"
 #define BANK3  "Bank 3"
 #define BANK4  "Bank 4"
 
 //GS Battery alarm
 #define MIN_VOLTAGE1 10.5f // First battery alarm level. Will emit 2 short tones every 10 sec.
 #define MIN_VOLTAGE2 10.0f // Second battery alarm level. Will emit 1 short + 1 long tone every 5 sec
 #define VOLTAGE_RATIO 600   // Default multiplier for battery voltage reading * 100. This can eb adjustd later from the menu.
 
 // Minimum voltage for lipo alert
//########### GROUND OSD TELEMETRY OUTPUT #########################################################################
// Activate osd output (comment if not needed)
 #define OSD_OUTPUT 
//OSD output baudrate ( send data as fast as possible to the osd, no need to have the same baudrate as input one. )
 #define OSD_BAUD 57600
//########### LCD ##################################################################################################
//LCD model
#define LCDLCM1602 // (adress: 0x27 or 0x20) HobbyKing IIC/I2C/TWI Serial 2004 20x4, LCM1602 IIC A0 A1 A2 & YwRobot Arduino LCM1602 IIC V1
//#define LCDGYLCD  // (adress: 0x20) Arduino-IIC-LCD GY-LCD-V1Arduino-IIC-LCD GY-LCD-V1
//#define LCD03I2C  // (adress: 0x63 or  0xc6) LCD03 / LCD05 
//#define GLCDEnable // Graphical LCD - Using system5x7 font so its nearly 20x4 size
//#define OLEDLCD  // Oled 128x64 i2c LCD (address 0x3C or 0x3D)

// I2C LCD Adress
#define I2CADRESS 0x27 // LCD03/05 have 0x63 or 0xc6 ( even if it's written 0xc6 when powering the lcd03, in fact it uses 0x63 so try booth)
                       // LCM1602 uses 0x27 & GY-LCD use 0x20
                       // OLED_LCD use 0x3d or 0x3d
//#################################### SERVOS ENDPOINTS #############################################################
// NO NEED TO EDIT THIS
//. Those are just default values when not configured.
// To prevent burning servo they boot starts at neutral for all values. Adjust them directly from the menu.

 #define PAN_MAXPWM 1500     //max pan servo pwm value
 #define PAN_MAXANGLE 90     //Max angle clockwise (on the right) relative to PAN_MAXPWM. 
 #define PAN_MINPWM 1500     //min pan servo pwm valuemin pan servo pwm value
 #define PAN_MINANGLE 90	    //Max angle counter clockwise (on the left) relative to PAN_MINPWM.

 #define TILT_MAXPWM 1500    //max tilt pwm value 
 #define TILT_MAXANGLE 90    //max tilt angle considering 0° is facing toward.
 #define TILT_MINPWM 1500    //min tilt pwm value
 #define TILT_MINANGLE 0     //minimum tilt angle. Considering 0 is facing toward, a -10 value would means we can tilt 10° down.



//########################################### BOARDS PINOUTS #########################################################
//DON'T EDIT THIS IF YOU DON'T KNOW WHAT YOU'RE DOINGG
//pinout for TEENSY++ 2
#ifdef TEENSYPLUS2
  #define PAN_SERVOPIN     26    //PWM Pin for pan servo 
  #define TILT_SERVOPIN    25 	 //PWM Pin for tilt servo 
  #define LEFT_BUTTON_PIN  10    //Any Digital pin
  #define RIGHT_BUTTON_PIN  9    //Any Digital pin
  #define ENTER_BUTTON_PIN  8    //Any Digital pin
  #define SOFTSERIAL_TX    14    //Digital pin used by SoftSerial for sending data to ground osd.
  #define SOFTSERIAL_RX     0    //Digital pin used by SoftSerial for receiving data from ground osd. ( unused yet )
  #define ADC_VOLTAGE      38    //(F0) ADC pin used for voltage reading
  #define BUZZER_PIN       16    //(C6) Any PWM pin (add a 100-150 ohm resistor between buzzer & ground)
#endif

//pinout for Arduino Mega 1280/2560
#ifdef MEGA
  #define PAN_SERVOPIN     11	//PWM Pin for pan servo
  #define TILT_SERVOPIN    12   //PWM Pin for tilt ervo
  #define LEFT_BUTTON_PIN  32   //Any Digital pin
  #define RIGHT_BUTTON_PIN 34   //Any Digital pin
  #define ENTER_BUTTON_PIN 36   //Any Digital pin
  #define ADC_VOLTAGE      41   //(A5) ADC pin used for voltage reading
  #define BUZZER_PIN        8   //(PH5) Any PWM pin ((add a 100-150 ohm resistor between buzzer & ground)
#endif

//################################################## DEBUG ##########################################################
//#define DEBUG

//###############################################END OF CONFIG#######################################################

