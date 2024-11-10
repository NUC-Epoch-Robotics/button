#include "button_motor.h"
#include "freertos.h"
#include "task.h"
#include "self_def_massage.h"
#include "KEY.h"
#include "multi_button.h"
#include "OLED_IIC_Config.h"
#include "OLED_Function.h"
#include "OLED_Front.h"
#include "Buzzer.h"
#include "tim.h"
#include "stdio.h"
#include "usart.h"
#include "ring_buffer.h"
#include "cmsis_os.h"
#include "queue.h"
#include "elog.h"
#include "elog_cfg.h"
#include "event_groups.h"
#include "shell_port.h"

#define LOG_TAG         "Button_mottor"
/*************** freeRTOS任务与队列 ****************/
extern osThreadId LED1_taskHandle;
extern osThreadId LED2_taskHandle;
extern osThreadId LED3_taskHandle;
//extern osEventFlagsId_t EventKey1Handle;
/* 队列 */
extern osMessageQId queuekey1Handle;
/** 事件组宏定义 **/
#define Event_0 (1<<0)
#define Event_1 (1<<1)
#define Event_2 (1<<2)

/*********串口发送变量参数*****************/
FrameInstance frame1;
uint8_t Data1[18];
	
/*********串口接收变量参数*****************/
extern uint8_t DataBuff[BUF_SIZE]; 
stRingBuff Ring_Buff;            //循环数组
uint8_t ringbuff[RING_BUFF_SIZE];

/*********按键变量*****************/
//extern	KEY_Configure_TypeDef KeyCfg;	
struct Button button1;
struct Button button2;
struct Button button3;
struct Button button4;
struct Button button5;
struct Button button6;
int t=0;

/*******iic************/
#define ADDR_24LCxx_Write 0xA0
#define ADDR_24LCxx_Read 0xA1
#define BufferSize 256
uint8_t WriteBuffer=0;

uint16_t i;
#include "i2c.h"

void KEY_Init(void)
{

	button_init(&button1, read_button1_GPIO, 0);
	button_init(&button2, read_button2_GPIO, 0);
	button_init(&button3, read_button3_GPIO, 0);
	button_init(&button4, read_button4_GPIO, 0);
	button_init(&button5, read_button5_GPIO, 0);
	button_init(&button6, read_button6_GPIO, 0);

	button_attach(&button1, DOUBLE_CLICK, button1_callback);
	button_attach(&button1, LONG_PRESS_START, button1_callback);
	button_attach(&button1, SINGLE_CLICK, button1_callback);
	button_start(&button1);

	button_attach(&button2, DOUBLE_CLICK, button2_callback);
	button_attach(&button2, LONG_PRESS_START, button2_callback);
	button_attach(&button2, SINGLE_CLICK, button2_callback);
	button_start(&button2);

	button_attach(&button3, DOUBLE_CLICK, button3_callback);
	button_attach(&button3, LONG_PRESS_START, button3_callback);
	button_attach(&button3, SINGLE_CLICK, button3_callback);
	button_start(&button3);

	button_attach(&button4, DOUBLE_CLICK, button4_callback);
	button_attach(&button4, LONG_PRESS_START, button4_callback);
	button_attach(&button4, SINGLE_CLICK, button4_callback);
	button_start(&button4);

	button_attach(&button5, DOUBLE_CLICK, button5_callback);
	button_attach(&button5, LONG_PRESS_START, button5_callback);
	button_attach(&button5, SINGLE_CLICK, button5_callback);
	button_start(&button5);

	button_attach(&button6, DOUBLE_CLICK, button6_callback);
	button_attach(&button6, LONG_PRESS_START, button6_callback);
	button_attach(&button6, SINGLE_CLICK, button6_callback);
	button_start(&button6);
}



//放初始化的代码   之后在freertos中调用完成初始化
void Buttonmotorinit()
{

  RingBufferinit(&Ring_Buff,ringbuff,RING_BUFF_SIZE );
	Data1[0]=0x11;
	Data1[1]=0x12;
	Data1[2]=0x13;
	Data1[3]=0x21;
	Data1[4]=0x22;
	Data1[5]=0x23;
	Data1[6]=0x31;
	Data1[7]=0x32;
	Data1[8]=0x33;
	Data1[9]=0x41;
	Data1[10]=0x42;
	Data1[11]=0x43;
	Data1[12]=0x51;
	Data1[13]=0x52;
	Data1[14]=0x53;
	Data1[15]=0x61;
	Data1[16]=0x62;
	Data1[17]=0x63;

   

 	KEY_Init();
  OLED_Init();
  Buzzer_on();
//  HAL_TIM_Base_Start_IT(&htim2); //使能定时器中断
//	HAL_TIM_Base_Start(&htim2);  //启动定时器
  frameInstance_init(&frame1,usart_W_DATA);
  easylogger_init();
	//ShellInit();
}



//freeRTOS执行按键任务
/***   执行按键任务  ***/
void button_task()
{
	TickType_t preTime;
	preTime = xTaskGetTickCount();
	
		while(1)
		{
		//执行任务
//			vTaskDelay(20);
			button_ticks();
//			vTaskDelay(20);
			vTaskDelayUntil(&preTime,5);
			
		}
}
/***   执行蜂鸣器任务  ***/
void buzzer_task()
{	
	  int i=0;
  osThreadSuspend(LED1_taskHandle);//单个任务挂起
	osThreadSuspend(LED2_taskHandle);//单个任务挂起
	osThreadSuspend(LED3_taskHandle);//单个任务挂起
		while(1)
		{
			BaseType_t res;
			res=xQueueReceive(queuekey1Handle, &i, NULL);
			    if ( res== pdPASS) {
            switch(i)
						{
					//		log_i("buzzer_task");
							case 1:
								osThreadSuspend(LED2_taskHandle);//单个任务挂起
	            	osThreadSuspend(LED3_taskHandle);//单个任务挂起
								osThreadResume(LED1_taskHandle);
//							log_i("任务1恢复，其余任务挂起");
//							printf("任务1恢复，其余任务挂起\r\n");
							
							break;
							case 2:
								osThreadSuspend(LED1_taskHandle);//单个任务挂起
	            	osThreadSuspend(LED3_taskHandle);//单个任务挂起
								osThreadResume(LED2_taskHandle);	
//								log_i("任务1恢复，其余任务挂起");
							break;
							case 3:
								osThreadSuspend(LED2_taskHandle);//单个任务挂起
		            osThreadSuspend(LED1_taskHandle);//单个任务挂起
								osThreadResume(LED3_taskHandle);
//								log_i("任务1恢复，其余任务挂起");
							}
						}

							vTaskDelay(20);
          }
				
					}


/*   执行LED */
void led1_task()
{
	//EventBits_t myEventBits_1=0;
		while(1)
		{
		//	myEventBits_1=xEventGroupWaitBits(EventKey1Handle,Event_0,pdTRUE,pdTRUE,portMAX_DELAY);
//			if(myEventBits_1 & Event_0)
//			{
			 log_i("led1_run");
			  HAL_GPIO_TogglePin (LED1_GPIO_Port,LED1_Pin);
//	 		  printf("电平反转1\r\n");
	     	vTaskDelay(200);
		}
}
		
void led2_task()
{
	
		while(1)
		{
   log_i("led2_run");
		HAL_GPIO_TogglePin(LED2_GPIO_Port,LED2_Pin);
		vTaskDelay(200);
			}
}


void led3_task()
{
		while(1)
		{
	  HAL_GPIO_TogglePin (LED3_GPIO_Port,LED3_Pin);
	 log_i("led3_run");
		vTaskDelay(200);
			}
}




/*串口接收回调*/
/* 主控板可在这使用挂起函数使任务单个执行*/
/*通过构造队列，接收到数据后写队列，之后在单独的一个任务中进行任务挂起*/
/****   使用事件组控制任务单个或多个执行    *****/
void User_rx_Callback(uint8_t data)
{
	uint8_t queue_flag=0;
	if( data==0x11)
	 {
	  //代码
		queue_flag=1;

		
		/* 写队列 */
		xQueueSendFromISR(queuekey1Handle, &queue_flag, NULL);
	 }
	if( data== 0x12)
	 {
	  //代码
		queue_flag=2;

		/* 写队列 */
		xQueueSendFromISR(queuekey1Handle, &queue_flag, NULL);
	 }
	if( data== 0x13)
	 {
	  //代码
		queue_flag=3;

		/* 写队列 */
		xQueueSendFromISR(queuekey1Handle, &queue_flag, NULL);
 	 }
}

/*  空闲回调函数  */
/*主板任务多的话可使用commit发送不同的命令来操控更多的任务实现不同功能*/
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart,uint16_t Size)
{
//	log_i("EventCallback");
//	printf("1111\r\n");

	uint8_t cnt =0;
	uint8_t ring_data[RING_BUFF_SIZE];	
//	log_i("Hello easylogger!");
//	uint32_t data_32;
//	uint32_t crc;
//	int commit;//接收传来的指令      
	
	
	WriteRingBuffer(&Ring_Buff,DataBuff, DataBuff[4]+6);
	memset(ring_data,0,sizeof(ring_data));
	ReadRingBuffer(&Ring_Buff,ring_data , DataBuff[4]+6);   
//	 for(int i=0;i<8;i++)
//	 {
//  printf("Usart_SendBuf[%d]=%u\r\n",i,ring_data[i]);
//	 }
	 
 if( huart->Instance == USART1)
   {   
//		 log_i("222");
				if(ring_data[0] == usart_frame_hand1&&
					 ring_data[1] == usart_frame_hand2&& 
					 ring_data[2] == usart_frame_hand1  )   //检测是否是包头  //DataBuff[%d]=	 
			 {  
		  	//  commit= DataBuff[3];                 //接收发来的命令，用来执行不同的代码，现在没写以后补充
			cnt = ring_data[4];
			cnt+=4;
			 //CRC检验 && 检测是否是包尾
				// 	memcpy(&data_32, ring_data, 4);
			//	 crc=CRC16_Check(&data_32, cnt+1);
			//   data_32 = ((uint32_t)ring_data[cnt+1]<< 24)|((uint32_t)ring_data[cnt+2]<<16)|((uint32_t)ring_data[cnt+3]<<8)|(uint32_t)ring_data[cnt+4];
			if(ring_data[cnt+1] == usart_frame_end ) 
				{ 
					  for(int i=0;i<=cnt;i++){
			      uint8_t data= ring_data[i+5];
						User_rx_Callback(data);		
					}						
				} 
			else  
			 {
					memset(ring_data,0,sizeof(ring_data));
			 } 
	  } 
  }
}



void button1_callback(void *button)
{
	uint32_t btn_event_val;
	
	btn_event_val = get_button_event((struct Button *)button);
	switch (btn_event_val)
	{
	case SINGLE_CLICK:
//		log_i("11");
  frame_buf(&frame1,&Data1[0],1);
	////		xEventGroupSetBitsFromISR(EventKey1Handle ,Event_0 ,NULL);	
	  break;
	
	case DOUBLE_CLICK:
  frame_buf(&frame1,&Data1[1],1);	
//	log_i("22");
  	break;
	
	case LONG_PRESS_START:
  frame_buf(&frame1,&Data1[2],1);
//  	log_i("33");
  	break;
	
	}
}


void button2_callback(void *button)
{
	uint32_t btn_event_val;

	btn_event_val = get_button_event((struct Button *)button);

	switch (btn_event_val)
	{
	case SINGLE_CLICK:
  frame_buf(&frame1,&Data1[3],1);
		break;

	case DOUBLE_CLICK:
		  frame_buf(&frame1,&Data1[4],1);
		break;

	case LONG_PRESS_START:
	  frame_buf(&frame1,&Data1[5],1);
		break;

	}
}

void button3_callback(void *button)
{
	uint32_t btn_event_val;

	btn_event_val = get_button_event((struct Button *)button);

	switch (btn_event_val)
	{
	case SINGLE_CLICK:
  frame_buf(&frame1,&Data1[6],1);
		break;
  
	case DOUBLE_CLICK:
  frame_buf(&frame1,&Data1[7],1);
		break;

	case LONG_PRESS_START:
  frame_buf(&frame1,&Data1[8],1);
		break;


	}
}

void button4_callback(void *button)
{
	uint32_t btn_event_val;

	btn_event_val = get_button_event((struct Button *)button);

	switch (btn_event_val)
	{
	case SINGLE_CLICK:
  frame_buf(&frame1,&Data1[9],1);
		break;

	case DOUBLE_CLICK:
  frame_buf(&frame1,&Data1[10],1);
		break;

	case LONG_PRESS_START:
  frame_buf(&frame1,&Data1[11],1);
		break;


	}
}

void button5_callback(void *button)
{
	uint32_t btn_event_val;

	btn_event_val = get_button_event((struct Button *)button);

	switch (btn_event_val)
	{

	case SINGLE_CLICK:
  frame_buf(&frame1,&Data1[12],1);
		break;

	case DOUBLE_CLICK:
  frame_buf(&frame1,&Data1[13],1);
		break;

	case LONG_PRESS_START:
		  frame_buf(&frame1,&Data1[14],1);
		break;
	}
}

void button6_callback(void *button)
{
	uint32_t btn_event_val;

	btn_event_val = get_button_event((struct Button *)button);

	switch (btn_event_val)
	{
	case SINGLE_CLICK:
  frame_buf(&frame1,&Data1[15],1);
		break;

	case DOUBLE_CLICK:
	  frame_buf(&frame1,&Data1[16],1);
		break;

	case LONG_PRESS_START:
	  frame_buf(&frame1,&Data1[17],1);
		break;

	}
}




