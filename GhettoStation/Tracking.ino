void antenna_tracking() {
// Tracking general function
    //only move servo if gps has a 3D fix, or standby to last known position.
    if (uav_fix_type == 3) {
	
	int rel_alt = uav_alt - home_alt; // relative altitude to ground in meters
	
	calc_tracking( home_lon, home_lat, uav_lon, uav_lat, rel_alt); //calculate tracking bearing/azimuth
	
	//set current GPS bearing relative to home_bearing
	
	if(Bearing >= home_bearing){
          Bearing-=home_bearing;
        }
        else
        {
          Bearing+=360-home_bearing;
        }
	// serv command
        if(home_dist>DONTTRACKUNDER) { //don't track when <10m 
	    servoPathfinder(Bearing,Elevation);
        }
   } 
}



void calc_tracking(float lon1, float lat1, float lon2, float lat2, int alt) {
  
  
//// (homelon, homelat, uavlon, uavlat, uavalt ) 
//// Return Bearing & Elevation angles in degree
//  float a, tc1, R, c, d, dLat, dLon;
// 
// // converting to radian
//  lon1=toRad(lon1);
//  lat1=toRad(lat1);
//  lon2=toRad(lon2);
//  lat2=toRad(lat2);
// 
// 
// //calculating bearing in degree decimal
  Bearing = calc_bearing(lon1,lat1,lon2,lat2);
//
// 
////calculating distance between uav & home
  Elevation = calc_elevation(lon1,lat1,lon2,lat2,alt);
 
}


int calc_bearing(float lon1, float lat1, float lon2, float lat2) {
  
  float calc;
  int b;

  float x = 69.1 * (lat2 - lat1);
  float y = 69.1 * (lon2 - lon1) * cos(lat1/57.3);
 
  calc=atan2(y,x);

  b= (int)round(toDeg(calc));

  if(b<0){
    b=360+b;
  }
  return b;


// bearing calc, feeded in radian, output degrees
//	float a;
//	//calculating bearing 
//	a=atan2(sin(lon2-lon1)*cos(lat2), cos(lat1)*sin(lat2)-sin(lat1)*cos(lat2)*cos(lon2-lon1));
//	a=toDeg(a);
//	if (a<0) a=360+a;
//        int b = (int)round(a);
//	return b;
}

int calc_elevation(float lon1, float lat1, float lon2, float lat2, int alt) {
  
  //calc distance

   float x = 69.1 * (lat2 - lat1);
   float y = 69.1 * (lon2 - lon1) * cos(lat1/57.3);
   
   float d = (float)sqrt((float)(x*x) + (float)(y*y))*1609.344;
   
 //calc elevation
   int a = (int)round(toDeg(atan((float)(alt)/(float)d)));
   return a;
}
  

  
////Azimuth cacl, feeded in radian, output in degrees
//  float a, el, c, d, R, dLat, dLon;
//  //calculating distance between uav & home
//  R=6371000.0;    //earth radius 6371km
//  dLat = (lat2-lat1);
//  dLon = (lon2-lon1);
//  a = sin(dLat/2) * sin(dLat/2) + sin(dLon/2) * sin(dLon/2) * cos(lat1) * cos(lat2);
//  c = 2* asin(sqrt(a));  
//  d = R * c;
//  home_dist = d;
//  el=atan(((float)alt)/d);// in radian
//  el=toDeg(el); // in degree
//  int b = (int)round(el);
//  return b;
//}

float toRad(float angle) {
// convert degrees to radians
	angle = angle*0.01745329; // (angle/180)*pi
	return angle;
}

float toDeg(float angle) {
// convert radians to degrees.
	angle = angle*57,29577951;   // (angle*180)/pi
        return angle;
}
	

