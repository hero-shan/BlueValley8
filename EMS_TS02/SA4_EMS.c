/*
 * File: SA4_EMS.c
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

#include "SA4_EMS.h"

/* Named constants for Chart: '<S3>/Chart1' */
#define IN_CHU_LowPwrDown_timeout      ((uint8_T)1U)
#define IN_CHU_LowPwrOn_timeout        ((uint8_T)2U)
#define IN_CHU_StartChg_timeout        ((uint8_T)1U)
#define IN_CHU_StopChg_timeout         ((uint8_T)2U)
#define IN_GetBattBasicData0           ((uint8_T)1U)
#define IN_GetBattBasicData1           ((uint8_T)2U)
#define IN_GetBattRtData0              ((uint8_T)1U)
#define IN_GetBattRtData1              ((uint8_T)2U)
#define IN_GetCellTemp0                ((uint8_T)1U)
#define IN_GetCellTemp1                ((uint8_T)2U)
#define IN_GetCellVol0                 ((uint8_T)1U)
#define IN_GetCellVol1                 ((uint8_T)2U)
#define IN_GetCharPreInf0              ((uint8_T)1U)
#define IN_GetCharPreInf1              ((uint8_T)2U)
#define IN_Init                        ((uint8_T)3U)
#define IN_LowPwrDown                  ((uint8_T)4U)
#define IN_LowPwrDown_ok               ((uint8_T)5U)
#define IN_LowPwrOn                    ((uint8_T)6U)
#define IN_NO_ACTIVE_CHILD             ((uint8_T)0U)
#define IN_SetPwr                      ((uint8_T)3U)
#define IN_StartChg                    ((uint8_T)4U)
#define IN_StartChg_Fault              ((uint8_T)5U)
#define IN_StopChg                     ((uint8_T)6U)
#define IN_charging_state              ((uint8_T)7U)
#define IN_free                        ((uint8_T)7U)
#define IN_free1                       ((uint8_T)8U)
#define IN_stop_ok3                    ((uint8_T)9U)

/* Named constants for Chart: '<Root>/Chart' */
#define IN_All_Self_Check              ((uint8_T)1U)
#define IN_Allow_InStation             ((uint8_T)2U)
#define IN_Car_In                      ((uint8_T)3U)
#define IN_ChangeBatt                  ((uint8_T)4U)
#define IN_Cloud_Pass                  ((uint8_T)5U)
#define IN_Init1                       ((uint8_T)2U)
#define IN_Init2                       ((uint8_T)2U)
#define IN_Init2_k                     ((uint8_T)3U)
#define IN_Init3                       ((uint8_T)3U)
#define IN_Init3_b                     ((uint8_T)6U)
#define IN_Init3_bc                    ((uint8_T)1U)
#define IN_Init3_bci                   ((uint8_T)4U)
#define IN_Init3_bci2                  ((uint8_T)2U)
#define IN_Init4                       ((uint8_T)4U)
#define IN_Init4_m                     ((uint8_T)2U)
#define IN_Init4_m2                    ((uint8_T)5U)
#define IN_Init4_m2t                   ((uint8_T)3U)
#define IN_Init5                       ((uint8_T)5U)
#define IN_Init5_b                     ((uint8_T)6U)
#define IN_Init5_br                    ((uint8_T)4U)
#define IN_Init6                       ((uint8_T)6U)
#define IN_Init6_o                     ((uint8_T)7U)
#define IN_Init6_oh                    ((uint8_T)5U)
#define IN_Init7                       ((uint8_T)7U)
#define IN_Init7_a                     ((uint8_T)6U)
#define IN_Init8                       ((uint8_T)8U)
#define IN_Init_c                      ((uint8_T)1U)
#define IN_MCE_COMM                    ((uint8_T)1U)
#define IN_MCE_InitOk                  ((uint8_T)2U)
#define IN_MCloud_COMM                 ((uint8_T)3U)
#define IN_MGrab_COMM                  ((uint8_T)4U)
#define IN_MIO_COMM                    ((uint8_T)5U)
#define IN_Main_Init                   ((uint8_T)7U)
#define IN_NO_ACTIVE_CHILD_p           ((uint8_T)0U)
#define IN_ODO_INIT                    ((uint8_T)2U)
#define IN_ODO_INIT1                   ((uint8_T)3U)
#define IN_Play_Sta1                   ((uint8_T)1U)
#define IN_Play_Sta2                   ((uint8_T)2U)
#define IN_Play_Sta3                   ((uint8_T)3U)
#define IN_Play_Sta4                   ((uint8_T)4U)
#define IN_Play_Sta4_a                 ((uint8_T)1U)
#define IN_Play_Sta5                   ((uint8_T)2U)
#define IN_Send_Init                   ((uint8_T)6U)
#define IN_Send_Init_o                 ((uint8_T)5U)
#define IN_Send_Init_oe                ((uint8_T)9U)
#define IN_Wait_Car                    ((uint8_T)8U)
#define IN_Wait_SelfMode               ((uint8_T)6U)
#define IN_dont_mount                  ((uint8_T)1U)
#define IN_mount_ok                    ((uint8_T)2U)
#define IN_mount_ok1                   ((uint8_T)3U)

/* Named constants for Chart: '<Root>/Chart1' */
#define IN_Init2_g                     ((uint8_T)2U)
#define IN_Init_l                      ((uint8_T)1U)

/* Exported block states */
real_T BMS_PackSocReal_pct[20];        /* Simulink.Signal object 'BMS_PackSocReal_pct' */
real_T Cloud_SendODO_km;               /* Simulink.Signal object 'Cloud_SendODO_km' */
real_T EMS_CHUSetPwr_kw[20];           /* Simulink.Signal object 'EMS_CHUSetPwr_kw' */
uint16_T CHU_LowPwrDown_count[20];     /* Simulink.Signal object 'CHU_LowPwrDown_count' */
uint16_T CHU_LowPwrOn_count[20];       /* Simulink.Signal object 'CHU_LowPwrOn_count' */
uint16_T GetBattBasicData_count0[20];  /* Simulink.Signal object 'GetBattBasicData_count0' */
uint16_T GetBattBasicData_count1[20];  /* Simulink.Signal object 'GetBattBasicData_count1' */
uint16_T GetBattRtData_count0[20];     /* Simulink.Signal object 'GetBattRtData_count0' */
uint16_T GetBattRtData_count1[20];     /* Simulink.Signal object 'GetBattRtData_count1' */
uint16_T GetCellTemp_count0[20];       /* Simulink.Signal object 'GetCellTemp_count0' */
uint16_T GetCellTemp_count1[20];       /* Simulink.Signal object 'GetCellTemp_count1' */
uint16_T GetCellVol_count0[20];        /* Simulink.Signal object 'GetCellVol_count0' */
uint16_T GetCellVol_count1[20];        /* Simulink.Signal object 'GetCellVol_count1' */
uint16_T GetCharEndInf_count0[20];     /* Simulink.Signal object 'GetCharEndInf_count0' */
uint16_T GetCharEndInf_count1[20];     /* Simulink.Signal object 'GetCharEndInf_count1' */
uint16_T GetCharPreInf_count0[20];     /* Simulink.Signal object 'GetCharPreInf_count0' */
uint16_T GetCharPreInf_count1[20];     /* Simulink.Signal object 'GetCharPreInf_count1' */
uint16_T Get_CHUChgRtData_count[20];   /* Simulink.Signal object 'Get_CHUChgRtData_count' */
uint16_T SetPwr_count[20];             /* Simulink.Signal object 'SetPwr_count' */
uint16_T StartChg_count[20];           /* Simulink.Signal object 'StartChg_count' */
uint16_T StopChg_count[20];            /* Simulink.Signal object 'StopChg_count' */
uint8_T AirCon_conn_flg;               /* Simulink.Signal object 'AirCon_conn_flg' */
uint8_T BATT_IsExist_flg[20];          /* Simulink.Signal object 'BATT_IsExist_flg' */
uint8_T Book_Init_flg;                 /* Simulink.Signal object 'Book_Init_flg' */
uint8_T CE_OutNetSelfCe_sour;          /* Simulink.Signal object 'CE_OutNetSelfCe_sour' */
uint8_T CHU_AllErr_flg[20];            /* Simulink.Signal object 'CHU_AllErr_flg' */
uint8_T CHU_CharFull_flg[20];          /* Simulink.Signal object 'CHU_CharFull_flg' */
uint8_T CHU_DevState_code[20];         /* Simulink.Signal object 'CHU_DevState_code' */
uint8_T CHU_ErrCode_buf0[20];          /* Simulink.Signal object 'CHU_ErrCode_buf0' */
uint8_T CHU_ErrCode_buf1[20];          /* Simulink.Signal object 'CHU_ErrCode_buf1' */
uint8_T CHU_ErrCode_buf2[20];          /* Simulink.Signal object 'CHU_ErrCode_buf2' */
uint8_T CHU_ErrCode_buf3[20];          /* Simulink.Signal object 'CHU_ErrCode_buf3' */
uint8_T CHU_GetBatBasic_flg[20];       /* Simulink.Signal object 'CHU_GetBatBasic_flg' */
uint8_T CHU_LowPwrDownErr_flg[20];     /* Simulink.Signal object 'CHU_LowPwrDownErr_flg' */
uint8_T CHU_LowPwrDown_timeout[20];    /* Simulink.Signal object 'CHU_LowPwrDown_timeout' */
uint8_T CHU_LowPwrOnErr_flg[20];       /* Simulink.Signal object 'CHU_LowPwrOnErr_flg' */
uint8_T CHU_LowPwrOn_timeout[20];      /* Simulink.Signal object 'CHU_LowPwrOn_timeout' */
uint8_T CHU_StartChgErr_flg[20];       /* Simulink.Signal object 'CHU_StartChgErr_flg' */
uint8_T CHU_StopChgErr_flg[20];        /* Simulink.Signal object 'CHU_StopChgErr_flg' */
uint8_T Chg_Control_mode[20];          /* Simulink.Signal object 'Chg_Control_mode' */
uint8_T Cloud_AppointLock_flg[20];     /* Simulink.Signal object 'Cloud_AppointLock_flg' */
uint8_T Cloud_BookCarNo_flg;           /* Simulink.Signal object 'Cloud_BookCarNo_flg' */
uint8_T Cloud_OutNetCheckBatNum_flg;   /* Simulink.Signal object 'Cloud_OutNetCheckBatNum_flg' */
uint8_T Cloud_OutNetLock_flg[20];      /* Simulink.Signal object 'Cloud_OutNetLock_flg' */
uint8_T Cloud_carType_Code;            /* Simulink.Signal object 'Cloud_carType_Code' */
uint8_T EMS_CHU_ComErr_flg[20];        /* Simulink.Signal object 'EMS_CHU_ComErr_flg' */
uint8_T EMS_Chger_BigSta[20];          /* Simulink.Signal object 'EMS_Chger_BigSta' */
uint8_T EMS_SendGetBattBasicDataCmd_flg[20];/* Simulink.Signal object 'EMS_SendGetBattBasicDataCmd_flg' */
uint8_T EMS_SendGetBattRtDataCmd_flg[20];/* Simulink.Signal object 'EMS_SendGetBattRtDataCmd_flg' */
uint8_T EMS_SendGetCellTempCmd_flg[20];/* Simulink.Signal object 'EMS_SendGetCellTempCmd_flg' */
uint8_T EMS_SendGetCellVolCmd_flg[20]; /* Simulink.Signal object 'EMS_SendGetCellVolCmd_flg' */
uint8_T EMS_SendGetCharEndInfCmd_flg[20];/* Simulink.Signal object 'EMS_SendGetCharEndInfCmd_flg' */
uint8_T EMS_SendGetCharPreInfCmd_flg[20];/* Simulink.Signal object 'EMS_SendGetCharPreInfCmd_flg' */
uint8_T EMS_SendLowPwrDownCmd_flg[20]; /* Simulink.Signal object 'EMS_SendLowPwrDownCmd_flg' */
uint8_T EMS_SendLowPwrOnCmd_flg[20];   /* Simulink.Signal object 'EMS_SendLowPwrOnCmd_flg' */
uint8_T EMS_SendSetPwrCmd_flg[20];     /* Simulink.Signal object 'EMS_SendSetPwrCmd_flg' */
uint8_T EMS_SendStartChgCmd_flg[20];   /* Simulink.Signal object 'EMS_SendStartChgCmd_flg' */
uint8_T EMS_SendStopChgCmd_flg[20];    /* Simulink.Signal object 'EMS_SendStopChgCmd_flg' */
uint8_T EMS_StopChar_flg[20];          /* Simulink.Signal object 'EMS_StopChar_flg' */
uint8_T FireContr_GetFire_flg;         /* Simulink.Signal object 'FireContr_GetFire_flg' */
uint8_T FireContr_conn_flg;            /* Simulink.Signal object 'FireContr_conn_flg' */
uint8_T Get_CHUChgRtDataErr_flg[20];   /* Simulink.Signal object 'Get_CHUChgRtDataErr_flg' */
uint8_T Get_CHUChgRtData_flg[20];      /* Simulink.Signal object 'Get_CHUChgRtData_flg' */
uint8_T Get_CHUErrStop_flg[20];        /* Simulink.Signal object 'Get_CHUErrStop_flg' */
uint8_T Grab_GetCarCard_flg;           /* Simulink.Signal object 'Grab_GetCarCard_flg' */
uint8_T Hand_LowPwrDown_flg[20];       /* Simulink.Signal object 'Hand_LowPwrDown_flg' */
uint8_T Hand_LowPwrOn_flg[20];         /* Simulink.Signal object 'Hand_LowPwrOn_flg' */
uint8_T Hand_StartChg_flg[20];         /* Simulink.Signal object 'Hand_StartChg_flg' */
uint8_T Hand_StopChg_flg[20];          /* Simulink.Signal object 'Hand_StopChg_flg' */
uint8_T Locat_Book_num[3];             /* Simulink.Signal object 'Locat_Book_num' */
uint8_T MCE_AllBag_num;                /* Simulink.Signal object 'MCE_AllBag_num' */
uint8_T MCE_AllErr_Lev;                /* Simulink.Signal object 'MCE_AllErr_Lev' */
uint8_T MCE_BatOutOrder_err;           /* Simulink.Signal object 'MCE_BatOutOrder_err' */
uint8_T MCE_BatOut_CarTy;              /* Simulink.Signal object 'MCE_BatOut_CarTy' */
uint8_T MCE_BatOut_sour;               /* Simulink.Signal object 'MCE_BatOut_sour' */
uint8_T MCE_CBLocator_CarTy;           /* Simulink.Signal object 'MCE_CBLocator_CarTy' */
uint8_T MCE_CBLocator_sour;            /* Simulink.Signal object 'MCE_CBLocator_sour' */
uint8_T MCE_CELocatorOrder_err;        /* Simulink.Signal object 'MCE_CELocatorOrder_err' */
uint8_T MCE_CELocator_CarTy;           /* Simulink.Signal object 'MCE_CELocator_CarTy' */
uint8_T MCE_CELocator_dest;            /* Simulink.Signal object 'MCE_CELocator_dest' */
uint8_T MCE_CELocator_sour;            /* Simulink.Signal object 'MCE_CELocator_sour' */
uint8_T MCE_ChangeBatOrder_err;        /* Simulink.Signal object 'MCE_ChangeBatOrder_err' */
uint8_T MCE_CheckBatCode_err;          /* Simulink.Signal object 'MCE_CheckBatCode_err' */
uint8_T MCE_Comm_err;                  /* Simulink.Signal object 'MCE_Comm_err' */
uint8_T MCE_Comm_flg;                  /* Simulink.Signal object 'MCE_Comm_flg' */
uint8_T MCE_ContinueOrder_err;         /* Simulink.Signal object 'MCE_ContinueOrder_err' */
uint8_T MCE_Continue_dest;             /* Simulink.Signal object 'MCE_Continue_dest' */
uint8_T MCE_Continue_sour;             /* Simulink.Signal object 'MCE_Continue_sour' */
uint8_T MCE_ControlMode;               /* Simulink.Signal object 'MCE_ControlMode' */
uint8_T MCE_DismountBatOrder_err;      /* Simulink.Signal object 'MCE_DismountBatOrder_err' */
uint8_T MCE_DismountBat_CarTy;         /* Simulink.Signal object 'MCE_DismountBat_CarTy' */
uint8_T MCE_EndThisTime_flg;           /* Simulink.Signal object 'MCE_EndThisTime_flg' */
uint8_T MCE_ErrData;                   /* Simulink.Signal object 'MCE_ErrData' */
uint8_T MCE_ErrResetOrder_err;         /* Simulink.Signal object 'MCE_ErrResetOrder_err' */
uint8_T MCE_ErrReset_dest;             /* Simulink.Signal object 'MCE_ErrReset_dest' */
uint8_T MCE_ErrReset_sour;             /* Simulink.Signal object 'MCE_ErrReset_sour' */
uint8_T MCE_GetBatOutOk_flg;           /* Simulink.Signal object 'MCE_GetBatOutOk_flg' */
uint8_T MCE_GetBatOut_flg;             /* Simulink.Signal object 'MCE_GetBatOut_flg' */
uint8_T MCE_GetCBOk_flg;               /* Simulink.Signal object 'MCE_GetCBOk_flg' */
uint8_T MCE_GetCELocatorOk_flg;        /* Simulink.Signal object 'MCE_GetCELocatorOk_flg' */
uint8_T MCE_GetCELocator_flg;          /* Simulink.Signal object 'MCE_GetCELocator_flg' */
uint8_T MCE_GetChangeBat_flg;          /* Simulink.Signal object 'MCE_GetChangeBat_flg' */
uint8_T MCE_GetContinue_flg;           /* Simulink.Signal object 'MCE_GetContinue_flg' */
uint8_T MCE_GetDismountBatOk_flg;      /* Simulink.Signal object 'MCE_GetDismountBatOk_flg' */
uint8_T MCE_GetDismountBat_flg;        /* Simulink.Signal object 'MCE_GetDismountBat_flg' */
uint8_T MCE_GetErrReset_flg;           /* Simulink.Signal object 'MCE_GetErrReset_flg' */
uint8_T MCE_GetInLocatorOk_flg;        /* Simulink.Signal object 'MCE_GetInLocatorOk_flg' */
uint8_T MCE_GetInLocator_flg;          /* Simulink.Signal object 'MCE_GetInLocator_flg' */
uint8_T MCE_GetInit_flg;               /* Simulink.Signal object 'MCE_GetInit_flg' */
uint8_T MCE_GetMountBatOk_flg;         /* Simulink.Signal object 'MCE_GetMountBatOk_flg' */
uint8_T MCE_GetMountBat_flg;           /* Simulink.Signal object 'MCE_GetMountBat_flg' */
uint8_T MCE_GetOutLocatorOk_flg;       /* Simulink.Signal object 'MCE_GetOutLocatorOk_flg' */
uint8_T MCE_GetOutLocator_flg;         /* Simulink.Signal object 'MCE_GetOutLocator_flg' */
uint8_T MCE_GetSqlInt_sta;             /* Simulink.Signal object 'MCE_GetSqlInt_sta' */
uint8_T MCE_GetStop_flg;               /* Simulink.Signal object 'MCE_GetStop_flg' */
uint8_T MCE_GetTimeOut_flg;            /* Simulink.Signal object 'MCE_GetTimeOut_flg' */
uint8_T MCE_InLocatorOrder_err;        /* Simulink.Signal object 'MCE_InLocatorOrder_err' */
uint8_T MCE_InLocator_CarTy;           /* Simulink.Signal object 'MCE_InLocator_CarTy' */
uint8_T MCE_InLocator_dest;            /* Simulink.Signal object 'MCE_InLocator_dest' */
uint8_T MCE_InPlacSens_flg;            /* Simulink.Signal object 'MCE_InPlacSens_flg' */
uint8_T MCE_InitDealOrder_err;         /* Simulink.Signal object 'MCE_InitDealOrder_err' */
uint8_T MCE_Initok_err;                /* Simulink.Signal object 'MCE_Initok_err' */
uint8_T MCE_Initok_flg;                /* Simulink.Signal object 'MCE_Initok_flg' */
uint8_T MCE_MountBatOrder_err;         /* Simulink.Signal object 'MCE_MountBatOrder_err' */
uint8_T MCE_MountBat_CarTy;            /* Simulink.Signal object 'MCE_MountBat_CarTy' */
uint8_T MCE_Order_err;                 /* Simulink.Signal object 'MCE_Order_err' */
uint8_T MCE_OutLocatorOrder_err;       /* Simulink.Signal object 'MCE_OutLocatorOrder_err' */
uint8_T MCE_OutLocator_CarTy;          /* Simulink.Signal object 'MCE_OutLocator_CarTy' */
uint8_T MCE_OutLocator_sour;           /* Simulink.Signal object 'MCE_OutLocator_sour' */
uint8_T MCE_ParkPlatStepData;          /* Simulink.Signal object 'MCE_ParkPlatStepData' */
uint8_T MCE_RGVSenser_sta;             /* Simulink.Signal object 'MCE_RGVSenser_sta' */
uint8_T MCE_RGVStepData;               /* Simulink.Signal object 'MCE_RGVStepData' */
uint8_T MCE_RunState;                  /* Simulink.Signal object 'MCE_RunState' */
uint8_T MCE_SelfCE_Ok;                 /* Simulink.Signal object 'MCE_SelfCE_Ok' */
uint8_T MCE_SelfCe_CarTy;              /* Simulink.Signal object 'MCE_SelfCe_CarTy' */
uint8_T MCE_SelfCheckOk_sta;           /* Simulink.Signal object 'MCE_SelfCheckOk_sta' */
uint8_T MCE_StackerSenser_sta;         /* Simulink.Signal object 'MCE_StackerSenser_sta' */
uint8_T MCE_StackerStepData;           /* Simulink.Signal object 'MCE_StackerStepData' */
uint8_T MCE_StartCe_flg;               /* Simulink.Signal object 'MCE_StartCe_flg' */
uint8_T MCE_StopOrder_err;             /* Simulink.Signal object 'MCE_StopOrder_err' */
uint8_T MCE_Stop_dest;                 /* Simulink.Signal object 'MCE_Stop_dest' */
uint8_T MCE_Stop_sour;                 /* Simulink.Signal object 'MCE_Stop_sour' */
uint8_T MCE_TimeOutOrder_err;          /* Simulink.Signal object 'MCE_TimeOutOrder_err' */
uint8_T MCE_TimeOut_dest;              /* Simulink.Signal object 'MCE_TimeOut_dest' */
uint8_T MCE_TimeOut_sour;              /* Simulink.Signal object 'MCE_TimeOut_sour' */
uint8_T MCE_WriSqlShutDown_sta;        /* Simulink.Signal object 'MCE_WriSqlShutDown_sta' */
uint8_T MChar_BatCodeCheck_ok;         /* Simulink.Signal object 'MChar_BatCodeCheck_ok' */
uint8_T MChar_SelfCe_sour;             /* Simulink.Signal object 'MChar_SelfCe_sour' */
uint8_T MChar_SelfCe_sour_old;         /* Simulink.Signal object 'MChar_SelfCe_sour_old' */
uint8_T MChar_StartCE_flg[30];         /* Simulink.Signal object 'MChar_StartCE_flg' */
uint8_T MChg_ErrStop_code[20];         /* Simulink.Signal object 'MChg_ErrStop_code' */
uint8_T MCloud_CESta_sta;              /* Simulink.Signal object 'MCloud_CESta_sta' */
uint8_T MCloud_CancelOrd_ok;           /* Simulink.Signal object 'MCloud_CancelOrd_ok' */
uint8_T MCloud_CarAllowIn_err;         /* Simulink.Signal object 'MCloud_CarAllowIn_err' */
uint8_T MCloud_CarAllowIn_sta;         /* Simulink.Signal object 'MCloud_CarAllowIn_sta' */
uint8_T MCloud_CarAllowOut_err;        /* Simulink.Signal object 'MCloud_CarAllowOut_err' */
uint8_T MCloud_CarAllowOut_sta;        /* Simulink.Signal object 'MCloud_CarAllowOut_sta' */
uint8_T MCloud_CarOutReqODO_flg;       /* Simulink.Signal object 'MCloud_CarOutReqODO_flg' */
uint8_T MCloud_Comm_err;               /* Simulink.Signal object 'MCloud_Comm_err' */
uint8_T MCloud_Comm_flg;               /* Simulink.Signal object 'MCloud_Comm_flg' */
uint8_T MCloud_GetCarODO_flg;          /* Simulink.Signal object 'MCloud_GetCarODO_flg' */
uint8_T MCloud_GetCarType_flg;         /* Simulink.Signal object 'MCloud_GetCarType_flg' */
uint8_T MCloud_SendCEData_flg;         /* Simulink.Signal object 'MCloud_SendCEData_flg' */
uint8_T MCloud_SendCancelOrd_flg;      /* Simulink.Signal object 'MCloud_SendCancelOrd_flg' */
uint8_T MCloud_SendCarCard_flg;        /* Simulink.Signal object 'MCloud_SendCarCard_flg' */
uint8_T MCloud_SendCarODO_flg;         /* Simulink.Signal object 'MCloud_SendCarODO_flg' */
uint8_T MCloud_SendCarOutOf_flg;       /* Simulink.Signal object 'MCloud_SendCarOutOf_flg' */
uint8_T MCloud_SendGetCarType_flg;     /* Simulink.Signal object 'MCloud_SendGetCarType_flg' */
uint8_T MCloud_SendmatchBatNo_flg;     /* Simulink.Signal object 'MCloud_SendmatchBatNo_flg' */
uint8_T MFan_Start_flg;                /* Simulink.Signal object 'MFan_Start_flg' */
uint8_T MGrab_Comm_err;                /* Simulink.Signal object 'MGrab_Comm_err' */
uint8_T MGrab_Comm_flg;                /* Simulink.Signal object 'MGrab_Comm_flg' */
uint8_T MIO_Comm_err;                  /* Simulink.Signal object 'MIO_Comm_err' */
uint8_T MIO_Comm_flg;                  /* Simulink.Signal object 'MIO_Comm_flg' */
uint8_T MIO_DoorErr_buf[3];            /* Simulink.Signal object 'MIO_DoorErr_buf' */
uint8_T MIO_DoorValue_null[3];         /* Simulink.Signal object 'MIO_DoorValue_null' */
uint8_T MIO_Door_err;                  /* Simulink.Signal object 'MIO_Door_err' */
uint8_T MIO_FireContr_err;             /* Simulink.Signal object 'MIO_FireContr_err' */
uint8_T MIO_LightValue_null[2];        /* Simulink.Signal object 'MIO_LightValue_null' */
uint8_T MIO_Light_err;                 /* Simulink.Signal object 'MIO_Light_err' */
uint8_T MIO_LightrErr_buf[2];          /* Simulink.Signal object 'MIO_LightrErr_buf' */
uint8_T MIO_ThunderErr_buf[2];         /* Simulink.Signal object 'MIO_ThunderErr_buf' */
uint8_T MIO_ThunderValue_null[2];      /* Simulink.Signal object 'MIO_ThunderValue_null' */
uint8_T MIO_Thunder_err;               /* Simulink.Signal object 'MIO_Thunder_err' */
uint8_T MIO_WaterValue_null;           /* Simulink.Signal object 'MIO_WaterValue_null' */
uint8_T MIO_Water_err;                 /* Simulink.Signal object 'MIO_Water_err' */
uint8_T MLed_Big_sta;                  /* Simulink.Signal object 'MLed_Big_sta' */
uint8_T MLed_Comm_flg;                 /* Simulink.Signal object 'MLed_Comm_flg' */
uint8_T MLed_FireErrCB_flg;            /* Simulink.Signal object 'MLed_FireErrCB_flg' */
uint8_T MLed_FireErrGo_flg;            /* Simulink.Signal object 'MLed_FireErrGo_flg' */
uint8_T MLed_PlayCBErr_flg;            /* Simulink.Signal object 'MLed_PlayCBErr_flg' */
uint8_T MLed_PlayCBing_flg;            /* Simulink.Signal object 'MLed_PlayCBing_flg' */
uint8_T MLed_PlayCancelOrder_flg;      /* Simulink.Signal object 'MLed_PlayCancelOrder_flg' */
uint8_T MLed_PlayCarNoInnet_flg;       /* Simulink.Signal object 'MLed_PlayCarNoInnet_flg' */
uint8_T MLed_PlayCloudNoIn_flg;        /* Simulink.Signal object 'MLed_PlayCloudNoIn_flg' */
uint8_T MLed_PlayErrGo_flg;            /* Simulink.Signal object 'MLed_PlayErrGo_flg' */
uint8_T MLed_PlayInPlac_flg;           /* Simulink.Signal object 'MLed_PlayInPlac_flg' */
uint8_T MLed_PlayIn_flg;               /* Simulink.Signal object 'MLed_PlayIn_flg' */
uint8_T MLed_PlayInitErr_flg;          /* Simulink.Signal object 'MLed_PlayInitErr_flg' */
uint8_T MLed_PlayNoBag_flg;            /* Simulink.Signal object 'MLed_PlayNoBag_flg' */
uint8_T MLed_PlayOut_flg;              /* Simulink.Signal object 'MLed_PlayOut_flg' */
uint8_T MLed_PlayStartCB_flg;          /* Simulink.Signal object 'MLed_PlayStartCB_flg' */
uint8_T MLed_Play_sta;                 /* Simulink.Signal object 'MLed_Play_sta' */
uint8_T MLed_PleaseGo_flg;             /* Simulink.Signal object 'MLed_PleaseGo_flg' */
uint8_T MLed_ShowFree_flg;             /* Simulink.Signal object 'MLed_ShowFree_flg' */
uint8_T MLed_Start10s_Num;             /* Simulink.Signal object 'MLed_Start10s_Num' */
uint8_T MLed_Start10s_flg;             /* Simulink.Signal object 'MLed_Start10s_flg' */
uint8_T MLight_Start_flg;              /* Simulink.Signal object 'MLight_Start_flg' */
uint8_T MLoc_Deal_err;                 /* Simulink.Signal object 'MLoc_Deal_err' */
uint8_T MLocat_AllowCESoc_pct;         /* Simulink.Signal object 'MLocat_AllowCESoc_pct' */
uint8_T MLocat_BatCode_null[30];       /* Simulink.Signal object 'MLocat_BatCode_null' */
uint8_T MLocat_BookLock_flg[20];       /* Simulink.Signal object 'MLocat_BookLock_flg' */
uint8_T MLocat_Car_type[20];           /* Simulink.Signal object 'MLocat_Car_type' */
uint8_T MLocat_CharEndSoc_pct;         /* Simulink.Signal object 'MLocat_CharEndSoc_pct' */
uint8_T MLocat_CharPow_kw;             /* Simulink.Signal object 'MLocat_CharPow_kw' */
uint8_T MLocat_Charging_flg[20];       /* Simulink.Signal object 'MLocat_Charging_flg' */
uint8_T MLocat_CheckClear_flg;         /* Simulink.Signal object 'MLocat_CheckClear_flg' */
uint8_T MLocat_CheckInNet_ok;          /* Simulink.Signal object 'MLocat_CheckInNet_ok' */
uint8_T MLocat_CheckLock_flg;          /* Simulink.Signal object 'MLocat_CheckLock_flg' */
uint8_T MLocat_Conf_ena[20];           /* Simulink.Signal object 'MLocat_Conf_ena' */
uint8_T MLocat_ErrReset_flg[20];       /* Simulink.Signal object 'MLocat_ErrReset_flg' */
uint8_T MLocat_FIFO_Buf[20];           /* Simulink.Signal object 'MLocat_FIFO_Buf' */
uint8_T MLocat_GetBatCode_flg[20];     /* Simulink.Signal object 'MLocat_GetBatCode_flg' */
uint8_T MLocat_InNet_flg[20];          /* Simulink.Signal object 'MLocat_InNet_flg' */
uint8_T MLocat_RealBook_num[3];        /* Simulink.Signal object 'MLocat_RealBook_num' */
uint8_T MLocat_SelfChar_ena[20];       /* Simulink.Signal object 'MLocat_SelfChar_ena' */
uint8_T MLocat_Soc_pct[20];            /* Simulink.Signal object 'MLocat_Soc_pct' */
uint8_T MQt_ClearInPlace_flg;          /* Simulink.Signal object 'MQt_ClearInPlace_flg' */
uint8_T MQt_ConfDa_push;               /* Simulink.Signal object 'MQt_ConfDa_push' */
uint8_T MQt_DisaSoundLigErr_flg;       /* Simulink.Signal object 'MQt_DisaSoundLigErr_flg' */
uint8_T MQt_GetCancelOrd_flg;          /* Simulink.Signal object 'MQt_GetCancelOrd_flg' */
uint8_T MQt_GetHandCBOk_flg;           /* Simulink.Signal object 'MQt_GetHandCBOk_flg' */
uint8_T MQt_GetInPlace_flg;            /* Simulink.Signal object 'MQt_GetInPlace_flg' */
uint8_T MQt_GetODOValue_flg;           /* Simulink.Signal object 'MQt_GetODOValue_flg' */
uint8_T MQt_GetPointBack_flg;          /* Simulink.Signal object 'MQt_GetPointBack_flg' */
uint8_T MQt_GetSql_flg;                /* Simulink.Signal object 'MQt_GetSql_flg' */
uint8_T MQt_NoBag_flg;                 /* Simulink.Signal object 'MQt_NoBag_flg' */
uint8_T MQt_SendAirData_flg;           /* Simulink.Signal object 'MQt_SendAirData_flg' */
uint8_T MQt_SendCEData_flg;            /* Simulink.Signal object 'MQt_SendCEData_flg' */
uint8_T MQt_SendChgData1_flg;          /* Simulink.Signal object 'MQt_SendChgData1_flg' */
uint8_T MQt_SendChgData2_flg;          /* Simulink.Signal object 'MQt_SendChgData2_flg' */
uint8_T MQt_SendChgerErr_flg;          /* Simulink.Signal object 'MQt_SendChgerErr_flg' */
uint8_T MQt_SendDcEne2_flg;            /* Simulink.Signal object 'MQt_SendDcEne2_flg' */
uint8_T MQt_SendDcEne_flg;             /* Simulink.Signal object 'MQt_SendDcEne_flg' */
uint8_T MQt_SendInPlace_flg;           /* Simulink.Signal object 'MQt_SendInPlace_flg' */
uint8_T MQt_SendLocatData_flg;         /* Simulink.Signal object 'MQt_SendLocatData_flg' */
uint8_T MQt_SendPoint_flg;             /* Simulink.Signal object 'MQt_SendPoint_flg' */
uint8_T MQt_StartSelfCE_sta;           /* Simulink.Signal object 'MQt_StartSelfCE_sta' */
uint8_T MRemo_CELocator_ok;            /* Simulink.Signal object 'MRemo_CELocator_ok' */
uint8_T MRemo_ChangeBat_ok;            /* Simulink.Signal object 'MRemo_ChangeBat_ok' */
uint8_T MRemo_GetInPlace_flg;          /* Simulink.Signal object 'MRemo_GetInPlace_flg' */
uint8_T MRemo_GetSelfCE_flg;           /* Simulink.Signal object 'MRemo_GetSelfCE_flg' */
uint8_T MSound_Start_flg;              /* Simulink.Signal object 'MSound_Start_flg' */
uint8_T MSql_DelUnfinishedOrder_flg;   /* Simulink.Signal object 'MSql_DelUnfinishedOrder_flg' */
uint8_T MSql_WriCeOkData_flg;          /* Simulink.Signal object 'MSql_WriCeOkData_flg' */
uint8_T MSql_WriCeStartData_flg;       /* Simulink.Signal object 'MSql_WriCeStartData_flg' */
uint8_T MSql_WriUnfinishedOrder_flg;   /* Simulink.Signal object 'MSql_WriUnfinishedOrder_flg' */
uint8_T MSqlite_WriAcEnergy_flg;       /* Simulink.Signal object 'MSqlite_WriAcEnergy_flg' */
uint8_T MSqlite_WriAirConData_flg;     /* Simulink.Signal object 'MSqlite_WriAirConData_flg' */
uint8_T MSqlite_WriBattData_flg[20];   /* Simulink.Signal object 'MSqlite_WriBattData_flg' */
uint8_T MSqlite_WriCEData_flg;         /* Simulink.Signal object 'MSqlite_WriCEData_flg' */
uint8_T MSqlite_WriCharData_flg[20];   /* Simulink.Signal object 'MSqlite_WriCharData_flg' */
uint8_T MSqlite_WriConfigData_flg;     /* Simulink.Signal object 'MSqlite_WriConfigData_flg' */
uint8_T MSqlite_WriErrData_flg;        /* Simulink.Signal object 'MSqlite_WriErrData_flg' */
uint8_T MSqlite_WriLocatData_flg[20];  /* Simulink.Signal object 'MSqlite_WriLocatData_flg' */
uint8_T MSqlite_WrtErr_flg;            /* Simulink.Signal object 'MSqlite_WrtErr_flg' */
uint8_T MSys_AllErr_Lev;               /* Simulink.Signal object 'MSys_AllErr_Lev' */
uint8_T Mnet_WriCeOkData_flg;          /* Simulink.Signal object 'Mnet_WriCeOkData_flg' */
uint8_T Mnet_WriCeStartData_flg;       /* Simulink.Signal object 'Mnet_WriCeStartData_flg' */
uint8_T PLC_Die_flg;                   /* Simulink.Signal object 'PLC_Die_flg' */
uint8_T QT_BatOutOrder_flg;            /* Simulink.Signal object 'QT_BatOutOrder_flg' */
uint8_T QT_CELocatorOrder_flg;         /* Simulink.Signal object 'QT_CELocatorOrder_flg' */
uint8_T QT_ChangeBatOrder_flg;         /* Simulink.Signal object 'QT_ChangeBatOrder_flg' */
uint8_T QT_ContinueOrder_flg;          /* Simulink.Signal object 'QT_ContinueOrder_flg' */
uint8_T QT_DismountBatOrder_flg;       /* Simulink.Signal object 'QT_DismountBatOrder_flg' */
uint8_T QT_ErrResetOrder_flg;          /* Simulink.Signal object 'QT_ErrResetOrder_flg' */
uint8_T QT_HandCE_flg;                 /* Simulink.Signal object 'QT_HandCE_flg' */
uint8_T QT_InLocatorOrder_flg;         /* Simulink.Signal object 'QT_InLocatorOrder_flg' */
uint8_T QT_InitOrder_flg;              /* Simulink.Signal object 'QT_InitOrder_flg' */
uint8_T QT_MountBatOrder_flg;          /* Simulink.Signal object 'QT_MountBatOrder_flg' */
uint8_T QT_OutLocatorOrder_flg;        /* Simulink.Signal object 'QT_OutLocatorOrder_flg' */
uint8_T QT_SlefCE_flg;                 /* Simulink.Signal object 'QT_SlefCE_flg' */
uint8_T QT_StopOrder_flg;              /* Simulink.Signal object 'QT_StopOrder_flg' */
uint8_T QT_TimeOutOrder_flg;           /* Simulink.Signal object 'QT_TimeOutOrder_flg' */
uint8_T Qt_CarAllowOut_sta;            /* Simulink.Signal object 'Qt_CarAllowOut_sta' */
uint8_T Qt_HandCharPow_kw[20];         /* Simulink.Signal object 'Qt_HandCharPow_kw' */
uint8_T RemotSend_Heart_flg;           /* Simulink.Signal object 'RemotSend_Heart_flg' */
uint8_T RemotSend_UpACEngData_flg;     /* Simulink.Signal object 'RemotSend_UpACEngData_flg' */
uint8_T RemotSend_UpAirConData_flg;    /* Simulink.Signal object 'RemotSend_UpAirConData_flg' */
uint8_T RemotSend_UpCEData_flg;        /* Simulink.Signal object 'RemotSend_UpCEData_flg' */
uint8_T RemotSend_UpCEingData_flg;     /* Simulink.Signal object 'RemotSend_UpCEingData_flg' */
uint8_T RemotSend_UpCHUData_flg;       /* Simulink.Signal object 'RemotSend_UpCHUData_flg' */
uint8_T RemotSend_UpErrCHUData_flg;    /* Simulink.Signal object 'RemotSend_UpErrCHUData_flg' */
uint8_T RemotSend_UpFanData_flg;       /* Simulink.Signal object 'RemotSend_UpFanData_flg' */
uint8_T RemotSend_UpFireData_flg;      /* Simulink.Signal object 'RemotSend_UpFireData_flg' */
uint8_T RemotSend_UpPLCData_flg;       /* Simulink.Signal object 'RemotSend_UpPLCData_flg' */
uint8_T Remot_conn_flg;                /* Simulink.Signal object 'Remot_conn_flg' */
uint8_T Send_CE_BATBAGOUT_flg;         /* Simulink.Signal object 'Send_CE_BATBAGOUT_flg' */
uint8_T Send_CE_CHANGEBAT_flg;         /* Simulink.Signal object 'Send_CE_CHANGEBAT_flg' */
uint8_T Send_CE_CHANGELOCATOR_flg;     /* Simulink.Signal object 'Send_CE_CHANGELOCATOR_flg' */
uint8_T Send_CE_CONTINUE_flg;          /* Simulink.Signal object 'Send_CE_CONTINUE_flg' */
uint8_T Send_CE_DISMOUNTBAT_flg;       /* Simulink.Signal object 'Send_CE_DISMOUNTBAT_flg' */
uint8_T Send_CE_ERRRESET_flg;          /* Simulink.Signal object 'Send_CE_ERRRESET_flg' */
uint8_T Send_CE_INIT_flg;              /* Simulink.Signal object 'Send_CE_INIT_flg' */
uint8_T Send_CE_INLOCATOR_flg;         /* Simulink.Signal object 'Send_CE_INLOCATOR_flg' */
uint8_T Send_CE_MOUNTBAT_flg;          /* Simulink.Signal object 'Send_CE_MOUNTBAT_flg' */
uint8_T Send_CE_OUTLOCATOR_flg;        /* Simulink.Signal object 'Send_CE_OUTLOCATOR_flg' */
uint8_T Send_CE_STOP_flg;              /* Simulink.Signal object 'Send_CE_STOP_flg' */
uint8_T Send_CE_TIMEOUT_flg;           /* Simulink.Signal object 'Send_CE_TIMEOUT_flg' */
uint8_T SetPwr_timeout[20];            /* Simulink.Signal object 'SetPwr_timeout' */
uint8_T Sta;                           /* Simulink.Signal object 'Sta' */
uint8_T StartChg_timeout[20];          /* Simulink.Signal object 'StartChg_timeout' */
uint8_T StopChg_timeout[20];           /* Simulink.Signal object 'StopChg_timeout' */
uint8_T Sys_Big_sta;                   /* Simulink.Signal object 'Sys_Big_sta' */
uint8_T Sys_sta[20];                   /* Simulink.Signal object 'Sys_sta' */
uint8_T chu_test_state[20];            /* Simulink.Signal object 'chu_test_state' */
uint8_T index_Chu;                     /* Simulink.Signal object 'index_Chu' */
uint8_T index_Err;                     /* Simulink.Signal object 'index_Err' */
uint8_T mdl_CEPrepareSig_flg[20];      /* Simulink.Signal object 'mdl_CEPrepareSig_flg' */
uint8_T stop_ok[20];                   /* Simulink.Signal object 'stop_ok' */

/* Block signals and states (default storage) */
DW rtDW;

/* Real-time model */
RT_MODEL rtM_;
RT_MODEL *const rtM = &rtM_;
extern void Chart1(real_T rtu_CHU_index, DW_Chart1 *localDW);
extern void ChangeElec(void);
extern void DatLib(void);
extern void Charger(void);

/* Forward declaration for local functions */
void MLocatErr_Deal(real_T rtu_CHU_index);
void init_clear(real_T rtu_CHU_index);
void CharEnd_Deal(real_T rtu_CHU_index);
void exit_internal_charging_state(real_T rtu_CHU_index, DW_Chart1 *localDW);
void clear(real_T rtu_CHU_index);
void Get_CHUChgRtData(real_T rtu_CHU_index);
void CHUPwrSet(real_T rtu_CHU_index);
void chg_main_state(real_T rtu_CHU_index, DW_Chart1 *localDW);
void CHU_deal(real_T rtu_CHU_index, DW_Chart1 *localDW);

/* Forward declaration for local functions */
void Remo_Deal(void);
void TimeOut_Deal(void);
void TimeOut_Order(void);
void InLocator_Deal(void);
void BatInLocator_Order(void);
void Init_Deal(void);
void Init_Order(void);
void Continue_Deal(void);
void Continue_Order(void);
void OutLocator_Deal(void);
void BatOutLocator_Order(void);
void BatOut_Deal(void);
void BatOut_Order(void);
void Stop_Deal(void);
void Stop_Order(void);
void CELocator_Deal(void);
void CELocator_Order(void);
void ChangeBat_Deal(void);
void ChangeBatt_Order(void);
void MountBat_Deal(void);
void ErrReset_Deal(void);
void Hand_CE(void);
void All_CheckErr(void);
void CheckBookBag_Deal(void);
void MCE_Clear(void);
void MCE_Initok(void);
void MIO_Clear(void);
void exit_internal_All_Self_Check(void);
void All_Self_Check(void);
void exit_atomic_Allow_InStation(void);
void CheckBatBag_Deal1(void);
void exit_internal_Allow_InStation(void);
void Allow_InStation(void);
void CheckCarInCancel_Deal(void);
void exit_internal_Car_In(void);
void Car_In(void);
void Check_err(void);
void CheckCBCancel_Deal(void);
void CB_Deal(void);
void Check_PLC_Die(void);
void CB_AllClear(void);
void UpData_FIFO(void);
void ChangeBatt(void);
void exit_internal_Cloud_Pass(void);
void Cloud_Pass(void);
void Main_CE(void);
void MIO_Door(void);
void MIO_Water(void);
void MIO_Light(void);
void MIO_Thunder(void);
void QtSend_CharDa_Deal(void);
void QtSend_CharErr_Deal(void);
void QtSend_CEDat_Deal(void);
void QtSend_LocatData_Deal(void);
void QtSend_AirData_Deal(void);
void QtSend_DcEne_Deal(void);
void QtSend_DcEne2_Deal(void);
void RemoSend_CharDa_Deal(void);
void RemoSend_CharErr_Deal(void);
void RemotSend_UpCEing_Deal(void);
void LedSta_Deal(void);
void Play_InitErr_Deal(void);
void Play_Free_Deal(void);
void Play_FireErr_Deal(void);
void Play_CBingErr_Deal(void);
void Play_PleaseGo_Deal(void);
void Led_AllPlay(void);
void MAIN(void);
void In_FIFO(void);
void enter_internal_c3_SA4_EMS(void);

/* Forward declaration for local functions */
void DatLib_CharDa_Deal(void);
void DatLib_ConfDa_Deal(void);
void DatLib_AirconDa_Deal(void);
void DatLib_LocatDa_Deal(void);
void DatLib_LocatDa_Deal_c(void);
void DatLib_BattData_Deal(void);
void DatLib_AcEnergy_Deal(void);

/* Function for Chart: '<Root>/Chart' */
void Remo_Deal(void)
{
  MRemo_ChangeBat_ok = (uint8_T)((Remot_conn_flg == 1) && (MRemo_ChangeBat_ok ==
    1));
  MRemo_CELocator_ok = (uint8_T)((Remot_conn_flg == 1) && (MRemo_CELocator_ok ==
    1));
}

/* Function for Chart: '<Root>/Chart' */
void TimeOut_Deal(void)
{
  Send_CE_TIMEOUT_flg = 1U;

  /* MCE_InLocator_dest = 1; */
}

/* Function for Chart: '<Root>/Chart' */
void TimeOut_Order(void)
{
  switch (rtDW.is_TimeOut_Order) {
   case IN_Init_c:
    if (QT_TimeOutOrder_flg == 1) {
      QT_TimeOutOrder_flg = 0U;
      rtDW.is_TimeOut_Order = IN_Send_Init;
      TimeOut_Deal();
      Sys_sta[9] = 2U;
    }
    break;

   case IN_Init2:
    rtDW.is_TimeOut_Order = IN_Init_c;
    Sys_sta[9] = 1U;
    QT_TimeOutOrder_flg = 0U;
    MCE_GetTimeOut_flg = 0U;
    break;

   case IN_Init3:
    rtDW.is_TimeOut_Order = IN_Init_c;
    Sys_sta[9] = 1U;
    QT_TimeOutOrder_flg = 0U;
    MCE_GetTimeOut_flg = 0U;
    break;

   case IN_Init4:
    if ((MCE_RunState == 3) || (rtDW.MCE_TimeOut_count >= 100.0)) {
      rtDW.MCE_TimeOut_count = 0.0;
      rtDW.is_TimeOut_Order = IN_Init5;

      /* MCE_TimeOut_ok = 1; */
      Sys_sta[9] = 4U;
    } else {
      rtDW.MCE_TimeOut_count++;
    }
    break;

   case IN_Init5:
    rtDW.is_TimeOut_Order = IN_Init_c;
    Sys_sta[9] = 1U;
    QT_TimeOutOrder_flg = 0U;
    MCE_GetTimeOut_flg = 0U;
    break;

   default:
    if (MCE_GetTimeOut_flg >= 1) {
      rtDW.MCE_TimeOut_Times = 0.0;
      if (MCE_GetTimeOut_flg == 1) {
        MCE_GetTimeOut_flg = 0U;

        /* Send_CE_TIMEOUT_flg=0; */
        rtDW.MCE_TimeOut_Times++;
        Send_CE_TIMEOUT_flg = 0U;
        rtDW.is_TimeOut_Order = IN_Init4;
        Sys_sta[9] = 3U;
        rtDW.MCE_TimeOut_count = 0.0;
      } else {
        MCE_Order_err = 1U;

        /* Send_CE_TIMEOUT_flg=0; */
        rtDW.MCE_TimeOut_count = 0.0;
        rtDW.MCE_TimeOut_Times++;
        Send_CE_TIMEOUT_flg = 0U;
        rtDW.is_TimeOut_Order = IN_Init_c;
        Sys_sta[9] = 1U;
        QT_TimeOutOrder_flg = 0U;
        MCE_GetTimeOut_flg = 0U;
      }
    } else if (rtDW.MCE_TimeOut_count >= 100.0) {
      /* Send_CE_TIMEOUT_flg=0; */
      rtDW.MCE_TimeOut_count = 0.0;
      rtDW.MCE_TimeOut_Times++;
      Send_CE_TIMEOUT_flg = 0U;
      rtDW.is_TimeOut_Order = IN_Send_Init;
      TimeOut_Deal();
      Sys_sta[9] = 2U;
    } else if (rtDW.MCE_TimeOut_Times >= 3.0) {
      rtDW.MCE_TimeOut_Times = 0.0;

      /* Send_CE_TIMEOUT_flg=0; */
      rtDW.MCE_TimeOut_count = 0.0;
      rtDW.MCE_TimeOut_Times++;
      Send_CE_TIMEOUT_flg = 0U;
      rtDW.is_TimeOut_Order = IN_Init3;

      /* MCE_TimeOutOrder_err  = 1; */
    } else {
      rtDW.MCE_TimeOut_count++;
    }
    break;
  }
}

/* Function for Chart: '<Root>/Chart' */
void InLocator_Deal(void)
{
  if ((MCE_ErrData == 0) && (MCE_RunState == 1)) {
    Send_CE_INLOCATOR_flg = 1U;
  } else {
    Send_CE_INLOCATOR_flg = 0U;
    MCE_Order_err = 1U;
  }

  /* MCE_InLocator_dest = 1; */
}

/* Function for Chart: '<Root>/Chart' */
void BatInLocator_Order(void)
{
  switch (rtDW.is_BatInLocator_Order) {
   case IN_Init_c:
    if (QT_InLocatorOrder_flg == 1) {
      QT_InLocatorOrder_flg = 0U;
      rtDW.is_BatInLocator_Order = IN_Send_Init;
      InLocator_Deal();
      Sys_sta[2] = 2U;
    }
    break;

   case IN_Init2:
    rtDW.is_BatInLocator_Order = IN_Init_c;
    Sys_sta[2] = 1U;
    QT_InLocatorOrder_flg = 0U;
    MCE_GetInLocator_flg = 0U;
    break;

   case IN_Init3:
    rtDW.is_BatInLocator_Order = IN_Init_c;
    Sys_sta[2] = 1U;
    QT_InLocatorOrder_flg = 0U;
    MCE_GetInLocator_flg = 0U;
    break;

   case IN_Init4:
    if ((MCE_GetInLocatorOk_flg == 1) || (rtDW.Wait_InLocator_cou >= 18000.0)) {
      rtDW.Wait_InLocator_cou = 0.0;
      rtDW.is_BatInLocator_Order = IN_Init5;
      MCE_GetInLocatorOk_flg = 0U;
      Sys_sta[2] = 4U;
    } else {
      rtDW.Wait_InLocator_cou++;
    }
    break;

   case IN_Init5:
    rtDW.is_BatInLocator_Order = IN_Init_c;
    Sys_sta[2] = 1U;
    QT_InLocatorOrder_flg = 0U;
    MCE_GetInLocator_flg = 0U;
    break;

   default:
    if (MCE_GetInLocator_flg == 1) {
      rtDW.MCE_InLocator_Times = 0.0;
      if (MCE_GetInLocator_flg == 1) {
        MCE_GetInLocator_flg = 0U;

        /* Send_CE_INLOCATOR_flg = 0; */
        rtDW.MCE_InLocator_count = 0.0;
        rtDW.MCE_InLocator_Times++;
        Send_CE_INLOCATOR_flg = 0U;
        rtDW.is_BatInLocator_Order = IN_Init4;
        Sys_sta[2] = 3U;
      } else {
        MCE_Order_err = 1U;

        /* Send_CE_INLOCATOR_flg = 0; */
        rtDW.MCE_InLocator_count = 0.0;
        rtDW.MCE_InLocator_Times++;
        Send_CE_INLOCATOR_flg = 0U;
        rtDW.is_BatInLocator_Order = IN_Init_c;
        Sys_sta[2] = 1U;
        QT_InLocatorOrder_flg = 0U;
        MCE_GetInLocator_flg = 0U;
      }
    } else if (rtDW.MCE_InLocator_count >= 100.0) {
      /* {MCE_InLocator_count=0} */
      /* Send_CE_INLOCATOR_flg = 0; */
      rtDW.MCE_InLocator_count = 0.0;
      rtDW.MCE_InLocator_Times++;
      Send_CE_INLOCATOR_flg = 0U;
      rtDW.is_BatInLocator_Order = IN_Send_Init;
      InLocator_Deal();
      Sys_sta[2] = 2U;
    } else if (MCE_Order_err == 1) {
      rtDW.MCE_InLocator_Times = 0.0;

      /* Send_CE_INLOCATOR_flg = 0; */
      rtDW.MCE_InLocator_count = 0.0;
      rtDW.MCE_InLocator_Times++;
      Send_CE_INLOCATOR_flg = 0U;
      rtDW.is_BatInLocator_Order = IN_Init2;
      MCE_Order_err = 1U;
    } else if (rtDW.MCE_InLocator_Times >= 3.0) {
      rtDW.MCE_InLocator_Times = 0.0;

      /* Send_CE_INLOCATOR_flg = 0; */
      rtDW.MCE_InLocator_count = 0.0;
      rtDW.MCE_InLocator_Times++;
      Send_CE_INLOCATOR_flg = 0U;
      rtDW.is_BatInLocator_Order = IN_Init3;
      MCE_InLocatorOrder_err = 1U;
    } else {
      rtDW.MCE_InLocator_count++;
    }
    break;
  }
}

/* Function for Chart: '<Root>/Chart' */
void Init_Deal(void)
{
  if (MCE_ErrData == 0) {
    Send_CE_INIT_flg = 1U;
  } else {
    Send_CE_INIT_flg = 0U;
    MCE_Order_err = 1U;
  }

  /* MCE_InLocator_dest = 1; */
}

/* Function for Chart: '<Root>/Chart' */
void Init_Order(void)
{
  switch (rtDW.is_Init_Order) {
   case IN_Init_c:
    if (QT_InitOrder_flg == 1) {
      QT_InitOrder_flg = 0U;
      rtDW.is_Init_Order = IN_Send_Init;
      Init_Deal();
      Sys_sta[1] = 2U;
    }
    break;

   case IN_Init2:
    rtDW.is_Init_Order = IN_Init_c;
    Sys_sta[1] = 1U;
    QT_InitOrder_flg = 0U;
    MCE_GetInit_flg = 0U;
    break;

   case IN_Init3:
    rtDW.is_Init_Order = IN_Init_c;
    Sys_sta[1] = 1U;
    QT_InitOrder_flg = 0U;
    MCE_GetInit_flg = 0U;
    break;

   case IN_Init4:
    /* [MCE_RGVStepData==1&&MCE_StackerStepData==1&&MCE_ParkPlatStepData==1] */
    rtDW.is_Init_Order = IN_Init5;
    Sys_sta[1] = 4U;

    /* MCE_Init_ok = 1 */
    break;

   case IN_Init5:
    rtDW.is_Init_Order = IN_Init_c;
    Sys_sta[1] = 1U;
    QT_InitOrder_flg = 0U;
    MCE_GetInit_flg = 0U;
    break;

   default:
    if (MCE_GetInit_flg >= 1) {
      rtDW.MCE_InitDeal_Times = 0.0;
      if (MCE_GetInit_flg == 1) {
        MCE_GetInit_flg = 0U;

        /* Send_CE_INIT_flg = 0; */
        rtDW.MCE_InitDeal_count = 0.0;
        rtDW.MCE_InitDeal_Times++;
        Send_CE_INIT_flg = 0U;
        rtDW.is_Init_Order = IN_Init4;
        Sys_sta[1] = 3U;
      } else {
        MCE_Order_err = 1U;

        /* Send_CE_INIT_flg = 0; */
        rtDW.MCE_InitDeal_count = 0.0;
        rtDW.MCE_InitDeal_Times++;
        Send_CE_INIT_flg = 0U;
        rtDW.is_Init_Order = IN_Init_c;
        Sys_sta[1] = 1U;
        QT_InitOrder_flg = 0U;
        MCE_GetInit_flg = 0U;
      }
    } else if (rtDW.MCE_InitDeal_count >= 100.0) {
      /* Send_CE_INIT_flg = 0; */
      rtDW.MCE_InitDeal_count = 0.0;
      rtDW.MCE_InitDeal_Times++;
      Send_CE_INIT_flg = 0U;
      rtDW.is_Init_Order = IN_Send_Init;
      Init_Deal();
      Sys_sta[1] = 2U;
    } else if (rtDW.MCE_InitDeal_Times >= 3.0) {
      rtDW.MCE_InitDeal_Times = 0.0;

      /* Send_CE_INIT_flg = 0; */
      rtDW.MCE_InitDeal_count = 0.0;
      rtDW.MCE_InitDeal_Times++;
      Send_CE_INIT_flg = 0U;
      rtDW.is_Init_Order = IN_Init3;
      MCE_InitDealOrder_err = 1U;
    } else if (MCE_Order_err == 1) {
      rtDW.MCE_InitDeal_Times = 0.0;

      /* Send_CE_INIT_flg = 0; */
      rtDW.MCE_InitDeal_count = 0.0;
      rtDW.MCE_InitDeal_Times++;
      Send_CE_INIT_flg = 0U;
      rtDW.is_Init_Order = IN_Init2;
      MCE_Order_err = 1U;
    } else {
      rtDW.MCE_InitDeal_count++;
    }
    break;
  }
}

/* Function for Chart: '<Root>/Chart' */
void Continue_Deal(void)
{
  Send_CE_CONTINUE_flg = 1U;

  /* MCE_InLocator_dest = 1; */
}

/* Function for Chart: '<Root>/Chart' */
void Continue_Order(void)
{
  switch (rtDW.is_Continue_Order) {
   case IN_Init_c:
    if (QT_ContinueOrder_flg == 1) {
      QT_ContinueOrder_flg = 0U;
      rtDW.is_Continue_Order = IN_Send_Init;
      Continue_Deal();
      Sys_sta[10] = 2U;
    }
    break;

   case IN_Init2:
    rtDW.is_Continue_Order = IN_Init_c;
    Sys_sta[10] = 1U;
    QT_ContinueOrder_flg = 0U;
    MCE_GetContinue_flg = 0U;
    break;

   case IN_Init3:
    rtDW.is_Continue_Order = IN_Init_c;
    Sys_sta[10] = 1U;
    QT_ContinueOrder_flg = 0U;
    MCE_GetContinue_flg = 0U;
    break;

   case IN_Init4:
    /* [MCE_RunState==7||MCE_RunState==1] */
    rtDW.is_Continue_Order = IN_Init5;

    /* Send_Continue_ok = 1; */
    Sys_sta[10] = 4U;
    break;

   case IN_Init5:
    rtDW.is_Continue_Order = IN_Init_c;
    Sys_sta[10] = 1U;
    QT_ContinueOrder_flg = 0U;
    MCE_GetContinue_flg = 0U;
    break;

   default:
    if (MCE_GetContinue_flg >= 1) {
      rtDW.MCE_Continue_Times = 0.0;
      if (MCE_GetContinue_flg == 1) {
        MCE_GetContinue_flg = 0U;

        /* Send_CE_CONTINUE_flg=0; */
        rtDW.MCE_Continue_count = 0.0;
        rtDW.MCE_Continue_Times++;
        Send_CE_CONTINUE_flg = 0U;
        rtDW.is_Continue_Order = IN_Init4;
        Sys_sta[10] = 3U;
      } else {
        MCE_Order_err = 1U;

        /* Send_CE_CONTINUE_flg=0; */
        rtDW.MCE_Continue_count = 0.0;
        rtDW.MCE_Continue_Times++;
        Send_CE_CONTINUE_flg = 0U;
        rtDW.is_Continue_Order = IN_Init_c;
        Sys_sta[10] = 1U;
        QT_ContinueOrder_flg = 0U;
        MCE_GetContinue_flg = 0U;
      }
    } else if (rtDW.MCE_Continue_count >= 100.0) {
      /* Send_CE_CONTINUE_flg=0; */
      rtDW.MCE_Continue_count = 0.0;
      rtDW.MCE_Continue_Times++;
      Send_CE_CONTINUE_flg = 0U;
      rtDW.is_Continue_Order = IN_Send_Init;
      Continue_Deal();
      Sys_sta[10] = 2U;
    } else if (rtDW.MCE_Continue_Times >= 3.0) {
      rtDW.MCE_Continue_Times = 0.0;

      /* Send_CE_CONTINUE_flg=0; */
      rtDW.MCE_Continue_count = 0.0;
      rtDW.MCE_Continue_Times++;
      Send_CE_CONTINUE_flg = 0U;
      rtDW.is_Continue_Order = IN_Init3;
      MCE_ContinueOrder_err = 1U;
    } else {
      rtDW.MCE_Continue_count++;
    }
    break;
  }
}

/* Function for Chart: '<Root>/Chart' */
void OutLocator_Deal(void)
{
  if ((MCE_ErrData == 0) && (MCE_RunState == 1)) {
    Send_CE_OUTLOCATOR_flg = 1U;
  } else {
    Send_CE_OUTLOCATOR_flg = 0U;
    MCE_Order_err = 1U;
  }

  /* MCE_InLocator_dest = 1; */
}

/* Function for Chart: '<Root>/Chart' */
void BatOutLocator_Order(void)
{
  switch (rtDW.is_BatOutLocator_Order) {
   case IN_Init_c:
    if (QT_OutLocatorOrder_flg == 1) {
      QT_OutLocatorOrder_flg = 0U;
      rtDW.is_BatOutLocator_Order = IN_Send_Init;
      OutLocator_Deal();
      Sys_sta[3] = 2U;
    }
    break;

   case IN_Init2:
    rtDW.is_BatOutLocator_Order = IN_Init_c;
    Sys_sta[3] = 1U;
    QT_OutLocatorOrder_flg = 0U;
    MCE_GetOutLocator_flg = 0U;
    break;

   case IN_Init3:
    rtDW.is_BatOutLocator_Order = IN_Init_c;
    Sys_sta[3] = 1U;
    QT_OutLocatorOrder_flg = 0U;
    MCE_GetOutLocator_flg = 0U;
    break;

   case IN_Init4:
    if ((MCE_GetOutLocatorOk_flg == 1) || (rtDW.Wait_OutLocator_cou >= 18000.0))
    {
      rtDW.Wait_OutLocator_cou = 0.0;
      rtDW.is_BatOutLocator_Order = IN_Init5;
      MCE_GetOutLocatorOk_flg = 0U;
      Sys_sta[3] = 4U;
    } else {
      rtDW.Wait_OutLocator_cou++;
    }
    break;

   case IN_Init5:
    rtDW.is_BatOutLocator_Order = IN_Init_c;
    Sys_sta[3] = 1U;
    QT_OutLocatorOrder_flg = 0U;
    MCE_GetOutLocator_flg = 0U;
    break;

   default:
    if (MCE_GetOutLocator_flg >= 1) {
      rtDW.MCE_OutLocator_Times = 0.0;
      if (MCE_GetOutLocator_flg == 1) {
        MCE_GetOutLocator_flg = 0U;

        /* Send_CE_OUTLOCATOR_flg=0; */
        rtDW.MCE_OutLocator_count = 0.0;
        rtDW.MCE_OutLocator_Times++;
        Send_CE_OUTLOCATOR_flg = 0U;
        rtDW.is_BatOutLocator_Order = IN_Init4;
        Sys_sta[3] = 3U;
      } else {
        MCE_Order_err = 1U;

        /* Send_CE_OUTLOCATOR_flg=0; */
        rtDW.MCE_OutLocator_count = 0.0;
        rtDW.MCE_OutLocator_Times++;
        Send_CE_OUTLOCATOR_flg = 0U;
        rtDW.is_BatOutLocator_Order = IN_Init_c;
        Sys_sta[3] = 1U;
        QT_OutLocatorOrder_flg = 0U;
        MCE_GetOutLocator_flg = 0U;
      }
    } else if (rtDW.MCE_OutLocator_count >= 100.0) {
      /* {MCE_OutLocator_count=0} */
      /* Send_CE_OUTLOCATOR_flg=0; */
      rtDW.MCE_OutLocator_count = 0.0;
      rtDW.MCE_OutLocator_Times++;
      Send_CE_OUTLOCATOR_flg = 0U;
      rtDW.is_BatOutLocator_Order = IN_Send_Init;
      OutLocator_Deal();
      Sys_sta[3] = 2U;
    } else if (MCE_Order_err == 1) {
      rtDW.MCE_OutLocator_Times = 0.0;

      /* Send_CE_OUTLOCATOR_flg=0; */
      rtDW.MCE_OutLocator_count = 0.0;
      rtDW.MCE_OutLocator_Times++;
      Send_CE_OUTLOCATOR_flg = 0U;
      rtDW.is_BatOutLocator_Order = IN_Init2;
      MCE_Order_err = 1U;
    } else if (rtDW.MCE_OutLocator_Times >= 3.0) {
      rtDW.MCE_OutLocator_Times = 0.0;

      /* Send_CE_OUTLOCATOR_flg=0; */
      rtDW.MCE_OutLocator_count = 0.0;
      rtDW.MCE_OutLocator_Times++;
      Send_CE_OUTLOCATOR_flg = 0U;
      rtDW.is_BatOutLocator_Order = IN_Init3;
      MCE_OutLocatorOrder_err = 1U;
    } else {
      rtDW.MCE_OutLocator_count++;
    }
    break;
  }
}

/* Function for Chart: '<Root>/Chart' */
void BatOut_Deal(void)
{
  if ((MCE_ErrData == 0) && (MCE_RunState == 1) && (MCE_RGVSenser_sta == 1) &&
      (MCE_StackerSenser_sta == 1) && (MCE_ParkPlatStepData == 1) &&
      (MCE_RGVStepData == 1) && (MCE_StackerStepData == 1)) {
    Send_CE_BATBAGOUT_flg = 1U;
  } else {
    Send_CE_BATBAGOUT_flg = 0U;
    MCE_Order_err = 1U;
  }

  /* MCE_InLocator_dest = 1; */
}

/* Function for Chart: '<Root>/Chart' */
void BatOut_Order(void)
{
  switch (rtDW.is_BatOut_Order) {
   case IN_Init_c:
    if (QT_BatOutOrder_flg == 1) {
      QT_BatOutOrder_flg = 0U;
      rtDW.is_BatOut_Order = IN_Send_Init;
      BatOut_Deal();
      Sys_sta[5] = 2U;
    }
    break;

   case IN_Init2:
    rtDW.is_BatOut_Order = IN_Init_c;
    Sys_sta[5] = 1U;
    QT_BatOutOrder_flg = 0U;
    MCE_GetBatOut_flg = 0U;
    break;

   case IN_Init3:
    rtDW.is_BatOut_Order = IN_Init_c;
    Sys_sta[5] = 1U;
    QT_BatOutOrder_flg = 0U;
    MCE_GetBatOut_flg = 0U;
    break;

   case IN_Init4:
    rtDW.is_BatOut_Order = IN_Init5;
    MCE_GetBatOutOk_flg = 0U;
    Sys_sta[5] = 4U;
    break;

   case IN_Init5:
    rtDW.is_BatOut_Order = IN_Init_c;
    Sys_sta[5] = 1U;
    QT_BatOutOrder_flg = 0U;
    MCE_GetBatOut_flg = 0U;
    break;

   default:
    if (MCE_GetBatOut_flg >= 1) {
      rtDW.MCE_BatOut_Times = 0.0;
      if (MCE_GetBatOut_flg == 1) {
        MCE_GetBatOut_flg = 0U;

        /* Send_CE_BATBAGOUT_flg=0; */
        rtDW.MCE_BatOut_count = 0.0;
        rtDW.MCE_BatOut_Times++;
        Send_CE_BATBAGOUT_flg = 0U;
        rtDW.is_BatOut_Order = IN_Init4;
        Sys_sta[5] = 3U;
      } else {
        MCE_Order_err = 1U;

        /* Send_CE_BATBAGOUT_flg=0; */
        rtDW.MCE_BatOut_count = 0.0;
        rtDW.MCE_BatOut_Times++;
        Send_CE_BATBAGOUT_flg = 0U;
        rtDW.is_BatOut_Order = IN_Init_c;
        Sys_sta[5] = 1U;
        QT_BatOutOrder_flg = 0U;
        MCE_GetBatOut_flg = 0U;
      }
    } else if (rtDW.MCE_BatOut_count >= 100.0) {
      /* Send_CE_BATBAGOUT_flg=0; */
      rtDW.MCE_BatOut_count = 0.0;
      rtDW.MCE_BatOut_Times++;
      Send_CE_BATBAGOUT_flg = 0U;
      rtDW.is_BatOut_Order = IN_Send_Init;
      BatOut_Deal();
      Sys_sta[5] = 2U;
    } else if (MCE_Order_err == 1) {
      rtDW.MCE_BatOut_Times = 0.0;

      /* Send_CE_BATBAGOUT_flg=0; */
      rtDW.MCE_BatOut_count = 0.0;
      rtDW.MCE_BatOut_Times++;
      Send_CE_BATBAGOUT_flg = 0U;
      rtDW.is_BatOut_Order = IN_Init2;
      MCE_Order_err = 1U;
    } else if (rtDW.MCE_BatOut_Times >= 3.0) {
      rtDW.MCE_BatOut_Times = 0.0;

      /* Send_CE_BATBAGOUT_flg=0; */
      rtDW.MCE_BatOut_count = 0.0;
      rtDW.MCE_BatOut_Times++;
      Send_CE_BATBAGOUT_flg = 0U;
      rtDW.is_BatOut_Order = IN_Init3;
      MCE_BatOutOrder_err = 1U;
    } else {
      rtDW.MCE_BatOut_count++;
    }
    break;
  }
}

/* Function for Chart: '<Root>/Chart' */
void Stop_Deal(void)
{
  Send_CE_STOP_flg = 1U;

  /* MCE_InLocator_dest = 1; */
}

/* Function for Chart: '<Root>/Chart' */
void Stop_Order(void)
{
  switch (rtDW.is_Stop_Order) {
   case IN_Init_c:
    if (QT_StopOrder_flg == 1) {
      QT_StopOrder_flg = 0U;
      rtDW.is_Stop_Order = IN_Send_Init;
      Stop_Deal();
      Sys_sta[11] = 2U;
    }
    break;

   case IN_Init2:
    rtDW.is_Stop_Order = IN_Init_c;
    Sys_sta[11] = 1U;
    QT_StopOrder_flg = 0U;
    MCE_GetStop_flg = 0U;
    break;

   case IN_Init3:
    rtDW.is_Stop_Order = IN_Init_c;
    Sys_sta[11] = 1U;
    QT_StopOrder_flg = 0U;
    MCE_GetStop_flg = 0U;
    break;

   case IN_Init4:
    /* [MCE_RunState==5] */
    rtDW.is_Stop_Order = IN_Init5;

    /* Send_Stop_ok = 1; */
    Sys_sta[11] = 4U;
    break;

   case IN_Init5:
    rtDW.is_Stop_Order = IN_Init_c;
    Sys_sta[11] = 1U;
    QT_StopOrder_flg = 0U;
    MCE_GetStop_flg = 0U;
    break;

   default:
    if (MCE_GetStop_flg >= 1) {
      rtDW.MCE_Stop_Times = 0.0;
      if (MCE_GetStop_flg == 1) {
        MCE_GetStop_flg = 0U;

        /* Send_CE_STOP_flg=0; */
        rtDW.MCE_Stop_count = 0.0;
        rtDW.MCE_Stop_Times++;
        Send_CE_STOP_flg = 0U;
        rtDW.is_Stop_Order = IN_Init4;
        Sys_sta[11] = 3U;
      } else {
        MCE_Order_err = 1U;

        /* Send_CE_STOP_flg=0; */
        rtDW.MCE_Stop_count = 0.0;
        rtDW.MCE_Stop_Times++;
        Send_CE_STOP_flg = 0U;
        rtDW.is_Stop_Order = IN_Init_c;
        Sys_sta[11] = 1U;
        QT_StopOrder_flg = 0U;
        MCE_GetStop_flg = 0U;
      }
    } else if (rtDW.MCE_Stop_count >= 100.0) {
      /* Send_CE_STOP_flg=0; */
      rtDW.MCE_Stop_count = 0.0;
      rtDW.MCE_Stop_Times++;
      Send_CE_STOP_flg = 0U;
      rtDW.is_Stop_Order = IN_Send_Init;
      Stop_Deal();
      Sys_sta[11] = 2U;
    } else if (rtDW.MCE_Stop_Times >= 3.0) {
      rtDW.MCE_Stop_Times = 0.0;

      /* Send_CE_STOP_flg=0; */
      rtDW.MCE_Stop_count = 0.0;
      rtDW.MCE_Stop_Times++;
      Send_CE_STOP_flg = 0U;
      rtDW.is_Stop_Order = IN_Init3;
      MCE_StopOrder_err = 1U;
    } else {
      rtDW.MCE_Stop_count++;
    }
    break;
  }
}

/* Function for Chart: '<Root>/Chart' */
void CELocator_Deal(void)
{
  if ((MCE_ErrData == 0) && (MCE_RunState == 1) && (MCE_RGVSenser_sta == 1) &&
      (MCE_StackerSenser_sta == 1) && (MCE_ParkPlatStepData == 1) &&
      (MCE_RGVStepData == 1)) {
    Send_CE_CHANGELOCATOR_flg = 1U;
  } else {
    Send_CE_CHANGELOCATOR_flg = 0U;
    MCE_Order_err = 1U;
  }

  /* MCE_InLocator_dest = 1; */
}

/* Function for Chart: '<Root>/Chart' */
void CELocator_Order(void)
{
  switch (rtDW.is_CELocator_Order) {
   case IN_Init_c:
    if (QT_CELocatorOrder_flg == 1) {
      QT_CELocatorOrder_flg = 0U;
      rtDW.is_CELocator_Order = IN_Send_Init;
      CELocator_Deal();
      Sys_sta[4] = 2U;
    }
    break;

   case IN_Init2:
    rtDW.is_CELocator_Order = IN_Init_c;
    Sys_sta[4] = 1U;
    QT_CELocatorOrder_flg = 0U;
    MCE_GetCELocator_flg = 0U;
    break;

   case IN_Init3:
    rtDW.is_CELocator_Order = IN_Init_c;
    Sys_sta[4] = 1U;
    QT_CELocatorOrder_flg = 0U;
    MCE_GetCELocator_flg = 0U;
    break;

   case IN_Init4:
    if ((MCE_GetCELocatorOk_flg == 1) || (rtDW.Wait_CELocator_cou >= 18000.0)) {
      rtDW.Wait_CELocator_cou = 0.0;
      rtDW.is_CELocator_Order = IN_Init5;
      MCE_GetCELocatorOk_flg = 0U;
      Sys_sta[4] = 4U;
      MRemo_CELocator_ok = 1U;
    } else {
      rtDW.Wait_CELocator_cou++;
    }
    break;

   case IN_Init5:
    rtDW.is_CELocator_Order = IN_Init_c;
    Sys_sta[4] = 1U;
    QT_CELocatorOrder_flg = 0U;
    MCE_GetCELocator_flg = 0U;
    break;

   default:
    if (MCE_GetCELocator_flg >= 1) {
      rtDW.MCE_CELocator_Times = 0.0;
      if (MCE_GetCELocator_flg == 1) {
        MCE_GetCELocator_flg = 0U;

        /* Send_CE_CHANGELOCATOR_flg=0; */
        rtDW.MCE_CELocator_count = 0.0;
        rtDW.MCE_CELocator_Times++;
        Send_CE_CHANGELOCATOR_flg = 0U;
        rtDW.is_CELocator_Order = IN_Init4;
        Sys_sta[4] = 3U;
      } else {
        MCE_Order_err = 1U;

        /* Send_CE_CHANGELOCATOR_flg=0; */
        rtDW.MCE_CELocator_count = 0.0;
        rtDW.MCE_CELocator_Times++;
        Send_CE_CHANGELOCATOR_flg = 0U;
        rtDW.is_CELocator_Order = IN_Init_c;
        Sys_sta[4] = 1U;
        QT_CELocatorOrder_flg = 0U;
        MCE_GetCELocator_flg = 0U;
      }
    } else if (rtDW.MCE_CELocator_count >= 100.0) {
      /* Send_CE_CHANGELOCATOR_flg=0; */
      rtDW.MCE_CELocator_count = 0.0;
      rtDW.MCE_CELocator_Times++;
      Send_CE_CHANGELOCATOR_flg = 0U;
      rtDW.is_CELocator_Order = IN_Send_Init;
      CELocator_Deal();
      Sys_sta[4] = 2U;
    } else if (MCE_Order_err == 1) {
      rtDW.MCE_CELocator_Times = 0.0;

      /* Send_CE_CHANGELOCATOR_flg=0; */
      rtDW.MCE_CELocator_count = 0.0;
      rtDW.MCE_CELocator_Times++;
      Send_CE_CHANGELOCATOR_flg = 0U;
      rtDW.is_CELocator_Order = IN_Init2;
      MCE_Order_err = 1U;
    } else if (rtDW.MCE_CELocator_Times >= 3.0) {
      rtDW.MCE_CELocator_Times = 0.0;

      /* Send_CE_CHANGELOCATOR_flg=0; */
      rtDW.MCE_CELocator_count = 0.0;
      rtDW.MCE_CELocator_Times++;
      Send_CE_CHANGELOCATOR_flg = 0U;
      rtDW.is_CELocator_Order = IN_Init3;
      MCE_CELocatorOrder_err = 1U;
    } else {
      rtDW.MCE_CELocator_count++;
    }
    break;
  }
}

/* Function for Chart: '<Root>/Chart' */
void ChangeBat_Deal(void)
{
  if ((MCE_ErrData == 0) && ((MCE_RunState == 7) || (MCE_RunState == 1)) &&
      (MCE_StackerSenser_sta == 1) && (MCE_RGVSenser_sta == 1) &&
      (MCE_RGVStepData == 1) && (MCE_StackerStepData == 1) && (MCE_ControlMode ==
       2)) {
    Send_CE_CHANGEBAT_flg = 1U;
  } else {
    Send_CE_CHANGEBAT_flg = 0U;
    MCE_Order_err = 1U;
  }

  /* MCE_CBLocator_sour = 1; */
}

/* Function for Chart: '<Root>/Chart' */
void ChangeBatt_Order(void)
{
  switch (rtDW.is_ChangeBatt_Order) {
   case IN_Init_c:
    if (QT_ChangeBatOrder_flg == 1) {
      QT_ChangeBatOrder_flg = 0U;
      rtDW.is_ChangeBatt_Order = IN_Send_Init;
      ChangeBat_Deal();
      Sys_sta[7] = 2U;
    }
    break;

   case IN_Init2:
    rtDW.is_ChangeBatt_Order = IN_Init_c;
    Sys_sta[7] = 1U;
    QT_ChangeBatOrder_flg = 0U;
    MCE_GetChangeBat_flg = 0U;
    break;

   case IN_Init3:
    rtDW.is_ChangeBatt_Order = IN_Init_c;
    Sys_sta[7] = 1U;
    QT_ChangeBatOrder_flg = 0U;
    MCE_GetChangeBat_flg = 0U;
    break;

   case IN_Init4:
    if ((MCE_GetCBOk_flg == 1) || (rtDW.Wait_ChangeBat_cou >= 24000.0)) {
      rtDW.Wait_ChangeBat_cou = 0.0;
      rtDW.is_ChangeBatt_Order = IN_Init5;
      MCE_GetCBOk_flg = 0U;
      Sys_sta[7] = 4U;
      MRemo_ChangeBat_ok = 1U;
    } else {
      rtDW.Wait_ChangeBat_cou++;
    }
    break;

   case IN_Init5:
    rtDW.is_ChangeBatt_Order = IN_Init_c;
    Sys_sta[7] = 1U;
    QT_ChangeBatOrder_flg = 0U;
    MCE_GetChangeBat_flg = 0U;
    break;

   default:
    if (MCE_GetChangeBat_flg >= 1) {
      rtDW.MCE_ChangeBat_Times = 0.0;
      if (MCE_GetChangeBat_flg == 1) {
        MCE_GetChangeBat_flg = 0U;

        /* Send_CE_CHANGEBAT_flg = 0; */
        rtDW.MCE_ChangeBat_count = 0.0;
        rtDW.MCE_ChangeBat_Times++;
        Send_CE_CHANGEBAT_flg = 0U;
        rtDW.is_ChangeBatt_Order = IN_Init4;
        Sys_sta[7] = 3U;
      } else {
        MCE_Order_err = 1U;

        /* Send_CE_CHANGEBAT_flg = 0; */
        rtDW.MCE_ChangeBat_count = 0.0;
        rtDW.MCE_ChangeBat_Times++;
        Send_CE_CHANGEBAT_flg = 0U;
        rtDW.is_ChangeBatt_Order = IN_Init_c;
        Sys_sta[7] = 1U;
        QT_ChangeBatOrder_flg = 0U;
        MCE_GetChangeBat_flg = 0U;
      }
    } else if (rtDW.MCE_ChangeBat_count >= 100.0) {
      /* Send_CE_CHANGEBAT_flg = 0; */
      rtDW.MCE_ChangeBat_count = 0.0;
      rtDW.MCE_ChangeBat_Times++;
      Send_CE_CHANGEBAT_flg = 0U;
      rtDW.is_ChangeBatt_Order = IN_Send_Init;
      ChangeBat_Deal();
      Sys_sta[7] = 2U;
    } else if (MCE_Order_err == 1) {
      rtDW.MCE_ChangeBat_Times = 0.0;

      /* Send_CE_CHANGEBAT_flg = 0; */
      rtDW.MCE_ChangeBat_count = 0.0;
      rtDW.MCE_ChangeBat_Times++;
      Send_CE_CHANGEBAT_flg = 0U;
      rtDW.is_ChangeBatt_Order = IN_Init2;
      MCE_Order_err = 1U;
    } else if (rtDW.MCE_ChangeBat_Times >= 3.0) {
      rtDW.MCE_ChangeBat_Times = 0.0;

      /* Send_CE_CHANGEBAT_flg = 0; */
      rtDW.MCE_ChangeBat_count = 0.0;
      rtDW.MCE_ChangeBat_Times++;
      Send_CE_CHANGEBAT_flg = 0U;
      rtDW.is_ChangeBatt_Order = IN_Init3;
      MCE_ChangeBatOrder_err = 1U;
    } else {
      rtDW.MCE_ChangeBat_count++;
    }
    break;
  }
}

/* Function for Chart: '<Root>/Chart' */
void MountBat_Deal(void)
{
  if ((MCE_ErrData == 0) && (MCE_RunState == 1) && (MCE_RGVSenser_sta == 2) &&
      (MCE_ParkPlatStepData == 1) && (MCE_RGVStepData == 1) &&
      (MCE_StackerStepData == 1)) {
    Send_CE_MOUNTBAT_flg = 1U;
  } else {
    Send_CE_MOUNTBAT_flg = 0U;
    MCE_Order_err = 1U;
  }

  /* MCE_InLocator_dest = 1; */
}

/* Function for Chart: '<Root>/Chart' */
void ErrReset_Deal(void)
{
  Send_CE_ERRRESET_flg = 1U;

  /* MCE_InLocator_dest = 1; */
}

/* Function for Chart: '<Root>/Chart' */
void Hand_CE(void)
{
  Remo_Deal();
  TimeOut_Order();
  BatInLocator_Order();
  Init_Order();
  Continue_Order();
  BatOutLocator_Order();
  BatOut_Order();
  Stop_Order();
  CELocator_Order();
  ChangeBatt_Order();
  switch (rtDW.is_MountBat_Order) {
   case IN_Init_c:
    if (QT_MountBatOrder_flg == 1) {
      QT_MountBatOrder_flg = 0U;
      rtDW.is_MountBat_Order = IN_Send_Init;
      MountBat_Deal();
      Sys_sta[8] = 2U;
    }
    break;

   case IN_Init2:
    rtDW.is_MountBat_Order = IN_Init_c;
    Sys_sta[8] = 1U;
    MCE_GetMountBat_flg = 0U;
    break;

   case IN_Init3:
    rtDW.is_MountBat_Order = IN_Init_c;
    Sys_sta[8] = 1U;
    MCE_GetMountBat_flg = 0U;
    break;

   case IN_Init4:
    if (MCE_GetMountBatOk_flg == 1) {
      rtDW.is_MountBat_Order = IN_Init5;
      MCE_GetMountBatOk_flg = 0U;
      Sys_sta[8] = 4U;
    }
    break;

   case IN_Init5:
    rtDW.is_MountBat_Order = IN_Init_c;
    Sys_sta[8] = 1U;
    MCE_GetMountBat_flg = 0U;
    break;

   default:
    if (MCE_GetMountBat_flg >= 1) {
      MCE_Order_err = 1U;
      rtDW.MCE_MountBat_count = 0.0;
      rtDW.MCE_MountBat_Times++;
      Send_CE_MOUNTBAT_flg = 0U;
      rtDW.is_MountBat_Order = IN_Init_c;
      Sys_sta[8] = 1U;
      MCE_GetMountBat_flg = 0U;
    } else if (rtDW.MCE_MountBat_count >= 100.0) {
      rtDW.MCE_MountBat_count = 0.0;
      rtDW.MCE_MountBat_Times++;
      Send_CE_MOUNTBAT_flg = 0U;
      rtDW.is_MountBat_Order = IN_Send_Init;
      MountBat_Deal();
      Sys_sta[8] = 2U;
    } else if (MCE_Order_err == 1) {
      rtDW.MCE_MountBat_count = 0.0;
      rtDW.MCE_MountBat_Times++;
      Send_CE_MOUNTBAT_flg = 0U;
      rtDW.is_MountBat_Order = IN_Init2;
      MCE_Order_err = 1U;
    } else if (rtDW.MCE_MountBat_Times >= 3.0) {
      rtDW.MCE_MountBat_Times = 0.0;
      rtDW.MCE_MountBat_count = 0.0;
      rtDW.MCE_MountBat_Times++;
      Send_CE_MOUNTBAT_flg = 0U;
      rtDW.is_MountBat_Order = IN_Init3;
      MCE_MountBatOrder_err = 1U;
    } else {
      rtDW.MCE_MountBat_count++;
      Send_CE_MOUNTBAT_flg = 0U;
    }
    break;
  }

  switch (rtDW.is_DisMountBat_Order) {
   case IN_Init_c:
    if (QT_MountBatOrder_flg == 1) {
      QT_MountBatOrder_flg = 0U;
      rtDW.is_DisMountBat_Order = IN_Send_Init;
      MountBat_Deal();
      Sys_sta[6] = 2U;
    }
    break;

   case IN_Init2:
    rtDW.is_DisMountBat_Order = IN_Init_c;
    Sys_sta[6] = 1U;
    MCE_GetDismountBat_flg = 0U;
    break;

   case IN_Init3:
    rtDW.is_DisMountBat_Order = IN_Init_c;
    Sys_sta[6] = 1U;
    MCE_GetDismountBat_flg = 0U;
    break;

   case IN_Init4:
    if (MCE_GetMountBatOk_flg == 1) {
      MCE_GetMountBatOk_flg = 0U;
      rtDW.is_DisMountBat_Order = IN_Init5;
      MCE_GetDismountBatOk_flg = 0U;
      Sys_sta[6] = 4U;
    }
    break;

   case IN_Init5:
    rtDW.is_DisMountBat_Order = IN_Init_c;
    Sys_sta[6] = 1U;
    MCE_GetDismountBat_flg = 0U;
    break;

   default:
    if (MCE_GetMountBat_flg >= 1) {
      if (MCE_GetMountBat_flg == 1) {
        MCE_GetMountBat_flg = 0U;
        rtDW.MCE_DismountBat_count = 0.0;
        rtDW.MCE_DismountBat_Times++;
        Send_CE_DISMOUNTBAT_flg = 0U;
        rtDW.is_DisMountBat_Order = IN_Init4;
        Sys_sta[6] = 3U;
      } else {
        MCE_Order_err = 1U;
        MCE_GetMountBat_flg = 0U;
        rtDW.MCE_DismountBat_count = 0.0;
        rtDW.MCE_DismountBat_Times++;
        Send_CE_DISMOUNTBAT_flg = 0U;
        rtDW.is_DisMountBat_Order = IN_Init_c;
        Sys_sta[6] = 1U;
        MCE_GetDismountBat_flg = 0U;
      }
    } else if (rtDW.MCE_MountBat_count >= 100.0) {
      rtDW.MCE_MountBat_count = 0.0;
      rtDW.MCE_DismountBat_count = 0.0;
      rtDW.MCE_DismountBat_Times++;
      Send_CE_DISMOUNTBAT_flg = 0U;
      rtDW.is_DisMountBat_Order = IN_Send_Init;
      MountBat_Deal();
      Sys_sta[6] = 2U;
    } else if (MCE_Order_err == 1) {
      rtDW.MCE_DismountBat_count = 0.0;
      rtDW.MCE_DismountBat_Times++;
      Send_CE_DISMOUNTBAT_flg = 0U;
      rtDW.is_DisMountBat_Order = IN_Init2;
      MCE_Order_err = 1U;
    } else if (rtDW.MCE_MountBat_Times >= 3.0) {
      rtDW.MCE_MountBat_Times = 0.0;
      rtDW.MCE_DismountBat_count = 0.0;
      rtDW.MCE_DismountBat_Times++;
      Send_CE_DISMOUNTBAT_flg = 0U;
      rtDW.is_DisMountBat_Order = IN_Init3;
      MCE_DismountBatOrder_err = 1U;
    } else {
      rtDW.MCE_DismountBat_count++;
      Send_CE_DISMOUNTBAT_flg = 0U;
    }
    break;
  }

  switch (rtDW.is_ErrReset_Order) {
   case IN_Init_c:
    if (QT_ErrResetOrder_flg == 1) {
      QT_ErrResetOrder_flg = 0U;
      rtDW.is_ErrReset_Order = IN_Send_Init_o;
      ErrReset_Deal();
      Sys_sta[12] = 2U;
    }
    break;

   case IN_Init2:
    rtDW.is_ErrReset_Order = IN_Init_c;
    Sys_sta[12] = 1U;
    QT_ErrResetOrder_flg = 0U;
    MCE_GetErrReset_flg = 0U;
    break;

   case IN_Init3:
    rtDW.is_ErrReset_Order = IN_Init_c;
    Sys_sta[12] = 1U;
    QT_ErrResetOrder_flg = 0U;
    MCE_GetErrReset_flg = 0U;
    break;

   case IN_Init4:
    rtDW.is_ErrReset_Order = IN_Init_c;
    Sys_sta[12] = 1U;
    QT_ErrResetOrder_flg = 0U;
    MCE_GetErrReset_flg = 0U;
    break;

   default:
    if (MCE_GetErrReset_flg >= 1) {
      rtDW.MCE_ErrReset_Times = 0.0;
      if (MCE_GetErrReset_flg == 1) {
        MCE_GetErrReset_flg = 0U;

        /* Send_CE_ERRRESET_flg=0; */
        rtDW.MCE_ErrReset_count = 0.0;
        rtDW.MCE_ErrReset_Times++;
        Send_CE_ERRRESET_flg = 0U;
        rtDW.is_ErrReset_Order = IN_Init4;
        Sys_sta[12] = 3U;
      } else {
        MCE_Order_err = 1U;

        /* Send_CE_ERRRESET_flg=0; */
        rtDW.MCE_ErrReset_count = 0.0;
        rtDW.MCE_ErrReset_Times++;
        Send_CE_ERRRESET_flg = 0U;
        rtDW.is_ErrReset_Order = IN_Init_c;
        Sys_sta[12] = 1U;
        QT_ErrResetOrder_flg = 0U;
        MCE_GetErrReset_flg = 0U;
      }
    } else if (rtDW.MCE_ErrReset_count >= 100.0) {
      /* Send_CE_ERRRESET_flg=0; */
      rtDW.MCE_ErrReset_count = 0.0;
      rtDW.MCE_ErrReset_Times++;
      Send_CE_ERRRESET_flg = 0U;
      rtDW.is_ErrReset_Order = IN_Send_Init_o;
      ErrReset_Deal();
      Sys_sta[12] = 2U;
    } else if (rtDW.MCE_ErrReset_Times >= 3.0) {
      rtDW.MCE_ErrReset_Times = 0.0;

      /* Send_CE_ERRRESET_flg=0; */
      rtDW.MCE_ErrReset_count = 0.0;
      rtDW.MCE_ErrReset_Times++;
      Send_CE_ERRRESET_flg = 0U;
      rtDW.is_ErrReset_Order = IN_Init3;
      MCE_ErrResetOrder_err = 1U;
    } else {
      rtDW.MCE_ErrReset_count++;
    }
    break;
  }
}

/* Function for Chart: '<Root>/Chart' */
void All_CheckErr(void)
{
  int32_T i;
  int32_T exitg1;
  if (FireContr_GetFire_flg == 1) {
    MSys_AllErr_Lev = 4U;
  } else if ((MCE_ErrData == 1) || (MIO_Comm_flg == 2) || (MCE_Comm_flg == 2) ||
             (MCloud_Comm_flg == 2) || (MIO_Door_err == 1) || (MIO_Water_err ==
              1) || (MIO_Thunder_err == 1) || (MIO_Light_err == 1)) {
    MSys_AllErr_Lev = 3U;
  } else {
    MSys_AllErr_Lev = (uint8_T)((FireContr_conn_flg == 2) || (AirCon_conn_flg ==
      2) || (MGrab_Comm_flg == 2) || (MLed_Comm_flg == 2));
  }

  /* Sound_Start_flg = 1;
     Light_Start_flg = 1;
     MSys_AllErr_Lev = 3; */
  if ((MQt_DisaSoundLigErr_flg == 0) && (MSys_AllErr_Lev >= 3)) {
    MSound_Start_flg = 1U;
    MLight_Start_flg = 1U;
  } else {
    MSound_Start_flg = 0U;
    MLight_Start_flg = 0U;
  }

  i = 0;
  do {
    exitg1 = 0;
    if (EMS_Chger_BigSta[i] == 3) {
      MFan_Start_flg = 1U;
      exitg1 = 1;
    } else if (i < 19) {
      i++;
    } else {
      MFan_Start_flg = 0U;
      exitg1 = 1;
    }
  } while (exitg1 == 0);
}

/* Function for Chart: '<Root>/Chart' */
void CheckBookBag_Deal(void)
{
  int32_T i;
  real_T BookLock;
  int32_T BookValue;
  int32_T Min_soc;
  int32_T Min_soc_num;
  int32_T j;
  boolean_T exitg1;
  boolean_T exitg2;
  int32_T exitg3;
  int32_T exitg4;
  boolean_T guard1 = false;
  guard1 = false;
  if (MLocat_CheckLock_flg >= 5) {
    rtDW.LockInit_cou++;
    if (rtDW.LockInit_cou >= 3000.0) {
      MLocat_CheckLock_flg = 1U;
      Book_Init_flg = 1U;
      guard1 = true;
    }
  } else {
    guard1 = true;
  }

  if (guard1) {
    if (MLocat_CheckLock_flg == 1) {
      MLocat_CheckLock_flg = 0U;
      i = 0;
      j = Locat_Book_num[0] - MLocat_RealBook_num[0];
      Min_soc = 100;
      Min_soc_num = 0;
      BookValue = 0;
      exitg2 = false;
      while ((!exitg2) && (j > 0)) {
        j--;
        do {
          exitg3 = 0;
          if ((MLocat_Conf_ena[i] == 1) && (MLocat_SelfChar_ena[i] == 1) &&
              (MLocat_InNet_flg[i] == 1) && (MLocat_GetBatCode_flg[i] == 1) &&
              (MLocat_Car_type[i] == 1) && (MLocat_BookLock_flg[i] == 0) &&
              (CHU_AllErr_flg[i] == 0) && (Min_soc >= MLocat_Soc_pct[i])) {
            Min_soc = MLocat_Soc_pct[i];
            Min_soc_num = i;
            BookValue = 1;
          }

          if (i < 19) {
            i++;
          } else {
            exitg3 = 1;
          }
        } while (exitg3 == 0);

        if (BookValue == 1) {
          MLocat_BookLock_flg[Min_soc_num] = 1U;
          i = MLocat_RealBook_num[0] + 1;
          if (i > 255) {
            i = 255;
          }

          MLocat_RealBook_num[0] = (uint8_T)i;
        } else {
          /* MLocat_BookLock_flg[Min_soc_num]=0; */
        }

        if (j > 0) {
          i = 0;
          Min_soc = 100;
          Min_soc_num = 0;
          BookValue = 0;
        } else {
          exitg2 = true;
        }
      }

      i = 0;
      j = Locat_Book_num[1] - MLocat_RealBook_num[1];
      Min_soc = 100;
      Min_soc_num = 0;
      BookValue = 0;
      exitg2 = false;
      while ((!exitg2) && (j > 0)) {
        j--;
        do {
          exitg3 = 0;
          if ((MLocat_Conf_ena[i] == 1) && (MLocat_SelfChar_ena[i] == 1) &&
              (MLocat_InNet_flg[i] == 1) && (MLocat_GetBatCode_flg[i] == 1) &&
              (MLocat_Car_type[i] == 2) && (MLocat_BookLock_flg[i] == 0) &&
              (CHU_AllErr_flg[i] == 0) && (Min_soc >= MLocat_Soc_pct[i])) {
            Min_soc = MLocat_Soc_pct[i];
            Min_soc_num = i;
            BookValue = 1;
          }

          if (i < 19) {
            i++;
          } else {
            exitg3 = 1;
          }
        } while (exitg3 == 0);

        if (BookValue == 1) {
          MLocat_BookLock_flg[Min_soc_num] = 1U;
          i = MLocat_RealBook_num[1] + 1;
          if (i > 255) {
            i = 255;
          }

          MLocat_RealBook_num[1] = (uint8_T)i;
        } else {
          /* MLocat_BookLock_flg[Min_soc_num]=0; */
        }

        if (j > 0) {
          i = 0;
          Min_soc = 100;
          Min_soc_num = 0;
          BookValue = 0;
        } else {
          exitg2 = true;
        }
      }

      i = 0;
      j = Locat_Book_num[2] - MLocat_RealBook_num[2];
      Min_soc = 100;
      Min_soc_num = 0;
      BookValue = 0;
      exitg1 = false;
      while ((!exitg1) && (j > 0)) {
        j--;
        do {
          exitg3 = 0;
          if ((MLocat_Conf_ena[i] == 1) && (MLocat_SelfChar_ena[i] == 1) &&
              (MLocat_InNet_flg[i] == 1) && (MLocat_GetBatCode_flg[i] == 1) &&
              (MLocat_Car_type[i] == 3) && (MLocat_BookLock_flg[i] == 0) &&
              (CHU_AllErr_flg[i] == 0) && (Min_soc >= MLocat_Soc_pct[i])) {
            Min_soc = MLocat_Soc_pct[i];
            Min_soc_num = i;
            BookValue = 1;
          }

          if (i < 19) {
            i++;
          } else {
            exitg3 = 1;
          }
        } while (exitg3 == 0);

        if (BookValue == 1) {
          MLocat_BookLock_flg[Min_soc_num] = 1U;
          i = MLocat_RealBook_num[2] + 1;
          if (i > 255) {
            i = 255;
          }

          MLocat_RealBook_num[2] = (uint8_T)i;
        } else {
          /* MLocat_BookLock_flg[Min_soc_num]=0; */
        }

        if (j > 0) {
          i = 0;
          Min_soc = 100;
          Min_soc_num = 0;
          BookValue = 0;
        } else {
          exitg1 = true;
        }
      }
    }
  }

  if (MLocat_CheckClear_flg == 1) {
    MLocat_CheckClear_flg = 0U;
    i = 0;
    j = 0;
    BookLock = 0.0;
    do {
      exitg3 = 0;
      j++;
      do {
        exitg4 = 0;
        if ((real_T)j - 1.0 >= -2.147483648E+9) {
          Min_soc = (int32_T)((real_T)j - 1.0);
        } else {
          Min_soc = MIN_int32_T;
        }

        if ((MLocat_BookLock_flg[i] == 1) && (MLocat_Car_type[i] == j) &&
            (Locat_Book_num[Min_soc] > 0)) {
          BookLock++;
        }

        if ((real_T)j - 1.0 >= -2.147483648E+9) {
          Min_soc = (int32_T)((real_T)j - 1.0);
        } else {
          Min_soc = MIN_int32_T;
        }

        if ((BookLock == Locat_Book_num[Min_soc]) && (BookLock > 0.0)) {
          if ((real_T)j - 1.0 >= -2.147483648E+9) {
            Min_soc = (int32_T)((real_T)j - 1.0);
            Min_soc_num = (int32_T)((real_T)j - 1.0);
          } else {
            Min_soc = MIN_int32_T;
            Min_soc_num = MIN_int32_T;
          }

          MLocat_RealBook_num[Min_soc] = Locat_Book_num[Min_soc_num];
          i++;
          exitg4 = 1;
        } else {
          if ((real_T)j - 1.0 >= -2.147483648E+9) {
            Min_soc = (int32_T)((real_T)j - 1.0);
          } else {
            Min_soc = MIN_int32_T;
          }

          if (Locat_Book_num[Min_soc] == 0) {
            i = 0;
            exitg4 = 1;
          } else if (i < 19) {
            i++;
          } else {
            exitg4 = 1;
          }
        }
      } while (exitg4 == 0);

      do {
        exitg4 = 0;
        if (MLocat_Car_type[i] == j) {
          MLocat_BookLock_flg[i] = 0U;
        }

        if (i < 19) {
          i++;
        } else {
          exitg4 = 1;
        }
      } while (exitg4 == 0);

      if (j < 3) {
        BookLock = 0.0;
        i = 0;
      } else {
        exitg3 = 1;
      }
    } while (exitg3 == 0);
  }

  i = 0;
  do {
    exitg4 = 0;
    if ((MLocat_BookLock_flg[i] == 1) && (CHU_AllErr_flg[i] == 1)) {
      MLocat_BookLock_flg[i] = 0U;
      j = 0;
      exitg4 = 2;
    } else if (i < 19) {
      i++;
    } else {
      exitg4 = 1;
    }
  } while (exitg4 == 0);

  if (exitg4 == 1) {
  } else {
    do {
      exitg3 = 0;
      if ((MLocat_Conf_ena[j] == 1) && (MLocat_SelfChar_ena[j] == 1) &&
          (MLocat_InNet_flg[j] == 1) && (MLocat_GetBatCode_flg[j] == 1) &&
          (MLocat_Car_type[j] == MLocat_Car_type[i]) && (MLocat_BookLock_flg[j] ==
           0) && (CHU_AllErr_flg[j] == 0)) {
        MLocat_BookLock_flg[j] = 1U;
        exitg3 = 1;
      } else if (j < 19) {
        j++;
      } else {
        exitg3 = 1;
      }
    } while (exitg3 == 0);
  }
}

/* Function for Chart: '<Root>/Chart' */
void MCE_Clear(void)
{
  rtDW.MCE_Comm_cou = 0.0;
  MCE_Comm_err = 0U;
}

/* Function for Chart: '<Root>/Chart' */
void MCE_Initok(void)
{
  int32_T i;
  int32_T exitg1;
  MCE_Initok_flg = (uint8_T)((MCE_RGVStepData == 1) && (MCE_StackerStepData == 1)
    && (MCE_ParkPlatStepData == 1));
  i = 0;
  do {
    exitg1 = 0;
    if (MLocat_InNet_flg[i] == 1) {
      MLocat_CheckInNet_ok = 1U;
      exitg1 = 1;
    } else {
      i++;
      if (i < 18) {
      } else {
        exitg1 = 1;
      }
    }
  } while (exitg1 == 0);
}

/* Function for Chart: '<Root>/Chart' */
void MIO_Clear(void)
{
  rtDW.MIO_Comm_cou = 0.0;
  MIO_Comm_err = 0U;
}

/* Function for Chart: '<Root>/Chart' */
void exit_internal_All_Self_Check(void)
{
  switch (rtDW.is_All_Self_Check) {
   case IN_MCE_COMM:
    rtDW.is_MCE_COMM = IN_NO_ACTIVE_CHILD_p;
    MCE_Clear();
    rtDW.is_All_Self_Check = IN_NO_ACTIVE_CHILD_p;
    break;

   case IN_MCE_InitOk:
    rtDW.is_MCE_InitOk = IN_NO_ACTIVE_CHILD_p;
    rtDW.MCE_Initok_cou = 0.0;
    MCE_Initok_err = 0U;
    rtDW.MLocat_CheckInNet_cou = 0.0;
    MCloud_SendmatchBatNo_flg = 0U;
    MLocat_CheckInNet_ok = 0U;
    rtDW.is_All_Self_Check = IN_NO_ACTIVE_CHILD_p;
    break;

   case IN_MCloud_COMM:
    rtDW.is_MCloud_COMM = IN_NO_ACTIVE_CHILD_p;
    rtDW.MCloud_Comm_cou = 0.0;
    MCloud_Comm_err = 0U;
    rtDW.is_All_Self_Check = IN_NO_ACTIVE_CHILD_p;
    break;

   case IN_MGrab_COMM:
    rtDW.is_MGrab_COMM = IN_NO_ACTIVE_CHILD_p;
    rtDW.MGrab_Comm_cou = 0.0;
    MGrab_Comm_err = 0U;
    rtDW.is_All_Self_Check = IN_NO_ACTIVE_CHILD_p;
    break;

   case IN_MIO_COMM:
    rtDW.is_MIO_COMM = IN_NO_ACTIVE_CHILD_p;
    MIO_Clear();
    rtDW.is_All_Self_Check = IN_NO_ACTIVE_CHILD_p;
    break;

   default:
    rtDW.is_All_Self_Check = IN_NO_ACTIVE_CHILD_p;
    break;
  }
}

/* Function for Chart: '<Root>/Chart' */
void All_Self_Check(void)
{
  if (MCE_SelfCheckOk_sta == 1) {
    MCE_SelfCheckOk_sta = 0U;
    exit_internal_All_Self_Check();
    rtDW.is_Main_CE = IN_Wait_Car;
    Sys_Big_sta = 1U;
    MCE_WriSqlShutDown_sta = 1U;
    Grab_GetCarCard_flg = 0U;
    Sta = 15U;
  } else if (MQt_StartSelfCE_sta != 1) {
    MCE_GetSqlInt_sta = 0U;
    exit_internal_All_Self_Check();
    rtDW.is_Main_CE = IN_Main_Init;
    Sys_Big_sta = 0U;
    rtDW.is_Main_Init = IN_Init3_bc;
    Sta = 0U;
  } else {
    switch (rtDW.is_All_Self_Check) {
     case IN_MCE_COMM:
      switch (rtDW.is_MCE_COMM) {
       case IN_Init_c:
        if ((MCE_Comm_flg == 1) && (MCE_ErrData == 0)) {
          rtDW.is_MCE_COMM = IN_Init1;
        } else if (rtDW.MCE_Comm_cou > 1000.0) {
          rtDW.MCE_Comm_cou = 0.0;
          rtDW.is_MCE_COMM = IN_Init2_k;
          Sta = 4U;

          /* MCE_Comm_err = 1; */
        } else {
          rtDW.MCE_Comm_cou++;

          /* MCE_Comm_flg=0; */
        }
        break;

       case IN_Init1:
        rtDW.is_MCE_COMM = IN_NO_ACTIVE_CHILD_p;
        MCE_Clear();
        rtDW.is_All_Self_Check = IN_MGrab_COMM;
        rtDW.is_MGrab_COMM = IN_Init_c;
        Sta = 5U;
        break;

       default:
        if ((MCE_Comm_flg == 1) && (MCE_ErrData == 0)) {
          MCE_Comm_err = 0U;
          rtDW.is_MCE_COMM = IN_Init1;
        }
        break;
      }
      break;

     case IN_MCE_InitOk:
      MCE_Initok();
      switch (rtDW.is_MCE_InitOk) {
       case IN_Init_c:
        if ((MCE_Initok_flg == 1) && (MCE_ControlMode == 2)) {
          rtDW.is_MCE_InitOk = IN_Init1;
          Sta = 11U;
        } else if (rtDW.MCE_Initok_cou > 1000.0) {
          rtDW.MCE_Initok_cou = 0.0;
          rtDW.is_MCE_InitOk = IN_Init2_k;
          Sta = 10U;
          MCE_Initok_err = 1U;
        } else {
          rtDW.MCE_Initok_cou++;

          /* MCE_Comm_flg=0; */
        }
        break;

       case IN_Init1:
        /* MCloud_SendmatchBatNo_flg=1 */
        rtDW.is_MCE_InitOk = IN_Init3_bci;
        Sta = 12U;
        break;

       case IN_Init2_k:
        if ((MCE_Initok_flg == 1) && (MCE_ControlMode == 2)) {
          MCE_Initok_err = 0U;
          rtDW.is_MCE_InitOk = IN_Init1;
          Sta = 11U;
        }
        break;

       case IN_Init3_bci:
        if ((rtDW.MLocat_CheckInNet_cou >= 10000.0) || (MLocat_CheckInNet_ok ==
             1)) {
          rtDW.MLocat_CheckInNet_cou = 0.0;
          rtDW.is_MCE_InitOk = IN_Init4_m2;
          Sta = 13U;
        } else {
          /* MLocat_CheckInNet_cou++; */
        }
        break;

       default:
        if (MQt_StartSelfCE_sta == 1) {
          rtDW.is_MCE_InitOk = IN_NO_ACTIVE_CHILD_p;
          rtDW.MCE_Initok_cou = 0.0;
          MCE_Initok_err = 0U;
          rtDW.MLocat_CheckInNet_cou = 0.0;
          MCloud_SendmatchBatNo_flg = 0U;
          MLocat_CheckInNet_ok = 0U;
          rtDW.is_All_Self_Check = IN_Wait_SelfMode;
          Sta = 14U;
          MCE_SelfCheckOk_sta = 1U;
        }
        break;
      }
      break;

     case IN_MCloud_COMM:
      switch (rtDW.is_MCloud_COMM) {
       case IN_Init_c:
        if (MCloud_Comm_flg == 1) {
          rtDW.is_MCloud_COMM = IN_Init1;
        } else if (rtDW.MCloud_Comm_cou > 3000.0) {
          rtDW.MCloud_Comm_cou = 0.0;
          rtDW.is_MCloud_COMM = IN_Init2_k;
          Sta = 8U;
          MCloud_Comm_err = 1U;
        } else {
          rtDW.MCloud_Comm_cou++;

          /* MCE_Comm_flg=0; */
        }
        break;

       case IN_Init1:
        rtDW.is_MCloud_COMM = IN_NO_ACTIVE_CHILD_p;
        rtDW.MCloud_Comm_cou = 0.0;
        MCloud_Comm_err = 0U;
        rtDW.is_All_Self_Check = IN_MCE_InitOk;
        rtDW.is_MCE_InitOk = IN_Init_c;
        Sta = 9U;
        break;

       default:
        if (MCloud_Comm_flg == 1) {
          MCloud_Comm_err = 0U;
          rtDW.is_MCloud_COMM = IN_Init1;
        }
        break;
      }
      break;

     case IN_MGrab_COMM:
      switch (rtDW.is_MGrab_COMM) {
       case IN_Init_c:
        if ((MGrab_Comm_flg == 1) && (MLed_Comm_flg == 1)) {
          rtDW.is_MGrab_COMM = IN_Init1;
        } else if (rtDW.MGrab_Comm_cou > 1000.0) {
          rtDW.MGrab_Comm_cou = 0.0;
          rtDW.is_MGrab_COMM = IN_Init2_k;
          Sta = 6U;
          MGrab_Comm_err = 1U;
        } else {
          rtDW.MGrab_Comm_cou++;

          /* MCE_Comm_flg=0; */
        }
        break;

       case IN_Init1:
        rtDW.is_MGrab_COMM = IN_NO_ACTIVE_CHILD_p;
        rtDW.MGrab_Comm_cou = 0.0;
        MGrab_Comm_err = 0U;
        rtDW.is_All_Self_Check = IN_MCloud_COMM;
        rtDW.is_MCloud_COMM = IN_Init_c;
        Sta = 7U;
        break;

       default:
        if ((MGrab_Comm_flg == 1) && (MLed_Comm_flg == 1)) {
          MGrab_Comm_err = 0U;
          rtDW.is_MGrab_COMM = IN_Init1;
        }
        break;
      }
      break;

     case IN_MIO_COMM:
      switch (rtDW.is_MIO_COMM) {
       case IN_Init_c:
        if ((MIO_Comm_flg == 1) && (MIO_Door_err == 0) && (MIO_Water_err == 0) &&
            (FireContr_GetFire_flg == 0) && (MIO_Thunder_err == 0) &&
            (MIO_Light_err == 0) && (FireContr_GetFire_flg == 0)) {
          rtDW.is_MIO_COMM = IN_Init1;
        } else if (rtDW.MIO_Comm_cou > 1000.0) {
          rtDW.MIO_Comm_cou = 0.0;
          rtDW.is_MIO_COMM = IN_Init2_k;
          Sta = 2U;
          MIO_Comm_err = 1U;
        } else {
          rtDW.MIO_Comm_cou++;

          /* MCE_Comm_flg=0; */
        }
        break;

       case IN_Init1:
        rtDW.is_MIO_COMM = IN_NO_ACTIVE_CHILD_p;
        MIO_Clear();
        rtDW.is_All_Self_Check = IN_MCE_COMM;
        rtDW.is_MCE_COMM = IN_Init_c;
        Sta = 3U;
        break;

       default:
        if ((MIO_Comm_flg == 1) && (MIO_Door_err == 0) && (MIO_Water_err == 0) &&
            (FireContr_GetFire_flg == 0) && (MIO_Thunder_err == 0) &&
            (MIO_Light_err == 0) && (FireContr_GetFire_flg == 0)) {
          MIO_Comm_err = 0U;
          rtDW.is_MIO_COMM = IN_Init1;
        }
        break;
      }
      break;
    }
  }
}

/* Function for Chart: '<Root>/Chart' */
void exit_atomic_Allow_InStation(void)
{
  rtDW.MCloud_CarAllowIn_cou = 0.0;
  rtDW.MCloud_CarAllowIn_time = 0.0;
  MCloud_SendCarCard_flg = 0U;
  MCloud_CarAllowIn_err = 0U;
  rtDW.CheckBag_time = 0.0;
  MCloud_CarAllowIn_sta = 0U;
  MCloud_GetCarType_flg = 0U;
  rtDW.MCloud_GetCarType_cou = 0.0;
  MCloud_SendGetCarType_flg = 0U;
  Cloud_OutNetCheckBatNum_flg = 0U;
  CE_OutNetSelfCe_sour = 0U;
  Cloud_BookCarNo_flg = 0U;
}

/* Function for Chart: '<Root>/Chart' */
void CheckBatBag_Deal1(void)
{
  int32_T i;
  int32_T Max_soc;
  int32_T Max_soc_num;
  int32_T exitg1;
  boolean_T exitg2;
  i = 0;
  Max_soc = 0;
  MCE_SelfCheckOk_sta = 0U;
  Max_soc_num = 0;
  if ((Cloud_carType_Code >= 1) && (Cloud_BookCarNo_flg == 0)) {
    do {
      exitg1 = 0;
      if ((MLocat_Conf_ena[MLocat_FIFO_Buf[i]] == 1) &&
          (MLocat_SelfChar_ena[MLocat_FIFO_Buf[i]] == 1) &&
          (MLocat_InNet_flg[MLocat_FIFO_Buf[i]] == 1) &&
          (MLocat_GetBatCode_flg[MLocat_FIFO_Buf[i]] == 1) &&
          (MLocat_Car_type[MLocat_FIFO_Buf[i]] == Cloud_carType_Code) &&
          (MLocat_BookLock_flg[MLocat_FIFO_Buf[i]] == 0) &&
          (CHU_AllErr_flg[MLocat_FIFO_Buf[i]] == 0) && (Max_soc <
           MLocat_Soc_pct[MLocat_FIFO_Buf[i]])) {
        Max_soc = MLocat_Soc_pct[MLocat_FIFO_Buf[i]];
        Max_soc_num = MLocat_FIFO_Buf[i];
      }

      if (i < 19) {
        i++;
      } else {
        exitg1 = 1;
      }
    } while (exitg1 == 0);

    if (Max_soc > MLocat_AllowCESoc_pct) {
      MChar_SelfCe_sour = (uint8_T)Max_soc_num;
      MCE_SelfCheckOk_sta = 1U;
    } else {
      MCE_SelfCheckOk_sta = 2U;
    }
  } else if ((Cloud_carType_Code >= 1) && (Cloud_BookCarNo_flg == 1)) {
    Cloud_BookCarNo_flg = 0U;
    MCE_SelfCheckOk_sta = 0U;
    do {
      exitg1 = 0;
      if ((MLocat_Conf_ena[i] == 1) && (MLocat_SelfChar_ena[i] == 1) &&
          (MLocat_InNet_flg[i] == 1) && (MLocat_GetBatCode_flg[i] == 1) &&
          (MLocat_Car_type[i] == Cloud_carType_Code) && (CHU_AllErr_flg[i] == 0)
          && (Max_soc <= MLocat_Soc_pct[i])) {
        Max_soc = MLocat_Soc_pct[i];
        Max_soc_num = i;
      }

      if (i < 19) {
        i++;
      } else {
        exitg1 = 1;
      }
    } while (exitg1 == 0);

    if (Max_soc > MLocat_AllowCESoc_pct) {
      MChar_SelfCe_sour = (uint8_T)Max_soc_num;
      MCE_SelfCheckOk_sta = 1U;
      MLocat_BookLock_flg[MChar_SelfCe_sour] = 0U;
    } else {
      MCE_SelfCheckOk_sta = 2U;
      i = 0;
      exitg2 = false;
      while ((!exitg2) && (i < 19)) {
        if (MLocat_BookLock_flg[i] == 1) {
          MLocat_BookLock_flg[i] = 0U;
          exitg2 = true;
        } else {
          i++;
        }
      }
    }
  } else {
    MCE_SelfCheckOk_sta = 2U;
  }
}

/* Function for Chart: '<Root>/Chart' */
void exit_internal_Allow_InStation(void)
{
  switch (rtDW.is_Allow_InStation) {
   case IN_Init_c:
    MCloud_SendCarCard_flg = 0U;
    rtDW.MCloud_CarAllowIn_time++;
    rtDW.is_Allow_InStation = IN_NO_ACTIVE_CHILD_p;
    break;

   case IN_Init4:
    rtDW.CheckBag_time++;
    MCloud_CarAllowIn_sta = 0U;
    rtDW.MCloud_CarAllowIn_time = 0.0;
    rtDW.is_Allow_InStation = IN_NO_ACTIVE_CHILD_p;
    break;

   case IN_Init5:
    if (rtDW.is_Init5 == IN_Init_c) {
      MQt_SendPoint_flg = 0U;
      rtDW.MQt_SendPoint_time++;
      rtDW.is_Init5 = IN_NO_ACTIVE_CHILD_p;
    } else {
      rtDW.is_Init5 = IN_NO_ACTIVE_CHILD_p;
    }

    MQt_SendPoint_flg = 0U;
    MQt_NoBag_flg = 0U;
    rtDW.MQt_SendPoint_cou = 0.0;
    rtDW.MQt_SendPoint_time = 0.0;
    MQt_GetPointBack_flg = 0U;
    rtDW.is_Allow_InStation = IN_NO_ACTIVE_CHILD_p;
    break;

   case IN_Init6:
    if (rtDW.is_Init6 == IN_Init_c) {
      MQt_SendPoint_flg = 0U;
      rtDW.MQt_SendPoint_time++;
      rtDW.is_Init6 = IN_NO_ACTIVE_CHILD_p;
    } else {
      rtDW.is_Init6 = IN_NO_ACTIVE_CHILD_p;
    }

    MQt_SendPoint_flg = 0U;
    MCloud_CarAllowIn_err = 0U;
    rtDW.MQt_SendPoint_cou = 0.0;
    rtDW.MQt_SendPoint_time = 0.0;
    MQt_GetPointBack_flg = 0U;
    rtDW.is_Allow_InStation = IN_NO_ACTIVE_CHILD_p;
    break;

   case IN_Init7:
    MCloud_SendGetCarType_flg = 0U;
    rtDW.MCloud_GetCarType_cou++;
    rtDW.is_Allow_InStation = IN_NO_ACTIVE_CHILD_p;
    break;

   case IN_Init8:
    MQt_SendPoint_flg = 0U;
    MQt_NoBag_flg = 0U;
    rtDW.MQt_SendPoint_cou = 0.0;
    rtDW.MQt_SendPoint_time = 0.0;
    MQt_GetPointBack_flg = 0U;
    rtDW.is_Allow_InStation = IN_NO_ACTIVE_CHILD_p;
    break;

   default:
    rtDW.is_Allow_InStation = IN_NO_ACTIVE_CHILD_p;
    break;
  }
}

/* Function for Chart: '<Root>/Chart' */
void Allow_InStation(void)
{
  if ((Sta == 29) || (Sta == 25) || (Sta == 27)) {
    MCE_EndThisTime_flg = 1U;
    MChar_StartCE_flg[MChar_SelfCe_sour] = 0U;

    /* MSql_DelUnfinishedOrder_flg = 1; */
    Grab_GetCarCard_flg = 0U;
    exit_internal_Allow_InStation();
    exit_atomic_Allow_InStation();
    rtDW.is_Main_CE = IN_All_Self_Check;
    Sys_Big_sta = 0U;
    MCE_WriSqlShutDown_sta = 0U;
    MCE_GetCBOk_flg = 0U;

    /* Sta = 0; */
    rtDW.is_All_Self_Check = IN_MIO_COMM;
    rtDW.is_MIO_COMM = IN_Init_c;
    Sta = 1U;
  } else {
    switch (rtDW.is_Allow_InStation) {
     case IN_Init_c:
      if (MCloud_CarAllowIn_sta == 1) {
        MCloud_CarAllowIn_sta = 0U;
        rtDW.MCloud_CarAllowIn_time = 0.0;
        MCloud_SendCarCard_flg = 0U;
        rtDW.MCloud_CarAllowIn_time++;
        rtDW.is_Allow_InStation = IN_Init1;
        Sta = 23U;
        MCE_WriSqlShutDown_sta = 2U;
        MChar_StartCE_flg[MChar_SelfCe_sour] = 1U;
        MLed_PlayIn_flg = 1U;

        /* MSql_WriCeStartData_flg = 1;
           MSql_WriUnfinishedOrder_flg = 1; */
        MCloud_CESta_sta = 1U;

        /* Mnet_WriCeStartData_flg = 1; */
      } else if ((rtDW.MCloud_CarAllowIn_time >= 10.0) || (MCloud_CarAllowIn_sta
                  == 2)) {
        rtDW.MCloud_CarAllowIn_time = 0.0;
        MCloud_CarAllowIn_sta = 0U;
        MCloud_SendCarCard_flg = 0U;
        rtDW.MCloud_CarAllowIn_time++;
        rtDW.is_Allow_InStation = IN_Init6;

        /* Sta = 29; */
        MLed_PlayCloudNoIn_flg = 1U;
        MLed_Start10s_flg = 1U;
        rtDW.is_Init6 = IN_Init_c;
        Sta = 24U;
        MQt_SendPoint_flg = 1U;
        MCloud_CarAllowIn_err = 1U;
      } else if (rtDW.MCloud_CarAllowIn_cou >= 1000.0) {
        rtDW.MCloud_CarAllowIn_cou = 0.0;
        rtDW.MCloud_CarAllowIn_time++;
        rtDW.is_Allow_InStation = IN_Init_c;
        Sta = 21U;
        MCloud_SendCarCard_flg = 1U;
      } else {
        rtDW.MCloud_CarAllowIn_cou++;
      }
      break;

     case IN_Init1:
      rtDW.is_Allow_InStation = IN_NO_ACTIVE_CHILD_p;
      exit_atomic_Allow_InStation();
      rtDW.is_Main_CE = IN_Car_In;
      Sys_Big_sta = 3U;
      MCloud_CancelOrd_ok = 0U;
      MQt_GetCancelOrd_flg = 0U;
      MCE_WriSqlShutDown_sta = 3U;
      rtDW.is_Car_In = IN_Init_c;
      Sta = 30U;
      break;

     case IN_Init3:
      rtDW.is_Allow_InStation = IN_Init7;
      Sta = 21U;
      MCloud_SendGetCarType_flg = 1U;
      break;

     case IN_Init4:
      if (rtDW.CheckBag_time >= 2.0) {
        rtDW.CheckBag_time = 0.0;
        rtDW.CheckBag_time++;
        MCloud_CarAllowIn_sta = 0U;
        rtDW.MCloud_CarAllowIn_time = 0.0;
        rtDW.is_Allow_InStation = IN_Init5;

        /* Sta = 29; */
        MLed_PlayNoBag_flg = 1U;
        MLed_Start10s_flg = 1U;
        rtDW.is_Init5 = IN_Init_c;
        Sta = 28U;
        MQt_SendPoint_flg = 1U;
        MQt_NoBag_flg = 1U;
      } else if (MCE_SelfCheckOk_sta > 1) {
        rtDW.CheckBag_time++;
        MCloud_CarAllowIn_sta = 0U;
        rtDW.MCloud_CarAllowIn_time = 0.0;
        rtDW.is_Allow_InStation = IN_Init4;
        Sta = 20U;
        CheckBatBag_Deal1();
      } else {
        if (MCE_SelfCheckOk_sta == 1) {
          rtDW.CheckBag_time = 0.0;
          MCE_SelfCheckOk_sta = 0U;
          rtDW.CheckBag_time++;
          MCloud_CarAllowIn_sta = 0U;
          rtDW.MCloud_CarAllowIn_time = 0.0;
          rtDW.is_Allow_InStation = IN_Init_c;
          Sta = 21U;
          MCloud_SendCarCard_flg = 1U;
        }
      }
      break;

     case IN_Init5:
      if (rtDW.is_Init5 == IN_Init_c) {
        if (rtDW.MQt_SendPoint_cou >= 10.0) {
          rtDW.MQt_SendPoint_cou = 0.0;
          rtDW.MQt_SendPoint_time++;
          rtDW.is_Init5 = IN_Init_c;
          Sta = 28U;
          MQt_SendPoint_flg = 1U;
          MQt_NoBag_flg = 1U;
        } else if ((rtDW.MQt_SendPoint_time >= 2.0) || (MQt_GetPointBack_flg ==
                    1)) {
          rtDW.MQt_SendPoint_time++;
          rtDW.is_Init5 = IN_Init1;
          Sta = 29U;
          MQt_SendPoint_flg = 0U;
          MQt_NoBag_flg = 0U;
        } else {
          rtDW.MQt_SendPoint_cou++;
        }
      }
      break;

     case IN_Init6:
      if (rtDW.is_Init6 == IN_Init_c) {
        if (rtDW.MQt_SendPoint_cou >= 10.0) {
          rtDW.MQt_SendPoint_cou = 0.0;
          rtDW.MQt_SendPoint_time++;
          rtDW.is_Init6 = IN_Init_c;
          Sta = 24U;
          MQt_SendPoint_flg = 1U;
          MCloud_CarAllowIn_err = 1U;
        } else if ((rtDW.MQt_SendPoint_time >= 2.0) || (MQt_GetPointBack_flg ==
                    1)) {
          rtDW.MQt_SendPoint_time++;
          rtDW.is_Init6 = IN_Init1;
          Sta = 25U;
          MQt_SendPoint_flg = 0U;
          MCloud_CarAllowIn_err = 0U;
        } else {
          rtDW.MQt_SendPoint_cou++;
        }
      }
      break;

     case IN_Init7:
      if ((rtDW.MCloud_GetCarType_cou >= 1000.0) || (MCloud_GetCarType_flg == 1))
      {
        rtDW.MCloud_GetCarType_cou = 0.0;
        MCloud_SendGetCarType_flg = 0U;
        rtDW.MCloud_GetCarType_cou++;
        rtDW.is_Allow_InStation = IN_Init4;
        Sta = 20U;
        CheckBatBag_Deal1();
      } else if (rtDW.MCloud_GetCarType_cou >= 200.0) {
        rtDW.MCloud_GetCarType_cou = 0.0;
        rtDW.MCloud_GetCarType_cou++;
        rtDW.is_Allow_InStation = IN_Init7;
        Sta = 21U;
        MCloud_SendGetCarType_flg = 1U;
      } else if (MCloud_GetCarType_flg == 2) {
        rtDW.MCloud_GetCarType_cou = 0.0;
        MCloud_SendGetCarType_flg = 0U;
        rtDW.MCloud_GetCarType_cou++;
        rtDW.is_Allow_InStation = IN_Init8;

        /* Sta = 29; */
        MLed_PlayCarNoInnet_flg = 1U;
        MLed_Start10s_flg = 1U;
        Sta = 27U;
        MQt_SendPoint_flg = 0U;
        MQt_NoBag_flg = 0U;
      } else {
        /* MCloud_SendGetCarType_flg=1 */
        rtDW.MCloud_GetCarType_cou++;
      }
      break;
    }
  }
}

/* Function for Chart: '<Root>/Chart' */
void CheckCarInCancel_Deal(void)
{
  if ((Sys_Big_sta == 3) && (MQt_GetCancelOrd_flg == 1)) {
    MCloud_SendCancelOrd_flg = 1U;
    rtDW.Get_cou++;
  }

  if ((MCloud_CancelOrd_ok == 1) || (rtDW.Get_cou >= 1000.0)) {
    MCloud_SendCancelOrd_flg = 0U;
    rtDW.Get_cou = 0.0;
    rtDW.CarIn_ToSelfCheck_flg = 1.0;
  }
}

/* Function for Chart: '<Root>/Chart' */
void exit_internal_Car_In(void)
{
  if (rtDW.is_Car_In == IN_Init6_o) {
    if (rtDW.is_Init6_p == IN_Init_c) {
      MQt_SendPoint_flg = 0U;
      rtDW.MQt_SendPoint_time++;
      rtDW.is_Init6_p = IN_NO_ACTIVE_CHILD_p;
    } else {
      rtDW.is_Init6_p = IN_NO_ACTIVE_CHILD_p;
    }

    /* Sta = 29; */
    MQt_SendPoint_flg = 0U;
    MQt_ClearInPlace_flg = 0U;
    rtDW.MQt_SendPoint_cou = 0.0;
    rtDW.MQt_SendPoint_time = 0.0;
    MQt_GetPointBack_flg = 0U;
    rtDW.is_Car_In = IN_NO_ACTIVE_CHILD_p;
  } else {
    rtDW.is_Car_In = IN_NO_ACTIVE_CHILD_p;
  }
}

/* Function for Chart: '<Root>/Chart' */
void Car_In(void)
{
  if (rtDW.CarIn_ToSelfCheck_flg == 1.0) {
    rtDW.CarIn_ToSelfCheck_flg = 0.0;
    MLed_PlayCancelOrder_flg = 1U;
    MChar_StartCE_flg[MChar_SelfCe_sour] = 0U;
    exit_internal_Car_In();
    rtDW.MCE_WaitInPlac_cou = 0.0;
    MQt_SendInPlace_flg = 0U;
    MQt_GetInPlace_flg = 0U;
    MQt_GetCancelOrd_flg = 0U;
    MCloud_SendCancelOrd_flg = 0U;
    MCloud_CancelOrd_ok = 0U;
    rtDW.is_Main_CE = IN_Init3_b;
    Sta = 60U;
    MLed_Start10s_flg = 1U;
    rtDW.MCE_WaitCancOrd_cou = 0.0;
  } else {
    CheckCarInCancel_Deal();
    switch (rtDW.is_Car_In) {
     case IN_Init_c:
      /* [MCE_InPlacSens_flg==0] */
      rtDW.is_Car_In = IN_Init5_b;
      Sta = 31U;
      break;

     case IN_Init1:
      if (MQt_StartSelfCE_sta == 1) {
        MQt_GetHandCBOk_flg = 0U;
        rtDW.is_Car_In = IN_NO_ACTIVE_CHILD_p;
        rtDW.MCE_WaitInPlac_cou = 0.0;
        MQt_SendInPlace_flg = 0U;
        MQt_GetInPlace_flg = 0U;
        MQt_GetCancelOrd_flg = 0U;
        MCloud_SendCancelOrd_flg = 0U;
        MCloud_CancelOrd_ok = 0U;
        rtDW.is_Main_CE = IN_ChangeBatt;
        Sys_Big_sta = 4U;
        MCE_GetCBOk_flg = 0U;
        rtDW.is_ChangeBatt = IN_Init_c;
        Sta = 40U;
        MCE_GetChangeBat_flg = 0U;
        MCloud_CESta_sta = 4U;
        MLed_PlayStartCB_flg = 1U;
      }
      break;

     case IN_Init2_k:
      if (MCE_InPlacSens_flg == 1) {
        /* {MCloud_Comm_err=0} */
        rtDW.is_Car_In = IN_Init1;
        Sta = 34U;

        /* MSql_WriCeStartData_flg = 1;
           Mnet_WriCeStartData_flg = 1;
           MSql_WriUnfinishedOrder_flg = 1; */
      }
      break;

     case IN_Init3_bci:
      if (MCE_InPlacSens_flg != 1) {
        rtDW.is_Car_In = IN_Init3_bci;
        Sta = 32U;
        rtDW.MCE_WaitInPlac_cou = 0.0;
      } else if (rtDW.MCE_WaitInPlac_cou >= 200.0) {
        rtDW.MCE_WaitInPlac_cou = 0.0;
        rtDW.is_Car_In = IN_Init4_m2;
        Sta = 33U;
        MQt_GetInPlace_flg = 0U;
        MQt_SendInPlace_flg = 1U;
        MCloud_CESta_sta = 2U;
        MSql_WriCeStartData_flg = 1U;
        Mnet_WriCeStartData_flg = 1U;
        MSql_WriUnfinishedOrder_flg = 1U;
        MLed_PlayInPlac_flg = 1U;
      } else {
        rtDW.MCE_WaitInPlac_cou++;
      }
      break;

     case IN_Init4_m2:
      if (MQt_GetInPlace_flg == 1) {
        MQt_GetInPlace_flg = 0U;
        rtDW.is_Car_In = IN_Init1;
        Sta = 34U;

        /* MSql_WriCeStartData_flg = 1;
           Mnet_WriCeStartData_flg = 1;
           MSql_WriUnfinishedOrder_flg = 1; */
      } else if (MRemo_GetInPlace_flg == 1) {
        MRemo_GetInPlace_flg = 0U;
        rtDW.is_Car_In = IN_Init6_o;
        rtDW.is_Init6_p = IN_Init_c;
        Sta = 28U;
        MQt_SendPoint_flg = 1U;
        MQt_ClearInPlace_flg = 1U;
      } else {
        if (MCE_InPlacSens_flg != 1) {
          MQt_SendInPlace_flg = 0U;
          rtDW.is_Car_In = IN_Init3_bci;
          Sta = 32U;
          rtDW.MCE_WaitInPlac_cou = 0.0;
        }
      }
      break;

     case IN_Init5_b:
      rtDW.is_Car_In = IN_Init3_bci;
      Sta = 32U;
      rtDW.MCE_WaitInPlac_cou = 0.0;
      break;

     default:
      if (Sta == 29) {
        if (rtDW.is_Init6_p == IN_Init_c) {
          MQt_SendPoint_flg = 0U;
          rtDW.MQt_SendPoint_time++;
          rtDW.is_Init6_p = IN_NO_ACTIVE_CHILD_p;
        } else {
          rtDW.is_Init6_p = IN_NO_ACTIVE_CHILD_p;
        }

        /* Sta = 29; */
        MQt_SendPoint_flg = 0U;
        MQt_ClearInPlace_flg = 0U;
        rtDW.MQt_SendPoint_cou = 0.0;
        rtDW.MQt_SendPoint_time = 0.0;
        MQt_GetPointBack_flg = 0U;
        rtDW.is_Car_In = IN_Init1;
        Sta = 34U;

        /* MSql_WriCeStartData_flg = 1;
           Mnet_WriCeStartData_flg = 1;
           MSql_WriUnfinishedOrder_flg = 1; */
      } else {
        if (rtDW.is_Init6_p == IN_Init_c) {
          if (rtDW.MQt_SendPoint_cou >= 10.0) {
            rtDW.MQt_SendPoint_cou = 0.0;
            rtDW.MQt_SendPoint_time++;
            rtDW.is_Init6_p = IN_Init_c;
            Sta = 28U;
            MQt_SendPoint_flg = 1U;
            MQt_ClearInPlace_flg = 1U;
          } else if ((rtDW.MQt_SendPoint_time >= 2.0) || (MQt_GetPointBack_flg ==
                      1)) {
            rtDW.MQt_SendPoint_time++;
            rtDW.is_Init6_p = IN_Init1;
            Sta = 29U;
            MQt_SendPoint_flg = 0U;
            MQt_ClearInPlace_flg = 0U;
          } else {
            rtDW.MQt_SendPoint_cou++;
          }
        }
      }
      break;
    }
  }
}

/* Function for Chart: '<Root>/Chart' */
void Check_err(void)
{
  if ((MSys_AllErr_Lev >= 3) && ((MCloud_Comm_flg != 2) &&
       (FireContr_GetFire_flg != 1)) && (MCE_RunState != 3) && (MCE_RunState !=
       5)) {
    QT_TimeOutOrder_flg = 1U;
    MCloud_CESta_sta = 7U;
  } else {
    QT_TimeOutOrder_flg = 0U;
  }
}

/* Function for Chart: '<Root>/Chart' */
void CheckCBCancel_Deal(void)
{
  if ((Sys_Big_sta == 4) && (MQt_GetCancelOrd_flg == 1)) {
    MCloud_SendCancelOrd_flg = 1U;
    rtDW.Get_cou++;
  }

  if ((MCloud_CancelOrd_ok == 1) || (rtDW.Get_cou >= 1000.0)) {
    MCloud_SendCancelOrd_flg = 0U;
    rtDW.Get_cou = 0.0;
    rtDW.CarIn_ToSelfCheck_flg = 1.0;
  }
}

/* Function for Chart: '<Root>/Chart' */
void CB_Deal(void)
{
  int32_T tmp;
  if (((MCE_RunState == 7) || (MCE_RunState == 1)) && (MCE_StackerSenser_sta ==
       1) && (MCE_RGVSenser_sta == 1) && (MCE_RGVStepData == 1) &&
      (MCE_StackerStepData == 1)) {
    Send_CE_CHANGEBAT_flg = 1U;
  } else {
    Send_CE_CHANGEBAT_flg = 0U;
    MCE_Order_err = 1U;
  }

  tmp = MChar_SelfCe_sour + 1;
  if (tmp > 255) {
    tmp = 255;
  }

  MCE_CBLocator_sour = (uint8_T)tmp;
  MCE_CBLocator_CarTy = Cloud_carType_Code;
}

/* Function for Chart: '<Root>/Chart' */
void Check_PLC_Die(void)
{
  if ((MCE_GetCBOk_flg != 1) && (PLC_Die_flg != 1)) {
    if ((MCE_RunState == 7) && (MCE_ControlMode == 2) && (MQt_StartSelfCE_sta ==
         1)) {
      rtDW.Check_PLCDie_cou++;
      PLC_Die_flg = (uint8_T)(rtDW.Check_PLCDie_cou >= 30000.0);
    } else {
      rtDW.Check_PLCDie_cou = 0.0;
    }
  } else {
    rtDW.Check_PLCDie_cou = 0.0;
  }
}

/* Function for Chart: '<Root>/Chart' */
void CB_AllClear(void)
{
  MCE_GetChangeBat_flg = 0U;
  Send_CE_CHANGEBAT_flg = 0U;
  MCE_Order_err = 0U;
  rtDW.MCE_ChangeBat_count = 0.0;
  rtDW.MCE_ChangeBat_Times = 0.0;
  MCE_CBLocator_sour = 0U;
  MCE_CBLocator_CarTy = 0U;
  MCE_GetCBOk_flg = 0U;

  /* MCE_TimeOut_CarTy = 0; */
  MQt_GetCancelOrd_flg = 0U;
  MCloud_SendCancelOrd_flg = 0U;
  rtDW.Get_cou = 0.0;
  MCloud_CancelOrd_ok = 0U;
}

/* Function for Chart: '<Root>/Chart' */
void UpData_FIFO(void)
{
  int32_T i;
  int32_T a;
  int32_T exitg1;
  i = 0;

  /* a = MLocat_FIFO_Buf[MChar_SelfCe_sour]; */
  do {
    exitg1 = 0;
    if (MLocat_FIFO_Buf[i] == MChar_SelfCe_sour) {
      a = MLocat_FIFO_Buf[i];
      do {
        MLocat_FIFO_Buf[i] = MLocat_FIFO_Buf[i + 1];
        i++;
      } while (i < 19);

      MLocat_FIFO_Buf[19] = (uint8_T)a;
      exitg1 = 1;
    } else if (i < 19) {
      i++;
    } else {
      exitg1 = 1;
    }
  } while (exitg1 == 0);
}

/* Function for Chart: '<Root>/Chart' */
void ChangeBatt(void)
{
  if (rtDW.CarIn_ToSelfCheck_flg == 1.0) {
    rtDW.CarIn_ToSelfCheck_flg = 0.0;
    MLed_PlayCancelOrder_flg = 1U;
    MChar_StartCE_flg[MChar_SelfCe_sour] = 0U;
    switch (rtDW.is_ChangeBatt) {
     case IN_Init4:
      rtDW.is_Init4 = IN_NO_ACTIVE_CHILD_p;
      PLC_Die_flg = 0U;
      rtDW.is_ChangeBatt = IN_NO_ACTIVE_CHILD_p;
      break;

     case IN_Send_Init_oe:
      /* Send_CE_CHANGEBAT_flg = 0; */
      rtDW.MCE_ChangeBat_count = 0.0;
      rtDW.MCE_ChangeBat_Times++;
      Send_CE_CHANGEBAT_flg = 0U;
      rtDW.is_ChangeBatt = IN_NO_ACTIVE_CHILD_p;
      break;

     default:
      rtDW.is_ChangeBatt = IN_NO_ACTIVE_CHILD_p;
      break;
    }

    CB_AllClear();
    rtDW.is_Main_CE = IN_Init3_b;
    Sta = 60U;
    MLed_Start10s_flg = 1U;
    rtDW.MCE_WaitCancOrd_cou = 0.0;
  } else if ((MQt_GetHandCBOk_flg == 1) && (MQt_StartSelfCE_sta == 1)) {
    MQt_GetHandCBOk_flg = 0U;
    switch (rtDW.is_ChangeBatt) {
     case IN_Init4:
      rtDW.is_Init4 = IN_NO_ACTIVE_CHILD_p;
      PLC_Die_flg = 0U;
      rtDW.is_ChangeBatt = IN_NO_ACTIVE_CHILD_p;
      break;

     case IN_Send_Init_oe:
      /* Send_CE_CHANGEBAT_flg = 0; */
      rtDW.MCE_ChangeBat_count = 0.0;
      rtDW.MCE_ChangeBat_Times++;
      Send_CE_CHANGEBAT_flg = 0U;
      rtDW.is_ChangeBatt = IN_NO_ACTIVE_CHILD_p;
      break;

     default:
      rtDW.is_ChangeBatt = IN_NO_ACTIVE_CHILD_p;
      break;
    }

    CB_AllClear();
    rtDW.is_Main_CE = IN_Cloud_Pass;
    Sys_Big_sta = 5U;
    MCE_WriSqlShutDown_sta = 6U;
    MCloud_CarAllowOut_sta = 0U;
    rtDW.is_Cloud_Pass = IN_Init_c;
    Sta = 50U;
    rtDW.MCloud_CarAllowOut_cou = 0.0;
    MCloud_SendCarOutOf_flg = 1U;
  } else {
    Check_err();
    CheckCBCancel_Deal();
    switch (rtDW.is_ChangeBatt) {
     case IN_Init_c:
      if ((MCE_Order_err == 0) && (MCE_ChangeBatOrder_err == 0)) {
        rtDW.is_ChangeBatt = IN_Send_Init_oe;
        CB_Deal();
        Sta = 41U;
      }
      break;

     case IN_Init2:
      rtDW.is_ChangeBatt = IN_Init_c;
      Sta = 40U;
      MCE_GetChangeBat_flg = 0U;
      MCloud_CESta_sta = 4U;
      MLed_PlayStartCB_flg = 1U;
      break;

     case IN_Init3:
      rtDW.is_ChangeBatt = IN_Init_c;
      Sta = 40U;
      MCE_GetChangeBat_flg = 0U;
      MCloud_CESta_sta = 4U;
      MLed_PlayStartCB_flg = 1U;
      break;

     case IN_Init4:
      if ((MCE_GetCBOk_flg == 1) && (MQt_StartSelfCE_sta == 1)) {
        rtDW.is_Init4 = IN_NO_ACTIVE_CHILD_p;
        PLC_Die_flg = 0U;
        rtDW.is_ChangeBatt = IN_Init5;
        Sta = 45U;
        MCE_WriSqlShutDown_sta = 5U;
        MCE_GetCBOk_flg = 0U;
        MChar_StartCE_flg[MChar_SelfCe_sour] = 0U;
        CHU_GetBatBasic_flg[MChar_SelfCe_sour] = 0U;
      } else {
        /* Sta = 42;
           Send_ChangeBat_ok = 1;
           MLocat_BatCode_null[MChar_SelfCe_sour]=0; */
        Check_PLC_Die();
        switch (rtDW.is_Init4) {
         case IN_dont_mount:
          if (MCE_ParkPlatStepData == 7) {
            rtDW.is_Init4 = IN_mount_ok;
            Sta = 43U;
            MLed_PlayCBing_flg = 1U;
          } else {
            /* MSql_WriCeOkData_flg = 1; */
          }
          break;

         case IN_mount_ok:
          if (MCE_ParkPlatStepData == 14) {
            rtDW.is_Init4 = IN_mount_ok1;
            Sta = 44U;
            MLed_PlayCBing_flg = 1U;
          } else {
            /* MSql_WriCeOkData_flg = 1; */
          }
          break;

         default:
          /* MSql_WriCeOkData_flg = 1; */
          break;
        }
      }
      break;

     case IN_Init5:
      rtDW.is_ChangeBatt = IN_Init8;
      Sta = 47U;

      /* MSql_WriCeOkData_flg = 1; */
      MChar_BatCodeCheck_ok = 1U;
      MCloud_CESta_sta = 5U;

      /* Mnet_WriCeOkData_flg=1; */
      break;

     case IN_Init6:
      if (MChar_BatCodeCheck_ok == 1) {
        MChar_BatCodeCheck_ok = 0U;
        rtDW.is_ChangeBatt = IN_NO_ACTIVE_CHILD_p;
        CB_AllClear();
        rtDW.is_Main_CE = IN_Cloud_Pass;
        Sys_Big_sta = 5U;
        MCE_WriSqlShutDown_sta = 6U;
        MCloud_CarAllowOut_sta = 0U;
        rtDW.is_Cloud_Pass = IN_Init_c;
        Sta = 50U;
        rtDW.MCloud_CarAllowOut_cou = 0.0;
        MCloud_SendCarOutOf_flg = 1U;
      }
      break;

     case IN_Init7:
      rtDW.is_ChangeBatt = IN_Init8;
      Sta = 47U;

      /* MSql_WriCeOkData_flg = 1; */
      MChar_BatCodeCheck_ok = 1U;
      MCloud_CESta_sta = 5U;

      /* Mnet_WriCeOkData_flg=1; */
      break;

     case IN_Init8:
      if (MChar_BatCodeCheck_ok == 1) {
        UpData_FIFO();

        /* {MChar_BatCodeCheck_ok = 0} */
        rtDW.is_ChangeBatt = IN_Init6;
        Sta = 47U;
        MLed_PlayOut_flg = 1U;
      }
      break;

     default:
      if (rtDW.MCE_ChangeBat_count >= 100.0) {
        /* Send_CE_CHANGEBAT_flg = 0; */
        rtDW.MCE_ChangeBat_count = 0.0;
        rtDW.MCE_ChangeBat_Times++;
        Send_CE_CHANGEBAT_flg = 0U;
        rtDW.is_ChangeBatt = IN_Send_Init_oe;
        CB_Deal();
        Sta = 41U;
      } else if (MCE_Order_err == 1) {
        rtDW.MCE_ChangeBat_Times = 0.0;

        /* Send_CE_CHANGEBAT_flg = 0; */
        rtDW.MCE_ChangeBat_count = 0.0;
        rtDW.MCE_ChangeBat_Times++;
        Send_CE_CHANGEBAT_flg = 0U;
        rtDW.is_ChangeBatt = IN_Init2;
        Sta = 49U;
        MCE_Order_err = 1U;
      } else if (rtDW.MCE_ChangeBat_Times >= 5.0) {
        rtDW.MCE_ChangeBat_Times = 0.0;

        /* Send_CE_CHANGEBAT_flg = 0; */
        rtDW.MCE_ChangeBat_count = 0.0;
        rtDW.MCE_ChangeBat_Times++;
        Send_CE_CHANGEBAT_flg = 0U;
        rtDW.is_ChangeBatt = IN_Init3;
        Sta = 49U;
        MCE_ChangeBatOrder_err = 1U;
      } else if (MCE_GetChangeBat_flg == 1) {
        MCE_GetChangeBat_flg = 0U;

        /* Send_CE_CHANGEBAT_flg = 0; */
        rtDW.MCE_ChangeBat_count = 0.0;
        rtDW.MCE_ChangeBat_Times++;
        Send_CE_CHANGEBAT_flg = 0U;
        rtDW.is_ChangeBatt = IN_Init4;
        MCE_WriSqlShutDown_sta = 4U;
        rtDW.Check_PLCDie_cou = 0.0;
        PLC_Die_flg = 0U;
        rtDW.is_Init4 = IN_dont_mount;
        Sta = 42U;
      } else {
        rtDW.MCE_ChangeBat_count++;
      }
      break;
    }
  }
}

/* Function for Chart: '<Root>/Chart' */
void exit_internal_Cloud_Pass(void)
{
  switch (rtDW.is_Cloud_Pass) {
   case IN_Init_c:
    rtDW.MCloud_CarAllowOut_time++;
    rtDW.MCloud_CarAllowOut_cou = 0.0;
    rtDW.is_Cloud_Pass = IN_NO_ACTIVE_CHILD_p;
    break;

   case IN_Init4_m2:
    if (rtDW.is_Init4_n == IN_Init_c) {
      rtDW.MCloud_CarODO_time++;
      rtDW.MCloud_CarODO_cou = 0.0;
      rtDW.is_Init4_n = IN_NO_ACTIVE_CHILD_p;
    } else {
      rtDW.is_Init4_n = IN_NO_ACTIVE_CHILD_p;
    }

    rtDW.is_Cloud_Pass = IN_NO_ACTIVE_CHILD_p;
    break;

   default:
    rtDW.is_Cloud_Pass = IN_NO_ACTIVE_CHILD_p;
    break;
  }
}

/* Function for Chart: '<Root>/Chart' */
void Cloud_Pass(void)
{
  if (MCE_SelfCE_Ok == 1) {
    MCE_SelfCE_Ok = 0U;
    MCE_EndThisTime_flg = 1U;
    MChar_StartCE_flg[MChar_SelfCe_sour] = 0U;

    /* MSql_DelUnfinishedOrder_flg = 1; */
    Grab_GetCarCard_flg = 0U;
    exit_internal_Cloud_Pass();
    rtDW.MCloud_CarAllowOut_cou = 0.0;
    MCloud_SendCarOutOf_flg = 0U;
    rtDW.MCloud_CarAllowOut_time = 0.0;
    MCloud_CarAllowOut_err = 0U;
    rtDW.MCE_WaitWriSqlCloud_cou = 0.0;
    MCloud_SendCarODO_flg = 0U;
    rtDW.MCloud_CarODO_cou = 0.0;
    rtDW.MCloud_CarODO_time = 0.0;
    MCloud_CarAllowOut_sta = 0U;
    rtDW.is_Main_CE = IN_All_Self_Check;
    Sys_Big_sta = 0U;
    MCE_WriSqlShutDown_sta = 0U;
    MCE_GetCBOk_flg = 0U;

    /* Sta = 0; */
    rtDW.is_All_Self_Check = IN_MIO_COMM;
    rtDW.is_MIO_COMM = IN_Init_c;
    Sta = 1U;
  } else {
    switch (rtDW.is_Cloud_Pass) {
     case IN_Init_c:
      if ((MCloud_CarAllowOut_sta == 1) || (Qt_CarAllowOut_sta == 1)) {
        MCloud_CarAllowOut_sta = 0U;
        Qt_CarAllowOut_sta = 0U;
        MSql_DelUnfinishedOrder_flg = 1U;
        rtDW.MCloud_CarAllowOut_time = 0.0;
        rtDW.MCloud_CarAllowOut_time++;
        rtDW.MCloud_CarAllowOut_cou = 0.0;
        rtDW.is_Cloud_Pass = IN_Init1;
        Sta = 52U;

        /* MCE_SelfCE_Ok=1; */
        MSql_WriCeOkData_flg = 1U;
        Mnet_WriCeOkData_flg = 1U;

        /* MSqlite_WriCEData_flg = 1; */
        MCloud_SendCEData_flg = 1U;

        /* MLed_PlayOut_flg = 1; */
        MCloud_CESta_sta = 6U;
        RemotSend_UpCEData_flg = 1U;
        MChar_SelfCe_sour_old = MChar_SelfCe_sour;
      } else if (MCloud_CarAllowOut_sta == 2) {
        rtDW.MCloud_CarAllowOut_time = 0.0;
        rtDW.MCloud_CarAllowOut_time++;
        rtDW.MCloud_CarAllowOut_cou = 0.0;
        rtDW.is_Cloud_Pass = IN_Init2_k;
        Sta = 51U;
        MCloud_CarAllowOut_err = 1U;
        MCloud_CarAllowOut_sta = 0U;
      } else if (rtDW.MCloud_CarAllowOut_cou >= 1000.0) {
        rtDW.MCloud_CarAllowOut_time++;
        rtDW.is_Cloud_Pass = IN_Init_c;
        Sta = 50U;
        rtDW.MCloud_CarAllowOut_cou = 0.0;
        MCloud_SendCarOutOf_flg = 1U;
      } else if (MCloud_CarOutReqODO_flg == 1) {
        rtDW.MCloud_CarAllowOut_time = 0.0;
        MCloud_SendCarOutOf_flg = 0U;
        rtDW.MCloud_CarAllowOut_time++;
        rtDW.MCloud_CarAllowOut_cou = 0.0;
        rtDW.is_Cloud_Pass = IN_Init4_m2;

        /* MCE_SelfCE_Ok=1; */
        rtDW.is_Init4_n = IN_ODO_INIT;
        Sta = 55U;
      } else {
        rtDW.MCloud_CarAllowOut_cou++;
      }
      break;

     case IN_Init1:
      if ((rtDW.MCE_WaitWriSqlCloud_cou > 400.0) ||
          ((chu_test_state[MChar_SelfCe_sour] >= 10) &&
           (CHU_GetBatBasic_flg[MChar_SelfCe_sour] == 1))) {
        rtDW.MCE_WaitWriSqlCloud_cou = 0.0;
        rtDW.is_Cloud_Pass = IN_Init3_bci;
        Sta = 53U;

        /* MLed_PlayOut_flg = 1; */
        MLed_Play_sta = 4U;
        MCE_WriSqlShutDown_sta = 0U;
      } else {
        rtDW.MCE_WaitWriSqlCloud_cou++;
      }
      break;

     case IN_Init2_k:
      /* [MCloud_CarAllowOut_sta==1] */
      MCloud_CarAllowOut_err = 0U;
      rtDW.is_Cloud_Pass = IN_Init1;
      Sta = 52U;

      /* MCE_SelfCE_Ok=1; */
      MSql_WriCeOkData_flg = 1U;
      Mnet_WriCeOkData_flg = 1U;

      /* MSqlite_WriCEData_flg = 1; */
      MCloud_SendCEData_flg = 1U;

      /* MLed_PlayOut_flg = 1; */
      MCloud_CESta_sta = 6U;
      RemotSend_UpCEData_flg = 1U;
      MChar_SelfCe_sour_old = MChar_SelfCe_sour;
      break;

     case IN_Init3_bci:
      if (MCE_InPlacSens_flg == 0) {
        rtDW.is_Cloud_Pass = IN_Init5_b;
        Sta = 54U;
        MCE_SelfCE_Ok = 1U;
      }
      break;

     case IN_Init4_m2:
      if (Sta == 57) {
        if (rtDW.is_Init4_n == IN_Init_c) {
          rtDW.MCloud_CarODO_time++;
          rtDW.MCloud_CarODO_cou = 0.0;
          rtDW.is_Init4_n = IN_NO_ACTIVE_CHILD_p;
        } else {
          rtDW.is_Init4_n = IN_NO_ACTIVE_CHILD_p;
        }

        rtDW.is_Cloud_Pass = IN_Init_c;
        Sta = 50U;
        rtDW.MCloud_CarAllowOut_cou = 0.0;
        MCloud_SendCarOutOf_flg = 1U;
      } else if (MCloud_CarAllowOut_sta == 2) {
        if (rtDW.is_Init4_n == IN_Init_c) {
          rtDW.MCloud_CarODO_time++;
          rtDW.MCloud_CarODO_cou = 0.0;
          rtDW.is_Init4_n = IN_NO_ACTIVE_CHILD_p;
        } else {
          rtDW.is_Init4_n = IN_NO_ACTIVE_CHILD_p;
        }

        rtDW.is_Cloud_Pass = IN_Init2_k;
        Sta = 51U;
        MCloud_CarAllowOut_err = 1U;
        MCloud_CarAllowOut_sta = 0U;
      } else {
        switch (rtDW.is_Init4_n) {
         case IN_Init_c:
          if (rtDW.MCloud_CarODO_cou >= 300.0) {
            rtDW.MCloud_CarODO_time++;
            rtDW.is_Init4_n = IN_Init_c;
            Sta = 56U;
            rtDW.MCloud_CarODO_cou = 0.0;
            MCloud_SendCarODO_flg = 1U;
          } else if (((MCloud_GetCarODO_flg == 1) && (rtDW.MCloud_CarODO_time >=
            2.0)) || (rtDW.MCloud_CarODO_time >= 5.0)) {
            rtDW.MCloud_CarODO_time = 0.0;
            rtDW.MCloud_CarODO_time++;
            rtDW.MCloud_CarODO_cou = 0.0;
            rtDW.is_Init4_n = IN_ODO_INIT1;
            Sta = 57U;
            MCloud_SendCarODO_flg = 0U;
          } else {
            rtDW.MCloud_CarODO_cou++;
          }
          break;

         case IN_ODO_INIT:
          if (MQt_GetODOValue_flg == 1) {
            MQt_GetODOValue_flg = 0U;
            rtDW.is_Init4_n = IN_Init_c;
            Sta = 56U;
            rtDW.MCloud_CarODO_cou = 0.0;
            MCloud_SendCarODO_flg = 1U;
          }
          break;
        }
      }
      break;
    }
  }
}

/* Function for Chart: '<Root>/Chart' */
void Main_CE(void)
{
  boolean_T guard1 = false;
  All_CheckErr();
  CheckBookBag_Deal();
  guard1 = false;
  switch (rtDW.is_Main_CE) {
   case IN_All_Self_Check:
    All_Self_Check();
    break;

   case IN_Allow_InStation:
    Allow_InStation();
    break;

   case IN_Car_In:
    Car_In();
    break;

   case IN_ChangeBatt:
    ChangeBatt();
    break;

   case IN_Cloud_Pass:
    Cloud_Pass();
    break;

   case IN_Init3_b:
    if (rtDW.MCE_WaitCancOrd_cou >= 100.0) {
      rtDW.MCE_WaitCancOrd_cou = 0.0;
      MSql_DelUnfinishedOrder_flg = 1U;
      MCE_EndThisTime_flg = 1U;
      MChar_StartCE_flg[MChar_SelfCe_sour] = 0U;

      /* MSql_DelUnfinishedOrder_flg = 1; */
      Grab_GetCarCard_flg = 0U;
      rtDW.is_Main_CE = IN_All_Self_Check;
      Sys_Big_sta = 0U;
      MCE_WriSqlShutDown_sta = 0U;
      MCE_GetCBOk_flg = 0U;

      /* Sta = 0; */
      rtDW.is_All_Self_Check = IN_MIO_COMM;
      rtDW.is_MIO_COMM = IN_Init_c;
      Sta = 1U;
    } else {
      rtDW.MCE_WaitCancOrd_cou++;
    }
    break;

   case IN_Main_Init:
    if ((MQt_GetSql_flg == 1) && (MQt_StartSelfCE_sta == 1)) {
      /* {MQt_GetSql_flg=0} */
      if ((MCE_GetSqlInt_sta <= 1) || (MCE_GetSqlInt_sta > 6)) {
        guard1 = true;
      } else if (MCE_GetSqlInt_sta == 2) {
        MChar_StartCE_flg[MChar_SelfCe_sour] = 1U;
        rtDW.is_Main_Init = IN_NO_ACTIVE_CHILD_p;
        rtDW.is_Main_CE = IN_Allow_InStation;
        Sys_Big_sta = 2U;
        MCloud_CarAllowIn_sta = 0U;
        MCloud_GetCarType_flg = 0U;
        rtDW.is_Allow_InStation = IN_Init1;
        Sta = 23U;
        MCE_WriSqlShutDown_sta = 2U;
        MChar_StartCE_flg[MChar_SelfCe_sour] = 1U;
        MLed_PlayIn_flg = 1U;

        /* MSql_WriCeStartData_flg = 1;
           MSql_WriUnfinishedOrder_flg = 1; */
        MCloud_CESta_sta = 1U;

        /* Mnet_WriCeStartData_flg = 1; */
      } else if (MCE_GetSqlInt_sta == 3) {
        MChar_StartCE_flg[MChar_SelfCe_sour] = 1U;
        rtDW.is_Main_Init = IN_NO_ACTIVE_CHILD_p;
        rtDW.is_Main_CE = IN_Car_In;
        Sys_Big_sta = 3U;
        MCloud_CancelOrd_ok = 0U;
        MQt_GetCancelOrd_flg = 0U;
        MCE_WriSqlShutDown_sta = 3U;
        rtDW.is_Car_In = IN_Init_c;
        Sta = 30U;
      } else if (MCE_GetSqlInt_sta == 4) {
        MChar_StartCE_flg[MChar_SelfCe_sour] = 1U;
        rtDW.is_Main_Init = IN_NO_ACTIVE_CHILD_p;
        rtDW.is_Main_CE = IN_ChangeBatt;
        Sys_Big_sta = 4U;
        MCE_GetCBOk_flg = 0U;
        MCE_GetChangeBat_flg = 0U;
        rtDW.is_ChangeBatt = IN_Init4;
        MCE_WriSqlShutDown_sta = 4U;
        rtDW.Check_PLCDie_cou = 0.0;
        PLC_Die_flg = 0U;
        rtDW.is_Init4 = IN_dont_mount;
        Sta = 42U;
      } else if (MCE_GetSqlInt_sta == 5) {
        rtDW.is_Main_Init = IN_NO_ACTIVE_CHILD_p;
        rtDW.is_Main_CE = IN_ChangeBatt;
        Sys_Big_sta = 4U;
        MCE_GetChangeBat_flg = 0U;
        rtDW.is_ChangeBatt = IN_Init5;
        Sta = 45U;
        MCE_WriSqlShutDown_sta = 5U;
        MCE_GetCBOk_flg = 0U;
        MChar_StartCE_flg[MChar_SelfCe_sour] = 0U;
        CHU_GetBatBasic_flg[MChar_SelfCe_sour] = 0U;
      } else if (MCE_GetSqlInt_sta == 6) {
        rtDW.is_Main_Init = IN_NO_ACTIVE_CHILD_p;
        rtDW.is_Main_CE = IN_Cloud_Pass;
        Sys_Big_sta = 5U;
        MCE_WriSqlShutDown_sta = 6U;
        MCloud_CarAllowOut_sta = 0U;
        rtDW.is_Cloud_Pass = IN_Init_c;
        Sta = 50U;
        rtDW.MCloud_CarAllowOut_cou = 0.0;
        MCloud_SendCarOutOf_flg = 1U;
      } else {
        MCE_EndThisTime_flg = 1U;
        MChar_StartCE_flg[MChar_SelfCe_sour] = 0U;

        /* MSql_DelUnfinishedOrder_flg = 1; */
        Grab_GetCarCard_flg = 0U;
        guard1 = true;
      }
    } else {
      if ((rtDW.is_Main_Init == IN_Init3_bc) && (MQt_StartSelfCE_sta == 1)) {
        rtDW.is_Main_Init = IN_Init4_m;
        Sta = 0U;
      }
    }
    break;
//2021  5
   default:
    if ((MQt_StartSelfCE_sta != 1) || (MSys_AllErr_Lev >= 3)) {
      rtDW.is_Main_CE = IN_All_Self_Check;
      Sys_Big_sta = 0U;
      MCE_WriSqlShutDown_sta = 0U;
      MCE_GetCBOk_flg = 0U;

      /* Sta = 0; */
      rtDW.is_All_Self_Check = IN_MIO_COMM;
      rtDW.is_MIO_COMM = IN_Init_c;
      Sta = 1U;
    } else {
      if (Grab_GetCarCard_flg == 1) {
        Grab_GetCarCard_flg = 0U;
        MLed_Play_sta = 3U;
        rtDW.is_Main_CE = IN_Allow_InStation;
        Sys_Big_sta = 2U;
        MCloud_CarAllowIn_sta = 0U;
        MCloud_GetCarType_flg = 0U;
        rtDW.is_Allow_InStation = IN_Init3;
        Sta = 20U;
        MCE_SelfCheckOk_sta = 0U;
      }
    }
    break;
  }

  if (guard1) {
    rtDW.is_Main_Init = IN_NO_ACTIVE_CHILD_p;
    rtDW.is_Main_CE = IN_All_Self_Check;
    Sys_Big_sta = 0U;
    MCE_WriSqlShutDown_sta = 0U;
    MCE_GetCBOk_flg = 0U;

    /* Sta = 0; */
    rtDW.is_All_Self_Check = IN_MIO_COMM;
    rtDW.is_MIO_COMM = IN_Init_c;
    Sta = 1U;
  }
}

/* Function for Chart: '<Root>/Chart' */
void MIO_Door(void)
{
  int32_T i;
  int32_T exitg1;
  i = 0;
  do {
    exitg1 = 0;
    if (MIO_DoorValue_null[i] == 0) {
      rtDW.MIO_DoorValue_cou[i]++;
      MIO_DoorErr_buf[i] = (uint8_T)(rtDW.MIO_DoorValue_cou[i] > 300.0);
    } else {
      rtDW.MIO_DoorValue_cou[i] = 0.0;
      MIO_DoorErr_buf[i] = 0U;
    }

    if (i >= 2) {
      exitg1 = 1;
    } else {
      i++;
    }
  } while (exitg1 == 0);

  MIO_Door_err = (uint8_T)((MIO_DoorErr_buf[0] + MIO_DoorErr_buf[1]) +
    MIO_DoorErr_buf[2] >= 1);
}

/* Function for Chart: '<Root>/Chart' */
void MIO_Water(void)
{
  if (MIO_WaterValue_null == 0) {
    rtDW.MIO_WaterValue_cou++;
  } else {
    rtDW.MIO_WaterValue_cou = 0.0;
  }

  MIO_Water_err = (uint8_T)(rtDW.MIO_WaterValue_cou > 300.0);
}

/* Function for Chart: '<Root>/Chart' */
void MIO_Light(void)
{
  int32_T i;
  int32_T exitg1;
  i = 0;
  do {
    exitg1 = 0;
    if ((MIO_LightValue_null[i] == 0) && (Sys_Big_sta == 4) && (Sta > 41)) {
      rtDW.MIO_LightValue_cou[i]++;
    } else {
      rtDW.MIO_LightValue_cou[i] = 0.0;
    }

    if (rtDW.MIO_LightValue_cou[i] > 5.0) {
      MIO_LightrErr_buf[i] = 1U;
      rtDW.MIO_LightValue_cou[i] = 10.0;
    } else {
      MIO_LightrErr_buf[i] = 0U;
    }

    if (i >= 1) {
      exitg1 = 1;
    } else {
      i = 1;
    }
  } while (exitg1 == 0);

  MIO_Light_err = (uint8_T)(MIO_LightrErr_buf[0] + MIO_LightrErr_buf[1] >= 1);
}

/* Function for Chart: '<Root>/Chart' */
void MIO_Thunder(void)
{
  int32_T i;
  int32_T exitg1;
  i = 0;
  do {
    exitg1 = 0;
    if (MIO_ThunderValue_null[i] == 0) {
      rtDW.MIO_ThunderValue_cou[i]++;
    } else {
      rtDW.MIO_ThunderValue_cou[i] = 0.0;
    }

    MIO_ThunderErr_buf[i] = (uint8_T)(rtDW.MIO_ThunderValue_cou[i] > 300.0);
    if (i >= 1) {
      exitg1 = 1;
    } else {
      i = 1;
    }
  } while (exitg1 == 0);

  MIO_Thunder_err = (uint8_T)(MIO_ThunderErr_buf[0] + MIO_ThunderErr_buf[1] >= 1);
}

/* Function for Chart: '<Root>/Chart' */
void QtSend_CharDa_Deal(void)
{
  int32_T i;
  int32_T exitg1;
  i = 0;
  do {
    exitg1 = 0;
    if (EMS_Chger_BigSta[i] == 3) {
      MQt_SendChgData1_flg = 1U;
      exitg1 = 1;
    } else if (i < 10) {
      MQt_SendChgData1_flg = 0U;
      i++;
    } else {
      MQt_SendChgData1_flg = 0U;
      exitg1 = 1;
    }
  } while (exitg1 == 0);

  /* MCE_InLocator_dest = 1; */
  i = 10;
  do {
    exitg1 = 0;
    if (EMS_Chger_BigSta[i] == 3) {
      MQt_SendChgData2_flg = 1U;
      exitg1 = 1;
    } else if (i < 19) {
      MQt_SendChgData2_flg = 0U;
      i++;
    } else {
      MQt_SendChgData2_flg = 0U;
      exitg1 = 1;
    }
  } while (exitg1 == 0);
}

/* Function for Chart: '<Root>/Chart' */
void QtSend_CharErr_Deal(void)
{
  /* [EMS_Chger_BigSta[i]==3] */
  MQt_SendChgerErr_flg = 1U;

  /* MCE_InLocator_dest = 1; */
}

/* Function for Chart: '<Root>/Chart' */
void QtSend_CEDat_Deal(void)
{
  /* [EMS_Chger_BigSta[i]==3] */
  MQt_SendCEData_flg = 1U;

  /* MCE_InLocator_dest = 1; */
}

/* Function for Chart: '<Root>/Chart' */
void QtSend_LocatData_Deal(void)
{
  /* [EMS_Chger_BigSta[i]==3] */
  MQt_SendLocatData_flg = 1U;

  /* MCE_InLocator_dest = 1; */
}

/* Function for Chart: '<Root>/Chart' */
void QtSend_AirData_Deal(void)
{
  /* [EMS_Chger_BigSta[i]==3] */
  MQt_SendAirData_flg = 1U;

  /* MCE_InLocator_dest = 1; */
}

/* Function for Chart: '<Root>/Chart' */
void QtSend_DcEne_Deal(void)
{
  /* [EMS_Chger_BigSta[i]==3] */
  MQt_SendDcEne_flg = 1U;

  /* MCE_InLocator_dest = 1; */
}

/* Function for Chart: '<Root>/Chart' */
void QtSend_DcEne2_Deal(void)
{
  /* [EMS_Chger_BigSta[i]==3] */
  MQt_SendDcEne2_flg = 1U;

  /* MCE_InLocator_dest = 1; */
}

/* Function for Chart: '<Root>/Chart' */
void RemoSend_CharDa_Deal(void)
{
  int32_T tmp;
  int32_T exitg1;
  do {
    exitg1 = 0;
    if ((CHU_AllErr_flg[index_Chu] == 0) && (index_Chu <= 19)) {
      RemotSend_UpCHUData_flg = 1U;
      exitg1 = 1;
    } else {
      tmp = index_Chu + 1;
      if (tmp > 255) {
        tmp = 255;
      }

      index_Chu = (uint8_T)tmp;
      if (index_Chu < 20) {
      } else {
        index_Chu = 0U;
        RemotSend_UpCHUData_flg = 0U;
        exitg1 = 1;
      }
    }
  } while (exitg1 == 0);

  /* MCE_InLocator_dest = 1; */
}

/* Function for Chart: '<Root>/Chart' */
void RemoSend_CharErr_Deal(void)
{
  int32_T tmp;
  int32_T exitg1;
  do {
    exitg1 = 0;
    if ((CHU_AllErr_flg[index_Err] == 1) && (index_Err <= 19)) {
      RemotSend_UpErrCHUData_flg = 1U;
      exitg1 = 1;
    } else {
      tmp = index_Err + 1;
      if (tmp > 255) {
        tmp = 255;
      }

      index_Err = (uint8_T)tmp;
      if (index_Err < 20) {
      } else {
        index_Err = 0U;
        RemotSend_UpErrCHUData_flg = 0U;
        exitg1 = 1;
      }
    }
  } while (exitg1 == 0);

  /* MCE_InLocator_dest = 1; */
}

/* Function for Chart: '<Root>/Chart' */
void RemotSend_UpCEing_Deal(void)
{
  RemotSend_UpCEingData_flg = (uint8_T)(Sys_Big_sta >= 3);

  /* MCE_InLocator_dest = 1; */
}

/* Function for Chart: '<Root>/Chart' */
void LedSta_Deal(void)
{
  switch (Sys_Big_sta) {
   case 0:
    MLed_Play_sta = 1U;
    break;

   case 1:
    MLed_Play_sta = 2U;
    break;

   default:
    if ((Sys_Big_sta >= 2) && (MSys_AllErr_Lev >= 3) && (Sta != 53)) {
      MLed_Play_sta = 5U;
    } else {
      if ((Sys_Big_sta >= 2) && (MSys_AllErr_Lev < 3) && (Sta != 53)) {
        MLed_Play_sta = 3U;
      }
    }
    break;
  }
}

/* Function for Chart: '<Root>/Chart' */
void Play_InitErr_Deal(void)
{
  if (rtDW.InitErr_cou >= 2500.0) {
    MLed_PlayInitErr_flg = 1U;
    rtDW.InitErr_cou = 0.0;
  } else {
    rtDW.InitErr_cou++;
  }

  /* Sound_Start_flg = 1;
     Light_Start_flg = 1;
     MSys_AllErr_Lev = 3; */
}

/* Function for Chart: '<Root>/Chart' */
void Play_Free_Deal(void)
{
  if (rtDW.Free_cou >= 2500.0) {
    MLed_ShowFree_flg = 1U;
    rtDW.Free_cou = 0.0;
  } else {
    rtDW.Free_cou++;
  }

  /* Sound_Start_flg = 1;
     Light_Start_flg = 1;
     MSys_AllErr_Lev = 3; */
}

/* Function for Chart: '<Root>/Chart' */
void Play_FireErr_Deal(void)
{
  if (rtDW.FireErr_cou >= 900.0) {
    rtDW.FireErr_cou = 0.0;
    if ((MSys_AllErr_Lev == 4) && (Sys_Big_sta == 4)) {
      MLed_FireErrCB_flg = 1U;
    } else {
      if ((MSys_AllErr_Lev == 4) && ((Sys_Big_sta == 3) || (Sys_Big_sta == 5)))
      {
        MLed_FireErrGo_flg = 1U;
      }
    }
  } else {
    rtDW.FireErr_cou++;
  }

  /* Sound_Start_flg = 1;
     Light_Start_flg = 1;
     MSys_AllErr_Lev = 3; */
}

/* Function for Chart: '<Root>/Chart' */
void Play_CBingErr_Deal(void)
{
  if ((MSys_AllErr_Lev == 3) && (Sys_Big_sta == 4)) {
    MLed_PlayCBErr_flg = 1U;
  } else {
    if ((MSys_AllErr_Lev == 3) && ((Sys_Big_sta == 3) || (Sys_Big_sta == 5))) {
      MLed_PlayErrGo_flg = 1U;
    }
  }

  /* Sound_Start_flg = 1;
     Light_Start_flg = 1;
     MSys_AllErr_Lev = 3; */
}

/* Function for Chart: '<Root>/Chart' */
void Play_PleaseGo_Deal(void)
{
  if (rtDW.PleaseGo_cou >= 900.0) {
    MLed_PleaseGo_flg = 1U;
    rtDW.PleaseGo_cou = 0.0;
  } else {
    rtDW.PleaseGo_cou++;
  }

  /* Sound_Start_flg = 1;
     Light_Start_flg = 1;
     MSys_AllErr_Lev = 3; */
}

/* Function for Chart: '<Root>/Chart' */
void Led_AllPlay(void)
{
  LedSta_Deal();
  switch (rtDW.is_Led_AllPlay) {
   case IN_Play_Sta1:
    if (MLed_Play_sta >= 2) {
      MLed_PlayInitErr_flg = 0U;
      rtDW.InitErr_cou = 0.0;
      rtDW.is_Led_AllPlay = IN_Play_Sta2;
      rtDW.is_Play_Sta2 = IN_Init3_bci2;
      MLed_Big_sta = 2U;
    } else {
      Play_InitErr_Deal();
    }
    break;

   case IN_Play_Sta2:
    if (MLed_Play_sta == 1) {
      rtDW.is_Play_Sta2 = IN_NO_ACTIVE_CHILD_p;
      rtDW.Free_cou = 0.0;
      MLed_ShowFree_flg = 0U;
      rtDW.is_Led_AllPlay = IN_Play_Sta1;
      MLed_Big_sta = 1U;
      rtDW.InitErr_cou = 2400.0;
    } else if (MLed_Play_sta >= 3) {
      rtDW.is_Play_Sta2 = IN_NO_ACTIVE_CHILD_p;
      rtDW.Free_cou = 0.0;
      MLed_ShowFree_flg = 0U;
      rtDW.is_Led_AllPlay = IN_Play_Sta3;
      rtDW.is_Play_Sta3 = IN_Play_Sta4_a;
      rtDW.is_Play_Sta4_i = IN_Init3_bci2;
      MLed_Big_sta = 11U;
    } else {
      switch (rtDW.is_Play_Sta2) {
       case IN_Init_c:
        /* MLed_Start10s_Num1 = 0;
           QtSend_CharDa_Deal();
           MLed_Start10s_Num1++;
           MCE_Comm_flg=0; */
        break;

       case IN_Init3_bci2:
        if (MLed_Play_sta == 2) {
          rtDW.is_Play_Sta2 = IN_Init4_m2t;
          MLed_Big_sta = 4U;
          rtDW.MLed_Start10s_Num1 = 2000.0;
          rtDW.Free_cou = 5000.0;
        }
        break;

       default:
        Play_Free_Deal();
        break;
      }
    }
    break;

   case IN_Play_Sta3:
    if ((MLed_Play_sta <= 2) && (MLed_Start10s_flg == 1)) {
      switch (rtDW.is_Play_Sta3) {
       case IN_Play_Sta4_a:
        rtDW.is_Play_Sta4_i = IN_NO_ACTIVE_CHILD_p;
        rtDW.PleaseGo_cou = 0.0;
        MLed_PleaseGo_flg = 0U;
        rtDW.is_Play_Sta3 = IN_NO_ACTIVE_CHILD_p;
        break;

       case IN_Play_Sta5:
        rtDW.is_Play_Sta5 = IN_NO_ACTIVE_CHILD_p;
        rtDW.PleaseGo_cou = 0.0;
        MLed_PleaseGo_flg = 0U;
        rtDW.is_Play_Sta3 = IN_NO_ACTIVE_CHILD_p;
        break;
      }

      rtDW.is_Led_AllPlay = IN_Play_Sta4;
      rtDW.is_Play_Sta4 = IN_Init3_bci2;
      MLed_Big_sta = 22U;
    } else if ((MLed_Play_sta <= 2) && (MLed_Start10s_flg != 1)) {
      switch (rtDW.is_Play_Sta3) {
       case IN_Play_Sta4_a:
        rtDW.is_Play_Sta4_i = IN_NO_ACTIVE_CHILD_p;
        rtDW.PleaseGo_cou = 0.0;
        MLed_PleaseGo_flg = 0U;
        rtDW.is_Play_Sta3 = IN_NO_ACTIVE_CHILD_p;
        break;

       case IN_Play_Sta5:
        rtDW.is_Play_Sta5 = IN_NO_ACTIVE_CHILD_p;
        rtDW.PleaseGo_cou = 0.0;
        MLed_PleaseGo_flg = 0U;
        rtDW.is_Play_Sta3 = IN_NO_ACTIVE_CHILD_p;
        break;
      }

      rtDW.is_Led_AllPlay = IN_Play_Sta1;
      MLed_Big_sta = 1U;
      rtDW.InitErr_cou = 2400.0;
    } else if (rtDW.is_Play_Sta3 == IN_Play_Sta4_a) {
      if (MLed_Play_sta == 5) {
        rtDW.is_Play_Sta4_i = IN_NO_ACTIVE_CHILD_p;
        rtDW.PleaseGo_cou = 0.0;
        MLed_PleaseGo_flg = 0U;
        rtDW.is_Play_Sta3 = IN_Play_Sta5;
        rtDW.is_Play_Sta5 = IN_Init3_bci2;
        MLed_Big_sta = 15U;
      } else {
        switch (rtDW.is_Play_Sta4_i) {
         case IN_Init_c:
          if (rtDW.MLed_Start10s_Num2 >= 1000.0) {
            rtDW.is_Play_Sta4_i = IN_Init4_m2t;
            MLed_Big_sta = 13U;
            rtDW.PleaseGo_cou = 2000.0;
            rtDW.MLed_Start10s_Num2 = 2000.0;
          } else {
            /* QtSend_CharDa_Deal(); */
            rtDW.MLed_Start10s_Num2++;

            /* MCE_Comm_flg=0; */
          }
          break;

         case IN_Init3_bci2:
          if (MLed_Play_sta == 4) {
            rtDW.is_Play_Sta4_i = IN_Init_c;
            MLed_Big_sta = 12U;
            rtDW.MLed_Start10s_Num2 = 0.0;
          }
          break;

         default:
          if (MLed_Play_sta != 4) {
            rtDW.is_Play_Sta4_i = IN_Init3_bci2;
            MLed_Big_sta = 11U;
          } else {
            Play_PleaseGo_Deal();
          }
          break;
        }
      }
    } else if (MLed_Play_sta == 4) {
      rtDW.is_Play_Sta5 = IN_NO_ACTIVE_CHILD_p;
      rtDW.PleaseGo_cou = 0.0;
      MLed_PleaseGo_flg = 0U;
      rtDW.is_Play_Sta3 = IN_Play_Sta4_a;
      rtDW.is_Play_Sta4_i = IN_Init3_bci2;
      MLed_Big_sta = 11U;
    } else {
      switch (rtDW.is_Play_Sta5) {
       case IN_Init_c:
        if (rtDW.MLed_Start10s_Num3 >= 100.0) {
          rtDW.is_Play_Sta5 = IN_Init4_m2t;
          MLed_Big_sta = 18U;
          rtDW.MLed_Start10s_Num3 = 2000.0;
          Play_CBingErr_Deal();
        } else {
          /* QtSend_CharDa_Deal(); */
          rtDW.MLed_Start10s_Num3++;

          /* MCE_Comm_flg=0; */
        }
        break;

       case IN_Init3_bci2:
        if (MLed_Play_sta == 5) {
          rtDW.is_Play_Sta5 = IN_Init5_br;
          MLed_Big_sta = 16U;
        }
        break;

       case IN_Init4_m2t:
        if ((MLed_Play_sta != 5) || (MSys_AllErr_Lev == 4)) {
          rtDW.is_Play_Sta5 = IN_Init3_bci2;
          MLed_Big_sta = 15U;
        }
        break;

       case IN_Init5_br:
        switch (MSys_AllErr_Lev) {
         case 4:
          rtDW.is_Play_Sta5 = IN_Init6_oh;
          MLed_Big_sta = 19U;
          rtDW.MLed_Start10s_Num4 = 0.0;
          break;

         case 3:
          rtDW.is_Play_Sta5 = IN_Init_c;
          MLed_Big_sta = 17U;
          rtDW.MLed_Start10s_Num3 = 0.0;
          break;
        }
        break;

       case IN_Init6_oh:
        if (rtDW.MLed_Start10s_Num4 >= 100.0) {
          rtDW.is_Play_Sta5 = IN_Init7_a;
          MLed_Big_sta = 20U;
          MLed_Play_sta = 6U;
          rtDW.FireErr_cou = 900.0;
          rtDW.MLed_Start10s_Num4 = 2000.0;
        } else {
          /* QtSend_CharDa_Deal(); */
          rtDW.MLed_Start10s_Num4++;

          /* MCE_Comm_flg=0; */
        }
        break;

       default:
        Play_FireErr_Deal();
        break;
      }
    }
    break;

   default:
    if (MLed_Big_sta == 24) {
      rtDW.is_Play_Sta4 = IN_NO_ACTIVE_CHILD_p;
      rtDW.is_Led_AllPlay = IN_Play_Sta1;
      MLed_Big_sta = 1U;
      rtDW.InitErr_cou = 2400.0;
    } else {
      switch (rtDW.is_Play_Sta4) {
       case IN_Init_c:
        if (rtDW.MLed_Start10s_Num1 >= 200.0) {
          rtDW.is_Play_Sta4 = IN_Init4_m2t;
          MLed_Big_sta = 24U;
          rtDW.MLed_Start10s_Num1 = 2000.0;
        } else {
          /* QtSend_CharDa_Deal(); */
          rtDW.MLed_Start10s_Num1++;

          /* MCE_Comm_flg=0; */
        }
        break;

       case IN_Init3_bci2:
        if (MLed_Start10s_flg == 1) {
          MLed_Start10s_flg = 0U;
          rtDW.is_Play_Sta4 = IN_Init_c;
          MLed_Big_sta = 23U;
          rtDW.MLed_Start10s_Num1 = 0.0;
        }
        break;
      }
    }
    break;
  }
}

/* Function for Chart: '<Root>/Chart' */
void MAIN(void)
{
  int32_T tmp;
  Hand_CE();
  Main_CE();
  if (rtDW.is_MIO_Door_Wat_Lig_Thu == IN_Init_c) {
    if (MIO_Comm_flg != 1) {
      rtDW.is_MIO_Door_Wat_Lig_Thu = IN_Init3_bci2;

      /* Sta = 2; */
    } else {
      /* MIO_Door(); */
      MIO_Door();
      MIO_Water();
      MIO_Light();
      MIO_Thunder();
    }
  } else {
    if (MIO_Comm_flg == 1) {
      rtDW.is_MIO_Door_Wat_Lig_Thu = IN_Init_c;
    }
  }

  if (rtDW.is_Qt_SendChgDat == IN_Init_c) {
    if (rtDW.QtSend_CharDa_cou >= 300.0) {
      rtDW.is_Qt_SendChgDat = IN_Init_c;
      rtDW.QtSend_CharDa_cou = 0.0;
      QtSend_CharDa_Deal();
    } else {
      rtDW.QtSend_CharDa_cou++;

      /* MCE_Comm_flg=0; */
    }
  } else {
    if (MSqlite_WrtErr_flg == 0) {
      rtDW.is_Qt_SendChgDat = IN_Init_c;
      rtDW.QtSend_CharDa_cou = 0.0;
      QtSend_CharDa_Deal();
    }
  }

  if (rtDW.is_Qt_SendChgErr == IN_Init_c) {
    if (rtDW.QtSend_CharErr_cou >= 100.0) {
      rtDW.is_Qt_SendChgErr = IN_Init_c;
      rtDW.QtSend_CharErr_cou = 0.0;
      QtSend_CharErr_Deal();
    } else {
      rtDW.QtSend_CharErr_cou++;

      /* MCE_Comm_flg=0; */
    }
  } else {
    if (MSqlite_WrtErr_flg == 0) {
      rtDW.is_Qt_SendChgErr = IN_Init_c;
      rtDW.QtSend_CharErr_cou = 0.0;
      QtSend_CharErr_Deal();
    }
  }

  if (rtDW.is_Qt_SendCEDat == IN_Init_c) {
    if (rtDW.QtSend_CEDat_cou >= 110.0) {
      rtDW.is_Qt_SendCEDat = IN_Init_c;
      rtDW.QtSend_CEDat_cou = 0.0;
      QtSend_CEDat_Deal();
    } else {
      rtDW.QtSend_CEDat_cou++;

      /* MCE_Comm_flg=0; */
    }
  } else {
    if (MSqlite_WrtErr_flg == 0) {
      rtDW.is_Qt_SendCEDat = IN_Init_c;
      rtDW.QtSend_CEDat_cou = 0.0;
      QtSend_CEDat_Deal();
    }
  }

  if (rtDW.is_Qt_SendChgErr1 == IN_Init_c) {
    if (rtDW.QtSend_LocatData_cou >= 100.0) {
      rtDW.is_Qt_SendChgErr1 = IN_Init_c;
      rtDW.QtSend_LocatData_cou = 0.0;
      QtSend_LocatData_Deal();
    } else {
      rtDW.QtSend_LocatData_cou++;

      /* MCE_Comm_flg=0; */
    }
  } else {
    if (MSqlite_WrtErr_flg == 0) {
      rtDW.is_Qt_SendChgErr1 = IN_Init_c;
      rtDW.QtSend_LocatData_cou = 0.0;
      QtSend_LocatData_Deal();
    }
  }

  if (rtDW.is_Qt_SendAirData == IN_Init_c) {
    if (rtDW.QtSend_AirData_cou >= 120.0) {
      rtDW.is_Qt_SendAirData = IN_Init_c;
      rtDW.QtSend_AirData_cou = 0.0;
      QtSend_AirData_Deal();
    } else {
      rtDW.QtSend_AirData_cou++;

      /* MCE_Comm_flg=0; */
    }
  } else {
    if (MSqlite_WrtErr_flg == 0) {
      rtDW.is_Qt_SendAirData = IN_Init_c;
      rtDW.QtSend_AirData_cou = 0.0;
      QtSend_AirData_Deal();
    }
  }

  if (rtDW.is_Qt_SendDcEne == IN_Init_c) {
    if (rtDW.QtSend_DcEne_cou >= 297.0) {
      rtDW.is_Qt_SendDcEne = IN_Init_c;
      rtDW.QtSend_DcEne_cou = 0.0;
      QtSend_DcEne_Deal();
    } else {
      rtDW.QtSend_DcEne_cou++;

      /* MCE_Comm_flg=0; */
    }
  } else {
    if (MSqlite_WrtErr_flg == 0) {
      rtDW.is_Qt_SendDcEne = IN_Init_c;
      rtDW.QtSend_DcEne_cou = 0.0;
      QtSend_DcEne_Deal();
    }
  }

  if (rtDW.is_Qt_SendDcEne2 == IN_Init_c) {
    if (rtDW.QtSend_DcEne2_cou >= 311.0) {
      rtDW.is_Qt_SendDcEne2 = IN_Init_c;
      rtDW.QtSend_DcEne2_cou = 0.0;
      QtSend_DcEne2_Deal();
    } else {
      rtDW.QtSend_DcEne2_cou++;

      /* MCE_Comm_flg=0; */
    }
  } else {
    if (MSqlite_WrtErr_flg == 0) {
      rtDW.is_Qt_SendDcEne2 = IN_Init_c;
      rtDW.QtSend_DcEne2_cou = 0.0;
      QtSend_DcEne2_Deal();
    }
  }

  if (rtDW.is_Remo_SendChgDat == IN_Init_c) {
    if (rtDW.RemoSend_CharDa_cou >= 100.0) {
      rtDW.RemoSend_CharDa_cou = 0.0;
      tmp = index_Chu + 1;
      if (tmp > 255) {
        tmp = 255;
      }

      index_Chu = (uint8_T)tmp;
      rtDW.is_Remo_SendChgDat = IN_Init_c;
      RemoSend_CharDa_Deal();
    } else {
      rtDW.RemoSend_CharDa_cou++;

      /* RemoSend_CharDa_Deal(); */
    }
  } else {
    if (MSqlite_WrtErr_flg == 0) {
      rtDW.is_Remo_SendChgDat = IN_Init_c;
      RemoSend_CharDa_Deal();
    }
  }

  if (rtDW.is_RemoSend_UpPLC == IN_Init_c) {
    if (rtDW.RemotSend_UpPLC_cou >= 1000.0) {
      rtDW.RemotSend_UpPLC_cou = 0.0;
      rtDW.is_RemoSend_UpPLC = IN_Init_c;
      RemotSend_UpPLCData_flg = 1U;
    } else {
      rtDW.RemotSend_UpPLC_cou++;

      /* MCE_Comm_flg=0; */
    }
  } else {
    if (MSqlite_WrtErr_flg == 0) {
      rtDW.is_RemoSend_UpPLC = IN_Init_c;
      RemotSend_UpPLCData_flg = 1U;
    }
  }

  if (rtDW.is_Remo_SendChgErr == IN_Init_c) {
    if (rtDW.RemoSend_CharErr_cou >= 100.0) {
      rtDW.RemoSend_CharErr_cou = 0.0;
      tmp = index_Err + 1;
      if (tmp > 255) {
        tmp = 255;
      }

      index_Err = (uint8_T)tmp;
      rtDW.is_Remo_SendChgErr = IN_Init_c;
      RemoSend_CharErr_Deal();
    } else {
      rtDW.RemoSend_CharErr_cou++;
    }
  } else {
    if (MSqlite_WrtErr_flg == 0) {
      rtDW.is_Remo_SendChgErr = IN_Init_c;
      RemoSend_CharErr_Deal();
    }
  }

  if (rtDW.is_RemoSend_UpACEng == IN_Init_c) {
    if (rtDW.RemotSend_UpACEng_cou >= 1000.0) {
      rtDW.RemotSend_UpACEng_cou = 0.0;
      rtDW.is_RemoSend_UpACEng = IN_Init_c;
      RemotSend_UpACEngData_flg = 1U;
    } else {
      rtDW.RemotSend_UpACEng_cou++;

      /* MCE_Comm_flg=0; */
    }
  } else {
    if (MSqlite_WrtErr_flg == 0) {
      rtDW.is_RemoSend_UpACEng = IN_Init_c;
      RemotSend_UpACEngData_flg = 1U;
    }
  }

  if (rtDW.is_RemotSend_UpAirCon == IN_Init_c) {
    if (rtDW.RemotSend_UpAirCon_cou >= 1000.0) {
      rtDW.RemotSend_UpAirCon_cou = 0.0;
      rtDW.is_RemotSend_UpAirCon = IN_Init_c;
      RemotSend_UpAirConData_flg = 1U;
    } else {
      rtDW.RemotSend_UpAirCon_cou++;

      /* MCE_Comm_flg=0; */
    }
  } else {
    if (MSqlite_WrtErr_flg == 0) {
      rtDW.is_RemotSend_UpAirCon = IN_Init_c;
      RemotSend_UpAirConData_flg = 1U;
    }
  }

  if (rtDW.is_RemotSend_UpFan == IN_Init_c) {
    if (rtDW.RemotSend_UpFan_cou >= 1000.0) {
      rtDW.RemotSend_UpFan_cou = 0.0;
      rtDW.is_RemotSend_UpFan = IN_Init_c;
      RemotSend_UpFanData_flg = 1U;
    } else {
      rtDW.RemotSend_UpFan_cou++;

      /* MCE_Comm_flg=0; */
    }
  } else {
    if (MSqlite_WrtErr_flg == 0) {
      rtDW.is_RemotSend_UpFan = IN_Init_c;
      RemotSend_UpFanData_flg = 1U;
    }
  }

  if (rtDW.is_RemotSend_UpFire == IN_Init_c) {
    if (rtDW.RemotSend_UpFire_cou >= 1000.0) {
      rtDW.RemotSend_UpFire_cou = 0.0;
      rtDW.is_RemotSend_UpFire = IN_Init_c;
      RemotSend_UpFireData_flg = 1U;
    } else {
      rtDW.RemotSend_UpFire_cou++;

      /* MCE_Comm_flg=0; */
    }
  } else {
    if (MSqlite_WrtErr_flg == 0) {
      rtDW.is_RemotSend_UpFire = IN_Init_c;
      RemotSend_UpFireData_flg = 1U;
    }
  }

  if (rtDW.is_RemotSend_UpCEing == IN_Init_c) {
    if (rtDW.RemotSend_UpCEing_cou >= 1000.0) {
      rtDW.is_RemotSend_UpCEing = IN_Init_c;
      rtDW.RemotSend_UpCEing_cou = 0.0;
      RemotSend_UpCEing_Deal();
    } else {
      rtDW.RemotSend_UpCEing_cou++;

      /* MCE_Comm_flg=0; */
    }
  } else {
    if (MSqlite_WrtErr_flg == 0) {
      rtDW.is_RemotSend_UpCEing = IN_Init_c;
      rtDW.RemotSend_UpCEing_cou = 0.0;
      RemotSend_UpCEing_Deal();
    }
  }

  if (rtDW.is_RemotSend_Heart == IN_Init_c) {
    if (rtDW.RemotSend_Heart_cou >= 500.0) {
      rtDW.RemotSend_Heart_cou = 0.0;
      rtDW.is_RemotSend_Heart = IN_Init_c;
      RemotSend_Heart_flg = 1U;
    } else {
      rtDW.RemotSend_Heart_cou++;

      /* MCE_Comm_flg=0; */
    }
  } else {
    if (MSqlite_WrtErr_flg == 0) {
      rtDW.is_RemotSend_Heart = IN_Init_c;
      RemotSend_Heart_flg = 1U;
    }
  }

  Led_AllPlay();
}

/* Function for Chart: '<Root>/Chart' */
void In_FIFO(void)
{
  int32_T i;
  i = 0;
  do {
    MLocat_FIFO_Buf[i] = (uint8_T)i;
    i++;
  } while (i <= 19);
}

/* Function for Chart: '<Root>/Chart' */
void enter_internal_c3_SA4_EMS(void)
{
  /* Chart: '<Root>/Chart' */
  rtDW.is_TimeOut_Order = IN_Init_c;
  Sys_sta[9] = 1U;
  QT_TimeOutOrder_flg = 0U;
  MCE_GetTimeOut_flg = 0U;
  rtDW.is_BatInLocator_Order = IN_Init_c;
  Sys_sta[2] = 1U;
  QT_InLocatorOrder_flg = 0U;
  MCE_GetInLocator_flg = 0U;
  rtDW.is_Init_Order = IN_Init_c;
  Sys_sta[1] = 1U;
  QT_InitOrder_flg = 0U;
  MCE_GetInit_flg = 0U;
  rtDW.is_Continue_Order = IN_Init_c;
  Sys_sta[10] = 1U;
  QT_ContinueOrder_flg = 0U;
  MCE_GetContinue_flg = 0U;
  rtDW.is_BatOutLocator_Order = IN_Init_c;
  Sys_sta[3] = 1U;
  QT_OutLocatorOrder_flg = 0U;
  MCE_GetOutLocator_flg = 0U;
  rtDW.is_BatOut_Order = IN_Init_c;
  Sys_sta[5] = 1U;
  QT_BatOutOrder_flg = 0U;
  MCE_GetBatOut_flg = 0U;
  rtDW.is_Stop_Order = IN_Init_c;
  Sys_sta[11] = 1U;
  QT_StopOrder_flg = 0U;
  MCE_GetStop_flg = 0U;
  rtDW.is_CELocator_Order = IN_Init_c;
  Sys_sta[4] = 1U;
  QT_CELocatorOrder_flg = 0U;
  MCE_GetCELocator_flg = 0U;
  rtDW.is_ChangeBatt_Order = IN_Init_c;
  Sys_sta[7] = 1U;
  QT_ChangeBatOrder_flg = 0U;
  MCE_GetChangeBat_flg = 0U;
  rtDW.is_MountBat_Order = IN_Init_c;
  Sys_sta[8] = 1U;
  MCE_GetMountBat_flg = 0U;
  rtDW.is_DisMountBat_Order = IN_Init_c;
  Sys_sta[6] = 1U;
  MCE_GetDismountBat_flg = 0U;
  rtDW.is_ErrReset_Order = IN_Init_c;
  Sys_sta[12] = 1U;
  QT_ErrResetOrder_flg = 0U;
  MCE_GetErrReset_flg = 0U;
  In_FIFO();
  rtDW.is_Main_CE = IN_Main_Init;
  Sys_Big_sta = 0U;
  rtDW.is_Main_Init = IN_Init3_bc;
  Sta = 0U;
  rtDW.is_MIO_Door_Wat_Lig_Thu = IN_Init3_bci2;

  /* Sta = 2; */
  rtDW.is_Qt_SendChgDat = IN_Init_c;
  rtDW.QtSend_CharDa_cou = 0.0;
  QtSend_CharDa_Deal();
  rtDW.is_Qt_SendChgErr = IN_Init_c;
  rtDW.QtSend_CharErr_cou = 0.0;
  QtSend_CharErr_Deal();
  rtDW.is_Qt_SendCEDat = IN_Init_c;
  rtDW.QtSend_CEDat_cou = 0.0;
  QtSend_CEDat_Deal();
  rtDW.is_Qt_SendChgErr1 = IN_Init_c;
  rtDW.QtSend_LocatData_cou = 0.0;
  QtSend_LocatData_Deal();
  rtDW.is_Qt_SendAirData = IN_Init_c;
  rtDW.QtSend_AirData_cou = 0.0;
  QtSend_AirData_Deal();
  rtDW.is_Qt_SendDcEne = IN_Init_c;
  rtDW.QtSend_DcEne_cou = 0.0;
  QtSend_DcEne_Deal();
  rtDW.is_Qt_SendDcEne2 = IN_Init_c;
  rtDW.QtSend_DcEne2_cou = 0.0;
  QtSend_DcEne2_Deal();
  rtDW.RemoSend_CharDa_cou = 30.0;
  index_Chu = 0U;
  rtDW.is_Remo_SendChgDat = IN_Init_c;
  RemoSend_CharDa_Deal();
  rtDW.RemotSend_UpPLC_cou = 0.0;
  rtDW.is_RemoSend_UpPLC = IN_Init_c;
  RemotSend_UpPLCData_flg = 1U;
  rtDW.RemoSend_CharErr_cou = 80.0;
  index_Err = 0U;
  rtDW.is_Remo_SendChgErr = IN_Init_c;
  RemoSend_CharErr_Deal();
  rtDW.RemotSend_UpACEng_cou = 100.0;
  rtDW.is_RemoSend_UpACEng = IN_Init_c;
  RemotSend_UpACEngData_flg = 1U;
  rtDW.RemotSend_UpAirCon_cou = 200.0;
  rtDW.is_RemotSend_UpAirCon = IN_Init_c;
  RemotSend_UpAirConData_flg = 1U;
  rtDW.RemotSend_UpFan_cou = 300.0;
  rtDW.is_RemotSend_UpFan = IN_Init_c;
  RemotSend_UpFanData_flg = 1U;
  rtDW.RemotSend_UpFire_cou = 400.0;
  rtDW.is_RemotSend_UpFire = IN_Init_c;
  RemotSend_UpFireData_flg = 1U;
  rtDW.is_RemotSend_UpCEing = IN_Init_c;
  rtDW.RemotSend_UpCEing_cou = 0.0;
  RemotSend_UpCEing_Deal();
  rtDW.RemotSend_Heart_cou = 330.0;
  rtDW.is_RemotSend_Heart = IN_Init_c;
  RemotSend_Heart_flg = 1U;
  MLed_Play_sta = 0U;
  rtDW.is_Led_AllPlay = IN_Play_Sta1;
  MLed_Big_sta = 1U;
  rtDW.InitErr_cou = 2400.0;
}

/* Output and update for atomic system: '<Root>/Chart' */
void ChangeElec(void)
{
  /* Chart: '<Root>/Chart' */
  if (rtDW.is_active_c3_SA4_EMS == 0U) {
    rtDW.is_active_c3_SA4_EMS = 1U;
    enter_internal_c3_SA4_EMS();
  } else {
    MAIN();
  }

  /* End of Chart: '<Root>/Chart' */
}

/* Function for Chart: '<Root>/Chart1' */
void DatLib_CharDa_Deal(void)
{
  int32_T i;
  int32_T exitg1;
  i = 0;
  do {
    exitg1 = 0;
    MSqlite_WriCharData_flg[i] = (uint8_T)((MLocat_Conf_ena[i] == 1) &&
      (EMS_Chger_BigSta[i] >= 1));

    /* MCE_InLocator_dest = 1; */
    if (i < 19) {
      i++;
    } else {
      exitg1 = 1;
    }
  } while (exitg1 == 0);
}

/* Function for Chart: '<Root>/Chart1' */
void DatLib_ConfDa_Deal(void)
{
  if ((MQt_ConfDa_push == 1) && (MSqlite_WrtErr_flg == 0)) {
    MSqlite_WriConfigData_flg = 1U;
    MQt_ConfDa_push = 0U;
  } else {
    /* {MSqlite_WriConfigData_flg = 0} */
  }

  /* MCE_InLocator_dest = 1; */
}

/* Function for Chart: '<Root>/Chart1' */
void DatLib_AirconDa_Deal(void)
{
  MSqlite_WriAirConData_flg = (uint8_T)(MSqlite_WrtErr_flg == 0);

  /* MCE_InLocator_dest = 1; */
}

/* Function for Chart: '<Root>/Chart1' */
void DatLib_LocatDa_Deal(void)
{
  int32_T i;
  int32_T exitg1;
  i = 0;
  do {
    exitg1 = 0;
    if (CHU_CharFull_flg[i] == 1) {
      if (rtDW.MSqlite_WriLoc_ok[i] == 0.0) {
        MSqlite_WriLocatData_flg[i] = 1U;
        rtDW.MSqlite_WriLoc_ok[i] = 1.0;
      } else {
        MSqlite_WriLocatData_flg[i] = 0U;
      }
    } else {
      rtDW.MSqlite_WriLoc_ok[i] = 0.0;
      MSqlite_WriLocatData_flg[i] = 0U;
    }

    if (i < 19) {
      i++;
    } else {
      exitg1 = 1;
    }
  } while (exitg1 == 0);
}

/* Function for Chart: '<Root>/Chart1' */
void DatLib_LocatDa_Deal_c(void)
{
  MSqlite_WriErrData_flg = (uint8_T)((MCE_AllErr_Lev >= 3) &&
    (MSqlite_WrtErr_flg == 0));

  /* MCE_InLocator_dest = 1; */
}

/* Function for Chart: '<Root>/Chart1' */
void DatLib_BattData_Deal(void)
{
  int32_T i;
  int32_T exitg1;
  i = 0;
  do {
    exitg1 = 0;
    MSqlite_WriBattData_flg[i] = (uint8_T)((MLocat_Conf_ena[i] == 1) &&
      (EMS_Chger_BigSta[i] >= 1));

    /* MCE_InLocator_dest = 1; */
    if (i < 19) {
      i++;
    } else {
      exitg1 = 1;
    }
  } while (exitg1 == 0);
}

/* Function for Chart: '<Root>/Chart1' */
void DatLib_AcEnergy_Deal(void)
{
  MSqlite_WriAcEnergy_flg = 1U;

  /* MCE_InLocator_dest = 1; */
}

/* Output and update for atomic system: '<Root>/Chart1' */
void DatLib(void)
{
  /* Chart: '<Root>/Chart1' */
  if (rtDW.is_active_c4_SA4_EMS == 0U) {
    rtDW.is_active_c4_SA4_EMS = 1U;
    rtDW.is_DatLib_CharDa = IN_Init_l;
    rtDW.DatLib_CharDa_cou = 0.0;
    DatLib_CharDa_Deal();
    rtDW.is_DatLib_ConfDa = IN_Init_l;
    rtDW.is_DatLib_AirconDa = IN_Init_l;
    rtDW.DatLib_AirconDa_cou = 0.0;
    DatLib_AirconDa_Deal();
    rtDW.is_DatLib_LocatDa = IN_Init_l;
    rtDW.is_DatLib_Err = IN_Init_l;
    rtDW.is_DatLib_BattData = IN_Init_l;
    rtDW.DatLib_BattData_cou = 0.0;
    DatLib_BattData_Deal();
    rtDW.is_DatLib_AcEnergy = IN_Init_l;
    rtDW.DatLib_AcEnergy_cou = 0.0;
    DatLib_AcEnergy_Deal();
  } else {
    if (rtDW.is_DatLib_CharDa == IN_Init_l) {
      if (MSqlite_WrtErr_flg == 1) {
        rtDW.is_DatLib_CharDa = IN_Init2_g;

        /* Sta = 2; */
      } else if (rtDW.DatLib_CharDa_cou >= 60.0) {
        rtDW.is_DatLib_CharDa = IN_Init_l;
        rtDW.DatLib_CharDa_cou = 0.0;
        DatLib_CharDa_Deal();
      } else {
        rtDW.DatLib_CharDa_cou++;

        /* MCE_Comm_flg=0; */
      }
    } else {
      if (MSqlite_WrtErr_flg == 0) {
        rtDW.is_DatLib_CharDa = IN_Init_l;
        rtDW.DatLib_CharDa_cou = 0.0;
        DatLib_CharDa_Deal();
      }
    }

    if (rtDW.is_DatLib_ConfDa == IN_Init_l) {
      if (MSqlite_WrtErr_flg == 1) {
        rtDW.is_DatLib_ConfDa = IN_Init2_g;

        /* Sta = 2; */
      } else {
        DatLib_ConfDa_Deal();

        /* MCE_Comm_flg=0; */
      }
    } else {
      if (MSqlite_WrtErr_flg == 0) {
        rtDW.is_DatLib_ConfDa = IN_Init_l;
      }
    }

    if (rtDW.is_DatLib_AirconDa == IN_Init_l) {
      if (MSqlite_WrtErr_flg == 1) {
        rtDW.is_DatLib_AirconDa = IN_Init2_g;

        /* Sta = 2; */
      } else if (rtDW.DatLib_AirconDa_cou >= 60.0) {
        rtDW.is_DatLib_AirconDa = IN_Init_l;
        rtDW.DatLib_AirconDa_cou = 0.0;
        DatLib_AirconDa_Deal();
      } else {
        rtDW.DatLib_AirconDa_cou++;

        /* MCE_Comm_flg=0; */
      }
    } else {
      if (MSqlite_WrtErr_flg == 0) {
        rtDW.is_DatLib_AirconDa = IN_Init_l;
        rtDW.DatLib_AirconDa_cou = 0.0;
        DatLib_AirconDa_Deal();
      }
    }

    if (rtDW.is_DatLib_LocatDa == IN_Init_l) {
      if (MSqlite_WrtErr_flg == 1) {
        rtDW.is_DatLib_LocatDa = IN_Init2_g;

        /* Sta = 2; */
      } else {
        DatLib_LocatDa_Deal();

        /* MCE_Comm_flg=0; */
      }
    } else {
      if (MSqlite_WrtErr_flg == 0) {
        rtDW.is_DatLib_LocatDa = IN_Init_l;
      }
    }

    if (rtDW.is_DatLib_Err == IN_Init_l) {
      if (MSqlite_WrtErr_flg == 1) {
        rtDW.is_DatLib_Err = IN_Init2_g;

        /* Sta = 2; */
      } else {
        DatLib_LocatDa_Deal_c();

        /* MCE_Comm_flg=0; */
      }
    } else {
      if (MSqlite_WrtErr_flg == 0) {
        rtDW.is_DatLib_Err = IN_Init_l;
      }
    }

    if (rtDW.is_DatLib_BattData == IN_Init_l) {
      if (MSqlite_WrtErr_flg == 1) {
        rtDW.is_DatLib_BattData = IN_Init2_g;

        /* Sta = 2; */
      } else if (rtDW.DatLib_BattData_cou >= 60.0) {
        rtDW.is_DatLib_BattData = IN_Init_l;
        rtDW.DatLib_BattData_cou = 0.0;
        DatLib_BattData_Deal();
      } else {
        rtDW.DatLib_BattData_cou++;

        /* MCE_Comm_flg=0; */
      }
    } else {
      if (MSqlite_WrtErr_flg == 0) {
        rtDW.is_DatLib_BattData = IN_Init_l;
        rtDW.DatLib_BattData_cou = 0.0;
        DatLib_BattData_Deal();
      }
    }

    if (rtDW.is_DatLib_AcEnergy == IN_Init_l) {
      if (MSqlite_WrtErr_flg == 1) {
        rtDW.is_DatLib_AcEnergy = IN_Init2_g;

        /* Sta = 2; */
      } else if (rtDW.DatLib_AcEnergy_cou >= 60.0) {
        rtDW.is_DatLib_AcEnergy = IN_Init_l;
        rtDW.DatLib_AcEnergy_cou = 0.0;
        DatLib_AcEnergy_Deal();
      } else {
        rtDW.DatLib_AcEnergy_cou++;

        /* MCE_Comm_flg=0; */
      }
    } else {
      if (MSqlite_WrtErr_flg == 0) {
        rtDW.is_DatLib_AcEnergy = IN_Init_l;
        rtDW.DatLib_AcEnergy_cou = 0.0;
        DatLib_AcEnergy_Deal();
      }
    }
  }

  /* End of Chart: '<Root>/Chart1' */
}

/* Function for Chart: '<S3>/Chart1' */
void MLocatErr_Deal(real_T rtu_CHU_index)
{
  int32_T rtu_CHU_index_0;
  int32_T rtu_CHU_index_1;
  int32_T rtu_CHU_index_2;
  int32_T rtu_CHU_index_3;
  int32_T rtu_CHU_index_4;
  if (rtu_CHU_index < 2.147483648E+9) {
    if (rtu_CHU_index >= -2.147483648E+9) {
      rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
    } else {
      rtu_CHU_index_0 = MIN_int32_T;
    }
  } else {
    rtu_CHU_index_0 = MAX_int32_T;
  }

  if (MLocat_ErrReset_flg[rtu_CHU_index_0] == 1) {
    if (rtu_CHU_index < 2.147483648E+9) {
      if (rtu_CHU_index >= -2.147483648E+9) {
        rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
      } else {
        rtu_CHU_index_0 = MIN_int32_T;
      }
    } else {
      rtu_CHU_index_0 = MAX_int32_T;
    }

    CHU_AllErr_flg[rtu_CHU_index_0] = 0U;
    if (rtu_CHU_index < 2.147483648E+9) {
      if (rtu_CHU_index >= -2.147483648E+9) {
        rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
      } else {
        rtu_CHU_index_0 = MIN_int32_T;
      }
    } else {
      rtu_CHU_index_0 = MAX_int32_T;
    }

    MLocat_ErrReset_flg[rtu_CHU_index_0] = 0U;
    if (rtu_CHU_index < 2.147483648E+9) {
      if (rtu_CHU_index >= -2.147483648E+9) {
        rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
      } else {
        rtu_CHU_index_0 = MIN_int32_T;
      }
    } else {
      rtu_CHU_index_0 = MAX_int32_T;
    }

    Get_CHUErrStop_flg[rtu_CHU_index_0] = 0U;
    if (rtu_CHU_index < 2.147483648E+9) {
      if (rtu_CHU_index >= -2.147483648E+9) {
        rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
      } else {
        rtu_CHU_index_0 = MIN_int32_T;
      }
    } else {
      rtu_CHU_index_0 = MAX_int32_T;
    }

    MChg_ErrStop_code[rtu_CHU_index_0] = 0U;
  } else {
    if (rtu_CHU_index < 2.147483648E+9) {
      if (rtu_CHU_index >= -2.147483648E+9) {
        rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
      } else {
        rtu_CHU_index_0 = MIN_int32_T;
      }
    } else {
      rtu_CHU_index_0 = MAX_int32_T;
    }

    if (BATT_IsExist_flg[rtu_CHU_index_0] == 1) {
      if (rtu_CHU_index < 2.147483648E+9) {
        if (rtu_CHU_index >= -2.147483648E+9) {
          rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
          rtu_CHU_index_1 = (int32_T)rtu_CHU_index;
          rtu_CHU_index_2 = (int32_T)rtu_CHU_index;
          rtu_CHU_index_3 = (int32_T)rtu_CHU_index;
          rtu_CHU_index_4 = (int32_T)rtu_CHU_index;
        } else {
          rtu_CHU_index_0 = MIN_int32_T;
          rtu_CHU_index_1 = MIN_int32_T;
          rtu_CHU_index_2 = MIN_int32_T;
          rtu_CHU_index_3 = MIN_int32_T;
          rtu_CHU_index_4 = MIN_int32_T;
        }
      } else {
        rtu_CHU_index_0 = MAX_int32_T;
        rtu_CHU_index_1 = MAX_int32_T;
        rtu_CHU_index_2 = MAX_int32_T;
        rtu_CHU_index_3 = MAX_int32_T;
        rtu_CHU_index_4 = MAX_int32_T;
      }

      if (((Get_CHUErrStop_flg[rtu_CHU_index_0] == 1) &&
           (MChg_ErrStop_code[rtu_CHU_index_1] != 63) &&
           (MChg_ErrStop_code[rtu_CHU_index_2] != 65)) ||
          ((CHU_ErrCode_buf3[rtu_CHU_index_3] != 40) &&
           (CHU_ErrCode_buf3[rtu_CHU_index_4] > 0))) {
        if (rtu_CHU_index < 2.147483648E+9) {
          if (rtu_CHU_index >= -2.147483648E+9) {
            rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
          } else {
            rtu_CHU_index_0 = MIN_int32_T;
          }
        } else {
          rtu_CHU_index_0 = MAX_int32_T;
        }

        CHU_AllErr_flg[rtu_CHU_index_0] = 1U;
      }
    }
  }
}

/* Function for Chart: '<S3>/Chart1' */
void init_clear(real_T rtu_CHU_index)
{
  int32_T rtu_CHU_index_0;
  if (rtu_CHU_index < 2.147483648E+9) {
    if (rtu_CHU_index >= -2.147483648E+9) {
      rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
    } else {
      rtu_CHU_index_0 = MIN_int32_T;
    }
  } else {
    rtu_CHU_index_0 = MAX_int32_T;
  }

  Hand_LowPwrOn_flg[rtu_CHU_index_0] = 0U;
  if (rtu_CHU_index < 2.147483648E+9) {
    if (rtu_CHU_index >= -2.147483648E+9) {
      rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
    } else {
      rtu_CHU_index_0 = MIN_int32_T;
    }
  } else {
    rtu_CHU_index_0 = MAX_int32_T;
  }

  CHU_LowPwrOn_timeout[rtu_CHU_index_0] = 0U;
  if (rtu_CHU_index < 2.147483648E+9) {
    if (rtu_CHU_index >= -2.147483648E+9) {
      rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
    } else {
      rtu_CHU_index_0 = MIN_int32_T;
    }
  } else {
    rtu_CHU_index_0 = MAX_int32_T;
  }

  CHU_LowPwrOn_count[rtu_CHU_index_0] = 0U;
  if (rtu_CHU_index < 2.147483648E+9) {
    if (rtu_CHU_index >= -2.147483648E+9) {
      rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
    } else {
      rtu_CHU_index_0 = MIN_int32_T;
    }
  } else {
    rtu_CHU_index_0 = MAX_int32_T;
  }

  EMS_SendLowPwrOnCmd_flg[rtu_CHU_index_0] = 0U;
  if (rtu_CHU_index < 2.147483648E+9) {
    if (rtu_CHU_index >= -2.147483648E+9) {
      rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
    } else {
      rtu_CHU_index_0 = MIN_int32_T;
    }
  } else {
    rtu_CHU_index_0 = MAX_int32_T;
  }

  EMS_SendLowPwrDownCmd_flg[rtu_CHU_index_0] = 0U;
  if (rtu_CHU_index < 2.147483648E+9) {
    if (rtu_CHU_index >= -2.147483648E+9) {
      rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
    } else {
      rtu_CHU_index_0 = MIN_int32_T;
    }
  } else {
    rtu_CHU_index_0 = MAX_int32_T;
  }

  CHU_LowPwrDown_count[rtu_CHU_index_0] = 0U;
  if (rtu_CHU_index < 2.147483648E+9) {
    if (rtu_CHU_index >= -2.147483648E+9) {
      rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
    } else {
      rtu_CHU_index_0 = MIN_int32_T;
    }
  } else {
    rtu_CHU_index_0 = MAX_int32_T;
  }

  CHU_LowPwrDown_timeout[rtu_CHU_index_0] = 0U;

  /* Hand_StartChg_flg[CHU_index]=0 */
  if (rtu_CHU_index < 2.147483648E+9) {
    if (rtu_CHU_index >= -2.147483648E+9) {
      rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
    } else {
      rtu_CHU_index_0 = MIN_int32_T;
    }
  } else {
    rtu_CHU_index_0 = MAX_int32_T;
  }

  EMS_SendStartChgCmd_flg[rtu_CHU_index_0] = 0U;
  if (rtu_CHU_index < 2.147483648E+9) {
    if (rtu_CHU_index >= -2.147483648E+9) {
      rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
    } else {
      rtu_CHU_index_0 = MIN_int32_T;
    }
  } else {
    rtu_CHU_index_0 = MAX_int32_T;
  }

  StartChg_count[rtu_CHU_index_0] = 0U;
  if (rtu_CHU_index < 2.147483648E+9) {
    if (rtu_CHU_index >= -2.147483648E+9) {
      rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
    } else {
      rtu_CHU_index_0 = MIN_int32_T;
    }
  } else {
    rtu_CHU_index_0 = MAX_int32_T;
  }

  StartChg_timeout[rtu_CHU_index_0] = 0U;
  if (rtu_CHU_index < 2.147483648E+9) {
    if (rtu_CHU_index >= -2.147483648E+9) {
      rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
    } else {
      rtu_CHU_index_0 = MIN_int32_T;
    }
  } else {
    rtu_CHU_index_0 = MAX_int32_T;
  }

  EMS_SendSetPwrCmd_flg[rtu_CHU_index_0] = 0U;
  if (rtu_CHU_index < 2.147483648E+9) {
    if (rtu_CHU_index >= -2.147483648E+9) {
      rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
    } else {
      rtu_CHU_index_0 = MIN_int32_T;
    }
  } else {
    rtu_CHU_index_0 = MAX_int32_T;
  }

  SetPwr_count[rtu_CHU_index_0] = 0U;
  if (rtu_CHU_index < 2.147483648E+9) {
    if (rtu_CHU_index >= -2.147483648E+9) {
      rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
    } else {
      rtu_CHU_index_0 = MIN_int32_T;
    }
  } else {
    rtu_CHU_index_0 = MAX_int32_T;
  }

  Get_CHUChgRtData_count[rtu_CHU_index_0] = 0U;
  if (rtu_CHU_index < 2.147483648E+9) {
    if (rtu_CHU_index >= -2.147483648E+9) {
      rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
    } else {
      rtu_CHU_index_0 = MIN_int32_T;
    }
  } else {
    rtu_CHU_index_0 = MAX_int32_T;
  }

  Hand_StopChg_flg[rtu_CHU_index_0] = 0U;
  if (rtu_CHU_index < 2.147483648E+9) {
    if (rtu_CHU_index >= -2.147483648E+9) {
      rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
    } else {
      rtu_CHU_index_0 = MIN_int32_T;
    }
  } else {
    rtu_CHU_index_0 = MAX_int32_T;
  }

  EMS_SendStopChgCmd_flg[rtu_CHU_index_0] = 0U;
  if (rtu_CHU_index < 2.147483648E+9) {
    if (rtu_CHU_index >= -2.147483648E+9) {
      rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
    } else {
      rtu_CHU_index_0 = MIN_int32_T;
    }
  } else {
    rtu_CHU_index_0 = MAX_int32_T;
  }

  StopChg_count[rtu_CHU_index_0] = 0U;
  if (rtu_CHU_index < 2.147483648E+9) {
    if (rtu_CHU_index >= -2.147483648E+9) {
      rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
    } else {
      rtu_CHU_index_0 = MIN_int32_T;
    }
  } else {
    rtu_CHU_index_0 = MAX_int32_T;
  }

  StopChg_timeout[rtu_CHU_index_0] = 0U;
  if (rtu_CHU_index < 2.147483648E+9) {
    if (rtu_CHU_index >= -2.147483648E+9) {
      rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
    } else {
      rtu_CHU_index_0 = MIN_int32_T;
    }
  } else {
    rtu_CHU_index_0 = MAX_int32_T;
  }

  Get_CHUErrStop_flg[rtu_CHU_index_0] = 0U;
  if (rtu_CHU_index < 2.147483648E+9) {
    if (rtu_CHU_index >= -2.147483648E+9) {
      rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
    } else {
      rtu_CHU_index_0 = MIN_int32_T;
    }
  } else {
    rtu_CHU_index_0 = MAX_int32_T;
  }

  Hand_LowPwrDown_flg[rtu_CHU_index_0] = 0U;
  if (rtu_CHU_index < 2.147483648E+9) {
    if (rtu_CHU_index >= -2.147483648E+9) {
      rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
    } else {
      rtu_CHU_index_0 = MIN_int32_T;
    }
  } else {
    rtu_CHU_index_0 = MAX_int32_T;
  }

  CHU_CharFull_flg[rtu_CHU_index_0] = 0U;

  /* 满充标志 */
  if (rtu_CHU_index < 2.147483648E+9) {
    if (rtu_CHU_index >= -2.147483648E+9) {
      rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
    } else {
      rtu_CHU_index_0 = MIN_int32_T;
    }
  } else {
    rtu_CHU_index_0 = MAX_int32_T;
  }

  Get_CHUErrStop_flg[rtu_CHU_index_0] = 0U;
  if (rtu_CHU_index < 2.147483648E+9) {
    if (rtu_CHU_index >= -2.147483648E+9) {
      rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
    } else {
      rtu_CHU_index_0 = MIN_int32_T;
    }
  } else {
    rtu_CHU_index_0 = MAX_int32_T;
  }

  MChg_ErrStop_code[rtu_CHU_index_0] = 0U;
}

/* Function for Chart: '<S3>/Chart1' */
void CharEnd_Deal(real_T rtu_CHU_index)
{
  int32_T i;
  int32_T rtu_CHU_index_0;
  int32_T rtu_CHU_index_1;
  int32_T rtu_CHU_index_2;
  if (rtu_CHU_index < 2.147483648E+9) {
    if (rtu_CHU_index >= -2.147483648E+9) {
      i = (int32_T)rtu_CHU_index;
      rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
      rtu_CHU_index_1 = (int32_T)rtu_CHU_index;
      rtu_CHU_index_2 = (int32_T)rtu_CHU_index;
    } else {
      i = MIN_int32_T;
      rtu_CHU_index_0 = MIN_int32_T;
      rtu_CHU_index_1 = MIN_int32_T;
      rtu_CHU_index_2 = MIN_int32_T;
    }
  } else {
    i = MAX_int32_T;
    rtu_CHU_index_0 = MAX_int32_T;
    rtu_CHU_index_1 = MAX_int32_T;
    rtu_CHU_index_2 = MAX_int32_T;
  }

  if (((Get_CHUErrStop_flg[i] == 1) && ((MChg_ErrStop_code[rtu_CHU_index_0] ==
         63) || (MChg_ErrStop_code[rtu_CHU_index_1] == 65))) ||
      (MLocat_Soc_pct[rtu_CHU_index_2] >= MLocat_CharEndSoc_pct)) {
    if (rtu_CHU_index < 2.147483648E+9) {
      if (rtu_CHU_index >= -2.147483648E+9) {
        i = (int32_T)rtu_CHU_index;
      } else {
        i = MIN_int32_T;
      }
    } else {
      i = MAX_int32_T;
    }

    CHU_CharFull_flg[i] = 1U;
    for (i = 0; i < 20; i++) {
      Get_CHUErrStop_flg[i] = 0U;
    }
  } else {
    if (rtu_CHU_index < 2.147483648E+9) {
      if (rtu_CHU_index >= -2.147483648E+9) {
        i = (int32_T)rtu_CHU_index;
      } else {
        i = MIN_int32_T;
      }
    } else {
      i = MAX_int32_T;
    }

    CHU_CharFull_flg[i] = 0U;
  }
}

/* Function for Chart: '<S3>/Chart1' */
void exit_internal_charging_state(real_T rtu_CHU_index, DW_Chart1 *localDW)
{
  int32_T tmp;
  int32_T rtu_CHU_index_0;
  switch (localDW->is_chg_main_state) {
   case IN_SetPwr:
    /* 判断CHU，EMS故障
       CharEnd_Deal(); */
    if (rtu_CHU_index < 2.147483648E+9) {
      if (rtu_CHU_index >= -2.147483648E+9) {
        rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
      } else {
        rtu_CHU_index_0 = MIN_int32_T;
      }
    } else {
      rtu_CHU_index_0 = MAX_int32_T;
    }

    SetPwr_count[rtu_CHU_index_0] = 0U;
    CharEnd_Deal(rtu_CHU_index);
    localDW->is_chg_main_state = IN_NO_ACTIVE_CHILD;
    break;

   case IN_StartChg:
    if (rtu_CHU_index < 2.147483648E+9) {
      if (rtu_CHU_index >= -2.147483648E+9) {
        rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
      } else {
        rtu_CHU_index_0 = MIN_int32_T;
      }
    } else {
      rtu_CHU_index_0 = MAX_int32_T;
    }

    StartChg_count[rtu_CHU_index_0] = 0U;
    if (rtu_CHU_index < 2.147483648E+9) {
      if (rtu_CHU_index >= -2.147483648E+9) {
        rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
      } else {
        rtu_CHU_index_0 = MIN_int32_T;
      }
    } else {
      rtu_CHU_index_0 = MAX_int32_T;
    }

    tmp = StartChg_timeout[rtu_CHU_index_0] + 1;
    if (tmp > 255) {
      tmp = 255;
    }

    if (rtu_CHU_index < 2.147483648E+9) {
      if (rtu_CHU_index >= -2.147483648E+9) {
        rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
      } else {
        rtu_CHU_index_0 = MIN_int32_T;
      }
    } else {
      rtu_CHU_index_0 = MAX_int32_T;
    }

    StartChg_timeout[rtu_CHU_index_0] = (uint8_T)tmp;
    localDW->is_chg_main_state = IN_NO_ACTIVE_CHILD;
    break;

   case IN_StopChg:
    if (rtu_CHU_index < 2.147483648E+9) {
      if (rtu_CHU_index >= -2.147483648E+9) {
        rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
      } else {
        rtu_CHU_index_0 = MIN_int32_T;
      }
    } else {
      rtu_CHU_index_0 = MAX_int32_T;
    }

    StopChg_count[rtu_CHU_index_0] = 0U;
    if (rtu_CHU_index < 2.147483648E+9) {
      if (rtu_CHU_index >= -2.147483648E+9) {
        rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
      } else {
        rtu_CHU_index_0 = MIN_int32_T;
      }
    } else {
      rtu_CHU_index_0 = MAX_int32_T;
    }

    tmp = StopChg_timeout[rtu_CHU_index_0] + 1;
    if (tmp > 255) {
      tmp = 255;
    }

    if (rtu_CHU_index < 2.147483648E+9) {
      if (rtu_CHU_index >= -2.147483648E+9) {
        rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
      } else {
        rtu_CHU_index_0 = MIN_int32_T;
      }
    } else {
      rtu_CHU_index_0 = MAX_int32_T;
    }

    StopChg_timeout[rtu_CHU_index_0] = (uint8_T)tmp;
    localDW->is_chg_main_state = IN_NO_ACTIVE_CHILD;
    break;

   default:
    localDW->is_chg_main_state = IN_NO_ACTIVE_CHILD;
    break;
  }

  localDW->is_GetBattBasicData = IN_NO_ACTIVE_CHILD;
  localDW->is_GetBattRtData = IN_NO_ACTIVE_CHILD;
  localDW->is_GetCharPreInf = IN_NO_ACTIVE_CHILD;
  localDW->is_GetCellTemp = IN_NO_ACTIVE_CHILD;
  localDW->is_GetCellVol = IN_NO_ACTIVE_CHILD;
}

/* Function for Chart: '<S3>/Chart1' */
void clear(real_T rtu_CHU_index)
{
  int32_T rtu_CHU_index_0;
  if (rtu_CHU_index < 2.147483648E+9) {
    if (rtu_CHU_index >= -2.147483648E+9) {
      rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
    } else {
      rtu_CHU_index_0 = MIN_int32_T;
    }
  } else {
    rtu_CHU_index_0 = MAX_int32_T;
  }

  EMS_SendGetCellVolCmd_flg[rtu_CHU_index_0] = 0U;
  if (rtu_CHU_index < 2.147483648E+9) {
    if (rtu_CHU_index >= -2.147483648E+9) {
      rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
    } else {
      rtu_CHU_index_0 = MIN_int32_T;
    }
  } else {
    rtu_CHU_index_0 = MAX_int32_T;
  }

  EMS_SendGetCellTempCmd_flg[rtu_CHU_index_0] = 0U;
  if (rtu_CHU_index < 2.147483648E+9) {
    if (rtu_CHU_index >= -2.147483648E+9) {
      rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
    } else {
      rtu_CHU_index_0 = MIN_int32_T;
    }
  } else {
    rtu_CHU_index_0 = MAX_int32_T;
  }

  EMS_SendGetBattBasicDataCmd_flg[rtu_CHU_index_0] = 0U;
  if (rtu_CHU_index < 2.147483648E+9) {
    if (rtu_CHU_index >= -2.147483648E+9) {
      rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
    } else {
      rtu_CHU_index_0 = MIN_int32_T;
    }
  } else {
    rtu_CHU_index_0 = MAX_int32_T;
  }

  EMS_SendGetCharPreInfCmd_flg[rtu_CHU_index_0] = 0U;
  if (rtu_CHU_index < 2.147483648E+9) {
    if (rtu_CHU_index >= -2.147483648E+9) {
      rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
    } else {
      rtu_CHU_index_0 = MIN_int32_T;
    }
  } else {
    rtu_CHU_index_0 = MAX_int32_T;
  }

  EMS_SendGetBattRtDataCmd_flg[rtu_CHU_index_0] = 0U;
  if (rtu_CHU_index < 2.147483648E+9) {
    if (rtu_CHU_index >= -2.147483648E+9) {
      rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
    } else {
      rtu_CHU_index_0 = MIN_int32_T;
    }
  } else {
    rtu_CHU_index_0 = MAX_int32_T;
  }

  EMS_SendGetCharEndInfCmd_flg[rtu_CHU_index_0] = 0U;

  /* Hand_StartChg_flg[CHU_index]=0 */
}

/* Function for Chart: '<S3>/Chart1' */
void Get_CHUChgRtData(real_T rtu_CHU_index)
{
  int32_T tmp;
  int32_T rtu_CHU_index_0;
  if (rtu_CHU_index < 2.147483648E+9) {
    if (rtu_CHU_index >= -2.147483648E+9) {
      rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
    } else {
      rtu_CHU_index_0 = MIN_int32_T;
    }
  } else {
    rtu_CHU_index_0 = MAX_int32_T;
  }

  if (Get_CHUChgRtData_flg[rtu_CHU_index_0] == 1) {
    if (rtu_CHU_index < 2.147483648E+9) {
      if (rtu_CHU_index >= -2.147483648E+9) {
        rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
      } else {
        rtu_CHU_index_0 = MIN_int32_T;
      }
    } else {
      rtu_CHU_index_0 = MAX_int32_T;
    }

    Get_CHUChgRtData_flg[rtu_CHU_index_0] = 0U;
    if (rtu_CHU_index < 2.147483648E+9) {
      if (rtu_CHU_index >= -2.147483648E+9) {
        rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
      } else {
        rtu_CHU_index_0 = MIN_int32_T;
      }
    } else {
      rtu_CHU_index_0 = MAX_int32_T;
    }

    Get_CHUChgRtData_count[rtu_CHU_index_0] = 0U;
  } else {
    if (rtu_CHU_index < 2.147483648E+9) {
      if (rtu_CHU_index >= -2.147483648E+9) {
        rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
      } else {
        rtu_CHU_index_0 = MIN_int32_T;
      }
    } else {
      rtu_CHU_index_0 = MAX_int32_T;
    }

    tmp = Get_CHUChgRtData_count[rtu_CHU_index_0] + 1;
    if (tmp > 65535) {
      tmp = 65535;
    }

    if (rtu_CHU_index < 2.147483648E+9) {
      if (rtu_CHU_index >= -2.147483648E+9) {
        rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
      } else {
        rtu_CHU_index_0 = MIN_int32_T;
      }
    } else {
      rtu_CHU_index_0 = MAX_int32_T;
    }

    Get_CHUChgRtData_count[rtu_CHU_index_0] = (uint16_T)tmp;
  }

  if (rtu_CHU_index < 2.147483648E+9) {
    if (rtu_CHU_index >= -2.147483648E+9) {
      rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
    } else {
      rtu_CHU_index_0 = MIN_int32_T;
    }
  } else {
    rtu_CHU_index_0 = MAX_int32_T;
  }

  if (Get_CHUChgRtData_count[rtu_CHU_index_0] >= 2000) {
    if (rtu_CHU_index < 2.147483648E+9) {
      if (rtu_CHU_index >= -2.147483648E+9) {
        rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
      } else {
        rtu_CHU_index_0 = MIN_int32_T;
      }
    } else {
      rtu_CHU_index_0 = MAX_int32_T;
    }

    Get_CHUChgRtDataErr_flg[rtu_CHU_index_0] = 1U;
  }
}

/* Function for Chart: '<S3>/Chart1' */
void CHUPwrSet(real_T rtu_CHU_index)
{
  int32_T rtu_CHU_index_0;
  int32_T rtu_CHU_index_1;
  if (rtu_CHU_index < 2.147483648E+9) {
    if (rtu_CHU_index >= -2.147483648E+9) {
      rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
    } else {
      rtu_CHU_index_0 = MIN_int32_T;
    }
  } else {
    rtu_CHU_index_0 = MAX_int32_T;
  }

  if (Chg_Control_mode[rtu_CHU_index_0] == 0) {
    if (rtu_CHU_index < 2.147483648E+9) {
      if (rtu_CHU_index >= -2.147483648E+9) {
        rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
        rtu_CHU_index_1 = (int32_T)rtu_CHU_index;
      } else {
        rtu_CHU_index_0 = MIN_int32_T;
        rtu_CHU_index_1 = MIN_int32_T;
      }
    } else {
      rtu_CHU_index_0 = MAX_int32_T;
      rtu_CHU_index_1 = MAX_int32_T;
    }

    EMS_CHUSetPwr_kw[rtu_CHU_index_0] = Qt_HandCharPow_kw[rtu_CHU_index_1];
  } else {
    if (rtu_CHU_index < 2.147483648E+9) {
      if (rtu_CHU_index >= -2.147483648E+9) {
        rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
      } else {
        rtu_CHU_index_0 = MIN_int32_T;
      }
    } else {
      rtu_CHU_index_0 = MAX_int32_T;
    }

    EMS_CHUSetPwr_kw[rtu_CHU_index_0] = MLocat_CharPow_kw;
  }

  if (rtu_CHU_index < 2.147483648E+9) {
    if (rtu_CHU_index >= -2.147483648E+9) {
      rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
    } else {
      rtu_CHU_index_0 = MIN_int32_T;
    }
  } else {
    rtu_CHU_index_0 = MAX_int32_T;
  }

  if (!(EMS_CHUSetPwr_kw[rtu_CHU_index_0] <= 30.0)) {
    if (rtu_CHU_index < 2.147483648E+9) {
      if (rtu_CHU_index >= -2.147483648E+9) {
        rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
      } else {
        rtu_CHU_index_0 = MIN_int32_T;
      }
    } else {
      rtu_CHU_index_0 = MAX_int32_T;
    }

    EMS_CHUSetPwr_kw[rtu_CHU_index_0] = 0.0;
  }
}

/* Function for Chart: '<S3>/Chart1' */
void chg_main_state(real_T rtu_CHU_index, DW_Chart1 *localDW)
{
  int32_T rtu_CHU_index_0;
  int32_T rtu_CHU_index_1;
  int32_T rtu_CHU_index_2;
  int32_T rtu_CHU_index_3;
  boolean_T guard1 = false;
  guard1 = false;
  switch (localDW->is_chg_main_state) {
   case IN_CHU_StartChg_timeout:
    localDW->is_chg_main_state = IN_stop_ok3;
    if (rtu_CHU_index < 2.147483648E+9) {
      if (rtu_CHU_index >= -2.147483648E+9) {
        rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
      } else {
        rtu_CHU_index_0 = MIN_int32_T;
      }
    } else {
      rtu_CHU_index_0 = MAX_int32_T;
    }

    EMS_Chger_BigSta[rtu_CHU_index_0] = 5U;
    if (rtu_CHU_index < 2.147483648E+9) {
      if (rtu_CHU_index >= -2.147483648E+9) {
        rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
      } else {
        rtu_CHU_index_0 = MIN_int32_T;
      }
    } else {
      rtu_CHU_index_0 = MAX_int32_T;
    }

    chu_test_state[rtu_CHU_index_0] = 14U;
    if (rtu_CHU_index < 2.147483648E+9) {
      if (rtu_CHU_index >= -2.147483648E+9) {
        rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
      } else {
        rtu_CHU_index_0 = MIN_int32_T;
      }
    } else {
      rtu_CHU_index_0 = MAX_int32_T;
    }

    Hand_LowPwrDown_flg[rtu_CHU_index_0] = 0U;
    break;

   case IN_CHU_StopChg_timeout:
    localDW->is_chg_main_state = IN_stop_ok3;
    if (rtu_CHU_index < 2.147483648E+9) {
      if (rtu_CHU_index >= -2.147483648E+9) {
        rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
      } else {
        rtu_CHU_index_0 = MIN_int32_T;
      }
    } else {
      rtu_CHU_index_0 = MAX_int32_T;
    }

    EMS_Chger_BigSta[rtu_CHU_index_0] = 5U;
    if (rtu_CHU_index < 2.147483648E+9) {
      if (rtu_CHU_index >= -2.147483648E+9) {
        rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
      } else {
        rtu_CHU_index_0 = MIN_int32_T;
      }
    } else {
      rtu_CHU_index_0 = MAX_int32_T;
    }

    chu_test_state[rtu_CHU_index_0] = 14U;
    if (rtu_CHU_index < 2.147483648E+9) {
      if (rtu_CHU_index >= -2.147483648E+9) {
        rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
      } else {
        rtu_CHU_index_0 = MIN_int32_T;
      }
    } else {
      rtu_CHU_index_0 = MAX_int32_T;
    }

    Hand_LowPwrDown_flg[rtu_CHU_index_0] = 0U;
    break;

   case IN_SetPwr:
    if (rtu_CHU_index < 2.147483648E+9) {
      if (rtu_CHU_index >= -2.147483648E+9) {
        rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
        rtu_CHU_index_1 = (int32_T)rtu_CHU_index;
        rtu_CHU_index_2 = (int32_T)rtu_CHU_index;
        rtu_CHU_index_3 = (int32_T)rtu_CHU_index;
      } else {
        rtu_CHU_index_0 = MIN_int32_T;
        rtu_CHU_index_1 = MIN_int32_T;
        rtu_CHU_index_2 = MIN_int32_T;
        rtu_CHU_index_3 = MIN_int32_T;
      }
    } else {
      rtu_CHU_index_0 = MAX_int32_T;
      rtu_CHU_index_1 = MAX_int32_T;
      rtu_CHU_index_2 = MAX_int32_T;
      rtu_CHU_index_3 = MAX_int32_T;
    }

    if (((Chg_Control_mode[rtu_CHU_index_0] == 0) &&
         (Hand_StopChg_flg[rtu_CHU_index_1] == 1)) ||
        (CHU_CharFull_flg[rtu_CHU_index_2] == 1) ||
        (Get_CHUChgRtDataErr_flg[rtu_CHU_index_3] == 1) || ((MIO_Water_err == 1)
         || (FireContr_GetFire_flg == 1))) {
      if (rtu_CHU_index < 2.147483648E+9) {
        if (rtu_CHU_index >= -2.147483648E+9) {
          rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
        } else {
          rtu_CHU_index_0 = MIN_int32_T;
        }
      } else {
        rtu_CHU_index_0 = MAX_int32_T;
      }

      Get_CHUChgRtData_count[rtu_CHU_index_0] = 0U;
      if (rtu_CHU_index < 2.147483648E+9) {
        if (rtu_CHU_index >= -2.147483648E+9) {
          rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
        } else {
          rtu_CHU_index_0 = MIN_int32_T;
        }
      } else {
        rtu_CHU_index_0 = MAX_int32_T;
      }

      Hand_StopChg_flg[rtu_CHU_index_0] = 0U;
      guard1 = true;
    } else {
      if (rtu_CHU_index < 2.147483648E+9) {
        if (rtu_CHU_index >= -2.147483648E+9) {
          rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
        } else {
          rtu_CHU_index_0 = MIN_int32_T;
        }
      } else {
        rtu_CHU_index_0 = MAX_int32_T;
      }

      if (Get_CHUErrStop_flg[rtu_CHU_index_0] == 1) {
        /* {Get_CHUErrStop_flg[CHU_index] =0} */
        /* 判断CHU，EMS故障
           CharEnd_Deal(); */
        if (rtu_CHU_index < 2.147483648E+9) {
          if (rtu_CHU_index >= -2.147483648E+9) {
            rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
          } else {
            rtu_CHU_index_0 = MIN_int32_T;
          }
        } else {
          rtu_CHU_index_0 = MAX_int32_T;
        }

        SetPwr_count[rtu_CHU_index_0] = 0U;
        CharEnd_Deal(rtu_CHU_index);
        localDW->is_chg_main_state = IN_stop_ok3;
        if (rtu_CHU_index < 2.147483648E+9) {
          if (rtu_CHU_index >= -2.147483648E+9) {
            rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
          } else {
            rtu_CHU_index_0 = MIN_int32_T;
          }
        } else {
          rtu_CHU_index_0 = MAX_int32_T;
        }

        EMS_Chger_BigSta[rtu_CHU_index_0] = 5U;
        if (rtu_CHU_index < 2.147483648E+9) {
          if (rtu_CHU_index >= -2.147483648E+9) {
            rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
          } else {
            rtu_CHU_index_0 = MIN_int32_T;
          }
        } else {
          rtu_CHU_index_0 = MAX_int32_T;
        }

        chu_test_state[rtu_CHU_index_0] = 14U;
        if (rtu_CHU_index < 2.147483648E+9) {
          if (rtu_CHU_index >= -2.147483648E+9) {
            rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
          } else {
            rtu_CHU_index_0 = MIN_int32_T;
          }
        } else {
          rtu_CHU_index_0 = MAX_int32_T;
        }

        Hand_LowPwrDown_flg[rtu_CHU_index_0] = 0U;
      } else {
        if (rtu_CHU_index < 2.147483648E+9) {
          if (rtu_CHU_index >= -2.147483648E+9) {
            rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
          } else {
            rtu_CHU_index_0 = MIN_int32_T;
          }
        } else {
          rtu_CHU_index_0 = MAX_int32_T;
        }

        if (SetPwr_count[rtu_CHU_index_0] >= 400) {
          /* 判断CHU，EMS故障
             CharEnd_Deal(); */
          if (rtu_CHU_index < 2.147483648E+9) {
            if (rtu_CHU_index >= -2.147483648E+9) {
              rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
            } else {
              rtu_CHU_index_0 = MIN_int32_T;
            }
          } else {
            rtu_CHU_index_0 = MAX_int32_T;
          }

          SetPwr_count[rtu_CHU_index_0] = 0U;
          CharEnd_Deal(rtu_CHU_index);
          localDW->is_chg_main_state = IN_SetPwr;
          if (rtu_CHU_index < 2.147483648E+9) {
            if (rtu_CHU_index >= -2.147483648E+9) {
              rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
            } else {
              rtu_CHU_index_0 = MIN_int32_T;
            }
          } else {
            rtu_CHU_index_0 = MAX_int32_T;
          }

          EMS_Chger_BigSta[rtu_CHU_index_0] = 3U;
          if (rtu_CHU_index < 2.147483648E+9) {
            if (rtu_CHU_index >= -2.147483648E+9) {
              rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
            } else {
              rtu_CHU_index_0 = MIN_int32_T;
            }
          } else {
            rtu_CHU_index_0 = MAX_int32_T;
          }

          chu_test_state[rtu_CHU_index_0] = 12U;
          if (rtu_CHU_index < 2.147483648E+9) {
            if (rtu_CHU_index >= -2.147483648E+9) {
              rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
            } else {
              rtu_CHU_index_0 = MIN_int32_T;
            }
          } else {
            rtu_CHU_index_0 = MAX_int32_T;
          }

          EMS_SendSetPwrCmd_flg[rtu_CHU_index_0] = 1U;
        } else {
          if (rtu_CHU_index < 2.147483648E+9) {
            if (rtu_CHU_index >= -2.147483648E+9) {
              rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
              rtu_CHU_index_1 = (int32_T)rtu_CHU_index;
            } else {
              rtu_CHU_index_0 = MIN_int32_T;
              rtu_CHU_index_1 = MIN_int32_T;
            }
          } else {
            rtu_CHU_index_0 = MAX_int32_T;
            rtu_CHU_index_1 = MAX_int32_T;
          }

          if ((MChar_StartCE_flg[rtu_CHU_index_0] == 1) ||
              (CHU_AllErr_flg[rtu_CHU_index_1] == 1)) {
            if (rtu_CHU_index < 2.147483648E+9) {
              if (rtu_CHU_index >= -2.147483648E+9) {
                rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
              } else {
                rtu_CHU_index_0 = MIN_int32_T;
              }
            } else {
              rtu_CHU_index_0 = MAX_int32_T;
            }

            Get_CHUChgRtData_count[rtu_CHU_index_0] = 0U;
            guard1 = true;
          } else {
            /* 获取CHU上传的实时数据，超时退出 */
            Get_CHUChgRtData(rtu_CHU_index);
            CHUPwrSet(rtu_CHU_index);

            /* EMS_SendSetPwrCmd_flg[CHU_index] = 0; */
            if (rtu_CHU_index < 2.147483648E+9) {
              if (rtu_CHU_index >= -2.147483648E+9) {
                rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
              } else {
                rtu_CHU_index_0 = MIN_int32_T;
              }
            } else {
              rtu_CHU_index_0 = MAX_int32_T;
            }

            rtu_CHU_index_1 = SetPwr_count[rtu_CHU_index_0] + 1;
            if (rtu_CHU_index_1 > 65535) {
              rtu_CHU_index_1 = 65535;
            }

            if (rtu_CHU_index < 2.147483648E+9) {
              if (rtu_CHU_index >= -2.147483648E+9) {
                rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
              } else {
                rtu_CHU_index_0 = MIN_int32_T;
              }
            } else {
              rtu_CHU_index_0 = MAX_int32_T;
            }

            SetPwr_count[rtu_CHU_index_0] = (uint16_T)rtu_CHU_index_1;
          }
        }
      }
    }
    break;

   case IN_StartChg:
    if (rtu_CHU_index < 2.147483648E+9) {
      if (rtu_CHU_index >= -2.147483648E+9) {
        rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
      } else {
        rtu_CHU_index_0 = MIN_int32_T;
      }
    } else {
      rtu_CHU_index_0 = MAX_int32_T;
    }

    if (CHU_DevState_code[rtu_CHU_index_0] == 0x04) {
      if (rtu_CHU_index < 2.147483648E+9) {
        if (rtu_CHU_index >= -2.147483648E+9) {
          rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
        } else {
          rtu_CHU_index_0 = MIN_int32_T;
        }
      } else {
        rtu_CHU_index_0 = MAX_int32_T;
      }

      StartChg_timeout[rtu_CHU_index_0] = 0U;
      if (rtu_CHU_index < 2.147483648E+9) {
        if (rtu_CHU_index >= -2.147483648E+9) {
          rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
        } else {
          rtu_CHU_index_0 = MIN_int32_T;
        }
      } else {
        rtu_CHU_index_0 = MAX_int32_T;
      }

      StartChg_count[rtu_CHU_index_0] = 0U;
      if (rtu_CHU_index < 2.147483648E+9) {
        if (rtu_CHU_index >= -2.147483648E+9) {
          rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
        } else {
          rtu_CHU_index_0 = MIN_int32_T;
        }
      } else {
        rtu_CHU_index_0 = MAX_int32_T;
      }

      rtu_CHU_index_1 = StartChg_timeout[rtu_CHU_index_0] + 1;
      if (rtu_CHU_index_1 > 255) {
        rtu_CHU_index_1 = 255;
      }

      if (rtu_CHU_index < 2.147483648E+9) {
        if (rtu_CHU_index >= -2.147483648E+9) {
          rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
        } else {
          rtu_CHU_index_0 = MIN_int32_T;
        }
      } else {
        rtu_CHU_index_0 = MAX_int32_T;
      }

      StartChg_timeout[rtu_CHU_index_0] = (uint8_T)rtu_CHU_index_1;
      localDW->is_chg_main_state = IN_SetPwr;
      if (rtu_CHU_index < 2.147483648E+9) {
        if (rtu_CHU_index >= -2.147483648E+9) {
          rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
        } else {
          rtu_CHU_index_0 = MIN_int32_T;
        }
      } else {
        rtu_CHU_index_0 = MAX_int32_T;
      }

      EMS_Chger_BigSta[rtu_CHU_index_0] = 3U;
      if (rtu_CHU_index < 2.147483648E+9) {
        if (rtu_CHU_index >= -2.147483648E+9) {
          rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
        } else {
          rtu_CHU_index_0 = MIN_int32_T;
        }
      } else {
        rtu_CHU_index_0 = MAX_int32_T;
      }

      chu_test_state[rtu_CHU_index_0] = 12U;
      if (rtu_CHU_index < 2.147483648E+9) {
        if (rtu_CHU_index >= -2.147483648E+9) {
          rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
        } else {
          rtu_CHU_index_0 = MIN_int32_T;
        }
      } else {
        rtu_CHU_index_0 = MAX_int32_T;
      }

      EMS_SendSetPwrCmd_flg[rtu_CHU_index_0] = 1U;
    } else {
      if (rtu_CHU_index < 2.147483648E+9) {
        if (rtu_CHU_index >= -2.147483648E+9) {
          rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
        } else {
          rtu_CHU_index_0 = MIN_int32_T;
        }
      } else {
        rtu_CHU_index_0 = MAX_int32_T;
      }

      if (StartChg_timeout[rtu_CHU_index_0] >= 6) {
        if (rtu_CHU_index < 2.147483648E+9) {
          if (rtu_CHU_index >= -2.147483648E+9) {
            rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
          } else {
            rtu_CHU_index_0 = MIN_int32_T;
          }
        } else {
          rtu_CHU_index_0 = MAX_int32_T;
        }

        StartChg_timeout[rtu_CHU_index_0] = 0U;
        if (rtu_CHU_index < 2.147483648E+9) {
          if (rtu_CHU_index >= -2.147483648E+9) {
            rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
          } else {
            rtu_CHU_index_0 = MIN_int32_T;
          }
        } else {
          rtu_CHU_index_0 = MAX_int32_T;
        }

        StartChg_count[rtu_CHU_index_0] = 0U;
        if (rtu_CHU_index < 2.147483648E+9) {
          if (rtu_CHU_index >= -2.147483648E+9) {
            rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
          } else {
            rtu_CHU_index_0 = MIN_int32_T;
          }
        } else {
          rtu_CHU_index_0 = MAX_int32_T;
        }

        rtu_CHU_index_1 = StartChg_timeout[rtu_CHU_index_0] + 1;
        if (rtu_CHU_index_1 > 255) {
          rtu_CHU_index_1 = 255;
        }

        if (rtu_CHU_index < 2.147483648E+9) {
          if (rtu_CHU_index >= -2.147483648E+9) {
            rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
          } else {
            rtu_CHU_index_0 = MIN_int32_T;
          }
        } else {
          rtu_CHU_index_0 = MAX_int32_T;
        }

        StartChg_timeout[rtu_CHU_index_0] = (uint8_T)rtu_CHU_index_1;
        localDW->is_chg_main_state = IN_CHU_StartChg_timeout;
        if (rtu_CHU_index < 2.147483648E+9) {
          if (rtu_CHU_index >= -2.147483648E+9) {
            rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
          } else {
            rtu_CHU_index_0 = MIN_int32_T;
          }
        } else {
          rtu_CHU_index_0 = MAX_int32_T;
        }

        chu_test_state[rtu_CHU_index_0] = 205U;
        if (rtu_CHU_index < 2.147483648E+9) {
          if (rtu_CHU_index >= -2.147483648E+9) {
            rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
          } else {
            rtu_CHU_index_0 = MIN_int32_T;
          }
        } else {
          rtu_CHU_index_0 = MAX_int32_T;
        }

        CHU_StartChgErr_flg[rtu_CHU_index_0] = 1U;
      } else {
        if (rtu_CHU_index < 2.147483648E+9) {
          if (rtu_CHU_index >= -2.147483648E+9) {
            rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
          } else {
            rtu_CHU_index_0 = MIN_int32_T;
          }
        } else {
          rtu_CHU_index_0 = MAX_int32_T;
        }

        if (StartChg_count[rtu_CHU_index_0] >= 1500) {
          if (rtu_CHU_index < 2.147483648E+9) {
            if (rtu_CHU_index >= -2.147483648E+9) {
              rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
            } else {
              rtu_CHU_index_0 = MIN_int32_T;
            }
          } else {
            rtu_CHU_index_0 = MAX_int32_T;
          }

          StartChg_count[rtu_CHU_index_0] = 0U;
          if (rtu_CHU_index < 2.147483648E+9) {
            if (rtu_CHU_index >= -2.147483648E+9) {
              rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
            } else {
              rtu_CHU_index_0 = MIN_int32_T;
            }
          } else {
            rtu_CHU_index_0 = MAX_int32_T;
          }

          rtu_CHU_index_1 = StartChg_timeout[rtu_CHU_index_0] + 1;
          if (rtu_CHU_index_1 > 255) {
            rtu_CHU_index_1 = 255;
          }

          if (rtu_CHU_index < 2.147483648E+9) {
            if (rtu_CHU_index >= -2.147483648E+9) {
              rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
            } else {
              rtu_CHU_index_0 = MIN_int32_T;
            }
          } else {
            rtu_CHU_index_0 = MAX_int32_T;
          }

          StartChg_timeout[rtu_CHU_index_0] = (uint8_T)rtu_CHU_index_1;
          localDW->is_chg_main_state = IN_StartChg;
          if (rtu_CHU_index < 2.147483648E+9) {
            if (rtu_CHU_index >= -2.147483648E+9) {
              rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
            } else {
              rtu_CHU_index_0 = MIN_int32_T;
            }
          } else {
            rtu_CHU_index_0 = MAX_int32_T;
          }

          EMS_Chger_BigSta[rtu_CHU_index_0] = 2U;
          if (rtu_CHU_index < 2.147483648E+9) {
            if (rtu_CHU_index >= -2.147483648E+9) {
              rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
            } else {
              rtu_CHU_index_0 = MIN_int32_T;
            }
          } else {
            rtu_CHU_index_0 = MAX_int32_T;
          }

          chu_test_state[rtu_CHU_index_0] = 11U;
          if (rtu_CHU_index < 2.147483648E+9) {
            if (rtu_CHU_index >= -2.147483648E+9) {
              rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
            } else {
              rtu_CHU_index_0 = MIN_int32_T;
            }
          } else {
            rtu_CHU_index_0 = MAX_int32_T;
          }

          EMS_SendStartChgCmd_flg[rtu_CHU_index_0] = 1U;
        } else {
          if (rtu_CHU_index < 2.147483648E+9) {
            if (rtu_CHU_index >= -2.147483648E+9) {
              rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
            } else {
              rtu_CHU_index_0 = MIN_int32_T;
            }
          } else {
            rtu_CHU_index_0 = MAX_int32_T;
          }

          if (CHU_AllErr_flg[rtu_CHU_index_0] != 0) {
            if (rtu_CHU_index < 2.147483648E+9) {
              if (rtu_CHU_index >= -2.147483648E+9) {
                rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
              } else {
                rtu_CHU_index_0 = MIN_int32_T;
              }
            } else {
              rtu_CHU_index_0 = MAX_int32_T;
            }

            StartChg_timeout[rtu_CHU_index_0] = 0U;
            if (rtu_CHU_index < 2.147483648E+9) {
              if (rtu_CHU_index >= -2.147483648E+9) {
                rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
              } else {
                rtu_CHU_index_0 = MIN_int32_T;
              }
            } else {
              rtu_CHU_index_0 = MAX_int32_T;
            }

            EMS_SendStartChgCmd_flg[rtu_CHU_index_0] = 0U;
            if (rtu_CHU_index < 2.147483648E+9) {
              if (rtu_CHU_index >= -2.147483648E+9) {
                rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
              } else {
                rtu_CHU_index_0 = MIN_int32_T;
              }
            } else {
              rtu_CHU_index_0 = MAX_int32_T;
            }

            StartChg_count[rtu_CHU_index_0] = 0U;
            if (rtu_CHU_index < 2.147483648E+9) {
              if (rtu_CHU_index >= -2.147483648E+9) {
                rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
              } else {
                rtu_CHU_index_0 = MIN_int32_T;
              }
            } else {
              rtu_CHU_index_0 = MAX_int32_T;
            }

            rtu_CHU_index_1 = StartChg_timeout[rtu_CHU_index_0] + 1;
            if (rtu_CHU_index_1 > 255) {
              rtu_CHU_index_1 = 255;
            }

            if (rtu_CHU_index < 2.147483648E+9) {
              if (rtu_CHU_index >= -2.147483648E+9) {
                rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
              } else {
                rtu_CHU_index_0 = MIN_int32_T;
              }
            } else {
              rtu_CHU_index_0 = MAX_int32_T;
            }

            StartChg_timeout[rtu_CHU_index_0] = (uint8_T)rtu_CHU_index_1;
            localDW->is_chg_main_state = IN_StartChg_Fault;
            if (rtu_CHU_index < 2.147483648E+9) {
              if (rtu_CHU_index >= -2.147483648E+9) {
                rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
              } else {
                rtu_CHU_index_0 = MIN_int32_T;
              }
            } else {
              rtu_CHU_index_0 = MAX_int32_T;
            }

            chu_test_state[rtu_CHU_index_0] = 206U;
          } else {
            /* EMS_SendStartChgCmd_flg[CHU_index] = 0; */
            if (rtu_CHU_index < 2.147483648E+9) {
              if (rtu_CHU_index >= -2.147483648E+9) {
                rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
              } else {
                rtu_CHU_index_0 = MIN_int32_T;
              }
            } else {
              rtu_CHU_index_0 = MAX_int32_T;
            }

            rtu_CHU_index_1 = StartChg_count[rtu_CHU_index_0] + 1;
            if (rtu_CHU_index_1 > 65535) {
              rtu_CHU_index_1 = 65535;
            }

            if (rtu_CHU_index < 2.147483648E+9) {
              if (rtu_CHU_index >= -2.147483648E+9) {
                rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
              } else {
                rtu_CHU_index_0 = MIN_int32_T;
              }
            } else {
              rtu_CHU_index_0 = MAX_int32_T;
            }

            StartChg_count[rtu_CHU_index_0] = (uint16_T)rtu_CHU_index_1;
          }
        }
      }
    }
    break;

   case IN_StartChg_Fault:
    localDW->is_chg_main_state = IN_stop_ok3;
    if (rtu_CHU_index < 2.147483648E+9) {
      if (rtu_CHU_index >= -2.147483648E+9) {
        rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
      } else {
        rtu_CHU_index_0 = MIN_int32_T;
      }
    } else {
      rtu_CHU_index_0 = MAX_int32_T;
    }

    EMS_Chger_BigSta[rtu_CHU_index_0] = 5U;
    if (rtu_CHU_index < 2.147483648E+9) {
      if (rtu_CHU_index >= -2.147483648E+9) {
        rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
      } else {
        rtu_CHU_index_0 = MIN_int32_T;
      }
    } else {
      rtu_CHU_index_0 = MAX_int32_T;
    }

    chu_test_state[rtu_CHU_index_0] = 14U;
    if (rtu_CHU_index < 2.147483648E+9) {
      if (rtu_CHU_index >= -2.147483648E+9) {
        rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
      } else {
        rtu_CHU_index_0 = MIN_int32_T;
      }
    } else {
      rtu_CHU_index_0 = MAX_int32_T;
    }

    Hand_LowPwrDown_flg[rtu_CHU_index_0] = 0U;
    break;

   case IN_StopChg:
    if (rtu_CHU_index < 2.147483648E+9) {
      if (rtu_CHU_index >= -2.147483648E+9) {
        rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
      } else {
        rtu_CHU_index_0 = MIN_int32_T;
      }
    } else {
      rtu_CHU_index_0 = MAX_int32_T;
    }

    if (StopChg_timeout[rtu_CHU_index_0] >= 5) {
      if (rtu_CHU_index < 2.147483648E+9) {
        if (rtu_CHU_index >= -2.147483648E+9) {
          rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
        } else {
          rtu_CHU_index_0 = MIN_int32_T;
        }
      } else {
        rtu_CHU_index_0 = MAX_int32_T;
      }

      StopChg_timeout[rtu_CHU_index_0] = 0U;
      if (rtu_CHU_index < 2.147483648E+9) {
        if (rtu_CHU_index >= -2.147483648E+9) {
          rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
        } else {
          rtu_CHU_index_0 = MIN_int32_T;
        }
      } else {
        rtu_CHU_index_0 = MAX_int32_T;
      }

      StopChg_count[rtu_CHU_index_0] = 0U;
      if (rtu_CHU_index < 2.147483648E+9) {
        if (rtu_CHU_index >= -2.147483648E+9) {
          rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
        } else {
          rtu_CHU_index_0 = MIN_int32_T;
        }
      } else {
        rtu_CHU_index_0 = MAX_int32_T;
      }

      rtu_CHU_index_1 = StopChg_timeout[rtu_CHU_index_0] + 1;
      if (rtu_CHU_index_1 > 255) {
        rtu_CHU_index_1 = 255;
      }

      if (rtu_CHU_index < 2.147483648E+9) {
        if (rtu_CHU_index >= -2.147483648E+9) {
          rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
        } else {
          rtu_CHU_index_0 = MIN_int32_T;
        }
      } else {
        rtu_CHU_index_0 = MAX_int32_T;
      }

      StopChg_timeout[rtu_CHU_index_0] = (uint8_T)rtu_CHU_index_1;
      localDW->is_chg_main_state = IN_CHU_StopChg_timeout;
      if (rtu_CHU_index < 2.147483648E+9) {
        if (rtu_CHU_index >= -2.147483648E+9) {
          rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
        } else {
          rtu_CHU_index_0 = MIN_int32_T;
        }
      } else {
        rtu_CHU_index_0 = MAX_int32_T;
      }

      chu_test_state[rtu_CHU_index_0] = 207U;
      if (rtu_CHU_index < 2.147483648E+9) {
        if (rtu_CHU_index >= -2.147483648E+9) {
          rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
        } else {
          rtu_CHU_index_0 = MIN_int32_T;
        }
      } else {
        rtu_CHU_index_0 = MAX_int32_T;
      }

      CHU_StopChgErr_flg[rtu_CHU_index_0] = 1U;
    } else {
      if (rtu_CHU_index < 2.147483648E+9) {
        if (rtu_CHU_index >= -2.147483648E+9) {
          rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
        } else {
          rtu_CHU_index_0 = MIN_int32_T;
        }
      } else {
        rtu_CHU_index_0 = MAX_int32_T;
      }

      if (StopChg_count[rtu_CHU_index_0] >= 200) {
        if (rtu_CHU_index < 2.147483648E+9) {
          if (rtu_CHU_index >= -2.147483648E+9) {
            rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
          } else {
            rtu_CHU_index_0 = MIN_int32_T;
          }
        } else {
          rtu_CHU_index_0 = MAX_int32_T;
        }

        StopChg_count[rtu_CHU_index_0] = 0U;
        if (rtu_CHU_index < 2.147483648E+9) {
          if (rtu_CHU_index >= -2.147483648E+9) {
            rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
          } else {
            rtu_CHU_index_0 = MIN_int32_T;
          }
        } else {
          rtu_CHU_index_0 = MAX_int32_T;
        }

        rtu_CHU_index_1 = StopChg_timeout[rtu_CHU_index_0] + 1;
        if (rtu_CHU_index_1 > 255) {
          rtu_CHU_index_1 = 255;
        }

        if (rtu_CHU_index < 2.147483648E+9) {
          if (rtu_CHU_index >= -2.147483648E+9) {
            rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
          } else {
            rtu_CHU_index_0 = MIN_int32_T;
          }
        } else {
          rtu_CHU_index_0 = MAX_int32_T;
        }

        StopChg_timeout[rtu_CHU_index_0] = (uint8_T)rtu_CHU_index_1;
        localDW->is_chg_main_state = IN_StopChg;
        if (rtu_CHU_index < 2.147483648E+9) {
          if (rtu_CHU_index >= -2.147483648E+9) {
            rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
          } else {
            rtu_CHU_index_0 = MIN_int32_T;
          }
        } else {
          rtu_CHU_index_0 = MAX_int32_T;
        }

        EMS_Chger_BigSta[rtu_CHU_index_0] = 4U;
        if (rtu_CHU_index < 2.147483648E+9) {
          if (rtu_CHU_index >= -2.147483648E+9) {
            rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
          } else {
            rtu_CHU_index_0 = MIN_int32_T;
          }
        } else {
          rtu_CHU_index_0 = MAX_int32_T;
        }

        chu_test_state[rtu_CHU_index_0] = 13U;
        if (rtu_CHU_index < 2.147483648E+9) {
          if (rtu_CHU_index >= -2.147483648E+9) {
            rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
          } else {
            rtu_CHU_index_0 = MIN_int32_T;
          }
        } else {
          rtu_CHU_index_0 = MAX_int32_T;
        }

        EMS_SendStopChgCmd_flg[rtu_CHU_index_0] = 1U;
      } else {
        if (rtu_CHU_index < 2.147483648E+9) {
          if (rtu_CHU_index >= -2.147483648E+9) {
            rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
            rtu_CHU_index_1 = (int32_T)rtu_CHU_index;
          } else {
            rtu_CHU_index_0 = MIN_int32_T;
            rtu_CHU_index_1 = MIN_int32_T;
          }
        } else {
          rtu_CHU_index_0 = MAX_int32_T;
          rtu_CHU_index_1 = MAX_int32_T;
        }

        if ((CHU_DevState_code[rtu_CHU_index_0] == 0x02) ||
            (CHU_DevState_code[rtu_CHU_index_1] == 0x03)) {
          if (rtu_CHU_index < 2.147483648E+9) {
            if (rtu_CHU_index >= -2.147483648E+9) {
              rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
            } else {
              rtu_CHU_index_0 = MIN_int32_T;
            }
          } else {
            rtu_CHU_index_0 = MAX_int32_T;
          }

          StopChg_timeout[rtu_CHU_index_0] = 0U;
          if (rtu_CHU_index < 2.147483648E+9) {
            if (rtu_CHU_index >= -2.147483648E+9) {
              rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
            } else {
              rtu_CHU_index_0 = MIN_int32_T;
            }
          } else {
            rtu_CHU_index_0 = MAX_int32_T;
          }

          StopChg_count[rtu_CHU_index_0] = 0U;
          if (rtu_CHU_index < 2.147483648E+9) {
            if (rtu_CHU_index >= -2.147483648E+9) {
              rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
            } else {
              rtu_CHU_index_0 = MIN_int32_T;
            }
          } else {
            rtu_CHU_index_0 = MAX_int32_T;
          }

          rtu_CHU_index_1 = StopChg_timeout[rtu_CHU_index_0] + 1;
          if (rtu_CHU_index_1 > 255) {
            rtu_CHU_index_1 = 255;
          }

          if (rtu_CHU_index < 2.147483648E+9) {
            if (rtu_CHU_index >= -2.147483648E+9) {
              rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
            } else {
              rtu_CHU_index_0 = MIN_int32_T;
            }
          } else {
            rtu_CHU_index_0 = MAX_int32_T;
          }

          StopChg_timeout[rtu_CHU_index_0] = (uint8_T)rtu_CHU_index_1;
          localDW->is_chg_main_state = IN_stop_ok3;
          if (rtu_CHU_index < 2.147483648E+9) {
            if (rtu_CHU_index >= -2.147483648E+9) {
              rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
            } else {
              rtu_CHU_index_0 = MIN_int32_T;
            }
          } else {
            rtu_CHU_index_0 = MAX_int32_T;
          }

          EMS_Chger_BigSta[rtu_CHU_index_0] = 5U;
          if (rtu_CHU_index < 2.147483648E+9) {
            if (rtu_CHU_index >= -2.147483648E+9) {
              rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
            } else {
              rtu_CHU_index_0 = MIN_int32_T;
            }
          } else {
            rtu_CHU_index_0 = MAX_int32_T;
          }

          chu_test_state[rtu_CHU_index_0] = 14U;
          if (rtu_CHU_index < 2.147483648E+9) {
            if (rtu_CHU_index >= -2.147483648E+9) {
              rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
            } else {
              rtu_CHU_index_0 = MIN_int32_T;
            }
          } else {
            rtu_CHU_index_0 = MAX_int32_T;
          }

          Hand_LowPwrDown_flg[rtu_CHU_index_0] = 0U;
        } else {
          /* EMS_SendStopChgCmd_flg[CHU_index] = 0 */
          if (rtu_CHU_index < 2.147483648E+9) {
            if (rtu_CHU_index >= -2.147483648E+9) {
              rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
            } else {
              rtu_CHU_index_0 = MIN_int32_T;
            }
          } else {
            rtu_CHU_index_0 = MAX_int32_T;
          }

          rtu_CHU_index_1 = StopChg_count[rtu_CHU_index_0] + 1;
          if (rtu_CHU_index_1 > 65535) {
            rtu_CHU_index_1 = 65535;
          }

          if (rtu_CHU_index < 2.147483648E+9) {
            if (rtu_CHU_index >= -2.147483648E+9) {
              rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
            } else {
              rtu_CHU_index_0 = MIN_int32_T;
            }
          } else {
            rtu_CHU_index_0 = MAX_int32_T;
          }

          StopChg_count[rtu_CHU_index_0] = (uint16_T)rtu_CHU_index_1;
        }
      }
    }
    break;

   case IN_free:
    if (rtu_CHU_index < 2.147483648E+9) {
      if (rtu_CHU_index >= -2.147483648E+9) {
        rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
        rtu_CHU_index_1 = (int32_T)rtu_CHU_index;
        rtu_CHU_index_2 = (int32_T)rtu_CHU_index;
      } else {
        rtu_CHU_index_0 = MIN_int32_T;
        rtu_CHU_index_1 = MIN_int32_T;
        rtu_CHU_index_2 = MIN_int32_T;
      }
    } else {
      rtu_CHU_index_0 = MAX_int32_T;
      rtu_CHU_index_1 = MAX_int32_T;
      rtu_CHU_index_2 = MAX_int32_T;
    }

    if ((Chg_Control_mode[rtu_CHU_index_0] == 1) ||
        ((Chg_Control_mode[rtu_CHU_index_1] == 0) &&
         (Hand_StartChg_flg[rtu_CHU_index_2] == 1))) {
      if (rtu_CHU_index < 2.147483648E+9) {
        if (rtu_CHU_index >= -2.147483648E+9) {
          rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
        } else {
          rtu_CHU_index_0 = MIN_int32_T;
        }
      } else {
        rtu_CHU_index_0 = MAX_int32_T;
      }

      Hand_StartChg_flg[rtu_CHU_index_0] = 0U;
      localDW->is_chg_main_state = IN_StartChg;
      if (rtu_CHU_index < 2.147483648E+9) {
        if (rtu_CHU_index >= -2.147483648E+9) {
          rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
        } else {
          rtu_CHU_index_0 = MIN_int32_T;
        }
      } else {
        rtu_CHU_index_0 = MAX_int32_T;
      }

      EMS_Chger_BigSta[rtu_CHU_index_0] = 2U;
      if (rtu_CHU_index < 2.147483648E+9) {
        if (rtu_CHU_index >= -2.147483648E+9) {
          rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
        } else {
          rtu_CHU_index_0 = MIN_int32_T;
        }
      } else {
        rtu_CHU_index_0 = MAX_int32_T;
      }

      chu_test_state[rtu_CHU_index_0] = 11U;
      if (rtu_CHU_index < 2.147483648E+9) {
        if (rtu_CHU_index >= -2.147483648E+9) {
          rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
        } else {
          rtu_CHU_index_0 = MIN_int32_T;
        }
      } else {
        rtu_CHU_index_0 = MAX_int32_T;
      }

      EMS_SendStartChgCmd_flg[rtu_CHU_index_0] = 1U;
    }
    break;

   case IN_free1:
    if (localDW->Chg_wait_cou >= 100.0) {
      localDW->is_chg_main_state = IN_free;
      localDW->Chg_wait_cou = 0.0;
      if (rtu_CHU_index < 2.147483648E+9) {
        if (rtu_CHU_index >= -2.147483648E+9) {
          rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
        } else {
          rtu_CHU_index_0 = MIN_int32_T;
        }
      } else {
        rtu_CHU_index_0 = MAX_int32_T;
      }

      chu_test_state[rtu_CHU_index_0] = 10U;
    } else {
      localDW->Chg_wait_cou++;
    }
    break;
  }

  if (guard1) {
    /* 判断CHU，EMS故障
       CharEnd_Deal(); */
    if (rtu_CHU_index < 2.147483648E+9) {
      if (rtu_CHU_index >= -2.147483648E+9) {
        rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
      } else {
        rtu_CHU_index_0 = MIN_int32_T;
      }
    } else {
      rtu_CHU_index_0 = MAX_int32_T;
    }

    SetPwr_count[rtu_CHU_index_0] = 0U;
    CharEnd_Deal(rtu_CHU_index);
    localDW->is_chg_main_state = IN_StopChg;
    if (rtu_CHU_index < 2.147483648E+9) {
      if (rtu_CHU_index >= -2.147483648E+9) {
        rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
      } else {
        rtu_CHU_index_0 = MIN_int32_T;
      }
    } else {
      rtu_CHU_index_0 = MAX_int32_T;
    }

    EMS_Chger_BigSta[rtu_CHU_index_0] = 4U;
    if (rtu_CHU_index < 2.147483648E+9) {
      if (rtu_CHU_index >= -2.147483648E+9) {
        rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
      } else {
        rtu_CHU_index_0 = MIN_int32_T;
      }
    } else {
      rtu_CHU_index_0 = MAX_int32_T;
    }

    chu_test_state[rtu_CHU_index_0] = 13U;
    if (rtu_CHU_index < 2.147483648E+9) {
      if (rtu_CHU_index >= -2.147483648E+9) {
        rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
      } else {
        rtu_CHU_index_0 = MIN_int32_T;
      }
    } else {
      rtu_CHU_index_0 = MAX_int32_T;
    }

    EMS_SendStopChgCmd_flg[rtu_CHU_index_0] = 1U;
  }
}

/* Function for Chart: '<S3>/Chart1' */
void CHU_deal(real_T rtu_CHU_index, DW_Chart1 *localDW)
{
  int32_T rtu_CHU_index_0;
  int32_T rtu_CHU_index_1;
  int32_T rtu_CHU_index_2;
  int32_T rtu_CHU_index_3;
  int32_T rtu_CHU_index_4;
  int32_T rtu_CHU_index_5;
  int32_T rtu_CHU_index_6;
  boolean_T guard1 = false;
  MLocatErr_Deal(rtu_CHU_index);
  guard1 = false;
  switch (localDW->is_CHU_deal) {
   case IN_CHU_LowPwrDown_timeout:
    localDW->is_CHU_deal = IN_LowPwrDown_ok;
    break;

   case IN_CHU_LowPwrOn_timeout:
    localDW->is_CHU_deal = IN_LowPwrDown_ok;
    break;

   case IN_Init:
    if (rtu_CHU_index < 2.147483648E+9) {
      if (rtu_CHU_index >= -2.147483648E+9) {
        rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
        rtu_CHU_index_1 = (int32_T)rtu_CHU_index;
        rtu_CHU_index_2 = (int32_T)rtu_CHU_index;
        rtu_CHU_index_3 = (int32_T)rtu_CHU_index;
        rtu_CHU_index_4 = (int32_T)rtu_CHU_index;
        rtu_CHU_index_5 = (int32_T)rtu_CHU_index;
        rtu_CHU_index_6 = (int32_T)rtu_CHU_index;
      } else {
        rtu_CHU_index_0 = MIN_int32_T;
        rtu_CHU_index_1 = MIN_int32_T;
        rtu_CHU_index_2 = MIN_int32_T;
        rtu_CHU_index_3 = MIN_int32_T;
        rtu_CHU_index_4 = MIN_int32_T;
        rtu_CHU_index_5 = MIN_int32_T;
        rtu_CHU_index_6 = MIN_int32_T;
      }
    } else {
      rtu_CHU_index_0 = MAX_int32_T;
      rtu_CHU_index_1 = MAX_int32_T;
      rtu_CHU_index_2 = MAX_int32_T;
      rtu_CHU_index_3 = MAX_int32_T;
      rtu_CHU_index_4 = MAX_int32_T;
      rtu_CHU_index_5 = MAX_int32_T;
      rtu_CHU_index_6 = MAX_int32_T;
    }

    if (((Chg_Control_mode[rtu_CHU_index_0] == 1) ||
         ((Chg_Control_mode[rtu_CHU_index_1] == 0) &&
          ((Hand_LowPwrOn_flg[rtu_CHU_index_2] == 1) ||
           (Hand_StartChg_flg[rtu_CHU_index_3] == 1)))) &&
        (BATT_IsExist_flg[rtu_CHU_index_4] == 1) &&
        (EMS_CHU_ComErr_flg[rtu_CHU_index_5] == 0) &&
        (MChar_StartCE_flg[rtu_CHU_index_6] == 0) && ((MIO_Water_err == 0) &&
         (FireContr_GetFire_flg == 0))) {
      if (rtu_CHU_index < 2.147483648E+9) {
        if (rtu_CHU_index >= -2.147483648E+9) {
          rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
        } else {
          rtu_CHU_index_0 = MIN_int32_T;
        }
      } else {
        rtu_CHU_index_0 = MAX_int32_T;
      }

      Hand_LowPwrOn_flg[rtu_CHU_index_0] = 0U;
      localDW->is_CHU_deal = IN_LowPwrOn;
      if (rtu_CHU_index < 2.147483648E+9) {
        if (rtu_CHU_index >= -2.147483648E+9) {
          rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
        } else {
          rtu_CHU_index_0 = MIN_int32_T;
        }
      } else {
        rtu_CHU_index_0 = MAX_int32_T;
      }

      EMS_Chger_BigSta[rtu_CHU_index_0] = 1U;
      if (rtu_CHU_index < 2.147483648E+9) {
        if (rtu_CHU_index >= -2.147483648E+9) {
          rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
        } else {
          rtu_CHU_index_0 = MIN_int32_T;
        }
      } else {
        rtu_CHU_index_0 = MAX_int32_T;
      }

      chu_test_state[rtu_CHU_index_0] = 1U;
      if (rtu_CHU_index < 2.147483648E+9) {
        if (rtu_CHU_index >= -2.147483648E+9) {
          rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
        } else {
          rtu_CHU_index_0 = MIN_int32_T;
        }
      } else {
        rtu_CHU_index_0 = MAX_int32_T;
      }

      EMS_SendLowPwrOnCmd_flg[rtu_CHU_index_0] = 1U;
    } else {
      if (rtu_CHU_index < 2.147483648E+9) {
        if (rtu_CHU_index >= -2.147483648E+9) {
          rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
          rtu_CHU_index_1 = (int32_T)rtu_CHU_index;
        } else {
          rtu_CHU_index_0 = MIN_int32_T;
          rtu_CHU_index_1 = MIN_int32_T;
        }
      } else {
        rtu_CHU_index_0 = MAX_int32_T;
        rtu_CHU_index_1 = MAX_int32_T;
      }

      if ((CHU_DevState_code[rtu_CHU_index_0] == 0x02) ||
          (CHU_DevState_code[rtu_CHU_index_1] == 0x03)) {
        localDW->is_CHU_deal = IN_LowPwrDown;
        if (rtu_CHU_index < 2.147483648E+9) {
          if (rtu_CHU_index >= -2.147483648E+9) {
            rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
          } else {
            rtu_CHU_index_0 = MIN_int32_T;
          }
        } else {
          rtu_CHU_index_0 = MAX_int32_T;
        }

        EMS_Chger_BigSta[rtu_CHU_index_0] = 6U;
        if (rtu_CHU_index < 2.147483648E+9) {
          if (rtu_CHU_index >= -2.147483648E+9) {
            rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
          } else {
            rtu_CHU_index_0 = MIN_int32_T;
          }
        } else {
          rtu_CHU_index_0 = MAX_int32_T;
        }

        chu_test_state[rtu_CHU_index_0] = 20U;
        if (rtu_CHU_index < 2.147483648E+9) {
          if (rtu_CHU_index >= -2.147483648E+9) {
            rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
          } else {
            rtu_CHU_index_0 = MIN_int32_T;
          }
        } else {
          rtu_CHU_index_0 = MAX_int32_T;
        }

        EMS_SendLowPwrDownCmd_flg[rtu_CHU_index_0] = 1U;
      }
    }
    break;

   case IN_LowPwrDown:
    if (rtu_CHU_index < 2.147483648E+9) {
      if (rtu_CHU_index >= -2.147483648E+9) {
        rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
      } else {
        rtu_CHU_index_0 = MIN_int32_T;
      }
    } else {
      rtu_CHU_index_0 = MAX_int32_T;
    }

    if (CHU_LowPwrDown_count[rtu_CHU_index_0] >= 300) {
      if (rtu_CHU_index < 2.147483648E+9) {
        if (rtu_CHU_index >= -2.147483648E+9) {
          rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
        } else {
          rtu_CHU_index_0 = MIN_int32_T;
        }
      } else {
        rtu_CHU_index_0 = MAX_int32_T;
      }

      rtu_CHU_index_1 = CHU_LowPwrDown_timeout[rtu_CHU_index_0] + 1;
      if (rtu_CHU_index_1 > 255) {
        rtu_CHU_index_1 = 255;
      }

      if (rtu_CHU_index < 2.147483648E+9) {
        if (rtu_CHU_index >= -2.147483648E+9) {
          rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
        } else {
          rtu_CHU_index_0 = MIN_int32_T;
        }
      } else {
        rtu_CHU_index_0 = MAX_int32_T;
      }

      CHU_LowPwrDown_timeout[rtu_CHU_index_0] = (uint8_T)rtu_CHU_index_1;
      if (rtu_CHU_index < 2.147483648E+9) {
        if (rtu_CHU_index >= -2.147483648E+9) {
          rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
        } else {
          rtu_CHU_index_0 = MIN_int32_T;
        }
      } else {
        rtu_CHU_index_0 = MAX_int32_T;
      }

      CHU_LowPwrDown_count[rtu_CHU_index_0] = 0U;
      localDW->is_CHU_deal = IN_LowPwrDown;
      if (rtu_CHU_index < 2.147483648E+9) {
        if (rtu_CHU_index >= -2.147483648E+9) {
          rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
        } else {
          rtu_CHU_index_0 = MIN_int32_T;
        }
      } else {
        rtu_CHU_index_0 = MAX_int32_T;
      }

      EMS_Chger_BigSta[rtu_CHU_index_0] = 6U;
      if (rtu_CHU_index < 2.147483648E+9) {
        if (rtu_CHU_index >= -2.147483648E+9) {
          rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
        } else {
          rtu_CHU_index_0 = MIN_int32_T;
        }
      } else {
        rtu_CHU_index_0 = MAX_int32_T;
      }

      chu_test_state[rtu_CHU_index_0] = 20U;
      if (rtu_CHU_index < 2.147483648E+9) {
        if (rtu_CHU_index >= -2.147483648E+9) {
          rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
        } else {
          rtu_CHU_index_0 = MIN_int32_T;
        }
      } else {
        rtu_CHU_index_0 = MAX_int32_T;
      }

      EMS_SendLowPwrDownCmd_flg[rtu_CHU_index_0] = 1U;
    } else {
      if (rtu_CHU_index < 2.147483648E+9) {
        if (rtu_CHU_index >= -2.147483648E+9) {
          rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
        } else {
          rtu_CHU_index_0 = MIN_int32_T;
        }
      } else {
        rtu_CHU_index_0 = MAX_int32_T;
      }

      if (CHU_LowPwrDown_timeout[rtu_CHU_index_0] >= 5) {
        if (rtu_CHU_index < 2.147483648E+9) {
          if (rtu_CHU_index >= -2.147483648E+9) {
            rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
          } else {
            rtu_CHU_index_0 = MIN_int32_T;
          }
        } else {
          rtu_CHU_index_0 = MAX_int32_T;
        }

        CHU_LowPwrDown_timeout[rtu_CHU_index_0] = 0U;
        if (rtu_CHU_index < 2.147483648E+9) {
          if (rtu_CHU_index >= -2.147483648E+9) {
            rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
          } else {
            rtu_CHU_index_0 = MIN_int32_T;
          }
        } else {
          rtu_CHU_index_0 = MAX_int32_T;
        }

        rtu_CHU_index_1 = CHU_LowPwrDown_timeout[rtu_CHU_index_0] + 1;
        if (rtu_CHU_index_1 > 255) {
          rtu_CHU_index_1 = 255;
        }

        if (rtu_CHU_index < 2.147483648E+9) {
          if (rtu_CHU_index >= -2.147483648E+9) {
            rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
          } else {
            rtu_CHU_index_0 = MIN_int32_T;
          }
        } else {
          rtu_CHU_index_0 = MAX_int32_T;
        }

        CHU_LowPwrDown_timeout[rtu_CHU_index_0] = (uint8_T)rtu_CHU_index_1;
        if (rtu_CHU_index < 2.147483648E+9) {
          if (rtu_CHU_index >= -2.147483648E+9) {
            rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
          } else {
            rtu_CHU_index_0 = MIN_int32_T;
          }
        } else {
          rtu_CHU_index_0 = MAX_int32_T;
        }

        CHU_LowPwrDown_count[rtu_CHU_index_0] = 0U;
        localDW->is_CHU_deal = IN_CHU_LowPwrDown_timeout;
        if (rtu_CHU_index < 2.147483648E+9) {
          if (rtu_CHU_index >= -2.147483648E+9) {
            rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
          } else {
            rtu_CHU_index_0 = MIN_int32_T;
          }
        } else {
          rtu_CHU_index_0 = MAX_int32_T;
        }

        chu_test_state[rtu_CHU_index_0] = 101U;
        if (rtu_CHU_index < 2.147483648E+9) {
          if (rtu_CHU_index >= -2.147483648E+9) {
            rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
          } else {
            rtu_CHU_index_0 = MIN_int32_T;
          }
        } else {
          rtu_CHU_index_0 = MAX_int32_T;
        }

        CHU_LowPwrDownErr_flg[rtu_CHU_index_0] = 1U;
      } else {
        if (rtu_CHU_index < 2.147483648E+9) {
          if (rtu_CHU_index >= -2.147483648E+9) {
            rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
          } else {
            rtu_CHU_index_0 = MIN_int32_T;
          }
        } else {
          rtu_CHU_index_0 = MAX_int32_T;
        }

        if (CHU_DevState_code[rtu_CHU_index_0] == 0x01) {
          if (rtu_CHU_index < 2.147483648E+9) {
            if (rtu_CHU_index >= -2.147483648E+9) {
              rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
            } else {
              rtu_CHU_index_0 = MIN_int32_T;
            }
          } else {
            rtu_CHU_index_0 = MAX_int32_T;
          }

          CHU_LowPwrDown_timeout[rtu_CHU_index_0] = 0U;
          if (rtu_CHU_index < 2.147483648E+9) {
            if (rtu_CHU_index >= -2.147483648E+9) {
              rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
            } else {
              rtu_CHU_index_0 = MIN_int32_T;
            }
          } else {
            rtu_CHU_index_0 = MAX_int32_T;
          }

          rtu_CHU_index_1 = CHU_LowPwrDown_timeout[rtu_CHU_index_0] + 1;
          if (rtu_CHU_index_1 > 255) {
            rtu_CHU_index_1 = 255;
          }

          if (rtu_CHU_index < 2.147483648E+9) {
            if (rtu_CHU_index >= -2.147483648E+9) {
              rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
            } else {
              rtu_CHU_index_0 = MIN_int32_T;
            }
          } else {
            rtu_CHU_index_0 = MAX_int32_T;
          }

          CHU_LowPwrDown_timeout[rtu_CHU_index_0] = (uint8_T)rtu_CHU_index_1;
          if (rtu_CHU_index < 2.147483648E+9) {
            if (rtu_CHU_index >= -2.147483648E+9) {
              rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
            } else {
              rtu_CHU_index_0 = MIN_int32_T;
            }
          } else {
            rtu_CHU_index_0 = MAX_int32_T;
          }

          CHU_LowPwrDown_count[rtu_CHU_index_0] = 0U;
          localDW->is_CHU_deal = IN_LowPwrDown_ok;
        } else {
          /* EMS_SendLowPwrDownCmd_flg[CHU_index] = 0; */
          if (rtu_CHU_index < 2.147483648E+9) {
            if (rtu_CHU_index >= -2.147483648E+9) {
              rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
            } else {
              rtu_CHU_index_0 = MIN_int32_T;
            }
          } else {
            rtu_CHU_index_0 = MAX_int32_T;
          }

          rtu_CHU_index_1 = CHU_LowPwrDown_count[rtu_CHU_index_0] + 1;
          if (rtu_CHU_index_1 > 65535) {
            rtu_CHU_index_1 = 65535;
          }

          if (rtu_CHU_index < 2.147483648E+9) {
            if (rtu_CHU_index >= -2.147483648E+9) {
              rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
            } else {
              rtu_CHU_index_0 = MIN_int32_T;
            }
          } else {
            rtu_CHU_index_0 = MAX_int32_T;
          }

          CHU_LowPwrDown_count[rtu_CHU_index_0] = (uint16_T)rtu_CHU_index_1;
        }
      }
    }
    break;

   case IN_LowPwrDown_ok:
    init_clear(rtu_CHU_index);
    localDW->is_CHU_deal = IN_Init;
    if (rtu_CHU_index < 2.147483648E+9) {
      if (rtu_CHU_index >= -2.147483648E+9) {
        rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
      } else {
        rtu_CHU_index_0 = MIN_int32_T;
      }
    } else {
      rtu_CHU_index_0 = MAX_int32_T;
    }

    EMS_Chger_BigSta[rtu_CHU_index_0] = 0U;
    if (rtu_CHU_index < 2.147483648E+9) {
      if (rtu_CHU_index >= -2.147483648E+9) {
        rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
      } else {
        rtu_CHU_index_0 = MIN_int32_T;
      }
    } else {
      rtu_CHU_index_0 = MAX_int32_T;
    }

    chu_test_state[rtu_CHU_index_0] = 0U;

    /* init_clear(); */
    break;

   case IN_LowPwrOn:
    if (rtu_CHU_index < 2.147483648E+9) {
      if (rtu_CHU_index >= -2.147483648E+9) {
        rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
      } else {
        rtu_CHU_index_0 = MIN_int32_T;
      }
    } else {
      rtu_CHU_index_0 = MAX_int32_T;
    }

    if (CHU_LowPwrOn_count[rtu_CHU_index_0] >= 400) {
      if (rtu_CHU_index < 2.147483648E+9) {
        if (rtu_CHU_index >= -2.147483648E+9) {
          rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
        } else {
          rtu_CHU_index_0 = MIN_int32_T;
        }
      } else {
        rtu_CHU_index_0 = MAX_int32_T;
      }

      rtu_CHU_index_1 = CHU_LowPwrOn_timeout[rtu_CHU_index_0] + 1;
      if (rtu_CHU_index_1 > 255) {
        rtu_CHU_index_1 = 255;
      }

      if (rtu_CHU_index < 2.147483648E+9) {
        if (rtu_CHU_index >= -2.147483648E+9) {
          rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
        } else {
          rtu_CHU_index_0 = MIN_int32_T;
        }
      } else {
        rtu_CHU_index_0 = MAX_int32_T;
      }

      CHU_LowPwrOn_timeout[rtu_CHU_index_0] = (uint8_T)rtu_CHU_index_1;
      if (rtu_CHU_index < 2.147483648E+9) {
        if (rtu_CHU_index >= -2.147483648E+9) {
          rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
        } else {
          rtu_CHU_index_0 = MIN_int32_T;
        }
      } else {
        rtu_CHU_index_0 = MAX_int32_T;
      }

      CHU_LowPwrOn_count[rtu_CHU_index_0] = 0U;
      localDW->is_CHU_deal = IN_LowPwrOn;
      if (rtu_CHU_index < 2.147483648E+9) {
        if (rtu_CHU_index >= -2.147483648E+9) {
          rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
        } else {
          rtu_CHU_index_0 = MIN_int32_T;
        }
      } else {
        rtu_CHU_index_0 = MAX_int32_T;
      }

      EMS_Chger_BigSta[rtu_CHU_index_0] = 1U;
      if (rtu_CHU_index < 2.147483648E+9) {
        if (rtu_CHU_index >= -2.147483648E+9) {
          rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
        } else {
          rtu_CHU_index_0 = MIN_int32_T;
        }
      } else {
        rtu_CHU_index_0 = MAX_int32_T;
      }

      chu_test_state[rtu_CHU_index_0] = 1U;
      if (rtu_CHU_index < 2.147483648E+9) {
        if (rtu_CHU_index >= -2.147483648E+9) {
          rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
        } else {
          rtu_CHU_index_0 = MIN_int32_T;
        }
      } else {
        rtu_CHU_index_0 = MAX_int32_T;
      }

      EMS_SendLowPwrOnCmd_flg[rtu_CHU_index_0] = 1U;
    } else {
      if (rtu_CHU_index < 2.147483648E+9) {
        if (rtu_CHU_index >= -2.147483648E+9) {
          rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
        } else {
          rtu_CHU_index_0 = MIN_int32_T;
        }
      } else {
        rtu_CHU_index_0 = MAX_int32_T;
      }

      if (CHU_LowPwrOn_timeout[rtu_CHU_index_0] >= 5) {
        if (rtu_CHU_index < 2.147483648E+9) {
          if (rtu_CHU_index >= -2.147483648E+9) {
            rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
          } else {
            rtu_CHU_index_0 = MIN_int32_T;
          }
        } else {
          rtu_CHU_index_0 = MAX_int32_T;
        }

        CHU_LowPwrOn_timeout[rtu_CHU_index_0] = 0U;
        if (rtu_CHU_index < 2.147483648E+9) {
          if (rtu_CHU_index >= -2.147483648E+9) {
            rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
          } else {
            rtu_CHU_index_0 = MIN_int32_T;
          }
        } else {
          rtu_CHU_index_0 = MAX_int32_T;
        }

        rtu_CHU_index_1 = CHU_LowPwrOn_timeout[rtu_CHU_index_0] + 1;
        if (rtu_CHU_index_1 > 255) {
          rtu_CHU_index_1 = 255;
        }

        if (rtu_CHU_index < 2.147483648E+9) {
          if (rtu_CHU_index >= -2.147483648E+9) {
            rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
          } else {
            rtu_CHU_index_0 = MIN_int32_T;
          }
        } else {
          rtu_CHU_index_0 = MAX_int32_T;
        }

        CHU_LowPwrOn_timeout[rtu_CHU_index_0] = (uint8_T)rtu_CHU_index_1;
        if (rtu_CHU_index < 2.147483648E+9) {
          if (rtu_CHU_index >= -2.147483648E+9) {
            rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
          } else {
            rtu_CHU_index_0 = MIN_int32_T;
          }
        } else {
          rtu_CHU_index_0 = MAX_int32_T;
        }

        CHU_LowPwrOn_count[rtu_CHU_index_0] = 0U;
        localDW->is_CHU_deal = IN_CHU_LowPwrOn_timeout;
        if (rtu_CHU_index < 2.147483648E+9) {
          if (rtu_CHU_index >= -2.147483648E+9) {
            rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
          } else {
            rtu_CHU_index_0 = MIN_int32_T;
          }
        } else {
          rtu_CHU_index_0 = MAX_int32_T;
        }

        chu_test_state[rtu_CHU_index_0] = 100U;
        if (rtu_CHU_index < 2.147483648E+9) {
          if (rtu_CHU_index >= -2.147483648E+9) {
            rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
          } else {
            rtu_CHU_index_0 = MIN_int32_T;
          }
        } else {
          rtu_CHU_index_0 = MAX_int32_T;
        }

        CHU_LowPwrOnErr_flg[rtu_CHU_index_0] = 1U;
      } else {
        if (rtu_CHU_index < 2.147483648E+9) {
          if (rtu_CHU_index >= -2.147483648E+9) {
            rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
          } else {
            rtu_CHU_index_0 = MIN_int32_T;
          }
        } else {
          rtu_CHU_index_0 = MAX_int32_T;
        }

        if (CHU_DevState_code[rtu_CHU_index_0] == 0x03) {
          if (rtu_CHU_index < 2.147483648E+9) {
            if (rtu_CHU_index >= -2.147483648E+9) {
              rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
            } else {
              rtu_CHU_index_0 = MIN_int32_T;
            }
          } else {
            rtu_CHU_index_0 = MAX_int32_T;
          }

          CHU_LowPwrOn_timeout[rtu_CHU_index_0] = 0U;
          if (rtu_CHU_index < 2.147483648E+9) {
            if (rtu_CHU_index >= -2.147483648E+9) {
              rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
            } else {
              rtu_CHU_index_0 = MIN_int32_T;
            }
          } else {
            rtu_CHU_index_0 = MAX_int32_T;
          }

          EMS_SendLowPwrOnCmd_flg[rtu_CHU_index_0] = 0U;
          if (rtu_CHU_index < 2.147483648E+9) {
            if (rtu_CHU_index >= -2.147483648E+9) {
              rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
            } else {
              rtu_CHU_index_0 = MIN_int32_T;
            }
          } else {
            rtu_CHU_index_0 = MAX_int32_T;
          }

          rtu_CHU_index_1 = CHU_LowPwrOn_timeout[rtu_CHU_index_0] + 1;
          if (rtu_CHU_index_1 > 255) {
            rtu_CHU_index_1 = 255;
          }

          if (rtu_CHU_index < 2.147483648E+9) {
            if (rtu_CHU_index >= -2.147483648E+9) {
              rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
            } else {
              rtu_CHU_index_0 = MIN_int32_T;
            }
          } else {
            rtu_CHU_index_0 = MAX_int32_T;
          }

          CHU_LowPwrOn_timeout[rtu_CHU_index_0] = (uint8_T)rtu_CHU_index_1;
          if (rtu_CHU_index < 2.147483648E+9) {
            if (rtu_CHU_index >= -2.147483648E+9) {
              rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
            } else {
              rtu_CHU_index_0 = MIN_int32_T;
            }
          } else {
            rtu_CHU_index_0 = MAX_int32_T;
          }

          CHU_LowPwrOn_count[rtu_CHU_index_0] = 0U;
          localDW->is_CHU_deal = IN_charging_state;
          if (rtu_CHU_index < 2.147483648E+9) {
            if (rtu_CHU_index >= -2.147483648E+9) {
              rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
            } else {
              rtu_CHU_index_0 = MIN_int32_T;
            }
          } else {
            rtu_CHU_index_0 = MAX_int32_T;
          }

          GetCellVol_count1[rtu_CHU_index_0] = 220U;
          localDW->is_GetCellVol = IN_GetCellVol0;
          if (rtu_CHU_index < 2.147483648E+9) {
            if (rtu_CHU_index >= -2.147483648E+9) {
              rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
            } else {
              rtu_CHU_index_0 = MIN_int32_T;
            }
          } else {
            rtu_CHU_index_0 = MAX_int32_T;
          }

          EMS_SendGetCellVolCmd_flg[rtu_CHU_index_0] = 1U;
          if (rtu_CHU_index < 2.147483648E+9) {
            if (rtu_CHU_index >= -2.147483648E+9) {
              rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
            } else {
              rtu_CHU_index_0 = MIN_int32_T;
            }
          } else {
            rtu_CHU_index_0 = MAX_int32_T;
          }

          GetCellTemp_count1[rtu_CHU_index_0] = 350U;
          localDW->is_GetCellTemp = IN_GetCellTemp0;
          if (rtu_CHU_index < 2.147483648E+9) {
            if (rtu_CHU_index >= -2.147483648E+9) {
              rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
            } else {
              rtu_CHU_index_0 = MIN_int32_T;
            }
          } else {
            rtu_CHU_index_0 = MAX_int32_T;
          }

          EMS_SendGetCellTempCmd_flg[rtu_CHU_index_0] = 1U;
          localDW->is_GetCharPreInf = IN_GetCharPreInf0;
          if (rtu_CHU_index < 2.147483648E+9) {
            if (rtu_CHU_index >= -2.147483648E+9) {
              rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
            } else {
              rtu_CHU_index_0 = MIN_int32_T;
            }
          } else {
            rtu_CHU_index_0 = MAX_int32_T;
          }

          EMS_SendGetCharPreInfCmd_flg[rtu_CHU_index_0] = 1U;
          if (rtu_CHU_index < 2.147483648E+9) {
            if (rtu_CHU_index >= -2.147483648E+9) {
              rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
            } else {
              rtu_CHU_index_0 = MIN_int32_T;
            }
          } else {
            rtu_CHU_index_0 = MAX_int32_T;
          }

          GetBattRtData_count1[rtu_CHU_index_0] = 50U;
          localDW->is_GetBattRtData = IN_GetBattRtData0;
          if (rtu_CHU_index < 2.147483648E+9) {
            if (rtu_CHU_index >= -2.147483648E+9) {
              rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
            } else {
              rtu_CHU_index_0 = MIN_int32_T;
            }
          } else {
            rtu_CHU_index_0 = MAX_int32_T;
          }

          EMS_SendGetBattRtDataCmd_flg[rtu_CHU_index_0] = 1U;
          if (rtu_CHU_index < 2.147483648E+9) {
            if (rtu_CHU_index >= -2.147483648E+9) {
              rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
            } else {
              rtu_CHU_index_0 = MIN_int32_T;
            }
          } else {
            rtu_CHU_index_0 = MAX_int32_T;
          }

          GetBattBasicData_count1[rtu_CHU_index_0] = 110U;
          localDW->is_GetBattBasicData = IN_GetBattBasicData0;
          if (rtu_CHU_index < 2.147483648E+9) {
            if (rtu_CHU_index >= -2.147483648E+9) {
              rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
            } else {
              rtu_CHU_index_0 = MIN_int32_T;
            }
          } else {
            rtu_CHU_index_0 = MAX_int32_T;
          }

          EMS_SendGetBattBasicDataCmd_flg[rtu_CHU_index_0] = 1U;
          localDW->is_chg_main_state = IN_free1;
          localDW->Chg_wait_cou = 0.0;
          if (rtu_CHU_index < 2.147483648E+9) {
            if (rtu_CHU_index >= -2.147483648E+9) {
              rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
            } else {
              rtu_CHU_index_0 = MIN_int32_T;
            }
          } else {
            rtu_CHU_index_0 = MAX_int32_T;
          }

          chu_test_state[rtu_CHU_index_0] = 10U;
        } else {
          /* EMS_SendLowPwrOnCmd_flg[CHU_index] = 0; */
          if (rtu_CHU_index < 2.147483648E+9) {
            if (rtu_CHU_index >= -2.147483648E+9) {
              rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
            } else {
              rtu_CHU_index_0 = MIN_int32_T;
            }
          } else {
            rtu_CHU_index_0 = MAX_int32_T;
          }

          rtu_CHU_index_1 = CHU_LowPwrOn_count[rtu_CHU_index_0] + 1;
          if (rtu_CHU_index_1 > 65535) {
            rtu_CHU_index_1 = 65535;
          }

          if (rtu_CHU_index < 2.147483648E+9) {
            if (rtu_CHU_index >= -2.147483648E+9) {
              rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
            } else {
              rtu_CHU_index_0 = MIN_int32_T;
            }
          } else {
            rtu_CHU_index_0 = MAX_int32_T;
          }

          CHU_LowPwrOn_count[rtu_CHU_index_0] = (uint16_T)rtu_CHU_index_1;
        }
      }
    }
    break;

   default:
    if (rtu_CHU_index < 2.147483648E+9) {
      if (rtu_CHU_index >= -2.147483648E+9) {
        rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
        rtu_CHU_index_1 = (int32_T)rtu_CHU_index;
        rtu_CHU_index_2 = (int32_T)rtu_CHU_index;
        rtu_CHU_index_3 = (int32_T)rtu_CHU_index;
      } else {
        rtu_CHU_index_0 = MIN_int32_T;
        rtu_CHU_index_1 = MIN_int32_T;
        rtu_CHU_index_2 = MIN_int32_T;
        rtu_CHU_index_3 = MIN_int32_T;
      }
    } else {
      rtu_CHU_index_0 = MAX_int32_T;
      rtu_CHU_index_1 = MAX_int32_T;
      rtu_CHU_index_2 = MAX_int32_T;
      rtu_CHU_index_3 = MAX_int32_T;
    }

    if (((EMS_Chger_BigSta[rtu_CHU_index_0] == 0x05) ||
         (EMS_Chger_BigSta[rtu_CHU_index_1] < 3)) &&
        ((Chg_Control_mode[rtu_CHU_index_2] == 0) &&
         (Hand_LowPwrDown_flg[rtu_CHU_index_3] == 1))) {
      if (rtu_CHU_index < 2.147483648E+9) {
        if (rtu_CHU_index >= -2.147483648E+9) {
          rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
        } else {
          rtu_CHU_index_0 = MIN_int32_T;
        }
      } else {
        rtu_CHU_index_0 = MAX_int32_T;
      }

      Hand_LowPwrDown_flg[rtu_CHU_index_0] = 0U;
      guard1 = true;
    } else {
      if (rtu_CHU_index < 2.147483648E+9) {
        if (rtu_CHU_index >= -2.147483648E+9) {
          rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
          rtu_CHU_index_1 = (int32_T)rtu_CHU_index;
          rtu_CHU_index_2 = (int32_T)rtu_CHU_index;
          rtu_CHU_index_3 = (int32_T)rtu_CHU_index;
        } else {
          rtu_CHU_index_0 = MIN_int32_T;
          rtu_CHU_index_1 = MIN_int32_T;
          rtu_CHU_index_2 = MIN_int32_T;
          rtu_CHU_index_3 = MIN_int32_T;
        }
      } else {
        rtu_CHU_index_0 = MAX_int32_T;
        rtu_CHU_index_1 = MAX_int32_T;
        rtu_CHU_index_2 = MAX_int32_T;
        rtu_CHU_index_3 = MAX_int32_T;
      }

      if ((Chg_Control_mode[rtu_CHU_index_0] == 1) &&
          ((chu_test_state[rtu_CHU_index_1] == 10) ||
           (chu_test_state[rtu_CHU_index_2] == 14)) &&
          (MChar_StartCE_flg[rtu_CHU_index_3] == 1)) {
        guard1 = true;
      } else {
        if (rtu_CHU_index < 2.147483648E+9) {
          if (rtu_CHU_index >= -2.147483648E+9) {
            rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
            rtu_CHU_index_1 = (int32_T)rtu_CHU_index;
            rtu_CHU_index_2 = (int32_T)rtu_CHU_index;
            rtu_CHU_index_3 = (int32_T)rtu_CHU_index;
            rtu_CHU_index_4 = (int32_T)rtu_CHU_index;
          } else {
            rtu_CHU_index_0 = MIN_int32_T;
            rtu_CHU_index_1 = MIN_int32_T;
            rtu_CHU_index_2 = MIN_int32_T;
            rtu_CHU_index_3 = MIN_int32_T;
            rtu_CHU_index_4 = MIN_int32_T;
          }
        } else {
          rtu_CHU_index_0 = MAX_int32_T;
          rtu_CHU_index_1 = MAX_int32_T;
          rtu_CHU_index_2 = MAX_int32_T;
          rtu_CHU_index_3 = MAX_int32_T;
          rtu_CHU_index_4 = MAX_int32_T;
        }

        if ((Chg_Control_mode[rtu_CHU_index_0] == 1) &&
            (chu_test_state[rtu_CHU_index_1] == 14) &&
            (CHU_CharFull_flg[rtu_CHU_index_2] == 0) &&
            ((MChar_StartCE_flg[rtu_CHU_index_3] == 0) &&
             (CHU_AllErr_flg[rtu_CHU_index_4] == 0))) {
          guard1 = true;
        } else {
          if (rtu_CHU_index < 2.147483648E+9) {
            if (rtu_CHU_index >= -2.147483648E+9) {
              rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
              rtu_CHU_index_1 = (int32_T)rtu_CHU_index;
              rtu_CHU_index_2 = (int32_T)rtu_CHU_index;
              rtu_CHU_index_3 = (int32_T)rtu_CHU_index;
            } else {
              rtu_CHU_index_0 = MIN_int32_T;
              rtu_CHU_index_1 = MIN_int32_T;
              rtu_CHU_index_2 = MIN_int32_T;
              rtu_CHU_index_3 = MIN_int32_T;
            }
          } else {
            rtu_CHU_index_0 = MAX_int32_T;
            rtu_CHU_index_1 = MAX_int32_T;
            rtu_CHU_index_2 = MAX_int32_T;
            rtu_CHU_index_3 = MAX_int32_T;
          }

          if ((Chg_Control_mode[rtu_CHU_index_0] == 0) &&
              (CHU_AllErr_flg[rtu_CHU_index_1] == 0) &&
              (chu_test_state[rtu_CHU_index_2] == 14) &&
              (Hand_StartChg_flg[rtu_CHU_index_3] == 1)) {
            guard1 = true;
          } else {
            if (rtu_CHU_index < 2.147483648E+9) {
              if (rtu_CHU_index >= -2.147483648E+9) {
                rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
                rtu_CHU_index_1 = (int32_T)rtu_CHU_index;
              } else {
                rtu_CHU_index_0 = MIN_int32_T;
                rtu_CHU_index_1 = MIN_int32_T;
              }
            } else {
              rtu_CHU_index_0 = MAX_int32_T;
              rtu_CHU_index_1 = MAX_int32_T;
            }

            if (((MIO_Water_err == 1) || (FireContr_GetFire_flg == 1)) &&
                ((chu_test_state[rtu_CHU_index_0] == 10) ||
                 (chu_test_state[rtu_CHU_index_1] == 14))) {
              guard1 = true;
            } else {
              if (localDW->is_GetCellVol == IN_GetCellVol0) {
                if (rtu_CHU_index < 2.147483648E+9) {
                  if (rtu_CHU_index >= -2.147483648E+9) {
                    rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
                  } else {
                    rtu_CHU_index_0 = MIN_int32_T;
                  }
                } else {
                  rtu_CHU_index_0 = MAX_int32_T;
                }

                if (GetCellVol_count0[rtu_CHU_index_0] >= 1) {
                  if (rtu_CHU_index < 2.147483648E+9) {
                    if (rtu_CHU_index >= -2.147483648E+9) {
                      rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
                    } else {
                      rtu_CHU_index_0 = MIN_int32_T;
                    }
                  } else {
                    rtu_CHU_index_0 = MAX_int32_T;
                  }

                  GetCellVol_count0[rtu_CHU_index_0] = 0U;
                  localDW->is_GetCellVol = IN_GetCellVol1;
                } else {
                  /* EMS_SendGetCellVolCmd_flg[CHU_index] = 0; */
                  if (rtu_CHU_index < 2.147483648E+9) {
                    if (rtu_CHU_index >= -2.147483648E+9) {
                      rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
                    } else {
                      rtu_CHU_index_0 = MIN_int32_T;
                    }
                  } else {
                    rtu_CHU_index_0 = MAX_int32_T;
                  }

                  rtu_CHU_index_1 = GetCellVol_count0[rtu_CHU_index_0] + 1;
                  if (rtu_CHU_index_1 > 65535) {
                    rtu_CHU_index_1 = 65535;
                  }

                  if (rtu_CHU_index < 2.147483648E+9) {
                    if (rtu_CHU_index >= -2.147483648E+9) {
                      rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
                    } else {
                      rtu_CHU_index_0 = MIN_int32_T;
                    }
                  } else {
                    rtu_CHU_index_0 = MAX_int32_T;
                  }

                  GetCellVol_count0[rtu_CHU_index_0] = (uint16_T)rtu_CHU_index_1;
                }
              } else {
                if (rtu_CHU_index < 2.147483648E+9) {
                  if (rtu_CHU_index >= -2.147483648E+9) {
                    rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
                  } else {
                    rtu_CHU_index_0 = MIN_int32_T;
                  }
                } else {
                  rtu_CHU_index_0 = MAX_int32_T;
                }

                if (GetCellVol_count1[rtu_CHU_index_0] >= 1000) {
                  if (rtu_CHU_index < 2.147483648E+9) {
                    if (rtu_CHU_index >= -2.147483648E+9) {
                      rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
                    } else {
                      rtu_CHU_index_0 = MIN_int32_T;
                    }
                  } else {
                    rtu_CHU_index_0 = MAX_int32_T;
                  }

                  GetCellVol_count1[rtu_CHU_index_0] = 0U;
                  localDW->is_GetCellVol = IN_GetCellVol0;
                  if (rtu_CHU_index < 2.147483648E+9) {
                    if (rtu_CHU_index >= -2.147483648E+9) {
                      rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
                    } else {
                      rtu_CHU_index_0 = MIN_int32_T;
                    }
                  } else {
                    rtu_CHU_index_0 = MAX_int32_T;
                  }

                  EMS_SendGetCellVolCmd_flg[rtu_CHU_index_0] = 1U;
                } else {
                  if (rtu_CHU_index < 2.147483648E+9) {
                    if (rtu_CHU_index >= -2.147483648E+9) {
                      rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
                    } else {
                      rtu_CHU_index_0 = MIN_int32_T;
                    }
                  } else {
                    rtu_CHU_index_0 = MAX_int32_T;
                  }

                  rtu_CHU_index_1 = GetCellVol_count1[rtu_CHU_index_0] + 1;
                  if (rtu_CHU_index_1 > 65535) {
                    rtu_CHU_index_1 = 65535;
                  }

                  if (rtu_CHU_index < 2.147483648E+9) {
                    if (rtu_CHU_index >= -2.147483648E+9) {
                      rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
                    } else {
                      rtu_CHU_index_0 = MIN_int32_T;
                    }
                  } else {
                    rtu_CHU_index_0 = MAX_int32_T;
                  }

                  GetCellVol_count1[rtu_CHU_index_0] = (uint16_T)rtu_CHU_index_1;
                }
              }

              if (localDW->is_GetCellTemp == IN_GetCellTemp0) {
                if (rtu_CHU_index < 2.147483648E+9) {
                  if (rtu_CHU_index >= -2.147483648E+9) {
                    rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
                  } else {
                    rtu_CHU_index_0 = MIN_int32_T;
                  }
                } else {
                  rtu_CHU_index_0 = MAX_int32_T;
                }

                if (GetCellTemp_count0[rtu_CHU_index_0] >= 1) {
                  if (rtu_CHU_index < 2.147483648E+9) {
                    if (rtu_CHU_index >= -2.147483648E+9) {
                      rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
                    } else {
                      rtu_CHU_index_0 = MIN_int32_T;
                    }
                  } else {
                    rtu_CHU_index_0 = MAX_int32_T;
                  }

                  GetCellTemp_count0[rtu_CHU_index_0] = 0U;
                  localDW->is_GetCellTemp = IN_GetCellTemp1;
                } else {
                  /* EMS_SendGetCellTempCmd_flg[CHU_index] = 0; */
                  if (rtu_CHU_index < 2.147483648E+9) {
                    if (rtu_CHU_index >= -2.147483648E+9) {
                      rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
                    } else {
                      rtu_CHU_index_0 = MIN_int32_T;
                    }
                  } else {
                    rtu_CHU_index_0 = MAX_int32_T;
                  }

                  rtu_CHU_index_1 = GetCellTemp_count0[rtu_CHU_index_0] + 1;
                  if (rtu_CHU_index_1 > 65535) {
                    rtu_CHU_index_1 = 65535;
                  }

                  if (rtu_CHU_index < 2.147483648E+9) {
                    if (rtu_CHU_index >= -2.147483648E+9) {
                      rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
                    } else {
                      rtu_CHU_index_0 = MIN_int32_T;
                    }
                  } else {
                    rtu_CHU_index_0 = MAX_int32_T;
                  }

                  GetCellTemp_count0[rtu_CHU_index_0] = (uint16_T)
                    rtu_CHU_index_1;
                }
              } else {
                if (rtu_CHU_index < 2.147483648E+9) {
                  if (rtu_CHU_index >= -2.147483648E+9) {
                    rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
                  } else {
                    rtu_CHU_index_0 = MIN_int32_T;
                  }
                } else {
                  rtu_CHU_index_0 = MAX_int32_T;
                }

                if (GetCellTemp_count1[rtu_CHU_index_0] >= 1000) {
                  if (rtu_CHU_index < 2.147483648E+9) {
                    if (rtu_CHU_index >= -2.147483648E+9) {
                      rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
                    } else {
                      rtu_CHU_index_0 = MIN_int32_T;
                    }
                  } else {
                    rtu_CHU_index_0 = MAX_int32_T;
                  }

                  GetCellTemp_count1[rtu_CHU_index_0] = 0U;
                  localDW->is_GetCellTemp = IN_GetCellTemp0;
                  if (rtu_CHU_index < 2.147483648E+9) {
                    if (rtu_CHU_index >= -2.147483648E+9) {
                      rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
                    } else {
                      rtu_CHU_index_0 = MIN_int32_T;
                    }
                  } else {
                    rtu_CHU_index_0 = MAX_int32_T;
                  }

                  EMS_SendGetCellTempCmd_flg[rtu_CHU_index_0] = 1U;
                } else {
                  if (rtu_CHU_index < 2.147483648E+9) {
                    if (rtu_CHU_index >= -2.147483648E+9) {
                      rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
                    } else {
                      rtu_CHU_index_0 = MIN_int32_T;
                    }
                  } else {
                    rtu_CHU_index_0 = MAX_int32_T;
                  }

                  rtu_CHU_index_1 = GetCellTemp_count1[rtu_CHU_index_0] + 1;
                  if (rtu_CHU_index_1 > 65535) {
                    rtu_CHU_index_1 = 65535;
                  }

                  if (rtu_CHU_index < 2.147483648E+9) {
                    if (rtu_CHU_index >= -2.147483648E+9) {
                      rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
                    } else {
                      rtu_CHU_index_0 = MIN_int32_T;
                    }
                  } else {
                    rtu_CHU_index_0 = MAX_int32_T;
                  }

                  GetCellTemp_count1[rtu_CHU_index_0] = (uint16_T)
                    rtu_CHU_index_1;
                }
              }

              if (localDW->is_GetCharPreInf == IN_GetCharPreInf0) {
                if (rtu_CHU_index < 2.147483648E+9) {
                  if (rtu_CHU_index >= -2.147483648E+9) {
                    rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
                  } else {
                    rtu_CHU_index_0 = MIN_int32_T;
                  }
                } else {
                  rtu_CHU_index_0 = MAX_int32_T;
                }

                if (GetCharPreInf_count0[rtu_CHU_index_0] >= 1) {
                  if (rtu_CHU_index < 2.147483648E+9) {
                    if (rtu_CHU_index >= -2.147483648E+9) {
                      rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
                    } else {
                      rtu_CHU_index_0 = MIN_int32_T;
                    }
                  } else {
                    rtu_CHU_index_0 = MAX_int32_T;
                  }

                  GetCharPreInf_count0[rtu_CHU_index_0] = 0U;
                  localDW->is_GetCharPreInf = IN_GetCharPreInf1;
                } else {
                  /* EMS_SendGetCharPreInfCmd_flg[CHU_index] = 0; */
                  if (rtu_CHU_index < 2.147483648E+9) {
                    if (rtu_CHU_index >= -2.147483648E+9) {
                      rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
                    } else {
                      rtu_CHU_index_0 = MIN_int32_T;
                    }
                  } else {
                    rtu_CHU_index_0 = MAX_int32_T;
                  }

                  rtu_CHU_index_1 = GetCharPreInf_count0[rtu_CHU_index_0] + 1;
                  if (rtu_CHU_index_1 > 65535) {
                    rtu_CHU_index_1 = 65535;
                  }

                  if (rtu_CHU_index < 2.147483648E+9) {
                    if (rtu_CHU_index >= -2.147483648E+9) {
                      rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
                    } else {
                      rtu_CHU_index_0 = MIN_int32_T;
                    }
                  } else {
                    rtu_CHU_index_0 = MAX_int32_T;
                  }

                  GetCharPreInf_count0[rtu_CHU_index_0] = (uint16_T)
                    rtu_CHU_index_1;
                }
              } else {
                if (rtu_CHU_index < 2.147483648E+9) {
                  if (rtu_CHU_index >= -2.147483648E+9) {
                    rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
                  } else {
                    rtu_CHU_index_0 = MIN_int32_T;
                  }
                } else {
                  rtu_CHU_index_0 = MAX_int32_T;
                }

                if (GetCharPreInf_count1[rtu_CHU_index_0] >= 1000) {
                  if (rtu_CHU_index < 2.147483648E+9) {
                    if (rtu_CHU_index >= -2.147483648E+9) {
                      rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
                    } else {
                      rtu_CHU_index_0 = MIN_int32_T;
                    }
                  } else {
                    rtu_CHU_index_0 = MAX_int32_T;
                  }

                  GetCharPreInf_count1[rtu_CHU_index_0] = 0U;
                  localDW->is_GetCharPreInf = IN_GetCharPreInf0;
                  if (rtu_CHU_index < 2.147483648E+9) {
                    if (rtu_CHU_index >= -2.147483648E+9) {
                      rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
                    } else {
                      rtu_CHU_index_0 = MIN_int32_T;
                    }
                  } else {
                    rtu_CHU_index_0 = MAX_int32_T;
                  }

                  EMS_SendGetCharPreInfCmd_flg[rtu_CHU_index_0] = 1U;
                } else {
                  if (rtu_CHU_index < 2.147483648E+9) {
                    if (rtu_CHU_index >= -2.147483648E+9) {
                      rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
                    } else {
                      rtu_CHU_index_0 = MIN_int32_T;
                    }
                  } else {
                    rtu_CHU_index_0 = MAX_int32_T;
                  }

                  rtu_CHU_index_1 = GetCharPreInf_count1[rtu_CHU_index_0] + 1;
                  if (rtu_CHU_index_1 > 65535) {
                    rtu_CHU_index_1 = 65535;
                  }

                  if (rtu_CHU_index < 2.147483648E+9) {
                    if (rtu_CHU_index >= -2.147483648E+9) {
                      rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
                    } else {
                      rtu_CHU_index_0 = MIN_int32_T;
                    }
                  } else {
                    rtu_CHU_index_0 = MAX_int32_T;
                  }

                  GetCharPreInf_count1[rtu_CHU_index_0] = (uint16_T)
                    rtu_CHU_index_1;
                }
              }

              if (localDW->is_GetBattRtData == IN_GetBattRtData0) {
                if (rtu_CHU_index < 2.147483648E+9) {
                  if (rtu_CHU_index >= -2.147483648E+9) {
                    rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
                  } else {
                    rtu_CHU_index_0 = MIN_int32_T;
                  }
                } else {
                  rtu_CHU_index_0 = MAX_int32_T;
                }

                if (GetBattRtData_count0[rtu_CHU_index_0] >= 1) {
                  if (rtu_CHU_index < 2.147483648E+9) {
                    if (rtu_CHU_index >= -2.147483648E+9) {
                      rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
                    } else {
                      rtu_CHU_index_0 = MIN_int32_T;
                    }
                  } else {
                    rtu_CHU_index_0 = MAX_int32_T;
                  }

                  GetBattRtData_count0[rtu_CHU_index_0] = 0U;
                  localDW->is_GetBattRtData = IN_GetBattRtData1;
                } else {
                  /* EMS_SendGetBattRtDataCmd_flg[CHU_index] = 0; */
                  if (rtu_CHU_index < 2.147483648E+9) {
                    if (rtu_CHU_index >= -2.147483648E+9) {
                      rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
                    } else {
                      rtu_CHU_index_0 = MIN_int32_T;
                    }
                  } else {
                    rtu_CHU_index_0 = MAX_int32_T;
                  }

                  rtu_CHU_index_1 = GetBattRtData_count0[rtu_CHU_index_0] + 1;
                  if (rtu_CHU_index_1 > 65535) {
                    rtu_CHU_index_1 = 65535;
                  }

                  if (rtu_CHU_index < 2.147483648E+9) {
                    if (rtu_CHU_index >= -2.147483648E+9) {
                      rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
                    } else {
                      rtu_CHU_index_0 = MIN_int32_T;
                    }
                  } else {
                    rtu_CHU_index_0 = MAX_int32_T;
                  }

                  GetBattRtData_count0[rtu_CHU_index_0] = (uint16_T)
                    rtu_CHU_index_1;
                }
              } else {
                if (rtu_CHU_index < 2.147483648E+9) {
                  if (rtu_CHU_index >= -2.147483648E+9) {
                    rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
                  } else {
                    rtu_CHU_index_0 = MIN_int32_T;
                  }
                } else {
                  rtu_CHU_index_0 = MAX_int32_T;
                }

                if (GetBattRtData_count1[rtu_CHU_index_0] >= 300) {
                  if (rtu_CHU_index < 2.147483648E+9) {
                    if (rtu_CHU_index >= -2.147483648E+9) {
                      rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
                    } else {
                      rtu_CHU_index_0 = MIN_int32_T;
                    }
                  } else {
                    rtu_CHU_index_0 = MAX_int32_T;
                  }

                  GetBattRtData_count1[rtu_CHU_index_0] = 0U;
                  localDW->is_GetBattRtData = IN_GetBattRtData0;
                  if (rtu_CHU_index < 2.147483648E+9) {
                    if (rtu_CHU_index >= -2.147483648E+9) {
                      rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
                    } else {
                      rtu_CHU_index_0 = MIN_int32_T;
                    }
                  } else {
                    rtu_CHU_index_0 = MAX_int32_T;
                  }

                  EMS_SendGetBattRtDataCmd_flg[rtu_CHU_index_0] = 1U;
                } else {
                  if (rtu_CHU_index < 2.147483648E+9) {
                    if (rtu_CHU_index >= -2.147483648E+9) {
                      rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
                    } else {
                      rtu_CHU_index_0 = MIN_int32_T;
                    }
                  } else {
                    rtu_CHU_index_0 = MAX_int32_T;
                  }

                  rtu_CHU_index_1 = GetBattRtData_count1[rtu_CHU_index_0] + 1;
                  if (rtu_CHU_index_1 > 65535) {
                    rtu_CHU_index_1 = 65535;
                  }

                  if (rtu_CHU_index < 2.147483648E+9) {
                    if (rtu_CHU_index >= -2.147483648E+9) {
                      rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
                    } else {
                      rtu_CHU_index_0 = MIN_int32_T;
                    }
                  } else {
                    rtu_CHU_index_0 = MAX_int32_T;
                  }

                  GetBattRtData_count1[rtu_CHU_index_0] = (uint16_T)
                    rtu_CHU_index_1;
                }
              }

              if (localDW->is_GetBattBasicData == IN_GetBattBasicData0) {
                if (rtu_CHU_index < 2.147483648E+9) {
                  if (rtu_CHU_index >= -2.147483648E+9) {
                    rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
                  } else {
                    rtu_CHU_index_0 = MIN_int32_T;
                  }
                } else {
                  rtu_CHU_index_0 = MAX_int32_T;
                }

                if (GetBattBasicData_count0[rtu_CHU_index_0] >= 1) {
                  if (rtu_CHU_index < 2.147483648E+9) {
                    if (rtu_CHU_index >= -2.147483648E+9) {
                      rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
                    } else {
                      rtu_CHU_index_0 = MIN_int32_T;
                    }
                  } else {
                    rtu_CHU_index_0 = MAX_int32_T;
                  }

                  GetBattBasicData_count0[rtu_CHU_index_0] = 0U;
                  localDW->is_GetBattBasicData = IN_GetBattBasicData1;
                } else {
                  /* EMS_SendGetBattBasicDataCmd_flg[CHU_index] = 0 */
                  if (rtu_CHU_index < 2.147483648E+9) {
                    if (rtu_CHU_index >= -2.147483648E+9) {
                      rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
                    } else {
                      rtu_CHU_index_0 = MIN_int32_T;
                    }
                  } else {
                    rtu_CHU_index_0 = MAX_int32_T;
                  }

                  rtu_CHU_index_1 = GetBattBasicData_count0[rtu_CHU_index_0] + 1;
                  if (rtu_CHU_index_1 > 65535) {
                    rtu_CHU_index_1 = 65535;
                  }

                  if (rtu_CHU_index < 2.147483648E+9) {
                    if (rtu_CHU_index >= -2.147483648E+9) {
                      rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
                    } else {
                      rtu_CHU_index_0 = MIN_int32_T;
                    }
                  } else {
                    rtu_CHU_index_0 = MAX_int32_T;
                  }

                  GetBattBasicData_count0[rtu_CHU_index_0] = (uint16_T)
                    rtu_CHU_index_1;
                }
              } else {
                if (rtu_CHU_index < 2.147483648E+9) {
                  if (rtu_CHU_index >= -2.147483648E+9) {
                    rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
                  } else {
                    rtu_CHU_index_0 = MIN_int32_T;
                  }
                } else {
                  rtu_CHU_index_0 = MAX_int32_T;
                }

                if (GetBattBasicData_count1[rtu_CHU_index_0] >= 1000) {
                  if (rtu_CHU_index < 2.147483648E+9) {
                    if (rtu_CHU_index >= -2.147483648E+9) {
                      rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
                    } else {
                      rtu_CHU_index_0 = MIN_int32_T;
                    }
                  } else {
                    rtu_CHU_index_0 = MAX_int32_T;
                  }

                  GetBattBasicData_count1[rtu_CHU_index_0] = 0U;
                  localDW->is_GetBattBasicData = IN_GetBattBasicData0;
                  if (rtu_CHU_index < 2.147483648E+9) {
                    if (rtu_CHU_index >= -2.147483648E+9) {
                      rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
                    } else {
                      rtu_CHU_index_0 = MIN_int32_T;
                    }
                  } else {
                    rtu_CHU_index_0 = MAX_int32_T;
                  }

                  EMS_SendGetBattBasicDataCmd_flg[rtu_CHU_index_0] = 1U;
                } else {
                  if (rtu_CHU_index < 2.147483648E+9) {
                    if (rtu_CHU_index >= -2.147483648E+9) {
                      rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
                    } else {
                      rtu_CHU_index_0 = MIN_int32_T;
                    }
                  } else {
                    rtu_CHU_index_0 = MAX_int32_T;
                  }

                  rtu_CHU_index_1 = GetBattBasicData_count1[rtu_CHU_index_0] + 1;
                  if (rtu_CHU_index_1 > 65535) {
                    rtu_CHU_index_1 = 65535;
                  }

                  if (rtu_CHU_index < 2.147483648E+9) {
                    if (rtu_CHU_index >= -2.147483648E+9) {
                      rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
                    } else {
                      rtu_CHU_index_0 = MIN_int32_T;
                    }
                  } else {
                    rtu_CHU_index_0 = MAX_int32_T;
                  }

                  GetBattBasicData_count1[rtu_CHU_index_0] = (uint16_T)
                    rtu_CHU_index_1;
                }
              }

              chg_main_state(rtu_CHU_index, localDW);
            }
          }
        }
      }
    }
    break;
  }

  if (guard1) {
    exit_internal_charging_state(rtu_CHU_index, localDW);
    clear(rtu_CHU_index);
    localDW->is_CHU_deal = IN_LowPwrDown;
    if (rtu_CHU_index < 2.147483648E+9) {
      if (rtu_CHU_index >= -2.147483648E+9) {
        rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
      } else {
        rtu_CHU_index_0 = MIN_int32_T;
      }
    } else {
      rtu_CHU_index_0 = MAX_int32_T;
    }

    EMS_Chger_BigSta[rtu_CHU_index_0] = 6U;
    if (rtu_CHU_index < 2.147483648E+9) {
      if (rtu_CHU_index >= -2.147483648E+9) {
        rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
      } else {
        rtu_CHU_index_0 = MIN_int32_T;
      }
    } else {
      rtu_CHU_index_0 = MAX_int32_T;
    }

    chu_test_state[rtu_CHU_index_0] = 20U;
    if (rtu_CHU_index < 2.147483648E+9) {
      if (rtu_CHU_index >= -2.147483648E+9) {
        rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
      } else {
        rtu_CHU_index_0 = MIN_int32_T;
      }
    } else {
      rtu_CHU_index_0 = MAX_int32_T;
    }

    EMS_SendLowPwrDownCmd_flg[rtu_CHU_index_0] = 1U;
  }
}

/*
 * Output and update for atomic system:
 *    '<S3>/Chart1'
 *    '<S3>/Chart10'
 *    '<S3>/Chart11'
 *    '<S3>/Chart12'
 *    '<S3>/Chart13'
 *    '<S3>/Chart14'
 *    '<S3>/Chart15'
 *    '<S3>/Chart16'
 *    '<S3>/Chart17'
 *    '<S3>/Chart18'
 *    ...
 */
void Chart1(real_T rtu_CHU_index, DW_Chart1 *localDW)
{
  int32_T rtu_CHU_index_0;

  /* Chart: '<S3>/Chart1' */
  if (localDW->is_active_c1_SA4_EMS == 0U) {
    localDW->is_active_c1_SA4_EMS = 1U;
    localDW->is_CHU_deal = IN_Init;
    if (rtu_CHU_index < 2.147483648E+9) {
      if (rtu_CHU_index >= -2.147483648E+9) {
        rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
      } else {
        rtu_CHU_index_0 = MIN_int32_T;
      }
    } else {
      rtu_CHU_index_0 = MAX_int32_T;
    }

    EMS_Chger_BigSta[rtu_CHU_index_0] = 0U;
    if (rtu_CHU_index < 2.147483648E+9) {
      if (rtu_CHU_index >= -2.147483648E+9) {
        rtu_CHU_index_0 = (int32_T)rtu_CHU_index;
      } else {
        rtu_CHU_index_0 = MIN_int32_T;
      }
    } else {
      rtu_CHU_index_0 = MAX_int32_T;
    }

    chu_test_state[rtu_CHU_index_0] = 0U;

    /* init_clear(); */
  } else {
    CHU_deal(rtu_CHU_index, localDW);
  }

  /* End of Chart: '<S3>/Chart1' */
}

/* Output and update for atomic system: '<Root>/Subsystem' */
void Charger(void)
{
  /* Chart: '<S3>/Chart1' incorporates:
   *  Constant: '<S3>/Constant'
   */
  Chart1(0.0, &rtDW.sf_Chart1_b);

  /* Chart: '<S3>/Chart10' incorporates:
   *  Constant: '<S3>/Constant9'
   */
  Chart1(9.0, &rtDW.sf_Chart10);

  /* Chart: '<S3>/Chart11' incorporates:
   *  Constant: '<S3>/Constant10'
   */
  Chart1(10.0, &rtDW.sf_Chart11);

  /* Chart: '<S3>/Chart12' incorporates:
   *  Constant: '<S3>/Constant11'
   */
  Chart1(11.0, &rtDW.sf_Chart12);

  /* Chart: '<S3>/Chart13' incorporates:
   *  Constant: '<S3>/Constant12'
   */
  Chart1(12.0, &rtDW.sf_Chart13);

  /* Chart: '<S3>/Chart14' incorporates:
   *  Constant: '<S3>/Constant13'
   */
  Chart1(13.0, &rtDW.sf_Chart14);

  /* Chart: '<S3>/Chart15' incorporates:
   *  Constant: '<S3>/Constant14'
   */
  Chart1(14.0, &rtDW.sf_Chart15);

  /* Chart: '<S3>/Chart16' incorporates:
   *  Constant: '<S3>/Constant15'
   */
  Chart1(15.0, &rtDW.sf_Chart16);

  /* Chart: '<S3>/Chart17' incorporates:
   *  Constant: '<S3>/Constant16'
   */
  Chart1(16.0, &rtDW.sf_Chart17);

  /* Chart: '<S3>/Chart18' incorporates:
   *  Constant: '<S3>/Constant17'
   */
  Chart1(17.0, &rtDW.sf_Chart18);

  /* Chart: '<S3>/Chart19' incorporates:
   *  Constant: '<S3>/Constant18'
   */
  Chart1(18.0, &rtDW.sf_Chart19);

  /* Chart: '<S3>/Chart2' incorporates:
   *  Constant: '<S3>/Constant1'
   */
  Chart1(1.0, &rtDW.sf_Chart2);

  /* Chart: '<S3>/Chart20' incorporates:
   *  Constant: '<S3>/Constant19'
   */
  Chart1(19.0, &rtDW.sf_Chart20);

  /* Chart: '<S3>/Chart3' incorporates:
   *  Constant: '<S3>/Constant2'
   */
  Chart1(2.0, &rtDW.sf_Chart3);

  /* Chart: '<S3>/Chart4' incorporates:
   *  Constant: '<S3>/Constant3'
   */
  Chart1(3.0, &rtDW.sf_Chart4);

  /* Chart: '<S3>/Chart5' incorporates:
   *  Constant: '<S3>/Constant4'
   */
  Chart1(4.0, &rtDW.sf_Chart5);

  /* Chart: '<S3>/Chart6' incorporates:
   *  Constant: '<S3>/Constant5'
   */
  Chart1(5.0, &rtDW.sf_Chart6);

  /* Chart: '<S3>/Chart7' incorporates:
   *  Constant: '<S3>/Constant6'
   */
  Chart1(6.0, &rtDW.sf_Chart7);

  /* Chart: '<S3>/Chart8' incorporates:
   *  Constant: '<S3>/Constant7'
   */
  Chart1(7.0, &rtDW.sf_Chart8);

  /* Chart: '<S3>/Chart9' incorporates:
   *  Constant: '<S3>/Constant8'
   */
  Chart1(8.0, &rtDW.sf_Chart9);
}

/* Model step function */
void SA4_EMS_step(void)
{
  /* Chart: '<Root>/Chart' */
  ChangeElec();

  /* Chart: '<Root>/Chart1' */
  DatLib();

  /* Outputs for Atomic SubSystem: '<Root>/Subsystem' */
  Charger();

  /* End of Outputs for SubSystem: '<Root>/Subsystem' */
}

/* Model initialize function */
void SA4_EMS_initialize(void)
{
  {
    int32_T i;

    /* Start for DataStoreMemory: '<Root>/MLocat_AllowCESoc_pct' */
    MLocat_AllowCESoc_pct = 80U;

    /* Start for DataStoreMemory: '<Root>/MLocat_CharEndSoc_pct' */
    MLocat_CharEndSoc_pct = 95U;

    /* Start for DataStoreMemory: '<Root>/MLocat_CharPow_kw' */
    MLocat_CharPow_kw = 5U;

    /* Start for DataStoreMemory: '<Root>/MLocat_Conf_ena' */
    for (i = 0; i < 20; i++) {
      MLocat_Conf_ena[i] = 1U;
    }

    /* End of Start for DataStoreMemory: '<Root>/MLocat_Conf_ena' */
  }
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
