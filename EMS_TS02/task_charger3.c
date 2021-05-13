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

#define CHU3_SEVER_IP		("192.168.1.23")     //gai
// #define CHU10_SEVER_IP		("192.168.1.26") 



void CHU3Recv_Server_Deal(uint8_t* recv_buf,uint16_t RecvData_dlc,uint8_t CHU_num)
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
		// printf("0x83:%d\r\n", recv_buf[13]);
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
			// printf("get 0x83!\n");

		break;
		case 0x85://BMS低压上电/下电反馈
			CHU_NonExe_code[CHU_num] = recv_buf[13];
			// printf("get 0x85!\n");
		break;
		case 0x86://充电启动指令反馈
			CHU_NonExe_code[CHU_num] = recv_buf[13];

		break;
		case 0x87://充电数据上报
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
			// printf("get 0x87!\n");

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

		break;
		case 0x8B://获取单体温度信息指令反馈
			for(i=0;i<32;i++)
			{
				BMS_CellTemp_buf[CHU_num][i] = recv_buf[15+i];
			}
			// printf("get 0x8B!\n");	
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

			BMS_BATT_AVAIL_CAPACITY[CHU_num]=((uint32_t)BattRtData_buf[CHU_num][19]<<24
				+(uint32_t)BattRtData_buf[CHU_num][20]<<16+(uint32_t)BattRtData_buf[CHU_num][21]<<8
				+(uint32_t)BattRtData_buf[CHU_num][22]);

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

			BMS_PART_VERS[CHU_num] = (uint32_t)BattBasicInf_buf[CHU_num][11]<<24
			+(uint32_t)BattBasicInf_buf[CHU_num][12]<<16+(uint32_t)BattBasicInf_buf[CHU_num][13]<<8
			+(uint32_t)BattBasicInf_buf[CHU_num][14];

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

void CHU3_Rec_Deal(uint8_t CHU_num,uint8_t* recv_buf,uint16_t len)
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
			rec_calchkval = mc_check_crc16(recv_buf,RecvData_dlc+13);
			my_calchkval = (recv_buf[RecvData_dlc+13]*256+recv_buf[RecvData_dlc+14]);
 
            if(my_calchkval == rec_calchkval)
            {
				for(i=0;i<RecvData_dlc+13;i++)
				  buf[i] = recv_buf[current_head+i];
				CHU3Recv_Server_Deal(buf,RecvData_dlc,CHU_num);
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
	// bzero(recv_buf, sizeof(recv_buf));
}


void pthread_CHU3(void)
{
	struct sockaddr_in servaddr2;
	uint8_t CHU_Recv_buf[500];
	uint8_t CHU_index = 2;
	int16_t read_size;
	int8_t CHU_connect_ret = -1;
	//create sockfd
    CHU_clientsockfd[CHU_index] = socket(AF_INET, SOCK_STREAM, 0);
//	printf("CHU%d_clientsockfd = %d \n",CHU_index,CHU_clientsockfd[CHU_index]);

    bzero(&servaddr2, sizeof(servaddr2));
    servaddr2.sin_family = AF_INET;
    servaddr2.sin_port = htons(CHU_COM_PORT);  //PCS服务器端口
    servaddr2.sin_addr.s_addr = inet_addr(CHU3_SEVER_IP);  //服务器ip

	//连接服务器成功返回0错误返回-1
	do{
		CHU_connect_ret = connect(CHU_clientsockfd[CHU_index], (struct sockaddr *)&servaddr2, sizeof(servaddr2));
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
				connect(CHU_clientsockfd[CHU_index], (struct sockaddr *)&servaddr2, sizeof(servaddr2));
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
			CHU3_Rec_Deal(CHU_index,CHU_Recv_buf,read_size);
			CHUSend_Discon_cou[CHU_index] = 0;
			mid_charger.EMS_CHU_LinkState[CHU_index] = 1;
		}
	}	
	close(CHU_clientsockfd[CHU_index]);
}

