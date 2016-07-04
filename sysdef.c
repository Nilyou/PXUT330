#include <stdio.h>
#include "sysdef.h"
#include "display.h"
#include "main.h"
#include "common.h"
#include "globe.h"
#include "sdcard.h"

#include "Music_alice.c"

extern int HoriOffsetScreen ;		//所有写在屏幕上的内容，水平方向开始点
extern int VertOffsetScreen ;		//所有写在屏幕上的内容，水平方向开始点
extern ADJUST_TIME		AdjustTime;
extern NOTES		Notes;
//extern u_char crPara[];
extern u_char bCOM2PC;
extern const u_int c_crPara[] ;
extern const u_int Echo_crPara[] ;
extern  unsigned short c_FWBaseLine ;	//基线
extern  unsigned short c_RFBaseLine ;	//射频基线
int extend ;
unsigned short GainTable[121] ;
extern const unsigned short c_gaintable[2][121] ;

#if C_DEVTYPE == 14
const unsigned int KEY_DIGITAB[] =
{ C_KEYCODMAX,20,15, 1, 8, 2, 1,19,11,14, 5, 0,12,13,12, 4, 6,14,32,10,21, 7, 9, 9, 8,16,18,19, 7,17, 8,16,17,18, 2, 3, 3,10};
/*  350c  00,01,02,03,04,05,06,07,08,09,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36 */
#else
const unsigned int KEY_DIGITAB[] =
//               0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36
{   C_KEYCODMAX,17, 1, 4, 7,36,18, 2, 5, 8,36,19, 3, 6, 9,36,16, 0,12,10,36,11,14,13,15,36,36,36,36,36,36,36,36,36,36,36,36};
//{ C_KEYCODMAX, 9, 6, 3,19,36,36, 8, 5, 2,18,36,36, 7, 4, 1,17,36,36,10,12, 0,16,36,36,15,13,14,11,36,36,36,36,36,36,36,36,36};

#endif
//const unsigned char MadeDate[21] = "2005-03-15          ";	/*长20*/
extern int curr_cr;

extern const unsigned char _version[];//版本号
#if C_DEVTYPE == 1 && C_MAKELIB == 0
const unsigned char _version1[10] = "PXUT-330 ";//版本号
#elif C_DEVTYPE == 1 && C_MAKELIB == 1
const unsigned char _version1[10] = "JIWU     ";//版本号
#elif C_DEVTYPE == 1 && C_MAKELIB == 2
const unsigned char _version1[10] = "NDC      ";//版本号
#elif C_DEVTYPE == 2
const unsigned char _version1[10] = "U1       ";//版本号
#elif C_DEVTYPE == 3
const unsigned char _version1[10] = "350+     ";//版本号
#elif C_DEVTYPE == 4
const unsigned char _version1[10] = "320C     ";//版本号
#elif C_DEVTYPE == 5
const unsigned char _version1[10] = "V1       ";//版本号
#elif C_DEVTYPE == 6
const unsigned char _version1[10] = "MCh      ";//版本号
#elif C_DEVTYPE == 7
const unsigned char _version1[10] = "V1       ";//版本号
#elif C_DEVTYPE == 8
const unsigned char _version1[10] = "350C     ";//版本号
#elif C_DEVTYPE == 9
const unsigned char _version1[10] = "360B+    ";//版本号
#elif C_DEVTYPE == 10
const unsigned char _version1[10] = "U2       ";//版本号
#elif C_DEVTYPE == 11
const unsigned char _version1[10] = "350B+     ";//版本号
#elif C_DEVTYPE == 12
const unsigned char _version1[10] = "V2       ";
#elif C_DEVTYPE == 13
const unsigned char _version1[10] = "TFT      ";
#elif C_DEVTYPE == 14
const unsigned char _version1[10] = "F1       ";
#elif C_DEVTYPE == 15
const unsigned char _version1[10] = "PXUT-390 ";
#elif C_DEVTYPE == 20
const unsigned char _version1[10] = "300C     ";
#endif

const unsigned char AmpMaxCom_Table[] =
// 0-8%,8,	16,	24,	32,	40,	48,	56
{
    5,	4,	3,	3,	2,	2,	2,	1,		//;0-12.8dB
    5,	4,	4,	3,	2,	2,	2,	1,		//;-25.6dB(20-40
    5,	5,	4,	4,	2,	2,	2,	1,		//;-38.4dB(20-40
    5,	5,	4,	3,	2,	2,	2,	1,		//;-51.2dB(40-60
    4,	4,	3,	3,	2,	2,	2,	1,		//;-64.0dB(40-60
    4,	3,	2,	2,	2,	1,	1,	1,		//;-76.8dB(60-80
    3,	3,	2,	1,	1,	1,	0,	0,		//;-89.6dB(80-100
    2,	2,	1,	1,	1,	1,	0,	0,		//;-102.4dB(80-100
    2,	2,	1,	1,	1,	1,	0,	0,		//;-115.2dB(100-120 dB
    2,	2,	1,	1,	1,	1,	0,	0
};		//;-120.0dB(100-120 dB


const unsigned char PowerTable[] =
#if C_DEVLIB == 3 || C_DEVLIB == 23 || C_DEVLIB == 24
//	  100%, 90%,80%,70%,60%,50%,40%,30%,20%,10%,0%,	小于0%报警
    {  100,  90, 80, 70, 60, 50, 40, 30, 20,  10, 5,};
#elif C_DEVLIB == 4
//100%, 90%,80%,70%,60%,50%,40%,30%,20%,10%,0%,	小于0%报警
    {
        40, 37, 34, 32, 31, 29, 28, 27, 26, 24, 15,
    };
#else
//100%, 90%,80%,70%,60%,50%,40%,30%,20%,10%,0%,	小于0%报警
    {
        30, 27, 25, 23, 22, 21, 20, 19, 17, 15, 10,
    };
#endif
/*
 00,28,27,26,25,25,25,25,24,24,24,24
 24,23,23,23,23,23,23,22,22,22,22,22,
 21,21,20,20,19,19,18,18,17,16,16
    {  27, 25, 24, 23, 23, 22, 21, 20, 18,  17, 16};

//*/


const unsigned int _RANGEVALUE[] = {50,100,200,300,500,1000,1250,2000,2500,3000,4000};	/*大于300每步100*/


const unsigned short c_SystemPowerStd[] = { 20, 60, 260, 260, 460};	//性能标准水平、垂直、分辨力、动态、余量

const unsigned char _LOG_B1[] = {"   Digital Ultrasonic Flaw Detector   "};
const unsigned char _LOG_B2[] = {"           Self-test Done             "};
const unsigned char _LOG_B3[] = {"                                      "};
const unsigned char _LOG_B4[] = {"                                      "};
const unsigned char _LOG_B5[] = {"                                      "};
const unsigned char _LOG_B6[] = {"                                      "};
//////
const unsigned char _LOG_B7[] = {"  Manufactory                         "};
const unsigned char _LOG_B8[] = {"  China Nantong Union TECH&DEV Co.,Ltd"};
const unsigned char _LOG_B9[] = {"Press 9 Initialization, 0 Battery Info"};


const unsigned short _SYSMENU_B1[] = //一共8行，不等变动，每行不等超过20字，且每行必须以CH(16)结束
{
    //CH(16),//'P','X', 'U','T','-' ,'M','1',CH(16),
    CH(2394),CH(4681),CH(2864),CH(2229),CH(2171),CH(2716),CH(16),
    CH(4293),CH(5554),CH(4229),CH(1912),CH(4189),CH(1808),CH(4429),CH(4143),CH(5039),CH(16),
    CH(16),
    CH(5552),CH(2876),CH(4574),CH(1941),CH(16),//CH(4490),CH(3423),CH(2790),CH(4681),CH(5508),CH(5135),CH(16),
    CH(16),
#if C_UDISK == 1
    CH(1730),CH(2790),CH(4242),CH(5135),CH(4566),CH(2951),'U',CH(3744),CH(16),
    CH(4221),CH(4217),CH(2070),CH(2002),CH(3312),CH(4888),'A',CH(4108),CH(4293),CH(3061),CH(16),
#else
    CH(16),
    CH(16),
#endif
    CH(1620),'8',CH(2876),CH(2208),' ',CH(1620),'9',CH(1985),CH(4228),CH(2715),' ',CH(1620),'0',CH(1873),CH(3120),CH(2171),CH(1956),CH(4837),CH(4702),CH(16),
    CH(16),
    //CH(4077),CH(2894),CH(4721),CH(4519),CH(5141),CH(4346),CH(4616),CH(5487),CH(1954),CH(3110),CH(2302),CH(16),
//	CH(3647),CH(4508),CH(5149),CH(3310),CH(4293),CH(3475),CH(2828),CH(4285),CH(3110),CH(2302),CH(5148),CH(4762),CH(2511),CH(4330),CH(16),
};

const unsigned short _MainMenu_B1[] =  //主探伤界面显示的菜单，每行字数必须为6
{
    6/*行*/,
    CH(217),CH(4429),CH(4143),CH(4293),CH(3061),CH(2006),CH(3277),CH(16),
    CH(218),CH(4721),CH(4519),CH(1846),CH(4293),CH(4172),CH(5435),CH(16),
    CH(219),CH(5039),CH(3887),CH(4852),CH(3660),CH(4803),CH(4973),CH(16),
    CH(220),CH(1903),CH(2570),CH(2329),CH(2308),CH(4429),CH(4143),CH(16),
    CH(221),CH(1921),CH(5465),CH(5417),CH(4429),CH(2009),CH(4524),CH(16),
    CH(222),CH(3454),CH(2531),CH(4817),CH(4429),CH(4429),CH(4143),CH(16),
};/*
探伤数据处理
系统参数设置
仪器性能校验
常规方法探伤
车轴直探穿透
轮箍斜探探伤
*/

const char _InnerInfo[C_LANGUAGE][20][21] =
{
    {"⒈购买单位","⒉仪器编号","⒊入库日期","⒋出厂日期","⒌维修日期","⒍软件版本","⒎突变点","⒏通道数"},
    {"1.Corporation     :","2.Serial number   :","3.Purchase date   :","4.Personnel       :","5.Governor        :","6.Software edition:","7.Saltation     :","8.Channel max     :",}
};

const char _UserInfo[C_LANGUAGE][19][21] =
{
    {"⒈单位名称:","⒉仪器编号:","⒊购买日期:","⒋使用人员:","⒌主管人员:","⒍软件版本:","⒎内核版本:","⒏工艺版本:",},
    {"1.Corporation     :","2.Serial number   :","3.Purchase date   :","4.Personnel       :","5.Governor        :","6.Software edition:","7.Core edition   :","8.Process edition :",}
};


extern char _serial[];

extern unsigned int FixGain	;
unsigned short FixGainPoint1 ;
unsigned short FixGainPoint2 ;
extern const unsigned short FixGain1 ;
extern const unsigned short FixGain2 ;
int g_nDenoise, g_nAutoDenoise;
int g_nRepeat;
int g_nPulseWidth;

void MSetAmplifierGain(u_short gain)
{
    int nDenoise ;
    nDenoise = 0;
#if C_DEVLIB == 1 || C_DEVLIB == 2 || C_DEVLIB == 23 || C_DEVLIB == 24
//;	if( gain > 700 && gain < 730) gain = ( gain - 700) * 1.10 + 700;
//;	else if( gain >= 730 && gain < 770) gain = ( gain - 730) * 1.15 + 700 + 30*1.1;
//;	else if( gain >= 770 && gain < 800)gain = ( gain - 770)*1.12 + 700 + 40*1.15 + 30 * 1.1;
//;	else if( gain >= 800 ) gain =  (gain - 800 ) + 700 + 30 * 1.12 + 40*1.15 + 30 * 1.1 ;
#endif
#if C_DEVLIB == 1 || C_DEVLIB == 2 || C_DEVLIB == 23 || C_DEVLIB == 24 || C_DEVLIB == 3
    ;
    if(gain > 1100)gain = 1100 + ( gain - 1100)/4.1;
   // if(gain > 1180)gain = 1180;
#endif
    if(gain >= FixGainPoint1)gain += FixGain1;
    if(gain >= FixGainPoint2)gain += FixGain2;
    if (gain > MAX_GAIN_DB)gain = MAX_GAIN_DB;
//*
#if C_DEVLIB == 1 || C_DEVLIB == 2 || C_DEVLIB == 3 || C_DEVLIB == 23 || C_DEVLIB == 24

#if C_DEVLIB == 1 || C_DEVLIB == 2 || C_DEVLIB == 3
    if( gain > 1100)nDenoise = 3;
    else if( gain > 950)nDenoise = 2;
    else if( gain > 800)nDenoise = 1;
    else nDenoise = 0;
#elif C_DEVLIB == 23 || C_DEVLIB == 24
//	if( gain > 1100)nDenoise = 4;
//	else if( gain > 900)nDenoise = 3;
//	else if( gain > 800)nDenoise = 2;
//	else if( gain > 700)nDenoise = 1;
//	else nDenoise = 0;;
    if( gain > 1000)nDenoise = 0;
    else if( gain > 900)nDenoise = 0;
    else nDenoise = 0;
#endif
//	*/
    if( MGetDenoise() == 0 )
    {
//		if( MGetFunctionMode(C_GATE_ALARM) )
        if( g_nAutoDenoise != nDenoise )
        {
//;			g_nDenoise = nDenoise;
            g_nAutoDenoise = nDenoise;
            ;
            DeNoise(g_nAutoDenoise);
            SetRepeatRate( g_nAutoDenoise * 100+100);
            AdjustTime.amp = AdjustTime.amp2 = AdjustTime.amp_total = AdjustTime.amp_total2 = 0;
        }
//		else
        {
//			if( g_nDenoise != 0)DeNoise(0);
        }
    }
    else g_nAutoDenoise = 0;
//if( MGetFunctionMode(C_GATE_ALARM) )
//	Write_Number(160,0,g_nDenoise,4,0,0);
//else Write_Number(160,0,9,4,0,0);
#endif

///	MSetColor(C_CR_WAVE);//如果在调节增益时变动颜色，速度比较慢
#if C_DEVLIB == 4
    SetAmplifierGain(MGetChannel(),gain);
#else
    SetAmplifierGain(gain);
#endif
}
extern DAC			Dac;
extern CHANNEL_PARAMETER			ChannelPara;
extern const unsigned long int ColorsBG[];
extern const unsigned char background[];
int MainProject(void)	//主项目调用
{
    int keycode;
    int i;


    MSetColor(C_CR_WAVE);
    {

        MSetAcquisitionEnable(0,C_SETMODE_SETSAVE);
        MSetHardEchoShow(0,C_SETMODE_SETSAVE);

        SystemParaRestore();
        ChannelParaRestore();
        for (i=0; i<30; i++)
        {
            Notes.name[i]=0;
        }
//		MSetSaveMode(0,C_SETMODE_SETSAVE);
        MSetSaveStatus( 0,C_SETMODE_SETSAVE);
        MSetSavePosition(MGetSaveMode(),C_SETMODE_SETSAVE);

        MSetRange(MGetRange(1),C_SETMODE_SETSAVE/*mode = 0\1\2*/);	/*设置当前通道	*/
        MSetSystemMode(0,0);	//设置成一般状态

        if(MGetDelay(3)>=21000)
        {
            MSetDelay(130019,C_SETMODE_SETSAVE);
        }

        MFclearScreen();

        MSetSystem();
        MSetSystemGain();
        keycode = C_KEYCODMAX;

        ClearEnvelope();
        MSetAcquisitionEnable(1,C_SETMODE_SETSAVE);
        MSetHardEchoShow(1,C_SETMODE_SETSAVE);
        ScreenRenovate();	/*屏幕刷新*/
        DrawDac(0);
        MActivateDac(MGetFunctionMode(C_DEPTH_COMPENSATE) ) ;
     // MSetSystem();
      //MSetSystemGain();
        ExpendTime(2);
//		MSetHardEchoShow(1,C_SETMODE_SETSAVE);
//		MActivateDac(MGetFunctionMode(C_DEPTH_COMPENSATE) ) ;
     // MParaRenovate(0);
        i = 0;
        u_int elapsedtime = GetElapsedTime() + 200;
//: Added By WST
#if (C_WRITE_TO_UDISK==1)


        memset(&g_UDiskInfo,0,sizeof(UDISK_INFO));
#endif
//: End\

        //	SwitchUsbDevicePowerOff();
      /*  int ret;
        while(1)
        {
            ret=CharToKeypad(0x01) ;
            Write_Number(10,100,ret,5,0,0);
            ExpendTime(200);
            ret=CharToKeypad(0x02) ;
            Write_Number(10,200,ret,5,0,0);
            ExpendTime(200);
            //ret=CharToKeypad(0x30) ;
          //  Write_Number(10,300,ret,5,0,0);
            ExpendTime(200);
        }*/
       // CharToKeypad(8) ;
       int SDkey[]={C_KEYCOD_SAVE,C_KEYCOD_SAVE};
       int count=0;
       int xpos=0,ypos=100;
       TIME_CLOCK TimeClock;
        while(1)
        {
/*                UGetSystemTime(&TimeClock) ;
    WriteDate(xpos+50, ypos, &TimeClock);
    WriteTime(xpos+50+11*C_ECHAR_HDOT,ypos, &TimeClock);//显示时间

int a;

BatteryPackRead(0x0B0D,2,&a) ;
Write_Number(10,100,a,6,0,0);
Write_Number(10,120,GetPowerStatus(),6,0,0);
*/
/*
Write_Number(10,120,C_OFF_FILEMAX,10,0,0);
Write_Number(10,140,C_OFF_FAT,10,0,0);
Write_Number(10,160,C_OFF_CHANNEL,10,0,0);
Write_Number(10,180,C_OFF_BACKUP,10,0,0);
*/
 /*   char out[64];
    int rev;
            rev=UsbDevDataIn(out,1);
            Write_Number(50,100,rev,5,0,0);
            for(i=0;i<10;i++)
            {
                Write_Number(10,10+i*16,out[i],5,0,0);
            }*/

//Write_Number(10,120,GetScanRotaryValue(0),6,0,0);
//Write_Number(10,140,GetScanRotaryValue(1),6,0,0);

          //  HoriOffsetScreen = 0;   //屏幕偏移
          //  VertOffsetScreen = 2;
/*u_char*   sampbuff;
u_char   echobuffer[C_COORWIDTH];
    sampbuff = GetSampleBuffer();

    for( i = 0; i< C_LEN_SAMP; i++)	echobuffer[i] = sampbuff[i]-28;
    DrawWave(echobuffer, 0, C_LEN_SAMP);		//画波形*/
//Write_Number(10,140,MGetDenoise(),6,0,0);
            if(GetElapsedTime()>elapsedtime)
            {
                count++;
                count=count%2;
                SwitchAlarmOn(count);
                elapsedtime = GetElapsedTime() + 200;
            }


            if( keycode < 0 || keycode >= C_KEYCODMAX)	keycode = MGetKeyCode(0);//SDkey[count];   //
     //       if(extend==1)keycode = C_KEYCOD_SAVE;
           // count=(count+1)%2;
            if(keycode != C_KEYCODMAX)
            {
                g_UDiskInfo.DataHeaderMark = 1;
            }
            keycode = KeyManage(keycode,1);
//: Added By WST
#if (C_WRITE_TO_UDISK==1)

		//	Write_Number(340,400,MGetSaveStatus(),2,0,0);
		//	Write_Number(10,100,g_UDiskInfo.DataHeaderMark,2,0,0);
            StorageData(keycode);
#endif
//: End
            //	if (GetElapsedTime() >= elapsedtime)
            {
                //		elapsedtime = GetElapsedTime() + 10;
                MParaRenovate(1);
            }

            //		Write_Number(10,100,MGetSystemMode(),10,0,0);
        }
    }
}

void SystemLog(void )
{
    const char _MainInfo[C_LANGUAGE][3][25] =
    {
        {"自检完成  ","请按任意键进入","８检定模式  ９初始化"},
        {"Press any key to start"," ","9.Initialization    "}
    };
    TIME_CLOCK TimeClock;
    int offset = C_OFF_BATTERY;
    int deci_len = 0;

    u_int xpos,ypos,para_xpos,para_ypos;
    u_int char_len,row_number,row_height,col_max,align;	/*字符数和字符行数*/
    int i,j;
//	u_char* epText;

    u_int retvalue;

 //   MAnyKeyReturn();
 //   MKeyRlx();
 //   MFclearScreen();

    extend = 1;

/*  int key;

    char buf[50];
    while(1)
    {
        key=GetKeyCode();
        if(key==0)
        {
            ExpendTime(100);
            Write_Number(10,50,key,5,0,1);
        }
        else
        {
            memset(buf,0,50);
            sprintf(buf,"%08x",key);
            ETextOut(10,60,buf,strlen(buf));
            //Write_Number(10,60,key-32768,10,0,1);
        }

    }*/
    //开机界面
    xpos = 120;
    ypos = 20;
    col_max = 28;
    row_number = 9;
    row_height = 48;//32;
    align = 2;//居中

   // DisplayBMP((u_char *)background,ColorsBG, 0, 100,281,640);
    MSetColor(C_CR_PARA);
    SetBackgroundColor(COLOR_BLACK);
    MSetDisplayColor(COLOR_YELLOW);
    MEraseWindow(120+32, 340+24*2,120+32+246, 340+24*2+30);
    TextOut(120+32, 340+24*2-6,  1, 8+14*MGetLanguage(),32, (u_char*)_MainInfo[MGetLanguage()][0], 4); //MGetLanguage()
    if(MGetLanguage()==0)
    {
        TextOut(120+32+5*24, 340+24*2-6,  1, 14,32, (u_char*)_MainInfo[MGetLanguage()][1], 4); //MGetLanguage()
    }

    TextOut(120+32+24,340+24*3,  1, 20,32, (u_char*)_MainInfo[MGetLanguage()][2], 4); //MGetLanguage()

 /*   if (MGetLanguage())
    {
        MFclearScreen();
        EMenuOut(0,ypos+row_height*1,_LOG_B1,38,1,row_height);
        EMenuOut(0,ypos+row_height*2,_LOG_B2,38,1,row_height);

        EMenuOut(0,ypos+row_height*7,_LOG_B9,38,1,row_height);

        //	GetSystemTime(&TimeClock) ;
        //	ypos = ypos+row_height*4+4;
        //	WriteDate(xpos+50, ypos, &TimeClock);
        //	WriteTime(xpos+50+11*C_ECHAR_HDOT,ypos, &TimeClock);//显示时间
    }
    else
    {
        CETextOut2(xpos,ypos,_SYSMENU_B1,col_max,row_number,row_height,1,align);
        //开始(xpos,ypos)，每行最多字符数col_max(英文),行数row_number,行高row_height，考虑编号serial=0不1有,对齐方式align＝0左1右2中间3两端
    }
*/


  //  MSetSystemTime(&TimeClock);
    UGetSystemTime(&TimeClock);
    ExpendTime(5);
    SetSystemTime(&TimeClock);
    ExpendTime(5);
   // GetSystemTime(&TimeClock) ;
    ypos = 70;//ypos+row_height*4+4;
    WriteDate(xpos+50, ypos, &TimeClock);
    WriteTime(xpos+50+11*C_ECHAR_HDOT,ypos, &TimeClock);//显示时间
    offset = C_OFF_BATTERY + 10;
    MCopyProtectedMemory( (void*)offset, &TimeClock, 8, PM_COPY_WRITE);
//    CopyMemoryToSystem((void*)offset,8, PM_COPY_WRITE);


    xpos = 60 + 10*C_ECHAR_HDOT;
    ypos = C_COORVPOSI + C_COORHEIGHT - 8;
    EMenuOut( xpos, ypos, "SN:",3,1,12);
    xpos += C_ECHAR_HDOT*3;
    INNER_INFO InnerInfo;
    offset = C_OFF_INNER_INFO;
    MCopyProtectedMemory(&InnerInfo, (void*)offset, sizeof(INNER_INFO), PM_COPY_READ);
   // if( InnerInfo.pzSerial[0] >= 20 )InnerInfo.pzSerial[0] = 0;
    //CETextOut2(xpos,ypos-8,InnerInfo.pzSerial+1,InnerInfo.pzSerial[0],1,24,0,0);	//显示编号
   // ETextOut(xpos+C_ECHAR_HDOT,ypos+8,_serial,8);	//显示编号
    for( i = 0; i < 8; i++)_serial[i] = InnerInfo.pzSerial[i];
    EMenuOut( xpos+C_ECHAR_HDOT,ypos, _serial,8,1,24);

    MSetAcquisition(0);
    int keycode;
    u_int elapsedtime = GetElapsedTime() + 1000 * 10;
    u_int SameKeyTime;
    int tempi=0;
    while( GetElapsedTime() < elapsedtime && !bCOM2PC) /*5sec 后或有按键退出*/
    {
      /*
u_int 	UsbDevDataOut(char* pDataBuf, u_int length) ;   //返回值为长度，为0不成功
u_int 	UsbDevDataIn(char* pDataBuf, u_int length) ;*/

        if(tempi<360 )
        {
            for(i=0;i<16;i++)
            {
                tempi+=4;
                MDrawPixel(136+72+tempi, 380,DP_SET);
            }

        }

        for( SameKeyTime = 0; SameKeyTime < 100; SameKeyTime++)
        {
            ExpendTime(5);
            keycode = MGetKey2();
            if( keycode != C_KEYCODMAX)	break;
        }
        if( keycode == C_KEYCODMAX)	continue;

        //else MSetScreenBrightness(2,C_SETMODE_SETSAVE);

        SameKeyTime = TestSameKeyTime(keycode,GetElapsedTime(),C_SAMEKEYTIME);
        /*测试按键keycode已按时长，MaxTime为最长时间,此时间到或有不同按键退出,返回值为同一按键持续时长*/

      //  MFclearScreen();

        xpos = 30;
        ypos = 50;
        char_len = 6;
        row_number = 6;
        row_height = 48;//32;

        elapsedtime = GetElapsedTime() + 1000 * 1;
        j = keycode;
  /*      while( GetElapsedTime() < elapsedtime )
        {
            i = MGetKeyCode(0);

            if( i == C_KEYCODMAX)continue;
            if( i != j )
            {
                j = i;
               // Write_Number(0,0,GetKeyCode()&0xff-0x80,4,0,0);
               // Write_Number(0,10,i,4,0,0);
                MKeyRlx();
                elapsedtime = GetElapsedTime() + 1000 * 5;
            }
        }
*/
        if( keycode == 1 && SameKeyTime >= C_SAMEKEYTIME)
        {
        MFclearScreen();
            //长按1键，显示生产信息

            int number,deci_len,Fixgain;
            number = 0;
            deci_len = 0;
            EMenuOut( 0,0,"Pwd:",4,1,8);
            if( Input_Number(C_ECHAR_HDOT*4,0,&number,8,&deci_len,0) < 0)
            {
                number = C_DEFPWD - 1;
            }
            //	CMenuOut(xpos,ypos,_InnerInfo,char_len,row_number,row_height);	/*在指定位置根据每行字符数、行数、行高写菜单*/
            for (i=0; i<row_number+2; i++)
            {
                TextOut(xpos, ypos+row_height*i-C_ECHAR_VDOT*MGetLanguage(),  1, 19,24, (u_char*)_InnerInfo[MGetLanguage()][i], 4);
            }
            para_xpos = xpos + (char_len+1) * C_CCHAR_HDOT;	/*写参数，位置水平方向右移*/
            para_ypos = ypos ;
            MSetColor( C_CR_PARA);


                if(InnerInfo.StorageTime.month>0x12)
                {
                    InnerInfo.StorageTime.yearh = 0x20;
                    InnerInfo.StorageTime.yearl = 0x11;
                    InnerInfo.StorageTime.month = 0x11;
                    InnerInfo.StorageTime.date = 1;
                }
                if(InnerInfo.LeaveFactoryTime.month>0x12)
                {
                    InnerInfo.LeaveFactoryTime.yearh = 0x20;
                    InnerInfo.LeaveFactoryTime.yearl = 0x11;
                    InnerInfo.LeaveFactoryTime.month = 0x11;
                    InnerInfo.LeaveFactoryTime.date = 1;
                }
                if(InnerInfo.MaintainTime.month>0x12)
                {
                    InnerInfo.MaintainTime.yearh = 0x10;
                    InnerInfo.MaintainTime.yearl = 0x00;
                    InnerInfo.MaintainTime.month = 1;
                    InnerInfo.MaintainTime.date = 1;
                }

                if(InnerInfo.pzPurchaser[0]>22)
                {
                  InnerInfo.pzPurchaser[0]=0;
                }

            while(number == C_DEFPWD || number == C_SUPERPWD )
            {
                //购买单位	pzPurchaser[15];
                xpos = para_xpos;
                ypos = para_ypos + row_height *0;

              //  Write_Number(xpos, 0, InnerInfo.pzPurchaser[0],4,0,1);
                CETextOut2(xpos,ypos,InnerInfo.pzPurchaser+1,InnerInfo.pzPurchaser[0],1,24,0,0);	/*在指定位置根据每行字符数、行数、行高写菜单*/

                //仪器编号
                ypos = para_ypos + row_height *1;
                //;CETextOut2(xpos,ypos,InnerInfo.pzSerial+1,InnerInfo.pzSerial[0],1,24,0,0);
               // ETextOut(xpos+8,ypos,_serial,8);	//显示编号
                for( i = 0; i < 8; i++)_serial[i] = InnerInfo.pzSerial[i];
                EMenuOut( xpos-C_ECHAR_HDOT,ypos, _serial,8,1,24);



                //入库日期StorageTime;
                ypos = para_ypos + row_height *2;
                WriteDate(xpos, ypos, &InnerInfo.StorageTime);


                //出厂日期10\8	LeaveFactoryTime;
                ypos = para_ypos + row_height *3;
                WriteDate(xpos, ypos, &InnerInfo.LeaveFactoryTime);

                //检修出 厂日期10\8	MaintainTime;
                ypos = para_ypos + row_height *4;
                WriteDate(xpos, ypos, &InnerInfo.MaintainTime);

                ypos = para_ypos + row_height *5;
                EMenuOut(xpos, ypos, _version1,8,1,12);
                EMenuOut(xpos+ 10*C_ECHAR_HDOT, ypos, _version,18,1,12);

                ypos = para_ypos + row_height *6;
                Write_Number(xpos, ypos, InnerInfo.FixGain,4,0,1);

                ypos = para_ypos + row_height *7;
                if(InnerInfo.Channel<10)InnerInfo.Channel=10;
                Write_Number(xpos, ypos, InnerInfo.Channel,4,0,0);

                keycode = MenuKeyReturn(8,3);	/*出现菜单时，按不大于keymax的键返回，mode=0其它键无效，=1确认键有效，=2返回键有效，=3确认返回键有效*/
                if (keycode == C_KEYCOD_RETURN)
                {
                    MCopyProtectedMemory(&InnerInfo, (void*)offset, sizeof(INNER_INFO), PM_COPY_READ);
                    retvalue = C_FALSE;
                    break;
                }
                if (keycode == C_KEYCOD_CONFIRM)
                {
                    MSetSystem();
                    MCopyProtectedMemory( offset, &InnerInfo, sizeof(INNER_INFO), PM_COPY_WRITE);
                    offset = C_OFF_INIT ;
                    CopyMemoryToSystem((void*)offset,C_OFF_USER_INFO-offset,PM_COPY_WRITE);
//                    CopyMemoryToSystem((void*)offset,sizeof(INNER_INFO), PM_COPY_WRITE);
                    retvalue = C_TRUE;
                    break;
                }

                ypos = para_ypos + row_height * (keycode - 1);

                MSetInputMode(C_INPUT_PINYIN,0);
                switch (keycode)
                {
                case 1:		//
                    Write_Space(xpos ,ypos,14,1);
                    MInputChar(xpos, ypos,1,InnerInfo.pzPurchaser,20,10);
                    break;
                case 2:		//
                    Write_Space(xpos ,ypos+C_ECHAR_VDOT,19,0);
                    MSetInputMode(C_INPUT_LOWERCASE,0);	//设置成小写
                    MInputChar(xpos, ypos,0,InnerInfo.pzSerial,19,19);
                    break;
                case 3:
                    InputDate(xpos,ypos, &InnerInfo.StorageTime);
                    break;
                case 4:
                    InputDate(xpos,ypos, &InnerInfo.LeaveFactoryTime);
                    break;
                case 5:
                    InputDate(xpos,ypos, &InnerInfo.MaintainTime);
                    break;
                case 6:
                    InputTime(xpos,ypos, &TimeClock);
                    WriteTime(xpos,ypos+8, &TimeClock);
                    break;
                case 7: //突变点
                    Fixgain=0;
                    Input_Number(xpos,ypos,&Fixgain,3,&deci_len,1);
                    InnerInfo.FixGain = Fixgain;
                    break;
                case 8: //通道数
                    while(1)
                    {
                        Fixgain=0;
                        Input_Number(xpos,ypos,&Fixgain,3,&deci_len,1);
                        if(Fixgain>9 && Fixgain<61 )break;

                    }
                    InnerInfo.Channel = Fixgain;
                    break;
                }
            }
        }
        else if(keycode == 2 && SameKeyTime >= C_SAMEKEYTIME)
        {
            MFclearScreen();
            SysPwd(1);
        }
        else if( keycode == 3 && SameKeyTime >= C_SAMEKEYTIME)
        {
            //
            MFclearScreen();
            SysPwd(0);
            int key;
            //*
#if C_DEVLIB == 2 || C_DEVLIB == 3 || C_DEVLIB == 23
            int x,y,len;
            int color,red = 0,green = 0,blue = 0;//red < 8,green < 8,blue<4
            u_int data;
            //set color for screen operation, (b0-b2): red ; (b3-b5): green ; (b6-b7): blue
            x = 8;
            y = 32;
            len = 8;
            ETextOut(10,10,"PRESS #!   ",8);
            while(true)
            {
                key = MAnyKeyReturn();
                switch(key)
                {
                case 1:
                    red++;
                    break;
                case 2:
                    red--;
                    break;
                case 3:
                    green++;
                    break;
                case 4:
                    green--;
                    break;
                case 6:
                    blue++;
                    break;
                case 7:
                    blue--;
                    break;
                case 5:
                    red = green = blue = 0;
                    break;
                case 8:
                    len += 2;
                    break;
                case 9:
                    len -= 2;
                    break;
                }
                if( key == C_KEYCOD_RETURN)break;
                len = len % 32;
                red &= 0x7;
                green &= 0x7;
                blue &= 0x3;
                color = ( red + ( green << 3) + (blue << 6) );
                MEraseWindow(x-4,y-4,x+len+8,y+4);
                //SetDisplayColor(color);
                Write_Number(100,100,red,4,0,0);
                Write_Number(100,110,green,4,0,0);
                Write_Number(100,120,blue,4,0,0);
                Write_Number(100,130,len,4,0,0);
                MDrawLine(x,y-2,x+len,y-2,C_CR_BACK);
                MDrawLine(x,y-1,x+len,y-1,C_CR_BACK);
                MDrawLine(x,y,x+len,y,C_CR_BACK);
                MDrawLine(x,y+1,x+len,y+1,C_CR_BACK);

                for( i = 0; i < 8; i++)
                {
                    data = MGetPixels( x, y-4+i, len);
                    Write_Number( x,60+i*10,data,10,0,0);
                }
            }

            MFclearScreen();
            ETextOut(10,10,"Test LCD Color! Keyin for Test,# next.",39);

            while(true)
            {
                key = MAnyKeyReturn();
                if( key == C_KEYCOD_RETURN)break;
                if(key == 0 || key > C_CR_NUMBER)continue;
                MFclearScreen();
                SetDisplayColor( c_crPara[key-1] );
                for(i = 0; i < C_VERTDOT_SCREEN; i++)DrawLine(0,i,C_HORIDOT_VIDEO-1,i);
            }
            MFclearScreen();
            ETextOut(10,10,"Test RGB Color! Please wait!",28);
            key = 0;

            while(true)
            {
                {
                    int r,g,b;
                    key = MAnyKeyReturn();
                    if( key == C_KEYCOD_RETURN)break;
                    b = 0;
                    if( key >= 0 && key < 4)b = key;
                    for( r = 0; r < 8; r++)
                    {
                        for( g = 0; g < 8; g++)
                        {
                            SetDisplayColor( (b << 6)+(g<<3)+r );
                            for( key = 0; key < 30; key++)DrawLine( r*40, g*30+key, r*40+40, g*30+key);
                            SetDisplayColor(COLOR_WHITE);
                            Write_Number(r*40+1, g*30+1,( (b << 6)+(g<<3)+r ),4,0,0);
                        }
                    }
                }

            }
            //*/

#endif
#if C_DEVLIB == 23
            int nPulseWidth = 0;

            SystemParaRestore();
            ChannelParaRestore();
            MSetSystemMode(0,0);	//设置成一般状态
            MSetBaseGain(400,C_SETMODE_SETSAVE);

            MSetSystem();
            MSetAcquisition(1);

            ScreenRenovate();	/*屏幕刷新*/

            nPulseWidth = 33;	//33
            ETextOut(10,10,"PulseWidth:  ",11);
            Write_Number(10+C_ECHAR_HDOT*11,10,nPulseWidth,4,0,0);
            ExpendTime(50);
            MKeyRlx();
            while(true)
            {

                MParaRenovate(1);
                key = MGetKeyCode(0);
                if( key == C_KEYCODMAX)continue;
                if( key == C_KEYCOD_RETURN)break;
                if( key == C_KEYCOD_PLUS || key == C_KEYCOD_MINUS)
                {
                    if( key == C_KEYCOD_PLUS )nPulseWidth++;
                    else if( key == C_KEYCOD_MINUS )nPulseWidth--;
                    SetPulseWidth(nPulseWidth);
                    if( nPulseWidth >= 127)nPulseWidth = 0;
                    else if( nPulseWidth < 0) nPulseWidth = 127;
                    ETextOut(10,0,"                  ",16);
                    ETextOut(10,10,"PulseWidth:       ",16);
                    Write_Number(10+8*11,10,nPulseWidth,4,0,0);
                    ExpendTime(20);
                    continue;
                }

                if( key == C_KEYCOD_BASEGAIN || key == C_KEYCOD_GATE || key == C_KEYCOD_RANGE)
                {
                    KeyManage(key,1);
                    ExpendTime(20);
                }
                MKeyRlx();
            }
#endif
        }
      /*  else if( keycode == 3 && SameKeyTime >= C_SAMEKEYTIME)
        {
            unsigned int pretime, currtime;
            unsigned int offset;
            unsigned int prekeycode, keycode ;
            short i,key[4],prekey[4];
//			short rot[4];
            unsigned short timespace = 100;

        MFclearScreen();
            while(true)
            {
                keycode = MGetKeyCode(0);
                if( keycode <= 9)timespace = 100 * keycode;	//两次读数之间的时间间隔
                currtime = GetElapsedTime();
                offset = C_OFF_STORE + 50+C_SIZE_INT;//			前次取旋钮值时时间
                MCopyProtectedMemory( &pretime , (void*)offset, C_SIZE_INT, PM_COPY_READ);
                Write_Number(10,0,timespace,4,0,0);
                if( currtime - pretime < timespace)continue;
#if( C_ROTARY > 0)
                keycode = GetRotaryValue();
#else
                keycode = 0;
#endif

                offset = C_OFF_STORE + 50;//			前次旋钮值
                MCopyProtectedMemory( &prekeycode , (void*)offset, C_SIZE_INT, PM_COPY_READ);
                MCopyProtectedMemory( (void*)offset, &keycode, C_SIZE_INT, PM_COPY_WRITE);


                for(i = 0; i < 4; i++)
                {
                    key[i] = keycode & 0xff;
                    keycode = keycode >> 8;
                    prekey[i] = prekeycode & 0xff;
                    prekeycode = prekeycode >> 8;
                    Write_Number(10,30+10*i,i,1,0,0);
                    if( key[i] > 0)Write_Number( 30,30+10*i,key[i],4,0,0);
                    else EMenuOut(30,30+10*i,"    ",4,1,8);
                    if( prekey[i] > 0)Write_Number( 100,30+10*i,prekey[i],4,0,0);
                    else EMenuOut(100,30+10*i,"    ",4,1,8);
                }

                keycode = ( key[0] + key[3] ) - ( key[1] + key[2] );
                prekeycode = ( prekey[0] + prekey[3] ) - ( prekey[1] + prekey[2] );

                Write_Number( 10,30+10*4,keycode,6,0,-1);
                Write_Number( 100,30+10*4,prekeycode,6,0,-1);
                //如果短时间内本次旋钮与前次相反，则判无效
                if( currtime - pretime < timespace *2 && keycode * prekeycode < 0)keycode = 0;
                offset = C_OFF_STORE + 50+C_SIZE_INT;//			前次取旋钮值时时间
                //if( keycode != 0)
                MCopyProtectedMemory( (void*)offset, &currtime, C_SIZE_INT, PM_COPY_WRITE);
                Write_Number( 50,30+10*4,keycode,6,0,-1);
            }
        }*/
        else if( keycode == 4 && SameKeyTime >= C_SAMEKEYTIME)
        {
        MFclearScreen();
            int grade = 0;
            EMenuOut(0,0,"C_OFF_FILE:",11,1,12);
            Write_Number(C_ECHAR_HDOT*12,00,C_OFF_FILE,5,0,0);
            EMenuOut(296,0,"C_OFF_FILEMAX:",11,1,12);
            Write_Number(296+C_ECHAR_HDOT*12,00,C_OFF_FILEMAX,10,0,0);
            EMenuOut(0,24,"C_LEN_FAT:",10,1,12);
            Write_Number(C_ECHAR_HDOT*12,24,C_LEN_FAT,5,0,0);
            EMenuOut(0,24*2,"C_LEN_FILE:",11,1,12);
            Write_Number(C_ECHAR_HDOT*12,24*2,C_LEN_FILE,5,0,0);
            EMenuOut(0,24*3,"FIX_GAIN:",9,1,12);
            Write_Number(C_ECHAR_HDOT*12,24*3,FixGain,5,0,0);
            EMenuOut(0,24*4,"FIXGAIN1:",9,1,12);
            Write_Number(C_ECHAR_HDOT*12,24*4,FixGain1-13,5,0,-1);
            EMenuOut(0,24*5,"FIXPOINT1:",10,1,12);
            Write_Number(C_ECHAR_HDOT*12,24*5,FixGainPoint1,5,0,0);
            EMenuOut(0,24*6,"BASELINE :",10,1,12);
            Write_Number(C_ECHAR_HDOT*12,24*6,c_FWBaseLine,5,0,0);
            Write_Number(C_ECHAR_HDOT*20,24*6,c_RFBaseLine,5,0,0);
            EMenuOut(0,24*7,"DAC:      ",10,1,12);
            Write_Number(C_ECHAR_HDOT*12,24*7,C_LEN_DAC,5,0,0);
            EMenuOut(0,24*8,"gatepeak: ",10,1,12);
            Write_Number(C_ECHAR_HDOT*12,24*8,C_LEN_PEAK,5,0,0);
            EMenuOut(0,24*9,"syspara:  ",10,1,12);
            Write_Number(C_ECHAR_HDOT*12,24*9,C_LEN_SYSPARA,5,0,0);
            EMenuOut(0,24*10,"WELD:     ",10,1,12);
            Write_Number(C_ECHAR_HDOT*12,24*10,C_LEN_WELD,5,0,0);
            EMenuOut(0,24*11,"CHSTAT:   ",10,1,12);
            Write_Number(C_ECHAR_HDOT*12,24*11,C_LEN_CHSTATUS,5,0,0);
            EMenuOut(0,24*12,"CHPARA:   ",10,1,12);
            Write_Number(C_ECHAR_HDOT*12,24*12,C_LEN_CHPARA,5,0,0);
            EMenuOut(0,24*13,"GATEPARA: ",10,1,12);
            Write_Number(C_ECHAR_HDOT*12,24*13,C_LEN_GATEPARA,5,0,0);

            grade = 0;
            MAnyKeyReturn();

//1#define C_OFF_INIT
            EMenuOut(0,24*grade,"C_OFF_INIT:",11,1,12);
            Write_Number(C_ECHAR_HDOT*12,24*grade,C_OFF_INIT,5,0,0);
            grade++;
//2#define C_OFF_PWD
            EMenuOut(0,24*grade,"C_OFF_PWD: ",11,1,12);
            Write_Number(C_ECHAR_HDOT*12,24*grade,C_OFF_PWD,5,0,0);
            grade++;
//3#define C_OFF_STORE
            EMenuOut(0,24*grade,"_OFF_STORE:",11,1,12);
            Write_Number(C_ECHAR_HDOT*12,24*grade,C_OFF_STORE,5,0,0);
            grade++;
//4#define C_OFF_TEMP
            EMenuOut(0,24*grade,"C_OFF_TEMP:",11,1,12);
            Write_Number(C_ECHAR_HDOT*12,24*grade,C_OFF_TEMP,5,0,0);
            grade++;
//5#define C_OFF_STACK
            EMenuOut(0,24*grade,"_OFF_STACK:",11,1,12);
            Write_Number(C_ECHAR_HDOT*12,24*grade,C_OFF_STACK,5,0,0);
            grade++;
//6#define C_OFF_INNER_INFO/
            EMenuOut(0,24*grade,"INNER_INFO:",11,1,12);
            Write_Number(C_ECHAR_HDOT*12,24*grade,C_OFF_INNER_INFO,5,0,0);
            grade++;
//7#define C_OFF_REPORT_INFO
            EMenuOut(0,24*grade,"REPORTINFO:",11,1,12);
            Write_Number(C_ECHAR_HDOT*12,24*grade,C_OFF_REPORT_INFO,5,0,0);
            grade++;
//8#define C_OFF_USER_INFO
            EMenuOut(0,24*grade,"_USER_INFO:",11,1,12);
            Write_Number(C_ECHAR_HDOT*12,24*grade,C_OFF_USER_INFO,5,0,0);
            grade++;
//9#define C_OFF_BATTERY
            EMenuOut(0,24*grade,"OFF_BATTERY:",11,1,12);
            Write_Number(C_ECHAR_HDOT*12,24*grade,C_OFF_BATTERY,5,0,0);
            grade++;
//10#define C_OFF_USER
            EMenuOut(0,24*grade,"C_OFF_USER: ",11,1,12);
            Write_Number(C_ECHAR_HDOT*12,24*grade,C_OFF_USER,5,0,0);
            grade++;
//11#define C_OFF_USERPARA
            EMenuOut(0,24*grade,"OFFUSERPARA:",11,1,12);
            Write_Number(C_ECHAR_HDOT*12,24*grade,C_OFF_USERPARA,5,0,0);
            grade++;
//12#define C_OFF_DAC
            EMenuOut(0,24*grade,"C_OFF_DAC:  ",11,1,12);
            Write_Number(C_ECHAR_HDOT*12,24*grade,C_OFF_DAC,5,0,0);
            grade++;
//13#define C_OFF_CURVE
            EMenuOut(0,24*grade,"C_OFF_CURVE:",11,1,12);
            Write_Number(C_ECHAR_HDOT*12,24*grade,C_OFF_CURVE,5,0,0);
            grade++;
//14#define C_OFF_DACDB
            EMenuOut(0,24*grade,"C_OFF_DACDB:",11,1,12);
            Write_Number(C_ECHAR_HDOT*12,24*grade,C_OFF_DACDB,5,0,0);
            grade++;
//15#define C_OFF_DACGATE
            EMenuOut(0,24*grade,"OFF_DACGATE:",11,1,12);
            Write_Number(C_ECHAR_HDOT*12,24*grade,C_OFF_DACGATE,5,0,0);
            grade++;
//16#define C_OFF_POWER
            EMenuOut(0,24*grade,"C_OFF_POWER:",11,1,12);
            Write_Number(C_ECHAR_HDOT*12,24*grade,C_OFF_POWER,5,0,0);
            grade++;
//17#define C_OFF_SYSTEM
            EMenuOut(0,24*grade,"_OFF_SYSTEM:",11,1,12);
            Write_Number(C_ECHAR_HDOT*12,24*grade,C_OFF_SYSTEM,5,0,0);
            grade++;
//18#define C_OFF_CHANNEL
            EMenuOut(0,24*grade,"OFF_CHANNEL:",11,1,12);
            Write_Number(C_ECHAR_HDOT*12,24*grade,C_OFF_CHANNEL,5,0,0);
            grade++;
//19#define C_OFF_BACKUP
            EMenuOut(0,24*grade,"_OFF_BACKUP:",11,1,12);
            Write_Number(C_ECHAR_HDOT*12,24*grade,C_OFF_BACKUP,5,0,0);
            grade=0;
//20#define C_OFF_FILEINFO
            EMenuOut(296,24*grade,"_FILEINFO:",11,1,12);
            Write_Number(296+C_ECHAR_HDOT*12,24*grade,C_OFF_FILEINFO,5,0,0);
            grade++;
//21#define C_OFF_PEAK
            EMenuOut(296,24*grade,"COFF_PEAK:",11,1,12);
            Write_Number(296+C_ECHAR_HDOT*12,24*grade,C_OFF_PEAK,5,0,0);
            grade++;
//22#define C_OFF_SAMP
            EMenuOut(296,24*grade,"C_OFF_SAMP:",11,1,12);
            Write_Number(296+C_ECHAR_HDOT*12,24*grade,C_OFF_SAMP,5,0,0);
            grade++;
//23#define C_OFF_FREQ
            EMenuOut(296,24*grade,"C_OFF_FREQ:",11,1,12);
            Write_Number(296+C_ECHAR_HDOT*12,24*grade,C_OFF_FREQ,5,0,0);
            grade++;
//24#define C_OFF_KEYFUNC
            EMenuOut(296,24*grade,"FF_KEYFUNC:",11,1,12);
            Write_Number(296+C_ECHAR_HDOT*12,24*grade,C_OFF_KEYFUNC,5,0,0);
            grade++;
//25#define C_OFF_FAT
            EMenuOut(296,24*grade,"C_OFF_FAT: ",11,1,12);
            Write_Number(296+C_ECHAR_HDOT*12,24*grade,C_OFF_FAT,5,0,0);
            grade++;
//26#define C_OFF_FILE
            EMenuOut(296,24*grade,"C_OFF_FILE:",11,1,12);
            Write_Number(296+C_ECHAR_HDOT*12,24*grade,C_OFF_FILE,6,0,0);
            grade++;
//27#define C_OFF_OTHER
            EMenuOut(296,24*grade,"COFF_OTHER:",11,1,12);
            Write_Number(296+C_ECHAR_HDOT*12,24*grade,C_OFF_OTHER,10,0,0);
            grade++;
//28#define C_OFF_FILEMAX
            EMenuOut(296,24*grade,"   FILEMAX:",11,1,12);
            Write_Number(296+C_ECHAR_HDOT*12,24*grade,C_OFF_FILEMAX,10,0,0);
            grade++;
//28#define C_OFF_CACHE
            EMenuOut(296,24*grade,"_OFF_CACHE:",11,1,12);
            Write_Number(296+C_ECHAR_HDOT*12,24*grade,C_OFF_CACHE,10,0,0);
            grade=0;

            MAnyKeyReturn();

            SystemParaRestore();
            ChannelParaRestore();
            MSetSystemMode(0,0);	//设置成一般状态
            MSetSystem();
            MSetAcquisition(1);

            ScreenRenovate();	/*屏幕刷新*/
            DrawDac(0);

            MParaRenovate(0);
            EMenuOut(10,30,"LCD",3,1,12);
            grade = 0;
            while(GetElapsedTime() < 1000*60*5)
            {
                keycode = MGetKeyCode(0);

                if( keycode == C_KEYCOD_RETURN )break;
#if C_DEVLIB == 3 || C_DEVLIB == 23 || C_DEVLIB == 24
                if( keycode == 4)grade++;
                else if( keycode == 1)grade--;
                else if( keycode == 0)grade = 0;
                else continue;
                if( grade < 0)grade = 31;
                if( grade > 31)grade = 0;
                ExpendTime(20);
#if C_DEVLIB == 24
                SetELBrightness(grade) ;
#else
                SetLCDBrightness(grade) ;
#endif
                Write_Number(10,40,grade,4,0,0);
#endif
            }
            EMenuOut(10,30,"VOL",3,1,12);
            MKeyRlx();
            ExpendTime(50);
            grade = 0;
            while(1)
            {
                keycode = MGetKeyCode(0);

                if( keycode == C_KEYCOD_RETURN )break;
#if C_DEVLIB == 3
                if( keycode == 4)grade++;
                else if( keycode == 1)grade--;
                else if( keycode == 0)grade = 0;
                else continue;
                if( grade < 0)grade = 0;
                if( grade > 31)grade = 31;
                ExpendTime(20);
                SetTransmitterVoltage(grade) ;
                Write_Number(10,40,grade,4,0,0);
#endif
            }
        }
        else if( keycode == 5 && SameKeyTime >= C_SAMEKEYTIME)
        {
            unsigned short nBaseLine, nFwline,nRfline;
            SystemParaRestore();
            ChannelParaRestore();
            MSetSystemMode(0,0);	//设置成一般状态
            MSetBaseGain(400,C_SETMODE_SETSAVE);

            MSetSystem();
            MSetAcquisition(1);

            ScreenRenovate();	/*屏幕刷新*/
            DrawDac(0);

            MParaRenovate(0);
            nFwline = c_FWBaseLine;
            nRfline = c_RFBaseLine;
            while(true)
            {
                ETextOut(0,0,"Full Wave BaseLine    ",20);
                ETextOut(0,24,"BaseLine    ",10);
                nBaseLine = nFwline;
                Write_Number(100,10, nBaseLine,4,0,0);
                SetEchoMode(FULL_WAVE);
                SetEchoBaseLine( c_FWBaseLine);
                while(true)
                {
                    keycode = MGetKeyCode(0);
                    if( keycode == C_KEYCODMAX)	continue;
                    if( keycode == C_KEYCOD_PLUS)nBaseLine++;
                    else if( keycode == C_KEYCOD_MINUS)nBaseLine--;
                    else if( keycode == C_KEYCOD_BASEGAIN)GainChange();
                    else if(keycode == C_KEYCOD_CONFIRM)Input_Number(100,10,nBaseLine,4,0,0);
                    if( nBaseLine > 4095)nBaseLine = 0;
                    else if( nBaseLine < 1)nBaseLine = 4095;
                    SetEchoBaseLine(nBaseLine) ;
                    Write_Number(100,10, nBaseLine,4,0,0);
                    ExpendTime(10);
                    if( keycode == C_KEYCOD_RETURN)break;
                }

                ExpendTime(100);
                MKeyRlx();
                ETextOut(0,0,"RF Wave BaseLine       ",20);
                nBaseLine = nRfline;
                Write_Number(100,10, nBaseLine,4,0,0);
                SetEchoMode(RF_WAVE);
                SetEchoBaseLine( c_RFBaseLine);
                while(true)
                {
                    keycode = MGetKeyCode(0);
                    if( keycode == C_KEYCODMAX)	continue;
                    if( keycode == C_KEYCOD_PLUS)nBaseLine++;
                    else if( keycode == C_KEYCOD_MINUS)nBaseLine--;
                    else if( keycode == C_KEYCOD_BASEGAIN)GainChange();
                    else if(keycode == C_KEYCOD_CONFIRM)Input_Number(100,10,nBaseLine,4,0,0);
                    if( nBaseLine > 4095)nBaseLine = 4095;
                    else if( nBaseLine < 1)nBaseLine = 0;
                    SetEchoBaseLine(nBaseLine) ;
                    Write_Number(100,10, nBaseLine,4,0,0);
                    ExpendTime(10);
                    if( keycode == C_KEYCOD_RETURN)break;
                }
            }
        }
        else if( keycode == 6 && SameKeyTime >= C_SAMEKEYTIME)
        {
             MFclearScreen();
        //    AdjustGainTable();
        //    while(true);

            int key;
            int nPulseWidth = 0;

            SystemParaRestore();
            ChannelParaRestore();
            MSetSystemMode(0,0);	//设置成一般状态
            MSetBaseGain(400,C_SETMODE_SETSAVE);

            MSetSystem();
            MSetAcquisition(1);

            ScreenRenovate();	/*屏幕刷新*/

            nPulseWidth = 33;	//33
            ETextOut(320,10,"PulseWidth:  ",11);
            Write_Number(320+C_ECHAR_HDOT*11,10,nPulseWidth,4,0,0);
            ExpendTime(50);
            MKeyRlx();
            while(true)
            {

                MParaRenovate(1);
                key = MGetKeyCode(0);
                if( key == C_KEYCODMAX)continue;
                if( key == C_KEYCOD_RETURN)break;
                if( key == C_KEYCOD_PLUS || key == C_KEYCOD_MINUS)
                {
                    if( key == C_KEYCOD_PLUS )nPulseWidth++;
                    else if( key == C_KEYCOD_MINUS )nPulseWidth--;
                    SetPulseWidth(nPulseWidth);
                    if( nPulseWidth >= 127)nPulseWidth = 0;
                    else if( nPulseWidth < 0) nPulseWidth = 127;
                    ETextOut(320,0,"                  ",16);
                    ETextOut(320,10,"PulseWidth:       ",16);
                    Write_Number(320+C_ECHAR_HDOT*11,10,nPulseWidth,4,0,0);
                    ExpendTime(20);
                    continue;
                }

                if( key == C_KEYCOD_BASEGAIN || key == C_KEYCOD_GATE || key == C_KEYCOD_RANGE)
                {
                    KeyManage(key,1);
                    ExpendTime(20);
                }
                MKeyRlx();
            }
        }
        else if( keycode == 7 && SameKeyTime >= C_SAMEKEYTIME)  //核心板检测
        {
            MFclearScreen();
            DisplayBMP(background,ColorsBG, 0, 100-24,281,640);
            MSetDisplayColor(COLOR_YELLOW);
            EMenuOut(162,2,"Kernel Testing ...      ",20,1,16);
            EMenuOut(120+32, 340+24*1-7,"1.Save Test    :     ",20,1,16);
            EMenuOut(120+32, 340+24*2-7,"2.Sound Volume :     ",20,1,16);
            EMenuOut(120+32, 340+24*3-7,"3.Internet Test:     ",20,1,16);
            EMenuOut(120+32, 340+24*4-7,"4.SD card Test :     ",20,1,16);
            EMenuOut(120+32, 340+24*5-7,"5.I2C Test     :     ",20,1,16);

            while(true)
            {

                int key = MGetKeyCode(0);
                char Sound=50,second=0,tempSecond=0;
                int Internet=0;

                MSetDisplayColor(COLOR_GREEN);

                    GetSystemTime(&TimeClock) ;
                    tempSecond = Bcd2Hex(TimeClock.second  & 0x7f)%60;
                if(tempSecond!=second  )
                {
                    ExpendTime(40);
                   // MEraseWindow(120+50,46,120+50+19*C_ECHAR_HDOT,46+24);
                    WriteDate(120+50, 46, &TimeClock);
                    WriteTime(120+50+11*C_ECHAR_HDOT,46, &TimeClock);//显示时间
                    EMenuOut(120+50+16*C_ECHAR_HDOT,46,":",1,1,16);
                    if(tempSecond<10)
                    {
                        Write_Number(120+50+17*C_ECHAR_HDOT,46,0,1,0,0);
                        Write_Number(120+50+18*C_ECHAR_HDOT,46,tempSecond,2,0,0);
                    }
                    else
                    {
                        Write_Number(120+50+17*C_ECHAR_HDOT,46,tempSecond,2,0,0);
                    }
                    second=tempSecond;

                }

                MSetDisplayColor(COLOR_CYAN);
                if( key == C_KEYCODMAX)continue;
                else if( key == C_KEYCOD_RETURN || key == C_KEYCOD_CONFIRM)break;
                else if( key == 1)  //save
                {
                    int temp1,temp2;
                    EMenuOut(120+32*9, 340+24*1-7,".......",7,1,16);

                    offset=C_OFF_TEMP;
                    CopyMemoryToSystem(offset,8,PM_COPY_READ);
                    MCopyProtectedMemory( &temp1, (void*)offset,8, PM_COPY_READ);
           //     Write_Number(10, 340+24*1-4,temp1,10,0,0);
           //     MAnyKeyReturn();
                    if(temp1==5)temp2=10;
                    else temp2=5;
                    MCopyProtectedMemory( (void*)offset,&temp2, 8, PM_COPY_WRITE);
                    CopyMemoryToSystem(offset,8,PM_COPY_WRITE);

                    CopyMemoryToSystem(offset,8,PM_COPY_READ);
                    MCopyProtectedMemory( &temp2, (void*)offset,8, PM_COPY_READ);

                    ExpendTime(10);
                    if((temp1==5 && temp2==10 )|| temp2==5)
                    EMenuOut(120+32*9, 340+24*1-7,"Success",7,1,16);
                    else
                    EMenuOut(120+32*9, 340+24*1-7,"Fail   ",7,1,16);
           //     MAnyKeyReturn();
                    MCopyProtectedMemory( (void*)offset,&temp1, 8, PM_COPY_WRITE);
                    CopyMemoryToSystem(offset,8,PM_COPY_WRITE);

                    CopyMemoryToSystem(offset,8,PM_COPY_READ);
                    MCopyProtectedMemory( &temp2, (void*)offset,8, PM_COPY_READ);
            //    Write_Number(10, 340+24*1-4,temp2,10,0,0);
           //     MAnyKeyReturn();


                }
                else if( key == 2)  //Sound
                {
                    WavePlayback((WavHeader*) alice, Sound) ;
                    WaveVolume(Sound) ;
                    key = C_KEYCODMAX;
                    elapsedtime = GetElapsedTime() + 1000 * 5;
                    while(1)
                    {
                        key = MGetKeyCode(0);
                        if( key == C_KEYCOD_RETURN || key == C_KEYCOD_CONFIRM)
                        {
                            MKeyRlx();
                            key = C_KEYCODMAX;
                            break;
                        }
                        else if(key==C_KEYCOD_PLUS)Sound+=10;
                        else if(key==C_KEYCOD_MINUS)Sound-=10;
                        else if(key!=C_KEYCODMAX)
                        {
                            break;
                        }
                        if(Sound>=100)Sound=100;
                        else if(Sound<=0)Sound=0;
                        Write_Number(120+32*9, 340+24*2-4,Sound,3,0,0);
                        if(GetElapsedTime()>=elapsedtime  )
                        {
                            WavePlayback((WavHeader*)alice, Sound) ;
                            elapsedtime = GetElapsedTime() + 1000 * 5;
                        }
                        WaveVolume(Sound) ;
                    }
                    while(1)
                    {
                        if(GetElapsedTime()>=elapsedtime )
                        {
                            break;
                        }
                    }
                }
                else if( key == 3)  //Internet
                {
                        /// The MAC address
                        unsigned char MacAddress[6] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55};

                        /// The IP address
                        unsigned char IpAddress[4] = {192, 168, 111,8 };//{10, 159, 245, 156};

                        EMenuOut(120+32*9, 340+24*3-7,".......",7,1,16);
                        Internet=EthernetInit(MacAddress, IpAddress);
                        ExpendTime(10);
                        if(Internet==0)
                        EMenuOut(120+32*9, 340+24*3-7,"Success",7,1,16);
                        else
                        EMenuOut(120+32*9, 340+24*3-7,"Fail   ",7,1,16);
                        //Write_Number(120+32*9, 340+24*4-4,Internet,3,0,0);
                        EthernetStop() ;
                        MKeyRlx();
                        key = C_KEYCODMAX;
                }
                else if( key == 4)  //SD
                {

                    EMenuOut(120+32*9, 340+24*4-7,".......",7,1,16);

                    if (DisplayQuery(14))	//已连接好U盘？
                    {
                        // U盘初始化
//                        memset(&g_UDiskInfo,0,sizeof(UDISK_INFO));
                        DisplayPrompt(19);	//正在连接U盘
                        if(!UDiskInitialize(2))
                        {
                            //初始化失败
                            DisplayPrompt(21);	//U盘连接失败
                            EMenuOut(120+32*9, 340+24*4-7,"Fail   ",7,1,16);
                        }
                        else
                        {
                            DisplayPrompt(20);	//U盘连接成功
                            if(WriteEchoDateToFile(2))
                            EMenuOut(120+32*9, 340+24*4-7,"Success",7,1,16);
                            else
                            EMenuOut(120+32*9, 340+24*4-7,"Fail   ",7,1,16);

                            DisplayPrompt(24);	//正在安全断开
                            FileClose(1);
                            DisplayPrompt(23);	//U盘断开连接
//                            memset(&g_UDiskInfo,0,sizeof(UDISK_INFO));
                        }
                    }

                    MSetDisplayColor(COLOR_YELLOW);
                    EMenuOut(162,2,"Kernel Testing ...      ",20,1,16);
                }
                else if( key == 5)  //I2C
                {
                    char a;
                    EMenuOut(120+32*9, 340+24*5-7,".......",7,1,16);
                    ExpendTime(50);
                    BatteryPackRead(0x0B0D,1,&a) ;
                    if(a>0 && a<=100)
                    EMenuOut(120+32*9, 340+24*5-7,"Success",7,1,16);
                    else
                    EMenuOut(120+32*9, 340+24*5-7,"Fail   ",7,1,16);

                }
                //else if( key == C_KEYCOD_PLUS || key == C_KEYCOD_MINUS)
            }

        }
        /*
        else if( keycode == 8 && SameKeyTime >= C_SAMEKEYTIME)
        {
        	//MenuMonthTest();//人工月性能校验
        	ManualMonthTest();//人工月性能校验

        }//*/
        else if( keycode == 8)
        {
            extend = 0;

        }
        else if( keycode == C_KEYCOD_RETURN )
        {
            //短按返回键，显示用户信息
            USER_INFO UserInfo;
            int offset = C_OFF_USER_INFO;
            MCopyProtectedMemory(&UserInfo, (void*)offset, sizeof(USER_INFO), PM_COPY_READ);
        MFclearScreen();

            //	CMenuOut(xpos,ypos,_UserInfo,char_len,row_number+2,row_height);	/*在指定位置根据每行字符数、行数、行高写菜单*/
            for (i=0; i<row_number+2; i++)
            {
                TextOut(xpos, ypos+row_height*i-(C_ECHAR_VDOT-4)*MGetLanguage(),  1, 19,row_height, (u_char*)_UserInfo[MGetLanguage()][i], 4);
            }
            para_xpos = xpos + char_len * C_CCHAR_HDOT;	/*写参数，位置水平方向右移*/
            para_ypos = ypos ;
            while(1)
            {

                //单位名称pzPurchaser[15];
                xpos = para_xpos;
                ypos = para_ypos + row_height *0;
                CETextOut2(xpos,ypos,UserInfo.pzPurchaser+1,UserInfo.pzPurchaser[0],1,24,0,0);	/*在指定位置根据每行字符数、行数、行高写菜单*/

                //仪器编号
                ypos = para_ypos + row_height *1;;
                CETextOut2(xpos,ypos ,UserInfo.pzSerial+1,UserInfo.pzSerial[0],1,24,0,0);

                //购买日期10\8	PurchaseTime;
                ypos = para_ypos + row_height *2;
                WriteDate(xpos, ypos+8, &UserInfo.PurchaseTime);

                //使用人员10 pzUser[5];
                ypos = para_ypos + row_height *3;
                CETextOut2(xpos,ypos,UserInfo.pzUser+1,UserInfo.pzUser[0],1,24,0,0);	/*在指定位置根据每行字符数、行数、行高写菜单*/

                //主管人员10		pzGovernor[5];
                ypos = para_ypos + row_height *4;
                CETextOut2(xpos,ypos,UserInfo.pzGovernor+1,UserInfo.pzGovernor[0],1,24,0,0);	/*在指定位置根据每行字符数、行数、行高写菜单*/

                //软件版本
                ypos = para_ypos + row_height *5;
                EMenuOut(xpos, ypos, _version1,8,1,12);
                EMenuOut(xpos+ 10*C_ECHAR_HDOT, ypos, _version,18,1,12);

                //内核版本
                ypos = para_ypos + row_height *6;
#if C_DEVLIB == 24
                EMenuOut(xpos, ypos+8, "REL V3.0",8,1,12);
#elif C_DEVTYPE == 3 || C_DEVTYPE == 9 || C_DEVTYPE == 11
                EMenuOut(xpos, ypos+8, "RDS V4.0",8,1,12);
#else
                EMenuOut(xpos, ypos+8, "N0 V2.0 ",8,1,12);
#endif
                ypos = para_ypos + row_height *7;
#if C_PROCESS == 2
                EMenuOut(xpos, ypos+8, "RP V2.0 ",8,1,12);
#elif C_PROCESS == 1
                EMenuOut(xpos, ypos+8, "NG V1.3 ",8,1,12);
#endif
                keycode = MenuKeyReturn(5,3);	/*出现菜单时，按不大于keymax的键返回，mode=0其它键无效，=1确认键有效，=2返回键有效，=3确认返回键有效*/
                if (keycode == C_KEYCOD_RETURN)
                {
                    MCopyProtectedMemory(&UserInfo, (void*)offset, sizeof(USER_INFO), PM_COPY_READ);
                    retvalue = C_FALSE;
                    break;
                }
                if (keycode == C_KEYCOD_CONFIRM)
                {
                    MSetSystem();
                    MCopyProtectedMemory( (void*)offset, &UserInfo, sizeof(USER_INFO), PM_COPY_WRITE);
                    CopyMemoryToSystem((void*)offset,C_OFF_BATTERY-C_OFF_USER_INFO, PM_COPY_WRITE);
                    retvalue = C_TRUE;
                    break;
                }

                ypos = para_ypos + row_height * (keycode - 1);

                MSetInputMode(C_INPUT_PINYIN,0);
                switch (keycode)
                {
                case 1:		//
                    Write_Space(xpos ,ypos,15,1);
                    MInputChar(xpos, ypos,1,UserInfo.pzPurchaser,20,10);
                    break;
                case 2:		//
                    Write_Space(xpos ,ypos+C_ECHAR_VDOT,20,0);
                    MSetInputMode(C_INPUT_LOWERCASE,0);	//设置成小写
                    MInputChar(xpos, ypos,0,UserInfo.pzSerial,19,19);
                    break;
                case 3:
                    InputDate(xpos, ypos, &UserInfo.PurchaseTime);
                    break;
                case 4:
                    Write_Space(xpos ,ypos,4,1);
                    MInputChar(xpos, ypos,1,UserInfo.pzUser,8,4);
                    break;
                case 5:
                    Write_Space(xpos ,ypos,4,1);
                    MInputChar(xpos, ypos,1,UserInfo.pzGovernor,8,4);
                    break;
                }
            }
        }
        else if( keycode == 0 )
        {
            //短按0键，显示电池信息
            /*	保护区开始8个字节存开机时间，再8字节存当前时间...
            	0-7			开机时间8
            	10-17		关机时间8
            	20-31		电池指示表11
            	36-39		开机时长,单位分钟
            	40-159		电池电量记忆表
            	160-164		开始充电时间,单位分钟
            */
        MFclearScreen();
            u_int ontime;
            u_char powerlevel;
            offset = C_OFF_BATTERY + 0;
            MCopyProtectedMemory( &TimeClock, (void*)offset, 8, PM_COPY_READ);
            xpos = 0;
            ypos = 20;
            EMenuOut(xpos, ypos, "Open:",5,1,12);
            WriteDate(xpos + 6*C_ECHAR_HDOT, ypos, &TimeClock);		//读并显示开机时间
            WriteTime(xpos + 17 * C_ECHAR_HDOT, ypos, &TimeClock);		//读并显示关机时间

            offset = C_OFF_BATTERY + 36;	//开机时长存储地址，单位分钟
            MCopyProtectedMemory( &ontime, (void*)offset, 4, PM_COPY_READ);
            EMenuOut(xpos+ 25 * C_ECHAR_HDOT, ypos, "Elapsed:",8,1,12);
            Write_Number(xpos+ 33 * C_ECHAR_HDOT, ypos,ontime,4,0,0);


            for( i = 0 ; i * 5 < ontime && i < 120; i++)
            {
                offset = C_OFF_BATTERY + 80 + 2*i ;
                MCopyProtectedMemory(  &powerlevel, (void*)offset,1, PM_COPY_READ);
                if( i % 12 == 0 )
                {
                    xpos = 0;
                    ypos += 24;
                    Write_Number(xpos,ypos,i/12,2,0,0);
                }
                xpos += 3*C_ECHAR_HDOT;
                Write_Number(xpos,ypos,powerlevel,2,0,0);
            }
            offset = C_OFF_BATTERY + 10;
            MCopyProtectedMemory( &TimeClock, (void*)offset, 8, PM_COPY_READ);
            xpos = 0;
            ypos = 400;
            EMenuOut(xpos, ypos, "Shut:",5,1,12);
            WriteDate(xpos+6*C_ECHAR_HDOT, ypos, &TimeClock);		//读并显示开机时间
            WriteTime(xpos + 18 * C_ECHAR_HDOT, ypos, &TimeClock);		//读并显示关机时间
            MAnyKeyReturn();
            break;
        }
        else if( keycode == 9)
        {
            //短按9，数据清除
            InitPara();
            offset = C_OFF_STACK;
            MCopyProtectedMemory( (void*)offset, GainTable,  121*4, PM_COPY_WRITE);
//            CopyMemoryToSystem((void*)offset,121*4, PM_COPY_WRITE);

        }
        break;
    }
#if C_DEVLIB == 1
    /* set echo display points, notext > 0 for 280, otherwise 308 */
    NotExtendEchoDisplay(extend) ;//暂时去除
#endif
    offset = C_OFF_BATTERY;
    GetSystemTime(&TimeClock);
    MCopyProtectedMemory( (void*)offset, &TimeClock, 8, PM_COPY_WRITE);
//    CopyMemoryToSystem((void*)offset,8, PM_COPY_WRITE);

    //开机时间
    SysPwd(0);

}
extern const u_short _COMM_B1[] ;
extern CACHE	cache;
//	const u_char pzText1[] = "wait keyin          ";
const u_char pzText2[] = "transmit start      ";
//	const u_char pzText3[] = "Test                ";

void Usb2PC(void)
{
    int temp=0;
	int byteIn;
	int byteOut;
	int i,j,k;
	int xpos,ypos;
	unsigned int offset;
	unsigned char out[C_LEN_FILE+C_LEN_FAT];
	unsigned char in[C_LEN_FILE+C_LEN_FAT];

    int File_temp;
/////
//CH(4293),CH(3061),CH(5393),CH(5258),CH(2011),CH(4268),CH( 312),CH(3975),CH(2140),CH(2093),'.','.','.',' '
//};//数据正在传输，请等待
//	const u_char pzText1[] = "wait keyin          ";
//	const u_char pzText2[] = "transmit start      ";
//	const u_char pzText3[] = "Test                ";

    const char _COMM_B1[C_LANGUAGE][3][25] =
    {
        {"传输中","连接失败"},
        {"Connecting,please wait!","Connection failed         "}
    };
	int file_no,file_total;
//	int time;
    POWER power;

    offset = C_OFF_POWER;
    MCopyProtectedMemory(&power, (void*)offset, sizeof(POWER), PM_COPY_READ);

    for(i=0;i<60;i++)
    {
        byteIn=UsbDevDataIn(in,1);
//    Write_Number(170,130,byteIn,4,0,0);
//    Write_Number(200,130,in[0],4,0,0);

        if( byteIn > 0)break;
        ExpendTime(4);


    }
	j = 0;
	for( k = 0 ; k < 100; k++)out[k] ='\0';

        byteOut=UsbDevDataOut(out,64);
    temp+=24;
 //   Write_Number(50,100+temp,byteOut,4,0,0);
  //  ExpendTime(10);

    xpos = 0;
    ypos = 0;
    MEraseWindow(xpos, ypos, xpos+C_ECHAR_HDOT*23,ypos+C_ECHAR_VDOT) ;
    TextOut(xpos,ypos,  1, 23,24, (u_char*)_COMM_B1[MGetLanguage()][0], 0); //MGetLanguage()
//byteIn=1;
    if(byteIn==0)
    {
        MEraseWindow(xpos, ypos, xpos+C_ECHAR_HDOT*23,ypos+C_ECHAR_VDOT) ;
        TextOut(xpos,ypos,  1, 23,24, (u_char*)_COMM_B1[MGetLanguage()][1], 0); //MGetLanguage()
        ExpendTime(100);
    }
	while(byteIn > 0)
	{
//Write_Number(10,20,11,4,0,0);
		/*xpos = 0;
		ypos = 0;
		if (MGetLanguage())
		{
			EMenuOut(xpos,ypos,"Connecting,please wait!",23,1,24);
		}
		else
		{
            TextOut(xpos,ypos,  1, 6,24, (u_char*)_COMM_B1[0][0], 0); //MGetLanguage()
		}*/
		file_no = 0;
		j=0;
		ExpendTime(1);
		file_total = DataDelete(-1);	//参数为负或正常删除返回剩余数据总数，参数过大返回－1

		for( i = 0, offset = C_OFF_FILEINFO; i < 4 ; i++, offset++)
		{//存储的文件总数
			MCopyProtectedMemory( &byteOut, (void*)offset, 1, PM_COPY_READ);
            out[j] = byteOut;
            j++;
			//while( !UsbDevDataOut(byteOut,1) );
		}
		for( i = 0; i < 4 ; i++)	//36
		{//文件头大小
			byteOut = ( C_LEN_FAT_C >> ( i * 8 ) ) & 0xff;
            out[j] = byteOut;
            j++;
			//while( !UsbDevDataOut(byteOut,1) );
		}
		for( i = 0; i < 4 ; i++)	//403	//808
		{//文件体大小
			byteOut = ( C_LEN_FILE_C >> ( i * 8 ) ) & 0xff;
            out[j] = byteOut;
            j++;
			//while( !UsbDevDataOut(byteOut,1) );
		}
        for( k = 0; k < 4 ; k++)
        {//型号
            byteOut = ( C_DEVTYPE >> ( k * 8 ) ) & 0xff;
            out[j] = byteOut;
            j++;
        }

        for( k = 0; k < 4 ; k++)
        {
            //Horiline
            byteOut = ( power.Horiline >> ( k * 8 ) ) & 0xff;
            out[j] = byteOut;
            j++;
        }
        for( k = 0; k < 4 ; k++)
        {
            //vertline
            byteOut = ( power.vertline >> ( k * 8 ) ) & 0xff;
            out[j] = byteOut;
            j++;
        }
        for( k = 0; k < 4 ; k++)
        {
            //ResolvingPower
            byteOut = ( power.ResolvingPower >> ( k * 8 ) ) & 0xff;
            out[j] = byteOut;
            j++;
        }
        for( k = 0; k < 4 ; k++)
        {
            //Sensitivity
            byteOut = ( power.Sensitivity >> ( k * 8 ) ) & 0xff;
            out[j] = byteOut;
            j++;
        }
        for( k = 0; k < 4 ; k++)
        {
            //Dynamic
            byteOut = ( power.Dynamic >> ( k * 8 ) ) & 0xff;
            out[j] = byteOut;
            j++;
        }
        byteOut=UsbDevDataOut(out,36);
    temp+=24;
   // Write_Number(50,100+temp,byteOut,4,0,0);
        ExpendTime(10);

    temp=0;

		for( file_no = 0; file_no < file_total; file_no++)
		{//文件头
		    File_temp=cache.cache[file_no];
		    offset = C_OFF_FAT + C_LEN_FAT * File_temp;

			MCopyProtectedMemory( &out, (void*)offset, C_LEN_FAT, PM_COPY_READ);
        byteOut=UsbDevDataOut(out,C_LEN_FAT_C);
    temp+=24;
   // Write_Number(80,100+temp,File_temp,4,0,0);
        ExpendTime(2);

   // Write_Number(80,100+temp,byteOut,4,0,0);
		}

    temp=0;
		xpos += 10*16+4;
		ypos = 10;
		for( file_no = 0; file_no < file_total; file_no++)
		{
		    File_temp=cache.cache[file_no];
		    offset = C_OFF_FILE + C_LEN_FILE * File_temp;

			MCopyProtectedMemory( &out, (void*)offset, C_LEN_FILE, PM_COPY_READ);

        byteOut=UsbDevDataOut(out,C_LEN_FILE_C);
    temp+=24;
  //  Write_Number(110,100+temp,byteOut,4,0,0);
        ExpendTime(2);
			Write_Ascii(xpos,ypos,'.');
			xpos += 8;
			if( xpos >= 240 )ypos += 10;
			xpos %= 240;

		}
		break;
	}
}






