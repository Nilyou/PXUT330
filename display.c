// display.c
/*
int DrawPower(u_char powerlevel,int mode)	//画电量指示,mode = 0立即1每分钟更新一次
void SysParaDisp(void)		//显示通道号、探头类型、频带等
*/
#include "Globe.h"
#include "display.h"
#include "sysdef.h"
#include "common.h"
#include "main.h"
#include "sdcard.h"
#include "M10Era.h"
//#include "Screen1.H"

/*extern const unsigned char _KeyLock[];
extern const unsigned char _ProbeFace[];
extern const unsigned char _init[];  	  //初始化
extern const unsigned char _envelope[]; 	  //包络
extern const unsigned char _echomax[];  	  //峰值
extern const unsigned char _DAC[];	         //DAC门
extern const unsigned char _TCG[];          //深度补偿
extern const unsigned char _expend[];	  //门内展宽
extern const unsigned char _alarm[];	  //门内报警
extern const unsigned char _Data[];   	  //数据处理
extern const unsigned char _Freeze[]; 	  //冻结
*/


extern const char 	_us[C_LANGUAGE][1][4];
const char 	_ProbeFace[] =
{
    0x00,0x00,0x07,0x04,0x04,0x04,0x04,0x3f,
    0x00,0x00,0x00,0x01,0x01,0x00,0x00,0x00,
    0x00,0x00,0xf0,0x10,0x10,0x10,0x10,0xfe,
    0x80,0x80,0x80,0xc0,0xc0,0x80,0x00,0x00,

    0x00,0x00,0x07,0x04,0x04,0x04,0x04,0x3f,
    0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0xf0,0x10,0x10,0x10,0x10,0xfe,
    0x00,0x80,0x40,0x20,0x18,0x18,0x00,0x00,

    0x00,0x00,0x3e,0x22,0x22,0x22,0x7f,0x10,
    0x08,0x04,0x02,0x01,0x07,0x00,0x00,0x00,
    0x00,0x00,0x3e,0x22,0x22,0x22,0x7f,0x08,
    0x10,0x20,0x40,0x80,0xe0,0x00,0x00,0x00,

    0x00,0x00,0x03,0x02,0x02,0x0f,0x00,0x00,
    0x00,0x0f,0x02,0x02,0x03,0x00,0x00,0x00,
    0x00,0x00,0xe0,0x20,0x20,0xf8,0x80,0x80,
    0x80,0xf8,0x20,0x20,0xe0,0x00,0x00,0x00
};

const unsigned char _KeyLock[] =
{
    0x00,0x00,0x00,0x00,0x00,0x00,0x0f,0x08,
    0x08,0x08,0x08,0x08,0x08,0x0f,0x00,0x00,
    0x00,0x00,0x30,0x48,0x48,0x40,0xf0,0x10,
    0x10,0x10,0x10,0x10,0x10,0xf0,0x00,0x00,

    0x00,0x00,0x01,0x02,0x02,0x02,0x0f,0x08,
    0x08,0x08,0x08,0x08,0x08,0x0f,0x00,0x00,
    0x00,0x00,0x80,0x40,0x40,0x40,0xf0,0x10,
    0x10,0x10,0x10,0x10,0x10,0xf0,0x00,0x00,
};

const unsigned char _init[] =  	  //初始化
{
0x01,0x06,0x08,0x10,0x20,0x47,0x47,0x87,0x87,0x87,0x87,0x87,0x87,0x87,0x87,0x87,
0x47,0x47,0x23,0x21,0x10,0x08,0x06,0x01,0xFF,0x00,0x00,0x00,0x00,0x81,0x81,0x81,
0x81,0x81,0x81,0x81,0x81,0x81,0x81,0x81,0x81,0xC3,0xFF,0xFF,0xFF,0x00,0x00,0xFF,
0x80,0x60,0x10,0x08,0x04,0xE2,0xE2,0xE1,0xE1,0xE1,0xE1,0xE1,0xE1,0xE1,0xE1,0xE1,
0xE2,0xE2,0xC4,0x84,0x08,0x10,0x60,0x80,
};
const unsigned char _envelope[] =  	  //包络
{
0x00,0x00,0x00,0x01,0x02,0x02,0x04,0x04,0x08,0x08,0x10,0x30,0x20,0x50,0x40,0x50,
0x80,0x90,0x00,0x28,0x00,0x28,0x00,0xFF,0x00,0x30,0xEC,0x22,0x21,0x20,0x20,0x50,
0x50,0x50,0x50,0x50,0x50,0x50,0x50,0x50,0x50,0x50,0x50,0x50,0x50,0x50,0x50,0xFF,
0x00,0x00,0x00,0x00,0x00,0x80,0x40,0x20,0x10,0x28,0x08,0x24,0x04,0x22,0x01,0x20,
0x00,0x50,0x00,0x50,0x00,0x50,0x00,0xFF,
};
const unsigned char _echomax[] =  	  //峰值
{
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x01,0xFF,0xFF,0x00,0x30,0x30,0x30,0x30,0x30,0x30,0x48,
0x48,0x48,0x48,0x48,0x48,0x48,0x48,0x48,0x48,0x48,0x48,0x48,0x48,0x86,0xFF,0xFF,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,
};
const unsigned char _DAC[] = {	  //DAC门
0x08,0x08,0x08,0x08,0x0F,0x00,0x00,0xFE,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x02,
0x02,0x02,0x02,0x02,0x02,0x04,0x04,0xFF,0x00,0x00,0x00,0x00,0x00,0x80,0x40,0x20,
0x10,0x88,0x44,0x22,0x11,0x08,0x04,0x82,0x81,0x80,0x80,0x80,0x80,0x40,0x40,0xFF,
0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0xF8,0x00,0x00,
0x00,0xFF,0x00,0x00,0x00,0x40,0x40,0xFF,
};
const unsigned char _TCG[] = {	  //深度补偿
0x00,0xFF,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x05,0x05,0x05,0x05,
0x05,0x05,0x05,0x05,0x05,0x05,0x05,0xFF,0x00,0x36,0x80,0x40,0x20,0x10,0x08,0x04,
0x02,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,
0x00,0xDB,0x08,0x00,0x08,0x00,0x08,0x00,0x08,0x14,0x80,0x54,0x3F,0x08,0x08,0x08,
0x08,0x14,0x14,0x14,0x14,0x14,0x14,0xFF,
};
const unsigned char _expend[] = {	  //门内展宽
0x00,0x02,0x00,0x02,0x01,0x00,0x02,0x00,0x00,0x02,0x00,0x20,0x62,0xD8,0x60,0x22,
0x00,0x00,0x02,0x00,0x08,0x0A,0x14,0xFF,0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0x00,
0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x28,0x28,0x28,0x28,0x28,0x28,0x44,0x45,0xFF,
0x00,0x40,0x00,0x40,0x80,0x00,0x40,0x00,0x00,0x40,0x00,0x04,0x46,0x1D,0x06,0x44,
0x00,0x00,0x40,0x00,0x00,0x40,0x40,0xFF,
};
const unsigned char _alarm[] = {	  //门内报警
0x00,0x00,0x00,0x00,0x42,0x22,0x13,0x02,0x52,0x02,0x12,0x42,0x13,0x22,0x42,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x84,0x43,
0x60,0x20,0x60,0x40,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,
0x00,0x00,0x00,0x00,0x20,0x20,0x21,0xFE,0x20,0x50,0x50,0x50,0x50,0x50,0x50,0x50,
0x88,0x88,0x88,0x88,0x88,0x88,0x88,0xFF,
};
const unsigned char _Data[] =  	  //数据处理
{
0x00,0x1F,0x10,0x70,0x43,0x42,0x4E,0x48,0x48,0x49,0x4A,0x4A,0x4A,0x4A,0x4A,0x49,
0x48,0x48,0x48,0x48,0x4A,0x78,0x08,0x0F,0x00,0xFF,0x00,0x00,0xFF,0x00,0x00,0x78,
0x84,0x02,0x01,0x01,0x31,0x01,0x01,0x02,0x87,0x7B,0x01,0x00,0x48,0x00,0x00,0xFF,
0x00,0xE0,0x20,0x20,0xFC,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,
0x04,0x84,0xC4,0xE4,0x74,0x3C,0x1E,0xFA,
};
const unsigned char _Freeze[] = {	  //冻结
0x00,0x00,0x00,0x10,0x52,0x72,0x3A,0x6E,0x0F,0x19,0x10,0x00,0x10,0x19,0x0F,0x6E,
0x3A,0x72,0x52,0x10,0x00,0x00,0x00,0x00,0x18,0x5A,0x3C,0x99,0xDB,0x7E,0x3C,0x18,
0x99,0xFF,0x7E,0x3C,0x7E,0xFF,0x99,0x18,0x18,0x3C,0x7E,0x5A,0x99,0x3C,0x5A,0x18,
0x00,0x00,0x00,0x08,0x4A,0x4E,0x5C,0x76,0xF0,0x98,0x08,0x00,0x08,0x98,0xF0,0x76,
0x5C,0x4E,0x4A,0x48,0x00,0x00,0x00,0x00,
};
const unsigned char _Fill[] = {	  //实心波
0x80,0x80,0x80,0x80,0x80,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,
0xC0,0xC0,0xC0,0xE0,0xE0,0xF1,0xFF,0xFF,0x00,0x00,0x00,0x30,0x30,0x30,0x30,0x30,
0x30,0x30,0x30,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0xFE,0xFF,0xFF,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x20,0x20,0x30,
0x30,0x30,0x30,0x30,0x30,0x78,0xFF,0xFF,
};

const unsigned char _usbdisk[] =  	  //U盘
{
    0x7F,0x80,0xBE,0xB6,0xB2,0xB4,0xA4,0x96,
    0x96,0xB6,0xBE,0x80,0xFF,0x7F,0x2A,0x3E,
    0x00,0x80,0x80,0x80,0x80,0x80,0x80,0x80,
    0x80,0x80,0x80,0x80,0x80,0x00,0x00,0x00,
};

const unsigned char _usberror[] =  	  //不可连接电脑
{
    0x07,0x04,0x05,0xFF,0x81,0xBD,0xBD,0xBD,
    0x81,0xFF,0x7E,0xC3,0xFF,0xFF,0x7C,0x01,
    0xF8,0x08,0xE8,0xE8,0xE8,0x08,0xF8,0xD3,
    0xF7,0x7E,0xBC,0x18,0x3C,0x7E,0xC7,0x83,
};

const unsigned char _usbarrow[] =  	  //不可连接电脑
{
    0x10,0x28,0x44,0x82,0x10,0x28,0x44,0x82,
    0x10,0x28,0x44,0x82,0x10,0x28,0x44,0x82,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};

const unsigned char _copy[] = {	  //屏幕拷贝
	0x00,0x03,0x02,0x02,0x0D,0x2B,0xF8,0xA9,
	0x89,0x0F,0x81,0x80,0x00,0xDB,0x00,0x00,
	0x00,0xF0,0x10,0x10,0xEC,0xF4,0x04,0xE4,
	0x24,0x3C,0xE0,0x00,0x00,0x6D,0x00,0x00,
};

extern int g_nDenoise;
GATE_AMPMAX stGateAmpMax;

unsigned char PowerAlarm;
extern unsigned char ChannelMax;
;
CURVE_AMEND stCurveAmend;
extern u_short dac_line[];
extern WELD_PARA stWeldPara;

extern ADJUST_TIME			AdjustTime;
extern FUNCTION             Function;   //功能

extern int HoriOffsetScreen ;		//所有写在屏幕上的内容，水平方向开始点
extern int VertOffsetScreen ;		//所有写在屏幕上的内容，水平方向开始点

extern DAC			Dac;

extern int extend ;
//色彩
extern CHANNEL_PARAMETER	ChannelPara;		/*15*2*/

extern u_char _infinity[C_LANGUAGE][1][6];
extern int curr_cr;

extern int GateParaMax,GateParaValue;
u_char crPara[C_CR_MAX+12];
const u_int Echo_crPara[] =
{

    0x007,//红
    0x0c7,//粉红
    0x03f,//黄
    0x038,//绿
    0x0f8,//青
    0x0c0,//蓝
    0x0A4,//紫
    0x0ff,//白
    0x000,//黑
};
const u_int c_crPara[] =
{
/*
    0x007,//红
    0x0c7,//粉红
    0x03f,//黄
    0x038,//绿
    0x0f8,//青
    0x0c0,//蓝
    0x084,//紫
    0x0ff,//白
    0x000,//黑
  */
    COLOR_RED,//红
    COLOR_PINK,//粉红
    COLOR_YELLOW,//黄
    COLOR_GREEN,//绿
    COLOR_CYAN,//青
    COLOR_BLUE,//蓝
    COLOR_GREY,//紫
    COLOR_WHITE,//白
    COLOR_BLACK,//黑

};
/*
颜色设置
1。回波
2。包络
3。菜单
4。参量
5。坐标
6。门A
7。门B
8。提示
9。曲线
红粉黄绿青兰紫白
//*/


//extern const u_int KEY_DIGITAB[];
//extern const u_char MadeDate[21];//制造日前
//extern const u_char _version[30];//版本号

//extern const u_char AmpMaxCom_Table[] ;
extern const u_char PowerTable[] ;
extern const u_int _RANGEVALUE[];

extern char _ParaName[C_LANGUAGE][21][16];
u_char tempvalue;
void DisplayBMP(const u_char * picture,const u_long * Palette, int xpos, int ypos,int length,int width)
//480*640图片
{
    int i,j;
    int x=xpos,y=ypos;
    // GrColor Color[480*640];
    u_long tempcolor,Color;

    for(i=0; i<length; i++)
    {
        for(j=0; j<width; j++)
        {
            tempcolor= Palette[*((u_char *)picture+(i*width+j))];
            Color=( ((tempcolor&0x0000ff)<<16)&0xff0000)+(tempcolor&0x00ff00)+(((tempcolor&0xff0000)>>16)&0x0000ff);
            Color=((((Color&0x0000f8)>>3)+((Color&0x00fc00)>>5)+((Color&0xf80000)>>8))&0xffff)^ c_crPara[crPara[ C_CR_BACK] ];
            MSetDisplayColor(Color);
            DrawPixel(x+j,y+i);
        }

    }

}

int DrawPower(u_char powerlevel,int mode)	//画电量指示,mode = 0立即1每分钟更新一次
{
    u_int xpos,ypos;
    int startchargetime;
    int offset;
    int elapsedtime = GetElapsedTime() / 1000;	//当前时间，单位s

    /* get the return value from battery management
       the return vaule: b0 -> battery charge fast(1) or trickle(0) ,;
    	             b1 -> battery charge on(1) or off(0) ;
    	             b2 -> battery connected(1) or disconnected(0) ;
    	             b3 -> DC adapter connected(1) or disconnected(0) ;
    	             b8-b15 -> battery capacity value < 32
    				 低8位与高8位应交换
    */
    ///if( MGetSystemMode() == 1)return -1;
    if( MGetAcquisitionEnable() != 1)return -1;
    if (elapsedtime < 10  )return 0; //开机10s之内
    if( mode != 0 ) 	//没有要求立即更新
    {
        if( MGetSystemMode() == 1 )return 0;	//测试状态不画电池
    }
///Write_Number(10,30,GetBatteryStatus()%256,4,0,0);
///Write_Number(10,40,GetBatteryStatus()/256,4,0,0);
    u_char powerstatus = (GetBatteryStatus() / 256)%4;	//得到电池状态，d1-d0
    int i,j;
    offset =  C_OFF_BATTERY + 40;		//开始充电时间
    MCopyProtectedMemory( &startchargetime, (void*)offset, C_SIZE_INT, PM_COPY_READ);

    MSetColor(C_CR_MENU);
    MEraseWindow(C_POWER_HPOSI, C_POWER_VPOSI, C_POWER_HPOSI + 11, C_POWER_VPOSI + 28) ;

    xpos = C_POWER_HPOSI;
    ypos = C_POWER_VPOSI + 7 ;
    DrawRectangle(xpos,ypos,xpos + 10,ypos+20);	//画电池外壳
    for(i = 1 ; i < 5; i++)
    {
        MDrawLine( xpos + 1, ypos + 4 * i ,xpos + 1, ypos + 4 * i ,C_CR_BACK);  //刻度
    }
    xpos += 2;
    ypos += 20;
    //外接充满0；电池充满不充电1；外接充电2；电池供电3
    if( ( powerstatus &  0x1 ) == 0 )	//外接电源；b0=0表示在充电，b1=0表示充满
    {
        for(i=0; i < 10; i++)
        {
            if( powerlevel >= PowerTable[i] )break;
        }
        if( i > 0)	i = ( 10 - i) * 2 ;	//反过来,从下往上画
        else i = 19;	//满电
        MSetColor(C_CR_PARA);
        for( ; i > 0; i--)	//画电量
            MDrawLine(xpos,ypos - i,xpos+7,ypos - i,C_CR_BACK);

        int chargedtime = (elapsedtime - startchargetime)/60;  //已充时间，单位为分钟

        if (( powerstatus & 0x2 ) != 0)//充电；b0=0表示在充电，b1=0表示充满
        {
            //在快充
            i = chargedtime / 6 + 1;  //快充一般为100-120分钟
            if( i > 16 ) i = 16;
            j = chargedtime/60;
            if(j > 2 )j = 0;
            else j = 2 - j;
        }
        else    //充满
        {
            //为慢充
            if(chargedtime > 60 * 3)i = 19;		//充电长达3小时，应已充满
            else if(chargedtime > 150)i = 18;
            else i = 17;
            j = 0;
        }

        //	for( ; i > 0; i--)	//画电量
        //		MDrawLine(xpos,ypos - i,xpos+7,ypos - i);

        MSetColor(C_CR_MENU);
        xpos = C_POWER_HPOSI + 5 ;
        ypos = C_POWER_VPOSI ;
        //箭头
        while( ypos < C_POWER_VPOSI + 5 -j)
        {
            MDrawLine(xpos - j,ypos,xpos + j,ypos,C_CR_BACK);
            ypos++;
        }
        for(i=j+1; i >= 0 ; i--,ypos++)
            MDrawLine(xpos - i,ypos,xpos + i,ypos,C_CR_BACK);

//		for(i=0;i<= 2 * j ;i++)
//			MDrawLine(xpos,ypos+i,xpos + 2 * j,ypos+i);

    }
    else //不在充电
    {
        for(i=0; i < 10; i++)
        {
            if( powerlevel >= PowerTable[i] )break;
        }

        //if( i >= 10 && ( (powerstatus & 0x0c) == 0 ) && powerlevel > 0 )	DisplayPrompt(1);		//d3=0表示未联充电器//提示电量不足
        if( ( (powerstatus & 0x02) != 0 ) && (powerlevel > 0 && powerlevel <= PowerAlarm))
        {
            BuzzerOn(10);
            DisplayPrompt(1);		//d3=0表示未联充电器//提示电量不足
            ExpendTime(30);
            BuzzerOn(10);
            DisplayPrompt(1);		//d3=0表示未联充电器//提示电量不足
            ExpendTime(50);
            BuzzerOn(10);
            DisplayPrompt(1);		//d3=0表示未联充电器//提示电量不足
            ExpendTime(100);
            if(powerlevel <= PowerTable[10])
            {

                CharToKeypad(LOW_BATTERY_OFF) ;

                while(1)
                {
                   // EMenuOut(10,0,"DONE!   ",8,1,16);
                    CopyMemoryAllSystem(PM_COPY_WRITE);
                    CharToKeypad(DETECTOR_OFF_ACK);
                    ExpendTime(20);
                }

            }
        }
        if( i > 0)	i = ( 10 - i) * 2 ;	//反过来,从下往上画
        else i = 19;	//满电
        MSetColor(C_CR_PARA);
        for( ; i > 0; i--)	//画电量
            MDrawLine(xpos,ypos - i,xpos+7,ypos - i,C_CR_BACK);
        xpos = C_POWER_HPOSI + 4;
        ypos = C_POWER_VPOSI + 4;

        MSetColor(C_CR_MENU);
        MDrawLine(xpos,ypos,xpos+2,ypos,C_CR_BACK);
        MDrawLine(xpos,ypos+1,xpos,ypos+2,C_CR_BACK);
        MDrawLine(xpos+2,ypos+1,xpos+2,ypos+2,C_CR_BACK);
    }

    if( extend == 0)
    {
        xpos = C_POWER_HPOSI-5*C_ECHAR_HDOT;
        ypos = C_POWER_VPOSI+2 ;
      //  MDrawLine(xpos,ypos-2,xpos + 3*C_ECHAR_HDOT,ypos-6,C_CR_BACK);
      //  MDrawLine(xpos,ypos+C_ECHAR_VDOT+2,xpos + 3*C_ECHAR_HDOT,ypos+C_ECHAR_VDOT+2,C_CR_BACK);
      EraseDrawRectangle(xpos-4,ypos-4,xpos + 52,ypos+30);
        MSetColor(C_CR_MENU);
        Write_Number(xpos,ypos,powerlevel,3,0,0);
        //Write_Ascii(xpos+3*C_ECHAR_HDOT,ypos,'%');
    }
    return 1;
}

void IcoDotDis(int xpos,int ypos, u_char *epText)    //24*24
{
    MUserDotsOut(xpos,ypos,(u_char*)(epText),8);
    MUserDotsOut(xpos,ypos+8,(u_char*)(epText+8),8);
    MUserDotsOut(xpos,ypos+8*2,(u_char*)(epText+8*2),8);
    MUserDotsOut(xpos+8,ypos,(u_char*)(epText+8*3),8);
    MUserDotsOut(xpos+8,ypos+8,(u_char*)(epText+8*4),8);
    MUserDotsOut(xpos+8,ypos+8*2,(u_char*)(epText+8*5),8);
    MUserDotsOut(xpos+8*2,ypos,(u_char*)(epText+8*6),8);
    MUserDotsOut(xpos+8*2,ypos+8,(u_char*)(epText+8*7),8);
    MUserDotsOut(xpos+8*2,ypos+8*2,(u_char*)(epText+8*8),8);
 /*    return;
   int i,j;
    short xtemp=x,ytemp=y;
    MEraseWindow(x,y,x+xdots,y+ydots);

    for( i = 0 ; i < (ydots*(xdots/8)); i++)    //总点数
    {
        for( j = 7; j >= 0; j--)    //一个值，8个点
        {
            if( ((pDots[i] >> j)&0x01) > 0)MDrawPixel(xtemp+7-j,ytemp,DP_SET);
        }
        xtemp+=8;
        if(i%(xdots/8)==0 && i>3)
        {
            xtemp=x;
            ytemp++;
        }
    }
*/

}
extern const u_char _ParaMenu_AD3[C_LANGUAGE][4][10] ;
//输入8* X的字符
void MUserDotsOut(short x,short y, u_char *pDots, u_int ydots)
{
    MEraseWindow(x,y,x+8,y+ydots);
    {
        int i,j;
        for( i = 0 ; i < ydots; i++)
        {
            for( j = 7; j >= 0; j--)
            {
                if( ((pDots[i] >> j)&0x01) > 0)MDrawPixel(x+7-j,y,DP_SET);
            }
            y++;
        }
    }
}
void PageDisp(int xpos,int ypos,u_char* mainpage)		//显示16*16图形
{
    u_char *epText;
    epText = (u_char *)mainpage;

#if C_DEVLIB != 23
    UserECharOut(xpos + HoriOffsetScreen, ypos , (char*)(epText)) ;
    UserECharOut(xpos + HoriOffsetScreen, ypos +8, (char*)(epText+8)) ;
    UserECharOut(xpos + HoriOffsetScreen+8, ypos , (char*)(epText+16)) ;
    UserECharOut(xpos + HoriOffsetScreen+8, ypos +8, (char*)(epText+24)) ;
#else
    MUserDotsOut(xpos,ypos,(u_char*)(epText),8);
    MUserDotsOut(xpos,ypos+8,(u_char*)(epText+8),8);
    MUserDotsOut(xpos+8,ypos,(u_char*)(epText+16),8);
    MUserDotsOut(xpos+8,ypos+8,(u_char*)(epText+24),8);
#endif
    //显示回车键
}
void SysParaDisp(void)		//显示通道号、探头类型、频带等
{
    char _Message_Dis[C_LANGUAGE][7][8] =
    {
        {
            "通道    ",   //1
            "增益    ",   //2
            "声程    ",   //2
            "Ｋ值    ",   //1
            "探头    ",   //1
            "频带    ",   //1
            "抑制    ",   //1
        },
        {
            "CH.     ",   //1
            "Gain    ",   //2
            "Range   ",   //2
            "Angle   ",   //1
            "Probe   ",   //1
            "FB      ",   //1
            "REJ     ",   //1
        }
    };
    int C_Menu_HPOSI=C_BASEGAIN_HPOSI-10;
    int rect_xpos,rect_ypos;	//框所再位置
    int xpos,ypos;
    u_short pzChn[] = {CH(4508),CH(2132)};	//通道
    u_short *cpText;
    u_char *epText;
    int i;

    int char_len,char_row,char_height;
    u_short Channel = MGetChannel();

    //if( MGetSystemMode() == 1 )return;	//测试状态不显示相关信息


    DrawPower( ( GetBatteryStatus()%256 ) ,0);	//立即更新电池

    MSetColor(C_CR_PARA);
    rect_xpos = C_Menu_HPOSI-1 ;
    rect_ypos = C_COORVPOSI ;

    MSetColor(C_CR_MENU);
    TextOut(C_Menu_HPOSI,rect_ypos+C_MenuHigh*0+3,1,5,C_CCHAR_VDOT,(u_char*)_Message_Dis[MGetLanguage()][0],4);   //通道1
    TextOut(C_Menu_HPOSI,rect_ypos+C_MenuHigh*1+3,1,5,C_CCHAR_VDOT,(u_char*)_Message_Dis[MGetLanguage()][1],4);   //增益4
    TextOut(C_Menu_HPOSI,rect_ypos+C_MenuHigh*5+3,1,5,C_CCHAR_VDOT,(u_char*)_Message_Dis[MGetLanguage()][2],4);   //声程2
    TextOut(C_Menu_HPOSI,rect_ypos+C_MenuHigh*7+3,1,5,C_CCHAR_VDOT,(u_char*)_Message_Dis[MGetLanguage()][3],4);   //K 值1
    TextOut(C_Menu_HPOSI,rect_ypos+C_MenuHigh*8+3,1,5,C_CCHAR_VDOT,(u_char*)_Message_Dis[MGetLanguage()][4],4);   //探头1
    TextOut(C_Menu_HPOSI,rect_ypos+C_MenuHigh*9+3,1,5,C_CCHAR_VDOT,(u_char*)_Message_Dis[MGetLanguage()][5],4);   //频带1
    TextOut(C_Menu_HPOSI,rect_ypos+C_MenuHigh*10+3,1,5,C_CCHAR_VDOT,(u_char*)_Message_Dis[MGetLanguage()][6],4);   //抑制1

/*
    xpos = 378;
    ypos = 8;
    epText = (u_char *)_KeyLock+MGetKeyLock()*32;
    cpText = (u_short *)(_KeyLock+MGetKeyLock()*32);
    for (i = 0; i < 1; i++)
    {
#if C_DEVLIB != 23
        UserECharOut(xpos + HoriOffsetScreen, ypos , (char*)(epText+i*16)) ;
        UserECharOut(xpos + HoriOffsetScreen, ypos +8, (char*)(epText+8+i*16)) ;
        UserECharOut(xpos + HoriOffsetScreen+8, ypos , (char*)(epText+1*16)) ;
        UserECharOut(xpos + HoriOffsetScreen+8, ypos +8, (char*)(epText+8+1*16)) ;
#else
        MUserDotsOut(xpos,ypos,(u_char*)(epText+i*8),8);
        MUserDotsOut(xpos,ypos+8,(u_char*)(epText+i*8+8),8);
        MUserDotsOut(xpos+8,ypos,(u_char*)(epText+i*16+16),8);
        MUserDotsOut(xpos+8,ypos+8,(u_char*)(epText+i*16+24),8);
#endif
        xpos += 8;
    }//显示键盘锁
*/
    xpos = 512;//432;
    ypos = C_COORVPOSI + C_COORHEIGHT-35;//7;
    for(i=0;i<4;i++)
    {
        switch (Function.funcMenu[i])
        {
            case C_FUNC_INIT:             //初始化
                epText=(u_char *)_init;
                        break;
            case C_FUNC_ENVELOPE:	          //回波包络
                epText=(u_char *)_envelope;
                        break;
            case C_FUNC_ECHOMAX:	          //峰值记忆
                epText=(u_char *)_echomax;
                        break;
            case C_FUNC_DACGATE:	          //DAC门
                epText=(u_char *)_DAC;
                        break;
            case C_FUNC_DACADJUST:            //深度补偿
                epText=(u_char *)_TCG;
                        break;
            case C_FUNC_GATEEXPEND:           //门内展宽
                epText=(u_char *)_expend;
                        break;
            case C_FUNC_GATEALARM:            //门内报警
                epText=(u_char *)_alarm;
                        break;
            case C_FUNC_DATA:		          //数据处理
                epText=(u_char *)_Data;
                        break;
            case C_FUNC_FREEZ:	              //波形冻结
                epText=(u_char *)_Freeze;
                        break;
            case C_FUNC_FILL:	              //实心波
                epText=(u_char *)_Fill;
                        break;
            default:
            break;
        }
      //  PageDisp(xpos-8+32*i,ypos,epText);
        IcoDotDis(xpos-8+32*i,ypos,epText);
        switch (Function.funcMenu[i])
        {
            case C_FUNC_INIT:             //初始化
                        break;
            case C_FUNC_ENVELOPE:	          //回波包络
                if (MGetFunctionMode(C_ECHO_ENVELOPE))
                {
                 //   InvertBytes(xpos-8+32*i,ypos, 1);
                    MInvertWindow( xpos-8+32*i,ypos, xpos-8+32*i+24,ypos+24) ;
                }
                        break;
            case C_FUNC_ECHOMAX:	          //峰值记忆
                if (MGetFunctionMode(C_ECHOMAX_MEMORY))
                {
                   // InvertBytes(xpos-8+32*i,ypos, 1);
                    MInvertWindow( xpos-8+32*i,ypos, xpos-8+32*i+24,ypos+24) ;
                }
                        break;
            case C_FUNC_DACGATE:	          //DAC门
                if (MGetFunctionMode(C_DAC_GATE))
                {
                   // InvertBytes(xpos-8+32*i,ypos, 1);
                    MInvertWindow( xpos-8+32*i,ypos, xpos-8+32*i+24,ypos+24) ;
                }
                        break;
            case C_FUNC_DACADJUST:            //深度补偿
                if (MGetFunctionMode(C_DEPTH_COMPENSATE))
                {
                   // InvertBytes(xpos-8+32*i,ypos, 1);
                    MInvertWindow( xpos-8+32*i,ypos, xpos-8+32*i+24,ypos+24) ;
                }
                        break;
            case C_FUNC_GATEEXPEND:           //门内展宽
              //  if (MGetFunctionMode(C_WAVE_EXPEND))
                {
              //      InvertBytes(xpos-8+32*i,ypos, 1);
                }
                        break;
            case C_FUNC_GATEALARM:            //门内报警
                if (MGetFunctionMode(C_GATE_ALARM))
                {
                   // InvertBytes(xpos-8+32*i,ypos, 1);
                    MInvertWindow( xpos-8+32*i,ypos, xpos-8+32*i+24,ypos+24) ;
                }
                        break;
            case C_FUNC_DATA:		          //数据处理
                        break;
            case C_FUNC_FREEZ:	              //波形冻结
                        break;
            case C_FUNC_FILL:	              //实心波
                if (MGetFill())
                {
                   // InvertBytes(xpos-8+32*i,ypos, 1);
                    MInvertWindow( xpos-8+32*i,ypos, xpos-8+32*i+24,ypos+24) ;
                }
                break;
            default:
            break;
        }
    }

    ////////////
    xpos = C_CHANNEL_HPOSI;
    ypos = C_CHANNEL_VPOSI+6;
    MSetColor(C_CR_PARA);
    MEraseWindow(xpos-8, C_CHANNEL_VPOSI+1, xpos+C_CCHAR_HDOT-8+C_ECHAR_HDOT, C_CHANNEL_VPOSI+30) ;
    Write_Number(xpos,ypos ,Channel,2,0,0);
 /*   if(Channel >= 10)
        Write_Number(xpos,ypos +4,Channel,2,0,0);
    else
    {
        Channel = CH(Channel+316);		//316为0的区位码
        cpText = &Channel;
        char_row = 1;
        CMenuOut(xpos,ypos,cpText,char_len,char_row,char_height);
    }	//以上为写通道号
*/
    EMenuOut(C_BASEGAIN_HPOSI + 2 * C_CCHAR_HDOT+16,C_BASEGAIN_VPOSI-28 ,"dB",2,1,C_ECHAR_VDOT);
    MSetColor(C_CR_MENU);

        EMenuOut(C_RANGE_HPOSI+10 + 4 * C_ECHAR_HDOT,C_RANGE_VPOSI-30,"( )",3,1,C_ECHAR_VDOT);
    if( MGetUnitType() > 0)
    {
        //单位inch
        EMenuOut(C_RANGE_HPOSI+16 + 4 * C_ECHAR_HDOT,C_RANGE_VPOSI-26+C_ECHAR_VDOT,"in",2,1,C_ECHAR_VDOT);
    }
    else
    {
        EMenuOut(C_RANGE_HPOSI+16 + 4 * C_ECHAR_HDOT,C_RANGE_VPOSI-26+C_ECHAR_VDOT,"mm",2,1,C_ECHAR_VDOT);
    }

    MSetColor(C_CR_PARA);
    switch (MGetScaleMode() )
    {
    case 0:
        Write_Ascii(C_RANGE_HPOSI+7 + 5 * C_ECHAR_HDOT,C_RANGE_VPOSI-26,'Y');
        break;
    case 1:
        Write_Ascii(C_RANGE_HPOSI+7 + 5 * C_ECHAR_HDOT,C_RANGE_VPOSI-26,'X');
        break;
    default:
        Write_Ascii(C_RANGE_HPOSI+7 + 5 * C_ECHAR_HDOT,C_RANGE_VPOSI-26,'S');
        break;
    }
    //声程的单位
    if(MGetLanguage()==0)
    {
        if (MGetAngle(0)>=C_MAX_ANGLE)
        {
            TextOut(C_RANGE_HPOSI+2*C_CCHAR_VDOT , C_RANGE_VPOSI+C_MenuHigh ,1,4,10,(char *)_infinity[MGetLanguage()][0],0);
        }
        else
        {
            Write_Number(C_RANGE_HPOSI+2*C_CCHAR_VDOT, C_RANGE_VPOSI+C_MenuHigh+4 , (MGetAngle(1) + 50)/100, 4,2,0);
        }
    }
    else
    {
        Write_Number(C_RANGE_HPOSI+3*C_CCHAR_HDOT-5, C_RANGE_VPOSI+C_MenuHigh+4 ,MGetAngle(0),4,1,0);	//angle
       // Write_Ascii(C_RANGE_HPOSI+3*C_CCHAR_HDOT+38, C_RANGE_VPOSI+C_MenuHigh+4 ,ANGLE);
    }

    //K值
    xpos = C_CHANNEL_HPOSI;
    ypos =  C_CHANNEL_VPOSI+C_MenuHigh*8;
    WriteProbe(MGetProbeMode()-0, xpos+C_CCHAR_HDOT, ypos+4);
    //写探头类型

    MSetColor(C_CR_MENU);
    ypos = C_CHANNEL_VPOSI+C_MenuHigh*9;
 //   epText = (u_char*)_ParaMenu_AD3_1 + ( (MGetBandMode()-0)*9+3 );
    MSetColor(C_CR_PARA);
 //   EMenuOut(xpos,ypos,epText,3,1,C_CCHAR_VDOT);	//	频带宽度：2.5\5.0\10\wide
    MEraseWindow(xpos, ypos, xpos+C_CCHAR_HDOT*3, ypos+24) ;
            epText = _ParaMenu_AD3[MGetLanguage()][MGetBandMode()]+2;
            TextOut(xpos,ypos, 1, 4,C_CCHAR_VDOT, epText, 4);
    MSetColor(C_CR_MENU);
    xpos = C_CHANNEL_HPOSI+C_CCHAR_HDOT;
    ypos = C_CHANNEL_VPOSI+C_MenuHigh*10+4;
    i = ( MGetReject() + 1)/2;
    MSetColor(C_CR_PARA);
    if( i < 10)
    {
        METextOut(xpos,ypos,"   ",3);
        Write_Number(xpos+4,ypos,i,1,0,0);
        MSetColor(C_CR_MENU);
        METextOut(xpos+C_ECHAR_HDOT+4,ypos,"%",1);
    }
    else
    {
        Write_Number(xpos,ypos,i,2,0,0);
        MSetColor(C_CR_MENU);
        METextOut(xpos+C_ECHAR_HDOT*2,ypos,"%",1);
    }
}

void WriteProbe(int type, int xpos, int ypos)
{
    int i,v,h;
//	int x,y;
    u_char *probe;


    probe = (u_char *)_ProbeFace + type * 32;

    v = xpos;//+HoriOffsetScreen;
    h = ypos + VertOffsetScreen;


    for (i = 0; i < 2; i++)
    {
#if C_DEVLIB != 23
        UserECharOut(v+HoriOffsetScreen, h, (char*)(probe+i*16)) ;
        UserECharOut(v+HoriOffsetScreen, h+8, (char*)(probe+8+i*16)) ;
#else
        MUserDotsOut(v,h,(u_char*)(probe+i*16),16);
#endif

        v+= 8;
    }
}

//各种参量调节函数
int MAdjustGain(u_int type,u_int mode,short low,short high)
/*调节增益,type=0系统增益=1补偿；mode=0立即此值越大自动调节所需时间越长,high高度*/
{
    u_int gatetype = 0;
    int gain ;
    int retvalue = C_FALSE;
    int step = 1;
    short gateamp;
    int TC_GateAmpMax = 230;
    int	TC_HighMax;
    int	TC_LowMin;
    //short preamp;

    if( MGetEchoMode() == C_RF_WAVE)
    {
        low=410;
        high=410;
    }

    if( low == high)
    {
        low--;
        high++;
    }


    TC_HighMax = (high*110)/100 ;
    TC_LowMin = (low*90)/100;
    TC_HighMax = ( TC_HighMax < TC_GateAmpMax ) ? TC_HighMax : TC_GateAmpMax;




    gateamp = Mabsi( MGetAmpMax(gatetype) );
    if( mode > 0)
    {
        if( gateamp > 100 )AdjustTime.GainPlusTime = 0;//波高大于100，小于140不自动调节波高
        if( gateamp <140 )AdjustTime.GainMinusTime = 0;
    }

    if ( AdjustTime.GainMinusTime >= ( mode * C_GAINMINUSTIME) || gateamp > TC_GateAmpMax)	/*增益减*/
    {
        gain = ( type == 0)? MGetBaseGain() : MGetSysGain();
        if( gateamp > high && gain >0)
        {
            while(gain > 0 && MGetSysGain() > 0 )
            {
                AdjustTime.amp_total = AdjustTime.amp_total2 = 0;
                AdjustTime.time = AdjustTime.time2 = 0;

                gateamp = Mabsi( MGetAmpMax(gatetype));
                if(gateamp <= high)break;
                ExpendTime(5);

                if(gateamp > TC_GateAmpMax)	step = 20;
                else if(gateamp > TC_HighMax)step = 5;
                else step = 1;

                if(type == 0)
                {
                    BaseGainMinus(step);
                    WriteBaseGain(C_BASEGAIN_HPOSI,C_BASEGAIN_VPOSI);
                    gain = MGetBaseGain();
                }
                else
                {
                    CompGainMinus(step);
                    WriteCompGain(C_COMPGAIN_HPOSI,C_COMPGAIN_VPOSI);
                    gain = MGetSysGain();
                }

                MParaRenovate(0);
                if(MGetKeyCode(5) == C_KEYCOD_RETURN)break;
//					if( Mabsi(pregain - gain) > 10 && Mabsi(preamp - gateamp) < 5 && preamp > 10 )break;
                //当增益变化大于1dB时，波高变化小于5且原波高大于10则退出
            }
            AdjustTime.GainMinusTime = 0;
            retvalue = C_TRUE;
        }

    }
    if ( retvalue == C_FALSE && ( AdjustTime.GainPlusTime >=  mode * C_GAINPLUSTIME ) )	/*增益加*/
    {
        gain = MGetSysGain();
        if( gateamp < low  && gain < C_MAX_BASEGAIN && gain < C_MAX_ADJUSTGAIN)
        {
            while( gateamp < low  && gain < C_MAX_ADJUSTGAIN)
            {
                ExpendTime(5);
                AdjustTime.amp_total = AdjustTime.amp_total2 = 0;
                AdjustTime.time = AdjustTime.time2 = 0;
                gateamp = Mabsi( MGetAmpMax(gatetype) ) ;
                if(gateamp < 10 )step = 20;
                else if( gateamp < TC_LowMin)step = 5;
                else step = 1;
                if(type == 0)
                {
                    BaseGainPlus(step);
                    WriteBaseGain(C_BASEGAIN_HPOSI,C_BASEGAIN_VPOSI);
                    MParaRenovate(0);
                }
                else
                {
                    CompGainPlus(step);
                    WriteCompGain(C_COMPGAIN_HPOSI,C_COMPGAIN_VPOSI);
                    MParaRenovate(0);
                }
                gain = MGetSysGain();
                if(MGetKeyCode(5) == C_KEYCOD_RETURN)break;
//				if( Mabsi(pregain - gain) > 10 && Mabsi(preamp - gateamp) < 5 && preamp > 10 )break;
            }
            AdjustTime.GainPlusTime = 0;
            retvalue = C_TRUE;
        }
    }
    if( retvalue == C_TRUE)MSetColor(C_CR_WAVE);
    if( MGetFunctionMode(C_ECHOMAX_MEMORY) )
    {
        ClearEnvelope();
    }
    return retvalue;
}

void WriteParaName(u_int ParaIndex) /*根据参量索引号写参量名*/
{
    u_short* pzText;
    MEraseWindow(0, 0, 18*C_ECHAR_HDOT, 24) ;
    DisplayPrompt(15);
    MSetColor(C_CR_MENU);
    TextOut(0,0, 1, 16,16, (u_char*)_ParaName[MGetLanguage()][ParaIndex], 0);
   // if (MGetLanguage())
    {
   //     TextOut(0,9, 1, 8,8, (u_char*)_ParaName[MGetLanguage()][ParaIndex]+8, 0);
    }
//	CMenuOut(0,0,pzText,4,1,24);	/*在指定位置根据每行字符数、行数、行高写菜单*/
}

int MSelectPrompt(int keycode)//在退出参数调节时判断是否要显示提示
{
    switch( keycode)
    {
    case C_KEYCOD_OFFSET:
    case C_KEYCOD_ANGLE:
    case C_KEYCOD_BASEGAIN:
    case C_KEYCOD_RANGE:
    case C_KEYCOD_SPEED:
    case C_KEYCOD_GATE:
    case C_KEYCOD_DELAY:
        return 1;
    default :
        DisplayPrompt(16);
        return 0;
    }
}

int ChannelChange(void)
{

    int xpos,ypos;
    int keycode,rotary;
    int number,deci_len;
    u_short Channel,cmax;
    int elapsedtime=GetElapsedTime();

    xpos = C_CHANNEL_HPOSI;
    ypos = C_CHANNEL_VPOSI;
    if(ChannelMax>10)cmax=1;
    else cmax=0;
    MInvertWindow(C_CHANNEL_HPOSI-8, C_CHANNEL_VPOSI+1, C_CHANNEL_HPOSI+C_CCHAR_HDOT-8+cmax*C_ECHAR_HDOT, C_CHANNEL_VPOSI+30) ;

    SystemParaStore();
    ChannelParaStore();
    //ExpendTime(30);
    while(1)
    {
        //	ExpendTime(10);
        keycode = MGetKeyCode(0);

        StorageData(keycode);

        //	if (GetElapsedTime() >= elapsedtime)
        {
            //		elapsedtime = GetElapsedTime() + 200;
            MParaRenovate(1);
        }

        if(keycode == C_KEYCODMAX)
        {
            rotary = MGetRotary(-1);
            if( rotary >= 128)
            {
                rotary = 0;
                keycode = C_KEYCOD_CONFIRM;
            }
            else if( rotary > 0 )keycode = C_KEYCOD_PLUS;
            else if( rotary < 0)keycode = C_KEYCOD_MINUS;
            else continue;
        }
        if(GetNoKeyTime() > 60 && MGetSystemMode() == 1)break;
        //在自动测试状态下未按超过60s则超出参量调节状态

        /*有按键*/
#if C_DEVTYPE == 14
        if(keycode == C_KEYCOD_RIGHT || keycode == C_KEYCOD_LEFT || keycode == C_KEYCOD_PLUS || keycode == C_KEYCOD_MINUS)
#else
        if(keycode == C_KEYCOD_PLUS || keycode == C_KEYCOD_MINUS )
#endif
        {
            MSetAcquisitionEnable(0,C_SETMODE_SET);
            MSetDacShowMode( 0,C_SETMODE_SAVE);
            Channel = MGetChannel();

#if C_DEVTYPE == 14
            if(keycode == C_KEYCOD_RIGHT || keycode == C_KEYCOD_PLUS )
#else
            if(keycode == C_KEYCOD_PLUS )
#endif
            {
                Channel++;
            }
            else
            {
#if C_MAKELIB == 1
                if( Channel == 0)Channel = ChannelMax/2;
#else
                if( Channel == 0)Channel = ChannelMax;
#endif
                Channel--;
            }
#if C_MAKELIB == 1
            if( Channel >= ChannelMax/2)Channel = 0;
#else
            if( Channel >= ChannelMax)Channel = 0;
#endif

            SystemParaRestore();
            MSetChannel( Channel,C_SETMODE_SETSAVE);
            g_UDiskInfo.DataHeaderMark = 1;
            SystemParaStore();
            ChannelParaRestore();
            MSetAcquisitionEnable(1,C_SETMODE_SET);
            MSetAcquisition(1);
            MActivateDac(MGetFunctionMode(C_DEPTH_COMPENSATE) ) ;
            MSetSystem();
            MChannelChange();
          //  ScreenRenovate();	/*屏幕刷新*/
            MSaveSystemElapsedTime( GetElapsedTime() / (1000*60), C_TIME_MINUTE );
            DrawDac(0);
            ClearEnvelope();
            MInvertWindow(C_CHANNEL_HPOSI-8, C_CHANNEL_VPOSI+1, C_CHANNEL_HPOSI+C_CCHAR_HDOT-8+cmax*C_ECHAR_HDOT, C_CHANNEL_VPOSI+30) ;
        }
        else
        {
            g_UDiskInfo.DataHeaderMark = 1;
            if((ChannelMax<=10 && keycode == C_KEYCOD_CHANNEL) || keycode == C_KEYCOD_CONFIRM)
            {
                //MInvertWindow(C_CHANNEL_HPOSI, C_CHANNEL_VPOSI-2, C_CHANNEL_HPOSI+C_CCHAR_HDOT, C_CHANNEL_VPOSI+20*3-2) ;
                ///if( MGetParaLock(1,C_LOCK_CHANNEL) != 1)
                {
                    ChannelMenu();
                    return -1;
                }
            }
            else if (keycode == C_KEYCOD_CHANNEL)
            {
                Channel = MGetChannel();
                number=0;
                deci_len=0;
                MInvertWindow(C_CHANNEL_HPOSI-8, C_CHANNEL_VPOSI+1, C_CHANNEL_HPOSI+C_CCHAR_HDOT-8+cmax*C_ECHAR_HDOT, C_CHANNEL_VPOSI+30) ;
                MEraseWindow(C_CHANNEL_HPOSI-8,C_CHANNEL_VPOSI+1, C_CHANNEL_HPOSI+C_CCHAR_HDOT-8+cmax*C_ECHAR_HDOT, C_CHANNEL_VPOSI+30);
                while(1)
                {
                    if( Input_Number(xpos-8,ypos+4,&number,2,&deci_len,0) != 1)
                    {
                        number=Channel;
                        break;
                    }
                    else
                    {
                        if( number < ChannelMax)
                        {
                            break;
                        }
                        else	DisplayPrompt(4);
                    }
                }

               // SystemParaRestore();
                Channel = number;
                MSetChannel( Channel,C_SETMODE_SETSAVE);
                g_UDiskInfo.DataHeaderMark = 1;
                SystemParaStore();
                ChannelParaRestore();
                MSetAcquisitionEnable(1,C_SETMODE_SET);
                MActivateDac(MGetFunctionMode(C_DEPTH_COMPENSATE) ) ;
                MSetSystem();
                ScreenRenovate();	//屏幕刷新
                MSaveSystemElapsedTime( GetElapsedTime() / (1000*60), C_TIME_MINUTE );
                DrawDac(0);
                ClearEnvelope();
                MInvertWindow(C_CHANNEL_HPOSI-8, C_CHANNEL_VPOSI+1, C_CHANNEL_HPOSI+C_CCHAR_HDOT-8+cmax*C_ECHAR_HDOT, C_CHANNEL_VPOSI+30) ;
            }
            else if(keycode < C_KEYCODMAX && keycode >=0)
            {
                MSelectPrompt(keycode);
                MInvertWindow(C_CHANNEL_HPOSI-8, C_CHANNEL_VPOSI+1, C_CHANNEL_HPOSI+C_CCHAR_HDOT-8+cmax*C_ECHAR_HDOT, C_CHANNEL_VPOSI+30) ;
                return keycode;
            }
        }
    }
    MInvertWindow(C_CHANNEL_HPOSI-8, C_CHANNEL_VPOSI+1, C_CHANNEL_HPOSI+C_CCHAR_HDOT-8+cmax*C_ECHAR_HDOT, C_CHANNEL_VPOSI+30) ;
    return keycode;
}

int BaseGainPlus(int step)		/*增益根据步长增加*/
{
    int	retvalue =  C_TRUE;

    int basegain = MGetBaseGain();
    int elsegain = MGetSurfGain() + MGetCompGain();
    if( step < 0)BaseGainMinus( -1 * step);
    else
    {
        basegain += step;
        if (basegain + elsegain > C_MAX_BASEGAIN)
        {
            basegain = C_MAX_BASEGAIN - elsegain;
            DisplayPrompt(3);
            retvalue =  C_FALSE;
        }

        MSetBaseGain(basegain,C_SETMODE_SETSAVE) ;
    }
    return retvalue;
}

int BaseGainMinus(int step)		/*增益根据步长*/
{
    int	retvalue =  C_TRUE;

    int basegain = MGetBaseGain();
    int elsegain = MGetSurfGain() + MGetCompGain();

    if( step < 0)BaseGainPlus( -1 * step);
    else
    {
        if (basegain < step || (basegain+elsegain < step) )
        {
            basegain = step;
            DisplayPrompt(3);
            retvalue =  C_FALSE;
        }

        basegain -= step;
        if (basegain + elsegain < 0 )basegain = 0 - elsegain ;
        MSetBaseGain(basegain,C_SETMODE_SETSAVE) ;
        ClearEnvelope();	//清包络或峰值
    }
    return retvalue;
}

int CompGainPlus(int step)		/*增益根据步长增加*/
{
    int	retvalue =  C_TRUE;
    if( step < 0)CompGainMinus( -1 * step);
    else
    {
        if( step == 0)
        {
            //	MAdjustGain(1,0,120,120);
        }
        else
        {
            int compgain = MGetCompGain() ;
            int elsegain = MGetSurfGain() + MGetBaseGain();

            compgain += step;
            if (compgain + elsegain > C_MAX_BASEGAIN)
            {
                compgain = C_MAX_BASEGAIN - elsegain;
                DisplayPrompt(3);
                retvalue =  C_FALSE;
            }
            MSetCompGain(compgain,C_SETMODE_SETSAVE) ;
        }

    }
    return retvalue;
}

int CompGainMinus(int step)		/*增益根据步长*/
{
    int	retvalue =  C_TRUE;
    int compgain = MGetCompGain() ;
    int elsegain = MGetSurfGain() + MGetBaseGain();

    if( step < 0)CompGainPlus( -1 * step);
    else
    {
        if( step == 0)
        {
            //	compgain = 0;
        }
        else
        {
            if (compgain+elsegain < step)
            {
                compgain = step - elsegain;
                DisplayPrompt(3);
                retvalue =  C_FALSE;
            }

            compgain -= step;
        }
        MSetCompGain(compgain,C_SETMODE_SETSAVE) ;
        ClearEnvelope();	//清包络或峰值
    }
    return retvalue;
}

int GainChange(void)
{
    int retvalue = C_TRUE;
    int keycode,rotary;
    //int	prekeycode = C_KEYCODMAX;
    int step = MGetParaStep(C_BASEGAIN);
    //int type = MGetParaSort(C_BASEGAIN);	//代改参数0系统，1补偿
    int type ;
    //int inv_xpos,inv_ypos;
    u_int time = 20;
    int elapsedtime= GetElapsedTime();

    if( MGetEquivalentDays(1) > 0 )type = 0;
    else type = MGetParaSort(C_BASEGAIN);	//代改参数0系统，1补偿;
    ClearEnvelope();	//清包络或峰值

    if(type == 0)
    {
        if(step == 0)step = ParaStepChange(C_BASEGAIN);	//系统增益步长不设为0
        InvertBytes(C_BASEGAIN_HPOSI, C_BASEGAIN_VPOSI, 4) ;
    }
    else
    {
        //扫查增益开始步长固定0
        step = 0;
        InvertBytes(C_COMPGAIN_HPOSI, C_COMPGAIN_VPOSI, 5) ;
    }

    if( MGetStdMode() == C_STD_AWS_NUM + 3)type = 1;

    while(1)
    {
        if(type == 0)
            WriteParaName(C_BASEGAIN);
        else
            WriteParaName(C_COMPGAIN);

        Write_Number((4+MGetLanguage()*5)*C_CCHAR_HDOT,0,step,3,1,0);

        while(1)
        {
            keycode = MGetKeyCode(0);
            if(keycode != C_KEYCODMAX)
            {
                g_UDiskInfo.DataHeaderMark = 1;
            }
            StorageData(C_KEYCODMAX);
            if(GetNoKeyTime() > 60 && MGetSystemMode() == 1)return retvalue;
            //在自动测试状态下未按超过60s则超出参量调节状态

            //	if (GetElapsedTime() >= elapsedtime)
            {
                //	elapsedtime = GetElapsedTime() + 200;
                MParaRenovate(1);
            }
            if(keycode == C_KEYCODMAX)
            {
#if C_ROTARY > 0
                rotary = MGetRotary(-1);
                if( rotary >= 128)
                {
                    rotary = 0;
                    keycode = C_KEYCOD_CONFIRM;
                }
                else if( rotary > 0 )
                {
                    keycode = C_KEYCOD_PLUS;
                }
                else if( rotary < 0)
                {
                    keycode = C_KEYCOD_MINUS;
                    rotary = (-1) * rotary;
                }
                else continue;
#else

                continue;
#endif
            }
            else rotary = 1;
            time = 10;
///			ExpendTime(time);
            ///	if(keycode < C_KEYCODMAX && keycode >=0)
            {
                /*有按键*/
                time = C_KEYDELAY;
#if C_DEVTYPE == 14
                if(keycode == C_KEYCOD_RIGHT || keycode == C_KEYCOD_LEFT || keycode == C_KEYCOD_PLUS || keycode == C_KEYCOD_MINUS)
#else
                if(keycode == C_KEYCOD_PLUS || keycode == C_KEYCOD_MINUS )
#endif
                {
                    g_UDiskInfo.DataHeaderMark = 1;
                    if(MGetFunctionMode(C_DAC_GATE))MDrawGate(0,0,-1,0);
                    if(type == 0)DrawDac(1);

                    do
                    {

                        if( step == 1)
                        {
                            ///if( rotary < 3)rotary = 1;
                            ///else if( rotary < 8)rotary = 10;
                            ///else rotary = 20;
                        }
                        else rotary = step;
                        if( MGetTestStatus(C_TEST_DAC) == 0 && MGetTestStatus(C_TEST_AVG) == 0)
                        {
                            if(time > 20)time -= 5;
                            else time -= 3;
                            if(time < 5)time = 5;
                            if(step > 1)time = 20;
                        }
                        else
                        {
                            if( type == 0 )time = 5;
                        }
                        ExpendTime(time);

                        if(type == 0)
                        {
#if C_DEVTYPE == 14
                            if(keycode == C_KEYCOD_RIGHT || keycode == C_KEYCOD_PLUS)
#else
                            if(keycode == C_KEYCOD_PLUS)
#endif
                                BaseGainPlus(rotary);
                            else BaseGainMinus(rotary);

                            WriteBaseGain(C_BASEGAIN_HPOSI,C_BASEGAIN_VPOSI);
                            InvertBytes(C_BASEGAIN_HPOSI, C_BASEGAIN_VPOSI, 4) ;
                        }
                        else
                        {
#if C_DEVTYPE == 14
                            if(keycode == C_KEYCOD_RIGHT || keycode == C_KEYCOD_PLUS)
#else
                            if(keycode == C_KEYCOD_PLUS)
#endif
                            {
                                if( step == 0)
                                {
                                    MAdjustGain(1,0,MGetAmpStdMax(),MGetAmpStdMax());
                                }
                                else CompGainPlus(rotary);
                            }
                            else
                            {
                                if( step == 0)MSetCompGain(0,C_SETMODE_SETSAVE);
                                else CompGainMinus(rotary);
                            }

                            WriteCompGain(C_COMPGAIN_HPOSI,C_COMPGAIN_VPOSI);
                            InvertBytes(C_COMPGAIN_HPOSI, C_COMPGAIN_VPOSI, 5);
                        }
#if C_ROTARY > 0
                        //;rotary = MGetKeyCode(0);
                        rotary = MGetKey2();
                        if(rotary == C_KEYCODMAX)
                        {
                            rotary = MGetRotary(-1);
                            if( rotary >= 128)
                            {
                                rotary = 0;
                                keycode = C_KEYCOD_CONFIRM;
                            }
                            else if( rotary > 0 )
                            {
                                keycode = C_KEYCOD_PLUS;
                            }
                            else if( rotary < 0)
                            {
                                keycode = C_KEYCOD_MINUS;
                                rotary = -1 * rotary;
                            }
                            else break;
                        }
                        else rotary = 1;
                        if( keycode != rotary )break;
#else
                        //if( keycode != MGetKeyCode(0) )break;
                        if( keycode != MGetKey2() )break;
#endif
                    }
                    while( true );

                    if(type == 0)DrawDac(0);
                    if( MGetFunctionMode(C_DAC_GATE) == 1)
                    {
                        DacGateInit();
                        //;MGateDraw();
                        MDrawGate(0,0,-1,0);//改为只变A门,即DAC门
                    }
                    continue;
                }
                else
                {
                    if(keycode == C_KEYCOD_BASEGAIN)
                    {
                        if(type == 0 )step = ParaStepChange(C_BASEGAIN);	//系统增益步长不设为0
                        else if( type == 1 )step = ParaStepChange(C_COMPGAIN);	//设置待调节参量步长
                        ExpendTime(20);
                    }
                    else if(keycode == C_KEYCOD_CONFIRM )//&& !MGetSystemMode() )	//设置成测试状态)
                    {
                        MSetParaSort(type + 1,C_BASEGAIN);
                        type = MGetParaSort(C_BASEGAIN);	//代改参数0系统，1补偿
                        if(type == 0 )step = MGetParaStep(C_BASEGAIN);
                        if(type == 1 )step = 0;//变为调节增益时步长改为0
                        InvertBytes(C_BASEGAIN_HPOSI, C_BASEGAIN_VPOSI, 4) ;
                        InvertBytes(C_COMPGAIN_HPOSI, C_COMPGAIN_VPOSI, 5) ;
                        ExpendTime(20);
#if C_ROTARY > 0
                        MGetRotary(-1);
#endif

                    }
                    /*			else if(keycode == C_KEYCOD_CONFIRM && MGetSystemMode() && type != 0)	//设置成测试状态)
                    			{
                    				MSetParaSort(0,C_BASEGAIN);
                    				type = 0;	//代改参数0系统，1补偿
                    				step = MGetParaStep(C_BASEGAIN);
                    				InvertBytes(C_BASEGAIN_HPOSI, C_BASEGAIN_VPOSI, 4) ;
                    				InvertBytes(C_COMPGAIN_HPOSI, C_COMPGAIN_VPOSI, 5) ;
                    				ExpendTime(20);
                    				#if C_ROTARY > 0
                    					 MGetRotary(-1);
                    				#endif
                    			}
                    */			else
                    {
                        MSelectPrompt(keycode);
                        retvalue = false;
                    }
                    break;
                }
                break;
            }
        }

        if( !retvalue ) break;
    }

    if(type == 0)
        WriteBaseGain(C_BASEGAIN_HPOSI,C_BASEGAIN_VPOSI);
    else
        WriteCompGain(C_COMPGAIN_HPOSI,C_COMPGAIN_VPOSI);

    if(keycode == C_KEYCOD_RETURN || keycode == C_KEYCOD_CONFIRM)
    {
        keycode = -1;
        DisplayPrompt(16);
    }
    else DisplayPrompt(15);
    ClearEnvelope();	//清包络或峰值
    return keycode;
}

int RejectChange(void)
{
    int retvalue = C_TRUE;
    int keycode,rotary;
    int step = 1;
    u_int time = C_KEYDELAY;
    ClearEnvelope();	//清包络或峰值

    WriteParaName(C_REJECT);
    EMenuOut(C_CCHAR_HDOT * (4+MGetLanguage()*4), 0,"%",1,1,12);
    ExpendTime(30);
    do
    {

        WriteReject((4+MGetLanguage()*1)*C_CCHAR_HDOT,0);
        SysParaDisp();
        while(1)
        {
            time=C_KEYDELAY;
            //	ExpendTime(time);
            MParaRenovate(1);
            keycode = MGetKeyCode(0);
            if(keycode != C_KEYCODMAX)
            {
                g_UDiskInfo.DataHeaderMark = 1;
            }
            StorageData(C_KEYCODMAX);

            if(GetNoKeyTime() > 60 && MGetSystemMode() == 1)return -1;
            //在自动测试状态下未按超过60s则超出参量调节状态
            if(keycode == C_KEYCODMAX)
            {
#if C_ROTARY > 0
                rotary = MGetRotary(-1);
                if( rotary >= 128)
                {
                    rotary = 0;
                    keycode = C_KEYCOD_CONFIRM;
                }
                else if( rotary > 0 )
                {
                    keycode = C_KEYCOD_PLUS;
                }
                else if( rotary < 0)
                {
                    keycode = C_KEYCOD_MINUS;
                    rotary = (-1) * rotary;
                }
                else continue;
#else
                continue;
#endif
            }
            else rotary = 1;

            ///if(keycode < C_KEYCODMAX && keycode >=0)
            {
                /*有按键*/
#if C_DEVTYPE == 14
                if(keycode == C_KEYCOD_RIGHT || keycode == C_KEYCOD_LEFT || keycode == C_KEYCOD_PLUS || keycode == C_KEYCOD_MINUS )
#else
                if(keycode == C_KEYCOD_PLUS || keycode == C_KEYCOD_MINUS )
#endif
                {
                    do
                    {
                        if(time > 20)time -= 5;
                        else time -= 3;
                        if(time < 3)time = 2;
                        ExpendTime(time);
#if C_DEVTYPE == 14
                        if(keycode == C_KEYCOD_RIGHT || keycode == C_KEYCOD_PLUS)
#else
                        if(keycode == C_KEYCOD_PLUS)
#endif
                            RejectPlus(rotary*step);
                        else RejectMinus(rotary*step);
                        WriteReject((4+MGetLanguage()*1)*C_CCHAR_HDOT,0);
                        ExpendTime(5);
                        MParaRenovate(1);
#if C_ROTARY > 0

                        rotary = MGetKeyCode(0);
                        if(rotary == C_KEYCODMAX)
                        {
                            if( rotary > 0 )
                            {
                                keycode = C_KEYCOD_PLUS;
                            }
                            else if( rotary < 0)
                            {
                                keycode = C_KEYCOD_MINUS;
                                rotary = -1 * rotary;
                            }
                            else break;
                        }
                        else rotary = 1;
                        if( keycode != rotary )break;
#else
                        if( keycode != MGetKeyCode(0) )break;
#endif
                    }
                    while( true );
                }
                else
                {
                    if( ( keycode == C_KEYCOD_REJECT || keycode == C_KEYCOD_CONFIRM) && MGetSystemMode() != 1)
                    {
                        keycode = DelayChange();
                        if( keycode == C_KEYCOD_CONFIRM)break;
                        else
                        {
                            MSelectPrompt(keycode);
                            SysParaDisp();
                            return keycode;
                        }
                    }
                    else
                    {
                        MSelectPrompt(keycode);
                        SysParaDisp();
                        return keycode;
                    }
                    break;
                }
                break;
            }
        }
        if( !retvalue ) break;
    }
    while (1);

    if(keycode == C_KEYCOD_RETURN || keycode == C_KEYCOD_CONFIRM)
    {
        keycode = -1;
        DisplayPrompt(16);
    }
    else DisplayPrompt(15);
    SysParaDisp();
    return keycode;
}

int RejectPlus(int step)
{
    u_int reject = MGetReject();

    if (reject >= C_MAX_REJECT)
    {
        DisplayPrompt(3);
        MSetReject(C_MAX_REJECT,C_SETMODE_SETSAVE) ;
        return C_FALSE;
    }
    reject++;
    MSetReject(reject,C_SETMODE_SETSAVE) ;
    return C_TRUE;
}

int RejectMinus(int step)
{
    u_int reject = MGetReject();

    if (reject == 0)
    {
        DisplayPrompt(3);
        MSetReject(0,C_SETMODE_SETSAVE) ;
        return C_FALSE;
    }
    reject--;
    MSetReject(reject,C_SETMODE_SETSAVE) ;
    return C_TRUE;
}

int OffsetChange(void)
{
    if( MGetParaLock(1,C_LOCK_OFFSET) == 1)
    {
        DisplayPrompt(16);
        return -1;
    }
    int retvalue = C_TRUE;
    int keycode,rotary;
    int step = 1;
    u_int time = C_KEYDELAY;
    ClearEnvelope();	//清包络或峰值

    WriteParaName(C_OFFSET);
    //EMenuOut(C_CCHAR_HDOT * 8, 0,"us",2,1,12);
    TextOut(C_CCHAR_HDOT * (8+MGetLanguage()*4), 5,1,4,16,(u_char*)_us[0][0],0);
    EMenuOut(C_CCHAR_HDOT * (8+MGetLanguage()*4)+C_ECHAR_HDOT+4, 0,"s",1,1,12);
    //ExpendTime(10);
    do
    {
        WriteRange(C_RANGE_HPOSI,C_RANGE_VPOSI);
        //InvertBytes(C_RANGE_HPOSI, C_RANGE_VPOSI, 5) ;

        WriteOffset((4+MGetLanguage()*4)*C_CCHAR_HDOT,0);
        while(1)
        {
            time=C_KEYDELAY;
            //	ExpendTime(time);
            MParaRenovate(1);
            keycode = MGetKeyCode(0);
            if(keycode != C_KEYCODMAX)
            {
                g_UDiskInfo.DataHeaderMark = 1;
            }
            StorageData(C_KEYCODMAX);
            if(keycode == C_KEYCODMAX)
            {
#if C_ROTARY > 0
                rotary = MGetRotary(-1);
                if( rotary >= 128)
                {
                    rotary = 0;
                    keycode = C_KEYCOD_CONFIRM;
                }
                else if( rotary > 0 )
                {
                    keycode = C_KEYCOD_PLUS;
                }
                else if( rotary < 0)
                {
                    keycode = C_KEYCOD_MINUS;
                    rotary = (-1) * rotary;
                }
                else continue;
#else
                continue;
#endif
            }
            else rotary = 1;
            if(GetNoKeyTime() > 60 && MGetSystemMode() == 1)return -1;
            //在自动测试状态下未按超过60s则超出参量调节状态
            //if(keycode < C_KEYCODMAX && keycode >=0)
            {
                /*有按键*/
#if C_DEVTYPE == 14
                if(keycode == C_KEYCOD_RIGHT || keycode == C_KEYCOD_LEFT || keycode == C_KEYCOD_PLUS || keycode == C_KEYCOD_MINUS)
#else
                if(keycode == C_KEYCOD_PLUS || keycode == C_KEYCOD_MINUS )
#endif
                {


                    do
                    {
                        if (GetSameKeyTime() > 20)
                        {
                            step=10;
                        }
                        else if (GetSameKeyTime() > 40)
                        {
                            step=20;
                        }
                        else
                        {
                            step=1;
                        }
                        if(time > 20)time -= 5;
                        else time -= 3;
                        if(time < 3)time = 2;
                        ExpendTime(time);
                        if( rotary < 3)rotary = 1;
                        else if( rotary < 8)rotary = 10;
                        else rotary = 20;
#if C_DEVTYPE == 14
                        if(keycode == C_KEYCOD_RIGHT || keycode == C_KEYCOD_PLUS)
#else
                        if(keycode == C_KEYCOD_PLUS)
#endif
                            OffsetPlus(rotary*step);
                        else OffsetMinus(rotary*step);
                        WriteOffset((4+MGetLanguage()*4)*C_CCHAR_HDOT,0);
                        ExpendTime(5);
                        MParaRenovate(1);
#if C_ROTARY > 0

                        rotary = MGetKeyCode(0);
                        if(rotary == C_KEYCODMAX)
                        {
                            rotary = MGetRotary(-1);
                            if( rotary >= 128)
                            {
                                rotary = 0;
                                keycode = C_KEYCOD_CONFIRM;
                            }
                            else if( rotary > 0 )
                            {
                                keycode = C_KEYCOD_PLUS;
                            }
                            else if( rotary < 0)
                            {
                                keycode = C_KEYCOD_MINUS;
                                rotary = -1 * rotary;
                            }
                            else break;
                        }
                        else rotary = 1;
                        if( keycode != rotary )break;
#else
                        if( keycode != MGetKeyCode(0) )break;
#endif
                    }
                    while( true );
                }
                else
                {
#if !(C_DEVTYPE == 14 ||C_DEVTYPE == 15||C_DEVTYPE == 1)
                    if( ( keycode == C_KEYCOD_OFFSET || keycode == C_KEYCOD_CONFIRM) && MGetSystemMode() != 1)
                    {
                        DisplayPrompt(15);
                        TestMenu();		//开始调校程序
                        DisplayPrompt(16);
                        return -1;
                    }
                    else
#endif
                    {
                        MSelectPrompt(keycode);
                        return keycode;
                    }
                    break;
                }
                break;
            }
        }
        if( !retvalue ) break;
    }
    while (1);

    if(keycode == C_KEYCOD_RETURN || keycode == C_KEYCOD_CONFIRM)
    {
        keycode = -1;
        DisplayPrompt(16);
    }
    else DisplayPrompt(15);
    return retvalue;
}

int OffsetPlus(int step)
{
    u_int offset = MGetOffset();

    if (offset >= C_MAX_OFFSET)
    {
        DisplayPrompt(3);
        MSetOffset(C_MAX_OFFSET,C_SETMODE_SETSAVE) ;
        return C_FALSE;
    }
    offset += step+0.6;
    MSetOffset(offset,C_SETMODE_SETSAVE) ;
    MSetTestStatus(C_TEST_OFFSET,0);	//设置测试状态,零点声速未测
    return C_TRUE;
}

int OffsetMinus(int step)
{
    u_int offset = MGetOffset();

    if (offset <= step)
    {
        DisplayPrompt(3);
        MSetOffset(0,C_SETMODE_SETSAVE) ;
        return C_FALSE;
    }
    offset -= step+0.5;
    MSetOffset(offset,C_SETMODE_SETSAVE) ;
    MSetTestStatus(C_TEST_OFFSET,0);	//设置测试状态,零点声速未测
    return C_TRUE;
}

int SpeedChange(void)
{
    if( MGetParaLock(1,C_LOCK_OFFSET) == 1)
    {
        STDMenu();		//开始调校程序
        DisplayPrompt(16);

        return -1;
    }

    int retvalue = C_TRUE;
    int keycode,rotary;
    u_int time = C_KEYDELAY;
    int step = MGetParaStep(C_SPEED);

    ClearEnvelope();	//清包络或峰值

    WriteParaName(C_SPEED);

    do
    {
        WriteSpeed((6+MGetLanguage()*3)*C_CCHAR_HDOT,0);
        Write_Ascii((4+MGetLanguage()*3)*C_CCHAR_HDOT,0,'-');
        Write_Number((4+MGetLanguage()*3)*C_CCHAR_HDOT+C_ECHAR_HDOT,0,step,1,0,0);
        WriteRange(C_RANGE_HPOSI,C_RANGE_VPOSI);

        while(1)
        {
            time = C_KEYDELAY;
            //	ExpendTime(time);
            MParaRenovate(1);
            keycode = MGetKeyCode(0);
            if(keycode != C_KEYCODMAX)
            {
                g_UDiskInfo.DataHeaderMark = 1;
            }
            StorageData(C_KEYCODMAX);
            if(keycode == C_KEYCODMAX)
            {
#if C_ROTARY > 0
                rotary = MGetRotary(-1);
                if( rotary >= 128)
                {
                    rotary = 0;
                    keycode = C_KEYCOD_CONFIRM;
                }
                else if( rotary > 0 )
                {
                    keycode = C_KEYCOD_PLUS;
                }
                else if( rotary < 0)
                {
                    keycode = C_KEYCOD_MINUS;
                    rotary = (-1) * rotary;
                }
                else continue;
#else
                continue;
#endif
            }
            else rotary = 1;
            if(GetNoKeyTime() > 60 && MGetSystemMode() == 1)return -1;
            //在自动测试状态下未按超过60s则超出参量调节状态
            //if(keycode < C_KEYCODMAX && keycode >=0)
            {
                /*有按键*/
#if C_DEVTYPE == 14
                if(keycode == C_KEYCOD_RIGHT || keycode == C_KEYCOD_LEFT || keycode == C_KEYCOD_PLUS || keycode == C_KEYCOD_MINUS)
#else
                if(keycode == C_KEYCOD_PLUS || keycode == C_KEYCOD_MINUS )
#endif
                {
                    do
                    {

                        if( step == 1)
                        {
                            ///	if( rotary <= 2)rotary = 1;
                            ///	else if( rotary <=8)rotary = 10;
                            ///	else rotary = 20;
                        }
                        else if( step == 2)
                        {
                            rotary = 50;
                        }
                        else
                        {
                            step = 1;
                            rotary = 1;
                        }
#if C_DEVTYPE == 14
                        if(keycode == C_KEYCOD_RIGHT || keycode == C_KEYCOD_PLUS)
#else
                        if(keycode == C_KEYCOD_PLUS)
#endif
                            SpeedPlus(rotary * step);
                        else SpeedMinus(rotary*step);

                        WriteSpeed((6+MGetLanguage()*3)*C_CCHAR_HDOT,0);
                        Write_Ascii((4+MGetLanguage()*3)*C_CCHAR_HDOT,0,'-');
                        Write_Number((4+MGetLanguage()*3)*C_CCHAR_HDOT+C_ECHAR_HDOT,0,step,1,0,0);

                        WriteRange(C_RANGE_HPOSI,C_RANGE_VPOSI);
                        WriteScale();
                        if(time > 20)time -= 5;
                        else time -= 3;
                        if(time < 3)time = 2;
                        ExpendTime(time);
#if C_ROTARY > 0
                        rotary = MGetKeyCode(0);
                        if(rotary == C_KEYCODMAX)
                        {
                            rotary = MGetRotary(-1);
                            if( rotary >= 128)
                            {
                                rotary = 0;
                                keycode = C_KEYCOD_CONFIRM;
                            }
                            else if( rotary > 0 )
                            {
                                keycode = C_KEYCOD_PLUS;
                            }
                            else if( rotary < 0)
                            {
                                keycode = C_KEYCOD_MINUS;
                                rotary = -1 * rotary;
                            }
                            else break;
                        }
                        else rotary = 1;
                        if( keycode != rotary )break;
#else
                        if( keycode != MGetKeyCode(0) )break;
#endif
                    }
                    while( true );

                }
                else
                {
                    if(keycode == C_KEYCOD_SPEED)
                    {
                        MKeyRlx();
                        step = ParaStepChange(C_SPEED);	//设置待调节参量步长
                    }
                    else if(  keycode == C_KEYCOD_CONFIRM )
                    {
                        DisplayPrompt(15);
                        STDMenu();		//开始调校程序
                        DisplayPrompt(16);

                        return -1;
                    }
                    else
                    {
                        MSelectPrompt(keycode);
                        return keycode;
                    }
                    break;
                }
                break;
            }
        }
        if( !retvalue ) break;
    }
    while (1);

    //InvertBytes(22*C_ECHAR_HDOT,0, 4) ;
    if(keycode == C_KEYCOD_RETURN || keycode == C_KEYCOD_CONFIRM)
    {
        keycode = -1;
        DisplayPrompt(16);
    }
    else DisplayPrompt(15);


    ClearEnvelope();	//清包络或峰值
    return retvalue;
}

int SpeedPlus(int step)
{
    u_int speed = MGetSpeed();

    speed += step;
    if (speed >= C_MAX_SPEED)
    {
        DisplayPrompt(3);
        speed = C_MAX_SPEED;
    }

    MSetSpeed(speed,C_SETMODE_SETSAVE);
    MSetTestStatus(C_TEST_OFFSET,0);	//设置测试状态,零点声速未测
    return C_TRUE;
}
int SpeedMinus(int step)
{
    u_int speed = MGetSpeed();

    if (speed < C_MIN_SPEED)
    {
        DisplayPrompt(3);
        return C_FALSE;
    }
    speed -= step;
    MSetSpeed(speed,C_SETMODE_SETSAVE) ;
    MSetTestStatus(C_TEST_OFFSET,0);	//设置测试状态,零点声速未测
    return C_TRUE;
}


int RangeChange(void)
{
    int retvalue = C_TRUE;
    //int keycode,prekeycode = C_KEYCODMAX;
    int keycode,rotary;
    int step = MGetParaStep(C_RANGE);
    int time = 15;

    ClearEnvelope();	//清包络或峰值
///	WriteParaName(C_RANGE + MGetScaleMode() );
    //InvertBytes(C_RANGE_HPOSI, C_RANGE_VPOSI, 5) ;

    do
    {
        WriteParaName(C_RANGE + MGetScaleMode() );
        Write_Ascii((4+MGetLanguage()*6)*C_CCHAR_HDOT,0,'-');
        Write_Number((4+MGetLanguage()*6)*C_CCHAR_HDOT+C_ECHAR_HDOT,0,step,1,0,0);
        InvertBytes(C_RANGE_HPOSI, C_RANGE_VPOSI, 5) ;

        while(1)
        {
            time = C_KEYDELAY;
            //	ExpendTime(time);
            keycode = MGetKeyCode(0);
            if(keycode != C_KEYCODMAX)
            {
                g_UDiskInfo.DataHeaderMark = 1;
            }
            StorageData(C_KEYCODMAX);
            MParaRenovate(1);
            if(keycode == C_KEYCODMAX)
            {
#if C_ROTARY > 0
                rotary = MGetRotary(-1);
                if( rotary >= 128)
                {
                    rotary = 0;
                    keycode = C_KEYCOD_CONFIRM;
                }
                else if( rotary > 0 )
                {
                    keycode = C_KEYCOD_PLUS;
                }
                else if( rotary < 0)
                {
                    keycode = C_KEYCOD_MINUS;
                    rotary = (-1) * rotary;
                }
                else continue;
#else
                continue;
#endif
            }
            else rotary = 1;
            if(GetNoKeyTime() > 60 && MGetSystemMode() == 1)return -1;
            //在自动测试状态下未按超过60s则超出参量调节状态
            ///if(keycode < C_KEYCODMAX && keycode >=0)
            {
                /*有按键*/
#if C_DEVTYPE == 14
                if(keycode == C_KEYCOD_RIGHT || keycode == C_KEYCOD_LEFT || keycode == C_KEYCOD_PLUS || keycode == C_KEYCOD_MINUS)
#else
                if(keycode == C_KEYCOD_PLUS || keycode == C_KEYCOD_MINUS )
#endif
                {
                    if(MGetFunctionMode(C_DAC_GATE))MDrawGate(0,0,-1,0);
                    DrawDac(1);

                    do
                    {

                        if( rotary < 3 || step == 2)rotary = 1;
#if C_DEVTYPE == 14
                        if(keycode == C_KEYCOD_RIGHT || keycode == C_KEYCOD_PLUS)
#else
                        if(keycode == C_KEYCOD_PLUS)
#endif
                            RangePlus(rotary*step);
                        else RangeMinus(rotary*step);


                        WriteRange(C_RANGE_HPOSI,C_RANGE_VPOSI);
                        //				InvertBytes(C_RANGE_HPOSI, C_RANGE_VPOSI, 5) ;
                        WriteScale();	//标度
                        if(time > 20)time -= 5;
                        else time -= 3;
                        if(time < 3)time = 2;
                        ExpendTime(time);
#if C_ROTARY > 0
                        rotary = MGetKeyCode(0);
                        if(rotary == C_KEYCODMAX)
                        {
                            rotary = MGetRotary(-1);
                            if( rotary >= 128)
                            {
                                rotary = 0;
                                keycode = C_KEYCOD_CONFIRM;
                            }
                            else if( rotary > 0 )
                            {
                                keycode = C_KEYCOD_PLUS;
                            }
                            else if( rotary < 0)
                            {
                                keycode = C_KEYCOD_MINUS;
                                rotary = -1 * rotary;
                            }
                            else break;
                        }
                        else rotary = 1;
                        if( keycode != rotary )break;
#else
                        if( keycode != MGetKeyCode(0) )break;
#endif

                    }
                    while( true );

                    DrawDac(0);
                    if( MGetFunctionMode(C_DAC_GATE) == 1)
                    {
                        DacGateInit();
                        MDrawGate(0,0,-1,0);
                    }
                }
                else if(keycode == C_KEYCOD_RANGE)
                {
                    MKeyRlx();
                    step = ParaStepChange(C_RANGE);	//设置待调节参量步长
                    InvertBytes(C_RANGE_HPOSI, C_RANGE_VPOSI, 5) ;
                }
                else if(keycode == C_KEYCOD_CONFIRM)
                {
                    //if( MGetAngle(0) == 0)MSetScaleMode(0,0);
                    //else MSetScaleMode(MGetScaleMode()+0,0);
                    MSetScaleMode(MGetScaleMode()+1,0);
                    u_char ch;
                    switch (MGetScaleMode() )
                    {
                    case 0:
                        Write_Ascii(C_RANGE_HPOSI+7 + 5 * C_ECHAR_HDOT,C_RANGE_VPOSI-26,'Y');
                        ch = ARROW3;
                        break;
                    case 1:
                        Write_Ascii(C_RANGE_HPOSI+7 + 5 * C_ECHAR_HDOT,C_RANGE_VPOSI-26,'X');
                        ch = ARROW2;
                        break;
                    default:
                        Write_Ascii(C_RANGE_HPOSI+7 + 5 * C_ECHAR_HDOT,C_RANGE_VPOSI-26,'S');
                        ch = ARROW1;
                        break;
                    }
                    if( MGetBGateMode() ==1 )
                    {
                        Write_Ascii(2*C_ECHAR_HDOT,C_COORVPOSI - C_ECHAR_VDOT-VertOffsetScreen-2,ch);
                        Write_Ascii(22*C_ECHAR_HDOT,C_COORVPOSI - C_ECHAR_VDOT-VertOffsetScreen-2,ch);
                    }
                    MParaRenovate(1);
                    WriteParaName(C_RANGE + MGetScaleMode() - 0);
                    WriteRange(C_RANGE_HPOSI,C_RANGE_VPOSI);
                    ExpendTime(20);
#if C_ROTARY > 0
                    MGetRotary(-1);
#endif
                }
                else
                {
                    WriteRange(C_RANGE_HPOSI,C_RANGE_VPOSI);
                    MSelectPrompt(keycode);
                    return keycode;
                }
                break;
            }
        }
        if( !retvalue ) break;
        WriteScale();	//标度

    }
    while (1);

    if(keycode == C_KEYCOD_RETURN || keycode == C_KEYCOD_CONFIRM)
    {
        keycode = -1;
        DisplayPrompt(16);
    }
    else DisplayPrompt(15);
    ;
    WriteRange(C_RANGE_HPOSI,C_RANGE_VPOSI);
    ClearEnvelope();	//清包络或峰值
    return retvalue;
}

int RangePlus(int step)		/*根据步长增加*/
{
    u_int range = MGetRange(1);
    u_int tempmaxrange,tempmaxrfrange,tempspeed;
    if (MGetUnitType() == 1)
    {
        tempmaxrange = 214000;
        tempmaxrfrange = C_MAX_RF_RANGE/0.254;
        tempspeed = CD_SPEED_2/0.254;
    }
    else
    {
        tempmaxrange = C_MAX_RANGE;
        tempmaxrfrange = C_MAX_RF_RANGE;
        tempspeed = CD_SPEED_2;
    }

    if (step == 2)
    {
        if (range >= 3000)
        {
            if( (range + 20)%1000 < 20 )range = (range + 20);	/*>300时以100为步长，如320-400-500*/
            range = ( range/1000 + 1) * 1000;
        }
        else
        {
            step = 1;
            while ( range + 5 > (u_int)_RANGEVALUE[step])
            {
                step++;
            }
            range = (u_int)_RANGEVALUE[step];
        }
    }
    else if( range <= 30000)
    {
        range = range + step*10;
    }
    else if( range <= 60000)
    {
        range = range + step*20;
    }
    else
    {
        range = range + step*50;
    }


    MSetRange(range,C_SETMODE_SETSAVE) ;

    if ( (  MGetEchoMode() != C_RF_WAVE && MGetRange(3) + MGetDelay(3) >= tempmaxrange ) || ( MGetEchoMode() == C_RF_WAVE && MGetRange(3) + MGetDelay(3) > tempmaxrfrange) )
    {
        int scale = MGetScaleMode();

        MSetScaleMode(2,0);
        if( MGetEchoMode() != C_RF_WAVE)MSetRange(tempmaxrange - MGetDelay(3), C_SETMODE_SETSAVE) ;
        else MSetRange(tempmaxrfrange - MGetDelay(3), C_SETMODE_SETSAVE) ;


        MSetScaleMode(scale,0);
        DisplayPrompt(3);
        return C_FALSE;
    }
    else if( MGetSpeed() < tempspeed * 110/100 && MGetProbeMode() == C_APROBE && MGetRange(3) + MGetDelay(3) >= tempmaxrange/2)
    {
        int scale = MGetScaleMode();

        MSetScaleMode(2,0);
        MSetRange(tempmaxrange/2 - MGetDelay(3), C_SETMODE_SETSAVE) ;

        MSetScaleMode(scale,0);
        DisplayPrompt(3);
        return C_FALSE;
    }

    return C_TRUE;
}

int RangeMinus(int step)		/*根据步长*/
{
    u_int range = MGetRange(0);
    u_int tempminrange,tempmintimerange;
    int Min_range=MGetSpeed()*2.5/1000+1;
    Min_range=Min_range*10;

    if (MGetUnitType() == 1)
    {
        tempminrange = Min_range/0.254;
        tempmintimerange = Min_range/0.254;
    }
    else
    {
        tempminrange = Min_range;
        tempmintimerange = Min_range;
    }


    if (range <= tempmintimerange)
    {
        MSetRange(tempmintimerange,C_SETMODE_SETSAVE) ;
        DisplayPrompt(3);
        return C_FALSE;
    }

    range = MGetRange(1);
    if (range <= tempminrange)
    {
        MSetRange(tempminrange,C_SETMODE_SETSAVE) ;
        DisplayPrompt(3);
        return C_FALSE;
    }

    if (step == 2 )
    {
        if (range > 3000)
        {
            if( range%1000 < 20 )range = ( range/1000 ) * 1000;;	/*>300时以100为步长，如320-400-500*/

            if( range%1000 == 0 )range = range - 1000;	/*>300时以100为步长，如320-400-500*/
            else range = ( range/1000 ) * 1000;
        }
        else
        {
            step = 1;	//暂用step作指针
            while ( range > (u_int)_RANGEVALUE[step])
            {
                step++;
            }
            range = (u_int)_RANGEVALUE[step -1];
            if (range <= tempmintimerange)
            {
                range= tempmintimerange;
            }
        }
    }
    else if( range <= 30000)
    {
        range = range - step*10;
    }
    else if( range <= 60000)
    {
        range = range - step*20;
    }
    else
    {
        range = range - step*50;
    }
//Write_Number(10,100,tempminrange,4,0,0);
    MSetRange(range,C_SETMODE_SETSAVE) ;
    return C_TRUE;
}

int DelayChange(void)
{
    int retvalue = C_TRUE;
    int keycode,rotary;
    int step = MGetParaStep(C_DELAY);
    u_int time = 20;

    ClearEnvelope();	//清包络或峰值

    WriteParaName(C_DELAY + MGetScaleMode() - 0);
    InvertBytes(0, C_COORVPOSI+C_COORHEIGHT+2, 5) ;
    do
    {
        Write_Ascii((4+MGetLanguage()*7)*C_CCHAR_HDOT,0,'-');
        Write_Number((4+MGetLanguage()*7)*C_CCHAR_HDOT+C_ECHAR_HDOT,0,step,1,0,0);

        while(1)
        {
            time = C_KEYDELAY;
            //	ExpendTime(time);
            MParaRenovate(1);
            keycode = MGetKeyCode(0);
            if(keycode != C_KEYCODMAX)
            {
                g_UDiskInfo.DataHeaderMark = 1;
            }
            StorageData(C_KEYCODMAX);
            if(keycode == C_KEYCODMAX)
            {
#if C_ROTARY > 0
                rotary = MGetRotary(-1);
                if( rotary >= 128)
                {
                    rotary = 0;
                    keycode = C_KEYCOD_CONFIRM;
                }
                else if( rotary > 0 )
                {
                    keycode = C_KEYCOD_PLUS;
                }
                else if( rotary < 0)
                {
                    keycode = C_KEYCOD_MINUS;
                    rotary = (-1) * rotary;
                }
                else continue;
#else
                continue;
#endif
            }
            else rotary = 1;

            if(GetNoKeyTime() > 60 && MGetSystemMode() == 1)return -1;
            //在自动测试状态下未按超过60s则超出参量调节状态

            ///	if(keycode < C_KEYCODMAX && keycode >=0)
            {
                /*有按键*/
#if C_DEVTYPE == 14
                if(keycode == C_KEYCOD_RIGHT || keycode == C_KEYCOD_LEFT || keycode == C_KEYCOD_PLUS || keycode == C_KEYCOD_MINUS )
#else
                if(keycode == C_KEYCOD_PLUS || keycode == C_KEYCOD_MINUS )
#endif
                {
                    if(MGetFunctionMode(C_DAC_GATE)) MGateDraw();
                    DrawDac(1);
                    do
                    {
                        if(time > 20)time -= 5;
                        else time -= 3;
                        if(time < 2)time = 1;
                        ExpendTime(time);
                        if( step == 2)rotary = 1;
                        if( rotary <= 3)rotary = 1;
                        //else rotary *= 2;
#if C_DEVTYPE == 14
                        if(keycode == C_KEYCOD_LEFT || keycode == C_KEYCOD_MINUS)
#else
                        if(keycode == C_KEYCOD_MINUS )
#endif
                            DelayMinus(rotary*step);
                        else DelayPlus(rotary*step);
                        WriteScale();
                        InvertBytes(0, C_COORVPOSI+C_COORHEIGHT+2, 5) ;

#if C_ROTARY > 0
                        rotary = MGetKeyCode(0);
                        if(rotary == C_KEYCODMAX)
                        {
                            rotary = MGetRotary(-1);
                            if( rotary >= 128)
                            {
                                rotary = 0;
                                keycode = C_KEYCOD_CONFIRM;
                            }
                            else if( rotary > 0 )
                            {
                                keycode = C_KEYCOD_PLUS;
                            }
                            else if( rotary < 0)
                            {
                                keycode = C_KEYCOD_MINUS;
                                rotary = -1 * rotary;
                            }
                            else break;

                            ///step = rotary;
                        }
                        else rotary = 1;
                        if( keycode != rotary )break;
#else
                        if( keycode != MGetKeyCode(0) )break;
#endif
                    }
                    while( true );

                    DrawDac(0);
                    if( MGetFunctionMode(C_DAC_GATE) == 1)
                    {
                        DacGateInit();
                        MGateDraw();
                    }
                }
                else if(keycode == C_KEYCOD_DELAY)
                {
                    MKeyRlx();
                    step = ParaStepChange(C_DELAY);	//设置待调节参量步长
                }
                else if(  keycode == C_KEYCOD_CONFIRM && MGetSystemMode() != 1)
                {
                    WriteScale();	//标度
                    ExpendTime(20);
#if C_ROTARY > 0
                    MGetRotary(-1);
#endif
                    keycode = RejectChange();
                    if( keycode == C_KEYCOD_CONFIRM)break;
                    else return keycode;
                }
                else
                {
                    MSelectPrompt(keycode);
                    WriteScale();	//标度
                    return keycode;
                }
                break;
            }
        }
        if( !retvalue ) break;
    }
    while (1);
    WriteScale();	//标度
    if(keycode == C_KEYCOD_RETURN || keycode == C_KEYCOD_CONFIRM)
    {
        keycode = -1;
        DisplayPrompt(16);
    }
    else DisplayPrompt(15);
    ;
    return keycode;
}

int DelayPlus(int step)		/*根据步长增加*/
{
    u_int delay = MGetDelay(1);	//得到当前标度下的延时距离
    if (step == 2 )
    {
        step = 0;
        delay += MGetRange(1)/2;	//当前标度下探测范围的一半
    }
    else
    {
        delay += step*2;
        if (MGetScaleMode()==1)
        {
            delay++;
        }
    }
    MSetScaleDelay(delay,C_SETMODE_SETSAVE) ;
    if( MGetSpeed() < CD_SPEED_2 * 110/100 && MGetProbeMode() == C_APROBE && MGetRange(3) + MGetDelay(3) >= C_MAX_RANGE/2)
    {
        int scale = MGetScaleMode();

        MSetScaleMode(2,0);
        MSetScaleDelay(C_MAX_RANGE/2 - MGetRange(3), C_SETMODE_SETSAVE) ;

        MSetScaleMode(scale,0);
        DisplayPrompt(3);
        return C_FALSE;
    }
    else if ( MGetRange(3) + MGetDelay(3) >= C_MAX_RANGE)
    {
        int scale = MGetScaleMode();

        MSetScaleMode(2,0);
        MSetScaleDelay(C_MAX_RANGE - MGetRange(3), C_SETMODE_SETSAVE) ;

        MSetScaleMode(scale,0);
        DisplayPrompt(3);
        return C_FALSE;
    }


    return C_TRUE;
}

int DelayMinus(int step)		/*根据步长*/
{
    int delay = MGetDelay(1);

    if( step == 2)
    {
        if(delay > MGetRange(1)/2 )
            delay -= MGetRange(1)/2;	//当前标度下探测范围的一半
        else delay = 0;
    }
    else
    {
        delay -= step*2;
    }

    if ( delay <= 0 )
    {
        DisplayPrompt(3);
        MSetDelay(0,C_SETMODE_SETSAVE) ;
        return C_FALSE;
    }

    MSetScaleDelay(delay,C_SETMODE_SETSAVE) ;
    return C_TRUE;
}

int AngleChange(void)
{
    if( MGetParaLock(1,C_LOCK_OFFSET) == 1)
    {
       // ColorMenu();
        DisplayPrompt(16);
        return -1;
    }
    int retvalue = C_TRUE;
    int keycode,rotary;
    int step = 1;
//	u_int angle = MGetAngle(0);
    u_int time = C_KEYDELAY;

    ClearEnvelope();	//清包络或峰值
    WriteParaName(C_ANGLE);
    //ExpendTime(30);
    do
    {
        if (MGetAngle(0)>=C_MAX_ANGLE)
        {
            TextOut((4+MGetLanguage()*8)*C_CCHAR_HDOT,0,1,4,10,(char *)_infinity[MGetLanguage()][0],0);
        }
        else
        {
            Write_Number((4+MGetLanguage()*8)*C_CCHAR_HDOT,0,( MGetAngle(1) + 50)/100,4,2,0);
        }
       // MChannelRenovate();
        DrawDac(1);
        WriteAngle((4+MGetLanguage()*4)*C_CCHAR_HDOT,0);
        Write_Ascii((4+MGetLanguage()*4)*C_CCHAR_HDOT + 4 * C_ECHAR_HDOT,0 ,ANGLE);
        WriteRange(C_RANGE_HPOSI,C_RANGE_VPOSI);

        //InvertBytes(C_RANGE_HPOSI, C_RANGE_VPOSI, 5) ;
        while(1)
        {
            time = C_KEYDELAY;
            //	ExpendTime(time);
            MParaRenovate(1);
            keycode = MGetKeyCode(0);
            if(keycode != C_KEYCODMAX)
            {
                g_UDiskInfo.DataHeaderMark = 1;
            }
            StorageData(C_KEYCODMAX);
            if(keycode == C_KEYCODMAX)
            {
#if C_ROTARY > 0
                rotary = MGetRotary(-1);
                if( rotary >= 128)
                {
                    rotary = 0;
                    keycode = C_KEYCOD_CONFIRM;
                }
                else if( rotary > 0 )
                {
                    keycode = C_KEYCOD_PLUS;
                }
                else if( rotary < 0)
                {
                    keycode = C_KEYCOD_MINUS;
                    rotary = -1 * rotary;
                }
                else continue;
#else
                continue;
#endif
            }
            else rotary = 1;
            if(GetNoKeyTime() > 60 && MGetSystemMode() == 1)return -1;
            //在自动测试状态下未按超过60s则超出参量调节状态
            //if(keycode != C_KEYCODMAX)BuzzerOn(1);
            ///if(keycode < C_KEYCODMAX && keycode >=0)
            {
                /*有按键*/
#if C_DEVTYPE == 14
                if(keycode == C_KEYCOD_RIGHT || keycode == C_KEYCOD_LEFT || keycode == C_KEYCOD_PLUS || keycode == C_KEYCOD_MINUS )
#else
                if(keycode == C_KEYCOD_PLUS || keycode == C_KEYCOD_MINUS )
#endif
                {
                    do
                    {
                        if(time > 20)time -= 5;
                        else time -= 3;
                        if(time < 3)time = 2;
                        ExpendTime(time);
                        if( rotary <= 2)rotary = 1;
                        else if( rotary <=8)rotary = 10;
                        else rotary = 20;
#if C_DEVTYPE == 14
                        if(keycode == C_KEYCOD_RIGHT || keycode == C_KEYCOD_PLUS )
#else
                        if(keycode == C_KEYCOD_PLUS)
#endif

                            AnglePlus(rotary*step);
                        else AngleMinus(rotary*step);

                        //WriteAngle(175,8);

                        if (MGetAngle(0)>=C_MAX_ANGLE)
                        {
                            TextOut((4+MGetLanguage()*8)*C_CCHAR_HDOT,0,1,4,10,(char *)_infinity[MGetLanguage()][0],0);
                        }
                        else
                        {
                            Write_Number((4+MGetLanguage()*8)*C_CCHAR_HDOT,0,( MGetAngle(1) + 50)/100,4,2,0);
                        }
                        //	MChannelRenovate();
                        WriteAngle((4+MGetLanguage()*4)*C_CCHAR_HDOT,0);
                        if(MGetLanguage()==0)
                        {
                            if (MGetAngle(0)>=C_MAX_ANGLE)
                            {
                                MEraseWindow(C_RANGE_HPOSI+2*C_CCHAR_VDOT, C_RANGE_VPOSI+C_MenuHigh+4,C_RANGE_HPOSI+2*C_CCHAR_VDOT+C_CCHAR_HDOT, C_RANGE_VPOSI+C_MenuHigh+4+C_CCHAR_VDOT );
                                TextOut(C_RANGE_HPOSI+2*C_CCHAR_VDOT , C_RANGE_VPOSI+C_MenuHigh ,1,4,10,(char *)_infinity[MGetLanguage()][0],0);
                            }
                            else
                            {
                                Write_Number(C_RANGE_HPOSI+2*C_CCHAR_VDOT, C_RANGE_VPOSI+C_MenuHigh+4 , (MGetAngle(1) + 50)/100, 4,2,0);
                            }
                        }
                        else
                        {
                            Write_Number(C_RANGE_HPOSI+3*C_CCHAR_HDOT-5, C_RANGE_VPOSI+C_MenuHigh+4 ,MGetAngle(0),4,1,0);	//angle
                           // Write_Ascii(C_RANGE_HPOSI+3*C_CCHAR_HDOT+38, C_RANGE_VPOSI+C_MenuHigh+4 ,ANGLE);
                        }
                        //	Write_Number(3*C_CCHAR_HDOT,8 ,( MGetAngle(1) + 50)/100,4,2,0);
                        //	Write_Number(C_RANGE_HPOSI + 12 * C_ECHAR_HDOT, C_ECHAR_HDOT + 1, (MGetAngle(1) + 50)/100, 4,2,0);
#if C_ROTARY > 0
                        rotary = MGetKeyCode(0);
                        if(rotary == C_KEYCODMAX)
                        {
                            rotary = MGetRotary(-1);
                            if( rotary >= 128)
                            {
                                rotary = 0;
                                keycode = C_KEYCOD_CONFIRM;
                            }
                            else if( rotary > 0 )
                            {
                                keycode = C_KEYCOD_PLUS;
                            }
                            else if( rotary < 0)
                            {
                                keycode = C_KEYCOD_MINUS;
                                rotary = -1 * rotary;
                            }
                            else break;
                            ///step = rotary;
                        }
                        else rotary = 1;
                        if( keycode != rotary )break;
#else
                        if( keycode != MGetKeyCode(0) )break;
#endif
                    }
                    while( true );
                }
                else
                {
#if C_DEVTYPE != 15 && C_COLOR != 1
                    if( ( keycode == C_KEYCOD_ANGLE || keycode == C_KEYCOD_CONFIRM) && MGetSystemMode() != 1)
                    {
#if C_DEVTYPE != 20
                        ColorMenu();
#elif  C_DEVTYPE == 9 || C_DEVTYPE == 11 || C_DEVTYPE == 3 || C_DEVTYPE == 20
                        //ParaMenu(1);
                        MSaveDefualtColor(MGetColorMode()+1);//设置默认颜色
                        MFclearScreen();
                        ScreenRenovate();	/*屏幕刷新*/
                        DrawDac(0);
                        MParaRenovate(0);
#endif
                        //	if(  C_DEVTYPE != 3)
                        {
                            DisplayPrompt(16);
                            return - 1;
                        }
                    }
                    else
#endif
                    {
                        MSelectPrompt(keycode);
                        return keycode;
                    }
                    break;
                }
                break;
            }
        }
        if( !retvalue ) break;
    }
    while (1);

    if(keycode == C_KEYCOD_RETURN || keycode == C_KEYCOD_CONFIRM)
    {
        keycode = -1;
        DisplayPrompt(16);
    }
    else DisplayPrompt(15);
    ;
    return retvalue;
}

int AnglePlus(int step)
{
    u_int angle = MGetAngle(0);

    if (angle >= C_MAX_ANGLE)
    {
        DisplayPrompt(3);
        return C_FALSE;
    }
    angle += step;
    MSetAngle(angle,C_SETMODE_SETSAVE) ;
    MSetTestStatus(C_TEST_ANGLE,0);	//设置测试状态,K值未测

    return C_TRUE;
}

int AngleMinus(int step)
{
    u_int angle = MGetAngle(0);

    if (angle < step)
    {
        DisplayPrompt(3);
        MSetAngle(0,C_SETMODE_SETSAVE) ;
        return C_FALSE;
    }

    angle -= step;
    MSetAngle(angle,C_SETMODE_SETSAVE) ;
    MSetTestStatus(C_TEST_ANGLE,0);	//设置测试状态,K值未测
    return C_TRUE;
}



void MDACGateChange(int keycode, int type)	/* 门改变*/
{
    int offset;
    //u_short dac_line[C_LEN_DACDB];
    //u_short	 VPosi = C_COORVPOSI + 200;
    int rotary;
    int deci_len;
    u_short time = 0;
    u_short  HPosi ;
    u_short  Height ;
    u_short  Width ;

    u_short xpos = C_CCHAR_HDOT * (3+MGetLanguage()*5) ;
    u_short ypos = 0 ;

    offset = C_OFF_DACGATE;
    MCopyProtectedMemory( dac_line, (void*)offset, C_LEN_DACDB, PM_COPY_READ);

    HPosi = MGetGatePara(0, 0); /* 水平位置 */
    Width = MGetGatePara(0, 1);
    Height = MGetGatePara(0, 2);
    if( MGetUnitType() > 0)deci_len = 3;
    else deci_len = 1;//都带1小数
    switch (type)
    {
    case 0://加减门位
        do
        {
#if C_DEVTYPE == 14
            if(keycode == C_KEYCOD_MINUS || keycode == C_KEYCOD_LEFT)
#else
            if(keycode == C_KEYCOD_MINUS)
#endif
            {
                if(HPosi < 1)
                {
                    DisplayPrompt(3);
                    break ;
                }
                DrawGateCurver( dac_line, HPosi , Width,1);
                HPosi --;
                DrawGateCurver( dac_line, HPosi + Width, 1,-1);
                DrawGateCurver( dac_line, HPosi, 1,-1);
                DrawGateCurver( dac_line, HPosi, Width, 1);
            }

#if C_DEVTYPE == 14
            else if(keycode == C_KEYCOD_PLUS || keycode == C_KEYCOD_RIGHT)
#else
            else if(keycode == C_KEYCOD_PLUS )
#endif
            {
                if((HPosi + Width) > C_WAVEWIDTH )
                {
                    DisplayPrompt(3);
                    break ;
                }

                DrawGateCurver( dac_line, HPosi , Width,1);
                DrawGateCurver( dac_line, HPosi, 1,-1);
                DrawGateCurver( dac_line, HPosi + Width, 1,-1);
                HPosi ++;
                DrawGateCurver( dac_line, HPosi , Width,1);

            }
            MSetGatePara(HPosi,Width,Height,0,C_SETMODE_SETSAVE);
            Write_Number(xpos, ypos, HPosi*MGetRange(1)/(C_COORHORIUNIT*10), 5, deci_len,0) ;
            keycode = MGetKeyCode(time);
            if(keycode == C_KEYCODMAX)
            {
                rotary = MGetRotary(-1);
                if( rotary >= 128)
                {
                    rotary = 0;
                    keycode = C_KEYCOD_CONFIRM;
                }
                else if( rotary > 0 )keycode = C_KEYCOD_PLUS;
                else if( rotary < 0)
                {
                    keycode = C_KEYCOD_MINUS;
                    rotary = -1 * rotary;
                }
            }
            if(GetSameKeyTime() > 5)time = 0;
            else if(GetSameKeyTime() > 3)time = 3;
            else time = 5;

#if C_DEVTYPE == 14
        }
        while(keycode == C_KEYCOD_PLUS || keycode == C_KEYCOD_MINUS || keycode == C_KEYCOD_RIGHT || keycode == C_KEYCOD_LEFT) ;
#else
        }
        while(keycode == C_KEYCOD_PLUS || keycode == C_KEYCOD_MINUS) ;
#endif
        break;
    case 1://加减门宽
        do
        {
#if C_DEVTYPE == 14
            if(keycode == C_KEYCOD_MINUS || keycode == C_KEYCOD_LEFT)
#else
            if(keycode == C_KEYCOD_MINUS )
#endif
            {
                if(Width < 1)
                {
                    DisplayPrompt(3);
                    break;
                }

                DrawGateCurver( dac_line, HPosi , Width,3);
                Width --;
                DrawGateCurver( dac_line, HPosi + Width,1, -1);
                DrawGateCurver( dac_line, HPosi , Width,3);
            }
#if C_DEVTYPE == 14
            else if(keycode == C_KEYCOD_PLUS || keycode == C_KEYCOD_RIGHT)
#else
            else if(keycode == C_KEYCOD_PLUS )
#endif
            {
                if((HPosi + Width) > C_WAVEWIDTH )
                {
                    DisplayPrompt(3);
                    break ;
                }
                DrawGateCurver( dac_line, HPosi , Width,3);
                DrawGateCurver( dac_line, HPosi + Width, 1, -1);
                Width ++;
                DrawGateCurver( dac_line, HPosi , Width,3);
            }
            MSetGatePara(HPosi,Width,Height,0,C_SETMODE_SETSAVE);
            Write_Number(xpos, ypos, Width*MGetRange(1)/(C_COORHORIUNIT*10), 5, deci_len, 0) ;
            keycode = MGetKeyCode(time);
            if(keycode == C_KEYCODMAX)
            {
                rotary = MGetRotary(-1);
                if( rotary >= 128)
                {
                    rotary = 0;
                    keycode = C_KEYCOD_CONFIRM;
                }
                else if( rotary > 0 )keycode = C_KEYCOD_PLUS;
                else if( rotary < 0)keycode = C_KEYCOD_MINUS;
                ;
            }

            if(GetSameKeyTime() > 5)time = 0;
            else if(GetSameKeyTime() > 3)time = 3;
            else time = 5;

#if C_DEVTYPE == 14
        }
        while(keycode == C_KEYCOD_PLUS || keycode == C_KEYCOD_MINUS || keycode == C_KEYCOD_RIGHT || keycode == C_KEYCOD_LEFT) ;
#else
        }
        while(keycode == C_KEYCOD_PLUS || keycode == C_KEYCOD_MINUS) ;
#endif
        break;
    case 2://加减门高
        DrawGateCurver( dac_line, MGetGatePara(0, 0), Width,0);
        do
        {
#if C_DEVTYPE == 14
            if(keycode == C_KEYCOD_MINUS || keycode == C_KEYCOD_LEFT)
#else
            if(keycode == C_KEYCOD_MINUS )
#endif
            {
                Height = DacGate_dBHigh(MGetGatedB()-2,MGetDacDb());//门与母线的dB差，DACdB值
            }
#if C_DEVTYPE == 14
            else if(keycode == C_KEYCOD_PLUS || keycode == C_KEYCOD_RIGHT)
#else
            else if(keycode == C_KEYCOD_PLUS )
#endif
            {
                Height = DacGate_dBHigh(MGetGatedB()+2,MGetDacDb());//门与母线的dB差，DACdB值
            }

            MSetGatePara(HPosi,Width,Height,0,C_SETMODE_SETSAVE);
            Write_Number(xpos, ypos, MGetGatedB(), 4,1,1) ;
            keycode = MGetKeyCode(time);
            if(keycode == C_KEYCODMAX)
            {
                rotary = MGetRotary(-1);
                if( rotary >= 128)
                {
                    rotary = 0;
                    keycode = C_KEYCOD_CONFIRM;
                }
                else if( rotary > 0 )keycode = C_KEYCOD_PLUS;
                else if( rotary < 0)keycode = C_KEYCOD_MINUS;
                ;
            }
            if(GetSameKeyTime() > 5)time = 0;
            else if(GetSameKeyTime() > 3)time = 3;
            else time = 5;

#if C_DEVTYPE == 14
        }
        while(keycode == C_KEYCOD_PLUS || keycode == C_KEYCOD_MINUS || keycode == C_KEYCOD_RIGHT || keycode == C_KEYCOD_LEFT) ;
#else
        } while(keycode == C_KEYCOD_PLUS || keycode == C_KEYCOD_MINUS) ;
#endif

        DacGateInit();
        DrawDacGate();
        break;
    }
}

int GateChange(void)	  /*改变门*/
{
    int keycode ;
    int rotary  =0;
    int deci_len;
    u_short  HPosi ;
    u_short  Height ;
    u_short  Width ;

    u_short xpos,ypos;
    u_short char_len,row_number,row_height;	/*字符数和字符行数*/

    u_char  mode = MGetParaSort(C_AGATEPOSI) ;
    char gatetype;

    xpos = 0;
    ypos = 0;
    char_len = 3;
    row_number = 1;
    row_height = 24;

    if( MGetUnitType() > 0)deci_len = 3;
    else deci_len = 1;//都带1小数
    ClearEnvelope();	//清包络或峰值
//	mode = 0;
#if C_DEVTYPE == 4 || C_DEVTYPE == 20 || C_DEVTYPE == 5
    mode %= 3;
#endif
    do
    {
        if( mode > 3 )gatetype = 1;
        else gatetype = 0;
        HPosi = MGetGatePara(gatetype, 0) ; /* 水平位置 */
        Width = MGetGatePara(gatetype, 1) ;
        Height = MGetGatePara(gatetype, 2) ;

        MSetGatePara( MGetGatePara(gatetype, 0),MGetGatePara(gatetype, 1),MGetGatePara(gatetype, 2),gatetype , C_SETMODE_SET);

        WriteParaName(C_AGATEPOSI + mode);

        u_short  posmax;
        if( MGetTestStatus(C_TEST_DAC) == 0 && MGetTestStatus(C_TEST_AVG) == 0 ) posmax = C_GATEPOSIMAX;
        else posmax = C_COORWIDTH;
        switch (mode)
        {
        case 0:
            Write_Number(xpos + C_CCHAR_HDOT * (3+MGetLanguage()*5), ypos, HPosi*MGetRange(1)/(C_COORHORIUNIT*10), 5, deci_len,0) ;
            if (MGetUnitType())
            {
                EMenuOut(xpos + C_CCHAR_HDOT * (7+MGetLanguage()*5), ypos+ 0,"in",2,1,12);
            }
            else
            {
                EMenuOut(xpos + C_CCHAR_HDOT * (7+MGetLanguage()*5), ypos+ 0,"mm",2,1,12);
            }
            break;
        case 1:
            Write_Number(xpos + C_CCHAR_HDOT * (3+MGetLanguage()*5), ypos, Width*MGetRange(1)/(C_COORHORIUNIT*10), 5, deci_len, 0) ;
            if (MGetUnitType())
            {
                EMenuOut(xpos + C_CCHAR_HDOT * (7+MGetLanguage()*5), ypos+ 0,"in",2,1,12);
            }
            else
            {
                EMenuOut(xpos + C_CCHAR_HDOT * (7+MGetLanguage()*5), ypos+ 0,"mm",2,1,12);
            }
            break;
        case 2:
            if ((MGetTestStatus(C_TEST_DAC) == 1 || MGetTestStatus(C_TEST_AVG) == 1) && MGetEchoMode() != C_RF_WAVE && MGetFunctionMode(C_DAC_GATE) == 1)
            {
                Write_Number(xpos + C_CCHAR_HDOT * (3+MGetLanguage()*5), ypos, MGetGatedB(), 4,1,1) ;
                EMenuOut(xpos + C_CCHAR_HDOT * (7+MGetLanguage()*5), ypos,"dB",2,1,12);
            }
            else
            {
                Write_Number(xpos + C_CCHAR_HDOT* (3+MGetLanguage()*5), ypos, Height*5, 4, 1,0) ;
                EMenuOut(xpos + C_CCHAR_HDOT * (7+MGetLanguage()*5), ypos,"% ",2,1,12);
            }
            break;
        case 4:
            Write_Number(xpos + C_CCHAR_HDOT * (3+MGetLanguage()*5), ypos, HPosi*MGetRange(1)/(C_COORHORIUNIT*10), 5, deci_len,0) ;
            if (MGetUnitType())
            {
                EMenuOut(xpos + C_CCHAR_HDOT * (7+MGetLanguage()*5), ypos+ 0,"in",2,1,12);
            }
            else
            {
                EMenuOut(xpos + C_CCHAR_HDOT * (7+MGetLanguage()*5), ypos+ 0,"mm",2,1,12);
            }
            break;
        case 5:
            Write_Number(xpos + C_CCHAR_HDOT * (3+MGetLanguage()*5), ypos, Width*MGetRange(1)/(C_COORHORIUNIT*10), 5, deci_len, 0) ;
            if (MGetUnitType())
            {
                EMenuOut(xpos + C_CCHAR_HDOT * (7+MGetLanguage()*5), ypos+ 0,"in",2,1,12);
            }
            else
            {
                EMenuOut(xpos + C_CCHAR_HDOT * (7+MGetLanguage()*5), ypos+ 0,"mm",2,1,12);
            }
            break;
        case 6:
            Write_Number(xpos + C_CCHAR_HDOT * (3+MGetLanguage()*5), ypos, Height*5, 4, 1,0) ;
            EMenuOut(xpos + C_CCHAR_HDOT * (7+MGetLanguage()*5), ypos+ 0,"% ",2,1,12);
            break;
        }

        ///ExpendTime(20);
        do
        {
            //	ExpendTime(10);
            MParaRenovate(1);
            keycode = MGetKeyCode(0);
            if(keycode != C_KEYCODMAX)
            {
                g_UDiskInfo.DataHeaderMark = 1;
            }
            StorageData(C_KEYCODMAX);
            if( MGetDacShowMode() > 0) MAdjustGain(0,0,MGetAmpStdMax(),MGetAmpStdMax() );
            if(keycode == C_KEYCODMAX)
#if C_ROTARY > 0
            {
                rotary = MGetRotary(-1);
                if( rotary >= 128)
                {
                    rotary = 0;
                    keycode = C_KEYCOD_CONFIRM;
                }
                else if( rotary > 0 )
                {
                    keycode = C_KEYCOD_PLUS;
                }
                else if( rotary < 0)
                {
                    keycode = C_KEYCOD_MINUS;
                    rotary = -1 * rotary;
                }
                else continue;
            }
#else
                continue;
#endif
            else rotary = 1;
            if(GetNoKeyTime() > 60 && MGetSystemMode() == 1)break;
            //在自动测试状态下未按超过60s则超出参量调节状态
        }
        while(keycode == C_KEYCODMAX);

        if(GetNoKeyTime() > 60 && MGetSystemMode() == 1)break;
        //BuzzerOn(1);
        switch (keycode)
        {
#ifdef C_KEYCOD_GATEB
//有门B键定义
        case C_KEYCOD_GATEB:
            if( mode >= 4)mode = 4+(mode + 1 )%3;
            else mode = 4 + mode%3;
            MSetParaSort(mode,C_AGATEPOSI);		//待调节参量名(门A/门B),系统增益/补偿增益
            ExpendTime(20);	/*消耗时间，以10ms为单位*/
            break ;
        case C_KEYCOD_GATE:
            if( mode < 3)mode = (mode + 1 )%3;
            else mode = mode%3;
            MSetParaSort(mode,C_AGATEPOSI);		//待调节参量名(门A/门B),系统增益/补偿增益
            ExpendTime(20);	/*消耗时间，以10ms为单位*/
            break ;
#else
//未定义门B键
        case C_KEYCOD_GATE:
            if( mode < 3)mode = (mode + 1 )%3;
            else mode = 4 + (mode+1)%3;
            MSetParaSort(mode,C_AGATEPOSI);		//待调节参量名(门A/门B),系统增益/补偿增益
            ExpendTime(20);	/*消耗时间，以10ms为单位*/
            break ;
        case C_KEYCOD_CONFIRM:
#if C_DEVTYPE == 4 || C_DEVTYPE == 20
            MSetFunctionMode( MGetFunctionMode(C_GATE_ALARM)+1,C_GATE_ALARM);
            MKeyRlx();
#elif C_DEVTYPE != 5
            if( !MGetSystemMode() )
            {
                mode = (mode + 4)%8 ;
                MSetParaSort(mode,C_AGATEPOSI);		//待调节参量名(门A/门B),系统增益/补偿增益
                ExpendTime(20);	/*消耗时间，以10ms为单位*/
            }
#endif
            break ;
#endif
#if C_DEVTYPE == 14
        case C_KEYCOD_RIGHT:
        case C_KEYCOD_LEFT:
#endif
        case C_KEYCOD_PLUS:
        case C_KEYCOD_MINUS:
            if ((MGetTestStatus(C_TEST_DAC) == 1 || MGetTestStatus(C_TEST_AVG) == 1) && MGetEchoMode() != C_RF_WAVE && MGetFunctionMode(C_DAC_GATE) == 1 && mode < 3)
            {
                MDACGateChange(keycode,mode);
                break;
            }
            switch (mode)
            {
            case 0:
                MGatePosi(keycode,0,rotary) ;
                break ;
            case 1:
                MGateWide(keycode,0,rotary) ;
                break ;
            case 2:
                MGateHigh(keycode,0,rotary) ;
                break ;
            case 4:
                MGatePosi(keycode,1,rotary) ;
                break ;
            case 5:
                MGateWide(keycode,1,rotary) ;
                break ;
            case 6:
                MGateHigh(keycode,1,rotary) ;
                break ;
            }
            MSetColor(C_CR_WAVE);
            break ;
        }

#ifdef C_KEYCOD_GATEB
//有门B键定义
#if C_DEVTYPE == 14
    }
    while( keycode == C_KEYCOD_CONFIRM ||keycode == C_KEYCOD_GATE ||keycode == C_KEYCOD_GATEB
            || keycode == C_KEYCOD_PLUS || keycode == C_KEYCOD_MINUS || keycode == C_KEYCODMAX
            || keycode == C_KEYCOD_RIGHT || keycode == C_KEYCOD_LEFT);
#else
    }
    while( keycode == C_KEYCOD_CONFIRM ||keycode == C_KEYCOD_GATE ||keycode == C_KEYCOD_GATEB  || keycode == C_KEYCOD_PLUS || keycode == C_KEYCOD_MINUS || keycode == C_KEYCODMAX );
#endif

#else
//未定义门B键
    } while( keycode == C_KEYCOD_CONFIRM ||keycode == C_KEYCOD_GATE  || keycode == C_KEYCOD_PLUS || keycode == C_KEYCOD_MINUS || keycode == C_KEYCODMAX );
#endif
    if(keycode == C_KEYCOD_RETURN || keycode == C_KEYCOD_CONFIRM)DisplayPrompt(16);
    else DisplayPrompt(15);
    ;
    MSetGatePara(MGetGatePara(0,0),MGetGatePara(0,1),MGetGatePara(0,2),0,C_SETMODE_SET);
    return keycode;
}

//
void MGatePosi(int keycode, int gatetype,int step) /* 门位改变*/
{

//    u_short	 VPosi = C_COORVPOSI + 200;
    u_short  HPosi ;
    u_short  Height ;
    u_short  Width ;
//	u_short  HPosi1 ;
//	u_short  Height1 ;
//	u_short  Width1 ;
    int deci_len;
    int rotary;
    int i;
    rotary = 0;
    u_int xpos = C_CCHAR_HDOT * (3+MGetLanguage()*5);
    u_int ypos = 0;
    i = 0;
    step = 4;
    HPosi = MGetGatePara(gatetype, 0); /* 水平位置 */
    Width = MGetGatePara(gatetype, 1);
    Height = MGetGatePara(gatetype, 2) ;
    int temp=0;
    u_short  posmax;
    if( MGetUnitType() > 0)deci_len = 3;
    else deci_len = 1;//都带1小数
    if( MGetTestStatus(C_TEST_DAC) == 0 && MGetTestStatus(C_TEST_AVG) == 0 ) posmax = C_GATEPOSIMAX;
    else posmax = C_WAVEWIDTH;
    do
    {
#if C_DEVTYPE == 14
        if(keycode == C_KEYCOD_RIGHT || keycode == C_KEYCOD_PLUS )
#else
        if(keycode == C_KEYCOD_PLUS)
#endif
        {
//#if C_ROTARY > 0
            MDrawGate(gatetype,0, -1, 0);
            HPosi += step;
            if( (HPosi + Width) >= posmax  || ( gatetype == 0 && (HPosi + Width) > MGetGatePara(1, 0) && MGetBGateMode() == 1) )
            {
                DisplayPrompt(3);
                if ( gatetype == 0 && (HPosi + Width) > MGetGatePara(1, 0) && MGetBGateMode() == 1 && MGetGatePara(1, 0) > Width)
                {
                    HPosi = MGetGatePara(1, 0) - Width -1 ;
                }
                else
                {
                    if( HPosi > C_WAVEWIDTH )HPosi = C_WAVEWIDTH;
                    if( HPosi + Width > posmax)HPosi = posmax - Width;
                }
            }
            MSetGatePara( HPosi,Width,Height,gatetype,C_SETMODE_SETSAVE);
            MDrawGate(gatetype,0, -1, 0);
            if (temp <= 10)
            {
                ExpendTime(10);
            }
            temp++;
        }
#if C_DEVTYPE == 14
        else if(keycode == C_KEYCOD_LEFT || keycode == C_KEYCOD_MINUS )
#else
        else if(keycode == C_KEYCOD_MINUS)
#endif
        {
//#if C_ROTARY > 0
            MDrawGate(gatetype,0, -1, 0);

            if( HPosi > step+4)HPosi -= step;
            else HPosi = 4;
            if( gatetype == 1 && HPosi < (MGetGatePara(0, 0) + MGetGatePara(0, 1) ) && MGetBGateMode() == 1)
            {
                HPosi = (MGetGatePara(0, 0) + MGetGatePara(0, 1) ) + 1;
            }
            if( HPosi <= 4)
            {
                DisplayPrompt(3);
            }
            MSetGatePara( HPosi,Width,Height,gatetype,C_SETMODE_SETSAVE);
            MDrawGate(gatetype,0, -1, 0);
            if (temp<=10)
            {
                ExpendTime(10);
            }
            temp++;
        }
        else
        {

            temp=0;
        }

        Write_Number(xpos, ypos, HPosi*MGetRange(1)/(C_COORHORIUNIT*10), 5, deci_len,0) ;

        rotary = MGetKey();//MGetKeyCode(0);
        if( keycode == rotary && temp > 15)
        {
            step *= 2;
            ExpendTime(4);
        }
        else
        {
            keycode = rotary;
            step = 1;
        }
//		keycode = MGetKeyCode(time);
//		if(GetSameKeyTime() > 5)time = 1;
//		else if(GetSameKeyTime() > 3)time = 3;
//		else time = 5;
#if C_ROTARY > 0
        if(keycode == C_KEYCODMAX)
        {
            rotary = MGetRotary(-1);
            if( rotary >= 128)
            {
                rotary = 0;
                keycode = C_KEYCOD_CONFIRM;
            }
            else if( rotary > 0 )keycode = C_KEYCOD_PLUS;
            else if( rotary < 0)
            {
                keycode = C_KEYCOD_MINUS;
                rotary = -1 * rotary;
            }
            step = rotary;
            ;
        }
        else step = 1;
#endif
        if( C_DEVLIB == 1 || C_DEVLIB == 24)
        {
            if( step > 4)step = 4;
        }
        else if( C_DEVLIB == 2 || C_DEVLIB == 23)
        {
            if( step > 4)step = 4;
        }
        else if( C_DEVLIB == 3)
        {
            if( step > 64)step = 64+((step-64) >> 3);
            else if( step > 16)step = 16+( (step - 16) >> 2);
            else if( step > 1)step = (step >> 1);
        }
        //MSetGatePara( HPosi,Width,Height,gatetype,C_SETMODE_SETSAVE);
#if C_DEVTYPE == 14
    }
    while(keycode == C_KEYCOD_PLUS || keycode == C_KEYCOD_MINUS || keycode == C_KEYCOD_RIGHT || keycode == C_KEYCOD_LEFT) ;
#else
    }
    while(keycode == C_KEYCOD_PLUS || keycode == C_KEYCOD_MINUS) ;
#endif
///	Write_Number(xpos, ypos, HPosi*MGetRange(1)/(C_COORHORIUNIT*10), 5, deci_len,0) ;
    MSetGatePara( HPosi,Width,Height,gatetype,C_SETMODE_SETSAVE);
}


void MGateWide(int keycode, int gatetype,int step)	/*门宽改变*/
{
//    u_short	 VPosi = C_COORVPOSI + 200;
    u_short  HPosi ;
    u_short  Height ;
    u_short  Width ;
//	u_short  HPosi1 = 0;
//	u_short  Height1 = 0;
//	u_short  Width1 = 0;
    u_int deci_len;

    u_int xpos = C_CCHAR_HDOT * (3+MGetLanguage()*5) ;
    u_int ypos = 0 ;
    int rotary;
    int i;
    rotary = 0;
    step = 4;
    i = 0;
    u_short  posmax;
    int temp=0;
    if( MGetTestStatus(C_TEST_DAC) == 0 && MGetTestStatus(C_TEST_AVG) == 0 ) posmax = C_GATEPOSIMAX;
    else posmax = C_WAVEWIDTH;
    if( MGetUnitType() > 0)deci_len = 3;
    else deci_len = 1;//都带1小数
    do
    {
        HPosi = MGetGatePara(gatetype, 0); /* 水平位置 */
        Width = MGetGatePara(gatetype, 1);
        Height = MGetGatePara(gatetype, 2) ;
#if C_DEVTYPE == 14
        if(keycode == C_KEYCOD_PLUS || keycode == C_KEYCOD_RIGHT)
#else
        if(keycode == C_KEYCOD_PLUS )
#endif
        {
///#if C_ROTARY > 0
            if( Width > 0)MDrawGate(gatetype,0, -1, 0);
            Width += step;
            if( (HPosi + Width ) >= posmax || ( gatetype == 0 && (HPosi + Width) > MGetGatePara(1, 0) && MGetBGateMode() == 1) )
            {
                DisplayPrompt(3);
                if ( gatetype == 0 && (HPosi + Width) > MGetGatePara(1, 0) && MGetBGateMode() == 1)
                {
                    Width = MGetGatePara(1, 0) - HPosi -1 ;
                }
                else
                {
                    if( HPosi > C_WAVEWIDTH )HPosi = C_WAVEWIDTH;
                    if( HPosi + Width > posmax)Width = posmax - HPosi;
                }
            }
            MSetGatePara( HPosi,Width,Height,gatetype,C_SETMODE_SETSAVE);
            MDrawGate(gatetype,0, -1, 0);
            if (temp<=10)
            {
                ExpendTime(10);
            }
            temp++;
        }
#if C_DEVTYPE == 14
        else if(keycode == C_KEYCOD_MINUS || keycode == C_KEYCOD_LEFT)
#else
        else if(keycode == C_KEYCOD_MINUS )
#endif
        {
//#if C_ROTARY > 0
            if( Width > 0)MDrawGate(gatetype,0, -1, 0);
            if( Width > step )Width -= step;
            else Width = 1;
            if( Width == 1 )
            {
                DisplayPrompt(3);
            }
            MSetGatePara( HPosi,Width,Height,gatetype,C_SETMODE_SETSAVE);
            MDrawGate(gatetype,0, -1, 0);
            if (temp<=10)
            {
                ExpendTime(10);
            }
            temp++;
            /*
            #else
            			for( i = 0; i < step; i++)
            			{
            				if(Width <= 1)
            				{
            					DisplayPrompt(3);
            					break;
            				}
            				MDrawGate(gatetype, 0, 1, 0);
            				Width--;
            				MSetGatePara( HPosi,Width,Height,gatetype,C_SETMODE_SETSAVE);
            				MDrawGate(gatetype,0, 1, 1);
            			}
            			if(Width <= 1)
            			{
            				break;
            			}
            #endif
            //*/
        }
        else
        {
            temp=0;
        }
        Write_Number(xpos, ypos, Width*MGetRange(1)/(C_COORHORIUNIT*10), 5, deci_len, 0) ;


        rotary = MGetKey();//MGetKeyCode(0);
        if( keycode == rotary && temp > 15)
        {
            step *= 2;
            ExpendTime(4);
        }
        else
        {
            keycode = rotary;
            step = 1;
        }

#if C_ROTARY > 0
        if(keycode == C_KEYCODMAX)
        {
            rotary = MGetRotary(-1);
            if( rotary >= 128)
            {
                rotary = 0;
                keycode = C_KEYCOD_CONFIRM;
            }
            else if( rotary > 0 )keycode = C_KEYCOD_PLUS;
            else if( rotary < 0)
            {
                keycode = C_KEYCOD_MINUS;
                rotary = -1 * rotary;
            }
            step = rotary;
            ;
        }
        else step = 1;
#endif
        if( C_DEVLIB == 1 || C_DEVLIB == 24)
        {
            if( step > 4)step = 4;
            //ExpendTime(5);
            //step = 1;
        }
        else if( C_DEVLIB == 2 || C_DEVLIB == 23)
        {
            if( step > 4)step = 4;
        }
        else if( C_DEVLIB == 3)
        {
            if( step > 100)step = (step >> 3);
            else if( step > 40)step = (step >> 2);
            else if( step > 20)step = (step >> 1);
        }
        //MSetGatePara( HPosi,Width,Height,gatetype,C_SETMODE_SETSAVE);
#if C_DEVTYPE == 14
    }
    while(keycode == C_KEYCOD_PLUS || keycode == C_KEYCOD_MINUS || keycode == C_KEYCOD_LEFT || keycode == C_KEYCOD_RIGHT) ;
#else
    }
    while(keycode == C_KEYCOD_PLUS || keycode == C_KEYCOD_MINUS) ;
#endif
///	Write_Number(xpos, ypos, Width*MGetRange(1)/(C_COORHORIUNIT*10), 5, deci_len, 0) ;
    MSetGatePara( HPosi,Width,Height,gatetype,C_SETMODE_SETSAVE);
}

void MGateHigh(int keycode, int gatetype,int step)	/* 门高改变*/
{
//	u_short	 VPosi = C_COORVPOSI + 200;
    u_short  HPosi ;
    u_short  Height ;
    u_short  Width ;
    u_int time = 10;
    int rotary;

    u_int xpos = C_CCHAR_HDOT * (3+MGetLanguage()*5) ;
    u_int ypos = 0 ;
    int temp=0;
    rotary = 0;
    step = 1;
    do
    {
        HPosi = MGetGatePara(gatetype, 0); /* 水平位置 */
        Width = MGetGatePara(gatetype, 1);
        Height = MGetGatePara(gatetype, 2) ;
#if C_DEVTYPE == 14
        if(keycode == C_KEYCOD_LEFT || keycode == C_KEYCOD_MINUS)
#else
        if(keycode == C_KEYCOD_MINUS)
#endif
        {
            if(Height < 4 || Height < step)
            {
                DisplayPrompt(3);
                break;
            }

            if(gatetype == 1 &&( Height < step || Height < 5) )
            {
                //门很低隐藏B门
                MDrawGate(gatetype,0,-1,0);
                Height = 0;
                MSetGatePara( HPosi,Width,Height,gatetype,C_SETMODE_SETSAVE);
            }
            else
            {
                MDrawGate(gatetype,0,-1,0);
                Height-= step;
                MSetGatePara( HPosi,Width,Height,gatetype,C_SETMODE_SETSAVE);
                MDrawGate(gatetype,0,-1,0);

            }
            if (temp==0)
            {
                ExpendTime(30);
            }
            temp++;
        }
#if C_DEVTYPE == 14
        else if(keycode == C_KEYCOD_RIGHT || keycode == C_KEYCOD_PLUS)
#else
        else if(keycode == C_KEYCOD_PLUS )
#endif
        {
            if(Height + step > 199)
            {
                DisplayPrompt(3);
                break;
            }
            if(gatetype == 1 && Height < 5)
            {
                Height = 5;
                MSetGatePara( HPosi,Width,Height,gatetype,C_SETMODE_SETSAVE);
                MDrawGate(gatetype,0,-1,0);

            }
            else
            {
                MDrawGate(gatetype,0,-1,0);
                Height+= step;
                MSetGatePara( HPosi,Width,Height,gatetype,C_SETMODE_SETSAVE);
                MDrawGate(gatetype,0,-1,0);
            }
            if (temp==0)
            {
                ExpendTime(30);
            }
            temp++;

        }
        else
        {
            temp=0;
        }
        Write_Number(xpos,ypos, Height * 5, 4,1,0) ;


//		keycode = MGetKey();//MGetKeyCode(0);
        keycode = MGetKeyCode(time);
        if(GetSameKeyTime() > 5)time = 1;
        else if(GetSameKeyTime() > 3)time = 3;
        else time = 5;
#if C_ROTARY > 0
        if(keycode == C_KEYCODMAX)
        {
            rotary = MGetRotary(-1);
            if( rotary >= 128)
            {
                rotary = 0;
                keycode = C_KEYCOD_CONFIRM;
            }
            else if( rotary > 0 )keycode = C_KEYCOD_PLUS;
            else if( rotary < 0)
            {
                keycode = C_KEYCOD_MINUS;
                rotary = -1*rotary;
            }
            step = rotary;
            ;
        }
        else step = 1;
#endif
#if C_DEVTYPE == 14
    }
    while(keycode == C_KEYCOD_PLUS || keycode == C_KEYCOD_MINUS || keycode == C_KEYCOD_RIGHT || keycode == C_KEYCOD_LEFT) ;
#else
    }
    while(keycode == C_KEYCOD_PLUS || keycode == C_KEYCOD_MINUS) ;
#endif
    MSetGatePara( HPosi,Width,Height,gatetype,C_SETMODE_SETSAVE);
}


/*显示各参数值*/
void WriteBaseGain(u_int xpos,u_int ypos)	//基本增益
{
    Write_Number(xpos,ypos,MGetBaseGain(),4,1,0);
}
void WriteCompGain(u_int xpos,u_int ypos)	//补偿增益
{
    Write_Number(xpos,ypos,MGetCompGain(),5,1,1);
}
void WriteSurfGain(u_int xpos,u_int ypos)	//表面补偿
{
    Write_Number(xpos,ypos,MGetSurfGain(),5,1,1);
}
void WriteRange(u_int xpos,u_int ypos)	//声程范围
{
    if( MGetUnitType() > 0)
    {
        //单位inch
        Write_Number(xpos,ypos,MGetRange(1),5,3,0);
    }
    else
    {
        Write_Number(xpos,ypos,MGetRange(1),5,1,0);
    }
}
void WriteSpeed(u_int xpos,u_int ypos)	//声速
{
    if( MGetUnitType() > 0)
    {
        //单位inch
        Write_Number(xpos,ypos,MGetSpeed(),5,2,0);
        EMenuOut(xpos + 5 * C_ECHAR_HDOT,ypos,"in/ms",5,1,8);
    }
    else
    {
        Write_Number(xpos,ypos,MGetSpeed(),5,0,0);
        EMenuOut(xpos + 5 * C_ECHAR_HDOT,ypos,"m/s",3,1,8);
    }
}
void WriteOffset(u_int xpos,u_int ypos)//零点
{
    Write_Number(xpos,ypos,MGetOffset()*10/16,5,2,0);
}
void WriteReject(u_int xpos,u_int ypos)//抑制
{
    Write_Number(xpos,ypos,MGetReject()*5,4,1,0);
}
void WriteDelay(u_int xpos,u_int ypos)	//延时
{
    if( MGetUnitType() > 0)
    {
        //单位inch
        Write_Number(xpos,ypos,MGetDelay(1),5,3,0);
    }
    else
    {
        Write_Number(xpos,ypos,MGetDelay(1),5,1,0);
    }
}
void WriteAngle(u_int xpos,u_int ypos)	//角度
{
    Write_Number(xpos,ypos,MGetAngle(0),4,1,0);
}
void WriteScale(void)	//标度 +快捷键
{
    /*
回波包络 CH(2756),CH(1808),CH(1692),CH(3471),
峰值记忆 CH(2369),CH(5421),CH(2839),CH(5068),
门内展宽 CH(3537),CH(3658),CH(5325),CH(3177),
数据处理 CH(4293),CH(3061),CH(2006),CH(3277),
    */
   const unsigned short _shortcut[] =
    {
        CH(2756),CH(1808),CH(1692),CH(3471),
        CH(2369),CH(5421),CH(2839),CH(5068),
        CH(3537),CH(3658),CH(5325),CH(3177),
        CH(4293),CH(3061),CH(2006),CH(3277),
    };

    u_int xpos = 0,ypos = C_COORVPOSI+C_COORHEIGHT+2;
    u_int i;
    u_int number = MGetDelay(1);	//得到当前标度下的延时距离
    int range;

    range = MGetRange(1)/5;	//得到当前标度下的探测范围
//	if( MGetProbeMode() == C_P_TRANSMISSION)range *= 2;
    MSetColor(C_CR_PARA);
    if( MGetUnitType() > 0)
    {
        //单位inch
        Write_Number(xpos,ypos,number,5,3,0);
        for(i=0; i<4 ; i++)
        {
            xpos += C_COORHORIUNIT*2;
            number += range;
            Write_Number(xpos,ypos,(number+5)/10,5,2,0);
        }
    }
    else
    {
        Write_Number(xpos,ypos,number,5,1,0);
        for(i=0; i<4 ; i++)
        {
            xpos += C_COORHORIUNIT*2;
            number += range;
            Write_Number(xpos,ypos,(number+5)/10,5,0,0);
        }
    }
    /*    CETextOut2(128+8,ypos+C_ECHAR_VDOT-2,_shortcut,4,1,16,1,0);
        InvertBytes(128+8,ypos+C_ECHAR_VDOT-2,7);
        CETextOut2(128*2+8,ypos+C_ECHAR_VDOT-2,_shortcut+4,4,1,16,1,0);
        CETextOut2(128*3+8,ypos+C_ECHAR_VDOT-2,_shortcut+8,4,1,16,1,0);
        InvertBytes(128*3+8,ypos+C_ECHAR_VDOT-2,7);
        CETextOut2(128*4+8,ypos+C_ECHAR_VDOT-2,_shortcut+12,4,1,16,1,0);*/
}
void WriteLongness(u_short xpos, u_short ypos,int number,u_char total_len/*总长*/,u_char mode/*是否写单位*/)
{
    if( MGetUnitType() > 0)
    {
        //单位inch
        Write_Number(xpos,ypos ,number,total_len,3,0);
        if( mode > 0)EMenuOut(xpos + total_len * C_ECHAR_HDOT,ypos,"in",2,1,8);
    }
    else
    {
        Write_Number(xpos,ypos,number,total_len,1,0);
        if( mode > 0)EMenuOut(xpos + total_len * C_ECHAR_HDOT,ypos,"mm",2,1,8);
    }

}

void ChannelScreenRenovate(void)	/*屏幕刷新*/
{
    MSetColor(C_CR_PARA);
    MCoorDraw(C_COORHPOSI ,C_COORVPOSI , C_COORHEIGHT , C_COORWIDTH );	/*画指定位置和大小的波形坐标*/

    WriteScale();	//标度
    SysParaDisp();	//显示通道号、探头类型、频带等
    MSetColor(C_CR_PARA);
    WriteRange(C_RANGE_HPOSI,C_RANGE_VPOSI);

    WriteBaseGain(C_BASEGAIN_HPOSI,C_BASEGAIN_VPOSI);
    WriteCompGain(C_COMPGAIN_HPOSI,C_COMPGAIN_VPOSI);
    WriteSurfGain(C_SURFGAIN_HPOSI,C_SURFGAIN_VPOSI);
    MSetColor(C_CR_MENU);

    WritePeakPrompt( MGetBGateMode(),MGetEquivalentMode());	//mode1=0B门失波1进波，mode2<0波高.>0dB数
    MParaRenovate(0);
    CalcuDac();
    if( MGetFunctionMode(C_DAC_GATE) )
    {
        DacGateInit();
    }///MGateDraw();
    MDrawGate(0,0,-1,0);
    MDrawGate(1,0,-1,0);

    MSetColor(C_CR_PARA);
    DrawPower(GetBatteryStatus()%256,0);
}
void ScreenRenovate(void)	/*屏幕刷新*/
{
    MChannelRenovate();
    DisplayPrompt(16);
}

void MChannelChange(void)	//本通道屏幕参数刷表
{

    int C_Menu_HPOSI=C_BASEGAIN_HPOSI-10;
    int rect_xpos,rect_ypos;	//框所在位置
    int i;
//    MFclearScreen();
    MSetColor(C_CR_PARA);
    MCoorDraw(C_COORHPOSI ,C_COORVPOSI , C_COORHEIGHT , C_COORWIDTH );	/*画指定位置和大小的波形坐标*/
    MSetColor(C_CR_PARA);
    WriteScale();	//标度
    MSetColor(C_CR_PARA);

    rect_xpos = C_Menu_HPOSI-1 ;
    rect_ypos = C_COORVPOSI-2 ;
 /*   for(i=0; i<14; i++)
    {
        //if(i!=1 &&i!=2 && i!=3&& i!=4 && i!=5)
        if(i==0 ||i==1||i==7 || i>=11)
        {
            MDrawLine( rect_xpos , rect_ypos+i*C_MenuHigh+VertOffsetScreen  ,rect_xpos+139 , rect_ypos+i*C_MenuHigh+VertOffsetScreen ,C_CR_BACK);
        }
    }
*/    MDrawLine( rect_xpos-1 , rect_ypos+2  ,rect_xpos-1 , rect_ypos + 401,C_CR_BACK );

    SysParaDisp();	//显示通道号、探头类型、频带等
    MSetColor(C_CR_PARA);
    WriteRange(C_RANGE_HPOSI,C_RANGE_VPOSI);

    WriteBaseGain(C_BASEGAIN_HPOSI,C_BASEGAIN_VPOSI);
    WriteCompGain(C_COMPGAIN_HPOSI,C_COMPGAIN_VPOSI);
    WriteSurfGain(C_SURFGAIN_HPOSI,C_SURFGAIN_VPOSI);
    MSetColor(C_CR_MENU);

    WritePeakPrompt( MGetBGateMode(),MGetEquivalentMode());	//mode1=0B门失波1进波，mode2<0波高.>0dB数
    MParaRenovate(0);
    CalcuDac();
    if( MGetFunctionMode(C_DAC_GATE) )
    {
        DacGateInit();
    }
    MDrawGate(0,0,-1,0);
    MDrawGate(1,0,-1,0);

    MSetColor(C_CR_PARA);
    DrawPower(GetBatteryStatus()%256,0);
}
void MChannelRenovate(void)	//本通道屏幕参数刷表
{

    int C_Menu_HPOSI=C_BASEGAIN_HPOSI-10;
    int rect_xpos,rect_ypos;	//框所再位置
    int i;
//	int xpos;

//;	g_nDenoise = -1;

    MFclearScreen();

//    DisplayBMP(ac330,Colors330, 0, 0,480,640);
//    MAnyKeyReturn();
    MSetColor(C_CR_PARA);
    MCoorDraw(C_COORHPOSI ,C_COORVPOSI , C_COORHEIGHT , C_COORWIDTH );	/*画指定位置和大小的波形坐标*/
    MSetColor(C_CR_PARA);
    WriteScale();	//标度
    MSetColor(C_CR_PARA);

    rect_xpos = C_Menu_HPOSI-1 ;
    rect_ypos = C_COORVPOSI-2 ;
    for(i=0; i<14; i++)
    {
        //if(i!=1 &&i!=2 && i!=3&& i!=4 && i!=5)
        if(i==0 ||i==1||i==7 || i>=11)
        {
            MDrawLine( rect_xpos , rect_ypos+i*C_MenuHigh+VertOffsetScreen  ,rect_xpos+139 , rect_ypos+i*C_MenuHigh+VertOffsetScreen ,C_CR_BACK);
        }
    }
    MDrawLine( rect_xpos-1 , rect_ypos+2  ,rect_xpos-1 , rect_ypos + 401,C_CR_BACK );

    SysParaDisp();	//显示通道号、探头类型、频带等

//	xpos = 20*C_ECHAR_HDOT;
//	EMenuOut(xpos, 0,"c=", 2,1,12);
//	Write_Number(xpos + 2*C_ECHAR_HDOT, 0, MGetSpeed(),5,0,0);
//

    MSetColor(C_CR_PARA);
    WriteRange(C_RANGE_HPOSI,C_RANGE_VPOSI);

    WriteBaseGain(C_BASEGAIN_HPOSI,C_BASEGAIN_VPOSI);
    WriteCompGain(C_COMPGAIN_HPOSI,C_COMPGAIN_VPOSI);
    WriteSurfGain(C_SURFGAIN_HPOSI,C_SURFGAIN_VPOSI);
    MSetColor(C_CR_MENU);

    WritePeakPrompt( MGetBGateMode(),MGetEquivalentMode());	//mode1=0B门失波1进波，mode2<0波高.>0dB数
    MParaRenovate(0);
    CalcuDac();
    if( MGetFunctionMode(C_DAC_GATE) )
    {
        DacGateInit();
    }///MGateDraw();
    MDrawGate(0,0,-1,0);
    MDrawGate(1,0,-1,0);

    MSetColor(C_CR_PARA);
    DrawPower(GetBatteryStatus()%256,0);

//	TIME_CLOCK tm;
//	GetSystemTime(&tm);
//	WriteTime(C_SYSTIME_HPOSI,C_SYSTIME_VPOSI,&tm);

    //WriteTime1(C_SYSTIME_HPOSI,C_SYSTIME_VPOSI,MGetSystemTime(8));

//;	if(MGetSystemMode() != 0 )DrawDac(0);		//在自动调节过程中不画DAC


}

int GetDispEchoPara(void)/*得到指定门（0=A or 1=B)内回波的位置大小并写在屏幕上*/
{
    //int dist_dire,dist_hori,dist_vert;
    //int gateamp = MGetAmpMax(0);
  //  int para_average[10];
  //  int average=0;
    int i;
    int ElapsedTime = GetElapsedTime();
    int time = ElapsedTime - AdjustTime.PreElapsedTime;
    int para1,para2,para3,para4;
    short pos1,pos2;
//	int alarm_enable = 0;		//
    para1 = MGetAmpMax(2);
    short amp1 = para1/256;
    short amp2 = para1%256;
    u_int time1 = MGetAmpTime(0);
    u_int time2 = MGetAmpTime(1);
    pos1 = MGetAmpPos(0);
    pos2 = MGetAmpPos(1);
    //if(gateamp > MGetGatePara(0,2) || MGetAmpMax(1) < MGetGatePara(1,2)) alarm_enable = 1;
    //else alarm_enable = 0;
    int mode1,mode2;
    mode1 = MGetBGateMode();
    mode2 = MGetEquivalentMode();	//mode1=0B门失波1进波，mode2<0波高.>0dB数
    if( MGetStdMode() == C_STD_AWS_NUM - 1)//AWS
    {
        mode1 = 0;
        mode2 = -1;
    }


    //if( MGetAcquisitionMode() == 1 && AdjustTime.ParaDisplayTime > 200 )
//	if( AdjustTime.ParaDisplayTime > C_RENOVATE_TIME )
    {
        if(mode1 == 0)
        {
            //单进波门
            para1 = (int)( (double)(time1 * MGetSpeed() )/(double)(C_SAMPLE_FREQ) +0.5) ;	//距离
//;Write_Number(10,30,para1,10,0,0);
            para1 += MGetDelay(3);
            //if( (para1+2)%5 < 3)para1 = (para1+2)/5 * 5;
            if( (para1)%5 < 1);
            else if( (para1+2)%5 < 2)para1++;
            else if( para1 > 1)para1--;

//;Write_Number(100,30,MGetAngle(2),10,0,0);
//;Write_Number(200,30,MGetAngle(3),10,0,0);
            if( !MGetFunctionMode(C_CURVER_AMEND) )
            {
                para2 = para1 * MGetAngle(2) / 10000;	//水平
                para3 = para1 * MGetAngle(3) / 10000;	//垂直
            }
            else
            {
                MGetCurveAmend(&para1,&para2,&para3,&stCurveAmend);
            }
            if (para2 >= MGetForward())
            {
                para2 -= MGetForward();
            }
            if (MGetProbeMode()==0 && para3 >= MGetForward())
            {
                para3 -= MGetForward();
            }
            ///else para2 = 0;

            if( mode2 > 0)
            {
                //将波高转换dB值
                para4 = Amp2db( pos1,amp1 );

                ///para4 += MGetSurfGain();

                switch (mode2)
                {
                case 1:
                    break;
                case 2:
                case 3:
                case 4:
                case 5:
                case 6:
                case 7:
                    para4 -= MGetLineGain( MGetEquivalentMode()-2 );
                    break;
                case 10:
                    break;
                case 11://显示孔径
                    if( AdjustTime.ParaDisplayTime > C_RENOVATE_TIME )
                    {
                        if( MGetUnitType() > 0 )
                        {
							Write_Number(23*C_ECHAR_HDOT,C_COORVPOSI - C_ECHAR_VDOT-VertOffsetScreen-2,(int)( 10*powf( 10,( para4 + 120 ) / 400.0)*100/25.4  + 0.5), 5,3,0);
                        }
                        else
                        {
							Write_Number(23*C_ECHAR_HDOT,C_COORVPOSI - C_ECHAR_VDOT-VertOffsetScreen-2,(int)( 10*powf( 10,( para4 + 120 ) / 400.0)  + 0.5), 3,1,0);
                        }
                    }
                    ///					Write_Number(23*8,C_COORVPOSI - 10,(int)( 10*powf( 10,( para4 + 120 ) / 400.0)  + 0.5), 3,1,0);
                    para4 -= MGetLineGain(2);
                    break;
                }

            }
            else para4 = amp1*5;

            if( MGetThick() > 0 && MGetDiameter() == 0 && MGetSystemMode() == 0)
            {
                if( (para3 / MGetThick() ) % 2 == 0) para3 = para3 % MGetThick();
                else para3 = MGetThick() - (para3 % MGetThick() );
            }
        }
        else//双进波门
        {
            //	para1 = MGetAmpTime(0) * MGetSpeed() /(C_SAMPLE_FREQ) ;	//距离
            para1 = (int)( (double)(time1 * MGetSpeed() )/(double)(C_SAMPLE_FREQ) +0.5) ;	//距离
            para1 += MGetDelay(3);
            //if( (para1+2)%5 < 3)para1 = (para1+2)/5 * 5;
            if( (para1)%5 < 1);
            else if( (para1+2)%5 < 2)para1++;
            else if( para1 > 1)para1--;
            //if( (para1+1)%5 < 3)para1 = (para1+1)/5 * 5;

///Write_Number(0,0,MGetAmpTime(0),10,0,0);
            if( !MGetFunctionMode(C_CURVER_AMEND) )
            {
                switch (MGetScaleMode() )
                {
                case 0://0垂直1水平2距离
                    para1 = para1 * MGetAngle(3) / 10000;	//余弦
                    if( MGetThick() > 0 && MGetDiameter() == 0 && MGetSystemMode() == 0)
                    {
                        if( (para1 / MGetThick() ) % 2 == 0) para1 = para1 % MGetThick();
                        else para1 = MGetThick() - (para1 % MGetThick() );
                    }

                    break;
                case 1:
                    para1 = para1 * MGetAngle(2) / 10000;	//正弦
                    break;
                default:
                    break;
                }
            }
            else
            {
                MGetCurveAmend(&para1,&para2,&para3,&stCurveAmend);
                switch (MGetScaleMode() )
                {
                case 0://0垂直1水平2距离
                    para1 = para3;
                    break;
                case 1://
                    para1 = para2;
                    break;
                default:
                    break;
                }
            }

//			para3 = MGetAmpTime(1) * MGetSpeed() /(C_SAMPLE_FREQ) ;	//距离
            para3 = (int)( (double)(time2 * MGetSpeed() )/(double)(C_SAMPLE_FREQ) +0.5) ;	//距离
            para3 += MGetDelay(3);
            //if( (para1+2)%5 < 3)para1 = (para1+2)/5 * 5;
            if( (para3)%5 < 1);
            else if( (para3+2)%5 < 2)para3++;
            else if( para3 > 1)para3--;
            //if( (para3+1)%5 < 3)para3 = (para3+1)/5 * 5;
            if( !MGetFunctionMode(C_CURVER_AMEND) )
            {
                switch (MGetScaleMode() )
                {
                case 0://0垂直1水平2距离
                    para3 = para3 * MGetAngle(3) / 10000;	//余弦
                    if( MGetThick() > 0 && MGetDiameter() == 0 && MGetSystemMode() == 0)
                    {
                        if( (para3 / MGetThick() ) % 2 == 0) para3 = para3 % MGetThick();
                        else para3 = MGetThick() - (para3 % MGetThick() );
                    }
                    break;
                case 1:
                    para3 = para3 * MGetAngle(2) / 10000;	//正弦
                    break;
                default:
                    break;
                }
            }
            else
            {
                MGetCurveAmend(&para3,&para2,&para4,&stCurveAmend);
                switch (MGetScaleMode() )
                {
                case 0://0垂直1水平2距离
                    para3 = para4;
                    break;
                case 1://
                    para3 = para2;
                    break;
                default:
                    break;
                }
            }

            //gateamp = MGetAmpMax(2) ;//同时得到两个门内的波高
            if( mode2 > 0 )
            {
                //将波高转换dB值
                para2 = Amp2db( pos1,amp1 );
                para4 = Amp2db( pos2,amp2 );
                switch (mode2)
                {
                case 1:
                    break;
                case 2:
                case 3:
                case 4:
                case 5:
                case 6:
                case 7:
                    para2 -= MGetLineGain( MGetEquivalentMode()-2 );
                    para4 -= MGetLineGain( MGetEquivalentMode()-2 );
                    break;
                case 10:
                    break;
                case 11://显示孔径
                    para2 -= MGetLineGain(2);
                    para4 -= MGetLineGain(2);
                    break;
                }
            }
            else
            {
                para2 = (amp1)*5;
                para4 = (amp2)*5;
            }
        }

        stGateAmpMax.mode[0] = mode1;
        stGateAmpMax.mode[1] = mode2;
        stGateAmpMax.para[0] = para1;
        stGateAmpMax.para[1] = para2;
        stGateAmpMax.para[2] = para3;
        stGateAmpMax.para[3] = para4;
        if( MGetAcquisitionEnable() > 0)
        {
            ChannelPara.wavepara[0] = para1;
            ChannelPara.wavepara[1] = para2;
            ChannelPara.wavepara[2] = para3;
            ChannelPara.wavepara[3] = para4;
        }
      //  if(average>10)average=0;
      //  para_average[AdjustTime.ParaDisplayTime%10]=para4;

      //  average++;

      //      Write_Number(10,75,average,10,0,0);
      //      Write_Number(10,75,para4,10,0,0);
      //      for(i=0;i<10;i++)
            {
       //        if(para_average[i]<1280)
               {
       //         Write_Number(10,100+24*i,para_average[i],10,0,0);
               }

      //         if(para_average[i]>1280)
      //         para_average[i]=0;

       //        if(para_average[i]>0)
               {
       //         para4=(para4+para_average[i])/2;
               }
            }
        if( AdjustTime.ParaDisplayTime > C_RENOVATE_TIME )
        {

           //  para4=(para_average[9]+para_average[8]+para_average[7]+para_average[6]+para_average[5]+
           //         para_average[4]+para_average[3]+para_average[2]+para_average[1]+para_average[0])/10;
            WritePeakInfo(mode1,mode2,para1,para2,para3,para4);
        }

        if( MGetStdMode() == C_STD_AWS_NUM - 1 && AdjustTime.ParaDisplayTime > C_RENOVATE_TIME )//AWS
        {
            int x = 28+2;
            int ypos;
            ypos = C_COORVPOSI+2;

            EMenuOut(x,ypos,"d=",2,1,8);
            if( MGetUnitType() > 0)para3 = (int)((para1 - 1000)*2+500)/1000;
            else para3 = (int)((para1 - 254)*2+127)/254;
            para1 = para4;

//Write_Number(10,50,para1,8,0,1);
            if( para1 < 1)para1 = 1;
			para1 = (int)( 200 * ( log10(para1) - log10(600) ) );
//Write_Number(100,50,para1,8,0,1);
            para1 = (MGetBaseGain() + MGetCompGain() - para1+5)/10;
//Write_Number(200,50,para1,8,0,1);
            para2 = (MGetBaseGain()+5)/10;

            x += C_ECHAR_HDOT*2;
            Write_Number(x,ypos,para1,2,0,0);
            x+= C_ECHAR_HDOT*2;
            EMenuOut(x,ypos,"-",1,1,C_ECHAR_VDOT);
            x += C_ECHAR_HDOT*1;
            Write_Number(x,ypos,para2,2,0,0);
            x+= C_ECHAR_HDOT*2;
            EMenuOut(x,ypos,"-",1,1,C_ECHAR_VDOT);
            x += C_ECHAR_HDOT*1;
            Write_Number(x,ypos,para3,2,0,0);
            x+= C_ECHAR_HDOT*2;
            EMenuOut(x,ypos,"=",1,1,C_ECHAR_VDOT);
            x += C_ECHAR_HDOT*1;
            Write_Number(x,ypos,para1-para2-para3,3,0,1);
        }

        /*
        Write_Number(10,30,AdjustTime.ParaDisplayTime,10,0,0);
        Write_Number(10,40,AdjustTime.amp_total,10,0,0);
        Write_Number(10,50,AdjustTime.amp_total2,10,0,0);
        Write_Number(10,60,AdjustTime.time,10,0,0);
        Write_Number(10,70,AdjustTime.time2,10,0,0);
        //*/

        if( AdjustTime.ParaDisplayTime > C_RENOVATE_TIME )
        {

            AdjustTime.ParaDisplayTime = 0;
        }
        AdjustTime.amp_total = 0;
        AdjustTime.amp_total2 = 0;
        AdjustTime.time = AdjustTime.time2 = 0;
    }

    if( MGetFunctionMode(C_GATE_ALARM) == 1)
    {
        //gateamp = MGetAmpMax(0);
        para1 = pos1;
        //para2 = MGetAmpMax(1);
        para3 = MGetGatePara(0, 2);
        para4 = MGetGatePara(1, 2);
        mode1 = MGetBGateMode();
//Write_Number(10,30,amp1,4,0,0);
//Write_Number(50,30,para3,4,0,0);
//Write_Number(10,40,amp2,4,0,0);
//Write_Number(50,40,para4,4,0,0);
//Write_Number(60,50,mode1,4,0,0);

        //首先看A门
        if( mode1 == 0 && amp2 < para4)AlarmOn();//MSetAlarmStatus(1);//B门失波
        else if (mode1 == 1 && amp2 > para4 )AlarmOn();//MSetAlarmStatus(1);//两进波门
        else if ((MGetTestStatus(C_TEST_DAC) == 1 || MGetTestStatus(C_TEST_AVG) == 1) && MGetFunctionMode(C_DAC_GATE) == 1 )
        {
            //有DAC门

            u_char *	sampbuff = GetSampleBuffer();
            u_short amp;
            mode2 = MGetAcquisitionEnable();
            para1 = para2 = MGetGatePara(0, 0);
            para3 = para1 + MGetGatePara(0, 1);

            ///MSetAcquisitionEnable(0,C_SETMODE_SETSAVE);
            for( para2 = 2*para2 + C_OFF_DACGATE; para1 < para3; para1++)
            {
                MCopyProtectedMemory( &amp, (void*)para2, C_SIZE_SHORT, PM_COPY_READ);
                if( sampbuff[para1]*2 > amp)
                {
                    AlarmOn();//MSetAlarmStatus(1);
                    break;
                }
                para2 += C_SIZE_SHORT;
            }
            ///MSetAcquisitionEnable(mode2,C_SETMODE_SETSAVE);
        }
        else if (amp1 > para3 )
        {
            //A门永远进波
            AlarmOn();//MSetAlarmStatus(1);
        }
       /* else
        {
            AlarmOn();//MSetAlarmStatus(0);
        }*/
    }
    AdjustTime.PreElapsedTime = ElapsedTime ;
    //MSetOntime( ElapsedTime/1000,0);
    AdjustTime.GainPlusTime += time;
    AdjustTime.GainMinusTime += time;
//	AdjustTime.ParaDisplayTime += time;
//    AlarmOn();

    ///if( gateamp > 100 )AdjustTime.GainPlusTime = 0;//波高大于100，小于160不自动调节波高
    ///if( gateamp <140 )AdjustTime.GainMinusTime = 0;

    return 0;
}

void WritePeakPrompt(int mode1,int mode2)//mode1=0B门失波1进波，mode2=0波高1dB数
{
    u_int testrange[3];		//待测声程值
    int offset;
    int number;

    u_char pzText[] = "   x          ";
    pzText[0] = PHI;
    u_char ch;

    offset = C_OFF_TEMP + 30;
    MCopyProtectedMemory( testrange, (void*)offset, 12, PM_COPY_READ);

    MSetColor(C_CR_MENU);
    MEraseWindow(0, C_COORVPOSI - C_ECHAR_VDOT-VertOffsetScreen-2, 40*C_ECHAR_HDOT, C_COORVPOSI - 1) ;

    if( mode2 != 1)
    {
        pzText[0] = ' ';
        pzText[1] = ' ';
        pzText[2] = ' ';
        pzText[3] = ' ';
    if( mode1 == 0 ) pzText[4] = ' ';
    else pzText[4] = 'L';
        pzText[5] = ' ';
        pzText[6] = ' ';
        pzText[7] = ' ';
        pzText[8] = ' ';
        pzText[9] = ' ';
        pzText[10] = 'L';
    }
    switch (mode2)
    {
    case 2:
    if( mode1 == 0 ) pzText[9] = 'R';
    else pzText[3] = 'R';
        break;
    case 3:
    if( mode1 == 0 ) pzText[9] = 'S';
    else pzText[3] = 'S';
        break;
    case 4:
    if( mode1 == 0 ) pzText[9] = 'E';
    else pzText[3] = 'E';
        break;
    case 5:
    if( mode1 == 0 ) pzText[9] = '4';
    else pzText[3] = '4';
        break;
    case 6:
    if( mode1 == 0 ) pzText[9] = '5';
    else pzText[3] = '5';
        break;
    case 7:
    if( mode1 == 0 ) pzText[9] = '6';
    else pzText[3] = '6';
        break;
    case 10:
        pzText[0] = ' ';
        pzText[1] = ' ';
        pzText[2] = ' ';
        pzText[3] = ' ';
        pzText[4] = PHI;
        pzText[5] = '2';
        pzText[9] = ' ';
        pzText[10] = ' ';
        break;
    case 11:
        pzText[0] = PHI;
        pzText[1] = ' ';
        pzText[2] = ' ';
        pzText[3] = ' ';
        pzText[4] = ' ';//216;
        pzText[6] = PHI;
        pzText[9] = ' ';
        pzText[10] = ' ';
        if( MGetUnitType() == 0)
        {
			pzText[7] = (int)( powf( 10,( MGetLineGain(2) + 120 ) / 400.0)  + 0.05)+0x30;
        }
        break;
    }

    if( mode1 == 0 )
    {
        //b门失波
        Write_Ascii(0,C_COORVPOSI - C_ECHAR_VDOT-VertOffsetScreen-2,ARROW1);
        Write_Ascii(7*C_ECHAR_HDOT,C_COORVPOSI - C_ECHAR_VDOT-VertOffsetScreen-2,ARROW2);
        Write_Ascii(14*C_ECHAR_HDOT,C_COORVPOSI - C_ECHAR_VDOT-VertOffsetScreen-2,ARROW3);
        if(mode2 > 0)
        {
            //显示dB数
            if( MGetStdMode() == C_STD_AWS_NUM - 1)//AWS
            {
                Write_Ascii(22*C_ECHAR_HDOT,C_COORVPOSI - C_ECHAR_VDOT-VertOffsetScreen-2,ARROW4);
                Write_Ascii(27*C_ECHAR_HDOT,C_COORVPOSI - C_ECHAR_VDOT-VertOffsetScreen-2,'%');
            }
            else
            {
                EMenuOut(22*C_ECHAR_HDOT,C_COORVPOSI - C_ECHAR_VDOT-VertOffsetScreen-2,pzText,11,1,8);
                ///			EMenuOut(33*8+2,C_COORVPOSI - 10,"dB",2,1,8);

                if (mode2 == 1)
                {
                    if( MGetUnitType() > 0)
                    {
                        WriteLongness(23*C_ECHAR_HDOT+1,C_COORVPOSI - C_ECHAR_VDOT-VertOffsetScreen-2,(UINT)(Dac.diameter*100/25.4+0.5),5,0);
                    }
                    else
                    {
                        Write_Number(23*C_ECHAR_HDOT,C_COORVPOSI - C_ECHAR_VDOT-VertOffsetScreen-2,Dac.diameter/10,2,0,0);
                        Write_Number(26*C_ECHAR_HDOT+2,C_COORVPOSI - C_ECHAR_VDOT-VertOffsetScreen-2,Dac.length/10,2,0,0);
                    }
                }
                if (mode2 == 11)
                {
                    //avg孔径
                    if( MGetUnitType() > 0)
                    {
						number = (UINT)( 10*powf( 10,( MGetLineGain(2) + 120 ) / 400.0)  + 0.5);
                        number = (UINT)( number * 1000.0/254+0.5);
                        WriteLongness(29*C_ECHAR_HDOT+1,C_COORVPOSI - C_ECHAR_VDOT-VertOffsetScreen-2,number,4,0);
                    }
					else Write_Number(29*C_ECHAR_HDOT+1,C_COORVPOSI - C_ECHAR_VDOT-VertOffsetScreen-2,(int)( 10*powf( 10,( MGetLineGain(2) + 120 ) / 400.0)  + 0.5)/10, 1,0,0);//显示dB
                    ///				Write_Number(27*8,C_COORVPOSI - 10,(int)( 10*powf( 10,( MGetLineGain(2) + 120 ) / 400.0)  + 0.5)/10, 1,0,0);//显示dB
                }

            }

        }
        else
        {
            //显示波高
            Write_Ascii(22*C_ECHAR_HDOT,C_COORVPOSI - C_ECHAR_VDOT-VertOffsetScreen-2,ARROW4);
            Write_Ascii(27*C_ECHAR_HDOT,C_COORVPOSI - C_ECHAR_VDOT-VertOffsetScreen-2,'%');
        }
    }
    else
    {
        //b门进波
        EMenuOut(0,C_COORVPOSI - C_ECHAR_VDOT-VertOffsetScreen-2,"A:",2,1,8);
        EMenuOut(20*C_ECHAR_HDOT,C_COORVPOSI - C_ECHAR_VDOT-VertOffsetScreen-2,"B:",2,1,8);

        switch (MGetScaleMode() )
        {
        case 0:
            ch = ARROW3;
            break;
        case 1:
            ch = ARROW2;
            break;
        default:
            ch = ARROW1;
            break;
        }
        Write_Ascii(2*C_ECHAR_HDOT,C_COORVPOSI - C_ECHAR_VDOT-VertOffsetScreen-2,ch);
        Write_Ascii(22*C_ECHAR_HDOT,C_COORVPOSI - C_ECHAR_VDOT-VertOffsetScreen-2,ch);

        if( mode2 >= 10)
        {
            pzText[4] = PHI;
            if(mode2 == 10 )pzText[5] = '2';
			else pzText[5] = (int)( powf( 10,( MGetLineGain(2) + 120 ) / 400.0)  + 0.05)+0x30;
            pzText[6] = ' ';
            pzText[7] = ' ';
        }

        if(mode2 > 0 )
        {
            if(mode2 == 1)
            {
                pzText[3] = 'D';
                pzText[4] = 'A';
                pzText[5] = 'C';
            }
            EMenuOut(10*C_ECHAR_HDOT,C_COORVPOSI - C_ECHAR_VDOT-VertOffsetScreen-2,pzText+3,8,1,8);
            EMenuOut(31*C_ECHAR_HDOT,C_COORVPOSI - C_ECHAR_VDOT-VertOffsetScreen-2,pzText+3,8,1,8);
        }
        else
        {
            //AVG
            Write_Ascii(10*C_ECHAR_HDOT,C_COORVPOSI - C_ECHAR_VDOT-VertOffsetScreen-2,ARROW4);
            Write_Ascii(15*C_ECHAR_HDOT,C_COORVPOSI - C_ECHAR_VDOT-VertOffsetScreen-2,'%');
            Write_Ascii(31*C_ECHAR_HDOT,C_COORVPOSI - C_ECHAR_VDOT-VertOffsetScreen-2,ARROW4);
            Write_Ascii(36*C_ECHAR_HDOT,C_COORVPOSI - C_ECHAR_VDOT-VertOffsetScreen-2,'%');
        }
    }
}

void WritePeakInfo(int mode1,int mode2,int para1,int para2,int para3,int para4)
{
    //mode1=0B门失波1进波，mode2<0波高>0dB数
    int ypos = C_COORVPOSI - C_ECHAR_VDOT-VertOffsetScreen-2;

    MSetColor(C_CR_PARA);
    if(mode1 == 0)
    {
        WriteLongness(1*C_ECHAR_HDOT,ypos,para1,6,0);
        WriteLongness(8*C_ECHAR_HDOT,ypos,para2,6,0);
        WriteLongness(15*C_ECHAR_HDOT,ypos,para3,6,0);
        /*
        		if( MGetUnitType() > 0)
        		{//单位inch
        			Write_Number(1*8,ypos,para1,6,3,0);
        			Write_Number(8*8,ypos,para2,6,3,0);
        			Write_Number(15*8,ypos,para3,6,3,0);
        		}
        		else
        		{
        			Write_Number(1*8,ypos,para1,6,1,0);
        			Write_Number(8*8,ypos,para2,6,1,0);
        			Write_Number(15*8,ypos,para3,6,1,0);
        		}
        //*/
        if( MGetEchoMode() == C_RF_WAVE)
        {
            if(para4<1280)
            Write_Number(23*C_ECHAR_HDOT,ypos,para4,4,1,1);
            else
            Write_Number(23*C_ECHAR_HDOT,ypos,para4-1280,4,1,1);
        }
        else if(mode2 < 0)Write_Number(23*C_ECHAR_HDOT,ypos,para4,4,1,0);	///
        else Write_Number(34*C_ECHAR_HDOT,ypos,para4,5,1,1);
    }
    else
    {
        WriteLongness(3*C_ECHAR_HDOT,ypos,para1,6,0);
        WriteLongness(23*C_ECHAR_HDOT,ypos,para3,6,0);
        /*		if( MGetUnitType() > 0)
        		{//单位inch
        			Write_Number(3*8,ypos,para1,6,3,0);
        			Write_Number(23*8,ypos,para3,6,3,0);
        		}
        		else
        		{
        			Write_Number(3*8,ypos,para1,6,1,0);
        			Write_Number(23*8,ypos,para3,6,1,0);
        		}
        //*/
        if( MGetEchoMode() == C_RF_WAVE)
        {
            if(para2<1280)
            Write_Number(11*C_ECHAR_HDOT,ypos,para2,5,1,1);
            else
            Write_Number(11*C_ECHAR_HDOT,ypos,para2-1280,5,1,1);
        }
        else if(mode2 < 0) Write_Number(11*C_ECHAR_HDOT,ypos,para2,4,1,0);
        else Write_Number(13*C_ECHAR_HDOT,ypos,para2,5,1,1);


        if( MGetEchoMode() == C_RF_WAVE)
        {
            if(para4<1280)
            Write_Number(32*C_ECHAR_HDOT,ypos,para4,5,1,1);
            else
            Write_Number(32*C_ECHAR_HDOT,ypos,para4-1280,5,1,1);
        }
        else if(mode2 < 0)Write_Number(32*C_ECHAR_HDOT,ypos,para4,4,1,0);	///
        else Write_Number(34*C_ECHAR_HDOT,ypos,para4,5,1,1);


        int xpos;
        xpos = C_COORWIDTH-C_ECHAR_HDOT*11;
        ypos = C_COORVPOSI;
        EraseDrawRectangle(xpos,ypos,xpos + C_ECHAR_HDOT*11,ypos + 30);
        xpos += 4;
        ypos += 3;
        EMenuOut( xpos,ypos,"B-A:",4,1,12);
        xpos += C_ECHAR_HDOT*4;
        if( para3 >= para1)
        {
            //b门内回波深度处理后比A门深
            WriteLongness(xpos+C_ECHAR_HDOT,ypos,para3-para1,4,0);
            /*			if( MGetUnitType() > 0)
            			{//单位inch
            				Write_Number( xpos,ypos,para3-para1,5,3,0);
            			}
            			else
            			{
            				Write_Number( xpos,ypos,para3-para1,5,1,0);
            			}
            //*/
        }
        else
        {
            Write_Ascii( xpos,ypos,'-');
            WriteLongness(xpos+C_ECHAR_HDOT,ypos,para1-para3,4,0);
            /*			if( MGetUnitType() > 0)
            			{//单位inch
            				Write_Number( xpos+8,ypos,para1-para3,4,3,0);
            			}
            			else
            			{
            				Write_Number( xpos+8,ypos,para1-para3,4,1,0);
            			}
            //*/
        }
    }
}

void MParaRenovate(u_int mode)	/*参数区刷新,0立即*/
{
    if(mode == 0)	AdjustTime.ParaDisplayTime = (C_RENOVATE_TIME  + C_RENOVATE_TIME );
    else
    {
        if (MGetSaveStatus()==1)
        {
            AdjustTime.ParaDisplayTime+=2;
        }
        AdjustTime.ParaDisplayTime++;
    }
    if(GateParaMax==1 && GateParaValue == 0)
    {
        if( AdjustTime.ParaDisplayTime > 2*C_RENOVATE_TIME )
        {
            AdjustTime.ParaDisplayTime = 0;
            DispEchoPara2();
        }
    }
    else
    {
        GetDispEchoPara();/*得到指定门（0=A or 1=B)内回波的位置大小并写在屏幕上*/
    }
    MSetGateAmpColor();

}


/* 画坐标 参数为左上和高度、宽度*/
int MCoorDraw(u_int CoorHPosi , u_int CoorVPosi , u_int CoorHeight , u_int CoorWidth )
{
    int i = 0;
    int j = 0;
    int k = 0;
    ClearCursor(2);

    if(CoorHeight < 100 || CoorWidth < 120)
        return C_FALSE ;/* 如果高度小于100或宽度小于120，则不可画坐标,返回C_FALSE*/
    else
    {
        SetBackgroundColor(c_crPara[crPara[ C_CR_WAVEBACK] ]);
        MEraseWindow(0, CoorVPosi,CoorHPosi + CoorWidth + 18, CoorVPosi + CoorHeight+2) ; /* 清除窗体 */
        SetBackgroundColor(c_crPara[crPara[ C_CR_BACK] ]);


        if(CoorWidth >= C_COORWIDTH)
        {
           // MDrawLine(CoorHPosi, CoorVPosi, 640, CoorVPosi,C_CR_WAVEBACK) ;	/* 上边的线 */
           // MDrawLine(CoorHPosi, CoorVPosi + CoorHeight+1,640, CoorVPosi + CoorHeight+1,C_CR_WAVEBACK) ;
            MDrawLine(CoorHPosi, CoorVPosi, C_SCREEN_WIDTH-2, CoorVPosi,C_CR_WAVEBACK) ;	/* 上边的线 */
            MDrawLine(CoorHPosi, CoorVPosi + CoorHeight+1,C_SCREEN_WIDTH-2, CoorVPosi + CoorHeight+1,C_CR_WAVEBACK) ;
        }
        else
        {
            MDrawLine(CoorHPosi, CoorVPosi, CoorHPosi + CoorWidth, CoorVPosi,C_CR_WAVEBACK) ;	/* 上边的线 */
            MDrawLine(CoorHPosi, CoorVPosi + CoorHeight+1,CoorHPosi + CoorWidth, CoorVPosi + CoorHeight+1,C_CR_WAVEBACK) ;
            /* 下边的线 */
        }
	
#if 0
        for (i = 1; i < 8; i++ ) /* 画水平的虚线，两线垂直相差40个点 */
#else
		for (i = 1; i < 5; i++ ) /* 画水平的虚线，两线垂直相差40个点 */
#endif
        {
            for (j = 0; j <= CoorWidth/8; j++ )/* 一条水平虚线共需画CoorWidth/4个点，每4个点画一个点 */
            {
                /* draw a point at (x,y) in one of three modes: DP_NOT(inverse), DP_NOT(set) or DP_RESET(clear) */
                //MDrawPixel(CoorHPosi + 4 * j, CoorVPosi + 40 * i, DP_NOT) ;
				MDrawLine( CoorHPosi + 8 * j, CoorVPosi + (CoorHeight/5) * i,CoorHPosi + 8 * j, CoorVPosi + (CoorHeight/5) * i ,C_CR_WAVEBACK);
            }
        }

        for (j = 0; j <= CoorWidth/4; j++ )/* 一条水平虚线共需画CoorWidth/4个点，每4个点画一个点 */
        {
            /* draw a point at (x,y) in one of three modes: DP_NOT(inverse), DP_NOT(set) or DP_RESET(clear) */
            //MDrawPixel(CoorHPosi + 4 * j, CoorVPosi + 40 * i+1, DP_NOT) ;
            MDrawLine(CoorHPosi + 4 * j, CoorVPosi + (CoorHeight/5) * i+1,CoorHPosi + 4 * j, CoorVPosi + (CoorHeight/5) * i+1,C_CR_WAVEBACK);
        }
		
        for (i = 0; i < CoorHeight/(CoorHeight/5); i++ )  /* 画垂直的虚线 */
        {
            for (j = 0; j < CoorWidth/C_COORHORIUNIT + 1; j++ )	/* 两线相差C_COORHORIUNIT个点 */
            {
                for (k = 1; k < (CoorHeight/5)/8; k ++ )	/* 两水平的虚线相差40个点，两线之间的垂直虚线所画的*/
                {
                    //MDrawPixel(CoorHPosi + 28 * j, CoorVPosi + 40 * i + 4 * k, DP_NOT) ;
                    MDrawLine(CoorHPosi + C_COORHORIUNIT * j, CoorVPosi + (CoorHeight/5) * i + 8 * k,CoorHPosi +C_COORHORIUNIT * j, CoorVPosi + (CoorHeight/5) * i + 8 * k,C_CR_WAVEBACK);
                }
            }
        }

    if( extend == 0)
    {
     /*   MDrawLine(CoorHPosi, CoorVPosi + CoorHeight/2,CoorWidth, CoorVPosi + CoorHeight/2,C_CR_WAVEBACK);
        MDrawLine(CoorHPosi+CoorWidth/2, CoorVPosi,CoorHPosi+CoorWidth/2, CoorVPosi + CoorHeight,C_CR_WAVEBACK);    //竖线
*/

        for (j = 0; j <= CoorWidth; j+=2) //横线
        {
            MDrawLine(CoorHPosi+j, CoorVPosi + CoorHeight/2,CoorHPosi + j, CoorVPosi + CoorHeight/2,C_CR_WAVEBACK);
        }
        for (j = 0; j <= CoorHeight; j+=2 )
        {
            MDrawLine(CoorHPosi+CoorWidth/2, CoorVPosi + j,CoorHPosi+CoorWidth/2, CoorVPosi + j,C_CR_WAVEBACK);
        }

    }
        return C_TRUE; /* 画完坐标,返回C_TRUE */
    }
}


void DrawDacGate(void)
{
    int offset;
    u_short dac_line[C_COORWIDTH];

    offset = C_OFF_DACGATE;
    MCopyProtectedMemory( dac_line, (void*)offset,C_COORWIDTH * C_SIZE_SHORT, PM_COPY_READ);

    DrawGateCurver( dac_line, MGetGatePara(0,0), MGetGatePara(0,1),0);
}

void MGateDraw(void)   /*画门*/
{
    MDrawGate(0,0,-1,0);
    MDrawGate(1,0,-1,0);
}


void MDrawGate( int/*-1两门都画0A门1B门*/ gatetype, int/*-1向下1向上*/ mode,
                int paratype/*-1全门0门位1门宽2门高*/ ,int clear/*0新画1清除*/)
{
    u_short	 v = C_COORVPOSI + C_COORHEIGHT;  /* 垂直位置 */
    u_short  p ;
    u_short  h ;
    u_short  w ;
    int gatenum;
    int s = 4;
    int m;

    if( gatetype == 0 || gatetype == 1)gatenum = gatetype + 1;
    else
    {
        gatenum = 2;
        gatetype = 0;
    }

    if( mode == 1 || mode == -1)m = mode ;
    else m = 1;

    if( clear > 0 )clear = 1;
    else clear = 0;

    if( paratype == 0 || paratype == 1 || paratype == 2 );
    else paratype = -1;

#if C_DEVTYPE == 4 || C_DEVTYPE == 20 || C_DEVTYPE == 5
    gatenum = 1;
#endif
    for( ; gatetype < gatenum; gatetype++)
    {
        if( gatetype == 0)MSetColor(C_CR_GATEA);
        else MSetColor(C_CR_GATEB);
        if( gatetype == 0 && paratype == -1 && MGetEchoMode() != C_RF_WAVE && MGetFunctionMode(C_DAC_GATE) == 1 )
        {
            //
            DrawDacGate();
            continue;
        }
        p = MGetGatePara(gatetype, 0); // 水平位置
        w = MGetGatePara(gatetype, 1);
        h = MGetGatePara(gatetype, 2)*2;

        if( mode == 0 )
        {
            if(gatetype == 0)m = 1;
            else if( MGetBGateMode() == 1)m = 1;
            else m = -1;
        }

        if( h < 5 )return;
        if( MGetEchoMode() == C_RF_WAVE )
        {
            MSetEchoMode(0, C_SETMODE_SAVE);
            MSetGatePara(p, w, (C_COORHEIGHT/2+h/2)/2, gatetype, C_SETMODE_SAVE);
            MDrawGate(gatetype, m, paratype,clear);
            MSetGatePara(p, w, (C_COORHEIGHT/2-h/2)/2, gatetype, C_SETMODE_SAVE);
            MDrawGate(gatetype, (-1)*m, paratype, clear);
            MSetEchoMode(C_RF_WAVE, C_SETMODE_SAVE);
            MSetGatePara(p, w, h/2,gatetype, C_SETMODE_SETSAVE);
            return;
        }
        if( paratype == 0 || paratype == -1)
        {
            //posi
            MDrawLine( p - s , v-h-s*m, p-clear,v-h-clear*m ,C_CR_WAVEBACK);
        }
        if( paratype == 1 || paratype == -1)
        {
            //wide
            MDrawLine( p+w + clear, v-h-clear*m, p+w+s,v-h-s*m ,C_CR_WAVEBACK);
        }
        if( paratype == 2 || paratype == -1)
        {
            //high
            MDrawLine( p + 1, v-h, p+w-1,v-h ,C_CR_WAVEBACK);
        }
        MSetColor(C_CR_UNDO);
    }

}
void Scrsave_Picture(void)
{
    u_int xpos = 0,ypos = 0;
    u_int angle = 900+1, len = 10;
    u_int edge;
    //int xstep,ystep,
    int lstep;
    //int type;

    int i;

    MKeyRlx();

    while(MGetKeyCode(0) == C_KEYCODMAX)
    {
        xpos = Random() % 260 + 60;
        ypos = Random() % 200 + 40;
        angle =  ( ( Random() % 1800 )/30 )* 30;
        edge = Random() % 7 ;
//		len = Random() % ( C_HORIDOT_SCREEN - xpos) + 10;
//angle = 0;
        len = 20;

        switch(edge)
        {
        case 1:
        case 2:
            lstep = 15;
            edge = 10;
            break;
        case 3:
            lstep = 30;
            break;
        case 4:
        case 5:
        case 6:
            lstep = 22;
            break;
        default :
            lstep = 18;
        }

        for(i=0; i< 12 && MGetKeyCode(0) == C_KEYCODMAX; i++)
        {
            if(edge > 8)DrawCircle(xpos, ypos,len);
            else DrawMultiEdge(xpos,ypos,edge,(float)angle/10.0,len);
            ExpendTime( 2*(10 - edge) );
            len += lstep;

        }
        for(; i>0 && MGetKeyCode(0) == C_KEYCODMAX ; i--)
        {
            len -= lstep;
            if(edge > 8)DrawCircle(xpos, ypos,len);
            else DrawMultiEdge(xpos,ypos,edge,(float)angle/10.0,len);
            ExpendTime( 2*(10 - edge) );
        }
    }
}


void MSetColor(int item)
{
    int crMaxDots = C_COORWIDTH + 10;
#if C_COLOR == 1 || C_DEVLIB == 23


    if( extend > 0)crMaxDots = C_COORWIDTH + 20;
    else crMaxDots = C_SCREEN_WIDTH ;

    if( item == C_CR_UNDO || item == C_CR_DEFAULT)
    {
        u_int cr = MLoadColor( C_CR_CURRENT );
        /*set color for screen operation, (b0-b2): red ; (b3-b5): green ; (b6-b7): blue */
        if( item == C_CR_DEFAULT )
        {
            if( C_COLOR != 1 && MGetColorMode()%2 == 1)MSetDisplayColor( c_crPara[C_CR_BLACK] );
            else MSetDisplayColor( c_crPara[C_CR_YELLOW] );
            //MSaveColor( C_CR_MENU, c_crPara[C_CR_YELLOW]);
        }
        else if(item == C_CR_UNDO )MSetDisplayColor( c_crPara[ crPara[item%(C_CR_MAX+12)] ]  );
        MSaveColor( C_CR_CURRENT, crPara[item%(C_CR_MAX+12)] );
        MSaveColor( C_CR_UNDO, cr );
        return;
    }
    if( item < 0 || item >= C_CR_MAX+12)item = 0;

    MSaveColor( C_CR_UNDO, MLoadColor( C_CR_CURRENT ) );
    MSaveColor( C_CR_CURRENT, crPara[item%(C_CR_MAX+12)] );
    if( item == C_CR_WAVE)
    {
        int start,end, thick;
        thick = MGetThick()*(C_COORWIDTH+8)/MGetRange(4);//板厚除以垂直声程
//*

#if C_DEVLIB == 3
        if( extend > 0)MSetEchoColor( 0, crMaxDots, Echo_crPara[ crPara[C_CR_WAVEBACK] ] );
#endif
        MSetEchoColor( crMaxDots+1, C_SCREEN_WIDTH-1, Echo_crPara[ crPara[C_CR_WAVEBACK] ] );

//			int amp0 = MGetAmpMax(0);
//			int amp1 = MGetAmpMax(1);
        //if( thick < 1 )///|| amp0 >= MGetGatePara(0,2) || ( amp1 >= MGetGatePara(1,2) && MGetGatePara(1,2) > 0) )
        if(1)
        {
            MSetEchoColor( 0,crMaxDots, Echo_crPara[ crPara[C_CR_WAVE]%C_CR_NUMBER ] );
        }
        else
        {
            //有板厚
            start = end =0;// HoriOffsetScreen;
            while( start < crMaxDots && end < crMaxDots)
            {
                end = start + thick;
                if( end > crMaxDots)end = crMaxDots;
                MSetEchoColor( start, end, Echo_crPara[ crPara[C_CR_WAVE]%C_CR_NUMBER ] );
                start = end+1;
                if( start > crMaxDots)start = crMaxDots;
                end = start + thick;
                if( end > crMaxDots)end = crMaxDots;
                MSetEchoColor( start,end,Echo_crPara[ crPara[C_CR_SEWAVE]%C_CR_NUMBER ] );
                start = end+1;
            }
        }


        MSetAmp2Gate(0,0);

        MSetAmp2Gate(1,0);



      //  MSetGateAmpColor();



        UpdateEchoColor() ;

    }
    //else
    {

        MSetDisplayColor( c_crPara[ crPara[item]%C_CR_NUMBER ]  );
    }

#endif
}

void MSaveColor(int item,u_int cr)
{
#if( C_COLOR == 1 || C_DEVLIB == 23)

//		unsigned int offset = 0;
    if( item < 0 || item >= C_CR_MAX+12)
    {
        item = 0;
        MSaveDefualtColor(0);
    }
    else
    {
        crPara[item] = cr;

///			offset = C_OFF_TEMP+60;	//颜色存储位置
///			MCopyProtectedMemory( (void*)offset, crPara, C_CR_MAX+10, PM_COPY_WRITE);
    }
#endif
}
u_int MLoadColor(int item)
{
#if( C_COLOR == 1 || C_DEVLIB == 23)

    if( item < 0 || item >= C_CR_MAX+12)item = 0;
    if( crPara[item] > C_CR_NUMBER)
    {
        MSaveDefualtColor(1);
    }
    return crPara[item];
#endif
    return 0;
}

void MSaveDefualtColor(int item)//设置默认颜色
{
#if( C_COLOR == 1)
    unsigned int offset;
   // item = item%9;
    if( item == 1)  //传统黑
    {
        for( offset = 0 ; offset < C_CR_MAX + 12; offset++)crPara[offset] = 0;
        crPara[C_CR_WAVE] = 2;//回波黄色
        crPara[C_CR_SEWAVE] = 3;//回波绿色
        crPara[ C_CR_ENVELOPE] = 4;//青
        crPara[ C_CR_MENU] = 3;//绿
        crPara[ C_CR_PARA] = 2;//青
        crPara[ C_CR_GATEA] = 1;//粉红
        crPara[ C_CR_GATEB] = 3;//绿
        crPara[ C_CR_CURVE] = 4;//青
        crPara[ C_CR_BACK] = 8;//黑
        crPara[ C_CR_WAVEBACK] = 8;//蓝
    }
    else if( item == 2) //海洋蓝
    {
        for( offset = 0 ; offset < C_CR_MAX + 12; offset++)crPara[offset] = 0;
        crPara[C_CR_WAVE] = 2;
        crPara[C_CR_SEWAVE] = 3;
        crPara[ C_CR_ENVELOPE] = 4;
        crPara[ C_CR_MENU] = 2;
        crPara[ C_CR_PARA] = 7;
        crPara[ C_CR_GATEA] = 1;
        crPara[ C_CR_GATEB] = 3;
        crPara[ C_CR_CURVE] = 4;
        crPara[ C_CR_BACK] = 5;
        crPara[ C_CR_WAVEBACK] = 8;
    }
    else if( item == 3) //梅子青
    {
        for( offset = 0 ; offset < C_CR_MAX + 12; offset++)crPara[offset] = 0;
        crPara[C_CR_WAVE] = 7;
        crPara[C_CR_SEWAVE] = 3;
        crPara[ C_CR_ENVELOPE] = 4;
        crPara[ C_CR_MENU] = 5;
        crPara[ C_CR_PARA] = 0;
        crPara[ C_CR_GATEA] = 1;
        crPara[ C_CR_GATEB] = 2;
        crPara[ C_CR_CURVE] = 4;
        crPara[ C_CR_BACK] = 4;
        crPara[ C_CR_WAVEBACK] = 5;
    }
    else if( item == 4) //火焰红
    {
        for( offset = 0 ; offset < C_CR_MAX + 12; offset++)crPara[offset] = 0;
        crPara[C_CR_WAVE] = 7;
        crPara[C_CR_SEWAVE] = 3;
        crPara[ C_CR_ENVELOPE] = 4;
        crPara[ C_CR_MENU] = 2;
        crPara[ C_CR_PARA] = 7;
        crPara[ C_CR_GATEA] = 1;
        crPara[ C_CR_GATEB] = 2;
        crPara[ C_CR_CURVE] = 4;
        crPara[ C_CR_BACK] = 0;
        crPara[ C_CR_WAVEBACK] = 5;
    }
    else if( item == 5) //香槟粉
    {
        for( offset = 0 ; offset < C_CR_MAX + 12; offset++)crPara[offset] = 0;
        crPara[C_CR_WAVE] = 8;
        crPara[C_CR_SEWAVE] = 3;
        crPara[ C_CR_ENVELOPE] = 5;
        crPara[ C_CR_MENU] = 2;
        crPara[ C_CR_PARA] = 8;
        crPara[ C_CR_GATEA] = 0;
        crPara[ C_CR_GATEB] = 4;
        crPara[ C_CR_CURVE] = 5;
        crPara[ C_CR_BACK] = 1;
        crPara[ C_CR_WAVEBACK] = 7;
    }
    else if( item == 6) //香草绿
    {
        for( offset = 0 ; offset < C_CR_MAX + 12; offset++)crPara[offset] = 0;
        crPara[C_CR_WAVE] = 8;
        crPara[C_CR_SEWAVE] = 3;
        crPara[ C_CR_ENVELOPE] = 5;
        crPara[ C_CR_MENU] = 5;
        crPara[ C_CR_PARA] = 8;
        crPara[ C_CR_GATEA] = 0;
        crPara[ C_CR_GATEB] = 4;
        crPara[ C_CR_CURVE] = 5;
        crPara[ C_CR_BACK] = 3;
        crPara[ C_CR_WAVEBACK] = 7;
    }
    else if( item == 7) //经典白
    {
        for( offset = 0 ; offset < C_CR_MAX + 12; offset++)crPara[offset] = 0;
        crPara[C_CR_WAVE] = 8;
        crPara[C_CR_SEWAVE] = 8;
        crPara[ C_CR_ENVELOPE] = 8;
        crPara[ C_CR_MENU] = 8;
        crPara[ C_CR_PARA] = 8;
        crPara[ C_CR_GATEA] = 8;
        crPara[ C_CR_GATEB] = 8;
        crPara[ C_CR_CURVE] = 8;
        crPara[ C_CR_BACK] = 7;
        crPara[ C_CR_WAVEBACK] = 7;
    }
    else if( item == 8) //经典黑
    {
        for( offset = 0 ; offset < C_CR_MAX + 12; offset++)crPara[offset] = 0;
        crPara[C_CR_WAVE] = 7;
        crPara[C_CR_SEWAVE] = 7;
        crPara[ C_CR_ENVELOPE] = 7;
        crPara[ C_CR_MENU] = 7;
        crPara[ C_CR_PARA] = 7;
        crPara[ C_CR_GATEA] = 7;
        crPara[ C_CR_GATEB] = 7;
        crPara[ C_CR_CURVE] = 7;
        crPara[ C_CR_BACK] = 8;
        crPara[ C_CR_WAVEBACK] = 8;
    }

/*
奇次波	偶数次波	峰值包络	提示菜单	坐标参量	波门A	波门B	定量曲线	屏幕底色	波形区
2	3	4	3	2	1	3	4	8	8	传统黑
2	3	4	2	7	1	3	4	5	8	海洋蓝
7	3	4	5	0	1	2	4	4	5	梅子青
7	3	4	2	7	1	2	4	0	5	火焰红
8	3	5	2	7	0	4	5	1	7	香槟粉
8	3	5	5	8	0	4	5	3	7	香草绿
8	8	8	8	8	8	8	8	7	7	经典白
7	7	7	7	7	7	7	7	8	8	经典黑
*/
    offset = C_OFF_TEMP+60;	//颜色存储位置
    MCopyProtectedMemory( (void*)offset, crPara, C_CR_MAX+12, PM_COPY_WRITE);
//    CopyMemoryToSystem( (void*)offset,C_CR_MAX+12,PM_COPY_WRITE);
    MSetColorMode(item,C_SETMODE_SAVE);
    SetBackgroundColor(c_crPara[crPara[ C_CR_BACK] ]);
#else
#if C_DEVLIB == 23
    unsigned int offset;
    item = item%2;
    if( item == 0)
    {
        for( offset = 0 ; offset < C_CR_MAX + 12; offset++)crPara[offset] = 0;
        crPara[C_CR_WAVE] = 2;//回波黄色
        crPara[C_CR_SEWAVE] = 2;//回波
        crPara[ C_CR_ENVELOPE] = 2;//
        crPara[ C_CR_MENU] = 2;//
        crPara[ C_CR_PARA] = 2;//
        crPara[ C_CR_GATEA] = 2;//
        crPara[ C_CR_GATEB] = 2;//
        crPara[ C_CR_CURVE] = 2;//
        crPara[ C_CR_BACK] = 8;//
        crPara[ C_CR_WAVEBACK] = 8;//
    }
    else if( item == 1)
    {
        for( offset = 0 ; offset < C_CR_MAX + 12; offset++)crPara[offset] = 0;
        crPara[C_CR_WAVE] = 8;//回波
        crPara[C_CR_SEWAVE] = 8;//回波
        crPara[ C_CR_ENVELOPE] = 8;//
        crPara[ C_CR_MENU] = 8;//
        crPara[ C_CR_PARA] = 8;//
        crPara[ C_CR_GATEA] = 8;//
        crPara[ C_CR_GATEB] = 8;//
        crPara[ C_CR_CURVE] = 8;//
        crPara[ C_CR_BACK] = 7;//
        crPara[ C_CR_WAVEBACK] = 7;//
    }
    offset = C_OFF_TEMP+60;	//颜色存储位置
    MCopyProtectedMemory( (void*)offset, crPara, C_CR_MAX+12, PM_COPY_WRITE);
//    CopyMemoryToSystem( (void*)offset,C_CR_MAX+12,PM_COPY_WRITE);
    MSetColorMode(item,C_SETMODE_SAVE);
    SetBackgroundColor(c_crPara[crPara[ C_CR_BACK] ]);

#endif
#endif
 //   MSetColor(C_CR_WAVEBACK);
 //   Write_Number(50,100,C_CR_WAVEBACK,1,0,0);
 //   Write_Number(50,120,crPara[ C_CR_WAVEBACK],5,0,0);
 //   MAnyKeyReturn();
}

void MSetGateAmpColor()
{
#if C_COLOR == 1 || C_DEVLIB == 23
    short start,end;
    int thick,crMaxDots;
    if( extend > 0)crMaxDots = C_COORWIDTH + 17;
    else crMaxDots = C_SCREEN_WIDTH-3 ;

    for( start = 0; start < 2; start++)
    {
        //用start来代门类型0门A,1门B

        if ((MGetTestStatus(C_TEST_DAC) == 1 || MGetTestStatus(C_TEST_AVG) == 1) && MGetFunctionMode(C_DAC_GATE) == 1 && start == 0)
        {
            thick = 2*MGetAmpPos1(0,1) + C_OFF_DACGATE;
            MCopyProtectedMemory( &end, (void*)thick, C_SIZE_SHORT, PM_COPY_READ);
            thick = MGetAmpPos1(0,11);
//Write_Number(150,30,end,4,0,0);
            if( thick < end && MGetAmp2Gate(start) > 0 )
            {
                //现在门内回波低于门高,原来高于门高
                MSetGateEchoColor( start,1);
                MSetAmp2Gate(start,0);
            }
            else if( thick >= end && MGetAmp2Gate(start) == 0 )
            {
                //现在门内回波高于门高,原来低于门高
                MSetGateEchoColor( start,1);
                MSetAmp2Gate(start,1);
            }
            else MSetGateEchoColor( start,0);

        }
        else
        {

            if( MGetAmpMax(start) < MGetGatePara(start,2) && MGetAmp2Gate(start) > 0 )
            {
                //现在门内回波低于门高,原来高于门高
                MSetGateEchoColor( start,1);
                MSetAmp2Gate(start,0);
            }
            else if( MGetAmpMax(start) >= MGetGatePara(start,2) && MGetAmp2Gate(start) == 0 )
            {
                //现在门内回波高于门高,原来低于门高
                MSetGateEchoColor( start,1);
                MSetAmp2Gate(start,1);
            }
            else MSetGateEchoColor( start,0);
        }
    }

    if( MGetGateEchoColor(0) > 0 || MGetGateEchoColor(1) > 0 )//表示门内的回波有跨越门高,需要改变回波颜色
    {
        //2表示门A,4表示门B,6表示两门都变

        MSetEchoColor( 0, crMaxDots, Echo_crPara[crPara[ C_CR_WAVEBACK] ]  );
        thick = MGetThick()*(C_COORWIDTH+8)/MGetRange(4);//板厚除以垂直声程
//	Write_Number(10,30,thick,10,0,0);
        if( thick < 1)
        {
            MSetEchoColor( 0, crMaxDots, Echo_crPara[ crPara[C_CR_WAVE]%C_CR_NUMBER ] );
        }
        else
        {
            start = end =0;// HoriOffsetScreen;
            while( start < crMaxDots && end < crMaxDots)
            {
                end = start + thick;
                if( end > crMaxDots)end = crMaxDots;
                MSetEchoColor( start, end, Echo_crPara[ crPara[C_CR_WAVE]%C_CR_NUMBER ] );
                start = end+1;
                if( start > crMaxDots)start = crMaxDots;
                end = start + thick;
                if( end > crMaxDots)end = crMaxDots;
                MSetEchoColor( start,end,Echo_crPara[ crPara[C_CR_SEWAVE]%C_CR_NUMBER ] );
                start = end+1;
            }
        }
        if( C_DEVLIB == 3)thick = 0;
        else thick = HoriOffsetScreen;
        if( MGetGateEchoColor(0) > 0 && MGetAmp2Gate(0) > 0 )
        {
            start = MGetAmpPos1(0,0);
            end = MGetAmpPos1(0,2) + thick+1;
            if(end>597)
            {
                end=597;
            }
            MSetEchoColor( start,end,Echo_crPara[ crPara[C_CR_GATEA]%C_CR_NUMBER ] );
        }
///Write_Number(10,50,start,4,0,0);
///Write_Number(50,50,end,4,0,0);
        if( MGetGatePara(1,2) > 0 && MGetGateEchoColor(1) > 0 && MGetAmp2Gate(1) > 0)
        {
            start = MGetAmpPos1(1,0);
            end = MGetAmpPos1(1,2) + thick+1;
            if( start > crMaxDots)start = crMaxDots;
            if( end > crMaxDots)end = crMaxDots;

            if(end>597)
            {
                end=597;
            }
            MSetEchoColor( start,end,Echo_crPara[ crPara[C_CR_GATEB]%C_CR_NUMBER ] );
        }
///Write_Number(100,50,start,4,0,0);
///Write_Number(150,50,end,4,0,0);
        MSetEchoColor( C_COORWIDTH+17 , C_SCREEN_WIDTH+1, Echo_crPara[ crPara[C_CR_WAVEBACK] ]);
        UpdateEchoColor() ;
        MSetGateEchoColor(0,0);
        MSetGateEchoColor(1,0);

    }
#endif
}

