
//------------------ Heading and Compass ----------------------------------------

static char buf_show[12];
const char buf_Rule[36] = {0x82,0x80,0x81,0x80,0x81,0x80,
                           0x84,0x80,0x81,0x80,0x81,0x80,
                           0x83,0x80,0x81,0x80,0x81,0x80,
                           0x85,0x80,0x81,0x80,0x81,0x80};
void setHeadingPatern()
{
  int start;
  start = round((osd_heading * 24)/360);
  start -= 3;
  if(start < 0) start += 24;
  for(int x=0; x <= 10; x++){
    buf_show[x] = buf_Rule[start];
    if(++start > 23) start = 0;
  }
  buf_show[7] = '\0';
}

//------------------ Battery Remaining Picture ----------------------------------

char setBatteryPic(uint16_t bat_level)
{
  if(bat_level <= 100){
    return 0xb4;
  }
  else if(bat_level <= 300){
    return 0xb5;
  }
  else if(bat_level <= 400){
    return 0xb6;
  }
  else if(bat_level <= 500){
    return 0xb7;
  }
  else if(bat_level <= 800){
    return 0xb8;
  }
  else return 0xb9;
}

//------------------ Vars Calculation ----------------------------------
void updateVars() { 
    float dstlon, dstlat;
  
//shrinking factor for longitude going to poles direction
    float rads = fabs(osd_home_lat) * 0.0174532925;
    double scaleLongDown = cos(rads);
    double scaleLongUp   = 1.0f/cos(rads);
    if (osd_got_home == 1) {
    //DST to Home
    dstlat = fabs(osd_home_lat - osd_lat) * 111319.5;
    dstlon = fabs(osd_home_lon - osd_lon) * 111319.5 * scaleLongDown;
    osd_home_distance = sqrt(sq(dstlat) + sq(dstlon));
    }
    else 
        osd_home_distance = 0;

    osd_home_direction = round((float)(bearing/360.0f) * 16.0f) + 1;	// array of arrows
    if (osd_home_direction > 16) osd_home_direction = 0;

    dt = millis();
    if (osd_home_distance > max_home_distance) max_home_distance = osd_home_distance;
    if (osd_airspeed > max_osd_airspeed) max_osd_airspeed = osd_airspeed;
    if (osd_groundspeed > max_osd_groundspeed) max_osd_groundspeed = osd_groundspeed;
    if (osd_alt_to_home > max_osd_home_alt) max_osd_home_alt = osd_alt_to_home;
   
}



void setVars(OSD &osd)
{

  updateVars();
  osd_alt_to_home = osd_alt - osd_home_alt;
  //Check arm/disarm switching.
  if((motor_armed == 0) && (last_armed == 0)) {
      //disarmed , & wasn't armed before 
      //do nothing
  }
  else if((motor_armed == 1) && (last_armed == 0)) {

      if (flight_status == 0) {   //first takeoff
        //initialise flight time
        FTime = (millis()/1000.0f);
        start_Time = FTime;
        start_battery_reading = osd_battery_remaining_A;
        last_battery_reading = osd_battery_remaining_A;    
     }
     else {
      disarmed_time += (millis()/1000.0f) - (landed_at_time/1000.0f); // increment time spent on ground after disarm in case of arming again.
       
     }
     flight_status = 1; // in flight
     
  }
  else if((motor_armed == 1) && (last_armed == 1)) {
     //armed, was armed before ==>> in flight, update vars
     start_Time = (millis()/1000) - FTime - disarmed_time;
     flight_status = 1; //in flight
     if (osd_groundspeed > 1.0) tdistance += (osd_groundspeed * (millis() - dt) / 1000.0);
     dt = millis();
  }
  else if((motor_armed == 0) && (last_armed == 1)) {
       //disarmed , was armed before ==>> if distance > 20m do nothing not back to home yet continue update var
       //                                  else show flight resume panel, unset home_pos.
       if (osd_home_distance < 20)
            //unset home pos we've landed near previous home pos.
            flight_status = 3; //landed at home
       else 
            flight_status = 2; //landed not at home
       
       landed_at_time = millis();
  }
  last_armed = motor_armed;
}


