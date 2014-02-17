##############################################################################
#                   Ghettostation antenna tracker                            #
#                               README                                       #
##############################################################################

##############################################################################
FEATURES:

- Multiple protocol compatible: MAVLINK, UAVTALK, MULTIWII MSP, LIGHTTELEMETRY
- 4 bank of settings stored in EEPROM
- Easy configuration trough LCD menu
- Manage any type of servo based Pan/Tilt setup: 180P/180T, 360P/90T, custom ones
- HMC5883L compass support (optional)



What you need:

- An Arduino MEGA 1280/2560 or a Teensy++2

- a serial wireless link between the tracker and the UAV ( Xbee, Oplink, RF433, FSK audio modems )

- optional: a bluetooth dongle to bridge telemetry from the tracker to your GCS.

- 2 servos (no unlimited rotation servo)

- a 5V ubec with enough amps for your servos

- 3 push buttons

- an LCD03/LCD05 I2C 20x4 lcd panel 
( http://www.hobbyking.com/hobbyking/store/__26852__IIC_I2C_TWI_Serial_2004_20x4_LCD_Module_Shield_For_Arduino_UNO_MEGA_R3.html )




  * *Build*

 - Follow the wiring scheme here:
   http://img839.imageshack.us/img839/2492/9jjr.png
   
   For Arduino Mega, follow the same scheme with those pins:
    PAN_SERVOPIN 3        
    TILT_SERVOPIN 4 
    LEFT_BUTTON_PIN 32 
    RIGHT_BUTTON_PIN 34 
    ENTER_BUTTON_PIN 36 
    Serial port for telemetry is uart1 so use R1/Tx1 pins ( 45/46 )

 - You can bridge telemetry with a bluetooth dongle if you want to use your groundstation software at the same time : 
   Wire bluetooth Tx to your serial module Rx, and BT rx to serial module Tx.


- you'll need an external 5/6v voltage source for the servos power. Teensy can't draw too much amp on it's own 5v line.





  * *Software*
- Teensy++2 users: install Teensyduino.
 Important,  you need to edit hardware\teensy\cores\teensy\new.cpp in your Arduino folder 
 and uncomment all the latest lines:

int __cxa_guard_acquire(__guard *g) {return !*(char *)(g);};
void __cxa_guard_release (__guard *g) {*(char *)g = 1;};
void __cxa_guard_abort (__guard *) {}; 

void __cxa_pure_virtual(void) {};

- edit config.h according to your needs.


Compile & start.


  * *Usage*

In the menu:
Enter = Select
Long press Enter ( >1sec ) = Go back.
LeftRight: Navigate.

 - At first launch, go to CONFIG menu, and set your servos endpoints & angle capabilities.

You will now you have good endpoints values when you don't ear your servo forcing. 

Then configure your servos angle limits. 90° as min& max angle for pan means you have a 180° pan config.

Once all those parameters are set, Ghettostation will know your hardware capabilities and adapt servo path to it's need.

It means if your pan servo is limited to 180° but your tilt can go almost 180° too, you can cover 360 by reversing your pan and tilt.
If you servo can go further, it'll just continue to it's own limits and then switch if needed.

If there's no path possible because your servo can go further, it will stop & wait you come back in a covered area.

- Go to SET HOME first & wait for telemetry data.

Wait for a 3D GPS fix then save home position. 
Once done you have to set the bearing.

There's now 4 different method for setting the bearing ( you have to choose the correct method on the config.h file )

Method 1: put your UAV 20m ahead, with the tracker pointing it, and click Enter again.

Method 2: manually enter bearing value from an external compass ( your smartphone one will do the job )

Method 3: just point the tracker to the North.

Method 4: Use internal HMC5883 compass reading   



Method 4 (auto): use HMC5883L compass ( experimental )

