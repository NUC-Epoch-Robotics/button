#ifndef __SELF_DEF_MASSAGE_H
#define __SELF_DEF_MASSAGE_H

#include "gpio.h"

#define usart_frame1_hand 0x5A   //֡ͷ
#define usart_frame2_hand 0xA5   //֡ͷ
#define usart_frame_end  0xD0    //֡β

#define USART_PACKAGE_LEN  64  //����������ֽ���
#define BUF_SIZE     64      //�����������ֽ���
 
/* �������� */
typedef enum 
{
 usart_W_REG = 0x80 ,                    //USARTд�Ĵ���ָ��
 usart_R_REG  ,                               //USART���Ĵ���ָ��
 usart_W_DATA   ,                            //USARTд����ָ��
 usart_R_DATA       ,                   //USART������ָ��
}FrameCommand;

 
/*����֡*/
typedef struct 
{
 uint8_t frame_hand1;     //֡ͷ
 uint8_t frame_hand2;     //֡ͷ
 uint8_t frame_hand3;     //֡ͷ
 FrameCommand  command;  //���� ���ƽ��շ�ʵ�ֲ�ͬģʽ
 uint16_t data[USART_PACKAGE_LEN]; //���ݰ�������
 uint8_t len;            //���ݳ���
 uint8_t frame_end;       //֡β
 uint32_t (*crc_check)(uint32_t *data,uint32_t len);
}FrameInstance;



typedef  uint32_t (*crc_check)( uint32_t *data,uint32_t len);
	
uint32_t CRC16_Check( uint32_t *data,uint32_t len);
//typedef uint16_t (*CRC16_check)(const uint8_t *data,uint8_t len);

//uint16_t CRC16_Check(const uint8_t *data,uint8_t len);

void frameInstance_init(FrameInstance* frame,FrameCommand command);
void frame_buf(FrameInstance* frame,uint8_t* Data,int len);

#endif


