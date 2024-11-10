#ifndef __BSP_IIC_H
#define __BSP_IIC_H

#include "gpio.h"
#include "tim.h"

#define IIC_DEVICE_CNT 2   // C板引出了I2C2和I2C3
#define MX_IIC_SLAVE_CNT 8 // 最大从机数目,根据需要修改
typedef enum
{
IIC_BLACK_MODE=0,//阻塞模式
IIC_IT_MODE,     //中断模式
IIC_DMA_MODE,     //DMA传输

}IIC_Work_Mode;

typedef enum
{
	IIC_Write_Mem = 0,//写寄存器
	IIC_Read_Mem,     //读寄存器
	
}IIC_Access_Mode;

typedef enum
{
	 IIC_SEQ_RELEASE= 0,     // 完成传输后释放总线占有权,这是默认的传输方式
   IIC_SEQ_HOLDON ,        // 保持总线占有权不释放,只支持IT和DMA模式
	
} IIC_Seq_Mode;


typedef struct iic_temp_s
{
	I2C_HandleTypeDef* bsp_iic_Handle;
	
	GPIO_TypeDef*   bsp_iic_port;//IIC GPIO类型
	uint16_t        bsp_iic_pin; //IIC GPIO引脚

	uint16_t reg;                // 暂时只支持7位地址(还有一位是读写位),注意不需要左移
	
  IIC_Work_Mode   Work_Mode;   //IIC工作模式
	IIC_Seq_Mode    Seq_Mode ;   //是否释放总线 
	IIC_Access_Mode  Access_Mode;
	 void (*callback)(struct iic_temp_s *); //接收完成后的回调函数
	 void *id; // 用于标识i2c instance
} IICInstance;

int bsp_iic_init(	IICInstance* iic,I2C_HandleTypeDef* bsp_iic_Handle ,IIC_Work_Mode   Work_Mode );
 
/*
*@brief 传输模式改变
*@param iic handle
*@param mode iic工作模式
*/
void IIC_Mode_Change(IICInstance* iic ,IIC_Work_Mode mode);

/*
*@brief IIC发送函数封装
*@param  iic      iic结构体指针
*@param  pData    发送数据首地址
*@param  Size     数据大小
*@param  IIC_Seq  IIC_SEQ_RELEASE 此次发送数据后释放总线
*                 IIC_SEQ_HOLDON  保持总线占有权不释放，可继续发送数据
*/
void IICSend(IICInstance* iic, uint8_t *pData, uint16_t Size, IIC_Work_Mode IIC_Seq);

/*
*@brief IIC接收函数封装
*@param  iic      iic结构体指针
*@param  pData    接收数据首地址
*@param  Size     数据大小
*@param  IIC_Seq  IIC_SEQ_RELEASE 此次接收数据后释放总线
*                 IIC_SEQ_HOLDON  保持总线占有权不释放，可继续接收数据
*/
void IICReceive(IICInstance* iic, uint8_t *pData, uint16_t Size, IIC_Work_Mode IIC_Seq);
/*
*@brief 对指定寄存器进行读写
*@param  iic           iic句柄
*@param  mem_addr      要读取的从机内存地址,目前只支持8位地址
*@param  pData         接收数据首地址
*@param  Size          要读取或写入的数据长度
*@param  mem8bit_flag  从机内存地址是否为8位
*/
void IICAccessMem( IICInstance* iic,   uint16_t reg,uint16_t mem_addr ,uint8_t *pData , uint16_t Size,uint8_t mem8bit_flag);

#endif

