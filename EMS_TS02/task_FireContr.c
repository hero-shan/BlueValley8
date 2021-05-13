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
// #include "check_crc.h"
#include <errno.h>
#include "mymain.h"

#define FIRE_CONTROL_IP     "192.168.1.201"
// #define FIRE_CONTROL_IP     "192.168.206.45"
#define FIRE_CONTROL_PORT     502
#define FIRE_RECV_BUFF_SIZE     200


//****************************
#define COMPUTER_RESET 0xA1 //复位
#define COMPUTER_XUNJIAN 0xA2 //巡检
#define COMPUTER_SNDEVNET 0xA5 //发送事件


int fireControlCount = 0;//没有收到数据的计数
// uint8_t fireControlConnectSta = 0;//网正常 但是收不到数，判定线异常
uint8_t FireContr_conn_flg = 0;//net link sta
uint8_t FireContr_HardErr_flg = 0;       //故障
uint8_t FireContr_GetFire_flg = 0;   //火警

//正常应答
uint8_t pollingResponse_buf[20] = {0xF0,0x00,0x03,0xA2,0x00,0x00,0x48,0x20};
uint8_t pollingResponse_buf_len = 8;
uint8_t sendNormalResponse_flg = 0;

//请求重发事件
// uint8_t requestSendAgain_buf[20] = {0xF0, 0x00, 0x03, 0xA2, 0xA1, 0x00, 0xCC, 0x3E};
// uint8_t requestSendAgain_buf_len = 8;

uint8_t fireControlRecv_buf[FIRE_RECV_BUFF_SIZE] = {0};

void parseFireControlMsg(const uint8_t *buf,int length);

void FireContr_Rec_Deal(uint8_t* recv_buf,int16_t Read_Size)
{
	uint16_t i=0,rec_calchkval,my_calchkval;
	uint16_t RecvData_dlc = 0;
	uint8_t buf[500];

    uint8_t currentHead = 0;
	// printf("Read_Size=%d \n",Read_Size);

    do
    {
    	if(recv_buf[currentHead + 0]==0xF0 && (recv_buf[currentHead + 1]==0x00))
        {
            RecvData_dlc = recv_buf[currentHead + 2] + 3 +2;
            memcpy(buf,(recv_buf + currentHead),RecvData_dlc);
            currentHead += RecvData_dlc;
            parseFireControlMsg(buf,RecvData_dlc);     
        }
        else
        {
            break;
        }
        
    } while (currentHead != Read_Size);
    
	bzero(recv_buf, Read_Size);
}

void diagLinkSta();

void thread_FireContr(void)
{
//	printf("thread_FireContr start!\n");

    //定义sockfd
    int send_flg = 0;//用于记录发送函数的返回值
    int recv_flg = 0;//用于记录读取函数的返回值
    int clientsockfd = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in servaddr;
	
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(FIRE_CONTROL_PORT);  ///服务器端口
    servaddr.sin_addr.s_addr = inet_addr(FIRE_CONTROL_IP);  ///服务器ip

	//close(clientsockfd);
	//连接服务器成功返回0错误返回-1
	int connect_flg = connect(clientsockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
	while(connect_flg < 0)
	{
		FireContr_conn_flg = 2;
        close(clientsockfd);
		sleep(2);
		clientsockfd = socket(AF_INET, SOCK_STREAM, 0);
		int flags = fcntl(clientsockfd, F_GETFL, 0);  
		fcntl(clientsockfd, F_SETFL, flags|O_NONBLOCK);
		connect_flg = connect(clientsockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
		sleep(5);
		if(recv(clientsockfd, fireControlRecv_buf, 
                FIRE_RECV_BUFF_SIZE,MSG_DONTWAIT|MSG_NOSIGNAL)> 0)
		{
			fireControlCount = 0;
            connect_flg = 1;
		}
//		printf("connect FireContr function failed.\n");
	}
	FireContr_conn_flg = 1;
//	printf("connect FireContr OK.\n");
    pthread_t recv_id;
    pthread_create(&recv_id,0,(void*)diagLinkSta,0);
	while(1)
	{
		// FireContrInter_Trans_Deal();
        // MIO_FireContr_err = FireContr_GetFire_flg;
		if(sendNormalResponse_flg == 1)
		{
			// printf("sendNormalResponse_flg\n");
			sendNormalResponse_flg = 0;
			send_flg = send(clientsockfd, pollingResponse_buf, 
                (pollingResponse_buf_len),MSG_DONTWAIT|MSG_NOSIGNAL); //发送
		}	
		
		if(FireContr_conn_flg != 1)
		{			
			// printf("bad\n");
            FireContr_conn_flg = 2;
			// conn1 = connect(clientsockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
			while(recv_flg <= 0)
			{
				close(clientsockfd);
				usleep(500*1000);
				clientsockfd = socket(AF_INET, SOCK_STREAM, 0);
				int flags = fcntl(clientsockfd, F_GETFL, 0);  
				fcntl(clientsockfd, F_SETFL, flags|O_NONBLOCK);
				connect_flg = connect(clientsockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
				sleep(4);
				recv_flg =recv(clientsockfd, fireControlRecv_buf, 
                    FIRE_RECV_BUFF_SIZE,MSG_DONTWAIT|MSG_NOSIGNAL);
                
                if(recv_flg < 0)
                {
 //                   printf("errno = %d\n",errno);  
                }
                else
                {
                    fireControlCount = 0;
                }
                
  //              printf("%d \n",recv_flg);
//				printf("connect FireContr function failed.\n");
			}
            FireContr_conn_flg = 1;		
		}
		else
		{
			FireContr_conn_flg = 1;
		}

		recv_flg = recv(clientsockfd, fireControlRecv_buf, 
                    FIRE_RECV_BUFF_SIZE,MSG_DONTWAIT|MSG_NOSIGNAL);
        
        if(recv_flg < 0 )
        {
            if((errno == EAGAIN) ||(errno == EWOULDBLOCK))
            {
                recv_flg = 0;  
            }
            else
            {
  //              printf("%d \n",recv_flg);
  //              printf("errno = %d\n",errno);
            }
        }		
		else if(recv_flg > 0)
        {
	//		printf("%d \n",recv_flg);
            FireContr_Rec_Deal(fireControlRecv_buf,recv_flg);
            fireControlCount = 0;
		}

		usleep(50*1000);//50 ms 消防收到数据后需要及时回复
	}
	close(clientsockfd);
}

void diagLinkSta()
{
    while (1)
    {
        sleep(1);
        // printf("fireControlCount = %d\n",fireControlCount);
        //判断接收数据超时
        if(fireControlCount < (10))
        {
            fireControlCount++;
            FireContr_conn_flg = 1;
        }
        else
        {
            fireControlCount = 500; 
            FireContr_conn_flg = 2;
        }
    }
}

void parseFireControlMsg(const uint8_t *buf,int length)
{
    // 主机发送： 0xF0 地址 数据长度 命令字 数据段 CRC16 校验值
    //      例： 0xF0 0x00 0x14/0x28 0xA5 数据段 CRC16 校验值
    uint16_t crc16Value = 0;
    uint16_t myCrc16Value = 0;

    uint8_t frameHead = buf[0];
    uint8_t devAddress = buf[1];
    uint8_t dataLength = buf[2];
    uint8_t cmdString = buf[3]; 
    uint8_t data_buf[40];
    int i;
    // for(i=0;i<length;++i)
    // {
    //     printf("%2x ",buf[i]);
    // }
    // printf("\nlen = %d\n\n",length);


    if((frameHead == 0xf0) && (devAddress == 0x00)
        && ((dataLength == 0x14) || (dataLength == 0x28) || (dataLength == 0x01) ))
    {
        crc16Value = (uint16_t)((uint8_t)buf[3 + dataLength] * 256 )
                        + (uint16_t)((uint8_t)buf[3 + dataLength + 1]);

        myCrc16Value =  FireC_check_crc16(buf+1, length-3);

        // printf("%d %d len = %d\n",crc16Value,myCrc16Value,length);
        if(crc16Value == myCrc16Value)
        {
            memcpy(data_buf,buf+4,dataLength);
            if(cmdString == COMPUTER_RESET)
            {
                //do nothing
            }
            else if(cmdString == COMPUTER_XUNJIAN)
            {
                //应答
                // printf("收到巡检 准备回复!!\n\n");
                sendNormalResponse_flg = 1;
            }
            else if (cmdString == COMPUTER_SNDEVNET)
            {
                //故障
                if((data_buf[0] == 0xc2) && (data_buf[2] == 0x09))
                {
                    //主电 备电 故障
    //                printf("收到主电 备电 故障!!  data_buf[1] %d\n\n",data_buf[1]);
                    FireContr_HardErr_flg = 1;        
                }
                else if ((data_buf[0] == 0xc2) && (data_buf[2] == 0x0A))
                {
                    //主电 备电 故障恢复
     //               printf("收到主电 备电 故障恢复!!\n\n");
                    FireContr_HardErr_flg = 0;                    
                }
                //火警
                else if ((data_buf[0] == 0xc3) && (data_buf[2] == 0x05))
                {
                    //火警
     //               printf("收到火警!!\n\n");
                    FireContr_GetFire_flg = 1;   
                }
            }
        }//end crc check 
    }
}


uint8_t getFireControlErrSta()
{
    return FireContr_HardErr_flg?1:0;
}

uint8_t getFireControlFireAlarm()
{
    return FireContr_GetFire_flg?1:0;
}