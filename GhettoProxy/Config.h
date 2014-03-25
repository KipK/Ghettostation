/*########################################### BOARD PINOUTS #########################################################
 Pins for Arduino Mini
 *SERIAL TELEMETRY INPUT PINOUT:
 
     Use TX/RX pads
     
 *SERIAL LTM OUTPUT PINOUT ( AltSoftSerial):
 
     TX: D9 on arduino mini/nano 

*/
//#define SOFTSERIAL_TX 9      //Digital pin used by SoftSerial for sending data to ground.

//#define SOFTSERIAL_RX 8      //Digital pin used by SoftSerial for receiving data from ground ( unused yet )
/*############################################## CONFIGURATION ####################################################
 # Comment/uncomment/edit according to your needs.
 ##################################################################################################################*/

//########## OPTIONS ###############################################################################################
//INPUT PROTOCOL
// Choose only one.

//#define PROTOCOL_UAVTALK                      // OpenPilot / Taulabs protocol
//#define PROTOCOL_MSP                          // MSP from Multiwii 
#define PROTOCOL_MAVLINK                        // Mavlink for Ardupilot / Autoquad / PixHawk / Taulabs (UAVOmavlinkBridge)

// Telemetry from GPS only ( NMEA / UBLOX / MTK ).
// Uncomment appropriate #include in GhettoProxy.ino
//#define PROTOCOL_GPS

//!uncomment to use altitude from Baro. Use GPs alt if commented.
#define BARO_ALT

//INPUT BAUDRATE
 #define INPUT_BAUD 57600

// GhettoProxy just listen & convert in passive mode. If disabled, it will initiate queries packet.
 #define PASSIVEMODE 1

//OUTOPUT BAUDRATE
#define OUTPUT_BAUD 2400


//#define DEBUG
