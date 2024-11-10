#include "ring_buffer.h"
#include "stddef.h"
#include "gpio.h"


void RingBufferinit(stRingBuff* ringBuf,uint8_t* data,uint8_t len)
{
ringBuf->in =0;
ringBuf->out =0;
ringBuf->buffer =data;
ringBuf->buffer_size =len;
}

//�ж������Ƿ�д��
int IsRingBufferFull(stRingBuff *ringBuf)
{
  if (ringBuf == NULL)
    {
        return 0;    
    }
  if(((ringBuf->in+1) % ringBuf->buffer_size) == ringBuf->out)
    {
        return 1;
    }
    return 0;
	}

	

//дһ���ֽ�	
void WriteOneByteToRingBuffer(stRingBuff *ringBuf,uint8_t data)
{
 if (ringBuf == NULL || IsRingBufferFull(ringBuf))//д֮ǰ���ж϶����Ƿ�д��
    {     
        return;
    }
    ringBuf->buffer[ringBuf->in] = data;
    ringBuf->in = (++ringBuf->in) % ringBuf->buffer_size;    //��ֹԽ��
 
}



//��һ���ֽ�
void ReadOneByteFromRingBuffer(stRingBuff *ringBuf,uint8_t *data)
{
 if (ringBuf == NULL||IsRingBufferEmpty(ringBuf)) //��֮ǰ�ж϶����Ƿ�Ϊ��
    {
       
        return;
    }
		
    *data = ringBuf->buffer[ringBuf->out];
    ringBuf->out = (++ringBuf->out) % ringBuf->buffer_size;    //��ֹԽ�� 
}


//д�����ֽ�
void WriteRingBuffer(stRingBuff *ringBuf,uint8_t *writeBuf,unsigned int len)
{
    unsigned int i;
 
 if (ringBuf == NULL)
    {
        return ;
    }
    
    for(i = 0; i < len; i++)
    {
        WriteOneByteToRingBuffer(ringBuf,writeBuf[i]);
    }
}
	
	
//������ֽ�
void ReadRingBuffer(stRingBuff *ringBuf,uint8_t *readBuf,unsigned int len)
{
    unsigned int i;
	
 if (ringBuf == NULL)
    {       
        return;
    }
    for(i = 0; i < len; i++)
    {
        ReadOneByteFromRingBuffer(ringBuf,&readBuf[i]);
    }
}

//��ȡ�Ѿ�д����е����ݳ���
int GetRingBufferLength(stRingBuff *ringBuf)
{
    if (ringBuf == NULL)
    {
        return 0;
    }
    return (ringBuf->in - ringBuf->out + ringBuf->buffer_size) % ringBuf->buffer_size;
}

int IsRingBufferEmpty(stRingBuff *ringBuf)
{ 
 if (ringBuf == NULL)
    {     
        return 0;
    }
    
 if(ringBuf->in == ringBuf->out)   //д��λ�úͶ���λ�����ʱΪ��
    {
        return 1;
    }
    return 0;
}

