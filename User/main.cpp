#ifndef STM32F10X_MD
#define STM32F10X_MD
#endif
#include "base.h"
#include "Hardware/oled096.h"
#include <stdio.h>

OLED096_I2C* oled;
 u32 time = 0;
static char str[25];

int main() {
	oled = new OLED096_I2C;
	oled->I2C.SCL_channel = GPIOB;
	oled->I2C.SCL_pin = GPIO_Pin_8;
	oled->I2C.SDA_channel = GPIOB;
	oled->I2C.SDA_pin = GPIO_Pin_9;
	oled->init();
	oled->showChar(0, 0, 'a');

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
TIM_InternalClockConfig(TIM2);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 1000 - 1;
	TIM_TimeBaseInitStructure.TIM_Prescaler = 7200 - 1;
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);

	TIM_ClearFlag(TIM2, TIM_FLAG_Update);
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);

	TIM_Cmd(TIM2, ENABLE);
	
	while(1) {
		sprintf(str, "Time: %02d:%02d:%02d.%01d", time/36000%99,
		time/600%60, time/10%60, time%10);
		oled->showString(0, 0, str);
	}
}

extern "C" 
	void TIM2_IRQHandler(void)
	{
		if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
		{
			time ++;
			TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
		}
	}
