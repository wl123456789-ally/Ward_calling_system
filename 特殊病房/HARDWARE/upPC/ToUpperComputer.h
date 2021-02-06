#ifndef __TOUPPERCOMPUTER_H__
#define __TOUPPERCOMPUTER_H__

#include "stm32f10x.h"


/**/
typedef struct
{
	volatile float ROL;
	volatile float PIT;
	volatile float YAW;
	volatile float /*high value*/ALE_USE/*unit: cm*/;
	volatile uint8_t FLAY_MODE;
	volatile uint8_t ARMED;
}TypeStatusStructure;

typedef struct
{
	volatile int ACC_X;
	volatile int ACC_Y;
	volatile int ACC_Z;
	volatile int GYRO_X;
	volatile int GYRO_Y;
	volatile int GYRO_Z;
	volatile int MAG_X;
	volatile int MAG_Y;
	volatile int MAG_Z;
}TypeSensorStructure;

#define dataBYTE0(dwTemp)       ( *( (uint8_t *)(&dwTemp)    ) )
#define dataBYTE1(dwTemp)       ( *( (uint8_t *)(&dwTemp) + 1) )
#define dataBYTE2(dwTemp)       ( *( (uint8_t *)(&dwTemp) + 2) )
#define dataBYTE3(dwTemp)       ( *( (uint8_t *)(&dwTemp) + 3) )

#define __CraftStatusUpdata(statusStructure, rol, pit, yaw, \
                                             high)                 do{\
	(&statusStructure)->ROL = rol;\
	(&statusStructure)->PIT = pit;\
	(&statusStructure)->YAW = yaw;\
	(&statusStructure)->ALE_USE = high;\
}while(0)

#define __IMUsensorUpdata(statusStructure, xAcc, yAcc, zAcc,\
                                           xGyro, yGyro, zGyro)    do{\
	(&statusStructure)->ACC_X = xAcc;\
	(&statusStructure)->ACC_Y = yAcc;\
	(&statusStructure)->ACC_Z = zAcc;\
	(&statusStructure)->GYRO_X = xGyro;\
	(&statusStructure)->GYRO_Y = yGyro;\
	(&statusStructure)->GYRO_Z = yGyro;\
}while(0)

#define __SensorUpdata(statusStructure, xAcc, yAcc, zAcc,\
                                        xGyro, yGyro, zGyro,\
																				xMag, yMag, zMag)           do{\
	(&statusStructure)->ACC_X = xAcc;\
	(&statusStructure)->ACC_Y = yAcc;\
	(&statusStructure)->ACC_Z = zAcc;\
	(&statusStructure)->GYRO_X = xGyro;\
	(&statusStructure)->GYRO_Y = yGyro;\
	(&statusStructure)->GYRO_Z = yGyro;\
	(&statusStructure)->MAG_X = xMag;\
	(&statusStructure)->MAG_Y = yMag;\
	(&statusStructure)->MAG_Z = zMag;\
}while(0)

void TransmitUserCurve_s32(uint8_t ch,int32_t dat);
void TransmitStatus(TypeStatusStructure* structure);
void TransmitSensordata(TypeSensorStructure* structure);
void get_pid(void);

#endif

