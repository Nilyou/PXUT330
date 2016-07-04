#include <stdio.h>
#include <string.h>

#include "globe.h"
#include "sdcard.h"
#include "ff.h"
#include "sysdef.h"
#include "tools.h"

UDISK_INFO g_UDiskInfo;
const int MAX_FRAMES_PER_SECOND=30; //15
FIL g_FileObject;

//写测试，连续写，按任意键盘 退出

int CreateEchoDataFile()
{
    int rtn;
    char filename[13];
    FRESULT res;

    /*
    if(!(rtn=UdkListDir(g_UDiskInfo.UDiskBuffer)))
    {
        ShowProblemLine();
        return FALSE;
    }*/

    //if(rtn=UdkOpenFile(g_UDiskInfo.filename,"DAT"))
    memset(filename,0,13);
    memcpy(filename,g_UDiskInfo.filename,8);
    strcat(filename,".DAT");
    TRACE(g_xpos,g_ypos+=10,"-I-  TP_1  ");

    res = f_open(&g_FileObject, filename, FA_OPEN_EXISTING|FA_WRITE|FA_READ);
    if( res == FR_OK )
    {

    	TRACE(g_xpos,g_ypos+=10,"-I- CreateEchoDataFile TP_1_0  ");
        // UdkSetFilePointer(1,0);
        f_lseek(&g_FileObject,0);
        //if(!UdkReadFile(1,sizeof(u_int),&g_UDiskInfo.TotalWriteTimes))
        if(f_read(&g_FileObject,&g_UDiskInfo.TotalWriteTimes,4,NULL)!=FR_OK)
        {
            ShowProblemLine();
            return FALSE;
        }
		TRACE(g_xpos,g_ypos+=10,"-I- CreateEchoDataFile TP_1_1  ");
        //READ

        // if(!UdkReadFile(1,sizeof(u_int),&g_UDiskInfo.LastFileLength))
        if(f_read(&g_FileObject,&g_UDiskInfo.LastFileLength,4,NULL)!=FR_OK)
        {
            ShowProblemLine();
            return FALSE;
        }

        //	g_UDiskInfo.LastFileLength;

#ifdef DEBUG
        Write_Number(50,50,g_UDiskInfo.LastFileLength,32,0,0);
#endif
        if (g_UDiskInfo.LastFileLength!=0)
        {
            g_UDiskInfo.filelen=g_UDiskInfo.LastFileLength;
        }
        else
        {
            //g_UDiskInfo.filelen = UdkGetFileLength(1);
            g_UDiskInfo.filelen = f_size(&g_FileObject);
            g_UDiskInfo.LastFileLength = g_UDiskInfo.filelen;

        }
        memcpy(g_UDiskInfo.UDiskBuffer,&g_UDiskInfo.LastFileLength,sizeof(u_int));

        //if(!UdkSetFilePointer(1,sizeof(u_int)))
        if(f_lseek(&g_FileObject,sizeof(u_int))!=FR_OK)
        {
            ShowProblemLine();
            return FALSE;
        }

        // Save
        //if(!UdkWriteFile(1,sizeof(u_int), g_UDiskInfo.UDiskBuffer))
        if(f_write(&g_FileObject,g_UDiskInfo.UDiskBuffer,4,NULL)!=FR_OK)
        {
            ShowProblemLine();
            return FALSE;
        }


#ifdef DEBUG
        Write_Number(50,60,g_UDiskInfo.LastFileLength,32,0,0);
#endif
        //if(!UdkSetFilePointer(1,g_UDiskInfo.filelen))
        if (f_lseek(&g_FileObject,g_UDiskInfo.filelen)!=FR_OK)
        {
            ShowProblemLine();
            return FALSE;
        }
    }
    else
    {
        TRACE(g_xpos,g_ypos+=10,"-I- CreateEchoDataFile TP_2  ");
        //if(!UdkCreateFile(g_UDiskInfo.filename,"DAT",0x21))
        res = f_open(&g_FileObject, filename, FA_CREATE_ALWAYS|FA_WRITE|FA_READ);
        if( res != FR_OK )
        {
            ShowProblemLine();
            return FALSE;
        }

        TRACE(g_xpos,g_ypos+=10,"-I- CreateEchoDataFile TP_3  ");

        memset(g_UDiskInfo.UDiskBuffer,0,sizeof(u_int));
        g_UDiskInfo.transfer_surplus = 2*sizeof(u_int);
    }
    g_UDiskInfo.operatormode = 1;
    g_UDiskInfo.DataHeaderMark = 1;

  //  WaitForKeyStroke();
    delay(5);

    return 1;

}


typedef struct
{
    u_char	second;
    u_char	minute;
    u_char	hour;
    u_char  notes_len;
}  HEADER_TIMESTAMP;


extern SYSTEM_STATUS		SystemStatus;		/*4*/
extern CHANNEL_STATUS		ChannelStatus;		/*4*/
extern CHANNEL_PARAMETER	ChannelPara;		/*15*2*/
extern GATE_PARA	AGatePara;
extern GATE_PARA	BGatePara;
extern DAC	Dac;
extern WELD_PARA	stWeldPara;
extern NOTES	Notes;

int GenerateDataHeader(void *ptrAddress)
{
    TIME_CLOCK tm;
    HEADER_TIMESTAMP timestamp;
    int offset;

    int i=0;

    GetSystemTime(&tm) ;

    timestamp.second = tm.second;
    timestamp.minute = tm.minute;
    timestamp.hour = tm.hour;
    timestamp.notes_len = Notes.name[0];

    offset = 0;

    memcpy(ptrAddress + offset,&timestamp,sizeof(HEADER_TIMESTAMP));
    offset += sizeof(HEADER_TIMESTAMP); //size = 4

    memcpy(ptrAddress + offset,&SystemStatus,sizeof(SYSTEM_STATUS));
    offset += sizeof(SYSTEM_STATUS); //size = 20

    memcpy(ptrAddress + offset,&ChannelStatus,sizeof(CHANNEL_STATUS));
    offset += sizeof(CHANNEL_STATUS);//size = 32

    memcpy(ptrAddress + offset,&ChannelPara,sizeof(CHANNEL_PARAMETER));
    offset += sizeof(CHANNEL_PARAMETER);//size = 32

    memcpy(ptrAddress + offset,&AGatePara,sizeof(GATE_PARA));
    offset += sizeof(GATE_PARA);//a门 size = 4

    memcpy(ptrAddress + offset,&BGatePara,sizeof(GATE_PARA));
    offset += sizeof(GATE_PARA);//b门 size = 4

    memcpy(ptrAddress + offset,&Dac,sizeof(DAC));
    offset += sizeof(DAC);//曲线 size = 48

    memcpy(ptrAddress + offset,&stWeldPara,sizeof(WELD_PARA));
    offset += sizeof(WELD_PARA);	//焊缝 size = 24

    memcpy(ptrAddress + offset,&Notes.name,(timestamp.notes_len+1)*2);
    offset += (timestamp.notes_len+1)*2;

#ifdef DEBUG
    TRACE(10, 100,"basegain=",MGetBaseGain());
    TRACE(10, 110,"offset=",MGetOffset());
    TRACE(10, 120,"CHANNEL_STATUS=",sizeof(CHANNEL_STATUS));
    TRACE(10, 130,"CHANNEL_PARAMETER=",sizeof(CHANNEL_PARAMETER));
#endif
    /*
    	TRACE(10, 100,"HEADER_TIMESTAMP=",sizeof(HEADER_TIMESTAMP));
    	TRACE(10, 110,"SYSTEM_STATUS=",sizeof(SYSTEM_STATUS));
    	TRACE(10, 120,"CHANNEL_STATUS=",sizeof(CHANNEL_STATUS));
    	TRACE(10, 130,"CHANNEL_PARAMETER=",sizeof(CHANNEL_PARAMETER));

    	TRACE(10, 100,"AGatePara=",sizeof(GATE_PARA));
    	TRACE(10, 110,"BGatePara=",sizeof(GATE_PARA));
    	TRACE(10, 120,"Dac=",sizeof(DAC));
    	TRACE(10, 130,"WeldPara=",sizeof(WELD_PARA));

    	Write_Number(10,40,timestamp.notes_len,10,0,0);
    	for (i=0;i<13;i++)
    	{
    		Write_Number(i*24,60,Notes.name[i],3,0,0);
    	}


    	for(;;);
    */

    return offset;
}

int WriteEchoDateToFile(int mode)
{

    unsigned char rtn;
    u_int time2,time3;
    long out_len, total_len,len,in_len, info_len;
    u_char echobuffer[C_LEN_SAMP];
    int offset;
    if(mode>0)	//for 单幅数据存U盘
    {
        offset = C_OFF_SAMP;
        MCopyProtectedMemory(g_UDiskInfo.pEchoWriteBuffer, (void*)offset, C_LEN_SAMP-1, PM_COPY_READ);	//波形参数
        g_UDiskInfo.sample_num=1;
    }
    in_len = (g_UDiskInfo.sample_num+1)*(ECHO_PACKAGE_SIZE+GATE_PEAK_SIZE);
    if (g_UDiskInfo.DataHeaderMark==0)
        info_len = 0;
    else if(g_UDiskInfo.DataHeaderMark==1)
    {
        //放置信息
        info_len = GenerateDataHeader(g_UDiskInfo.pEchoWriteBuffer + in_len);
        // info_len = 100;
        //for(i=0; i<info_len;i++)
        //	*(g_UDiskInfo.pEchoWriteBuffer + in_len + i) = i%255;

        g_UDiskInfo.DataHeaderMark = 0;
        Notes.name[0]=0;
    }
    in_len += info_len;

    // 压缩数据

    LzwInitBuf();

    LzwEncode(in_len, g_UDiskInfo.pEchoWriteBuffer, &out_len, g_UDiskInfo.UDiskBuffer+g_UDiskInfo.transfer_surplus+ 2*sizeof(u_long));

    LzwDestroyBuf();


    memcpy(g_UDiskInfo.UDiskBuffer + g_UDiskInfo.transfer_surplus, &out_len, sizeof(u_long));
    memcpy(g_UDiskInfo.UDiskBuffer + g_UDiskInfo.transfer_surplus + sizeof(u_long), &info_len, sizeof(u_long));

    g_UDiskInfo.TotalWriteTimes++;

    total_len = g_UDiskInfo.transfer_surplus + 2*sizeof(u_long) + out_len;


    if((g_UDiskInfo.filelen%BUFFER_LENGTH)!=0)
    {
        len = BUFFER_LENGTH - g_UDiskInfo.filelen%BUFFER_LENGTH;
        if (total_len<len)
            len = total_len;
        //if(!(rtn = UdkWriteFile(1,len, g_UDiskInfo.UDiskBuffer)))
    //    TRACE(g_xpos,g_ypos+=10,"-I- WriteEchoDateToFile TP_1  ");
        if(f_write(&g_FileObject,g_UDiskInfo.UDiskBuffer,len,NULL)!=FR_OK)
        {
            ShowProblemLine();
            return FALSE;
        }
        f_sync(&g_FileObject);
        g_UDiskInfo.filelen += len;
        total_len -= len;
        memcpy(g_UDiskInfo.UDiskBuffer,g_UDiskInfo.UDiskBuffer+len,total_len);
    }

    while (total_len>=BUFFER_LENGTH)
    {
        time3 = GetElapsedTime();
        //if(!(rtn = UdkWriteFile(1,BUFFER_LENGTH, g_UDiskInfo.UDiskBuffer)))
       //  TRACE(g_xpos,g_ypos+=10,"-I- WriteEchoDateToFile TP_2  ");

        if(f_write(&g_FileObject,g_UDiskInfo.UDiskBuffer,BUFFER_LENGTH,NULL)!=FR_OK)
        {
            ShowProblemLine();
            return FALSE;
        }
        f_sync(&g_FileObject);
        time2 = GetElapsedTime();
        g_UDiskInfo.filelen += BUFFER_LENGTH;
        total_len -= BUFFER_LENGTH;
        memcpy(g_UDiskInfo.UDiskBuffer,g_UDiskInfo.UDiskBuffer+BUFFER_LENGTH,total_len);

#ifdef _DEBUG
//        unsigned int FreeSpaceLowPart, FreeSpaceHighPart, TotalSpaceLowPart,TotalSpaceHighPart,Cluster;
//	Cluster = UdkGetCapacity(&FreeSpaceLowPart, &FreeSpaceHighPart, &TotalSpaceLowPart,&TotalSpaceHighPart);
        unsigned int FreeSpaceLowPart, TotalSpaceLowPart,ClusterSize;
        ClusterSize= SdCard_GetCapacity(&FreeSpaceLowPart,&TotalSpaceLowPart);


        SetDisplayColor(COLOR_GREEN);
        TRACE(10, 90,"FileSize=",g_UDiskInfo.filelen );
        Write_Number(180, 90,g_UDiskInfo.filelen,10,0,0);
        TRACE(10, 115,"Interval=",time2-time3);
        Write_Number(180, 115,time2-time3,10,0,0);
        TRACE(10, 140,"Times=",g_UDiskInfo.sample_num);
        Write_Number(180, 140,g_UDiskInfo.sample_num,10,0,0);
        TRACE(10, 165,"surplus=",g_UDiskInfo.transfer_surplus);
        Write_Number(180, 165,g_UDiskInfo.transfer_surplus,10,0,0);
        TRACE(10, 190,"out_len=",out_len);
        Write_Number(180, 190,out_len,10,0,0);
        TRACE(10, 215,"in_len=",in_len);
        Write_Number(180, 215,in_len,10,0,0);
        TRACE(10, 240,"total_seconds=",g_UDiskInfo.TotalWriteTimes);
        Write_Number(180, 240,g_UDiskInfo.TotalWriteTimes,10,0,0);
        TRACE(10, 265,"Free_Space:",FreeSpaceLowPart);
        Write_Number(180, 265,FreeSpaceLowPart,10,0,0);
        TRACE(10, 290,"Total Space:",TotalSpaceLowPart);
        Write_Number(180, 290,TotalSpaceLowPart,10,0,0);
        TRACE(10, 315,"Cluster:",ClusterSize);
        Write_Number(180, 315,ClusterSize,10,0,0);
        TRACE(10, 340,"Free size(Cluster):",FreeSpaceLowPart);
        Write_Number(180, 340,FreeSpaceLowPart,10,0,0);

#endif
    }
    g_UDiskInfo.transfer_surplus = total_len;

    return TRUE;

}

void CreateFileName(void)
{
    g_UDiskInfo.filename[0] = ((g_UDiskInfo.system_time.yearl&0xf0)>>4) + 0x30 ;
    g_UDiskInfo.filename[1] = (g_UDiskInfo.system_time.yearl&0xf) + 0x30 ;
    g_UDiskInfo.filename[2]='-';
    g_UDiskInfo.filename[3] = ((g_UDiskInfo.system_time.month&0xf0)>>4) + 0x30 ;
    g_UDiskInfo.filename[4] = (g_UDiskInfo.system_time.month&0xf) + 0x30 ;
    g_UDiskInfo.filename[5]='-';
    g_UDiskInfo.filename[6] = ((g_UDiskInfo.system_time.date&0xf0)>>4) + 0x30 ;
    g_UDiskInfo.filename[7] = (g_UDiskInfo.system_time.date&0xf) + 0x30 ;
}

//U盘初始化
int UDiskInitialize(int mode)
{
    //unsigned int FreeSpaceLowPart, FreeSpaceHighPart, TotalSpaceLowPart,TotalSpaceHighPart;
    unsigned int FreeSpaceLowPart, TotalSpaceLowPart;
    DIR dirinfo;


    TRACE(g_xpos,g_ypos+=10,"-I- Please insert sd card!" );
  //  while((SDCardGetStatus()&0x01)==0);

    if((SDCardGetStatus()&0x01)==0)
        return FALSE;

    {
        // U盘初始化
        TRACE(g_xpos,g_ypos+=10,"-I- SDCard_Init" );
        if(!SDCard_Init())
        {
            TRACE(g_xpos,g_ypos+=10,"-E- SDCard_Init Failure\n\r");
            return FALSE;
        }
/*
        TRACE(g_xpos,g_ypos+=10,"-I- SdCard_Format" );
        if(!SdCard_Format())
        {
            TRACE(g_xpos,g_ypos+=10,"-E- SdCard_Format Failure\n\r");
            return FALSE;
        }
*/
        TRACE(g_xpos,g_ypos+=10,"-I- SDCard_Mount" );
        if(!SDCard_Mount())
        {
            TRACE(g_xpos,g_ypos+=10,"-E- SDCard_Mount Failure\n\r");
            return FALSE;
        }
        if(mode==1)
        {
            if(f_opendir(&dirinfo,"/PXUTSOLO")!=FR_OK)
                f_mkdir("PXUTSOLO");
            f_chdir("/PXUTSOLO");
        }
        else if(mode==2)
        {
            if(f_opendir(&dirinfo,"/PXUTTEST")!=FR_OK)
                f_mkdir("PXUTTEST");
            f_chdir("/PXUTTEST");
        }
        else
        {

            if(f_opendir(&dirinfo,"/PXUTDATA")!=FR_OK)
                f_mkdir("PXUTDATA");
            f_chdir("/PXUTDATA");
        }

        GetSystemTime(&g_UDiskInfo.system_time);
        CreateFileName();
        if(!CreateEchoDataFile())
        {
            ShowProblemLine();
            return FALSE;
        }
        g_UDiskInfo.IsAttached = 1;

        GetSystemTime(&g_UDiskInfo.system_time);
        g_UDiskInfo.last_tick_count = GetElapsedTime();

        g_UDiskInfo.pEchoDataBuffer = g_UDiskInfo.DataBuffer[0];
        g_UDiskInfo.pEchoWriteBuffer = g_UDiskInfo.DataBuffer[1];

        //	UdkGetCapacity(&FreeSpaceLowPart, &FreeSpaceHighPart, &TotalSpaceLowPart,&TotalSpaceHighPart);

        SdCard_GetCapacity(&FreeSpaceLowPart,&TotalSpaceLowPart);
        if (FreeSpaceLowPart<=4)
        {
            g_UDiskInfo.DiskIsFull = 1;

            MSetSaveStatus( 1,C_SETMODE_SETSAVE);
            DisplayPrompt(25);	//U盘容量已满
            FileClose(0);
//			UdkSafelyClosed();
//            memset(&g_UDiskInfo,0,sizeof(UDISK_INFO));
            //  SwitchUsbDevicePowerOff() ;
            DisplayPrompt(25);	//U盘容量已满
            ExpendTime(60);
            DisplayPrompt(15);
            ExpendTime(60);
            DisplayPrompt(25);	//U盘容量已满
            ExpendTime(60);
            MSetSaveStatus( 0,C_SETMODE_SETSAVE);
            DisplayPrompt(23);	//U盘断开连接
            DisplayPrompt(16);
            return FALSE;
        }
        else if (FreeSpaceLowPart<=40)
        {
            DisplayPrompt(26);	//U盘即将存满
            ExpendTime(30);
            DisplayPrompt(15);
            ExpendTime(30);
            DisplayPrompt(26);	//U盘即将存满
            ExpendTime(30);
            DisplayPrompt(15);
            ExpendTime(30);
            DisplayPrompt(26);	//U盘即将存满
        }
        return TRUE;
    }

    return FALSE;
}
extern unsigned char _usbarrow[];

int EchoDataStorageFunc(int keycode,int mode)
{

    u_char* ptr;
    TIME_CLOCK cur_time;
    int i,j;
    u_int  offset = C_OFF_SAMP ;
    u_char sampbuff[C_LEN_SAMP+1] ;
    //unsigned int FreeSpaceLowPart, FreeSpaceHighPart, TotalSpaceLowPart,TotalSpaceHighPart,Cluster;
    unsigned int ClusterSize,FreeSpaceLowPart, TotalSpaceLowPart;
    WAVEPARA pWavePara;

    pWavePara.wavepara[0]=ChannelPara.wavepara[0];
    pWavePara.wavepara[1]=ChannelPara.wavepara[1];
    pWavePara.wavepara[2]=ChannelPara.wavepara[2];
    pWavePara.wavepara[3]=ChannelPara.wavepara[3];

    MCopyProtectedMemory( sampbuff, (void*)offset, C_LEN_SAMP, PM_COPY_READ);
    for (i=0; i<C_COORWIDTH; i++)
    {
        if (sampbuff[i]>=200)
        {
            sampbuff[i]=199;
        }
        pWavePara.sampbuff[i]=sampbuff[i];
//		Write_Number((i)%10*24,(i/10)*8,pWavePara.sampbuff[i],3,0,0);
    }
    if(g_UDiskInfo.operatormode == 1)
    {
        if(g_UDiskInfo.sample_num < MAX_FRAMES_PER_SECOND )
        {
            if(GetElapsedTime() - g_UDiskInfo.last_tick_count >= C_WRITE_INTERVAL/MAX_FRAMES_PER_SECOND)
            {
                ptr = GetSampleBuffer();
                memcpy(g_UDiskInfo.pEchoDataBuffer + g_UDiskInfo.sample_num*(ECHO_PACKAGE_SIZE+GATE_PEAK_SIZE), ptr, ECHO_PACKAGE_SIZE);

                //GATE_PEAK* pGatePeak = GetGatePeakBuffer() ;
                memcpy(g_UDiskInfo.pEchoDataBuffer + g_UDiskInfo.sample_num*(ECHO_PACKAGE_SIZE+GATE_PEAK_SIZE) + ECHO_PACKAGE_SIZE,&pWavePara, sizeof(WAVEPARA));
                g_UDiskInfo.sample_num ++;
                g_UDiskInfo.last_tick_count = GetElapsedTime();
            }
        }
    }

    GetSystemTime(&cur_time);
    if (BCDtoDecimal(cur_time.second)!= BCDtoDecimal(g_UDiskInfo.system_time.second))
    {
        char *ptrTemp;
        ptrTemp = g_UDiskInfo.pEchoDataBuffer;
        g_UDiskInfo.pEchoDataBuffer = g_UDiskInfo.pEchoWriteBuffer;
        g_UDiskInfo.pEchoWriteBuffer = ptrTemp;

     //   TRACE(g_xpos,g_ypos+=10,"-I- EchoDataStorageFunc TP_2  ");
      //  WaitForKeyStroke();

        // 写数据到U盘
        if(!WriteEchoDateToFile(mode))
        {
            ShowProblemLine();
            return FALSE;
        }

        PageDisp(16*7+9+MGetLanguage()*8*C_ECHAR_HDOT,0,(u_char*)_usbarrow);
        MEraseWindow(16*7+9+MGetLanguage()*8*C_ECHAR_HDOT,0,16*7+16+MGetLanguage()*8*C_ECHAR_HDOT,4*(4-g_UDiskInfo.TotalWriteTimes%5));	//0~4
        g_UDiskInfo.sample_num = 0;
        g_UDiskInfo.system_time.second = cur_time.second;
    }


//        Cluster=UdkGetCapacity(&FreeSpaceLowPart, &FreeSpaceHighPart, &TotalSpaceLowPart,&TotalSpaceHighPart);

    ClusterSize = SdCard_GetCapacity(&FreeSpaceLowPart,&TotalSpaceLowPart);
  //  TRACE(g_xpos,g_ypos+=10,"-I- EchoDataStorageFunc TP_3 para1=%d para2=%d para3=%d", ClusterSize,FreeSpaceLowPart,TotalSpaceLowPart );
  //  WaitForKeyStroke();
    if (FreeSpaceLowPart<=4)
    {
        g_UDiskInfo.DiskIsFull = 1;
    }
    else if (FreeSpaceLowPart<=40)
    {
        TIME_CLOCK TimeClock;
        GetSystemTime(&TimeClock);
        if (TimeClock.second == 3 || TimeClock.second == 5 || TimeClock.second == 7)
        {
            DisplayPrompt(26);	//U盘即将存满
        }
    }

    return TRUE;
}

void StorageData(int keycode)
{

    if (MGetSaveMode() && MGetSaveStatus() == 1)
    {
        if ((!g_UDiskInfo.DiskIsFull) )//&& g_UDiskInfo.IsAttached)
        {
            if (!EchoDataStorageFunc(keycode,0))
            {
                DisplayPrompt(21);	//U盘连接失败
            }
        }
        else
        {
            DisplayPrompt(25);	//U盘容量已满
            FileClose(0);
            //UdkSafelyClosed();
//            memset(&g_UDiskInfo,0,sizeof(UDISK_INFO));
            //SwitchUsbDevicePowerOff() ;
            DisplayPrompt(25);	//U盘容量已满
            ExpendTime(60);
            DisplayPrompt(15);
            ExpendTime(60);
            DisplayPrompt(25);	//U盘容量已满
            ExpendTime(60);
            MSetSaveStatus( 0,C_SETMODE_SETSAVE);
            DisplayPrompt(23);	//U盘断开连接
            DisplayPrompt(16);
            /*	TIME_CLOCK TimeClock;
            	GetSystemTime(&TimeClock);
            	WriteTime(50,100,&TimeClock);
            	MAnyKeyReturn();*/
            //g_UDiskInfo.DiskIsFull = 1;
        }
    }
}

int FileClose(int mode)
{
    if ((MGetSaveMode() && MGetSaveStatus() == 1 )|| mode==1)
    {

    	TRACE(g_xpos,g_ypos+=10,"-I- FileClose TP_1  ");
    	//WaitForKeyStroke();

        //if(!UdkWriteFile(1,g_UDiskInfo.transfer_surplus, g_UDiskInfo.UDiskBuffer))
        if(f_write(&g_FileObject,g_UDiskInfo.UDiskBuffer,g_UDiskInfo.transfer_surplus,NULL)!=FR_OK)
        {
            ShowProblemLine();
            return FALSE;
        }

        //UdkSetFilePointer(1,0);
        f_lseek(&g_FileObject,0);

        memcpy(g_UDiskInfo.UDiskBuffer,&g_UDiskInfo.TotalWriteTimes,sizeof(u_int));
        //if(!UdkWriteFile(1,sizeof(u_int), g_UDiskInfo.UDiskBuffer))
        if(f_write(&g_FileObject,g_UDiskInfo.UDiskBuffer,sizeof(u_int),NULL)!=FR_OK)
        {
            ShowProblemLine();
            return FALSE;
        }

        g_UDiskInfo.LastFileLength = 0;
        memcpy(g_UDiskInfo.UDiskBuffer,&g_UDiskInfo.LastFileLength,sizeof(u_int));

        //if(!UdkWriteFile(1,sizeof(u_int), g_UDiskInfo.UDiskBuffer))
        if(f_write(&g_FileObject,g_UDiskInfo.UDiskBuffer,sizeof(u_int),NULL)!=FR_OK)
        {
            ShowProblemLine();
            return FALSE;
        }

        g_UDiskInfo.filelen += g_UDiskInfo.transfer_surplus;
        g_UDiskInfo.transfer_surplus = 0;
        g_UDiskInfo.DataHeaderMark = 1;

        f_close(&g_FileObject);
        SDCardStopTransfer();
        memset(&g_UDiskInfo,0,sizeof(UDISK_INFO));
        //UdkSetFilePointer(1,g_UDiskInfo.filelen);
        //f_lseek(&g_FileObject,f_size(&g_FileObject));
        return TRUE;
    }
    else
    {
         return FALSE;
    }

}

void Write_UDisk_Len(int xpos,int ypos,int SpacePart,int Cluster,int filelen)
{
    /*
    	SpacePart ：占用的簇的数量
    	Cluster  ：每个簇的大小
    	filelen	 ：小于2G的可以直接写大小，大于2G的置0。
     */

    u_int NumLen=1,tempLen,Len,LenLow;
    u_int LenKB,LenMB,LenGB;
    if (filelen>0)
    {
        Write_Number(xpos,ypos,filelen,11,0,0);

        if (filelen < 1024)
        {
            Write_Number(xpos+18*16,ypos,filelen,4,0,0);
            EMenuOut(xpos+24*16,ypos," B",2,1,24);
        }
        else if (filelen<1048576)
        {
            LenKB=filelen/10.24+0.5;
            Write_Number(xpos+18*16,ypos,LenKB,6,2,0);
            EMenuOut(xpos+24*16,ypos,"KB",2,1,24);
        }
        else if (filelen<1073741824)
        {
            LenKB=filelen/1024+0.5;
            LenMB = LenKB/10.24+0.5;
            Write_Number(xpos+18*16,ypos,LenMB,6,2,0);
            EMenuOut(xpos+24*16,ypos,"MB",2,1,24);
        }
        else
        {
            LenKB=filelen/1024+0.5;
            LenMB = LenKB/1024+0.5;
            LenGB = LenMB/10.24+0.5;
            Write_Number(xpos+18*16,ypos,LenGB,6,2,0);
            EMenuOut(xpos+24*16,ypos,"GB",2,1,24);
        }
    }
    else
    {
        Len=(((SpacePart*2)/1000+0.5)*Cluster/2);
        LenLow= ((SpacePart%1000)*Cluster)%1000;	//低三位
        tempLen=Len;
        do
        {
            tempLen=tempLen/10;
            if (tempLen>0)
            {
                NumLen++;
            }

        }
        while (tempLen>0);

        Write_Number(xpos,ypos,Len,NumLen,0,0);
        Write_Number(xpos+NumLen*16,ypos,LenLow,3,0,0);

        LenKB=(((SpacePart*2)/1024+0.5)*Cluster/2);	//KB

        if (LenKB == 0)
        {
            Write_Number(xpos+18*16,ypos,SpacePart*Cluster,4,0,0);
            EMenuOut(xpos+24*16,ypos," B",2,1,24);
        }
        else if (LenKB<1024)
        {
            Write_Number(xpos+18*16,ypos,LenKB,4,0,0);
            EMenuOut(xpos+24*16,ypos,"KB",2,1,24);
        }
        else if (LenKB<1048576)
        {
            LenMB = LenKB/10.24+0.5;
            Write_Number(xpos+18*16,ypos,LenMB,6,2,0);
            EMenuOut(xpos+24*16,ypos,"MB",2,1,24);
        }
        else if (LenKB<1073741824)
        {
            LenMB = LenKB/1024+0.5;
            LenGB = LenMB/10.24+0.5;
            Write_Number(xpos+18*16,ypos,LenGB,6,2,0);
            EMenuOut(xpos+24*16,ypos,"GB",2,1,24);
        }
    }

}

/*
1.单幅数据存U盘
*/
extern FAT fat;
int SaveToUDisk(int file_no)
{

    int offset;
    int i,count=0;

    //SetUsbMode(USB_HOST_MODE);

    if (DisplayQuery(14))	//已连接好U盘？
    {
        // U盘初始化
//        memset(&g_UDiskInfo,0,sizeof(UDISK_INFO));
        // SetUsbMode(USB_HOST_MODE);
        DisplayPrompt(19);	//正在连接U盘
        if(!UDiskInitialize(1))
        //if(!SDCard_Init())
        {
            //初始化失败
            DisplayPrompt(21);	//U盘连接失败
            MSetSaveStatus( 0,C_SETMODE_SETSAVE);
            // SwitchUsbDevicePowerOff() ;
            ScreenRenovate();
            DrawDac(0);
            DisplayPrompt(16);
            return FALSE;
        }
        DisplayPrompt(20);	//U盘连接成功
        MSetAcquisition(0);
        MEraseWindow(0,0,320,18);
        g_UDiskInfo.DataHeaderMark = 1;
        MSetSaveStatus( 1,C_SETMODE_SETSAVE);

        offset = C_OFF_FILEINFO + C_SIZE_INT;
        MCopyProtectedMemory(&fat, (void*)offset, C_LEN_FAT, PM_COPY_READ);
        for(i=0; i<12; i++)
        {
            Notes.name[i]=fat.name[i];
        }
        /*	if(MInputChar(0, 0, 1,Notes.name,30,30) != C_TRUE)	//最多30个字符
        	{
        		//退出存储

        		FileClose();
        		UdkSafelyClosed();
        		SwitchUsbDevicePowerOff();
        		memset(&g_UDiskInfo,0,sizeof(UDISK_INFO));
        		MSetSaveStatus( 0, C_SETMODE_SETSAVE);
        		ScreenRenovate();
        		DrawDac(0);
        		DisplayPrompt(23);	//U盘断开连接
        		DisplayPrompt(16);
        		return 0;
        	}*/

//		MSetAcquisition(1);
        //连续存
        //	DataDisplay(file_no);
        if ((!g_UDiskInfo.DiskIsFull) )//&& g_UDiskInfo.IsAttached)
        {
            TIME_CLOCK cur_time;

            GetSystemTime(&cur_time);
            count=0;
            while(1)
            {
                GetSystemTime(&cur_time);
                if (!EchoDataStorageFunc(0,1))
                {
                    DisplayPrompt(21);	//U盘连接失败
                }

                if(BCDtoDecimal(cur_time.second) != BCDtoDecimal(g_UDiskInfo.system_time.second))
                {
                    count++;
                }
                if(count>2)	break;

            }
            DisplayPrompt(24);	//正在安全断开
            FileClose(1);
            DisplayPrompt(23);	//U盘断开连接
            MSetSaveStatus( 0,C_SETMODE_SETSAVE);
//            memset(&g_UDiskInfo,0,sizeof(UDISK_INFO));
            //	UdkSafelyClosed();
        }
        else
        {
            DisplayPrompt(25);	//U盘容量已满
            FileClose(1);
//			UdkSafelyClosed();
//            memset(&g_UDiskInfo,0,sizeof(UDISK_INFO));
            // SwitchUsbDevicePowerOff() ;
            DisplayPrompt(25);	//U盘容量已满
            ExpendTime(60);
            DisplayPrompt(15);
            ExpendTime(60);
            DisplayPrompt(25);	//U盘容量已满
            ExpendTime(60);
            MSetSaveStatus( 0,C_SETMODE_SETSAVE);
            DisplayPrompt(23);	//U盘断开连接
            DisplayPrompt(16);
        }


        return 0;



    }
    //未连接U盘，或退出连接
    MSetSaveStatus( 0, C_SETMODE_SETSAVE);
    //SwitchUsbDevicePowerOff() ;
    DisplayPrompt(16);


    return 0;
}




