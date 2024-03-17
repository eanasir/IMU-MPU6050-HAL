//MPU6050.c KNR 2024

#include "MPU-6050.h"
#include <stdint.h>
#include <stdio.h>
#include <math.h>

void MPU6050_Reset(){
	uint8_t Reset = 1;
	Reset = Reset << 7;
	HAL_I2C_Mem_Write(&MPU6050_PORT, MPU6050_ADDRESS, PWR_MGMT_1, 1, &Reset, 1, timeout);
}
int16_t MPU6050_TEMP_Read(){
	uint8_t TEMP_OUT[2];//temperatura zapisana jest w dwóch bajtach
	int16_t TEMP_Signed;

	HAL_I2C_Mem_Read(&MPU6050_PORT, MPU6050_ADDRESS, TEMP_OUT_H, 1, TEMP_OUT, 2, timeout);

    TEMP_Signed = (int16_t)(TEMP_OUT[0] << 8 | TEMP_OUT[1]);
     //przesuwamy jeden bajt i dopisujemy drugi

    return TEMP_Signed;
}

float MPU6050_TEMP_Read_Celsius(){
	float temp_C;
	temp_C = (float)(((int16_t)MPU6050_TEMP_Read()/340.0)+36.53);
	return temp_C;//funkcja zwraca temperature w stopniach celsjusza
}
uint8_t MPU6050_Init(){
	uint8_t Who_Am_I;
	HAL_I2C_Mem_Read(&MPU6050_PORT, MPU6050_ADDRESS, WHO_AM_I, 1, &Who_Am_I, 1, timeout);
	//debilu to jest rejestr do odczytu

	if(Who_Am_I == 104){
		//adres WHO_AM_I powinien zwrocic 0x68 wg danych producenta
		uint8_t Pwr_Mgmt = 0;
		HAL_I2C_Mem_Write(&MPU6050_PORT, MPU6050_ADDRESS, PWR_MGMT_1, 1, &Pwr_Mgmt, 1, timeout);
		// zapisanie tego rejestru zerami włącza czujnik w normalnym trybie pracy
		return 1;
	}
	else{
		return 0;
	}

}
void MPU6050_Config(uint8_t G_SEL,uint8_t A_SEL){
	if(G_SEL < 5){
		G_SEL = G_SEL << 3;// przesuniecie bitow o 3 by zapisały bit 3 i 4 w odpowiednich rejestrach


		HAL_I2C_Mem_Write(&MPU6050_PORT, MPU6050_ADDRESS, GYRO_CONFIG, 1, &G_SEL, 1, timeout);
	}
	if(A_SEL < 5){
		A_SEL = A_SEL << 3;
		HAL_I2C_Mem_Write(&MPU6050_PORT, MPU6050_ADDRESS, ACCEL_CONFIG, 1, &A_SEL, 1, timeout);

	}
	/* ta funkcja przyjmuje liczy 0,1,2,3,4 jako argumenty - w zależności od nich ustawia zakres
	* IMU. dokładne wartości zakresów znajdują się w mapie rejestrów w rozdziale 4.4 i 4.5.
	* w celu optymalizacji kodu zamiast dzielić przez te wartości, mnożę przez ich odwrotności.
	*/
}
void MPU6050_ACCEL_Read(float ACCEL[3],uint8_t A_SEL){
	int16_t ACCEL_Int[3];
	uint8_t bufferX[2];
	uint8_t bufferY[2];
	uint8_t bufferZ[2];


	HAL_I2C_Mem_Read(&MPU6050_PORT, MPU6050_ADDRESS, ACCEL_XOUT, 1, bufferX, 2, timeout);
	//odczyt x
	ACCEL_Int[0]= (int16_t)(bufferX[0]<< 8 | bufferX[1]);
	HAL_I2C_Mem_Read(&MPU6050_PORT, MPU6050_ADDRESS, ACCEL_YOUT, 1, bufferY, 2, timeout);
	//odczyt y
	ACCEL_Int[1]= (int16_t)(bufferY[0]<< 8 | bufferY[1]);
	HAL_I2C_Mem_Read(&MPU6050_PORT, MPU6050_ADDRESS, ACCEL_ZOUT, 1, bufferZ, 2, timeout);
	//odczyt z
	ACCEL_Int[2]= (int16_t)(bufferZ[0]<< 8 | bufferZ[1]);


	switch (A_SEL){
		case 0:
			for(int i = 0; i < 3; i++){
			ACCEL[i]= (float)(ACCEL_Int[i] * 0.000061);
			}
			break;
		case 1:
			for(int i = 0; i < 3; i++){
			ACCEL[i]= (float)(ACCEL_Int[i] * 0.000122);
			}
			break;
		case 2:
			for(int i = 0; i < 3; i++){
			ACCEL[i]= (float)(ACCEL_Int[i] * 0.000244);
			}
			break;
		case 3:
			for(int i = 0; i < 3; i++){
			ACCEL[i]= (float)(ACCEL_Int[i] * 0.000488);
			}
			break;
		}

	/*
	 * w zależności od ustawień przyjętych przy konfigurowaniu akcelerometru pomiary będą zwracane
	 * w różnym zakresie, który można znaleźć w mapie rejestrów w rozdziale 4.17.
	 * w celu optymalizacji kodu zamiast dzielić przez te wartości, mnożę przez ich odwrotności.
	 */
}
void MPU6050_GYRO_Read(float GYRO[3],uint8_t G_SEL){
	int16_t GYRO_Int[3];
	uint8_t bufferX[2];
	uint8_t bufferY[2];
	uint8_t bufferZ[2];


	HAL_I2C_Mem_Read(&MPU6050_PORT, MPU6050_ADDRESS, GYRO_XOUT, 1, bufferX, 2, timeout);
	//odczyt x
	GYRO_Int[0]= (int16_t)(bufferX[0]<< 8 | bufferX[1]);
	HAL_I2C_Mem_Read(&MPU6050_PORT, MPU6050_ADDRESS, GYRO_YOUT, 1, bufferY, 2, timeout);
	//odczyt y
	GYRO_Int[1]= (int16_t)(bufferY[0]<< 8 | bufferY[1]);
	HAL_I2C_Mem_Read(&MPU6050_PORT, MPU6050_ADDRESS, GYRO_ZOUT, 1, bufferZ, 2, timeout);
	//odczyt z
	GYRO_Int[2]= (int16_t)(bufferZ[0]<< 8 | bufferZ[1]);



	switch (G_SEL){
		case 0:
			for(int i = 0; i < 3; i++){
			GYRO[i]= (float)(GYRO_Int[i] * 0.007633);
			}
			break;
		case 1:
			for(int i = 0; i < 3; i++){
			GYRO[i]= (float)(GYRO_Int[i] * 0.015267);
			}
			break;
		case 2:
			for(int i = 0; i < 3; i++){
			GYRO[i]= (float)(GYRO_Int[i] * 0.030487);
			}
			break;
		case 3:
			for(int i = 0; i < 3; i++){
			GYRO[i]= (float)(GYRO_Int[i] * 0.060975);
			}
			break;
		}

	/*
	 * w zależności od ustawień przyjętych przy konfigurowaniu żyroskopu pomiary będą zwracane
	 * w różnym zakresie, który znaleźć można w mapie rejestrów w rozdziale 4.19.
	 */
}
void MPU6050_SELF_TEST(float OFFSET_ACCEL[3],float OFFSET_GYRO[3]){
	MPU6050_Config(0, 2);
	uint8_t buffer[4];
	HAL_I2C_Mem_Read(&MPU6050_PORT, MPU6050_ADDRESS, SELF_TEST_X, 1, buffer, 4, timeout);

	if((buffer[0]&31) != 0){
	OFFSET_GYRO[0] = (float)(25.0 * 131.0 *(pow( 1.046,(double)( buffer[0] & 31) - 1.0)));
	//offset żyroskopu w osi x
	}
	else{
		OFFSET_GYRO[0] = 0;
	}
	if((buffer[1]&31) != 0){
	OFFSET_GYRO[1] = (float)(-25.0 * 131.0 *(pow( 1.046,(double)( buffer[1] & 31) - 1.0)));
	//offset żyroskopu w osi y
	}
	else{
		OFFSET_GYRO[1] = 0;
	}
	if((buffer[2]&31) != 0){
	OFFSET_GYRO[2] = (float)(25.0 * 131.0 *(pow( 1.046,(double)( buffer[2] & 31) - 1.0)));
	//offset żyroskopu w osi z
	}
	else{
		OFFSET_GYRO[2] = 0;
	}
	if(((buffer[0] & 224)|(buffer[3] & 3))!=0 ){
	OFFSET_ACCEL[0]= (float)(4096 * 0.34 * pow((0.92/0.34),(((double)((buffer[0] & 224)|(buffer[3] & 3)))-1.0)/30.0));
	//offset akcelerometru w osi x
	}
	else{
		OFFSET_ACCEL[0]=0;
	}
	if(((buffer[1] & 224)|(buffer[3] & 12))!=0 ){
	OFFSET_ACCEL[1]= (float)(4096 * 0.34 * pow((0.92/0.34),(((double)((buffer[1] & 224)|(buffer[3] & 12)))-1.0)/30.0));
	//offset akcelerometru w osi y
	}
	else{
		OFFSET_ACCEL[1]=0;
		}
	if(((buffer[1] & 224)|(buffer[3] & 12))!=0 ){
	OFFSET_ACCEL[2]= (float)(4096 * 0.34 * pow((0.92/0.34),(((double)((buffer[2] & 224)|(buffer[3] & 48)))-1.0)/30.0));
	//offset akcelerometru w osi z
	}
	else{
		OFFSET_ACCEL[2]=0;
		}
	/*
	 * wzory na poszczególne offsety pochodzą z rozdziału 4.1 mapy rejestrów.
	 */
}









