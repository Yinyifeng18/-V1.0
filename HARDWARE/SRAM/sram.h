#ifndef __SRAM_H
#define __SRAM_H															    
#include "sys.h" 
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//�ⲿSRAM ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/16
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////

											  
void FSMC_SRAM_Init(void);
void FSMC_SRAM_WriteBuffer(u8* pBuffer,u32 WriteAddr,u32 NumHalfwordToWrite);
void FSMC_SRAM_ReadBuffer(u8* pBuffer,u32 ReadAddr,u32 NumHalfwordToRead);


void fsmc_sram_test_write(u32 addr,u8 data);
u8 fsmc_sram_test_read(u32 addr);
void mymemcpy(u32 SrcAddr,u32 DestAddr,u32 n);  
void FSMC_SRAM_PictureDeal(u16 sx,u16 sy,u16 ex,u16 ey, u16 *newAddr,u16 *oldAddr);



/*************************************ͼ������**********************************/
#define ADD_METHOD_VALUE 6500
//#define ADD_METHOD_VALUE 5
#define ABSADD_METHOD_VALUE 85
#define BY_METHOD_VALUE 20000

/*************************************ͼ������**********************************/

#define PICTURE_WIDHT 240
#define PICTURE_HEIGHT 320

#define _2V_IMAGE_ADDR ((u32)(0x00060000))	//��ֵ�洢λ��

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
	int Compress_rate;//ѹ����
	int threshold_value; //ֱ�Ӽӷ���ֵ RGB565 �ӷ�С��128��ȡMAX100�� 
	int threshold_value_abs;//����ֵ�ۼӷ���ֵ�˷�С��65535��ȡMAX40000��
	int threshold_value_m;//�˷���ֵ�˷�С��65535��ȡMAX40000��
	int Method;   //1��ֱ���ۼӷ� 2������ֵ�ۼӷ�  4:�˻��� 0:Ĭ��ֱ�Ӽӷ�
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

