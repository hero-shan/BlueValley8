#ifndef _CLOUD_PLATFORM_H_
#define _CLOUD_PLATFORM_H_


//	车辆进站放行地址
typedef struct {

unsigned char *id;
unsigned char *pwd;
unsigned char *firmId;
unsigned char *carNo;

}carInfo_t;



//车辆放行地址
typedef struct {

unsigned char *id;
unsigned char *pwd;
unsigned char *firmId;
unsigned char *carNo;
unsigned char *orderId;
unsigned char *newBatteryNo;

}carOutOf_t;
// ODO
typedef struct {
unsigned char *id;
unsigned char *pwd;
unsigned char *firmId;
unsigned char *carNo;
unsigned char *orderId;
unsigned char *newBatteryNo;
unsigned char *ODO;
}carOdo_t;
//取消换电订单
typedef struct {

unsigned char *id;
unsigned char *pwd;
unsigned char *firmId;
unsigned char *carNo;
unsigned char *orderId;

}cancelOrder_t;

//匹配电池包溯源码
typedef struct {

unsigned char *id;
unsigned char *pwd;
unsigned char *firmId;
unsigned char *batteryNo;
unsigned char *orderId;
}matchBatNo_t;

//时间校对
typedef struct {
unsigned char *id;
unsigned char *pwd;
unsigned char *firmId;
}getDate_t;

typedef struct {
unsigned char *id;
unsigned char *pwd;
unsigned char *firmId;
unsigned char *starttime;
unsigned char *endtime;

}appointment_t;


/*换电站中各个时刻可预约的电池数量*/
typedef struct {

unsigned char *id;
unsigned char *pwd;
unsigned char *firmId;
unsigned char *carType1;
unsigned char *carType2;
unsigned char carType_count[3][24];

}AvailableNum_t;

/*换电站中各个时刻可预约的电池数量*/
typedef struct {

unsigned char *id;
unsigned char *pwd;
unsigned char *firmId;
unsigned char *orderId;
unsigned char orderState;
}CESta_t;
/*
换电站端内部电池数量和布局 struct 
*/
typedef struct {
unsigned char *id;
unsigned char *pwd;
unsigned char *firmId;
unsigned char *carType1;
unsigned char *carType2;
unsigned char *carType3;
unsigned char carType_number[3];
unsigned char carType_socPercent[3][20];
unsigned char carType_avaCount[3];
}LayOutNum_t;

LayOutNum_t LayOutNum;

// //换电站端内部电池数量和布局的对接接口
// char* jsondata_powerPlantLayOutNum = "{  \"id\" : \"B00000092006009\",\
//                                     \"pwd\":\"12345678\",\
//                                     \"firmId\":\"B00000092006009\",\
//                                     \"data\":[{\
//                                         \"carType\":\"EU300\",\
//                                         \"number\":10,\
//                                         \"socPercent\":[0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,10],\
//                                         \"avaCount\":10\
//                                         }]}";

// //换电站中各个时刻可预约的电池数量
// char* jsondata_powerPlantAvailableNum = "{  \"id\" : \"B00000092006009\",\
//                                     \"pwd\":\"12345678\",\
//                                     \"firmId\":\"B00000092006009\",\
//                                     \"data\":[{\
//                                         \"carType\":\"EU300\",\
//                                         \"count\":[0,1,2,3,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0]\
//                                         }]}";





// 换电站中各个时刻可预约的电池数量
void json_AvailableNum_build(AvailableNum_t AvailableNum,unsigned char *buf);
//换电站端内部电池数量和布局的对接接口
void json_LayOutNum_build(LayOutNum_t LayOutNum,unsigned char *buf);

//从云端返回结果中，从中提取值
unsigned char Seek_value( char * buf, char *goal_buf,char *result_buf, int result_buf_len);
//将汉字（UTF8）转成URL可识别的编码
int URLEncode(const char* str, const int strSize, char* result, const int resultSize);




/*取消订单url构建函数*/
void url_cancelOrder_build(cancelOrder_t cancelOrder,char num);
/*时间校对*/
void url_getDate_build(getDate_t getDate);
/*车辆信息url构建函数*/
void url_carInfo_build(carInfo_t carInfo);
/*车辆进站放行url构建函数*/
void url_carOutOf_build(carOutOf_t carOutOf);
/*车辆ODO url构建函数*/
void url_carOdo_build(carOdo_t carOdo);

void url_matchBatNo_build(matchBatNo_t matchBatNo);
void url_CESta_build(CESta_t CESta);

//***********************cycle***************************


typedef struct charg_data{
char *SeatNo;
char *BatteryNo;
char *SOC;
char *Voltage;
char *Temperature;
char *Current;
char *MaxVoltage;
char *MinVoltage;
char *MaxTemperature;
char *MinTemperature;
char *GNDA;
char *GNDZ;
char *ZAS;
char *V1;
char *Instruct;
char *I1;
char *OutI;
char *NL;
char *RL;
char *BJ;
char *BMSZNo;
char *BMSCNo;
char *MinTNo;
char *MaxTNo;
char *MinVNo;
char *MaxVNo;
char *Error;
char *BMSODO;
}
chargeData;



typedef struct chang_data{
char *ChangeStatus;
char *CarNo;
char *NewBatteryNo;
char *OldBatteryNo;
char *NewSeat;
char *OldSeat;
char *ChangeDate;
char *electricityDown;
char *electricityUp;
char *capacityDown;
char *capacityUp;
char *socUp;
char *socDown;
}
changeData;

typedef struct run_data{
char *Status;
char *Device1;
char *Device2;
char *Device3;
char *Device4;
char *Device5;
char *Device6;
char *Device7;
char *ErrLevel;
char *RunStatus;
char *Message_Device;
char *Message_No;
char *Message_Mes;
}
runData;

typedef struct car_data{

char *car_type;
char *car_number;

changeData ChangeData;
runData RunData;
changeData dcMeterData;
changeData acMeterData;
chargeData ChargeData;

} car_inf_t;


typedef struct json2{

char *id;
char *pwd;
char *firmId;

char car_num;

car_inf_t car1_data;
car_inf_t car2_data;
car_inf_t car3_data;
char *ChangePowerID;
char *Status;
car_inf_t Data;

} json_t;










#endif