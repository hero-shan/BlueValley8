/*
 * File: SA4_EMS.h
 *
 * Code generated for Simulink model 'SA4_EMS'.
 *
 * Model version                  : 1.418
 * Simulink Coder version         : 8.14 (R2018a) 06-Feb-2018
 * C/C++ source code generated on : Mon May 10 14:44:01 2021
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: NXP->Cortex-M4
 * Code generation objectives:
 *    1. Execution efficiency
 *    2. RAM efficiency
 * Validation result: Not run
 */

#ifndef RTW_HEADER_SA4_EMS_h_
#define RTW_HEADER_SA4_EMS_h_
#ifndef SA4_EMS_COMMON_INCLUDES_
# define SA4_EMS_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif                                 /* SA4_EMS_COMMON_INCLUDES_ */

/* Macros for accessing real-time model data structure */
#ifndef rtmGetErrorStatus
# define rtmGetErrorStatus(rtm)        ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
# define rtmSetErrorStatus(rtm, val)   ((rtm)->errorStatus = (val))
#endif

/* Forward declaration for rtModel */
typedef struct tag_RTM RT_MODEL;

/* Block signals and states (default storage) for system '<S3>/Chart1' */
typedef struct {
  real_T Chg_wait_cou;                 /* '<S3>/Chart1' */
  uint8_T is_active_c1_SA4_EMS;        /* '<S3>/Chart1' */
  uint8_T is_CHU_deal;                 /* '<S3>/Chart1' */
  uint8_T is_GetCellVol;               /* '<S3>/Chart1' */
  uint8_T is_GetCellTemp;              /* '<S3>/Chart1' */
  uint8_T is_GetBattBasicData;         /* '<S3>/Chart1' */
  uint8_T is_GetCharPreInf;            /* '<S3>/Chart1' */
  uint8_T is_chg_main_state;           /* '<S3>/Chart1' */
  uint8_T is_GetBattRtData;            /* '<S3>/Chart1' */
} DW_Chart1;

/* Block signals and states (default storage) for system '<Root>' */
typedef struct {
  DW_Chart1 sf_Chart9;                 /* '<S3>/Chart9' */
  DW_Chart1 sf_Chart8;                 /* '<S3>/Chart8' */
  DW_Chart1 sf_Chart7;                 /* '<S3>/Chart7' */
  DW_Chart1 sf_Chart6;                 /* '<S3>/Chart6' */
  DW_Chart1 sf_Chart5;                 /* '<S3>/Chart5' */
  DW_Chart1 sf_Chart4;                 /* '<S3>/Chart4' */
  DW_Chart1 sf_Chart3;                 /* '<S3>/Chart3' */
  DW_Chart1 sf_Chart20;                /* '<S3>/Chart20' */
  DW_Chart1 sf_Chart2;                 /* '<S3>/Chart2' */
  DW_Chart1 sf_Chart19;                /* '<S3>/Chart19' */
  DW_Chart1 sf_Chart18;                /* '<S3>/Chart18' */
  DW_Chart1 sf_Chart17;                /* '<S3>/Chart17' */
  DW_Chart1 sf_Chart16;                /* '<S3>/Chart16' */
  DW_Chart1 sf_Chart15;                /* '<S3>/Chart15' */
  DW_Chart1 sf_Chart14;                /* '<S3>/Chart14' */
  DW_Chart1 sf_Chart13;                /* '<S3>/Chart13' */
  DW_Chart1 sf_Chart12;                /* '<S3>/Chart12' */
  DW_Chart1 sf_Chart11;                /* '<S3>/Chart11' */
  DW_Chart1 sf_Chart10;                /* '<S3>/Chart10' */
  DW_Chart1 sf_Chart1_b;               /* '<S3>/Chart1' */
  real_T MSqlite_WriLoc_ok[20];        /* '<Root>/Chart1' */
  real_T MIO_DoorValue_cou[3];         /* '<Root>/Chart' */
  real_T MIO_LightValue_cou[2];        /* '<Root>/Chart' */
  real_T MIO_ThunderValue_cou[2];      /* '<Root>/Chart' */
  real_T CarIn_ToSelfCheck_flg;        /* '<Root>/Chart' */
  real_T DatLib_AirconDa_cou;          /* '<Root>/Chart1' */
  real_T DatLib_CharDa_cou;            /* '<Root>/Chart1' */
  real_T DatLib_BattData_cou;          /* '<Root>/Chart1' */
  real_T DatLib_AcEnergy_cou;          /* '<Root>/Chart1' */
  real_T MCE_BatOut_Times;             /* '<Root>/Chart' */
  real_T MCE_BatOut_count;             /* '<Root>/Chart' */
  real_T MCE_CELocator_Times;          /* '<Root>/Chart' */
  real_T MCE_CELocator_count;          /* '<Root>/Chart' */
  real_T MCE_ChangeBat_Times;          /* '<Root>/Chart' */
  real_T MCE_ChangeBat_count;          /* '<Root>/Chart' */
  real_T MCE_Continue_Times;           /* '<Root>/Chart' */
  real_T MCE_Continue_count;           /* '<Root>/Chart' */
  real_T MCE_DismountBat_Times;        /* '<Root>/Chart' */
  real_T MCE_DismountBat_count;        /* '<Root>/Chart' */
  real_T MCE_ErrReset_Times;           /* '<Root>/Chart' */
  real_T MCE_ErrReset_count;           /* '<Root>/Chart' */
  real_T MCE_InLocator_Times;          /* '<Root>/Chart' */
  real_T MCE_InLocator_count;          /* '<Root>/Chart' */
  real_T MCE_InitDeal_Times;           /* '<Root>/Chart' */
  real_T MCE_InitDeal_count;           /* '<Root>/Chart' */
  real_T MCE_MountBat_Times;           /* '<Root>/Chart' */
  real_T MCE_MountBat_count;           /* '<Root>/Chart' */
  real_T MCE_OutLocator_Times;         /* '<Root>/Chart' */
  real_T MCE_OutLocator_count;         /* '<Root>/Chart' */
  real_T MCE_Stop_Times;               /* '<Root>/Chart' */
  real_T MCE_Stop_count;               /* '<Root>/Chart' */
  real_T MCE_TimeOut_Times;            /* '<Root>/Chart' */
  real_T MCE_TimeOut_count;            /* '<Root>/Chart' */
  real_T MCE_Comm_cou;                 /* '<Root>/Chart' */
  real_T MCE_Initok_cou;               /* '<Root>/Chart' */
  real_T MCloud_Comm_cou;              /* '<Root>/Chart' */
  real_T MGrab_Comm_cou;               /* '<Root>/Chart' */
  real_T MIO_Comm_cou;                 /* '<Root>/Chart' */
  real_T MCE_WaitInPlac_cou;           /* '<Root>/Chart' */
  real_T MCloud_CarAllowIn_cou;        /* '<Root>/Chart' */
  real_T MCloud_CarAllowOut_cou;       /* '<Root>/Chart' */
  real_T MCloud_CarAllowOut_time;      /* '<Root>/Chart' */
  real_T MIO_WaterValue_cou;           /* '<Root>/Chart' */
  real_T MCE_WaitWriSqlCloud_cou;      /* '<Root>/Chart' */
  real_T MCloud_CarAllowIn_time;       /* '<Root>/Chart' */
  real_T QtSend_CharDa_cou;            /* '<Root>/Chart' */
  real_T QtSend_CharErr_cou;           /* '<Root>/Chart' */
  real_T QtSend_CEDat_cou;             /* '<Root>/Chart' */
  real_T QtSend_LocatData_cou;         /* '<Root>/Chart' */
  real_T CheckBag_time;                /* '<Root>/Chart' */
  real_T MLocat_CheckInNet_cou;        /* '<Root>/Chart' */
  real_T MQt_SendPoint_cou;            /* '<Root>/Chart' */
  real_T MQt_SendPoint_time;           /* '<Root>/Chart' */
  real_T Get_cou;                      /* '<Root>/Chart' */
  real_T MCloud_GetCarType_cou;        /* '<Root>/Chart' */
  real_T MCloud_CarODO_cou;            /* '<Root>/Chart' */
  real_T MCloud_CarODO_time;           /* '<Root>/Chart' */
  real_T MCE_WaitCancOrd_cou;          /* '<Root>/Chart' */
  real_T QtSend_AirData_cou;           /* '<Root>/Chart' */
  real_T RemoSend_CharDa_cou;          /* '<Root>/Chart' */
  real_T RemoSend_CharErr_cou;         /* '<Root>/Chart' */
  real_T RemotSend_Heart_cou;          /* '<Root>/Chart' */
  real_T RemotSend_UpACEng_cou;        /* '<Root>/Chart' */
  real_T RemotSend_UpAirCon_cou;       /* '<Root>/Chart' */
  real_T RemotSend_UpCEing_cou;        /* '<Root>/Chart' */
  real_T RemotSend_UpFan_cou;          /* '<Root>/Chart' */
  real_T RemotSend_UpFire_cou;         /* '<Root>/Chart' */
  real_T RemotSend_UpPLC_cou;          /* '<Root>/Chart' */
  real_T LockInit_cou;                 /* '<Root>/Chart' */
  real_T Wait_CELocator_cou;           /* '<Root>/Chart' */
  real_T Wait_ChangeBat_cou;           /* '<Root>/Chart' */
  real_T Wait_InLocator_cou;           /* '<Root>/Chart' */
  real_T Wait_OutLocator_cou;          /* '<Root>/Chart' */
  real_T FireErr_cou;                  /* '<Root>/Chart' */
  real_T Free_cou;                     /* '<Root>/Chart' */
  real_T InitErr_cou;                  /* '<Root>/Chart' */
  real_T PleaseGo_cou;                 /* '<Root>/Chart' */
  real_T MLed_Start10s_Num1;           /* '<Root>/Chart' */
  real_T MLed_Start10s_Num2;           /* '<Root>/Chart' */
  real_T MLed_Start10s_Num3;           /* '<Root>/Chart' */
  real_T MLed_Start10s_Num4;           /* '<Root>/Chart' */
  real_T QtSend_DcEne_cou;             /* '<Root>/Chart' */
  real_T QtSend_DcEne2_cou;            /* '<Root>/Chart' */
  real_T Check_PLCDie_cou;             /* '<Root>/Chart' */
  uint8_T is_active_c4_SA4_EMS;        /* '<Root>/Chart1' */
  uint8_T is_DatLib_CharDa;            /* '<Root>/Chart1' */
  uint8_T is_DatLib_ConfDa;            /* '<Root>/Chart1' */
  uint8_T is_DatLib_AirconDa;          /* '<Root>/Chart1' */
  uint8_T is_DatLib_LocatDa;           /* '<Root>/Chart1' */
  uint8_T is_DatLib_Err;               /* '<Root>/Chart1' */
  uint8_T is_DatLib_BattData;          /* '<Root>/Chart1' */
  uint8_T is_DatLib_AcEnergy;          /* '<Root>/Chart1' */
  uint8_T is_active_c3_SA4_EMS;        /* '<Root>/Chart' */
  uint8_T is_TimeOut_Order;            /* '<Root>/Chart' */
  uint8_T is_BatInLocator_Order;       /* '<Root>/Chart' */
  uint8_T is_Init_Order;               /* '<Root>/Chart' */
  uint8_T is_Continue_Order;           /* '<Root>/Chart' */
  uint8_T is_BatOutLocator_Order;      /* '<Root>/Chart' */
  uint8_T is_BatOut_Order;             /* '<Root>/Chart' */
  uint8_T is_Stop_Order;               /* '<Root>/Chart' */
  uint8_T is_CELocator_Order;          /* '<Root>/Chart' */
  uint8_T is_ChangeBatt_Order;         /* '<Root>/Chart' */
  uint8_T is_MountBat_Order;           /* '<Root>/Chart' */
  uint8_T is_DisMountBat_Order;        /* '<Root>/Chart' */
  uint8_T is_ErrReset_Order;           /* '<Root>/Chart' */
  uint8_T is_Main_CE;                  /* '<Root>/Chart' */
  uint8_T is_Main_Init;                /* '<Root>/Chart' */
  uint8_T is_All_Self_Check;           /* '<Root>/Chart' */
  uint8_T is_MIO_COMM;                 /* '<Root>/Chart' */
  uint8_T is_MCE_COMM;                 /* '<Root>/Chart' */
  uint8_T is_MGrab_COMM;               /* '<Root>/Chart' */
  uint8_T is_MCloud_COMM;              /* '<Root>/Chart' */
  uint8_T is_MCE_InitOk;               /* '<Root>/Chart' */
  uint8_T is_Allow_InStation;          /* '<Root>/Chart' */
  uint8_T is_Init5;                    /* '<Root>/Chart' */
  uint8_T is_Init6;                    /* '<Root>/Chart' */
  uint8_T is_Car_In;                   /* '<Root>/Chart' */
  uint8_T is_Init6_p;                  /* '<Root>/Chart' */
  uint8_T is_ChangeBatt;               /* '<Root>/Chart' */
  uint8_T is_Init4;                    /* '<Root>/Chart' */
  uint8_T is_Cloud_Pass;               /* '<Root>/Chart' */
  uint8_T is_Init4_n;                  /* '<Root>/Chart' */
  uint8_T is_Qt_SendChgDat;            /* '<Root>/Chart' */
  uint8_T is_Qt_SendChgErr;            /* '<Root>/Chart' */
  uint8_T is_Qt_SendCEDat;             /* '<Root>/Chart' */
  uint8_T is_Qt_SendChgErr1;           /* '<Root>/Chart' */
  uint8_T is_Qt_SendAirData;           /* '<Root>/Chart' */
  uint8_T is_Qt_SendDcEne;             /* '<Root>/Chart' */
  uint8_T is_Qt_SendDcEne2;            /* '<Root>/Chart' */
  uint8_T is_MIO_Door_Wat_Lig_Thu;     /* '<Root>/Chart' */
  uint8_T is_Remo_SendChgDat;          /* '<Root>/Chart' */
  uint8_T is_Remo_SendChgErr;          /* '<Root>/Chart' */
  uint8_T is_RemoSend_UpPLC;           /* '<Root>/Chart' */
  uint8_T is_RemoSend_UpACEng;         /* '<Root>/Chart' */
  uint8_T is_RemotSend_UpAirCon;       /* '<Root>/Chart' */
  uint8_T is_RemotSend_UpFan;          /* '<Root>/Chart' */
  uint8_T is_RemotSend_UpFire;         /* '<Root>/Chart' */
  uint8_T is_RemotSend_UpCEing;        /* '<Root>/Chart' */
  uint8_T is_RemotSend_Heart;          /* '<Root>/Chart' */
  uint8_T is_Led_AllPlay;              /* '<Root>/Chart' */
  uint8_T is_Play_Sta4;                /* '<Root>/Chart' */
  uint8_T is_Play_Sta2;                /* '<Root>/Chart' */
  uint8_T is_Play_Sta3;                /* '<Root>/Chart' */
  uint8_T is_Play_Sta4_i;              /* '<Root>/Chart' */
  uint8_T is_Play_Sta5;                /* '<Root>/Chart' */
} DW;

/* Real-time Model Data Structure */
struct tag_RTM {
  const char_T * volatile errorStatus;
};

/* Block signals and states (default storage) */
extern DW rtDW;

/*
 * Exported States
 *
 * Note: Exported states are block states with an exported global
 * storage class designation.  Code generation will declare the memory for these
 * states and exports their symbols.
 *
 */
extern real_T BMS_PackSocReal_pct[20]; /* Simulink.Signal object 'BMS_PackSocReal_pct' */
extern real_T Cloud_SendODO_km;        /* Simulink.Signal object 'Cloud_SendODO_km' */
extern real_T EMS_CHUSetPwr_kw[20];    /* Simulink.Signal object 'EMS_CHUSetPwr_kw' */
extern uint16_T CHU_LowPwrDown_count[20];/* Simulink.Signal object 'CHU_LowPwrDown_count' */
extern uint16_T CHU_LowPwrOn_count[20];/* Simulink.Signal object 'CHU_LowPwrOn_count' */
extern uint16_T GetBattBasicData_count0[20];/* Simulink.Signal object 'GetBattBasicData_count0' */
extern uint16_T GetBattBasicData_count1[20];/* Simulink.Signal object 'GetBattBasicData_count1' */
extern uint16_T GetBattRtData_count0[20];/* Simulink.Signal object 'GetBattRtData_count0' */
extern uint16_T GetBattRtData_count1[20];/* Simulink.Signal object 'GetBattRtData_count1' */
extern uint16_T GetCellTemp_count0[20];/* Simulink.Signal object 'GetCellTemp_count0' */
extern uint16_T GetCellTemp_count1[20];/* Simulink.Signal object 'GetCellTemp_count1' */
extern uint16_T GetCellVol_count0[20]; /* Simulink.Signal object 'GetCellVol_count0' */
extern uint16_T GetCellVol_count1[20]; /* Simulink.Signal object 'GetCellVol_count1' */
extern uint16_T GetCharEndInf_count0[20];/* Simulink.Signal object 'GetCharEndInf_count0' */
extern uint16_T GetCharEndInf_count1[20];/* Simulink.Signal object 'GetCharEndInf_count1' */
extern uint16_T GetCharPreInf_count0[20];/* Simulink.Signal object 'GetCharPreInf_count0' */
extern uint16_T GetCharPreInf_count1[20];/* Simulink.Signal object 'GetCharPreInf_count1' */
extern uint16_T Get_CHUChgRtData_count[20];/* Simulink.Signal object 'Get_CHUChgRtData_count' */
extern uint16_T SetPwr_count[20];      /* Simulink.Signal object 'SetPwr_count' */
extern uint16_T StartChg_count[20];    /* Simulink.Signal object 'StartChg_count' */
extern uint16_T StopChg_count[20];     /* Simulink.Signal object 'StopChg_count' */
extern uint8_T AirCon_conn_flg;        /* Simulink.Signal object 'AirCon_conn_flg' */
extern uint8_T BATT_IsExist_flg[20];   /* Simulink.Signal object 'BATT_IsExist_flg' */
extern uint8_T Book_Init_flg;          /* Simulink.Signal object 'Book_Init_flg' */
extern uint8_T CE_OutNetSelfCe_sour;   /* Simulink.Signal object 'CE_OutNetSelfCe_sour' */
extern uint8_T CHU_AllErr_flg[20];     /* Simulink.Signal object 'CHU_AllErr_flg' */
extern uint8_T CHU_CharFull_flg[20];   /* Simulink.Signal object 'CHU_CharFull_flg' */
extern uint8_T CHU_DevState_code[20];  /* Simulink.Signal object 'CHU_DevState_code' */
extern uint8_T CHU_ErrCode_buf0[20];   /* Simulink.Signal object 'CHU_ErrCode_buf0' */
extern uint8_T CHU_ErrCode_buf1[20];   /* Simulink.Signal object 'CHU_ErrCode_buf1' */
extern uint8_T CHU_ErrCode_buf2[20];   /* Simulink.Signal object 'CHU_ErrCode_buf2' */
extern uint8_T CHU_ErrCode_buf3[20];   /* Simulink.Signal object 'CHU_ErrCode_buf3' */
extern uint8_T CHU_GetBatBasic_flg[20];/* Simulink.Signal object 'CHU_GetBatBasic_flg' */
extern uint8_T CHU_LowPwrDownErr_flg[20];/* Simulink.Signal object 'CHU_LowPwrDownErr_flg' */
extern uint8_T CHU_LowPwrDown_timeout[20];/* Simulink.Signal object 'CHU_LowPwrDown_timeout' */
extern uint8_T CHU_LowPwrOnErr_flg[20];/* Simulink.Signal object 'CHU_LowPwrOnErr_flg' */
extern uint8_T CHU_LowPwrOn_timeout[20];/* Simulink.Signal object 'CHU_LowPwrOn_timeout' */
extern uint8_T CHU_StartChgErr_flg[20];/* Simulink.Signal object 'CHU_StartChgErr_flg' */
extern uint8_T CHU_StopChgErr_flg[20]; /* Simulink.Signal object 'CHU_StopChgErr_flg' */
extern uint8_T Chg_Control_mode[20];   /* Simulink.Signal object 'Chg_Control_mode' */
extern uint8_T Cloud_AppointLock_flg[20];/* Simulink.Signal object 'Cloud_AppointLock_flg' */
extern uint8_T Cloud_BookCarNo_flg;    /* Simulink.Signal object 'Cloud_BookCarNo_flg' */
extern uint8_T Cloud_OutNetCheckBatNum_flg;/* Simulink.Signal object 'Cloud_OutNetCheckBatNum_flg' */
extern uint8_T Cloud_OutNetLock_flg[20];/* Simulink.Signal object 'Cloud_OutNetLock_flg' */
extern uint8_T Cloud_carType_Code;     /* Simulink.Signal object 'Cloud_carType_Code' */
extern uint8_T EMS_CHU_ComErr_flg[20]; /* Simulink.Signal object 'EMS_CHU_ComErr_flg' */
extern uint8_T EMS_Chger_BigSta[20];   /* Simulink.Signal object 'EMS_Chger_BigSta' */
extern uint8_T EMS_SendGetBattBasicDataCmd_flg[20];/* Simulink.Signal object 'EMS_SendGetBattBasicDataCmd_flg' */
extern uint8_T EMS_SendGetBattRtDataCmd_flg[20];/* Simulink.Signal object 'EMS_SendGetBattRtDataCmd_flg' */
extern uint8_T EMS_SendGetCellTempCmd_flg[20];/* Simulink.Signal object 'EMS_SendGetCellTempCmd_flg' */
extern uint8_T EMS_SendGetCellVolCmd_flg[20];/* Simulink.Signal object 'EMS_SendGetCellVolCmd_flg' */
extern uint8_T EMS_SendGetCharEndInfCmd_flg[20];/* Simulink.Signal object 'EMS_SendGetCharEndInfCmd_flg' */
extern uint8_T EMS_SendGetCharPreInfCmd_flg[20];/* Simulink.Signal object 'EMS_SendGetCharPreInfCmd_flg' */
extern uint8_T EMS_SendLowPwrDownCmd_flg[20];/* Simulink.Signal object 'EMS_SendLowPwrDownCmd_flg' */
extern uint8_T EMS_SendLowPwrOnCmd_flg[20];/* Simulink.Signal object 'EMS_SendLowPwrOnCmd_flg' */
extern uint8_T EMS_SendSetPwrCmd_flg[20];/* Simulink.Signal object 'EMS_SendSetPwrCmd_flg' */
extern uint8_T EMS_SendStartChgCmd_flg[20];/* Simulink.Signal object 'EMS_SendStartChgCmd_flg' */
extern uint8_T EMS_SendStopChgCmd_flg[20];/* Simulink.Signal object 'EMS_SendStopChgCmd_flg' */
extern uint8_T EMS_StopChar_flg[20];   /* Simulink.Signal object 'EMS_StopChar_flg' */
extern uint8_T FireContr_GetFire_flg;  /* Simulink.Signal object 'FireContr_GetFire_flg' */
extern uint8_T FireContr_conn_flg;     /* Simulink.Signal object 'FireContr_conn_flg' */
extern uint8_T Get_CHUChgRtDataErr_flg[20];/* Simulink.Signal object 'Get_CHUChgRtDataErr_flg' */
extern uint8_T Get_CHUChgRtData_flg[20];/* Simulink.Signal object 'Get_CHUChgRtData_flg' */
extern uint8_T Get_CHUErrStop_flg[20]; /* Simulink.Signal object 'Get_CHUErrStop_flg' */
extern uint8_T Grab_GetCarCard_flg;    /* Simulink.Signal object 'Grab_GetCarCard_flg' */
extern uint8_T Hand_LowPwrDown_flg[20];/* Simulink.Signal object 'Hand_LowPwrDown_flg' */
extern uint8_T Hand_LowPwrOn_flg[20];  /* Simulink.Signal object 'Hand_LowPwrOn_flg' */
extern uint8_T Hand_StartChg_flg[20];  /* Simulink.Signal object 'Hand_StartChg_flg' */
extern uint8_T Hand_StopChg_flg[20];   /* Simulink.Signal object 'Hand_StopChg_flg' */
extern uint8_T Locat_Book_num[3];      /* Simulink.Signal object 'Locat_Book_num' */
extern uint8_T MCE_AllBag_num;         /* Simulink.Signal object 'MCE_AllBag_num' */
extern uint8_T MCE_AllErr_Lev;         /* Simulink.Signal object 'MCE_AllErr_Lev' */
extern uint8_T MCE_BatOutOrder_err;    /* Simulink.Signal object 'MCE_BatOutOrder_err' */
extern uint8_T MCE_BatOut_CarTy;       /* Simulink.Signal object 'MCE_BatOut_CarTy' */
extern uint8_T MCE_BatOut_sour;        /* Simulink.Signal object 'MCE_BatOut_sour' */
extern uint8_T MCE_CBLocator_CarTy;    /* Simulink.Signal object 'MCE_CBLocator_CarTy' */
extern uint8_T MCE_CBLocator_sour;     /* Simulink.Signal object 'MCE_CBLocator_sour' */
extern uint8_T MCE_CELocatorOrder_err; /* Simulink.Signal object 'MCE_CELocatorOrder_err' */
extern uint8_T MCE_CELocator_CarTy;    /* Simulink.Signal object 'MCE_CELocator_CarTy' */
extern uint8_T MCE_CELocator_dest;     /* Simulink.Signal object 'MCE_CELocator_dest' */
extern uint8_T MCE_CELocator_sour;     /* Simulink.Signal object 'MCE_CELocator_sour' */
extern uint8_T MCE_ChangeBatOrder_err; /* Simulink.Signal object 'MCE_ChangeBatOrder_err' */
extern uint8_T MCE_CheckBatCode_err;   /* Simulink.Signal object 'MCE_CheckBatCode_err' */
extern uint8_T MCE_Comm_err;           /* Simulink.Signal object 'MCE_Comm_err' */
extern uint8_T MCE_Comm_flg;           /* Simulink.Signal object 'MCE_Comm_flg' */
extern uint8_T MCE_ContinueOrder_err;  /* Simulink.Signal object 'MCE_ContinueOrder_err' */
extern uint8_T MCE_Continue_dest;      /* Simulink.Signal object 'MCE_Continue_dest' */
extern uint8_T MCE_Continue_sour;      /* Simulink.Signal object 'MCE_Continue_sour' */
extern uint8_T MCE_ControlMode;        /* Simulink.Signal object 'MCE_ControlMode' */
extern uint8_T MCE_DismountBatOrder_err;/* Simulink.Signal object 'MCE_DismountBatOrder_err' */
extern uint8_T MCE_DismountBat_CarTy;  /* Simulink.Signal object 'MCE_DismountBat_CarTy' */
extern uint8_T MCE_EndThisTime_flg;    /* Simulink.Signal object 'MCE_EndThisTime_flg' */
extern uint8_T MCE_ErrData;            /* Simulink.Signal object 'MCE_ErrData' */
extern uint8_T MCE_ErrResetOrder_err;  /* Simulink.Signal object 'MCE_ErrResetOrder_err' */
extern uint8_T MCE_ErrReset_dest;      /* Simulink.Signal object 'MCE_ErrReset_dest' */
extern uint8_T MCE_ErrReset_sour;      /* Simulink.Signal object 'MCE_ErrReset_sour' */
extern uint8_T MCE_GetBatOutOk_flg;    /* Simulink.Signal object 'MCE_GetBatOutOk_flg' */
extern uint8_T MCE_GetBatOut_flg;      /* Simulink.Signal object 'MCE_GetBatOut_flg' */
extern uint8_T MCE_GetCBOk_flg;        /* Simulink.Signal object 'MCE_GetCBOk_flg' */
extern uint8_T MCE_GetCELocatorOk_flg; /* Simulink.Signal object 'MCE_GetCELocatorOk_flg' */
extern uint8_T MCE_GetCELocator_flg;   /* Simulink.Signal object 'MCE_GetCELocator_flg' */
extern uint8_T MCE_GetChangeBat_flg;   /* Simulink.Signal object 'MCE_GetChangeBat_flg' */
extern uint8_T MCE_GetContinue_flg;    /* Simulink.Signal object 'MCE_GetContinue_flg' */
extern uint8_T MCE_GetDismountBatOk_flg;/* Simulink.Signal object 'MCE_GetDismountBatOk_flg' */
extern uint8_T MCE_GetDismountBat_flg; /* Simulink.Signal object 'MCE_GetDismountBat_flg' */
extern uint8_T MCE_GetErrReset_flg;    /* Simulink.Signal object 'MCE_GetErrReset_flg' */
extern uint8_T MCE_GetInLocatorOk_flg; /* Simulink.Signal object 'MCE_GetInLocatorOk_flg' */
extern uint8_T MCE_GetInLocator_flg;   /* Simulink.Signal object 'MCE_GetInLocator_flg' */
extern uint8_T MCE_GetInit_flg;        /* Simulink.Signal object 'MCE_GetInit_flg' */
extern uint8_T MCE_GetMountBatOk_flg;  /* Simulink.Signal object 'MCE_GetMountBatOk_flg' */
extern uint8_T MCE_GetMountBat_flg;    /* Simulink.Signal object 'MCE_GetMountBat_flg' */
extern uint8_T MCE_GetOutLocatorOk_flg;/* Simulink.Signal object 'MCE_GetOutLocatorOk_flg' */
extern uint8_T MCE_GetOutLocator_flg;  /* Simulink.Signal object 'MCE_GetOutLocator_flg' */
extern uint8_T MCE_GetSqlInt_sta;      /* Simulink.Signal object 'MCE_GetSqlInt_sta' */
extern uint8_T MCE_GetStop_flg;        /* Simulink.Signal object 'MCE_GetStop_flg' */
extern uint8_T MCE_GetTimeOut_flg;     /* Simulink.Signal object 'MCE_GetTimeOut_flg' */
extern uint8_T MCE_InLocatorOrder_err; /* Simulink.Signal object 'MCE_InLocatorOrder_err' */
extern uint8_T MCE_InLocator_CarTy;    /* Simulink.Signal object 'MCE_InLocator_CarTy' */
extern uint8_T MCE_InLocator_dest;     /* Simulink.Signal object 'MCE_InLocator_dest' */
extern uint8_T MCE_InPlacSens_flg;     /* Simulink.Signal object 'MCE_InPlacSens_flg' */
extern uint8_T MCE_InitDealOrder_err;  /* Simulink.Signal object 'MCE_InitDealOrder_err' */
extern uint8_T MCE_Initok_err;         /* Simulink.Signal object 'MCE_Initok_err' */
extern uint8_T MCE_Initok_flg;         /* Simulink.Signal object 'MCE_Initok_flg' */
extern uint8_T MCE_MountBatOrder_err;  /* Simulink.Signal object 'MCE_MountBatOrder_err' */
extern uint8_T MCE_MountBat_CarTy;     /* Simulink.Signal object 'MCE_MountBat_CarTy' */
extern uint8_T MCE_Order_err;          /* Simulink.Signal object 'MCE_Order_err' */
extern uint8_T MCE_OutLocatorOrder_err;/* Simulink.Signal object 'MCE_OutLocatorOrder_err' */
extern uint8_T MCE_OutLocator_CarTy;   /* Simulink.Signal object 'MCE_OutLocator_CarTy' */
extern uint8_T MCE_OutLocator_sour;    /* Simulink.Signal object 'MCE_OutLocator_sour' */
extern uint8_T MCE_ParkPlatStepData;   /* Simulink.Signal object 'MCE_ParkPlatStepData' */
extern uint8_T MCE_RGVSenser_sta;      /* Simulink.Signal object 'MCE_RGVSenser_sta' */
extern uint8_T MCE_RGVStepData;        /* Simulink.Signal object 'MCE_RGVStepData' */
extern uint8_T MCE_RunState;           /* Simulink.Signal object 'MCE_RunState' */
extern uint8_T MCE_SelfCE_Ok;          /* Simulink.Signal object 'MCE_SelfCE_Ok' */
extern uint8_T MCE_SelfCe_CarTy;       /* Simulink.Signal object 'MCE_SelfCe_CarTy' */
extern uint8_T MCE_SelfCheckOk_sta;    /* Simulink.Signal object 'MCE_SelfCheckOk_sta' */
extern uint8_T MCE_StackerSenser_sta;  /* Simulink.Signal object 'MCE_StackerSenser_sta' */
extern uint8_T MCE_StackerStepData;    /* Simulink.Signal object 'MCE_StackerStepData' */
extern uint8_T MCE_StartCe_flg;        /* Simulink.Signal object 'MCE_StartCe_flg' */
extern uint8_T MCE_StopOrder_err;      /* Simulink.Signal object 'MCE_StopOrder_err' */
extern uint8_T MCE_Stop_dest;          /* Simulink.Signal object 'MCE_Stop_dest' */
extern uint8_T MCE_Stop_sour;          /* Simulink.Signal object 'MCE_Stop_sour' */
extern uint8_T MCE_TimeOutOrder_err;   /* Simulink.Signal object 'MCE_TimeOutOrder_err' */
extern uint8_T MCE_TimeOut_dest;       /* Simulink.Signal object 'MCE_TimeOut_dest' */
extern uint8_T MCE_TimeOut_sour;       /* Simulink.Signal object 'MCE_TimeOut_sour' */
extern uint8_T MCE_WriSqlShutDown_sta; /* Simulink.Signal object 'MCE_WriSqlShutDown_sta' */
extern uint8_T MChar_BatCodeCheck_ok;  /* Simulink.Signal object 'MChar_BatCodeCheck_ok' */
extern uint8_T MChar_SelfCe_sour;      /* Simulink.Signal object 'MChar_SelfCe_sour' */
extern uint8_T MChar_SelfCe_sour_old;  /* Simulink.Signal object 'MChar_SelfCe_sour_old' */
extern uint8_T MChar_StartCE_flg[30];  /* Simulink.Signal object 'MChar_StartCE_flg' */
extern uint8_T MChg_ErrStop_code[20];  /* Simulink.Signal object 'MChg_ErrStop_code' */
extern uint8_T MCloud_CESta_sta;       /* Simulink.Signal object 'MCloud_CESta_sta' */
extern uint8_T MCloud_CancelOrd_ok;    /* Simulink.Signal object 'MCloud_CancelOrd_ok' */
extern uint8_T MCloud_CarAllowIn_err;  /* Simulink.Signal object 'MCloud_CarAllowIn_err' */
extern uint8_T MCloud_CarAllowIn_sta;  /* Simulink.Signal object 'MCloud_CarAllowIn_sta' */
extern uint8_T MCloud_CarAllowOut_err; /* Simulink.Signal object 'MCloud_CarAllowOut_err' */
extern uint8_T MCloud_CarAllowOut_sta; /* Simulink.Signal object 'MCloud_CarAllowOut_sta' */
extern uint8_T MCloud_CarOutReqODO_flg;/* Simulink.Signal object 'MCloud_CarOutReqODO_flg' */
extern uint8_T MCloud_Comm_err;        /* Simulink.Signal object 'MCloud_Comm_err' */
extern uint8_T MCloud_Comm_flg;        /* Simulink.Signal object 'MCloud_Comm_flg' */
extern uint8_T MCloud_GetCarODO_flg;   /* Simulink.Signal object 'MCloud_GetCarODO_flg' */
extern uint8_T MCloud_GetCarType_flg;  /* Simulink.Signal object 'MCloud_GetCarType_flg' */
extern uint8_T MCloud_SendCEData_flg;  /* Simulink.Signal object 'MCloud_SendCEData_flg' */
extern uint8_T MCloud_SendCancelOrd_flg;/* Simulink.Signal object 'MCloud_SendCancelOrd_flg' */
extern uint8_T MCloud_SendCarCard_flg; /* Simulink.Signal object 'MCloud_SendCarCard_flg' */
extern uint8_T MCloud_SendCarODO_flg;  /* Simulink.Signal object 'MCloud_SendCarODO_flg' */
extern uint8_T MCloud_SendCarOutOf_flg;/* Simulink.Signal object 'MCloud_SendCarOutOf_flg' */
extern uint8_T MCloud_SendGetCarType_flg;/* Simulink.Signal object 'MCloud_SendGetCarType_flg' */
extern uint8_T MCloud_SendmatchBatNo_flg;/* Simulink.Signal object 'MCloud_SendmatchBatNo_flg' */
extern uint8_T MFan_Start_flg;         /* Simulink.Signal object 'MFan_Start_flg' */
extern uint8_T MGrab_Comm_err;         /* Simulink.Signal object 'MGrab_Comm_err' */
extern uint8_T MGrab_Comm_flg;         /* Simulink.Signal object 'MGrab_Comm_flg' */
extern uint8_T MIO_Comm_err;           /* Simulink.Signal object 'MIO_Comm_err' */
extern uint8_T MIO_Comm_flg;           /* Simulink.Signal object 'MIO_Comm_flg' */
extern uint8_T MIO_DoorErr_buf[3];     /* Simulink.Signal object 'MIO_DoorErr_buf' */
extern uint8_T MIO_DoorValue_null[3];  /* Simulink.Signal object 'MIO_DoorValue_null' */
extern uint8_T MIO_Door_err;           /* Simulink.Signal object 'MIO_Door_err' */
extern uint8_T MIO_FireContr_err;      /* Simulink.Signal object 'MIO_FireContr_err' */
extern uint8_T MIO_LightValue_null[2]; /* Simulink.Signal object 'MIO_LightValue_null' */
extern uint8_T MIO_Light_err;          /* Simulink.Signal object 'MIO_Light_err' */
extern uint8_T MIO_LightrErr_buf[2];   /* Simulink.Signal object 'MIO_LightrErr_buf' */
extern uint8_T MIO_ThunderErr_buf[2];  /* Simulink.Signal object 'MIO_ThunderErr_buf' */
extern uint8_T MIO_ThunderValue_null[2];/* Simulink.Signal object 'MIO_ThunderValue_null' */
extern uint8_T MIO_Thunder_err;        /* Simulink.Signal object 'MIO_Thunder_err' */
extern uint8_T MIO_WaterValue_null;    /* Simulink.Signal object 'MIO_WaterValue_null' */
extern uint8_T MIO_Water_err;          /* Simulink.Signal object 'MIO_Water_err' */
extern uint8_T MLed_Big_sta;           /* Simulink.Signal object 'MLed_Big_sta' */
extern uint8_T MLed_Comm_flg;          /* Simulink.Signal object 'MLed_Comm_flg' */
extern uint8_T MLed_FireErrCB_flg;     /* Simulink.Signal object 'MLed_FireErrCB_flg' */
extern uint8_T MLed_FireErrGo_flg;     /* Simulink.Signal object 'MLed_FireErrGo_flg' */
extern uint8_T MLed_PlayCBErr_flg;     /* Simulink.Signal object 'MLed_PlayCBErr_flg' */
extern uint8_T MLed_PlayCBing_flg;     /* Simulink.Signal object 'MLed_PlayCBing_flg' */
extern uint8_T MLed_PlayCancelOrder_flg;/* Simulink.Signal object 'MLed_PlayCancelOrder_flg' */
extern uint8_T MLed_PlayCarNoInnet_flg;/* Simulink.Signal object 'MLed_PlayCarNoInnet_flg' */
extern uint8_T MLed_PlayCloudNoIn_flg; /* Simulink.Signal object 'MLed_PlayCloudNoIn_flg' */
extern uint8_T MLed_PlayErrGo_flg;     /* Simulink.Signal object 'MLed_PlayErrGo_flg' */
extern uint8_T MLed_PlayInPlac_flg;    /* Simulink.Signal object 'MLed_PlayInPlac_flg' */
extern uint8_T MLed_PlayIn_flg;        /* Simulink.Signal object 'MLed_PlayIn_flg' */
extern uint8_T MLed_PlayInitErr_flg;   /* Simulink.Signal object 'MLed_PlayInitErr_flg' */
extern uint8_T MLed_PlayNoBag_flg;     /* Simulink.Signal object 'MLed_PlayNoBag_flg' */
extern uint8_T MLed_PlayOut_flg;       /* Simulink.Signal object 'MLed_PlayOut_flg' */
extern uint8_T MLed_PlayStartCB_flg;   /* Simulink.Signal object 'MLed_PlayStartCB_flg' */
extern uint8_T MLed_Play_sta;          /* Simulink.Signal object 'MLed_Play_sta' */
extern uint8_T MLed_PleaseGo_flg;      /* Simulink.Signal object 'MLed_PleaseGo_flg' */
extern uint8_T MLed_ShowFree_flg;      /* Simulink.Signal object 'MLed_ShowFree_flg' */
extern uint8_T MLed_Start10s_Num;      /* Simulink.Signal object 'MLed_Start10s_Num' */
extern uint8_T MLed_Start10s_flg;      /* Simulink.Signal object 'MLed_Start10s_flg' */
extern uint8_T MLight_Start_flg;       /* Simulink.Signal object 'MLight_Start_flg' */
extern uint8_T MLoc_Deal_err;          /* Simulink.Signal object 'MLoc_Deal_err' */
extern uint8_T MLocat_AllowCESoc_pct;  /* Simulink.Signal object 'MLocat_AllowCESoc_pct' */
extern uint8_T MLocat_BatCode_null[30];/* Simulink.Signal object 'MLocat_BatCode_null' */
extern uint8_T MLocat_BookLock_flg[20];/* Simulink.Signal object 'MLocat_BookLock_flg' */
extern uint8_T MLocat_Car_type[20];    /* Simulink.Signal object 'MLocat_Car_type' */
extern uint8_T MLocat_CharEndSoc_pct;  /* Simulink.Signal object 'MLocat_CharEndSoc_pct' */
extern uint8_T MLocat_CharPow_kw;      /* Simulink.Signal object 'MLocat_CharPow_kw' */
extern uint8_T MLocat_Charging_flg[20];/* Simulink.Signal object 'MLocat_Charging_flg' */
extern uint8_T MLocat_CheckClear_flg;  /* Simulink.Signal object 'MLocat_CheckClear_flg' */
extern uint8_T MLocat_CheckInNet_ok;   /* Simulink.Signal object 'MLocat_CheckInNet_ok' */
extern uint8_T MLocat_CheckLock_flg;   /* Simulink.Signal object 'MLocat_CheckLock_flg' */
extern uint8_T MLocat_Conf_ena[20];    /* Simulink.Signal object 'MLocat_Conf_ena' */
extern uint8_T MLocat_ErrReset_flg[20];/* Simulink.Signal object 'MLocat_ErrReset_flg' */
extern uint8_T MLocat_FIFO_Buf[20];    /* Simulink.Signal object 'MLocat_FIFO_Buf' */
extern uint8_T MLocat_GetBatCode_flg[20];/* Simulink.Signal object 'MLocat_GetBatCode_flg' */
extern uint8_T MLocat_InNet_flg[20];   /* Simulink.Signal object 'MLocat_InNet_flg' */
extern uint8_T MLocat_RealBook_num[3]; /* Simulink.Signal object 'MLocat_RealBook_num' */
extern uint8_T MLocat_SelfChar_ena[20];/* Simulink.Signal object 'MLocat_SelfChar_ena' */
extern uint8_T MLocat_Soc_pct[20];     /* Simulink.Signal object 'MLocat_Soc_pct' */
extern uint8_T MQt_ClearInPlace_flg;   /* Simulink.Signal object 'MQt_ClearInPlace_flg' */
extern uint8_T MQt_ConfDa_push;        /* Simulink.Signal object 'MQt_ConfDa_push' */
extern uint8_T MQt_DisaSoundLigErr_flg;/* Simulink.Signal object 'MQt_DisaSoundLigErr_flg' */
extern uint8_T MQt_GetCancelOrd_flg;   /* Simulink.Signal object 'MQt_GetCancelOrd_flg' */
extern uint8_T MQt_GetHandCBOk_flg;    /* Simulink.Signal object 'MQt_GetHandCBOk_flg' */
extern uint8_T MQt_GetInPlace_flg;     /* Simulink.Signal object 'MQt_GetInPlace_flg' */
extern uint8_T MQt_GetODOValue_flg;    /* Simulink.Signal object 'MQt_GetODOValue_flg' */
extern uint8_T MQt_GetPointBack_flg;   /* Simulink.Signal object 'MQt_GetPointBack_flg' */
extern uint8_T MQt_GetSql_flg;         /* Simulink.Signal object 'MQt_GetSql_flg' */
extern uint8_T MQt_NoBag_flg;          /* Simulink.Signal object 'MQt_NoBag_flg' */
extern uint8_T MQt_SendAirData_flg;    /* Simulink.Signal object 'MQt_SendAirData_flg' */
extern uint8_T MQt_SendCEData_flg;     /* Simulink.Signal object 'MQt_SendCEData_flg' */
extern uint8_T MQt_SendChgData1_flg;   /* Simulink.Signal object 'MQt_SendChgData1_flg' */
extern uint8_T MQt_SendChgData2_flg;   /* Simulink.Signal object 'MQt_SendChgData2_flg' */
extern uint8_T MQt_SendChgerErr_flg;   /* Simulink.Signal object 'MQt_SendChgerErr_flg' */
extern uint8_T MQt_SendDcEne2_flg;     /* Simulink.Signal object 'MQt_SendDcEne2_flg' */
extern uint8_T MQt_SendDcEne_flg;      /* Simulink.Signal object 'MQt_SendDcEne_flg' */
extern uint8_T MQt_SendInPlace_flg;    /* Simulink.Signal object 'MQt_SendInPlace_flg' */
extern uint8_T MQt_SendLocatData_flg;  /* Simulink.Signal object 'MQt_SendLocatData_flg' */
extern uint8_T MQt_SendPoint_flg;      /* Simulink.Signal object 'MQt_SendPoint_flg' */
extern uint8_T MQt_StartSelfCE_sta;    /* Simulink.Signal object 'MQt_StartSelfCE_sta' */
extern uint8_T MRemo_CELocator_ok;     /* Simulink.Signal object 'MRemo_CELocator_ok' */
extern uint8_T MRemo_ChangeBat_ok;     /* Simulink.Signal object 'MRemo_ChangeBat_ok' */
extern uint8_T MRemo_GetInPlace_flg;   /* Simulink.Signal object 'MRemo_GetInPlace_flg' */
extern uint8_T MRemo_GetSelfCE_flg;    /* Simulink.Signal object 'MRemo_GetSelfCE_flg' */
extern uint8_T MSound_Start_flg;       /* Simulink.Signal object 'MSound_Start_flg' */
extern uint8_T MSql_DelUnfinishedOrder_flg;/* Simulink.Signal object 'MSql_DelUnfinishedOrder_flg' */
extern uint8_T MSql_WriCeOkData_flg;   /* Simulink.Signal object 'MSql_WriCeOkData_flg' */
extern uint8_T MSql_WriCeStartData_flg;/* Simulink.Signal object 'MSql_WriCeStartData_flg' */
extern uint8_T MSql_WriUnfinishedOrder_flg;/* Simulink.Signal object 'MSql_WriUnfinishedOrder_flg' */
extern uint8_T MSqlite_WriAcEnergy_flg;/* Simulink.Signal object 'MSqlite_WriAcEnergy_flg' */
extern uint8_T MSqlite_WriAirConData_flg;/* Simulink.Signal object 'MSqlite_WriAirConData_flg' */
extern uint8_T MSqlite_WriBattData_flg[20];/* Simulink.Signal object 'MSqlite_WriBattData_flg' */
extern uint8_T MSqlite_WriCEData_flg;  /* Simulink.Signal object 'MSqlite_WriCEData_flg' */
extern uint8_T MSqlite_WriCharData_flg[20];/* Simulink.Signal object 'MSqlite_WriCharData_flg' */
extern uint8_T MSqlite_WriConfigData_flg;/* Simulink.Signal object 'MSqlite_WriConfigData_flg' */
extern uint8_T MSqlite_WriErrData_flg; /* Simulink.Signal object 'MSqlite_WriErrData_flg' */
extern uint8_T MSqlite_WriLocatData_flg[20];/* Simulink.Signal object 'MSqlite_WriLocatData_flg' */
extern uint8_T MSqlite_WrtErr_flg;     /* Simulink.Signal object 'MSqlite_WrtErr_flg' */
extern uint8_T MSys_AllErr_Lev;        /* Simulink.Signal object 'MSys_AllErr_Lev' */
extern uint8_T Mnet_WriCeOkData_flg;   /* Simulink.Signal object 'Mnet_WriCeOkData_flg' */
extern uint8_T Mnet_WriCeStartData_flg;/* Simulink.Signal object 'Mnet_WriCeStartData_flg' */
extern uint8_T PLC_Die_flg;            /* Simulink.Signal object 'PLC_Die_flg' */
extern uint8_T QT_BatOutOrder_flg;     /* Simulink.Signal object 'QT_BatOutOrder_flg' */
extern uint8_T QT_CELocatorOrder_flg;  /* Simulink.Signal object 'QT_CELocatorOrder_flg' */
extern uint8_T QT_ChangeBatOrder_flg;  /* Simulink.Signal object 'QT_ChangeBatOrder_flg' */
extern uint8_T QT_ContinueOrder_flg;   /* Simulink.Signal object 'QT_ContinueOrder_flg' */
extern uint8_T QT_DismountBatOrder_flg;/* Simulink.Signal object 'QT_DismountBatOrder_flg' */
extern uint8_T QT_ErrResetOrder_flg;   /* Simulink.Signal object 'QT_ErrResetOrder_flg' */
extern uint8_T QT_HandCE_flg;          /* Simulink.Signal object 'QT_HandCE_flg' */
extern uint8_T QT_InLocatorOrder_flg;  /* Simulink.Signal object 'QT_InLocatorOrder_flg' */
extern uint8_T QT_InitOrder_flg;       /* Simulink.Signal object 'QT_InitOrder_flg' */
extern uint8_T QT_MountBatOrder_flg;   /* Simulink.Signal object 'QT_MountBatOrder_flg' */
extern uint8_T QT_OutLocatorOrder_flg; /* Simulink.Signal object 'QT_OutLocatorOrder_flg' */
extern uint8_T QT_SlefCE_flg;          /* Simulink.Signal object 'QT_SlefCE_flg' */
extern uint8_T QT_StopOrder_flg;       /* Simulink.Signal object 'QT_StopOrder_flg' */
extern uint8_T QT_TimeOutOrder_flg;    /* Simulink.Signal object 'QT_TimeOutOrder_flg' */
extern uint8_T Qt_CarAllowOut_sta;     /* Simulink.Signal object 'Qt_CarAllowOut_sta' */
extern uint8_T Qt_HandCharPow_kw[20];  /* Simulink.Signal object 'Qt_HandCharPow_kw' */
extern uint8_T RemotSend_Heart_flg;    /* Simulink.Signal object 'RemotSend_Heart_flg' */
extern uint8_T RemotSend_UpACEngData_flg;/* Simulink.Signal object 'RemotSend_UpACEngData_flg' */
extern uint8_T RemotSend_UpAirConData_flg;/* Simulink.Signal object 'RemotSend_UpAirConData_flg' */
extern uint8_T RemotSend_UpCEData_flg; /* Simulink.Signal object 'RemotSend_UpCEData_flg' */
extern uint8_T RemotSend_UpCEingData_flg;/* Simulink.Signal object 'RemotSend_UpCEingData_flg' */
extern uint8_T RemotSend_UpCHUData_flg;/* Simulink.Signal object 'RemotSend_UpCHUData_flg' */
extern uint8_T RemotSend_UpErrCHUData_flg;/* Simulink.Signal object 'RemotSend_UpErrCHUData_flg' */
extern uint8_T RemotSend_UpFanData_flg;/* Simulink.Signal object 'RemotSend_UpFanData_flg' */
extern uint8_T RemotSend_UpFireData_flg;/* Simulink.Signal object 'RemotSend_UpFireData_flg' */
extern uint8_T RemotSend_UpPLCData_flg;/* Simulink.Signal object 'RemotSend_UpPLCData_flg' */
extern uint8_T Remot_conn_flg;         /* Simulink.Signal object 'Remot_conn_flg' */
extern uint8_T Send_CE_BATBAGOUT_flg;  /* Simulink.Signal object 'Send_CE_BATBAGOUT_flg' */
extern uint8_T Send_CE_CHANGEBAT_flg;  /* Simulink.Signal object 'Send_CE_CHANGEBAT_flg' */
extern uint8_T Send_CE_CHANGELOCATOR_flg;/* Simulink.Signal object 'Send_CE_CHANGELOCATOR_flg' */
extern uint8_T Send_CE_CONTINUE_flg;   /* Simulink.Signal object 'Send_CE_CONTINUE_flg' */
extern uint8_T Send_CE_DISMOUNTBAT_flg;/* Simulink.Signal object 'Send_CE_DISMOUNTBAT_flg' */
extern uint8_T Send_CE_ERRRESET_flg;   /* Simulink.Signal object 'Send_CE_ERRRESET_flg' */
extern uint8_T Send_CE_INIT_flg;       /* Simulink.Signal object 'Send_CE_INIT_flg' */
extern uint8_T Send_CE_INLOCATOR_flg;  /* Simulink.Signal object 'Send_CE_INLOCATOR_flg' */
extern uint8_T Send_CE_MOUNTBAT_flg;   /* Simulink.Signal object 'Send_CE_MOUNTBAT_flg' */
extern uint8_T Send_CE_OUTLOCATOR_flg; /* Simulink.Signal object 'Send_CE_OUTLOCATOR_flg' */
extern uint8_T Send_CE_STOP_flg;       /* Simulink.Signal object 'Send_CE_STOP_flg' */
extern uint8_T Send_CE_TIMEOUT_flg;    /* Simulink.Signal object 'Send_CE_TIMEOUT_flg' */
extern uint8_T SetPwr_timeout[20];     /* Simulink.Signal object 'SetPwr_timeout' */
extern uint8_T Sta;                    /* Simulink.Signal object 'Sta' */
extern uint8_T StartChg_timeout[20];   /* Simulink.Signal object 'StartChg_timeout' */
extern uint8_T StopChg_timeout[20];    /* Simulink.Signal object 'StopChg_timeout' */
extern uint8_T Sys_Big_sta;            /* Simulink.Signal object 'Sys_Big_sta' */
extern uint8_T Sys_sta[20];            /* Simulink.Signal object 'Sys_sta' */
extern uint8_T chu_test_state[20];     /* Simulink.Signal object 'chu_test_state' */
extern uint8_T index_Chu;              /* Simulink.Signal object 'index_Chu' */
extern uint8_T index_Err;              /* Simulink.Signal object 'index_Err' */
extern uint8_T mdl_CEPrepareSig_flg[20];/* Simulink.Signal object 'mdl_CEPrepareSig_flg' */
extern uint8_T stop_ok[20];            /* Simulink.Signal object 'stop_ok' */

/* Model entry point functions */
extern void SA4_EMS_initialize(void);
extern void SA4_EMS_step(void);

/* Real-time Model object */
extern RT_MODEL *const rtM;

/*-
 * The generated code includes comments that allow you to trace directly
 * back to the appropriate location in the model.  The basic format
 * is <system>/block_name, where system is the system number (uniquely
 * assigned by Simulink) and block_name is the name of the block.
 *
 * Use the MATLAB hilite_system command to trace the generated code back
 * to the model.  For example,
 *
 * hilite_system('<S3>')    - opens system 3
 * hilite_system('<S3>/Kp') - opens and selects block Kp which resides in S3
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'SA4_EMS'
 * '<S1>'   : 'SA4_EMS/Chart'
 * '<S2>'   : 'SA4_EMS/Chart1'
 * '<S3>'   : 'SA4_EMS/Subsystem'
 * '<S4>'   : 'SA4_EMS/Subsystem/Chart1'
 * '<S5>'   : 'SA4_EMS/Subsystem/Chart10'
 * '<S6>'   : 'SA4_EMS/Subsystem/Chart11'
 * '<S7>'   : 'SA4_EMS/Subsystem/Chart12'
 * '<S8>'   : 'SA4_EMS/Subsystem/Chart13'
 * '<S9>'   : 'SA4_EMS/Subsystem/Chart14'
 * '<S10>'  : 'SA4_EMS/Subsystem/Chart15'
 * '<S11>'  : 'SA4_EMS/Subsystem/Chart16'
 * '<S12>'  : 'SA4_EMS/Subsystem/Chart17'
 * '<S13>'  : 'SA4_EMS/Subsystem/Chart18'
 * '<S14>'  : 'SA4_EMS/Subsystem/Chart19'
 * '<S15>'  : 'SA4_EMS/Subsystem/Chart2'
 * '<S16>'  : 'SA4_EMS/Subsystem/Chart20'
 * '<S17>'  : 'SA4_EMS/Subsystem/Chart3'
 * '<S18>'  : 'SA4_EMS/Subsystem/Chart4'
 * '<S19>'  : 'SA4_EMS/Subsystem/Chart5'
 * '<S20>'  : 'SA4_EMS/Subsystem/Chart6'
 * '<S21>'  : 'SA4_EMS/Subsystem/Chart7'
 * '<S22>'  : 'SA4_EMS/Subsystem/Chart8'
 * '<S23>'  : 'SA4_EMS/Subsystem/Chart9'
 */
#endif                                 /* RTW_HEADER_SA4_EMS_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
