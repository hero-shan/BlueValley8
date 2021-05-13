#include "stdio.h"
#include "unistd.h"
#include "sys/types.h"
#include "sys/stat.h"
#include "fcntl.h"
#include "stdlib.h"
#include "string.h"
 
#include <sys/shm.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <sys/ioctl.h>   
#include <sys/socket.h>   
#include <netdb.h>    
#include <net/if.h> 

#include <pthread.h>
#include <bits/wordsize.h>
#include "mymain.h"
#include <errno.h>


#define GET_DATA_BYTE0(data)	(uint8_t)((data >> 0 ) && 0xff)
#define GET_DATA_BYTE1(data)	(uint8_t)((data >> 8 ) && 0xff)	
#define GET_DATA_BYTE2(data)	(uint8_t)((data >> 16) && 0xff)
#define GET_DATA_BYTE3(data)	(uint8_t)((data >> 24) && 0xff)

#define test_ip "114.242.183.18"
// #define test_ip "172.18.1.90"
// #define test_ip "192.168.1.254"
const char * ptr_ip;

#define PORT                  8656//8864 5000
#define BUFFER_SIZE           1000
#define SENDDATA_SIZE         83

uint8_t Cloud_OutNet_CarNo[20][16];
uint8_t Cloud_Appoint_CarNo[20][16];
uint8_t Station_No[20]={0x31,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30};
extern uint8_t SourseStorage_Batt_NUM[50];

extern double RBCar_Batt_Soc;
extern double ReplaceBatt_SOC;

extern uint8_t FireContr_HardErr_flg;
extern uint8_t FireContr_GetFire_flg;
extern double RBCar_Batt_Available_Energy;
extern double RBCar_Batt_Available_Cap;
extern double RBCar_Batt_Soc;

extern double ReplaceBatt_Available_Energy;
extern double ReplaceBatt_Available_Cap;
extern double ReplaceBatt_SOC;


uint8_t RemotSend_LogOn_flg = 0;
uint8_t RemotSend_LogOut_flg = 0;
// uint8_t RemotSend_Heart_flg = 0;

// uint8_t RemotSend_UpPLCData_flg = 0;
// uint8_t RemotSend_UpACEngData_flg = 0;
// uint8_t RemotSend_UpAirConData_flg = 0;
// uint8_t RemotSend_UpFanData_flg = 0;
// uint8_t RemotSend_UpFireData_flg = 0;
// uint8_t RemotSend_UpCEData_flg = 0;
// uint8_t RemotSend_UpCHUData_flg = 0;
// uint8_t RemotSend_UpErrCHUData_flg = 0;
// uint8_t RemotSend_UpCEingData_flg = 0;

uint8_t Remo_SendFanBack_flg;
uint8_t Remo_SendDoorBack_flg;
uint8_t Remo_SendAirBack_flg;
uint8_t Remo_SendChuBack_flg;
uint8_t Remo_SendPLCBack_flg;
uint8_t Remo_SendEMSBack_flg;
uint8_t Remo_SendBack_buf[100]={0x23,0x23,0x80};
// uint8_t Remo_SendDoorBack_buf[100]={0x23,0x23,0x01,0xFE};
// uint8_t Remo_SendAirBack_buf[100]={0x23,0x23,0x01};
// uint8_t Remo_SendChuBack_buf[100]={0x23,0x23,0x01};
// uint8_t Remo_SendPLCBack_buf[100]={0x23,0x23,0x01};
// uint8_t Remo_SendEMSBack_buf[100]={0x23,0x23,0x01};
uint8_t RemotSend_Heart_buf[100]={0x23,0x23,0x07,0xFE};//通信心跳
uint8_t RemotSend_LogOn_buf[100]={0x23,0x23,0x01,0xFE};//换电站登入
int RemotSend_LogOn_buf_len=0;
uint8_t RemotSend_LogOut_buf[100]={0x23,0x23,0x04,0xFE};//换电站登出 
int RemotSend_LogOut_buf_len=0;

uint8_t RemotSend_UpPLCData_buf[100]={0x23,0x23,0x02,0xFE};//换电站设备数据
int RemotSend_UpPLCData_buf_len=0;
uint8_t RemotSend_UpACEngData_buf[200]={0x23,0x23,0x02,0xFE};//交流电表数据
int RemotSend_UpACEngData_buf_len=0;
uint8_t RemotSend_UpAirConData_buf[100]={0x23,0x23,0x02,0xFE};//换电站空调数据
int RemotSend_UpAirConData_buf_len=0;
uint8_t RemotSend_UpFanData_buf[100]={0x23,0x23,0x02,0xFE};//风机数据
int RemotSend_UpFanData_buf_len=0;
uint8_t RemotSend_UpFireData_buf[100]={0x23,0x23,0x02,0xFE};//消防、门禁、水禁数据
int RemotSend_UpFireData_buf_len=0;
uint8_t RemotSend_UpCEData_buf[200]={0x23,0x23,0x02,0xFE};//汽车换电结果数据
int RemotSend_UpCEData_buf_len=0;
uint8_t RemotSend_UpCHUData_buf[10000]={0x23,0x23,0x02,0xFE};//电池包未报警的充电机充电数据
int RemotSend_UpCHUData_buf_len=0;
uint8_t RemotSend_UpErrCHUData_buf[10000]={0x23,0x23,0x02,0xFE};//电池包报警的充电机数据
int RemotSend_UpErrCHUData_buf_len=0;
uint8_t RemotSend_UpCEingData_buf[200]={0x23,0x23,0x02,0xFE};//汽车换电过程的状态数据
int RemotSend_UpCEingData_buf_len=0;

uint8_t Remot_conn_flg = 0;
uint16_t RemotSend_Discon_con;
uint8_t Remot_Recv_buf[500];
uint8_t Remo_SpareTire_place = 0;

void RemotRecv_Server_Deal(uint8_t* recv_buf,uint16_t dlc)
{
	uint8_t i;
	switch (recv_buf[2])
	{
		case 0x80:  					//换电站终端控制
		{
			switch (recv_buf[28])
			{
				case 3:    			//风机远程控制
				{
					if(recv_buf[29]==1)
						Fan_Start_flg = 1,Remo_SendBack_buf[29] = 1;
					else if(recv_buf[29]==2)
						Fan_Start_flg = 0,Remo_SendBack_buf[29] = 2;
					Remo_SendFanBack_flg = 1;
					Remo_SendBack_buf[3] = 1;
					Remo_SendBack_buf[28] = 3;
//					printf("Remo3333=%d!\n",recv_buf[28]);
				}break;
				case 4:    			//卷帘门控制
				{
					Remo_SendBack_buf[28] = 4;
					Remo_SendBack_buf[29] = recv_buf[29];
					Remo_SendDoorBack_flg = 1;
					if(Sys_Big_sta>=3){
						Remo_SendBack_buf[3] = 2;
						break;
					}
					Remo_SendBack_buf[3] = 1;
					if(recv_buf[29]==1)
						Door_Start_flg = 1,Door2_Start_flg = 1;
					else if(recv_buf[29]==2)
						Door_Start_flg = 0,Door2_Start_flg = 0;
					Remo_SendBack_buf[3] = 1;
//					printf("Remo44444=%d!\n",recv_buf[28]);
				}break;
				case 5:    			//空调远程控制 
				{
					Remo_SendAirBack_flg = 1;
					Remo_SendBack_buf[28] = 5;
					Remo_SendBack_buf[29] = recv_buf[29];
					if(recv_buf[29]>=1&&recv_buf[29]<=3)
					{
						Remo_SendBack_buf[3] = 1;
						if(recv_buf[30]>=1&&recv_buf[30]<=8)
						{
							Qt_AirSetData_flg[recv_buf[29]-1] = recv_buf[30];
							AirSet_EnginStartTemp_C = recv_buf[31];
						}				
						else if(recv_buf[30]==9)
							Qt_AirOpen_flg[recv_buf[29]-1] = 1;
						else if (recv_buf[30]==10)
							Qt_AirOpen_flg[recv_buf[29]-1] = 2;
					}	
					else
					{
						Remo_SendBack_buf[3] = 2;
					}
//					printf("Remo5555=%d %d!\n",AirSet_EnginStartTemp_C,Qt_AirSetData_flg[recv_buf[29]]);			
				}
				break;
				case 6:    			//充电远程控制{23 23 80 FE 30 30 30 30 30 30 30 30 30 30 30 30 30 30 30 01 00 09 E4 BA AC 41 44 44 06 01 01 16 A4 }
				{
					Remo_SendChuBack_flg = 1;
					Remo_SendBack_buf[28] = 6;
					Remo_SendBack_buf[29] = recv_buf[29];
					if(recv_buf[29]>=1&&recv_buf[29]<=20)
					{
						Remo_SendBack_buf[3] = 1;
						MLocat_SelfChar_ena[recv_buf[29]-1] = 0;
						if(recv_buf[30]==1)
							Hand_LowPwrOn_flg[recv_buf[29]-1] = 1;
						//	printf("Hand_LowPwrOn_flg zhugongxiafa=%d!\n",Hand_LowPwrOn_flg[0]);		
						else if(recv_buf[30]==2)
							Hand_StartChg_flg[recv_buf[29]-1] = 1;
						else if(recv_buf[30]==3)
							Hand_StopChg_flg[recv_buf[29]-1] = 1;
						else if(recv_buf[30]==4)
							Hand_LowPwrDown_flg[recv_buf[29]-1] = 1;
					}
					else
					{
						Remo_SendBack_buf[3] = 2;
					}
			//		printf("Remo6666=%d!\n",recv_buf[28]);			
				}
				break;
				case 7:    			//换电设备的远程控制{23 23 80 FE 30 30 30 30 30 30 30 30 30 30 30 30 30 30 30 01 00 0B E4 BA AC 41 44 44 07 03 01 01 01 0A 34 }
				{
					Remo_SendPLCBack_flg = 1;
					Remo_SendBack_buf[28] = 7;
					Remo_SendBack_buf[29] = recv_buf[29];
					if(MQt_StartSelfCE_sta==1){
						Remo_SendBack_buf[3] = 2;
						break;
					}
					Remo_SendBack_buf[3] = 1;
					switch(recv_buf[29])
					{
						case 1:{
						}
						break;
						case 2:{
							QT_InitOrder_flg = 1;
			//				printf("QT_InitOrder_flg=%d!\n",QT_InitOrder_flg);
						}
						break;
						case 3:{
							QT_ChangeBatOrder_flg = 1;
							MCE_CBLocator_sour = recv_buf[30];
							MCE_CBLocator_CarTy = recv_buf[32];
			//				printf("MCE_CBLocator_sour=%d %d\n",MCE_CBLocator_sour,recv_buf[30]);
						}
						break;
						case 4:{
							QT_InLocatorOrder_flg = 1;
							MCE_InLocator_dest = recv_buf[31];
							MCE_InLocator_CarTy = recv_buf[32];
			//				printf("QT_InLocatorOrder_flg=%d %d",MCE_InLocator_dest,MCE_InLocator_CarTy);
						}break;
						case 5:{
							QT_OutLocatorOrder_flg = 1;
							MCE_OutLocator_sour = recv_buf[30];
							MCE_OutLocator_CarTy = recv_buf[32];
			//				printf("QT_InLocatorOrder_flg=%d %d",MCE_OutLocator_sour,MCE_OutLocator_CarTy);
						}break;		
						case 6:{
							QT_CELocatorOrder_flg = 1;
							MCE_CELocator_sour = recv_buf[30];
							MCE_CELocator_dest = recv_buf[31];
							MCE_CELocator_CarTy = recv_buf[32];
						}
						break;
						case 7:{
							QT_BatOutOrder_flg = 1;
							MCE_BatOut_sour = recv_buf[30];
							MCE_BatOut_CarTy = recv_buf[32];
						}
						break;
						case 8:{
							QT_DismountBatOrder_flg = 1;
							MCE_DismountBat_CarTy = recv_buf[32];
						}break;
						case 9:{
							QT_MountBatOrder_flg = 1;
							MCE_MountBat_CarTy = recv_buf[32];
						}break;	
						case 10:{
							QT_TimeOutOrder_flg = 1;
			//				printf("QT_TimeOutOrder_flg%d\n",QT_TimeOutOrder_flg);
						}break;
						case 11:{
							QT_ContinueOrder_flg = 1;
			//				printf("QT_ContinueOrder_flg=%d!\n",QT_ContinueOrder_flg);
						}break;
						case 12:{
							QT_StopOrder_flg = 1;
			//				printf("QT_StopOrder_flg\n");
						}break;
						case 13:{
							QT_ErrResetOrder_flg = 1;					
			//				printf("QT_ErrResetOrder_flg\n");
						}break;
						case 14:{
							MQt_StartSelfCE_sta = 1;//自动换电
			//				printf("MQt_StartSelfCE_sta%d\n",MQt_StartSelfCE_sta);
						}break;
						// case 15:{
						// 	MQt_StartSelfCE_sta = 2;//手动换电
						// 	printf("MQt_StartSelfCE_sta%d\n",MQt_StartSelfCE_sta);
						// }break;

						default:
						break;
					}
				}break;	
				case 9:    			//EMS远程控制
				{
					Remo_SendEMSBack_flg = 1;
					if(recv_buf[29]>=1&&recv_buf[29]<=0x1A){
						Remo_SendBack_buf[3] = 1;
						switch (recv_buf[29])
						{
							case 1:{			//到位
								Remo_SendBack_buf[29]=1;
								if(Sys_Big_sta==3&&Sta==33){
									if(recv_buf[30]==0x01){	//0x01 表示使用备胎
										Remo_SpareTire_place = recv_buf[31];  //备用轮胎所在位置
									}
									else{
										Remo_SpareTire_place = 0;
									}
									MRemo_GetInPlace_flg = 1;
								}
								
								else Remo_SendBack_buf[3] = 2;
							}break;
							case 2:{			//换电站自动模式开关
								Remo_SendBack_buf[29]=2;
								MQt_SendPoint_flg = 1;
								MRemo_GetSelfCE_flg = 1;
								if(recv_buf[30])
									MQt_StartSelfCE_sta = 1;
								else MQt_StartSelfCE_sta = 2;
							}break;
							case 3:{			//手动取消订单
								Remo_SendBack_buf[29]=3;
								// if(recv_buf[30])
								MQt_GetCancelOrd_flg = 1;
							}break;
							case 4:{			//充电完成SOC
								MLocat_CharEndSoc_pct = recv_buf[30];
							}break;
							case 5:{			//换电允许SOC
								MLocat_AllowCESoc_pct = recv_buf[30];
							}break;
							case 6:{			//充电机自动使能
								if(recv_buf[31]<=2&&recv_buf[30]<=20)
									MLocat_SelfChar_ena[recv_buf[30]] = (recv_buf[31]==1);
							}break;
							case 7:{			//库位故障清除
								if(recv_buf[31]<=2&&recv_buf[30]<=20)
									MLocat_ErrReset_flg[recv_buf[30]] = (recv_buf[31]==1);
							}break;
							case 8:{			//库位使能
								}break;
							case 9:{			//充电完成清除
								}break;
							case 10:{			//自动充电
								}break;
							case 11:{			//关闭声光报警
								MQt_DisaSoundLigErr_flg = 1;
								Sound_Start_flg = 0;
								Light_Start_flg = 0;
								}break;
							case 0x1A:{			//手动完成订单???

								}break;
							case 0x1C:{			//退网 电池包锁定与解锁
								uint8_t Remo_OutNet_BatteryNo[32];
								uint8_t Remo_AppointOutNet_flg = 0;
								uint8_t Remo_LockNoLock_flg = 0;
								uint8_t Remo_OutNet_CarNo[16];
								
								for(i=0;i<32;i++){
									Remo_OutNet_BatteryNo[i] = recv_buf[31+i];	//要锁定、解锁的电池包编码
								}
								Remo_LockNoLock_flg = recv_buf[62+1];			//0x01表示解锁0x02表示锁定
								Remo_AppointOutNet_flg = recv_buf[62+2];		//0x01表示预约换电0x02 表示退网
								for(i=0;i<16;i++){
									Remo_OutNet_CarNo[i] = recv_buf[65+i];		//新能源汽车车牌照
								}

								if(Remo_AppointOutNet_flg==2){
									if(Remo_LockNoLock_flg==2){			//退网锁定
										for(i=0;i<20;i++){
											if(strcmp(MLocat_BatteryNo[i],Remo_OutNet_BatteryNo)==0){
												Cloud_OutNetLock_flg[i] = 1;
												strcpy(Cloud_OutNet_CarNo[i],Remo_OutNet_CarNo);
												break;
											}
										}
									}
									else if(Remo_LockNoLock_flg==1){	//退网解锁
										for(i=0;i<20;i++){
											if(strcmp(MLocat_BatteryNo[i],Remo_OutNet_BatteryNo)==0){
												Cloud_OutNetLock_flg[i] = 0;
												strcpy(Cloud_OutNet_CarNo[i],"");
												break;
											}
										}
									}
								}
								if(Remo_AppointOutNet_flg==1){
									if(Remo_LockNoLock_flg==2){			//预约锁定
										for(i=0;i<20;i++){
											if(strcmp(MLocat_BatteryNo[i],Remo_OutNet_BatteryNo)==0){
												Cloud_AppointLock_flg[i] = 1;
												strcpy(Cloud_Appoint_CarNo[i],Remo_OutNet_CarNo);
												break;
											}
										}
									}
									else if(Remo_LockNoLock_flg==1){	//预约解锁
										for(i=0;i<20;i++){
											if(strcmp(MLocat_BatteryNo[i],Remo_OutNet_BatteryNo)==0){
												Cloud_AppointLock_flg[i] = 0;
												strcpy(Cloud_Appoint_CarNo[i],"");
												break;
											}
										}
									}
								}

								}break;
							case 0x1D:{			//预约换电

								}break;
							default:
								break;
						}
					}
					else
					{
						Remo_SendBack_buf[3] = 2;
					}
					Remo_SendBack_buf[28] = 9;
			//		printf("Remo999=%d!\n",recv_buf[28]);			
				}
				break;			
				default:
				break;
			}
		}
		break;
		case 0x07:
		{
			//printf("i=%d %d",recv_buf[2],recv_buf[3]);
		}
		break;
		default:
		break;
	}
}
void Remot_Rec_Deal(uint8_t* recv_buf,int16_t Read_Size) //{23,23,80,fe,0*15,01,dlc,data*,crc16}
{
	uint16_t i=0;
	uint8_t Recv_Server_buf[500];
	uint16_t rec_calchkval = 0;
    uint16_t my_calchkval = 0;
    uint16_t RecvData_dlc = 0;
    uint16_t current_head = 0;
	uint8_t buf[500];
    static unsigned short ss;
    do
    {
        if(recv_buf[current_head+0]==0x23&&recv_buf[current_head+1]==0x23)
        {
            RecvData_dlc = (recv_buf[current_head+20]*256+recv_buf[current_head+21]);//当前帧的数据单元长度
            rec_calchkval = mc_check_crc16(recv_buf+current_head,RecvData_dlc+22);
            my_calchkval = (recv_buf[current_head+RecvData_dlc+22]*256+recv_buf[current_head+RecvData_dlc+23]);
			if(my_calchkval == rec_calchkval)
            {
				// printf("crcrcrc_remoremo = %d %d\n",rec_calchkval,my_calchkval);
				for(i=0;i<RecvData_dlc+22;i++)
				{
					buf[i] = recv_buf[current_head+i];
					// printf("%d",buf[i]);
				}
				RemotRecv_Server_Deal(buf,RecvData_dlc);
                ss++;
                current_head = current_head + RecvData_dlc + 24;//头3+校验2
            }
            else
            {
	//			printf("errsdsfdsfs %d",current_head);
                break;
				ss = 0;
            }
        }
    }while((current_head + 5 <= Read_Size)&&(Read_Size>0));	
	
	bzero(Remot_Recv_buf, sizeof(Remot_Recv_buf));
	bzero(recv_buf, sizeof(recv_buf));
}

/* 交流电表 */
double ac_GroupActiveTotalEnergy = 0;		//当前组合有功总电能 
double ac_PositiveActiveTotalEnergy = 0;		//当前正向总有功电能
double ac_NegativeActiveTotalEnergy = 0;		//当前反向总有功电能
double ac_GroupReactiveTotalEnergy = 0;		//当前组合无功总电能
double ac_PositiveReactiveTotalEnergy = 0;	//当前正向总无功电能
double ac_NegativeReactiveTotalEnergy = 0;	//当前反向总无功电能

double acA_Voltage = 0;		//A相电压 
double acB_Voltage = 0;		//B相电压 
double acC_Voltage = 0;		//C相电压 
double acA_Current = 0;		//A相电流 
double acB_Current = 0;		//B相电流 
double acC_Current = 0;		//C相电流 
double acPt = 0;			//电压变比PT
double acCt = 0;			//电流变比CT
double acA_ActivePower = 0;	//A 相有功功率 
double acB_ActivePower = 0;	//B 相有功功率 
double acC_ActivePower = 0;	//C 相有功功率 
double acA_ReactivePower = 0;	//A 相无功功率 
double acB_ReactivePower = 0;	//B 相无功功率 
double acC_ReactivePower = 0;	//C 相无功功率 
double acA_PhasePowerFactor = 0;//A 相功率因数 
double acB_PhasePowerFactor = 0;//B 相功率因数 
double acC_PhasePowerFactor = 0;//C 相功率因数 
double ac_Err = 0;//交流电表告警

void RemotSend_Inter_Deal(uint8_t num)
{
	int i,check16=0;
	time_t timep={0};
	struct tm *p=NULL;
	time (&timep);	
	p = localtime(&timep);

	switch (num)
	{
		case 0:  									//换电站登入
		{		
			// bzero(RemotSend_LogOn_buf,100);
			for(i=0;i<15;i++)
				RemotSend_LogOn_buf[i+4] = Station_No[i];	//换电站的编码
			RemotSend_LogOn_buf[19] = 0x01;
			RemotSend_LogOn_buf[20] = 0x00;
			RemotSend_LogOn_buf[21] = 0x26;  		//数据单元长度
			RemotSend_LogOn_buf[22] = p->tm_year-100;
			RemotSend_LogOn_buf[23] = p->tm_mon+1;
			RemotSend_LogOn_buf[24] = p->tm_mday;
			RemotSend_LogOn_buf[25] = p->tm_hour;
			RemotSend_LogOn_buf[26] = p->tm_min;
			RemotSend_LogOn_buf[27] = p->tm_sec;
			RemotSend_LogOn_buf[28] = 0;
			RemotSend_LogOn_buf[29] = 1;			//登入流水号
			for(i=30;i<50;i++)
				RemotSend_LogOn_buf[i] = 0;			//ICCID ""
			RemotSend_LogOn_buf[59] = 0xf0;
			RemotSend_LogOn_buf[58] = 0xf7;
			RemotSend_LogOn_buf[57] = 0xf3;
			RemotSend_LogOn_buf[56] = 0x05;
			RemotSend_LogOn_buf[55] = 0x03;
			RemotSend_LogOn_buf[54] = 0x0;
			RemotSend_LogOn_buf[53] = 0x0;
			RemotSend_LogOn_buf[52] = 0x0;
			RemotSend_LogOn_buf[51] = 0x0;
			RemotSend_LogOn_buf[50] = 0x0; 			//远程控制列表
			/**			*/
			RemotSend_LogOn_buf[20] = ((59 -21) >> 8) & 0xff;
			RemotSend_LogOn_buf[21] = (59 -21) & 0xff;	//数据单元长度
			RemotSend_LogOn_buf_len = 62;		//buf 长度

			check16 = mc_check_crc16(RemotSend_LogOn_buf,60);
			RemotSend_LogOn_buf[60] = (uint8_t)(check16/256);
			RemotSend_LogOn_buf[61] = (uint8_t)(check16%256);
		}
		break;
		case 1:								//实时信息上报:1.换电站设备数据
		{
			// bzero(RemotSend_UpPLCData_buf,100);

			for(i=0;i<15;i++)
				RemotSend_UpPLCData_buf[i+4] = Station_No[i];	//换电站的编码
			RemotSend_UpPLCData_buf[19] = 0x01;
			RemotSend_UpPLCData_buf[20] = 0x00;
			RemotSend_UpPLCData_buf[21] = 0x10;  		//数据单元长度
			RemotSend_UpPLCData_buf[22] = p->tm_year-100;
			RemotSend_UpPLCData_buf[23] = p->tm_mon+1;
			RemotSend_UpPLCData_buf[24] = p->tm_mday;
			RemotSend_UpPLCData_buf[25] = p->tm_hour;
			RemotSend_UpPLCData_buf[26] = p->tm_min;
			RemotSend_UpPLCData_buf[27] = p->tm_sec;
			RemotSend_UpPLCData_buf[28] = 0x01;			//1.换电站设备数据 
			RemotSend_UpPLCData_buf[29] = MCE_RunState;
			RemotSend_UpPLCData_buf[30] = MCE_RGVSenser_sta;
			RemotSend_UpPLCData_buf[31] = MCE_StackerSenser_sta; 
			RemotSend_UpPLCData_buf[32] = MCE_RGVStepData;
			RemotSend_UpPLCData_buf[33] = MCE_StackerStepData;
			RemotSend_UpPLCData_buf[34] = MCE_ParkPlatStepData; 
			RemotSend_UpPLCData_buf[35] = CE_ErrData[0];
			RemotSend_UpPLCData_buf[36] = CE_ErrData[1];
			if(CE_ErrData[0]==0&&CE_ErrData[1]==0)
				RemotSend_UpPLCData_buf[37] = 0;   		  //???故障等级
			else RemotSend_UpPLCData_buf[37] = 3;   

			RemotSend_UpPLCData_buf[38] = (MQt_StartSelfCE_sta==1);
			RemotSend_UpPLCData_buf[20] =  ((38 -21) >> 8) & 0xff;
			RemotSend_UpPLCData_buf[21] =  ((38 -21) >> 0) & 0xff;	//数据单元长度

			check16 = mc_check_crc16(RemotSend_UpPLCData_buf,39);
			RemotSend_UpPLCData_buf[39] = (uint8_t)(check16/256);
			RemotSend_UpPLCData_buf[40] = (uint8_t)(check16%256);
			RemotSend_UpPLCData_buf_len = 41;		//buf 长度
		}
		break;
		case 2:								//实时信息上报:2.交流电表数据
		{
			// bzero(RemotSend_UpACEngData_buf,200);

			for(i=0;i<15;i++)
				RemotSend_UpACEngData_buf[i+4] = Station_No[i];	//换电站的编码
			RemotSend_UpACEngData_buf[19] = 0x01;

			int j=20;
			RemotSend_UpACEngData_buf[j++] = 0x00;
			RemotSend_UpACEngData_buf[j++] = 0x10;  		//数据单元长度
			RemotSend_UpACEngData_buf[j++] = p->tm_year-100;
			RemotSend_UpACEngData_buf[j++] = p->tm_mon+1;
			RemotSend_UpACEngData_buf[j++] = p->tm_mday;
			RemotSend_UpACEngData_buf[j++] = p->tm_hour;
			RemotSend_UpACEngData_buf[j++] = p->tm_min;
			RemotSend_UpACEngData_buf[j++] = p->tm_sec;
			RemotSend_UpACEngData_buf[j++] = 0x02;			//2交流电表数据
			
			RemotSend_UpACEngData_buf[j++] = (uint8_t)(((int)ac_GroupActiveTotalEnergy)>>24 & 0xff);
			RemotSend_UpACEngData_buf[j++] = (uint8_t)(((int)ac_GroupActiveTotalEnergy)>>16 & 0xff);
			RemotSend_UpACEngData_buf[j++] = (uint8_t)(((int)ac_GroupActiveTotalEnergy)>>8  & 0xff); 
			RemotSend_UpACEngData_buf[j++] = (uint8_t)(((int)ac_GroupActiveTotalEnergy)>>0  & 0xff);
			RemotSend_UpACEngData_buf[j++] = (uint8_t)(((int)ac_PositiveActiveTotalEnergy)>>24 & 0xff);
			RemotSend_UpACEngData_buf[j++] = (uint8_t)(((int)ac_PositiveActiveTotalEnergy)>>16 & 0xff); 
			RemotSend_UpACEngData_buf[j++] = (uint8_t)(((int)ac_PositiveActiveTotalEnergy)>>8 & 0xff);
			RemotSend_UpACEngData_buf[j++] = (uint8_t)(((int)ac_PositiveActiveTotalEnergy)>>0 & 0xff);
			RemotSend_UpACEngData_buf[j++] = (uint8_t)(((int)ac_NegativeActiveTotalEnergy)>>24 & 0xff);
			RemotSend_UpACEngData_buf[j++] = (uint8_t)(((int)ac_NegativeActiveTotalEnergy)>>16 & 0xff); 
			RemotSend_UpACEngData_buf[j++] = (uint8_t)(((int)ac_NegativeActiveTotalEnergy)>>8 & 0xff);
			RemotSend_UpACEngData_buf[j++] = (uint8_t)(((int)ac_NegativeActiveTotalEnergy)>>0 & 0xff);
			RemotSend_UpACEngData_buf[j++] = (uint8_t)(((int)ac_GroupReactiveTotalEnergy)>>24 & 0xff);
			RemotSend_UpACEngData_buf[j++] = (uint8_t)(((int)ac_GroupReactiveTotalEnergy)>>16 & 0xff); 
			RemotSend_UpACEngData_buf[j++] = (uint8_t)(((int)ac_GroupReactiveTotalEnergy)>>8 & 0xff);
			RemotSend_UpACEngData_buf[j++] = (uint8_t)(((int)ac_GroupReactiveTotalEnergy)>>0 & 0xff);
			RemotSend_UpACEngData_buf[j++] = (uint8_t)(((int)ac_PositiveReactiveTotalEnergy)>>24 & 0xff);
			RemotSend_UpACEngData_buf[j++] = (uint8_t)(((int)ac_PositiveReactiveTotalEnergy)>>16 & 0xff); 
			RemotSend_UpACEngData_buf[j++] = (uint8_t)(((int)ac_PositiveReactiveTotalEnergy)>>8 & 0xff);
			RemotSend_UpACEngData_buf[j++] = (uint8_t)(((int)ac_PositiveReactiveTotalEnergy)>>0 & 0xff);
			RemotSend_UpACEngData_buf[j++] = (uint8_t)(((int)ac_NegativeReactiveTotalEnergy)>>24 & 0xff);
			RemotSend_UpACEngData_buf[j++] = (uint8_t)(((int)ac_NegativeReactiveTotalEnergy)>>16 & 0xff); 
			RemotSend_UpACEngData_buf[j++] = (uint8_t)(((int)ac_NegativeReactiveTotalEnergy)>>8 & 0xff);
			RemotSend_UpACEngData_buf[j++] = (uint8_t)(((int)ac_NegativeReactiveTotalEnergy)>>0 & 0xff);
			RemotSend_UpACEngData_buf[j++] = (uint8_t)(((int)acA_Voltage)>>8 & 0xff);
			RemotSend_UpACEngData_buf[j++] = (uint8_t)(((int)acA_Voltage)>>0 & 0xff);
			RemotSend_UpACEngData_buf[j++] = (uint8_t)(((int)acB_Voltage)>>8 & 0xff);
			RemotSend_UpACEngData_buf[j++] = (uint8_t)(((int)acB_Voltage)>>0 & 0xff);
			RemotSend_UpACEngData_buf[j++] = (uint8_t)(((int)acC_Voltage)>>8 & 0xff);
			RemotSend_UpACEngData_buf[j++] = (uint8_t)(((int)acC_Voltage)>>0 & 0xff);
			RemotSend_UpACEngData_buf[j++] = (uint8_t)(((int)acA_Current)>>24 & 0xff);
			RemotSend_UpACEngData_buf[j++] = (uint8_t)(((int)acA_Current)>>16 & 0xff); 
			RemotSend_UpACEngData_buf[j++] = (uint8_t)(((int)acA_Current)>>8 & 0xff);
			RemotSend_UpACEngData_buf[j++] = (uint8_t)(((int)acA_Current)>>0 & 0xff);
			RemotSend_UpACEngData_buf[j++] = (uint8_t)(((int)acB_Current)>>24 & 0xff);
			RemotSend_UpACEngData_buf[j++] = (uint8_t)(((int)acB_Current)>>16 & 0xff); 
			RemotSend_UpACEngData_buf[j++] = (uint8_t)(((int)acB_Current)>>8 & 0xff);
			RemotSend_UpACEngData_buf[j++] = (uint8_t)(((int)acB_Current)>>0 & 0xff);
			RemotSend_UpACEngData_buf[j++] = (uint8_t)(((int)acC_Current)>>24 & 0xff);
			RemotSend_UpACEngData_buf[j++] = (uint8_t)(((int)acC_Current)>>16 & 0xff); 
			RemotSend_UpACEngData_buf[j++] = (uint8_t)(((int)acC_Current)>>8 & 0xff);
			RemotSend_UpACEngData_buf[j++] = (uint8_t)(((int)acC_Current)>>0 & 0xff);
			RemotSend_UpACEngData_buf[j++] = (uint8_t)(((int)acPt)>>8 & 0xff);
			RemotSend_UpACEngData_buf[j++] = (uint8_t)(((int)acPt)>>0 & 0xff);
			RemotSend_UpACEngData_buf[j++] = (uint8_t)(((int)acCt)>>8 & 0xff);
			RemotSend_UpACEngData_buf[j++] = (uint8_t)(((int)acCt)>>0 & 0xff);
			RemotSend_UpACEngData_buf[j++] = (uint8_t)(((int)acA_ActivePower)>>24 & 0xff);
			RemotSend_UpACEngData_buf[j++] = (uint8_t)(((int)acA_ActivePower)>>16 & 0xff); 
			RemotSend_UpACEngData_buf[j++] = (uint8_t)(((int)acA_ActivePower)>>8 & 0xff);
			RemotSend_UpACEngData_buf[j++] = (uint8_t)(((int)acA_ActivePower)>>0 & 0xff);

			RemotSend_UpACEngData_buf[j++] = (uint8_t)(((int)acB_ActivePower)>>24 & 0xff);
			RemotSend_UpACEngData_buf[j++] = (uint8_t)(((int)acB_ActivePower)>>16 & 0xff); 
			RemotSend_UpACEngData_buf[j++] = (uint8_t)(((int)acB_ActivePower)>>8 & 0xff);
			RemotSend_UpACEngData_buf[j++] = (uint8_t)(((int)acB_ActivePower)>>0 & 0xff);
			RemotSend_UpACEngData_buf[j++] = (uint8_t)(((int)acC_ActivePower)>>24 & 0xff);
			RemotSend_UpACEngData_buf[j++] = (uint8_t)(((int)acC_ActivePower)>>16 & 0xff); 
			RemotSend_UpACEngData_buf[j++] = (uint8_t)(((int)acC_ActivePower)>>8 & 0xff);
			RemotSend_UpACEngData_buf[j++] = (uint8_t)(((int)acC_ActivePower)>>0 & 0xff);
			RemotSend_UpACEngData_buf[j++] = (uint8_t)(((int)acA_ReactivePower)>>24 & 0xff);
			RemotSend_UpACEngData_buf[j++] = (uint8_t)(((int)acA_ReactivePower)>>16 & 0xff); 
			RemotSend_UpACEngData_buf[j++] = (uint8_t)(((int)acA_ReactivePower)>>8 & 0xff);
			RemotSend_UpACEngData_buf[j++] = (uint8_t)(((int)acA_ReactivePower)>>0 & 0xff);
			RemotSend_UpACEngData_buf[j++] = (uint8_t)(((int)acB_ReactivePower)>>24 & 0xff);
			RemotSend_UpACEngData_buf[j++] = (uint8_t)(((int)acB_ReactivePower)>>16 & 0xff); 
			RemotSend_UpACEngData_buf[j++] = (uint8_t)(((int)acB_ReactivePower)>>8 & 0xff);
			RemotSend_UpACEngData_buf[j++] = (uint8_t)(((int)acB_ReactivePower)>>0 & 0xff);
			RemotSend_UpACEngData_buf[j++] = (uint8_t)(((int)acC_ReactivePower)>>24 & 0xff);
			RemotSend_UpACEngData_buf[j++] = (uint8_t)(((int)acC_ReactivePower)>>16 & 0xff); 
			RemotSend_UpACEngData_buf[j++] = (uint8_t)(((int)acC_ReactivePower)>>8 & 0xff);
			RemotSend_UpACEngData_buf[j++] = (uint8_t)(((int)acC_ReactivePower)>>0 & 0xff);
			RemotSend_UpACEngData_buf[j++] = (uint8_t)(((int)acA_PhasePowerFactor)>>8 & 0xff);
			RemotSend_UpACEngData_buf[j++] = (uint8_t)(((int)acA_PhasePowerFactor)>>0 & 0xff);
			RemotSend_UpACEngData_buf[j++] = (uint8_t)(((int)acB_PhasePowerFactor)>>8 & 0xff);
			RemotSend_UpACEngData_buf[j++] = (uint8_t)(((int)acB_PhasePowerFactor)>>0 & 0xff);
			RemotSend_UpACEngData_buf[j++] = (uint8_t)(((int)acC_PhasePowerFactor)>>8 & 0xff);
			RemotSend_UpACEngData_buf[j++] = (uint8_t)(((int)acC_PhasePowerFactor)>>0 & 0xff);
			RemotSend_UpACEngData_buf[j++] = 0x00;
			RemotSend_UpACEngData_buf[j++] = 0x00;
			RemotSend_UpACEngData_buf[20] = (((j-1)-21) >> 8) & 0xff;
			RemotSend_UpACEngData_buf[21] = (((j-1)-21) >> 0) & 0xff;	//数据单元长度
			
			check16 = mc_check_crc16(RemotSend_UpACEngData_buf,j);
			RemotSend_UpACEngData_buf[j++] = (uint8_t)(check16/256);
			RemotSend_UpACEngData_buf[j++] = (uint8_t)(check16%256);
			RemotSend_UpACEngData_buf_len = j;		//buf 长度
		}
		break;
		case 3:								//实时信息上报:3.空调数据
		{
			// bzero(RemotSend_UpAirConData_buf,200);
			for(i=0;i<15;i++)
				RemotSend_UpAirConData_buf[i+4] = Station_No[i];	//换电站的编码
			RemotSend_UpAirConData_buf[19] = 0x01;

			int j=20;
			RemotSend_UpAirConData_buf[j++] = 0x00;
			RemotSend_UpAirConData_buf[j++] = 0x10;  		//数据单元长度
			RemotSend_UpAirConData_buf[j++] = p->tm_year-100;
			RemotSend_UpAirConData_buf[j++] = p->tm_mon+1;
			RemotSend_UpAirConData_buf[j++] = p->tm_mday;
			RemotSend_UpAirConData_buf[j++] = p->tm_hour;
			RemotSend_UpAirConData_buf[j++] = p->tm_min;
			RemotSend_UpAirConData_buf[j++] = p->tm_sec;
			RemotSend_UpAirConData_buf[j++] = 0x03;			//3.空调数据
			RemotSend_UpAirConData_buf[j++] = 0x03;	//空调个数

			int k=0;
			for(k=0;k<3;k++)
			{
				RemotSend_UpAirConData_buf[j++] = k+1;//空调编号
				RemotSend_UpAirConData_buf[j++] = ((uint16_t)(AirCon_Temp[k]+180) >>8) & 0xff;//环境温度
				RemotSend_UpAirConData_buf[j++] = (uint16_t)(AirCon_Temp[k]+180) & 0xff;
				RemotSend_UpAirConData_buf[j++] = ((uint16_t)(AirCon_Humi[k]+0) >>8) & 0xff;//柜内回风温度
				RemotSend_UpAirConData_buf[j++] =  (uint16_t)(AirCon_Humi[k]+0) & 0xff; 
				RemotSend_UpAirConData_buf[j++] = ((uint16_t)(AirRead_EnginStartTemp_C[k]+180) >>8) & 0xff;//环境温度
				RemotSend_UpAirConData_buf[j++] = (uint16_t)(AirRead_EnginStartTemp_C[k]+180) & 0xff;
				RemotSend_UpAirConData_buf[j++] = ((uint16_t)(AirRead_HertStartTemp_C[k]+180) >>8) & 0xff;//环境温度
				RemotSend_UpAirConData_buf[j++] = (uint16_t)(AirRead_HertStartTemp_C[k]+180) & 0xff;

				RemotSend_UpAirConData_buf[j++] = (AirCon_MerRunSta[k]+(AirCon_EnginRunSta[k] << 2)
												+ (AirCon_HertRunSta[k] << 3));//空调运行状态
                
				RemotSend_UpAirConData_buf[j++] = AirCon_Err1[k];//空调故障
				RemotSend_UpAirConData_buf[j++] = AirCon_Err2[k];
				RemotSend_UpAirConData_buf[j++] = AirCon_Err3[k];
				if(AirCon_Err1[k] == 0 && AirCon_Err2[k] == 0 && AirCon_Err3[k] == 0)
				{	RemotSend_UpAirConData_buf[j++] = 0x00;//故障等级
				}
				else{
					RemotSend_UpAirConData_buf[j++] = 0x01;//故障等级
				}
			}
			RemotSend_UpAirConData_buf[20] = (((j-1)-21) >> 8) & 0xff;
			RemotSend_UpAirConData_buf[21] = (((j-1)-21) >> 0) & 0xff;	//数据单元长度

			check16 = mc_check_crc16(RemotSend_UpAirConData_buf,j);
			RemotSend_UpAirConData_buf[j++] = (uint8_t)(check16/256);
			RemotSend_UpAirConData_buf[j++] = (uint8_t)(check16%256);
			RemotSend_UpAirConData_buf_len = j;		//buf 长度
		}
		break;
		case 4:								//实时信息上报:4 风机数据
		{
			// bzero(RemotSend_UpFanData_buf,100);
			for(i=0;i<15;i++)
				RemotSend_UpFanData_buf[i+4] = Station_No[i];	//换电站的编码
			RemotSend_UpFanData_buf[19] = 0x01;

			int j=20;
			RemotSend_UpFanData_buf[j++] = 0x00;
			RemotSend_UpFanData_buf[j++] = 0x10;  		//数据单元长度
			RemotSend_UpFanData_buf[j++] = p->tm_year-100;
			RemotSend_UpFanData_buf[j++] = p->tm_mon+1;
			RemotSend_UpFanData_buf[j++] = p->tm_mday;
			RemotSend_UpFanData_buf[j++] = p->tm_hour;
			RemotSend_UpFanData_buf[j++] = p->tm_min;
			RemotSend_UpFanData_buf[j++] = p->tm_sec;
			RemotSend_UpFanData_buf[j++] = 0x04;		//4 风机数据
			RemotSend_UpFanData_buf[j++] = 0x6;/* 换电站的风机个数 */
			int k=0;
			for(k=0;k<6;k++)
			{
				/* 0 bit 风机的类型；1：保温风扇   0：散热风扇*/
				/* 1 bit 风机状态；1：运行   0：停止 */
				/* 2-7 bit 保留*/
				RemotSend_UpFanData_buf[j++] = 0x0;	
			}
			RemotSend_UpFanData_buf[20] = (((j-1)-21) >> 8) & 0xff;
			RemotSend_UpFanData_buf[21] = (((j-1)-21) >> 0) & 0xff;	//数据单元长度

			check16 = mc_check_crc16(RemotSend_UpFanData_buf,j);
			RemotSend_UpFanData_buf[j++] = (uint8_t)(check16/256);
			RemotSend_UpFanData_buf[j++] = (uint8_t)(check16%256);
			RemotSend_UpFanData_buf_len = j;		//buf 长度
		}
		break;
		case 5:								//实时信息上报:5 消防、门禁、水禁数据
		{
			// bzero(RemotSend_UpFireData_buf,100);
			for(i=0;i<15;i++)
				RemotSend_UpFireData_buf[i+4] = Station_No[i];	//换电站的编码
			RemotSend_UpFireData_buf[19] = 0x01;

			int j=20;
			RemotSend_UpFireData_buf[j++] = 0x00;
			RemotSend_UpFireData_buf[j++] = 0x10;  		//数据单元长度
			RemotSend_UpFireData_buf[j++] = p->tm_year-100;
			RemotSend_UpFireData_buf[j++] = p->tm_mon+1;
			RemotSend_UpFireData_buf[j++] = p->tm_mday;
			RemotSend_UpFireData_buf[j++] = p->tm_hour;
			RemotSend_UpFireData_buf[j++] = p->tm_min;
			RemotSend_UpFireData_buf[j++] = p->tm_sec;
			RemotSend_UpFireData_buf[j++] = 0x05;		//5消防、门禁、水禁数据

			if(FireContr_GetFire_flg == 1)
			{
				RemotSend_UpFireData_buf[j++] = 2;// 消防故障
				RemotSend_UpFireData_buf[j++] = 0x4;// 消防故障等级
			}
			else if(FireContr_HardErr_flg == 1)
			{
				RemotSend_UpFireData_buf[j++] = 1;// 消防故障
				RemotSend_UpFireData_buf[j++] = 0x1;// 消防故障等级
			}
			else
			{
				RemotSend_UpFireData_buf[j++] = 0;// 消防故障
				RemotSend_UpFireData_buf[j++] = 0x0;// 消防故障等级
			}
			
			RemotSend_UpFireData_buf[j++] = 1+(MIO_DoorErr_buf[0] || MIO_DoorErr_buf[1] || MIO_DoorErr_buf[2]) ;// 门禁状态
			RemotSend_UpFireData_buf[j++] = MIO_Water_err;// 水浸状态
			RemotSend_UpFireData_buf[j++] = MIO_ThunderErr_buf[0] || MIO_ThunderErr_buf[1];// 防雷故障

			RemotSend_UpFireData_buf[20] = (((j-1)-21) >> 8) & 0xff;
			RemotSend_UpFireData_buf[21] = (((j-1)-21) >> 0) & 0xff;	//数据单元长度
			
			check16 = mc_check_crc16(RemotSend_UpFireData_buf,j);
			RemotSend_UpFireData_buf[j++] = (uint8_t)(check16/256);
			RemotSend_UpFireData_buf[j++] = (uint8_t)(check16%256);	
			RemotSend_UpFireData_buf_len = j;		//buf 长度
		}
		break;
		case 6:								//实时信息上报:6 汽车换电结果数据
		{
			// bzero(RemotSend_UpCEData_buf,100);
			for(i=0;i<15;i++)
				RemotSend_UpCEData_buf[i+4] = Station_No[i];	//换电站的编码
			RemotSend_UpCEData_buf[19] = 0x01;
			int j=20;
			RemotSend_UpCEData_buf[j++] = 0x00;
			RemotSend_UpCEData_buf[j++] = 0x10;  		//数据单元长度
			RemotSend_UpCEData_buf[j++] = p->tm_year-100;
			RemotSend_UpCEData_buf[j++] = p->tm_mon+1;
			RemotSend_UpCEData_buf[j++] = p->tm_mday;
			RemotSend_UpCEData_buf[j++] = p->tm_hour;
			RemotSend_UpCEData_buf[j++] = p->tm_min;
			RemotSend_UpCEData_buf[j++] = p->tm_sec;
			RemotSend_UpCEData_buf[j++] = 0x06;		//6 汽车换电结果数据

			int k=0;							
			for(k=0;k<16;k++)
				{RemotSend_UpCEData_buf[j++] = Cloud_Grab_carNo[k];}// 车牌号	16
			
			RemotSend_UpCEData_buf[j++] = 0x01;	// 换电结果	1

			RemotSend_UpCEData_buf[j++] = p->tm_year-100;
			RemotSend_UpCEData_buf[j++] = p->tm_mon+1;
			RemotSend_UpCEData_buf[j++] = p->tm_mday;
			RemotSend_UpCEData_buf[j++] = p->tm_hour;
			RemotSend_UpCEData_buf[j++] = p->tm_min;
			RemotSend_UpCEData_buf[j++] = p->tm_sec;
			// for(k=0;k<6;k++)
			// 	{RemotSend_UpCEData_buf[j++] = 0x00;}// 换电完成时间	6
										
			for(k=0;k<32;k++)
				{RemotSend_UpCEData_buf[j++] = SourseStorage_Batt_NUM[k];}// 换上电池包编号	32						

			for(k=0;k<32;k++)
				{RemotSend_UpCEData_buf[j++] = Cloud_BatteryNo_now[k];}// 换下电池包编号	32

			RemotSend_UpCEData_buf[j++] = MChar_SelfCe_sour+1;		// 换上电池包舱位编号	1
			RemotSend_UpCEData_buf[j++] = MChar_SelfCe_sour+1;		// 换下电池包舱位编号	1
			RemotSend_UpCEData_buf[j++] = RBCar_Batt_Soc;		// 换下电池SOC	1
			RemotSend_UpCEData_buf[j++] = ReplaceBatt_SOC;		// 换上电池SOC	1
			RemotSend_UpCEData_buf[j++] = (uint16_t)(RBCar_Batt_Available_Cap*100) >> 8;	// 换下电池可用容量	2
			RemotSend_UpCEData_buf[j++] = (uint16_t)(RBCar_Batt_Available_Cap*100) & 0xff;
			RemotSend_UpCEData_buf[j++] = (uint16_t)(ReplaceBatt_Available_Cap*100) >> 8;		// 换上电池可用容量	2
			RemotSend_UpCEData_buf[j++] = (uint16_t)(ReplaceBatt_Available_Cap*100) & 0xff;
			RemotSend_UpCEData_buf[j++] = (uint16_t)(RBCar_Batt_Available_Energy*100) >> 8;	// 换下电池可用能量	2
			RemotSend_UpCEData_buf[j++] = (uint16_t)(RBCar_Batt_Available_Energy*100) & 0xff;
			RemotSend_UpCEData_buf[j++] = (uint16_t)(ReplaceBatt_Available_Energy*100) >> 8;	// 换上电池可用能量	2
			RemotSend_UpCEData_buf[j++] = (uint16_t)(ReplaceBatt_Available_Energy*100) & 0xff;

			RemotSend_UpCEData_buf[20] = (((j-1)-21) >> 8) & 0xff;
			RemotSend_UpCEData_buf[21] = (((j-1)-21) >> 0) & 0xff;  		//数据单元长度

			check16 = mc_check_crc16(RemotSend_UpCEData_buf,j);
			RemotSend_UpCEData_buf[j++] = (uint8_t)(check16/256);
			RemotSend_UpCEData_buf[j++] = (uint8_t)(check16%256);		
			RemotSend_UpCEData_buf_len = j;//buf len

		}
		break;
		case 7:								//实时信息上报:7 未报警充电机设备数据
		{
			// bzero(RemotSend_UpCHUData_buf,100);
			for(i=0;i<15;i++)
				RemotSend_UpCHUData_buf[i+4] = Station_No[i];	//换电站的编码
			RemotSend_UpCHUData_buf[19] = 0x01;		
			RemotSend_UpCHUData_buf[22] = p->tm_year-100;
			RemotSend_UpCHUData_buf[23] = p->tm_mon+1;
			RemotSend_UpCHUData_buf[24] = p->tm_mday;
			RemotSend_UpCHUData_buf[25] = p->tm_hour;
			RemotSend_UpCHUData_buf[26] = p->tm_min;
			RemotSend_UpCHUData_buf[27] = p->tm_sec;
			RemotSend_UpCHUData_buf[28] = 0x07;		//7 未报警充电机设备数据
			RemotSend_UpCHUData_buf[29] = 1;/* 电池未报警的充电机设备个数 */		
			int index=0;int j=30;
			index = index_Chu;
			// index = 10;
			// for(index = 0;index<1;index++)
			{			
				RemotSend_UpCHUData_buf[j++] = p->tm_year-100;		// 时间	6
				RemotSend_UpCHUData_buf[j++] = p->tm_mon+1;
				RemotSend_UpCHUData_buf[j++] = p->tm_mday;
				RemotSend_UpCHUData_buf[j++] = p->tm_hour;
				RemotSend_UpCHUData_buf[j++] = p->tm_min;
				RemotSend_UpCHUData_buf[j++] = p->tm_sec;	
				RemotSend_UpCHUData_buf[j++] = index+1;	// 充电机编号	1		

				RemotSend_UpCHUData_buf[j++] = CHU_DevState_code[index];	// 状态码	1  buf[37]
				RemotSend_UpCHUData_buf[j++] = MChg_ErrStop_code[index];	// 自停止故障	1
				RemotSend_UpCHUData_buf[j++] = 0x00;	// 心跳故障	1
				RemotSend_UpCHUData_buf[j++] = 0x00;	// 故障等级	1
				RemotSend_UpCHUData_buf[j++] = CHU_BattType[index];	// 电池包类型	1
				RemotSend_UpCHUData_buf[j++] = CHU_BMS_LinkState[index];	// BMS连接状态	1
				RemotSend_UpCHUData_buf[j++] = CHU_NegTemp_C[index];	// 正极柱温度	1
				RemotSend_UpCHUData_buf[j++] = CHU_PosTemp_C[index];	// 负极柱温度	1
				RemotSend_UpCHUData_buf[j++] = (uint8_t)((((int)DCEnVol_V[index]*100)>> 24) & 0xff);	// 直流电压	4
				RemotSend_UpCHUData_buf[j++] = (uint8_t)((((int)DCEnVol_V[index]*100)>> 16) & 0xff);
				RemotSend_UpCHUData_buf[j++] = (uint8_t)((((int)DCEnVol_V[index]*100)>>8 ) & 0xff);
				RemotSend_UpCHUData_buf[j++] = (uint8_t)((((int)DCEnVol_V[index]*100)>>0 ) & 0xff);		
				RemotSend_UpCHUData_buf[j++] = (uint8_t)((((int)DCEnCur_A[index]*100)>>24) & 0xff);		// 直流电流	4
				RemotSend_UpCHUData_buf[j++] = (uint8_t)((((int)DCEnCur_A[index]*100)>>16) & 0xff);
				RemotSend_UpCHUData_buf[j++] = (uint8_t)((((int)DCEnCur_A[index]*100)>> 8 ) & 0xff);
				RemotSend_UpCHUData_buf[j++] = (uint8_t)((((int)DCEnCur_A[index]*100)>> 0 ) & 0xff);              
				RemotSend_UpCHUData_buf[j++] = (uint8_t)((((int)DCEn_Act_W[index]*10000)>>24) & 0xff);		// 有功功率	4
				RemotSend_UpCHUData_buf[j++] = (uint8_t)((((int)DCEn_Act_W[index]*10000)>>16) & 0xff);
				RemotSend_UpCHUData_buf[j++] = (uint8_t)((((int)DCEn_Act_W[index]*10000)>> 8 ) & 0xff);
				RemotSend_UpCHUData_buf[j++] = (uint8_t)((((int)DCEn_Act_W[index]*10000)>> 0 ) & 0xff);				     
			    RemotSend_UpCHUData_buf[j++] = (uint8_t)((((int)DCEnPos_Act_W[index]*100)>>24) & 0xff);		// 正向有功电能	4
				RemotSend_UpCHUData_buf[j++] = (uint8_t)((((int)DCEnPos_Act_W[index]*100)>>16) & 0xff);
				RemotSend_UpCHUData_buf[j++] = (uint8_t)((((int)DCEnPos_Act_W[index]*100)>> 8 ) & 0xff);
				RemotSend_UpCHUData_buf[j++] = (uint8_t)((((int)DCEnPos_Act_W[index]*100)>> 0 ) & 0xff);	
				RemotSend_UpCHUData_buf[j++] = (uint8_t)((((int)DCEnNeg_Act_W[index]*100)>>24) & 0xff);		// 反向有功电能	4
				RemotSend_UpCHUData_buf[j++] = (uint8_t)((((int)DCEnNeg_Act_W[index]*100)>>16) & 0xff);
				RemotSend_UpCHUData_buf[j++] = (uint8_t)((((int)DCEnNeg_Act_W[index]*100)>> 8 ) & 0xff);
				RemotSend_UpCHUData_buf[j++] = (uint8_t)((((int)DCEnNeg_Act_W[index]*100)>> 0 ) & 0xff);		
				int k=0;
				for(k=0;k<32;k++)	
					RemotSend_UpCHUData_buf[j++] = BMS_BATT_NUM[index][k];// 动力电池编码	32		
				RemotSend_UpCHUData_buf[j++] = (uint8_t)((((int)BMS_CharPackVolReal_V[index]*10) >> 8 ) & 0xff);	// 电压输出值	2
				RemotSend_UpCHUData_buf[j++] = (uint8_t)((((int)BMS_CharPackVolReal_V[index]*10) >> 0 ) & 0xff);
				RemotSend_UpCHUData_buf[j++] = (uint8_t)((((int)BMS_CharPackCurReal_A[index]*10) >> 8 ) & 0xff);	// 电流输出值	2
				RemotSend_UpCHUData_buf[j++] = (uint8_t)((((int)BMS_CharPackCurReal_A[index]*10) >> 0 ) & 0xff);
				RemotSend_UpCHUData_buf[j++] = (uint8_t)((((int)BMS_ReqPackVol_V[index]*10) >> 8 ) & 0xff);	// BMS需求电压	2
				RemotSend_UpCHUData_buf[j++] = (uint8_t)((((int)BMS_ReqPackVol_V[index]*10) >> 0 ) & 0xff);
				RemotSend_UpCHUData_buf[j++] = (uint8_t)((((int)BMS_ReqPackCur_A[index]*10) >> 8 ) & 0xff);	// BMS需求电流	2
				RemotSend_UpCHUData_buf[j++] = (uint8_t)((((int)BMS_ReqPackCur_A[index]*10) >> 0 ) & 0xff);
				RemotSend_UpCHUData_buf[j++] = 2;	// 充电模式	1 CHU_ChgMode[index]
				RemotSend_UpCHUData_buf[j++] = (uint8_t)(((int)Charger_OutPwr_W[index] >> 8 ) & 0xff);	// 实际输出功率	2
				RemotSend_UpCHUData_buf[j++] = (uint8_t)(((int)Charger_OutPwr_W[index] >> 0 ) & 0xff);
				RemotSend_UpCHUData_buf[j++] = (uint8_t)(((int)BMS_RemainCharTimeReal_min[index] >> 8 ) & 0xff);	// 估算剩余充电时间	2
				RemotSend_UpCHUData_buf[j++] = (uint8_t)(((int)BMS_RemainCharTimeReal_min[index] >> 0 ) & 0xff);
				RemotSend_UpCHUData_buf[j++] = (uint8_t)(((int)ChU_AllChrTimeReal_min[index] >> 8 ) & 0xff);	// 单次累计充电时间	2
				RemotSend_UpCHUData_buf[j++] = (uint8_t)(((int)ChU_AllChrTimeReal_min[index] >> 0 ) & 0xff);
				RemotSend_UpCHUData_buf[j++] = BMS_BatteryType_Null[index];	// 电池类型	1
				RemotSend_UpCHUData_buf[j++] = (uint8_t)((((int)BMS_RatedCap_Ah[index]*10) >> 8 ) & 0xff);	// 整车动力蓄电池系统额定容量	2
				RemotSend_UpCHUData_buf[j++] = (uint8_t)((((int)BMS_RatedCap_Ah[index]*10) >> 0 ) & 0xff);
				RemotSend_UpCHUData_buf[j++] = (uint8_t)((((int)BMS_RatedPackVol_V[index]*10) >> 8 ) & 0xff);	// 整车动力蓄电池系统额定总电压	2
				RemotSend_UpCHUData_buf[j++] = (uint8_t)((((int)BMS_RatedPackVol_V[index]*10) >> 0 ) & 0xff);
				RemotSend_UpCHUData_buf[j++] = 255;//(uint8_t)(((int)Charger_OutPwr_W[index] >> 16) & 0xff);;	// 电池组充电次数	4
				RemotSend_UpCHUData_buf[j++] = 255;//(uint8_t)(((int)Charger_OutPwr_W[index] >> 16) & 0xff);
				RemotSend_UpCHUData_buf[j++] = 255;//(uint8_t)(((int)Charger_OutPwr_W[index] >> 8 ) & 0xff);
				RemotSend_UpCHUData_buf[j++] = 255;//(uint8_t)(((int)Charger_OutPwr_W[index] >> 0 ) & 0xff);
				RemotSend_UpCHUData_buf[j++] = (uint8_t)((((int)CellVol_AllowCharMax_V[index]*100) >> 8 ) & 0xff);	//??? 单体动力蓄电池最高允许充电电压	2
				RemotSend_UpCHUData_buf[j++] = (uint8_t)((((int)CellVol_AllowCharMax_V[index]*100) >> 0 ) & 0xff);
				RemotSend_UpCHUData_buf[j++] = (uint8_t)((((int)PackCur_AllowCharMax_A[index]*10) >> 8 ) & 0xff);	// 最高允许充电电流	2 ??
				RemotSend_UpCHUData_buf[j++] = (uint8_t)((((int)PackCur_AllowCharMax_A[index]*10) >> 0 ) & 0xff);
				RemotSend_UpCHUData_buf[j++] = (uint8_t)((((int)BMS_RatedEnergy_Kwh[index]*10) >> 8 ) & 0xff);	// 动力蓄电池标称总能量	2 ??
				RemotSend_UpCHUData_buf[j++] = (uint8_t)((((int)BMS_RatedEnergy_Kwh[index]*10) >> 0 ) & 0xff); 
				RemotSend_UpCHUData_buf[j++] = Temp_AllowCharMax_C[index];	// 最高允许温度	1 ??
				RemotSend_UpCHUData_buf[j++] = Batt_SOH[index];	// 动力电池健康度（SOH）	1 ??
				RemotSend_UpCHUData_buf[j++] = BMS_SerialNum_Null[index];	// 动力电池PACK串联数	1
				RemotSend_UpCHUData_buf[j++] = BMS_ParallNum_Null[index];	// 动力电池PACK并联数	1
				RemotSend_UpCHUData_buf[j++] = (uint8_t)(((int)BMS_ODO[index] >> 24) & 0xff);	// 动力电池行驶里程	4 ??
				RemotSend_UpCHUData_buf[j++] = (uint8_t)(((int)BMS_ODO[index] >> 16) & 0xff);
				RemotSend_UpCHUData_buf[j++] = (uint8_t)(((int)BMS_ODO[index] >> 8 ) & 0xff);
				RemotSend_UpCHUData_buf[j++] = (uint8_t)(((int)BMS_ODO[index] >> 0 ) & 0xff);
				RemotSend_UpCHUData_buf[j++] = BMS_SW_VERS[index];	// BMS次软件版本号后两位	1  ??
				RemotSend_UpCHUData_buf[j++] = BMS_SW_VERS_main[index];	// BMS主软件版本号后两位	1
				RemotSend_UpCHUData_buf[j++] = BMS_HW_VERS[index];	// BMS硬件版本号后两位	1
				RemotSend_UpCHUData_buf[j++] = (uint8_t)(((int)BMS_PART_VERS[index] >> 24) & 0xff);	// 电池包总成零部件号	4 ??
				RemotSend_UpCHUData_buf[j++] = (uint8_t)(((int)BMS_PART_VERS[index] >> 16) & 0xff);
				RemotSend_UpCHUData_buf[j++] = (uint8_t)(((int)BMS_PART_VERS[index] >> 8 ) & 0xff);
				RemotSend_UpCHUData_buf[j++] = (uint8_t)(((int)BMS_PART_VERS[index] >> 0 ) & 0xff);
				RemotSend_UpCHUData_buf[j++] = BMS_BATT_SOC[index];	// SOC	1
				RemotSend_UpCHUData_buf[j++] = Rly_Sta[index];	// 继电器状态	1
				RemotSend_UpCHUData_buf[j++] = (uint8_t)((((int)BMS_BATT_VOLT_V1[index]*100) >> 8 ) & 0xff);	// 动力电池总电压V1	2
				RemotSend_UpCHUData_buf[j++] = (uint8_t)((((int)BMS_BATT_VOLT_V1[index]*100) >> 0 ) & 0xff);
				RemotSend_UpCHUData_buf[j++] = (uint8_t)((((int)BMS_BATT_VOLT_V2[index]*100) >> 8 ) & 0xff);	// 动力电池总电压V2	2
				RemotSend_UpCHUData_buf[j++] = (uint8_t)((((int)BMS_BATT_VOLT_V2[index]*100) >> 0 ) & 0xff);
				RemotSend_UpCHUData_buf[j++] = (uint8_t)((((int)BMS_BATT_VOLT_V3[index]*100) >> 8 ) & 0xff);	// 动力电池总电压V3	2
				RemotSend_UpCHUData_buf[j++] = (uint8_t)((((int)BMS_BATT_VOLT_V3[index]*100) >> 0 ) & 0xff);
				RemotSend_UpCHUData_buf[j++] = (uint8_t)((((int)BMS_BATT_CUR[index]*100) >> 8 ) & 0xff);	// 动力电池总电流	2
				RemotSend_UpCHUData_buf[j++] = (uint8_t)((((int)BMS_BATT_CUR[index]*100) >> 0 ) & 0xff);
				RemotSend_UpCHUData_buf[j++] = (uint8_t)(((int)BMS_BATT_POSITIVE_RESISTANCE[index] >> 8 ) & 0xff);	// 动力电池正极对GND绝缘电阻	2
				RemotSend_UpCHUData_buf[j++] = (uint8_t)(((int)BMS_BATT_POSITIVE_RESISTANCE[index] >> 0 ) & 0xff);
				RemotSend_UpCHUData_buf[j++] = (uint8_t)(((int)BMS_BATT_NEGATIVE_RESISTANCE[index] >> 8 ) & 0xff);	// 动力电池负极对GND绝缘电阻	2
				RemotSend_UpCHUData_buf[j++] = (uint8_t)(((int)BMS_BATT_NEGATIVE_RESISTANCE[index] >> 0 ) & 0xff);
				RemotSend_UpCHUData_buf[j++] = (uint8_t)((((int)BMS_BATT_AVAIL_ENERGY[index]*100) >> 8 ) & 0xff);	// 动力电池可用能量	2
				RemotSend_UpCHUData_buf[j++] = (uint8_t)((((int)BMS_BATT_AVAIL_ENERGY[index]*100) >> 0 ) & 0xff);
				RemotSend_UpCHUData_buf[j++] = (uint8_t)((((int)BMS_BATT_AVAIL_CAPACITY[index]*100) >> 8 ) & 0xff);	// 动力电池可用容量	2
				RemotSend_UpCHUData_buf[j++] = (uint8_t)((((int)BMS_BATT_AVAIL_CAPACITY[index]*100) >> 0 ) & 0xff);	
				RemotSend_UpCHUData_buf[j++] = (uint8_t)(((int)BMS_CHARGE_TOT_AH[index] >> 24) & 0xff);	// // 累计充电总安时	4??
				RemotSend_UpCHUData_buf[j++] = (uint8_t)(((int)BMS_CHARGE_TOT_AH[index] >> 16) & 0xff);
				RemotSend_UpCHUData_buf[j++] = (uint8_t)(((int)BMS_CHARGE_TOT_AH[index] >> 8 ) & 0xff);
				RemotSend_UpCHUData_buf[j++] = (uint8_t)(((int)BMS_CHARGE_TOT_AH[index] >> 0 ) & 0xff);
				RemotSend_UpCHUData_buf[j++] = 0x02;	// 快换电池工作模式	1
				RemotSend_UpCHUData_buf[j++] = (uint8_t)((((int)BMS_FEED_MAX_PWR[index]*100) >> 8 ) & 0xff);	// 当前状态允许最大回馈功率	2
				RemotSend_UpCHUData_buf[j++] = (uint8_t)((((int)BMS_FEED_MAX_PWR[index]*100) >> 0 ) & 0xff);
				RemotSend_UpCHUData_buf[j++] = (uint8_t)((((int)BMS_DISCHG_MAX_PWR[index]*100) >> 8 ) & 0xff);	// 当前状态允许最大放电功率	2
				RemotSend_UpCHUData_buf[j++] = (uint8_t)((((int)BMS_DISCHG_MAX_PWR[index]*100) >> 0 ) & 0xff);
				RemotSend_UpCHUData_buf[j++] = 0x00;	// 动力电池状态	1
				RemotSend_UpCHUData_buf[j++] = (uint8_t)((((int)(BMS_CELL_VOLT_MAX[index]*1000)) >> 8 ) & 0xff);	// 单体电芯最高电压	2
				RemotSend_UpCHUData_buf[j++] = (uint8_t)((((int)(BMS_CELL_VOLT_MAX[index]*1000)) >> 0 ) & 0xff);
				RemotSend_UpCHUData_buf[j++] = BMS_MAX_VOLT_NUM[index];	// 最高单体电压编号	1
				RemotSend_UpCHUData_buf[j++] = (uint8_t)((((int)(BMS_CELL_VOLT_MIN[index]*1000)) >> 8 ) & 0xff);	// 单体电芯最低电压	2
				RemotSend_UpCHUData_buf[j++] = (uint8_t)((((int)(BMS_CELL_VOLT_MIN[index]*1000)) >> 0 ) & 0xff);
				RemotSend_UpCHUData_buf[j++] = BMS_MIN_VOLT_NUM[index];	// 最低单体电压编号	1
				RemotSend_UpCHUData_buf[j++] = BMS_CELL_TEMP_MAX[index]+48;	// 最高动力蓄电池温度	1
				RemotSend_UpCHUData_buf[j++] = BMS_MAX_TEMP_NUM[index];	// 最高动力蓄电池温度检测点编号	1
				RemotSend_UpCHUData_buf[j++] = BMS_CELL_TEMP_MIN[index]+48;	// 最低动力蓄电池温度	1
				RemotSend_UpCHUData_buf[j++] = BMS_MIN_TEMP_NUM[index];	// 最低动力蓄电池温度检测点编号	1
				RemotSend_UpCHUData_buf[j++] = (uint8_t)(((int)BMS_Err_msg[index][0] >> 0 ) & 0xff);	// 动力电池故障	6
				RemotSend_UpCHUData_buf[j++] = (uint8_t)(((int)BMS_Err_msg[index][1] >> 0 ) & 0xff);
				RemotSend_UpCHUData_buf[j++] = (uint8_t)(((int)BMS_Err_msg[index][2] >> 0 ) & 0xff);
				RemotSend_UpCHUData_buf[j++] = (uint8_t)(((int)BMS_Err_msg[index][3] >> 0 ) & 0xff);
				RemotSend_UpCHUData_buf[j++] = (uint8_t)(((int)BMS_Err_msg[index][4] >> 0  ) & 0xff);
				RemotSend_UpCHUData_buf[j++] = (uint8_t)(((int)BMS_Err_msg[index][5] >> 0  ) & 0xff);
				RemotSend_UpCHUData_buf[j++] = 112;	// 单体电压监测点位个数	1	
				// printf("jjj=%d\n",j);			
				for(k=0;k<112;k++)	// 单体电压总成信息列表	2×v
				{
					RemotSend_UpCHUData_buf[j++] = (uint8_t)(((int)BMS_CellVol_buf[index][k] >> 8 ) & 0xff);
					RemotSend_UpCHUData_buf[j++] = (uint8_t)(((int)BMS_CellVol_buf[index][k] >> 0 ) & 0xff);
				}
				RemotSend_UpCHUData_buf[j++] = 32;//BATT_TEMP_SAMPLE_NUM[index];	//?? 动力电池温度点总数	1
				for(k=0;k<32;k++)	// 动力电池温度总成信息列表	2×t
				{
					RemotSend_UpCHUData_buf[j++] = BMS_CellTemp_buf[index][k];
				}
			}
			if(EMS_CHU_LinkState[index]!=1||CHU_DevState_code[index]<3)
			{
				for(i=37;i<444;i++)
					RemotSend_UpCHUData_buf[i] = 0xff;
				RemotSend_UpCHUData_buf[186] = 112;
				RemotSend_UpCHUData_buf[298] = 32;
	//			printf("errrrrrrrrr = %d %d %d\n",EMS_CHU_LinkState[index],CHU_DevState_code[index],index);
			}
			RemotSend_UpCHUData_buf[20] = (((j-1)-21) >> 8) & 0xff;
			RemotSend_UpCHUData_buf[21] = (((j-1)-21) >> 0) & 0xff;  
			check16 = mc_check_crc16(RemotSend_UpCHUData_buf,j);
			RemotSend_UpCHUData_buf[j++] = (uint8_t)(check16/256);
			RemotSend_UpCHUData_buf[j++] = (uint8_t)(check16%256);	
			RemotSend_UpCHUData_buf_len = j;	
		}
		break;
		case 8:								//实时信息上报:8 电池包报警的充电机设备信息
		{
			// bzero(RemotSend_UpCHUData_buf,100);
			for(i=0;i<15;i++)
				RemotSend_UpErrCHUData_buf[i+4] = Station_No[i];	//换电站的编码
			RemotSend_UpErrCHUData_buf[19] = 0x01;		
			RemotSend_UpErrCHUData_buf[22] = p->tm_year-100;
			RemotSend_UpErrCHUData_buf[23] = p->tm_mon+1;
			RemotSend_UpErrCHUData_buf[24] = p->tm_mday;
			RemotSend_UpErrCHUData_buf[25] = p->tm_hour;
			RemotSend_UpErrCHUData_buf[26] = p->tm_min;
			RemotSend_UpErrCHUData_buf[27] = p->tm_sec;
			RemotSend_UpErrCHUData_buf[28] = 0x08;		//8 报警充电机设备数据
			RemotSend_UpErrCHUData_buf[29] = 1;/* 电池未报警的充电机设备个数 */		
			int index=0;int j=30;
			index = index_Err;
			// for(index = 0;index<1;index++)
			{		
				RemotSend_UpErrCHUData_buf[j++] = p->tm_year-100;		// 时间	6
				RemotSend_UpErrCHUData_buf[j++] = p->tm_mon+1;
				RemotSend_UpErrCHUData_buf[j++] = p->tm_mday;
				RemotSend_UpErrCHUData_buf[j++] = p->tm_hour;
				RemotSend_UpErrCHUData_buf[j++] = p->tm_min;
				RemotSend_UpErrCHUData_buf[j++] = p->tm_sec;	
				RemotSend_UpErrCHUData_buf[j++] = index+1;	// 充电机编号	1			

				RemotSend_UpErrCHUData_buf[j++] = CHU_DevState_code[index];	// 状态码	1
				RemotSend_UpErrCHUData_buf[j++] = MChg_ErrStop_code[index];	// 自停止故障	1
				RemotSend_UpErrCHUData_buf[j++] = 0x00;	// 心跳故障	1
				RemotSend_UpErrCHUData_buf[j++] = 0x02;	// 故障等级	1
				RemotSend_UpErrCHUData_buf[j++] = CHU_BattType[index];	// 电池包类型	1
				RemotSend_UpErrCHUData_buf[j++] = CHU_BMS_LinkState[index];	// BMS连接状态	1
				RemotSend_UpErrCHUData_buf[j++] = CHU_NegTemp_C[index];	// 正极柱温度	1
				RemotSend_UpErrCHUData_buf[j++] = CHU_PosTemp_C[index];	// 负极柱温度	1
				RemotSend_UpErrCHUData_buf[j++] = (uint8_t)((((int)DCEnVol_V[index]*100)>> 24) & 0xff);	// 直流电压	4
				RemotSend_UpErrCHUData_buf[j++] = (uint8_t)((((int)DCEnVol_V[index]*100)>> 16) & 0xff);
				RemotSend_UpErrCHUData_buf[j++] = (uint8_t)((((int)DCEnVol_V[index]*100)>>8 ) & 0xff);
				RemotSend_UpErrCHUData_buf[j++] = (uint8_t)((((int)DCEnVol_V[index]*100)>>0 ) & 0xff);		
				RemotSend_UpErrCHUData_buf[j++] = (uint8_t)((((int)DCEnCur_A[index]*100)>>24) & 0xff);		// 直流电流	4
				RemotSend_UpErrCHUData_buf[j++] = (uint8_t)((((int)DCEnCur_A[index]*100)>>16) & 0xff);
				RemotSend_UpErrCHUData_buf[j++] = (uint8_t)((((int)DCEnCur_A[index]*100)>> 8 ) & 0xff);
				RemotSend_UpErrCHUData_buf[j++] = (uint8_t)((((int)DCEnCur_A[index]*100)>> 0 ) & 0xff);              
				RemotSend_UpErrCHUData_buf[j++] = (uint8_t)((((int)DCEn_Act_W[index]*10000)>>24) & 0xff);		// 有功功率	4
				RemotSend_UpErrCHUData_buf[j++] = (uint8_t)((((int)DCEn_Act_W[index]*10000)>>16) & 0xff);
				RemotSend_UpErrCHUData_buf[j++] = (uint8_t)((((int)DCEn_Act_W[index]*10000)>> 8 ) & 0xff);
				RemotSend_UpErrCHUData_buf[j++] = (uint8_t)((((int)DCEn_Act_W[index]*10000)>> 0 ) & 0xff);				     
			    RemotSend_UpErrCHUData_buf[j++] = (uint8_t)((((int)DCEnPos_Act_W[index]*100)>>24) & 0xff);		// 正向有功电能	4
				RemotSend_UpErrCHUData_buf[j++] = (uint8_t)((((int)DCEnPos_Act_W[index]*100)>>16) & 0xff);
				RemotSend_UpErrCHUData_buf[j++] = (uint8_t)((((int)DCEnPos_Act_W[index]*100)>> 8 ) & 0xff);
				RemotSend_UpErrCHUData_buf[j++] = (uint8_t)((((int)DCEnPos_Act_W[index]*100)>> 0 ) & 0xff);	
				RemotSend_UpErrCHUData_buf[j++] = (uint8_t)((((int)DCEnNeg_Act_W[index]*100)>>24) & 0xff);		// 反向有功电能	4
				RemotSend_UpErrCHUData_buf[j++] = (uint8_t)((((int)DCEnNeg_Act_W[index]*100)>>16) & 0xff);
				RemotSend_UpErrCHUData_buf[j++] = (uint8_t)((((int)DCEnNeg_Act_W[index]*100)>> 8 ) & 0xff);
				RemotSend_UpErrCHUData_buf[j++] = (uint8_t)((((int)DCEnNeg_Act_W[index]*100)>> 0 ) & 0xff);		
				int k=0;
				for(k=0;k<32;k++)	
					RemotSend_UpErrCHUData_buf[j++] = BMS_BATT_NUM[index][k];// 动力电池编码	32		
				RemotSend_UpErrCHUData_buf[j++] = (uint8_t)((((int)BMS_CharPackVolReal_V[index]*10) >> 8 ) & 0xff);	// 电压输出值	2
				RemotSend_UpErrCHUData_buf[j++] = (uint8_t)((((int)BMS_CharPackVolReal_V[index]*10) >> 0 ) & 0xff);
				RemotSend_UpErrCHUData_buf[j++] = (uint8_t)((((int)BMS_CharPackCurReal_A[index]*10) >> 8 ) & 0xff);	// 电流输出值	2
				RemotSend_UpErrCHUData_buf[j++] = (uint8_t)((((int)BMS_CharPackCurReal_A[index]*10) >> 0 ) & 0xff);
				RemotSend_UpErrCHUData_buf[j++] = (uint8_t)((((int)BMS_ReqPackVol_V[index]*10) >> 8 ) & 0xff);	// BMS需求电压	2
				RemotSend_UpErrCHUData_buf[j++] = (uint8_t)((((int)BMS_ReqPackVol_V[index]*10) >> 0 ) & 0xff);
				RemotSend_UpErrCHUData_buf[j++] = (uint8_t)((((int)BMS_ReqPackCur_A[index]*10) >> 8 ) & 0xff);	// BMS需求电流	2
				RemotSend_UpErrCHUData_buf[j++] = (uint8_t)((((int)BMS_ReqPackCur_A[index]*10) >> 0 ) & 0xff);
				RemotSend_UpErrCHUData_buf[j++] = 2;	// 充电模式	1 CHU_ChgMode[index]
				RemotSend_UpErrCHUData_buf[j++] = (uint8_t)(((int)Charger_OutPwr_W[index] >> 8 ) & 0xff);	// 实际输出功率	2
				RemotSend_UpErrCHUData_buf[j++] = (uint8_t)(((int)Charger_OutPwr_W[index] >> 0 ) & 0xff);
				RemotSend_UpErrCHUData_buf[j++] = (uint8_t)(((int)BMS_RemainCharTimeReal_min[index] >> 8 ) & 0xff);	// 估算剩余充电时间	2
				RemotSend_UpErrCHUData_buf[j++] = (uint8_t)(((int)BMS_RemainCharTimeReal_min[index] >> 0 ) & 0xff);
				RemotSend_UpErrCHUData_buf[j++] = (uint8_t)(((int)ChU_AllChrTimeReal_min[index] >> 8 ) & 0xff);	// 单次累计充电时间	2
				RemotSend_UpErrCHUData_buf[j++] = (uint8_t)(((int)ChU_AllChrTimeReal_min[index] >> 0 ) & 0xff);
				RemotSend_UpErrCHUData_buf[j++] = BMS_BatteryType_Null[index];	// 电池类型	1
				RemotSend_UpErrCHUData_buf[j++] = (uint8_t)((((int)BMS_RatedCap_Ah[index]*10) >> 8 ) & 0xff);	// 整车动力蓄电池系统额定容量	2
				RemotSend_UpErrCHUData_buf[j++] = (uint8_t)((((int)BMS_RatedCap_Ah[index]*10) >> 0 ) & 0xff);
				RemotSend_UpErrCHUData_buf[j++] = (uint8_t)((((int)BMS_RatedPackVol_V[index]*10) >> 8 ) & 0xff);	// 整车动力蓄电池系统额定总电压	2
				RemotSend_UpErrCHUData_buf[j++] = (uint8_t)((((int)BMS_RatedPackVol_V[index]*10) >> 0 ) & 0xff);
				RemotSend_UpErrCHUData_buf[j++] = 255;//(uint8_t)(((int)Charger_OutPwr_W[index] >> 16) & 0xff);;	// 电池组充电次数	4
				RemotSend_UpErrCHUData_buf[j++] = 255;//(uint8_t)(((int)Charger_OutPwr_W[index] >> 16) & 0xff);
				RemotSend_UpErrCHUData_buf[j++] = 255;//(uint8_t)(((int)Charger_OutPwr_W[index] >> 8 ) & 0xff);
				RemotSend_UpErrCHUData_buf[j++] = 255;//(uint8_t)(((int)Charger_OutPwr_W[index] >> 0 ) & 0xff);
				RemotSend_UpErrCHUData_buf[j++] = (uint8_t)((((int)CellVol_AllowCharMax_V[index]*100) >> 8 ) & 0xff);	//??? 单体动力蓄电池最高允许充电电压	2
				RemotSend_UpErrCHUData_buf[j++] = (uint8_t)((((int)CellVol_AllowCharMax_V[index]*100) >> 0 ) & 0xff);
				RemotSend_UpErrCHUData_buf[j++] = (uint8_t)((((int)PackCur_AllowCharMax_A[index]*10) >> 8 ) & 0xff);	// 最高允许充电电流	2 ??
				RemotSend_UpErrCHUData_buf[j++] = (uint8_t)((((int)PackCur_AllowCharMax_A[index]*10) >> 0 ) & 0xff);
				RemotSend_UpErrCHUData_buf[j++] = (uint8_t)((((int)BMS_RatedEnergy_Kwh[index]*10) >> 8 ) & 0xff);	// 动力蓄电池标称总能量	2 ??
				RemotSend_UpErrCHUData_buf[j++] = (uint8_t)((((int)BMS_RatedEnergy_Kwh[index]*10) >> 0 ) & 0xff); 
				RemotSend_UpErrCHUData_buf[j++] = Temp_AllowCharMax_C[index];	// 最高允许温度	1 ??
				RemotSend_UpErrCHUData_buf[j++] = Batt_SOH[index];	// 动力电池健康度（SOH）	1 ??
				RemotSend_UpErrCHUData_buf[j++] = BMS_SerialNum_Null[index];	// 动力电池PACK串联数	1
				RemotSend_UpErrCHUData_buf[j++] = BMS_ParallNum_Null[index];	// 动力电池PACK并联数	1
				RemotSend_UpErrCHUData_buf[j++] = (uint8_t)(((int)BMS_ODO[index] >> 24) & 0xff);	// 动力电池行驶里程	4 ??
				RemotSend_UpErrCHUData_buf[j++] = (uint8_t)(((int)BMS_ODO[index] >> 16) & 0xff);
				RemotSend_UpErrCHUData_buf[j++] = (uint8_t)(((int)BMS_ODO[index] >> 8 ) & 0xff);
				RemotSend_UpErrCHUData_buf[j++] = (uint8_t)(((int)BMS_ODO[index] >> 0 ) & 0xff);
				RemotSend_UpErrCHUData_buf[j++] = BMS_SW_VERS[index];	// BMS次软件版本号后两位	1  ??
				RemotSend_UpErrCHUData_buf[j++] = BMS_SW_VERS_main[index];	// BMS主软件版本号后两位	1
				RemotSend_UpErrCHUData_buf[j++] = BMS_HW_VERS[index];	// BMS硬件版本号后两位	1
				RemotSend_UpErrCHUData_buf[j++] = (uint8_t)(((int)BMS_PART_VERS[index] >> 24) & 0xff);	// 电池包总成零部件号	4 ??
				RemotSend_UpErrCHUData_buf[j++] = (uint8_t)(((int)BMS_PART_VERS[index] >> 16) & 0xff);
				RemotSend_UpErrCHUData_buf[j++] = (uint8_t)(((int)BMS_PART_VERS[index] >> 8 ) & 0xff);
				RemotSend_UpErrCHUData_buf[j++] = (uint8_t)(((int)BMS_PART_VERS[index] >> 0 ) & 0xff);
				RemotSend_UpErrCHUData_buf[j++] = BMS_BATT_SOC[index];	// SOC	1
				RemotSend_UpErrCHUData_buf[j++] = Rly_Sta[index];	// 继电器状态	1
				RemotSend_UpErrCHUData_buf[j++] = (uint8_t)((((int)BMS_BATT_VOLT_V1[index]*100) >> 8 ) & 0xff);	// 动力电池总电压V1	2
				RemotSend_UpErrCHUData_buf[j++] = (uint8_t)((((int)BMS_BATT_VOLT_V1[index]*100) >> 0 ) & 0xff);
				RemotSend_UpErrCHUData_buf[j++] = (uint8_t)((((int)BMS_BATT_VOLT_V2[index]*100) >> 8 ) & 0xff);	// 动力电池总电压V2	2
				RemotSend_UpErrCHUData_buf[j++] = (uint8_t)((((int)BMS_BATT_VOLT_V2[index]*100) >> 0 ) & 0xff);
				RemotSend_UpErrCHUData_buf[j++] = (uint8_t)((((int)BMS_BATT_VOLT_V3[index]*100) >> 8 ) & 0xff);	// 动力电池总电压V3	2
				RemotSend_UpErrCHUData_buf[j++] = (uint8_t)((((int)BMS_BATT_VOLT_V3[index]*100) >> 0 ) & 0xff);
				RemotSend_UpErrCHUData_buf[j++] = (uint8_t)((((int)BMS_BATT_CUR[index]*100) >> 8 ) & 0xff);	// 动力电池总电流	2
				RemotSend_UpErrCHUData_buf[j++] = (uint8_t)((((int)BMS_BATT_CUR[index]*100) >> 0 ) & 0xff);
				RemotSend_UpErrCHUData_buf[j++] = (uint8_t)(((int)BMS_BATT_POSITIVE_RESISTANCE[index] >> 8 ) & 0xff);	// 动力电池正极对GND绝缘电阻	2
				RemotSend_UpErrCHUData_buf[j++] = (uint8_t)(((int)BMS_BATT_POSITIVE_RESISTANCE[index] >> 0 ) & 0xff);
				RemotSend_UpErrCHUData_buf[j++] = (uint8_t)(((int)BMS_BATT_NEGATIVE_RESISTANCE[index] >> 8 ) & 0xff);	// 动力电池负极对GND绝缘电阻	2
				RemotSend_UpErrCHUData_buf[j++] = (uint8_t)(((int)BMS_BATT_NEGATIVE_RESISTANCE[index] >> 0 ) & 0xff);
				RemotSend_UpErrCHUData_buf[j++] = (uint8_t)((((int)BMS_BATT_AVAIL_ENERGY[index]*100) >> 8 ) & 0xff);	// 动力电池可用能量	2
				RemotSend_UpErrCHUData_buf[j++] = (uint8_t)((((int)BMS_BATT_AVAIL_ENERGY[index]*100) >> 0 ) & 0xff);
				RemotSend_UpErrCHUData_buf[j++] = (uint8_t)((((int)BMS_BATT_AVAIL_CAPACITY[index]*100) >> 8 ) & 0xff);	// 动力电池可用容量	2
				RemotSend_UpErrCHUData_buf[j++] = (uint8_t)((((int)BMS_BATT_AVAIL_CAPACITY[index]*100) >> 0 ) & 0xff);	
				RemotSend_UpErrCHUData_buf[j++] = (uint8_t)(((int)BMS_CHARGE_TOT_AH[index] >> 24) & 0xff);	// // 累计充电总安时	4??
				RemotSend_UpErrCHUData_buf[j++] = (uint8_t)(((int)BMS_CHARGE_TOT_AH[index] >> 16) & 0xff);
				RemotSend_UpErrCHUData_buf[j++] = (uint8_t)(((int)BMS_CHARGE_TOT_AH[index] >> 8 ) & 0xff);
				RemotSend_UpErrCHUData_buf[j++] = (uint8_t)(((int)BMS_CHARGE_TOT_AH[index] >> 0 ) & 0xff);
				RemotSend_UpErrCHUData_buf[j++] = 0x02;	// 快换电池工作模式	1
				RemotSend_UpErrCHUData_buf[j++] = (uint8_t)((((int)BMS_FEED_MAX_PWR[index]*100) >> 8 ) & 0xff);	// 当前状态允许最大回馈功率	2
				RemotSend_UpErrCHUData_buf[j++] = (uint8_t)((((int)BMS_FEED_MAX_PWR[index]*100) >> 0 ) & 0xff);
				RemotSend_UpErrCHUData_buf[j++] = (uint8_t)((((int)BMS_DISCHG_MAX_PWR[index]*100) >> 8 ) & 0xff);	// 当前状态允许最大放电功率	2
				RemotSend_UpErrCHUData_buf[j++] = (uint8_t)((((int)BMS_DISCHG_MAX_PWR[index]*100) >> 0 ) & 0xff);
				RemotSend_UpErrCHUData_buf[j++] = 0x00;	// 动力电池状态	1
				RemotSend_UpErrCHUData_buf[j++] = (uint8_t)((((int)(BMS_CELL_VOLT_MAX[index]*1000)) >> 8 ) & 0xff);	// 单体电芯最高电压	2
				RemotSend_UpErrCHUData_buf[j++] = (uint8_t)((((int)(BMS_CELL_VOLT_MAX[index]*1000)) >> 0 ) & 0xff);
				RemotSend_UpErrCHUData_buf[j++] = BMS_MAX_VOLT_NUM[index];	// 最高单体电压编号	1
				RemotSend_UpErrCHUData_buf[j++] = (uint8_t)((((int)(BMS_CELL_VOLT_MIN[index]*1000)) >> 8 ) & 0xff);	// 单体电芯最低电压	2
				RemotSend_UpErrCHUData_buf[j++] = (uint8_t)((((int)(BMS_CELL_VOLT_MIN[index]*1000)) >> 0 ) & 0xff);
				RemotSend_UpErrCHUData_buf[j++] = BMS_MIN_VOLT_NUM[index];	// 最低单体电压编号	1
				RemotSend_UpErrCHUData_buf[j++] = BMS_CELL_TEMP_MAX[index]+48;	// 最高动力蓄电池温度	1
				RemotSend_UpErrCHUData_buf[j++] = BMS_MAX_TEMP_NUM[index];	// 最高动力蓄电池温度检测点编号	1
				RemotSend_UpErrCHUData_buf[j++] = BMS_CELL_TEMP_MIN[index]+48;	// 最低动力蓄电池温度	1
				RemotSend_UpErrCHUData_buf[j++] = BMS_MIN_TEMP_NUM[index];	// 最低动力蓄电池温度检测点编号	1
				RemotSend_UpErrCHUData_buf[j++] = (uint8_t)(((int)BMS_Err_msg[index][0] >> 0 ) & 0xff);	// 动力电池故障	6
				RemotSend_UpErrCHUData_buf[j++] = (uint8_t)(((int)BMS_Err_msg[index][1] >> 0 ) & 0xff);
				RemotSend_UpErrCHUData_buf[j++] = (uint8_t)(((int)BMS_Err_msg[index][2] >> 0 ) & 0xff);
				RemotSend_UpErrCHUData_buf[j++] = (uint8_t)(((int)BMS_Err_msg[index][3] >> 0 ) & 0xff);
				RemotSend_UpErrCHUData_buf[j++] = (uint8_t)(((int)BMS_Err_msg[index][4] >> 0  ) & 0xff);
				RemotSend_UpErrCHUData_buf[j++] = (uint8_t)(((int)BMS_Err_msg[index][5] >> 0  ) & 0xff);
				RemotSend_UpErrCHUData_buf[j++] = 112;	// 单体电压监测点位个数	1	
				
				for(k=0;k<112;k++)	// 单体电压总成信息列表	2×v
				{
					RemotSend_UpErrCHUData_buf[j++] = (uint8_t)(((int)BMS_CellVol_buf[index][k] >> 8 ) & 0xff);
					RemotSend_UpErrCHUData_buf[j++] = (uint8_t)(((int)BMS_CellVol_buf[index][k] >> 0 ) & 0xff);
				}
				RemotSend_UpErrCHUData_buf[j++] = 32;//BATT_TEMP_SAMPLE_NUM[index];	//?? 动力电池温度点总数	1
				for(k=0;k<32;k++)	// 动力电池温度总成信息列表	2×t
				{
					RemotSend_UpErrCHUData_buf[j++] = BMS_CellTemp_buf[index][k];
				}
			}
			if(EMS_CHU_LinkState[index_Chu]!=1||CHU_DevState_code[index_Chu]<3)
			{
				for(i=37;i<444;i++)
					RemotSend_UpErrCHUData_buf[i] = 0xff;
				RemotSend_UpErrCHUData_buf[186] = 112;
				RemotSend_UpErrCHUData_buf[298] = 32;
			}
			RemotSend_UpErrCHUData_buf[20] = (((j-1)-21) >> 8) & 0xff;
			RemotSend_UpErrCHUData_buf[21] = (((j-1)-21) >> 0) & 0xff;  
			check16 = mc_check_crc16(RemotSend_UpErrCHUData_buf,j);
			RemotSend_UpErrCHUData_buf[j++] = (uint8_t)(check16/256);
			RemotSend_UpErrCHUData_buf[j++] = (uint8_t)(check16%256);	
			RemotSend_UpErrCHUData_buf_len = j;	
		}
		break;
		case 9:	//实时信息上报:9  汽车换电过程的状态数据
		{
			// bzero(RemotSend_UpCEingData_buf,100);
			for(i=0;i<15;i++)
				{RemotSend_UpCEingData_buf[i+4] = Station_No[i];}	//换电站的编码
			RemotSend_UpCEingData_buf[19] = 0x01;
			
			RemotSend_UpCEingData_buf[20] = 0x00;
			RemotSend_UpCEingData_buf[21] = 0x10;  		//数据单元长度
			RemotSend_UpCEingData_buf[22] = p->tm_year-100;
			RemotSend_UpCEingData_buf[23] = p->tm_mon+1;
			RemotSend_UpCEingData_buf[24] = p->tm_mday;
			RemotSend_UpCEingData_buf[25] = p->tm_hour;
			RemotSend_UpCEingData_buf[26] = p->tm_min;
			RemotSend_UpCEingData_buf[27] = p->tm_sec;
			RemotSend_UpCEingData_buf[28] = 0x09;	//9 汽车换电过程的状态数据
			int j=29;
			RemotSend_UpCEingData_buf[j++] = Sys_Big_sta;		// 换电状态	1
			int i=0;
			for(i=0;i<16;i++)		// 车牌号	16
				{RemotSend_UpCEingData_buf[j++] = Cloud_Grab_carNo[i];}

			RemotSend_UpCEingData_buf[j++] = p->tm_year-100;		// 换电时间	6
			RemotSend_UpCEingData_buf[j++] = p->tm_mon+1;
			RemotSend_UpCEingData_buf[j++] = p->tm_mday;
			RemotSend_UpCEingData_buf[j++] = p->tm_hour;
			RemotSend_UpCEingData_buf[j++] = p->tm_min;
			RemotSend_UpCEingData_buf[j++] = p->tm_sec;
			int index = 0;
			for(index =0;index<32;index++)		// 换上电池包编号	32
				{RemotSend_UpCEingData_buf[j++] = SourseStorage_Batt_NUM[index];}
			for(index =0;index<32;index++)		// 换下的电池包编号	32
				{RemotSend_UpCEingData_buf[j++] = Cloud_BatteryNo_now[index];}

			RemotSend_UpCEingData_buf[j++] = MChar_SelfCe_sour+1;		// 换上的电池包舱位编号	1
			RemotSend_UpCEingData_buf[j++] = MChar_SelfCe_sour+1;		// 换下的电池包舱位编号	1

			RemotSend_UpCEingData_buf[20] = (((j-1)-21) >> 8) & 0xff;
			RemotSend_UpCEingData_buf[21] = (((j-1)-21) >> 0) & 0xff;  		//数据单元长度

			check16 = mc_check_crc16(RemotSend_UpCEingData_buf,j);
			RemotSend_UpCEingData_buf[j++] = (uint8_t)(check16/256);
			RemotSend_UpCEingData_buf[j++] = (uint8_t)(check16%256);			
			RemotSend_UpCEingData_buf_len = j;//buf len
		}
		break;
		case 10://	换电站登出
		{
			bzero(RemotSend_LogOut_buf,100);
			for(i=0;i<15;i++)
				RemotSend_LogOut_buf[i+4] = Station_No[i];	//换电站的编码
			RemotSend_LogOut_buf[19] = 0x01;
			
			RemotSend_LogOut_buf[20] = 0x00;
			RemotSend_LogOut_buf[21] = 0x00;  		//数据单元长度
			RemotSend_LogOut_buf[22] = p->tm_year-100;
			RemotSend_LogOut_buf[23] = p->tm_mon+1;
			RemotSend_LogOut_buf[24] = p->tm_mday;
			RemotSend_LogOut_buf[25] = p->tm_hour;
			RemotSend_LogOut_buf[26] = p->tm_min;
			RemotSend_LogOut_buf[27] = p->tm_sec;
			RemotSend_LogOut_buf[28] = 0;
			RemotSend_LogOut_buf[29] = 1;			//登出流水号

			RemotSend_LogOut_buf[20] = (((30-1)-21) >> 8) & 0xff;
			RemotSend_LogOut_buf[21] = (((30-1)-21) >> 0) & 0xff;	//数据单元长度

			check16 = mc_check_crc16(RemotSend_LogOut_buf,30);
			RemotSend_LogOut_buf[30] = (uint8_t)(check16/256);
			RemotSend_LogOut_buf[31] = (uint8_t)(check16%256);
			RemotSend_LogOut_buf_len = 32;		//buf 长度
		}break;
		case 11://	通信心跳
		{
			for(i=0;i<15;i++)
				RemotSend_Heart_buf[i+4] = Station_No[i];	//换电站的编码
			RemotSend_Heart_buf[19] = 0x01;

			RemotSend_Heart_buf[20] = 0;
			RemotSend_Heart_buf[21] = 6;	//数据单元长度
			RemotSend_Heart_buf[22] = p->tm_year-100;
			RemotSend_Heart_buf[23] = p->tm_mon+1;
			RemotSend_Heart_buf[24] = p->tm_mday;
			RemotSend_Heart_buf[25] = p->tm_hour;
			RemotSend_Heart_buf[26] = p->tm_min;
			RemotSend_Heart_buf[27] = p->tm_sec;

			check16 = mc_check_crc16(RemotSend_Heart_buf,28);
			RemotSend_Heart_buf[28] = (uint8_t)(check16/256);
			RemotSend_Heart_buf[29] = (uint8_t)(check16%256);
		}break;
		case 12://	终端控制的应答数据
		{
			for(i=0;i<15;i++)
				Remo_SendBack_buf[i+4] = Station_No[i];	//换电站的编码
			Remo_SendBack_buf[19] = 0x01;

			Remo_SendBack_buf[20] = 0;
			Remo_SendBack_buf[21] = 8;	//数据单元长度
			Remo_SendBack_buf[22] = p->tm_year-100;
			Remo_SendBack_buf[23] = p->tm_mon+1;
			Remo_SendBack_buf[24] = p->tm_mday;
			Remo_SendBack_buf[25] = p->tm_hour;
			Remo_SendBack_buf[26] = p->tm_min;
			Remo_SendBack_buf[27] = p->tm_sec;
			// Remo_SendBack_buf[28] = 0;

			check16 = mc_check_crc16(Remo_SendBack_buf,30);
			Remo_SendBack_buf[30] = (uint8_t)(check16/256);
			Remo_SendBack_buf[31] = (uint8_t)(check16%256);
			// Remo_SendBack_buf[29] = 0;
		}break;
		default:
		break;
	}
}
struct hostent *gethost;
void thread_Remote(void)
{
	//定义sockfd
    int clientsockfd = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in servaddr;
	char ip[50];
	int i = 0;
	int16_t conn1 = -1;
	int16_t Send_flg;
	int16_t Remot_ReadSize;

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);  ///服务器端口
	// while((gethost = gethostbyname("download.bjsetc.cn"))==NULL)
	// {
	// 	printf("connect Remote function failed22222222222.\n");
	// 	sleep(10);
	// }
	// ptr_ip = inet_ntop(gethost->h_addrtype,gethost->h_addr_list[0],ip,sizeof(ip));
	// printf("ipipip = %s\n",ptr_ip);
    // servaddr.sin_addr.s_addr = inet_addr(ptr_ip);  ///服务器ip 
	servaddr.sin_addr.s_addr = inet_addr(test_ip);
	//close(clientsockfd);
	 ///连接服务器成功返回0错误返回-1
	//  RemotSend_Inter_Deal(1);
//	printf("connect Remote function failed22222222222.\n");
	conn1 = connect(clientsockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
	while(conn1<0)
	{
		close(clientsockfd);
		sleep(2);
		clientsockfd = socket(AF_INET, SOCK_STREAM, 0);
		int flags = fcntl(clientsockfd, F_GETFL, 0);  
		fcntl(clientsockfd, F_SETFL, flags|O_NONBLOCK);
		// fcntl(clientsockfd, F_SETFL, flags&~O_NONBLOCK);
		conn1 = connect(clientsockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
		sleep(4);
		// if(recv(clientsockfd, Qt_Recv_buf, BUFFER_SIZE,MSG_DONTWAIT|MSG_NOSIGNAL)>=0)
		// {
		// 	conn1 = 1;
		// }
		conn1 = send(clientsockfd, Send_ReConnect_buf, (1),MSG_DONTWAIT|MSG_NOSIGNAL);
	//	printf("connect Remote function failed22222222222.\n");
		Remot_conn_flg = 2;
		//sleep(10);
	}
	Remot_conn_flg = 1;
//	printf("connect Remote OK11111111111111.\n");
	RemotSend_LogOn_flg = 1;


	while(1)
	{
		// RemotSend_LogOn_flg = 1;
		// RemotSend_Heart_flg = 1;
		// RemotSend_UpCEingData_flg=1;

		if(RemotSend_LogOn_flg==1){
	//		printf("connect Remote OKloasononon.\n");
			RemotSend_LogOn_flg = 0;
			RemotSend_Inter_Deal(0);
			send(clientsockfd, RemotSend_LogOn_buf, RemotSend_LogOn_buf_len,MSG_DONTWAIT|MSG_NOSIGNAL);
		}
		if(QtEMS_ShutDown_flg==1){ //RemotSend_LogOut_flg
			QtEMS_ShutDown_flg = 0;
			RemotSend_Inter_Deal(10);
			send(clientsockfd, RemotSend_LogOut_buf, RemotSend_LogOut_buf_len,MSG_DONTWAIT|MSG_NOSIGNAL);
		}
		if(RemotSend_UpPLCData_flg == 1)
		{
			// printf("11111111111111===111111\n");
			RemotSend_UpPLCData_flg = 0;
			RemotSend_Inter_Deal(1);
			send(clientsockfd, RemotSend_UpPLCData_buf, RemotSend_UpPLCData_buf_len,MSG_DONTWAIT|MSG_NOSIGNAL);
		}
		if(RemotSend_UpACEngData_flg == 1)
		{
			// printf("222222222==22222222\n");
			RemotSend_UpACEngData_flg = 0;
			RemotSend_Inter_Deal(2);
			send(clientsockfd, RemotSend_UpACEngData_buf, RemotSend_UpACEngData_buf_len,MSG_DONTWAIT|MSG_NOSIGNAL);
		}
		if(RemotSend_UpAirConData_flg == 1)
		{
			// printf("33333333333====333333\n");
			RemotSend_UpAirConData_flg = 0;
			RemotSend_Inter_Deal(3);
			send(clientsockfd, RemotSend_UpAirConData_buf,RemotSend_UpAirConData_buf_len,MSG_DONTWAIT|MSG_NOSIGNAL);
		}
		if(RemotSend_UpFanData_flg == 1)
		{
			// printf("444444===444444\n");
			RemotSend_UpFanData_flg = 0;
			RemotSend_Inter_Deal(4);
			send(clientsockfd, RemotSend_UpFanData_buf,RemotSend_UpFanData_buf_len,MSG_DONTWAIT|MSG_NOSIGNAL);
		}
		if(RemotSend_UpFireData_flg == 1)
		{
			// printf("55555===55555555\n");
			RemotSend_UpFireData_flg = 0;
			RemotSend_Inter_Deal(5);
			send(clientsockfd, RemotSend_UpFireData_buf, RemotSend_UpFireData_buf_len,MSG_DONTWAIT|MSG_NOSIGNAL);
		}
		if(RemotSend_UpCEData_flg == 1)
		{
			RemotSend_UpCEData_flg = 0;
			RemotSend_Inter_Deal(6);
			send(clientsockfd, RemotSend_UpCEData_buf, RemotSend_UpCEData_buf_len,MSG_DONTWAIT|MSG_NOSIGNAL);
		}
		if(RemotSend_UpCHUData_flg == 1)
		{
			// printf("chuchuchu====chuchuhuc\n");
			RemotSend_UpCHUData_flg = 0;
			RemotSend_Inter_Deal(7);
			send(clientsockfd, RemotSend_UpCHUData_buf, RemotSend_UpCHUData_buf_len,MSG_DONTWAIT|MSG_NOSIGNAL);
		}
		if(RemotSend_UpErrCHUData_flg == 1)
		{
			// printf("errerrerr==erererchuer\n");
			RemotSend_UpErrCHUData_flg = 0;
			RemotSend_Inter_Deal(8);
			send(clientsockfd, RemotSend_UpErrCHUData_buf, RemotSend_UpErrCHUData_buf_len,MSG_DONTWAIT|MSG_NOSIGNAL);
		}
		if(RemotSend_UpCEingData_flg == 1)
		{
			// printf("UpCEing===UpCEing\n");
			RemotSend_UpCEingData_flg = 0;
			RemotSend_Inter_Deal(9);
			send(clientsockfd, RemotSend_UpCEingData_buf, RemotSend_UpCEingData_buf_len,MSG_DONTWAIT|MSG_NOSIGNAL);
		}
		if(RemotSend_Heart_flg==1){
			// printf("6666666===666666\n");
			RemotSend_Heart_flg = 0;
			RemotSend_Inter_Deal(11);
			send(clientsockfd, RemotSend_Heart_buf, 30,MSG_DONTWAIT|MSG_NOSIGNAL);
		}
		if(Remo_SendFanBack_flg==1){
			Remo_SendFanBack_flg = 0;
			RemotSend_Inter_Deal(12);
			send(clientsockfd, Remo_SendBack_buf, 32,MSG_DONTWAIT|MSG_NOSIGNAL);
		}
		if(Remo_SendDoorBack_flg==1){
			Remo_SendDoorBack_flg = 0;
			RemotSend_Inter_Deal(12);
			send(clientsockfd, Remo_SendBack_buf, 32,MSG_DONTWAIT|MSG_NOSIGNAL);
		}
		if(Remo_SendAirBack_flg==1){
			Remo_SendAirBack_flg = 0;
			RemotSend_Inter_Deal(12);
			send(clientsockfd, Remo_SendBack_buf, 32,MSG_DONTWAIT|MSG_NOSIGNAL);
		}
		if(Remo_SendChuBack_flg==1){	
			Remo_SendChuBack_flg = 0;
			RemotSend_Inter_Deal(12);
			send(clientsockfd, Remo_SendBack_buf, 32,MSG_DONTWAIT|MSG_NOSIGNAL);
		}
		if(Remo_SendPLCBack_flg==1){
			Remo_SendPLCBack_flg = 0;
			RemotSend_Inter_Deal(12);
			send(clientsockfd, Remo_SendBack_buf, 32,MSG_DONTWAIT|MSG_NOSIGNAL);
		}
		if(Remo_SendEMSBack_flg==1){
			Remo_SendEMSBack_flg = 0;
			RemotSend_Inter_Deal(12);
			send(clientsockfd, Remo_SendBack_buf, 32,MSG_DONTWAIT|MSG_NOSIGNAL);
		}
		if(MRemo_ChangeBat_ok==1){
			MRemo_ChangeBat_ok = 0;
			Remo_SendBack_buf[3] = 5;
			Remo_SendBack_buf[28] = 7;
			Remo_SendBack_buf[29] = 3;
			RemotSend_Inter_Deal(12);
			send(clientsockfd, Remo_SendBack_buf, 32,MSG_DONTWAIT|MSG_NOSIGNAL);
			// printf("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\n");
		}
		if(MRemo_CELocator_ok==1){
			MRemo_CELocator_ok = 0;
			Remo_SendBack_buf[3] = 5;
			Remo_SendBack_buf[28] = 7;
			Remo_SendBack_buf[29] = 6;
			RemotSend_Inter_Deal(12);
			send(clientsockfd, Remo_SendBack_buf, 32,MSG_DONTWAIT|MSG_NOSIGNAL);
			// printf("bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb\n");
		}
		if(RemotSend_Discon_con>=30)
		{			
			Remot_conn_flg = 2;
			Send_flg = -1;
			// conn1 = connect(clientsockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
			while(Send_flg<0)
			{
				close(clientsockfd);
				sleep(1);
				clientsockfd = socket(AF_INET, SOCK_STREAM, 0);
				int flags = fcntl(clientsockfd, F_GETFL, 0);  
				fcntl(clientsockfd, F_SETFL, flags|O_NONBLOCK);
				conn1 = connect(clientsockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
				sleep(2);
				Send_flg = send(clientsockfd, Send_ReConnect_buf, (1),MSG_DONTWAIT|MSG_NOSIGNAL);

				fprintf(stderr, "connect remode function failed.\n");
				Remot_conn_flg = 2;
				//sleep(10);
				// printf("Send_flg22222%d = ",Send_flg);
			}		
		}
		else
		{
			Remot_conn_flg = 1;
		}		
		Remot_ReadSize=(recv(clientsockfd, Remot_Recv_buf, BUFFER_SIZE,MSG_DONTWAIT|MSG_NOSIGNAL));
		if(Remot_ReadSize<=0){
		// if((recv(clientsockfd, Qt_Recv_buf, BUFFER_SIZE,MSG_DONTWAIT|MSG_NOSIGNAL))<0){
				}	
		else{
		    // printf("Read_Size11111 %d\n",Remot_ReadSize);
			for(i=0;i<Remot_ReadSize;i++)
			{
	//			printf("%2x ",Remot_Recv_buf[i]);
			}
			printf("\n");
			Remot_Rec_Deal(Remot_Recv_buf,	Remot_ReadSize);
			RemotSend_Discon_con = 0;
		}
		usleep(100*1000);//100ms
	}
	close(clientsockfd);
}


