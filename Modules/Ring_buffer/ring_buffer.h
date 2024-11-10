#ifndef __RING_BUFFER_H__
#define __RING_BUFFER_H__
#include"gpio.h"

#define RING_BUFF_SIZE 200

typedef struct ringBuff{
    unsigned int in;                             //写入的位置
    unsigned int out;                            //读出的位置
	  unsigned int buffer_size;	
    unsigned char  *buffer;     //数据域
}stRingBuff;
//队列初始化
void RingBufferinit(stRingBuff* ringBuf,uint8_t* data,uint8_t len);
//判断队列是否为空
int IsRingBufferEmpty(stRingBuff *ringBuf);

//判断队列是否为满
int IsRingBufferFull(stRingBuff *ringBuf);

//写一节字节	
void WriteOneByteToRingBuffer(stRingBuff *ringBuf,uint8_t data);

//读一节字节
void ReadOneByteFromRingBuffer(stRingBuff *ringBuf,uint8_t *data);
	
//写入多个字节
void WriteRingBuffer(stRingBuff *ringBuf,uint8_t *writeBuf,unsigned int len);
	
//读入多个字节
void ReadRingBuffer(stRingBuff *ringBuf,uint8_t *readBuf,unsigned int len);

//获取队列长度
int GetRingBufferLength(stRingBuff *ringBuf);

#endif

