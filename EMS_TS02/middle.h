// #ifndef _TASK_CHARGER_H_
// #define _TASK_CHARGER_H_

// #include "mymain.h"

/* 空调中间层  */
typedef struct{
    uint8_t AirCon_MerRunSta[3];/* 机组运行状态 */
    uint8_t AirCon_EnginRunSta[3];/* 压缩机运行状态 */
    uint8_t AirCon_HertRunSta[3];/* 加热器运行状态 */
    uint8_t AirCon_Err1[3];/* 空调故障数组1 */
    uint8_t AirCon_Err2[3];/* 空调故障数组2 */
    uint8_t AirCon_Err3[3];/* 空调故障数组3 */
    int AirCon_Temp[3];/* 环境温度 */
    int AirCon_Humi[3];/* 环境湿度 */
    int16_t AirSet_EnginStartTemp_C;/* 压缩机启动温度 */
    int16_t AirSet_EnginStopTemp_C;/* 压缩机停止温度 */
    int16_t AirSet_HertStartTemp_C;/* 加热器启动温度 */
    int16_t AirSet_HertStopTemp_C;/* 加热器停止温度 */
    int16_t AirSet_FanStartTemp_C;/* 内风机待机开启温度 */
    int16_t AirSet_FanStopTemp_C;/* 内风机待机停止回差 */
    int16_t AirSet_ErrOverTemp_C;/* 柜内高温告警温度 */
    int16_t AirSet_ErrUnderTemp_C;/* 柜内低温告警温度 */

    int16_t AirRead_EnginStartTemp_C[3];/* 读取参数压缩机启动温度 */
    int16_t AirRead_HertStartTemp_C[3];/* 读取参数加热器启动温度 */

    uint8_t AirCon_SendStaData_flg[3];/* 发送读取状态数据标志 */
    uint8_t AirCon_SendErrData_flg[3];/* 发送读取告警数据标志 */
    uint8_t AirCon_SendSenserData_flg[3];/* 发送读取传感器数据标志 */
    uint8_t AirCon_SendGetData_flg[3];/* 发送读取系统参数标志 */
    uint8_t AirCon_SendSetData_flg[3];/* 发送设置参数标志 */
    uint8_t AirCon_SendOnOff_flg[3];/* 发送开关机标志 */
    uint8_t Qt_AirSetData_flg[3];/* 上位机空调参数控制指令 */
    uint8_t Qt_AirOpen_flg[3];/* 上位机空调开关机控制指令 */
}mid_aircon_t;


/* 消防中间层  */
typedef struct{
    uint8_t FireContr_HardErr_flg;/* 消防硬件故障 */
    uint8_t FireContr_GetFire_flg;/* 消防火警故障 */
    uint8_t Send_FireCheckBack_flg;/* 消防发送回馈标志 */

}mid_firecon_t;

/* IO板卡中间层  */
typedef struct{
    uint8_t MIO_DoorValue_null[3];/* IO门禁1状态 */
    // uint8_t IO_DoorValue2_null;/* IO门禁2状态 */
    // uint8_t IO_DoorValue3_null;/* IO门禁3状态 */
    uint8_t MIO_ThunderValue_null[2];/* IO雷击1状态 */
    // uint8_t IO_ThunderValue2_null;/* IO雷击2状态 */
    uint8_t MIO_WaterValue_null;/* IO水禁状态 */
    uint8_t MIO_LightValue_null[2];/* IO光栅1状态 */
    // uint8_t IO_LightValue2_null;/* IO光栅2状态 */

    uint8_t Fan_Start_flg;/* 开启风扇标志 */
    uint8_t Sound_Start_flg;/* 开启声音报警 */
    uint8_t Light_Start_flg;/* 开启光报警 */
    uint8_t Door_Start_flg;/* 开启门1标志 */
    uint8_t Door2_Start_flg;/* 开启门2标志 */

    uint8_t Fan_StartLast_flg;/* 开启风扇上一时刻标志 */
    uint8_t Sound_StartLast_flg;/* 开启声音上一时刻报警 */
    uint8_t Light_StartLast_flg;/* 开启光上一时刻报警 */
    uint8_t Door_StartLast_flg;/* 开启门1上一时刻标志 */
    uint8_t Door2_StartLast_flg;/* 开启门2上一时刻标志 */

}mid_iocard_t;


/* 换电设备中间层  */
typedef struct{
    uint8_t CE_InitBack_flg;
    uint8_t CE_InitBack_err;
    uint8_t CE_ChangBatBack_flg;
    uint8_t CE_ChangBatBack_err;
    uint8_t CE_ChangLocatorBack_flg;
    uint8_t CE_ChangLocatorBack_err;
    uint8_t CE_BatOutBack_flg;
    uint8_t CE_BatOutBack_err;
    uint8_t CE_BatInLocatorBack_flg;
    uint8_t CE_BatInLocatorBack_err;
    uint8_t CE_BatOutLocatorBack_flg;
    uint8_t CE_BatOutLocatorBack_err;
    uint8_t CE_DismountBatBack_flg;
    uint8_t CE_DismountBatBack_err;
    uint8_t CE_MountBatBack_flg;
    uint8_t CE_MountBatBack_err;
    uint8_t CE_RGVOutChangPlatBack_flg;
    uint8_t CE_RGVOutChangPlatBack_err;
    uint8_t CE_TimeOutBack_flg;
    uint8_t CE_TimeOutBack_err;
    uint8_t CE_ContinueBack_flg;
    uint8_t CE_ContinueBack_err;
    uint8_t CE_StopBack_flg;
    uint8_t CE_StopBack_err;
    uint8_t CE_ErrResetBack_flg;
    uint8_t CE_ErrResetBack_err;
    uint8_t CE_RunState;
    uint8_t CE_ControlMode;
    uint8_t CE_ErrData[2];
    uint8_t CE_CarTypeNow;  	  //当前车型
    uint8_t CE_RGVStepData;  	  //RGV工步信息
    uint8_t CE_StackerStepData;   //码垛机工步信息
    uint8_t CE_ParkPlatStepData;  //泊车平台工步信息
    uint8_t CE_RotaTroSta;   	  //旋杆状态
    uint8_t CE_GetCBOk_flg;		  //换电完成
    uint8_t CE_GetDismountBatOk_flg;    //拆卸完成
    uint8_t CE_GetMountBatOk_flg;       //安装完成
    uint8_t CE_GetInLocatorOk_flg;   //入库完成
    uint8_t CE_GetOutLocatorOk_flg;  //出库完成
    uint8_t CE_GetCELocatorOk_flg;  //更换库位完成-source
    uint8_t CE_CELocatorOk_sour;
    uint8_t CE_CELocatorOk_des;   //更换库位完成-destination
    uint8_t CE_BatToLocatorOk_flg;//电池放至库位完成
    uint8_t CE_LocatorSenser[21]; //库位传感20+1
    uint8_t CE_RGVSenser_sta;     //RGV传感器状态
    uint8_t CE_StackerSenser_sta; //码垛机传感器状态
    uint8_t CE_TurnOverSenser_sta;//周转仓传感器状态
    uint8_t CE_SliOutSenser_sta;  //滑出位传感器状态
    uint8_t CE_UpDownDoor_sta;    //升降门状态
    uint8_t CE_ElecLinear_sta[20];    //电动推杆状态
    uint8_t CE_ParkPlatSenser_sta;    //泊车平台传感器状态

    uint8_t MCE_CBLocator_sour;
    uint8_t MCE_CBLocator_CarTy;
    uint8_t MCE_InLocator_dest;
    uint8_t MCE_InLocator_CarTy;
    uint8_t MCE_OutLocator_sour;
    uint8_t MCE_OutLocator_CarTy;
    uint8_t MCE_CELocator_sour;
    uint8_t MCE_CELocator_dest;
    uint8_t MCE_CELocator_CarTy;
    uint8_t MCE_DismountBat_CarTy;
    uint8_t MCE_MountBat_CarTy;
    uint8_t MCE_BatOut_sour;
    uint8_t MCE_BatOut_CarTy;
    uint8_t MCE_TimeOut_sour;
    uint8_t MCE_TimeOut_dest;
    uint8_t MCE_Continue_sour;
    uint8_t MCE_Continue_dest;
    uint8_t MCE_Stop_sour;
    uint8_t MCE_Stop_dest;
    uint8_t MCE_ErrReset_sour;
    uint8_t MCE_ErrReset_dest;

}mid_PLC_t;

/* 交流电表中间层  */
typedef struct{
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

    uint8_t ac_SetPtParameter;
    uint8_t ac_SetCtParameter;

}mid_AcEnger_t;


/* 语音屏中间层  */
typedef struct{
    uint8_t LCD_ShowFree_flg;/* 显示空闲 */
    uint8_t MLed_PlayIn_flg;/* 欢迎xxx进入 */
    uint8_t LCD_ShowInCEProcess_flg;/* 换电过程中 */
    uint8_t MLed_PlayOut_flg;/* 换电结束* */
    uint8_t MLed_PlayCancelOrder_flg;/* 取消订单成功 */
    uint8_t MLed_PlayNoBag_flg;/* 无可更换电池包 */
    uint8_t MLed_PlayCloudNoIn_flg;/* 云端禁止进入 */
    unsigned char Cloud_Grab_carNo[30];

}mid_LCD_t;

/* 点发云端中间层  */
typedef struct{

    uint8_t MCloud_SendCarCard_flg;/* 显示空闲 */
    uint8_t MCloud_SendCarOutOf_flg;
    uint8_t MCloud_SendCarODO_flg;
    uint8_t MCloud_SendCancelOrd_flg;
    uint8_t MCloud_SendGetCarType_flg;
    uint8_t mdl_send_AvailableNum_flg;
    uint8_t MCloud_CESta_sta;
    uint8_t MCloud_SendmatchBatNo_flg;

    uint8_t MCloud_CarAllowIn_sta;/* 显示空闲 */
    uint8_t Cloud_carType_Code;
    unsigned char Cloud_carType_now[50];/* 换电结束 */
    unsigned char Cloud_orderId_now[50];/* 云平台得到当前订单号 */
    unsigned char Cloud_BatteryNo_now[100];/* 云平台得到电池编码 */
    unsigned char Cloud_OutBatteryNo_now[100];/* 云端禁止进入 */
    unsigned char Cloud_Grab_carNo[30];/*抓拍机得到的车牌信息*/
    unsigned char Cloud_Grab_carNo2[30];
    unsigned char Cloud_Grab_carNo_cancel[30];/*取消订单用到的车牌信息*/
    unsigned char Cloud_orderId_now_cancel[50] ;
    unsigned char Staion_to_net_id[100];
    unsigned char Staion_to_net_pwd[100];
    unsigned char Staion_to_net_firmId[100];
    uint8_t MLocat_BatteryNo[20][100];
    uint8_t MLocat_InNet_flg[20];

    double Cloud_SendODO_km;

    uint8_t MCloud_CarOutReqODO_flg;
    uint8_t MCloud_CarAllowOut_sta;
    uint8_t MCloud_GetCarODO_flg;
    uint8_t MCloud_CancelOrd_ok;
    uint8_t MCloud_GetCarType_flg;

}mid_netburst_t;

/* 周期发云端中间层  */
typedef struct{

    uint8_t MCloud_Comm_flg;/* 显示空闲 */
    uint8_t Cloud_Comm_err_count;

    uint8_t MCloud_SendCarODO_flg;
    uint8_t MCloud_SendCancelOrd_flg;

    unsigned char Staion_to_net_id[100];
    unsigned char Staion_to_net_pwd[100];
    unsigned char Staion_to_net_firmId[100];
    uint8_t MLocat_BatteryNo[20][100];

}mid_netcycle_t;

/* 充电机中间层  */
typedef struct{

    uint8_t EMS_CHU_LinkState[20];
    uint8_t CHU_DevState_code[20];
    uint8_t CHU_BMS_LinkState[20];
    uint8_t CHU_BattType[20];/* 显示空闲 */
    uint8_t CHU_ErrCode_buf0[20];
    uint8_t CHU_ErrCode_buf1[20];
    uint8_t CHU_ErrCode_buf2[20];
    uint8_t CHU_ErrCode_buf3[20];
    uint8_t CHU_ErrCode_buf4[20];

    uint8_t Get_CHUChgRtData_flg[20];
    double BMS_PackSocReal_pct[20];
    double BMS_CharPackVolReal_V[20];
    double BMS_CharPackCurReal_A[20];
    double BMS_ReqPackVol_V[20];
    double BMS_ReqPackCur_A[20];
    // uint8_t CHU_ChgMode[20];
    double BMS_RemainCharTimeReal_min[20];
    double ChU_AllChrTimeReal_min[20];
    double Charger_OutPwr_W[20];
    uint8_t FCH_BigState_Null[20];
    //0x88
    double CHU_AllOutEnergy_kwh[20];
    uint8_t Get_StopCmdRes_flg[20];
    //0x89
    uint8_t Get_CHUErrStop_flg[20];
    uint8_t CHUErrStop_code[20];
    uint8_T MChg_ErrStop_code[20];

}mid_charger_t;
mid_charger_t mid_charger;


/* 数据库中间层  */
typedef struct{

    uint8_T MSql_WriCeStartData_flg;/* 显示空闲 */
    uint8_T MSql_WriCeOkData_flg;
    uint8_T MSqlite_WriBattData_flg[20];
    uint8_T MSqlite_WriCharData_flg[20];
    uint8_T MSqlite_WriConfigData_flg;
    uint8_T MSqlite_WriAirConData_flg;
    uint8_T MSqlite_WriLocatData_flg[20];
    uint8_T MSql_WriUnfinishedOrder_flg;
    uint8_T MSql_DelUnfinishedOrder_flg;
    uint8_T MCE_WriSqlShutDown_sta;

    uint8_T chu_test_state[20];
    uint8_T CHU_GetBatBasic_flg[20];
    unsigned char Cloud_carType_now[50];
    uint8_T MChar_SelfCe_sour;
    
    uint8_t MLocat_BatteryNo[20][100];
    unsigned char Cloud_BatteryNo_now[100];
    unsigned char Cloud_Grab_carNo[30];

    uint8_T MLocat_CharEndSoc_pct;
    uint8_T MLocat_AllowCESoc_pct;
    uint8_T MLocat_SelfChar_ena[20];
    uint8_T MLocat_CharPow_kw;

    int AirCon_Temp[3];
    int AirCon_Humi[3];

    uint8_T MLocat_BookLock_flg[20];
    uint8_t Cloud_BookcarNo1[20][100];
    uint8_t Cloud_BookcarNo2[20][100];
    uint8_t Cloud_BookcarNo3[20][100];

}mid_sqlite_t;

/* Qt通信中间层  */
typedef struct{

    uint8_t QtEMS_ShutDown_flg;/* 显示空闲 */
    uint8_T QT_InitOrder_flg;
    uint8_T QT_ChangeBatOrder_flg;
    uint8_T MCE_CBLocator_sour;
    uint8_T MCE_CBLocator_CarTy;
    uint8_T QT_CELocatorOrder_flg;
    uint8_T MCE_CELocator_sour;
    uint8_T MCE_CELocator_dest;
    uint8_T MCE_CELocator_CarTy;
    uint8_T QT_BatOutOrder_flg;
    uint8_T MCE_BatOut_sour;
    uint8_T MCE_BatOut_CarTy;
    uint8_T QT_InLocatorOrder_flg;
    uint8_T MCE_InLocator_dest;
    uint8_T MCE_InLocator_CarTy;
    uint8_T QT_OutLocatorOrder_flg;
    uint8_T MCE_OutLocator_sour;
    uint8_T MCE_OutLocator_CarTy;
    uint8_T QT_TimeOutOrder_flg;
    uint8_T QT_ContinueOrder_flg;
    uint8_T QT_StopOrder_flg;
    uint8_T QT_ErrResetOrder_flg;
    uint8_T MQt_StartSelfCE_sta; //no
    
    uint8_T Hand_LowPwrOn_flg[20];
    uint8_T Hand_StartChg_flg[20];
    uint8_T Hand_StopChg_flg[20];
    uint8_T Hand_LowPwrDown_flg[20];
    uint8_t MQt_ConfDa_push;

    uint8_T MLocat_AllowCESoc_pct;
    uint8_T MLocat_CharEndSoc_pct;
    uint8_T MLocat_CharPow_kw;
    uint8_T MLocat_SelfChar_ena[20];
    uint8_T MLocat_Conf_ena[20];//no
    uint8_T MLocat_ErrReset_flg[20];//no
    
    uint8_T Grab_GetCarCard_flg;
    uint8_T MQt_DisaSoundLigErr_flg;
    uint8_t Sound_Start_flg; //no
    uint8_t Light_Start_flg; //no
    uint8_T MQt_GetInPlace_flg;
    uint8_T MQt_SendInPlace_flg;//no
    uint8_T MQt_GetHandCBOk_flg;
    uint8_T MQt_GetCancelOrd_flg;
    uint8_T MQt_GetPointBack_flg;
    real_T Cloud_SendODO_km;
    uint8_T MQt_GetODOValue_flg;
    uint8_T MCloud_CarOutReqODO_flg;//no
    uint8_t Qt_AirSetData_flg[3];
    int16_t AirSet_EnginStartTemp_C;
    int16_t AirSet_HertStartTemp_C;
    uint8_t Qt_AirOpen_flg[3];

    uint8_t QtBook_BatNax_num[3][50];//no
    unsigned char mdl_send_AvailableNum_flg;
    
    /***no*/
    uint8_T MCE_GetSqlInt_sta;
    unsigned char Cloud_Grab_carNo[30];
    uint8_t Cloud_carType_Code;
    unsigned char Cloud_OutBatteryNo_now[100];
    unsigned char Cloud_orderId_now[50];
    uint8_T MChar_SelfCe_sour;


}mid_qtTcp_t;

// #endif