//与功能有关的函数


int GateParaMax,GateParaValue;
int SearchPeak(u_char * sampbuff,u_int curr_pos,u_int direction)	//返回最高波所在水平点数，如为负数则为无最高波
{
    //寻找sampbuff指向波形存储区的最高波，curr_pos为当前位置,direction为方向:C_KEYCOD_LEFT左C_KEYCOD_RIGHT右
    u_char preamp = *(sampbuff+curr_pos);	//前一波峰值
    u_char amp = *(sampbuff+curr_pos);	//当前波峰值

    u_short search_time = 0;	//寻找次数，不超过1
    short retvalue = 0;

    if(curr_pos <= 5 )
    {
        curr_pos = C_COORWIDTH-1;
    }
    if(direction == C_KEYCOD_RIGHT)	//右移
    {
        while(search_time <= 1)
        {
            preamp = amp;
            curr_pos++;
            if(curr_pos >= C_COORWIDTH )
            {
                curr_pos = 14;
                search_time ++;
            }
            amp = *(sampbuff+curr_pos);	//当前波峰值
            if( amp <= 5 || amp >= 200) continue;
            if( amp <= preamp)continue;
            if( amp < *(sampbuff+curr_pos+1) )continue;
            if( amp < *(sampbuff+curr_pos+2) )continue;
            retvalue = curr_pos;
            break;
        }
    }
    else if(direction == C_KEYCOD_LEFT)	//左移
    {
        while(search_time <= 1)
        {
            preamp = amp;
            curr_pos--;
            if(curr_pos <= 5 )
            {
                curr_pos = C_COORWIDTH-1;
                search_time ++;
            }
            amp = *(sampbuff+curr_pos);	//当前波峰值
            if( amp <= 5 || amp >= 200) continue;
            if( amp <= preamp)continue;
            if( amp < *(sampbuff+curr_pos-1) )continue;
            if( amp < *(sampbuff+curr_pos-2) )continue;
            retvalue = curr_pos;
            break;
        }
    }
    if(retvalue >= 5)
    {
        unsigned short dist_dire = (int)( (double)(curr_pos * MGetRange(3)+MGetRange(3)/2) / (double)C_COORWIDTH)+MGetDelay(3);
        unsigned short dist_hori = dist_dire * MGetAngle(2) / 10000;
        unsigned short dist_vert = dist_dire * MGetAngle(3) / 10000;
        short db;

        db = Amp2db( curr_pos,amp );//-MGetSurfGain() ;
        switch( MGetEquivalentMode() )	//mode1=0B门失波1进波，mode2<0波高.>0dB数
        {
        case 1:
            break;
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
            db -=  MGetLineGain( MGetEquivalentMode()-2 );
            break;
        case 10:
        case 11://显示孔径
            if( MGetUnitType() > 0 )
            {
				Write_Number(23*C_ECHAR_HDOT,C_COORVPOSI - C_ECHAR_VDOT-8,(int)( 10*powf( 10,( db + 120 ) / 400.0)*100/25.4  + 0.5), 5,3,0);
            }
            else
            {
				Write_Number(23*C_ECHAR_HDOT,C_COORVPOSI - C_ECHAR_VDOT-8,(int)( 10*powf( 10,( db + 120 ) / 400.0)  + 0.5), 3,1,0);
            }
            //	Write_Number(23*8,C_COORVPOSI - 10,(int)( 10*powf( 10,( db + 120 ) / 400.0)  + 0.5), 3,1,0);
            db -= MGetLineGain( 2 );
            break;
        default:
            db = amp * 5;
            break;
        }
        if (dist_hori >= MGetForward())
        {
            dist_hori -= MGetForward();
        }
        if( MGetThick() > 0 && MGetDiameter() == 0 && MGetSystemMode() == 0)
        {
            if( (dist_vert / MGetThick() ) % 2 == 0) dist_vert = dist_vert % MGetThick();
            else dist_vert = MGetThick() - (dist_vert % MGetThick() );
        }
        WritePeakInfo(0,MGetEquivalentMode(),dist_dire,dist_hori,dist_vert,db);
    }
    return retvalue;
}

u_int PeakMemory(void)		//峰值记忆
{
    u_int gatetype = 0 ;
    u_int retvalue ;
    u_char sampbuff[C_LEN_SAMP+1] ;
    u_char *samp ;
    u_int  offset = C_OFF_SAMP ;
//	u_int left = 0,right= 0 ;
    int i;

    if(MGetFunctionMode(C_ECHO_ENVELOPE) || MGetFunctionMode(C_ECHOMAX_MEMORY))
    {
        GateParaMax =1;
    }
    else
        GateParaMax =0;

    if( !MGetFunctionMode(C_ECHOMAX_MEMORY) )return 0;
    retvalue = MGetGateParaMax(gatetype);
    MSetColor(C_CR_ENVELOPE);
    if(retvalue == 1)
    {
        offset = C_OFF_PEAK;
        MCopyProtectedMemory( (void*)offset, &GatePeak, sizeof(GATE_PEAK), PM_COPY_WRITE);//门参数
        offset = C_OFF_SAMP ;
        MCopyProtectedMemory( sampbuff, (void*)offset, C_LEN_SAMP, PM_COPY_READ);
        DrawWave(sampbuff,0,C_LEN_SAMP);

        samp = GetSampleBuffer();
        for (i=0; i<C_COORWIDTH; i++)
        {
       //     Write_Number(10+16*4*(i%10),10+24*(i/10),samp[i],4,0,0);
            if (samp[i]>=200)
            {
                samp[i]=200;
            }
        }
         //   Write_Number(10,10,2,4,0,0);
        //    MAnyKeyReturn();
#if C_DEVTYPE != 3 && C_DEVTYPE != 9 && C_DEVTYPE != 11

        if( MGetSystemMode() )
#endif
        {
            int left,right,mid;
            for(i = 0; i < C_LEN_SAMP; i++)	sampbuff[i] = 0;
            left = GateMax.Pos;
            right = GateMax.Pos;
            mid = right;

            while(samp[left - 1] <= samp[left] && samp[left] > 0) left-- ;
            while(samp[right] >= samp[right+1] && samp[right] > 0) right++ ;

//			for( i = left; i <= right; i++)
            {
//				sampbuff[i] = samp[i];
            }
            for( i = left; i <= mid; i++)
            {
                sampbuff[i-1] = samp[i]+1;
            }
            sampbuff[mid] = samp[mid]+2;
            for( i = mid; i <= right; i++)
            {
                sampbuff[i+1] = samp[i]+1;
            }
        }
#if C_DEVTYPE != 3 && C_DEVTYPE != 9 && C_DEVTYPE != 11

        else
        {
            for(i = 0; i < C_LEN_SAMP; i++)	sampbuff[i] = samp[i];
        }
#endif

        MCopyProtectedMemory( (void*)offset, sampbuff, C_LEN_SAMP, PM_COPY_WRITE);
        DrawWave(sampbuff,0,C_LEN_SAMP);
    }
    MSetColor(C_CR_UNDO);

    GateParaValue=retvalue;
    return retvalue;
}

int DepthCompensate(void)		//深度补偿
{
    /*	if( MGetFunctionMode(C_DEPTH_COMPENSATE) == 0 ) return -1;
    	if( MGetTestStatus(C_TEST_DAC) == 0 && MGetTestStatus(C_TEST_AVG) == 0 )return 0;

    	int pos = MGetAmpPos(0) ;

    	if( MGetAmpMax(0) < 5 )
    	{
    		pos =  MGetGatePara(0,0) + MGetGatePara(0,1)/2 ;	//如A门内波太低,已A门内中间点作为补偿点
    	}
    	MSetBaseGain( MGetBaseGain() + DACDB[pos/2] - DACDB[0] , C_SETMODE_SET );
    */
    return 1;
}

int EchoEnvelope(void)	//回波包络12:02
{
    u_int gatetype = 0 ;
    u_int retvalue ;
    u_char sampbuff[C_LEN_SAMP+1] ;
    //u_char *samp ;
    int  offset;
    int high;
    int pos;	//包络点
    int start,end;	//更改开始点
    int dot;	//更改点数
    int k;	//斜率
    int i,j;
    //u_char preamp;
    u_char amp;

    if(MGetFunctionMode(C_ECHO_ENVELOPE) || MGetFunctionMode(C_ECHOMAX_MEMORY))
    {
        GateParaMax =1;
    }
    else
        GateParaMax =0;
    if( MGetFunctionMode(C_ECHO_ENVELOPE) != 1 || MGetAcquisitionEnable() != 1 )return -1;
    ///if( MGetSaveMode()==1 && MGetSaveStatus()==1 )return -1;

    pos = MGetAmpPos(gatetype);	/*得到指定门（A or B)内回波水平点数，*/
    amp = MGetAmpMax(gatetype);

    MSetColor(C_CR_ENVELOPE);
    offset = C_OFF_SAMP;
    MCopyProtectedMemory( sampbuff, (void*)offset, C_LEN_SAMP, PM_COPY_READ);
//*
    if(amp > sampbuff[pos] && amp > 20)
    {
        retvalue=MGetGateParaMax(gatetype);
        if(retvalue== C_TRUE)
        {
            offset = C_OFF_PEAK;
            MCopyProtectedMemory( (void*)offset, &GatePeak, C_LEN_PEAK, PM_COPY_WRITE);//门参数
        }

        GateParaValue=retvalue;
        DrawWave(sampbuff,0,C_LEN_SAMP);
        //DrawEnvelope(sampbuff,0,C_LEN_SAMP);

        sampbuff[pos] = amp;
        //&& sampbuff[start] > 0
        start = MGetGatePara(0, 0);
        end = start + MGetGatePara(0, 1);
        if( end >= C_COORWIDTH-1)end = C_COORWIDTH - 2;
//Write_Number(20,30,pos,4,0,0);
//Write_Number(100,30,amp,4,0,0);
        if( amp > sampbuff[pos-1] )
        {
            for( i = pos - 1, dot = pos , j = 0; i >= start && j < 4 ; i--, j++ )   // 返回值为门参数
            {
                high = sampbuff[i];
                if( high <= 1 )
                {
                    if( dot == pos)dot = i;
                    continue;
                }
                //是一个峰值
                if( high >= sampbuff[i-1] && high > sampbuff[i+1] )break;
                if( high > sampbuff[i-1] && high >= sampbuff[i+1] )break;
                if( high > amp)break;
            }
            if( i > start)dot = i;//找到一个峰值
            //else if( i == pos - 1)dot--;//后面全为0
            high = sampbuff[dot];
            k = 1000* ( amp - high )/(pos -dot) ;
//Write_Number(20,40,k,10,0,0);
//Write_Number(20,50,dot,4,0,0);
//Write_Number(100,50,high,4,0,0);
            for( i = 1; dot + i < pos; i++)
            {
                sampbuff[dot + i] = (u_char)( high + ( i * k + 0)/1000 );
            }
        }

        if( amp > sampbuff[pos+1] )
        {
            for( i = pos + 1,dot = pos, j = 0; i <= end && j < 4; i++,j++ )
            {
                high = sampbuff[i];
                if( high <= 1 )
                {
                    if( dot == pos)dot = i;
                    continue;
                }
                //是一个峰值
                if( high > sampbuff[i-1] && high >= sampbuff[i+1] ) break;
                if( high >= sampbuff[i-1] && high > sampbuff[i+1] ) break;
                if( high > amp)break;
            }
            if( i < end)dot = i;//找到一个峰值
            //else if( i == pos + 1)dot++;//后面全为0
            //high = sampbuff[pos];
            k = 1000 * ( sampbuff[dot] - amp  )/(dot - pos) ;
//Write_Number(20,60,k,10,0,0);
//Write_Number(20,70,dot,4,0,0);
//Write_Number(100,70,sampbuff[dot],4,0,0);
            for( i = 1; pos + i < dot; i++)
            {
                sampbuff[pos + i] = (u_char) ( amp + ( i * k + 0)/1000 );
            }
        }
        /*/
        		if( pos >= 2 )
        		{
        			sampbuff[pos-1] = ( sampbuff[pos-2] + amp )/2;
        			if(pos >= 3)
        			{
        				sampbuff[pos-2] = ( sampbuff[pos-3] + sampbuff[pos-1] )/2;
        				start--;
        				dots++;

        				if(pos >= 4)
        				{
        					sampbuff[pos-3] = ( sampbuff[pos-4] + sampbuff[pos-2] )/2;
        					start--;
        					dots++;
        				}
        			}
        		}
        		else if( pos <= C_COORWIDTH - 2 )
        		{
        			sampbuff[pos+1] = ( sampbuff[pos+2] + amp )/2;
        			if( pos <= C_COORWIDTH - 3 )
        			{
        				sampbuff[pos+2] = ( sampbuff[pos+3] + sampbuff[pos+1] )/2;
        				dots++;

        				if( pos <= C_COORWIDTH - 4 )
        				{
        					sampbuff[pos+3] = ( sampbuff[pos+4] + sampbuff[pos+2] )/2;
        					dots++;
        				}
        			}
        		}
        //*/
        offset = C_OFF_SAMP ;
        MCopyProtectedMemory( (void*)offset, sampbuff , end, PM_COPY_WRITE);
        DrawWave(sampbuff,0,C_LEN_SAMP);
        //DrawEnvelope(sampbuff,0,C_LEN_SAMP);

        MSetColor(C_CR_UNDO);
        return 1;
    }
    MSetColor(C_CR_UNDO);
    return 0;
}
void ClearEnvelope(void)		//清包络或清峰值
{
    int offset = C_OFF_SAMP;
    u_char sampbuff[C_LEN_SAMP+1];
    int i,start;


    GateMax.Amp = 0;	//最高波置初值为0
    MSetColor(C_CR_ENVELOPE);
//	start = 0;
//	dots = C_LEN_SAMP;
    MCopyProtectedMemory( sampbuff, (void*)offset, C_LEN_SAMP, PM_COPY_READ);
    for( start = 0 ; start <= C_LEN_SAMP; start++)
        if( sampbuff[start] > 0)break;
    if(start < C_LEN_SAMP )
    {
        //有包络或峰值记忆时才清屏幕
//		start--;
//		dots = C_LEN_SAMP - start;
        if( MGetFunctionMode(C_ECHO_ENVELOPE) )//DrawEnvelope(sampbuff,0,C_LEN_SAMP);
            DrawWave(sampbuff,0,C_LEN_SAMP);		//清包络或清峰值
        else if( MGetFunctionMode(C_ECHOMAX_MEMORY) ) DrawWave(sampbuff,0,C_LEN_SAMP);		//清包络或清峰值
//		DrawWave(sampbuff,start,dots);		//清包络或清峰值
    }
    for( i = 0 ; i <= C_LEN_SAMP; i++)sampbuff[i] = 0;
    MSetColor(C_CR_UNDO);
    MCopyProtectedMemory( (void*)offset, sampbuff, C_LEN_SAMP, PM_COPY_WRITE);
     ClearCursor(2);
}

int GateWaveExpend(int mode/*=0展宽，1缩回*/)		//门内展宽
{
    int range = MGetRange(-2);//当前所存的值
    int delay = MGetDelay(-2);
    int gateposi ;
    int gatewide ;
    if( mode == 0)
    {
        //展宽
        gateposi = MGetGatePara( 0,0);
        gatewide = MGetGatePara( 0,1);
        MSetRange(range,-1);
        MSetDelay(delay,-1);
        delay = delay + (int)( (float)( C_SAMPLE_FREQ * range )/(float)MGetSpeed() * (float)gateposi/ (float)C_COORWIDTH + 0.6);
        range = range * gatewide/C_COORWIDTH;
        MSetRange( range, -2);
        MSetDelay( delay, -2);
        MSetGatePara( 0, C_COORWIDTH, MGetGatePara(0,2), 0, C_SETMODE_SETSAVE);
    }
    else if( mode == 1)
    {
        gatewide = range * C_COORWIDTH/MGetRange(-1);
        range = MGetRange(-1);
        gateposi = (int)( (float)(delay - MGetDelay(-1) )* (float)MGetSpeed()/(float)( C_SAMPLE_FREQ * range) * (float)C_COORWIDTH + 0.6);
        delay = MGetDelay(-1);
        MSetRange( range, -2);
        MSetDelay( delay, -2);
        MSetGatePara( gateposi, gatewide, MGetGatePara(0,2), 0, C_SETMODE_SETSAVE);
    }
    return 0;
}
int MSetCurveAmend(CURVE_AMEND *stCurveAmend)//参数正确可计算返回1，否则返回0；
{
    double alpha, beta;
    double R = (double)stCurveAmend->Diameter /20.0;//传过来的是直径,且带一位小数
    double thick = (double)stCurveAmend->Thick/10.0;
    double r = R - thick;
    double angle = (double)stCurveAmend->Angle/10.0;
    double half_len_bow, half_len_arc;
    /*
    Write_Number(70,30,stCurveAmend->Diameter,5,0,0);
    Write_Number(120,30,stCurveAmend->Thick,5,0,0);
    Write_Number(170,30,10*r,5,0,0);
    Write_Number(220,30,stCurveAmend->Angle,5,0,0);
    MAnyKeyReturn();
    //*/
    if( r < 0  )
    {
        half_len_bow  = 0;
        half_len_arc = 0;
        beta = 0;
    }
    else
    {
        alpha = (double)angle * C_PI/180.0;//转化为弧度；
		if( R * sin(alpha) < r )
        {
            //回波打到内壁
            	beta = acos( R * sin(alpha) / r );
			half_len_bow = R * cos(alpha) -  r * sin(beta) ;//半弦长
            beta = C_PI/2 - alpha - beta;
        }
        	else
        {
			half_len_bow = R * cos(alpha) ;//半弦长
            //		beta = acos( (R * R + r * r - half_len_bow * half_len_bow )/( 2 * R * r) ) ;
            beta = C_PI/2 - alpha ;
        }
        half_len_arc = beta * R ;//beta是弧度，＝2*Pi*R*(angle/360)=2*Pi*R*(beta/2*Pi);
    }
    stCurveAmend->Beta = (u_short)( beta*10000+0.5);//半弦长对应的角度
    stCurveAmend->Half_Len_Bow = ( u_short)(half_len_bow*10+0.5);//半弦长
    stCurveAmend->Half_len_Arc = ( u_short)(half_len_arc*10+0.5);//半弦长对应的弧长
    /*
    Write_Number(20,40,beta*10000,5,4,0);
    Write_Number(100,40,half_len_bow*10,5,1,0);
    Write_Number(150,40,half_len_arc*10,5,1,0);
    MAnyKeyReturn();
    //*/
    return 1;
}
int MGetCurveAmend(int *dist, int *hori, int *vert, CURVE_AMEND *stCurveAmend)//返回经修正后的水平和垂直
{
    double beta = (double)stCurveAmend->Beta /10000.0;//已为弧度；
    double R = (double)stCurveAmend->Diameter /20.0;//传过来的是直径,且代一位小数
    double thick = (double)stCurveAmend->Thick/10.0;
//	double r = R - thick;
    double angle = (double)stCurveAmend->Angle * C_PI/1800.0;//转化为弧度；
    double half_len_bow = (double)stCurveAmend->Half_Len_Bow /10.0;//半弦长
//	double half_len_arc = (double)stCurveAmend->Half_len_Arc / 10.0;//半弧长
    double temp_dist;//,temp_hori,temp_vert;
    double arc, alpha, bow_hori,bow_vert = 0;
    int i;

    temp_dist = (double)(*dist)/10.0 ;//转换为浮点数
    arc = 0;
    alpha = 0;
    i = 0;
    /*
    Write_Number(20,30,10000*beta,5,0,0);
    Write_Number(70,30,20*R,5,0,0);
    Write_Number(120,30,10*thick,5,0,0);
    Write_Number(170,30,10*r,5,0,0);
    Write_Number(220,30,angle*10,5,1,0);
    Write_Number(20,40,half_len_bow*10,5,1,0);
    Write_Number(70,40,half_len_arc*10,5,1,0);

    Write_Number(20,50,temp_dist*10,5,1,0);
    //*/
    if( half_len_bow < 1)
    {
        //半弦长为0
        arc = temp_dist * MGetAngle(2) / 10000;	//水平
        bow_vert = temp_dist * MGetAngle(3) / 10000;	//垂直
        while(bow_vert > 2*thick && thick > 0)
        {
            bow_vert -= 2*thick;
        }
        if( bow_vert > thick)
        {
            bow_vert = 2*thick - bow_vert;
        }
        alpha = 0;
    }
    else
    {
        for(i = 0; i < 100 ; i++)
        {
            if( temp_dist > half_len_bow)
            {
                //距离大于半弦长
                //	arc += half_len_arc;
                alpha += beta;
                temp_dist -= half_len_bow;
//	Write_Number(20,60,alpha*10,5,0,0);
//	Write_Number(70,60,temp_dist*10,5,0,0);
                continue;
            }
            else
            {
                if( i % 2 == 0)
                {
                    //在奇数半弦
                    bow_hori = temp_dist;
					bow_vert = sqrt( R*R + bow_hori*bow_hori - 2*R*bow_hori*cos(angle) );
                    //alpha += acos(  (R*R + bow_vert*bow_vert - bow_hori*bow_hori)/(2*R*bow_vert) );
                    	alpha += asin(bow_hori * sin(angle)/bow_vert);
                }
                else
                {
                    //在偶数半弦
                    bow_hori = half_len_bow - temp_dist;
					bow_vert = sqrt( R*R + bow_hori*bow_hori - 2*R*bow_hori*cos(angle) );
                    alpha += beta;
                    //alpha -= acos(  (R*R + bow_vert*bow_vert - bow_hori*bow_hori)/(2*R*bow_vert) );
                    	alpha -= asin(bow_hori * sin(angle)/bow_vert);
                }

                bow_vert = R - bow_vert;
                arc = alpha * R;
                break;
            }
        }
    }
    *hori = (int)( arc * 10 + 0.5);
    *vert = (int)( bow_vert * 10+0.5);
    return (int)(alpha*10+0.5);

}





