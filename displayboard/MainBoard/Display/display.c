/****************************************************************************************************/
/*			@FileName		:	display.c															  	*/
/*			@Author			:	zhengbaiqiu															*/
/*			@Version		:	v1.0																*/
/*			@Date			:	2019.11.12  														*/
/****************************************************************************************************/


/* Includes --------------------------------------------------------------------*/
#include "stm32f10x.h"

/* Project ---------------------------------------------------------------------*/
#include "maindef.h"
//#include "font.h"
#include "lcd_driver.h"	
#include "gui.h"	
#include "delay.h"
#include "display.h"
#include "apiLib.h"

/* Variable --------------------------------------------------------------------*/
extern u16 Touch_X;
extern u16 Touch_Y;

u8 mpass[4] = {0,1,2,2};

/* Function --------------------------------------------------------------------*/
void DrawTestPage(u8 *str);
void Test_Display(void);
void Display_Test(u8 test_mode,u8 test_cmd,u8 test_status,u16 dat1,u16 dat2,u8 flag);





/*****************************************************************************
 * @name       :void DrawTestPage(u8 *str)
 * @date       :2018-08-09 
 * @function   :Drawing test interface
 * @parameters :str:the start address of the Chinese and English strings
 * @retvalue   :None
******************************************************************************/ 
void DrawTestPage(u8 *str)
{
	//绘制固定栏up
	//LCD_Clear(WHITE);
	LCD_Clear(GRAY0);
	LCD_Fill(0,0,lcddev.width,20,BLUE);
	//绘制固定栏down
	LCD_Fill(0,lcddev.height-20,lcddev.width,lcddev.height,BLUE);
	POINT_COLOR=WHITE;
	//POINT_COLOR=GRAY0;
	Gui_StrCenter(0,2,WHITE,BLUE,str,16,1);//居中显示
	//Gui_StrCenter(0,lcddev.height-18,WHITE,BLUE,"http://www.zbq.com",16,1);//居中显示
	Gui_StrCenter(0,lcddev.height-18,WHITE,BLUE,"请按提示操作",16,1);//居中显示
	//绘制测试区域
	//LCD_Fill(0,20,lcddev.width,lcddev.height-20,WHITE);
}

/*****************************************************************************
 * @name       :void Display_Avoid_Error(u8 pos,u16 y,u16 dat1,u16 dat2,u8 flag)
 * @date       :2020-09-19 
 * @author     :zhengbaiqiu 
 * @function   :显示免碰撞数据
 * @parameters :pos:  1 : 左侧 2：右侧
 * @retvalue   :None
******************************************************************************/
void Display_Avoid_Error(u8 pos,u16 y,u16 dat1,u16 dat2,u8 flag)
{
	if(y <= 270)
	{
		if(pos == 1)
		{
			if(flag == 0)
			{
				Gui_DrawFont_GBK16(84,y,BLACK,GRAY0,"远:");
				Gui_DrawFont_Num16(108,y,BLACK,GRAY0,dat1);
				Gui_DrawFont_GBK16(156,y,BLACK,GRAY0,"近:");
				Gui_DrawFont_Num16(180,y,BLACK,GRAY0,dat2);
			}
			else if(flag == 1)
			{
				Gui_DrawFont_GBK16(84,y,RED,GRAY0,"远:");
				Gui_DrawFont_Num16(108,y,RED,GRAY0,dat1);
				Gui_DrawFont_GBK16(156,y,BLACK,GRAY0,"近:");
				Gui_DrawFont_Num16(180,y,BLACK,GRAY0,dat2);
			}
			else if(flag == 2)
			{
				Gui_DrawFont_GBK16(84,y,BLACK,GRAY0,"远:");
				Gui_DrawFont_Num16(108,y,BLACK,GRAY0,dat1);
				Gui_DrawFont_GBK16(156,y,RED,GRAY0,"近:");
				Gui_DrawFont_Num16(180,y,RED,GRAY0,dat2);
			}
			else if(flag == 3)
			{
				Gui_DrawFont_GBK16(84,y,RED,GRAY0,"远:");
				Gui_DrawFont_Num16(108,y,RED,GRAY0,dat1);
				Gui_DrawFont_GBK16(156,y,RED,GRAY0,"近:");
				Gui_DrawFont_Num16(180,y,RED,GRAY0,dat2);
			}
		}
		else if(pos == 2)
		{
			if(flag == 0)
			{
				Gui_DrawFont_GBK16(326,y,BLACK,GRAY0,"远:");
				Gui_DrawFont_Num16(350,y,BLACK,GRAY0,dat1);
				Gui_DrawFont_GBK16(398,y,BLACK,GRAY0,"近:");
				Gui_DrawFont_Num16(422,y,BLACK,GRAY0,dat2);
			}
			else if(flag == 1)
			{
				Gui_DrawFont_GBK16(326,y,RED,GRAY0,"远:");
				Gui_DrawFont_Num16(350,y,RED,GRAY0,dat1);
				Gui_DrawFont_GBK16(398,y,BLACK,GRAY0,"近:");
				Gui_DrawFont_Num16(422,y,BLACK,GRAY0,dat2);
			}
			else if(flag == 2)
			{
				Gui_DrawFont_GBK16(326,y,BLACK,GRAY0,"远:");
				Gui_DrawFont_Num16(350,y,BLACK,GRAY0,dat1);
				Gui_DrawFont_GBK16(398,y,RED,GRAY0,"近:");
				Gui_DrawFont_Num16(422,y,RED,GRAY0,dat2);
			}
			else if(flag == 3)
			{
				Gui_DrawFont_GBK16(326,y,RED,GRAY0,"远:");
				Gui_DrawFont_Num16(350,y,RED,GRAY0,dat1);
				Gui_DrawFont_GBK16(398,y,RED,GRAY0,"近:");
				Gui_DrawFont_Num16(422,y,RED,GRAY0,dat2);
			}
		}
	}
}

/*****************************************************************************
 * @name       :void Display_Fail(u8 item,u8 position)
 * @date       :2020-09-19 
 * @author     :zhengbaiqiu 
 * @function   :显示报错
 * @parameters :item：显示哪一项错误    position：位置
 * @retvalue   :None
******************************************************************************/
void Display_Fail(u8 item,u8 position)
{
	u16 mpos;
	
	if(position < 10)
	{
		switch(position)
		{
			case 0 :
				mpos = 50;
				break;
			case 1 :
				mpos = 74;
				break;
			case 2 :
				mpos = 98;
				break;
			case 3 :
				mpos = 122;
				break;
			case 4 :
				mpos = 146;
				break;
			case 5 :
				mpos = 170;
				break;
			case 6 :
				mpos = 194;
				break;
			case 7 :
				mpos = 218;
				break;
			case 8 :
				mpos = 242;
				break;
			case 9 :
				mpos = 266;
				break;
			default :
				break;
		}
		
		switch(item)
		{
			case TEST_GUIDE1 :
				Gui_DrawFont_GBK24(242,mpos,RED,GRAY0,"左引导供电");
				break;
			case TEST_GUIDE2 :
				Gui_DrawFont_GBK24(242,mpos,RED,GRAY0,"右引导供电");
				break;
			case TEST_GUIDE3 :
				Gui_DrawFont_GBK24(242,mpos,RED,GRAY0,"上引导");
				break;
			case TEST_GUIDE4 :
				Gui_DrawFont_GBK24(242,mpos,RED,GRAY0,"前引导");
				break;
			case TEST_LEFT_AVOID_BUMP :
				Gui_DrawFont_GBK24(242,mpos,RED,GRAY0,"左免");
				break;
			case TEST_RIGHT_AVOID_BUMP :
				Gui_DrawFont_GBK24(242,mpos,RED,GRAY0,"右免");
				break;
			case TEST_FRONT_AVOID_BUMP0 :
				Gui_DrawFont_GBK24(242,mpos,RED,GRAY0,"中免1");
				break;
			case TEST_FRONT_AVOID_BUMP1 :
				Gui_DrawFont_GBK24(242,mpos,RED,GRAY0,"中免2");
				break;
			case TEST_FRONT_AVOID_BUMP2 :
				Gui_DrawFont_GBK24(242,mpos,RED,GRAY0,"中免3");
				break;
			case TEST_FRONT_AVOID_BUMP3 :
				Gui_DrawFont_GBK24(242,mpos,RED,GRAY0,"中免4");
				break;
			case TEST_FRONT_AVOID_BUMP4 :
				Gui_DrawFont_GBK24(242,mpos,RED,GRAY0,"中免5");
				break;
			case TEST_FRONT_AVOID_BUMP5 :
				Gui_DrawFont_GBK24(242,mpos,RED,GRAY0,"中免6");
				break;
			case TEST_FRONT_AVOID_BUMP6 :
				Gui_DrawFont_GBK24(242,mpos,RED,GRAY0,"中免7");
				break;
			case TEST_FRONT_AVOID_BUMP7 :
				Gui_DrawFont_GBK24(242,mpos,RED,GRAY0,"中免8");
				break;
			case TEST_FRONT_AVOID_BUMP8 :
				Gui_DrawFont_GBK24(242,mpos,RED,GRAY0,"中免9");
				break;
			case TEST_FRONT_AVOID_BUMP9 :
				Gui_DrawFont_GBK24(242,mpos,RED,GRAY0,"中免10");
				break;
			case TEST_POWER_DC5V :
				Gui_DrawFont_GBK24(242,mpos,RED,GRAY0,"DC5V");
				break;
			case TEST_POWER_VCC3V3 :
				Gui_DrawFont_GBK24(242,mpos,RED,GRAY0,"VCC3V3");
				break;
			case TEST_POWER_VCC1V8 :
				Gui_DrawFont_GBK24(242,mpos,RED,GRAY0,"VCC1V8");
				break;
			case TEST_POWER_VCC1V3 :
				Gui_DrawFont_GBK24(242,mpos,RED,GRAY0,"VCC1V3");
				break;
			case TEST_POWER_DC3V3 :
				Gui_DrawFont_GBK24(242,mpos,RED,GRAY0,"DC3V3");
				break;
			case TEST_TOF_VSEL4V6 :
				Gui_DrawFont_GBK24(242,mpos,RED,GRAY0,"VSEL4V6");
				break;
			case TEST_TOF_CSI_AVDD :
				Gui_DrawFont_GBK24(242,mpos,RED,GRAY0,"CSI_AVDD");
				break;
			case TEST_TOF_CSI_DVDD :
				Gui_DrawFont_GBK24(242,mpos,RED,GRAY0,"CSI_DVDD");
				break;
			case TEST_TOF_INIT :
				Gui_DrawFont_GBK24(242,mpos,RED,GRAY0,"TOF初始化");
				break;
// 			case TEST_TOF_CHECK :
// 				Gui_DrawFont_GBK24(242,mpos,RED,GRAY0,"TOF检测");
// 				break;
			default :
				break;
		}
	}
}

/*****************************************************************************
 * @name       :void Display_Test(u8 test_mode,u8 test_cmd,u8 test_status,u16 dat1,u16 dat2,u8 flag)
 * @date       :2020-09-19 
 * @author     :zhengbaiqiu 
 * @function   :display test
 * @parameters :test_cmd 读取到的命令码
 * @retvalue   :None
******************************************************************************/
void Display_Test(u8 test_mode,u8 test_cmd,u8 test_status,u16 dat1,u16 dat2,u8 flag)
{
	switch(test_cmd)
	{
		case TEST_START ://01
			if(test_status == 0)
			{
				//Gui_DrawFont_GBK24(140,130,BLUE,GRAY0,"版本号错误-F01");
			}
			Gui_DrawFont_GBK24(0,50,BLACK,GRAY0,"...");
			break;
		case TEST_GUIDE1 ://02
			if(test_status == 1)
			{
				Gui_DrawFont_GBK24(0,50,BLACK,GRAY0,"左引导供电");
			}
			else
			{
				Gui_DrawFont_GBK24(0,50,RED,GRAY0,"左引导供电");
				Display_Fail(TEST_GUIDE1,mMaininf.mWork.mWorkFileCont);
				mMaininf.mWork.mWorkFileCont ++;
			}
			Gui_DrawFont_GBK24(0,74,BLACK,GRAY0,"...");
			break;
		case TEST_GUIDE2 ://03
			if(test_status == 1)
			{
				Gui_DrawFont_GBK24(0,74,BLACK,GRAY0,"右引导供电");
			}
			else
			{
				Gui_DrawFont_GBK24(0,74,RED,GRAY0,"右引导供电");
				Display_Fail(TEST_GUIDE2,mMaininf.mWork.mWorkFileCont);
				mMaininf.mWork.mWorkFileCont ++;
			}
			Gui_DrawFont_GBK24(0,98,BLACK,GRAY0,"...");
			break;
		case TEST_GUIDE3 ://04
			if(test_status == 1)
			{
				Gui_DrawFont_GBK24(0,98,BLACK,GRAY0,"上引导");
			}
			else
			{
				Gui_DrawFont_GBK24(0,98,RED,GRAY0,"上引导");
				Display_Fail(TEST_GUIDE3,mMaininf.mWork.mWorkFileCont);
				mMaininf.mWork.mWorkFileCont ++;
			}
			Gui_DrawFont_GBK24(0,122,BLACK,GRAY0,"...");
			break;
		case TEST_GUIDE4 ://05
			if(test_status == 1)
			{
				Gui_DrawFont_GBK24(0,122,BLACK,GRAY0,"前引导");
			}
			else
			{
				Gui_DrawFont_GBK24(0,122,RED,GRAY0,"前引导");
				Display_Fail(TEST_GUIDE4,mMaininf.mWork.mWorkFileCont);
				mMaininf.mWork.mWorkFileCont ++;
			}
			Gui_DrawFont_GBK24(0,146,BLACK,GRAY0,"...");
			break;
		case TEST_LEFT_AVOID_BUMP ://06
			if(test_status == 1)
			{
				Gui_DrawFont_GBK24(0,146,BLACK,GRAY0,"左免");
				
				Display_Avoid_Error(1,150,dat1,dat2,0);
			}
			else
			{
				Gui_DrawFont_GBK24(0,146,RED,GRAY0,"左免");
				Display_Avoid_Error(1,150,dat1,dat2,flag);
				Display_Fail(TEST_LEFT_AVOID_BUMP,mMaininf.mWork.mWorkFileCont);
				Display_Avoid_Error(2,24*mMaininf.mWork.mWorkFileCont+54,dat1,dat2,flag);
				mMaininf.mWork.mWorkFileCont ++;
			}
			Gui_DrawFont_GBK24(0,170,BLACK,GRAY0,"...");
			break;
		case TEST_RIGHT_AVOID_BUMP ://07
			if(test_status == 1)
			{
				Gui_DrawFont_GBK24(0,170,BLACK,GRAY0,"右免");
				
				Display_Avoid_Error(1,174,dat1,dat2,0);
			}
			else
			{
				Gui_DrawFont_GBK24(0,170,RED,GRAY0,"右免");
				Display_Avoid_Error(1,174,dat1,dat2,flag);
				Display_Fail(TEST_RIGHT_AVOID_BUMP,mMaininf.mWork.mWorkFileCont);
				Display_Avoid_Error(2,24*mMaininf.mWork.mWorkFileCont+54,dat1,dat2,flag);
				mMaininf.mWork.mWorkFileCont ++;
			}
			Gui_DrawFont_GBK24(0,194,BLACK,GRAY0,"...");
			break;
		case TEST_FRONT_AVOID_BUMP0 ://8
			if(test_status == 1)
			{
				Gui_DrawFont_GBK24(0,194,BLACK,GRAY0,"中免1");
				
				Display_Avoid_Error(1,198,dat1,dat2,0);
			}
			else
			{
				Gui_DrawFont_GBK24(0,194,RED,GRAY0,"中免1");
				Display_Avoid_Error(1,198,dat1,dat2,flag);
				Display_Fail(TEST_FRONT_AVOID_BUMP0,mMaininf.mWork.mWorkFileCont);
				Display_Avoid_Error(2,24*mMaininf.mWork.mWorkFileCont+54,dat1,dat2,flag);
				mMaininf.mWork.mWorkFileCont ++;
			}
			Gui_DrawFont_GBK24(0,218,BLACK,GRAY0,"...");
			break;
		case TEST_FRONT_AVOID_BUMP1 ://9
			if(test_status == 1)
			{
				Gui_DrawFont_GBK24(0,218,BLACK,GRAY0,"中免2");
				
				Display_Avoid_Error(1,222,dat1,dat2,0);
			}
			else
			{
				Gui_DrawFont_GBK24(0,218,RED,GRAY0,"中免2");
				Display_Avoid_Error(1,222,dat1,dat2,flag);
				Display_Fail(TEST_FRONT_AVOID_BUMP1,mMaininf.mWork.mWorkFileCont);
				Display_Avoid_Error(2,24*mMaininf.mWork.mWorkFileCont+54,dat1,dat2,flag);
				mMaininf.mWork.mWorkFileCont ++;
			}
			Gui_DrawFont_GBK24(0,242,BLACK,GRAY0,"...");
			break;
		case TEST_FRONT_AVOID_BUMP2 ://10
			if(test_status == 1)
			{
				Gui_DrawFont_GBK24(0,242,BLACK,GRAY0,"中免3");
				
				Display_Avoid_Error(1,246,dat1,dat2,0);
			}
			else
			{
				Gui_DrawFont_GBK24(0,242,RED,GRAY0,"中免3");
				Display_Avoid_Error(1,246,dat1,dat2,flag);
				Display_Fail(TEST_FRONT_AVOID_BUMP2,mMaininf.mWork.mWorkFileCont);
				Display_Avoid_Error(2,24*mMaininf.mWork.mWorkFileCont+54,dat1,dat2,flag);
				mMaininf.mWork.mWorkFileCont ++;
			}
			Gui_DrawFont_GBK24(0,266,BLACK,GRAY0,"...");
			break;
		case TEST_FRONT_AVOID_BUMP3 ://11
			if(test_status == 1)
			{
				Gui_DrawFont_GBK24(0,266,BLACK,GRAY0,"中免4");
				
				Display_Avoid_Error(1,270,dat1,dat2,0);
			}
			else
			{
				Gui_DrawFont_GBK24(0,266,RED,GRAY0,"中免4");
				Display_Avoid_Error(1,270,dat1,dat2,flag);
				Display_Fail(TEST_FRONT_AVOID_BUMP3,mMaininf.mWork.mWorkFileCont);
				Display_Avoid_Error(2,24*mMaininf.mWork.mWorkFileCont+54,dat1,dat2,flag);
				mMaininf.mWork.mWorkFileCont ++;
			}
			LCD_Fill(0,41,235,295,GRAY0);
			Gui_DrawFont_GBK24(0,50,BLACK,GRAY0,"...");
			break;
		case TEST_FRONT_AVOID_BUMP4 ://12
			if(test_status == 1)
			{
				Gui_DrawFont_GBK24(0,50,BLACK,GRAY0,"中免5");
				
				Display_Avoid_Error(1,54,dat1,dat2,0);
			}
			else
			{
				Gui_DrawFont_GBK24(0,50,RED,GRAY0,"中免5");
				Display_Avoid_Error(1,54,dat1,dat2,flag);
				Display_Fail(TEST_FRONT_AVOID_BUMP4,mMaininf.mWork.mWorkFileCont);
				Display_Avoid_Error(2,24*mMaininf.mWork.mWorkFileCont+54,dat1,dat2,flag);
				mMaininf.mWork.mWorkFileCont ++;
			}
			Gui_DrawFont_GBK24(0,74,BLACK,GRAY0,"...");
			break;
		case TEST_FRONT_AVOID_BUMP5 ://13
			if(test_status == 1)
			{
				Gui_DrawFont_GBK24(0,74,BLACK,GRAY0,"中免6");
				
				Display_Avoid_Error(1,78,dat1,dat2,0);
			}
			else
			{
				Gui_DrawFont_GBK24(0,74,RED,GRAY0,"中免6");
				Display_Avoid_Error(1,78,dat1,dat2,flag);
				Display_Fail(TEST_FRONT_AVOID_BUMP5,mMaininf.mWork.mWorkFileCont);
				Display_Avoid_Error(2,24*mMaininf.mWork.mWorkFileCont+54,dat1,dat2,flag);
				mMaininf.mWork.mWorkFileCont ++;
			}
			Gui_DrawFont_GBK24(0,98,BLACK,GRAY0,"...");
			break;
		case TEST_FRONT_AVOID_BUMP6 ://14
			if(test_status == 1)
			{
				Gui_DrawFont_GBK24(0,98,BLACK,GRAY0,"中免7");
				
				Display_Avoid_Error(1,102,dat1,dat2,0);
			}
			else
			{
				Gui_DrawFont_GBK24(0,98,RED,GRAY0,"中免7");
				Display_Avoid_Error(1,102,dat1,dat2,flag);
				Display_Fail(TEST_FRONT_AVOID_BUMP6,mMaininf.mWork.mWorkFileCont);
				Display_Avoid_Error(2,24*mMaininf.mWork.mWorkFileCont+54,dat1,dat2,flag);
				mMaininf.mWork.mWorkFileCont ++;
			}
			Gui_DrawFont_GBK24(0,122,BLACK,GRAY0,"...");
			break;
		case TEST_FRONT_AVOID_BUMP7 ://15
			if(test_status == 1)
			{
				Gui_DrawFont_GBK24(0,122,BLACK,GRAY0,"中免8");
				
				Display_Avoid_Error(1,126,dat1,dat2,0);
			}
			else
			{
				Gui_DrawFont_GBK24(0,122,RED,GRAY0,"中免8");
				Display_Avoid_Error(1,126,dat1,dat2,flag);
				Display_Fail(TEST_FRONT_AVOID_BUMP7,mMaininf.mWork.mWorkFileCont);
				Display_Avoid_Error(2,24*mMaininf.mWork.mWorkFileCont+54,dat1,dat2,flag);
				mMaininf.mWork.mWorkFileCont ++;
			}
			Gui_DrawFont_GBK24(0,146,BLACK,GRAY0,"...");
			break;
		case TEST_FRONT_AVOID_BUMP8 ://16
			if(test_status == 1)
			{
				Gui_DrawFont_GBK24(0,146,BLACK,GRAY0,"中免9");
				
				Display_Avoid_Error(1,150,dat1,dat2,0);
			}
			else
			{
				Gui_DrawFont_GBK24(0,146,RED,GRAY0,"中免9");
				Display_Avoid_Error(1,150,dat1,dat2,flag);
				Display_Fail(TEST_FRONT_AVOID_BUMP8,mMaininf.mWork.mWorkFileCont);
				Display_Avoid_Error(2,24*mMaininf.mWork.mWorkFileCont+54,dat1,dat2,flag);
				mMaininf.mWork.mWorkFileCont ++;
			}
			Gui_DrawFont_GBK24(0,170,BLACK,GRAY0,"...");
			break;
		case TEST_FRONT_AVOID_BUMP9 ://17
			if(test_status == 1)
			{
				Gui_DrawFont_GBK24(0,170,BLACK,GRAY0,"中免10");
				
				Display_Avoid_Error(1,174,dat1,dat2,0);
			}
			else
			{
				Gui_DrawFont_GBK24(0,170,RED,GRAY0,"中免10");
				Display_Avoid_Error(1,174,dat1,dat2,flag);
				Display_Fail(TEST_FRONT_AVOID_BUMP9,mMaininf.mWork.mWorkFileCont);
				Display_Avoid_Error(2,24*mMaininf.mWork.mWorkFileCont+54,dat1,dat2,flag);
				mMaininf.mWork.mWorkFileCont ++;
			}
			if(mMaininf.mWork.mWorkPageFlag == 2)//i9
			{
				Gui_DrawFont_GBK24(0,194,BLACK,GRAY0,"...");
			}
			break;
		case TEST_POWER_DC5V ://18
			if(test_status == 1)
			{
				Gui_DrawFont_GBK24(0,194,BLACK,GRAY0,"DC5V");
			}
			else
			{
				Gui_DrawFont_GBK24(0,194,RED,GRAY0,"DC5V");
				Display_Fail(TEST_POWER_DC5V,mMaininf.mWork.mWorkFileCont);
				mMaininf.mWork.mWorkFileCont ++;
			}
			Gui_DrawFont_GBK24(0,218,BLACK,GRAY0,"...");
			break;
		case TEST_POWER_VCC3V3 ://19
			if(test_status == 1)
			{
				Gui_DrawFont_GBK24(0,218,BLACK,GRAY0,"VCC3V3");
			}
			else
			{
				Gui_DrawFont_GBK24(0,218,RED,GRAY0,"VCC3V3");
				Display_Fail(TEST_POWER_VCC3V3,mMaininf.mWork.mWorkFileCont);
				mMaininf.mWork.mWorkFileCont ++;
			}
			Gui_DrawFont_GBK24(0,242,BLACK,GRAY0,"...");
			break;
		case TEST_POWER_VCC1V8 ://20
			if(test_status == 1)
			{
				Gui_DrawFont_GBK24(0,242,BLACK,GRAY0,"VCC1V8");
			}
			else
			{
				Gui_DrawFont_GBK24(0,242,RED,GRAY0,"VCC1V8");
				Display_Fail(TEST_POWER_VCC1V8,mMaininf.mWork.mWorkFileCont);
				mMaininf.mWork.mWorkFileCont ++;
			}
			Gui_DrawFont_GBK24(0,266,BLACK,GRAY0,"...");
			break;
		case TEST_POWER_VCC1V3 ://21
			if(test_status == 1)
			{
				Gui_DrawFont_GBK24(0,266,BLACK,GRAY0,"VCC1V3");
			}
			else
			{
				Gui_DrawFont_GBK24(0,266,RED,GRAY0,"VCC1V3");
				Display_Fail(TEST_POWER_VCC1V3,mMaininf.mWork.mWorkFileCont);
				mMaininf.mWork.mWorkFileCont ++;
			}
			LCD_Fill(0,41,235,295,GRAY0);
			Gui_DrawFont_GBK24(0,50,BLACK,GRAY0,"...");
			break;
		case TEST_POWER_DC3V3 ://22
			if(test_status == 1)
			{
				Gui_DrawFont_GBK24(0,50,BLACK,GRAY0,"DC3V3");
			}
			else
			{
				Gui_DrawFont_GBK24(0,50,RED,GRAY0,"DC3V3");
				Display_Fail(TEST_POWER_DC3V3,mMaininf.mWork.mWorkFileCont);
				mMaininf.mWork.mWorkFileCont ++;
			}
// 			if(mMaininf.mWork.mWorkPageFlag == 2)//i9
// 			{
// 				Gui_DrawFont_GBK24(0,74,BLACK,GRAY0,"...");
// 			}
			Gui_DrawFont_GBK24(0,74,BLACK,GRAY0,"...");
			break;
		case TEST_TOF_VSEL4V6 ://23
			if(test_status == 1)
			{
				Gui_DrawFont_GBK24(0,74,BLACK,GRAY0,"VSEL4V6");
			}
			else
			{
				Gui_DrawFont_GBK24(0,74,RED,GRAY0,"VSEL4V6");
				Display_Fail(TEST_TOF_VSEL4V6,mMaininf.mWork.mWorkFileCont);
				mMaininf.mWork.mWorkFileCont ++;
			}
			Gui_DrawFont_GBK24(0,98,BLACK,GRAY0,"...");
			break;
		case TEST_TOF_CSI_AVDD ://24
			if(test_status == 1)
			{
				Gui_DrawFont_GBK24(0,98,BLACK,GRAY0,"CSI_AVDD");
			}
			else
			{
				Gui_DrawFont_GBK24(0,98,RED,GRAY0,"CSI_AVDD");
				Display_Fail(TEST_TOF_CSI_AVDD,mMaininf.mWork.mWorkFileCont);
				mMaininf.mWork.mWorkFileCont ++;
			}
			Gui_DrawFont_GBK24(0,122,BLACK,GRAY0,"...");
			break;
		case TEST_TOF_CSI_DVDD ://25
			if(test_status == 1)
			{
				Gui_DrawFont_GBK24(0,122,BLACK,GRAY0,"CSI_DVDD");
			}
			else
			{
				Gui_DrawFont_GBK24(0,122,RED,GRAY0,"CSI_DVDD");
				Display_Fail(TEST_TOF_CSI_DVDD,mMaininf.mWork.mWorkFileCont);
				mMaininf.mWork.mWorkFileCont ++;
			}
			Gui_DrawFont_GBK24(0,146,BLACK,GRAY0,"...");
			break;
		case TEST_TOF_INIT ://26
			if(test_status == 1)
			{
				Gui_DrawFont_GBK24(0,146,BLACK,GRAY0,"TOF初始化");
			}
			else
			{
				Gui_DrawFont_GBK24(0,146,RED,GRAY0,"TOF初始化");
				Display_Fail(TEST_TOF_INIT,mMaininf.mWork.mWorkFileCont);
				mMaininf.mWork.mWorkFileCont ++;
			}
			//Gui_DrawFont_GBK24(0,170,BLACK,GRAY0,"...");
			break;
// 		case TEST_TOF_CHECK ://29
// 			if(test_status == 1)
// 			{
// 				Gui_DrawFont_GBK24(0,218,BLACK,GRAY0,"TOF检测");
// 			}
// 			else
// 			{
// 				Gui_DrawFont_GBK24(0,218,RED,GRAY0,"TOF检测");
// 				Display_Fail(TEST_TOF_CHECK,mMaininf.mWork.mWorkFileCont);
// 				mMaininf.mWork.mWorkFileCont ++;
// 			}
// 			break;
		case 0xfe ://ALL
			if(test_status == 1)
			{
				LCD_Clear_Area(0,20,479,299,GRAY0);
				Gui_DrawFont_GBK128(116,78,GREEN,GRAY0,mpass);
			}
			else
			{
				
			}
			//mMaininf.mWork.mWorkFileCont = 0;
			break;
		default :
			break;
	}
//	Gui_DrawPoint(0,0,GRAY0);
}

/*****************************************************************************
 * @name       :void Test_Display(void)
 * @date       :2020-09-19 
 * @author     :zhengbaiqiu 
 * @function   :display test
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void Test_Display(void)
{
//	if(mMaininf.mWork.mWorkInitFlag == 1)//初始化成功
	if(1)
	{
		if(mMaininf.mWork.mWorkPageFlag == 0)
		{
			if(mMaininf.mMode.mScreenPressFlag == 1)            //    屏幕触发
			{
				mMaininf.mMode.mScreenPressFlag = 0;
				
				if((Touch_X >= 96) && (Touch_X <= 216) && (Touch_Y >= 136) && (Touch_Y <= 184))
				{
					api_SendCMDData(2,1,1);//i8
				}
				if((Touch_X >= 264) && (Touch_X <= 384) && (Touch_Y >= 136) && (Touch_Y <= 184))
				{
					api_SendCMDData(2,2,1);//i9
				}
			}
			if(mMaininf.mUart1.mReceiveFlag == 1)
			{
				mMaininf.mUart1.mReceiveFlag = 0;
				
				if((mMaininf.mUart1.ReceiveBuf[0] == 2) && (mMaininf.mUart1.ReceiveBuf[1] == 1) && 
					(mMaininf.mUart1.ReceiveBuf[2] == 1))//接收到反馈（i8主板测试）
				{
					mMaininf.mUart1.mReceiveFlag = 0;
					mMaininf.mWork.mWorkPageFlag = 1;
					DrawTestPage("I8免碰撞板工装");
					LCD_Fill(0,20,lcddev.width,40,YELLOW);
					Gui_StrCenters(0,240,22,BLACK,YELLOW,"检测栏",16,1);//居中显示
					Gui_StrCenters(240,480,22,BLACK,YELLOW,"报错栏",16,1);//居中显示
					Gui_DrawLine(240,41,240,299,BLACK);
					api_SendCMDData(2,4,1);//
				}
				if((mMaininf.mUart1.ReceiveBuf[0] == 2) && (mMaininf.mUart1.ReceiveBuf[1] == 2) && 
					(mMaininf.mUart1.ReceiveBuf[2] == 1))//接收到反馈（i9主板测试）
				{
					mMaininf.mUart1.mReceiveFlag = 0;
					mMaininf.mWork.mWorkPageFlag = 2;
					DrawTestPage("I9免碰撞板工装");
					LCD_Fill(0,20,lcddev.width,40,YELLOW);
					Gui_StrCenters(0,240,22,BLACK,YELLOW,"检测栏",16,1);//居中显示
					Gui_StrCenters(240,480,22,BLACK,YELLOW,"报错栏",16,1);//居中显示
					Gui_DrawLine(240,41,240,299,BLACK);
					api_SendCMDData(2,5,1);//
					//mMaininf.mWork.mWorkChecking = 1;
				}
			}
		}
		else if(mMaininf.mWork.mWorkPageFlag == 1)//i8
		{
			if(mMaininf.mUart1.mReceiveFlag == 1)
			{
				mMaininf.mUart1.mReceiveFlag = 0;
				
				if((mMaininf.mUart1.ReceiveBuf[0] == 2) && (mMaininf.mUart1.ReceiveBuf[1] == 3) && 
					(mMaininf.mUart1.ReceiveBuf[2] == 1))//检测按键按下
				{
					mMaininf.mWork.mWorkFileCont = 0;
					
// 					LCD_Clear_Area(0,44,235,295,GRAY0);
// 					LCD_Clear_Area(242,44,475,295,GRAY0);
					LCD_Fill(0,20,lcddev.width,40,YELLOW);
					LCD_Clear_Area(0,40,479,299,GRAY0);
					Gui_StrCenters(0,240,22,BLACK,YELLOW,"检测栏",16,1);//居中显示
					Gui_StrCenters(240,480,22,BLACK,YELLOW,"报错栏",16,1);//居中显示
					Gui_DrawLine(240,41,240,299,BLACK);
					
					Gui_DrawFont_GBK24(0,50,BLACK,GRAY0,"...");
					api_SendCMDData(2,3,1);//返回信息
					//vTaskDelay(5);
				}
				if(mMaininf.mUart1.ReceiveBuf[0] == 1)//接收到指令
				{
					mMaininf.mWork.mWorkAvoidBumpDat1 = ((mMaininf.mUart1.ReceiveBuf[3] << 8) | mMaininf.mUart1.ReceiveBuf[4]);
					mMaininf.mWork.mWorkAvoidBumpDat2 = ((mMaininf.mUart1.ReceiveBuf[5] << 8) | mMaininf.mUart1.ReceiveBuf[6]);
					mMaininf.mWork.mWorkAvoidBumpErrorFlag = mMaininf.mUart1.ReceiveBuf[7];
					
					Display_Test(1,mMaininf.mUart1.ReceiveBuf[1],mMaininf.mUart1.ReceiveBuf[2],mMaininf.mWork.mWorkAvoidBumpDat1,mMaininf.mWork.mWorkAvoidBumpDat2,mMaininf.mWork.mWorkAvoidBumpErrorFlag);
					
					api_SendCMDData(1,mMaininf.mUart1.ReceiveBuf[1],1);//返回信息
					//vTaskDelay(5);
					//mMaininf.mWork.mWorkChecking = 0;
				}
			}
		}
		else if(mMaininf.mWork.mWorkPageFlag == 2)//i9
		{
			if(mMaininf.mUart1.mReceiveFlag == 1)
			{
				mMaininf.mUart1.mReceiveFlag = 0;
				
				if((mMaininf.mUart1.ReceiveBuf[0] == 2) && (mMaininf.mUart1.ReceiveBuf[1] == 3) && 
					(mMaininf.mUart1.ReceiveBuf[2] == 1))//检测按键按下
				{
					mMaininf.mWork.mWorkFileCont = 0;
					
					LCD_Clear_Area(0,44,235,295,GRAY0);
					LCD_Clear_Area(242,44,475,295,GRAY0);
					
					Gui_DrawFont_GBK24(0,50,BLACK,GRAY0,"...");
					api_SendCMDData(2,3,1);//返回信息
					//vTaskDelay(5);
				}
				if(mMaininf.mUart1.ReceiveBuf[0] == 1)//接收到指令
				{
					mMaininf.mWork.mWorkAvoidBumpDat1 = ((mMaininf.mUart1.ReceiveBuf[3] << 8) | mMaininf.mUart1.ReceiveBuf[4]);
					mMaininf.mWork.mWorkAvoidBumpDat2 = ((mMaininf.mUart1.ReceiveBuf[5] << 8) | mMaininf.mUart1.ReceiveBuf[6]);
					mMaininf.mWork.mWorkAvoidBumpErrorFlag = mMaininf.mUart1.ReceiveBuf[7];
					
					Display_Test(1,mMaininf.mUart1.ReceiveBuf[1],mMaininf.mUart1.ReceiveBuf[2],mMaininf.mWork.mWorkAvoidBumpDat1,mMaininf.mWork.mWorkAvoidBumpDat2,mMaininf.mWork.mWorkAvoidBumpErrorFlag);
					
					api_SendCMDData(1,mMaininf.mUart1.ReceiveBuf[1],1);//返回信息
				}
			}
		}
		
		
// // 		if(mMaininf.mWork.mWorkChecking == 0)//未在检测状态
// // 		{
// // 			if(mMaininf.mMode.mScreenPressFlag == 1)            //    屏幕触发
// // 			{
// // 				mMaininf.mMode.mScreenPressFlag = 0;
// // 				
// // 				if((Touch_X >= 305) && (Touch_X <= 423) && (Touch_Y >= 215) && (Touch_Y <= 278))
// // 				{
// // 					LCD_Clear_Area(305,215,423,278,GRAY0);
// // 					
// // 					Gui_DrawFont_GBK24(140,130,BLUE,GRAY0,"检测中..              ");
// // 					
// // 					api_SendCMDData(2,1,1);//开始检测
// // 					mMaininf.mWork.mWorkChecking = 1;
// // 				}
// // 			}
// // 		}
// //		else
// 		if(1)
// 		{
// 			if(mMaininf.mUart1.mReceiveFlag == 1)            //    接收到反馈
// 			{
// 				mMaininf.mUart1.mReceiveFlag = 0;
// 				
// 				if((mMaininf.mUart1.ReceiveBuf[0] == 2) && (mMaininf.mUart1.ReceiveBuf[1] == 1) && 
// 				   (mMaininf.mUart1.ReceiveBuf[2] == 1))//接收到反馈
// 				{
// 					LCD_Clear_Area(305,215,423,278,GRAY0);
// 					
// 					Gui_DrawFont_GBK24(140,130,BLUE,GRAY0,"检测中...             ");
// 					api_SendCMDData(2,1,1);//开始检测
// 					vTaskDelay(5);
// // 					mMaininf.mWork.mWorkChecking = 1;
// 				}
// 				if(mMaininf.mUart1.ReceiveBuf[0] == 1)//接收到指令
// 				{
// // 					if((mMaininf.mUart1.ReceiveBuf[1] == 1) && (mMaininf.mUart1.ReceiveBuf[2] == 1))
// // 					{
// // 						api_SendCMDData(1,1,1);//返回信息
// // 					}
// // 					else
// // 					{
// // 						api_SendCMDData(1,mMaininf.mUart1.ReceiveBuf[1],1);//返回信息
// // 						vTaskDelay(5);
// 					
// 						Display_Test(1,mMaininf.mUart1.ReceiveBuf[1],mMaininf.mUart1.ReceiveBuf[2]);
// 						
// // 						DisplayButtonUp(305,215,423,278); //x1,y1,x2,y2
// // 						Gui_DrawFont_GBK24(316,235,BLACK,GRAY0,"开始检测");
// 						
// 						api_SendCMDData(1,mMaininf.mUart1.ReceiveBuf[1],1);//返回信息
// 						vTaskDelay(5);
// 						mMaininf.mWork.mWorkChecking = 0;
// // 					}
// 				}
// 			}
// 			
// 			//Display_Test(mMaininf.mWork.mWorkMODE,mMaininf.mWork.mWorkCMD,mMaininf.mWork.mWorkSTATUS);
// 		}
	}

	Gui_DrawPoint(0,0,GRAY0);
}






















