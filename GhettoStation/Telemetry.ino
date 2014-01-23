void init_serial() {
#if defined(MEGA) 
    Serial1.begin(baudrates[configuration.baudrate]);
#endif
#if defined(TEENSYPLUS2) 
    Uart.begin(baudrates[configuration.baudrate]); 
#endif

#ifdef DEBUG
    Serial.println("Serial initialised"); 
#endif

}

//Preparing adding other protocol
void get_telemetry() {
// if (telemetryMetro.check() == 1) {  
   if (millis() - lastpacketreceived > 2000) { 
      telemetry_ok = false;
   }
 
 // OpenPilot / Taulabs 
   if (configuration.telemetry==0) {
      if (uavtalk_read()) {
         protocol = "UAVT";
      }
   }

// Multiwii / Baseflight
    if (configuration.telemetry==1) {
      msp_read(); 
    }


// Ghettostation custom light protocol. 
   if (configuration.telemetry==2) {
      ltm_read();
   }


// Ardupilot / PixHawk / Taulabs ( mavlink output ) / Other
    if (configuration.telemetry==3) {
      mavlink_read(); 
    }
}

void telemetry_off() {
  //reset uav data
  uav_lat = 0;
  uav_lon = 0;                    
  uav_satellites_visible = 0;     
  uav_fix_type = 0;               
  uav_alt = 0;                    
  uav_groundspeed = 0;
  protocol = "";
  telemetry_ok = false;
}


