//与各种参量有关的函数
extern u_short dac_line[];
extern FUNCTION             Function;   //功能


int ChannelParaInit(int mode)	/*参数初始化，mode=0当前一个通道，mode=1所有通道,2所有用户*/
{
    int retvalue = 1;
    //int offset = C_OFF_CHANNEL;
    //u_char i=0;
    //int probe,speed;
//	int speed;
    int i;
    u_char Channel;
    int stdmode=MGetStdMode();
    int nCurrentUser = GetCurrentUser();
//	int nUserNumber = GetUserNumber();

    int testrange[3];
    int offset;
    int ChMax = 0, UserMax, temp_ch ;

    Dac.num = 0;

    Channel = MGetChannel();
    if( Channel < C_CHMAX)temp_ch = Channel;
    else temp_ch = 0;
    UserMax = nCurrentUser + 1;
    if(mode == 0)
    {
        ChMax = Channel + 1;
    }
    else if (mode > 0)
    {
        MSaveDefualtColor(1);//设置默认颜色
        SystemStatus.option = 0;
        SystemStatus.sys = 0;
        Function.funcMenu[0]=1;
        Function.funcMenu[1]=2;
        Function.funcMenu[2]=7;
        Function.funcMenu[3]=9;

        offset = C_OFF_KEYFUNC;
        MCopyProtectedMemory( (void*)offset, &Function, C_LEN_FUNCTION, PM_COPY_WRITE);
        CopyMemoryToSystem( (void*)offset,C_LEN_FUNCTION,PM_COPY_WRITE);

        /*返回值为当前打印机类型1\2\3\4=LQ\HP\EPSON C/P*/
        MSetPrinterMode(0,0/*mode = 0*/);	/*设置当前打印机类型*/
        /*返回值为当前输入法类型1\2\3\4=数字\大写\小写*/
        MSetInputMode(C_INPUT_DIGITAL,0/*mode = 0*/);	/*设置当前输入法类型*/

        /*返回值为当前系统状态1\2\3\4=正常\调节\测试版\特殊版*/
        MSetSystemMode(0 ,0/*mode = 0*/);	/*设置当前系统状态*/
        /*返回值为当前屏幕亮度*/
        MSetScreenBrightness(3,C_SETMODE_SETSAVE/*mode = 0*/);		/**/

        MSetSaveMode(0,C_SETMODE_SETSAVE);//关闭连续记录
        MSetSaveStatus( 0,C_SETMODE_SETSAVE);
        MSetSavePosition(0,C_SETMODE_SETSAVE);

        MSetFill(0,C_SETMODE_SETSAVE);
        MSetDenoise(0,C_SETMODE_SETSAVE);
        MSetRepeatRate(100,C_SETMODE_SETSAVE);
        if(extend==0)
        {
            MSetPulseWidthInput(C_PULSE_MIN, C_SETMODE_SET);
        }
        else
        {
            MSetPulseWidth(0,C_SETMODE_SETSAVE);
        }

        MSetUnitType(0);

        ChMax = C_CHMAX ;
        Channel = 0;
        if( mode > 1)
        {
            nCurrentUser = 0;
            UserMax = C_USERMAX;
            SetUserNumber( C_USERMAX);
        }
    }



    for( ; nCurrentUser < UserMax; nCurrentUser++)
    {
        if( mode > 0 )
        {
            SetCurrentUser(nCurrentUser);
            SystemParaStore();
            Channel = 0;
        }

        for( ; Channel < ChMax; Channel++)
        {
            ExpendTime(50);
//Write_Number(0,0,Channel,4,0,0);
            ChannelStatus.option = 0;
            ChannelStatus.status = 0;
            ChannelStatus.sys = 0;

            SystemStatus.displaymode = 1;
            SystemStatus.language = 0;
            SystemStatus.unit = 0;

            stWeldPara.type = 0;
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

            MSetAmpStdMax(80);//回波降落时的标准高度，默认80；


            /*返回值为当前屏保类型*/
            ///MSetScrsaveMode(1,0/*mode = 0*/);	/*设置当前屏保类型*/

            /*返回值为当前屏保延时,不超过30分钟*/
            ///MSetScrsaveTime(1,0/*mode = 0*/);	/*设置当前屏保延时,不超过30分钟*/

            /*返回值为当前标度1\2\3=垂直\水平\距离*/
            MSetScaleMode(0,2/*mode = 0\1\2*/);	/*设置当前标度*/

            /*返回所用当量标准：母线\判废\定量\评定	*/
            MSetDacMode(2,0/*mode = 0*/);

            /*返回所用当量显示：dB值\孔径	*/
            MSetAvgMode(0,0/*mode = 0*/);

            /*返回所用频带宽度：2.5\5.0\10\wide	*/
#if C_DEVTYPE == 4 || C_DEVTYPE == 20 || C_DEVTYPE == 5 || C_DEVTYPE == 12
            MSetBandMode(3,0/*mode = 0\1\2*/);
#else
            MSetBandMode(0,0/*mode = 0\1\2*/);
#endif
            /*返回所用探头阻尼：50Q\400Q 	*/
            MSetDampMode(1,0/*mode = 0\1\2*/);

            /*返回所用发射电压：200V\400V  	*/
            MSetPulseMode(2,0/*mode = 0\1\2*/);

            /*返回所用检波方式：全波\正半波\负半波\射频  	*/
            MSetEchoMode(0,0/*mode = 0\1\2*/);


            /*返回值为当前探头模式0\1\2\3=直\斜\双晶*/
            MSetProbeMode(C_APROBE,0/*mode = 0\1\2*/);	/*设置探头模式*/

            /*返回值为当前测试状态0\1\2\=未测\AVG\DAC\AVG*/
            MSetTestStatus(C_TEST_ALL,0);

            /*
            以下为各种参量的获得与设置
            */
            MSetGatePara(C_COORHORIUNIT * 2,C_COORHORIUNIT * 3,120,0,C_SETMODE_SETSAVE);
            MSetGatePara(C_COORHORIUNIT * 7,C_COORHORIUNIT * 2,0,1,C_SETMODE_SETSAVE);
            MSetGatedB(21, 0);

            //设置门

            /*返回值为当前通道试件声速：	无小数	*/
//			if( MGetProbeMode() == C_APROBE)speed = 3230;
//			else speed = 5900;
//			MSetSpeed(speed,0/*mode = 0\1\2*/);	/*设置当前通道	*/
            /*返回值为当前通道试件声速：	无小数	*/
            if(MGetProbeMode() != 1)	//非斜探头
            {
                if( MGetUnitType() > 0)MSetSpeed(CD_SPEED_1*1000/254,C_SETMODE_SAVE);//单位inch
                else MSetSpeed(CD_SPEED_1,C_SETMODE_SAVE);
            }
            else
            {
                if( MGetUnitType() > 0)MSetSpeed(CD_SPEED_2*1000/254,C_SETMODE_SAVE);//单位inch
                else MSetSpeed(CD_SPEED_2,C_SETMODE_SAVE);

            }

            MSetDepth(0,0);
            /*返回值为当前通道折射角	一位小数*/
            MSetAngle(0,0/*mode = 0\1\2*/);	/*设置当前通道	*/


            /*返回值为当前通道探头前沿长度 一位小数	*/
            MSetForward(0,0/*mode = 0*/);	/*设置当前通道	*/

            /*返回值为当前通道探头频率 一位小数	*/
            MSetFrequence(250,0/*mode = 0*/);	/*设置当前通道	*/

            /*返回值为当前通道晶片尺寸 a*256+b	*/
            MSetCrystal_l(13000,0/*mode = 0*/);	/*设置当前通道	*/
            MSetCrystal_w(13000,0/*mode = 0*/);	/*设置当前通道	*/

            /*返回值为当前通道基本增益 一位小数	*/
            MSetBaseGain(450,0/*mode = 0\1\2*/);	/*设置当前通道	*/

            /*返回值为当前通道表面补偿增益 一位小数	*/
            MSetSurfGain(0,0/*mode = 0\1\2*/);	/*设置当前通道	*/

            /*返回值为当前通道补偿增益 一位小数	*/
            MSetCompGain(0,0/*mode = 0\1\2*/);	/*设置当前通道	*/


            /*返回值为当前通道	*/
            for(i = 0; i < 6 ; i++)MSetLineGain(i,0);

            /*返回值为当前通道	*/

            if( MGetUnitType() > 0)MSetRange(5000,C_SETMODE_SETSAVE/*mode = 0\1\2*/);	/*设置当前通道	*/
            else MSetRange(2000,C_SETMODE_SETSAVE/*mode = 0\1\2*/);	/*设置当前通道	*/


            /*返回值为当前通道	*/
            MSetDelay(0,0/*mode = 0\1\2*/);	/*设置当前通道	*/

            MSetDepthGain(0);


            MSetReject(0,0);

            /*返回值为当前通道	*/
            MSetOffset(0,0/*mode = 0\1\2*/);	/*设置当前通道	*/

            MSetFunctionMode(0,C_FUNCTION_ALL);
            /*返回所用标准	*/
            if(mode > 0 )
            {
                MSetStdMode(12,0/*mode = 0*/);
            }
            else
            {
                MSetStdMode(stdmode,0/*mode = 0*/);
            }
            //MSetStdMode(1,C_SETMODE_SAVE);
            MSetCurveNumber(3);
            MSetBlock(0);
            MSetThick(0);
            MSetDiameter(0);

            MSetChannel(Channel,0);
	
			ChannelPara.iAngle = 60;
	        ChannelPara.iThickness = 200;
			ChannelPara.iPcs = 462;
			
            ChannelParaStore();

            retvalue = C_TRUE;
        }
    }

//Write_Number(0,10,1,4,0,0);
    if( mode > 1)
    {
        SetCurrentUser(0);
        SetUserNumber( 1 );
    }

//Write_Number(0,20,2,4,0,0);
    MSetChannel(temp_ch,C_SETMODE_SETSAVE);
    ChannelParaRestore();
//Write_Number(0,20,3,4,0,0);
    MSetSurfGain(0,0/*mode = 0\1\2*/);	/*设置当前通道	*/
    ChannelParaStore();
//Write_Number(0,20,4,4,0,0);
    ExpendTime(20);
    MSetSystem();		/*设置当前系统各状态*/


    if (MGetStdMode()==2)	//JB/T4730-2005
    {
        testrange[1] = 1000;
        testrange[2] = 0;
        offset = C_OFF_TEMP + 0;
        MCopyProtectedMemory( (void*)offset, testrange, 12, PM_COPY_WRITE);
        testrange[0] = 500;
        testrange[1] = 300;
        offset = C_OFF_TEMP + 20;
        MCopyProtectedMemory( (void*)offset, testrange, 8, PM_COPY_WRITE);
        testrange[0] = 500;
        testrange[1] = 10;
        testrange[2] = 60;
        offset = C_OFF_TEMP + 30;
        MCopyProtectedMemory( (void*)offset, testrange, 12, PM_COPY_WRITE);
    }
    else if (MGetStdMode()==12)	//JB/T47013
    {
        testrange[1] = 1000;
        testrange[2] = 0;
        offset = C_OFF_TEMP + 0;
        MCopyProtectedMemory( (void*)offset, testrange, 12, PM_COPY_WRITE);
        testrange[0] = 500;
        testrange[1] = 300;
        offset = C_OFF_TEMP + 20;
        MCopyProtectedMemory( (void*)offset, testrange, 8, PM_COPY_WRITE);
        testrange[0] = 800;
        testrange[1] = 20;
        testrange[2] = 400;
        offset = C_OFF_TEMP + 30;
        MCopyProtectedMemory( (void*)offset, testrange, 12, PM_COPY_WRITE);
    }
    else if (MGetStdMode()==7)	//CB/T3559-94
    {
        testrange[1] = 500;
        testrange[2] = 1000;
        offset = C_OFF_TEMP + 0;
        MCopyProtectedMemory( (void*)offset, testrange, 12, PM_COPY_WRITE);
        testrange[0] = 500;
        testrange[1] = 300;
        offset = C_OFF_TEMP + 20;
        MCopyProtectedMemory( (void*)offset, testrange, 8, PM_COPY_WRITE);
        testrange[0] = 500;
        testrange[1] = 30;
        testrange[2] = 400;
        offset = C_OFF_TEMP + 30;
        MCopyProtectedMemory( (void*)offset, testrange, 12, PM_COPY_WRITE);
    }
//Write_Number(0,20,5,4,0,0);


    return retvalue;
}

//门有关的
void MSetGatePara(u_short Posi,u_short Wide,u_short High,int gatetype,int mode)
/*设置门参数，gatetype=0A门1B门*/
{
    if (mode == C_SETMODE_SAVE || mode == C_SETMODE_SETSAVE)
    {
        if(Posi > C_HORIDOT_SCREEN )Posi = C_HORIDOT_SCREEN - 1;
        if(Posi + Wide > C_HORIDOT_SCREEN )Wide = C_HORIDOT_SCREEN - 1 - Posi ;
        if(High > C_COORHEIGHT )High = C_COORHEIGHT ;
        if( gatetype == 0)
        {
            AGatePara.Position = Posi ;
            AGatePara.Width = Wide ;
            AGatePara.Height = High ;
        }
        else if( gatetype == 1)
        {
            BGatePara.Position = Posi ;
            BGatePara.Width = Wide ;
            BGatePara.Height = High ;
        }
    }
    if (mode == C_SETMODE_SETSAVE || mode == C_SETMODE_SET)
    {
        GATE_PARA	GatePara;
#if C_DEVLIB == 1
        Posi = Posi + HoriOffsetScreen;
#endif
        if( MGetEchoMode() == C_RF_WAVE)
        {
            Posi = ( (Posi+1)/2 ) * 2;	//射频是门位必须为双
            Wide = ( (Wide+1)/2 ) * 2;
            High = ( (High+1)/2 ) * 2;
        }
        GatePara.Position = Posi ;

        GatePara.Width = Wide ;
        GatePara.Height = High ;
        //if( (MGetBGateMode() == 1 && gatetype == 1) || gatetype == 0 )
#if C_DEVLIB == 4
        SetPeakGate(MGetChannel(),&GatePara, gatetype) ;
#else
        SetPeakGate(&GatePara, gatetype) ;
#endif
        MSetColor(C_CR_WAVE);
        //if( MGetBGateMode() != 0 && gatetype == 1)
        if( gatetype == 1)
        {
            //ExpendTime(10);
            MSetGatePara(MGetGatePara(0,0),MGetGatePara(0,1),MGetGatePara(0,2),0,C_SETMODE_SET);
        }
    }
}


u_short MGetGatePara(int gatetype, int paratype)   /* 返回值为门参数 */
{
    if(gatetype == 0 && paratype == 0)
    {
        if( AGatePara.Position >= C_COORWIDTH )AGatePara.Position = C_COORWIDTH;
        return AGatePara.Position;
    }

    if(gatetype == 0 && paratype == 1)
    {
        if( AGatePara.Position > C_COORWIDTH )AGatePara.Position = C_COORWIDTH;
        if( AGatePara.Position + AGatePara.Width >= C_GATEPOSIMAX )AGatePara.Width = C_GATEPOSIMAX - AGatePara.Position;
        return AGatePara.Width;
    }
    if(gatetype == 0 && paratype == 2)
    {
        if( AGatePara.Height > C_COORHEIGHT )AGatePara.Height = C_COORHEIGHT;
        return AGatePara.Height;
    }
    if(gatetype == 1 && paratype == 0)
    {
        if( BGatePara.Position >= C_COORWIDTH )BGatePara.Position = C_COORWIDTH;
        return BGatePara.Position;
    }

    if(gatetype == 1 && paratype == 1)
    {
        if( BGatePara.Position > C_COORWIDTH )BGatePara.Position = C_COORWIDTH;
        if( BGatePara.Position + BGatePara.Width >= C_GATEPOSIMAX )BGatePara.Width = C_GATEPOSIMAX - BGatePara.Position;
        return BGatePara.Width;
    }

    if(gatetype == 1 && paratype == 2)
    {
        if( BGatePara.Height > C_COORHEIGHT )BGatePara.Height = C_COORHEIGHT;
        return BGatePara.Height;
    }

    return 0;
}


//DAC gate
int MGetDacDb(void)
{
    int offset;
    u_short dac_dB;
    int basegain;

    basegain = MGetBaseGain() + 481.31;	//基准增益
    offset = C_OFF_DACDB + MGetGatePara(0,0)*C_SIZE_SHORT ;	//门位
    MCopyProtectedMemory(&dac_dB, (void*)offset, C_SIZE_SHORT, PM_COPY_READ);
    //dacdB：在DAC母线门位处的dB值
    dac_dB = basegain - dac_dB;
    return dac_dB;
}

int DacGate_dBHigh(int dB,int dac_dB)//门与母线的dB差，DACdB值
{
    //int offset;
    int gatehigh;

    MSetGatedB(dB, 0);
	gatehigh = (int)pow(10,(float)(dB+dac_dB)/200.0);
    if(gatehigh >= C_COORHEIGHT ) gatehigh = C_COORHEIGHT - 1 ;
    return gatehigh;
}

void DacGateInit(void)
{
    int offset;
    int dac_dB,gate_dB;
    int basegain;
    int gatehigh;
    int i;
    int dist1,dist2;
    int step = 14;


    dac_dB = MGetDacDb();	//dacdB：在DAC母线门为处的dB值
    gate_dB = MGetGatedB();

    gatehigh = DacGate_dBHigh(gate_dB,dac_dB);//门与母线的dB差，DACdB值
    MSetGatePara( MGetGatePara(0,0),MGetGatePara(0,1),gatehigh,0,C_SETMODE_SETSAVE);

    basegain = MGetBaseGain() + 481.31 + gate_dB;	//基准增益
    offset = C_OFF_DACDB;
    MCopyProtectedMemory( dac_line, (void*)offset, C_LEN_DACDB, PM_COPY_READ);
    dist2 = 0;
	dac_line[dist2] = (int)pow( 10,( basegain - dac_line[dist2] )/200.0) ;
    while(dist2 <= C_COORWIDTH)
    {
        dist1 = dist2;
        dist2 += step;
		dac_line[dist2] = (int)pow( 10,( basegain - dac_line[dist2] )/200.0) ;
        GetLine_TwoDot(dac_line,dist1,dist2);
    }
    for(i = 0 ; i < C_SCREEN_WIDTH; i ++ )
    {
        if(dac_line[i] >= C_COORHEIGHT)dac_line[i] = C_COORHEIGHT - 1;
    }
    offset = C_OFF_DACGATE;
    MCopyProtectedMemory((void*)offset, dac_line, C_LEN_DACDB, PM_COPY_WRITE);
}

void DACGatedB(void)	//DAC门dB差
{
    int offset;
    int basegain;
    u_short Height;
    u_short gatedB;

    basegain = MGetBaseGain() + 481.31;	//基准增益
    offset = C_OFF_DACDB + MGetGatePara(0,0);	//门位
    MCopyProtectedMemory(&gatedB, (void*)offset, C_SIZE_SHORT, PM_COPY_READ);
	Height = (int) pow(10,(basegain - gatedB)/200.0) / pow(10, MGetGatedB()/200.0) ;
}


//其他通道参量有关的函数

void MSetPara(int bgain,int cgain,int sgain,int scale,int range,int offset,int delay,int angle,int speed)
{
    int Min_range=MGetSpeed()*2.5/1000+1;
    Min_range=Min_range*10;
    int mode = C_SETMODE_SETSAVE;
    if( bgain >= 0 && bgain < C_MAX_BASEGAIN)MSetBaseGain(bgain,mode);
    else MSetBaseGain(CD_BASEGAIN,mode);
    if( cgain >= 0)MSetCompGain(cgain,mode);
    else MSetCompGain(CD_COMPGAIN,mode);
    if( sgain >= 0)MSetSurfGain(sgain,mode);
    else MSetSurfGain(CD_SURFGAIN,mode);
    if( scale >= 0 && scale < 3)MSetScaleMode(scale,mode);
    else MSetScaleMode(0,mode);
    if( angle >= 0 && angle < C_MAX_ANGLE)MSetAngle(angle,mode);
    else MSetAngle(0,mode);
    if( offset >= 0 && offset < C_MAX_OFFSET)MSetOffset(offset,mode);
    else MSetOffset(0,mode);
    if (MGetUnitType()>0)
    {
        if( speed >= C_MIN_SPEED*100/25.4 && speed < C_MAX_SPEED*100/25.4)MSetSpeed(speed,mode);
        else if( MGetAngle(0) > 200 )MSetSpeed(CD_SPEED_1*100/25.4,mode);//横波
        else MSetSpeed(CD_SPEED_2*100/25.4,mode);//纵波
        if( range >= Min_range/0.254 && range < C_MAX_RANGE/0.254 )MSetRange(range,mode);
        else MSetRange(CD_RANGE/0.254,mode);
        if( delay >= 0 && delay < C_MAX_DELAY/0.254)MSetDelay(delay,mode);
        else MSetDelay(0,mode);
    }
    else
    {
        if( speed >= C_MIN_SPEED && speed < C_MAX_SPEED)MSetSpeed(speed,mode);
        else if( MGetAngle(0) > 200 )MSetSpeed(CD_SPEED_1,mode);//横波
        else MSetSpeed(CD_SPEED_2,mode);//纵波
        if( range >= Min_range && range < C_MAX_RANGE )MSetRange(range,mode);
        else MSetRange(CD_RANGE,mode);
        if( delay >= 0 && delay < C_MAX_DELAY)MSetDelay(delay,mode);
        else MSetDelay(0,mode);
    }
}

u_short MGetThick(void)//得到工件厚度
{
    return ChannelPara.Thick;
}
void MSetThick(u_short thick)//设置工件厚度
{
    ChannelPara.Thick = thick;
}

u_short MGetDiameter(void)//得到工件直径
{
    return ChannelPara.Diameter;
}
void MSetDiameter(u_short Diameter)//设置工件直径
{
    ChannelPara.Diameter = Diameter;
    if( Diameter >= 2 * MGetThick() )
    {
        MSetFunctionMode( 1,C_CURVER_AMEND);
    }
    else
    {
        MSetFunctionMode( 0,C_CURVER_AMEND);
    }
}

u_short MGetOffset(void)			/*返回值为当前通道	*/
{
    if( ChannelPara.Offset > C_MAX_OFFSET)MSetOffset(C_MAX_OFFSET,C_SETMODE_SAVE);
    return ChannelPara.Offset;
}

void MSetOffset(u_int Offset,int mode/*mode = 0\1\2*/)	/*设置当前通道	*/
{
    if (mode == C_SETMODE_SAVE || mode == C_SETMODE_SETSAVE)
    {
        ChannelPara.Offset = Offset;
    }
    if (mode == C_SETMODE_SETSAVE || mode == C_SETMODE_SET)
    {
        Offset = ( Offset + MGetDelay(0) )/2;
        if( MGetProbeMode() == C_P_TRANSMISSION)Offset /= 2;
#if C_DEVLIB == 4
        SetEchoDelay(MGetChannel(),  Offset) ;
#else
        SetEchoDelay( Offset ) ;
#endif

    }
}

u_short MGetSpeed(void)			/*返回值为当前通道试件声速：	无小数	*/
{
    if( MGetUnitType() > 0)
    {
        //单位inch
        if( ChannelPara.Speed > (C_MAX_SPEED*100/25.4+0.5))ChannelPara.Speed = C_MAX_SPEED*100/25.4+0.5;
        else if( ChannelPara.Speed < (C_MIN_SPEED*100/25.4+0.5))ChannelPara.Speed = C_MIN_SPEED*100/25.4+0.5;
    }
    else
    {
        //单位mm
        if( ChannelPara.Speed > C_MAX_SPEED)ChannelPara.Speed = C_MAX_SPEED;
        else if( ChannelPara.Speed < C_MIN_SPEED)ChannelPara.Speed = C_MIN_SPEED;
    }
    return ChannelPara.Speed;
}

void MSetSpeed(u_short Speed,int mode/*mode = 0\1\2*/)	/*设置当前通道	*/
{
    int Scale=MGetScaleMode();
    if (mode == C_SETMODE_SETSAVE || mode == C_SETMODE_SET)
    {
        MSetScaleMode(2,C_SETMODE_SETSAVE);
        float range = (float)(MGetRange(0) * Speed )/ (C_SAMPLE_FREQ);
        MSetRange( (int)(range+0.5),C_SETMODE_SAVE);
        MSetScaleMode(Scale,C_SETMODE_SETSAVE);
    }
    if (mode == C_SETMODE_SAVE || mode == C_SETMODE_SETSAVE)
    {
        ChannelPara.Speed = Speed;
    }
}

u_int MGetAngle(u_int mode)			/*返回值为当前通道折射角0角度1正切2正弦3余弦	一位小数*/
{
    u_int retvalue = ChannelPara.Angle;
    float angle = (float)retvalue * C_PI/1800.0;	//角度化为弧度
    switch (mode)
    {
    case 1://k值
		retvalue = (int)(tanf(angle) * 10000.0 + 0.5);
        break;
    case 2://水平
		retvalue = (int)(sinf(angle) * 10000.0 + 0.5);
        break;
    case 3://垂直
		retvalue = (int)(cosf(angle) * 10000.0 + 0.5);
        break;
    case 4://k值
		retvalue = (int)(sinf(angle) * 10000.0/cosf(angle) + 0.5);
        break;
    }
    return retvalue;
}
void MSetAngle(u_short Angle,int mode)	/*设置当前通道	*/
{
    ChannelPara.Angle = Angle;
}

int MGetDepth()
{
    return ChannelPara.depth;
}
void MSetDepth(int depth,int mode)
{
    ChannelPara.depth = depth;
}
u_short MGetKvalue(void)			/*返回值为当前通道K值 两位小数	*/
{
    float Angle = ChannelPara.Angle;
    if( Angle > 810 ) return 999;
    else
    {
        Angle = (ChannelPara.Angle)*C_PI/1800.0;
		return (int)(tan(Angle)*100+0.5);
    }
}
void MSetKvalue(u_short Kvalue,int mode/*mode = 0\1\2*/)	/*设置当前通道	*/
{
	ChannelPara.Angle = (int)( atan( (float)Kvalue/100.0) *1800/C_PI + 0.5);
}

u_short MGetForward(void)			/*返回值为当前通道探头前沿长度 一位小数	*/
{
    return ChannelPara.Forward;
}
void MSetForward(u_short Forward,int mode/*mode = 0*/)	/*设置当前通道	*/
{
    ChannelPara.Forward = Forward;
}

u_short MGetFrequence(void)			/*返回值为当前通道探头频率 一位小数	*/
{
    if( ChannelPara.Frequence > 0)
        return ChannelPara.Frequence;
    else return 5;
}
void MSetFrequence(u_short Frequence,int mode/*mode = 0*/)	/*设置当前通道	*/
{
    ChannelPara.Frequence = Frequence;
    if( C_DEVLIB != 3)
    {
        if( Frequence < 400 )MSetBandMode(0,C_SETMODE_SETSAVE);
        else if( Frequence < 800 )MSetBandMode(1,C_SETMODE_SETSAVE);
        else if( Frequence < 1200 )MSetBandMode(2,C_SETMODE_SETSAVE);
        else MSetBandMode(3,C_SETMODE_SETSAVE);
    }
}

u_short MGetCrystal_l(void)			/*返回值为当前通道晶片尺寸 a*256+b	*/
{
    return ChannelPara.Crystal_l;
}
u_short MGetCrystal_w(void)			/*返回值为当前通道晶片尺寸 a*256+b	*/
{
    return ChannelPara.Crystal_w;
}

void MSetCrystal_l(u_short Crystal,int mode/*mode = 0*/)	/*设置当前通道	*/
{
    ChannelPara.Crystal_l = Crystal;
}
void MSetCrystal_w(u_short Crystal,int mode/*mode = 0*/)	/*设置当前通道	*/
{
    ChannelPara.Crystal_w = Crystal;
}
int MGetNearField(double multi)		//得到近场区长度
{
    short number;
    int Near;

    if( MGetUnitType() > 0)
    {
        //单位inch
        number= MGetCrystal_l()*25.4/1000+0.5;	//晶片尺寸的低8位，如为0则是直探头，用直径计算
        if( number == 0)number = MGetCrystal_w()*25.4/1000+0.5;

        if( MGetCrystal_w() == 0)number *= MGetCrystal_l()*25.4 /1000+0.5;
        else number *= MGetCrystal_w ()*25.4/1000+0.5;
        if( number == 0)number = 25;
        multi *= number;
        Near = ( 100 * multi * MGetFrequence())/(4 * (MGetSpeed()*254/1000.0+0.5));
        return  (Near);
    }
    else
    {
        number= MGetCrystal_l()/1000;	//晶片尺寸的低8位，如为0则是直探头，用直径计算
        if( number == 0)number = MGetCrystal_w()/1000;

        if( MGetCrystal_w() <100)number *= MGetCrystal_l()/1000 ;
        else number *= MGetCrystal_w ()/1000;
        if( number == 0)number = 25;

        multi *= number;
        Near = ( 100 * multi * MGetFrequence())/(4 * MGetSpeed()+0.5 );
        return  ( Near);
    }
}
float MGetAttenuate(int depth)
{
    float att = 0;

    /*	if((MGetCrystal()/256)<15 && depth>1200)
    	{
    		att = 0.024;
    	}

    Write_Number(10,80,MGetCrystal()/256,8,0,0);
    Write_Number(10,90,depth,8,0,0);
    Write_Number(10,100,att,8,0,0);*/
    return att;
}

u_int MGetRange(int mode)			/*返回值为当前标度下的声程范围	*/
{
    if( ChannelPara.Range > C_MAX_RANGE)MSetRange(1000,C_SETMODE_SAVE);
    u_int retvalue = ChannelPara.Range ;	//
    if( mode == -1)
    {
        //展宽前声程
        retvalue = ChannelPara.OldRange ;
    }
    else if( mode == -2)
    {
        //展宽前声程
        retvalue = ChannelPara.Range ;
    }

    if(mode > 0)	//返回当前标度下声程
    {
        if( mode == 3)return retvalue;	//返回距离声程
        else if( mode == 4)return (retvalue * MGetAngle(3))/10000;		//余弦,垂直
        else if( mode == 5)return (retvalue * MGetAngle(2))/10000;		//正弦,水平
        switch ( MGetScaleMode() )
        {
        case 0:		//垂直
            retvalue = (retvalue * MGetAngle(3))/10000;		//余弦
            break;
        case 1:		//水平
            retvalue = (retvalue * MGetAngle(2))/10000;		//正弦
            break;
        case 2:		//距离
            break;
        }
    }
    else if( mode == 0)
    {
        //返回声程点
        int offset =  C_OFF_TEMP+42;
        MCopyProtectedMemory( &retvalue, (void*)offset, C_SIZE_INT, PM_COPY_READ);
    }

    if( MGetUnitType() > 0)
    {
        //单位inch
///		retvalue = (u_int)( retvalue*100/25.4+0.5);
    }
    return retvalue;
}

void MSetRange(int Range,int mode/*mode = 0\1\2*/)	/*设置当前通道	*/
{
    if( mode == -1)
    {
        //展宽前声程
        ChannelPara.OldRange = Range;
        return;
    }
    else if( mode == -2)
    {
        ChannelPara.Range = Range;
        return;
    }
    switch ( MGetScaleMode() )
    {
    case 0:		//垂直
        Range = (u_int)( Range * 10000.0 / MGetAngle(3) + 0.5);		//余弦
        break;
    case 1:		//水平
        Range = (u_int)( Range * 10000.0 / MGetAngle(2) + 0.5);		//正弦
        break;
    case 2:		//距离
        break;
    }
    if (mode == C_SETMODE_SAVE || mode == C_SETMODE_SETSAVE)
    {
        if(Range >= C_MAX_RANGE) Range = C_MAX_RANGE;
        ChannelPara.Range = Range;
    }
    if (mode == C_SETMODE_SETSAVE || mode == C_SETMODE_SET)
    {
        u_int speed = MGetSpeed();

        u_int rm = C_MAX_RANGE*90/100;
        if( MGetSpeed() < CD_SPEED_2 * 110/100 && MGetProbeMode() == C_APROBE)rm /= 2;
        if(Range >= rm) Range = rm + (Range - rm)*60/100;
        Range = (C_SAMPLE_FREQ * Range + speed/2)/speed;
        if( MGetProbeMode() == C_P_TRANSMISSION)Range >>= 1;

        MSetEchoRange(Range);




        MSetColor(C_CR_WAVE);
        int offset =  C_OFF_TEMP+42;
//        int size = C_SIZE_INT;

        MCopyProtectedMemory( (void*)offset, &Range, C_SIZE_INT, PM_COPY_WRITE);

    }
}

u_int MGetDelay(int mode)			/*返回值为当前通道	*/
{
    if( ChannelPara.Delay > C_MAX_DELAY ) MSetDelay(0,C_SETMODE_SAVE);
    u_int retvalue = ChannelPara.Delay ;	//延时时间，单位1/80 000 ms

    if( mode == -1)return ChannelPara.OldDelay;//展宽前延时
    else if( mode == -2)return ChannelPara.Delay;
    if(mode != 0)	//返回当前标度下延时距离
    {
        retvalue = retvalue * MGetSpeed() /( C_SAMPLE_FREQ );	//单位为mm，带一位小数
        if(mode == 3)return retvalue;	//此模式下直接返回延时距离
        switch ( MGetScaleMode() )
        {
        case 0:		//垂直
            retvalue = retvalue * MGetAngle(3)/10000;		//余弦
            break;
        case 1:		//水平
            retvalue = retvalue * MGetAngle(2)/10000;		//正弦
            break;
        case 2:		//距离
            break;
        }
    }

    return retvalue;
}
void MSetDelay(int Delay,int mode/*mode = 0\1\2*/)	/*设置当前通道	*/
{
    if( mode == -1 )
    {
        //展宽前延时
        ChannelPara.OldDelay = Delay;
        return;
    }
    else if( mode == -2)
    {
        ChannelPara.Delay = Delay;
        return;
    }
    if (mode == C_SETMODE_SAVE || mode == C_SETMODE_SETSAVE)
    {
        ChannelPara.Delay = Delay;
    }
    if (mode == C_SETMODE_SETSAVE || mode == C_SETMODE_SET)
    {
        MSetSystemDelay();
    }
}
void MSetScaleDelay(int Delay,int mode/*mode = 0\1\2*/)	/*设置当前标度下的延时距离	*/
{

    switch ( MGetScaleMode() )
    {
    case 0:		//垂直
        Delay = ( Delay * 10000 + MGetAngle(3)/2 ) / MGetAngle(3);		//余弦
        break;
    case 1:		//水平
        Delay = ( Delay * 10000 + MGetAngle(2)/2 ) / MGetAngle(2);		//正弦
        break;
    case 2:		//距离
        break;
    }

    //if( Delay + MGetRange(3) > C_MAX_RANGE) Delay = C_MAX_RANGE - MGetRange(3);
    Delay = ( Delay * C_SAMPLE_FREQ + MGetSpeed()/2 )/(int)MGetSpeed();

    MSetDelay(Delay,mode);
}

u_short MGetReject(void)			/*返回值为当前通道	*/
{
    if( ChannelPara.Reject < C_MAX_REJECT)return ChannelPara.Reject;
    else return C_MAX_REJECT;
}
void MSetReject(u_short Reject,int mode/*mode = 0\1\2*/)	/*设置当前通道	*/
{
    if( Reject > 800 )Reject = 800;
    if (mode == C_SETMODE_SAVE || mode == C_SETMODE_SETSAVE)
    {
        ChannelPara.Reject = Reject;
    }
    if (mode == C_SETMODE_SETSAVE || mode == C_SETMODE_SET)
    {
#if C_DEVLIB == 4
        SetEchoReject(MGetChannel(), Reject);
#else
        SetEchoReject(Reject);
#endif
    }
}

u_short MGetSysGain(void)	//得到总体增益
{
    if( ChannelPara.SurfGain + ChannelPara.BaseGain + ChannelPara.CompGain > C_MAX_SYSGAIN)
    {
        ChannelPara.SurfGain = ChannelPara.CompGain = 0;
        ChannelPara.BaseGain = 450;
    }
    return ( ChannelPara.SurfGain + ChannelPara.BaseGain + ChannelPara.CompGain );
}

u_short MGetSurfGain(void)			/*返回值为当前通道表面补偿增益 一位小数	*/
{
    if (ChannelPara.SurfGain > C_MAX_SURFGAIN)ChannelPara.SurfGain = 0;
    return ChannelPara.SurfGain;
}

int MSetSurfGain(u_short SurfGain,int mode/*mode = 0\1\2*/)	/*设置当前通道	*/
{
    if (SurfGain > C_MAX_SURFGAIN)
    {
        ChannelPara.SurfGain = 0;
        return C_FALSE;
    }
    if ( (MGetBaseGain() + MGetCompGain() + SurfGain ) > C_MAX_BASEGAIN)return C_FALSE;
    if (mode == C_SETMODE_SAVE || mode == C_SETMODE_SETSAVE)
    {
        ChannelPara.SurfGain = SurfGain;
    }
    if (mode == C_SETMODE_SETSAVE || mode == C_SETMODE_SET)
    {
        MSetSystemGain();
    }
    return C_TRUE;
}

u_short MGetBaseGain(void)			/*返回值为当前通道基本增益 一位小数	*/
{
    return ( ( ChannelPara.BaseGain > C_MAX_BASEGAIN)? C_MAX_BASEGAIN : ChannelPara.BaseGain );
}

int MSetBaseGain(short BaseGain,int mode/*mode = 0\1\2*/)	/*设置当前通道	*/
{
    if( BaseGain < 0 )BaseGain = 0;
    if( BaseGain > C_MAX_BASEGAIN )BaseGain = C_MAX_BASEGAIN ;
    if (mode == C_SETMODE_SAVE || mode == C_SETMODE_SETSAVE)
    {
        ChannelPara.BaseGain = BaseGain;
    }
    if (mode == C_SETMODE_SETSAVE || mode == C_SETMODE_SET)
    {
        u_short gain = BaseGain + MGetSurfGain() + MGetCompGain() ;
        if (gain > C_MAX_BASEGAIN)gain = C_MAX_BASEGAIN;
//		if (gain > 1000)gain = 1000 + (gain - 1000)/2;
//		if (gain > 900)gain = 900 + (gain - 900)/2;
//		if (gain > MAX_GAIN_DB)gain = MAX_GAIN_DB;
        MSetAmplifierGain(gain);
    }
    return C_TRUE;
}

short MGetGatedB(void)	/*返回值为当前通道DAC门 一位小数*/
{
    return ChannelPara.gatedB;
}

void MSetGatedB(short gatedB, int mode/*mode = 0*/) //DAC门dB差
{
    ChannelPara.gatedB = gatedB;
}

short MGetCompGain(void)			/*返回值为当前通道补偿增益 一位小数	*/
{
    return ( ( ChannelPara.CompGain > C_MAX_BASEGAIN)? C_MAX_BASEGAIN : ChannelPara.CompGain );
}

int MSetCompGain(short CompGain,int mode/*mode = 0\1\2*/)	/*设置当前通道	*/
{
    if( CompGain > C_MAX_BASEGAIN)CompGain = C_MAX_BASEGAIN;
    if (mode == C_SETMODE_SAVE || mode == C_SETMODE_SETSAVE)
    {
        ChannelPara.CompGain = CompGain;
    }
    if (mode == C_SETMODE_SETSAVE || mode == C_SETMODE_SET)
    {
        MSetSystemGain();
        u_short gain = CompGain + MGetSurfGain() + MGetBaseGain();
        if (gain > C_MAX_BASEGAIN)gain = C_MAX_BASEGAIN;
//		if (gain > 1000)gain = 1000 + (gain - 1000)/2;
//		if (gain > 900)gain = 900 + (gain - 900)/2;
//		if (gain > MAX_GAIN_DB)gain = MAX_GAIN_DB;
        MSetAmplifierGain(gain);
    }

    return 0;
}

u_short MGetDepthGain(void)
{
    return	ChannelPara.DepthGain;
}
void MSetDepthGain(u_short gain)//设置无DAC/AVG时深度补偿值
{
    if( gain > 300 )gain = 300;
    ChannelPara.DepthGain = gain;
}

short MGetLineGain(int line)//得到曲线的偏移量
{
    line %= 6;
    return ChannelPara.lineGain[line]	;
}


void MSetLineGain(int line,short gain)//设置曲线的偏移量
{
    int i;
    line %= 6;
    ChannelPara.lineGain[line] = gain ;
    for( i = line+1; i < 6 ; i++)
    {
        if( ChannelPara.lineGain[i] > gain ) ChannelPara.lineGain[i] = gain;
    }
    for( i = line-1; i >= 0 ; i--)
    {
        if( ChannelPara.lineGain[i] < gain ) ChannelPara.lineGain[i] = gain;
    }
}

void MSetSystemGain(void)	/*设置当前增益 */
{
    u_short gain = MGetBaseGain() + MGetSurfGain() + MGetCompGain() ;
//;	if (gain > 1100)gain = 1100 + (gain - 1100)/2;
//;	if (gain > 1000)gain = 1000 + (gain - 1000)/2;
//;	if (gain > C_MAX_BASEGAIN)gain = C_MAX_BASEGAIN;
    MSetAmplifierGain(gain);
}

void MSetSystemRange(void)	/*设置当前声程 */
{
    /*SAMPLE_FREQ*1000*RANGE/SPEED */
    u_int range = MGetRange(3);
    u_int speed = MGetSpeed();
    u_int rm = C_MAX_RANGE*90/100;
    if( MGetSpeed() < CD_SPEED_2 * 110/100 && MGetProbeMode() == C_APROBE)rm /= 2;
    if(range >= rm) range = rm + (range - rm)*60/100;

    range = (C_SAMPLE_FREQ * range + speed/2)/speed;
    if( MGetProbeMode() == C_P_TRANSMISSION)range >>= 1;
    MSetEchoRange( range ) ;
    MSetColor(C_CR_WAVE);
}

void MSetSystemDelay(void)	/*设置当前延时或零点 */
{
    u_int edelay = ( MGetDelay(0) + MGetOffset() ) /2;
    if( MGetProbeMode() == C_P_TRANSMISSION)edelay /= 2;
#if C_DEVLIB == 4
    SetEchoDelay(MGetChannel(), edelay ) ;
#else
    SetEchoDelay( edelay ) ;
#endif
}

void MSetSystemBright(void)	/*设置当前屏幕亮度*/
{
    u_int cur_bright = 0,obj_bright = C_BRIGHTNESS_COM;
    int mode;
    cur_bright = 0;
    obj_bright = C_BRIGHTNESS_COM;
    mode = 0;
#if C_DEVLIB == 1 || C_DEVLIB == 3 || C_DEVLIB == 23 || C_DEVLIB == 24
    switch ( MGetScreenBrightness() )
    {
    case 0:
        obj_bright = C_BRIGHTNESS_MIN;
        break;
    case 1:
        obj_bright = C_BRIGHTNESS_COM;
        break;
    case 2:
        obj_bright = C_BRIGHTNESS_MID;
        break;
    case 3:
        obj_bright = C_BRIGHTNESS_MAX;
        break;
    }
#endif
#if C_DEVLIB == 1
    do
    {
        cur_bright = AdjustBrightness(0) ;/*得到当前亮度*/
        mode = obj_bright - cur_bright;
        AdjustBrightness(mode) ;
    }
    while( mode != 0 );
#elif C_DEVLIB == 3 || C_DEVLIB == 23
//Write_Number(10,30,1,4,0,0);

    SetLCDBrightness(obj_bright) ;
#elif C_DEVLIB == 24
    SetELBrightness(obj_bright) ;
#endif

}

int MSetSystemProbe(void)
{
    int Ax = MGetProbeMode() -0 ;
    int i;
    if (Ax < C_DPROBE)
    {
        Ax = T_T_SINGLE;
        for(i=0; i<=120; i++)GainTable[i] = c_gaintable[0][i]; //初试化增益表

    }
    else
    {
        Ax = T_R_DUAL;
        for(i=0; i<=120; i++)GainTable[i] = c_gaintable[1][i]; //初试化增益表

    }
#if C_DEVLIB == 4
    SetProbeMode( MGetChannel(), Ax) ;
#else
    SetProbeMode(Ax) ;
#endif


    return C_TRUE;
}


void MSetSystem(void)		/*设置当前系统各状态*/
{

#if C_DEVLIB == 4
    SetEchoReject(MGetChannel(), MGetReject());
#else
    SetEchoReject(MGetReject());
#endif

#if C_DEVLIB != 3
#if C_DEVLIB == 4
    SetAmplifierBand( MGetChannel(), MGetBandMode() + AMP_BAND_NARROW_1);
#else
    SetAmplifierBand( MGetBandMode() + AMP_BAND_NARROW_1);
#endif
    MSetPulseMode( MGetPulseMode(), C_SETMODE_SET);
    //SetTransmitterVoltage( MGetPulseMode() + LOW_PULSE ) ;
#else
    if( MGetPulseMode() > 0)SetTransmitterVoltage(HIGH_PULSE) ;
    else SetTransmitterVoltage(LOW_PULSE) ;
#endif

#if C_DEVLIB == 4
    SetProbeDamping( MGetChannel(), MGetDampMode() + LOW_DAMPING) ;
#else
    MSetDampMode( MGetDampMode()  ,C_SETMODE_SET) ;
#endif

#if C_DEVLIB == 4
    SetEchoMode( MGetChannel(), MGetEchoMode() + FULL_WAVE ) ;
#elif C_DEVLIB == 3
    if( MGetEchoMode() == 0 )SetEchoMode( FULL_WAVE ) ;
    else SetEchoMode( RF_WAVE ) ;
#else
    SetEchoMode( MGetEchoMode() + FULL_WAVE ) ;
#if C_DEVLIB == 23 || C_DEVLIB == 24
//    if( MGetEchoMode()%4 == 3)SetEchoBaseLine( c_RFBaseLine);
//    else SetEchoBaseLine( c_FWBaseLine);
#endif
#endif

    MSetGatePara( MGetGatePara(1,0),MGetGatePara(1,1),MGetGatePara(1,2),1,C_SETMODE_SETSAVE);
    MSetGatePara( MGetGatePara(0,0),MGetGatePara(0,1),MGetGatePara(0,2),0,C_SETMODE_SETSAVE);

    MSetSystemGain();
    MSetSystemRange();
    MSetSystemDelay();
    MSetSystemBright();
    MSetSystemProbe();
}


/*

*/


