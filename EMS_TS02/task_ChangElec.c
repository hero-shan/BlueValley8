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
#include "SA4_EMS.h"

#define ORDER_PLCHERAT           		 1   //换电设备心跳
#define ORDER_INIT                		 2   //初始化指令
#define ORDER_INIT_BACK           		 3   //初始化指令反馈
#define ORDER_CHANGEBATBAG    	  		 4   //更换电池包指令
#define ORDER_CHANGEBATBAG_BACK   		 5   //更换电池包指令反馈
#define ORDER_CHANGELOCATOR    	  		 6   //电池包更换库位指令
#define ORDER_CHANGELOCATOR_BACK  		 7   //电池包更换库位指令反馈
#define ORDER_BATBAGOUT       	 		 8   //电池包移出滑出机构
#define ORDER_BATBAGOUT_BACK      		 9   //电池包移出滑出机构指令反馈
#define ORDER_BATBAGINLOCATOR     		 10   //电池包入库指令
#define ORDER_BATBAGINLOCATOR_BACK 		 11   //电池包入库指令反馈
#define ORDER_BATBAGOUTLOCATOR     		 12   //电池包出库指令
#define ORDER_BATBAGOUTLOCATOR_BACK 	 13   //电池包出库指令反馈
#define ORDER_DISMOUNTBATBAG   		     14   //拆卸电池包指令
#define ORDER_DISMOUNTBATBAG_BACK 	     15   //拆卸电池包指令反馈
#define ORDER_MOUNTBATBAG   		     16   //安装电池包指令
#define ORDER_MOUNTBATBAG_BACK 	    	 17   //安装电池包指令反馈
#define ORDER_RGVOUTCHANGPLAT   		 18   //RGV伸出至换电平台指令
#define ORDER_RGVOUTCHANGPLAT_BACK 	     19   //RGV伸出至换电平台指令反馈
#define ORDER_RGVINCHANGPLAT   		 	 20   //RGV缩回电池仓内指令
#define ORDER_RGVINCHANGPLAT_BACK 	     21   //RGV缩回电池仓内指令反馈
#define ORDER_TIMEOUT   		     	 22   //暂停指令
#define ORDER_TIMEOUT_BACK 	     		 23   //暂停指令反馈
#define ORDER_CONTINUE   		    	 24   //继续指令
#define ORDER_CONTINUE_BACK 	    	 25   //继续指令反馈
#define ORDER_STOP  		    		 26   //停止指令
#define ORDER_STOP_BACK 	     		 27   //停止指令反馈
#define ORDER_ERRRESET   		   	     28   //故障复位指令
#define ORDER_ERRRESET_BACK 	         29   //故障复位指令反馈
#define ORDER_RUNSTATE   		    	 30   //运行状态
#define ORDER_CONTROLMODE   		     31   //控制模式
#define ORDER_ERRDATA   		    	 32   //故障信息
#define ORDER_CARTYPENOW   		    	 33   //当前车型
#define ORDER_RGVSTEPDATA   		     34   //RGV工步信息
#define ORDER_STACKERSTEPDATA   		 35   //码垛机工步信息
#define ORDER_PARKINGSTEPDATA   		 36   //泊车平台工步信息
#define ORDER_ROTATRODATA   		     37   //旋杆状态
#define ORDER_CEOK   		   			 38   //换电完成
#define ORDER_CEOK_BACK 	     		 39   //换电完成接收反馈
#define ORDER_DISMOUNTOK   		     	 40   //拆卸完成信号
#define ORDER_DISMOUNTOK_BACK 	    	 41   //拆卸完成信号接收反馈
#define ORDER_MOUNTOK   		    	 42   //安装完成信号
#define ORDER_MOUNTOK_BACK 	     		 43   //安装完成信号接收反馈
#define ORDER_INLOCATOROK   		     44   //入库完成信号
#define ORDER_INLOCATOROK_BACK 	     	 45   //入库完成信号接收反馈
#define ORDER_OUTLOCATOROK   		     46   //出库完成信号
#define ORDER_OUTLOCATOROK_BACK 	     47   //出库完成信号接收反馈
#define ORDER_CHANGELOCATOROK   		 48   //更换库位完成信号
#define ORDER_CHANGELOCATOROK_BACK 	     49   //更换库位完成信号接收反馈
#define ORDER_BATTOLOCATOROK   			 50   //电池放至库位完成信号 
#define ORDER_BATTOLOCATOROK_BACK 	     51   //电池放至库位完成信号反馈
#define ORDER_LOCATORSENSER 	     	 52   //库位传感器
#define ORDER_RGVSENSERSTA	    		 53   //RGV传感器状态
#define ORDER_STACKERSENSERSTA	    	 54   //码垛机传感器状态
#define ORDER_TURNOVERSENSERSTA	    	 55   //周转仓传感器状态
#define ORDER_SLIOUTSENSERSTA	    	 56   //滑出位传感器状态
#define ORDER_UPDOWNDOORSTA	    		 57   //升降门状态
#define ORDER_CHARSOCKPOWERSTA	    	 58   //充电插座供电状态
#define ORDER_ELECLINEARSTA	    		 59   //电动推杆状态


#define PORT                  7000
#define BUFFER_SIZE           500
#define SEND_DCHARORDER_COUNT 15
#define SEND_GETPMUDATA_COUNT 15 
#define SEND_PCSDATA_COUNT    20

mid_PLC_t mid_PLC;

uint8_t CESend_INIT_buf[3+0]={0xFE,ORDER_INIT,0x0};
uint8_t CESend_CHANGEBATBAG_buf[3+2]={0xFE,ORDER_CHANGEBATBAG,0x2};
uint8_t CESend_BATBAGINLOCATOR_buf[3+2]={0xFE,ORDER_BATBAGINLOCATOR,0x2};
uint8_t CESend_BATBAGOUTLOCATOR_buf[3+2]={0xFE,ORDER_BATBAGOUTLOCATOR,0x2};
uint8_t CESend_CHANGELOCATOR_buf[3+3]={0xFE,ORDER_CHANGELOCATOR,0x2};
uint8_t CESend_BATBAGOUT_buf[3+2]={0xFE,ORDER_BATBAGOUT,0x2};
uint8_t CESend_DISMOUNTBATBAG_buf[3+1]={0xFE,ORDER_DISMOUNTBATBAG,0x1};
uint8_t CESend_MOUNTBATBAG_buf[3+1]={0xFE,ORDER_MOUNTBATBAG,0x1};
uint8_t CESend_RGVOUTCHANGPLAT_buf[3+2]={0xFE,ORDER_RGVOUTCHANGPLAT,0x2};
uint8_t CESend_TIMEOUT_buf[3+2]={0xFE,ORDER_TIMEOUT,0x0};
uint8_t CESend_CONTINUE_buf[3+2]={0xFE,ORDER_CONTINUE,0x0};
uint8_t CESend_STOP_buf[3+2]={0xFE,ORDER_STOP,0x0};
uint8_t CESend_ERRRESET_buf[3+2]={0xFE,ORDER_ERRRESET,0x0};

uint8_t CESend_CEOK_BACK_buf[3+0]={0xFE,ORDER_CEOK_BACK,0x0};
uint8_t CESend_DISMOUNTOK_BACK_buf[3+0]={0xFE,ORDER_DISMOUNTOK_BACK,0x0};
uint8_t CESend_MOUNTOK_BACK_buf[3+0]={0xFE,ORDER_MOUNTOK_BACK,0x0};
uint8_t CESend_INLOCATOROK_BACK_buf[3+0]={0xFE,ORDER_INLOCATOROK_BACK,0x0};
uint8_t CESend_OUTLOCATOROK_BACK_buf[3+0]={0xFE,ORDER_OUTLOCATOROK_BACK,0x0};
uint8_t CESend_CELOCATOROK_BACK_buf[3+0]={0xFE,ORDER_CHANGELOCATOROK_BACK,0x0};
uint8_t CESend_BATTOLOCATOROK_BACK_buf[3+0]={0xFE,ORDER_BATTOLOCATOROK_BACK,0x0};
uint8_t CESend_ELECLINEARSTA_buf[3+20]={0xFE,ORDER_ELECLINEARSTA,0x14};

uint8_t CE_BCSData_buf[50];
uint8_t CE_Recv_buf[BUFFER_SIZE];
uint16_t CE_RecvData_dlc;
uint16_t Task_100ms_cou;

uint8_t CE_ErrData[2];
uint8_t CE_LocatorSenser[21]; //库位传感20+1

uint8_t Send_CE_CEOK_BACK_flg;
uint8_t Send_CE_DISMOUNTOK_BACK_flg;
uint8_t Send_CE_MOUNTOK_BACK_flg;
uint8_t Send_CE_INLOCATOROK_BACK_flg;
uint8_t Send_CE_OUTLOCATOROK_BACK_flg;
uint8_t Send_CE_CELOCATOROK_BACK_flg;
uint8_t Send_CE_BATTOLOCATOROK_BACK_flg;
uint8_t Send_CE_ELECLINEARSTA_flg;

// uint8_t MCE_Comm_flg = 0;
uint16_t CESend_Discon_con;

void Test_Deal(uint8_t * recv_buf,int recv_buf_len)
{
	uint8_t i;

	switch(recv_buf[1])
	{
		case 3:	{
			mid_PLC.CE_InitBack_flg = recv_buf[3];
			mid_PLC.CE_InitBack_err = recv_buf[4];//初始化指令反馈	
//			printf("get 0x03 = %d\n",mid_PLC.CE_InitBack_flg);
		}
			// if(mid_PLC.CE_InitBack_flg==1){
			// 	MCE_GetInit_flg = 1;
			// }
			// else MCE_GetInit_flg = 2;
		break;
		case 5:{
			mid_PLC.CE_ChangBatBack_flg = recv_buf[3];	
			mid_PLC.CE_ChangBatBack_err = recv_buf[4];	
		}
			// if(mid_PLC.CE_ChangBatBack_flg==1){
			// 	MCE_GetChangeBat_flg = 1;
			// 	printf("get 0x05 = %d\n",mid_PLC.CE_ChangBatBack_flg);
			// }
			// else MCE_GetChangeBat_flg = 2;
		break;
		case 7:{
			mid_PLC.CE_ChangLocatorBack_flg = recv_buf[3];	
			mid_PLC.CE_ChangLocatorBack_err = recv_buf[4];
		}	
			// if(CE_ChangLocatorBack_flg==1){
			// 	MCE_GetCELocator_flg = 1;
			// }
			// else MCE_GetCELocator_flg = 2;									
		break;
		case 9:{
			mid_PLC.CE_BatOutBack_flg = recv_buf[3];	
			mid_PLC.CE_BatOutBack_err = recv_buf[4];	
//			printf("get 0x09 = %d\n",mid_PLC.CE_BatOutBack_flg);
		}
			// if(CE_BatOutBack_flg==1){
			// 	MCE_GetBatOut_flg = 1;
			// }
			// else MCE_GetBatOut_flg = 2;									
		break;
		case 11:{
			mid_PLC.CE_BatInLocatorBack_flg = recv_buf[3];	
			mid_PLC.CE_BatInLocatorBack_err = recv_buf[4];
		}
			// if(CE_BatInLocatorBack_flg==1){
			// 	MCE_GetInLocator_flg = 1;
			// }
			// else MCE_GetInLocator_flg = 2;											
		break;
		case 13:{
			mid_PLC.CE_BatOutLocatorBack_flg = recv_buf[3];	
			mid_PLC.CE_BatOutLocatorBack_err = recv_buf[4];	
//			printf("get CE_BatOutLocatorBack_flg0x13 = %d\n",mid_PLC.CE_BatOutLocatorBack_err);
		}
			// if(CE_BatOutLocatorBack_flg==1){
			// 	MCE_GetOutLocator_flg = 1;
			// }
			// else MCE_GetOutLocator_flg = 2;										
		break;
		case 15:{
			mid_PLC.CE_DismountBatBack_flg = recv_buf[3];	
			mid_PLC.CE_DismountBatBack_err = recv_buf[4];	
		}
			// if(CE_DismountBatBack_flg==1){
			// 	MCE_GetDismountBat_flg = 1;
			// }
			// else MCE_GetDismountBat_flg = 2;										
		break;
		case 17:{
			mid_PLC.CE_MountBatBack_flg = recv_buf[3];	
			mid_PLC.CE_MountBatBack_err = recv_buf[4];
			// if(CE_MountBatBack_flg==1){
			// 	MCE_GetMountBat_flg = 1;
			// }
			// else MCE_GetMountBat_flg = 2;	
		}										
		break;
		case 19:{
			mid_PLC.CE_RGVOutChangPlatBack_flg = recv_buf[3];	
			mid_PLC.CE_RGVOutChangPlatBack_err = recv_buf[4];
		}
		break;

		case 23:{
			mid_PLC.CE_TimeOutBack_flg = recv_buf[3];	
			mid_PLC.CE_TimeOutBack_err = recv_buf[4];
//			printf("CE_TimeOutBack_flg%d\n",mid_PLC.CE_TimeOutBack_flg);
			// if(CE_TimeOutBack_flg==1){
			// 	MCE_GetTimeOut_flg = 1;
			// }
			// else MCE_GetTimeOut_flg = 2;		
		}								
		break;
		case 25:{
			mid_PLC.CE_ContinueBack_flg = recv_buf[3];	
			mid_PLC.CE_ContinueBack_err = recv_buf[4];
//			printf("CE_ContinueBack_flg%d\n",mid_PLC.CE_ContinueBack_flg);
			// if(CE_ContinueBack_flg==1){
			// 	MCE_GetContinue_flg = 1;
			// }
			// else MCE_GetContinue_flg = 2;	
		}								
		break;
		case 27:{
			mid_PLC.CE_StopBack_flg = recv_buf[3];	
			mid_PLC.CE_StopBack_err = recv_buf[4];	
//			printf("CE_StopBack_flg%d\n",mid_PLC.CE_StopBack_flg);
			// if(CE_StopBack_flg==1){
			// 	MCE_GetStop_flg = 1;
			// }
			// else MCE_GetStop_flg = 2;	
		}								
		break;
		case 29:{
			mid_PLC.CE_ErrResetBack_flg = recv_buf[3];	
			mid_PLC.CE_ErrResetBack_err = recv_buf[4];
//			printf("CE_ErrResetBack_flg%d\n",mid_PLC.CE_ErrResetBack_flg);
			// if(CE_ErrResetBack_flg==1){
			// 	MCE_GetErrReset_flg = 1;
			// }
			// else MCE_GetErrReset_flg = 2;		
		}									
		break;
		case 30:
			mid_PLC.CE_RunState = recv_buf[3];		
		break;
		case 31:
			mid_PLC.CE_ControlMode = recv_buf[3];		
		break;
		case 32:{
			mid_PLC.CE_ErrData[0] = recv_buf[3];	
			mid_PLC.CE_ErrData[1] = recv_buf[4];	
		}
			// printf("CE_ErrData=%d %d\n",recv_buf[3],recv_buf[4]);
		break;
		case 33:
			mid_PLC.CE_CarTypeNow = recv_buf[3];		
		break;
		case 34:
			mid_PLC.CE_RGVStepData = recv_buf[3];		
		break;
		case 35:
			mid_PLC.CE_StackerStepData = recv_buf[3];
			// printf("stracker%d\n",CE_StackerStepData);	
		break;
		case 36:
			mid_PLC.CE_ParkPlatStepData = recv_buf[3];		
		break;
		case 37:
			mid_PLC.CE_RotaTroSta = recv_buf[3];		
		break;
		case 38:
		{
			mid_PLC.CE_GetCBOk_flg = 1;	
			Send_CE_CEOK_BACK_flg = 1;	
		}
		break;
		case 40:{
			mid_PLC.CE_GetDismountBatOk_flg = 1;	
			Send_CE_DISMOUNTOK_BACK_flg = 1;
		}
		break;
		case 42:{
			mid_PLC.CE_GetMountBatOk_flg = 1;	
			Send_CE_MOUNTOK_BACK_flg = 1;	
		}
		break;
		case 44:{
			mid_PLC.CE_GetInLocatorOk_flg = 1;		
			Send_CE_INLOCATOROK_BACK_flg = 1;
		}
		break;
		case 46:{
			mid_PLC.CE_GetOutLocatorOk_flg = 1;	
			Send_CE_OUTLOCATOROK_BACK_flg = 1;	
		}
		break;
		case 48:{
			mid_PLC.CE_GetCELocatorOk_flg = 1;
			mid_PLC.CE_CELocatorOk_sour = recv_buf[3];
			mid_PLC.CE_CELocatorOk_des = recv_buf[4];	
			Send_CE_CELOCATOROK_BACK_flg = 1;	
		}
		break;
		case 50:{
			mid_PLC.CE_BatToLocatorOk_flg = recv_buf[3];	//???????	
			// Send_CE_BATTOLOCATOROK_BACK_flg = 1;
		}
		break;
		case 52:{
			for(i=0;i<20;i++){
				mid_PLC.CE_LocatorSenser[i] = recv_buf[3+i];
			}  
		}   
			// printf("CE_RGVSenser_sta%d%d\n",CE_LocatorSenser[10],CE_LocatorSenser[11]);			
		break;
		case 53:           
			mid_PLC.CE_RGVSenser_sta = recv_buf[3];	
			// printf("CE_RGVSenser_sta%d\n",CE_RGVSenser_sta);
		break;
		case 54:           
			mid_PLC.CE_StackerSenser_sta = recv_buf[3];	
			// printf("CE_StackerSenser_sta%d\n",CE_StackerSenser_sta);
		break;
		case 55:            
			mid_PLC.CE_TurnOverSenser_sta = recv_buf[3];	
			// printf("CE_TurnOverSenser_sta: %d \n",CE_TurnOverSenser_sta);
		break;
		case 56:            	
			mid_PLC.CE_SliOutSenser_sta = recv_buf[3];
			// printf("CE_SliOutSenser_sta: %d \n",CE_SliOutSenser_sta);	
		break;
		case 57:            	
			mid_PLC.CE_UpDownDoor_sta = recv_buf[3];	
			// printf("CE_UpDownDoor_sta: %d \n",CE_UpDownDoor_sta);
		break;
		case 59:       
		for(i=0;i<20;i++)   
		{
			mid_PLC.CE_ElecLinear_sta[i] = recv_buf[i+3];	
			// printf("CE_ElecLinear_sta[i]: %d \n",CE_ElecLinear_sta[i]);
		}  	
		break;
		case 60:            	
			mid_PLC.CE_ParkPlatSenser_sta = recv_buf[3];
			// if(mid_PLC.CE_ParkPlatSenser_sta == 0x02)
			// {                                     
			// 	MCE_InPlacSens_flg = 1;
			// 	// printf("MCE_InPlacSens_flg: %d \n",MCE_InPlacSens_flg);
			// }	
			// else  
			// 	MCE_InPlacSens_flg = 0;
		break;

		default:
		break;
	}
}
uint16_t CE_Buf1_dlc,CE_Buf2_dlc,CE_Buf3_dlc;

void CE_Rec_Deal(int len)
{
	uint16_t i;
	static uint16_t num = 0;
	// Task_100ms_cou++;
	// printf("Task_100ms_cou:%d\r\n", Task_100ms_cou);
	
    uint16_t RecvData_dlc;
    uint16_t current_head = 0;
	uint8_t buf_temp[100];

	do{
       if(CE_Recv_buf[current_head + 0]==0xfe)
        {
            RecvData_dlc = CE_Recv_buf[current_head + 2];//当前帧的数据单元长度

			for(i = 0;i<RecvData_dlc + 3;i++)
			{
				buf_temp[i] = CE_Recv_buf[current_head + i];
			}

			Test_Deal(buf_temp,RecvData_dlc);
			bzero(buf_temp, sizeof(buf_temp));
			current_head = current_head + RecvData_dlc + 3;
			num++;
			// printf("recv_num: %d \n",num);
		}
	}while(current_head + 3 <= len);

	bzero(CE_Recv_buf, sizeof(CE_Recv_buf));
	//memcpy(CE_Recv_buf,0,500);	
}

void CEInter_Crc16_Deal(uint8_t add){
	CESend_CHANGEBATBAG_buf[3] = mid_PLC.MCE_CBLocator_sour;
	CESend_CHANGEBATBAG_buf[4] = mid_PLC.MCE_CBLocator_CarTy;
	mid_PLC.MCE_CBLocator_sour=0,mid_PLC.MCE_CBLocator_CarTy=0;

	CESend_BATBAGINLOCATOR_buf[3] = mid_PLC.MCE_InLocator_dest;
	CESend_BATBAGINLOCATOR_buf[4] = mid_PLC.MCE_InLocator_CarTy;
	mid_PLC.MCE_InLocator_dest=0,mid_PLC.MCE_InLocator_CarTy=0;

	CESend_BATBAGOUTLOCATOR_buf[3] = mid_PLC.MCE_OutLocator_sour;
	CESend_BATBAGOUTLOCATOR_buf[4] = mid_PLC.MCE_OutLocator_CarTy;
	mid_PLC.MCE_OutLocator_sour=0,mid_PLC.MCE_OutLocator_CarTy=0;

	CESend_CHANGELOCATOR_buf[3] = mid_PLC.MCE_CELocator_sour;
	CESend_CHANGELOCATOR_buf[4] = mid_PLC.MCE_CELocator_dest;
	CESend_CHANGELOCATOR_buf[5] = mid_PLC.MCE_CELocator_CarTy;
	mid_PLC.MCE_CELocator_sour=0,mid_PLC.MCE_CELocator_dest,mid_PLC.MCE_CELocator_CarTy=0;

	CESend_DISMOUNTBATBAG_buf[3] = mid_PLC.MCE_DismountBat_CarTy;
	CESend_DISMOUNTBATBAG_buf[4] = 0x00;

	CESend_MOUNTBATBAG_buf[3] = mid_PLC.MCE_MountBat_CarTy;
	CESend_MOUNTBATBAG_buf[4] = 0x00;

	CESend_BATBAGOUT_buf[3] = mid_PLC.MCE_BatOut_sour;
	CESend_BATBAGOUT_buf[4] = mid_PLC.MCE_BatOut_CarTy;
	// mid_PLC.MCE_BatOut_sour=0,mid_PLC.MCE_BatOut_CarTy=0;

	CESend_TIMEOUT_buf[3] = mid_PLC.MCE_TimeOut_sour;
	CESend_TIMEOUT_buf[4] = mid_PLC.MCE_TimeOut_dest;
	CESend_CONTINUE_buf[3] = mid_PLC.MCE_Continue_sour;
	CESend_CONTINUE_buf[4] = mid_PLC.MCE_Continue_dest;
	CESend_STOP_buf[3] = mid_PLC.MCE_Stop_sour;
	CESend_STOP_buf[4] = mid_PLC.MCE_Stop_dest;
	CESend_ERRRESET_buf[3] = mid_PLC.MCE_ErrReset_sour;
	CESend_ERRRESET_buf[4] = mid_PLC.MCE_ErrReset_dest;

}
void mid_PLC_Interface(){
	/*  Hand_CE */
	MCE_StackerSenser_sta = mid_PLC.CE_StackerSenser_sta;
	QT_HandCE_flg = 1;
	MCE_RGVSenser_sta=mid_PLC.CE_RGVSenser_sta;
	MCE_RunState = mid_PLC.CE_RunState;
	MCE_ControlMode = mid_PLC.CE_ControlMode;

	MCE_ErrData = ((mid_PLC.CE_ErrData[0]!=0)||(mid_PLC.CE_ErrData[1]!=0));
	CE_ErrData[0] = mid_PLC.CE_ErrData[0];
	CE_ErrData[1] = mid_PLC.CE_ErrData[1];
	// MCE_CarTypeNow = CE_CarTypeNow;
	MCE_RGVStepData = mid_PLC.CE_RGVStepData;
	MCE_StackerStepData = mid_PLC.CE_StackerStepData;
	MCE_ParkPlatStepData = mid_PLC.CE_ParkPlatStepData;
	// MCE_RotaTroSta = CE_RotaTroSta;
	// MCE_ParkPlatStepData = 1;

	int i=0;MCE_AllBag_num = 0;
	for(i=0;i<20;i++)   
	{
		if(MCE_Comm_flg==1)
		{
			CE_LocatorSenser[i] = mid_PLC.CE_LocatorSenser[i];
			MCE_AllBag_num += (mid_PLC.CE_LocatorSenser[i]==2&&(CHU_DevState_code[i]>=3));	
		}
	}  
	if(mid_PLC.CE_ParkPlatSenser_sta == 0x02){                                     
//		MCE_InPlacSens_flg = 1;
	}	
	else  
//		MCE_InPlacSens_flg = 0;

	mid_PLC.MCE_CBLocator_sour = MCE_CBLocator_sour;
	// MCE_CBLocator_sour = 255;
	mid_PLC.MCE_CBLocator_CarTy = MCE_CBLocator_CarTy;
	// MCE_CBLocator_CarTy = 0;
	mid_PLC.MCE_InLocator_dest = MCE_InLocator_dest;
	// MCE_InLocator_dest = 255;
	mid_PLC.MCE_InLocator_CarTy = MCE_InLocator_CarTy;
	// MCE_InLocator_CarTy = 0;
	mid_PLC.MCE_OutLocator_sour = MCE_OutLocator_sour;
	// MCE_OutLocator_sour = 255;
	mid_PLC.MCE_OutLocator_CarTy = MCE_OutLocator_CarTy;
	// MCE_OutLocator_CarTy = 0;
	mid_PLC.MCE_CELocator_sour = MCE_CELocator_sour;
	// MCE_CELocator_sour = 255;
	mid_PLC.MCE_CELocator_dest = MCE_CELocator_dest;
	// MCE_CELocator_dest = 255;

	// printf("mid_PLC.MCE_CELocator_sour = %d mid_PLC.MCE_CELocator_dest  = %d\n\n",mid_PLC.MCE_CELocator_sour,mid_PLC.MCE_CELocator_dest);
	mid_PLC.MCE_CELocator_CarTy = MCE_CELocator_CarTy;
	mid_PLC.MCE_DismountBat_CarTy = MCE_DismountBat_CarTy;
	mid_PLC.MCE_MountBat_CarTy = MCE_MountBat_CarTy;
	mid_PLC.MCE_BatOut_sour = MCE_BatOut_sour;
	mid_PLC.MCE_BatOut_CarTy = MCE_BatOut_CarTy;
	mid_PLC.MCE_TimeOut_sour = MCE_TimeOut_sour;
	mid_PLC.MCE_TimeOut_dest = MCE_TimeOut_dest;
	mid_PLC.MCE_Continue_sour = MCE_Continue_sour;
	mid_PLC.MCE_Continue_dest = MCE_Continue_dest;
	mid_PLC.MCE_Stop_sour = MCE_Stop_sour;
	mid_PLC.MCE_Stop_dest = MCE_Stop_dest;
	mid_PLC.MCE_ErrReset_sour = MCE_ErrReset_sour;
	mid_PLC.MCE_ErrReset_dest = MCE_ErrReset_dest;


	if(mid_PLC.CE_InitBack_flg){
		MCE_GetInit_flg = mid_PLC.CE_InitBack_flg;
		mid_PLC.CE_InitBack_flg = 0;
	}
	if(mid_PLC.CE_ChangBatBack_flg){
		MCE_GetChangeBat_flg = mid_PLC.CE_ChangBatBack_flg;
		mid_PLC.CE_ChangBatBack_flg = 0;
	}
	if(mid_PLC.CE_ChangLocatorBack_flg){
		MCE_GetCELocator_flg = mid_PLC.CE_ChangLocatorBack_flg;
		mid_PLC.CE_ChangLocatorBack_flg = 0;
	}
	if(mid_PLC.CE_BatOutBack_flg){
		MCE_GetBatOut_flg = mid_PLC.CE_BatOutBack_flg;
		mid_PLC.CE_BatOutBack_flg = 0;
	}
	if(mid_PLC.CE_BatInLocatorBack_flg){
		MCE_GetInLocator_flg = mid_PLC.CE_BatInLocatorBack_flg;
		mid_PLC.CE_BatInLocatorBack_flg = 0;
	}
	if(mid_PLC.CE_BatOutLocatorBack_flg){
		MCE_GetOutLocator_flg = mid_PLC.CE_BatOutLocatorBack_flg;
		mid_PLC.CE_BatOutLocatorBack_flg = 0;
	}
	if(mid_PLC.CE_TimeOutBack_flg){
		MCE_GetTimeOut_flg = mid_PLC.CE_TimeOutBack_flg;
		mid_PLC.CE_TimeOutBack_flg = 0;
	}
	if(mid_PLC.CE_ContinueBack_flg){
		MCE_GetContinue_flg = mid_PLC.CE_ContinueBack_flg;
		mid_PLC.CE_ContinueBack_flg = 0;
	}
	if(mid_PLC.CE_StopBack_flg){
		MCE_GetStop_flg = mid_PLC.CE_StopBack_flg;
		mid_PLC.CE_StopBack_flg = 0;
	}
	if(mid_PLC.CE_ErrResetBack_flg){
		MCE_GetErrReset_flg = mid_PLC.CE_ErrResetBack_flg;
		mid_PLC.CE_ErrResetBack_flg = 0;
	}
	if(mid_PLC.CE_ErrResetBack_flg){
		MCE_GetErrReset_flg = mid_PLC.CE_ErrResetBack_flg;
		mid_PLC.CE_ErrResetBack_flg = 0;
	}
	if(mid_PLC.CE_GetCBOk_flg){
		MCE_GetCBOk_flg = mid_PLC.CE_GetCBOk_flg;
		mid_PLC.CE_GetCBOk_flg = 0;
	}
	if(mid_PLC.CE_GetInLocatorOk_flg){
		MCE_GetInLocatorOk_flg = mid_PLC.CE_GetInLocatorOk_flg;
		mid_PLC.CE_GetInLocatorOk_flg = 0;
	}
	if(mid_PLC.CE_GetOutLocatorOk_flg){
		MCE_GetOutLocatorOk_flg = mid_PLC.CE_GetOutLocatorOk_flg;
		mid_PLC.CE_GetOutLocatorOk_flg = 0;
	}
	if(mid_PLC.CE_GetCELocatorOk_flg){
		MCE_GetCELocatorOk_flg = mid_PLC.CE_GetCELocatorOk_flg;
		mid_PLC.CE_GetCELocatorOk_flg = 0;
	}
}

void thread_CE(void)
{
	//定义sockfd
    int clientsockfd = socket(AF_INET, SOCK_STREAM, O_NONBLOCK);
	struct sockaddr_in servaddr;
	
	int8_t conn1 = -1;
	uint8_t Task_1s_flg = 0;

	uint8_t recv_cou;
	uint8_t Send1_cou;
	int16_t Send_flg;

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);  ///服务器端口
    servaddr.sin_addr.s_addr = inet_addr("192.168.1.153");  ///服务器ip

	conn1 = connect(clientsockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
	while(conn1<0)
	{
		close(clientsockfd);
		sleep(2);
		clientsockfd = socket(AF_INET, SOCK_STREAM, 0);
		int flags = fcntl(clientsockfd, F_GETFL, 0);  
		fcntl(clientsockfd, F_SETFL, flags|O_NONBLOCK);
		conn1 = connect(clientsockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
		sleep(2);
		// if(recv(clientsockfd, CE_Recv_buf, BUFFER_SIZE,MSG_DONTWAIT|MSG_NOSIGNAL)>=0)
		// {
		// 	conn1 = 1;
		// }
		conn1 = send(clientsockfd, Send_ReConnect_buf, (1),MSG_DONTWAIT|MSG_NOSIGNAL);
//		fprintf(stderr, "connect CE function failed.\n");
		MCE_Comm_flg = 2;
		//sleep(10);
	}
	MCE_Comm_flg = 1;
	while(1)
	{	
		mid_PLC_Interface();
		// ChangeElec();	
		if(Send_CE_INIT_flg == 1)
		{
//			printf("Send_CE_INIT_flg%d\n",Send_CE_INIT_flg);
			syslog(LOG_INFO,"PLC Send_CE_INIT_flg\n");
			
			Send_CE_INIT_flg = 0;
			CEInter_Crc16_Deal(1);
			send(clientsockfd, CESend_INIT_buf,(3),MSG_DONTWAIT|MSG_NOSIGNAL); 
		}
		if(Send_CE_CHANGEBAT_flg == 1)
		{		
			Send_CE_CHANGEBAT_flg = 0;
			CEInter_Crc16_Deal(1);
//			printf("Send_CE_CHANGEBAT_flg%d%d%d%d\n",CESend_CHANGEBATBAG_buf[1],CESend_CHANGEBATBAG_buf[2],CESend_CHANGEBATBAG_buf[3],CESend_CHANGEBATBAG_buf[4]);
			syslog(LOG_INFO,"PLC Send_CE_CHANGEBAT_flg %d %d %d %d\n",\
			CESend_CHANGEBATBAG_buf[1],CESend_CHANGEBATBAG_buf[2],CESend_CHANGEBATBAG_buf[3],CESend_CHANGEBATBAG_buf[4]);
			
			send(clientsockfd, CESend_CHANGEBATBAG_buf,(5),MSG_DONTWAIT|MSG_NOSIGNAL); 
		}
		if(Send_CE_INLOCATOR_flg == 1)
		{
			Send_CE_INLOCATOR_flg = 0;
			CEInter_Crc16_Deal(1);
//			printf("CESend_BATBAGINLOCATOR_buf%d%d%d%d\n",CESend_BATBAGINLOCATOR_buf[1],CESend_BATBAGINLOCATOR_buf[2],CESend_BATBAGINLOCATOR_buf[3],CESend_BATBAGINLOCATOR_buf[4]);
			syslog(LOG_INFO,"PLC Send_BATBAGINLOCATOR_flg %d %d %d %d\n",\
			CESend_BATBAGINLOCATOR_buf[1],CESend_BATBAGINLOCATOR_buf[2],CESend_BATBAGINLOCATOR_buf[3],CESend_BATBAGINLOCATOR_buf[4]);
			
			send(clientsockfd, CESend_BATBAGINLOCATOR_buf,(5),MSG_DONTWAIT|MSG_NOSIGNAL); 
		}
		if(Send_CE_OUTLOCATOR_flg == 1)
		{
//			printf("CESend_BATBAGOUTLOCATOR_buf%d%d%d%d\n",CESend_BATBAGOUTLOCATOR_buf[1],CESend_BATBAGOUTLOCATOR_buf[2],CESend_BATBAGOUTLOCATOR_buf[3],CESend_BATBAGOUTLOCATOR_buf[4]);
			syslog(LOG_INFO,"PLC Send_BATBAGPOUTLOCATOR_flg %d %d %d %d\n",\
			CESend_BATBAGOUTLOCATOR_buf[1],CESend_BATBAGOUTLOCATOR_buf[2],CESend_BATBAGOUTLOCATOR_buf[3],CESend_BATBAGOUTLOCATOR_buf[4]);
			
			Send_CE_OUTLOCATOR_flg = 0;
			CEInter_Crc16_Deal(1);
			send(clientsockfd, CESend_BATBAGOUTLOCATOR_buf,(5),MSG_DONTWAIT|MSG_NOSIGNAL); 
		}
		if(Send_CE_CHANGELOCATOR_flg == 1)
		{
			Send_CE_CHANGELOCATOR_flg = 0;
//			printf("mid_PLC.MCE_CELocator_sour = %d mid_PLC.MCE_CELocator_dest  = %d\n",mid_PLC.MCE_CELocator_sour,mid_PLC.MCE_CELocator_dest);
			syslog(LOG_INFO,"Send_CE_CHANGELOCATOR_flg %d %d\n",mid_PLC.MCE_CELocator_sour,mid_PLC.MCE_CELocator_dest);

			CEInter_Crc16_Deal(1);

			send(clientsockfd, CESend_CHANGELOCATOR_buf,(6),MSG_DONTWAIT|MSG_NOSIGNAL); 
		}
		if(Send_CE_BATBAGOUT_flg == 1)
		{
			syslog(LOG_INFO,"Send_CE_BATBAGOUT_flg%d %d %d\n",Send_CE_BATBAGOUT_flg,CESend_BATBAGOUT_buf[3],CESend_BATBAGOUT_buf[4]);
			Send_CE_BATBAGOUT_flg = 0;
			CEInter_Crc16_Deal(1);
			send(clientsockfd, CESend_BATBAGOUT_buf,(5),MSG_DONTWAIT|MSG_NOSIGNAL); 
		}
		// if(Send_CE_DISMOUNTBAT_flg == 1)
		if(0)
		{
			Send_CE_DISMOUNTBAT_flg = 0;
			CEInter_Crc16_Deal(1);
			send(clientsockfd, CESend_DISMOUNTBATBAG_buf,(5),MSG_DONTWAIT|MSG_NOSIGNAL); 
		}
		// if(Send_CE_MOUNTBAT_flg == 1)
		if(0)
		{
			Send_CE_MOUNTBAT_flg = 0;
			CEInter_Crc16_Deal(1);
			send(clientsockfd, CESend_MOUNTBATBAG_buf,(5),MSG_DONTWAIT|MSG_NOSIGNAL); 
		}
		//if(Send_CE_RGVOUTCHANGPLAT_flg == 1)
		if(0)
		{
			// Send_CE_RGVOUTCHANGPLAT_flg = 0;
			CEInter_Crc16_Deal(1);
			send(clientsockfd, CESend_RGVOUTCHANGPLAT_buf,(5),MSG_DONTWAIT|MSG_NOSIGNAL); 
		}
		if(Send_CE_TIMEOUT_flg == 1)
		{
			syslog(LOG_INFO,"Send_CE_TIMEOUT_flg%d\n",Send_CE_TIMEOUT_flg);
			Send_CE_TIMEOUT_flg = 0;
			CEInter_Crc16_Deal(1);
			send(clientsockfd, CESend_TIMEOUT_buf,(5),MSG_DONTWAIT|MSG_NOSIGNAL); 
		}
		if(Send_CE_CONTINUE_flg == 1)
		{
			syslog(LOG_INFO,"Send_CE_CONTINUE_flg%d\n",Send_CE_CONTINUE_flg);
			Send_CE_CONTINUE_flg = 0;
			CEInter_Crc16_Deal(1);
			send(clientsockfd, CESend_CONTINUE_buf,(5),MSG_DONTWAIT|MSG_NOSIGNAL); 
		}
		if(Send_CE_STOP_flg == 1)
		{
			syslog(LOG_INFO,"Send_CE_STOP_flg%d\n",Send_CE_STOP_flg);
			Send_CE_STOP_flg = 0;
			CEInter_Crc16_Deal(1);
			send(clientsockfd, CESend_STOP_buf,(5),MSG_DONTWAIT|MSG_NOSIGNAL); 
		}
		if(Send_CE_ERRRESET_flg == 1)
		{
			syslog(LOG_INFO,"Send_CE_ERRRESET_flg%d\n",Send_CE_ERRRESET_flg);
			Send_CE_ERRRESET_flg = 0;
			CEInter_Crc16_Deal(1);
			send(clientsockfd, CESend_ERRRESET_buf,(5),MSG_DONTWAIT|MSG_NOSIGNAL); 
		}
		static uint8_t CE_SendHeart_cou = 0;
		uint8_t CESend_HEART_buf[3] = {0xfe,0x3a,0x00};
			if(CE_SendHeart_cou<=10)
				CE_SendHeart_cou++;
			else
			    CE_SendHeart_cou = 0;
		if(CE_SendHeart_cou==5)
		{					
			// Send_CE_MOUNTBAT_flg = 0;
			CEInter_Crc16_Deal(1);
			send(clientsockfd, CESend_HEART_buf,(3),MSG_DONTWAIT|MSG_NOSIGNAL); 
		}
		if(Send_CE_CEOK_BACK_flg == 1)
		{
			Send_CE_CEOK_BACK_flg = 0;
			CEInter_Crc16_Deal(1);
			send(clientsockfd, CESend_CEOK_BACK_buf,(3),MSG_DONTWAIT|MSG_NOSIGNAL); 
		}
		if(Send_CE_DISMOUNTOK_BACK_flg == 1)
		{
			Send_CE_DISMOUNTOK_BACK_flg = 0;
			CEInter_Crc16_Deal(1);
			send(clientsockfd, CESend_DISMOUNTOK_BACK_buf,(3),MSG_DONTWAIT|MSG_NOSIGNAL); 
		}
		if(Send_CE_MOUNTOK_BACK_flg == 1)
		{
			Send_CE_MOUNTOK_BACK_flg = 0;
			CEInter_Crc16_Deal(1);
			send(clientsockfd, CESend_MOUNTOK_BACK_buf,(3),MSG_DONTWAIT|MSG_NOSIGNAL); 
		}
		if(Send_CE_INLOCATOROK_BACK_flg == 1)
		{
			Send_CE_INLOCATOROK_BACK_flg = 0;
			CEInter_Crc16_Deal(1);
			send(clientsockfd, CESend_INLOCATOROK_BACK_buf,(3),MSG_DONTWAIT|MSG_NOSIGNAL); 
		}
		if(Send_CE_OUTLOCATOROK_BACK_flg == 1)
		{
			Send_CE_OUTLOCATOROK_BACK_flg = 0;
			CEInter_Crc16_Deal(1);
			send(clientsockfd, CESend_OUTLOCATOROK_BACK_buf,(3),MSG_DONTWAIT|MSG_NOSIGNAL); 
		}
		if(Send_CE_CELOCATOROK_BACK_flg == 1)
		{
			Send_CE_CELOCATOROK_BACK_flg = 0;
			CEInter_Crc16_Deal(1);
			send(clientsockfd, CESend_CELOCATOROK_BACK_buf,(3),MSG_DONTWAIT|MSG_NOSIGNAL); 
		}
		if(Send_CE_BATTOLOCATOROK_BACK_flg == 1)
		{
			Send_CE_BATTOLOCATOROK_BACK_flg = 0;
			CEInter_Crc16_Deal(1);
			send(clientsockfd, CESend_BATTOLOCATOROK_BACK_buf,(3),MSG_DONTWAIT|MSG_NOSIGNAL); 
		}
		if(Send_CE_ELECLINEARSTA_flg == 1)
		{
			Send_CE_ELECLINEARSTA_flg = 0;
			CEInter_Crc16_Deal(1);
			Send_flg = send(clientsockfd, CESend_ELECLINEARSTA_buf,(23),MSG_DONTWAIT|MSG_NOSIGNAL); //发送充放电指令-500ms
							
		}	
		if(CESend_Discon_con>=10)
		{			
			MCE_Comm_flg = 2;
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
				sleep(5);
				Send_flg = send(clientsockfd, Send_ReConnect_buf, (1),MSG_DONTWAIT|MSG_NOSIGNAL);

//				fprintf(stderr, "connect CE function failed.\n");
				syslog(LOG_INFO,"换电设备connect CE function failed Send_flg \n");
				MCE_Comm_flg = 2;
				//sleep(10);				
				// CESend_Discon_con = 0;
				int i = 0;
				for(i=0;i<20;i++)
				{
					CE_LocatorSenser[i] = 0;
				}	
			}	
			CESend_Discon_con = 0;	
		}
		else
		{
			MCE_Comm_flg = 1;
		}
		usleep(50*1000);//10ms	
		int CE_RecvData_len = 0;
		if((CE_RecvData_len = recv(clientsockfd, CE_Recv_buf, BUFFER_SIZE,MSG_DONTWAIT|MSG_NOSIGNAL)) < 0)
		{
			;
		}	
		else
		{
			// printf("Qt_Recv_buf %d\n",Qt_Recv_buf[0]);
			CE_Rec_Deal(CE_RecvData_len);
			CESend_Discon_con = 0;
		}
	}
	close(clientsockfd);
}

// extern void thread_7(void);
// int main(void)
// {
//     pthread_t id_1,id_2,id_3,id_4,id_5,id_6,id_7;
//     int i,ret;
// 	printf("thread test!!!\n");

// /*创建线程一    */	
// 	ret=pthread_create(&id_1,NULL,(void  *) thread_1,NULL);    		
// 	if(ret!=0)    	
// 	{        	
// 		printf("Create pthread 1 error!\n");    		
// 		return -1;    	
// 	}
// 	else
// 	{
// 		printf("thread 1 OK!\n");
// 	}
// 	ret=pthread_create(&id_7,NULL,(void  *) thread_7,NULL);    		
// 	if(ret!=0)    	
// 	{        	
// 		printf("Create pthread 7 error!\n");    		
// 		return -1;    	
// 	}
// 	else
// 	{
// 		printf("thread 7 OK!\n");
// 	}
// 	while(1)
// 	{
// 		sleep(1);		//do something or delay
// 		//printf("main thread!\n");
// 	}
// 	return 0;
// }
