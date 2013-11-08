#ifdef SIMUGPS

int radius = 6371000;
int simudist = 800;
int simu=0;
void set_simugps() {
  uav_fix_type = 3;
  uav_satellites_visible = 10;
  home_lat = 48.86917;		
  home_lon =  2.24111;		
  uav_alt =  0;
  //home_bearing = 0;
  home_pos = true;
  //home_bear = true;
} 
void simulate_gps() {
  if (simugpsMetro.check() == 1) {
    simu++;
    if (simu<360) {
//            uav_lat = (LAT_STEPS * cos(simu)) + home_lat;
//            uav_lon = (LON_STEPS * sin(simu)) + home_lon;
  uav_lat = toDeg(asin(sin(toRad(home_lat)) * cos(simudist / radius) + cos(toRad(home_lat)) * sin(simudist / radius) * cos(toRad(simu))));
  uav_lon = toDeg(toRad(home_lon) + atan2(sin(toRad(simu)) * sin(simudist / radius) * cos(toRad(home_lat)), cos(simudist / radius) - sin(toRad(home_lat)) * sin(toRad(uav_lat))));

    }
    else simu = 0;
  }
}
#endif
