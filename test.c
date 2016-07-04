/*test.c,各种测试函数*/
#include "sdcard.h"
extern UDISK_INFO g_UDiskInfo;
extern const unsigned short c_SystemPowerStd[];
extern GATE_PEAK			GatePeak;

#if C_MAX_DOT_NUMBER > C_SCREEN_WIDTH
u_short dac_line[C_MAX_DOT_NUMBER];
#else
u_short dac_line[C_SCREEN_WIDTH];
#endif
u_short daccurve[C_SCREEN_WIDTH];

typedef struct _TagPOINT
{
    long x;
    long y;
} POINT,*LPPOINT;

int tspLine(POINT *PosIn, POINT *PosOut, int n, int ch, float tx1, float tx2, float ty1, float ty2);

void HotKeySet(void)
{
    u_int char_len = 6, crow_height = 48;
    int xpos,ypos;
    int para_xpos = C_CCHAR_HDOT * ( char_len + 0) ,para_ypos =  4;
    int retvalue = C_TRUE;
    int keycode;
    int row;
    u_short* cpText = 0;
    int i;
    int len;
    int item;
    u_int elapsedtime1 = GetElapsedTime();
    int offset;
    int crnum = 0;

    //for( i = 0 ; i < 4 ; i++)Function.funcMenu[i] = 0; //保存原设置

    //MCopyProtectedMemory( (void*)offset, &Function, C_LEN_FUNCTION, PM_COPY_WRITE);

    offset = C_OFF_KEYFUNC;
    MCopyProtectedMemory( & Function, (void*)offset, C_LEN_FUNCTION, PM_COPY_READ);
    crnum = 5;
    //SystemParaStore();
    MSetColor(C_CR_MENU);
    MSetAcquisition(0);
    while(true)
    {
        para_ypos =  4;
        MFclearScreen();
        MSetColor(C_CR_MENU);
        EraseDrawRectangle(26*C_ECHAR_HDOT, 380, 26*C_ECHAR_HDOT + (6+1*MGetLanguage())*C_CCHAR_HDOT+16, 380 + 2 * (C_CCHAR_VDOT+12));
        MSetColor(C_CR_MENU);
        for (i=0; i<crnum; i++)
        {
            TextOut(C_ECHAR_HDOT,para_ypos+crow_height*i,1,16,24,(u_char*)_FuncHotKey_A[MGetLanguage()][i],4);
        }
        TextOut(27*C_ECHAR_HDOT,380+4,1,10,24,(u_char*)_MenuPrompt_B1[MGetLanguage()][0],4);
        TextOut(27*C_ECHAR_HDOT,380+4+36,1,10,24,(u_char*)_MenuPrompt_B1[MGetLanguage()][0]+10,4);
//        EMenuOut(27*C_ECHAR_HDOT,380+4,_TestMenu_Enter,1,1,C_ECHAR_VDOT);
        MSetColor(C_CR_PARA);
        if(MGetLanguage())
        {
            para_ypos+=C_ECHAR_VDOT;
        }
        for( i = 0,row = 1; i < crnum-1 ; i++,row++)
        {
            xpos = para_xpos + C_ECHAR_HDOT+24*MGetLanguage();
            ypos = para_ypos + crow_height *row ;
            //MSetColor( i ) ;
            TextOut(xpos,ypos,1,17,crow_height,(u_char*)_FuncHotKey_B[MGetLanguage()][Function.funcMenu[i]]+2,4);
            // MSetColor(C_CR_UNDO);
        }
        MKeyRlx();		/*只有按键已释放才退出*/

        do
        {
            if( GetElapsedTime() > elapsedtime1 + 200 )
            {
                InvertWords(27*C_ECHAR_HDOT,380+4, 1); /*反白显示几个字的区域*/
                elapsedtime1 = GetElapsedTime() ;
            }
            keycode = MGetKeyCode(10);
            if( keycode >= 0 && keycode <= crnum )break;
        }
        while( keycode != C_KEYCOD_RETURN && keycode != C_KEYCOD_CONFIRM );

        if (keycode == 0)
        {
            Function.funcMenu[0]=1;
            Function.funcMenu[1]=2;
            Function.funcMenu[2]=7;
            Function.funcMenu[3]=0;
            continue;
        }
        else if (keycode == C_KEYCOD_RETURN)
        {
            //  SystemParaRestore();
            //  ChannelParaRestore();

            offset = C_OFF_KEYFUNC;
            MCopyProtectedMemory( &Function, (void*)offset, C_LEN_FUNCTION, PM_COPY_READ);
            retvalue = C_FALSE;
            break;
        }
        else if (keycode == C_KEYCOD_CONFIRM)
        {
            offset = C_OFF_KEYFUNC;
            MCopyProtectedMemory( (void*)offset, &Function, C_LEN_FUNCTION, PM_COPY_WRITE);
//            CopyMemoryToSystem( (void*)offset,C_LEN_FUNCTION,PM_COPY_WRITE);
            // SystemParaStore();
            //  ChannelParaStore();

            retvalue = C_TRUE;
            break;
        }
        if(MGetLanguage())
        {
            para_ypos-=C_ECHAR_VDOT;
        }
        row = item = keycode ;
        //  row--;
        item--;
        xpos = para_xpos;
        ypos = para_ypos + crow_height * row-1;

        InvertWords(C_ECHAR_HDOT,ypos, 1); /*反白显示几个字的区域*/
        ypos = para_ypos;

        len = 6;
        row = 9;
        MPushWindow(xpos, ypos, xpos+ (len+5+10*MGetLanguage())*C_ECHAR_HDOT, ypos + (row+0) * crow_height) ;
        EraseDrawRectangle(xpos, ypos, xpos+ (len+5+10*MGetLanguage())*C_ECHAR_HDOT, ypos + (row+0) * crow_height) ;
        for( i = 0; i < row ; i++)
        {
            TextOut(xpos + C_ECHAR_HDOT,ypos+4+i*crow_height,1,17,crow_height,(u_char*)_FuncHotKey_B[MGetLanguage()][i],4);
        }
        while(1)
        {
            keycode = MenuKeyReturn(row,2);	/*出现菜单时，按不大于keymax的键返回，mode=0其它键无效，=1确认键有效，=2返回键有效，=3确认返回键有效*/
            if (keycode == C_KEYCOD_RETURN||keycode == C_KEYCOD_CONFIRM)
                break;
            else if(keycode > 0 && keycode <= row)
            {
                Function.funcMenu[item]=keycode-1;
                break;
            }
        }
        MPopWindow();
    }
    MSetAcquisition(1);
    ClearEnvelope();	//清包络或峰值
    ScreenRenovate();	/*屏幕刷新*/
    DrawDac(0);		//新画DAC

    return ;
}
void AmplitudedBTable(void)
{
    int i;
    int xpos=64;	//起始坐标
    int ypos=48;
    int Gain[7],High[7];
    int key=C_KEYCODMAX;
    for (i=0; i<7; i++)
    {
        Gain[i]=0;
        High[i]=0;
    }
    MSetColor(C_CR_UNDO);
    ClearEnvelope();
    SystemParaStore();
    ChannelParaStore();
    //测试各点
    MSetAcquisition(0);	//关采样,只看曲线
    //设置声程100,只留母线,
    for(i = 0 ; i < 6; i++)
    {
        MSetLineGain(i,0);
    }
    MSetSurfGain(0,C_SETMODE_SETSAVE);
    MSetRange(1000,C_SETMODE_SETSAVE);
    MSetSystem();

    DrawDac(2);
    MSetBaseGain( MGetBaseGain()-200*log10( dac_line[48]/320.0),C_SETMODE_SETSAVE);
    DrawDac(2);
    MSetBaseGain( MGetBaseGain()-200*log10( dac_line[48]/320.0),C_SETMODE_SETSAVE);
    ScreenRenovate();	/*屏幕刷新*/
    DrawDac(0);

    u_int elapsedtime = GetElapsedTime() + 100;

    while( GetElapsedTime() < elapsedtime || key==C_KEYCODMAX)
    {
        key=MGetKeyCode(0);
        // MAnyKeyReturn();
    }
    for (i=0; i<7; i++)
    {
        High[i]=(dac_line[48*(i+1)]*10)/4+0.5;
    }
    for (i=0; i<7; i++)		//计算dB值
    {

        DrawDac(2);
        MSetBaseGain( MGetBaseGain()-200*log10( dac_line[48*(i+1)]/320.0),C_SETMODE_SETSAVE);
        Gain[i]=MGetBaseGain();
    }
    //画表
    MSetAcquisition(0);
    MFclearScreen();
    MSetColor(C_CR_MENU);
    TextOut(xpos,ypos+16,1,30,32,(u_char*)_AmplitudedB[MGetLanguage()][0],4*(1-MGetLanguage()));
    EMenuOut(xpos+32,ypos*2+16,"10203040506070",2,7,48);
    DrawRectangle(xpos,ypos,SCREEN_WIDTH-xpos,SCREEN_HEIGHT-ypos);
    for (i=0; i<7; i++)
    {
        CheckDrawLine(xpos,ypos*(2+i),SCREEN_WIDTH-xpos,ypos*(2+i));

        Write_Number(xpos*3+32,ypos*(2+i)+16,Gain[i],5,1,0);
        Write_Number(xpos*5+32*2+16,ypos*(2+i)+16,High[i],5,1,0);
        if (i<2)
        {
            CheckDrawLine(xpos*(3+i*3),ypos,xpos*(3+i*3),SCREEN_HEIGHT-ypos);
        }
    }

    MAnyKeyReturn();
    SystemParaRestore();
    ChannelParaRestore();
}

int MenuChoice(void)	//选择菜单
{
    u_int char_len = 5,row_number = 9,crow_height = 38;
    int xpos,ypos,inv_xpos = 0,inv_ypos = 0;
    int keycode;
    u_short *cpText;
    u_char *epText;
    int i;
    int prestatus=0;
    u_int elapsedtime1 = GetElapsedTime() ;
    int deci_len ,number ,row;
    int para_xpos ,para_ypos;
    int revalue=1;

    row_number = 10;
    crow_height =40;// C_ECHAR_VDOT*2;

    MSetColor(C_CR_MENU);
    MSetAcquisitionEnable(0,C_SETMODE_SETSAVE);
    MSetHardEchoShow(0,C_SETMODE_SETSAVE);
    ClearEnvelope();

    DisplayPrompt(16);

    xpos = C_COORWIDTH - ( char_len + 1+5*MGetLanguage() ) * C_CCHAR_HDOT;
    ypos = C_COORVPOSI + 1;
    EraseDrawRectangle(xpos, ypos, xpos + (char_len+1+6*MGetLanguage())*C_CCHAR_HDOT, ypos + crow_height * (row_number)+6*C_FILL ) ;
    inv_xpos = xpos + 4;
    inv_ypos = ypos + crow_height + 4;

    for (i=0; i<row_number; i++)
    {
        epText = (u_char*) _Menu_Choice[MGetLanguage()][i];
        TextOut(xpos+4,ypos+3+crow_height*i,1,11+5*MGetLanguage(),crow_height,epText,4*(1-MGetLanguage()));
        //  if (MGetLanguage())
        {
            //      TextOut(xpos+4,ypos+3+C_ECHAR_VDOT+crow_height*i,1,11,8,epText+11,0);
        }
    }


    MKeyRlx();
    while (revalue)
    {
        keycode=MGetKeyCode(0);
        revalue=0;
        switch (keycode)
        {
        case 1:	//功能菜单
            FuncMenu();
            return C_KEYCOD_RETURN;
        case 2:	//设置菜单
            ChannelMenu();
            return C_KEYCOD_RETURN;
            break;
        case 3:	//选项菜单
            ParaMenu(1);
            return C_KEYCOD_RETURN;
            break;
        case 4:	//仪器调校

            if(MGetSystemMode() == 1)break;
            if (MGetSaveStatus() == 1)break;
            ScreenRenovate();	/*屏幕刷新*/
            DrawDac(0);		//新画DAC
            TestMenu();
            return C_KEYCOD_RETURN;
            break;
        case 5:	//数据处理
            if(MGetSystemMode() == 1)break;
            if (MGetSaveStatus() == 1)break;
            DataProcedure();
            break;
        case 6:	//标准
            if( MGetTestStatus(C_TEST_AVG) == 1)break;//已做AVG则不显示标准菜单
            if( MGetParaLock(1,C_LOCK_DACAVG) == 1)break;	//参量保护
            STDMenu();
            return C_KEYCOD_RETURN;
        case 7:	//色彩
            ColorMenu();
            return C_KEYCOD_RETURN;
            break;
        case 8:	//波幅dB表
            if(MGetUnitType()==0 && (MGetTestStatus(C_TEST_DAC) == 1 || (MGetTestStatus(C_TEST_AVG) == 1 && MGetAvgMode()==0)))
            {
                AmplitudedBTable();
            }
            break;
        case 9:	//快捷设置
            HotKeySet();
            return C_KEYCOD_RETURN;
            break;
        case 0:	//初始化
            Usb2PC();
            //InitPara();
            break;
        case C_KEYCOD_RETURN:
        case C_KEYCOD_CONFIRM:
            break;
        default:
            revalue=1;
            break;
        }
    }

    MSetColor(C_CR_UNDO);
    ClearEnvelope();	//清包络或峰值
    MSetAcquisitionEnable(1,C_SETMODE_SETSAVE);
    MSetHardEchoShow(1,C_SETMODE_SETSAVE);
    MSetSystem();
    ScreenRenovate();
    DrawDac(0);
    return C_KEYCOD_RETURN;
}
int TestChoice(int Test)
{
    u_int char_len = 5,row_number = 9,crow_height = 38;
    int xpos,ypos,inv_xpos = 0,inv_ypos = 0;
    int keycode;
    u_short *cpText;
    u_char *epText;
    int i;
    int prestatus=0;
    u_int elapsedtime1 = GetElapsedTime() ;
    int deci_len ,number ,row;
    int para_xpos ,para_ypos;
    //int revalue=-1;

    row_number = 2;
    crow_height = 48;

    if(MGetSystemMode() == 1)return -1;

    if (MGetSaveStatus() == 1)return -1;

    MSetColor(C_CR_MENU);
    MSetAcquisitionEnable(0,C_SETMODE_SETSAVE);
    MSetHardEchoShow(0,C_SETMODE_SETSAVE);
    ClearEnvelope();

    DisplayPrompt(16);
    if (Test == C_TEST_OFFSET)
    {
        WriteParaName(C_OFFSET);
        //EMenuOut(C_CCHAR_HDOT * 8, 0,"us",2,1,12);
        TextOut(C_CCHAR_HDOT * (8+MGetLanguage()*4), 5,1,4,16,(u_char*)_us[0][0],0);
        EMenuOut(C_CCHAR_HDOT * (8+MGetLanguage()*4)+C_ECHAR_HDOT+4, 0,"s",1,1,12);
        WriteOffset((4+MGetLanguage()*4)*C_CCHAR_HDOT,0);
    }
    else if (Test == C_TEST_ANGLE)
    {

        if (MGetAngle(0)>=C_MAX_ANGLE)
        {
            TextOut(7*C_CCHAR_HDOT,0,1,4,10,(char *)_infinity[MGetLanguage()][0],0);
        }
        else
        {
            Write_Number(7*C_CCHAR_HDOT,0,( MGetAngle(1) + 50)/100,4,2,0);
        }
        WriteParaName(C_ANGLE);
        WriteAngle(4*C_CCHAR_HDOT,0);
        Write_Ascii(4*C_CCHAR_HDOT + 4 * C_ECHAR_HDOT,0 ,ANGLE);
        if (MGetAngle(0)>=C_MAX_ANGLE)
        {
            TextOut(7*C_CCHAR_HDOT,0,1,4,7,(char *)_infinity[MGetLanguage()][0],0);
        }
        else
        {
            Write_Number(7*C_CCHAR_HDOT,0,( MGetAngle(1) + 50)/100,4,2,0);
        }
        // MChannelRenovate();
        //	Write_Number(4*C_CCHAR_HDOT,8,( MGetAngle(1) + 50)/100,4,2,0);
        //	Write_Number(C_RANGE_HPOSI + 12 * C_ECHAR_HDOT, C_ECHAR_HDOT + 1, (MGetAngle(1) + 50)/100, 4,2,0);
    }
    else if (Test == C_TEST_DAC)
    {
        MSetTestStatus(C_MAKE_DACAVG,0);	//设置成制作DAC
        keycode = TestMenuDac();
        SystemParaRestore();
        ChannelParaRestore();
        MSetSystemMode(0,C_SETMODE_SETSAVE);	//设置成测试状态
        MSetSystem();
        ScreenRenovate();
        DrawDac(0);
        MSetAcquisition(1);
        return C_KEYCOD_RETURN;
    }
    xpos = C_COORWIDTH - ( char_len + 1 ) * C_CCHAR_HDOT;
    ypos = C_COORVPOSI + 1;
    EraseDrawRectangle(xpos, ypos, xpos + (char_len+1)*C_CCHAR_HDOT, ypos + crow_height * (row_number)+6*C_FILL ) ;
    inv_xpos = xpos + 4;
    inv_ypos = ypos + crow_height + 4;

    for (i=0; i<row_number; i++)
    {
        epText = (u_char*) _TestMenu_Menu[MGetLanguage()][i];
        TextOut(xpos+4,ypos+3+crow_height*i,1,10-2*MGetLanguage(),crow_height,epText,0);
        //    if (MGetLanguage())
        {
            //       TextOut(xpos+4,ypos+3+10+crow_height*i,1,8,8,epText+11,0);
        }
    }


    MKeyRlx();
    while (1)
    {
        keycode=MGetKeyCode(0);
        if (keycode==1 ||keycode==7  ||keycode==8 )
        {
            if (Test == C_TEST_OFFSET &&(keycode==7 || keycode==1) )
            {
                keycode = TestMenuOffset();//测零点
            }
            else if (Test == C_TEST_ANGLE && (keycode==8 || keycode==1) )
            {
                keycode = TestMenuAngle();//测角度
            }
            else if (Test == C_TEST_DAC)
            {
            }
            break;
        }
        else if (keycode==2)
        {
            MKeyRlx();
            keycode=-1;

            MSetColor(C_CR_UNDO);
            ClearEnvelope();	//清包络或峰值
            SystemParaRestore();
            ChannelParaRestore();

            MSetAcquisitionEnable(1,C_SETMODE_SETSAVE);
            MSetHardEchoShow(1,C_SETMODE_SETSAVE);
            MSetSystem();
            ScreenRenovate();	/*屏幕刷新*/
            DrawDac(0);
            if (Test == C_TEST_OFFSET)
            {
                keycode = OffsetChange();
            }
            else if (Test == C_TEST_ANGLE)
            {
                keycode = AngleChange();
            }
            else if (Test == C_TEST_DAC)
            {

            }
            break;
        }
        else if(keycode == C_KEYCOD_RETURN || keycode == C_KEYCOD_CONFIRM)
        {
            //revalue=C_KEYCOD_RETURN;
            break;
        }
    }

    MSetColor(C_CR_UNDO);
    ClearEnvelope();	//清包络或峰值
    MSetAcquisitionEnable(1,C_SETMODE_SETSAVE);
    MSetHardEchoShow(1,C_SETMODE_SETSAVE);
    MSetSystem();
    ScreenRenovate();	/*屏幕刷新*/
    DrawDac(0);
    return C_KEYCOD_RETURN;
}
int TestMenu(void)
{
    if(MGetSystemMode() == 1)return C_FALSE;

    if (MGetSaveStatus() == 1)return C_FALSE;

    int xpos = ( C_HORIDOT_SCREEN - C_CCHAR_HDOT * 12 ) / 8 * 8;
    int ypos = C_COORVPOSI;

    int keycode;
    int char_len = 6;
    int row_number = 6;
    u_int row_height = 48;
    int retvalue = 0;
#if C_DEVTYPE == 1
    row_number = 6;
#elif C_DEVTYPE == 4 || C_DEVTYPE == 20 || C_DEVTYPE == 5
    row_number = 5;
#elif C_DEVTYPE == 3 || C_DEVTYPE == 8 || C_DEVTYPE == 11 || C_DEVTYPE == 12
    row_number = 6;
#else
    row_number = 7;
#endif

    ClearEnvelope();	//清包络或峰值
    MSetAcquisitionEnable(0,C_SETMODE_SETSAVE);
    MSetHardEchoShow(0,C_SETMODE_SETSAVE);
    ClearEnvelope();
    SystemParaStore();
    ChannelParaStore();

    MSetColor(C_CR_MENU);
    EraseDrawRectangle(xpos, ypos, xpos+ (char_len+1+2*MGetLanguage())*C_CCHAR_HDOT+16, ypos + row_number * row_height) ;
//	CMenuOut(xpos + C_ECHAR_HDOT,ypos+4,_TestMenu_A1,char_len,row_number,row_height);	/*在指定位置根据每行字符数、行数、行高写菜单*/
    TextOut(xpos + C_ECHAR_HDOT-1,ypos+4,row_number,13,row_height,(u_char*)_TestMenu_A1[MGetLanguage()][0],4);
    MSetColor(C_CR_UNDO);

    MSetSystemMode(1,0);		//设置成自动调节状态
    keycode = MenuKeyReturn(row_number,3);	/*出现菜单时，按不大于keymax的键返回，mode=0其它键无效，=1确认键有效，=2返回键有效，=3确认返回键有效*/
    //;MCoorDraw(C_COORHPOSI ,C_COORVPOSI , C_COORHEIGHT , C_COORWIDTH );	/*画坐标,清除波形区内容*/
    MChannelRenovate();

    DisplayPrompt(15);
    switch (keycode)
    {
    case 1:
        retvalue = TestMenuOffset();
        break;
    case 2:

        retvalue = C_TRUE;
        if (MGetOffset() == 0)
        {
            if( DisplayQuery(1) == 1)
            {
                if( TestMenuOffset() == C_FALSE)
                {
                    retvalue = C_FALSE;
                }
            }
        }
        if(retvalue == C_TRUE)
            retvalue = TestMenuAngle();
        break;
    case 3:
        MSetTestStatus(C_MAKE_DACAVG,0);	//设置成制作DAC
        retvalue = TestMenuDac();
        break;
    case 4:
        MSetTestStatus(C_MAKE_DACAVG,1);	//设置成制作AVG
        retvalue = TestMenuDac();
        break;
    case 5:
        retvalue = TestMenuSystem();
        break;
    case 6:
#if C_MAKELIB == 1
        retvalue = TestMenuSensitivity();
#else
        TestFlawHigh();		//测高
        retvalue = C_FALSE;
#endif
        break;
    case 7:
#if C_DEVTYPE != 3 && C_DEVTYPE != 8 && C_DEVTYPE != 12 && C_DEVTYPE != 11
        //   break;
        TestMenuFft();  //测频谱
#else
        TestMenuSystem();
#endif
        retvalue = C_FALSE;
        break;
    case 8:
        break;
    case 9:
        retvalue = TestMenuThick();
        break;
    case C_KEYCOD_RETURN:
        MKeyRlx();
        break;
    }
    if( retvalue == C_FALSE)
    {
        SystemParaRestore();
        ChannelParaRestore();
    }
    else
    {
        SystemParaStore();
        ChannelParaStore();
    }
    MKeyRlx();
    MSetColor(C_CR_UNDO);
    ClearEnvelope();	//清包络或峰值
    SystemParaRestore();
    ChannelParaRestore();
    MSetSystemMode(0,C_SETMODE_SETSAVE);	//设置成测试状态
    MSetSystem();
    ScreenRenovate();
    DrawDac(0);
    MSetAcquisition(1);
    return C_TRUE;
}

int TestMenuOffset(void)
{
    int xpos = ( C_HORIDOT_SCREEN - C_CCHAR_HDOT * 8 - C_ECHAR_HDOT * (13+6*MGetLanguage())) / 8 * 8;
    int ypos = C_COORVPOSI;
    u_int para_xpos,para_ypos;
    u_int testrange[3];		//待测声程值
    u_int offset;
    int deci_len, number = 1;
    //u_short* cpText;
    //u_int crow_height = 24;

    int retvalue = -1;
    u_int row_height = 48;
    int keycode;

    DisplayPrompt(15);
    MSetColor(C_CR_MENU);
    //CMenuOut(0,0,_TestMenu_A1+1,5,1,row_height);
    TextOut(0,0,1,11,row_height,(u_char*)_TestMenu_A1[MGetLanguage()][0]+2,4);
    EraseDrawRectangle(xpos, ypos, xpos+ (14+3*MGetLanguage())*C_CCHAR_HDOT+4, ypos + 4 * row_height+24) ;
//	CEMenuOut(xpos + C_ECHAR_HDOT,ypos+4,_TestMenu_AB1,8,4,row_height);
    TextOut(xpos + C_ECHAR_HDOT,ypos+4+24,4,17,row_height,(u_char*)_TestMenu_AB1[MGetLanguage()][0],4);
//    EMenuOut(xpos + 2*C_ECHAR_HDOT+8-2*C_ECHAR_HDOT*MGetLanguage(),ypos+3 * row_height,_TestMenu_Enter,1,1,C_ECHAR_VDOT);

    MSetColor(C_CR_UNDO);

    para_xpos = xpos + (9+3*MGetLanguage()) * C_CCHAR_HDOT-8;	/*写参数，位置水平方向右移*/
    para_ypos = ypos + 4;

    offset = C_OFF_TEMP + 0;
    MCopyProtectedMemory( testrange, (void*)offset, 12, PM_COPY_READ);
    while(retvalue == -1)
    {
        /*声速*/
        xpos = para_xpos;
        ypos = para_ypos + row_height *0 ;
        WriteSpeed(xpos,ypos+C_ECHAR_VDOT+1 );


        /*一次声程*/
        ypos = para_ypos + row_height *1 ;
        WriteLongness(xpos,ypos+C_ECHAR_VDOT+1,testrange[1],5,1);
        /*
        if( MGetUnitType() > 0)
        {//单位inch
        	Write_Number(xpos,ypos+C_ECHAR_VDOT+1 ,testrange[1],6,3,0);
        	EMenuOut(xpos + 6 * C_ECHAR_VDOT,ypos+8,"inch",4,1,8);
        }
        else
        {
        	Write_Number(xpos,ypos+C_ECHAR_VDOT+1 ,testrange[1],6,1,0);
        	EMenuOut(xpos + 6 * C_ECHAR_VDOT,ypos+8,"mm  ",4,1,8);
        }
        //*/

        /*二次声程*/
        ypos = para_ypos + row_height *2 ;
        WriteLongness(xpos,ypos+C_ECHAR_VDOT+1,testrange[2],5,1);
        /*
        if( MGetUnitType() > 0)
        {//单位inch
        	Write_Number(xpos,ypos+C_ECHAR_VDOT+1 ,testrange[2],6,3,0);
        	EMenuOut(xpos + 6 * C_ECHAR_VDOT,ypos+8,"inch",4,1,8);
        }
        else
        {
        	Write_Number(xpos,ypos+C_ECHAR_VDOT+1 ,testrange[2],6,1,0);
        	EMenuOut(xpos + 6 * C_ECHAR_VDOT,ypos+8,"mm  ",4,1,8);
        }
        //*/

        keycode = MenuKeyReturn(3,3);	/*出现菜单时，按不大于keymax的键返回，mode=0其它键无效，=1确认键有效，=2返回键有效，=3确认返回键有效*/
        ypos = para_ypos + row_height * (keycode - 1);

        switch (keycode)
        {
        case 1:
            if( MGetUnitType() > 0)deci_len = 2;//单位inch
            else deci_len = 0;
            number = 0;
            while(1)
            {
                if( Input_Number(xpos,ypos,&number,4, &deci_len,0) != 1)break;
                else
                {
                    if( number <= C_MAX_SPEED)
                    {
                        MSetSpeed(number,C_SETMODE_SAVE);
                        break;
                    }
                    else	DisplayPrompt(4);
                }
            }
            break;
        case 2:
            if( MGetUnitType() > 0)deci_len = 3;//单位inch
            else deci_len = 1;
            number = 0;

            while(1)
            {
                if( Input_Number(xpos,ypos,&number,4, &deci_len,0) != 1)break;
                else
                {
                    if( number <= 30000)
                    {
                        testrange[1] = number;
                        if( MGetProbeMode() == C_P_TRANSMISSION)testrange[2] = number * 3;
                        else testrange[2] = number * 2;
                        break;
                    }
                    else	DisplayPrompt(4);
                }
            }
            break;
        case 3:
            if( MGetUnitType() > 0)deci_len = 3;//单位inch
            else deci_len = 1;
            number = 0;
            while(1)
            {
                if( Input_Number(xpos,ypos,&number,4, &deci_len,0) != 1)break;
                else
                {
                    if( number <= 30000*2 )
                    {
                        testrange[2] = number;
                        break;
                    }
                    else	DisplayPrompt(4);
                }
            }
            break;
        case C_KEYCOD_RETURN:
            MKeyRlx();
            retvalue = 0;
            break;
        case C_KEYCOD_CONFIRM:
            if( testrange[1] > 10)
            {
                retvalue = 1;
                offset = C_OFF_TEMP + 0;
                MCopyProtectedMemory( (void*)offset, testrange, 12, PM_COPY_WRITE);
            }
            break;
        }
    }
    if(retvalue == 1)
    {
        MSetTestStatus(C_TEST_DAC,0);	//设置测试状态,DAC作
        MSetTestStatus(C_TEST_AVG,0);	//设置测试状态,AVG作
        MSetFunctionMode(0,C_FUNCTION_ALL);
        MSetSystem();
        // MChannelRenovate();	//本通道屏幕参数刷表

        while(1)
        {
            int Zerooffset=MGetOffset();
            if (TestOffset(testrange[1],testrange[2],0) == C_FALSE)
            {
                MSetOffset(Zerooffset,C_SETMODE_SETSAVE) ;
                return C_FALSE;
            }
            return C_TRUE;
            //;if( DisplayQuery(12) == 1) return C_TRUE;
        }
    }
    return C_FALSE;
}

int TestOffset(u_int testrange1,u_int testrange2,u_int mode)
{
    /*
    t = ( R2*T1 - R1 * T2)/( R2 - R1) = T1 - (T2-T1)R1/(R2-R1)
    */
    GateMax.Amp = 0;	//最高波置初值为0

    int gain = 400,range = 1000,offset = 0, offset0 = 0, delay = 0,speed = MGetSpeed();
    u_int echotime = 0;
    short gateamp;
    int i;
    u_int char_len,row_number,crow_height;
    //int number,deci_len;
    int xpos,ypos,menu_xpos,menu_ypos;
    int confirm_num = 0;
    u_int preElapsedtime;
    int retvalue = C_TRUE;
    int keycode;
    int gatetype = 0;
    //int i;
//	int elapsedtime = GetElapsedTime() + 1000;
    u_int testrange0 = testrange1;


    MSetEchoMode(0,C_SETMODE_SAVE);
    if( MGetProbeMode() == C_DPROBE )
    {
        offset0 = 160*800/( testrange1 + testrange2 );
        if( offset0 > 160 * 8)offset0 = (offset0 - 160 * 8)/2 + 160 * 8;
        if( offset0 > 160 * 4)offset0 = (offset0 - 160 * 4)/2 + 160 * 4;
        if( offset0 > 160 * 2)offset0 = (offset0 - 160 * 2)/2 + 160 * 2;;
        MSetGatePara(C_COORHORIUNIT * 4-2,C_COORHORIUNIT * 4,120,0,C_SETMODE_SETSAVE);
    }
    else MSetGatePara(C_COORHORIUNIT * 4-2,C_COORHORIUNIT * 2,120,0,C_SETMODE_SETSAVE);

    MKeyRlx();

    MSetBGateMode(0,C_SETMODE_SETSAVE);
    MSetGatePara(C_COORHORIUNIT * 4,C_COORHORIUNIT * 2,0,1,C_SETMODE_SETSAVE);
    range = testrange0 * 10/4;		//放在第4格
//	if( MGetProbeMode() == C_P_TRANSMISSION)range /= 2;
    MSetPara(gain,0,0,2,range,offset0,delay,MGetAngle(0),speed);
//	MSetPulseMode(0,C_SETMODE_SAVE);
    MSetSystem();
    MChannelRenovate();	//本通道屏幕参数刷表

    MSetFunctionMode(0,C_FUNCTION_ALL);
    MSetAcquisition(1);

    char_len = 7;
    row_number = 3;
    crow_height = 48-14*MGetLanguage();

    menu_xpos = C_COORHORIUNIT * 10-(char_len+2+MGetLanguage())*C_CCHAR_HDOT-30*MGetLanguage();
    menu_ypos = C_COORVPOSI + 1;
    MSetColor(C_CR_MENU);
    EraseDrawRectangle(menu_xpos, menu_ypos, C_COORHORIUNIT * 10+17, menu_ypos + crow_height *(row_number+MGetLanguage())) ;
//	CEMenuOut(menu_xpos + C_ECHAR_HDOT,menu_ypos + 4, _TestMenu_AB3 ,char_len,row_number,crow_height);	/*在指定位置根据每行字符数、行数、行高写菜单*/
    TextOut(menu_xpos + C_ECHAR_HDOT,menu_ypos+4, row_number+2*MGetLanguage(), 16+MGetLanguage(),crow_height-12*MGetLanguage(), (u_char*)_TestMenu_AB3[MGetLanguage()][0], 4);

    WriteLongness(menu_xpos + 9 * C_ECHAR_HDOT+16*MGetLanguage(), menu_ypos  + 3+(crow_height+13)*1*MGetLanguage() ,testrange0,4/*总长*/,0/*是否写单位*/);
    if( MGetUnitType() > 0)EMenuOut(menu_xpos + 9 * C_ECHAR_HDOT+40*MGetLanguage(), menu_ypos  + 20+20*MGetLanguage(),"in",2,1,8);
    else EMenuOut(menu_xpos + 9 * C_ECHAR_HDOT+72*MGetLanguage(), menu_ypos  + 20+(crow_height-4)*1*MGetLanguage(),"mm",2,1,8);
    ///Write_Number(menu_xpos + 11 * C_ECHAR_HDOT,menu_ypos  + 4 ,testrange0,4,1,0);
    MSetColor(C_CR_UNDO);


    xpos = 0;
    ypos = 0;
    ClearCursor(2);
    MSetGateParaInit();	//门内最高波设初值

    preElapsedtime = GetElapsedTime();	//开始测试时间

    // MSetSystemMode(1,0);		//设置成自动调节状态
    DisplayPrompt(15);
    MSetColor(C_CR_MENU);
    //CMenuOut(0,0,_TestMenu_A1+1,5,1,24);
    TextOut(0,0,1,11,24,(u_char*)_TestMenu_A1[MGetLanguage()][0]+2,4);
    while (1)
    {
        if( !MGetFunctionMode(C_ECHOMAX_MEMORY) && !MGetFunctionMode(C_ECHO_ENVELOPE) )
        {
            if(MAdjustGain(0,1,0,MGetAmpStdMax()))
            {
                MSetGateParaInit();	//门内最高波设初值
            }

        }
        keycode = MGetKeyCode(0);
        MParaRenovate(1);
        gateamp = MGetAmpMax(gatetype);
        if(!MGetFunctionMode(C_ECHOMAX_MEMORY) && !MGetFunctionMode(C_ECHO_ENVELOPE))
        {
            if(MGetGateParaMax(gatetype) == C_TRUE)
            {
                ClearCursor(2);
                xpos = GateMax.Pos;
                ypos = C_COORVPOSI + C_COORHEIGHT - 2 - GateMax.Amp*2 ;
                if((GateMax.Amp*2) < C_COORHEIGHT)	DrawCursor(xpos,ypos,2);
            }
        }
        else
        {
            xpos = GateMax.Pos;
            ypos = C_COORVPOSI + C_COORHEIGHT - 2 - GateMax.Amp*2 ;
        }

#if C_ACQU == 1
        if (GetElapsedTime() >= elapsedtime)
        {
            elapsedtime = GetElapsedTime() + 1000;
            MSetAcquisitionEnable(MGetAcquisitionEnable(),C_SETMODE_SETSAVE);
        }
#endif
        if(keycode == C_KEYCOD_CONFIRM && confirm_num == 0)
        {
            //t = ( R2*T1 - R1 * T2)/( R2 - R1) = T1 - (T2-T1)R1/(R2-R1)

            confirm_num++;
            ClearCursor(2);
            //echotime = MGetAmpTime(gatetype);
            //testrange0 = MGetAmpDist(gatetype);
            echotime = GateMax.Time;
            testrange0 = GateMax.Dist;
            //
            offset = echotime - testrange1*C_SAMPLE_FREQ/speed;//先按预置声速和一次波时间算出零点

            MKeyRlx();

            if( testrange2 <= testrange1 )
            {
                //如果二次声程小于一次声程或为0，则只测一次
                confirm_num++;
                break;
            }
            else
            {
                //range = MGetRange(3) + (testrange2 + testrange0 - 2*testrange1) * 100/40;		//放在第4格
                range = (testrange2 + testrange0 - testrange1) * 100/40;		//放在第4格
                testrange0 = testrange2;
//				if( MGetProbeMode() == C_P_TRANSMISSION)range /= 2;
                gain = MGetBaseGain()+120;
                MSetRange(range,C_SETMODE_SETSAVE);
                MSetBaseGain(gain,C_SETMODE_SAVE);
                MSetSystem();

                if( MGetProbeMode() == C_DPROBE )MSetGatePara(C_COORHORIUNIT * 4-7,C_COORHORIUNIT * 3,120,0,C_SETMODE_SETSAVE);
                else MSetGatePara(C_COORHORIUNIT * 4-7,C_COORHORIUNIT * 1.5,120,0,C_SETMODE_SETSAVE);


                if( !MGetFunctionMode(C_ECHOMAX_MEMORY))ClearCursor(2);
                else ClearEnvelope();

                MChannelRenovate();	//本通道屏幕参数刷表

                //WriteRange(C_RANGE_HPOSI,C_RANGE_VPOSI);
                //WriteScale();	//标度

                GateMax.Amp = 0;

                //MGateDraw();
                //MGateDraw();

                char_len = 7;
                row_number = 3;
                crow_height = 48-14*MGetLanguage();
                xpos = C_COORHORIUNIT * 10-(char_len+1)*C_CCHAR_HDOT;
                xpos = C_COORHORIUNIT * 10-(char_len+2+MGetLanguage())*C_CCHAR_HDOT-16*MGetLanguage();
                ypos = C_COORVPOSI + 1;

                MSetColor(C_CR_MENU);
                EraseDrawRectangle(menu_xpos, menu_ypos, C_COORHORIUNIT * 10+17, menu_ypos + crow_height * (row_number+MGetLanguage())) ;
                //	CEMenuOut(menu_xpos + C_ECHAR_HDOT,menu_ypos + 4, _TestMenu_AB3 ,char_len,row_number,crow_height);	/*在指定位置根据每行字符数、行数、行高写菜单*/
                TextOut(menu_xpos + C_ECHAR_HDOT,menu_ypos+4, row_number+2*MGetLanguage(), 16+MGetLanguage(),crow_height-12*MGetLanguage(), (u_char*)_TestMenu_AB3[MGetLanguage()][0], 4);

                WriteLongness(menu_xpos + 9 * C_ECHAR_HDOT+16*MGetLanguage(), menu_ypos  + 3+(crow_height+13)*1*MGetLanguage() ,testrange0,4/*总长*/,0/*是否写单位*/);
                if( MGetUnitType() > 0)EMenuOut(menu_xpos + 9 * C_ECHAR_HDOT+40*MGetLanguage(), menu_ypos  + 20+20*MGetLanguage(),"in",2,1,8);
                else EMenuOut(menu_xpos + 9 * C_ECHAR_HDOT+72*MGetLanguage(), menu_ypos  + 20+(crow_height-4)*1*MGetLanguage(),"mm",2,1,8);
            }
        }
        else if(keycode == C_KEYCOD_CONFIRM && confirm_num == 1)
        {
            confirm_num++;
            //offset = MGetAmpTime(gatetype) - echotime ;	//T2 - T1
            offset = GateMax.Time - echotime ;	//T2 - T1
            offset = echotime - testrange1 * offset / (testrange2 - testrange1);

            speed = testrange1 * C_SAMPLE_FREQ/(echotime - offset);
            MSetSpeed(speed,C_SETMODE_SETSAVE);

            break;

        }
        else if(keycode == C_KEYCOD_RETURN)
        {
            if( DisplayQuery(4) == 1)
            {
                retvalue = C_FALSE;
                //SystemParaRestore();
                //ChannelParaRestore();
                break;
            }
        }
        else if( keycode == C_FUNC_1 ||keycode == C_FUNC_2||keycode == C_FUNC_3||keycode == C_FUNC_4)
        {
            if(keycode == C_FUNC_1)i=0;
            else if(keycode == C_FUNC_2)i=1;
            else if(keycode == C_FUNC_3)i=2;
            else if(keycode == C_FUNC_4)i=3;

            /*Write_Number(50,100,keycode,2,0,0);
            Write_Number(50,120,Function.funcMenu[0],2,0,0);
            Write_Number(50,140,Function.funcMenu[1],2,0,0);
            Write_Number(50,160,Function.funcMenu[2],2,0,0);
            Write_Number(50,180,Function.funcMenu[3],2,0,0);
            */
            if(Function.funcMenu[i]==C_ECHO_ENVELOPE+1 )
            {
                ClearEnvelope();
                if( MGetFunctionMode(C_ECHO_ENVELOPE) == 0)
                {
                    MSetFunctionMode(1,C_ECHO_ENVELOPE);
                    MSetFunctionMode(0,C_ECHOMAX_MEMORY);

                }
                else if( MGetFunctionMode(C_ECHO_ENVELOPE) == 1)
                {
                    MSetFunctionMode(0,C_ECHO_ENVELOPE);
                }
                MKeyRlx();
            }
            else if(Function.funcMenu[i]==C_ECHOMAX_MEMORY+1)
            {
                ClearEnvelope();
                if( MGetFunctionMode(C_ECHOMAX_MEMORY) == 0)
                {
                    MSetFunctionMode(1,C_ECHOMAX_MEMORY);
                    MSetFunctionMode(0,C_ECHO_ENVELOPE);
                    MSetGateParaInit();	//门内最高波设初值
                }
                else if( MGetFunctionMode(C_ECHOMAX_MEMORY) == 1)
                {
                    MSetFunctionMode(0,C_ECHOMAX_MEMORY);
                }
                MKeyRlx();
            }
            SysParaDisp();
        }
#if C_DEVTYPE == 1 || C_DEVTYPE == 15
        else if(keycode == C_KEYCOD_AUTOGAIN )//&& GetSameKeyTime() > 20)MAdjustGain(0,0,MGetAmpStdMax(),MGetAmpStdMax());
#else
        else if(keycode == C_KEYCOD_BASEGAIN )//&& GetSameKeyTime() > 20)MAdjustGain(0,0,MGetAmpStdMax(),MGetAmpStdMax());
#endif
        {
            MAdjustGain(0,0,MGetAmpStdMax(),MGetAmpStdMax());
            MSetGateParaInit();	//门内最高波设初值
            MKeyRlx();
        }
        else if (keycode == C_KEYCOD_GATE)
        {
            GateChange();
            MKeyRlx();
        }
        else if (keycode == C_KEYCOD_RANGE)
        {
            RangeChange();
            MKeyRlx();
        }
        //else if (keycode == C_KEYCOD_OFFSET)
        //{
        //	OffsetChange();
        //}
        else continue;
        DisplayPrompt(15);
        MSetColor(C_CR_MENU);
        //	CMenuOut(0,0,_TestMenu_A1+1,5,1,24);
        TextOut(0,0,1,11,24,(u_char*)_TestMenu_A1[MGetLanguage()][0]+2,4);
        //ExpendTime(10);
    }
    if( confirm_num > 1)
    {
//		if( MGetProbeMode() == C_P_TRANSMISSION)offset /= 2;
        MSetOffset(offset + MGetOffset(),C_SETMODE_SETSAVE);
        if( MGetProbeMode() == C_APROBE )
        {
            //斜探头
            char_len = 8;
            row_number = 2;
            crow_height = 48-14*MGetLanguage();

            menu_xpos = C_COORHORIUNIT * 10-(char_len+2+MGetLanguage())*C_CCHAR_HDOT-16*MGetLanguage();
            menu_ypos = C_COORVPOSI + 1;

            //;MCoorDraw(C_COORHPOSI ,C_COORVPOSI , C_COORHEIGHT , C_COORWIDTH );	/*画坐标,清除波形区内容*/

            MChannelRenovate();
            MSetColor(C_CR_MENU);
//			CEMenuOut(menu_xpos + C_ECHAR_HDOT,menu_ypos + 4, _TestMenu_AB7,char_len,row_number,crow_height);
            EraseDrawRectangle(menu_xpos, menu_ypos, C_COORHORIUNIT * 10+17, menu_ypos + (crow_height+MGetLanguage()) * row_number+8*MGetLanguage()) ;
            TextOut(menu_xpos + C_ECHAR_HDOT,menu_ypos+6, row_number+1*MGetLanguage(), 16+MGetLanguage(),crow_height-12*MGetLanguage(), (u_char*)_TestMenu_AB3[MGetLanguage()][1], 4);

            int number,deci_len;

            if( MGetUnitType() > 0)deci_len = 3;//单位inch
            else deci_len = 1;
            number = testrange1;
            menu_xpos = menu_xpos+8*C_ECHAR_HDOT+C_ECHAR_HDOT*3*MGetLanguage();
            menu_ypos = C_COORVPOSI + 50+MGetLanguage();
            EMenuOut(menu_xpos + 4 * C_ECHAR_HDOT, menu_ypos,"mm",2,1,24);
            while(1)
            {
                if( Input_Number(menu_xpos, menu_ypos,&number,2, &deci_len,0) != 1)break;
                else
                {
                    if( number <= testrange1)
                    {
                        ///Write_Number(menu_xpos, menu_ypos, number, 2, 1, 0);
                        WriteLongness(menu_xpos,menu_ypos,number,4,1);
                        MSetForward(testrange1-number,C_SETMODE_SAVE);
                        break;
                    }
                    else	DisplayPrompt(4);
                }
            }
        }
        MSetTestStatus(C_TEST_OFFSET,1);	//设置测试状态,零点已测

        MSetGatePara(C_COORHORIUNIT * 3, C_COORHORIUNIT * 2 ,120,0,C_SETMODE_SETSAVE);

        //	MChannelRenovate();	//本通道屏幕参数刷表

        //SystemParaStore();
        //ChannelParaStore();

        retvalue = C_TRUE;
    }

    MKeyRlx();
    range = testrange1 * 100/40;		//放在第6.5格
    MSetRange(range,C_SETMODE_SETSAVE);
    WriteScale();	//标度
    MSetFunctionMode(0,C_ECHOMAX_MEMORY);

    return retvalue;
}

int TestMenuAngle(void)
{
    int xpos = ( C_HORIDOT_SCREEN - C_CCHAR_HDOT * 11 - C_ECHAR_HDOT * (10+MGetLanguage()*11)) / 8 * 8;
    int ypos = C_COORVPOSI;
    u_int para_xpos,para_ypos;
    u_int testrange[2];		//[0]反射体深度，[1]反射体直径，
    u_int offset;
    int deci_len, number = 1;

    int retvalue = -1;
    u_int row_height = 48;
    int keycode;

    ScreenRenovate();
    DisplayPrompt(15);
    MSetColor(C_CR_MENU);
//	CMenuOut(0,0,_TestMenu_A1+7,5,1,row_height);
    TextOut(0,0,1,11,row_height,(u_char*)_TestMenu_A1[MGetLanguage()][1]+2,4);

    if( MGetAngle(0) < 10) MSetKvalue(200,C_SETMODE_SAVE);

//	EraseDrawRectangle(xpos, ypos, xpos+ 14*C_CCHAR_HDOT, ypos + 4 * row_height) ;
//	CEMenuOut(xpos + C_ECHAR_HDOT,ypos+4,_TestMenu_AC1,9,4,row_height);	/*在指定位置根据每行字符数、行数、行高写菜单*/
    EraseDrawRectangle(xpos, ypos, xpos+ (16+3*MGetLanguage())*C_CCHAR_HDOT+16, ypos + 4 * row_height+24) ;
    //	CEMenuOut(xpos + C_ECHAR_HDOT,ypos+4,_TestMenu_AB1,8,4,row_height);
    TextOut(xpos + C_ECHAR_HDOT,ypos+4+24,4,18,row_height,(u_char*)_TestMenu_AC1[MGetLanguage()][0],4);
//    EMenuOut(xpos + 2*C_ECHAR_HDOT+8-2*C_ECHAR_HDOT*MGetLanguage(),ypos+3 * row_height,_TestMenu_Enter,1,1,C_ECHAR_VDOT);

    MSetColor(C_CR_UNDO);

    offset = C_OFF_TEMP + 20;
    MCopyProtectedMemory( testrange, (void*)offset, 8, PM_COPY_READ);

    para_xpos = xpos + (9+3*MGetLanguage()) * C_CCHAR_HDOT+8;	/*写参数，位置水平方向右移*/
    para_ypos = ypos + 4;

    while(retvalue == -1)
    {
        /*直径 */
        xpos = para_xpos;
        ypos = para_ypos + row_height *0 ;
        WriteLongness(xpos,ypos+C_ECHAR_VDOT+1,testrange[0],5,1);
        /*
        if( MGetUnitType() > 0)
        {//单位inch
        	Write_Number(xpos,ypos+C_ECHAR_VDOT+1 ,testrange[0],4,3,0);
        	EMenuOut(xpos + 6 * C_ECHAR_VDOT,ypos+8,"in",2,1,8);
        }
        else
        {
        	Write_Number(xpos,ypos+C_ECHAR_VDOT+1 ,testrange[0],4,1,0);
        	EMenuOut(xpos + 6 * C_ECHAR_VDOT,ypos+8,"mm",2,1,8);
        }
        //*/
        /*深度*/
        ypos = para_ypos + row_height *1 ;
        WriteLongness(xpos,ypos+C_ECHAR_VDOT+1,testrange[1],5,1);
        /*
        if( MGetUnitType() > 0)
        {//单位inch
        	Write_Number(xpos,ypos+C_ECHAR_VDOT+1 ,testrange[1],4,3,0);
        	EMenuOut(xpos + 6 * C_ECHAR_VDOT,ypos+8,"in",2,1,8);
        }
        else
        {
        	Write_Number(xpos,ypos+C_ECHAR_VDOT+1 ,testrange[1],4,1,0);
        	EMenuOut(xpos + 6 * C_ECHAR_VDOT,ypos+8,"mm",2,1,8);
        }
        //*/
        /*标称K值\折射*/
        xpos = para_xpos  ;
        ypos = para_ypos + row_height *2;
        if (MGetAngle(0)>=C_MAX_ANGLE)
        {

            Write_Ascii(xpos + 0 * C_ECHAR_HDOT,ypos+C_ECHAR_VDOT-6+6  ,' ');
            Write_Ascii(xpos + 1 * C_ECHAR_HDOT,ypos+C_ECHAR_VDOT-6+6  ,' ');
            TextOut(xpos + 0 * C_ECHAR_HDOT,ypos+C_ECHAR_VDOT-6+4 ,1,4,10,(char *)_infinity[MGetLanguage()][0],0);
        }
        else
        {
            Write_Number(xpos + 0 * C_ECHAR_HDOT,ypos+C_ECHAR_VDOT+1+4 ,( MGetAngle(1) + 50)/100,4,2,0);
        }
        Write_Ascii(xpos + 5 * C_ECHAR_HDOT,ypos+C_ECHAR_VDOT+1 ,'/');
        Write_Number(xpos + 6 * C_ECHAR_HDOT,ypos+C_ECHAR_VDOT+1 ,MGetAngle(0),4,1,0);
        Write_Ascii(xpos + 10 * C_ECHAR_HDOT,ypos+C_ECHAR_VDOT+1 ,ANGLE);


        keycode = MenuKeyReturn(3,3);	/*出现菜单时，按不大于keymax的键返回，mode=0其它键无效，=1确认键有效，=2返回键有效，=3确认返回键有效*/
        ypos = para_ypos + row_height * (keycode - 1);

        switch (keycode)
        {
        case 1:
            if( MGetUnitType() > 0)deci_len = 3;//单位inch
            else deci_len = 1;
            number = 0;
            while(1)
            {
                if( Input_Number(xpos,ypos,&number,4, &deci_len,0) != 1)break;
                else
                {
                    if( number <= C_MAX_RANGE)
                    {
                        testrange[0] = number;
                        break;
                    }
                    else	DisplayPrompt(4);
                }
            }
            break;
        case 2:
            if( MGetUnitType() > 0)deci_len = 3;//单位inch
            else deci_len = 1;
            number = 0;

            while(1)
            {
                if( Input_Number(xpos,ypos,&number,4, &deci_len,0) != 1)break;
                else
                {
                    if( number <= C_MAX_RANGE-1)
                    {
                        testrange[1] = number;
                        break;
                    }
                    else	DisplayPrompt(4);
                }
            }
            break;
        case 3:
            deci_len = 2;
            number = 0;

            while(1)
            {
                if( Input_Number(xpos,ypos,&number,2, &deci_len,0) != 1)break;
                else
                {
                    if( number <= 9000)
                    {
                        if(number<1000)		/*斜探头输入K值*/
                            MSetKvalue(number,C_SETMODE_SAVE);
                        else MSetAngle(number/10,C_SETMODE_SAVE);	/*设置当前通道	*/

                        break;
                    }
                    else	DisplayPrompt(4);
                }
            }
            break;
        case C_KEYCOD_RETURN:
            MKeyRlx();
            retvalue = 0;
            break;
        case C_KEYCOD_CONFIRM:
            retvalue = 1;
            offset = C_OFF_TEMP + 20;
            MCopyProtectedMemory( (void*)offset, testrange, 8, PM_COPY_WRITE);
            break;
        }
    }

    if(retvalue == 1 && MGetAngle(0) > 0)
    {
        while(1)
        {
            if (TestAngle(testrange[0],testrange[1]) == C_FALSE)return C_FALSE;
            return C_TRUE;
            //;if( DisplayQuery(12) == 1) return C_TRUE;
        }
    }
    else if(retvalue == 1 && MGetAngle(0) == 0)return C_TRUE;
    return C_FALSE;
}

int TestAngle(u_int diameter,u_int depth)	//直径、深度
{
    GateMax.Amp = 0;	//最高波置初值为0

    int gain = 600,range = 1000,angle;
    //u_int echotime;
    u_int gateamp;

    u_int char_len,row_number,crow_height;
    //int number,deci_len;
    int xpos,ypos;
    u_int preElapsedtime;
    int retvalue = C_TRUE;
    int keycode,i;
    int gatetype = 0;
    double dAngle;
//	int elapsedtime = GetElapsedTime() + 1000;
    u_int testrange0 = depth * 10000 / MGetAngle(3) - diameter/2;
    //MGetAngle(3) = 34.2
    //depth = 0.03
    MSetAcquisition(1);

    MKeyRlx();
    MSetScaleMode(2,0);

    MSetFunctionMode(0,C_FUNCTION_ALL);
    MSetDelay(0,C_SETMODE_SAVE);
    MSetBaseGain(gain,C_SETMODE_SAVE);
    MSetCompGain(0,C_SETMODE_SAVE);
    MSetSurfGain(0,C_SETMODE_SAVE);
    MSetEchoMode(0,C_SETMODE_SAVE);
    MSetGatePara(C_COORHORIUNIT * 2,C_COORHORIUNIT * 2,120,0,C_SETMODE_SETSAVE);
    MSetBGateMode(0,C_SETMODE_SETSAVE);
    MSetGatePara(C_COORHORIUNIT * 2,C_COORHORIUNIT * 2,0,1,C_SETMODE_SETSAVE);

    range = testrange0 * 100/30;		//放在第7格
    MSetRange(range,C_SETMODE_SAVE);
    MSetSystem();

    MChannelRenovate();	//本通道屏幕参数刷表

    char_len = 7;
    row_number = 3;
    crow_height = 48-14*MGetLanguage();

    xpos = C_COORHORIUNIT *5-17-MGetLanguage()*C_CCHAR_HDOT;// 6-(char_len+1)*C_CCHAR_HDOT-17;
    ypos = C_COORVPOSI + 1;
    MSetColor(C_CR_MENU);
    EraseDrawRectangle(xpos, ypos, xpos+(char_len+3+MGetLanguage()*2)*C_CCHAR_HDOT+6*MGetLanguage(), ypos + crow_height * (row_number+MGetLanguage())) ;
    TextOut(xpos + C_ECHAR_HDOT,ypos+4, row_number+2*MGetLanguage(), 16+MGetLanguage(),crow_height-12*MGetLanguage(), (u_char*)_TestMenu_AB3[MGetLanguage()][2], 4);

    WriteLongness(xpos + 9 * C_ECHAR_HDOT+32*MGetLanguage(), ypos  + 4+(crow_height+13)*1*MGetLanguage() ,depth,4/*总长*/,0/*是否写单位*/);
    if( MGetUnitType() > 0)EMenuOut(xpos + 9 * C_ECHAR_HDOT+40*MGetLanguage(), ypos  + 20+20*MGetLanguage(),"in",2,1,8);
    else EMenuOut(xpos + 9 * C_ECHAR_HDOT+94*MGetLanguage(), ypos  + 22+(crow_height-4)*1*MGetLanguage(),"mm",2,1,8);

    GateMax.Amp = 0;
    xpos = 0;
    ypos = 0;
    ClearCursor(2);

    preElapsedtime = GetElapsedTime();	//开始测试时间

    ///MSetSystemMode(1,0);		//设置成自动调节状态

    xpos = 0;
    ypos = 0;
    ClearCursor(2);
    DisplayPrompt(15);
    MSetColor(C_CR_MENU);
    //CMenuOut(0,0,_TestMenu_A1+7,5,1,24);
    TextOut(0,0,1,11,24,(u_char*)_TestMenu_A1[MGetLanguage()][1]+2,4);
    while (1)
    {
//		AdjustTime.ParaDisplayTime++;
        if( !MGetFunctionMode(C_ECHOMAX_MEMORY) && !MGetFunctionMode(C_ECHO_ENVELOPE) )
        {
            if (MAdjustGain(0,1,0,MGetAmpStdMax()) )
            {
                GateMax.Amp = 0;	//增益自动调节过
            }
        }
        MParaRenovate(1);
        keycode = MGetKeyCode(0);
        gateamp = MGetAmpMax(gatetype);
        if(!MGetFunctionMode(C_ECHOMAX_MEMORY) && !MGetFunctionMode(C_ECHO_ENVELOPE))
        {
            if(MGetGateParaMax(gatetype) == C_TRUE)
            {
                ClearCursor(2);
                xpos = GateMax.Pos;
                ypos = C_COORVPOSI + C_COORHEIGHT - 2 - GateMax.Amp*2 ;
                if((GateMax.Amp*2) < C_COORHEIGHT)	DrawCursor(xpos,ypos,2);
            }
        }
        else
        {
            xpos = GateMax.Pos;
            ypos = C_COORVPOSI + C_COORHEIGHT - 2 - GateMax.Amp*2 ;
        }
        dAngle = acos( (float)depth / (float)( GateMax.Time * MGetSpeed() /(C_SAMPLE_FREQ) + diameter / 2)  ) ;
//		if( AdjustTime.ParaDisplayTime > C_RENOVATE_TIME )
        if(GetElapsedTime() > preElapsedtime + 30)
        {
            Write_Number(C_ECHAR_HDOT*10,0,(int)( 1800 * dAngle/C_PI),4,1,0);
            Write_Ascii(C_ECHAR_HDOT*14,0,ANGLE);
            Write_Ascii(C_ECHAR_HDOT*15,0,'/');
            Write_Number(C_ECHAR_HDOT*16,0,(int)( 100*tanf(dAngle) + 0.5),4,2,0);

//			AdjustTime.ParaDisplayTime=0;
        }
        if(keycode == C_KEYCOD_CONFIRM)
        {
            ClearCursor(2);
            MKeyRlx();

            angle = (int)( 1800.0 * dAngle/C_PI +0.5 );
            MSetAngle(angle,0);

            MSetTestStatus(C_TEST_ANGLE,1);	//设置测试状态,K值已测
            //SystemParaStore();
            //ChannelParaStore();

            retvalue = C_TRUE;
            break;
        }
        else if(keycode == C_KEYCOD_RETURN)
        {
            if( DisplayQuery(4) == 1)
            {
                retvalue = C_FALSE;
//				SystemParaRestore();
//				ChannelParaRestore();

                break;
            }
        }

        else if( keycode == C_FUNC_1 ||keycode == C_FUNC_2||keycode == C_FUNC_3||keycode == C_FUNC_4)
        {
            if(keycode == C_FUNC_1)i=0;
            else if(keycode == C_FUNC_2)i=1;
            else if(keycode == C_FUNC_3)i=2;
            else if(keycode == C_FUNC_4)i=3;


            if(Function.funcMenu[i]==C_ECHO_ENVELOPE+1 )
            {
                ClearEnvelope();
                if( MGetFunctionMode(C_ECHO_ENVELOPE) == 0)
                {
                    MSetFunctionMode(1,C_ECHO_ENVELOPE);
                    MSetFunctionMode(0,C_ECHOMAX_MEMORY);

                }
                else if( MGetFunctionMode(C_ECHO_ENVELOPE) == 1)
                {
                    MSetFunctionMode(0,C_ECHO_ENVELOPE);
                }
                MKeyRlx();
            }
            else if(Function.funcMenu[i]==C_ECHOMAX_MEMORY+1)
            {
                ClearEnvelope();
                if( MGetFunctionMode(C_ECHOMAX_MEMORY) == 0)
                {
                    MSetFunctionMode(1,C_ECHOMAX_MEMORY);
                    MSetFunctionMode(0,C_ECHO_ENVELOPE);
                    MSetGateParaInit();	//门内最高波设初值
                }
                else if( MGetFunctionMode(C_ECHOMAX_MEMORY) == 1)
                {
                    MSetFunctionMode(0,C_ECHOMAX_MEMORY);
                }
                MKeyRlx();
            }

            SysParaDisp();
        }

#if C_DEVTYPE == 1 || C_DEVTYPE == 15
        else if(keycode == C_KEYCOD_AUTOGAIN )
#else
        else if(keycode == C_KEYCOD_BASEGAIN )//&& GetSameKeyTime() > 20)MAdjustGain(0,0,MGetAmpStdMax(),MGetAmpStdMax());
#endif
        {
            MAdjustGain(0,0,MGetAmpStdMax(),MGetAmpStdMax());
            MSetGateParaInit();	//门内最高波设初值
            MKeyRlx();
        }
//;		else if(keycode == C_KEYCOD_BASEGAIN && GetSameKeyTime() > 20)MAdjustGain(0,0,MGetAmpStdMax(),MGetAmpStdMax());
        else if( keycode == C_KEYCOD_GATE || keycode == C_KEYCOD_BASEGAIN || keycode == C_KEYCOD_RANGE )
        {
            KeyManage(keycode,1);
            MKeyRlx();
            DisplayPrompt(15);
            MSetColor(C_CR_MENU);
            //CMenuOut(0,0,_TestMenu_A1+7,5,1,24);
            TextOut(0,0,1,11,24,(u_char*)_TestMenu_A1[MGetLanguage()][1]+2,4);
        }
#if C_ACQU == 1
        if (GetElapsedTime() >= elapsedtime)
        {
            elapsedtime = GetElapsedTime() + 1000;
            MSetAcquisitionEnable(MGetAcquisitionEnable(),C_SETMODE_SETSAVE);
        }
#endif
    }
    MSetFunctionMode(0,C_ECHOMAX_MEMORY);
    MKeyRlx();
    return retvalue;
}

int TestMenuDac(void)
{
    int xpos = ( C_HORIDOT_SCREEN - C_CCHAR_HDOT * 9 - C_ECHAR_HDOT * (9+8*MGetLanguage())) / 8 * 8-32;
    int ypos = C_COORVPOSI;
    u_int para_xpos,para_ypos;
    u_int testrange[3];		//最大深度、直径、长度
    u_int offset;
    u_short* cpText = 0;
    int deci_len, number = 1;

    int retvalue = -1;
    u_short row_height = 48, row_number = 3;//菜单行数

    int keycode;

    if( MGetTestStatus(C_MAKE_DACAVG) == 0 )
    {
        if (MGetTestStatus(C_TEST_OFFSET) == 0 || MGetTestStatus(C_TEST_ANGLE) == 0 || MGetOffset() == 0 || MGetAngle(0) == 0 )
        {
            if( DisplayQuery(2) == 1)
            {
                if (MGetTestStatus(C_TEST_OFFSET) == 0|| MGetOffset() == 0)
                {
                    if( TestMenuOffset() == C_FALSE)
                    {
                        retvalue = C_FALSE;
                        MChannelRenovate();
                        MSetSystemMode(0,0);		//设置成自动调节状态
                        return 0;
                    }
                    SystemParaStore();
                    ChannelParaStore();
                }
                if (MGetTestStatus(C_TEST_ANGLE) == 0|| MGetAngle(0) == 0)
                {
                    if( TestMenuAngle() == C_FALSE )
                    {
                        retvalue = C_FALSE;
                        MSetSystemMode(0,0);		//设置成自动调节状态
                        ScreenRenovate();
                        return 0;
                    }
                    SystemParaStore();
                    ChannelParaStore();
                }
            }
        }
    }
    else
    {
        if (MGetTestStatus(C_TEST_OFFSET) == 0 || MGetOffset() == 0 )
        {
            if( DisplayQuery(1) == 1)
            {
                if( TestMenuOffset() == C_FALSE)
                {
                    retvalue = C_FALSE;
                    MChannelRenovate();
                    MSetSystemMode(0,0);		//设置成自动调节状态
                    return 0;
                }
            }
        }
    }

    MSetGatePara(C_COORHORIUNIT * 6,C_COORHORIUNIT * 2,120,0,C_SETMODE_SETSAVE);
    MSetGatePara(C_COORHORIUNIT * 6,C_COORHORIUNIT * 2,0,1,C_SETMODE_SETSAVE);
    MSetSystem();
    ScreenRenovate();	//本通道屏幕参数刷表
    MSetColor(C_CR_MENU);
    DisplayPrompt(15);
    if( MGetTestStatus(C_MAKE_DACAVG) == 0 )
        //	CMenuOut(0,0,_TestMenu_A1+13,5,1,row_height);
        TextOut(0,0,1,11,row_height,(u_char*)_TestMenu_A1[MGetLanguage()][2]+2,4);
    else
        //	CMenuOut(0,0,_TestMenu_A1+19,5,1,row_height);
        TextOut(0,0,1,11,row_height,(u_char*)_TestMenu_A1[MGetLanguage()][3]+2,4);



    EraseDrawRectangle(xpos, ypos, xpos+ (13+3*MGetLanguage())*C_CCHAR_HDOT + C_CCHAR_HDOT, ypos + (row_number+1) * row_height+24) ;

    if( MGetTestStatus(C_MAKE_DACAVG) == 0 )
    {
        TextOut(xpos + C_ECHAR_HDOT,ypos+4+24,(row_number+1),17,row_height,(u_char*)_TestMenu_DAC[MGetLanguage()][0],4);
//        EMenuOut(xpos + 2*C_ECHAR_HDOT+8-2*C_ECHAR_HDOT*MGetLanguage(),ypos+3 * row_height,_TestMenu_Enter,1,1,C_ECHAR_VDOT);
        if( MGetTestStatus(C_TEST_DAC) != 0 )
        {
            MEraseWindow(xpos + C_ECHAR_HDOT,ypos+4+row_height*3+24, xpos+C_ECHAR_HDOT*18,ypos+4+row_height*3+24*2) ;
            TextOut(xpos + C_ECHAR_HDOT,ypos+4+row_height*3+24,(1),17,row_height,(u_char*)_TestMenu_DAC[MGetLanguage()][4],4);
//            EMenuOut(xpos + C_ECHAR_HDOT+8,ypos+3 * row_height,_TestMenu_Enter,1,1,C_ECHAR_VDOT);
        }
        //	CEMenuOut(xpos + C_ECHAR_HDOT,ypos+4,_TestMenu_AD1A,7,(row_number+1),row_height);	/*在指定位置根据每行字符数、行数、行高写菜单*/
        //	CEMenuOut(xpos + C_ECHAR_HDOT,ypos+4+row_height*3,_TestMenu_AD1+21,7,1,row_height);	/*在指定位置根据每行字符数、行数、行高写菜单*/
    }
    else
    {
        TextOut(xpos + C_ECHAR_HDOT,ypos+4+24,(row_number+1),17,row_height,(u_char*)_TestMenu_AVG[MGetLanguage()][0],4);
//        EMenuOut(xpos + 2*C_ECHAR_HDOT+8-2*C_ECHAR_HDOT*MGetLanguage(),ypos+3 * row_height,_TestMenu_Enter,1,1,C_ECHAR_VDOT);
        if( MGetTestStatus(C_TEST_AVG) != 0 )
        {
            MEraseWindow(xpos + C_ECHAR_HDOT,ypos+4+row_height*3+24, xpos+C_ECHAR_HDOT*18,ypos+4+row_height*3+24*2) ;
            TextOut(xpos + C_ECHAR_HDOT,ypos+4+row_height*3+24,(1),17,row_height,(u_char*)_TestMenu_AVG[MGetLanguage()][4],4);
//            EMenuOut(xpos + C_ECHAR_HDOT+8,ypos+3 * row_height,_TestMenu_Enter,1,1,C_ECHAR_VDOT);
        }
        //	CEMenuOut(xpos + C_ECHAR_HDOT,ypos+4,_TestMenu_AD1,7,(row_number+1),row_height);	/*在指定位置根据每行字符数、行数、行高写菜单*/
        //	CEMenuOut(xpos + C_ECHAR_HDOT,ypos+4+row_height*3,_TestMenu_AD1A+21,7,1,row_height);
        //	if( MGetTestStatus(C_TEST_AVG) == 0  )
        //		CEMenuOut(xpos + C_ECHAR_HDOT,ypos+4+row_height*3,_TestMenu_AD1+21,7,1,row_height);	/*在指定位置根据每行字符数、行数、行高写菜单*/
    }
    MSetColor(C_CR_UNDO);
    MSetAvgMode(0,C_SETMODE_SETSAVE);

    para_xpos = xpos + (9+3*MGetLanguage()) * C_CCHAR_HDOT;	/*写参数，位置水平方向右移*/
    para_ypos = ypos + 4;

    offset = C_OFF_TEMP + 30;
    MCopyProtectedMemory( testrange, (void*)offset, 12, PM_COPY_READ);
    while(retvalue == -1)
    {
        /*最大深度*/
        xpos = para_xpos;
        ypos = para_ypos + row_height *0 ;
        /*		WriteLongness(xpos,ypos+C_ECHAR_VDOT+1,testrange[0],5,1);
        */
        if( MGetUnitType() > 0)
        {
            //单位inch
            Write_Number(xpos,ypos+C_ECHAR_VDOT+1 ,testrange[0],6,3,0);
            EMenuOut(xpos + 5 * C_ECHAR_HDOT,ypos+C_ECHAR_VDOT,"in",2,1,8);
        }
        else
        {
            Write_Number(xpos,ypos+C_ECHAR_VDOT+1 ,testrange[0],6,1,0);
            EMenuOut(xpos + 5 * C_ECHAR_HDOT,ypos+C_ECHAR_VDOT,"mm",2,1,8);
        }
//
        /*反射体直径*/
        ypos = para_ypos + row_height *1 ;
///		WriteLongness(xpos,ypos+C_ECHAR_VDOT+1,testrange[1],5,1);
        /*		Write_Number(xpos,ypos+C_ECHAR_VDOT+1 ,testrange[1],6,1,0);
        		EMenuOut(xpos + 6 * C_ECHAR_VDOT,ypos+8,"mm",2,1,8);
        */

        if( MGetUnitType() > 0)
        {
            //单位inch
            Write_Number(xpos,ypos+C_ECHAR_VDOT+1 ,testrange[1],6,3,0);
            EMenuOut(xpos + 5 * C_ECHAR_HDOT,ypos+C_ECHAR_VDOT,"in",2,1,8);
        }
        else
        {
            Write_Number(xpos,ypos+C_ECHAR_VDOT+1 ,testrange[1],6,1,0);
            EMenuOut(xpos + 5 * C_ECHAR_HDOT,ypos+C_ECHAR_VDOT,"mm",2,1,8);
        }
//
        /*反射体长度*/
        ypos = para_ypos + row_height *2 ;
        if( MGetTestStatus(C_MAKE_DACAVG) == 0 )
        {
///			WriteLongness(xpos,ypos+C_ECHAR_VDOT+1,testrange[2],5,1);
            /*			Write_Number(xpos,ypos+C_ECHAR_VDOT+1 ,testrange[2],6,1,0);
            			EMenuOut(xpos + 6 * C_ECHAR_VDOT,ypos+8,"mm",2,1,8);
            */
            if( MGetUnitType() > 0)
            {
                //单位inch
                Write_Number(xpos,ypos+C_ECHAR_VDOT+1 ,testrange[2],6,3,0);
                EMenuOut(xpos + 5 * C_ECHAR_HDOT,ypos+C_ECHAR_VDOT,"in",2,1,8);
            }
            else
            {
                Write_Number(xpos,ypos+C_ECHAR_VDOT+1 ,testrange[2],6,1,0);
                EMenuOut(xpos + 5 * C_ECHAR_HDOT,ypos+C_ECHAR_VDOT,"mm",2,1,8);
            }
//
            /*表面补偿*/
            ypos = para_ypos + row_height *3 ;
            ///		WriteLongness(xpos,ypos+C_ECHAR_VDOT+1,testrange[1],5,1);
//;			Write_Number(xpos,ypos+C_ECHAR_VDOT+1 ,MGetSurfGain(),4,1,0);
//;			EMenuOut(xpos + 6 * C_ECHAR_VDOT,ypos+8,"dB",2,1,8);
            while(true)
            {
                keycode = MAnyKeyReturn();
#if C_DEVTYPE == 14
                if( ( keycode >= 1 && keycode <= row_number)||
                        keycode == C_KEYCOD_CONFIRM || keycode == C_KEYCOD_POINT || keycode == C_KEYCOD_RETURN )
#else
                if( ( keycode >= 1 && keycode <= row_number)||
                        keycode == C_KEYCOD_CONFIRM || keycode == C_KEYCOD_DEL || keycode == C_KEYCOD_RETURN )
#endif
                {
                    MKeyRlx();
                    break;
                }
            }
        }
        else
        {
#if C_DEVTYPE==20
            MSetAvgMode(0,C_SETMODE_SAVE);
#endif
            ypos = para_ypos + row_height *2+24 ;
            //	cpText = (u_short*)_ParaMenu_AC3A + (MGetAvgMode()-0) * 5 + 1;		//由于DB算为两个字
            //	CEMenuOut(xpos,ypos , cpText ,2,1,16);
            TextOut(xpos,ypos ,1,6,16,(u_char*)_ParaMenu_AC3A[MGetLanguage()][MGetAvgMode()]+2,4);
            /*表面补偿*/
            ypos = para_ypos + row_height *3 ;
            ///		WriteLongness(xpos,ypos+C_ECHAR_VDOT+1,testrange[1],5,1);
//;			Write_Number(xpos,ypos+C_ECHAR_VDOT+1 ,MGetSurfGain(),4,1,0);
//;			EMenuOut(xpos + 6 * C_ECHAR_VDOT,ypos+8,"dB",2,1,8);

            testrange[2] = 0;
//			keycode = MenuKeyReturn(2,3);	/*出现菜单时，按不大于keymax的键返回，mode=0其它键无效，=1确认键有效，=2返回键有效，=3确认返回键有效*/
            while(true)
            {
                keycode = MAnyKeyReturn();
                //	if( MGetTestStatus(C_MAKE_DACAVG) != 0 && keycode == C_KEYCOD_DEL)continue;
#if C_DEVTYPE == 14
                if( ( keycode >= 1 || keycode == 2 || keycode == 3)||
                        keycode == C_KEYCOD_CONFIRM || keycode == C_KEYCOD_POINT || keycode == C_KEYCOD_RETURN )
#else
                if( ( keycode == 1 || keycode == 2 || keycode == 3)||
                        keycode == C_KEYCOD_CONFIRM || keycode == C_KEYCOD_DEL || keycode == C_KEYCOD_RETURN )
#endif
                {
                    MKeyRlx();
                    break;
                }
            }
        }


        ypos = para_ypos + row_height * (keycode - 1);

        if( MGetUnitType() > 0)deci_len = 3;//单位inch
        else deci_len = 1;
        switch (keycode)
        {
        case 1:
            number = 0;

            while(1)
            {
                if( Input_Number(xpos,ypos,&number,5, &deci_len,0) != 1)break;
                else
                {
                    if( number <= C_MAX_RANGE)
                    {
                        testrange[0] = number;
                        break;
                    }
                    else	DisplayPrompt(4);
                }
            }
            break;
        case 2:
            //	deci_len = 1;
            number = 0;

            while(1)
            {
                if( Input_Number(xpos,ypos,&number,4, &deci_len,0) != 1)break;
                else
                {
                    if( number <= C_MAX_RANGE)
                    {
                        testrange[1] = number;
                        break;
                    }
                    else	DisplayPrompt(4);
                }
            }
            break;
        case 3:
            if( MGetTestStatus(C_MAKE_DACAVG) == 0 )
            {
                //		deci_len = 1;
                number = 0;

                while(1)
                {
                    if( Input_Number(xpos,ypos,&number,4, &deci_len,0) != 1)break;
                    else
                    {
                        if( number <= C_MAX_RANGE)
                        {
                            testrange[2] = number;
                            break;
                        }
                        else	DisplayPrompt(4);
                    }
                }
            }
            else
            {
#if C_DEVTYPE!=20
                //	cpText = (u_short*)_ParaMenu_AC3A ;
                MPushWindow(xpos, ypos, xpos + 5*C_CCHAR_HDOT+1, ypos + C_CCHAR_VDOT * 2+4+24) ;
                EraseDrawRectangle(xpos, ypos, xpos + (5)*C_CCHAR_HDOT, ypos + C_CCHAR_VDOT * 2+24) ;
                //CEMenuOut(xpos+4,ypos+4 , cpText ,3,2,22);	/*在指定位置根据每行字符数、行数、行高写菜单*/
                TextOut(xpos+4,ypos+4 ,2,8,C_CCHAR_VDOT+8,(u_char*)_ParaMenu_AC3A[MGetLanguage()][0],4);
                while(1)
                {
                    keycode = MAnyKeyReturn();
                    if (keycode == C_KEYCOD_RETURN||keycode == C_KEYCOD_CONFIRM)
                        break;
                    else if(keycode >= 1 && keycode <= 2)
                    {
                        MSetAvgMode(keycode-1,C_SETMODE_SAVE);
                        break;
                    }
                }
                MPopWindow();
#endif
            }
            break;
        case 4:
            //	deci_len = 1;
            number = 0;

            while(1)
            {
                if( Input_Number(xpos,ypos,&number,4, &deci_len,0) != 1)break;
                else
                {
                    if( number <= 80)
                    {
                        MSetSurfGain(number,C_SETMODE_SAVE);
                        break;
                    }
                    else	DisplayPrompt(4);
                }
            }
            break;
        case C_KEYCOD_RETURN:
            MKeyRlx();
            retvalue = 0;
            break;
        case C_KEYCOD_CONFIRM:
#if C_DEVTYPE == 14
        case C_KEYCOD_POINT:
#else
        case C_KEYCOD_DEL:
#endif
            ChannelParaStore();
            if( MGetUnitType() > 0)
            {
                if( testrange[0] < 10 || testrange[0] > 50000 || testrange[1] > (int)(1000/0.254) || testrange[2] > (int)(1000/0.254))
                {
                    DisplayPrompt(4);
                    DisplayPrompt(15);
                    if( MGetTestStatus(C_MAKE_DACAVG) == 0 )
                        //	CMenuOut(0,0,_TestMenu_A1+13,5,1,row_height);
                        TextOut(0,0,1,11,row_height,(u_char*)_TestMenu_A1[MGetLanguage()][2]+2,4);
                    else
                        //	CMenuOut(0,0,_TestMenu_A1+19,5,1,row_height);
                        TextOut(0,0,1,11,row_height,(u_char*)_TestMenu_A1[MGetLanguage()][3]+2,4);

                    break;
                }
            }
            else
            {
                if( testrange[0] < 10 || testrange[0] > 50000 || testrange[1] > 1000 || testrange[2] > 1000)
                {
                    DisplayPrompt(4);
                    DisplayPrompt(15);
                    if( MGetTestStatus(C_MAKE_DACAVG) == 0 )
                        //	CMenuOut(0,0,_TestMenu_A1+13,5,1,row_height);
                        TextOut(0,0,1,11,row_height,(u_char*)_TestMenu_A1[MGetLanguage()][2]+2,4);
                    else
                        //	CMenuOut(0,0,_TestMenu_A1+19,5,1,row_height);
                        TextOut(0,0,1,11,row_height,(u_char*)_TestMenu_A1[MGetLanguage()][3]+2,4);

                    break;
                }
            }
            retvalue = 1;
#if C_DEVTYPE == 14
            if( C_KEYCOD_POINT == keycode )retvalue = 2;
#else
            if( C_KEYCOD_DEL == keycode )retvalue = 2;
#endif
            offset = C_OFF_TEMP + 30;
            MCopyProtectedMemory( (void*)offset, testrange, 12, PM_COPY_WRITE);
            break;
        }
    }

    if(retvalue >= 1)
    {
        while(1)
        {
            if( retvalue > 1)retvalue = Dac.num;
            else retvalue = 0;

            if (TestDac(testrange[0],testrange[1],testrange[2],retvalue) == C_FALSE) return C_FALSE;
            //;if( DisplayQuery(12) == 1)
            {
                SystemParaStore();
                ChannelParaStore();
                return C_TRUE;
            }
        }
    }
    MSetSystem();
    return C_FALSE;
}

int TestDac(u_int depthmax,u_int diameter,u_int length,short test_num )//最大深度、反射体直径、长度
{
    int gain = 500,range = 1000;
    u_int gateamp;

    u_char* sampbuffer;

    u_short char_len,row_number,crow_height;
    //int number,deci_len;
    short xpos,ypos,menu_xpos,menu_ypos,para_xpos,para_ypos;
    int deci_len, number = 1;

    u_int preElapsedtime;
    int retvalue = C_FALSE;
    short keycode,rotary;
    u_short gatetype = 0;

    int space;
    u_short dac_db[10],dac_dist[10];
    short SameKeyTime;
    int temp,i,j;
//	int offset;
    short oldbasegain;
//	short newbasegain;
    short surfgain = MGetSurfGain();
//	int elapsedtime = GetElapsedTime() + 1000;

    if( depthmax > 1500 && MGetPulseMode() == 0) MSetPulseMode(1,C_SETMODE_SETSAVE);

    MSetAcquisition(1);

    MSetFunctionMode(0,C_FUNCTION_ALL);

    ClearCursor(2);
    MKeyRlx();
    Dac.num = test_num ;	//选中波次数
    for(i=0; i< 10; i++)
    {
//		Dac.db[i] = 0;
//		Dac.dist[i] = 0;
        dac_db[i] = Dac.db[i] ;
        dac_dist[i] = Dac.dist[i] ;
    }
    if( test_num == 0)
    {
        MSetThick(0);
        MSetScaleMode(0,0);
        MSetDelay(0,C_SETMODE_SAVE);
        MSetBaseGain(gain,C_SETMODE_SETSAVE);
        MSetCompGain(0,C_SETMODE_SAVE);
        MSetSurfGain(0,C_SETMODE_SAVE);

        MSetGatePara(C_COORHORIUNIT * 1,C_COORHORIUNIT * 8,0,1,C_SETMODE_SETSAVE);
    }
    else
    {
        diameter = Dac.diameter ;
        length = Dac.length;//最大深度、反射体直径、长度
    }
    MSetEchoMode(0,C_SETMODE_SAVE);
    MSetBGateMode(0,C_SETMODE_SETSAVE);
//	range = depthmax * 100/80;		//放在第8格
//	range = ((range+249)/250)*250;
    MSetCompGain(0,C_SETMODE_SETSAVE);
    MSetSurfGain(0,C_SETMODE_SAVE);
    range = (int)( ( depthmax * 100/80 )/50.0+0.9)*50;		//放在第8格
    MSetRange(range,C_SETMODE_SAVE);
    MSetSystem();
    if( MGetTestStatus(C_MAKE_DACAVG) == 0 )
    {
        MSetGatePara(C_COORHORIUNIT * 0.8,C_COORHORIUNIT*15.1/2,140,0,C_SETMODE_SETSAVE);
    }
    else
    {
        MSetGatePara(C_COORHORIUNIT * 7.1,C_COORHORIUNIT * 1.5,140,0,C_SETMODE_SETSAVE);
    }
    if( MGetStdMode() == C_STD_AWS_NUM - 1)//AWS
    {
        MSetStdMode(0,C_SETMODE_SAVE);
    }
    short linegain[6];
    for(i = 0 ; i < 6; i++)
    {
        linegain[i] = MGetLineGain(i);
    }
    for(i = 0 ; i < 6; i++)
    {
        MSetLineGain(i,0);
    }
    ScreenRenovate();	//本通道屏幕参数刷表
    DrawDac(0);
    DisplayPrompt(15);
    WritePeakPrompt( MGetBGateMode(),-1);

    char_len = 14+3*MGetLanguage();
    row_number = 1;
    crow_height = 48-14*MGetLanguage();
    MSetColor(C_CR_MENU);
    if( MGetTestStatus(C_MAKE_DACAVG) == 0 )
    {
        //	CMenuOut(0,0,_TestMenu_A1+13,5,1,24);
        TextOut(0,0,1,11,C_CCHAR_VDOT,(u_char*)_TestMenu_A1[MGetLanguage()][2]+2,4);
        menu_xpos = C_COORHORIUNIT * 11-(char_len+3+MGetLanguage())*C_CCHAR_HDOT-20*MGetLanguage();
        menu_ypos = C_COORVPOSI ;
    }
    else
    {
        //	CMenuOut(0,0,_TestMenu_A1+19,5,1,24);
        TextOut(0,0,1,11,C_CCHAR_VDOT,(u_char*)_TestMenu_A1[MGetLanguage()][3]+2,4);
        menu_xpos = 0;
        menu_ypos = C_COORVPOSI ;
    }


    EraseDrawRectangle(menu_xpos, menu_ypos, menu_xpos + (char_len+1+2*MGetLanguage())*C_CCHAR_HDOT+18, menu_ypos + crow_height * row_number) ;
    MSetColor(C_CR_MENU);
    if(test_num > 0)
    {
        TextOut(menu_xpos + C_ECHAR_HDOT-10*MGetLanguage(),menu_ypos + 4,1,28+5*MGetLanguage(),C_CCHAR_VDOT,(u_char*)_TestMenu_AD2[MGetLanguage()][2],4);
//        EMenuOut(menu_xpos+8 + (16+3*MGetLanguage())*C_ECHAR_HDOT,menu_ypos+1,_TestMenu_Enter,1,1,C_ECHAR_VDOT);
    }
    else TextOut(menu_xpos + C_ECHAR_HDOT-10*MGetLanguage(),menu_ypos + 4,1,28+5*MGetLanguage(),C_CCHAR_VDOT,(u_char*)_TestMenu_AD2[MGetLanguage()][0],4);
//	if(test_num > 0)CEMenuOut(menu_xpos + C_ECHAR_HDOT,menu_ypos + 4, _TestMenu_AD5 ,char_len,row_number,crow_height);
//	else CEMenuOut(menu_xpos + C_ECHAR_HDOT,menu_ypos + 4, _TestMenu_AD2 ,char_len,row_number,crow_height);

    preElapsedtime = GetElapsedTime();	//开始测试时间

    MSetSystemMode(1,0);			//设置成自动调节状态
    MSetTestStatus(C_TEST_DAC,0);	//设置测试状态,DAC作
    MSetTestStatus(C_TEST_AVG,0);	//设置测试状态,AVG作

    GateMax.Amp = 0;	//最高波置初值为0
    xpos = 0;
    ypos = 0;

    if( MGetUnitType() > 0)
    {
        diameter = diameter*254/1000;
        length = length*254/1000;//最大深度、反射体直径、长度
    }



    while(1)
    {

        keycode = MGetKeyCode(0);
        while (test_num == 0)
        {
            MParaRenovate(1);
            if( !MGetFunctionMode(C_ECHOMAX_MEMORY) && !MGetFunctionMode(C_ECHO_ENVELOPE) )
            {
                if (MAdjustGain(0,0,0,MGetAmpStdMax()))
                {
                    GateMax.Amp = 0;	//增益自动调节过
                }
            }
            if( GateMax.Amp == 0)
            {
                DrawDac(0);
                GateMax.Amp = 1;
            }

            gateamp = MGetAmpMax(gatetype);
            if( !MGetFunctionMode(C_ECHOMAX_MEMORY) && !MGetFunctionMode(C_ECHO_ENVELOPE) )
            {
                if ( MGetGateParaMax( gatetype) == C_TRUE  )
                {
                    ClearCursor(2);
                    xpos = GateMax.Pos;
                    ypos = C_COORVPOSI + C_COORHEIGHT - 2 - GateMax.Amp*2 ;
                    if((GateMax.Amp*2) < C_COORHEIGHT)	DrawCursor(xpos,ypos,2);
                }
            }

            oldbasegain = MGetBaseGain();
            keycode = MGetKeyCode(0);
            if(keycode != C_KEYCODMAX) break;
            else if( MGetRotary(-1) != 0)break;
        }

        MParaRenovate(1);
        if(GateMax.Amp == 0)
        {

//;			newbasegain = MGetBaseGain();
//;			MSetBaseGain(oldbasegain,C_SETMODE_SAVE);
//;			DrawDac(1);
//;			MSetBaseGain(newbasegain,C_SETMODE_SAVE);
//;			DrawDac(1);
            EraseDrawRectangle(menu_xpos, menu_ypos, menu_xpos + (char_len+1+2*MGetLanguage())*C_CCHAR_HDOT+18, menu_ypos + crow_height * row_number) ;
            MSetColor(C_CR_MENU);
            if(test_num > 0)
            {
                TextOut(menu_xpos + C_ECHAR_HDOT-10*MGetLanguage(),menu_ypos + 4,1,28+5*MGetLanguage(),C_CCHAR_VDOT,(u_char*)_TestMenu_AD2[MGetLanguage()][2],4);
//                EMenuOut(menu_xpos + (16+3*MGetLanguage())*C_ECHAR_HDOT,menu_ypos+1,_TestMenu_Enter,1,1,C_CCHAR_VDOT);
            }
            else TextOut(menu_xpos + C_ECHAR_HDOT-10*MGetLanguage(),menu_ypos + 4,1,28+5*MGetLanguage(),C_CCHAR_VDOT,(u_char*)_TestMenu_AD2[MGetLanguage()][0],4);

            /*			EraseDrawRectangle(menu_xpos, menu_ypos, menu_xpos + (char_len+1)*C_CCHAR_HDOT, menu_ypos + crow_height * row_number) ;
            			MSetColor(C_CR_MENU);
            			if(test_num > 0)CEMenuOut(menu_xpos + C_ECHAR_HDOT,menu_ypos + 4, _TestMenu_AD5 ,char_len,row_number,crow_height);
            			else CEMenuOut(menu_xpos + C_ECHAR_HDOT,menu_ypos + 4, _TestMenu_AD2 ,char_len,row_number,crow_height);
            */
            GateMax.Amp = 1;
        }

        gateamp = MGetAmpMax(gatetype);
        if ( !MGetFunctionMode(C_ECHOMAX_MEMORY) && !MGetFunctionMode(C_ECHO_ENVELOPE))
        {
            if( MGetGateParaMax( gatetype) == C_TRUE )
            {
                ClearCursor(2);
                xpos = GateMax.Pos;
                ypos = C_COORVPOSI + C_COORHEIGHT - 2 - GateMax.Amp*2 ;
                if((GateMax.Amp*2) < C_COORHEIGHT)DrawCursor(xpos,ypos,2);
            }
        }
        else
        {
            xpos = GateMax.Pos;
            ypos = C_COORVPOSI + C_COORHEIGHT - 2 - GateMax.Amp*2 ;
        }

        if(keycode == C_KEYCODMAX)
        {
            /*
            			rotary = MGetRotary(-2);
            			if(rotary > 0)rotary = 1;
            			else if( rotary < 0)rotary = -1;
            			if( rotary != 0)
            			{
            				BaseGainPlus(20*rotary);
            				WriteBaseGain(C_BASEGAIN_HPOSI,C_BASEGAIN_VPOSI);
            			}
            			rotary = MGetRotary(1);
            			if( rotary > 0 )keycode = C_KEYCOD_RIGHT;
            			else if( rotary < 0)keycode = C_KEYCOD_LEFT;
            			else continue;
            /*/
            rotary = MGetRotary(-1);
            if( rotary > 0 )keycode = C_KEYCOD_RIGHT;
            else if( rotary < 0 )keycode = C_KEYCOD_LEFT;
            else continue;
//*/
        }
        else rotary  = 0;
        SameKeyTime = TestSameKeyTime(keycode,GetElapsedTime(),C_SAMEKEYTIME);
        oldbasegain = MGetBaseGain();
#if C_ACQU == 1
        if (GetElapsedTime() >= elapsedtime)
        {
            elapsedtime = GetElapsedTime() + 1000;
            MSetAcquisitionEnable(MGetAcquisitionEnable(),C_SETMODE_SETSAVE);
        }
#endif
        if( keycode == C_KEYCOD_CONFIRM && test_num != 0)
        {
            ClearEnvelope();	//清包络或峰值
            ClearCursor(2);
            if(MGetTestStatus(C_MAKE_DACAVG) == 0)
                MSetTestStatus(C_TEST_DAC,1);	//设置测试状态,DAC已作
            else
            {
                MSetTestStatus(C_TEST_AVG,1);	//设置测试状态,AVG已作
                if( MGetCurveNumber() > 3)MSetCurveNumber(3);//AVG不超过3根线
            }
            MSetEquivalentDays(1,1);
            MSetCompGain(0,C_SETMODE_SETSAVE);
            range = (int)( ( depthmax * 100/80 )/50.0+0.9)*50;		//放在第8格
            MSetRange(range,C_SETMODE_SAVE);
            Dac.diameter = diameter;
            Dac.length = length;//最大深度、反射体直径、长度
///			MSetParaLock(1, C_LOCK_OFFSET);	//零点k值声速都被保护
///			MSetParaLock(1, C_LOCK_PROBE);	//通道参量都被保护
/////////////////////////////////////////////////////////////////////

            MKeyRlx();

            ExpendTime(50);
            MChannelRenovate();
            xpos = ( C_HORIDOT_SCREEN - C_CCHAR_HDOT * 7 - C_ECHAR_HDOT * (9+5*MGetLanguage())) / 8 * 8;
            ypos = C_COORVPOSI+C_CCHAR_VDOT;

            if (MGetTestStatus(C_MAKE_DACAVG) == 1)
            {
                if (MGetLanguage())
                {
                    EraseDrawRectangle(xpos-112, ypos-C_CCHAR_VDOT, xpos+ 10*C_CCHAR_HDOT + (1+3*MGetLanguage())*C_ECHAR_HDOT, ypos + 2 * crow_height) ;
                    //  TextOut(xpos + C_ECHAR_HDOT-10,ypos+10-24, 1, 14,C_ECHAR_VDOT, (u_char*)_TestMenu_set[MGetLanguage()][0]+14, 4);
                    TextOut(xpos + C_ECHAR_HDOT-112,ypos+1, 1, 15,C_ECHAR_VDOT, (u_char*)_TestMenu_set[MGetLanguage()][0], 4);
                    TextOut(xpos + C_ECHAR_HDOT-112,ypos+4+crow_height, 1, 16,C_ECHAR_VDOT, (u_char*)_TestMenu_set[MGetLanguage()][2], 4);
                }
                else
                {
                    EraseDrawRectangle(xpos, ypos-C_CCHAR_VDOT, xpos+ 10*C_CCHAR_HDOT + C_ECHAR_HDOT, ypos + 2 * crow_height) ;
                    TextOut(xpos + C_ECHAR_HDOT,ypos+4, 1, 14,C_CCHAR_VDOT, (u_char*)_TestMenu_set[MGetLanguage()][0], 4);
                    TextOut(xpos + C_ECHAR_HDOT,ypos+4+crow_height, 1, 14,C_CCHAR_VDOT, (u_char*)_TestMenu_set[MGetLanguage()][2], 4);
                }
//                EMenuOut(xpos + 2*C_ECHAR_HDOT+3*C_CCHAR_HDOT+8,ypos+4+ crow_height,_TestMenu_Enter,1,1,C_ECHAR_VDOT);
            }
            else
            {
                if (MGetLanguage())
                {
                    EraseDrawRectangle(xpos-112, ypos-C_CCHAR_VDOT, xpos+ 10*C_CCHAR_HDOT + C_ECHAR_HDOT, ypos + 3 * crow_height) ;
                    //  TextOut(xpos + C_ECHAR_HDOT-16,ypos+10-24, 1, 14,C_ECHAR_VDOT, (u_char*)_TestMenu_set[MGetLanguage()][0]+14, 4);
                    TextOut(xpos + C_ECHAR_HDOT-112,ypos+1, 1, 15,C_ECHAR_VDOT, (u_char*)_TestMenu_set[MGetLanguage()][0], 4);
                    TextOut(xpos + C_ECHAR_HDOT-112,ypos+4+crow_height, 1, 15,C_ECHAR_VDOT, (u_char*)_TestMenu_set[MGetLanguage()][1], 4);
                    TextOut(xpos + C_ECHAR_HDOT-112,ypos+4+crow_height*2, 1, 16,C_ECHAR_VDOT, (u_char*)_TestMenu_set[MGetLanguage()][2], 4);
                }
                else
                {
                    EraseDrawRectangle(xpos, ypos-C_CCHAR_VDOT, xpos+ 10*C_CCHAR_HDOT + C_ECHAR_HDOT, ypos + 3 * crow_height) ;
                    TextOut(xpos + C_ECHAR_HDOT,ypos+4, 1, 14,C_CCHAR_VDOT, (u_char*)_TestMenu_set[MGetLanguage()][0], 4);
                    TextOut(xpos + C_ECHAR_HDOT,ypos+4+crow_height, 1, 14,C_CCHAR_VDOT, (u_char*)_TestMenu_set[MGetLanguage()][1], 4);
                    TextOut(xpos + C_ECHAR_HDOT,ypos+4+crow_height*2, 1, 14,C_CCHAR_VDOT, (u_char*)_TestMenu_set[MGetLanguage()][2], 4);
                }
//                EMenuOut(xpos + 2*C_ECHAR_HDOT+2*C_CCHAR_HDOT+8,ypos+4+ 2* crow_height,_TestMenu_Enter,1,1,C_ECHAR_VDOT);
            }
            MKeyRlx();

            para_xpos = xpos + 6 * C_CCHAR_HDOT+16;	/*写参数，位置水平方向右移*/
            para_ypos = ypos + 4-C_CCHAR_VDOT;

            length = MGetThick();
            surfgain = MGetSurfGain();
            retvalue = -1;
            while(retvalue == -1)
            {
                /*表面补偿*/
                xpos = para_xpos;
                ypos = para_ypos + crow_height *0 ;
                Write_Number(xpos,ypos+C_ECHAR_VDOT+1 ,MGetSurfGain(),4,1,0);
                EMenuOut(xpos + 4 * C_ECHAR_HDOT,ypos+C_ECHAR_VDOT,"dB",2,1,8);

                if (MGetTestStatus(C_MAKE_DACAVG) == 0)
                {
                    /*工件厚度*/
                    ypos = para_ypos + crow_height *1 ;
                    WriteLongness(xpos,ypos+C_ECHAR_VDOT+1,MGetThick(),4,1);
                }
                while(true)
                {
                    keycode = MAnyKeyReturn();
                    if( ( keycode >= 1 && keycode <= 2)||
                            keycode == C_KEYCOD_CONFIRM || keycode == C_KEYCOD_RETURN )
                    {
                        MKeyRlx();
                        break;
                    }
                }

                ypos = para_ypos + crow_height * (keycode - 1);

                if( MGetUnitType() > 0)deci_len = 3;//单位inch
                else deci_len = 1;
                switch (keycode)
                {
                case 1:
                    deci_len = 1;
                    number = 0;
                    if ( Input_Number(xpos,ypos,&number,3, &deci_len,0) == 1)
                    {
                        if( number <= 500)
                            MSetSurfGain(number,C_SETMODE_SAVE);
                    }
                    break;
                case 2:
                    if (MGetTestStatus(C_MAKE_DACAVG) == 0)
                    {
                        number = 0;
                        if ( Input_Number(xpos,ypos,&number,4, &deci_len,0) == 1)
                        {
                            if( number <= 60000)
                                MSetThick(number);
                        }
                    }
                    break;
                case C_KEYCOD_CONFIRM:
                    length = MGetThick();
                    surfgain = MGetSurfGain();
                    MSetDiameter(0);
                    retvalue = 1;
                    break;

                case C_KEYCOD_RETURN:
                    retvalue = 2;
                    break;
                }
            }
////////////////////////////////////////////////////////////
            if (retvalue == 2)	//返回不退出
            {

                EraseDrawRectangle(xpos, ypos, xpos+ 10*C_CCHAR_HDOT + C_ECHAR_HDOT, ypos + 3 * crow_height) ;
                ScreenRenovate();	//本通道屏幕参数刷表
                MEraseWindow(0, 0,C_CCHAR_HDOT*8,C_CCHAR_VDOT ) ;
                MSetColor(C_CR_MENU);
                if( MGetTestStatus(C_MAKE_DACAVG) == 0 )
                {
                    //	CMenuOut(0,0,_TestMenu_A1+13,5,1,24);
                    TextOut(0,0,1,11,C_CCHAR_VDOT,(u_char*)_TestMenu_A1[MGetLanguage()][2]+2,4);
                    menu_xpos = C_COORHORIUNIT * 11-(char_len+3+MGetLanguage())*C_CCHAR_HDOT-20*MGetLanguage();
                    menu_ypos = C_COORVPOSI ;
                }
                else
                {
                    //	CMenuOut(0,0,_TestMenu_A1+19,5,1,24);
                    TextOut(0,0,1,11,C_CCHAR_VDOT,(u_char*)_TestMenu_A1[MGetLanguage()][3]+2,4);
                    menu_xpos = 0;
                    menu_ypos = C_COORVPOSI ;
                }

                EraseDrawRectangle(menu_xpos, menu_ypos, menu_xpos + (char_len+1+2*MGetLanguage())*C_CCHAR_HDOT+18, menu_ypos + crow_height * row_number) ;
                MSetColor(C_CR_MENU);
                if(test_num > 0)
                {
                    TextOut(menu_xpos + C_ECHAR_HDOT-10*MGetLanguage(),menu_ypos + 4,1,28+5*MGetLanguage(),C_CCHAR_VDOT,(u_char*)_TestMenu_AD2[MGetLanguage()][2],4);
//                    EMenuOut(menu_xpos+8 + (25+3*MGetLanguage())*C_ECHAR_HDOT,menu_ypos+1,_TestMenu_Enter,1,1,C_ECHAR_VDOT);
                }
                else TextOut(menu_xpos + C_ECHAR_HDOT-10*MGetLanguage(),menu_ypos + 4,1,28+5*MGetLanguage(),C_CCHAR_VDOT,(u_char*)_TestMenu_AD2[MGetLanguage()][0],4);

                /*				EraseDrawRectangle(menu_xpos, menu_ypos, menu_xpos + (char_len+1)*C_CCHAR_HDOT, menu_ypos + crow_height * row_number) ;
                				MSetColor(C_CR_MENU);
                				if(test_num > 0)CEMenuOut(menu_xpos + C_ECHAR_HDOT,menu_ypos + 4, _TestMenu_AD5 ,char_len,row_number,crow_height);
                				else CEMenuOut(menu_xpos + C_ECHAR_HDOT,menu_ypos + 4, _TestMenu_AD2 ,char_len,row_number,crow_height);
                */
                DrawDac(0);
                continue;
            }

            MSetThick(length);
            MSetSurfGain(surfgain,C_SETMODE_SAVE);

            retvalue = c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_THNUM];
            number = MGetThick()/10;

            if( MGetUnitType() > 0)number = (UINT)(number * 254/1000.0+0.5);//单位inch
            if (length == 0 || MGetTestStatus(C_MAKE_DACAVG) == 1)
            {
                //	MSetStdMode(0,C_SETMODE_SAVE);
                for( i = 0 ; i < retvalue; i++)
                {
                    for( j = 0; j < 6; j++)
                    {
                        linegain[j] = 0;
                    }
                    break;

                }
            }
            else
            {
                for( i = 0 ; i < retvalue; i++)
                {
                    if(MGetStdMode()==10||MGetStdMode()==11)
                    {
                        if( number>=c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_THICK+7*i] )    continue;
                    }
                    else
                    {
                        if( number>c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_THICK+7*i] )    continue;

                    }


                    for( j = 0; j < 6; j++)
                    {
                        linegain[j] = c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_THICK+7*i+j+1];

                    }
                    break;
                }
            }
            retvalue = C_TRUE;

            break;
        }
        else if(keycode == C_KEYCOD_RETURN)
        {
            if( DisplayQuery(4) == 1)
            {
                ClearCursor(2);
                retvalue = C_FALSE;
                SystemParaRestore();
                ChannelParaRestore();
                break;
            }
        }
        else if( (gateamp > 5 || MGetFunctionMode(C_ECHOMAX_MEMORY) ) && (keycode == C_KEYCOD_LEFT || keycode == C_KEYCOD_RIGHT || ( keycode == C_KEYCOD_CONFIRM && MGetTestStatus(C_MAKE_DACAVG)  && test_num == 0 ) ) )
        {
            int search = 1;
            u_char echobuff[C_LEN_SAMP+1] ;
            u_int  offset = C_OFF_SAMP ;

            if( MGetFunctionMode(C_ECHOMAX_MEMORY) ) search = 0;
            MSetAcquisitionEnable(0,C_SETMODE_SETSAVE);
            MCopyProtectedMemory( echobuff, (void*)offset, C_LEN_SAMP, PM_COPY_READ);
            sampbuffer = GetSampleBuffer();
//;			ClearCursor(2);
            if(keycode == C_KEYCOD_CONFIRM)keycode = C_KEYCOD_RIGHT;

            if( search )
            {
                if(keycode == C_KEYCOD_RIGHT)xpos-= 2;
                else if(keycode == C_KEYCOD_LEFT)xpos+= 2;	//使得当前最高波能第一次被选中
            }
            EraseDrawRectangle(menu_xpos, menu_ypos, menu_xpos + (char_len+1+2*MGetLanguage())*C_CCHAR_HDOT+18, menu_ypos + crow_height * row_number);
            MSetColor(C_CR_MENU);
            //	CEMenuOut(menu_xpos + C_ECHAR_HDOT,menu_ypos + 4, _TestMenu_AD3 ,char_len,row_number,crow_height);
            TextOut(menu_xpos + C_ECHAR_HDOT-10*MGetLanguage(),menu_ypos + 4,1,28+4*MGetLanguage(),C_CCHAR_VDOT,(u_char*)_TestMenu_AD2[MGetLanguage()][1],4);
//            EMenuOut(menu_xpos +8+ (11+2*MGetLanguage())*C_ECHAR_HDOT,menu_ypos+1,_TestMenu_Enter,1,1,C_ECHAR_VDOT);

            ExpendTime(10);
            MKeyRlx();
            while( xpos > 0 )	//当SearchPeak返回-1时表示未能找到最高波
            {
                if(keycode == C_KEYCOD_LEFT || keycode == C_KEYCOD_RIGHT)
                {

                    if( !search )
                    {
                        search = 1;
                        sampbuffer = echobuff;
                    }
                    else
                    {
                        ClearCursor(2);
                        sampbuffer = GetSampleBuffer();
                        xpos = SearchPeak(sampbuffer,xpos,keycode);
                    }
                    if( xpos <= 0 || xpos >= C_COORWIDTH)break;
                    ypos = C_COORVPOSI + C_COORHEIGHT - 2 - (*(sampbuffer + xpos))*2;


                    DrawCursor(xpos,ypos,2);
                    ExpendTime(20);
                }
                if(keycode == C_KEYCOD_CONFIRM)
                {
                    ClearCursor(2);
                    if( test_num > 0)DrawDac(1);		//参数为1表示清DAC
                    if(MGetTestStatus(C_MAKE_DACAVG) == 1 )test_num = 0;
                    dac_dist[test_num] = MGetRange(3) * xpos / C_COORWIDTH;	//得到采样点距离
                    if( MGetUnitType() > 0)dac_dist[test_num] = dac_dist[test_num] *254/1000;//花成MM
                    ///gateamp = *(sampbuffer + xpos) * 100 / 98;
                    gateamp = *(sampbuffer + xpos) ;

                    if ( MGetFunctionMode(C_ECHOMAX_MEMORY))
                    {
                        gateamp -=2;
                    }
                    dac_db[test_num] = MGetBaseGain() + MGetCompGain() + (int)( 200 * ( log10(255) - log10(gateamp) ) );
                    //需化为128%=255时的增益值
                    if(MGetTestStatus(C_MAKE_DACAVG) == 1 )
                    {
                        //test_num = 0;
                        if( test_num < 1 )
                        {
                            if( diameter < 5)
                            {
                                //大平底
                                if( MGetUnitType() > 0)
                                {
                                    i =  (int)( 200.0*log10(2.0*(MGetSpeed()*254/1000)* dac_dist[0]/( MGetFrequence() * 100.0*C_PI*4) ) + 0.5 );
                                }
                                else
                                {
                                    i =  (int)( 200.0*log10(2.0*MGetSpeed()* dac_dist[0]/( MGetFrequence() * 100.0*C_PI*4) ) + 0.5 );
                                }
//Write_Number(10,30,MGetSpeed(),8,0,0);
//Write_Number(10,40,dac_dist[0],8,0,0);
//Write_Number(10,50,(int)( MGetFrequence() * 100.0*C_PI*4),8,0,0);
//Write_Number(10,60,200*MGetSpeed()* dac_dist[0]/( MGetFrequence() * 100.0*C_PI*4),8,0,0);
//Write_Number(10,70,200*log10(2.0*MGetSpeed()* dac_dist[0]/( MGetFrequence() * 100.0*C_PI*4) ),8,0,0);
                                //		i -= 20;
                                dac_db[0] += i;
                            }
                            else
                            {
                                //平底孔
                                i =  (int)( 400.0 * log10(diameter/20.0) + 0.5);
                                dac_db[0] += i;
                            }
                            test_num++;
                        }

                        /*		ETextOut(10,50,"i:              ",9);
                        		Write_Number(10,60,i,4,0,0);
                        		ETextOut(10,70,"dac_db:         ",9);
                        		Write_Number(10,80,dac_db[0],4,0,0);
                        		ETextOut(10,90,"dac_dist:       ",9);
                        		Write_Number(10,100,dac_dist[0],4,0,0);
                        */
                    }
                    else test_num++;

                    for(i = 0; i < test_num-1; i++)
                    {
                        if(depthmax>= 400)
                        {
                            space=25;
                        }
                        else if (depthmax>= 200)
                        {
                            space=15;
                        }
                        else
                        {
                            space=10;
                        }

                        if( Mabsi( dac_dist[i] - dac_dist[test_num-1] ) < space )	//两点间距小于2.5mm
                        {
                            test_num--;
                            dac_db[i] = dac_db[test_num];
                            break;
                        }
                    }

                    for(i = 0; i < test_num; i++)
                    {
                        //对所有点按浅深顺序排列
                        for(j = i+1; j < test_num; j++)
                        {
                            if(dac_dist[i] > dac_dist[j] )
                            {
                                temp = dac_dist[i];
                                dac_dist[i] = dac_dist[j];
                                dac_dist[j] = temp;
                                temp = dac_db[i];
                                dac_db[i] = dac_db[j];
                                dac_db[j] = temp;
                            }
                        }
                    }
                    for(i = 1; i < test_num; i++)	//对两点间过于靠近的点删除一个
                    {
                        if(depthmax>= 400)
                        {
                            space=25;
                        }
                        else if (depthmax>= 200)
                        {
                            space=15;
                        }
                        else
                        {
                            space=10;
                        }

                        if( Mabsi( dac_dist[i-1] - dac_dist[i] ) < space )	//两点间距小于2.5mm
                        {
                            test_num--;
                            for(j = i; j< test_num ; j++)
                            {
                                dac_dist[j] = dac_dist[j+1];
                                dac_db[j] = dac_db[j+1];
                            }
                        }
                    }
                    if(test_num == 10)	//点数过多，对部分靠近的点进行合并
                    {
                        temp = (dac_dist[9] - dac_dist[0])/10;

                        for(i = 1; i < 9; i++)	//第1点和第10点不进行处理
                        {
                            if(dac_dist[i] - dac_dist[i-1] <= temp )
                            {
                                //如两点间距小于10点平均距离，则删除后一点
                                test_num--;
                                for(j = i; j< test_num ; j++)
                                {
                                    dac_dist[j] = dac_dist[j+1];
                                    dac_db[j] = dac_db[j+1];
                                }
                                break;
                            }
                        }
                    }
                    Dac.num = test_num;	//选中波次数
                    for(i=0; i< 10; i++)
                    {
                        Dac.db[i] = dac_db[i];
                        Dac.dist[i] = dac_dist[i];
                    }

                    //offset = C_OFF_DAC + 0*C_SIZE_SHORT;
                    //MCopyProtectedMemory((void*)offset,dac_db, 10*C_SIZE_SHORT, PM_COPY_WRITE);
                    //offset = C_OFF_DAC + 10*C_SIZE_SHORT;
                    //MCopyProtectedMemory((void*)offset,dac_dist, 10*C_SIZE_SHORT, PM_COPY_WRITE);
                    //offset = C_OFF_DAC + 20*C_SIZE_SHORT;
                    //MCopyProtectedMemory((void*)offset,&test_num, 1*C_SIZE_SHORT, PM_COPY_WRITE);

//			offset = C_OFF_DAC ;
//			MCopyProtectedMemory((void*)offset, &Dac, C_LEN_DAC, PM_COPY_WRITE);

                    if(MGetTestStatus(C_MAKE_DACAVG) == 1 && MGetAvgMode() == 1)
                    {
                        {
                            MSetTestStatus(C_TEST_AVG,1);	//设置测试状态,AVG已作
                            if( MGetCurveNumber() > 3)MSetCurveNumber(3);//AVG不超过3根线
                        }
                        MSetEquivalentDays(1,1);
                        MSetCompGain(0,C_SETMODE_SETSAVE);
                        MSetSurfGain(surfgain,C_SETMODE_SAVE);
                        range = (int)( ( depthmax * 100/80 )/50.0+0.9)*50;		//放在第8格
                        MSetRange(range,C_SETMODE_SAVE);
                        Dac.diameter = diameter;
                        Dac.length = length;//最大深度、反射体直径、长度

                        retvalue = C_TRUE;
                        break;
                    }
                    DrawDac(0);	//参数为0表示画DAC
                    GateMax.Amp = 1;

                    MSetCompGain(0,C_SETMODE_SETSAVE);
                    WriteCompGain(C_COMPGAIN_HPOSI,C_COMPGAIN_VPOSI);

                    break;
                }
                else if(keycode == C_KEYCOD_RETURN)
                {
                    ClearCursor(2);
                    MKeyRlx();
                    break;
                }
                while(true)
                {
                    keycode = MGetKeyCode(20);
                    if(keycode == C_KEYCODMAX)
                    {
                        rotary = MGetRotary(-1);
                        if( rotary > 0 )keycode = C_KEYCOD_RIGHT;
                        else if( rotary < 0)keycode = C_KEYCOD_LEFT;
                        else continue;
                        break;
                    }
                    if( keycode == C_KEYCOD_RIGHT || keycode == C_KEYCOD_LEFT || keycode ==  C_KEYCOD_RETURN || keycode == C_KEYCOD_CONFIRM )
                    {
                        MKeyRlx();
                        rotary  = 0;
                        break;
                    }
                    else continue;
                }
                if( MGetFunctionMode(C_ECHOMAX_MEMORY))ClearEnvelope();
            }
            if( retvalue == C_TRUE)break;
            ExpendTime(20);//延时0.2s
            MSetGateParaInit();	//门内最高波设初值
            MSetAcquisitionEnable(1,C_SETMODE_SETSAVE);
        }
        else if( keycode == C_FUNC_1 ||keycode == C_FUNC_2||keycode == C_FUNC_3||keycode == C_FUNC_4)
        {

            if(keycode == C_FUNC_1)i=0;
            else if(keycode == C_FUNC_2)i=1;
            else if(keycode == C_FUNC_3)i=2;
            else if(keycode == C_FUNC_4)i=3;


            if(Function.funcMenu[i]==C_ECHO_ENVELOPE+1 )
            {
                ClearEnvelope();
                if( MGetFunctionMode(C_ECHO_ENVELOPE) == 0)
                {
                    MSetFunctionMode(1,C_ECHO_ENVELOPE);
                    MSetFunctionMode(0,C_ECHOMAX_MEMORY);

                }
                else if( MGetFunctionMode(C_ECHO_ENVELOPE) == 1)
                {
                    MSetFunctionMode(0,C_ECHO_ENVELOPE);
                }
                MKeyRlx();
            }
            else if(Function.funcMenu[i]==C_ECHOMAX_MEMORY+1)
            {
                ClearEnvelope();
                if( MGetFunctionMode(C_ECHOMAX_MEMORY) == 0)
                {
                    MSetFunctionMode(1,C_ECHOMAX_MEMORY);
                    MSetFunctionMode(0,C_ECHO_ENVELOPE);
                    MSetGateParaInit();	//门内最高波设初值
                }
                else if( MGetFunctionMode(C_ECHOMAX_MEMORY) == 1)
                {
                    MSetFunctionMode(0,C_ECHOMAX_MEMORY);
                }
                MKeyRlx();
            }

            SysParaDisp();
        }
        else if(keycode == C_KEYCOD_BASEGAIN && SameKeyTime > C_SAMEKEYTIME)
        {
            GainChange();
            MSetColor(C_CR_MENU);
            DisplayPrompt(15);
            MSetColor(C_CR_MENU);
            if( MGetTestStatus(C_MAKE_DACAVG) == 0 )
                //	CMenuOut(0,0,_TestMenu_A1+13,5,1,24);
                TextOut(0,0,1,11,C_CCHAR_VDOT,(u_char*)_TestMenu_A1[MGetLanguage()][2]+2,4);
            else
                //	CMenuOut(0,0,_TestMenu_A1+19,5,1,24);
                TextOut(0,0,1,11,C_CCHAR_VDOT,(u_char*)_TestMenu_A1[MGetLanguage()][3]+2,4);
            GateMax.Amp = 0;
            MKeyRlx();
        }
#if C_DEVTYPE == 1 || C_DEVTYPE == 15
        else if(keycode == C_KEYCOD_AUTOGAIN )
#else
        else if(keycode == C_KEYCOD_BASEGAIN )//&& GetSameKeyTime() > 20)MAdjustGain(0,0,MGetAmpStdMax(),MGetAmpStdMax());
#endif
        {
            if( !MGetFunctionMode(C_ECHOMAX_MEMORY))ClearCursor(2);
            else ClearEnvelope();
            DrawDac(1);

            if ((ChannelPara.wavepara[2]<150 && ChannelPara.wavepara[2]>70)|| test_num == 0)
            {
                MAdjustGain(0,0,MGetAmpStdMax(),MGetAmpStdMax());
            }
            else
            {
                MAdjustGain(1,0,MGetAmpStdMax(),MGetAmpStdMax());
            }
            MSetGateParaInit();	//门内最高波设初值
            GateMax.Amp = 0;
            DrawDac(0);
            MKeyRlx();
        }
#if C_DEVTYPE == 14
        else if (keycode == C_KEYCOD_POINT && test_num > 0)
#else
        else if (keycode == C_KEYCOD_DEL && test_num > 0)
#endif
        {
            int num = 0;
            if( !MGetFunctionMode(C_ECHOMAX_MEMORY))ClearCursor(2);
            EraseDrawRectangle(menu_xpos, menu_ypos, menu_xpos + (char_len+1+2*MGetLanguage())*C_CCHAR_HDOT+18, menu_ypos + crow_height * row_number);
            MSetColor(C_CR_MENU);
            //	CEMenuOut(menu_xpos + C_ECHAR_HDOT,menu_ypos + 4, _TestMenu_AD6 ,char_len,row_number,crow_height);
            TextOut(menu_xpos + C_ECHAR_HDOT-10*MGetLanguage(),menu_ypos + 4,1,28+5*MGetLanguage(),C_CCHAR_VDOT,(u_char*)_TestMenu_AD2[MGetLanguage()][3],4);
//            EMenuOut(menu_xpos +8+ (17+3*MGetLanguage())*C_ECHAR_HDOT,menu_ypos+1,_TestMenu_Enter,1,1,C_ECHAR_VDOT);

            ExpendTime(10);
            MKeyRlx();
            MSetCompGain(0,C_SETMODE_SETSAVE);
            WriteCompGain(C_COMPGAIN_HPOSI,C_COMPGAIN_VPOSI);
            xpos = dac_dist[num] * C_COORWIDTH/MGetRange(3);	//得到采样点距离
            gateamp = pow(10, ( (int)( 200 * log10(255) ) - dac_db[num] + MGetBaseGain() + MGetCompGain() ) /200.0 );
            while(true)
            {
                ypos = C_COORVPOSI + C_COORHEIGHT - 2 - gateamp*2;
                DrawCursor(xpos,ypos,2);

                while(true)
                {
                    keycode = MGetKey();
                    if(keycode == C_KEYCODMAX)
                    {
                        rotary = MGetRotary(-1);
                        if( rotary > 0 )keycode = C_KEYCOD_RIGHT;
                        else if( rotary < 0)keycode = C_KEYCOD_LEFT;
                        else continue;
                        break;
                    }
#if C_DEVTYPE == 14
                    else if(keycode == C_KEYCOD_PLUS || keycode == C_KEYCOD_MINUS || keycode == C_KEYCOD_RIGHT || keycode == C_KEYCOD_LEFT || keycode == C_KEYCOD_POINT || keycode == C_KEYCOD_CONFIRM  || keycode == C_KEYCOD_RETURN )
#else
                    else if( keycode == C_KEYCOD_RIGHT || keycode == C_KEYCOD_LEFT || keycode == C_KEYCOD_DEL || keycode == C_KEYCOD_CONFIRM  || keycode == C_KEYCOD_RETURN )
#endif
                        break;
                }
#if C_DEVTYPE == 14
                if( keycode == C_KEYCOD_PLUS )
                {
                    gateamp+=3;
                }
                else if(keycode == C_KEYCOD_MINUS)
                {
                    gateamp-=3;
                }
                else if(keycode == C_KEYCOD_LEFT)
                {
                    num--;
                    if( num <0)num = test_num-1;
                    ClearCursor(2);
                    xpos = dac_dist[num] * C_COORWIDTH/MGetRange(3);	//得到采样点距离
                    gateamp = pow(10, ( (int)( 200 * log10(255) ) - dac_db[num] + MGetBaseGain() + MGetCompGain() ) /200.0 );
                    MKeyRlx();
                    continue;
                }
                else if(keycode == C_KEYCOD_RIGHT)
                {
                    num++;
                    if( num >= test_num)num = 0;
                    ClearCursor(2);
                    xpos = dac_dist[num] * C_COORWIDTH/MGetRange(3);	//得到采样点距离
                    gateamp = pow(10, ( (int)( 200 * log10(255) ) - dac_db[num] + MGetBaseGain() + MGetCompGain() ) /200.0 );
                    MKeyRlx();
                    continue;
                }

#else
                if( keycode == C_KEYCOD_RIGHT )
                {
                    gateamp+=3;
                }
                else if(keycode == C_KEYCOD_LEFT)
                {
                    gateamp-=3;
                }
#if C_DEVTYPE == 14
                else if (keycode == C_KEYCOD_POINT)
#else
                else if(keycode == C_KEYCOD_DEL)
#endif
                {
                    num++;
                    if( num >= test_num)num = 0;
                    ClearCursor(2);
                    xpos = dac_dist[num] * C_COORWIDTH/MGetRange(3);	//得到采样点距离
                    gateamp = pow(10, ( (int)( 200 * log10(255) ) - dac_db[num] + MGetBaseGain() + MGetCompGain() ) /200.0 );
                    MKeyRlx();
                    continue;
                }
#endif
                else if(keycode == C_KEYCOD_RETURN)
                {
                    DrawDac(1);
                    for(i = num; i < test_num; i++)
                    {
                        dac_dist[i] = dac_dist[i+1];
                        dac_db[i] = dac_db[i+1];
                    }
                    test_num--;
                    Dac.num = test_num;	//选中波次数
                    for(i=0; i< 10; i++)
                    {
                        Dac.db[i] = dac_db[i];
                        Dac.dist[i] = dac_dist[i];
                    }
                    if( test_num < 1)break;
                    if( num >= test_num)num = test_num-1;
                    DrawDac(0);	//参数为0表示画DAC
                    ClearCursor(2);
                    EraseDrawRectangle(menu_xpos, menu_ypos, menu_xpos + (char_len+1+2*MGetLanguage())*C_CCHAR_HDOT+18, menu_ypos + crow_height * row_number);
                    MSetColor(C_CR_MENU);
                    //	CEMenuOut(menu_xpos + C_ECHAR_HDOT,menu_ypos + 4, _TestMenu_AD6 ,char_len,row_number,crow_height);
                    TextOut(menu_xpos + C_ECHAR_HDOT-10*MGetLanguage(),menu_ypos + 4,1,28+5*MGetLanguage(),C_CCHAR_VDOT,(u_char*)_TestMenu_AD2[MGetLanguage()][3],4);
//                    EMenuOut(menu_xpos +8+ (17+3*MGetLanguage())*C_ECHAR_HDOT,menu_ypos+1,_TestMenu_Enter,1,1,C_ECHAR_VDOT);

                    xpos = dac_dist[num] * C_COORWIDTH/MGetRange(3);	//得到采样点距离
                    gateamp = pow(10, ( (int)( 200 * log10(255) ) - dac_db[num] + MGetBaseGain() + MGetCompGain() ) /200.0 );
                    MKeyRlx();
                    ExpendTime(20);
                    continue;
                }
                else if( keycode == C_KEYCOD_CONFIRM)
                {
                    ClearCursor(2);
                    MKeyRlx();
                    break;
                }
                ClearCursor(2);
                dac_db[num] = MGetBaseGain() + MGetCompGain() + (int)( 200 * ( log10(255) - log10(gateamp) ) );
                Dac.num = test_num;	//选中波次数
                for(i=0; i< 10; i++)
                {
                    Dac.db[i] = dac_db[i];
                    Dac.dist[i] = dac_dist[i];
                }
                DrawDac(-1);
            }
            MSetGateParaInit();	//门内最高波设初值
            MKeyRlx();
        }
//		else if(keycode == C_KEYCOD_BASEGAIN && GetSameKeyTime() > 20)MAdjustGain(0,0,MGetAmpStdMax(),MGetAmpStdMax());
        else if( keycode == C_KEYCOD_GATE || keycode == C_KEYCOD_BASEGAIN || keycode == C_KEYCOD_RANGE )
        {
            if( MGetFunctionMode(C_ECHOMAX_MEMORY))ClearEnvelope();
            KeyManage(keycode,1);
            MKeyRlx();
            oldbasegain = MGetBaseGain();

            DisplayPrompt(15);
            MSetColor(C_CR_MENU);
            if( MGetTestStatus(C_MAKE_DACAVG) == 0 )
                //	CMenuOut(0,0,_TestMenu_A1+13,5,1,24);
                TextOut(0,0,1,11,C_CCHAR_VDOT,(u_char*)_TestMenu_A1[MGetLanguage()][2]+2,4);
            else
                //CMenuOut(0,0,_TestMenu_A1+19,5,1,24);
                TextOut(0,0,1,11,C_CCHAR_VDOT,(u_char*)_TestMenu_A1[MGetLanguage()][3]+2,4);

            GateMax.Amp = 0;
        }
        else continue;

        /*	EraseDrawRectangle(menu_xpos, menu_ypos, menu_xpos + (char_len+1)*C_CCHAR_HDOT, menu_ypos + crow_height * row_number) ;
        	MSetColor(C_CR_MENU);
        	if(test_num > 0)CEMenuOut(menu_xpos + C_ECHAR_HDOT,menu_ypos + 4, _TestMenu_AD5 ,char_len,row_number,crow_height);
        	else CEMenuOut(menu_xpos + C_ECHAR_HDOT,menu_ypos + 4, _TestMenu_AD2 ,char_len,row_number,crow_height);
        */
        EraseDrawRectangle(menu_xpos, menu_ypos, menu_xpos + (char_len+1+2*MGetLanguage())*C_CCHAR_HDOT+18, menu_ypos + crow_height * row_number) ;
        MSetColor(C_CR_MENU);

        if(test_num > 0)
        {
            TextOut(menu_xpos + C_ECHAR_HDOT-10*MGetLanguage(),menu_ypos + 4,1,28+5*MGetLanguage(),C_CCHAR_VDOT,(u_char*)_TestMenu_AD2[MGetLanguage()][2],4);
//            EMenuOut(menu_xpos+8 + (20+3*MGetLanguage())*C_ECHAR_HDOT,menu_ypos+1,_TestMenu_Enter,1,1,C_ECHAR_VDOT);
        }
        else TextOut(menu_xpos + C_ECHAR_HDOT-10*MGetLanguage(),menu_ypos + 4,1,28+5*MGetLanguage(),C_CCHAR_VDOT,(u_char*)_TestMenu_AD2[MGetLanguage()][0],4);

    }
    for(i = 0 ; i < 6; i++)
    {
        MSetLineGain(i,linegain[i]);
    }
    MSetFunctionMode(0,C_ECHOMAX_MEMORY);
    MKeyRlx();

    if( retvalue == C_TRUE && MGetTestStatus(C_MAKE_DACAVG) == 0)
    {
        MSetDelay(0,C_SETMODE_SETSAVE);
        MSetSystemMode(0,0);
        DrawDac(2);
        if( MGetThick()*2 <= MGetRange(4) && MGetStdMode()!=11 )// if( MGetThick()*2 <= MGetRange(4) && MGetStdMode()<C_STD_MAX-1 )
        {
            if (MGetLineGain(0)==0 && MGetLineGain(1)==0 && MGetLineGain(2)==0 )
            {
                MSetBaseGain( MGetBaseGain()-200*log10( dac_line[2*C_COORWIDTH/MGetRange(4)]/320.0),C_SETMODE_SETSAVE);
            }
            else if (MGetThick()==0)
            {
                MSetBaseGain( MGetBaseGain()-200*log10( dac_line[2*C_COORWIDTH/MGetRange(4)]/80.0),C_SETMODE_SETSAVE);
            }
            else if (MGetStdMode()==2)	//JB4730
            {
                MSetBaseGain( MGetBaseGain()-200*log10( dac_line[MGetThick()*2*C_COORWIDTH/MGetRange(4)]/40.0),C_SETMODE_SETSAVE);
            }
            else if (MGetStdMode()==7||MGetStdMode()==4)	//CBT3559-94
            {
                MSetBaseGain( MGetBaseGain()-200*log10( dac_line[MGetThick()*2*C_COORWIDTH/MGetRange(4)]/80.0),C_SETMODE_SETSAVE);
            }
             else if (MGetStdMode()==12)	//JB47013
            {
                MSetBaseGain( MGetBaseGain()-200*log10( dac_line[MGetThick()*2*C_COORWIDTH/MGetRange(4)]/80.0),C_SETMODE_SETSAVE);
            }
            else if(MGetLineGain(0)-MGetLineGain(2) < 130)
            {
                MSetBaseGain( MGetBaseGain()-200*log10( dac_line[MGetThick()*2*C_COORWIDTH/MGetRange(4)]/40.0),C_SETMODE_SETSAVE);
            }
            else
            {
                MSetBaseGain( MGetBaseGain()-200*log10( dac_line[MGetThick()*2*C_COORWIDTH/MGetRange(4)]/20.0),C_SETMODE_SETSAVE);
            }
        }
        else
        {
            MSetBaseGain( MGetBaseGain()-200*log10( dac_line[2*C_COORWIDTH/MGetRange(4)]/40.0),C_SETMODE_SETSAVE);
        }
        if (MGetThick()>=30)
        {
            int Scale=MGetScaleMode();
            MSetScaleMode(0,C_SETMODE_SETSAVE);
            if (MGetThick()<160)
            {
                MSetRange(MGetThick() * 100/30+0.5,C_SETMODE_SETSAVE);
            }
            else if(MGetThick()>490)
            {
                MSetRange(MGetThick() * 100/80+0.5,C_SETMODE_SETSAVE);
            }
            else
            {
                MSetRange(MGetThick() * 100/40+0.5,C_SETMODE_SETSAVE);
            }
            MSetScaleMode(Scale,C_SETMODE_SETSAVE);
        }
    }
    return retvalue;
}


int TestMenuStandard(void)
{
    return 0;
}

int TestMenuThick(void)
{
    return 0;
}

int TestMenuSystem(void)
{
    return MenuMonthTest();//月性能校验
}

int CalcuDac(void)
{
    int offset;
    short test_num = 0;	//选中波次数
    short near_time = 1;
    int dac_db[10+2],dac_dist[10+2];	//在画DAC曲线时增加关尾两点，其值与原头尾相同
//	u_short dac_line[308*2];
    //int basegain;
    int i,j;
    int dist1,dist2;
    int delay,range;

    if( (MGetTestStatus(C_TEST_AVG) == 1 || MGetTestStatus(C_MAKE_DACAVG) == 1 )&& MGetAvgMode() == 1)
    {
        near_time = 0;
    }
    else near_time = 3;

    test_num = Dac.num ;	//选中波次数

    if(test_num < 1 || test_num > 10)return -1;

    range = MGetRange(3);
    if( MGetUnitType() > 0)range = range*254/1000;
    delay = MGetDelay(3);
    if( MGetUnitType() > 0)delay = delay*254/1000;


    for(i=0; i< 10; i++)
    {
        dac_db[i] = Dac.db[i];
        dac_dist[i] = Dac.dist[i];
    }

    if( MGetTestStatus(C_MAKE_DACAVG) == 1 || MGetTestStatus(C_TEST_AVG) == 1)
    {
        //AVG
        if( dac_dist[0] <= MGetNearField(near_time) )
        {
            // 第一点在三倍近场区内
            for(i = test_num; i > 0; i--)	//所点向后移一点,增加头
            {
                dac_dist[i] = dac_dist[i-1];
                dac_db[i] = dac_db[i-1];
            }
        }
        else
        {
            // 第一点在三倍近场区外
            for(i = test_num + 1; i > 1; i--)	//所有点向后移两点,增加头
            {
                dac_dist[i] = dac_dist[i-2];
                dac_db[i] = dac_db[i-2];
            }
            test_num++;
            if( MGetNearField(near_time) < dac_dist[2]/5) dac_dist[1] = dac_dist[2]/5;
            else
                dac_dist[1] = MGetNearField(near_time);//第2点为3倍近场区

            dac_db[1] = 400*( log10(dac_dist[2]) - log10(dac_dist[1]) ) - 2 * MGetAttenuate(dac_dist[2]) * ( dac_dist[2] - dac_dist[1] );

            if( dac_db[1] >= dac_db[2] )dac_db[1] = 0;
            else dac_db[1] = dac_db[2] - dac_db[1] ;
            dac_dist[0] = 0;
            dac_db[0] = dac_db[1];
        }

        dac_dist[0] = 0;	//增加的头距离为0，dB数与原第一点相同
        test_num++;
        dac_dist[test_num] = range + delay;//MGetRange(3) + MGetDelay(3);		//最后一点为当前最大声程
        ///if( MGetUnitType() > 0)dac_dist[test_num] = dac_dist[test_num] *254/1000;//化成MM
        if(dac_dist[test_num] >= 3 * dac_dist[test_num -1] )
        {
            //最后一个测试点距屏幕右端非常大，增加一点
            test_num++;
            dac_dist[test_num] = dac_dist[test_num-1];
            dac_dist[test_num - 1] = 2 * dac_dist[test_num - 2];
//			dac_db[test_num - 1] = dac_db[test_num - 2] + 120 + 2 * MGetAttenuate(dac_dist[test_num-1]/1000) * dac_dist[test_num - 2] ;
            dac_db[test_num - 1] = dac_db[test_num - 2] + 120 + 2 * MGetAttenuate(dac_dist[2]) *( dac_dist[2] - dac_dist[1] );
        }
        //dac_db[test_num] = dac_db[test_num - 1] + 400*( log10(dac_dist[test_num]) - log10(dac_dist[test_num - 1]) ) + 2 * MGetAttenuate(dac_dist[test_num-1]/10) * ( dac_dist[test_num] - dac_dist[test_num - 1] );
        dac_db[test_num] = dac_db[test_num - 1] + 400*( log10(dac_dist[test_num]) - log10(dac_dist[test_num - 1]) ) + 2 * MGetAttenuate(dac_dist[2]) * ( dac_dist[test_num] - dac_dist[test_num - 1] );
        //dac_db[test_num] = dac_db[test_num - 1] + 400*( log10(dac_dist[test_num]*1.0/dac_dist[test_num - 1]) ) + 2 * MGetAttenuate(dac_dist[2]) * ( dac_dist[test_num] - dac_dist[test_num - 1] );
        test_num++;			//增加一头一尾共两点
    }
    else
    {
        //DAC
        for(i = test_num; i > 0; i--)	//所点向后移一点,增加头
        {
            dac_dist[i] = dac_dist[i-1];
            dac_db[i] = dac_db[i-1];
        }
        dac_dist[0] = 0;	//增加的头距离为0，dB数与原第一点相同
        test_num++;
        dac_dist[test_num] = range + delay;//MGetRange(3) + MGetDelay(3);		//最后一点为当前最大声程
        ///if( MGetUnitType() > 0)dac_dist[test_num] = dac_dist[test_num] *254/1000;//化成MM
        dac_db[test_num] = dac_db[test_num-1];
        test_num++;			//增加一头一尾共两点
    }

    for(i = 1; i< test_num ; i++)
    {
        //对延时进行处理
        //if(MGetDelay(3) < dac_dist[i])
        if( delay < dac_dist[i])
        {
            //有延时，延掉i-1个点
            dac_db[i-1] = GetMiddleValue(dac_dist[i-1],dac_db[i-1],dac_dist[i],dac_db[i],delay );//MGetDelay(3) );
            dac_dist[i-1] = delay;//MGetDelay(3);
            test_num = test_num - (i - 1);		//测试点减少i-1个点
            for(j = 0; j<test_num ; j++)
            {
                //将延时点前去除
                dac_dist[j] = dac_dist[j+i-1];
                dac_db[j] = dac_db[j+i-1];
            }
            break;
        }
    }

    for(i = 1 ; i < test_num; i++)//增加 延时+距离 一点
    {
        if( dac_dist[i] >delay + range)// MGetDelay(3)+MGetRange(3) )
        {
            dac_db[i] = GetMiddleValue(dac_dist[i-1],dac_db[i-1],dac_dist[i],dac_db[i], delay+range);//MGetDelay(3)+MGetRange(3) );
            dac_dist[i] = delay+range;//MGetDelay(3)+MGetRange(3);
            test_num = i+1;
            break;
        }
    }
    for(i = 0; i < test_num; i++)
    {
        if(dac_db[i] > 65535 )dac_db[i] = 65535;
        else if(dac_db[i] < 0 )dac_db[i] = 0;
    }
    dist2 = 0;
    dac_line[dist2] = dac_db[0];
    //Write_Number(10,30,22,4,0,0);


    for(i = 1, j=0, offset = C_OFF_DACDB; i< test_num && dist2 < C_SCREEN_WIDTH-1; i++)
    {
        //将距离值化为屏幕上的点
        dist1 = dist2 ;
//		dist2 = ( dac_dist[i] - MGetDelay(3) ) * C_COORWIDTH / MGetRange(3);
        dist2 = ( dac_dist[i] - delay ) * C_COORWIDTH / range;

        if( dist2 >= C_SCREEN_WIDTH)
        {
            dac_line[C_SCREEN_WIDTH-1] = GetMiddleValue(dist1,dac_line[0],dist2,dac_db[i], C_SCREEN_WIDTH-1 );
            dist2 = C_SCREEN_WIDTH-1;
        }
        else dac_line[dist2] = dac_db[i];
        j = dist2 - dist1;
        dac_line[j] = dac_line[dist2];
        GetLine_TwoDot(dac_line,0,j);
        //	dist2--;
        //	j--;
        MCopyProtectedMemory((void*)offset, dac_line, (j+1)*C_SIZE_SHORT, PM_COPY_WRITE);
        dac_line[0] = dac_line[j];
        offset += (j)*C_SIZE_SHORT;
    }

    return 2;
}


int DrawDac2(int mode)		//画和或计算DAC曲线,mode=0计算,=1不
{
    int offset;
    int basegain;
    int i;
    int dist1,dist2;
    int step = 4;
//	u_short dac_line[308*2];
//	u_int dac_temp;
    int retvalue = -1;
#define C_EXPF 2.302585
    //#define C_EXPF 1.848489938
    //#define C_EXPF 1.319249827
    int test_num;
    double dTemp;
//	int xpos, ypos;
    u_char pzText[2] = " ";

    pzText[0] = PHI;

    retvalue = 0;
    if( (MGetSystemMode() == 0 && MGetTestStatus(C_TEST_DAC) == 0 && MGetTestStatus(C_TEST_AVG) == 0 )|| MGetEchoMode() == C_RF_WAVE)
    {
//Write_Number(10,30,1,4,0,0);
        retvalue = -1;		//不在自动调节状态，也未制作DAC\AVG,无射频
    }

    //offset = C_OFF_DAC + 20*C_SIZE_SHORT;
    //MCopyProtectedMemory(&test_num, (void*)offset,1*C_SIZE_SHORT, PM_COPY_READ);


///	if(test_num < 1 || test_num > 10)return -1;

    test_num = Dac.num;
    if( test_num < 1)retvalue = -1;
    if(mode == 0 && retvalue != -1)
    {
//Write_Number(10,30,2,4,0,0);
        if( CalcuDac() < 0)retvalue = -1;
    }
//Write_Number(10,30,3,4,0,0);

    if( retvalue == -1 )
    {
        if( MGetEchoMode() != C_RF_WAVE && MGetFunctionMode(C_DEPTH_COMPENSATE) )	//返回测试状态<0=未测>=0已测，
        {
            /*           dist1 = 0;
                       dist2 = C_COORWIDTH-1;
                       dac_line[dist2] = (u_int)( 160 / Mexpf(C_EXPF*( MGetDepthGain() )/200.0, C_SIZE_SHORT) + 0.5 );
                       //Write_Number( 100,30,dac_line[dist1],10,0,0);
                       dac_line[dist1] = 160;
                       GetLine_TwoDot(dac_line,dist1,dist2);
                       //MDacGatePeak();
            #if C_DEVLIB == 1 || C_DEVLIB == 2 || C_DEVLIB == 23 || C_DEVLIB == 24
                      dac_coe = 160.0/(double)dac_line[0];
                       //Write_Number( 100,40,dac_coe*1000,10,3,0);
                       //	DrawCurver(dac_line,0,C_COORWIDTH);
                       for(i = 0; i < C_COORWIDTH; i++)
                       {
                           if( dac_line[i] < 256)dactable[i] = (int)( dac_coe*dac_line[i] + 0.5);
                           else dactable[i] = 160;
                       }
                       MActivateDac(1);
            #endif*/

            for(i=0; i<500; i++)
            {

                //       SetAmplifierDynamicGain(dac_line[i]-dac_line[0]) ;
            }
            MActivateDac(1);
        }
        return retvalue;
    }
//Write_Number(10,30,4,4,0,0);

    //if( (MGetSystemMode() == 1 || MGetTestStatus(C_TEST_DAC) == 1 || MGetTestStatus(C_TEST_AVG) == 1) )
#if C_DEVLIB == 1 || C_DEVLIB == 2 || C_DEVLIB == 23 || C_DEVLIB == 24
    if( MGetSystemMode() == 0 && MGetFunctionMode(C_DEPTH_COMPENSATE))
    {
        u_short DynamicGain[C_MAX_DOT_NUMBER];
        offset = C_OFF_DACDB;
        MCopyProtectedMemory( dac_line, (void*)offset, C_LEN_DACDB, PM_COPY_READ);

        for(i=0; i<500; i++)
        {
            DynamicGain[i]=10;//dac_line[i]-dac_line[0] ;
        }
        Write_Number(20,100,1,1,0,0);

        SystemInit(480,500);
        Write_Number(20,100,11,2,0,0);
//        SetAmplifierDynamicGain(DynamicGain) ;
        /* basegain = MGetBaseGain() + 481.31;	//基准增益
         dist2 = 0;
         //dac_line[dist2] = (u_int)expf( C_EXPF*( basegain - dac_line[dist2] )/200.0) ;
        dac_line[dist2] = (u_int)( dac_line[dist2] + MGetLineGain(0)  - ( 200 * ( log10(255) - log10(MGetAmpStdMax()) ) ) + 0.5) ;
         dac_line[dist2] /= 5;
         while(dist2 <= C_COORWIDTH)
         {
             dist1 = dist2;
             dist2 += step;
             //dac_line[dist2] = (u_int)expf(C_EXPF*( basegain - dac_line[dist2] )/200.0) ;
        	dac_line[dist2] = (u_int)( dac_line[dist2] + MGetLineGain(0) - ( 200 * ( log10(255) - log10(MGetAmpStdMax()) ) ) + 0.5) ;
             dac_line[dist2] /= 5;
             GetLine_TwoDot(dac_line,dist1,dist2);
         }
         for(i = 0 ; i < C_COORWIDTH; i ++ )
         {
             if(dac_line[i] >= C_COORHEIGHT)dac_line[i] = C_COORHEIGHT - 1;
         }
         //MDacGatePeak();

         dac_coe = 160.0/(double)dac_line[0];
        #if ( C_DEVLIB == 1 || C_DEVLIB	== 2 || C_DEVLIB == 23 || C_DEVLIB == 24)
         for(i = 0; i < C_COORWIDTH; i++)
         {
             if( dac_line[i] < 256)dactable[i] = (int)( dac_coe*dac_line[i] + 0.5);
             else dactable[i] = 160;
         }
        #endif
        */
        Write_Number(20,100,2,1,0,0);
        MActivateDac(1);
        Write_Number(20,100,3,1,0,0);
    }
    else
    {
#if ( C_DEVLIB == 1 || C_DEVLIB	== 2 || C_DEVLIB == 23 || C_DEVLIB == 24)
        /*    for(i = 0; i < C_COORWIDTH; i++)
            {
                dactable[i] = 160;
            }*/
        MActivateDac(0);
#endif
    }
//-------dac-------
#endif

//Write_Number(10,30,5,4,0,0);
    offset = C_OFF_DACDB;
    MCopyProtectedMemory( dac_line, (void*)offset, C_LEN_DACDB, PM_COPY_READ);

    if( (MGetSystemMode() == 1 && MGetTestStatus(C_TEST_DAC) == 0 && MGetTestStatus(C_TEST_AVG) == 0) )
    {
        basegain = MGetBaseGain() + 481.31;	//基准增益
    }
    else
    {
        basegain = MGetBaseGain() + MGetLineGain(0) + 481.31;	//基准增益，应加上判废线
    }
    dist2 = 0;
    //dac_line[dist2] = (u_int)expf( C_EXPF*( basegain - dac_line[dist2] )/200.0) ;
    dac_line[dist2] = (u_int)( dac_line[dist2] + MGetLineGain(0)  - ( 200 * ( log10(255) - log10(MGetAmpStdMax()) ) ) + 0.5) ;
    dac_line[dist2] /= 5;
    if(MGetFunctionMode(C_DEPTH_COMPENSATE) == 1)
    {
        dist1 = dist2;
        dist2 = C_COORWIDTH;
        dac_line[dist2] = dac_line[dist1];
        GetLine_TwoDot(dac_line,dist1,dist2);
    }
    else
    {
        while(dist2 < C_SCREEN_WIDTH)
        {
            dist1 = dist2;
            dist2 += step;
            //dac_line[dist2] = (u_int)expf(C_EXPF*( basegain - dac_line[dist2] )/200.0) ;
            dac_line[dist2] = (u_int)( dac_line[dist2] + MGetLineGain(0)  - ( 200 * ( log10(255) - log10(MGetAmpStdMax()) ) ) + 0.5) ;
            dac_line[dist2] /= 5;
            GetLine_TwoDot(dac_line,dist1,dist2);
        }
    }

    MSetColor(C_CR_CURVE);
    DrawCurver(dac_line,0,C_COORWIDTH);
    MSetColor(C_CR_UNDO);
    if( MGetSystemMode() == 1 )return 1;

//Write_Number(10,30,6,4,0,0);

    int line;
    MSetColor(C_CR_CURVE);
    for(line = 1; line < MGetCurveNumber(); line++)
    {
        if( MGetLineGain(line) != MGetLineGain(line-1) )
        {
            dTemp = ( MGetLineGain(line-1) - MGetLineGain(line) )/5  ;
            dac_line[0] = (int)( dac_line[0]-dTemp + 0.5);
            for(i = 4 ; i < C_COORWIDTH+4; i+=4)
            {
                dac_line[i] = (int)( dac_line[i]-dTemp + 0.5);
                dac_line[i-2] = (int)( ( dac_line[i-4] + dac_line[i] )/2.0 + 0.5);
                dac_line[i-3] = (int)( ( dac_line[i-4] + dac_line[i-2] )/2.0 + 0.5);
                dac_line[i-1] = (int)( ( dac_line[i-2] + dac_line[i] )/2.0 + 0.5);
            }
            DrawCurver( dac_line,0,C_COORWIDTH);

        }
    }
    MSetColor(C_CR_UNDO);
    return 1;
}

int DrawDac(int mode)		//画和或计算DAC曲线,mode=0计算,=1不
{

    int offset;
    int basegain;
    int i;
    int dist1,dist2;
    int step = 1;
//	u_short dac_line[308*2];
//	u_int dac_temp;
    int retvalue = -1;
    //#define C_EXPF 1.848489938
    //#define C_EXPF 1.319249827
    int test_num;
    double dTemp;
//	int xpos, ypos;
    u_char pzText[2] = " ";

    POINT PTin[16],PTout[C_SCREEN_WIDTH];
//;	if( MGetDacShowMode() > 0)return DrawDac2(mode);//可以显示从低到高的DAC曲线

    pzText[0] = PHI;

    retvalue = 0;
    if( (MGetSystemMode() == 0 && MGetTestStatus(C_TEST_DAC) == 0 && MGetTestStatus(C_TEST_AVG) == 0 )|| MGetEchoMode() == C_RF_WAVE)
    {
//Write_Number(10,30,1,4,0,0);
        retvalue = -1;		//不在自动调节状态，也未制作DAC\AVG,无射频
    }


    test_num = Dac.num;
    if( test_num < 1)retvalue = -1;
    if(mode <= 0 && retvalue != -1)
    {
        if( CalcuDac() < 0)retvalue = -1;
    }



    u_int DynamicGain[C_MAX_DOT_NUMBER];
    //if( (MGetSystemMode() == 1 || MGetTestStatus(C_TEST_DAC) == 1 || MGetTestStatus(C_TEST_AVG) == 1) )
#if ( C_DEVLIB == 1 || C_DEVLIB	== 2 || C_DEVLIB == 23 || C_DEVLIB == 24)
    if( MGetSystemMode() == 0 && MGetFunctionMode(C_DEPTH_COMPENSATE))
    {

        offset = C_OFF_DACDB;
        MCopyProtectedMemory( dac_line, (void*)offset, C_LEN_DACDB, PM_COPY_READ);

        for(i=0; i<500; i++)
        {
            DynamicGain[i]=(dac_line[i]-dac_line[0]+MGetBaseGain()) ;//0;//170;//
            if(DynamicGain[i]>900 ||DynamicGain[i]<0)
            {
                //   Write_Number(10,60,DynamicGain[i],10,0,0);
                //   MAnyKeyReturn();
                DynamicGain[i]=0;

            }
        }
        /*  for(i=100;i<500;i++)
          {
              DynamicGain[i]=40;//dac_line[0]-dac_line[i] ;
          }*/

        // for(i=0;i<240;i++)
        {
            //    Write_Number((i%10)*64,(i/10)*20,DynamicGain[i+200],4,0,0);
        }
//MAnyKeyReturn();
        // SystemInit(480,500);
        //MSetBaseGain( MGetBaseGain(), C_SETMODE_SET );
        SetAmplifierDynamicGain(DynamicGain) ;

        AmplifierDynamicGainOnOff(1) ;
//   AmplifierDynamicGainOnOff(0) ;
        MSetAmplifierGain(MGetBaseGain());
    }
    else
    {
        /*      MActivateDac(0);
              for(i=0;i<500;i++)
              {
                 DynamicGain[i]=MGetBaseGain() ;
              }
              SetAmplifierDynamicGain(DynamicGain) ;
        */
        AmplifierDynamicGainOnOff(0) ;
    }
    if( retvalue == -1 )
    {
        if( MGetEchoMode() != C_RF_WAVE && MGetFunctionMode(C_DEPTH_COMPENSATE) )	//返回测试状态<0=未测>=0已测，
        {

            //  for(i=0;i<500;i++)
            {

                //  SetAmplifierDynamicGain(dac_line[i]-dac_line[0], i) ;
            }
            //   MActivateDac(1);

            //Write_Number(10,70,3,4,0,0);

            /*            dist1 = 0;
            dist2 = C_COORWIDTH-1;
            dac_line[dist2] = (u_int)( 160 / Mexpf(C_EXPF*( MGetDepthGain() )/200.0, C_SIZE_SHORT) + 0.5 );
            //Write_Number( 100,30,dac_line[dist1],10,0,0);
            dac_line[dist1] = 160;
            GetLine_TwoDot(dac_line,dist1,dist2);
            //MDacGatePeak();
            #if ( C_DEVLIB == 1 || C_DEVLIB	== 2 || C_DEVLIB == 23 || C_DEVLIB == 24)
            dac_coe = 160.0/(double)dac_line[0];
            for(i = 0; i < C_COORWIDTH; i++)
            {

                if( dac_line[i] < 256)dactable[i] = (int)( dac_coe*dac_line[i] + 0.5);
                else dactable[i] = 255;
            }
            MActivateDac(1);
            #endif*/
        }
        return retvalue;
    }
//Write_Number(10,30,4,4,0,0);
//-------dac-------
#endif

//Write_Number(10,30,5,4,0,0);
    offset = C_OFF_DACDB;
    MCopyProtectedMemory( dac_line, (void*)offset, C_LEN_DACDB, PM_COPY_READ);

    /*
    for(i = 0; i < C_COORWIDTH; i++)
    {	//将距离值化为屏幕上的点
    dac_line[i] /= 4;
    }
    DrawCurver(dac_line,0,C_COORWIDTH);
    for(i = 0; i < C_COORWIDTH; i++)
    {	//将距离值化为屏幕上的点
    dac_line[i] *= 4;
    }
    */
    /*for(i=0;i<C_COORWIDTH;i++)
    {
        Write_Number((i%10)*64,(i/10)*24,dac_line[i],3,0,0);
    }
        Write_Number(0,0,1,3,0,0);
    MAnyKeyReturn();*/
    if( (MGetSystemMode() == 1 && MGetTestStatus(C_TEST_DAC) == 0 && MGetTestStatus(C_TEST_AVG) == 0) )
    {
        basegain = MGetBaseGain() + 481.31;	//基准增益
    }
    else
    {
        basegain = MGetBaseGain() + MGetLineGain(0) + 481.31;	//基准增益，应加上判废线
    }
    dist2 = 0;
    //dac_line[dist2] = (u_int)expf( C_EXPF*( basegain - dac_line[dist2] )/200.0) ;
    dac_line[dist2] = (u_int)(Mexpf(C_EXPF*( basegain - dac_line[dist2] )/200.0, C_SIZE_SHORT) *2);
    if(dac_line[dist2] > 32767 )    dac_line[dist2] = 32767;
    if(MGetFunctionMode(C_DEPTH_COMPENSATE) == 1)
    {
        dist1 = dist2;
        dist2 = C_COORWIDTH;
        dac_line[dist2] = dac_line[dist1];
        GetLine_TwoDot(dac_line,dist1,dist2);
    }
    else
//		0.011512925465
    {
        int temp[10];
        for(i=0; i<10; i++)
        {
            temp[i]=Dac.db[i];
        }
        while(dist2 < C_SCREEN_WIDTH)
        {
            dist1 = dist2;
            dist2 += step;
            //dac_line[dist2] = (u_int)expf(C_EXPF*( basegain - dac_line[dist2] )/200.0) ;
            dac_line[dist2] = (u_int)(Mexpf(C_EXPF*( basegain - dac_line[dist2] )/200.0,C_SIZE_SHORT)*2);// C_SIZE_SHORT) ;
            //dac_line[dist2] = (u_int)exp( (0.011512925465*( basegain - dac_line[dist2] ) ) );
            //dac_line[dist2] = (u_int)pow(10.0,( basegain - dac_line[dist2] )/200.0) ;

            if(dac_line[dist2] > 32767 )    dac_line[dist2] = 32767;
            GetLine_TwoDot(dac_line,dist1,dist2);

            /*   PTin[0].x = dist1;
               PTin[0].y = dac_line[dist1];
               PTin[1].x = dist2;
               PTin[1].y = dac_line[dist2];

               int nnum = tspLine(PTin,PTout,2,0,0,0,0,0);

                  for(i=dist1;i<nnum;i++)
                   {
                       dac_line[i] = PTout[i].y;
                   }*/
        }
        /*
        for(i=0;i<C_COORWIDTH;i++)
        {
            Write_Number((i%10)*64,(i/10)*24,dac_line[i],3,0,0);
        }
        */
        for(i=0; i<10; i++)
        {
            Dac.db[i]=temp[i];
        }
//    offset = C_OFF_DACDB;
//    MCopyProtectedMemory( dac_line, (void*)offset, C_LEN_DACDB, PM_COPY_READ);
        /* PTin[0].x=0;
         PTin[0].y=(u_int)Mexpf(C_EXPF*( basegain - dac_line[0] )/200.0,C_SIZE_SHORT);
         PTin[7].x=C_SCREEN_WIDTH-1;
         PTin[7].y=dac_line[C_SCREEN_WIDTH-1];*/
        /*    DrawCurver(dac_line,0,C_COORWIDTH);
           Write_Number(50,100,100,3,0,0);
           MAnyKeyReturn();
           int nstep = C_SCREEN_WIDTH/7;
           for(i=0;i<8;i++)
           {
               PTin[i].x = nstep*i;
               PTin[i].y = dac_line[nstep*i];//dac_line[nstep*i];
           }
           int nnum = tspLine(PTin,PTout,8,0,0,0,0,0);
          for(i=0;i<nnum;i++)
           {
               dac_line[i] = PTout[i].y;
           }
           DrawCurver(dac_line,0,C_COORWIDTH);
           Write_Number(50,100,nnum,3,0,0);
           MAnyKeyReturn();
         for(i=0;i<nnum;i++)
           {
               dac_line[i] = PTout[i].y;
           }
           MCopyProtectedMemory((void*)offset, dac_line, C_LEN_DACDB, PM_COPY_WRITE);*/

    }
    /*for(i=0;i<C_COORWIDTH;i++)
    {
        Write_Number((i%10)*64,(i/10)*24,dac_line[i],3,0,0);
    }
        Write_Number(0,0,2,3,0,0);
    MAnyKeyReturn();*/
    if( !((MGetTestStatus(C_TEST_AVG) == 1 || MGetTestStatus(C_MAKE_DACAVG) == 1 ) && MGetAvgMode() == 1) )
    {
        if( mode < 0) DrawCurver(daccurve,0,C_COORWIDTH);
        for( i = 0; i < C_SCREEN_WIDTH; i++)    daccurve[i] = dac_line[i];
        if( mode < 2) DrawCurver(daccurve,0,C_COORWIDTH);
        if( MGetSystemMode() == 1 ) return 1;
    }
//Write_Number(10,30,6,4,0,0);

    int line;
    for(line = 1; line < MGetCurveNumber(); line++)
    {
        if( MGetLineGain(line) != MGetLineGain(line-1) )
        {
            dTemp = expf( C_EXPF*( MGetLineGain(line-1) - MGetLineGain(line) )/200.0) ;
            //dac_line[0] = (int)( dac_line[0]/dTemp + 0.5);

            for(i = 0 ; i < C_COORWIDTH; i++)
            {
                dac_line[i] = (int)( dac_line[i]/dTemp + 0.5);
            }
            /*	for(i = 4 ; i < C_COORWIDTH+4; i+=4)
            	{
            		dac_line[i] = (int)( dac_line[i]/dTemp + 0.5);
            		dac_line[i-2] = ( (int)( dac_line[i-4] + dac_line[i]  ) ) >> 1;
            		dac_line[i-3] = ( (int)( dac_line[i-4] + dac_line[i-2] ) ) >>1;
            		dac_line[i-1] = ( (int)( dac_line[i-2] + dac_line[i] ) ) >>1;
            	}*/
            if( !((MGetTestStatus(C_TEST_AVG) == 1 || MGetTestStatus(C_MAKE_DACAVG) == 1 ) && MGetAvgMode() == 1) )
            {
                if( mode < 2)   DrawCurver( dac_line,0,C_COORWIDTH);
            }
        }
    }
    return 1;
}

int GetMiddleValue(int dist1,int db1,int dist2,int db2,int dist)
{
    //由两点得中间点的当量
    int retvalue ;

    if(dist >= dist1 && dist <= dist2 && dist1 != dist2)
    {
        retvalue = (double)( dist - dist1 ) * ( db2 - db1 )/(double)( dist2 - dist1 ) + db1;
    }
    else retvalue = -1;	//返回-1表示输入参数有误

    return retvalue ;
}


int GetLine_TwoDot(u_short line[],int dist1,int dist2)
{

//*
    int dist;
    double k;
    if( dist1 > dist2 + 1 )
    {
        dist = dist1;
        dist1 = dist2;
        dist2 = dist;
    }
    else if( dist1 >= dist2 - 1)return -1;
    if( dist2 >= C_COORWIDTH)dist2 = C_COORWIDTH;

    k = (double)( line[dist2] - line[dist1])/(double)(dist2 - dist1);
    for( dist = 1 ; dist1 + dist < dist2; dist++)
    {
        line[dist1 + dist] = (int)( dist*k + line[dist1] );
    }

    /*/
    	int dist = ( (dist1 + dist2) / 2 );

    	if( dist > dist1 )
    	{
    		line[dist] = ( ( line[dist1] + line[dist2] ) / 2 );
    		GetLine_TwoDot(line,dist1,dist);
    		GetLine_TwoDot(line,dist,dist2);
    	}
    //*/
    return 1;
}

void GetCenterPoint(int dist1,int db1,int dist2,int db2)
{
    int dist = ( (dist1 + dist2) >> 1);
    int db = ( (db1 + db2) >> 1);

    if( dist > dist1)
    {
        GetCenterPoint(dist1,db1,dist,db);
    }
}


void GetDacCompensateTable(void)
{
    int offset;
    int basegain;
    //int i,j;
    int dist1,dist2;
    int step = 14;
    u_short dac_line[C_SCREEN_WIDTH];
    //int test_num;
    u_short first_high;
    basegain = MGetBaseGain() + 481.31;	//基准增益

    if( MGetSystemMode() != 0 && MGetTestStatus(C_TEST_DAC) == 1)
    {
        offset = C_OFF_DACDB;
        MCopyProtectedMemory( dac_line, (void*)offset, C_LEN_DACDB, PM_COPY_READ);
        basegain = MGetBaseGain() + 481.31;	//基准增益
        dist2 = 0;
        dac_line[dist2] = (int)expf(2.302585*( basegain - dac_line[dist2] )/200.0) ;
        first_high = dac_line[ dist2 ];
        dac_line[dist2] = 256;	//扩大256倍
        while(dist2 <= C_COORWIDTH)
        {
            dist1 = dist2;
            dist2 += step;
            dac_line[dist2] = (int)expf(2.302585*( basegain - dac_line[dist2] )/200.0) ;
            dac_line[dist2] = ( dac_line[ dist2 ] << 8 )/first_high;	//扩大256倍
            GetLine_TwoDot(dac_line,dist1,dist2);
        }
    }
}


int TestMenuFft(void)
{
    int xpos,ypos;
    int deci_len = 1;
    int number;
    int frequency = 0;
    int keycode;
    int offset;
//	int retvalue = C_FALSE;
    /*	short mode;

    	short gain1 = MGetBaseGain();
    	short gain2 = MGetCompGain();
    	short gain3 = MGetSurfGain();
    	short scale = MGetScaleMode();
    	short fre0 = MGetFrequence(),fre1;
    	short delay0 = MGetDelay(scale);
    	short echomode = MGetEchoMode();
    	short bandmode = MGetBandMode();
    	short range0 = MGetRange(1);
    	short posi,wide,high;
    	short daccomp = MGetFunctionMode(C_DEPTH_COMPENSATE);
    	short probemode = MGetProbeMode();

    	mode = MGetSaveFrequency();

    	posi = MGetGatePara(0,0);
    	wide = MGetGatePara(0,1);
    	high = MGetGatePara(0,2);
    //*/
    //  int savefre;
    int bandmode;   // = MGetBandMode();
    //  savefre = MGetSaveFrequency( ) ;
    AllParaStore();
    MSetFunctionMode(0, C_FUNCTION_ALL);

    MSetTestStatus(C_TEST_ALL,0);	//设置测试状态,DAC作

    xpos = 0;
    ypos = 0;
    //EraseDrawRectangle(xpos, ypos, 28*8, 229) ;
    MSetColor(C_CR_MENU);
    //CEMenuOut(xpos, ypos, _Menu_DB3 ,8,1,24);	//请输入探头频率
    TextOut(xpos, ypos,1,14+MGetLanguage()*12,8,(u_char*)_Menu_DB[MGetLanguage()][1],0);
    number = 0;
    deci_len = 2;
    if ( Input_Number(xpos + (17+MGetLanguage()*12)*C_ECHAR_HDOT,ypos,&number,2,&deci_len,0) )
    {
        MSetFrequence(number,C_SETMODE_SETSAVE);

        //	CEMenuOut(xpos, ypos, _Menu_DB2 ,8,1,24);	//试块声程
        MEraseWindow(xpos, ypos,xpos+C_CCHAR_HDOT*7, ypos+24);
        TextOut(xpos, ypos,1,14+MGetLanguage()*12,8,(u_char*)_Menu_DB[MGetLanguage()][0],0);

        number = 0;
        if( MGetUnitType() > 0)deci_len = 3;//单位inch
        else deci_len = 1;
        if( Input_Number(xpos + (17+MGetLanguage()*12)*C_ECHAR_HDOT,ypos,&number,2,&deci_len,0) )
        {
            u_char *sampbuff;
            u_char echobuffer[C_LEN_SAMP];	//用来存储包络或峰值
            int i;

            MSetSaveFrequency(1, C_SETMODE_SETSAVE ) ;
            SystemParaStore();
            ChannelParaStore();

            MEraseWindow(0, 0, 26*16,24) ;
            DisplayPrompt(15);
            //	MSetProbeMode(C_P_NORMAL,C_SETMODE_SETSAVE);
            if( DisplayQuery(1) == 1)
            {
                if( TestMenuOffset() == C_FALSE )return C_FALSE;
            }
            DisplayPrompt(15);

            MSetAcquisition(1);
            MSetEchoMode(C_RF_WAVE,C_SETMODE_SETSAVE);
            if( MGetFrequence() < 400 )MSetBandMode(0,C_SETMODE_SETSAVE);
            else if( MGetFrequence() < 800 )MSetBandMode(1,C_SETMODE_SETSAVE);
            else if( MGetFrequence() < 1200 )MSetBandMode(2,C_SETMODE_SETSAVE);
            else MSetBandMode(3,C_SETMODE_SETSAVE);

            //MSetBandMode(bandmode,C_SETMODE_SAVE);
            MSetBGateMode(0,C_SETMODE_SETSAVE);
            MSetGatePara(5,28*9,120,0,C_SETMODE_SETSAVE);
            MChannelRenovate();

            MSetScaleMode(2,C_SETMODE_SETSAVE);
            if( number > 1000)MSetScaleDelay(number - 40,C_SETMODE_SETSAVE);
            else MSetScaleDelay(number * 96/100,C_SETMODE_SETSAVE);
            MSetBaseGain(100,C_SETMODE_SETSAVE);
            MSetCompGain(0,C_SETMODE_SETSAVE);
            MSetSurfGain(0,C_SETMODE_SETSAVE);
            MSetAcquisition(1);

            SystemParaStore();
            ChannelParaStore();

            ExpendTime(50);
            MKeyRlx();

            while(true)
            {
                MEraseWindow(0, 0, 20*16,24) ;
                MSetColor(C_CR_MENU);
                //	CEMenuOut(0,0, _Menu_DB4 ,8,1,24);	//探头频普测试中
                MEraseWindow(xpos, ypos,xpos+C_CCHAR_HDOT*7, ypos+24);
                TextOut(0, 0,1,21+MGetLanguage()*2,16,(u_char*)_Menu_DB[MGetLanguage()][2],4);
                //MAdjustGain(0,0,120,160) ;
                frequency = test_fft();
                MEraseWindow(0, 0, 10*32, 24) ;
                MSetColor(C_CR_MENU);
                //	CEMenuOut(0, 0, _Menu_DB5 ,10,1,24);
                MEraseWindow(xpos, ypos,xpos+C_CCHAR_HDOT*7, ypos+24);
                TextOut(0, 0,1,22+MGetLanguage()*3,32,(u_char*)_Menu_DB[MGetLanguage()][3],4);
                Write_Number(5*16+2+124*MGetLanguage(),4,frequency,4,2,0);
                keycode = MAnyKeyReturn();
                if( keycode == C_KEYCOD_RETURN)
                {
                    sampbuff = GetSampleBuffer();
                    MSetAcquisitionEnable(0,C_SETMODE_SETSAVE);
                    for( i = 0; i< C_LEN_SAMP; i++)
                    {
                        if( sampbuff[i] >= 28)echobuffer[i] = sampbuff[i]-28;//-28;
                        else echobuffer[i] =0;
                    }
                    if( DisplayQuery(5) == 1)
                    {
                        MSetFrequence(frequency,C_SETMODE_SETSAVE);
                        MSetSystemMode(0,0);	//设置成一般状态
                        //	MSetBandMode(bandmode,C_SETMODE_SETSAVE);
                        SystemParaStore();
                        ChannelParaStore();

                        MSetSystem();
                        MEraseWindow(0, 0, 20*8, 24) ;
                        offset = C_OFF_PEAK;
                        MCopyProtectedMemory( (void*)offset, &GatePeak, C_LEN_PEAK, PM_COPY_WRITE);//门参数
                        offset = C_OFF_SAMP;
                        MCopyProtectedMemory( (void*)offset, echobuffer, C_LEN_SAMP, PM_COPY_WRITE); //以上为波形
                        MSetSaveFrequency(1, C_SETMODE_SETSAVE ) ;
                        DataSave();
                        number = 1;
                        break;
                    }
                    else
                    {
                        number = 0;
                        break;
                    }
                }
            }
        }
    }

    AllParaRestore();
    if( number == 1)
    {
        MSetFrequence(frequency,C_SETMODE_SETSAVE);
        SystemParaStore();
        ChannelParaStore();
    }
    /*
    	SystemParaRestore();
    	ChannelParaRestore();

    	MSetBaseGain(gain1,C_SETMODE_SETSAVE);
    	MSetCompGain(gain2,C_SETMODE_SETSAVE);
    	MSetSurfGain(gain3,C_SETMODE_SETSAVE);
    	MSetSaveFrequency(mode, C_SETMODE_SETSAVE ) ;
    	MSetFrequence(fre0,C_SETMODE_SETSAVE);
    	MSetScaleMode(scale,C_SETMODE_SETSAVE);
    	MSetDelay(delay0,scale);
    	MSetEchoMode(echomode,C_SETMODE_SETSAVE);
    	MSetBandMode(bandmode,C_SETMODE_SETSAVE);
    	MSetRange(range0,C_SETMODE_SETSAVE);
    	MSetGatePara(posi,wide,high,0,C_SETMODE_SAVE);
    	MSetFunctionMode(daccomp,C_DEPTH_COMPENSATE);
    	MSetProbeMode(probemode,C_SETMODE_SETSAVE);
    //*/
    SystemParaRestore();
    ChannelParaRestore();
    MSetSystem();

    return C_FALSE;
}

int TestMenuSensitivity(void)//灵敏度校验
{
    int xpos = ( C_HORIDOT_SCREEN - C_CCHAR_HDOT * 9 - C_ECHAR_HDOT * 10) / 8 * 8;
    int ypos = C_COORVPOSI;
    u_int para_xpos,para_ypos;
    u_int testrange[2];		//[0]反射体深度，[1]基准波高度，
    u_int offset;
    int deci_len, number = 1;

    int retvalue = -1;
    u_int row_height = 48;
    int keycode;

    ScreenRenovate();
    DisplayPrompt(15);
    MSetColor(C_CR_MENU);
//	CMenuOut(0,0,_TestMenu_A1+6*5+1,5,1,row_height);//提示：灵敏度校验

    TextOut(0,0,1,11,row_height,(u_char*)_TestMenu_A1[MGetLanguage()][5]+2,4);

    EraseDrawRectangle(xpos, ypos, xpos+ 14*C_CCHAR_HDOT, ypos + 3 * row_height) ;
    MSetColor(C_CR_MENU);
    CEMenuOut(xpos + C_ECHAR_HDOT,ypos+4,_TestMenu_AF1,7,3,row_height);	/*在指定位置根据每行字符数、行数、行高写菜单*/

    offset = C_OFF_TEMP + 50;
    MCopyProtectedMemory( testrange, (void*)offset, 8, PM_COPY_READ);
    testrange[1] = MGetAmpStdMax()/2;
    para_xpos = xpos + 7 * C_CCHAR_HDOT;	/*写参数，位置水平方向右移*/
    para_ypos = ypos + 4;

    while(retvalue == -1)
    {
        /*深度 */
        xpos = para_xpos;
        ypos = para_ypos + row_height *0 ;
        Write_Number(xpos,ypos+C_ECHAR_VDOT+1 ,testrange[0],4,1,0);
        EMenuOut(xpos + 4 * C_ECHAR_VDOT,ypos+8,"mm",2,1,8);

        /*高度*/
        ypos = para_ypos + row_height *1 ;
        Write_Number(xpos,ypos+C_ECHAR_VDOT+1 ,testrange[1],2,0,0);
        EMenuOut(xpos + 2 * C_ECHAR_VDOT,ypos+8,"% ",2,1,8);

        keycode = MenuKeyReturn(2,3);	/*出现菜单时，按不大于keymax的键返回，mode=0其它键无效，=1确认键有效，=2返回键有效，=3确认返回键有效*/
        ypos = para_ypos + row_height * (keycode - 1);

        switch (keycode)
        {
        case 1:
            deci_len = 1;
            number = 0;
            if ( Input_Number(xpos,ypos,&number,4, &deci_len,0) == 1)
                testrange[0] = number;
            break;
        case 2:
            deci_len = 0;
            number = 0;
            if ( Input_Number(xpos,ypos,&number,2, &deci_len,0) == 1)
                testrange[1] = number;
            break;
        case C_KEYCOD_RETURN:
            retvalue = 0;
            break;
        case C_KEYCOD_CONFIRM:
            retvalue = 1;
            offset = C_OFF_TEMP + 20;
            MCopyProtectedMemory( (void*)offset, testrange, 8, PM_COPY_WRITE);
            break;
        }
    }

    if(retvalue == 1)
    {
        while(1)
        {
            if (TestSensitivity(testrange[0],testrange[1], 0 ) == C_FALSE)return C_FALSE;
            if( DisplayQuery(12) == 1) return C_TRUE;
        }
    }
    return C_FALSE;
}
int TestSensitivity(u_int depth/*反射体深度*/, u_int stdamp/*基准波高度*/, int mode/*0可调增益，1不可调*/)
{

    int gain ,range;//, angle, speed;
    //u_int echotime;
    u_int gateamp;

    u_int char_len,row_number,crow_height;
    //int number,deci_len;
    int xpos,ypos;
    u_int preElapsedtime;
    int retvalue = C_TRUE;
    int keycode;
    int gatetype = 0;
    int diameter = 0;

    u_int testrange0 = depth * 10000 / MGetAngle(3) - diameter/2;

    MSetAcquisition(1);

    MKeyRlx();
    MSetScaleMode(0,0);

    if( mode == 0)gain = 600;
    else gain = MGetBaseGain();
    range = testrange0 * 100/80;		//放在第8格

    MSetPara(gain,0,0,0,range,MGetOffset(),0,MGetAngle(0),MGetSpeed() );

    MSetFunctionMode(0,C_FUNCTION_ALL);
    MSetEchoMode(0,C_SETMODE_SETSAVE);
    MSetGatePara(C_COORHORIUNIT * 7,C_COORHORIUNIT * 2,120,0,C_SETMODE_SETSAVE);
    MSetBGateMode(0,C_SETMODE_SETSAVE);
    MSetGatePara(C_COORHORIUNIT * 7.5,C_COORHORIUNIT * 1,0,1,C_SETMODE_SETSAVE);

    MSetSystem();
    MChannelRenovate();	//本通道屏幕参数刷表

    char_len = 7;
    row_number = 3;
    crow_height = 48-14*MGetLanguage();

    xpos = C_COORHORIUNIT * 6-(char_len+1)*C_CCHAR_HDOT-17;
    ypos = C_COORVPOSI + 1;
    MSetColor(C_CR_MENU);
    EraseDrawRectangle(xpos, ypos, C_COORHORIUNIT * 6, ypos + crow_height * row_number) ;
    TextOut(xpos + C_ECHAR_HDOT,ypos+4, row_number+2*MGetLanguage(), 17,crow_height-12*MGetLanguage(), (u_char*)_TestMenu_AB3[MGetLanguage()][2], 4);
    if (MGetLanguage())
    {
//        EMenuOut(xpos + 18*C_ECHAR_HDOT,ypos+4 * (crow_height-12*MGetLanguage()),_TestMenu_Enter,1,1,C_ECHAR_VDOT);
    }
    else
    {
//        EMenuOut(xpos + 9*C_ECHAR_HDOT,ypos+2 * crow_height,_TestMenu_Enter,1,1,C_ECHAR_VDOT);
    }
    WriteLongness(xpos + 11 * C_ECHAR_HDOT-8*MGetLanguage(), ypos  + 4+29*MGetLanguage() ,depth,4/*总长*/,0/*是否写单位*/);
    if( MGetUnitType() > 0)EMenuOut(xpos + 11 * C_ECHAR_HDOT+40*MGetLanguage(), ypos  + 12+20*MGetLanguage(),"in",2,1,8);
    else EMenuOut(xpos + 11 * C_ECHAR_HDOT+40*MGetLanguage(), ypos  + 12+20*MGetLanguage(),"mm",2,1,8);
///	Write_Number(xpos + 11 * C_ECHAR_HDOT,ypos  + 4 ,depth,4,1,0);

    GateMax.Amp = 0;
    xpos = 0;
    ypos = 0;
    ClearCursor(2);

    preElapsedtime = GetElapsedTime();	//开始测试时间

    ///MSetSystemMode(1,0);		//设置成自动调节状态

    xpos = 0;
    ypos = 0;
    ClearCursor(2);
    DisplayPrompt(15);
    MSetColor(C_CR_MENU);
    //CMenuOut(0,0,_TestMenu_A1+6*5+1,5,1,24);
    TextOut(0,0,1,11,24,(u_char*)_TestMenu_A1[MGetLanguage()][5]+2,4);
    GateMax.Amp = 0;	//最高波置初值为0

    while (1)
    {
        if( mode == 0)
        {
            if ( MAdjustGain(0,0,stdamp*2,stdamp*2) )
            {
                GateMax.Amp = 0;	//增益自动调节过
            }
        }
        MParaRenovate(1);
        gateamp = MGetAmpMax(gatetype);
        if ( MGetGateParaMax( gatetype) == C_TRUE )
        {
            ClearCursor(2);
            xpos = GateMax.Pos;
            ypos = C_COORVPOSI + C_COORHEIGHT - 2 - GateMax.Amp*2 ;
            if((GateMax.Amp*2) < C_COORHEIGHT)DrawCursor(xpos,ypos,2);
        }
        keycode = MGetKeyCode(0);

        if(keycode == C_KEYCOD_CONFIRM)
        {
            ClearCursor(2);
            MKeyRlx();

            MSetEquivalentDays(2,1);//测试天数,mode = 0仪器性能，1零点/K/dac/avg,2灵敏度
            SystemParaStore();
            ChannelParaStore();

            retvalue = C_TRUE;
            break;
        }
        else if(keycode == C_KEYCOD_RETURN)
        {
            if( DisplayQuery(4) == 1)
            {
                retvalue = C_FALSE;
//				SystemParaRestore();
//				ChannelParaRestore();

                break;
            }
        }
        else if(mode == 0 && keycode == C_KEYCOD_BASEGAIN && GetSameKeyTime() > 20)MAdjustGain(0,0,stdamp*2,stdamp*2);
        else if( keycode == C_KEYCOD_GATE || keycode == C_KEYCOD_BASEGAIN || keycode == C_KEYCOD_RANGE )
        {
            KeyManage(keycode,1);
            MKeyRlx();
            DisplayPrompt(15);
            MSetColor(C_CR_MENU);
            //	CMenuOut(0,0,_TestMenu_A1+6*5+1,5,1,24);
            TextOut(0,0,1,11,24,(u_char*)_TestMenu_A1[MGetLanguage()][5]+2,4);
        }
    }
    MKeyRlx();
    return retvalue;

}
int ManualMonthTest(void)//人工月性能校验
{

    u_int gain,range,offset,delay,speed;
    int retvalue = C_FALSE;
    u_int echotime;
//	int i,j;
    int keycode;
    u_int gateamp;

    u_int xpos,ypos;//,menu_xpos,menu_ypos;
    u_int char_len,row_number,crow_height;//,row_height;
    u_int gatetype = 0;

    u_short _Prompt_TB1[] =
    {
        CH(5039),CH(3887),CH(4852),CH(3660),CH(4254),CH(2215),CH(1866),CH(4252)
    };//	仪器性能手动测试
    MSetProbeMode(0,C_SETMODE_SETSAVE);

    gain = 300;
    range = 1250;
    offset = 0;
    delay = 0;
    speed = 5960;

    MSetPara(gain,0,0,2,range,offset,delay,0,speed);

    MSetEchoMode(0,C_SETMODE_SAVE);
    MSetPulseMode(1,C_SETMODE_SAVE);
    MSetBandMode(0,C_SETMODE_SAVE);
    MSetGatePara(C_COORHORIUNIT * 3.8,C_COORHORIUNIT * 1.2,120,0,C_SETMODE_SETSAVE);
    MSetGatePara(C_COORHORIUNIT * 4,C_COORHORIUNIT * 1.5,0,1,C_SETMODE_SETSAVE);

    MSetSystem();
    MSetAcquisition(1);	/*设置采样与否，0不采样1采样*/

    MChannelRenovate();	//本通道屏幕参数刷表

    char_len = 11;
    row_number = 2;
    crow_height = 48-14*MGetLanguage();

    xpos = C_COORWIDTH-(char_len+1+MGetLanguage()*3)*C_CCHAR_HDOT;
    ypos = C_COORVPOSI+1;
//	EraseDrawRectangle(xpos, ypos, 280, ypos+crow_height * row_number) ;
    MSetColor(C_CR_MENU);
//	CEMenuOut(xpos + C_ECHAR_HDOT,ypos + 4, _Menu_Power1 ,char_len,row_number,crow_height);

    /* if (MGetLanguage())
     {
         EraseDrawRectangle(C_COORWIDTH-(14+1)*C_CCHAR_HDOT, ypos, C_COORWIDTH, ypos+15 * 3) ;
         EMenuOut(C_COORWIDTH-(14+1)*C_CCHAR_HDOT + C_ECHAR_HDOT,ypos + 4, _Menu_Power1E ,28,3,15);
     }
     else
     {
         EraseDrawRectangle(xpos, ypos, C_COORWIDTH, ypos+24 * 2) ;
         CEMenuOut(xpos + C_ECHAR_HDOT,ypos + 4, _Menu_Power1 ,11,2,24);
     }*/
    EraseDrawRectangle(xpos, ypos, C_COORWIDTH, ypos+32 * (2+MGetLanguage())+MGetLanguage()*8) ;
    TextOut(xpos + C_ECHAR_HDOT,ypos + 8,2+MGetLanguage()*2,22,32-MGetLanguage()*6,(u_char*)_Menu_Power[MGetLanguage()][0],4);

    CEMenuOut(0,0, _Prompt_TB1 ,8,1,16);

    GateMax.Amp = 0;	//
    xpos = 0;
    ypos = 0;
    ClearCursor(2);
    while (1)
    {
        if (MAdjustGain(0,2,120,120) )
        {
            GateMax.Amp = 0;	//增益自动调节过
        }
        MParaRenovate(1);
        keycode = MGetKeyCode(10);
        gateamp = MGetAmpMax(gatetype);
        if ( MGetGateParaMax(gatetype) == C_TRUE )
        {
            ClearCursor(2);
            xpos = GateMax.Pos;
            ypos = C_COORVPOSI + C_COORHEIGHT - 2 - GateMax.Amp*2 ;
            if((GateMax.Amp*2) < C_COORHEIGHT)DrawCursor(xpos,ypos,2);
        }
        if(keycode == C_KEYCOD_CONFIRM)
        {
            ClearCursor(2);
            echotime = MGetAmpTime(gatetype);

            MGateDraw();
            MSetGatePara(C_COORHORIUNIT * 7.8,C_COORHORIUNIT * 1.5,120,0,C_SETMODE_SETSAVE);//门暂移到第8格
            MGateDraw();
            MKeyRlx();
            MAdjustGain(0,0,80,160);
            ExpendTime(10);
            offset = 2 * echotime - MGetAmpTime(gatetype);
            MSetOffset(offset,C_SETMODE_SETSAVE);
            speed = 500*C_SAMPLE_FREQ/(echotime - offset);
            MSetSpeed(speed,C_SETMODE_SETSAVE);

            range = 2500;
            MSetRange(range,C_SETMODE_SAVE);
            MSetSystem();
            MChannelRenovate();	//本通道屏幕参数刷表
            break;
        }
        else if(keycode == C_KEYCOD_RETURN)
        {
            if( DisplayQuery(4) == 1)
            {
                MSetAcquisition(0);
                return retvalue;
            }
        }
    }
    keycode = C_KEYCODMAX;
    MSetColor(C_CR_MENU);
    CEMenuOut(0,0, _Prompt_TB1 ,8,1,16);

    while(true)
    {
        if(keycode == C_KEYCOD_GATE || keycode == C_KEYCOD_RANGE || keycode == C_KEYCOD_BASEGAIN )
        {
            keycode = KeyManage(keycode,1);
        }
        else if( keycode == C_KEYCOD_RETURN)
        {
            if( DisplayQuery(4) == 1)
            {
                MSetAcquisition(0);
                break;
            }
        }
        else
        {
            keycode = MGetKeyCode(0);
            MSetColor(C_CR_MENU);
            CEMenuOut(0,0, _Prompt_TB1 ,8,1,16);
        }
        MParaRenovate(1);
    }
    return 1;
}

extern const u_int c_crPara[];
extern const u_int Echo_crPara[];

extern u_char crPara[];
int MenuMonthTest(void)//月性能校验
{
    POWER power;
    u_int table[] = {1000, 794, 631, 501, 398, 316, 251, 200, 158, 125, 100, 79, 63, 50, 39};

    int gain_temp;
    int number2 = 0;

    u_char tempcrPara[C_CR_MAX+12];
    u_int gain,range,offset,delay,speed;
    int retvalue = C_FALSE;
    u_int echotime;
    int number;//,deci_len;
    int i,j;
    int keycode;
    u_int gateamp;
    u_char *sampbuff;

    u_int xpos,ypos;//,menu_xpos,menu_ypos;
    u_int char_len,row_number,crow_height,row_height;
    u_int gatetype = 0;
    const char _Prompt_B[C_LANGUAGE][6][50] =
    {
        {
            "水平线性不合格，重测！  ",
            "垂直线性不合格，重测！  ",
            "分辨力不合格，重测！    ",
            "动态范围不合格，重测！  ",
            "灵敏度余量不合格，重测！",
            "查看前次测试结果？Ｙ／Ｎ",
        },
        {
            "Horizontal linearity     disqualification,redo!   ",
            "Vertical linearity       disqualification,redo!   ",
            "Resolution               disqualification,redo!   ",
            "Dynamic range            disqualification,redo!   ",
            "Surplus sensitivity      disqualification,redo!   ",
            "Detector performance     before? Y/N              ",
        }
    };

    AllParaStore();
    MSetUnitType(0);

    MSetFunctionMode(0,C_FUNCTION_ALL);
    MSetTestStatus(C_TEST_ALL,0);	//设置测试状态,DAC作
    MSetEchoMode(0,C_SETMODE_SETSAVE);
    MSetBandMode(0,C_SETMODE_SAVE);
    MSetBGateMode(0,C_SETMODE_SETSAVE);
    MSetReject(0,C_SETMODE_SETSAVE);
    MSetGatePara(MGetGatePara(1,0),MGetGatePara(1,1),0,1,C_SETMODE_SETSAVE);
    MSetDelay(0,C_SETMODE_SETSAVE);		//设置距离50.0mm的延时
    MSetStdMode(0,C_SETMODE_SAVE);
    offset = C_OFF_POWER;
    MCopyProtectedMemory(&power, (void*)offset, sizeof(POWER), PM_COPY_READ);
    MSetProbeMode(0,C_SETMODE_SETSAVE);
//	int elapsedtime = GetElapsedTime() + 1000;
    short begin = 0;
    retvalue = true;
    while (retvalue)
    {
        /* set the gate structure for peak maximum, gate_index 0 for gate 1 and 1 for gate 2 */
        if( begin == 0)
        {
            if( CMessageBox( 0, 0,(char *) _Prompt_B[MGetLanguage()][5],24,MB_YESNO) == MB_NO )retvalue = true;
            else retvalue = false;
            begin = 1;
        }
        //水平线性
        MSetPulseMode(0,C_SETMODE_SAVE);
        while(retvalue)
        {
            gain = 300;
            range = 1250;
            offset = 0;
            delay = 0;
            speed = 5920;


            MSetPara(gain,0,0,2,range,offset,delay,0,speed);

            MSetGatePara(C_COORHORIUNIT * 3.8,C_COORHORIUNIT * 1.5,120,0,C_SETMODE_SETSAVE);

            MSetSystem();
            MSetAcquisition(1);	/*设置采样与否，0不采样1采样*/

            MChannelRenovate();	//本通道屏幕参数刷表

            char_len = 11;
            row_number = 2;
            crow_height = 48-14*MGetLanguage();

            xpos = C_COORWIDTH-(char_len+1+MGetLanguage()*4)*C_CCHAR_HDOT;
            ypos = C_COORVPOSI+1;
            //	EraseDrawRectangle(xpos, ypos, C_COORWIDTH, ypos+crow_height * row_number) ;
            MSetColor(C_CR_MENU);
            //	CEMenuOut(xpos + C_ECHAR_HDOT,ypos + 4, _Menu_Power1 ,char_len,row_number,crow_height);
            /*   if (MGetLanguage())
               {
                   EraseDrawRectangle(C_COORWIDTH-(14+1)*C_CCHAR_HDOT, ypos, C_COORWIDTH, ypos+15 * 3) ;
                   EMenuOut(C_COORWIDTH-(14+1)*C_CCHAR_HDOT + C_ECHAR_HDOT,ypos + 4, _Menu_Power1E ,28,3,15);
               }
               else
               {
                   EraseDrawRectangle(xpos, ypos, C_COORWIDTH, ypos+24 * 2) ;
                   CEMenuOut(xpos + C_ECHAR_HDOT,ypos + 4, _Menu_Power1 ,11,2,24);
               }*/

            EraseDrawRectangle(xpos, ypos, C_COORWIDTH+18, ypos+32 * (2+MGetLanguage())+10+MGetLanguage()*8) ;
            TextOut(xpos + C_ECHAR_HDOT,ypos + 8,2+MGetLanguage()*2,22,32-MGetLanguage()*6,(u_char*)_Menu_Power[MGetLanguage()][0],4);

            MEraseWindow(0, 0, 24*C_ECHAR_HDOT,17 ) ;
            //	CMenuOut(0,0, _Power_prompt + 1 * 6, 6, 1, 24);
            TextOut(0,0,1,20,24,(char *)_Power_prompt[MGetLanguage()][0],4);

            GateMax.Amp = 0;	//
            xpos = 0;
            ypos = 0;
            ClearCursor(2);
            ExpendTime(50);
            while (1)
            {
                if(MAdjustGain(0,1,120,120))
                {
                    MSetGateParaInit();	//门内最高波设初值
                }
                keycode = MGetKeyCode(0);
                MParaRenovate(1);
                gateamp = MGetAmpMax(gatetype);
                if(MGetGateParaMax(gatetype) == C_TRUE)
                {
                    ClearCursor(2);
                    xpos = GateMax.Pos;
                    ypos = C_COORVPOSI + C_COORHEIGHT - 2 - GateMax.Amp*2 ;
                    if((GateMax.Amp*2) < C_COORHEIGHT)	DrawCursor(xpos,ypos,2);
                }

                if(keycode == C_KEYCOD_CONFIRM)
                {
                    MSetDelay((255 * C_SAMPLE_FREQ + MGetSpeed()/2 )/(int)MGetSpeed(),C_SETMODE_SETSAVE);
                    ClearCursor(2);
                    echotime = MGetAmpTime(gatetype);

                    MGateDraw();
                    MSetGatePara(C_COORHORIUNIT * 7.8,C_COORHORIUNIT * 1.5,120,0,C_SETMODE_SETSAVE);//门暂移到第8格
                    MGateDraw();
                    MKeyRlx();
                    MAdjustGain(0,0,160,160);
                    ExpendTime(50);
                    offset = 2 * echotime - MGetAmpTime(gatetype);
                    MSetOffset(offset,C_SETMODE_SETSAVE);
                    speed = 500*C_SAMPLE_FREQ/(echotime - offset);
                    MSetSpeed(speed,C_SETMODE_SETSAVE);

                    range = 1250;
                    MSetRange(range,C_SETMODE_SAVE);
                    MSetSystem();
                    MChannelRenovate();	//本通道屏幕参数刷表
                    MSetColor(C_CR_MENU);
                    //	CMenuOut(0,0, _Power_prompt + 1 * 6, 6, 1, 24);
                    MEraseWindow(0, 0,C_CCHAR_HDOT*10,C_CCHAR_VDOT ) ;
                    TextOut(0,0,1,20,24,(char *)_Power_prompt[MGetLanguage()][0],4);

                    power.Horiline = 0;
                    MParaRenovate(1);


                    for(i = 0; i < 4; i++)
                    {

                        MGateDraw();
                        MSetGatePara(C_COORHORIUNIT * (2*i+1.5),C_COORHORIUNIT * 1,100,0,C_SETMODE_SETSAVE);
                        MGateDraw();
                        //if( i == 4)MSetDelay(50,C_SETMODE_SETSAVE);		//最后一次波设置延时,避免过度靠后
                        for(j = 0 ; j < 10; j++)
                        {
                            ExpendTime(10);
                            MAdjustGain(0,0,100,120);
                            AdjustTime.amp_total = AdjustTime.amp_total2 = 0;
                            AdjustTime.time = AdjustTime.time2 = 0;
                            gateamp = MGetAmpMax(0) ;
                            if( gateamp < 105 && gateamp > 95)break;
                        }
                        if( i == 0 )power.gain[0] = MGetBaseGain();
                        MParaRenovate(1);

                        number = MGetAmpDist1(gatetype);
//Write_Number(10,30,number,4,0,0);
                        number = Mabsi(number - (i+1)*250);
                        if( number > 200)number -= 250;
//Write_Number(100,30,number,4,0,0);
                        //当前波与25的整数倍的差

                        if( number > 4)number -= 4;
                        if(number > power.Horiline)	power.Horiline = number;
                        ExpendTime(50);

                    }
                    break;

                }
                else if(keycode == C_KEYCOD_RETURN)
                {
                    if( DisplayQuery(4) == 1)
                    {
                        retvalue = false;
                        break;
                    }
                }
#if C_ACQU == 1
                if (GetElapsedTime() >= elapsedtime)
                {
                    elapsedtime = GetElapsedTime() + 1000;
                    MSetAcquisitionEnable(MGetAcquisitionEnable(),C_SETMODE_SETSAVE);
                }
#endif
            }
            MKeyRlx();
            if( power.Horiline > c_SystemPowerStd[0] )
            {
                if( CMessageBox( 0, 0,(char *) _Prompt_B[MGetLanguage()][0],24,MB_OKCANCEL) == MB_CANCEL )
                {
                    retvalue = true;
                    break;
                }
            }
            else break;
        }
        //分辨力
        //range = 850;
        //delay = range - 75*3;//将回波放在7.5格左右
        //MSetScaleDelay(delay,C_SETMODE_SETSAVE);		//设置距离50.0mm的延时

        //range = 100 * 3;
        MSetDelay(0,C_SETMODE_SETSAVE);		//设置距离0.0mm的延时
        MSetPulseMode(2,C_SETMODE_SAVE);
        while(retvalue)
        {
            range = 2500;
            MSetRange(range,C_SETMODE_SAVE);

            gain = 600;
            MSetBaseGain(gain,C_SETMODE_SAVE);
            MSetGatePara(C_COORHORIUNIT * 3,C_COORHORIUNIT*5/6,100,0,C_SETMODE_SETSAVE);
            MSetSystem();
            MChannelRenovate();	//本通道屏幕参数刷表
            char_len = 11;
            row_number = 3;
            crow_height = 48-14*MGetLanguage();

            xpos = C_COORWIDTH-(char_len+1+MGetLanguage()*3)*C_CCHAR_HDOT;
            ypos = C_COORVPOSI+1;
            //	EraseDrawRectangle(xpos, ypos, C_COORHORIUNIT * 7, ypos+crow_height * row_number) ;
            MSetColor(C_CR_MENU);
            /*   if (MGetLanguage())
               {
                   EraseDrawRectangle(xpos, ypos, C_COORWIDTH-6*8, ypos+12 * 3) ;
                   EMenuOut(xpos + C_ECHAR_HDOT,ypos + 4, _Menu_Power3E ,26,3,12);
               }
               else
               {
                   EraseDrawRectangle(xpos, ypos, C_COORWIDTH-9*8, ypos+24 * 3) ;
                   CEMenuOut(xpos + C_ECHAR_HDOT,ypos + 4, _Menu_Power3 ,11,3,24);
               }*/

            EraseDrawRectangle(xpos, ypos, C_COORWIDTH+18, ypos+32 * 3+10) ;
            TextOut(xpos + C_ECHAR_HDOT,ypos + 8,3,22,32-MGetLanguage()*6,(u_char*)_Menu_Power[MGetLanguage()][2],4);
            //	CEMenuOut(xpos + C_ECHAR_HDOT,ypos + 4, _Menu_Power3,char_len,row_number,crow_height);	/*在指定位置根据每行字符数、行数、行高写菜单*/
            //	CMenuOut(0,0, _Power_prompt + 3 * 6, 6, 1, 24);

            MEraseWindow(0, 0, 23*C_ECHAR_HDOT,17 ) ;
            TextOut(0,0,1,20,16,(char *)_Power_prompt[MGetLanguage()][2],4);


            GateMax.Amp = 0;	//增益自动调节过
            xpos = 0;
            ypos = 0;
            ClearCursor(2);
            //*
            while (retvalue)
            {
                if(MAdjustGain(0,0,100,100))
                {
                    MSetGateParaInit();	//门内最高波设初值
                }
                keycode = MGetKeyCode(0);
                MParaRenovate(1);
                gateamp = MGetAmpMax(gatetype);
                if(MGetGateParaMax(gatetype) == C_TRUE)
                {
                    ClearCursor(2);
                    xpos = GateMax.Pos;
                    ypos = C_COORVPOSI + C_COORHEIGHT - 2 - GateMax.Amp*2 ;
                    if((GateMax.Amp*2) < C_COORHEIGHT)	DrawCursor(xpos,ypos,2);
                }
                if(keycode == C_KEYCOD_CONFIRM)
                {
                    ClearCursor(2);
                    MKeyRlx();

                    MAdjustGain(0,0,200,200);
                    power.ResolvingPower = MGetSysGain();
                    power.gain[2] = MGetBaseGain();

                    sampbuff = GetSampleBuffer() ;
//					u_char temp;

                    MSetAcquisitionEnable(0,C_SETMODE_SETSAVE);
                    sampbuff = GetSampleBuffer() ;

                    for(i = C_COORWIDTH*85/100, gateamp = 200; i <= C_COORWIDTH*91/100 +1; i++)
                    {
                        if( ( *(sampbuff+i) ) < gateamp )
                        {
                            //找最低波
                            gateamp = ( *(sampbuff+i) );
                        }
                    }
                    // Write_Number(10,100,gateamp,4,0,0);
                    // MAnyKeyReturn();
                    MSetAcquisitionEnable(1,C_SETMODE_SETSAVE);
//Write_Number(10,30,gateamp,4,0,0);
//ExpendTime(200);
                    if( gateamp <= 1)power.ResolvingPower = 400;
                    else power.ResolvingPower = 200*log10(100.0/gateamp)+40;

                    // Write_Number(20,10,power.ResolvingPower,4,1,0);
                    // MAnyKeyReturn();

                    break;
                }
                else if(keycode == C_KEYCOD_RETURN)
                {
                    if( DisplayQuery(4) == 1)
                    {
                        retvalue = false;
                        break;
                    }
                }

#if C_ACQU == 1
                if (GetElapsedTime() >= elapsedtime)
                {
                    elapsedtime = GetElapsedTime() + 1000;
                    MSetAcquisitionEnable(MGetAcquisitionEnable(),C_SETMODE_SETSAVE);
                }
#endif
            }
            /*/
            while (1)
            {
            	if (MAdjustGain(0,0,100,100) )
            	{
            		GateMax.Amp = 0;	//增益自动调节过
            	}
            	MParaRenovate(1);
            	keycode = MGetKeyCode(2);
            	gateamp = MGetAmpMax(gatetype);
            	if ( MGetGateParaMax(gatetype) == C_TRUE )
            	{
            		ClearCursor(2);
            		xpos = GateMax.Pos;
            		ypos = C_COORVPOSI + C_COORHEIGHT - 2 - GateMax.Amp;
            		if(GateMax.Amp < C_COORHEIGHT)DrawCursor(xpos,ypos,2);
            	}
            	if(keycode == C_KEYCOD_CONFIRM)
            	{
            		ClearCursor(2);
            		MKeyRlx();

            		MAdjustGain(0,0,40,40);
            		power.ResolvingPower = MGetSysGain();
            		power.gain[2] = MGetBaseGain();

            		sampbuff = GetSampleBuffer() ;
            		u_char temp;

            		while(true)
            		{
            			BaseGainPlus(1);
            			WriteBaseGain(C_BASEGAIN_HPOSI,C_BASEGAIN_VPOSI);
            			ExpendTime(5);
            			sampbuff = GetSampleBuffer() ;

            			for(i = 280*85/100, gateamp = 200; i <= 280*91/100 ; i++)
            			{
            				if( ( *(sampbuff+i) ) < gateamp )
            				{//找最低波
            					gateamp = ( *(sampbuff+i) );
            				}
            			}
            			if( gateamp >= 100 || MGetSysGain() >= C_MAX_SYSGAIN)break;
            		}
            		power.ResolvingPower = MGetSysGain() -	power.ResolvingPower;

            		//Write_Number(20,10,power.ResolvingPower,4,1,0);
            		//MAnyKeyReturn();

            		break;
            	}
            	else if(keycode == C_KEYCOD_RETURN)
            	{
            		MSetAcquisition(0);
            		return retvalue;
            	}
            }
            //*/
            MKeyRlx();
            if(retvalue == false)break;

            if( power.ResolvingPower < c_SystemPowerStd[2] )
            {
                if( CMessageBox( 0, 0,(char *) _Prompt_B[MGetLanguage()][2],24,MB_OKCANCEL) == MB_CANCEL)
                {
                    retvalue = true;
                    break;
                }
            }
            else break;
        }


        //垂直线性、动态范围
        while(retvalue)
        {
            range = 2500;	//第8格
            MSetRange(range,C_SETMODE_SETSAVE);
            MSetGatePara((int)(C_COORHORIUNIT * 7.5),C_COORHORIUNIT * 1,100,0,C_SETMODE_SETSAVE);

            MChannelRenovate();	//本通道屏幕参数刷表
            power.vertline = 0;

            gain = 400;
            MSetBaseGain(gain,C_SETMODE_SETSAVE);
            MSetSystem();
            MChannelRenovate();	//本通道屏幕参数刷表

            GateMax.Amp = 0;	//增益自动调节过
            // set the gate structure for peak maximum, gate_index 0 for gate 1 and 1 for gate 2
            char_len = 11;
            row_number = 3;
            crow_height = 48-14*MGetLanguage();
            xpos = C_COORWIDTH-(char_len+2+MGetLanguage())*C_CCHAR_HDOT-10;
            ypos = C_COORVPOSI+1;
            //		EraseDrawRectangle(xpos, ypos, C_COORHORIUNIT * 7, ypos+crow_height * row_number) ;
            MSetColor(C_CR_MENU);
            /*  if (MGetLanguage())
              {
                  EraseDrawRectangle(xpos, ypos, C_COORWIDTH-7*8, ypos+12 * 3) ;
                  EMenuOut(xpos + C_ECHAR_HDOT,ypos + 4, _Menu_Power4E ,26,3,12);
              }
              else
              {
                  EraseDrawRectangle(xpos, ypos, C_COORWIDTH-10*8, ypos+24 * 3) ;
                  CEMenuOut(xpos + C_ECHAR_HDOT,ypos + 4, _Menu_Power4 ,11,3,24);
              }*/

            EraseDrawRectangle(xpos, ypos, C_COORWIDTH+18, ypos+32 * (3+MGetLanguage())+10) ;
            TextOut(xpos + C_ECHAR_HDOT,ypos + 8,3+MGetLanguage(),21,32-MGetLanguage()*6,(u_char*)_Menu_Power[MGetLanguage()][3],4);

//			CEMenuOut(xpos + C_ECHAR_HDOT,ypos + 4, _Menu_Power4 ,char_len,row_number,crow_height);
            //	CMenuOut(0, 0, _Power_prompt + 2 * 6, 6, 1, 24);
            MEraseWindow(0, 0, 23*C_ECHAR_HDOT,17 ) ;
            TextOut(0,0,1,20,16,(char *)_Power_prompt[MGetLanguage()][1],4);
            MKeyRlx();

            xpos = 0;
            ypos = 0;
            ClearCursor(2);
            while (retvalue)
            {
                if(MAdjustGain(0,0,100,100))
                {
                    MSetGateParaInit();	//门内最高波设初值
                }
                keycode = MGetKeyCode(0);
                MParaRenovate(1);
                gateamp = MGetAmpMax(gatetype);
                if(MGetGateParaMax(gatetype) == C_TRUE)
                {
                    ClearCursor(2);
                    xpos = GateMax.Pos;
                    ypos = C_COORVPOSI + C_COORHEIGHT - 2 - GateMax.Amp*2 ;
                    if((GateMax.Amp*2) < C_COORHEIGHT)	DrawCursor(xpos,ypos,2);
                }
                if(keycode == C_KEYCOD_CONFIRM)
                {
                    ClearCursor(2);
                    MKeyRlx();
                    MSetBaseGain(MGetBaseGain()+ 60,C_SETMODE_SETSAVE);

                    while(MGetAmpMax(0) < 200)
                    {
                        MSetBaseGain( MGetBaseGain() + 1 , C_SETMODE_SETSAVE);
                        AdjustTime.amp_total = AdjustTime.amp_total2 = 0;
                        AdjustTime.time = AdjustTime.time2 = 0;
                        ExpendTime(20);
                    }
                    while(MGetAmpMax(0) > 202)
                    {
                        MSetBaseGain( MGetBaseGain() - 1 , C_SETMODE_SETSAVE);
                        AdjustTime.amp_total = AdjustTime.amp_total2 = 0;
                        AdjustTime.time = AdjustTime.time2 = 0;
                        ExpendTime(20);
                    }

                    gain_temp = MGetBaseGain();
                    power.gain[4] = power.gain[1] = MGetBaseGain();

                    i = 0;
                    number2 = 0;
                    power.vertline = 0;
                    MParaRenovate(1);
                    gateamp = 0 ;
                    for( i = 0; i < 20 && MGetSysGain() > 0; i++)
                    {
                        for( number = 0 ; number < 20; number++)
                        {
                            ExpendTime(10);
                            MParaRenovate(0);
                            if( Mabsi(gateamp - MGetAmpMax(0) ) < 2 )
                            {
                                break;
                            }
                            gateamp = MGetAmpMax(0);
                        }
                        if( gateamp > 0)power.Dynamic = MGetBaseGain();
                        number = gateamp*5 - table[i];
                        if(i < 13)
                        {
                            //计算垂直线性
                            if (number >= 0)
                            {
                                if(number > power.vertline) power.vertline = number;
                            }
                            else
                            {
                                if( number < number2) number2 = number;
                            }
                            MSetBaseGain( MGetBaseGain() - 20 , C_SETMODE_SETSAVE);
                        }
                        else
                        {
                            if( gateamp <= 1)break;
                            MSetBaseGain( MGetBaseGain() - 5 , C_SETMODE_SETSAVE);
                        }

//Write_Number(200,30,number,4,0,-1);
//Write_Number(200,40,power.vertline,4,0,-1);
//Write_Number(200,50,number2,4,0,-1);
                        WriteBaseGain(C_BASEGAIN_HPOSI,C_BASEGAIN_VPOSI);
                        AdjustTime.amp_total = AdjustTime.amp_total2 = 0;
                        AdjustTime.time = AdjustTime.time2 = 0;
                        ExpendTime(50);
                    }

                    power.vertline -= number2;//减去负值
                    MSetColor(C_CR_MENU);
                    //	CMenuOut(0,0, _Power_prompt + 4 * 6, 6, 1, 24);
                    MEraseWindow(0, 0,C_CCHAR_HDOT*8,C_CCHAR_VDOT ) ;
                    TextOut(0,0,1,20,16,(char *)_Power_prompt[MGetLanguage()][3],4);

                    power.Dynamic = gain_temp - power.Dynamic + 10;
                    MSetBaseGain( gain_temp - 60, C_SETMODE_SETSAVE);
                    //Write_Number(20,10,power.Dynamic,4,1,0);
                    //MAnyKeyReturn();
                    //MKeyRlx();

                    break;
                }
                else if(keycode == C_KEYCOD_RETURN)
                {
                    if( DisplayQuery(4) == 1)
                    {
                        retvalue = false;
                        break;
                    }
                }
#if C_ACQU == 1

                if (GetElapsedTime() >= elapsedtime)
                {
                    elapsedtime = GetElapsedTime() + 1000;
                    MSetAcquisitionEnable(MGetAcquisitionEnable(),C_SETMODE_SETSAVE);
                }
#endif
            }
            MKeyRlx();
            if(retvalue == false)break;
            if( power.vertline > c_SystemPowerStd[1] )
            {
                if( CMessageBox( 0, 0, (char *)_Prompt_B[MGetLanguage()][1],24,MB_OKCANCEL) == MB_CANCEL)
                {
                    retvalue = true;
                    break;
                }
            }
            else if( power.Dynamic < c_SystemPowerStd[3] )
            {
                if( CMessageBox( 0, 0,(char *) _Prompt_B[MGetLanguage()][3],24,MB_OKCANCEL) == MB_CANCEL)
                {
                    retvalue = true;
                    break;
                }
            }
            else break;
        }


        while(retvalue)
        {
            //灵敏度余量
            range = 2000 *100/80;	//第8格
            MSetRange(range,C_SETMODE_SETSAVE);
            MSetGatePara((int)(C_COORHORIUNIT * 7.5),C_COORHORIUNIT * 1,100,0,C_SETMODE_SETSAVE);

            MChannelRenovate();	//本通道屏幕参数刷表


            MKeyRlx();

            MSetSystem();
            MChannelRenovate();	//本通道屏幕参数刷表

            GateMax.Amp = 0;	//增益自动调节过

            char_len = 11;
            row_number = 3;
            crow_height = 48-14*MGetLanguage();
            xpos = C_COORWIDTH-(char_len+2+MGetLanguage()*2)*C_CCHAR_HDOT-10;
            ypos = C_COORVPOSI+1;
            //	EraseDrawRectangle(xpos, ypos, C_COORHORIUNIT * 7, ypos+crow_height * row_number) ;
            MSetColor(C_CR_MENU);
            /* if (MGetLanguage())
             {
                 EraseDrawRectangle(xpos, ypos, C_COORWIDTH-7*8, ypos+12 * 3) ;
                 EMenuOut(xpos + C_ECHAR_HDOT,ypos + 4, _Menu_Power4E ,26,3,12);
             }
             else
             {
                 EraseDrawRectangle(xpos, ypos, C_COORWIDTH-9*8, ypos+24 * 3) ;
                 CEMenuOut(xpos + C_ECHAR_HDOT,ypos + 4, _Menu_Power4 ,11,3,24);
             }*/

            EraseDrawRectangle(xpos, ypos, C_COORWIDTH+18, ypos+32 * (3+MGetLanguage())+10) ;
            TextOut(xpos + C_ECHAR_HDOT,ypos + 8,3+MGetLanguage(),21,32-MGetLanguage()*6,(u_char*)_Menu_Power[MGetLanguage()][3],4);
            //	CEMenuOut(xpos + C_ECHAR_HDOT,ypos + 4, _Menu_Power4 ,char_len,row_number,crow_height);
            MSetColor(C_CR_MENU);
            //	CMenuOut(0,0, _Power_prompt + 5 * 6, 6, 1, 24);
            MEraseWindow(0, 0, 23*C_ECHAR_HDOT,17 ) ;
            TextOut(0,0,1,20,16,(char *)_Power_prompt[MGetLanguage()][4],4);


            MKeyRlx();

            xpos = 0;
            ypos = 0;
            ClearCursor(2);
            while (retvalue)
            {
                if(MAdjustGain(0,0,100,100))
                {
                    MSetGateParaInit();	//门内最高波设初值
                }
                keycode = MGetKeyCode(0);
                MParaRenovate(1);
                gateamp = MGetAmpMax(gatetype);
                if(MGetGateParaMax(gatetype) == C_TRUE)
                {
                    ClearCursor(2);
                    xpos = GateMax.Pos;
                    ypos = C_COORVPOSI + C_COORHEIGHT - 2 - GateMax.Amp*2 ;
                    if((GateMax.Amp*2) < C_COORHEIGHT)	DrawCursor(xpos,ypos,2);
                }
                if(keycode == C_KEYCOD_CONFIRM)
                {
                    ClearCursor(2);
                    MKeyRlx();
                    MAdjustGain(0,0,100,100);
                    power.Sensitivity = MGetBaseGain();
                    power.gain[3] = MGetBaseGain();
                    MSetBaseGain(MGetBaseGain(), C_SETMODE_SETSAVE);
                    break;
                }
                else if(keycode == C_KEYCOD_RETURN)
                {
                    if( DisplayQuery(4) == 1)
                    {
                        retvalue = false;
                        break;
                    }
                }
#if C_ACQU == 1
                if (GetElapsedTime() >= elapsedtime)
                {
                    elapsedtime = GetElapsedTime() + 1000;
                    MSetAcquisitionEnable(MGetAcquisitionEnable(),C_SETMODE_SETSAVE);
                }
#endif
            }
            xpos = 0;
            ypos = C_COORVPOSI;

            range = 40000 *100/80;	//第8格
            MSetRange(range,C_SETMODE_SETSAVE);
            MChannelRenovate();
            //	EraseDrawRectangle(xpos + 2, ypos, xpos+20*C_ECHAR_HDOT, ypos+24) ;
            MSetColor(C_CR_MENU);
            /*  if (MGetLanguage())
              {
                  EraseDrawRectangle(xpos + 2, ypos, xpos+34*C_ECHAR_HDOT, ypos+18) ;
                  EMenuOut(xpos + C_ECHAR_HDOT,ypos + 4, _Menu_Power5E ,33,1,12);
              }
              else
              {
                  EraseDrawRectangle(xpos + 2, ypos, xpos+20*C_ECHAR_HDOT, ypos+24) ;
                  CEMenuOut(xpos + 2 + C_ECHAR_HDOT,ypos + 4, _Menu_Power5 ,9,1,crow_height);
              }*/

            EraseDrawRectangle(xpos+2, ypos, xpos+(20+MGetLanguage()*11)*C_ECHAR_HDOT, ypos+32+10) ;
            TextOut(xpos+2 + C_ECHAR_HDOT,ypos + 8,1,18+MGetLanguage()*11,32-MGetLanguage()*6,(u_char*)_Menu_Power[MGetLanguage()][4],4);
//			CEMenuOut(xpos + 2 + C_ECHAR_HDOT,ypos + 4, _Menu_Power5 ,9,1,crow_height);
            MSetColor(C_CR_MENU);
            //	CMenuOut(0,0, _Power_prompt + 5 * 6, 6, 1, 24);
            MEraseWindow(0, 0,C_CCHAR_HDOT*8,C_CCHAR_VDOT ) ;
            TextOut(0,0,1,20,16,(char *)_Power_prompt[MGetLanguage()][4],4);

            while(1)
            {
                if(MGetKeyCode(2) == C_KEYCOD_CONFIRM)
                {
                    int i = 0;
                    do
                    {
                        BaseGainPlus(2);
                        WriteBaseGain(C_BASEGAIN_HPOSI,C_BASEGAIN_VPOSI);
                        ExpendTime(5);
                        if( MGetAmpMax(gatetype) > 20 )i++;
                        else i = 0;
                    }
                    while( MGetBaseGain() < C_MAX_SYSGAIN && i < 3 );

                    power.Sensitivity = MGetBaseGain() - power.Sensitivity;

                    break;
                }
                else if(MGetKeyCode(2) == C_KEYCOD_RETURN)
                {
                    if( DisplayQuery(4) == 1)
                    {
                        retvalue = false;
                        break;
                    }
                }

            }
            MKeyRlx();
            if(retvalue == false)break;
            if( power.Sensitivity < c_SystemPowerStd[4] )
            {
                if( CMessageBox( 0, 0, (char *)_Prompt_B[MGetLanguage()][4],24,MB_OKCANCEL) == MB_CANCEL)
                {
                    retvalue = true;
                    break;
                }
            }
            else break;
        }
        for (i=0; i<C_CR_MAX+12; i++)
        {
            tempcrPara[i]=crPara[i];
        }
        while(true)
        {

            MSaveDefualtColor(1);
            MSetAcquisition(0);
            power.test = 1;

            offset =  C_OFF_POWER;
            MCopyProtectedMemory((void*)offset, &power, sizeof(POWER), PM_COPY_WRITE);

            //以下显示测试结果
            MFclearScreen();

            xpos = 40;
            ypos = 1;
            char_len = 6;
            row_number = 6;
            row_height = 48;

            MSetColor(C_CR_MENU);
            EraseDrawRectangle(xpos, ypos, xpos+10*C_CCHAR_HDOT + C_ECHAR_VDOT+9*C_ECHAR_VDOT*MGetLanguage(), ypos+row_height*6) ;

//			EraseDrawRectangle(xpos, ypos, xpos+9*C_CCHAR_HDOT + C_ECHAR_VDOT, ypos+row_height*6) ;

            //	CMenuOut(xpos+4, ypos+4, _Power_prompt, char_len, row_number, row_height);

            TextOut(xpos+4,ypos+4,1,14+MGetLanguage()*6,row_height,(char *)_Power_prompt[MGetLanguage()][5],4);
            TextOut(xpos+4,ypos+4+row_height,1,14+MGetLanguage()*6,row_height,(char *)_Power_prompt[MGetLanguage()][0],4);
            TextOut(xpos+4,ypos+4+row_height*2,1,14+MGetLanguage()*6,row_height,(char *)_Power_prompt[MGetLanguage()][1],4);
            TextOut(xpos+4,ypos+4+row_height*3,1,14+MGetLanguage()*6,row_height,(char *)_Power_prompt[MGetLanguage()][2],4);
            TextOut(xpos+4,ypos+4+row_height*4,1,14+MGetLanguage()*6,row_height,(char *)_Power_prompt[MGetLanguage()][3],4);
            TextOut(xpos+4,ypos+4+row_height*5,1,14+MGetLanguage()*6,row_height,(char *)_Power_prompt[MGetLanguage()][4],4);

            xpos = 40;
            ypos = ypos+row_height*6+12;
            char_len = 3;
            row_number = 2;
            DrawRectangle(xpos, ypos, xpos+(6+MGetLanguage())*C_CCHAR_HDOT, ypos+row_height*row_number);
            MSetColor(C_CR_MENU);

            //  if(MGetLanguage()==1)	//English
            {
                //CMenuOut(xpos+10, ypos+4, _Power_prompt3, 1, row_number, row_height);
                //       EMenuOut(xpos+10+16, ypos+12,"1. Print",8,1,8);
                //       EMenuOut(xpos+10+16, ypos+12+row_height,"# Redo ",8,1,8);
                //       EMenuOut(xpos+10+16, ypos+12+row_height*2,"ぁ Exit ",8,1,8);
            }
            //   else
            {
                TextOut(xpos+10,ypos+4,row_number,8,row_height,(char *)_Power_prompt[MGetLanguage()][6],4);
//                EMenuOut(xpos+10,ypos+4+2*row_height,_TestMenu_Enter,1,1,C_ECHAR_VDOT);

                // CMenuOut(xpos+10, ypos+4, _Power_prompt2, char_len, row_number, row_height);
            }

            xpos = 180+14*C_ECHAR_HDOT*MGetLanguage();
            ypos = 8;
            ypos += row_height;
            Write_Number(xpos,ypos,power.Horiline,4,1,0);
            Write_Ascii(xpos+2*C_CCHAR_HDOT, ypos,'%');

            ypos += row_height;
            Write_Number(xpos,ypos,power.vertline,4,1,0);
            Write_Ascii(xpos+2*C_CCHAR_HDOT, ypos,'%');

            ypos += row_height;
            Write_Number(xpos,ypos,power.ResolvingPower,4,1,0);
            EMenuOut(xpos + 2*C_CCHAR_HDOT+16,ypos,"dB",2,1,8);

            ypos += row_height;
            Write_Number(xpos,ypos,power.Dynamic,4,1,0);
            EMenuOut(xpos + 2*C_CCHAR_HDOT+16,ypos,"dB",2,1,8);

            ypos += row_height;
            Write_Number(xpos,ypos,power.Sensitivity,4,1,0);
            EMenuOut(xpos + 2*C_CCHAR_HDOT+16,ypos,"dB",2,1,8);

            do
            {
                keycode = MAnyKeyReturn();
            }
            while( keycode != 1 && keycode != C_KEYCOD_CONFIRM && keycode!= C_KEYCOD_RETURN);

            if (keycode == 1)
            {
                //	retvalue = MGetColorMode();
                if( TestPrinterReady() != 0)
                {
                    //	MSaveDefualtColor(0);
                    PrinterInitialize();		//打印机初始化
                    xpos = 0;
                    ypos = 0;

                    for(i = 0; i< 156/12 && ypos <= 156; i++)
                    {
                        ScrcpyInit(40);
                        ScrcpyInvert(xpos,ypos,40);
                        ypos += 12;
                    }
                    PrintEnd();
                    //	MSaveDefualtColor(retvalue);
                }
            }
            else if (keycode == C_KEYCOD_RETURN)
            {
                offset = C_OFF_TEMP+60;	//颜色存储位置
                MCopyProtectedMemory( (void*)offset, tempcrPara, C_CR_MAX+12, PM_COPY_WRITE);
                MCopyProtectedMemory( crPara, (void*)offset, C_CR_MAX+12, PM_COPY_READ);
                MSetColor(C_CR_DEFAULT);
                SetBackgroundColor(c_crPara[crPara[ C_CR_BACK] ]);
                MFclearScreen();
                MKeyRlx();
                retvalue = false;
                break;
            }
            else if (keycode == C_KEYCOD_CONFIRM)
            {
                offset = C_OFF_TEMP+60;	//颜色存储位置
                MCopyProtectedMemory( (void*)offset, tempcrPara, C_CR_MAX+12, PM_COPY_WRITE);
                MCopyProtectedMemory( crPara, (void*)offset, C_CR_MAX+12, PM_COPY_READ);
                MSetColor(C_CR_DEFAULT);
                SetBackgroundColor(c_crPara[crPara[ C_CR_BACK] ]);
                MSetEquivalentDays(0,1);//测试天数,mode = 0仪器性能，1零点/K/dac/avg,2灵敏度
                retvalue = true;
                break;
            }
        }
        if( retvalue )break;
        retvalue = true;
    }
    AllParaRestore();
    return retvalue;
}



int WeldSetup(void)
{
    u_int char_len = 6,row_number = 9,crow_height = 44;
    int number,deci_len;
    int xpos,ypos;
    //int x1,y1,x2,y2,r;
    //double rate;//时间长度单位mm跟屏幕点数的比率
    int menu_xpos = 0,menu_ypos = 0;
    int para_xpos = C_CCHAR_HDOT * ( char_len + 1) ,para_ypos = crow_height + 4;
    //int weld_xpos,weld_ypos;
    int retvalue = C_TRUE;
    int keycode;
    int page_max = 2, page = 0;
    int row;
    //unsigned int offset;
    u_short* cpText = 0;
    //u_char* epText = 0;
    int i;
    //u_int elapsedtime1 = GetElapsedTime() ;//,elapsedtime2;



    MSetAcquisitionEnable(0,C_SETMODE_SETSAVE);
    MSetHardEchoShow(0,C_SETMODE_SETSAVE);


    while(1)
    {
        MFclearScreen();
        MSetColor(C_CR_MENU);

        if( page < 1)page = 1;
        else if(page > page_max )page = page_max;

        TextOut(C_ECHAR_HDOT,4,1,25,16,(u_char*)_Weld_A1[MGetLanguage()][0],4);
        //	CEMenuOut(C_ECHAR_HDOT,4, _Weld_A1,12,1,crow_height);
        Write_Number(C_ECHAR_HDOT + (10+MGetLanguage()*2) * C_ECHAR_HDOT, 4+3*MGetLanguage(),page,1,0,0);
        if (MGetLanguage())
        {
            Write_Number(C_ECHAR_HDOT + 25 * C_ECHAR_HDOT+4, 7,page_max,1,0,0);
        }
        else
        {
            Write_Number(C_ECHAR_HDOT + 17 * C_ECHAR_HDOT, 4,page_max,1,0,0);
        }


        menu_xpos = 0;
        menu_ypos = crow_height-4;
        para_xpos = menu_xpos+6*C_CCHAR_HDOT;
        para_ypos = menu_ypos + 4;
        //	CEMenuOut(menu_xpos+C_ECHAR_HDOT,para_ypos, cpText ,char_len,row_number,crow_height);	//在指定位置根据每行字符数、行数、行高写菜单

        if(page == 1)
        {
            row_number = 9;
            if( stWeldPara.type == 3)//T型
            {
                for (i=0; i<row_number; i++)
                {
                    TextOut(menu_xpos+C_ECHAR_HDOT,para_ypos+crow_height*i,1,22,12+12*MGetLanguage(),(u_char*)_Weld_B1[MGetLanguage()][i],4);
                }
            }
            else
            {
                for (i=0; i<row_number; i++)
                {

                    TextOut(menu_xpos+C_ECHAR_HDOT,para_ypos+crow_height*i,1,22,12+12*MGetLanguage(),(u_char*)_Weld_B2[MGetLanguage()][i],4);
                }
            }
        }
        else if (page == 2)
        {
            row_number = 4;     //7
            if( stWeldPara.type == 3)//T型
            {
                for (i=0; i<row_number; i++)
                {
                    TextOut(menu_xpos+C_ECHAR_HDOT,para_ypos+crow_height*i,1,22,12+12*MGetLanguage(),(u_char*)_Weld_B12[MGetLanguage()][i],4);
                }
            }
            else
            {
                for (i=0; i<row_number; i++)
                {
                    TextOut(menu_xpos+C_ECHAR_HDOT,para_ypos+crow_height*i,1,22,12+12*MGetLanguage(),(u_char*)_Weld_B22[MGetLanguage()][i],4);

                }
            }
        }

        MSetColor(C_CR_PARA);
        if( stWeldPara.type > 0 )
        {
            if( stWeldPara.thick1 < 10)
            {
                stWeldPara.face = 0;
                stWeldPara.thick1 = 200;
                stWeldPara.wide = 250;
                stWeldPara.thick2 = 200;
                stWeldPara.high1 = 30;
                stWeldPara.high2 = 20;
                stWeldPara.radius = 0;
                stWeldPara.angle1 = 600;
                stWeldPara.angle2 = 300;
                stWeldPara.deep = 200;
                stWeldPara.blunt = 0;
                stWeldPara.space = 20;
                stWeldPara.probe = 200;

                stWeldPara.angle3=0;	//坡内角度，< 180;
                stWeldPara.angle4=0;	//坡内角度，< 180;
                stWeldPara.deep3=0;	//坡内深度，< 100mm;
            }
        }
        if( stWeldPara.type < 3 && stWeldPara.face > 1 )
        {
            stWeldPara.face = 0;
        }

        if( stWeldPara.type == 3  )
        {
            if( stWeldPara.thick2 < stWeldPara.deep + stWeldPara.blunt)
                stWeldPara.blunt = stWeldPara.thick2 - stWeldPara.deep;
        }
        else
        {
            //钝边加坡口深大于板厚
            if( stWeldPara.thick1 < stWeldPara.deep)stWeldPara.deep = stWeldPara.thick1;
            if( stWeldPara.thick1 < stWeldPara.deep + stWeldPara.blunt )
            {
                if( stWeldPara.type == 1 )
                {
                    stWeldPara.blunt = stWeldPara.thick1 - stWeldPara.deep;
                }
                else stWeldPara.blunt = 0;
            }
        }

        if( stWeldPara.type < 3 && stWeldPara.thick2 > stWeldPara.thick1  )
        {
            //单面/双
            stWeldPara.thick2 = stWeldPara.thick1 ;
        }
        if( stWeldPara.type < 3 )
        {
            //单面/双
            if( stWeldPara.angle1 > 1500  )	stWeldPara.angle1 = 1500;
            if( stWeldPara.angle1 < stWeldPara.angle2 )stWeldPara.angle2 = stWeldPara.angle1/2;
            if( stWeldPara.angle3 > 1500  )	stWeldPara.angle3 = 1500;
            if( stWeldPara.angle3 < stWeldPara.angle4 )stWeldPara.angle4 = stWeldPara.angle3/2;

            ///	if( stWeldPara.thick2 < stWeldPara.deep )stWeldPara.thick2 = stWeldPara.deep ;
        }
        else if(stWeldPara.angle1 > 800  )
        {
            stWeldPara.angle1 = 800 ;
        }
        if( stWeldPara.type == 3 && stWeldPara.angle2 > 800  )
        {
            stWeldPara.angle1 = 800;
        }

        if(MGetLanguage())
        {
            para_ypos+=C_ECHAR_VDOT-1;
        }
        switch(page)
        {
        case 1://第一页
            //焊缝类型
            row = 0;
            xpos = para_xpos;
            ypos = para_ypos + crow_height *row;
            //	cpText = (u_short*)_Weld_C1 + stWeldPara.type * 3 + 1;
            //	CEMenuOut(xpos,ypos , cpText ,2,1,crow_height);	//在指定位置根据每行字符数、行数、行高写菜单
            TextOut(xpos,ypos,1,8,16,(u_char*)_Weld_C1[MGetLanguage()][stWeldPara.type]+2,4);


            //探测面
            row = 1;
            xpos = para_xpos;
            ypos = para_ypos + crow_height *row;
            //	cpText = (u_short*)_Weld_D1 + stWeldPara.face * 3 + 1;
            //	CEMenuOut(xpos,ypos , cpText ,2,1,crow_height);	//在指定位置根据每行字符数、行数、行高写菜单
            TextOut(xpos,ypos,1,2,16,(u_char*)_Weld_D1[MGetLanguage()][stWeldPara.face]+2+8*MGetLanguage(),4);

            //工件厚度
            row = 2;
            xpos = para_xpos;
            ypos = para_ypos + crow_height *row;
            //Write_Number(xpos,ypos,stWeldPara.thick1 ,4,1,0);
            WriteLongness(xpos,ypos,stWeldPara.thick1,4,1);

            //有效厚度
            row = 3;
            xpos = para_xpos;
            ypos = para_ypos + crow_height *row;
//			Write_Number(xpos,ypos,stWeldPara.thick2 ,4,1,0);
            WriteLongness(xpos,ypos,stWeldPara.thick2,4,1);

            //焊缝宽度
            row = 4;
            xpos = para_xpos;
            ypos = para_ypos + crow_height *row;
//			Write_Number(xpos,ypos,stWeldPara.wide  ,4,1,0);
            WriteLongness(xpos,ypos,stWeldPara.wide,4,1);

            //根部半径
            row = 5;
            xpos = para_xpos;
            ypos = para_ypos + crow_height *row;
//			Write_Number(xpos,ypos,stWeldPara.radius  ,4,1,0);
            WriteLongness(xpos,ypos,stWeldPara.radius,4,1);


            //上端余高
            row = 6;
            xpos = para_xpos;
            ypos = para_ypos + crow_height *row;
//			Write_Number(xpos,ypos,stWeldPara.high1  ,4,1,0);
            WriteLongness(xpos,ypos,stWeldPara.high1,4,1);

            //下端余高
            row = 7;
            xpos = para_xpos;
            ypos = para_ypos + crow_height *row;
//			Write_Number(xpos,ypos,stWeldPara.high2 ,4,1,0);
            WriteLongness(xpos,ypos,stWeldPara.high2,4,1);

            //坡口角度
            row = 8;
            xpos = para_xpos;
            ypos = para_ypos + crow_height *row;
            Write_Number(xpos,ypos,stWeldPara.angle1 ,4,1,0);

            break;
        case 2://第二页
            //坡口面角
            row = 0;
            xpos = para_xpos;
            ypos = para_ypos + crow_height *row;
            Write_Number(xpos,ypos,stWeldPara.angle2 ,4,1,0);

            //坡口深度
            row = 1;
            xpos = para_xpos;
            ypos = para_ypos + crow_height *row;
//			Write_Number(xpos,ypos,stWeldPara.deep  ,4,1,0);
            WriteLongness(xpos,ypos,stWeldPara.deep,4,1);

            //钝边高度
            row = 2;
            xpos = para_xpos;
            ypos = para_ypos + crow_height *row;
//			Write_Number(xpos,ypos,stWeldPara.blunt  ,4,1,0);
            WriteLongness(xpos,ypos,stWeldPara.blunt,4,1);

            //根部间隙
            row = 3;
            xpos = para_xpos;
            ypos = para_ypos + crow_height *row;
//			Write_Number(xpos,ypos,stWeldPara.space  ,4,1,0);
            WriteLongness(xpos,ypos,stWeldPara.space,4,1);

            //坡口角度
            row = 4;
            if(row>(row_number-1))break;
            xpos = para_xpos;
            ypos = para_ypos + crow_height *row;
            Write_Number(xpos,ypos,stWeldPara.angle3 ,4,1,0);

            //坡口角度
            row = 5;
            if(row>(row_number-1))break;
            xpos = para_xpos;
            ypos = para_ypos + crow_height *row;
            Write_Number(xpos,ypos,stWeldPara.angle4 ,4,1,0);

            //坡口角度
            row = 6;
            if(row>(row_number-1))break;
            xpos = para_xpos;
            ypos = para_ypos + crow_height *row;
            WriteLongness(xpos,ypos,stWeldPara.deep3,4,1);

            break;
        }
        //画焊缝

        stWeldPara.probe = 0;
        WELD_DRAW( &stWeldPara,300, 600, 120, 250,MGetAngle(0), 0);

        MKeyRlx();		//只有按键已释放才退出
        do
        {
            ///		if( GetElapsedTime() > elapsedtime1 + 200 )
            ///		{
            ///			InvertWords(27*C_ECHAR_HDOT,190+4, 2); //反白显示几个字的区域
            ///			elapsedtime1 = GetElapsedTime() ;
            ///		}
            keycode = MGetKeyCode(10);
#if C_DEVTYPE == 14
            if ( (keycode > 0 && keycode <= row_number)
                    || ( keycode == C_KEYCOD_PLUS || keycode == C_KEYCOD_MINUS || keycode == C_KEYCOD_RIGHT || keycode == C_KEYCOD_LEFT)
                    || ( keycode == C_KEYCOD_CONFIRM || keycode == C_KEYCOD_RETURN )
               )
#else
            if ( (keycode > 0 && keycode <= row_number)
                    || ( keycode == C_KEYCOD_PLUS || keycode == C_KEYCOD_MINUS)
                    || ( keycode == C_KEYCOD_CONFIRM || keycode == C_KEYCOD_RETURN )
               )
#endif
            {
                MKeyRlx();
                break;
            }
            else
            {
                keycode = MGetRotary(-1);
                if( keycode > 0 )keycode = C_KEYCOD_PLUS;
                else if( keycode < 0)keycode = C_KEYCOD_MINUS;
                else continue;
                break;
            }
        }
        while(true);

        if (keycode == C_KEYCOD_RETURN)
        {
            MKeyRlx();
            retvalue = C_FALSE;
            break;
        }
        if (keycode == C_KEYCOD_CONFIRM)
        {
            SystemParaStore();
            ChannelParaStore();
            MSetSystem();

            retvalue = C_TRUE;
            break;
        }
#if C_DEVTYPE == 14
        if (keycode == C_KEYCOD_RIGHT || keycode == C_KEYCOD_PLUS )
#else
        if (keycode == C_KEYCOD_RIGHT)
#endif
        {
            page++;
            if(page > page_max)page = 1;
            continue;
        }
#if C_DEVTYPE == 14
        else if (keycode == C_KEYCOD_LEFT || keycode == C_KEYCOD_MINUS )
#else
        else if (keycode == C_KEYCOD_LEFT)
#endif
        {
            page--;
            if(page == 0)page = page_max;
            continue;
        }


        row = keycode - 1;
        xpos = para_xpos;
        ypos = para_ypos + crow_height * row-1;

        InvertWords(menu_xpos + C_ECHAR_HDOT,ypos-MGetLanguage()*C_ECHAR_VDOT, 1); //反白显示几个字的区域

        if( MGetUnitType() > 0)deci_len = 3;
        else deci_len = 1;//都带1小数
        number = 100;
        if( page == 1)//输入参数
        {
            switch (keycode)
            {
            case 1:		//焊缝类型
                //	cpText = (u_short*)_Weld_C1 ;
                MPushWindow(xpos, ypos, xpos + (5+MGetLanguage())*C_CCHAR_HDOT+10, ypos + crow_height * 4) ;
                EraseDrawRectangle(xpos, ypos, xpos + (5+MGetLanguage())*C_CCHAR_HDOT+10, ypos + crow_height * 4) ;
                //	CEMenuOut(xpos+4,ypos+4 , cpText ,3,4,crow_height);	//在指定位置根据每行字符数、行数、行高写菜单
                for (i=0; i<4; i++)
                {
                    TextOut(xpos+4,ypos+4+crow_height*i,1,6+2*MGetLanguage(),16,(u_char*)_Weld_C1[MGetLanguage()][i],4);
                }
                while(1)
                {
                    keycode = MAnyKeyReturn();
                    if (keycode == C_KEYCOD_RETURN||keycode == C_KEYCOD_CONFIRM)
                    {
                        MKeyRlx();
                        break;
                    }
                    else if(keycode >= 1 && keycode <= 4)
                    {
                        stWeldPara.type = keycode-1;
                        break;
                    }
                }
                MPopWindow();
                break;
            case 2:	//探测面
//				cpText = (u_short*)_Weld_D1 ;
                MPushWindow(xpos, ypos, xpos + (6+2*MGetLanguage())*C_CCHAR_HDOT+1, ypos + crow_height * 3+1) ;
                EraseDrawRectangle(xpos, ypos, xpos + (6+2*MGetLanguage())*C_CCHAR_HDOT, ypos + crow_height * 3) ;
                //CEMenuOut(xpos+4,ypos+4 , cpText ,3,3,crow_height);	//在指定位置根据每行字符数、行数、行高写菜单
                for (i=0; i<3; i++)
                {
                    TextOut(xpos+4,ypos+4+crow_height*i,1,8+3*MGetLanguage(),16,(u_char*)_Weld_D1[MGetLanguage()][i],4);
                }
                while(1)
                {
                    keycode = MAnyKeyReturn();
                    if (keycode == C_KEYCOD_RETURN||keycode == C_KEYCOD_CONFIRM)
                    {
                        MKeyRlx();
                        break;
                    }
                    else if(keycode >= 1 && keycode <= 3)
                    {
                        stWeldPara.face = keycode-1;
                        break;
                    }
                }
                MPopWindow();
                break;
            case 3://工件厚度
                while(true)
                {
                    if( Input_Number(xpos,ypos,&number,2, &deci_len,0) != 1)break;
                    else
                    {
                        stWeldPara.thick1 = number;
                        break;
                    }
                }
                break;
            case 4://有效厚度
                while(true)
                {
                    if( Input_Number(xpos,ypos,&number,2, &deci_len,0) != 1)break;
                    else if( number <= stWeldPara.thick1 && stWeldPara.type < 3 )
                    {
                        stWeldPara.thick2 = number;
                        break;
                    }
                    else
                    {
                        stWeldPara.thick2 = number;
                        break;
                    }

                }
                break;
            case 5://焊缝宽度
                while(true)
                {
                    if( Input_Number(xpos,ypos,&number,2, &deci_len,0) != 1)break;
                    else
                    {
                        stWeldPara.wide = number;
                        break;
                    }
                }
                break;
            case 6://根部半径
                while(true)
                {
                    if( Input_Number(xpos,ypos,&number,2, &deci_len,0) != 1)break;
                    else
                    {
                        stWeldPara.radius = number;
                        break;
                    }
                }
                break;
            case 7://上端余高
                while(true)
                {
                    if( Input_Number(xpos,ypos,&number,2, &deci_len,0) != 1)break;
                    else
                    {
                        stWeldPara.high1 = number;
                        break;
                    }
                }
                break;
            case 8://下端余高
                while(true)
                {
                    if( Input_Number(xpos,ypos,&number,2, &deci_len,0) != 1)break;
                    else
                    {
                        stWeldPara.high2 = number;
                        break;
                    }
                }

                break;
            case 9://坡口角度
                deci_len = 1;//都带1小数
                while(true)
                {
                    if( Input_Number(xpos,ypos,&number,2, &deci_len,0) != 1)break;
                    else
                    {
                        stWeldPara.angle1 = number;
                        if( stWeldPara.type < 3 )stWeldPara.angle2 = stWeldPara.angle1/2;
                        break;
                    }
                }
                break;
            }
        }
        else if(page == 2)
        {
            switch( keycode )
            {
                //第二页
            case 1://坡口面角
                deci_len = 1;//都带1小数
                while(true)
                {
                    if( Input_Number(xpos,ypos,&number,2, &deci_len,0) != 1)break;
                    else
                    {
                        stWeldPara.angle2 = number;
                        if( number > stWeldPara.angle1 && stWeldPara.type < 2 )
                        {
                            stWeldPara.angle1 = number;
                        }
                        break;
                    }
                }
                break;
            case 2://坡口深度
                while(true)
                {
                    if( Input_Number(xpos,ypos,&number,2, &deci_len,0) != 1)break;
                    else if( number <= stWeldPara.thick1)
                    {
                        stWeldPara.deep = number;
                        if( stWeldPara.type == 3  )
                        {
                            if( stWeldPara.thick2 < stWeldPara.deep + stWeldPara.blunt)
                                stWeldPara.blunt = stWeldPara.thick2 - stWeldPara.deep;
                        }
                        else
                        {
                            //钝边加坡口深大于板厚
                            if( stWeldPara.thick1 < stWeldPara.deep)stWeldPara.deep = stWeldPara.thick1;
                            if( stWeldPara.type < 2 )
                            {
                                stWeldPara.blunt = stWeldPara.thick1 - stWeldPara.deep;
                            }
                        }

                        break;
                    }
                }
                break;
            case 3://钝边高度
                while(true)
                {
                    if( Input_Number(xpos,ypos,&number,2, &deci_len,0) != 1)break;
                    else //if( number < stWeldPara.thick1 )
                    {
                        stWeldPara.blunt = number;
                        if( stWeldPara.type == 3  )
                        {
                            if( stWeldPara.thick2 < stWeldPara.deep + stWeldPara.blunt)
                                stWeldPara.deep = stWeldPara.thick2 - stWeldPara.blunt;
                        }
                        else
                        {
                            //钝边加坡口深大于板厚
                            if( stWeldPara.thick1 < stWeldPara.deep)stWeldPara.deep = stWeldPara.thick1;
                            if( stWeldPara.type < 2 )
                            {
                                stWeldPara.deep = stWeldPara.thick1 - stWeldPara.blunt;
                            }
                        }


                        break;
                    }
                }
                break;
            case 4://根部间隙
                while(true)
                {
                    if( Input_Number(xpos,ypos,&number,2, &deci_len,0) != 1)break;
                    else
                    {
                        stWeldPara.space = number;
                        break;
                    }
                }
                break;

            case 5://坡口角度
                deci_len = 1;//都带1小数
                while(true)
                {
                    if( Input_Number(xpos,ypos,&number,2, &deci_len,0) != 1)break;
                    else
                    {
                        stWeldPara.angle3 = number;
                        if( stWeldPara.type < 3 )stWeldPara.angle4 = stWeldPara.angle3/2;
                        break;
                    }
                }
                break;

            case 6://坡口面角
                deci_len = 1;//都带1小数
                while(true)
                {
                    if( Input_Number(xpos,ypos,&number,2, &deci_len,0) != 1)break;
                    else
                    {
                        stWeldPara.angle4 = number;
                        if( number > stWeldPara.angle3 && stWeldPara.type < 2 )
                        {
                            stWeldPara.angle3 = number;
                        }
                        break;
                    }
                }
                break;

            case 7://坡口深度
                while(true)
                {
                    if( Input_Number(xpos,ypos,&number,2, &deci_len,0) != 1)break;
                    else if( number <= stWeldPara.thick1)
                    {
                        stWeldPara.deep3 = number;
                        if( stWeldPara.type == 3  )
                        {
                            if( stWeldPara.thick2 < stWeldPara.deep + stWeldPara.blunt)
                                stWeldPara.blunt = stWeldPara.thick2 - stWeldPara.deep3;
                        }
                        else
                        {
                            //钝边加坡口深大于板厚
                            if( stWeldPara.thick1 < stWeldPara.deep)stWeldPara.deep3 = stWeldPara.thick1;
                            if( stWeldPara.type < 2 )
                            {
                                stWeldPara.blunt = stWeldPara.thick1 - stWeldPara.deep3;
                            }
                        }

                        break;
                    }
                }
                break;
            }
        }

    }
    return retvalue;
}


int ShowWeld(short lxpos,short lypos,short width,short height,short angle,int dist)
{

    int keycode;
    short xpos,ypos;
    short cypos,size;
//	int number;
    int probe;

    u_char echo[C_LEN_SAMP];
    probe = C_OFF_SAMP;
    MCopyProtectedMemory( echo, (void*)probe, C_LEN_SAMP, PM_COPY_READ);

    xpos = ( C_COORWIDTH - C_CCHAR_HDOT * 15 ) / 8 * 8-2*8*MGetLanguage();
    ypos = C_COORVPOSI;
    EraseDrawRectangle(xpos, ypos, C_COORWIDTH+24+9*8*MGetLanguage(), ypos + 60) ;
//	CEMenuOut(xpos + C_ECHAR_HDOT,ypos+4,_Weld_A2,12,1,24);
    TextOut(xpos + C_ECHAR_HDOT,ypos+4,1,28,32,(u_char*)_Weld_A2[MGetLanguage()][0],4);

    if( stWeldPara.type == 3)
    {
        cypos = lypos +height/2+5;
        size = 30;
    }
    else
    {
        cypos = lypos +height/2+5;
        size = 50;
    }

    xpos += C_CCHAR_HDOT * 12+5*8*MGetLanguage();
    ypos += 4;
    probe = stWeldPara.probe - MGetForward();
    keycode = 1;
    probe = 0;
    while(true)
    {
        if( keycode > 0)
        {
            stWeldPara.probe = probe + MGetForward();
            EraseDrawRectangle(lxpos, lypos, lxpos+500, lypos+height) ;
            WELD_DRAW( &stWeldPara,lxpos+10, lxpos+480, cypos, size,angle, dist);
        }
        else
        {
            keycode = 1;
            break;
        }
        Write_Number( xpos,ypos+MGetLanguage()*C_ECHAR_VDOT,probe,5,1,0);
        if( MGetUnitType() > 0)keycode = 3;//单位inch
        else keycode = 1;//临时用作小数位数
        keycode = Input_Number(xpos-MGetLanguage()*C_ECHAR_HDOT*6,ypos+24,&probe,3, &keycode,0) ;
        if( keycode == 0)break;//按了返回键
    }
    probe = C_OFF_SAMP;
    MCopyProtectedMemory( (void*)probe, echo, C_LEN_SAMP, PM_COPY_WRITE); //
    return keycode;
}
int ShowWeldA(short lxpos,short lypos,short width,short height,short angle,int dist)
{
    short xpos,ypos;
    short cypos,size;

    if( stWeldPara.type == 3)
    {
        cypos = lypos +height/2+5;
        size = 30;
    }
    else
    {
        cypos = lypos +height/2+5;
        size = 50;
    }

    xpos = ( C_COORWIDTH - C_CCHAR_HDOT * 14) / 8 * 8;
    ypos = C_COORVPOSI;
    EraseDrawRectangle(xpos, ypos, C_COORWIDTH, ypos + 40) ;
//	CEMenuOut(xpos + C_ECHAR_HDOT,ypos+4,_Weld_A2,12,1,24);
    TextOut(xpos + C_ECHAR_HDOT,ypos+4,1,28,24,(u_char*)_Weld_A2[MGetLanguage()][0],4);
    xpos += C_CCHAR_HDOT * 12;
    ypos += 4;
    EraseDrawRectangle(lxpos, lypos, lxpos+500, lypos+height) ;
    WELD_DRAW( &stWeldPara,lxpos+10, lxpos+480, cypos, size,angle, dist);
    Write_Number( xpos,ypos + 9,stWeldPara.probe - MGetForward(),5,1,0);
    while(true)
    {
        if( MAnyKeyReturn() == C_KEYCOD_RETURN)break;//按了返回键
    }
    return stWeldPara.probe;
}



int tspLine(POINT *PosIn, POINT *PosOut, int n, int ch, float tx1, float tx2, float ty1, float ty2)
{
    const int MAX_NUM = 32;
    float x[MAX_NUM], y[MAX_NUM];
    int num = 0, tt;
    float a[MAX_NUM], b[MAX_NUM], c[MAX_NUM], dx[MAX_NUM], dy[MAX_NUM];
    float qx[MAX_NUM], qy[MAX_NUM];
    float bx3, bx4, by3, by4;
    float cx, cy, t[MAX_NUM+1], px[MAX_NUM], py[MAX_NUM];
//	float u[3], v[3];
    short i;

    for (i=0; i<n; i++)
    {
        x[i] = PosIn[i].x;
        y[i] = PosIn[i].y;
    }
    for (i=0; i<n; i++)
        t[i] = 73;//hypot(x[i+1] - x[i], y[i+1] - y[i]);
    t[n] = 1.0e+8;//为以后的计算初始化t[n]，t[0]废弃
    switch(ch)
    {
    case 0: //自由条件
        a[0] = 2;
        c[0] = 1;
        dx[0] = 3 * (x[1] - x[0]) / t[1];
        dy[0] = 3 * (y[1] - y[0]) / t[1];
        a[n] = 2;
        b[n] = 1;
        dx[n] = 3 * (x[n] - x[n-1]) / t[n];
        dy[n] = 3 * (y[n] - y[n-1]) / t[n];
        break;
    case 1: //夹持条件
        a[0] = 1;
        c[0] = 0;
        dx[0] = tx1;
        dy[0] = ty1;
        a[n] = 1;
        b[n] = 0;
        dx[n] = tx2;
        dy[n] = ty2;
        break;
    default:
        return false;
    }
    //计算方程组系数阵和常数阵
    for (i=1; i<=n-1; i++)
    {
        a[i] = 2 * (t[i] + t[i+1]);
        b[i] = t[i+1];
        c[i] = t[i];
        dx[i] = 3 * (t[i] * (x[i+1] - x[i]) / t[i+1] + t[i+1] * (x[i] - x[i-1]) / t[i]);
        dy[i] = 3 * (t[i] * (y[i+1] - y[i]) / t[i+1] + t[i+1] * (y[i] - y[i-1]) / t[i]);
    }
    //采用追赶法解方程组
    c[0] = c[0] / a[0];
    for (i=1; i<=n-1; i++)
    {
        a[i] = a[i] - b[i] * c[i-1];
        c[i] = c[i] / a[i];
    }
    a[n] = a[n] - b[n] * c[i-1];
    qx[0] = dx[0] / a[0];
    qy[0] = dy[0] / a[0];

    for (i=1; i<=n; i++)
    {
        qx[i] = (dx[i] - b[i] * qx[i-1]) / a[i];
        qy[i] = (dy[i] - b[i] * qy[i-1]) / a[i];
    }
    px[n] = qx[n];
    py[n] = qy[n];

    for (i=n-1; i>=0; i--)
    {
        px[i] = qx[i] - c[i] * px[i+1];
        py[i] = qy[i] - c[i] * py[i+1];
    }
    //计算曲线上点的坐标

    for (i=0; i<n-1; i++)
    {
        bx3 = (3 * (x[i+1] - x[i]) / t[i+1] - 2 * px[i] - px[i+1]) / t[i+1];
        bx4 = ((2 * (x[i] - x[i+1]) / t[i+1] + px[i] + px[i+1]) / t[i+1]) / t[i+1];
        by3 = (3 * (y[i+1] - y[i]) / t[i+1] - 2 * py[i] - py[i+1]) / t[i+1];
        by4 = ((2 * (y[i] - y[i+1]) / t[i+1] + py[i] + py[i+1]) / t[i+1]) / t[i+1];
        tt = 0;
        while (tt < t[i+1])
        {
            cx = x[i] + (px[i] + (bx3 + bx4 * tt) * tt) * tt;
            cy = y[i] + (py[i] + (by3 + by4 * tt) * tt) * tt;
            //if ((cx <= 10)) Beep(1000,2);
            PosOut[num].x = (short)cx;
            PosOut[num++].y = (short)cy;
            tt++;
        }
    }
    PosOut[num].x = PosIn[n-1].x;
    PosOut[num++].y = PosIn[n-1].y;
    return num;
}
