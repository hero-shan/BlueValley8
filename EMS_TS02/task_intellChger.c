
#include "string.h"
#include "stdlib.h"
#include "stdio.h"
#include "ctype.h"

#include "pthread.h"

#include "errno.h"
#include "unistd.h"
#include "sys/time.h"
#include "time.h"
#include <curl/curl.h>
#include "cJSON.h"

#define SYS_BATT_NUM    (18)

#define USE_CURL_HOST_SET (1)
#define USE_CURL_DNS_SET (1)

#define false (0)
#define true (1)

#define INTERVAL_TIME (1)

typedef unsigned long size_t;
typedef unsigned char uint8_t;

int batt_index = 17;

uint8_t tableSta[20]={0};
uint8_t tableErr_flg[20]={0};
uint8_t tableErrCnt[20]={0};

typedef struct _intelChgerTable_t{
    uint8_t battNo[32];

    uint8_t h_num;
    uint8_t l_num;
    char real_table[17][13][20];

    double Soc_Index[20];
    int Soc_IdxNum;
    double Volt_Index[10];
    int Volt_IdxNum;
    double table[16][12];

} intelChgerTable_t;

intelChgerTable_t intelChgerTable[20] = {0};
// double Soc_Index[20]={0.05l,0.1l,0.2l,0.3l,0.4l,0.5l,0.6l,0.7l,0.8l};
// double Volt_Index[4]={3.5l,3.55l,3.65l};
double Tmp_Index[16]={-20l,-15l,-10l,-5l,0l,5l,10l,15l,20l,25l,30l,35l,40l,45l,50l,55l};




//获取列坐标
int get_l_Index(double soc,double volt)
{
	int no = 0;
	int i = 0;    
	if(soc <= 0.8){no = 0;}
	else {no = 1;}

    if(no == 0)
    {
        for(i=0;i<intelChgerTable[batt_index].Volt_IdxNum;i++)
        {
            if(soc > intelChgerTable[batt_index].Soc_Index[i]){;}
            else {return i;}   
        }
    }
    else if(no == 1)
    {
        for(i=0;i<intelChgerTable[batt_index].Soc_IdxNum;i++)
        {
            if(volt > intelChgerTable[batt_index].Volt_Index[i]){;}
            else {return intelChgerTable[batt_index].Volt_IdxNum + i;} 
        }
    }
    return -1;    
}

//获取行坐标
int get_h_Index(double tmp)
{
	int i = 0; 
    for(i=0;i<16;i++)
    {
        if(tmp > Tmp_Index[i]){;}
        else {return i;}   
    }	
    return -1;
}


/* 更新智能充电表格 */
void updataIntelChgerTable(char *str)
{
    const char s[2] = ",";
    int i=0,j=0;
    char *token;
    uint8_t battNo_tmp[33]={0};
    uint8_t tableData_tmp[2000]={0};

    cJSON *cJSON_buffer = NULL;
    unsigned char ParseResult_result[20];//返回结果

    cJSON_buffer = cJSON_Parse(str);
    if(NULL == cJSON_buffer)
        return; 
    
    if(cJSON_GetObjectItem(cJSON_buffer, "pack_id")->valuestring != NULL)
        strcpy(battNo_tmp ,cJSON_GetObjectItem(cJSON_buffer, "pack_id")->valuestring);
    
    /* 解析车牌 */
    strcpy(intelChgerTable[batt_index].battNo,battNo_tmp);
    // printf("battNo:%s\n",intelChgerTable[batt_index].battNo);

    if(cJSON_GetObjectItem(cJSON_buffer, "policy")->valuestring != NULL)
        strcpy(tableData_tmp ,cJSON_GetObjectItem(cJSON_buffer, "policy")->valuestring);

    cJSON_Delete(cJSON_buffer);
    /* 解析表 */
    token = strtok(tableData_tmp, s);
    // printf("%s\n",token);

    if(token != NULL)
    {    
        intelChgerTable[batt_index].l_num = ((token[2] -'0')* 10 + (token[3]-'0'));
        intelChgerTable[batt_index].h_num = ((token[4] -'0')* 10 + (token[5]-'0'));
    }

    for(i=0;i< intelChgerTable[batt_index].h_num + 1 ;i++)
    {
        for(j=0;j< intelChgerTable[batt_index].l_num + 1 ;j++)
        {
            if(token != NULL)
            {
                strcpy(intelChgerTable[batt_index].real_table[i][j],token);
                // printf("%s\t",intelChgerTable[batt_index].real_table[i][j]);
            }
            token = strtok(NULL, s);
            // printf("\n");
        }    
    }

    for(i=1;i< intelChgerTable[batt_index].h_num + 1 ;i++)
    {
        for(j=1;j< intelChgerTable[batt_index].l_num + 1 ;j++)
        {
            intelChgerTable[batt_index].table[i-1][j-1] = atof(intelChgerTable[batt_index].real_table[i][j]);
            // printf("%.3lf\t",intelChgerTable[batt_index].table[i-1][j-1]);
        }
        // printf("\n"); 
    }

    int idx = 0;
    intelChgerTable[batt_index].Soc_IdxNum =0;
    intelChgerTable[batt_index].Volt_IdxNum=0;
    for(i=0;i< intelChgerTable[batt_index].l_num ;i++)
    {
        if(intelChgerTable[batt_index].table[0][i] <= 0.8)
        {
            intelChgerTable[batt_index].Soc_Index[idx++] = intelChgerTable[batt_index].table[0][i];
            intelChgerTable[batt_index].Soc_IdxNum++;
        }else
        {
            intelChgerTable[batt_index].Volt_Index[idx++] = intelChgerTable[batt_index].table[0][i];
            intelChgerTable[batt_index].Volt_IdxNum++;
        }
    }

    tableSta[batt_index] = 1;
    printf("回馈结果:%d---%d\n",batt_index,tableSta[batt_index]);
}

/* 读取智能充电表，获取充电倍率 */
double getIntelChgerData(char *battNo,char num,double soc,double volt,double tmp)
{
    double Chg_rate = 0;
    int l=0,h=0;

    if((strcmp(intelChgerTable[num].battNo,battNo) == 0) && (tableSta[num] == 1))
    {
        l = get_l_Index(soc,volt);
        h = get_h_Index(tmp);
        if((l != -1) && (h != -1))
        {
            Chg_rate = intelChgerTable[num].table[h][l];
  //          printf("h:%d l:%d\n",h,l);
  //          printf("Chg_rate = %lf\n",Chg_rate);
            return Chg_rate;
        }
    }
    // else
    {
        tableSta[num] = 0;
        return -1;
    }
}


/* 回调函数， 一定要遵循回调函数的格式 */
static size_t httpCurlResCallback(void* buffer, size_t size, size_t nmemb, void* pUserArg)
{
    size_t len = 0;
    int kkk = 0;
 //   printf("%s; buffer = %s\n", __FILE__, (char *) buffer);
    len = size * nmemb;

    updataIntelChgerTable((char *) buffer);

    return len;
}


int time_count = 0;




char url_IntelChger[2000]={0};
void url_IntelChger_build(char *battNo,int lock)
{
    sprintf(url_IntelChger,"http://10.69.53.10:8080/policy?pack_id=\"%s\"&lock=%d",battNo,lock);
}

void aa()
{
    
    char *url="http://10.69.53.10:8080/policy?pack_id=%27A55PE01615G122AA0011HE00%27&lock=1";
    char *jsondata="";

    CURLcode curlRet = 0;
    CURL* pHandle = NULL;
    long *pHttpCode;
    char pRes[10000]= {0};
    FILE fptr;

    // curl_global_init(CURL_GLOBAL_SSL|CURL_GLOBAL_ALL );
    while (1)
    {
        while(1)
        {
            usleep(100*1000);
            if(batt_index<SYS_BATT_NUM-1)
            {
                batt_index++;
            }
            else
            {
                batt_index = 0;
            }
            time_count ++;      
            if(time_count >= INTERVAL_TIME*60*10)
            {
                time_count = 0;
                int i;
                for(i=0;i<20;i++)
                {
    //                printf("%d ",tableErrCnt[i]);
                    tableSta[i] = 0;
                    tableErr_flg[i] = 0;
                    tableErrCnt[i] = 0;
                }
     //           printf("\n");
                batt_index = 0;
            }        
            if(tableSta[batt_index] == 0 && tableErr_flg[batt_index] == 0)
            {    
                url_IntelChger_build("A55PE01615G122AA0011HE00",1);
                // url = url_IntelChger;
                // jsondata;
     //           printf("6666:%d\n",batt_index);
                break;
            }
        }      
        do
        {
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
     //           printf("curl_easy_setopt() error! curlRet = %d\n", curlRet);
                break;
            }
            /* 设置超时时间 5秒*/
            curlRet = curl_easy_setopt(pHandle, CURLOPT_TIMEOUT, 5);
            if(curlRet != 0)
            {
     //           printf("curl_easy_setopt() error! curlRet = %d\n", curlRet);
                break;
            }
            
            #if USE_CURL_HOST_SET
            struct curl_slist *host = curl_slist_append(NULL,
                                                "localhost:80:127.0.0.1");

            curl_easy_setopt(pHandle, CURLOPT_RESOLVE,host);
            #endif
            #if USE_CURL_DNS_SET
            curl_easy_setopt(pHandle, CURLOPT_DNS_USE_GLOBAL_CACHE, false );
            curl_easy_setopt(pHandle, CURLOPT_DNS_CACHE_TIMEOUT, 2 );
            #endif
            
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
     //           printf("curl_easy_setopt() error. curlRet = %d\n", curlRet);
                break;
            }
            curlRet = curl_easy_setopt(pHandle, CURLOPT_POSTFIELDS, jsondata);
            if(curlRet != 0)
            {
    //            printf("curl_easy_setopt() error. curlRet = %d\n", curlRet);
                break;
            }
            curlRet = curl_easy_setopt(pHandle, CURLOPT_WRITEDATA, fptr);
            // printf("url= %p\n",pHandle);
            if(curlRet != 0)
            {
    //            printf("curl_easy_setopt() error. curlRet = %d\n", curlRet);
                break;
            }
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
            if(curlRet != 0)
            {
     //           printf("curl_easy_perform() error. curlRet = %d\n", curlRet);
                
     //           printf("errno: %s\n",strerror(errno));
                curl_slist_free_all(headers);
                curl_easy_cleanup(pHandle);
                // 释放全局curl对象
                // curl_global_cleanup();

                if((++tableErrCnt[batt_index] ) >= 3)
                    tableErr_flg[batt_index] = 1;


                break;
            }
            // 释放headers
            curl_slist_free_all(headers);
            // 释放host
            #if USE_CURL_HOST_SET
            curl_slist_free_all(host);  
            #endif          
            // 释放curl对象
            curl_easy_cleanup(pHandle);
            // 释放全局curl对象
            // curl_global_cleanup();
        }while(0);
    }
}