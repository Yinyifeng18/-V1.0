#ifndef __SRAM_H
#define __SRAM_H															    
#include "sys.h" 
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//外部SRAM 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/16
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////

											  
void FSMC_SRAM_Init(void);
void FSMC_SRAM_WriteBuffer(u8* pBuffer,u32 WriteAddr,u32 NumHalfwordToWrite);
void FSMC_SRAM_ReadBuffer(u8* pBuffer,u32 ReadAddr,u32 NumHalfwordToRead);


void fsmc_sram_test_write(u32 addr,u8 data);
u8 fsmc_sram_test_read(u32 addr);
void mymemcpy(u32 SrcAddr,u32 DestAddr,u32 n);  
void FSMC_SRAM_PictureDeal(u16 sx,u16 sy,u16 ex,u16 ey, u16 *newAddr,u16 *oldAddr);



/*************************************图像处理部分**********************************/
#define ADD_METHOD_VALUE 6500
//#define ADD_METHOD_VALUE 5
#define ABSADD_METHOD_VALUE 85
#define BY_METHOD_VALUE 20000

/*************************************图像处理部分**********************************/

#define PICTURE_WIDHT 240
#define PICTURE_HEIGHT 320

#define _2V_IMAGE_ADDR ((u32)(0x00060000))	//二值存储位置

typedef struct Coordinate
{
	u8 update;
	vu16 x;
	vu16 y;
	vu16 xbak;
	vu16 ybak;
}Coordinate_st;

typedef struct Arithmetic_Param
{
	int Compress_rate;//压缩比
	int threshold_value; //直接加法阈值 RGB565 加法小于128（取MAX100） 
	int threshold_value_abs;//绝对值累加法阈值乘法小于65535（取MAX40000）
	int threshold_value_m;//乘法阈值乘法小于65535（取MAX40000）
	int Method;   //1：直接累加法 2：绝对值累加法  4:乘积法 0:默认直接加法
	u8 DspFocus;
	u8 Dsp_2V;
	int res;
}Arithmetic_Param_st;
typedef struct hotpoint
{
	unsigned int index;
	unsigned int cnt;
}hotpoint_st;

Coordinate_st *FSMC_SRAM_PictureDeal1(u16 sx,u16 sy,u16 ex,u16 ey, u16 *newAddr,u16 *oldAddr);

/**********************************************************************************/

#endif

