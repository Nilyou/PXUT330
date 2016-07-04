//test.h
#ifndef _TEST_H_


typedef struct
{
    int speed;		//预置声速
    int range[2];	//第一声程/第二声程
    int rangemode;	//声程类型0距离，1深度，2不用两次波的曲面

    int offset;		//零点
    unsigned short *pPmt1;	//提示1
    unsigned short *pPmt2;	//提示2

    int len1;	//提示1的每行长度
    int row1;	//提示1的行数
    int len2;	//提示2的每行长度
    int row2;	//提示2的行数

} TEST_OFFSET;

TEST_OFFSET stTestOffset;
int TestOffset(TEST_OFFSET *stTestOffset)
{
    /*
    t = ( R2*T1 - R1 * T2)/( R2 - R1) = T1 - (T2-T1)R1/(R2-R1)
    */

    int i,j;
    int testrange[2];
    int gain= 400, range, offset , offset0, delay = 0, speed = stTestOffset->speed;

    int amp_low,amp_high;
    int gateposi,gatewide;
    int rangemode = stTestOffset->rangemode;

    int echotime;
    int gateamp;

    int len,row,crow_height;

    //int number,deci_len;
    int xpos,ypos,menu_xpos,menu_ypos;
    int confirm_num = 0;
    int retvalue = C_TRUE;
    int keycode;
    int gatetype = 0;

    unsigned short _Pmt[] = { CH(1866),CH(3367),CH(2167),CH(4189),CH(4357),CH(100),CH(100),CH(100),CH(100),CH(100),CH(100)};
    //int i;

    for( i = 0; i < 2; i++)testrange[i] = stTestOffset->range[i];

    MKeyRlx();
    MSetScaleMode(2,0);

    if (MGetProbeMode() == C_DPROBE)	offset0 = 120;
    MSetOffset(offset0,C_SETMODE_SAVE);

    range = testrange[0] * 100/40;		//放在第4格
    MSetPara(gain,0,0,0,range,offset0,delay,0,speed);

    amp_low = MGetAmpStdMax()-1;
    amp_high = MGetAmpStdMax() +1;
    gateposi = C_COORHORIUNIT * 4;
    if( MGetProbeMode() == C_DPROBE )gatewide = C_COORHORIUNIT * 3;
    else gatewide = C_COORHORIUNIT * 2;
    MSetGatePara(gateposi,gatewide,120,0,C_SETMODE_SETSAVE);

    MSetBGateMode(0,C_SETMODE_SETSAVE);
    MSetGatePara(C_COORHORIUNIT * 6,C_COORHORIUNIT * 2,0,1,C_SETMODE_SETSAVE);

    MSetSystem();
    MChannelRenovate();	//本通道屏幕参数刷表

    MSetFunctionMode(0,C_FUNCTION_ALL);
    MSetAcquisition(1);

    len = stTestOffset->len1;
    row = stTestOffset->row1;
    crow_height = 24;

    menu_xpos = C_COORHORIUNIT * 10-(len+1)*C_CCHAR_HDOT;
    menu_ypos = C_COORVPOSI + 1;
    EraseDrawRectangle(menu_xpos, menu_ypos, C_COORHORIUNIT * 6, menu_ypos + crow_height * row_number) ;
    CEMenuOut(menu_xpos + C_ECHAR_HDOT,menu_ypos + 4, stTestOffset->pPmt1 ,len,row,crow_height);	/*在指定位置根据每行字符数、行数、行高写菜单*/
    //Write_Number(menu_xpos + 11 * C_ECHAR_HDOT,menu_ypos  + 4 ,testrange0,4,1,0);

    xpos = 0;
    ypos = 0;
    ClearCursor(2);
    MSetGateParaInit();	//门内最高波设初值

    preElapsedtime = GetElapsedTime();	//开始测试时间

    MSetSystemMode(1,0);		//设置成自动调节状态
    CMenuOut(0,0,_Pmt,6,1,24);
    while (1)
    {

        if (MAdjustGain(0,1,amp_low, amp_high ) )
        {
            MSetGateParaInit();	//门内最高波设初值
        }

        MParaRenovate(0);
        gateamp = MGetAmpMax(gatetype);
        if(MGetGateParaMax(gatetype) == C_TRUE )
        {
            ClearCursor(2);
            xpos = GateMax.Pos;
            ypos = C_COORVPOSI + C_COORHEIGHT - 2 - GateMax.Amp*2 ;
            if((GateMax.Amp*2) < C_COORHEIGHT)	DrawCursor(xpos,ypos,2);
        }
        keycode = MGetKeyCode(0);

        if(keycode == C_KEYCOD_CONFIRM )
        {
            //t = ( R2*T1 - R1 * T2)/( R2 - R1) = T1 - (T2-T1)R1/(R2-R1)
            if( confirm == 0)
            {
                confirm_num++;
                ClearCursor(2);
                echotime = MGetAmpTime(gatetype);

                offset = echotime - testrange[0]*C_SAMPLE_FREQ/speed;//先按预置声速和一次波时间算出零点

                if( testrange[1] <= testrange[0] )
                {
                    //如果二次声程小于一次声程或为0，则只测一次
                    confirm_num++;
                    break;
                }
                else
                {
                    if( testrange[1] > testrange[0] * 2)//二次声程大于一次的两倍
                    {
                        range = testrange[1] * 100/80;		//放在第8格
                    }
                    gateposi = C_COORHORIUNIT * 8-7;
                    if( MGetProbeMode() == C_DPROBE )gatewide = C_COORHORIUNIT * 3;
                    else gatewide = C_COORHORIUNIT * 1.5;
                    MSetGatePara(gateposi, gatewide, 120,0,C_SETMODE_SETSAVE);
                    gain = MGetBaseGain()+120;
                    MSetRange(range,C_SETMODE_SETSAVE);
                    MSetBaseGain(gain,C_SETMODE_SETSAVE);

                    MChannelRenovate();	//本通道屏幕参数刷表

                    MSetGateParaInit();	//门内最高波设初值


                    len = stTestOffset->len2;
                    row = stTestOffset->row2;
                    crow_height = 24;

                    menu_xpos = C_COORHORIUNIT * 8-(len+1)*C_CCHAR_HDOT;
                    menu_ypos = C_COORVPOSI + 1;
                    EraseDrawRectangle(menu_xpos, menu_ypos, C_COORHORIUNIT * 6, menu_ypos + crow_height * row_number) ;
                    CEMenuOut(menu_xpos + C_ECHAR_HDOT,menu_ypos + 4, stTestOffset->pPmt2 ,len,row,crow_height);	/*在指定位置根据每行字符数、行数、行高写菜单*/

                    //				xpos = C_COORHORIUNIT * 6-(char_len+1)*C_CCHAR_HDOT;
                    //				ypos = C_COORVPOSI + 1;
                    Write_Number(xpos + 11 * C_ECHAR_HDOT,ypos  + 4 ,testrange0,4,1,0);
                }

            }
            else
            {
                confirm_num++;
                offset = MGetAmpTime(gatetype) - echotime ;	//T2 - T1
                offset = echotime - testrange[0] * offset / (testrange[1] - testrange[0]);

                speed = testrange[0] * C_SAMPLE_FREQ/(echotime - offset);
                MSetSpeed(speed,C_SETMODE_SETSAVE);

                break;
            }
        }
        else if(keycode == C_KEYCOD_RETURN)
        {
            if( DisplayQuery(4) == 1)
            {
                retvalue = C_FALSE;
                SystemParaRestore();
                ChannelParaRestore();
                break;
            }
        }
        else if(keycode == C_KEYCOD_BASEGAIN)
        {
            if (GetSameKeyTime() > 20)
            {
                MAdjustGain(0,0,MGetAmpStdMax(),MGetAmpStdMax());
            }
            else
            {
                GainChange();
                //	CMenuOut(0,0,_TestMenu_A1+1,5,1,crow_height);
                TextOut(0,0,1,11,crow_height,(u_char*)_TestMenu_A1[MGetLanguage()][0]+2,4);
            }
        }
        else if (keycode == C_KEYCOD_GATE)
        {
            GateChange();
        }
        else if (keycode == C_KEYCOD_RANGE)
        {
            RangeChange();
        }
        else if (keycode == C_KEYCOD_OFFSET)
        {
            OffsetChange();
        }
        else continue;
        DisplayPrompt(15);
        CMenuOut(0,0,_Pmt,6,1,24);
        ExpendTime(10);
    }
    if( confirm_num > 1)
    {
        MSetOffset(offset + offset0,C_SETMODE_SETSAVE);
        if( MGetProbeMode() == C_APROBE )
        {
            //斜探头
            len = 8;
            row = 2;
            crow_height = 24;

            menu_xpos = C_COORHORIUNIT * 6-(char_len+1)*C_CCHAR_HDOT;
            menu_ypos = C_COORVPOSI + 1;
            EraseDrawRectangle(menu_xpos, menu_ypos, C_COORHORIUNIT * 10+17, menu_ypos + crow_height * row) ;
            TextOut(menu_xpos + C_ECHAR_HDOT,menu_ypos+2, row+1*MGetLanguage(), 17,crow_height-12*MGetLanguage(), (u_char*)_TestMenu_AB3[MGetLanguage()][1], 4);

//			EraseDrawRectangle(menu_xpos, menu_ypos, C_COORHORIUNIT * 6, menu_ypos + row * crow_height) ;
//			CEMenuOut(menu_xpos + C_ECHAR_HDOT,menu_ypos + 4, _TestMenu_AB7,len,row,crow_height);
            int number,deci_len;

            deci_len = 1;
            number = testrange1;
            menu_xpos = menu_xpos+10*C_ECHAR_HDOT+8*MGetLanguage();
            menu_ypos = C_COORVPOSI + 37-4*MGetLanguage();

            while(1)
            {
                if( Input_Number(menu_xpos, menu_ypos,&number,2, &deci_len,0) != 1)break;
                else
                {
                    Write_Number(menu_xpos, menu_ypos, number, 2, 1, 0);
                    MSetForward(testrange1-number,C_SETMODE_SAVE);
                    break;
                }
            }
        }
        //MSetTestStatus(C_TEST_OFFSET,1);	//设置测试状态,零点已测

        MSetGatePara(C_COORHORIUNIT * 3, C_COORHORIUNIT * 2 ,120,0,C_SETMODE_SETSAVE);

        //	MChannelRenovate();	//本通道屏幕参数刷表

        SystemParaStore();
        ChannelParaStore();

        retvalue = C_TRUE;
    }

    MKeyRlx();
    range = testrange1 * 100/40;		//放在第6.5格
    MSetRange(range,C_SETMODE_SETSAVE);
    return retvalue;
}

int TestMenuZero(int iBlock/*试块类型*/,bool bBlockModify,int iProbemode/*探头类型*/, bool bProbeModify, int iSoundmode/*声波类型*/, bool bSoundModify, int iRange1/*声程1*/, bool bRangeModify1, int iRange2/*声程2*/, bool bRangeModify2);
//测零点菜单，各bool量为真可修改相应量
int TestZero(int iProbemode/*探头类型*/, int iSpeed/*默认声速*/, int iRange1/*声程1*/, int iRange2/*声程2*/,int iForward/*是否测前沿*/);


int TestMenuAngle( int iAngle/*标称折射*/, bool bAngleModify, int iDiameter/*孔直径*/, bool bDiameterModify, int iDepth/*孔深*/, bool bDepthModify,int iContact/*接触面*/,bool bContact,int iOutRandius/*外半径*/);
int TestAngle( int iAngle/*标称折射*/, int iDiameter/*孔直径*/, int iDepth/*孔深*/, int iContact/*接触面*/,int iOutRandius,int iInnerRandius);

int TestSensitivity(int mode,int iRange,bool bParaAdjust);//调灵敏度


int TestMenuOffset(void);
int TestOffset(u_int testrang1,u_int testrange2,u_int mode);
//第1声程，第2声程 mode = 0距离测，=1深度 =2 曲面
//int TestMenuAngle(void);
//int TestAngle(u_int diameter,u_int depth);//直径、深度

int TestMenuDac(void);
int TestDac(u_int depthmax,u_int diameter,u_int length);//最大深度、反射体直径、长度
int CalcuDac(void);	//计算DAC,
int DrawDac(int mode);		//画和或计算DAC曲线,mode=0，计算=1不
int GetLine_TwoDot(u_short* line,int dist1,int dist2);
int GetMiddleValue(int dist1,int db1,int dist2,int db2,int dist);	//得到中间值

int TestMenuStandard(void);
int TestMenuThick(void);
int TestMenuSystem(void);


#define _TEST_H_
#endif

