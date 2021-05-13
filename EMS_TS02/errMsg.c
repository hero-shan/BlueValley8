
#include "stdio.h"
#include "unistd.h"
#include "sys/time.h"
#include "time.h"
#include "string.h"
#include "stdlib.h"

#include "mymain.h"
#include "sqlite3.h"
#include "errMsg.h"
#include "task_sqlite.h"

typedef unsigned char uint8_t;

table_ErrRecord_t err_table_CBDev[]={
{1,"换电设备",3,"SWE01-3-001","急停报警"},
{2,"换电设备",3,"SWE01-3-002","备用"},
{3,"换电设备",3,"SWE01-3-003","自检轴使能丢失报警"},
{4,"换电设备",3,"SWE01-3-004","初始化RGV与停车台交互中报警"},
{5,"换电设备",3,"SWE01-3-005","初始化码垛机货叉回中间位禁止报警"},
{6,"换电设备",3,"SWE01-3-006","初始化超时报警"},
{30,"换电设备",3,"SWE01-3-030","Rx_侧推位置扭矩参数设置异常报警"},
{31,"换电设备",3,"SWE01-3-031","EMS通讯异常报警"},
{50,"换电设备",3,"SWE01-3-050","指令触发_当前模式不满足报警"},
{51,"换电设备",3,"SWE01-3-051","指令触发_换电_停车台状态不满足报警"},
{52,"换电设备",3,"SWE01-3-052","指令触发_换电_RGV状态不满足报警"},
{53,"换电设备",3,"SWE01-3-053","指令触发_换电_缓存仓状态不满足报警"},
{54,"换电设备",3,"SWE01-3-054","指令触发_换电_码垛机状态不满足报警"},
{55,"换电设备",3,"SWE01-3-055","指令触发_换电_停车台车在位异常报警"},
{56,"换电设备",3,"SWE01-3-056","指令触发_换电_RGV包状态异常报警"},
{57,"换电设备",3,"SWE01-3-057","指令触发_换电_Tz包状态异常报警"},
{58,"换电设备",3,"SWE01-3-058","指令触发_换电_缓存仓包状态异常报警"},
{59,"换电设备",3,"SWE01-3-059","指令触发_换电_码垛机包状态异常报警"},
{60,"换电设备",3,"SWE01-3-060","指令触发_换电_存取仓号异常报警"},
{61,"换电设备",3,"SWE01-3-061","指令触发_换电_车型号异常报警"},
{62,"换电设备",3,"SWE01-3-062","指令触发_子程序_停车台状态不满足报警"},
{63,"换电设备",3,"SWE01-3-063","指令触发_子程序_RGV状态不满足报警"},
{64,"换电设备",3,"SWE01-3-064","指令触发_子程序_缓存仓状态不满足报警"},
{65,"换电设备",3,"SWE01-3-065","指令触发_子程序_码垛机状态不满足报警"},
{66,"换电设备",3,"SWE01-3-066","指令触发_存仓号异常报警"},
{67,"换电设备",3,"SWE01-3-067","指令触发_取仓号异常报警"},
{68,"换电设备",3,"SWE01-3-068","指令触发_车型号异常报警"},
{69,"换电设备",3,"SWE01-3-069","指令触发_滑出库状态异常报警"},
{100,"换电设备",3,"SWE01-3-100","仓门电机静点报警"},
{101,"换电设备",3,"SWE01-3-101","仓门电机动点报警"},
{102,"换电设备",3,"SWE01-3-102","前推杆静点报警"},
{103,"换电设备",3,"SWE01-3-103","前推杆动点报警"},
{104,"换电设备",3,"SWE01-3-104","中推杆静点报警"},
{105,"换电设备",3,"SWE01-3-105","中推杆动点报警"},
{106,"换电设备",3,"SWE01-3-106","后推杆静点报警"},
{107,"换电设备",3,"SWE01-3-107","后推杆动点报警"},
{108,"换电设备",3,"SWE01-3-108","停车台有车报警"},
{109,"换电设备",3,"SWE01-3-109","停车台有车在位异常"},
{110,"换电设备",3,"SWE01-3-110","停车台无车报警"},
{111,"换电设备",3,"SWE01-3-111","RGV有包报警"},
{112,"换电设备",3,"SWE01-3-112","RGV有包在位异常报警"},
{113,"换电设备",3,"SWE01-3-113","RGV无包报警"},
{114,"换电设备",3,"SWE01-3-114","Tz有包报警"},
{115,"换电设备",3,"SWE01-3-115","Tz有包在位异常报警"},
{116,"换电设备",3,"SWE01-3-116","Tz无包报警"},
{117,"换电设备",3,"SWE01-3-117","缓存仓有包报警"},
{118,"换电设备",3,"SWE01-3-118","缓存仓有包在位异常报警"},
{119,"换电设备",3,"SWE01-3-119","缓存仓无包报警"},
{120,"换电设备",3,"SWE01-3-120","码垛机有包报警"},
{121,"换电设备",3,"SWE01-3-121","码垛机无包报警"},
{122,"换电设备",3,"SWE01-3-122","车底盘左右在位异常报警"},
{123,"换电设备",3,"SWE01-3-123","车底盘电池检测异常报警"},
{124,"换电设备",3,"SWE01-3-124","前推杆未在静点报警"},
{125,"换电设备",3,"SWE01-3-125","中推杆未在静点报警"},
{126,"换电设备",3,"SWE01-3-126","后推杆未在静点报警"},
{127,"换电设备",3,"SWE01-3-127","前推杆未在动点报警"},
{128,"换电设备",3,"SWE01-3-128","中推杆未在动点报警"},
{129,"换电设备",3,"SWE01-3-129","后推杆未在动点报警"},
{130,"换电设备",3,"SWE01-3-130","Rx拆电池异常报警"},
{131,"换电设备",3,"SWE01-3-131","Rx装电池异常报警"},
{134,"换电设备",3,"SWE01-3-134","仓N推杆静点报警"},
{135,"换电设备",3,"SWE01-3-135","仓N推杆动点报警"},
{136,"换电设备",3,"SWE01-3-136","车前轮定位异常报警"},
{137,"换电设备",3,"SWE01-3-137","车后轮定位异常报警"},
{138,"换电设备",3,"SWE01-3-138","仓门未打开报警"},
{139,"换电设备",3,"SWE01-3-139","仓N推杆未缩回报警"},
{140,"换电设备",3,"SWE01-3-140","左车门打开报警"},
{141,"换电设备",3,"SWE01-3-141","右车门打开报警"},
{200,"换电设备",3,"SWE01-3-200","货叉中间位报警"},
{201,"换电设备",3,"SWE01-3-201","PFx_A车换型位报警"},
{202,"换电设备",3,"SWE01-3-202","PFx_Eu300位报警"},
{203,"换电设备",3,"SWE01-3-203","PFL_松开位报警"},
{204,"换电设备",3,"SWE01-3-204","PFR_松开位报警"},
{205,"换电设备",3,"SWE01-3-205","PBL_松开位报警"},
{206,"换电设备",3,"SWE01-3-206","PBR_松开位报警"},
{207,"换电设备",3,"SWE01-3-207","PFz_泊车位报警"},
{208,"换电设备",3,"SWE01-3-208","PBz_泊车位报警"},
{209,"换电设备",3,"SWE01-3-209","Rx_拆解位报警"},
{210,"换电设备",3,"SWE01-3-210","RL_中间位报警"},
{211,"换电设备",3,"SWE01-3-211","RR_中间位报警"},
{212,"换电设备",3,"SWE01-3-212","Ry_接驳位报警"},
{213,"换电设备",3,"SWE01-3-213","Ry_拆装位报警"},
{214,"换电设备",3,"SWE01-3-214","Tz_底层位报警"},
{215,"换电设备",3,"SWE01-3-215","Tz_缓存上位报警"},
{216,"换电设备",3,"SWE01-3-216","Tz_码垛机位报警"},
{217,"换电设备",3,"SWE01-3-217","Sy_中间位报警"},
{218,"换电设备",3,"SWE01-3-218","Sz_缓存上位报警"},
{219,"换电设备",3,"SWE01-3-219","Sz_缓存下位报警"},
{220,"换电设备",3,"SWE01-3-220","Sz_平台上位报警"},
{221,"换电设备",3,"SWE01-3-221","Sz_平台下位报警"},
{222,"换电设备",3,"SWE01-3-222","Sz_仓上位报警"},	
{223,"换电设备",3,"SWE01-3-223","Sz_仓下位报警"},	
{224,"换电设备",3,"SWE01-3-224","Sx_平台位_Eu300报警"},
{225,"换电设备",3,"SWE01-3-225","Sx_平台位_Eu5报警"},
{226,"换电设备",3,"SWE01-3-226","Sx_目标仓位报警"},
{300,"换电设备",3,"SWE01-3-300","Ry_网络连接异常"},
{301,"换电设备",3,"SWE01-3-301","Ry_极限传感器报警"},
{302,"换电设备",3,"SWE01-3-302","Ry_驱动器报警"},
{305,"换电设备",3,	"SWE01-3-305","Ry_定位超时报警"},
{306,"换电设备",3,	"SWE01-3-306","Ry_备用07报警"},
{307,"换电设备",3,	"SWE01-3-307","Ry_备用08报警"},
{308,"换电设备",3,	"SWE01-3-308","Rx_网络连接异常"},
{309,"换电设备",3,	"SWE01-3-309","Rx_极限传感器报警"},
{310,"换电设备",3,	"SWE01-3-310","Rx_驱动器报警"},
{313,"换电设备",3,	"SWE01-3-313","Rx_定位超时报警"},
{314,"换电设备",3,	"SWE01-3-314","Rx_备用07报警"},
{315,"换电设备",3,	"SWE01-3-315","Rx_备用08报警"},
{316,"换电设备",3,	"SWE01-3-316","RL_网络连接异常"},
{317,"换电设备",3,	"SWE01-3-317","RL_极限传感器报警"},
{318,"换电设备",3,	"SWE01-3-318","RL_驱动器报警"},
{321,"换电设备",3,	"SWE01-3-321",	"RL_定位超时报警"},
{322,"换电设备",3,	"SWE01-3-322",	"RL_备用07报警"},
{323,"换电设备",3,	"SWE01-3-323",	"RL_备用08报警"},
{324,"换电设备",3,	"SWE01-3-324",	"RR_网络连接异常"},
{325,"换电设备",3,	"SWE01-3-325",	"RR_极限传感器报警"},
{326,"换电设备",3,	"SWE01-3-326",	"RR_驱动器报警"	},
{329,"换电设备",3,	"SWE01-3-329",	"RR_定位超时报警"},
{330,"换电设备",3,	"SWE01-3-330",	"RR_备用07报警"},
{331,"换电设备",3,	"SWE01-3-331",	"RR_备用08报警"},
{332,"换电设备",3,	"SWE01-3-332",	"Tz_网络连接异常"},
{333,"换电设备",3,	"SWE01-3-333",	"Tz_极限传感器报警"},
{334,"换电设备",3,	"SWE01-3-334",	"Tz_驱动器报警"},		
{337,"换电设备",3,	"SWE01-3-337",	"Tz_定位超时报警"},
{338,"换电设备",3,	"SWE01-3-338",	"Tz_备用07报警"},
{339,"换电设备",3,	"SWE01-3-339",	"Tz_备用08报警"},
{340,"换电设备",3,	"SWE01-3-340",	"PFx_网络连接异常"},
{341,"换电设备",3,	"SWE01-3-341",	"PFx_极限传感器报警"},
{342,"换电设备",3,	"SWE01-3-342",	"PFx_驱动器报警"	},
{345,"换电设备",3,	"SWE01-3-345",	"PFx_定位超时报警"},
{346,"换电设备",3,	"SWE01-3-346",	"PFx_备用07报警"},
{347,"换电设备",3,	"SWE01-3-347",	"PFx_备用08报警"},
{348,"换电设备",3,	"SWE01-3-348",	"PFz_网络连接异常"},
{349,"换电设备",3,	"SWE01-3-349",	"PFz_极限传感器报警"},
{350,"换电设备",3,	"SWE01-3-350",	"PFz_驱动器报警"},
{353,"换电设备",3,	"SWE01-3-353",	"PFz_定位超时报警"},
{354,"换电设备",3,	"SWE01-3-354",	"PFz_备用07报警"},
{355,"换电设备",3,	"SWE01-3-355",	"PFz_备用08报警"},
{356,"换电设备",3,	"SWE01-3-356",	"PBz_网络连接异常"},
{357,"换电设备",3,	"SWE01-3-357",	"PBz_极限传感器报警"},
{358,"换电设备",3,	"SWE01-3-358",	"PBz_驱动器报警"},		
{361,"换电设备",3,	"SWE01-3-361",	"PBz_定位超时报警"},
{362,"换电设备",3,	"SWE01-3-362",	"PBz_备用07报警"},
{363,"换电设备",3,	"SWE01-3-363",	"PBz_备用08报警"},
{364,"换电设备",3,	"SWE01-3-364",	"PFL_网络连接异常"},
{365,"换电设备",3,	"SWE01-3-365",	"PFL_极限传感器报警"},
{366,"换电设备",3,	"SWE01-3-366",	"PFL_驱动器报警"},
{369,"换电设备",3,	"SWE01-3-369",	"PFL_定位超时报警"},
{370,"换电设备",3,"SWE01-3-370","PFL_备用01"},
{371,"换电设备",3,"SWE01-3-371","PFL_备用02"},
{372,"换电设备",3,"SWE01-3-372","PFR_网络连接异常"},
{373,"换电设备",3,"SWE01-3-373","PFR_极限传感器报警"},
{374,"换电设备",3,"SWE01-3-374","PFR_驱动器报警"},
{377,"换电设备",3,"SWE01-3-377","PFR_定位超时报警"},
{378,"换电设备",3,"SWE01-3-378","PFR_备用01"},
{379,"换电设备",3,"SWE01-3-379","PFR_备用02"},
{380,"换电设备",3,"SWE01-3-380","PBL_网络连接异常"},
{381,"换电设备",3,"SWE01-3-381","PBL_极限传感器报警"},
{382,"换电设备",3,"SWE01-3-382","PBL_驱动器报警"	},	
{385,"换电设备",3,"SWE01-3-385","PBL_定位超时报警"},
{386,"换电设备",3,"SWE01-3-386","PBL_备用01"},
{387,"换电设备",3,"SWE01-3-387","PBL_备用02"},
{388,"换电设备",3,"SWE01-3-388","PBR_网络连接异常"},
{389,"换电设备",3,"SWE01-3-389","PBR_极限传感器报警"},
{390,"换电设备",3,"SWE01-3-390","PBR_驱动器报警"},
{393,"换电设备",3,"SWE01-3-393","PBR_定位超时报警"},
{394,"换电设备",3,"SWE01-3-394","PBR_备用01"},
{395,"换电设备",3,"SWE01-3-395","PBR_备用02"},
{396,"换电设备",3,"SWE01-3-396","Sx_网络连接异常"},
{397,"换电设备",3,"SWE01-3-397","Sx_极限传感器报警"},
{398,"换电设备",3,"SWE01-3-398","Sx_驱动器报警"	},	
{401,"换电设备",3,"SWE01-3-401","Sx_定位超时报警"},
{402,"换电设备",3,"SWE01-3-402","Sx_备用07报警"},
{403,"换电设备",3,"SWE01-3-403","Sx_备用08报警"},
{404,"换电设备",3,"SWE01-3-404","Sy_网络连接异常"},
{405,"换电设备",3,"SWE01-3-405","Sy_极限传感器报警"},
{406,"换电设备",3,"SWE01-3-406","Sy_驱动器报警"},
{409,"换电设备",3,"SWE01-3-409","Sy_定位超时报警"},
{410,"换电设备",3,"SWE01-3-410","Sy_备用07报警"},
{411,"换电设备",3,"SWE01-3-411","Sy_备用08报警"},
{412,"换电设备",3,"SWE01-3-412","Sz_网络连接异常"},
{413,"换电设备",3,"SWE01-3-413","Sz_极限传感器报警"},
{414,"换电设备",3,"SWE01-3-414","Sz_驱动器报警"	},
{417,"换电设备",3,"SWE01-3-417","Sz_定位超时报警"},
{418,"换电设备",3,"SWE01-3-418","Sz_备用07报警"},
{419,"换电设备",3,"SWE01-3-419","Sz_备用08报警"},
{420,"换电设备",3,"SWE01-3-420","Tz不在底层位报警"},

};

table_ErrRecord_t err_table_ChgerDev[]={

{81,"充电设备",2,"CHG0N-2-081","BMS心跳超时"},
{80,"充电设备",2,"CHG0N-2-080","AC/DC模块通信故障"},

{3,"充电设备",2,"CHG0N-2-003","ACDC输入过压告警"},
{4,"充电设备",2,"CHG0N-2-004","ACDC输入欠压告警"},
{5,"充电设备",2,"CHG0N-2-005","ACDC三相输入不平衡告警"},
{6,"充电设备",2,"CHG0N-2-006","ACDC三相输入缺相告警"},
{7,"充电设备",2,"CHG0N-2-007","ACDC模块严重不均流"},
{8,"充电设备",2,"CHG0N-2-008","ACDC模块通信中断"},
{9,"充电设备",2,"CHG0N-2-009","ACDC输出过压告警"},
{10,"充电设备",2,"CHG0N-2-010","ACDC过温告警"},
{11,"充电设备",2,"CHG0N-2-011","ACDC风扇故障告警"},
{12,"充电设备",2,"CHG0N-2-012","ACDC模块保护告警"},
{13,"充电设备",2,"CHG0N-2-013","ACDC模块故障告警"},
{14,"充电设备",2,"CHG0N-2-014","ACDC模块放电异常"},
{15,"充电设备",2,"CHG0N-2-015","CHU与EMS通信故障"},

{20,"充电设备",2,"CHG0N-2-020","充电机故障中止"},
{21,"充电设备",2,"CHG0N-2-021","BMS主动中止"},
{43,"充电设备",2,"CHG0N-2-043","BMS接收CRM报文超时"},
{44,"充电设备",2,"CHG0N-2-044","BMS接收CST报文超时"},
{45,"充电设备",2,"CHG0N-2-045","BMS接收CTS报文超时"},
{46,"充电设备",2,"CHG0N-2-046","BMS接收CML报文超时"},
{47,"充电设备",2,"CHG0N-2-047","BMS接收CRO报文超时"},
{48,"充电设备",2,"CHG0N-2-048","BMS接收CCS报文超时"},
{49,"充电设备",2,"CHG0N-2-049","BMS接收CSD报文超时"},
{50,"充电设备",2,"CHG0N-2-050","CHU接收BRM报文超时"},
{51,"充电设备",2,"CHG0N-2-051","CHU接收BCP报文超时"},
{52,"充电设备",2,"CHG0N-2-052","CHU接收BRO报文超时"},
{53,"充电设备",2,"CHG0N-2-053","CHU接收BCS报文超时"},
{54,"充电设备",2,"CHG0N-2-054","CHU接收BCL报文超时"},
{55,"充电设备",2,"CHG0N-2-055","CHU接收BST报文超时"},
{56,"充电设备",2,"CHG0N-2-056","CHU接收BSD报文超时"},

{67,"充电设备",2,"CHG0N-2-067","BMS绝缘故障"},
{68,"充电设备",2,"CHG0N-2-068","BMS输出连接器过温故障"},
{69,"充电设备",2,"CHG0N-2-069","BMS元件过温故障"},
{70,"充电设备",2,"CHG0N-2-070","BMS充电连接器故障"},
{71,"充电设备",2,"CHG0N-2-071","BMS电池组温度过高故障"},
{72,"充电设备",2,"CHG0N-2-072","BMS高压继电器故障"},
{73,"充电设备",2,"CHG0N-2-073","BMS其他故障"},
{74,"充电设备",2,"CHG0N-2-074","BMS电流过大"},
{75,"充电设备",2,"CHG0N-2-075","BMS电压异常"},
{76,"充电设备",2,"CHG0N-2-076","CHU检测电池单体过压"},
{77,"充电设备",2,"CHG0N-2-077","CHU检测电池总电压过压"},
{78,"充电设备",2,"CHG0N-2-078","CHU检测电池温度过高"},
{79,"充电设备",2,"CHG0N-2-079","CHU检测插拔机构温度过高"},

};

table_ErrRecord_t err_table_FireCntrl[]={

{1,"消防设备",1,"FFS01-1-001","消防设备硬件故障"},
{2,"消防设备",4,"FFS01-4-002","消防火警"},
};

table_ErrRecord_t err_table_AirCon[]={

{2	,"空调设备",1,"ACS0N-1-002","柜内温度传感器故障"},
{3	,"空调设备",1,"ACS0N-1-003","冷凝盘管温度传感器故障"},
{4	,"空调设备",1,"ACS0N-1-004","柜外温度传感器故障"},
{5	,"空调设备",1,"ACS0N-1-005","柜内湿度传感器故障"},
{6	,"空调设备",1,"ACS0N-1-006","压缩机欠流故障"},
{7	,"空调设备",1,"ACS0N-1-007","压缩机过载故障"},
{8	,"空调设备",1,"ACS0N-1-008","加热器欠流故障"},
{9	,"空调设备",1,"ACS0N-1-009","加热器过载故障"},
{10,"空调设备",1,"ACS0N-1-010","内风机告警"},
{11,"空调设备",1,"ACS0N-1-011","外风机告警"},
{12,"空调设备",1,"ACS0N-1-012","柜内高温告警"},
{13,"空调设备",1,"ACS0N-1-013","柜内低温告警"},
{14,"空调设备",1,"ACS0N-1-014","电源电压超限告警"},
{15,"空调设备",1,"ACS0N-1-015","外部故障告警"},

};

/*自检测故障*/
table_ErrRecord_t err_table_SelfChkErr[]={
/*通信故障*/
{0,"云端通信",3,"CPF01-3-000","EMS与云端通信故障"},
{0,"换电设备",3,"SWE01-3-000","EMS与换电设备通信故障"},
{0,"充电设备",2,"CHG0N-2-000","EMS与充电设备通信故障"},
{0,"空调设备",1,"ACS0N-1-000","EMS与空调设备通信故障"},

{0,"消防设备",1,"FFS01-1-000","EMS与消防设备通信故障"},
{0,"直流电表",1,"DEM0N-1-000","EMS与直流电表通信故障"},
{0,"交流电表",1,"AEM01-1-000","EMS与交流电表通信故障"},
/*电池包热失控故障*/
{0,"电池包",4,"BAT0N-4-000","电池包热失控故障"},
/*门禁开启*/
{1,"门禁",3,"EGD01-3-001","巡检门开启"},
{1,"门禁",3,"EGD02-3-001","电池仓门开启"},
{1,"门禁",3,"EGD03-3-001","维护门开启"},
{1,"门禁",3,"OGD01-3-001","入口光栅触发"},
{1,"门禁",3,"OGD02-3-001","出口光栅触发"},

/*雷击故障*/
{1,"雷击",3,"LSA01-3-001","充电柜雷击故障"},
{1,"雷击",3,"LSA02-3-001","配电柜雷击故障"},
/*水浸故障*/
{1,"水浸",3,"WLG01-3-001","水浸故障"},
/*充电系统故障*/
{0,"充电系统",3,"CHG00-3-000","所有充电仓位均有2级故障"},

};

table_ErrRecord_t err_table_db[]={
/*交流电表*/
{1,"交流电表",1,"AEM01-1-001","A相过压"},
{2,"交流电表",1,"AEM01-1-002","B相过压"},
{3,"交流电表",1,"AEM01-1-003","C相过压"},
{4,"交流电表",1,"AEM01-1-004","A相失压"},
{5,"交流电表",1,"AEM01-1-005","B相失压"},
{6,"交流电表",1,"AEM01-1-006","C相失压"},
{7,"交流电表",1,"AEM01-1-007","A相逆向"},
{8,"交流电表",1,"AEM01-1-008","B相逆向"},
{9,"交流电表",1,"AEM01-1-009","C相逆向"},
/*直流电表*/
{10,"直流电表",1,"NULL","NULL"},
};



int GetErrIndex_Ce(int errCode)
{
	int i=0;
	int index = -1;
	int array_len = sizeof(err_table_CBDev) /sizeof(table_ErrRecord_t);
	for(i=0;i<array_len;i++)
	{
		if(err_table_CBDev[i].Err_code == errCode)
		{
			index = i;break;
		}
	}	
	return index;
}

int GetErrIndex_Chger(int errCode)
{
	int i=0;
	int index = -1;
	int array_len = sizeof(err_table_ChgerDev)/sizeof(table_ErrRecord_t);
	// printf("array_len = %d\n",array_len);
	for(i=0;i<array_len;i++)
	{
		if(err_table_ChgerDev[i].Err_code == errCode)
		{
			index = i;break;
		}
	}	
	return index;
}

/*换电设备*/
int CE_Err_Code = 0;
int CE_Err_Code_last = 0;
/*充电设备*/
uint8_t ChgerDev_Err_Sta[20] = {0};//当前故障状态
uint8_t ChgerDev_Err_LastSta[20] = {0};//上传故障状态
/*空调故障*/
uint8_t AirCon_Err_Sta[3][14] = {0};//当前故障状态
uint8_t AirCon_Err_LastSta[3][14] = {0};//上传故障状态
/*消防故障*/
uint8_t FireCon_Err_Sta[2] = {0};//当前故障状态
uint8_t FireCon_Err_LastSta[2] = {0};//上传故障状态
/*自检测故障 故障类型*/
uint8_t SelfChkErr_EMS_Cloud_ComErr= {0};
uint8_t SelfChkErr_EMS_Cloud_ComErr_last= {0};
uint8_t SelfChkErr_EMS_CeDev_ComErr= {0};
uint8_t SelfChkErr_EMS_CeDev_ComErr_last= {0};
uint8_t SelfChkErr_EMS_ChgerDev_ComErr[20]= {0};
uint8_t SelfChkErr_EMS_ChgerDev_ComErr_last[20]= {0};
uint8_t SelfChkErr_EMS_AirCon_ComErr= {0};
uint8_t SelfChkErr_EMS_AirCon_ComErr_last= {0};
uint8_t SelfChkErr_EMS_FireCon_ComErr= {0};
uint8_t SelfChkErr_EMS_FireCon_ComErr_last= {0};
uint8_t SelfChkErr_ACMeter_ComErr= {0};
uint8_t SelfChkErr_ACMeter_ComErr_last= {0};
uint8_t SelfChkErr_DCMeter_ComErr[18]= {0};
uint8_t SelfChkErr_DCMeter_ComErr_last[18]= {0};
//*************************************************************
// uint8_t SelfChkErr_BattHeat_Err[20] = {0};//电池包热失控故障
// uint8_t SelfChkErr_BattHeat_Err_last[20]= {0};//电池包热失控故障
uint8_t SelfChkErr_DoorStop_Err[3] = {0};//门禁开启
uint8_t SelfChkErr_DoorStop_Err_last[3] = {0};//门禁开启
uint8_t SelfChkErr_Lightr_Err[2] = {0,0};//光栅
uint8_t SelfChkErr_Lightr_Err_last[2] = {0};//光栅

uint8_t SelfChkErr_Thunder_Err[2] = {0};//雷击故障
uint8_t SelfChkErr_Thunder_Err_last[2] = {0};//雷击故障
uint8_t SelfChkErr_Water_Err= {0};//水浸故障
uint8_t SelfChkErr_Water_Err_last= {0};//水浸故障
// uint8_t SelfChkErr_ChgerSys_Err= {0};//充电系统故障
// uint8_t SelfChkErr_ChgerSys_Err_last= {0};//充电系统故障


/*******************************
 * 读取各个模块的故障信息
 * ****************************/
void SysErr_interface()
{
	int i=0;
	/*换电设备*/
	CE_Err_Code = (uint16_t)CE_ErrData[0] *256 + (uint16_t)CE_ErrData[1];
	/*充电设备*/
	for(i=0;i<18;++i)
	{ChgerDev_Err_Sta[i] = Get_CHUErrStop_flg[i];}
	/*空调故障*/
	for(i=0;i<3;i++)
	{
		AirCon_Err_Sta[i][0] = (AirCon_Err1[i] & 0x01)>>0;
		AirCon_Err_Sta[i][1] = (AirCon_Err1[i] & 0x02)>>1;
		AirCon_Err_Sta[i][2] = (AirCon_Err1[i] & 0x04)>>2;
		AirCon_Err_Sta[i][3] = (AirCon_Err1[i] & 0x08)>>3;
		AirCon_Err_Sta[i][4] = (AirCon_Err1[i] & 0x10)>>4;
		AirCon_Err_Sta[i][5] = (AirCon_Err1[i] & 0x20)>>5;
		AirCon_Err_Sta[i][6] = (AirCon_Err1[i] & 0x40)>>6;
		AirCon_Err_Sta[i][7] = (AirCon_Err1[i] & 0x80)>>7;
		AirCon_Err_Sta[i][8] = (AirCon_Err2[i] & 0x01)>>0;
		AirCon_Err_Sta[i][9] = (AirCon_Err2[i] & 0x02)>>1;
		AirCon_Err_Sta[i][10] = (AirCon_Err2[i] & 0x10)>>4;
		AirCon_Err_Sta[i][11] = (AirCon_Err2[i] & 0x20)>>5;
		AirCon_Err_Sta[i][12] = (AirCon_Err3[i] & 0x01)>>0;
		AirCon_Err_Sta[i][13] = (AirCon_Err3[i] & 0x02)>>1;
	}
	/*消防故障*/
	FireCon_Err_Sta[0] = 0;//硬件故障
	FireCon_Err_Sta[1] = FireContr_GetFire_flg;//火警告警
	//**************************************//
	/*自检测故障 故障类型*/
	SelfChkErr_EMS_Cloud_ComErr = (MCloud_Comm_flg == 2);/* 云端通信 */
	SelfChkErr_EMS_CeDev_ComErr = (MCE_Comm_flg == 2);/* 换电设备 */
	for(i = 0;i<18;i++)
	{
		SelfChkErr_EMS_ChgerDev_ComErr[i]= EMS_CHU_ComErr_flg[i];/*充电设备通信故障*/
		SelfChkErr_DCMeter_ComErr[i]= 0;/*DC METER设备通信故障*/
	}
	SelfChkErr_EMS_AirCon_ComErr = (AirCon_conn_flg == 2);/*空调设备通信故障*/
	SelfChkErr_EMS_FireCon_ComErr = (FireContr_conn_flg == 2);/*消防设备通信故障*/
	SelfChkErr_ACMeter_ComErr= 0;/*AC METER设备通信故障*/

	/*IO CARD自检测故障*/
	SelfChkErr_DoorStop_Err[0] = MIO_DoorErr_buf[0];//门禁1开启告警
	SelfChkErr_DoorStop_Err[1] = MIO_DoorErr_buf[1];//门禁2开启告警
	SelfChkErr_DoorStop_Err[2] = MIO_DoorErr_buf[2];//门禁3开启告警
	SelfChkErr_Lightr_Err[0] = MIO_LightrErr_buf[0];//光栅1告警
	SelfChkErr_Lightr_Err[1] = MIO_LightrErr_buf[1];//光栅2告警
	SelfChkErr_Thunder_Err[0] = MIO_ThunderErr_buf[0];//雷击故障
	SelfChkErr_Thunder_Err[1] = MIO_ThunderErr_buf[1];
	SelfChkErr_Water_Err = MIO_Water_err;//水浸故障0
}


inline uint8_t getCBDevErrRisingSig()
{
	uint8_t ret = ((CE_Err_Code_last !=  CE_Err_Code) && (CE_Err_Code != 0));
	CE_Err_Code_last = CE_Err_Code;
	
	return ret;
}

inline uint8_t getChgerErrRisingSig(int DevIndex)
{
	uint8_t ret = (ChgerDev_Err_Sta[DevIndex] == 1 && ChgerDev_Err_LastSta[DevIndex] == 0);
	ChgerDev_Err_LastSta[DevIndex] = ChgerDev_Err_Sta[DevIndex];
	
	return ret;
}

inline uint8_t getAirConErrRisingSig(int DevIndex,int ErrIndex)
{
	uint8_t ret = (AirCon_Err_Sta[DevIndex][ErrIndex] == 1 && AirCon_Err_LastSta[DevIndex][ErrIndex] == 0);
	AirCon_Err_LastSta[DevIndex][ErrIndex] = AirCon_Err_Sta[DevIndex][ErrIndex];
	
	return ret;
}

inline uint8_t getFireConErrRisingSig(int ErrIndex)
{
	uint8_t ret = (FireCon_Err_Sta[ErrIndex] == 1 && FireCon_Err_LastSta[ErrIndex] == 0);
	FireCon_Err_LastSta[ErrIndex] = FireCon_Err_Sta[ErrIndex];
	
	return ret;
}

inline uint8_t getEMS_Cloud_ComErrRisingSig()
{
	uint8_t ret = (SelfChkErr_EMS_Cloud_ComErr == 1 && SelfChkErr_EMS_Cloud_ComErr_last == 0);
	SelfChkErr_EMS_Cloud_ComErr_last = SelfChkErr_EMS_Cloud_ComErr;

	return ret;
}

inline uint8_t getEMS_CeDev_ComErrRisingSig()
{
	uint8_t ret = (SelfChkErr_EMS_CeDev_ComErr == 1 && SelfChkErr_EMS_CeDev_ComErr_last == 0);
	SelfChkErr_EMS_CeDev_ComErr_last = SelfChkErr_EMS_CeDev_ComErr;
	
	return ret;
}

inline uint8_t getEMS_ChgerDev_ComErrRisingSig(int DevIndex)
{
	uint8_t ret = (SelfChkErr_EMS_ChgerDev_ComErr_last[DevIndex] == 0 && SelfChkErr_EMS_ChgerDev_ComErr[DevIndex] == 1);
	SelfChkErr_EMS_ChgerDev_ComErr_last[DevIndex] = SelfChkErr_EMS_ChgerDev_ComErr[DevIndex];
	
	return ret;
}

inline uint8_t getEMS_AirCon_ComErrRisingSig()
{
	uint8_t ret = (SelfChkErr_EMS_AirCon_ComErr == 1 && SelfChkErr_EMS_AirCon_ComErr_last == 0);
	SelfChkErr_EMS_AirCon_ComErr_last = SelfChkErr_EMS_AirCon_ComErr;
	return ret;
}

inline uint8_t getEMS_FireCon_ComErrRisingSig()
{
	uint8_t ret = (SelfChkErr_EMS_FireCon_ComErr == 1 && SelfChkErr_EMS_FireCon_ComErr_last == 0);
	SelfChkErr_EMS_FireCon_ComErr_last = SelfChkErr_EMS_FireCon_ComErr;
	return ret;
}

inline uint8_t getDCMeter_ComErrRisingSig(int DevIndex)
{
	uint8_t ret = (SelfChkErr_DCMeter_ComErr[DevIndex] == 1 && SelfChkErr_DCMeter_ComErr_last[DevIndex] == 0);
	SelfChkErr_DCMeter_ComErr_last[DevIndex] = SelfChkErr_DCMeter_ComErr[DevIndex];
	return ret;
}

inline uint8_t getACMeter_ComErrRisingSig(int DevIndex)
{
	uint8_t ret = (SelfChkErr_ACMeter_ComErr == 1 && SelfChkErr_ACMeter_ComErr_last == 0);
	SelfChkErr_ACMeter_ComErr_last = SelfChkErr_ACMeter_ComErr;
	return ret;
}

inline uint8_t getDoorStop_ErrrrRisingSig(int DevIndex)
{
	uint8_t ret = (SelfChkErr_DoorStop_Err[DevIndex] == 1 && SelfChkErr_DoorStop_Err_last[DevIndex] == 0);
	SelfChkErr_DoorStop_Err_last[DevIndex] = SelfChkErr_DoorStop_Err[DevIndex];
	return ret;
}

inline uint8_t getLightr_ErrrrRisingSig(int DevIndex)
{
	uint8_t ret = (SelfChkErr_Lightr_Err[DevIndex] == 1 && SelfChkErr_Lightr_Err_last[DevIndex] == 0);
	SelfChkErr_Lightr_Err_last[DevIndex] = SelfChkErr_Lightr_Err[DevIndex];
	return ret;
}

inline uint8_t getThunder_ErrrrRisingSig(int DevIndex)
{
	uint8_t ret = (SelfChkErr_Thunder_Err[DevIndex] == 1 && SelfChkErr_Thunder_Err_last[DevIndex] == 0);
	SelfChkErr_Thunder_Err_last[DevIndex] = SelfChkErr_Thunder_Err[DevIndex];
	return ret;
}

inline uint8_t getWater_ErrrrRisingSig()
{
	uint8_t ret = (SelfChkErr_Water_Err == 1 && SelfChkErr_Water_Err_last == 0);
	SelfChkErr_Water_Err_last = SelfChkErr_Water_Err;
	return ret;
}

// 


uint8_t Sql_ErrData_buf[5000];
void wrtData2SqlDB(sqlite3 *db,const char *sqlCmdBuf)
{
	int ret;
	char *zErrMsg = NULL;
	ret = sqlite3_exec( db , sqlCmdBuf , 0 , 0 , &zErrMsg );
	if(ret)
	{
		uint8_t cmdInfo[25];
		strncpy(cmdInfo,sqlCmdBuf,sizeof(cmdInfo));
//		printf( "%s err.ret = %d\n",cmdInfo,ret) ;
//		printf( "err:%s\n",zErrMsg) ;
	}
}

/*故障信息写入*//*每次发生故障调用*/
void Sql_ErrInter_Deal2(table_ErrRecord_t errdata_t,int DevIndex)
{
	sql_ErrRecord_t ErrRecord;

	strcpy(ErrRecord.ErrDev,errdata_t.ErrDev);/* 故障设备 */
	ErrRecord.ErrDev_count = DevIndex;/* 故障设备序号 */
	strcpy(ErrRecord.Err_code , errdata_t.Err_code_str);/* 故障代码*/
	ErrRecord.Err_Lev = errdata_t.Err_Lev;/* 故障level*/
	strcpy(ErrRecord.ErrName,errdata_t.ErrName);/* 故障miao shu*/

	sprintf(Sql_ErrData_buf,"INSERT INTO 故障记录 \
		VALUES (Null,\"%s\",%d,%d,\"%s\",\"%s\",\"%s\");",
		ErrRecord.ErrDev,ErrRecord.ErrDev_count,ErrRecord.Err_Lev,ErrRecord.Err_code,ErrRecord.ErrName,NowTime);

	// printf("Sql_ErrData_buf%s\n",Sql_ErrData_buf);
}

void sql_WriteErrData(sqlite3 *db)
{
	int i=0,j=0;
	/* 换电设备 1*/
	if(getCBDevErrRisingSig())
	{
//		printf( "write data to sql db.CE_Err_Code=%d\n",CE_Err_Code);
		int ret=0;
		if( -1 != (ret = GetErrIndex_Ce(CE_Err_Code)))
		{
			Sql_ErrInter_Deal2(err_table_CBDev[ret],1);
			wrtData2SqlDB(db,Sql_ErrData_buf);
		}	
	}
	
	/* 充电设备 2*/
	for(i=0;i<18;i++)
	{
		if(getChgerErrRisingSig(i))
		{
//			printf( "write data to sql db.MChg_ErrStop_code =%d\n",MChg_ErrStop_code[i]);
			int ret=0;
			if(-1 != (ret = GetErrIndex_Chger(MChg_ErrStop_code[i])))
			{
				Sql_ErrInter_Deal2(err_table_ChgerDev[ret],i+1);
				wrtData2SqlDB(db,Sql_ErrData_buf);
			}					
		}
	}
	
	/* 空调 3*/
	for(i=0;i<3;i++)
	{
		for(j=0;j<14;j++)
		{
			if(getAirConErrRisingSig(i,j))
			{
//				printf( "write data to sql db. AirCon=%d,err %d=\n",i,j);
				Sql_ErrInter_Deal2(err_table_AirCon[j],i+1);
				wrtData2SqlDB(db,Sql_ErrData_buf);
			}	
		}
	}

	/*消防 4*/
	for(i=0;i<2;++i)
	{
		if(getFireConErrRisingSig(i))
		{
//			printf( "write data to sql db.err %d=\n",i);
			Sql_ErrInter_Deal2(err_table_FireCntrl[i],1);
			wrtData2SqlDB(db,Sql_ErrData_buf);
		}
	}

	//*********************************************************************
	/*自检测故障 故障类型*/
	/* 云端通信故障 1*/
	if(getEMS_Cloud_ComErrRisingSig())
	{
//		printf( "write data to sql db. EMS_Cloud_ComErr\n");
		Sql_ErrInter_Deal2(err_table_SelfChkErr[0],1);
		wrtData2SqlDB(db,Sql_ErrData_buf);
	}
	
	/* 换电设备通信故障 2*/
	if(getEMS_CeDev_ComErrRisingSig())
	{
//		printf( "write data to sql db. EMS_CeDev_ComErr\n");
		Sql_ErrInter_Deal2(err_table_SelfChkErr[1],1);
		wrtData2SqlDB(db,Sql_ErrData_buf);
	}
	
	/* 充电设备通信故障 3*/
	for(i=0;i<18;i++)
	{
		if(getEMS_ChgerDev_ComErrRisingSig(i))
		{
//			printf( "write data to sql db. EMS_ChgerDev_ComErr\n");
			Sql_ErrInter_Deal2(err_table_SelfChkErr[2],i+1);
			wrtData2SqlDB(db,Sql_ErrData_buf);
		}
	}
	/* 空调通信故障 4*/
	if(getEMS_AirCon_ComErrRisingSig())
	{
//		printf( "write data to sql db. EMS_AirCon_ComErr\n");
		Sql_ErrInter_Deal2(err_table_SelfChkErr[3],1);
		wrtData2SqlDB(db,Sql_ErrData_buf);
	}
	/* 消防通信故障 5*/
	if(getEMS_FireCon_ComErrRisingSig())
	{
//		printf( "write data to sql db. EMS_FireCon_ComErr\n");
		Sql_ErrInter_Deal2(err_table_SelfChkErr[4],1);
		wrtData2SqlDB(db,Sql_ErrData_buf);
	}
	/* DC通信故障 6*/
	for(i=0;i<18;i++)
	{	
		if(getDCMeter_ComErrRisingSig(i))
		{
//			printf( "write data to sql db.DCMeter_ComErr \n");
			Sql_ErrInter_Deal2(err_table_SelfChkErr[5],1+i);
			wrtData2SqlDB(db,Sql_ErrData_buf);
		}
	}
	/* AC通信故障 7*/
	if(getACMeter_ComErrRisingSig(i))
	{
//		printf( "write data to sql db.ACMeter_ComErr \n");
		Sql_ErrInter_Deal2(err_table_SelfChkErr[6],1);
		wrtData2SqlDB(db,Sql_ErrData_buf);
	}
	/* DOOR1-3 8*/
	for(i=0;i<3;i++)
	{
		if(getDoorStop_ErrrrRisingSig(i))
		{
//			printf( "write data to sql db.DOOR \n");
			Sql_ErrInter_Deal2(err_table_SelfChkErr[8+i],1+i);
			wrtData2SqlDB(db,Sql_ErrData_buf);		
		}
	}
	/* Lightr1-2 9*/
	for(i=0;i<2;i++)
	{
		if(getLightr_ErrrrRisingSig(i))
		{
//			printf( "getLightr_ErrrrRisingSig = %d \n",SelfChkErr_Lightr_Err[1]);
//			printf( "getLightr_ErrrrRisingSig = %d \n",SelfChkErr_Lightr_Err[0]);
//			printf( "getLightr_ErrrrRisingSig = %d \n",MIO_LightrErr_buf[1]);
//			printf( "getLightr_ErrrrRisingSig = %d \n",MIO_LightValue_null[1]);
			Sql_ErrInter_Deal2(err_table_SelfChkErr[11+i],1+i);
			wrtData2SqlDB(db,Sql_ErrData_buf);		
		}
	}
	/* Thunder 1-2 10*/
	for(i=0;i<2;i++)
	{
		if(getThunder_ErrrrRisingSig(i))
		{
//			printf( "write data to sql db.Thunder \n");
			Sql_ErrInter_Deal2(err_table_SelfChkErr[13+i],1+i);
			wrtData2SqlDB(db,Sql_ErrData_buf);		
		}
	}
	/* Water  11*/
	if(getWater_ErrrrRisingSig())
	{
//		printf( "write data to sql db.Water \n");
		Sql_ErrInter_Deal2(err_table_SelfChkErr[15],1);
		wrtData2SqlDB(db,Sql_ErrData_buf);		
	}
}






