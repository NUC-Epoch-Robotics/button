###### 【STM32】串口不定长接收 

1.[【STM32】串口不定长接收 保姆级教程_stm32串口接收不定长数据方法-CSDN博客](https://blog.csdn.net/a1547998353/article/details/139193387)

2.【【工作STM32】第10集 STM32串口DMA模式与收发不定长数据 | keysking的stm32教程】https://www.bilibili.com/video/BV1do4y1F7wt?vd_source=5738113e28372f4ced286d8bc1d8c66a



3.谈STM32串口通信(二)自定义帧格式和超时接收_stm32串口超时接收-CSDN博客](https://blog.csdn.net/weixin_40134414/article/details/105374200)

通过此csnd我了解到可以通过定义结构体把发送的数据帧创建起来

之后把数据帧放到一个数组中（看作发送缓冲区）最后，将缓冲区数据发送出去4.[STM32CubeMX HAL库串口+DMA+IDLE空闲中断不定长度数据接收和发送_hal 库 清除 dma接收中断标志位-CSDN博客](https://blog.csdn.net/CSDN_Xu_xue/article/details/104403532)

​      

 接收数据 采用空闲中断+DMA接收

  

​       为了减少误进入串口空闲中断，串口RX的IO管脚一定设置成Pull-up<上拉模式>，串口空闲中断只是接收的数据时触发，发送时不触发。5.[HAL库 串口空闲中断+DMA接收不定长数据 详解及踩坑_hal库串口空闲中断-CSDN博客](https://blog.csdn.net/soledade/article/details/129030082)



主要是空闲中断如何实现 空闲中断与普通的串口回调函数的差别[深入理解 STM32 串口空闲中断的工作原理与实现方法_stm32串口空闲中断触发条件-CSDN博客](https://blog.csdn.net/weixin_66608063/article/details/135007636)

 

说明空闲中断的代码框架[STM32串口：DMA空闲中断实现接收不定长数据（基于HAL库）_dma不定长hal-CSDN博客](https://blog.csdn.net/hhhbdbfb/article/details/136630467)