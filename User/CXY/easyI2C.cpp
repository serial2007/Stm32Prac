#include "easyI2C.h"	

void easyI2C::Init() {
	RCC_APB2PeriphClockCmd(_GPIOX_Periph_Word(SCL_channel), ENABLE);
	RCC_APB2PeriphClockCmd(_GPIOX_Periph_Word(SDA_channel), ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; // 开漏输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = SCL_pin;
 	GPIO_Init(SCL_channel, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = SDA_pin;
 	GPIO_Init(SDA_channel, &GPIO_InitStructure);

	// 给高电平	

}

void easyI2C::Start() {
	this->Write_SDA(1);
	this->Write_SCL(1);
	this->Write_SDA(0);
	this->Write_SCL(0);
}

void easyI2C::Stop() {
	this->Write_SDA(0);
	this->Write_SCL(1);
	this->Write_SDA(1);
}

void easyI2C::SendByte(u8 byte) {
	for(u8 i=0; i<8; i++) {
		// 高位先行
		this->Write_SDA(!!(byte & (0x80 >> i)));
		this->Write_SCL(1);
		this->Write_SCL(0);
	}

}

u8 easyI2C::ReceiveByte(void) {
	u8 Byte = 0;
	this->Write_SDA(1);
	this->Write_SCL(1);
	for(u8 i=0; i<8; i++) {
		this->Write_SCL(1);
		Byte = Byte << 1;
		Byte |= this->Read_SDA();
		this->Write_SCL(0);
	}
	return Byte;
}

u8 easyI2C::ReceiveAck(void) {
	this->Write_SDA(1);
	this->Write_SCL(1);
	u8 AckBit = this->Read_SDA();
	this->Write_SCL(0);
	return AckBit;
}
