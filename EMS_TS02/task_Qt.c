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

#define PORT                  5000
#define BUFFER_SIZE           1000
#define SENDDATA_SIZE         83


mid_qtTcp_t mid_qtTcp;

uint8_t Send_ReConnect_buf[10]={0};

uint8_t QtSend_Sev2Back_buf[6]={0xFE,0x82,0x1,0x0,0x55,0x55};
uint8_t QtSend_Sev3Back_buf[6]={0xFE,0x83,0x1,0x0,0x55,0x55};
uint8_t QtSend_Sev4Back_buf[6]={0xFE,0x84,0x1,0x0,0x55,0x55};
uint8_t QtSend_Sev5Back_buf[6]={0xFE,0x85,0x1,0x0,0x55,0x55};
uint8_t QtSend_Sev6Back_buf[6]={0xFE,0x86,0x1,0x0,0x55,0x55};
uint8_t QtSend_Sev12Back_buf[6]={0xFE,0x8c,0x1,0x0,0x55,0x55};
uint8_t QtSend_Sev13_buf[9]={0xFE,0x8d,0x04,0x0,0x0,0x0,0x0,0x55,0x55};
uint8_t QtSend_Sev14Back_buf[6]={0xFE,0x8e,0x1,0x0,0x55,0x55};
uint8_t QtSend_Sev15Back_buf[6]={0xFE,0x8f,0x1,0x0,0x55,0x55};
uint8_t QtSend_Sev17Back_buf[6]={0xFE,0x91,0x1,0x0,0x55,0x55};
uint8_t QtSend_Sev18Back_buf[6]={0xFE,0x92,0x1,0x0,0x55,0x55};

// uint8_t QtSend_CEErr_buf[500]={0xFE,0x8A,0x0,0x0};
uint8_t QtSend_CEData_buf[500]={0xFE,0x8A,0x0,0x0};
// uint8_t Data_CEErr_buf[500];


uint8_t Qt_Recv_buf[500];
uint8_t Qt_AllData_buf[500];
uint8_t Qt_Data3_buf[500];
uint8_t Qt_Data4_buf[500];
uint8_t Qt_Data5_buf[500];
uint8_t Qt_Data6_buf[500];
uint8_t Qt_Data12_buf[500];
uint8_t Qt_Data13_buf[500];
uint32_t Qt_Data14_buf[500];
uint8_t Qt_Data15_buf[500];
uint8_t Qt_Data17_buf[500];
uint8_t Qt_Data18_buf[500];
uint16_t Qt_RecvData_dlc,Qt_Buf1_dlc;
uint8_t Recv_Server_buf[500];
uint8_t QtBook_BatNax_num[3][50];
uint8_t QtEMS_ShutDown_flg;

uint8_t Qt_conn_flg = 0;
uint16_t QtSend_Discon_con;

int16_t  Usr_PowerReq_kw;
uint8_t EMS_Run_sta;
uint8_t EMS_SysStartStop_sta;
uint8_t EMS_SelfSet_flg;
uint8_t EMS_HandSet_flg;
uint8_t EMS_SelfRun_flg;
u_int32_t Recv_count;
int32_t Read_Size;
int32_t Read_Size1;

uint8_t Qt_SendSev2Back_flg;
uint8_t Qt_SendSev2Back_Data;
uint8_t Qt_SendSev3Back_flg;
uint8_t Qt_SendSev3Back_Data;
uint8_t Qt_SendSev4Back_flg;
uint8_t Qt_SendSev4Back_Data;
uint8_t Qt_SendSev5Back_flg;
uint8_t Qt_SendSev5Back_Data;
uint8_t Qt_SendSev6Back_flg;
uint8_t Qt_SendSev6Back_Data;
uint8_t Qt_SendSev12Back_flg;
uint8_t Qt_SendSev14Back_flg;
uint8_t Qt_SendSev15Back_flg;
uint8_t Qt_SendSev17Back_flg;
uint8_t Qt_SendSev18Back_flg;
uint8_t Qt_SendChgerErr_flg;
uint8_t Qt_SendChgData1_flg;
uint8_t Qt_SendChgData2_flg;
uint8_t Qt_SendCEData_flg;

uint8_t Qt_CarAllowIn_err;
uint8_t Qt_CarAllowOut_err;
uint8_t Qt_CarODO_err;
uint8_t Qt_CancelOrd_err = 0;
uint8_t Qt_InNet_err[20];

uint8_t Qt_ACEnergySetPT_flg = 0;
uint8_t Qt_ACEnergySetCT_flg = 0;
uint16_t Qt_ACEnergySetPT_tmp = 0;
uint16_t Qt_ACEnergySetCT_tmp = 0;

uint8_t QtIo_Disable_buf[8] ={0};

void QtRecv_Server_Deal(uint16_t current_head,uint16_t dlc)
{
	uint8_t i=0;
	// Recv_count++;
	switch (Recv_Server_buf[current_head+1])
		{
		case 1:{
			QtEMS_ShutDown_flg = (Recv_Server_buf[current_head+4]==1);
			
			for(i = 0;i < 8;i++){//IO板卡屏蔽信号
				QtIo_Disable_buf[i] = ((Recv_Server_buf[current_head+5]>>i)&0x1);
			}
		}break;
		case 2:{
			for(i=0;i<dlc;i++)
			{
				Qt_AllData_buf[i] = Recv_Server_buf[current_head+3+i];
				// printf("Qt_data=%d\n",Qt_AllData_buf[i]);
			}
			Qt_SendSev2Back_flg = 1;
			Qt_SendSev2Back_Data = Qt_AllData_buf[0];
			QtSend_Sev2Back_buf[3] = Qt_SendSev2Back_Data;
			// printf("MCE_ErrData444444444444444=%d\n",Qt_AllData_buf[0]);
			switch(Qt_AllData_buf[0])
			{
				case 2:{
					QT_InitOrder_flg = 1;
	//				printf("MCE_ErrData11111111111111=%d\n",MCE_ErrData);
					// printf("Sys_sta[1]=%d!\n",Sys_sta[1]);
				}
				break;
				case 4:{

					QT_ChangeBatOrder_flg = 1;
					MCE_CBLocator_sour = Qt_AllData_buf[1];
					MCE_CBLocator_CarTy = Qt_AllData_buf[3];
	//				printf("MCE_CBLocator_sour=%d%d\n",mid_qtTcp.MCE_CBLocator_sour,mid_qtTcp.MCE_CBLocator_CarTy);
				}
				break;
				case 6:{
					QT_CELocatorOrder_flg = 1;
					MCE_CELocator_sour = Qt_AllData_buf[1];
					MCE_CELocator_dest = Qt_AllData_buf[2];
					MCE_CELocator_CarTy = Qt_AllData_buf[3];
				}
				break;
				case 8:{
					QT_BatOutOrder_flg = 1;
					MCE_BatOut_sour = Qt_AllData_buf[1];
					MCE_BatOut_CarTy = Qt_AllData_buf[3];
				}
				break;
				case 10:{
					QT_InLocatorOrder_flg = 1;
					MCE_InLocator_dest = Qt_AllData_buf[2];
					MCE_InLocator_CarTy = Qt_AllData_buf[3];
	//				printf("QT_InLocatorOrder_flg=%d %d",mid_qtTcp.MCE_InLocator_dest,mid_qtTcp.MCE_InLocator_CarTy);
				}break;
				case 12:{
					QT_OutLocatorOrder_flg = 1;
					MCE_OutLocator_sour = Qt_AllData_buf[1];
					MCE_OutLocator_CarTy = Qt_AllData_buf[3];
	//				printf("QT_OutLocatorOrder_flg=%d %d",mid_qtTcp.MCE_OutLocator_sour,mid_qtTcp.MCE_OutLocator_CarTy);
				}break;
				case 14:{
					QT_DismountBatOrder_flg = 1;
					MCE_DismountBat_CarTy = Qt_AllData_buf[3];
				}break;
				case 16:{
					QT_MountBatOrder_flg = 1;
					MCE_MountBat_CarTy = Qt_AllData_buf[3];
				}break;
				case 22:{
					QT_TimeOutOrder_flg = 1;
	//				printf("QT_TimeOutOrder_flg%d\n",QT_TimeOutOrder_flg);
				}break;
				case 24:{
					QT_ContinueOrder_flg = 1;
	//				printf("QT_ContinueOrder_flg=%d!\n",QT_ContinueOrder_flg);
				}break;
				case 26:{
					QT_StopOrder_flg = 1;
	//				printf("QT_StopOrder_flg\n");
				}break;
				case 28:{
					QT_ErrResetOrder_flg = 1;	
					MCE_Order_err = 0;				
					MCE_InitDealOrder_err = 0;
					MCE_ChangeBatOrder_err = 0;
					MCE_InLocatorOrder_err = 0;
					MCE_OutLocatorOrder_err = 0;
					MCE_BatOutOrder_err = 0;
					MCE_CELocatorOrder_err = 0;
					MCE_TimeOutOrder_err = 0;
					MCE_ContinueOrder_err = 0;
					MCE_StopOrder_err = 0;
					MCE_ErrResetOrder_err = 0;

					MCE_Initok_err = 0;
	//				printf("QT_ErrResetOrder_flg\n");
				}break;
				case 29:{
					MQt_StartSelfCE_sta = 1;//自动换电
	//				printf("MQt_StartSelfCE_sta%d\n",MQt_StartSelfCE_sta);
				}break;
				case 30:{
					MQt_StartSelfCE_sta = 2;//手动换电
	//				printf("MQt_StartSelfCE_sta%d\n",MQt_StartSelfCE_sta);
				}break;
				case 31:{
					//Qt_CarAllowOut_sta = 1; //云端放行
				}break;
				default:
				break;
			}
		}
			break;
		case 3:
		{
			for(i=0;i<dlc;i++)
			{
				Qt_Data3_buf[i] = Recv_Server_buf[current_head+3+i];
			}
			Qt_SendSev3Back_flg = 1;
			Qt_SendSev3Back_Data = Qt_Data3_buf[0];
			QtSend_Sev3Back_buf[3] = Qt_SendSev3Back_Data;
			if(Chg_Control_mode[Qt_Data3_buf[0]-1]==0){
				if(Qt_Data3_buf[2]==1)
					Hand_LowPwrOn_flg[Qt_Data3_buf[0]-1] = 1;
				else if(Qt_Data3_buf[2]==2)
					Hand_StartChg_flg[Qt_Data3_buf[0]-1] = 1;
				else if(Qt_Data3_buf[2]==3)
					Hand_StopChg_flg[Qt_Data3_buf[0]-1] = 1;
				else if(Qt_Data3_buf[2]==4)
					Hand_LowPwrDown_flg[Qt_Data3_buf[0]-1] = 1;

				if(Qt_Data3_buf[0]==18){
					// if(Qt_Data3_buf[2]==1)
					// 	Door_Start_flg=1,Door2_Start_flg=1;
					// else if(Qt_Data3_buf[2]==4)
					// 	Door_Start_flg=0,Door2_Start_flg=0;
					// else if(Qt_Data3_buf[2]==2)
					// 	MIO_WaterValue_null = 0;	//需要清掉IO板卡水禁
					// else if(Qt_Data3_buf[2]==3)
					// 	MIO_WaterValue_null = 1;
				}
				if(Qt_Data3_buf[0]==17){
					// if(Qt_Data3_buf[2]==1)
					// 	Door_Start_flg=1,Door2_Start_flg=1;
					// else if(Qt_Data3_buf[2]==4)
					// 	Door_Start_flg=0,Door2_Start_flg=0;
					// if(Qt_Data3_buf[2]==2)
					// 	FireContr_GetFire_flg = 1;	//需要清掉IO板卡水禁
					// else if(Qt_Data3_buf[2]==3)
					// 	FireContr_GetFire_flg = 0;
				}
			}
			Qt_HandCharPow_kw[Qt_Data3_buf[0]-1] = Qt_Data3_buf[1];
	//		printf("Qt_Data3_buf=%d,%d,%d,%d,%d\n",Qt_Data3_buf[0],Hand_LowPwrOn_flg[0],
	//		Hand_StartChg_flg[0],Hand_StopChg_flg[0],Hand_LowPwrDown_flg[0]);
		}
		break;
		case 4:
		{
			MQt_ConfDa_push = 1;
			for(i=0;i<dlc;i++)
			{
				Qt_Data4_buf[i] = Recv_Server_buf[current_head+3+i];
			}
			Qt_SendSev4Back_flg = 1;
			MLocat_AllowCESoc_pct = Qt_Data4_buf[0];
			MLocat_CharEndSoc_pct = Qt_Data4_buf[1];
			MLocat_CharPow_kw = Qt_Data4_buf[2];
			for(i=0;i<8;i++){
				MLocat_SelfChar_ena[i] = ((Qt_Data4_buf[3]>>i)&0x1);
				MLocat_SelfChar_ena[i+8] = ((Qt_Data4_buf[4]>>i)&0x1);
			}
			for(i=0;i<4;i++){
				MLocat_SelfChar_ena[i+16] = ((Qt_Data4_buf[5]>>i)&0x1);
			}		
	//		printf("MLocat_SelfChar_ena[0]=%d\n",MLocat_SelfChar_ena[0]);		
		}
		break;
		case 5:
		{
			for(i=0;i<dlc;i++)
			{
				Qt_Data5_buf[i] = Recv_Server_buf[current_head+3+i];
			}
			Qt_SendSev5Back_flg = 1;
			for(i=0;i<8;i++){
				MLocat_Conf_ena[i] = ((Qt_Data5_buf[3]>>i)&0x1);
				MLocat_Conf_ena[i+8] = ((Qt_Data5_buf[4]>>i)&0x1);

				MLocat_ErrReset_flg[i] = ((Qt_Data5_buf[0]>>i)&0x1);
				MLocat_ErrReset_flg[i+8] = ((Qt_Data5_buf[1]>>i)&0x1);
			}
			for(i=0;i<4;i++){
				MLocat_Conf_ena[i+16] = ((Qt_Data5_buf[5]>>i)&0x1);

				MLocat_ErrReset_flg[i+16] = ((Qt_Data5_buf[2]>>i)&0x1);
			}
			// EMS_CHU_ComErr_flg[0] = 0;
	//		printf("MLocat_ErrReset_flg%d\n",Qt_Data5_buf[0]);
		}
		break;
		case 6:
		{
			for(i=0;i<dlc;i++)
			{
				Qt_Data6_buf[i] = Recv_Server_buf[current_head+3+i];
			}
			Qt_SendSev6Back_flg = 1;
			uint8_t Test_Grab_carNo[30];
			strncpy(Test_Grab_carNo,Qt_Data6_buf,10);
			// if((Sta>=1&&Sta<=15)||
			// 	(Sys_Big_sta==3&&Sta<=32&&(strcmp(Cloud_Grab_carNo,Test_Grab_carNo)!=0))){
			if((Sta==15)||0){
				Grab_GetCarCard_flg = 1;
				strncpy(Cloud_Grab_carNo,Qt_Data6_buf,10);
				for(i=0;i<10;i++)
					Cloud_Grab_carNo2[i] = Qt_Data6_buf[i+10];
				Cloud_Grab_carNo2[i] = '\0';
				if(strcmp(Cloud_Grab_carNo,"")==0)
					strcpy(Cloud_Grab_carNo,Cloud_Grab_carNo2);
//				printf("Cloud_Grab_carNo111111111111=%s, %s\n",Cloud_Grab_carNo,Cloud_Grab_carNo2);
			}
		}
		break;
		case 0x0c:
		{
			for(i=0;i<dlc;i++)
			{
				Qt_Data12_buf[i] = Recv_Server_buf[current_head+3+i];
			}
			Qt_SendSev12Back_flg = 1;
			if(Qt_Data12_buf[0]){
				MCE_Order_err = 0;
				MCE_InitDealOrder_err = 0;
				MCE_ChangeBatOrder_err = 0;
				MCE_InLocatorOrder_err = 0;
				MCE_OutLocatorOrder_err = 0;
				MCE_BatOutOrder_err = 0;
				MCE_CELocatorOrder_err = 0;
				MCE_TimeOutOrder_err = 0;
				MCE_ContinueOrder_err = 0;
				MCE_StopOrder_err = 0;
				MCE_ErrResetOrder_err = 0;

				MCE_Initok_err = 0;
			}
			if(Qt_Data12_buf[1]){
				MQt_DisaSoundLigErr_flg = 1;
				Sound_Start_flg = 0;
				Light_Start_flg = 0;
			}
			else MQt_DisaSoundLigErr_flg = 0;
			if(Qt_Data12_buf[2]){
				MQt_GetInPlace_flg = 1;
				MQt_SendInPlace_flg = 0;	
			}
			if(Qt_Data12_buf[3]){
				MQt_GetHandCBOk_flg = 1;
	//			printf("MQt_GetHandCBOk_flg=%d\n",MQt_GetHandCBOk_flg);
			}
			if(Qt_Data12_buf[4]&&Sys_Big_sta>=3&&Sys_Big_sta<=4){
				MQt_GetCancelOrd_flg = 1;
	//			printf("MQt_GetCancelOrd_flg=%d\n",MQt_GetCancelOrd_flg);
			}	
		}
		break;
		case 0x0d:
		{
			MQt_GetPointBack_flg = 1;
	//		printf("MQt_GetPointBack_flg=%d\n",MQt_GetPointBack_flg);
		}
		break;
		case 0x0e:
		{
			for(i=0;i<3;i++)
			{
				Qt_Data14_buf[i] = Recv_Server_buf[current_head+3+i];
			}
			Cloud_SendODO_km = (Qt_Data14_buf[0]<<16)+(Qt_Data14_buf[1]<<8)+(Qt_Data14_buf[2]);
			MQt_GetODOValue_flg = 1;
			MCloud_CarOutReqODO_flg = 0;
			Qt_SendSev14Back_flg = 1;
	//		printf("Cloud_SendODO_km=%2f\n",Cloud_SendODO_km);
		}
		break;
		case 0x0f:
		{
			for(i=0;i<8;i++)
			{
				Qt_Data15_buf[i] = Recv_Server_buf[current_head+3+i];
			}
			if(Qt_Data15_buf[1]>=1){
				if(Qt_Data15_buf[0]==3){
				Qt_AirSetData_flg[Qt_Data15_buf[1]-1] = 0xff;
				AirSet_EnginStartTemp_C = (Qt_Data15_buf[3]-48);
				AirSet_HertStartTemp_C = (Qt_Data15_buf[2]-48);
				}	
				else if(Qt_Data15_buf[0]==1){
					Qt_AirOpen_flg[Qt_Data15_buf[1]-1] = 1;
				}
				else if(Qt_Data15_buf[0]==2){
					Qt_AirOpen_flg[Qt_Data15_buf[1]-1] = 2;
				}
			}	
			//set ac dev
			if(Qt_Data15_buf[0]==4)
			{
				Qt_ACEnergySetPT_flg = 1;
				Qt_ACEnergySetPT_tmp = (uint16_t)(Qt_Data15_buf[4]*256 + Qt_Data15_buf[5]);
			}
			else if(Qt_Data15_buf[0]==5)
			{
				Qt_ACEnergySetCT_flg = 1;
				Qt_ACEnergySetCT_tmp = (uint16_t)(Qt_Data15_buf[6]*256 + Qt_Data15_buf[7]);
			}
			Qt_SendSev15Back_flg = 1;
	//		printf("Qt_ACEnergySetPT_tmp432432534= %d %d %d %d\n",Qt_Data15_buf[0],Qt_Data15_buf[4],Qt_ACEnergySetCT_tmp,Qt_Data15_buf[7]);
		}
		break;
		case 0x11:
		{
			for(i=0;i<72;i++)
			{
				Qt_Data17_buf[i] = Recv_Server_buf[current_head+3+i];
			}
			for(i=0;i<24;i++)
			{
				QtBook_BatNax_num[0][i] = Qt_Data17_buf[i];
				QtBook_BatNax_num[1][i] = Qt_Data17_buf[i+24];
				QtBook_BatNax_num[2][i] = Qt_Data17_buf[i+48];
	//			printf("Cloud_SendODO_km=\n");
	//			printf(" %d ",QtBook_BatNax_num[0][i]);
	//			printf("Cloud_SendODO_km=\n");
			}
			mdl_send_AvailableNum_flg = 1;
			Qt_SendSev17Back_flg = 1;
			// printf("Cloud_SendODO_km=%2f\n",Cloud_SendODO_km);
		}
		break;
		case 0x12:
		{
			MQt_GetSql_flg = 1;
			for(i=0;i<100;i++)
			{
				Qt_Data18_buf[i] = Recv_Server_buf[current_head+3+i];
			}
			MCE_GetSqlInt_sta = Qt_Data18_buf[0];
			for(i=1;i<11;i++)
				Cloud_Grab_carNo[i-1] = Qt_Data18_buf[i];
			Cloud_carType_Code = Qt_Data18_buf[11];
			for(i=12;i<32+12;i++)
				Cloud_OutBatteryNo_now[i-12] = Qt_Data18_buf[i];
			//	printf("aaaaaCloud_orderId_now=%d\n",Cloud_BatteryNo_now[i-12]);
			for(i=44;i<44+24;i++)
				Cloud_orderId_now[i-44] = Qt_Data18_buf[i];
	//		printf("aaaaaCloud_orderId_now=%s,,%s\n",Cloud_orderId_now,Cloud_BatteryNo_now);

			MChar_SelfCe_sour = Qt_Data18_buf[68]-1;
			MLocat_AllowCESoc_pct = Qt_Data18_buf[69];
			MLocat_CharEndSoc_pct = Qt_Data18_buf[70];
			MLocat_CharPow_kw = Qt_Data18_buf[71];
			for(i=0;i<8;i++){
				MLocat_SelfChar_ena[i] = ((Qt_Data18_buf[72]>>i)&0x1);
				MLocat_SelfChar_ena[i+8] = ((Qt_Data18_buf[73]>>i)&0x1);
			}
			for(i=0;i<4;i++){
				MLocat_SelfChar_ena[i+16] = ((Qt_Data18_buf[74]>>i)&0x1);
			}	
			uint16_t FIFO_Data = 0;
			for(i=0;i<20;i++){
				FIFO_Data += (Qt_Data18_buf[i+75]);
			}	
			if(FIFO_Data==190)
			{
				for(i=0;i<20;i++){
					MLocat_FIFO_Buf[i] = (Qt_Data18_buf[i+75]);//
				}
			}	
			else
			{
				for(i=0;i<20;i++){
					MLocat_FIFO_Buf[i] = (i);//
				}
			}	
			Qt_SendSev18Back_flg = 1;
			int index = 0;
			for(index = 0;index<20;++index)
			{
				syslog(LOG_INFO,"MLocat_FIFO_Buf[%d] '%d'\n",index,MLocat_FIFO_Buf[index]);
	//			printf("%2x  ",MLocat_FIFO_Buf[index]);
			}

	//		printf("MQt_GetSql_flgMQt_GetSql_flgMQt_GetSql_flgMQt_GetSql_flg=%s\n\n\n\n",Cloud_Grab_carNo);
		}
		break;
		default:
			break;
	}
}
void Qt_Rec_Deal(int16_t Read_Size)
{

	uint16_t i=0;
	uint16_t rec_calchkval = 0;
    uint16_t my_calchkval = 0;
    uint16_t RecvData_dlc;
    uint16_t current_head = 0;
    static unsigned short ss;
	// QtSend_Discon_con = 0;
	for(i=0;i<Read_Size;i++)
	{
		Recv_Server_buf[i] = Qt_Recv_buf[i];
	}	
    do
    {
        if(Recv_Server_buf[current_head+0]==0xfe)
        {
            RecvData_dlc = Recv_Server_buf[current_head + 2];//当前帧的数据单元长度
//          printf("RecvData_dlc = %d\n",RecvData_dlc);
            rec_calchkval = (0x55 * 256 + 0x55);
            my_calchkval = (Recv_Server_buf[current_head+RecvData_dlc+3]*256+Recv_Server_buf[current_head+RecvData_dlc+4]);
            if(my_calchkval == rec_calchkval)
            {

				QtRecv_Server_Deal(current_head,RecvData_dlc);
                ss++;
                current_head = current_head + RecvData_dlc + 5;//头3+校验2
            }
            else
            {
                break;
				ss = 0;
            }
        }
    }while((current_head + 5 <= Read_Size)&&(Read_Size>0));	
	
	bzero(Qt_Recv_buf, sizeof(Qt_Recv_buf));
	bzero(Recv_Server_buf, sizeof(Recv_Server_buf));
}
/*QtSend充电数据上报*/
uint8_t QtSend_ChgData_buf1[500];
uint8_t QtSend_ChgData_buf2[500];
uint16_t QtSend_ChgData_buf1_len = 0;
uint16_t QtSend_ChgData_buf2_len = 0;
// uint8_t CHU_AllErr_flg[20];
// uint8_t Cloud_InNet_flg[20];
// uint8_t CHU_CharFull_flg[20];

/*0x87发送充电数据1-10*/
void QtInter_ChgData1_Deal()
{
	int i,j=0;
	int ChgingBag_sum = 0;
	QtSend_ChgData_buf1[0] = 0xfe;
	QtSend_ChgData_buf1[1] = 0x87;

	// EMS_Chger_BigSta[0] = 3;
	// EMS_Chger_BigSta[2] = 3;
	// BMS_PackSocReal_pct[0] = 54;
	// BMS_PackSocReal_pct[2] = 32;
	// BMS_ReqPackVol_V[0] = 550;
	// BMS_ReqPackVol_V[2] = 360;
	for(i=0;i<10;i++)
	{	
		if(EMS_Chger_BigSta[i] == 3)	
		// if(1)
		{
			ChgingBag_sum++;
			QtSend_ChgData_buf1[4+j++] = i+1;//库位编号1
			QtSend_ChgData_buf1[4+j++] = CHU_DevState_code[i];//BMS连接状态
			QtSend_ChgData_buf1[4+j++] = CHU_BattType[i];//电池类型					
			QtSend_ChgData_buf1[4+j++] = BMS_PackSocReal_pct[i];//当前SOC

			QtSend_ChgData_buf1[4+j++] = BMS_CharPackVolReal_V[i]*10/256;//电压输出值
			QtSend_ChgData_buf1[4+j++] = (int)BMS_CharPackVolReal_V[i]*10%256;//电压输出值
			QtSend_ChgData_buf1[4+j++] = BMS_CharPackCurReal_A[i]*10/256;//电流输出值
			QtSend_ChgData_buf1[4+j++] = (int)BMS_CharPackCurReal_A[i]*10%256;//电流输出值

			QtSend_ChgData_buf1[4+j++] = BMS_ReqPackVol_V[i]*10/256;//BMS需求电压
			QtSend_ChgData_buf1[4+j++] = (int)BMS_ReqPackVol_V[i]*10%256;//BMS需求电压
			QtSend_ChgData_buf1[4+j++] = BMS_ReqPackCur_A[i]*10/256;//BMS需求电流
			QtSend_ChgData_buf1[4+j++] = (int)BMS_ReqPackCur_A[i]*10%256;//BMS需求电流

			QtSend_ChgData_buf1[4+j++] = Charger_OutPwr_W[i]/256;//实际输出功率
			QtSend_ChgData_buf1[4+j++] = (int)Charger_OutPwr_W[i]%256;//实际输出功率
			QtSend_ChgData_buf1[4+j++] = FCH_BigState_Null[i];//充电阶段

			QtSend_ChgData_buf1[4+j++] = (EMS_CHU_LinkState[i]==1);//EMS与充电机连接状态	
			QtSend_ChgData_buf1[4+j++] = 0;//预留	
			QtSend_ChgData_buf1[4+j++] = 0;//预留	
			QtSend_ChgData_buf1[4+j++] = 0;//预留	
			QtSend_ChgData_buf1[4+j++] = 0;//预留			
		}
		else
		{
			continue;
		}
	}
	QtSend_ChgData_buf1[2] = (j+1);/*消息长度*/
	QtSend_ChgData_buf1[3] = ChgingBag_sum;/*充电包个数*/

	QtSend_ChgData_buf1[2+j+2] = 0x55;
	QtSend_ChgData_buf1[3+j+2] = 0x55;

	// syslog(LOG_INFO,"QtSend_ChgData_buf1 len == %d,ChgingBag_sum == %d \n",(QtSend_ChgData_buf1[2]),ChgingBag_sum);
}

/*0x88发送充电数据11-20*/
void QtInter_ChgData2_Deal()
{
	int i,j=0;
	int ChgingBag_sum = 0;
	QtSend_ChgData_buf2[0] = 0xfe;
	QtSend_ChgData_buf2[1] = 0x88;

	for(i=10;i<20;i++)
	{	
		if(EMS_Chger_BigSta[i] == 3)	
		// if(1)
		{
			ChgingBag_sum++;
			QtSend_ChgData_buf2[4+j++] = i+1;//库位编号1
			QtSend_ChgData_buf2[4+j++] = CHU_DevState_code[i];//BMS连接状态
			QtSend_ChgData_buf2[4+j++] = CHU_BattType[i];//电池类型					
			QtSend_ChgData_buf2[4+j++] = BMS_PackSocReal_pct[i];//当前SOC
			QtSend_ChgData_buf2[4+j++] = BMS_CharPackVolReal_V[i]*10/256;//电压输出值
			QtSend_ChgData_buf2[4+j++] = (int)BMS_CharPackVolReal_V[i]*10%256;//电压输出值
			QtSend_ChgData_buf2[4+j++] = BMS_CharPackCurReal_A[i]*10/256;//电流输出值
			QtSend_ChgData_buf2[4+j++] = (int)BMS_CharPackCurReal_A[i]*10%256;//电流输出值
			QtSend_ChgData_buf2[4+j++] = BMS_ReqPackVol_V[i]*10/256;//BMS需求电压
			QtSend_ChgData_buf2[4+j++] = (int)BMS_ReqPackVol_V[i]*10%256;//BMS需求电压
			QtSend_ChgData_buf2[4+j++] = BMS_ReqPackCur_A[i]*10/256;//BMS需求电流
			QtSend_ChgData_buf2[4+j++] = (int)BMS_ReqPackCur_A[i]*10%256;//BMS需求电流
			QtSend_ChgData_buf2[4+j++] = Charger_OutPwr_W[i]/256;//实际输出功率
			QtSend_ChgData_buf2[4+j++] = (int)Charger_OutPwr_W[i]%256;//实际输出功率
			QtSend_ChgData_buf2[4+j++] = FCH_BigState_Null[i];//充电阶段
			QtSend_ChgData_buf2[4+j++] = (EMS_CHU_LinkState[i]==1);//EMS与充电机连接状态
			QtSend_ChgData_buf2[4+j++] = 0;//预留	
			QtSend_ChgData_buf2[4+j++] = 0;//预留	
			QtSend_ChgData_buf2[4+j++] = 0;//预留	
			QtSend_ChgData_buf2[4+j++] = 0;//预留			
		}
		else
		{
			continue;
		}
	}
	QtSend_ChgData_buf2[2] = (j+1);/*消息长度*/
	QtSend_ChgData_buf2[3] = ChgingBag_sum;/*充电包个数*/
	QtSend_ChgData_buf2[3+QtSend_ChgData_buf2[2]] = 0x55;
	QtSend_ChgData_buf2[4+QtSend_ChgData_buf2[2]] = 0x55;
}

/*充电故障信息*/
uint8_t QtSend_ChgerErr_buf[300];
uint8_t QtSend_LocatData_buf[300];
uint8_t QtSend_AirData_buf[300];
uint16_t QtSend_ChgerErr_buf_len=0;
//0x89
void QtInter_ChgerErr_Deal()
{
	int i;
	QtSend_ChgerErr_buf[0] = 0xfe;
	QtSend_ChgerErr_buf[1] = 0x89;

	for(i=0;i<20;i++)
	{
		QtSend_ChgerErr_buf[3+i*8] = (i+1);/*库位号*/
		QtSend_ChgerErr_buf[4+i*8] = MChg_ErrStop_code[i];/*CHU1自停止故障编码*/
		QtSend_ChgerErr_buf[5+i*8] = CHU_ErrCode_buf0[i];/*ACDC1模块故障*/
		QtSend_ChgerErr_buf[6+i*8] = CHU_ErrCode_buf1[i];
		QtSend_ChgerErr_buf[7+i*8] = CHU_ErrCode_buf2[i];
		QtSend_ChgerErr_buf[8+i*8] = CHU_ErrCode_buf3[i];
		QtSend_ChgerErr_buf[9+i*8] = BMS_BATT_SOC[i];/*充电机通信故障*/
		QtSend_ChgerErr_buf[10+i*8] = 0;//Qt_InNet_err[i];/*预留*/

	}
	QtSend_ChgerErr_buf[2] = 8*20;/*消息长度*/
	QtSend_ChgerErr_buf[2+QtSend_ChgerErr_buf[2]+1] = 0x55;
	QtSend_ChgerErr_buf[2+QtSend_ChgerErr_buf[2]+2] = 0x55;

}
//0x8A
void QtInter_CEData_Deal()
{
	uint8_t i=0;
	uint8_t Data_CEData_buf[500];
	uint8_t SelfCe_err=0;
	if(MCE_Order_err==1)
	   SelfCe_err = 1;
	else if(MCE_InitDealOrder_err==1)
	   SelfCe_err = 2;
	else if(MCE_ChangeBatOrder_err==1)
	   SelfCe_err = 3;
	else if(MCE_InLocatorOrder_err==1)
	   SelfCe_err = 4;
	else if(MCE_OutLocatorOrder_err==1)
	   SelfCe_err = 5;
	else if(MCE_BatOutOrder_err==1)
	   SelfCe_err = 6;
	else if(MCE_CELocatorOrder_err==1)
	   SelfCe_err = 7;
	else if(MCE_TimeOutOrder_err==1)
	   SelfCe_err = 8;
	else if(MCE_ContinueOrder_err==1)
	   SelfCe_err = 9;
	else if(MCE_StopOrder_err==1)
	   SelfCe_err = 10;
	else if(MCE_ErrResetOrder_err==1)
	   SelfCe_err = 11;

	Data_CEData_buf[0] = CE_ErrData[0];
	Data_CEData_buf[1] = CE_ErrData[1];
	Data_CEData_buf[2] = (MCE_Comm_flg==1)+((MIO_Comm_flg==1)<<1)+((MCloud_Comm_flg==1)<<2)+
						((FireContr_conn_flg==1)<<3)+((AirCon_conn_flg==1)<<4)+((MGrab_Comm_flg==1)<<5)+((MLed_Comm_flg==1)<<6)+((0>1)<<7);	
	Data_CEData_buf[3] = (ACEnCon_conn_flg==1)+((AirCon_Eachconn_sta[0]==1)<<1)+((AirCon_Eachconn_sta[1]==1)<<2)+((AirCon_Eachconn_sta[2]==1)<<3);
	Data_CEData_buf[4] = SelfCe_err;
	Data_CEData_buf[5] = MIO_DoorErr_buf[0]+(MIO_DoorErr_buf[1]<<1)+(MIO_DoorErr_buf[2]<<2)+(MIO_Water_err<<3)+
							(MIO_ThunderErr_buf[0]<<4)+(MIO_ThunderErr_buf[1]<<5)+(MIO_LightrErr_buf[0]<<6)+(MIO_LightrErr_buf[1]<<7);
	Data_CEData_buf[6] = FireContr_HardErr_flg+(FireContr_GetFire_flg<<1);		//消防故障
	Data_CEData_buf[7] = 0;
	Data_CEData_buf[8] = 0;		//空调故障
	Data_CEData_buf[9] = 0;
	Data_CEData_buf[10] = 0;		//交流电表故障???
	Data_CEData_buf[11] = MCE_RunState;
	Data_CEData_buf[12] = MQt_StartSelfCE_sta;
	Data_CEData_buf[13] = Cloud_carType_Code;
	Data_CEData_buf[14] = MCE_RGVStepData;
	Data_CEData_buf[15] = MCE_StackerStepData;
	Data_CEData_buf[16] = MCE_ParkPlatStepData;
	Data_CEData_buf[17] = MCE_ControlMode;
	Data_CEData_buf[18] = MCE_RGVSenser_sta;
	Data_CEData_buf[19] = MCE_StackerSenser_sta;
	for(i=0;i<32;i++)
	{
		Data_CEData_buf[20+i] = Cloud_BatteryNo_now[i];
	}
	for(i=0;i<24;i++)
	{
		Data_CEData_buf[52+i] = Cloud_orderId_now[i];
	}
	Data_CEData_buf[76] = MChar_SelfCe_sour+1;
	Data_CEData_buf[77] = Sys_Big_sta;
	Data_CEData_buf[78] = MSound_Start_flg;//预留
	Data_CEData_buf[79] = MFan_Start_flg;//预留
	Data_CEData_buf[80] = MQt_SendInPlace_flg;//车辆到位确认需求信号
	Data_CEData_buf[81] = MCE_Initok_err;//换电设备初始位置异常
	Data_CEData_buf[82] = MCloud_CarOutReqODO_flg;//ODO里程需求
	// printf("SelfCe_err%d\n",SelfCe_err);

	for(i=0;i<SENDDATA_SIZE;i++)
	{
		QtSend_CEData_buf[3+i]=Data_CEData_buf[i];
	}
	QtSend_CEData_buf[2]=SENDDATA_SIZE;
	QtSend_CEData_buf[3+SENDDATA_SIZE]=0x55;
	QtSend_CEData_buf[3+SENDDATA_SIZE+1]=0x55;
}
//0x8B
void QtInter_LocatData_Deal()
{
	int i;
	QtSend_LocatData_buf[0] = 0xfe;
	QtSend_LocatData_buf[1] = 0x8B;
	for(i=0;i<50;i++){
		QtSend_LocatData_buf[i+3] = 0; 
	}
	// QtSend_LocatData_buf[48] = 0; 
	// QtSend_LocatData_buf[49] = 0; 
	// QtSend_LocatData_buf[50] = 0; 
	for(i=0;i<8;i++){		
		QtSend_LocatData_buf[3] |= (CHU_AllErr_flg[i]&&EMS_CHU_ComErr_flg[i]!=1)<<i;
		QtSend_LocatData_buf[3+1] |= (CHU_AllErr_flg[i+8]&&EMS_CHU_ComErr_flg[i+8]!=1)<<i;//库位故障状态

		QtSend_LocatData_buf[6] |= MLocat_InNet_flg[i]<<i;
		QtSend_LocatData_buf[6+1] |= MLocat_InNet_flg[i+8]<<i;//电池认证状态

		QtSend_LocatData_buf[9] |= MLocat_Conf_ena[i]<<i;
		QtSend_LocatData_buf[9+1] |= MLocat_Conf_ena[i+8]<<i;//库位使能状态

		QtSend_LocatData_buf[12] |= (CE_LocatorSenser[i]==2)<<i;
		QtSend_LocatData_buf[12+1] |= (CE_LocatorSenser[i+8]==2)<<i;//库位传感器状态
	
		QtSend_LocatData_buf[15] |= CHU_CharFull_flg[i]<<i;
		QtSend_LocatData_buf[15+1] |= CHU_CharFull_flg[i+8]<<i;//充电完成状态

		QtSend_LocatData_buf[18] |= (CHU_DevState_code[i]>2)<<i;
		QtSend_LocatData_buf[18+1] |= (CHU_DevState_code[i+8]>2)<<i;//电池连接状态

		QtSend_LocatData_buf[21] |= (EMS_CHU_LinkState[i]==1)<<i;
		QtSend_LocatData_buf[21+1] |= (EMS_CHU_LinkState[i+8]==1)<<i;//EMS与充电机连接状态
	}
	// printf("errrrr = %d\n",QtSend_LocatData_buf[3]);
	for(i=0;i<2;i++)
	{
		QtSend_LocatData_buf[3+2] |= (CHU_AllErr_flg[i+16]&&EMS_CHU_ComErr_flg[i+16]!=1)<<i;//库位故障状态
		QtSend_LocatData_buf[6+2] |= MLocat_InNet_flg[i+16]<<i;//电池认证状态
		QtSend_LocatData_buf[9+2] |= MLocat_Conf_ena[i+16]<<i;//电池认证状态
		QtSend_LocatData_buf[12+2] |= (CE_LocatorSenser[i+16]==2)<<i;//库位传感器状态
		QtSend_LocatData_buf[15+2] |= CHU_CharFull_flg[i+16]<<i;//充电完成状态
		QtSend_LocatData_buf[18+2] |= (CHU_DevState_code[i+16]>2)<<i;//电池连接状态
		QtSend_LocatData_buf[21+2] |= (EMS_CHU_LinkState[i+16]==1)<<i;//EMS与充电机连接状态
	}
	QtSend_LocatData_buf[24] = MCE_AllBag_num;
	for(i=0;i<20;i++)
	{
		QtSend_LocatData_buf[25+i] = CHU_DevState_code[i]; //电池包状态
	}
	QtSend_LocatData_buf[45] = LayOutNum.carType_number[0];			//EU300电池包个数
	QtSend_LocatData_buf[46] = LayOutNum.carType_number[1];			//EU5A电池包个数
	QtSend_LocatData_buf[47] = LayOutNum.carType_number[2];			//EU5B电池包个数
	for(i=0;i<8;i++){
		QtSend_LocatData_buf[48] |= (MLocat_BookLock_flg[i]<<i);
		QtSend_LocatData_buf[49] |= (MLocat_BookLock_flg[i+8]<<i);

		QtSend_LocatData_buf[51] |= (MLocat_SelfChar_ena[i]<<i);
		QtSend_LocatData_buf[52] |= (MLocat_SelfChar_ena[i+8]<<i);
	}
	QtSend_LocatData_buf[50] = (MLocat_BookLock_flg[16]+(MLocat_BookLock_flg[17]<<1));		//预约状态
	QtSend_LocatData_buf[53] = (MLocat_SelfChar_ena[16]+(MLocat_SelfChar_ena[17]<<1));		//充电机自动充电使能状态
	QtSend_LocatData_buf[2] = 51;/*消息长度*/
	QtSend_LocatData_buf[2+QtSend_LocatData_buf[2]+1] = 0x55;
	QtSend_LocatData_buf[2+QtSend_LocatData_buf[2]+2] = 0x55;
}
//0x90
void QtInter_AirData_Deal()
{
	int i,j=0;
	QtSend_AirData_buf[0] = 0xfe;
	QtSend_AirData_buf[1] = 0x90;

	for(i=0;i<3;i++)
	{
		QtSend_AirData_buf[3+i*6] = (i+1);/*空调编号*/
		if(AirCon_Eachconn_sta[i]==1){
			
			QtSend_AirData_buf[4+i*6] = AirCon_MerRunSta[i]+(AirCon_EnginRunSta[i]<<1)+(AirCon_HertRunSta[i]<<2);
			QtSend_AirData_buf[5+i*6] = (AirCon_Temp[i]+48);
			QtSend_AirData_buf[6+i*6] = (AirCon_Humi[i]);
			QtSend_AirData_buf[7+i*6] = (AirRead_HertStartTemp_C[i]+48);
			QtSend_AirData_buf[8+i*6] = (AirRead_EnginStartTemp_C[i]+48);
		}
		else{

			QtSend_AirData_buf[4+i*6] = 0;
			QtSend_AirData_buf[5+i*6] = (0+48);
			QtSend_AirData_buf[6+i*6] = (0);
			QtSend_AirData_buf[7+i*6] = (0+48);
			QtSend_AirData_buf[8+i*6] = (0+48);
		}
	}
	// 	for(i=21;i<24;i++)
	// 		QtSend_AirData_buf[i] = 0; //预留
		j=25;
		QtSend_AirData_buf[j++] = (uint8_t)(((uint32_t)(ac_GroupActiveTotalEnergy *100) >> 24) & 0xff);
		QtSend_AirData_buf[j++] = (uint8_t)(((uint32_t)(ac_GroupActiveTotalEnergy *100) >> 16) & 0xff);
		QtSend_AirData_buf[j++] = (uint8_t)(((uint32_t)(ac_GroupActiveTotalEnergy *100) >> 8) & 0xff);
		QtSend_AirData_buf[j++] = (uint8_t)(((uint32_t)(ac_GroupActiveTotalEnergy *100) >> 0) & 0xff);

		QtSend_AirData_buf[j++] = (uint8_t)(((uint32_t)(ac_PositiveActiveTotalEnergy *100) >> 24) & 0xff);
		QtSend_AirData_buf[j++] = (uint8_t)(((uint32_t)(ac_PositiveActiveTotalEnergy *100) >> 16) & 0xff);
		QtSend_AirData_buf[j++] = (uint8_t)(((uint32_t)(ac_PositiveActiveTotalEnergy *100) >> 8) & 0xff);
		QtSend_AirData_buf[j++] = (uint8_t)(((uint32_t)(ac_PositiveActiveTotalEnergy *100) >> 0) & 0xff);		

		QtSend_AirData_buf[j++] = (uint8_t)(((uint32_t)(ac_NegativeActiveTotalEnergy *100) >> 24) & 0xff);
		QtSend_AirData_buf[j++] = (uint8_t)(((uint32_t)(ac_NegativeActiveTotalEnergy *100) >> 16) & 0xff);
		QtSend_AirData_buf[j++] = (uint8_t)(((uint32_t)(ac_NegativeActiveTotalEnergy *100) >> 8) & 0xff);
		QtSend_AirData_buf[j++] = (uint8_t)(((uint32_t)(ac_NegativeActiveTotalEnergy *100) >> 0) & 0xff);

		QtSend_AirData_buf[j++] = (uint8_t)(((uint32_t)(ac_GroupReactiveTotalEnergy *100) >> 24) & 0xff);
		QtSend_AirData_buf[j++] = (uint8_t)(((uint32_t)(ac_GroupReactiveTotalEnergy *100) >> 16) & 0xff);
		QtSend_AirData_buf[j++] = (uint8_t)(((uint32_t)(ac_GroupReactiveTotalEnergy *100) >> 8) & 0xff);
		QtSend_AirData_buf[j++] = (uint8_t)(((uint32_t)(ac_GroupReactiveTotalEnergy *100) >> 0) & 0xff);

		QtSend_AirData_buf[j++] = (uint8_t)(((uint32_t)(ac_PositiveReactiveTotalEnergy *100) >> 24) & 0xff);
		QtSend_AirData_buf[j++] = (uint8_t)(((uint32_t)(ac_PositiveReactiveTotalEnergy *100) >> 16) & 0xff);
		QtSend_AirData_buf[j++] = (uint8_t)(((uint32_t)(ac_PositiveReactiveTotalEnergy *100) >> 8) & 0xff);
		QtSend_AirData_buf[j++] = (uint8_t)(((uint32_t)(ac_PositiveReactiveTotalEnergy *100) >> 0) & 0xff);	

		QtSend_AirData_buf[j++] = (uint8_t)(((uint32_t)(ac_NegativeReactiveTotalEnergy *100) >> 24) & 0xff);
		QtSend_AirData_buf[j++] = (uint8_t)(((uint32_t)(ac_NegativeReactiveTotalEnergy *100) >> 16) & 0xff);
		QtSend_AirData_buf[j++] = (uint8_t)(((uint32_t)(ac_NegativeReactiveTotalEnergy *100) >> 8) & 0xff);
		QtSend_AirData_buf[j++] = (uint8_t)(((uint32_t)(ac_NegativeReactiveTotalEnergy *100) >> 0) & 0xff);	

		QtSend_AirData_buf[j++] = (uint8_t)(((uint16_t)(acA_Voltage *10) >> 8) & 0xff);
		QtSend_AirData_buf[j++] = (uint8_t)(((uint16_t)(acA_Voltage *10) >> 0) & 0xff);

		QtSend_AirData_buf[j++] = (uint8_t)(((uint16_t)(acB_Voltage *10) >> 8) & 0xff);
		QtSend_AirData_buf[j++] = (uint8_t)(((uint16_t)(acB_Voltage *10) >> 0) & 0xff);

		QtSend_AirData_buf[j++] = (uint8_t)(((uint16_t)(acC_Voltage *10) >> 8) & 0xff);
		QtSend_AirData_buf[j++] = (uint8_t)(((uint16_t)(acC_Voltage *10) >> 0) & 0xff);		
					
		QtSend_AirData_buf[j++] = (uint8_t)(((uint16_t)(acA_Current *100) >> 8) & 0xff);
		QtSend_AirData_buf[j++] = (uint8_t)(((uint16_t)(acA_Current *100) >> 0) & 0xff);

		QtSend_AirData_buf[j++] = (uint8_t)(((uint16_t)(acB_Current *100) >> 8) & 0xff);
		QtSend_AirData_buf[j++] = (uint8_t)(((uint16_t)(acB_Current *100) >> 0) & 0xff);

		QtSend_AirData_buf[j++] = (uint8_t)(((uint16_t)(acC_Current *100) >> 8) & 0xff);
		QtSend_AirData_buf[j++] = (uint8_t)(((uint16_t)(acC_Current *100) >> 0) & 0xff);						

		QtSend_AirData_buf[j++] = (uint8_t)(((uint16_t)(acPt *100) >> 8) & 0xff);
		QtSend_AirData_buf[j++] = (uint8_t)(((uint16_t)(acPt *100) >> 0) & 0xff);

		QtSend_AirData_buf[j++] = (uint8_t)(((uint16_t)(acCt *100) >> 8) & 0xff);
		QtSend_AirData_buf[j++] = (uint8_t)(((uint16_t)(acCt *100) >> 0) & 0xff);			

		QtSend_AirData_buf[j++] = (uint8_t)(((uint32_t)(acA_ActivePower *1000) >> 24) & 0xff);
		QtSend_AirData_buf[j++] = (uint8_t)(((uint32_t)(acA_ActivePower *1000) >> 16) & 0xff);
		QtSend_AirData_buf[j++] = (uint8_t)(((uint32_t)(acA_ActivePower *1000) >> 8) & 0xff);
		QtSend_AirData_buf[j++] = (uint8_t)(((uint32_t)(acA_ActivePower *1000) >> 0) & 0xff);	

		QtSend_AirData_buf[j++] = (uint8_t)(((uint32_t)(acB_ActivePower *1000) >> 24) & 0xff);
		QtSend_AirData_buf[j++] = (uint8_t)(((uint32_t)(acB_ActivePower *1000) >> 16) & 0xff);
		QtSend_AirData_buf[j++] = (uint8_t)(((uint32_t)(acB_ActivePower *1000) >> 8) & 0xff);
		QtSend_AirData_buf[j++] = (uint8_t)(((uint32_t)(acB_ActivePower *1000) >> 0) & 0xff);

		QtSend_AirData_buf[j++] = (uint8_t)(((uint32_t)(acC_ActivePower *1000) >> 24) & 0xff);
		QtSend_AirData_buf[j++] = (uint8_t)(((uint32_t)(acC_ActivePower *1000) >> 16) & 0xff);
		QtSend_AirData_buf[j++] = (uint8_t)(((uint32_t)(acC_ActivePower *1000) >> 8) & 0xff);
		QtSend_AirData_buf[j++] = (uint8_t)(((uint32_t)(acC_ActivePower *1000) >> 0) & 0xff);

		QtSend_AirData_buf[j++] = (uint8_t)(((uint32_t)(acA_ReactivePower *1000) >> 24) & 0xff);
		QtSend_AirData_buf[j++] = (uint8_t)(((uint32_t)(acA_ReactivePower *1000) >> 16) & 0xff);
		QtSend_AirData_buf[j++] = (uint8_t)(((uint32_t)(acA_ReactivePower *1000) >> 8) & 0xff);
		QtSend_AirData_buf[j++] = (uint8_t)(((uint32_t)(acA_ReactivePower *1000) >> 0) & 0xff);

		QtSend_AirData_buf[j++] = (uint8_t)(((uint32_t)(acB_ReactivePower *1000) >> 24) & 0xff);
		QtSend_AirData_buf[j++] = (uint8_t)(((uint32_t)(acB_ReactivePower *1000) >> 16) & 0xff);
		QtSend_AirData_buf[j++] = (uint8_t)(((uint32_t)(acB_ReactivePower *1000) >> 8) & 0xff);
		QtSend_AirData_buf[j++] = (uint8_t)(((uint32_t)(acB_ReactivePower *1000) >> 0) & 0xff);

		QtSend_AirData_buf[j++] = (uint8_t)(((uint32_t)(acC_ReactivePower *1000) >> 24) & 0xff);
		QtSend_AirData_buf[j++] = (uint8_t)(((uint32_t)(acC_ReactivePower *1000) >> 16) & 0xff);
		QtSend_AirData_buf[j++] = (uint8_t)(((uint32_t)(acC_ReactivePower *1000) >> 8) & 0xff);
		QtSend_AirData_buf[j++] = (uint8_t)(((uint32_t)(acC_ReactivePower *1000) >> 0) & 0xff);	

		QtSend_AirData_buf[j++] = (uint8_t)(((uint16_t)(acA_PhasePowerFactor *1000) >> 8) & 0xff);
		QtSend_AirData_buf[j++] = (uint8_t)(((uint16_t)(acA_PhasePowerFactor *1000) >> 0) & 0xff);

		QtSend_AirData_buf[j++] = (uint8_t)(((uint16_t)(acB_PhasePowerFactor *1000) >> 8) & 0xff);
		QtSend_AirData_buf[j++] = (uint8_t)(((uint16_t)(acB_PhasePowerFactor *1000) >> 0) & 0xff);

		QtSend_AirData_buf[j++] = (uint8_t)(((uint16_t)(acC_PhasePowerFactor *1000) >> 8) & 0xff);
		QtSend_AirData_buf[j++] = (uint8_t)(((uint16_t)(acC_PhasePowerFactor *1000) >> 0) & 0xff);

		QtSend_AirData_buf[j++] = (uint8_t)(((uint16_t)(ac_Err) >> 8) & 0xff);
		QtSend_AirData_buf[j++] = (uint8_t)(((uint16_t)(ac_Err) >> 0) & 0xff);

	QtSend_AirData_buf[2] = j-3;/*消息长度*/
	// printf("QtSend_AirData_bufQtSend_AirData_buf %d %f %d %d\n",QtSend_AirData_buf[2],acA_Voltage,QtSend_AirData_buf[97],QtSend_AirData_buf[98]);

	// QtSend_AirData_buf[2] = 6*3;/*消息长度*/
	QtSend_AirData_buf[2+QtSend_AirData_buf[2]+1] = 0x55;
	QtSend_AirData_buf[2+QtSend_AirData_buf[2]+2] = 0x55;
}

/*直流电表信息*/
uint8_t QtSend_DcEne_buf[300];
uint8_t QtSend_DcEne2_buf[300];
uint16_t QtSend_DcEne_buf_len=0;
//0x93
void QtInter_DcEne_Deal()
{
	int i,j = 3;
	QtSend_DcEne_buf[0] = 0xfe;
	QtSend_DcEne_buf[1] = 0x93;

	for(i=0;i<10;i++)
	{
		QtSend_DcEne_buf[j++] = (i+1);/*库位号*/
		QtSend_DcEne_buf[j++] = (uint8_t)(((uint32_t)(DCEnVol_V[i] *1000) >> 24) & 0xff);
		QtSend_DcEne_buf[j++] = (uint8_t)(((uint32_t)(DCEnVol_V[i] *1000) >> 16) & 0xff);
		QtSend_DcEne_buf[j++] = (uint8_t)(((uint32_t)(DCEnVol_V[i] *1000) >> 8) & 0xff);
		QtSend_DcEne_buf[j++] = (uint8_t)(((uint32_t)(DCEnVol_V[i] *1000) >> 0) & 0xff);

		QtSend_DcEne_buf[j++] = (uint8_t)(((uint32_t)(DCEnCur_A[i] *1000) >> 24) & 0xff);
		QtSend_DcEne_buf[j++] = (uint8_t)(((uint32_t)(DCEnCur_A[i] *1000) >> 16) & 0xff);
		QtSend_DcEne_buf[j++] = (uint8_t)(((uint32_t)(DCEnCur_A[i] *1000) >> 8) & 0xff);
		QtSend_DcEne_buf[j++] = (uint8_t)(((uint32_t)(DCEnCur_A[i] *1000) >> 0) & 0xff);	

		QtSend_DcEne_buf[j++] = (uint8_t)(((uint32_t)(DCEn_Act_W[i] *1000) >> 24) & 0xff);
		QtSend_DcEne_buf[j++] = (uint8_t)(((uint32_t)(DCEn_Act_W[i] *1000) >> 16) & 0xff);
		QtSend_DcEne_buf[j++] = (uint8_t)(((uint32_t)(DCEn_Act_W[i] *1000) >> 8) & 0xff);
		QtSend_DcEne_buf[j++] = (uint8_t)(((uint32_t)(DCEn_Act_W[i] *1000) >> 0) & 0xff);

		QtSend_DcEne_buf[j++] = (uint8_t)(((uint32_t)(DCEnPos_Act_W[i] *1000) >> 24) & 0xff);
		QtSend_DcEne_buf[j++] = (uint8_t)(((uint32_t)(DCEnPos_Act_W[i] *1000) >> 16) & 0xff);
		QtSend_DcEne_buf[j++] = (uint8_t)(((uint32_t)(DCEnPos_Act_W[i] *1000) >> 8) & 0xff);
		QtSend_DcEne_buf[j++] = (uint8_t)(((uint32_t)(DCEnPos_Act_W[i] *1000) >> 0) & 0xff);
		QtSend_DcEne_buf[j++] = (uint8_t)(((uint32_t)(DCEnNeg_Act_W[i] *1000) >> 24) & 0xff);
		QtSend_DcEne_buf[j++] = (uint8_t)(((uint32_t)(DCEnNeg_Act_W[i] *1000) >> 16) & 0xff);
		QtSend_DcEne_buf[j++] = (uint8_t)(((uint32_t)(DCEnNeg_Act_W[i] *1000) >> 8) & 0xff);
		QtSend_DcEne_buf[j++] = (uint8_t)(((uint32_t)(DCEnNeg_Act_W[i] *1000) >> 0) & 0xff);
	}
	QtSend_DcEne_buf[2] = j-3;/*消息长度*/
	QtSend_DcEne_buf[2+QtSend_DcEne_buf[2]+1] = 0x55;
	QtSend_DcEne_buf[2+QtSend_DcEne_buf[2]+2] = 0x55;

}
//0x94
void QtInter_DcEne2_Deal()
{
	int i,j = 3;
	QtSend_DcEne2_buf[0] = 0xfe;
	QtSend_DcEne2_buf[1] = 0x94;

	for(i=10;i<20;i++)
	{
		QtSend_DcEne2_buf[j++] = (i+1);/*库位号*/
		QtSend_DcEne2_buf[j++] = (uint8_t)(((uint32_t)(DCEnVol_V[i] *1000) >> 24) & 0xff);
		QtSend_DcEne2_buf[j++] = (uint8_t)(((uint32_t)(DCEnVol_V[i] *1000) >> 16) & 0xff);
		QtSend_DcEne2_buf[j++] = (uint8_t)(((uint32_t)(DCEnVol_V[i] *1000) >> 8) & 0xff);
		QtSend_DcEne2_buf[j++] = (uint8_t)(((uint32_t)(DCEnVol_V[i] *1000) >> 0) & 0xff);

		QtSend_DcEne2_buf[j++] = (uint8_t)(((uint32_t)(DCEnCur_A[i] *1000) >> 24) & 0xff);
		QtSend_DcEne2_buf[j++] = (uint8_t)(((uint32_t)(DCEnCur_A[i] *1000) >> 16) & 0xff);
		QtSend_DcEne2_buf[j++] = (uint8_t)(((uint32_t)(DCEnCur_A[i] *1000) >> 8) & 0xff);
		QtSend_DcEne2_buf[j++] = (uint8_t)(((uint32_t)(DCEnCur_A[i] *1000) >> 0) & 0xff);	

		QtSend_DcEne2_buf[j++] = (uint8_t)(((uint32_t)(DCEn_Act_W[i] *1000) >> 24) & 0xff);
		QtSend_DcEne2_buf[j++] = (uint8_t)(((uint32_t)(DCEn_Act_W[i] *1000) >> 16) & 0xff);
		QtSend_DcEne2_buf[j++] = (uint8_t)(((uint32_t)(DCEn_Act_W[i] *1000) >> 8) & 0xff);
		QtSend_DcEne2_buf[j++] = (uint8_t)(((uint32_t)(DCEn_Act_W[i] *1000) >> 0) & 0xff);

		QtSend_DcEne2_buf[j++] = (uint8_t)(((uint32_t)(DCEnPos_Act_W[i] *1000) >> 24) & 0xff);
		QtSend_DcEne2_buf[j++] = (uint8_t)(((uint32_t)(DCEnPos_Act_W[i] *1000) >> 16) & 0xff);
		QtSend_DcEne2_buf[j++] = (uint8_t)(((uint32_t)(DCEnPos_Act_W[i] *1000) >> 8) & 0xff);
		QtSend_DcEne2_buf[j++] = (uint8_t)(((uint32_t)(DCEnPos_Act_W[i] *1000) >> 0) & 0xff);
		QtSend_DcEne2_buf[j++] = (uint8_t)(((uint32_t)(DCEnNeg_Act_W[i] *1000) >> 24) & 0xff);
		QtSend_DcEne2_buf[j++] = (uint8_t)(((uint32_t)(DCEnNeg_Act_W[i] *1000) >> 16) & 0xff);
		QtSend_DcEne2_buf[j++] = (uint8_t)(((uint32_t)(DCEnNeg_Act_W[i] *1000) >> 8) & 0xff);
		QtSend_DcEne2_buf[j++] = (uint8_t)(((uint32_t)(DCEnNeg_Act_W[i] *1000) >> 0) & 0xff);
	}
	QtSend_DcEne2_buf[2] = j-3;/*消息长度*/
	QtSend_DcEne2_buf[2+QtSend_DcEne2_buf[2]+1] = 0x55;
	QtSend_DcEne2_buf[2+QtSend_DcEne2_buf[2]+2] = 0x55;

}

uint8_t Qt_CarAllowIn_LiftBar_sta;
void mid_qtTcp_Interface()
{
	int i = 0;

	if(mid_qtTcp.QtEMS_ShutDown_flg){
		QtEMS_ShutDown_flg = mid_qtTcp.QtEMS_ShutDown_flg;
		mid_qtTcp.QtEMS_ShutDown_flg = 0;
	}
	if(mid_qtTcp.QT_InitOrder_flg){
		QT_InitOrder_flg = mid_qtTcp.QT_InitOrder_flg;
		mid_qtTcp.QT_InitOrder_flg = 0;
	}
	if(mid_qtTcp.QT_ChangeBatOrder_flg){
		QT_ChangeBatOrder_flg = mid_qtTcp.QT_ChangeBatOrder_flg;
		MCE_CBLocator_sour = mid_qtTcp.MCE_CBLocator_sour;
		MCE_CBLocator_CarTy = mid_qtTcp.MCE_CBLocator_CarTy;
		mid_qtTcp.QT_ChangeBatOrder_flg = 0;
		mid_qtTcp.MCE_CBLocator_sour = 0;
		mid_qtTcp.MCE_CBLocator_CarTy = 0;
	}
	if(mid_qtTcp.QT_CELocatorOrder_flg){
		QT_CELocatorOrder_flg = mid_qtTcp.QT_CELocatorOrder_flg;
		MCE_CELocator_sour = mid_qtTcp.MCE_CELocator_sour;
		MCE_CELocator_dest = mid_qtTcp.MCE_CELocator_dest;
		MCE_CELocator_CarTy = mid_qtTcp.MCE_CELocator_CarTy;
		mid_qtTcp.QT_CELocatorOrder_flg = 0;
		mid_qtTcp.MCE_CELocator_sour = 0;
		mid_qtTcp.MCE_CELocator_dest = 0;
		mid_qtTcp.MCE_CELocator_CarTy = 0;
	}
	if(mid_qtTcp.QT_BatOutOrder_flg){
		QT_BatOutOrder_flg = mid_qtTcp.QT_BatOutOrder_flg;
		MCE_BatOut_sour = mid_qtTcp.MCE_BatOut_sour;
		MCE_BatOut_CarTy = mid_qtTcp.MCE_BatOut_CarTy;
		mid_qtTcp.QT_BatOutOrder_flg = 0;
		mid_qtTcp.MCE_BatOut_sour = 0;
		mid_qtTcp.MCE_BatOut_CarTy = 0;
	}
	if(mid_qtTcp.QT_InLocatorOrder_flg){
		QT_InLocatorOrder_flg = mid_qtTcp.QT_InLocatorOrder_flg;
		MCE_InLocator_dest = mid_qtTcp.MCE_InLocator_dest;
		MCE_InLocator_CarTy = mid_qtTcp.MCE_InLocator_CarTy;
		mid_qtTcp.QT_InLocatorOrder_flg = 0;
		mid_qtTcp.MCE_InLocator_dest = 0;
		mid_qtTcp.MCE_InLocator_CarTy = 0;
	}
	if(mid_qtTcp.QT_OutLocatorOrder_flg){
		QT_OutLocatorOrder_flg = mid_qtTcp.QT_OutLocatorOrder_flg;
		MCE_OutLocator_sour = mid_qtTcp.MCE_OutLocator_sour;
		MCE_OutLocator_CarTy = mid_qtTcp.MCE_OutLocator_CarTy;
		mid_qtTcp.QT_OutLocatorOrder_flg = 0;
		mid_qtTcp.MCE_OutLocator_sour = 0;
		mid_qtTcp.MCE_OutLocator_CarTy = 0;
	}
	if(mid_qtTcp.QT_TimeOutOrder_flg){
		QT_TimeOutOrder_flg = mid_qtTcp.QT_TimeOutOrder_flg;
		mid_qtTcp.QT_TimeOutOrder_flg = 0;
	}
	if(mid_qtTcp.QT_ContinueOrder_flg){
		QT_ContinueOrder_flg = mid_qtTcp.QT_ContinueOrder_flg;
		mid_qtTcp.QT_ContinueOrder_flg = 0;
	}
	if(mid_qtTcp.QT_StopOrder_flg){
		QT_StopOrder_flg = mid_qtTcp.QT_StopOrder_flg;
		mid_qtTcp.QT_StopOrder_flg = 0;
	}
	if(mid_qtTcp.QT_ErrResetOrder_flg){
		QT_ErrResetOrder_flg = mid_qtTcp.QT_ErrResetOrder_flg;
		mid_qtTcp.QT_ErrResetOrder_flg = 0;
	}
	for(i = 0;i<20;i++)
	{
		if(mid_qtTcp.Hand_LowPwrOn_flg[i]){
			Hand_LowPwrOn_flg[i] = mid_qtTcp.Hand_LowPwrOn_flg[i];
			mid_qtTcp.Hand_LowPwrOn_flg[i] = 0;
		}
		if(mid_qtTcp.Hand_StartChg_flg[i]){
			Hand_StartChg_flg[i] = mid_qtTcp.Hand_StartChg_flg[i];
			mid_qtTcp.Hand_StartChg_flg[i] = 0;Qt_InNet_err[i] = 0;
		}
		if(mid_qtTcp.Hand_StopChg_flg[i]){
			Hand_StopChg_flg[i] = mid_qtTcp.Hand_StopChg_flg[i];
			mid_qtTcp.Hand_StopChg_flg[i] = 0;
		}
		if(mid_qtTcp.Hand_LowPwrDown_flg[i]){
			Hand_LowPwrDown_flg[i] = mid_qtTcp.Hand_LowPwrDown_flg[i];
			mid_qtTcp.Hand_LowPwrDown_flg[i] = 0;
		}

		MLocat_SelfChar_ena[i] = mid_qtTcp.MLocat_SelfChar_ena[i];
		// MLocat_Conf_ena[i] = mid_qtTcp.MLocat_Conf_ena[i];
		// MLocat_ErrReset_flg[i] = mid_qtTcp.MLocat_ErrReset_flg[i];
	}

	MLocat_AllowCESoc_pct = mid_qtTcp.MLocat_AllowCESoc_pct;
	MLocat_CharEndSoc_pct = mid_qtTcp.MLocat_CharEndSoc_pct;
	MLocat_CharPow_kw = mid_qtTcp.MLocat_CharPow_kw;
	Cloud_SendODO_km = mid_qtTcp.Cloud_SendODO_km;

	if(mid_qtTcp.MQt_ConfDa_push){
		MQt_ConfDa_push = mid_qtTcp.MQt_ConfDa_push;
		mid_qtTcp.MQt_ConfDa_push = 0;
	}
	if(mid_qtTcp.Grab_GetCarCard_flg){
		Grab_GetCarCard_flg = mid_qtTcp.Grab_GetCarCard_flg;
		mid_qtTcp.Grab_GetCarCard_flg = 0;
	}
	// if(mid_qtTcp.MQt_DisaSoundLigErr_flg){
		MQt_DisaSoundLigErr_flg = mid_qtTcp.MQt_DisaSoundLigErr_flg;
	// 	mid_qtTcp.MQt_DisaSoundLigErr_flg = 0;
	// }
	if(mid_qtTcp.MQt_GetInPlace_flg){
		MQt_GetInPlace_flg = mid_qtTcp.MQt_GetInPlace_flg;
		mid_qtTcp.MQt_GetInPlace_flg = 0;
	}
	if(mid_qtTcp.MQt_GetHandCBOk_flg){
		MQt_GetHandCBOk_flg = mid_qtTcp.MQt_GetHandCBOk_flg;
		mid_qtTcp.MQt_GetHandCBOk_flg = 0;
	}
	if(mid_qtTcp.MQt_GetCancelOrd_flg){
		MQt_GetCancelOrd_flg = mid_qtTcp.MQt_GetCancelOrd_flg;
		mid_qtTcp.MQt_GetCancelOrd_flg = 0;
	}
	if(mid_qtTcp.MQt_GetPointBack_flg){
		MQt_GetPointBack_flg = mid_qtTcp.MQt_GetPointBack_flg;
		mid_qtTcp.MQt_GetPointBack_flg = 0;
	}
	if(mid_qtTcp.MQt_GetODOValue_flg){
		MQt_GetODOValue_flg = mid_qtTcp.MQt_GetODOValue_flg;
		mid_qtTcp.MQt_GetODOValue_flg = 0;
	}
	AirSet_EnginStartTemp_C = mid_qtTcp.AirSet_EnginStartTemp_C;
	AirSet_HertStartTemp_C = mid_qtTcp.AirSet_HertStartTemp_C;
	for(i = 0;i<3;i++)
	{
		if(mid_qtTcp.Qt_AirSetData_flg[i]){
			Qt_AirSetData_flg[i] = mid_qtTcp.Qt_AirSetData_flg[i];
			mid_qtTcp.Qt_AirSetData_flg[i] = 0;
		}
		if(mid_qtTcp.Qt_AirOpen_flg[i]){
			Qt_AirOpen_flg[i] = mid_qtTcp.Qt_AirOpen_flg[i];
			mid_qtTcp.Qt_AirOpen_flg[i] = 0;
		}
	}
	if(mid_qtTcp.mdl_send_AvailableNum_flg){
		mdl_send_AvailableNum_flg = mid_qtTcp.mdl_send_AvailableNum_flg;
		mid_qtTcp.mdl_send_AvailableNum_flg = 0;
	}
}
void thread_Qt(void)
{
//	printf("thread_Qt tid %d\n",gettid());
//	syslog(LOG_INFO,"thread_Qt gettid '%d'\n",gettid());
	//定义sockfd
    int clientsockfd = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in servaddr;
	
	int16_t conn1 = -1;
	int16_t Send_flg;

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);  ///服务器端口
    servaddr.sin_addr.s_addr = inet_addr("192.168.1.2");  ///服务器ip
//	servaddr.sin_addr.s_addr = inet_addr("192.168.1.254"); 

	//close(clientsockfd);
	 ///连接服务器成功返回0错误返回-1
	 conn1 = connect(clientsockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
	while(conn1<0)
	{
		close(clientsockfd);
		sleep(1);
		clientsockfd = socket(AF_INET, SOCK_STREAM, 0);
		int flags = fcntl(clientsockfd, F_GETFL, 0);  
		fcntl(clientsockfd, F_SETFL, flags|O_NONBLOCK);
		// fcntl(clientsockfd, F_SETFL, flags&~O_NONBLOCK);
		conn1 = connect(clientsockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
		sleep(2);
		// if(recv(clientsockfd, Qt_Recv_buf, BUFFER_SIZE,MSG_DONTWAIT|MSG_NOSIGNAL)>=0)
		// {
		// 	conn1 = 1;
		// }
		conn1 = send(clientsockfd, Send_ReConnect_buf, (1),MSG_DONTWAIT|MSG_NOSIGNAL);
//		printf("connect qt function failed.\n");
		Qt_conn_flg = 2;
		//sleep(10);
	}
	Qt_conn_flg = 1;
	printf("connect Qt OK.\n");

	while(1)
	{
		int i = 0;
		if(Qt_CarAllowIn_LiftBar_sta==1)     //2021-3-24 抬杆控制标志
		{
	//		printf("Qt_CarAllowIn_LiftBar_staQt_CarAllowIn_LiftBar_sta Qt OK.\n");
			Qt_CarAllowIn_LiftBar_sta =0;
			MQt_SendPoint_flg =1;
			QtSend_Sev13_buf[6]=1;
		}
		else
		{
			QtSend_Sev13_buf[6]=0;
		}
		
		// mid_qtTcp_Interface();
		// EMS_Chger_BigSta[0] = 3;
		// EMS_Chger_BigSta[2] = 3;
		// ChangeElec();
		if(Qt_SendSev2Back_flg==1){
			Qt_SendSev2Back_flg = 0;
			send(clientsockfd, QtSend_Sev2Back_buf, (6),MSG_DONTWAIT|MSG_NOSIGNAL);
		}
		if(Qt_SendSev3Back_flg==1){
			Qt_SendSev3Back_flg = 0;
			send(clientsockfd, QtSend_Sev3Back_buf, (6),MSG_DONTWAIT|MSG_NOSIGNAL);
		}
		if(Qt_SendSev4Back_flg==1){
			Qt_SendSev4Back_flg = 0;
			send(clientsockfd, QtSend_Sev4Back_buf, (6),MSG_DONTWAIT|MSG_NOSIGNAL);
		}
		if(Qt_SendSev5Back_flg==1){
			Qt_SendSev5Back_flg = 0;
			send(clientsockfd, QtSend_Sev5Back_buf, (6),MSG_DONTWAIT|MSG_NOSIGNAL);
		}
		if(Qt_SendSev6Back_flg==1){
			Qt_SendSev6Back_flg = 0;
			send(clientsockfd, QtSend_Sev6Back_buf, (6),MSG_DONTWAIT|MSG_NOSIGNAL);
		}
		if(Qt_SendSev12Back_flg==1){
			Qt_SendSev12Back_flg = 0;
			send(clientsockfd, QtSend_Sev12Back_buf, (6),MSG_DONTWAIT|MSG_NOSIGNAL);
		}
		if(MQt_SendPoint_flg==1||Qt_CarAllowIn_err>0||Qt_CarAllowOut_err>0||\
			Qt_CarODO_err>0||Qt_CancelOrd_err>0||ChecklostBat_err_flg>0||PLC_Die_flg>0){
			MQt_SendPoint_flg = 0;
//			printf("inneterrrrrrrreeeerrrr %d %d %d\n",Qt_CarAllowIn_err,Qt_CarAllowOut_err,Qt_CancelOrd_err);
			syslog(LOG_INFO,"Qt_CarAllowIn_err Qt_CarAllowOut_err Qt_CancelOrd_err %d %d %d\n",Qt_CarAllowIn_err,Qt_CarAllowOut_err,Qt_CancelOrd_err);
			syslog(LOG_INFO,"ChecklostBat_err_flg PLC_Die_flg %d %d\n",ChecklostBat_err_flg,PLC_Die_flg);
			if(MQt_NoBag_flg) QtSend_Sev13_buf[3]=2;
			else if(MQt_ClearInPlace_flg) QtSend_Sev13_buf[3]=16;
			else if(Qt_CarAllowIn_err>=2){
				QtSend_Sev13_buf[3]=1;
				QtSend_Sev13_buf[4]=Qt_CarAllowIn_err;
				Qt_CarAllowIn_err = 0;
			} 
			else if(Qt_CarAllowOut_err>=2){
				QtSend_Sev13_buf[3]=32;
				QtSend_Sev13_buf[4]=Qt_CarAllowOut_err;
				Qt_CarAllowOut_err = 0;
			} 
			else if(Qt_CarODO_err>=2){
				QtSend_Sev13_buf[3]=64;
				QtSend_Sev13_buf[4]=Qt_CarODO_err;
				Qt_CarODO_err = 0;
			} 
			else if(Qt_CancelOrd_err>=2){
				QtSend_Sev13_buf[3]=128;
				QtSend_Sev13_buf[4]=Qt_CancelOrd_err;
				Qt_CancelOrd_err = 0;
			} 
			else if(MRemo_GetSelfCE_flg) {
				MRemo_GetSelfCE_flg = 0;
				QtSend_Sev13_buf[3] = MQt_StartSelfCE_sta*4;
			}
			else QtSend_Sev13_buf[3]=0;

			if(ChecklostBat_err_flg) {
				ChecklostBat_err_flg = 0;
				QtSend_Sev13_buf[5] = 1;
			}
			if(PLC_Die_flg) {
				PLC_Die_flg = 0;
				QtSend_Sev13_buf[5] += 2;
			}

			send(clientsockfd, QtSend_Sev13_buf, (9),MSG_DONTWAIT|MSG_NOSIGNAL);
			QtSend_Sev13_buf[5] = 0;
			QtSend_Sev13_buf[6]=0;
			// int i =0;
			// for(i = 0;i<7;++i)
			// 	printf("%2x  ",QtSend_Sev13_buf[i]);
			
			// printf("\n\n\n");

		}
		if(Qt_SendSev14Back_flg==1){
			Qt_SendSev14Back_flg = 0;
			send(clientsockfd, QtSend_Sev14Back_buf, (6),MSG_DONTWAIT|MSG_NOSIGNAL);
		}
		if(Qt_SendSev15Back_flg==1){
			Qt_SendSev15Back_flg = 0;
			send(clientsockfd, QtSend_Sev15Back_buf, (6),MSG_DONTWAIT|MSG_NOSIGNAL);
		}
		if(Qt_SendSev17Back_flg==1){
			Qt_SendSev17Back_flg = 0;
			send(clientsockfd, QtSend_Sev17Back_buf, (6),MSG_DONTWAIT|MSG_NOSIGNAL);
		}
		if(Qt_SendSev18Back_flg==1){
			Qt_SendSev18Back_flg = 0;
			send(clientsockfd, QtSend_Sev18Back_buf, (6),MSG_DONTWAIT|MSG_NOSIGNAL);
		}
		// if(0){
		if(MQt_SendChgerErr_flg==1){
			MQt_SendChgerErr_flg = 0;
			QtInter_ChgerErr_Deal();
			send(clientsockfd, QtSend_ChgerErr_buf, (QtSend_ChgerErr_buf[2]+5),MSG_DONTWAIT|MSG_NOSIGNAL);
		}
		if(MQt_SendChgData1_flg==1){
			MQt_SendChgData1_flg = 0;
			QtInter_ChgData1_Deal();
			send(clientsockfd, QtSend_ChgData_buf1, (QtSend_ChgData_buf1[2]+5),MSG_DONTWAIT|MSG_NOSIGNAL);
		}
		if(MQt_SendChgData2_flg==1){
			MQt_SendChgData2_flg = 0;
			QtInter_ChgData2_Deal();
			send(clientsockfd, QtSend_ChgData_buf2, (QtSend_ChgData_buf2[2]+5),MSG_DONTWAIT|MSG_NOSIGNAL);
		}
		// if(0){
		if(MQt_SendCEData_flg==1){
			// printf("MQt_SendCEData_flg\n");
			MQt_SendCEData_flg = 0;			
			QtInter_CEData_Deal(1);
			Send_flg = send(clientsockfd, QtSend_CEData_buf, (QtSend_CEData_buf[2]+5),MSG_DONTWAIT|MSG_NOSIGNAL); //发送充放电指令-500ms
		}	
		if(MQt_SendLocatData_flg==1){
			// printf("aaaaaaaaaa\n");
			MQt_SendLocatData_flg = 0;
			QtInter_LocatData_Deal();
			send(clientsockfd, QtSend_LocatData_buf, (QtSend_LocatData_buf[2]+5),MSG_DONTWAIT|MSG_NOSIGNAL);
		}
		if(MQt_SendAirData_flg==1){
			// printf("aaaaaaaaaa\n");
			MQt_SendAirData_flg = 0;
			QtInter_AirData_Deal();
			// for(i=0;i<22;i++)
			//   printf("%x ",QtSend_AirData_buf[i]);
			send(clientsockfd, QtSend_AirData_buf, (QtSend_AirData_buf[2]+5),MSG_DONTWAIT|MSG_NOSIGNAL);
		}
		// if(MQt_SendDcEne_flg==1){
		// 	// printf("aaaaaaaaaa\n");
		// 	MQt_SendDcEne_flg = 0;
		// 	QtInter_DcEne_Deal();
		// 	send(clientsockfd, QtSend_DcEne_buf, (QtSend_DcEne_buf[2]+5),MSG_DONTWAIT|MSG_NOSIGNAL);
		// }
		// if(MQt_SendDcEne2_flg==1){
		// 	// printf("aaaaaaaaaa\n");
		// 	MQt_SendDcEne2_flg = 0;
		// 	QtInter_DcEne2_Deal();
		// 	send(clientsockfd, QtSend_DcEne2_buf, (QtSend_DcEne2_buf[2]+5),MSG_DONTWAIT|MSG_NOSIGNAL);
		// }
		if(QtSend_Discon_con>=15)
		{			
			Qt_conn_flg = 2;
			Send_flg = -1;
			// conn1 = connect(clientsockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
			while(Send_flg<0)
			{
				syslog(LOG_INFO,"connect qt function failed Send_flg,QtSend_Discon_con %d %d\n",Send_flg,QtSend_Discon_con);
				close(clientsockfd);
				sleep(1);
				clientsockfd = socket(AF_INET, SOCK_STREAM, 0);
				int flags = fcntl(clientsockfd, F_GETFL, 0);  
				fcntl(clientsockfd, F_SETFL, flags|O_NONBLOCK);
				conn1 = connect(clientsockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
				sleep(2);
				Send_flg = send(clientsockfd, Send_ReConnect_buf, (1),MSG_DONTWAIT|MSG_NOSIGNAL);

				fprintf(stderr, "connect qt function failed.\n");
				Qt_conn_flg = 2;
				//sleep(10);
				// printf("Send_flg22222%d = ",Send_flg);
			}	
			QtSend_Discon_con = 0;	
		}
		else
		{
			Qt_conn_flg = 1;
		}		
		Read_Size1=(recv(clientsockfd, Qt_Recv_buf, BUFFER_SIZE,MSG_DONTWAIT|MSG_NOSIGNAL));
		if(Read_Size1<=0){
		// if((recv(clientsockfd, Qt_Recv_buf, BUFFER_SIZE,MSG_DONTWAIT|MSG_NOSIGNAL))<0){
				}	
		else{
		    // printf("Read_Size1 %d\n",Read_Size1);
			Read_Size=Read_Size1;
			Qt_Rec_Deal(Read_Size1);
			QtSend_Discon_con = 0;
		}
		usleep(10*1000);//10ms
	}
	close(clientsockfd);
}


