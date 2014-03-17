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

#include "Config.h"

#include <SoftwareSerial.h>
#include <Metro.h>
#include "GhettoStation.h"
#include "LightTelemetry.cpp"

#include <avr/pgmspace.h>
#include <arduino.h>
#ifdef PROTOCOL_NMEA
#include <GPS_NMEA.h>
#define GPSTELEMETRY
#endif
#ifdef PROTOCOL_UBLOX
#include <GPS_UBLOX.h>
#define GPSTELEMETRY
#endif
#ifdef PROTOCOL_MTK
#include <GPS_MTK.h>
#define GPSTELEMETRY
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

///*
// * BOF preprocessor bug prevent
// */
//#define nop() __asm volatile ("nop")
//#if 1
//nop();
//#endif
///*
// * EOF preprocessor bug prevent
//*/

//################################### SETTING OBJECTS ###############################################

//##### LOOP RATES
Metro loop4hz = Metro(250);  // 4hz loop
Metro loop5hz = Metro(200);  // 5hz loop
Metro loop10hz = Metro(100); //10hz loop  


//#################################### SETUP LOOP ####################################################

void setup() {
    //start serial com  
    init_serial();
    #ifdef GPSTELEMETRY
    GPS.Init();
    #endif
    if (OUTPUT_BAUD < 2400)
      slowrate = 1;
}

//######################################## MAIN LOOP #####################################################################
void loop() {
  
 get_telemetry();  
 
 if (loop4hz.check()) {
   if (ltm_counter == 0) {
       send_LTM_Gframe();
       ltm_counter = 1;
   }
       
   else {
       send_LTM_Sframe();
       ltm_counter = 0;
   }
 }
 if (loop5hz.check()) {
   if (slowrate == 1)
        send_LTM_Aframe();
 }
  
  if (loop10hz.check() == 1) {
    if (slowrate == 0)
        send_LTM_Aframe();
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
}

