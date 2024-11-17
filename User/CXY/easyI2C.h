#pragma once
#include "base.h"

class easyI2C {
public:
	easyI2C() {
		this->delay_us = 1;
	}
	GPIO_TypeDef* SCL_channel;
	u16 SCL_pin;
	GPIO_TypeDef* SDA_channel;
	u16 SDA_pin;

	void Init();
	void Start();
	void Stop();
	void SendByte(u8 byte);
	u8 ReceiveByte(void);

	template <typename T> void SendAck(T Ackbit) {
		this->Write_SDA((BitAction)Ackbit);
		this->Write_SCL(1);
		this->Write_SCL(0);
	}
	u8 ReceiveAck(void); 
/**
 * @brief 写入SCL的电平
 */
	template <typename T> inline void Write_SCL(T x) {
		GPIO_WriteBit(SCL_channel, SCL_pin, (BitAction)x);
		Delay_us(this->delay_us);
	}
/**
 * @brief 写入SDA的电平
 */
	template <typename T> inline void Write_SDA(T x) {
		GPIO_WriteBit(SDA_channel, SDA_pin, (BitAction)x);
		Delay_us(this->delay_us);
	}
/**
 * @brief 读取SDA的电平
 */
	inline u8 Read_SDA(void) {
		u8 BitValue = GPIO_ReadInputDataBit(this->SDA_channel,
			this->SDA_pin);
		Delay_us(this->delay_us);
		return BitValue;
	}

	u32 delay_us;
};

