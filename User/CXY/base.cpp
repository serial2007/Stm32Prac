#include "base.h"

u32 _GPIOX_Periph_Word(GPIO_TypeDef* RCC_APB2Periphx) {
  switch (reinterpret_cast<u32>(RCC_APB2Periphx)) {
  case reinterpret_cast<u32>(GPIOA):
    return RCC_APB2Periph_GPIOA;
  case reinterpret_cast<u32>(GPIOB):
    return RCC_APB2Periph_GPIOB;
	case reinterpret_cast<u32>(GPIOC):
    return RCC_APB2Periph_GPIOC;
	case reinterpret_cast<u32>(GPIOD):
    return RCC_APB2Periph_GPIOD;
	case reinterpret_cast<u32>(GPIOE):
    return RCC_APB2Periph_GPIOE;
	case reinterpret_cast<u32>(GPIOF):
    return RCC_APB2Periph_GPIOF;
	case reinterpret_cast<u32>(GPIOG):
    return RCC_APB2Periph_GPIOG;
	default:
		return 0;
  }
}
