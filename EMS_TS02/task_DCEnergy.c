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


#define PORT                  502
// #define DCEn_CONTROL_ADDR     1

#define BUFFER_SIZE           100
#define SENDGETIOIN_SIZE         8
#define SENDCONTIOOUT_SIZE         20 
#define SENDCLEARIOOUT_SIZE         20  
int DCEn_CONTROL_ADDR = 1;

uint8_t DCEnCon_SendVolData_buf[500]={0X01,0x03,0x00,0x00,0x00,0x02};//电压
uint8_t DCEnCon_SenCurData_buf[500]={0X01,0x03,0x00,0x0E,0x00,0x02};//电流
uint8_t DCEnCon_SendActData_buf[500]={0X01,0x03,0x00,0x1C,0x00,0x02};//有功功率
uint8_t DCEnCon_Send_Pos_ActData_buf[500]={0X01,0x03,0x00,0x2A,0x00,0x02};//正向有功
uint8_t DCEnCon_Send_Rev_ActData_buf[500]={0X01,0x03,0x00,0x38,0x00,0x02};//反向有功

uint8_t DCEnCon_Send_VolData_flg;
uint8_t DCEnCon_Send_CurData_flg;
uint8_t DCEnCon_SendActData_flg;
uint8_t DCEnCon_Send_Pos_ActData_flg;
uint8_t DCEnCon_Send_Rev_ActData_flg;

// uint8_t DCEnCon_conn_flg = 0;
uint16_t DCEnConSend_Discon_con;

uint8_t Dc_num = 1;
real_T DCEnVol_V[20]={0};
real_T DCEnCur_A[20]={0};
real_T DCEn_Act_W[20]={0};
real_T DCEnPos_Act_W[20]={0};
real_T DCEnNeg_Act_W[20]={0};


// void DCEnConRecv_Server_Deal(uint8_t* recv_buf,uint8_t Dc_num)  //Dc_num:空调123
void DCEnConRecv_Server_Deal(uint8_t* recv_buf)  
{
	uint8_t i=0;
	// Recv_count++;
	switch (Dc_num)
	{
		case 1:
		{			
	 		DCEnVol_V[DCEn_CONTROL_ADDR-1] = (double)((recv_buf[4]<<24+recv_buf[5]<<16+recv_buf[6]<<8+recv_buf[7]))/100;	
		}break;
		case 2:
		{
			DCEnCur_A[DCEn_CONTROL_ADDR-1] = (double)((recv_buf[4]<<24+recv_buf[5]<<16+recv_buf[6]<<8+recv_buf[7]))/1000;
		}break;
		case 3:
		{
			DCEn_Act_W[DCEn_CONTROL_ADDR-1] = (double)((recv_buf[4]<<24+recv_buf[5]<<16+recv_buf[6]<<8+recv_buf[7]))/1000;
		}break;
		case 4:
		{
			DCEnPos_Act_W[DCEn_CONTROL_ADDR-1] = (double)((recv_buf[4]<<24+recv_buf[5]<<16+recv_buf[6]<<8+recv_buf[7]))/10000;
		}break;
		case 5:
		{
			DCEnNeg_Act_W[DCEn_CONTROL_ADDR-1] = (double)((recv_buf[4]<<24+recv_buf[5]<<16+recv_buf[6]<<8+recv_buf[7]))/10000;
		}break;
		// case 10:
		// {
		// 	check16 = mc_check_crc16(DCEnCon1_SendErrData_buf,6);
		// 	DCEnCon1_SendStaData_buf[6] = (uint8_t)(check16/256);
		// 	DCEnCon1_SendStaData_buf[7] = (uint8_t)(check16%256);
		// }break;
		default:
			break;
		}
}
void DCEnCon_Rec_Deal(int16_t Read_Size,uint8_t* recv_buf)
{
	uint16_t i=0,rec_calchkval,my_calchkval;
	uint16_t RecvData_dlc;
	uint8_t buf[500];
	if((recv_buf[0]==1)&&(recv_buf[1]==3))
	{
		RecvData_dlc = recv_buf[2];
		rec_calchkval = mc_check_crc16(recv_buf,RecvData_dlc+3);
		my_calchkval = (recv_buf[RecvData_dlc+3]*256+recv_buf[RecvData_dlc+4]);
		if((my_calchkval==rec_calchkval))
		{			
			// DCEnConRecv_Server_Deal(recv_buf,(recv_buf[2]));
			 DCEnConRecv_Server_Deal(recv_buf);
		}	
	}
	bzero(recv_buf, sizeof(recv_buf));
}
void DCEnConInter_Trans_Deal(uint8_t Dc_num)
{
	uint16_t check16=0;
	switch (Dc_num)
	{
		case 1:
		{
			DCEnCon_Send_VolData_flg = 1;
			DCEnCon_SendVolData_buf[0] = DCEn_CONTROL_ADDR;
			check16 = mc_check_crc16(DCEnCon_SendVolData_buf,6);
			DCEnCon_SendVolData_buf[6] = (uint8_t)(check16/256);
			DCEnCon_SendVolData_buf[7] = (uint8_t)(check16%256);
		}break;
		case 2:
		{
			DCEnCon_Send_CurData_flg = 1;
			DCEnCon_SenCurData_buf[0] = DCEn_CONTROL_ADDR;
			check16 = mc_check_crc16(DCEnCon_SenCurData_buf,6);
			DCEnCon_SenCurData_buf[6] = (uint8_t)(check16/256);
			DCEnCon_SenCurData_buf[7] = (uint8_t)(check16%256);
		}break;
		case 3:
		{		
			DCEnCon_SendActData_flg = 1;
			DCEnCon_SendActData_buf[0] = DCEn_CONTROL_ADDR;	
			check16 = mc_check_crc16(DCEnCon_SendActData_buf,6);
			DCEnCon_SendActData_buf[6] = (uint8_t)(check16/256);
			DCEnCon_SendActData_buf[7] = (uint8_t)(check16%256);
		}break;
		case 4:
		{
			DCEnCon_Send_Pos_ActData_flg = 1;
			DCEnCon_Send_Pos_ActData_buf[0] = DCEn_CONTROL_ADDR;	
			check16 = mc_check_crc16(DCEnCon_Send_Pos_ActData_buf,6);
			DCEnCon_Send_Pos_ActData_buf[6] = (uint8_t)(check16/256);
			DCEnCon_Send_Pos_ActData_buf[7] = (uint8_t)(check16%256);
		}break;
		case 5:
		{
			DCEnCon_Send_Rev_ActData_flg = 1;
			DCEnCon_Send_Rev_ActData_buf[0] = DCEn_CONTROL_ADDR;
			check16 = mc_check_crc16(DCEnCon_Send_Rev_ActData_buf,6);
			DCEnCon_Send_Rev_ActData_buf[6] = (uint8_t)(check16/256);
			DCEnCon_Send_Rev_ActData_buf[7] = (uint8_t)(check16%256);
		}break;
		// case 10:
		// {
		// 	check16 = mc_check_crc16(DCEnCon1_SendErrData_buf,6);
		// 	DCEnCon1_SendStaData_buf[6] = (uint8_t)(check16/256);
		// 	DCEnCon1_SendStaData_buf[7] = (uint8_t)(check16%256);
		// }break;
		default:
			break;
	}
}
uint8_t DCEnCon_conn_flg;
void thread_DCEnCon(void)
{
//	printf("thread_DCEnCon tid %d\n",gettid());
	//定义sockfd
    int clientsockfd = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in servaddr;
	
	int8_t conn1 = -1;
	int16_t Send_flg;
	uint8_t DCEnCon_Recv_buf[500];
	int16_t read_size;
	// static uint8_t Dc_num = 1;

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);  ///服务器端口
    servaddr.sin_addr.s_addr = inet_addr("192.168.1.254");  ///服务器ip 202

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
		// if(recv(clientsockfd, DCEnCon_Recv_buf, BUFFER_SIZE,MSG_DONTWAIT|MSG_NOSIGNAL)>=0)
		// {
		// 	conn1 = 1;
		// }
//		printf("connect DCEnCon function failed.%d\n",conn1);
		DCEnCon_conn_flg = 2;
		//sleep(10);
	}
	DCEnCon_conn_flg = 1;
	fprintf(stderr, "connect DCEnCon OK.\n");

	while(1)
	{
		DCEnConInter_Trans_Deal(Dc_num);
		if(DCEnCon_Send_VolData_flg==1)
		{
			DCEnCon_Send_VolData_flg = 0;
			Send_flg = send(clientsockfd, DCEnCon_SendVolData_buf, (6+2),MSG_DONTWAIT|MSG_NOSIGNAL); //发送充放电指令-500ms
			Dc_num++;
		}	
		if(DCEnCon_Send_CurData_flg==1)
		{
			DCEnCon_Send_CurData_flg = 0;
			Send_flg = send(clientsockfd, DCEnCon_SenCurData_buf, (6+2),MSG_DONTWAIT|MSG_NOSIGNAL); //发送充放电指令-500ms
			Dc_num++;
		}
		if(DCEnCon_SendActData_flg==1)
		{
			DCEnCon_SendActData_flg = 0;
			Send_flg = send(clientsockfd, DCEnCon_SendActData_buf, (6+2),MSG_DONTWAIT|MSG_NOSIGNAL); //发送充放电指令-500ms
			Dc_num++;
		}	
		if(DCEnCon_Send_Pos_ActData_flg==1)
		{
			DCEnCon_Send_Pos_ActData_flg = 0;
			Send_flg = send(clientsockfd, DCEnCon_Send_Pos_ActData_buf, (6+2),MSG_DONTWAIT|MSG_NOSIGNAL); //发送充放电指令-500ms
			Dc_num++;
		}	
		if(DCEnCon_Send_Rev_ActData_flg==1)
		{
			DCEnCon_Send_Rev_ActData_flg = 0;
			Send_flg = send(clientsockfd, DCEnCon_Send_Rev_ActData_buf, (6+2),MSG_DONTWAIT|MSG_NOSIGNAL); //发送充放电指令-500ms
			Dc_num = 1;
			DCEn_CONTROL_ADDR++;
			if (DCEn_CONTROL_ADDR==20)
			{
				DCEn_CONTROL_ADDR = 1;
			}
		}

		if(DCEnConSend_Discon_con>=15)
		{			
			DCEnCon_conn_flg = 2;
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

				fprintf(stderr, "connect DCEnCon function failed.\n");
				DCEnCon_conn_flg = 2;
				//sleep(10);
			}	
			DCEnConSend_Discon_con = 0;	
		}
		else
		{
			DCEnCon_conn_flg = 1;
		}
		read_size=(recv(clientsockfd, DCEnCon_Recv_buf, BUFFER_SIZE,MSG_DONTWAIT|MSG_NOSIGNAL));
		if(read_size<0){}
		else{
			// Read_Size=Read_Size1;
			DCEnCon_Rec_Deal(read_size,DCEnCon_Recv_buf);
			DCEnConSend_Discon_con = 0;
		}
		usleep(1000*1000);//1000ms
	}
	close(clientsockfd);
}


