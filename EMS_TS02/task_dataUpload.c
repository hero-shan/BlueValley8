
#include "stdio.h"
#include "unistd.h"
#include "sys/time.h"
#include "time.h"
#include <curl/curl.h>
#include "string.h"
#include "stdlib.h"

#include "cloud_platform.h"
#include "mymain.h"

#include <resolv.h>
#include "task_dataUpload.h"
#include "errMsg.h"

uint8_t stationRtDataTemp[20000];
uint8_t Cloud_Send10s_count = 0;

uint8_t Cloud_ChangePwrCmplt_flg=0;
uint8_t Cloud_SysErr_flg = 0;
uint8_t Cloud_RtData_flg = 0;

net_StationRtData_t StationRtData;
unsigned char Mnet_WriCeStartData_flg;
unsigned char Mnet_WriCeOkData_flg;

extern char NowTime[100];
char startChgTime[20][30];

extern int CE_Err_Code;
extern uint8_t ChgerDev_Err_Sta[];
char *url_sendData = "https://tswitcher.bj-abt.com/switcher-console/ems/sendData.htm";

/* 回调函数， 一定要遵循huidiao函数的格式*/
static size_t httpCurlResCallback(void* buffer, size_t size, size_t nmemb, void* pUserArg)
{
    int i=0;
    size_t len = 0;len = size * nmemb;
 //   printf("%s; buffer = %s\n", __FILE__, (char *) buffer);
    
    cJSON *cJSON_buffer = NULL;
    unsigned char ParseResult_result[20];//返回结果

    cJSON_buffer = cJSON_Parse(buffer);
    if(NULL == cJSON_buffer)
        return;  
    if(cJSON_GetObjectItem(cJSON_buffer, "result")==NULL)
        return;
// 换电站运行实时数据采集地址
    strcpy(ParseResult_result ,cJSON_GetObjectItem(cJSON_buffer, "result")->valuestring);
 //   printf("result :%s\n",ParseResult_result);
    return len;
}


void pthread_dataUpload(void) 
{
 //   printf("pthread_dataUpload tid %d\n",gettid());
    syslog(LOG_INFO,"pthread_dataUpload gettid '%d'\n",gettid());

    char *url = url_sendData;
    while (1)
    {
        while(1)
        {        
            // printf("sasbbb %d\n",gettid());
            	// char * test;
            	// strcpy(test,0);

            sleep(1);
            // usleep(10*1000);
            stationRtData_ErrData_Interf(&StationRtData);
            stationRtData_ChangeBatt_Interf(&StationRtData);

            if(++Cloud_Send10s_count >= 10)
            {
                Cloud_RtData_flg = 1;
                Cloud_Send10s_count = 0;
            }
            if(Cloud_ChangePwrCmplt_flg || Cloud_RtData_flg || 0) //Cloud_SysErr_flg
            {
                build_station_rtdata(StationRtData);
                break;
            }
        }
        do{
            networkManger(url,(char *)stationRtDataTemp);
        }while(0);

    }
    return;
}

void stationRtData_ChangeBatt_Interf(net_StationRtData_t * StationRtData)
{
   /*换电站实时数据 换电*/
    if(Mnet_WriCeStartData_flg==1)
	{
        strcpy(CE_StartTime,NowTime);
		Mnet_WriCeStartData_flg = 0;
        strcpy(StationRtData->ChangeData.ChangeDate,CE_StartTime);//车辆到站时间
        strcpy(StationRtData->ChangeData.CarNo , Cloud_Grab_carNo);/*车辆车牌 */
		strcpy(StationRtData->ChangeData.NewBatteryNo,BMS_BATT_NUM[MChar_SelfCe_sour]);///*满电电池包编码*/
        strcpy(StationRtData->ChangeData.OldBatteryNo,Cloud_BatteryNo_now);//OldBatteryNo/*空电电池包编码 */
        StationRtData->ChangeData.NewSeat = MChar_SelfCe_sour+1;/*满电电池包原来的位置 */
        StationRtData->ChangeData.OldSeat = MChar_SelfCe_sour+1;/*空电电池包放置位置 */
		StationRtData->ChangeData.electricityUp = BMS_BATT_AVAIL_ENERGY[MChar_SelfCe_sour];//换上电池可用能量
		StationRtData->ChangeData.capacityUp = BMS_BATT_AVAIL_CAPACITY[MChar_SelfCe_sour];//换上电池可用容量
		StationRtData->ChangeData.socUp = BMS_BATT_SOC[MChar_SelfCe_sour];//换上SOC
	}
	if(Mnet_WriCeOkData_flg==1&&chu_test_state[MChar_SelfCe_sour_old]>=10&&CHU_GetBatBasic_flg[MChar_SelfCe_sour_old]==1)
	{
		Mnet_WriCeOkData_flg = 0;
		StationRtData->ChangeData.electricityDown = BMS_BATT_AVAIL_ENERGY[MChar_SelfCe_sour_old];//换下电池可用能量
		StationRtData->ChangeData.capacityDown = BMS_BATT_AVAIL_CAPACITY[MChar_SelfCe_sour_old];//换下电池可用容量
		StationRtData->ChangeData.socDown = BMS_BATT_SOC[MChar_SelfCe_sour_old];//换下SOC
        strcpy(StationRtData->ChangeData.logDatetime,NowTime);/* 记录时间 */
        if(1)
            StationRtData->ChangeData.ChangeStatus = 1;
        
        Cloud_ChangePwrCmplt_flg=1;
	}
}

void stationRtData_ErrData_Interf(net_StationRtData_t * StationRtData)
{
    int i=0;
    /* 换电站实时数据 换电站运行数据 */  
    StationRtData->RunData.Device1Sta = 0;/* 视频监控状态 0:正常 1:故障 */
    if((SelfChkErr_EMS_CeDev_ComErr == 0) && (CE_Err_Code ==0))
    {
        StationRtData->RunData.Device2Sta = 0;/* 码垛机状态 0:正常 1:故障 */
        StationRtData->RunData.Device3Sta = 0;/* RGV状态 0:正常 1:故障 */
    }
    else if((SelfChkErr_EMS_CeDev_ComErr == 0) && (CE_Err_Code !=0))
    {
        StationRtData->RunData.Device2Sta = 1;/* 码垛机状态 0:正常 1:故障 */
        StationRtData->RunData.Device3Sta = 1;/* RGV状态 0:正常 1:故障 */      
    }
    for(i=0;i<18;++i)
    {
        if(SelfChkErr_EMS_ChgerDev_ComErr[i] || ChgerDev_Err_Sta[i])
        {
            /* 充电机状态 0:正常 1:故障充电机全部正常时，为正常，
            只要有一个故障，就报故障。故障信息提现在Message里 */
            StationRtData->RunData.Device4Sta = 1;
            // printf("SelfChkErr_EMS_ChgerDev_ComErr[i]= %d %d\n",SelfChkErr_EMS_ChgerDev_ComErr[i],i);
            break;
        }
    }
    StationRtData->RunData.Device5Sta = 0;/* 电池包状态 0:正常 1:故障 */
    { 
    /* 消防设备状态 0:正常 1:故障 */
    StationRtData->RunData.Device6Sta = ((SelfChkErr_EMS_FireCon_ComErr == 1) 
                                        || FireContr_GetFire_flg == 1);
    }
    StationRtData->RunData.Device7Sta = 0;/* 交流电表状态 0:正常 1:故障 */
    //dev8
    {
        int tmp = AirCon_Err1[0] || AirCon_Err2[0] || AirCon_Err3[0]
              || AirCon_Err1[1] || AirCon_Err2[1] || AirCon_Err3[1]     
              || AirCon_Err1[2] || AirCon_Err2[2] || AirCon_Err3[2];
        /* 空调设备状态 0:正常 1:故障 */
        StationRtData->RunData.Device8Sta = ((AirCon_conn_flg == 2) || (tmp !=0) );
    }
    // StationRtData->RunData.Message = 0;/* 异常信息，如果多个设备异常，多个异常信息一起发送 */

    /* 异常等级   0:正常  1:警告  2:异常但能运行 3:严重异常不可运行  4火警 */
    if(FireContr_GetFire_flg == 1)
        {StationRtData->RunData.ErrLevel = 4;}
    else if(StationRtData->RunData.Device2Sta || StationRtData->RunData.Device3Sta)
        {StationRtData->RunData.ErrLevel = 3;}
    // else if(){}
    else 
    {StationRtData->RunData.ErrLevel = 0;}
    
    /* 整站状态  0:正常 1:故障 */
    StationRtData->RunData.StationStatus = StationRtData->RunData.Device1Sta || 
                                            StationRtData->RunData.Device2Sta ||
                                            StationRtData->RunData.Device3Sta ||
                                            StationRtData->RunData.Device4Sta ||
                                            StationRtData->RunData.Device5Sta ||
                                            StationRtData->RunData.Device6Sta ||
                                            StationRtData->RunData.Device7Sta || 
                                            StationRtData->RunData.Device8Sta;
    StationRtData->RunData.RunStatus = 0;/* 站的运行状态 1自动   2 手动 */
    strcpy(StationRtData->RunData.logDatetime,NowTime);/* 记录时间 */

    Cloud_SysErr_flg = StationRtData->RunData.StationStatus;

}

void stationRtData_Interf(net_StationRtData_t * StationRtData)
{
    int i=0;
    
    //充电实时数据//
    for(i=0;i<18;i++)
    {
        StationRtData->ChargeData[i].SeatNo = i+1;/*	电池支架位置编号	*/	
        strcpy (StationRtData->ChargeData[i].BatteryNo , BMS_BATT_NUM[i]);/*	电池包编号	*/	
        StationRtData->ChargeData[i].SOC= BMS_PackSocReal_pct[i];;/*	两位小数（如数据12表示为12%）	*/	
        memcpy(StationRtData->ChargeData[i].Voltage,BMS_CellVol_buf[i],128);/*	从BMS获取的单体电压信息(两位小数	38120表示为38.120)	*/
        memcpy(StationRtData->ChargeData[i].Temperature,BMS_CellTemp_buf[i],32);/*	从BMS获取的单体温度信息	*/	
        StationRtData->ChargeData[i].Current = BMS_CharPackCurReal_A[i]*100;/*	充电电流	*/	
        StationRtData->ChargeData[i].MaxVoltage = BMS_CELL_VOLT_MAX[i];	/*	单体电芯最高电压	*/
        StationRtData->ChargeData[i].MinVoltage = BMS_CELL_VOLT_MIN[i];	/*	单体电芯最低电压	*/
        StationRtData->ChargeData[i].MaxTemperature = BMS_CELL_TEMP_MAX[i];	/*	单体电芯最高温度	*/
        StationRtData->ChargeData[i].MinTemperature = BMS_CELL_TEMP_MIN[i];	/*	单体电芯最低温度	*/
        StationRtData->ChargeData[i].GNDA = BMS_BATT_POSITIVE_RESISTANCE[i];/*	动力电池正极对GND绝缘电阻	*/	
        StationRtData->ChargeData[i].GNDZ = BMS_BATT_NEGATIVE_RESISTANCE[i];/*	动力电池负极对GND绝缘电阻	*/	
        StationRtData->ChargeData[i].ZAS = BMS_CHARGE_TOT_AH[i];	/*	累计充电总安时	*/
        StationRtData->ChargeData[i].V1 = BMS_BATT_VOLT_V1[i];	/*	动力电池内部总电压V1	*/
        StationRtData->ChargeData[i].Instruct = 0;/*	直流充电端电流指令	*/	
        StationRtData->ChargeData[i].I1 = BMS_BATT_CUR[i];	/*	动力电池充/放电电流	*/
        StationRtData->ChargeData[i].OutI = 0;/*	充电机最大输出电流	*/	
        StationRtData->ChargeData[i].NL = BMS_BATT_AVAIL_ENERGY[i];	/*	动力电池可用能量	*/
        StationRtData->ChargeData[i].RL = BMS_BATT_AVAIL_CAPACITY[i];	/*	动力电池可用容量	*/
        StationRtData->ChargeData[i].BJ = BMS_PART_VERS[i];	/*	电池包总成零部件号	*/
        StationRtData->ChargeData[i].BMSZNo = BMS_SW_VERS_main[i];	/*	BMS主软件版本号	*/
        StationRtData->ChargeData[i].BMSCNo = BMS_SW_VERS[i];	/*	BMS次软件版本号	*/
        StationRtData->ChargeData[i].MinTNo = BMS_MIN_TEMP_NUM[i];	/*	最低温度编号	*/
        StationRtData->ChargeData[i].MaxTNo = BMS_MAX_TEMP_NUM[i];	/*	最高温度编号	*/
        StationRtData->ChargeData[i].MinVNo = BMS_MIN_VOLT_NUM[i];	/*	最低单体电压编号	*/
        StationRtData->ChargeData[i].MaxVNo =BMS_MAX_VOLT_NUM[i];	/*	最高单体电压编号	*/
        StationRtData->ChargeData[i].Error = 0;	/*	动力电池系统故障显示	*/
        StationRtData->ChargeData[i].BMSODO = BMS_ODO[i];	/* BMS里程 */
        strcpy(StationRtData->ChargeData[i].logDatetime,NowTime);/*	记录时间 */	
    }

    /*库位信息*/
    for ( i = 0; i < 18; i++)
    {
        StationRtData->SeatInfoData[i].seatNo = i+1;/* 库位信息（此字段为必要信息） */
        StationRtData->SeatInfoData[i].currentSoc = BMS_PackSocReal_pct[i];/* 电池包当前soc */
        StationRtData->SeatInfoData[i].endSoc = 0;/* 库位包截止充电时soc */
        strcpy(StationRtData->SeatInfoData[i].batteryNo,BMS_BATT_NUM[i]);/* 电池包溯源码 */
        if(BATT_IsExist_flg[i]==0)
            StationRtData->SeatInfoData[i].chargeState = 0;/* 充电状态0-无电池，1-待充电，2-充电中，3-充电完成，4-有故障 */
        else
        {
            if(BMS_BATT_SOC[i] >=95)
                StationRtData->SeatInfoData[i].chargeState = 3;
            else if(CHU_DevState_code[i]>=3)
                StationRtData->SeatInfoData[i].chargeState = 2;
            else
                StationRtData->SeatInfoData[i].chargeState = 1;
        }
        
        strcpy(StationRtData->SeatInfoData[i].logDatetime,NowTime);/* 2019-11-21 17:53:30	记录时间 */
        strcpy(StationRtData->SeatInfoData[i].strategyChargeStartTime,startChgTime[i]);/* 2019-11-21 17:53:30	库位充电时间 */
        
        StationRtData->SeatInfoData[i].strategyIsValid = 1;/* 有效状态状态，0-失效，1-有效，无效则按照放到到库位则充电 */
    }

     /* 换电站实时数据 直流电表 */
    StationRtData->DcMeterData.ammeterNo = 0;/* 电流表编号 */
    StationRtData->DcMeterData.ammeterState = 0;/* 开关量0--未连接  1--连接  2--通信故障 */
    StationRtData->DcMeterData.ammeterVoltage = 0;/* 电压（V） */
    StationRtData->DcMeterData.ammeterCurrent = 0;/* 电流（A） */
    StationRtData->DcMeterData.ammeterPower = 0;/* 有功功率（kwh） */
    StationRtData->DcMeterData.ammeterPositivePower = 0;/* 正向有功功率 */
    StationRtData->DcMeterData.ammeteNegativePower = 0;/* 逆向有功功率 */
    StationRtData->DcMeterData.ammeterDcKwh = 0;/* 电表直流耗电量（kwh） */

    /* 换电站实时数据 交流电表 */ 
    StationRtData->AcMeterData.acAmmeterSatate	=0;
    StationRtData->AcMeterData.acGroupActiveTotalEnergy	=1;
    StationRtData->AcMeterData.acGroupActivePointedEnergy	=2;
    StationRtData->AcMeterData.acGroupActivePeekEnergy	=3;
    StationRtData->AcMeterData.acGroupActiveFlatEnergy	=4;
    StationRtData->AcMeterData.acGroupActiveValleyEnergy	=5;
    StationRtData->AcMeterData.acPositiveActiveTotalEnergy	=6;
    StationRtData->AcMeterData.acPositiveActivePointedEnergy	=7;
    StationRtData->AcMeterData.acPositiveActivePeekEnergy	=8;
    StationRtData->AcMeterData.acPositiveActiveFlatEnergy	=9;
    StationRtData->AcMeterData.acPositiveActiveValleyEnergy	=10;
    StationRtData->AcMeterData.acNegativeActiveTotalEnergy	=11;
    StationRtData->AcMeterData.acNegativeActivePointedEnergy	=12;
    StationRtData->AcMeterData.acNegativeActivePeekEnergy	=13;
    StationRtData->AcMeterData.acNegativeActiveFlatEnergy	=14;
    StationRtData->AcMeterData.acNegativeActiveValleyEnergy	=15;
    StationRtData->AcMeterData.acGroupReactiveTotalEnergy	=16;
    StationRtData->AcMeterData.acGroupReactivePointedEnergy	=17;
    StationRtData->AcMeterData.acGroupReactivePeekEnergy	=18;
    StationRtData->AcMeterData.acGroupReactiveFlatEnergy	=19;
    StationRtData->AcMeterData.acGroupReactiveValleyEnergy	=20;
    StationRtData->AcMeterData.acPositiveReactiveTotalEnergy	=21;
    StationRtData->AcMeterData.acPositiveReactivePointedEnergy	=22;
    StationRtData->AcMeterData.acPositiveReactivePeekEnergy	=23;
    StationRtData->AcMeterData.acPositiveReactiveFlatEnergy	=24;
    StationRtData->AcMeterData.acPositiveReactiveValleyEnergy	=25;
    StationRtData->AcMeterData.acNegativeReactiveTotalEnergy	=26;
    StationRtData->AcMeterData.acNegativeReactivePointedEnergy	=27;
    StationRtData->AcMeterData.acNegativeReactivePeekEnergy	=28;
    StationRtData->AcMeterData.acNegativeReactiveFlatEnergy	=29;
    StationRtData->AcMeterData.acNegativeReactiveValleyEnergy	=30;
    StationRtData->AcMeterData.acAVoltage	=31;
    StationRtData->AcMeterData.acBVoltage	=32;
    StationRtData->AcMeterData.acCVoltage	=33;
    StationRtData->AcMeterData.acACurrent	=34;
    StationRtData->AcMeterData.acBurrent	=35;
    StationRtData->AcMeterData.acCurrent	=36;
    StationRtData->AcMeterData.acAActivePower	=37;
    StationRtData->AcMeterData.acBctivePower	=38;
    StationRtData->AcMeterData.acCctivePower	=39;
    StationRtData->AcMeterData.acTotalActivePower	=40;
    StationRtData->AcMeterData.acAReactivePower	=41;
    StationRtData->AcMeterData.acBReactivePower	=42;
    StationRtData->AcMeterData.acCReactivePower	=43;
    StationRtData->AcMeterData.acTotalReactivePower	=44;
    StationRtData->AcMeterData.acALookPower	=45;
    StationRtData->AcMeterData.acBLookPower	=46;
    StationRtData->AcMeterData.acCLookPower	=47;
    StationRtData->AcMeterData.acTotalLookPower	=48;
    StationRtData->AcMeterData.acAPhasePowerFactor	=49;
    StationRtData->AcMeterData.acBPhasePowerFactor	=50;
    StationRtData->AcMeterData.acCPhasePowerFactor	=51;
    StationRtData->AcMeterData.acTotalPowerFactor	=52;
    StationRtData->AcMeterData.acCt	=53;
    StationRtData->AcMeterData.acPt	=54;
    StationRtData->AcMeterData.acZroeCurrent	=55;
    StationRtData->AcMeterData.acVoltageUneven	=56;
    StationRtData->AcMeterData.acCurrentUneven	=57;
}

void build_station_rtdata()
{
    cJSON *data_data = cJSON_CreateObject();
    char *pStr;

    /* 换电的数据 */
    if(Cloud_ChangePwrCmplt_flg == 1)
    {
        Cloud_ChangePwrCmplt_flg = 0;
        /* 换电的数据 */
        cJSON *ChangeData_obj = ChangeDataObjBuild();
        cJSON_AddItemToObject(data_data, "changeData", ChangeData_obj);

        Cloud_RtData_flg = 1;
    }
    
    /* 当前站内电池的充电数据 */
    if(Cloud_RtData_flg == 1||Cloud_SysErr_flg == 1)
    {   
        // Cloud_RtData_flg = 0;
        // Cloud_SysErr_flg = 0
        stationRtData_Interf(&StationRtData);
        // stationRtData_ErrData_Interf(&StationRtData);
        /* 当前站内电池的充电数据 */
        cJSON *ChargeData = ChargeDataObjBuild();
        cJSON_AddItemToObject(data_data, "chargeData", ChargeData);
        /* 换电站实时数据 库位信息 */
        cJSON *SeatInfoData = SeatInfoDataBuild();
        cJSON_AddItemToObject(data_data, "seatInfoData", SeatInfoData); 
        /* 换电站实时数据 直流电表 */
        cJSON *DcMeterData = DcMeterDataBuild();
        cJSON_AddItemToObject(data_data, "dcMeterData", DcMeterData);
        /* 换电站实时数据 交流电表 */
        // cJSON *AcMeterData = AcMeterDataBuild();
        // cJSON_AddItemToObject(data_data, "AcMeterData", AcMeterData);

         /* 换电站运行数据 */
        cJSON *RunData_obj  = RunDataObjBuild();
        cJSON_AddItemToObject(data_data, "runData", RunData_obj);
        Cloud_RtData_flg = 0;
        Cloud_SysErr_flg = 0;
    }

    /*cJSON跟目录*/
    cJSON *root = cJSON_CreateObject();//id pwd firmId status  
    cJSON_AddStringToObject(root, "id", Staion_to_net_id);
    cJSON_AddStringToObject(root, "pwd", Staion_to_net_pwd);
    cJSON_AddStringToObject(root, "firmId", Staion_to_net_firmId);    
    cJSON_AddStringToObject(root, "status", "0");//status need be replace
    cJSON_AddItemToObject(root, "data", data_data);

    pStr = cJSON_Print(root);
    cJSON_Delete(root);
 //   printf("len of pStr = %ld",strlen(pStr));
    strcpy(stationRtDataTemp,pStr);free(pStr);
 //   printf("json is:\r\n%s\r\n", stationRtDataTemp);
}

cJSON *ChangeDataObjBuild()
{
    cJSON * ChangeData_obj;
    ChangeData_obj = cJSON_CreateObject();
        // printf("ssss111\n");
    Cloud_ChangePwrCmplt_flg = 0;
    uint8_t temp[200];
    sprintf(temp,"%d",StationRtData.ChangeData.ChangeStatus);
    cJSON_AddStringToObject(ChangeData_obj,"ChangeStatus",temp);
    cJSON_AddStringToObject(ChangeData_obj,"CarNo",StationRtData.ChangeData.CarNo);
    cJSON_AddStringToObject(ChangeData_obj,"NewBatteryNo",StationRtData.ChangeData.NewBatteryNo);//满电电池包编码
    cJSON_AddStringToObject(ChangeData_obj,"OldBatteryNo",StationRtData.ChangeData.OldBatteryNo);//空电电池包编码
    sprintf(temp,"%d",StationRtData.ChangeData.NewSeat);
    cJSON_AddStringToObject(ChangeData_obj,"NewSeat",temp);//满电电池包原来的位置
    sprintf(temp,"%d",StationRtData.ChangeData.OldSeat);
    cJSON_AddStringToObject(ChangeData_obj,"OldSeat",temp);//空电电池包放置位置
    cJSON_AddStringToObject(ChangeData_obj,"ChangeDate",StationRtData.ChangeData.ChangeDate);
    sprintf(temp,"%d",StationRtData.ChangeData.electricityDown);
    cJSON_AddStringToObject(ChangeData_obj,"electricityDown",temp);
    sprintf(temp,"%d",StationRtData.ChangeData.electricityUp);
    cJSON_AddStringToObject(ChangeData_obj,"electricityUp",temp);
    sprintf(temp,"%d",StationRtData.ChangeData.capacityDown);
    cJSON_AddStringToObject(ChangeData_obj,"capacityDown",temp);
    sprintf(temp,"%d",StationRtData.ChangeData.capacityUp);
    cJSON_AddStringToObject(ChangeData_obj,"capacityUp",temp);
    sprintf(temp,"%d",StationRtData.ChangeData.socUp);
    cJSON_AddStringToObject(ChangeData_obj,"socUp",temp);
    sprintf(temp,"%d",StationRtData.ChangeData.socDown);
    cJSON_AddStringToObject(ChangeData_obj,"socDown",temp);
    cJSON_AddStringToObject(ChangeData_obj,"logDatetime",StationRtData.ChangeData.logDatetime);

    return ChangeData_obj;
}

cJSON *RunDataObjBuild()
{
    cJSON *RunData_obj;
    RunData_obj = cJSON_CreateObject();

    cJSON_AddStringToObject(RunData_obj,"status",StationRtData.RunData.StationStatus?"1":"0");
    cJSON_AddStringToObject(RunData_obj,"device1",StationRtData.RunData.Device1Sta?"1":"0");   
    cJSON_AddStringToObject(RunData_obj,"device2",StationRtData.RunData.Device2Sta?"1":"0");  
    cJSON_AddStringToObject(RunData_obj,"device3",StationRtData.RunData.Device3Sta?"1":"0");  
    cJSON_AddStringToObject(RunData_obj,"device4",StationRtData.RunData.Device4Sta?"1":"0");  
    cJSON_AddStringToObject(RunData_obj,"device5",StationRtData.RunData.Device5Sta?"1":"0");
    cJSON_AddStringToObject(RunData_obj,"device6",StationRtData.RunData.Device6Sta?"1":"0");  
    cJSON_AddStringToObject(RunData_obj,"device7",StationRtData.RunData.Device7Sta?"1":"0");  
    cJSON_AddStringToObject(RunData_obj,"device8",StationRtData.RunData.Device8Sta?"1":"0");   

    cJSON_AddStringToObject(RunData_obj,"runStatus",StationRtData.RunData.RunStatus?"1":"0");
    if(Cloud_SysErr_flg == 1)
    {
        cJSON *message_obj = ErrMsgBuild();
        uint8_t temp[200];
        sprintf(temp,"%d",StationRtData.RunData.ErrLevel);
        cJSON_AddStringToObject(RunData_obj,"errLevel",temp);
        
        cJSON_AddItemToObject(RunData_obj,"message",message_obj);
    }
    else
    {  
        cJSON *message_obj = cJSON_CreateArray();
        cJSON_AddStringToObject(RunData_obj,"errLevel","0");
        cJSON_AddItemToObject(RunData_obj,"message",message_obj);
    }
    return RunData_obj;

}

cJSON *ChargeDataObjBuild()
{
    /* 当前站内电池的充电数据 */
    cJSON *Array_ChargeData = cJSON_CreateArray();
    int i;

    stationRtData_Interf(&StationRtData);
    for(i=0;i<18;i++)
    {
        if(!(BATT_IsExist_flg[i]==1&&(CHU_DevState_code[i]>=3)))
            continue;

        cJSON *ChargeData_obj = cJSON_CreateObject();
        uint8_t temp[200];
        cJSON_AddNumberToObject(ChargeData_obj,"seatNo",StationRtData.ChargeData[i].SeatNo);
        cJSON_AddStringToObject(ChargeData_obj,"batteryNo",StationRtData.ChargeData[i].BatteryNo); 
        cJSON_AddNumberToObject(ChargeData_obj,"soc",StationRtData.ChargeData[i].SOC);
        cJSON_AddNumberToObject(ChargeData_obj,"voltage",(double)(StationRtData.ChargeData[i].Voltage[0]*0.02)*1000); 
        cJSON_AddNumberToObject(ChargeData_obj,"temperature",(double)(StationRtData.ChargeData[i].Temperature[0]-48));
        cJSON_AddNumberToObject(ChargeData_obj,"current",StationRtData.ChargeData[i].Current); 
        cJSON_AddNumberToObject(ChargeData_obj,"gnda",StationRtData.ChargeData[i].GNDA);
        cJSON_AddNumberToObject(ChargeData_obj,"gndz",StationRtData.ChargeData[i].GNDZ);
        cJSON_AddNumberToObject(ChargeData_obj,"minTemperature",StationRtData.ChargeData[i].MinTemperature);
        cJSON_AddNumberToObject(ChargeData_obj,"maxTemperature",StationRtData.ChargeData[i].MaxTemperature);//单体电芯最高温度 
        cJSON_AddNumberToObject(ChargeData_obj,"minVoltage",StationRtData.ChargeData[i].MinVoltage);
        cJSON_AddNumberToObject(ChargeData_obj,"maxVoltage",StationRtData.ChargeData[i].MaxVoltage); 
        cJSON_AddNumberToObject(ChargeData_obj,"nl",StationRtData.ChargeData[i].NL);//动力电池可用能量
        cJSON_AddStringToObject(ChargeData_obj,"outi","");//充电机最大输出电流     
        cJSON_AddStringToObject(ChargeData_obj,"i",""); 
        cJSON_AddStringToObject(ChargeData_obj,"instruct","");//直流充电端电流指令
        cJSON_AddStringToObject(ChargeData_obj,"v",""); 
        cJSON_AddNumberToObject(ChargeData_obj,"zas",StationRtData.ChargeData[i].ZAS);//累计充电总安时
        cJSON_AddNumberToObject(ChargeData_obj,"maxtNo",StationRtData.ChargeData[i].MaxTNo); 
        cJSON_AddNumberToObject(ChargeData_obj,"mintNo",StationRtData.ChargeData[i].MinTNo); 
        cJSON_AddNumberToObject(ChargeData_obj,"bmscNo",StationRtData.ChargeData[i].BMSCNo);//BMS次软件版本号
        cJSON_AddNumberToObject(ChargeData_obj,"bmszNo",StationRtData.ChargeData[i].BMSZNo);//BMS主软件版本号 
        cJSON_AddNumberToObject(ChargeData_obj,"bj",StationRtData.ChargeData[i].BJ);//电池包总成零部件号
        cJSON_AddNumberToObject(ChargeData_obj,"rl",StationRtData.ChargeData[i].RL);//动力电池可用容量 
        cJSON_AddStringToObject(ChargeData_obj,"bmsodo",""); 
        cJSON_AddStringToObject(ChargeData_obj,"error","");
        cJSON_AddNumberToObject(ChargeData_obj,"maxvNo",StationRtData.ChargeData[i].MaxVNo);//最高单体电压编号 
        cJSON_AddNumberToObject(ChargeData_obj,"minvNo",StationRtData.ChargeData[i].MinVNo);//最低单体电压编号

        cJSON_AddItemToArray(Array_ChargeData, ChargeData_obj);
    }
    return Array_ChargeData;

}

cJSON *SeatInfoDataBuild()
{
    cJSON *Array_SeatInfoData = cJSON_CreateArray();
    int i;
    for(i=0;i<18;i++)
    {   
        if(!(BATT_IsExist_flg[i]==1&&(CHU_DevState_code[i]>=3)))
            continue;

        cJSON *SeatInfoData_obj = cJSON_CreateObject();
        cJSON_AddNumberToObject(SeatInfoData_obj,"seatNo",StationRtData.SeatInfoData[i].seatNo);/* 库位信息（此字段为必要信息） */
        cJSON_AddNumberToObject(SeatInfoData_obj,"currentSoc",StationRtData.SeatInfoData[i].currentSoc);/* 电池包当前soc */
        cJSON_AddNumberToObject(SeatInfoData_obj,"endSoc",StationRtData.SeatInfoData[i].endSoc);/* 库位包截止充电时soc */
        cJSON_AddStringToObject(SeatInfoData_obj,"batteryNo",StationRtData.SeatInfoData[i].batteryNo);/* 电池包溯源码 */
        cJSON_AddNumberToObject(SeatInfoData_obj,"chargeState",StationRtData.SeatInfoData[i].chargeState);/* 充电状态0-无电池，1-待充电，2-充电中，3-充电完成，4-有故障 */
        cJSON_AddStringToObject(SeatInfoData_obj,"logDatetime",StationRtData.SeatInfoData[i].logDatetime);/* 2019-11-21 17:53:30	记录时间 */
        cJSON_AddStringToObject(SeatInfoData_obj,"strategyChargeStartTime",StationRtData.SeatInfoData[i].strategyChargeStartTime);/* 2019-11-21 17:53:30	库位充电时间 */
        cJSON_AddNumberToObject(SeatInfoData_obj,"strategyIsValid",StationRtData.SeatInfoData[i].strategyIsValid);/* 有效状态状态，0-失效，1-有效，无效则按照放到到库位则充电 */
        cJSON_AddItemToArray(Array_SeatInfoData, SeatInfoData_obj);
    }
    return Array_SeatInfoData;
}

cJSON *DcMeterDataBuild()
{
    char temp[200];
    cJSON * DC_data = cJSON_CreateObject();

    sprintf(temp,"%d",StationRtData.DcMeterData.ammeterNo);
    cJSON_AddStringToObject(DC_data,"ammeterNo",temp);
    sprintf(temp,"%d",StationRtData.DcMeterData.ammeterState);
    cJSON_AddStringToObject(DC_data,"ammeterState",temp);
    sprintf(temp,"%.3lf", StationRtData.DcMeterData.ammeterVoltage);
    cJSON_AddStringToObject(DC_data,"ammeterVoltage",temp);
    sprintf(temp,"%.3lf",StationRtData.DcMeterData.ammeterCurrent);
    cJSON_AddStringToObject(DC_data,"ammeterCurrent",temp);
    sprintf(temp,"%.3lf",StationRtData.DcMeterData.ammeterPower);
    cJSON_AddStringToObject(DC_data,"ammeterPower",temp);
    sprintf(temp,"%.3lf",StationRtData.DcMeterData.ammeterPositivePower);
    cJSON_AddStringToObject(DC_data,"ammeterPositivePower",temp);
    sprintf(temp,"%.3lf",StationRtData.DcMeterData.ammeteNegativePower);
    cJSON_AddStringToObject(DC_data,"ammeteNegativePower",temp);
    sprintf(temp,"%.3lf", StationRtData.DcMeterData.ammeterDcKwh);
    cJSON_AddStringToObject(DC_data,"ammeterDcKwh",temp);
    return DC_data;
}

cJSON *AcMeterDataBuild()
{
    char temp[200];
    cJSON * AC_data = cJSON_CreateObject();

    sprintf(temp,"%.3lf",StationRtData.AcMeterData.acAmmeterSatate );
    cJSON_AddStringToObject(AC_data,"acAmmeterSatate",temp);

    sprintf(temp,"%.3lf",StationRtData.AcMeterData.acGroupActiveTotalEnergy);
    cJSON_AddStringToObject(AC_data,"acGroupActiveTotalEnergy",temp);

    sprintf(temp,"%.3lf",StationRtData.AcMeterData.acGroupActivePointedEnergy );
    cJSON_AddStringToObject(AC_data,"acGroupActivePointedEnergy",temp);

    sprintf(temp,"%.3lf",StationRtData.AcMeterData.acGroupActivePeekEnergy );
    cJSON_AddStringToObject(AC_data,"acGroupActivePeekEnergy",temp);

    sprintf(temp,"%.3lf",StationRtData.AcMeterData.acGroupActiveFlatEnergy );
    cJSON_AddStringToObject(AC_data,"acGroupActiveFlatEnergy",temp);

    sprintf(temp,"%.3lf",StationRtData.AcMeterData.acGroupActiveValleyEnergy );
    cJSON_AddStringToObject(AC_data,"acGroupActiveValleyEnergy",temp);

    sprintf(temp,"%.3lf",StationRtData.AcMeterData.acPositiveActiveTotalEnergy );
    cJSON_AddStringToObject(AC_data,"acPositiveActiveTotalEnergy",temp);

    sprintf(temp,"%.3lf",StationRtData.AcMeterData.acPositiveActivePointedEnergy );
    cJSON_AddStringToObject(AC_data,"acPositiveActivePointedEnergy",temp);

    sprintf(temp,"%.3lf",StationRtData.AcMeterData.acPositiveActivePeekEnergy );
    cJSON_AddStringToObject(AC_data,"acPositiveActivePeekEnergy",temp);

    sprintf(temp,"%.3lf",StationRtData.AcMeterData.acPositiveActiveFlatEnergy );
    cJSON_AddStringToObject(AC_data,"acPositiveActiveFlatEnergy",temp);

    sprintf(temp,"%.3lf",StationRtData.AcMeterData.acPositiveActiveValleyEnergy);
    cJSON_AddStringToObject(AC_data,"acPositiveActiveValleyEnergy",temp);

    sprintf(temp,"%.3lf",StationRtData.AcMeterData.acNegativeActiveTotalEnergy );
    cJSON_AddStringToObject(AC_data,"acNegativeActiveTotalEnergy",temp);

    sprintf(temp,"%.3lf",StationRtData.AcMeterData.acNegativeActivePointedEnergy );
    cJSON_AddStringToObject(AC_data,"acNegativeActivePointedEnergy",temp);

    sprintf(temp,"%.3lf",StationRtData.AcMeterData.acNegativeActivePeekEnergy );
    cJSON_AddStringToObject(AC_data,"acNegativeActivePeekEnergy",temp);

    sprintf(temp,"%.3lf",StationRtData.AcMeterData.acNegativeActiveFlatEnergy );
    cJSON_AddStringToObject(AC_data,"acNegativeActiveFlatEnergy",temp);

    sprintf(temp,"%.3lf",StationRtData.AcMeterData.acNegativeActiveValleyEnergy);
    cJSON_AddStringToObject(AC_data,"acNegativeActiveValleyEnergy",temp);

    sprintf(temp,"%.3lf",StationRtData.AcMeterData.acGroupReactiveTotalEnergy );
    cJSON_AddStringToObject(AC_data,"acGroupReactiveTotalEnergy",temp);

    sprintf(temp,"%.3lf",StationRtData.AcMeterData.acGroupReactivePointedEnergy);
    cJSON_AddStringToObject(AC_data,"acGroupReactivePointedEnergy",temp);

    sprintf(temp,"%.3lf",StationRtData.AcMeterData.acGroupReactivePeekEnergy );
    cJSON_AddStringToObject(AC_data,"acGroupReactivePeekEnergy",temp);

    sprintf(temp,"%.3lf",StationRtData.AcMeterData.acGroupReactiveFlatEnergy );
    cJSON_AddStringToObject(AC_data,"acGroupReactiveFlatEnergy",temp);

    sprintf(temp,"%.3lf",StationRtData.AcMeterData.acGroupReactiveValleyEnergy );
    cJSON_AddStringToObject(AC_data,"acGroupReactiveValleyEnergy",temp);

    sprintf(temp,"%.3lf",StationRtData.AcMeterData.acPositiveReactiveTotalEnergy );
    cJSON_AddStringToObject(AC_data,"acPositiveReactiveTotalEnergy",temp);

    sprintf(temp,"%.3lf",StationRtData.AcMeterData.acPositiveReactivePointedEnergy );
    cJSON_AddStringToObject(AC_data,"acPositiveReactivePointedEnergy",temp);

    sprintf(temp,"%.3lf",StationRtData.AcMeterData.acPositiveReactivePeekEnergy);
    cJSON_AddStringToObject(AC_data,"acPositiveReactivePeekEnergy",temp);

    sprintf(temp,"%.3lf",StationRtData.AcMeterData.acPositiveReactiveFlatEnergy);
    cJSON_AddStringToObject(AC_data,"acPositiveReactiveFlatEnergy",temp);

    sprintf(temp,"%.3lf",StationRtData.AcMeterData.acPositiveReactiveValleyEnergy);
    cJSON_AddStringToObject(AC_data,"acPositiveReactiveValleyEnergy",temp);

    sprintf(temp,"%.3lf",StationRtData.AcMeterData.acNegativeReactiveTotalEnergy);
    cJSON_AddStringToObject(AC_data,"acNegativeReactiveTotalEnergy",temp);

    sprintf(temp,"%.3lf",StationRtData.AcMeterData.acNegativeReactivePointedEnergy);
    
    cJSON_AddStringToObject(AC_data,"acNegativeReactivePointedEnergy",temp);
    sprintf(temp,"%.3lf",StationRtData.AcMeterData.acNegativeReactivePeekEnergy);

    cJSON_AddStringToObject(AC_data,"acNegativeReactivePeekEnergy",temp);
    sprintf(temp,"%.3lf",StationRtData.AcMeterData.acNegativeReactiveFlatEnergy);

    cJSON_AddStringToObject(AC_data,"acNegativeReactiveFlatEnergy",temp);
    sprintf(temp,"%.3lf",StationRtData.AcMeterData.acNegativeReactiveValleyEnergy );

    cJSON_AddStringToObject(AC_data,"acNegativeReactiveValleyEnergy",temp);
    sprintf(temp,"%.3lf",StationRtData.AcMeterData.acAVoltage );

    cJSON_AddStringToObject(AC_data,"acAVoltage",temp);
    sprintf(temp,"%.3lf",StationRtData.AcMeterData.acBVoltage );
    cJSON_AddStringToObject(AC_data,"acBVoltage",temp);
    sprintf(temp,"%.3lf",StationRtData.AcMeterData.acCVoltage );
    cJSON_AddStringToObject(AC_data,"acCVoltage",temp);
    sprintf(temp,"%.3lf",StationRtData.AcMeterData.acACurrent );
    cJSON_AddStringToObject(AC_data,"acACurrent",temp);
    sprintf(temp,"%.3lf",StationRtData.AcMeterData.acBurrent );
    cJSON_AddStringToObject(AC_data,"acBurrent",temp);
    sprintf(temp,"%.3lf",StationRtData.AcMeterData.acCurrent );
    cJSON_AddStringToObject(AC_data,"acCurrent",temp);
    sprintf(temp,"%.3lf",StationRtData.AcMeterData.acAActivePower );
    cJSON_AddStringToObject(AC_data,"acAActivePower",temp);
    sprintf(temp,"%.3lf",StationRtData.AcMeterData.acBctivePower );
    cJSON_AddStringToObject(AC_data,"acBctivePower",temp);
    sprintf(temp,"%.3lf",StationRtData.AcMeterData.acCctivePower );
    cJSON_AddStringToObject(AC_data,"acCctivePower",temp);
    sprintf(temp,"%.3lf",StationRtData.AcMeterData.acTotalActivePower );
    cJSON_AddStringToObject(AC_data,"acTotalActivePower",temp);
    sprintf(temp,"%.3lf",StationRtData.AcMeterData.acAReactivePower);
    cJSON_AddStringToObject(AC_data,"acAReactivePower",temp);
    sprintf(temp,"%.3lf",StationRtData.AcMeterData.acBReactivePower);
    cJSON_AddStringToObject(AC_data,"acBReactivePower",temp);
    sprintf(temp,"%.3lf",StationRtData.AcMeterData.acCReactivePower);
    cJSON_AddStringToObject(AC_data,"acCReactivePower",temp);
    sprintf(temp,"%.3lf",StationRtData.AcMeterData.acTotalReactivePower);
    cJSON_AddStringToObject(AC_data,"acTotalReactivePower",temp);
    sprintf(temp,"%.3lf",StationRtData.AcMeterData.acALookPower);
    cJSON_AddStringToObject(AC_data,"acALookPower",temp);
    sprintf(temp,"%.3lf",StationRtData.AcMeterData.acBLookPower);
    cJSON_AddStringToObject(AC_data,"acBLookPower",temp);
    sprintf(temp,"%.3lf",StationRtData.AcMeterData.acCLookPower);
    cJSON_AddStringToObject(AC_data,"acCLookPower",temp);
    sprintf(temp,"%.3lf",StationRtData.AcMeterData.acTotalLookPower);
    cJSON_AddStringToObject(AC_data,"acTotalLookPower",temp);
    sprintf(temp,"%.3lf",StationRtData.AcMeterData.acAPhasePowerFactor );
    cJSON_AddStringToObject(AC_data,"acAPhasePowerFactor",temp);

    sprintf(temp,"%.3lf",StationRtData.AcMeterData.acBPhasePowerFactor );
    cJSON_AddStringToObject(AC_data,"acBPhasePowerFactor",temp);

    sprintf(temp,"%.3lf",StationRtData.AcMeterData.acCPhasePowerFactor );
    cJSON_AddStringToObject(AC_data,"acCPhasePowerFactor",temp);

    sprintf(temp,"%.3lf",StationRtData.AcMeterData.acTotalPowerFactor );
    cJSON_AddStringToObject(AC_data,"acTotalPowerFactor",temp);
    
    sprintf(temp,"%.3lf",StationRtData.AcMeterData.acCt);
    cJSON_AddStringToObject(AC_data,"acCt",temp);

    sprintf(temp,"%.3lf",StationRtData.AcMeterData.acPt);
    cJSON_AddStringToObject(AC_data,"acPt",temp);

    sprintf(temp,"%.3lf",StationRtData.AcMeterData.acZroeCurrent );
    cJSON_AddStringToObject(AC_data,"acZroeCurrent",temp);

    sprintf(temp,"%.3lf",StationRtData.AcMeterData.acVoltageUneven );
    cJSON_AddStringToObject(AC_data,"acVoltageUneven",temp);

    sprintf(temp,"%.3lf",StationRtData.AcMeterData.acCurrentUneven );
    cJSON_AddStringToObject(AC_data,"acCurrentUneven",temp);  

    return  AC_data;
}


void networkManger(char * url,char *jsondata)
{
    if(url == NULL || jsondata == NULL)
        return;
    
    CURLcode curlRet = 0;
    CURL* pHandle = NULL;
    long *pHttpCode;
    char pRes[10000]= {0};
    do{
        /* 全局初始化 */
        // curlRet = curl_global_init(CURL_GLOBAL_SSL|CURL_GLOBAL_ALL );

        struct curl_slist* headers = NULL;
        char tmp[50] = {0};
        sprintf(tmp, "Content-Length: %d", (int)strlen(jsondata));
        headers = curl_slist_append(headers, "Accept: application/json");
        headers = curl_slist_append(headers, "Content-Type: application/json");
        headers = curl_slist_append(headers, "charset: utf-8");
        headers = curl_slist_append(headers, tmp);

        /* 初始化libcurl会话 */
        pHandle = curl_easy_init();
        if(NULL == pHandle)
        {
            printf("curl_easy_init()error.\n");
            break;
        }

        /* 设置连接超时时间 2秒*/
        curlRet = curl_easy_setopt(pHandle, CURLOPT_CONNECTTIMEOUT, 2);
        /* 设置超时时间 5秒*/
        curlRet = curl_easy_setopt(pHandle, CURLOPT_TIMEOUT, 5);
        /* 设置url*/
        curlRet = curl_easy_setopt(pHandle, CURLOPT_URL, url);
        if(curlRet != 0)
        {
            printf("curl_easy_setopt() error. curlRet = %d\n", curlRet);
            break;
        }

        curlRet = curl_easy_setopt(pHandle, CURLOPT_HTTPHEADER, headers);
        if(curlRet != 0)
        {
            printf("curl_easy_setopt() error. curlRet = %d\n", curlRet);
            break;
        }
        curlRet = curl_easy_setopt(pHandle, CURLOPT_POST, 1L);
        if(curlRet != 0)
        {
            printf("curl_easy_setopt() error. curlRet = %d\n", curlRet);
            break;
        }
        curlRet = curl_easy_setopt(pHandle, CURLOPT_POSTFIELDS, jsondata);
        if(curlRet != 0)
        {
            printf("curl_easy_setopt() error. curlRet = %d\n", curlRet);
            break;
        }
        /* 设置 回调函数 */
        curlRet = curl_easy_setopt(pHandle, CURLOPT_WRITEFUNCTION, httpCurlResCallback);
        if(curlRet != 0)
        {
            printf("curl_easy_setopt() error. curlRet = %d\n", curlRet);
            break;
        }
        /*传入回调函数需要的结构体的指针 */
        curlRet = curl_easy_setopt(pHandle, CURLOPT_WRITEDATA, (void *)pRes);
        if(curlRet != 0)
        {
            printf("curl_easy_setopt() error. curlRet = %d\n", curlRet);
            break;
        }
        curlRet = curl_easy_setopt(pHandle, CURLOPT_NOSIGNAL, 1);
        if(curlRet != 0)
        {
            printf("curl_easy_setopt() error. curlRet = %d\n", curlRet);
            break;
        }

        curlRet = curl_easy_perform(pHandle);
        if (curlRet == CURLE_COULDNT_RESOLVE_HOST){
                res_init();
        }
        if(curlRet != 0)
        {
            printf("curl_easy_perform() error. curlRet = %d\n", curlRet);
            curl_slist_free_all(headers);
            headers = NULL;
            curl_easy_cleanup(pHandle);
            pHandle = NULL;
            // 释放全局curl对象
            // curl_global_cleanup();
            break;
        }
        // 释放headers
        curl_slist_free_all(headers);
        headers = NULL;
        // 释放curl对象
        curl_easy_cleanup(pHandle);
        pHandle = NULL;
            // 释放全局curl对象
        // curl_global_cleanup();
    }while(0);
}

/*******************************
 * 构建一组云端通信使用的JSON故障信息
 * ****************************/
cJSON * ErrMsgBuild()
{
	int i=0,j=0;
    cJSON *message_obj = cJSON_CreateArray();
	if(NULL == message_obj)
		return NULL;

	/*换电设备*/
	if(SelfChkErr_EMS_CeDev_ComErr)
	{
		cJSON *tmp_obj = NULL;
		if(NULL !=(tmp_obj = ErrObjBuild("Device2",1,"EMS与换电设备通信故障")))
		{
			cJSON_AddItemToArray(message_obj,tmp_obj);
		}
	}
    else if(CE_Err_Code)
    {
        int ret = GetErrIndex_Ce(CE_Err_Code);
		cJSON *tmp_obj = NULL;
		if(NULL !=(tmp_obj = ErrObjBuild("Device2",1,
			(ret !=-1)?err_table_CBDev[ret].ErrName:"换电设备 UNKNOWN_ERR")))
		{
			cJSON_AddItemToArray(message_obj,tmp_obj);
		}
    }
    
	/*充电设备*/
    for(i=0;i<18;++i)
    {
        if(SelfChkErr_EMS_ChgerDev_ComErr[i])
		{
			cJSON *tmp_obj = NULL;
			if(NULL !=(tmp_obj = ErrObjBuild("Device4",i+1,"EMS与充电设备通信故障")))
			{
				cJSON_AddItemToArray(message_obj,tmp_obj);
			}
		}
		else if(1 == ChgerDev_Err_Sta[i])
        {
            int ret = GetErrIndex_Chger(MChg_ErrStop_code[i]);
			cJSON *tmp_obj = NULL;
			if(NULL !=(tmp_obj = ErrObjBuild("Device4",i+1,\
					(ret !=-1)?err_table_ChgerDev[ret].ErrName:"充电设备 UNKNOWN_ERR")))
			{
				cJSON_AddItemToArray(message_obj,tmp_obj);
			}
        }
    }

	/*消防故障*/
	if(SelfChkErr_EMS_FireCon_ComErr)
	{
		cJSON *tmp_obj = NULL;
		if(NULL !=(tmp_obj = ErrObjBuild("Device6",1,"EMS与消防设备通信故障")))
		{
			cJSON_AddItemToArray(message_obj,tmp_obj);
		}
	}
	else
	{
		for(i=0;i<2;++i)
		{
			if(1 == FireCon_Err_Sta[i])
			{
				cJSON *tmp_obj = NULL;
				if(NULL !=(tmp_obj = ErrObjBuild("Device6",1,err_table_FireCntrl[i].ErrName)))
				{
					cJSON_AddItemToArray(message_obj,tmp_obj);
				}
			}
		}
	} 

	/*空调故障*/
	if(SelfChkErr_EMS_AirCon_ComErr)
	{
		cJSON *tmp_obj = NULL;
		if(NULL !=(tmp_obj = ErrObjBuild("Device8",1,"EMS与空调设备通信故障")))
		{
			cJSON_AddItemToArray(message_obj,tmp_obj);
		}
	}
	else
	{	/* 空调 */
		for(i=0;i<3;i++)
		{
			for(j=0;j<14;j++)
			{
				if( 1 == AirCon_Err_Sta[i][j] )
				{	
					cJSON *tmp_obj = NULL;
					if(NULL !=(tmp_obj = ErrObjBuild("Device8",i+1,err_table_AirCon[j].ErrName)))
					{
						cJSON_AddItemToArray(message_obj,tmp_obj);
					}
				}
			}
		}
	}

    return message_obj;
}


/************************************************************
 * 构建一条云端通信使用的JSON故障信息
 * **********************************************************/
static cJSON * ErrObjBuild(char* DevName,int DevNo,char * Mes)
{
	if(NULL == DevName || NULL == Mes )
		return NULL;
	
	cJSON *tmp_obj = cJSON_CreateObject();
	if(NULL == tmp_obj )
		return NULL;

	cJSON_AddStringToObject(tmp_obj, "Device", DevName);
	uint8_t tmp[200];
	sprintf(tmp,"%d",DevNo);
	cJSON_AddStringToObject(tmp_obj, "No", tmp);
	cJSON_AddStringToObject(tmp_obj, "Mes",Mes);

	return tmp_obj;
}