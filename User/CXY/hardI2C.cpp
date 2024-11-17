#include "hardI2C.h"

hardI2C::hardI2C(I2C_TypeDef* _I2Cx)
{
	this->I2Cx = _I2Cx;
	this->SCL_channel = GPIOB;
	this->SDA_channel = GPIOB;
	
	if(I2Cx == I2C1) {
		this->SCL_pin = GPIO_Pin_6;
		this->SDA_pin = GPIO_Pin_7;
	}
	if(I2Cx == I2C2) {
		this->SCL_pin = GPIO_Pin_10;
		this->SDA_pin = GPIO_Pin_11;
	}
}

void hardI2C::Init()
{
	if(I2Cx == I2C1)
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
	if(I2Cx == I2C2)
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);
	RCC_APB2PeriphClockCmd(_GPIOX_Periph_Word(SCL_channel),ENABLE);
	RCC_APB2PeriphClockCmd(_GPIOX_Periph_Word(SDA_channel),ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_InitStructure.GPIO_Pin = SCL_pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SCL_channel, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = SDA_pin;
	GPIO_Init(SDA_channel, &GPIO_InitStructure);
	
	I2C_InitTypeDef I2C_InitStructure;
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_ClockSpeed = 100000;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable; // 接受一个字节后是否给应答
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_OwnAddress1 = 0x00; // I2C地址
	I2C_Init(I2Cx, &I2C_InitStructure);
	I2C_Cmd(I2Cx, ENABLE);
}

