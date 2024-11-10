#include "bsp_usart.h"
#include "usart.h"
#include "stm32f1xx_hal.h"
#include "stdio.h"

/*参考CSDN
*原文链接：https://blog.csdn.net/m0_63954303/article/details/139794861
*
*/

/*    接收数据参数    */    
 	uint8_t Rx_data[BUF_SIZE];
	
/* bsp_usart_init：串口接收初始化函数
 * huart	 	想要初始化的串口的句柄
 * huart_type	想要初始化的串口编号，BSP_USART
 * return		成功返回0，失败返回-1 */
void  bsp_usart_init(	USARTInstance* usart, UART_HandleTypeDef* usart_handle, USART_TRANSFER_MODE TRANSFER_MODE)
{
	
		usart->TRANSFER_MODE = TRANSFER_MODE;
		usart->usart_handle = usart_handle;
}                      
/*
*对usart进行底层封装
*对串口重定向
*/


//基于HAL库usart发送函数封装
 void UartSend(USARTInstance *usart, uint8_t *send_buf, uint16_t send_size,USART_TRANSFER_MODE mode )
{
	//USART_TRANSFER_MODE mode=usart->TRANSFER_MODE ;
    switch (mode)
    {
    case USART_TRANSFER_BLOCKING:
        HAL_UART_Transmit(usart->usart_handle, send_buf, send_size, 100);
        break;
    case USART_TRANSFER_IT:
        HAL_UART_Transmit_IT(usart->usart_handle, send_buf, send_size);
        break;
    case USART_TRANSFER_DMA:
        HAL_UART_Transmit_DMA(usart->usart_handle, send_buf, send_size);
        break;
    default:
        while (1)
            ; // illegal mode! check your code context! 检查定义instance的代码上下文,可能出现指针越界
        
    }
}

//基于HAL库usart发送函数封装
 void UartReceive(USARTInstance *usart, uint8_t *send_buf, uint16_t send_size, USART_TRANSFER_MODE mode)
{
	
	//USART_TRANSFER_MODE mode=usart->TRANSFER_MODE ;
    switch (mode)
    {
    case USART_TRANSFER_BLOCKING:
        HAL_UART_Receive(usart->usart_handle, send_buf, send_size, 100);
        break;
    case USART_TRANSFER_IT:
        HAL_UART_Receive_IT(usart->usart_handle, send_buf, send_size);
        break;
    case USART_TRANSFER_DMA:
        HAL_UART_Receive_DMA(usart->usart_handle, send_buf, send_size);
        break;
		 case USART_TRANSFER_IDLE:
        HAL_UART_Receive_DMA(usart->usart_handle, send_buf, send_size);
        break;
		
    default:
        while (1)
            ; // illegal mode! check your code context! 检查定义instance的代码上下文,可能出现指针越界
    
    }
}

/* 不定长接收 在DMA模式下接收一定数量的数据，直到收到预期数量的数据或发生空闲事件 */
void  Uart_Idle_rcDMA(UART_HandleTypeDef *huart,uint8_t* DataBuff)
{
 HAL_UARTEx_ReceiveToIdle_DMA(&huart1,DataBuff,BUF_SIZE);//BUF_SIZE 此处填写一次接收的最大数据长度
 __HAL_DMA_DISABLE_IT(&hdma_usart1_rx, DMA_IT_HT);		   // 手动关闭DMA_IT_HT中断
}

/*  开启串口接收中断  */
void UART_Receive_IT_enable( USARTInstance *usart,uint32_t InterruptDef)
	{
		/* 使能空闲中断 */
		__HAL_UART_ENABLE_IT(usart->usart_handle, InterruptDef);	
		/* 清除第一次空闲中断标志，方便后续接收数据 */
		__HAL_UART_CLEAR_IDLEFLAG(usart->usart_handle);	
	}
	





