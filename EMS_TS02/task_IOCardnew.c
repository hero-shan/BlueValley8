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
#define BUFFER_SIZE           1000
#define SENDGETIOIN_SIZE         10
#define SENDCONTIOOUT_SIZE         20 
#define SENDCLEARIOOUT_SIZE         20  

 
mid_iocard_t mid_iocard;


uint8_t IOCard_SendGetIOIn_buf[500]={0x0};
uint8_t IOCard_SendGetIOOut_buf[500]={0x0};
uint8_t IOCard_SendContIOOut_buf[500]={0x0};
uint8_t IOCard_SendClearIOOut_buf[500]={0x0};
uint8_t CE_CEData_buf[500];
//uint8_t IOCard_Recv_buf[500];
uint8_t IOCard_AllData_buf[500];
uint8_t IOCard_Data3_buf[500];
uint16_t IOCard_RecvData_dlc,IOCard_Buf1_dlc;
uint8_t IOCard_Recv_buf[500];

uint16_t IOCardSend_Discon_con[2];

uint8_t EMS_SysStartStop_sta;
uint8_t EMS_SelfSet_flg;
uint8_t EMS_HandSet_flg;
uint8_t EMS_SelfRun_flg;
u_int32_t Recv_count;
int32_t Read_Size;
int32_t Read_Size1;

// uint8_t IO_DoorValue1_null;
// uint8_t IO_DoorValue2_null;
// uint8_t IO_DoorValue3_null;
// uint8_t IO_ThunderValue1_null;
// uint8_t IO_ThunderValue2_null;
// uint8_t IO_WaterValue_null;
// uint8_t IO_LightValue1_null;
// uint8_t IO_LightValue2_null;

uint8_t Fan_Start_flg;
uint8_t Sound_Start_flg;
uint8_t Light_Start_flg;
uint8_t DistLed_Start_flg; // uint8_t Led_StartLast_flg=0;
uint8_t CharLed_Start_flg;

uint8_t Door_Start_flg;
uint8_t Door_Close_flg;
uint8_t Door2_Start_flg;
uint8_t Door2_Close_flg;

// uint8_t Fan_StartLast_flg;
// uint8_t Sound_StartLast_flg;
// uint8_t Light_StartLast_flg;
// uint8_t Door_StartLast_flg;
// uint8_t Door2_StartLast_flg;

uint8_t Send_GetIOIn_flg;
uint8_t Send_GetIOOut_flg;
uint8_t Send_ContIOOut_flg;

uint8_t Send_IOinputstate_buf[8]={0x0};
uint8_t Send_IOoutputstate_buf[8]={0x0};
uint8_t Send_IOctrloutput_buf[8]={0x0};
uint8_t IOCard_InputPinState[2];
uint8_t IOCard_OutputPinState[2];
uint8_t IO1_ContOutData;
uint8_t IO2_ContOutData;

void IoRecv_Server_Deal(uint16_t dlc)  
{
	uint8_t i=0;
	Recv_count++;
	uint8_t subAddr=IOCard_Recv_buf[0];
	uint8_t funcCode=IOCard_Recv_buf[1];
	switch (funcCode)
	{
		case 1: {
//					IOCard_OutputPinState[subAddr-1]=IOCard_Recv_buf[3];
					IOCard_OutputPinState[0]=IOCard_Recv_buf[3];
	//				printf("IOCard_OutputPinState[%d]=%d\n",subAddr,IOCard_OutputPinState[subAddr-1]);
				}break;
		case 2: {
//					IOCard_InputPinState[subAddr-1]=IOCard_Recv_buf[3];
					IOCard_InputPinState[0]=IOCard_Recv_buf[3];  //开关量输入
	//				printf("IOCard_InputPinState[%d]=%d\n",subAddr,IOCard_InputPinState[subAddr-1]);
				}break;
		case 15:{
	//				printf("Set IO_%d successful\n",subAddr);
				}break;
		default:
			break;
	}
}
void IOCard_Rec_Deal(int16_t Read_Size)
{
	uint16_t rec_calchkval,my_calchkval;
	if((IOCard_Recv_buf[0]==0x01||IOCard_Recv_buf[0]==0x11)
	&&(IOCard_Recv_buf[1]==0x01||IOCard_Recv_buf[1]==0x02||IOCard_Recv_buf[1]==0x0f))
	{
		if(IOCard_Recv_buf[0]==0x11)IOCardSend_Discon_con[0]=0;
//		if(IOCard_Recv_buf[0]==0x02)IOCardSend_Discon_con[1]=0;

		if(IOCard_Recv_buf[1]==0x01||IOCard_Recv_buf[1]==0x02)
		{
			rec_calchkval = mc_check_crc16(IOCard_Recv_buf,4);
			my_calchkval = (IOCard_Recv_buf[4]*256+IOCard_Recv_buf[5]);
			if(my_calchkval==rec_calchkval)
			{			
				IoRecv_Server_Deal(IOCard_RecvData_dlc);
			}
		}
		if(IOCard_Recv_buf[1]==0x0f)
		{
			rec_calchkval = mc_check_crc16(IOCard_Recv_buf,6);
			my_calchkval = (IOCard_Recv_buf[6]*256+IOCard_Recv_buf[7]);
			if(my_calchkval==rec_calchkval)
			{			
				IoRecv_Server_Deal(IOCard_RecvData_dlc);
			}
		}
	}
	bzero(IOCard_Recv_buf, sizeof(IOCard_Recv_buf));
}
void IOcard_build_getIn_buf(int8_t subaddr)  //开关量输入
{
	int Crc16_Val=0;
	Send_GetIOIn_flg = 1;
	IOCard_SendGetIOIn_buf[0]=subaddr;
	IOCard_SendGetIOIn_buf[1]=0x02;   //function code
	IOCard_SendGetIOIn_buf[2]=0x00;
	IOCard_SendGetIOIn_buf[3]=0x20;   //input register address
	IOCard_SendGetIOIn_buf[4]=0x00;
	IOCard_SendGetIOIn_buf[5]=0x08;   //input register number
	Crc16_Val = mc_check_crc16(IOCard_SendGetIOIn_buf,6);
	IOCard_SendGetIOIn_buf[7] = (Crc16_Val&0xff);
	IOCard_SendGetIOIn_buf[6]= ((Crc16_Val>>8)&0xff);
	// int i;
	// printf("Send_GetInState=");
	// for(i=0;i<8;i++)
	// {
	// 	printf("%02x ",IOCard_SendGetIOIn_buf[i]);
	// }
	// printf("\n");
}
void IOcard_build_getout_buf(int8_t subaddr)
{
	int Crc16_Val=0;
	Send_GetIOOut_flg =1;
	IOCard_SendGetIOOut_buf[0]=subaddr;
	IOCard_SendGetIOOut_buf[1]=0x01;   //function code
	IOCard_SendGetIOOut_buf[2]=0x00;
	IOCard_SendGetIOOut_buf[3]=0x00;   //output register address
	IOCard_SendGetIOOut_buf[4]=0x00;
	IOCard_SendGetIOOut_buf[5]=0x08;   //output register number
	Crc16_Val = mc_check_crc16(IOCard_SendGetIOOut_buf,6);
	IOCard_SendGetIOOut_buf[7] = (Crc16_Val&0xff);
	IOCard_SendGetIOOut_buf[6]= ((Crc16_Val>>8)&0xff);
	// int i;
	// printf("Send_GetOutState=");
	// for(i=0;i<8;i++)
	// {
	// 	printf("%02x ",IOCard_SendGetIOOut_buf[i]);
	// }
	// printf("\n");
}
void IOcard_build_setout_buf(int8_t subaddr,int8_t data)
{
	int Crc16_Val=0;
	IOCard_SendContIOOut_buf[0]=subaddr;
	IOCard_SendContIOOut_buf[1]=0x0f;   //function code
	IOCard_SendContIOOut_buf[2]=0x00;
	IOCard_SendContIOOut_buf[3]=0x00;   //output register address
	IOCard_SendContIOOut_buf[4]=0x00;
	IOCard_SendContIOOut_buf[5]=0x08;   //output register number
	IOCard_SendContIOOut_buf[6]=0x01;   //Bytes
	IOCard_SendContIOOut_buf[7]=data;
	Crc16_Val = mc_check_crc16(IOCard_SendContIOOut_buf,8);
	IOCard_SendContIOOut_buf[9] = (Crc16_Val&0xff);
	IOCard_SendContIOOut_buf[8]= ((Crc16_Val>>8)&0xff);
	// int i;
	// printf("Send_CtrlOutState=");
	// for(i=0;i<10;i++)
	// {
	// 	printf("%02x ",IOCard_SendContIOOut_buf[i]);
	// }
	// printf("\n");
}
void IOCardInter_Trans_Deal()
{
	static uint8_t Num=0;
	if(Num<5)Num++;
	else Num=0;
	switch (Num)
	{
		case 0:
			{
				IOcard_build_getIn_buf(0x11);
			}break;
		case 1:
			{
				IOcard_build_getout_buf(0x11);
			}break;
		case 2:
			{
//				IOcard_build_getIn_buf(2);
			}break;
		case 3:
			{
//				IOcard_build_getout_buf(2);
			}break;
		case 4:
			{
				//fan, light, sound, led
				Send_ContIOOut_flg=1;
				IOcard_build_setout_buf(0x11,IO1_ContOutData);
			}break;
		case 5:
			{
				//door1_open, door1_close, door2_open, door2_close
	//			Send_ContIOOut_flg[1]=1;
	//			IOcard_build_setout_buf(2,IO2_ContOutData);
			}break;
		default:
			break;
	}

}
/*IO板卡中间层对接函数*/
void mid_IOCard_Interface()
{
	uint8_t Front_Car_InPlace;  
	uint8_t Back_Car_InPlace;   //后车到位
	if(AirCon_conn_flg ==1)  //1
	{
  		if(AirCon_Temp[0]>22||AirCon_Temp[1]>22||AirCon_Temp[2]>22)
  		{
    		IO1_ContOutData |= 0x03;//散热风扇 两路
  		}
  		else{
    		IO1_ContOutData &= 0xFC;
  		}
  		if(AirCon_Temp[0]<10||AirCon_Temp[1]<10||AirCon_Temp[2]<10)
  		{
    		IO1_ContOutData |= 0x0C;//保温风扇 两路
  		}
  		else{
    		IO1_ContOutData &= 0xF3;
  		}
	}
//	 IO1_ContOutData |=((1<<4)
//            +(1<<5)); 
  		IO1_ContOutData |=((MLight_Start_flg<<4)
            +(MSound_Start_flg<<5));

 	Front_Car_InPlace = (IOCard_InputPinState[0]&0x01);
	Back_Car_InPlace = ((IOCard_InputPinState[0]>>1)&0x01);

	MIO_DoorValue_null[0] = ((IOCard_InputPinState[0]>>2)&0x01);
	MIO_WaterValue_null=((IOCard_InputPinState[0]>>5)&0x01);
//	printf("8888888888888IOCard_InputPinState[0]= %d\n",IOCard_InputPinState[0]);

	MIO_DoorValue_null[1] = 0;
	MIO_DoorValue_null[2] = 0;
	MIO_ThunderValue_null[0] = 0;
	MIO_ThunderValue_null[1] = 0;
	MIO_LightValue_null[0] = 0;
	MIO_LightValue_null[1] = 0;


	if(QtIo_Disable_buf[0]==1)	MIO_DoorValue_null[0] = 1;//延用SA03协议，筛选出门禁水浸
//	if(QtIo_Disable_buf[1]==1)	MIO_DoorValue_null[1] = 1;
//	if(QtIo_Disable_buf[2]==1)	MIO_DoorValue_null[2] = 1;
//	if(QtIo_Disable_buf[3]==1)	MIO_ThunderValue_null[0] = 1;
//	if(QtIo_Disable_buf[4]==1)	MIO_ThunderValue_null[1] = 1;
	if(QtIo_Disable_buf[5]==1)	MIO_WaterValue_null = 1;
//	if(QtIo_Disable_buf[6]==1)	MIO_LightValue_null[0] = 1;
//	if(QtIo_Disable_buf[7]==1)	MIO_LightValue_null[1] = 1;

//前后车到位检测
    if(Front_Car_InPlace==1&&Back_Car_InPlace==1)
  	{
    	MCE_InPlacSens_flg =1;
  	}
  	else
  	{
    	MCE_InPlacSens_flg =0;
  	}

}

void thread_IOCard(void)
{
//	printf("thread_IOCard tid %d\n",gettid());
	//定义sockfd
	IOCardSend_Discon_con[1] =0;
	mid_iocard.MIO_ThunderValue_null[0] = 1;
	mid_iocard.MIO_ThunderValue_null[1] = 1;
	mid_iocard.MIO_WaterValue_null = 1;
	mid_iocard.MIO_LightValue_null[0] = 1;
	mid_iocard.MIO_LightValue_null[1] = 1;
    int clientsockfd = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in servaddr;
	int8_t conn1 = -1;
	int16_t Send_flg[2];
	
	bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family=AF_INET;
    servaddr.sin_port=htons(PORT);
    servaddr.sin_addr.s_addr=inet_addr("192.168.1.205");  //205

	conn1=connect(clientsockfd,(struct sockaddr *)&servaddr,sizeof(servaddr));
	while(conn1<0)
    {
 //       static int i=1;
		close(clientsockfd);
		sleep(1);
		clientsockfd = socket(AF_INET, SOCK_STREAM, 0);
		int flags = fcntl(clientsockfd, F_GETFL, 0);
		fcntl(clientsockfd, F_SETFL, flags|O_NONBLOCK);
		conn1 = connect(clientsockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
//		printf("conn111=%d\n",conn1);
		// if(i==1)i=2;				
		// else i=1;
		sleep(2);
		IOcard_build_getIn_buf(1); //读板卡
		send(clientsockfd, IOCard_SendGetIOIn_buf, (8),MSG_DONTWAIT|MSG_NOSIGNAL);
		sleep(2);
		Read_Size1=recv(clientsockfd, IOCard_Recv_buf, BUFFER_SIZE,MSG_DONTWAIT|MSG_NOSIGNAL);
//		printf("Read_Size1=%d\n",Read_Size1,IOCard_Recv_buf[3]);
		if(Read_Size1<0){
//			fprintf(stderr, "connect IOCard function failed.\n");  //打印到文件
		}
		else if(Read_Size1>0) 
		{
			conn1=0;
			IOCard_Rec_Deal(Read_Size1);
		}
		MIO_Comm_flg = 2;
    }
	MIO_Comm_flg = 1;
 //   fprintf(stderr, "connect IOCard OK.\n");
	//void pthread_create();
	while(1)
	{
		mid_IOCard_Interface();
		IOCardInter_Trans_Deal();
		if(Send_ContIOOut_flg==1)
		{
			Send_ContIOOut_flg = 0;
			send(clientsockfd, IOCard_SendContIOOut_buf, (10),MSG_DONTWAIT|MSG_NOSIGNAL); 
//			printf("IO_Control_Out_1\n");
		}

		if(Send_GetIOIn_flg==1)
		{
			Send_GetIOIn_flg = 0;	
			Send_flg[0] = send(clientsockfd, IOCard_SendGetIOIn_buf, (8),MSG_DONTWAIT|MSG_NOSIGNAL);
		}
		if(Send_GetIOOut_flg==1)
		{
			Send_GetIOOut_flg=0;
			Send_flg[0]=send(clientsockfd, IOCard_SendGetIOOut_buf, (8),MSG_DONTWAIT|MSG_NOSIGNAL); 
		}
/*		
		if(Send_GetIOIn_flg[1]==1)
		{
			Send_GetIOIn_flg[1] = 0;	
			Send_flg[1] = send(clientsockfd, IOCard_SendGetIOIn_buf, (8),MSG_DONTWAIT|MSG_NOSIGNAL); 
		}
		if(Send_GetIOOut_flg[1]==1)
		{
			Send_GetIOOut_flg[1]=0;
			Send_flg[1]=send(clientsockfd, IOCard_SendGetIOOut_buf, (8),MSG_DONTWAIT|MSG_NOSIGNAL); 
		}
*/		
		if((IOCardSend_Discon_con[0]>15)&&(IOCardSend_Discon_con[1]>15))
		{

			MIO_Comm_flg = 2;
			conn1=-1;
			while(conn1<0)
			{
				static int i=1;
				close(clientsockfd);
				sleep(1);
				clientsockfd = socket(AF_INET, SOCK_STREAM, 0);
				int flags = fcntl(clientsockfd, F_GETFL, 0);
				fcntl(clientsockfd, F_SETFL, flags|O_NONBLOCK);
				conn1 = connect(clientsockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
//				printf("conn1=%d\n",conn1);
				if(i==1)i=2;				
				else i=1;
				sleep(2);
				IOcard_build_getIn_buf(i);
				send(clientsockfd, IOCard_SendGetIOIn_buf, (8),MSG_DONTWAIT|MSG_NOSIGNAL);
				sleep(1);
				Read_Size1=recv(clientsockfd, IOCard_Recv_buf, BUFFER_SIZE,MSG_DONTWAIT|MSG_NOSIGNAL);
//				printf("Read_Size1=%d\n",Read_Size1);
				if(Read_Size1<0){
					syslog(LOG_INFO,"板卡connect IOCard function failed Send_flg \n");
//					fprintf(stderr, "connect IOCard function failed.\n");  //打印到文件
				}		
				else if(Read_Size1>0) 
				{
					conn1=0;
					IOCard_Rec_Deal(Read_Size1);
				}
			}
		}
		MIO_Comm_flg = 1;
		usleep(200*1000);//500ms
		Read_Size1=(recv(clientsockfd, IOCard_Recv_buf, BUFFER_SIZE,MSG_DONTWAIT|MSG_NOSIGNAL));
		if(Read_Size1<0)
		{
			// printf("read failure");
			// if((errno == EAGAIN)||(errno==EWOULDBLOCK))
			// {
			// 	printf("The %dth IOCard is not disconnect\n",i);
			// }
			// else
			// {
			// 	printf("The %dth IOCard is disconnect\n",i);
			// }
			// printf("read failure\n");
		}	
		else
		{
			Read_Size=Read_Size1;
	//		printf("Read_Size = %d\n",Read_Size);
			IOCard_Rec_Deal(Read_Size1);
			
		}
		sleep(1);
	}
	close(clientsockfd);
}