/*************************************************************************
项目：金莱特力量站
PCB板：MCU=STC8C2K64S4、系统时钟24M
外设：金莱特TFT+飞梭+础慧下控

************************************************************
2024-0906：卸力时必须时必须是向心力模式，向心力模式收绳的速度是恒定的；
							不能用一般模式，因为收绳速度会越来越快。
************************************************************
需要完善：
蜂鸣器																				OK
接收/发送 参数大于FD的数字会分开发送					OK
增加手动收绳功能															OK
适配TFT-透传																	ok
飞梭-数据																			ok
飞梭-透传																			OK
清计数：次数-距离															OK  在回复tft时已清	
数据限制																			ok	回力已经限制
整合两个电机																	OK
下控错误码整理 																OK
测试下控大于FD数据														OK

1、距离计算、处理
2、次数计算、处理
3、下控通讯中断 报通讯故障										//等两个下控
4、电机状态不是0怎样处理											OK
5、折算																				//需等APP
**************************************************************************/
#include "config.h"
#include "bsp.h"

#include "tick.h"
#include "uart.h"

#include "CH_Protocol.h"
#include "JLT_Protocol.h"
#include "FeiSuo_Protocol.h"
#include "Buzzer.h"

void main( void )
{
		EA = 0;											//关总中断
    
		Gpio_init();								//初始化IO
		Timer0_init();							//初始化定时器T0
		Timer1_init();							//初始化定时器1（蜂鸣器）
		Uart4_Init();								//初始化串口4（础慧下控）
		Uart3_Init();								//初始化串口3（础慧下控）
		Uart2_Init();								//初始化串口2（TFT）
		Uart1_Init();								//初始化串口1（飞梭）
		EA = 1;											//打开总中断

		inital_TFT_Data();					//初始化数据-TFT （金莱特）
		inital_MotorL_Data();				//初始化数据-下控（础慧）
		inital_MotorR_Data();				//初始化数据-下控2（础慧）
		inital_FS_Data();						//初始化数据-飞梭

		LongTone;
	
    while(1)
    {
			if(ubTime5mS)
			{				
				ubTime5mS=0;
				
			  Task_Buzzer();				  // 蜂鸣器

				Motor_Uart_End();				// 通讯接收清数据
				
				TFT_Uart_End();					// 通讯接收清数据
				
				FS_Uart_End();
			}
			
			if(ubTime100mS)
			{
				ubTime100mS=0;
				
				Task_Motor();       			// 下控通讯 (础慧)
				
				Task_MotorError();				// 下控故障整合处理
				
				Task_Distance();					// 距离计算
				
				Task_PullNumber();				// 次数计算
			}
			
			if(ubTime200mS)
			{
				ubTime200mS=0;
				
				Task_FS();								//飞梭通讯

			}

			if(ubTime1S)								
			{
			
				ubTime1S=0;
				
				Task_TFT();      					//TFT通讯(金莱特)

//				if(++KK >=120)							//测试TFT
//				{
//					KK=0;
//					if(Key_State)						//120秒改变一次状态		
//						Key_State=0;
//					else
//						Key_State=1;
//				}

//				if(RunState==1)						//再上力状态每秒，拉力+10；距离+10
//				{
//					Key_Force +=10;
//					if(Key_Force >=Preinstall_Max_Weight)
//						Key_Force=0;
//					
//					RunDistance +=10;			
//					if(RunDistance >=200)
//						RunDistance=10;
//				}

//				RunPullNumber++;					//每秒计次 +1
//				if(RunPullNumber >=4000)
//					RunPullNumber=0;
			}
    }
}

