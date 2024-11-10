#include "KEY.h"
#include "multi_button.h"
#include "main.h"
#include "gpio.h"

uint8_t read_button1_GPIO()
{
	return HAL_GPIO_ReadPin(KEY1_GPIO_Port, KEY1_Pin);
}

uint8_t read_button2_GPIO()
{
	return HAL_GPIO_ReadPin(KEY2_GPIO_Port, KEY2_Pin);
}

uint8_t read_button3_GPIO()
{
	return HAL_GPIO_ReadPin(KEY3_GPIO_Port, KEY3_Pin);
}

uint8_t read_button4_GPIO()
{
	return HAL_GPIO_ReadPin(KEY4_GPIO_Port, KEY4_Pin);
}

uint8_t read_button5_GPIO()
{
	return HAL_GPIO_ReadPin(KEY5_GPIO_Port, KEY5_Pin);
}

uint8_t read_button6_GPIO()
{
	return HAL_GPIO_ReadPin(KEY6_GPIO_Port, KEY6_Pin);
}


