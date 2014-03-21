/*########################################### BOARD PINOUTS #########################################################
 Pins for Arduino Mini
 *SERIAL INPUT PINOUT:
 
     Use TX/RX pads
     
 *SERIAL OUTPUT PINOUT:
 
     TX to D9 (PIN 12)( arduino nano )
     RX to D8 (PIN 11) ( arduino nano ) 
*/
/*############################################## CONFIGURATION ####################################################
 # Comment/uncomment/edit according to your needs.
 ##################################################################################################################*/

//########## OPTIONS ###############################################################################################
//INPUT PROTOCOL
// Choose only one.

//#define PROTOCOL_UAVTALK                      // OpenPilot / Taulabs protocol
//#define PROTOCOL_MSP                          // MSP from Multiwii 
#define PROTOCOL_MAVLINK                        // Mavlink for Ardupilot / Autoquad / PixHawk / Taulabs (UAVOmavlinkBridge)

/*  Not done yet
//#define PROTOCOL_NMEA                              // GPS NMEA 
//#define PROTOCOL_UBLOX                             // GPS UBLOX binary protocol
//#define PROTOCOL_MTK                               // GPS MTK binary protocol
*/

//INPUT BAUDRATE
 #define INPUT_BAUD 57600

// GhettoProxy just listen & convert in passive mode. If disabled, it will initiate queries packet.
 #define PASSIVEMODE 1

//OUTOPUT BAUDRATE
#define OUTPUT_BAUD 2400



