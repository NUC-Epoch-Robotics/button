#include "FreeRTOS.h"
#include "task.h"
#include "shell.h"

#include "stm32f1xx_hal.h"
#include "usart.h"

#include "shell_port.h"
#include "semphr.h"

/* 1. 创建shell对象，开辟shell缓冲区 */
Shell shell;
char shellBuffer[512];

static SemaphoreHandle_t shellMutex;

/**
 * @brief 用户shell写
 *
 * @param data 数据
 * @param len 数据长度
 *
 * @return short 实际写入的数据长度
 */
short userShellWrite(char *data, unsigned short len)
{
	HAL_UART_Transmit(&huart1, (uint8_t *)data, len, 0x1FF);
	return len;
}

/**
 * @brief 用户shell读
 *
 * @param data 数据
 * @param len 数据长度
 *
 * @return short 实际读取到
 */
short userShellRead(char *data, unsigned short len)
{
	if (HAL_UART_Receive(&huart1, (uint8_t *)data, len, 0x1FF) != HAL_OK)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

/**
 * @brief 用户shell上锁
 *
 * @param shell shell
 *
 * @return int 0
 */
int userShellLock(Shell *shell)
{
	xSemaphoreTake(shellMutex, portMAX_DELAY);
	return 0;
}

/**
 * @brief 用户shell解锁
 *
 * @param shell shell
 *
 * @return int 0
 */
int userShellUnlock(Shell *shell)
{
	xSemaphoreGive(shellMutex);
	return 0;
}

/**
 * @brief 用户shell初始化
 *
 */
void ShellInit(void)
{
	shellMutex = xSemaphoreCreateMutex();

	shell.write = userShellWrite;
	shell.read = userShellRead;
	shell.lock = userShellLock;
	shell.unlock = userShellUnlock;
	shellInit(&shell, shellBuffer, 512);

	    if (xTaskCreate(shellTask, "shell", 256, &shell, -2, NULL) != pdPASS)
	   {
	// logError("shell task creat failed");
	   }
}
