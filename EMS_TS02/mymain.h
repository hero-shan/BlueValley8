#include "sys/types.h"
#include "sys/stat.h"
#include "fcntl.h"
#include "stdlib.h"
#include "string.h"
#include "SA4_EMS.h"
 
#include <sys/shm.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <sys/ioctl.h>   
#include <sys/socket.h>   
#include <netdb.h>    
#include <net/if.h> 

#include <pthread.h>
#include <bits/wordsize.h>
#include "cJSON.h"
#include "cloud_platform.h"
#include "task_dataUpload.h"
#include "errMsg.h"
#include "middle.h"
#include "curl/curl.h"

#include <syslog.h>
#include <execinfo.h>

#include <sys/syscall.h>
extern pid_t gettid();

#include <signal.h>
extern void sigHandler(int signo) ;

extern pthread_mutex_t mutex_lock;
extern uint8_t stationRtDataTemp[20000];
extern char *url_sendData;

extern unsigned char mdl_send_AvailableNum_flg;
extern uint8_t QtBook_BatNax_num[3][50];

extern uint8_t MLocat_BatteryNo[20][100];
extern uint16_t Get_ChuData[10];
extern void thread_7(void);

extern uint8_t Qt_CarAllowIn_LiftBar_sta;  //2021-3-24


char CE_StartTime[100];
extern char NowTime[100];
extern uint8_t starttime[50];
extern uint8_t endtime[50];
extern uint8_t starttime1[50];
extern uint8_t endtime1[50];
extern uint8_t Cloud_SendBookCE_flg;
extern uint8_t Book_Init_flg;
extern uint8_t Send_ReConnect_buf[10];
extern uint8_t BCS2_BCSData_buf[50];

extern uint8_t BCS1_conn_flg;
extern uint8_t BCS2_conn_flg;
extern uint8_t BCS3_conn_flg;
extern uint8_t BCS4_conn_flg;
extern uint8_t Qt_conn_flg;
extern uint8_t PCS_conn_flg;
extern uint8_t link_state;

extern uint16_t QtSend_Discon_con;
extern uint16_t CESend_Discon_con;
extern uint16_t FireContrSend_Discon_con;
extern uint16_t AirConSend_Discon_con[3];
extern uint16_t CHUSend_Discon_cou[20];
extern uint16_t ACEnConSend_Discon_con;
extern uint16_t IOCardSend_Discon_con[2];
extern uint16_t RemotSend_Discon_con;
extern uint16_t DCEnConSend_Discon_con;

extern int16_t  Usr_PowerReq_kw;
extern uint8_t EMS_Run_sta;
extern uint8_t EMS_SelfSet_flg;
extern uint8_t EMS_HandSet_flg;
extern uint8_t EMS_SelfRun_flg;
extern uint8_t EMS_SysStartStop_sta;

extern uint32_t Recv_count;
extern int32_t Read_Size;

// extern uint8_t CE_InitBack_flg;
// extern uint8_t CE_InitBack_err;
// extern uint8_t CE_ChangBatBack_flg;
// extern uint8_t CE_ChangBatBack_err;
// extern uint8_t CE_ChangLocatorBack_flg;
// extern uint8_t CE_ChangLocatorBack_err;
// extern uint8_t CE_BatOutBack_flg;
// extern uint8_t CE_BatOutBack_err;
// extern uint8_t CE_BatInLocatorBack_flg;
// extern uint8_t CE_BatInLocatorBack_err;
// extern uint8_t CE_BatOutLocatorBack_flg;
// extern uint8_t CE_BatOutLocatorBack_err;
// extern uint8_t CE_DismountBatBack_flg;
// extern uint8_t CE_DismountBatBack_err;
// extern uint8_t CE_MountBatBack_flg;
// extern uint8_t CE_MountBatBack_err;
// extern uint8_t CE_RGVOutChangPlatBack_flg;
// extern uint8_t CE_RGVOutChangPlatBack_err;
// extern uint8_t CE_TimeOutBack_flg;
// extern uint8_t CE_TimeOutBack_err;
// extern uint8_t CE_ContinueBack_flg;
// extern uint8_t CE_ContinueBack_err;
// extern uint8_t CE_StopBack_flg;
// extern uint8_t CE_StopBack_err;
// extern uint8_t CE_ErrResetBack_flg;
// extern uint8_t CE_ErrResetBack_err;
// extern uint8_t CE_RunState;
// extern uint8_t CE_ControlMode;
extern uint8_t CE_ErrData[2];
// extern uint8_t CE_CarTypeNow;  	  //当前车型
// extern uint8_t CE_RGVStepData;  	  //RGV工步信息
// extern uint8_t CE_StackerStepData;   //码垛机工步信息
// extern uint8_t CE_ParkPlatStepData;  //泊车平台工步信息
// extern uint8_t CE_RotaTroSta;   	  //旋杆状态
// extern uint8_t CE_CBOk_flg;		  //换电完成
// extern uint8_t CE_DismountOk_flg;    //拆卸完成
// extern uint8_t CE_MountOk_flg;       //安装完成
// extern uint8_t CE_InLocatorOk_flg;   //入库完成
// extern uint8_t CE_OutLocatorOk_flg;  //出库完成
// extern uint8_t CE_CELocatorOk_sour;  //更换库位完成-source
// extern uint8_t CE_CELocatorOk_des;   //更换库位完成-destination
// extern uint8_t CE_BatToLocatorOk_flg;//电池放至库位完成
extern uint8_t CE_LocatorSenser[21]; //库位传感20+1
// extern uint8_t CE_RGVSenser_sta;     //RGV传感器状态
// extern uint8_t CE_StackerSenser_sta; //码垛机传感器状态
// extern uint8_t CE_TurnOverSenser_sta;//周转仓传感器状态
// extern uint8_t CE_SliOutSenser_sta;  //滑出位传感器状态
// extern uint8_t CE_UpDownDoor_sta;    //升降门状态
// extern uint8_t CE_ElecLinear_sta[20];    //电动推杆状态
// extern uint8_t CE_ParkPlatSenser_sta;    //泊车平台传感器状态


extern uint8_t IO_DoorValue1_null;
extern uint8_t IO_DoorValue2_null;
extern uint8_t IO_DoorValue3_null;
extern uint8_t IO_ThunderValue1_null;
extern uint8_t IO_ThunderValue2_null;
extern uint8_t IO_WaterValue_null;
extern uint8_t IO_LightValue1_null;
extern uint8_t IO_LightValue2_null;
extern uint8_t Sound_Start_flg;
extern uint8_t Light_Start_flg;
extern uint8_t Fan_Start_flg;
extern uint8_t Door_Start_flg;
extern uint8_t Door2_Start_flg;

extern unsigned char Staion_to_net_id[100];
extern unsigned char Staion_to_net_pwd[100];
extern unsigned char Staion_to_net_firmId[100];

//grab 抓拍
extern unsigned char Cloud_Grab_carNo[30];/*抓拍机得到的车牌信息*/
extern unsigned char Cloud_Grab_carNo2[30];
extern unsigned char Cloud_Grab_carNo_cancel[30];/*抓拍机得到的车牌信息*/
extern unsigned char Cloud_orderId_now_cancel[50];
extern unsigned char Cloud_carType_now[50];
extern unsigned char Cloud_orderId_now[50];/*云平台得到当前订单号*/
extern unsigned char Cloud_BatteryNo_now[100];/*云平台得到电池编码*/
extern unsigned char Locat_BatteryNo[20][100];/*CHU读到的电池编码*/
extern unsigned char Cloud_OutBatteryNo_now[100];
extern unsigned char ChangeBatCar_BatteryNo[100];/*换电车辆电池编码*/
extern unsigned char Cloud_CarODO_now[30];/*ODO*/

extern uint8_t ChecklostBat_err_flg;

extern uint8_t LCD_conn_flg;
extern int Cloud_Comm_flg;
extern uint8_t CE_conn_flg;
extern uint8_t Qt_conn_flg;
extern uint8_t IOCard_conn_flg;
extern uint8_t Cloud_carType_Code;
extern unsigned char Cloud_Comm_err_count;


extern uint8_t EMS_CHU_LinkState[20];
extern int CHU_clientsockfd[20];
//0x83
extern uint8_t CHU_DevState_code[20];
extern uint8_t CHU_BMS_LinkState[20];
extern uint8_t CHU_BattType[20];
extern uint8_t CHU_BattType[20];
extern double CHU_NegTemp_C[20];
extern double CHU_PosTemp_C[20];
extern uint8_t CHU_ErrCode_buf0[20];
extern uint8_t CHU_ErrCode_buf1[20];
extern uint8_t CHU_ErrCode_buf2[20];
extern uint8_t CHU_ErrCode_buf3[20];

//0x89
extern uint8_t CHUErrStop_code[20];
//0x8a
extern uint8_t BMS_CellVol_buf[20][120];
//0x8b
extern uint8_t BMS_CellTemp_buf[20][32];
extern uint8_t CHU_AllErr_flg[20];
extern uint8_t Cloud_InNet_flg[20];
extern uint8_t CHU_CharFull_flg[20];
extern uint8_t EMS_CHU_LinkState[20];

//0X83
extern uint8_t CHU_DevState_code[20];
extern uint8_t CHU_BMS_LinkState[20];
extern uint8_t CHU_BattType[20];
extern double CHU_NegTemp_C[20];
extern double CHU_PosTemp_C[20];

extern uint8_t CHU_ErrCode_buf0[20];
extern uint8_t CHU_ErrCode_buf1[20];
extern uint8_t CHU_ErrCode_buf2[20];
extern uint8_t CHU_ErrCode_buf3[20];
extern uint8_t CHU_ErrCode_buf4[20];
extern uint8_t CHU_KMState[20];//三项接触器
extern uint8_t Get_CHUHeart_flg[20];
extern uint16_t CHU_heart_err_count[20];//65535*10
//0x85
extern uint8_t CHU_NonExe_code[20];
//0x87
extern uint8_t Get_CHUChgRtData_flg[20];
extern double BMS_PackSocReal_pct[20];
extern double BMS_CharPackVolReal_V[20];
extern double BMS_CharPackCurReal_A[20];
extern double BMS_ReqPackVol_V[20];
extern double BMS_ReqPackCur_A[20];
extern uint8_t CHU_ChgMode[20];
extern double BMS_RemainCharTimeReal_min[20];
extern double ChU_AllChrTimeReal_min[20];
extern double Charger_OutPwr_W[20];
extern uint8_t FCH_BigState_Null[20];
//0x88
extern double CHU_AllOutEnergy_kwh[20];
extern uint8_t Get_StopCmdRes_flg[20];
//0x89
extern uint8_t Get_CHUErrStop_flg[20];

//0x8D
// uint8_t ChaPrepDataBuf[20][];
extern uint8_t BMS_BatteryType_Null[20];
extern double BMS_RatedCap_Ah[20];
extern double BMS_RatedPackVol_V[20];
extern double CellVol_AllowCharMax_V[20];
extern double PackCur_AllowCharMax_A[20];
extern double BMS_RatedEnergy_Kwh[20];
extern double Temp_AllowCharMax_C[20];
//0x8F 获取电池实时数据指令反馈
extern uint8_t BattRtData_buf[20][60];
extern uint8_t Rly_Sta[20];//继电器当前状态
extern double BMS_BATT_SOC[20];    //动力电池剩余电量SOC
extern double BMS_BATT_VOLT_V1[20]; //动力电池内部总电压V1
extern double BMS_BATT_VOLT_V2[20];  //动力电池外部总电压V2
extern double BMS_BATT_VOLT_V3[20];  //动力电池负载端总电压V3
extern double BMS_BATT_CUR[20];     //动力电池充/放电电流
extern double BMS_BATT_POSITIVE_RESISTANCE[20]; 	//动力电池正极对GND绝缘电阻
extern double BMS_BATT_NEGATIVE_RESISTANCE[20]; 	//动力电池负极对GND绝缘电阻
extern double BMS_BATT_AVAIL_ENERGY[20];       	//动力电池可用能量
extern double BMS_BATT_AVAIL_CAPACITY[20];      	//动力电池可用容量
extern double BMS_CHARGE_TOT_AH[20];       		//累计充电总安时
extern uint8_t BATT_WORK_MODE[20];		//快换电池工作模式	EU300无该信号
extern double BATT_ALLW_OUTPUT_CUR[20];	//电池包最大允许输出电流 EU300无该信号
extern double BATT_ALLW_FB_CUR[20];		//电池包最大允许回馈电流 EU300无该信号
extern double BMS_FEED_MAX_PWR[20];             	//当前状态允许最大回馈功率
extern double BMS_DISCHG_MAX_PWR[20];				//当前状态最大允许放电功率
extern uint8_t BMS_COOLING_FAN_STA[20];	//动力电池冷却风扇当前状态
extern uint8_t BMS_BATT_HEAT_STA[20];		//动力电池加热状态
extern uint8_t BMS_BAT_BALANCE_STA[20];	//动力电池均衡状态
extern uint8_t BMS_SYS_STA[20];			//动力电池系统状态	EU300无该信号
extern uint8_t BATT_Chg_Sta[20];		//动力电池充电状态	EU300无该信号
extern uint8_t BATT_PWR_LIMIT_RATIO_K[20];	//电池功率限制比例K	EU300无该信号
extern uint8_t BMS_Err_msg[20][7];//电池故障信息
extern double BMS_CELL_VOLT_MAX[20];//单体最高电压
extern double BMS_CELL_VOLT_MIN[20];//单体最低电压
extern double BMS_CELL_TEMP_MAX[20];//单体最高温度
extern double BMS_CELL_TEMP_MIN[20];//单体最低温度
extern uint8_t BMS_MAX_VOLT_NUM[20];//最高单体电压编号
extern uint8_t BMS_MAX_TEMP_NUM[20];//最高温度编号
extern uint8_t BMS_MIN_TEMP_NUM[20];//最低温度编号
extern uint8_t BMS_MIN_VOLT_NUM[20];//最低单体电压编号
//0x90获取电池基本信息指令反馈
extern uint8_t BattBasicInf_buf[20][60];
extern int Batt_SOH[20];//	动力电池健康度 INTEGER
extern unsigned char BMS_ParallNum_Null[20];  //BMS传出电池并联数
extern unsigned char BMS_SerialNum_Null[20];  //BMS传出电池串联数
extern int BATT_TEMP_SAMPLE_NUM[20];//温度采集点个数
extern double	BMS_ODO[20];//动力电池行驶里程
extern double BMS_SW_VERS_main[20];	//软件版本
extern double BMS_SW_VERS[20];	//软件版本
extern double BMS_HW_VERS[20];	//硬件版本
extern double BMS_PART_VERS[20];//BMS零件号
extern uint8_t BMS_BATT_NUM[20][40];//动力电池编码


/*air*/
extern int AirCon_Temp[3];
extern int AirCon_Humi[3];
extern uint8_t AirCon_MerRunSta[3];	
extern uint8_t AirCon_EnginRunSta[3];	
extern uint8_t AirCon_HertRunSta[3];
extern uint8_t Qt_AirSetData_flg[3];
extern uint8_t Qt_AirOpen_flg[3];
extern int16_t AirSet_EnginStartTemp_C;
extern int16_t AirSet_EnginStopTemp_C;
extern int16_t AirSet_HertStartTemp_C;
extern int16_t AirSet_HertStopTemp_C;
extern int16_t AirSet_FanStartTemp_C;
extern int16_t AirSet_FanStopTemp_C;
extern int16_t AirSet_ErrOverTemp_C;
extern int16_t AirSet_ErrUnderTemp_C;
extern int16_t AirRead_EnginStartTemp_C[3];
extern int16_t AirRead_HertStartTemp_C[3];
extern uint8_t AirCon_Err1[3];
extern uint8_t AirCon_Err2[3];
extern uint8_t AirCon_Err3[3];
extern uint8_t Init_starttime[3][50];
extern uint8_t Init_endtime[3][50];
extern uint8_t FireContr_GetFire_flg;
extern uint8_t FireContr_HardErr_flg;
extern uint8_t AirCon_Eachconn_sta[3];

extern int starthour,endhour;
extern uint8_t Locat_Book_num[3],Clear_Book_num[3];
extern uint8_t Cloud_BookcarNo1[20][100],Cloud_BookcarNo2[20][100],Cloud_BookcarNo3[20][100];
extern uint8_t Clear_BookcarNo1[20][100],Clear_BookcarNo2[20][100],Clear_BookcarNo3[20][100];
extern uint8_t plate[5][30];
extern uint8_t Book_Init_cou;

extern uint8_t QtEMS_ShutDown_flg;

/* 交流电表 */
extern uint8_t ACEnCon_conn_flg;
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
double acPt ;			//电压变比PT
double acCt ;			//电流变比CT
double acA_ActivePower ;	//A 相有功功率 
double acB_ActivePower;	//B 相有功功率 
double acC_ActivePower ;	//C 相有功功率 
double acA_ReactivePower ;	//A 相无功功率 
double acB_ReactivePower ;	//B 相无功功率 
double acC_ReactivePower ;	//C 相无功功率 
double acA_PhasePowerFactor ;//A 相功率因数 
double acB_PhasePowerFactor ;//B 相功率因数 
double acC_PhasePowerFactor;//C 相功率因数 
double ac_Err;//交流电表告警
/* 直流电表  */
real_T DCEnVol_V[20];
real_T DCEnCur_A[20];
real_T DCEn_Act_W[20];
real_T DCEnPos_Act_W[20];
real_T DCEnNeg_Act_W[20];


extern uint8_t Qt_CarAllowIn_err;
extern uint8_t Qt_CarAllowOut_err;
extern uint8_t Qt_CarODO_err;
extern uint8_t Qt_CancelOrd_err;
extern uint8_t Qt_InNet_err[20];

extern uint8_t Qt_ACEnergySetPT_flg;
extern uint8_t Qt_ACEnergySetCT_flg;
extern uint16_t Qt_ACEnergySetPT_tmp;
extern uint16_t Qt_ACEnergySetCT_tmp;
extern uint8_t QtIo_Disable_buf[8];

extern uint8_t Remo_SpareTire_place;
extern uint8_t Cloud_OutNet_CarNo[20][16];
extern uint8_t Cloud_Appoint_CarNo[20][16];

extern void ChangeElec(void);
extern void Charger(void);

extern char *url_getDate;
extern char *url_appointment_base;

extern void GetMacTime();



extern uint8_t sql_BattNum[40];
extern mid_netcycle_t mid_netcycle;