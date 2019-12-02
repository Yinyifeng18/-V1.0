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
//ALIENTEK 探索者STM32F407开发板 实验35
//摄像头 实验 -库函数版本
//技术支持：www.openedv.com
//淘宝店铺：http://eboard.taobao.com  
//广州市星翼电子科技有限公司  
//作者：正点原子 @ALIENTEK


/***************************  图像缓存相关 *******************************/
/*用做DMA的双缓冲 buf*/
#define RGB_buf_SIZE  2048
__align(4) u32 RGB_buf1[RGB_buf_SIZE];
__align(4) u32 RGB_buf2[RGB_buf_SIZE];

u32 *RGB_BUF = (u32 *)0x68000000;/*将图像的缓存定义到外部SRAM 前一帧 */

u32 *RGB_BUF1 = (u32 *)(0x68000000+0x00030000);/*将图像的缓存定义到外部SRAM 后一帧 */

u32 *RGB_BUF2 = (u32 *)(0x68000000+0x00060000);/*将图像的缓存定义到外部SRAM 二值 */


volatile u32 ImageDateLenght=0;/*图像数据大小  单位u32*/
volatile u8  buf_sta=0;   /*0 图像还没有采集完成，1 采集完成  2 处理完成*/



/*************************************************************************/

/*************************************************************************/
void RGB_dcmi_rx_callback(void);
extern void (*dcmi_rx_callback)(void);


void RGB_funHandler(void)
{
	u16 i;
	u16 rlen;//剩余数据长度
	u32 *pbuf;
    if(buf_sta==0)	//jpeg数据还未采集完?
    {
        DMA2_Stream1->CR&=~(1<<0);		//停止当前传输
        while(DMA2_Stream1->CR&0X01);	//等待DMA2_Stream1可配置 
        rlen=RGB_buf_SIZE-DMA2_Stream1->NDTR;//得到剩余数据长度	
        pbuf=RGB_BUF+ImageDateLenght;//偏移到有效数据末尾,继续添加
        if(DMA2_Stream1->CR&(1<<19))for(i=0;i<rlen;i++)pbuf[i]=RGB_buf2[i];//读取buf1里面的剩余数据
        else for(i=0;i<rlen;i++)pbuf[i]=RGB_buf1[i];//读取buf0里面的剩余数据 
        ImageDateLenght+=rlen;			//加上剩余长度
        buf_sta=1; 				//标记JPEG数据采集完按成,等待其他函数处理
    }
    if(buf_sta==2)	//上一次的jpeg数据已经被处理了
    {
        DMA2_Stream1->NDTR=RGB_buf_SIZE;//传输长度为jpeg_buf_size*4字节
        DMA2_Stream1->CR|=1<<0;			//重新传输
        buf_sta=0;					//标记数据未采集
        ImageDateLenght=0;				//数据重新开始
    }
}

//RGB565测试
//RGB数据直接显示在LCD上面
void rgb565_test(void)
{ 
 u8  Step = 0;
	LCD_Clear(WHITE);
 POINT_COLOR=RED; 
 Coordinate_st* xy={0};

		
	LCD_ShowString(30,50,200,16,16,"ALIENTEK STM32F4");
	LCD_ShowString(30,70,200,16,16,"OV2640 RGB565 Mode");
	
	OV2640_RGB565_Mode();	//RGB565模式
	My_DCMI_Init();			//DCMI配置
 dcmi_rx_callback=RGB_dcmi_rx_callback;/*设置回调函数*/
 DCMI_DMA_Init_DOUBLE_BUF((u32)&RGB_buf1,(u32)&RGB_buf2,RGB_buf_SIZE, /*两个buf 和buf的大小*/
                            DMA_MemoryDataSize_Word,DMA_MemoryInc_Enable);/*一次传输32bit，内存地址自增*/
    
 	OV2640_OutSize_Set(PICTURE_WIDHT,PICTURE_HEIGHT); /*设定摄像头输出的图像大小*/
	DCMI_Start(); 		//启动传输
	while(1)
	{  
//        if(buf_sta == 1)
//        {
//            /*将内存中的图像数据显示到屏幕上面*/
//            LCD_Color_Fill(0,                      /* 将图像一直保持在LCD的X中间 */
//                            40,                                            /* 设定Y的起始地址 */
//                            239,  /* X图像结束坐标 */
//                            279,(u16 *)RGB_BUF+16);     /* Y轴的结束地址 
//            buf_sta=2;
//        }
				switch(Step)
				{
				  case 0:
						{
							 /* 如果捕捉到数据，就拷贝到RGB_BUF1 */
							 if(buf_sta == 1)
								{

									  mymemcpy(0x68000000, 0x68000000+0x00030000, PICTURE_WIDHT*PICTURE_HEIGHT*2+16);
																		  LCD_Color_Fill((lcddev.width - PICTURE_WIDHT)/2,                      /* 将图像一直保持在LCD的X中间 */
                            (lcddev.height - PICTURE_HEIGHT)/2,                                            /* 设定Y的起始地址 */
                            (lcddev.width - PICTURE_WIDHT)/2+PICTURE_WIDHT-1,  /* X图像结束坐标 */
                             (lcddev.height - PICTURE_HEIGHT)/2+PICTURE_HEIGHT-1,(u16 *)RGB_BUF1+16);    
								  	buf_sta=2;
									  Step++;
								}
								break;
						}
						case 1:
						{
							   /* 在捕捉一帧数据 */
							   if(buf_sta == 1)
										{

//              LCD_Color_Fill((lcddev.width - PictureWidth)/2,                      /* 将图像一直保持在LCD的X中间 */
//                            (lcddev.height - PictureHeight)/2,                                            /* 设定Y的起始地址 */
//                            (lcddev.width - PictureWidth)/2+PictureWidth-1,  /* X图像结束坐标 */
//                             (lcddev.height - PictureHeight)/2+PictureHeight-1,(u16 *)RGB_BUF+16);     

												buf_sta=2;
										 	Step++;
										}  
									break;
						}
						/* 获得两帧数据，对比 */
						case 2:
						{
							  xy = FSMC_SRAM_PictureDeal1(0,0,239,319, (u16 *)RGB_BUF+16,(u16 *)RGB_BUF1+16);
							  
  							Step++;
							  break;
						}
						/* 显示左边位置 */
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
											
											/* 通过串口发送坐标 */
											
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
	pbuf=RGB_BUF+ImageDateLenght;//偏移到有效数据末尾
	if(DMA2_Stream1->CR&(1<<19))//buf0已满,正常处理buf1
	{ 
		for(i=0;i<RGB_buf_SIZE;i++)pbuf[i]=RGB_buf1[i];//读取buf1里面的数据
		ImageDateLenght+=RGB_buf_SIZE;//偏移
	}else //buf1已满,正常处理buf0
	{
		for(i=0;i<RGB_buf_SIZE;i++)pbuf[i]=RGB_buf2[i];//读取buf2里面的数据
		ImageDateLenght+=RGB_buf_SIZE;//偏移 
	} 	
}
int main(void)
{ 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);  //初始化延时函数
	uart_init(115200);		//初始化串口波特率为115200
	usart2_init(42,115200);		//初始化串口2波特率为115200
	LED_Init();					//初始化LED 
 	LCD_Init();					//LCD初始化  
 	KEY_Init();					//按键初始化 
//	TIM3_Int_Init(10000-1,8400-1);//10Khz计数,1秒钟中断一次
//	FSMC_SRAM_Init();/*初始化SRAM*/
// 	usmart_dev.init(84);		//初始化USMART
// 	POINT_COLOR=RED;//设置字体为红色 
//	LCD_ShowString(30,50,200,16,16,(u8*)"Explorer STM32F4");	
//	LCD_ShowString(30,70,200,16,16,(u8*)"OV2640 TEST");	
//	LCD_ShowString(30,90,200,16,16,(u8*)"ATOM@ALIENTEK");
//	LCD_ShowString(30,110,200,16,16,(u8*)"2014/5/14");  	 
//	while(OV2640_Init())//初始化OV2640
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
