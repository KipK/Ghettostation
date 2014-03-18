/*


Program  : GhettOSD (Supports the variant: minimOSD)
Version  : V1.0
Author(s): Guillaume S

GhettOSD is forked from minimosd-extra. Mavlink support has been removed. 
GhettOSD use LightTelemetry protocol sent by Ghettostation: https://github.com/KipK/Ghettostation/wiki


Original minimod-extra credits:
Author(s): Sandro Benigno
Coauthor(s):
Jani Hirvinen   (All the EEPROM routines)
Michael Oborne  (OSD Configutator)
Mike Smith      (BetterStream and Fast Serial libraries)
Gábor Zoltán
Pedro Santos
Special Contribuitor:
Andrew Tridgell by all the support on MAVLink
Doug Weibel by his great orientation since the start of this project
Contributors: James Goppert, Max Levine, Burt Green, Eddie Furey
and all other members of DIY Drones Dev team
Thanks to: Chris Anderson, Jordi Munoz


This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>

*/

/* ************************************************************ */
/* **************** MAIN PROGRAM - MODULES ******************** */
/* ************************************************************ */

#undef PROGMEM 
#define PROGMEM __attribute__(( section(".progmem.data") )) 

#undef PSTR 
#define PSTR(s) (__extension__({static prog_char __c[] PROGMEM = (s); &__c[0];})) 

#define isPAL 1

/* **********************************************/
/* ***************** INCLUDES *******************/

//#define membug 
//#define FORCEINIT  // You should never use this unless you know what you are doing 

// AVR Includes
#include <FastSerial.h>
#include <math.h>
#include <inttypes.h>
#include <avr/pgmspace.h>
// Get the common arduino functions
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "wiring.h"
#endif
#include <EEPROM.h>
#include <Metro.h>

#ifdef membug
#include <MemoryFree.h>
#endif

// Configurations
#include "OSD_Config.h"
#include "ArduCam_Max7456.h"
#include "OSD_Vars.h"
#include "OSD_Func.h"
#include "Lighttelemetry.cpp"

/* *************************************************/
/* ***************** DEFINITIONS *******************/

//OSD Hardware 
//#define ArduCAM328
#define MinimOSD

#define TELEMETRY_SPEED  57600  // How fast our MAVLink telemetry is coming to Serial port
#define BOOTTIME         4000   // Time in milliseconds that we show boot loading bar and wait user input

// Objects and Serial definitions
FastSerialPort0(Serial);
OSD osd; //OSD object 

Metro osdMetro = Metro(100);  //( 10hz )


/* **********************************************/
/* ***************** SETUP() *******************/

void setup() 
{
#ifdef ArduCAM328
    pinMode(10, OUTPUT); // USB ArduCam Only
#endif
    pinMode(MAX7456_SELECT,  OUTPUT); // OSD CS

    Serial.begin(TELEMETRY_SPEED);


#ifdef membug
    Serial.println(freeMem());
#endif

    // Prepare OSD for displaying 
    unplugSlaves();
    osd.init();

    // Start 
    startPanels();
    delay(500);

    // OSD debug for development (Shown at start)
#ifdef membug
    osd.setPanel(1,1);
    osd.openPanel();
    osd.printf("%i",freeMem()); 
    osd.closePanel();
#endif

    // Just to easy up development things
#ifdef FORCEINIT
    InitializeOSD();
#endif


    // Check EEPROM to see if we have initialized it already or not
    // also checks if we have new version that needs EEPROM reset
//    if(readEEPROM(CHK1) + readEEPROM(CHK2) != VER) {
//        osd.setPanel(6,9);
//        osd.openPanel();
//        osd.printf_P(PSTR("Missing/Old Config")); 
//        osd.closePanel();
        //InitializeOSD();
//    }

    // Get correct panel settings from EEPROM
    readSettings();
    for(panel = 0; panel < npanels; panel++) readPanelSettings();
    panel = 0; //set panel to 0 to start in the first navigation screen
    // Show bootloader bar
    //loadBar();
    delay(2000);
    Serial.flush();

    // House cleaning, clear display and enable timers
    osd.clear();

} // END of setup();



/* ***********************************************/
/* ***************** MAIN LOOP *******************/

// Mother of all happenings, The loop()
// As simple as possible.
void loop() 
{
    ltm_read();
//    if (osdMetro.check() == 1) {
    if (LTMpassed = 1) {  // refresh osd since we've received a new data packet.
       osd_refresh(); 
       LTMpassed = 0;
     }
}

/* *********************************************** */
/* ******** functions used in main loop() ******** */
void osd_refresh()
{
    setHeadingPatern();  // generate the heading patern

    //osd_battery_pic_A = setBatteryPic(osd_battery_remaining_A);     // battery A remmaning picture
    //osd_battery_pic_B = setBatteryPic(osd_battery_remaining_B);     // battery B remmaning picture

    setVars(osd); 
    
    writePanels();       // writing enabled panels (check OSD_Panels Tab)
    
}


void unplugSlaves(){
    //Unplug list of SPI
#ifdef ArduCAM328
    digitalWrite(10,  HIGH); // unplug USB HOST: ArduCam Only
#endif
    digitalWrite(MAX7456_SELECT,  HIGH); // unplug OSD
}
