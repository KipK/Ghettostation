#ifdef PROTOCOL_GPS
void gps_read() {
    GPS.Read();
    if (GPS.NewData)  // New GPS data?
        {
          
         uav_satellites_visible = GPS.NumSats;
         uav_fix_type = GPS.Fix;
         if (uav_fix_type == 1) {
             uav_fix_type=3; 
         }
	uav_lat = GPS.Lattitude * 10000000.0;;
	uav_lon = GPS.Longitude * 10000000.0;
	uav_alt = round(GPS.Altitude / 10.0f); //from mm to cm
	uav_groundspeed = round(GPS.Ground_Speed / 100.0f); // in m/s
	uav_heading = round(GPS.Ground_Course / 100.0f);  // in deg
        } 
  
}
#endif
