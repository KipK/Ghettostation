#ifdef SOFT_MODEM
int string_index = 0;
volatile unsigned char data = 0;
int string_index_max =0;
char string_buf[80];      //string_buf[40];
static int XOR;
int checksum_tx=0; //the checksum generated
int checksum_rx=0;

// Package Pointers
char *token;
char *search = ",";
char *search2= "*";
char *brkb, *pEnd;

int modem_read(void) {
    while (modem.available() > 0) {
  	uint8_t c = modem.read();
        string_index++;          
         if(data == 0b00100100){   
           string_index_max=string_index-2;// 
           protocol = "AFSK"; 
           recover_gps();  
           string_index = 0;          
           string_buf[0] = 0b00100100;     
        }
       else
        {
         string_buf[string_index] = data;
        }
    }
}



void recover_gps(){
  if (strncmp (string_buf, "$PAK", 4) == 0)//looking for $PAK head.... (str1, str2, max number characters to becompared, devolve ZERO se forem iguais)
       {
        int32_t softmodem_lat;
        int32_t softmodem_lon;
        int softmodem_alt;
        int softmodem_speed;
        int softmodem_sats;
        int softmodem_fixtype;
        /* Token will point to the data between comma "'", returns the data in the order received */
	/*THE $PAK order is: UTC, date, Lat, Lon, alt*/
	token = strtok_r(string_buf, search, &brkb); //Contains the header $PAK
	token = strtok_r(NULL, search, &brkb); //Contains Latitude  
        softmodem_lat = atol(token);
	token = strtok_r(NULL, search, &brkb);  //Contains longitude  
        softmodem_lon= atol(token);
	token = strtok_r(NULL, search, &brkb); //Contains altitude
	softmodem_alt = atoi(token);
        token = strtok_r(NULL, search, &brkb); //Contains sats visible
        softmodem_sats = atoi(token);
        token = strtok_r(NULL, search, &brkb); //Contains gps fix type
        softmodem_fixtype = atoi(token);


        int c;
        int n;
        int i;
        for (checksum_rx=0, i = 0; i <100; i++) {
          if(string_buf[i] =='*'){
            checksum_tx = strtol(&string_buf[i+1],NULL, 16);
            break;
          }
          else
          {
            c= (unsigned char) string_buf[i];
            if (c != ',') checksum_rx ^= c;
          }
        }
       
        if(checksum_tx == checksum_rx){
         uav_lat = softmodem_lat / 10000000;
         uav_lon = softmodem_lon / 10000000;
         uav_alt = softmodem_alt;
         uav_fix_type=softmodem_fixtype;
         uav_satellites_visible = softmodem_sats;
         uav_groundspeed = softmodem_speed;
        }
       }
}


   
   
#endif
