#ifndef __RING_BUFFER_H__
#define __RING_BUFFER_H__
#include"gpio.h"

#define RING_BUFF_SIZE 200

typedef struct ringBuff{
    unsigned int in;                             //д���λ��
    unsigned int out;                            //������λ��
	  unsigned int buffer_size;	
    unsigned char  *buffer;     //������
}stRingBuff;
//���г�ʼ��
void RingBufferinit(stRingBuff* ringBuf,uint8_t* data,uint8_t len);
//�ж϶����Ƿ�Ϊ��
int IsRingBufferEmpty(stRingBuff *ringBuf);

//�ж϶����Ƿ�Ϊ��
int IsRingBufferFull(stRingBuff *ringBuf);

//дһ���ֽ�	
void WriteOneByteToRingBuffer(stRingBuff *ringBuf,uint8_t data);

//��һ���ֽ�
void ReadOneByteFromRingBuffer(stRingBuff *ringBuf,uint8_t *data);
	
//д�����ֽ�
void WriteRingBuffer(stRingBuff *ringBuf,uint8_t *writeBuf,unsigned int len);
	
//�������ֽ�
void ReadRingBuffer(stRingBuff *ringBuf,uint8_t *readBuf,unsigned int len);

//��ȡ���г���
int GetRingBufferLength(stRingBuff *ringBuf);

#endif

