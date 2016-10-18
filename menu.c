/*menu.c*/
#include <string.h>
#include <stdarg.h>
#include "Media.h"
#include "ff.h"

//#include "ding.c"
//#include "Music_alice.c"
//#include "Music_lee.c"
//#include "Music_byebye.c"

extern GATE_MAX				GateMax;		//���ڳ��ֵ���߲�
extern FUNCTION             Function;   //����
extern CACHE	cache;
extern int curr_cr;
extern FIL g_FileObject;

int iTofdMode = 0;

int KeyManage(int keycode,int mode)		//��������,mode=1renovate
{
    u_int SameKeyTime = 0;
    int mode1;			//mode1 =1������0�̰�
    int xpos,ypos;
    u_char* sampbuffer ;
    char funcNumber=10;
    int prestatus=0;
    /*
        Function.funcMenu[0]=8;
        Function.funcMenu[1]=5;
        Function.funcMenu[2]=6;
        Function.funcMenu[3]=7;
    */

    xpos = 2;

    if(keycode != C_KEYCODMAX)
    {
        SameKeyTime = TestSameKeyTime(keycode,GetElapsedTime(),C_SAMEKEYTIME);
        //   Write_Number(10,50,SameKeyTime,5,0,0);
    }
    else if( MGetSystemMode() == 0 )
    {
#if( C_ROTARY > 0 )
        keycode = MGetRotary(0);//����
        if( keycode > 0)
        {
            CompGainPlus(10);
            WriteCompGain(C_COMPGAIN_HPOSI,C_COMPGAIN_VPOSI);
        }
        else if( keycode < 0)
        {
            CompGainMinus(10);
            WriteCompGain(C_COMPGAIN_HPOSI,C_COMPGAIN_VPOSI);
        }
        else
        {
            keycode = MGetRotary(1);//����
            if( keycode > 0)
            {
                //	MGatePosi(C_KEYCOD_PLUS,0,keycode);
            }
            else if( keycode < 0)
            {
                //	MGatePosi(C_KEYCOD_MINUS,0,-1*keycode);
            }
        }
        return -1;
#endif
        SameKeyTime = 0;
    }
    //ͬһ����ʱ��
    if(SameKeyTime >= C_SAMEKEYTIME)mode1 = 1;	//mode1 =1������0�̰�
    else mode1 = 0;

    if( keycode != C_KEYCODMAX && MGetKeyLock() > 0 && !(keycode == C_KEYCOD_RETURN && mode1 == 1) )
    {
        int tm1;
        tm1 = GetElapsedTime() + 1000 *2;
        //	MPushWindow(0,0,C_BASEGAIN_HPOSI-2,C_COORVPOSI-10);
        //	MEraseWindow(0,0,C_BASEGAIN_HPOSI-2,C_COORVPOSI-10);
        if (MGetLanguage())
        {
            EMenuOut( 0, 4, _KEYLOCK_A3_E,16,1,12);
        }
        else
        {
            CEMenuOut( 0, 0,_KEYLOCK_A3,6,1,12);
        }
        while( GetElapsedTime() < tm1 )
        {
        }
        DisplayPrompt(16);
        //	MPopWindow();
        return -1;//����������-1
    }


//;	MSetColor(C_CR_WAVE);
    ///MParaRenovate(0);

//	SystemParaStore();
//	ChannelParaStore();

    switch (keycode)
    {
    case C_KEYCOD_CHANNEL:
        if( MGetSystemMode() != 1)
        {
            if( mode1 == 1)
            {
                //  RunSdCardFsTest();
#if C_DEVTYPE!=15
                g_UDiskInfo.DataHeaderMark = 1;
                StorageData(keycode);
                keycode == ChannelMenu();
                keycode = -1;
#endif
            }
            else keycode = ChannelChange();
        }
        /*
        if( mode1 == 1)
        {
        	TestGainTable();
        	break;
        }
        */
        break;
    case C_KEYCOD_BASEGAIN:
#ifndef C_KEYCOD_AUTOGAIN
        if( (MGetAmpMax(0) > 150 && MGetAmpMax(0) < 160) || (MGetAmpMax(1) > 150 && MGetAmpMax(1) < 160) )
            keycode = GainChange();
        else if( MGetSystemMode() == 0 )		//������80-160��40-80%֮�䣬�������Զ�����
        {
            //if( MGetFunctionMode(C_AUTOADJUST_GAIN) == 1 || mode1 == 1)
            if( mode1 == 1 )
            {
#if C_DEVTYPE!=20
                DisplayPrompt(18);
                DrawDac(1);
                MAdjustGain(0,0,MGetAmpStdMax(),MGetAmpStdMax());
                DrawDac(0);
                DisplayPrompt(16);
                ClearEnvelope();	//�������ֵ
                keycode = -1;
#endif
            }
            else
                keycode = GainChange();
        }
        else 		//������80-160��40-80%֮�䣬�������Զ�����
            keycode = GainChange();
#else
        keycode = GainChange();
#endif
        break;
#ifdef C_KEYCOD_AUTOGAIN
    case C_KEYCOD_AUTOGAIN:

        DisplayPrompt(18);
        DrawDac(1);
        MAdjustGain(0,0,MGetAmpStdMax(),MGetAmpStdMax());
        DrawDac(0);
        DisplayPrompt(16);
        ClearEnvelope();	//�������ֵ
        keycode = -1;
        break;
#endif
    case C_KEYCOD_OFFSET:
#if C_DEVTYPE == 3 || C_DEVTYPE == 4 || C_DEVTYPE == 20 || C_DEVTYPE == 5 || C_DEVTYPE == 8  || C_DEVTYPE == 9   || C_DEVTYPE == 11 || C_DEVTYPE == 12 || C_DEVTYPE == 13
        if( mode1 == 1)
        {
            g_UDiskInfo.DataHeaderMark = 1;
            StorageData(keycode);
            keycode == TestMenu();
            keycode = -1;
        }
        else keycode = OffsetChange();
        break;
#elif C_DEVTYPE == 2 || C_DEVTYPE == 10
        if( mode1 == 1)
        {
            keycode == RejectChange();
            keycode = -1;
        }
        else keycode = OffsetChange();
        break;
#elif C_DEVTYPE == 14
        keycode = OffsetChange();
        break;
#elif C_DEVTYPE == 15 || C_DEVTYPE == 1
//
        keycode = TestChoice(C_TEST_OFFSET);
        break;
#endif

#if C_DEVTYPE == 2 || C_DEVTYPE == 10  || C_DEVTYPE == 14
    case C_KEYCOD_TEST:
        g_UDiskInfo.DataHeaderMark = 1;
        StorageData(keycode);
        keycode == TestMenu();
        keycode = -1;
        break;
#endif
    case C_KEYCOD_SPEED:
        if( mode1 == 1)
        {
            g_UDiskInfo.DataHeaderMark = 1;
            StorageData(keycode);
            keycode == STDMenu();
            keycode = -1;
        }
        else keycode = SpeedChange();
        break;
    case C_KEYCOD_RANGE:
        keycode = RangeChange();
        break;
    case C_KEYCOD_ANGLE:
#if C_DEVTYPE == 3 || C_DEVTYPE == 11
        keycode = AngleChange();
        break;
#elif C_DEVTYPE == 2 || C_DEVTYPE == 4 || C_DEVTYPE == 20 || C_DEVTYPE == 5 || C_DEVTYPE == 8 || C_DEVTYPE == 10  || C_DEVTYPE == 12 || C_DEVTYPE == 13 || C_DEVTYPE == 14 || C_DEVTYPE == 9
        if( mode1 == 1)
        {
            g_UDiskInfo.DataHeaderMark = 1;
            StorageData(keycode);
            keycode == ColorMenu();//ParaMenu(1);
            keycode = -1;
        }
        else keycode = AngleChange();
        break;
#elif C_DEVTYPE == 15 || C_DEVTYPE == 1
        keycode = TestChoice(C_TEST_ANGLE);
        break;
#endif

#if C_DEVTYPE==15 || C_DEVTYPE == 1
    case C_KEYCOD_DAC:
        keycode = TestChoice(C_TEST_DAC);
        break;
    case C_KEYCOD_SETUP:
        g_UDiskInfo.DataHeaderMark = 1;
        StorageData(keycode);
        MenuChoice();
        keycode = -1;
        break;
#endif
#if C_DEVTYPE == 20
    case C_KEYCOD_SETUP:

        g_UDiskInfo.DataHeaderMark = 1;
        StorageData(keycode);
        if( mode1 == 1)
        {
            //�������ż������������˵��ĵڶ�ҳ���鿴���û�׼����
            ParaMenu1(2);
        }
        else
        {
            ParaMenu1(1);
        }
        keycode = -1;
        break;

#elif C_DEVTYPE == 2 || C_DEVTYPE == 3 || C_DEVTYPE == 4 || C_DEVTYPE == 5 || C_DEVTYPE == 8  || C_DEVTYPE == 9  || C_DEVTYPE == 10  || C_DEVTYPE == 11 || C_DEVTYPE == 12 || C_DEVTYPE == 13 || C_DEVTYPE == 14
    case C_KEYCOD_SETUP:
        g_UDiskInfo.DataHeaderMark = 1;
        StorageData(keycode);
        if( mode1 == 1)
        {
            //�������ż������������˵��ĵڶ�ҳ���鿴���û�׼����
            ParaMenu(2);
        }
        else ParaMenu(1);
        keycode = -1;
        break;
#endif
    case C_KEYCOD_GATE:
        if( MGetFunctionMode( C_WAVE_EXPEND	) && MGetGatePara(0,0) > 1)
        {
            /*			GateWaveExpend(0);
            			MSetSystemRange();	//���õ�ǰ����
            			MSetSystemDelay();
            			MChannelRenovate();
            			MDrawGate(-1,0,-1,0);
            			DrawDac(0);
            			//WriteRange(C_RANGE_HPOSI,C_RANGE_VPOSI);
            			//WriteScale();
            			MKeyRlx();
            			while(true)
            			{
            				MParaRenovate(0);
            				if( MGetKeyCode(10) == C_KEYCOD_GATE) break;
            			}
            			GateWaveExpend(1);
            			MSetSystemRange();	//���õ�ǰ����
            			MSetSystemDelay();
            */
            int range = MGetRange(-2);//��ǰ�����ֵ
            int delay = MGetDelay(-2);
            int gateposi ;
            int gatewide ;
            int elapsedtime= GetElapsedTime();
            ChannelParaStore();
            gateposi = MGetGatePara( 0,0);
            gatewide = MGetGatePara( 0,1);
            MSetDelay( delay + (int)( (float)( C_SAMPLE_FREQ * range )/(float)MGetSpeed() * (float)gateposi/ (float)C_COORWIDTH + 0.6), -2);
            MSetRange( range * gatewide/C_COORWIDTH, -2);
            MSetGatePara( 5, C_COORWIDTH-10, MGetGatePara(0,2), 0, C_SETMODE_SETSAVE);
            MSetSystemRange();	//���õ�ǰ����
            MSetSystemDelay();
            MChannelRenovate();
			MDrawGate(-1,0,-1,0);
            DrawDac(0);
            MKeyRlx();
            g_UDiskInfo.DataHeaderMark = 1;
            while(true)
            {
                keycode=MGetKeyCode(0);
                StorageData(C_KEYCODMAX);
                //	if (GetElapsedTime() >= elapsedtime)
                {
                    //	elapsedtime = GetElapsedTime() + 200;
                    MParaRenovate(1);
                }
                if( keycode == C_KEYCOD_GATE) break;
            }

//			MSetRange( range, -2);
//			MSetDelay( delay, -2);
//			MSetGatePara( gateposi, gatewide, MGetGatePara(0,2), 0, C_SETMODE_SETSAVE);
//			MSetSystemRange();	//���õ�ǰ����
//			MSetSystemDelay();
            g_UDiskInfo.DataHeaderMark = 1;
            ChannelParaRestore();
            MSetSystem();
            MChannelRenovate();
            DrawDac(0);
            DisplayPrompt(16);
            MKeyRlx();
            keycode = -1;
        }
        //*/

        else
        {
            MSetParaSort( MGetParaSort(C_AGATEPOSI) %3 ,C_AGATEPOSI) ;
            keycode = GateChange();
        }
        break;
#if C_DEVTYPE == 2 || C_DEVTYPE == 10  || C_DEVTYPE == 14
    case C_KEYCOD_GATEB:
        MSetParaSort( 4, C_BGATEPOSI);
        keycode = GateChange();
        break;
#endif
    case C_KEYCOD_RETURN:

        ClearEnvelope();	//�������ֵ

//SetScanRotaryEncoder(1, 1, 0, 1) ;
//SetScanRotaryEncoder(0, 1, 0, 1) ;

        if(0)
        {
            int tm1,tm2;
            MKeyRlx();
            ClearEnvelope();
            tm1 = GetElapsedTime() + 1000 *3;
            tm2 = MGetKeyLock();
            //	MPushWindow(0,0,C_BASEGAIN_HPOSI-2,C_COORVPOSI-10);
            //	MEraseWindow(0,0,C_BASEGAIN_HPOSI-2,C_COORVPOSI-10);
            if( MGetKeyLock() > 0)
            {
                if (MGetLanguage())
                {
                    EMenuOut( 0, 4, _KEYLOCK_A2_E,13,1,12);
                }
                else
                {
                    CEMenuOut( 0, 0,_KEYLOCK_A2,6,1,12);
                }
            }
            else
            {
                if (MGetLanguage())
                {
                    EMenuOut( 0, 4, _KEYLOCK_A1_E,13,1,12);
                }
                else
                {
                    CEMenuOut( 0, 0,_KEYLOCK_A1,6,1,12);
                }
            }
            while( GetElapsedTime() < tm1 )
            {
                keycode = MGetKeyCode(10);
                if(keycode == C_KEYCOD_POINT)
                {
                    //�̰�ȷ��������Լ��ʱ���ڰ��˵��
                    MSetKeyLock( tm2 + 1,C_SETMODE_SAVE);
                    break;
                }
            }
            DisplayPrompt(16);
            ClearEnvelope();
            //	MPopWindow();
            if ( tm2 != MGetKeyLock() )
            {
                MChannelRenovate();
                DrawDac(1);
            }

            MKeyRlx();


        }
        else if( stWeldPara.type > 0 )//�������ؼ�,���к�������
        {
            ShowWeld(20,58+240,520,160,MGetAngle(0), MGetAmpDist1(0) );
            ScreenRenovate();
            DrawDac(0);
        }
        ExpendTime(50);
        keycode = -1;
        break;

    case C_FUNC_1:  //���ܿ�ݼ�F1-F4
//SetScanRotaryEncoder(1, 0, 0, 1) ;
//SetScanRotaryEncoder(0, 0, 0, 1) ;
        funcNumber=0;
        keycode = -1;
        break;
    case C_FUNC_2:
//    WavePlayback((WavHeader*) byebye, 100) ;
//    WaveVolume(80) ;
        funcNumber=1;
        keycode = -1;
        break;
    case C_FUNC_3:
//    WavePlayback((WavHeader*) lee, 100) ;
//    WaveVolume(100) ;
        funcNumber=2;
        keycode = -1;
        break;
    case C_FUNC_4:
//    WavePlayback((WavHeader*) ding, 100) ;
//    WaveVolume(10) ;
        funcNumber=3;
        keycode = -1;
        break;
    case C_KEYCOD_CONFIRM:

        //WavePlayback((WavHeader*) ding, 30) ;
//    WavePlayback((WavHeader*) alice, 100) ;
//    WaveVolume(50) ;
        // SetScanRotaryEncoder(1, 0, 1, 1) ;
        // SetScanRotaryEncoder(0, 0, 1, 1) ;
#if C_DEVTYPE == 4 || C_DEVTYPE == 20
        keycode = -1;
        break;
#endif

        if( mode1 == 1)
        {
            break;
            if( MGetAmpMax(0) < 5)
            {
                keycode = -1;
                break;
            }
            mode1 = MGetBGateMode();
            MSetBGateMode(0,C_SETMODE_SETSAVE);//����B��Ϊʧ��,������ԭ��״̬
            MChannelRenovate();
            DrawDac(1);
            ClearEnvelope();
            MSetAcquisitionEnable(0,C_SETMODE_SETSAVE);
            sampbuffer = GetSampleBuffer();
            ClearCursor(2);
            if( keycode != C_KEYCOD_CONFIRM )
            {
                keycode = MGetKeyCode(10);
                if(keycode == C_KEYCODMAX)
                {
                    keycode = MGetRotary(-1);
                    if( keycode > 0 )keycode = C_KEYCOD_LEFT;
                    else if( keycode < 0)keycode = C_KEYCOD_RIGHT;
                    else keycode = C_KEYCODMAX;
                }
            }
            else
            {
                xpos = GateMax.Pos;
                keycode = C_KEYCOD_RIGHT;
            }
            if(keycode == C_KEYCOD_LEFT)xpos++;
            else if(keycode == C_KEYCOD_RIGHT)xpos--;	//ʹ�õ�ǰ��߲��ܵ�һ�α�ѡ��


            do
            {
                if(keycode == C_KEYCOD_LEFT || keycode == C_KEYCOD_RIGHT)
                {
                    ClearCursor(2);
                    xpos = SearchPeak(sampbuffer,xpos,keycode);
                    if( xpos < 0)break ;
                    ypos = C_COORVPOSI + C_COORHEIGHT - 2 - (*(sampbuffer + xpos))*2;
                    DrawCursor(xpos,ypos,2);
                }
                else if(keycode == C_KEYCOD_RETURN)
                {
                    ClearCursor(2);
                    break;
                }
                keycode = MGetKeyCode(10);
                if(keycode == C_KEYCODMAX)
                {
                    keycode = MGetRotary(-1);
                    if( keycode > 0 )keycode = C_KEYCOD_LEFT;
                    else if( keycode < 0)keycode = C_KEYCOD_RIGHT;
                    else keycode = C_KEYCODMAX;
                }
                ExpendTime(20);
            }
            while( xpos > 0 );	//��SearchPeak����-1ʱ��ʾδ���ҵ���߲�
            MSetGateParaInit();	//������߲����ֵ
            MSetAcquisitionEnable(1,C_SETMODE_SETSAVE);
            MSetBGateMode(mode1,C_SETMODE_SETSAVE);//�ָ�B������
            MChannelRenovate();
            DrawDac(1);
        }
        else//�̰�
        {
            /*    Usb2PC();
                int i,rev;
                char out[100];
                for(i=0;i<64;i++)out[i]=i;
                rev=UsbDevDataOut(out,64);
                Write_Number(50,100,rev,5,0,0);
                MAnyKeyReturn();
            */
            /*         int FileMax=(C_OFF_FILE+4095)/4096*4096;//C_OFF_FILEMAX/4;//
                     int a[FileMax];
                     char b[FileMax];
                     char c[FileMax];
                     int i,j,rev;
                     while(1)
                     {
                        for(i=0;i<FileMax;i++)
                         {
                             a[i]=i;
                         }
                        for(i=0;i<FileMax;i++)
                         {
                            // b[i]=2;
                             c[i]=MEMORY_STORAGE[i];
                         }

                         for(i=0,j=0;i<145;i++)
                         {
                             if(i%5==0 && i>0)j++;
                             Write_Number((i%10)*112,10+j*16,c[C_OFF_CHANNEL-i],7,0,0);
                         }
                         Write_Number(10,0,1,3,0,0);
                         Write_Number(30,0,rev,3,0,0);
                     keycode=MAnyKeyReturn();

                     int offset = C_OFF_INIT;

                         for(i=0;i<(FileMax/4096);i++){
                             rev=CopyProtectedMemory((void*)offset+i*4096, c+i*4096,4096, PM_COPY_WRITE);
                             ExpendTime(2);
                         }

                        for(i=0;i<FileMax;i++)
                         {
                            // b[i]=2;
                             c[i]=MEMORY_STORAGE[i];
                         }

                         MEMORY_STORAGE[C_OFF_CHANNEL]=18;
                         for(i=0,j=0;i<145;i++)
                         {
                             if(i%5==0 && i>0)j++;
                             Write_Number((i%10)*112,10+j*16,MEMORY_STORAGE[C_OFF_CHANNEL-i],7,0,0);
                         }
                         Write_Number(10,0,2,3,0,0);
                         Write_Number(30,0,rev,3,0,0);
                     keycode=MAnyKeyReturn();

                         for(i=0;i<(FileMax/4096);i++){
                            rev=CopyProtectedMemory(MEMORY_STORAGE+i*4096, (void*)offset+i*4096,4096, PM_COPY_READ);
                             ExpendTime(2);
                         }

                         for(i=0,j=0;i<145;i++)
                         {
                             if(i%5==0 && i>0)j++;
                             Write_Number((i%10)*112,10+j*16,MEMORY_STORAGE[C_OFF_CHANNEL-i],7,0,0);
                         }
                             Write_Number(10,0,3,3,0,0);
                         Write_Number(30,0,rev,3,0,0);
                     keycode=MAnyKeyReturn();
                         for(i=0,j=0;i<FileMax;i++)
                         {
                             if(c[i]!=MEMORY_STORAGE[i]){
                                 j++;
                             //    Write_Number(30,j*20,c[i],7,0,0);
                             }
                         }
                             Write_Number(10,0,4,1,0,0);
                             Write_Number(30,0,j,4,0,0);
                     keycode=MAnyKeyReturn();
                     if(keycode==C_KEYCOD_RETURN)break;


                     }
            */
            if(MGetTestStatus(C_TEST_DAC) == 1)
            {
                MSetDelay(0,C_SETMODE_SETSAVE);
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
                DrawDac(0);		//�»�DAC
                if( MGetThick()*2 <= MGetRange(4) && MGetStdMode()!=11 )//if( MGetThick()*2 <= MGetRange(4) && MGetStdMode()<C_STD_MAX-1 )
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
                    else if (MGetStdMode()==12)	//JB47013
                    {
                        MSetBaseGain( MGetBaseGain()-200*log10( dac_line[MGetThick()*2*C_COORWIDTH/MGetRange(4)]/80.0),C_SETMODE_SETSAVE);

                    }
                    else if (MGetStdMode()==7||MGetStdMode()==4)	//CBT3559-94
                    {
                        //MSetBaseGain( MGetBaseGain()-200*log10( dac_line[2*C_COORWIDTH/MGetRange(4)]/160.0),C_SETMODE_SETSAVE);
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
                MSetSystem();
                SystemParaStore();
                ChannelParaStore();
                ScreenRenovate();
                DrawDac(0);

            }
        }
        keycode = -1;
        break;
    case C_KEYCOD_SAVE:
        if( MGetSystemMode() != 1)
        {
            if( stWeldPara.type > 0 && !MGetSaveMode())
            {
                //�к���/��������
                if( ShowWeld(20,58+240,520,160,MGetAngle(0), MGetAmpDist1(0) ) == 0)
                {
                    //���˷��ؼ�
                    keycode = -1;
                    break;
                }
            }

            GetDispEchoPara();/*�õ�ָ���ţ�0=A or 1=B)�ڻز���λ�ô�С��д����Ļ��*/
            MSetAcquisitionEnable(0,C_SETMODE_SETSAVE);
            WaveSave(mode1);
            MSetAcquisition(1);
            DisplayPrompt(16);
        }
        //if( MGetSystemMode() != 1)DataMenu(1);
        keycode = -1;
        break;
    case C_KEYCOD_DELAY:
        keycode = DelayChange();
        break;
#if  !(C_DEVTYPE == 14 ||C_DEVTYPE == 15||C_DEVTYPE == 1)
    case C_KEYCOD_PLUS:
//	case C_KEYCOD_RIGHT:
        g_UDiskInfo.DataHeaderMark = 1;
        StorageData(keycode);
        ClearEnvelope();	//�������ֵ
        DisplayPrompt(18);
        DrawDac(1);
        MAdjustGain(0,0,MGetAmpStdMax(),MGetAmpStdMax());
        DrawDac(0);
        DisplayPrompt(16);
        keycode = -1;
        break;
#endif
#if C_DEVTYPE == 14
    case C_KEYCOD_MINUS:
#endif
    case C_KEYCOD_LEFT:
        keycode = -1;
        if(extend==0)
        {
            int key;
            int nPulseWidth = 0;

            //   SystemParaRestore();
            //   ChannelParaRestore();
            //   MSetSystemMode(0,0);	//���ó�һ��״̬
            //   MSetBaseGain(400,C_SETMODE_SETSAVE);

            //   MSetSystem();
            //   MSetAcquisition(1);

            //   ScreenRenovate();	/*��Ļˢ��*/

            nPulseWidth = MGetPulseWidthNumber();	//33
            ETextOut(320,10,"PulseWidth:  ",11);
            Write_Number(320+C_ECHAR_HDOT*11,10,nPulseWidth,4,0,0);
            ExpendTime(20);
            MKeyRlx();
            while(true)
            {

                MParaRenovate(1);
                key = MGetKeyCode(0);
                if( key == C_KEYCODMAX)continue;
                if( key == C_KEYCOD_RETURN)
                {
                    ScreenRenovate();	/*��Ļˢ��*/
                    break;
                }
                if( key == C_KEYCOD_PLUS || key == C_KEYCOD_MINUS)
                {
                    if( key == C_KEYCOD_PLUS )nPulseWidth++;
                    else if( key == C_KEYCOD_MINUS )nPulseWidth--;
                    SetPulseWidth(nPulseWidth);
                    MSetPulseWidthInput(nPulseWidth, C_SETMODE_SETSAVE);
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
        break;
        if( mode1 == 1)
        {
            //�����Ӻż�����������
#if C_LIGHT == 1
#if C_DEVLIB == 1  || C_DEVLIB == 3|| C_DEVLIB == 24 || C_DEVLIB == 23
            int bright;
            //	CEMenuOut(0,0,_Bright_A1,7,1,16);
            TextOut(0,0,1,18,16,(u_char*)_Bright_A1[MGetLanguage()][0],4);

            MKeyRlx();
            bright = 0;
            while( true)
            {
                keycode = MGetKeyCode(0);

                if( keycode == C_KEYCOD_RIGHT )
                {
                    MSetAcquisitionEnable(0,C_SETMODE_SETSAVE);
                    MSetScreenBrightness( (MGetScreenBrightness()+1)%4,C_SETMODE_SETSAVE);
                    ExpendTime(20);
                    MSetAcquisitionEnable(1,C_SETMODE_SETSAVE);
                }
                else if( keycode == C_KEYCOD_LEFT )
                {
                    MSetAcquisitionEnable(0,C_SETMODE_SETSAVE);
                    MSetScreenBrightness( (MGetScreenBrightness()-1)%4,C_SETMODE_SETSAVE);
                    ExpendTime(20);
                    MSetAcquisitionEnable(1,C_SETMODE_SETSAVE);
                }
                else if( keycode != C_KEYCODMAX)break;
                //	MKeyRlx();
            }
            DisplayPrompt(16);
#else  // C_DEVLIB == 23
            int bright;
            //	CEMenuOut(0,0,_Bright_A1,7,1,16);
            TextOut(0,0,1,18,16,(u_char*)_Bright_A1[MGetLanguage()][0],4);
            MKeyRlx();
            bright = 0;
            while( true)
            {
                keycode = MGetKeyCode(0);

                if( keycode == C_KEYCOD_RIGHT || keycode == C_KEYCOD_LEFT )
                {
                    if (MGetScreenBrightness()==0)
                    {
                        MSetAcquisitionEnable(0,C_SETMODE_SETSAVE);
                        MSetScreenBrightness(3,C_SETMODE_SETSAVE);
                        ExpendTime(20);
                        MSetAcquisitionEnable(1,C_SETMODE_SETSAVE);
                    }
                    else
                    {
                        MSetAcquisitionEnable(0,C_SETMODE_SETSAVE);
                        MSetScreenBrightness(0,C_SETMODE_SETSAVE);
                        ExpendTime(20);
                        MSetAcquisitionEnable(1,C_SETMODE_SETSAVE);
                    }
                }
                //
                else if( keycode != C_KEYCODMAX)break;

                //MKeyRlx();
                ExpendTime(20);
            }
            DisplayPrompt(16);
#endif
#endif
        }
        else
        {
            //�̰��Ӻż�������ɨ������ʹ����80��
            ///	MAdjustGain(1,0,MGetAmpStdMax(),MGetAmpStdMax());
#if C_FILL == 1
            //   MSetFill(MGetFill()+1,C_SETMODE_SETSAVE);
#endif
        }
        keycode = -1;
        break;///�ݲ���������Ĳ������������
        MKeyRlx();
        ExpendTime(100);
        keycode = MGetKeyCode(10);
        MKeyRlx();
        if( keycode == C_KEYCOD_RIGHT)
        {
            ExpendTime(100);
            if( MGetKeyCode(10) == C_KEYCOD_LEFT)
            {
///				TestGainTable();
            }
        }
        else if( keycode == 7 )
        {
            ExpendTime(100);
            if( MGetKeyCode(10) == C_KEYCOD_LEFT)
            {
///				TestGainTable1();
            }
        }
        else
        {
            ExpendTime(100);
            if( MGetKeyCode(10) == C_KEYCOD_LEFT)
            {
///				if( keycode == 0 )keycode = 10;
///				TestGain(keycode);
            }
        }

        keycode = -1;
        break;
    case C_KEYCODMAX:
        keycode = C_KEYCODMAX;
        break;
    default :
        keycode = -1;
        break;
    }
/////////////�����ù���
    /*#define C_KEYCOD_FUNCTION	16     //���÷�ֵ����
    #define C_KEYCOD_ENVELOPE	1      //�ز�����
    #define C_KEYCOD_FREEZ	    2      //�ز�����
    #define C_KEYCOD_DATA		3*/
    if(funcNumber<4)
    {
        switch (Function.funcMenu[funcNumber])
        {
        case C_FUNC_INIT:             //��ʼ��

            MSetAcquisition(0);
            if (MGetSaveMode()==1 && MGetSaveStatus()==1 )
            {
                break;
            }
            InitPara();//��ʼ��
            MSetAcquisition(1);
            ScreenRenovate();	/*��Ļˢ��*/
            DrawDac(0);		//�»�DAC
            break;
        case C_FUNC_ENVELOPE:	      //�ز�����

            DisplayPrompt(16);
            g_UDiskInfo.DataHeaderMark = 1;
            StorageData(keycode);
            ClearEnvelope();
            if(MGetFunctionMode(C_ECHO_ENVELOPE) == 0)
            {
                MSetFunctionMode(1,C_ECHO_ENVELOPE);
                MSetFunctionMode(0,C_ECHOMAX_MEMORY);
                MSetSaveFrequency(0,0);
            }
            else if (MGetFunctionMode(C_ECHO_ENVELOPE) == 1)
            {
                MSetFunctionMode(0,C_ECHO_ENVELOPE);
                MSetFunctionMode(0,C_ECHOMAX_MEMORY);
            }
            SysParaDisp();
            break;
        case C_FUNC_ECHOMAX:	          //��ֵ����
            DisplayPrompt(16);
            g_UDiskInfo.DataHeaderMark = 1;
            StorageData(keycode);
            ClearEnvelope();
            if(MGetFunctionMode(C_ECHOMAX_MEMORY) == 0)
            {
                MSetFunctionMode(0,C_ECHO_ENVELOPE);
                MSetFunctionMode(0,C_GATE_ALARM);
                MSetFunctionMode(1,C_ECHOMAX_MEMORY);
                MSetSaveFrequency(0,0);
            }
            else if (MGetFunctionMode(C_ECHOMAX_MEMORY) == 1)
            {
                MSetFunctionMode(0,C_ECHO_ENVELOPE);
                MSetFunctionMode(0,C_ECHOMAX_MEMORY);
            }
            SysParaDisp();
            break;

        case C_FUNC_DACGATE:	          //DAC��

            prestatus = MGetFunctionMode(C_DAC_GATE);	//ԭ״̬
            MSetFunctionMode( prestatus+1,C_DAC_GATE);
            if( MGetFunctionMode(C_DAC_GATE) == 1 )
            {
                if( MGetTestStatus(C_TEST_DAC) == 0 && MGetTestStatus(C_TEST_AVG) == 0)
                {
                    MSetFunctionMode(0,C_DAC_GATE);
                }
                if( MGetFunctionMode(C_DEPTH_COMPENSATE) == 1 && MGetFunctionMode(C_DAC_GATE) == 1)
                {
                    //  InvertWords(inv_xpos, inv_ypos + (C_DEPTH_COMPENSATE + 1)* crow_height-2,1 ); /*������ʾ����*/
                    MSetFunctionMode(0,C_DEPTH_COMPENSATE);
                }
                if( MGetFunctionMode(C_DAC_GATE) == 1)
                {
                    int dac_dB;

                    dac_dB = MGetDacDb();	//dacdB����DACĸ����Ϊ����dBֵ
                    MSetGatedB(11, 0);
                    DacGateInit();
                    //MSetGatePara( MGetGatePara(0,0),MGetGatePara(0,1),DacGate_dBHigh(11,dac_dB),0,C_SETMODE_SETSAVE);
                }
            }
            else if( prestatus == 1)
            {
                //��DAC�Ÿĳ���ͨ��
                MSetGatePara( MGetGatePara(0,0),MGetGatePara(0,1),160,0,C_SETMODE_SETSAVE);
            }
            ScreenRenovate();	/*��Ļˢ��*/
            DrawDac(0);
            break;
        case C_FUNC_DACADJUST:	      //��Ȳ���

            if( MGetFunctionMode(C_DAC_GATE) == 1 && prestatus == 0)
            {
                //   InvertWords(inv_xpos, inv_ypos + (C_DAC_GATE + 1)* crow_height-2,1 ); /*������ʾ����*/
                MSetFunctionMode(0,C_DAC_GATE);

                MSetGatePara( MGetGatePara(0,0),MGetGatePara(0,1),160,0,C_SETMODE_SETSAVE);
            }
            if( MGetEquivalentDays(1) <= 0 || MGetEchoMode() == C_RF_WAVE)	//���ز���״̬<0=δ��>=0�Ѳ⣬
            {
                MSetFunctionMode( 0,C_DEPTH_COMPENSATE);
            }
            else
            {
                prestatus = MGetFunctionMode(C_DEPTH_COMPENSATE);	//ԭ״̬
                MSetFunctionMode( prestatus+1,C_DEPTH_COMPENSATE);
            }
            ScreenRenovate();	/*��Ļˢ��*/
            DrawDac(0);
            break;
        /*
		case C_FUNC_GATEEXPEND:        //����չ��
            if( MGetGatePara(0,0) > 1)
            {
                prestatus = MGetFunctionMode(C_WAVE_EXPEND);	//ԭ״̬
                MSetFunctionMode( prestatus+1,C_WAVE_EXPEND);
                int range = MGetRange(-2);//��ǰ�����ֵ
                int delay = MGetDelay(-2);
                int gateposi ;
                int gatewide ;
                int elapsedtime= GetElapsedTime();
                ChannelParaStore();
                gateposi = MGetGatePara( 0,0);
                gatewide = MGetGatePara( 0,1);
                MSetDelay( delay + (int)( (float)( C_SAMPLE_FREQ * range )/(float)MGetSpeed() * (float)gateposi/ (float)C_COORWIDTH + 0.6), -2);
                MSetRange( range * gatewide/C_COORWIDTH, -2);
                MSetGatePara( 5, C_COORWIDTH-10, MGetGatePara(0,2), 0, C_SETMODE_SETSAVE);
                MSetSystemRange();	//���õ�ǰ����
                MSetSystemDelay();
                MChannelRenovate();
                //MDrawGate(-1,0,-1,0);
				MDrawGate(-1,0,-1,0);
                DrawDac(0);
                MKeyRlx();
                g_UDiskInfo.DataHeaderMark = 1;
                while(true)
                {
                    keycode=MGetKeyCode(0);
                    StorageData(C_KEYCODMAX);
                    MParaRenovate(1);
                    if( keycode == C_KEYCOD_GATE) break;
                    else if(keycode == C_KEYCOD_RETURN || (funcNumber==0&&keycode==C_FUNC_1) ||(funcNumber==1&&keycode==C_FUNC_2)
                            ||(funcNumber==2&&keycode==C_FUNC_3) ||(funcNumber==3&&keycode==C_FUNC_4) )
                    {
                        break;
                    }
                }
                g_UDiskInfo.DataHeaderMark = 1;
                ChannelParaRestore();
                MSetSystem();
                MChannelRenovate();
                DrawDac(0);
                // InvertBytes(400+24+32*funcNumber,7, 1);
                // MInvertWindow( 512+32*funcNumber,C_COORVPOSI + C_COORHEIGHT-32,400+24+32*funcNumber+24,7+24) ;
                MInvertWindow( 504+32*funcNumber,C_COORVPOSI + C_COORHEIGHT-35,504+32*funcNumber+24,C_COORVPOSI + C_COORHEIGHT-35+24) ;
                DisplayPrompt(16);
                keycode = -1;
                MKeyRlx();
            }
            break;
		*/
        case C_FUNC_GATEALARM:	      //���ڱ���
            prestatus = MGetFunctionMode(C_GATE_ALARM);	//ԭ״̬
            MSetFunctionMode( prestatus+1,C_GATE_ALARM);

            if(MGetFunctionMode(C_GATE_ALARM) == 1)
            {
                ClearEnvelope();
                MSetFunctionMode(0,C_ECHOMAX_MEMORY);
            }
            SysParaDisp();
            break;
        case C_FUNC_DATA:		      //���ݴ���
            if (MGetSaveStatus() <= 0)
            {
                // InvertBytes(400+24+32*funcNumber,7, 1);
                //MInvertWindow( 512+32*funcNumber,C_COORVPOSI + C_COORHEIGHT-32,400+24+32*funcNumber+24,7+24) ;
                MInvertWindow( 504+32*funcNumber,C_COORVPOSI + C_COORHEIGHT-35,504+32*funcNumber+24,C_COORVPOSI + C_COORHEIGHT-35+24) ;
                if( MGetSystemMode() != 1)
                {
                    MSetAcquisitionEnable(0,C_SETMODE_SETSAVE);
                    DataMenu(mode1);
                    MSetSystem();
                    MSetAcquisitionEnable(1,C_SETMODE_SETSAVE);
                    DisplayPrompt(16);
                }
                //if( MGetSystemMode() != 1)DataMenu(1);
                MKeyRlx();
                ExpendTime(20);
            }
            break;
        case C_FUNC_FREEZ:	          //���ζ���
            if( MGetAmpMax(0) >= 5)
            {
                // InvertBytes(400+24+32*funcNumber,7, 1);
                // MInvertWindow( 512+32*funcNumber,C_COORVPOSI + C_COORHEIGHT-32,400+24+32*funcNumber+24,7+24) ;
                MInvertWindow( 504+32*funcNumber,C_COORVPOSI + C_COORHEIGHT-35,504+32*funcNumber+24,C_COORVPOSI + C_COORHEIGHT-35+24) ;
                mode1 = MGetBGateMode();
                MSetBGateMode(0,C_SETMODE_SETSAVE);//����B��Ϊʧ��,������ԭ��״̬
                // MChannelRenovate();
                // DrawDac(1);
                ClearEnvelope();
                MSetAcquisitionEnable(0,C_SETMODE_SETSAVE);
                sampbuffer = GetSampleBuffer();
                ClearCursor(2);
                if( keycode != C_KEYCOD_CONFIRM )
                {
                    keycode = MGetKeyCode(10);
                    if(keycode == C_KEYCODMAX)
                    {
                        keycode = MGetRotary(-1);
                        if( keycode > 0 )keycode = C_KEYCOD_LEFT;
                        else if( keycode < 0)keycode = C_KEYCOD_RIGHT;
                        else keycode = C_KEYCODMAX;
                    }
                }
                else
                {
                    xpos = GateMax.Pos;
                    keycode = C_KEYCOD_RIGHT;
                }
                if(keycode == C_KEYCOD_LEFT)xpos++;
                else if(keycode == C_KEYCOD_RIGHT)xpos--;	//ʹ�õ�ǰ��߲��ܵ�һ�α�ѡ��


                do
                {
                    if(keycode == C_KEYCOD_LEFT || keycode == C_KEYCOD_RIGHT)
                    {
                        ClearCursor(2);
                        xpos = SearchPeak(sampbuffer,xpos,keycode);
                        if( xpos < 0)break ;
                        ypos = C_COORVPOSI + C_COORHEIGHT - 2 - (*(sampbuffer + xpos))*2;
                        DrawCursor(xpos,ypos,2);
                    }
                    else if(keycode == C_KEYCOD_RETURN || (funcNumber==0&&keycode==C_FUNC_1) ||(funcNumber==1&&keycode==C_FUNC_2)
                            ||(funcNumber==2&&keycode==C_FUNC_3) ||(funcNumber==3&&keycode==C_FUNC_4) )
                    {
                        // InvertBytes(400+24+32*funcNumber,7, 1);
                        MInvertWindow( 504+32*funcNumber,C_COORVPOSI + C_COORHEIGHT-35,504+32*funcNumber+24,C_COORVPOSI + C_COORHEIGHT-35+24) ;
                        ClearCursor(2);
                        break;
                    }
                    keycode = MGetKeyCode(10);
                    if(keycode == C_KEYCODMAX)
                    {
                        keycode = MGetRotary(-1);
                        if( keycode > 0 )keycode = C_KEYCOD_LEFT;
                        else if( keycode < 0)keycode = C_KEYCOD_RIGHT;
                        else keycode = C_KEYCODMAX;
                    }
                    ExpendTime(20);
                }
                while( xpos > 0 );	//��SearchPeak����-1ʱ��ʾδ���ҵ���߲�
                MSetGateParaInit();	//������߲����ֵ
                MSetAcquisitionEnable(1,C_SETMODE_SETSAVE);
                MSetBGateMode(mode1,C_SETMODE_SETSAVE);//�ָ�B������
                // DrawDac(1);
                keycode = -1;
            }
            break;
        case C_FUNC_FILL:	          //ʵ�Ĳ���
            prestatus = MGetFill();	//ԭ״̬
            // MSetFunctionMode( prestatus+1,C_WAVE_FILL);
            MSetFill(MGetFill()+1,C_SETMODE_SETSAVE);
            SysParaDisp();
            break;
		case C_FUNC_TOFD:
			TOFDFunc();
			break;
        default:
            break;
        }

    }



/////////////
    if( MGetStdMode() == C_STD_AWS_NUM - 1)//AWS
    {
        MSetBGateMode(0, C_SETMODE_SAVE);
        //MSetEquivalentMode();	//mode1=0B��ʧ��1������mode2<0����.>0dB��
    }

    if( keycode == C_KEYCOD_RETURN )
    {
        keycode = -1 ;
    }
//	if( keycode == -1 )ExpendTime(20);
//Write_Number(10,300,1,1,0,0);
    if(MGetSystemMode() == 0 && keycode != C_KEYCODMAX)
//	if( MGetSystemMode() == 0 && keycode == -1 )
    {
//Write_Number(10,300,2,1,0,0);
        /*int i,j,rev=0;
        int FileMax=(C_OFF_FILE+4095)/4096*4096;
        char c[FileMax];*/
        SystemParaStore();
        ChannelParaStore();

        /* for(i=0;i<FileMax;i++)
         {
             c[i]=MEMORY_STORAGE[i];
         }*/
        //  CopyMemoryAllSystem(PM_COPY_WRITE);

        /*      CopyMemoryAllSystem(PM_COPY_READ);
                          for(i=0,j=0;i<FileMax;i++)
                          {
                              if(c[i]!=MEMORY_STORAGE[i]){
                                  j++;
                              }
                          }
                              Write_Number(30,0,j,4,0,0);
        */
        /*        MEMORY_STORAGE[C_OFF_BACKUP]=12;
                            for(i=0,j=0;i<145;i++)
                            {
                                if(i%5==0 && i>0)j++;
                                Write_Number((i%10)*112,10+j*16,MEMORY_STORAGE[C_OFF_BACKUP-i],7,0,0);
                            }
                         Write_Number(10,0,1,3,0,0);
                         Write_Number(10,20,rev,3,0,0);
                         MAnyKeyReturn();
                rev=CopyMemoryAllSystem(PM_COPY_WRITE);

                            for(i=0,j=0;i<145;i++)
                            {
                                if(i%5==0 && i>0)j++;
                                Write_Number((i%10)*112,10+j*16,MEMORY_STORAGE[C_OFF_BACKUP-i],7,0,0);
                            }
                         Write_Number(10,0,2,3,0,0);
                         Write_Number(10,20,rev,3,0,0);
                         MAnyKeyReturn();

                MEMORY_STORAGE[C_OFF_BACKUP]=13;
                rev=CopyMemoryAllSystem(PM_COPY_READ);

                           for(i=0,j=0;i<145;i++)
                            {
                                if(i%5==0 && i>0)j++;
                                Write_Number((i%10)*112,10+j*16,MEMORY_STORAGE[C_OFF_BACKUP-i],7,0,0);
                            }
                         Write_Number(10,0,3,3,0,0);
                         Write_Number(10,20,rev,3,0,0);
                         MAnyKeyReturn();
            */
    }
    //if(mode == 1 && keycode == C_KEYCOD_RETURN)MChannelRenovate();
    return keycode;
}
int ParaMenu1(int page)//���ֵڼ�ҳ�Ĳ����˵�
{
    u_int char_len = 6,row_number = 8,crow_height = 48,erow_height =16;
    int number,deci_len,sign;
    int xpos,ypos;
    //int menu_xpos = 0,menu_ypos = 0;
    int para_xpos = C_CCHAR_HDOT * ( char_len + 1) ,para_ypos = crow_height + 0;
    int retvalue = C_TRUE;
    int keycode;
    int page_max = 2;
    int row;
//	int offset;
    u_short* cpText = 0;
    u_char* epText = 0;
    u_char* epText1 = 0;
    int i;
    u_int elapsedtime1 = GetElapsedTime() ;//,elapsedtime2;

    MSetColor(C_CR_MENU);

    MSetAcquisition(0);
    ClearEnvelope();
    SystemParaStore();
    ChannelParaStore();
    DisplayPrompt(15);

#if C_DEVTYPE==20
    MSetDacMode(2,0/*mode = 0*/);
#endif

    while(1)
    {
        MFclearScreen();
        MSetColor(C_CR_MENU);

        for (i=0; i<row_number; i++)
        {
            switch(page)
            {
            case 1:
                if( MGetTestStatus(C_TEST_AVG) != 1)
                {
                    epText1 =  (u_char *)_ParaMenu_A6[MGetLanguage()][i];
                    epText =  (u_char*)_ParaMenu_B6;
                }
                else
                {
                    epText1 =  (u_char *)_ParaMenu_A6A1[MGetLanguage()][i];
                    epText =  (u_char*)_ParaMenu_B6A1;
                }
                row_number = 8;
                break;
            case 2:
                epText1 =(u_char *)_ParaMenu_A3[MGetLanguage()][i];
                epText =  (u_char*)_ParaMenu_B3;
                row_number = 2;
                break;

            }
            TextOut(C_ECHAR_HDOT,para_ypos+crow_height*i,1,11,crow_height,epText1,0);
            if (MGetLanguage())
            {
                TextOut(C_ECHAR_HDOT,para_ypos+10+crow_height*i,1,11,crow_height,epText1+11,2);
            }
        }
        TextOut(C_ECHAR_HDOT,4,1,25,16,(u_char*)_ParaMenu_H1[MGetLanguage()][0],4);
        Write_Number(C_ECHAR_HDOT + 12 * C_ECHAR_HDOT+4, 4+3*MGetLanguage(),page,1,0,0);
        if (MGetLanguage())
        {
            Write_Number(C_ECHAR_HDOT + 24 * C_ECHAR_HDOT+8,7,page_max,1,0,0);
        }
        else
        {
            Write_Number(C_ECHAR_HDOT + 19 * C_ECHAR_HDOT+4, 4,page_max,1,0,0);
        }

        //	CEMenuOut(C_ECHAR_HDOT,para_ypos, cpText ,char_len,row_number,crow_height);	/*��ָ��λ�ø���ÿ���ַ������������и�д�˵�*/
        EMenuOut(para_xpos+C_ECHAR_HDOT*5,para_ypos + 8, epText ,3,row_number,crow_height);	/*��ָ��λ�ø���ÿ���ַ������������и�д�˵�*/

        EraseDrawRectangle(26*C_ECHAR_HDOT, 380, 26*C_ECHAR_HDOT + (5+1)*C_CCHAR_HDOT, 380 + 2 * (C_CCHAR_VDOT+12)) ;
        //	CMenuOut(27*C_ECHAR_HDOT,190+4,_MenuPrompt_B1,5,2,24);	/*��ָ��λ�ø���ÿ���ַ������������и�д�˵�*/
        TextOut(27*C_ECHAR_HDOT,380+4,1,10,24,(u_char*)_MenuPrompt_B1[MGetLanguage()][0],4);
        TextOut(27*C_ECHAR_HDOT,380+4+36,1,10,24,(u_char*)_MenuPrompt_B1[MGetLanguage()][0]+10,4);
//        EMenuOut(27*C_ECHAR_HDOT,380+4,_TestMenu_Enter,1,1,C_ECHAR_VDOT);

        MSetColor(C_CR_PARA);

        xpos = para_xpos;
        //���̱��
        switch(page)
        {
        case 1:
            row = 0;
            xpos = para_xpos;
            ypos = para_ypos + crow_height *row;
            //	cpText = (u_short*)_ParaMenu_AB1 + (MGetScaleMode()-0) * 3 + 1;
            //	CEMenuOut(xpos,ypos , cpText ,2,1,crow_height);	/*��ָ��λ�ø���ÿ���ַ������������и�д�˵�*/
            TextOut(xpos,ypos ,1,7,crow_height,(u_char*)_ParaMenu_AB1[MGetLanguage()][MGetScaleMode()]+2,4);

            //���油��
            row = 1;
            ypos = para_ypos + crow_height *row + C_ECHAR_HDOT;
            Write_Number(xpos,ypos,MGetSurfGain(),4,1,0);

            row = 2;//�������
            ypos = para_ypos + crow_height *row + C_ECHAR_HDOT * 1;
            WriteLongness(xpos , ypos  ,MGetThick(),4/*�ܳ�*/,1/*�Ƿ�д��λ*/);
            //Write_Number(xpos,ypos,MGetThick(),4,1,0);

            row = 3;//��ӡ��
            ypos = para_ypos + crow_height *row + C_ECHAR_HDOT;
            epText = (u_char*)_ParaMenu_AB3 + ( (MGetPrinterMode()-0)*15+2 );
            EMenuOut(xpos,ypos , epText ,13,1,erow_height);	/*��ָ��λ�ø���ÿ���ַ������������и�д�˵�*/

            //3��	������׼��ĸ��\�з�\����\����
            row = 4;
            xpos = para_xpos;
            ypos = para_ypos + crow_height *row + C_ECHAR_HDOT * 0;
            TextOut(xpos,ypos ,1,8,crow_height,(u_char*)_ParaMenu_AC2[MGetLanguage()][MGetDacMode()]+2,4);

            //7��	�з���RL��DAC+0.0dB
            row = 5;
            xpos = para_xpos;
            ypos = para_ypos + crow_height *row + C_ECHAR_HDOT * 1;
            if( MGetTestStatus(C_TEST_AVG) == 1)
            {
                Write_Number(xpos,ypos,(int)( 10*powf( 10,( MGetLineGain(0) + 120 ) / 400.0)  + 0.6) ,4,1,0);
            }
            else Write_Number(xpos,ypos,MGetLineGain(0),5,1,1);

            //8��	������SL��DAC+0.0dB
            row = 6;
            xpos = para_xpos;
            ypos = para_ypos + crow_height *row + C_ECHAR_HDOT * 1;
            if( MGetTestStatus(C_TEST_AVG) == 1)
            {
                Write_Number(xpos,ypos,(int)( 10*powf( 10,( MGetLineGain(1) + 120 ) / 400.0)  + 0.6) ,4,1,0);
            }
            else Write_Number(xpos,ypos,MGetLineGain(1),5,1,1);

            //9��	������EL��DAC+0.0dB
            row = 7;
            xpos = para_xpos;
            ypos = para_ypos + crow_height *row + C_ECHAR_HDOT * 1;
            if( MGetTestStatus(C_TEST_AVG) == 1)
            {
                Write_Number(xpos,ypos,(int)( 10*powf( 10,( MGetLineGain(2) + 120 ) / 400.0)  + 0.6) ,4,1,0);
            }
            else Write_Number(xpos,ypos,MGetLineGain(2),5,1,1);

            break;

        case 2:

            //1��	��¼��ʽ������\����
            row = 0;
            if( row < row_number)
            {
                xpos = para_xpos;
                ypos = para_ypos + crow_height *row + C_ECHAR_HDOT * 0;
                //	cpText = (u_short*)_ParaMenu_AC1 + MGetSaveMode() * 3 + 1;
                //	CEMenuOut(xpos,ypos , cpText ,2,1,crow_height);	/*��ָ��λ�ø���ÿ���ַ������������и�д�˵�*/
                TextOut(xpos,ypos ,1,6,crow_height,(u_char*)_ParaMenu_AC1[MGetLanguage()][MGetSaveMode()]+2,4);
            }
            //2��	��¼�����10sec-120sec	;��Ϊ������˲���Ϊ0��ѡ����ʱ�Զ���Ϊ30sec
            row = 1;
            if( row < row_number)
            {
                xpos = para_xpos;
                ypos = para_ypos + crow_height *row + C_ECHAR_HDOT * 0;
                TextOut(xpos,ypos ,1,8,crow_height,(u_char*)_ParaMenu_AC7[MGetLanguage()][MGetSavePosition()]+2,4);

                //Write_Number(xpos,ypos,MGetSaveTime(),4,1,0);
            }
            break;
        }

        MKeyRlx();		/*ֻ�а������ͷŲ��˳�*/
        do
        {
            if( GetElapsedTime() > elapsedtime1 + 200 )
            {
                InvertWords(27*C_ECHAR_HDOT,380+4, 1); /*������ʾ�����ֵ�����*/
                elapsedtime1 = GetElapsedTime() ;
            }
            keycode = MGetKeyCode(10);
            if ( (keycode > 0 && keycode <= row_number)
                    || ( keycode == C_KEYCOD_PLUS || keycode == C_KEYCOD_MINUS)
                    || ( keycode == C_KEYCOD_LEFT || keycode == C_KEYCOD_RIGHT)
                    || ( keycode == C_KEYCOD_CONFIRM || keycode == C_KEYCOD_RETURN )
               )break;
            else
            {
                keycode = MGetRotary(-1);
                if( keycode > 0 )keycode = C_KEYCOD_PLUS;
                else if( keycode < 0)keycode = C_KEYCOD_MINUS;
                else continue;
                break;
            }
        }
        while(1);

        //keycode = MAnyKeyReturn();

        if (keycode == C_KEYCOD_RETURN)
        {

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

        if (keycode == C_KEYCOD_RIGHT)
        {
            page++;
            if(page > page_max)page = 1;
            continue;
        }
        else if (keycode == C_KEYCOD_LEFT)
        {
            page--;
            if(page == 0)page = page_max;
            continue;
        }

        row = keycode - 1;
        xpos = para_xpos;
        ypos = para_ypos + crow_height * row-1;

        InvertWords(C_ECHAR_HDOT,ypos, 1); /*������ʾ�����ֵ�����*/

        if( page == 1)
        {
            switch (keycode)
            {
            case 1:		//���
                //	cpText = (u_short*)_ParaMenu_AB1 ;
                MPushWindow(xpos, ypos, xpos + (6)*C_CCHAR_HDOT, ypos + crow_height * 3) ;
                EraseDrawRectangle(xpos, ypos, xpos + (6)*C_CCHAR_HDOT, ypos + crow_height * 3) ;
                //	CEMenuOut(xpos+4,ypos+4 , cpText ,3,3,crow_height);	/*��ָ��λ�ø���ÿ���ַ������������и�д�˵�*/
                TextOut(xpos+4,ypos+4 ,3,10,crow_height,(u_char*)_ParaMenu_AB1[MGetLanguage()][0],4);
                while(1)
                {
                    keycode = MAnyKeyReturn();
                    if (keycode == C_KEYCOD_RETURN||keycode == C_KEYCOD_CONFIRM)
                        break;
                    else if(keycode >= 1 && keycode <= 3)
                    {
                        MSetScaleMode(keycode-1,C_SETMODE_SAVE);
                        break;
                    }
                }
                MPopWindow();
                break;

            case 2:	//���油��
                deci_len = 1;
                number = 100;
                while(true)
                {
                    if( Input_Number(xpos,ypos-C_ECHAR_VDOT,&number,2, &deci_len,0) != 1)break;
                    else
                    {
                        if( number <= C_MAX_SURFGAIN)
                        {
                            MSetSurfGain(number,C_SETMODE_SAVE);
                            break;
                        }
                        else	DisplayPrompt(4);
                    }
                }

                break;
            case 3:	//
                if( MGetUnitType() > 0)deci_len = 3;//��λinch
                else deci_len = 1;
                number = 0;
                while( true )//��δ������������������
                {
                    int i,j,k;
                    if( Input_Number(xpos,ypos,&number,4, &deci_len,0) != 1)
                        break;
                    else if( number < 50000)
                    {
                        MSetThick(number);
                        number /= 10;
                        if( MGetUnitType() > 0)number = (UINT)(number * 254/1000.0+0.5);//��λinch
                        k = c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_THNUM];
                        for( i = 0 ; i < k; i++)
                        {
                            if(MGetStdMode()==0)break;
                            if(MGetStdMode()==10||MGetStdMode()==11)
                            {
                                if( number >=c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_THICK+7*i] )continue;

                            }
                            else
                            {
                                if( number >c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_THICK+7*i] )continue;

                            }

                            for( j = 0; j < MGetCurveNumber(); j++)
                            {
                                MSetLineGain(j, c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_THICK+7*i+j+1] );
                            }
                            break;
                        }
                        break;
                    }
                }
                break;
            case 4:	//��ӡ��
                epText = (u_char*)_ParaMenu_AB3;
                //ypos -= erow_height * 4 ;
                MPushWindow(xpos, ypos, xpos + 8*C_CCHAR_HDOT, ypos + erow_height * 5) ;
                EraseDrawRectangle(xpos, ypos, xpos + 8*C_CCHAR_HDOT, ypos + erow_height * 5) ;
                EMenuOut(xpos+4,ypos+4, epText ,15,4,15);	/*��ָ��λ�ø���ÿ���ַ������������и�д�˵�*/
                while(1)
                {
                    keycode = MAnyKeyReturn();
                    if (keycode == C_KEYCOD_RETURN||keycode == C_KEYCOD_CONFIRM)
                        break;
                    else if(keycode >= 1 && keycode <= 4)
                    {
                        MSetPrinterMode(keycode-1,0/*mode = 0\1\2*/);	/*����̽ͷģʽ*/
                        break;
                    }
                }
                MPopWindow();

                break;
            case 5:	//������׼
#if C_DEVTYPE==20
                break;
#else
                //	cpText = (u_short*)_ParaMenu_AC2 ;
                if( MGetCurveNumber() > 3)ypos -= crow_height * ( MGetCurveNumber()-3 );
                MPushWindow(xpos, ypos, xpos + 6*C_CCHAR_HDOT, ypos + crow_height * ( MGetCurveNumber()+1 ) ) ;
                EraseDrawRectangle(xpos, ypos, xpos + (6)*C_CCHAR_HDOT, ypos + crow_height * ( MGetCurveNumber()+1 )) ;
                //	CEMenuOut(xpos+4,ypos+4 , cpText, 4,MGetCurveNumber()+1,crow_height);	/*��ָ��λ�ø���ÿ���ַ������������и�д�˵�*/
                TextOut(xpos+4,ypos+4 ,MGetCurveNumber()+1,10,crow_height,(u_char*)_ParaMenu_AC2[MGetLanguage()][0],4);
                while(1)
                {
                    keycode = MAnyKeyReturn();
                    if (keycode == C_KEYCOD_RETURN||keycode == C_KEYCOD_CONFIRM)
                        break;
                    else if(keycode > 0 && keycode <= MGetCurveNumber()+1 )
                    {
                        MSetDacMode(keycode-1,C_SETMODE_SAVE);
                        break;
                    }
                }
                MPopWindow();
                break;
#endif
            case 6:		//Rl
                if( MGetParaLock(1,C_LOCK_DACAVG) == 1)break;

                deci_len = 1;
                number = 0;
                if( MGetTestStatus(C_TEST_AVG) == 1) sign = 0;
                else sign = 1;

                while( 1 )
                {
                    if( Input_Number(xpos,ypos,&number,3, &deci_len,sign) < 1)break;
                    else
                    {
                        if(MGetTestStatus(C_TEST_AVG) == 1)
                        {
                            if( number >= 5 && number < 100)
                            {
                                number = (int)( 400 * log10( number/10)-120 );
                            }
                            else break;
                        }
                        else if(number < -300 || number > 400)break;
                        MSetLineGain(0, number);
                        break;
                    }
                }

                break;
            case 7:
                if( MGetParaLock(1,C_LOCK_DACAVG) == 1)break;

                deci_len = 1;
                number = 0;
                if(MGetTestStatus(C_TEST_AVG) == 1)sign = 0;
                else sign = 1;
                while( 1 )
                {
                    if( Input_Number(xpos,ypos,&number,3, &deci_len,sign) < 1)break;
                    else
                    {
                        if(MGetTestStatus(C_TEST_AVG) == 1)
                        {
                            //AVG
                            if( number >= 5 && number < 100)
                                number = (int)( 400 * log10( number/10) - 120);
                            else break;
                        }
                        else if(number < -300 || number > 400)break;

                        MSetLineGain(1, number);
                        break;
                    }
                }

                break;
            case 8:	//El
                if( MGetParaLock(1,C_LOCK_DACAVG) == 1)break;

                deci_len = 1;
                number = 0;
                if(MGetTestStatus(C_TEST_AVG) == 1)sign = 0;
                else sign = 1;
                while( 1 )
                {
                    if( Input_Number(xpos,ypos,&number,3, &deci_len,sign) < 1)break;
                    else
                    {
                        if(MGetTestStatus(C_TEST_AVG) == 1)
                        {
                            if( number >= 5 && number < 100)
                                number = (int)( 400 * log10( number/10) -120);
                            else break;
                        }
                        else if(number < -300 || number > 400)break;

                        MSetLineGain(2, number);
                        break;
                    }
                }
                break;
            }
        }
        else if( page == 2)
        {
#if C_UDISK == 1
            switch (keycode)
            {
            case 1:	//�洢ģʽ
                //*
                if (MGetSaveStatus() > 0)
                {
                    break;
                }
                MPushWindow(xpos, ypos, xpos + 5*C_CCHAR_HDOT+1, ypos + crow_height * 2) ;
                EraseDrawRectangle(xpos, ypos, xpos + (5)*C_CCHAR_HDOT, ypos + crow_height * 2) ;
                TextOut(xpos+4,ypos+4 ,2,8,crow_height,(u_char*)_ParaMenu_AC1[MGetLanguage()][0],4);
                while(1)
                {
                    keycode = MAnyKeyReturn();
                    if (keycode == C_KEYCOD_RETURN||keycode == C_KEYCOD_CONFIRM)
                        break;
                    else if(keycode >= 1 && keycode <= 2)
                    {
                        MSetSaveMode(keycode-1,C_SETMODE_SETSAVE);
                        if (MGetSaveMode())
                        {
                            MSetSaveStatus( 0,C_SETMODE_SETSAVE);
                            MSetSavePosition(1,C_SETMODE_SETSAVE);
                        }
                        else
                        {
                            MSetSavePosition(0,C_SETMODE_SETSAVE);
                        }
                        /*	if( MGetSaveTime() > 600 || MGetSaveTime() < 1)
                        {
                        MSetSaveTime(10,C_SETMODE_SAVE);
                        offset = C_OFF_STORE +	34;		//���Զ������ݸ���
                        number = 0;
                        MCopyProtectedMemory( (void*)offset, &number,4, PM_COPY_WRITE);
                        					}*/
                        break;
                    }
                }
                MPopWindow();
                //*/
                break;

            case 2:	//�洢λ��
                break;
                if (MGetSaveStatus() > 0)
                {
                    break;
                }
                ypos -= crow_height * 1;
                MPushWindow(xpos, ypos, xpos + 5*C_CCHAR_HDOT+1, ypos + crow_height * 2) ;
                EraseDrawRectangle(xpos, ypos, xpos + (5)*C_CCHAR_HDOT, ypos + crow_height * 2) ;
                TextOut(xpos+4,ypos+4 ,2,10,crow_height,(u_char*)_ParaMenu_AC7[MGetLanguage()][0],4);
                while(1)
                {
                    keycode = MAnyKeyReturn();
                    if (keycode == C_KEYCOD_RETURN||keycode == C_KEYCOD_CONFIRM)
                        break;
                    else if(keycode >= 1 && keycode <= 2)
                    {
                        MSetSavePosition(keycode-1,C_SETMODE_SETSAVE);
                        break;
                    }
                }
                MPopWindow();
                break;

            }
            ExpendTime(20);
#endif
        }
    }
    SystemParaRestore();
    ChannelParaRestore();
    MSetScreenBrightness( MGetScreenBrightness(),C_SETMODE_SETSAVE);
    MSetAcquisition(1);
    ClearEnvelope();	//�������ֵ

    MSetColor(C_CR_UNDO);
    ScreenRenovate();	/*��Ļˢ��*/
    DrawDac(0);		//�»�DAC
    return 1;
}

int ParaMenu(int page)//���ֵڼ�ҳ�Ĳ����˵�
{
    u_int char_len = 6,row_number = 9,crow_height = 48,erow_height = 30;
    int number,deci_len,sign;
    int xpos,ypos;
    //int menu_xpos = 0,menu_ypos = 0;
    int para_xpos = C_CCHAR_HDOT * ( char_len + 1) ,para_ypos = crow_height ;
    int retvalue = C_TRUE;
    int keycode;
    int page_max = 2;
    int row;
    int stdswitch=0;
    u_short* cpText = 0;
    u_char* epText = 0;
    u_char* epText1 = 0;
    int i;
    u_int elapsedtime1 = GetElapsedTime() ;//,elapsedtime2;
    int old_unit = MGetUnitType();
    int old_unit1 = MGetUnitType();

#if C_DEVTYPE == 9 || C_DEVTYPE == 10 || C_DEVTYPE == 13 || C_DEVTYPE == 14  ||C_DEVTYPE == 15
    page_max = 3;	//3
#else
    page_max = 2;
#endif

    i = 0;
    if( page < 1)page = 1;
    else if(page > page_max )page = page_max;

    MSetColor(C_CR_MENU);

    MSetAcquisition(0);
    ClearEnvelope();
    SystemParaStore();
    ChannelParaStore();
    DisplayPrompt(15);

    xpos = 0;
    while(1)
    {
        para_ypos = crow_height-4 ;
        MFclearScreen();
        MSetColor(C_CR_MENU);

        TextOut(C_ECHAR_HDOT,4,1,25,16,(u_char*)_ParaMenu_H1[MGetLanguage()][0],4);
        Write_Number(C_ECHAR_HDOT + 12 * C_ECHAR_HDOT+4, 4+3*MGetLanguage(),page,1,0,0);
        if (MGetLanguage())
        {
            Write_Number(C_ECHAR_HDOT + 24 * C_ECHAR_HDOT+16, 7,page_max,1,0,0);
        }
        else
        {
            Write_Number(C_ECHAR_HDOT + 19 * C_ECHAR_HDOT+4, 4,page_max,1,0,0);
        }

        for (i=0; i<row_number; i++)
        {
            switch(page)
            {
            case 1:
                if( MGetTestStatus(C_TEST_AVG) == 1)
                {
                    epText1 =(char *)_ParaMenu_A22[MGetLanguage()][i];
                    epText =  (u_char*)_ParaMenu_B2A1;
                }
                else
                {
                    epText1 = (char *)_ParaMenu_A2[MGetLanguage()][i];
                    epText =  (u_char*)_ParaMenu_B2;
                }
                if((MGetStdMode() ==10||(MGetStdMode() ==11))&& (page == 1))
                {
                    row_number = 6;
                }
                else
                {
                    row_number = 9;
                }
                break;
            case 2:
                epText1 =(char *)_ParaMenu_A3[MGetLanguage()][i];
                epText =  (u_char*)_ParaMenu_B3;
#if C_DEVTYPE == 1
                row_number = 9;
#else
                row_number = 9;
#endif

                break;
            case 3:
                epText1 =	(char *)_ParaMenu_C1[MGetLanguage()][i];
                epText =  (u_char*)_ParaMenu_D1;
#if C_DEVTYPE == 1
                row_number = 1;
#else
                row_number = 3;
#endif
                break;
            }

            TextOut(C_ECHAR_HDOT,para_ypos+crow_height*i,1,22,crow_height,epText1,0);
            //   if (MGetLanguage())
            {
                //      TextOut(C_ECHAR_HDOT,para_ypos+C_ECHAR_VDOT+crow_height*i,1,11,crow_height,epText1+11,2);
            }
        }
        //	CEMenuOut(C_ECHAR_HDOT,para_ypos, cpText ,char_len,row_number,crow_height);	/*��ָ��λ�ø���ÿ���ַ������������и�д�˵�*/
        EMenuOut(para_xpos+C_ECHAR_HDOT*5,para_ypos+C_ECHAR_VDOT*MGetLanguage() , epText ,3,row_number,crow_height);	/*��ָ��λ�ø���ÿ���ַ������������и�д�˵�*/

        EraseDrawRectangle(26*C_ECHAR_HDOT, 380, 26*C_ECHAR_HDOT + (6+1*MGetLanguage())*C_CCHAR_HDOT, 380 + 2 * (C_CCHAR_VDOT+12)) ;
        //	CMenuOut(27*C_ECHAR_HDOT,190+4,_MenuPrompt_B1,5,2,24);	/*��ָ��λ�ø���ÿ���ַ������������и�д�˵�*/

        TextOut(27*C_ECHAR_HDOT,380+4,1,10,24,(u_char*)_MenuPrompt_B1[MGetLanguage()][0],4);
        TextOut(27*C_ECHAR_HDOT,380+4+36,1,10,24,(u_char*)_MenuPrompt_B1[MGetLanguage()][0]+10,4);
//        EMenuOut(27*C_ECHAR_HDOT,380+4,_TestMenu_Enter,1,1,C_ECHAR_VDOT);
        MSetColor(C_CR_PARA);
        xpos = para_xpos;
        if(MGetLanguage())
        {
            para_ypos+=C_ECHAR_VDOT;
        }

        switch(page)
        {
        case 1:
            //���̱��
            row = 0;
            ypos = para_ypos + crow_height *row;
            //	cpText = (u_short*)_ParaMenu_AB1 + (MGetScaleMode()-0) * 3 + 1;
            //	CEMenuOut(xpos,ypos , cpText ,2,1,crow_height);	/*��ָ��λ�ø���ÿ���ַ������������и�д�˵�*/
            TextOut(xpos,ypos ,1,7,crow_height,(u_char*)_ParaMenu_AB1[MGetLanguage()][MGetScaleMode()]+2,4);

            //���油��
            row = 1;
            ypos = para_ypos + crow_height *row ;
            Write_Number(xpos,ypos,MGetSurfGain(),4,1,0);
            row = 2;//�������
            ypos = para_ypos + crow_height *row ;
            WriteLongness(xpos,ypos,MGetThick(),5,1);

#if C_DEVTYPE == 4 || C_DEVTYPE == 20
            break;
#endif
            row = 3;//�����⾶
            ypos = para_ypos + crow_height *row ;
            WriteLongness(xpos,ypos,MGetDiameter(),5,1);
            ///Write_Number(xpos,ypos,MGetDiameter(),4,1,0);

            //3��	������׼��ĸ��\�з�\����\����
            row = 4;
            xpos = para_xpos;
            ypos = para_ypos + crow_height *row;
            TextOut(xpos,ypos ,1,8,crow_height,(u_char*)_ParaMenu_AC2[MGetLanguage()][MGetDacMode()]+2,4);
            //4��	������ʾ��dBֵ\�׾�
            row = 5;
            xpos = para_xpos;
            ypos = para_ypos + crow_height *row;

//			if( MGetTestStatus(C_TEST_AVG) == 1)
            {
                TextOut(xpos,ypos ,1,6,crow_height,(u_char*)_ParaMenu_AC3A[MGetLanguage()][MGetAvgMode()]+2,4);
            }
//			else
            {
//				cpText = (u_short*)_ParaMenu_AC3 + (MGetAvgMode()-0) * 4 + 1;		//����DB��Ϊ������
//				CEMenuOut(xpos,ypos , cpText ,2,1,crow_height);
            }

            if(MGetStdMode() !=10&&MGetStdMode() !=11)
            {
                //7��	�з���RL��DAC+0.0dB
                row = 6;
                xpos = para_xpos;
                ypos = para_ypos + crow_height *row ;
                if( MGetTestStatus(C_TEST_AVG) == 1)
                {
                    number = (int)( 10*powf( 10,( MGetLineGain(0) + 120 ) / 400.0)  + 0.6);
                    if(MGetUnitType() > 0)
                    {
                        number = (UINT)(number * 1000/254.0+0.5);
                    }
                    //WriteLongness(xpos,ypos,number,5,1);
                    Write_Number(xpos,ypos,number,5,1,1);

                    //				//Write_Number(xpos,ypos,(int)( 10*powf( 10,( MGetLineGain(0) + 120 ) / 400.0)  + 0.5) ,4,1,0);
                }
                else Write_Number(xpos,ypos,MGetLineGain(0),5,1,1);

                //8��	������SL��DAC+0.0dB
                row = 7;
                xpos = para_xpos;
                ypos = para_ypos + crow_height *row;
                if( MGetTestStatus(C_TEST_AVG) == 1)
                {
                    number = (int)( 10*powf( 10,( MGetLineGain(1) + 120 ) / 400.0)  + 0.6);
                    if(MGetUnitType() > 0)
                    {
                        number = (UINT)(number * 1000/254.0+0.5);
                    }
                    // WriteLongness(xpos,ypos,number,5,1);
                    Write_Number(xpos,ypos,number,5,1,1);
                    //Write_Number(xpos,ypos,(int)( 10*powf( 10,( MGetLineGain(1) + 120 ) / 400.0)  + 0.5) ,4,1,0);
                }
                else Write_Number(xpos,ypos,MGetLineGain(1),5,1,1);

                //9��	������EL��DAC+0.0dB
                row = 8;
                xpos = para_xpos;
                ypos = para_ypos + crow_height *row;
                if( MGetTestStatus(C_TEST_AVG) == 1)
                {
                    number = (int)( 10*powf( 10,( MGetLineGain(2) + 120 ) / 400.0)  + 0.6);
                    if(MGetUnitType() > 0)
                    {
                        number = (UINT)(number * 1000/254.0+0.5);
                    }
                    //  WriteLongness(xpos,ypos,number,5,1);
                    Write_Number(xpos,ypos,number,5,1,1);
                    //Write_Number(xpos,ypos,(int)( 10*powf( 10,( MGetLineGain(2) + 120 ) / 400.0)  + 0.5) ,4,1,0);
                }
                else Write_Number(xpos,ypos,MGetLineGain(2),5,1,1);
            }
            break;

        case 2:

//#if C_UDISK == 1
            //1��	��¼��ʽ������\����
            row = 0;
            if( row < row_number)
            {
                xpos = para_xpos;
                ypos = para_ypos + crow_height *row ;
                //	cpText = (u_short*)_ParaMenu_AC1 + MGetSaveMode() * 3 + 1;
                //	CEMenuOut(xpos,ypos , cpText ,2,1,crow_height);
                TextOut(xpos,ypos ,1,6,crow_height,(u_char*)_ParaMenu_AC1[MGetLanguage()][MGetSaveMode()]+2,4);
            }
            //2��	��¼�����10sec-120sec	;��Ϊ������˲���Ϊ0��ѡ����ʱ�Զ���Ϊ30sec
            row = 1;
            if( row < row_number)
            {
                xpos = para_xpos;
                ypos = para_ypos + crow_height *row ;
                TextOut(xpos,ypos ,1,8,crow_height,(u_char*)_ParaMenu_AC7[MGetLanguage()][MGetSavePosition()]+2,4);

                //Write_Number(xpos,ypos,MGetSaveTime(),4,1,0);
            }
            /*#else
            			row = 0;//������λ
            			if( row >= row_number)break;
            			ypos = para_ypos + crow_height *row + C_ECHAR_HDOT * 1;
            			epText = (u_char*)_ParaMenu_CA1 + MGetUnitType() * 6 + 2;
            			EMenuOut(xpos,ypos , epText ,4,1,crow_height);

            			row = 1;//����
            			if( row >= row_number)break;
            			ypos = para_ypos + crow_height *row + C_ECHAR_HDOT * 0;
            			epText = (u_char *)_MainMenu_C1A11[MGetLanguage()][MGetLanguage()];
            			TextOut(xpos,ypos,1,8,crow_height,epText+2,8);
            #endif*/

            //λ�÷�ʽ,ǰ�ط�ֵ
            row = 2;
            ypos = para_ypos + crow_height *row ;
            TextOut(xpos,ypos ,1,6,crow_height,(u_char*)_ParaMenu_AC6[MGetLanguage()][MGetReadForeland()]+2,4);

            xpos = para_xpos;

            row = 3;//����
            if( row >= row_number)break;
            ypos = para_ypos + crow_height *row;
            epText = (u_char *)_MainMenu_C1A11[MGetLanguage()][MGetLanguage()];
            TextOut(xpos,ypos,1,8,crow_height,epText+2,8);


            //6��	B����;��ʧ��\����
            row = 4;
            ypos = para_ypos + crow_height *row;
            TextOut(xpos,ypos,1,6+2*MGetLanguage(),crow_height,(u_char*)_ParaMenu_AC5[MGetLanguage()][MGetBGateMode()]+2,4);

            row = 5;//������������
            xpos = para_xpos;
            ypos = para_ypos + crow_height *row ;
            //	cpText = (u_short*)_ParaMenu_AD8 + MGetSoundAlarm(0) * 2 + 1;
            //	CEMenuOut(xpos,ypos , cpText ,1,1,crow_height);	 /*��ָ��λ�ø���ÿ���ַ������������и�д�˵�*/
            TextOut(xpos,ypos,1,4,crow_height,(u_char*)_ParaMenu_AD8[MGetLanguage()][MGetSoundAlarm(0)]+2,4);

            //5��	��Ļ���ȣ�����\һ��\����\����
#if C_LIGHT == 1
#if C_DEVLIB == 1 || C_DEVLIB == 3 || C_DEVLIB == 23 || C_DEVLIB == 24
            row = 6;
            if( row >= row_number)break;
            xpos = para_xpos;
            ypos = para_ypos + crow_height *row ;
            TextOut(xpos,ypos ,1,8,crow_height,(u_char*)_ParaMenu_AC4[MGetLanguage()][MGetScreenBrightness()]+2,4);
#endif
#endif
            //7��
            row = 7;
            if( row >= row_number)break;
            xpos = para_xpos;
            ypos = para_ypos + crow_height *row;
            Write_Number(xpos,ypos,MGetAmpStdMax()*5,4,1,0);

            //����ͼʾ
            row = 8;
            if( row < row_number)
            {
                xpos = para_xpos;
                ypos = para_ypos + crow_height *row;
                //	cpText = (u_short*)_Weld_C1 + stWeldPara.type  * 3 + 1;
                //	CEMenuOut(xpos,ypos , cpText ,2,1,crow_height);	/*��ָ��λ�ø���ÿ���ַ������������и�д�˵�*/
                TextOut(xpos,ypos,1,8,16,(u_char*)_Weld_C1[MGetLanguage()][stWeldPara.type]+2,4);
            }

            //8��	������ʱ��  min		��������ʱΪ0����Ϊ��ʱ�Զ���Ϊ5min
            /*				row = 7;
            			xpos = para_xpos;
            			ypos = para_ypos + crow_height *row + C_ECHAR_HDOT * 1;
            */
            break;
        case 3:

            row = 0;//��ӡ��
            ypos = para_ypos + crow_height *row ;
            epText = (u_char*)_ParaMenu_AB3 + ( (MGetPrinterMode()-0)*15+2 );
            EMenuOut(xpos,ypos , epText ,13,1,erow_height);	/*��ָ��λ�ø���ÿ���ַ������������и�д�˵�*/

            row = 1;//�洢Ƶ��
            if( row >= row_number)break;
            ypos = para_ypos + crow_height *row ;
            //	cpText = (u_short*)_ParaMenu_AD9 + MGetSaveFrequency() * 2 + 1;
            //	CEMenuOut(xpos,ypos , cpText ,1,1,crow_height);	 /*��ָ��λ�ø���ÿ���ַ������������и�д�˵�*/
            TextOut(xpos,ypos,1,3,crow_height,(u_char*)_ParaMenu_AD9[MGetLanguage()][MGetSaveFrequency()]+2,4);
            //6��	����������ȫ��\����
            row = 2;
            if( row >= row_number)break;
            xpos = para_xpos;
            ypos = para_ypos + crow_height *row ;
            u_short paratemp=0;
            int j;
            for(j = 0; j < C_LOCK_ALL; j++)
            {
                if(MGetParaLock(0,j) == 1)paratemp++;
            }
            epText = (u_char *)_ParaMenu_AD6[MGetLanguage()][paratemp];
            TextOut(xpos,ypos,1,4,crow_height,epText,4);

            row = 3;//������λ
            if( row >= row_number)break;
            ypos = para_ypos + crow_height *row;
            epText = (u_char*)_ParaMenu_CA1 + MGetUnitType() * 6 + 2;
            EMenuOut(xpos,ypos , epText ,4,1,crow_height);	 /*��ָ��λ�ø���ÿ���ַ������������и�д�˵�*/

            break;

        }

        MKeyRlx();		/*ֻ�а������ͷŲ��˳�*/
        do
        {
            if( GetElapsedTime() > elapsedtime1 + 200 )
            {
                InvertWords(27*C_ECHAR_HDOT,380+4, 1); /*������ʾ�����ֵ�����*/
                elapsedtime1 = GetElapsedTime() ;
            }
            keycode = MGetKeyCode(10);
            if ( (keycode > 0 && keycode <= row_number)
                    || ( keycode == C_KEYCOD_PLUS || keycode == C_KEYCOD_MINUS)
                    || ( keycode == C_KEYCOD_LEFT || keycode == C_KEYCOD_RIGHT)
                    || ( keycode == C_KEYCOD_CONFIRM || keycode == C_KEYCOD_RETURN )
               )
            {
                if(  !((MGetStdMode()==10||MGetStdMode() ==11)&&keycode == 5)  )
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
        while(1);

        //keycode = MAnyKeyReturn();

        if (keycode == C_KEYCOD_RETURN)
        {

            retvalue = C_FALSE;
            break;
        }
        if (keycode == C_KEYCOD_CONFIRM)
        {

            if( old_unit1 != old_unit )//���ȵ�λ�иı�
            {
                if( old_unit1 == 0)
                {
                    //�µ�λinch
                    MSetThick( (u_int)(MGetThick()/0.254+0.5) );
                    MSetDiameter( (u_int)(MGetDiameter()/0.254+0.5) );

                    stWeldPara.face = stWeldPara.face/0.254+0.5;
                    stWeldPara.thick1 = stWeldPara.thick1/0.254+0.5;
                    stWeldPara.wide = stWeldPara.wide/0.254+0.5;
                    stWeldPara.thick2 = stWeldPara.thick2/0.254+0.5;
                    stWeldPara.high1 = stWeldPara.high1/0.254+0.5;
                    stWeldPara.high2 = stWeldPara.high2/0.254+0.5;
                    stWeldPara.radius = stWeldPara.radius/0.254+0.5;
                    stWeldPara.deep = stWeldPara.deep/0.254+0.5;
                    stWeldPara.blunt = stWeldPara.blunt/0.254+0.5;
                    stWeldPara.space = stWeldPara.space/0.254+0.5;
                    stWeldPara.probe = stWeldPara.probe/0.254+0.5;
                }
                else
                {
                    MSetThick( (u_int)(MGetThick()*25.4/100+0.5) );
                    MSetDiameter( (u_int)(MGetDiameter()*25.4/100+0.5) );

                    stWeldPara.face = stWeldPara.face*0.254+0.5;
                    stWeldPara.thick1 = stWeldPara.thick1*0.254+0.5;
                    stWeldPara.wide = stWeldPara.wide*0.254+0.5;
                    stWeldPara.thick2 = stWeldPara.thick2*0.254+0.5;
                    stWeldPara.high1 = stWeldPara.high1*0.254+0.5;
                    stWeldPara.high2 = stWeldPara.high2*0.254+0.5;
                    stWeldPara.radius = stWeldPara.radius*0.254+0.5;
                    stWeldPara.deep = stWeldPara.deep*0.254+0.5;
                    stWeldPara.blunt = stWeldPara.blunt*0.254+0.5;
                    stWeldPara.space = stWeldPara.space*0.254+0.5;
                    stWeldPara.probe = stWeldPara.probe*0.254+0.5;
                }
                old_unit1=MGetUnitType();
            }
            SystemParaStore();
            ChannelParaStore();
            deci_len = MGetChannel();//����ͨ��
            if( old_unit != MGetUnitType() )//���ȵ�λ�иı�
            {
                int keycodetemp;
                if( old_unit == 0)
                {
                    //�µ�λinch
                    keycodetemp = C_OFF_TEMP + 0;
                    MCopyProtectedMemory( &number, (void*)keycodetemp, 4, PM_COPY_READ);
                    number = (u_int)(number * 100/25.4+0.5);
                    MCopyProtectedMemory( (void*)keycodetemp, &number, 4, PM_COPY_WRITE);
                    keycodetemp += 4;
                    MCopyProtectedMemory( &number, (void*)keycodetemp, 4, PM_COPY_READ);
                    number = (u_int)(number * 100/25.4+0.5);
                    MCopyProtectedMemory( (void*)keycodetemp, &number, 4, PM_COPY_WRITE);
                    keycodetemp += 4;
                    MCopyProtectedMemory( &number, (void*)keycodetemp, 4, PM_COPY_READ);
                    number = (u_int)(number * 100/25.4+0.5);
                    MCopyProtectedMemory( (void*)keycodetemp, &number, 4, PM_COPY_WRITE);
                    //����Ϊ����������ֵ
                    keycodetemp = C_OFF_TEMP + 20;
                    MCopyProtectedMemory( &number, (void*)keycodetemp, 4, PM_COPY_READ);
                    number = (u_int)(number * 100/25.4+0.5);
                    MCopyProtectedMemory( (void*)keycodetemp, &number, 4, PM_COPY_WRITE);
                    keycodetemp += 4;
                    MCopyProtectedMemory( &number, (void*)keycodetemp, 4, PM_COPY_READ);
                    number = (u_int)(number * 100/25.4+0.5);
                    MCopyProtectedMemory( (void*)keycodetemp, &number, 4, PM_COPY_WRITE);
                    //����Ϊ��Kֵ������ֵ
                    keycodetemp = C_OFF_TEMP + 30;
                    MCopyProtectedMemory( &number, (void*)keycodetemp, 4, PM_COPY_READ);
                    number = (u_int)(number * 100/25.4+0.5);
                    MCopyProtectedMemory( (void*)keycodetemp, &number, 4, PM_COPY_WRITE);
                    keycodetemp += 4;
                    MCopyProtectedMemory( &number, (void*)keycodetemp, 4, PM_COPY_READ);
                    number = (u_int)(number * 100/25.4+0.5);
                    MCopyProtectedMemory( (void*)keycodetemp, &number, 4, PM_COPY_WRITE);
                    keycodetemp += 4;
                    MCopyProtectedMemory( &number, (void*)keycodetemp, 4, PM_COPY_READ);
                    number = (u_int)(number * 100/25.4+0.5);
                    MCopyProtectedMemory( (void*)keycodetemp, &number, 4, PM_COPY_WRITE);
                    //����Ϊ��DAC������ֵ
                }
                else
                {
                    keycodetemp = C_OFF_TEMP + 0;
                    MCopyProtectedMemory( &number, (void*)keycodetemp, 4, PM_COPY_READ);
                    number = (u_int)(number*25.4/100+0.5);
                    MCopyProtectedMemory( (void*)keycodetemp, &number, 4, PM_COPY_WRITE);
                    keycodetemp += 4;
                    MCopyProtectedMemory( &number, (void*)keycodetemp, 4, PM_COPY_READ);
                    number = (u_int)(number*25.4/100+0.5);
                    MCopyProtectedMemory( (void*)keycodetemp, &number, 4, PM_COPY_WRITE);
                    keycodetemp += 4;
                    MCopyProtectedMemory( &number, (void*)keycodetemp, 4, PM_COPY_READ);
                    number = (u_int)(number*25.4/100+0.5);
                    MCopyProtectedMemory( (void*)keycodetemp, &number, 4, PM_COPY_WRITE);
                    //����Ϊ����������ֵ
                    keycodetemp = C_OFF_TEMP + 20;
                    MCopyProtectedMemory( &number, (void*)keycodetemp, 4, PM_COPY_READ);
                    number = (u_int)(number*25.4/100+0.5);
                    MCopyProtectedMemory( (void*)keycodetemp, &number, 4, PM_COPY_WRITE);
                    keycodetemp += 4;
                    MCopyProtectedMemory( &number, (void*)keycodetemp, 4, PM_COPY_READ);
                    number = (u_int)(number*25.4/100+0.5);
                    MCopyProtectedMemory( (void*)keycodetemp, &number, 4, PM_COPY_WRITE);
                    //����Ϊ��Kֵ������ֵ
                    keycodetemp = C_OFF_TEMP + 30;
                    MCopyProtectedMemory( &number, (void*)keycodetemp, 4, PM_COPY_READ);
                    number = (u_int)(number*25.4/100+0.5);
                    MCopyProtectedMemory( (void*)keycodetemp, &number, 4, PM_COPY_WRITE);
                    keycodetemp += 4;
                    MCopyProtectedMemory( &number, (void*)keycodetemp, 4, PM_COPY_READ);
                    number = (u_int)(number*25.4/100+0.5);
                    MCopyProtectedMemory( (void*)keycodetemp, &number, 4, PM_COPY_WRITE);
                    keycodetemp += 4;
                    MCopyProtectedMemory( &number, (void*)keycodetemp, 4, PM_COPY_READ);
                    number = (u_int)(number*25.4/100+0.5);
                    MCopyProtectedMemory( (void*)keycodetemp, &number, 4, PM_COPY_WRITE);
                    //����Ϊ��DAC������ֵ
                }

                for( sign = 0; sign < C_CHMAX; sign++)
                {
                    MSetChannel(sign,C_SETMODE_SETSAVE);
                    SystemParaStore();
                    ChannelParaRestore();
                    if( old_unit == 0)
                    {
                        //�µ�λinch
                        MSetSpeed( (u_int)(ChannelPara.Speed/0.254+0.5) ,C_SETMODE_SAVE);
                        MSetThick( (u_int)(MGetThick()/0.254+0.5) );
                        MSetDiameter( (u_int)(MGetDiameter()/0.254+0.5) );
                        MSetCrystal_l(ChannelPara.Crystal_l/25.4+0.5,C_SETMODE_SETSAVE);
                        MSetCrystal_w(ChannelPara.Crystal_w/25.4+0.5,C_SETMODE_SETSAVE);
                        MSetForward((u_int)( MGetForward()/0.254+0.5),C_SETMODE_SAVE);
                        MSetRange( (u_int)( MGetRange(1)/0.254+0.5 ),C_SETMODE_SAVE);

                        stWeldPara.face = stWeldPara.face/0.254+0.5;
                        stWeldPara.thick1 = stWeldPara.thick1/0.254+0.5;
                        stWeldPara.wide = stWeldPara.wide/0.254+0.5;
                        stWeldPara.thick2 = stWeldPara.thick2/0.254+0.5;
                        stWeldPara.high1 = stWeldPara.high1/0.254+0.5;
                        stWeldPara.high2 = stWeldPara.high2/0.254+0.5;
                        stWeldPara.radius = stWeldPara.radius/0.254+0.5;
                        stWeldPara.deep = stWeldPara.deep/0.254+0.5;
                        stWeldPara.blunt = stWeldPara.blunt/0.254+0.5;
                        stWeldPara.space = stWeldPara.space/0.254+0.5;
                        stWeldPara.probe = stWeldPara.probe/0.254+0.5;
                        //////
                    }
                    else
                    {
                        MSetSpeed( (u_int)(ChannelPara.Speed*0.254+0.5) ,C_SETMODE_SAVE);
                        MSetThick( (u_int)(MGetThick()*25.4/100+0.5) );
                        MSetDiameter( (u_int)(MGetDiameter()*25.4/100+0.5) );
                        MSetCrystal_l((ChannelPara.Crystal_l)*25.4+0.5,C_SETMODE_SETSAVE);
                        if (ChannelPara.Crystal_w>0)
                        {
                            MSetCrystal_w((ChannelPara.Crystal_w)*25.4+0.5,C_SETMODE_SETSAVE);
                        }
                        MSetForward((u_int)( MGetForward()*25.4/100+0.5),C_SETMODE_SAVE);
                        MSetRange( (u_int)( MGetRange(1)*0.254+0.5 ),C_SETMODE_SAVE);						//////

                        stWeldPara.face = stWeldPara.face*0.254+0.5;
                        stWeldPara.thick1 = stWeldPara.thick1*0.254+0.5;
                        stWeldPara.wide = stWeldPara.wide*0.254+0.5;
                        stWeldPara.thick2 = stWeldPara.thick2*0.254+0.5;
                        stWeldPara.high1 = stWeldPara.high1*0.254+0.5;
                        stWeldPara.high2 = stWeldPara.high2*0.254+0.5;
                        stWeldPara.radius = stWeldPara.radius*0.254+0.5;
                        stWeldPara.deep = stWeldPara.deep*0.254+0.5;
                        stWeldPara.blunt = stWeldPara.blunt*0.254+0.5;
                        stWeldPara.space = stWeldPara.space*0.254+0.5;
                        stWeldPara.probe = stWeldPara.probe*0.254+0.5;
                    }

                    ChannelParaStore();
                }

            }
            MSetChannel(deci_len,C_SETMODE_SETSAVE);
            SystemParaStore();
            ChannelParaRestore();
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

        InvertWords(C_ECHAR_HDOT,ypos-MGetLanguage()*C_CCHAR_VDOT, 1); /*������ʾ�����ֵ�����*/

        if( page == 1)
        {
            switch (keycode)
            {
            case 1:		//���
                //	cpText = (u_short*)_ParaMenu_AB1 ;
                MPushWindow(xpos, ypos, xpos + (6+MGetLanguage())*C_CCHAR_HDOT+1, ypos + crow_height * 3+1) ;
                EraseDrawRectangle(xpos, ypos, xpos + (6+MGetLanguage())*C_CCHAR_HDOT, ypos + crow_height * 3) ;
                //	CEMenuOut(xpos+4,ypos+4 , cpText ,3,3,crow_height);	/*��ָ��λ�ø���ÿ���ַ������������и�д�˵�*/
                TextOut(xpos+4,ypos+4 ,3,10,crow_height,(u_char*)_ParaMenu_AB1[MGetLanguage()][0],4);
                while(1)
                {
                    keycode = MAnyKeyReturn();
                    if (keycode == C_KEYCOD_RETURN||keycode == C_KEYCOD_CONFIRM)
                        break;
                    else if(keycode >= 1 && keycode <= 3)
                    {
                        MSetScaleMode(keycode-1,C_SETMODE_SAVE);
                        break;
                    }
                }
                MPopWindow();
                break;
            case 2:	//���油��
                deci_len = 1;
                number = 100;
                while(1)
                {
                    if( Input_Number(xpos,ypos-C_ECHAR_VDOT,&number,2, &deci_len,0) != 1)break;
                    else
                    {
                        if( number <= C_MAX_SURFGAIN)
                        {
                            MSetSurfGain(number,C_SETMODE_SAVE);
                            break;
                        }
                        else	DisplayPrompt(4);
                    }
                }

                break;


            case 3:	//�������
                if( MGetUnitType() > 0)deci_len = 3;//��λinch
                else deci_len = 1;
                number = 100;
                while( true )//��δ������������������
                {
                    int i,j,k;
                    if( Input_Number(xpos,ypos-C_ECHAR_VDOT,&number,4, &deci_len,0) != 1)
                        break;
                    else if( number < 50000)
                    {
                        MSetThick(number);
                        number /= 10;
                        if( MGetUnitType() > 0)number = (UINT)(number * 254/1000.0+0.5);//��λinch
                        k = c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_THNUM];
                        for( i = 0 ; i < k; i++)
                        {
                            if(MGetStdMode()==0)break;
                            if(MGetStdMode()==10||MGetStdMode()==11)
                            {
                                if( number >=c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_THICK+7*i] )continue;

                            }
                            else
                            {
                                if( number >c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_THICK+7*i] )continue;

                            }

                            for( j = 0; j < MGetCurveNumber(); j++)
                            {
                                MSetLineGain(j, c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_THICK+7*i+j+1] );
                            }
                            break;
                        }
                        break;
                    }
                }
                int j;
                if(MGetStdMode()==12)//�жϺ���Ƿ��ڷ�Χ��
                {

                    if( (MGetBlock()==0&&(MGetThick()<60||MGetThick()>2000))\
                            ||(MGetBlock()==1&&(MGetThick()<80||MGetThick()>1200))\
                            ||(MGetBlock()==2&&(MGetThick()<=2000||MGetThick()>5000))\
                            ||(MGetBlock()==3&&MGetThick()<40)    )

                    {
                        for( j = 0; j < MGetCurveNumber(); j++)
                        {
                            MSetLineGain(j, 0 );
                        }
                    }

                }

                else if(MGetStdMode()==2)//�жϺ���Ƿ��ڷ�Χ��
                {
                    if( (MGetBlock()==0&&(MGetThick()<80||MGetThick()>1200))\
                            ||(MGetBlock()==1&&(MGetThick()<60||MGetThick()>1200))\
                            ||(MGetBlock()==2&&(MGetThick()<=1200||MGetThick()>4000))\
                            ||(MGetBlock()==3&&MGetThick()<40) )
                    {
                        for( j = 0; j < MGetCurveNumber(); j++)
                        {
                            MSetLineGain(j, 0 );
                        }
                    }
                }
                else if(MGetStdMode()==4)//�жϺ���Ƿ��ڷ�Χ��
                {
                    if( MGetThick()<50)
                    {
                        for( j = 0; j < MGetCurveNumber(); j++)
                        {
                            MSetLineGain(j, 0 );
                        }
                    }
                }

                stdswitch=1;
                break;
            case 4:
                if( MGetUnitType() > 0)deci_len = 3;//��λinch
                else deci_len = 1;
                number = 100;
                while( true )//��δ������������������
                {
                    if( Input_Number(xpos,ypos-C_ECHAR_VDOT,&number,4, &deci_len,0) != 1)
                        break;
                    else if( number < 50000)
                    {
                        MSetDiameter(number);
                        break;
                    }
                }
                break;

            case 5:	//������׼
                //cpText = (u_short*)_ParaMenu_AC2 ;
                if( MGetCurveNumber() > 3)ypos -= crow_height * ( MGetCurveNumber()-3 );
                MPushWindow(xpos, ypos, xpos + (6+MGetLanguage())*C_CCHAR_HDOT+1, ypos + crow_height * ( MGetCurveNumber()+1 )+1 ) ;
                EraseDrawRectangle(xpos, ypos, xpos + (6+MGetLanguage())*C_CCHAR_HDOT, ypos + crow_height * ( MGetCurveNumber()+1 )) ;
                //CEMenuOut(xpos+4,ypos+4 , cpText, 4,MGetCurveNumber()+1,crow_height);	/*��ָ��λ�ø���ÿ���ַ������������и�д�˵�*/
                TextOut(xpos+4,ypos+4 ,MGetCurveNumber()+1,10,crow_height,(u_char*)_ParaMenu_AC2[MGetLanguage()][0],4);
                while(1)
                {
                    keycode = MAnyKeyReturn();
                    if (keycode == C_KEYCOD_RETURN||keycode == C_KEYCOD_CONFIRM)
                        break;
                    else if(keycode > 0 && keycode <= MGetCurveNumber()+1 )
                    {
                        MSetDacMode(keycode-1,C_SETMODE_SAVE);
                        break;
                    }
                }
                MPopWindow();
                break;
            case 6:	//������ʾ
                //	if( MGetTestStatus(C_TEST_AVG) == 1)
                //		cpText = (u_short*)_ParaMenu_AC3A ;
                //	else cpText = (u_short*)_ParaMenu_AC3 ;
                MPushWindow(xpos, ypos, xpos + (5+MGetLanguage())*C_CCHAR_HDOT+1, ypos + crow_height * 2+1) ;
                EraseDrawRectangle(xpos, ypos, xpos + (5+MGetLanguage())*C_CCHAR_HDOT, ypos + crow_height * 2) ;
                //	CEMenuOut(xpos+4,ypos+4 , cpText ,3,2,crow_height);	/*��ָ��λ�ø���ÿ���ַ������������и�д�˵�*/
                TextOut(xpos+4,ypos+4 ,2,8,crow_height,(u_char*)_ParaMenu_AC3A[MGetLanguage()][0],4);
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
                break;
            case 7:		//Rl
                if( MGetParaLock(1,C_LOCK_DACAVG) == 1)break;

                if(MGetTestStatus(C_TEST_AVG) == 1 && MGetUnitType() > 0)deci_len = 3;
                else deci_len = 1;
                number = 0;
                if( MGetTestStatus(C_TEST_AVG) == 1) sign = 0;
                else sign = 1;

                while( 1 )
                {
                    if( Input_Number(xpos,ypos-C_ECHAR_VDOT,&number,3, &deci_len,sign) < 1)break;
                    else
                    {
                        if(MGetTestStatus(C_TEST_AVG) == 1)
                        {
                            if(MGetUnitType() > 0)
                            {
                                number = (UINT)(number * 254/1000.0+0.5);
                            }
                            if( number >= 5 && number < 100)
                                number = (int)( 400 * log10( number/10.0) -120);
                            else break;

                        }
                        else if(number < -300 || number > 400)break;
                        MSetLineGain(0, number);
                        break;
                    }
                }

                break;
            case 8:
                if( MGetParaLock(1,C_LOCK_DACAVG) == 1)break;

                if(MGetTestStatus(C_TEST_AVG) == 1 && MGetUnitType() > 0)deci_len = 3;
                else deci_len = 1;
                number = 0;
                if(MGetTestStatus(C_TEST_AVG) == 1)sign = 0;
                else sign = 1;
                while( 1 )
                {
                    if( Input_Number(xpos,ypos-C_ECHAR_VDOT,&number,3, &deci_len,sign) < 1)break;
                    else
                    {
                        if(MGetTestStatus(C_TEST_AVG) == 1)
                        {
                            //AVG
                            if(MGetUnitType() > 0)
                            {
                                number = (UINT)(number * 254/1000.0+0.5);
                            }
                            if( number >= 5 && number < 100)
                                number = (int)( 400 * log10( number/10.0) -120);
                            else break;
                        }
                        else if(number < -300 || number > 400)break;

                        MSetLineGain(1, number);
                        break;
                    }
                }

                /*		while( 1 )//��δ������������������
                		{
                			if( Input_Number(xpos,ypos,&number,3, &deci_len,1) != 1)break;
                			else if(number >= -300 && number <= 400)
                			{
                				MSetSlGain(number,C_SETMODE_SAVE);
                				break;
                			}
                		}
                */
                break;
            case 9:	//El
                if( MGetParaLock(1,C_LOCK_DACAVG) == 1)break;

                if(MGetTestStatus(C_TEST_AVG) == 1 && MGetUnitType() > 0)deci_len = 3;
                else deci_len = 1;
                number = 0;
                if(MGetTestStatus(C_TEST_AVG) == 1)sign = 0;
                else sign = 1;
                while( 1 )
                {
                    if( Input_Number(xpos,ypos-C_ECHAR_VDOT,&number,3, &deci_len,sign) < 1)break;
                    else
                    {
                        if(MGetTestStatus(C_TEST_AVG) == 1)
                        {
                            if(MGetUnitType() > 0)
                            {
                                number = (UINT)(number * 254/1000.0+0.5);
                            }
                            if( number >= 5 && number < 100)
                                number = (int)( 400 * log10( number/10.0) -120);
                            else break;
                        }
                        else if(number < -300 || number > 400)break;

                        MSetLineGain(2, number);
                        break;
                    }
                }
                break;
            }
        }
        else if( page == 2)
        {
            switch (keycode)
            {

#if C_UDISK == 1
            case 1:	//                //*
                if (MGetSaveStatus() > 0)
                {
                    break;
                }
                MPushWindow(xpos, ypos, xpos + (5+MGetLanguage())*C_CCHAR_HDOT+1, ypos + crow_height * 2+1) ;
                EraseDrawRectangle(xpos, ypos, xpos + (5+MGetLanguage())*C_CCHAR_HDOT, ypos + crow_height * 2) ;
                TextOut(xpos+4,ypos+4 ,2,8,crow_height,(u_char*)_ParaMenu_AC1[MGetLanguage()][0],4);
                while(1)
                {
                    keycode = MAnyKeyReturn();
                    if (keycode == C_KEYCOD_RETURN||keycode == C_KEYCOD_CONFIRM)
                        break;
                    else if(keycode >= 1 && keycode <= 2)
                    {
                        MSetSaveMode(keycode-1,C_SETMODE_SETSAVE);
                        if (MGetSaveMode())
                        {
                            MSetSaveStatus( 0,C_SETMODE_SETSAVE);
                            MSetSavePosition(1,C_SETMODE_SETSAVE);
                        }
                        else
                        {
                            MSetSavePosition(0,C_SETMODE_SETSAVE);
                        }
                        /*	if( MGetSaveTime() > 600 || MGetSaveTime() < 1)
                        	{
                        		MSetSaveTime(10,C_SETMODE_SAVE);
                        		offset = C_OFF_STORE +	34;		//���Զ������ݸ���
                        		number = 0;
                        		MCopyProtectedMemory( (void*)offset, &number,4, PM_COPY_WRITE);
                        	}*/
                        break;
                    }
                }
                MPopWindow();
                //*/
                break;

            case 2:	//�洢λ��
                break;
                if (MGetSaveStatus() > 0)
                {
                    break;
                }
                ypos -= crow_height * 1;
                MPushWindow(xpos, ypos, xpos + (5+MGetLanguage())*C_CCHAR_HDOT+1, ypos + crow_height * 2+1) ;
                EraseDrawRectangle(xpos, ypos, xpos + (5+MGetLanguage())*C_CCHAR_HDOT, ypos + crow_height * 2) ;
                TextOut(xpos+4,ypos+4 ,2,10,crow_height,(u_char*)_ParaMenu_AC7[MGetLanguage()][0],4);
                while(1)
                {
                    keycode = MAnyKeyReturn();
                    if (keycode == C_KEYCOD_RETURN||keycode == C_KEYCOD_CONFIRM)
                        break;
                    else if(keycode >= 1 && keycode <= 2)
                    {
                        MSetSavePosition(keycode-1,C_SETMODE_SETSAVE);
                        break;
                    }
                }
                MPopWindow();
                break;
                /*#else
                			case 1://������λ
                				epText = (u_char*)_ParaMenu_CA1 ;
                				MPushWindow(xpos, ypos, xpos + 4*C_CCHAR_HDOT, ypos + 12 * 3) ;
                				EraseDrawRectangle(xpos, ypos, xpos + 4*C_CCHAR_HDOT, ypos + 12 * 3) ;
                				EMenuOut(xpos+4,ypos+8 , epText ,6,2,12);
                				number = MGetUnitType();
                				while(1)
                				{
                					keycode = MAnyKeyReturn();
                					if (keycode == C_KEYCOD_RETURN||keycode == C_KEYCOD_CONFIRM)
                						break;
                					else if(keycode >= 1 && keycode <= 2)
                					{
                						MSetUnitType(keycode-1);
                						break;
                					}
                				}
                				if( old_unit1 != MGetUnitType() )//���ȵ�λ�иı�
                				{
                					if( old_unit1 == 0)
                					{//�µ�λinch
                						MSetThick( (u_int)(MGetThick()/0.254+0.5) );
                						MSetDiameter( (u_int)(MGetDiameter()/0.254+0.5) );

                						stWeldPara.face = stWeldPara.face/0.254+0.5;
                						stWeldPara.thick1 = stWeldPara.thick1/0.254+0.5;
                						stWeldPara.wide = stWeldPara.wide/0.254+0.5;
                						stWeldPara.thick2 = stWeldPara.thick2/0.254+0.5;
                						stWeldPara.high1 = stWeldPara.high1/0.254+0.5;
                						stWeldPara.high2 = stWeldPara.high2/0.254+0.5;
                						stWeldPara.radius = stWeldPara.radius/0.254+0.5;
                						stWeldPara.deep = stWeldPara.deep/0.254+0.5;
                						stWeldPara.blunt = stWeldPara.blunt/0.254+0.5;
                						stWeldPara.space = stWeldPara.space/0.254+0.5;
                						stWeldPara.probe = stWeldPara.probe/0.254+0.5;
                					}
                					else
                					{
                						MSetThick( (u_int)(MGetThick()*25.4/100+0.5) );
                						MSetDiameter( (u_int)(MGetDiameter()*25.4/100+0.5) );

                						stWeldPara.face = stWeldPara.face*0.254+0.5;
                						stWeldPara.thick1 = stWeldPara.thick1*0.254+0.5;
                						stWeldPara.wide = stWeldPara.wide*0.254+0.5;
                						stWeldPara.thick2 = stWeldPara.thick2*0.254+0.5;
                						stWeldPara.high1 = stWeldPara.high1*0.254+0.5;
                						stWeldPara.high2 = stWeldPara.high2*0.254+0.5;
                						stWeldPara.radius = stWeldPara.radius*0.254+0.5;
                						stWeldPara.deep = stWeldPara.deep*0.254+0.5;
                						stWeldPara.blunt = stWeldPara.blunt*0.254+0.5;
                						stWeldPara.space = stWeldPara.space*0.254+0.5;
                						stWeldPara.probe = stWeldPara.probe*0.254+0.5;
                					}
                					old_unit1=MGetUnitType();
                				}
                				MPopWindow();

                				break;
                			case 2://����
                				MPushWindow(xpos, ypos, xpos + 6*C_CCHAR_HDOT, ypos + 16 * 3) ;
                				EraseDrawRectangle(xpos, ypos, xpos + 6*C_CCHAR_HDOT, ypos + 16 * 3) ;

                				TextOut(xpos+4,ypos+8,1,10,16,(char *)_MainMenu_C1A11[MGetLanguage()][0],4);
                				TextOut(xpos+4,ypos+8+20,1,10,16,(char *)_MainMenu_C1A11[MGetLanguage()][1],4);
                				while(1)
                				{
                					keycode = MAnyKeyReturn();
                					if (keycode == C_KEYCOD_RETURN||keycode == C_KEYCOD_CONFIRM)
                						break;
                					else if(keycode >= 1 && keycode <= 2)
                					{
                						MSetLanguage(keycode-1);
                						break;
                					}
                				}
                				MPopWindow();

                				break;*/
#endif
            case 3:	//λ����ʾ
                //	cpText = (u_short*)_ParaMenu_AC6 ;
                MPushWindow(xpos, ypos, xpos + (5+MGetLanguage())*C_CCHAR_HDOT+1, ypos + crow_height * 2+1) ;
                EraseDrawRectangle(xpos, ypos, xpos + (5+MGetLanguage())*C_CCHAR_HDOT, ypos + crow_height * 2) ;
                //	CEMenuOut(xpos+4,ypos+4 , cpText ,3,2,crow_height);	/*��ָ��λ�ø���ÿ���ַ������������и�д�˵�*/
                TextOut(xpos+4,ypos+4,2,8,crow_height,(u_char*)_ParaMenu_AC6[MGetLanguage()][0],4);
                while(1)
                {
                    keycode = MAnyKeyReturn();
                    if (keycode == C_KEYCOD_RETURN||keycode == C_KEYCOD_CONFIRM)
                        break;
                    else if(keycode >= 1 && keycode <= 2)
                    {
                        MSetReadForeland(keycode-1);
                        break;
                    }
                }
                MPopWindow();
                break;


            case 4://����
                MPushWindow(xpos, ypos, xpos + 7*C_CCHAR_HDOT+1, ypos + crow_height * 2+1) ;
                EraseDrawRectangle(xpos, ypos, xpos + 7*C_CCHAR_HDOT, ypos + crow_height * 2) ;

                TextOut(xpos+4,ypos+8,1,10,crow_height,(char *)_MainMenu_C1A11[MGetLanguage()][0],4);
                TextOut(xpos+4,ypos+8+30,1,10,crow_height,(char *)_MainMenu_C1A11[MGetLanguage()][1],4);
                while(1)
                {
                    keycode = MAnyKeyReturn();
                    if (keycode == C_KEYCOD_RETURN||keycode == C_KEYCOD_CONFIRM)
                        break;
                    else if(keycode >= 1 && keycode <= 2)
                    {
                        MSetLanguage(keycode-1);
                        break;
                    }
                }
                MPopWindow();

                break;
            case 5:	//B������
                if( MGetFunctionMode(C_CURVER_AMEND ) )break;
                MPushWindow(xpos, ypos, xpos + (5+MGetLanguage()*3)*C_CCHAR_HDOT+1, ypos + crow_height * 2+1) ;
                EraseDrawRectangle(xpos, ypos, xpos + (5+2*MGetLanguage())*C_CCHAR_HDOT, ypos + crow_height * 2) ;
                TextOut(xpos+4,ypos+4,1,8+2*MGetLanguage(),crow_height,(u_char*)_ParaMenu_AC5[MGetLanguage()][0],4);
                TextOut(xpos+4,ypos+4+crow_height,1,8+2*MGetLanguage(),crow_height,(u_char*)_ParaMenu_AC5[MGetLanguage()][1],4);
                while(1)
                {
                    keycode = MAnyKeyReturn();
                    if (keycode == C_KEYCOD_RETURN||keycode == C_KEYCOD_CONFIRM)
                        break;
                    else if(keycode >= 1 && keycode <= 2)
                    {
                        MSetBGateMode(keycode-1,C_SETMODE_SETSAVE);
                        break;
                    }
                }
                MPopWindow();
                break;
            case 6:
                //	cpText = (u_short*)_ParaMenu_AD8 ;
                MPushWindow(xpos, ypos, xpos + (4+MGetLanguage())*C_CCHAR_HDOT+2, ypos + crow_height * 2+2) ;
                EraseDrawRectangle(xpos, ypos, xpos + (4+MGetLanguage())*C_CCHAR_HDOT, ypos + crow_height * 2) ;
                //	CEMenuOut(xpos+4,ypos+4 , cpText ,2,2,crow_height);	/*��ָ��λ�ø���ÿ���ַ������������и�д�˵�*/
                TextOut(xpos+4,ypos+4,2,6,crow_height,(u_char*)_ParaMenu_AD8[MGetLanguage()][0],4);
                while(1)
                {
                    keycode = MAnyKeyReturn();
                    if (keycode == C_KEYCOD_RETURN||keycode == C_KEYCOD_CONFIRM)
                        break;
                    else if(keycode >= 1 && keycode <= 2)
                    {
                        MSetSoundAlarm(0, keycode-1);
                        break;
                    }
                }
                MPopWindow();
                break;
            case 7://��Ļ����
#if C_LIGHT == 1
#if	C_DEVLIB == 1 || C_DEVLIB == 3 || C_DEVLIB == 24 ||C_DEVLIB == 23
                ypos-=C_CCHAR_HDOT+30;
                //	cpText = (u_short*)_ParaMenu_AC4 ;
                MPushWindow(xpos, ypos, xpos + (6+MGetLanguage())*C_CCHAR_HDOT+1, ypos + crow_height * 4+1) ;
                EraseDrawRectangle(xpos, ypos, xpos + (6+MGetLanguage())*C_CCHAR_HDOT, ypos + crow_height * 4) ;
                //	CEMenuOut(xpos+4,ypos+4 , cpText ,3,4,crow_height);	//��ָ��λ�ø���ÿ���ַ������������и�д�˵�
                TextOut(xpos+4,ypos+4 ,4,10,crow_height,(u_char*)_ParaMenu_AC4[MGetLanguage()][0],4);
                while(1)
                {
                    keycode = MAnyKeyReturn();
                    if (keycode == C_KEYCOD_RETURN||keycode == C_KEYCOD_CONFIRM)
                        break;
                    else if(keycode >= 1 && keycode <= 4)
                    {
                        MSetScreenBrightness(keycode-1,C_SETMODE_SETSAVE);
                        break;
                    }
                }
                MPopWindow();
#else		//C_DEVLIB == 23		//�������������
                //	cpText = (u_short*)_ParaMenu_AC4 ;
                break;
                MPushWindow(xpos, ypos, xpos + 6*C_CCHAR_HDOT, ypos + crow_height * 2) ;
                EraseDrawRectangle(xpos, ypos, xpos + (6)*C_CCHAR_HDOT, ypos + crow_height * 2) ;
                //	CEMenuOut(xpos+4,ypos+4 , cpText ,3,1,crow_height);	//��ָ��λ�ø���ÿ���ַ������������и�д�˵�
                //	CEMenuOut(xpos+4,ypos+4+crow_height , cpText+3 ,1,1,crow_height);	//��ָ��λ�ø���ÿ���ַ������������и�д�˵�
                //	CEMenuOut(xpos+4+16,ypos+4+crow_height , cpText+3*3+1 ,2,1,crow_height);	//��ָ��λ�ø���ÿ���ַ������������и�д�˵�
                TextOut(xpos+4,ypos+4 ,1,10,crow_height,(u_char*)_ParaMenu_AC4[MGetLanguage()][0],4);
                TextOut(xpos+4,ypos+4+crow_height ,1,10,crow_height,(u_char*)_ParaMenu_AC4[MGetLanguage()][4],4);
                while(1)
                {
                    keycode = MAnyKeyReturn();
                    if (keycode == C_KEYCOD_RETURN||keycode == C_KEYCOD_CONFIRM)
                        break;
                    else if(keycode == 1)
                    {
                        MSetScreenBrightness(0,C_SETMODE_SETSAVE);
                        break;
                    }
                    else if(keycode == 2)
                    {
                        MSetScreenBrightness(3,C_SETMODE_SETSAVE);
                        break;
                    }
                }
                MPopWindow();
#endif
#endif
                break;
            case 8:
                deci_len = 0;
                number = 100;
                while( true )//��δ������������������
                {
                    if( Input_Number(xpos,ypos-C_ECHAR_VDOT,&number,2, &deci_len,0) != 1)
                        break;
                    else if(number >= 40 && number <= 90)
                    {
                        MSetAmpStdMax( number);
                        break;
                    }
                }
                break;
            case 9:			//����ͼʾ
                if( MGetTestStatus(C_TEST_AVG) == 1)break;

                //	cpText = (u_short*)_Weld_C1 ;
                ypos -= crow_height * 3+8+24*MGetLanguage();
                MPushWindow(xpos, ypos, xpos + (5+MGetLanguage())*C_CCHAR_HDOT+11, ypos + crow_height * 4+1) ;
                EraseDrawRectangle(xpos, ypos, xpos + (5+MGetLanguage())*C_CCHAR_HDOT+10, ypos + crow_height * 4) ;
                //	CEMenuOut(xpos+4,ypos+4 , cpText ,3,4,crow_height);
                for(i = 0; i < 4; i ++)
                {
                    TextOut(xpos+4,ypos+4+crow_height*i,1,8,16,(u_char*)_Weld_C1[MGetLanguage()][i],4);
                }
                while(1)
                {
                    keycode = MAnyKeyReturn();
                    if (keycode == C_KEYCOD_RETURN||keycode == C_KEYCOD_CONFIRM)
                        break;
                    else if(keycode >= 1 && keycode <= 4)
                    {
                        stWeldPara.type = keycode -1;
                        break;
                    }
                }
                MPopWindow();
                if( ( keycode > 1 && keycode <= 4) || keycode == C_KEYCOD_CONFIRM)
                {
                    //��ѡ���˺�������
                    int i,j,k;
                    if( stWeldPara.type == 3 && stWeldPara.face == 2)
                    {
                        stWeldPara.thick2 = MGetThick();
                    }
                    else
                    {
                        stWeldPara.thick1 = MGetThick();
                    }
                    WeldSetup();
                    if( stWeldPara.type > 0)
                    {
                        if( stWeldPara.type == 3 && stWeldPara.face ==2)
                        {
                            MSetThick( stWeldPara.thick2 );
                            number = stWeldPara.thick2;
                        }
                        else
                        {
                            MSetThick( stWeldPara.thick1 );
                            number = stWeldPara.thick1;
                        }
                        number /= 10;
                        if( MGetUnitType() > 0)number = (UINT)(number * 254/1000.0+0.5);//��λinch
                        k = c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_THNUM];
                        for( i = 0 ; i < k; i++)
                        {
                            if(MGetStdMode()==0)break;
                            if(MGetStdMode()==10||MGetStdMode()==11)
                            {
                                if( number >=c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_THICK+7*i] )continue;

                            }
                            else
                            {
                                if( number >c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_THICK+7*i] )continue;

                            }
                            for( j = 0; j < MGetCurveNumber(); j++)
                            {
                                MSetLineGain(j, c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_THICK+7*i+j+1] );
                            }
                            break;
                        }
                    }
                }
                break;

                /*	case 9:	//��������ʱ��

                	if (MGetSaveStatus() > 0)
                	{
                		break;
                	}
                	deci_len = 1;
                	number = 0;
                	while( MGetSaveMode() )//��δ������������������
                	{
                		if( Input_Number(xpos,ypos,&number,3, &deci_len,0) != 1)
                			break;
                		else if(number >= 1 && number <= 250)
                		{
                			MSetSaveTime(number,C_SETMODE_SAVE);
                			break;
                		}
                	}

                	break;*/
            }
            ExpendTime(20);
        }
        else if( page == 3)
        {
            switch (keycode)
            {

            case 1:	//��ӡ��
                break;
                epText = (u_char*)_ParaMenu_AB3;
                //ypos -= erow_height * 4 ;
                MPushWindow(xpos, ypos, xpos + 10*C_CCHAR_HDOT+1, ypos + erow_height * 4+1) ;
                EraseDrawRectangle(xpos, ypos, xpos + 10*C_CCHAR_HDOT, ypos + erow_height * 4) ;
                EMenuOut(xpos+4,ypos+4, epText ,15,4,erow_height);	/*��ָ��λ�ø���ÿ���ַ������������и�д�˵�*/
                while(1)
                {
                    keycode = MAnyKeyReturn();
                    if (keycode == C_KEYCOD_RETURN||keycode == C_KEYCOD_CONFIRM)
                        break;
                    else if(keycode >= 1 && keycode <= 4)
                    {
                        MSetPrinterMode(keycode-1,0/*mode = 0\1\2*/);	/*����̽ͷģʽ*/
                        break;
                    }
                }
                MPopWindow();

                break;

            case 2://�Ƿ��Ƶ��
                //	cpText = (u_short*)_ParaMenu_AD9 ;
                MPushWindow(xpos, ypos, xpos + 4*C_CCHAR_HDOT+1, ypos + crow_height * 2) ;
                EraseDrawRectangle(xpos, ypos, xpos + 4*C_CCHAR_HDOT, ypos + crow_height * 2) ;
                //	CEMenuOut(xpos+4,ypos+4 , cpText ,2,2,crow_height);	/*��ָ��λ�ø���ÿ���ַ������������и�д�˵�*/
                TextOut(xpos+4,ypos+4,2,5,crow_height,(u_char*)_ParaMenu_AD9[MGetLanguage()][0],4);
                while(1)
                {
                    keycode = MAnyKeyReturn();
                    if (keycode == C_KEYCOD_RETURN||keycode == C_KEYCOD_CONFIRM)
                        break;
                    else if(keycode >= 1 && keycode <= 2)
                    {
                        MSetSaveFrequency(keycode-1,0);
                        break;
                    }
                }
                if (MGetFrequence() != 0)
                {
                    MSetFunctionMode(0,C_ECHO_ENVELOPE);
                    MSetFunctionMode(0,C_ECHOMAX_MEMORY);
                }
                MPopWindow();
                break;
            case 3:
                ypos -= 24;
                //	cpText = (u_short*)_ParaProtect1 ;//4-6ȥ��
                MPushWindow(xpos, ypos, xpos+ (12+7*MGetLanguage())*C_CCHAR_HDOT+1, ypos + 5 * crow_height+1) ;
                EraseDrawRectangle(xpos, ypos, xpos+ (12+7*MGetLanguage())*C_CCHAR_HDOT, ypos + 5 * crow_height) ;
                //	CMenuOut(xpos + C_ECHAR_HDOT,ypos+4,cpText,6,5,crow_height);

                int i = 0;
                for (i=0; i<5; i++)
                {
                    TextOut(xpos+ C_ECHAR_HDOT,ypos+4+i*crow_height,1,12+9*MGetLanguage(),crow_height,(u_char*)_ParaProtect1[MGetLanguage()][i],4);
                }
                // if (MGetLanguage())
                {
                    //     TextOut(xpos+ C_ECHAR_HDOT,ypos+4+4*crow_height-8,1,12,8,(u_char*)_ParaProtect1[MGetLanguage()][3]+12,0);
                }
                int ypos1;

                xpos += (6+6*MGetLanguage())*C_CCHAR_HDOT+MGetLanguage();

                for(i = 0; i < C_LOCK_ALL; i ++)
                {
                    ypos1 = ypos + crow_height * (i +2);
                    //	cpText = (u_short*)_ParaProtect2 + MGetParaLock(0,i) * 2 ;
                    //	CMenuOut(xpos + C_ECHAR_HDOT,ypos1,cpText,2,1,crow_height);

                    MEraseWindow(xpos+ C_ECHAR_HDOT-1,ypos1, xpos+ C_ECHAR_HDOT+8*C_ECHAR_HDOT,ypos1+C_ECHAR_VDOT) ;
                    TextOut(xpos+ C_ECHAR_HDOT,ypos1,1,8,crow_height,(u_char*)_ParaProtect2[MGetLanguage()][MGetParaLock(0,i)],0);

                }

                do
                {
                    keycode = MAnyKeyReturn();
                    if (keycode == C_KEYCOD_RETURN || keycode == C_KEYCOD_CONFIRM)
                        break;

                    if (keycode == 0)
                    {
                        if(MGetParaLock(0,0)==1 || MGetParaLock(0,1)==1 || MGetParaLock(0,2)==1)
                        {
                            MSetParaLock(0, C_LOCK_ALL);	//���ж�������
                        }
                        else
                        {
                            MSetParaLock(1, C_LOCK_ALL);	//���ж�������
                        }


                    }
                    else if(keycode >0 && keycode <= C_LOCK_ALL)
                    {
                        MSetParaLock(MGetParaLock(0,keycode-1)+1, keycode-1);	//
                    }
                    else continue;
                    for(i = 0; i < C_LOCK_ALL; i ++)
                    {
                        ypos1 = ypos + crow_height * (i +2);
                        //	cpText = (u_short*)_ParaProtect2 + MGetParaLock(0,i) * 2 ;
                        //	CMenuOut(xpos + C_ECHAR_HDOT,ypos1,cpText,2,1,crow_height);
                        MEraseWindow(xpos+ C_ECHAR_HDOT-1,ypos1, xpos+ C_ECHAR_HDOT+8*C_ECHAR_HDOT,ypos1+C_ECHAR_VDOT) ;
                        TextOut(xpos+ C_ECHAR_HDOT,ypos1,1,8,crow_height,(u_char*)_ParaProtect2[MGetLanguage()][MGetParaLock(0,i)],0);
                    }
                }
                while(1);
                MPopWindow();
                break;
            case 4://������λ
                epText = (u_char*)_ParaMenu_CA1 ;
                MPushWindow(xpos, ypos, xpos + 4*C_CCHAR_HDOT+8*MGetLanguage(), ypos + erow_height * 3) ;
                EraseDrawRectangle(xpos, ypos, xpos + 4*C_CCHAR_HDOT+8*MGetLanguage(), ypos + erow_height* 3) ;
                EMenuOut(xpos+4,ypos+8 , epText ,6,2,erow_height);	/*��ָ��λ�ø���ÿ���ַ������������и�д�˵�*/
                number = MGetUnitType();
                while(1)
                {
                    keycode = MAnyKeyReturn();
                    if (keycode == C_KEYCOD_RETURN||keycode == C_KEYCOD_CONFIRM)
                        break;
                    else if(keycode >= 1 && keycode <= 2)
                    {
                        MSetUnitType(keycode-1);
                        break;
                    }
                }
                if( old_unit1 != MGetUnitType() )//���ȵ�λ�иı�
                {
                    if( old_unit1 == 0)
                    {
                        //�µ�λinch
                        MSetThick( (u_int)(MGetThick()/0.254+0.5) );
                        MSetDiameter( (u_int)(MGetDiameter()/0.254+0.5) );

//                        stWeldPara.face = stWeldPara.face/0.254+0.5;
                        stWeldPara.thick1 = stWeldPara.thick1/0.254+0.5;
                        stWeldPara.wide = stWeldPara.wide/0.254+0.5;
                        stWeldPara.thick2 = stWeldPara.thick2/0.254+0.5;
                        stWeldPara.high1 = stWeldPara.high1/0.254+0.5;
                        stWeldPara.high2 = stWeldPara.high2/0.254+0.5;
                        stWeldPara.radius = stWeldPara.radius/0.254+0.5;
                        stWeldPara.deep = stWeldPara.deep/0.254+0.5;
                        stWeldPara.blunt = stWeldPara.blunt/0.254+0.5;
                        stWeldPara.space = stWeldPara.space/0.254+0.5;
                        stWeldPara.probe = stWeldPara.probe/0.254+0.5;
                    }
                    else
                    {
                        MSetThick( (u_int)(MGetThick()*25.4/100+0.5) );
                        MSetDiameter( (u_int)(MGetDiameter()*25.4/100+0.5) );

//                        stWeldPara.face = stWeldPara.face*0.254+0.5;
                        stWeldPara.thick1 = stWeldPara.thick1*0.254+0.5;
                        stWeldPara.wide = stWeldPara.wide*0.254+0.5;
                        stWeldPara.thick2 = stWeldPara.thick2*0.254+0.5;
                        stWeldPara.high1 = stWeldPara.high1*0.254+0.5;
                        stWeldPara.high2 = stWeldPara.high2*0.254+0.5;
                        stWeldPara.radius = stWeldPara.radius*0.254+0.5;
                        stWeldPara.deep = stWeldPara.deep*0.254+0.5;
                        stWeldPara.blunt = stWeldPara.blunt*0.254+0.5;
                        stWeldPara.space = stWeldPara.space*0.254+0.5;
                        stWeldPara.probe = stWeldPara.probe*0.254+0.5;
                    }
                    old_unit1=MGetUnitType();
                }
                MPopWindow();

                break;
            }
        }
    }
    SystemParaRestore();
    ChannelParaRestore();
    MSetScreenBrightness( MGetScreenBrightness(),C_SETMODE_SETSAVE);

    SetRepeatRate(g_nRepeat);
    if(extend==0)
    {
        MSetPulseWidthInput(MGetPulseWidthNumber(), C_SETMODE_SETSAVE);
    }
    else
    {
        MSetPulseWidth(MGetPulseWidth(), C_SETMODE_SETSAVE);
    }
    g_nDenoise = 0;
    g_nAutoDenoise = 0;
    MSetBaseGain(MGetBaseGain(), C_SETMODE_SETSAVE);
    MSetAcquisitionEnable(1,C_SETMODE_SETSAVE);
    MSetHardEchoShow(1,C_SETMODE_SETSAVE);
    MSetColor(C_CR_WAVE);
    ScreenRenovate();	/*��Ļˢ��*/
    ClearEnvelope();	//�������ֵ
    MSetColor(C_CR_UNDO);
    if( retvalue == C_TRUE && MGetTestStatus(C_TEST_DAC) == 1 && stdswitch==1)
    {
        MSetDelay(0,C_SETMODE_SETSAVE);
        DrawDac(2);		//�»�DAC
        if( MGetThick()*2 <= MGetRange(4) && MGetStdMode()!=11 )//if( MGetThick()*2 <= MGetRange(4) && MGetStdMode()<C_STD_MAX-1 )
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
            else if (MGetStdMode()==12)	//47013
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

        if (MGetThick()>=30 && stdswitch ==1)
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
        MSetSystem();
        MSetColor(C_CR_PARA);
        WriteBaseGain(C_BASEGAIN_HPOSI,C_BASEGAIN_VPOSI);
        WriteCompGain(C_COMPGAIN_HPOSI,C_COMPGAIN_VPOSI);
        WriteSurfGain(C_SURFGAIN_HPOSI,C_SURFGAIN_VPOSI);
        SystemParaStore();
        ChannelParaStore();
        ScreenRenovate();
    }
    DrawDac(0);		//�»�DAC
    return 1;
}

int ChannelMenu(void)
{
    int FileMax=(C_OFF_FAT+3)/4;//C_OFF_FILEMAX/4;//
    int offset = 0;

    u_short char_len = 6,row_number = 10,crow_height = 48,erow_height =20;
    int number,deci_len;//,sign;
    short xpos = 0,ypos = 0;
    //int menu_xpos = 0,menu_ypos = 0;
    short para_xpos = C_CCHAR_HDOT * ( char_len + 1) ,para_ypos =  4;
    int retvalue = C_TRUE;
    int keycode;
    short page_max = 1, page = 1,pagemax=0;
    int row;
    u_short* cpText = 0;
    u_char* epText = 0;
    u_char* epText1 = 0;
    int i,j,k;
    int stdflag=0;
    int testrange[3];

    u_int elapsedtime1 = GetElapsedTime();// ,elapsedtime2;
    MSetAcquisitionEnable(0,C_SETMODE_SETSAVE);
    MSetHardEchoShow(0,C_SETMODE_SETSAVE);
    ClearEnvelope();
    SystemParaStore();
    ChannelParaStore();



    DisplayPrompt(15);
    para_xpos = C_CCHAR_HDOT * ( char_len + 1) ;
    short row_number_page1, para_ypos_page1;
    short row_number_page2=5, para_ypos_page2=48;
#if  C_DEVTYPE == 3 || C_DEVTYPE == 5
    page_max = 2;
    row_number_page1 = 8;
    para_ypos_page1 =para_ypos_page2=  48;
    row_number_page2 = 2;
#elif  C_DEVTYPE == 4
    page_max = 1;
    row_number_page1 = 8;
    para_ypos_page1 =para_ypos_page2=  48;
    row_number_page2 = 0;
#elif  C_DEVTYPE == 20
    page_max = 1;
    row_number_page1 = 8;
    para_ypos_page1 =para_ypos_page2=  48;
    row_number_page2 = 0;
#elif  C_DEVTYPE == 12
    row_number_page1 = 6;
    para_ypos_page1 =  56;
#elif C_DEVTYPE == 8  || C_DEVTYPE == 1
    page_max = 2;
    row_number_page1 = 9;
    para_ypos_page1 =para_ypos_page2=  48;
    row_number_page2 = 3;
#elif C_DEVTYPE == 9  || C_DEVTYPE == 14 ||C_DEVTYPE == 15
    page_max = 2;
    row_number_page1 = 9;
    para_ypos_page1 =para_ypos_page2=  48;
    row_number_page2 = 5;
#else
    row_number_page1 = 9;
    para_ypos_page1 =  4;
#endif

    while(true)
    {
        MFclearScreen();


        MSetColor(C_CR_MENU);

        TextOut(C_ECHAR_HDOT,4,1,25,24,(u_char*)_ParaMenu_H1[MGetLanguage()][0],4);
        Write_Number(C_ECHAR_HDOT + 12 * C_ECHAR_HDOT+4, 4+3*MGetLanguage(),page,1,0,0);
        if (MGetLanguage())
        {
            Write_Number(C_ECHAR_HDOT + 24 * C_ECHAR_HDOT+8+6*MGetLanguage(), 7,page_max,1,0,0);
        }
        else
        {
            Write_Number(C_ECHAR_HDOT + 19 * C_ECHAR_HDOT+4, 4,page_max,1,0,0);
        }

        //CEMenuOut(C_ECHAR_HDOT,para_ypos, cpText ,char_len,row_number,crow_height);	/*��ָ��λ�ø���ÿ���ַ������������и�д�˵�*/
        for (i=0; i<row_number; i++)
        {
            switch(page)
            {
            case 1:
                epText1 = (char *)_ChannelMenu_A1[MGetLanguage()][i];
                epText =  (u_char*)_ChannelMenu_B1;
                row_number = row_number_page1;
                para_ypos =  para_ypos_page1-8*MGetLanguage();
                break;
            case 2:
                epText1 =  (char *)_ChannelMenu_A2[MGetLanguage()][i];
                epText =  (u_char*)_ChannelMenu_B2;
                row_number = row_number_page2;
                para_ypos =  para_ypos_page2-8*MGetLanguage();
                break;
            }
            TextOut(C_ECHAR_HDOT,para_ypos+crow_height*i,1,22,crow_height,epText1,0);
            //      if (MGetLanguage())
            {
                //         TextOut(C_ECHAR_HDOT,para_ypos+C_ECHAR_VDOT+crow_height*i,1,11,crow_height,epText1+11,2);
            }
        }
        if(MGetLanguage())
        {
            para_ypos+=C_ECHAR_VDOT;
        }
        EMenuOut(para_xpos+C_ECHAR_HDOT*5,para_ypos, epText ,3,row_number,crow_height);	/*��ָ��λ�ø���ÿ���ַ������������и�д�˵�*/

        EraseDrawRectangle(26*C_ECHAR_HDOT, 380, 26*C_ECHAR_HDOT + (6+1*MGetLanguage())*C_CCHAR_HDOT, 380 + 2 * (C_CCHAR_VDOT+12)) ;
        //	CMenuOut(27*C_ECHAR_HDOT,190+4,_MenuPrompt_B1,5,2,24);	/*��ָ��λ�ø���ÿ���ַ������������и�д�˵�*/

        TextOut(27*C_ECHAR_HDOT,380+4,1,10,24,(u_char*)_MenuPrompt_B1[MGetLanguage()][0],4);
        TextOut(27*C_ECHAR_HDOT,380+4+36,1,10,24,(u_char*)_MenuPrompt_B1[MGetLanguage()][0]+10,4);
//        EMenuOut(27*C_ECHAR_HDOT,380+4,_TestMenu_Enter,1,1,C_ECHAR_VDOT);
        MSetColor(C_CR_PARA);

        switch(page)
        {
        case 1:
            /*̽ͷ���� */
            row = 0;
            xpos = para_xpos;
            ypos = para_ypos + crow_height *row;
            epText = (char *)_ParaMenu_AB2[MGetLanguage()][MGetProbeMode()]+2;
            TextOut(xpos,ypos,1,8,16,epText,4);
            //	cpText = (u_short*)_ParaMenu_AB2 + ( (MGetProbeMode()-0)*4+1 );
            //	CEMenuOut(xpos,ypos , cpText ,3,1,crow_height);	/*��ָ��λ�ø���ÿ���ַ������������и�д�˵�*/

            //̽ͷƵ��
            row = 1;
            ypos = para_ypos + crow_height *row ;
            Write_Number(xpos,ypos,MGetFrequence(),5,2,0);

            //��Ƭ�ߴ�
            row = 2;
            ypos = para_ypos + crow_height *row ;
            Write_Crystal(xpos,ypos,MGetCrystal_l(),MGetCrystal_w());
            //	EMenuOut(xpos + 5*8,ypos,"mm",2,1,12);

            //�����
            row = 3;
            ypos = para_ypos + crow_height *row ;
            if (MGetAngle(0)>=C_MAX_ANGLE)
            {
                TextOut(xpos,ypos-6,1,3,10,(char *)_infinity[MGetLanguage()][0],0);
            }
            else
            {
                Write_Number(xpos,ypos,( MGetAngle(1) + 50)/100,4,2,0);	//K value
            }
            Write_Ascii(xpos + 4 * C_ECHAR_HDOT,ypos,'/');
            Write_Number(xpos + 5 * C_ECHAR_HDOT,ypos ,MGetAngle(0),4,1,0);	//angle
            Write_Ascii(xpos + 9 * C_ECHAR_HDOT,ypos ,ANGLE);

            //����
            row = 4;
            ypos = para_ypos + crow_height *row ;
            ///Write_Number(xpos,ypos,MGetSpeed(),5,0,0);
            WriteSpeed(xpos,ypos );

            //̽ͷǰ��
            row = 5;
            if( row < row_number)
            {
                ypos = para_ypos + crow_height *row ;
                ///		Write_Number(xpos,ypos,MGetForward(),4,1,0);
                WriteLongness(xpos,ypos,MGetForward(),5,1);
            }
            //	Ĭ�ϱ�׼

            row = 6;
            if( row < row_number)
            {
                xpos = para_xpos;
                ypos = para_ypos + crow_height *row ;
                epText = (u_char*)_STDMenu_AB1 + ( MGetStdMode()*C_STD_LEN+2 );
                EMenuOut(xpos,ypos , epText ,C_STD_LEN-2,1,crow_height);	/*��ָ��λ�ø���ÿ���ַ������������и�д�˵�*/
            }
            //3��	Ƶ����ȣ�2.5\5.0\wide
            row = 7;
            if( row < row_number)
            {
                xpos = para_xpos;
                ypos = para_ypos + crow_height *row + C_ECHAR_HDOT * 0;
                epText = _ParaMenu_AD3[MGetLanguage()][MGetBandMode()]+2;
                TextOut(xpos,ypos, 1, 4,erow_height, epText, 4);
                //	epText = (u_char*)_ParaMenu_AD3 + ( MGetBandMode()*9+3 );
                //	EMenuOut(xpos,ypos+C_ECHAR_VDOT+1,epText,6,1,erow_height);	/*��ָ��λ�ø���ÿ���ַ������������и�д�˵�*/
            }
            //  2��  �첨��ʽ��ȫ��\���벨\���벨\��Ƶ
            row = 8;
            if( row < row_number)
            {
                xpos = para_xpos;
                ypos = para_ypos + crow_height *row + C_ECHAR_HDOT * 0;
                epText = (char *)_ParaMenu_AD2[MGetLanguage()][MGetEchoMode()]+2;
                TextOut(xpos,ypos+1,1,8,16,epText,4);
                //	cpText = (u_short*)_ParaMenu_AD2 + ( MGetEchoMode()*6+1 );
                //	CEMenuOut(xpos,ypos+1,cpText,4,1,crow_height);	/*��ָ��λ�ø���ÿ���ַ������������и�д�˵�*/
            }

            break;
        case 2:
            //4��	̽ͷ���᣺50Q\400Q
            row = 0;
            if( row < row_number)
            {
                xpos = para_xpos;
                ypos = para_ypos + crow_height *row;
                epText = (u_char*)_ParaMenu_AD4 + ( MGetDampMode()*10+3);
                EMenuOut(xpos,ypos+1,epText,7,1,erow_height);	/*��ָ��λ�ø���ÿ���ַ������������и�д�˵�*/
            }
            //5��	�����ѹ��200V\400V
            row = 1;
            if( row < row_number)
            {
                xpos = para_xpos;
                ypos = para_ypos + crow_height *row ;
                //	epText = (u_char*)_ParaMenu_AD5 + ( MGetPulseMode()*10+3 );
                //	EMenuOut(xpos,ypos+C_ECHAR_VDOT+1,epText,7,1,erow_height);	/*��ָ��λ�ø���ÿ���ַ������������и�д�˵�*/
                epText = (char *)_ParaMenu_AD5[MGetLanguage()][MGetPulseMode()]+2;
                TextOut(xpos,ypos,1,6,16,epText,4);
            }

            row = 2;//�ظ�Ƶ��
            if( row >= row_number)break;
            ypos = para_ypos + crow_height *row ;
            //	epText = (u_char*)_ParaMenu_CD1 + ( (MGetRepeatRate()/100) - 1) * 8 + 2;
            //	EMenuOut(xpos,ypos , epText ,6,1,crow_height);	 /*��ָ��λ�ø���ÿ���ַ������������и�д�˵�*/
            Write_Number(xpos,ypos,MGetRepeatRate(),4,0,0);

            row = 3;//����
            if( row >= row_number)break;
            ypos = para_ypos + crow_height *row ;
            epText = (u_char*)_ParaMenu_CC1 + MGetDenoise() * 6 + 2;
            EMenuOut(xpos,ypos , epText ,4,1,crow_height);	 /*��ָ��λ�ø���ÿ���ַ������������и�д�˵�*/

            row = 4;//������
            if( row >= row_number)break;
            ypos = para_ypos + crow_height *row ;
            if(extend==0)
            {
                Write_Number(xpos,ypos,MGetPulseWidthNumber(),4,0,0);
            }
            else
            {
                epText = (char *)_MainMenu_C1A3[MGetLanguage()][MGetPulseWidth()];
                TextOut(xpos,ypos,1,8,16,epText,4);
            }
            //	cpText = (u_short*)_ParaMenu_CE1 +  MGetPulseWidth() * 2 + 1;
            //	CEMenuOut(xpos,ypos , cpText ,1,1,crow_height);	 /*��ָ��λ�ø���ÿ���ַ������������и�д�˵�*/

            break;
        }////////////
        // keycode = MAnyKeyReturn();

        MKeyRlx();		/*ֻ�а������ͷŲ��˳�*/
        do
        {
            if( GetElapsedTime() > elapsedtime1 + 200 )
            {
                InvertWords(27*C_ECHAR_HDOT,380+4, 1); /*������ʾ�����ֵ�����*/
                elapsedtime1 = GetElapsedTime() ;
            }
            keycode = MGetKeyCode(0);
            if ( (keycode > 0 && keycode <= row_number)|| (keycode == 0 && 10 == row_number)
                    || ( ( keycode == C_KEYCOD_PLUS || keycode == C_KEYCOD_MINUS) && page_max > 1 )
                    || ( keycode == C_KEYCOD_LEFT || keycode == C_KEYCOD_RIGHT)
                    || ( keycode == C_KEYCOD_CONFIRM || keycode == C_KEYCOD_RETURN )
                    || ( keycode == C_KEYCOD_DEL && extend == 0 )
               )
                break;
            else
            {
                keycode = MGetRotary(-1);
                if( keycode > 0 )keycode = C_KEYCOD_PLUS;
                else if( keycode < 0)keycode = C_KEYCOD_MINUS;
                else continue;
                break;
            }
        }
        while( keycode != C_KEYCOD_RETURN && keycode != C_KEYCOD_CONFIRM );



        if (keycode == C_KEYCOD_RETURN)
        {
#if C_ACQU == 1
            MSetAcquisitionEnable(MGetAcquisitionEnable(),C_SETMODE_SETSAVE);
#endif
            SystemParaRestore();
            ChannelParaRestore();

            retvalue = C_FALSE;
            break;
        }
        else if (keycode == C_KEYCOD_CONFIRM)
        {
#if C_ACQU == 1
            MSetAcquisitionEnable(MGetAcquisitionEnable(),C_SETMODE_SETSAVE);
#endif
            MSetSystem();
            SystemParaStore();
            ChannelParaStore();

            //  CopyMemoryAllSystem(PM_COPY_WRITE);
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
        else if( keycode == C_KEYCOD_DEL)
        {
            if(extend == 0)
            {
                page_max = 2;
                row_number_page1 = 9;
                para_ypos_page1 =para_ypos_page2=  36;
                row_number_page2 = 5;
                continue;
            }
        }


        if(keycode == 0)keycode = 10;
        row = keycode - 1;
        xpos = para_xpos;
        ypos = para_ypos + crow_height * row-24;

        InvertWords(C_ECHAR_HDOT,ypos+24*(1-MGetLanguage()), 1); /*������ʾ�����ֵ�����*/

        if( page == 1)
        {
            switch (keycode)
            {
            case 1:		/*̽ͷ���� */
                if( MGetParaLock(1,C_LOCK_PROBE) == 1)break;

                //	cpText = (u_short*)_ParaMenu_AB2;
                MPushWindow(xpos, ypos, xpos + (5+2*MGetLanguage())*C_CCHAR_HDOT+1, ypos + crow_height * 4+1) ;
                EraseDrawRectangle(xpos, ypos, xpos + (5+2*MGetLanguage())*C_CCHAR_HDOT, ypos + crow_height * 4) ;
                //	CEMenuOut(xpos+4,ypos+4, cpText ,4,4,crow_height);	/*��ָ��λ�ø���ÿ���ַ������������и�д�˵�*/
                for (i=0; i<4; i++)
                {
                    epText = (char *)_ParaMenu_AB2[MGetLanguage()][i];
                    TextOut(xpos+4,ypos+4+i*crow_height,1,9,16,epText,4);
                }

                int offset;
                while(1)
                {
                    keycode = MAnyKeyReturn();
                    if (keycode == C_KEYCOD_RETURN||keycode == C_KEYCOD_CONFIRM)
                        break;
                    else if(keycode >= 1 && keycode <= 4)
                    {
                        MSetProbeMode(keycode-1,C_SETMODE_SAVE);	/*����̽ͷģʽ*/
                        if(MGetProbeMode() != 1)	//��б̽ͷ
                        {
                            if( MGetUnitType() > 0)MSetSpeed(CD_SPEED_1*1000/254,C_SETMODE_SAVE);//��λinch
                            else MSetSpeed(CD_SPEED_1,C_SETMODE_SAVE);

                            if( MGetUnitType() > 0)MSetCrystal_l(20000/25.4,C_SETMODE_SETSAVE);//��λinch
                            else MSetCrystal_l(20000,C_SETMODE_SETSAVE);
                            MSetCrystal_w(0,C_SETMODE_SETSAVE);
                            MSetAngle(0,C_SETMODE_SAVE);
                            MSetForward(0,C_SETMODE_SETSAVE);

                            if (MGetStdMode()==2)	//JB/T4730-2005
                            {
                                testrange[1] = 1000;
                                testrange[2] = 0;
                                offset = C_OFF_TEMP + 0;
                                MCopyProtectedMemory( (void*)offset, testrange, 12, PM_COPY_WRITE);
                                testrange[0] = 2000;
                                testrange[1] = 20;
                                offset = C_OFF_TEMP + 30;
                                MCopyProtectedMemory( (void*)offset, testrange, 12, PM_COPY_WRITE);
                            }
                            else if (MGetStdMode()==12)	//JB/T47013-2005
                            {
                                testrange[1] = 1000;
                                testrange[2] = 0;
                                offset = C_OFF_TEMP + 0;
                                MCopyProtectedMemory( (void*)offset, testrange, 12, PM_COPY_WRITE);
                                testrange[0] = 2000;
                                testrange[1] = 20;
                                offset = C_OFF_TEMP + 30;
                                MCopyProtectedMemory( (void*)offset, testrange, 12, PM_COPY_WRITE);
                            }
                            else if (MGetStdMode()==7)	//CB/T3559-94
                            {
                                testrange[1] = 1000;
                                testrange[2] = 2000;
                                offset = C_OFF_TEMP + 0;
                                MCopyProtectedMemory( (void*)offset, testrange, 12, PM_COPY_WRITE);
                                testrange[0] = 500;
                                testrange[1] = 30;
                                testrange[2] = 400;
                                offset = C_OFF_TEMP + 30;
                                MCopyProtectedMemory( (void*)offset, testrange, 12, PM_COPY_WRITE);
                            }

                        }
                        else 	//б̽ͷ
                        {
                            if( MGetUnitType() > 0)
                            {
                                MSetCrystal_l(13000/25.4,C_SETMODE_SETSAVE);
                                MSetCrystal_w(13000/25.4,C_SETMODE_SETSAVE);
                            }
                            else
                            {
                                MSetCrystal_l(13000,C_SETMODE_SETSAVE);
                                MSetCrystal_w(13000,C_SETMODE_SETSAVE);
                            }
                            if( MGetUnitType() > 0)MSetSpeed(CD_SPEED_2*1000/254,C_SETMODE_SAVE);//��λinch
                            else MSetSpeed(CD_SPEED_2,C_SETMODE_SAVE);
                            MSetScaleMode(2,0);
                            if(MGetRange(1)>C_MAX_RANGE/2- MGetDelay(3))
                            {
                                MSetRange(C_MAX_RANGE/2 - MGetDelay(3), C_SETMODE_SETSAVE) ;

                            }
                            MSetScaleMode(MGetScaleMode(),0);

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

                            else if (MGetStdMode()==12)	//JB/T47013-2005
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
                        }
                        MSetScaleMode(0,C_SETMODE_SAVE);

                        break;
                    }
                }
                MPopWindow();
                break;
            case 2:	//̽ͷƵ��
                if( MGetParaLock(1,C_LOCK_PROBE) == 1)break;
                deci_len = 2;
                number = 100;
                while(1)
                {
                    if( Input_Number(xpos,ypos,&number,2, &deci_len,0) != 1)break;
                    else
                    {
                        if( number > 0 && number <= 2000)
                        {
                            MSetFrequence(number,C_SETMODE_SAVE);
                            if( C_DEVLIB != 3)
                            {
                                if(number < 100 || number > 750) MSetBandMode(2,C_SETMODE_SAVE);
                                else if(number >= 100 && number <= 350) MSetBandMode(0,C_SETMODE_SAVE);
                                else if(number <= 750) MSetBandMode(1,C_SETMODE_SAVE);
                                else if(number <= 1250) MSetBandMode(2,C_SETMODE_SAVE);
                            }

                            break;
                        }
                        else	DisplayPrompt(4);
                    }
                }
                break;
            case 3:	//��Ƭ�ߴ�
                if( MGetParaLock(1,C_LOCK_PROBE) == 1)break;
                deci_len = 1;
                number = 100;
                int number1=100;
                while(1)
                {
                    if( Input_Crystal(xpos,ypos,&number,&number1) != 1)break;
                    else
                    {

                        if( number <= 50000 && number1 <= 50000)
                        {
                            MSetCrystal_l(number,C_SETMODE_SAVE);
                            MSetCrystal_w(number1,C_SETMODE_SAVE);
                            break;
                        }
                        else	DisplayPrompt(4);
                    }
                }
                break;
            case 4:	//�����
                if( MGetParaLock(1,C_LOCK_OFFSET) == 1)break;
                number = 0;
                deci_len = 2;
                if ( Input_Number(xpos,ypos,&number,2, &deci_len,0) == 1)
                {
                    if(number <= 500)
                    {
                        /*б̽ͷ����Kֵ*/
                        MSetKvalue(number,C_SETMODE_SAVE);
                    }
                    else if(number <= C_MAX_ANGLE*10)
                    {
                        number /=10;
                        MSetAngle(number,C_SETMODE_SAVE);	/*���õ�ǰͨ��	*/
                    }
                }
                break;

            case 5:	//����
                if( MGetParaLock(1,C_LOCK_OFFSET) == 1)break;
                if( MGetUnitType() > 0)deci_len = 2;//��λinch
                else deci_len = 0;
                number = 100;
                int MaxSpeed = C_MAX_SPEED,MinSpeed = C_MIN_SPEED;
                if (MGetUnitType())
                {
                    MaxSpeed = C_MAX_SPEED*100/25.4+0.5;
                    MinSpeed = C_MIN_SPEED*100/25.4+0.5;
                }
                while(1)
                {
                    if( Input_Number(xpos,ypos,&number,5, &deci_len,0) != 1)break;
                    else
                    {
                        if(number >= MinSpeed && number <= MaxSpeed)
                        {
                            MSetSpeed(number,C_SETMODE_SAVE);
                            break;
                        }
                        else	DisplayPrompt(4);
                    }
                }
                break;
            case 6:	//ǰ��
                if( MGetParaLock(1,C_LOCK_PROBE) == 1)break;
                if( MGetUnitType() > 0)deci_len = 3;//��λinch
                else deci_len = 1;
                number = 100;
                while(1)
                {
                    if( Input_Number(xpos,ypos,&number,2, &deci_len,0) != 1)break;
                    else
                    {
                        if(( number <= 3000 && MGetUnitType()==0)||( number <= 11000 && MGetUnitType()==1))
                        {
                            MSetForward(number,C_SETMODE_SAVE);
                            break;
                        }
                        else	DisplayPrompt(4);
                    }
                }
                break;
            case 7:		//Ĭ�ϱ�׼
                if( MGetTestStatus(C_TEST_AVG) == 1)break;//����AVG����ʾ��׼�˵�
                if( MGetParaLock(1,C_LOCK_DACAVG) == 1)break;	//��������
                ypos -= 4 * crow_height;
                MPushWindow(xpos, ypos, xpos+ (C_STD_LEN+2)*C_ECHAR_HDOT+1, ypos + (C_STD_MAX+1) * erow_height+1) ;
                //EraseDrawRectangle(xpos, ypos, xpos+ (C_STD_LEN+2)*C_ECHAR_HDOT, ypos + (C_STD_MAX+1) * erow_height) ;
                //EMenuOut(xpos + C_ECHAR_HDOT,ypos+4,_STDMenu_AB1,C_STD_LEN,C_STD_MAX,erow_height);	/*��ָ��λ�ø���ÿ���ַ������������и�д�˵�*/
                /*
                while(1)
                {
                    //keycode = MenuKeyReturn(row,2);	//���ֲ˵�ʱ����������keymax�ļ����أ�mode=0��������Ч��=1ȷ�ϼ���Ч��=2���ؼ���Ч��=3ȷ�Ϸ��ؼ���Ч
                    keycode = MAnyKeyReturn();	//���ֲ˵�ʱ����������keymax�ļ����أ�mode=0��������Ч��=1ȷ�ϼ���Ч��=2���ؼ���Ч��=3ȷ�Ϸ��ؼ���Ч
                    stdflag=1;
                    if (keycode == C_KEYCOD_RETURN||keycode == C_KEYCOD_CONFIRM)
                        break;
                    else if(keycode >= 0 && keycode <= C_STD_MAX|| keycode == C_KEYCOD_PLUS || keycode == C_KEYCOD_MINUS)
                    {
                        if (keycode==0)
                        {
                            keycode=10;
                        }
                		if (keycode == C_KEYCOD_PLUS)//change
                		{
                			keycode=11;
                		}
                		if (keycode == C_KEYCOD_MINUS)//change
                		{
                			keycode=12;
                		}
                        MSetStdMode(keycode-1,C_SETMODE_SAVE);
                        MSetBlock(0);
                		if(keycode == 11 || keycode == 12)
                		MSetDacMode(0,0);
                		else
                        MSetDacMode(2,0);

                        MSetCurveNumber( c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_LINENUM] );

                        k = c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_THNUM];//�Կ�����
                        number = MGetThick()/10;
                        if( MGetUnitType() > 0)number = (UINT)(number * 254/1000.0+0.5);//��λinch
                        for( i = 0 ; i < k; i++)
                        {
                			if(MGetStdMode()==0)break;
                            if( number >=  c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_THICK+7*i] )continue;
                            for( j = 0; j < MGetCurveNumber(); j++)
                            {
                                MSetLineGain(j, c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_THICK+7*i+j+1] );
                            }
                            break;
                        }

                        break;
                    }
                }*/

                stdflag=1;
                while(1)
                {
                    EraseDrawRectangle(xpos, ypos, xpos+ (C_STD_LEN+2)*C_ECHAR_HDOT, ypos + 12 * erow_height) ;

                    if(pagemax==0)
                        EMenuOut(xpos + C_ECHAR_HDOT,ypos+4,_STDMenu_AB1+9*C_STD_LEN*pagemax,C_STD_LEN,9,erow_height);	/*��ָ��λ�ø���ÿ���ַ������������и�д�˵�*/
                    else
                        EMenuOut(xpos + C_ECHAR_HDOT,ypos+4,_STDMenu_AB1+9*C_STD_LEN*pagemax,C_STD_LEN,4,erow_height);	/*��ָ��λ�ø���ÿ���ַ������������и�д�˵�*/
                    Write_Number(xpos + 11* C_ECHAR_HDOT, ypos+10*erow_height+6,pagemax+1,1,0,0);
                    TextOut(xpos+12*C_ECHAR_HDOT,ypos+10*erow_height+4,1,3,24,"/2",4);

                    while(1)
                    {
                        keycode=MAnyKeyReturn();
                        if( (keycode>=1&&keycode<=9)||(keycode==C_KEYCOD_PLUS)||(keycode==C_KEYCOD_MINUS)||(keycode==C_KEYCOD_CONFIRM)||(keycode==C_KEYCOD_RETURN) )
                            break;

                    }

                    if(keycode>=1&&keycode<=9&&(pagemax==0||pagemax==1))
                    {
                        if(pagemax==0)
                        {
                            MSetStdMode(keycode-1,C_SETMODE_SAVE);
                            MSetBlock(0);
                            MSetDacMode(2,0);

                            MSetCurveNumber( c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_LINENUM] );

                            k = c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_THNUM];//�Կ�����
                            number = MGetThick()/10;
                            if( MGetUnitType() > 0)number = (UINT)(number * 254/1000.0+0.5);//��λinch
                            for( i = 0 ; i < k; i++)
                            {
                                if(MGetStdMode()==0)break;
                                if( number >c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_THICK+7*i] )continue;
                                for( j = 0; j < MGetCurveNumber(); j++)
                                {
                                    MSetLineGain(j, c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_THICK+7*i+j+1] );
                                }
                                break;
                            }

                        }
                        else
                        {
                            MSetStdMode(keycode-1+9,C_SETMODE_SAVE);
                            MSetBlock(0);
                            if(keycode==2||keycode==3)
                                MSetDacMode(0,0);
                            else
                                MSetDacMode(2,0);

                            MSetCurveNumber( c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_LINENUM] );

                            k = c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_THNUM];//�Կ�����
                            number = MGetThick()/10;
                            if( MGetUnitType() > 0)number = (UINT)(number * 254/1000.0+0.5);//��λinch
                            for( i = 0 ; i < k; i++)
                            {

                                if(MGetStdMode()==10||MGetStdMode()==11)
                                {
                                    if( number >=c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_THICK+7*i] )continue;

                                }
                                else
                                {
                                    if( number >c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_THICK+7*i] )continue;

                                }
                                for( j = 0; j < MGetCurveNumber(); j++)
                                {
                                    MSetLineGain(j, c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_THICK+7*i+j+1] );
                                }
                                break;
                            }


                        }
                        if(MGetStdMode()==12)//47013
                        {
                            int testrange[3];
                            int offset;
                            testrange[0] = 800;
                            testrange[1] = 20;
                            testrange[2] = 400;
                            offset = C_OFF_TEMP + 30;
                            MCopyProtectedMemory( (void*)offset, testrange, 12, PM_COPY_WRITE);
                        }
                        else if(MGetStdMode()==2)//4730
                        {
                            int testrange[3];
                            int offset;
                            testrange[0] = 500;
                            testrange[1] = 10;
                            testrange[2] = 60;
                            offset = C_OFF_TEMP + 30;
                            MCopyProtectedMemory( (void*)offset, testrange, 12, PM_COPY_WRITE);
                        }
                        break;

                    }
                    else if(keycode==C_KEYCOD_PLUS)
                    {
                        pagemax++;
                        if(pagemax>1) pagemax=0;
                        continue;
                    }
                    else if(keycode==C_KEYCOD_MINUS)
                    {
                        pagemax--;
                        if(pagemax<0)  pagemax=1;
                        continue;
                    }
                    else if(keycode==C_KEYCOD_CONFIRM||keycode==C_KEYCOD_RETURN)
                        break;
                }
                MPopWindow();
                break;
            case 8:
                if( MGetParaLock(1,C_LOCK_PROBE) == 1)break;
                ypos-=crow_height*2;
                MPushWindow(xpos, ypos, xpos+ 11*C_ECHAR_HDOT+1, ypos + 4 * crow_height+5+1) ;
                EraseDrawRectangle(xpos, ypos, xpos+ 11*C_ECHAR_HDOT, ypos + 4 * crow_height+5) ;
                //	EMenuOut(xpos + C_ECHAR_HDOT,ypos+4,_ParaMenu_AD3,9,3,15);	/*��ָ��λ�ø���ÿ���ַ������������и�д�˵�*/

                TextOut(xpos + 4,ypos+4, 1, 10,crow_height, (u_char*)_ParaMenu_AD3[MGetLanguage()][0], 4);
                TextOut(xpos + 4,ypos+4+crow_height, 1, 10,crow_height, (u_char*)_ParaMenu_AD3[MGetLanguage()][1], 4);
                TextOut(xpos + 4,ypos+4+crow_height*2, 1, 10,crow_height, (u_char*)_ParaMenu_AD3[MGetLanguage()][2], 4);
                TextOut(xpos + 4,ypos+4+crow_height*3, 1, 10,crow_height, (u_char*)_ParaMenu_AD3[MGetLanguage()][3], 4);

                while(1)
                {
                    keycode = MenuKeyReturn(4,2);	/*���ֲ˵�ʱ����������keymax�ļ����أ�mode=0��������Ч��=1ȷ�ϼ���Ч��=2���ؼ���Ч��=3ȷ�Ϸ��ؼ���Ч*/
                    if (keycode == C_KEYCOD_RETURN||keycode == C_KEYCOD_CONFIRM)
                        break;
                    else if(keycode >= 1 && keycode <= 4)
                    {
                        MSetBandMode(keycode-1,C_SETMODE_SAVE);
                        break;
                    }
                }
                MPopWindow();
                break;
            case 9:
                if( MGetParaLock(1,C_LOCK_PROBE) == 1)break;
                //cpText = (u_short*)_ParaMenu_AD2 ;
                ypos -= 3 * crow_height;
#if C_DEVLIB != 3
                deci_len = 4;
#else
                deci_len =2;
#endif
                MPushWindow(xpos, ypos, xpos+ 6*C_CCHAR_HDOT+1, ypos + deci_len * crow_height+1) ;
                EraseDrawRectangle(xpos, ypos, xpos+ 6*C_CCHAR_HDOT, ypos + deci_len * crow_height) ;
                for (i=0; i<deci_len; i++)
                {
                    TextOut(xpos+C_ECHAR_HDOT,ypos+ i * crow_height+4,1,8,16,_ParaMenu_AD2[MGetLanguage()][i],4);
                }
                //	CEMenuOut(xpos + C_ECHAR_HDOT,ypos+4,cpText,5,deci_len,crow_height);	/*��ָ��λ�ø���ÿ���ַ������������и�д�˵�*/
                while(1)
                {
                    keycode = MenuKeyReturn(4,2);	/*���ֲ˵�ʱ����������keymax�ļ����أ�mode=0��������Ч��=1ȷ�ϼ���Ч��=2���ؼ���Ч��=3ȷ�Ϸ��ؼ���Ч*/
                    if (keycode == C_KEYCOD_RETURN||keycode == C_KEYCOD_CONFIRM)
                        break;
                    else if(keycode >= 1 && keycode <= deci_len)
                    {
                        MSetEchoMode(keycode-1,C_SETMODE_SETSAVE);
                        break;
                    }
                }
                MPopWindow();
                break;
            }
        }
        else if( page == 2)
        {
            switch(keycode)
            {

            case 1:	//̽ͷ����
                if( MGetParaLock(1,C_LOCK_PROBE) == 1)break;
                MPushWindow(xpos, ypos, xpos+ 12*C_ECHAR_HDOT+1, ypos + 3 * erow_height+1) ;
                EraseDrawRectangle(xpos, ypos, xpos+ 12*C_ECHAR_HDOT, ypos + 3 * erow_height) ;

                EMenuOut(xpos + C_ECHAR_HDOT,ypos+6,_ParaMenu_AD4,10,2,20);	/*��ָ��λ�ø���ÿ���ַ������������и�д�˵�*/
                while(1)
                {
                    keycode = MenuKeyReturn(2,2);	/*��������keymax�ļ����أ�mode=0��������Ч��=1ȷ�ϼ���Ч��=2���ؼ���Ч��=3ȷ�Ϸ��ؼ���Ч*/
                    if (keycode == C_KEYCOD_RETURN||keycode == C_KEYCOD_CONFIRM)
                        break;
                    else if(keycode >= 1 && keycode <= 2)
                    {
                        MSetDampMode(keycode-1,C_SETMODE_SAVE);
                        break;
                    }
                }
                MPopWindow();
                break;
            case 2:	//�����ѹ
                if( MGetParaLock(1,C_LOCK_PROBE) == 1)break;

#if C_DEVLIB == 23 || C_DEVLIB == 24
                //	ypos -= 5 * erow_height;
                MPushWindow(xpos, ypos, xpos+ 12*C_ECHAR_HDOT+1, ypos + 4 * crow_height+1) ;
                EraseDrawRectangle(xpos, ypos , xpos+ 12*C_ECHAR_HDOT, ypos + 4 * crow_height) ;
                //	EMenuOut(xpos + C_ECHAR_HDOT,ypos + 6 ,_ParaMenu_AD5,10,4,17);
                TextOut(xpos + C_ECHAR_HDOT,ypos + 6 ,4,6,crow_height,(char *)_ParaMenu_AD5[MGetLanguage()][0],4);
                TextOut(xpos + C_ECHAR_HDOT,ypos + 6+crow_height ,4,6,crow_height,(char *)_ParaMenu_AD5[MGetLanguage()][1],4);
                TextOut(xpos + C_ECHAR_HDOT,ypos + 6+crow_height*2 ,4,6,crow_height,(char *)_ParaMenu_AD5[MGetLanguage()][2],4);
                TextOut(xpos + C_ECHAR_HDOT,ypos + 6+crow_height*3 ,4,6,crow_height,(char *)_ParaMenu_AD5[MGetLanguage()][3],4);
                while(1)
                {
                    keycode = MenuKeyReturn(4,2);
                    if (keycode == C_KEYCOD_RETURN||keycode == C_KEYCOD_CONFIRM)
                        break;
                    else if(keycode >= 1 && keycode <= 4)
                    {
                        MSetPulseMode(keycode-1,C_SETMODE_SAVE);
                        //SetTransmitterVoltage(LOW_PULSE+keycode-1) ;
                        break;
                    }
                }
                MPopWindow();
#else

                //	ypos -= 3 * erow_height;
                MPushWindow(xpos, ypos, xpos+ 12*C_ECHAR_HDOT, ypos + 3 * erow_height) ;
                EraseDrawRectangle(xpos, ypos , xpos+ 12*C_ECHAR_HDOT, ypos + 3 * erow_height) ;
                EMenuOut(xpos + C_ECHAR_HDOT,ypos + 6 ,_ParaMenu_AD5,10,2,17);	/*��ָ��λ�ø���ÿ���ַ������������и�д�˵�*/
                while(1)
                {
                    keycode = MenuKeyReturn(2,2);	/*���ֲ˵�ʱ����������keymax�ļ����أ�mode=0��������Ч��=1ȷ�ϼ���Ч��=2���ؼ���Ч��=3ȷ�Ϸ��ؼ���Ч*/
                    if (keycode == C_KEYCOD_RETURN||keycode == C_KEYCOD_CONFIRM)
                        break;
                    else if(keycode >= 1 && keycode <= 2)
                    {
                        MSetPulseMode(keycode-1,C_SETMODE_SETSAVE);
                        break;
                    }
                }
                MPopWindow();
#endif

                break;

            case 3://�ظ�Ƶ��
                if( MGetParaLock(1,C_LOCK_PROBE) == 1)break;
                deci_len = 0;
                number = 100;
                while(1)
                {
                    if( Input_Number(xpos,ypos,&number,4, &deci_len,0) != 1)break;
                    else
                    {
                        if(number >= 10 && number <= 1000)
                        {
                            MSetRepeatRate(number,C_SETMODE_SAVE);
                            break;
                        }
                    }
                }
                break;
            case 4://����
                if( MGetParaLock(1,C_LOCK_PROBE) == 1)break;
                epText = (u_char*)_ParaMenu_CC1 ;
                MPushWindow(xpos, ypos, xpos + 5*C_CCHAR_HDOT, ypos + erow_height * 6) ;
                EraseDrawRectangle(xpos, ypos, xpos + 5*C_CCHAR_HDOT, ypos + erow_height * 6) ;
                EMenuOut(xpos+4,ypos+8 , epText ,6,5,erow_height);	/*��ָ��λ�ø���ÿ���ַ������������и�д�˵�*/
                while(1)
                {
                    keycode = MAnyKeyReturn();
                    if (keycode == C_KEYCOD_RETURN||keycode == C_KEYCOD_CONFIRM)
                        break;
                    else if(keycode >= 0 && keycode <= 4)
                    {
                        g_nDenoise = keycode;
                        if( g_nRepeat < 100 * g_nDenoise + 100)g_nRepeat = 100 * g_nDenoise + 100;
                        MSetDenoise(g_nDenoise, C_SETMODE_SETSAVE);
                        break;
                    }
                }
                MPopWindow();

                break;
            case 5://������
                if( MGetParaLock(1,C_LOCK_PROBE) == 1)break;
                if(extend==0)
                {
                    deci_len = 0;
                    number = 0;
                    while(1)
                    {
                        if( Input_Number(xpos,ypos,&number,4, &deci_len,0) != 1)break;
                        else
                        {
                            if(number >= 1 && number <= 100)
                            {
                                MSetPulseWidthInput(number, C_SETMODE_SETSAVE);
                                break;
                            }
                        }
                    }
                }
                else
                {
                    MSetPulseWidth(MGetPulseWidth()+1,C_SETMODE_SETSAVE);
                }
                break;
            }
        }
    }
    if( retvalue == C_TRUE && MGetTestStatus(C_TEST_DAC) == 1 && stdflag==1)
    {
        MSetDelay(0,C_SETMODE_SETSAVE);
        DrawDac(0);		//�»�DAC
        if( MGetThick()*2 <= MGetRange(4) && MGetStdMode()!=11 )//if( MGetThick()*2 <= MGetRange(4) && MGetStdMode()<C_STD_MAX-1 )
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
            else if (MGetStdMode()==7)	//CBT3559-94
            {
                MSetBaseGain( MGetBaseGain()-200*log10( dac_line[2*C_COORWIDTH/MGetRange(4)]/160.0),C_SETMODE_SETSAVE);
            }
            else if (MGetStdMode()==12)	//47013
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
        MSetSystem();
        SystemParaStore();
        ChannelParaStore();
        ScreenRenovate();
    }
    MSetColor(C_CR_UNDO);
    MSetSystem();
    ClearEnvelope();	//�������ֵ
    MSetRepeatRate(MGetRepeatRate(), C_SETMODE_SET);
    if(extend==0)
    {
        MSetPulseWidthInput(MGetPulseWidthNumber(), C_SETMODE_SETSAVE);
    }
    else
    {
        MSetPulseWidth(MGetPulseWidth(), C_SETMODE_SETSAVE);
    }
    g_nAutoDenoise = 0;
    MSetBaseGain(MGetBaseGain(), C_SETMODE_SETSAVE);
    MSetAcquisitionEnable(1,C_SETMODE_SETSAVE);
    MSetHardEchoShow(1,C_SETMODE_SETSAVE);
    ScreenRenovate();	/*��Ļˢ��*/
    DrawDac(0);		//�»�DAC
    return 1;
}



int OptionMenu(int page)//���ֵڼ�ҳ�Ĳ����˵�
{
    u_int char_len = 6,row_number = 9,crow_height = 48,erow_height =30;
    int number,deci_len;//,sign;
    int xpos,ypos;
    //int menu_xpos = 0,menu_ypos = 0;
    int para_xpos = C_CCHAR_HDOT * ( char_len + 1) ,para_ypos = crow_height + 4;
    int retvalue = C_TRUE;
    int keycode;
    int page_max = 2;
    int row;
    u_short* cpText = 0;
    u_char* epText = 0;
    int i;
    u_int elapsedtime1 = GetElapsedTime() ;//,elapsedtime2;

    if( page < 1)page = 1;
    else if(page > page_max )page = page_max;

    MSetAcquisitionEnable(0,C_SETMODE_SETSAVE);
    MSetHardEchoShow(0,C_SETMODE_SETSAVE);
    ClearEnvelope();
    SystemParaStore();
    ChannelParaStore();
    DisplayPrompt(15);

    while(1)
    {
        MFclearScreen();

        switch(page)
        {
        case 1:
            cpText =  (u_short*)_OptionMenu_B1;
            epText =  (u_char*)_OptionMenu_C1;
            break;
        case 2:
            cpText =  (u_short*)_ParaMenu_A3;
            epText =  (u_char*)_ParaMenu_B3;
            break;
        }

        MSetColor(C_CR_MENU);
        TextOut(C_ECHAR_HDOT,4,1,25,16,(u_char*)_ParaMenu_H1[MGetLanguage()][0],4);
        Write_Number(C_ECHAR_HDOT + 12 * C_ECHAR_HDOT+4,4+3*MGetLanguage(),page,1,0,0);
        if (MGetLanguage())
        {
            Write_Number(C_ECHAR_HDOT + 24 * C_ECHAR_HDOT+8,7,page_max,1,0,0);
        }
        else
        {
            Write_Number(C_ECHAR_HDOT + 19 * C_ECHAR_HDOT+4,4,page_max,1,0,0);
        }
        CEMenuOut(C_ECHAR_HDOT,para_ypos, cpText ,char_len,row_number,crow_height);	/*��ָ��λ�ø���ÿ���ַ������������и�д�˵�*/
        EMenuOut(para_xpos+C_ECHAR_HDOT*5,para_ypos + 8, epText ,3,row_number,crow_height);	/*��ָ��λ�ø���ÿ���ַ������������и�д�˵�*/

        EraseDrawRectangle(26*C_ECHAR_HDOT, 380, 26*C_ECHAR_HDOT + (5+1)*C_CCHAR_HDOT, 380 + 2 * (C_CCHAR_VDOT+12)) ;
//		CMenuOut(27*C_ECHAR_HDOT,190+4,_MenuPrompt_B1,5,2,24);	/*��ָ��λ�ø���ÿ���ַ������������и�д�˵�*/

        TextOut(27*C_ECHAR_HDOT,380+4,1,10,24,(u_char*)_MenuPrompt_B1[MGetLanguage()][0],4);
        TextOut(27*C_ECHAR_HDOT,380+4+36,1,10,24,(u_char*)_MenuPrompt_B1[MGetLanguage()][0]+10,4);
//        EMenuOut(27*C_ECHAR_HDOT,380+4,_TestMenu_Enter,1,1,C_ECHAR_VDOT);
        MSetColor(C_CR_PARA);
        switch(page)
        {
        case 1:
            //���̱��
            row = 0;
            xpos = para_xpos;
            ypos = para_ypos + crow_height *row;
            //	cpText = (u_short*)_ParaMenu_AB1 + (MGetScaleMode()-0) * 3 + 1;
            //	CEMenuOut(xpos,ypos , cpText ,2,1,crow_height);	/*��ָ��λ�ø���ÿ���ַ������������и�д�˵�*/
            TextOut(xpos,ypos ,1,10,crow_height,(u_char*)_ParaMenu_AB1[MGetLanguage()][MGetScaleMode()]+2,4);

            //���油��
            row = 1;
            ypos = para_ypos + crow_height *row + C_ECHAR_HDOT;
            Write_Number(xpos,ypos,MGetSurfGain(),4,1,0);

            //5��	��Ļ���ȣ�����\һ��\����\����
#if C_DEVTYPE == 4 || C_DEVTYPE == 20 || C_DEVTYPE == 5
            row = 2;//�������
            ypos = para_ypos + crow_height *row + C_ECHAR_HDOT * 1;
            Write_Number(xpos,ypos,MGetThick(),4,1,0);
#elif C_COLOR != 1
            row = 2;//��Ļ����
            xpos = para_xpos;
            ypos = para_ypos + crow_height *row + C_ECHAR_HDOT * 0;
            TextOut(xpos,ypos ,1,8,crow_height,(u_char*)_ParaMenu_AC4[MGetLanguage()][MGetScreenBrightness()]+2,4);
#endif
            //6����ӡ��
            row = 3;
            ypos = para_ypos + crow_height *row + C_ECHAR_HDOT;
            epText = (u_char*)_ParaMenu_AB3 + ( (MGetPrinterMode()-0)*15+2 );
            EMenuOut(xpos,ypos , epText ,13,1,erow_height);	/*��ָ��λ�ø���ÿ���ַ������������и�д�˵�*/

            //3��	������׼��ĸ��\�з�\����\����
            row = 4;
            xpos = para_xpos;
            ypos = para_ypos + crow_height *row + C_ECHAR_HDOT * 0;
            TextOut(xpos,ypos ,1,8,crow_height,(u_char*)_ParaMenu_AC2[MGetLanguage()][MGetDacMode()]+2,4);
            //4��	������ʾ��dBֵ\�׾�
            //2��	�첨��ʽ��ȫ��\���벨\���벨\��Ƶ
            row = 5;
            xpos = para_xpos;
            ypos = para_ypos + crow_height *row + C_ECHAR_HDOT * 0;
            epText = (char *)_ParaMenu_AD2[MGetLanguage()][MGetEchoMode()]+2;
            TextOut(xpos,ypos+1,1,8,16,epText,4);
            //	cpText = (u_short*)_ParaMenu_AD2 + ( MGetEchoMode()*6+1 );
            //	CEMenuOut(xpos,ypos+1,cpText,4,1,crow_height);	/*��ָ��λ�ø���ÿ���ַ������������и�д�˵�*/

            //3��	Ƶ����ȣ�2.5\5.0\10\wide
            row = 6;
            xpos = para_xpos;
            ypos = para_ypos + crow_height *row + C_ECHAR_HDOT * 0;
            //	epText = (u_char*)_ParaMenu_AD3 + ( MGetBandMode()*9+3 );
            //	EMenuOut(xpos,ypos+C_ECHAR_VDOT+1,epText,4,1,erow_height);	/*��ָ��λ�ø���ÿ���ַ������������и�д�˵�*/

            epText = _ParaMenu_AD3[MGetLanguage()][MGetBandMode()]+2;
            TextOut(xpos,ypos, 1, 4,erow_height, epText, 4);
            //4��	̽ͷ���᣺50Q\400Q
            row = 7;
            xpos = para_xpos;
            ypos = para_ypos + crow_height *row + C_ECHAR_HDOT * 0;
            epText = (u_char*)_ParaMenu_AD4 + ( MGetDampMode()*10+3 );
            EMenuOut(xpos,ypos+C_ECHAR_VDOT+1,epText,7,1,erow_height);	/*��ָ��λ�ø���ÿ���ַ������������и�д�˵�*/

            //5��	�����ѹ��200V\400V
            row = 8;
            xpos = para_xpos;
            ypos = para_ypos + crow_height *row + C_ECHAR_HDOT * 0;
            //	epText = (u_char*)_ParaMenu_AD5 + ( MGetPulseMode()*10+3 );
            //	EMenuOut(xpos,ypos+C_ECHAR_VDOT+1,epText,7,1,erow_height);	/*��ָ��λ�ø���ÿ���ַ������������и�д�˵�*/
            epText = (char *)_ParaMenu_AD5[MGetLanguage()][MGetPulseMode()]+2;
            TextOut(xpos,ypos,1,6,16,epText,4);
            break;

        case 2:
            //1��	��¼��ʽ������\����
            row = 0;
            xpos = para_xpos;
            ypos = para_ypos + crow_height *row + C_ECHAR_HDOT * 0;
            //	cpText = (u_short*)_ParaMenu_AC1 + MGetSaveMode() * 3 + 1;
            //	CEMenuOut(xpos,ypos , cpText ,2,1,crow_height);	/*��ָ��λ�ø���ÿ���ַ������������и�д�˵�*/
            TextOut(xpos,ypos ,1,6,crow_height,(u_char*)_ParaMenu_AC1[MGetLanguage()][MGetSaveMode()]+2,4);

            //2��	��¼�����10sec-120sec	;��Ϊ������˲���Ϊ0��ѡ����ʱ�Զ���Ϊ30sec
            row = 1;
            xpos = para_xpos;
            ypos = para_ypos + crow_height *row + C_ECHAR_HDOT * 1;
            Write_Number(xpos,ypos,MGetSaveTime(),4,1,0);


            row = 2;//	B����;��ʧ��\����
            xpos = para_xpos;
            ypos = para_ypos + crow_height *row + C_ECHAR_HDOT * 0;
            TextOut(xpos,ypos,1,6+2*MGetLanguage(),crow_height,(u_char*)_ParaMenu_AC5[MGetLanguage()][MGetBGateMode()]+2,4);

            row = 3;//������������
            xpos = para_xpos;
            ypos = para_ypos + crow_height *row + C_ECHAR_HDOT * 0;
            //	cpText = (u_short*)_ParaMenu_AD8 + MGetSoundAlarm(0) * 2 + 1;
            //CEMenuOut(xpos,ypos , cpText ,1,1,crow_height);	 /*��ָ��λ�ø���ÿ���ַ������������и�д�˵�*/
            TextOut(xpos,ypos,1,4,crow_height,(u_char*)_ParaMenu_AD8[MGetLanguage()][MGetSaveFrequency()]+2,4);

            //6��	����������ȫ��\����
            row = 4;
            xpos = para_xpos;
            ypos = para_ypos + crow_height *row + C_ECHAR_HDOT * 0;
            u_short paratemp=0;
            int j;
            for(j = 0; j < C_LOCK_ALL; j++)
            {
                if(MGetParaLock(0,j) == 1)paratemp++;
            }
            epText = (u_char *)_ParaMenu_AD6[MGetLanguage()][paratemp];
            TextOut(xpos,ypos,1,4,crow_height,epText,4);

            //7��
            row = 5;
            xpos = para_xpos;
            ypos = para_ypos + crow_height *row + C_ECHAR_HDOT * 1;
            Write_Number(xpos,ypos,MGetAmpStdMax()*5,4,1,0);

            row = 6;//�洢Ƶ��
            ypos = para_ypos + crow_height *row + C_ECHAR_HDOT * 0;
            //	cpText = (u_short*)_ParaMenu_AD9 + MGetSaveFrequency() * 2 + 1;
            //	CEMenuOut(xpos,ypos , cpText ,1,1,crow_height);	 /*��ָ��λ�ø���ÿ���ַ������������и�д�˵�*/
            TextOut(xpos,ypos,1,4,crow_height,(u_char*)_ParaMenu_AD9[MGetLanguage()][MGetSaveFrequency()]+2,4);

            row = 7;//�������
            ypos = para_ypos + crow_height *row + C_ECHAR_HDOT * 1;
            Write_Number(xpos,ypos,MGetThick(),4,1,0);

            row = 8;//�����⾶
            ypos = para_ypos + crow_height *row + C_ECHAR_HDOT * 1;
            Write_Number(xpos,ypos,MGetDiameter(),4,1,0);


            //8��	������ʱ��  min		��������ʱΪ0����Ϊ��ʱ�Զ���Ϊ5min
            /*				row = 7;
            				xpos = para_xpos;
            				ypos = para_ypos + crow_height *row + C_ECHAR_HDOT * 1;
            */
            break;
        }

        MKeyRlx();		/*ֻ�а������ͷŲ��˳�*/
        do
        {
            if( GetElapsedTime() > elapsedtime1 + 200 )
            {
                InvertWords(27*C_ECHAR_HDOT,380+4, 1); /*������ʾ�����ֵ�����*/
                elapsedtime1 = GetElapsedTime() ;
            }
            keycode = MGetKeyCode(10);
            if (keycode >= 0 && keycode < C_KEYCODMAX)break;
            else
            {
                keycode = MGetRotary(-1);
                if( keycode > 0 )keycode = C_KEYCOD_PLUS;
                else if( keycode < 0)keycode = C_KEYCOD_MINUS;
                else continue;
                break;
            }
        }
        while(1);

        //keycode = MAnyKeyReturn();

        if (keycode == C_KEYCOD_RETURN)
        {

            retvalue = C_FALSE;
            break;
        }
        if (keycode == C_KEYCOD_CONFIRM)
        {
            SystemParaStore();
            ChannelParaStore();

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

        InvertWords(C_ECHAR_HDOT,ypos, 1); /*������ʾ�����ֵ�����*/

        if( page == 1)
        {
            switch (keycode)
            {
            case 1:		//���
                //	cpText = (u_short*)_ParaMenu_AB1 ;
                MPushWindow(xpos, ypos, xpos + (4)*C_CCHAR_HDOT, ypos + crow_height * 3) ;
                EraseDrawRectangle(xpos, ypos, xpos + (4)*C_CCHAR_HDOT, ypos + crow_height * 3) ;
                //	CEMenuOut(xpos+4,ypos+4 , cpText ,3,3,crow_height);	/*��ָ��λ�ø���ÿ���ַ������������и�д�˵�*/
                TextOut(xpos+4,ypos+4 ,3,10,crow_height,(u_char*)_ParaMenu_AB1[MGetLanguage()][0],4);
                while(1)
                {
                    keycode = MAnyKeyReturn();
                    if (keycode == C_KEYCOD_RETURN||keycode == C_KEYCOD_CONFIRM)
                        break;
                    else if(keycode >= 1 && keycode <= 3)
                    {
                        MSetScaleMode(keycode-1,C_SETMODE_SAVE);
                        break;
                    }
                }
                MPopWindow();
                break;
            case 2:	//���油��
                deci_len = 1;
                number = 100;
                while(1)
                {
                    if( Input_Number(xpos,ypos,&number,2, &deci_len,0) != 1)break;
                    else
                    {
                        if( number <= C_MAX_SURFGAIN)
                        {
                            MSetSurfGain(number,C_SETMODE_SAVE);
                            break;
                        }
                        else	DisplayPrompt(4);
                    }
                }

                break;
            case 3:	//��Ļ����
#if C_DEVTYPE != 4
                MPushWindow(xpos, ypos, xpos + 6*C_CCHAR_HDOT, ypos + crow_height * 4) ;
                EraseDrawRectangle(xpos, ypos, xpos + (6)*C_CCHAR_HDOT, ypos + crow_height * 4) ;
                //	CEMenuOut(xpos+4,ypos+4 , cpText ,3,4,crow_height);	//��ָ��λ�ø���ÿ���ַ������������и�д�˵�
                TextOut(xpos+4,ypos+4 ,4,10,crow_height,(u_char*)_ParaMenu_AC4[MGetLanguage()][0],4);

                while(1)
                {
                    keycode = MAnyKeyReturn();
                    if (keycode == C_KEYCOD_RETURN||keycode == C_KEYCOD_CONFIRM)
                        break;
                    else if(keycode >= 1 && keycode <= 4)
                    {
                        MSetScreenBrightness(keycode-1,C_SETMODE_SETSAVE);
                        break;
                    }
                }
                MPopWindow();
#elif C_COLOR == 1
                deci_len = 1;
                number = 0;
                int k,j;
                while( true )//��δ������������������
                {
                    if( Input_Number(xpos,ypos,&number,4, &deci_len,0) != 1)
                        break;
                    else
                    {
                        MSetThick(number);
                        number /= 10;
                        if( MGetUnitType() > 0)number = (UINT)(number * 254/1000.0+0.5);//��λinch
                        k = c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_THNUM];
                        for( i = 0 ; i < k; i++)
                        {
                            if(MGetStdMode()==0)break;
                            if(MGetStdMode()==10||MGetStdMode()==11)
                            {
                                if( number >=c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_THICK+7*i] )continue;

                            }
                            else
                            {
                                if( number >c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_THICK+7*i] )continue;

                            }
                            for( j = 0; j < MGetCurveNumber(); j++)
                            {
                                MSetLineGain(j, c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_THICK+7*i+j+1] );
                            }
                            break;
                        }
                        break;
                    }
                }
#endif
                break;
            case 4://��ӡ��
                epText = (u_char*)_ParaMenu_AB3;
                //ypos -= erow_height * 4 ;
                MPushWindow(xpos, ypos, xpos + 8*C_CCHAR_HDOT, ypos + erow_height * 4) ;
                EraseDrawRectangle(xpos, ypos, xpos + 8*C_CCHAR_HDOT, ypos + erow_height * 4) ;
                EMenuOut(xpos+4,ypos+4, epText ,15,3,15);	/*��ָ��λ�ø���ÿ���ַ������������и�д�˵�*/
                while(1)
                {
                    keycode = MAnyKeyReturn();
                    if (keycode == C_KEYCOD_RETURN||keycode == C_KEYCOD_CONFIRM)
                        break;
                    else if(keycode >= 1 && keycode <= 3)
                    {
                        MSetPrinterMode(keycode-1,0/*mode = 0\1\2*/);	/*����̽ͷģʽ*/
                        break;
                    }
                }
                MPopWindow();

                break;
            case 5:	//������׼
                //	cpText = (u_short*)_ParaMenu_AC2 ;
                if( MGetCurveNumber() > 3)ypos -= crow_height * ( MGetCurveNumber()-3 );
                MPushWindow(xpos, ypos, xpos + 6*C_CCHAR_HDOT, ypos + crow_height * ( MGetCurveNumber()+1 ) ) ;
                EraseDrawRectangle(xpos, ypos, xpos + (6)*C_CCHAR_HDOT, ypos + crow_height * ( MGetCurveNumber()+1 )) ;
                //	CEMenuOut(xpos+4,ypos+4 , cpText, 4,MGetCurveNumber()+1,crow_height);	/*��ָ��λ�ø���ÿ���ַ������������и�д�˵�*/
                TextOut(xpos+4,ypos+4 ,MGetCurveNumber()+1,10,crow_height,(u_char*)_ParaMenu_AC2[MGetLanguage()][0],4);
                while(1)
                {
                    keycode = MAnyKeyReturn();
                    if (keycode == C_KEYCOD_RETURN||keycode == C_KEYCOD_CONFIRM)
                        break;
                    else if(keycode > 0 && keycode <= MGetCurveNumber()+1 )
                    {
                        MSetDacMode(keycode-1,C_SETMODE_SAVE);
                        break;
                    }
                }
                MPopWindow();
                break;
            case 6:
                //	cpText = (u_short*)_ParaMenu_AD2 ;
                MPushWindow(xpos, ypos, xpos+ 6*C_CCHAR_HDOT, ypos + 4 * crow_height) ;
                EraseDrawRectangle(xpos, ypos, xpos+ 6*C_CCHAR_HDOT, ypos + 4 * crow_height) ;
                //	CEMenuOut(xpos + C_ECHAR_HDOT,ypos+4,cpText,5,4,crow_height);	/*��ָ��λ�ø���ÿ���ַ������������и�д�˵�*/
                for (i=0; i<deci_len; i++)
                {
                    TextOut(xpos+C_ECHAR_HDOT,ypos+ i * crow_height+4,1,8,16,_ParaMenu_AD2[MGetLanguage()][i],4);
                }
                while(1)
                {
                    keycode = MenuKeyReturn(4,2);	/*���ֲ˵�ʱ����������keymax�ļ����أ�mode=0��������Ч��=1ȷ�ϼ���Ч��=2���ؼ���Ч��=3ȷ�Ϸ��ؼ���Ч*/
                    if (keycode == C_KEYCOD_RETURN||keycode == C_KEYCOD_CONFIRM)
                        break;
                    else if(keycode >= 1 && keycode <= 4)
                    {
                        MSetEchoMode(keycode-1,C_SETMODE_SETSAVE);
                        break;
                    }
                }
                MPopWindow();
                break;
            case 7:
                ypos-=crow_height*2;
                MPushWindow(xpos, ypos, xpos+ 11*C_ECHAR_HDOT, ypos + 3 * crow_height+5) ;
                EraseDrawRectangle(xpos, ypos, xpos+ 11*C_ECHAR_HDOT, ypos + 3 * crow_height+5) ;
                //	EMenuOut(xpos + C_ECHAR_HDOT,ypos+4,_ParaMenu_AD3,9,3,15);	/*��ָ��λ�ø���ÿ���ַ������������и�д�˵�*/

                TextOut(xpos + C_ECHAR_HDOT,ypos+4, 1, 10,crow_height, (u_char*)_ParaMenu_AD3[MGetLanguage()][0], 4);
                TextOut(xpos + C_ECHAR_HDOT,ypos+4+crow_height, 1, 10,crow_height, (u_char*)_ParaMenu_AD3[MGetLanguage()][1], 4);
                TextOut(xpos + C_ECHAR_HDOT,ypos+4+crow_height*2, 1, 10,crow_height, (u_char*)_ParaMenu_AD3[MGetLanguage()][2], 4);

                while(1)
                {
                    keycode = MenuKeyReturn(4,2);	/*���ֲ˵�ʱ����������keymax�ļ����أ�mode=0��������Ч��=1ȷ�ϼ���Ч��=2���ؼ���Ч��=3ȷ�Ϸ��ؼ���Ч*/
                    if (keycode == C_KEYCOD_RETURN||keycode == C_KEYCOD_CONFIRM)
                        break;
                    else if(keycode >= 1 && keycode <= 4)
                    {
                        MSetBandMode(keycode-1,C_SETMODE_SAVE);
                        break;
                    }
                }
                MPopWindow();
                break;
            case 8:
                MPushWindow(xpos, ypos, xpos+ 12*C_ECHAR_HDOT, ypos + 3 * erow_height+1) ;
                EraseDrawRectangle(xpos, ypos, xpos+ 12*C_ECHAR_HDOT, ypos + 3 * erow_height) ;
                EMenuOut(xpos + C_ECHAR_HDOT,ypos+6,_ParaMenu_AD4,10,2,17);	/*��ָ��λ�ø���ÿ���ַ������������и�д�˵�*/
                while(1)
                {
                    keycode = MenuKeyReturn(2,2);	/*��������keymax�ļ����أ�mode=0��������Ч��=1ȷ�ϼ���Ч��=2���ؼ���Ч��=3ȷ�Ϸ��ؼ���Ч*/
                    if (keycode == C_KEYCOD_RETURN||keycode == C_KEYCOD_CONFIRM)
                        break;
                    else if(keycode >= 1 && keycode <= 2)
                    {
                        MSetDampMode(keycode-1,C_SETMODE_SAVE);
                        break;
                    }
                }
                MPopWindow();
                break;
            case 9:
                MPushWindow(xpos, ypos - erow_height, xpos+ 12*C_ECHAR_HDOT, ypos+ 7 * erow_height) ;
                EraseDrawRectangle(xpos, ypos - erow_height, xpos+ 12*C_ECHAR_HDOT, ypos + 7 * erow_height) ;
                //EMenuOut(xpos + C_ECHAR_HDOT,ypos + 6 - erow_height,_ParaMenu_AD5,10,2,17);	/*��ָ��λ�ø���ÿ���ַ������������и�д�˵�*/

                TextOut(xpos + C_ECHAR_HDOT,ypos + 6 ,4,6,17,(char *)_ParaMenu_AD5[MGetLanguage()][0],4);
                TextOut(xpos + C_ECHAR_HDOT,ypos + 6+17 ,4,6,17,(char *)_ParaMenu_AD5[MGetLanguage()][1],4);
                TextOut(xpos + C_ECHAR_HDOT,ypos + 6+17*2 ,4,6,17,(char *)_ParaMenu_AD5[MGetLanguage()][2],4);
                TextOut(xpos + C_ECHAR_HDOT,ypos + 6+17*3 ,4,6,17,(char *)_ParaMenu_AD5[MGetLanguage()][3],4);
                while(1)
                {
#if C_DEVLIB == 23 || C_DEVLIB == 24
                    keycode = MenuKeyReturn(4,2);	/*���ֲ˵�ʱ����������keymax�ļ����أ�mode=0��������Ч��=1ȷ�ϼ���Ч��=2���ؼ���Ч��=3ȷ�Ϸ��ؼ���Ч*/
                    if (keycode == C_KEYCOD_RETURN||keycode == C_KEYCOD_CONFIRM)
                        break;
                    else if(keycode >= 1 && keycode <= 4)
#else
                    keycode = MenuKeyReturn(2,2);	/*���ֲ˵�ʱ����������keymax�ļ����أ�mode=0��������Ч��=1ȷ�ϼ���Ч��=2���ؼ���Ч��=3ȷ�Ϸ��ؼ���Ч*/
                    if (keycode == C_KEYCOD_RETURN||keycode == C_KEYCOD_CONFIRM)
                        break;
                    else if(keycode >= 1 && keycode <= 2)
#endif
                    {
                        MSetPulseMode(keycode-1,C_SETMODE_SAVE);
                        break;
                    }
                }
                MPopWindow();
                break;
            }
        }
        else if( page == 2)
        {
            switch (keycode)
            {
#if C_UDISK == 1

            case 1:	//�洢ģʽ
                //*
                MPushWindow(xpos, ypos, xpos + 5*C_CCHAR_HDOT+1, ypos + crow_height * 2) ;
                EraseDrawRectangle(xpos, ypos, xpos + (5)*C_CCHAR_HDOT, ypos + crow_height * 2) ;
                TextOut(xpos+4,ypos+4 ,2,8,crow_height,(u_char*)_ParaMenu_AC1[MGetLanguage()][0],4);
                while(1)
                {
                    keycode = MAnyKeyReturn();
                    if (keycode == C_KEYCOD_RETURN||keycode == C_KEYCOD_CONFIRM)
                        break;

                    /*	else if(keycode >= 1 && keycode <= 2)
                    	{
                    		MSetSaveMode(keycode-1,C_SETMODE_SAVE);
                    		if( MGetSaveTime() > 600 )MSetSaveTime(10,C_SETMODE_SAVE);
                    		break;
                    	}*/
                }
                MPopWindow();
                //*/
                break;
            case 2:	//��������ʱ��
                //*
                deci_len = 1;
                number = 100;
                while( MGetSaveMode() )//��δ������������������
                {
                    if( Input_Number(xpos,ypos,&number,3, &deci_len,0) != 1)
                        break;
                    else if(number >= 1 && number <= 250)
                    {
                        MSetSaveTime(number,C_SETMODE_SAVE);
                        break;
                    }
                }
                //*/
                break;

#endif
            case 3:	//B������
                MPushWindow(xpos, ypos, xpos + 5*C_CCHAR_HDOT, ypos + crow_height * 2) ;
                EraseDrawRectangle(xpos, ypos, xpos + (5)*C_CCHAR_HDOT, ypos + crow_height * 2) ;
                TextOut(xpos+4,ypos+4,2,8+2*MGetLanguage(),crow_height,(u_char*)_ParaMenu_AC5[MGetLanguage()][0],4);
                while(1)
                {
                    keycode = MAnyKeyReturn();
                    if (keycode == C_KEYCOD_RETURN||keycode == C_KEYCOD_CONFIRM)
                        break;
                    else if(keycode >= 1 && keycode <= 2)
                    {
                        MSetBGateMode(keycode-1,C_SETMODE_SAVE);
                        break;
                    }
                }
                MPopWindow();
                break;
            case 4:
                //	cpText = (u_short*)_ParaMenu_AD8 ;
                MPushWindow(xpos, ypos, xpos + 4*C_CCHAR_HDOT, ypos + crow_height * 2) ;
                EraseDrawRectangle(xpos, ypos, xpos + 4*C_CCHAR_HDOT, ypos + crow_height * 2) ;
                //	CEMenuOut(xpos+4,ypos+4 , cpText ,2,2,crow_height);	/*��ָ��λ�ø���ÿ���ַ������������и�д�˵�*/
                TextOut(xpos+4,ypos+4,2,6,crow_height,(u_char*)_ParaMenu_AD8[MGetLanguage()][0],4);
                while(1)
                {
                    keycode = MAnyKeyReturn();
                    if (keycode == C_KEYCOD_RETURN||keycode == C_KEYCOD_CONFIRM)
                        break;
                    else if(keycode >= 1 && keycode <= 2)
                    {
                        MSetSoundAlarm(0, keycode-1);
                        break;
                    }
                }
                MPopWindow();
                break;
            case 5:
                ypos -= 10;
                MPushWindow(xpos, ypos, xpos+ 11*C_CCHAR_HDOT, ypos + 5 * crow_height) ;
                EraseDrawRectangle(xpos, ypos, xpos+ 11*C_CCHAR_HDOT, ypos + 5 * crow_height) ;
                //CMenuOut(xpos + C_ECHAR_HDOT,ypos+4,cpText,6,5,crow_height);
                int i = 0;
                int ypos1;
                for (i=0; i<5; i++)
                {
                    TextOut(xpos+ C_ECHAR_HDOT,ypos+4+i*crow_height,1,12,crow_height,(u_char*)_ParaProtect1[MGetLanguage()][i],4);
                }
                if (MGetLanguage())
                {
                    TextOut(xpos+ C_ECHAR_HDOT,ypos+4+4*crow_height-8,1,12,crow_height,(u_char*)_ParaProtect1[MGetLanguage()][3]+12,4);
                }

                xpos += 6*C_CCHAR_HDOT;

                for(i = 0; i < C_LOCK_ALL; i ++)
                {
                    ypos1 = ypos + crow_height * (i +2)+4;
                    //	cpText = (u_short*)_ParaProtect2 + MGetParaLock(0,i) * 2 ;
                    //CMenuOut(xpos + C_ECHAR_HDOT,ypos1,cpText,2,1,crow_height);
                    TextOut(xpos+ C_ECHAR_HDOT,ypos1,1,8,crow_height,(u_char*)_ParaProtect2[MGetLanguage()][MGetParaLock(0,i)],0);
                }

                do
                {
                    keycode = MAnyKeyReturn();
                    if (keycode == C_KEYCOD_RETURN || keycode == C_KEYCOD_CONFIRM)
                        break;

                    if (keycode == 0)
                    {
                        MSetParaLock(1, C_LOCK_ALL);	//���ж�������
                    }
                    else if(keycode >0 && keycode <= C_LOCK_ALL)
                    {
                        MSetParaLock(MGetParaLock(0,keycode-1)+1, keycode-1);	//
                    }
                    else continue;
                    for(i = 0; i < C_LOCK_ALL; i ++)
                    {
                        ypos1 = ypos + crow_height * (i +2)+4;
                        //	cpText = (u_short*)_ParaProtect2 + MGetParaLock(0,i) * 2 ;
                        //CMenuOut(xpos + C_ECHAR_HDOT,ypos1,cpText,2,1,crow_height);
                        TextOut(xpos+ C_ECHAR_HDOT,ypos1,1,8,crow_height,(u_char*)_ParaProtect2[MGetLanguage()][MGetParaLock(0,i)],0);
                    }
                }
                while(1);
                MPopWindow();
                break;
            case 6:
                deci_len = 0;
                number = 100;
                while( true )//��δ������������������
                {
                    if( Input_Number(xpos,ypos,&number,2, &deci_len,0) != 1)
                        break;
                    else if(number >= 40 && number <= 90)
                    {
                        MSetAmpStdMax( number);
                        break;
                    }
                }
                break;
            case 7:
                //	cpText = (u_short*)_ParaMenu_AD9 ;
                MPushWindow(xpos, ypos, xpos + 4*C_CCHAR_HDOT, ypos + crow_height * 2) ;
                EraseDrawRectangle(xpos, ypos, xpos + 4*C_CCHAR_HDOT, ypos + crow_height * 2) ;
                //	CEMenuOut(xpos+4,ypos+4 , cpText ,2,2,crow_height);	/*��ָ��λ�ø���ÿ���ַ������������и�д�˵�*/
                TextOut(xpos,ypos,2,6,crow_height,(u_char*)_ParaMenu_AD9[MGetLanguage()][0],4);
                while(1)
                {
                    keycode = MAnyKeyReturn();
                    if (keycode == C_KEYCOD_RETURN||keycode == C_KEYCOD_CONFIRM)
                        break;
                    else if(keycode >= 1 && keycode <= 2)
                    {
                        MSetSaveFrequency(keycode-1,0);
                        break;
                    }
                }
                if (MGetFrequence() != 0)
                {
                    MSetFunctionMode(0,C_ECHO_ENVELOPE);
                    MSetFunctionMode(0,C_ECHOMAX_MEMORY);
                }
                MPopWindow();
                break;
            case 8:
                deci_len = 1;
                number = 0;
                while( true )//��δ������������������
                {
                    int i,j,k;
                    if( Input_Number(xpos,ypos,&number,4, &deci_len,0) != 1)
                        break;
                    else
                    {
                        MSetThick(number);
                        number /= 10;
                        if( MGetUnitType() > 0)number = (UINT)(number * 254/1000.0+0.5);//��λinch
                        k = c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_THNUM];
                        for( i = 0 ; i < k; i++)
                        {
                            if(MGetStdMode()==0)break;
                            if(MGetStdMode()==10||MGetStdMode()==11)
                            {
                                if( number >=c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_THICK+7*i] )continue;

                            }
                            else
                            {
                                if( number >c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_THICK+7*i] )continue;

                            }
                            for( j = 0; j < MGetCurveNumber(); j++)
                            {
                                MSetLineGain(j, c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_THICK+7*i+j+1] );
                            }
                            break;
                        }
                        break;
                    }
                }
                break;
            case 9:
                deci_len = 1;
                number = 0;
                while( true )//��δ������������������
                {
                    if( Input_Number(xpos,ypos,&number,4, &deci_len,0) != 1)
                        break;
                    else
                    {
                        MSetDiameter(number);
                        break;
                    }
                }
                break;

            }
            ExpendTime(20);
        }
    }
    SystemParaRestore();
    ChannelParaRestore();
    MSetScreenBrightness( MGetScreenBrightness(),C_SETMODE_SETSAVE);
    ClearEnvelope();	//�������ֵ
    MSetSystem();
    MSetColor(C_CR_MENU);
    MSetAcquisitionEnable(1,C_SETMODE_SETSAVE);
    MSetHardEchoShow(1,C_SETMODE_SETSAVE);
//	ScreenRenovate();	/*��Ļˢ��*/
//	DrawDac(0);		//�»�DAC
    return 1;
}


int FuncMenu(void)
{
    u_int char_len = 5,row_number = 9,crow_height = 40;
    int xpos,ypos,inv_xpos = 0,inv_ypos = 0;
    int keycode;
    u_short *cpText;
    u_char *epText;
    int i;
    int prestatus=0;
    u_int elapsedtime1 = GetElapsedTime() ;
    int deci_len ,number ,row;
    int para_xpos ,para_ypos;
#if 0
	#if C_DEVTYPE == 1
		row_number = 10;
		crow_height = 40;
	#elif C_DEVTYPE == 15
		row_number = 11;
		crow_height = 36;
	#endif
#else
	row_number = 11;
	crow_height = 36;
#endif

    MSetColor(C_CR_MENU);
//	MGateDraw();
    MSetAcquisitionEnable(0,C_SETMODE_SETSAVE);
    MSetHardEchoShow(0,C_SETMODE_SETSAVE);
    ClearEnvelope();
    SystemParaStore();
    ChannelParaStore();

    xpos = C_COORWIDTH - ( char_len + 1+5*MGetLanguage() ) * C_CCHAR_HDOT;
    ypos = C_COORVPOSI + 1;
//	MPushWindow(xpos, ypos, xpos + (char_len+1)*C_CCHAR_HDOT, ypos + crow_height * (row_number) ) ;
    EraseDrawRectangle(xpos, ypos, xpos + (char_len+1+6*MGetLanguage())*C_CCHAR_HDOT, ypos + crow_height * (row_number)+6*C_FILL ) ;
    inv_xpos = xpos + 4;
    inv_ypos = ypos + crow_height + 4;
    for (i=0; i<row_number; i++)
    {
        epText = (u_char*) _FuncMenu_A1[MGetLanguage()][i];
        TextOut(xpos+4,ypos+3+crow_height*i,1,11+5*MGetLanguage(),crow_height,epText,0);
        // if (MGetLanguage())
        // {
        //     TextOut(xpos+4,ypos+3+C_ECHAR_VDOT+crow_height*i,1,11,8,epText+11,0);
        // }
		
    }

//	CEMenuOut(xpos+4,ypos+4 , cpText ,char_len,row_number,crow_height);	/*��ָ��λ�ø���ÿ���ַ������������и�д�˵�*/
    InvertWords(xpos+4+6*MGetLanguage()*C_ECHAR_HDOT, ypos+2, 2+1*MGetLanguage()); /*������ʾ�����ֵ�����*/

#if C_FILL == 1
    if(MGetFill() == 1)
        InvertWords(inv_xpos, inv_ypos + (8 )* crow_height-2,1 );
    MSetFunctionMode(0,C_WAVE_FILL);
    //	InvertWords(inv_xpos, inv_ypos + (9)* crow_height,1 );
#endif
#if C_DEVTYPE == 1
    for(i = 0; i < row_number-2; i++)
#elif C_DEVTYPE == 15
    for(i = 0; i < row_number-3; i++)
#endif
    {
        if( MGetFunctionMode(i) == 1 )InvertWords(inv_xpos, inv_ypos + (i + 1)* crow_height-2,1 ); /*������ʾ����*/
    }
    DisplayPrompt(15);
    while(1)
    {
        MKeyRlx();		/*ֻ�а������ͷŲ��˳�*/
        xpos = C_COORWIDTH - ( char_len + 1+5*MGetLanguage() ) * C_CCHAR_HDOT;
        ypos = C_COORVPOSI + 1;
        do
        {
            if( GetElapsedTime() > elapsedtime1 + 200 )
            {
                InvertWords(xpos+4+6*MGetLanguage()*C_ECHAR_HDOT, ypos+2, 2+1*MGetLanguage()); /*������ʾ�����ֵ�����*/
                elapsedtime1 = GetElapsedTime() ;
            }
            keycode = MGetKeyCode(10);

            if( keycode == C_KEYCOD_CONFIRM || keycode == C_KEYCOD_RETURN)break;
            if (keycode >= 0 && keycode < row_number - 1)break;
        }
        while(1);

//		keycode = MAnyKeyReturn();
        if(keycode == 0 )
        {
            //MChannelRenovate();

            if (MGetSaveMode()==1 && MGetSaveStatus()==1 )
            {
                continue;
            }
            InitPara();//��ʼ��
            MSetAcquisitionEnable(1,C_SETMODE_SETSAVE);
            MSetHardEchoShow(1,C_SETMODE_SETSAVE);
            ScreenRenovate();	/*��Ļˢ��*/
            DrawDac(0);		//�»�DAC
            return 1;
        }
#if C_DEVTYPE == 1|| C_DEVTYPE == 2 || C_DEVTYPE == 9 || C_DEVTYPE == 10 || C_DEVTYPE == 11 || C_DEVTYPE == 13 || C_DEVTYPE == 14 ||C_DEVTYPE == 15
//[Nilyou]
        else if(keycode == C_BSCAN + 1)//Bɨ��
        {
            TOFDFunc();
            return 1;
        }
//		else if(keycode == C_FLAW_HEIGHT);//���Ʋ��
#endif
        else if(keycode == C_KEYCOD_CONFIRM)
        {
            MActivateDac(MGetFunctionMode(C_DEPTH_COMPENSATE) ) ;

            //����������չ����B�Ų���Ϊ����
            if( MGetFunctionMode(C_CURVER_AMEND) == 1 )MSetBGateMode(0, C_SETMODE_SETSAVE);
#if C_ACQU == 1
            MSetAcquisitionEnable(MGetAcquisitionEnable(),C_SETMODE_SETSAVE);
#endif
            MSetSystem();
            SystemParaStore();
            ChannelParaStore();

            //MGateDraw();
            break;
        }
        else if(keycode == C_KEYCOD_RETURN)
        {
#if C_ACQU == 1
            MSetAcquisitionEnable(MGetAcquisitionEnable(),C_SETMODE_SETSAVE);
#endif
            MSetSystem();

            //MGateDraw();
            break;
        }

        keycode--;		//1-9 -> 0-8
        prestatus = MGetFunctionMode(keycode);	//ԭ״̬
        if(keycode <= C_FUNCTION_MAX) MSetFunctionMode( prestatus+1,keycode);
        switch (keycode)
        {
        case C_ECHO_ENVELOPE:
            if( MGetFunctionMode(C_ECHOMAX_MEMORY) == 1)
            {
                InvertWords(inv_xpos, inv_ypos + (C_ECHOMAX_MEMORY + 1) * crow_height-2,1 ); /*������ʾ����*/
                MSetFunctionMode(0,C_ECHOMAX_MEMORY);
            }
            if (MGetFunctionMode(C_ECHO_ENVELOPE) == 1)
            {
                MSetSaveFrequency(0,0);
            }
            break;
        case C_ECHOMAX_MEMORY:		//��ֵ����
            if( MGetFunctionMode(C_ECHO_ENVELOPE) == 1)
            {
                InvertWords(inv_xpos, inv_ypos + (C_ECHO_ENVELOPE + 1) * crow_height-2,1 ); /*������ʾ����*/
                MSetFunctionMode(0,C_ECHO_ENVELOPE);
            }
            if(MGetFunctionMode(C_GATE_ALARM) == 1)
            {
                InvertWords(inv_xpos, inv_ypos + (C_GATE_ALARM + 1) * crow_height-2,1 ); /*������ʾ����*/
                MSetFunctionMode(0,C_GATE_ALARM);

            }
            if (MGetFunctionMode(C_ECHOMAX_MEMORY) == 1)
            {
                MSetSaveFrequency(0,0);
            }
            break;
        case C_GATE_ALARM:
            if(MGetFunctionMode(C_ECHOMAX_MEMORY) == 1)
            {
                InvertWords(inv_xpos, inv_ypos + (C_ECHOMAX_MEMORY + 1) * crow_height-2,1 ); /*������ʾ����*/
                MSetFunctionMode(0,C_ECHOMAX_MEMORY);
            }
            break;
        case C_DAC_GATE:
//Write_Number(10,30,MGetFunctionMode(C_DEPTH_COMPENSATE),4,0,0);
//Write_Number(10,40,MGetFunctionMode(C_DAC_GATE),4,0,0);
//Write_Number(10,40,MGetFunctionMode(C_DAC_GATE),4,0,0);
            if( MGetFunctionMode(C_DAC_GATE) == 1 )
            {
                if( MGetTestStatus(C_TEST_DAC) == 0 && MGetTestStatus(C_TEST_AVG) == 0)
                {
                    MSetFunctionMode(0,C_DAC_GATE);
                }
                if( MGetFunctionMode(C_DEPTH_COMPENSATE) == 1 && MGetFunctionMode(C_DAC_GATE) == 1)
                {
                    InvertWords(inv_xpos, inv_ypos + (C_DEPTH_COMPENSATE + 1)* crow_height-2,1 ); /*������ʾ����*/
                    MSetFunctionMode(0,C_DEPTH_COMPENSATE);
                }
                if( MGetFunctionMode(C_DAC_GATE) == 1)
                {
                    int dac_dB;

                    dac_dB = MGetDacDb();	//dacdB����DACĸ����Ϊ����dBֵ
                    MSetGatedB(11, 0);
                    DacGateInit();
                    //MSetGatePara( MGetGatePara(0,0),MGetGatePara(0,1),DacGate_dBHigh(11,dac_dB),0,C_SETMODE_SETSAVE);
                }
            }
            else if( prestatus == 1)
            {
                //��DAC�Ÿĳ���ͨ��
                MSetGatePara( MGetGatePara(0,0),MGetGatePara(0,1),160,0,C_SETMODE_SETSAVE);
            }

            break;
        case C_DEPTH_COMPENSATE:
            if( MGetFunctionMode(C_DAC_GATE) == 1 && prestatus == 0)
            {
                InvertWords(inv_xpos, inv_ypos + (C_DAC_GATE + 1)* crow_height-2,1 ); /*������ʾ����*/
                MSetFunctionMode(0,C_DAC_GATE);
                MSetGatePara( MGetGatePara(0,0),MGetGatePara(0,1),160,0,C_SETMODE_SETSAVE);
            }
            if( MGetEquivalentDays(1) <= 0 || MGetEchoMode() == C_RF_WAVE)	//���ز���״̬<0=δ��>=0�Ѳ⣬
            {
                MSetFunctionMode( 0,C_DEPTH_COMPENSATE);
            }
            break;
        case C_CURVER_AMEND:
            keycode = C_CURVER_AMEND;
            if( MGetFunctionMode(keycode) == 0)break;
            /*
            if( MGetFunctionMode(C_CURVER_AMEND) != 1)break;
            else if( MGetAngle(0) == 0)
            {
            	MSetFunctionMode( 0,C_CURVER_AMEND);
            	break;
            }
            //*/
            cpText = (u_short*)_ParaMenu_A5 ;
            xpos = inv_xpos - 5* C_CCHAR_HDOT;
            ypos = inv_ypos + (C_CURVER_AMEND + 1) * crow_height;
            para_xpos = xpos + C_CCHAR_HDOT * 6 ;
            para_ypos = ypos + 8;
            if(MGetLanguage())
            {
                MPushWindow(xpos-32, ypos, xpos +13*C_CCHAR_HDOT+11, ypos + crow_height * 2+8) ;
                //   MPushWindow(xpos + 6*C_CCHAR_HDOT+10, ypos, xpos + 9*C_CCHAR_HDOT+11, ypos + crow_height * 2+8) ;
                EraseDrawRectangle(xpos-32, ypos, xpos + 13*C_CCHAR_HDOT+10, ypos + crow_height * 2+7) ;

                TextOut(xpos+4-32,ypos+4,1,15,16,(u_char*)_ParaMenu_A5[MGetLanguage()][0],4);
                TextOut(xpos+4-32,ypos+4+crow_height,1,15,16,(u_char*)_ParaMenu_A5[MGetLanguage()][1],4);
            }
            else
            {
                MPushWindow(xpos, ypos, xpos + 10*C_CCHAR_HDOT+11, ypos + crow_height * 2+8) ;
                EraseDrawRectangle(xpos, ypos, xpos + 10*C_CCHAR_HDOT+10, ypos + crow_height * 2+7) ;
                TextOut(xpos+4,ypos+8,1,14,32,(u_char*)_ParaMenu_A5[MGetLanguage()][0],4);
                TextOut(xpos+4,ypos+8+crow_height,1,14,32,(u_char*)_ParaMenu_A5[MGetLanguage()][1],4);
            }
            //	MPushWindow(xpos, ypos, xpos + 10*C_CCHAR_HDOT+1, ypos + crow_height * 3) ;
            //	EraseDrawRectangle(xpos, ypos, xpos + 10*C_CCHAR_HDOT, ypos + crow_height * 3) ;
            //	CEMenuOut(xpos+4,ypos+8 , cpText ,6,2,crow_height);	/*��ָ��λ�ø���ÿ���ַ������������и�д�˵�*/
            while(1)
            {
                row = 0;//�������
                xpos = para_xpos+C_CCHAR_HDOT * 3 *MGetLanguage();
                ypos = para_ypos + crow_height *row;
                Write_Number(xpos,ypos,MGetThick(),4,1,0);

                row = 1;//�����⾶
                ypos = para_ypos + crow_height *row ;
                Write_Number(xpos,ypos,MGetDiameter(),4,1,0);

                while(true)
                {
                    keycode = MAnyKeyReturn();
                    if (keycode == 1 || keycode == 2 || keycode == C_KEYCOD_CONFIRM || keycode == C_KEYCOD_RETURN)break;
                }

                if (keycode == C_KEYCOD_RETURN)
                {
                    MSetFunctionMode( 0,C_CURVER_AMEND);
                    break;
                }
                if (keycode == C_KEYCOD_CONFIRM)
                {
                    break;
                }
                row = keycode - 1;
                xpos = para_xpos+C_CCHAR_HDOT * 3 *MGetLanguage();
                ypos = para_ypos + crow_height * row-1;
                if( keycode == 1)
                {
                    int i,j,k;
                    deci_len = 1;
                    number = 0;
                    if( Input_Number(xpos,ypos,&number,4, &deci_len,0) == 1)
                    {
                        MSetThick(number);
                        number /= 10;
                        if( MGetUnitType() > 0)number = (UINT)(number * 254/1000.0+0.5);//��λinch
                        k = c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_THNUM];
                        for( i = 0 ; i < k; i++)
                        {
                            if(MGetStdMode()==0)break;
                            if(MGetStdMode()==10||MGetStdMode()==11)
                            {
                                if( number >=c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_THICK+7*i] )continue;

                            }
                            else
                            {
                                if( number >c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_THICK+7*i] )continue;

                            }
                            for( j = 0; j < MGetCurveNumber(); j++)
                            {
                                MSetLineGain(j, c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_THICK+7*i+j+1] );
                            }
                            break;
                        }
                    }
                }
                else if( keycode == 2)
                {
                    deci_len = 1;
                    number = 0;

                    if( Input_Number(xpos,ypos,&number,4, &deci_len,0) == 1)
                    {
                        //	if( number >= 2 * MGetThick() )
                        MSetDiameter(number);
                    }
                }
            }
            MPopWindow();
            keycode = C_CURVER_AMEND;
            break;

#if C_FILL == 1
        case C_WAVE_FILL:
            MSetFill(MGetFill()+1,C_SETMODE_SETSAVE);
            //	InvertWords(inv_xpos, inv_ypos + (6 + 1)* crow_height,1 ); /*������ʾ����*/
            break;
#endif
        case C_WAVE_EXPEND:		/*����չ��*/
            break;

        case C_AUTOADJUST_GAIN:
            break;
        }

        if( keycode <= C_FUNCTION_MAX && prestatus != MGetFunctionMode(keycode) )//״̬�Ѹı�
            InvertWords(inv_xpos, inv_ypos + (keycode + 1)* crow_height-2,1 ); /*������ʾ����*/
    }

    MSetColor(C_CR_UNDO);
    ClearEnvelope();	//�������ֵ
    SystemParaRestore();
    ChannelParaRestore();

    MSetAcquisitionEnable(1,C_SETMODE_SETSAVE);
    MSetHardEchoShow(1,C_SETMODE_SETSAVE);
    MSetSystem();
    ScreenRenovate();	/*��Ļˢ��*/
    DrawDac(0);

    return 1;
}

extern u_char crPara[];
extern const u_int c_crPara[] ;
extern const u_int Echo_crPara[] ;

const u_char _COLOR_Menu[C_LANGUAGE][9][15] =
{
    {
        "���Զ���",
        "����ͳ��",
        "��������",
        "��÷����",
        "�������",
        "�����ķ�",
        "�������",
        "�������",
        "�������",
    },
    {
        "0.Custom     ",
        "1.Tradition  ",
        "2.Blue       ",
        "3.Cyan       ",
        "4.Red        ",
        "5.Pink       ",
        "6.Green      ",
        "7.White      ",
        "8.Black      ",
    }
};
const u_char _COLOR_A1[C_LANGUAGE][10][15] =
{
    {
        "����Ļ��ɫ:",
        "����������:",
        "����λز�:",
        "��ż�λز�:",
        "����ʾ�˵�:",
        "����������:",
        "���������:",
        "�����ţ�  :",
        "�����ţ�  :",
        "����ֵ����:",
    },
    {
        "0.Background:",
        "1.WaveGround:",
        "2.ODD wave  :",
        "3.EVEN wave :",
        "4.TIP menu  :",
        "5.Curve     :",
        "6.Param     :",
        "7.Gate A    :",
        "8.Gate B    :",
        "9.Envelope  :",
    }
};
const u_char _COLOR_B1[C_LANGUAGE][9][8] =
{
    {
        "����",
        "����",
        "����",
        "����",
        "����",
        "����",
        "����",
        "����",
        "����",
    },
    {
        "1.Red   ",
        "2.Pink  ",
        "3.Yellow",
        "4.Green ",
        "5.Cyan  ",
        "6.Blue  ",
        "7.Gray  ",
        "8.White ",
        "9.Black ",
    }
};
/*


0����ɫ���ã��Զ���/Ĭ������1/Ĭ������2/Ĭ������3
0. �ָ�Ĭ��ֵ
1. ��λز���
2��ż�λز���
3����ʾ�˵���
4���������ߣ�
5. �������
6������A��
7������B��
8����ֵ���磺
9.��Ļ��ɫ

*/
int ColorMenu(void)
{
#if C_COLOR == 1 && C_DEVTYPE != 20
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

    u_char crTemp[C_CR_MAX+12];
    for( i = 0 ; i < C_CR_MAX+12 ; i++)crTemp[i] = crPara[i]; //����ԭ����
    crnum = 9;
    MSetColor(C_CR_MENU);
    MSetAcquisition(0);
    MSetHardEchoShow(0,C_SETMODE_SETSAVE);
    ClearEnvelope();
    SystemParaStore();
    ChannelParaStore();

    DisplayPrompt(15);
    while(true)
    {
        MFclearScreen();
        MSetColor(C_CR_MENU);
        EraseDrawRectangle(26*C_ECHAR_HDOT, 380, 26*C_ECHAR_HDOT + (6+1*MGetLanguage())*C_CCHAR_HDOT, 380 + 2 * (C_CCHAR_VDOT+12));
        MSetColor(C_CR_MENU);
        for (i=0; i<crnum; i++)
        {
            TextOut(C_ECHAR_HDOT,para_ypos+crow_height*i,1,16,24,(u_char*)_COLOR_Menu[MGetLanguage()][i],4);
        }

        TextOut(27*C_ECHAR_HDOT,380+4,1,10,24,(u_char*)_MenuPrompt_B1[MGetLanguage()][0],4);
        TextOut(27*C_ECHAR_HDOT,380+4+36,1,10,24,(u_char*)_MenuPrompt_B1[MGetLanguage()][0]+10,4);
//        EMenuOut(27*C_ECHAR_HDOT,380+4,_TestMenu_Enter,1,1,C_ECHAR_VDOT);
        MSetColor(C_CR_UNDO);

        /*    for( i = 0,row = 0; i < crnum ; i++,row++)
            {
                xpos = para_xpos + C_ECHAR_HDOT+8*MGetLanguage();
                ypos = para_ypos + crow_height *row ;
                MSetColor( i ) ;
                TextOut(xpos,ypos,1,6,crow_height,(u_char*)_COLOR_B1[MGetLanguage()][MLoadColor(i)]+2,4);
                MSetColor(C_CR_UNDO);
            }*/
        MKeyRlx();		/*ֻ�а������ͷŲ��˳�*/

        xpos = para_xpos;
        ypos = para_ypos + crow_height *MGetColorMode();
        InvertWords(C_ECHAR_HDOT,ypos, 1); /*������ʾ�����ֵ�����*/
        do
        {
            if( GetElapsedTime() > elapsedtime1 + 200 )
            {
                InvertWords(27*C_ECHAR_HDOT,380+4, 1); /*������ʾ�����ֵ�����*/
                elapsedtime1 = GetElapsedTime() ;
            }
            keycode = MGetKeyCode(10);
            if( keycode >= 0 && keycode < crnum )break;
        }
        while( keycode != C_KEYCOD_RETURN && keycode != C_KEYCOD_CONFIRM );


        if (keycode == C_KEYCOD_RETURN)
        {
#if C_ACQU == 1
            MSetAcquisitionEnable(MGetAcquisitionEnable(),C_SETMODE_SETSAVE);
#endif
            SystemParaRestore();
            ChannelParaRestore();
            for( i = 0 ; i < C_CR_MAX+12 ; i++)crPara[i] = crTemp[i] ; //�ָ�ԭ����
            retvalue = C_FALSE;
            break;
        }
        else if (keycode == C_KEYCOD_CONFIRM)
        {
#if C_ACQU == 1
            MSetAcquisitionEnable(MGetAcquisitionEnable(),C_SETMODE_SETSAVE);
#endif
            MSetSystem();

            offset = C_OFF_TEMP+60;	//��ɫ�洢λ��
            MCopyProtectedMemory( (void*)offset, crPara, C_CR_MAX+12, PM_COPY_WRITE);
//            CopyMemoryToSystem( (void*)offset,C_CR_MAX+12,PM_COPY_WRITE);
            SystemParaStore();
            ChannelParaStore();

            retvalue = C_TRUE;
            break;
        }
        /*   else if( keycode == C_KEYCOD_COLOR)
           {
               MSaveDefualtColor(MGetColorMode()+1);//����Ĭ����ɫ
               continue;
           }*/
        else if( keycode > 0)
        {
//;			MSaveDefualtColor(0);//����Ĭ����ɫ
            MSaveDefualtColor(keycode);//����Ĭ����ɫ
            continue;
        }
        else if( keycode == 0)
        {
            MSaveDefualtColor(keycode);//����Ĭ����ɫ
            CustomizeColorMenu();

        }

    }
    SetBackgroundColor(c_crPara[crPara[ C_CR_BACK] ]);
    MSetColor(C_CR_UNDO);
    MSetColor(C_CR_WAVE);
    MSetSystem();
    MSetAcquisition(1);
    ClearEnvelope();	//�������ֵ
    ScreenRenovate();	/*��Ļˢ��*/
    DrawDac(0);		//�»�DAC
#endif
    return 1;
}
int CustomizeColorMenu(void)
{
#if C_COLOR == 1 && C_DEVTYPE != 20
    u_int char_len = 6, crow_height = 48;//,row_number = 10,  erow_height = 12;
    //int number,deci_len,sign;
    int xpos,ypos;
    //int menu_xpos = 0,menu_ypos = 0;
    int para_xpos = C_CCHAR_HDOT * ( char_len + 3*MGetLanguage()) ,para_ypos =  4;
    int retvalue = C_TRUE;
    int keycode;
    //int page_no = 1,page_max = 3;
    int row;
    u_short* cpText = 0;
    //u_char* epText = 0;
    int i;//,j,k;
    int len;//,line;
    int item;
    u_int elapsedtime1 = GetElapsedTime();// ,elapsedtime2;
    int offset;
    int crnum = 0;

//#if C_DEVTYPE == 20
//	return 0;
//#endif
    u_char crTemp[C_CR_MAX+12];
    for( i = 0 ; i < C_CR_MAX+12 ; i++)crTemp[i] = crPara[i]; //����ԭ����

#if C_DEVTYPE == 1 || C_DEVTYPE == 8  || C_DEVTYPE == 9  || C_DEVTYPE == 10  || C_DEVTYPE == 11 || C_DEVTYPE == 12 || C_DEVTYPE == 13 || C_DEVTYPE == 14 ||C_DEVTYPE == 15
    crnum = 10;
#elif C_DEVTYPE == 4 || C_DEVTYPE == 20 || C_DEVTYPE == 5
    crnum = 8;
#endif
    MSetColor(C_CR_MENU);
    MSetAcquisition(0);
    MSetHardEchoShow(0,C_SETMODE_SETSAVE);
    ClearEnvelope();
    SystemParaStore();
    ChannelParaStore();

    DisplayPrompt(15);
    while(true)
    {
        MFclearScreen();

        //cpText =  (u_short*)_COLOR_A1;
        //epText =  (u_char*)_STDMenu_B1;
        MSetColor(C_CR_MENU);
        EraseDrawRectangle(26*C_ECHAR_HDOT, 380, 26*C_ECHAR_HDOT + (6+1*MGetLanguage())*C_CCHAR_HDOT, 380 + 2 * (C_CCHAR_VDOT+12));
        MSetColor(C_CR_MENU);
        for (i=0; i<crnum; i++)
        {
            TextOut(C_ECHAR_HDOT,para_ypos+crow_height*i,1,16,24,(u_char*)_COLOR_A1[MGetLanguage()][i],4);
        }
        //CEMenuOut(C_ECHAR_HDOT,para_ypos, cpText ,char_len,crnum,crow_height);	/*��ָ��λ�ø���ÿ���ַ������������и�д�˵�*/
        //EMenuOut(para_xpos+C_ECHAR_HDOT*5,para_ypos + 8, epText ,3,row_number,crow_height);	/*��ָ��λ�ø���ÿ���ַ������������и�д�˵�*/

        //CMenuOut(27*C_ECHAR_HDOT,190+4,_MenuPrompt_B1,5,2,24);	/*��ָ��λ�ø���ÿ���ַ������������и�д�˵�*/
        TextOut(27*C_ECHAR_HDOT,380+4,1,10,24,(u_char*)_MenuPrompt_B1[MGetLanguage()][0],4);
        TextOut(27*C_ECHAR_HDOT,380+4+36,1,10,24,(u_char*)_MenuPrompt_B1[MGetLanguage()][0]+10,4);
//        EMenuOut(27*C_ECHAR_HDOT,380+4,_TestMenu_Enter,1,1,C_ECHAR_VDOT);
        MSetColor(C_CR_UNDO);
        //MSetColor(C_CR_PARA);

        //
        for( i = 0,row = 0; i < crnum ; i++,row++)
        {
            xpos = para_xpos + C_ECHAR_HDOT+8*MGetLanguage();
            ypos = para_ypos + crow_height *row ;
            MSetColor( i ) ;
            TextOut(xpos,ypos,1,6,crow_height,(u_char*)_COLOR_B1[MGetLanguage()][MLoadColor(i)]+2,4);
            MSetColor(C_CR_UNDO);
        }
        MKeyRlx();		/*ֻ�а������ͷŲ��˳�*/

        do
        {
            if( GetElapsedTime() > elapsedtime1 + 200 )
            {
                InvertWords(27*C_ECHAR_HDOT,380+4, 1); /*������ʾ�����ֵ�����*/
                elapsedtime1 = GetElapsedTime() ;
            }
            keycode = MGetKeyCode(10);
            if( keycode >= 0 && keycode < crnum )break;
        }
        while( keycode != C_KEYCOD_RETURN && keycode != C_KEYCOD_CONFIRM && keycode != C_KEYCOD_COLOR );

        if (keycode == C_KEYCOD_RETURN)
        {
#if C_ACQU == 1
            MSetAcquisitionEnable(MGetAcquisitionEnable(),C_SETMODE_SETSAVE);
#endif
            SystemParaRestore();
            ChannelParaRestore();
            for( i = 0 ; i < C_CR_MAX+12 ; i++)crPara[i] = crTemp[i] ; //�ָ�ԭ����
            retvalue = C_FALSE;
            break;
        }
        else if (keycode == C_KEYCOD_CONFIRM)
        {
#if C_ACQU == 1
            MSetAcquisitionEnable(MGetAcquisitionEnable(),C_SETMODE_SETSAVE);
#endif
            MSetSystem();

            offset = C_OFF_TEMP+60;	//��ɫ�洢λ��
            MCopyProtectedMemory( (void*)offset, crPara, C_CR_MAX+12, PM_COPY_WRITE);
//            CopyMemoryToSystem( (void*)offset,C_CR_MAX+12,PM_COPY_WRITE);
            SystemParaStore();
            ChannelParaStore();

            retvalue = C_TRUE;
            break;
        }
        else if( keycode == C_KEYCOD_COLOR)
        {
            MSaveDefualtColor(MGetColorMode()+1);//����Ĭ����ɫ
            continue;
        }
        /*  else if( keycode == 0)
          {
        //;			MSaveDefualtColor(0);//����Ĭ����ɫ
              MSaveDefualtColor(MGetColorMode()+1);//����Ĭ����ɫ
              continue;
          }*/

        row = item = keycode ;
        //item--;
        xpos = para_xpos;
        ypos = para_ypos + crow_height * row-1;

        InvertWords(C_ECHAR_HDOT,ypos, 1); /*������ʾ�����ֵ�����*/
        ypos = para_ypos + crow_height * 0;

        len = 2;
        row = 9;
        MPushWindow(xpos, ypos, xpos+ (len+5+3*MGetLanguage())*C_ECHAR_HDOT, ypos + (row+0) * crow_height) ;
        EraseDrawRectangle(xpos, ypos, xpos+ (len+5+3*MGetLanguage())*C_ECHAR_HDOT, ypos + (row+0) * crow_height) ;
        for( i = 0; i < row ; i++)
        {
#if( C_COLOR == 1)
            if( i == row - 1)SetDisplayColor( RED_B0|GREEN_B0|BLUE_B0 );
            else SetDisplayColor( c_crPara[ i ] );
#endif
            TextOut(xpos + C_ECHAR_HDOT,ypos+8+i*crow_height,1,3+5*MGetLanguage(),crow_height,(u_char*)_COLOR_B1[MGetLanguage()][i],4);
        }
        while(1)
        {
            /*  MSetColor(C_CR_MENU);
              Write_Number(100,50,MLoadColor(0),1,0,0);
              Write_Number(100,70,MLoadColor(1),1,0,0);
              Write_Number(100,90,item,1,0,0);
              MAnyKeyReturn();*/
            keycode = MenuKeyReturn(row,2);	/*���ֲ˵�ʱ����������keymax�ļ����أ�mode=0��������Ч��=1ȷ�ϼ���Ч��=2���ؼ���Ч��=3ȷ�Ϸ��ؼ���Ч*/

            if(MLoadColor(1) == (keycode-1) && (item==2||item==3||item==5||item==7||item==8||item==9))  //�벨��������ɫ
                continue;
            if(MLoadColor(0) == (keycode-1)&& (item==4||item==6))  //���ɫ������ɫ
                continue;
            /*   for(i=0;i<row;i++)
               {
                   if(item==0 && MLoadColor(i) ==  (keycode-1))  //��ɫ������������ɫ
                   {
                       i=250;
                       break;
                   }
                   if(item==1 && MLoadColor(i) ==  (keycode-1))  //����������������ɫ
                   {
                       i=250;
                       break;
                   }
               }
               if(i==250)continue;*/

            if (keycode == C_KEYCOD_RETURN||keycode == C_KEYCOD_CONFIRM)
                break;
            else if(keycode >= 1 && keycode <= row)
            {
                MSaveColor(item,keycode-1 );
                //Write_Number( 200,30,item,4,0,0);
                //Write_Number( 200,40,keycode,4,0,0);
                //MAnyKeyReturn();
                break;
            }
        }
        MPopWindow();
    }
    SetBackgroundColor(c_crPara[crPara[ C_CR_BACK] ]);
    MSetColor(C_CR_UNDO);
    MSetColor(C_CR_WAVE);
    MSetSystem();
//    MSetAcquisition(1);
//    ClearEnvelope();	//�������ֵ
//    ScreenRenovate();	/*��Ļˢ��*/
//    DrawDac(0);		//�»�DAC
#endif
    return 1;
}

void InitPara(void)
{
    int xpos, ypos ;
    u_short* cpText ;
    int keycode ;
    int offset=0,offmax;
    int Zero;
    int row_number;
    int i;
    int crow_height = 48,erow_height =20;
    int language= MGetLanguage();
    int Unit = MGetUnitType();
    if (MGetSaveMode()==1 && MGetSaveStatus()==1 )
    {
        return;
    }

    SysPwd(0);
    MSetColor(C_CR_DEFAULT);

    offset = C_OFF_INNER_INFO;
    INNER_INFO InnerInfo;
    MCopyProtectedMemory(&InnerInfo, (void*)offset, sizeof(INNER_INFO), PM_COPY_READ);

    xpos = C_COORWIDTH - 6 * C_CCHAR_HDOT-16*MGetLanguage();
    ypos = C_COORVPOSI + 1 ;

    MFclearScreen();
    MSetColor(C_CR_MENU);
    row_number = 6;
    MPushWindow(xpos, ypos, xpos + 6*C_CCHAR_HDOT+MGetLanguage()*4*C_CCHAR_HDOT+2, ypos + crow_height *row_number+1 ) ;
    MEraseWindow(xpos, ypos, xpos + 6*C_CCHAR_HDOT+MGetLanguage()*4*C_CCHAR_HDOT, ypos + crow_height * row_number+1) ; /* ������� */
    for (i=0; i<row_number; i++)
// @todo (union#1#): Now Here
    {
        TextOut(xpos+4, ypos+4+i*crow_height,  1, 17,24, (u_char*)_FuncMenu_A2[MGetLanguage()][i], 4);
    }
    DrawRectangle(xpos, ypos, xpos + 6*C_CCHAR_HDOT+MGetLanguage()*4*C_CCHAR_HDOT, ypos + crow_height * row_number ) ; /* ������ */

    while(1)
    {
        keycode = MAnyKeyReturn();
        if (keycode == C_KEYCOD_RETURN || keycode == C_KEYCOD_CONFIRM)
            break;
        else if( (keycode >= 1 && keycode <row_number) || keycode == 9)
        {
            //if( DisplayQuery(3) == 0)break;

            if( keycode == 1)
            {
                offset = C_OFF_CHANNEL;
                // if( DisplayQuery(8) == 0)break;
                DisplayPrompt(17);//��ʾ�����������
                ChannelParaInit(0);	/*������ʼ����mode=0��ǰһ��ͨ����mode=1����ͨ��*/
            }
            else if (keycode == 2 && MGetSaveStatus()==0)	//������¼�½����嵱ǰͨ��
            {
                offset = C_OFF_SYSTEM;
                if( DisplayQuery(9) == 0)break;

                DisplayPrompt(17);//��ʾ�����������
                MSetChannel(0,0);
                ChannelParaInit(1);	/*������ʼ����mode=0��ǰһ��ͨ����mode=1����ͨ��*/
            }
            else if (keycode == 3 && MGetSaveStatus()==0)
            {
                offset = C_OFF_FILEINFO;
                int file_total;

                if( DisplayQuery(10) == 0)break;

                for(i=0; i<C_FILE_NUMBER; i++)
                {
                    cache.cache[i]=i;
                }
                offset = C_OFF_CACHE;
                MCopyProtectedMemory( (void*)offset, &cache, sizeof(CACHE), PM_COPY_WRITE);

                offset = C_OFF_FILEINFO;
                file_total = 0;		//��մ洢��̽������
                MCopyProtectedMemory( (void*)offset, &file_total, C_SIZE_INT, PM_COPY_WRITE);
                CopyMemoryToSystem((void*)offset, C_OFF_FAT-offset,PM_COPY_WRITE);

            }
            else if (keycode == 5)
            {
                MSetSystemTime() ;
            }
            else if (keycode == 6)
            {
                if((SDCardGetStatus()&0x01)==1)
                {
                    DisplayPrompt(19);	//��������U��
                    // U�̳�ʼ��
                    if(SDCard_Init())
                    {
                        // DisplayPrompt(20);	//U�����ӳɹ�
                        DisplayPrompt(29);	//SD���ڸ�ʽ��
                        if(SdCard_Format())
                        {
                            DisplayPrompt(30);	//SD��ʽ���ɹ�
                            ExpendTime(100);
                            DisplayPrompt(24);	//���ڰ�ȫ�Ͽ�
                            FileClose(0);
                            DisplayPrompt(23);	//U�̶Ͽ�����
                        }
                        else
                        {
                            DisplayPrompt(21);	//U������ʧ��
                        }
                    }

                }
                else
                {
                    DisplayPrompt(21);	//U������ʧ��
                }


            }
            else if ((keycode == 4 || (keycode == 9 && GetElapsedTime() < 1000 * 30) ) && MGetSaveStatus()==0)//����30�����ڳ�ʼ���а�9����ȫ������
            {
                if( DisplayQuery(11) == 0)break;
                int mode,i;

                if( keycode == 9 )
                {
                    //����ʱ��Լ��ʱ��
                    mode = 2;
                    for( i = 0; i < 5; i++)
                    {
                        offset = C_OFF_INIT + i * C_SIZE_INT;
                        MCopyProtectedMemory( &Zero, (void*)offset, C_SIZE_INT, PM_COPY_READ);
                        if( Zero != i)break;
                    }
                    if( i < 5 )//˵���ǵ�һ�γ�ʼ��
                    {
                        for( i = 0; i < 5; i++)
                        {
                            offset = C_OFF_INIT + i * C_SIZE_INT;
                            MCopyProtectedMemory( (void*)offset, &i,C_SIZE_INT, PM_COPY_WRITE);
                        }
                        offset = C_OFF_PWD;
                    }
                    else offset = C_OFF_STORE;
                }
                else
                {
                    mode = 1;
                    offset = C_OFF_DAC;
                }

                //�������
                Zero = 0;
                offmax = C_OFF_FILEMAX;
                DisplayPrompt(17);//��ʾ�����������
                u_int time = GetElapsedTime();
                u_int elapsedtime = 400;
                int xpos = (6+4*MGetLanguage())* C_CCHAR_HDOT;
                MSetColor(C_CR_MENU);
                for( ; offset < offmax; offset += C_SIZE_INT)
                {
                    MCopyProtectedMemory( (void*)offset, &Zero, C_SIZE_INT, PM_COPY_WRITE);	//д0
                    if( time + elapsedtime < GetElapsedTime() )
                    {
                        Write_Ascii(xpos,7,'.');
                        time = GetElapsedTime();
                        xpos += 8;
                    }
                }
                ExpendTime(100);
                Write_Ascii(xpos,7,'.');
                MSetChannel(0,0);




                ChannelParaInit(mode);	/*������ʼ����mode=0��ǰͨ����mode=1����ͨ��,2�����û�*/

                if(MGetStdMode()==2)
                {
                    int testrange[3];
                    testrange[1] = 1000;
                    testrange[2] = 0;
                    offset = C_OFF_TEMP + 0;
                    MCopyProtectedMemory( (void*)offset, testrange, 12, PM_COPY_WRITE);
                    testrange[0] = 500;
                    testrange[1] = 300;
                    offset = C_OFF_TEMP + 20;
                    MCopyProtectedMemory( (void*)offset, testrange, 8, PM_COPY_WRITE);
                    testrange[0] = 800;
                    testrange[1] = 10;
                    testrange[2] = 60;
                    offset = C_OFF_TEMP + 30;
                    MCopyProtectedMemory( (void*)offset, testrange, 12, PM_COPY_WRITE);


                }

                else if(MGetStdMode()==12)
                {
                    int testrange[3];
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

                if( keycode == 9)
                {
                    offset = C_OFF_INNER_INFO;
                    /*   INNER_INFO InnerInfo;
                       MCopyProtectedMemory(&InnerInfo, (void*)offset, sizeof(INNER_INFO), PM_COPY_READ);
                       InnerInfo.pzPurchaser[0] = 0;
                       //InnerInfo.pzSerial[0] = 0;
                       InnerInfo.MaintainTime.yearh = 0x20;
                       InnerInfo.MaintainTime.yearl = 0x14;
                       InnerInfo.MaintainTime.month = 1;
                       InnerInfo.MaintainTime.date = 1;

                       InnerInfo.LeaveFactoryTime.yearh = 0x20;
                       InnerInfo.LeaveFactoryTime.yearl = 0x10;
                       InnerInfo.LeaveFactoryTime.month = 0x10;
                       InnerInfo.LeaveFactoryTime.date = 1;
                       InnerInfo.StorageTime.yearh = 0x20;
                       InnerInfo.StorageTime.yearl = 0x10;
                       InnerInfo.StorageTime.month = 9;
                       InnerInfo.StorageTime.date = 1;*/
                    MCopyProtectedMemory((void*)offset, &InnerInfo, sizeof(INNER_INFO), PM_COPY_WRITE);

                    USER_INFO UserInfo;

                    UserInfo.pzPurchaser[0] = 0;
                    UserInfo.pzSerial[0] = UserInfo.pzUser[0] = UserInfo.pzGovernor[0] = 0;
                    UserInfo.PurchaseTime.yearh = 0x20;
                    UserInfo.PurchaseTime.yearl = 0x10;
                    UserInfo.PurchaseTime.month = 10;
                    UserInfo.PurchaseTime.date = 1;
                    offset = C_OFF_USER_INFO;
                    MCopyProtectedMemory((void*)offset, &UserInfo, sizeof(USER_INFO), PM_COPY_WRITE);
                    language=0;	//��9��ʼ���ָ�����
                    Unit = 0;

                }

                for(i=0; i<C_FILE_NUMBER; i++)
                {
                    cache.cache[i]=i;
                }
                offset = C_OFF_CACHE;
                MCopyProtectedMemory( (void*)offset, &cache, sizeof(CACHE), PM_COPY_WRITE);

                offset = C_OFF_INIT;
                CopyMemoryToSystem((void*)offset, C_OFF_BATTERY-offset,PM_COPY_WRITE);
                CopyMemoryAllSystem(PM_COPY_WRITE); //ȫ����ʼ����
                offset = C_OFF_FILEINFO;
                CopyMemoryToSystem((void*)offset, C_OFF_FAT-offset,PM_COPY_WRITE);

                offset = C_OFF_F_BACKUP;
                CopyMemoryToSystem((void*)offset, C_OFF_FILEMAX-offset,PM_COPY_WRITE);
            }

            //ChannelParaRestore();
            MSetSurfGain(0,2);
            break;
        }
    }
    MPopWindow() ;
    MSetColor( C_CR_MENU);
    MSetColor( C_CR_WAVE);
    MSetLanguage(language);
    MSetUnitType(Unit);
}

void InitParaTemp(void)
{
    int xpos, ypos ;
    u_short* cpText ;
    int keycode ;
    int offset,offmax;
    int Zero;
    int row_number;
    int i;
    int crow_height = 48,erow_height =20;
    int language= MGetLanguage();
    int Unit = MGetUnitType();
    if (MGetSaveMode()==1 && MGetSaveStatus()==1 )
    {
        return;
    }
    MSetColor(C_CR_DEFAULT);

    xpos = C_COORWIDTH - 6 * C_CCHAR_HDOT-16*MGetLanguage();
    ypos = C_COORVPOSI + 1 ;

    offset = C_OFF_INNER_INFO;
    INNER_INFO InnerInfo;
    MCopyProtectedMemory(&InnerInfo, (void*)offset, sizeof(INNER_INFO), PM_COPY_READ);

    MFclearScreen();
    MSetColor(C_CR_MENU);
    row_number = 5;
    MPushWindow(xpos, ypos, xpos + 6*C_CCHAR_HDOT+MGetLanguage()*3*C_CCHAR_HDOT+2, ypos + crow_height *row_number ) ;
    MEraseWindow(xpos, ypos, xpos + 6*C_CCHAR_HDOT+MGetLanguage()*3*C_CCHAR_HDOT, ypos + crow_height * row_number) ; /* ������� */
    for (i=0; i<row_number; i++)
// @todo (union#1#): Now Here
    {
        TextOut(xpos+4, ypos+4+i*crow_height,  1, 17,24, (u_char*)_FuncMenu_A2[MGetLanguage()][i], 4);
    }
    DrawRectangle(xpos, ypos, xpos + 6*C_CCHAR_HDOT+MGetLanguage()*3*C_CCHAR_HDOT, ypos + crow_height * row_number ) ; /* ������ */

    int mode;

    mode = 2;
    for( i = 0; i < 5; i++)
    {
        offset = C_OFF_INIT + i * C_SIZE_INT;
        MCopyProtectedMemory( &Zero, (void*)offset, C_SIZE_INT, PM_COPY_READ);
        if( Zero != i)break;
    }
    if( i < 5 )//˵���ǵ�һ�γ�ʼ��
    {
        for( i = 0; i < 5; i++)
        {
            offset = C_OFF_INIT + i * C_SIZE_INT;
            MCopyProtectedMemory( (void*)offset, &i,C_SIZE_INT, PM_COPY_WRITE);
        }
        offset = C_OFF_PWD;
    }
    else offset = C_OFF_STORE;


    //�������
    Zero = 0;
    offmax = C_OFF_FILEMAX;
    DisplayPrompt(17);//��ʾ�����������
    u_int time = GetElapsedTime();
    u_int elapsedtime = 400;
    xpos = 6* C_CCHAR_HDOT;
    MSetColor(C_CR_MENU);
    for( ; offset < offmax; offset += C_SIZE_INT)
    {
        MCopyProtectedMemory( (void*)offset, &Zero, C_SIZE_INT, PM_COPY_WRITE);	//д0
        if( time + elapsedtime < GetElapsedTime() )
        {
            Write_Ascii(xpos,7,'.');
            time = GetElapsedTime();
            xpos += 8;
        }
    }
    ExpendTime(100);
    Write_Ascii(xpos,7,'.');
    MSetChannel(0,0);




    ChannelParaInit(mode);	/*������ʼ����mode=0��ǰһ��ͨ����mode=1����ͨ��,2�����û�*/


    int testrange[3];
    testrange[1] = 1000;
    testrange[2] = 0;
    offset = C_OFF_TEMP + 0;
    MCopyProtectedMemory( (void*)offset, testrange, 12, PM_COPY_WRITE);
    testrange[0] = 500;
    testrange[1] = 300;
    offset = C_OFF_TEMP + 20;
    MCopyProtectedMemory( (void*)offset, testrange, 8, PM_COPY_WRITE);
    testrange[0] = 800;
    testrange[1] = 10;
    testrange[2] = 60;
    offset = C_OFF_TEMP + 30;
    MCopyProtectedMemory( (void*)offset, testrange, 12, PM_COPY_WRITE);

    offset = C_OFF_INNER_INFO;
    /*   INNER_INFO InnerInfo;
       MCopyProtectedMemory(&InnerInfo, (void*)offset, sizeof(INNER_INFO), PM_COPY_READ);
       InnerInfo.pzPurchaser[0] = 0;
      // InnerInfo.pzSerial[0] = 0;
       InnerInfo.MaintainTime.yearh = 0x20;
       InnerInfo.MaintainTime.yearl = 0x14;
       InnerInfo.MaintainTime.month = 1;
       InnerInfo.MaintainTime.date = 1;

       InnerInfo.LeaveFactoryTime.yearh = 0x20;
       InnerInfo.LeaveFactoryTime.yearl = 0x10;
       InnerInfo.LeaveFactoryTime.month = 0x10;
       InnerInfo.LeaveFactoryTime.date = 1;
       InnerInfo.StorageTime.yearh = 0x20;
       InnerInfo.StorageTime.yearl = 0x10;
       InnerInfo.StorageTime.month = 9;
       InnerInfo.StorageTime.date = 1;*/
    MCopyProtectedMemory((void*)offset, &InnerInfo, sizeof(INNER_INFO), PM_COPY_WRITE);

    USER_INFO UserInfo;

    UserInfo.pzPurchaser[0] = 0;
    UserInfo.pzSerial[0] = UserInfo.pzUser[0] = UserInfo.pzGovernor[0] = 0;
    UserInfo.PurchaseTime.yearh = 0x20;
    UserInfo.PurchaseTime.yearl = 0x10;
    UserInfo.PurchaseTime.month = 10;
    UserInfo.PurchaseTime.date = 1;
    offset = C_OFF_USER_INFO;
    MCopyProtectedMemory((void*)offset, &UserInfo, sizeof(USER_INFO), PM_COPY_WRITE);
    language=0;	//��9��ʼ���ָ�����
    Unit = 0;

    //ChannelParaRestore();
    MSetSurfGain(0,2);

    MPopWindow() ;

    MSetColor( C_CR_MENU);
    MSetColor( C_CR_WAVE);
    MSetLanguage(language);
    MSetUnitType(Unit);

}



int STDMenu(void)//��׼�˵�
{
    u_int char_len = 6,crow_height = 48,erow_height = 30;//row_number = 10,
    int number,deci_len,sign;
    int xpos,ypos;
    //int menu_xpos = 0,menu_ypos = 0;
    int para_xpos = C_CCHAR_HDOT * ( char_len + 1) ,para_ypos =  4;
    int retvalue = C_TRUE;
    int keycode;
    //int page_no = 1,page_max = 3;
    int row;
    u_short* cpText = 0;
    u_char* epText = 0;
    int i,j,k;
    int len,line;
    u_int elapsedtime1 = GetElapsedTime() ;
    int basegain1=MGetBaseGain();
    int basegain2;
#if C_DEVTYPE == 20
    return 0;
#endif
    if( MGetTestStatus(C_TEST_AVG) == 1)return 0;//����AVG����ʾ��׼�˵�
    if( MGetParaLock(1,C_LOCK_DACAVG) == 1)return 0;	//��������
    MSetAcquisitionEnable(0,C_SETMODE_SETSAVE);
    MSetHardEchoShow(0,C_SETMODE_SETSAVE);
    ClearEnvelope();
    SystemParaStore();
    ChannelParaStore();

    DisplayPrompt(15);
    while(true)
    {
        MFclearScreen();
        para_ypos =  4;
        //	cpText =  (u_short*)_STDMenu_A1;
        //epText =  (u_char*)_STDMenu_B1;

        MSetColor(C_CR_MENU);
        if(MGetStdMode() == 10||MGetStdMode() == 11)//if(MGetStdMode() >= C_STD_11345_2013_NUM -1)
        {
            if(MGetBlock() > 1)
            {
                for (i=0; i<MGetCurveNumber()+4; i++)
                {
                    TextOut(C_ECHAR_HDOT,para_ypos+crow_height*i, 1, 13+MGetLanguage()*2,16, (u_char*)_STDMenu_A2[MGetLanguage()][i], 4);
                }
            }
            else if(MGetBlock() == 1)
            {
                if(MGetThick()/10 <15)
                {
                    for (i=0; i<MGetCurveNumber()+4 - 1; i++)
                    {
                        TextOut(C_ECHAR_HDOT,para_ypos+crow_height*i, 1, 13+MGetLanguage()*2,16, (u_char*)_STDMenu_A3[MGetLanguage()][i], 4);
                    }
                }
                else
                {
                    for (i=0; i<MGetCurveNumber()+4; i++)
                    {
                        TextOut(C_ECHAR_HDOT,para_ypos+crow_height*i, 1, 13+MGetLanguage()*2,16, (u_char*)_STDMenu_A4[MGetLanguage()][i], 4);
                    }
                }
            }
            else if(MGetBlock() == 0)
            {
                for (i=0; i<MGetCurveNumber()+4; i++)
                {
                    TextOut(C_ECHAR_HDOT,para_ypos+crow_height*i, 1, 13+MGetLanguage()*2,16, (u_char*)_STDMenu_A5[MGetLanguage()][i], 4);
                }
            }
        }
        else
        {
            for (i=0; i<MGetCurveNumber()+4; i++)
            {
                TextOut(C_ECHAR_HDOT,para_ypos+crow_height*i, 1, 13+MGetLanguage()*2,16, (u_char*)_STDMenu_A1[MGetLanguage()][i], 4);
            }
        }
        if(MGetStdMode()==1 || MGetStdMode()==8|| MGetStdMode()==3 )
        {
            MEraseWindow(C_ECHAR_HDOT,para_ypos+crow_height*2,C_ECHAR_HDOT+C_CCHAR_HDOT*6,para_ypos+crow_height*2+C_CCHAR_VDOT);
            TextOut(C_ECHAR_HDOT,para_ypos+crow_height*2, 1, 13+MGetLanguage()*2,16, (u_char*)_STDMenu_A1[MGetLanguage()][10], 4);
        }
        //  if(MGetLanguage())
        {
            //      TextOut(C_ECHAR_HDOT,para_ypos+11, 1, 8,10, (u_char*)_STDMenu_A1[MGetLanguage()][0]+13, 4);
            //      TextOut(C_ECHAR_HDOT,para_ypos+crow_height+11, 1, 8,10, (u_char*)_STDMenu_A1[MGetLanguage()][1]+13, 4);
        }
        //	CEMenuOut(C_ECHAR_HDOT,para_ypos, cpText ,char_len,MGetCurveNumber()+4,crow_height);	/*��ָ��λ�ø���ÿ���ַ������������и�д�˵�*/
        //EMenuOut(para_xpos+C_ECHAR_HDOT*5,para_ypos + 8, epText ,3,row_number,crow_height);	/*��ָ��λ�ø���ÿ���ַ������������и�д�˵�*/

        EraseDrawRectangle(26*C_ECHAR_HDOT, 380, 26*C_ECHAR_HDOT + (5+1+1*MGetLanguage())*C_CCHAR_HDOT, 380 + 2 * (C_CCHAR_VDOT+12)) ;
        //	CMenuOut(27*C_ECHAR_HDOT,190+4,_MenuPrompt_B1,5,2,24);	/*��ָ��λ�ø���ÿ���ַ������������и�д�˵�*/
        TextOut(27*C_ECHAR_HDOT,380+4,1,10,24,(u_char*)_MenuPrompt_B1[MGetLanguage()][0],4);
        TextOut(27*C_ECHAR_HDOT,380+4+36,1,10,24,(u_char*)_MenuPrompt_B1[MGetLanguage()][0]+10,4);
//        EMenuOut(27*C_ECHAR_HDOT,380+4,_TestMenu_Enter,1,1,C_ECHAR_VDOT);
        if(MGetLanguage())
        {
            para_ypos+= C_ECHAR_VDOT;
        }

        MSetColor(C_CR_PARA);
        /*��׼���� */
        row = 0;
        xpos = para_xpos;
        ypos = para_ypos + crow_height *row ;
        epText = (u_char*)_STDMenu_AB1 + ( MGetStdMode()*C_STD_LEN+2 );
        EMenuOut(xpos,ypos , epText ,C_STD_LEN-2,1,crow_height);	/*��ָ��λ�ø���ÿ���ַ������������и�д�˵�*/

        //������
        row = 1;
        ypos = para_ypos + crow_height *row ;
        if((MGetStdMode() == 10||MGetStdMode() == 11)&&(MGetBlock() == 1)&&(MGetThick()/10 <15))
        {
            Write_Number(xpos,ypos,MGetCurveNumber()-1,2,0,0);
        }
        else
            Write_Number(xpos,ypos,MGetCurveNumber(),2,0,0);

        //�Կ�����
        row = 2;
        ypos = para_ypos + crow_height *row ;
        epText = (u_char*)_STDMenu_B1[MGetStdMode()]+1 + ( MGetBlock()*C_BLOCK_LEN+2 ) ;
        EMenuOut(xpos, ypos, epText ,C_BLOCK_LEN-2, 1, erow_height);	/*��ָ��λ�ø���ÿ���ַ������������и�д�˵�*/

        //�������
        row = 3;
        ypos = para_ypos + crow_height *row ;
        WriteLongness(xpos,ypos,MGetThick(),5,1);
        ///Write_Number(xpos,ypos,MGetThick(),4,1,0);

        //��1һ6����
        if((MGetStdMode() == 10||MGetStdMode() == 11) &&(MGetBlock() == 1)&&(MGetThick()/10 <15))
        {
            for( i = 0; i < (MGetCurveNumber()-1); i++)
            {
                row++;
                xpos = para_xpos;
                ypos = para_ypos + crow_height *row ;
                Write_Number(xpos,ypos,MGetLineGain(i),5,1,1);
            }
        }
        else
        {
            for( i = 0; i < MGetCurveNumber(); i++)
            {
                row++;
                xpos = para_xpos;
                ypos = para_ypos + crow_height *row ;
                Write_Number(xpos,ypos,MGetLineGain(i),5,1,1);
            }
        }
        MKeyRlx();		/*ֻ�а������ͷŲ��˳�*/
        do
        {
            if( GetElapsedTime() > elapsedtime1 + 200 )
            {
                InvertWords(27*C_ECHAR_HDOT,380+4, 1); /*������ʾ�����ֵ�����*/
                elapsedtime1 = GetElapsedTime() ;
            }
            keycode = MGetKeyCode(10);
            if( keycode >= 0 && keycode <= MGetCurveNumber() + 4)break;
        }
        while( keycode != C_KEYCOD_RETURN && keycode != C_KEYCOD_CONFIRM );

        if (keycode == C_KEYCOD_RETURN)
        {
#if C_ACQU == 1
            MSetAcquisitionEnable(MGetAcquisitionEnable(),C_SETMODE_SETSAVE);
#endif
            SystemParaRestore();
            ChannelParaRestore();

            retvalue = C_FALSE;
            break;
        }
        else if (keycode == C_KEYCOD_CONFIRM)
        {
#if C_ACQU == 1
            MSetAcquisitionEnable(MGetAcquisitionEnable(),C_SETMODE_SETSAVE);
#endif

            retvalue = C_TRUE;
            break;
        }

        if(keycode == 0)keycode = 10;
        keycode--;//��1��10��Ϊ0��9
        row = keycode ;
        xpos = para_xpos;
        ypos = para_ypos + crow_height * row-1;

        InvertWords(C_ECHAR_HDOT,ypos-MGetLanguage()*C_ECHAR_VDOT, 1); /*������ʾ�����ֵ�����*/

        switch (keycode)
        {
        case 0:		//��׼
            len = C_STD_LEN;
            row = 9;
            short pagemax1=0;
            while(1)
            {
                MPushWindow(xpos, ypos, xpos+ (len+2)*C_ECHAR_HDOT, ypos + (row+2) * erow_height) ;
                EraseDrawRectangle(xpos, ypos, xpos+ (len+2)*C_ECHAR_HDOT, ypos + (row+2) * erow_height) ;
                if(pagemax1==0)
                    EMenuOut(xpos + C_ECHAR_HDOT,ypos+4,_STDMenu_AB1,len,row,erow_height);	/*��ָ��λ�ø���ÿ���ַ������������и�д�˵�*/
                else
                    EMenuOut(xpos + C_ECHAR_HDOT,ypos+4,_STDMenu_AB1+9*C_STD_LEN,C_STD_LEN,4,erow_height);


                Write_Number(xpos + 11* C_ECHAR_HDOT, ypos+10*erow_height+6,pagemax1+1,1,0,0);
                TextOut(xpos+12*C_ECHAR_HDOT,ypos+10*erow_height+4,1,3,24,"/2",4);


                while(1)
                {
                    keycode = MAnyKeyReturn();
                    if((keycode>=1&&keycode<=9&&pagemax1==0)||(keycode>=1&&keycode<=4&&pagemax1==1)||keycode==C_KEYCOD_PLUS||keycode==C_KEYCOD_MINUS||keycode==C_KEYCOD_CONFIRM||keycode==C_KEYCOD_RETURN)
                        break;

                }
                if(keycode==C_KEYCOD_PLUS)
                {

                    pagemax1++;
                    if(pagemax1>1) pagemax1=0;
                    continue;

                }

                if(keycode==C_KEYCOD_MINUS)
                {

                    pagemax1--;
                    if(pagemax1<0) pagemax1=1;
                    continue;

                }
                if(keycode>=1&&keycode<=9&&pagemax1==0)
                {

                    MSetStdMode(keycode-1,C_SETMODE_SAVE);
                    if (MGetStdMode() == 1)
                    {
                        MSetBlock(1);
                    }
                    else
                    {
                        MSetBlock(0);
                    }

                    MSetDacMode(2,0);

                    MSetCurveNumber( c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_LINENUM] );

                    k = c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_THNUM];//�Կ�����
                    number = MGetThick()/10;
                    if( MGetUnitType() > 0)number = (UINT)(number * 254/1000.0+0.5);//��λinch
                    for( i = 0 ; i < k; i++)
                    {
                        if(MGetStdMode()==0)break;
                        if( number >c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_THICK+7*i] )continue;
                        for( j = 0; j < MGetCurveNumber(); j++)
                        {
                            MSetLineGain(j, c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_THICK+7*i+j+1] );
                        }
                        break;
                    }


                    if(MGetStdMode()==2)
                    {

                        int testrange[3];
                        int offset;
                        testrange[0] = 500;
                        testrange[1] = 10;
                        testrange[2] = 60;
                        offset = C_OFF_TEMP + 30;
                        MCopyProtectedMemory( (void*)offset, testrange, 12, PM_COPY_WRITE);
                    }

                    break;
                }
                if(keycode>=1&&keycode<=4&&pagemax1==1)
                {

                    MSetStdMode(keycode-1+9,C_SETMODE_SAVE);

                    MSetBlock(0);

                    if(MGetStdMode()==10||MGetStdMode()==11)
                        MSetDacMode(0,0);
                    else
                        MSetDacMode(2,0);

                    MSetCurveNumber( c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_LINENUM] );

                    k = c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_THNUM];//�Կ�����
                    number = MGetThick()/10;
                    if( MGetUnitType() > 0)number = (UINT)(number * 254/1000.0+0.5);//��λinch
                    for( i = 0 ; i < k; i++)
                    {
                        if(MGetStdMode()==10||MGetStdMode()==11)
                        {
                            if( number >=c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_THICK+7*i] )continue;
                            for( j = 0; j < MGetCurveNumber(); j++)
                            {
                                MSetLineGain(j, c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_THICK+7*i+j+1] );
                            }
                            break;
                        }
                        else
                        {
                            if( number >c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_THICK+7*i] )continue;
                            for( j = 0; j < MGetCurveNumber(); j++)
                            {
                                MSetLineGain(j, c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_THICK+7*i+j+1] );
                            }
                            break;
                        }

                    }
                    if(MGetStdMode()==12)//47013
                    {
                        int testrange[3];
                        int offset;
                        testrange[0] = 800;
                        testrange[1] = 20;
                        testrange[2] = 400;
                        offset = C_OFF_TEMP + 30;
                        MCopyProtectedMemory( (void*)offset, testrange, 12, PM_COPY_WRITE);
                    }
                    break;
                }
                if(keycode==C_KEYCOD_CONFIRM||keycode==C_KEYCOD_RETURN)
                    break;
            }
            MPopWindow();
            break;
        case 1:	//������
            deci_len = 0;
            number = 100;
            while(1)
            {
                if( Input_Number(xpos,ypos-C_ECHAR_VDOT,&number,2, &deci_len,0) != 1)break;
                else
                {
                    if( number > 0 && number <= 6)
                    {
                        MSetCurveNumber(number);

                        break;
                    }
                    else	DisplayPrompt(4);
                }
            }
            break;
        case 2:	//�Կ�����
            //break;
            row =  _STDMenu_B1[MGetStdMode()][0]- '0' ;


            len = C_BLOCK_LEN;
            epText = (u_char*)( _STDMenu_B1[MGetStdMode()]+1 );
            MPushWindow(xpos, ypos, xpos+ 11*C_ECHAR_HDOT, ypos + (row+1) * erow_height) ;
            EraseDrawRectangle(xpos, ypos, xpos+ (len+2)*C_ECHAR_HDOT, ypos + (row+1) * erow_height) ;
            EMenuOut(xpos + C_ECHAR_HDOT, ypos+4, epText ,len, row, erow_height);	/*��ָ��λ�ø���ÿ���ַ������������и�д�˵�*/
            while(1)
            {
                keycode = MenuKeyReturn(row,2);	/*���ֲ˵�ʱ����������keymax�ļ����أ�mode=0��������Ч��=1ȷ�ϼ���Ч��=2���ؼ���Ч��=3ȷ�Ϸ��ؼ���Ч*/
                if (keycode == C_KEYCOD_RETURN||keycode == C_KEYCOD_CONFIRM)
                    break;
                else if(keycode >= 1 && keycode <= row)
                {
                    MSetBlock(keycode-1);
                    MSetCurveNumber( c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_LINENUM] );
                    k = c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_THNUM];//�Կ�����
                    number = MGetThick()/10;
                    if( MGetUnitType() > 0)number = (UINT)(number * 254/1000.0+0.5);//��λinch
                    for( i = 0 ; i < k; i++)
                    {
                        if(MGetStdMode()==0)break;

                        if(MGetStdMode()==10||MGetStdMode()==11)
                        {
                            if( number >=c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_THICK+7*i] )continue;

                        }
                        else
                        {
                            if( number >c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_THICK+7*i] )continue;

                        }

                        for( j = 0; j < MGetCurveNumber(); j++)
                        {
                            MSetLineGain(j, c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_THICK+7*i+j+1] );
                        }
                        break;
                    }


                    if(MGetBlock()==1&&MGetStdMode()==12)
                    {
                        int testrange[3];
                        int offset;
                        testrange[0] = 800;
                        testrange[1] = 10;
                        testrange[2] = 60;
                        offset = C_OFF_TEMP + 30;
                        MCopyProtectedMemory( (void*)offset, testrange, 12, PM_COPY_WRITE);
                    }
                    else if(MGetBlock()==0&&MGetStdMode()==12)
                    {

                        int testrange[3];
                        int offset;
                        testrange[0] = 800;
                        testrange[1] = 20;
                        testrange[2] = 400;
                        offset = C_OFF_TEMP + 30;
                        MCopyProtectedMemory( (void*)offset, testrange, 12, PM_COPY_WRITE);
                    }
                    else if(MGetStdMode()==2)
                    {

                        int testrange[3];
                        int offset;
                        testrange[0] = 500;
                        testrange[1] = 10;
                        testrange[2] = 60;
                        offset = C_OFF_TEMP + 30;
                        MCopyProtectedMemory( (void*)offset, testrange, 12, PM_COPY_WRITE);
                    }
                    if(MGetStdMode()==12)//�жϺ���Ƿ��ڷ�Χ��
                    {

                        if( (MGetBlock()==0&&(MGetThick()<60||MGetThick()>2000))\
                                ||(MGetBlock()==1&&(MGetThick()<80||MGetThick()>1200))\
                                ||(MGetBlock()==2&&(MGetThick()<=2000||MGetThick()>5000))\
                                ||(MGetBlock()==3&&MGetThick()<40)    )

                        {
                            for( j = 0; j < MGetCurveNumber(); j++)
                            {
                                MSetLineGain(j, 0 );
                            }
                        }

                    }

                    else if(MGetStdMode()==2)//�жϺ���Ƿ��ڷ�Χ��
                    {
                        if( (MGetBlock()==0&&(MGetThick()<80||MGetThick()>1200))\
                                ||(MGetBlock()==1&&(MGetThick()<60||MGetThick()>1200))\
                                ||(MGetBlock()==2&&(MGetThick()<=1200||MGetThick()>4000))\
                                ||(MGetBlock()==3&&MGetThick()<40) )
                        {
                            for( j = 0; j < MGetCurveNumber(); j++)
                            {
                                MSetLineGain(j, 0 );
                            }
                        }
                    }
                    else if(MGetStdMode()==4)//�жϺ���Ƿ��ڷ�Χ��
                    {
                        if( MGetThick()<50)
                        {
                            for( j = 0; j < MGetCurveNumber(); j++)
                            {
                                MSetLineGain(j, 0 );
                            }
                        }
                    }

                    break;
                }
            }
            MPopWindow();
            break;
        case 3:	//�������
            //break;
            if( MGetUnitType() > 0)deci_len = 3;//��λinch
            else deci_len = 1;
            number = 0;
            while(1)
            {
                if( Input_Number(xpos,ypos-C_ECHAR_VDOT,&number,4,&deci_len,0) != 1)break;

                else
                {
                    if(( number < 50000 && MGetUnitType()==0)||( number < (50000/0.254+0.5) && MGetUnitType()==1))
                    {
                        MSetThick(number);
                        number /= 10;
                        if( MGetUnitType() > 0)number = (UINT)(number * 254/1000.0+0.5);//��λinch
                        k = c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_THNUM];
                        for( i = 0 ; i < k; i++)
                        {
                            if(MGetStdMode()==0)break;
                            if(MGetStdMode()==10||MGetStdMode()==11)
                            {
                                if( number >=c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_THICK+7*i] )continue;
                            }
                            else
                            {
                                if( number >c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_THICK+7*i] )continue;

                            }
                            for( j = 0; j < MGetCurveNumber(); j++)
                            {
                                MSetLineGain(j, c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_THICK+7*i+j+1] );
                            }
                            break;
                        }
                    }
                    if(MGetStdMode()==12)//�жϺ���Ƿ��ڷ�Χ��
                    {

                        if( (MGetBlock()==0&&(MGetThick()<60||MGetThick()>2000))\
                                ||(MGetBlock()==1&&(MGetThick()<80||MGetThick()>1200))\
                                ||(MGetBlock()==2&&(MGetThick()<=2000||MGetThick()>5000))\
                                ||(MGetBlock()==3&&MGetThick()<40) )

                        {
                            for( j = 0; j < MGetCurveNumber(); j++)
                            {
                                MSetLineGain(j, 0 );
                            }
                        }

                    }

                    else if(MGetStdMode()==2)//�жϺ���Ƿ��ڷ�Χ��
                    {
                        if( (MGetBlock()==0&&(MGetThick()<80||MGetThick()>1200))\
                                ||(MGetBlock()==1&&(MGetThick()<60||MGetThick()>1200))\
                                ||(MGetBlock()==2&&(MGetThick()<=1200||MGetThick()>4000))\
                                || (MGetBlock()==3&&MGetThick()<40)   )
                        {
                            for( j = 0; j < MGetCurveNumber(); j++)
                            {
                                MSetLineGain(j, 0 );
                            }
                        }
                    }
                    else if(MGetStdMode()==4)//�жϺ���Ƿ��ڷ�Χ��
                    {
                        if( MGetThick()<50)
                        {
                            for( j = 0; j < MGetCurveNumber(); j++)
                            {
                                MSetLineGain(j, 0 );
                            }
                        }
                    }
                    break;
                }
            }
            break;
        case 4:	//��һ����
        case 5:	//��2����
        case 6:
        case 7:
        case 8:
        case 9:
            deci_len = 1;
            number = 0;
            if(MGetTestStatus(C_TEST_AVG) == 1)sign = 0;
            else sign = 1;
            line = keycode - 4;
            if( MGetCurveNumber() < line)break;
            while( 1 )
            {
                if( Input_Number(xpos,ypos-C_ECHAR_VDOT,&number,3, &deci_len,sign) < 1)break;
                else
                {
                    if(MGetTestStatus(C_TEST_AVG) == 1)
                    {
                        if( number >= 5 && number < 300)
                            number = (int)( 400 * log10( number/10.0) -120);
                        else break;
                    }
                    else if(number < -300 || number > 400)break;

                    MSetLineGain(line,number);
                    break;
                }
            }
            break;
        }
    }
    MSetColor(C_CR_UNDO);
    ScreenRenovate();	/*��Ļˢ��*/
    MSetAcquisitionEnable(1,C_SETMODE_SETSAVE);
    MSetHardEchoShow(1,C_SETMODE_SETSAVE);
    ClearEnvelope();	//�������ֵ
    if( retvalue == C_TRUE && MGetTestStatus(C_TEST_DAC) == 1)
    {
        MSetDelay(0,C_SETMODE_SETSAVE);
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
        DrawDac(2);		//�»�DAC
        DrawDac(0);
        if( MGetThick()*2 <= MGetRange(4) && MGetStdMode()!=11 )//if( MGetThick()*2 <= MGetRange(4) && MGetStdMode()<C_STD_MAX-1 )
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
            else if (MGetStdMode()==12)	//47013
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

        MSetSystem();
        MSetColor(C_CR_PARA);
        WriteBaseGain(C_BASEGAIN_HPOSI,C_BASEGAIN_VPOSI);
        WriteCompGain(C_COMPGAIN_HPOSI,C_COMPGAIN_VPOSI);
        WriteSurfGain(C_SURFGAIN_HPOSI,C_SURFGAIN_VPOSI);
        //  WriteSurfGain(C_BASEGAIN_HPOSI + 9 * C_ECHAR_HDOT,C_BASEGAIN_VPOSI);
        SystemParaStore();
        ChannelParaStore();
        ScreenRenovate();
    }
    DrawDac(0);		//�»�DAC

    return 1;
}


int DataCall(int file_no,int SaveMode)	//�����Ѵ洢�����ݲ����ǵ���ǰͨ��
{
    //��ʾ��ʾ:ͨ��X�����ݽ�������,Y/N?

    MEraseWindow(0,0,C_CCHAR_HDOT*12,25);
    if(CMessageBox( 0,0,(char *) _DataMenu_E1[MGetLanguage()][0],22,MB_YESNO) == MB_YES)
    {
        MSetChannel(MGetChannel(),C_SETMODE_SETSAVE);
        //	MSetSaveMode(0,0);
        MSetSaveMode(SaveMode,C_SETMODE_SETSAVE);
        MSetSaveFrequency(0,0);
        ChannelParaStore();
        SystemParaStore();	//���洢ͨ�����ݺ�ϵͳ���ݵ���洢��
        MSetSystem();
        return 1;

    }
    else return 0;
}
int TestFlawHigh(void)		//���
{
    u_short xpos,ypos,menu_xpos,menu_ypos;
    short keycode;
    int high = 0, high1=0,high2=0,i;
    char num, dac_num = Dac.num;
    u_char len;//,row;
    u_char *samp;
    u_char sampbuff[C_LEN_SAMP+1] ;


    AllParaStore();
    Dac.num = 0;

    MSetFunctionMode(0, C_DAC_GATE);
    MSetTestStatus(C_TEST_ALL,0);	//���ò���״̬,DAC��
    MSetAcquisition(1);
    ScreenRenovate();
    len = 11;
    menu_xpos = 10;
    menu_ypos = C_COORVPOSI+2;
    MSetGatePara(C_COORHORIUNIT * 6,C_COORHORIUNIT * 3,0,1,C_SETMODE_SETSAVE);
    MSetBGateMode(0,C_SETMODE_SETSAVE);


    for( keycode = 0 ; keycode < C_LEN_SAMP+1; keycode++)sampbuff[keycode] = 0;
    num = 0;
    MSetDepth(0,0);
    while(true)
    {
        if( num == 0)
        {
            MSetGatePara(C_COORHORIUNIT * 4,C_COORHORIUNIT * 5,120,0,C_SETMODE_SETSAVE);
        }
        else
        {
            MSetGatePara(C_COORHORIUNIT * 2,C_COORHORIUNIT * 7,120,0,C_SETMODE_SETSAVE);
        }
        MChannelRenovate();
        DisplayPrompt(15);
        //	CEMenuOut(0 , 0,  _TestMenu_AG1 ,5,1,24);	//д����ȱ�ݸ߶�
        TextOut(0 , 0,1,16,32,(char *)_TestMenu_AG1[MGetLanguage()][0],4);
        EraseDrawRectangle(menu_xpos, menu_ypos, menu_xpos + (len+1+5*MGetLanguage())*C_CCHAR_HDOT, menu_ypos + 2 * 32+6+8*MGetLanguage()) ;
        if( num == 0)
        {
            //	CEMenuOut( menu_xpos+8, menu_ypos+4,  _TestMenu_AG2 ,len,2,24);	//д����ȱ���¶�
            TextOut(menu_xpos+7, menu_ypos+4,2+MGetLanguage(),23+MGetLanguage(),32-10*MGetLanguage(),(char *)_TestMenu_AG2[MGetLanguage()][0],4-3*MGetLanguage());

        }
        else
        {
            //	CEMenuOut( menu_xpos+8, menu_ypos+4,  _TestMenu_AG3 ,len,2,24);	//д����ȱ���϶�
            TextOut(menu_xpos+7, menu_ypos+4,2+MGetLanguage(),23+MGetLanguage(),32-10*MGetLanguage(),(char *)_TestMenu_AG2[MGetLanguage()][1],4-3*MGetLanguage());
        }
        DrawWave(sampbuff,0,C_LEN_SAMP);
        while(true)
        {


            if (!MGetFunctionMode(C_ECHOMAX_MEMORY))
            {
                if(MAdjustGain(0,1,MGetAmpStdMax()/2,MGetAmpStdMax()))
                {
                    MSetGateParaInit();	//������߲����ֵ
                    //	g_UDiskInfo.DataHeaderMark = 1;
                }
            }
            keycode = MGetKeyCode(0);
            //	StorageData(keycode);
            MParaRenovate(1);
            // gateamp = MGetAmpMax(gatetype);
            if(!MGetFunctionMode(C_ECHOMAX_MEMORY))
            {
                if(MGetGateParaMax(0) == C_TRUE)
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
                ypos = C_COORVPOSI + C_COORHEIGHT - 2 - (GateMax.Amp*2) ;
            }
            /*          if (MAdjustGain(0,0,MGetAmpStdMax()/2,MGetAmpStdMax()))
                      {
                          GateMax.Amp = 0;	//�����Զ����ڹ�
                      }
                      MParaRenovate(1);
                      keycode = MGetKeyCode(0);//Code(0);
                      if ( MGetGateParaMax(0) == C_TRUE )
                      {
                          ClearCursor(2);
                          xpos = GateMax.Pos;
                          ypos = C_COORVPOSI + C_COORHEIGHT - 2 - GateMax.Amp*2;
                          if((GateMax.Amp*2) < C_COORHEIGHT)DrawCursor(xpos,ypos,2);
                      }
            */
            if( keycode == C_KEYCOD_CONFIRM)
            {
                short left,right;
                left = right = MGetAmpPos(0);
                MSetAcquisitionEnable(0,C_SETMODE_SET);
                samp = GetSampleBuffer();
                while(samp[left - 1] <= samp[left] && samp[left] > 0) left-- ;
                while(samp[right] >= samp[right+1] && samp[right] > 0) right++ ;

                for( keycode = left-1; keycode <= right+1; keycode++)
                {
                    sampbuff[keycode] = samp[keycode];
                }
                MSetAcquisitionEnable(1,C_SETMODE_SET);

                num++;
                if( num == 1)
                {
                    high = MGetAmpTime(0) ;
                    high1 = (int)( (double)(high * MGetSpeed() )/(double)(C_SAMPLE_FREQ) +0.5) ;	//����
                    high1 = high1 * MGetAngle(3) / 10000;	//��ֱ
                }
                else if( num == 2)
                {
                    high2 = MGetAmpTime(0);
                    if( high > high2 )	high -= high2;
                    else high = high2 - high;
                    high2 = (int)( (double)(high2 * MGetSpeed() )/(double)(C_SAMPLE_FREQ) +0.5) ;	//����
                    high2 = high2 * MGetAngle(3) / 10000;	//��ֱ
                }
                break;
            }
            else if( keycode == C_KEYCOD_RETURN)
            {
                if( DisplayQuery(4) != 1)continue;
                num = 10;//������ʾ���·��ؼ�
                break;
            }
            //else if( num == 0 && (keycode == C_KEYCOD_RANGE || keycode == C_KEYCOD_BASEGAIN) )
            else if(  (keycode == C_KEYCOD_RANGE || keycode == C_KEYCOD_BASEGAIN || keycode == C_KEYCOD_GATE ) )
            {
                KeyManage(keycode,1);
                DisplayPrompt(15);
                //			CEMenuOut(0 , 0,  _TestMenu_AG1 ,5,1,24);	//д����ȱ�ݸ߶�
                TextOut(0 , 0,1,16,32,(char *)_TestMenu_AG1[MGetLanguage()][0],4);
            }
#if C_DEVTYPE == 15 || C_DEVTYPE == 1
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
                        MSetFunctionMode(0,C_ECHOMAX_MEMORY);

                    }
                    MKeyRlx();
                    keycode =-1;
                    KeyManage(keycode,1);
                    DisplayPrompt(15);
                    TextOut(0 , 0,1,16,32,(char *)_TestMenu_AG1[MGetLanguage()][0],4);
                    //	CEMenuOut(0 , 0,  _TestMenu_AG1 ,5,1,24);	//д����ȱ�ݸ߶�

                }
                else if(Function.funcMenu[i]==C_ECHOMAX_MEMORY+1)
                {
                    ClearEnvelope();
                    if( MGetFunctionMode(C_ECHOMAX_MEMORY) == 1)
                    {
                        MSetFunctionMode(0,C_ECHOMAX_MEMORY);
                    }
                    else if( MGetFunctionMode(C_ECHOMAX_MEMORY) == 0)
                    {
                        MSetFunctionMode(1,C_ECHOMAX_MEMORY);
                        MSetFunctionMode(0,C_ECHO_ENVELOPE);
                        MSetSaveFrequency(0,0);
                    }
                    MKeyRlx();
                    keycode =-1;
                    KeyManage(keycode,1);
                    DisplayPrompt(15);
                    TextOut(0 , 0,1,16,32,(char *)_TestMenu_AG1[MGetLanguage()][0],4);
                    //	CEMenuOut(0 , 0,  _TestMenu_AG1 ,5,1,24);	//д����ȱ�ݸ߶�

                }
            }
#endif
            //else if( keycode == C_KEYCOD_SPEED || keycode == C_KEYCOD_OFFSET || keycode == C_KEYCOD_ANGLE);
            /*
            			else if( keycode == C_KEYCOD_GATE)
            			{
            				KeyManage(keycode,1);
            				DisplayPrompt(15);
            				CEMenuOut(0 , 0,  _TestMenu_AG1 ,5,1,24);	//д����ȱ�ݸ߶�
            			}
            //*/
            //MKeyRlx();
        }
        MKeyRlx();

        if( num == 2)
        {
            //��ȷ�����Σ���ʾ�߶�ֵ
            MChannelRenovate();
            EraseDrawRectangle(menu_xpos, menu_ypos, menu_xpos + (len+2+3*MGetLanguage())*C_CCHAR_HDOT+8, menu_ypos + 2 * 32+6) ;
            TextOut(menu_xpos+7, menu_ypos+4,2,23,32,(char *)_TestMenu_AG2[MGetLanguage()][2],4-3*MGetLanguage());
//			EraseDrawRectangle(menu_xpos, menu_ypos, menu_xpos + (len+1)*C_CCHAR_HDOT, menu_ypos + 2 * 24) ;
//			CEMenuOut( menu_xpos+8, menu_ypos+4,  _TestMenu_AG4 ,len,2,24);	//д����ȱ�ݸ߶�
            if( MGetUnitType() > 0)EMenuOut(menu_xpos + 16 + (len-1) * C_CCHAR_HDOT, menu_ypos+4,"in",2,1,8);
            else EMenuOut(menu_xpos +16+ (len-1+3*MGetLanguage()) * C_CCHAR_HDOT, menu_ypos+4,"mm",2,1,8);

            DrawWave(sampbuff,0,C_LEN_SAMP);
            MSetAcquisition(0);
            high = (int)( (double)(high * MGetSpeed() )/(double)(C_SAMPLE_FREQ) +0.5)* MGetAngle(3) / 10000 ;
            Write_Number( menu_xpos+16 + (7+3*MGetLanguage())*C_CCHAR_HDOT,menu_ypos+4, high,4,1,0);
            MKeyRlx();
            while(true)
            {
                keycode = MGetKeyCode(0);
                if( keycode == C_KEYCOD_YES)
                {
                    //�洢
                    int offset;
                    MChannelRenovate();
                    MSetDepth(high1 - high2,0);


                    DrawWave(sampbuff,0,C_LEN_SAMP);
                    offset = C_OFF_PEAK;
                    MCopyProtectedMemory( (void*)offset, &GatePeak, C_LEN_PEAK, PM_COPY_WRITE);//�Ų���
                    offset = C_OFF_SAMP;
                    MCopyProtectedMemory( (void*)offset, sampbuff, C_LEN_SAMP, PM_COPY_WRITE); //����Ϊ����
                    MSetSystemMode(0,0);		//���ó��Զ�����״̬
                    ChannelParaStore();
                    SystemParaStore();
                    DataSave();
                    break;
                }
                else if( keycode == C_KEYCOD_NO)break;
            }
            break;
        }
        if( num == 10)break;//����
    }

    Dac.num = dac_num;
    AllParaRestore();
    MSetDepth(0,0);
    MSetSystem();
    return 1;
}

/*
�˵�6dB��
��������̽ͷʹȱ�����
��߲��ڽ�������enȷ��

����̽ͷ����˵�ز���
Ϊ40��ʱ����ǲ�enȷ��


��������̽ͷʹȱ���Ҷ�
��߲��ڽ�������enȷ��

����̽ͷ���Ҷ˵�ز���
Ϊ40��ʱ����ǲ�enȷ��

������������Ǽ䳤����
�����룺    mm��enȷ��

6dB��
��������̽ͷʹȱ��
��߲��ڽ�������enȷ��


�ֱ�������̽ͷ���ز���
Ϊ40��ʱ����ǲ�enȷ��

������������Ǽ䳤����
�����룺    mm��enȷ��

*/
#define C_LEN_BUFF 32
#define C_BASE		0
#define C_WAVENUM	2
#define C_REJ	10

//Bɨ��
void BScan(void)
{
    int gain ,range;//, angle, speed;
    u_int amp,nextamp;
//	int gatetype = 0;
    u_int gateamp;
//	u_int char_len,row_number,crow_height;
    //int number,deci_len;
//	int xposPeak,yposPeak,
    int xpos,ypos,x0,y0;
    u_int preElapsedtime;
    int keycode;
    u_char* sampbuffer;
    int i, j, k,l;
    int tm ;
    u_char bug[C_LEN_BUFF][4][2];
    int ip,jp,kp,Unit;//,lp;
    int flag;//=0��ʾǰһ�����޵ײ��޻ز���1�еײ��޻ز���2�лز��޵ײ���3�лز��еײ�
    int preypos=0;
    int high=100;

    AllParaStore();
    MSetTestStatus(C_TEST_DAC,0);	//���ò���״̬,DAC��
    MSetTestStatus(C_TEST_AVG,0);	//���ò���״̬,AVG��
    MSetAcquisition(0);
    ScreenRenovate();
    MKeyRlx();
    //���ó�ֱ̽ͷ
    MSetProbeMode(0,C_SETMODE_SAVE);
    if( MGetUnitType() > 0)MSetSpeed(CD_SPEED_1*1000/254,C_SETMODE_SAVE);//��λinch
    else MSetSpeed(CD_SPEED_1,C_SETMODE_SAVE);
    if (MGetUnitType() > 0)
    {
        MSetCrystal_l(787,C_SETMODE_SETSAVE);
    }
    else
    {
        MSetCrystal_l(20000,C_SETMODE_SETSAVE);
    }

    MSetCrystal_w(0,C_SETMODE_SETSAVE);
    MSetAngle(0,C_SETMODE_SAVE);
    MSetForward(0,C_SETMODE_SAVE);
    if( DisplayQuery(1) == 1)
    {
        if( TestMenuOffset() == C_FALSE)
        {
            MSetAcquisition(1);
            AllParaRestore();
            MSetSystem();
			ScreenRenovate();
            DrawDac(0);
            return ;
        }
    }
	
    ScreenRenovate();
    tm = 32;//��ʱ�����и�
    xpos = ( C_HORIDOT_SCREEN - C_CCHAR_HDOT * 6 - C_ECHAR_HDOT * 15) / 8 * 8;
    ypos = C_COORVPOSI;
    EraseDrawRectangle(xpos, ypos, xpos+ (13+MGetLanguage()*3)*C_CCHAR_HDOT, ypos + 1 * tm+8) ;
    /* if (MGetLanguage())
     {
         EMenuOut(xpos + 2,ypos+4,"1.Thickness:",12,1,16);
     }
     else
     {
         CEMenuOut(xpos + C_ECHAR_HDOT,ypos+4,_BSCAN_B2,6,1,tm);
     }
     */
    TextOut(xpos + C_ECHAR_HDOT,ypos+4,1,12,32,(char *)_Bscan[MGetLanguage()][3],4);
    x0 = xpos + (7+MGetLanguage()*2) * C_CCHAR_HDOT;	/*д������λ��ˮƽ��������*/
    y0 = ypos + 4;
    if( MGetUnitType() > 0)
    {
        Unit = 3;//��λinch
        range = 3973;
    }
    else
    {
        Unit = 1;
        range = 1000;
    }

    while(true)
    {
        xpos = x0;
        ypos = y0 + tm *0 ;
        WriteLongness(xpos,ypos+1 ,range,5,1);
        //	Write_Number(xpos,ypos+C_ECHAR_VDOT+1 ,range,4,1,0);
        //	EMenuOut(xpos + 5 * C_ECHAR_VDOT,ypos+8,"mm",2,1,8);

        keycode = MenuKeyReturn(3,3);	/*���ֲ˵�ʱ����������keymax�ļ����أ�mode=0��������Ч��=1ȷ�ϼ���Ч��=2���ؼ���Ч��=3ȷ�Ϸ��ؼ���Ч*/
        ypos = y0 + tm * (keycode - 1);

        if (keycode == 1)
        {
            if ( Input_Number(xpos,ypos,&range,5, &Unit,0) != 1)
            {
                if(range < C_MAX_RANGE && MGetUnitType()==0)
                {
                    range = 1000;

                }
                else if( range < (C_MAX_RANGE/0.254+0.5) && MGetUnitType()==1)
                {
                    range = 3973;
                }
            }
            //	break;
        }
        else if( keycode == C_KEYCOD_RETURN )
        {
            //	range = 1000;
            break;
        }
        else if( keycode == C_KEYCOD_CONFIRM )
        {
            break;
        }
    }

    gain = MGetBaseGain();
//	Write_Number(10,50,range,10,0,0);
//	MAnyKeyReturn();
    range = range * 10/8;
//	Write_Number(10,60,range,10,0,0);
//	MAnyKeyReturn();
    MSetPara(gain,0,0,0,range,MGetOffset(),0,MGetAngle(0),MGetSpeed() );

    MSetFunctionMode(0,C_FUNCTION_ALL);
    MSetEchoMode(0,C_SETMODE_SETSAVE);
    MSetGatePara(C_COORHORIUNIT * 7,C_COORHORIUNIT * 2,0,0,C_SETMODE_SETSAVE);
    MSetBGateMode(0,C_SETMODE_SETSAVE);
    MSetGatePara(C_COORHORIUNIT * 7.5,C_COORHORIUNIT * 1,0,1,C_SETMODE_SETSAVE);

    MSetSystem();
 
    MSetAcquisition(1);
    ScreenRenovate();
    xpos = 16;
    ypos = C_COORVPOSI+1;
    EraseDrawRectangle(xpos, ypos, xpos+ (12+MGetLanguage()*4)*C_CCHAR_HDOT, ypos + 1 * 38) ;
//	CEMenuOut(xpos + 4,ypos+2,_BSCAN_B3,9,1,16);
    TextOut(xpos + 4,ypos+2,1,21,32,(char *)_Bscan[MGetLanguage()][1],2);

    MSetGatePara(C_COORHORIUNIT * 7.5,C_COORHORIUNIT * 1,140,0,C_SETMODE_SETSAVE);
    MDrawGate(-1,0,-1,0);

    while(true)
    {
        if(MAdjustGain(0,1,80,160))
        {
            MSetGateParaInit();	//������߲����ֵ
        }
        gateamp = MGetAmpMax(0);
        if(!MGetFunctionMode(C_ECHOMAX_MEMORY))
        {
            if(MGetGateParaMax(0) == C_TRUE)
            {
                ClearCursor(2);
                xpos = GateMax.Pos;
                ypos = C_COORVPOSI + C_COORHEIGHT - 2 - GateMax.Amp*2 ;
                if((GateMax.Amp*2) < C_COORHEIGHT)	DrawCursor(xpos,ypos,2);
            }

        }

        MParaRenovate(0);

        keycode = MGetKeyCode(0);
        if( keycode == C_KEYCOD_RETURN )
        {
            break;
        }
        else if( keycode == C_KEYCOD_CONFIRM )
        {
            ExpendTime(30);
            //	MAdjustGain(0,1,160,160 );
            //	MSetBaseGain( MGetBaseGain()+ 60 * range/500,C_SETMODE_SETSAVE);
            break;
        }
    }

    xpos = 0;
    ypos = 0;
    MSetColor(C_CR_MENU);
    MKeyRlx();

    //MAdjustGain(0,0,MGetAmpStdMax(),MGetAmpStdMax() );
    MFclearScreen();
    MSetColor(C_CR_PARA);
    MCoorDraw(C_COORHPOSI ,C_COORVPOSI , C_COORHEIGHT - 150 , C_COORWIDTH );	/*��ָ��λ�úʹ�С�Ĳ�������*/
//	CEMenuOut(0,0,_BSCAN_B4,9,1,16);
    TextOut(0,0,1,18,32,(char *)_Bscan[MGetLanguage()][2],4);
//	WriteScale();	//���
    MSetColor(C_CR_MENU);
    for( i = 0 ; i < C_LEN_BUFF; i++)
    {
        for( j = 0; j < 4; j++)
        {
            bug[i][j][0] = 0;
            bug[i][j][1] = 0;
        }
    }
    x0 = 0;
    preypos = y0 = C_COORVPOSI +30;//C_COORVPOSI + C_COORHEIGHT;
    tm = 30;//30;//���ʱ�䳤��
    preElapsedtime = GetElapsedTime();	//��ʼ����ʱ��

	int iWidth = C_COORWIDTH + 20;
	
    for( xpos = 0,ip = 0, flag = 1; xpos < iWidth; xpos++)
    {
        keycode = MGetKeyCode(0);
        if(keycode == C_KEYCOD_RETURN)
        {
            if( DisplayQuery(4) == 1)
            {
                break;
            }
        }
//		MParaRenovate(0);
//;		MSetAcquisitionEnable(0,C_SETMODE_SETSAVE);
        sampbuffer = GetSampleBuffer();

        for( j = 0; j < 4; j++)
        {
            bug[ip][j][0] = 0;
            bug[ip][j][1] = 0;
        }
        k = C_COORWIDTH*80/100 ;
        for( i = k+5,amp = 0; i >= k - 5 ; i--)
        {
            //�Ӳ���ˮƽ0��80���ҷ岨
            nextamp = *(sampbuffer+i);
//			if( nextamp > C_REJ )
            {
                //���߱������5����ʾһ�����ο�ʼ
                for( l = i-1  ; l >= k - 5 && nextamp >= C_REJ ; l--)//����С��2��ʾһ�����ν���
                {
                    nextamp = *(sampbuffer+l);	//��ǰ����ֵ
                    if( nextamp > amp )
                    {
                        //Ŀǰ����߲�
                        amp = nextamp;
                        i = l;
                    }
                }
            }
        }
        bug[ip][0][0] = C_COORWIDTH*80/100;
        bug[ip][0][1] = (amp>>2)<<2;//����ײ���λ�ú͸߶�

        k = C_COORWIDTH*80/100 - 5;
        i = 28;
        for( amp = *(sampbuffer+i) ; i <= 56 ; i++)
        {
            //�Ӳ���ˮƽ0��80���ҷ岨
            if( amp <= C_REJ )break;
            amp = *(sampbuffer+i);
        }//ȥ��ʼ��

        for( amp = *(sampbuffer+i), jp = 1 ; i <= k ; i++)
        {
            //�Ӳ���ˮƽ0��80���ҷ岨
            if( amp > C_REJ )
            {
                //���߱������5����ʾһ�����ο�ʼ
                for( l = i+1 ,nextamp = amp; l < k && nextamp >= 2 ; l++)//����С��2��ʾһ�����ν���
                {
                    nextamp = *(sampbuffer+l);	//��ǰ����ֵ
                    if( nextamp > amp )
                    {
                        //Ŀǰ����߲�
                        amp = nextamp;
                        i = l;
                    }
                }
                bug[ip][jp][0] = i;//(i>>2)<<2;
                bug[ip][jp][1] = (amp>>2)<<2;//����һ���岨��λ�ú͸߶�

                ///	break;//���ֻ��һ��ȱ�ݲ������ڴ��������������ȥ��

                jp++;
                if( jp > C_WAVENUM + 1)//���ײ�����4���岨,[0]�ײ�[1][2][3]
                {
                    break;
                    if( bug[ip][1][1] <= bug[ip][2][1] && bug[ip][1][1] <= bug[ip][3][1])
                    {
                        //��һ�����
                        bug[ip][1][1] = bug[ip][2][1];
                        bug[ip][1][0] = bug[ip][2][0];
                        bug[ip][2][1] = bug[ip][3][1];
                        bug[ip][2][0] = bug[ip][3][0];
                    }
                    else if( bug[ip][2][1] <= bug[ip][3][1] )
                    {
                        //�ڶ������
                        bug[ip][2][1] = bug[ip][3][1];
                        bug[ip][2][0] = bug[ip][3][0];
                    }
                    jp--;
                }
                i = l + 5;
            }
            nextamp = *(sampbuffer+i);	//��ǰ����ֵ
            amp = nextamp;
        }
        /*
        		if( bug[ip][0][1] > 32)
        		{//�еײ�,�ҽϸ�
        			if( bug[ip][1][1] * bug[ip][2][0] * 2< bug[ip][2][1] * bug[ip][1][1])
        			{
        				 bug[ip][1][1] = bug[ip][2][1] ;
        				 bug[ip][1][0] = bug[ip][2][0] ;
        			}
        		}
        		else
        		{
        		}
        		if( bug[ip][0][1] == 0)
        		{//�޵ײ�
        			for( jp = 1; jp < 4; jp++)
        			{

        				if( bug[ip][jp][1] > 0)
        				{//�޵ײ�
        					for( j = jp ; j < 4; j++)
        					{
        //						 bug[ip][j-jp][1] = bug[ip][j][1] ;
        //						 bug[ip][j-jp][0] = bug[ip][j][0] ;
        					}
        					break;
        				}
        			}
        		}
        /*/
        if( bug[ip][1][1] > 16 || bug[ip][1][1] * 8 > bug[ip][0][1] )
        {
            //�޵ײ�
            bug[ip][0][1] = bug[ip][1][1] ;
            bug[ip][0][0] = bug[ip][1][0] ;
        }
//*/
//;		MSetAcquisitionEnable(1,C_SETMODE_SETSAVE);
        if( GetElapsedTime() <= preElapsedtime + tm)
        {
            xpos--;//���ʱ��
            continue;
        }
        else preElapsedtime = GetElapsedTime();

		
        for( kp = 0 ; kp < C_LEN_BUFF; kp++)
        {
            for( jp = 0; jp < 4; jp++)
            {
//		Write_Number( kp*20,30+19*jp,bug[kp][jp][0]/4,2,0,0);
//		Write_Number( kp*20,30+19*jp+9,bug[kp][jp][1]/4,2,0,0);
            }
        }

        ip++;
        if( ip >= C_LEN_BUFF)
        {
            //����������
            ip = C_LEN_BUFF - 1;
            jp = C_LEN_BUFF/2;
            //flag;//=0��ʾǰһ�����޵ײ��޻ز���1�еײ��޻ز���2�лز��޵ײ���3�лز��еײ�
//Write_Number(0,0,bug[jp][0][1],4,0,0);
//Write_Number(0,10,bug[jp][0][0],4,0,0);
//Write_Number(40,0,bug[jp][1][1],4,0,0);
//Write_Number(40,10,bug[jp][1][0],4,0,0);
//Write_Number(80,0,bug[jp][2][1],4,0,0);
//Write_Number(80,10,bug[jp][2][0],4,0,0);
            if( bug[jp][0][1] > 32  && bug[jp][0][1] *2> bug[jp][1][1])
            {
                //�еײ�,����һ���ĸ߶�
                if( bug[jp][1][1] < 64 && bug[jp][1][1] * 2 < bug[jp][0][1] && bug[jp][0][1] > 128 )
                {
                    //���ײ�
                    kp =  bug[jp][C_BASE][0];//λ��
                    amp = bug[jp][C_BASE][1];//����
                    ypos = C_COORVPOSI + ( (kp-0)*C_COORHEIGHT*80)/(C_COORWIDTH*100)+high;
                    ///MSetDisplayColor( c_crPara[(amp>>6)%8] );
                    x0 = xpos-C_LEN_BUFF/2;
                    MDrawPixel( x0, preypos ,DP_SET);
#if C_DEVLIB == 23
                    MDrawPixel( x0, ypos ,0);
#else
                    MDrawPixel( x0, ypos , DP_SET);
#endif
                    if( y0 != ypos )MDrawLine(x0,y0,x0,ypos,C_CR_WAVEBACK);
                    y0 = ypos;
                    flag = 1;
                }
                else if( bug[jp][1][1] > 64 ||
                         (bug[jp][1][1] > 32 && bug[jp][1][1] * 2 > bug[jp][0][1] ) )
                {
                    //ȱ�ݲ����ڵײ���1/2,���ߵײ�С��80
                    kp =  bug[jp][1][0];//λ��
                    amp = bug[jp][1][1];//����
                    ypos = C_COORVPOSI + ( (kp-0)*C_COORHEIGHT*80)/(C_COORWIDTH*100)+high;
                    ///MSetDisplayColor( c_crPara[(amp>>6)%8] );
                    x0 = xpos-C_LEN_BUFF/2;
                    MDrawPixel( x0, preypos ,DP_SET);
#if C_DEVLIB == 23
                    MDrawPixel( x0, ypos ,0);
#else
                    MDrawPixel( x0, ypos , DP_SET);
#endif

                    if( y0 != ypos )MDrawLine(x0,y0,x0,ypos,C_CR_WAVEBACK);
                    y0 = ypos;
                    flag = 2;
                }
                else// if( bug[jp][1][1] > 32 && bug[jp][0][1] > 128+64 && bug[jp][1][1]*8 >  bug[jp][0][1])
                {
                    //ȱ�ݲ��ײ�����һ���߶ȣ�
                    kp =  bug[jp][1][0];//λ��
                    amp = bug[jp][1][1];//����
                    ypos = C_COORVPOSI + ( (kp-0)*C_COORHEIGHT*80)/(C_COORWIDTH*100)+high;
                    ///MSetDisplayColor( c_crPara[(amp>>6)%8] );
                    x0 = xpos-C_LEN_BUFF/2;
                    MDrawPixel( x0, preypos ,DP_SET);
                    MDrawPixel( x0, ypos , DP_SET);

                    kp =  bug[jp][C_BASE][0];//λ��
                    amp = bug[jp][C_BASE][1];//����
                    ypos = C_COORVPOSI + ( (kp-0)*C_COORHEIGHT*80)/(C_COORWIDTH*100)+high;
                    ///MSetDisplayColor( c_crPara[(amp>>6)%8] );
                    x0 = xpos-C_LEN_BUFF/2;
                    MDrawPixel( x0, preypos ,DP_SET);
                    MDrawPixel( x0, ypos , DP_SET);
                    flag = 1;
                }



            }
            else//�޵ײ�
            {
                if( bug[jp][1][1] > 0  && bug[jp][2][1] > 0 )//������ȱ�ݲ�
                {
                    if( ( bug[jp][1][1] < 64 && bug[jp][1][1] * 4 < bug[jp][2][1] ) ||
                            bug[jp][2][1] > 128 + 32)
                    {
                        //ȱ�ݲ����ڵײ���1/2,���ߵײ�С��80
                        kp =  bug[jp][2][0];//λ��
                        amp = bug[jp][2][1];//����
                        ypos = C_COORVPOSI + ( (kp-0)*C_COORHEIGHT*80)/(C_COORWIDTH*100)+high;
                        ///MSetDisplayColor( c_crPara[(amp>>6)%8] );
                        x0 = xpos-C_LEN_BUFF/2;
                        MDrawPixel( x0, preypos ,DP_SET);
                        MDrawPixel( x0, ypos , DP_SET);
                        if( y0 != ypos )MDrawLine(x0,y0,x0,ypos,C_CR_WAVEBACK);
                        y0 = ypos;
                        flag = 2;
                    }
                    else if( ( bug[jp][1][1] * 4 > bug[jp][2][1] && bug[jp][2][1] < 128 + 64 ) ||
                             (  bug[jp][1][1] > 16 && bug[jp][1][1] * 2 > bug[jp][2][1]  ) )
                    {
                        //�޲�
                        kp =  bug[jp][1][0];//λ��
                        amp = bug[jp][1][1];//����
                        ypos = C_COORVPOSI + ( (kp-0)*C_COORHEIGHT*80)/(C_COORWIDTH*100)+high;
                        ///MSetDisplayColor( c_crPara[(amp>>6)%8] );
                        x0 = xpos-C_LEN_BUFF/2;
                        MDrawPixel( x0, preypos ,DP_SET);
                        MDrawPixel( x0, ypos , DP_SET);
                        if( y0 != ypos )MDrawLine(x0,y0,x0,ypos,C_CR_WAVEBACK);
                        y0 = ypos;
                        flag = 2;
                    }
                    else// if( bug[jp][2][1] > 32 && bug[jp][1][1] > 128+64 && bug[jp][2][1]*8 >  bug[jp][1][1])
                    {
                        //ȱ�ݲ��ײ�����һ���߶ȣ�
                        kp =  bug[jp][2][0];//λ��
                        amp = bug[jp][2][1];//����
                        ypos = C_COORVPOSI + ( (kp-0)*C_COORHEIGHT*80)/(C_COORWIDTH*100)+high;
                        ///MSetDisplayColor( c_crPara[(amp>>6)%8] );
                        x0 = xpos-C_LEN_BUFF/2;
                        MDrawPixel( x0, preypos ,DP_SET);
                        MDrawPixel( x0, ypos , DP_SET);

                        kp =  bug[jp][1][0];//λ��
                        amp = bug[jp][1][1];//����
                        ypos = C_COORVPOSI + ( (kp-0)*C_COORHEIGHT*80)/(C_COORWIDTH*100)+high;
                        ///MSetDisplayColor( c_crPara[(amp>>6)%8] );
                        x0 = xpos-C_LEN_BUFF/2;
                        MDrawPixel( x0, preypos ,DP_SET);
                        MDrawPixel( x0, ypos , DP_SET);
                        flag = 1;
                    }
                }
                else if( bug[jp][1][1] > 0 )
                {
                    kp =  bug[jp][1][0];//λ��
                    amp = bug[jp][1][1];//����
                    ypos = C_COORVPOSI + ( (kp-0)*C_COORHEIGHT*80)/(C_COORWIDTH*100)+high;
                    ///MSetDisplayColor( c_crPara[(amp>>6)%8] );
                    x0 = xpos-C_LEN_BUFF/2;
                    MDrawPixel( x0, preypos ,DP_SET);
                    MDrawPixel( x0, ypos , DP_SET);
                    if( y0 != ypos )MDrawLine(x0,y0,x0,ypos,C_CR_WAVEBACK);
                    y0 = ypos;
                    flag = 2;
                }
                else
                {
                    kp =  bug[jp][0][0];//λ��
                    amp = bug[jp][0][1];//����
                    ypos = C_COORVPOSI + ( (kp-0)*C_COORHEIGHT*80)/(C_COORWIDTH*100)+high;
                    ///MSetDisplayColor( c_crPara[(amp>>6)%8] );
                    x0 = xpos-C_LEN_BUFF/2;
                    MDrawPixel( x0, preypos ,DP_SET);
                    MDrawPixel( x0, ypos , DP_SET);
                    if( y0 != ypos )MDrawLine(x0,y0,x0,ypos,C_CR_WAVEBACK);
                    y0 = ypos;
                    flag = 2;
                }
            }

            /*
            			if( bug[jp][C_BASE][1] > 0)
            			{//�еײ�
            				if( bug[jp][C_BASE+1][1] == 0)//��������
            				{
            					kp =  bug[jp][C_BASE][0];//λ��
            					amp = bug[jp][C_BASE][1];//����
            					ypos = C_COORVPOSI + ( (kp-0)*C_COORHEIGHT*100)/(C_COORWIDTH*80);
            					MSetDisplayColor( c_crPara[(amp>>6)%8] );
            					x0 = xpos-C_LEN_BUFF/2;
            					MDrawPixel( x0, ypos , DP_SET);
            					if( flag == 2 )MDrawLine(x0,y0,x0,ypos);
            					y0 = ypos;
            					flag = 1;
            				}
            				else //��������
            				{
            					for( kp = jp+1; kp < C_LEN_BUFF ; kp++)
            					{//����в�
            						if( bug[kp][C_BASE][1] > 0 && bug[kp][C_BASE+1][1] > 0)continue;
            						break;
            					}
            					for( lp = jp-1; lp >= 0; lp--)
            					{//��ǰ��
            						if( bug[lp][C_BASE][1] > 0 && bug[lp][C_BASE+1][1] > 0)continue;
            						break;
            					}
            					if( jp - lp > kp - jp )
            					{//ǰ��ͬʱ���ڵĲ��Ⱥ����
            						if( lp < 0 && kp < C_LEN_BUFF)
            						{//ǰ��һֱ���лز�
            							if( bug[jp][C_BASE+1][1] > bug[jp][C_BASE][1]*2 || ( bug[kp][C_BASE][1] <= 0 && kp - jp > C_LEN_BUFF/4) )
            							{//�ز����ڵײ�˫�������ߺ���ײ����ڻز���ʧ�һز�����ʱ��ϳ�
            								kp =  bug[jp][C_BASE+1][0];//λ��
            								amp = bug[jp][C_BASE+1][1];//����
            								ypos = C_COORVPOSI + ( (kp-0)*C_COORHEIGHT*100)/(C_COORWIDTH*80);
            								MSetDisplayColor( c_crPara[(amp>>6)%8] );
            								x0 = xpos-C_LEN_BUFF/2;
            								MDrawPixel( x0, ypos , DP_SET);
            								if( flag == 1 )MDrawLine(x0,y0,x0,ypos);
            								y0 = ypos;
            								flag = 2;
            							}
            							else
            							{
            								kp =  bug[jp][C_BASE][0];//λ��
            								amp = bug[jp][C_BASE][1];//����
            								ypos = C_COORVPOSI + ( (kp-0)*C_COORHEIGHT*100)/(C_COORWIDTH*80);
            								MSetDisplayColor( c_crPara[(amp>>6)%8] );
            								x0 = xpos-C_LEN_BUFF/2;
            								MDrawPixel( x0, ypos , DP_SET);
            								if( flag == 2 )MDrawLine(x0,y0,x0,ypos);
            								y0 = ypos;
            								flag = 1;
            							}

            						}
            						else //if( kp < C_LEN_BUFF )//ǰ�治���лز�
            						{//���߶��лز�������������
            							if(  ( bug[kp][C_BASE+1][1] <= 0 && kp - jp > C_LEN_BUFF/8) &&
            								( bug[lp][C_BASE+1][1] <= 0 && jp - lp > C_LEN_BUFF/8) &&
            								(bug[jp][C_BASE][1] < bug[jp][C_BASE+1][1]*4 ) )
            							{//����ز����ڵײ���ʧ,�ҳ���ʱ�䲻��,����ǰ��ز����ڵײ���ʧ��ʱ�䲻��
            								kp =  bug[jp][C_BASE+1][0];//λ��
            								amp = bug[jp][C_BASE+1][1];//����
            								ypos = C_COORVPOSI + ( (kp-0)*C_COORHEIGHT*100)/(C_COORWIDTH*80);
            								MSetDisplayColor( c_crPara[(amp>>6)%8] );
            								MDrawPixel( xpos - C_LEN_BUFF/2, ypos , DP_SET);
            								kp =  bug[jp][C_BASE][0];//λ��
            								amp = bug[jp][C_BASE][1];//����
            								ypos = C_COORVPOSI + ( (kp-0)*C_COORHEIGHT*100)/(C_COORWIDTH*80);
            								MSetDisplayColor( c_crPara[(amp>>6)%8] );
            								MDrawPixel( xpos - C_LEN_BUFF/2, ypos , DP_SET);
            							}
            							else if( bug[kp][C_BASE][1] <= 0 && jp - lp > C_LEN_BUFF/4 )
            							{//�ײ�����ʧ�һز��ѳ���һ��ʱ��
            								kp =  bug[jp][C_BASE+1][0];//λ��
            								amp = bug[jp][C_BASE+1][1];//����
            								ypos = C_COORVPOSI + ( (kp-0)*C_COORHEIGHT*100)/(C_COORWIDTH*80);
            								MSetDisplayColor( c_crPara[(amp>>6)%8] );
            								x0 = xpos-C_LEN_BUFF/2;
            								MDrawPixel( x0, ypos , DP_SET);
            								if( flag == 1 )MDrawLine(x0,y0,x0,ypos);
            								y0 = ypos;
            								flag = 2;
            							}
            							else
            							{
            								kp =  bug[jp][C_BASE][0];//λ��
            								amp = bug[jp][C_BASE][1];//����
            								ypos = C_COORVPOSI + ( (kp-0)*C_COORHEIGHT*100)/(C_COORWIDTH*80);
            								MSetDisplayColor( c_crPara[(amp>>6)%8] );
            								x0 = xpos-C_LEN_BUFF/2;
            								MDrawPixel( x0, ypos , DP_SET);
            								if( flag == 2 )MDrawLine(x0,y0,x0,ypos);
            								y0 = ypos;
            								flag = 1;
            							}
            						}
            					}
            					else
            					{//����ͬʱ���ڵĲ���ǰ���
            						if( lp >= 0 && kp >= C_LEN_BUFF)
            						{//����һֱ���лز�
            							if( bug[jp][C_BASE+1][1] > bug[jp][C_BASE][1]*2 ||
            								( bug[lp][C_BASE][1] <= 0 && jp - lp > C_LEN_BUFF/4) )
            							{//�ز����ڵײ�˫��������ǰ��ײ����ڻز���ʧ�һز�����ʱ��ϳ�
            								kp =  bug[jp][C_BASE+1][0];//λ��
            								amp = bug[jp][C_BASE+1][1];//����
            								ypos = C_COORVPOSI + ( (kp-0)*C_COORHEIGHT*100)/(C_COORWIDTH*80);
            								MSetDisplayColor( c_crPara[(amp>>6)%8] );
            								x0 = xpos-C_LEN_BUFF/2;
            								MDrawPixel( x0, ypos , DP_SET);
            								if( flag == 1 )MDrawLine(x0,y0,x0,ypos);
            								y0 = ypos;
            								flag = 2;
            							}
            							else
            							{
            								kp =  bug[jp][C_BASE][0];//λ��
            								amp = bug[jp][C_BASE][1];//����
            								ypos = C_COORVPOSI + ( (kp-0)*C_COORHEIGHT*100)/(C_COORWIDTH*80);
            								MSetDisplayColor( c_crPara[(amp>>6)%8] );
            								x0 = xpos-C_LEN_BUFF/2;
            								MDrawPixel( x0, ypos , DP_SET);
            								if( flag == 2 )MDrawLine(x0,y0,x0,ypos);
            								y0 = ypos;
            								flag = 1;
            							}
            						}
            						else //if( kp < C_LEN_BUFF )//ǰ�治���лز�
            						{//���߶��лز�������������
            							if(  ( bug[kp][C_BASE+1][1] <= 0 && kp - jp > C_LEN_BUFF/8) &&
            								( bug[lp][C_BASE+1][1] <= 0 && jp - lp > C_LEN_BUFF/8) &&
            								( bug[jp][C_BASE][1] < bug[jp][C_BASE+1][1]*4 ) )
            							{//����ز����ڵײ���ʧ,�ҳ���ʱ�䲻��,����ǰ��ز����ڵײ���ʧ��ʱ�䲻��
            								kp =  bug[jp][C_BASE+1][0];//λ��
            								amp = bug[jp][C_BASE+1][1];//����
            								ypos = C_COORVPOSI + ( (kp-0)*C_COORHEIGHT*100)/(C_COORWIDTH*80);
            								MSetDisplayColor( c_crPara[(amp>>6)%8] );
            								MDrawPixel( xpos - C_LEN_BUFF/2, ypos , DP_SET);
            								kp =  bug[jp][C_BASE][0];//λ��
            								amp = bug[jp][C_BASE][1];//����
            								ypos = C_COORVPOSI + ( (kp-0)*C_COORHEIGHT*100)/(C_COORWIDTH*80);
            								MSetDisplayColor( c_crPara[(amp>>6)%8] );
            								MDrawPixel( xpos - C_LEN_BUFF/2, ypos , DP_SET);
            							}
            							else if( bug[kp][C_BASE][1] <= 0 && jp - lp > C_LEN_BUFF/4 )
            							{//�ײ�����ʧ�һز��ѳ���һ��ʱ��
            								kp =  bug[jp][C_BASE+1][0];//λ��
            								amp = bug[jp][C_BASE+1][1];//����
            								ypos = C_COORVPOSI + ( (kp-0)*C_COORHEIGHT*100)/(C_COORWIDTH*80);
            								MSetDisplayColor( c_crPara[(amp>>6)%8] );
            								x0 = xpos-C_LEN_BUFF/2;
            								MDrawPixel( x0, ypos , DP_SET);
            								if( flag == 1 )MDrawLine(x0,y0,x0,ypos);
            								y0 = ypos;
            								flag = 2;
            							}
            							else
            							{
            								kp =  bug[jp][C_BASE][0];//λ��
            								amp = bug[jp][C_BASE][1];//����
            								ypos = C_COORVPOSI + ( (kp-0)*C_COORHEIGHT*100)/(C_COORWIDTH*80);
            								MSetDisplayColor( c_crPara[(amp>>6)%8] );
            								x0 = xpos-C_LEN_BUFF/2;
            								MDrawPixel( x0, ypos , DP_SET);
            								if( flag == 2 )MDrawLine(x0,y0,x0,ypos);
            								y0 = ypos;
            								flag = 1;
            							}
            						}
            					}
            				}
            			}
            			else
            			{//�޵ײ�
            				if( bug[jp][C_BASE+1][1] <= 2)//��������
            				{
            					x0 = xpos-C_LEN_BUFF/2;
            					ypos = C_COORVPOSI;
            			//		MDrawPixel( x0, ypos , DP_SET);
            			//		if( flag != 2 )MDrawLine(x0,y0,x0,ypos);
            					y0 = ypos;
            					flag = 2;
            				}
            				else //��������
            				{
            					kp =  bug[jp][C_BASE+1][0];//λ��
            					amp = bug[jp][C_BASE+1][1];//����
            					ypos = C_COORVPOSI + ( (kp-0)*C_COORHEIGHT*100)/(C_COORWIDTH*80);
            					MSetDisplayColor( c_crPara[(amp>>6)%8] );
            					x0 = xpos-C_LEN_BUFF/2;
            					MDrawPixel( x0, ypos , DP_SET);
            					if( flag == 1 )MDrawLine(x0,y0,x0,ypos);
            					y0 = ypos;
            					flag = 2;
            				}
            			}
            //*/
            for( jp = 1 ; jp < C_LEN_BUFF; jp++)
            {
                for( kp = 0; kp < 4; kp++)
                {
                    bug[jp-1][kp][1] = bug[jp][kp][1];
                    bug[jp-1][kp][0] = bug[jp][kp][0];
                }
            }
        }
    }
    MKeyRlx();

    if( xpos >= C_COORWIDTH )
        MDrawLine(x0,preypos,x0,ypos,C_CR_WAVEBACK);
    DisplayPrompt(15);
    //	CEMenuOut(0,0,_Bscan,10,1,24);
    TextOut(0,0,1,32,16,(char *)_Bscan[MGetLanguage()][0],4);
    MAnyKeyReturn();

    AllParaRestore();
    MSetSystem();
//	ScreenRenovate();
//	DrawDac(0);

}

//TOFD����Ƶ��
int g_iTofdFreq = 15;
//��������ֵ
long g_iEncValue = 6;

//�Ƕ�
//�������
//PCS, 2λС��
int g_iPCS = (int)((2.0 * 2.0 / 3.0 * tan(3.1415926/180.0*60) * 200 / 10 + 0.05) * 10);

//1mmһ�Σ���������С��1000mm
#define MAX_LINE 1000
u_char g_pEcho[MAX_LINE][ECHO_PACKAGE_SIZE];
int    g_iLine = 0;

u_int g_iRang = 0, g_iDelay = 0;

void DrawFuncMenu( int iIndex )
{
	char  szkey[64];
	int   iNumber = 0;
			
	MSetDisplayColor( 0x3F << 5 );
	//������д�������	
	EraseWindow( 0, 0, C_HORIDOT_SCREEN, C_VERTDOT_SCREEN );
	
	DrawLine( 1, 55, C_HORIDOT_SCREEN - 1, 55 );
	DrawLine( C_HORIDOT_SCREEN - 1, 55, C_HORIDOT_SCREEN - 1, C_VERTDOT_SCREEN - 1 );
	DrawLine( 1, 55, 1, C_VERTDOT_SCREEN - 1 );
	DrawLine( 1, C_VERTDOT_SCREEN - 1, C_HORIDOT_SCREEN - 1, C_VERTDOT_SCREEN - 1 );
	
	DrawLine( 1, 420, C_HORIDOT_SCREEN - 1, 420 );
	DrawLine( 160, 420, 160, 479 );
	DrawLine( 320, 420, 320, 479 );
	DrawLine( 480, 420, 480, 479 );
	/*
	DrawLine( C_COORHPOSI+2, C_VERTDOT_VIDEO- C_COORVPOSI - 18, C_COORHPOSI+2, C_VERTDOT_VIDEO-5 );
	DrawLine( C_COORHPOSI+2, C_VERTDOT_VIDEO- C_COORVPOSI - 18, C_COORHPOSI+494, C_VERTDOT_VIDEO - C_COORVPOSI - 18 );
	DrawLine( C_COORHPOSI+494, C_VERTDOT_VIDEO - C_COORVPOSI - 18, C_COORHPOSI+494, C_VERTDOT_VIDEO-5 );
	DrawLine( C_COORHPOSI+2, C_VERTDOT_VIDEO-5, C_COORHPOSI+494, C_VERTDOT_VIDEO-5 );
	
	DrawLine( C_COORHPOSI+123, C_VERTDOT_VIDEO- C_COORVPOSI - 18, C_COORHPOSI+123, C_VERTDOT_VIDEO-5 );
	DrawLine( C_COORHPOSI+246, C_VERTDOT_VIDEO- C_COORVPOSI - 18, C_COORHPOSI+246, C_VERTDOT_VIDEO-5 );
	DrawLine( C_COORHPOSI+369, C_VERTDOT_VIDEO- C_COORVPOSI - 18, C_COORHPOSI+369, C_VERTDOT_VIDEO-5 );
	*/
	if( iIndex == 0 )
	{
		MSetDisplayColor( 0xFFFF );
		TextOut( 1, 435, 1, 159, 465, "��У", 4 );
		//TextOut( C_COORHPOSI+42, C_VERTDOT_VIDEO- C_COORVPOSI - 5, 1, C_COORHPOSI+122, C_VERTDOT_VIDEO- C_COORVPOSI, "��У", 4 );
		MSetDisplayColor( 0x3F << 5 );
		
		TextOut( C_COORHPOSI+10, C_COORVPOSI+10, 1, C_COORHPOSI+100, C_COORVPOSI+30, "0. ̽ͷǰ��", 4 );
		TextOut( C_COORHPOSI+10, C_COORVPOSI+40, 1, C_COORHPOSI+100, C_COORVPOSI+60, "1. ������", 4 );
	}
	else
	{
		TextOut( 1, 435, 1, 159, 465, "��У", 4 );
	}
	
	if( iIndex == 1 )
	{
		MSetDisplayColor( 0xFFFF );
		TextOut( 200, 435, 1, 319, 465, "����", 4 );
		MSetDisplayColor( 0x3F << 5 );

		TextOut( C_COORHPOSI+10, C_COORVPOSI+10, 1, C_COORHPOSI+200, C_COORVPOSI+30, "0.̽ͷƵ��:     MHz", 4 );
		Write_Number(C_COORHPOSI+152, C_COORVPOSI+10, MGetFrequence(),5,2,0);
		TextOut( C_COORHPOSI+10, C_COORVPOSI+40, 1, C_COORHPOSI+100, C_COORVPOSI+60, "1.��Ƭ�ߴ�:      ", 4 );
		Write_Crystal( C_COORHPOSI+152, C_COORVPOSI+40, MGetCrystal_l(), MGetCrystal_w());
		sprintf( szkey, "2.�Ƕ�   :%d ��           ", ChannelPara.iAngle );
		TextOut( C_COORHPOSI+10, C_COORVPOSI+70, 1, C_COORHPOSI+200, C_COORVPOSI+90, szkey, 4 );
		TextOut( C_COORHPOSI+10, C_COORVPOSI+100, 1, C_COORHPOSI+100, C_COORVPOSI+120, "3.�������:     mm ", 4 );
		Write_Number(C_COORHPOSI+152, C_COORVPOSI+100, ChannelPara.iThickness,5,1,0);
		TextOut( C_COORHPOSI+10, C_COORVPOSI+130, 1, C_COORHPOSI+100, C_COORVPOSI+150, "4.����   :", 4 );
		WriteSpeed( C_COORHPOSI+152, C_COORVPOSI+130 );
		TextOut( C_COORHPOSI+10, C_COORVPOSI+160, 1, C_COORHPOSI+100, C_COORVPOSI+180, "5.���   :     ��s ", 4 );
		Write_Number( C_COORHPOSI+152, C_COORVPOSI+160, MGetOffset()*10/16, 5, 2, 0 );
		sprintf( szkey, "6.PCS   :%d.%d", g_iPCS/10, g_iPCS%10 );
		TextOut( C_COORHPOSI+10, C_COORVPOSI+190, 1, C_COORHPOSI+100, C_COORVPOSI+210, szkey, 4 );
		TextOut( C_COORHPOSI+10, C_COORVPOSI+220, 1, C_COORHPOSI+100, C_COORVPOSI+240, "7.̽ͷǰ��:        ", 4 );
		WriteLongness(C_COORHPOSI+152, C_COORVPOSI+220,MGetForward(),5,1);
		iNumber = MGetPulseMode();
		sprintf( szkey, "8.�����ѹ:%s         ", iNumber == 0?"��ѹ":iNumber == 1?"��ѹ":iNumber == 2?"��׼":"��ѹ" );
		TextOut( C_COORHPOSI+10, C_COORVPOSI+250, 1, C_COORHPOSI+100, C_COORVPOSI+270, szkey, 4 );
		sprintf( szkey, "9.ɨ��Ƶ��:%d.%d%d mm/��       ", (g_iTofdFreq * g_iEncValue)/100, (g_iTofdFreq * g_iEncValue)%100/10, (g_iTofdFreq * g_iEncValue)%10 );
		TextOut( C_COORHPOSI+10, C_COORVPOSI+280, 1, C_COORHPOSI+100, C_COORVPOSI+300, szkey, 4 );
	}
	else
	{
		TextOut( 200, 435, 1, 319, 465, "����", 4 );
	}
	
	TextOut( 321, 435, 1, 479, 465, "TOFDɨ��", 4 );
	TextOut( 481, 435, 1, C_HORIDOT_SCREEN - 1, 465, "���ݷ���", 4 );
}

void CalibrationFunc( int iIndex )
{
	int   keycode      = 0;
	int   iEncoder     = 0;
	u_int iRotaryValue = 0, iRotaryValueOld = -1;
	int   number = 0, deci_len = 1;
	char  szkey[64];
	u_char uMax = 0;
	int   iPosMax = 0;
	short xpos, ypos, xposOld, yposOld, i, j;
	int iPt = 3, is = 30;
	int iStep = 0;
	bool bFirst = true;
	bool bAutoSet = true;
	
	u_char* buffer;
	u_char sampbuffer[ECHO_PACKAGE_SIZE], sampbufferOld[ECHO_PACKAGE_SIZE];
	
	
	//������д�������	
	EraseWindow( 0, 0, C_HORIDOT_SCREEN, C_VERTDOT_SCREEN );

	//���ͼ����
	MSetDisplayColor( 0x3F << 5 );
	EraseDrawRectangle( 1, 1, 502, 477 );
	
	//���ܰ�����
	DrawLine( 1, 430, 502, 430 );
	DrawLine( 126, 430, 126, 477 );
	DrawLine( 251, 430, 251, 477 );
	DrawLine( 376, 430, 376, 477 );
	
	//Aɨ�ָ���
	DrawLine( 1, 104, 502, 104 );
	
	
	MSetDisplayColor( 0x3F << 5 );
	//Aɨ����̶ȷָ���
	DrawLine( 1, 134, 502, 134 );
	
	//�ָ�ֱ��
	DrawLine( 502, 3, C_HORIDOT_SCREEN-1, 3 );
	DrawLine( 502, 479, C_HORIDOT_SCREEN-1, 479 );
	
	if( iIndex == 0 )
	{
		MSetDisplayColor( 0x3F << 5 );
		DrawLine( 1, 400, 502, 400 );
		DrawLine( 502, 104, C_HORIDOT_SCREEN-1, 104 );
		DrawLine( 502, 194, C_HORIDOT_SCREEN-1, 194 );
		
		MSetRange(300,C_SETMODE_SETSAVE) ;
		MSetScaleDelay( 0, C_SETMODE_SETSAVE );
		MSetBaseGain( 160, C_SETMODE_SAVE );
		MSetSystem();
		MSetProbeMode(2,C_SETMODE_SAVE);			//���ó�˫��̽ͷ
		MSetEchoMode(0,C_SETMODE_SETSAVE);			//ȫ��ģʽ
		
		MSetDisplayColor( 0xFFE0 );
		TextOut( 11, 9, 1, 41, 15, "ǰ�����У׼", 4 );
		TextOut( 10, 10, 1, 40, 16, "ǰ�����У׼", 4 );
		
		sprintf( szkey, "̽ͷǰ��:%d.%d mm    ", MGetForward()/10, MGetForward()%10 );
		TextOut( 10, 105, 1, 500, 133, szkey, 4 );

		MSetDisplayColor( 0x3F << 5 );
		if( MGetOffset() == 0 )
			TextOut( 6, 440, 1, 125, 470, "У׼���", 4 );
		else
			TextOut( 6, 440, 1, 125, 470, "������", 4 );
		TextOut( 132, 440, 1, 250, 470, "����ǰ��", 4 );
		TextOut( 265, 440, 1, 375, 470, "�ֶ�У׼", 4 );
		MSetDisplayColor( 0x1F<<11 );
		TextOut( 400, 440, 1, 501, 470, "�˳�", 4 );
		
		iStep = MGetRange(3)*1000.0*2*10/MGetSpeed()/5/16;
		sprintf( szkey, "0.0us" );
		TextOut( 2, 400, 1, 101, 429, szkey, 4 );
		MSetDisplayColor( 0xFFE0 );
		sprintf( szkey, "%d.%d", iStep/10, iStep%10 );
		TextOut( 103, 400, 1, 201, 429, szkey, 4 );
		sprintf( szkey, "%d.%d", iStep*2/10, (iStep*2)%10 );
		TextOut( 203, 400, 1, 301, 429, szkey, 4 );
		sprintf( szkey, "%d.%d", iStep*3/10, (iStep*3)%10 );
		TextOut( 303, 400, 1, 401, 429, szkey, 4 );
		sprintf( szkey, "%d.%d", iStep*4/10, (iStep*4)%10 );
		TextOut( 403, 400, 1, 501, 429, szkey, 4 );

		SetDisplayColor( 0xFFFF );
		TextOut( 514, 13, 1, C_HORIDOT_SCREEN-1, 43, "���� dB", 4 );
		sprintf( szkey, "%d.%d ",MGetBaseGain()/10, MGetBaseGain()%10 );
		TextOut( 534, 53, 1, C_HORIDOT_SCREEN-1, 83, szkey, 4 );
		MSetDisplayColor( 0xFFE0 );
		TextOut( 514, 105, 1, C_HORIDOT_SCREEN-1, 135, "��� us", 4 );
		Write_Number( 534, 135, MGetOffset()*10/16, 5, 2, 0 );
		
		iPt = 196, is = 30;
		TextOut( 504, iPt, 1, C_HORIDOT_SCREEN-1, iPt+is, "1.������", 4 );
		iPt += is;
		TextOut( 504, iPt, 1, C_HORIDOT_SCREEN-1, iPt+is, "2.У׼���", 4 );
		iPt += is;
		TextOut( 504, iPt, 1, C_HORIDOT_SCREEN-1, iPt+is, "3.����ǰ��", 4 );
		iPt += is;
		TextOut( 504, iPt, 1, C_HORIDOT_SCREEN-1, iPt+is, "4.����ǰ��", 4 );
		
		while( true )
		{
			buffer = GetSampleBuffer();
			memcpy( sampbuffer, buffer, ECHO_PACKAGE_SIZE );
			
			//����Aɨ���Σ���ʼʱֱ�ӻ��ƣ�����ֻ���Ʋ���ͬ�Ĳ���
			if( bFirst )
			{
				EraseWindow( 2, 135, 500, 264 );
				MSetDisplayColor( 0xFFE0 );
				xposOld = 2; 
				yposOld = 399 - (sampbuffer[0] * 264 / 255);
				uMax = sampbuffer[0];
				iPosMax = 0;
				for( j = 1; j < ECHO_PACKAGE_SIZE; j++ )
				{
					xpos = 2 + j;
					ypos = 399 - (sampbuffer[j] * 264 / 255);	
					if( ypos )
					DrawLine( xposOld, yposOld, xpos, ypos );
					xposOld = xpos; 
					yposOld = ypos;
					if( sampbuffer[j] > uMax )
					{
						uMax = sampbuffer[j];
						iPosMax = j;
					}
				}
				memcpy( sampbufferOld, sampbuffer, ECHO_PACKAGE_SIZE );
				bFirst = false;
			}
			else
			{
				uMax = sampbuffer[0];
				iPosMax = 0;
				for( j = 1; j < ECHO_PACKAGE_SIZE; j++ )
				{
					if( (sampbuffer[j] != sampbufferOld[j]) || (sampbuffer[j-1] != sampbufferOld[j-1]) )
					{
						//���ԭͼ����ʾ
						MSetDisplayColor( 0 );
						xposOld = 1 + j;
						yposOld = 399 - (sampbufferOld[j-1] * 264 / 255);	
						xpos = 2 + j;
						ypos = 399 - (sampbufferOld[j] * 264 / 255);		
						DrawLine( xposOld, yposOld, xpos, ypos );
						//�����µ�ͼ��
						MSetDisplayColor( 0xFFE0 );
						xposOld = 1 + j;
						yposOld = 399 - (sampbuffer[j-1] * 264 / 255);	
						xpos = 2 + j;
						ypos = 399 - (sampbuffer[j] * 264 / 255);		
						DrawLine( xposOld, yposOld, xpos, ypos );
					}
					if( sampbuffer[j] > uMax )
					{
						uMax = sampbuffer[j];
						iPosMax = j;
					}
				}
				memcpy( sampbufferOld, sampbuffer, ECHO_PACKAGE_SIZE );
			}
			
			keycode = MGetKeyCode( 0 );
			
			if( keycode == 13 )
			{
				if( bAutoSet )
				{
					int basegain = MGetBaseGain() - 10;
					MSetBaseGain( basegain, C_SETMODE_SAVE );
					MSetSystem();
					SetDisplayColor( 0xFFFF );
					sprintf( szkey, "%d.%d ",MGetBaseGain()/10, MGetBaseGain()%10 );
					TextOut( 534, 53, 1, C_HORIDOT_SCREEN-1, 83, szkey, 4 );
				}
				else
				{
					number = MGetOffset();
					number-=10;
					if( number >= 0 )
					{
						MSetOffset( number, C_SETMODE_SETSAVE);
						MSetSystem();
						
						MSetDisplayColor( 0xFFE0 );
						Write_Number( 534, 135, MGetOffset()*10/16, 5, 2, 0 );
						
						MSetDisplayColor( 0x3F << 5 );
						EraseWindow( 2, 431, 123, 45 );
						if( number != 0 )
							TextOut( 6, 440, 1, 125, 470, "������", 4 );
						else
							TextOut( 6, 440, 1, 125, 470, "У׼���", 4 );
					}
				}
				bFirst = true;
			}
			//+����
			else if( keycode == 14 )
			{
				if( bAutoSet )
				{
					MSetBaseGain( MGetBaseGain()+10, C_SETMODE_SAVE );
					MSetSystem();
					SetDisplayColor( 0xFFFF );
					sprintf( szkey, "%d.%d ", MGetBaseGain()/10, MGetBaseGain()%10 );
					TextOut( 534, 53, 1, C_HORIDOT_SCREEN-1, 83, szkey, 4 );
				}
				else
				{
					number = MGetOffset();
					number+=10;
					MSetOffset( number, C_SETMODE_SETSAVE);
					MSetSystem();
					
					MSetDisplayColor( 0xFFE0 );
					Write_Number( 534, 135, MGetOffset()*10/16, 5, 2, 0 );
					MSetDisplayColor( 0x3F << 5 );
					EraseWindow( 2, 431, 123, 45 );
					TextOut( 6, 440, 1, 125, 470, "������", 4 );
				}
				bFirst = true;	
			}
			else if( keycode == 16 )
			{
				if( MGetOffset() == 0 )
				{
					
					//MSetGatePara( 5, C_COORWIDTH-10, MGetGatePara(0,2), 0, C_SETMODE_SETSAVE);
					//int iPos = MGetAmpPos(0);
					//number = 30 * 1000 * iPos * 10 * 10 * 2 / MGetSpeed() / ECHO_PACKAGE_SIZE ;
					number = MGetRange(3) * 1000 * iPosMax * 10 * 2 / MGetSpeed() / ECHO_PACKAGE_SIZE ;
					
					MSetOffset( number, C_SETMODE_SETSAVE);
					MSetSystem();
					
					MSetDisplayColor( 0xFFE0 );
					Write_Number( 534, 135, MGetOffset()*10/16, 5, 2, 0 );
					
					if( number != 0 )
					{
						MSetDisplayColor( 0x3F << 5 );
						EraseWindow( 2, 431, 123, 45 );
						TextOut( 6, 440, 1, 125, 470, "������", 4 );
					}
					MKeyRlx();
				}
				else
				{
					MSetOffset(0,C_SETMODE_SETSAVE);
					MSetSystem();
					
					MSetDisplayColor( 0xFFE0 );
					Write_Number( 534, 135, MGetOffset()*10/16, 5, 2, 0 );
					
					MSetDisplayColor( 0x3F << 5 );
					EraseWindow( 2, 431, 123, 45 );
					TextOut( 6, 440, 1, 125, 470, "У׼���", 4 );
					MKeyRlx();
				}
			}
			else if( keycode == 17 )
			{
				int number = 0, deci_len = 1;
				if( Input_Number(300,105,&number,4, &deci_len,0) == 1 )
				{
					if( number >= 0 && ((number <= 3000 && MGetUnitType()==0)||( number <= 11000 && MGetUnitType()==1)) )
					{
						MSetDisplayColor( 0xFFE0 );
						MSetForward(number,C_SETMODE_SAVE);
						sprintf( szkey, "̽ͷǰ��:%d.%d mm    ", number/10, number%10 );
						TextOut( 10, 105, 1, 500, 133, szkey, 4 );
					}
				}
			}
			else if( keycode == 18 )
			{
				if( bAutoSet )
				{
					bAutoSet = false;
					MSetDisplayColor( 0xFFFF );
					TextOut( 265, 440, 1, 375, 470, "�ֶ�У׼", 4 );
					MSetDisplayColor( 0xFFE0 );
					TextOut( 514, 13, 1, C_HORIDOT_SCREEN-1, 43, "���� dB", 4 );
					sprintf( szkey, "%d.%d ",MGetBaseGain()/10, MGetBaseGain()%10 );
					TextOut( 534, 53, 1, C_HORIDOT_SCREEN-1, 83, szkey, 4 );
				}
				else
				{
					bAutoSet = true;
					MSetDisplayColor( 0x3F << 5 );
					TextOut( 265, 440, 1, 375, 470, "�ֶ�У׼", 4 );
					MSetDisplayColor( 0xFFFF );
					TextOut( 514, 13, 1, C_HORIDOT_SCREEN-1, 43, "���� dB", 4 );
					sprintf( szkey, "%d.%d ",MGetBaseGain()/10, MGetBaseGain()%10 );
					TextOut( 534, 53, 1, C_HORIDOT_SCREEN-1, 83, szkey, 4 );
				}
			}
			else if( keycode == 19 )
			{
				break;
			}
			
		}
	}
	else if( iIndex == 1 )
	{
		int iLen = 100;
		//���ñ�����
		do
		{
			SetScanRotaryEncoder( iEncoder, 1, 1, 1 );
			SetScanRotaryEncoder( iEncoder, 1, 0, 1 );
			keycode = MGetKeyCode( 0 );			
			if( keycode == C_KEYCOD_RETURN )
			{
				//ȡ�������𣬷�ֹ�������ж�
				while( true )
				{
					keycode = MGetKeyCode( 0 );
					if( keycode != C_KEYCOD_RETURN )
						break;
				}
			}
		}
		while( GetScanRotaryValue(iEncoder) != 0 );

		MSetDisplayColor( 0xFFE0 );
		TextOut( 11, 9, 1, 41, 15, "������У׼", 4 );
		TextOut( 10, 10, 1, 40, 16, "������У׼", 4 );
		
		sprintf( szkey, "��ǰ�����ֵ:%d.%d%d  ", g_iEncValue/100, g_iEncValue%100/10, g_iEncValue%10 );
		TextOut( 10, 60, 1, 500, 90, "�����ֵ�ο���Χ:0.04��0.08", 4 );	
		TextOut( 10, 105, 1, 500, 133, szkey, 4 );

		MSetDisplayColor( 0x3F << 5 );
		TextOut( 36, 440, 1, 125, 470, "����", 4 );
		TextOut( 132, 440, 1, 250, 470, "�ƶ�����", 4 );
		TextOut( 285, 440, 1, 375, 470, "���", 4 );
		MSetDisplayColor( 0x1F<<11 );
		TextOut( 400, 440, 1, 501, 470, "�˳�", 4 );
		
		MSetDisplayColor( 0x3F << 5 );
		iPt = 60, is = 30;
		TextOut( 504, iPt, 1, C_HORIDOT_SCREEN-1, iPt+is, "1.ָ��ɨ��", 4 );
		iPt += is;
		TextOut( 504, iPt, 1, C_HORIDOT_SCREEN-1, iPt+is, "���ƶ�����.", 4 );
		iPt += is;
		TextOut( 504, iPt, 1, C_HORIDOT_SCREEN-1, iPt+is, "2.��ɨ���", 4 );
		iPt += is;
		TextOut( 504, iPt, 1, C_HORIDOT_SCREEN-1, iPt+is, "�ƶ�ָ����", 4 );
		iPt += is;
		TextOut( 504, iPt, 1, C_HORIDOT_SCREEN-1, iPt+is, "��.", 4 );
		iPt += is;
		TextOut( 504, iPt, 1, C_HORIDOT_SCREEN-1, iPt+is, "3.�ԱȲο�", 4 );
		iPt += is;
		TextOut( 504, iPt, 1, C_HORIDOT_SCREEN-1, iPt+is, "��׼,�Ƚϱ�", 4 );
		iPt += is;
		TextOut( 504, iPt, 1, C_HORIDOT_SCREEN-1, iPt+is, "���ֵ.", 4 );
		
		MSetDisplayColor( 0x3F << 5 );
		TextOut( 10, 140, 1, 160, 170, "�뽫ɨ����ƶ�", 4 );
		MSetDisplayColor( 0xFFE0 );
		TextOut( 180, 140, 1, 245, 170, "100", 4 );
		MSetDisplayColor( 0x3F << 5 );
		TextOut( 245, 140, 1, 500, 170, "mm   ", 4 );
		
		TextOut( 10, 180, 1, 500, 210, "�������ƶ�����:", 4 );
		TextOut( 10, 220, 1, 500, 250, "�����ֵ:", 4 );
			
		i = 0;
		while( true )
		{
			iRotaryValue = GetScanRotaryValue( iEncoder );
			
			if( iRotaryValue > 60000 )
			{
				SetScanRotaryEncoder( iEncoder, 1, 1, 1 );
				SetScanRotaryEncoder( iEncoder, 1, 0, 1 );
				iRotaryValue = 0;
			}
			
			if( iRotaryValue != iRotaryValueOld )
			{
				MSetDisplayColor( 0xFFE0 );
				sprintf( szkey, "%d         ", iRotaryValue );
				TextOut( 200, 180, 1, 500, 210, szkey, 4 );
				
				if( iRotaryValue != 0 )
				{
					sprintf( szkey, "%d.%d%d  ", (iLen*100/iRotaryValue)/100, (iLen*100/iRotaryValue)%100/10, (iLen*100/iRotaryValue)%10 );
					TextOut( 126, 220, 1, 500, 240, szkey, 4 );
				}
				else
				{
					TextOut( 126, 220, 1, 500, 240, "0.00  ", 4 );
				}
				
				iRotaryValueOld = iRotaryValue;
			}
			keycode = MGetKeyCode( 0 );
			
			if( keycode == 16 )
			{
				SetScanRotaryEncoder( iEncoder, 1, 1, 1 );
				SetScanRotaryEncoder( iEncoder, 1, 0, 1 );
				iRotaryValue = 0;
			}
			else if( keycode == 17 )
			{
				int number = 100, deci_len = 0;
				if( Input_Number(300,140,&number,4, &deci_len,0) == 1 )
				{
					iLen = number;
					MSetDisplayColor( 0xFFE0 );
					sprintf( szkey, "%d", iLen );
					TextOut( 180, 140, 1, 245, 170, szkey, 4 );
					iRotaryValueOld = -1;
				}
			}
			else if( (iRotaryValue != 0) && (keycode == 18) )
			{
				g_iEncValue = iLen * 100 / iRotaryValue;
				break;
			}
			else if( keycode == 19 )
			{
				break;
			}
		}
	}
}

void SetFunc( int iIndex )
{
	int keycode      = 0;
	int number = 0, deci_len = 0;
	char  szkey[64];

	//0. ̽ͷƵ��
	if( iIndex == 0 )
	{
		number = 500, deci_len = 2;
		TextOut( 0, 0, 1, 100, 20, "̽ͷƵ��(0 �� 20):", 4 );
		if( Input_Number(0,21,&number,4, &deci_len,0) == 1)
		{
			if( number > 0 && number <= 2000)
			{
				MSetFrequence(number, C_SETMODE_SAVE);
				if( C_DEVLIB != 3)
				{
					if(number < 100 || number > 750) 
						MSetBandMode(2,C_SETMODE_SAVE);
					else if(number >= 100 && number <= 350) 
						MSetBandMode(0,C_SETMODE_SAVE);
					else if(number <= 750) 
						MSetBandMode(1,C_SETMODE_SAVE);
					else if(number <= 1250) 
						MSetBandMode(2,C_SETMODE_SAVE);
				}
			}
		}
	}
	//1. ��Ƭ�ߴ�
	else if( iIndex == 1 )
	{
		number = 100;
        int number1=100;
		TextOut( 0, 0, 1, 100, 20, "��Ƭ�ߴ�:", 4 );
		if( Input_Crystal(0,21,&number,&number1) == 1)
		{
			if( number <= 50000 && number1 <= 50000)
			{
				MSetCrystal_l(number,C_SETMODE_SAVE);
				MSetCrystal_w(number1,C_SETMODE_SAVE);
			}
		}
	}
	//2. �Ƕ�
	else if( iIndex == 2 )
	{
		number = 60, deci_len = 0;
		TextOut( 0, 0, 1, 100, 20, "�Ƕ�(0 �� 90):", 4 );
		if( Input_Number(0,21,&number,4, &deci_len,0) == 1)
		{
			if( number >= 0 && number < 90)
			{
				ChannelPara.iAngle = number;
				g_iPCS = (int)(( 2.0 * 2.0 / 3.0 * tan(3.1415926/180.0* ChannelPara.iAngle) * ChannelPara.iThickness / 10.0 + 0.05) * 10);
				ChannelPara.iPcs = g_iPCS;
				ChannelParaStore();
			}
		}
	}
	//3. �������
	else if( iIndex == 3 )
	{
		number = 20, deci_len = 1;
		TextOut( 0, 0, 1, 100, 20, "���:", 4 );
		if( Input_Number(0,21,&number,4, &deci_len,0) == 1)
		{
			if( number >= 0 )
			{
				ChannelPara.iThickness = number;	
				g_iPCS = (int)(( 2.0 * 2.0 / 3.0 * tan(3.1415926/180.0* ChannelPara.iAngle) * ChannelPara.iThickness / 10.0 + 0.05) * 10);
				ChannelPara.iPcs = g_iPCS;
				ChannelParaStore();
			}
		}
	}
	//4. ����
	else if( iIndex == 4 )
	{
		if( MGetUnitType() > 0 )
			deci_len = 2;//��λinch
        else 
			deci_len = 0;
        
		number = 100;
		
        int MaxSpeed = C_MAX_SPEED, MinSpeed = C_MIN_SPEED;
        
		if( MGetUnitType() )
		{
			MaxSpeed = C_MAX_SPEED * 100 / 25.4 + 0.5;
			MinSpeed = C_MIN_SPEED * 100 / 25.4 + 0.5;
			
			sprintf( szkey, "����(%.2f��%.2f)", MinSpeed/100.0, MaxSpeed/100.0 );
			TextOut( 0, 0, 1, 100, 20, szkey, 4 );
		}
		else
		{
			sprintf( szkey, "����(%2d��%2d)", MinSpeed, MaxSpeed );
			TextOut( 0, 0, 1, 100, 20, szkey, 4 );
		}
		
		
		if( Input_Number(0,21,&number, 5, &deci_len,0) == 1)
		{
			if( number >= MinSpeed && number <= MaxSpeed )
			{
				MSetSpeed( number, C_SETMODE_SAVE );
			}
		}
	}
	//5. ���
	else if( iIndex == 5 )
	{
		number = 0, deci_len = 1;
		TextOut( 0, 0, 1, 100, 20, "���(0��30):", 4 );
		if( Input_Number(0,21,&number,4, &deci_len,0) == 1)
		{
			if( number >= 0 )
			{
				MSetOffset( number * 16, C_SETMODE_SETSAVE );
			}
		}
	}
	//6. PCS
	else if( iIndex == 6 )
	{
		number = 0, deci_len = 1;
		TextOut( 0, 0, 1, 100, 20, "PCS(δ��дʱ�Զ�����):", 4 );
		if( Input_Number(0,21,&number,4, &deci_len,0) == 1)
		{
			g_iPCS = number;
			ChannelPara.iPcs = g_iPCS;
			ChannelParaStore();
		}		
	}
	//7. ̽ͷǰ��
	else if( iIndex == 7 )
	{
		sprintf( szkey, "̽ͷǰ��:" );
		number = 0, deci_len = 1;
		TextOut( 0, 0, 1, 100, 20, szkey, 4 );
		
		if( Input_Number(0,21,&number,2, &deci_len,0) == 1)
		{
			if(( number <= 3000 && MGetUnitType()==0)||( number <= 11000 && MGetUnitType()==1))
			{
				MSetForward(number,C_SETMODE_SAVE);
			}
		}	
	}
	//8. �����ѹ
	else if( iIndex == 8 )
	{
		EraseDrawRectangle( C_COORHPOSI+2, C_COORVPOSI, 495, C_VERTDOT_VIDEO - C_COORVPOSI - 20 ) ;
		TextOut( C_COORHPOSI+10, C_COORVPOSI+10, 1, C_COORHPOSI+100, C_COORVPOSI+30, "1.��ѹ", 4 );
		TextOut( C_COORHPOSI+10, C_COORVPOSI+40, 1, C_COORHPOSI+100, C_COORVPOSI+60, "2.��ѹ", 4 );
		TextOut( C_COORHPOSI+10, C_COORVPOSI+70, 1, C_COORHPOSI+100, C_COORVPOSI+90, "3.��׼", 4 );
		TextOut( C_COORHPOSI+10, C_COORVPOSI+100, 1, C_COORHPOSI+100, C_COORVPOSI+120, "4.��ѹ", 4 );
		
		while( true )
		{
			keycode = MGetKeyCode( 0 );
			
			if( keycode >= 1 && keycode <= 4 )
			{
				MSetPulseMode(keycode-1,C_SETMODE_SAVE);
				break;
			}			
			
			if( keycode == C_KEYCOD_RETURN )
			{
				//ȡ�������𣬷�ֹ�������ж�
				while( true )
				{
					keycode = MGetKeyCode( 0 );
					if( keycode != C_KEYCOD_RETURN )
						break;
				}
				break;
			}
		}
	}
	//9. ɨ��Ƶ��
	else if( iIndex == 9 )
	{
		number = 0, deci_len = 2;
		sprintf( szkey, "ɨ��Ƶ��(%d.%d%d �� 1000):  ", g_iEncValue/100, g_iEncValue%100/10, g_iEncValue%10 );
		TextOut( 0, 0, 1, 100, 20, szkey, 4 );
		if( Input_Number(0,21,&number,4, &deci_len,0) == 1)
		{
			if(number >= g_iEncValue && number <= 1000 )
			{
				g_iTofdFreq = number/g_iEncValue;
			}
		}
	}
}

void DADraw( short iIndex, short iLineStart, short iLineR[2], short iLineB[2], short iIndexB )
{
	short xpos, ypos, xposOld, yposOld, i, j, iMaxLine;
	short clrR, clrG, clrB, clr;
	int   L = 0, iPt = 13, is = 28, L0 = 0, L1 = 0;
	double S0 = 0, S1 = 0;

	char  szkey[64];
	
	if( g_iLine > 324 )
		iMaxLine = 324;
	else
		iMaxLine = g_iLine;
#if 0
	sprintf( szkey, "%d,%d,%d,%d,%d,%d", g_iLine,g_iRang,g_iDelay,g_iPCS,g_iTofdFreq,g_iEncValue );
	TextOut( 10, 10, 1, 170, 40, szkey, 4 );
	MAnyKeyReturn();
#endif	
	EraseWindow( 2, 4, 499, 426 );
	MSetDisplayColor( 0xFFE0 );
	TextOut( 504, iPt, 1, C_HORIDOT_SCREEN, iPt+29, "Aɨ����", 4 );
	iPt += 40;
	sprintf( szkey, "����(%02d)", iIndexB + 1 );
	TextOut( 504, iPt, 1, C_HORIDOT_SCREEN, iPt+29, szkey, 4 );
	iPt = 106;
	TextOut( 504, iPt, 1, C_HORIDOT_SCREEN, iPt+is-1, "ɨ�賤��", 4 );
	iPt += is;
	if( g_iLine != 0 )
		L = (g_iLine + 1) * g_iTofdFreq * g_iEncValue; 
	
	sprintf( szkey, "%d.%dmm", L/100, (L%100)/10 );
	TextOut( 504, iPt, 1, C_HORIDOT_SCREEN, iPt+is-1, szkey, 4 );
	iPt += is;
	TextOut( 504, iPt, 1, C_HORIDOT_SCREEN, iPt+is-1, "ͼ��λ��", 4 );
	iPt += is;
	L = iLineStart * g_iTofdFreq * g_iEncValue;
	sprintf( szkey, "%d.%dmm", L/100, (L%100)/10 );
	TextOut( 504, iPt, 1, C_HORIDOT_SCREEN, iPt+is-1, "        ", 4 );
	TextOut( 504, iPt, 1, C_HORIDOT_SCREEN, iPt+is-1, szkey, 4 );
	iPt += is;
	L = (iLineStart + iMaxLine) * g_iTofdFreq * g_iEncValue;
	sprintf( szkey, "%d.%dmm", L/100, (L%100)/10 );
	TextOut( 504, iPt, 1, C_HORIDOT_SCREEN, iPt+is-1, "        ", 4 );
	TextOut( 504, iPt, 1, C_HORIDOT_SCREEN, iPt+is-1, szkey, 4 );
	iPt += (is+2);

	S0 = g_iRang*iLineR[0]/ECHO_PACKAGE_SIZE + g_iDelay;
	if( S0 >= g_iPCS/2 ) 		
		L0 = sqrt( pow(S0/10.0,2) - pow(g_iPCS/10.0/2, 2) ) * 100;
	else
		L0 = -1;
	
	S1 = g_iRang*iLineR[1]/ECHO_PACKAGE_SIZE + g_iDelay;
	if( S1 >= g_iPCS/2 ) 		
		L1 = sqrt( pow(S1/10.0,2) - pow(g_iPCS/10.0/2, 2) ) * 100;
	else
		L1 = -1;
	
	if( iIndex != 1 )
	{
		MSetDisplayColor( 0x1F<<11 );
		TextOut( 504, iPt-1, 1, C_HORIDOT_SCREEN, iPt+is-1, "        ", 4 );
		TextOut( 504, iPt, 1, C_HORIDOT_SCREEN, iPt+is-1, "����1", 4 );
		iPt += is;
		TextOut( 504, iPt, 1, C_HORIDOT_SCREEN, iPt+is-1, "0.0mm", 4 );
		iPt += is;
		TextOut( 504, iPt-1, 1, C_HORIDOT_SCREEN, iPt+is-1, "        ", 4 );
		TextOut( 504, iPt, 1, C_HORIDOT_SCREEN, iPt+is-1, "����2", 4 );
		iPt += is;

		if( L1 != -1 && L0 == -1 ) 		
		{
			sprintf( szkey, "%d.%dmm", L1/100, (L1%100)/10 );
			TextOut( 504, iPt, 1, C_HORIDOT_SCREEN, iPt+is-1, "        ", 4 );
			TextOut( 504, iPt, 1, C_HORIDOT_SCREEN, iPt+is-1, szkey, 4 );
		}
		else if( L1 != -1 && L0 != -1 ) 		
		{
			if( L1 - L0 >= 0 )
				sprintf( szkey, "%d.%dmm", (L1 - L0)/100, ((L1 - L0)%100)/10 );
			else
				sprintf( szkey, "-%d.%dmm", (L0 - L1)/100, ((L0 - L1)%100)/10 );
			TextOut( 504, iPt, 1, C_HORIDOT_SCREEN, iPt+is-1, "        ", 4 );
			TextOut( 504, iPt, 1, C_HORIDOT_SCREEN, iPt+is-1, szkey, 4 );
		}
		else
		{
			TextOut( 504, iPt, 1, C_HORIDOT_SCREEN, iPt+is-1, "        ", 4 );
			TextOut( 504, iPt, 1, C_HORIDOT_SCREEN, iPt+is-1, "--", 4 );
		}
	}
	else
	{
		MSetDisplayColor( 0x1F );
		TextOut( 504, iPt, 1, C_HORIDOT_SCREEN, iPt+is-1, "        ", 4 );
		TextOut( 504, iPt, 1, C_HORIDOT_SCREEN, iPt+is-1, "����1", 4 );
		iPt += is;
		L = iLineB[0] * g_iTofdFreq * g_iEncValue;
		sprintf( szkey, "%d.%dmm", L/100, (L%100)/10 );
		TextOut( 504, iPt, 1, C_HORIDOT_SCREEN, iPt+is-1, "        ", 4 );
		TextOut( 504, iPt, 1, C_HORIDOT_SCREEN, iPt+is-1, szkey, 4 );
		iPt += is;
		TextOut( 504, iPt, 1, C_HORIDOT_SCREEN, iPt+is-1, "        ", 4 );
		TextOut( 504, iPt, 1, C_HORIDOT_SCREEN, iPt+is-1, "����2", 4 );
		iPt += is;
		L = iLineB[1] * g_iTofdFreq * g_iEncValue;
		sprintf( szkey, "%d.%dmm", L/100, (L%100)/10 );
		TextOut( 504, iPt, 1, C_HORIDOT_SCREEN, iPt+is-1, "        ", 4 );
		TextOut( 504, iPt, 1, C_HORIDOT_SCREEN, iPt+is-1, szkey, 4 );
	}
	MSetDisplayColor( 0xFFE0 );
	iPt += (is+2);
	TextOut( 504, iPt, 1, C_HORIDOT_SCREEN, iPt+is-1, "�����߶�", 4 );
	iPt += is;
	
	if( L1 != -1 && L0 == -1 )
		L = L1;
	else if( L1 != -1 && L0 != -1 )
		L = abs(L1-L0);
	else 
		L = 0;
	
	sprintf( szkey, "%d.%dmm", L/100, (L%100)/10 );
	MSetDisplayColor( 0xFFFF );
	TextOut( 504, iPt, 1, C_HORIDOT_SCREEN, iPt+is-1, "        ", 4 );
	if( L1 == -1 && L0 == -1 )
		TextOut( 504, iPt, 1, C_HORIDOT_SCREEN, iPt+is-1, "--", 4 );
	else
		TextOut( 504, iPt, 1, C_HORIDOT_SCREEN, iPt+is-1, szkey, 4 );
	
	iPt += is;
	MSetDisplayColor( 0xFFE0 );
	TextOut( 504, iPt, 1, C_HORIDOT_SCREEN, iPt+is-1, "��������", 4 );
	iPt += is;
	L = (abs(iLineB[1] - iLineB[0]) + 1) * g_iTofdFreq * g_iEncValue;
	sprintf( szkey, "%d.%dmm", L/100, (L%100)/10 );
	MSetDisplayColor( 0xFFFF );
	TextOut( 504, iPt, 1, C_HORIDOT_SCREEN, iPt+is-1, "        ", 4 );
	TextOut( 504, iPt, 1, C_HORIDOT_SCREEN, iPt+is-1, szkey, 4 );
	
	MSetDisplayColor( 0x3F << 5 );
	//Aɨ�ָ���
	DrawLine( 1, 104, 502, 104 );
	//�ָ�ֱ��
	iPt = 3;
	DrawLine( 502, iPt, C_HORIDOT_SCREEN-1, iPt );
	iPt = 104;
	DrawLine( 502, iPt, C_HORIDOT_SCREEN-1, iPt );
	iPt = 105;
	DrawLine( 502, iPt, C_HORIDOT_SCREEN-1, iPt );
	iPt = 106;
	DrawLine( 502, iPt, C_HORIDOT_SCREEN-1, iPt );
	iPt += is;
	DrawLine( 502, iPt, C_HORIDOT_SCREEN-1, iPt );
	iPt += is;
	DrawLine( 502, iPt, C_HORIDOT_SCREEN-1, iPt );
	iPt += is;
	DrawLine( 502, iPt, C_HORIDOT_SCREEN-1, iPt );
	iPt += is;
	DrawLine( 502, iPt, C_HORIDOT_SCREEN-1, iPt );
	iPt += is;
	DrawLine( 502, iPt, C_HORIDOT_SCREEN-1, iPt );
	iPt += 1;
	DrawLine( 502, iPt, C_HORIDOT_SCREEN-1, iPt );
	iPt += 1;
	DrawLine( 502, iPt, C_HORIDOT_SCREEN-1, iPt );
	iPt += is;
	DrawLine( 502, iPt, C_HORIDOT_SCREEN-1, iPt );
	iPt += is;
	DrawLine( 502, iPt, C_HORIDOT_SCREEN-1, iPt );
	iPt += is;
	DrawLine( 502, iPt, C_HORIDOT_SCREEN-1, iPt );
	iPt += is;
	DrawLine( 502, iPt, C_HORIDOT_SCREEN-1, iPt );
	iPt += 1;
	DrawLine( 502, iPt, C_HORIDOT_SCREEN-1, iPt );
	iPt += 1;
	DrawLine( 502, iPt, C_HORIDOT_SCREEN-1, iPt );
	iPt += is;
	DrawLine( 502, iPt, C_HORIDOT_SCREEN-1, iPt );
	iPt += is;
	DrawLine( 502, iPt, C_HORIDOT_SCREEN-1, iPt );
	iPt += is;
	DrawLine( 502, iPt, C_HORIDOT_SCREEN-1, iPt );
	//iPt += is;
	//DrawLine( 502, iPt, C_HORIDOT_SCREEN, iPt );
	//����
	DrawLine( 502, 477, C_HORIDOT_SCREEN-1, 477 );
	DrawLine( 502, 478, C_HORIDOT_SCREEN-1, 478 );
	DrawLine( 502, 479, C_HORIDOT_SCREEN-1, 479 );
	
	xposOld = 2;
	yposOld = 103 - (g_pEcho[iLineB[iIndexB]][0] * 100 / 255);	
	
	MSetDisplayColor( 0xFFE0 );
	for( j = 1; j < ECHO_PACKAGE_SIZE; j++ )
	{
		xpos = 2 + j;
		ypos = 103 - (g_pEcho[iLineB[iIndexB]][j] * 99 / 255);		
		DrawLine( xposOld, yposOld, xpos, ypos );
		xposOld = xpos;
		yposOld = ypos;	
	}
	
	for( i = 0; i <= iMaxLine; i++ )
	{
		for( j = 0; j < ECHO_PACKAGE_SIZE; j++ )
		{
			//R,G,B��ֵ��ͬʱΪ�ң�������ֵԽ����ɫԽ��
			clrR = g_pEcho[iLineStart+i][j] * 0x1F / 0xFF;
			clrG = g_pEcho[iLineStart+i][j] * 0x3F / 0xFF;
			clrB = g_pEcho[iLineStart+i][j] * 0x1F / 0xFF;
			clr  = ((0x1F & clrR) << 11) | ((0x3F & clrG) << 5) | (0x1F & clrB);
			//���û�����ɫ
			SetDisplayColor( clr );
			xpos = j + 2;
			ypos = 105 + i;
			DrawPixel( xpos, ypos );
		}
	}
	
	//����
	MSetDisplayColor( 0x1F << 11 );
	if( iLineR[0] < ECHO_PACKAGE_SIZE-2 ) 
	{
		DrawLine( 2 + iLineR[0], 4, 2 + iLineR[0], 429  );
		DrawLine( 3 + iLineR[0], 4, 3 + iLineR[0], 429  );
		DrawLine( 4 + iLineR[0], 4, 4 + iLineR[0], 429  );
	}
	else
	{
		DrawLine( 499, 4, 499, 429  );
		DrawLine( 500, 4, 500, 429  );
		DrawLine( 501, 4, 501, 429  );
	}	
	
	if( iLineR[1] < ECHO_PACKAGE_SIZE ) 
		DrawLine( 2 + iLineR[1], 4, 2 + iLineR[1], 429  );
	else
		DrawLine( 501, 4, 501, 429  );
	
	MSetDisplayColor( 0x1F );
	if( iLineB[0] >= iLineStart && iLineB[0] <= (iLineStart+iMaxLine) ) 
		DrawLine( 2, 105 + iLineB[0] - iLineStart, 501, 105 + iLineB[0] - iLineStart  );
	if( iLineB[1] >= iLineStart && iLineB[1] <= (iLineStart+iMaxLine) ) 
		DrawLine( 2, 105 + iLineB[1] - iLineStart, 501, 105 + iLineB[1] - iLineStart  );
}

bool LoadTofdFile()
{
	int i = 0;
	char szkey[32];
	int keycode;
	
	FILINFO fi;
	DIR dirinfo;
	int iFileCount = 0;
	char fname[1000][13];
	int iStartFile = 0;
	int iPt = 105, is = 30;
	bool bUpdate = true;
	
	EraseWindow( 2, 4, 500, 100 );
	EraseWindow( 2, 105, 500, 324 );
	MSetDisplayColor( 0xFFFF );
	
	if (DisplayQuery(14))	//�����Ӻ�U�̣�
	{
		DisplayPrompt(19);	//��������U��
		if(UDiskInitialize(3))
		{
			DisplayPrompt(20);	//U�����ӳɹ�
			g_UDiskInfo.DataHeaderMark = 1;

			f_opendir(&dirinfo,"/PXUTTOFD");
			//f_opendir(&dirinfo,"/PXUTDATA");

			while( f_readdir(&dirinfo, &fi) == 0 )
			{
				
				if( strcmp(fi.fname,"") )
				{
					for( i = 0; i < 10; i++ )
					{
						if( fi.fname[i] == '.' && 
							(fi.fname[i+1] == 't' || fi.fname[i+1] == 'T') && 
							(fi.fname[i+2] == 'o' || fi.fname[i+2] == 'O') &&
							(fi.fname[i+3] == 'f' || fi.fname[i+3] == 'F')
						  )
						{
							strcpy( fname[iFileCount++], fi.fname );
							break;
						}
						else if(fi.fname[i] == 0)
							break;
					}
				}
				else
					break;
			}
			
			sprintf(szkey, "�ļ�����:%d", iFileCount);
			TextOut( 10, 10, 1, 170, 40, szkey, 4 );
			TextOut( 10, 40, 1, 170, 70, "�����ּ�ѡ���ļ���+/-����ҳ", 4 );
			
			while(true)
			{
				if( bUpdate )
				{
					EraseWindow( 2, 105, 500, 324 );
					if( iStartFile + 10 < iFileCount )
					{
						iPt = 105;
						for( i = 0; i < 10; i++ )
						{
							sprintf( szkey, "%d: %s", i, fname[iStartFile+i] );
							TextOut( 10, iPt, 1, 500, iPt+is-1, szkey, 4 );
							iPt += is;
						}
					}
					else
					{
						iPt = 105;
						for( i = 0; i < iFileCount - iStartFile; i++ )
						{
							sprintf( szkey, "%d: %s", i, fname[iStartFile+i] );
							TextOut( 10, iPt, 1, 500, iPt+is-1, szkey, 4 );
							iPt += is;
						}
					}
					
					bUpdate = false;
				}

				keycode = MGetKeyCode( 0 );
				
				if( keycode >=0 && keycode <= 9 )
				{
					if( iStartFile + keycode < iFileCount )
					{
						sprintf(szkey, "%s", fname[iStartFile+keycode]);
						FRESULT res = f_open(&g_FileObject, szkey, FA_OPEN_EXISTING|FA_WRITE|FA_READ);	
						if( res == FR_OK )
						{
							f_lseek( &g_FileObject, 0 );
							f_read( &g_FileObject, &g_iLine, sizeof(int), NULL );
							f_read( &g_FileObject, &g_iRang, sizeof(u_int), NULL );
							f_read( &g_FileObject, &g_iDelay, sizeof(u_int), NULL );
							f_read(&g_FileObject, &g_iPCS, sizeof(int), NULL);
							f_read(&g_FileObject, &g_iTofdFreq, sizeof(int), NULL);
							f_read(&g_FileObject, &g_iEncValue, sizeof(long), NULL);
										
							for( i = 0; i <= g_iLine; i++ )
							{
								f_read(&g_FileObject, g_pEcho[i], ECHO_PACKAGE_SIZE, NULL);
							}
							f_close(&g_FileObject);
							return true;
						}	
						else 
						{
							strcat( szkey, " ��ȡʧ��! �����������!" );
							TextOut( 10, 10, 1, 170, 40, szkey, 4 );
							MAnyKeyReturn();
							return false;
						}
					}
				}
				else if( keycode == 13 )
				{
					if( iStartFile != 0 )
					{
						iStartFile -= 10;
						if( iStartFile < 0 )
							iStartFile = 0;
						bUpdate = true;
					}
				}
				else if( keycode == 14 )
				{
					if( iStartFile + 10 < iFileCount )
					{
						iStartFile += 10;
						bUpdate = true;
					}
				}
				else if( keycode == C_KEYCOD_CONFIRM || keycode == C_KEYCOD_RETURN )
				{
					return false;
				}
			}
		}
		else 
			return false;
	}
	else 
		return false;
}

void DAFunc()
{
	char  szkey[64];
	int   iStep = 1;
	int   iNumber = 0, keycode;
	short xpos, ypos, i, j, iMaxLine, iLineStart = 0;
	short iLineR[2] = {0, 20}, iLineB[2] = {2 , 20};
	short clrR, clrG, clrB, clr;
	short iIndex = 0, iIndexR = 0, iIndexB = 0;
	
	iLineR[0] = (g_iPCS/2 - g_iRang) * ECHO_PACKAGE_SIZE / g_iDelay/10;
	
	if( iLineR[0] < 0 )
		iLineR[0] = 0;
	//������д�������	
	EraseWindow( 0, 0, C_HORIDOT_SCREEN, C_VERTDOT_SCREEN );

	//���ͼ����
	MSetDisplayColor( 0x3F << 5 );
	EraseDrawRectangle( 1, 1, 502, 477 );
	
	//���ܰ�����
	DrawLine( 1, 430, 502, 430 );
	DrawLine( 126, 430, 126, 477 );
	DrawLine( 251, 430, 251, 477 );
	DrawLine( 376, 430, 376, 477 );

	MSetDisplayColor( 0xFFFF );
	sprintf( szkey, "����(%02d)", iIndexR + 1 );
	TextOut( 6, 440, 1, 125, 470, szkey, 4 );
	MSetDisplayColor( 0x3F << 5 );
	sprintf( szkey, "����(%02d)", iIndexB + 1 );
	TextOut( 132, 440, 1, 250, 470, szkey, 4 );
	sprintf( szkey, "�����ļ�" );
	TextOut( 265, 440, 1, 375, 470, szkey, 4 );
	sprintf( szkey, "����(%02d)", iStep );
	TextOut( 382, 440, 1, 501, 470, szkey, 4 );
	
	if( g_iLine > 324 )
		iMaxLine = 324;
	else
		iMaxLine = g_iLine;
	
	DADraw( iIndex, iLineStart, iLineR, iLineB, iIndexB );

	while( true )
	{
		keycode = MGetKeyCode( 0 );

		if( keycode == C_KEYCOD_CONFIRM )
		{
			g_iDelay = g_iPCS/2 - g_iRang*iLineR[0]/ECHO_PACKAGE_SIZE; 
			DADraw( iIndex, iLineStart, iLineR, iLineB, iIndexB );
		}
		else if( keycode == C_KEYCOD_RETURN )
		{
			break;
		}
		else if( keycode == 13 )
		{
			if( iIndex == 0 )
			{
				if( iLineR[iIndexR] - iStep < 0 )
					iLineR[iIndexR] = 0;
				else 
					iLineR[iIndexR] -= iStep;
				
				DADraw( iIndex, iLineStart, iLineR, iLineB, iIndexB );
			}
			else if( iIndex == 1 )
			{
				if( iLineB[iIndexB] - iStep < 0 )
					iLineB[iIndexB] = 0;
				else 
					iLineB[iIndexB] -= iStep;
				
				if( iLineB[iIndexB] < iLineStart )
					iLineStart = iLineB[iIndexB];
				
				DADraw( iIndex, iLineStart, iLineR, iLineB, iIndexB );
			}
		}
		else if( keycode == 14 )
		{
			if( iIndex == 0 )
			{
				if( iLineR[iIndexR] + iStep > ECHO_PACKAGE_SIZE )
					iLineR[iIndexR] = ECHO_PACKAGE_SIZE;
				else 
					iLineR[iIndexR] += iStep;
					
				DADraw( iIndex, iLineStart, iLineR, iLineB, iIndexB );
			}
			else if( iIndex == 1 )
			{
				if( iLineB[iIndexB] + iStep > g_iLine )
					iLineB[iIndexB] = g_iLine;
				else 
					iLineB[iIndexB] += iStep;
				
				if( iLineB[iIndexB] > iLineStart + 324 )
					iLineStart += iLineB[iIndexB] - (iLineStart + 324);
				else if( iLineB[iIndexB] < iLineStart )
					iLineStart = iLineB[iIndexB];
				
				DADraw( iIndex, iLineStart, iLineR, iLineB, iIndexB );
			}
		}
		else if( keycode == 16 )
		{
			if( iIndex != 0 )
			{
				iIndex = 0;
				DADraw( iIndex, iLineStart, iLineR, iLineB, iIndexB );
			}
			else
			{
				iIndexR = ++iIndexR % 2;
			}
			
			MSetDisplayColor( 0xFFFF );
			sprintf( szkey, "����(%02d)", iIndexR+1 );
			TextOut( 6, 440, 1, 125, 470, szkey, 4 );
			MSetDisplayColor( 0x3F << 5 );
			sprintf( szkey, "����(%02d)", iIndexB+1 );
			TextOut( 132, 440, 1, 250, 470, szkey, 4 );
			sprintf( szkey, "�����ļ�" );
			TextOut( 265, 440, 1, 375, 470, szkey, 4 );
			sprintf( szkey, "����(%02d)", iStep );
			TextOut( 382, 440, 1, 501, 470, szkey, 4 );
		}
		else if( keycode == 17 )
		{
			if( iIndex != 1 )
			{
				iIndex = 1;
			}
			else
			{
				iIndexB = ++iIndexB % 2;	
			}
			
			DADraw( iIndex, iLineStart, iLineR, iLineB, iIndexB );
			
			MSetDisplayColor( 0xFFFF );
			sprintf( szkey, "����(%02d)", iIndexB+1 );
			TextOut( 132, 440, 1, 250, 470, szkey, 4 );
			MSetDisplayColor( 0x3F << 5 );
			sprintf( szkey, "����(%02d)", iIndexR+1 );
			TextOut( 6, 440, 1, 125, 470, szkey, 4 );
			sprintf( szkey, "�����ļ�" );
			TextOut( 265, 440, 1, 375, 470, szkey, 4 );
			sprintf( szkey, "����(%02d)", iStep );
			TextOut( 382, 440, 1, 501, 470, szkey, 4 );
		}
		else if( keycode == 18 )
		{
			LoadTofdFile();
			DADraw( iIndex, iLineStart, iLineR, iLineB, iIndexB );
		}
		else if( keycode == 19 )
		{
			if( iStep == 1 )
				iStep = 5;
			else if( iStep == 5 )
				iStep = 10;
			else if( iStep == 10 )
				iStep = 20;
			else if( iStep == 20 )
				iStep = 50;
			else
				iStep = 1;
			
			MSetDisplayColor( 0x3F << 5 );
			sprintf( szkey, "����(%02d)", iStep );
			TextOut( 382, 440, 1, 501, 470, szkey, 4 );
		}
	}
}

//TOFDɨ��
void TOFDFunc(void)
{
	char  szkey[32];
	int   keycode, iIndex = 1;
	int   i = 0;
	int iX0 = 0;
	int iY0 = C_COORVPOSI + C_COORHEIGHT - C_COORHEIGHT/2 + 10;
	int iX1 = ECHO_PACKAGE_SIZE;
	int iY1 = C_COORVPOSI + C_COORHEIGHT;
	
	iTofdMode = 1;									//����TOFDģʽ
	g_iPCS = ChannelPara.iPcs;
	
	MSetAcquisition(1);		
	EnableEchoDisplay( 0 ) ;
	//EraseWindow(C_COORHPOSI, 0, 495, C_VERTDOT_VIDEO );	//�����ͼ��ͼ��
	EraseWindow( 0, 0, C_HORIDOT_SCREEN, C_VERTDOT_SCREEN );
	DrawFuncMenu( iIndex );

	while( true )
	{
		keycode = MGetKeyCode( 20 );

		if( keycode == 16 )
		{
			iIndex = 0;
			DrawFuncMenu( iIndex );
		}
		else if( keycode == 17 )
		{
			iIndex = 1;
			DrawFuncMenu( iIndex );
		}
		else if( keycode == 18 )
		{
			//iIndex = 2;
			//DrawFuncMenu( iIndex );
			BScanEx();
			EraseWindow( 0, 0, C_HORIDOT_SCREEN, C_VERTDOT_SCREEN );
			iIndex = 1;
			DrawFuncMenu( iIndex );
		}
		else if( keycode == 19 )
		{
			//iIndex = 3;
			//DrawFuncMenu( iIndex );
			DAFunc();
			//������д�������	
			EraseWindow( 0, 0, C_HORIDOT_SCREEN, C_VERTDOT_SCREEN );
			iIndex = 1;
			DrawFuncMenu( iIndex );
		}
		
		if( keycode >= 0 && keycode <= 9 )
		{
			if( iIndex == 0 )
			{
				CalibrationFunc( keycode );
			}
			else if( iIndex == 1 )
			{
				SetFunc( keycode );
			}
			
			MSetSystem();
			EraseWindow( 0, 0, C_HORIDOT_SCREEN, C_VERTDOT_SCREEN );
			//EraseWindow(C_COORHPOSI, 0, 495, C_VERTDOT_VIDEO );	//�����ͼ��ͼ��
			DrawFuncMenu( iIndex );
		}
		
		if( keycode == C_KEYCOD_CONFIRM )
		{
			break;
		}
		if( keycode == C_KEYCOD_RETURN )
		{
			break;
		}
	}
	
	iTofdMode = 0;
	//��ԭAɨ��ʾ����
	SetEchoLayout( C_COORHPOSI, C_COORHPOSI + 500, C_COORVPOSI + VertOffsetScreen, 0 );
	EnableEchoDisplay( 1 ) ;
	
	MSetAcquisitionEnable(1,C_SETMODE_SETSAVE);
    MSetHardEchoShow(1,C_SETMODE_SETSAVE);
    ScreenRenovate();	/*��Ļˢ��*/
    DrawDac(0);		//�»�DAC
}

void DrawAxisDot()
{
	//��������
	int i = 0;
	u_int iRange = 0, iDelay = 0;
	char  szkey[32];
	
	MSetDisplayColor( 0xFFE0 );
	
	for( i = 0; i < 10; i++ )
	{
		DrawPixel(102, 5 + i * 10);
		DrawPixel(202, 5 + i * 10);
		DrawPixel(302, 5 + i * 10);
		DrawPixel(402, 5 + i * 10);
		//DrawLine( 102, 5 + i * 10, 102, 5 + i * 10 + 1 );
		//DrawLine( 202, 5 + i * 10, 202, 5 + i * 10 + 1 );
		//DrawLine( 302, 5 + i * 10, 302, 5 + i * 10 + 1 );
		//DrawLine( 402, 5 + i * 10, 402, 5 + i * 10 + 1 );
	}
	
	for( i = 0; i < 50; i++ )
	{
		DrawPixel(3 + i * 10, 23);
		DrawPixel(3 + i * 10, 43);
		DrawPixel(3 + i * 10, 63);
		DrawPixel(3 + i * 10, 83);
		//DrawLine( 3 + i * 10, 23, 3 + i * 10 + 1, 23 );
		//DrawLine( 3 + i * 10, 43, 3 + i * 10 + 1, 43 );
		//DrawLine( 3 + i * 10, 63, 3 + i * 10 + 1, 63 );
		//DrawLine( 3 + i * 10, 83, 3 + i * 10 + 1, 83 );
	}
	
	iDelay = MGetDelay(3);
	iRange = MGetRange(3);
	sprintf( szkey, "%d.%d ", iDelay/10, iDelay%10 );
	TextOut( 2, 105, 1, 101, 135, szkey, 4 );
	sprintf( szkey, "%d.%d ", (iDelay+iRange/5)/10, (iDelay+iRange/5)%10 );
	TextOut( 103, 105, 1, 201, 135, szkey, 4 );
	sprintf( szkey, "%d.%d ", (iDelay+iRange*2/5)/10, (iDelay+iRange*2/5)%10 );
	TextOut( 203, 105, 1, 301, 135, szkey, 4 );
	sprintf( szkey, "%d.%d ", (iDelay+iRange*3/5)/10, (iDelay+iRange*3/5)%10 );
	TextOut( 303, 105, 1, 401, 135, szkey, 4 );
	sprintf( szkey, "%d.%d ", (iDelay+iRange*4/5)/10, (iDelay+iRange*4/5)%10 );
	TextOut( 403, 105, 1, 501, 135, szkey, 4 );
}

void DrawTofdBtn( int iBtn[4], int iStep[3], int iIndexFunc )
{
	char  szkey[32];
	int iPt = 105, is = 30;
	
	MSetDisplayColor( 0x3F << 5 );
	if( iBtn[0] == 0 )
	{	
		TextOut( 6, 440, 1, 125, 470, "       ", 4 );	
		TextOut( 6, 440, 1, 125, 470, "��ʼɨ��", 4 );	
	}
	else
	{
		TextOut( 6, 440, 1, 125, 470, "       ", 4 );	
		TextOut( 6, 440, 1, 125, 470, "ֹͣɨ��", 4 );
	}	
	
	TextOut( 132, 440, 1, 250, 470, "�����л�", 4 );
	TextOut( 265, 440, 1, 375, 470, "��������", 4 );
	MSetDisplayColor( 0x1F<<11 );
	TextOut( 400, 440, 1, 501, 470, "�˳�", 4 );
	
	if( iIndexFunc == 0 )
	{
		MSetDisplayColor( 0x3F << 5 );
		TextOut( 514, iPt, 1, C_HORIDOT_SCREEN-1, iPt+is-1, "���� dB", 4 );
		iPt += is;
		sprintf( szkey, "%d.%d ",MGetBaseGain()/10, MGetBaseGain()%10 );
		TextOut( 534, iPt, 1, C_HORIDOT_SCREEN-1, iPt+is-1, szkey, 4 );
		iPt += is;
		sprintf( szkey, "%d.%d ",iStep[0]/10, iStep[0]%10 );
		TextOut( 534, iPt, 1, C_HORIDOT_SCREEN-1, iPt+is-1, szkey, 4 );
		iPt += is;
		TextOut( 514, iPt, 1, C_HORIDOT_SCREEN-1, iPt+is-1, "���� mm", 4 );
		iPt += is;
		sprintf( szkey, "%d.%d ", MGetRange(3)/10, MGetRange(3)%10 );
		TextOut( 534, iPt, 1, C_HORIDOT_SCREEN-1, iPt+is-1, szkey, 4 );
		iPt += is;
		sprintf( szkey, "%d.%d ",iStep[1]/10, iStep[1]%10 );
		TextOut( 534, iPt, 1, C_HORIDOT_SCREEN-1, iPt+is-1, szkey, 4 );
		iPt += is;
		TextOut( 514, iPt, 1, C_HORIDOT_SCREEN-1, iPt+is-1, "��ʱ mm", 4 );
		iPt += is;
		sprintf( szkey, "%d.%d ", MGetDelay(3)/10, MGetDelay(3)%10 );
		TextOut( 534, iPt, 1, C_HORIDOT_SCREEN-1, iPt+is-1, szkey, 4 );
		iPt += is;
		sprintf( szkey, "%d.%d ",iStep[2]/10, iStep[2]%10 );
		TextOut( 534, iPt, 1, C_HORIDOT_SCREEN-1, iPt+is-1, szkey, 4 );
	}
	else if( iIndexFunc == 1 )
	{
		MSetDisplayColor( 0xFFFF );
		TextOut( 514, iPt, 1, C_HORIDOT_SCREEN-1, iPt+is-1, "���� dB", 4 );
		iPt += is;
		sprintf( szkey, "%d.%d ",MGetBaseGain()/10, MGetBaseGain()%10 );
		TextOut( 534, iPt, 1, C_HORIDOT_SCREEN-1, iPt+is-1, szkey, 4 );
		iPt += is;
		sprintf( szkey, "%d.%d ",iStep[0]/10, iStep[0]%10 );
		TextOut( 534, iPt, 1, C_HORIDOT_SCREEN-1, iPt+is-1, szkey, 4 );
		iPt += is;
		MSetDisplayColor( 0x3F << 5 );
		TextOut( 514, iPt, 1, C_HORIDOT_SCREEN-1, iPt+is-1, "���� mm", 4 );
		iPt += is;
		sprintf( szkey, "%d.%d ", MGetRange(3)/10, MGetRange(3)%10 );
		TextOut( 534, iPt, 1, C_HORIDOT_SCREEN-1, iPt+is-1, szkey, 4 );
		iPt += is;
		sprintf( szkey, "%d.%d ",iStep[1]/10, iStep[1]%10 );
		TextOut( 534, iPt, 1, C_HORIDOT_SCREEN-1, iPt+is-1, szkey, 4 );
		iPt += is;
		TextOut( 514, iPt, 1, C_HORIDOT_SCREEN-1, iPt+is-1, "��ʱ mm", 4 );
		iPt += is;
		sprintf( szkey, "%d.%d ", MGetDelay(3)/10, MGetDelay(3)%10 );
		TextOut( 534, iPt, 1, C_HORIDOT_SCREEN-1, iPt+is-1, szkey, 4 );
		iPt += is;
		sprintf( szkey, "%d.%d ",iStep[2]/10, iStep[2]%10 );
		TextOut( 534, iPt, 1, C_HORIDOT_SCREEN-1, iPt+is-1, szkey, 4 );
	}
	else if( iIndexFunc == 2 )
	{
		MSetDisplayColor( 0x3F << 5 );
		TextOut( 514, iPt, 1, C_HORIDOT_SCREEN-1, iPt+is-1, "���� dB", 4 );
		iPt += is;
		sprintf( szkey, "%d.%d ",MGetBaseGain()/10, MGetBaseGain()%10 );
		TextOut( 534, iPt, 1, C_HORIDOT_SCREEN-1, iPt+is-1, szkey, 4 );
		iPt += is;
		sprintf( szkey, "%d.%d ",iStep[0]/10, iStep[0]%10 );
		TextOut( 534, iPt, 1, C_HORIDOT_SCREEN-1, iPt+is-1, szkey, 4 );
		iPt += is;
		MSetDisplayColor( 0xFFFF );
		TextOut( 514, iPt, 1, C_HORIDOT_SCREEN-1, iPt+is-1, "���� mm", 4 );
		iPt += is;
		sprintf( szkey, "%d.%d ", MGetRange(3)/10, MGetRange(3)%10 );
		TextOut( 534, iPt, 1, C_HORIDOT_SCREEN-1, iPt+is-1, szkey, 4 );
		iPt += is;
		sprintf( szkey, "%d.%d ",iStep[1]/10, iStep[1]%10 );
		TextOut( 534, iPt, 1, C_HORIDOT_SCREEN-1, iPt+is-1, szkey, 4 );
		iPt += is;
		MSetDisplayColor( 0x3F << 5 );
		TextOut( 514, iPt, 1, C_HORIDOT_SCREEN-1, iPt+is-1, "��ʱ mm", 4 );
		iPt += is;
		sprintf( szkey, "%d.%d ", MGetDelay(3)/10, MGetDelay(3)%10 );
		TextOut( 534, iPt, 1, C_HORIDOT_SCREEN-1, iPt+is-1, szkey, 4 );
		iPt += is;
		sprintf( szkey, "%d.%d ",iStep[2]/10, iStep[2]%10 );
		TextOut( 534, iPt, 1, C_HORIDOT_SCREEN-1, iPt+is-1, szkey, 4 );
	}
	else if( iIndexFunc == 3 )
	{
		MSetDisplayColor( 0x3F << 5 );
		TextOut( 514, iPt, 1, C_HORIDOT_SCREEN-1, iPt+is-1, "���� dB", 4 );
		iPt += is;
		sprintf( szkey, "%d.%d ",MGetBaseGain()/10, MGetBaseGain()%10 );
		TextOut( 534, iPt, 1, C_HORIDOT_SCREEN-1, iPt+is-1, szkey, 4 );
		iPt += is;
		sprintf( szkey, "%d.%d ",iStep[0]/10, iStep[0]%10 );
		TextOut( 534, iPt, 1, C_HORIDOT_SCREEN-1, iPt+is-1, szkey, 4 );
		iPt += is;
		TextOut( 514, iPt, 1, C_HORIDOT_SCREEN-1, iPt+is-1, "���� mm", 4 );
		iPt += is;
		sprintf( szkey, "%d.%d ", MGetRange(3)/10, MGetRange(3)%10 );
		TextOut( 534, iPt, 1, C_HORIDOT_SCREEN-1, iPt+is-1, szkey, 4 );
		iPt += is;
		sprintf( szkey, "%d.%d ",iStep[1]/10, iStep[1]%10 );
		TextOut( 534, iPt, 1, C_HORIDOT_SCREEN-1, iPt+is-1, szkey, 4 );
		iPt += is;
		MSetDisplayColor( 0xFFFF );
		TextOut( 514, iPt, 1, C_HORIDOT_SCREEN-1, iPt+is-1, "��ʱ mm", 4 );
		iPt += is;
		sprintf( szkey, "%d.%d ", MGetDelay(3)/10, MGetDelay(3)%10 );
		TextOut( 534, iPt, 1, C_HORIDOT_SCREEN-1, iPt+is-1, szkey, 4 );
		iPt += is;
		sprintf( szkey, "%d.%d ",iStep[2]/10, iStep[2]%10 );
		TextOut( 534, iPt, 1, C_HORIDOT_SCREEN-1, iPt+is-1, szkey, 4 );
	}
}

void BScanEx(void)
{
	int iBtn[4] = {0,0,0,0};
	int iStep[3] = {10,10,10};
	short xpos, ypos, xposOld, yposOld, i, j, keycode;
	int iPt = 3, is = 30;
	bool bFirst = true;
	int iIndexFunc = 0;
	u_char* buffer;
	u_char sampbuffer[ECHO_PACKAGE_SIZE], sampbufferOld[ECHO_PACKAGE_SIZE];
	char  szkey[64];
	
	short clrR, clrG, clrB, clr;
	u_int iRotaryValue   =  0;
	int   iEncoder       =  0;
	int   iLine          =  0;
	int   iLineStart     =  0;
	int   iLineDrawCount =  0;
	int   iLineLast      = -1;
	
	//������д�������	
	EraseWindow( 0, 0, C_HORIDOT_SCREEN, C_VERTDOT_SCREEN );

	//���ͼ����
	MSetDisplayColor( 0x3F << 5 );
	EraseDrawRectangle( 1, 1, 502, 477 );
	
	//���ܰ�����
	DrawLine( 1, 430, 502, 430 );
	DrawLine( 126, 430, 126, 477 );
	DrawLine( 251, 430, 251, 477 );
	DrawLine( 376, 430, 376, 477 );

	MSetDisplayColor( 0x20 << 5 );
	//Aɨ�ָ���
	DrawLine( 1, 104, 502, 104 );
	
	MSetDisplayColor( 0x3F << 5 );
	//Aɨ����̶ȷָ���
	DrawLine( 1, 134, 502, 134 );
	
	//�ָ�ֱ��
	iPt = 3;
	DrawLine( 502, iPt, C_HORIDOT_SCREEN-1, iPt );
	iPt = 104;
	DrawLine( 502, iPt, C_HORIDOT_SCREEN-1, iPt );
	iPt = 105;
	DrawLine( 502, iPt, C_HORIDOT_SCREEN-1, iPt );
	iPt = 106;
	DrawLine( 502, iPt, C_HORIDOT_SCREEN-1, iPt );
	iPt += is;
	//DrawLine( 502, iPt, C_HORIDOT_SCREEN-1, iPt );
	iPt += is;
	//DrawLine( 502, iPt, C_HORIDOT_SCREEN-1, iPt );
	iPt += is;
	DrawLine( 502, iPt, C_HORIDOT_SCREEN-1, iPt );
	iPt += is;
	//DrawLine( 502, iPt, C_HORIDOT_SCREEN-1, iPt );
	iPt += is;
	//DrawLine( 502, iPt, C_HORIDOT_SCREEN-1, iPt );
	iPt += is;
	DrawLine( 502, iPt, C_HORIDOT_SCREEN-1, iPt );
	iPt += is;
	//DrawLine( 502, iPt, C_HORIDOT_SCREEN-1, iPt );
	iPt += is;
	//DrawLine( 502, iPt, C_HORIDOT_SCREEN-1, iPt );
	iPt += is;
	DrawLine( 502, iPt, C_HORIDOT_SCREEN-1, iPt );
	iPt += 1;
	DrawLine( 502, iPt, C_HORIDOT_SCREEN-1, iPt );
	iPt += 1;
	DrawLine( 502, iPt, C_HORIDOT_SCREEN-1, iPt );
	//iPt += is;
	//DrawLine( 502, iPt, C_HORIDOT_SCREEN-1, iPt );
	//iPt += is;
	//DrawLine( 502, iPt, C_HORIDOT_SCREEN-1, iPt );
	//iPt += is;
	//DrawLine( 502, iPt, C_HORIDOT_SCREEN-1, iPt );
	//iPt += is;
	//DrawLine( 502, iPt, C_HORIDOT_SCREEN, iPt );
	//����
	DrawLine( 502, 477, C_HORIDOT_SCREEN-1, 477 );
	DrawLine( 502, 478, C_HORIDOT_SCREEN-1, 478 );
	DrawLine( 502, 479, C_HORIDOT_SCREEN-1, 479 );
	
	MSetDisplayColor( 0xFFE0 );
	TextOut( 514, 13, 1, C_HORIDOT_SCREEN-1, 43, "λ�� mm", 4 );
	TextOut( 534, 53, 1, C_HORIDOT_SCREEN, 83, "0.0   ", 4 );
	
	DrawAxisDot();
	DrawTofdBtn( iBtn, iStep, iIndexFunc );
	
	MSetProbeMode(2,C_SETMODE_SAVE);			//���ó�˫��̽ͷ
	MSetEchoMode(3,C_SETMODE_SETSAVE);			//��Ƶģʽ
	
	while( true )
	{
		//��ȡAɨ����
		buffer = GetSampleBuffer();
		memcpy( sampbuffer, buffer, ECHO_PACKAGE_SIZE );
		
		//����Aɨ���Σ���ʼʱֱ�ӻ��ƣ�����ֻ���Ʋ���ͬ�Ĳ���
		if( bFirst )
		{
			EraseWindow( 2, 4, 500, 100 );
			DrawAxisDot();
			MSetDisplayColor( 0xFFE0 );
			xposOld = 2; 
			yposOld = 103 - (sampbuffer[0] * 99 / 255);
			for( j = 1; j < ECHO_PACKAGE_SIZE; j++ )
			{
				xpos = 2 + j;
				ypos = 103 - (sampbuffer[j] * 99 / 255);	
				if( ypos )
				DrawLine( xposOld, yposOld, xpos, ypos );
				xposOld = xpos; 
				yposOld = ypos;
			}
			memcpy( sampbufferOld, sampbuffer, ECHO_PACKAGE_SIZE );
			bFirst = false;
		}
		else
		{
			for( j = 1; j < ECHO_PACKAGE_SIZE; j++ )
			{
				if( (sampbuffer[j] != sampbufferOld[j]) || (sampbuffer[j-1] != sampbufferOld[j-1]) )
				{
					//���ԭͼ����ʾ
					MSetDisplayColor( 0 );
					xposOld = 1 + j;
					yposOld = 103 - (sampbufferOld[j-1] * 99 / 255);	
					xpos = 2 + j;
					ypos = 103 - (sampbufferOld[j] * 99 / 255);		
					DrawLine( xposOld, yposOld, xpos, ypos );
					//�����µ�ͼ��
					MSetDisplayColor( 0xFFE0 );
					xposOld = 1 + j;
					yposOld = 103 - (sampbuffer[j-1] * 99 / 255);	
					xpos = 2 + j;
					ypos = 103 - (sampbuffer[j] * 99 / 255);		
					DrawLine( xposOld, yposOld, xpos, ypos );
				}
			}
			memcpy( sampbufferOld, sampbuffer, ECHO_PACKAGE_SIZE );
		}
		
		//��ʼTOFDɨ��
		if( iBtn[0] == 1 )
		{
			iRotaryValue = GetScanRotaryValue( iEncoder );
			
			if( iRotaryValue > 60000 )
			{
				SetScanRotaryEncoder( iEncoder, 1, 1, 1 );
				SetScanRotaryEncoder( iEncoder, 1, 0, 1 );
				continue;
			}
			
			iLine = iRotaryValue /g_iTofdFreq;

			if( iLineLast != iLine )
			{
				MSetDisplayColor( 0xFFE0 );
				if( iLine >= MAX_LINE )
				{
					TextOut( 514, 13, 1, C_HORIDOT_SCREEN-1, 43, "λ�� mm", 4 );
					TextOut( 534, 53, 1, C_HORIDOT_SCREEN, 83, "MAX  ", 4 );
					iLineLast = iLine;
					continue;
				}	
				else
				{
					TextOut( 514, 13, 1, C_HORIDOT_SCREEN-1, 43, "λ�� mm", 4 );
					sprintf( szkey, "%d.%d ", (iLine * g_iTofdFreq * g_iEncValue)/100, ((iLine * g_iTofdFreq * g_iEncValue)%100)/10 );
					if( iLine == 0 )
						TextOut( 534, 53, 1, C_HORIDOT_SCREEN, 83, "0.0   ", 4 );
					else
						TextOut( 534, 53, 1, C_HORIDOT_SCREEN, 83, szkey, 4 );
				}
				//��ȡAɨ����
				buffer = GetSampleBuffer();
				memcpy( g_pEcho[iLine], buffer, ECHO_PACKAGE_SIZE );
				if( g_iLine < iLine )
					g_iLine = iLine;

				if( g_iLine >= 294 )
				{
					if( g_iLine - iLine < 294 )
					{
						iLineStart = g_iLine - 294;
					}
					else
					{
						iLineStart = iLine;
					}
					
					iLineDrawCount = 294;
				}
				else
				{
					iLineStart = 0;
					iLineDrawCount = g_iLine;
				}
					
				for( i = iLineStart; i <= iLineStart + iLineDrawCount; i++ )
				{
					if( i == iLine )
					{
						MSetDisplayColor( 0xFFE0 );
						DrawLine( 2, 135 + i - iLineStart, 501, 135 + i - iLineStart );
					}
					else
					{
						iRotaryValue = GetScanRotaryValue( iEncoder );
						//ɨ�賤��С����ʾʱֱ�Ӱ�ɨ�������ʾ
						iLine = iRotaryValue /g_iTofdFreq;
			
						if( iLine < MAX_LINE )
						{
							buffer = GetSampleBuffer();
							memcpy( g_pEcho[iLine], buffer, ECHO_PACKAGE_SIZE );
							if( g_iLine < iLine )
								g_iLine = iLine;
						}
						
						for( j = 0; j < ECHO_PACKAGE_SIZE; j++ )
						{
							//R,G,B��ֵ��ͬʱΪ�ң�������ֵԽ����ɫԽ��
							clrR = g_pEcho[i][j] * 0x1F / 0xFF;
							clrG = g_pEcho[i][j] * 0x3F / 0xFF;
							clrB = g_pEcho[i][j] * 0x1F / 0xFF;
							clr  = ((0x1F & clrR) << 11) | ((0x3F & clrG) << 5) | (0x1F & clrB);
							//���û�����ɫ
							SetDisplayColor( clr );
							xpos = j + 2;
							ypos = 135 + i - iLineStart;
							DrawPixel( xpos, ypos );
						}
					}
				}
				
				iLineLast = iLine;
			}
		}
		
		//��ⰴ��
		keycode = MGetKeyCode( 0 );
		
		//-����
		if( keycode == 13 )
		{
			if( iIndexFunc == 1 )
			{
				int basegain = MGetBaseGain() - iStep[0];
				MSetBaseGain( basegain, C_SETMODE_SAVE );
				MSetSystem();
				SetDisplayColor( 0xFFFF );
				sprintf( szkey, "%d.%d ", MGetBaseGain()/10, MGetBaseGain()%10 );
				TextOut( 534, 136, 1, C_HORIDOT_SCREEN-1, 165, szkey, 4 );
			}
			else if( iIndexFunc == 2 )
			{
				int iRang = MGetRange(3) - iStep[1];
				if( iRang < 90 )
					iRang = 90;
				MSetRange( iRang, C_SETMODE_SETSAVE );
				SetDisplayColor( 0xFFFF );
				sprintf( szkey, "%d.%d ", iRang/10, iRang%10 );
				TextOut( 534, 226, 1, C_HORIDOT_SCREEN-1, 255, szkey, 4 );
			}
			else if( iIndexFunc == 3 )
			{
				MSetScaleDelay( MGetDelay(3) - iStep[2], C_SETMODE_SETSAVE );
				SetDisplayColor( 0xFFFF );
				sprintf( szkey, "%d.%d ", MGetDelay(3)/10, MGetDelay(3)%10 );
				TextOut( 534, 316, 1, C_HORIDOT_SCREEN-1, 345, szkey, 4 );
			}
	
			bFirst = true;
		}
		//+����
		else if( keycode == 14 )
		{
			if( iIndexFunc == 1 )
			{
				MSetBaseGain( MGetBaseGain()+iStep[0], C_SETMODE_SAVE );
				MSetSystem();
				SetDisplayColor( 0xFFFF );
				sprintf( szkey, "%d.%d ", MGetBaseGain()/10, MGetBaseGain()%10 );
				TextOut( 534, 136, 1, C_HORIDOT_SCREEN-1, 165, szkey, 4 );
			}
			else if( iIndexFunc == 2 )
			{
				MSetRange( MGetRange(3) +iStep[1], C_SETMODE_SETSAVE );
				SetDisplayColor( 0xFFFF );
				sprintf( szkey, "%d.%d ", MGetRange(3)/10, MGetRange(3)%10 );
				TextOut( 534, 226, 1, C_HORIDOT_SCREEN-1, 255, szkey, 4 );
			}
			else if( iIndexFunc == 3 )
			{
				MSetScaleDelay( MGetDelay(3) + iStep[2], C_SETMODE_SETSAVE );
				SetDisplayColor( 0xFFFF );
				sprintf( szkey, "%d.%d ", MGetDelay(3)/10, MGetDelay(3)%10 );
				TextOut( 534, 316, 1, C_HORIDOT_SCREEN-1, 345, szkey, 4 );
			}
			
			bFirst = true;	
		}
		else if( keycode == 0 )
		{
			if( iIndexFunc != 1 )
			{
				iIndexFunc = 1;
			}
			else
			{
				if( iStep[0] == 1 )
					iStep[0] = 10;
				else if( iStep[0] == 10 )
					iStep[0] = 50;
				else
					iStep[0] = 1;
			}
			DrawTofdBtn( iBtn, iStep, iIndexFunc );
		}
		else if( keycode == 4 )
		{
			if( iIndexFunc != 2 )
			{
				iIndexFunc = 2;
			}
			else
			{
				if( iStep[1] == 1 )
					iStep[1] = 10;
				else if( iStep[1] == 10 )
					iStep[1] = 50;
				else
					iStep[1] = 1;
			}
			DrawTofdBtn( iBtn, iStep, iIndexFunc );
		}
		else if( keycode == 5 )
		{
			if( iIndexFunc != 3 )
			{
				iIndexFunc = 3;
			}
			else
			{
				if( iStep[2] == 10 )
					iStep[2] = 50;
				else if( iStep[2] == 50 )
					iStep[2] = 100;
				else
					iStep[2] = 10;
			}
			DrawTofdBtn( iBtn, iStep, iIndexFunc );
		}
		//���ܼ�1��ȷ�ϰ���
		else if( keycode == 16 )
		{
			if( iBtn[0] == 0 )
			{
				//��ʼ��������
				for( i = 0; i < MAX_LINE; i++ ) 
				{
					memset( g_pEcho[i], 0, ECHO_PACKAGE_SIZE );
				}	
				g_iLine = 0;
				
				EraseWindow( 2, 135, 500, 295 );
				
				//��ʼ��������
				SetScanRotaryEncoder( iEncoder, 1, 1, 1 );
				SetScanRotaryEncoder( iEncoder, 1, 0, 1 );
				iRotaryValue =  0;
				
				//��ʼɨ��FLAG
				iBtn[0] = 1;
			}
			else
			{
				iBtn[0] = 0;
			}
			//���°�����ʾ
			DrawTofdBtn( iBtn, iStep, iIndexFunc );
		}
		else if( keycode == 17 )
		{
			iIndexFunc = ++iIndexFunc % 4;
			DrawTofdBtn( iBtn, iStep, iIndexFunc );
		}
		else if( keycode == 18 )
		{
			if ( g_iLine != 0 && MGetSaveStatus() == 0)	//��ʼ��¼
			{
				if (DisplayQuery(14))	//�����Ӻ�U�̣�
				{
					DisplayPrompt(19);	//��������U��
					
					if(UDiskInitialize(3))
					{
						DisplayPrompt(20);	//U�����ӳɹ�
						
						g_UDiskInfo.DataHeaderMark = 1;
						MSetSaveStatus( 1,C_SETMODE_SETSAVE);
						EraseWindow( 2, 4, 500, 100 );
						memset( szkey, 0, 64 );
						if(MInputChar(10, 10, 0, Notes.name, 30, 30) == C_TRUE)	//���30���ַ�
						{
							EraseWindow( 2, 4, 500, 100 );
							
							for( i = 0; i < Notes.name[0]; i++ )
								szkey[i] = Notes.name[i+1];
							strcat(szkey, ".tof" );
							
							FRESULT res = f_open(&g_FileObject, szkey, FA_CREATE_ALWAYS|FA_WRITE|FA_READ);
							if( res == FR_OK )
							{
								f_lseek( &g_FileObject, 0 );
								f_write(&g_FileObject, &g_iLine, sizeof(int), NULL);
								u_int iRang  = MGetRange(3);
								f_write(&g_FileObject, &iRang, sizeof(u_int), NULL);
								u_int iDelay = MGetDelay(3);
								f_write(&g_FileObject, &iDelay, sizeof(u_int), NULL);
								f_write(&g_FileObject, &g_iPCS, sizeof(int), NULL);
								f_write(&g_FileObject, &g_iTofdFreq, sizeof(int), NULL);
								f_write(&g_FileObject, &g_iEncValue, sizeof(long), NULL);
								
								for( i = 0; i <= g_iLine; i++ )
									f_write(&g_FileObject, g_pEcho[i], ECHO_PACKAGE_SIZE, NULL);
									
								f_close(&g_FileObject);
								
								strcat(szkey, " �ѱ���! �����������!" );
								TextOut( 10, 31, 1, 500, 61, szkey, 4 );
							}
							else
							{
								strcat(szkey, " �򿪳���! �����������!" );
								TextOut( 10, 31, 1, 500, 61, szkey, 4 );
							}
						}	
						MSetSaveStatus( 0,C_SETMODE_SETSAVE);	
						
						MAnyKeyReturn();
						bFirst = true;						
					}	
				}
			}
			
		}
		//ȡ������
		else if( keycode == 19 )
		{
			g_iRang  = MGetRange(3);
			g_iDelay = MGetDelay(3);
			break;
		}	
	}
}

