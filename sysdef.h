/*
sysdef.h
设置与系统相关重要常量
*/
#ifndef _SYSDEF_H_
#define _SYSDEF_H_
//1=330，2＝U1,3=350+，4=320c,5=v1,6=8channel,7=jiwu,8=350c,9=360B,10=u2,11=350b,12=v2
//13=TFT,20=300C,14=F1,15=390(class)            //360b+ & F1 需要将C_FILL 1	//实心波
#define C_DEVTYPE 1
#define C_UDISK 1	//0,不带U盘记录：1，U盘记录
/*
重要的三个调节量
C_DEVTYPE：仪器型号
C_UDISK：U盘记录
C_FILL：实心波	在globe.h中修改
C_DEVLIB	23	//23tft .24 EL
*/

#define C_PROCESS 1	//硬件板子工艺
#define C_ACQU 0	//定时开采样


#if C_DEVTYPE == 20
#define C_LIGHT 0	//屏幕亮度可调
#else
#define C_LIGHT 1	//屏幕亮度可调
#endif

#if C_DEVTYPE == 1
#define C_COLOR	1
#define C_ROTARY 0
#elif C_DEVTYPE == 2
#define C_COLOR	1
#define C_ROTARY 1
#elif C_DEVTYPE == 3
#define C_COLOR	0
#define C_ROTARY 0
#elif C_DEVTYPE == 4
#define C_COLOR	1
#define C_ROTARY 0
#elif C_DEVTYPE == 20
#define C_COLOR	1
#define C_ROTARY 0
#elif C_DEVTYPE == 5 || C_DEVTYPE == 12
#define C_COLOR	1
#define C_ROTARY 1
#elif C_DEVTYPE == 6
#define C_COLOR	1
#define C_ROTARY 0
#elif C_DEVTYPE == 7
#define C_COLOR	0
#define C_ROTARY 0
#elif C_DEVTYPE == 8
#define C_COLOR	1
#define C_ROTARY 0
#elif C_DEVTYPE == 9
#define C_COLOR	0
#define C_ROTARY 0
#elif C_DEVTYPE == 10
#define C_COLOR	1
#define C_ROTARY 1
#elif C_DEVTYPE == 11
#define C_COLOR	0
#define C_ROTARY 0
#elif C_DEVTYPE == 13
#define C_COLOR	1
#define C_ROTARY 0
#elif C_DEVTYPE == 14
#define C_COLOR	1
#define C_ROTARY 0
#elif C_DEVTYPE == 15
#define C_COLOR	1
#define C_ROTARY 0
#endif
//垂直线性补偿
#if C_DEVLIB == 24
#ifdef C_COLOR
#undef C_COLOR
#endif
#define C_COLOR 0
#endif
#define C_AMPCOMP 0

#ifndef true
#define true 1
#endif

#ifndef false
#define false 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef BOOL
#define BOOL int
#endif

#ifndef bool
#define bool unsigned int
#endif
#ifndef u_int
#define u_int unsigned int
#endif
#ifndef UINT
#define UINT unsigned int
#endif

#ifndef u_short
#define u_short unsigned short
#endif

#ifndef u_char
#define u_char unsigned char
#endif

#ifndef u_long
#define u_long unsigned int
#endif

//#ifndef INT
#define INT int
//#endif


#define MB_OK		1
#define MB_CANCEL	2
#define MB_OKCANCEL (MB_OK+MB_CANCEL)
#define MB_YES		4
#define MB_NO		8
#define MB_YESNO	(MB_YES+MB_NO)

#define C_MAX_BIT		9
//=pow(2,C_MAX_BIT)
#define C_MAX_DOT_NUMBER	512


#define CH(a)		(a) //( ( (a)/100 )*256 + ( (a)%100 ) )
#define ch(a)		(a) //( ( (a)/100 )*256 + ( (a)%100 ) )
#define cH(a)		(a) //( ( (a)/100 )*256 + ( (a)%100 ) )
#define Ch(a)		(a) //( ( (a)/100 )*256 + ( (a)%100 ) )

#define msin(a) sin( (a)*C_PI/180.0 )
#define mcos(a) cos( (a)*C_PI/180.0 )
#define mtan(a) tan( (a)*C_PI/180.0 )
#define macos(a) ( acos(a)*1800/C_PI )
#define masin(a) ( asin(a)*1800/C_PI )
#define matan(a) ( atan(a)*1800/C_PI )


#define C_SIZE_INT			( 4 )
#define C_SIZE_SHORT		( 2 )
#define C_SIZE_CHAR			( 1 )
#define C_TRUE	1
#define C_FALSE 0

//波形参数刷新时间
#define C_RENOVATE_TIME 30

#define C_DEBUG		1

#if C_DEVTYPE == 14
#define C_CHMAX				60					/*通道数*/
#else
#define C_CHMAX				60				/*通道数*/
#endif

#if C_DEVTYPE == 3 || C_DEVTYPE == 5
#define C_MAX_SYSGAIN		1100
#define C_MAX_BASEGAIN		1100				/*显示的最大增益值*/
#define C_MAX_ADJUSTGAIN	800				/*仪器自动调节的最大增益*/
#define C_MAX_RANGE	  		60000				/*声程最大范围5000mm*/
#define C_MIN_RANGE			100
#define C_MAX_DELAY			(200 * 200 * SAMPLE_FREQ )		/*延时最大范围500ms*/
#elif C_DEVTYPE == 8
#define C_MAX_SYSGAIN		1200
#define C_MAX_BASEGAIN		1200				/*显示的最大增益值*/
#define C_MAX_ADJUSTGAIN	800				/*仪器自动调节的最大增益*/
#define C_MAX_RANGE	  		60000				/*声程最大范围5000mm*/
#define C_MIN_RANGE			100
#define C_MAX_DELAY			(200 * 200 * SAMPLE_FREQ )		/*延时最大范围500ms*/

#elif C_DEVTYPE == 1
#define C_MAX_SYSGAIN		1100
#define C_MAX_BASEGAIN		1100				/*显示的最大增益值*/
#define C_MAX_ADJUSTGAIN	800				/*仪器自动调节的最大增益*/
#define C_MAX_RANGE	  		100000				/*声程最大范围5000mm*/
#define C_MIN_RANGE			100
#define C_MAX_DELAY			(200 * 200 * SAMPLE_FREQ )		/*延时最大范围500ms*/

#elif C_DEVTYPE == 2 || C_DEVTYPE == 10 || C_DEVTYPE == 11 || C_DEVTYPE == 9 || C_DEVTYPE == 13 || C_DEVTYPE == 14 ||C_DEVTYPE == 15
#define C_MAX_SYSGAIN		1200
#define C_MAX_BASEGAIN		1200				/*显示的最大增益值*/
#define C_MAX_ADJUSTGAIN	800				/*仪器自动调节的最大增益*/
#define C_MAX_RANGE	  		100000				/*声程最大范围5000mm*/
#define C_MIN_RANGE			100
#define C_MAX_DELAY			(200 * 200 * SAMPLE_FREQ )		/*延时最大范围500ms*/
#else
#define C_MAX_SYSGAIN		1100
#define C_MAX_BASEGAIN		1100				/*显示的最大增益值*/
#define C_MAX_ADJUSTGAIN	800				/*仪器自动调节的最大增益*/
#define C_MAX_RANGE	  		50000				/*声程最大范围5000mm*/
#define C_MIN_RANGE			100
#define C_MAX_DELAY			(200 * 200 * SAMPLE_FREQ )		/*延时最大范围500ms*/
#endif

#define C_MAX_ADJUSTGAIN	800				/*仪器自动调节的最大增益*/
#define C_MAX_OFFSET		16*300
#define C_MAX_ZERO			16*300
#define C_MAX_REJECT 		160				/*延时最大范围80％*/
#define C_MAX_SURFGAIN		500					/*表面补偿增益最大值*/
#define C_MAX_RF_RANGE		14000				/*射频下声程最大范围2600mm*/
#define C_MAX_FREQUENCE		1500					/*频率最大值15.0MHz*/

#define C_MIN_TIMERANGE		320

#define C_MAX_ANGLE			900//810
#define C_MAX_SPEED			9999
#define C_MIN_SPEED			200
#define CD_SPEED_1			5920
#define CD_SPEED_2			3230
#define CD_RANGE			1250
#define CD_BASEGAIN			450
#define CD_COMPGAIN			0
#define CD_SURFGAIN			0

//键值
#define C_KEYCODMAX 36

#define C_KEYCOD_AUTOGAIN	11
#define C_KEYCOD_BASEGAIN	0
#define C_KEYCOD_GATE		1
#define C_KEYCOD_OFFSET		7
#define C_KEYCOD_RANGE		4
#define C_KEYCOD_DELAY		5
#define C_KEYCOD_REJECT		5
#define C_KEYCOD_ANGLE		8

#define C_KEYCOD_DAC		9
#define C_KEYCOD_SETUP		6
#define C_KEYCOD_RETURN		15
#define C_KEYCOD_INPUT		15
#define C_KEYCOD_CONFIRM	10
#define C_KEYCOD_PAGEDOWN	10

#define C_KEYCOD_MINUS1		13
#define C_KEYCOD_MINUS		13
#define C_KEYCOD_LEFT		13
#define C_KEYCOD_RIGHT		14
#define C_KEYCOD_PLUS		14

#define C_KEYCOD_SAVE		12
#define C_KEYCOD_DEL		12
#define C_KEYCOD_POINT		14
#define C_KEYCOD_ASCII		14
#define C_KEYCOD_CHANNEL	3
#define C_KEYCOD_YES		9
#define C_KEYCOD_NO			6

#define C_KEYCOD_SPEED		2
#define C_KEYCOD_COLOR		20


#define C_FUNC_1            16     //快捷功能1
#define C_FUNC_2	        17     //回波包络
#define C_FUNC_3	        18     //回波包络
#define C_FUNC_4	        19

#define C_FUNC_INIT	        0      //初始化
#define C_FUNC_ENVELOPE	    1      //回波包络
#define C_FUNC_ECHOMAX	    2      //峰值记忆
#define C_FUNC_DACGATE	    3      //DAC门
#define C_FUNC_DACADJUST	4      //深度补偿
#define C_FUNC_FILL	        5      //实心波
#define C_FUNC_GATEALARM    6      //门内报警
#define C_FUNC_DATA		    7      //数据处理
#define C_FUNC_FREEZ	    8      //波形冻结
#define C_FUNC_GATEEXPEND   9      //门内展宽


#define C_SENSITIVITY_GAIN	30	/*两次灵敏度校验间的最大差值，单位0.1dB*/

#define C_DAYS_PROBE		7	/*试块校验有效天数*/
#define C_DAYS_SYSTEM		30	/*系统性能校验有效天数*/
#define C_DAYS_SENSITIVITY	1	/*灵敏度校验有效天数*/

#define C_SAMEKEYTIME		400	/*对长时间算长按，单位1ms*/

void MSetAmplifierGain(u_short gain);
int MainProject(void);	//主项目调用
void SystemLog(void);   //开机画面


#endif


