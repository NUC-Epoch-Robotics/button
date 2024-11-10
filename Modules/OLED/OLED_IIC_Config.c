/**
 * ************************************************************************
 * 
 * @file OLED_IIC_Config.c
 * @author zxr
 * @brief IIC��ʼ�����ú�OLED��Ļ�ĵײ�����
 * 
 * ************************************************************************
 * @copyright Copyright (c) 2024 zxr 
 * ************************************************************************
 */
#include "OLED_IIC_Config.h"
#include "i2c.h"
#include "bsp_iic.h"
unsigned char  ScreenBuffer[SCREEN_PAGE_NUM][SCREEN_COLUMN];//������Ļ�Ĵ洢�ռ�



/**
* @brief  ��OLED�Ĵ�����ַдһ��byte������
* @param  addr���Ĵ�����ַ
* @param  data��Ҫд�������
* @retval ��
*/
IICInstance OLED_IIC;
void I2C_WriteByte(uint8_t addr, uint8_t data)
{
	extern I2C_HandleTypeDef hi2c1;
//	HAL_I2C_Mem_Write(&hi2c1, OLED_ADDRESS, addr, I2C_MEMADD_SIZE_8BIT, &data, 1, 8);
	IICAccessMem( &OLED_IIC , OLED_ADDRESS, addr, &data,  1,I2C_MEMADD_SIZE_8BIT);
}

/**
 * ************************************************************************
 * @brief д�����
 * @param[in] cmd  д�������
 * ************************************************************************
 */
void WriteCmd(unsigned char cmd)
{
	I2C_WriteByte(0x00, cmd);
}

/**
 * ************************************************************************
 * @brief д���ݺ���
 * @param[in] dat  д�������
 * ************************************************************************
 */
void WriteDat(unsigned char dat)
{
	I2C_WriteByte(0x40, dat);
}

/**
 * ************************************************************************
 * @brief ����OLED
 * ************************************************************************
 */
void OLED_ON(void)
{
	WriteCmd(0X8D);  //���õ�ɱ�
	WriteCmd(0X14);  //������ɱ�
	WriteCmd(0XAF);  //OLED����
}

/**
 * ************************************************************************
 * @brief ����OLED
 * ************************************************************************
 */
void OLED_OFF(void)
{
	WriteCmd(0X8D);  //���õ�ɱ�
	WriteCmd(0X10);  //�رյ�ɱ�
	WriteCmd(0XAE);  //OLED����
}

/**
 * ************************************************************************
 * @brief OLED��������
 * ************************************************************************
 */
void OLED_CLS(void)//����
{
	unsigned char m,n;
	for(m=0;m<8;m++)
	{
		WriteCmd(0xb0+m);	//page0-page1
		WriteCmd(0x00);		//low column start address
		WriteCmd(0x10);		//high column start address
		for(n=0;n<128;n++)
		{
			WriteDat(0x00);
		}
	}
}

/**
 * ************************************************************************
 * @brief OLED��ʼ������
 * ************************************************************************
 */
void OLED_Init(void)
{
	bsp_iic_init(	&OLED_IIC, &hi2c1,IIC_BLACK_MODE);
	WriteCmd(0xAE); //��ʾ�ر�
	WriteCmd(0x20);	//�����ڴ�Ѱַģʽ
	WriteCmd(0x10);	//00,ˮƽѰַģʽ;01,��ֱѰַģʽ;10,ҳѰַģʽ(��λ);11,��Ч
	WriteCmd(0xb0);	//����ҳѰַģʽ��ҳ��ʼ��ַ,0-7
	WriteCmd(0xc8);	//����COM���ɨ�跽��
	WriteCmd(0x00); //-���õ��е�ַ
	WriteCmd(0x10); //-���ø��е�ַ
	WriteCmd(0x40); //-������ʼ�е�ַ
	WriteCmd(0x81); //���öԱȶȿ��ƼĴ���
	WriteCmd(0xff); //���ȵ��� 0x00~0xff
	WriteCmd(0xa1); //���ö�����ӳ��0��127
	WriteCmd(0xa6); //����������ʾ
	WriteCmd(0xa8); //���ø��ñ���(1��64)
	WriteCmd(0x3F); //
	WriteCmd(0xa4); //0xa4,�����ѭRAM����;0xa5,�������RAM����
	WriteCmd(0xd3); //������ʾƫ��
	WriteCmd(0x00); //��ƫ��
	WriteCmd(0xd5); //--set display clock divide ratio/oscillator frequency
	WriteCmd(0xf0); //--set divide ratio
	WriteCmd(0xd9); //--set pre-charge period
	WriteCmd(0x22); //
	WriteCmd(0xda); //--set com pins hardware configuration
	WriteCmd(0x12);
	WriteCmd(0xdb); //--set vcomh
	WriteCmd(0x20); //0x20,0.77xVcc
	WriteCmd(0x8d); //����DC-DCʹ��
	WriteCmd(0x14); //
	WriteCmd(0xaf); //--turn on oled panel
	OLED_CLS();
}

/**
 * ************************************************************************
 * @brief �������ݻ�����
 * ************************************************************************
 */
void OLED_RefreshRAM(void)
{
	// ҳѰַģʽ���
	for(unsigned short int m = 0; m < SCREEN_ROW/8; m++)
	{
		WriteCmd(0xb0+m);		//����ҳ��ַb0~b7
		WriteCmd(0x00);			//������ʾλ�á��е͵�ַ00-0f
		WriteCmd(0x10);			//������ʾλ�á��иߵ�ַ10-1f
		for(unsigned short int n = 0; n < SCREEN_COLUMN; n++)
		{
			WriteDat(ScreenBuffer[m][n]);
		}
	} 
}

/**
 * ************************************************************************
 * @brief ������ݻ�����
 * ************************************************************************
 */
void OLED_ClearRAM(void)
{
	for(unsigned short int m = 0; m < SCREEN_ROW/8; m++)
	{
		for(unsigned short int n = 0; n < SCREEN_COLUMN; n++)
		{
			ScreenBuffer[m][n] = 0x00;
		}
	}
}

/**
 * ************************************************************************
 * @brief �����������ص�����
 * 
 * @param[in] x  			��ʼ�������(x:0~127��
 * @param[in] y  			��ʼ��������(y:0~63��
 * @param[in] set_pixel  	�õ������  SET_PIXEL = 1, RESET_PIXEL = 0
 * 
 * ************************************************************************
 */
void OLED_SetPixel(signed short int x, signed short int y, unsigned char set_pixel)
{ 
	if (x >= 0 && x < SCREEN_COLUMN && y >= 0 && y < SCREEN_ROW) {
		if(set_pixel){
				ScreenBuffer[y/8][x] |= (0x01 << (y%8));
		}  
		else{
				ScreenBuffer[y/8][x] &= ~(0x01 << (y%8));
		}
	}
}

/**
 * ************************************************************************
 * @brief ��Ļ����ȡ����ʾ
 * 
 * @param[in] mode  ����
 * 					��	ON	0xA7	����ȫ��
 *  				��	OFF	0xA6	Ĭ�ϴ�ģʽ���������ص���
 * 
 * ************************************************************************
 */
void OLED_DisplayMode(unsigned char mode)
{
	WriteCmd(mode);
}

/**
 * ************************************************************************
 * @brief ��Ļ���ȵ���
 * 
 * @param[in] intensity  ���ȴ�С��0~255����Ĭ��Ϊ0x7f
 * 
 * ************************************************************************
 */
void OLED_IntensityControl(unsigned char intensity)
{
	WriteCmd(0x81);
	WriteCmd(intensity);
}

/**
 * ************************************************************************
 * @brief ȫ������ƫ��ָ������
 * 
 * @param[in] shift_num  ƫ�ƾ��루0~63��
 * 
 * ************************************************************************
 */
void OLED_Shift(unsigned char shift_num)
{
	for(unsigned char i = 0; i < shift_num; i++)
		{
			WriteCmd(0xd3);//������ʾƫ�ƣ���ֱ����ƫ��
			WriteCmd(i);//ƫ����
			HAL_Delay(10);//��ʱʱ��
		}
}


/**
 * ************************************************************************
 * @brief ��Ļ����ˮƽ�����������
 * 
 * @param[in] start_page  	��ʼҳ��	(0~7)
 * @param[in] end_page  	����ҳ��	(0~7)
 * @param[in] direction  	��������
 * 								��		LEFT	0x27
 * 								��		RIGHT	0x26
 * @note �ڿ�ʼҳ���ͽ���ҳ��֮������ݲŻ�������ţ���д����˳�򲻵øı�
 * ************************************************************************
 */
void OLED_HorizontalShift(unsigned char start_page,unsigned char end_page,unsigned char direction)
{
	WriteCmd(0x2e);  //�رչ���

	WriteCmd(direction);//���ù�������
	WriteCmd(0x00);//�����ֽ����ã�Ĭ��Ϊ0x00
	WriteCmd(start_page);//���ÿ�ʼҳ��ַ
	WriteCmd(0x05);//����ÿ����������֮���ʱ������֡Ƶ
	WriteCmd(end_page);//���ý���ҳ��ַ
	WriteCmd(0x00);//�����ֽ����ã�Ĭ��Ϊ0x00
	WriteCmd(0xff);//�����ֽ����ã�Ĭ��Ϊ0xff

	WriteCmd(0x2f);//��������-0x2f�����ù���-0x2e��������Ҫ��д����
}
