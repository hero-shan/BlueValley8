#include "stdio.h"
#include "string.h"
#include "stdlib.h"

#include "unistd.h"
#include "sys/time.h"
#include "time.h"
#include "curl/curl.h"

#include "mymain.h"
#include "cloud_platform.h"

//车辆进站1/车辆放行2/车辆ODO3/取消订单4/入网认证11/各个时刻可预约的电池数量12/询问车型14
mid_netburst_t mid_netburst;

// unsigned char Staion_to_net_id[100] = "SA03 test12345";
// unsigned char Staion_to_net_pwd[100] = "12345678";
// unsigned char Staion_to_net_firmId[100] = "SA03 test12345";
unsigned char Staion_to_net_id[100] = "SA03test10";
unsigned char Staion_to_net_pwd[100] = "12345678";
unsigned char Staion_to_net_firmId[100] = "SA03test10";

unsigned char Cloud_Grab_carNo[30] ;//= "京Q6X256";/*抓拍机得到的车牌信息*/
unsigned char Cloud_Grab_carNo2[30] ;

unsigned char Cloud_Grab_carNo_cancel[30] ;//= "京Q6X256";/*抓拍机得到的车牌信息*/
unsigned char Cloud_orderId_now_cancel[50] ;
// unsigned char Cloud_Grab_carNo[30] = {0xe9,0xb2,0x81,66,68,0x33,0x30,0x36,0x36,0x30};
unsigned char Cloud_carType_now[50];
unsigned char Cloud_orderId_now[50] ;//= "00010033202010200002";/*云平台得到当前订单号*/
unsigned char Cloud_BatteryNo_now[100] ;//= "00MPE0JC431D0MA431007001";/*云平台得到电池编码*/
unsigned char Cloud_CarODO_now[30] = "65536";/*ODO*/
unsigned char Cloud_OutBatteryNo_now[100] ;//= "A010E00438J111400002EE00096769";
//grab 抓拍

// unsigned char Locat_BatteryNo[20][100];/*CHU读到的电池编码*/
unsigned char ChangeBatCar_BatteryNo[100];/*换电车辆电池编码*/
uint8_t Cloud_carType_Code;
uint8_t MCloud_SendmatchBatNo_buf[20];

char *url_LocalHost = "192.168.0.198:80";

uint8_t Cloud_SendGetCarType_num=0;
// /*换电站端内部电池数量和布局的对接接口url地址*/
// char *url_LayOutNum = "https://tswitcher.bj-abt.com/switcher-console/ems/powerPlantLayOutNum.htm";
/*换电站中各个时刻可预约的电池数量url地址*/
char *url_AvailableNum = "https://tswitcher.bj-abt.com/switcher-console/ems/powerPlantAvailableNum.htm";
//以下通信采用url末尾加参数形式
/*取消订单url地址*/
char *url_cancelOrder_base = "https://tswitcher.bj-abt.com/switcher-console/ems/cancelOrder.htm";
/**/
char *url_carOutOf_base = "https://tswitcher.bj-abt.com/switcher-console/ems/carOutOf.htm";
/**/
char *url_getDate_base = "https://tswitcher.bj-abt.com/switcher-console/ems/getDate.htm";
/*车辆ODO*/
char *url_carOdo_base = "https://tswitcher.bj-abt.com/switcher-console/ems/carOdo.htm";
/*车辆信息url*/
char *url_carInfo_base = "https://tswitcher.bj-abt.com/switcher-console/ems/carInfo.htm";
/*电池包溯源码是否匹配url*/
char *url_matchBatNo_base = "https://tswitcher.bj-abt.com/switcher-console/ems/checkBatteryNo.htm";
/*获取CarType  url*/
char *url_getCarType_base = "https://tswitcher.bj-abt.com/switcher-console/ems/carType.htm";
/*获取换电状态  url*/
char *url_CESta_base = "https://tswitcher.bj-abt.com/switcher-console/ems/orderState.htm";

/**/
char url_carOdo_buf[1000];
char url_cancelOrder_buf[1000];
char url_carOutOf_buf[1000];
char url_carInfo_buf[1000];
char url_getDate_buf[1000];
char url_matchBatNo_buf[1000];
char url_CESta_buf[1000];

unsigned char jsondata_AvailableNum_test[200];
// unsigned char jsondata_LayOutNum_test[200];

char result_result[20];
char result_error[50];
char result_carType[20];
char result_orderId[20];
char result_BatteryNo[50]; 

carInfo_t carInfo;
carOutOf_t carOutOf;
carOdo_t carOdo;
cancelOrder_t cancelOrder;
matchBatNo_t matchBatNo;
getDate_t getDate;
AvailableNum_t AvailableNum;
CESta_t CESta;
// LayOutNum_t LayOutNum;

uint8_t MLocat_InNet_num=0;
uint8_t MLocat_InNetOT_cou=0;

// unsigned char mdl_send_CarInfo_flg = 0;
// unsigned char mdl_send_CarOutOf_flg = 0;
// unsigned char mdl_send_CarOdo_flg = 0;
// unsigned char mdl_send_cancelOrder_flg = 0;
// unsigned char mdl_send_matchBatNo_flg = 1;

// unsigned char mdl_send_getDate_flg = 0;
unsigned char mdl_send_AvailableNum_flg = 0;
unsigned char mdl_send_LayOutNum_flg = 0;

unsigned char get_url_response_ok_flg;
unsigned char Http_Req_code = 0;

void InNet_Num_Deal(uint8_t value){
    if(MLocat_InNet_num>=17){
        MLocat_InNet_num = 0;
        MLocat_InNet_flg[MLocat_InNet_num-1] = value;
        MCloud_SendmatchBatNo_flg = 0;
        MLocat_CheckInNet_ok = 1;
    }
    else
    {
        MLocat_InNet_num++;
        MLocat_InNet_flg[MLocat_InNet_num-1] = value;
        MCloud_SendmatchBatNo_flg = 1;
    }
}
/*
*   使用不同的编码以区别是哪个请求返回的数据
*   0x01:车辆进站1
*   0x02:车辆放行2
*   0x03:车辆ODO
*   0x04:取消订单
*   0x05:
*   12:换电站中各个时刻可预约的电池数量
*   13:换电站端内部电池数量和布局的对接接口
*/
uint8_t result_err_Deal(char *result_err)
{
    if(strcmp(result_error,"FirmId empty")==0){
        return 2;           //firmId为空
    }
    else if(strcmp(result_error,"CarNo empty")==0){
        return 3;           //车牌号为空
    }
    else if(strcmp(result_error,"Carrier not exist")==0){
        return 4;
    }
    else if(strcmp(result_error,"Station not exist")==0){
        return 5;
    }
    else if(strcmp(result_error,"Car pack not exist")==0){
        return 6;
    }
    else if(strcmp(result_error,"Car model not exist")==0){
        return 7;
    }
    else if(strcmp(result_error,"Car not exist")==0){
        return 8;
    }
    else if(strcmp(result_error,"Latest order unfinish")==0){
        return 9;
    }
    else if(strcmp(result_error,"Car id is empty")==0){
        return 10;
    }
    else if(strcmp(result_error,"Wallet id is empty")==0){
        return 11;
    }
    else if(strcmp(result_error,"Car carrier id is empty")==0){
        return 12;
    }
    else if(strcmp(result_error,"Car pack is empty or trace id is empty")==0){
        return 13;
    }
    else if(strcmp(result_error,"Carrier order code is empty")==0){
        return 14;
    }
    else if(strcmp(result_error,"Rent station id is empty")==0){
        return 15;
    }
    else if(strcmp(result_error,"Station order code is empty")==0){
        return 16;
    }
    else if(strcmp(result_error,"Arrears of balance")==0){
        return 17;
    }
    else if(strcmp(result_error,"No valid carproduct")==0){
        return 18;
    }
    else if(strcmp(result_error,"No carproduct")==0){
        return 19;
    }
    else if(strcmp(result_error,"OrderId is empty")==0){
        return 20;
    }
    else if(strcmp(result_error,"New battery trace id is empty")==0){
        return 21;
    }
    else if(strcmp(result_error,"New pack not exist")==0){
        return 22;
    }
    else if(strcmp(result_error,"Order not exist")==0){
        return 23;
    }
    else if(strcmp(result_error,"Order doesn't match this station")==0){
        return 24;
    }
    else if(strcmp(result_error,"CarNo changed")==0){
        return 25;
    }
    else if(strcmp(result_error,"Old pack does not recover")==0){
        return 26;
    }
    else if(strcmp(result_error,"This pack is in renting Please check")==0){
        return 27;
    }
    else if(strcmp(result_error,"This pack is not in this station Please check")==0){
        return 28;
    }
    else if(strcmp(result_error,"Car ODO is empty")==0){
        return 29;
    }
    else if(strcmp(result_error,"Invalid car ODO")==0){
        return 30;
    }
    else if(strcmp(result_error,"Invalid upload ODO")==0){
        return 31;
    }
    else if(strcmp(result_error,"Invalid last order")==0){
        return 32;
    }
    else if(strcmp(result_error,"Invalid ODO")==0){
        return 33;
    }
    else if(strcmp(result_error,"Wallet not existed")==0){
        return 34;
    }
    else if(strcmp(result_error,"Invalid old record")==0){
        return 35;
    }
    else if(strcmp(result_error,"update wallet balance failed")==0){
        return 36;
    }
    else if(strcmp(result_error,"ODO is empty")==0){
        return 37;
    }
    else if(strcmp(result_error,"Order canceled")==0){
        return 38;
    }
    else if(strcmp(result_error,"Fail to update order state")==0){
        return 39;
    }
    else if(strcmp(result_error,"Order already paid")==0){
        return 40;
    }
    else if(strcmp(result_error,"Order exception")==0){
        return 41;
    }
    // else if(strcmp(result_error,"Order already canceled")==0){
    //     return 42;
    // }
    else if(strcmp(result_error,"Incorrect battery pack number")==0){
        return 43;
    }
    else if(strcmp(result_error,"ODO decrease")==0){
        return 44;
    }
    else if(strcmp(result_error,"Station is stoped")==0){
        return 45;
    }
    else
    {
        return 90;
    }
    
}
/* 回调函数， 一定要遵循回调函数的格式*/
static size_t httpCurlResCallback(void* buffer, size_t size, size_t nmemb, void* pUserArg)
{
    size_t len = 0;
 //   printf("%s; buffer = %s\n", __FILE__, (char *) buffer);
    len = size * nmemb;
    strcpy(result_result,"");
    strcpy(result_carType,"");
    strcpy(result_orderId,"");
    strcpy(result_BatteryNo,"");

    switch(Http_Req_code)
    {
     case 1:{

        syslog(LOG_INFO,"InStation buffer == %s\n",(char *)buffer);

        mid_netburst.MCloud_SendCarCard_flg = 0; 
        Seek_value(buffer,"\"result\"",result_result,sizeof result_result); 
 //       printf("result :%s\n",result_result);   
        Seek_value(buffer,"\"carType\"",result_carType,sizeof result_carType);
 //       printf("carType :%s\n",result_carType); 
        Seek_value(buffer,"\"orderId\"",result_orderId,sizeof result_orderId);
 //       printf("orderId :%s\n",result_orderId);
        Seek_value(buffer,"\"BatteryNo\"",result_BatteryNo,50); 
 //       printf("BatteryNo :%s\n",result_BatteryNo);
        Seek_value(buffer,"\"error\"",result_error,sizeof result_error); 
        //if((strlen(result_result) != 0)  && (result_carType[0] !='\0') && (result_orderId[0] !='\0') && (result_BatteryNo[0] !='\0'))
        {
            if(strcmp(result_result,"OK")==0){
                mid_netburst.MCloud_CarAllowIn_sta = 1;

                if(strcmp(result_carType,"EU300")==0){
                    mid_netburst.Cloud_carType_Code = 1;
                }
                else if(strcmp(result_carType,"EU5A")==0){
                    mid_netburst.Cloud_carType_Code = 2;
                }
                else if(strcmp(result_carType,"EU5B")==0){
                    mid_netburst.Cloud_carType_Code = 3;
                }
                else mid_netburst.Cloud_carType_Code = 0;
                strcpy(mid_netburst.Cloud_carType_now,result_carType);
                strcpy(Cloud_orderId_now,result_orderId);
                strcpy(mid_netburst.Cloud_BatteryNo_now,result_BatteryNo);
                strcpy(Cloud_OutBatteryNo_now,MLocat_BatteryNo[MChar_SelfCe_sour]);
            }
            else{
                mid_netburst.MCloud_CarAllowIn_sta=2;
                Qt_CarAllowIn_err = result_err_Deal(result_error);
            }   
        }

    }break;
    case 2:{
        syslog(LOG_INFO,"OutStation buffer == %s\n",(char *)buffer);

        mid_netburst.MCloud_SendCarOutOf_flg = 0;
        Seek_value(buffer,"\"result\"",result_result,sizeof result_result); 
        Seek_value(buffer,"\"error\"",result_error,sizeof result_error); 
  //      printf("result :%s\n",result_result);
        if(strcmp(result_result,"ODO")==0){

            mid_netburst.MCloud_CarOutReqODO_flg = 1;
        }
        else if(strcmp(result_result,"OK")==0||(strcmp(result_error,"Switch completed")==0)){
            mid_netburst.MCloud_CarAllowOut_sta = 1;
        }
        else if(strcmp(result_result,"NG")==0){
            mid_netburst.MCloud_CarAllowOut_sta = 2;
            Qt_CarAllowOut_err = result_err_Deal(result_error);
            // else if(strcmp(result_error,"Thispackisinrenting.Pleasecheck.")==0){
            //     mid_netburst.MCloud_CarAllowOut_sta=10;//该订单不存在
            // }
        }
    }break;  
    case 3:{
        syslog(LOG_INFO,"ODO buffer == %s\n",(char *)buffer);

        mid_netburst.MCloud_SendCarODO_flg = 0;
        Seek_value(buffer,"\"result\"",result_result,sizeof result_result); 
        Seek_value(buffer,"\"error\"",result_error,sizeof result_error); 

        if(strcmp(result_result,"OK")==0||
            strcmp(result_error,"ODO already upload")==0||
                strcmp(result_error,"Switch completed")==0){
            mid_netburst.MCloud_GetCarODO_flg = 1;   
        }      
        // else if(strcmp(result_error,"ODOdecrease")==0){
        //     mid_netburst.MCloud_CarOutReqODO_flg = 2;
        //     mid_netburst.MCloud_GetCarODO_flg = 0;
        // }
        else
        {
            mid_netburst.MCloud_CarAllowOut_sta = 2;
            Qt_CarODO_err = result_err_Deal(result_error);
        }
    }break;
    case 4:{
        syslog(LOG_INFO,"CancelOrd buffer == %s\n",(char *)buffer);


        mid_netburst.MCloud_SendCancelOrd_flg = 0;
        Seek_value(buffer,"\"result\"",result_result,sizeof result_result); 
        Seek_value(buffer,"\"error\"",result_error,sizeof result_error); 

        if((strcmp(result_result,"OK")==0)||(strcmp(result_error,"Order already canceled")==0)
            ||(strcmp(result_error,"Order finished")==0)){
            mid_netburst.MCloud_CancelOrd_ok = 1;  
   //         printf("Order already canceled\n");   
        }   
        // else if (strcmp(result_result,"NG")==0)
        else
        {
            // mid_netburst.MCloud_SendCancelOrd_flg = 1;
            Qt_CancelOrd_err = result_err_Deal(result_error);
        }
    }break;  
    case 11:{   //电池包溯源码
        syslog(LOG_INFO,"MLocat_InNet_num buffer == %s\n",(char *)buffer);

        Seek_value(buffer,"\"result\"",result_result,sizeof result_result); 
        if(strcmp(result_result,"OK")==0){
            mid_netburst.MLocat_InNet_flg[MLocat_InNet_num] = 1;
  //          printf("resultMLocat_InNet_numMLocat_InNet_num :%s,%d\n",result_result,MLocat_InNet_num);
            Qt_InNet_err[MLocat_InNet_num] = 0;
        }
        else if(strcmp(result_result,"NG")==0){
            mid_netburst.MLocat_InNet_flg[MLocat_InNet_num] = 0;
            Qt_InNet_err[MLocat_InNet_num] = 1;
        }
        if(MLocat_InNet_num<17)
            MLocat_InNet_num++;
        else 
            MLocat_InNet_num = 0; 

    }break; 
    case 12:{
        mid_netburst.mdl_send_AvailableNum_flg = 0;
    }break; 
    case 13:{
        mid_netburst.MCloud_CESta_sta = 0;
    }break; 
    case 14:{
        syslog(LOG_INFO,"GetcarType buffer == %s\n",(char *)buffer);

        Seek_value(buffer,"\"result\"",result_result,sizeof result_result); 
   //     printf("result :%s\n",result_result);     
        Seek_value(buffer,"\"carType\"",result_carType,sizeof result_carType);
   //     printf("carType :%s\n",result_carType); 
        Seek_value(buffer,"\"error\"",result_error,sizeof result_error); 

        if((strcmp(result_result,"OK")==0)  )//&& (result_carType[0] !='\0')
        {
            mid_netburst.MCloud_GetCarType_flg = 1;
            if(strcmp(result_carType,"EU300")==0){
                mid_netburst.Cloud_carType_Code = 1;
            }
            else if(strcmp(result_carType,"EU5A")==0){
                mid_netburst.Cloud_carType_Code = 2;
            }
            else if(strcmp(result_carType,"EU5B")==0){
                mid_netburst.Cloud_carType_Code = 3;
            }
            else mid_netburst.Cloud_carType_Code = 0;
        }
        else                    //"Car not exist"
        {
            if(Cloud_SendGetCarType_num==0){
                Cloud_SendGetCarType_num++;
                strcpy(mid_netburst.Cloud_Grab_carNo,mid_netburst.Cloud_Grab_carNo2);
                strcpy(Cloud_Grab_carNo,Cloud_Grab_carNo2);
                break;
            }
            mid_netburst.MCloud_GetCarType_flg = 2;
            mid_netburst.Cloud_carType_Code = 0;
            Qt_CarAllowIn_err = result_err_Deal(result_error);
        }
        mid_netburst.MCloud_SendGetCarType_flg = 0;

        // Appoint_GetCarType();
        // AppointOutNet_GetCarType();

    }break;      
    default:break;
    }
    return len;
}

// void Cloud_InterFace(){
//     int i=0;
//     static uint8_t InNet_count = 0;
//     if(InNet_count<3){
//         InNet_count++;
//     }
//     else InNet_count = 0;

//     if(InNet_count>=0){
//         for(i=0;i<18;i++)
//         {
//             if(MLocat_SelfChar_ena[i]==1&&MLocat_InNet_flg[i]==0&&
//                 strcmp(MLocat_BatteryNo[i],"")!=0){
//                 MCloud_SendmatchBatNo_buf[i] = 1;
//                 MCloud_SendmatchBatNo_flg=1;
//                 // break;
//             }
//             else MCloud_SendmatchBatNo_buf[i] = 0;
//         }
//     }
//     if(Sta!=21)
//         Cloud_SendGetCarType_num = 0;
// }

/*点发云端中间层对接函数*/
void mid_netBurst_Interface()
{
    if(MCloud_SendCarCard_flg)
    {
        mid_netburst.MCloud_SendCarCard_flg = MCloud_SendCarCard_flg;
        MCloud_SendCarCard_flg = 0;
    }
    if(MCloud_SendCarOutOf_flg)
    {
        mid_netburst.MCloud_SendCarOutOf_flg = MCloud_SendCarOutOf_flg;
        MCloud_SendCarOutOf_flg = 0;
    }
    if(MCloud_SendCarODO_flg)
    {
        mid_netburst.MCloud_SendCarODO_flg = MCloud_SendCarODO_flg;
        MCloud_SendCarODO_flg = 0;
    }
    if(MCloud_SendCancelOrd_flg)
    {
        mid_netburst.MCloud_SendCancelOrd_flg = MCloud_SendCancelOrd_flg;
        MCloud_SendCancelOrd_flg = 0;
    }
    if(MCloud_SendGetCarType_flg)
    {
        mid_netburst.MCloud_SendGetCarType_flg = MCloud_SendGetCarType_flg;
        MCloud_SendGetCarType_flg = 0;
    }
    if(mdl_send_AvailableNum_flg)
    {
        mid_netburst.mdl_send_AvailableNum_flg = mdl_send_AvailableNum_flg;
        mdl_send_AvailableNum_flg = 0;
    }
    if(MCloud_CESta_sta)
    {
        mid_netburst.MCloud_CESta_sta = MCloud_CESta_sta;
        MCloud_CESta_sta = 0;
    }
    if(mid_netburst.MCloud_CarAllowIn_sta)
    {
        Qt_CarAllowIn_LiftBar_sta = mid_netburst.MCloud_CarAllowIn_sta;  //2021-3-24 抬杆控制标志
        MCloud_CarAllowIn_sta = mid_netburst.MCloud_CarAllowIn_sta;
        mid_netburst.MCloud_CarAllowIn_sta = 0;
    }

    
    if(mid_netburst.Cloud_carType_Code)
    {
        Cloud_carType_Code = mid_netburst.Cloud_carType_Code;
        mid_netburst.Cloud_carType_Code = 0;
    }
    if(strcmp(mid_netburst.Cloud_carType_now,"")!=0){
        strcpy(Cloud_carType_now,mid_netburst.Cloud_carType_now);
        strcpy(mid_netburst.Cloud_carType_now,"");
    }
    // if(strcmp(mid_netburst.Cloud_orderId_now,"")!=0){
        strcpy(mid_netburst.Cloud_orderId_now,Cloud_orderId_now);
        // strcpy(mid_netburst.Cloud_orderId_now,"");
    // }
    if(strcmp(mid_netburst.Cloud_BatteryNo_now,"")!=0){
        strcpy(Cloud_BatteryNo_now,mid_netburst.Cloud_BatteryNo_now);
        strcpy(mid_netburst.Cloud_BatteryNo_now,"");
    }
    // if(strcmp(mid_netburst.Cloud_OutBatteryNo_now,"")!=0){
        strcpy(mid_netburst.Cloud_OutBatteryNo_now,Cloud_OutBatteryNo_now);//out
        // strcpy(mid_netburst.Cloud_OutBatteryNo_now,"");
    // }
        
    // if(Cloud_SendGetCarType_num)
    //   strcpy(Cloud_Grab_carNo,Cloud_Grab_carNo2);
    strcpy(mid_netburst.Cloud_Grab_carNo,Cloud_Grab_carNo);
    strcpy(mid_netburst.Cloud_Grab_carNo2,Cloud_Grab_carNo2);
    strcpy(mid_netburst.Cloud_orderId_now_cancel,Cloud_orderId_now_cancel);
    strcpy(mid_netburst.Cloud_Grab_carNo_cancel,Cloud_Grab_carNo_cancel);
    strcpy(mid_netburst.Staion_to_net_id,Staion_to_net_id);
    strcpy(mid_netburst.Staion_to_net_pwd,Staion_to_net_pwd);
    strcpy(mid_netburst.Staion_to_net_firmId,Staion_to_net_firmId);//in
    
    if(mid_netburst.MCloud_CarOutReqODO_flg)
    {
        MCloud_CarOutReqODO_flg = mid_netburst.MCloud_CarOutReqODO_flg;
        mid_netburst.MCloud_CarOutReqODO_flg = 0;
    }
    if(mid_netburst.MCloud_CarAllowOut_sta)
    {
        MCloud_CarAllowOut_sta = mid_netburst.MCloud_CarAllowOut_sta;
        mid_netburst.MCloud_CarAllowOut_sta = 0;
    }
    if(mid_netburst.MCloud_GetCarODO_flg)
    {
        MCloud_GetCarODO_flg = mid_netburst.MCloud_GetCarODO_flg;
        mid_netburst.MCloud_GetCarODO_flg = 0;
    }
    if(mid_netburst.MCloud_CancelOrd_ok)
    {
        MCloud_CancelOrd_ok = mid_netburst.MCloud_CancelOrd_ok;
        mid_netburst.MCloud_CancelOrd_ok = 0;
    }
    if(mid_netburst.MCloud_GetCarType_flg)
    {
        MCloud_GetCarType_flg = mid_netburst.MCloud_GetCarType_flg;
        mid_netburst.MCloud_GetCarType_flg = 0;
    }
    
    // Cloud_SendODO_km = mid_netburst.Cloud_SendODO_km;

    int i = 0;
    for(i=0;i<20;i++)
    {
        if(BATT_IsExist_flg[i]==1&&EMS_CHU_ComErr_flg[i]==0&&
			(CHU_DevState_code[i]>=3))
        {
            MLocat_InNet_flg[i] = mid_netburst.MLocat_InNet_flg[i];
            strcpy(MLocat_BatteryNo[i],BMS_BATT_NUM[i]);
            strcpy(mid_netburst.MLocat_BatteryNo[i],MLocat_BatteryNo[i]);
        }
        else
        {
            MLocat_InNet_flg[i] = 0;
            mid_netburst.MLocat_InNet_flg[i] = 0;

            // if(MSql_WriCeStartData_flg == 1 && MChar_SelfCe_sour == i)
            if(Sta>=30&&Sta<=33 && MChar_SelfCe_sour == i)
            {
                strcpy(sql_BattNum,BMS_BATT_NUM[MChar_SelfCe_sour]);
            }
            else
            {
                // bzero(BMS_BATT_NUM[i],40);
                // bzero(MLocat_BatteryNo[i],40);
                // bzero(mid_netburst.MLocat_BatteryNo[i],40);
                strcpy(BMS_BATT_NUM[i],"");
                strcpy(MLocat_BatteryNo[i],"");
                strcpy(mid_netburst.MLocat_BatteryNo[i],"");
            }
        }
    }

    static uint8_t InNet_count = 0;
    if(InNet_count<3){
        InNet_count++;
    }
    else InNet_count = 0;
    if(InNet_count>=0){
        for(i=0;i<18;i++)
        {
            if(MLocat_SelfChar_ena[i]==1&&MLocat_InNet_flg[i]==0&&
                strcmp(MLocat_BatteryNo[i],"")!=0){
                MCloud_SendmatchBatNo_buf[i] = 1;
                MCloud_SendmatchBatNo_flg=1;
                // break;
            }
            else MCloud_SendmatchBatNo_buf[i] = 0,Qt_InNet_err[i] = 0;
        }
    }
    if(Sta!=21)
        Cloud_SendGetCarType_num = 0;
}
/**/
int pthread_net_burst(void)
{  
//    printf("pthread_net_burst tid %d\n",gettid());
    syslog(LOG_INFO,"pthread_net_burst gettid '%d'\n",gettid());

    CURLcode curlRet = 0;
    CURL* pHandle_burst = NULL;
    long *pHttpCode;
    char pRes[20000]= {0};

    FILE* fptr;

    /*写入curl的url 和json 缓存*/
    char *url;
    char *jsondata = "";
    // Cloud_SendODO_km = 65536;
    // MCloud_SendGetCarType_flg = 1;
    // mdl_send_LayOutNum_flg = 1;
    // strcpy(Cloud_Grab_carNo,"浙BD36001"),strcpy(Cloud_Grab_carNo2,"鲁BD36001");
    while(1)  
    {   
        while(1)
        {
            // printf("sasbbb %d\n",gettid());
            	// char * test;
            	// strcpy(test,0);
            // MCloud_SendCancelOrd_flg = 1;
            // strcpy(Cloud_Grab_carNo_cancel,"鲁BD36001");
            // strcpy(Cloud_orderId_now_cancel,"00010042202012170009");
            // MCloud_SendCarOutOf_flg = 1;
            // MCloud_SendCarODO_flg=1;
            usleep(500 * 1000);
            mid_netBurst_Interface();

            // Cloud_InterFace();
            if(1 == mid_netburst.MCloud_SendCarCard_flg)//车辆进站1
            {
                // MCloud_SendCarCard_flg = 0;
                syslog(LOG_INFO,"SEND InStation buffer\n");

                carInfo.id = mid_netburst.Staion_to_net_id;
                carInfo.pwd = mid_netburst.Staion_to_net_pwd;
                carInfo.firmId = mid_netburst.Staion_to_net_firmId;
                carInfo.carNo = mid_netburst.Cloud_Grab_carNo;
                // carInfo.carNo = "";
                url_carInfo_build(carInfo);

                syslog(LOG_INFO,"车辆进站send carInfo == %s\n",(char *)url_carInfo_buf);

                url = url_carInfo_buf;
                Http_Req_code = 1;
                // zero(jsondata);
                break; 
            } 
            else if(1 == mid_netburst.MCloud_SendCarOutOf_flg)//车辆放行2
            {
                // MCloud_SendCarOutOf_flg = 0;

            
                carOutOf.id = mid_netburst.Staion_to_net_id;
                carOutOf.pwd = mid_netburst.Staion_to_net_pwd;
                carOutOf.firmId = mid_netburst.Staion_to_net_firmId;
                carOutOf.carNo = mid_netburst.Cloud_Grab_carNo;
                // carOutOf.orderId = "";
                carOutOf.orderId = mid_netburst.Cloud_orderId_now;
                carOutOf.newBatteryNo = mid_netburst.Cloud_OutBatteryNo_now;
                url_carOutOf_build(carOutOf);

                syslog(LOG_INFO,"车辆放行send carOutOf == %s\n",(char *)url_carOutOf_buf);

                url = url_carOutOf_buf;
                Http_Req_code = 2;
                break; 
            }
            else if(1 == mid_netburst.MCloud_SendCarODO_flg)//车辆ODO mdl_send_CarOdo_flg
            {
                // MCloud_SendCarODO_flg = 0;
                
                carOdo.id = mid_netburst.Staion_to_net_id;
                carOdo.pwd = mid_netburst.Staion_to_net_pwd;
                carOdo.firmId = mid_netburst.Staion_to_net_firmId;
                carOdo.carNo = mid_netburst.Cloud_Grab_carNo;
                carOdo.orderId = mid_netburst.Cloud_orderId_now;
                carOdo.newBatteryNo = mid_netburst.Cloud_OutBatteryNo_now;
                // carOdo.newBatteryNo = "00MPE0JC431D0MA431007001";
                sprintf(Cloud_CarODO_now,"%d",(int)(Cloud_SendODO_km));
                carOdo.ODO = Cloud_CarODO_now;
                url_carOdo_build(carOdo);

                syslog(LOG_INFO,"车辆ODOsend carOdo == %s\n",(char *)url_carOdo_buf);

                url = url_carOdo_buf;
                Http_Req_code = 3;
                break; 
            } 
            else if(1 == mid_netburst.MCloud_SendCancelOrd_flg)//取消订单
            {
                // MCloud_SendCancelOrd_flg = 0;
                
                cancelOrder.id = mid_netburst.Staion_to_net_id;
                cancelOrder.pwd = mid_netburst.Staion_to_net_pwd;
                cancelOrder.firmId = mid_netburst.Staion_to_net_firmId;
                cancelOrder.carNo = mid_netburst.Cloud_Grab_carNo_cancel;
                cancelOrder.orderId = mid_netburst.Cloud_orderId_now_cancel;
                url_cancelOrder_build(cancelOrder,1);


                syslog(LOG_INFO,"取消订单send cancelOrder == %s\n",(char *)url_cancelOrder_buf);
                url = url_cancelOrder_buf;
                Http_Req_code = 4;
                break; 
            }
            else if(1 == mid_netburst.mdl_send_AvailableNum_flg)//换电站中各个时刻可预约的电池数量 qt
            {
                // mdl_send_AvailableNum_flg = 0;

                AvailableNum.id = mid_netburst.Staion_to_net_id;
                AvailableNum.pwd = mid_netburst.Staion_to_net_pwd;
                AvailableNum.firmId = mid_netburst.Staion_to_net_firmId;
                AvailableNum.carType1="EU300";
                AvailableNum.carType2="EU5A";
                
                json_AvailableNum_build(AvailableNum,jsondata_AvailableNum_test);

                url = url_AvailableNum;
                jsondata = jsondata_AvailableNum_test;
                Http_Req_code = 12;
                break;

            }
            else if(0 != mid_netburst.MCloud_CESta_sta)//换电状态更新 
            {
                // mdl_send_LayOutNum_flg = 0;
                
                CESta.id = mid_netburst.Staion_to_net_id;
                CESta.pwd = mid_netburst.Staion_to_net_pwd;
                CESta.firmId = mid_netburst.Staion_to_net_firmId;
                CESta.orderId = mid_netburst.Cloud_orderId_now;
                CESta.orderState = mid_netburst.MCloud_CESta_sta;

                url_CESta_build(CESta);
                url = url_CESta_buf;
                Http_Req_code = 13;
                break;
            }
            else if(1 == mid_netburst.MCloud_SendGetCarType_flg)//询问车型
            {
                // MCloud_SendGetCarType_flg = 0;
                
                
                cancelOrder.id = mid_netburst.Staion_to_net_id;
                cancelOrder.pwd = mid_netburst.Staion_to_net_pwd;
                cancelOrder.firmId = mid_netburst.Staion_to_net_firmId;
                cancelOrder.carNo = mid_netburst.Cloud_Grab_carNo;
                url_cancelOrder_build(cancelOrder,2);

                url = url_cancelOrder_buf;
                Http_Req_code = 14;
                syslog(LOG_INFO,"询问车型send cancelOrder == %s\n",(char *)url_cancelOrder_buf);
                break; 
            }
            else if(1 == MCloud_SendmatchBatNo_flg)//检查车辆VIN和电池包溯源码是否匹配的接口
            {
                int i=0;
                MCloud_SendmatchBatNo_flg = 0;
                matchBatNo.id = mid_netburst.Staion_to_net_id;
                matchBatNo.pwd = mid_netburst.Staion_to_net_pwd;
                matchBatNo.firmId = mid_netburst.Staion_to_net_firmId;
                while (1)
                {
                    if(MCloud_SendmatchBatNo_buf[MLocat_InNet_num]==1)
                    {
                       break;
                    }
                    else 
                    {
                        if(MLocat_InNet_num<17)
                            MLocat_InNet_num++;
                        else {MLocat_InNet_num = 0;break; }
                    }
   //                 printf("MLocat_InNet_num=== %d",MLocat_InNet_num);
                }
                matchBatNo.batteryNo = mid_netburst.MLocat_BatteryNo[MLocat_InNet_num];
                url_matchBatNo_build(matchBatNo);

                url = url_matchBatNo_buf;
                Http_Req_code = 11;
                break; 
            }
            else
            {
                Http_Req_code = 0;
            }                      
        }   
        /*执行url解析与通信*/
        do{         
            /* 全局初始化 */
            // curlRet = curl_global_init(CURL_GLOBAL_SSL|CURL_GLOBAL_ALL );
            // if(curlRet != 0)
            // {
            //     printf("curl_global_init() error. curlRet = %d\n", curlRet);
            //     // break;
            // }
            pthread_mutex_lock(&mutex_lock);
            
            struct curl_slist* headers = NULL;
            char headers_tmp[100] = {0};
            /*设置url头部信息*/
            snprintf(headers_tmp, sizeof(headers_tmp), "Content-Length: %d", (int)strlen(jsondata));
            headers = curl_slist_append(headers, "Accept: application/json");
            headers = curl_slist_append(headers, "Content-Type: application/json");
            headers = curl_slist_append(headers, "charset: utf-8");
            headers = curl_slist_append(headers, headers_tmp);
            
            /* 初始化libcurl会话 */
            pHandle_burst = curl_easy_init();
            if(NULL == pHandle_burst)
            {
     //           printf("curl_easy_init()error.\n");
                break;
            }

            /* 设置连接超时时间 2秒*/
            curlRet = curl_easy_setopt(pHandle_burst, CURLOPT_CONNECTTIMEOUT, 2);
            if(curlRet != 0)
            {
     //           printf("curl_easy_setopt() error! curlRet = %d\n", curlRet);
                break;
            }

            /* 设置超时时间 5秒*/
            curlRet = curl_easy_setopt(pHandle_burst, CURLOPT_TIMEOUT, 8);
            if(curlRet != 0)
            {
     //           printf("curl_easy_setopt() error! curlRet = %d\n", curlRet);
                break;
            }

            /* 设置url*/
            curlRet = curl_easy_setopt(pHandle_burst, CURLOPT_URL, url);
            if(curlRet != 0)
            {
     //           printf("curl_easy_setopt() error. curlRet = %d\n", curlRet);
                break;
            }
            curlRet = curl_easy_setopt(pHandle_burst, CURLOPT_HTTPHEADER, headers);
                if(curlRet != 0)
            {
    //            printf("curl_easy_setopt() error. curlRet = %d\n", curlRet);
                break;
            }
            curlRet = curl_easy_setopt(pHandle_burst, CURLOPT_POST, 1L);
            if(curlRet != 0)
            {
     //           printf("curl_easy_setopt() error. curlRet = %d\n", curlRet);
                break;
            }
            curlRet =curl_easy_setopt(pHandle_burst, CURLOPT_SSL_VERIFYPEER, 0);
            if(curlRet != 0)
            {
     //           printf("curl_easy_setopt() error. curlRet = %d\n", curlRet);
                break;
            }
            curlRet = curl_easy_setopt(pHandle_burst, CURLOPT_POSTFIELDS, jsondata);
            if(curlRet != 0)
            {
     //           printf("curl_easy_setopt() error. curlRet = %d\n", curlRet);
                break;
            }
            //curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
            curlRet = curl_easy_setopt(pHandle_burst, CURLOPT_WRITEDATA, fptr);
            if(curlRet != 0)
            {
     //           printf("curl_easy_setopt() error. curlRet = %d\n", curlRet);
                break;
            }
            //curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);   //debug 
            /* 设置 回调函数 */
            curlRet = curl_easy_setopt(pHandle_burst, CURLOPT_WRITEFUNCTION, httpCurlResCallback);
            if(curlRet != 0)
            {
     //           printf("curl_easy_setopt() error. curlRet = %d\n", curlRet);
                break;
            }

     //       printf("there is safe !\n");

            /*传入回调函数需要的结构体的指针 */
            curlRet = curl_easy_setopt(pHandle_burst, CURLOPT_WRITEDATA, (void *)pRes);
            if(curlRet != 0)
            {
     //           printf("curl_easy_setopt() error. curlRet = %d line = %d\n", curlRet,__LINE__);
                break;
            }
            curlRet = curl_easy_setopt(pHandle_burst, CURLOPT_NOSIGNAL, 1);
            if(curlRet != 0)
            {
    //            printf("curl_easy_setopt() error. curlRet = %d\n", curlRet);
                break;
            }

            curlRet = curl_easy_perform(pHandle_burst);
            if(curlRet != 0)
            {
                if(MLocat_InNetOT_cou<3){
                    MLocat_InNetOT_cou++;
                }
                else{
                    MLocat_InNetOT_cou = 0;
                    // InNet_Num_Deal(0);
                }
     //           printf("curl_easy_perform() error. curlRet = %d line = %d  \n", curlRet,__LINE__);
                // 释放headers
                curl_slist_free_all(headers);
                // 释放curl对象
                curl_easy_cleanup(pHandle_burst);

                pthread_mutex_unlock(&mutex_lock); 
                    // 释放全局curl对象
                // curl_global_cleanup();
                break;
            }
            get_url_response_ok_flg = 1;
            // 释放headers
            curl_slist_free_all(headers);
            // 释放curl对象
            curl_easy_cleanup(pHandle_burst);
            pthread_mutex_unlock(&mutex_lock); 
                // 释放全局curl对象
            // curl_global_cleanup();

        }while(0);

        // 释放curl对象
        // curl_easy_cleanup(pHandle_burst);
    
    }
    //释放全局curl对象
    // curl_global_cleanup();

}

// 换电站中各个时刻可预约的电池数量
void json_AvailableNum_build(AvailableNum_t AvailableNum,unsigned char *buf)
{
    char temp[500],i=0;
    uint8_t num[3][50];
    for(i=0;i<24;i++)
    {
        num[0][i] = QtBook_BatNax_num[0][i];
        num[1][i] = QtBook_BatNax_num[1][i];
        num[2][i] = QtBook_BatNax_num[2][i];
    }
    sprintf(temp,"{\"id\" : \"%s\",\
                    \"pwd\":\"%s\",\
                    \"firmId\":\"%s\",\
                    \"data\":[\
                    {\"carType\":\"%s\",\
                        \"count\":[%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d]\
                    },\
                    {\"carType\":\"%s\",\
                        \"count\":[%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d]\
                    }\
                    ]}",AvailableNum.id,AvailableNum.pwd,AvailableNum.firmId,AvailableNum.carType1,\
                        num[0][0],num[0][1],num[0][2],num[0][3],num[0][4],num[0][5],num[0][6],num[0][7],num[0][8],num[0][9],num[0][10],num[0][11],\
                        num[0][12],num[0][13],num[0][14],num[0][15],num[0][16],num[0][17],num[0][18],num[0][19],num[0][20],num[0][21],num[0][22],num[0][23],\
                        AvailableNum.carType2,\
                        num[1][0],num[1][1],num[1][2],num[1][3],num[1][4],num[1][5],num[1][6],num[1][7],num[1][8],num[1][9],num[1][10],num[1][11],\
                        num[1][12],num[1][13],num[1][14],num[1][15],num[1][16],num[1][17],num[1][18],num[1][19],num[1][20],num[1][21],num[1][22],num[1][23]\
            );
    strcpy(buf,temp);            
 //   printf("powerPlantAvailableNumaaaaaa = %s\n",buf);
}

//换电站端内部电池数量和布局的对接接口
void json_LayOutNum_build(LayOutNum_t LayOutNum,unsigned char *buf)
{
    char temp[500];

    //换电站端内部电池数量和布局的对接接口
    sprintf(temp,"{  \"id\" : \"%s\",\
                    \"pwd\":\"%s\",\
                    \"firmId\":\"%s\",\
                    \"data\":[\
                    {\"carType\":\"%s\",\"number\":%d,\"socPercent\":[%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d],\"avaCount\":%d},\
                    {\"carType\":\"%s\",\"number\":%d,\"socPercent\":[%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d],\"avaCount\":%d}\
                    ]}",LayOutNum.id,LayOutNum.pwd,LayOutNum.firmId,\
                         LayOutNum.carType1,LayOutNum.carType_number[0],\
                         LayOutNum.carType_socPercent[0][0],LayOutNum.carType_socPercent[0][1],LayOutNum.carType_socPercent[0][2],\
                         LayOutNum.carType_socPercent[0][3],LayOutNum.carType_socPercent[0][4],LayOutNum.carType_socPercent[0][5],\
                         LayOutNum.carType_socPercent[0][6],LayOutNum.carType_socPercent[0][7],LayOutNum.carType_socPercent[0][8],\
                         LayOutNum.carType_socPercent[0][9],LayOutNum.carType_socPercent[0][10],LayOutNum.carType_socPercent[0][11],\
                         LayOutNum.carType_socPercent[0][12],LayOutNum.carType_socPercent[0][13],LayOutNum.carType_socPercent[0][14],\
                         LayOutNum.carType_socPercent[0][15],LayOutNum.carType_socPercent[0][16],LayOutNum.carType_socPercent[0][17],\
                         LayOutNum.carType_socPercent[0][18],LayOutNum.carType_socPercent[0][19],\
                         LayOutNum.carType_avaCount[0],\
                         LayOutNum.carType2,LayOutNum.carType_number[1],\
                         LayOutNum.carType_socPercent[1][0],LayOutNum.carType_socPercent[1][1],LayOutNum.carType_socPercent[1][2],\
                         LayOutNum.carType_socPercent[1][3],LayOutNum.carType_socPercent[1][4],LayOutNum.carType_socPercent[1][5],\
                         LayOutNum.carType_socPercent[1][6],LayOutNum.carType_socPercent[1][7],LayOutNum.carType_socPercent[1][8],\
                         LayOutNum.carType_socPercent[1][9],LayOutNum.carType_socPercent[1][10],LayOutNum.carType_socPercent[1][11],\
                         LayOutNum.carType_socPercent[1][12],LayOutNum.carType_socPercent[1][13],LayOutNum.carType_socPercent[1][14],\
                         LayOutNum.carType_socPercent[1][15],LayOutNum.carType_socPercent[1][16],LayOutNum.carType_socPercent[1][17],\
                         LayOutNum.carType_socPercent[1][18],LayOutNum.carType_socPercent[1][19],\
                         LayOutNum.carType_avaCount[1]);
                        //  LayOutNum.carType3,LayOutNum.carType_number[2],\
                        //  LayOutNum.carType_socPercent[2][0],LayOutNum.carType_socPercent[2][1],LayOutNum.carType_socPercent[2][2],\
                        //  LayOutNum.carType_socPercent[2][3],LayOutNum.carType_socPercent[2][4],LayOutNum.carType_socPercent[2][5],\
                        //  LayOutNum.carType_socPercent[2][6],LayOutNum.carType_socPercent[2][7],LayOutNum.carType_socPercent[2][8],\
                        //  LayOutNum.carType_socPercent[2][9],LayOutNum.carType_socPercent[2][10],LayOutNum.carType_socPercent[2][11],\
                        //  LayOutNum.carType_socPercent[2][12],LayOutNum.carType_socPercent[2][13],LayOutNum.carType_socPercent[2][14],\
                        //  LayOutNum.carType_socPercent[2][15],LayOutNum.carType_socPercent[2][16],LayOutNum.carType_socPercent[2][17],\
                        //  LayOutNum.carType_socPercent[2][18],LayOutNum.carType_socPercent[2][19],\
                        //  LayOutNum.carType_avaCount[2]);
    strcpy(buf,temp); 
 //   printf("bufaaaaaaa = %s\n",buf);

}


/*将汉字（UTF8）转成URL可识别的编码*/
int URLEncode(const char* str, const int strSize, char* result, const int resultSize)
{
    int i;
    int j = 0;//for result index
    char ch;
 
    if ((str==NULL) || (result==NULL) || (strSize<=0) || (resultSize<=0)) {
        return 0;
    }
 
    for ( i=0; (i<strSize)&&(j<resultSize); ++i) {
        ch = str[i];
        if (((ch>='A') && (ch<='Z')) ||
            ((ch>='a') && (ch<='z')) ||
            ((ch>='0') && (ch<='9'))) {
            result[j++] = ch;
        } else if (ch == ' ') {
            result[j++] = '+';
        } else if (ch == '.' || ch == '-' || ch == '_' || ch == '*') {
            result[j++] = ch;
        } else {
            if (j+3 < resultSize) {
                sprintf(result+j, "%%%02X", (unsigned char)ch);
                j += 3;
            } else {
                return 0;
            }
        }
    }
 
    result[j] = '\0';
    return j;
}


/*从云端返回结果中，从中提取值*/
unsigned char Seek_value(char * buf,char *goal_buf,char *result_buf, int result_buf_len)
{
    // printf("in\n");
    int buf_len = 0;
    int goal_buf_len = 0;
    int len2;

    char * location = NULL;
    int i,j = 0;
    char temp[100];/*存储键值的缓存*/

    buf_len = strlen(buf);
    goal_buf_len = strlen(goal_buf);

    /*需要查找的源字符串为空，或者第一个串小于第二个，直接返回*/
    if(buf_len == 0 || buf_len < goal_buf_len)
    {
        result_buf[0] = '\0';
        return 0;
    }
    // printf("len of buf %d\n",buf_len);
    location = strstr(buf,goal_buf);

    if(location == NULL)
    {
        result_buf[0] = '\0';
        return 0;
    }
    /*将指针移到目标串右方*/
    location += strlen(goal_buf);
    /*除去目标字符串及其左方字符串后的长度*/
    len2 = strlen(location);

// {"result":"OK","carType":"EU300","orderId":"00010016202008130001","BatteryNo":"00MPE0JC431D0MA431049001"}
    for(i = 0;i < len2;i++)
    {
        if(*location == '\0' || *location == ','|| *location == '}')
        {
            break;
        }
        else if( ((*location>='A') && (*location<='Z')) ||
            ((*location>='a') && (*location<='z')) ||
            ((*location>='0') && (*location<='9')) ||
            (*location==32))
        {
            temp[j++] = *location;
        }
        else if(*location & 0x80)
        {
            temp[j++] = *location;
        }
        location++;
    }
    temp[j] = '\0';
    if( strlen(temp) <= result_buf_len)
    {
        strcpy(result_buf,temp);
    }  
    else
    {
        return 0;
    }
    return 1;
}


/*取消订单url构建函数*/
void url_cancelOrder_build(cancelOrder_t cancelOrder,char num)
{
    char carNo_Obj_buf[100] = {0};
    unsigned char temp[200] = {0};

    if(num==1)
        strcpy(url_cancelOrder_buf,url_cancelOrder_base);
    else if(num==2)
        strcpy(url_cancelOrder_buf,url_getCarType_base);
    sprintf(temp,"?id=%s",cancelOrder.id);
    strcat(url_cancelOrder_buf,temp);

    sprintf(temp,"&pwd=%s",cancelOrder.pwd);
    strcat(url_cancelOrder_buf,temp);    

    sprintf(temp,"&firmId=%s",cancelOrder.firmId);
    strcat(url_cancelOrder_buf,temp); 

    URLEncode(cancelOrder.carNo, strlen(cancelOrder.carNo), carNo_Obj_buf, 100);
    sprintf(temp,"&carNo=%s",carNo_Obj_buf);
    strcat(url_cancelOrder_buf,temp); 

    if(num==1){
    sprintf(temp,"&orderId=%s",cancelOrder.orderId);
    strcat(url_cancelOrder_buf,temp); 
    }


    // printf("%s\n",url_cancelOrder);
}

/*车辆信息url构建函数*/
void url_carInfo_build(carInfo_t carInfo)
{
    char carNo_Obj_buf[100] = {0};
    unsigned char temp[200] = {0};
    // zero(url_carInfo);
    strcpy(url_carInfo_buf,url_carInfo_base);

    sprintf(temp,"?id=%s",carInfo.id);
    strcat(url_carInfo_buf,temp);

    sprintf(temp,"&pwd=%s",carInfo.pwd);
    strcat(url_carInfo_buf,temp);    

    sprintf(temp,"&firmId=%s",carInfo.firmId);
    strcat(url_carInfo_buf,temp); 

    URLEncode(carInfo.carNo, strlen(carInfo.carNo), carNo_Obj_buf, 100);
    sprintf(temp,"&carNo=%s",carNo_Obj_buf);
    strcat(url_carInfo_buf,temp); 
 //   printf("%s\n",url_carInfo_buf);
}

/*匹配电池包溯源码*/
void url_matchBatNo_build(matchBatNo_t matchBatNo)
{
    char carNo_Obj_buf[100] = {0};
    unsigned char temp[200] = {0};
    // zero(url_carInfo);
    strcpy(url_matchBatNo_buf,url_matchBatNo_base);

    sprintf(temp,"?id=%s",matchBatNo.id);
    strcat(url_matchBatNo_buf,temp);
    sprintf(temp,"&pwd=%s",matchBatNo.pwd);
    strcat(url_matchBatNo_buf,temp);    
    sprintf(temp,"&firmId=%s",matchBatNo.firmId);
    strcat(url_matchBatNo_buf,temp); 

    sprintf(temp,"&batteryNo=%s",matchBatNo.batteryNo);
    strcat(url_matchBatNo_buf,temp); 

 //   printf("%s\n",url_matchBatNo_buf);
}
/*换电状态更新*/
void url_CESta_build(CESta_t CESta)
{
    // char carNo_Obj_buf[100] = {0};
    unsigned char temp[200] = {0};
    // zero(url_carInfo);
    strcpy(url_CESta_buf,url_CESta_base);

    sprintf(temp,"?id=%s",CESta.id);
    strcat(url_CESta_buf,temp);
    sprintf(temp,"&pwd=%s",CESta.pwd);
    strcat(url_CESta_buf,temp);    
    sprintf(temp,"&firmId=%s",CESta.firmId);
    strcat(url_CESta_buf,temp); 
    sprintf(temp,"&orderId=%s",CESta.orderId);
    strcat(url_CESta_buf,temp); 
    sprintf(temp,"&orderState=%d",CESta.orderState);
    strcat(url_CESta_buf,temp); 

 //   printf("%s\n",url_CESta_buf);
}

//时间校对
void url_getDate_build(getDate_t getDate)
{

    char carNo_Obj_buf[100] = {0};
    unsigned char temp[200] = {0};

    strcpy(url_getDate_buf,url_getDate_base);

    sprintf(temp,"?id=%s",getDate.id);
    strcat(url_getDate_buf,temp);

    sprintf(temp,"&pwd=%s",getDate.pwd);
    strcat(url_getDate_buf,temp);    

    sprintf(temp,"&firmId=%s",getDate.firmId);
    strcat(url_getDate_buf,temp); 

    // printf("%s\n",url_getDate_buf);
}


/*车辆进站放行url构建函数*/
void url_carOutOf_build(carOutOf_t carOutOf)
{
    char carNo_Obj_buf[100] = {0};
    unsigned char temp[200] = {0};

    strcpy(url_carOutOf_buf,url_carOutOf_base);

    sprintf(temp,"?id=%s",carOutOf.id);
    strcat(url_carOutOf_buf,temp);

    sprintf(temp,"&pwd=%s",carOutOf.pwd);
    strcat(url_carOutOf_buf,temp);    

    sprintf(temp,"&firmId=%s",carOutOf.firmId);
    strcat(url_carOutOf_buf,temp); 

    /**/
    URLEncode(carOutOf.carNo, strlen(carOutOf.carNo), carNo_Obj_buf, 100);
    sprintf(temp,"&carNo=%s",carNo_Obj_buf);
    strcat(url_carOutOf_buf,temp); 

    sprintf(temp,"&orderId=%s",carOutOf.orderId);
    strcat(url_carOutOf_buf,temp); 

    sprintf(temp,"&newBatteryNo=%s",carOutOf.newBatteryNo);
    strcat(url_carOutOf_buf,temp); 

  //  printf("%s\n",url_carOutOf_buf);
}

/*车辆ODO url构建函数*/
void url_carOdo_build(carOdo_t carOdo)
{
    char carNo_Obj_buf[100] = {0};
    unsigned char temp[200] = {0};

    strcpy(url_carOdo_buf,url_carOdo_base);

    sprintf(temp,"?id=%s",carOdo.id);
    strcat(url_carOdo_buf,temp);

    sprintf(temp,"&pwd=%s",carOdo.pwd);
    strcat(url_carOdo_buf,temp);    

    sprintf(temp,"&firmId=%s",carOdo.firmId);
    strcat(url_carOdo_buf,temp); 

    URLEncode(carOdo.carNo, strlen(carOdo.carNo), carNo_Obj_buf, 100);
    sprintf(temp,"&carNo=%s",carNo_Obj_buf);
    strcat(url_carOdo_buf,temp); 

    sprintf(temp,"&orderId=%s",carOdo.orderId);
    strcat(url_carOdo_buf,temp); 

    sprintf(temp,"&newBatteryNo=%s",carOdo.newBatteryNo);
    strcat(url_carOdo_buf,temp); 

    sprintf(temp,"&ODO=%s",carOdo.ODO);
    strcat(url_carOdo_buf,temp); 
 //   printf("odo=%s\n",url_carOdo_buf);
}




