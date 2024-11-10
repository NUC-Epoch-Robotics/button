#include "bsp_usart.h"
#include "usart.h"
#include "stm32f1xx_hal.h"
#include "stdio.h"

/*�ο�CSDN
*ԭ�����ӣ�https://blog.csdn.net/m0_63954303/article/details/139794861
*
*/

/*    �������ݲ���    */    
 	uint8_t Rx_data[BUF_SIZE];
	
/* bsp_usart_init�����ڽ��ճ�ʼ������
 * huart	 	��Ҫ��ʼ���Ĵ��ڵľ��
 * huart_type	��Ҫ��ʼ���Ĵ��ڱ�ţ�BSP_USART
 * return		�ɹ�����0��ʧ�ܷ���-1 */
void  bsp_usart_init(	USARTInstance* usart, UART_HandleTypeDef* usart_handle, USART_TRANSFER_MODE TRANSFER_MODE)
{
	
		usart->TRANSFER_MODE = TRANSFER_MODE;
		usart->usart_handle = usart_handle;
}                      
/*
*��usart���еײ��װ
*�Դ����ض���
*/


//����HAL��usart���ͺ�����װ
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
            ; // illegal mode! check your code context! ��鶨��instance�Ĵ���������,���ܳ���ָ��Խ��
        
    }
}

//����HAL��usart���ͺ�����װ
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
            ; // illegal mode! check your code context! ��鶨��instance�Ĵ���������,���ܳ���ָ��Խ��
    
    }
}

/* ���������� ��DMAģʽ�½���һ�����������ݣ�ֱ���յ�Ԥ�����������ݻ��������¼� */
void  Uart_Idle_rcDMA(UART_HandleTypeDef *huart,uint8_t* DataBuff)
{
 HAL_UARTEx_ReceiveToIdle_DMA(&huart1,DataBuff,BUF_SIZE);//BUF_SIZE �˴���дһ�ν��յ�������ݳ���
 __HAL_DMA_DISABLE_IT(&hdma_usart1_rx, DMA_IT_HT);		   // �ֶ��ر�DMA_IT_HT�ж�
}

/*  �������ڽ����ж�  */
void UART_Receive_IT_enable( USARTInstance *usart,uint32_t InterruptDef)
	{
		/* ʹ�ܿ����ж� */
		__HAL_UART_ENABLE_IT(usart->usart_handle, InterruptDef);	
		/* �����һ�ο����жϱ�־����������������� */
		__HAL_UART_CLEAR_IDLEFLAG(usart->usart_handle);	
	}
	





