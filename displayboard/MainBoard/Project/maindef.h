/****************************************************************************************************/
/*			@FileName		:	MAINDEF.h																																	  	*/
/*			@Author			:	zhengbaiqiu																																		*/
/*			@Version		:	v1.0																																					*/
/*			@Date				:	2019.11.12  																																	*/
/****************************************************************************************************/

#ifndef	__MAINDEF_H__
#define	__MAINDEF_H__

#include "stm32f10x.h"
#include <stdlib.h>

/* Scheduler include files. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"


/*		struct congig		*/



#ifdef __cplusplus
extern "C" {
#endif
	
	
#define m_Ok 1                         //真
#define m_NoOk 0                       //假
	
#define BURN_TIMEOUT 300               //烧录步骤500ms超时	
	
#define mePRINT_QUEUE_SIZE	(5)
#define meBURN_QUEUE_SIZE	(5)
#define mDELAY100msTick		( pdMS_TO_TICKS( ( TickType_t ) 100 ) )	
#define mDELAY200msTick		( pdMS_TO_TICKS( ( TickType_t ) 200 ) )	
#define mDELAY500msTick		( pdMS_TO_TICKS( ( TickType_t ) 500 ) )
	
	
/* Task priorities. */
//#define mainQUEUE_POLL_PRIORITY				( tskIDLE_PRIORITY + 2 )
//#define mainCHECK_TASK_PRIORITY				( tskIDLE_PRIORITY + 3 )
//#define mainSEM_TEST_PRIORITY				( tskIDLE_PRIORITY + 1 )
//#define mainBLOCK_Q_PRIORITY				( tskIDLE_PRIORITY + 2 )
//#define mainCREATOR_TASK_PRIORITY           ( tskIDLE_PRIORITY + 3 )
//#define mainFLASH_TASK_PRIORITY				( tskIDLE_PRIORITY + 1 )
//#define mainCOM_TEST_PRIORITY				( tskIDLE_PRIORITY + 1 )
//#define mainINTEGER_TASK_PRIORITY           ( tskIDLE_PRIORITY )
#define mainTIMER_TASK_PRIORITY				( tskIDLE_PRIORITY + 3 )
#define mainDISPLAY_TASK_PRIORITY				( tskIDLE_PRIORITY + 1 )
// #define mainCHECKKEY_TASK_PRIORITY				( tskIDLE_PRIORITY + 3 )
// #define mainCHECKSD_TASK_PRIORITY				( tskIDLE_PRIORITY + 3 )
// #define mainPRINT_TASK_PRIORITY          (tskIDLE_PRIORITY + 2 )
// #define mainBURN_TASK_PRIORITY          (tskIDLE_PRIORITY + 2 )
// #define mainVERSIONCHECK_TASK_PRIORITY				( tskIDLE_PRIORITY + 2 )


/* Task Stack. */
#define mainTIMER_TASK_STACK_SIZE			( configMINIMAL_STACK_SIZE + 64 )
#define mainDISPLAY_TASK_STACK_SIZE			( configMINIMAL_STACK_SIZE + 64 )
// #define mainCHECKKEY_TASK_STACK_SIZE			( configMINIMAL_STACK_SIZE + 64 )
// #define mainCHECKSD_TASK_STACK_SIZE			( configMINIMAL_STACK_SIZE + 64 )
// #define mainVERSIONCHECK_TASK_STACK_SIZE			( configMINIMAL_STACK_SIZE + 64 )
// #define mainPRINT_TASK_STACK_SIZE			( configMINIMAL_STACK_SIZE + 64 )
// #define mainBURN_TASK_STACK_SIZE			( configMINIMAL_STACK_SIZE + 128 )



/*--------------------------------------------------------------------------

        自定义结构体

---------------------------------------------------------------------------*/	


/*   	 自定义			*/
#define SYSTEMTIMEMAX                86400000               /*   System timing maximum 24 hours  */

/* timeout time define */

#define TIMEOUT_0ms           (0)               /*   0ms超时      */
#define TIMEOUT_5ms           (5)               /*   5ms超时      */
#define TIMEOUT_10ms          (10)              /*   10ms超时     */
#define TIMEOUT_50ms          (50)              /*   50ms超时     */
#define TIMEOUT_100ms         (100)             /*   100ms超时    */
#define TIMEOUT_300ms         (300)             /*   300ms超时    */
#define TIMEOUT_500ms         (500)             /*   500ms超时    */
#define TIMEOUT_1000ms        (1000)            /*   1000ms超时   */
#define TIMEOUT_3500ms        (3500)            /*   3500ms超时   */
#define TIMEOUT_5000ms        (5000)            /*   5000ms超时   */

/*   	 结构体声明			*/

typedef enum {
	ST_0x55 = 0x01,
	ST_0xAA,
	ST_MODE,
	ST_CMD,
	ST_STA,
	ST_DAT1,
	ST_DAT2,
	ST_DAT3,
	ST_DAT4,
	ST_DAT5,
	ST_BCC
} UART1_RECEIVE_STEP;


typedef enum
{
	TEST_START = 0x01,                //01开始
	TEST_GUIDE1,                      //02
	TEST_GUIDE2,                      //03
	TEST_GUIDE3,                      //04
	TEST_GUIDE4,                      //05
	TEST_LEFT_AVOID_BUMP,             //06
	TEST_RIGHT_AVOID_BUMP,            //07
	TEST_FRONT_AVOID_BUMP0,           //08
	TEST_FRONT_AVOID_BUMP1,           //09
	TEST_FRONT_AVOID_BUMP2,           //10
	TEST_FRONT_AVOID_BUMP3,           //11
	TEST_FRONT_AVOID_BUMP4,           //12
	TEST_FRONT_AVOID_BUMP5,           //13
	TEST_FRONT_AVOID_BUMP6,           //14
	TEST_FRONT_AVOID_BUMP7,           //15
	TEST_FRONT_AVOID_BUMP8,           //16
	TEST_FRONT_AVOID_BUMP9,           //17
	TEST_POWER_DC5V,                  //18
	TEST_POWER_VCC3V3,                //19
	TEST_POWER_VCC1V8,                //20
	TEST_POWER_VCC1V3,                //21
	TEST_POWER_DC3V3,                 //22
	TEST_TOF_VSEL4V6,                 //23
	TEST_TOF_CSI_AVDD,                //24
	TEST_TOF_CSI_DVDD,                //25
	TEST_TOF_INIT,                    //26
	
}enum_TEST_CMD;

typedef struct _ST_SYSTEM_MANAGER{
	u32 mSystemTick ;                /*  1ms 模拟滴答计数器   */
	u32 mPowerOnStartTime ;
	
	u32 mReadUartStatusStartTime ;
}_ST_SYSTEM_MANAGER;

typedef struct _ST_USART_MANAGER{
	u8 TxBuffer[64];
	u8 RxBuffer[64];
	u8 SendBuf[64];
	u8 ReceiveBuf[64];
	
	u16 mSendSize;
	u16 mSendCont;
	
	u8 mReceiveStep : 4;
	u8 mReceiveBCC;
	u16 mReceiveSize;
	u16 mReceiveCont;
	u16 mReceiveTimeOut;
	
	u8 mReceiveFlag : 1;
	
}_ST_USART_MANAGER;

typedef struct _ST_MODE_MANAGER{
	u8 mKeyPressFlag : 1;            //  0 : no press    1 : press  按键触发
	u8 mScreenPressFlag : 1;         //  0 : no press    1 : press  屏幕按下
	
	u8 mModePage;
}_ST_MODE_MANAGER;

typedef struct _ST_WORK_MANAGER{
	u8 mWorkMODE ;           //工作模式
 	u8 mWorkCMD ;            //工作命令字
 	u8 mWorkSTATUS ;         //传感器状态
	
	u8 mWorkInitFlag : 1;    //0:初始化失败   1：初始化成功
	u8 mWorkChecking : 1;    //0:没有检测     1：正在检测中
	u8 mWorkKeyCont ;
	
	u8 mWorkPageFlag;
	
	u8 mWorkFileCont;
	
	u16 mWorkAvoidBumpDat1;
	u16 mWorkAvoidBumpDat2;
	u8 mWorkAvoidBumpErrorFlag;
}_ST_WORK_MANAGER;


typedef struct _ST_MAINMANAGER{
	/*        文件内定义        */
	_ST_SYSTEM_MANAGER         mSystem;
	_ST_USART_MANAGER          mUart1;
	
	_ST_MODE_MANAGER           mMode;
	_ST_WORK_MANAGER           mWork;

}_ST_MAINMANAGER;
		
extern _ST_MAINMANAGER mMaininf;
	
void prvSetupHardware(void);
void vCreateQueueAndSemphr(void);



	
	
#ifdef _cplusplus
extern }
#endif

#endif /*__MAINDEF_H__*/	

