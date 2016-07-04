#include "string.h"
#include "sdcard.h"
#include "filefunc.h"
extern GATE_AMPMAX stGateAmpMax;
extern UDISK_INFO g_UDiskInfo;
extern NOTES	Notes;
extern CACHE	cache;

/*
void   TRACE(int x, int y,char   *prompt,   ...)
{
#pragma warn -8004
     char   buf[2048];
     va_list   argptr;
     int cnt;
     va_start(argptr,   prompt);
     cnt   =   vsprintf(buf,   prompt,   argptr);
     //OutputDebugString(buf);
     EraseWindow(x,y,strlen(buf)*8, 8);
     ETextOut(x,y,buf,strlen(buf));
     va_end(argptr);
}
*/
#define MAX_LUNS        4
#define DRV_MMC          0
#define ID_DRV1 DRV_MMC

#if defined(at91cap9stk)
#define MCI_ID 1 //no connector for MCIO/SPI0
#else
#define MCI_ID 0
#endif

#if _FS_TINY == 0

#define STR_ROOT_DIRECTORY "0:"
#else
#define STR_ROOT_DIRECTORY ""
#endif

Media medias[MAX_LUNS];

int WaveSave(int mode)	//mode > 0 为长按键，直接进入记录菜单
//返回值－1未做，0开或停自动存，1人工存，2屏拷3进入数据处理
{
    //u_int char_len = 5,row_number = 4,crow_height = 24;
    //int xpos,ypos;
    int retvalue;
    int keycode;
    //u_short pzName[12];
    //u_short *cpText;
    //u_char* epText;
    u_char *sampbuff;
    u_char echobuffer[C_LEN_SAMP];	//用来存储包络或峰值
///	u_char SaveBuffer[C_LEN_SAMP];	//用来临时保存波形
    int offset;
    int i;

    if( DataDelete(-1) >= C_FILE_NUMBER )
    {
        DisplayPrompt(6);
        DisplayPrompt(16);
        return -1;
    }
    MSetInputMode(C_INPUT_DIGITAL,0/*mode = 0*/);	/*设置当前输入法类型*/

    MSetAcquisitionEnable(0,C_SETMODE_SETSAVE);
    sampbuff = GetSampleBuffer();
    if(MGetEchoMode()==C_RF_WAVE)
    {
        for( i = 0; i< C_LEN_SAMP; i++)	echobuffer[i] = sampbuff[i]-28;
    }
    else
    {
        for( i = 0; i< C_LEN_SAMP; i++)	echobuffer[i] = sampbuff[i];
    }

///以下存缺陷波的频谱
//	Write_Number(10,30,MGetSaveFrequency(),4,0,0);
//Write_Number(10,40,MGetSaveMode(),4,0,0);
//Write_Number(10,50,MGetFunctionMode(C_ECHO_ENVELOPE),4,0,0);
//Write_Number(10,60,MGetFunctionMode(C_ECHOMAX_MEMORY),4,0,0);
//MAnyKeyReturn();
    SystemParaStore();
    ChannelParaStore();
    if( MGetSaveFrequency() && MGetSaveMode() != 1 && MGetFunctionMode(C_ECHO_ENVELOPE) != 1 && MGetFunctionMode(C_ECHOMAX_MEMORY) != 1 )
    {
        //同时存频谱且无包络、无峰值、无连续存
        int scale = MGetScaleMode();
        int dist = MGetAmpDist(-1) ;//缺陷波的声程
        int gain;
        int gain1 = MGetBaseGain();
        int gain2 = MGetCompGain();
        int gain3 = MGetSurfGain();
        int fre0 = MGetFrequence(),fre1;
        int delay0 = MGetDelay(scale);
        int echomode = MGetEchoMode();
        int range0 = MGetRange(1);
        int posi,wide,high;
        int daccomp = MGetFunctionMode(C_DEPTH_COMPENSATE);
        GATE_PEAK stGatePeak;


//		memcpy( &stGatePeak,&GatePeak,sizeof( GATE_PEAK) );
        offset = C_OFF_PEAK;
        MCopyProtectedMemory( (void*)offset, &GatePeak, sizeof(GATE_PEAK), PM_COPY_WRITE);//门参数
//        CopyMemoryToSystem( (void*)offset, sizeof(GATE_PEAK),PM_COPY_WRITE);
        MCopyProtectedMemory( &stGatePeak, (void*)offset, sizeof(GATE_PEAK), PM_COPY_READ);//门参数

        gain = gain1+gain2+gain3;
        if( gain > 20)gain -= 20;
        else gain = 0;
        MSetEchoMode(C_RF_WAVE,C_SETMODE_SETSAVE);
        MSetBaseGain(gain,C_SETMODE_SETSAVE);
        MSetCompGain(0,C_SETMODE_SETSAVE);
        MSetSurfGain(0,C_SETMODE_SETSAVE);
     //   MAdjustGain(0,0,120,120);


        posi = MGetGatePara(0,0);
        wide = MGetGatePara(0,1);
        high = MGetGatePara(0,2);


        if( dist > 1000)MSetScaleDelay(dist - 40,C_SETMODE_SETSAVE);
        else if( dist > 400)MSetScaleDelay(dist * 96/100,C_SETMODE_SETSAVE);
        else MSetScaleDelay(dist * 90/100,C_SETMODE_SETSAVE);
        MSetFunctionMode(0,C_DEPTH_COMPENSATE);
        MSetScaleMode(3,C_SETMODE_SETSAVE);
        ///MSetFrequence(100,C_SETMODE_SETSAVE);
        MSetAcquisition(1);

        MKeyRlx();

        do
        {
            MEraseWindow(0, 0, 20*C_ECHAR_HDOT, C_CCHAR_VDOT) ;
            //	CEMenuOut(0,0, _Menu_DB4 ,8,1,24);	//探头频普测试中
            TextOut(0, 0,1,17,8,(u_char*)_Menu_DB[MGetLanguage()][2],4);
            fre1 = test_fft();
            MEraseWindow(0, 0, 10*C_ECHAR_HDOT, C_CCHAR_VDOT) ;
            //	CEMenuOut(0, 0, _Menu_DB5 ,10,1,24);
            TextOut(0, 0,1,23,16,(u_char*)_Menu_DB[MGetLanguage()][3],4);
            Write_Number((5+6*MGetLanguage())*C_ECHAR_HDOT,0,fre1,4,2,0);
            keycode = MAnyKeyReturn();
        }
        while( keycode != C_KEYCOD_RETURN);

//		SystemParaRestore();
//		ChannelParaRestore();
        MSetAcquisitionEnable(0,C_SETMODE_SETSAVE);
        MSetEchoMode(echomode,C_SETMODE_SETSAVE);
        MSetBaseGain(gain1,C_SETMODE_SETSAVE);
        MSetCompGain(gain2,C_SETMODE_SETSAVE);
        MSetSurfGain(gain3,C_SETMODE_SETSAVE);
        MSetFrequence(fre0,C_SETMODE_SETSAVE);
        MSetScaleMode(scale,C_SETMODE_SETSAVE);
        MSetDelay(delay0,scale);
        MSetRange(range0,C_SETMODE_SETSAVE);
        MSetGatePara(posi,wide,high,0,C_SETMODE_SAVE);
        MSetFunctionMode(daccomp,C_DEPTH_COMPENSATE);
        //for( i = 0; i< C_LEN_SAMP; i++)	sampbuff[i] = SaveBuffer[i];
        offset = C_OFF_PEAK;
        MCopyProtectedMemory( (void*)offset, &stGatePeak, sizeof(GATE_PEAK), PM_COPY_WRITE);//门参数
//        CopyMemoryToSystem( (void*)offset,sizeof(GATE_PEAK),PM_COPY_WRITE);
        MCopyProtectedMemory( &GatePeak, (void*)offset, sizeof(GATE_PEAK), PM_COPY_READ);//门参数

    }
    else
    {
        //不存频谱
        MSetSaveFrequency(0,0);
    }

///
    //MSetHardEchoShow(0,C_SETMODE_SETSAVE);
    DisplayPrompt(15);
    retvalue = 5;

    if( MGetSaveMode() )		//连续存储，存到U盘
    {
        MKeyRlx();
        if (MGetSaveStatus() == 0)	//开始记录
        {
            //SetUsbMode(USB_HOST_MODE);
int g_xpos=50,g_ypos=100;
            if (DisplayQuery(14))	//已连接好U盘？
            {
                // U盘初始化
               // memset(&g_UDiskInfo,0,sizeof(UDISK_INFO));

                DisplayPrompt(19);	//正在连接U盘
                if(!UDiskInitialize(0))
                {
                    //初始化失败
                    //ShowProblemLine();
                    DisplayPrompt(21);	//U盘连接失败
                    MSetSaveStatus( 0,C_SETMODE_SETSAVE);
                    //SwitchUsbDevicePowerOff() ;
                    ScreenRenovate();
                    DrawDac(0);
                    DisplayPrompt(16);
                    //	SetUsbMode(USB_SLAVE_MODE);
                     return FALSE;
                }
                DisplayPrompt(20);	//U盘连接成功
                MSetAcquisition(0);
                MEraseWindow(0,0,320,C_CCHAR_VDOT);
                g_UDiskInfo.DataHeaderMark = 1;
                MSetSaveStatus( 1,C_SETMODE_SETSAVE);
                for (i=0; i<30; i++)
                {
                    Notes.name[i]=0;
                }
                if(MInputChar(0, 0, 1,Notes.name,30,30) != C_TRUE)	//最多30个字符
                {
                    //退出存储

                    FileClose(0);
                    //	UdkSafelyClosed();
                    //	SwitchUsbDevicePowerOff();
//                    memset(&g_UDiskInfo,0,sizeof(UDISK_INFO));
                    MSetSaveStatus( 0, C_SETMODE_SETSAVE);
                    ScreenRenovate();
                    DrawDac(0);
                    DisplayPrompt(23);	//U盘断开连接
                    DisplayPrompt(16);
                    //	SetUsbMode(USB_SLAVE_MODE);
                    return 0;
                }

                MSetAcquisition(1);
                //连续存

                ScreenRenovate();	/*屏幕刷新*/
                DrawDac(0);
                DisplayPrompt(16);
                return 0;
            }
            //未连接U盘，或退出连接
            MSetSaveStatus( 0, C_SETMODE_SETSAVE);
            //SwitchUsbDevicePowerOff() ;
            DisplayPrompt(16);
            //	SetUsbMode(USB_SLAVE_MODE);
        }
        else if (MGetSaveStatus() == 1)//结束记录 出现选择菜单，1.新加标签，2.暂停存储，3.结束存储。
        {
            ExpendTime(10);
            MSetAcquisitionEnable(0,C_SETMODE_SETSAVE);
            MSetHardEchoShow(0,C_SETMODE_SETSAVE);
            ClearEnvelope();
            EraseDrawRectangle(12*C_CCHAR_HDOT, C_COORVPOSI+4, 12*C_CCHAR_HDOT + (5+1+MGetLanguage()*3)*C_CCHAR_HDOT, C_COORVPOSI+16 + 30 * (4) ) ;
            TextOut(12*C_CCHAR_HDOT+4,C_COORVPOSI+8, 1, 12,24, (u_char*)_SaveMenu[MGetLanguage()][0], 4);
            TextOut(12*C_CCHAR_HDOT+4,C_COORVPOSI+8+30, 1, 12,24, (u_char*)_SaveMenu[MGetLanguage()][1], 4);
            TextOut(12*C_CCHAR_HDOT+4,C_COORVPOSI+8+30*2, 1, 12,24, (u_char*)_SaveMenu[MGetLanguage()][2], 4);
            TextOut(12*C_CCHAR_HDOT+4,C_COORVPOSI+8+30*3, 1, 12,24, (u_char*)_SaveMenu[MGetLanguage()][3], 4);
            //	CEMenuOut(12*16+4,29+4 , (u_short*)_SaveMenu ,5,4,24);	/*在指定位置根据每行字符数、行数、行高写菜单*/
            while (1)
            {
                keycode = MGetKeyCode(0);

                if (keycode == 1)	//1.新加标签
                {
                    for (i=0; i<30; i++)
                    {
                        Notes.name[i]=0;
                    }
                    g_UDiskInfo.DataHeaderMark = 1;
                    MSetAcquisition(0);
                    ScreenRenovate();	/*屏幕刷新*/
                    MEraseWindow(0,0,320,C_CCHAR_VDOT);
                    if(MInputChar(0, 0, 1,Notes.name,30,30) != C_TRUE)	//最多30个字符
                    {
                        //	g_UDiskInfo.DataHeaderMark = 0;
                    }
                    ScreenRenovate();	/*屏幕刷新*/
                    DrawDac(0);
                    MSetAcquisition(1);
                    MSetSaveStatus( 1,C_SETMODE_SETSAVE);
                    return 0;

                }
                else if (keycode == 2)	//2.暂停存储
                {
                    MSetSaveStatus( 1,C_SETMODE_SETSAVE);
                    DisplayPrompt(27);	//正在安全断开
                    FileClose(0);

//                    memset(&g_UDiskInfo,0,sizeof(UDISK_INFO));
                    //	UdkSafelyClosed();
                    MSetSaveStatus( 2,C_SETMODE_SETSAVE);

                    DisplayPrompt(16);
                    break;
                }
                else if (keycode == 3)	//3.结束存储
                {

                    DisplayPrompt(24);	//正在安全断开
                    FileClose(0);
                    DisplayPrompt(23);	//U盘断开连接
                    MSetSaveStatus( 0,C_SETMODE_SETSAVE);
//                    memset(&g_UDiskInfo,0,sizeof(UDISK_INFO));
                    //	UdkSafelyClosed();
                    //SwitchUsbDevicePowerOff() ;
                    ScreenRenovate();	/*屏幕刷新*/
                    DrawDac(0);

                    DisplayPrompt(16);
                    //	SetUsbMode(USB_SLAVE_MODE);
                    return 0;
                }
                else if (keycode == 4)	//4.存储信息
                {
                    MKeyRlx();
                    int Len,UsedSpace;
                    int sec=0,min=0,hou=0;
                    //unsigned int FreeSpaceLowPart, FreeSpaceHighPart, TotalSpaceLowPart,TotalSpaceHighPart,Cluster;
                    //Cluster = UdkGetCapacity(&FreeSpaceLowPart, &FreeSpaceHighPart, &TotalSpaceLowPart,&TotalSpaceHighPart);
                    unsigned int FreeSpaceLowPart, TotalSpaceLowPart,ClusterSize;
                    ClusterSize = SdCard_GetCapacity(&FreeSpaceLowPart,&TotalSpaceLowPart);

                    EraseDrawRectangle(1,C_COORVPOSI,C_CCHAR_HDOT*25+4,C_COORVPOSI+C_CCHAR_VDOT*6+15);
                    //	CEMenuOut(24,29+8 , (u_short*)_UDiskMenu ,15,6,24);
                    TextOut(C_CCHAR_HDOT,C_COORVPOSI+8+MGetLanguage()*4 , 1, 30,24, (u_char*)_UDiskMenu[MGetLanguage()][0], 4);
                    TextOut(C_CCHAR_HDOT,C_COORVPOSI+8+C_CCHAR_VDOT+MGetLanguage()*4 , 1, 30,24, (u_char*)_UDiskMenu[MGetLanguage()][1], 4);
                    TextOut(C_CCHAR_HDOT,C_COORVPOSI+8+C_CCHAR_VDOT*2+MGetLanguage()*4 , 1, 30,24, (u_char*)_UDiskMenu[MGetLanguage()][2], 4);
                    TextOut(C_CCHAR_HDOT,C_COORVPOSI+8+C_CCHAR_VDOT*3+MGetLanguage()*4 , 1, 30,24, (u_char*)_UDiskMenu[MGetLanguage()][3], 4);
                    TextOut(C_CCHAR_HDOT,C_COORVPOSI+8+C_CCHAR_VDOT*4+MGetLanguage()*4 , 1, 30,24, (u_char*)_UDiskMenu[MGetLanguage()][4], 4);
                    TextOut(C_CCHAR_HDOT,C_COORVPOSI+8+C_CCHAR_VDOT*5+MGetLanguage()*4 , 1, 30,24, (u_char*)_UDiskMenu[MGetLanguage()][5], 4);

                    Write_UDisk_Len(28+(4+MGetLanguage())*C_CCHAR_HDOT,C_COORVPOSI+8+4*MGetLanguage(),0,0,g_UDiskInfo.filelen);	//当前文件
                    Write_UDisk_Len(28+(4+MGetLanguage())*C_CCHAR_HDOT,C_COORVPOSI+8+C_CCHAR_VDOT+4*MGetLanguage(),TotalSpaceLowPart-FreeSpaceLowPart,ClusterSize,0);	//已用容量
                    Write_UDisk_Len(28+(4+MGetLanguage())*C_CCHAR_HDOT,C_COORVPOSI+8+C_CCHAR_VDOT*2+4*MGetLanguage(),FreeSpaceLowPart,ClusterSize,0);	//可用容量
                    Write_UDisk_Len(28+(4+MGetLanguage())*C_CCHAR_HDOT,C_COORVPOSI+8+C_CCHAR_VDOT*3+4*MGetLanguage(),TotalSpaceLowPart,ClusterSize,0);	//总容量

                    Len=(((FreeSpaceLowPart*2)/1000+0.5)*ClusterSize/2);
                    sec=Len/2;
                    if (sec>60)
                    {
                        min=sec/60;
                        sec%=60;
                        if (min>60)
                        {
                            hou=min/60;
                            min%=60;
                        }
                    }
                    Write_Number(24+(7+MGetLanguage()*3)*C_CCHAR_HDOT,C_COORVPOSI+8+C_CCHAR_VDOT*4+4*MGetLanguage(),hou,4,0,0);	//预计可存储时间 小时
                    Write_Number(24+(12+MGetLanguage()*3)*C_CCHAR_HDOT-8,C_COORVPOSI+8+C_CCHAR_VDOT*4+4*MGetLanguage(),min,2,0,0);	//预计可存储时间 分
                    Write_Number(24+(14+MGetLanguage()*4)*C_CCHAR_HDOT,C_COORVPOSI+8+C_CCHAR_VDOT*4+4*MGetLanguage(),sec,2,0,0);	//预计可存储时间 秒

                  //  DrawRectangle(24+5*C_CCHAR_HDOT,187,24+12*C_CCHAR_HDOT,187+24);	//主框
                 //   DrawRectangle(24+12*C_CCHAR_HDOT,187+4,24+13*C_CCHAR_HDOT,187+24-4);	//头框
                  //  DrawRectangle(24+12*C_CCHAR_HDOT+5,187+7,24+12*C_CCHAR_HDOT+11,187+24-14);	//小框1
                 //   DrawRectangle(24+12*C_CCHAR_HDOT+5,187+14,24+12*C_CCHAR_HDOT+11,187+24-7);	//小框2
                    UsedSpace=TotalSpaceLowPart-FreeSpaceLowPart;
                    UsedSpace=(111*UsedSpace)/TotalSpaceLowPart+0.5;
                  //  for (i=0; i<UsedSpace; i++)
                  //  {
                  //      MDrawLine(25+5*C_CCHAR_HDOT+i,188,25+5*C_CCHAR_HDOT+i,187+23,C_CR_BACK);
                  //  }

                    UsedSpace=TotalSpaceLowPart-FreeSpaceLowPart;
                    UsedSpace=(1000.0*UsedSpace)/TotalSpaceLowPart+0.5;
                    Write_Number(24+(2+MGetLanguage()*2)*C_CCHAR_HDOT,176+8+4*MGetLanguage(),UsedSpace,4,1,0);
                    EMenuOut(24+(5+MGetLanguage()*2)*C_CCHAR_HDOT,176+8+4*MGetLanguage(),"%",1,1,12);

                    while (1)
                    {
                        keycode = MGetKeyCode(0);
                        if (keycode == C_KEYCOD_RETURN || keycode == C_KEYCOD_CONFIRM)
                        {
                            break;
                        }
                    }
                    ScreenRenovate();	/*屏幕刷新*/
                    DrawDac(0);

                    DisplayPrompt(16);
                    break;
                }
                else if (keycode == C_KEYCOD_RETURN || keycode == C_KEYCOD_CONFIRM )
                {
                    break;
                }
            }
        }
        else if (MGetSaveStatus() == 2)	//重新开始
        {
            EraseDrawRectangle(12*C_CCHAR_HDOT, C_COORVPOSI+4, 12*C_CCHAR_HDOT + (6+1*MGetLanguage())*C_CCHAR_HDOT, C_COORVPOSI+4 + 30 * (2) ) ;
            TextOut(12*C_CCHAR_HDOT+4,C_COORVPOSI+8, 1, 10,24, (u_char*)_PushMenu[MGetLanguage()][0], 4);
            TextOut(12*C_CCHAR_HDOT+4,C_COORVPOSI+8+30, 1, 10,24, (u_char*)_PushMenu[MGetLanguage()][1], 4);
            while (1)
            {
                keycode = MGetKeyCode(0);

                if (keycode == C_KEYCOD_RETURN || keycode == C_KEYCOD_CONFIRM)
                {
                    ScreenRenovate();	/*屏幕刷新*/
                    DrawDac(0);
                    DisplayPrompt(16);
                    return 0;
                }
                else if (keycode ==1)	//继续存储
                {
                    break;
                }
                else if (keycode ==2)	//关闭U盘
                {
                    //	MSetSaveStatus( 1,C_SETMODE_SETSAVE);
                    //	DisplayPrompt(24);	//正在安全断开
                    //	FileClose(0);
                    //	DisplayPrompt(23);	//U盘断开连接
                    //	memset(&g_UDiskInfo,0,sizeof(UDISK_INFO));
                    //	UdkSafelyClosed();
                    //SwitchUsbDevicePowerOff() ;
                    MSetSaveStatus( 0, C_SETMODE_SETSAVE);
                    ScreenRenovate();	/*屏幕刷新*/
                    DrawDac(0);
                    DisplayPrompt(16);
                    return 0;
                }

            }
            DisplayPrompt(19);	//正在连接U盘
            if(!UDiskInitialize(0))
            {
                //初始化失败
                //ShowProblemLine();
                DisplayPrompt(21);	//U盘连接失败
                MSetSaveStatus( 0,C_SETMODE_SETSAVE);
                //SwitchUsbDevicePowerOff() ;
                //	SetUsbMode(USB_SLAVE_MODE);
                return FALSE;
            }
            DisplayPrompt(20);	//U盘连接成功
            MSetAcquisition(0);
            MEraseWindow(0,0,320,18);
            g_UDiskInfo.DataHeaderMark = 1;

            for (i=0; i<30; i++)
            {
                Notes.name[i]=0;
            }
            if(MInputChar(0, 0, 1,Notes.name,30,30) != C_TRUE)	//最多30个字符
            {
                //	g_UDiskInfo.DataHeaderMark = 0;
            }
            MSetAcquisition(1);
            MSetSaveStatus( 1,C_SETMODE_SETSAVE);
            //连续存
        }
        ScreenRenovate();	/*屏幕刷新*/
        DrawDac(0);
        DisplayPrompt(16);
        return 0;
    }

//
    ///MEraseWindow(0, 0, C_HORIDOT_VIDEO, C_VERTDOT_VIDEO) ;
    ///MChannelRenovate();
    DisplayPrompt(15);
    MSetHardEchoShow(0,C_SETMODE_SETSAVE);


    if( MGetFunctionMode(C_ECHO_ENVELOPE) == 1 || MGetFunctionMode(C_ECHOMAX_MEMORY) == 1)	//有包络
    {
        //offset = C_OFF_PEAK;
        //CopyProtectedMemory( (void*)offset, &GatePeak, sizeof(GATE_PEAK), PM_COPY_WRITE);//门参数

///		for( i = 0; i< C_LEN_SAMP; i++)	echobuffer[i] = SaveBuffer[i];
        ///offset = C_OFF_SAMP;
        ///CopyProtectedMemory( echobuffer, (void*)offset, C_LEN_SAMP, PM_COPY_READ); //以上为包络峰值
        ///MSetHardEchoShow(0,C_SETMODE_SETSAVE);

        GATE_PEAK gpTemp;
        memcpy(&gpTemp, &GatePeak, sizeof(GATE_PEAK) );
        offset = C_OFF_PEAK;
        MCopyProtectedMemory(&GatePeak, (void*)offset, C_LEN_PEAK, PM_COPY_READ);//门参数
        ExpendTime(20);
        AdjustTime.ParaDisplayTime = C_RENOVATE_TIME+1;
        MSetAcquisitionEnable(0,C_SETMODE_SETSAVE);
        ExpendTime(20);
        GetDispEchoPara();
        ExpendTime(20);
        //;	DispEchoPara2();
        ///	DrawDac(0);		//新画DAC
        ///DrawWave(echobuffer, 0, C_LEN_SAMP);		//画包络峰值


        if( MGetFunctionMode(C_ECHOMAX_MEMORY) == 1)	//有峰值
            retvalue = 7;		//询问是否存峰值
        else	//询问是否存包络
            retvalue = 6;

        if( DisplayQuery(retvalue) == 0)
        {
            offset = C_OFF_PEAK;
            MCopyProtectedMemory( (void*)offset, &GatePeak, C_LEN_PEAK, PM_COPY_WRITE);//门参数
//            CopyMemoryToSystem( (void*)offset,C_LEN_PEAK,PM_COPY_WRITE);
            ClearEnvelope();	//清包络或峰值
        }
        else
        {
            ChannelPara.wavepara[0] = stGateAmpMax.para[0];
            ChannelPara.wavepara[1] = stGateAmpMax.para[1];
            ChannelPara.wavepara[2] = stGateAmpMax.para[2];
            ChannelPara.wavepara[3] = stGateAmpMax.para[3];
            SystemParaStore();
            ChannelParaStore();
            DataSave();
            DisplayPrompt(15);

            ClearEnvelope();	//清包络或峰值

            MSetAcquisitionEnable(1,C_SETMODE_SETSAVE);
            MSetHardEchoShow(1,C_SETMODE_SETSAVE);
            MSetSystem();
            ScreenRenovate();	/*屏幕刷新*/
            DrawDac(0);		//新画DAC
            return 0;
        }
    }
//*/
///	if( MGetSaveFrequency() && MGetSaveMode() != 1 && MGetFunctionMode(C_ECHO_ENVELOPE) != 1 && MGetFunctionMode(C_ECHOMAX_MEMORY) != 1 )
///		for( i = 0; i< C_LEN_SAMP; i++)	echobuffer[i] = SaveBuffer[i];
///	else
///		for( i = 0; i< C_LEN_SAMP; i++)	echobuffer[i] = sampbuff[i];
///	MSetHardEchoShow(0,C_SETMODE_SETSAVE);
   // ClearEnvelope();	//清包络或峰值
    MEraseWindow(0, 0, C_HORIDOT_VIDEO, C_VERTDOT_VIDEO) ;
    MChannelRenovate();
    DrawDac(0);		//新画DAC

    offset = C_OFF_PEAK;
    MCopyProtectedMemory( (void*)offset, &GatePeak, C_LEN_PEAK, PM_COPY_WRITE);//门参数
//    CopyMemoryToSystem( (void*)offset,C_LEN_PEAK,PM_COPY_WRITE);
    offset = C_OFF_SAMP;
    MCopyProtectedMemory( (void*)offset, echobuffer, C_LEN_SAMP, PM_COPY_WRITE); //以上为波形
//    CopyMemoryToSystem( (void*)offset,C_LEN_SAMP,PM_COPY_WRITE);

///
///	for( i = 0; i< C_LEN_SAMP; i++)	echobuffer[i] = 80;
///	offset = C_OFF_SAMP;
///	MCopyProtectedMemory( echobuffer, (void*)offset, C_LEN_SAMP, PM_COPY_READ); //以上为波形
///
    DrawWave(echobuffer, 0, C_LEN_SAMP);		//画波形
    if( DataSave() == 1)retvalue =  1;
    else retvalue = -1;

    DisplayPrompt(16);
    SystemParaRestore();
    ChannelParaRestore();
    MSetSystem();
    ClearEnvelope();	//清包络或峰值
    ScreenRenovate();	/*屏幕刷新*/
    DrawDac(0);		//新画DAC
    MSetAcquisitionEnable(1,C_SETMODE_SETSAVE);
    MSetHardEchoShow(1,C_SETMODE_SETSAVE);
    return 1;

}

extern const u_int c_crPara[];
extern const u_int Echo_crPara[];

extern u_char crPara[];
int DataMenu(int mode)	//mode > 0 为长按键，直接进入记录菜单
{
    //返回值－1未做，0开或停自动存，1人工存，2屏拷3进入数据处理
    u_int char_len = 5,row_number = 2,crow_height = 48;
    int xpos,ypos;
    int retvalue;
    int keycode;
//	u_short pzName[12];
    u_short *cpText;
//	u_char* epText;
    u_char *sampbuff;
    u_char echobuffer[C_LEN_SAMP];	//用来存储包络或峰值
    u_char tempcrPara[C_CR_MAX+12];
    int offset;
    int i;


    MSetAcquisitionEnable(0,C_SETMODE_SETSAVE);
    sampbuff = GetSampleBuffer();
    for( i = 0; i< C_LEN_SAMP; i++)	echobuffer[i] = sampbuff[i];
    MSetHardEchoShow(0,C_SETMODE_SETSAVE);
    ClearEnvelope();	//清包络或峰值
    MSetSaveStatus( 0,C_SETMODE_SETSAVE);//停止连续记录
    SystemParaStore();
    ChannelParaStore();


    DisplayPrompt(15);
    retvalue = 5;

    if(1)	//存储波形
    {
     /*   DrawWave(echobuffer, 0, C_LEN_SAMP);		//画波形

        xpos = C_COORWIDTH - ( char_len + 1 ) * C_CCHAR_HDOT;
        ypos = C_COORVPOSI + 1;
        EraseDrawRectangle(xpos, ypos, xpos + ( char_len + 1 )*C_CCHAR_HDOT, ypos + crow_height * (row_number) ) ;
        for (i=0; i<row_number; i++)
        {
            TextOut(xpos+4,ypos+4+i*crow_height,1,10,crow_height,(u_char*)_DataMenu_A1[MGetLanguage()][i],0);
            if (MGetLanguage())
            {
                TextOut(xpos+4,ypos+4+i*crow_height+10,1,10,8,(u_char*)_DataMenu_A1[MGetLanguage()][i]+10,0);
            }
        }*/
        while(1)
        {
            keycode =2;// MAnyKeyReturn();
            MKeyRlx();

//			if(keycode == 1)
//				if( DataSave() == 1)retvalue =  1;
//				else retvalue = -1;
//			else if(keycode == 2)
            if(keycode == 1)
            {
                Usb2PC();
                retvalue = 1;
                break;
                //retvalue = MGetColorMode();
                for (i=0; i<C_CR_MAX+12; i++)
                {
                    tempcrPara[i]=crPara[i];
                }
                MSaveDefualtColor(1);
                ScreenRenovate();	/*屏幕刷新*/
                DrawWave(echobuffer, 0, C_LEN_SAMP);		//画波形
                DrawDac(0);		//新画DAC
                ScreenCopy();
                //	MSaveDefualtColor(retvalue);
                offset = C_OFF_TEMP+60;	//颜色存储位置
                MCopyProtectedMemory( (void*)offset, tempcrPara, C_CR_MAX+12, PM_COPY_WRITE);
//                CopyMemoryToSystem( (void*)offset,C_CR_MAX+12,PM_COPY_WRITE);
                MCopyProtectedMemory( crPara, (void*)offset, C_CR_MAX+12, PM_COPY_READ);
                MSetColor(C_CR_DEFAULT);
                SetBackgroundColor(c_crPara[crPara[ C_CR_BACK] ]);
                retvalue = 1;
                break;
            }
            else if(keycode == 2)
            {
                //	retvalue = MGetColorMode();
                //	MSaveDefualtColor(0);
                for (i=0; i<C_CR_MAX+12; i++)
                {
                    tempcrPara[i]=crPara[i];
                }
                MSaveDefualtColor(1);
                DataProcedure();
                //	MSaveDefualtColor(retvalue);
                //	int offset ;
                offset = C_OFF_TEMP+60;	//颜色存储位置
                MCopyProtectedMemory( (void*)offset, tempcrPara, C_CR_MAX+12, PM_COPY_WRITE);
//                CopyMemoryToSystem( (void*)offset,C_CR_MAX+12,PM_COPY_WRITE);
                MCopyProtectedMemory( crPara, (void*)offset, C_CR_MAX+12, PM_COPY_READ);
                MSetColor(C_CR_DEFAULT);
                SetBackgroundColor(c_crPara[crPara[ C_CR_BACK] ]);
                retvalue = 2;
                break;
            }
            else if(keycode == C_KEYCOD_RETURN || keycode == C_KEYCOD_CONFIRM)	break;
            else continue;
            break;
        }
    }

    SystemParaRestore();
    ChannelParaRestore();
    MSetAcquisition(1);
    ScreenRenovate();	/*屏幕刷新*/
    DrawDac(0);		//新画DAC
    return retvalue;
}

int DataProcedure()	//数据处理
{
    u_int char_len = 3,row_number ,crow_height = 48;
//	int deci_len;
    int xpos,ypos;
//	int	menu_xpos = 0,menu_ypos = 0;
    int retvalue;
    int keycode;
    int channel= MGetChannel();
    int SaveMode = MGetSaveMode();
    int datachannel;
    int file_no,file_total;
    u_char echobuffer[C_LEN_SAMP];	//用来存储包络或峰值
    u_short freqbuffer[C_LEN_SAMP];	//用来存频谱
    int language;
    int SoundAlarm;
//	u_short pzName[12];
    u_short *cpText;
//	u_char* epText;
    int offset;
    int i;//,j;
    int temp;
    bool bShowWeld = false;
    int view=0;
    row_number = 8;

    language=MGetLanguage();
    SoundAlarm=MGetSoundAlarm(0);
    MSetAcquisition(0);

    file_no = 0;
    file_total = DataDelete(-1);	//参数为负或正常删除返回剩余数据总数，参数过大返回－1
    MFclearScreen();
    retvalue = 1;
    keycode = 1;
    DisplayPrompt(15);

    MSetColor(C_CR_MENU);
    while(retvalue)
    {
        if(file_total <= 0)
        {
            DisplayPrompt(2);
            return -1;
        }
        if( retvalue ==1)
        {
            DataDisplay(file_no,language);
            //	MSetChannel(channel,C_SETMODE_SETSAVE);
            MSetAcquisition(0);
        }

        MSetLanguage(language);
        MSetSoundAlarm(0,SoundAlarm);



#if C_DEVTYPE == 1
        row_number = 7;//没有连续存
#else
        if( MGetSaveFrequency())row_number = 8;
        else row_number = 7;
#endif

        crow_height = ( C_COORHEIGHT/row_number );
        xpos = C_COORWIDTH -(3+MGetLanguage()*2) * C_CCHAR_HDOT; //C_HORIDOT_SCREEN
       /* if (MGetSaveFrequency()==1 && language==1)
        {
            xpos = C_COORWIDTH - (char_len - 1) * C_CCHAR_HDOT;
        }
        else if (language==1)
        {
            xpos = C_COORWIDTH - (char_len - 2) * C_CCHAR_HDOT;
        }
        else
        {
            xpos = C_COORWIDTH ;
        }*/
        ypos = C_COORVPOSI + 1;
        //cpText = (u_short*)_DataMenu_A2 ;
        //	MPushWindow(xpos-16, ypos, xpos + 3*C_CCHAR_HDOT+20, ypos + crow_height * row_number ) ;
        if(view%2==0)
        {
            EraseDrawRectangle(xpos, ypos, xpos + (3+MGetLanguage()*2)*C_CCHAR_HDOT+16, ypos + crow_height * row_number ) ;
            //	CEMenuOut(xpos+4,ypos+4 , cpText ,char_len,row_number,crow_height);	/*在指定位置根据每行字符数、行数、行高写菜单*/
            for(i=0;i<row_number;i++)
            {
                TextOut(xpos+4,ypos+4+i*crow_height,1,6+MGetLanguage()*2,crow_height,(u_char*)_DataMenu_A2[MGetLanguage()][i],4);
            }

    #if C_UDISK == 0
            if (MGetSaveFrequency())
            {
                TextOut(xpos+4,ypos+4+crow_height*7,1,10,crow_height,(u_char*)_DataMenu_A2[MGetLanguage()][9],4);
            }
    #endif
            MDrawLine(xpos+1,ypos+crow_height*3,xpos + (3+MGetLanguage()*2)*C_CCHAR_HDOT+15,ypos+crow_height*3,C_CR_BACK);

        }

        while(1)
        {
            keycode = MGetKeyCode(20);

            if( keycode >= 0 && keycode < row_number );
#if C_DEVTYPE == 14
            else if( keycode == C_KEYCOD_LEFT || keycode == C_KEYCOD_RIGHT || keycode == C_KEYCOD_PLUS || keycode == C_KEYCOD_MINUS );
#else
            else if( keycode == C_KEYCOD_PLUS || keycode == C_KEYCOD_MINUS );
#endif
            else if( keycode == C_KEYCOD_RETURN || keycode == C_KEYCOD_CONFIRM )
            {
                view++;
            }
            else
            {
#if C_ROTARY > 0
                keycode = MGetRotary(-1);
                if( keycode > 0 )
                {
                    keycode = C_KEYCOD_PLUS;
                }
                else if( keycode < 0)
                {
                    keycode = C_KEYCOD_MINUS;
                }
                else continue;
#else
                continue;
#endif
            }
            //	MPopWindow();
            MKeyRlx();
            switch(keycode)
            {
            case 1:		//安日期查
                if ((temp = SeekDate(file_total)) == file_total)
                {
                    DisplayPrompt(10);
                }
                else file_no = temp;
                retvalue = 1;
                break;
            case 2:		//按编号查
                if ((temp = SeekName(file_total)) == file_total)
                {
                    DisplayPrompt(10);
                }
                else file_no = temp;
                retvalue = 1;
                break;
            case 3:		//屏拷
                view++;

                if(MGetSaveFrequency())retvalue = 1;
                //ScreenCopy();
                //retvalue = 1;
                break;
            case 4:		//清除
                retvalue = 1;
                if( MGetSaveMode() && MGetSaveStatus() )//连续数据
                {
                    if( DisplayQuery(13) == 1)//询问是否清连续数据
                    {
                        //MPopWindow();
                        EMenuOut(0,0,"Delete...        ",16,1,12);
                        /*
                        while( MGetSaveMode() && MGetSaveStatus() )//连续数据
                        {
                        	if( DataDelete(file_no) == file_no)file_no--;	//删除一个数据，
                        	if(file_no < 0)
                        	{
                        		xpos = 0 ;		//显示空格
                        		ypos = 0 ;
                        		char_len = 6;
                        		row_number = 1;
                        		crow_height = 24;
                        		CMenuOut(xpos, ypos, _Message_A1 + 15 * char_len, 7, row_number, crow_height);

                        		DisplayPrompt(5);
                        		retvalue = 0;
                        		file_total = 0;
                        		break;
                        	}
                        	DataRestore(file_no);
                        	file_total--;
                        }
                        /*/
                        if( MGetSaveMode() && MGetSaveStatus() )//连续数据
                        {
                            int prename = ShortStr2Number( &fat.name[1],8);
                            int currname  = prename;
                            int file_no_old = file_no;
                            int del_number;
                            int file_offset,file_offset_old,fat_offset_old,fat_offset;
                            //Write_Number(20,30,file_no,4,0,0);
                            //Write_Number(20,40,file_total,4,0,0);
                            //Write_Number(20,50,prename,8,0,0);
                            while( currname == prename)
                            {
                                file_no++;
                                if(file_no >= file_total)break;
                                DataRestore(file_no);
                                offset = C_OFF_FILEINFO + C_SIZE_INT;
                                MCopyProtectedMemory( &fat, (void*)offset, C_LEN_FAT, PM_COPY_READ);
                                currname  = ShortStr2Number( &fat.name[1],8);
                                //Write_Number(100,30,currname,8,0,0);
                                prename++;
                                //ExpendTime(10);
                            }
                            //if( file_no >= file_total )del_number = file_no - file_no_old ;	//被连续删除的个数
                            //else del_number = file_no - file_no_old ;	//被连续删除的个数
                            del_number = file_no - file_no_old ;	//被连续删除的个数
                            //Write_Number(100,40,del_number,4,0,0);
                            //if( del_number > 0 )
                            {
                                fat_offset_old = C_OFF_FAT + C_LEN_FAT * file_no_old;	//文件头位置
                                fat_offset = fat_offset_old + C_LEN_FAT * del_number;
                                file_offset_old = C_OFF_FILE + GetDataPosi(file_no_old);	//文件体位置
                                file_offset = file_offset_old + C_LEN_FILE * del_number;

                                ///for( i = file_no ; i <= file_total; i++ )
                                for( i = file_no ; i < file_total; i++ )
                                {
                                    MCopyProtectedMemory( (void*)fat_offset_old, (void*)fat_offset, C_LEN_FAT, PM_COPY_MOVE);
                                    MCopyProtectedMemory( (void*)file_offset_old, (void*)file_offset, C_LEN_FILE, PM_COPY_MOVE);
                                    fat_offset_old += C_LEN_FAT;
                                    fat_offset += C_LEN_FAT;
                                    file_offset_old += C_LEN_FILE;
                                    file_offset += C_LEN_FILE;

                                }
                                file_no = file_no_old;
                                file_total -= del_number;
                                if( file_no >= file_total )file_no = 0;
                            }
                            //MAnyKeyReturn();

                        }
                    }
                    else
                    {
                        if( DataDelete(file_no) == file_no)file_no--;	//删除一个数据，
                        if(file_no < 0)
                        {
                            xpos = 0 ;		//显示空格
                            ypos = 0 ;
                            char_len = 6;
                            row_number = 1;
                            crow_height = 24;
                            //CMenuOut(xpos, ypos, _Message_A1 + 15 * char_len, 6, row_number, crow_height);
                            TextOut(xpos, ypos,row_number,23,crow_height,(char *)_Message_A1[MGetLanguage()][15],4);

                            DisplayPrompt(5);
                            retvalue = 0;
                            //return 0;
                        }
                        file_total--;
                    }
                }
                else if( DisplayQuery(0) == 1)
                {
                    //MPopWindow();
                    if( DataDelete(file_no) == file_no)file_no--;	//删除一个数据，
                    if(file_no < 0)
                    {
                        xpos = 0 ;		//显示空格
                        ypos = 0 ;
                        char_len = 6;
                        row_number = 1;
                        crow_height = 24;
                        //	CMenuOut(xpos, ypos, _Message_A1 + 15 * char_len, 6, row_number, crow_height);
                        TextOut(xpos, ypos,row_number,22,crow_height,(char *)_Message_A1[MGetLanguage()][15],4);

                        DisplayPrompt(5);
                        retvalue = 0;
                        //return 0;
                    }
                    file_total--;
                }
                offset = C_OFF_FILEINFO;
                MCopyProtectedMemory( (void*)offset, &file_total, C_SIZE_INT, PM_COPY_WRITE);
                CopyMemoryToSystem((void*)offset, C_OFF_FAT-offset,PM_COPY_WRITE);

                break;
            case 5:		//报告
                Usb2PC();
             /*   if(language==1)
                {
                    ReportTextE(language,file_no);
                }
                else
                {
                    ReportText(0,file_no);
                }*/
                retvalue = 1;
                break;
            case 6:		//调用
                datachannel=MGetChannel();
                MSetChannel(channel,C_SETMODE_SETSAVE);
                if( DataCall(file_no,SaveMode) == 1 )retvalue = 0;
                else MSetChannel(datachannel,C_SETMODE_SETSAVE);
                ExpendTime(20);
                if(MGetSaveFrequency())retvalue = 1;

                break;
            case 8:		//播放->输出
#if C_UDISK == 1
                break;
                SaveToUDisk(file_no);
                //file_no = AutoSavePlay(file_no,file_total);
                retvalue = 1;
                break;
            case 7:		//频谱
#endif
                if( MGetSaveFrequency() )
                {
                    offset = C_OFF_FREQ;
                    MCopyProtectedMemory(echobuffer, (void*)offset, C_LEN_SAMP, PM_COPY_READ);	//波形参数
                    for( i = 0 ; i < C_LEN_SAMP; i++)
                    {
                        freqbuffer[i] = echobuffer[i];//一个是16位，一个是8位
                    }
                    offset = C_OFF_SAMP;
                    MCopyProtectedMemory(echobuffer, (void*)offset, C_LEN_SAMP, PM_COPY_READ);	//波形参数
                    if( retvalue == 2)
                    {
                        DrawCurver( freqbuffer,0,C_LEN_SAMP);//画频谱
                        DrawWave(echobuffer,0,C_LEN_SAMP);		//清波形
                        WriteScale();
                        retvalue = 1;
                    }
                    else
                    {
                        DrawWave(echobuffer,0,C_LEN_SAMP);		//清波形
                        DrawCurver( freqbuffer,0,C_LEN_SAMP);//画频谱
                        int range  = 0 ;
                        xpos = 0;
                        ypos = 458;
                        //	if( MGetEchoMode() == C_RF_WAVE)
                        {
                            range = MGetFrequence()/10;
                            if (range < 8) range = 10.0;
                            else if(range < 16)range = 20.0;
                            else if(range < 40)range = 50.0;
                            else if(range < 80)range = 100.0;
                            else range = 200.0;
                            range = range/5;
                        }
                        //	else range = 200/5;
                        for(i=0; i<5 ; i++)
                        {
                            Write_Number(xpos,ypos,i*range,5,1,0);
                          //  xpos += 7 * 8;
                            xpos += 6 * C_ECHAR_HDOT;
                        }
                        retvalue = 2;
                    }
                }
                break;
            case 0:
                if( !bShowWeld && stWeldPara.type > 0)
                {
                    ShowWeld(20,58+240,520,160,MGetAngle(0), MGetAmpDist1(0) );
                    //	bShowWeld = true;
                }
                else
                {
                    bShowWeld = false;
                }
                break;
#if C_DEVTYPE == 14
            case C_KEYCOD_RIGHT:
#endif
            case C_KEYCOD_PLUS:
                file_no++;
                if(file_no >= file_total)file_no = 0;
                retvalue = 1;
                break;
#if C_DEVTYPE == 14
            case C_KEYCOD_LEFT:
#endif
            case C_KEYCOD_MINUS:
                file_no--;
                if(file_no < 0)file_no = file_total-1;
                retvalue = 1;
                break;
            case C_KEYCOD_RETURN:
                //case C_KEYCOD_CONFIRM:
                retvalue = 0;
                break;
            }
            MKeyRlx();
            break;
        }
    }
    SystemParaRestore();
    ChannelParaRestore();
    return 1;
}

int DataSave()		//返回1表示已存，0未存
{
    ///输入编号
    int offset;
//	int retvalue;
    u_short *cpText;
    int file_total;
    /*//
    	u_char echobuffer[C_LEN_SAMP];

    	MChannelRenovate();
    	offset = C_OFF_SAMP;
    	MCopyProtectedMemory(echobuffer, (void*)offset, C_LEN_SAMP, PM_COPY_READ);	//波形参数
    	DrawWave(echobuffer,0,C_LEN_SAMP);		//画波形
    	Write_Number(10,30,1,2,0,0);
    	MAnyKeyReturn();
    //*/

//	ChannelParaStore();
//	SystemParaStore();
    DisplayPrompt(15);
    DispEchoPara2();
    offset = C_OFF_FILEINFO;
    MCopyProtectedMemory( &file_total, (void*)offset, C_SIZE_INT, PM_COPY_READ);

    fat.posi = file_total;  //C_LEN_FILE *
    fat.date = MGetSystemTime(-1);//MGetCurrentDate();
    fat.name[0] = 0;

    if(MInputChar(0, 0, 1,fat.name,16,8) != C_TRUE)return 0;
    offset = C_OFF_FILEINFO + C_SIZE_INT;
    MCopyProtectedMemory( (void*)offset, &fat, C_LEN_FAT, PM_COPY_WRITE); //以上为文件头


    if(DataStore(file_total) != 1) return 0;
//DataRestore(file_total);
    offset = C_OFF_FILEINFO;
    MCopyProtectedMemory( &file_total, (void*)offset, C_SIZE_INT, PM_COPY_READ);
    file_total++;
    MCopyProtectedMemory( (void*)offset, &file_total, C_SIZE_INT, PM_COPY_WRITE);
//    CopyMemoryToSystem( (void*)offset,C_SIZE_INT,PM_COPY_WRITE);

    offset = C_OFF_FILEINFO;
    CopyMemoryToSystem( (void*)offset,C_OFF_FAT-offset,PM_COPY_WRITE);
    DisplayPrompt(14);
    ExpendTime(10);

    return 1;
}

int DataStore(int file_no)
{
    //存储波形及数据
    int file_total;
    long int offset,fat_offset,file_offset;
    int i;
//	int data_len;

    int Channel = MGetChannel();
    int nCurrentUser = GetCurrentUser();


    file_total = DataDelete(-1);	//参数为负或正常删除返回剩余数据总数，参数过大返回－1

    file_total =cache.cache[file_total];
   /* for(i=0;i<260;i++)
    {
    Write_Number((i%10)*3*16,(i/10)*24,cache.cache[i],4,0,0);
    }*/

    offset = C_OFF_FILEINFO + C_SIZE_INT;
    fat_offset = C_OFF_FAT + C_LEN_FAT * file_total;
    MCopyProtectedMemory((void*)fat_offset, (void*)offset, C_LEN_FAT, PM_COPY_MOVE); //以上为文件头

    CopyMemoryToSystem( (void*)fat_offset,C_LEN_FAT,PM_COPY_WRITE);

    MCopyProtectedMemory(&fat, (void*)fat_offset, C_LEN_FAT, PM_COPY_READ);

    file_offset = C_OFF_FILE + C_LEN_FILE*file_total ;
///Write_Number(100,30,file_offset,10,0,0);///
    offset = C_OFF_SYSTEM +  C_LEN_SYSPARA * nCurrentUser;
    MCopyProtectedMemory( (void*)file_offset, (void*)offset, C_LEN_SYSPARA, PM_COPY_MOVE);//系统参数
//    CopyMemoryToSystem( (void*)file_offset,C_LEN_SYSPARA,PM_COPY_WRITE);
//Write_Number(10,40,file_offset,10,0,0);
//Write_Number(100,40,offset,4,0,0);
    file_offset += C_LEN_SYSPARA;
    ///offset = C_OFF_CHANNEL + C_LEN_CHNPARA * MGetChannel();
    offset = C_OFF_CHANNEL + C_LEN_CHNPARA * C_CHMAX * nCurrentUser + C_LEN_CHNPARA * Channel;
    //MCopyProtectedMemory( (void*)file_offset, (void*)offset, C_LEN_CHNPARA, PM_COPY_MOVE);//通道参数

    MCopyProtectedMemory((void*)file_offset, (void*)offset, C_LEN_CHSTATUS, PM_COPY_MOVE);
//    CopyMemoryToSystem( (void*)file_offset,C_LEN_CHSTATUS,PM_COPY_WRITE);
//Write_Number(10,50,file_offset,10,0,0);
//Write_Number(6100,50,offset,4,0,0);

    offset += C_LEN_CHSTATUS;
    file_offset += C_LEN_CHSTATUS;
    MCopyProtectedMemory((void*)file_offset, (void*)offset, C_LEN_CHPARA, PM_COPY_MOVE);
//    CopyMemoryToSystem( (void*)file_offset,C_LEN_CHPARA,PM_COPY_WRITE);
//Write_Number(10,60,file_offset,10,0,0);
//Write_Number(6100,60,offset,4,0,0);

    offset += C_LEN_CHPARA;
    file_offset += C_LEN_CHPARA;
    MCopyProtectedMemory((void*)file_offset, (void*)offset, C_LEN_GATEPARA, PM_COPY_MOVE);
//    CopyMemoryToSystem( (void*)file_offset,C_LEN_GATEPARA,PM_COPY_WRITE);
//Write_Number(10,70,file_offset,10,0,0);
//Write_Number(100,70,offset,4,0,0);

    offset += C_LEN_GATEPARA;
    file_offset += C_LEN_GATEPARA;
    MCopyProtectedMemory((void*)file_offset, (void*)offset, C_LEN_GATEPARA, PM_COPY_MOVE);
//    CopyMemoryToSystem( (void*)file_offset,C_LEN_GATEPARA,PM_COPY_WRITE);
//Write_Number(10,80,file_offset,10,0,0);
//Write_Number(100,80,offset,4,0,0);

    offset += C_LEN_GATEPARA;
    file_offset += C_LEN_GATEPARA;
    MCopyProtectedMemory( (void*)file_offset, (void*)offset, C_LEN_DAC, PM_COPY_MOVE);
//    CopyMemoryToSystem( (void*)file_offset,C_LEN_DAC,PM_COPY_WRITE);
//Write_Number(10,90,file_offset,10,0,0);
//Write_Number(100,90,offset,4,0,0);

    offset += C_LEN_DAC;
    file_offset += C_LEN_DAC;
    MCopyProtectedMemory( (void*)file_offset, (void*)offset, C_LEN_WELD, PM_COPY_MOVE);	//焊缝
//    CopyMemoryToSystem( (void*)file_offset,C_LEN_WELD,PM_COPY_WRITE);

    file_offset += C_LEN_WELD;
    offset = C_OFF_PEAK;
    MCopyProtectedMemory( (void*)file_offset, (void*)offset, C_LEN_PEAK, PM_COPY_MOVE);//门参数
//    CopyMemoryToSystem( (void*)file_offset,C_LEN_PEAK,PM_COPY_WRITE);

//Write_Number(10,100,file_offset,10,0,0);
//Write_Number(100,100,offset,4,0,0);

    file_offset += C_LEN_PEAK;
    offset = C_OFF_SAMP;
    MCopyProtectedMemory( (void*)file_offset, (void*)offset, 1*C_LEN_SAMP, PM_COPY_MOVE);	//波形参数
//    CopyMemoryToSystem( (void*)file_offset,C_LEN_SAMP,PM_COPY_WRITE);
//Write_Number(10,110,file_offset,10,0,0);
//Write_Number(100,110,offset,4,0,0);

    file_offset += C_LEN_SAMP;
    offset = C_OFF_FREQ;
    MCopyProtectedMemory( (void*)file_offset, (void*)offset, 1*C_LEN_SAMP, PM_COPY_MOVE);	//频谱
    //CopyMemoryToSystem( (void*)file_offset,C_LEN_SAMP,PM_COPY_WRITE);
    file_offset=C_OFF_FILE + C_LEN_FILE*file_total ;
    CopyMemoryToSystem( (void*)file_offset,C_LEN_FILE,PM_COPY_WRITE);

//Write_Number(10,120,file_offset,10,0,0);
//Write_Number(100,120,offset,4,0,0);
//MAnyKeyReturn();
    //file_offset += 1*C_LEN_SAMP;
    //offset = C_OFF_DAC;
    //MCopyProtectedMemory( (void*)file_offset, (void*)offset, C_LEN_DAC, PM_COPY_MOVE);	//DAC参数
    //MCopyProtectedMemory( (void*)file_offset, &Dac, C_LEN_DAC, PM_COPY_WRITE);	//DAC参数

    return 1;
}

void DispEchoPara2(void)
{
    int para1,para2,para3,para4;
//	int xpos,ypos,row_height = 24;
    int mode1,mode2;

    mode1 = MGetBGateMode();
    mode2 = MGetEquivalentMode();	//mode1=0B门失波1进波，mode2<0波高.>0dB数
    if( MGetStdMode() == C_STD_AWS_NUM - 1)//AWS
    {
        mode1 = 0;
        mode2 = -1;
    }


    para1 = ChannelPara.wavepara[0] ;	//距离
    para2 = ChannelPara.wavepara[1] ;	//距离
    para3 = ChannelPara.wavepara[2] ;	//距离
    para4 = ChannelPara.wavepara[3] ;	//距离
    if(mode1 == 0)
    {
        if( mode2 > 0)
        {
            //将波高转换dB值

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
                break;
            case 10:
                break;
            case 11://显示孔径
                para4 += MGetLineGain(2);
                if( MGetUnitType() > 0 )
                {
                    Write_Number(23*C_ECHAR_HDOT,C_COORVPOSI - C_ECHAR_VDOT-VertOffsetScreen-2,(int)( 10*powf( 10,( para4 + 120 ) / 400.0)*100/25.4  + 0.5), 5,3,0);
                }
                else
                {
                    Write_Number(23*C_ECHAR_HDOT,C_COORVPOSI - C_ECHAR_VDOT-VertOffsetScreen-2,(int)( 10*powf( 10,( para4 + 120 ) / 400.0)  + 0.5), 3,1,0);
                }
//				Write_Number(23*8,C_COORVPOSI - 10,(int)( 10*powf( 10,( para4 + 120 ) / 400.0)  + 0.5), 3,1,0);
                para4 -= MGetLineGain(2);
                break;
            }

        }

    }
    WritePeakInfo(mode1,mode2,para1,para2,para3,para4);
    if( MGetStdMode() == C_STD_AWS_NUM - 1)//AWS
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

        x += 8*2;
        Write_Number(x,ypos,para1,2,0,0);
        x+= 8*2;
        EMenuOut(x,ypos,"-",1,1,8);
        x += 8*1;
        Write_Number(x,ypos,para2,2,0,0);
        x+= 8*2;
        EMenuOut(x,ypos,"-",1,1,8);
        x += 8*1;
        Write_Number(x,ypos,para3,2,0,0);
        x+= 8*2;
        EMenuOut(x,ypos,"=",1,1,8);
        x += 8*1;
        Write_Number(x,ypos,para1-para2-para3,3,0,1);
    }
    if( MGetDepth() != 0)
    {
        Write_Number(28*16,0,MGetDepth(),5,1,1);
    }
//Write_Number(10,30,10,4,0,0);
    return;
}


int DataRestore(int file_no1)
{
    //存储波形及数据
    long int offset,fat_offset,file_offset;
//	int data_len;
//	u_char* sampbuff;
//	int dataposi;
    int file_no;
    file_no=cache.cache[file_no1];       //2013/5/2
    fat_offset = C_OFF_FAT + C_LEN_FAT * file_no ;
    offset = C_OFF_FILEINFO + C_SIZE_INT;
    MCopyProtectedMemory( (void*)offset, (void*)fat_offset, C_LEN_FAT, PM_COPY_MOVE); //以上为文件头

    short prnmode = MGetPrinterMode();
    file_offset = C_OFF_FILE + C_LEN_FILE * file_no;
///Write_Number(100,30,file_offset,10,0,0);///
    MCopyProtectedMemory( &SystemStatus, (void*)file_offset,C_LEN_SYSPARA, PM_COPY_READ);//系统参数

    MSetPrinterMode(prnmode,C_SETMODE_SETSAVE);

    file_offset += C_LEN_SYSPARA;
    MCopyProtectedMemory(&ChannelStatus, (void*)file_offset, C_LEN_CHSTATUS, PM_COPY_READ);

    file_offset += C_LEN_CHSTATUS;
    MCopyProtectedMemory(&ChannelPara, (void*)file_offset, C_LEN_CHPARA, PM_COPY_READ);

    file_offset += C_LEN_CHPARA;
    MCopyProtectedMemory(&AGatePara, (void*)file_offset, C_LEN_GATEPARA, PM_COPY_READ);

    file_offset += C_LEN_GATEPARA;
    MCopyProtectedMemory(&BGatePara, (void*)file_offset, C_LEN_GATEPARA, PM_COPY_READ);

    file_offset += C_LEN_GATEPARA;
    offset = C_OFF_DAC;
    MCopyProtectedMemory( (void*)offset, (void*)file_offset, C_LEN_DAC, PM_COPY_MOVE);	//DAC参数
//Write_Number(10,90,file_offset,10,0,0);
//Write_Number(100,90,offset,4,0,0);
    file_offset += C_LEN_DAC;
    MCopyProtectedMemory(&stWeldPara, (void*)file_offset, C_LEN_WELD, PM_COPY_READ);

    file_offset += C_LEN_WELD;
    offset = C_OFF_PEAK;
    MCopyProtectedMemory( (void*)offset, (void*)file_offset, C_LEN_PEAK, PM_COPY_MOVE);//门参数
//Write_Number(10,100,file_offset,10,0,0);
//Write_Number(100,100,offset,4,0,0);

    file_offset += C_LEN_PEAK;
    offset = C_OFF_SAMP;
    MCopyProtectedMemory( (void*)offset, (void*)file_offset, 1*C_LEN_SAMP, PM_COPY_MOVE) ;	//波形参数
    /*/Write_Number(10,110,file_offset,10,0,0);
    //Write_Number(100,110,offset,4,0,0);

    //	MAnyKeyReturn();
    	u_char echobuffer[C_LEN_SAMP];
    	MCopyProtectedMemory( echobuffer, (void*)file_offset, C_LEN_SAMP, PM_COPY_READ);	//波形参数

    //	MChannelRenovate();
    //	DrawWave(echobuffer,0,C_LEN_SAMP);		//画波形
    //Write_Number(0,0,1,4,0,0);
    //	MAnyKeyReturn();

    Write_Number(10,120,file_offset,10,0,0);
    Write_Number(100,120,offset,4,0,0);
    Write_Number(150,120,C_LEN_SAMP,4,0,0);
    	MCopyProtectedMemory( echobuffer, (void*)offset, C_LEN_SAMP, PM_COPY_READ);	//波形参数
    Write_Number(0,0,2,4,0,0);
    	MChannelRenovate();
    Write_Number(0,0,3,4,0,0);
    	DrawWave(echobuffer,0,C_LEN_SAMP);		//画波形
    Write_Number(0,0,4,4,0,0);
    	MAnyKeyReturn();
    //*/
    file_offset += C_LEN_SAMP;
    offset = C_OFF_FREQ;
    MCopyProtectedMemory( (void*)offset, (void*)file_offset, 1*C_LEN_SAMP, PM_COPY_MOVE);	//频谱

    //Write_Number(10,120,file_offset,10,0,0);
    //Write_Number(100,120,offset,4,0,0);
    //Write_Number(150,120,C_LEN_SAMP,4,0,0);
    //file_offset += 1*C_LEN_SAMP;
    //offset = C_OFF_DAC;
    //MCopyProtectedMemory( (void*)offset, (void*)file_offset, C_LEN_DAC, PM_COPY_MOVE);	//DAC参数

//	MAnyKeyReturn();
    MSetAcquisition(0);

    return 1;
}


int DataDisplay(int file_no,int language)	//显示存储得数据
{
    int offset;//,file_offset,fat_offset;
    u_char echobuffer[C_LEN_SAMP];
    int file_total = DataDelete(-1);	//参数为负或正常删除返回剩余数据总数，参数过大返回－1

    DataRestore(file_no);
    MSetLanguage(language);
    stCurveAmend.Angle = MGetAngle(0);
    stCurveAmend.Diameter = MGetDiameter();
    stCurveAmend.Thick = MGetThick();
    MSetCurveAmend(&stCurveAmend);
    if( stCurveAmend.Thick < 20)MSetFunctionMode(0,C_CURVER_AMEND);
    /*
    	//offset = C_OFF_PEAK;
    	//MCopyProtectedMemory(&GatePeak, (void*)offset, C_LEN_PEAK, PM_COPY_READ);//门参数
    	file_offset = ( ( C_OFF_FILE + GetDataPosi(file_no) + 1)/2)*2;
    	file_offset += C_LEN_SYSPARA;
    	file_offset += C_LEN_CHSTATUS;
    	file_offset += C_LEN_CHPARA;
    	file_offset += C_LEN_GATEPARA;
    	file_offset += C_LEN_GATEPARA;
    	file_offset += C_LEN_DAC;
    	file_offset += C_LEN_PEAK;
    //*/
    offset = C_OFF_SAMP;
    MCopyProtectedMemory(echobuffer, (void*)offset, C_LEN_SAMP-1, PM_COPY_READ);	//波形参数
    offset = C_OFF_DAC;
    MCopyProtectedMemory( &Dac, (void*)offset, C_LEN_DAC, PM_COPY_READ);

    CalcuDac();
    MEraseWindow(0, 0, 40*C_ECHAR_HDOT, C_COORVPOSI - 1) ;//请屏幕左上角区域
    MChannelRenovate();
    DrawWave(echobuffer,0,C_LEN_SAMP-1);		//画波形
    DisplayPrompt(15);
    DispEchoPara2();
    //Write_Ascii( 400,30,file_no+0x30);
    Write_Number(12*C_ECHAR_HDOT,0,file_no+1,4,0,0);
    Write_Number(12*C_ECHAR_HDOT+(GetNumberLen(file_no+1) + 1)*C_ECHAR_HDOT,0,file_total,4,0,0);
    Write_Ascii(12*C_ECHAR_HDOT+GetNumberLen(file_no+1)*C_ECHAR_HDOT,0,'/');

//	fat_offset = C_OFF_FAT + C_LEN_FAT * file_no ;
//	MCopyProtectedMemory(&fat, (void*)fat_offset, C_LEN_FAT, PM_COPY_READ);
    offset = C_OFF_FILEINFO + C_SIZE_INT;
    MCopyProtectedMemory(&fat, (void*)offset, C_LEN_FAT, PM_COPY_READ);

    WriteFileDate(0, 0, fat.date);

    CEMenuOut2(21*C_ECHAR_HDOT,0,fat.name + 1,fat.name[0],1,C_ECHAR_VDOT);


    DrawDac(0);		//新画DAC
    return 1;
}

int GetDataPosi(int file_no)	//得到第 x 个数据存储位置
{
    int offset = C_OFF_FILEINFO ;
    int file_total;
    u_int retvalue;

    MCopyProtectedMemory( &file_total, (void*)offset, C_SIZE_INT, PM_COPY_READ);
    if(file_no < 0 || file_no >= file_total)	retvalue = -1;
    else
    {
        retvalue = ( (C_LEN_FILE * file_no+1)/2)*2;
    }
    return retvalue;
}

int DataDelete(int file_no)	//删除一个数据，参数为负或正常删除返回剩余数据总数，参数过大返回－1
{
    int fat_offset,file_offset ;
    int offset ;
    int file_total;
    int i;

    offset = C_OFF_FILEINFO ;
    MCopyProtectedMemory( &file_total, (void*)offset, C_SIZE_INT, PM_COPY_READ);
    if(file_total < 0)
    {
        file_total = 0;
        MCopyProtectedMemory( (void*)offset, &file_total, C_SIZE_INT, PM_COPY_WRITE);
//        CopyMemoryToSystem( (void*)offset,C_SIZE_INT,PM_COPY_WRITE);
    }

    if(file_no < 0 )return file_total;	//参数为负返回数据总数
    if(file_no >= file_total)return -1;

    file_total--;
    MCopyProtectedMemory( (void*)offset, &file_total, C_SIZE_INT, PM_COPY_WRITE);
//    CopyMemoryToSystem( (void*)offset,C_SIZE_INT,PM_COPY_WRITE);

/////////2013/5/2///删除数据时将cache数组处理
int temp_no=cache.cache[file_no];
for(i=file_no;i<file_total;i++)
{
    cache.cache[i]=cache.cache[i+1];
}
cache.cache[file_total]=temp_no;

    offset = C_OFF_CACHE ;
    MCopyProtectedMemory( (void*)offset, &cache, sizeof(CACHE), PM_COPY_WRITE);
 /*   for(i=0;i<260;i++)
    {
    Write_Number((i%10)*3*16,(i/10)*24,cache.cache[i],4,0,0);
    }
        MAnyKeyReturn();*/
/////////end
/*
    fat_offset = C_OFF_FAT + C_LEN_FAT * file_no;	//文件头位置
    file_offset = C_OFF_FILE + GetDataPosi(file_no);	//文件体位置

    for( file_no++ ; file_no <= file_total && file_no < C_FILE_NUMBER; file_no++ )
    {
        offset = fat_offset;
        fat_offset += C_LEN_FAT;
        MCopyProtectedMemory( (void*)offset, (void*)fat_offset, C_LEN_FAT, PM_COPY_MOVE);
//        CopyMemoryToSystem( (void*)offset,C_LEN_FAT,PM_COPY_WRITE);
        offset = file_offset;
        file_offset += C_LEN_FILE;
        MCopyProtectedMemory( (void*)offset, (void*)file_offset, C_LEN_FILE, PM_COPY_MOVE);
        CopyMemoryToSystem( (void*)offset,C_LEN_FILE,PM_COPY_WRITE);
    }
*/
//    offset = C_OFF_FILEINFO;
//    CopyMemoryToSystem((void*)offset, C_OFF_FAT-offset,PM_COPY_WRITE);

    return file_total;
}
int FileDelete(int file_no,unsigned int off_total, unsigned off_fat,int len_fat, unsigned int off_file, int len_file)	//删除一个数据，参数为负或正常删除返回剩余数据总数，参数过大返回－1
{
    int fat_offset,file_offset ;
    int offset ;
    int file_total;

    MCopyProtectedMemory( &file_total, (void*)off_total, C_SIZE_INT, PM_COPY_READ);
    if(file_total < 0)
    {
        file_total = 0;
        ///	MCopyProtectedMemory( (void*)offset, &file_total, C_SIZE_INT, PM_COPY_WRITE);
        MCopyProtectedMemory( (void*)off_total, &file_total, C_SIZE_INT, PM_COPY_WRITE);
//        CopyMemoryToSystem( (void*)off_total,C_SIZE_INT,PM_COPY_WRITE);
    }

    if(file_no < 0 )return file_total;	//参数为负返回数据总数
    if(file_no >= file_total)return -1;

    file_total--;
    MCopyProtectedMemory( (void*)off_total, &file_total, C_SIZE_INT, PM_COPY_WRITE);
//    CopyMemoryToSystem( (void*)off_total,C_SIZE_INT,PM_COPY_WRITE);

    fat_offset = off_fat + len_fat * file_no;	//文件头位置
    file_offset = off_file + len_file * file_no;	//文件体位置

    for( file_no++ ; file_no <= file_total; file_no++ )
    {
        offset = fat_offset;
        fat_offset += len_fat;
        MCopyProtectedMemory( (void*)offset, (void*)fat_offset, C_LEN_FAT, PM_COPY_MOVE);
//        CopyMemoryToSystem( (void*)offset,C_LEN_FAT,PM_COPY_WRITE);
        offset = file_offset;
        file_offset += len_file;
        MCopyProtectedMemory( (void*)offset, (void*)file_offset, C_LEN_FILE, PM_COPY_MOVE);
        CopyMemoryToSystem( (void*)offset,C_LEN_FILE,PM_COPY_WRITE);
    }
    return file_total;
}
int SavePretreatment(void)	//存储预处理
{
    u_char* sampbuff;
    u_char echobuffer[C_LEN_SAMP];
    int offset,i;

    sampbuff = GetSampleBuffer();

    MSetAcquisitionEnable(0,C_SETMODE_SETSAVE);
    for( i = 0; i< C_LEN_SAMP; i++)	echobuffer[i] = sampbuff[i];
    offset = C_OFF_PEAK;
    MCopyProtectedMemory( (void*)offset, &GatePeak, C_LEN_PEAK, PM_COPY_WRITE);//门参数
//        CopyMemoryToSystem( (void*)offset,C_LEN_PEAK,PM_COPY_WRITE);

    MSetAcquisitionEnable(1,C_SETMODE_SETSAVE);

    offset = C_OFF_SAMP;
    MCopyProtectedMemory( (void*)offset, echobuffer, C_LEN_SAMP, PM_COPY_WRITE); //以上为波形
//    CopyMemoryToSystem( (void*)offset,C_LEN_SAMP,PM_COPY_WRITE);

    ChannelParaStore();
    SystemParaStore();

    return 1;
}

int AutoSave(int mode)
{
    //if mode < 0 return 已自动存个数，else mode >0 自动存return 1存了一个，return 0 时间未到
    //return -1 表示是不在自动存

    int preAutosaveTime;
    int elapsedtime = GetElapsedTime()/1000;	//开机时长，单位s
    int autosavenum = 0;
    int offset;
    int file_total;
    int i;
    int retvalue = 0;

    //Write_Number(10,40,elapsedtime,8,0,-1);

    if( MGetSaveMode()==0 || MGetSaveStatus()==0 || MGetAcquisitionEnable() != 1)return -1;	//return -1 表示是不在自动存

    offset = C_OFF_STORE +	34;		//已自动存数据个数
    MCopyProtectedMemory( &autosavenum, (void*)offset, C_SIZE_INT, PM_COPY_READ);
    if(mode < 0)return autosavenum;	//if mode < 0 return 已自动存个数，

    offset = C_OFF_STORE +	30;		//前次自动存储时的开机时间sec
    MCopyProtectedMemory( &preAutosaveTime, (void*)offset, sizeof(preAutosaveTime), PM_COPY_READ);
    Write_Number(26 * 8 ,4 ,autosavenum,5,0,0);
    elapsedtime = GetElapsedTime();
    //Write_Number(10,50,elapsedtime,8,0,-1);
    //Write_Number(10,60,preAutosaveTime,8,0,-1);

    i = preAutosaveTime + 100*MGetSaveTime();
//Write_Number(10,60,preAutosaveTime,10,0,0);///
//Write_Number(10,70,i,10,0,0);///
//Write_Number(10,80,elapsedtime,10,0,0);///
//Write_Number(10,90,autosavenum,10,0,0);///

    //Write_Number(100,40,MGetSaveTime(),8,0,-1);
    //Write_Number(100,50,i,8,0,-1);
    //MGetKeyCode(40);

    if( elapsedtime >= i )//&& elapsedtime <= i+2000  )
    {
//		u_short pzText[9];
        u_short month,date;

        file_total = DataDelete(-1);
        if( file_total >= C_FILE_NUMBER )
        {
            MSetSaveMode(0,C_SETMODE_SETSAVE);//关闭连续记录
            MSetSaveStatus( 0,C_SETMODE_SETSAVE);
            return -1;
        }

        MCopyProtectedMemory( (void*)offset, &elapsedtime, sizeof(elapsedtime), PM_COPY_WRITE);
        autosavenum++;
        offset = C_OFF_STORE +	34;		//前次自动存储时的开机时间sec
        MCopyProtectedMemory( (void*)offset, &autosavenum, C_SIZE_INT, PM_COPY_WRITE);

        fat.posi = cache.cache[file_total];//C_LEN_FILE *
///Write_Number(100,50,file_total,10,0,0);///
///Write_Number(100,60,fat.posi,10,0,0);///
        month= fat.date = MGetSystemTime(-1);//年×65536＋月×256＋日，MGetCurrentDate();
        date = month%256;
        month = month/256;
        month = month%256;
//上面已经转为HEX
///		month = Bcd2Hex(month & 0x1f );
///	    date = Bcd2Hex(date & 0x3f );	/* 日期  BCD码转换成十六进制 */

        fat.name[0] = 8;
        fat.name[1] = month/10 + 48;
        fat.name[2] = month%10 + 48;
        fat.name[3] = date/10 + 48;
        fat.name[4] = date%10 + 48;
        fat.name[5] = autosavenum / 1000 + 48;
        fat.name[6] = ( autosavenum / 100 )% 10 + 48;
        fat.name[7] = ( autosavenum / 10 )% 10 + 48;
        fat.name[8] = ( autosavenum / 1 )% 10 + 48;

        offset = C_OFF_FILEINFO + C_SIZE_INT;
        MCopyProtectedMemory( (void*)offset, &fat, C_LEN_FAT, PM_COPY_WRITE); //以上为文件头

        SavePretreatment();	//存储预处理
        if(DataStore(file_total) != 1) return 0;

        offset = C_OFF_FILEINFO;
        file_total++;
        MCopyProtectedMemory( (void*)offset, &file_total, C_SIZE_INT, PM_COPY_WRITE);
///Write_Number(10,30,file_total,4,0,0);////
///CEMenuOut2(10,40,fat.name + 1,fat.name[0],1,16);///
        retvalue = 1;
    }

    return retvalue;
}

int AutoSavePlay(int file_no,int file_total)		//从当前数据开始播放连续存储的波形
{
    int tm1,tm0;
    int keycode;

    int offset;
    u_char echobuffer[C_LEN_SAMP];

    if( !MGetSaveMode() || !MGetSaveStatus() )return file_no;

    offset = C_OFF_SAMP;
    MCopyProtectedMemory(echobuffer, (void*)offset, C_LEN_SAMP, PM_COPY_READ);	//波形参数
    //取当前显示的波形
    MEraseWindow(0, 0, 40*C_ECHAR_HDOT, C_COORVPOSI - 1) ;
    MCoorDraw(C_COORHPOSI ,C_COORVPOSI , C_COORHEIGHT , C_COORWIDTH );	/*画指定位置和大小的波形坐标*/

    MChannelRenovate();
    DrawWave(echobuffer,0,C_LEN_SAMP);		//画波形

    tm1 = GetElapsedTime();
    DrawDac(0);		//新画DAC
//	MAnyKeyReturn();
//	MGateDraw();
    do
    {
        /*
        		DataDisplay(file_no);
        /*/
        DispEchoPara2();
        DrawWave(echobuffer,0,C_LEN_SAMP);		//画波形
        DataRestore(file_no);
        offset = C_OFF_SAMP;
        MCopyProtectedMemory(echobuffer, (void*)offset, C_LEN_SAMP, PM_COPY_READ);	//波形参数
        DrawWave(echobuffer,0,C_LEN_SAMP);		//画波形
        /*
        		WriteScale();	//标度
        		//SysParaDisp();	//显示通道号、探头类型、频带等

        		EMenuOut(C_RANGE_HPOSI + 6 * C_ECHAR_HDOT,C_RANGE_VPOSI,"mm( )",5,1,12);
        		switch (MGetScaleMode() )
        		{
        		case 1:
        			Write_Ascii(C_RANGE_HPOSI + 9 * C_ECHAR_HDOT,C_RANGE_VPOSI+ 0,'Y');
        			break;
        		case 2:
        			Write_Ascii(C_RANGE_HPOSI + 9 * C_ECHAR_HDOT,C_RANGE_VPOSI+ 0,'X');
        			break;
        		default:
        			Write_Ascii(C_RANGE_HPOSI + 9 * C_ECHAR_HDOT,C_RANGE_VPOSI+ 0,'S');
        			break;
        		}

        		Write_Number(22*C_ECHAR_HDOT, 0, MGetSpeed(),5,0,0);

        		Write_Number(22*C_ECHAR_HDOT, C_ECHAR_HDOT + 1, (MGetAngle(1) + 50)/100, 4,2,0);


        		WriteBaseGain(C_BASEGAIN_HPOSI,C_BASEGAIN_VPOSI);
        		WriteCompGain(C_BASEGAIN_HPOSI + 4 * C_ECHAR_HDOT,C_BASEGAIN_VPOSI);
        		//EMenuOut(C_BASEGAIN_HPOSI + 9 * C_ECHAR_HDOT,C_BASEGAIN_VPOSI ,"dB",2,1,12);

        		WriteRange(C_RANGE_HPOSI,C_RANGE_VPOSI);

        //		DispEchoPara2();
        //Write_Number(10,30,4,4,0,0);
        //*/
        int file_total = DataDelete(-1);	//参数为负或正常删除返回剩余数据总数，参数过大返回－1
        Write_Number(0,9,file_no+1,4,0,0);
        Write_Ascii(GetNumberLen(file_no+1)*C_ECHAR_HDOT,9,'/');
        Write_Number((GetNumberLen(file_no+1) + 1)*C_ECHAR_HDOT,9,file_total,4,0,0);

        WriteFileDate(0, 0, fat.date);
//Write_Number(10,30,11,4,0,0);

        offset = C_OFF_FILEINFO + C_SIZE_INT;
        MCopyProtectedMemory( &fat, (void*)offset, C_LEN_FAT, PM_COPY_READ);
        CEMenuOut2(11*C_ECHAR_HDOT,0,fat.name + 1,fat.name[0],1,16);

//*/
        if( MGetSaveTime() < 10)tm0 = tm1 + 100*MGetSaveTime();
        else tm0 = tm1 + 1000;	//1sec放一幅
        file_no++;
        do
        {
            keycode = MGetKeyCode(10);
            tm1 = GetElapsedTime();
        }
        while( keycode != C_KEYCOD_RETURN && tm1 < tm0 );	//按返回键退出，有2秒钟则放下一幅

    }
    while((MGetSaveMode() == 1 && MGetSaveStatus() != 0 && file_no < file_total) && keycode != C_KEYCOD_RETURN );	//表示当前数据是自动存

    file_no--;
    if(file_no >= file_total)file_no = file_total - 1;

    return file_no;
}


int SeekDate(int file_total)
{
    int fat_offset;
    int file_no = 0;
    TIME_CLOCK TimeClock;
    u_int date;
    int retvalue = file_total;

    Write_Space(0,C_ECHAR_VDOT,12,0);
    InputDate(0,C_ECHAR_VDOT,&TimeClock);
    date = date2int(&TimeClock);

    while(file_no < file_total)
    {
        fat_offset = C_OFF_FAT + C_LEN_FAT * cache.cache[file_no] ;
        MCopyProtectedMemory(&fat, (void*)fat_offset, C_LEN_FAT, PM_COPY_READ);

        if(fat.date	== date)
        {
            retvalue = file_no;
            break;
        }
        file_no++;
    }
    return retvalue;
}

int SeekName(int file_total)
{
    int fat_offset;
    int file_no = 0;
    int retvalue = file_total;
    u_short	name[9];
    int i;

    MEraseWindow(0, 0, 10*C_CCHAR_HDOT, 24) ;
    if(MInputChar(0, 0, 1, name,16,8) != C_TRUE) return 0;

    while(file_no < file_total)
    {
        fat_offset = C_OFF_FAT + C_LEN_FAT * cache.cache[file_no] ;
        MCopyProtectedMemory(&fat, (void*)fat_offset, C_LEN_FAT, PM_COPY_READ);

        for(i=0; i<8; i++)
        {
            if(fat.name[i] != name[i])break;
        }

        if(i == 8)
        {
            retvalue = file_no;
            break;
        }
        file_no++;
    }
    return retvalue;
}


