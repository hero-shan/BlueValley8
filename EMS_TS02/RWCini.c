#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "mymain.h"

#include "iniparser.h"
dictionary* ini;

void create_example_ini_file(void);
int  parse_ini_file(char * ini_name);
void append_example_ini_file();

int a(int argc, char * argv[])
{
     int  status ;

    // if (argc<2) {
        // create_example_ini_file();
        //   append_example_ini_file();
         status = parse_ini_file("example.ini");
       
    // } else {
        //  status = parse_ini_file(argv[1]);
    // }
     return status ;
}
// void append_example_ini_file()
// {  FILE * example;
//     int ret ;
//      example = fopen("example.ini", "a");//
//     //  ini = iniparser_load("example.ini");
//     ret = iniparser_set( ini, "Local Network Identification:ss", "wwwwsssww");
//      ret = iniparser_set( ini, "Local Network Identification:ww", "wwwwssswwwwwww");
//     iniparser_dump(ini, example);
//     fclose(example);
// }

// void create_example_ini_file(void)
// {
//     FILE * example;
//     example = fopen("example.ini", "w");//
//     int ret ;


//    ini = iniparser_load("/home/slc/linux/test/example.ini");
  
//     ret = iniparser_set( ini, "ipaddrpool",NULL);
   
//     // iniparser_dump_ini(ini, example);

//     ret = iniparser_set( ini, "ipaddrpool:pwd", "wwwwww");
//     ret = iniparser_set( ini, "ipaddrpool:firmId", "wwwwww");
//     ret = iniparser_set( ini, "ipaddrpool:id", "wwwwww");
//        iniparser_dumpsection_ini(ini, "ipaddrpool", example); 
//     // iniparser_dump(ini, example);
//     // fclose(example);
//     //*******************************************************///

//     //  FILE    *   ini ;

//     // ini = fopen("example.ini", "w");
//     // fprintf(ini,
//     // "#\n"
//     // "# This is an example of ini file\n"
//     // "#\n"
//     // "\n"
//     // "[Local Network Identification]\n"
//     // "\n"
//     // "firmId  = \"sa04\";\n"
//     // "id  = \"06\";\n"
//     // "pwd  = \"123456\";\n"
//     // "URL_Entry = \"aa12\";\n"
//     // "URL_Gettime = \"b12b\";\n"
//     // "URL_Out = \"cc555c\";\n"
//     // "URL_UpdateODO  = \"3fg\";\n"
//     // "URL_Realtimedata  = \"4fdsf\";\n"
//     // "URL_Exchangeresult  = \"fsf5\";\n"
//     // "URL_Cancelorder  = \"retwe6\";\n"
//     // "BatteryNumVerification  = \"dgdsg7\";\n"
//     // "URL  = \"ggg8\";\n"
//     // "QueryVehicleInf  = \"www\";\n"
//     // "queueNumber  = \"qqq10\";\n"
//     // "powerPlantLayOutNum  = \"1eee1\";\n"
//     // "appointmentList   = \"1eee2\";\n"
//     // "powerPlantAvailableNum   = \"ee13\";\n"

//     // "orderState.htm   = \"1ee4\";\n"
//     // "\n"
//     // "\n"
//     // "\n");
//     // fclose(ini);
// }

uint8_t iniData_URL_buf[100] = {0};//URL = "https://tswitcher.bj-abt.com/switcher-console/";
uint8_t iniData_carInfo_base_buf[100] = {0};//carInfo_base = "ems/carInfo.htm";

uint8_t iniData_getDate_buf[100] = {0};//getDate = "ems/getDate.htm";
uint8_t iniData_carOutOf_base_buf[100] = {0};//carOutOf_base = "ems/carOutOf.htm";
uint8_t iniData_carOdo_base_buf[100] = {0};//carOdo_base  = "ems/carOdo.htm";

uint8_t iniData_Realtimedata_buf[100] = {0};//Realtimedata  = "ems/sendData.htm";
uint8_t iniData_Exchangeresult_buf[100] = {0};//Exchangeresult  = "fsf5";
uint8_t iniData_cancelOrder_base_buf[100] = {0};//cancelOrder_base  = "ems/cancelOrder.htm";
uint8_t iniData_matchBatNo_base_buf[100] = {0};//matchBatNo_base  = "ems/checkBatteryNo.htm";
uint8_t iniData_getCarType_base_buf[100] = {0};//getCarType_base = "ems/carType.htm";
uint8_t iniData_queueNumber_buf[100] = {0};//queueNumber = "ems/queueNumber.htm";

uint8_t iniData_powerPlantLayOutNum_buf[100] = {0};//powerPlantLayOutNum = "ems/powerPlantLayOutNum.htm";
uint8_t iniData_url_appointment_buf[100] = {0};//url_appointment = "ems/appointmentList.htm";
uint8_t iniData_powerPlantAvailableNum_buf[100] = {0};//powerPlantAvailableNum = "ems/powerPlantAvailableNum.htm";
uint8_t iniData_CESta_base_buf[100] = {0};//CESta_base = "ems/orderState.htm";




/*取消订单url地址*/
extern char *url_cancelOrder_base;
/**/
extern char *url_carOutOf_base;
/**/
extern char *url_getDate;// = "https://tswitcher.bj-abt.com/switcher-console/ems/getDate.htm";
/*车辆ODO*/
extern char *url_carOdo_base;
/*车辆信息url*/
extern char *url_carInfo_base;
/*电池包溯源码是否匹配url*/
extern char *url_matchBatNo_base;
/*获取CarType  url*/
extern char *url_getCarType_base;
/*获取换电状态  url*/
extern char *url_CESta_base;

/*换电站端内部电池数量和布局的对接接口url地址*/
extern char *url_LayOutNum;
extern char *url_appointment_base;
extern char *url_AvailableNum;

extern char *url_sendData;


//queueNumber is missing

int parse_ini_file(char * ini_name)
{
    int             b ;
    int             i ;
    double          d ;
    const char  *   s ;
    const char  **   ss ;
    int ret ;

    ini = iniparser_load(ini_name);
    if (ini==NULL) {
        fprintf(stderr, "cannot parse file: %s\n", ini_name);
        return -1 ;
    }
    // iniparser_dump(ini, stderr);

    /* Get pizza attributes */
 //   printf("Local_Network_Identification:\n");

 //   printf("\n%s:\n",iniparser_getsecname(ini,0));///获取dictionary对象的第n个section的名字

    s = iniparser_getstring(ini,"Local_Network_Identification:id","null");
 //   printf("id : %s\n",s);
    strcpy(Staion_to_net_id,s);

    s = iniparser_getstring(ini,"Local_Network_Identification:pwd","null");
 //   printf("pwd : %s\n",s);
    strcpy(Staion_to_net_pwd,s);

    s = iniparser_getstring(ini,"Local_Network_Identification:firmId","null");//返回idictionary对象的section:key对应的整形值
    strcpy(Staion_to_net_firmId,s);
 //   printf("firmId : %s\n",s);
    //************************url common head
    s = iniparser_getstring(ini,"Local_Network_Identification:URL","null");//返回idictionary对象的section:key对应的整形值
    strcpy(iniData_URL_buf,s);
 //   printf("URL : %s\n",s);


    s = iniparser_getstring(ini,"Local_Network_Identification:carInfo_base","null");
//    printf("carInfo_base  : %s\n",s);
    strcpy(iniData_carInfo_base_buf,iniData_URL_buf);
    strcat(iniData_carInfo_base_buf,s);
    url_carInfo_base = iniData_carInfo_base_buf;

    s = iniparser_getstring(ini,"Local_Network_Identification:getDate","null");
//    printf("getDate  : %s\n",s);
    strcpy(iniData_getDate_buf,iniData_URL_buf);
    strcat(iniData_getDate_buf,s);
    //url_getDate = iniData_getDate_buf;

    s = iniparser_getstring(ini,"Local_Network_Identification:carOutOf_base","null");
 //   printf("carOutOf_base  : %s\n",s);
    strcpy(iniData_carOutOf_base_buf,iniData_URL_buf);
    strcat(iniData_carOutOf_base_buf,s);
    url_carOutOf_base = iniData_carOutOf_base_buf;

    s = iniparser_getstring(ini,"Local_Network_Identification:carOdo_base","null");
 //   printf("carOdo_base  : %s\n",s);
    strcpy(iniData_carOdo_base_buf,iniData_URL_buf);
    strcat(iniData_carOdo_base_buf,s);
    url_carOdo_base = iniData_carOdo_base_buf;

    s = iniparser_getstring(ini,"Local_Network_Identification:Realtimedata","null");
 //   printf("Realtimedata  : %s\n",s);
    strcpy(iniData_Realtimedata_buf,iniData_URL_buf);
    strcat(iniData_Realtimedata_buf,s);
    url_sendData = iniData_Realtimedata_buf;

    // s = iniparser_getstring(ini,"Local_Network_Identification:Exchangeresult","null");
    // printf("Exchangeresult  : %s\n",s);
    // strcpy(iniData_Exchangeresult_buf,iniData_URL_buf);
    // strcat(iniData_Exchangeresult_buf,s);

    s = iniparser_getstring(ini,"Local_Network_Identification:cancelOrder_base","null");
 //   printf("cancelOrder_base  : %s\n",s);
    strcpy(iniData_cancelOrder_base_buf,iniData_URL_buf);
    strcat(iniData_cancelOrder_base_buf,s);
    url_cancelOrder_base = iniData_cancelOrder_base_buf;

    s = iniparser_getstring(ini,"Local_Network_Identification:matchBatNo_base","null");
 //   printf("matchBatNo_base  : %s\n",s);
    strcpy(iniData_matchBatNo_base_buf,iniData_URL_buf);
    strcat(iniData_matchBatNo_base_buf,s);
    url_matchBatNo_base = iniData_matchBatNo_base_buf;

    s = iniparser_getstring(ini,"Local_Network_Identification:getCarType_base","null");
 //   printf("getCarType_base  : %s\n",s);
    strcpy(iniData_getCarType_base_buf,iniData_URL_buf);
    strcat(iniData_getCarType_base_buf,s);
    url_getCarType_base = iniData_getCarType_base_buf;

    s = iniparser_getstring(ini,"Local_Network_Identification:queueNumber","null");
//    printf("queueNumber  : %s\n",s);
    strcpy(iniData_queueNumber_buf,iniData_URL_buf);
    strcat(iniData_queueNumber_buf,s);

    s = iniparser_getstring(ini,"Local_Network_Identification:powerPlantLayOutNum","null");
 //   printf("powerPlantLayOutNum  : %s\n",s);
    strcpy(iniData_powerPlantLayOutNum_buf,iniData_URL_buf);
    strcat(iniData_powerPlantLayOutNum_buf,s);
    url_LayOutNum = iniData_powerPlantLayOutNum_buf;

    s = iniparser_getstring(ini,"Local_Network_Identification:url_appointment","null");
 //   printf("url_appointment  : %s\n",s);
    strcpy(iniData_url_appointment_buf,iniData_URL_buf);
    strcat(iniData_url_appointment_buf,s);
    url_appointment_base = iniData_url_appointment_buf;


    s = iniparser_getstring(ini,"Local_Network_Identification:powerPlantAvailableNum","null");
 //   printf("powerPlantAvailableNum  : %s\n",s);
    strcpy(iniData_powerPlantAvailableNum_buf,iniData_URL_buf);
    strcat(iniData_powerPlantAvailableNum_buf,s);
    url_AvailableNum = iniData_powerPlantAvailableNum_buf;

    s = iniparser_getstring(ini,"Local_Network_Identification:CESta_base","null");
 //   printf("CESta_base  : %s\n",s);
    strcpy(iniData_CESta_base_buf,iniData_URL_buf);
    strcat(iniData_CESta_base_buf,s);
    url_CESta_base = iniData_CESta_base_buf;


 //   printf("url_cancelOrder_base = %s \n",url_cancelOrder_base);
 //   printf("url_carOutOf_base = %s \n",url_carOutOf_base); 
    // printf("url_getDate = %s \n",url_getDate);
 //   printf("url_cancelOrder_base = %s \n",url_cancelOrder_base);   

    iniparser_freedict(ini);
    return 0 ;
}
