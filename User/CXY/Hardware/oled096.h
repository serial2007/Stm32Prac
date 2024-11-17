#ifndef OLED096_H
	#define OLED096_H
#include "base.h"
#include "easyI2C.h"
#include "OLED_Font.h"

class OLED096_I2C {
public:
	OLED096_I2C();
	easyI2C I2C;
	void I2C_default(void);
	void init(void);
  void I2C_WriteCommand(u8 Command);
	void I2C_WriteData(u8 Command);
	u8 slaveAddress;

	void setCursor(u8 Y, u8 X);
	void clear(void);
	void showChar(u8 line, u8 col, char Char);
  void showString(uint8_t Line, uint8_t Column, char *String);
  void showNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
  void showSignedNum(uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length);
	void showBinNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
	void showHexNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
};

#endif
