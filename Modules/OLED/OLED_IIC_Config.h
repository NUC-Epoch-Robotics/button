/**
 * ************************************************************************
 * 
 * @file OLED_IIC_Config.h
 * @author zxr
 * @brief IIC��OLED��������ͷ�ļ�
 * 
 * ************************************************************************
 * @copyright Copyright (c) 2024 zxr 
 * ************************************************************************
 */
#ifndef OLED_IIC_CONFIG_H
#define OLED_IIC_CONFIG_H

#include "main.h"
#include "stm32f1xx_hal.h"

#define  OLED_ADDRESS 		0x78	//OLED��ַ  Ĭ��0x78

//OLED������ƺ�
#define  	LEFT 			0x27
#define  	RIGHT 			0x26
#define  	UP 			0X29
#define  	DOWM			0x2A
#define  	ON			0xA7
#define  	OFF			0xA6


#define    	SCREEN_PAGE_NUM		(8)     //��Ļҳ��
#define    	SCREEN_PAGEDATA_NUM	(128)   //ÿҳ�����ݸ���
#define		SCREEN_COLUMN		(128)   //����
#define  	SCREEN_ROW		(64)    //����


void WriteCmd(unsigned char cmd);		//д����
void WriteDat(unsigned char dat);		//д����
void OLED_ON(void);				//����OLED
void OLED_OFF(void);				//����OLED
void OLED_CLS(void);				//OLED��������
void OLED_Init(void);				//OLED��ʼ������
void OLED_RefreshRAM(void);			//�������ݻ�����
void OLED_ClearRAM(void);			//������ݻ�����
void OLED_SetPixel(signed short int x, signed short int y, unsigned char set_pixel);	//�����������ص�����
void OLED_DisplayMode(unsigned char mode);	//��Ļ����ȡ����ʾ
void OLED_IntensityControl(unsigned char intensity);//��Ļ���ȵ���
void OLED_Shift(unsigned char shift_num);	//ȫ������ƫ��ָ������
void OLED_HorizontalShift(unsigned char start_page,unsigned char end_page,unsigned char direction);	//��Ļ����ˮƽ�����������

#endif  /*OLED_IIC_CONFIG_H*/

