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
// #include "middle.h"


#define PORT                  502
#define AIR1_CONTROL_ADDR     1
#define AIR2_CONTROL_ADDR     2
#define AIR3_CONTROL_ADDR     3
#define BUFFER_SIZE           100
#define SENDGETIOIN_SIZE         8
#define SENDCONTIOOUT_SIZE         20 
#define SENDCLEARIOOUT_SIZE         20  

mid_aircon_t mid_aircon;

uint8_t AirCon1_SendStaData_buf[500]={AIR1_CONTROL_ADDR,0x01,0x00,0x00,0x00,0x06};//状态数据
uint8_t AirCon1_SendErrData_buf[500]={AIR1_CONTROL_ADDR,0x02,0x00,0x00,0x00,0x12};//告警数据
uint8_t AirCon1_SendSenserData_buf[500]={AIR1_CONTROL_ADDR,0x04,0x00,0x00,0x00,0x0A};//传感器数据
uint8_t AirCon1_SendGetData_buf[500]={AIR1_CONTROL_ADDR,0x03,0x00,0x00,0x00,0x0A};//参数读取
uint8_t AirCon1_SendSetData_buf[500]={AIR1_CONTROL_ADDR,0x06,0x00,0x00,0x00,0x03,0x2};//1个参数设置
uint8_t AirCon1_SendOnOff_buf[500]={AIR1_CONTROL_ADDR,0x05,0x00,0x00,0x00,0x0};//控制开关

uint8_t AirCon2_SendStaData_buf[500]={AIR2_CONTROL_ADDR,0x01,0x00,0x00,0x00,0x06};//状态数据
uint8_t AirCon2_SendErrData_buf[500]={AIR2_CONTROL_ADDR,0x02,0x00,0x00,0x00,0x12};//告警数据
uint8_t AirCon2_SendSenserData_buf[500]={AIR2_CONTROL_ADDR,0x04,0x00,0x00,0x00,0x0A};//传感器数据
uint8_t AirCon2_SendGetData_buf[500]={AIR2_CONTROL_ADDR,0x03,0x00,0x00,0x00,0x0A};//参数读取
uint8_t AirCon2_SendSetData_buf[500]={AIR2_CONTROL_ADDR,0x06,0x00,0x00,0x00,0x03,0x2};//1个参数设置
uint8_t AirCon2_SendOnOff_buf[500]={AIR2_CONTROL_ADDR,0x05,0x00,0x00,0x00,0x0};//控制开关

uint8_t AirCon3_SendStaData_buf[500]={AIR3_CONTROL_ADDR,0x01,0x00,0x00,0x00,0x06};//状态数据
uint8_t AirCon3_SendErrData_buf[500]={AIR3_CONTROL_ADDR,0x02,0x00,0x00,0x00,0x12};//告警数据
uint8_t AirCon3_SendSenserData_buf[500]={AIR3_CONTROL_ADDR,0x04,0x00,0x00,0x00,0x0A};//传感器数据
uint8_t AirCon3_SendGetData_buf[500]={AIR3_CONTROL_ADDR,0x03,0x00,0x00,0x00,0x0A};//参数读取
uint8_t AirCon3_SendSetData_buf[500]={AIR3_CONTROL_ADDR,0x06,0x00,0x00,0x00,0x03,0x2};//1个参数设置
uint8_t AirCon3_SendOnOff_buf[500]={AIR3_CONTROL_ADDR,0x05,0x00,0x00,0x00,0x0};//控制开关
// uint8_t AirCon_SendClearIOOut_buf[500]={0x0};

uint8_t AirCon_SendStaData_flg[3];
uint8_t AirCon_SendErrData_flg[3];
uint8_t AirCon_SendSenserData_flg[3];
uint8_t AirCon_SendGetData_flg[3];
uint8_t AirCon_SendSetData_flg[3];
uint8_t AirCon_SendOnOff_flg[3];
uint8_t Qt_AirSetData_flg[3];
uint8_t Qt_AirOpen_flg[3];

// uint8_t AirCon_conn_flg = 0;
uint16_t AirConSend_Discon_con[3];
uint8_t AirCon_Eachconn_sta[3];
uint8_t Send_FireCheckBack_flg;
uint8_t AirCon_GetFire_flg;
/**Rec***/
uint8_t AirCon_MerRunSta[3];	
uint8_t AirCon_EnginRunSta[3];	
uint8_t AirCon_HertRunSta[3];
uint8_t AirCon_Err1[3];
uint8_t AirCon_Err2[3];
uint8_t AirCon_Err3[3];
int AirCon_Temp[3];
int AirCon_Humi[3];
int16_t AirSet_EnginStartTemp_C;
int16_t AirSet_EnginStopTemp_C;
int16_t AirSet_HertStartTemp_C;
int16_t AirSet_HertStopTemp_C;
int16_t AirSet_FanStartTemp_C;
int16_t AirSet_FanStopTemp_C;
int16_t AirSet_ErrOverTemp_C;
int16_t AirSet_ErrUnderTemp_C;

int16_t AirRead_EnginStartTemp_C[3];
int16_t AirRead_HertStartTemp_C[3];


void AirConRecv_Server_Deal(uint8_t* recv_buf,uint8_t num)  //num:空调123
{
	uint8_t i=0;
	// AirConSend_Discon_con = 0;
	// Recv_count++;
	AirConSend_Discon_con[num] = 0;	
	AirCon_Eachconn_sta[num] = 1;
	switch (recv_buf[1])  //0x0103020003b844
		{
		case 0x01:	


			// printf("Air_sta111%x\n",recv_buf[3]);	
			mid_aircon.AirCon_MerRunSta[num] = (recv_buf[3]&0x01);	
			mid_aircon.AirCon_EnginRunSta[num] = ((recv_buf[3]>>2)&0x01);	
			mid_aircon.AirCon_HertRunSta[num] = ((recv_buf[3]>>3)&0x01);


			// printf("mid_aircon %d = %d %d %d \n"
			// ,num,mid_aircon.AirCon_MerRunSta[num]
			// ,mid_aircon.AirCon_EnginRunSta[num]
			// ,mid_aircon.AirCon_HertRunSta[num]);

			break;
		case 0x02:    //errData
		{
			// printf("Air_err222 %x %x %x\n",recv_buf[3],recv_buf[4],recv_buf[5]);	
			mid_aircon.AirCon_Err1[num] = recv_buf[3];
			mid_aircon.AirCon_Err2[num] = recv_buf[4];
			mid_aircon.AirCon_Err3[num] = recv_buf[5];
		}
		   break;
		case 0x06:    //1个参数设置
		{
			// for(i=0;i<8;i++)
				// printf(" Air_err6666666%x\n",recv_buf[5]);
		}
		break;
		case 0x86:    //1个参数设置
		{
			// for(i=0;i<8;i++)
///				printf(" Air_err888886666666%x",recv_buf[3]);
		}
		break;
		case 0x04:    //SenserData
		{
			mid_aircon.AirCon_Temp[num] = ((recv_buf[15]*256*0+recv_buf[16])-180)/2;
			mid_aircon.AirCon_Humi[num] = ((recv_buf[17]*256+recv_buf[18])-0);
			
//			printf("AirCon_Temp %d = %d %d\n",num
//				,mid_aircon.AirCon_Temp[num],mid_aircon.AirCon_Humi[num]);



			// printf("aaaaaaaaaaaaa[num]%d %d=\n",AirCon_Temp[0],AirCon_Humi[0]);	
			// printf("Air_err444%x\n",recv_buf[0]);
		}
			break;
		case 0x03:    //参数读取
		{		
			mid_aircon.AirRead_EnginStartTemp_C[num] = ((recv_buf[3]*256+recv_buf[4])-180)/2;
  			mid_aircon.AirRead_HertStartTemp_C[num] = ((recv_buf[19]*256+recv_buf[20])-180)/2;
			//   printf("AirRead_HertStartTemp_C[num]%d %d=\n",mid_aircon.AirRead_HertStartTemp_C[num],num);	
		}
			break;
		default:
			break;
	}
}
void AirCon_Rec_Deal(int16_t Read_Size,uint8_t* recv_buf)
{
	uint16_t i=0,rec_calchkval,my_calchkval;
	uint16_t RecvData_dlc;
	uint8_t buf[500];
	if((recv_buf[0]>=1)&&(recv_buf[0]<=3)&&(recv_buf[1]>=1)&&(recv_buf[1]<=16))
	{
		RecvData_dlc = recv_buf[2];
		rec_calchkval = mc_check_crc16(recv_buf,RecvData_dlc+3);
		my_calchkval = (recv_buf[RecvData_dlc+3]*256+recv_buf[RecvData_dlc+4]);
		if((my_calchkval==rec_calchkval)&&(recv_buf[0]>=1)||(recv_buf[1]==6))
		{			
			AirConRecv_Server_Deal(recv_buf,(recv_buf[0]-1));
		}	
	}
	bzero(recv_buf, sizeof(recv_buf));
}
void AirConInter_Trans_Deal(uint8_t num)
{
	uint16_t check16=0;
	switch (num)
	{
		case 1:
		{
			AirCon_SendStaData_flg[0] = 1;
			check16 = mc_check_crc16(AirCon1_SendStaData_buf,6);
			AirCon1_SendStaData_buf[6] = (uint8_t)(check16/256);
			AirCon1_SendStaData_buf[7] = (uint8_t)(check16%256);
		}break;
		case 2:
		{
			AirCon_SendStaData_flg[1] = 1;
			check16 = mc_check_crc16(AirCon2_SendStaData_buf,6);
			AirCon2_SendStaData_buf[6] = (uint8_t)(check16/256);
			AirCon2_SendStaData_buf[7] = (uint8_t)(check16%256);
		}break;
		case 3:
		{
			AirCon_SendStaData_flg[2] = 1;
			check16 = mc_check_crc16(AirCon3_SendStaData_buf,6);
			AirCon3_SendStaData_buf[6] = (uint8_t)(check16/256);
			AirCon3_SendStaData_buf[7] = (uint8_t)(check16%256);
		}break;
		case 4:
		{
			AirCon_SendErrData_flg[0] = 1;
			check16 = mc_check_crc16(AirCon1_SendErrData_buf,6);
			AirCon1_SendErrData_buf[6] = (uint8_t)(check16/256);
			AirCon1_SendErrData_buf[7] = (uint8_t)(check16%256);
		}break;
		case 5:
		{
			AirCon_SendErrData_flg[1] = 1;
			check16 = mc_check_crc16(AirCon2_SendErrData_buf,6);
			AirCon2_SendErrData_buf[6] = (uint8_t)(check16/256);
			AirCon2_SendErrData_buf[7] = (uint8_t)(check16%256);
		}break;
		case 6:
		{
			AirCon_SendErrData_flg[2] = 1;
			check16 = mc_check_crc16(AirCon3_SendErrData_buf,6);
			AirCon3_SendErrData_buf[6] = (uint8_t)(check16/256);
			AirCon3_SendErrData_buf[7] = (uint8_t)(check16%256);
		}break;
		case 7:
		{
			AirCon_SendSenserData_flg[0] = 1;
			check16 = mc_check_crc16(AirCon1_SendSenserData_buf,6);
			AirCon1_SendSenserData_buf[6] = (uint8_t)(check16/256);
			AirCon1_SendSenserData_buf[7] = (uint8_t)(check16%256);
		}break;
		case 8:
		{
			AirCon_SendSenserData_flg[1] = 1;
			check16 = mc_check_crc16(AirCon2_SendSenserData_buf,6);
			AirCon2_SendSenserData_buf[6] = (uint8_t)(check16/256);
			AirCon2_SendSenserData_buf[7] = (uint8_t)(check16%256);
		}break;
		case 9:
		{
			AirCon_SendSenserData_flg[2] = 1;
			check16 = mc_check_crc16(AirCon3_SendSenserData_buf,6);
			AirCon3_SendSenserData_buf[6] = (uint8_t)(check16/256);
			AirCon3_SendSenserData_buf[7] = (uint8_t)(check16%256);
		}break;
		case 10:
		{
			AirCon_SendGetData_flg[0] = 1;
			check16 = mc_check_crc16(AirCon1_SendGetData_buf,6);
			AirCon1_SendGetData_buf[6] = (uint8_t)(check16/256);
			AirCon1_SendGetData_buf[7] = (uint8_t)(check16%256);
		}break;
		case 11:
		{
			AirCon_SendGetData_flg[1] = 1;
			check16 = mc_check_crc16(AirCon2_SendGetData_buf,6);
			AirCon2_SendGetData_buf[6] = (uint8_t)(check16/256);
			AirCon2_SendGetData_buf[7] = (uint8_t)(check16%256);
		}break;
		case 12:
		{
			AirCon_SendGetData_flg[2] = 1;
			check16 = mc_check_crc16(AirCon3_SendGetData_buf,6);
			AirCon3_SendGetData_buf[6] = (uint8_t)(check16/256);
			AirCon3_SendGetData_buf[7] = (uint8_t)(check16%256);
		}break;
		
		default:
			break;
		}
}
void AirConInter_SetOnOff_Deal(){

//	printf("AirConInter_SetOnOff_Deal mid_aircon.Qt_AirSetData_flg[i] = %d\n\n\n",mid_aircon.Qt_AirSetData_flg[0]);
	uint16_t check16 = 0;
	if(mid_aircon.Qt_AirSetData_flg[0]>=1){
		AirCon_SendSetData_flg[0] = 1;
		if(mid_aircon.Qt_AirSetData_flg[0]==255)
			AirCon1_SendSetData_buf[3] = 0x00;
		else if(mid_aircon.Qt_AirSetData_flg[0]==1)
			AirCon1_SendSetData_buf[3] = 0x00;
		else if(mid_aircon.Qt_AirSetData_flg[0]==2)
			AirCon1_SendSetData_buf[3] = 0x01;
		else if(mid_aircon.Qt_AirSetData_flg[0]==3)
			AirCon1_SendSetData_buf[3] = 0x08;
		else if(mid_aircon.Qt_AirSetData_flg[0]==4)
			AirCon1_SendSetData_buf[3] = 0x09;
		else if(mid_aircon.Qt_AirSetData_flg[0]==5)
			AirCon1_SendSetData_buf[3] = 0x0C;
		else if(mid_aircon.Qt_AirSetData_flg[0]==6)
			AirCon1_SendSetData_buf[3] = 0x0D;
		else if(mid_aircon.Qt_AirSetData_flg[0]==7)
			AirCon1_SendSetData_buf[3] = 0x10;
		else if(mid_aircon.Qt_AirSetData_flg[0]==8)
			AirCon1_SendSetData_buf[3] = 0x11;
		if(mid_aircon.Qt_AirSetData_flg[0]==255 || (mid_aircon.Qt_AirSetData_flg[0]==1))
		{
			AirCon1_SendSetData_buf[4] = (mid_aircon.AirSet_EnginStartTemp_C*2+180)/256;
			AirCon1_SendSetData_buf[5] = (mid_aircon.AirSet_EnginStartTemp_C*2+180)%256;
		}
		else if(mid_aircon.Qt_AirSetData_flg[0]==3)
		{
			AirCon1_SendSetData_buf[4] = (mid_aircon.AirSet_HertStartTemp_C*2+180)/256;
			AirCon1_SendSetData_buf[5] = (mid_aircon.AirSet_HertStartTemp_C*2+180)%256;
		}		

		check16 = mc_check_crc16(AirCon1_SendSetData_buf,6);
		AirCon1_SendSetData_buf[6] = (uint8_t)(check16/256);
		AirCon1_SendSetData_buf[7] = (uint8_t)(check16%256);
	// printf("airairairairairair2343243242= %d %d \n",AirSet_EnginStartTemp_C,AirSet_EnginStartTemp_C);
		if(mid_aircon.Qt_AirSetData_flg[0]==255){
			mid_aircon.Qt_AirSetData_flg[0] = 3;
			// mid_aircon.AirSet_EnginStartTemp_C = mid_aircon.AirSet_HertStartTemp_C;
		}	
		else mid_aircon.Qt_AirSetData_flg[0] = 0;
		// AirCon1_SendSetData_buf[7] = (AirSet_EnginStartTemp_C*2+180)/256;
		// AirCon1_SendSetData_buf[8] = (AirSet_EnginStartTemp_C*2+180)%256;
		// AirCon1_SendSetData_buf[9] = (AirSet_EnginStopTemp_C*2+180)/256;
		// AirCon1_SendSetData_buf[10] = (AirSet_EnginStopTemp_C*2+180)/256;
		// AirCon1_SendSetData_buf[11] = (AirSet_HertStartTemp_C*2+180)/256;
		// AirCon1_SendSetData_buf[12] = (AirSet_HertStartTemp_C*2+180)%256;
		// AirCon1_SendSetData_buf[13] = (AirSet_HertStopTemp_C*2+180)/256;
		// AirCon1_SendSetData_buf[14] = (AirSet_HertStopTemp_C*2+180)%256;
		// AirCon1_SendSetData_buf[15] = (AirSet_FanStartTemp_C*2+180)/256;
		// AirCon1_SendSetData_buf[16] = (AirSet_FanStartTemp_C*2+180)%256;
		// AirCon1_SendSetData_buf[17] = (AirSet_FanStopTemp_C*2+180)/256;
		// AirCon1_SendSetData_buf[18] = (AirSet_FanStopTemp_C*2+180)%256;
		// AirCon1_SendSetData_buf[19] = (AirSet_ErrOverTemp_C*2+180)/256;
		// AirCon1_SendSetData_buf[20] = (AirSet_ErrOverTemp_C*2+180)%256;
		// AirCon1_SendSetData_buf[21] = (AirSet_ErrUnderTemp_C*2+180)/256;
		// AirCon1_SendSetData_buf[22] = (AirSet_ErrUnderTemp_C*2+180)%256;
	}			
	else if(mid_aircon.Qt_AirSetData_flg[1]>=1){
		AirCon_SendSetData_flg[1] = 1;
		if(mid_aircon.Qt_AirSetData_flg[1]==255)
			AirCon2_SendSetData_buf[3] = 0x00;
		else if(mid_aircon.Qt_AirSetData_flg[1]==1)
			AirCon2_SendSetData_buf[3] = 0x00;
		else if(mid_aircon.Qt_AirSetData_flg[1]==2)
			AirCon2_SendSetData_buf[3] = 0x01;
		else if(mid_aircon.Qt_AirSetData_flg[1]==3)
			AirCon2_SendSetData_buf[3] = 0x08;
		else if(mid_aircon.Qt_AirSetData_flg[1]==4)
			AirCon2_SendSetData_buf[3] = 0x09;
		else if(mid_aircon.Qt_AirSetData_flg[1]==5)
			AirCon2_SendSetData_buf[3] = 0x0C;
		else if(mid_aircon.Qt_AirSetData_flg[1]==6)
			AirCon2_SendSetData_buf[3] = 0x0D;
		else if(mid_aircon.Qt_AirSetData_flg[1]==7)
			AirCon2_SendSetData_buf[3] = 0x10;
		else if(mid_aircon.Qt_AirSetData_flg[1]==8)
			AirCon2_SendSetData_buf[3] = 0x11;

		if(mid_aircon.Qt_AirSetData_flg[1]==255 || (mid_aircon.Qt_AirSetData_flg[1]==1))
		{
			AirCon2_SendSetData_buf[4] = (mid_aircon.AirSet_EnginStartTemp_C*2+180)/256;
			AirCon2_SendSetData_buf[5] = (mid_aircon.AirSet_EnginStartTemp_C*2+180)%256;
		}
		else if(mid_aircon.Qt_AirSetData_flg[1]==3)
		{
			AirCon2_SendSetData_buf[4] = (mid_aircon.AirSet_HertStartTemp_C*2+180)/256;
			AirCon2_SendSetData_buf[5] = (mid_aircon.AirSet_HertStartTemp_C*2+180)%256;
		}
		check16 = mc_check_crc16(AirCon2_SendSetData_buf,6);
		AirCon2_SendSetData_buf[6] = (uint8_t)(check16/256);
		AirCon2_SendSetData_buf[7] = (uint8_t)(check16%256);
		
		if(mid_aircon.Qt_AirSetData_flg[1]==255){
			mid_aircon.Qt_AirSetData_flg[1] = 3;
			// mid_aircon.AirSet_EnginStartTemp_C = mid_aircon.AirSet_HertStartTemp_C;
		}	
		else mid_aircon.Qt_AirSetData_flg[1] = 0;
	}			
	else if(mid_aircon.Qt_AirSetData_flg[2]>=1){
		AirCon_SendSetData_flg[2] = 1;
		if(mid_aircon.Qt_AirSetData_flg[2]==255)
			AirCon3_SendSetData_buf[3] = 0x00;
		else if(mid_aircon.Qt_AirSetData_flg[2]==1)
			AirCon3_SendSetData_buf[3] = 0x00;
		else if(mid_aircon.Qt_AirSetData_flg[2]==2)
			AirCon3_SendSetData_buf[3] = 0x01;
		else if(mid_aircon.Qt_AirSetData_flg[2]==3)
			AirCon3_SendSetData_buf[3] = 0x08;
		else if(mid_aircon.Qt_AirSetData_flg[2]==4)
			AirCon3_SendSetData_buf[3] = 0x09;
		else if(mid_aircon.Qt_AirSetData_flg[2]==5)
			AirCon3_SendSetData_buf[3] = 0x0C;
		else if(mid_aircon.Qt_AirSetData_flg[2]==6)
			AirCon3_SendSetData_buf[3] = 0x0D;
		else if(mid_aircon.Qt_AirSetData_flg[2]==7)
			AirCon3_SendSetData_buf[3] = 0x10;
		else if(mid_aircon.Qt_AirSetData_flg[2]==8)
			AirCon3_SendSetData_buf[3] = 0x11;

		if(mid_aircon.Qt_AirSetData_flg[2]==255 || (mid_aircon.Qt_AirSetData_flg[2]==1))
		{
			AirCon3_SendSetData_buf[4] = (mid_aircon.AirSet_EnginStartTemp_C*2+180)/256;
			AirCon3_SendSetData_buf[5] = (mid_aircon.AirSet_EnginStartTemp_C*2+180)%256;
		}
		else if(mid_aircon.Qt_AirSetData_flg[2]==3)
		{
			AirCon3_SendSetData_buf[4] = (mid_aircon.AirSet_HertStartTemp_C*2+180)/256;
			AirCon3_SendSetData_buf[5] = (mid_aircon.AirSet_HertStartTemp_C*2+180)%256;
		}

		check16 = mc_check_crc16(AirCon3_SendSetData_buf,6);
		AirCon3_SendSetData_buf[6] = (uint8_t)(check16/256);
		AirCon3_SendSetData_buf[7] = (uint8_t)(check16%256);
		
		if(mid_aircon.Qt_AirSetData_flg[2]==255){
			mid_aircon.Qt_AirSetData_flg[2] = 3;
			mid_aircon.AirSet_EnginStartTemp_C = mid_aircon.AirSet_HertStartTemp_C;
		}	
		else mid_aircon.Qt_AirSetData_flg[2] = 0;
	}
	else if(mid_aircon.Qt_AirOpen_flg[0]>0)
	{	
		AirCon_SendOnOff_flg[0] = 1;
		if(mid_aircon.Qt_AirOpen_flg[0]==1)
		AirCon1_SendOnOff_buf[4] = 0xff;
		else
			AirCon1_SendOnOff_buf[4] = 0;
		mid_aircon.Qt_AirOpen_flg[0] = 0;	
		AirCon1_SendOnOff_buf[5] = 0;				
		check16 = mc_check_crc16(AirCon1_SendOnOff_buf,6);
		AirCon1_SendOnOff_buf[6] = (uint8_t)(check16/256);
		AirCon1_SendOnOff_buf[7] = (uint8_t)(check16%256);
	}
	else if(mid_aircon.Qt_AirOpen_flg[1]>0)
	{	
		AirCon_SendOnOff_flg[1] = 1;
		if(mid_aircon.Qt_AirOpen_flg[1]==1)
		AirCon2_SendOnOff_buf[4] = 0xff;
		else
			AirCon2_SendOnOff_buf[4] = 0;
		mid_aircon.Qt_AirOpen_flg[1] = 0;	
		AirCon2_SendOnOff_buf[5] = 0;				
		check16 = mc_check_crc16(AirCon2_SendOnOff_buf,6);
		AirCon2_SendOnOff_buf[6] = (uint8_t)(check16/256);
		AirCon2_SendOnOff_buf[7] = (uint8_t)(check16%256);
	}

	else if(mid_aircon.Qt_AirOpen_flg[2]>0)
	{		
		AirCon_SendOnOff_flg[2] = 1;
		if(mid_aircon.Qt_AirOpen_flg[2]==1)
		AirCon3_SendOnOff_buf[4] = 0xff;
		else
			AirCon3_SendOnOff_buf[4] = 0;
		mid_aircon.Qt_AirOpen_flg[2] = 0;	
		AirCon3_SendOnOff_buf[5] = 0;	
		check16 = mc_check_crc16(AirCon3_SendOnOff_buf,6);
		AirCon3_SendOnOff_buf[6] = (uint8_t)(check16/256);
		AirCon3_SendOnOff_buf[7] = (uint8_t)(check16%256);
	}
}
/*空调中间层对接函数*/
void mid_AirCon_Interface()
{
	int i = 0;
	for(i=0;i<3;++i)
	{
		AirCon_MerRunSta[i] = mid_aircon.AirCon_MerRunSta[i];
		AirCon_EnginRunSta[i] = mid_aircon.AirCon_EnginRunSta[i];
		AirCon_HertRunSta[i] = mid_aircon.AirCon_HertRunSta[i];
		AirCon_Err1[i] = mid_aircon.AirCon_Err1[i];
		AirCon_Err2[i] = mid_aircon.AirCon_Err2[i];
		AirCon_Err3[i] = mid_aircon.AirCon_Err3[i];
		AirCon_Temp[i] = mid_aircon.AirCon_Temp[i];
		AirCon_Humi[i] = mid_aircon.AirCon_Humi[i];
		AirRead_EnginStartTemp_C[i] = mid_aircon.AirRead_EnginStartTemp_C[i];
		AirRead_HertStartTemp_C[i] = mid_aircon.AirRead_HertStartTemp_C[i];

		if(Qt_AirSetData_flg[i])
		{
			mid_aircon.Qt_AirSetData_flg[i] = Qt_AirSetData_flg[i];
			Qt_AirSetData_flg[i] = 0;
			// printf("mid_aircon.Qt_AirSetData_flg[i] = %d\n\n\n",mid_aircon.Qt_AirSetData_flg[i]);

		}
		if(Qt_AirOpen_flg[i])
		{
			mid_aircon.Qt_AirOpen_flg[i] = Qt_AirOpen_flg[i];
			Qt_AirOpen_flg[i] = 0;
		}
	}
	mid_aircon.AirSet_EnginStartTemp_C = AirSet_EnginStartTemp_C;
	mid_aircon.AirSet_EnginStopTemp_C = AirSet_EnginStopTemp_C;
	mid_aircon.AirSet_HertStartTemp_C = AirSet_HertStartTemp_C;
	mid_aircon.AirSet_HertStopTemp_C = AirSet_HertStopTemp_C;
	mid_aircon.AirSet_FanStartTemp_C = AirSet_FanStartTemp_C;
	mid_aircon.AirSet_FanStopTemp_C = AirSet_FanStopTemp_C;
	mid_aircon.AirSet_ErrOverTemp_C = AirSet_ErrOverTemp_C;
	mid_aircon.AirSet_ErrUnderTemp_C = AirSet_ErrUnderTemp_C;
	
}
void thread_AirCon(void)
{
	//定义sockfd
    int clientsockfd = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in servaddr;
	
	int8_t conn1 = -1;
	int16_t Send_flg;
	uint8_t AirCon_Recv_buf[500];
	int16_t read_size;
	static uint8_t num = 1;

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);  ///服务器端口
    servaddr.sin_addr.s_addr = inet_addr("192.168.1.200");  ///服务器ip
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
		conn1 = connect(clientsockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
		sleep(2);
		conn1 = send(clientsockfd, Send_ReConnect_buf, (1),MSG_DONTWAIT|MSG_NOSIGNAL);
		// if(recv(clientsockfd, AirCon_Recv_buf, BUFFER_SIZE,MSG_DONTWAIT|MSG_NOSIGNAL)>=0)
		// {
		// 	conn1 = 1;
		// }
//		printf("connect AirCon function failed.%d\n",conn1);
		AirCon_conn_flg = 2;
		//sleep(10);
	}
	AirCon_conn_flg = 1;
//	printf("connect AirCon  33333333 OK.\n");

	while(1)
	{
		mid_AirCon_Interface();

		if(mid_aircon.Qt_AirSetData_flg[0]||mid_aircon.Qt_AirSetData_flg[1]||mid_aircon.Qt_AirSetData_flg[2]
		   ||mid_aircon.Qt_AirOpen_flg[0]||mid_aircon.Qt_AirOpen_flg[1]||mid_aircon.Qt_AirOpen_flg[2])
			AirConInter_SetOnOff_Deal();
		else
			AirConInter_Trans_Deal(num);

		if(AirCon_SendSetData_flg[0]==1)
		{
			AirCon_SendSetData_flg[0] = 0;
			int i;
			// for(i=0;i<8;++i)
			// 	printf("%2x ",AirCon1_SendSetData_buf[i]);

			Send_flg = send(clientsockfd, AirCon1_SendSetData_buf, (6+2),MSG_DONTWAIT|MSG_NOSIGNAL); //发送充放电指令-500ms
		}
		else if(AirCon_SendSetData_flg[1]==1)
		{
			AirCon_SendSetData_flg[1] = 0;
			Send_flg = send(clientsockfd, AirCon2_SendSetData_buf, (6+2),MSG_DONTWAIT|MSG_NOSIGNAL); //发送充放电指令-500ms
		}
		else if(AirCon_SendSetData_flg[2]==1)
		{
			AirCon_SendSetData_flg[2] = 0;
			Send_flg = send(clientsockfd, AirCon3_SendSetData_buf, (6+2),MSG_DONTWAIT|MSG_NOSIGNAL); //发送充放电指令-500ms
		}
		else if(AirCon_SendOnOff_flg[0]==1)
		{
			AirCon_SendOnOff_flg[0] = 0;
			Send_flg = send(clientsockfd, AirCon1_SendOnOff_buf, (6+2),MSG_DONTWAIT|MSG_NOSIGNAL); //发送充放电指令-500ms
		}
		else if(AirCon_SendOnOff_flg[1]==1)
		{
			AirCon_SendOnOff_flg[1] = 0;
			Send_flg = send(clientsockfd, AirCon2_SendOnOff_buf, (6+2),MSG_DONTWAIT|MSG_NOSIGNAL); //发送充放电指令-500ms
		}
		else if(AirCon_SendOnOff_flg[2]==1)
		{
			AirCon_SendOnOff_flg[2] = 0;
			Send_flg = send(clientsockfd, AirCon3_SendOnOff_buf, (6+2),MSG_DONTWAIT|MSG_NOSIGNAL); //发送充放电指令-500ms
		}
		else if(AirCon_SendStaData_flg[0]==1)
		{
//			printf("11222222222222222222222222222222222222\n\n");
			
			int i=0;
			for(i=0;i<8;++i)
//				printf("%2x  ",AirCon1_SendStaData_buf[i]);
			AirCon_SendStaData_flg[0] = 0;
			Send_flg = send(clientsockfd, AirCon1_SendStaData_buf, (6+2),MSG_DONTWAIT|MSG_NOSIGNAL); //发送充放电指令-500ms
			num++;
		}	
		else if(AirCon_SendStaData_flg[1]==1)
		{
			AirCon_SendStaData_flg[1] = 0;
			Send_flg = send(clientsockfd, AirCon2_SendStaData_buf, (6+2),MSG_DONTWAIT|MSG_NOSIGNAL); //发送充放电指令-500ms
			num++;
		}	
		else if(AirCon_SendStaData_flg[2]==1)
		{
			AirCon_SendStaData_flg[2] = 0;
			Send_flg = send(clientsockfd, AirCon3_SendStaData_buf, (6+2),MSG_DONTWAIT|MSG_NOSIGNAL); //发送充放电指令-500ms
			num++;
		}	
		else if(AirCon_SendErrData_flg[0]==1)
		{
			AirCon_SendErrData_flg[0] = 0;
			Send_flg = send(clientsockfd, AirCon1_SendErrData_buf, (6+2),MSG_DONTWAIT|MSG_NOSIGNAL); //发送充放电指令-500ms
			num++;
		}
		else if(AirCon_SendErrData_flg[1]==1)
		{
			AirCon_SendErrData_flg[1] = 0;
			Send_flg = send(clientsockfd, AirCon2_SendErrData_buf, (6+2),MSG_DONTWAIT|MSG_NOSIGNAL); //发送充放电指令-500ms
			num++;
		}
		else if(AirCon_SendErrData_flg[2]==1)
		{
			AirCon_SendErrData_flg[2] = 0;
			Send_flg = send(clientsockfd, AirCon3_SendErrData_buf, (6+2),MSG_DONTWAIT|MSG_NOSIGNAL); //发送充放电指令-500ms
			num++;
		}
		else if(AirCon_SendSenserData_flg[0]==1)
		{
			AirCon_SendSenserData_flg[0] = 0;
			Send_flg = send(clientsockfd, AirCon1_SendSenserData_buf, (6+2),MSG_DONTWAIT|MSG_NOSIGNAL); //发送充放电指令-500ms
			num++;
		}
		else if(AirCon_SendSenserData_flg[1]==1)
		{
			AirCon_SendSenserData_flg[1] = 0;
			Send_flg = send(clientsockfd, AirCon2_SendSenserData_buf, (6+2),MSG_DONTWAIT|MSG_NOSIGNAL); //发送充放电指令-500ms
			num++;
		}
		else if(AirCon_SendSenserData_flg[2]==1)
		{
			AirCon_SendSenserData_flg[2] = 0;
			Send_flg = send(clientsockfd, AirCon3_SendSenserData_buf, (6+2),MSG_DONTWAIT|MSG_NOSIGNAL); //发送充放电指令-500ms
			num++;
		}
		else if(AirCon_SendGetData_flg[0]==1)
		{
			AirCon_SendGetData_flg[0] = 0;
			// printf("connect AirCon function failed000000000000000000000000000000.\n\n\n");	
			Send_flg = send(clientsockfd, AirCon1_SendGetData_buf, (6+2),MSG_DONTWAIT|MSG_NOSIGNAL); //发送充放电指令-500ms
			num++;
		}
		else if(AirCon_SendGetData_flg[1]==1)
		{
			AirCon_SendGetData_flg[1] = 0;
			Send_flg = send(clientsockfd, AirCon2_SendGetData_buf, (6+2),MSG_DONTWAIT|MSG_NOSIGNAL); //发送充放电指令-500ms
			num++;
		}
		else if(AirCon_SendGetData_flg[2]==1)
		{
			AirCon_SendGetData_flg[2] = 0;
			Send_flg = send(clientsockfd, AirCon3_SendGetData_buf, (6+2),MSG_DONTWAIT|MSG_NOSIGNAL); //发送充放电指令-500ms
			num = 1;
		}
		// printf("AirConSend_Discon_con324242= %d \n",AirConSend_Discon_con);  
		if(AirCon_Eachconn_sta[0]+AirCon_Eachconn_sta[1]+AirCon_Eachconn_sta[2]==6)
		{			
			AirCon_conn_flg = 2;
			Send_flg = -1;
			// conn1 = connect(clientsockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
			while(Send_flg<=0)
			{
				static int index = 1;
				close(clientsockfd);
				// sleep(1);
				clientsockfd = socket(AF_INET, SOCK_STREAM, 0);
				int flags = fcntl(clientsockfd, F_GETFL, 0);  
				fcntl(clientsockfd, F_SETFL, flags|O_NONBLOCK);
				conn1 = connect(clientsockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
				sleep(2);
				if(1 == index)
				{
					index = 2;
					send(clientsockfd, AirCon1_SendErrData_buf, (6+2),MSG_DONTWAIT|MSG_NOSIGNAL);
				}
				else if(2 == index)
				{
					index = 3;
					send(clientsockfd, AirCon2_SendErrData_buf, (6+2),MSG_DONTWAIT|MSG_NOSIGNAL);
				}
				else if(3 == index)
				{
					index = 1;
					send(clientsockfd, AirCon3_SendErrData_buf, (6+2),MSG_DONTWAIT|MSG_NOSIGNAL);
				}
				
				fprintf(stderr, "connect AirCon function failed.\n");
				syslog(LOG_INFO,"空调connect AirCon function failed Send_flg \n");
				sleep(1);
				Send_flg=(recv(clientsockfd, AirCon_Recv_buf, BUFFER_SIZE,MSG_DONTWAIT|MSG_NOSIGNAL));
				AirCon_conn_flg = 2;
				//sleep(10);
			}	
			AirCon_Rec_Deal(Send_flg,AirCon_Recv_buf);
		}
		else
		{
			AirCon_conn_flg = 1;
		}
		usleep(300*1000);//300ms
//		printf("num = %d\n",num);

		read_size=(recv(clientsockfd, AirCon_Recv_buf, 
				BUFFER_SIZE,MSG_DONTWAIT|MSG_NOSIGNAL));
		if(read_size<0){}
		else
		{
			// Read_Size=Read_Size1;
			AirCon_Rec_Deal(read_size,AirCon_Recv_buf);
		}
	}
	close(clientsockfd);
}
