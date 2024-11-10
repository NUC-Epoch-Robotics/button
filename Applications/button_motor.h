#ifndef __BUTTON_MOTOR_H
#define __BUTTON_MOTOR_H

#include "gpio.h"


void Buttonmotorinit(void);
void User_rx_Callback(uint8_t data);
void user_TIM2_IRQHandler(void);
uint8_t read_button1_GPIO(void);
uint8_t read_button2_GPIO(void);
uint8_t read_button3_GPIO(void);
uint8_t read_button4_GPIO(void);
uint8_t read_button5_GPIO(void);
uint8_t read_button6_GPIO(void);
void button1_callback(void *button);
void button2_callback(void *button);
void button3_callback(void *button);
void button4_callback(void *button);
void button5_callback(void *button);
void button6_callback(void *button);
void KEY_Init(void);
#endif

