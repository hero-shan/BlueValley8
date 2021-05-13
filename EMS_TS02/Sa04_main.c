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
#include"mymain.h"



/**  2020 12 31
1.修改模型：换电过程中发送暂停1s周期；解决报手动发送超时故障；
2.修改模型：换点过程中云端通信故障时，继续换电；
3.修改充电机中间层：增加换电结束后，清除取消订单标志和取消订单完成标志；
4.修改周期发云端中间层：解决云端通信标志诊断 **/
/**  2021 1 12
1.Qt上传消防故障；
2.增加空调3个连接状态
3.空调连接失败时，删除上传的空调数据
4.增加上传低压上电后的SOC
/**  2021 1 13
1.语音报允许出站提前到更换电池完成
2.解决语音播报换电完成请驶离，断网后不播报
3.水禁、消防故障后充电策略处理
4.有一个包充电就开风扇
5.消防断线重连，消防故障诊断
6.修改上传声光报警、风扇状态
**/
/**  2021 1 15
1.上传充电总故障去除连接故障
2.在预约数据库中增加先入先出数据
3.增加上电qt读先入先出数据库
4.0x90增加上传交流电表数据
5.增加云端进站、出站、ODO、取消订单失败原因
6.增加交流电表控制指令
7.修改上传云端驱动 无法识别空格问题
8.上传入网失败故障
**/
/**  2021 1 19
1.自检不接收车牌;
2.无可换电池包与进站失败不同时发；
3.上传空调信息周期改为1.2s;
4.询问车型报出故障原因
**/
/**  2021 1 21
1.上传预约个数不用放置个数
2.上电读取先入先出加不为零条件
3.温度数据库写入个数
4.增加输入ODO及放行订单已经完成Switch completed
5.修改模型放行时清除请求ODO标志
6.到位弹窗标志清除
**/
/**  2021 1 27
1.修改写数据库周期为60s(空调数据/充电数据/电池基本信息);
2.增加与上位机断开连接后关闭下位机进程;
3.数据库条数超过20w条后，删除前10w条(空调数据/充电数据/电池基本信息);
4.先入先出初始化上位机下发为0时，不接收
5.等待车辆到位状态取消上升沿判断(mode)
**/
/**  2021 1 28
1.new IO_Card  pthread
**/
/**  2021 2 3
1.数据上传优化CJson;
2.换电设备未连接时，放置信息清0;
3.充电过程中，点击低压下电不响应，这时点击停止充电，充电机停止充电，并执行低压下电(mode);
4.空调在断开时，不进行存储数据;
5.充电自停止码未清零;
6.三个驱动重连
**/
/**  2021 2 5
1.上位机崩溃后，根据进程名5s结束下位机进程;
2.qt上传直流电表数据;
3.模型数据库调度，qt上传直流电表调度;
4.远程退网锁包;
5.充电数据库增加直流电表;
6.交流电表写数据库;
7.板卡初始化值1;
8.模型清除到位下降沿；模型修改直流电表上传Qt时间；
**/
/**  2021 2 20
1.修改模型；
2.修改配置文件，增加修改url;
3.故障复位增加清除手动换电超时故障;
4.屏蔽发送预约请求;
5.增加系统崩溃日志存储;
**/
/**  2021 2 24
1.删除数据上传/周期发/点法云端curl_global_init和curl_global_cleanup();
2.主函数增加curl_global_init和curl_global_cleanup();
3.数据上传/周期发/点法云端增加CURLOPT_NOSIGNAL设为1;
4.修改模型手动换电等操作条件去掉泊车平台初始位置;
5.接受Qt下发先入先出数据处理，并打印日志;
6.增加禁用板卡信息Qt，板卡中间层
**/
/**  2021 3 1
1.修改模型手动换电删除泊车平台；
2.进入自动换电后，初始化清除换电完成信号;
**/
/**  2021 3 8
1.周期发云端与上传数据两个线程合并;
2.增加libcurl互斥锁在周期发和点发两个驱动中;
3.抓车牌只在15状态（等待车辆状态）
**/
/**  2021 3 13
1.mode 清车牌；清进站出站状态标志,修改进站标志发送时间10s;
2.+log;
3.配置文件标题去掉空格
**/
/**  2021 3 15
1.mode 清取消订单完成标志;
2.curl超时时间改为8s;
3.
**/
/* 2021 03 18
* 1. 入站，出战，取消订单，odo等加上日志*
* 2. eu5单体电压显示异常修复*
* 3. 数据库 未完成订单，参数配置 执行错误后，500ms后重试1次
*/
/**  2021 3 19
1.增加校验偷包问题,并上传Qt偷包标志;
2.云端超时时间改为30s;
**/
/**  2021 3 24
1.光栅触发时间50ms mode
2.添加换电超时时间
3.订单取消 语音去掉请驶离
4.电池包丢包update
5.下位机接收到的上位机下发换电完成标志清除
6.增加故障打印日志
**/
/**  2021 3 30
1.交流电表update;
**/
/**  2021 4 2
1.交流电表reconnect;
2.滑出电池包修改
**/

//#include "mathcalls.h"
// #include <pthreadtypes.h>
#define PORT  5000
#define BUFFER_SIZE 1024

extern void thread_CE(void);
extern void pthread_AV(void);
extern void thread_Qt(void);
extern void thread_ModeCharger(void);
extern void pthread_net_burst(void);
extern void pthread_net_cycle(void);
extern void pthread_sqlite(void);
extern void thread_IOCard(void);
extern void thread_FireContr(void);
extern void thread_AirCon(void);
extern void pthread_CHU1(void);
extern void pthread_CHU2(void);
extern void pthread_CHU3(void);
extern void pthread_CHU4(void);
extern void pthread_CHU5(void);
extern void pthread_CHU6(void);
extern void pthread_CHU7(void);
extern void pthread_CHU8(void);
extern void pthread_CHU9(void);
extern void pthread_CHU10(void);
extern void pthread_CHU11(void);
extern void pthread_CHU12(void);
extern void pthread_CHU13(void);
extern void pthread_CHU14(void);
extern void pthread_CHU15(void);
extern void pthread_CHU16(void);
extern void pthread_CHU17(void);
extern void pthread_CHU18(void);
extern void thread_ACEnCon(void);
extern void thread_Remote(void);
extern void pthread_dataUpload(void) ;

extern void thread_DCEnCon(void);//2021

pid_t gettid()
{
	return syscall(SYS_gettid);
}
void sigHandler(int signo) 
{
	// printf("sasa %d\n",gettid());
	syslog(LOG_INFO,"Crash gettid '%d'\n",gettid());
	// LOG_ERROR_ARGS("=====recv SIGINT %d=====", signo);
	
	// //打印错误堆栈信息
	// LOG_ERROR("----------------------------Dump Program Error Strings-------------------------");
	int j = 0, nptrs = 0;
 	void* buffer[1000] ;
 	char** strings = NULL;
 	nptrs = backtrace(buffer, 1000);
 //	printf("backtrace() returned %d addresses\n", nptrs);
 	strings = backtrace_symbols(buffer, nptrs);
	// backtrace_symbols_fd(buffer, nptrs,stdout);

 	if (strings == NULL) {
		syslog(LOG_INFO,"backtrace_symbols null\n");
 // 		printf("backtrace_symbols null\n");
 // 		printf("-------------------------------------------------------------------------------");
  		exit(0);
 	}

 	for (j = 0; j < nptrs; j++) {
//  		printf("  [%02d] %s", j, strings[j]);
		syslog(LOG_INFO,"Crash!!! '%s'\n",strings[j]);
 	}

 	free(strings);
	// LOG_ERROR("-------------------------------------------------------------------------------");
	
	//恢复默认信号操作
	signal(signo, SIG_DFL);
  	raise(signo);
}

pthread_mutex_t mutex_lock;

int main(void)
{
	pthread_mutex_init(&mutex_lock,NULL);

    pthread_t id_1,id_2,id_3,id_4,id_5,id_6,id_7,id_8,id_9,id_10,id_11,id_12,id_13,
				id_14,id_15,id_16,id_17,id_18,id_19,id_20,id_21,id_22,id_23,id_24,id_25,
				id_26,id_27,id_28,id_39,id_40,id_41,id_42;
    int i,ret;

	static int16_t Qt_ShutDown_count = 0;
	signal(SIGSEGV, sigHandler);
	signal(SIGABRT, sigHandler);
	signal(SIGQUIT, sigHandler);
//	printf("sa0000 %d\n",gettid());

	curl_global_init(CURL_GLOBAL_SSL|CURL_GLOBAL_ALL );

/*创建线程一 */ 
	ret=pthread_create(&id_1,NULL,(void  *) thread_CE,NULL);    		
	if(ret!=0)    	
	{        	
//		printf("Create pthread 1 error!\n");    		
		return -1;    	
	}
	else
	{
//		printf("thread 1 OK!\n");
	}  	
	/*创建线程二  */
	ret=pthread_create(&id_2,NULL,(void  *) pthread_AV,NULL);			
			
	/*创建线程三  */ 
	ret=pthread_create(&id_3,NULL,(void  *) thread_Qt,NULL);
				
	/*创建线程4 */  
	ret=pthread_create(&id_4,NULL,(void  *) thread_ModeCharger,NULL);			
	/*创建线程5 */ 
	ret=pthread_create(&id_5,NULL,(void  *) pthread_net_burst,NULL);			
	/*创建线程6 */
	ret=pthread_create(&id_6,NULL,(void  *) pthread_net_cycle,NULL);
/*创建线程39 */ 
	ret=pthread_create(&id_39,NULL,(void  *) thread_DCEnCon,NULL);
#if 0			
	/*创建线程7 */ 
	ret=pthread_create(&id_7,NULL,(void  *) pthread_sqlite,NULL);			
	/*创建线程8 */
	ret=pthread_create(&id_8,NULL,(void  *) thread_IOCard,NULL);			
	/*创建线程9*/  
	ret=pthread_create(&id_9,NULL,(void  *) thread_FireContr,NULL);			
	/*创建线程10 */
	ret=pthread_create(&id_10,NULL,(void  *) thread_AirCon,NULL);		
	 
	/*创建线程11 */ 
	ret=pthread_create(&id_11,NULL,(void  *) pthread_CHU1,NULL);			
	/*创建线程12 */ 
	ret=pthread_create(&id_12,NULL,(void  *) pthread_CHU2,NULL);			
	/*创建线程13 */ 
	ret=pthread_create(&id_13,NULL,(void  *) pthread_CHU3,NULL);
	/*创建线程14 */ 
	ret=pthread_create(&id_14,NULL,(void  *) pthread_CHU4,NULL);
	/*创建线程15 */ 
	ret=pthread_create(&id_15,NULL,(void  *) pthread_CHU5,NULL);
	/*创建线程16 */ 
	ret=pthread_create(&id_16,NULL,(void  *) pthread_CHU6,NULL);
	/*创建线程17 */ 
	ret=pthread_create(&id_17,NULL,(void  *) pthread_CHU7,NULL);
	/*创建线程18 */ 
	ret=pthread_create(&id_18,NULL,(void  *) pthread_CHU8,NULL);
	/*创建线程19 */ 
	ret=pthread_create(&id_19,NULL,(void  *) pthread_CHU9,NULL);
	/*创建线程20 */ 
	ret=pthread_create(&id_20,NULL,(void  *) pthread_CHU10,NULL);
	/*创建线程21 */ 
	ret=pthread_create(&id_21,NULL,(void  *) pthread_CHU11,NULL);
	/*创建线程22 */ 
	ret=pthread_create(&id_22,NULL,(void  *) pthread_CHU12,NULL);
	/*创建线程23 */ 
	ret=pthread_create(&id_23,NULL,(void  *) pthread_CHU13,NULL);
	/*创建线程24 */ 
	ret=pthread_create(&id_24,NULL,(void  *) pthread_CHU14,NULL);
	/*创建线程25 */ 
	ret=pthread_create(&id_25,NULL,(void  *) pthread_CHU15,NULL);
	/*创建线程26 */ 
	ret=pthread_create(&id_26,NULL,(void  *) pthread_CHU16,NULL);
	/*创建线程27 */ 
	ret=pthread_create(&id_27,NULL,(void  *) pthread_CHU17,NULL);
	/*创建线程28 */ 
	ret=pthread_create(&id_28,NULL,(void  *) pthread_CHU18,NULL);
	/*创建线程29 */ 
	ret=pthread_create(&id_29,NULL,(void  *) pthread_CHU19,NULL);
	/*创建线程30 */ 
	ret=pthread_create(&id_30,NULL,(void  *) pthread_CHU20,NULL);

	/*创建线程40 */ 
	ret=pthread_create(&id_40,NULL,(void  *) thread_ACEnCon,NULL);
	/*创建线程41 */ 
	// ret=pthread_create(&id_41,NULL,(void  *) thread_Remote,NULL);
	/*创建线程42 */ 
	// ret=pthread_create(&id_42,NULL,(void  *) pthread_dataUpload,NULL);
#endif	
	parse_ini_file("example.ini");

	SA4_EMS_initialize();
	openlog("EMS_LOCAL2", LOG_CONS | LOG_PID, LOG_LOCAL2);

	while(1)
	{

		sleep(1);		//do something or delay
		GetMacTime();
		if(QtSend_Discon_con<15) QtSend_Discon_con++;
		else QtSend_Discon_con=16;
		if(CESend_Discon_con<15) CESend_Discon_con++;
		else CESend_Discon_con=16;
		// if(FireContrSend_Discon_con<15)FireContrSend_Discon_con++;
		// else FireContrSend_Discon_con = 16;
		for(i=0; i<3; i++)
		{
			if(AirConSend_Discon_con[i]<15)AirConSend_Discon_con[i]++;
			else{
				AirConSend_Discon_con[i] = 16;
				AirCon_Eachconn_sta[i] = 2;
			}
		}

		if(IOCardSend_Discon_con[0]<15)IOCardSend_Discon_con[0]++;
		else 
		{
			IOCardSend_Discon_con[0] = 16;
			MIO_Comm_flg = 2;
		}
		if(IOCardSend_Discon_con[1]<15)IOCardSend_Discon_con[1]++;
		else
		{ 
			IOCardSend_Discon_con[1] = 16;
			MIO_Comm_flg = 2;
		}

		if(RemotSend_Discon_con<30)RemotSend_Discon_con++;
		else RemotSend_Discon_con = 36;
		if(ACEnConSend_Discon_con<15)ACEnConSend_Discon_con++;
		else ACEnConSend_Discon_con = 16;
		if(DCEnConSend_Discon_con<15)DCEnConSend_Discon_con++;
		else DCEnConSend_Discon_con = 16;
		for(i=0;i<18;i++)
		{
			if(CHUSend_Discon_cou[i]<15)CHUSend_Discon_cou[i]++;
			else CHUSend_Discon_cou[i] = 16;
		}
		{
			Cloud_Comm_err_count++;
			if(Cloud_Comm_err_count >= 10*3)
				MCloud_Comm_flg = 2,Cloud_Comm_err_count=50; 
		}
		// printf("chu_test_state=%d,%d,%d,%d,%d\n",chu_test_state[0],MQt_ConfDa_push,MSqlite_WriConfigData_flg,
		// 										MSqlite_WrtErr_flg,MLocat_SelfChar_ena[0]);
	//	printf("SysSta11=%d,%f,%d,%d,%d,%d,%d,%d \n",
	//	Sta,acA_ActivePower,CE_LocatorSenser[14],MLed_Big_sta,MLocat_Car_type[11],MLocat_BookLock_flg[11],MLocat_FIFO_Buf[11],CHU_AllErr_flg[11]);
		
		// printf("SysSta22=%d,%d,%d,%d,%s,%d \n",
		// Book_Init_flg,MLocat_RealBook_num[1],Hand_LowPwrOn_flg[15],chu_test_state[11],Cloud_BookcarNo2[1],Clear_Book_num[1]);
		// printf("SysSta33=%d,%d,%d,%d,%d,%d \n",
		// MLocat_Conf_ena[3],MLocat_SelfChar_ena[3],MLocat_InNet_flg[3],MLocat_GetBatCode_flg[3],MLocat_Car_type[3],MCE_GetSqlInt_sta);
		// printf("SysSta16=%f,%f,%f,%d,%d,%d \n",
		// rtDW.RemotSend_UpAirCon_cou,rtDW.RemotSend_UpPLC_cou,rtDW.RemotSend_UpACEng_cou,MLocat_InNet_flg[15],MLocat_GetBatCode_flg[15],MLocat_Car_type[15]);
		for(i=0;i<20;i++){
//			printf("%d=%d ",i,MLocat_FIFO_Buf[i]);
		}
		
		// openlog("EMS_LOCAL2", LOG_CONS | LOG_PID, LOG_LOCAL2);
   		syslog(LOG_INFO,"Sta '%d',MSys_AllErr_Lev '%d', Qt_conn_flg '%d',MIO_LightValue_null[0],QtIo_Disable_buf '%d%d%d%d %d%d%d%d' \n",\
		   Sta,MSys_AllErr_Lev,Qt_conn_flg,\
		   MIO_LightValue_null[0],MIO_LightrErr_buf[0],QtIo_Disable_buf[2],QtIo_Disable_buf[3],\
		   QtIo_Disable_buf[4],QtIo_Disable_buf[5],QtIo_Disable_buf[6],QtIo_Disable_buf[7]);


		FILE *fp = popen("ps -e | grep \'SA04_EMS_Screen\' | awk \'{print $1}\'","r");
		char buffer[10] = {0};
		while(NULL != fgets(buffer,10,fp))
		{
//			printf("PID: = %s\n",buffer);
		}
		pclose(fp);
		/*
		if(strcmp(buffer,"")==0)
		{
			Qt_ShutDown_count++;
			if(Qt_ShutDown_count>=5)
			{
				syslog(LOG_INFO,"POPEN ShutDown!!! \n");
				Qt_ShutDown_count = 0;
				break;
			}
		}
		else
		{
			Qt_ShutDown_count = 0;
		}
		*/
	}
	return 0;
}
