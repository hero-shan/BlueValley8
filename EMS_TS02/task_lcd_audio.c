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
#include <iconv.h>

#include "task_lcd_audio.h"

/* ini parser headers*/
#include "dictionary.h"
#include "iniparser.h"
#include "mymain.h"


#define AV_PORT (10000)
#define AV_ADDRESS ("192.168.1.106")   //106
   
#define PRINT_TEST
#define ARRAY_SIZE_MAX  30

//*******************************************************************//

char stationFault_buf[100] = ""; 
char stationFault0_buf[100] = ""; 
char stationFault1_buf[100] = ""; 
char stationFault2_buf[100] = "";

char stationFireErr_buf[100] = "";
char stationFireErr0_buf[100] = "";
char stationFireErr1_buf[100] = "";

char carIn_buf[100] = "";
char carIn0_buf[100] = "";

char carNotInNet_buf[100] = "";
char carNotInNet0_buf[100] = "";
char carAbnormal_buf[100] = "";
char carAbnormal0_buf[100] = "";
char stationNoBattery_buf[100] = ""; 
char stationNoBattery0_buf[100] = "";

char orderCanceled_buf[100] = "";
char carRising_buf[100] = "";
char carRising0_buf[100] = "";

char changeBattSuccess_buf[100] = "";
char changeBattSuccess0_buf[100] = "";

char carInPlace_buf[100] = "";
char carInPlace0_buf[100] = "";
char carInPlace1_buf[100] = "";

char showCarPlate[30] = "";
char currentTime[50] = "";


lcdControl_t lcdControlData[ARRAY_SIZE_MAX];
int currentLength = 0;
lcdControlSig_t lcdControlSig={0};


void updateItem(const int index,
            const char *line1buf,
            const char *line2buf,    
            const char *line3buf,
            const char *line4buf    
            )
{
    strcpy(lcdControlData[index].lcdShowData[0],line1buf);
    strcpy(lcdControlData[index].lcdShowData[1],line2buf);
    strcpy(lcdControlData[index].lcdShowData[2],line3buf);

    strcpy(lcdControlData[index].lcdShowData[3],line1buf);
    strcat(lcdControlData[index].lcdShowData[3],line2buf);
    strcat(lcdControlData[index].lcdShowData[3],line3buf);
//    strcat(lcdControlData[index].lcdShowData[0],line4buf);
    if(index ==13)
    {
        lcdControlData[index].playAudio_flg =1; 
    }
      
}

struct tm *p;
void getLocalTime()
{
    time_t timep;
    time(&timep);	
	p = localtime(&timep);
	strftime(currentTime,50,"%m-%d %H:%M",p);
}
void updataShowBuf(int index)
{
    if(index == 5 || index == 13 )
    {    
        updateItem(13,"欢迎",showCarPlate,"进入能源服务站","");
        getLocalTime();
        updateItem(5,"欢迎进入","蓝谷能源服务站",currentTime,"");
    }
}

void myAddItem( const char *desc,
            const char *line1buf,
            const char *line2buf,    
            const char *line3buf,
            const char *line4buf,
            const char  playAudio
            )   
{
    if(currentLength >= ARRAY_SIZE_MAX)
        return;

    lcdControlData[currentLength].show_flg = 0;
    strcpy(lcdControlData[currentLength].lcdShowData[3], desc); 

    strcpy(lcdControlData[currentLength].lcdShowData[0],line1buf);
    strcpy(lcdControlData[currentLength].lcdShowData[1],line2buf);
    strcpy(lcdControlData[currentLength].lcdShowData[2],line3buf);
    
    // strcpy(lcdControlData[currentLength].lcdShowData[0],line1buf);
    // strcat(lcdControlData[currentLength].lcdShowData[0],line2buf);
    // strcat(lcdControlData[currentLength].lcdShowData[0],line3buf);
 //   strcat(lcdControlData[currentLength].lcdShowData[0],line4buf);

    lcdControlData[currentLength].playAudio_flg = playAudio;

    ++currentLength;
}

void lcdSendBufBuild(int index)
{
    if(lcdControlData[index].playAudio_flg)
    {
        lcd_show_buf_build(lcdControlData[index].lcdShowData[0],lcdControlData[index].lcdShowData[1],
            lcdControlData[index].lcdShowData[2],lcdControlData[index].lcdShowData[3],1,0);
    } 
    else
    {
        lcd_show_buf_build(lcdControlData[index].lcdShowData[0],lcdControlData[index].lcdShowData[1],
            lcdControlData[index].lcdShowData[2],"",0,0);               
    }    
}

void lcdShowPolicyinit()
{
    /*id0 换电站故障暂不支持换电*/
    myAddItem("换电站故障暂不支持换电",stationFault_buf,stationFault0_buf,"","",0);
    /*id1 换电站故障暂时不支持换电请驶离*/
    myAddItem("换电站故障暂时不支持换电请驶离",stationFault_buf,stationFault0_buf,stationFault1_buf,"",1);    
    /*id2 换电站故障请专业人员维修*/
    myAddItem("换电站故障请专业人员维修",stationFault_buf,stationFault0_buf,stationFault2_buf,"",1);     
    /*id3 消防故障 请尽快驶离换电站*/
    myAddItem("消防故障请尽快驶离换电站",stationFireErr_buf,stationFireErr0_buf,"","",1);  
    /*id4 消防故障 请远离换电站*/
    myAddItem("消防故障请远离换电站",stationFireErr_buf,stationFireErr1_buf,"","",1);
    /*id5 欢迎进入 蓝谷能源服务站*/
    myAddItem("欢迎进入蓝谷能源服务站",carIn_buf,carIn0_buf,"","",0); 
    /*id6 车辆为未入网车辆请驶离*/
    myAddItem("车辆为未入网车辆 请驶离",carNotInNet_buf,carNotInNet0_buf,"","",1);
    /*id7 车辆异常请驶离*/
    myAddItem("车辆异常请驶离",carAbnormal_buf,carAbnormal0_buf,"","",1);
    /*id8 无可换电池包请驶离*/
    myAddItem("无可换电池包请驶离",stationNoBattery_buf,stationNoBattery0_buf,"","",1);
    /*id9 订单取消请驶离*/
    myAddItem("订单取消请驶离",orderCanceled_buf,"请驶离","","",1);   
    /*id10 车辆升举请勿上下车*/
    myAddItem("车辆升举 请勿上下车",carRising_buf,carRising0_buf,"","",1); 
    /*id11 换电完成欢迎下次光临*/
    myAddItem("换电完成 欢迎下次光临",changeBattSuccess_buf,changeBattSuccess0_buf,"","",1); 
    /*id12 车辆已经到位，请松开手刹，车辆下电*/
    myAddItem("车辆已经到位，请松开手刹，车辆下电",carInPlace_buf,carInPlace0_buf,carInPlace1_buf,"",1);  
    /*id13 欢迎进站*/
    myAddItem("欢迎进站",carIn_buf,carIn0_buf,"","",1); 
     /*id14 换电完成 请驶离*/
    myAddItem("换电完成 请驶离",changeBattSuccess_buf,carAbnormal0_buf,"","",1); 
    /*id15 /#WR_FLASH=1*/
    myAddItem("开启语音屏存储","/#WR_FLASH=1","","","",0); 
    /*id16*/
    myAddItem("关闭语音屏存储","/#WR_FLASH=0","","","",0); 
    /*id17*/
    myAddItem("欢迎进入蓝谷能源服务站","欢迎进入蓝谷能源服务站","","","",0);
}

void updataControlSig()
{
    /*换电站故障暂不支持换电*/
    lcdControlData[0].show_flg = MLed_PlayInitErr_flg; 
    MLed_PlayInitErr_flg= 0;
    /*换电站故障暂时不支持换电请驶离*/
    lcdControlData[1].show_flg = MLed_PlayErrGo_flg; 
    MLed_PlayErrGo_flg = 0;
    /*换电站故障请专业人员维修*/
    lcdControlData[2].show_flg = MLed_PlayCBErr_flg; 
    MLed_PlayCBErr_flg = 0;
    /*消防故障请尽快驶离换电站*/
    lcdControlData[3].show_flg = MLed_FireErrGo_flg; 
    MLed_FireErrGo_flg = 0;
    /*消防故障请远离换电站*/
    lcdControlData[4].show_flg = MLed_FireErrCB_flg; 
    MLed_FireErrCB_flg = 0;
    /*欢迎进入蓝谷能源服务站*/
    lcdControlData[5].show_flg = MLed_ShowFree_flg; 
    MLed_ShowFree_flg = 0;
    /*车辆为未入网车辆请驶离*/
    lcdControlData[6].show_flg = MLed_PlayCarNoInnet_flg; 
    MLed_PlayCarNoInnet_flg = 0;
    /*车辆异常请驶离*/
    lcdControlData[7].show_flg = MLed_PlayCloudNoIn_flg; 
    MLed_PlayCloudNoIn_flg = 0;
    /*无可换电池包请驶离*/
    lcdControlData[8].show_flg = MLed_PlayNoBag_flg; 
    MLed_PlayNoBag_flg = 0;
    /*订单取消请驶离*/
    lcdControlData[9].show_flg = MLed_PlayCancelOrder_flg; 
    MLed_PlayCancelOrder_flg = 0;
    /*车辆升举请勿上下车*/
    lcdControlData[10].show_flg = (MLed_PlayStartCB_flg||MLed_PlayCBing_flg); 
    MLed_PlayStartCB_flg = 0;MLed_PlayCBing_flg = 0;
    /*换电完成欢迎下次光临*/
    lcdControlData[11].show_flg = (MLed_PlayOut_flg||0); 
    MLed_PlayOut_flg = 0;
    /*车辆到位*/
    lcdControlData[12].show_flg = (MLed_PlayInPlac_flg); 
    MLed_PlayInPlac_flg = 0;
    /*欢迎进站*/
    lcdControlData[13].show_flg = (MLed_PlayIn_flg); 
    MLed_PlayIn_flg = 0;
    /*换电完成 请驶离*/
    lcdControlData[14].show_flg = (MLed_PleaseGo_flg); 
    MLed_PleaseGo_flg = 0;
    strcpy(showCarPlate,Cloud_Grab_carNo);

}

int myselect()
{
    int index = 0;
    for(index=0;index < ARRAY_SIZE_MAX; ++index)
    {
        if(lcdControlData[index].show_flg)
        {
            lcdControlData[index].show_flg = 0;
            updataShowBuf(index);
            lcdSendBufBuild(index);
            return index;
        }
    }
    return -1;
}

/*海康逻辑屏显示缓存*/
uint8_t HK_LcdShow_buf[500];
uint16_t HK_LcdShow_buf_len;

uint8_t Audio_Cmd_buf[500];
uint16_t Audio_Cmd_buf_len;

uint8_t LCD_conn_flg = 0;/*连接状态*//*1:link 2:unlink other:unknown*/

void pthread_AV(void)
// int main(void)
{
    LoadIniFile();
    lcdShowPolicyinit();

    int AV_Connect_ret = 0;/*socket连接函数返回值*/
    int Send_flg = 0;
    u_int8_t Send_ReConnect_buf[10];
    // uint8_t AV_Recv_buf[200];

    //定义sockfd
    int clientsockfd = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in servaddr;
	
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(AV_PORT);  ///服务器端口
    servaddr.sin_addr.s_addr = inet_addr(AV_ADDRESS);  ///服务器ip

	//连接服务器成功返回0错误返回-1
	AV_Connect_ret = connect(clientsockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
	while(AV_Connect_ret < 0)
	{   
        close(clientsockfd);
		sleep(1);
		clientsockfd = socket(AF_INET, SOCK_STREAM, 0);
		int flags = fcntl(clientsockfd, F_GETFL, 0);  
		fcntl(clientsockfd, F_SETFL, flags|O_NONBLOCK);
		AV_Connect_ret = connect(clientsockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
		sleep(1);
        AV_Connect_ret = send(clientsockfd, Send_ReConnect_buf, (1),MSG_DONTWAIT|MSG_NOSIGNAL);
		LCD_conn_flg = 2;
	}
	LCD_conn_flg = 1;
	printf("connect to AV OK.\n");
    close(clientsockfd);
    ///#WR_FLASH=1 ：写入FLASH开启
    /*
    lcd_show_buf_build(lcdControlData[15].lcdShowData[0],"",0,1);
    AV_Connect_ret = send(clientsockfd,HK_LcdShow_buf,HK_LcdShow_buf_len,MSG_DONTWAIT|MSG_NOSIGNAL);
    sleep(2);
    lcd_show_buf_build(lcdControlData[17].lcdShowData[0],"",0,1);
	AV_Connect_ret = send(clientsockfd,HK_LcdShow_buf,HK_LcdShow_buf_len,MSG_DONTWAIT|MSG_NOSIGNAL);
    */		
	while(1)
	{
        updataControlSig();
        int selectId = 0;
        if(-1 != (selectId = myselect()) )
        {
            do
            {
                clientsockfd = socket(AF_INET, SOCK_STREAM, 0);
                int flags = fcntl(clientsockfd, F_GETFL, 0);  
                fcntl(clientsockfd, F_SETFL, flags|O_NONBLOCK);
                AV_Connect_ret = connect(clientsockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
                sleep(1);
                AV_Connect_ret = send(clientsockfd, HK_LcdShow_buf, HK_LcdShow_buf_len,MSG_DONTWAIT|MSG_NOSIGNAL);
                bzero(HK_LcdShow_buf,sizeof HK_LcdShow_buf);HK_LcdShow_buf_len = 0;
                sleep(2);              
                close(clientsockfd);

            }while (AV_Connect_ret < 0);

 //           Send_flg = send(clientsockfd, HK_LcdShow_buf, HK_LcdShow_buf_len,MSG_DONTWAIT|MSG_NOSIGNAL);  
//            printf("HK_LcdShow_buf_len %d\n" ,HK_LcdShow_buf_len);
            
        }
        else
        {
            sleep(1);
            #if 0
            static int i=0;
            if(i < 14)
                ++i;
            else
                i=0;
                
            lcdControlData[i].show_flg = 1;
            #endif

        }
        /*
        if(Send_flg < 0)
        {
            LCD_conn_flg = 2;
            do
            {
                printf("connect AV function failed.\n");
                close(clientsockfd);
                sleep(1);
                clientsockfd = socket(AF_INET, SOCK_STREAM, 0);
                int flags = fcntl(clientsockfd, F_GETFL, 0);  
                fcntl(clientsockfd, F_SETFL, flags|O_NONBLOCK);
                AV_Connect_ret = connect(clientsockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
                sleep(1);
                AV_Connect_ret = send(clientsockfd, Send_ReConnect_buf, (1),MSG_DONTWAIT|MSG_NOSIGNAL);

            }while (AV_Connect_ret < 0);

            LCD_conn_flg = 1;
            
        }*/
        
	}
	close(clientsockfd);
}


uint8_t Line_showbuf_temp[300];
//
void lcd_show_buf_build(uint8_t *line1buf,uint8_t *line2buf,uint8_t *line3buf,
                        uint8_t *Audiobuf,uint8_t EN_playAudio,uint8_t General_Display)  //海康
{
    uint32_t total_len = 0,len_temp=0; 
    uint16_t i;
    //帧头
    HK_LcdShow_buf[0] = 0x55;
    HK_LcdShow_buf[1] = 0xaa;
    HK_LcdShow_buf[2] = 0x00;
    HK_LcdShow_buf[3] = 0x00;
    //地址，固定为0x01
    HK_LcdShow_buf[4] = 0x01;
    //保留 2字节
    HK_LcdShow_buf[5] = 0x00;
    HK_LcdShow_buf[6] = 0x00;
    //操作码0x00--0xff  采用固定：常规显示数据 0xDA
    if(General_Display ==1)
    {
        HK_LcdShow_buf[7] =0xDA;
        total_len =Fixed_ShowBuf_bulid(line1buf);
        HK_LcdShow_buf[26] = 0x03;
    }
    else
    {
        HK_LcdShow_buf[7] =0xDB;    //即时信息
        total_len =Area_ShowBuf_bulid(line1buf,line2buf,line3buf,Audiobuf,EN_playAudio);

        if(EN_playAudio ==1)
        {
            if(strcmp(line3buf,"")==0)
            {
                HK_LcdShow_buf[26] = 0x03;
            }
            else
            {
                HK_LcdShow_buf[26] = 0x04;
            }           
        }
        else
        {
            if(strcmp(line3buf,"")==0)
            {
                HK_LcdShow_buf[26] = 0x02;
            }
            else
            {
                HK_LcdShow_buf[26] = 0x03;
            }            
        }      
    }   
    //帧序号
    HK_LcdShow_buf[8] =0x00;
    HK_LcdShow_buf[9] =0x00;
    
    //保留
    HK_LcdShow_buf[14] = 0x00;
    HK_LcdShow_buf[15] = 0x00;
     
    
    HK_LcdShow_buf[20] = 0x01;  //节目块个数
    HK_LcdShow_buf[21] = 0x01;  //节目号

    for(i=0;i<18;i++)
    {
        HK_LcdShow_buf[27+i] = 0;
    }
    
    for(i=0; i<total_len; i++)
    {
        HK_LcdShow_buf[45+i] =Line_showbuf_temp[i];
    }
    
    HK_LcdShow_buf[45+i] =0x00;
    HK_LcdShow_buf[46+i] =0x00;
    HK_LcdShow_buf[47+i] =0x0D;
    HK_LcdShow_buf[48+i] =0x0A;

    len_temp =total_len +24;
    HK_LcdShow_buf[22] = (uint8_t)(len_temp>>0)&0xff;
    HK_LcdShow_buf[23] = (uint8_t)(len_temp>>8)&0xff;
    HK_LcdShow_buf[24] = (uint8_t)(len_temp>>16)&0xff;
    HK_LcdShow_buf[25] = (uint8_t)(len_temp>>24)&0xff;
    len_temp =len_temp+1;
    //数据总长：不分包时 =数据域长度   低字节在前 
    HK_LcdShow_buf[10] =(uint8_t)(len_temp>>0)&0xff;
    HK_LcdShow_buf[11] =(uint8_t)(len_temp>>8)&0xff;
    HK_LcdShow_buf[12] =(uint8_t)(len_temp>>16)&0xff;
    HK_LcdShow_buf[13] =(uint8_t)(len_temp>>24)&0xff;
    //数据长度
    HK_LcdShow_buf[16] = (uint8_t)(len_temp>>0)&0xff;
    HK_LcdShow_buf[17] = (uint8_t)(len_temp>>8)&0xff;
    HK_LcdShow_buf[18] = (uint8_t)(len_temp>>16)&0xff;
    HK_LcdShow_buf[19] = (uint8_t)(len_temp>>24)&0xff;

    HK_LcdShow_buf_len =total_len +49;  //24 + 1 + 24
}


uint32_t Fixed_ShowBuf_bulid(uint8_t *line1buf)     //固定显示
{
    uint32_t line_len_temp =0,String_len;  
    uint16_t i;
    uint8_t Line_showbuf_GBK_temp1[300] = {0};
   
    u2g(line1buf, strlen(line1buf), Line_showbuf_GBK_temp1, sizeof(Line_showbuf_GBK_temp1));
    String_len = strlen(Line_showbuf_GBK_temp1);

    Line_showbuf_temp[0] = 0x01;
    Line_showbuf_temp[5] = 0x0E;   //字符内码

    Line_showbuf_temp[6] = 0x00;
    Line_showbuf_temp[7] = 0x00;  //x
    Line_showbuf_temp[8] = 0x00;
    Line_showbuf_temp[9] = 0x00;  //左上角X\Y坐标 (0,0)
    Line_showbuf_temp[10] = 0x3F;
    Line_showbuf_temp[11] = 0x00;
    Line_showbuf_temp[12] = 0x1F;
    Line_showbuf_temp[13] = 0x00;  //右下角X\Y坐标 (64,32)

    Line_showbuf_temp[14] = 0x01;    //字体颜色  默认红
    Line_showbuf_temp[15] = 0x00;
    Line_showbuf_temp[16] = 0x00;

    Line_showbuf_temp[17] = 0x20;   //0x20:向左连续移动
    Line_showbuf_temp[18] = 0x00;

    Line_showbuf_temp[19] = 0xFF;   //速度
    Line_showbuf_temp[20] = 0xFF;   //每页停留时间

    Line_showbuf_temp[21] = 0x20;    //0x20:32x32字体 0x10:16x16

    Line_showbuf_temp[22] = (uint8_t)(String_len>>0)&0xff;
    Line_showbuf_temp[23] = (uint8_t)(String_len>>8)&0xff;
    Line_showbuf_temp[24] = (uint8_t)(String_len>>16)&0xff;
    Line_showbuf_temp[25] = (uint8_t)(String_len>>24)&0xff;   

    for(i=0; i<String_len; i++)
    {
        Line_showbuf_temp[26+i] = Line_showbuf_GBK_temp1[i]; 
    }

    line_len_temp = 26 + String_len;
    Line_showbuf_temp[1] = (uint8_t)(line_len_temp>>0)&0xff; 
    Line_showbuf_temp[2] = (uint8_t)(line_len_temp>>8)&0xff;
    Line_showbuf_temp[3] = (uint8_t)(line_len_temp>>16)&0xff;
    Line_showbuf_temp[4] = (uint8_t)(line_len_temp>>24)&0xff;   //整个数据域的大小

    return line_len_temp;
}

uint32_t Area_ShowBuf_bulid(uint8_t *line2buf,uint8_t *line3buf,uint8_t *line4buf,uint8_t *Audiobuf,uint8_t EN_playAudio)
{
    uint8_t Line_showbuf_GBK_temp1[300] = {0} ,Line_showbuf_GBK_temp2[300] ={0};
    uint8_t Line_showbuf_GBK_temp3[300] = {0} ,Line_showbuf_GBK_temp4[300] ={0};
    uint32_t line_len_temp[4],String_len[4];
    uint16_t i;

    u2g(line2buf, strlen(line2buf), Line_showbuf_GBK_temp1, sizeof(Line_showbuf_GBK_temp1));
    String_len[0] = strlen(Line_showbuf_GBK_temp1);

    Line_showbuf_temp[0] = 0x01;
    Line_showbuf_temp[5] = 0x0E;   //字符内码

    Line_showbuf_temp[6] = 0x00;
    Line_showbuf_temp[7] = 0x00;  //x
    Line_showbuf_temp[8] = 0x00;
    Line_showbuf_temp[9] = 0x00;  //左上角X\Y坐标 (0,0)
    Line_showbuf_temp[10] = 0x3F;
    Line_showbuf_temp[11] = 0x00;
    Line_showbuf_temp[12] = 0x0F;
    Line_showbuf_temp[13] = 0x00;  //右下角X\Y坐标 (64,32)

    Line_showbuf_temp[14] = 0x01;    //字体颜色  默认红
    Line_showbuf_temp[15] = 0x00;
    Line_showbuf_temp[16] = 0x00;

    Line_showbuf_temp[17] = 0x20;   //0x20:向左连续移动
    Line_showbuf_temp[18] = 0x00;

    Line_showbuf_temp[19] = 0x2F;   //速度
    Line_showbuf_temp[20] = 0xFF;   //每页停留时间

    Line_showbuf_temp[21] = 0x10;    //0x10:16x16

    Line_showbuf_temp[22] = (uint8_t)(String_len[0]>>0)&0xff;
    Line_showbuf_temp[23] = (uint8_t)(String_len[0]>>8)&0xff;
    Line_showbuf_temp[24] = (uint8_t)(String_len[0]>>16)&0xff;
    Line_showbuf_temp[25] = (uint8_t)(String_len[0]>>24)&0xff;   

    for(i=0; i<String_len[0]; i++)
    {
        Line_showbuf_temp[26+i] = Line_showbuf_GBK_temp1[i]; 
    }

    line_len_temp[0] = 26 + String_len[0];  //strlen
    Line_showbuf_temp[1] = (uint8_t)(line_len_temp[0]>>0)&0xff; 
    Line_showbuf_temp[2] = (uint8_t)(line_len_temp[0]>>8)&0xff;
    Line_showbuf_temp[3] = (uint8_t)(line_len_temp[0]>>16)&0xff;
    Line_showbuf_temp[4] = (uint8_t)(line_len_temp[0]>>24)&0xff;   //整个数据域的大小
    
    /*---------------------第二行--------------------------------------*/
    u2g(line3buf, strlen(line3buf), Line_showbuf_GBK_temp2, sizeof(Line_showbuf_GBK_temp2));
    String_len[1] = strlen(Line_showbuf_GBK_temp2);

    Line_showbuf_temp[line_len_temp[0]+0] = 0x02;
    Line_showbuf_temp[line_len_temp[0]+5] = 0x0E;   //字符内码

    Line_showbuf_temp[line_len_temp[0]+6] = 0x00;
    Line_showbuf_temp[line_len_temp[0]+7] = 0x00;  //x
    Line_showbuf_temp[line_len_temp[0]+8] = 0x0f;
    Line_showbuf_temp[line_len_temp[0]+9] = 0x00;  //左上角X\Y坐标 (0,0)
    Line_showbuf_temp[line_len_temp[0]+10] = 0x3F;
    Line_showbuf_temp[line_len_temp[0]+11] = 0x00;
    Line_showbuf_temp[line_len_temp[0]+12] = 0x1F;
    Line_showbuf_temp[line_len_temp[0]+13] = 0x00;  //右下角X\Y坐标 (64,32)

    Line_showbuf_temp[line_len_temp[0]+14] = 0x01;    //字体颜色  默认红
    Line_showbuf_temp[line_len_temp[0]+15] = 0x00;
    Line_showbuf_temp[line_len_temp[0]+16] = 0x00;

    Line_showbuf_temp[line_len_temp[0]+17] = 0x20;   //0x20:向左连续移动
    Line_showbuf_temp[line_len_temp[0]+18] = 0x00;

    Line_showbuf_temp[line_len_temp[0]+19] = 0x2F;   //速度
    Line_showbuf_temp[line_len_temp[0]+20] = 0xFF;   //每页停留时间

    Line_showbuf_temp[line_len_temp[0]+21] = 0x10;    //0x10:16x16

    Line_showbuf_temp[line_len_temp[0]+22] = (uint8_t)(String_len[1]>>0)&0xff;
    Line_showbuf_temp[line_len_temp[0]+23] = (uint8_t)(String_len[1]>>8)&0xff;
    Line_showbuf_temp[line_len_temp[0]+24] = (uint8_t)(String_len[1]>>16)&0xff;
    Line_showbuf_temp[line_len_temp[0]+25] = (uint8_t)(String_len[1]>>24)&0xff;   

    for(i=0; i<String_len[1]; i++)
    {
        Line_showbuf_temp[line_len_temp[0]+26+i] = Line_showbuf_GBK_temp2[i]; 
    }

    line_len_temp[1] = 26 + String_len[1];  //strlen
    Line_showbuf_temp[line_len_temp[0]+1] = (uint8_t)(line_len_temp[1]>>0)&0xff; 
    Line_showbuf_temp[line_len_temp[0]+2] = (uint8_t)(line_len_temp[1]>>8)&0xff;
    Line_showbuf_temp[line_len_temp[0]+3] = (uint8_t)(line_len_temp[1]>>16)&0xff;
    Line_showbuf_temp[line_len_temp[0]+4] = (uint8_t)(line_len_temp[1]>>24)&0xff;   //整个数据域的大小
    
    /*---------------------第三行--------------------------------------*/
    if(strcmp(line4buf,"")==0)
    {
        line_len_temp[2] =0;
    }
    else
    {
        u2g(line4buf, strlen(line4buf), Line_showbuf_GBK_temp3, sizeof(Line_showbuf_GBK_temp3));
        String_len[2] = strlen(Line_showbuf_GBK_temp3);

        Line_showbuf_temp[line_len_temp[0]+line_len_temp[1]+0] = 0x03;
        Line_showbuf_temp[line_len_temp[0]+line_len_temp[1]+5] = 0x0E;   //字符内码

        Line_showbuf_temp[line_len_temp[0]+line_len_temp[1]+6] = 0x00;
        Line_showbuf_temp[line_len_temp[0]+line_len_temp[1]+7] = 0x00;  //x
        Line_showbuf_temp[line_len_temp[0]+line_len_temp[1]+8] = 0x1f;
        Line_showbuf_temp[line_len_temp[0]+line_len_temp[1]+9] = 0x00;  //左上角X\Y坐标 (0,0)
        Line_showbuf_temp[line_len_temp[0]+line_len_temp[1]+10] = 0x3F;
        Line_showbuf_temp[line_len_temp[0]+line_len_temp[1]+11] = 0x00;
        Line_showbuf_temp[line_len_temp[0]+line_len_temp[1]+12] = 0x2F;
        Line_showbuf_temp[line_len_temp[0]+line_len_temp[1]+13] = 0x00;  //右下角X\Y坐标 (64,32)

        Line_showbuf_temp[line_len_temp[0]+line_len_temp[1]+14] = 0x01;    //字体颜色  默认红
        Line_showbuf_temp[line_len_temp[0]+line_len_temp[1]+15] = 0x00;
        Line_showbuf_temp[line_len_temp[0]+line_len_temp[1]+16] = 0x00;

        Line_showbuf_temp[line_len_temp[0]+line_len_temp[1]+17] = 0x20;   //0x20:向左连续移动
        Line_showbuf_temp[line_len_temp[0]+line_len_temp[1]+18] = 0x00;

        Line_showbuf_temp[line_len_temp[0]+line_len_temp[1]+19] = 0x2F;   //速度
        Line_showbuf_temp[line_len_temp[0]+line_len_temp[1]+20] = 0xFF;   //每页停留时间

        Line_showbuf_temp[line_len_temp[0]+line_len_temp[1]+21] = 0x10;    //0x10:16x16

        Line_showbuf_temp[line_len_temp[0]+line_len_temp[1]+22] = (uint8_t)(String_len[2]>>0)&0xff;
        Line_showbuf_temp[line_len_temp[0]+line_len_temp[1]+23] = (uint8_t)(String_len[2]>>8)&0xff;
        Line_showbuf_temp[line_len_temp[0]+line_len_temp[1]+24] = (uint8_t)(String_len[2]>>16)&0xff;
        Line_showbuf_temp[line_len_temp[0]+line_len_temp[1]+25] = (uint8_t)(String_len[2]>>24)&0xff;   

        for(i=0; i<String_len[2]; i++)
        {
            Line_showbuf_temp[line_len_temp[0]+line_len_temp[1]+26+i] = Line_showbuf_GBK_temp3[i]; 
        }

        line_len_temp[2] = 26 + String_len[2];  //strlen
        Line_showbuf_temp[line_len_temp[0]+line_len_temp[1]+1] = (uint8_t)(line_len_temp[2]>>0)&0xff; 
        Line_showbuf_temp[line_len_temp[0]+line_len_temp[1]+2] = (uint8_t)(line_len_temp[2]>>8)&0xff;
        Line_showbuf_temp[line_len_temp[0]+line_len_temp[1]+3] = (uint8_t)(line_len_temp[2]>>16)&0xff;
        Line_showbuf_temp[line_len_temp[0]+line_len_temp[1]+4] = (uint8_t)(line_len_temp[2]>>24)&0xff;   //整个数据域的大小
    }
    /*---------------------语音--------------------------------------*/
    if(EN_playAudio ==1)
    {
        u2g(Audiobuf, strlen(Audiobuf), Line_showbuf_GBK_temp4, sizeof(Line_showbuf_GBK_temp4));           
        String_len[3] = strlen(Line_showbuf_GBK_temp4);
        if(line_len_temp[2] ==0)
        {
            Line_showbuf_temp[line_len_temp[0]+line_len_temp[1]+line_len_temp[2]+0] = 0x03;
        }
        else
        {
            Line_showbuf_temp[line_len_temp[0]+line_len_temp[1]+line_len_temp[2]+0] = 0x04;
        }              
        Line_showbuf_temp[line_len_temp[0]+line_len_temp[1]+line_len_temp[2]+5] = 0x2D;   //字符内码

        Line_showbuf_temp[line_len_temp[0]+line_len_temp[1]+line_len_temp[2]+6] = 0x00;
        Line_showbuf_temp[line_len_temp[0]+line_len_temp[1]+line_len_temp[2]+7] = 0x00;  
        Line_showbuf_temp[line_len_temp[0]+line_len_temp[1]+line_len_temp[2]+8] = 0x10;
        Line_showbuf_temp[line_len_temp[0]+line_len_temp[1]+line_len_temp[2]+9] = 0x00;  
        Line_showbuf_temp[line_len_temp[0]+line_len_temp[1]+line_len_temp[2]+10] = 0x3f;
        Line_showbuf_temp[line_len_temp[0]+line_len_temp[1]+line_len_temp[2]+11] = 0x00;
        Line_showbuf_temp[line_len_temp[0]+line_len_temp[1]+line_len_temp[2]+12] = 0x1f;
        Line_showbuf_temp[line_len_temp[0]+line_len_temp[1]+line_len_temp[2]+13] = 0x00;  
        Line_showbuf_temp[line_len_temp[0]+line_len_temp[1]+line_len_temp[2]+14] = 0x01;    
        Line_showbuf_temp[line_len_temp[0]+line_len_temp[1]+line_len_temp[2]+15] = 0x00;
        Line_showbuf_temp[line_len_temp[0]+line_len_temp[1]+line_len_temp[2]+16] = 0x00;
        Line_showbuf_temp[line_len_temp[0]+line_len_temp[1]+line_len_temp[2]+17] = 0x1b;   
        Line_showbuf_temp[line_len_temp[0]+line_len_temp[1]+line_len_temp[2]+18] = 0xff;
        Line_showbuf_temp[line_len_temp[0]+line_len_temp[1]+line_len_temp[2]+19] = 0x14;   
        Line_showbuf_temp[line_len_temp[0]+line_len_temp[1]+line_len_temp[2]+20] = 0x05;   
        Line_showbuf_temp[line_len_temp[0]+line_len_temp[1]+line_len_temp[2]+21] = 0x10;   //baoliu

        Line_showbuf_temp[line_len_temp[0]+line_len_temp[1]+line_len_temp[2]+22] = (uint8_t)(String_len[3]>>0)&0xff;
        Line_showbuf_temp[line_len_temp[0]+line_len_temp[1]+line_len_temp[2]+23] = (uint8_t)(String_len[3]>>8)&0xff;
        Line_showbuf_temp[line_len_temp[0]+line_len_temp[1]+line_len_temp[2]+24] = (uint8_t)(String_len[3]>>16)&0xff;  
        Line_showbuf_temp[line_len_temp[0]+line_len_temp[1]+line_len_temp[2]+25] = (uint8_t)(String_len[3]>>24)&0xff;    //字符串长度 

        for(i=0; i<String_len[3]; i++)
        {
            Line_showbuf_temp[line_len_temp[0]+line_len_temp[1]+line_len_temp[2]+26+i] = Line_showbuf_GBK_temp4[i]; 
        }

        line_len_temp[3] = 26 + String_len[3];
        Line_showbuf_temp[line_len_temp[0]+line_len_temp[1]+line_len_temp[2]+1] = (uint8_t)(line_len_temp[3]>>0)&0xff; 
        Line_showbuf_temp[line_len_temp[0]+line_len_temp[1]+line_len_temp[2]+2] = (uint8_t)(line_len_temp[3]>>8)&0xff;
        Line_showbuf_temp[line_len_temp[0]+line_len_temp[1]+line_len_temp[2]+3] = (uint8_t)(line_len_temp[3]>>16)&0xff;
        Line_showbuf_temp[line_len_temp[0]+line_len_temp[1]+line_len_temp[2]+4] = (uint8_t)(line_len_temp[3]>>24)&0xff;   //整个数据域的大小 

        return (line_len_temp[0]+line_len_temp[1]+line_len_temp[2]+line_len_temp[3]);     
    }   
    else
    {        
        return (line_len_temp[0]+line_len_temp[1]+line_len_temp[2]);
    }

}


int code_convert(char *from_charset, char *to_charset, char *inbuf, size_t inlen,
char *outbuf, size_t outlen) 
{
    iconv_t cd;
    char **pin = &inbuf;
    char **pout = &outbuf;
    
    cd = iconv_open(to_charset, from_charset);
    if (cd == 0)
    return -1;
    memset(outbuf, 0, outlen);
    if (iconv(cd, pin, &inlen, pout, &outlen) == -1)
    return -1;
    iconv_close(cd);
    *pout = '\0';
    
    return 0;
}
     
int u2g(char *inbuf, size_t inlen, char *outbuf, size_t outlen) 
{
    return code_convert("utf-8", "gb2312", inbuf, inlen, outbuf, outlen);
}
    
int g2u(char *inbuf, size_t inlen, char *outbuf, size_t outlen) 
{
    return code_convert("gb2312", "utf-8", inbuf, inlen, outbuf, outlen);
}



int LoadIniFile()
{
    dictionary *ini;
    const char *s;
    if(NULL == (ini = iniparser_load("LCD.ini")))
        return -1;

    //返回idictionary对象的section:key对应的整形值
    s = iniparser_getstring(ini, "LCD:stationFault_buf", "null");
    strcpy(stationFault_buf,s);    
    // printf("stationFault_buf : %s\n", s);

    s = iniparser_getstring(ini, "LCD:stationFault0_buf", "null");
    strcpy(stationFault0_buf,s);    
    // printf("stationFault0_buf : %s\n", s);

    s = iniparser_getstring(ini, "LCD:stationFault1_buf", "null");
    strcpy(stationFault1_buf,s);    
    // printf("stationFault1_buf : %s\n", s);

    s = iniparser_getstring(ini, "LCD:stationFault2_buf", "null");
    strcpy(stationFault2_buf,s);    
    // printf("stationFault2_buf : %s\n", s);

    s = iniparser_getstring(ini, "LCD:stationFireErr_buf", "null");
    strcpy(stationFireErr_buf,s);    
    // printf("stationFireErr_buf : %s\n", s);

    s = iniparser_getstring(ini, "LCD:stationFireErr0_buf", "null");
    strcpy(stationFireErr0_buf,s);    
    // printf("stationFireErr0_buf : %s\n", s);

    s = iniparser_getstring(ini, "LCD:stationFireErr1_buf", "null");
    strcpy(stationFireErr1_buf,s);    
    // printf("stationFireErr1_buf : %s\n", s);

    s = iniparser_getstring(ini, "LCD:carIn_buf", "null");
    strcpy(carIn_buf,s);    
    // printf("carIn_buf : %s\n", s);

    s = iniparser_getstring(ini, "LCD:carIn0_buf", "null");
    strcpy(carIn0_buf,s);    
    // printf("carIn0_buf : %s\n", s);

    s = iniparser_getstring(ini, "LCD:carNotInNet_buf", "null");
    strcpy(carNotInNet_buf,s);    
    // printf("carNotInNet_buf : %s\n", s);

    s = iniparser_getstring(ini, "LCD:carNotInNet0_buf", "null");
    strcpy(carNotInNet0_buf,s);    
    // printf("carNotInNet0_buf : %s\n", s);

    s = iniparser_getstring(ini, "LCD:carAbnormal_buf", "null");
    strcpy(carAbnormal_buf,s);    
    // printf("carAbnormal_buf : %s\n", s);

    s = iniparser_getstring(ini, "LCD:carAbnormal0_buf", "null");
    strcpy(carAbnormal0_buf,s);    
    // printf("carAbnormal0_buf : %s\n", s);

    s = iniparser_getstring(ini, "LCD:stationNoBattery_buf", "null");
    strcpy(stationNoBattery_buf,s);    
    // printf("stationNoBattery_buf : %s\n", s);

    s = iniparser_getstring(ini, "LCD:stationNoBattery0_buf", "null");
    strcpy(stationNoBattery0_buf,s);    
    // printf("stationNoBattery0_buf : %s\n", s);


    s = iniparser_getstring(ini, "LCD:orderCanceled_buf", "null");
    strcpy(orderCanceled_buf,s);    
    // printf("orderCanceled_buf : %s\n", s);

    s = iniparser_getstring(ini, "LCD:carRising_buf", "null");
    strcpy(carRising_buf,s);    
    // printf("carRising_buf : %s\n", s);

    s = iniparser_getstring(ini, "LCD:carRising0_buf", "null");
    strcpy(carRising0_buf,s);    
    // printf("carRising0_buf : %s\n", s);

    s = iniparser_getstring(ini, "LCD:carInPlace_buf", "null");
    strcpy(carInPlace_buf,s);    
    // printf("carInPlace_buf : %s\n", s);

    s = iniparser_getstring(ini, "LCD:carInPlace0_buf", "null");
    strcpy(carInPlace0_buf,s);    
    // printf("carInPlace0_buf : %s\n", s);

    s = iniparser_getstring(ini, "LCD:carInPlace1_buf", "null");
    strcpy(carInPlace1_buf,s);    
    // printf("carInPlace1_buf : %s\n", s);

    s = iniparser_getstring(ini, "LCD:changeBattSuccess_buf", "null");
    strcpy(changeBattSuccess_buf,s);    
    // printf("changeBattSuccess_buf : %s\n", s);

    s = iniparser_getstring(ini, "LCD:changeBattSuccess0_buf", "null");
    strcpy(changeBattSuccess0_buf,s);    
    // printf("changeBattSuccess0_buf : %s\n", s);

    iniparser_freedict(ini);

}
