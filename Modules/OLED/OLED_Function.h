/**
 * ************************************************************************
 * 
 * @file OLED_Function.h
 * @author zxr
 * @brief OLED���ܺ�������ͷ�ļ�
 * 
 * ************************************************************************
 * @copyright Copyright (c) 2024 zxr 
 * ************************************************************************
 */
#ifndef _OLED_FUNCTION_H_
#define _OLED_FUNCTION_H_

#include "OLED_IIC_Config.h"

//�ַ�����ʾ����
void OLED_ShowStr(signed short int x, signed short int y, unsigned char ch[], unsigned char TextSize);
//���ĺ�����ʾ����
void OLED_ShowChinese(signed short int x, signed short int y, unsigned char* ch);
//BMPͼƬ��ʾ����
void OLED_ShowBMP(signed short int x0,signed short int y0,signed short int L,signed short int H,const unsigned char BMP[]);

#endif /* _OLED_FUNCTION_H_ */
