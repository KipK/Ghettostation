#ifndef LCD_I2C_h
#define LCD_I2C_h

#include "Arduino.h"

#define INTERNAL_I2C_PULLUPS

#define I2C_PULLUPS_ENABLE         PORTC |= 1<<4; PORTC |= 1<<5;   // PIN A4&A5 (SDA&SCL)
#define I2C_PULLUPS_DISABLE        PORTC &= ~(1<<4); PORTC &= ~(1<<5);

#define I2C_SPEED 100000L


class LCD03_I2C
{
  public:
    LCD03_I2C(int address,int cols_max,int rows_max);
    void Send_Cmd(byte my_byte);
    void Send_Char(char my_char);
    void Set_Cursor(byte col, byte row);
    void PrintVal(uint8_t i);
    void PrintChar(int col, int row, const char *s);
	void PrintInt(int col, int row, int input_Int, int length);
	void PrintLong(int col, int row, long input_Long, int length);
	void PrintFloat(int col, int row, float input_Float, int length, int dec_points); 
    void Clear();
    void SetLine(byte line);
    void Backlight_On();
    void Backlight_Off();

  private:
  	int _address;
  	int _cols_max;
  	int _rows_max;
    void I2C_Init();
    void waitTransmissionI2C();
    //void i2c_stop(void);
    //uint8_t i2c_read(uint8_t ack);
    void i2c_rep_start(uint8_t I2Caddress);
    //size_t i2c_read_to_buf(uint8_t add, void *buf, size_t size);
    void i2c_write(uint8_t data);
    //size_t i2c_read_reg_to_buf(uint8_t add, uint8_t reg, void *buf, size_t size);
    //uint8_t i2c_readAck();
    //uint8_t i2c_readNak(void);
    //void swap_endianness(void *buf, size_t size);
    //void i2c_writeReg(uint8_t add, uint8_t reg, uint8_t val);
    //uint8_t i2c_readReg(uint8_t add, uint8_t reg);  	
};

#endif