//全局性的变量
//globe.h
#ifndef _GLOBE_H_
#define _GLOBE_H_

//1机务,2ndc
#define C_MAKELIB   0

#define C_DEVLIB	23	//23tft .24 EL

#define C_FILL 1	//实心波	//360b+ & F1 &350
//#define  PM_COPY_MOVE 1
#ifndef _CUDEFM1_H_
/*
#if C_DEVLIB == 1
	#include "cudefm1.h"
#elif C_DEVLIB == 2
	#include "cudefm2.h"
#elif C_DEVLIB == 23
	#include "cudefm2c.h"
#elif C_DEVLIB == 24
	#include "cudefm2e.h"
#elif C_DEVLIB == 3
	#include "cudefm3.h"
#elif C_DEVLIB == 4
	#include "cudefm4.h"
#endif

*/
#include "M10Era.h"
#define PM_COPY_MOVE 1
#define ARROW1      238
#define ARROW2      239
#define ARROW3      240
#define ARROW4      241
#define ENTER       242
#define PHI         243
#define ANGLE       160

/*
(phi)= 'z' + 14     ==216
(angle) = 'z'+ 8    ==167
(\>) = 'z'+9        ==211
(->) = 'z'+10       ==212
(|>) = 'z'+11       ==213
(^) = 'z' + 12      ==214
(enter) = 'z'+ 15   ==215
*/
/* CH( 100) = " " */
#if C_DEVLIB == 4
#ifndef DP_NOT
#define DP_NOT 0
#endif
#endif

#ifndef DP_SET
#define DP_SET 0
#endif
#ifndef DP_NOT
#define DP_NOT 0
#endif

#ifndef USB_DEV_DATA_TRANSFER_OK
#define USB_DEV_DATA_TRANSFER_OK	USB_DATA_TRANSFER_OK
#endif
#ifndef USB_DEV_DATA_TRANSFER_OK
#define USB_DEV_DATA_TRANSFER_OK	USB_DATA_TRANSFER_OK
#endif
#ifndef USB_DEV_DATA_TRANSFER_BUSY
#define USB_DEV_DATA_TRANSFER_BUSY	USB_DATA_TRANSFER_BUSY
#endif
#ifndef USB_DEV_DATA_TRANSFER_ERROR
#define USB_DEV_DATA_TRANSFER_ERROR	USB_DATA_TRANSFER_ERROR
#endif
#ifndef USB_DEV_DATA_TRANSFER_DECLINED
#define USB_DEV_DATA_TRANSFER_DECLINED	USB_DATA_TRANSFER_DECLINED
#endif

#define _CUDEFM1_H_
#endif
//#include "sysdef.h"
#include <math.h>

//单片机交互指令
#define LED_ALARM_ON			100
#define LED_ALARM_OFF			101
#define BUZZER_ALARM_ON			102
#define BUZZER_ALARM_OFF		103
#define	KEYCODE_T_REQ			104
#define LOW_BATTERY_OFF			105
#define DETECTOR_ON_CODE		106
#define DETECTOR_ON_ACK			107
#define DETECTOR_OFF_CODE		108
#define DETECTOR_OFF_ACK		109

#define RTC_R_YEAR				110
#define RTC_R_MONTH				111
#define RTC_R_DATE				112
#define RTC_R_HOUR				113
#define RTC_R_MINUTE			114
#define RTC_R_SECOND			115
#define RTC_R_DAY				116

#define RTC_R_YEAR_ACK			117
#define RTC_R_MONTH_ACK			118
#define RTC_R_DATE_ACK			119
#define RTC_R_HOUR_ACK			120
#define RTC_R_MINUTE_ACK		121
#define RTC_R_SECOND_ACK		122
#define RTC_R_DAY_ACK			123

#define RTC_W_YEAR				124
#define RTC_W_MONTH				125
#define RTC_W_DATE				126
#define RTC_W_HOUR				127
#define RTC_W_MINUTE			128

#define RTC_W_YEAR_ACK			129
#define RTC_W_MONTH_ACK			130
#define RTC_W_DATE_ACK			131
#define RTC_W_HOUR_ACK			132
#define RTC_W_MINUTE_ACK		133

#define RTC_W_READY				134
#define RTC_W_ACK				155
#define RTC_W_REQ				196

//0黑白1彩色
/*
const u_int c_crPara[] = {
	0x007,//红
	0x0c7,//粉红
	0x03f,//黄
	0x038,//绿
	0x0f8,//青
	0x0c0,//蓝
	0x084,//紫
	0x0ff,//白
};*/
/*
1. 一次回波：
2。二次回波：
3。峰值包络：
4。提示菜单：
5。波门A：
6。波门B：
7。定量曲线：
8. 坐标参量
*/
#define C_CR_BACK		0
#define C_CR_WAVEBACK 	1
#define C_CR_WAVE 		2
#define C_CR_SEWAVE 	3
#define C_CR_MENU		4
#define C_CR_CURVE		5
#define C_CR_PARA		6
#define C_CR_GATEA		7
#define C_CR_GATEB		8
#define C_CR_ENVELOPE 	9


#define C_CR_MAX		10

#define C_CR_UNDO		11
#define C_CR_CURRENT 	12
#define C_CR_DEFAULT	13

#define C_CR_RED			0
#define C_CR_GREEN			3
#define C_CR_BLUE			5
#define C_CR_YELLOW			2
#define C_CR_PINK			1
#define C_CR_CYAN			4
#define C_CR_WHITE			7
#define C_CR_BLACK			8
#define C_CR_PURPLE			6
#define C_CR_NUMBER			9

#define C_EXPF 2.302585

#define C_SAMPLE_FREQ (200 * SAMPLE_FREQ )

#define C_DEBUG		1

//#if C_MAKELIB == 2
//#define C_CHMAX				5					/*通道数*/
//#else
//#define C_CHMAX				10				/*通道数*/
//#endif

#define C_PI 3.1415926535897932
#define UINT	unsigned int
#define  PI		3.1415926535897932

#define C_LANGUAGE	2
//正常16*16字库
#define HZCODE  160
//自建16*16字库
#define HZCDEF  144
//自建16*8文字库
#define HZCEDEF  140
//正常8*8字库
#define HZECODE  32
//自建8*8字库,< 32
#define HZEDEF  16

#define TXAL	6
#define TXAR	7
#define TXALR	8
#define TXAC	9
#define TXCR	10
#define TXEOF	0
#define TXSP	32
/*
#define CH(a)		( ( (a)/100 )*256 + ( (a)%100 ) )
#define ch(a)		( ( (a)/100 )*256 + ( (a)%100 ) )
*/
//#define C16(a)		( (a)/100 ) + 144 ),( (a)%100 )  + 144)
//#define c16(a)		( (a)/100 ) + 144 ),( (a)%100 )  + 144)
//正常16*16字库
#define HF(a)		( (a)/100  + HZCODE ),( (a)%100   + HZCODE)
//自建16*16字库
#define FF(a)		( (a)/100  + HZCDEF ),( (a)%100   + HZCDEF)
//自建16*8字库
#define FO(a)		( (a)/100  + HZCEDEF ),( (a)%100   + HZCEDEF)
//正常8*8字库
#define HO(a)		( (a)/100  )
//自建8*8字库
#define OO(a)		( (a)/100  + HZEDEF)

#define C_INPUT_DIGITAL		0
#define C_INPUT_CAPITAL		(C_INPUT_DIGITAL+1)
#define C_INPUT_LOWERCASE	(C_INPUT_CAPITAL+1)
#define C_INPUT_PINYIN		(C_INPUT_LOWERCASE+1)
#define C_INPUT_QUWEI		(C_INPUT_PINYIN+1)
#define C_INPUT_ALL			(C_INPUT_QUWEI + 1)

/*输入法总数*/
#define C_MAX_INPUTMODE		C_INPUT_ALL

#define C_ZPROBE	0					/*直探头为0*/
#define C_APROBE	(C_ZPROBE+1)		/*斜探头为1*/
#define C_DPROBE	(C_APROBE+1)		/*双晶探头为2*/
#define C_MANUAL	(C_DPROBE+1)



//const u_char MadeDate[21] = "2003-04-10 A10      ";	/*长20*/
#define C_P_NORMAL	0
#define C_P_ANGLE	1
#define C_P_DOUBLE	2
#define C_P_TRANSMISSION	3


#define C_SEGMENT_VIDEO  0x2000
#define C_OFFSET_VIDEO  1
#define C_SCREEN_WIDTH	500 //308
#define	C_VERTDOT_VIDEO  480    //240
#define	C_HORIDOT_VIDEO  1024   //512
#define	C_HORIDOT_SCREEN  640   //320
#define	C_VERTDOT_SCREEN  480   //240

#define C_HORIDOT  500
#define C_MenuHigh  30
#define C_PeakHPOSI 20
#define C_PeakVPOSI 10
#define C_COORHPOSI	1
/*坐标从上向下垂直位置点数*/
#define C_COORVPOSI  55 //28
/*坐标高度*/
#define C_COORHEIGHT  400   //200
/*坐标水平每格点数*/
#define	C_COORHORIUNIT  (48)  //(28)
#define	C_HORIUNIT  48  //28
/*坐标水平总点数*/
#define C_COORWIDTH  (10*C_COORHORIUNIT)
/*坐标水平总点数*/
#define C_WAVEWIDTH  (C_COORWIDTH-2)
#define C_COORVPOSI2  460   //229
#define C_GATEPOSIMAX (C_COORWIDTH+10)

#define C_STD_AWS_NUM 10
#define C_STD_11345_2013_NUM 11


#define C_ECHAR_VDOT  24		/*一个英文字符垂直方向点数*/
#define C_ECHAR_HDOT  16		/*一个英文字符水平方向点数*/
#define C_CCHAR_VDOT  24	/*一个中文字符垂直方向点数*/
#define C_CCHAR_HDOT  24	/*一个中文字符水平方向点数*/

//时间单位，MILLION：毫秒，SECOND:秒，minute:分钟
#define C_TIME_MILLION	0
#define C_TIME_SECOND	1
#define C_TIME_MINUTE	2

#define C_PRB_NUM 5
//每个通道的探头数0在用探头,1初探2核查组合3斜4单直5双直
#define C_PRB_1ST 4
#define C_PRB2_COMB 0
#define C_PRB2_ANGLE 1
#define C_PRB2_NORMAL 2
#define C_PRB2_DOUBLE 3
//初探/组合/斜/直/双直

#define	PASS			0x0
#define FOR_CHECK		0x40
#define CHECK_PASS		0x80
#define PERIMETER_DOT	0xc0 /*1100 周向点状	192*/
#define PERIMETER_PLANE	0xd0 /*1101				208*/
#define RADII_DOT		0xe0 /*1110	径向点状	224*/
#define RADII_PLANE		0xf0 /*1111				240*/

#define NORMAL			0
#define DOUBLE_OUT		1
#define DOUBLE_IN		2
#define TRANSMISSION	3
#define AXES			0
#define RADIAL			1

/* 保护*/
#define C_LOCK_OFFSET		0
#define C_LOCK_DACAVG		1
#define	C_LOCK_PROBE		2
#define	C_LOCK_SYSTEM		3
#define C_LOCK_ALL			3
/*
1.全部数据
2.定位参数
3.定量设置
4.探头参数
5.系统变量
*/


#define C_KEYDELAY 25


//#define C_SIZE_INT		( sizeof(int) )
#define C_SIZE_INT			( 4 )
#define C_SIZE_SHORT		( 2 )
#define C_SIZE_CHAR			( 1 )
#define C_TRUE	1
#define C_FALSE 0


#define C_SETMODE_SAVE		0					/*调用函数时只存*/
#define	C_SETMODE_SET		(C_SETMODE_SAVE+1)	/*调用函数时只设置*/
#define	C_SETMODE_SETSAVE	(C_SETMODE_SET+1)	/*调用函数时既存也设置*/

#define C_GAINPLUSTIME		4000	/*增益加时间,单位为1ms*/
#define C_GAINMINUSTIME		1500	/*增益加时间,单位为1ms*/
#define C_PARADISPLAYTIME	50		/*波形参数显示时间,单位为1ms*/

#if C_DEVLIB == 1 || C_DEVLIB == 2
#define	C_BRIGHTNESS_MAX	31		/*最亮*/
#define	C_BRIGHTNESS_MID	13		/*次亮*/
#define	C_BRIGHTNESS_COM	9		/*一般*/
#define	C_BRIGHTNESS_MIN	5		/*最暗*/
#elif C_DEVLIB == 24
#define	C_BRIGHTNESS_MAX	0		/*最亮*/
#define	C_BRIGHTNESS_MID	19		/*次亮*/
#define	C_BRIGHTNESS_COM	23		/*一般*/
#define	C_BRIGHTNESS_MIN	27		/*最暗*/
#elif C_DEVLIB == 23
#define	C_BRIGHTNESS_MAX	200		/*最亮*/
#define	C_BRIGHTNESS_MID	120		/*次亮*/
#define	C_BRIGHTNESS_COM	80		/*标准*/
#define	C_BRIGHTNESS_MIN	40		/*一般*/
#elif C_DEVLIB == 3
#define	C_BRIGHTNESS_MAX	31		/*最亮*/
#define	C_BRIGHTNESS_MID	10		/*次亮*/
#define	C_BRIGHTNESS_COM	3		/*一般*/
#define	C_BRIGHTNESS_MIN	0		/*最暗*/
#endif

#define C_PULSE_MIN		18  //33
#define C_PULSE_MED		33  //36
#define C_PULSE_MAX		36  //40

//工艺
#define C_CRAFT_NORMAL		0		/*常规探伤*/
#define C_CRAFT_TRANS		1		/*直探头穿透*/
#define C_CRAFT_MAX			2		/*工艺总数*/
/*
参量名
*/
#define C_BASEGAIN			0
#define C_SURFGAIN			(C_BASEGAIN + 1)
#define C_COMPGAIN			(C_SURFGAIN + 1)
#define C_RANGE				(C_COMPGAIN + 1)	/*分垂直水平距离三种*/
#define C_SPEED				(C_RANGE + 3)
#define C_OFFSET			(C_SPEED + 1)
#define C_DELAY				(C_OFFSET + 1)		/*分垂直水平距离三种*/
#define C_REJECT			(C_DELAY + 3)
#define C_ANGLE				(C_REJECT + 1)
#define C_AGATEPOSI			(C_ANGLE + 1)
#define C_AGATEWIDE			(C_AGATEPOSI + 1)
#define C_AGATEHIGH			(C_AGATEWIDE + 1)
#define C_BGATEPOSI			(C_AGATEHIGH + 2)
#define C_BGATEWIDE			(C_BGATEPOSI + 1)
#define C_BGATEHIGH			(C_BGATEWIDE + 1)
#define C_CHANNEL			(C_BGATEHIGH + 2)

#define C_TESTDAYSMAX_0		184
#define C_TESTDAYSMAX_1		62
#define C_TESTDAYSMAX_2		15
/*
功能名
*/
#define C_ECHO_ENVELOPE		0		/*包络*/
#define C_ECHOMAX_MEMORY	1		/*峰值记忆*/
#define C_GATE_ALARM		2		/*门内报警*/
#define	C_DAC_GATE			3		/*DAC门*/
#define C_DEPTH_COMPENSATE	4		/*深度补偿*/
#define	C_CURVER_AMEND		5		/*曲面修正*/
#if C_FILL == 1
#define	C_WAVE_FILL		    7		/*实心波形*/
#define C_WAVE_EXPEND		6		/*门内展宽*/
#define C_BSCAN				8
#define C_AUTOADJUST_GAIN	9		/*自动增益*/
#define C_FLAW_HEIGHT		10
//#define C_INITIALIZE		9
#define C_FUNCTION_ALL		11
#define C_FUNCTION_MAX		8
#else
#define	C_WAVE_FILL		    11		/*实心波形*/
#define C_WAVE_EXPEND		6		/*门内展宽*/
#define C_BSCAN				7
#define C_AUTOADJUST_GAIN	8		/*自动增益*/
#define C_FLAW_HEIGHT		9
//#define C_INITIALIZE		9
#define C_FUNCTION_ALL		10
#define C_FUNCTION_MAX		7
#endif

/*Function
d0:包络
d1:峰值
d2:报警
d3:DAC门
d4:深度补偿
d5:自动增益
d6:曲面修正
d7:
在测试时d0-d2可使用
*/

#define C_RF_WAVE ( RF_WAVE - FULL_WAVE)


/*------------
thick;		//工件厚度
angle[4];	//折射角1

zero[4];	//零点
g_base[4];	//基本增益
g_comp[4];	//补偿增益
diameter;	//轮箍直径
Frequence;	//探头频率：2.5Mhz

SurfGain;	//表面补偿增益
BaseGain;	//基本增益
CompGain;	//补偿增益

Range;
Delay;
Offset;
*/


/*
结构体变量定义
*/
#if C_MAKELIB > 0
#define C_USERMAX		5		/*员工最大数*/
#else
#define C_USERMAX		1		/*员工最大数*/
#endif
#define C_USERNAMELEN	4		/*姓名长度*/
#define C_USERNUMLEN	10		/*工号长度*/
#define C_SUPERPWD		13578642		/*默认超级密码*/
#define C_DEFPWD		1470		/*初始密码*/

typedef struct
{
    u_char  funcType[9];	//功能种类
    u_char	funcMenu[4];	//4个功能键F1-F4
} FUNCTION;


typedef struct
{
    u_int	pwd;		//密码
    u_short name[ ( (C_USERNAMELEN+2)/2 )*2];	//姓名
    u_char	num[( (C_USERNUMLEN+4)/4 )*4];	//工号
} USER;

typedef struct
{
    int left;
    int top;
    int bottom;
    int right;
} RECT;

typedef  struct
{
    unsigned int type:3	;	//类型0无，1单面，2双面，3T型
    unsigned int face:3 ;	//探测面0A面，1B面，2C面
    unsigned int thick1:14 ;	//厚度1，工件厚度、翼板厚度<200.0mm;
    unsigned int thick2:14 ;	//厚度2，有效厚度、腹板厚度<200.0mm;
    unsigned int wide:10 ;	//焊缝宽度、A面焊脚，< 100mm;
    unsigned int radius:10 ;	//根部半径、B面焊脚，< 100mm;
    unsigned int high1:7 ;	//上端余高、A端余高，<100mm;
    unsigned int high2:7 ;	//下端余高、B端余高，<100mm;
    unsigned int angle1:11;	//坡口角度、A坡口角，< 180;
    unsigned int angle2:10;	//坡口面角、B坡口角，< 90;
    unsigned int deep:10;	//坡口深度，< 100mm;
    unsigned int blunt:10;	//钝边高度，< 100mm;
    unsigned int space:10;	//根部间隙，< 100mm
    unsigned int probe:14;

    unsigned int angle3:10;	//坡内角度，< 180;
    unsigned int angle4:10;	//坡内面角，< 180;
    unsigned int deep3:10;	//坡内深度，< 100mm;
} WELD_PARA;

typedef  struct
{
    unsigned short lpos;//显示工件左端
    unsigned short rpos;//显示工件右端
    unsigned short xpos;//焊缝的中心水平
    unsigned short ypos;//焊缝的中心垂直
    unsigned short size;//显示工件的大小
    unsigned short wxpos;
    unsigned short wypos;//
    unsigned short dots;
} WELD_POSI;

typedef struct
{
    u_int nElapsedTime;			//开机时长，以毫秒为单位
    u_int nStartElapsedTime;	//高16位开机时间，低16位开机时长，以分为单位
    u_int days:16;				//距2000年1月1日的天数
    u_int denoise:3;			//降噪次数
    u_int repeat:10	;			//重复频率
    u_int pulsewidth:1;			//脉冲宽度
    u_int fill:1;				//实心波
    u_int sys;					//d31-d29打印机，d28-26输入法,d25测试状态,d24采样状态，d23硬件波形，d22软件波形
    //d21允许光标，d20有无光标，d19-d16用户数(最大16)，d15-d12通道号(最大16)
    //d11按键声音d10报警声音,d9d8屏幕亮度，d7-d0输入法是否可选

    u_int option;				//d31波形记录方式(0单幅1连续),d30启动连续存,d29-d22记录间隔(单位为0.1秒,最大25.0秒)
    //d21存储时是否记录缺陷波频谱，d20-d16当前选择的探伤工艺
    //d15d14颜色模式
    //D13曲线显示模式，0波幅，1dB值，此时自动调整增益
    //D12键盘锁
    //d11,d10门A门B内回波是否高于门高
    //d9=1A门变色,d8=1B门变色
    //d7=0峰值读数1前沿读数

    //U2: start
    u_int displaymode:1;		//显示方式,1:全屏,0非
    u_int mainmenuitem:5;		//当前主菜单项数,每页6项,最多24项
    u_int samp_len:10;			//显示回波长度
    u_int coor_width:9;			//坐标区长度
    u_int coor_extern:6;		//坐标区右方扩展点数
    u_int hunit:6;				//坐标区水平方向每格点数
    u_int language:2;			//语言
    u_int unit:2;				//单位
    u_char menu2page[24];		//次级菜单页数,最多4页,每页4项
    //U2: end
    u_int prepare[4];			//预留值

} SYSTEM_STATUS ;		//整个系统的状态 68 BYTES

typedef struct
{
    u_int sys;				//仪器探头状态，31d30:频带宽度：2.5\5.0\10\wide，
    //d29d28检波方式：全波\正半波\负半波\射频 d27d26探头阻尼：50Q\400Q
    //d25d24发射电压：200V\400V ,d23-d20重复频率，
    //d19d18降噪次数：1-4次，d17d16波形:纵波/横波
    //d15-d12探头方式：单直/单斜/双晶/穿透
    //d11-d8标准号
    //d7-d0功能

    u_int block;			//试块，d19-d0一位对应一种试块，1表示在试块选择菜单中会出现
    //d31-d24高8位为当前所选试块
    //u_short probe;		//探头,波型D3D2横波/众波，直/斜/双晶/穿透d1d0,组合探头d6-d4
    //d9d8核查用探头00组合探头01小于K1斜探头10单晶直探头11双晶直探头
    //d11初查用d12核查用组合探头d13小于K1斜探头d14单晶直探头d15双晶直探头=0未测1已测,

    u_int option;			//d31－d28当量标准:0000母线/从上到下，
    //d27d26当量显示：%/dB值/孔径,d25d24标度：垂直\水平\距离
    //d23-d16参量锁
    //d15-d9参量步长,d15d14基准增益，d13d12调节增益，d11声程，d10延时，d9声速
    //d8-d5参量类别，d8:基准增益\调节增益，d7:门A\门B，D6D5：门位\宽\高
    //d4B门类型
    //d2-d0曲线数目

    u_int status;			//d31-d24仪器性能测试天数，D23-d18探头测试天数(零点、K值、DAC)，
    //d17－d14灵敏度测试天数，d13-d7基准波高(0-128)
    //d6-d0:d0:零点声速，D1：K值\折射角，D2：DAC,D3:AVG,
    //3300+: start
    u_int externstatus;		//d0 探测面：内侧/外侧
    //d1 自动增益大小
    //d2 报告格式 d3 模拟或数字探伤(0数字，1模拟),d4-d10基准高度(0-128),d11,12通用、标准,其他。
    //d13,d14 大平底，圆柱面，空心轴
    //3300+: start
    u_int prepare[4];		//预留值

} CHANNEL_STATUS ;	//每个通道的状态 36 BYTES


typedef struct
{
    u_int		Range;
    u_int		OldRange;		//展宽前的声程
    u_int		OldDelay;		//展宽前的延时
    u_int		Delay;

    u_int		wavepara[4];	//波形的四个参数(距离,水平,垂直,高度/位置,高度,位置,高度)

    u_short		Thick;
    u_short		Diameter;		//直径

    u_short		Offset;
    u_short		Speed;			/*试件声速：	m/s	*/
    u_short		Angle;			/*折射角：60o	*/
    u_short		Forward;		/*探头前沿长度	*/
    u_short		Frequence;		/*探头频率：2.5Mhz	*/

    u_short		Reject;

    u_short		SurfGain;		/*表面补偿增益	*/
    u_short		BaseGain;		/*基本增益	*/

    u_short		DepthGain;		//深度补偿值
    short		CompGain;		/*补偿增益	*/

    short		lineGain[6];	//六条线偏移

    short		gatedB;
    short		depth;			//缺陷高度

    u_short		Crystal_l;		/*晶片尺寸	*/
    u_short		Crystal_w;		/*晶片尺寸	*/

    //3300+: start
    u_short		PoreLong;		//横孔长度
    u_short		PoreDiameter;	//横孔直径
    u_short		PoreDepth[2];	//2横孔孔深
    //3300+: end
    u_int		prepare[4];		//预留值

} CHANNEL_PARAMETER;		//100 BYTES


typedef struct
{
    u_int sys;			//d31d30屏幕亮度,d29-d27屏保格式,d26是否要密码，d25-d21屏保延时(不超过30，单位为分)
    u_int option;		//d31-d24当前选择的探测工艺，

} USER_STATUS;


#define C_CS_BLOCK			0
#define C_CS_COMPCONT		1
#define C_CS_LOCOMOTIVE		2
#define C_CS_NUMBER			3
#define C_CS_OBJECT			4
#define C_CS_OPTION			5
#define C_CS_STATUS			6
#define C_CS_SYS			7


typedef struct
{
    short cgain[C_CRAFT_MAX];
    u_short bgain[C_CRAFT_MAX];
    u_short sgain[C_CRAFT_MAX];
    u_short offset[C_CRAFT_MAX];
    u_short speed[C_CRAFT_MAX];
    u_short angle[C_CRAFT_MAX];
    u_short range[C_CRAFT_MAX];
    u_short ampstd[C_CRAFT_MAX];
} USER_PARA;

typedef struct
{
    u_int		wavepara[4];	//波形的四个参数(距离,水平,垂直,高度/位置,高度,位置,高度)
    u_char      sampbuff[C_COORWIDTH];	//包络线
} WAVEPARA;
typedef struct
{
    int		posi;		//文件位置
    u_int	date;		//存储日期 (256*(256*(100*yearh + yearl) + month) + date)
    u_short	name[12];	//文件名
} FAT;		//

typedef struct
{
    u_short	name[30];	//标签名
} NOTES;		//标签

typedef struct
{
    u_short		Thick;
    u_short		Diameter;	//直径
    u_short		Angle;
    u_short		Half_Len_Bow;	//半弦长
    u_short		Half_len_Arc;	//半弧长
    u_short		Beta;			//半弦长对应的角度
} CURVE_AMEND;
typedef struct
{
    u_short GainPlusTime;		/*增益加时间*/
    u_short GainMinusTime;		/*增益减时间*/
    u_short ParaDisplayTime;		/*参数显示时间*/
    u_int	PreElapsedTime;			/*前次读出的开机时长*/

    int amp_total;		//累计波形高度
    int amp;		//前次平均波高
    int amp_total2;		//B门内累计波形高度
    int amp2;		//B门内前次平均波高
    int time;		//读波的次数
    int time2;		//B门内读波的次数

} ADJUST_TIME;		/*各种调节时间,单位为1msec*/

typedef struct
{
    u_int 	Time;	//传输时间
    u_int	Dist;	//距离声程
    u_short	Pos;	//在屏幕上的水平点数
    u_short dB;		//最高波的dB数
    short  Amp;		//门内最高波

} GATE_MAX;		//门内出现的最高波
typedef struct
{
    u_char mode[2];
    int para[4];
} GATE_AMPMAX;

typedef struct
{
    u_char	date;
    u_char	month;
    u_char	yearl;
    u_char  yearh;
} SAVE_DATE;

typedef struct
{
    u_char      Channel;        //通道数
    short       FixGain;		//突变点的波幅调节(突变点在32.9-33.0之间)9
    u_short		pzPurchaser[11];	//购买单位	22，11个字
    u_short		pzSerial[20];		//内部仪器编号20
    TIME_CLOCK	StorageTime;		//入库日期10\8
    TIME_CLOCK	LeaveFactoryTime;	//出厂日期10\8
    TIME_CLOCK	MaintainTime;		//检修出厂日期10\8
} INNER_INFO;

typedef struct
{
    u_short 	pzPurchaser[11];	//单位名称	22，11个字
    u_short		pzSerial[20];		//仪器编号20
    TIME_CLOCK	PurchaseTime;		//购买日期10\8
    u_short		pzUser[5];			//使用人员10
    u_short		pzGovernor[5]; 		//主管人员10
} USER_INFO;


typedef struct
{
    int			input;
    u_short		BugLength;	//缺陷长度
    u_short		BugHeight;	//缺陷高度
    u_short 	TestDep[20];		//检验单位
    u_short		EntrustDep[20];		//委托单位
    u_short		WorkpieceName[20];	//工件名称
    u_short		WorkpieceNum[20];	//工件编号
    u_short		WorkpieceSpec[20];	//工件规格
    u_short		MaterialNumber[20];	//材料牌号
    u_short		SurfaceStatus[20];	//表面状况
    //u_short		HeatTreatment[11];	//热处理
    //u_short		coupling[11];		//耦合剂
    //记录编号
    //u_short		position[11];		//检测部位
    //u_short		scale[11];			//检测比例
    //u_short		time[11];			//检测时机
    //u_short		standard[11];		//验收标准
    //u_short		level[11];			//合格级别
} REPORT_INFO;

typedef struct
{
    u_short		dac_db[C_COORWIDTH];		//母线
    u_short		dac_high[C_COORWIDTH];		//曲线高度
} DAC_LINE;

typedef struct
{
    u_short db[10];
    u_short dist[10];
    short num;
    u_short diameter;//反射体直径
    u_short length;//反射体长度
} DAC;


//probe
typedef struct
{
    u_short pzProbeManufactory[13] ;	//探头厂家
    u_short pzProbeSerial[11] ;		//探头编号
    u_int	date;			//存储日期 (256*(256*(256*yearh + yearl) + month) + date)

    short ProbeMode;		//探头类型
    //short Horiline;		//水平线性
    //short Vertline;		//垂直线性
    short Lvalue;			//前沿长度
    short Angle;			//折射角
    short Frequence;		//频率
    short Sensitivity;		//灵敏度余量
    short StartWaveWidth;	//始波宽度
    short ResolvingPower1;	//分辨力
    short ResolvingPower2;	//分辨力
    short Bevel;			//偏斜角
} PROBE_CAPACITY;

u_short gaintable[121];



//------仪器性能-----
typedef struct
{
    u_int test;
    u_int Horiline;			//水平线性
    u_int vertline;			//垂直线性
    u_int ResolvingPower;	//分辨力
    u_int Sensitivity;		//灵敏度余量
    u_int Dynamic;			//动态范围
    int gain[5];	//记录5个状态下的增益0水平1垂直2分辨力3灵敏度余量4动态范围
} POWER;

//

#define C_CHANNEL_ZPROBE	0							/*直探头通道号为0*/
#define C_CHANNEL_APROBE	(C_CHANNEL_ZPROBE+1)		/*斜探头通道号为1*/
#define C_CHANNEL_LPROBE	(C_CHANNEL_APROBE+1)		/*小角度探头通道号为2*/
#define C_CHANNEL_MANUAL	(C_CHANNEL_LPROBE+1)

/*常量定义*/
#define C_MAX_SAMEKEYTIME	30		/*100*10ms = 1 sec*/

/*显示及存储常量*/
#define C_BASEGAIN_HPOSI	510//(9*16)
#define C_BASEGAIN_VPOSI	C_COORVPOSI+C_MenuHigh*2//(0)
#define C_COMPGAIN_HPOSI	510//C_BASEGAIN_HPOSI+4*16-3//(13*16)
#define C_COMPGAIN_VPOSI	C_COORVPOSI+C_MenuHigh*3//(0)
#define C_SURFGAIN_HPOSI	510//(17*16)
#define C_SURFGAIN_VPOSI	C_COORVPOSI+C_MenuHigh*4//(0)
#define C_RANGE_HPOSI		510//(25*16)
#define C_RANGE_VPOSI		C_COORVPOSI+C_MenuHigh*6//(0)
#define C_POWER_HPOSI		(( C_HORIDOT_SCREEN -2 * C_ECHAR_HDOT) /16*16 +4)
#define C_POWER_VPOSI		( C_COORVPOSI + C_COORHEIGHT -6)
#define C_SYSTIME_HPOSI		510//8//( ( C_HORIDOT_SCREEN - 3 * C_ECHAR_HDOT) /16 * 16 - 4)
#define C_SYSTIME_VPOSI		C_COORVPOSI+C_MenuHigh*11+4// + C_COORHEIGHT+1//( C_COORVPOSI + C_COORHEIGHT + C_ECHAR_VDOT+1)
#define C_CHANNEL_HPOSI		564//( C_COORWIDTH + 10 )
#define C_CHANNEL_VPOSI		C_COORVPOSI//C_COORVPOSI+5//( C_COORVPOSI + 80+4 )

#define C_PBASEGAIN_HPOSI	(33*8)
#define C_PBASEGAIN_VPOSI	(0)
#define C_PCOMPGAIN_HPOSI	(33*8)
#define C_PCOMPGAIN_VPOSI	(0)
#define C_PRANGE_HPOSI		(22*8)
#define C_PRANGE_VPOSI		(0)
#define C_PPOWER_HPOSI		(37*8 + 3)
#define C_PPOWER_VPOSI		(0)

//#define C_SIZE_INT		( sizeof(int) )


#define C_SETMODE_SAVE		0					/*调用函数时只存*/
#define	C_SETMODE_SET		(C_SETMODE_SAVE+1)	/*调用函数时只设置*/
#define	C_SETMODE_SETSAVE	(C_SETMODE_SET+1)	/*调用函数时既存也设置*/

#define C_GAINPLUSTIME		4000	/*增益加时间,单位为1ms*/
#define C_GAINMINUSTIME		1500	/*增益加时间,单位为1ms*/
#define C_PARADISPLAYTIME	50		/*波形参数显示时间,单位为1ms*/


//通道数据长度



#define	C_LEN_DAC		( (sizeof(DAC)/C_SIZE_INT +1)*C_SIZE_INT)
#define C_LEN_PEAK		( (sizeof(GATE_PEAK)/C_SIZE_INT +1)*C_SIZE_INT )	/*回波参数*/
#define C_LEN_SYSPARA	( (sizeof(SYSTEM_STATUS)/C_SIZE_INT +1)*C_SIZE_INT)		/*系统数据长度*/
#define C_LEN_SAMP		( (C_COORWIDTH/C_SIZE_INT +1)*C_SIZE_INT )					/*波形参数*/
#define C_LEN_WELD		( (sizeof(WELD_PARA)/C_SIZE_INT +1)*C_SIZE_INT )
#define C_LEN_CHSTATUS	( (sizeof(CHANNEL_STATUS)/C_SIZE_INT +1)*C_SIZE_INT )
#define C_LEN_POWER		( (sizeof(POWER)/C_SIZE_INT + 1)*C_SIZE_INT )
#define C_LEN_CHPARA	( (sizeof(CHANNEL_PARAMETER)/C_SIZE_INT +1)*C_SIZE_INT )
#define C_LEN_GATEPARA	( (sizeof(GATE_PARA)/C_SIZE_INT +1)*C_SIZE_INT )
#define C_LEN_GATEPEAK	( (sizeof(GATE_PEAK)/C_SIZE_INT +1)*C_SIZE_INT )

#define C_LEN_CHNPARA	( C_LEN_CHSTATUS + C_LEN_CHPARA + 2*C_LEN_GATEPARA + C_LEN_DAC+C_LEN_WELD)
#define C_LEN_USER		( (sizeof(USER)/C_SIZE_INT +1)*C_SIZE_INT * C_USERMAX )
#define C_LEN_USERPARA	( (sizeof(USER_PARA)/C_SIZE_INT +1)*C_SIZE_INT * C_USERMAX )
///#define C_LEN_CHNPARA	( C_LEN_CHSTATUS + C_LEN_CHPARA + 2*C_LEN_GATEPARA )
#define C_LEN_CURVE		( (sizeof(CURVE_AMEND)/C_SIZE_INT +1)*C_SIZE_INT )

#define C_LEN_FILE_C		(C_LEN_SYSPARA + C_LEN_CHNPARA + C_LEN_PEAK + 2*C_LEN_SAMP)
#define C_LEN_FILE		(((C_LEN_SYSPARA + C_LEN_CHNPARA + C_LEN_PEAK + 2*C_LEN_SAMP +255)/256)*256)
///#define C_LEN_FILE		(C_LEN_SYSPARA + C_LEN_CHNPARA + C_LEN_PEAK + 1*C_LEN_SAMP + C_LEN_DAC)
#define C_LEN_FAT_C		( (sizeof(FAT)/C_SIZE_INT +1)*C_SIZE_INT )			/*每个文件头长度*/
#define C_LEN_FAT		( ((sizeof(FAT)+255)/256)*256)			/*每个文件头长度*/
#define C_LEN_DACDB		(((500 * C_SIZE_SHORT)/C_SIZE_INT +1)*C_SIZE_INT)

#define C_LEN_FUNCTION	( (sizeof(FUNCTION)/C_SIZE_INT +1)*C_SIZE_INT)		/*功能热键长度*/

#if C_MAKELIB > 0
#define C_FILE_NUMBER	200					/*能存储的文件数*/
#else
#define C_FILE_NUMBER	1000					/*能存储的文件数*/
#endif

typedef struct
{
   int cache[C_FILE_NUMBER];	//
} CACHE;

#define C_OFF_INIT		0//(0+64*1024)
#define C_OFF_PWD		20//(20+64*1024)					/*密码区首地址*/
#define C_OFF_STORE		40//(40+64*1024)					/*保护区首地址*/
/*
	0-3			在函数调用时检测的同一按键持续时长
	4-11		画光标时的光标位置
	20			前次按键的键值
	21			同一键值连续按键次数
	22-25		有新按键时，距开机时的时长单位ms
	26-29		同一按键持续时长
	30-33		前次自动存储时的开机时间sec
	34-37		已自动存数据个数
	40			//前次读回波参数时的时间
	50			前次旋钮值
	54			前次取旋钮值时时间
	58			左旋钮值
	60			右旋钮值
*/

#define C_OFF_TEMP			( C_OFF_STORE + 100)	/*探伤过程临时存储区首地址*/
/*
	0-3			当前所用的声程值testrange[0]
	4-11		测试声速零点时一次和二次声程值testrange[1]/[2]共2个字，8
	20-27		测K值反射体中心深度和反射体直径
	28-29		测K值标称折射角
	30-33		DAC最大探测深度
	34-41		反射体直径和长度
	42-49		实际设置时的声程（已通过运算）
	50－57		测K值反射体中心深度和基准波高度

	60			颜色
*/
#define C_OFF_STACK			( C_OFF_TEMP + 100)		/*栈开始*/
/*
	0-4			栈指针,指向第一个空位置
	10-99		内容
*/
#define C_OFF_INNER_INFO	( C_OFF_STACK + 256*C_SIZE_INT) //121
/*
	0-29		购买单位	30，15个字
	30-69		内部仪器编号20
	70-77		入库日期8
	80-87		出厂日期8
	90-97		检修出厂日期8
*/
#define C_OFF_REPORT_INFO	( C_OFF_INNER_INFO + ((sizeof(INNER_INFO)+3)/4)*4)
/* */
#define C_OFF_USER_INFO		((( C_OFF_REPORT_INFO + sizeof(REPORT_INFO)+255)/256)*256)	/*用户信息*/
/*
	0-29		单位名称	30，15个字
	30-69		仪器编号20
	70-77		购买日期8
	80-89		使用人员10
	90-99		主管人员10
*/
#define C_OFF_BATTERY			((( C_OFF_USER_INFO + sizeof(USER_INFO)+8+255)/256)*256)/*电池信息*/
/*	保护区开始8个字节存开机时间，再8字节存当前时间...
	0-7			开机时间8
	10-17		关机时间8
	20-31		电池指示表11
	36-39		开机时长,单位分钟
	40-		开始充电时间,单位分钟
	80-320		电池电量记忆表
*/
#define C_OFF_USER			( C_OFF_BATTERY + 320)	/*人员信息*/
/*
0-3	nUserNumber
4-7 nCurrentUser
8-	stUser
*/
#define C_OFF_USERPARA		( ( ( C_OFF_USER + C_LEN_USER+C_SIZE_INT*2 + 1)/2)*2)	/*人员各工艺参数信息*/
/*
0-3	nUserNumber
4-7 nCurrentUser
8-	stUser
*/
#define C_OFF_DAC			( ( ( C_OFF_USERPARA + C_LEN_USERPARA+1)/2)*2)
/*
	0-9*C_SIZE_SHORT	测试距离
	10-19				换成128％时的增益数
	20(0-3)				测试点数
	21(4-8)
	22					增益
	23					声程
	24					延时
*/
#define C_OFF_CURVE			((( C_OFF_DAC + C_LEN_DAC+1)/2)*2)
//曲面参数
#define C_OFF_DACDB			((( C_OFF_CURVE + C_LEN_CURVE+1)/2)*2)
//DAC按dB值所得到的基准线
#define C_OFF_DACGATE		((( C_OFF_DACDB + C_LEN_DACDB+1)/2)*2)

#define C_OFF_POWER     (((C_OFF_DACGATE + C_LEN_DACDB+1)/2)*2)

#define C_OFF_SYSTEM 	(((C_OFF_POWER + C_LEN_POWER+2)/2)*2)

#define C_OFF_CHANNEL	(((C_OFF_SYSTEM + C_LEN_SYSPARA * C_USERMAX+1)/2)*2)/*保护区通道数据的首地址*/

#define C_OFF_BACKUP	(((C_OFF_CHANNEL + C_LEN_CHNPARA * C_CHMAX * C_USERMAX+1)/2)*2)
//备份系统参量和状态参量
#define C_OFF_KEYFUNC	(((C_OFF_BACKUP + C_LEN_CHNPARA + C_LEN_SYSPARA + 1)/2)*2)

#define C_OFF_FILEINFO	(((C_OFF_KEYFUNC+C_LEN_FUNCTION +511)/512)*512)
/*
	0-3		总数
	4-8		位置	struct FAT
	8－12	日期	y2-m1-d1
	12－35	文件名	SHORT 12 × 2
*/
#define C_OFF_PEAK		(((C_OFF_FILEINFO + C_SIZE_INT + C_LEN_FAT+1)/2)*2 )
//GatePeak

#define C_OFF_SAMP		(((C_OFF_PEAK + C_LEN_PEAK+1)/4)*4)
//sampbuff

#define C_OFF_FREQ		(((C_OFF_SAMP + C_LEN_SAMP+1)/4)*4)
//频谱
#define C_OFF_CACHE     (((C_OFF_FREQ + C_LEN_SAMP+255)/256)*256)

#define C_OFF_FAT		(((C_OFF_CACHE + sizeof(CACHE)+255)/256)*256)
/*
	0-3		位置
	4－7	日期	y2-m1-d1
	8－19	文件名
*/
#define C_OFF_FILE		(((C_OFF_FAT  + C_LEN_FAT * C_FILE_NUMBER+255)/256)*256)
#define C_OFF_OTHER		(((C_OFF_FILE + C_LEN_FILE * C_FILE_NUMBER+255)/256)*256)

#define C_OFF_F_BACKUP  (((C_OFF_OTHER+255)/256)*256)

#define C_OFF_FILEMAX   (((C_OFF_F_BACKUP+(C_OFF_FAT-C_OFF_FILEINFO)+255)/256)*256)

//测试状态名
#define C_TEST_OFFSET		0
#define C_TEST_ANGLE		(C_TEST_OFFSET + 1)
#define C_TEST_DAC			(C_TEST_ANGLE + 1)
#define C_TEST_AVG			(C_TEST_DAC + 1)
#define C_MAKE_DACAVG		(C_TEST_AVG + 1)
#define C_TEST_MAX			(C_MAKE_DACAVG + 1)
#define C_TEST_ALL			8

#define C_OFF_START
/*
0-4		主菜单位置
1-26*4	各工艺菜单位置
*/
#endif


