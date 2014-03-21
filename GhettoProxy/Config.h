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

//########################################### BOARD PINOUTS #########################################################
//DON'T EDIT THIS IF YOU DON'T KNOW WHAT YOU'RE DOINGG
 #define SOFTSERIAL_TX 5      //Digital pin used by SoftSerial for sending data to ground.
 #define SOFTSERIAL_RX 6      //Digital pin used by SoftSerial for receiving data from ground ( unused yet )


//###############################################END OF CONFIG#######################################################

