#ifdef SIMUGPS
#define LAT_STEPS	0.000009	// about 1m
#define LON_STEPS	0.000014	// about 1m at latitude of 48.8582�
int simstatus=1;
int simcounter=0;
void set_simugps() {
  uav_fix_type = 3;
  uav_satellites_visible = 10;
  uav_lat = 48.86917;		
  uav_lon =  2.24111;		
  uav_alt =  0;
  home_lat = uav_lat;
  home_lon = uav_lon; 
  home_alt = uav_alt;
  home_bearing = 0;
  home_pos = true;
  home_bear = true;
} 
void simulate_gps() {
  if (simugpsMetro.check() == 1) {
    simcounter++;
    
    for (i=0, i++, i<360) {
      //x = cx + r * cos(a)
      //y = cy + r * sin(a)
      
      x = home_lat + (100*cos(i));
      
        uav_lat = home_lat + LAT_STEPS/50;;
        uav_lon = uav_lon + LON_STEPS;
         uav_alt++;
    }
    
    
    switch (simstatus) {
      case 1:  
               
                uav_lat = uav_lat + LAT_STEPS/50;;
                uav_lon = uav_lon + LON_STEPS;
                uav_alt++;
                if (simcounter==200) {
                    simcounter=0;
                    simstatus++; 
                }
               break;
      case 2:  

                uav_lat = uav_lat - (LAT_STEPS/50);
                uav_lon = uav_lon + LON_STEPS;
                uav_alt--;
                if (simcounter==200) {
                    simcounter=0;
                    simstatus++; 
                }
               break;
               
       case 3:
              //etc..
                uav_lat = uav_lat - LAT_STEPS;
                uav_lon = uav_lon - (LON_STEPS/50);
                uav_alt++;
                if (simcounter==200) {
                    simcounter=0;
                    simstatus++; 
                }
               break;
        case 4:
              //etc
                uav_lat = uav_lat + (LAT_STEPS/50);
                uav_lon = uav_lon - LON_STEPS;
                uav_alt = 50;// glitch on altitude
                if (simcounter==200) {
                    simcounter=0;
                    simstatus++; 
                }
               break; 
      case 5:
                //etc
                uav_lat = uav_lat + LAT_STEPS;
                uav_lon = uav_lon + (LON_STEPS/50);
                uav_alt = 50;// glitch on altitude
                if (simcounter==200) {
                    simcounter=0;
                    simstatus=1; 
                }
               break;      
      default:
               break;
    }
  }
  
}
#endif
