/*
 * This file is part of the EasyLogger Library.
 *
 * Copyright (c) 2015, Armink, <armink.ztl@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * 'Software'), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * Function: Portable interface for each platform.
 * Created on: 2015-04-28
 */
 
#include <elog.h>
#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

//! ��־�����ź������
static SemaphoreHandle_t LogMutexSemaphore = NULL; 


/**
 * EasyLogger port initialize
 *
 * @return result
 */
ElogErrCode elog_port_init(void) {
    ElogErrCode result = ELOG_NO_ERR;

    /* add your code here */
     //! ���������ź�ֵ
    LogMutexSemaphore = xSemaphoreCreateMutex();
    if (LogMutexSemaphore == NULL) {
        printf("elog sem create fail\r\n");
        result =  ELOG_SEM_FAIL;	//!< ע�⣺ElogErrCode ö����û��������ͣ���Ҫ����һ����������
    }
    return result;
}

/**
 * EasyLogger port deinitialize
 *
 */
void elog_port_deinit(void) {

    /* add your code here */

}

/**
 * output log port interface
 *
 * @param log output of log
 * @param size log size
 */
void elog_port_output(const char *log, size_t size) {
    
    /* add your code here */
     printf("%.*s ", size, log);
}

/**
 * output lock
 */
void elog_port_output_lock(void) {
    
    /* add your code here */
     if (NULL != LogMutexSemaphore)
    {
        xSemaphoreTake(LogMutexSemaphore, portMAX_DELAY); //!< �ȴ������ź���
    }   
}

/**
 * output unlock
 */
void elog_port_output_unlock(void) {
    
    /* add your code here */
      if (NULL != LogMutexSemaphore)
    {
        xSemaphoreGive(LogMutexSemaphore); //!< ���ͻ����ź���
    } 
}

/**
 * get current time interface
 *
 * @return current time
 */
const char *elog_port_get_time(void) {
    
    /* add your code here */
   static char cur_system_time[16] = { 0 };

#if (INCLUDE_xTaskGetSchedulerState  == 1 )
    if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)
    {
#endif
        TickType_t tick = xTaskGetTickCount();
        snprintf(cur_system_time, 16, "%d.%.3d", (tick / configTICK_RATE_HZ), tick % configTICK_RATE_HZ);
			 
#if (INCLUDE_xTaskGetSchedulerState  == 1 )
    }
#endif
	 return cur_system_time;

}

/**
 * get current process name interface
 *
 * @return current process name
 */
const char *elog_port_get_p_info(void) {
    
    /* add your code here */
    return "";
}

/**
 * get current thread name interface
 *
 * @return ��������
 */
const char *elog_port_get_t_info(void) {
    
    /* add your code here */
    #if (INCLUDE_xTaskGetSchedulerState  == 1 )
    if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)
    {
#endif
        return pcTaskGetName(xTaskGetCurrentTaskHandle());
#if (INCLUDE_xTaskGetSchedulerState  == 1 )
    }
#endif 

    return "";

}
/*******************************************************************************
* Function Name  : static_set_output_log_format
* Description    : ��̬������־��ӡ��ʽ
* Input          : None
* Output         : None
* Return         : None
* example        ; log_a("Hello EasyLogger!");		//����
				   log_e("Hello EasyLogger!");		//����
				   log_w("Hello EasyLogger!");		//����
				   log_i("Hello EasyLogger!");		//��Ϣ
				   log_d("Hello EasyLogger!");		//����
				   log_v("Hello EasyLogger!");		//��ϸ
*******************************************************************************/
void easylogger_init(void)
{
    /* init Easylogger */
	elog_init();

	/* set EasyLogger log format */
   /* ���ԣ������������ */
    elog_set_fmt(ELOG_LVL_ASSERT, ELOG_FMT_ALL);

    /* ���������־������־��ǩ����־ʱ�����־�߳���Ϣ */
    elog_set_fmt(ELOG_LVL_ERROR, ELOG_FMT_LVL | ELOG_FMT_TAG | ELOG_FMT_TIME| ELOG_FMT_T_INFO);

    /* ���棺�����־������־��ǩ����־ʱ�����־�߳���Ϣ */
    elog_set_fmt(ELOG_LVL_WARN, ELOG_FMT_LVL | ELOG_FMT_TAG | ELOG_FMT_TIME | ELOG_FMT_T_INFO);

    /* ��Ϣ�������־������־��ǩ����־ʱ�����־�߳���Ϣ */
    elog_set_fmt(ELOG_LVL_INFO, ELOG_FMT_LVL | ELOG_FMT_TAG | ELOG_FMT_TIME | ELOG_FMT_T_INFO);
 
    /* ���ԣ�������˺������������Ϣ֮����������� */
    elog_set_fmt(ELOG_LVL_DEBUG, ELOG_FMT_ALL & ~(ELOG_FMT_FUNC  | ELOG_FMT_T_INFO));

    /* ��ϸ������������� */
    elog_set_fmt(ELOG_LVL_VERBOSE, ELOG_FMT_ALL);

	/*Eenbale color*/
	elog_set_text_color_enabled(true);

	/* start EasyLogger */
	elog_start();
    
}

