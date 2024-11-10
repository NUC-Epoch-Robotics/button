// /*   明海校验用于自定义的串口通信中     */
///**  可实现数据校验同时对错误数据进行纠错  **/ 
//#include "stdio.h"
//#include "stdlib.h"//调用system函数

////#include <malloc.h>
//#include <dos.h>//注意pow(double,int)而不是pow(int,int)
//#define M 100 //最大的数据位数
//#define N 50//最大的校验位数

//struct Hamming{
//	int flag;
//	int value;//海明码的数值
//}H[M];//存放海明码信息

//int P[N];//存放校验位信息
//int C[N];//存放检验位信息
//int m[M + N];//存放需要纠正的海明码
//int DC = 0, PC = 0, EC = 0, HC = 0, ERROR = 0, ERROR1 = 0;//DC是数据的位数,PC是校验位的位数,不能在此更改这些数据的值，否则出错

////void showInf(){//显示信息
////	printf("-------------------------------------------------------------------------------\n");
////	printf("*****************************************海明码********************************\n");
////	printf("***************************************输入格式如下:***************************\n");
////	printf("*****************************如果要输入1101，需输入 1 1 0 1 -1******************\n");
////	printf("**************输入数据 1 1 0 1 -1的过程中，不能回退删除，否则会发生错误*********\n");
////	printf("-----------------------------选择操作：----------------------------------------\n");
////	printf("1 输入数据后生成海明码 2 输入海明码  3 清屏  4 纠正海明码 5 返回主页 6 结束程序\n\n");
////	printf("请选择操作,只能输入一个数: ");
////}

//int inputData(int choice){//输入数据或海明码，参数是1就表示输入的是数据，参数是2就表示输入的是海明码。返回-1就表示输入数据错误。
//	int data, i = 1;
//	//scanf("%d", &data);
//	if (data != 0 && data != 1)
//	{
//		system("cls");
//		printf("\n\n提示信息:输入格式不正确,第一位只能是0或1,请重新选择操作\n\n\n");
//		return -1;
//	}
//	DC = 1;
//	PC = 1;
//	while (data != -1){
//		if (pow(2.0, PC - 1) != i){//存放数据值到海明码结构体中
//			H[i].flag = 0;//数据位，位置标记为0
//			H[i].value = data;
//			m[i] = data;
//			DC++;
//			scanf("%d", &data);
//		}
//		else if (pow(2.0, PC - 1) == i&&choice == 1){//设置校验位
//			H[i].flag = 1;//校验位，位置标记为1
//			PC++;
//		}
//		if ((pow(2.0, PC - 1) == i) && choice == 2){//把海明码中的校验位放到校验位数组中，以便用这个校验位和利用数据生成的校验位异或来计算哪一位出错
//			H[i].flag = 1;
//			P[PC++] = data;
//			m[i] = data;
//			scanf("%d", &data);
//		}
//		i++;
//	}
//	//累计次数是多加了一次，减回来。
//	DC--;
//	PC--;
//	if (PC + DC < 3)
//		ERROR = 2;
//	return 0;
//}

//void caculateHammingCode(){//根据海明码结构体中数据位的数据值计算海明码，并把正确的海明码放到海明码结构体中。
//	int m, j, k, temp = 0;
//	for (m = 3; m <= DC + PC; m++){
//		temp = 0;
//		if (H[m].flag == 0){//数据位
//			for (j = PC; j >= 1; j--){
//				k = (int)pow(2.0, j - 1);
//				if (k < m){
//					H[k].value = H[k].value^H[m].value;
//					temp = k;
//					break;
//				}
//			}
//			for (k = j - 1; k >= 1; k--){
//				if (H[k].flag == 1){//校验位
//					if (temp + pow(2.0, k - 1) == m){
//						H[k].value = H[k].value^H[m].value;
//						break;
//					}
//					if (temp + pow(2.0, k - 1) < m){
//						H[k].value = H[k].value^H[m].value;
//						temp = temp + pow(2.0, k - 1);
//					}
//				}
//			}
//		}
//	}
//}

//void caculateC(int pc){//计算检验位的值，并放到检验位的数组中。接受的参数是校验位的个数
//	int i, k;
//	for (i = 1; i <= pc; i++){
//		k = (int)pow(2.0, i - 1);
//		C[i] = P[i] ^ H[k].value;
//	}
//}

//void initial(){//使海明码结构体清零
//	int i;
//	for (i = 1; i <= DC + PC; i++){
//		H[i].value = 0;
//	}
//}

//void showHammingCode(int dc, int pc){//打印海明码结构体中的值。接受的参数是dc为数据位的个数，pc为校验位的个数。
//	int i;
//	printf("\n\n生成的海明码为:\n");
//	for (i = 1; i <= dc + pc; i++){
//		printf("第%d位:", i);
//		printf("  %d\n", H[i].value);
//	}
//	printf("\n");
//}
//void judge(int pc){//判断出哪一位出错，并打印出这一位是数据位还是校验位
//	int i, result = 0, flag = 0;
//	//system("cls");
//	for (i = 1; i <= pc; i++){
//		if (C[i] == 1) flag++;

//	}
//	for (i = 1; i <= pc; i++){
//		if (flag == 0)
//			result = result + C[i] * pow(2.0, pc - i);
//		else result = result + C[i] * pow(2.0, i - 1);
//	}
//	if (result != 0){
//		printf("\n\n\n提示信息:第%d位出错,", result);
//		EC = result;
//		if (flag >= 2)
//			printf(" 这一位是数据位!\n\n");
//		else printf(" 这一位是校验位!\n\n");
//	}
//	else printf("\n\n提示信息:海明码正确!\n\n\n");
//}
//void mend(int mc){//纠正错误的海明码。并打印出纠正前错误的海明码和纠正后正确的海明码
//	int i, k = 0, p = 1;
//	if (EC == 0){
//		printf("\n\n提示信息:海明码正确或者已经纠正或者海明码为空,请重新选择操作\n\n\n");
//		return;
//	}
//	system("cls");
//	printf("\n提示信息:海明码第%d位有错误\n\n", EC);
//	printf("\n输入的错误的海明码是:\n\n");
//	for (i = 1; i <= mc; i++)
//		printf("第%d位%d\n", i, m[i]);
//	m[EC] = !m[EC];
//	printf("\n\n纠正后的正确的海明码是:\n\n");
//	for (i = 1; i <= mc; i++)
//		printf("第%d位%d\n", i, m[i]);
//}

//void run(int command){//运行不同的操作模式，command为1表示输入的数据位，command为2表示输入的是海明码
//	int i;
//	if (command == 1){//输入的是数据
//		printf("\n\n请输入数据,以-1做结束符：");
//		i = inputData(1);//返回输入的数据位数
//		if (i == -1) return;
//		caculateHammingCode();
//		showHammingCode(DC, PC);
//		initial();
//		HC = PC + DC;
//		DC = 0;
//		PC = 0;
//	}
//	else {//输入的是海明码
//		printf("\n\n请输入海明码,以-1做结束符,只能有一位错误:");
//		i = inputData(2);
//		if (i == -1) return;
//		caculateHammingCode();
//		if (ERROR == 1 || ERROR == 2) {
//			system("cls");
//			printf("\n\n提示信息:输入格式不正确,第一位只能是0或1,并且输入的位数要大于等于3.请重选操作\n\n\n");
//			ERROR = 0;
//			PC = 0;
//			return;
//		}
//		caculateC(PC);
//		judge(PC);
//		initial();
//		HC = PC + DC;
//		PC = 0;
//		DC = 0;
//	}
//}
////void main(){
////	int function, flag = 1, command, md = 0, back = 0;
////	showInf();
////	scanf("%d", &function);
////	while (flag){
////		switch (function){
////		case 1: command = 1;
////			run(command);//输入数据
////			md = 0;
////			break;
////		case 2: command = 2;//输入海明码
////			run(command);
////			md = 1;
////			break;
////		case 3: system("cls");//清屏
////			break;
////		case 4:system("cls");
////			if (md == 1)//纠正海明码
////				mend(HC);
////			else printf("\n\n提示信息:您还没有输入海明码或者输入的海明码已经被纠正了.\n\n\n");
////			md = 0;
////			EC = 0;
////			break;
////		case 5: system("cls");//返回主页
////			back = 1;
////			break;
////		case 6:exit(1);//退出
////		default: system("cls");
////			printf("\n\n\n提示信息:操作不正确，请重新选择!\n\n");
////			break;
////		}
////		if (back == 1){
////			showInf();
////			back = 0;
////		}
////		else {
////			printf("\n1 输入数据后生成海明码 2 输入海明码  3 清屏  4 纠正海明码 5 返回主页 6 结束程序\n\n");
////			printf("请选择操作,只能输入一个数: ");
////		}
////		scanf("%d", &function);
////	}
////}
