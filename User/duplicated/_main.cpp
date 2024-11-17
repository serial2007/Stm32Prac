#include "stm32f10x.h"                  // Device header
#include "Hardware/oled096.h"
#include "Hardware/MPU6050.h"
#include <stdio.h>

int main(void)
{
	/*模块初始化*/

	OLED096_I2C oled;
	oled.I2C.delay_us = 0;
	oled.I2C.SCL_channel = GPIOB;
	oled.I2C.SCL_pin = GPIO_Pin_8;
	oled.I2C.SDA_channel = GPIOB;
	oled.I2C.SDA_pin = GPIO_Pin_9;
	oled.init();
	
	MPU6050 mpu6050;
	mpu6050.I2C.SCL_channel = GPIOB;
	mpu6050.I2C.SCL_pin = GPIO_Pin_10;
	mpu6050.I2C.SDA_channel = GPIOB;
	mpu6050.I2C.SDA_pin = GPIO_Pin_11;

	mpu6050.Init();
	u8 id = mpu6050.ReadReg(0x75); // whoami

	u8 chk = mpu6050.WriteReg(0x6B, 0x00);
	chk &=   mpu6050.WriteReg(0x19, 0xAA);
	u8 reg = mpu6050.ReadReg(0x19);

	while(1) {
		mpu6050.GetData();
		oled.showSignedNum(0,0,mpu6050.AccX,5);
		oled.showSignedNum(1,0,mpu6050.AccY,5);
		oled.showSignedNum(2,0,mpu6050.AccZ,5);
		oled.showSignedNum(0,8,mpu6050.GyroX,5);
		oled.showSignedNum(1,8,mpu6050.GyroY,5);
		oled.showSignedNum(2,8,mpu6050.GyroZ,5);
	}
} 
