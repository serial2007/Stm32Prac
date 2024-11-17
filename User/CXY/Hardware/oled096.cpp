#include "oled096.h"



void OLED096_I2C::I2C_default(void) {
	this->I2C.SCL_channel = GPIOB;
	this->I2C.SCL_pin = GPIO_Pin_8;
	this->I2C.SDA_channel = GPIOB;
	this->I2C.SDA_pin = GPIO_Pin_9;
}

void OLED096_I2C::init(void) {
  for (int i = 0; i < 1000; i++) // 上电延时
    for (int j = 0; j < 1000; j++);
	this->I2C.Init();
				//端口初始化
	this->I2C.Write_SCL(1);
	this->I2C.Write_SDA(1);

	this->I2C_WriteCommand(0xAE);	//关闭显示
	this->I2C_WriteCommand(0xD5);	//设置显示时钟分频比/振荡器频率
	this->I2C_WriteCommand(0x80);
	this->I2C_WriteCommand(0xA8);	//设置多路复用率
	this->I2C_WriteCommand(0x3F);
	this->I2C_WriteCommand(0xD3);	//设置显示偏移
	this->I2C_WriteCommand(0x00);
	this->I2C_WriteCommand(0x40);	//设置显示开始行
	this->I2C_WriteCommand(0xA1);	//设置左右方向，0xA1正常 0xA0左右反置
	this->I2C_WriteCommand(0xC8);	//设置上下方向，0xC8正常 0xC0上下反置
	this->I2C_WriteCommand(0xDA);	//设置COM引脚硬件配置
	this->I2C_WriteCommand(0x12);
	this->I2C_WriteCommand(0x81);	//设置对比度控制
	this->I2C_WriteCommand(0xCF);
	this->I2C_WriteCommand(0xD9);	//设置预充电周期
	this->I2C_WriteCommand(0xF1);
	this->I2C_WriteCommand(0xDB);	//设置VCOMH取消选择级别
	this->I2C_WriteCommand(0x30);
	this->I2C_WriteCommand(0xA4);	//设置整个显示打开/关闭
	this->I2C_WriteCommand(0xA6);	//设置正常/倒转显示
	this->I2C_WriteCommand(0x8D);	//设置充电泵
	this->I2C_WriteCommand(0x14);
	this->I2C_WriteCommand(0xAF);	//开启显示
		
	this->clear();				//OLED清屏
}


/**
  * @brief  OLED写命令
  * @param  Command 要写入的命令
  */
void OLED096_I2C::I2C_WriteCommand(u8 Command) {
	this->I2C.Start();
	this->I2C.SendByte(slaveAddress); //从机地址
	this->I2C.Write_SCL(Bit_SET); // 等待回传
	this->I2C.Write_SCL(Bit_RESET);
	this->I2C.SendByte(0x00);
	this->I2C.Write_SCL(Bit_SET); // 等待回传
	this->I2C.Write_SCL(Bit_RESET);
	this->I2C.SendByte(Command);
	this->I2C.Write_SCL(Bit_SET); // 等待回传
	this->I2C.Write_SCL(Bit_RESET);
	this->I2C.Stop();
}

/**
  * @brief  OLED写数据
  * @param  Command 要写入的命令
  */
void OLED096_I2C::I2C_WriteData(u8 Command) {
	this->I2C.Start();
	this->I2C.SendByte(slaveAddress); //从机地址
	this->I2C.Write_SCL(Bit_SET); // 等待回传
	this->I2C.Write_SCL(Bit_RESET);
	this->I2C.SendByte(0x40);
	this->I2C.Write_SCL(Bit_SET); // 等待回传
	this->I2C.Write_SCL(Bit_RESET);
	this->I2C.SendByte(Command);
	this->I2C.Write_SCL(Bit_SET); // 等待回传
	this->I2C.Write_SCL(Bit_RESET);
	this->I2C.Stop();
}

void OLED096_I2C::setCursor(u8 Y, u8 X) {
	this->I2C_WriteCommand(0xB0 | Y);
	this->I2C_WriteCommand(0x10 | ((X & 0xF0) >> 4));
	this->I2C_WriteCommand(0x00 | (X & 0x0F));
}

/**
  * @brief  OLED清屏
  */
void OLED096_I2C::clear(void) {
	u8 i, j;
	for (j = 0; j < 8; j++)
	{
		this->setCursor(j, 0);
		for(i = 0; i < 128; i++)
		{
			this->I2C_WriteData(0x00);
		}
	}
}

/**
  * @brief  OLED显示一个字符
  * @param  line 行位置，范围：0-3
  * @param  col 列位置，范围：0-15
  * @param  Char 要显示的一个字符，范围：ASCII可见字符
  */
void OLED096_I2C::showChar(u8 line, u8 col, char Char) {
	uint8_t i;
	this->setCursor(line * 2, col * 8);		//设置光标位置在上半部分
	for (i = 0; i < 8; i++)
	{
		this->I2C_WriteData(OLED_F8x16[Char - ' '][i]);			//显示上半部分内容
	}
	this->setCursor(line * 2 + 1, col * 8);	//设置光标位置在下半部分
	for (i = 0; i < 8; i++)
	{
		this->I2C_WriteData(OLED_F8x16[Char - ' '][i + 8]);		//显示下半部分内容
	}
}

/**
  * @brief  OLED显示字符串
  * @param  Line 起始行位置，范围：0-3
  * @param  Column 起始列位置，范围：0-15
  * @param  String 要显示的字符串，范围：ASCII可见字符
  * @retval 无
  */
void OLED096_I2C::showString(uint8_t Line, uint8_t Column, char* String) {
	uint8_t i;
	for (i = 0; String[i] != '\0'; i++)
	{
		this->showChar(Line, Column + i, String[i]);
	}
}

/**
  * @brief  OLED显示数字（十进制，正数）
  * @param  Line 起始行位置，范围：0-3
  * @param  Column 起始列位置，范围：0-15
  * @param  Number 要显示的数字，范围：0-4294967295
  * @param  Length 要显示数字的长度，范围：1-10
  */
void OLED096_I2C::showNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length) {
	for(s8 i = Length - 1; i >= 0; i--) {
		this->showChar(Line, Column + i, Number % 10 + '0');
		Number /= 10;
	}
}

/**
  * @brief  OLED显示数字（十进制，有符号数）
  * @param  Line 起始行位置，范围：0-3
  * @param  Column 起始列位置，范围：0-15
  * @param  Number 要显示的数字，范围：0-4294967295
  * @param  Length 要显示数字的长度，范围：1-10
  */
void OLED096_I2C::showSignedNum(uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length) {
	if(Number >= 0)
		this->showChar(Line, Column, '+');
	else {
		this->showChar(Line, Column, '-');
		Number = -Number;
	}
	for(s8 i = Length - 1; i >= 0; i--) {
		this->showChar(Line, Column + i + 1, Number % 10 + '0');
		Number /= 10;
	}
}

/**
  * @brief  OLED显示数字（十进制，正数）
  * @param  Line 起始行位置，范围：0-3
  * @param  Column 起始列位置，范围：0-15
  * @param  Number 要显示的数字，范围：0~4294967295
  * @param  Length 要显示数字的长度，范围：1~10
  * @retval 无
  */
void OLED096_I2C::showBinNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length) {
	for (u8 i = 0; i < Length; i++)							
	{
		OLED096_I2C::showChar(Line, Column + i, '0' + !!(Number & (1 << (Length-i-1))));
	}
}

/**
  * @brief  OLED显示数字（十六进制，正数）
  * @param  Line 起始行位置，范围：0-3
  * @param  Column 起始列位置，范围：0-15
  * @param  Number 要显示的数字，范围：0~0xFFFFFFFF
  * @param  Length 要显示数字的长度，范围：1~8
  */
 void OLED096_I2C::showHexNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
	uint8_t i, SingleNumber;
	for (i = 0; i < Length; i++)							
	{
		SingleNumber = ((Number >> ((Length-i-1) * 4)) & 0xf);
		if (SingleNumber < 10)
		{
			this->showChar(Line, Column + i, SingleNumber + '0');
		}
		else
		{
			this->showChar(Line, Column + i, SingleNumber - 10 + 'A');
		}
	}
}

OLED096_I2C::OLED096_I2C() {
	this->slaveAddress = 0x78;
}
