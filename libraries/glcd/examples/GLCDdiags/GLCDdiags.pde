/*
 * GLCDdiags
 *
 * This sketch tests the memory and interface to the GLCD module as well as report
 * the current GLCD library configuration to the serial port.
 *
 * The serial port is configured to 9600 baud.
 *
 * It also displays a set of visual screens on the GLCD that can aid in diagnosing
 * mis-configured/wired chip select lines.
 *
 * The memory associated with each chip is tested seperately.
 * Tests are performed starting on chip #0.
 * The GLCD goes through a series of visual displays as the memory is tested.
 * The chip # under test as well as the x coordinate values are displayed: 
 * if everthing is working and configured properly, chip #0 will be on the left
 * and each increasing chip # will advance to the right.
 *
 * Status and error information is sent to the serial port while testing.
 *
 * The sketch peforms a few different memory tests but the main tests walk an incrementing pattern
 * through memory horizontally by incrementing through pages column at a time (left to right)
 * as well as vertically by incrementing thorough pages a page at time (top to bottom).
 * 
 * NOTE:
 * This sketch is a diagnostic tool, it is not an example of library usage.
 * It uses internal GLCD library information that is not needed and should not normally used by sketches.
 * Sketches that use these undocumented capabilities may not work correctly in future releases
 *   
 */


#include <glcd.h>
#include "glcd_Buildinfo.h"
#include "include/glcd_io.h"
#include "include/glcd_errno.h"
#include "fonts/SystemFont5x7.h"       // system font

/*
 * Macros to convert chip#s to upper and lower pixel coordinates.
 * x1,y1 is upper left pixel coordinate and x2,y2 is lower left right coordinate.
 */

#define chip2x1(chip) ((chip * CHIP_WIDTH) % DISPLAY_WIDTH)
#define chip2y1(chip) (((chip * CHIP_WIDTH)/DISPLAY_WIDTH) * CHIP_HEIGHT)
#define chip2x2(chip) ((chip2x1(chip) + CHIP_WIDTH) >= DISPLAY_WIDTH ? DISPLAY_WIDTH-1 : chip2x1(chip) + CHIP_WIDTH-1)
#define chip2y2(chip) ((chip2y1(chip) + CHIP_HEIGHT) >= DISPLAY_HEIGHT ? DISPLAY_HEIGHT-1 : chip2y1(chip) + CHIP_HEIGHT-1)

#include <avr/pgmspace.h>
#define P(name)   static const prog_char name[] PROGMEM   // declare a static string in AVR Progmem

#define MAX_ERRORS 10

#ifdef _AVRIO_AVRIO_
#define SerialPrintPINstr(x) \
  _SerialPrintPINstr(x, AVRIO_PIN2AVRPORT(AVRIO_PIN2AVRPIN(x)), AVRIO_PIN2AVRBIT(AVRIO_PIN2AVRPIN(x)))
#else
#define SerialPrintPINstr(x) _SerialPrintPINStr(x)
#endif

/*
 * declare a string for a horizontal line in program memory
 */
P(hline) =  "--------------------------------------------------------------------\n";


#define xstr(s) str(s)
#define str(...) #__VA_ARGS__

/*
 * Function to print a simple Quoted string to serial port.
 * The string is automagically forced to live in AVR flash/program space.
 */

#define SerialPrintQ(str) SerialPrintP(PSTR(str))

/*
 * print Progmem string to the serial port
 * (have to insert carriage returns as serial port runs in 'raw' mode)
 *
 */

void SerialPrintP(const prog_char * str )
{
  char c;
  const prog_char *p = str;

  while ((c = pgm_read_byte(p++)))
  {
    if(c == '\n')
      Serial.print('\r');
    Serial.print(c);   
  }
}

#ifdef SERIALPRINTF

/*
 * Define a REAL printf since Arduino doesn't have one
 *
 * SerialPrintf() will automatically put the format string in AVR program space
 * 
 */

#define SerialPrintf(fmt, ...) _SerialPrintf(PSTR(fmt), ##__VA_ARGS__)

extern "C" {
  int serialputc(char c, FILE *fp)
  { 
      if(c == '\n')
        Serial.write('\r'); 
    Serial.write(c); 
  }
}


void _SerialPrintf(const char *fmt, ...)
{
FILE stdiostr;
va_list ap;

  fdev_setup_stream(&stdiostr, serialputc, NULL, _FDEV_SETUP_WRITE);

  va_start(ap, fmt);
  vfprintf_P(&stdiostr, fmt, ap);
  va_end(ap);
}

/*
 * Define an eprintf() function for error output
 * (map it to the SerialPrintf() defined above)
 */
#define eprintf(...) SerialPrintf(__VA_ARGS__)

#endif //SERIALPRINTF


/*
 * GlcdPrintf() will automatically put the format string in AVR program space
 */
#define GlcdPrintf(fmt, ...) GLCD.Printf_P(PSTR(fmt), ##__VA_ARGS__)


void setup()
{
  Serial.begin(9600);

#ifdef CORE_TEENSY
  delay(2000);    // allow USB time to come up.
                  // plus give user time to start serial monitor
                  // NOTE: for Teensy users:
                  //       Watch for the serial monitor icon in the IDE
                  //       to briefly "flash". When it does, USB is up and the IDE
                  //       has noticed the Teensy board. You can then click on the icon
                  //       to connect the teensy board virtual com port.
#endif

  delay(5);    // allow the hardware time settle
  SerialPrintQ("Serial initialized\n");
}


/*
 * Attempt to show some graphical information on
 * the display that will easily visually demonstrate
 * whether or not the chip select lines are properly
 * connected.
 */
void showchipselscreen(void)
{
  /*
   * draw a trangle
   */
  for(int x = 0; x < GLCD.Width; x++)
  {
     GLCD.DrawVLine( x, 0, map(x, 0, GLCD.Right, 0, GLCD.Bottom));
     delay(50); // delay is to allow seeing if chip selects are duplicated or overlapping
  }   
  delay(4000);
  // show chips
  GLCD.ClearScreen();
  for(int chip = 0; chip < glcd_CHIP_COUNT; chip++)
  {
    // delay and flash is to allow seeing duplicate or overlapping chip selects
    for(uint8_t flash = 0; flash < 4; flash++)
    {
      GLCD.CursorToXY(chip2x1(chip), chip2y1(chip));
      if(flash & 1)
        GLCD.SetFontColor(BLACK);
      else
        GLCD.SetFontColor(WHITE);
      GLCD.print("Chip:");
      GLCD.print(chip);
      delay(350); 
    }
  }

  delay(5000);

  /*
   * show Version info and Sequential ascii characters 
   */
  GLCD.ClearScreen();
  GLCD.CursorTo(0,0);
  GLCD.print("GLCD   ver ");
  GLCD.println(GLCD_VERSION, DEC); 
  GLCD.print("gText  ver ");
  GLCD.println(GTEXT_VERSION, DEC); 
  GLCD.print("Device ver ");
  GLCD.println(GLCD_Device, DEC); // no newline to prevent erase EOL
  for(int i=0; i  < GLCD.Width / GLCD.CharWidth(' '); i++ )
  {
     GLCD.print(char('A' + i)); // show the ascii character
  }
  delay(5000);
}

void  loop()
{   // run over and over again

int lcount = 1;
unsigned int glcdspeed, kops, kops_fract;
int status;

  while(1)
  {
    /*
     * Dump GLCD config information *before* trying to talk to the GLCD
     * in case there is a problem talking to the GLCD.
     * This way ensures the GLCD information is always available on the serial port.
     */

    /*
     * dump the GLCD library configuration information to
     * the serial port.
     */
    showGLCDconfig();


    SerialPrintP(hline);
    SerialPrintQ("Diag Loop: ");
    Serial.println(lcount);

    SerialPrintQ("Initializing GLCD\n");
    status = GLCD.Init();   // initialise the library, non inverted writes pixels onto a clear screen

#ifndef GLCD_NOINIT_CHECKS
    if(status) // did the initialization fail?
    {
	SerialPrintQ("GLCD initialization Failed: ");
	switch(status)
	{
		case GLCD_EBUSY:
			SerialPrintQ("BUSY wait Timeout");
			break;
		case GLCD_ERESET:
			SerialPrintQ("RESET wait Timeout");
			break;
	}
	SerialPrintQ(" (status code: ");
	Serial.print(status);
	Serial.println(')');
    	goto finished;
    }
#endif


    GLCD.SelectFont(System5x7, BLACK);


    SerialPrintQ("Displaying ChipSelect Screens\n");
    showchipselscreen();
    if( lcdmemtest())
    {
      /*
       * memory tests failed.
       */
      SerialPrintQ("TEST FAILED\n");
    }
    else
    {
      SerialPrintQ("Tests PASSED\n");

      /*
       * Diags report loop count on completion
       */
      GLCD.ClearScreen();
      GLCD.CursorTo(0,0);
      GLCD.print("Diag Loop: ");
      GLCD.println(lcount);
      GLCD.println("Tests PASSED");

      /*
       * All GLCD tests passed so now
       * perform a GLCD "speed" test.
       */

      glcdspeed = getglcdspeed();
      /*
       * Calculate the speed in K operations/sec
       * Since the speed value reported is 10x the actual value,
       * Dividing by 100 gives the integer K ops/sec
       * Modulo 100 gives the factional K ops/sec
       */

      kops = glcdspeed/100;
      kops_fract = glcdspeed %100;

      GLCD.print("K SetDot/s: ");
      GLCD.print(kops);
      GLCD.print(".");
      GLCD.println(kops_fract);


      SerialPrintQ("GLCD.SetDot() speed (K ops/sec): ");
      Serial.print(kops);
      SerialPrintQ(".");
      Serial.println(kops_fract);
    }

finished:

    delay(5000);
    lcount++;
  }
}


uint8_t lcdmemtest(void)
{
  uint8_t errors = 0;

  SerialPrintQ("Walking 1s data test\n");

  errors = lcdw1test();
  if(errors)
    return(errors);

  SerialPrintQ("Wr/Rd Chip Select Test\n");

  errors = lcdrwseltest();
  if(errors)
    return(errors);

  GLCD.ClearScreen();

  SerialPrintQ("Testing GLCD memory pages\n");

  for(uint8_t chip = 0; chip < glcd_CHIP_COUNT; chip++)
  {
    uint8_t col = chip2x1(chip);
    uint8_t ecol = chip2x2(chip);

    if(col >= CHIP_WIDTH)
      GLCD.CursorToXY(0,chip2y1(chip));
    else
      GLCD.CursorToXY(CHIP_WIDTH,chip2y1(chip));
    GLCD.print("Chip:");
    GLCD.print((int)chip);

    /*
     * Assumes font is 8 pixels high
     */
    if(col >= CHIP_WIDTH)
      GLCD.CursorToXY(0,chip2y1(chip)+8);
    else
      GLCD.CursorToXY(CHIP_WIDTH,chip2y1(chip)+8);
    GLCD.print((int)col);
    GLCD.print('-');
    GLCD.print((int)ecol);
    delay(500);

//  SerialPrintf("Horizontal Page Test Chip: %d Pixels %d-%d\n", chip, col, ecol);

    SerialPrintQ("Horizontal Page Test Chip: ");
    Serial.print((int)chip);
    SerialPrintQ(" Pixels ");
    Serial.print((int)col);
    Serial.print('-');
    Serial.println((unsigned int)ecol);

    errors += lcdhpagetest(col, ecol, chip2y1(chip)/8, (chip2y2(chip)+1)/8 - 1, 0, 255);


//  SerialPrintf("Vertical Page Test Chip: %d Pixels %d-%d\n", chip, col, ecol);

    SerialPrintQ("Vertical Page Test Chip: ");
    Serial.print((int)chip);
    SerialPrintQ(" Pixels ");
    Serial.print((int)col);
    Serial.print('-');
    Serial.println((int)ecol);

    errors += lcdvpagetest(col, ecol, chip2y1(chip)/8, (chip2y2(chip)+1)/8 - 1, 0, 255);
    GLCD.ClearScreen();

    col += CHIP_WIDTH;
    ecol += CHIP_WIDTH;
    if(ecol > GLCD.Right)
      ecol = GLCD.Right;
  }


  GLCD.CursorTo(0,0);
  GLCD.print("Full Display");
  GLCD.CursorTo(0,1);
  GLCD.print((int)0);
  GLCD.print('-');
  GLCD.print((int)GLCD.Right);
  delay(1000);

//SerialPrintf("Full Module Horizontal Page Test:Pixels %d-%d\n",  0, GLCD.Right);

  SerialPrintQ("Full Module Horizontal Page Test:Pixels ");
  Serial.print(0);
  Serial.print('-');
  Serial.println((int)GLCD.Right);

  errors += lcdhpagetest(0, GLCD.Right, 0, GLCD.Bottom/8, 0, 255);

//SerialPrintf("Full Module Vertical Page Test:Pixels %d-%d\n",  0, GLCD.Right);

  SerialPrintQ("Full Module Vertical Page Test:Pixels ");
  Serial.print(0);
  Serial.print('-');
  Serial.println((int)GLCD.Right);

  errors += lcdvpagetest(0, GLCD.Right, 0, GLCD.Bottom/8, 0, 255);

  GLCD.ClearScreen();

  return(errors);
}

/*
 * Walk a bit through a single memory location to see if
 * basic reads/writes work.
 */

uint8_t
lcdw1test(void)
{
  uint8_t errors = 0;
  uint8_t rdata;

  for(uint8_t pat = 1;  pat != 0; pat <<= 1)
  {
    GLCD.GotoXY(0,0);
    GLCD.WriteData(pat);
    GLCD.GotoXY(0,0);
    rdata = GLCD.ReadData();

    if(rdata != pat)
    {
//    eprintf(" Compare error: %x != %x\n", rdata, pat);
      SerialPrintQ(" Compare error: ");
      Serial.print((unsigned int)rdata, HEX);
      SerialPrintQ(" != ");
      Serial.println((unsigned int)pat, HEX);

      errors++;
    }
  }
  return(errors);
}

/*
 * LCD read/write chip select test.
 * This test attempts to detect chip select issues by writing the chip number
 * to the lowest page of memory for each chip.
 * This is done incrementing and decrementing.
 * It must be done both ways because when chip selects are wrong, it is possible
 * to write to more than 1 chip at a time. To catch this, you have write do the operation
 * more tha once. Once by writing incrementing addresses and then decrementing addresses.
 */

uint8_t
lcdrwseltest()
{
  uint8_t errors = 0;
  uint8_t rdata; // read data


  for(uint8_t chip = 0; chip < glcd_CHIP_COUNT; chip++)
  {
    GLCD.GotoXY(chip2x1(chip), chip2y1(chip));
    GLCD.WriteData(chip);
  }
  for(uint8_t chip = 0; chip < glcd_CHIP_COUNT; chip++)
  {
    GLCD.GotoXY(chip2x1(chip), chip2y1(chip));
    rdata = GLCD.ReadData();
    if(rdata != chip)
    {
//    eprintf(" Compare error: chip:%d %x != %x\n", chip, rdata, chip);
      SerialPrintQ(" Compare error: chip:");
      Serial.print((int)chip);
      Serial.print(' ');
      Serial.print((unsigned int)rdata, HEX);
      SerialPrintQ(" != ");
      Serial.println((unsigned int)chip, HEX);
      errors++;
    }
  }

  for(int chip = glcd_CHIP_COUNT - 1; chip >= 0; chip--)
  {
    GLCD.GotoXY(chip2x1(chip), chip2y1(chip));
    GLCD.WriteData(chip);
  }
  for(int chip = glcd_CHIP_COUNT - 1; chip >= 0; chip--)
  {
    GLCD.GotoXY(chip2x1(chip), chip2y1(chip));
    rdata = GLCD.ReadData();
    if(rdata != chip)
    {
//    eprintf(" Compare error: chip:%d  %x != %x\n", chip, rdata, chip);
      SerialPrintQ(" Compare error: chip:");
      Serial.print((int)chip);
      Serial.print(' ');
      Serial.print((unsigned int)rdata, HEX);
      SerialPrintQ(" != ");
      Serial.println((unsigned int)chip, HEX);
      errors++;
    }
  }

  return(errors);
}


/*
 * Walk incrementing values through incrementing memory locations.
 * 
 * A value starting at sval ending at eval will be walked through memory
 * pages horizontally.
 * The starting x location will be filled in with sval and the value will
 * incremented through all locations to be tested. Values are written through
 * incrementing x values and when the maximum x value is reached on a row/page,
 * writing is continued down on the next row/page.
 *
 * All the values are read and compared to expected values.
 *
 * Then process starts over again by incrementing the starting value.
 * This repeats until the starting value reaches the ending value.
 *
 * Each memory location will tested with an incrementing value evel-sval+1 times.
 *
 * If sval is 0 and eval is 255, every memory location will be tested for every value.
 *
 */


int lcdhpagetest(uint8_t x1, uint8_t x2, uint8_t spage, uint8_t epage, uint8_t sval, uint8_t eval)
{
  uint8_t x;
  uint8_t data;
  uint8_t rdata;
  uint8_t page;
  uint8_t errors = 0;

  /*
   * perform each interation of test across memory with an incrementing pattern
   * starting at sval and bumping sval each iteration.
   */
  do
  {
    /*
     * write out all glcd memory pages
     * sequentially through incrementing columns (x values)
     */

    data = sval;
    for(page = spage; page <= epage; page++)
    {

      GLCD.GotoXY(x1, page * 8);
      for(x = x1; x <= x2; x++)
      {
        /*
	 * GotoXY() is intentially not done here in the loop to 
         * let the hardware bump its internal address.
         * This ensures that the glcd code and hardware are
         * properly tracking each other.
         */
        GLCD.WriteData(data);
        data++;
      }
    }

    /*
     * Now go back and verify the pages
     */

    data = sval;
    for(page = spage; page <= epage; page++)
    {

      for(x = x1; x<= x2; x++)
      {
        /*
	 * Reads don't auto advance X
         */
        GLCD.GotoXY(x, page * 8);
        rdata = GLCD.ReadData();

        if(data != rdata)
        {
//        eprintf(" Verify error: (%d,%d) %x!=%x\n", x, spage*8, data, rdata);
          SerialPrintQ(" Verify error: (");
          Serial.print((unsigned int) x);
          Serial.print(',');
          Serial.print((unsigned int) (spage*8));
          SerialPrintQ(") ");
          Serial.print((unsigned int)data, HEX);
          SerialPrintQ("!=");
          Serial.println((unsigned int)rdata, HEX);

          if(++errors > MAX_ERRORS)
            return(errors);
        }
        data++;
      }
    }
  } 
  while(sval++ != eval);
  return(0);
}

/*
 * Walk incrementing values through vertical memory page locations.
 * 
 * A value starting at sval ending at eval will be walked through memory pages
 * Vertically.
 * The starting x location will be filled in with sval and the value will
 * incremented through all memory pages to be tested. Values are written through
 * incrementing row/page values and when the maximum row/page value is reached,
 * writing is continued at the top page of the next column/x location.
 *
 * All the values are read and compared to expected values.
 *
 * Then process starts over again by incrementing the starting value.
 * This repeats until the starting value reaches the ending value.
 *
 * Each memory location will tested with an incrementing value evel-sval+1 times.
 *
 * If sval is 0 and eval is 255, every memory location will be tested for every value.
 *
 */


int lcdvpagetest(uint8_t x1, uint8_t x2, uint8_t spage, uint8_t epage, uint8_t sval, uint8_t eval)
{
  uint8_t x;
  uint8_t data;
  uint8_t rdata;
  uint8_t page;
  uint8_t errors = 0;

  /*
   * perform each interation of test across memory with an incrementing pattern
   * starting at sval and bumping sval each iteration.
   */
  do
  {
    /*
     * write out all glcd memory pages
     * sequentially through incrementing columns (x values)
     */

    data = sval;
    for(x = x1; x <= x2; x++)
    {
      for(page = spage; page <= epage; page++)
      {
        GLCD.GotoXY(x, page * 8);
        GLCD.WriteData(data);
        data++;
      }
    }

    /*
     * Now go back and verify the pages
     */

    data = sval;
    for(x = x1; x<= x2; x++)
    {
      for(page = spage; page <= epage; page++)
      {
        GLCD.GotoXY(x, page * 8);
        rdata = GLCD.ReadData();

        if(data != rdata)
        {
//        eprintf(" Verify error: (%d,%d) %x!=%x\n", x, spage*8, data, rdata);

          SerialPrintQ(" Verify error: (");
          Serial.print((unsigned int) x);
          Serial.print(',');
          Serial.print((unsigned int) (spage*8));
          SerialPrintQ(") ");
          Serial.print((unsigned int)data, HEX);
          SerialPrintQ("!=");
          Serial.println((unsigned int)rdata, HEX);

          if(++errors > MAX_ERRORS)
            return(errors);
        }
        data++;
      }
    }
  } 
  while(sval++ != eval);
  return(0);
}

/*
 * Dump the GLCD configuration information out
 * the serial port.
 */

void showGLCDconfig(void)
{
#ifdef ARDUINO
  SerialPrintP(hline);
  SerialPrintQ("Reported Arduino Revision: ");
  Serial.print(ARDUINO/100);
  Serial.print('.');
  Serial.println(ARDUINO%100);
#endif
  SerialPrintP(hline);
  SerialPrintQ("GLCD Lib Configuration: glcd ver: ");
  Serial.print(GLCD_VERSION);
  SerialPrintQ(" glcd_Device ver: ");
  Serial.print(GLCD_Device);
  SerialPrintQ(" gText ver: ");
  Serial.println(GTEXT_VERSION);
#ifdef GLCD_GLCDLIB_DATESTR
  SerialPrintQ("GLCD Lib build date: ");
  SerialPrintQ(GLCD_GLCDLIB_DATESTR);
  Serial.println();
#endif
#ifdef GLCD_GLCDLIB_BUILDSTR
  SerialPrintQ("GLCD Lib build number: ");
  SerialPrintQ(GLCD_GLCDLIB_BUILDSTR);
  Serial.println();
#endif

/*
 * ifdef to support manual config files vs auto config files 
 */

#ifdef glcd_ConfigName
  SerialPrintQ("Config File:");
  SerialPrintQ(glcd_ConfigName);
#else
  SerialPrintQ("Panel Configuration:");
  SerialPrintQ(glcd_PanelConfigName);
  Serial.println();
  SerialPrintQ("Pin Configuration:");
  SerialPrintQ(glcd_PinConfigName);
#endif
  Serial.println();
  SerialPrintP(hline);

  SerialPrintQ("GLCD:");
  SerialPrintQ(glcd_DeviceName);

//SerialPrintf("DisplayWidth:%d DisplayHeight:%d\n", GLCD.Width, GLCD.Height);
  SerialPrintQ(" DisplayWidth:");
  Serial.print((int)GLCD.Width);
  SerialPrintQ(" DisplayHeight:");
  Serial.println((int)GLCD.Height);

//SerialPrintf("Chips:%d", glcd_CHIP_COUNT);
  SerialPrintQ("Chips:");
  Serial.print(glcd_CHIP_COUNT);


//SerialPrintf(" ChipWidth:%3d ChipHeight:%2d\n", CHIP_WIDTH, CHIP_HEIGHT);
  SerialPrintQ(" ChipWidth:");
  Serial.print(CHIP_WIDTH);
  SerialPrintQ(" ChipHeight:");
  Serial.println(CHIP_HEIGHT);

#ifdef glcdCSEL1
  SerialPrintQ(" CSEL1:");
  SerialPrintPINstr(glcdCSEL1);
#endif
#ifdef glcdCSEL2
  SerialPrintQ(" CSEL2:");
  SerialPrintPINstr(glcdCSEL2);
#endif
#ifdef glcdCSEL3
  SerialPrintQ(" CSEL3:");
  SerialPrintPINstr(glcdCSEL3);
#endif
#ifdef glcdCSEL4
  SerialPrintQ(" CSEL4:");
  SerialPrintPINstr(glcdCSEL4);
#endif

#if defined(glcdCSEL1) || defined(glcdCSEL2) || defined(glcdCSEL3) || defined(glcdCSEL4)
  Serial.println();
#endif


#ifdef glcdRES
  SerialPrintQ(" RES:");
  SerialPrintPINstr(glcdRES);
#endif
  SerialPrintQ(" RW:");
  SerialPrintPINstr(glcdRW);

  SerialPrintQ(" DI:");
  SerialPrintPINstr(glcdDI);

#ifdef glcdEN
  SerialPrintQ(" EN:");
  SerialPrintPINstr(glcdEN);
#endif

#ifdef glcdE1
  SerialPrintQ(" E1:");
  SerialPrintPINstr(glcdE1);
#endif
#ifdef glcdE2
  SerialPrintQ(" E2:");
  SerialPrintPINstr(glcdE2);
#endif

  Serial.println();

//  SerialPrintf(" D0:%s", GLCDdiagsPIN2STR(glcdData0Pin));
  SerialPrintQ(" D0:");
  SerialPrintPINstr(glcdData0Pin);

  SerialPrintQ(" D1:");
  SerialPrintPINstr(glcdData1Pin);

  SerialPrintQ(" D2:");
  SerialPrintPINstr(glcdData2Pin);

  SerialPrintQ(" D3:");
  SerialPrintPINstr(glcdData3Pin);

  Serial.println();

  SerialPrintQ(" D4:");
  SerialPrintPINstr(glcdData4Pin);

  SerialPrintQ(" D5:");
  SerialPrintPINstr(glcdData5Pin);

  SerialPrintQ(" D6:");
  SerialPrintPINstr(glcdData6Pin);

  SerialPrintQ(" D7:");
  SerialPrintPINstr(glcdData7Pin);

  Serial.println();

//  SerialPrintf("Delays: tDDR:%d tAS:%d tDSW:%d tWH:%d tWL: %d\n",
//  GLCD_tDDR, GLCD_tAS, GLCD_tDSW, GLCD_tWH, GLCD_tWL);

  SerialPrintQ("Delays: tDDR:");
  Serial.print(GLCD_tDDR);
  SerialPrintQ(" tAS:");
  Serial.print(GLCD_tAS);
  SerialPrintQ(" tDSW:");
  Serial.print(GLCD_tDSW);
  SerialPrintQ(" tWH:");
  Serial.print(GLCD_tWH);
  SerialPrintQ(" tWL:");
  Serial.println(GLCD_tWL);


#ifdef glcd_CHIP0
  SerialPrintQ("ChipSelects:");
  SerialPrintQ(" CHIP0:(");
  SerialPrintQ(xstr(glcd_CHIP0));
  SerialPrintQ(")");
#endif
#ifdef glcd_CHIP1
  SerialPrintQ(" CHIP1:(");
  SerialPrintQ(xstr(glcd_CHIP1));
  SerialPrintQ(")");
#endif
#ifdef glcd_CHIP2
  SerialPrintQ(" CHIP2:(");
  SerialPrintQ(xstr(glcd_CHIP2));
  SerialPrintQ(")");
#endif
#ifdef glcd_CHIP3
  SerialPrintQ(" CHIP3:(");
  SerialPrintQ(xstr(glcd_CHIP3));
  SerialPrintQ(")");
#endif

#ifdef glcd_CHIP0
  Serial.println();
#endif



#ifdef _AVRIO_AVRIO_
  /*
   * Show AVRIO GLCD data mode
   *
   * Requires getting down and dirty and mucking around done
   * in avrio land.
   */

  SerialPrintQ("Data mode: ");
  /*
   * First check for full 8 bit mode
   *
   */
  if(AVRDATA_8BIT(glcdData0Pin, glcdData1Pin, glcdData2Pin, glcdData3Pin,
  glcdData4Pin, glcdData5Pin, glcdData6Pin, glcdData7Pin))
  {
    /*
     * full 8 bit mode
     */
    SerialPrintQ("byte\n");
  }
  else
  {
    SerialPrintQ("\n d0-d3:");
    if(AVRDATA_4BITHI(glcdData0Pin, glcdData1Pin, glcdData2Pin, glcdData3Pin) ||
      AVRDATA_4BITLO(glcdData0Pin, glcdData1Pin, glcdData2Pin, glcdData3Pin))
    {
      SerialPrintQ("nibble mode");
#ifndef GLCD_ATOMIC_IO
      SerialPrintQ("-Non-Atomic");
#else
      SerialPrintQ("-disabled"); // for now this "knows" avrio disabled nibbles when in atomic mode.
#endif
    }
    else
    {
      SerialPrintQ("bit i/o");
    }

    SerialPrintQ("\n d4-d7:");

    if(AVRDATA_4BITHI(glcdData4Pin, glcdData5Pin, glcdData6Pin, glcdData7Pin) ||
      AVRDATA_4BITLO(glcdData4Pin, glcdData5Pin, glcdData6Pin, glcdData7Pin))
    {
      SerialPrintQ("nibble mode");
#ifndef GLCD_ATOMIC_IO
      SerialPrintQ("-Non-Atomic");
#else
      SerialPrintQ("-disabled"); // for now this "knows" avrio disabled nibbles when in atomic mode.
#endif
    }
    else
    {
      SerialPrintQ("bit i/o");
    }
    Serial.println();
  }

#endif // _AVRIO_AVRIO_

  /*
   * Show font rendering:
   */

#ifdef GLCD_OLD_FONTDRAW
  SerialPrintQ("Text Render: ");
  SerialPrintQ("OLD\n");
#endif

  /*
   * show no scroll down if disabled.
   */

#ifdef GLCD_NO_SCROLLDOWN
  SerialPrintQ("NO Down Scroll\n");
#endif

  /*
   * show READ CACHE if enabled
   */
#ifdef GLCD_READ_CACHE
  SerialPrintQ("READ CACHE enabled\n");
#endif


}

#ifdef _AVRIO_AVRIO_
/*
 * The avrio version of the pin string also contain
 * the AVR port and bit number of the pin.
 * The format is PIN_Pb where P is the port A-Z 
 * and b is the bit number within the port 0-7
 */
void
_SerialPrintPINstr(uint8_t pin, uint8_t avrport, uint8_t avrbit)
{

  /*
   * Check to see if Ardino pin# is used or
   * if AVRPIN #s are used.
   */
  if(pin >= AVRIO_PIN(AVRIO_PORTA, 0))
  {
    
//  SerialPrintf("0x%x", pin);
    /*
     * print pin value in hex when AVRPIN #s are used
     */
    SerialPrintQ("0x");
    Serial.print(pin,HEX);
  }
  else
  {
//  SerialPrintf("%d", pin);
    Serial.print(pin,DEC);
  }

//SerialPrintf("(PIN_%c%d)", pin, 'A'-AVRIO_PORTA+avrport, avrbit);

  SerialPrintQ("(PIN_");
  Serial.print((char)('A' - AVRIO_PORTA+avrport));
  Serial.print((int)avrbit);
  Serial.print(')');

}
#else
void
_SerialPrintPINstr(uint16_t pin)
{
  Serial.print((int) pin);
}
#endif


/*
 * This function returns a composite "speed" of the glcd
 * by returning the SetDot() speed in 1/10 operations/sec.
 * i.e. return value is 1/10 the number of SetDot() calls
 * per second.
 */
uint16_t
getglcdspeed()
{
uint16_t iter = 0;
unsigned long startmillis;

  startmillis = millis();

  while(millis() - startmillis < 1000) // loop for 1 second
  {
    /*
     * Do 10 operations to minimize the effects of the millis() call
     * and the loop.
     *
     * Note: The pixel locations were chosen to ensure that a
     * a set colum and set page operation are needed for each SetDot()
     * call.
     * The intent is to get an overall feel for the speed of the GLD
     * as each SetDot() call will do these operations to the glcd:
     * - set page
     * - set column
     * - read byte (dummy read)
     * - read byte (real read)
     * - set column (set column back for write)
     * - write byte
     */

    GLCD.SetDot(GLCD.Right, GLCD.Bottom, WHITE);
    GLCD.SetDot(GLCD.Right-1, GLCD.Bottom-9, WHITE);
    GLCD.SetDot(GLCD.Right, GLCD.Bottom, WHITE);
    GLCD.SetDot(GLCD.Right-1, GLCD.Bottom-9, WHITE);
    GLCD.SetDot(GLCD.Right, GLCD.Bottom, WHITE);
    GLCD.SetDot(GLCD.Right-1, GLCD.Bottom-9, WHITE);
    GLCD.SetDot(GLCD.Right, GLCD.Bottom, WHITE);
    GLCD.SetDot(GLCD.Right-1, GLCD.Bottom-9, WHITE);
    GLCD.SetDot(GLCD.Right, GLCD.Bottom, WHITE);
    GLCD.SetDot(GLCD.Right-1, GLCD.Bottom-9, WHITE);
    iter++;
  }

  return(iter);

}
