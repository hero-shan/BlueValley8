#ifndef _TASK_LCD_AUDIO_H_
#define _TASK_LCD_AUDIO_H_

#include "sys/types.h"
#include "unistd.h"

typedef struct _LcdShow
{
    uint8_t showLineNumber;

    uint8_t LineShow_buf[4][200];
    uint16_t LineShow_buf_len[4];

}lcdShow_t;


typedef struct _LcdControl
{
    uint8_t show_flg;
 //   uint8_t eventDescribe[50];    

    uint8_t playAudio_flg;
    uint8_t lcdShowData[4][200];
 //   lcdShow_t lcdShowData;
    // uint8_t Line1Show_buf[200];
    // uint8_t Line2Show_buf[200];
    // uint8_t Line3Show_buf[200];
    // uint8_t Line4Show_buf[200];

    // uint16_t Line1Show_buf_len;
    // uint16_t Line2Show_buf_len;
    // uint16_t Line3Show_buf_len;
    // uint16_t Line4Show_buf_len;

}lcdControl_t;


typedef struct 
{
    /* data */
    uint8_t LCD_ShowStationFault0_flg;/*显示 换电站故障暂不支持换电*/
    uint8_t LCD_ShowStationFault1_flg;/*显示 换电站故障暂时不支持换电请驶离*/
    uint8_t LCD_ShowStationFault2_flg;/*显示 换电站故障暂时不支持换电请驶离*/
    uint8_t LCD_ShowFireErr0_flg;/*显示 消防故障请尽快驶离换电站*/
    uint8_t LCD_ShowFireErr1_flg;/*显示 消防故障请远离换电站*/
    uint8_t LCD_ShowCarIn_flg;/*显示 欢迎进入蓝谷能源服务站*/
    uint8_t LCD_ShowCarNotInNet_flg;/*显示 车辆为未入网车辆请驶离*/
    uint8_t LCD_ShowCarAbnormal_flg;/*显示 车辆异常请驶离*/
    uint8_t LCD_ShowStationNoBattery_flg;/*显示 无可换电池包请驶离*/
    uint8_t LCD_ShowOrderCanceled_flg;/*显示 订单取消请驶离*/
    uint8_t LCD_ShowCarRising_flg;/*显示 车辆升举请勿上下车*/
    uint8_t LCD_ShowChangeBattSuccess_flg;/*显示 换电完成欢迎下次光临*/
    
}lcdControlSig_t;



unsigned short CRC16(unsigned char *puchMsg, unsigned short  usDataLen);
void lcd_show_buf_build(uint8_t *line1buf,uint8_t *line2buf,uint8_t *line3buf,
                        uint8_t *Audiobuf,uint8_t EN_playAudio,uint8_t General_Display);
uint32_t Area_ShowBuf_bulid(uint8_t *line2buf,uint8_t *line3buf,uint8_t *line4buf,uint8_t *Audiobuf,uint8_t EN_playAudio);

uint32_t Fixed_ShowBuf_bulid(uint8_t *line1buf);
//void lcd_show_buf_build(lcdShow_t show_t);
//void audio_buf_build(lcdShow_t show_t);

int u2g(char *inbuf, size_t inlen, char *outbuf, size_t outlen);


int LoadIniFile();

#endif // !_TASK_LCD_AUDIO_H_