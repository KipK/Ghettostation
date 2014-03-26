##############################################################################
#                             GhettoProxy                                    #
#                               README                                       #
##############################################################################

Ghettoproxy converts many telemetry protocols to LTM (Light Telemetry). 
LTM fits low baudrates like 1200/2400 bauds so this can be used to send data
to GhettoStation over long range transmission like OpenLRSng transparent 
serial link, or FSK modem trough fpv Video TX Audio channel.

Input Protocols:

    MAVLink
    UAVTalk
    Multiwii MSP
    GPS NMEA (ascii)
    GPS UBlox (binary)
    GPS MTK (binary)

This is made to be used with Ghettostation antenna tracker & ground osd.

Hardware: Any Atmega328P arduino. Arduino mini recommended

Telemetry Input: UART Tx/Rx serial port 
LTM Output: pin D9 ( tx only )


##############################################################################