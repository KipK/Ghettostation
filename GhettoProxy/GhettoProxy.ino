/**
 ******************************************************************************
 *
 * @file       GhettoStation.ino
 * @author     Guillaume S
 * @brief      Arduino based antenna tracker & telemetry display for UAV projects.
 * @project    https://code.google.com/p/ghettostation/
 * 
 *             
 *             
 *
 * @see        The GNU Public License (GPL) Version 3
 *
 *****************************************************************************
*/
#include <avr/pgmspace.h>
#include <arduino.h>

#include "Config.h"

#include <AltSoftSerial.h>
//#include <SoftwareSerial.h>
#include <Metro.h>
#include "GhettoStation.h"

#include "LightTelemetry.cpp"

/*
 * BOF preprocessor bug prevent
 */
#define nop() __asm volatile ("nop")
#if 1
nop();
#endif
/*
 * EOF preprocessor bug prevent
*/


#ifdef PROTOCOL_GPS
/* Uncomment the gps protocol you use */
//#include <GPS_NMEA.h>
//#include <GPS_UBLOX.h>
//#include <GPS_MTK.h>
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

//##### LOOP RATES

Metro loop10hz = Metro(100); //10hz loop  
Metro loopTelemetry = Metro(1);  


//#################################### SETUP LOOP ####################################################

void setup() {
    //start serial com  
    init_serial();
    #ifdef PROTOCOL_GPS 
     GPS.Init();
    #endif
}

//######################################## MAIN LOOP #####################################################################
void loop() {
  
 //get_telemetry();  
 
 if (loop10hz.check()) {
   
   send_LTM();
        
 }
 if (loopTelemetry.check()) {
   get_telemetry();
 }
}

//######################################## TELEMETRY FUNCTIONS #############################################
void init_serial() {
    
    SerialPort1.begin(INPUT_BAUD);
    SerialPort2.begin(OUTPUT_BAUD);
}

//Preparing adding other protocol
void get_telemetry() {
        
#if defined(PROTOCOL_UAVTALK) // OpenPilot / Taulabs 
      uavtalk_read();
#endif

#if defined(PROTOCOL_MSP) // Multiwii
      #ifndef PASSIVEMODE // query MSP packets
      static unsigned long previous_millis_low = 0;
      static unsigned long previous_millis_high = 0;
      static unsigned long previous_millis_onsec = 0;
      static uint8_t queuedMSPRequests = 0;
      unsigned long currentMillis = millis();
      if((currentMillis - previous_millis_low) >= 1000) // 1hz
      {
       setMspRequests(); 
      }
      if((currentMillis - previous_millis_low) >= 100)  // 10 Hz (Executed every 100ms)
      {
      blankserialRequest(MSP_ATTITUDE); 
      previous_millis_low = millis();
      }
      if((currentMillis - previous_millis_high) >= 200) // 20 Hz (Executed every 50ms)
      {
      uint8_t MSPcmdsend;
      if(queuedMSPRequests == 0)
        queuedMSPRequests = modeMSPRequests;
      uint32_t req = queuedMSPRequests & -queuedMSPRequests;
      queuedMSPRequests &= ~req;
      switch(req) {
        case REQ_MSP_IDENT:
          MSPcmdsend = MSP_IDENT;
          break;
        case REQ_MSP_STATUS:
          MSPcmdsend = MSP_STATUS;
          break;
        case REQ_MSP_RAW_GPS:
          MSPcmdsend = MSP_RAW_GPS;
          break;
        case REQ_MSP_ALTITUDE:
          MSPcmdsend = MSP_ALTITUDE;
          break;
        case REQ_MSP_ANALOG:
          MSPcmdsend = MSP_ANALOG;
          break;
      } 
      previous_millis_high = millis();
      }
      #endif
      msp_read(); 
#endif


#if defined(PROTOCOL_MAVLINK) // Ardupilot / PixHawk / Taulabs ( mavlink output ) / Other
      if(enable_frame_request == 1){//Request rate control
    enable_frame_request = 0;
        if (!PASSIVEMODE) {
           request_mavlink_rates();
        }
      }
      read_mavlink(); 
#endif

#if defined(PROTOCOL_GPS )
 gps_read();
#endif
}

#ifdef DEBUG
Serial.print();
Serial.println();
Serial.print();
Serial.println();
Serial.print();
Serial.println();
Serial.print();
Serial.println();
Serial.print();
Serial.println();
Serial.print();
Serial.println();
Serial.print();
Serial.println();
Serial.print();
Serial.println();
Serial.print();
Serial.println();
Serial.print();
Serial.println();
Serial.print();
Serial.println();
Serial.print();
Serial.println();

#endif
