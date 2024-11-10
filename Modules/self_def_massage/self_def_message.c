/**
  ******************************************************************************
  * @file     self_def_massage.c
  * @author   songzi
  * @version  
  * @date     
  * @brief   自定义串口通信 定义一个通信协议 实现不定长接收
  ******************************************************************************
  * @attention
  * 
  *
  *
  * 
  ******************************************************************************
  */ 
/* Includes -------------------------------------------------------------------*/
#include "self_def_massage.h"
#include "ring_buffer.h"
#include "bsp_usart.h"
#include "usart.h"
#include "bsp_usart.h"
#include "stdio.h"
#include "string.h"
#include "KEY.h"
#include "crc.h"
#include "string.h"
/* Private  typedef -----------------------------------------------------------*/
/* Private  define ------------------------------------------------------------*/




/* Private  macro -------------------------------------------------------------*/
extern DMA_HandleTypeDef hdma_usart1_rx;
/* Private  variables ---------------------------------------------------------*/
void  uart_rcDMA(UART_HandleTypeDef *huart,uint8_t *DataBuff);
/* Extern   variables ---------------------------------------------------------*/
/* Extern   function prototypes -----------------------------------------------*/
/* Private  function prototypes -----------------------------------------------*/
/* Private  functions ---------------------------------------------------------*/

uint32_t CRC16_Check( uint32_t *data,uint32_t len);

/**
  * @brief  
  * @param 
  * @param 
  * @retval 
	
  */
/*    发送数据参数    */
 uint8_t Usart_SendBuf[USART_PACKAGE_LEN];
 
/*    接收数据参数    */    
  uint8_t DataBuff[BUF_SIZE]; 
 
  
  USARTInstance Uart_frame;

/*初始化数据帧，配置所需要的发送命令，发送长度*/
void frameInstance_init(FrameInstance* frame,FrameCommand command)
{ 
  bsp_usart_init(&Uart_frame,&huart1,USART_TRANSFER_DMA);
	frame->frame_hand1 = usart_frame1_hand;
	frame->frame_hand2 = usart_frame2_hand;
	frame->frame_hand3 = usart_frame1_hand;
	frame->command     = command;
	frame->frame_end = usart_frame_end;
	//frame->crc_check=CRC16_Check;


	UART_Receive_IT_enable(&Uart_frame ,UART_IT_IDLE);//启用串口空闲中断	
		Uart_Idle_rcDMA(Uart_frame.usart_handle ,DataBuff);
}



/**  把数据帧放在堆栈中  **/
void frame_buf(FrameInstance* frame,uint8_t* Data,int len)
{ 
//	uint32_t crc=0;
//	uint32_t data_32=0;
	memset(Usart_SendBuf,0,sizeof(Usart_SendBuf));
	int t=0;
  Usart_SendBuf[t++] = frame->frame_hand1;
  Usart_SendBuf[t++] = frame->frame_hand2;
	Usart_SendBuf[t++] = frame->frame_hand3;
  Usart_SendBuf[t++] = frame->command;
	Usart_SendBuf[t++] = len;     
  int i=0;
	for( i=t;i<len+t;i++)
	 {
	 Usart_SendBuf[i] = Data[i-t]; 
	 }
	
  t=len+t;
	//memcpy(&data_32, Data, 4);
//	crc=frame->crc_check(&data_32, t);//不注释会跑飞
	//printf("crc_send=%u\r\n",crc);
//  Usart_SendBuf[t++] = (crc>> 24)& 0xFF;
//	Usart_SendBuf[t++] = (crc>> 16)& 0xFF;
//	Usart_SendBuf[t++] = (crc>> 8)& 0xFF;
//	Usart_SendBuf[t++] = crc & 0xFF; 
  Usart_SendBuf[t] = usart_frame_end;
  t=t+2;                                            //停止位
//	 for(int i=0;i<t-1;i++)
//	 {
//  printf("Usart_SendBuf[%d]=%u\r\n",i,Usart_SendBuf[i]);
//	 }
  UartSend(&Uart_frame, Usart_SendBuf,t,USART_TRANSFER_DMA ); //DMA传输
}


uint32_t CRC16_Check( uint32_t *data,uint32_t len)
{
	uint32_t temp=0;
	temp=HAL_CRC_Calculate(&hcrc,data,len);
	 
	return temp;
}
/*   CRC校验   */
//uint16_t CRC16_Check(const uint8_t *data,uint8_t len)
//{
////	HAL_CRC_Calculate();
//    uint16_t CRC16 = 0xFFFF;
//    uint8_t state,i,j;
//    for(i = 0; i < len; i++ )
//    {
//        CRC16 ^= data[i];
//        for( j = 0; j < 8; j++)
//        {
//            state = CRC16 & 0x01;
//            CRC16 >>= 1;
//            if(state)
//            {
//                CRC16 ^= 0xA001;
//            }
//        }
//    }
//   return CRC16;
//}
 

void HAL_UART_ErrorCallback(UART_HandleTypeDef * huart)
{
    if(huart->Instance == USART1)
    {
		  Uart_Idle_rcDMA(Uart_frame.usart_handle, DataBuff); // 接收发生错误后重启
      memset(DataBuff, 0, sizeof(DataBuff));
    }
}



