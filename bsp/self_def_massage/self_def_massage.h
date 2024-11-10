#ifndef __SELF_DEF_MASSAGE_H
#define __SELF_DEF_MASSAGE_H

#include "gpio.h"

#define usart_frame_hand1 0x5A   //帧头
#define usart_frame_hand2 0xA5   //帧头
#define usart_frame_end  0xD0    //帧尾

#define USART_PACKAGE_LEN  64  //单次最大发送字节数
#define BUF_SIZE     64      //单次最大接收字节数
 
/* 发送命令 */
typedef enum 
{
 usart_W_REG = 0x80 ,                    //USART写寄存器指令
 usart_R_REG  ,                               //USART读寄存器指令
 usart_W_DATA   ,                            //USART写数据指令
 usart_R_DATA       ,                   //USART读数据指令
}FrameCommand;

 
/*数据帧*/
typedef struct 
{
 uint8_t frame_hand1;     //帧头
 uint8_t frame_hand2;     //帧头
 uint8_t frame_hand3;     //帧头
 FrameCommand  command;  //命令 控制接收方实现不同模式
 uint16_t data[USART_PACKAGE_LEN]; //数据包缓冲区
 uint8_t len;            //数据长度
 uint8_t frame_end;       //帧尾
 //uint16_t (*CRC16_check)(const uint8_t *data,uint8_t len);
}FrameInstance;


//typedef uint16_t (*CRC16_check)(const uint8_t *data,uint8_t len);

//uint16_t CRC16_Check(const uint8_t *data,uint8_t len);

void frameInstance_init(FrameInstance* frame,FrameCommand command);
void frame_buf(FrameInstance* frame,uint8_t* Data,int len);
void Uart_Idle_rcDMA(UART_HandleTypeDef *huart,uint8_t* DataBuff);

#endif


