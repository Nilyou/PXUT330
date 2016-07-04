#include <stdio.h>
#include <string.h>
#include <math.h>

//#include "cudefm2c.h"
#include "M10Era.h"
#include "filefunc.h"
#include "sdcard.h"

#define USB_HOST_MODE 0     //Set USB Controller to Host mode
#define USB_SLAVE_MODE 1	//Set USB Controller to Slave mode

//const int FixGain	= 245;
//unsigned short GainTable[121] ;
unsigned char DBUF[BUFFER_LENGTH];

static char *cTips[] = {/*0*/"USB Host Mode",
                             /*1*/"Waiting for  USB connect signal",
                             /*2*/"Recv Data...[IN]",
                             /*3*/"Send Data...[OUT]",
                             /*4*/"Usb device connected successfully!",
                             /*5*/"Press any key to Send Channel Status...[Out]",
                             /*6*/"Freq[Hz]: ",
                             /*7*/"Dirctory Content",
                             /*8*/"Press any key to Receive Freq...[Out]",
                             /*9*/"Bulk OUT ERROR",
                             /*10*/"Bulk IN ERROR",


                       };
static int Y_POS[]= {0, 0x08, 0x10, 0x18, 0x20, 0x28,0x30, 0x38, };
//const int TransInterval[]={100, 800};

void RDDelay(u_int time)
{
    u_int time1, time2;
    time1 = GetElapsedTime();
    while( 1 )
    {
        time2 = GetElapsedTime();
        if( time2 - time1  >= time ) break;
    }

}

/********************************************************************
* 函数名: FTextOut
* 功  能: 输出一个字符串
* 参  数:   x, y -- start display coor
*			*pzText -- characters' buf
*			iSectionLen -- characters' iSectionLen
*			color -- Display color
* 返  回:
*
********************************************************************/
void FTextOut( int x, int y, char* pzText, int iSectionLen, int color)
{
    EraseWindow(x, y, SCREEN_WIDTH-1, y+7);
    if(iSectionLen>40)
        iSectionLen = 40;
    SetDisplayColor(color);
    pzText[iSectionLen] = '\0';
    ETextOut( x, y, pzText, iSectionLen);
    return ;
}
/********************************************************************
* 函数名: HexTextOut
* 功  能: 按十六进制输出字符串中的内容
* 参  数:   x, y -- start display coor
*			*pzText -- characters' buf
*			iSectionLen -- characters' iSectionLen
*			color1 -- Display color 1
*			color2 -- Display color 2
*
* 返  回:
*
********************************************************************/
void HexTextOut(int x, int y, char *pzText, int iSectionLen, int color1, int color2)
{
    char outText[160+1];
    int i;
    char CHAR_TABLE[16] = {0x30, 0x31, 0x32, 0x33, 0x34, 0x35,0x36, 0x37, 0x38, 0x39,
                           0x41, 0x42, 0x43, 0x44, 0x45, 0x46
                          };

    EraseWindow(x, y, SCREEN_WIDTH-1, y+7);

    //
    // 当前只支持40个字符
    //
    if (iSectionLen>40)
        iSectionLen = 40;

    outText[iSectionLen*2+1] = '\0';
    //
    // Decimal to Hex
    //
    for (i=0; i<iSectionLen; i++)
    {
        outText[2*i] = ((u_char)pzText[i]/16);
        outText[2*i+1] = ((u_char)pzText[i]%16);
    }

    for (i=0; i<iSectionLen*2; i++)
        outText[i] = CHAR_TABLE[(int)outText[i]];
    //
    // Output string
    //
    for (i=0; i<iSectionLen; i++)
    {
        //SetDisplayColor((i%2)==0?COLOR_RED:COLOR_CYAN);
        SetDisplayColor((i%2)==0?color1:color2);
        ETextOut(x+16*i, y, outText+i*2,2);
    }

    return;
}

/********************************************************************
* 函数名: FormatPrint
* 功  能: 用格式化方式打印一个数字
* 参  数:   x, y -- start display coor
*			value --
*			mode  = 0  十进制
*				  = 1  十六进制
*
* 返  回:
*
********************************************************************/
void FormatPrint(int x, int y, int value, int iSectionLen, int mode)
{

    return;
}

int GetCh()
{
    int key_index;
    for(;;)
    {
        key_index = GetKeyCode()&0xff;
        if (key_index<0)
            break;
    }
    return abs(key_index);
}

/**********************************************************
*
* 名称: RDDelayFixedTime
* 功能: 延时一个固定的时间，在该时间内有任意键按下，按"返回"键退出
* 参数:
*       delay_second --- 延时时间，以秒为单位
*
* 返回: int
*
***********************************************************/
int RDDelayFixedTime( u_int delay_second )
{
#define KEY_EXIT			12		// “返回”

    int iRtn = 0;

    u_int iKeyPress = 0;
//	char cBuf[10];

    u_int elapsedtime = GetElapsedTime() + 1000 * delay_second;

    while( (GetElapsedTime() < elapsedtime ) || (iKeyPress != 0) ) //5sec 后或有按键退出，进入下一道流程
    {
        if( iKeyPress == 0 )
        {
            if( RDKbhit() != 0 )
                iKeyPress++;
        }
        else
        {
            if( RDKbhit() == KEY_EXIT )		//按下了返回键
                break;
        }

    }

    return iRtn ;
}
/**********************************************************
*
* 名称: RDKbhit
* 功能: 只有按键释放后才返回键值，按下时是直接返回的
* 参数:
*
* 返回: 返回按键的绝对值
*
***********************************************************/
int RDKbhit()
{
#define KEY_PRESS_TIME 2

    int iKeyCode = GetKeyCode()&0xff;
    if( iKeyCode >= 0) return 0 ;
    iKeyCode=abs(iKeyCode);
    RDBuzzerOn(KEY_PRESS_TIME);
    return iKeyCode;
}
/**********************************************************
*
* 名称: RDBuzzerOn
* 功能: 让蜂鸣器鸣叫叫一段时间，以100ms为单位
* 参数:
*       time --- 鸣叫时间
*
*
* 返回: int
*
***********************************************************/
void RDBuzzerOn(u_int time)
{
    SwitchBuzzerOn( 1 );
    RDDelay( time*100 );
    SwitchBuzzerOn( 0 );
}

extern  u_int number;


#if 0
u_char USBPipeInitialize()
{
    //SetDisplayMode(SCR_OVER_WRITE);
    SetDisplayMode(1);

    SetDisplayColor( COLOR_YELLOW );
    ETextOut(10, Y_POS[0], cTips[0], strlen(cTips[0]));
    //
    // Set Host mode
    //
    //SetUsbMode(USB_HOST_MODE);
    int IsQuit;

    u_int elapsedtime, delay_second;
    delay_second = 2;
    IsQuit = 0;

    /*
     * 初始化USB Connection
     */

    while (!IsQuit)
    {
        elapsedtime = GetElapsedTime() + 1000 * delay_second;

        while( GetElapsedTime() < elapsedtime  )
        {
            //	if (IsUsbDeviceAttached()==1)
            {
                EraseWindow(10, Y_POS[1], SCREEN_WIDTH-1,Y_POS[1]+7);
                ETextOut(10, Y_POS[1], cTips[4], strlen(cTips[4]));
                IsQuit =1 ;
                break;
            }
        }
        //	SetUsbMode(USB_HOST_MODE);
        RDDelay(1000);

    }

    return 1;

}
#endif
void DebugIntData(int x, int y,char *cTips, int ii)
{

    char cBuf[40+1], cText[40+1];

    int iLen;
    memset(cBuf,0 ,41);
    if (cTips == NULL)
        iLen = 0;
    else
    {
        iLen = strlen(cTips);
        if( iLen > 40 -8 )
            iLen = 40 -8 ;
        memcpy(cBuf, cTips, iLen);
    }

    DecimalToAscii(cText,ii,0x01010008);
    strcat(cBuf, cText);
    EraseWindow(x,y,x+strlen(cBuf)*8,y+7);
    ETextOut( x, y , cBuf, strlen(cBuf));
}



unsigned char ArmTestForWrite1()
{
    int i;
    unsigned char rtn;
    u_int time1, time2,time3;
    u_int filelen=0,err_time;

    TIME_CLOCK tm;
// @todo (union#1#): Temporarily Disabled
#if 0
    if(!(rtn=UdkListDir(DBUF)))
        ShowProblemLine();

    if(rtn=UdkOpenFile("ad45N7v4","txt"))
    {
        filelen=UdkGetFileLength(1);
        if(!UdkSetFilePointer(1,filelen))
            ShowProblemLine();

    }
    else
    {
        if(!UdkCreateFile("ad45N7v4","txt",0x20))
            ShowProblemLine();
    }

    // FOR TEST
    for (i=0; i<BUFFER_LENGTH; i++)
        DBUF[i] = i%256;
    time3 = time1 = GetElapsedTime();

    i=0;
    err_time = 0;
    while( 1 )
    {

        time3 = GetElapsedTime();
        if(!(rtn = UdkWriteFile(1,BUFFER_LENGTH, DBUF)))
        {
            ShowProblemLine();
            //	UsbHostInAbort() ;
            //	UsbHostOutAbort() ;
            err_time ++;
            TRACE(100, 170,"Reset Times=",err_time);
        }
        time2 = GetElapsedTime();
        filelen += BUFFER_LENGTH;
        TRACE(10, 90,"FileSize=",filelen );
        TRACE(10, 100,"Interval=",time2-time3);
        TRACE(10, 110,"Times=",i++);

        UGetSystemTime(&tm);

        TRACE(10, 120,"day:",tm.day);
        TRACE(10, 130,"hor:",tm.hour);
        TRACE(10, 140,"min:",tm.minute);
        TRACE(10, 150,"sec:",tm.second);

        TRACE(10, 160,"elipse:",time2 - time1);
        //	if( time2 - time1  >= 60000 ) break;

        //RDDelay(500);
        RDDelay(2);

        if(abs(GetKeyCode()&0xff)==1)
            break;

    }
// @todo (union#1#): Temporarily Disabled
#endif
    return TRUE;
}
