/**
 ******************************************************************************
 *
 * @file       GhettoStation.ino
 * @author     Guillaume Sartre 
 * @brief      Arduino based antenna tracker & telemetry display for UAV projects.
 * 
 *
 *             UAVTalk arduino api stripped down from minoposd: 
 *             http://code.google.com/p/minoposd/wiki/OpenPilot
 *
 * @see        The GNU Public License (GPL) Version 3
 *
 *****************************************************************************/

#define CONFIG_VERSION 2001 // Edit only if you want to reset eeprom

#include "Config.h"
#include <avr/pgmspace.h>
#include <LCD03_I2C.h>
#include <Metro.h>
#include <MenuSystem.h>
#include <Button.h>
#include <Servo.h>
#include <EEPROM.h>
#include <Flash.h>
#include "Eeprom.h"
#include "GhettoStation.h"


#if defined(PROTOCOL_UAVTALK)
#include "UAVTalk.cpp"
#endif

#if defined(PROTOCOL_MSP)
#include "MSP.cpp"
#endif



//################################### SETTING OBJECTS ###############################################


//##### LCD
//Configuring LCD ( adress, width, lines)
LCD03_I2C LCD(0x63,20,4);

//##### SERVOS 
//Declaring pan/tilt servos using ServoEaser library
Servo pan_servo;
//ServoEaser pan_servoEaser;
Servo tilt_servo;
//ServoEaser tilt_servoEaser;

//#####	RATE LOOPS 
//setting telemetry refresh rate.
Metro telemetryMetro = Metro(100);
//setting lcd refresh rate at 5hz.
Metro lcdMetro = Metro(200);
//setting tracking loop at 5hz ( same as gps rate on uav, servo loops are running at 50hz)
//Metro trackingMetro = Metro(100);
//setting button status check loop
Metro buttonMetro = Metro(100);
//setting activity loop time
Metro activityMetro = Metro(100);

//##### BUTTONS 
Button right_button = Button(RIGHT_BUTTON_PIN,BUTTON_PULLUP_INTERNAL);
Button left_button = Button(LEFT_BUTTON_PIN,BUTTON_PULLUP_INTERNAL);
Button enter_button = Button(ENTER_BUTTON_PIN,BUTTON_PULLUP_INTERNAL);




//#################################### SETUP LOOP ####################################################

void setup() {


    pinMode(LED_PIN, OUTPUT);
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
	
	// attach servos 
	attach_servo(pan_servo, PAN_SERVOPIN, configuration.pan_minpwm, configuration.pan_maxpwm);
	attach_servo(tilt_servo, TILT_SERVOPIN, configuration.tilt_minpwm, configuration.tilt_maxpwm); 

        
	// move servo to neutral pan & 45° tilt at startup to prevent forcing on endpoints if misconfigured
//	move_servo(pan_servo, 1, 0, configuration.pan_minangle, configuration.pan_maxangle);
//	move_servo(tilt_servo, 2, 0, configuration.tilt_minangle, configuration.tilt_maxangle);
        servoPathfinder(0, 0);
//       pan_servoEaser.easeTo( 0, 2000);
//       tilt_servoEaser.easeTo( 0, 2000);
       
       // setup button callback events
       enter_button.releaseHandler(enterButtonReleaseEvents);
       left_button.releaseHandler(leftButtonReleaseEvents);
       right_button.releaseHandler(rightButtonReleaseEvents);
       //left_button.setHoldThreshold(1000);
       //right_button.setHoldThreshold(1000);

}

//######################################## MAIN LOOP #####################################################################
void loop() {
  
        //update buttons internal states
        if (buttonMetro.check() == 1) {
        enter_button.isPressed();
        left_button.isPressed();
        right_button.isPressed();
        }
//        enter_button.isPressed();
//        left_button.isPressed();
//        right_button.isPressed();
	//get telemetry data ( default at 10hz)
	
        get_telemetry();
	
	//checking activity
        check_activity();     
        //lcd refresh loop ( default 10hz )
	refresh_lcd();
      
        
}

