#include "base.h"

/* 这里使用I2C2外设 */
class hardI2C {
public:
	hardI2C(I2C_TypeDef* _I2Cx);
	void Init();
	inline void Start() {
		I2C_GenerateSTART(I2Cx, ENABLE);
	}
	

	I2C_TypeDef* I2Cx;
	GPIO_TypeDef* SCL_channel;
	u16 SCL_pin;
	GPIO_TypeDef* SDA_channel;
	u16 SDA_pin;
};
