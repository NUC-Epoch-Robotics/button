#include "bsp_iic.h"
#include "i2c.h"
#include "stm32f1xx_hal.h"


static uint8_t idx = 0; // 配合中断以及初始化
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
*@brief 传输模式改变
*@param iic handle
*@param mode iic工作模式
*/
void IIC_Mode_Change(IICInstance* iic ,IIC_Work_Mode mode)
{
	if (iic->Work_Mode != IIC_DMA_MODE && iic->Work_Mode != IIC_IT_MODE && iic->Work_Mode != IIC_BLACK_MODE)
       while (1)
            ; // error mode! 请查看是否正确设置模式，或出现指针越界导致模式被异常修改的情况

	if(iic->Work_Mode !=mode)
  { 
    iic->Work_Mode = mode;
  }
		
}

/*
*@brief IIC发送函数封装
*@param  iic      iic句柄
*@param  pData    发送数据首地址
*@param  Size     数据大小
*@param  IIC_Seq  IIC_SEQ_RELEASE 此次发送数据后释放总线
*                 IIC_SEQ_HOLDON  保持总线占有权不释放，可继续发送数据
*/
void IICSend(IICInstance* iic, uint8_t *pData, uint16_t Size, IIC_Work_Mode IIC_Seq)
{

  switch (iic->Work_Mode)
	{
	 case IIC_BLACK_MODE://阻塞模式
  	 if(IIC_Seq== IIC_SEQ_RELEASE)
    {
      HAL_I2C_Master_Transmit(iic->bsp_iic_Handle ,  iic->reg , pData, Size, iic_clog_time );
    } 
     else if(IIC_Seq== IIC_SEQ_HOLDON)
	  {
	  	 HAL_I2C_Master_Transmit(iic->bsp_iic_Handle ,  iic->reg , pData, Size, iic_clog_time );
	  }
		break;
	case IIC_IT_MODE:   //中断模式
	   if(IIC_Seq== IIC_SEQ_RELEASE)
    {
       HAL_I2C_Master_Seq_Transmit_IT(iic->bsp_iic_Handle ,  iic->reg , pData, Size, I2C_OTHER_AND_LAST_FRAME);
    } 
     else if(IIC_Seq== IIC_SEQ_HOLDON)
	  {
	  	 HAL_I2C_Master_Seq_Transmit_IT(iic->bsp_iic_Handle ,  iic->reg , pData, Size, I2C_OTHER_FRAME );
	  }
		break;
	case IIC_DMA_MODE: //DMA传输
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
			while(1); //状态不在所设模式，停止传输；
		
	}

}
/*
*@brief IIC接收函数封装，
*@param  iic      iic句柄
*@param  pData    接收数据首地址
*@param  Size     数据大小
*@param  IIC_Seq  IIC_SEQ_RELEASE 此次接收数据后释放总线
*                 IIC_SEQ_HOLDON  保持总线占有权不释放，可继续接收数据
*/

void IICReceive(IICInstance* iic, uint8_t *pData, uint16_t Size, IIC_Work_Mode IIC_Seq)
{
  switch (iic->Work_Mode)
	{
	 case IIC_BLACK_MODE://阻塞模式
  	 if(IIC_Seq== IIC_SEQ_RELEASE)
    {
      HAL_I2C_Master_Receive(iic->bsp_iic_Handle ,  iic->reg , pData, Size, iic_clog_time );
    } 
     else if(IIC_Seq== IIC_SEQ_HOLDON)
	  {
	  	HAL_I2C_Master_Receive(iic->bsp_iic_Handle ,  iic->reg , pData, Size, iic_clog_time );
	  }
		break;
	case IIC_IT_MODE:   //中断模式
	   if(IIC_Seq== IIC_SEQ_RELEASE)
    {
      HAL_I2C_Master_Seq_Receive_IT(iic->bsp_iic_Handle ,  iic->reg , pData, Size, I2C_OTHER_AND_LAST_FRAME);
    } 
     else if(IIC_Seq== IIC_SEQ_HOLDON)
	  {
	  	HAL_I2C_Master_Seq_Receive_IT(iic->bsp_iic_Handle ,  iic->reg , pData, Size, I2C_OTHER_FRAME );
	  }
		break;
	case IIC_DMA_MODE: //DMA接收
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
			while(1); //状态不在所设模式，停止传输；
		
	}
}

//对寄存器操作
void IICAccessMem( IICInstance* iic, uint16_t reg,  uint16_t mem_addr, uint8_t *pData, uint16_t Size,uint8_t mem8bit_flag)
{
	 uint16_t bit_flag = mem8bit_flag ? I2C_MEMADD_SIZE_8BIT : I2C_MEMADD_SIZE_16BIT;//判断mem8bit_flag是否为8
switch(iic->Access_Mode)
{
	case IIC_Write_Mem:
		HAL_I2C_Mem_Write(iic->bsp_iic_Handle ,reg, mem_addr,bit_flag, pData ,Size, iic_clog_time);
    break;
   
	case IIC_Read_Mem:
		HAL_I2C_Mem_Read(iic->bsp_iic_Handle ,reg, mem_addr, bit_flag, pData ,Size, iic_clog_time);
    break;
	
	default:
	  while(1); //状态不在所设模式，停止传输；
	  
}
}


/**
 * @brief IIC接收完成回调函数
 *
 * @param hi2c handle
 */
void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    // 如果是当前i2c硬件发出的complete,且dev_address和之前发起接收的地址相同,同时回到函数不为空, 则调用回调函数
    for (uint8_t i = 0; i < idx; i++)
    {
        if (iic_instance[i]->bsp_iic_Handle == hi2c && hi2c->Devaddress == iic_instance[i]->reg)
        {
            if (iic_instance[i]->callback != NULL) // 回调函数不为空
                iic_instance[i]->callback(iic_instance[i]);
            return;
        }
    }
}

void HAL_I2C_MemRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    HAL_I2C_MasterRxCpltCallback(hi2c);
}
