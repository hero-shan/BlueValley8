#include <stdio.h>
#include <sys/vfs.h>
#include <sqlite3.h>
#include <bits/wordsize.h>
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
#include "string.h"
#include "task_sqlite.h"
#include "task_charger.h"

#include "mymain.h"

mid_sqlite_t mid_sqlite;

int MCE_WriShutDown_sta = 0;
int MCE_WriSqlShutDown_sta_last = 0;

char NowTime[100];/*系统当前时间*/
char CE_StartTime[100];/*换电开始时间*/

uint8_t DB_FileName_buf[100];/*数据库文件名称*/

char cmdBuf[2000];

/*不同table创建指针*/
uint8_t *Sql_CE_Record;
uint8_t *Sql_Batt_Data;
uint8_t *Sql_Char_Data;
uint8_t *Sql_Config_Data;
uint8_t *Sql_Err_Data;
uint8_t *Sql_AirCon_Data;
uint8_t *Sql_Locator_Data;
uint8_t *Sql_acEnergy_Record;

uint8_t Sql_CE_Record_buf[1000];/**/
uint8_t Sql_BattData_buf[1000];
uint8_t Sql_CharData_buf[5000];
uint8_t Sql_ConfigData_buf[5000];
uint8_t Sql_ErrData_buf[5000];
uint8_t Sql_AirConData_buf[5000];
uint8_t Sql_LocatorData_buf[5000];
uint8_t Sql_acEnergyData_buf[2000];

/*写数据库标志位*/
uint8_t Sql_WrtCEDada_flg = 0;/**/
uint8_t Sql_WrtBattData_flg = 0;/**/
uint8_t Sql_WrtCharData_flg[20] = {0};/**/
uint8_t Sql_WrtConfigDada_flg = 0;/**/
uint8_t Sql_WrtErrData_flg = 0;/**/
uint8_t Sql_WrtAirConData_flg = 0;/**/
uint8_t Sql_WrtLocatorData_flg = 0;/**/

char *Sql_UnfinishedOrder_Data;
char *Sql_appointment_Data;
char Sql_UnfinishedOrder_buf[500];
char Sql_appointment_buf[500];


/*写数据库 err 标志位 */
uint8_t Sql_Wrt_err_flg = 0;
/*数据库句柄*/
sqlite3 *db=NULL;

uint8_t SourseStorage_Batt_NUM[40];
uint8_t SourseStorage_Grab_carNo[30];
uint8_t SourseStorage_OutBatt_NUM[50];

uint8_t ChecklostBat_CarBatt_NUM[50];
uint8_t ChecklostBat_err_flg = 0;

uint8_t SourseStorage_carType[30];
uint8_t SourseStorage_orderId[50];
double ReplaceBatt_Available_Energy=0;
double ReplaceBatt_Available_Cap=0;
double ReplaceBatt_SOC=0;

double RBCar_Batt_Available_Energy=0;
double RBCar_Batt_Available_Cap=0;
double RBCar_Batt_Soc=0;
uint8_t SqlCloud_BookNum[3]={0};
uint8_t Sql_DeleMonth_buf[3] = {0};

uint8_t sql_BattNum[40];
void Sql_WriCeData()
{
	if(mid_sqlite.MSql_WriCeStartData_flg==1)	{ //&&strcmp(sql_BattNum,"")!=0
		mid_sqlite.MSql_WriCeStartData_flg = 0;
		strcpy(CE_StartTime,NowTime);//车辆到站时间
		strcpy(SourseStorage_carType,mid_sqlite.Cloud_carType_now);
		//read the batt num
		strcpy(SourseStorage_Batt_NUM,sql_BattNum);

		strcpy(SourseStorage_OutBatt_NUM,mid_sqlite.Cloud_BatteryNo_now);
		strcpy(SourseStorage_Grab_carNo,mid_sqlite.Cloud_Grab_carNo);
		strcpy(SourseStorage_orderId,Cloud_orderId_now);
		// strcpy(SourseStorage_Batt_NUM,"00MPE0JC431D0MA431019001");//源库位电池编号
		ReplaceBatt_Available_Energy = BMS_BATT_AVAIL_ENERGY[MChar_SelfCe_sour];//换上电池可用能量
		ReplaceBatt_Available_Cap = BMS_RatedCap_Ah[MChar_SelfCe_sour];//换上电池可用容量
		ReplaceBatt_SOC = BMS_BATT_SOC[MChar_SelfCe_sour];//换上SOC

		strcpy(ChecklostBat_CarBatt_NUM,SourseStorage_OutBatt_NUM);
	}
	if(mid_sqlite.MSql_WriCeOkData_flg==1&&mid_sqlite.chu_test_state[MChar_SelfCe_sour_old]>=10&&
		mid_sqlite.CHU_GetBatBasic_flg[MChar_SelfCe_sour_old]==1)
	{
		mid_sqlite.MSql_WriCeOkData_flg = 0;
		MSqlite_WriCEData_flg = 1;
		RBCar_Batt_Available_Energy = BMS_BATT_AVAIL_ENERGY[MChar_SelfCe_sour_old];//换下电池可用能量
		RBCar_Batt_Available_Cap = BMS_RatedCap_Ah[MChar_SelfCe_sour_old];//换下电池可用容量
		RBCar_Batt_Soc = BMS_BATT_SOC[MChar_SelfCe_sour_old];//换下SOC

		syslog(LOG_INFO,"CheckLostBatt MLocat_BatteryNo == %s %s %d\n",MLocat_BatteryNo[MChar_SelfCe_sour_old],\
		ChecklostBat_CarBatt_NUM,MChar_SelfCe_sour_old);
		if(strcmp(MLocat_BatteryNo[MChar_SelfCe_sour_old],"")!=0){
			if(strcmp(ChecklostBat_CarBatt_NUM,MLocat_BatteryNo[MChar_SelfCe_sour_old])!=0){
				ChecklostBat_err_flg = 1;
			}
		}
		
	}
}
/*
*数据库写入线程，接受主逻辑调度，
*当需要存入数据库时，自动响应请求，并将需要存储的数据打包写入数据库
*
*/
void SqlErr_interface();
void sql_WriteErrData();

/*数据库中间层对接函数*/
void mid_sqlite_Interface()
{
	int i = 0;
	mid_sqlite.MSql_WriCeStartData_flg = MSql_WriCeStartData_flg;	MSql_WriCeStartData_flg = 0;
	if(MSql_WriCeOkData_flg == 1)
	{
		mid_sqlite.MSql_WriCeOkData_flg = MSql_WriCeOkData_flg;	
		MSql_WriCeOkData_flg = 0;
	}

	mid_sqlite.MSqlite_WriConfigData_flg = MSqlite_WriConfigData_flg;	MSqlite_WriConfigData_flg = 0;
	mid_sqlite.MSqlite_WriAirConData_flg = MSqlite_WriAirConData_flg;	MSqlite_WriAirConData_flg = 0;
	mid_sqlite.MSql_WriUnfinishedOrder_flg = MSql_WriUnfinishedOrder_flg;	MSql_WriUnfinishedOrder_flg = 0;
	mid_sqlite.MSql_DelUnfinishedOrder_flg = MSql_DelUnfinishedOrder_flg; 	MSql_DelUnfinishedOrder_flg = 0;
	mid_sqlite.MCE_WriSqlShutDown_sta = MCE_WriSqlShutDown_sta;	

	// mid_sqlite.MChar_SelfCe_sour = MChar_SelfCe_sour;
	mid_sqlite.MLocat_CharEndSoc_pct = MLocat_CharEndSoc_pct;
	mid_sqlite.MLocat_AllowCESoc_pct = MLocat_AllowCESoc_pct;
	mid_sqlite.MLocat_CharPow_kw = MLocat_CharPow_kw;
	// mid_sqlite.MChar_SelfCe_sour = MChar_SelfCe_sour;
	
	strcpy(mid_sqlite.Cloud_carType_now,Cloud_carType_now);
	strcpy(mid_sqlite.Cloud_BatteryNo_now,Cloud_BatteryNo_now);
	strcpy(mid_sqlite.Cloud_Grab_carNo,Cloud_Grab_carNo);

	for(i=0;i<20;i++)
	{
		mid_sqlite.MSqlite_WriBattData_flg[i] = MSqlite_WriBattData_flg[i];		MSqlite_WriBattData_flg[i] = 0;
		mid_sqlite.MSqlite_WriCharData_flg[i] = MSqlite_WriCharData_flg[i];		MSqlite_WriCharData_flg[i] = 0;
		mid_sqlite.MSqlite_WriLocatData_flg[i] = MSqlite_WriLocatData_flg[i];	MSqlite_WriLocatData_flg[i] = 0;

		mid_sqlite.chu_test_state[i] = chu_test_state[i];
		mid_sqlite.CHU_GetBatBasic_flg[i] = CHU_GetBatBasic_flg[i];
		mid_sqlite.MLocat_SelfChar_ena[i] = MLocat_SelfChar_ena[i];
		mid_sqlite.MLocat_BookLock_flg[i] = MLocat_BookLock_flg[i];

		strcpy(mid_sqlite.MLocat_BatteryNo[i],MLocat_BatteryNo[i]);
		strcpy(mid_sqlite.Cloud_BookcarNo1[i],Cloud_BookcarNo1[i]);
		strcpy(mid_sqlite.Cloud_BookcarNo2[i],Cloud_BookcarNo2[i]);
		strcpy(mid_sqlite.Cloud_BookcarNo3[i],Cloud_BookcarNo3[i]);
	}
	for(i=0;i<3;i++)
	{
		mid_sqlite.AirCon_Temp[i] = AirCon_Temp[i];
		mid_sqlite.AirCon_Humi[i] = AirCon_Humi[i];
	}
}

char *zErrMsg = NULL;
void pthread_sqlite(void)
{
//	printf("pthread_sqlite tid %d\n",gettid());
	syslog(LOG_INFO,"pthread_sqlite gettid '%d'\n",gettid());

	int ret = 0;
	// char *zErrMsg = NULL;
	int i;
	
	sqlite3_initialize();

	sprintf(DB_FileName_buf,"SA04_station.db");
	//打开数据库文件,如果不存在将创建一个数据库文件
	ret = sqlite3_open_v2(DB_FileName_buf,&db,SQLITE_OPEN_READWRITE|SQLITE_OPEN_CREATE,NULL); 
	if( ret )
	{
		sqlite3_close_v2(db);
		sqlite3_shutdown();
		//return 0;
	}
//	printf( "Open %s ok!\n",DB_FileName_buf);
	//1
	Create_CE_Record_Table();
	ret = sqlite3_exec( db, Sql_CE_Record, 0, 0, &zErrMsg );
	if(ret)
	{
//		printf( "Create_CE_Record_Table err. ret= :%d ErrMsg:%s\n",ret,zErrMsg);
		syslog(LOG_INFO,"Create_CE_Record_Table err. ret = : '%d' %s \n",ret,zErrMsg);
	}
	//2
	Create_BattData_Table();
    ret = sqlite3_exec( db, Sql_Batt_Data, 0, 0, &zErrMsg );
	if(ret)
	{
//		printf( "Create_BattData_Table err. ret= :%d ErrMsg:%s\n",ret,zErrMsg);
		syslog(LOG_INFO,"Create_BattData_Table err. ret = : '%d' %s \n",ret,zErrMsg);
	}
	//3
	Create_CharData_Table();
    ret = sqlite3_exec( db, Sql_Char_Data, 0, 0, &zErrMsg );
	if(ret)
	{
//		printf( "Create_CharData_Table err. ret= :%d ErrMsg:%s\n",ret,zErrMsg);
		syslog(LOG_INFO,"Create_CharData_Table err. ret = : '%d' %s \n",ret,zErrMsg);
	}
	//4
	Create_ConfigData_Table();
    ret = sqlite3_exec( db, Sql_Config_Data, 0, 0, &zErrMsg );
	if(ret)
	{
//		printf( "Create_ConfigData_Table err. ret= :%d ErrMsg:%s\n",ret,zErrMsg);
		syslog(LOG_INFO,"Create_ConfigData_Table err. ret = : '%d' %s \n",ret,zErrMsg);
	}
	//5
	Create_ErrData_Table();
    ret = sqlite3_exec( db, Sql_Err_Data, 0, 0, &zErrMsg );
	if(ret)
	{
//		printf( "Create_ErrData_Table err. ret= :%d ErrMsg:%s\n",ret,zErrMsg);
		syslog(LOG_INFO,"Create_ErrData_Table err. ret = : '%d' %s \n",ret,zErrMsg);
	}
	//6
	Create_AirConData_Table();
    ret = sqlite3_exec( db, Sql_AirCon_Data, 0, 0, &zErrMsg );
	if(ret)
	{
//		printf( "Create_AirConData_Table err. ret= :%d ErrMsg:%s\n",ret,zErrMsg);
		syslog(LOG_INFO,"Create_AirConData_Table err. ret = : '%d' %s \n",ret,zErrMsg);
	}
	//7
	Create_LocatorData_Table();
    ret = sqlite3_exec( db, Sql_Locator_Data, 0, 0, &zErrMsg );
	if(ret)
	{
//		printf( "Create_LocatorData_Table err. ret= :%d ErrMsg:%s\n",ret,zErrMsg);
		syslog(LOG_INFO,"Create_LocatorData_Table err. ret = : '%d' %s \n",ret,zErrMsg);
	}
	//8
	Create_UnfinishedOrder_Table();
	ret = sqlite3_exec( db, Sql_UnfinishedOrder_Data, 0, 0, &zErrMsg );
	if(ret)
	{
//		printf( "Create_unfini_Table err. ret= :%d ErrMsg:%s\n",ret,zErrMsg);
		syslog(LOG_INFO,"Create_unfini_Table err. ret = : '%d' %s \n",ret,zErrMsg);
	}
	//9
    {
		char *cmd = SQL_CHANGEBATT_RT_DATA;
		ret = sqlite3_exec( db, cmd, 0, 0, &zErrMsg );
		if(ret)
		{
//			printf( "Create_Table err. ret= :%d ErrMsg:%s\n",ret,zErrMsg);
			syslog(LOG_INFO,"Create_Table err. ret = : '%d' %s \n",ret,zErrMsg);
		}

		sql_AddRTTable();
		ret = sqlite3_exec( db, cmdBuf, 0, 0, &zErrMsg );
		if(ret)
		{
			if(!( strcmp(zErrMsg,"UNIQUE constraint failed") == 0))
				printf( "insert _Table err. ret= :%d ErrMsg:%s\n",ret,zErrMsg);
		}
	}
	//10
	Create_appointment_Table();
	ret = sqlite3_exec( db, Sql_appointment_Data, 0, 0, &zErrMsg );
	if(ret)
	{
//		printf( "Create_appoint_Table err. ret= :%d ErrMsg:%s\n",ret,zErrMsg);
		syslog(LOG_INFO,"Create_appoint_Table err. ret = : '%d' %s \n",ret,zErrMsg);
	}
	//Sql_acEnergy_Record
	Create_acEnergy_Table();
	ret = sqlite3_exec( db, Sql_acEnergy_Record, 0, 0, &zErrMsg );
	if(ret)
	{
//		printf( "Create_acEnergy_Table err. ret= :%d ErrMsg:%s\n",ret,zErrMsg);
		syslog(LOG_INFO,"Create_acEnergy_Table err. ret = : '%d' %s \n",ret,zErrMsg);
	}


	while(1)
	{
        // GetMacTime();
		DatLib();

		mid_sqlite_Interface();

		Sql_WriCeData();

		//1
		if(1 == MSqlite_WriCEData_flg)
		{
			MSqlite_WriCEData_flg = 0;
			Sql_CEDataInter_Deal();
//			printf("%ld\n",strlen(Sql_CE_Record_buf));
			ret = sqlite3_exec( db , Sql_CE_Record_buf , 0 , 0 , &zErrMsg );
			if(ret)
			{
//				printf( "insert ce data err. ret = :%d\n" , ret) ;
				syslog(LOG_INFO,"insert ce datar err. ret = : '%d' %s \n"
						,ret,zErrMsg);
				Sql_Wrt_err_flg = 1;
				usleep(500*1000);
				ret = sqlite3_exec( db , Sql_CE_Record_buf , 0 , 0 , &zErrMsg );
			}
		}			
		//2
		for(i = 0;i<20;i++)
		{
			if(1 == mid_sqlite.MSqlite_WriBattData_flg[i])
			{
				mid_sqlite.MSqlite_WriBattData_flg[i] = 0;
				Sql_BattDataInter_Deal(i);
				// printf("%s\n",Sql_BattData_buf);
				// printf("%ld\n",strlen(Sql_BattData_buf));
				ret = sqlite3_exec( db , Sql_BattData_buf , 0 , 0 , &zErrMsg );
				if(ret)
				{
//					printf( "insert batt data err. ret = :%d\n" , ret) ;
					syslog(LOG_INFO,"insert batt datar err. ret = : '%d' %s \n"
						,ret,zErrMsg);
					Sql_Wrt_err_flg = 1;
				}
			}
		}
		//3
		for(i = 0;i<20;i++)
		{
			if(1 == mid_sqlite.MSqlite_WriCharData_flg[i])
			{
//				printf( "MSqlite_WriCharData_flg =%d\n",mid_sqlite.MSqlite_WriCharData_flg[1]);
				mid_sqlite.MSqlite_WriCharData_flg[i] = 0;
				Sql_CharInter_Deal(i);
				// printf("%s\n",Sql_CharData_buf);
				ret = sqlite3_exec( db , Sql_CharData_buf , 0 , 0 , &zErrMsg );
				if(ret)
				{
//					printf( "insert char data[%d] err. ret = :%d\n",i, ret) ;
					syslog(LOG_INFO,"insert char datar err. ret = : '%d' %s \n"
						,ret,zErrMsg);
					Sql_Wrt_err_flg = 1;
				}
			}
		}	
		//4
		if(1 == mid_sqlite.MSqlite_WriConfigData_flg)
		{
//			printf( "Write ConfigData\n") ;
			mid_sqlite.MSqlite_WriConfigData_flg = 0;
			Sql_ConfInter_Deal();
			ret = sqlite3_exec( db , Sql_ConfigData_buf , 0 , 0 , &zErrMsg );
			if(ret)
			{
//				printf( "insert config data err. ret = :%d\n" , ret) ;
				syslog(LOG_INFO,"insert config datar err. ret = : '%d' %s \n"
						,ret,zErrMsg);
				Sql_Wrt_err_flg = 1;
				usleep(500*1000);
				ret = sqlite3_exec( db , Sql_ConfigData_buf , 0 , 0 , &zErrMsg );
			}
		}
		//5
		if(0)
		// if(1 == MSqlite_WriErrData_flg)
		{
			MSqlite_WriErrData_flg = 0;
			Sql_ErrInter_Deal();
			ret = sqlite3_exec( db , Sql_ErrData_buf , 0 , 0 , &zErrMsg );
			if(ret)
			{
//				printf( "insert err data err. ret = :%d\n" , ret) ;
				Sql_Wrt_err_flg = 1;
			}
		}		
		sql_WriteErrData(db);
		// SqlErr_interface();
		// sql_WriteErrData();
		//6
		// if(0)
		if(1 == mid_sqlite.MSqlite_WriAirConData_flg)
		{
			mid_sqlite.MSqlite_WriAirConData_flg = 0;
			for(i=0;i<3;i++)
			{
				if(AirCon_Eachconn_sta[i]==1)
				{
					Sql_AirInter_Deal(i);
					ret = sqlite3_exec( db , Sql_AirConData_buf , 0 , 0 , &zErrMsg );
					if(ret)
					{
//						printf( "insert airCon data err. ret = :%d %s\n",ret,zErrMsg) ;
						syslog(LOG_INFO,"insert airCon datar err. ret = : '%d' %s \n"
						,ret,zErrMsg);
						Sql_Wrt_err_flg = 1;
					}
				}
				
			}
			// if(Sql_DeleMonth_buf[0]==1)
			// {
			// 	Sql_DeleMonth_buf[0] = 0;
			// 	strcpy(Sql_AirConData_buf,"DELETE from 空调数据 where ID > 0; " \
         	// 		  "SELECT * from 空调数据") ;
			// 	ret = sqlite3_exec( db , Sql_AirConData_buf , 0 , 0 , &zErrMsg );

			// 	strcpy(Sql_AirConData_buf,"DELETE from 充电数据 where ID > 0; " \
         	// 		  "SELECT * from 充电数据") ;
			// 	ret = sqlite3_exec( db , Sql_AirConData_buf , 0 , 0 , &zErrMsg );

			// 	strcpy(Sql_AirConData_buf,"DELETE from 电池包数据 where ID > 0; " \
         	// 		  "SELECT * from 电池包数据") ;
			// 	ret = sqlite3_exec( db , Sql_AirConData_buf , 0 , 0 , &zErrMsg );
			// 	printf("12345678901234567890\n");
			// }
		}	
		//7
		for(i = 0;i<20;i++)
		{
			if(1 == mid_sqlite.MSqlite_WriLocatData_flg[i])
			{
				mid_sqlite.MSqlite_WriLocatData_flg[i] = 0;
				Sql_LocatInter_Deal(i);
				ret = sqlite3_exec( db , Sql_LocatorData_buf , 0 , 0 , &zErrMsg );
				if(ret)
				{
//					printf( "insert Locator data err. ret = :%d\n" , ret) ;
					syslog(LOG_INFO,"insert Locator datar err. ret = : '%d' %s \n"
						,ret,zErrMsg);
					Sql_Wrt_err_flg = 1;
				}
			}
		}
		//8
		// if(0)
		if(1 == mid_sqlite.MSql_WriUnfinishedOrder_flg||1 == mid_sqlite.MSql_DelUnfinishedOrder_flg)
		{
			if(mid_sqlite.MSql_WriUnfinishedOrder_flg==1)
			{
				mid_sqlite.MSql_WriUnfinishedOrder_flg = 0;
				Sql_UnfinishedOrder_Deal();
			}
			else if(1 == mid_sqlite.MSql_DelUnfinishedOrder_flg)
			{
				mid_sqlite.MSql_DelUnfinishedOrder_flg = 0;
				Sql_UnfinishedOrder_Del();	
			}
			// uint8_t first_in_flg = 1;
			// try_again:

			ret = sqlite3_exec( db , Sql_UnfinishedOrder_buf , 0 , 0 , &zErrMsg );
			if(ret)
			{
//				printf( "insert UnfinishedOrder_Del data err. ret = :%d %s\n",ret,zErrMsg) ;
				syslog(LOG_INFO,"insert or del UnfinishedOrder err. ret = : '%d' %s \n"
						,ret,zErrMsg);

				Sql_Wrt_err_flg = 1;
				usleep(500*1000);
				ret = sqlite3_exec( db , Sql_UnfinishedOrder_buf , 0 , 0 , &zErrMsg );

				// if(first_in_flg == 1)
				// {
				// 	first_in_flg--;
				// 	goto try_again;
				// }
			}
		}
		//9*****	
		if((mid_sqlite.MCE_WriSqlShutDown_sta > MCE_WriSqlShutDown_sta_last))
		{
			MCE_WriSqlShutDown_sta_last = mid_sqlite.MCE_WriSqlShutDown_sta;
			if(mid_sqlite.MCE_WriSqlShutDown_sta == 2|| mid_sqlite.MCE_WriSqlShutDown_sta == 3)
				{sql_updateRTTable2();}
			else
				{sql_updateRTTable1();}

			ret = sqlite3_exec( db, cmdBuf, 0, 0, &zErrMsg );
			if(ret)
			{
//				printf( "insert batt data err. ret = :%d\n" , ret) ;
				// Sql_Wrt_err_flg = 1;
			}		
		}
		else if(MCE_WriSqlShutDown_sta < MCE_WriSqlShutDown_sta_last)
		{
			MCE_WriSqlShutDown_sta_last = MCE_WriSqlShutDown_sta;
			sql_deleteRTTable();
			ret = sqlite3_exec( db, cmdBuf, 0, 0, &zErrMsg );
			if(ret)
			{
//				printf( "insert batt data err. ret = :%d\n" , ret) ;
				// Sql_Wrt_err_flg = 1;
			}
		}
		//10
		static int cou_10s = 0;
		// printf("cou_10scou_10scou_10s=====%d\n",cou_10s);
		if(cou_10s<50)
			cou_10s++;
		if(cou_10s >=40)
		{
			cou_10s = 0;
			// MSqlite_appointData_flg = 0;
			Sql_appointment_Dele();
			ret = sqlite3_exec( db , Sql_appointment_buf , 0 , 0 , &zErrMsg );
			for(i=0;i<3;i++)
				SqlCloud_BookNum[i]=0;
			for(i = 0;i<20;i++)
			{
				Sql_appointment_Deal(i);
				ret = sqlite3_exec( db , Sql_appointment_buf , 0 , 0 , &zErrMsg );
				if(ret)
				{
//					printf( "insert appointment data err. ret = :%d\n" , ret) ;
				}
			}
		}
		if(MSqlite_WriAcEnergy_flg==1)
		{
			MSqlite_WriAcEnergy_flg = 0;
			if(ACEnCon_conn_flg==1)
			{
				Sql_acEnergy_Deal();
				ret = sqlite3_exec( db , Sql_acEnergyData_buf , 0 , 0 , &zErrMsg );
				if(ret)
				{
//					printf( "insert acEnergyData data err. ret = :%d %s\n", ret,zErrMsg) ;
				}
			}
		}

		sleep(1);
	}
	sqlite3_close_v2(db); //关闭数据库
	sqlite3_shutdown();
	// return 0;
}


void Create_CE_Record_Table()
{
	//创建表,如果表存在，则不创建，并给出提示信息，存储在 error_msg 中
	//表中只有一个自增键 每次插入自动+1
	Sql_CE_Record = "CREATE TABLE 车辆换电记录( \
	ID	INTEGER PRIMARY KEY,\
    换电车辆车牌号 TEXT,\
    车辆到站时间 TEXT,\
    车辆型号 TEXT,\
    电池编号 TEXT,\
    订单号 TEXT,\
    源库位 INTEGER,\
    源库位电池编号 TEXT,\
    源库位电池型号 TEXT,\
    换上电池可用能量 REAL,\
    换上电池可用容量 REAL,\
    换上电池SOC REAL,\
    换下电池可用能量 REAL,\
    换下电池可用容量 REAL,\
    换下电池SOC REAL,\
    目标库位 INTEGER,\
    ODO REAL,\
    ODO是否已发送 INTEGER,\
    ODO是否发送成功 INTEGER,\
    换电信息是否已发送 INTEGER,\
    换电信息是否发送成功 INTEGER,\
    换电完成时间 TEXT,\
    出站请求是否已发送 INTEGER,\
    出站请求是否成功 INTEGER,\
    订单取消是否已发送 INTEGER,\
    订单取消是否成功 INTEGER,\
    软件是否复位 INTEGER\
	);";

}

void Create_BattData_Table()
{
    Sql_Batt_Data = "CREATE TABLE 电池包数据( \
	ID	INTEGER PRIMARY KEY,\
	采样时刻  TEXT, \
	库位号 INTEGER, \
	动力电池健康度 INTEGER, \
	动力电池PACK串联数 INTEGER, \
	动力电池PACK并联数   INTEGER, \
	动力电池温度点总数  INTEGER, \
	动力电池行驶里程  REAL, \
	BMS次软件版本号  INTEGER, \
	BMS主软件版本号  INTEGER, \
	BMS硬件版本号  INTEGER, \
	电池包总成零部件号  REAL, \
	动力电池编码  REAL, \
	动力电池正端继电器状态  INTEGER, \
	动力电池负端继电器状态  INTEGER, \
	动力电池预充继电器状态  INTEGER, \
	动力电池预热继电器状态  INTEGER, \
	动力电池充电继电器状态  INTEGER, \
	动力电池剩余电量SOC  INTEGER, \
	动力电池内部总电压V1   REAL, \
	动力电池内部总电压V2  REAL, \
	动力电池内部总电压V3  REAL, \
	动力电池充放电电流  REAL, \
	动力电池正极对地绝缘电阻  INTEGER, \
	动力电池负极对地绝缘电阻  INTEGER, \
	动力电池可用能量  REAL, \
	动力电池可用容量  REAL, \
	累计充电总安时   REAL, \
	快换电池工作模式  INTEGER, \
	电池包最大允许输出电流  REAL,\
	电池最大允许反馈电流  REAL, \
	当前状态允许最大回馈功率  REAL, \
	当前状态允许最大放电功率  REAL, \
	动力电池冷却风扇当前状态  INTEGER, \
	动力电池加热系统状态  INTEGER, \
	动力电池均衡状态  INTEGER, \
	动力电池系统状态   INTEGER, \
	动力电池充电状态  INTEGER, \
	电池功率限制比例K   REAL, \
	单体电芯最高电压  REAL, \
	最高单体电压编号  INTEGER, \
	单体电芯最低电压  REAL, \
	最低单体电压编号   INTEGER, \
	最高动力蓄电池温度  INTEGER, \
	最高动力蓄电池温度编号   INTEGER, \
	最低动力蓄电池温度  INTEGER, \
	最低动力蓄电池温度编号   INTEGER\
	);";

}

void Create_CharData_Table()
{
	Sql_Char_Data = "CREATE TABLE 充电数据(\
			ID  INTEGER PRIMARY KEY, \
			采样时刻  TEXT, \
			库位号   INTEGER, \
			直流电表电压 REAL, \
			直流电表电流 REAL, \
			直流电表有功功率 REAL, \
			直流电表正向有功 REAL, \
			直流电表反向有功 REAL, \
			电池编号 TEXT, \
			电池类型 TEXT, \
			SOC   INTEGER, \
			正极柱温度  INTEGER, \
			负极柱温度  INTEGER, \
			正在充电  VARCHAR(4), \
			充电电压  REAL, \
			充电电流  REAL, \
			最大功率  REAL, \
			实际功率  REAL, \
			需求电压  REAL, \
			需求电流  REAL, \
			单体电压数量  INTEGER, \
			单体电压1  REAL,单体电压2  REAL,单体电压3  REAL,单体电压4  REAL,单体电压5  REAL,单体电压6  REAL,单体电压7  REAL,单体电压8 REAL,单体电压9  REAL,单体电压10  REAL, \
			单体电压11  REAL,单体电压12  REAL,单体电压13  REAL,单体电压14  REAL,单体电压15  REAL,单体电压16  REAL,单体电压17  REAL,单体电压18 REAL,单体电压19  REAL,单体电压20  REAL, \
			单体电压21  REAL,单体电压22  REAL,单体电压23  REAL,单体电压24  REAL,单体电压25  REAL,单体电压26  REAL,单体电压27  REAL,单体电压28 REAL,单体电压29  REAL,单体电压30  REAL, \
			单体电压31  REAL,单体电压32  REAL,单体电压33  REAL,单体电压34  REAL,单体电压35  REAL,单体电压36  REAL,单体电压37  REAL,单体电压38 REAL,单体电压39  REAL,单体电压40  REAL, \
			单体电压41  REAL,单体电压42  REAL,单体电压43  REAL,单体电压44  REAL,单体电压45  REAL,单体电压46  REAL,单体电压47  REAL,单体电压48 REAL,单体电压49  REAL,单体电压50  REAL, \
			单体电压51  REAL,单体电压52  REAL,单体电压53  REAL,单体电压54  REAL,单体电压55  REAL,单体电压56  REAL,单体电压57  REAL,单体电压58 REAL,单体电压59  REAL,单体电压60  REAL, \
			单体电压61  REAL,单体电压62  REAL,单体电压63  REAL,单体电压64  REAL,单体电压65  REAL,单体电压66  REAL,单体电压67  REAL,单体电压68 REAL,单体电压69  REAL,单体电压70  REAL, \
			单体电压71  REAL,单体电压72  REAL,单体电压73  REAL,单体电压74  REAL,单体电压75  REAL,单体电压76  REAL,单体电压77  REAL,单体电压78 REAL,单体电压79  REAL,单体电压80  REAL, \
			单体电压81  REAL,单体电压82  REAL,单体电压83  REAL,单体电压84  REAL,单体电压85  REAL,单体电压86  REAL,单体电压87  REAL,单体电压88 REAL,单体电压89  REAL,单体电压90  REAL, \
			单体电压91  REAL,单体电压92  REAL,单体电压93  REAL,单体电压94  REAL,单体电压95  REAL,单体电压96  REAL,单体电压97  REAL,单体电压98 REAL,单体电压99  REAL,单体电压100  REAL, \
			单体电压101  REAL,单体电压102  REAL,单体电压103  REAL,单体电压104  REAL,单体电压105  REAL,单体电压106  REAL,单体电压107  REAL,单体电压108 REAL,单体电压109  REAL,单体电压110  REAL, \
			单体电压111  REAL,单体电压112  REAL,单体电压113  REAL,单体电压114  REAL,单体电压115  REAL,单体电压116  REAL,单体电压117  REAL,单体电压118 REAL,单体电压119  REAL,单体电压120  REAL, \
			单体温度数量  INTEGER, \
			单体温度1  INTEGER,单体温度2  INTEGER,单体温度3  INTEGER,单体温度4  INTEGER,单体温度5  INTEGER,单体温度6  INTEGER,单体温度7  INTEGER,单体温度8  INTEGER,\
			单体温度9  INTEGER,单体温度10  INTEGER,单体温度11  INTEGER,单体温度12  INTEGER,单体温度13  INTEGER,单体温度14 INTEGER,单体温度15  INTEGER,单体温度16  INTEGER,\
			单体温度17  INTEGER,单体温度18  INTEGER,单体温度19  INTEGER,单体温度20  INTEGER,单体温度21  INTEGER,单体温度22  INTEGER,单体温度23  INTEGER,单体温度24  INTEGER,\
			单体温度25  INTEGER,单体温度26  INTEGER,单体温度27  INTEGER,单体温度28  INTEGER,单体温度29  INTEGER,单体温度30  INTEGER,单体温度31  INTEGER,单体温度32  INTEGER \
			);";

}

void Create_ConfigData_Table()
{
	Sql_Config_Data = "CREATE TABLE 配置参数(\
	ID  INTEGER PRIMARY KEY, \
	换电站名称  TEXT, \
	电网最大输出功率   INTEGER, \
	充电完成SOC INTEGER, \
	换电允许SOC INTEGER, \
	充电机使能   INTEGER, \
	充电机充电功率  INTEGER \
	);";
}

void Create_ErrData_Table()
{
	Sql_Err_Data = "CREATE TABLE 故障记录(\
		ID  INTEGER PRIMARY KEY, \
		故障设备  TEXT, \
		设备序号   TEXT, \
		故障级别 TEXT, \
		故障代号 INTEGER, \
		故障内容   TEXT, \
		故障发生时间  TEXT \
		);";
}

void Create_AirConData_Table()
{
	Sql_AirCon_Data = "CREATE TABLE 空调数据(\
		ID  INTEGER PRIMARY KEY, \
		采样时刻  TEXT, \
		空调设备号   INTEGER, \
		温度 INTEGER, \
		湿度 INTEGER \
		);";
}

void Create_LocatorData_Table()
{
	Sql_Locator_Data =  "CREATE TABLE 库位信息(\
		ID  INTEGER PRIMARY KEY, \
		库位状态 TEXT, \
		库位使能 INTEGER, \
		电池编号 TEXT, \
		电池类型 TEXT, \
		入库时间 TEXT, \
		充电完成 TEXT, \
		充电完成时间 TEXT \
		);";
}

//*****************************************************

void Sql_CEDataInter_Deal()
{
    sql_ChageBattRecord_t CB_Record;
	strcpy(CB_Record.Plate_numbers,SourseStorage_Grab_carNo);//换电车辆车牌号
    strcpy(CB_Record.Car_ArrivalTime,CE_StartTime);//车辆到站时间
    strcpy(CB_Record.Car_Model,SourseStorage_carType);//车辆型号
    strcpy(CB_Record.Batt_NUM,SourseStorage_OutBatt_NUM);//电池编号
    strcpy(CB_Record.Order_Number,SourseStorage_orderId);//订单号
    CB_Record.SourseStorage = MChar_SelfCe_sour+1;//源库位
    strcpy(CB_Record.SourseStorage_Batt_NUM,SourseStorage_Batt_NUM);//源库位电池编号
	strcpy(CB_Record.SourseStorage_Car_Model,SourseStorage_carType);//源
    CB_Record.ReplaceBatt_Available_Energy = ReplaceBatt_Available_Energy;//换上电池可用能量
    CB_Record.ReplaceBatt_Available_Cap = ReplaceBatt_Available_Cap;//换上电池可用容量
	CB_Record.ReplaceBatt_SOC = ReplaceBatt_SOC;//换上SOC

    CB_Record.RBCar_Batt_Available_Energy = RBCar_Batt_Available_Energy;//换下电池可用能量
    CB_Record.RBCar_Batt_Available_Cap = RBCar_Batt_Available_Cap;//换下电池可用容量
    CB_Record.RBCar_Batt_SOC = RBCar_Batt_Soc;//换下电池SOC
    CB_Record.Target_Location = CB_Record.SourseStorage;//目标库位

    CB_Record.Batt_ODO = 65535;//ODO
    CB_Record.If_ODO_HasSend_flg = 1;//ODO是否已发送
    CB_Record.If_ODO_SendOk_flg = 1; 
    CB_Record.If_RBInf_HasSend_flg = 1;//换电信息是否已发送
    CB_Record.If_RBInf_SendOk_flg = 1;//换电信息发送成功   
    strcpy(CB_Record.RB_CompleteTime,NowTime); 
    CB_Record.If_CarExitReq_HasSend_flg = 1;//出站请求是否已发送
    CB_Record.If_CarExitReq_SendOk_flg = 1;//  
    CB_Record.If_OrderCancelReq_HasSend_flg = 1;//订单取消是否已发送
    CB_Record.If_OrderCancelReq_SendOk_flg = 1;//  
    CB_Record.If_SoftWare_HasReset_flg = 1;//软件是否复位

	sprintf(Sql_CE_Record_buf,"INSERT INTO 车辆换电记录\
	VALUES (Null,\"%s\",\"%s\",\"%s\",\"%s\",\
	\"%s\",%d,\"%s\",\"%s\",\
	%.2f,%.2f,%.2f,\
	%.2f,%.2f,%.2f,\
	%d,%.2f,\
	%d,%d,\
	%d,%d,\
	\"%s\",\
	%d,%d,\
	%d,%d,\
	%d\
	);",CB_Record.Plate_numbers,CB_Record.Car_ArrivalTime,CB_Record.Car_Model,CB_Record.Batt_NUM,\
	CB_Record.Order_Number,CB_Record.SourseStorage,CB_Record.SourseStorage_Batt_NUM,CB_Record.SourseStorage_Car_Model,\
	CB_Record.ReplaceBatt_Available_Energy,CB_Record.ReplaceBatt_Available_Cap,CB_Record.ReplaceBatt_SOC,\
	CB_Record.RBCar_Batt_Available_Energy,CB_Record.RBCar_Batt_Available_Cap,CB_Record.RBCar_Batt_SOC,\
	CB_Record.Target_Location,CB_Record.Batt_ODO,\
	CB_Record.If_ODO_HasSend_flg,CB_Record.If_ODO_SendOk_flg,\
	CB_Record.If_RBInf_HasSend_flg,CB_Record.If_RBInf_SendOk_flg,\
	CB_Record.RB_CompleteTime,\
	CB_Record.If_CarExitReq_HasSend_flg,CB_Record.If_CarExitReq_SendOk_flg,\
	CB_Record.If_OrderCancelReq_HasSend_flg,CB_Record.If_OrderCancelReq_SendOk_flg,\
	CB_Record.If_SoftWare_HasReset_flg
	);

}

void Sql_BattDataInter_Deal(uint8_t LocatorNum)
{
	sql_BattData_t BattData;

	strcpy(BattData.Time_now,NowTime);
	BattData.StorageNum = (LocatorNum+1);//库位号
	BattData.Batt_SOH = Batt_SOH[LocatorNum];//	动力电池健康度 INTEGER
	BattData.BMS_ParallNum_Null = BMS_ParallNum_Null[LocatorNum];  //BMS传出电池并联数
	BattData.BMS_SerialNum_Null = BMS_SerialNum_Null[LocatorNum];  //BMS传出电池串联数
	
	BattData.BATT_TEMP_SAMPLE_NUM = BATT_TEMP_SAMPLE_NUM[LocatorNum];//温度采集点个数
	BattData.BMS_ODO = BMS_ODO[LocatorNum];//动力电池行驶里程

	BattData.BMS_SW_VERS_main = BMS_SW_VERS_main[LocatorNum];	//软件ZHU版本
	BattData.BMS_SW_VERS = BMS_SW_VERS[LocatorNum];	//软件CI版本
	BattData.BMS_HW_VERS = BMS_HW_VERS[LocatorNum];		
	BattData.BMS_PART_VERS = BMS_PART_VERS[LocatorNum];//BMS零件号
	strcpy(BattData.BMS_BATT_NUM,BMS_BATT_NUM[LocatorNum]);//动力电池编码

	BattData.BMS_POS_RLY_STA = ((BMS_SYS_STA[LocatorNum]&0x01)==1);//正极继电器状态
	BattData.BMS_NEG_RLY_STA = ((BMS_SYS_STA[LocatorNum]&0x02)==2);//负极继电器状态
	BattData.BMS_PRE_CHG_RLY_STA = ((BMS_SYS_STA[LocatorNum]&0x04)==4);//预充继电器当前状态
	BattData.BMS_BATT_HEAT_RLY_STA = 0;//加热继电器当前状态	//650加热不对
	BattData.BMS_CHG_RLY_STA = 0;//充电继电器当前状态	//EU300无该信号

	BattData.BMS_BATT_SOC = BMS_BATT_SOC[LocatorNum];    //动力电池剩余电量SOC
	BattData.BMS_BATT_VOLT_V1 = BMS_BATT_VOLT_V1[LocatorNum]; //动力电池内部总电压V1
	BattData.BMS_BATT_VOLT_V2 = BMS_BATT_VOLT_V2[LocatorNum];  //动力电池外部总电压V2
	BattData.BMS_BATT_VOLT_V3  = BMS_BATT_VOLT_V3[LocatorNum];  //动力电池负载端总电压V3
	BattData.BMS_BATT_CUR = BMS_BATT_CUR[LocatorNum];     //动力电池充/放电电流

	BattData.BMS_BATT_POSITIVE_RESISTANCE = BMS_BATT_POSITIVE_RESISTANCE[LocatorNum]; 	//动力电池正极对GND绝缘电阻
	BattData.BMS_BATT_NEGATIVE_RESISTANCE = BMS_BATT_NEGATIVE_RESISTANCE[LocatorNum]; 	//动力电池负极对GND绝缘电阻
	BattData.BMS_BATT_AVAIL_ENERGY = BMS_BATT_AVAIL_ENERGY[LocatorNum];       	//动力电池可用能量
	BattData.BMS_BATT_AVAIL_CAPACITY = BMS_BATT_AVAIL_CAPACITY[LocatorNum];      	//动力电池可用容量
	BattData.BMS_CHARGE_TOT_AH = BMS_CHARGE_TOT_AH[LocatorNum];       		//累计充电总安时

	BattData.BATT_WORK_MODE = BATT_WORK_MODE[LocatorNum];//快换电池工作模式	EU300无该信号
									//0X00行车模式
									//0X01换电站监控
									//0X02换电站充电模式
									//0X03储能模式

	BattData.BATT_ALLW_OUTPUT_CUR = BATT_ALLW_OUTPUT_CUR[LocatorNum];	//电池包最大允许输出电流 EU300无该信号
	BattData.BATT_ALLW_FB_CUR = BATT_ALLW_FB_CUR[LocatorNum];		//电池包最大允许回馈电流 EU300无该信号
	BattData.BMS_FEED_MAX_PWR = BMS_FEED_MAX_PWR[LocatorNum];             	//当前状态允许最大回馈功率
	BattData.BMS_DISCHG_MAX_PWR = BMS_DISCHG_MAX_PWR[LocatorNum];				//当前状态最大允许放电功率

	BattData.BMS_COOLING_FAN_STA = BMS_COOLING_FAN_STA[LocatorNum];	//动力电池冷却风扇当前状态
	BattData.BMS_BATT_HEAT_STA = BMS_BATT_HEAT_STA[LocatorNum];		//动力电池加热状态
	BattData.BMS_BAT_BALANCE_STA = BMS_BAT_BALANCE_STA[LocatorNum];	//动力电池均衡状态

	BattData.BMS_SYS_STA = BMS_SYS_STA[LocatorNum];			//动力电池系统状态	EU300无该信号
	BattData.BATT_Chg_Sta = BATT_Chg_Sta[LocatorNum];		//动力电池充电状态	EU300无该信号
	BattData.BATT_PWR_LIMIT_RATIO_K = BATT_PWR_LIMIT_RATIO_K[LocatorNum];	//电池功率限制比例K	EU3	
	BattData.BMS_CELL_VOLT_MAX = BMS_CELL_VOLT_MAX[LocatorNum];//单体最高电压
	BattData.BMS_CELL_VOLT_MIN = BMS_CELL_VOLT_MIN[LocatorNum];//单体最低电压
	BattData.BMS_CELL_TEMP_MAX = BMS_CELL_TEMP_MAX[LocatorNum];//单体最高温度
	BattData.BMS_CELL_TEMP_MIN = BMS_CELL_TEMP_MIN[LocatorNum];//单体最低温度

	BattData.BMS_MAX_VOLT_NUM = BMS_MAX_VOLT_NUM[LocatorNum];//最高单体电压编号
	BattData.BMS_MAX_TEMP_NUM = BMS_MAX_TEMP_NUM[LocatorNum];//最高温度编号
	BattData.BMS_MIN_TEMP_NUM = BMS_MIN_TEMP_NUM[LocatorNum];//最低温度编号
	BattData.BMS_MIN_VOLT_NUM = BMS_MIN_VOLT_NUM[LocatorNum];//最低单体电压编号

	sprintf(Sql_BattData_buf,"INSERT INTO 电池包数据\
		VALUES (Null,\"%s\",%d,%d,%d,%d,\
		%d,%.2f,%.2f,%.2f,%.2f,\
		%.2f,\"%s\",\
		%d,%d,%d,%d,%d,\
		%.2f,%.2f,%.2f,%.2f,%.2f,\
		%.2f,%.2f,\
		%.2f,%.2f,%.2f,\
		%d,%.2f,%.2f,%.2f,%.2f,\
		%d,%d,%d,\
		%d,%d,%d,\
		%.4f,%d,%.4f,%d,\
		%.2f,%d,%.2f,%d\
		);",BattData.Time_now,BattData.StorageNum,BattData.Batt_SOH,BattData.BMS_SerialNum_Null,BattData.BMS_ParallNum_Null,\
		BattData.BATT_TEMP_SAMPLE_NUM,BattData.BMS_ODO,BattData.BMS_SW_VERS,BattData.BMS_SW_VERS_main,BattData.BMS_HW_VERS,\
		BattData.BMS_PART_VERS,BattData.BMS_BATT_NUM,\
		BattData.BMS_POS_RLY_STA,BattData.BMS_NEG_RLY_STA,BattData.BMS_PRE_CHG_RLY_STA,BattData.BMS_BATT_HEAT_RLY_STA,BattData.BMS_CHG_RLY_STA,\
		BattData.BMS_BATT_SOC,BattData.BMS_BATT_VOLT_V1,BattData.BMS_BATT_VOLT_V2,BattData.BMS_BATT_VOLT_V3,BattData.BMS_BATT_CUR,\
		BattData.BMS_BATT_POSITIVE_RESISTANCE,BattData.BMS_BATT_NEGATIVE_RESISTANCE,\
		BattData.BMS_BATT_AVAIL_ENERGY,BattData.BMS_BATT_AVAIL_CAPACITY,BattData.BMS_CHARGE_TOT_AH,\
		BattData.BATT_WORK_MODE,BattData.BATT_ALLW_OUTPUT_CUR,BattData.BATT_ALLW_FB_CUR,BattData.BMS_FEED_MAX_PWR,BattData.BMS_DISCHG_MAX_PWR,\
		BattData.BMS_COOLING_FAN_STA,BattData.BMS_BATT_HEAT_STA,BattData.BMS_BAT_BALANCE_STA,\
		BattData.BMS_SYS_STA,BattData.BATT_Chg_Sta,BattData.BATT_PWR_LIMIT_RATIO_K,\
		BattData.BMS_CELL_VOLT_MAX,BattData.BMS_MAX_VOLT_NUM,BattData.BMS_CELL_VOLT_MIN,BattData.BMS_MIN_VOLT_NUM,\
		BattData.BMS_CELL_TEMP_MAX,BattData.BMS_MAX_TEMP_NUM,BattData.BMS_CELL_TEMP_MIN,BattData.BMS_MIN_TEMP_NUM);
}

uint8_t Cloud_SendBookCE_flg=5,Book_Init_flg = 0;;
int starthour=0,endhour=0;
uint8_t starttime[50],endtime[50],starttime1[50],endtime1[50];
uint8_t Init_starttime[3][50]={0},Init_endtime[3][50]={0};
struct tm *p;
void Time_deal(void)
{

}
void GetMacTime()
{
	// static uint8_t Book_Start_flg = 0;
	static long int StartTime_s=0,Start=0,end=0;
    time_t timep;
    // struct tm *p;
	struct tm *s_0,*s_1,*s_2,*e_0,*e_1,*e_2;
	struct tm *s,*e;
    time (&timep);	
	int starthour_loc=0,endhour_loc=0;
	p = localtime(&timep);
	strftime(NowTime,50,"%G-%m-%d %H:%M:%S",p);
	// if(Book_Init_flg==0)
	// {
	// 	if(p->tm_min<29||(p->tm_min==29&&p->tm_sec<=29)){
	// 		Cloud_SendBookCE_flg=6;
	// 	}
	// 	else{
	// 		Cloud_SendBookCE_flg=5;
	// 	}
		
	// 	StartTime_s = timep;
	// 	s_0 = localtime(&StartTime_s);
	// 	sprintf(Init_starttime[0],"%d-%.2d-%.2d%s%d:%.2d:%.2d",s_0->tm_year+1900,s_0->tm_mon+1,s_0->tm_mday,"%20",s_0->tm_hour,0,0);
	// 	StartTime_s = (timep+3600*1);
	// 	e_0 = localtime(&StartTime_s);
	// 	sprintf(Init_endtime[0],"%d-%.2d-%.2d%s%d:%.2d:%.2d",e_0->tm_year+1900,e_0->tm_mon+1,e_0->tm_mday,"%20",e_0->tm_hour,0,0);
		
	// 	strcpy(Init_starttime[1],Init_endtime[0]);
	// 	StartTime_s = (timep+3600*2);
	// 	e_1 = localtime(&StartTime_s);
	// 	sprintf(Init_endtime[1],"%d-%.2d-%.2d%s%d:%.2d:%.2d",e_1->tm_year+1900,e_1->tm_mon+1,e_1->tm_mday,"%20",e_1->tm_hour,0,0);
		
	// 	strcpy(Init_starttime[2],Init_endtime[1]);
	// 	StartTime_s = (timep+3600*3);
	// 	e_2 = localtime(&StartTime_s);
	// 	sprintf(Init_endtime[2],"%d-%.2d-%.2d%s%d:%.2d:%.2d",e_2->tm_year+1900,e_2->tm_mon+1,e_2->tm_mday,"%20",e_2->tm_hour,0,0);

	// 	// printf("timetimetime111 %s %s %s %s %s %s\n",Init_starttime[0],Init_endtime[0],Init_starttime[1],Init_endtime[1],Init_starttime[2],Init_endtime[2]);
	// }
	// else if(Book_Init_flg==1)
	// {
	// 	if((p->tm_min==30)&&(p->tm_sec==0))
	// 	{
	// 		Cloud_SendBookCE_flg = 1; 
	// 		StartTime_s = (timep+3600*2);
	// 		s_0 = localtime(&StartTime_s);
	// 		sprintf(starttime,"%d-%.2d-%.2d%s%d:%.2d:%.2d",s_0->tm_year+1900,s_0->tm_mon+1,s_0->tm_mday,"%20",s_0->tm_hour,0,0);
	// 		StartTime_s = (timep+3600*3);
	// 		s_0 = localtime(&StartTime_s);
	// 		sprintf(endtime,"%d-%.2d-%.2d%s%d:%.2d:%.2d",s_0->tm_year+1900,s_0->tm_mon+1,s_0->tm_mday,"%20",s_0->tm_hour,0,0);
	// 		// printf("5min5min %s %s\n",starttime,endtime);
	// 	}
	// 	else if (p->tm_min>=0&&(p->tm_sec==0))
	// 	{
	// 		if(Cloud_SendBookCE_flg==0)
	// 			Cloud_SendBookCE_flg = 2; 

	// 		StartTime_s = (timep+3600*1);
	// 		s_0 = localtime(&StartTime_s);
	// 		sprintf(starttime,"%d-%.2d-%.2d%s%d:%.2d:%.2d",s_0->tm_year+1900,s_0->tm_mon+1,s_0->tm_mday,"%20",s_0->tm_hour,0,0);
	// 		StartTime_s = (timep+3600*2);
	// 		s_0 = localtime(&StartTime_s);
	// 		sprintf(endtime,"%d-%.2d-%.2d%s%d:%.2d:%.2d",s_0->tm_year+1900,s_0->tm_mon+1,s_0->tm_mday,"%20",s_0->tm_hour,0,0);
			
	// 		strcpy(starttime1,endtime);
	// 		StartTime_s = (timep+3600*3);
	// 		s_0 = localtime(&StartTime_s);
	// 		sprintf(endtime1,"%d-%.2d-%.2d%s%d:%.2d:%.2d",s_0->tm_year+1900,s_0->tm_mon+1,s_0->tm_mday,"%20",s_0->tm_hour,0,0);
	// 		// printf("1min1min %s %s %s %s\n",starttime,endtime,starttime1,endtime1);
	// 	}
	// }	

	// //if((p->tm_mday==1)&&(p->tm_hour==9)&&(p->tm_min==0)&&(p->tm_sec==0))
	// if (p->tm_min>=0&&(p->tm_sec==30))
	// {
	// 	// system("cp SA04_station.db /home/tcc/Desktop/test/SA04_station_1.db");
	// 	char cmdBufTmp[200] = "";
	// 	char test_time[200] = "";

	// 	strcpy(cmdBufTmp,"cp ");
	// 	strcat(cmdBufTmp,"SA04_station.db ");
	// 	strcat(cmdBufTmp,"/home/tcc/Desktop/db/SA04_station_");    
	// 	sprintf(test_time,"%d_%d_%d",p->tm_year+1900,p->tm_mon+1,p->tm_mday);   
	// 	strcat(cmdBufTmp,test_time);
	// 	system(cmdBufTmp);
	// 	printf("%s\n",cmdBufTmp);		
	// 	printf("cp SA04_station");

	// 	Sql_DeleMonth_buf[0] = 1;
	// }	转到376行
}

/*充电数据接口函数*//*ok*/
void Sql_CharInter_Deal(uint8_t locate)
{
	int j;
	sql_CharData_t CharData;
	uint8_t vol_buf[200];
	uint8_t temp_buf[200];
	uint8_t ss_vol[2000],ss_temp[2000];
	uint8_t Car_Type[10];

	CharData.DCEnVol_V = DCEnVol_V[locate];
	CharData.DCEnCur_A = DCEnCur_A[locate];
	CharData.DCEn_Act_W = DCEn_Act_W[locate];
	CharData.DCEnPos_Act_W = DCEnPos_Act_W[locate];
	CharData.DCEnNeg_Act_W = DCEnNeg_Act_W[locate];

	CharData.Soc = BMS_PackSocReal_pct[locate];
	CharData.PosTemp = CHU_PosTemp_C[locate];
	CharData.NegTemp = CHU_NegTemp_C[locate];
	CharData.CharVol = BMS_CharPackVolReal_V[locate];
	CharData.CharCur = BMS_CharPackCurReal_A[locate];
	CharData.MaxPow = 30*1000;/*最大充电功率*/
	CharData.RealPow = Charger_OutPwr_W[locate];
	CharData.ReqVol = BMS_ReqPackVol_V[locate];
	CharData.ReqCur = BMS_ReqPackCur_A[locate];
	// CharData.CellNum = 112;/*单体数量*/
	CharData.TempNum = 32;/*单体温度数量*/

	__bzero(vol_buf,200);
	__bzero(temp_buf,200);
	__bzero(ss_vol,2000);
	__bzero(ss_temp,2000);

	int Serial = 0,Temp_num = 0;
	if(BMS_SerialNum_Null[locate]>50)
		Serial = BMS_SerialNum_Null[locate];
	else 
		Serial = 112;
	CharData.CellNum = Serial;/*单体数量*/

	for(j=0;j<120;j++)
	{
		if(j<Serial)
		{
			CharData.CellVol[j] = ((double)BMS_CellVol_buf[locate][j]*0.02);
		}
		else
		{
			CharData.CellVol[j] = 0;
		}
		sprintf(vol_buf,"%.2f",CharData.CellVol[j]);
		strcat(ss_vol,vol_buf);
		if(j<(120-1))
		strcat(ss_vol,",");
	}

	if(CHU_BattType[locate]==1)
	{	
		strcpy(Car_Type,"EU300");
		if(BATT_TEMP_SAMPLE_NUM[locate]>10)
			Temp_num = BATT_TEMP_SAMPLE_NUM[locate];
		else 
			Temp_num = 32;
	
	}
	else if(CHU_BattType[locate]==2)
	{	
		strcpy(Car_Type,"EU5A");
		Temp_num = 20;
	
	}
	else if(CHU_BattType[locate]==3)
	{	
		strcpy(Car_Type,"EU5B");
		Temp_num = 24;
	}


	for(j=0;j<32;j++)
	{
		if(j<Temp_num)
		{
			CharData.CellTemp[j] = (BMS_CellTemp_buf[locate][j]-48);
		}
		else
		{
			CharData.CellTemp[j] = 0;
		}

		// CharData.CellTemp[j] = (BMS_CellTemp_buf[locate][j]-48);
		sprintf(temp_buf,"%d",CharData.CellTemp[j]);
		strcat(ss_temp,temp_buf);
		if(j<31)
		strcat(ss_temp,",");
	}


	sprintf(Sql_CharData_buf,"INSERT INTO 充电数据 \
	VALUES (Null,\"%s\",%d,%f,%f,%f,%f,%f,\"%s\",\"%s\",%d,%d,%d,\"%s\",%f,%f,%f,%f,%f,%f,%d,%s,%d,%s);",
	NowTime,(locate+1),CharData.DCEnVol_V,CharData.DCEnCur_A,CharData.DCEn_Act_W,CharData.DCEnPos_Act_W,CharData.DCEnNeg_Act_W,BMS_BATT_NUM[locate],Car_Type,CharData.Soc,CharData.PosTemp,CharData.NegTemp,"no",CharData.CharVol,
	CharData.CharCur,CharData.MaxPow,CharData.RealPow,CharData.ReqVol,CharData.ReqCur,CharData.CellNum,ss_vol,CharData.TempNum,ss_temp);
//	printf("INSERT INTO 充电数据 = %f\n",CharData.CellVol[11]);
}

/*配置信息接口函数*/
void Sql_ConfInter_Deal(void)
{
	sql_Config_t ConfigInf;
	int i=0,tep=0;
	strcpy(ConfigInf.CEStation_Name,"no.11 station");
	ConfigInf.GridMaxOutPow = 100*1000;
	ConfigInf.CharEndSoc = mid_sqlite.MLocat_CharEndSoc_pct;
	ConfigInf.CE_AllowSoc = mid_sqlite.MLocat_AllowCESoc_pct;
	// memcpy(ConfigInf.Charer_ena,MLocat_SelfChar_ena,20);
	for(i=0;i<18;i++){
		tep += (mid_sqlite.MLocat_SelfChar_ena[i]<<i)*1;

	}
	ConfigInf.Charer_ena = tep;//自动充电使能
	ConfigInf.Char_ReqPow = mid_sqlite.MLocat_CharPow_kw;

	sprintf(Sql_ConfigData_buf,"INSERT INTO 配置参数 \
	VALUES (Null,\"%s\",%d,%d,%d,%d,%d);",
	ConfigInf.CEStation_Name,ConfigInf.GridMaxOutPow,ConfigInf.CharEndSoc,
	ConfigInf.CE_AllowSoc,ConfigInf.Charer_ena,ConfigInf.Char_ReqPow);
    
//	printf("CONFIGDATA=%s\n",Sql_ConfigData_buf);
}

/*故障信息写入*//*每次发生故障调用*/
void Sql_ErrInter_Deal(void)
{

}
/*空调信息写入*/
void Sql_AirInter_Deal(uint8_t num)
{
	sql_AirConData_t AirConData;

	AirConData.AirCon_DevNum = (num+1);
	AirConData.AirCon_Temp = mid_sqlite.AirCon_Temp[num];
	AirConData.AirCon_Humi = mid_sqlite.AirCon_Humi[num];
	sprintf(Sql_AirConData_buf,"INSERT INTO 空调数据 \
			VALUES (Null,\"%s\",%d,%d,%d);",
			NowTime,AirConData.AirCon_DevNum,AirConData.AirCon_Temp,AirConData.AirCon_Humi);
}

/*库位信息接口函数*/
void Sql_LocatInter_Deal(uint8_t num)
{
	sql_LocatorData_t LocatorData;
	uint8_t Car_Type[10];
	
	if(CHU_BattType[num]==1)
		strcpy(Car_Type,"EU300");
	else if(CHU_BattType[num]==2)
		strcpy(Car_Type,"EU5A");
	else if(CHU_BattType[num]==3)
		strcpy(Car_Type,"EU5B");

	strcpy(LocatorData.Locator_sta,"正常");
	LocatorData.Locator_ena = (num+1);
	strcpy(LocatorData.Bat_code,mid_sqlite.MLocat_BatteryNo[num]);
	strcpy(LocatorData.Bat_type,Car_Type);
	strcpy(LocatorData.Char_ok,"是");
	sprintf(Sql_LocatorData_buf,"INSERT INTO 库位信息 \
			VALUES (Null,\"%s\",%d,\"%s\",\"%s\",\"%s\",\"%s\",\"%s\");",
			LocatorData.Locator_sta,LocatorData.Locator_ena,LocatorData.Bat_code
			,LocatorData.Bat_type,NowTime,LocatorData.Char_ok,NowTime);
}

/* 故障记录 */
// typedef struct{
// int Err_code;

// char *ErrDev;
// char Err_Lev;

// char *Err_code_str;
// char *ErrName;

// }table_ErrRecord_t;

// table_ErrRecord_t err_table_CBDev[500]={
// {1,"换电设备",3,"SWE01-3-001","急停报警"},
// {2,"换电设备",3,"SWE01-3-002","备用"},
// {3,"换电设备",3,"SWE01-3-003","自检轴使能丢失报警"},
// {4,"换电设备",3,"SWE01-3-004","初始化RGV与停车台交互中报警"},
// {5,"换电设备",3,"SWE01-3-005","初始化码垛机货叉回中间位禁止报警"},
// {6,"换电设备",3,"SWE01-3-006","初始化超时报警"},
// {30,"换电设备",3,"SWE01-3-030","Rx_侧推位置扭矩参数设置异常报警"},
// {31,"换电设备",3,"SWE01-3-031","EMS通讯异常报警"},
// {50,"换电设备",3,"SWE01-3-050","指令触发_当前模式不满足报警"},
// {51,"换电设备",3,"SWE01-3-051","指令触发_换电_停车台状态不满足报警"},
// {52,"换电设备",3,"SWE01-3-052","指令触发_换电_RGV状态不满足报警"},
// {53,"换电设备",3,"SWE01-3-053","指令触发_换电_缓存仓状态不满足报警"},
// {54,"换电设备",3,"SWE01-3-054","指令触发_换电_码垛机状态不满足报警"},
// {55,"换电设备",3,"SWE01-3-055","指令触发_换电_停车台车在位异常报警"},
// {56,"换电设备",3,"SWE01-3-056","指令触发_换电_RGV包状态异常报警"},
// {57,"换电设备",3,"SWE01-3-057","指令触发_换电_Tz包状态异常报警"},
// {58,"换电设备",3,"SWE01-3-058","指令触发_换电_缓存仓包状态异常报警"},
// {59,"换电设备",3,"SWE01-3-059","指令触发_换电_码垛机包状态异常报警"},
// {60,"换电设备",3,"SWE01-3-060","指令触发_换电_存取仓号异常报警"},
// {61,"换电设备",3,"SWE01-3-061","指令触发_换电_车型号异常报警"},
// {62,"换电设备",3,"SWE01-3-062","指令触发_子程序_停车台状态不满足报警"},
// {63,"换电设备",3,"SWE01-3-063","指令触发_子程序_RGV状态不满足报警"},
// {64,"换电设备",3,"SWE01-3-064","指令触发_子程序_缓存仓状态不满足报警"},
// {65,"换电设备",3,"SWE01-3-065","指令触发_子程序_码垛机状态不满足报警"},
// {66,"换电设备",3,"SWE01-3-066","指令触发_存仓号异常报警"},
// {67,"换电设备",3,"SWE01-3-067","指令触发_取仓号异常报警"},
// {68,"换电设备",3,"SWE01-3-068","指令触发_车型号异常报警"},
// {69,"换电设备",3,"SWE01-3-069","指令触发_滑出库状态异常报警"},
// {100,"换电设备",3,"SWE01-3-100","仓门电机静点报警"},
// {101,"换电设备",3,"SWE01-3-101","仓门电机动点报警"},
// {102,"换电设备",3,"SWE01-3-102","前推杆静点报警"},
// {103,"换电设备",3,"SWE01-3-103","前推杆动点报警"},
// {104,"换电设备",3,"SWE01-3-104","中推杆静点报警"},
// {105,"换电设备",3,"SWE01-3-105","中推杆动点报警"},
// {106,"换电设备",3,"SWE01-3-106","后推杆静点报警"},
// {107,"换电设备",3,"SWE01-3-107","后推杆动点报警"},
// {108,"换电设备",3,"SWE01-3-108","停车台有车报警"},
// {109,"换电设备",3,"SWE01-3-109","停车台有车在位异常"},
// {110,"换电设备",3,"SWE01-3-110","停车台无车报警"},
// {111,"换电设备",3,"SWE01-3-111","RGV有包报警"},
// {112,"换电设备",3,"SWE01-3-112","RGV有包在位异常报警"},
// {113,"换电设备",3,"SWE01-3-113","RGV无包报警"},
// {114,"换电设备",3,"SWE01-3-114","Tz有包报警"},#include "errMsg.h"
// {115,"换电设备",3,"SWE01-3-115","Tz有包在位异常报警"},
// {116,"换电设备",3,"SWE01-3-116","Tz无包报警"},
// {117,"换电设备",3,"SWE01-3-117","缓存仓有包报警"},
// {118,"换电设备",3,"SWE01-3-118","缓存仓有包在位异常报警"},
// {119,"换电设备",3,"SWE01-3-119","缓存仓无包报警"},
// {120,"换电设备",3,"SWE01-3-120","码垛机有包报警"},
// {121,"换电设备",3,"SWE01-3-121","码垛机无包报警"},
// {122,"换电设备",3,"SWE01-3-122","车底盘左右在位异常报警"},
// {123,"换电设备",3,"SWE01-3-123","车底盘电池检测异常报警"},
// {124,"换电设备",3,"SWE01-3-124","前推杆未在静点报警"},
// {125,"换电设备",3,"SWE01-3-125","中推杆未在静点报警"},
// {126,"换电设备",3,"SWE01-3-126","后推杆未在静点报警"},
// {127,"换电设备",3,"SWE01-3-127","前推杆未在动点报警"},
// {128,"换电设备",3,"SWE01-3-128","中推杆未在动点报警"},
// {129,"换电设备",3,"SWE01-3-129","后推杆未在动点报警"},
// {130,"换电设备",3,"SWE01-3-130","Rx拆电池异常报警"},
// {131,"换电设备",3,"SWE01-3-131","Rx装电池异常报警"},
// {134,"换电设备",3,"SWE01-3-134","仓N推杆静点报警"},
// {135,"换电设备",3,"SWE01-3-135","仓N推杆动点报警"},
// {136,"换电设备",3,"SWE01-3-136","车前轮定位异常报警"},
// {137,"换电设备",3,"SWE01-3-137","车后轮定位异常报警"},
// {138,"换电设备",3,"SWE01-3-138","仓门未打开报警"},
// {139,"换电设备",3,"SWE01-3-139","仓N推杆未缩回报警"},
// {140,"换电设备",3,"SWE01-3-140","左车门打开报警"},
// {141,"换电设备",3,"SWE01-3-141","右车门打开报警"},
// {200,"换电设备",3,"SWE01-3-200","货叉中间位报警"},
// {201,"换电设备",3,"SWE01-3-201","PFx_A车换型位报警"},
// {202,"换电设备",3,"SWE01-3-202","PFx_Eu300位报警"},
// {203,"换电设备",3,"SWE01-3-203","PFL_松开位报警"},
// {204,"换电设备",3,"SWE01-3-204","PFR_松开位报警"},
// {205,"换电设备",3,"SWE01-3-205","PBL_松开位报警"},
// {206,"换电设备",3,"SWE01-3-206","PBR_松开位报警"},
// {207,"换电设备",3,"SWE01-3-207","PFz_泊车位报警"},
// {208,"换电设备",3,"SWE01-3-208","PBz_泊车位报警"},
// {209,"换电设备",3,"SWE01-3-209","Rx_拆解位报警"},
// {210,"换电设备",3,"SWE01-3-210","RL_中间位报警"},
// {211,"换电设备",3,"SWE01-3-211","RR_中间位报警"},
// {212,"换电设备",3,"SWE01-3-212","Ry_接驳位报警"},
// {213,"换电设备",3,"SWE01-3-213","Ry_拆装位报警"},
// {214,"换电设备",3,"SWE01-3-214","Tz_底层位报警"},
// {215,"换电设备",3,"SWE01-3-215","Tz_缓存上位报警"},
// {216,"换电设备",3,"SWE01-3-216","Tz_码垛机位报警"},
// {217,"换电设备",3,"SWE01-3-217","Sy_中间位报警"},
// {218,"换电设备",3,"SWE01-3-218","Sz_缓存上位报警"},
// {219,"换电设备",3,"SWE01-3-219","Sz_缓存下位报警"},
// {220,"换电设备",3,"SWE01-3-220","Sz_平台上位报警"},
// {221,"换电设备",3,"SWE01-3-221","Sz_平台下位报警"},
// {222,"换电设备",3,"SWE01-3-222","Sz_仓上位报警"},	
// {223,"换电设备",3,"SWE01-3-223","Sz_仓下位报警"},	
// {224,"换电设备",3,"SWE01-3-224","Sx_平台位_Eu300报警"},
// {225,"换电设备",3,"SWE01-3-225","Sx_平台位_Eu5报警"},
// {226,"换电设备",3,"SWE01-3-226","Sx_目标仓位报警"},
// {300,"换电设备",3,"SWE01-3-300","Ry_网络连接异常"},
// {301,"换电设备",3,"SWE01-3-301","Ry_极限传感器报警"},
// {302,"换电设备",3,"SWE01-3-302","Ry_驱动器报警"},
// {305,"换电设备",3,	"SWE01-3-305","Ry_定位超时报警"},
// {306,"换电设备",3,	"SWE01-3-306","Ry_备用07报警"},
// {307,"换电设备",3,	"SWE01-3-307","Ry_备用08报警"},
// {308,"换电设备",3,	"SWE01-3-308","Rx_网络连接异常"},
// {309,"换电设备",3,	"SWE01-3-309","Rx_极限传感器报警"},
// {310,"换电设备",3,	"SWE01-3-310","Rx_驱动器报警"},
// {313,"换电设备",3,	"SWE01-3-313","Rx_定位超时报警"},
// {314,"换电设备",3,	"SWE01-3-314","Rx_备用07报警"},
// {315,"换电设备",3,	"SWE01-3-315","Rx_备用08报警"},
// {316,"换电设备",3,	"SWE01-3-316","RL_网络连接异常"},
// {317,"换电设备",3,	"SWE01-3-317","RL_极限传感器报警"},
// {318,"换电设备",3,	"SWE01-3-318","RL_驱动器报警"},
// {321,"换电设备",3,	"SWE01-3-321",	"RL_定位超时报警"},
// {322,"换电设备",3,	"SWE01-3-322",	"RL_备用07报警"},
// {323,"换电设备",3,	"SWE01-3-323",	"RL_备用08报警"},
// {324,"换电设备",3,	"SWE01-3-324",	"RR_网络连接异常"},
// {325,"换电设备",3,	"SWE01-3-325",	"RR_极限传感器报警"},
// {326,"换电设备",3,	"SWE01-3-326",	"RR_驱动器报警"	},
// {329,"换电设备",3,	"SWE01-3-329",	"RR_定位超时报警"},
// {330,"换电设备",3,	"SWE01-3-330",	"RR_备用07报警"},
// {331,"换电设备",3,	"SWE01-3-331",	"RR_备用08报警"},
// {332,"换电设备",3,	"SWE01-3-332",	"Tz_网络连接异常"},
// {333,"换电设备",3,	"SWE01-3-333",	"Tz_极限传感器报警"},
// {334,"换电设备",3,	"SWE01-3-334",	"Tz_驱动器报警"},		
// {337,"换电设备",3,	"SWE01-3-337",	"Tz_定位超时报警"},
// {338,"换电设备",3,	"SWE01-3-338",	"Tz_备用07报警"},
// {339,"换电设备",3,	"SWE01-3-339",	"Tz_备用08报警"},
// {340,"换电设备",3,	"SWE01-3-340",	"PFx_网络连接异常"},
// {341,"换电设备",3,	"SWE01-3-341",	"PFx_极限传感器报警"},
// {342,"换电设备",3,	"SWE01-3-342",	"PFx_驱动器报警"	},
// {345,"换电设备",3,	"SWE01-3-345",	"PFx_定位超时报警"},
// {346,"换电设备",3,	"SWE01-3-346",	"PFx_备用07报警"},
// {347,"换电设备",3,	"SWE01-3-347",	"PFx_备用08报警"},
// {348,"换电设备",3,	"SWE01-3-348",	"PFz_网络连接异常"},
// {349,"换电设备",3,	"SWE01-3-349",	"PFz_极限传感器报警"},
// {350,"换电设备",3,	"SWE01-3-350",	"PFz_驱动器报警"},
// {353,"换电设备",3,	"SWE01-3-353",	"PFz_定位超时报警"},
// {354,"换电设备",3,	"SWE01-3-354",	"PFz_备用07报警"},
// {355,"换电设备",3,	"SWE01-3-355",	"PFz_备用08报警"},
// {356,"换电设备",3,	"SWE01-3-356",	"PBz_网络连接异常"},
// {357,"换电设备",3,	"SWE01-3-357",	"PBz_极限传感器报警"},
// {358,"换电设备",3,	"SWE01-3-358",	"PBz_驱动器报警"},		
// {361,"换电设备",3,	"SWE01-3-361",	"PBz_定位超时报警"},
// {362,"换电设备",3,	"SWE01-3-362",	"PBz_备用07报警"},
// {363,"换电设备",3,	"SWE01-3-363",	"PBz_备用08报警"},
// {364,"换电设备",3,	"SWE01-3-364",	"PFL_网络连接异常"},
// {365,"换电设备",3,	"SWE01-3-365",	"PFL_极限传感器报警"},
// {366,"换电设备",3,	"SWE01-3-366",	"PFL_驱动器报警"},
// {369,"换电设备",3,	"SWE01-3-369",	"PFL_定位超时报警"},
// {370,"换电设备",3,"SWE01-3-370","PFL_备用01"},
// {371,"换电设备",3,"SWE01-3-371","PFL_备用02"},
// {372,"换电设备",3,"SWE01-3-372","PFR_网络连接异常"},
// {373,"换电设备",3,"SWE01-3-373","PFR_极限传感器报警"},
// {374,"换电设备",3,"SWE01-3-374","PFR_驱动器报警"},
// {377,"换电设备",3,"SWE01-3-377","PFR_定位超时报警"},
// {378,"换电设备",3,"SWE01-3-378","PFR_备用01"},
// {379,"换电设备",3,"SWE01-3-379","PFR_备用02"},
// {380,"换电设备",3,"SWE01-3-380","PBL_网络连接异常"},
// {381,"换电设备",3,"SWE01-3-381","PBL_极限传感器报警"},
// {382,"换电设备",3,"SWE01-3-382","PBL_驱动器报警"	},	
// {385,"换电设备",3,"SWE01-3-385","PBL_定位超时报警"},
// {386,"换电设备",3,"SWE01-3-386","PBL_备用01"},
// {387,"换电设备",3,"SWE01-3-387","PBL_备用02"},
// {388,"换电设备",3,"SWE01-3-388","PBR_网络连接异常"},
// {389,"换电设备",3,"SWE01-3-389","PBR_极限传感器报警"},
// {390,"换电设备",3,"SWE01-3-390","PBR_驱动器报警"},
// {393,"换电设备",3,"SWE01-3-393","PBR_定位超时报警"},
// {394,"换电设备",3,"SWE01-3-394","PBR_备用01"},
// {395,"换电设备",3,"SWE01-3-395","PBR_备用02"},
// {396,"换电设备",3,"SWE01-3-396","Sx_网络连接异常"},
// {397,"换电设备",3,"SWE01-3-397","Sx_极限传感器报警"},
// {398,"换电设备",3,"SWE01-3-398","Sx_驱动器报警"	},	
// {401,"换电设备",3,"SWE01-3-401","Sx_定位超时报警"},
// {402,"换电设备",3,"SWE01-3-402","Sx_备用07报警"},
// {403,"换电设备",3,"SWE01-3-403","Sx_备用08报警"},
// {404,"换电设备",3,"SWE01-3-404","Sy_网络连接异常"},
// {405,"换电设备",3,"SWE01-3-405","Sy_极限传感器报警"},
// {406,"换电设备",3,"SWE01-3-406","Sy_驱动器报警"},
// {409,"换电设备",3,"SWE01-3-409","Sy_定位超时报警"},
// {410,"换电设备",3,"SWE01-3-410","Sy_备用07报警"},
// {411,"换电设备",3,"SWE01-3-411","Sy_备用08报警"},
// {412,"换电设备",3,"SWE01-3-412","Sz_网络连接异常"},
// {413,"换电设备",3,"SWE01-3-413","Sz_极限传感器报警"},
// {414,"换电设备",3,"SWE01-3-414","Sz_驱动器报警"	},
// {417,"换电设备",3,"SWE01-3-417","Sz_定位超时报警"},
// {418,"换电设备",3,"SWE01-3-418","Sz_备用07报警"},
// {419,"换电设备",3,"SWE01-3-419","Sz_备用08报警"},
// {420,"换电设备",3,"SWE01-3-420","Tz不在底层位报警"},

// };

// table_ErrRecord_t err_table_ChgerDev[100]={

// {81,"充电设备",2,"CHG0N-2-081","BMS心跳超时"},
// {80,"充电设备",2,"CHG0N-2-080","AC/DC模块通信故障"},

// {3,"充电设备",2,"CHG0N-2-003","ACDC输入过压告警"},
// {4,"充电设备",2,"CHG0N-2-004","ACDC输入欠压告警"},
// {5,"充电设备",2,"CHG0N-2-005","ACDC三相输入不平衡告警"},
// {6,"充电设备",2,"CHG0N-2-006","ACDC三相输入缺相告警"},
// {7,"充电设备",2,"CHG0N-2-007","ACDC模块严重不均流"},
// {8,"充电设备",2,"CHG0N-2-008","ACDC模块通信中断"},
// {9,"充电设备",2,"CHG0N-2-009","ACDC输出过压告警"},
// {10,"充电设备",2,"CHG0N-2-010","ACDC过温告警"},
// {11,"充电设备",2,"CHG0N-2-011","ACDC风扇故障告警"},
// {12,"充电设备",2,"CHG0N-2-012","ACDC模块保护告警"},
// {13,"充电设备",2,"CHG0N-2-013","ACDC模块故障告警"},
// {14,"充电设备",2,"CHG0N-2-014","ACDC模块放电异常"},
// {15,"充电设备",2,"CHG0N-2-015","CHU与EMS通信故障"},

// {20,"充电设备",2,"CHG0N-2-020","充电机故障中止"},
// {21,"充电设备",2,"CHG0N-2-021","BMS主动中止"},
// {43,"充电设备",2,"CHG0N-2-043","BMS接收CRM报文超时"},
// {44,"充电设备",2,"CHG0N-2-044","BMS接收CST报文超时"},
// {45,"充电设备",2,"CHG0N-2-045","BMS接收CTS报文超时"},
// {46,"充电设备",2,"CHG0N-2-046","BMS接收CML报文超时"},
// {47,"充电设备",2,"CHG0N-2-047","BMS接收CRO报文超时"},
// {48,"充电设备",2,"CHG0N-2-048","BMS接收CCS报文超时"},
// {49,"充电设备",2,"CHG0N-2-049","BMS接收CSD报文超时"},
// {50,"充电设备",2,"CHG0N-2-050","CHU接收BRM报文超时"},
// {51,"充电设备",2,"CHG0N-2-051","CHU接收BCP报文超时"},
// {52,"充电设备",2,"CHG0N-2-052","CHU接收BRO报文超时"},
// {53,"充电设备",2,"CHG0N-2-053","CHU接收BCS报文超时"},
// {54,"充电设备",2,"CHG0N-2-054","CHU接收BCL报文超时"},
// {55,"充电设备",2,"CHG0N-2-055","CHU接收BST报文超时"},
// {56,"充电设备",2,"CHG0N-2-056","CHU接收BSD报文超时"},

// {67,"充电设备",2,"CHG0N-2-067","BMS绝缘故障"},
// {68,"充电设备",2,"CHG0N-2-068","BMS输出连接器过温故障"},
// {69,"充电设备",2,"CHG0N-2-069","BMS元件过温故障"},
// {70,"充电设备",2,"CHG0N-2-070","BMS充电连接器故障"},
// {71,"充电设备",2,"CHG0N-2-071","BMS电池组温度过高故障"},
// {72,"充电设备",2,"CHG0N-2-072","BMS高压继电器故障"},
// {73,"充电设备",2,"CHG0N-2-073","BMS其他故障"},
// {74,"充电设备",2,"CHG0N-2-074","BMS电流过大"},
// {75,"充电设备",2,"CHG0N-2-075","BMS电压异常"},
// {76,"充电设备",2,"CHG0N-2-076","CHU检测电池单体过压"},
// {77,"充电设备",2,"CHG0N-2-077","CHU检测电池总电压过压"},
// {78,"充电设备",2,"CHG0N-2-078","CHU检测电池温度过高"},
// {79,"充电设备",2,"CHG0N-2-079","CHU检测插拔机构温度过高"},

// };

// table_ErrRecord_t err_table_FireCntrl[2]={

// {1,"消防设备",1,"FFS01-1-001","消防设备硬件故障"},
// {2,"消防设备",4,"FFS01-4-002","消防火警"},
// };

// table_ErrRecord_t err_table_AirCon[20]={

// {2	,"空调设备",1,"ACS0N-1-002","柜内温度传感器故障"},
// {3	,"空调设备",1,"ACS0N-1-003","冷凝盘管温度传感器故障"},
// {4	,"空调设备",1,"ACS0N-1-004","柜外温度传感器故障"},
// {5	,"空调设备",1,"ACS0N-1-005","柜内湿度传感器故障"},
// {6	,"空调设备",1,"ACS0N-1-006","压缩机欠流故障"},
// {7	,"空调设备",1,"ACS0N-1-007","压缩机过载故障"},
// {8	,"空调设备",1,"ACS0N-1-008","加热器欠流故障"},
// {9	,"空调设备",1,"ACS0N-1-009","加热器过载故障"},
// {10,"空调设备",1,"ACS0N-1-010","内风机告警"},
// {11,"空调设备",1,"ACS0N-1-011","外风机告警"},
// {12,"空调设备",1,"ACS0N-1-012","柜内高温告警"},
// {13,"空调设备",1,"ACS0N-1-013","柜内低温告警"},
// {14,"空调设备",1,"ACS0N-1-014","电源电压超限告警"},
// {15,"空调设备",1,"ACS0N-1-015","外部故障告警"},

// };

// /*自检测故障*/
// table_ErrRecord_t err_table_SelfChkErr[20]={
// /*通信故障*/
// {0,"云端通信",3,"CPF01-3-000","EMS与云端通信故障"},
// {0,"换电设备",3,"SWE01-3-000","EMS与换电设备通信故障"},
// {0,"充电设备",2,"CHG0N-2-000","EMS与充电设备通信故障"},
// {0,"空调设备",1,"ACS0N-1-000","EMS与空调设备通信故障"},

// {0,"消防设备",1,"FFS01-1-000","EMS与消防设备通信故障"},
// {0,"直流电表",1,"DEM0N-1-000","EMS与直流电表通信故障"},
// {0,"交流电表",1,"AEM01-1-000","EMS与交流电表通信故障"},
// /*电池包热失控故障*/
// {0,"电池包",4,"BAT0N-4-000","电池包热失控故障"},
// /*门禁开启*/
// {1,"门禁",3,"EGD01-3-001","巡检门开启"},
// {1,"门禁",3,"EGD02-3-001","电池仓门开启"},
// {1,"门禁",3,"EGD03-3-001","维护门开启"},
// {1,"门禁",3,"OGD01-3-001","入口光栅触发"},
// {1,"门禁",3,"OGD02-3-001","出口光栅触发"},

// /*雷击故障*/
// {1,"雷击",3,"LSA01-3-001","充电柜雷击故障"},
// {1,"雷击",3,"LSA02-3-001","配电柜雷击故障"},
// /*水浸故障*/
// {1,"水浸",3,"WLG01-3-001","水浸故障"},
// /*充电系统故障*/
// {0,"充电系统",3,"CHG00-3-000","所有充电仓位均有2级故障"},

// };

// table_ErrRecord_t err_table_db[10]={
// /*交流电表*/
// {1,"交流电表",1,"AEM01-1-001","A相过压"},
// {2,"交流电表",1,"AEM01-1-002","B相过压"},
// {3,"交流电表",1,"AEM01-1-003","C相过压"},
// {4,"交流电表",1,"AEM01-1-004","A相失压"},
// {5,"交流电表",1,"AEM01-1-005","B相失压"},
// {6,"交流电表",1,"AEM01-1-006","C相失压"},
// {7,"交流电表",1,"AEM01-1-007","A相逆向"},
// {8,"交流电表",1,"AEM01-1-008","B相逆向"},
// {9,"交流电表",1,"AEM01-1-009","C相逆向"},
// /*直流电表*/
// {10,"直流电表",1,"NULL","NULL"},
// };

// /*换电设备*/
// int CE_Err_Code = 0;
// int CE_Err_Code_last = 0;
// /*充电设备*/
// uint8_t ChgerDev_Err_Sta[20] = {0};//当前故障状态
// uint8_t ChgerDev_Err_LastSta[20] = {0};//上传故障状态

// /*空调故障*/
// uint8_t AirCon_Err_Sta[3][14] = {0};//当前故障状态
// uint8_t AirCon_Err_LastSta[3][14] = {0};//上传故障状态

// extern uint8_t AirCon_Err1[3];
// extern uint8_t AirCon_Err2[3];
// extern uint8_t AirCon_Err3[3];

// /*自检测故障 故障类型*/
// uint8_t SelfChkErr_EMS_Cloud_ComErr= {0};
// uint8_t SelfChkErr_EMS_Cloud_ComErr_last= {0};
// uint8_t SelfChkErr_EMS_CeDev_ComErr= {0};
// uint8_t SelfChkErr_EMS_CeDev_ComErr_last= {0};
// uint8_t SelfChkErr_EMS_ChgerDev_ComErr[20]= {0};
// uint8_t SelfChkErr_EMS_ChgerDev_ComErr_last[20]= {0};
// uint8_t SelfChkErr_EMS_AirCon_ComErr= {0};
// uint8_t SelfChkErr_EMS_AirCon_ComErr_last= {0};
// uint8_t SelfChkErr_EMS_FireCon_ComErr= {0};
// uint8_t SelfChkErr_EMS_FireCon_ComErr_last= {0};

// uint8_t SelfChkErr_BattHeat_Err[20] = {0};//电池包热失控故障
// uint8_t SelfChkErr_BattHeat_Err_last[20]= {0};//电池包热失控故障
// uint8_t SelfChkErr_DoorStop_Err[5] = {0};//门禁开启
// uint8_t SelfChkErr_DoorStop_Err_last[5] = {0};//门禁开启
// uint8_t SelfChkErr_Thunder_Err[2] = {0};//雷击故障
// uint8_t SelfChkErr_Thunder_Err_last[2] = {0};//雷击故障
// uint8_t SelfChkErr_Water_Err= {0};//水浸故障
// uint8_t SelfChkErr_Water_Err_last= {0};//水浸故障
// uint8_t SelfChkErr_ChgerSys_Err= {0};//充电系统故障
// uint8_t SelfChkErr_ChgerSys_Err_last= {0};//充电系统故障

// int GetErrIndex_Ce(int errCode)
// {
// 	int i=0;
// 	int index = -1;
// 	for(i=0;i<500;i++)
// 	{
// 		if(err_table_CBDev[i].Err_code == errCode)
// 		{
// 			index = i;break;
// 		}
// 	}	
// 	return index;
// }

// int GetErrIndex_Chger(int errCode)
// {
// 	int i=0;
// 	int index = -1;
// 	for(i=0;i<100;i++)
// 	{
// 		if(err_table_ChgerDev[i].Err_code == errCode)
// 		{
// 			index = i;break;
// 		}
// 	}	
// 	return index;
// }

// void SqlErr_interface()
// {
// 	int i=0;
// 	CE_Err_Code = (uint16_t)CE_ErrData[0] *256 + (uint16_t)CE_ErrData[1];
// 	memcpy(ChgerDev_Err_Sta,Get_CHUErrStop_flg,20);

// 	/*空调故障*/
// 	for(i=0;i<3;i++)
// 	{
// 		AirCon_Err_Sta[i][0] = (AirCon_Err1[i] & 0x01)>>0;
// 		AirCon_Err_Sta[i][1] = (AirCon_Err1[i] & 0x02)>>1;
// 		AirCon_Err_Sta[i][2] = (AirCon_Err1[i] & 0x04)>>2;
// 		AirCon_Err_Sta[i][3] = (AirCon_Err1[i] & 0x08)>>3;
// 		AirCon_Err_Sta[i][4] = (AirCon_Err1[i] & 0x10)>>4;
// 		AirCon_Err_Sta[i][5] = (AirCon_Err1[i] & 0x20)>>5;
// 		AirCon_Err_Sta[i][6] = (AirCon_Err1[i] & 0x40)>>6;
// 		AirCon_Err_Sta[i][7] = (AirCon_Err1[i] & 0x80)>>7;
// 		AirCon_Err_Sta[i][8] = (AirCon_Err2[i] & 0x01)>>0;
// 		AirCon_Err_Sta[i][9] = (AirCon_Err2[i] & 0x02)>>1;
// 		AirCon_Err_Sta[i][10] = (AirCon_Err2[i] & 0x10)>>4;
// 		AirCon_Err_Sta[i][11] = (AirCon_Err2[i] & 0x20)>>5;
// 		AirCon_Err_Sta[i][12] = (AirCon_Err3[i] & 0x01)>>0;
// 		AirCon_Err_Sta[i][13] = (AirCon_Err3[i] & 0x02)>>1;
// 	}

// 	SelfChkErr_EMS_Cloud_ComErr = (MCloud_Comm_flg == 2);
// 	SelfChkErr_EMS_CeDev_ComErr = (MCE_Comm_flg == 2);

// 	for(i = 0;i<20;i++)
// 	{
// 		SelfChkErr_EMS_ChgerDev_ComErr[i]= EMS_CHU_ComErr_flg[i];
// 	}

// 	SelfChkErr_EMS_AirCon_ComErr = (AirCon_conn_flg == 2);
// 	SelfChkErr_EMS_FireCon_ComErr = (FireContr_conn_flg == 2);

// 	// SelfChkErr_DoorStop_Err;//门禁开启
// 	// SelfChkErr_Thunder_Err;//雷击故障
// 	// SelfChkErr_Water_Err;//水浸故障
// }

// /*故障信息写入*//*每次发生故障调用*/
// void Sql_ErrInter_Deal2(table_ErrRecord_t errdata_t,int DevIndex)
// {
// 	sql_ErrRecord_t ErrRecord;

// 	strcpy(ErrRecord.ErrDev,errdata_t.ErrDev);/* 故障设备 */

// 	(ErrRecord.ErrDev_count = DevIndex);/* 故障设备序号 */
// 	strcpy(ErrRecord.Err_code , errdata_t.Err_code_str);/* 故障代码*/
// 	ErrRecord.Err_Lev = errdata_t.Err_Lev;/* 故障level*/
// 	strcpy(ErrRecord.ErrName,errdata_t.ErrName);/* 故障miao shu*/

// 	sprintf(Sql_ErrData_buf,"INSERT INTO 故障记录 \
// 		VALUES (Null,\"%s\",%d,%d,\"%s\",\"%s\",\"%s\");",
// 		ErrRecord.ErrDev,ErrRecord.ErrDev_count,ErrRecord.Err_Lev,ErrRecord.Err_code,ErrRecord.ErrName,NowTime);

// 	// printf("Sql_ErrData_buf%s\n",Sql_ErrData_buf);
// }

// void sql_WriteErrData()
// {
// 	// CE_Err_Code = 353;
// 	int i=0,j=0;
// 	/* 换电设备 */
// 	// printf( "SqlErrwrite11 = :%d\n" , 1) ;
// 	if(CE_Err_Code_last !=  CE_Err_Code)
// 	{
// 		if(CE_Err_Code != 0)
// 		{
// 			//write data to sql db
// 			printf( "CE_Err_Code\n") ;
// 			table_ErrRecord_t errdata_t;
// 			int ret=0;
// 			ret = GetErrIndex_Ce(CE_Err_Code);
// 			if(ret>0)
// 			{
// 				// printf( "Err_code_str = :%s\n" , err_table_CBDev[ret].Err_code_str) ;
// 				// strcpy(errdata_t.Err_code_str,err_table_CBDev[3].Err_code_str);
// 				errdata_t.Err_code_str = err_table_CBDev[ret].Err_code_str;
// 				errdata_t.ErrDev = err_table_CBDev[ret].ErrDev;
// 				errdata_t.ErrName = err_table_CBDev[ret].ErrName;

// 				errdata_t.Err_code = err_table_CBDev[ret].Err_code;
// 				errdata_t.Err_Lev = err_table_CBDev[ret].Err_Lev;
// 				Sql_ErrInter_Deal2(errdata_t,1);
// 				ret = sqlite3_exec( db , Sql_ErrData_buf , 0 , 0 , &zErrMsg );
// 				if(ret)
// 				{
// 					printf( "insert err data err. ret = :%d\n" , ret) ;
// 					printf( "err:%s\n",zErrMsg) ;
// 					Sql_Wrt_err_flg = 1;
// 				}
// 			}	
// 		}
// 	}
// 	CE_Err_Code_last = CE_Err_Code;
// 	/* 充电设备 */
// 	for(i=0;i<18;i++)
// 	{
// 		if(ChgerDev_Err_Sta[i] == 1 && ChgerDev_Err_LastSta[i] == 0)
// 		{
// 			//write data to sql db
// 			// printf( "ChgerDev_Err_Sta\n") ;
// 			table_ErrRecord_t errdata_t;
// 			int ret=0;
// 			printf("MChg_ErrStop_code %d\n",MChg_ErrStop_code[i]);
// 			ret = GetErrIndex_Chger(MChg_ErrStop_code[i]);
// 			if(ret>0)
// 			{
// 				(errdata_t.Err_code_str = err_table_ChgerDev[ret].Err_code_str);
// 				(errdata_t.ErrDev = err_table_ChgerDev[ret].ErrDev);
// 				(errdata_t.ErrName = err_table_ChgerDev[ret].ErrName);
// 				errdata_t.Err_code = err_table_ChgerDev[ret].Err_code;
// 				errdata_t.Err_Lev = err_table_ChgerDev[ret].Err_Lev;

// 				Sql_ErrInter_Deal2(errdata_t,i+1);
// 				ret = sqlite3_exec( db , Sql_ErrData_buf , 0 , 0 , &zErrMsg );
// 				if(ret)
// 				{
// 					printf( "insert err data err. ret = :%d\n" , ret) ;
// 					printf( "err:%s\n",zErrMsg) ;
// 					Sql_Wrt_err_flg = 1;
// 				}	
// 			}					
// 		}
// 	}
// 	memcpy(ChgerDev_Err_LastSta,ChgerDev_Err_Sta,20);
// 	/* 空调 */
// 	for(i=0;i<3;i++)
// 	{
// 		for(j=0;j<14;j++)
// 		{
// 			if(AirCon_Err_Sta[i][j] == 1 && AirCon_Err_LastSta[i][j] == 0)
// 			{
// 				int ret=0;
// 				Sql_ErrInter_Deal2(err_table_AirCon[j],i+1);
// 				ret = sqlite3_exec( db , Sql_ErrData_buf , 0 , 0 , &zErrMsg );
// 				if(ret)
// 				{
// 					printf( "insert err data err. ret = :%d\n" , ret) ;
// 					printf( "err:%s\n",zErrMsg) ;
// 					Sql_Wrt_err_flg = 1;
// 				}
// 			}
// 			memcpy(AirCon_Err_LastSta[i],AirCon_Err_Sta[i],14);
// 		}
// 	}
// 	/* 云端通信故障 */
// 	if(SelfChkErr_EMS_Cloud_ComErr == 1 && SelfChkErr_EMS_Cloud_ComErr_last == 0)
// 	{
// 		int ret=0;
// 		Sql_ErrInter_Deal2(err_table_SelfChkErr[0],1);
// 		ret = sqlite3_exec( db , Sql_ErrData_buf , 0 , 0 , &zErrMsg );
// 		if(ret)
// 		{
// 			printf( "insert err data err. ret = :%d\n" , ret) ;
// 			printf( "err:%s\n",zErrMsg) ;
// 			Sql_Wrt_err_flg = 1;
// 		}
// 	}
// 	SelfChkErr_EMS_Cloud_ComErr_last = SelfChkErr_EMS_Cloud_ComErr;
// 	/* 换电设备通信故障 */
// 	if(SelfChkErr_EMS_CeDev_ComErr == 1 && SelfChkErr_EMS_CeDev_ComErr_last == 0)
// 	{
// 		int ret=0;
// 		Sql_ErrInter_Deal2(err_table_SelfChkErr[1],1);
// 		ret = sqlite3_exec( db , Sql_ErrData_buf , 0 , 0 , &zErrMsg );
// 		if(ret)
// 		{
// 			printf( "insert err data err. ret = :%d\n" , ret) ;
// 			printf( "err:%s\n",zErrMsg) ;
// 			Sql_Wrt_err_flg = 1;
// 		}
// 	}
// 	SelfChkErr_EMS_CeDev_ComErr_last = SelfChkErr_EMS_CeDev_ComErr;
// 	/* 充电设备通信故障 */
// 	for(i=0;i<20;i++)
// 	{
// 		if(SelfChkErr_EMS_ChgerDev_ComErr_last[i] == 0 && SelfChkErr_EMS_ChgerDev_ComErr[i] == 1)
// 		{
// 			int ret=0;
// 			/*故障代码没有更改*/
// 			Sql_ErrInter_Deal2(err_table_SelfChkErr[2],i+1);
// 			ret = sqlite3_exec( db , Sql_ErrData_buf , 0 , 0 , &zErrMsg );
// 			if(ret)
// 			{
// 				printf( "insert err data err. ret = :%d\n" , ret) ;
// 				printf( "err:%s\n",zErrMsg) ;
// 				Sql_Wrt_err_flg = 1;
// 			}
// 		}
// 	}
// 	memcpy(SelfChkErr_EMS_ChgerDev_ComErr_last,SelfChkErr_EMS_ChgerDev_ComErr,18);

// 	if(SelfChkErr_EMS_AirCon_ComErr == 1 && SelfChkErr_EMS_AirCon_ComErr_last == 0)
// 	{
// 		int ret=0;
// 		/*故障代码没有更改*/
// 		Sql_ErrInter_Deal2(err_table_SelfChkErr[3],1);
// 		ret = sqlite3_exec( db , Sql_ErrData_buf , 0 , 0 , &zErrMsg );
// 		if(ret)
// 		{
// 			printf( "insert err data err. ret = :%d\n" , ret) ;
// 			printf( "err:%s\n",zErrMsg) ;
// 			Sql_Wrt_err_flg = 1;
// 		}
// 	}
// 	SelfChkErr_EMS_AirCon_ComErr_last = SelfChkErr_EMS_AirCon_ComErr;

// 	if(SelfChkErr_EMS_FireCon_ComErr == 1 && SelfChkErr_EMS_FireCon_ComErr_last == 0)
// 	{
// 		int ret=0;
// 		/*故障代码没有更改*/
// 		Sql_ErrInter_Deal2(err_table_SelfChkErr[4],1);
// 		ret = sqlite3_exec( db , Sql_ErrData_buf , 0 , 0 , &zErrMsg );
// 		if(ret)
// 		{
// 			printf( "insert err data err. ret = :%d\n" , ret) ;
// 			printf( "err:%s\n",zErrMsg) ;
// 			Sql_Wrt_err_flg = 1;
// 		}	
// 	}
// 	SelfChkErr_EMS_FireCon_ComErr_last = SelfChkErr_EMS_FireCon_ComErr;
// }


//未完成订单 表创建
void Create_UnfinishedOrder_Table()
{
	Sql_UnfinishedOrder_Data =  "CREATE TABLE 未完成订单(\
	ID	INTEGER PRIMARY KEY,\
    换电车辆车牌号 TEXT,\
    车辆到站时间 TEXT,\
    车辆型号 TEXT,\
    电池编号 TEXT,\
    订单号 TEXT,\
    源库位 INTEGER,\
    源库位电池编号 TEXT,\
    源库位电池型号 TEXT,\
    目标库位 INTEGER,\
    ODO REAL,\
    ODO是否已发送 INTEGER,\
    ODO是否发送成功 INTEGER,\
    换电完成时间 TEXT,\
    出站请求是否已发送 INTEGER,\
    出站请求是否成功 INTEGER,\
    订单取消是否已发送 INTEGER,\
    订单取消是否成功 INTEGER \
		);";
}
//预约数据 表创建
void Create_appointment_Table()
{
	Sql_appointment_Data =  "CREATE TABLE 预约数据(\
	ID	INTEGER PRIMARY KEY,\
    库位号 INTEGER,\
    库位预约状态 INTEGER,\
    预约车辆型号 TEXT,\
    预约库位电池编号 TEXT,\
    预约车牌 TEXT,\
	先入先出值 INTEGER\
		);";
}

/*未完成订单写入对接*/
void Sql_UnfinishedOrder_Deal(void)
{
	sql_UnfinishedOrder_t UnfinishedOrder;

	strcpy(UnfinishedOrder.Plate_numbers,mid_sqlite.Cloud_Grab_carNo);//换电车辆车牌号
    strcpy(UnfinishedOrder.Car_ArrivalTime,CE_StartTime);//车辆到站时间
    strcpy(UnfinishedOrder.Car_Model,mid_sqlite.Cloud_carType_now);//车辆型号
    strcpy(UnfinishedOrder.Batt_NUM,mid_sqlite.Cloud_BatteryNo_now);//电池编号
    strcpy(UnfinishedOrder.Order_Number,Cloud_orderId_now);//订单号
    UnfinishedOrder.SourseStorage = MChar_SelfCe_sour+1;//源库位
    strcpy(UnfinishedOrder.SourseStorage_Batt_NUM,sql_BattNum);//源库位电池编号
	strcpy(UnfinishedOrder.SourseStorage_Car_Model,mid_sqlite.Cloud_carType_now);//源
 
    UnfinishedOrder.Target_Location = UnfinishedOrder.SourseStorage;//目标库位
    UnfinishedOrder.Batt_ODO = 0;//ODO
    UnfinishedOrder.If_ODO_HasSend_flg = 1;//ODO是否已发送
    UnfinishedOrder.If_ODO_SendOk_flg = 1; 

    strcpy(UnfinishedOrder.RB_CompleteTime,NowTime); 
    UnfinishedOrder.If_CarExitReq_HasSend_flg = 1;//出站请求是否已发送
    UnfinishedOrder.If_CarExitReq_SendOk_flg = 1;//  
    UnfinishedOrder.If_OrderCancelReq_HasSend_flg = 1;//订单取消是否已发送
    UnfinishedOrder.If_OrderCancelReq_SendOk_flg = 1;//  

	sprintf(Sql_UnfinishedOrder_buf,"INSERT INTO 未完成订单 \
		VALUES (Null,\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",%d,\
        \"%s\",\"%s\",%d,%f,%d,%d,\"%s\",%d,%d,%d,%d\
        );",\
		UnfinishedOrder.Plate_numbers,UnfinishedOrder.Car_ArrivalTime,\
        UnfinishedOrder.Car_Model,UnfinishedOrder.Batt_NUM,\
        UnfinishedOrder.Order_Number,UnfinishedOrder.SourseStorage,\
        UnfinishedOrder.SourseStorage_Batt_NUM,UnfinishedOrder.SourseStorage_Car_Model,\
        UnfinishedOrder.Target_Location,UnfinishedOrder.Batt_ODO,\
        UnfinishedOrder.If_ODO_HasSend_flg,UnfinishedOrder.If_ODO_SendOk_flg,\
        UnfinishedOrder.RB_CompleteTime,UnfinishedOrder.If_CarExitReq_HasSend_flg,\
        UnfinishedOrder.If_CarExitReq_SendOk_flg,UnfinishedOrder.If_OrderCancelReq_HasSend_flg,\
        UnfinishedOrder.If_OrderCancelReq_SendOk_flg);
}

/*预约数据写入对接*/
void Sql_appointment_Deal(uint8_t locate)
{
	sql_appointment_t appointment;

	appointment.StorageNum = (locate+1);
	appointment.appoint_flg = mid_sqlite.MLocat_BookLock_flg[locate];
	if(appointment.appoint_flg){
		if(CHU_BattType[locate]==1){
			strcpy(appointment.Car_Model,"EU300");
			strcpy(appointment.Batt_NUM,mid_sqlite.MLocat_BatteryNo[locate]);
			strcpy(appointment.Plate_numbers,mid_sqlite.Cloud_BookcarNo1[SqlCloud_BookNum[0]++]);
		}
		else if(CHU_BattType[locate]==2){
			strcpy(appointment.Car_Model,"EU5A");
			strcpy(appointment.Batt_NUM,mid_sqlite.MLocat_BatteryNo[locate]);
			strcpy(appointment.Plate_numbers,mid_sqlite.Cloud_BookcarNo2[SqlCloud_BookNum[1]++]);
		}
		else if(CHU_BattType[locate]==3){
			strcpy(appointment.Car_Model,"EU5B");
			strcpy(appointment.Batt_NUM,mid_sqlite.MLocat_BatteryNo[locate]);
			strcpy(appointment.Plate_numbers,mid_sqlite.Cloud_BookcarNo3[SqlCloud_BookNum[2]++]);
		}
	}
	else{
		strcpy(appointment.Car_Model,"");
		strcpy(appointment.Batt_NUM,"");
		strcpy(appointment.Plate_numbers,"");
	}
	sprintf(Sql_appointment_buf,"INSERT INTO 预约数据 \
		VALUES (Null,\"%d\",\"%d\",\"%s\",\"%s\",\"%s\",\"%d\");",\
		appointment.StorageNum,appointment.appoint_flg,\
        appointment.Car_Model,appointment.Batt_NUM,appointment.Plate_numbers,MLocat_FIFO_Buf[locate]);
		// printf("sadsad = %s\n",Sql_appointment_buf);
}
/*预约数据删除*/
void Sql_appointment_Dele(void)
{
	// sql_UnfinishedOrder_t UnfinishedOrder;
    strcpy(Sql_appointment_buf,"DELETE from 预约数据 where ID > 0; " \
         	"SELECT * from 预约数据") ;
}
/*未完成订单删除*/
void Sql_UnfinishedOrder_Del(void)
{
	// sql_UnfinishedOrder_t UnfinishedOrder;
    // strcpy(Sql_UnfinishedOrder_buf,"DELETE from 未完成订单 where ID > 0; " \
    //      	"SELECT * from 未完成订单") ;
	strcpy(Sql_UnfinishedOrder_buf,"DELETE from 未完成订单 where ID = (select max(ID) from 未完成订单 ); " \
         	"SELECT * from 未完成订单") ;
	printf("deleedledeledeledeledel\n");
}

void sql_AddRTTable()
{
	sql_changeBattRtData_t tmp = {0};

	(tmp.SourseStorage = MChar_SelfCe_sour+1);
	strcpy(tmp.Plate_numbers ,mid_sqlite.Cloud_Grab_carNo);
	strcpy(tmp.ArrivalTime ,"CE_StartTime");
	strcpy(tmp.Model ,mid_sqlite.Cloud_carType_now);
	strcpy(tmp.Batt_NUM ,mid_sqlite.Cloud_BatteryNo_now);
	strcpy(tmp.Order_Number ,Cloud_orderId_now);

	strcpy(tmp.SourseBatt_NUM ,BMS_BATT_NUM[MChar_SelfCe_sour]);

	(tmp.AotoChangeBattRunSta = mid_sqlite.MCE_WriSqlShutDown_sta);
	tmp.TargetLocation = tmp.SourseStorage;
	
	sprintf(cmdBuf,"INSERT INTO 实时数据 \
		VALUES (1,%d,\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\
				%d,\"%s\",%d\
        );",
		tmp.AotoChangeBattRunSta,tmp.Plate_numbers,tmp.ArrivalTime,tmp.Model,tmp.Batt_NUM,\
		tmp.Order_Number,tmp.SourseStorage,tmp.SourseBatt_NUM,\
		tmp.TargetLocation\
		);
}

void sql_updateRTTable1()
{
	sprintf(cmdBuf,"update 实时数据 set 自动换电运行状态 = %d where ID=1;",mid_sqlite.MCE_WriSqlShutDown_sta);
}

void sql_updateRTTable2()
{
	sql_changeBattRtData_t tmp = {0};

	(tmp.SourseStorage = MChar_SelfCe_sour+1);

	strcpy(tmp.Plate_numbers ,mid_sqlite.Cloud_Grab_carNo);
	strcpy(tmp.ArrivalTime ,"CE_StartTime22222");
	strcpy(tmp.Model ,mid_sqlite.Cloud_carType_now);
	strcpy(tmp.Batt_NUM ,mid_sqlite.Cloud_BatteryNo_now);
	strcpy(tmp.Order_Number ,Cloud_orderId_now);

	strcpy(tmp.SourseBatt_NUM ,BMS_BATT_NUM[MChar_SelfCe_sour]);

	(tmp.AotoChangeBattRunSta = mid_sqlite.MCE_WriSqlShutDown_sta);
	tmp.TargetLocation = tmp.SourseStorage;

	sprintf(cmdBuf,"UPDATE 实时数据 set  自动换电运行状态 = %d,\
										换电车辆车牌号 = \"%s\",\
										车辆到站时间 = \"%s\",\
										车辆型号 = \"%s\",\
										电池编号 = \"%s\",\
										订单号 = \"%s\",\
										源库位 = %d,\
										源库位电池编号 = \"%s\",\
										目标库位 = %d\
										where ID=1;",
		tmp.AotoChangeBattRunSta,tmp.Plate_numbers,tmp.ArrivalTime,tmp.Model,tmp.Batt_NUM,\
		tmp.Order_Number,tmp.SourseStorage,tmp.SourseBatt_NUM,\
		tmp.TargetLocation\
		);
}

void sql_deleteRTTable()
{
	sql_changeBattRtData_t tmp = {0};
	strcpy(tmp.Plate_numbers ,"");
	strcpy(tmp.ArrivalTime ,"");
	strcpy(tmp.Model ,"");
	strcpy(tmp.Batt_NUM ,"");
	strcpy(tmp.Order_Number ,"");
	(tmp.SourseStorage = 0);
	strcpy(tmp.SourseBatt_NUM ,"");
	tmp.AotoChangeBattRunSta = mid_sqlite.MCE_WriSqlShutDown_sta;
	tmp.TargetLocation =0;
	
	sprintf(cmdBuf,"UPDATE 实时数据 set  自动换电运行状态 = %d,\
										换电车辆车牌号 = \"%s\",\
										车辆到站时间 = \"%s\",\
										车辆型号 = \"%s\",\
										电池编号 = \"%s\",\
										订单号 = \"%s\",\
										源库位 = %d,\
										源库位电池编号 = \"%s\",\
										目标库位 = %d\
										where ID=1;",
		tmp.AotoChangeBattRunSta,tmp.Plate_numbers,tmp.ArrivalTime,tmp.Model,tmp.Batt_NUM,\
		tmp.Order_Number,tmp.SourseStorage,tmp.SourseBatt_NUM,\
		tmp.TargetLocation\
		);
}


//code ***************

void Create_acEnergy_Table()
{
	Sql_acEnergy_Record = "create table 交流电表数据(\
ID INTEGER PRIMARY KEY, \
时间 TEXT,\
当前组合有功总电能 REAL,\
当前正向总有功电能 REAL,\
当前反向总有功电能 REAL,\
当前组合无功总电能 REAL,\
当前正向总无功电能 REAL,\
当前反向总无功电能 REAL,\
A相电压 REAL,\
B相电压 REAL,\
C相电压 REAL,\
A相电流 REAL,\
B相电流 REAL,\
C相电流 REAL,\
A相有功功率 REAL,\
B相有功功率 REAL,\
C相有功功率 REAL,\
A相无功功率 REAL,\
B相无功功率 REAL,\
C相无功功率 REAL,\
A相功率因数 REAL,\
B相功率因数 REAL,\
C相功率因数 REAL,\
A相过压 INTEGER,\
B相过压 INTEGER,\
C相过压 INTEGER,\
A相失压 INTEGER,\
B相失压 INTEGER,\
C相失压 INTEGER,\
A相逆向 INTEGER,\
B相逆向 INTEGER,\
C相逆向 INTEGER,\
DI状态 INTEGER,\
DO状态 INTEGER,\
电压变比PT INTEGER,\
电流变比PT INTEGER\
);";

}

typedef struct sql_acEnergy{
uint8_t logTime[50];
double ac_GroupActiveTotalEnergy ;		//当前组合有功总电能 
double ac_PositiveActiveTotalEnergy ;		//当前正向总有功电能
double ac_NegativeActiveTotalEnergy ;		//当前反向总有功电能
double ac_GroupReactiveTotalEnergy ;		//当前组合无功总电能
double ac_PositiveReactiveTotalEnergy ;	//当前正向总无功电能
double ac_NegativeReactiveTotalEnergy ;	//当前反向总无功电能
double acA_Voltage;		//A相电压 
double acB_Voltage ;		//B相电压 
double acC_Voltage ;		//C相电压 
double acA_Current ;		//A相电流 
double acB_Current ;		//B相电流 
double acC_Current ;		//C相电流 

double acA_ActivePower ;	//A 相有功功率 
double acB_ActivePower;	//B 相有功功率 
double acC_ActivePower ;	//C 相有功功率 
double acA_ReactivePower ;	//A 相无功功率 
double acB_ReactivePower ;	//B 相无功功率 
double acC_ReactivePower ;	//C 相无功功率 
double acA_PhasePowerFactor ;//A 相功率因数 
double acB_PhasePowerFactor ;//B 相功率因数 
double acC_PhasePowerFactor;//C 相功率因数 

uint16_t ac_Err;//交流电表告警

uint8_t acDI_Sta;
uint8_t acDO_Sta;
double acPt ;			//电压变比PT
double acCt ;			//电流变比CT

}sql_acEnergy_t;



void Sql_acEnergy_Deal()
{
	sql_acEnergy_t acData;
	strcpy(acData.logTime,NowTime);
		acData.ac_GroupActiveTotalEnergy = ac_GroupActiveTotalEnergy;
		acData.ac_PositiveActiveTotalEnergy = ac_PositiveActiveTotalEnergy;
		acData.ac_NegativeActiveTotalEnergy = ac_NegativeActiveTotalEnergy;
		acData.ac_GroupReactiveTotalEnergy = ac_GroupReactiveTotalEnergy;
		acData.ac_PositiveReactiveTotalEnergy = ac_PositiveReactiveTotalEnergy;
		acData.ac_NegativeReactiveTotalEnergy = ac_NegativeReactiveTotalEnergy;
		acData.acA_Voltage = acA_Voltage;
		acData.acB_Voltage = acB_Voltage;
		acData.acC_Voltage = acC_Voltage;
		acData.acA_Current = acA_Current;
		acData.acB_Current = acB_Current;
		acData.acC_Current = acC_Current;
		acData.acA_ActivePower = acA_ActivePower;
		acData.acB_ActivePower = acB_ActivePower;
		acData.acC_ActivePower = acC_ActivePower;
		acData.acA_ReactivePower = acA_ReactivePower;
		acData.acB_ReactivePower = acB_ReactivePower;
		acData.acC_ReactivePower = acC_ReactivePower;
		acData.acA_PhasePowerFactor = acA_PhasePowerFactor;
		acData.acB_PhasePowerFactor = acB_PhasePowerFactor;
		acData.acC_PhasePowerFactor = acC_PhasePowerFactor;
		acData.acDI_Sta = 0;
		acData.acDO_Sta = 0;
		acData.acPt = acPt;
		acData.acCt = acCt;

		acData.ac_Err = ac_Err;

	uint8_t acErrString[50] = "";
	int i=0;
	for(i=0;i<9;++i)
	{
		uint8_t temp[20] = "";
		sprintf(temp,"%d,",(acData.ac_Err>>i) & 0x01);
		strcat(acErrString,temp);
	}

	sprintf(Sql_acEnergyData_buf,"INSERT INTO 交流电表数据\
		VALUES (Null,\"%s\",\
		%.2lf,%.2lf,%.2lf,\
		%.2lf,%.2lf,%.2lf,\
		%.2lf,%.2lf,%.2lf,\
		%.2lf,%.2lf,%.2lf,\
		%.2lf,%.2lf,%.2lf,\
		%.2lf,%.2lf,%.2lf,\
		%.2lf,%.2lf,%.2lf,\
		%s\
		%d,%d,%.2lf,%.2lf\
		);",acData.logTime,
		acData.ac_GroupActiveTotalEnergy,acData.ac_PositiveActiveTotalEnergy,acData.ac_NegativeActiveTotalEnergy,\
		acData.ac_GroupReactiveTotalEnergy,acData.ac_PositiveReactiveTotalEnergy,acData.ac_NegativeReactiveTotalEnergy,\
		acData.acA_Voltage,acData.acB_Voltage,acData.acC_Voltage,\
		acData.acA_Current,acData.acB_Current,acData.acC_Current,\
		acData.acA_ActivePower,acData.acB_ActivePower,acData.acC_ActivePower,\
		acData.acA_ReactivePower,acData.acB_ReactivePower,acData.acC_ReactivePower,\
		acData.acA_PhasePowerFactor,acData.acB_PhasePowerFactor,acData.acC_PhasePowerFactor,\
		acErrString,\
		acData.acDI_Sta,acData.acDO_Sta,\
		acData.acPt,acData.acCt
		);
		printf("Sql_acEnergyData_buf = %s\n",Sql_acEnergyData_buf);
}



