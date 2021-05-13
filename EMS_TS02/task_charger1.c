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

#include "SA4_EMS.h"
#include "mymain.h"

#define CHU_COM_PORT              (5000)

#define CHU1_SEVER_IP		("192.168.1.21")

uint8_t MLocat_BatteryNo[20][100];
uint8_t EMS_CHU_LinkState[20];
uint16_t CHUSend_Discon_cou[20];
int CHU_clientsockfd[20];

//0X83
uint8_t CHU_DevState_code[20];
uint8_t CHU_BMS_LinkState[20];
uint8_t CHU_BattType[20];
// uint8_t CHU_BattType[20] = {1,1,1,0,1,1,1,1,2,2,2,2,2,2,2,2,2,2};
double CHU_NegTemp_C[20];
double CHU_PosTemp_C[20];

uint8_t CHU_ErrCode_buf0[20];
uint8_t CHU_ErrCode_buf1[20];
uint8_t CHU_ErrCode_buf2[20];
uint8_t CHU_ErrCode_buf3[20];
uint8_t CHU_ErrCode_buf4[20];
uint8_t CHU_KMState[20];//三项接触器
uint8_t Get_CHUHeart_flg[20];
uint16_t CHU_heart_err_count[20];//65535*10
//0x85
uint8_t CHU_NonExe_code[20];
//0x87
uint8_t Get_CHUChgRtData_flg[20];
double BMS_PackSocReal_pct[20];
double BMS_CharPackVolReal_V[20];
double BMS_CharPackCurReal_A[20];
double BMS_ReqPackVol_V[20];
double BMS_ReqPackCur_A[20];
// uint8_t CHU_ChgMode[20];
double BMS_RemainCharTimeReal_min[20];
double ChU_AllChrTimeReal_min[20];
double Charger_OutPwr_W[20];
uint8_t FCH_BigState_Null[20];
//0x88
double CHU_AllOutEnergy_kwh[20];
uint8_t Get_StopCmdRes_flg[20];
//0x89
uint8_t Get_CHUErrStop_flg[20];
uint8_t CHUErrStop_code[20];
//0x8a
uint8_t BMS_CellVol_buf[20][120];
//0x8b
uint8_t BMS_CellTemp_buf[20][32] = {0};
//0x8D
// uint8_t ChaPrepDataBuf[20][];
uint8_t BMS_BatteryType_Null[20];
double BMS_RatedCap_Ah[20];
double BMS_RatedPackVol_V[20];
double CellVol_AllowCharMax_V[20];
double PackCur_AllowCharMax_A[20];
double BMS_RatedEnergy_Kwh[20];
double Temp_AllowCharMax_C[20];
//0x8F 获取电池实时数据指令反馈
uint8_t BattRtData_buf[20][60];
uint8_t Rly_Sta[20];//继电器当前状态
double BMS_BATT_SOC[20];    //动力电池剩余电量SOC
// double BMS_BATT_SOC[20]={40,33,96,92,0,13,12,1, 99,82,0,0,0,0,3,13,12,13}; 
double BMS_BATT_VOLT_V1[20]; //动力电池内部总电压V1
double BMS_BATT_VOLT_V2[20];  //动力电池外部总电压V2
double BMS_BATT_VOLT_V3[20];  //动力电池负载端总电压V3
double BMS_BATT_CUR[20];     //动力电池充/放电电流
double BMS_BATT_POSITIVE_RESISTANCE[20]; 	//动力电池正极对GND绝缘电阻
double BMS_BATT_NEGATIVE_RESISTANCE[20]; 	//动力电池负极对GND绝缘电阻
double BMS_BATT_AVAIL_ENERGY[20];       	//动力电池可用能量
double BMS_BATT_AVAIL_CAPACITY[20];      	//动力电池可用容量
double BMS_CHARGE_TOT_AH[20];       		//累计充电总安时
uint8_t BATT_WORK_MODE[20];		//快换电池工作模式	EU300无该信号
double BATT_ALLW_OUTPUT_CUR[20];	//电池包最大允许输出电流 EU300无该信号
double BATT_ALLW_FB_CUR[20];		//电池包最大允许回馈电流 EU300无该信号
double BMS_FEED_MAX_PWR[20];             	//当前状态允许最大回馈功率
double BMS_DISCHG_MAX_PWR[20];				//当前状态最大允许放电功率
uint8_t BMS_COOLING_FAN_STA[20];	//动力电池冷却风扇当前状态
uint8_t BMS_BATT_HEAT_STA[20];		//动力电池加热状态
uint8_t BMS_BAT_BALANCE_STA[20];	//动力电池均衡状态
uint8_t BMS_SYS_STA[20];			//动力电池系统状态	EU300无该信号
uint8_t BATT_Chg_Sta[20];		//动力电池充电状态	EU300无该信号
uint8_t BATT_PWR_LIMIT_RATIO_K[20];	//电池功率限制比例K	EU300无该信号
uint8_t BMS_Err_msg[20][7];//电池故障信息
double BMS_CELL_VOLT_MAX[20];//单体最高电压
double BMS_CELL_VOLT_MIN[20];//单体最低电压
double BMS_CELL_TEMP_MAX[20];//单体最高温度
double BMS_CELL_TEMP_MIN[20];//单体最低温度
uint8_t BMS_MAX_VOLT_NUM[20];//最高单体电压编号
uint8_t BMS_MAX_TEMP_NUM[20];//最高温度编号
uint8_t BMS_MIN_TEMP_NUM[20];//最低温度编号
uint8_t BMS_MIN_VOLT_NUM[20];//最低单体电压编号
//0x90获取电池基本信息指令反馈
uint8_t BattBasicInf_buf[20][60];
int Batt_SOH[20];//	动力电池健康度 INTEGER
unsigned char BMS_ParallNum_Null[20];  //BMS传出电池并联数
unsigned char BMS_SerialNum_Null[20];  //BMS传出电池串联数
int BATT_TEMP_SAMPLE_NUM[20]={0};//温度采集点个数
double	BMS_ODO[20];//动力电池行驶里程
double BMS_SW_VERS_main[20];	//软件版本
double BMS_SW_VERS[20];	//软件版本
double BMS_HW_VERS[20];	//硬件版本
double BMS_PART_VERS[20];//BMS零件号
uint8_t BMS_BATT_NUM[20][40];//动力电池编码

uint16_t Get_ChuData[10];
void CHU1Recv_Server_Deal(uint8_t* recv_buf,uint16_t RecvData_dlc,uint8_t CHU_num)
{
	uint16_t rec_calchkval;
	uint16_t my_calchkval;
	uint8_t i;
	uint16_t CHU_RecvData_dlc;
	// CHU_Recv_count++;

	// printf("recv_buf[7]:0x%x\r\n", recv_buf[7]);
	switch(recv_buf[7])
	{
		case 0x83://chu heart 
			mid_charger.CHU_DevState_code[CHU_num] = recv_buf[13];
			mid_charger.CHU_BMS_LinkState[CHU_num]= recv_buf[14];
			mid_charger.CHU_BattType[CHU_num] = recv_buf[15];
			CHU_NegTemp_C[CHU_num] = recv_buf[16];
			CHU_PosTemp_C[CHU_num] = recv_buf[17];

			CHU_ErrCode_buf0[CHU_num] = recv_buf[18];
			CHU_ErrCode_buf1[CHU_num] = recv_buf[19];
			CHU_ErrCode_buf2[CHU_num] = recv_buf[20];
			CHU_ErrCode_buf3[CHU_num] = recv_buf[21];
			CHU_ErrCode_buf4[CHU_num] = recv_buf[22];
			CHU_KMState[CHU_num] = recv_buf[23];//三项接触器
			// for(i=0;i<20;i++)
			// printf("get 0x83!\n");
			Get_ChuData[0]++;

		break;
		case 0x85://BMS低压上电/下电反馈
			CHU_NonExe_code[CHU_num] = recv_buf[13];
//			printf("get 0x85!\n");
		break;
		case 0x86://充电启动指令反馈
			CHU_NonExe_code[CHU_num] = recv_buf[13];

		break;
		case 0x87://充电数据上报
			// printf("get 0x87777777777777777777777 %d %d \n!",recv_buf[16],recv_buf[17]);
			Get_ChuData[1]++;
			mid_charger.Get_CHUChgRtData_flg[CHU_num] = 1;

			mid_charger.BMS_PackSocReal_pct[CHU_num] = recv_buf[13];
			mid_charger.BMS_CharPackVolReal_V[CHU_num] = (double)(recv_buf[14] * 256 + recv_buf[15])*0.1;
			mid_charger.BMS_CharPackCurReal_A[CHU_num] = (double)(recv_buf[16] * 256 + recv_buf[17])*0.1;

			BMS_ReqPackVol_V[CHU_num] = (double)(recv_buf[18] * 256 + recv_buf[19])*0.1;
			BMS_ReqPackCur_A[CHU_num] = (double)(recv_buf[20] * 256 + recv_buf[21])*0.1;

			// recv_buf[22];//charge mode
			BMS_RemainCharTimeReal_min[CHU_num] = (double)(recv_buf[23] * 256 + recv_buf[24]);
			ChU_AllChrTimeReal_min[CHU_num] = (double)(recv_buf[25] * 256 + recv_buf[26]);
			Charger_OutPwr_W[CHU_num] = (double)(recv_buf[27] * 256 + recv_buf[28]);
			FCH_BigState_Null[CHU_num] = (recv_buf[29]);
//			printf("get 0x87!\n");

		break;
		case 0x88://充电停止指令反馈
			Get_StopCmdRes_flg[CHU_num] = 1;
			if(recv_buf[13] != 0)
			{
				CHU_AllOutEnergy_kwh[CHU_num] = (double)(recv_buf[14] * 256 + recv_buf[15]);
			}	
			// printf("get 0x88!\n");

		break;
		case 0x89://充电停止信息上报
			mid_charger.Get_CHUErrStop_flg[CHU_num] = 1;
			if(recv_buf[13] != 0)
			{
				mid_charger.MChg_ErrStop_code[CHU_num] = recv_buf[13];
				CHU_AllOutEnergy_kwh[CHU_num] = (double)(recv_buf[14] * 256 + recv_buf[15]);
			}				
//			printf("get 0x89!=%d\n",recv_buf[13]);
		break;
		case 0x8A://获取单体电压信息指令反馈
			// for(i=0;i<recv_buf[14];i++)
			for(i=0;i<112;i++)
			{
				BMS_CellVol_buf[CHU_num][i] = recv_buf[15+i];
			}
			// printf("get 0x8A!%d\n",recv_buf[17]);
			Get_ChuData[2]++;
		break;
		case 0x8B://获取单体温度信息指令反馈
			for(i=0;i<32;i++)
			{
				BMS_CellTemp_buf[CHU_num][i] = recv_buf[15+i];
			}
			// printf("get 0x8B!\n");	
			Get_ChuData[3]++;
		break;
		case 0x8D://获取充电准备信息指令反馈
			BMS_BatteryType_Null[CHU_num] = recv_buf[14];
			BMS_RatedCap_Ah[CHU_num] = (double)(recv_buf[15]*256 + recv_buf[16])*0.1;
			BMS_RatedPackVol_V[CHU_num] = (double)(recv_buf[17]*256 + recv_buf[18])*0.1;	

			CellVol_AllowCharMax_V[CHU_num] = (double)(recv_buf[22]*256 + recv_buf[23])*0.01;
			PackCur_AllowCharMax_A[CHU_num] = (double)(recv_buf[24]*256 + recv_buf[25])*0.01-400;
			BMS_RatedEnergy_Kwh[CHU_num] = (double)(recv_buf[26]*256 + recv_buf[27])*0.01;
			Temp_AllowCharMax_C[CHU_num] = recv_buf[28];	
			// printf("get 0x8D!\n");			
		break;
		case 0x8E://获取充电结束信息指令反馈
			// printf("get 0x8E!\n");
		break;
		case 0x8F://获取电池实时数据指令反馈
		{	
			for(i=0;i<53;i++)
			{
				BattRtData_buf[CHU_num][i] = recv_buf[i+13];
			}
			// printf("get 0x8F!\n");
			Get_ChuData[4]++;
			Rly_Sta[CHU_num]=BattRtData_buf[CHU_num][1];
			BMS_BATT_SOC[CHU_num]=BattRtData_buf[CHU_num][2];
			BMS_BATT_VOLT_V1[CHU_num]=(BattRtData_buf[CHU_num][3]*256
				+BattRtData_buf[CHU_num][4])/100;
			BMS_BATT_VOLT_V2[CHU_num]=(BattRtData_buf[CHU_num][5]*256
				+BattRtData_buf[CHU_num][6])/100;
			BMS_BATT_VOLT_V3[CHU_num]=(BattRtData_buf[CHU_num][7]*256
				+BattRtData_buf[CHU_num][8])/100;	
			BMS_BATT_CUR[CHU_num]=(BattRtData_buf[CHU_num][9]*256
				+BattRtData_buf[CHU_num][10])/100;	
			BMS_BATT_POSITIVE_RESISTANCE[CHU_num]=(BattRtData_buf[CHU_num][11]*256
				+BattRtData_buf[CHU_num][12]);
			BMS_BATT_NEGATIVE_RESISTANCE[CHU_num]=(BattRtData_buf[CHU_num][13]*256
				+BattRtData_buf[CHU_num][14]);
			BMS_BATT_AVAIL_ENERGY[CHU_num]=(BattRtData_buf[CHU_num][15]*256
				+BattRtData_buf[CHU_num][16])/100;	
			BMS_BATT_AVAIL_CAPACITY[CHU_num]=(BattRtData_buf[CHU_num][17]*256
				+BattRtData_buf[CHU_num][18])/100;

			BMS_CHARGE_TOT_AH[CHU_num]=(((uint32_t)BattRtData_buf[CHU_num][19]<<24)
				+((uint32_t)BattRtData_buf[CHU_num][20]<<16)+((uint32_t)BattRtData_buf[CHU_num][21]<<8)
				+((uint32_t)BattRtData_buf[CHU_num][22]));

			BATT_WORK_MODE[CHU_num] = BattRtData_buf[CHU_num][23];

			BATT_ALLW_OUTPUT_CUR[CHU_num]=(BattRtData_buf[CHU_num][24]*256
				+BattRtData_buf[CHU_num][25])/100;	
			BATT_ALLW_FB_CUR[CHU_num]=(BattRtData_buf[CHU_num][26]*256
				+BattRtData_buf[CHU_num][27])/100;	

			BMS_FEED_MAX_PWR[CHU_num]=(BattRtData_buf[CHU_num][28]*256
				+BattRtData_buf[CHU_num][29])/100;	
			BMS_DISCHG_MAX_PWR[CHU_num]=(BattRtData_buf[CHU_num][30]*256
				+BattRtData_buf[CHU_num][31])/100;	

			// 动力电池状态	BattRtData_buf[CHU_num][32]	
			BMS_SYS_STA[CHU_num] = BattRtData_buf[CHU_num][33];
			BATT_Chg_Sta[CHU_num] = BattRtData_buf[CHU_num][34];
			BATT_PWR_LIMIT_RATIO_K[CHU_num] = BattRtData_buf[CHU_num][35];
			//电池故障信息
			BMS_Err_msg[CHU_num][0] = BattRtData_buf[CHU_num][36];
			BMS_Err_msg[CHU_num][1] = BattRtData_buf[CHU_num][37];
			BMS_Err_msg[CHU_num][2] = BattRtData_buf[CHU_num][38];
			BMS_Err_msg[CHU_num][3] = BattRtData_buf[CHU_num][39];
			BMS_Err_msg[CHU_num][4] = BattRtData_buf[CHU_num][40];
			BMS_Err_msg[CHU_num][5] = BattRtData_buf[CHU_num][41];
			BMS_Err_msg[CHU_num][6] = BattRtData_buf[CHU_num][42];		
			//单体最高电压		
			BMS_CELL_VOLT_MAX[CHU_num] = (double)((uint16_t)BattRtData_buf[CHU_num][43]*256
				+(uint16_t)BattRtData_buf[CHU_num][44])/1000;
			BMS_MAX_VOLT_NUM[CHU_num] = BattRtData_buf[CHU_num][45];
			//单体最低电压
			BMS_CELL_VOLT_MIN[CHU_num] = (double)((uint16_t)BattRtData_buf[CHU_num][46]*256
				+(uint16_t)BattRtData_buf[CHU_num][47])/1000;
			BMS_MIN_VOLT_NUM[CHU_num] = BattRtData_buf[CHU_num][48];
			//单体最高温度
			BMS_CELL_TEMP_MAX[CHU_num] = BattRtData_buf[CHU_num][49]-48;
			BMS_MAX_TEMP_NUM[CHU_num] = BattRtData_buf[CHU_num][50];
			//单体最低温度
			BMS_CELL_TEMP_MIN[CHU_num] = BattRtData_buf[CHU_num][51]-48;
			BMS_MIN_TEMP_NUM[CHU_num] = BattRtData_buf[CHU_num][52];					
		}			
		break;
		case 0x90://获取电池基本信息指令反馈
		{
			// printf("get 0x90!\n");
			CHU_GetBatBasic_flg[CHU_num] = 1;
			for(i=0;i<47;i++)
			{
				BattBasicInf_buf[CHU_num][i]= recv_buf[i+13];
			}

			// BattBasicInf_buf[CHU_num][0];
			Batt_SOH[CHU_num] = BattBasicInf_buf[CHU_num][1];
			BMS_SerialNum_Null[CHU_num] = BattBasicInf_buf[CHU_num][2];
			BMS_ParallNum_Null[CHU_num] = BattBasicInf_buf[CHU_num][3];
			BATT_TEMP_SAMPLE_NUM[CHU_num] = BattBasicInf_buf[CHU_num][4];
			
			BMS_ODO[CHU_num] = BattBasicInf_buf[CHU_num][5]*256*256
			+BattBasicInf_buf[CHU_num][6]*256+BattBasicInf_buf[CHU_num][7];

			BMS_SW_VERS[CHU_num] = BattBasicInf_buf[CHU_num][8];
			BMS_SW_VERS_main[CHU_num] = BattBasicInf_buf[CHU_num][9];
			BMS_HW_VERS[CHU_num] = BattBasicInf_buf[CHU_num][10];

			BMS_PART_VERS[CHU_num] = ((uint32_t)BattBasicInf_buf[CHU_num][11]<<24)
			+((uint32_t)BattBasicInf_buf[CHU_num][12]<<16)+((uint32_t)BattBasicInf_buf[CHU_num][13]<<8)
			+((uint32_t)BattBasicInf_buf[CHU_num][14]);

			int i;
			for(i=0;i<32;i++)
				BMS_BATT_NUM[CHU_num][i] = BattBasicInf_buf[CHU_num][15+i];
		}

		break;
		default:
		break;
	}
	// bzero(recv_buf, sizeof(recv_buf));	
}

void CHU1_Rec_Deal(uint8_t CHU_num,uint8_t* recv_buf,uint16_t len)
{
	uint16_t i=0;
	uint16_t rec_calchkval = 0;
    uint16_t my_calchkval = 0;
    uint16_t RecvData_dlc;
    uint16_t current_head = 0;
	uint8_t buf[500];
    static unsigned short ss;

    do
    {
		if(recv_buf[current_head+0]==0xfe&&recv_buf[current_head+1]==0x01)
        {
			// printf("sever=%x\n",recv_buf[current_head+7]);
            RecvData_dlc = recv_buf[current_head + 12];//当前帧的数据单元长度
			rec_calchkval = mc_check_crc16(recv_buf+current_head,RecvData_dlc+13);
			my_calchkval = ((uint16_t)recv_buf[RecvData_dlc+13]*256+(uint16_t)recv_buf[RecvData_dlc+14]);
			// printf("rec_calchkval,%dmy_calchkval,%d\n",rec_calchkval,my_calchkval);
            if(my_calchkval == rec_calchkval)
            {
				
				for(i=0;i<RecvData_dlc+13;i++)
				{
					buf[i] = recv_buf[current_head+i];
					// printf("%d",buf[i]);
				}
				 
				CHU1Recv_Server_Deal(buf,RecvData_dlc,0);
                ss++;
                current_head = current_head + RecvData_dlc + 15;//
            }
            else
            {
                break;
				ss = 0;
            }
        }
		else
		{
			break;
		}	
    }while(current_head + 5 <= len);		
	bzero(recv_buf, sizeof(recv_buf));
	bzero(buf, sizeof(recv_buf));
}

struct sockaddr_in servaddr;


uint8_t EMS_CHU_LowPwrOnCmd_buf[16] = {0xfe,0x01,0x00,0x00,0x00,0x00,0x00,0x05,0x00,0x00,0x00,0x00,0x01,0x01,0x00,0x00};
uint8_t EMS_CHU_LowPwrOnCmd_buf_len = 16;

uint8_t EMS_CHU_LowPwrDownCmd_buf[16] = {0xfe,0x01,0x00,0x00,0x00,0x00,0x00,0x05,0x00,0x00,0x00,0x00,0x01,0x02,0x00,0x00};
uint8_t EMS_CHU_LowPwrDownCmd_buf_len = 16;
// 0x06
uint8_t EMS_CHU_StartChgCmd_buf[17] = {0xfe,0x01,0x00,0x00,0x00,0x00,0x00,0x06,0x00,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x00};
uint8_t EMS_CHU_StartChgCmd_buf_len = 17;

// 0x07
uint8_t EMS_CHU_SetPwrCmd_buf[17] = {0xfe,0x01,0x00,0x00,0x00,0x00,0x00,0x07,0x00,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x00};
uint8_t EMS_CHU_SetPwrCmd_buf_len = 17;

//0x08
uint8_t EMS_CHU_StopChgCmd_buf[15] = {0xfe,0x01,0x00,0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
uint8_t EMS_CHU_StopChgCmd_buf_len = 15;

//0x0A
uint8_t EMS_CHU_GetCellVolCmd_buf[15] = {0xfe,0x01,0x00,0x00,0x00,0x00,0x00,0x0A,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
uint8_t EMS_CHU_GetCellVolCmd_buf_len = 15;

//0x0B
uint8_t EMS_CHU_GetCellTempCmd_buf[15] = {0xfe,0x01,0x00,0x00,0x00,0x00,0x00,0x0B,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
uint8_t EMS_CHU_GetCellTempCmd_buf_len = 15;

//0x0D
uint8_t EMS_CHU_GetCharPreInfCmd_buf[15] = {0xfe,0x01,0x00,0x00,0x00,0x00,0x00,0x0D,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
uint8_t EMS_CHU_GetCharPreInfCmd_buf_len = 15;

//0x0E
uint8_t EMS_CHU_GetCharEndInfCmd_buf[15] = {0xfe,0x01,0x00,0x00,0x00,0x00,0x00,0x0E,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
uint8_t EMS_CHU_GetCharEndInfCmd_buf_len = 15;

//0x0F
uint8_t EMS_CHU_GetBattRtDataCmd_buf[15] = {0xfe,0x01,0x00,0x00,0x00,0x00,0x00,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
uint8_t EMS_CHU_GetBattRtDataCmd_buf_len = 15;

//0x10
uint8_t EMS_CHU_GetBattBasicDataCmd_buf[15] = {0xfe,0x01,0x00,0x00,0x00,0x00,0x00,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
uint8_t EMS_CHU_GetBattBasicDataCmd_buf_len = 15;


void mid_charger_Interface(void)
{
	int i=0,j=0;
	for(i=0;i<20;i++)
	{
		EMS_CHU_LinkState[i] = mid_charger.EMS_CHU_LinkState[i];
		CHU_DevState_code[i] = mid_charger.CHU_DevState_code[i];
		CHU_BMS_LinkState[i] = mid_charger.CHU_BMS_LinkState[i];
		CHU_BattType[i] = mid_charger.CHU_BattType[i];
		Get_CHUChgRtData_flg[i] = mid_charger.Get_CHUChgRtData_flg[i];
		BMS_PackSocReal_pct[i] = mid_charger.BMS_PackSocReal_pct[i];
		BMS_CharPackVolReal_V[i] = mid_charger.BMS_CharPackVolReal_V[i];
		BMS_CharPackCurReal_A[i] = mid_charger.BMS_CharPackCurReal_A[i];
		if(mid_charger.Get_CHUErrStop_flg[i]){
			Get_CHUErrStop_flg[i] = mid_charger.Get_CHUErrStop_flg[i];
			mid_charger.Get_CHUErrStop_flg[i] = 0;
		}
		if(mid_charger.MChg_ErrStop_code[i]){
			MChg_ErrStop_code[i] = mid_charger.MChg_ErrStop_code[i];
			mid_charger.MChg_ErrStop_code[i] = 0;
		}

		BATT_IsExist_flg[i] = (CE_LocatorSenser[i]==2);
		EMS_CHU_ComErr_flg[i] = (EMS_CHU_LinkState[i]>1);

		MLocat_Conf_ena[i] = 1;

		Chg_Control_mode[i]=(MLocat_SelfChar_ena[i]);
		MLocat_Charging_flg[i]=(EMS_Chger_BigSta[i]==3);

		if(BATT_IsExist_flg[i]==1&&EMS_CHU_ComErr_flg[i]==0&&
			(CHU_DevState_code[i]>=3))
		{
			MLocat_Soc_pct[i] = BMS_BATT_SOC[i];
			MLocat_Car_type[i] = CHU_BattType[i];
			MLocat_GetBatCode_flg[i] = 1;
			// strncpy(MLocat_BatteryNo[i],BMS_BATT_NUM[i],40);	
		}
		else
		{
			MLocat_Soc_pct[i] = 0;
			MLocat_Car_type[i] = 0;
			MLocat_GetBatCode_flg[i] = 0;
			// MLocat_InNet_flg[i] = 0;
			// bzero(MLocat_BatteryNo[i],40);
		}	
	}
	if(MCE_EndThisTime_flg==1)
	{
		MCE_EndThisTime_flg = 0;
		// bzero(Cloud_Grab_carNo,30);
		bzero(Cloud_carType_now,50);
		bzero(Cloud_orderId_now,50);
		bzero(Cloud_BatteryNo_now,100);
		bzero(Cloud_CarODO_now,30);
		// bzero(ChangeBatCar_BatteryNo,100);
		bzero(Cloud_OutBatteryNo_now,100);

		Cloud_carType_Code = 0;
		// Grab_GetCarCard_flg = 0;
		MRemo_GetInPlace_flg = 0;

		MCloud_SendCancelOrd_flg = 0;
		MCloud_CancelOrd_ok = 0;//加两个预约的!!!!
	}
	if(Sta==30||Sta==31)
	{
		strcpy(Cloud_Grab_carNo_cancel,Cloud_Grab_carNo);
		strcpy(Cloud_orderId_now_cancel,Cloud_orderId_now);
	}

	/***   test!!! ****/
	// MLocat_BookLock_flg[1]=1,MLocat_BookLock_flg[3]=1,MLocat_BookLock_flg[5]=1;
	// CHU_BattType[1]=1,CHU_BattType[3]=2,CHU_BattType[5]=1;
	// strcpy(Cloud_BookcarNo1[0],"鲁BD36001");
	// strcpy(Cloud_BookcarNo1[1],"鲁BD35001");
	// strcpy(Cloud_BookcarNo2[0],"浙BD36001");
	// strcpy(MLocat_BatteryNo[0],"000"),strcpy(MLocat_BatteryNo[1],"111"),strcpy(MLocat_BatteryNo[2],"222");
	// strcpy(MLocat_BatteryNo[3],"333"),strcpy(MLocat_BatteryNo[4],"444"),strcpy(MLocat_BatteryNo[5],"555");
	// strcpy(MLocat_BatteryNo[6],"666"),strcpy(MLocat_BatteryNo[7],"00MPE0JC431D0MA431028001"),strcpy(MLocat_BatteryNo[8],"BABT12345678901");
	// strcpy(MLocat_BatteryNo[9],"999"),strcpy(MLocat_BatteryNo[10],"10"),strcpy(MLocat_BatteryNo[11],"11");
	// strcpy(MLocat_BatteryNo[12],"12"),strcpy(MLocat_BatteryNo[13],"13"),strcpy(MLocat_BatteryNo[14],"14");
	// strcpy(MLocat_BatteryNo[15],"15"),strcpy(MLocat_BatteryNo[16],"16"),strcpy(MLocat_BatteryNo[17],"17");
	// strcpy(Cloud_OutBatteryNo_now,"00MPE0JC431D0MA431028001");
	// MLocat_AllowCESoc_pct=0;MCE_AllBag_num=5;
	
	// CE_LocatorSenser[17]=2;CE_LocatorSenser[2]=2;CE_LocatorSenser[3]=2;CE_LocatorSenser[1]=2;CE_LocatorSenser[0]=2;CE_LocatorSenser[4]=2;
	// MLocat_SelfChar_ena[17] = 1;MLocat_SelfChar_ena[2] = 1;MLocat_SelfChar_ena[3] = 1;MLocat_SelfChar_ena[1] = 1;MLocat_SelfChar_ena[0] = 1;MLocat_SelfChar_ena[4] = 1;
	// MLocat_InNet_flg[17] = 1;MLocat_InNet_flg[2] = 1;MLocat_InNet_flg[3] = 1;MLocat_InNet_flg[1] = 1;MLocat_InNet_flg[0] = 1;MLocat_InNet_flg[4] = 1;
	// MLocat_Car_type[17] = 1;MLocat_Car_type[2] = 1;MLocat_Car_type[3] = 2;MLocat_Car_type[1] = 1;MLocat_Car_type[0] = 2;MLocat_Car_type[4] = 2;
	// MLocat_GetBatCode_flg[17] = 1;MLocat_GetBatCode_flg[2] = 1;MLocat_GetBatCode_flg[3] = 1;MLocat_GetBatCode_flg[1] = 1;MLocat_GetBatCode_flg[0] = 1;MLocat_GetBatCode_flg[4] = 1;
	// CHU_BattType[17]=1;CHU_BattType[2]=1;CHU_BattType[3]=2;MSys_AllErr_Lev=0;CHU_BattType[1]=1;CHU_BattType[0]=2;CHU_BattType[4]=2;
	// EMS_CHU_ComErr_flg[17]=0;EMS_CHU_ComErr_flg[2]=0;EMS_CHU_ComErr_flg[3]=0;EMS_CHU_ComErr_flg[1]=0;EMS_CHU_ComErr_flg[0]=0;EMS_CHU_ComErr_flg[4]=0;
	// if(Hand_LowPwrOn_flg[15])
	// 	EMS_CHU_ComErr_flg[0]=1,EMS_CHU_ComErr_flg[1]=1;
	// strcpy(Cloud_Grab_carNo,"鲁BD55355");
	// MCE_SelfCheckOk_sta = 1;
	// MSys_AllErr_Lev = 0;
	// MQt_StartSelfCE_sta = 1;
	// MLocat_SelfChar_ena[1] = 1;MLocat_GetBatCode_flg[1]=1;
	// MLocat_Conf_ena[1]=1;MLocat_Soc_pct[1] = 90;
	// MLocat_Charging_flg[1]=1;MLocat_Car_type[1] = 1;
	// BATT_IsExist_flg[1]=1;MLocat_InNet_flg[1] = 1;

	// MCloud_GetCarType_flg = 1;
	// Cloud_carType_Code = 1;
	// strcpy(Cloud_Grab_carNo,"浙BD36001"),strcpy(Cloud_Grab_carNo2,"鲁BD36001");
	MQt_GetSql_flg = 1;
	// Remot_conn_flg = 1;
	// MCE_ControlMode = 2;
	// MIO_DoorValue_null[0] = 1;MIO_DoorValue_null[1] = 1;MIO_DoorValue_null[2] = 1;
	// MIO_WaterValue_null = 1;MIO_LightValue_null[0] = 1;MIO_LightValue_null[1] = 1;
	// MIO_ThunderValue_null[0] = 1;MIO_ThunderValue_null[1] = 1;
	MGrab_Comm_flg = 1;MLed_Comm_flg = 1;
	// MCE_RGVStepData = 1;MCE_StackerStepData=1;
    // MCE_ParkPlatStepData=1;
	// MChar_SelfCe_sour  = 0;
	// Grab_GetCarCard_flg = 1;
	// MCE_InPlacSens_flg = 1;
	// MCE_GetChangeBat_flg = 1;
	// MChar_BatCodeCheck_ok = 1;

}

void thread_ModeCharger(void)
{
	printf("run to EMS thread.\n");
	int i=0;
	int index = 0;

	uint16_t test_count;
	// Hand_LowPwrOn_flg[0]=1;
	while(1)
	{
		usleep(10*1000);//1s
		test_count++;
		// CHU_DevState_code[0]=3;
//		MCE_SelfCheckOk_sta =1;    //2021
		mid_charger_Interface();
		Charger();
		ChangeElec();	

		if(test_count >= 100)
		{
			test_count = 0;
			// stationRtData_ChangeBatt_Interf(&StationRtData);
			SysErr_interface();
			// printf("StartChg_timeout[0] = %d\n",StartChg_timeout[0]);
			// printf("CHU_timeout0[0] = %d\n",CHU_timeout0[0]);
			// printf("chu_test_state[0] = %d\n",chu_test_state[0]);CHU_Recv_count
			// printf("CHU_Recv_count = %d\n",CHU_Recv_count);
		}
	
		for(index = 0;index <20;index++)
		{
			if(EMS_SendLowPwrOnCmd_flg[index] == 1)
			{
				syslog(LOG_INFO,"CHU_SendLowPwrOnCmd_flg[%d] %d\n",index,EMS_SendLowPwrOnCmd_flg[index]);
				// printf("Hand_LowPwrOn_flg[9]=%d%d\n",Chg_Control_mode[0],Hand_LowPwrOn_flg[1]);
				EMS_SendLowPwrOnCmd_flg[index] = 0;
				send(CHU_clientsockfd[index], EMS_CHU_LowPwrOnCmd_buf, EMS_CHU_LowPwrOnCmd_buf_len,MSG_DONTWAIT | MSG_NOSIGNAL);
			}
			if(EMS_SendLowPwrDownCmd_flg[index] == 1)
			{
				syslog(LOG_INFO,"CHU_SendLowPwrDownCmd_flg[%d] %d\n",index,EMS_SendLowPwrDownCmd_flg[index]);
				EMS_SendLowPwrDownCmd_flg[index] = 0;
				send(CHU_clientsockfd[index], EMS_CHU_LowPwrDownCmd_buf, EMS_CHU_LowPwrDownCmd_buf_len,MSG_DONTWAIT | MSG_NOSIGNAL);
			}
			if(EMS_SendStartChgCmd_flg[index] == 1)
			{
				syslog(LOG_INFO,"CHU_SendStartChgCmd_flg[%d] %d\n",index,EMS_SendStartChgCmd_flg[index]);
				EMS_SendStartChgCmd_flg[index] = 0;
				send(CHU_clientsockfd[index], EMS_CHU_StartChgCmd_buf, EMS_CHU_StartChgCmd_buf_len,MSG_DONTWAIT | MSG_NOSIGNAL);
//				printf("EMS_SendStartChgCmd_flg[10]=%d%d\n",index,EMS_SendStartChgCmd_flg[index]);
			}
			if(EMS_SendSetPwrCmd_flg[index] == 1)
			{
				syslog(LOG_INFO,"CHU_SendSetPwrCmd_flg[%d] %d\n",index,EMS_SendSetPwrCmd_flg[index]);
				EMS_SendSetPwrCmd_flg[index] = 0;
				EMS_CHU_SetPwrCmd_buf[13] = ((EMS_CHUSetPwr_kw[index]*1000)/256);
				EMS_CHU_SetPwrCmd_buf[14] = ((int)(EMS_CHUSetPwr_kw[index]*1000)%256);
				send(CHU_clientsockfd[index], EMS_CHU_SetPwrCmd_buf, EMS_CHU_SetPwrCmd_buf_len,MSG_DONTWAIT | MSG_NOSIGNAL);
			}
			if(EMS_SendStopChgCmd_flg[index] == 1)
			{
				syslog(LOG_INFO,"CHU_SendStopChgCmd_flg[%d] %d\n",index,EMS_SendStopChgCmd_flg[index]);
				EMS_SendStopChgCmd_flg[index] =0;
				send(CHU_clientsockfd[index], EMS_CHU_StopChgCmd_buf, EMS_CHU_StopChgCmd_buf_len,MSG_DONTWAIT | MSG_NOSIGNAL);
			}

			if(EMS_SendGetCellVolCmd_flg[index] == 1)//0A
			{
				EMS_SendGetCellVolCmd_flg[index] = 0;
				send(CHU_clientsockfd[index], EMS_CHU_GetCellVolCmd_buf, EMS_CHU_GetCellVolCmd_buf_len,MSG_DONTWAIT | MSG_NOSIGNAL);
//				printf("sendCell\n");
			}
			if(EMS_SendGetCellTempCmd_flg[index] == 1)//0B
			{
				EMS_SendGetCellTempCmd_flg[index] = 0;
				send(CHU_clientsockfd[index], EMS_CHU_GetCellTempCmd_buf, EMS_CHU_GetCellTempCmd_buf_len,MSG_DONTWAIT | MSG_NOSIGNAL);
				// printf("sendTemp\n");
			}
			// if(0)
			if(EMS_SendGetCharPreInfCmd_flg[index] == 1)//0D
			{
				EMS_SendGetCharPreInfCmd_flg[index] = 0;
				send(CHU_clientsockfd[index], EMS_CHU_GetCharPreInfCmd_buf, EMS_CHU_GetCharPreInfCmd_buf_len,MSG_DONTWAIT | MSG_NOSIGNAL);
			}
			if(EMS_SendGetCharEndInfCmd_flg[index] == 1)//0E
			{
				EMS_SendGetCharEndInfCmd_flg[index] = 0;
				send(CHU_clientsockfd[index], EMS_CHU_GetCharEndInfCmd_buf, EMS_CHU_GetCharEndInfCmd_buf_len,MSG_DONTWAIT | MSG_NOSIGNAL);
			}
			if(EMS_SendGetBattRtDataCmd_flg[index] == 1)//0F
			{
				EMS_SendGetBattRtDataCmd_flg[index] = 0;
				send(CHU_clientsockfd[index], EMS_CHU_GetBattRtDataCmd_buf, EMS_CHU_GetBattRtDataCmd_buf_len,MSG_DONTWAIT | MSG_NOSIGNAL);
			}
			if(EMS_SendGetBattBasicDataCmd_flg[index] == 1)//10
			{
				EMS_SendGetBattBasicDataCmd_flg[index] = 0;
				// printf("aaa\n");
				send(CHU_clientsockfd[index], EMS_CHU_GetBattBasicDataCmd_buf,15,MSG_DONTWAIT | MSG_NOSIGNAL);
			}
		}
	}
}


void pthread_CHU1(void)
{
	uint8_t CHU_Recv_buf[500];
	uint8_t CHU_index = 0;
	int16_t read_size;
	int8_t CHU_connect_ret = -1;
	//create sockfd
    CHU_clientsockfd[CHU_index] = socket(AF_INET, SOCK_STREAM, 0);
//	printf("CHU%d_clientsockfd = %d \n",CHU_index,CHU_clientsockfd[CHU_index]);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(CHU_COM_PORT);  
    servaddr.sin_addr.s_addr = inet_addr(CHU1_SEVER_IP);  //服务器ip

	//连接服务器成功返回0错误返回-1
	do{
		CHU_connect_ret = connect(CHU_clientsockfd[CHU_index], (struct sockaddr *)&servaddr, sizeof(servaddr));
		if(CHU_connect_ret < 0)
		{	
//			printf("connect to CHU%d failed.\n",CHU_index);
			sleep(5);
			mid_charger.EMS_CHU_LinkState[CHU_index] = 2;
		}			
	}
	while(CHU_connect_ret<0);
	mid_charger.EMS_CHU_LinkState[CHU_index] = 0x01;
	while (1)
	{
		usleep(10*1000);//10ms
		if(CHUSend_Discon_cou[CHU_index]>=15)
		{			
			mid_charger.EMS_CHU_LinkState[CHU_index] = 2;
			int8_t Send_flg = -1;
			// conn1 = connect(clientsockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
			while(Send_flg<0)
			{
				close(CHU_clientsockfd[CHU_index]);
				sleep(1);
				CHU_clientsockfd[CHU_index] = socket(AF_INET, SOCK_STREAM, 0);
				int flags = fcntl(CHU_clientsockfd[CHU_index], F_GETFL, 0);  
				fcntl(CHU_clientsockfd[CHU_index], F_SETFL, flags|O_NONBLOCK);
				connect(CHU_clientsockfd[CHU_index], (struct sockaddr *)&servaddr, sizeof(servaddr));
				sleep(5);
				Send_flg = send(CHU_clientsockfd[CHU_index], Send_ReConnect_buf, (1),MSG_DONTWAIT|MSG_NOSIGNAL);

				fprintf(stderr, "connect qt function failed.\n");
				mid_charger.EMS_CHU_LinkState[CHU_index] = 2;
				//sleep(10);
				// printf("Send_flg22222%d = ",Send_flg);
			}		
		}
		else
		{
			mid_charger.EMS_CHU_LinkState[CHU_index] = 1;
		}
		
		int32_t Read_Size1=(recv(CHU_clientsockfd[CHU_index], CHU_Recv_buf, 500,MSG_DONTWAIT|MSG_NOSIGNAL));
		if(Read_Size1<=0){
		// if((recv(clientsockfd, Qt_Recv_buf, BUFFER_SIZE,MSG_DONTWAIT|MSG_NOSIGNAL))<0){
			// mid_charger.EMS_CHU_LinkState[CHU_index] = 0x02;
				}	
		else{
			// printf("Read_Size1 %d\n",Read_Size1);
			CHU1_Rec_Deal(CHU_index,CHU_Recv_buf,read_size);	
			CHUSend_Discon_cou[CHU_index] = 0;
			mid_charger.EMS_CHU_LinkState[CHU_index] = 1;
		}
	}	
	close(CHU_clientsockfd[CHU_index]);
}

