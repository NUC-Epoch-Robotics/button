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
#include "bsp_usart.h"
#include "usart.h"
#include "bsp_usart.h"
#include "stdio.h"
#include "string.h"
#include "KEY.h"
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
 	uint8_t rx_data[BUF_SIZE];
  int RxLine=0; 




/*初始化数据帧，配置所需要的发送命令，发送长度*/
void frameInstance_init(FrameInstance* frame,FrameCommand command)
{
	frame->frame_hand1 = usart_frame_hand1;
	frame->frame_hand2 = usart_frame_hand2;
	frame->frame_hand3 = usart_frame_hand1;
	frame->command     = command;
	frame->frame_end = usart_frame_end;
	
	Uart_Idle_rcDMA(&huart1, DataBuff);
   __HAL_UART_ENABLE_IT(&huart1,UART_IT_IDLE);//启用串口空闲中断	
}
 


/**  把数据帧放在堆栈中  **/
void frame_buf(FrameInstance* frame,uint8_t* Data,int len)
{ 
	memset(Usart_SendBuf,0,sizeof(Usart_SendBuf));
	int i=0,t=0;
  Usart_SendBuf[t++] = frame->frame_hand1;
  Usart_SendBuf[t++] = frame->frame_hand2;
	Usart_SendBuf[t++] = frame->frame_hand3;
  Usart_SendBuf[t++] = frame->command;
	Usart_SendBuf[t++] = len;       
	for( i=t;i<len+t;i++)
	 {
	 Usart_SendBuf[i] = Data[i-t]; 
	 }
  t=len+t;
//  Usart_SendBuf[t++] = frame->CRC16_check(Data, len)>> 8;
//	Usart_SendBuf[t++] = frame->CRC16_check(Data, len); 
  Usart_SendBuf[t] = usart_frame_end;
	 t=t+2;         //停止位

  HAL_UART_Transmit_DMA(&huart1, Usart_SendBuf,t); 
}
 
/* 不定长接收 在DMA模式下接收一定数量的数据，直到收到预期数量的数据或发生空闲事件*/
void  Uart_Idle_rcDMA(UART_HandleTypeDef *huart,uint8_t* DataBuff)
{
 HAL_UARTEx_ReceiveToIdle_DMA(&huart1,DataBuff,BUF_SIZE);//BUF_SIZE 此处填写一次接收的最大数据长度
 __HAL_DMA_DISABLE_IT(&hdma_usart1_rx, DMA_IT_HT);		   // 手动关闭DMA_IT_HT中断
}


/* DMA串口发送函数封装 */
void Uart_TxDMA(UART_HandleTypeDef *huart,uint8_t* Usart_SendBuf)
{
 HAL_UART_Transmit_DMA(&huart1, Usart_SendBuf, sizeof(Usart_SendBuf));
}

/*  空闲回调函数  */
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart,uint16_t Size)
{
	printf("Size=%u\r\n ",Size);
	uint8_t cnt =0;
	int commit;//接收传来的指令      
	memset(rx_data,0,sizeof(rx_data));

 if( huart->Instance == USART1)
   {   
				if(DataBuff[0] == usart_frame_hand1&&
					 DataBuff[1] == usart_frame_hand2&& 
					 DataBuff[2] == usart_frame_hand1  )   //检测是否是包头  DataBuff[%d]=
			 {  
		  	//  commit= DataBuff[3];                 //接收发来的命令，用来执行不同的代码，现在没写以后补充
					cnt =  DataBuff[4];
			
					//接收发来的数据
			for(int i=5;i<cnt+5;i++)
			 {
				rx_data[i-5]= DataBuff[i];
			 } 
		  	//CRC检验 && 检测是否是包尾
			if(DataBuff[cnt+5] == usart_frame_end ) 
				{ 
					User_rx_Callback(rx_data);					
				} 
			else  
			 {
					memset(rx_data,0,sizeof(rx_data));
			 } 
	  } 
  }
}



/*接收回调*/
void User_rx_Callback(uint8_t* data)
{
	if( data[0]==0x11){

	LED1();
	
	}
	if( data[1]== 0x22){

	//代码
	}
}



void HAL_UART_ErrorCallback(UART_HandleTypeDef * huart)
{
    if(huart->Instance == USART1)
    {
			
		  Uart_Idle_rcDMA(&huart1, DataBuff); // 接收发生错误后重启
      memset(DataBuff, 0, sizeof(DataBuff));
        
    }
}


/*   CRC校验   */
uint16_t CRC16_Check(const uint8_t *data,uint8_t len)
{
    uint16_t CRC16 = 0xFFFF;
    uint8_t state,i,j;
    for(i = 0; i < len; i++ )
    {
        CRC16 ^= data[i];
        for( j = 0; j < 8; j++)
        {
            state = CRC16 & 0x01;
            CRC16 >>= 1;
            if(state)
            {
                CRC16 ^= 0xA001;
            }
        }
    }
    return CRC16;
}
