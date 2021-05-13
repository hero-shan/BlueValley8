#ifndef _TASK_SQLITE_H_
#define _TASK_SQLITE_H_

typedef unsigned char uint8_t;


#define DEV_SHELFCHECK  1
#define WAIT_CAR    2
#define ALLOW_CARIN    3
#define IN_PLATFORM  4
#define REPLACEBATT_UNFINISH    5
#define REPLACEBATT_FINISH    6
#define CAR_OUT    7
#define ORDER_CANCELED	8




#define STEPCHANGED(a) ((a - a##_last))
#define UPDATESTA(a) (a##_last = a)

#define SQL_CHANGEBATT_RT_DATA  "CREATE TABLE 实时数据(\
	ID	INTEGER PRIMARY KEY,\
    自动换电运行状态 INTEGER,\
	换电车辆车牌号 TEXT,\
    车辆到站时间 TEXT,\
    车辆型号 TEXT,\
    电池编号 TEXT,\
    订单号 TEXT,\
    源库位 INTEGER,\
    源库位电池编号 TEXT,\
    目标库位 INTEGER\
		);";


/* changeBattRtData  */
typedef struct{
	uint8_t Plate_numbers[20];//换电车辆车牌号
	uint8_t ArrivalTime[30];//车辆到站时间
	uint8_t Model[10];//车辆型号
	uint8_t Batt_NUM[50];//电池编号
	uint8_t Order_Number[50];//订单号
	uint8_t SourseStorage;//源库位
	uint8_t SourseBatt_NUM[50];//源库位电池编号
    uint8_t AotoChangeBattRunSta;//自动换电运行状态
    uint8_t TargetLocation;//目标库位

}sql_changeBattRtData_t;

void sql_AddRTTable();
void sql_updateRTTable1();
void sql_updateRTTable2();
void sql_deleteRTTable();


//车辆换电记录 使用数据
typedef struct {
	uint8_t Plate_numbers[20];//换电车辆车牌号
	uint8_t Car_ArrivalTime[30];//车辆到站时间
	uint8_t Car_Model[10];//车辆型号
	uint8_t Batt_NUM[50];//电池编号
	uint8_t Order_Number[50];//订单号
	uint8_t SourseStorage;//源库位
	uint8_t SourseStorage_Batt_NUM[50];//源库位电池编号
	uint8_t SourseStorage_Car_Model[10];//源库位电池型号
	double ReplaceBatt_Available_Energy;//换上电池可用能量
	double ReplaceBatt_Available_Cap;//换上电池可用容量
	double ReplaceBatt_SOC;//换上电池SOC
	double RBCar_Batt_Available_Energy;//换下电池可用能量
	double RBCar_Batt_Available_Cap;//换下电池可用容量
	double RBCar_Batt_SOC;//换下电池SOC
	uint8_t Target_Location;//目标库位
	double Batt_ODO;//ODO
	uint8_t If_ODO_HasSend_flg;//ODO是否已发送
	uint8_t If_ODO_SendOk_flg;//ODO是否发送成功
	uint8_t If_RBInf_HasSend_flg;//换电信息是否已发送
	uint8_t If_RBInf_SendOk_flg;//换电信息是否发送成功
	uint8_t RB_CompleteTime[30];//换电完成时间
	uint8_t If_CarExitReq_HasSend_flg;//出站请求是否已发送
	uint8_t If_CarExitReq_SendOk_flg;//出站请求是否成功
	uint8_t If_OrderCancelReq_HasSend_flg;//订单取消是否已发送
	uint8_t If_OrderCancelReq_SendOk_flg;//订单取消是否成功
	uint8_t If_SoftWare_HasReset_flg;//软件是否复位
} sql_ChageBattRecord_t;


//电池包数据 使用数据
typedef struct{

	unsigned char Time_now[100];
	unsigned char StorageNum;//库位号
	int Batt_SOH;//	动力电池健康度 INTEGER
	unsigned char BMS_ParallNum_Null;  //BMS传出电池并联数
	unsigned char BMS_SerialNum_Null;  //BMS传出电池串联数
	int BATT_TEMP_SAMPLE_NUM;//温度采集点个数
	double	BMS_ODO;//动力电池行驶里程

	double BMS_SW_VERS_main;	//软件版本
	double BMS_SW_VERS;	//软件版本
	double BMS_HW_VERS;	//硬件版本

	double BMS_PART_VERS;//BMS零件号
	uint8_t BMS_BATT_NUM[32];//动力电池编码

	uint8_t BMS_POS_RLY_STA;//正极继电器状态
	uint8_t BMS_NEG_RLY_STA;//负极继电器状态
	uint8_t BMS_PRE_CHG_RLY_STA;//预充继电器当前状态
	uint8_t BMS_BATT_HEAT_RLY_STA;//加热继电器当前状态	//650加热不对
	uint8_t BMS_CHG_RLY_STA;//充电继电器当前状态	//EU300无该信号

	double BMS_BATT_SOC;    //动力电池剩余电量SOC
	double BMS_BATT_VOLT_V1; //动力电池内部总电压V1
	double BMS_BATT_VOLT_V2;  //动力电池外部总电压V2
	double BMS_BATT_VOLT_V3 ;  //动力电池负载端总电压V3
	double BMS_BATT_CUR;     //动力电池充/放电电流

	double BMS_BATT_POSITIVE_RESISTANCE; 	//动力电池正极对GND绝缘电阻
	double BMS_BATT_NEGATIVE_RESISTANCE; 	//动力电池负极对GND绝缘电阻
	double BMS_BATT_AVAIL_ENERGY ;       	//动力电池可用能量
	double BMS_BATT_AVAIL_CAPACITY;      	//动力电池可用容量
	double BMS_CHARGE_TOT_AH;       		//累计充电总安时

	uint8_t BATT_WORK_MODE;		//快换电池工作模式	EU300无该信号
										//0X00行车模式
										//0X01换电站监控
										//0X02换电站充电模式
										//0X03储能模式

	double BATT_ALLW_OUTPUT_CUR;	//电池包最大允许输出电流 EU300无该信号
	double BATT_ALLW_FB_CUR;		//电池包最大允许回馈电流 EU300无该信号
	double BMS_FEED_MAX_PWR;             	//当前状态允许最大回馈功率
	double BMS_DISCHG_MAX_PWR;				//当前状态最大允许放电功率

	uint8_t BMS_COOLING_FAN_STA;	//动力电池冷却风扇当前状态
	uint8_t BMS_BATT_HEAT_STA;		//动力电池加热状态
	uint8_t BMS_BAT_BALANCE_STA;	//动力电池均衡状态


	uint8_t BMS_SYS_STA;			//动力电池系统状态	EU300无该信号
	uint8_t BATT_Chg_Sta;		//动力电池充电状态	EU300无该信号
	uint8_t BATT_PWR_LIMIT_RATIO_K;	//电池功率限制比例K	EU300无该信号

	double BMS_CELL_VOLT_MAX;//单体最高电压
	double BMS_CELL_VOLT_MIN;//单体最低电压
	double BMS_CELL_TEMP_MAX;//单体最高温度
	double BMS_CELL_TEMP_MIN;//单体最低温度

	uint8_t BMS_MAX_VOLT_NUM;//最高单体电压编号
	uint8_t BMS_MAX_TEMP_NUM;//最高温度编号
	uint8_t BMS_MIN_TEMP_NUM;//最低温度编号
	uint8_t BMS_MIN_VOLT_NUM;//最低单体电压编号

	// char vol_buf[200];
	// char temp_buf[200];
	// char ss_vol[20][2000];
	// char ss_temp[20][2000];

}sql_BattData_t;


/* 充电数据  */
typedef struct{

	double CellVol[150];
	int CellTemp[40];
	int Soc;
	int PosTemp;
	int NegTemp;
	double CharVol;
	double CharCur;
	double MaxPow;
	double RealPow;
	double ReqVol;
	double ReqCur;
	int CellNum;
	int TempNum;
	double DCEnVol_V;
	double DCEnCur_A;
	double DCEn_Act_W;
	double DCEnPos_Act_W;
	double DCEnNeg_Act_W;

}sql_CharData_t;


/* 配置参数  */
typedef struct {

uint8_t CEStation_Name[1000];
int GridMaxOutPow;
int CharEndSoc;
int CE_AllowSoc;
int Charer_ena;
int Char_ReqPow;

}sql_Config_t;

/* 故障记录  */
typedef struct{

char ErrDev[20];
int ErrDev_count;//设备序号
char Err_Lev;
char Err_code[20];
char ErrName[100];
char ErrTime[100];

}sql_ErrRecord_t;


/* 空调数据  */
typedef struct {

int AirCon_DevNum;
int AirCon_Temp;
int AirCon_Humi;
}sql_AirConData_t;


/* 库位信息  */
typedef struct{

char Locator_sta[10];
int Locator_ena;
char Bat_code[100];
char Bat_type[100];
char Locator_time[100];
char Char_ok[10];
char Char_ok_time[100];

}sql_LocatorData_t;

/* 未完成订单  */
typedef struct{
	uint8_t Plate_numbers[20];//换电车辆车牌号
	uint8_t Car_ArrivalTime[30];//车辆到站时间
	uint8_t Car_Model[10];//车辆型号
	uint8_t Batt_NUM[50];//电池编号
	uint8_t Order_Number[50];//订单号
	uint8_t SourseStorage;//源库位
	uint8_t SourseStorage_Batt_NUM[50];//源库位电池编号
	uint8_t SourseStorage_Car_Model[10];//源库位电池型号
    uint8_t Target_Location;//目标库位
	double Batt_ODO;//ODO
	uint8_t If_ODO_HasSend_flg;//ODO是否已发送
	uint8_t If_ODO_SendOk_flg;//ODO是否发送成功
	
	uint8_t RB_CompleteTime[30];//换电完成时间
	uint8_t If_CarExitReq_HasSend_flg;//出站请求是否已发送
	uint8_t If_CarExitReq_SendOk_flg;//出站请求是否成功
	
	uint8_t If_OrderCancelReq_HasSend_flg;//订单取消是否已发送
	uint8_t If_OrderCancelReq_SendOk_flg;//订单取消是否成功

}sql_UnfinishedOrder_t;
/* 预约数据  */
typedef struct{
	uint8_t StorageNum;//库位号
	uint8_t appoint_flg;//库位预约状态
	uint8_t Car_Model[10];//预约车辆型号
	uint8_t Batt_NUM[50];//预约库位电池编号
	uint8_t Plate_numbers[20];//预约车牌号
}sql_appointment_t;
/*创建TABLE函数*/
void Create_CE_Record_Table();
void Create_BattData_Table();
void Create_CharData_Table();
void Create_ConfigData_Table();
void Create_ErrData_Table();
void Create_AirConData_Table();
void Create_LocatorData_Table();
void Create_UnfinishedOrder_Table();
void Create_appointment_Table();
void Create_acEnergy_Table();

//**************************************
void GetMacTime();/*获取系统时间*/

void Sql_BattDataInter_Deal(uint8_t LocatorNum);/*电池数据接口函数*/
void Sql_CEDataInter_Deal(void);/*换电数据接口函数*/
void Sql_CharInter_Deal(uint8_t locate);/*充电数据接口函数*/
void Sql_ConfInter_Deal(void);/*配置信息接口函数*/
void Sql_ErrInter_Deal(void);/*故障信息接口函数*/
void Sql_AirInter_Deal(uint8_t num);/*空调信息接口函数*/
void Sql_LocatInter_Deal(uint8_t num);/*库位信息接口函数*/
void Sql_UnfinishedOrder_Deal(void);
void Sql_UnfinishedOrder_Del(void);
void Sql_appointment_Deal(uint8_t locate);
void Sql_appointment_Dele(void);


void Sql_acEnergy_Deal();


#endif