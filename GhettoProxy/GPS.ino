#if defined (PROTOCOL_NMEA) || defined (PROTOCOL_UBLOX) || defined (PROTOCOL_MTK)
void gps_read() {
    GPS.Read();
    if (GPS.NewData)  // New GPS data?
        {
          
         uav_satellites_visible = GPS.NumSats;
         uav_fix_type = GPS.Fix;
         #if defined (PROTOCOL_NMEA) || defined (PROTOCOL_UBLOX)
         if (uav_fix_type == 1) {
             uav_fix_type = 3; // 3D fix
         }
         else 
             uav_fix_type == 1; // no fix
         #endif
         // MTK already have 4 sat fix status.
	uav_lat = GPS.Lattitude;
	uav_lon = GPS.Longitude;
        #if defined (PROTOCOL_NMEA)
	uav_alt = round(GPS.Altitude / 10.0f); //from mm to cm
        #else
        uav_alt = GPS.Altitude; //in cm
        #endif
	uav_groundspeed = round(GPS.Ground_Speed / 100.0f); // in m/s
	uav_heading = round(GPS.Ground_Course / 100.0f);  // in deg
        } 
  
}
#endif
