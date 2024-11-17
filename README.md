# Button-Debug-Medule
## 做一个按键调试模块，该模块可以与主控进行通信，并可以显示一些主控的状态。
### Readme

### 主控板

本模块使用到freertos，模块执行原理也是根据freertos来展开，若要调试的主控板里没有用到freertos，则不能使用该模块



Buttonmotorinit()函数实现程序初始化 中间层的函数都在此函数进行初始化
*
*xEventGroupSetBitsFromISR()发送事件标志位， xEventGroupWaitBits()接收事件标志位
*
*使用事件组来控制任务的恢复（开始）与挂起（暂停），事件组程序在 button_task() 任务中使用

*这里面 可以新添加任务，点灯任务只是个案例，可以更改为其他任务，也可以是单独的一个语句（使用时主控板里主要改动的是这个地方）
![image-20241117203859688](C:\Users\27795\AppData\Roaming\Typora\typora-user-images\image-20241117203859688.png)

比如 : 我想要让按键单独控制发射任务 shoot_task();

```
if (myEventBits_1 & Event_0)
 {
		shoot_task();
 }s
```

​		





*想用按键控制一个功能，可以把对应的函数放在 button_task() 的if语句里面 if判断里只能到Event_8 (目前写到8了，不够用的话请自行在事件组宏定义处自行添加)
*/



## 使用

主控板使用的串口是uart7进行通信





Event_0
Event_1
Event_2
Event_3
Event_4
Event_5
Event_6
Event_7
Event_8等等 是宏定义的事件标志位

每个对应一个事件发生

只需在button_mottor.c 文件中的void button_task()任务中  if中添加 按键按下后所要执行的对应程序

这里面 可以新添加任务，可以更改为其他任务，也可以是单独的一个语句

```c
			if(myEventBits_1 & Event_0)
				{
			 // 按键按下后所要执行的对应程序
			  }
```

代码里面if语句只写到Event_5，若不够，请自行添加

![image-20241117201644366](C:\Users\27795\AppData\Roaming\Typora\typora-user-images\image-20241117201644366.png)

每个按键依次控制从Event_0开始的3个事件标志位







