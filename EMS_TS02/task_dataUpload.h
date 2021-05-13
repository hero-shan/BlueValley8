#ifndef _TASK_DATAUPLOAD_H
#define _TASK_DATAUPLOAD_H

#include "cJSON.h"



/* 换电站实时数据 换电 */
typedef struct{
    char ChangeStatus;/*是否成功  0:成功  1:失败 */
    char CarNo[20];/*车辆车牌 */
    char NewBatteryNo[32];/*满电电池包编码*/
    char OldBatteryNo[32];/*空电电池包编码 */
    char NewSeat;/*满电电池包原来的位置 */
    char OldSeat;/*空电电池包放置位置 */
    char ChangeDate[30];/* 2019-01-17 09:10:25 换电时间yyyy-MM-dd HH:mm:ss */
    char electricityDown;/* 换下电池可用电量 */
    char electricityUp;/* 换上电池可用电量 */
    char capacityDown;/* 换下电池可用容量 */
    char capacityUp;/* 换上电池可用容量 */
    char socUp;/* 换上电池SOC */
    char socDown;/* 换下电池SOC */
    char logDatetime[30];/* 记录时间 */

}net_ChangeData_t;

/* 换电站实时数据 直流电表 */
typedef struct{
    char ammeterNo;/* 电流表编号 */
    char ammeterState;/* 开关量0--未连接  1--连接  2--通信故障 */
    double ammeterVoltage;/* 电压（V） */
    double ammeterCurrent;/* 电流（A） */
    double ammeterPower;/* 有功功率（kwh） */
    double ammeterPositivePower;/* 正向有功功率 */
    double ammeteNegativePower;/* 逆向有功功率 */
    double ammeterDcKwh;/* 电表直流耗电量（kwh） */

}net_DcMeterData_t;

/* 换电站实时数据 交流电表 */
typedef struct{
    double	acAmmeterSatate;	//	设备开关量0--未连接  1--连接  2--通信故障
    double	acGroupActiveTotalEnergy;	//	当前组合有功总电能(kwh)
    double	acGroupActivePointedEnergy;	//	当前组合有功功尖电能（kwh）
    double	acGroupActivePeekEnergy;	//	当前组合有功峰电能(kwh)
    double	acGroupActiveFlatEnergy;	//	当前组合有功功平电能（kwh）
    double	acGroupActiveValleyEnergy;	//	当前组合有功功谷电能（kwh）
    double	acPositiveActiveTotalEnergy;	//	当前正向总有功电能(kwh)
    double	acPositiveActivePointedEnergy;	//	当前正向有功尖电能(kwh)
    double	acPositiveActivePeekEnergy;	//	当前正向有功峰电能（kwh）
    double	acPositiveActiveFlatEnergy;	//	当前正向有功平电能(kwh)
    double	acPositiveActiveValleyEnergy;	//	当前正向有功谷电能(kwh)
    double	acNegativeActiveTotalEnergy;	//	当前反向总有功电能(kwh)
    double	acNegativeActivePointedEnergy;	//	当前反向有功尖电能(kwh)
    double	acNegativeActivePeekEnergy;	//	当前反向有功峰电能(kwh)
    double	acNegativeActiveFlatEnergy;	//	当前反向有功平电能(kwh)
    double	acNegativeActiveValleyEnergy;	//	当前反向有功谷电能(kwh)
    double	acGroupReactiveTotalEnergy;	//	当前组合无功总电能(kwh)
    double	acGroupReactivePointedEnergy;	//	当前组合无功功尖电能（kwh）
    double	acGroupReactivePeekEnergy;	//	当前组合无功功峰电能(kwh)
    double	acGroupReactiveFlatEnergy;	//	当前组合无功功平电能（kwh）
    double	acGroupReactiveValleyEnergy;	//	当前组合无功功谷电能（kwh）
    double	acPositiveReactiveTotalEnergy;	//	当前正向总无功电能(kwh)
    double	acPositiveReactivePointedEnergy;	//	当前正向有无尖电能(kwh)
    double	acPositiveReactivePeekEnergy;	//	当前正向无功峰电能（kwh）
    double	acPositiveReactiveFlatEnergy;	//	当前正向无功平电能(kwh)
    double	acPositiveReactiveValleyEnergy;	//	当前正向无功谷电能(kwh)
    double	acNegativeReactiveTotalEnergy;	//	当前反向总无功电能(kwh)
    double	acNegativeReactivePointedEnergy;	//	当前反向无功尖电能(kwh)
    double	acNegativeReactivePeekEnergy;	//	当前反向无功峰电能(kwh)
    double	acNegativeReactiveFlatEnergy;	//	当前反向无功平电能(kwh)
    double	acNegativeReactiveValleyEnergy;	//	当前反向无功谷电能(kwh)
    double	acAVoltage;	//	A相电压V
    double	acBVoltage;	//	B相电压V
    double	acCVoltage;	//	C相电压V
    double	acACurrent;	//	A相电流A
    double	acBurrent;	//	B相电流A
    double	acCurrent;	//	C相电流A
    double	acAActivePower;	//	A相有功功率kw
    double	acBctivePower;	//	B相有功功率kw
    double	acCctivePower;	//	C相有功功率kw
    double	acTotalActivePower;	//	总有功功率kw
    double	acAReactivePower;	//	A相无功功率kVar
    double	acBReactivePower;	//	B相无功功率kVar
    double	acCReactivePower;	//	C相无功功率kVar
    double	acTotalReactivePower;	//	总无功功率kVar
    double	acALookPower;	//	A相视在功率kVA
    double	acBLookPower;	//	B相视在功率kVA
    double	acCLookPower;	//	C相视在功率kVA
    double	acTotalLookPower;	//	总视在功率kVA
    double	acAPhasePowerFactor;	//	A相功率因数
    double	acBPhasePowerFactor;	//	B相功率因数
    double	acCPhasePowerFactor;	//	C相功率因数
    double	acTotalPowerFactor;	//	总功率因数
    double	acCt;	//	电流变比CT
    double	acPt;	//	电压变比PT
    double	acZroeCurrent;	//	零序电流
    double	acVoltageUneven;	//	电压不平衡度%
    double	acCurrentUneven;	//	电流不平衡度%
    char logDatetime[30];/* 记录时间 */   
}net_AcMeterData_t;


/* 换电站实时数据 库位信息 */
typedef struct{

    char seatNo;/* 库位信息（此字段为必要信息） */
    char currentSoc;/* 电池包当前soc */
    char endSoc;/* 库位包截止充电时soc */
    char batteryNo[32];/* 电池包溯源码 */
    char chargeState;/* 充电状态0-无电池，1-待充电，2-充电中，3-充电完成，4-有故障 */
    char logDatetime[30];/* 2019-11-21 17:53:30	记录时间 */
    char strategyChargeStartTime[30];/* 2019-11-21 17:53:30	库位充电时间 */
    char strategyIsValid;/* 有效状态状态，0-失效，1-有效，无效则按照放到到库位则充电 */
}net_SeatInfoData_t;

/* 换电站实时数据 换电站运行数据 */
typedef struct{
    char StationStatus;/* 整站状态  0:正常 1:故障 */
    char Device1Sta;/* 视频监控状态 0:正常 1:故障 */
    char Device2Sta;/* 码垛机状态 0:正常 1:故障 */
    char Device3Sta;/* RGV状态 0:正常 1:故障 */
    char Device4Sta;/* 充电机状态 0:正常 1:故障
    充电机全部正常时，为正常，只要有一个故障，就报故障。故障信息提现在Message里 */

    char Device5Sta;/* 电池包状态 0:正常 1:故障 */
    char Device6Sta;/* 消防设备状态 0:正常 1:故障 */
    char Device7Sta;/* 交流电表状态 0:正常 1:故障 */
    char Device8Sta;/* 空调设备状态 0:正常 1:故障 */
    char* Message;/* 异常信息，如果多个设备异常，多个异常信息一起发送 */
    char ErrLevel;/* 异常等级   0:正常  1:警告  2:异常但能运行 3:严重异常不可运行  4火警 */
    char RunStatus;/* 站的运行状态 1自动   2 手动 */
    char logDatetime[30];/* 记录时间 */

}net_RunData_t;

/* 换电站实时数据 当前站内电池的充电数据 */
typedef struct{
    char SeatNo;/* 电池支架位置编号 */
    char BatteryNo[32];/* 电池包编号 */
    char SOC;/* 两位小数（如数据12表示为12%） */
    uint8_t Voltage[128];/* 从BMS获取的单体电压信息(两位小数   38120表示为38.120) */
    char Temperature[32];/* 从BMS获取的单体温度信息 */
    double Current;/* 充电电流 */
    double MaxVoltage;	/* 单体电芯最高电压 */
    double MinVoltage;	/* 单体电芯最低电压 */
    double MaxTemperature;	/* 单体电芯最高温度 */
    double MinTemperature;	/* 单体电芯最低温度 */
    double GNDA;/* 动力电池正极对GND绝缘电阻 */
    double GNDZ;/* 动力电池负极对GND绝缘电阻 */
    double ZAS;	/* 累计充电总安时 */
    double V1;	/* 动力电池内部总电压V1 */
    double Instruct;/* 直流充电端电流指令 */
    double I1;	/* 动力电池充/放电电流 */
    double OutI;/* 充电机最大输出电流 */
    double NL;	/* 动力电池可用能量 */
    double RL;	/* 动力电池可用容量 */
    double BJ;	/* 电池包总成零部件号 */
    double BMSZNo;	/* BMS主软件版本号 */
    double BMSCNo;	/* BMS次软件版本号 */
    double MinTNo;	/* 最低温度编号 */
    double MaxTNo;	/* 最高温度编号 */
    double MinVNo;	/* 最低单体电压编号 */
    double MaxVNo;	/* 最高单体电压编号 */
    double Error;	/* 动力电池系统故障显示 */
    double BMSODO;  /* BMS里程 */
    char logDatetime[30];/* 记录时间 */

}net_ChargeData_t;


/* 换电站实时数据  */
typedef struct{
    char StationID[200];/* 换电站名称 */
    char StationSta;/* 换电站状态 */
    net_ChangeData_t ChangeData;/* 换电站实时数据 换电 */
    net_DcMeterData_t DcMeterData;/* 换电站实时数据 直流电表 */
    net_AcMeterData_t AcMeterData;/* 换电站实时数据 交流电表 */
    net_SeatInfoData_t SeatInfoData[20];/* 换电站实时数据 库位信息 */
    net_RunData_t RunData;/* 换电站实时数据 换电站运行数据 */
    net_ChargeData_t ChargeData[20];/* 换电站实时数据 当前站内电池的充电数据 */

}net_StationRtData_t;


cJSON *ChangeDataObjBuild();
cJSON *RunDataObjBuild();
cJSON *ChargeDataObjBuild();
cJSON *SeatInfoDataBuild();
cJSON *DcMeterDataBuild();
cJSON *AcMeterDataBuild();

void stationRtData_ChangeBatt_Interf(net_StationRtData_t * StationRtData);
void stationRtData_ErrData_Interf(net_StationRtData_t * StationRtData);
void stationRtData_Interf(net_StationRtData_t * StationRtData);

void build_station_rtdata();
void networkManger(char * url,char *jsondata);


extern net_StationRtData_t StationRtData;
extern uint8_t Cloud_ChangePwrCmplt_flg;
extern uint8_t Cloud_SysErr_flg;
extern uint8_t Cloud_RtData_flg;


#endif // !_TASK_DATAUPLOAD_H



