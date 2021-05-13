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

//发送数据1/预约查询2/获取时间3/电池数量和布局的对接接口5


#define CAR_TYPE_NUM    3
#define CAR_TYPE_NAME_LEN   100

mid_netcycle_t mid_netcycle;

LayOutNum_t LayOutNum = {};
unsigned char jsondata_LayOutNum_test[200];
uint8_t Lay_SocPercent_buf[20];


char car_type[CAR_TYPE_NUM][CAR_TYPE_NAME_LEN] = {
    "EU300",
    "EU260"
    "EU5"
};

char zong_buf[10000] = {'\0'};
char zong2_buf[10000] = {'\0'};
json_t json_test;


/*json_build function
*
*/

/*换电站端内部电池数量和布局的对接接口url地址*/
char *url_LayOutNum = "https://tswitcher.bj-abt.com/switcher-console/ems/powerPlantLayOutNum.htm";
char *url_appointment_base = "https://tswitcher.bj-abt.com/switcher-console/ems/appointmentList.htm";
// char *url_appointment = "https://tswitcher.bj-abt.com/switcher-console/ems/appointmentList.htm?id=SA03%20test12345&pwd=12345678&firmId=SA03%20test12345&appointStartTime=2020-09-05%2018:00:00&appointEndTime=2020-09-05%2019:00:00";
// char *url_sendData = "https://tswitcher.bj-abt.com/switcher-console/ems/queueNumber.htm";


// char *url_sendData = "https://tswitcher.bj-abt.com/switcher-console/ems/sendData.htm";
char *url_getDate = "https://tswitcher.bj-abt.com/switcher-console/ems/getDate.htm?id=SA03%20test12345&pwd=12345678&firmId=SA03%20test12345";

appointment_t appointment1;
appointment_t appointment2;
// int MCloud_Comm_flg;
// int Cloud_Comm_err_flg;
unsigned char Cloud_Comm_err_count = 0;
unsigned char Cloud_Comm_cycle_index = 0;
unsigned char url_send_index = 1;

unsigned char now_Time_Cloud[30] = {'\0'};
uint8_t url_appointment_buf[200];
uint8_t Cloud_BookcarNo1[20][100],Cloud_BookcarNo2[20][100],Cloud_BookcarNo3[20][100];
uint8_t Clear_BookcarNo1[20][100],Clear_BookcarNo2[20][100],Clear_BookcarNo3[20][100];
uint8_t Cloud_BookcarType[30];
uint8_t Cloud_Bookcount=0;
uint8_t Book_Init_cou=0;

//时间校对
void url_appointment_build(appointment_t appointment)
{
    unsigned char temp[200] = {0};

    strcpy(url_appointment_buf,url_appointment_base);

    sprintf(temp,"?id=%s&pwd=%s&firmId=%s",appointment.id,appointment.pwd,appointment.firmId);
    strcat(url_appointment_buf,temp);

    sprintf(temp,"&appointStartTime=%s&appointEndTime=%s",appointment.starttime,appointment.endtime);
    strcat(url_appointment_buf,temp); 

//    printf("%s\n",url_appointment_buf);
}

uint8_t plate[5][30];

int GetCatPlate(uint8_t *buf,int len)
{
    if(len == 0)
    {
        return 0;
    }
//    printf("len = %d\n",len);
 //   printf("buf = %s\n",buf);
    int i,j=0;
    int Car_index = 0;

    for(i=0;i<len+1;i++)
    {   
  //      printf("%x ",buf[i]);
        if(buf[i] == '\0')
        {
            // printf("car %d\n",Car_index);
 //           printf("car %s\n",plate[j]);
            return j+1;
        }
        else if(buf[i] == ',')
        {
  //          printf("car %s\n",plate[j]);
            plate[j] [Car_index] ='\0';
            Car_index = 0;
            j++;  
        }
        else
        {
            plate[j][Car_index++] = buf[i];
        }
    }
}
uint8_t Clear_count=0;
uint8_t Locat_Book_num[3]={0};   //库位预约个数（包类型最多三个） 0:EU300;1:EU5;2:EU260
uint8_t Clear_Book_num[3]={0};
/* 回调函数， 一定要遵循huidiao函数的格式*/
static size_t httpCurlResCallback(void* buffer, size_t size, size_t nmemb, void* pUserArg)
{
    int i=0;
    size_t len = 0;len = size * nmemb;
  //  printf("%s; buffer = %s\n", __FILE__, (char *) buffer);
    // syslog(LOG_INFO,"Net_Cycle buffer == %s\n",(char *)buffer);
    
    cJSON *cJSON_buffer = NULL;
    unsigned char ParseResult_result[20];//返回结果

    cJSON_buffer = cJSON_Parse(buffer);
    if(NULL == cJSON_buffer)
        return;  

    switch(Cloud_Comm_cycle_index)
    {
    case 1:{// 换电站运行实时数据采集地址
            strcpy(ParseResult_result ,cJSON_GetObjectItem(cJSON_buffer, "result")->valuestring);
   //         printf("result :%s\n",ParseResult_result);
            if(ParseResult_result[0] !='\0')
            {
                if(strcmp(ParseResult_result,"OK")==0)
                {
  //                  printf("res %d parse ok !\n",Cloud_Comm_cycle_index);
                }
            }
    }
    break;
    case 127:{
        strcpy(ParseResult_result ,cJSON_GetObjectItem(cJSON_buffer, "result")->valuestring);
  //      printf("result :%s\n",ParseResult_result);
        if(ParseResult_result[0] !='\0')
        {
            if(strcmp(ParseResult_result,"OK")==0)
            {
                cJSON *car_arry = cJSON_GetObjectItem( cJSON_buffer, "data");  //orderCar_list 是使用 cjson对象
                if(Cloud_SendBookCE_flg==1||Cloud_SendBookCE_flg==5||Cloud_SendBookCE_flg==6) 
                {
                    Appoint_CallBack_Deal(0,0,0);
                    if( NULL != car_arry )
                    {
                        cJSON *car_list  = car_arry->child;
                        int count=0;
                        while( car_list != NULL )
                        {
                            char * carLpnNo   = cJSON_GetObjectItem( car_list , "carLpnNo")->valuestring ;
                            char * carType = cJSON_GetObjectItem( car_list , "carType")->valuestring ;
                            count = cJSON_GetObjectItem( car_list , "count")->valueint;

                            GetCatPlate(carLpnNo,strlen(carLpnNo));   
     //                       printf("carLpnNo: %s  carType: %s  count: %d\n",carLpnNo,carType,count);
                            car_list = car_list->next ;  
                            
                            Appoint_CallBack_Deal(count,carType,1);
                        }                                     
                    }
                }
                else if(Cloud_SendBookCE_flg==2)
                {
                    Cloud_SendBookCE_flg = 3;
                    if( NULL != car_arry )
                    {
                        cJSON *car_list  = car_arry->child;
                        int count=0;
                        Clear_Book_num[0] = 0;
                        Clear_Book_num[1] = 0;
                        Clear_Book_num[2] = 0; 
                        while( car_list != NULL )
                        {
                            char * carLpnNo = cJSON_GetObjectItem( car_list , "carLpnNo")->valuestring ;
                            char * carType = cJSON_GetObjectItem( car_list , "carType")->valuestring ;
                            count = cJSON_GetObjectItem( car_list , "count")->valueint;

                            GetCatPlate(carLpnNo,strlen(carLpnNo));   
         //                   printf("carLpnNo: %s  carType: %s  count: %d\n",carLpnNo,carType,count);
                            car_list = car_list->next ;   
                        
                            Appoint_CallBack_Deal(count,carType,2);
                        }                                              
                    }
                }
                else if(Cloud_SendBookCE_flg==3)
                {
                    Cloud_SendBookCE_flg = 4;
                    if( NULL != car_arry )
                    {
                        cJSON *car_list  = car_arry->child;
                        int count=0;
                        while( car_list != NULL )
                        {
                            char * carLpnNo = cJSON_GetObjectItem( car_list , "carLpnNo")->valuestring ;
                            char * carType = cJSON_GetObjectItem( car_list , "carType")->valuestring ;
                            count = cJSON_GetObjectItem( car_list , "count")->valueint;

                            GetCatPlate(carLpnNo,strlen(carLpnNo));   
         //                   printf("carLpnNo: %s  carType: %s  count: %d\n",carLpnNo,carType,count);
                            car_list = car_list->next ;  
                            int i=0;
                            Appoint_CallBack_Deal(count,carType,2);
                            // Clear_count+=count;
                        }     
                    }
                }
                if(Cloud_SendBookCE_flg==4)
				{
					Cloud_SendBookCE_flg = 0;
					Appoint_CallBack_Deal(0,0,4);
				}               
            }
        }
        else
        {
     //       printf("res %d parse err !\n",Cloud_Comm_cycle_index);  
        }
    }
    break;
    case 3:{
        strcpy(ParseResult_result ,cJSON_GetObjectItem(cJSON_buffer, "result")->valuestring);
        if(ParseResult_result[0] != '\0')
        {
            if(strstr(ParseResult_result,"202") || strstr(ParseResult_result,"203") )
            {
      //          printf("now_Time_Cloud:%s\n",ParseResult_result);
      //          printf("get time from cloud\n");
                MCloud_Comm_flg = 1;
                Cloud_Comm_err_count = 0;
            }
        }
    }
    break;
    case 4:{      //智能充电

    }
    case 12:{      //电池数量和布局

    }
    break;
    default:
    break;
    }
    return len;
}

void  LayOut_Inter_Deal(void)
{
    int i = 0,j = 0,Soc_num = 0;
    for(i = 0;i<20;i++) {
        LayOutNum.carType_socPercent[0][i] = 0;
        LayOutNum.carType_socPercent[1][i] = 0;  
    }
    LayOutNum.carType_avaCount[0] = 0;   
    LayOutNum.carType_avaCount[1] = 0;
    LayOutNum.carType_number[0] = 0;
    LayOutNum.carType_number[1] = 0;
    
    for(i=0;i<18;i++)
    {
        if(CHU_BattType[i]==1&&(CE_LocatorSenser[i]==2)&&(CHU_DevState_code[i]>=3)){
            for(j=19;j>=0;j--){
                if(BMS_BATT_SOC[i] >= j*5){
                    LayOutNum.carType_socPercent[0][19-j]++;
                    break;
                }           
            }
            if(BMS_BATT_SOC[i]>=MLocat_AllowCESoc_pct){
                 LayOutNum.carType_avaCount[0]++;
            }
            LayOutNum.carType_number[0]++;
        }
        else if(CHU_BattType[i]==2&&(CE_LocatorSenser[i]==2)&&(CHU_DevState_code[i]>=3)){
            for(j=19;j>=0;j--){
                if(BMS_BATT_SOC[i] >= j*5){
                    LayOutNum.carType_socPercent[1][19-j]++;
                    break;
                }           
            }
            if(BMS_BATT_SOC[i]>=MLocat_AllowCESoc_pct){
                 LayOutNum.carType_avaCount[1]++;
            }
            LayOutNum.carType_number[1]++;
        }   
    }
    for(i=0;i<20;i++){
        Soc_num += (LayOutNum.carType_socPercent[0][i]+LayOutNum.carType_socPercent[1][i]);
    }
    LayOutNum.carType_socPercent[0][19]+=(MCE_AllBag_num-Soc_num);
}

void mid_netCycle_Interface()
{
    
    // mid_netcycle.MCloud_Comm_flg = MCloud_Comm_flg;
    // mid_netcycle.Cloud_Comm_err_count = Cloud_Comm_err_count;
    Cloud_Comm_err_count = Cloud_Comm_err_count;
    MCloud_Comm_flg = MCloud_Comm_flg;

    strcpy(mid_netcycle.Staion_to_net_id,Staion_to_net_id);
    strcpy(mid_netcycle.Staion_to_net_pwd,Staion_to_net_pwd);
    strcpy(mid_netcycle.Staion_to_net_firmId,Staion_to_net_firmId);
}

uint8_t Cloud_SendUp10s_count = 0;

int pthread_net_cycle(void)
{
 //   printf("pthread_net_cycle tid %d\n",gettid());
    syslog(LOG_INFO,"pthread_net_cycle gettid '%d'\n",gettid());

    CURLcode curlRet = 0;
    CURL* pHandle = NULL;
    long *pHttpCode;
    char pRes[10000]= {0};
  
    FILE* fptr;
    char *url = NULL;
    char *jsondata = NULL;

    while(1)
    {   
        while(1)
        {
            usleep(500*1000);

            mid_netCycle_Interface();
        
            if(url_send_index == 1)         //发送数据 no
            {
                url_send_index = 3;
   
                stationRtData_ErrData_Interf(&StationRtData);
                stationRtData_ChangeBatt_Interf(&StationRtData);

                if(++Cloud_SendUp10s_count >= 10)
                {
                    Cloud_RtData_flg = 1;
                    Cloud_SendUp10s_count = 0;
                }
                if(Cloud_ChangePwrCmplt_flg || Cloud_RtData_flg || 0) //Cloud_SysErr_flg
                {
                    build_station_rtdata(StationRtData);
                    url = url_sendData;
                    jsondata = stationRtDataTemp;

                    Cloud_Comm_cycle_index = 1;

                    break;
                }

                // Cloud_Comm_cycle_index = 1;
                // break;
            }
            else if(url_send_index == 2)            //预约查询2
            {
                url_send_index = 3;

                appointment1.id = mid_netcycle.Staion_to_net_id;
                appointment1.pwd = mid_netcycle.Staion_to_net_pwd;
                appointment1.firmId = mid_netcycle.Staion_to_net_firmId;

                Cloud_Comm_cycle_index = 2;     
                // if((Cloud_SendBookCE_flg==5&&Book_Init_cou<=2)||(Cloud_SendBookCE_flg==6&&Book_Init_cou<=1))    //为1：半点锁包；为2：整点解包1；为3：整点解包2；
                // {    
                //     // if(Cloud_SendBookCE_flg==5&&Book_Init_cou<=2)
                //     url_send_index = 2;
                //     appointment1.starttime = Init_starttime[Book_Init_cou]; 
                //     appointment1.endtime = Init_endtime[Book_Init_cou]; 
                //     url_appointment_build(appointment1); 
                //     url = url_appointment_buf;
                //     break;
                // }
                // else if(Cloud_SendBookCE_flg==1)    //为1：半点锁包；为2：整点解包1；为3：整点解包2；
                // {    
                //     appointment1.starttime = starttime; 
                //     appointment1.endtime = endtime; 
                //     url_appointment_build(appointment1); 
                //     url = url_appointment_buf;
                //     break;
                // }
                // else if(Cloud_SendBookCE_flg==2)
                // {
                //     appointment1.starttime = starttime; 
                //     appointment1.endtime = endtime; 
                //     url_appointment_build(appointment1); 
                //     url = url_appointment_buf;
                //     break;
                // }
                // else if(Cloud_SendBookCE_flg==3)
                // {
                //     appointment1.starttime = starttime1; 
                //     appointment1.endtime = endtime1; 
                //     url_appointment_build(appointment1); 
                //     url = url_appointment_buf;
                //     break;
                // }

                // break;
            }
            else if(url_send_index == 3)                   //获取时间3
            {
                url_send_index = 1;
                url = url_getDate;
                // url = "192.168.0.198:6000";               
                jsondata = "";
                Cloud_Comm_cycle_index = 3;
                break;
            }          
            else if(url_send_index == 4)
            {
                url_send_index = 5;

                Cloud_Comm_cycle_index = 4;
                // break;
            }
            else if(url_send_index == 5)//换电站端内部电池数量和布局的对接接口
            {
                url_send_index = 1;
                
                LayOut_Inter_Deal();
                LayOutNum.id = mid_netcycle.Staion_to_net_id;
                LayOutNum.pwd = mid_netcycle.Staion_to_net_pwd;
                LayOutNum.firmId = mid_netcycle.Staion_to_net_firmId;
                LayOutNum.carType1= "EU300";
                LayOutNum.carType2= "EU5A";
                LayOutNum.carType3= "EU5B";                  
                json_LayOutNum_build(LayOutNum,jsondata_LayOutNum_test);
                
                url = url_LayOutNum;
                jsondata = jsondata_LayOutNum_test;
                Cloud_Comm_cycle_index = 12;
                break;
            }
        }
        do
        {
            if((Cloud_Comm_cycle_index==2&&Cloud_SendBookCE_flg==0)||Cloud_Comm_cycle_index==4)
              break;

            pthread_mutex_lock(&mutex_lock); 
            /* 全局初始化 */
            // curlRet = curl_global_init(CURL_GLOBAL_SSL|CURL_GLOBAL_ALL );
            // if(curlRet != 0)
            // {
            //     printf("curl_global_init() error. curlRet = %d\n", curlRet);
            // }
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
     //           printf("curl_easy_init()error.\n");
                break;
            }

            /* 设置连接超时时间 2秒*/
            curlRet = curl_easy_setopt(pHandle, CURLOPT_CONNECTTIMEOUT, 2);
            if(curlRet != 0)
            {
      //          printf("curl_easy_setopt() error! curlRet = %d\n", curlRet);
                break;
            }
            /* 设置超时时间 5秒*/
            curlRet = curl_easy_setopt(pHandle, CURLOPT_TIMEOUT, 5);
            if(curlRet != 0)
            {
    //            printf("curl_easy_setopt() error! curlRet = %d\n", curlRet);
                break;
            }

            /* 设置url*/
            curlRet = curl_easy_setopt(pHandle, CURLOPT_URL, url);
            if(curlRet != 0)
            {
     //           printf("curl_easy_setopt() error. curlRet = %d\n", curlRet);
                break;
            }

            curlRet = curl_easy_setopt(pHandle, CURLOPT_HTTPHEADER, headers);
            if(curlRet != 0)
            {
     //           printf("curl_easy_setopt() error. curlRet = %d\n", curlRet);
                break;
            }
            curlRet = curl_easy_setopt(pHandle, CURLOPT_POST, 1L);
            if(curlRet != 0)
            {
    //            printf("curl_easy_setopt() error. curlRet = %d\n", curlRet);
                break;
            }
            curlRet = curl_easy_setopt(pHandle, CURLOPT_POSTFIELDS, jsondata);
            if(curlRet != 0)
            {
    //            printf("curl_easy_setopt() error. curlRet = %d\n", curlRet);
                break;
            }
            // curlRet = curl_easy_setopt(pHandle, CURLOPT_WRITEDATA, fptr);
            // printf("url= %p\n",pHandle);
            // if(curlRet != 0)
            // {
            //     printf("curl_easy_setopt() error. curlRet = %d\n", curlRet);
            //     break;
            // }
            /* 设置 回调函数 */
            curlRet = curl_easy_setopt(pHandle, CURLOPT_WRITEFUNCTION, httpCurlResCallback);
            if(curlRet != 0)
            {
    //            printf("curl_easy_setopt() error. curlRet = %d\n", curlRet);
                break;
            }
            // printf("there is safe !\n");
            /*传入回调函数需要的结构体的指针 */
            curlRet = curl_easy_setopt(pHandle, CURLOPT_WRITEDATA, (void *)pRes);
            if(curlRet != 0)
            {
     //           printf("curl_easy_setopt() error. curlRet = %d\n", curlRet);
                break;
            }
            curlRet = curl_easy_setopt(pHandle, CURLOPT_NOSIGNAL, 1);
            if(curlRet != 0)
            {
     //           printf("curl_easy_setopt() error. curlRet = %d\n", curlRet);
                break;
            }

            curlRet = curl_easy_perform(pHandle);
            if (curlRet == CURLE_COULDNT_RESOLVE_HOST){
                 res_init();
            }
            if(curlRet != 0)
            {
     //           printf("curl_easy_perform() error. curlRet = %d %d\n", curlRet,Cloud_Comm_cycle_index);
                curl_slist_free_all(headers);
                curl_easy_cleanup(pHandle);

                pthread_mutex_unlock(&mutex_lock); 
                // 释放全局curl对象
                // curl_global_cleanup();
                break;
            }
            // 释放headers
            curl_slist_free_all(headers);
            // 释放curl对象
            curl_easy_cleanup(pHandle);

            pthread_mutex_unlock(&mutex_lock); 
                // 释放全局curl对象
            // curl_global_cleanup();
            
        }while(0);
        //  curl_easy_cleanup(pHandle);
        //         // 释放全局curl对象
        //  curl_global_cleanup();

        // if(curlRet != 0)
        /*和云端通信故障计数*/
        // Cloud_Comm_err_count++;
        // if(Cloud_Comm_err_count >= 5*3)
        //     MCloud_Comm_flg = 2; 
    }

    while(1)
    {
        // Cloud_Comm_err = 1;
     //   printf("run to main\n");
        sleep(5);
    }
}


// /*从云端返回结果中，从中提取值*/
// unsigned char Seek_value(char * buf,char *goal_buf,char *result_buf, int result_buf_len)
// {
//     // printf("in\n");
//     int buf_len = 0;
//     int goal_buf_len = 0;
//     int len2;

//     char * location = NULL;
//     int i,j = 0;
//     char temp[100];/*存储键值的缓存*/

//     buf_len = strlen(buf);
//     goal_buf_len = strlen(goal_buf);

//     /*需要查找的源字符串为空，或者第一个串小于第二个，直接返回*/
//     if(buf_len == 0 || buf_len < goal_buf_len)
//     {
//         result_buf[0] = '\0';
//         return 0;
//     }
//     // printf("len of buf %d\n",buf_len);
//     location = strstr(buf,goal_buf);

//     if(location == NULL)
//     {
//         result_buf[0] = '\0';
//         return 0;
//     }
//     /*将指针移到目标串右方*/
//     location += strlen(goal_buf);
//     /*除去目标字符串及其左方字符串后的长度*/
//     len2 = strlen(location);

// // {"result":"OK","carType":"EU300","orderId":"00010016202008130001","BatteryNo":"00MPE0JC431D0MA431049001"}
//     for(i = 0;i < len2;i++)
//     {
//         if(*location == '\0' || *location == ','|| *location == '}')
//         {
//             break;
//         }
//         else if( ((*location>='A') && (*location<='Z')) ||
//             ((*location>='a') && (*location<='z')) ||
//             ((*location>='0') && (*location<='9')))
//         {
//             temp[j++] = *location;
//         }
//         else if(*location & 0x80)
//         {
//             temp[j++] = *location;
//         }
//         location++;
//     }
//     temp[j] = '\0';
//     if( strlen(temp) <= result_buf_len)
//     {
//         strcpy(result_buf,temp);
//     }  
//     else
//     {
//         return 0;
//     }
//     // printf("%s\n",temp);
//     // printf("%s\n",result_buf);
//     // printf("len of temp = %d\n",(int)strlen(temp));
//     return 1;
// }
void json_interface()
{
    
    json_test.id = Staion_to_net_id;
    json_test.pwd = Staion_to_net_pwd;
    json_test.firmId = Staion_to_net_firmId;
    
    json_test.ChangePowerID = "鄂州1号换电站";
    json_test.Status="0";
    json_test.Data.ChangeData.ChangeStatus = "aaa";
    json_test.Data.ChangeData.CarNo = "bbb";
    json_test.Data.ChangeData.NewBatteryNo = "ccc";
    json_test.Data.ChangeData.OldBatteryNo = "ddd";
    json_test.Data.ChangeData.NewSeat = "eee";

    json_test.Data.RunData.Status = "0";
    json_test.Data.RunData.Device1 = "0";
    json_test.Data.RunData.Device2 = "0";
    json_test.Data.RunData.Device3 = "0";
    json_test.Data.RunData.Device4 = "0";
    json_test.Data.RunData.Device5 = "0";
    json_test.Data.RunData.ErrLevel = "0";
    json_test.Data.RunData.RunStatus = "0";

    json_test.Data.ChargeData.SeatNo = "1";
    json_test.Data.ChargeData.BatteryNo = "00MPE0JC431D0MA431020001EE00110233";
    json_test.Data.ChargeData.SOC = "10";
    json_test.Data.ChargeData.Voltage = "";
    json_test.Data.ChargeData.Temperature = "";
    json_test.Data.ChargeData.Current = "";
    json_test.Data.ChargeData.GNDA = "";
    json_test.Data.ChargeData.GNDZ = "";
    json_test.Data.ChargeData.MinTemperature = "";
    json_test.Data.ChargeData.MaxTemperature = "";
    json_test.Data.ChargeData.MinVoltage = "";
    json_test.Data.ChargeData.MaxVoltage = "";
    json_test.Data.ChargeData.MinTNo = "";
    json_test.Data.ChargeData.MaxTNo = "";
    json_test.Data.ChargeData.ZAS = "";
    json_test.Data.ChargeData.NL = "";
    json_test.Data.ChargeData.OutI = "";
    json_test.Data.ChargeData.I1 = "";
    json_test.Data.ChargeData.Instruct = "";
    json_test.Data.ChargeData.V1 = "";
    json_test.Data.ChargeData.BJ = "";
    json_test.Data.ChargeData.RL = "";
    json_test.Data.ChargeData.BMSCNo = "";
    json_test.Data.ChargeData.BMSODO = "";
    json_test.Data.ChargeData.BMSZNo = "";
    json_test.Data.ChargeData.Error = "";
    json_test.Data.ChargeData.MinVNo = "";
    json_test.Data.ChargeData.MaxVNo = "";

    // json_build(&json_test);

}