##############################################################################
#                   Ghettostation antenna tracker                            #
#                               README                                       #
##############################################################################

##############################################################################


All setup & usage instruction are now on the Wiki:
 https://github.com/KipK/Ghettostation/wiki

Project is separe in 3 different software:

 - Ghettostation: Main groundstation/antenna tracker software running on 
                  Teensy++2 or Arduino Mega. Reads data from UAVTalk, MAVLink,
                  Multiwii MSP, NMEA GPS, Ublox GPS. 
                  
 - GhettOSD:      Minimosd software for ground osd within the Ghettostation.
 
 - GhettoProxy:   Simple LTM (Lighttelemetry) telemetry converter running on
                  Arduino mini.
                  Takes UAVTalk, MAVlink, MSP, NMEA, Ublox protocols &
                  converts it to LTM. Allows using really low baudrate 
                  wireless serial links.
                  
 
Credits:

Author: Guillaume S (KipK)

Sample codes & inspirations from different projects:
- multiwii (https://github.com/multiwii/multiwii-firmware)
- minimosd-extra (http://code.google.com/p/minimosd-extra/)
- UAVTalk implementation from minoposd (http://code.google.com/p/minoposd/)
- NMEA & Ublox libraries from Jordi Muñoz and Jose Julio (DIYDrones.com)

 