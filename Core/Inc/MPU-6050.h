// MPU6050.h KNR 2024
/*
 * mapa rejestrów https://invensense.tdk.com/wp-content/uploads/2015/02/MPU-6000-Register-Map1.pdf
 * datasheet https://invensense.tdk.com/wp-content/uploads/2015/02/MPU-6000-Datasheet1.pdf
 */
#include "main.h"
#include "i2c.h"

#define MPU6050_PORT hi2c1
extern I2C_HandleTypeDef MPU6050_PORT;

#define WHO_AM_I 0x75 // rejestr kontrolny
#define PWR_MGMT_1 0x6B // zarzadzanie zasilaniem
#define TEMP_OUT_L    0X42
#define TEMP_OUT_H	  0x41 //czujnik temperatury
#define GYRO_CONFIG 0x1B //konfiguracja żyroskopu
#define ACCEL_CONFIG 0x1C //konfiguracja akcelerometru
#define ACCEL_XOUT 0x3B // akcelerometr x
#define ACCEL_YOUT 0x3D // akcelerometr y
#define ACCEL_ZOUT 0x3F // akcelerometr z
#define GYRO_XOUT 0x43 // żyroskop x
#define GYRO_YOUT 0x45 // żyroskop y
#define GYRO_ZOUT 0x47 // żyroskop z
#define SELF_TEST_X 0x0D // test akcelerometru/żyroskopu x
#define SELF_TEST_Y 0x0E //test akcelerometru/żyroskopu y
#define SELF_TEST_Z 0x0F //test akcelerometru/żyroskopu z
#define SELF_TEST_A 0x10 // test akcelerometru  x/y/z
#define MPU6050_ADDRESS 0xD0
#define timeout 1000



int16_t MPU6050_TEMP_Read();
uint8_t MPU6050_Init();
float MPU6050_TEMP_Read_Celsius();
void MPU6050_Config(uint8_t G_SEL,uint8_t A_SEL);
void MPU6050_GYRO_Read(float GYRO[3],uint8_t G_SEL);
void MPU6050_ACCEL_Read(float ACCEL[3],uint8_t A_SEL);
void MPU6050_SELF_TEST(float OFFSET_ACCEL[3],float OFFSET_GYRO[3]);
void MPU6050_Reset();

