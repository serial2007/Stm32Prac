#include "stm32f10x.h"
#include "Delay.h"

int main() {
	GPIO_InitTypeDef GPIO_InitStructure = {
		.GPIO_Mode = GPIO_Mode_AF_OD,
		.GPIO_Pin  = GPIO_Pin_10 | GPIO_Pin_11,
		.GPIO_Speed = GPIO_Speed_50MHz
	};
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	I2C_InitTypeDef I2C_InitStructure = {
		.I2C_Mode = I2C_Mode_I2C,
		.I2C_ClockSpeed = 100000,
		.I2C_DutyCycle = I2C_DutyCycle_16_9,
		.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit,
		.I2C_OwnAddress1 = 0,
		.I2C_Ack = I2C_Ack_Enable
	};
	I2C_Init(I2C2, &I2C_InitStructure);
	I2C_Cmd(I2C2, ENABLE);

// 生成起始条件
	I2C_GenerateSTART(I2C2, ENABLE);
	while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT));

	I2C_Send7bitAddress(I2C2, 0xD0, I2C_Direction_Transmitter);
		// 应答位不需要处理
	while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

	I2C_GenerateSTOP(I2C2, ENABLE);
}