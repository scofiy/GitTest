/*************************************************************************
��Ŀ������������վ
PCB�壺MCU=STC8C2K64S4��ϵͳʱ��24M
���裺������TFT+����+�����¿�

************************************************************
2024-0906��ж��ʱ����ʱ������������ģʽ��������ģʽ�������ٶ��Ǻ㶨�ģ�
							������һ��ģʽ����Ϊ�����ٶȻ�Խ��Խ�졣
************************************************************
��Ҫ���ƣ�
������																				OK
����/���� ��������FD�����ֻ�ֿ�����					OK
�����ֶ���������															OK
����TFT-͸��																	ok
����-����																			ok
����-͸��																			OK
�����������-����															OK  �ڻظ�tftʱ����	
��������																			ok	�����Ѿ�����
�����������																	OK
�¿ش��������� 																OK
�����¿ش���FD����														OK

1��������㡢����
2���������㡢����
3���¿�ͨѶ�ж� ��ͨѶ����										//�������¿�
4�����״̬����0��������											OK
5������																				//���APP
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
		EA = 0;											//�����ж�
    
		Gpio_init();								//��ʼ��IO
		Timer0_init();							//��ʼ����ʱ��T0
		Timer1_init();							//��ʼ����ʱ��1����������
		Uart4_Init();								//��ʼ������4�������¿أ�
		Uart3_Init();								//��ʼ������3�������¿أ�
		Uart2_Init();								//��ʼ������2��TFT��
		Uart1_Init();								//��ʼ������1������
		EA = 1;											//�����ж�

		inital_TFT_Data();					//��ʼ������-TFT �������أ�
		inital_MotorL_Data();				//��ʼ������-�¿أ����ۣ�
		inital_MotorR_Data();				//��ʼ������-�¿�2�����ۣ�
		inital_FS_Data();						//��ʼ������-����

		LongTone;
	
    while(1)
    {
			if(ubTime5mS)
			{				
				ubTime5mS=0;
				
			  Task_Buzzer();				  // ������

				Motor_Uart_End();				// ͨѶ����������
				
				TFT_Uart_End();					// ͨѶ����������
				
				FS_Uart_End();
			}
			
			if(ubTime100mS)
			{
				ubTime100mS=0;
				
				Task_Motor();       			// �¿�ͨѶ (����)
				
				Task_MotorError();				// �¿ع������ϴ���
				
				Task_Distance();					// �������
				
				Task_PullNumber();				// ��������
			}
			
			if(ubTime200mS)
			{
				ubTime200mS=0;
				
				Task_FS();								//����ͨѶ

			}

			if(ubTime1S)								
			{
			
				ubTime1S=0;
				
				Task_TFT();      					//TFTͨѶ(������)

//				if(++KK >=120)							//����TFT
//				{
//					KK=0;
//					if(Key_State)						//120��ı�һ��״̬		
//						Key_State=0;
//					else
//						Key_State=1;
//				}

//				if(RunState==1)						//������״̬ÿ�룬����+10������+10
//				{
//					Key_Force +=10;
//					if(Key_Force >=Preinstall_Max_Weight)
//						Key_Force=0;
//					
//					RunDistance +=10;			
//					if(RunDistance >=200)
//						RunDistance=10;
//				}

//				RunPullNumber++;					//ÿ��ƴ� +1
//				if(RunPullNumber >=4000)
//					RunPullNumber=0;
			}
    }
}

