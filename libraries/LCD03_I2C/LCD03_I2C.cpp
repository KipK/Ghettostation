#include "Arduino.h"
#include "LCD03_I2C.h"

LCD03_I2C::LCD03_I2C(int address, int cols_max, int rows_max)
{
	I2C_Init();
	_address = address;
	_cols_max = cols_max;
	_rows_max = rows_max;
  i2c_rep_start(_address<<1); 
  i2c_write(0x00);// Command register
  i2c_write(04);// Hide cursor
  i2c_write(12);// Clear screen
  i2c_write(19);// Backlight on
}

void LCD03_I2C::Send_Cmd(byte my_byte)
{ 
  i2c_rep_start(_address<<1);
  i2c_write(0x00);
  i2c_write(my_byte);
}

void LCD03_I2C::Send_Char(char my_char)
{ 
  i2c_rep_start(_address<<1);
  i2c_write(0x00);
  i2c_write(my_char);
}

void LCD03_I2C::Set_Cursor(byte col, byte row) 
{
  row = min(row,_rows_max-1);
  col = min(col,_cols_max-1);
  Send_Cmd(03); // set cursor (row, column)
  Send_Cmd(row+1);
  Send_Cmd(col+1);
}

void LCD03_I2C::PrintVal(uint8_t i) 
{
  Send_Char(i);
}

void LCD03_I2C::PrintChar(int col, int row, const char *s) 
{
  Set_Cursor(col-1, row-1);
  while (*s) {
    PrintVal(*s++);
  }
}


void LCD03_I2C::PrintInt(int col, int row, int input_Int, int length) 
{
  char s[20];
  itoa(input_Int, s, length);
  PrintChar(col, row, s);
}

void LCD03_I2C::PrintLong(int col, int row, long input_Long, int length) 
{
  char s[20];
  ltoa(input_Long, s, length);
  PrintChar(col, row, s);
}

void LCD03_I2C::PrintFloat(int col, int row, float input_Float, int length, int dec_points) 
{
  char s[20];
  dtostrf(input_Float, length, dec_points, s);
  PrintChar(col, row, s);
}

void LCD03_I2C::Clear() 
{
  Send_Cmd(12); // clear screen
}

void LCD03_I2C::SetLine(byte line) 
{ 
  Set_Cursor(0,line-1);
}

void LCD03_I2C::Backlight_On() 
{
  i2c_rep_start(_address<<1); 
  i2c_write(0x00);// Command register
  i2c_write(04);// Hide cursor
  i2c_write(19);// Backlight on
}

void LCD03_I2C::Backlight_Off() 
{
  i2c_rep_start(_address<<1);
  i2c_write(0x00);// Command register
  i2c_write(04);// Hide cursor
  i2c_write(20);// Backlight off
}

//I2C related functions
void LCD03_I2C::I2C_Init() 
{
  #if defined(INTERNAL_I2C_PULLUPS)
    I2C_PULLUPS_ENABLE
  #else
    I2C_PULLUPS_DISABLE
  #endif
  TWSR = 0;                                // no prescaler => prescaler = 1
  TWBR = ((F_CPU / I2C_SPEED) - 16) / 2;   // change the I2C clock rate
  TWCR = 1<<TWEN;                          // enable twi module, no interrupt
}

void LCD03_I2C::waitTransmissionI2C() 
{
  uint16_t count = 255;
  while (!(TWCR & (1<<TWINT))) {
    count--;
    if (count==0) {              //we are in a blocking state => we don't insist
      TWCR = 0;                  //and we force a reset on TWINT register
      break;
    }
  }
}

void LCD03_I2C::i2c_rep_start(uint8_t I2Caddress) {
  TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN) ; // send REPEAT START condition
  waitTransmissionI2C();                       // wait until transmission completed
  TWDR = I2Caddress;                           // send device address
  TWCR = (1<<TWINT) | (1<<TWEN);
  waitTransmissionI2C();                       // wail until transmission completed
}

void LCD03_I2C::i2c_write(uint8_t data ) {	
  TWDR = data;                                 // send data to the previously addressed device
  TWCR = (1<<TWINT) | (1<<TWEN);
  waitTransmissionI2C();
}

