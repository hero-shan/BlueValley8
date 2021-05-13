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
#define ACEn_CONTROL_ADDR     1

#define BUFFER_SIZE           200
#define SENDGETIOIN_SIZE        8
#define SENDCONTIOOUT_SIZE     20
#define SENDCLEARIOOUT_SIZE    20

mid_AcEnger_t mid_AcEnger;

uint8_t ACEnCon_SendActData_buf[500]={ACEn_CONTROL_ADDR,0x03,0x00,0x00,0x00,0x3C};		//有功/无功电能
uint8_t ACEnCon_SendVolCurData_buf[500]={ACEn_CONTROL_ADDR,0x03,0x00,0x61,0x00,0x06};	//单相电压电流
uint8_t ACEnCon_SendPTCTData_buf[500]={ACEn_CONTROL_ADDR,0x03,0x00,0x8D,0x00,0x08};		//PTCT参数,DIO状态
uint8_t ACEnCon_SendPowerData_buf[500]={ACEn_CONTROL_ADDR,0x03,0x01,0x64,0x00,0x1C};	//单相功率
uint8_t ACEnCon_SendSetPt_buf[500]={ACEn_CONTROL_ADDR,0x10,0x00,0x8D,0x00,0x01,0x02};	//设置PT参数
uint8_t ACEnCon_SendSetCt_buf[500]={ACEn_CONTROL_ADDR,0x10,0x00,0x8E,0x00,0x01,0x02};	//设置CT参数

uint8_t ACEnCon_A_OverVolt_flg=0;
uint8_t ACEnCon_B_OverVolt_flg=0;
uint8_t ACEnCon_C_OverVolt_flg=0;
uint8_t ACEnCon_A_UnderVolt_flg=0;
uint8_t ACEnCon_B_UnderVolt_flg=0;
uint8_t ACEnCon_C_UnderVolt_flg=0;
uint8_t ACEnCon_A_RevDirc_flg=0;
uint8_t ACEnCon_B_RevDirc_flg=0;
uint8_t ACEnCon_C_RevDirc_flg=0;
uint8_t ACEnCon_DI_State=0;
uint8_t ACEnCon_DO_State=0;

uint8_t ACEnCon_SendActData_flg;
uint8_t ACEnCon_SendVolCurData_flg;
uint8_t ACEnCon_SendPTCTData_flg;
uint8_t ACEnCon_SendPowerData_flg;
uint8_t ACEnCon_SendSetPt_flg=0;
uint8_t ACEnCon_SendSetCt_flg=0;

uint16_t ACEnConSend_Discon_con;
uint8_t ACEnCon_GetFire_flg;

uint8_t num = 1;


void ACEnConRecv_Server_Deal(uint8_t* recv_buf)  //num:空调123
{
	uint8_t i=0;
	switch (num)
	{
		case 1:
		{
			mid_AcEnger.ac_GroupActiveTotalEnergy      = (double)((recv_buf[3]<<24)+(recv_buf[4]<<16)+(recv_buf[5]<<8)+(recv_buf[6]))*0.01*mid_AcEnger.acPt*mid_AcEnger.acCt;			//当前组合有功总电能
			mid_AcEnger.ac_PositiveActiveTotalEnergy   = (double)((recv_buf[23]<<24)+(recv_buf[24]<<16)+(recv_buf[25]<<8)+(recv_buf[26]))*0.01*mid_AcEnger.acPt*mid_AcEnger.acCt;		//当前正向总有功电能
			mid_AcEnger.ac_NegativeActiveTotalEnergy   = (double)((recv_buf[43]<<24)+(recv_buf[44]<<16)+(recv_buf[45]<<8)+(recv_buf[46]))*0.01*mid_AcEnger.acPt*mid_AcEnger.acCt;		//当前反向总有功电能
			mid_AcEnger.ac_GroupReactiveTotalEnergy    = (double)((recv_buf[63]<<24)+(recv_buf[64]<<16)+(recv_buf[65]<<8)+(recv_buf[66]))*0.01*mid_AcEnger.acPt*mid_AcEnger.acCt;		//当前组合无功总电能
			mid_AcEnger.ac_PositiveReactiveTotalEnergy = (double)((recv_buf[83]<<24)+(recv_buf[84]<<16)+(recv_buf[85]<<8)+(recv_buf[86]))*0.01*mid_AcEnger.acPt*mid_AcEnger.acCt;   	//当前正向总无功电能
			mid_AcEnger.ac_NegativeReactiveTotalEnergy = (double)((recv_buf[103]<<24)+(recv_buf[104]<<16)+(recv_buf[105]<<8)+(recv_buf[106]))*0.01*mid_AcEnger.acPt*mid_AcEnger.acCt; 	//当前反向总无功电能
		}break;
		case 2:
		{
			mid_AcEnger.acA_Voltage = (double)(recv_buf[3]*256+recv_buf[4])*mid_AcEnger.acPt*0.1 ;		//A相电压
 			mid_AcEnger.acB_Voltage = (double)(recv_buf[5]*256+recv_buf[6])*mid_AcEnger.acPt*0.1 ;		//B相电压
		 	mid_AcEnger.acC_Voltage = (double)(recv_buf[7]*256+recv_buf[8])*mid_AcEnger.acPt*0.1 ;		//C相电压
 			mid_AcEnger.acA_Current = (double)(recv_buf[9]*256+recv_buf[10])*mid_AcEnger.acCt*0.01 ;		//A相电流
 			mid_AcEnger.acB_Current = (double)(recv_buf[11]*256+recv_buf[12])*mid_AcEnger.acCt*0.01 ;		//B相电流
 			mid_AcEnger.acC_Current = (double)(recv_buf[13]*256+recv_buf[14])*mid_AcEnger.acCt*0.01 ;		//C相电流

			//  printf("currentcurr111111 %d %d %d %d\n",recv_buf[7],recv_buf[8],recv_buf[9],recv_buf[10]);
		}break;
		case 3:
		{
			mid_AcEnger.acPt = (double)(recv_buf[3]*256+recv_buf[4]);		//电压变比PT
			mid_AcEnger.acCt = (double)(recv_buf[5]*256+recv_buf[6]);		//电流变比CT
			// ACEnCon_A_OverVolt_flg=recv_buf[5]&0x01;	//A相过压
			// ACEnCon_B_OverVolt_flg=recv_buf[5]&0x02;	//B相过压
			// ACEnCon_C_OverVolt_flg=recv_buf[5]&0x04;	//C相过压
			// ACEnCon_A_UnderVolt_flg=recv_buf[5]&0x08;	//A相失压
			// ACEnCon_B_UnderVolt_flg=recv_buf[5]&0x10;	//B相失压
			// ACEnCon_C_UnderVolt_flg=recv_buf[5]&0x20;	//C相失压
			// ACEnCon_A_RevDirc_flg=recv_buf[5]&0x40;	//A相逆向
			// ACEnCon_B_RevDirc_flg=recv_buf[5]&0x80;	//B相逆向
			// ACEnCon_A_RevDirc_flg=recv_buf[4]&0x01;	//C相逆向
			// ACEnCon_DI_State=recv_buf[4]&0x01;			//DI状态
			// ACEnCon_DI_State=recv_buf[4]&0x01;			//DO状态

			mid_AcEnger.ac_Err = (double)(recv_buf[7]*256+recv_buf[8]);

		}break;
		case 4:
		{
			mid_AcEnger.acA_ActivePower      =  (double)(((int32_t)recv_buf[3]<<24)+((int32_t)recv_buf[4]<<16)+((int32_t)recv_buf[5]<<8)+((int32_t)recv_buf[6]))*mid_AcEnger.acPt*mid_AcEnger.acCt*0.001;			//A 相有功功率
			mid_AcEnger.acB_ActivePower      =  (double)(((int32_t)recv_buf[7]<<24)+((int32_t)recv_buf[8]<<16)+((int32_t)recv_buf[9]<<8)+((int32_t)recv_buf[10]))*mid_AcEnger.acPt*mid_AcEnger.acCt*0.001; 	 		//B 相有功功率
			mid_AcEnger.acC_ActivePower      =  (double)(((int32_t)recv_buf[11]<<24)+((int32_t)recv_buf[12]<<16)+((int32_t)recv_buf[13]<<8)+((int32_t)recv_buf[14]))*mid_AcEnger.acPt*mid_AcEnger.acCt*0.001;		//C 相有功功率
			mid_AcEnger.acA_ReactivePower    =  (double)(((int32_t)recv_buf[19]<<24)+((int32_t)recv_buf[20]<<16)+((int32_t)recv_buf[21]<<8)+((int32_t)recv_buf[22]))*mid_AcEnger.acPt*mid_AcEnger.acCt*0.001;		//A 相无功功率
			mid_AcEnger.acB_ReactivePower    =  (double)(((int32_t)recv_buf[23]<<24)+((int32_t)recv_buf[24]<<16)+((int32_t)recv_buf[25]<<8)+((int32_t)recv_buf[26]))*mid_AcEnger.acPt*mid_AcEnger.acCt*0.001;		//B 相无功功率
			mid_AcEnger.acC_ReactivePower    =  (double)(((int32_t)recv_buf[27]<<24)+((int32_t)recv_buf[28]<<16)+((int32_t)recv_buf[29]<<8)+((int32_t)recv_buf[30]))*mid_AcEnger.acPt*mid_AcEnger.acCt*0.001;		//C 相无功功率
			mid_AcEnger.acA_PhasePowerFactor =  (double)((int16_t)recv_buf[47]*256+(int16_t)recv_buf[48])*0.001;	//A 相功率因数
			mid_AcEnger.acB_PhasePowerFactor =  (double)((int16_t)recv_buf[49]*256+(int16_t)recv_buf[50])*0.001;	//B 相功率因数
			mid_AcEnger.acC_PhasePowerFactor =  (double)((int16_t)recv_buf[51]*256+(int16_t)recv_buf[52])*0.001;	//C 相功率因数
		}break;
		// case 10:
		// {
		// 	check16 = mc_check_crc16(ACEnCon1_SendErrData_buf,6);
		// 	ACEnCon1_SendStaData_buf[6] = (uint8_t)(check16/256);
		// 	ACEnCon1_SendStaData_buf[7] = (uint8_t)(check16%256);
		// }break;
		default:
			break;
		}
	// switch (num)  //0x0103020003b844
	// 	{
	// 	case 0x2C:
	// 		printf("ACEn_sta111%x\n",recv_buf[0]);
	// 		ACEnCon_MerRunSta[num] = (recv_buf[3]&0x01);
	// 		break;
	// 	case 0x2E:    //errData
	// 	{
	// 		printf("ACEn_err222%x\n",recv_buf[0]);
	// 		ACEnCon_Err1[num] = recv_buf[3];
	// 	}
	// 	   break;
	// 	case 0x0C:    //多个参数设置
	// 	{
	// 		printf("ACEn_err333%x\n",recv_buf[0]);
	// 	}
	// 	   break;
	// 	case 0x06:    //SenserData
	// 	{
	// 		// ACEnCon_Temp[num] = ((recv_buf[15]*256+recv_buf[16])-180)/2;
	// 		printf("ACEn_err444%x\n",recv_buf[0]);
	// 	}
	// 		break;
	// 	case 0x40:    //控制参数
	// 	{
	// 		printf("ACEn_err555%x\n",recv_buf[0]);
	// 	}break;
	// 	default:
	// 		break;
	// }
}

void ACEnCon_Rec_Deal(int16_t Read_Size,uint8_t* recv_buf)
{
	uint16_t i=0,rec_calchkval,my_calchkval;
	uint16_t RecvData_dlc;
	uint8_t buf[500];
	if((recv_buf[0]==1)&&((recv_buf[1]==3)||(recv_buf[1]==0x10)))
	{
		if(recv_buf[1]==3)
		{
			RecvData_dlc = recv_buf[2];
			rec_calchkval = mc_check_crc16(recv_buf,RecvData_dlc+3);
			my_calchkval = (recv_buf[RecvData_dlc+3]*256+recv_buf[RecvData_dlc+4]);
			if((my_calchkval==rec_calchkval))
			{
				ACEnConRecv_Server_Deal(recv_buf);   //fault
			}
		}
		else if(recv_buf[1]==0x10)
		{
//			printf("AC parameter download successful!\n");
		}
		else
		{
//			printf("Reading Data fault!\n");
		}
	}
	bzero(recv_buf, sizeof(recv_buf));
}

void ACEnConInter_Trans_Deal(uint8_t num)
{
	uint16_t check16=0;
	ACEnCon_SendSetPt_buf[7]=(uint8_t)(Qt_ACEnergySetPT_tmp*1)/256;
	ACEnCon_SendSetPt_buf[8]=(uint8_t)(Qt_ACEnergySetPT_tmp*1)%256;
	check16 = mc_check_crc16(ACEnCon_SendSetPt_buf,9);
	ACEnCon_SendSetPt_buf[9] = (uint8_t)(check16/256);
	ACEnCon_SendSetPt_buf[10] = (uint8_t)(check16%256);

	ACEnCon_SendSetCt_buf[7]=(uint8_t)((Qt_ACEnergySetCT_tmp*1)/256);
	ACEnCon_SendSetCt_buf[8]=(uint8_t)((Qt_ACEnergySetCT_tmp*1)%256);
	check16 = mc_check_crc16(ACEnCon_SendSetCt_buf,9);
	ACEnCon_SendSetCt_buf[9] = (uint8_t)(check16/256);
	ACEnCon_SendSetCt_buf[10] = (uint8_t)(check16%256);
	
	switch (num)
	{
		case 1:
				{
					ACEnCon_SendActData_flg = 1;
					check16 = mc_check_crc16(ACEnCon_SendActData_buf,6);
					ACEnCon_SendActData_buf[6] = (uint8_t)(check16/256);
					ACEnCon_SendActData_buf[7] = (uint8_t)(check16%256);
				}break;
		case 2:
				{
					ACEnCon_SendVolCurData_flg = 1;
					check16 = mc_check_crc16(ACEnCon_SendVolCurData_buf,6);
					ACEnCon_SendVolCurData_buf[6] = (uint8_t)(check16/256);
					ACEnCon_SendVolCurData_buf[7] = (uint8_t)(check16%256);
				}break;
		case 3:
				{
					ACEnCon_SendPTCTData_flg = 1;
					check16 = mc_check_crc16(ACEnCon_SendPTCTData_buf,6);
					ACEnCon_SendPTCTData_buf[6] = (uint8_t)(check16/256);
					ACEnCon_SendPTCTData_buf[7] = (uint8_t)(check16%256);
				}break;
		case 4:
				{
					ACEnCon_SendPowerData_flg = 1;
					check16 = mc_check_crc16(ACEnCon_SendPowerData_buf,6);
					ACEnCon_SendPowerData_buf[6] = (uint8_t)(check16/256);
					ACEnCon_SendPowerData_buf[7] = (uint8_t)(check16%256);
				}break;
		default:
				break;
	}
}

void ACEnRev_Buf(int clientsockfd,uint8_t *recv_buff,int8_t *conn1)
{
	int8_t read_size=-1;
	int8_t timeout=0;
	while(read_size<0)
	{
		usleep(50*1000);
		read_size=recv(clientsockfd,recv_buff,BUFFER_SIZE,MSG_DONTWAIT|MSG_NOSIGNAL);
		timeout++;
		if(read_size>0)break;
		if(timeout>=4)
		{
			*conn1=-1;
			return;
		}
	}
	ACEnCon_Rec_Deal(read_size,recv_buff);
	ACEnConSend_Discon_con = 0;
}

void mid_AcEnger_Interface(){

	ac_GroupActiveTotalEnergy = mid_AcEnger.ac_GroupActiveTotalEnergy;
	ac_PositiveActiveTotalEnergy = mid_AcEnger.ac_PositiveActiveTotalEnergy;
	ac_NegativeActiveTotalEnergy = mid_AcEnger.ac_NegativeActiveTotalEnergy;
	ac_GroupReactiveTotalEnergy = mid_AcEnger.ac_GroupReactiveTotalEnergy;
	ac_PositiveReactiveTotalEnergy = mid_AcEnger.ac_PositiveReactiveTotalEnergy;
	ac_NegativeReactiveTotalEnergy = mid_AcEnger.ac_NegativeReactiveTotalEnergy;
	acA_Voltage = mid_AcEnger.acA_Voltage;
	acB_Voltage = mid_AcEnger.acB_Voltage;
	acC_Voltage = mid_AcEnger.acC_Voltage;
	acA_Current = mid_AcEnger.acA_Current;
	acB_Current = mid_AcEnger.acB_Current;
	acC_Current = mid_AcEnger.acC_Current;
	acPt = mid_AcEnger.acPt;
	acCt = mid_AcEnger.acCt;
	acA_ActivePower = mid_AcEnger.acA_ActivePower;
	acB_ActivePower = mid_AcEnger.acB_ActivePower;
	acC_ActivePower = mid_AcEnger.acC_ActivePower;
	acA_ReactivePower = mid_AcEnger.acA_ReactivePower;
	acB_ReactivePower = mid_AcEnger.acB_ReactivePower;
	acC_ReactivePower = mid_AcEnger.acC_ReactivePower;
	acA_PhasePowerFactor = mid_AcEnger.acA_PhasePowerFactor;
	acB_PhasePowerFactor = mid_AcEnger.acB_PhasePowerFactor;
	acC_PhasePowerFactor = mid_AcEnger.acC_PhasePowerFactor;
	ac_Err = mid_AcEnger.ac_Err;
	// mid_AcEnger.ac_SetPtParameter=Qt_ACEnergySetPT_flg; //设置PT参数标志
	// mid_AcEnger.ac_SetCtParameter=Qt_ACEnergySetCT_flg; //设置PT参数标志
	
	// printf("%.2f %.2f %.2f %.2f %.2f %.2f\n",ac_GroupActiveTotalEnergy,ac_PositiveActiveTotalEnergy,
	// ac_NegativeActiveTotalEnergy,ac_GroupReactiveTotalEnergy,ac_PositiveReactiveTotalEnergy,ac_NegativeReactiveTotalEnergy);
}

uint8_t ACEnCon_conn_flg;

void thread_ACEnCon(void)
{
	//定义sockfd
    int clientsockfd = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in servaddr;

	int8_t conn1 = -1;
	int16_t Send_flg;
	uint8_t ACEnCon_Recv_buf[500];
	int16_t read_size;

	bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family=AF_INET;
    servaddr.sin_port=htons(PORT);
    if(1)
        servaddr.sin_addr.s_addr=inet_addr("192.168.1.203");
    else
        servaddr.sin_addr.s_addr=inet_addr("192.168.0.178");
	conn1 = connect(clientsockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
	if(conn1<0)
    {
        close(clientsockfd);
        sleep(1);
        clientsockfd=socket(AF_INET,SOCK_STREAM,0);
        conn1=connect(clientsockfd,(struct sockaddr *)&servaddr,sizeof(servaddr));
        sleep(2);
		conn1=send(clientsockfd,Send_ReConnect_buf,(1),MSG_DONTWAIT|MSG_NOSIGNAL);
//		printf("Connect ACEnCon  fail !\n");
		ACEnCon_conn_flg = 2;
    }
	ACEnCon_conn_flg = 1;
//	fprintf(stderr, "connect ACEnCon OK\n");

	while(1)
	{
		mid_AcEnger_Interface();
		ACEnConInter_Trans_Deal(num);
		if(Qt_ACEnergySetPT_flg==1)
		{
//			printf("ok\n");
			Qt_ACEnergySetPT_flg=0;
			uint8_t SendPt_flg=0;
			SendPt_flg=send(clientsockfd, ACEnCon_SendSetPt_buf, (11),MSG_DONTWAIT|MSG_NOSIGNAL);
//			printf("SendPt_flg=%d\n",SendPt_flg);
		}
		if(Qt_ACEnergySetCT_flg==1)
		{
			Qt_ACEnergySetCT_flg=0;
			uint8_t SendCt_flg=0;
			SendCt_flg=send(clientsockfd, ACEnCon_SendSetCt_buf, (11),MSG_DONTWAIT|MSG_NOSIGNAL);
			// printf("SendCt_flg=%d\n",SendCt_flg);
		}
		if(ACEnCon_SendActData_flg==1)
		{
			ACEnCon_SendActData_flg = 0;
			Send_flg = send(clientsockfd, ACEnCon_SendActData_buf, (8),MSG_DONTWAIT|MSG_NOSIGNAL);
			// printf("acenconacencon2222222222222222222222222222222 %s\n",ACEnCon_SendActData_buf);
			ACEnRev_Buf(clientsockfd,ACEnCon_Recv_buf,&conn1);
			num++;
		}
		if(ACEnCon_SendVolCurData_flg==1)
		{
			ACEnCon_SendVolCurData_flg = 0;
			Send_flg = send(clientsockfd, ACEnCon_SendVolCurData_buf, (8),MSG_DONTWAIT|MSG_NOSIGNAL);
			ACEnRev_Buf(clientsockfd,ACEnCon_Recv_buf,&conn1);
			num++;
		}
		if(ACEnCon_SendPTCTData_flg==1)
		{
			ACEnCon_SendPTCTData_flg = 0;
			Send_flg = send(clientsockfd, ACEnCon_SendPTCTData_buf, (8),MSG_DONTWAIT|MSG_NOSIGNAL);
			ACEnRev_Buf(clientsockfd,ACEnCon_Recv_buf,&conn1);
			num++;
		}
		if(ACEnCon_SendPowerData_flg==1)
		{
			ACEnCon_SendPowerData_flg = 0;
			Send_flg = send(clientsockfd, ACEnCon_SendPowerData_buf, (8),MSG_DONTWAIT|MSG_NOSIGNAL);
			ACEnRev_Buf(clientsockfd,ACEnCon_Recv_buf,&conn1);
			num=1;
		}
		while(ACEnConSend_Discon_con>=15)
		{
			conn1=-1;
			Send_flg=-1;
			// ACEnConSend_Discon_con++;
			while(conn1<=0)
			{
				close(clientsockfd);
				sleep(1);
				clientsockfd = socket(AF_INET, SOCK_STREAM, 0);
				int flags = fcntl(clientsockfd, F_GETFL, 0);
				fcntl(clientsockfd, F_SETFL, flags|O_NONBLOCK);
				Send_flg = connect(clientsockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
				sleep(2);
				conn1 = send(clientsockfd, ACEnCon_SendActData_buf, (8),MSG_DONTWAIT|MSG_NOSIGNAL);
				usleep(500*1000);
				conn1=recv(clientsockfd,ACEnCon_Recv_buf,BUFFER_SIZE,MSG_DONTWAIT|MSG_NOSIGNAL);
				// if(read_size<0) fprintf(stderr, "connect ACEnCon function failed\n");
				// else
				// {
				// 	conn1=0;
				// }
				syslog(LOG_INFO,"交流电表connect ACEnCon function failed Send_flg \n");
//				fprintf(stderr, "connect ACEnCon function failed\n");
				ACEnCon_conn_flg = 2;
			}
			ACEnConSend_Discon_con = 0;
		}
		ACEnCon_conn_flg = 1;
		usleep(300*1000);//1000ms
	}
	close(clientsockfd);
}