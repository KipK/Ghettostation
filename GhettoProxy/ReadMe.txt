##############################################################################
#                             GhettoProxy                                    #
#                               README                                       #
##############################################################################

Ghettoproxy converts Mavlink, MSP, UAVTalk telemetry to LightTelemetry.
Lighttelemetry brings enough info to the ground at really low baudrates
(1200 / 2400 bauds ). 
This is made to be used with Ghettostation antenna tracker & ground osd.

Hardware: Any Atmega328P arduino. Arduino mini recommended

Telemetry Input: UART Tx/Rx serial port 
LTM Output: pin 5 ( tx only )

##############################################################################