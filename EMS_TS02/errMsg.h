#ifndef _ERR_MSG_H
#define _ERR_MSG_H
#include "sqlite3.h"


/* 故障记录 */
typedef struct{
int Err_code;

char *ErrDev;
char Err_Lev;

char *Err_code_str;
char *ErrName;

}table_ErrRecord_t;


extern table_ErrRecord_t err_table_CBDev[];
extern table_ErrRecord_t err_table_ChgerDev[];

extern table_ErrRecord_t err_table_FireCntrl[];
extern table_ErrRecord_t err_table_AirCon[];
extern table_ErrRecord_t err_table_SelfChkErr[];
extern table_ErrRecord_t err_table_db[];

int GetErrIndex_Chger(int errCode);
int GetErrIndex_Ce(int errCode);


static cJSON * ErrObjBuild(char* DevName,int DevNo,char * Mes);
cJSON * ErrMsgBuild();

void wrtData2SqlDB(sqlite3 *db,const char *sqlCmdBuf);
void Sql_ErrInter_Deal2(table_ErrRecord_t errdata_t,int DevIndex);
void sql_WriteErrData(sqlite3 *db);



/*换电设备*/
extern int CE_Err_Code;
extern uint8_t SelfChkErr_EMS_CeDev_ComErr;
/*充电设备*/
extern uint8_t ChgerDev_Err_Sta[20];//当前故障状态
extern uint8_t SelfChkErr_EMS_ChgerDev_ComErr[20];
/*空调故障*/
extern uint8_t AirCon_Err_Sta[3][14];//当前故障状态
/*消防故障*/
extern uint8_t FireCon_Err_Sta[2];//当前故障状态

// //*************************************************************
// // uint8_t SelfChkErr_BattHeat_Err[20] = {0};//电池包热失控故障
// // uint8_t SelfChkErr_BattHeat_Err_last[20]= {0};//电池包热失控故障
// uint8_t SelfChkErr_DoorStop_Err[3] = {0};//门禁开启
// uint8_t SelfChkErr_DoorStop_Err_last[3] = {0};//门禁开启
// uint8_t SelfChkErr_Lightr_Err[2] = {0};//光栅
// uint8_t SelfChkErr_Lightr_Err_last[2] = {0};//光栅

// uint8_t SelfChkErr_Thunder_Err[2] = {0};//雷击故障
// uint8_t SelfChkErr_Thunder_Err_last[2] = {0};//雷击故障
// uint8_t SelfChkErr_Water_Err= {0};//水浸故障
// uint8_t SelfChkErr_Water_Err_last= {0};//水浸故障
// uint8_t SelfChkErr_ChgerSys_Err= {0};//充电系统故障
// uint8_t SelfChkErr_ChgerSys_Err_last= {0};//充电系统故障

extern uint8_t SelfChkErr_EMS_Cloud_ComErr;
extern uint8_t SelfChkErr_EMS_CeDev_ComErr;
extern uint8_t SelfChkErr_EMS_ChgerDev_ComErr[20];
extern uint8_t SelfChkErr_EMS_AirCon_ComErr;
extern uint8_t SelfChkErr_EMS_FireCon_ComErr;
extern uint8_t SelfChkErr_ACMeter_ComErr;
extern uint8_t SelfChkErr_DCMeter_ComErr[18];


#endif // !


