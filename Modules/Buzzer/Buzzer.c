#include "stm32f1xx_hal.h"
#include "Buzzer.h"
#include "tim.h"
#include "gpio.h"

//设置定时器的CCR寄存器的值
#define BELL_PWM_FREQ                   2000000

void Buzzer_SetFreq(uint16_t freq)
{	
	uint32_t Period = BELL_PWM_FREQ / freq;
	uint16_t Pulse  = Period  / 2;
	
	 HAL_TIM_PWM_Init(&htim2);// 初始化定时器的PWM模式
	
	 HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_1);// 启动定时器的PWM模式	
	
	/*设置频率和占空比*/
	
	__HAL_TIM_SET_AUTORELOAD (&htim2, Period - 1);
	
	__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2,Pulse);	
}

void Buzzer_off(void)
{	
	HAL_TIM_PWM_Stop(&htim2,TIM_CHANNEL_1);// 停止定时器的PWM输出
}

void Buzzer_on(void)
{
	Buzzer_SetFreq(2000);//设置自动重装载值为2000 ARR寄存器的值为1000 占空比为50%

	Buzzer_off();
}

