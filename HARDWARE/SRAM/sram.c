#include "sram.h"	  
#include "usart.h"	
#include "lcd.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//�ⲿSRAM ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/5/14
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	 


//ʹ��NOR/SRAM�� Bank1.sector3,��ַλHADDR[27,26]=10 
//��IS61LV25616/IS62WV25616,��ַ�߷�ΧΪA0~A17 
//��IS61LV51216/IS62WV51216,��ַ�߷�ΧΪA0~A18
#define Bank1_SRAM3_ADDR    ((u32)(0x68000000))	
  						   
//��ʼ���ⲿSRAM
void FSMC_SRAM_Init(void)
{	
	GPIO_InitTypeDef  GPIO_InitStructure;
	FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
  FSMC_NORSRAMTimingInitTypeDef  readWriteTiming; 
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOD|RCC_AHB1Periph_GPIOE|RCC_AHB1Periph_GPIOF|RCC_AHB1Periph_GPIOG, ENABLE);//ʹ��PD,PE,PF,PGʱ��  
  RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FSMC,ENABLE);//ʹ��FSMCʱ��  
   
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;//PB15 �������,���Ʊ���
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ�� //PB15 �������,���Ʊ���

	GPIO_InitStructure.GPIO_Pin = (3<<0)|(3<<4)|(0XFF<<8);//PD0,1,4,5,8~15 AF OUT
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//�������
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOD, &GPIO_InitStructure);//��ʼ��  
	
  GPIO_InitStructure.GPIO_Pin = (3<<0)|(0X1FF<<7);//PE0,1,7~15,AF OUT
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//�������
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOE, &GPIO_InitStructure);//��ʼ��  
	
 	GPIO_InitStructure.GPIO_Pin = (0X3F<<0)|(0XF<<12); 	//PF0~5,12~15
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//�������
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOF, &GPIO_InitStructure);//��ʼ��  

	GPIO_InitStructure.GPIO_Pin =(0X3F<<0)| GPIO_Pin_10;//PG0~5,10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//�������
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOG, &GPIO_InitStructure);//��ʼ�� 
 
 
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource0,GPIO_AF_FSMC);//PD0,AF12
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource1,GPIO_AF_FSMC);//PD1,AF12
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource4,GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource5,GPIO_AF_FSMC); 
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource8,GPIO_AF_FSMC); 
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource9,GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource10,GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource11,GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource12,GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource13,GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource14,GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource15,GPIO_AF_FSMC);//PD15,AF12
 
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource0,GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource1,GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource7,GPIO_AF_FSMC);//PE7,AF12
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource8,GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource9,GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource10,GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource11,GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource12,GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource13,GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource14,GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource15,GPIO_AF_FSMC);//PE15,AF12
 
  GPIO_PinAFConfig(GPIOF,GPIO_PinSource0,GPIO_AF_FSMC);//PF0,AF12
  GPIO_PinAFConfig(GPIOF,GPIO_PinSource1,GPIO_AF_FSMC);//PF1,AF12
  GPIO_PinAFConfig(GPIOF,GPIO_PinSource2,GPIO_AF_FSMC);//PF2,AF12
  GPIO_PinAFConfig(GPIOF,GPIO_PinSource3,GPIO_AF_FSMC);//PF3,AF12
  GPIO_PinAFConfig(GPIOF,GPIO_PinSource4,GPIO_AF_FSMC);//PF4,AF12
  GPIO_PinAFConfig(GPIOF,GPIO_PinSource5,GPIO_AF_FSMC);//PF5,AF12
  GPIO_PinAFConfig(GPIOF,GPIO_PinSource12,GPIO_AF_FSMC);//PF12,AF12
  GPIO_PinAFConfig(GPIOF,GPIO_PinSource13,GPIO_AF_FSMC);//PF13,AF12
  GPIO_PinAFConfig(GPIOF,GPIO_PinSource14,GPIO_AF_FSMC);//PF14,AF12
  GPIO_PinAFConfig(GPIOF,GPIO_PinSource15,GPIO_AF_FSMC);//PF15,AF12
	
  GPIO_PinAFConfig(GPIOG,GPIO_PinSource0,GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOG,GPIO_PinSource1,GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOG,GPIO_PinSource2,GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOG,GPIO_PinSource3,GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOG,GPIO_PinSource4,GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOG,GPIO_PinSource5,GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOG,GPIO_PinSource10,GPIO_AF_FSMC);
	
 	  
 	readWriteTiming.FSMC_AddressSetupTime = 0x00;	 //��ַ����ʱ�䣨ADDSET��Ϊ1��HCLK 1/36M=27ns
  readWriteTiming.FSMC_AddressHoldTime = 0x00;	 //��ַ����ʱ�䣨ADDHLD��ģʽAδ�õ�	
  readWriteTiming.FSMC_DataSetupTime = 0x08;		 ////���ݱ���ʱ�䣨DATAST��Ϊ9��HCLK 6*9=54ns	 	 
  readWriteTiming.FSMC_BusTurnAroundDuration = 0x00;
  readWriteTiming.FSMC_CLKDivision = 0x00;
  readWriteTiming.FSMC_DataLatency = 0x00;
  readWriteTiming.FSMC_AccessMode = FSMC_AccessMode_A;	 //ģʽA 
    

 
  FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM3;//  ��������ʹ��NE3 ��Ҳ�Ͷ�ӦBTCR[4],[5]��
  FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable; 
  FSMC_NORSRAMInitStructure.FSMC_MemoryType =FSMC_MemoryType_SRAM;// FSMC_MemoryType_SRAM;  //SRAM   
  FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;//�洢�����ݿ��Ϊ16bit  
  FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode =FSMC_BurstAccessMode_Disable;// FSMC_BurstAccessMode_Disable; 
  FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
	FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait=FSMC_AsynchronousWait_Disable;
  FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;   
  FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;  
  FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;	//�洢��дʹ�� 
  FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;  
  FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable; // ��дʹ����ͬ��ʱ��
  FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;  
  FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &readWriteTiming;
  FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &readWriteTiming; //��дͬ��ʱ��

  FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);  //��ʼ��FSMC����

 	FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM3, ENABLE);  // ʹ��BANK1����3										  
											
}
	  														  
//��ָ����ַ(WriteAddr+Bank1_SRAM3_ADDR)��ʼ,����д��n���ֽ�.
//pBuffer:�ֽ�ָ��
//WriteAddr:Ҫд��ĵ�ַ
//n:Ҫд����ֽ���
void FSMC_SRAM_WriteBuffer(u8* pBuffer,u32 WriteAddr,u32 n)
{
	for(;n!=0;n--)  
	{										    
		*(vu8*)(Bank1_SRAM3_ADDR+WriteAddr)=*pBuffer;	  
		WriteAddr++;
		pBuffer++;
	}   
}																			    
//��ָ����ַ((WriteAddr+Bank1_SRAM3_ADDR))��ʼ,��������n���ֽ�.
//pBuffer:�ֽ�ָ��
//ReadAddr:Ҫ��������ʼ��ַ
//n:Ҫд����ֽ���
void FSMC_SRAM_ReadBuffer(u8* pBuffer,u32 ReadAddr,u32 n)
{
	for(;n!=0;n--)  
	{											    
		*pBuffer++=*(vu8*)(Bank1_SRAM3_ADDR+ReadAddr);    
		ReadAddr++;
	}  
} 
 

//�����ڴ�
//*des:Ŀ�ĵ�ַ
//*src:Դ��ַ
//n:��Ҫ���Ƶ��ڴ泤��(�ֽ�Ϊ��λ)
void mymemcpy(u32 SrcAddr,u32 DestAddr,u32 n)  
{  
	u8 temp = 0;
	for(;n!=0;n--)
	{
		temp=*(vu8*)(SrcAddr);
		*(vu8*)(DestAddr)=temp;
		SrcAddr++;
		DestAddr++;
	}
}  

void FSMC_SRAM_PictureDeal(u16 sx,u16 sy,u16 ex,u16 ey, u16 *newAddr,u16 *oldAddr)
{
 u16 height,width;	
	u16 y1, y2; 
	u16 i,j;

	width=ex-sx+1; 			//�õ����Ŀ��
	height=ey-sy+1;			//�߶�
		for(i=0;i<height;i++)
	{
 		LCD_SetCursor(sx,sy+i);   	//���ù��λ�� 
		 LCD_WriteRAM_Prepare();     //��ʼд��GRAM
		for(j=0;j<width;j++)
		{
			 	y1 = 299*(newAddr[i*width+j]>>11) + 587*((newAddr[i*width+j]>>5)&0x003f) + 114*(newAddr[i*width+j]&0x001f);
	   	y2 = 299*(oldAddr[i*width+j]>>11) + 587*((oldAddr[i*width+j]>>5)&0x003f) + 114*(oldAddr[i*width+j]&0x001f);
			  if(abs(y1 -y2) > ADD_METHOD_VALUE)
					{
						 LCD->LCD_RAM = YELLOW;
					}
					else
					{
						LCD->LCD_RAM = BLACK;
					}
		}
	}		  
	
}





static Coordinate_st cd_xy = {0};
Arithmetic_Param_st ArP =
{	
	2,
	ADD_METHOD_VALUE,
	ABSADD_METHOD_VALUE,
	BY_METHOD_VALUE,	
	1,
	1,
	1,
	0
};
hotpoint_st Xres_Tab[PICTURE_WIDHT+2] = {0};
hotpoint_st Yres_Tab[PICTURE_HEIGHT+2] = {0};


void PictureDealReturn_out()
{
		vu16 i,j=0;
		cd_xy.update = 0;
		cd_xy.x = 0;
		cd_xy.y = 0;
		for(i=0;i<PICTURE_HEIGHT;i++)Yres_Tab[i].cnt = 0;
		for(j=0;j<PICTURE_WIDHT;j++)Xres_Tab[j].cnt = 0;
}

Coordinate_st *FSMC_SRAM_PictureDeal1(u16 sx,u16 sy,u16 ex,u16 ey, u16 *newAddr,u16 *oldAddr)
{
 u16 height,width;	
 u16 y1, y2; 
 int minY,minX = 0;
 vu16 tmp_a,tmp_b,tmp_c = 0;
	vu16 x_id = 0;	//240
	vu16 y_id = 0;  //320
		vu16 i = 0;
	vu16 j = 0;  
	vu8 *p2;
	vu16 a_Totol = 0;
	width=ex-sx+1; 			//�õ����Ŀ��
	height=ey-sy+1;			//�߶�
	
	PictureDealReturn_out();
	
	p2 = (vu8*)(Bank1_SRAM3_ADDR+_2V_IMAGE_ADDR);
	
		for(y_id=0;y_id<height;y_id++)
	{
 		LCD_SetCursor(sx,sy+y_id);   	//���ù��λ�� 
		 LCD_WriteRAM_Prepare();     //��ʼд��GRAM
		for(x_id=0;x_id<width;x_id++)
		{
			
			 	y1 = 299*(newAddr[y_id*width+x_id]>>11) + 587*((newAddr[y_id*width+x_id]>>5)&0x003f) + 114*(newAddr[y_id*width+x_id]&0x001f);
	   	 y2 = 299*(oldAddr[y_id*width+x_id]>>11) + 587*((oldAddr[y_id*width+x_id]>>5)&0x003f) + 114*(oldAddr[y_id*width+x_id]&0x001f);
 
//			 y1 = (newAddr[y_id*width+x_id]>>13) +((newAddr[y_id*width+x_id]>>3)&0x001f) + (newAddr[y_id*width+x_id]&0x0003);
//	   	 y2 = (oldAddr[y_id*width+x_id]>>13) + ((oldAddr[y_id*width+x_id]>>3)&0x001f) + (oldAddr[y_id*width+x_id]&0x0003);
		
			if(ArP.Method == 0 || ArP.Method == 1)
			{
			    if(abs(y1 -y2) > ADD_METHOD_VALUE)
					{
//						LCD->LCD_RAM = GREEN;
						
						if(ArP.Dsp_2V)
						{
							*p2=0x5f;
							*(p2+1)=0x5f;
						}
						
						Xres_Tab[x_id].index=x_id;
						Xres_Tab[x_id].cnt++;

						Yres_Tab[y_id].index=y_id;
						Yres_Tab[y_id].cnt++;
					}
					else
					{
						//LCD->LCD_RAM = BLACK;
						if(ArP.Dsp_2V)
						{
							*p2=0x00;
							*(p2+1)=0x00;
						}
					}
			}
		}
	}		  
	if(ArP.DspFocus)
	{
	
		//ð������ �Ӵ�С
		hotpoint_st temp={0};
		for(i=0;i<PICTURE_HEIGHT-1;i++)//n��Ԫ�أ�ֻ��Ҫn-1��ð��
		{
			for(j=1;j<PICTURE_HEIGHT-i;j++)//j=1,Ϊ�˺�ǰһ��a[j-1]���бȽϡ�n-i��ʾÿ��������һ��Ԫ�ر����õ�������ȷ��λ�ã�֮�����ƶ���
			{
				if(Yres_Tab[j-1].cnt < Yres_Tab[j].cnt)
				{
					temp.index = Yres_Tab[j].index;
					temp.cnt = Yres_Tab[j].cnt;			
					Yres_Tab[j].index = Yres_Tab[j-1].index;
					Yres_Tab[j].cnt = Yres_Tab[j-1].cnt;
					Yres_Tab[j-1].index = temp.index;
					Yres_Tab[j-1].cnt = temp.cnt;
				}
			}
		}
		for(i=0;i<PICTURE_WIDHT-1;i++)//n��Ԫ�أ�ֻ��Ҫn-1��ð��
		{
			for(j=1;j<PICTURE_WIDHT-i;j++)//j=1,Ϊ�˺�ǰһ��a[j-1]���бȽϡ�n-i��ʾÿ��������һ��Ԫ�ر����õ�������ȷ��λ�ã�֮�����ƶ���
			{
				if(Xres_Tab[j-1].cnt < Xres_Tab[j].cnt)
				{
					temp.index = Xres_Tab[j].index;
					temp.cnt = Xres_Tab[j].cnt;			
					Xres_Tab[j].index = Xres_Tab[j-1].index;
					Xres_Tab[j].cnt = Xres_Tab[j-1].cnt;
					Xres_Tab[j-1].index = temp.index;
					Xres_Tab[j-1].cnt = temp.cnt;
				}
			}
		}
		
		//��̬�ֲ����е�
		for(i=0;i<PICTURE_WIDHT;i++)
		{
			if(Xres_Tab[i].cnt<5)     
			{
				minX = i;
				break;
			}
		}
		if(minX<8)//ֻ��6���ж� ����
		{
			PictureDealReturn_out();
			return &cd_xy;
		}
		for(i=0;i<PICTURE_HEIGHT;i++)
		{
			if(Yres_Tab[i].cnt<5)
			{
				minY = i;
				break;
			}
		}
		if(minY<8)	//ֻ��6���ж�  ����
		{
			PictureDealReturn_out();
			return &cd_xy;
		}
		
/*
		tmp_a = (unsigned int)(minX*0.2);//����
		tmp_b = (unsigned int)(minX*0.8);//����
		tmp_c = (unsigned int)(minX*0.6);//�м�
*/
		tmp_a = 2;
		tmp_b = minX - 2;
		tmp_c = minX - 4;
		if(tmp_c >= 2)	//�������вŴ���
		{
			for(j = tmp_a ;j<=tmp_b;j++)//ȡ�м�60%
			{
				a_Totol += Xres_Tab[j].index;		
			}
			cd_xy.x = (unsigned int)(a_Totol/tmp_c);
			a_Totol = 0;
		}
		else
		{			
			PictureDealReturn_out();
			return &cd_xy;				
		}
/*
		tmp_a = (unsigned int)(minY*0.2);
		tmp_b = (unsigned int)(minY*0.8);
		tmp_c = (unsigned int)(minY*0.6);
*/
		tmp_a = 2;
		tmp_b = minY - 2;
		tmp_c = minY - 4;
		if(tmp_c >= 2)
		{
			for(j = tmp_a ;j<=tmp_b;j++)//ȡ�м�60%
			{
				a_Totol += Yres_Tab[j].index;		
			}
		
			cd_xy.y=(unsigned int)(a_Totol/tmp_c);
			a_Totol = 0;
		}
		else
		{			
			PictureDealReturn_out();
			return &cd_xy;				
		}

		cd_xy.update = 1;
		

		
	}


	
	return &cd_xy;
	

}
