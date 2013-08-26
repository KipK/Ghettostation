/**
 ******************************************************************************
 *
 * @file       GhettoStation.ino
 * @author     Guillaume Sartre 
 * @brief      Arduino based antenna tracker & telemetry display for UAV projects.
 * @project	   https://code.google.com/p/ghettostation/
 * 
 *             
 *             
 *
 * @see        The GNU Public License (GPL) Version 3
 *
 *****************************************************************************/

#define CONFIG_VERSION 2004 // Edit only if you want to reset eeprom

#include "Config.h"
#include <avr/pgmspace.h>

#include <Wire.h> 



#ifdef BEARING_METHOD_4
//HMC5883L i2c mag b
#include <HMC5883L.h>
#endif

#include <LiquidCrystal_I2C.h>

#include <Metro.h>
#include <MenuSystem.h>
#include <Button.h>
#include <Servo.h>
#include <EEPROM.h>
#include <Flash.h>



#if defined(SOFT_MODEM)
 #include <SoftModem.h>
#endif


#include "Eeprom.h"
#include "GhettoStation.h"

#ifdef TEENSYPLUS2
// This line defines a "Uart" object to access the serial port
HardwareSerial Uart = HardwareSerial();
#endif

#ifdef PROTOCOL_UAVTALK
#include "UAVTalk.cpp"
#endif

#ifdef PROTOCOL_MSP
#include "MSP.cpp"
#endif

#ifdef PROTOCOL_MAVLINK
#include <mavlink.h>
#include "Mavlink.cpp"
#endif



//################################### SETTING OBJECTS ###############################################






// Set the pins on the I2C chip used for LCD connections:
//                    addr, en,rw,rs,d4,d5,d6,d7,bl,blpol
LiquidCrystal_I2C LCD(I2CADDRESS, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // LCM1602 IIC A0 A1 A2 & YwRobot Arduino LCM1602 IIC V1" 
//iquidCrystal_I2C lcd(I2CADDRESS, 4, 5, 6, 0, 1, 2, 3, 7, NEGATIVE);  // Arduino-IIC-LCD GY-LCD-V1



//##### SERVOS 

//Declaring pan/tilt servos using ServoEaser library
 Servo pan_servo;
 Servo tilt_servo;

//#####	RATE LOOPS 
//setting telemetry refresh rate.
//Metro telemetryMetro = Metro(60);
//setting lcd refresh rate 
Metro lcdMetro = Metro(100);
//setting button status check loop
Metro buttonMetro = Metro(100);
//setting activity loop time
Metro activityMetro = Metro(200);

#if defined(DEBUG)
//Debug output
Metro debugMetro = Metro(1000); // output serial debug data each second.
#endif


//##### BUTTONS 
Button right_button = Button(RIGHT_BUTTON_PIN,BUTTON_PULLUP_INTERNAL);
Button left_button = Button(LEFT_BUTTON_PIN,BUTTON_PULLUP_INTERNAL);
Button enter_button = Button(ENTER_BUTTON_PIN,BUTTON_PULLUP_INTERNAL);



//##### SOFTMODEM AUDIO TELEMETRY
#ifdef SOFT_MODEM
SoftModem modem;
int modemTimer=0;
#endif


//#################################### SETUP LOOP ####################################################

void setup() {


//init LCD
init_lcdscreen();



	//init setup
	init_menu();
	
	// retrieve configuration from EEPROM
	EEPROM_read(0, configuration);
        // set temp value for servo pwm config
        servoconf_tmp[0] = configuration.pan_minpwm;
        servoconf_tmp[1] = configuration.pan_maxpwm;
        servoconf_tmp[2] = configuration.tilt_minpwm;
        servoconf_tmp[3] = configuration.tilt_maxpwm;
        delay(20);
	//clear eeprom & write default parameters if config is empty or wrong
	if (configuration.config_crc != CONFIG_VERSION) {
		clear_eeprom();
                delay(20);
		}


         //start serial com	
	init_serial();
         
         
#ifdef SOFT_MODEM
         //start softmodem
        modem.begin ();
#endif
	
	// attach servos 
	attach_servo(pan_servo, PAN_SERVOPIN, configuration.pan_minpwm, configuration.pan_maxpwm);
	attach_servo(tilt_servo, TILT_SERVOPIN, configuration.tilt_minpwm, configuration.tilt_maxpwm); 

        
	// move servo to neutral pan & 45° tilt at startup to prevent forcing on endpoints if misconfigured
        servoPathfinder(0, 0);
       
       // setup button callback events
       enter_button.releaseHandler(enterButtonReleaseEvents);
       left_button.releaseHandler(leftButtonReleaseEvents);
       right_button.releaseHandler(rightButtonReleaseEvents);
       

}

//######################################## MAIN LOOP #####################################################################
void loop() {

        //update buttons internal states
        if (buttonMetro.check() == 1) {
        enter_button.isPressed();
        left_button.isPressed();
        right_button.isPressed();
        }
	//get telemetry data 
        get_telemetry();
//	test_servos();
	//checking activity
        check_activity();     
        //lcd refresh loop
	refresh_lcd();
#if defined(DEBUG)
        //debug output
        debug();

#endif
        
}

