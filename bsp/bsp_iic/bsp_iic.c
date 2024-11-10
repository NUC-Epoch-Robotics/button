#include "bsp_iic.h"
#include "i2c.h"
#include "stm32f1xx_hal.h"


static uint8_t idx = 0; // ����ж��Լ���ʼ��
static IICInstance *iic_instance[IIC_DEVICE_CNT] = {NULL};
#define iic_clog_time 100


int bsp_iic_init(	IICInstance* iic,I2C_HandleTypeDef* bsp_iic_Handle ,IIC_Work_Mode   Work_Mode )
{
	if(iic->bsp_iic_Handle!=NULL )
	{
		iic->Work_Mode = Work_Mode;
		iic->bsp_iic_Handle = bsp_iic_Handle;
	  return 0;
	 }
	return -1;
}    



/*
*@brief ����ģʽ�ı�
*@param iic handle
*@param mode iic����ģʽ
*/
void IIC_Mode_Change(IICInstance* iic ,IIC_Work_Mode mode)
{
	if (iic->Work_Mode != IIC_DMA_MODE && iic->Work_Mode != IIC_IT_MODE && iic->Work_Mode != IIC_BLACK_MODE)
       while (1)
            ; // error mode! ��鿴�Ƿ���ȷ����ģʽ�������ָ��Խ�絼��ģʽ���쳣�޸ĵ����

	if(iic->Work_Mode !=mode)
  { 
    iic->Work_Mode = mode;
  }
		
}

/*
*@brief IIC���ͺ�����װ
*@param  iic      iic���
*@param  pData    ���������׵�ַ
*@param  Size     ���ݴ�С
*@param  IIC_Seq  IIC_SEQ_RELEASE �˴η������ݺ��ͷ�����
*                 IIC_SEQ_HOLDON  ��������ռ��Ȩ���ͷţ��ɼ�����������
*/
void IICSend(IICInstance* iic, uint8_t *pData, uint16_t Size, IIC_Work_Mode IIC_Seq)
{

  switch (iic->Work_Mode)
	{
	 case IIC_BLACK_MODE://����ģʽ
  	 if(IIC_Seq== IIC_SEQ_RELEASE)
    {
      HAL_I2C_Master_Transmit(iic->bsp_iic_Handle ,  iic->reg , pData, Size, iic_clog_time );
    } 
     else if(IIC_Seq== IIC_SEQ_HOLDON)
	  {
	  	 HAL_I2C_Master_Transmit(iic->bsp_iic_Handle ,  iic->reg , pData, Size, iic_clog_time );
	  }
		break;
	case IIC_IT_MODE:   //�ж�ģʽ
	   if(IIC_Seq== IIC_SEQ_RELEASE)
    {
       HAL_I2C_Master_Seq_Transmit_IT(iic->bsp_iic_Handle ,  iic->reg , pData, Size, I2C_OTHER_AND_LAST_FRAME);
    } 
     else if(IIC_Seq== IIC_SEQ_HOLDON)
	  {
	  	 HAL_I2C_Master_Seq_Transmit_IT(iic->bsp_iic_Handle ,  iic->reg , pData, Size, I2C_OTHER_FRAME );
	  }
		break;
	case IIC_DMA_MODE: //DMA����
		 if(IIC_Seq== IIC_SEQ_RELEASE)
    {
       HAL_I2C_Master_Seq_Transmit_DMA(iic->bsp_iic_Handle ,  iic->reg , pData, Size, I2C_OTHER_AND_LAST_FRAME );
    } 
     else if(IIC_Seq== IIC_SEQ_HOLDON)
	  {
	  	 HAL_I2C_Master_Seq_Transmit_DMA(iic->bsp_iic_Handle ,  iic->reg , pData, Size, I2C_OTHER_FRAME );
	  }
		break;
		default:
			while(1); //״̬��������ģʽ��ֹͣ���䣻
		
	}

}
/*
*@brief IIC���պ�����װ��
*@param  iic      iic���
*@param  pData    ���������׵�ַ
*@param  Size     ���ݴ�С
*@param  IIC_Seq  IIC_SEQ_RELEASE �˴ν������ݺ��ͷ�����
*                 IIC_SEQ_HOLDON  ��������ռ��Ȩ���ͷţ��ɼ�����������
*/

void IICReceive(IICInstance* iic, uint8_t *pData, uint16_t Size, IIC_Work_Mode IIC_Seq)
{
  switch (iic->Work_Mode)
	{
	 case IIC_BLACK_MODE://����ģʽ
  	 if(IIC_Seq== IIC_SEQ_RELEASE)
    {
      HAL_I2C_Master_Receive(iic->bsp_iic_Handle ,  iic->reg , pData, Size, iic_clog_time );
    } 
     else if(IIC_Seq== IIC_SEQ_HOLDON)
	  {
	  	HAL_I2C_Master_Receive(iic->bsp_iic_Handle ,  iic->reg , pData, Size, iic_clog_time );
	  }
		break;
	case IIC_IT_MODE:   //�ж�ģʽ
	   if(IIC_Seq== IIC_SEQ_RELEASE)
    {
      HAL_I2C_Master_Seq_Receive_IT(iic->bsp_iic_Handle ,  iic->reg , pData, Size, I2C_OTHER_AND_LAST_FRAME);
    } 
     else if(IIC_Seq== IIC_SEQ_HOLDON)
	  {
	  	HAL_I2C_Master_Seq_Receive_IT(iic->bsp_iic_Handle ,  iic->reg , pData, Size, I2C_OTHER_FRAME );
	  }
		break;
	case IIC_DMA_MODE: //DMA����
		 if(IIC_Seq== IIC_SEQ_RELEASE)
    {
      HAL_I2C_Master_Seq_Receive_DMA(iic->bsp_iic_Handle ,  iic->reg , pData, Size, I2C_OTHER_AND_LAST_FRAME );
    } 
     else if(IIC_Seq== IIC_SEQ_HOLDON)
	  {
	  	HAL_I2C_Master_Seq_Receive_DMA(iic->bsp_iic_Handle ,  iic->reg , pData, Size, I2C_OTHER_FRAME );
	  }
		break;
		default:
			while(1); //״̬��������ģʽ��ֹͣ���䣻
		
	}
}

//�ԼĴ�������
void IICAccessMem( IICInstance* iic, uint16_t reg,  uint16_t mem_addr, uint8_t *pData, uint16_t Size,uint8_t mem8bit_flag)
{
	 uint16_t bit_flag = mem8bit_flag ? I2C_MEMADD_SIZE_8BIT : I2C_MEMADD_SIZE_16BIT;//�ж�mem8bit_flag�Ƿ�Ϊ8
switch(iic->Access_Mode)
{
	case IIC_Write_Mem:
		HAL_I2C_Mem_Write(iic->bsp_iic_Handle ,reg, mem_addr,bit_flag, pData ,Size, iic_clog_time);
    break;
   
	case IIC_Read_Mem:
		HAL_I2C_Mem_Read(iic->bsp_iic_Handle ,reg, mem_addr, bit_flag, pData ,Size, iic_clog_time);
    break;
	
	default:
	  while(1); //״̬��������ģʽ��ֹͣ���䣻
	  
}
}


/**
 * @brief IIC������ɻص�����
 *
 * @param hi2c handle
 */
void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    // ����ǵ�ǰi2cӲ��������complete,��dev_address��֮ǰ������յĵ�ַ��ͬ,ͬʱ�ص�������Ϊ��, ����ûص�����
    for (uint8_t i = 0; i < idx; i++)
    {
        if (iic_instance[i]->bsp_iic_Handle == hi2c && hi2c->Devaddress == iic_instance[i]->reg)
        {
            if (iic_instance[i]->callback != NULL) // �ص�������Ϊ��
                iic_instance[i]->callback(iic_instance[i]);
            return;
        }
    }
}

void HAL_I2C_MemRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    HAL_I2C_MasterRxCpltCallback(hi2c);
}
