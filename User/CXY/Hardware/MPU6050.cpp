#include "MPU6050.h"

void MPU6050::Init()
{
	I2C.Init();
	WriteReg(MPU6050_PWR_MGMT_1, 0x01); // 选择X轴的陀螺仪时钟
	WriteReg(MPU6050_PWR_MGMT_2, 0x00);
	WriteReg(MPU6050_SMPLRT_DIV, 0x09); // 输出10分频
	WriteReg(MPU6050_CONFIG, 0x06);     // 外部同步 滤波设置
	WriteReg(MPU6050_GYRO_CONFIG, 0x18); // 陀螺仪配置 最大量程
	WriteReg(MPU6050_ACCEL_CONFIG, 0x18); // 加速度计配置
}
/**
 * @brief 指定地址写寄存器
 * @param RegAddress 寄存器地址
 * @param Data 数据
 * @retval 是否得到应答（1应答）
 */
bool MPU6050::WriteReg(u8 RegAddress, u8 Data) {
	I2C.Start();
	I2C.SendByte(slaveAddress);
	if (I2C.ReceiveAck()) return 0;
	I2C.SendByte(RegAddress);
	if (I2C.ReceiveAck()) return 0;
	I2C.SendByte(Data);
	if (I2C.ReceiveAck()) return 0;
// 如果发送多个字节，则在这里加入
	I2C.Stop();
	return 1;
}
/**
 * @brief 指定地址读寄存器
 * @param RegAddress 寄存器地址
 * @retval 寄存器的值
 * @attention 收不到数据没有特殊处理
 */
u8 MPU6050::ReadReg(u8 RegAddress)
{
	I2C.Start();
	I2C.SendByte(slaveAddress);
	I2C.ReceiveAck();
	I2C.SendByte(RegAddress);
	I2C.ReceiveAck();
// 重复起始条件
	I2C.Start();
	I2C.SendByte(slaveAddress | 0x01); //发送读指令
	I2C.ReceiveAck();
	u8 Data = I2C.ReceiveByte();
	I2C.SendAck(1); // 不继续接受字节
	I2C.Stop();
	return Data;
}
/**
 *	@brief 获取传感器的数据，结果存放在class内部变量里
 */
void MPU6050::GetData() {
	AccX = static_cast<s16>(((u16)ReadReg(MPU6050_ACCEL_XOUT_H) << 8) | (u16)ReadReg(MPU6050_ACCEL_XOUT_L));
	AccY = static_cast<s16>(((u16)ReadReg(MPU6050_ACCEL_YOUT_H) << 8) | (u16)ReadReg(MPU6050_ACCEL_YOUT_L));
	AccZ = static_cast<s16>(((u16)ReadReg(MPU6050_ACCEL_ZOUT_H) << 8) | (u16)ReadReg(MPU6050_ACCEL_ZOUT_L));
	GyroX = static_cast<s16>(((u16)ReadReg(MPU6050_GYRO_XOUT_H) << 8) | (u16)ReadReg(MPU6050_GYRO_XOUT_L));
	GyroY = static_cast<s16>(((u16)ReadReg(MPU6050_GYRO_YOUT_H) << 8) | (u16)ReadReg(MPU6050_GYRO_YOUT_L));
	GyroZ = static_cast<s16>(((u16)ReadReg(MPU6050_GYRO_ZOUT_H) << 8) | (u16)ReadReg(MPU6050_GYRO_ZOUT_L));
	Temp  = static_cast<s16>(((u16)ReadReg(MPU6050_TEMP_OUT_H) << 8) | (u16)ReadReg(MPU6050_TEMP_OUT_L));
}

u8 MPU6050::GetID()
{
	return ReadReg(MPU6050_WHO_AM_I);
}
