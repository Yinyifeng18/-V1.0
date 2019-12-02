#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "lcd.h"
#include "usmart.h"  
#include "usart2.h"  
#include "timer.h" 
#include "ov2640.h" 
#include "dcmi.h" 
#include "sram.h"
//ALIENTEK ̽����STM32F407������ ʵ��35
//����ͷ ʵ�� -�⺯���汾
//����֧�֣�www.openedv.com
//�Ա����̣�http://eboard.taobao.com  
//������������ӿƼ����޹�˾  
//���ߣ�����ԭ�� @ALIENTEK


/***************************  ͼ�񻺴���� *******************************/
/*����DMA��˫���� buf*/
#define RGB_buf_SIZE  2048
__align(4) u32 RGB_buf1[RGB_buf_SIZE];
__align(4) u32 RGB_buf2[RGB_buf_SIZE];

u32 *RGB_BUF = (u32 *)0x68000000;/*��ͼ��Ļ��涨�嵽�ⲿSRAM ǰһ֡ */

u32 *RGB_BUF1 = (u32 *)(0x68000000+0x00030000);/*��ͼ��Ļ��涨�嵽�ⲿSRAM ��һ֡ */

u32 *RGB_BUF2 = (u32 *)(0x68000000+0x00060000);/*��ͼ��Ļ��涨�嵽�ⲿSRAM ��ֵ */


volatile u32 ImageDateLenght=0;/*ͼ�����ݴ�С  ��λu32*/
volatile u8  buf_sta=0;   /*0 ͼ��û�вɼ���ɣ�1 �ɼ����  2 �������*/



/*************************************************************************/

/*************************************************************************/
void RGB_dcmi_rx_callback(void);
extern void (*dcmi_rx_callback)(void);


void RGB_funHandler(void)
{
	u16 i;
	u16 rlen;//ʣ�����ݳ���
	u32 *pbuf;
    if(buf_sta==0)	//jpeg���ݻ�δ�ɼ���?
    {
        DMA2_Stream1->CR&=~(1<<0);		//ֹͣ��ǰ����
        while(DMA2_Stream1->CR&0X01);	//�ȴ�DMA2_Stream1������ 
        rlen=RGB_buf_SIZE-DMA2_Stream1->NDTR;//�õ�ʣ�����ݳ���	
        pbuf=RGB_BUF+ImageDateLenght;//ƫ�Ƶ���Ч����ĩβ,�������
        if(DMA2_Stream1->CR&(1<<19))for(i=0;i<rlen;i++)pbuf[i]=RGB_buf2[i];//��ȡbuf1�����ʣ������
        else for(i=0;i<rlen;i++)pbuf[i]=RGB_buf1[i];//��ȡbuf0�����ʣ������ 
        ImageDateLenght+=rlen;			//����ʣ�೤��
        buf_sta=1; 				//���JPEG���ݲɼ��갴��,�ȴ�������������
    }
    if(buf_sta==2)	//��һ�ε�jpeg�����Ѿ���������
    {
        DMA2_Stream1->NDTR=RGB_buf_SIZE;//���䳤��Ϊjpeg_buf_size*4�ֽ�
        DMA2_Stream1->CR|=1<<0;			//���´���
        buf_sta=0;					//�������δ�ɼ�
        ImageDateLenght=0;				//�������¿�ʼ
    }
}

//RGB565����
//RGB����ֱ����ʾ��LCD����
void rgb565_test(void)
{ 
 u8  Step = 0;
	LCD_Clear(WHITE);
 POINT_COLOR=RED; 
 Coordinate_st* xy={0};

		
	LCD_ShowString(30,50,200,16,16,"ALIENTEK STM32F4");
	LCD_ShowString(30,70,200,16,16,"OV2640 RGB565 Mode");
	
	OV2640_RGB565_Mode();	//RGB565ģʽ
	My_DCMI_Init();			//DCMI����
 dcmi_rx_callback=RGB_dcmi_rx_callback;/*���ûص�����*/
 DCMI_DMA_Init_DOUBLE_BUF((u32)&RGB_buf1,(u32)&RGB_buf2,RGB_buf_SIZE, /*����buf ��buf�Ĵ�С*/
                            DMA_MemoryDataSize_Word,DMA_MemoryInc_Enable);/*һ�δ���32bit���ڴ��ַ����*/
    
 	OV2640_OutSize_Set(PICTURE_WIDHT,PICTURE_HEIGHT); /*�趨����ͷ�����ͼ���С*/
	DCMI_Start(); 		//��������
	while(1)
	{  
//        if(buf_sta == 1)
//        {
//            /*���ڴ��е�ͼ��������ʾ����Ļ����*/
//            LCD_Color_Fill(0,                      /* ��ͼ��һֱ������LCD��X�м� */
//                            40,                                            /* �趨Y����ʼ��ַ */
//                            239,  /* Xͼ��������� */
//                            279,(u16 *)RGB_BUF+16);     /* Y��Ľ�����ַ 
//            buf_sta=2;
//        }
				switch(Step)
				{
				  case 0:
						{
							 /* �����׽�����ݣ��Ϳ�����RGB_BUF1 */
							 if(buf_sta == 1)
								{

									  mymemcpy(0x68000000, 0x68000000+0x00030000, PICTURE_WIDHT*PICTURE_HEIGHT*2+16);
																		  LCD_Color_Fill((lcddev.width - PICTURE_WIDHT)/2,                      /* ��ͼ��һֱ������LCD��X�м� */
                            (lcddev.height - PICTURE_HEIGHT)/2,                                            /* �趨Y����ʼ��ַ */
                            (lcddev.width - PICTURE_WIDHT)/2+PICTURE_WIDHT-1,  /* Xͼ��������� */
                             (lcddev.height - PICTURE_HEIGHT)/2+PICTURE_HEIGHT-1,(u16 *)RGB_BUF1+16);    
								  	buf_sta=2;
									  Step++;
								}
								break;
						}
						case 1:
						{
							   /* �ڲ�׽һ֡���� */
							   if(buf_sta == 1)
										{

//              LCD_Color_Fill((lcddev.width - PictureWidth)/2,                      /* ��ͼ��һֱ������LCD��X�м� */
//                            (lcddev.height - PictureHeight)/2,                                            /* �趨Y����ʼ��ַ */
//                            (lcddev.width - PictureWidth)/2+PictureWidth-1,  /* Xͼ��������� */
//                             (lcddev.height - PictureHeight)/2+PictureHeight-1,(u16 *)RGB_BUF+16);     

												buf_sta=2;
										 	Step++;
										}  
									break;
						}
						/* �����֡���ݣ��Ա� */
						case 2:
						{
							  xy = FSMC_SRAM_PictureDeal1(0,0,239,319, (u16 *)RGB_BUF+16,(u16 *)RGB_BUF1+16);
							  
  							Step++;
							  break;
						}
						/* ��ʾ���λ�� */
						case 3:
						{
							
							if((abs(xy->x - xy->xbak) > 5) || (abs(xy->y-xy->ybak) > 5))
							{
								if((xy->x == 0) && (xy->y == 0))
						  	{
							   	
							  }
								else
								{
							    printf("x=%3d,y=%3d!",xy->x,xy->y);
							 
							  		if(xy->update)
												LCD_Fill(xy->x-10,xy->y-10,xy->x+10,xy->y+10,RED);
											
											/* ͨ�����ڷ������� */
											
//											u8 Bcd_X = (((xy->x/10)<<4) + (xy->x%10));
//											u8 Bcd_Y = (((xy->y/10)<<4) + (xy->y%10));										
											Usart1SendXYVal(xy->x, xy->y);
							  
								  xy->xbak = xy->x;
								  xy->ybak = xy->y;
								}
							}
							Step = 0;
						  break;
						}
				}
        
		delay_ms(10);		
	}    
} 

void RGB_dcmi_rx_callback(void)
{ 
	u16 i;
	u32 *pbuf;
	pbuf=RGB_BUF+ImageDateLenght;//ƫ�Ƶ���Ч����ĩβ
	if(DMA2_Stream1->CR&(1<<19))//buf0����,��������buf1
	{ 
		for(i=0;i<RGB_buf_SIZE;i++)pbuf[i]=RGB_buf1[i];//��ȡbuf1���������
		ImageDateLenght+=RGB_buf_SIZE;//ƫ��
	}else //buf1����,��������buf0
	{
		for(i=0;i<RGB_buf_SIZE;i++)pbuf[i]=RGB_buf2[i];//��ȡbuf2���������
		ImageDateLenght+=RGB_buf_SIZE;//ƫ�� 
	} 	
}
int main(void)
{ 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);  //��ʼ����ʱ����
	uart_init(115200);		//��ʼ�����ڲ�����Ϊ115200
	usart2_init(42,115200);		//��ʼ������2������Ϊ115200
	LED_Init();					//��ʼ��LED 
 	LCD_Init();					//LCD��ʼ��  
 	KEY_Init();					//������ʼ�� 
//	TIM3_Int_Init(10000-1,8400-1);//10Khz����,1�����ж�һ��
//	FSMC_SRAM_Init();/*��ʼ��SRAM*/
// 	usmart_dev.init(84);		//��ʼ��USMART
// 	POINT_COLOR=RED;//��������Ϊ��ɫ 
//	LCD_ShowString(30,50,200,16,16,(u8*)"Explorer STM32F4");	
//	LCD_ShowString(30,70,200,16,16,(u8*)"OV2640 TEST");	
//	LCD_ShowString(30,90,200,16,16,(u8*)"ATOM@ALIENTEK");
//	LCD_ShowString(30,110,200,16,16,(u8*)"2014/5/14");  	 
//	while(OV2640_Init())//��ʼ��OV2640
//	{
//		LCD_ShowString(30,130,240,16,16,(u8*)"OV2640 ERR");
//		delay_ms(200);
//	    LCD_Fill(30,130,239,170,WHITE);
//		delay_ms(200);
//	}
//	LCD_ShowString(30,130,200,16,16,(u8*)"OV2640 OK");  	  
//    delay_ms(200);
//    rgb565_test(); 

//     while(1);
//					
    while(1)
				{
//					 u8 temp = (((20/10)<<4) + (20%10));
					 Usart1SendXYVal(120, 150);
					 delay_ms(200);
				}
}
