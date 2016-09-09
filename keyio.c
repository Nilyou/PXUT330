//keyio.c
#include "Fchar.h"
#include "simkai24.h"
#ifndef _KEYIO_C_
#define _KEYIO_C_

char MEMORY_STORAGE[(C_OFF_FILEMAX+4095)/4096*4096];
int nRotaryIndex;
extern int HoriOffsetScreen ;		//所有写在屏幕上的内容，水平方向开始点
extern int iTofdMode;

int MGetRotary(int rotary/*0第一个旋钮1第二个，－1全部，－2判断是否有旋钮*/)
{
    int prekeycode, keycode ;
    unsigned int pretime, currtime;
    unsigned int offset;
    unsigned short i,key[4],prekey[4];
//	short rot[4];
    unsigned short timespace;

    offset = C_OFF_STORE + 50+C_SIZE_INT;//			前次取旋钮值时时间
    MCopyProtectedMemory( &pretime , (void*)offset, C_SIZE_INT, PM_COPY_READ);
    timespace = 300;	//两次读数之间的时间间隔
    currtime = GetElapsedTime();

    if( Mabsi(currtime - pretime) < timespace)return 0;
    else if(Mabsi(currtime - pretime) < 10*timespace)
    {
        offset = C_OFF_STORE + 50;//			前次旋钮值
        keycode = 0;
        MCopyProtectedMemory( (void*)offset, &keycode, C_SIZE_INT, PM_COPY_WRITE);
    }
#if( C_ROTARY > 0)
    keycode = GetRotaryValue();
//Write_Number( 10,100,keycode & 0x7f,4,0,0);
//Write_Number( 50,100,(keycode & 0x7f00)>>8,4,0,0);
//Write_Number( 100,100,currtime,10,0,0);
//	keycode = 0;
#else
    keycode = 0;
#endif

#if C_DEVLIB == 3
//	keycode = keycode & 0xffffffff;//d15
#else
//	keycode = keycode & 0x7f7f7f7f;
#endif
//*
//Write_Number(10,40,keycode,15,0,0);
//Write_Number(10,50,(keycode>>24)&0xff,4,0,0);
//Write_Number(10,60,(keycode>>16)&0xff,4,0,0);
//Write_Number(10,70,(keycode>>8)&0xff,4,0,0);
//Write_Number(10,80,(keycode>>0)&0xff,4,0,0);
//*/
    if( keycode == 0)return 0;
    offset = C_OFF_STORE + 50;//			前次旋钮值
    MCopyProtectedMemory( &prekeycode , (void*)offset, C_SIZE_INT, PM_COPY_READ);
    MCopyProtectedMemory( (void*)offset, &keycode, C_SIZE_INT, PM_COPY_WRITE);


#if C_DEVLIB == 3
    key[2] = 0;
    if( (keycode & 0x8000) != 0)key[3] = 128;
    else key[3] = 0;
    //keycode = ( (keycode & 0x7f) << 8) + ( (keycode >> 8 ) & 0x7f) ;
    for(i = 0; i < 2; i++)
    {
        key[i] = keycode & 0x7f;
        keycode = (keycode >> 8);// & 0x7f;
        prekey[i] = prekeycode & 0x7f;
        prekeycode = (prekeycode >> 8);// & 0x7f;
        if(key[i] != prekey[i] && extend == 0)
        {
            Write_Number(10+i*50,40,prekey[i],2,0,0);
            Write_Number(10+i*50,50,key[i],2,0,0);
        }
    }
#else
    for(i = 0; i < 4; i++)
    {
        key[i] = keycode & 0xff;
        keycode = (keycode >> 8) ;//& 0xff;
        prekey[i] = prekeycode & 0xff;
        prekeycode = (prekeycode >> 8);//&0xff;
//		Write_Number( 10,30+10*i,key[i],4,0,0);
//		Write_Number( 100,30+10*i,prekey[i],4,0,0);
//if(key[i] != prekey[i] && extend == 0)
        if(extend == 0)
        {
            Write_Number(10+i*50,40,prekey[i],2,0,0);
            Write_Number(10+i*50,50,key[i],2,0,0);
        }
    }
#endif

    switch(rotary)
    {
    case 0:
        offset = C_OFF_STORE + 60;//			右旋钮值
        i = ( key[3] - key[2] );
        MCopyProtectedMemory( (void*)offset, &i, C_SIZE_SHORT, PM_COPY_WRITE);
        offset = C_OFF_STORE + 58;//			左旋钮值
        MCopyProtectedMemory( &i, (void*)offset, C_SIZE_SHORT, PM_COPY_READ);
        keycode = ( key[0] - key[1]) + i;
        i = 0;
        MCopyProtectedMemory( (void*)offset, &i, C_SIZE_SHORT, PM_COPY_WRITE);
        prekeycode = ( prekey[0] - prekey[1]);
        break;
    case 1:
        offset = C_OFF_STORE + 58;//			左旋钮值
        i = ( key[0] - key[1]);
        MCopyProtectedMemory( (void*)offset, &i, C_SIZE_SHORT, PM_COPY_WRITE);
        offset = C_OFF_STORE + 60;//右旋钮值置0
        MCopyProtectedMemory( &i, (void*)offset, C_SIZE_SHORT, PM_COPY_READ);
        keycode = ( key[3] - key[2] ) + i;
        i = 0;
        MCopyProtectedMemory( (void*)offset, &i, C_SIZE_SHORT, PM_COPY_WRITE);//右旋钮值置0
        prekeycode = ( prekey[3] - prekey[2]);
        break;
    case -1:
        offset = C_OFF_STORE + 58;//			左右旋钮值置0
        keycode = 0;
        MCopyProtectedMemory( (void*)offset, &keycode, C_SIZE_INT, PM_COPY_WRITE);
#if C_DEVLIB == 3
        if(key[3] > 0)
        {
            prekeycode = keycode = key[3];
            ExpendTime(40);
            GetRotaryValue();
        }
        else
        {
            keycode =  key[0] - key[1] ;
            prekeycode =  prekey[0]  -  prekey[1];
        }
#else
        keycode = ( key[0] + key[3] ) - ( key[1] + key[2] );
        prekeycode = ( prekey[0] + prekey[3] ) - ( prekey[1] + prekey[2] );
#endif

        break;
    case -2:
        keycode = ( key[3] - key[2] );
        prekeycode = ( prekey[3] - prekey[2]);
        if( currtime - pretime < timespace *2 && keycode * prekeycode < 0)keycode = 0;
        if( keycode >= 128)keycode = 127;
        else if( keycode <= -128)keycode = -127;
        i =  keycode + 128;
        keycode = ( key[0] - key[1]);
        prekeycode = ( prekey[0] - prekey[1]);
        if( currtime - pretime < timespace *2 && keycode * prekeycode < 0)keycode = 0;
        if( keycode >= 128)keycode = 127;
        else if( keycode <= -128)keycode = -127;

        prekeycode = ( ( ( keycode + 128 ) & 0xff ) << 16) + i;
        keycode = prekeycode;
        break;
    }

    if( rotary == -1 || rotary == 0 || rotary == 1)
    {
#if C_DEVLIB != 3
        keycode = Mabsi(keycode) * keycode;
        if( keycode > 127 )keycode = 127;
        if( keycode < -127 )keycode = -127;
        if( keycode > 20 || keycode < -20)keycode /= 2;
        if( keycode > 1 || keycode < -1)keycode /= 2;
#endif
    }
//	Write_Number( 10,30+10*4,keycode,6,0,-1);
//	Write_Number( 100,30+10*4,prekeycode,6,0,-1);
    //如果短时间内本次旋钮与前次相反，则判无效
    if( currtime - pretime < timespace *2 && keycode * prekeycode < 0)keycode = 0;
    if( keycode > 0 && keycode <= 2)keycode = 1;
    offset = C_OFF_STORE + 50+C_SIZE_INT;//			前次取旋钮值时时间
    //if( keycode != 0)
    MCopyProtectedMemory( (void*)offset, &currtime, C_SIZE_INT, PM_COPY_WRITE);

//	Write_Number( 200,30+10*4,keycode,6,0,-1);
//Write_Number(10,100,keycode,4,0,0);
    return keycode;
}
//得到键值，time表示得到键值返回之前的最小时间

int MGetKey()
{
    int KeyState=0,KeyNumber=0,KeyTemp=KEY_DIGITAB[0];
    int keycode=0,temp=0;
    int offset,file_total;

    // CharToKeypad(0x02) ; //开机给状态100

    while(1)
    {
        temp=CharToKeypad(KEYCODE_T_REQ) ;
        ExpendTime(2);
        keycode = GetKeyCode();


        if(keycode>0)
        {
//Write_Number(100,200,keycode,10,0,0);
            if(keycode==(DETECTOR_OFF_CODE*256+DETECTOR_OFF_CODE))    //关机
            {
               // EMenuOut(10,0,"TURNOFF!",8,1,16);
               // ExpendTime(1000);
                temp=0;
                while(1)
                {
                    CopyMemoryAllSystem(PM_COPY_WRITE);

  /*                  offset = C_OFF_FILEINFO ;
                    MCopyProtectedMemory( &file_total, (void*)offset, C_SIZE_INT, PM_COPY_READ);
                    if(file_total>C_FILE_NUMBER || file_total<0)file_total=0;
                    CopyMemoryAllData(file_total,PM_COPY_WRITE);
*/
                   // EMenuOut(10,0,"DONE!   ",8,1,16);

                    temp=CharToKeypad(DETECTOR_OFF_ACK);

                    ExpendTime(20);
                }
            }


            KeyState=(keycode>>8)&0xff;   //状态位
            KeyNumber = (keycode&0xff)-200;
//Write_Number(100,240,KeyNumber,10,0,0);
            if(KeyState>199 && KeyNumber<36)
            {
                KeyTemp = KEY_DIGITAB[KeyNumber+1];
            }
            else    //没有按键
            {
                KeyTemp=KEY_DIGITAB[0];
            }
            break;
        }

    }
//Write_Number(100,180,KeyState,10,0,0);
//Write_Number(100,200,KeyTemp,10,0,0);
    return KeyTemp;
}

int MGetKey2()
{
    int keycode = MGetKeyCode(2) ;
//   if(keycode < C_KEYCODMAX && MGetSoundAlarm(0) == 0)BuzzerOn(1);
    return keycode;
}
extern int SecondTime;
int MGetKeyCode(u_int time)
{
    TIME_CLOCK TimeClock;
    static int elapsedtime1=0;
    int PreElapsedTime,elapsedtime = GetElapsedTime();	//开机时长，单位ms
    int prekeycode = 0,
                     keycode;// = GetKeyCode()&0xff;
    /*
        if (keycode==0)
            keycode = KEY_DIGITAB[0];
        else
        {
            keycode -= 0x80;
            keycode = KEY_DIGITAB[keycode+1];
        }
    */
    keycode = MGetKey();



    u_char samekeytime;		//同一键值连续按键次数
    int offset;

//    keycode = ( keycode > 0)? keycode : 0 ;

//;	MGetGatePeakBuffer();  	//允许采样才能得到门内参数


    int startchargetime;			//开始充时时间
    u_char powerlevel=0;
    u_char powerstatus=0;
    int ontime = elapsedtime / (1000*60);		//开机时长，单位分钟
if( iTofdMode != 1 )
{       if( MGetAcquisitionEnable() == 1 && SecondTime<=elapsedtime)
        {
            SecondTime= elapsedtime +1000;
            GetSystemTime(&TimeClock);
            WriteTime(C_SYSTIME_HPOSI,C_SYSTIME_VPOSI,&TimeClock);
        }


    if( ontime != MLoadSystemElapsedTime(C_TIME_MINUTE) )
    {
        powerlevel = ( GetBatteryStatus()%256 ) ;	//电池电量
        powerstatus = (GetBatteryStatus() / 256)%16;	//得到电池状态，d3-d0
        //新的一分钟
        //GetSystemTime(&TimeClock);
        if( MGetAcquisitionEnable() == 1)
        {
          //  WriteTime(C_SYSTIME_HPOSI,C_SYSTIME_VPOSI,&TimeClock);
            DrawPower(powerlevel,1);
            //WriteTime1(C_SYSTIME_HPOSI,C_SYSTIME_VPOSI,MGetSystemTime(8));
        }
        MSaveSystemElapsedTime( ontime, C_TIME_MINUTE );
        if( ontime > 3 )
        {
            //开机第3分钟开始每分钟记录一次当前时间
            offset = C_OFF_BATTERY + 10;
            MCopyProtectedMemory( (void*)offset, &TimeClock, 8, PM_COPY_WRITE);
//            CopyMemoryToSystem(offset, 8,PM_COPY_WRITE);
            offset = C_OFF_BATTERY + 36;	//开机时长存储地址，单位分钟
            MCopyProtectedMemory( (void*)offset, &ontime, 4, PM_COPY_WRITE);
//            CopyMemoryToSystem(offset, 4,PM_COPY_WRITE);

            //每5分钟记录电池电量，每0－4分钟都是记在同一位置，
            if( ontime % 5 == 0 && ontime < 60*10)
            {
                offset = C_OFF_BATTERY + 80 + 2*(ontime / 5 );
                MCopyProtectedMemory( (void*)offset, &powerlevel, 1, PM_COPY_WRITE);
//                CopyMemoryToSystem(offset, 1,PM_COPY_WRITE);
            }
        }

    }
    else
    {
        offset =  C_OFF_BATTERY + 40;		//开始充电时间
        MCopyProtectedMemory( &startchargetime, (void*)offset, C_SIZE_INT, PM_COPY_READ);
#if C_DEVLIB == 3
        //V1,电池信息与其他不同
        if(startchargetime == 0 && ( powerstatus & 0x1 ) != 0 && MGetAcquisitionEnable() == 1)
#else
        if(startchargetime == 0 && ( powerstatus & 0x2 ) != 0 && MGetAcquisitionEnable() == 1)
#endif
        {
            MCopyProtectedMemory( (void*)offset, &ontime, C_SIZE_INT, PM_COPY_WRITE);
//            CopyMemoryToSystem(offset, C_SIZE_INT,PM_COPY_WRITE);
            //记录开始充电的时间
            DrawPower(powerlevel,0);	//一旦开始充电需立即更新电池图标
        }
#if C_DEVLIB == 3
        //V1,电池信息与其他不同
        else if( startchargetime != 0 && ( powerstatus & 0x1 ) == 0 && MGetAcquisitionEnable() == 1)
#else
        else if( startchargetime != 0 && ( powerstatus & 0x2 ) == 0 && MGetAcquisitionEnable() == 1)
#endif
        {
            startchargetime = 0;
            MCopyProtectedMemory( (void*)offset, &startchargetime, C_SIZE_INT, PM_COPY_WRITE);
//            CopyMemoryToSystem(offset, C_SIZE_INT,PM_COPY_WRITE);
            //记录0表示停止充电
            DrawPower(powerlevel,0);	//一旦停止充电需立即更新电池图标
        }
    }
}
  //  if( elapsedtime1 + 10 < GetElapsedTime()  )
/*    if(keycode==C_KEYCODMAX)
    {
       // elapsedtime1=GetElapsedTime();
        if( MGetAcquisitionEnable() == 1 )
        {
            PeakMemory();		//峰值记忆
            EchoEnvelope();		//回波包络
            //		DepthCompensate();		//深度补偿
            //	AutoSave(1);//if mode < 0 return 已自动存个数，else mode >0 自动存return 1存了一个，return 0 时间未到
            //return -1 表示是不在自动存
        }
    }
*/
    offset = C_OFF_STORE + 20;		//前次按键键值
    MCopyProtectedMemory( &prekeycode,(void*)offset,  1, PM_COPY_READ);
    prekeycode = prekeycode % 256;

    offset = C_OFF_STORE + 21;		//同一键值连续按键次数
    MCopyProtectedMemory( &samekeytime,(void*)offset,  1, PM_COPY_READ);

    offset = C_OFF_STORE + 22;		//前次按键实际时间
    MCopyProtectedMemory( &PreElapsedTime,(void*)offset,  4, PM_COPY_READ);

    if(prekeycode != keycode || keycode == 0)
    {
        if(keycode > 0)
        {
            offset = C_OFF_STORE + 22;		//新按键时间
            MCopyProtectedMemory( (void*)offset, &elapsedtime, 4, PM_COPY_WRITE);
        }
//		if(keycode != 0)
        {
            offset = C_OFF_STORE + 20;		//新键值
            MCopyProtectedMemory( (void*)offset, &keycode, 1, PM_COPY_WRITE);
            samekeytime = 0;
        }
        PreElapsedTime = elapsedtime ;
    }

    prekeycode = keycode;
    if(samekeytime < 200)samekeytime++;
    offset = C_OFF_STORE + 21;
    MCopyProtectedMemory( (void*)offset, &samekeytime, 1, PM_COPY_WRITE);
    PreElapsedTime = elapsedtime - PreElapsedTime;	//同一按键时长

    offset = C_OFF_STORE + 26;
    MCopyProtectedMemory( (void*)offset, &PreElapsedTime, 4, PM_COPY_WRITE);

//;;Write_Number(10,30,keycode,4,0,0);
    // keycode = KEY_DIGITAB[keycode];
//;;Write_Number(10,40,keycode,4,0,0);
//;;Write_Number(10,50,C_KEYCODMAX,4,0,0);
//;;Write_Number(10,60,keycode < C_KEYCODMAX,4,0,0);
//;;Write_Number(10,70,keycode != C_KEYCOD_SAVE,4,0,0);
//;;Write_Number(10,80,MGetAcquisitionEnable(),4,0,0);
//;;Write_Number(10,90,(keycode < C_KEYCODMAX) && (keycode != C_KEYCOD_SAVE) && MGetAcquisitionEnable() ,4,0,0);
    if( (MGetSoundAlarm(0)==0) && (keycode >= 0) && (keycode < C_KEYCODMAX) )
    {
        BuzzerOn(1);
    }
    //else

//	if( MGetSystemMode() == 0  && (keycode < C_KEYCODMAX) && (keycode != C_KEYCOD_SAVE) && MGetAcquisitionEnable() )ClearEnvelope();	//清包络或峰值

           // if( elapsedtime1 + 1000 < GetElapsedTime() )
            {
                elapsedtime1=GetElapsedTime();
                if( MGetAcquisitionEnable() == 1 )
                {
                    PeakMemory();		//峰值记忆
                    EchoEnvelope();		//回波包络
                    //		DepthCompensate();		//深度补偿
    //			AutoSave(1);//if mode < 0 return 已自动存个数，else mode >0 自动存return 1存了一个，return 0 时间未到
                    //return -1 表示是不在自动存
                }
            }

//	if(keycode < C_KEYCODMAX)
//		MSetAcquisitionEnable(MGetAcquisitionEnable(),C_SETMODE_SETSAVE);
    return keycode;
}
int MGetAnyKey(u_int time)
{
    int keycode;
    do
    {
        keycode = MGetKey();//MGetKeyCode(10);
        ExpendTime(time);
        if (keycode >= 0 && keycode < C_KEYCODMAX)break;
    }
    while(1);

    if(keycode < C_KEYCODMAX && MGetSoundAlarm(0) == 0)BuzzerOn(1);
    return keycode;
}

int MAnyKeyReturn(void)		/*直到释放当前按键并有新的按键才退出*/
{
    MKeyRlx();		/*只有按键已释放才退出*/
    int keycode;
    do
    {
        keycode = MGetKey();//MGetKeyCode(10);
        if (keycode >= 0 && keycode < C_KEYCODMAX)break;
    }
    while(true);
    if(keycode < C_KEYCODMAX && MGetSoundAlarm(0) == 0)BuzzerOn(1);
    return keycode;
}

int MAnyTimeReturn(u_int time)			/*直到有按键或时间到才退出 */
{
    int keycode = 0;
    int i = 0;
    MKeyRlx();		/*只有按键已释放才退出*/

    time = time/10 + 1;		/*至少查询一次键盘，每10＊10ms才查询一次 */
    for(i=0; i<time; i++)
    {
        /*消耗时间，以10ms为单位*/
        keycode = MGetKey();//MGetKeyCode(10);
        ExpendTime(10);
        if (keycode >= 0 && keycode < C_KEYCODMAX)break;
    }
    if(keycode < C_KEYCODMAX && MGetSoundAlarm(0) == 0)BuzzerOn(1);
    return keycode;
}

int MenuKeyReturn(int keymax,int mode)	/*出现菜单时，按不大于keymax的键返回，
	mode=0其它键无效，=1确认键有效，=2返回键有效，=3确认返回键有效*/
{
    int keycode;
    while (1)
    {
        keycode = MAnyKeyReturn();
        if(keymax > 0 && keycode == 0)continue;
        if ( (mode == 1 || mode == 3) && keycode == C_KEYCOD_CONFIRM)break;
        else if ( (mode == 2 || mode == 3) && keycode == C_KEYCOD_RETURN)break;
        else if (keycode <= Mabsi(keymax) && keycode >= 0)break;
    }
    //BuzzerOn(1) ;	 		/*峰鸣0.1 * 1 秒*/
    return keycode;
}

void MKeyRlx(void)		/*只有按键已释放才退出*/
{
    ExpendTime(20);
    while( MGetKey() < C_KEYCODMAX )
        {}
    //while( GetKeyIndex() > 0 )
    //{}
}

int CopyMemoryToSystem(void* memsrc, u_int bytesize,u_int copymode)
{
    u_int rev=0,i;

    u_int FileMax=bytesize;//(bytesize+4095)/4096*4096;
    char temp[FileMax];
    switch(copymode)
    {
    case PM_COPY_READ:

             rev=CopyProtectedMemory(MEMORY_STORAGE+ (int)memsrc, memsrc,FileMax, PM_COPY_READ);
       /* for(i=0; i<(FileMax/4096); i++)
        {
            rev=CopyProtectedMemory(MEMORY_STORAGE+ (int)memsrc+i*4096,memsrc+i*4096,4096, PM_COPY_READ);
            // ExpendTime(2);
        }*/
        break;
    case PM_COPY_WRITE:
        for(i=0; i<FileMax; i++)
        {
            temp[i] =MEMORY_STORAGE[i+ (int)memsrc];
        }
          rev=CopyProtectedMemory(memsrc, temp,FileMax, PM_COPY_WRITE);
        /*for(i=0; i<(FileMax/4096); i++)
        {
            rev=CopyProtectedMemory(memsrc+i*4096, temp+i*4096,4096, PM_COPY_WRITE);
            //  ExpendTime(2);
        }*/
        break;
    case PM_COPY_MOVE:
        break;
    default:
        break;
    }
    // ExpendTime(10);
    return rev;
}

int CopyMemoryAllData(u_int file_total,u_int copymode)
{
    long int FileMax=file_total*C_LEN_FILE;
    u_int length=C_LEN_FILE;
    char temp[FileMax];
    int offset = C_OFF_FILE;
    int rev=0;
    long int i;
    switch(copymode)
    {
    case PM_COPY_READ:
        for(i=0;i<file_total;i++)
        {
            rev=CopyProtectedMemory(MEMORY_STORAGE+offset+i*length, (void*)offset+i*length,length, PM_COPY_READ);
            Write_Number(10,100,i,4,0,0);
            MAnyKeyReturn();
        }

        break;
    case PM_COPY_WRITE:
        for(i=0; i<FileMax; i++)
        {
            temp[i] =MEMORY_STORAGE[i];
        }
           rev=CopyProtectedMemory((void*)offset, temp,FileMax, PM_COPY_WRITE);
        break;
    case PM_COPY_MOVE:
        break;
    }
   // ExpendTime(10);
    return rev;

}

int CopyMemoryAllSystem(u_int copymode)
{
    int offset = C_OFF_BATTERY;//C_OFF_INIT;
    int FileMax=C_OFF_FILEINFO-C_OFF_BATTERY;//(C_OFF_FILE+4095)/4096*4096;//C_OFF_FILEMAX/4;C_OFF_FILE
    char temp[FileMax];
    int rev=0;
    int i;
    switch(copymode)
    {
    case PM_COPY_READ:
            rev=CopyProtectedMemory(MEMORY_STORAGE+offset, (void*)offset,FileMax, PM_COPY_READ);
        break;
    case PM_COPY_WRITE:
        for(i=0; i<FileMax; i++)
        {
            temp[i] =MEMORY_STORAGE[i+offset];
        }
           rev=CopyProtectedMemory((void*)offset, temp,FileMax, PM_COPY_WRITE);
        break;
    case PM_COPY_MOVE:
        break;
    }
    ExpendTime(10);
    return rev;
}

int MCopyProtectedMemory(void* memdes, void* memsrc, u_int bytesize, u_int copymode)
{
    switch(copymode)
    {
    case PM_COPY_READ:
        memcpy(memdes,MEMORY_STORAGE + (int) memsrc, bytesize);
        break;
    case PM_COPY_WRITE:
        memcpy(MEMORY_STORAGE + (int)memdes,memsrc,bytesize );
        break;
    case PM_COPY_MOVE:
        memmove(MEMORY_STORAGE + (int)memdes,MEMORY_STORAGE + (int)memsrc,bytesize);
        break;
    }
    return 1;
    // return CopyProtectedMemory(memdes, memsrc,  bytesize,  copymode) ;
}

void MFclearScreen(void)
{

    int offset = C_OFF_SAMP;
    u_char sampbuff[C_LEN_SAMP+1];
    int i;
    for( i = 0 ; i <= C_LEN_SAMP; i++)sampbuff[i] = 0;
    MCopyProtectedMemory( (void*)offset, sampbuff, C_LEN_SAMP, PM_COPY_WRITE);//清包络存储区

    EraseWindow(0, 0, C_HORIDOT_VIDEO-10, C_VERTDOT_VIDEO) ;
}

void MCTextOut(u_int x, u_int y, u_short* pzText, u_int len)
{
    int i;
    x += HoriOffsetScreen;
    y += VertOffsetScreen;

    for(i=0; i<len; i++)
    {
        WriteCharDots24((u_short *)pzText+i,x+i*C_CCHAR_HDOT, y) ;
    }
    //  CTextOut(x , y,  pzText,  len) ;
    /*  for(i=0; i<len; i++)
      {
          WriteCharDots32((u_short *)pzText+i,x+i*C_CCHAR_HDOT, y) ;
      }*/
}

void METextOut(u_int x, u_int y, char* pzText, u_int len)
{
    int i;
    // x += HoriOffsetScreen;
    // y += VertOffsetScreen;
    //Ex (x , y,  pzText,  len) ;
    for(i=0; i<len; i++)
    {
        // WriteCharDots24((u_char *)pzText+i,x+i*C_ECHAR_HDOT, y) ;
        WriteCharDots16((u_char *)pzText+i,x+i*C_ECHAR_HDOT, y) ;
    }
}

void MDrawLine(u_int x1, u_int y1, u_int x2, u_int y2,u_int BGColor )   //BGColor,背景色
{
    int temp;

    x1 += HoriOffsetScreen;
    x2 += HoriOffsetScreen;
    y1 += VertOffsetScreen;
    y2 += VertOffsetScreen;
    if( y1 > y2 )
    {
        temp = x1;
        x1 = x2;
        x2 = temp;
        temp = y1;
        y1 = y2;
        y2 = temp;
    }
    if( y2 >= C_VERTDOT_VIDEO )
    {
        if( y1 >= C_VERTDOT_VIDEO )return;
        x2 = x1 + ( x2 - x1 )*(C_VERTDOT_SCREEN - 1 - y1 )/( y2 - y1);
        y2 = C_VERTDOT_SCREEN - 1;
    }
    if( x2 >= C_HORIDOT_VIDEO)
    {
        if( x1 >= C_HORIDOT_VIDEO)return;
        y2 = y1 + ( y2 - y1 )*( C_HORIDOT_VIDEO - 1 - x1)/(x2-x1);
        x2 =  C_HORIDOT_VIDEO - 1;
    }
#if C_DEVLIB == 23
    ;
    SetDisplayColor( curr_cr ^ c_crPara[crPara[ BGColor] ] );
    ;
    SetDisplayMode(1);
    DrawLine(x1, y1, x2, y2);
    ;
    SetDisplayMode(0);
    ;
    SetDisplayColor( curr_cr );
#else
    ;
    SetDisplayMode(1);
    DrawLine(x1, y1, x2, y2);
    ;
    SetDisplayMode(0);
#endif

}
void MDrawPixel(int x, int y, int mode)
{
    if( x < 0 || y < 0 || x > C_HORIDOT_VIDEO || y >= C_VERTDOT_SCREEN - 1) ;
    else
    {
        x += HoriOffsetScreen;
        y += VertOffsetScreen;
#if C_DEVLIB == 23 || C_DEVLIB == 4
        ;
        SetDisplayColor( curr_cr ^ c_crPara[crPara[ C_CR_BACK] ] );
        ;
        SetDisplayMode(1);
        DrawPixel(x , y) ;
        ;
        SetDisplayMode(0);
        ;
        SetDisplayColor( curr_cr );
#else
        ;
        SetDisplayMode(1);
        DrawPixel(x , y,mode) ;
        ;
        SetDisplayMode(0);
#endif
    }
}
u_int MGetPixels(u_int x, u_int y, u_int len)
{
    int i,j,k;
#if C_COLOR == 1
    x += HoriOffsetScreen;
    y += VertOffsetScreen;
    for(i = 0, j = 0; i < len; i++)
    {
        j <<= 1;
#if C_DEVLIB == 23
        k = GetPixel(x, y) ;
        if( x%2 == 0 && ( ( k  & 0xff00) > 0 ) )j++;
        else if(x%2 != 0 && ( ( k  & 0xff) > 0 ) )j++;
#else
        if( GetPixels(x, y, 1) > 0) j++;
#endif
    }
    return j;
#else
    x += HoriOffsetScreen;
    y += VertOffsetScreen;
    i = 0;
    j = 0;
    k = 0;
#if C_DEVLIB == 23
    k = GetPixel(x, y) ;
    if( x%2 == 0 && ( ( k  & 0xff00) > 0 ) )j++;
    else if(x%2 != 0 && ( ( k  & 0xff) > 0 ) )j++;
    return j ;
#else
    return GetPixels(x, y, len) ;
#endif

#endif
}
void m_DrawLine(  int x1,int y1,int x2,int y2)//在pDC所指在设备上从点(x1,y1)到(x2,y2)画直线
{
    MDrawLine( x1,y1,x2,y2,C_CR_BACK);
}
void m_DrawDot(  int x,int y)//在pDC所指在设备上从画点(x,y)
{
    MDrawPixel( x, y, DP_SET) ;
}

int m_DrawArc1( int x1,int y1,int x2,int y2,int mode)//在pDC所指在设备上从点(x1,y1)/(x2,y2)的圆弧,mode=0直线1向上凸－1向下凹
{
    int x,y,radius;//圆心/半径
    double angle,start_angle;
    int x3,y3;

    if ( x1 > x2 )
    {
        //如果第一点在第二点的右边，则交换
        x3 = x1;
        y3 = y1;
        x1 = x2;
        y1 = y2;
        x2 = x3;
        y2 = y3;
    }
    radius = 0;
    if( mode == 0)
    {
        m_DrawLine(x1,y1,x2,y2);
    }
    else
    {
        //－1向下左凹,1向上右凸
        int h,v,r;
        if( x1 == x2)
        {
            //垂直两点
            y = ( y1 + y2 )/2;
            radius = (y1 - y2)*mode;
            angle = asin( (double)( y1 - y2 )/(2*radius) )*180/PI;
            if( angle < 0)angle *= -1;
            if( radius < 0 )radius *= (-1);
            if( mode > 0)
            {
                m_DrawArc2( x1 - (UINT)( radius * sin( angle ) +0.5),y,mode*radius,angle,2*angle);
            }
            else m_DrawArc2( x1 + (UINT)( radius * sin( angle ) +0.5),y,(-1)*mode*radius,180+angle,2*angle);
        }
        else if( y1 == y2)
        {
            x = (x1+x2)/2;
            radius = (x1-x2)*mode;
            angle = asin( (double)( x1-x2)/(2*radius) )*180/PI;
            if( angle < 0)angle *= -1;
            if( radius < 0 )radius *= (-1);
            if( mode > 0)
            {
                m_DrawArc2( x,(UINT)( y1 + radius * sin( angle*PI/180 ) + 0.5),mode*radius,angle,2*angle);
            }
            else m_DrawArc2( x,(UINT)( y1 - radius * sin( angle*PI/180 ) + 0.5),(-1)*mode*radius,180+angle,2*angle);
        }
        else if( y2 > y1 )//第二点在下
        {
            x = x2 - x1;
            y = y2 - y1;//临时赋值
            if( x > y )
            {
                radius = (UINT)( ( x*x + y * y ) /( 2* y ) + 0.5);
                angle = ( atan( (double)(x)/(double)( radius - y) ) * 180/PI + 0.5);

                if( mode > 0)//1向上右凸
                {
                    x = x1;
                    y = y1 + radius;
                    m_DrawArc2( x,y, radius, 90.0,angle);
                }
                else//－1向下左凹
                {
                    x = x2;
                    y = y2 - radius;
                    m_DrawArc2( x,y, radius, 270.0,angle);
                }
            }
            else if( x2 - x1 < y2 - y1 )
            {
                radius = (UINT)( ( x*x + y * y ) /( 2* x ) + 0.5);
                angle = ( atan( (double)(y)/(double)( radius - x) ) * 180/PI + 0.5);
                if( mode > 0)//1向上右凸
                {
                    x = x2 - radius;
                    y = y2;
                    m_DrawArc2( x,y, radius, angle,angle);
                }
                else//－1向下左凹
                {
                    x = x1 + radius;
                    y = y1;
                    m_DrawArc2( x,y, radius, 180.0+angle, angle);
                }
            }
            else
            {
                //水平等于垂直长
                radius = x;
                angle = 90.0;

                h = v = radius/2;
                r = (UINT)( sqrt( v * v + h * h) + 0.5 );
                if( mode > 0)//1向上右凸
                {
                    //	x = x1 + (x2-x1)*mode;
                    //	y = y1 + (radius - y1 + y2 )* mode;
                    //	radius *= mode;
                    mode--;
                    x = x2 + h * mode;
                    y = y2 + v + v * mode;
                    radius += mode * r;

                    start_angle = ( ( atan( (double)(x-x1)/(double)( y - y1) ) ) * 180/PI + 0);
                    if( y > y2 )angle = ( atan( (double)(x - x2)/(double)( y - y2) ) * 180/PI + 0);
                    else angle = 0;
                    angle = start_angle - angle;
                    start_angle += 90;
                    m_DrawArc2( x,y, radius, start_angle,angle);
                }
                else//－1向下左凹
                {
//					x = x1;
//					y = y1 - radius;
//					m_DrawArc2( x,y, radius, 90.0,angle);
                    mode = (-1 )* mode - 1;
                    x = x1 + h * mode;
                    y = y1 - v - v * mode;
                    radius += mode * r;

                    start_angle = ( ( atan( (double)(x2-x)/(double)( y2 - y) ) ) * 180/PI + 0.5);
                    if( y < y1 )angle = ( atan( (double)(x1 - x)/(double)( y1 - y) ) * 180/PI + 0.5);
                    else angle = 0;
                    angle = start_angle - angle;
                    start_angle += 270;
                    m_DrawArc2( x,y, radius, start_angle,angle);
                }

            }
        }
        else//第二点在上
        {

            x = x2 - x1;
            y = y1 - y2;//临时赋值
            if( x > y )
            {
                ////第二点在上,水平比垂直长
                radius = (UINT)( ( x*x + y * y ) /( 2* y ) + 0.5);
                angle = ( atan( (double)(x)/(double)( radius - y) ) * 180/PI + 0);

                v = radius - y;
                h = (UINT)( tan( angle*PI/360.0) * v+0.5);
                r = (UINT)( sqrt( v * v + h * h) + 0.5 );
                if( mode > 0)//1向上右凸
                {
                    //	x = x1 + (x2-x1)*mode;
                    //	y = y1 + (radius - y1 + y2 )* mode;
                    //	radius *= mode;
                    mode--;
                    x = x2 + h * mode;
                    y = y2 + v + v * mode;
                    radius += mode * r;

                    start_angle = ( ( atan( (double)(x-x1)/(double)( y - y1) ) ) * 180/PI + 0);
                    if( y > y2 )angle = ( atan( (double)(x - x2)/(double)( y - y2) ) * 180/PI + 0);
                    else angle = 0;
                    angle = start_angle - angle;
                    start_angle += 90;
                    m_DrawArc2( x,y, radius, start_angle,angle);
                }
                else//－1向下左凹
                {
//					x = x1;
//					y = y1 - radius;
//					m_DrawArc2( x,y, radius, 90.0,angle);
                    mode = (-1 )* mode - 1;
                    x = x1 + h * mode;
                    y = y1 - v - v * mode;
                    radius += mode * r;

                    start_angle = ( ( atan( (double)(x2-x)/(double)( y2 - y) ) ) * 180/PI + 0.5);
                    if( y < y1 )angle = ( atan( (double)(x1 - x)/(double)( y1 - y) ) * 180/PI + 0.5);
                    else angle = 0;
                    angle = start_angle - angle;
                    start_angle += 270;
                    m_DrawArc2( x,y, radius, start_angle,angle);
                }
            }
            else if( x < y )//第二点在上
            {
                //垂直比水平长x《y
                radius = (UINT)( ( x*x + y * y ) /( 2* x ) + 0.5);
                angle = ( atan( (double)(y)/(double)( radius - x) ) * 180/PI + 0);

                h = radius - x;
                v = (UINT)(h/tan( angle*PI/360.0) + 0.5);//角度的一半
                r = (UINT)( sqrt( v * v + h * h) + 0.5 );

                if( mode > 0)//1向上右凸
                {
//					x = x1 + radius;
//					y = y1;
//					m_DrawArc2( x,y, radius, 180+angle,angle);
                    h = radius - x;
                    v = (UINT)(h*tan( angle*PI/360.0) + 0.5);//角度的一半
                    r = (UINT)( sqrt( v * v + h * h) + 0.5 );
                    mode--;
                    x = x2 + h + h * mode;
                    y = y2 + v + v * mode;
                    radius += mode * r;

                    start_angle = 180 - ( atan( (double)( y - y1)/(double)(x - x1) ) * 180/PI + 0);
                    if( x > x2 )angle = 180 - ( ( atan( (double)( y - y2)/(double)(x-x2) ) ) * 180/PI + 0);
                    angle = start_angle - angle;
                    //start_angle += 90;
                    m_DrawArc2( x,y, radius, start_angle,angle);
                }
                else//－1向下左凹
                {
//					x = x2 - radius;
//					y = y2;
//					m_DrawArc2( x,y, radius, 360.0, angle);
                    mode = ( -1 )* mode -1;
                    x = x2 - h - h * mode;
                    y = y2 - v * mode;
                    radius += mode * r;

                    start_angle = 90 - ( ( atan( (double)( y2 - y)/(double)(x2-x) ) ) * 180/PI + 0);
                    if( x1 > x )angle = 90 - ( atan( (double)( y1 - y)/(double)(x1 - x) ) * 180/PI + 0);
                    else angle = 0;
                    angle = start_angle - angle;
                    start_angle += 270;
                    m_DrawArc2( x,y, radius, start_angle,angle);
                }
            }
            else //第二点在上
            {
                //垂直等水平
                radius = x;
                angle = 90;

                h = v = radius/2;
                r = (UINT)( sqrt( v * v + h * h) + 0.5 );

                if( mode > 0)//1向上右凸
                {
//					x = x1 + radius;
//					y = y1;
//					m_DrawArc2( x,y, radius, 180+angle,angle);
                    mode--;
                    x = x2 + 0 + h * mode;
                    y = y1 + 0 + v * mode;
                    radius += mode * r;

                    start_angle = 180 - ( atan( (double)( y - y1)/(double)(x - x1) ) * 180/PI + 0);
                    if( x > x2 )angle = 180 - ( ( atan( (double)( y - y2)/(double)(x-x2) ) ) * 180/PI + 0);
                    else angle = 90;
                    angle = start_angle - angle;
                    //start_angle += 90;
                    m_DrawArc2( x,y, radius, start_angle,angle);
                }
                else//－1向下左凹
                {
//					x = x2 - radius;
//					y = y2;
//					m_DrawArc2( x,y, radius, 360.0, angle);
                    mode = ( -1 )* mode -1;
                    x = x1 - 0 - h * mode;
                    y = y2 - v * mode;
                    radius += mode * r;

                    start_angle = 90 - ( ( atan( (double)( y2 - y)/(double)(x2-x) ) ) * 180/PI + 0);
                    if( x1 > x )angle = 90 - ( atan( (double)( y1 - y)/(double)(x1 - x) ) * 180/PI + 0);
                    else angle = 0;
                    angle = start_angle - angle;
                    start_angle += 270;
                    m_DrawArc2( x,y, radius, start_angle,angle);
                }
            }
        }
    }
    return radius;

}
//在pDC所指在设备上以点(x,y)为圆心,radius为半径的圆环,start_angle为起始角度,水平x方向为0度
//angle为从起始值顺时针方向旋转角度值,
void m_DrawArc2(int x,int y,int radius,double start_angle,double angle)
{
    int h,v,p;
    double end_angle;

    end_angle = start_angle - angle;

    if( end_angle < 0 && start_angle > 0)
    {
        //横跨第一和第四象限
        m_DrawArc2(  x, y, radius, 0.0, 0.0-end_angle);
    }
    else if( start_angle <= 0)
    {
        start_angle += 360;
        end_angle += 360;
    }
    h = x;
    v = y;//圆心
    p = 1 - radius;
    x = 0 ;
    y = radius;
    if( start_angle > 360 || angle > 360 )return;//起始角度/角度大于360度则退出

    while(x < y)  // 根据圆的对称性，从(h,v + radius)开始，顺时钟画45度角的范围即可
    {
        if(p < 0)
        {
            //还在圆心的左面
            p += (x<<1) + 1 ;
        }
        else
        {
            //已到圆心的右面
            y -- ;
            p += ( (x - y)<<1 ) + 1 ;
        }

        if( y == 0)angle = 0;
        else angle = ( atan( (double)x/(double)y ) * 180/PI );
        if( angle <= start_angle && angle >= end_angle && angle <= 45)m_DrawDot(  h + y, v - x) ;	// 0+45
        angle = 180 - angle;
        if( angle <= start_angle && angle >= end_angle && angle > 90+45 && angle <= 180)m_DrawDot(  h - y, v - x) ;	// 左下180-45

        if( y == 0)angle = 0;
        else angle = ( 360 + atan( (-1)*(double)x/(double)y ) * 180/PI );
        if( angle <= start_angle && angle >= end_angle )
            if(angle <= start_angle && angle >= end_angle && angle > 270 + 45 && angle <= 360)m_DrawDot(  h + y, v + x) ;	// 右下0-45
        angle = 360 - angle + 180;
        if( angle <= start_angle && angle >= end_angle && angle > 180 && angle <= 180 + 45)m_DrawDot(  h - y, v + x) ;	// 右下180+45

        if( x == 0)angle = 90;
        else angle = ( atan( (double)y/(double)x ) * 180/PI );
        if( angle <= start_angle && angle >= end_angle && angle > 90-45 && angle <= 90)m_DrawDot(  h + x, v - y) ;	// 右下90-45
        angle = angle + 180;
        if( angle <= start_angle && angle >= end_angle && angle > 270-45 && angle <= 270)m_DrawDot(  h - x, v + y) ;	// 右下270-45

        if( x == 0)angle = 90;
        else angle = ( 180 + atan( (-1)*(double)y/(double)x ) * 180/PI );
        if( angle <= start_angle && angle >= end_angle && angle > 90 && angle <= 90 + 45) m_DrawDot(  h - x, v - y) ;	// 右下90+45
        angle = angle + 180;
        if( angle <= start_angle && angle >= end_angle && angle > 270 && angle <= 270 + 45)m_DrawDot(  h + x, v + y) ;	// 右下270+45

        x++ ;

    }
}
//在pDC所指在设备上从点(x1,y1)/(x2,y2)的二次弧线,mode=0直线1向上凸－1向下凹
int m_DrawArc3( int x1,int y1,int x2,int y2,int mode)
{
    int x,y;
    if ( x1 > x2 )
    {
        //如果第一点在第二点的右边，则交换
        x = x1;
        y = y1;
        x1 = x2;
        y1 = y2;
        x2 = x;
        y2 = y;
    }
//	if( x1 > x2 )return false;

    double a,b,c;
    //y = a*(x-b)^2 + c
    if( ( mode < 0 && y1 < y2 ) || ( mode > 0 && y1 > y2) )
    {
        b = x2;
        c = y2;
        a = ( y1 - c)/( ( x1 - b)*( x1 - b) );
    }
    else
    {
        b = x1;
        c = y1;
        a = ( y2 - c)/( ( x2 - b)*( x2 - b) );
    }

    if( ( x2 - x1 > y2 - y1 && y2 > y1 ) ||
            ( x2 - x1 > y1 - y2 && y1 > y2 ) )
    {
        //x>y
        mode = 1;
    }
    else if( ( x2 - x1 < y2 - y1 && y2 > y1 ) ||
             ( x2 - x1 < y1 - y2 && y1 > y2 ) )
    {
        //x<y
        mode = -1;
    }
    else mode = 0 ;//x=y

    for( x = x1+1 ; x <= x2; x++)
    {
        if( y1 > y2 )y = (UINT)( a * ( x - b)*( x - b) + c - 0.5);
        else y = (UINT)( a * ( x - b)*( x - b) + c + 0.5);
        if( y != y1 || x == x2)m_DrawLine( x1,y1,x-1,y);
        /*
        		if( mode > 1)
        		{//x>y
        			x1 = x+1;
        			y1 = y;
        		}
        		else
        		{
        			x1 = x;
        			if( y > y1 )y1 = y+1;
        			else y1 = y-1;
        		}
        /*/
        if( y1!= y)
        {
            x1 = x;
            y1 = y;
        }
//*/
    }
    return 1;
}
void m_DrawCircle(int x,int y,int radius)
{
    int h,v,p;

    h = x;
    v = y;//圆心
    p = 1 - radius;
    x = 0 ;
    y = radius;

    // 首先画垂直方向的四点
    m_DrawDot(  h , v +  radius) ;	// 上边
    m_DrawDot(  h , v -  radius) ;	// 下边
    m_DrawDot(  h - radius , v ) ;		// 左边
    m_DrawDot(  h + radius , v ) ;		// 右边

    while(x < y)  // 根据圆的对称性，从(h,v + radius)开始，顺时钟画45度角的范围即可
    {
        if(p < 0)
        {
            //还在圆心的左面
            p += (x<<1) + 1 ;
        }
        else
        {
            //已到圆心的右面
            y -- ;
            p += ( (x - y)<<1 ) + 1 ;
        }
        m_DrawDot(  h + x, v + y) ;	// 上右270+45
        m_DrawDot(  h - x, v + y) ;	// 上左270-45
        m_DrawDot(  h + x, v - y) ;	// 下右90-45
        m_DrawDot(  h - x, v - y) ;	// 下左90+45
        if( x != y && x + y != 0)//不相等也不相反
        {
            m_DrawDot(  h + y, v + x) ;	// 右上0-45
            m_DrawDot(  h - y, v + x) ;	// 右下0+45
            m_DrawDot(  h + y, v - x) ;	// 左上180-45
            m_DrawDot(  h - y, v - x) ;	// 左下180+45
        }

        x++ ;

    }
}

#define C_PUSH_TIME 2
u_int push_left[C_PUSH_TIME],push_top[C_PUSH_TIME],push_top_init, push_right[C_PUSH_TIME],push_bottom[C_PUSH_TIME];
int MPushWindow(u_int left, u_int top, u_int right, u_int bottom)
{
    int i;
    i = 0;



    left += HoriOffsetScreen;
    right += HoriOffsetScreen;
    top += VertOffsetScreen;
    bottom += VertOffsetScreen;
    if(top >= C_VERTDOT_VIDEO)top = C_VERTDOT_VIDEO-1;
    if( bottom >= C_VERTDOT_VIDEO)bottom = C_VERTDOT_VIDEO-1;
    if( left >= C_HORIDOT_VIDEO) left = C_HORIDOT_VIDEO-1;
    if( right >= C_HORIDOT_VIDEO) right = C_HORIDOT_VIDEO-1;

#if C_DEVLIB == 23
//	MPush( left);
//	MPush( right);
//	MPush( top);
//	MPush( bottom);
    for( i = C_PUSH_TIME-1; i > 0; i--)
    {
        push_left[i] = push_left[i-1];
        push_top[i] = push_top[i-1];
        push_right[i] = push_right[i-1];
        push_bottom[i] = push_bottom[i-1];
    }
    push_left[0] = left;
    push_top[0] = top;
    push_right[0] = right;
    push_bottom[0] = bottom;
    PushWindow( (C_HORIDOT_SCREEN+10)*65536+left, push_top_init*65536+top, right-left, bottom-top);
    push_top_init += (bottom - top);
#else
    PushWindow( left, top, right, bottom);
#endif

    return 0;
}
int MPopWindow(void)
{
    int i;
    i = 0;


    /*
    	left += HoriOffsetScreen;
    	right += HoriOffsetScreen;
    	top += VertOffsetScreen;
    	bottom += VertOffsetScreen;
    	if(top >= C_VERTDOT_VIDEO)top = C_VERTDOT_VIDEO-1;
    	if( bottom >= C_VERTDOT_VIDEO)bottom = C_VERTDOT_VIDEO-1;
    	if( left >= C_HORIDOT_VIDEO) left = C_HORIDOT_VIDEO-1;
    	if( right >= C_HORIDOT_VIDEO) right = C_HORIDOT_VIDEO-1;
    */
#if C_DEVLIB == 23
//	int left,right,top,bottom;
//	bottom = MPop(1);
//	top = MPop(1);
//	right = MPop(1);
//	left = MPop(1);
    push_top_init -= (push_bottom[0]-push_top[0]);
    PopWindow( (10+C_HORIDOT_SCREEN)*65536+push_left[0], push_top_init*65536+push_top[0], push_right[0]-push_left[0], push_bottom[0]-push_top[0]);

    for( i = 0; i < C_PUSH_TIME-1; i++)
    {
        push_left[i] = push_left[i+1];
        push_top[i] = push_top[i+1];
        push_right[i] = push_right[i+1];
        push_bottom[i] = push_bottom[i+1];
    }
#else
    PopWindow();
#endif

    return 0;
}

void MEraseWindow(u_int left, u_int top, u_int right, u_int bottom)
{
    left += HoriOffsetScreen;
    right += HoriOffsetScreen;
    top += VertOffsetScreen;
    bottom += VertOffsetScreen;
    if(top >= C_VERTDOT_VIDEO)top = C_VERTDOT_VIDEO-1;
    if( bottom >= C_VERTDOT_VIDEO)bottom = C_VERTDOT_VIDEO-1;
    if( left >= C_HORIDOT_VIDEO) left = C_HORIDOT_VIDEO-1;
    if( right >= C_HORIDOT_VIDEO) right = C_HORIDOT_VIDEO-1;
#if C_DEVLIB == 23
    ;
    SetDisplayMode(0);
    EraseWindow( left, top, right-left, bottom-top);
///	SetDisplayMode(1);
#else
    EraseWindow( left, top, right, bottom);
#endif
}

void MInvertWindow(u_int left, u_int top, u_int right, u_int bottom)
{
    left += HoriOffsetScreen;
    right += HoriOffsetScreen;
    top += VertOffsetScreen;
    bottom += VertOffsetScreen;
    if(top >= C_VERTDOT_VIDEO)top = C_VERTDOT_VIDEO-1;
    if( bottom >= C_VERTDOT_VIDEO)bottom = C_VERTDOT_VIDEO-1;
    if( left >= C_HORIDOT_VIDEO) left = C_HORIDOT_VIDEO-1;
    if( right >= C_HORIDOT_VIDEO) right = C_HORIDOT_VIDEO-1;
#if C_DEVLIB == 23
    InvertWindow( left, top, right-left, bottom-top);
#else
    InvertWindow( left, top, right, bottom);
#endif
}

/* activate/deactivate DAC funtion , bOn != 0 -> activate else deactivate */
void	MActivateDac(int bOn)
{
#if C_DEVLIB == 1 ||  C_DEVLIB == 2 ||  C_DEVLIB == 23 || C_DEVLIB == 24
//    ActivateDac(bOn) ;
    /* switch dynamic gain on(1) and off(0) */
//    AmplifierDynamicGainOnOff(bOn) ;
#endif
}
/* peak amplitude dac adjusting */
void   MDacGatePeak()
{
#if C_DEVLIB == 1 ||  C_DEVLIB == 2 ||  C_DEVLIB == 2
    if(MGetFunctionMode(C_DEPTH_COMPENSATE))DacGatePeak()  ;
#endif
}

int mSendByteToPrinter(u_int pdata)
{
    u_int fn, tm0, tm ;


    tm0 = GetElapsedTime() ;

    do
    {
        //fn = SendByteToPrinter(pdata) ;
        tm = GetElapsedTime() - tm0 ;
    }
    while((!fn)) ;

    return fn;
}

void MSetEchoColor(u_int pos_s, u_int pos_e, u_int rgb)
{
    u_int Color;
#if C_COLOR == 1 || C_DEVLIB == 23
    if( pos_e >= C_SCREEN_WIDTH)pos_e = C_SCREEN_WIDTH - 1;
    else if( pos_e < 1)pos_e = 1;
    if( pos_s >= pos_e )pos_s = pos_e - 1;
    rgb &= 0xff;

    rgb ^= Echo_crPara[crPara[ C_CR_WAVEBACK] ];

    //Color=(((rgb&0x0018)>>3)+((rgb&0x0700)>>6)+((rgb&0xE000)>>8))&0xff;
// @todo (union#1#): 临时回波颜色

    // rgb = ECHO_COLOR_YELLOW;


#if C_DEVLIB == 4
    SetEchoColor( Color) ;

#else

    SetEchoColor(pos_s, pos_e,  rgb) ;
#endif

    //  UpdateEchoColor();



//	SetEchoColor(0, 300,  rgb) ;
#endif
}
u_int 	MSetEchoRange(u_int range)
{
    range = range * C_HORIUNIT / C_COORHORIUNIT;
#if C_DEVLIB == 4
    return SetEchoRange(MGetChannel(), range ) ;
#else
    return SetEchoRange(range);
#endif
}
void 	MSetDisplayColor(u_int rgb)
{
#if C_COLOR == 1 || C_DEVLIB == 23
    // rgb &= 0x0ff;
    curr_cr = rgb;
    //SetDisplayColor( curr_cr ^ c_crPara[crPara[ C_CR_BACK] ] );
    SetDisplayColor(rgb) ;
#endif

}
int DrawWave(u_char *buff, int start, int dots)
{
    int i ;
    int hpos,lpos = 0;
    u_char buffer[C_COORWIDTH + 1];

    if( start >= C_COORWIDTH)return -1;
    if( start + dots >= C_COORWIDTH)dots = C_COORWIDTH - start;
    for (i = start; i < start + dots; i++)	//截取超过屏幕的波
    {
        if(buff[i] >= 200)	buffer[i] = 199;
        else buffer[i] = buff[i];
    }

    for (i = start + 1; i < start + dots-1; i++)
    {
        hpos = buffer[i];

        if(buffer[i - 1] <= buffer[i] && buffer[i] <= buffer[i + 1])	/* h(i-1)<h(i)<h(i+1) */
        {
            lpos = buffer[i-1];
        }
        else if(buffer[i - 1] <= buffer[i] && buffer[i] >= buffer[i + 1])  /* 处理最高点 */
        {
            if(buffer[i - 1] <= buffer[i + 1])
            {
                lpos = buffer[i-1];
            }
            else
            {
                lpos = buffer[i+1];
            }
        }
        else if(buffer[i - 1] >= buffer[i] && buffer[i] >= buffer[i + 1])
        {
            lpos = buffer[i+1];
        }

        if(lpos == 0 && hpos == 0)continue;
        if(lpos == 0 && hpos > lpos) lpos++;
        MDrawLine(i, (C_COORVPOSI+C_COORHEIGHT-hpos*2), i, (C_COORVPOSI+C_COORHEIGHT-lpos*2) ,C_CR_WAVEBACK) ;

    }

    return 1;
}
u_int DrawMultiEdge(int xpos,int ypos,u_int edge,float angle,int len)	/*画多边形*/
{
    /*以指定位置为中心，画边数=edge，倾斜角=angle，顶点到中心距离为len的多边形*/

    int x0,y0,x1,y1,x2,y2;
    float angle_step = 2 * C_PI/edge ;
    int i;
    if (edge < 3)return C_FALSE;

    angle = angle * C_PI/180.0 ;/*化为弧度*/

    x2 = 0;
    y2 = 0;

    x0 = xpos + (int)( len * cos( angle ) ) ;
    y0 = ypos - (int)( len * sin( angle ) ) ;	/*第一个点*/
    x1 = x0;
    y1 = y0;
    for(i = 1; i< edge; i++)
    {
        angle += angle_step;
        x2 = xpos + (int)( len * cos( angle ) ) ;
        y2 = ypos - (int)( len * sin( angle ) ) ;
        CheckDrawLine(x1, y1,x2 ,y2) ;	 /* 画边 */
        x1 = x2 ;
        y1 = y2 ;
    }
    CheckDrawLine(x2, y2,x0 ,y0) ;	 /*最后一条边 */

    return C_TRUE;
}

int DrawCurver( short * buff,int startposi,int dotnumber)
{
    int endposi = startposi + dotnumber ;
    int x1,y1,x2,y2,y3,y4;
    int step = 1;		//每隔step点画一次
    int coor_top,coor_max;


#if C_DEVLIB == 23
    MSetColor(C_CR_CURVE);
    ;
    SetDisplayColor( curr_cr ^ c_crPara[crPara[ C_CR_WAVEBACK] ] );
    ;
    SetDisplayMode(1);
#endif
    if( dotnumber <= 0 )return -1;
    if( endposi >= C_COORWIDTH)
        endposi = C_COORWIDTH -1 ;
    x2 = startposi+HoriOffsetScreen;
    endposi += HoriOffsetScreen;
    coor_max = C_COORVPOSI + 400 + VertOffsetScreen;
    coor_top = C_COORVPOSI + VertOffsetScreen ;
    y2 = coor_max - buff[x2];
    if( y2 < coor_top) y2 = coor_top;

    while(x2 < endposi)
    {
        x1 = x2;
        y1 = y2;
        x2 += step;
        y2 = coor_max - buff[x2-HoriOffsetScreen];
        if( y2 < coor_top) y2 = coor_top;
        y3=(y1-C_COORVPOSI-VertOffsetScreen)+C_COORVPOSI+VertOffsetScreen;
        if( y3 <= coor_top) y3 = coor_top+1;
        y4=(y2-C_COORVPOSI-VertOffsetScreen)+C_COORVPOSI+VertOffsetScreen;
        if( y4 <= coor_top) y4 = coor_top+1;
        if(y3!=y4 && ((y3+1)<=(y4-1)))
        {
            DrawLine(x1, y3+1, x2-1, y4-1);
            DrawLine(x1+1, y3+1, x2, y4-1);
        }
        else
        {
            DrawLine(x1, y3, x2-1, y4);
            // DrawLine(x1, y3+1, x2-1, y4+1);
        }
        //DrawLine(x1, y3-1, x2-1, y4-1);
    }
#if C_DEVLIB == 23
    ;
    SetDisplayMode(0);
    ;
    SetDisplayColor( curr_cr );
    MSetColor(C_CR_UNDO);
#endif
    return 1;
}


int DrawEnvelope(	u_char * buff,int startposi,int dotnumber)
{
    int endposi = startposi + dotnumber ;
    int x1,y1,x2,y2;
    int step = 1;		//每隔step点画一次

    if( dotnumber <= 0 )return -1;
    if( endposi >= C_COORWIDTH)
        endposi = C_COORWIDTH -1 ;
    x2 = startposi;
    if( buff[x2] >= C_COORHEIGHT) y2 = C_COORVPOSI + 1;
    else y2 = C_COORVPOSI + C_COORHEIGHT - buff[x2];
    MSetColor(C_CR_ENVELOPE);
    do
    {
        x1 = x2;
        y1 = y2;
        x2 += step;
        if(x2 > endposi)break;
///		if(x2 > endposi)x2 = endposi;
        if( buff[x2] >= C_COORHEIGHT) y2 = C_COORVPOSI + 1;
        else y2 = C_COORVPOSI + C_COORHEIGHT - buff[x2];
        if(y1 == y2 && y2 == C_COORVPOSI + C_COORHEIGHT)continue;
        MDrawLine(x1,y1,x2-1,y2 ,C_CR_WAVEBACK);
    }
    while(x2 < endposi);
    MSetColor(C_CR_UNDO);
    return 1;
}

int DrawGateCurver( short * buff,int startposi,int dotnumber,int mode)
{
    //mode = 0都画1只画两标志2只画前标志3只画后标志,-1不画标志
    int endposi ;
    int x1,y1,x2,y2;
    int step = 1;		//每隔step点画一次

    if( dotnumber <= 0 )return -1;
    endposi = startposi + dotnumber ;
    if( endposi >= C_COORWIDTH)
        endposi = C_COORWIDTH -1 ;

    x2 = startposi;
    y2 = C_COORVPOSI + C_COORHEIGHT - buff[x2];

    MSetColor(C_CR_GATEA);

    if( x2 > 0 && ( mode == 0 || mode == 1 || mode == 2) )
    {
        if ( y2 > C_COORVPOSI+ 1)
        {
            MDrawLine(x2, C_COORVPOSI + 1, x2, y2-1,C_CR_WAVEBACK) ;	// left
        }
    }

    do
    {
        x1 = x2;
        y1 = y2;
        x2 += step;
        if(x2 > endposi)break;
///		if(x2 > endposi)x2 = endposi;
        y2 = C_COORVPOSI + C_COORHEIGHT - buff[x2];
        if(y1 == y2 && y2 == C_COORVPOSI + C_COORHEIGHT)continue;
        if( mode < 1)MDrawLine(x1,y1,x2-1,y2,C_CR_WAVEBACK);
    }
    while(x2 < endposi);

    if( x2 < C_COORWIDTH-1 &&  ( mode == 0 || mode == 1 || mode == 3) )
    {
        if ( y2 > C_COORVPOSI+ 1)
        {
            MDrawLine(x2, C_COORVPOSI + 1, x2, y2,C_CR_WAVEBACK) ;	// right
        }
    }

//	MDrawLine(endposi, y2-3, endposi, y2+5) ;	// right
    MSetColor(C_CR_UNDO);

    return 1;
}

void DrawRectangle(u_int left, u_int top, u_int right, u_int bottom)
{
    if( left > right )Swap(&left,&right);
    if( top > bottom)Swap(&top,&bottom);
    CheckDrawLine(left, top, left, bottom) ;	 /* 左边 */
    CheckDrawLine(left+1, top, right, top) ;	 /* 上边 */
    CheckDrawLine(right, top+1, right,bottom) ; /* 右边 */
    CheckDrawLine(left+1,  bottom, right-1, bottom) ;/* 下边 */
}

u_int CheckDrawLine(int x1,int y1,int x2,int y2)
{
    /*在屏幕内画一条线，超出屏幕范围不显示*/
    int temp;
    float tan_angle;

    if( x1 > x2 )
    {
        temp = x1;
        x1 = x2;
        x2 = temp;
        temp = y1;
        y1 = y2;
        y2 = temp;
    }	//都变成从左到右
    if(x2 <= 0 || x1 > C_HORIDOT_VIDEO )return C_FALSE;

    tan_angle = (float)( Mabsi(y2 - y1) )/(float)(x2 - x1) ;	//与水平方向夹角的正切
    if( x1 < 0 )
    {
        x1 = (int)( ( 0 - x1 ) * tan_angle );
        if(y1 > y2)y1 = y1 - x1;
        else y1 = y1 + x1;
        x1 = 0;
    }
    if( x2 >= C_HORIDOT_VIDEO )
    {
        x2 = (int)( (C_HORIDOT_VIDEO - x1) * tan_angle ) ;
        if(y1 > y2)y2 = y1 - x2;
        else y2 = y1 + x2;
        x2 = C_HORIDOT_VIDEO;
    }
    if( y1 > y2 )
    {
        temp = x1;
        x1 = x2;
        x2 = temp;
        temp = y1;
        y1 = y2;
        y2 = temp;
    }	//都变成从上到下
    if( y2 <= 0 || y1 >= C_VERTDOT_SCREEN )return C_FALSE;

    tan_angle = (float)( Mabsi(x2 - x1) )/(float)(y2 - y1) ;	//与垂直方向夹角的正切
    if( y1 < 1)
    {
        y1 = (int)( ( 1 - y1)  * tan_angle );
        if( x2 > x1 )x1 = x1 + y1;
        else x1 = x1 - y1;
        y1 = 1;
    }
    if( y2 > C_VERTDOT_SCREEN-2 )
    {
        y2 = (int)( (C_VERTDOT_SCREEN - 2 - y1) * tan_angle );
        if( x2 > x1 )x2 = x1 + y2;
        else x2 = x1 - y2;
        y2 = C_VERTDOT_SCREEN -2 ;
    }

    MDrawLine(x1,y1,x2,y2,C_CR_BACK);

    return C_TRUE;

}

/* 清屏并画矩形 */
void EraseDrawRectangle(u_int left, u_int top, u_int right, u_int bottom)
{
    MEraseWindow(left, top, right+1, bottom+1) ; /* 清除窗体 */
    DrawRectangle(left, top, right, bottom) ; /* 画矩形 */
    MSetColor(C_CR_MENU);
}




/* 画圆，圆心坐标和半径 */
void CirclePlotPoints(u_int, u_int, u_int, u_int) ;
void DrawCircle(u_int HCenter, u_int VCenter, u_int CircleRadius)
{
    int x = 0 ;
    int y = CircleRadius ;
    int p = 1 - CircleRadius ;


    /* 首先画垂直方向的四点 */
    MDrawPixel( HCenter , VCenter +  y, DP_NOT) ;	/* 上边 */
    MDrawPixel( HCenter , VCenter -  y, DP_NOT) ;	/* 下边 */
    MDrawPixel( HCenter - y, VCenter , DP_NOT) ;		/* 左边 */
    MDrawPixel( HCenter + y, VCenter , DP_NOT) ;		/* 右边 */


    while(x < y)  /* 根据圆的对称性，从(HCenter,VCenter + CircleRadius)开始，顺时钟画45度角的范围即可 */
    {
        if(p < 0)
            p += 2 * x + 1 ;
        else
        {
            y -- ;
            p += 2 * (x - y) + 1 ;
        }
        CirclePlotPoints(HCenter, VCenter, x, y) ;
        x++ ;

    }
}

void DrawArc1(int x1,int y1,int x2,int y2,int r)//画从点(x1,y1)到点(x2,y2)半径为r的弧线
{
    //目前假定两点在同一水平线上，y1=y2,则圆心坐标为（(x2+x1)/2,y2-sqrt(r^2-((x2-x1)/2)^2) );
    int cx1,cy1;//圆心
    int x = 0 ;
    int y = r ;
    int p = 1 - r ;

    cx1 = (x1+x2)/2;
    cy1 = y1 - sqrt( 10000*(r*r - ( cx1-x1)*( cx1-x1) )/100 );

    MDrawPixel( cx1 , cy1 +  y, DP_NOT) ;	/* 上边 */
    while(x < cx1-x1)  /* 根据圆的对称性，从(HCenter,VCenter + CircleRadius)开始，顺时钟画45度角的范围即可 */
    {
        if(p < 0)
            p += 2 * x + 1 ;
        else
        {
            y -- ;
            p += 2 * (x - y) + 1 ;
        }
        MDrawPixel( cx1 + x, cy1 + y, DP_NOT) ;
        MDrawPixel( cx1 - x, cy1 + y, DP_NOT) ;
        x++ ;

    }
}
/* 画圆周上的点 */
void CirclePlotPoints(u_int HCenter, u_int VCenter, u_int x, u_int y)
{
    MDrawPixel( HCenter + x, VCenter + y, DP_NOT) ;
    MDrawPixel( HCenter - x, VCenter + y, DP_NOT) ;
    MDrawPixel( HCenter + x, VCenter - y, DP_NOT) ;
    MDrawPixel( HCenter - x, VCenter - y, DP_NOT) ;
    if( Mabsi(x) != Mabsi(y) )
    {
        MDrawPixel( HCenter + y, VCenter + x, DP_NOT) ;
        MDrawPixel( HCenter - y, VCenter + x, DP_NOT) ;
        MDrawPixel( HCenter + y, VCenter - x, DP_NOT) ;
        MDrawPixel( HCenter - y, VCenter - x, DP_NOT) ;
    }
}
void DrawCursor(u_int xpos,u_int ypos,u_int mode)	/*画光标，mode=0英文1中文*/
{
    int operate = 0;	//如果进行了操作则为1，否则为0
    int offset;

    offset = C_OFF_STORE + 4;	//4-11		画光标时的光标位置xpos
    MCopyProtectedMemory( (void*)offset, &xpos, 4, PM_COPY_WRITE);
    offset = C_OFF_STORE + 8;	//4-11		画光标时的光标位置ypos
    MCopyProtectedMemory( (void*)offset, &ypos, 4, PM_COPY_WRITE);
    MSetColor(C_CR_PARA);
//	xpos++;
//	xpos++;
    switch ( mode )
    {
    case 0 :
//		ypos += C_ECHAR_VDOT+1;
//		MDrawLine(xpos,ypos,xpos+C_ECHAR_VDOT-1,ypos);
        MDrawLine(xpos,ypos,xpos,ypos+C_ECHAR_VDOT-1,C_CR_WAVEBACK);
        operate = 1;
        break;
    case 1:
//		ypos += C_CCHAR_VDOT+1;
//		MDrawLine(xpos,ypos,xpos+C_CCHAR_VDOT-1,ypos);
        MDrawLine(xpos,ypos,xpos,ypos+C_CCHAR_VDOT-1,C_CR_WAVEBACK);
        operate = 1;
        break;
    case 2:
//;		if(MGetFunctionMode(C_ECHO_ENVELOPE) || MGetFunctionMode(C_ECHOMAX_MEMORY) )
//;			break;	//如果包络或峰值记忆惊动则不画光标
        ypos -= 4;
        MDrawLine(xpos+1,ypos+1,xpos + 4,ypos + 4,C_CR_WAVEBACK);
        MDrawLine(xpos,ypos,xpos + 4,ypos - 4,C_CR_WAVEBACK);
        MDrawLine(xpos,ypos,xpos - 4,ypos + 4,C_CR_WAVEBACK);
        MDrawLine(xpos,ypos,xpos - 4,ypos - 4,C_CR_WAVEBACK);
        operate = 1;
        break;
    }
    MSetCursorShow( ( MGetCursorShow() + operate)%2  ,0);
    MSetColor(C_CR_UNDO);
}
void ClearCursor(u_int mode)	/*清光标，mode=0英文1中文*/
{
    int offset;
    u_int xpos,ypos;

    offset = C_OFF_STORE + 4;	//4-11		画光标时的光标位置xpos
    MCopyProtectedMemory(&xpos, (void*)offset, 4, PM_COPY_READ);
    offset = C_OFF_STORE + 8;	//4-11		画光标时的光标位置ypos
    MCopyProtectedMemory(&ypos, (void*)offset, 4, PM_COPY_READ);

    if( MGetCursorShow() > 0)	/*>0有光标*/
        DrawCursor(xpos,ypos,mode);
}


void CMenuOut(u_int xpos,u_int ypos,const u_short pzText[],u_int char_len,u_int row_number,u_int row_height)
{
    /*在指定位置根据每行字符数、行数、行高写菜单*/
    int i;
    for ( i=0; i< row_number ; i++ )
    {
        MCTextOut(xpos, ypos, (u_short *)pzText, char_len) ;
        ypos += row_height;
        pzText += char_len;
    }
}

void EMenuOut(u_int xpos,u_int ypos,const u_char* pzText,u_int char_len,u_int row_number,u_int row_height)
{
    /*在指定位置根据每行字符数、行数、行高写菜单*/
    int i;
    for ( i=0; i< row_number ; i++ )
    {
        METextOut(xpos, ypos, (char*)pzText, char_len) ;
        ypos += row_height;
        pzText += char_len;
    }
}
void CEMenuOut2(u_int xpos,u_int ypos,const u_short* pzText,u_int char_len,u_int row_number,u_int row_height)
{
    /*在指定位置根据每行字符数、行数、行高写菜单*/

    u_int sour_xpos = xpos;
    int row,col,pointer;
    u_short textvalue;
    u_short cpzText[2];
    u_char epzText[3];

    pointer = 0;
    if(char_len > 80 )char_len = 0;
    for ( row=0; row< row_number ; row++ )
    {
        for (col = 0; col < char_len ; col++ )
        {
            textvalue = *(pzText + pointer );
            Write_Space(xpos ,ypos,1,1);

            if (textvalue < 256 && textvalue >= 32)
            {
                /*为8位字符*/
                epzText[0] = textvalue;

                pointer++;		//指针指向下一个
                if(*(pzText + pointer )<256 && *(pzText + pointer ) >= 32)
                {
                    col++;
                    epzText[1] = *(pzText + pointer );
                }
                else
                {
                    epzText[1] = ' ';	//下一个为中文
                    pointer--;
                }
                METextOut(xpos, ypos, (char*)epzText, 2) ;
                xpos += 2*C_ECHAR_HDOT;

//		METextOut(xpos, ypos + C_ECHAR_VDOT -1, (char*)epzText, 1) ;
//		xpos += 1*C_ECHAR_HDOT;
            }
            else
            {
                cpzText[0] = textvalue;
                MCTextOut(xpos, ypos, (u_short *)cpzText, 1) ;
                xpos += C_CCHAR_HDOT;
            }
            pointer++;		//指针指向下一个
        }
        xpos = sour_xpos;
        ypos += row_height;
    }
}

void CEMenuOut(u_int xpos,u_int ypos,const u_short* pzText,u_int char_len,u_int row_number,u_int row_height)
{
    /*在指定位置根据每行字符数、行数、行高写菜单*/

    u_int sour_xpos = xpos;
    int row,col,pointer;
    u_short textvalue;
    u_short cpzText[2];
    u_char epzText[3];

    pointer = 0;
    if(char_len > 80 )char_len = 0;
    for ( row=0; row< row_number ; row++ )
    {
        for (col = 0; col < char_len ; col++ )
        {
            textvalue = *(pzText + pointer );
            Write_Space(xpos ,ypos,1,1);

            if (textvalue < 256 && textvalue >= 32)
            {
                /*为8位字符*/
                epzText[0] = textvalue;

                pointer++;		//指针指向下一个
                if(*(pzText + pointer )<256 && *(pzText + pointer ) >= 32)
                {
                    epzText[1] = *(pzText + pointer );
                }
                else
                {
                    epzText[1] = ' ';	//下一个为中文
                    pointer--;
                }
                METextOut(xpos, ypos , (char*)epzText, 2) ;
                xpos += 2*C_ECHAR_HDOT;

//		METextOut(xpos, ypos + C_ECHAR_VDOT -1, (char*)epzText, 1) ;
//		xpos += 1*C_ECHAR_HDOT;
            }
            else
            {
                cpzText[0] = textvalue;
                MCTextOut(xpos, ypos, (u_short *)cpzText, 1) ;
                xpos += C_CCHAR_HDOT;
            }
            pointer++;		//指针指向下一个
        }
        xpos = sour_xpos;
        ypos += row_height;
    }
}

int CETextOut(u_int xpos,u_int ypos,u_int lpos,u_int rpos,const u_short* pzText,u_int char_max,u_int row_height,u_int type)
/*最左lpos最右rpos，开始(xpos,ypos)，共写字符数char_max,每行点数，文本类型type=0英1中*/
{
    u_short textvalue;
    u_short cpzText[2];
    u_char epzText[3];
    u_int i;

//	lpos = (lpos + 7 )/8 * 8 ;
//	rpos = rpos/8 * 8;	//以字节取整

    if(lpos > xpos || rpos < xpos || char_max == 0 || type > 1 || row_height <= (1+type) * C_ECHAR_VDOT )
        return C_FALSE;

    for(i = 0; i < char_max; i++)
    {
        textvalue = *(pzText + i);

        if(xpos > rpos)
        {
            xpos = lpos;
            ypos += row_height;
        }
        Write_Space(xpos ,ypos,1,type);
        if (textvalue < 256 && textvalue >= 32)
        {
            /*为8位字符*/
            epzText[0] = textvalue;
            METextOut(xpos, ypos + ( C_ECHAR_VDOT -1 ) * type, (char*)epzText, 1) ;
            xpos += C_ECHAR_HDOT;
        }
        else if(textvalue >= 256)
        {
            cpzText[0] = textvalue;
            MCTextOut(xpos, ypos, (u_short *)cpzText, 1) ;
            xpos += C_CCHAR_HDOT;
        }
        else
        {
            xpos += C_ECHAR_HDOT * ( 1+type);
        }
    }
    while(xpos <= rpos)
    {
        Write_Space(xpos ,ypos,1,type);
        xpos += C_ECHAR_HDOT * ( 1+type );
    }
    return C_TRUE;
}

void Write_Space(u_int xpos ,u_int ypos,u_int len,u_int mode)
{
    int i,j;

    mode++;
    for(i=0; i<mode; i++)
    {
        for(j=0; j<len * mode ; j++)
        {
            Write_Ascii(xpos,ypos,' ');
            xpos +=C_ECHAR_HDOT;
        }
        xpos -= C_ECHAR_HDOT * j;
        //  ypos += C_ECHAR_VDOT;
    }
}
int CETextOut2(int xpos,int ypos,const u_short* pzText,int col_max,int row_number,int row_height,int serial,int align)
/*开始(xpos,ypos)，每行最多字符数col_max(英文),行数row_number,行高row_height,考虑编号serial=0不1有,对齐方式align＝0左1右2中间3两端*/
{
    int sour_xpos = xpos;
    u_short textvalue;
    u_short cpzText[2];
    u_char epzText[3];
    u_short *pTemp;	//临时指针
    int len_erow;//每行英文字符数，每个中文作两个英文
    int len_row;//每行字符数，每个中文字为1，每个英文词（以空格结尾）也为1
    int len_char;//每行字符数，每个中文字为1，每个英文字也为1
    int len_dots;//每行的点数
    float len_space;//每行两个字符间的空格点数
    int i,j;
    int pointer;

    MEraseWindow(xpos, ypos, xpos+C_ECHAR_HDOT*col_max,ypos+row_number*row_height-(row_height-17) ) ;	//清显示区
    pTemp = (u_short*)pzText ;
    for(i = 0  , len_char = 0 ; i < row_number; i++,ypos += row_height)
    {
        xpos = sour_xpos ;
        pTemp = (u_short*)(pTemp + len_char);
        for( j = 0, len_dots = 0, len_row = 0, len_erow = 0, len_char = 1 ; j < col_max ; j++, len_erow++, len_char++)//10H换行符
        {
            if( *(pTemp+j) < 32)break;	//遇到换行符
            if( *(pTemp+j) == 32)continue;
            if( *(pTemp+j) > 255 )
            {
                len_erow++;//中文字符长度相当于英文2
                len_row++;//中文字符加1
                len_dots += C_CCHAR_HDOT;
            }
            else
            {
                len_dots += C_ECHAR_HDOT;
                if( *(pTemp+j+1) > 255 || *(pTemp+j+1) <= 32);
                else len_row++;
            }
        }
        len_space =  8.0 * ( col_max - len_erow) /(len_row-1);//该行两个字符间的空格点数
//Write_Number(0,ypos,len_space,4,0,0);
//Write_Number(0,ypos+8,len_erow,4,0,0);
//Write_Number(0,ypos+16,len_row,4,0,0);

        switch( align )
        {
        case 3://两端
            for (pointer = 1, textvalue = *pTemp,j = 0 ; pointer <= col_max; pointer++)	//指针指向下一个
            {
                if (textvalue < 256 && textvalue > 32)
                {
                    /*为非空格8位字符*/
                    epzText[0] = textvalue;
                    METextOut(xpos, ypos + 2 , (char*)epzText, 1) ;
                    xpos += 1*C_ECHAR_HDOT;
                }
                else if (textvalue >= 256 )
                {
                    //为中文字符
                    cpzText[0] = textvalue;
                    MCTextOut(xpos, ypos, (u_short *)cpzText, 1) ;
                    xpos += C_CCHAR_HDOT;
                }
                else if( textvalue < 32 )break;//小于32为控制符，比如10h换行13h回车
                textvalue = *(pTemp + pointer );//得到字符
                if(textvalue == 32 || textvalue >= 256)
                {
                    j++;
                    xpos += len_space;
                    if( (int)(j * len_space+0.5)/j > len_space )
                    {
                        //调整两字符间距
                        xpos++;
                        j = 0;
                    }
                }

            }
            break;
        case 2://中间
        case 1://右对齐
            if( align == 1 )xpos = xpos + col_max * C_ECHAR_HDOT - len_dots;//与左对齐相比，仅开始位置后移
            else if( align == 2 )xpos = xpos + ( col_max * C_ECHAR_HDOT - len_dots)/2;
        case 0://左对齐,此为默认
        default:
            for (pointer = 0; pointer < col_max; pointer++)	//指针指向下一个
            {
                textvalue = *(pTemp + pointer );//得到字符

                if (textvalue < 256 && textvalue >= 32)
                {
                    /*为8位字符*/
                    epzText[0] = textvalue;
                    METextOut(xpos, ypos + 2 , (char*)epzText, 1) ;
                    xpos += 1*C_ECHAR_HDOT;
                }
                else if (textvalue >= 256 )
                {
                    //为中文字符
                    cpzText[0] = textvalue;
                    MCTextOut(xpos, ypos, (u_short *)cpzText, 1) ;
                    xpos += C_CCHAR_HDOT;
                }
                else break;//小于32为控制符，比如10h换行13h回车
            }
            break;
        }
    }
    return C_TRUE;
}
int Write_UNumber(u_int xpos,u_int ypos,u_int number,u_int total_len,u_int deci_len)
{
    /*在指定位置(xpos,ypos为屏幕上的点）写无符号数，total_len为可写总长度（包括点整数小数长），deci_len为小数位长度*/

    int iLoop;
    int int_len,actual_len = 1; /*总长度,实际长度 */
    u_char AscChar;
    u_int int_number,deci_number,power_number; /*整数，小数，幂 */

    do
    {
        power_number = 1;
        for(iLoop = 0; iLoop < deci_len; iLoop++)power_number *=10; /* 小数的10次方*/
        int_number = number / power_number;		/*得到整数*/
        deci_number = number % power_number;		/*得到小数*/
        if (deci_len >0 )int_len = total_len - deci_len -1;
        else int_len = total_len;					/*得到*/
        actual_len = GetNumberLen(int_number);	/*得到待写数实际的整数位数,不大于总长度*/
        if (actual_len > total_len)return C_FALSE; /*可写区长度小于数实际长度*/
        if (actual_len > int_len)
        {
            deci_len--;
            number = (number + 5 )/10;
        }
        else
        {
            int_len = actual_len;
            break;
        }
    }
    while(1);

    MEraseWindow(xpos,ypos,xpos+C_ECHAR_HDOT*total_len,ypos+C_ECHAR_VDOT);
    /*清写数字区域*/

    power_number = 1;
    for(iLoop = 1; iLoop < int_len; iLoop++)
        power_number *=10;			/* 整数位的幂，如两位数则为10，四位则为1000 */
    for(iLoop = 0; iLoop < int_len; iLoop++)
    {
        AscChar = (int_number/power_number+48);	/*将数字变为ASCII字符*/
        Write_Ascii(xpos,ypos,AscChar);
        int_number = int_number % power_number;
        power_number /= 10;
        xpos += C_ECHAR_HDOT;	/*水平位置加1字符长度=C_ECHAR_HDOT=8*/

    }

    if (deci_len > 0)
    {
        AscChar = '.';
        Write_Ascii(xpos,ypos,AscChar);
        xpos-=8;
        power_number = 1;
        for(iLoop = 1; iLoop < deci_len; iLoop++)power_number *=10;
        for(iLoop = 0; iLoop < deci_len; iLoop++)
        {
            xpos += C_ECHAR_HDOT;	/*水平位置加1字符长度=C_ECHAR_HDOT=8*/
            AscChar = (deci_number/power_number+48);	/*将数字变为ASCII字符*/
            Write_Ascii(xpos,ypos,AscChar);
            deci_number = deci_number % power_number;
            power_number /= 10;
            if (deci_number == 0)break;
        }
    }
    return C_TRUE;
}

int Write_Number(u_int xpos,u_int ypos,int number,u_int total_len,u_int deci_len,u_int sign)
{
    /*在指定位置(xpos,ypos为屏幕上的点）写有符号数，total_len为总长度（包括符号点整数小数），deci_len为小数位长度*/
    u_char AscChar = 0;

    if (sign)
    {
        if (number >= 0 )
            AscChar = '+';
        else
        {
            AscChar = '-';
            number = Mabsi(number);
        }
        xpos += C_ECHAR_HDOT;
        total_len--;
    }
    if ( Write_UNumber(xpos,ypos,number,total_len,deci_len) )
    {
        if (sign)
        {
            xpos -= C_ECHAR_HDOT;
            Write_Ascii(xpos,ypos,AscChar);
        }
        return C_TRUE;
    }
    else
    {
#ifdef C_DEBUG
        Write_Ascii(xpos,ypos,'E');
#endif
        return C_FALSE;
    }
}

void Write_Ascii(u_int xpos,u_int ypos,u_char AscChar)
{
    /*在指定位置写一个ASCII字符*/

    ypos -= VertOffsetScreen;
    char pzText[2];
    pzText[0] = AscChar;
    METextOut(xpos,ypos, pzText, 1) ;
}

void Write_Crystal(u_int xpos,u_int ypos,u_int crystal_l,u_int crystal_w)		/*写晶片尺寸*/
{
    int number;

    if (MGetUnitType())
    {
        if(crystal_w == 0)
        {
            number = crystal_l;
            Write_Number(xpos,ypos,number,5,3,0);
        }
        else
        {
            number = crystal_l;
            Write_Number(xpos-16,ypos,number,5,3,0);
            xpos += 3*C_ECHAR_HDOT;
            Write_Ascii(xpos,ypos,'X');
            number = crystal_w;
            xpos += C_ECHAR_HDOT;
            Write_Number(xpos,ypos,number,5,3,0);
        }
        EMenuOut(xpos+ 5*C_ECHAR_HDOT,ypos,"in", 2,1,20);
    }
    else
    {
        if(crystal_w == 0)
        {
            number = crystal_l;
            Write_Number(xpos,ypos,number,5,3,0);
        }
        else
        {
            number = crystal_l;
            Write_Number(xpos,ypos,number/1000,2,0,0);
            xpos += 2*C_ECHAR_HDOT;
            Write_Ascii(xpos,ypos,'X');
            number = crystal_w;
            xpos += C_ECHAR_HDOT;
            Write_Number(xpos,ypos,number/1000,2,0,0);
        }
        EMenuOut(xpos + 5*C_ECHAR_HDOT,ypos,"mm",2,1,12);
    }
}


int Input_Crystal(u_int xpos1,u_int ypos1,int *number_l,int *number_w)			//输入晶片尺寸
{
    u_int xpos = xpos1;
    u_int ypos = ypos1;

    u_int s_xPos =xpos ;		//初始水平位置
    u_int int_len ;			//代表当前输入的整数位
    u_int int_len1 =0;			//代表当前输入的整数位
    u_int deci_len1=0;	//代表当前输入的小数位
    u_int int_len2 =0;			//代表当前输入的整数位
    u_int deci_len2=0;	//代表当前输入的小数位
    u_int exitcode=0;

    u_int temp_number = 0;
    u_int temp_number1 = 0;
    u_int temp_number2 = 0;
    u_int temp_multiple=1;
    int keycode=0;
    u_char AscChar = 0;

    int i;
    int_len =13;

    do
    {
        if( keycode != -1)//==-1表示按下某一个无效键
        {
            if (int_len > 12)
            {
                xpos = s_xPos;		//初始水平位置
                MEraseWindow(xpos,ypos,xpos+C_ECHAR_HDOT*12,ypos+C_ECHAR_VDOT); //清写数字区域
                int_len = 0;
                int_len1 = 0;
                deci_len1 = 0;
                int_len2 = 0;
                deci_len2 = 0;
                temp_number = 0;
                exitcode = 0;
            }

            InvertBytes( xpos+2, ypos, 1); //反白显示1个字节的区域
        }

        keycode = MAnyKeyReturn();

        if (keycode == C_KEYCOD_CONFIRM)
        {
            if(exitcode<1)
            {
                temp_number1 = temp_number;
            }
            else
            {
                temp_number2 = temp_number;
            }

            break;	//输入确认键
        }
        if (keycode == C_KEYCOD_RETURN)
        {
            exitcode = 2;
            break;
        }
        if (keycode >=0 && keycode <= 9 )		//输入数字键
        {
            temp_number = temp_number * 10 +keycode;
            AscChar = 0x30 + keycode;		//化为ASCII码
        }
#if C_DEVTYPE == 14
        else if (keycode == C_KEYCOD_MINUS1)	//按下乘号,乘号之前一组数,乘号之后一组数
#else
        else if (keycode == C_KEYCOD_POINT)	//按下乘号,乘号之前一组数,乘号之后一组数

#endif
        {
            exitcode++;
            AscChar = 'X';
            temp_number1 = temp_number;
            temp_number = 0;
        }
        else if (keycode == C_KEYCOD_DEL)	//按下删除键
        {
            int_len = 13;	//如此赋值为了初始化
            continue;
        }
        else if (keycode ==C_KEYCOD_POINT && MGetUnitType() == 1)	//按下小数点 P_KEYCOD_MINUS1
        {
            if ((exitcode==0 && deci_len1==0)||(exitcode==1 && deci_len2==0))
            {
                AscChar = '.';
            }
            else
            {
                keycode = -1;
                AscChar = ' ';
                continue;
            }
        }
        else
        {
            keycode = -1;
            continue;
        }
        Write_Ascii(xpos,ypos,AscChar);
        if (exitcode == 1)
        {
            if (AscChar == '.')deci_len2 ++;	//输入小数位
            else if(deci_len2>0)
            {
                deci_len2 ++;
            }
            else if (AscChar != 'X')
            {
                int_len2 ++ ;
            }
            int_len=int_len2;

            if (MGetUnitType())
            {
                if (int_len2 < 2 && deci_len2 < 5) xpos += C_ECHAR_HDOT;	//已输入字数小于定义的最大长度

            }
            else
            {
                if (int_len2 < 3 && deci_len2 < 1) xpos += C_ECHAR_HDOT;	//已输入字数小于定义的最大长度
            }
        }
        else
        {
            if (AscChar == '.')deci_len1 ++;	//输入小数位
            else if(deci_len1>0)
            {
                deci_len1 ++;
            }
            else
            {
                int_len1 ++ ;
            }

            int_len=int_len1;
            if (MGetUnitType())
            {
                if (int_len1 < 2 && deci_len1 < 5) xpos += C_ECHAR_HDOT;	//已输入字数小于定义的最大长度
            }
            else
            {
                if (int_len1 < 3 && deci_len1 < 1) xpos += C_ECHAR_HDOT;	//已输入字数小于定义的最大长度
            }
        }
    }
    while(exitcode < 2);
    MEraseWindow(s_xPos,ypos,s_xPos+C_ECHAR_HDOT*13,ypos+C_ECHAR_VDOT); //清写数字区域
    if(exitcode >1)return 0;

    if (int_len1 == 0)return -1;	//无输入

    if (MGetUnitType())
    {
        if (deci_len1==0)
        {
            deci_len1++;
        }
        else if (deci_len1>4)
        {
            deci_len1=4;
            temp_number1=temp_number1/10+0.5;
        }
        if (deci_len2==0)
        {
            deci_len2++;
        }
        else if (deci_len2>4)
        {
            deci_len2=4;
            temp_number2=temp_number2/10+0.5;
        }

        temp_number1=temp_number1*pow(10,3-deci_len1+int_len1);
        temp_number2=temp_number2*pow(10,3-deci_len2+int_len2);
    }
    else
    {
        temp_number1=temp_number1*1000;
        temp_number2=temp_number2*1000;
    }
    /*	Write_Number(10,30,deci_len1,10,0,0);
    	Write_Number(10,40,int_len1,10,0,0);
    	Write_Number(10,50,temp_number1,10,0,0);
    	Write_Number(10,60,temp_number2,10,0,0);
    	MAnyKeyReturn();*/

    *number_l = temp_number1;
    *number_w = temp_number2;

    return 1;
}

int Input_Number(u_int xpos,u_int ypos,int *number,u_int int_len,int *deci_len,u_int sign )
{
    /*在指定位置(xpos,ypos为屏幕上的点）输入有符号数，int_len为整数位最大长度，
    deci_len为小数位最大长度,如为负数则可变小数长度，sign为符号0无符号1有符号*/

    int temp_deci_len = 0;	/*代表当前输入的*/
//	int temp_number;
    int temp_sign = 0;
    int total_len; /*总长度*/
    u_int s_xPos = xpos;		/*初始水平位置*/
    int keycode = 0;
    int exitcode = 0;
    u_char AscChar = 0;
    u_int int_number = 0;
    int i;

    if (deci_len !=0)
        total_len = int_len+Mabsi(*deci_len)+1;	/*小数点1位*/
    else total_len = int_len;

    keycode = *number;

    if (int_len > total_len || temp_deci_len > Mabsi(*deci_len))
    {
        xpos = s_xPos;		/*初始水平位置*/
        MEraseWindow(xpos,ypos,xpos+C_ECHAR_HDOT*(total_len+sign),ypos+C_ECHAR_VDOT); /*清写数字区域*/
        int_len =0;			/*代表当前输入的总长度，包括小数点*/
        temp_deci_len = 0;	/*代表当前输入的小数位长度*/
        temp_sign = 0;		/*代表当前输入的符号*/
        int_number = 0;		/*代表当前输入的数字*/
        exitcode = 0;
    }

    int_len = total_len + 1;	/*如此赋值为了下面的初始化*/
    do
    {
        if( keycode != -1)//==-1表示按下某一个无效键
        {
            if (int_len > total_len || temp_deci_len > Mabsi(*deci_len))
            {
                xpos = s_xPos;		/*初始水平位置*/
                MEraseWindow(xpos,ypos,xpos+C_ECHAR_HDOT*(total_len+sign),ypos+C_ECHAR_VDOT); /*清写数字区域*/
                temp_deci_len = 0;	/*代表当前输入的小数位长度*/
                temp_sign = 0;		/*代表当前输入的符号*/
                exitcode = 0;
                if( keycode > 0 && keycode <= 9)
                {
                    int_len = 1;			/*代表当前输入的总长度，包括小数点*/
                    int_number = keycode;		/*代表当前输入的数字*/
                    AscChar = 0x30 + keycode;		/*化为ASCII码*/
                    Write_Ascii(xpos,ypos,AscChar);
                    if (int_len < total_len) xpos += C_ECHAR_HDOT;	/*已输入字数小于定义的最大长度*/
                }
                else
                {
                    int_len = 0;
                    int_number = 0;		/*代表当前输入的数字*/
                }
            }

            InvertBytes( xpos, ypos, 1); /*反白显示1个字节的区域*/
        }
        keycode = MAnyKeyReturn();
        if (keycode == C_KEYCOD_CONFIRM )
        {
            if (AscChar == '.')
            {
                int_number *= 10;	/*刚按过小数点*/
                temp_deci_len++;
            }
            break;	/*输入确认键*/
        }
        if (keycode == C_KEYCOD_RETURN )
        {
            exitcode = 2;
            break;	/*输入确认键*/
        }
        if (sign > 0 && keycode == C_KEYCOD_MINUS1 && int_len == 0)		/*有符号*/
        {
            /*第一次按键输入减号 */
            AscChar = '-';		/*化为ASCII码*/
            temp_sign = 1;
        }
        else if ( keycode == 0 && int_number ==0 && int_len > sign && exitcode == 0)		/*第一个输入0*/
        {
            /*第一次已输入0，现仍输入0 ,if sign=0,int_len>0;if sign=1,int_len>1*/
            keycode = -1;
            continue;
        }
        else if (keycode >=0 && keycode <= 9)		/*输入数字键,无符号*/
        {
            int_number = int_number * 10 +keycode;
            AscChar = 0x30 + keycode;		/*化为ASCII码*/
        }
        else if (keycode == C_KEYCOD_POINT && *deci_len != 0)	/*按下小数点*/
        {
            //*deci_len != 0表示可以输入小数
            exitcode++;
            AscChar = '.';
        }
        else if (keycode == C_KEYCOD_DEL)
        {
            int_len = total_len+1;
            continue;
        }
        else
        {
            keycode = -1;
            continue;
        }

        Write_Ascii(xpos,ypos,AscChar);

        int_len ++ ;
        if (exitcode == 1 && AscChar != '.')temp_deci_len ++;	/*输入小数位*/
        if (int_len < total_len) xpos += C_ECHAR_HDOT;	/*已输入字数小于定义的最大长度*/

    }
    while(exitcode < 2);

    MEraseWindow(s_xPos,ypos,s_xPos+C_ECHAR_HDOT*total_len,ypos+C_ECHAR_VDOT); /*清写数字区域*/
    if(exitcode >1)return 0;//按了返回键

    if ( *deci_len >= 0)
    {
        /*不可变小数位长度*/
        for(i = *deci_len; i>temp_deci_len; i--) int_number *= 10;
        for(i = *deci_len; i<temp_deci_len; i++) int_number = (int_number+5)/10;
    }
    else 	*deci_len = temp_deci_len;
    *number = int_number;
    if (int_len == 0)return -1;	/*无输入*/
    else if (temp_sign>0)
    {
        *number = 0 - int_number ;
        return 2;
    }
    else return 1;
}

int Input_Float(u_int xpos,u_int ypos,float *number,u_int int_len,u_int deci_len,u_int sign)
{
    /*在指定位置(xpos,ypos为屏幕上的点）输入浮点数，int_len为整数位最大长度，
    deci_len为小数位最大长度,sign为符号0无符号1有符号*/

    u_int total_len,temp_deci_len ; /*总长度*/
    u_int s_xPos = xpos;		/*初始水平位置*/
    int keycode;
    int exitcode = 0,temp_sign;
    u_char AscChar = 0;
    u_int int_number = 0;
    float deci_number = 1.0;
    int i;

    temp_deci_len = 0;
    temp_sign = 0;

    if (deci_len !=0 )
        total_len = int_len + deci_len + 1;	/*小数点1位*/
    else total_len = int_len;

    int_len = total_len + 1;	/*如此赋值为了下面的初始化*/
    do
    {
        if (int_len > total_len || temp_deci_len > deci_len )
        {
            xpos = s_xPos;		/*初始水平位置*/
            MEraseWindow(xpos,ypos,xpos+C_ECHAR_HDOT*(total_len+sign),ypos+C_ECHAR_VDOT); /*清写数字区域*/
            int_len =0;			/*代表当前输入的总长度，包括小数点*/
            temp_deci_len = 0;	/*代表当前输入的小数位长度*/
            temp_sign = 0;		/*代表当前输入的符号*/
            int_number = 0;		/*代表当前输入的数字*/
        }

        InvertBytes( xpos, ypos, 1); /*反白显示1个字节的区域*/
        keycode = MAnyKeyReturn();
        if (keycode ==C_KEYCOD_CONFIRM ) break;	/*输入确认键*/
        if (keycode == C_KEYCOD_RETURN )
        {
            exitcode = 2;
            break;	/*输入确认键*/
        }
        if (sign > 0&&keycode==0&&int_len == 0)		/*有符号*/
        {
            /*第一次按键输入0 */
            AscChar = '-';		/*化为ASCII码*/
            temp_sign = 1;
        }
        else if (keycode >=0 && keycode <= 9)		/*输入数字键,无符号*/
        {
            int_number = int_number * 10 +keycode;
            AscChar = 0x30 + keycode;		/*化为ASCII码*/
        }

        if (keycode == C_KEYCOD_POINT)	/*按下小数点*/
        {
            exitcode++;
            AscChar = '.';
        }
        Write_Ascii(xpos,ypos,AscChar);

        int_len ++ ;
        if (exitcode == 1)temp_deci_len ++;	/*当前输入小数位*/
        if (int_len < total_len) xpos += C_ECHAR_HDOT;	/*已输入字数小于定义的最大长度*/

    }
    while(exitcode < 2);

    MEraseWindow(s_xPos,ypos,s_xPos+C_ECHAR_HDOT*(total_len+sign),ypos+C_ECHAR_VDOT); /*清写数字区域*/
    if(exitcode >1)return 0;

    if (temp_sign>0)deci_number = -1.0;
    else deci_number = 1.0;
    for( i=0; i<temp_deci_len; i++)deci_number *= 10.0;
    *number =  ( (float)int_number )/deci_number;
    if (int_len == 0)return -1;	/*无输入*/
    else if (temp_sign>0)	return 2;
    else return 1;
}

int EMessageBox( int x,int y, u_char * epText,int len,int mode )
{
    int len1 = 0, len2 = 0;
    int x1,y1,x2,y2;
    u_short *szText;
    int keycode;
    int retvalue;

    const u_short _MB_OK[] =
    {
        ENTER,CH(4023),CH(2208),
    };//(回车)确定
    const u_short _MB_CANCEL[] =
    {
        '#',CH(4001),CH(4791),
    };//（＃）取消
    const u_short _MB_OKCANCEL[] =
    {
        ENTER,CH(4023),CH(2208),' ','#',CH(4001),CH(4791),
    };//(回车)确定 （＃）取消
    const u_short _MB_YES[] =
    {
        'Y',CH(4239),
    };//(Y)是
    const u_short _MB_NO[] =
    {
        'N',CH(2381),
    };//(N)否
    const u_short _MB_YESNO[] =
    {
        'Y',CH(4239),' ','N',CH(2381),
    };//(Y)是 (N)否
    szText = (u_short *)_MB_OK;
    x1 = x2 = x+8;
    y1 = y+10;
    y2 = y + 32+4;
    len1 = len;
    switch( mode )
    {
    case MB_OK:
        szText = (u_short *)_MB_OK;
        len2 = 3;
        break;
    case MB_CANCEL:
        szText = (u_short *)_MB_CANCEL;
        len2 = 3;
        break;
    case MB_OKCANCEL:
        szText = (u_short *)_MB_OKCANCEL;
        len2 = 7;
        break;
    case MB_YES:
        szText = (u_short *)_MB_YES;
        len2 = 2;
        break;
    case MB_NO:
        szText = (u_short *)_MB_NO;
        len2 = 2;
        break;
    case MB_YESNO:
        szText = (u_short *)_MB_YESNO;
        len2 = 5;
        break;
    }
    if( len1 >= 2*len2 )//提示语较长
    {
        len = len1/2+1;
        x2 = x + ( len1 - len2)/2*C_ECHAR_HDOT;
    }
    else
    {
        len = len2;
        x1 = x + ( len2 - len1)/2*C_ECHAR_HDOT;
    }

    MPushWindow(x, y, x + (len+1)*C_CCHAR_HDOT, y + 32*2) ;
    EraseDrawRectangle(x+1, y+1, x + (len+1)*C_CCHAR_HDOT, y + 32*2) ;
    EraseDrawRectangle(x+3, y+3, x + (len+1)*C_CCHAR_HDOT-2, y + 32*2-2) ;

    EMenuOut(x1,y1+4,epText,len1,1,16);

    CETextOut2(x2,y2,szText,len2,1,16,0,0);
    while(true)
    {
        keycode = MAnyKeyReturn();
        if( (mode == MB_OK || mode == MB_OKCANCEL) && keycode == C_KEYCOD_CONFIRM)retvalue = MB_OK;
        else if( (mode == MB_CANCEL || mode == MB_OKCANCEL) && keycode == C_KEYCOD_RETURN)retvalue = MB_CANCEL;
        else if( (mode == MB_YES || mode == MB_YESNO) && keycode == C_KEYCOD_YES)retvalue = MB_YES;
        else if( (mode == MB_NO || mode == MB_YESNO) && keycode == C_KEYCOD_NO)retvalue = MB_NO;
        else continue;
        break;
    }
    MPopWindow();
    return retvalue;
}
int EMessageBox2( int x,int y, u_char * epText,int len, int row,int mode )
{
    int len1 = 0, len2 = 0;
    int x1,y1,x2,y2;
    u_short *szText;
    int keycode;
    int row_height = 32;
    int retvalue;

    const u_short _MB_OK[] =
    {
        ENTER,
    };//(回车)确定
    const u_short _MB_CANCEL[] =
    {
        '#',
    };//（＃）取消
    const u_short _MB_OKCANCEL[] =
    {
        ENTER,' ','#',
    };//(回车)确定 （＃）取消
    const u_short _MB_YES[] =
    {
        'Y',
    };//(Y)是
    const u_short _MB_NO[] =
    {
        'N',
    };//(N)否
    const u_short _MB_YESNO[] =
    {
        'Y',' ','N',
    };//(Y)是 (N)否
    szText = (u_short *)_MB_OK;
    x1 = x2 = x+8;
    y1 = y + 10 ;
    y2 = y +row_height * row + 4;
    len1 = len;
    switch( mode )
    {
    case MB_OK:
        szText = (u_short *)_MB_OK;
        len2 = 1;
        break;
    case MB_CANCEL:
        szText = (u_short *)_MB_CANCEL;
        len2 = 1;
        break;
    case MB_OKCANCEL:
        szText = (u_short *)_MB_OKCANCEL;
        len2 = 3;
        break;
    case MB_YES:
        szText = (u_short *)_MB_YES;
        len2 = 1;
        break;
    case MB_NO:
        szText = (u_short *)_MB_NO;
        len2 = 1;
        break;
    case MB_YESNO:
        szText = (u_short *)_MB_YESNO;
        len2 = 3;
        break;
    }
    if( len1 >= 2*len2 )//提示语较长
    {
        len = len1/2+1;
        x2 = x + ( len1 - len2)/2*C_ECHAR_HDOT;
    }
    else
    {
        len = len2;
        x1 = x + ( len2 - len1)/2*C_ECHAR_HDOT;
    }

    MPushWindow(x, y, x + (len+1)*C_CCHAR_HDOT, y + row_height*(row + 1) ) ;
    EraseDrawRectangle(x+1, y+1, x + (len+1)*C_CCHAR_HDOT, y + row_height*(row + 1)) ;
    EraseDrawRectangle(x+3, y+3, x + (len+1)*C_CCHAR_HDOT-2, y + row_height*(row + 1)-2) ;

    EMenuOut(x1,y1+4,epText,len1,row,row_height);

    CETextOut2(x2,y2,szText,len2,1,row_height,0,0);
    while(true)
    {
        keycode = MAnyKeyReturn();
        if( (mode == MB_OK || mode == MB_OKCANCEL) && keycode == C_KEYCOD_CONFIRM)retvalue = MB_OK;
        else if( (mode == MB_CANCEL || mode == MB_OKCANCEL) && keycode == C_KEYCOD_RETURN)retvalue = MB_CANCEL;
        else if( (mode == MB_YES || mode == MB_YESNO) && keycode == C_KEYCOD_YES)retvalue = MB_YES;
        else if( (mode == MB_NO || mode == MB_YESNO) && keycode == C_KEYCOD_NO)retvalue = MB_NO;
        else continue;
        break;
    }
    MPopWindow();
    return retvalue;
}
int CMessageBox( int x,int y, u_char * epText,int len,int mode )
{
    int len1 = 0, len2 = 0;
    int x1,y1,x2,y2;
    u_short *szText ;
    int keycode;
    int retvalue;
    int row = 1;
    int row1= 0;

    const u_short _MB_OK[] =
    {
        ENTER,CH(4023),CH(2208),
    };//(回车)确定
    const u_short _MB_CANCEL[] =
    {
        '#',CH(4543),CH(1986),
    };//（＃）退出
    const u_short _MB_OKCANCEL[] =
    {
        ENTER,CH(4023),CH(2208),' ','#',CH(4543),CH(1986),
    };//(回车)确定 （＃）退出
    const u_short _MB_YES[] =
    {
        'Y',CH(4239),
    };//(Y)是
    const u_short _MB_NO[] =
    {
        'N',CH(2381),
    };//(N)否
    const u_short _MB_YESNO[] =
    {
        'Y',CH(4239),' ','N',CH(2381),
    };//(Y)是 (N)否
    szText = (u_short *)_MB_OK;
    x1 = x2 = x;
    y1 = y+6;
    y2 = y + C_CCHAR_VDOT+8;
    len1 = len;
    switch( mode )
    {
    case MB_OK:
        szText = (u_short *)_MB_OK;
        len2 = 3;
        break;
    case MB_CANCEL:
        szText = (u_short *)_MB_CANCEL;
        len2 = 3;
        break;
    case MB_OKCANCEL:
        szText = (u_short *)_MB_OKCANCEL;
        len2 = 7;
        break;
    case MB_YES:
        szText = (u_short *)_MB_YES;
        len2 = 2;
        break;
    case MB_NO:
        szText = (u_short *)_MB_NO;
        len2 = 2;
        break;
    case MB_YESNO:
        szText = (u_short *)_MB_YESNO;
        len2 = 5;
        break;
    }

    if( len1 >= len2 )//提示语较长
    {
        len = len1;
        x2 = x + ( len1 - len2)*C_ECHAR_HDOT;
        if(MGetLanguage()==1)
        {
            row1=1;
        }

    }
    else
    {
        len = len2;
        x1 = x + ( len2 - len1)*C_ECHAR_HDOT;
        row1=0;
    }

//	MPushWindow(x, y, x + (len+1)*C_CCHAR_HDOT+1, y + 24*(row+1)) ;
//	MEraseWindow(0, y-2, len1*8,240 ) ;//y + 24*(row+1)
    EraseDrawRectangle(x+1, y+1, x + (len+2)*C_ECHAR_HDOT, y + 42*row+row1*30) ;
    EraseDrawRectangle(x+3, y+3, x + (len+2)*C_ECHAR_HDOT-2, y +42*row+row1*30-2) ;

//	CETextOut2(x+C_ECHAR_HDOT,y+1,epText,len1,row,16,0,0);
    TextOut(x+C_ECHAR_HDOT,y+5,row+MGetLanguage(),len+MGetLanguage(),C_ECHAR_VDOT,(u_char *)epText,4);

//	CETextOut2(x2+C_ECHAR_HDOT,y2,szText,len2,1,16,0,0);
    while(true)
    {
        keycode = MAnyKeyReturn();
        if( (mode == MB_OK || mode == MB_OKCANCEL) && keycode == C_KEYCOD_CONFIRM)retvalue = MB_OK;
        else if( (mode == MB_CANCEL || mode == MB_OKCANCEL) && keycode == C_KEYCOD_RETURN)retvalue = MB_CANCEL;
        else if( (mode == MB_YES || mode == MB_YESNO) && keycode == C_KEYCOD_YES)retvalue = MB_YES;
        else if( (mode == MB_NO || mode == MB_YESNO) && keycode == C_KEYCOD_NO)retvalue = MB_NO;
        else continue;
        break;
    }
    //	MPopWindow();
    return retvalue;
}




/* 显示提示信息 */
extern const char _Message_A1[C_LANGUAGE][31][22] ;

extern const u_char _Message_B1[C_LANGUAGE][15][24];
extern const u_short _Message_B2[] ;
extern const unsigned char _usbdisk[];
extern const unsigned char _usberror[];

void DisplayPrompt(u_int num)
{
    u_short xpos,ypos;
    u_char char_len,row_number,row_height;	/*字符数和字符行数*/

    xpos = 0 ;
    ypos = 0 ;
    char_len = 6;
    row_number = 1;
    row_height = C_CCHAR_VDOT;
//;	if( GetElapsedTime() < 1000 * 60)//开机1分钟内
    {
//;		MSetColor(C_CR_DEFAULT);
    }
//;	else 	MSetColor(C_CR_MENU);
    if(num == 15)
    {
        MEraseWindow(0,ypos,C_CCHAR_HDOT*16,C_CCHAR_VDOT+ypos);
//		CMenuOut(xpos, ypos, _Message_A1 + num * char_len, char_len, row_number, row_height);
//		CMenuOut(xpos+5*C_CCHAR_HDOT, ypos, _Message_A1 + num * char_len, char_len, row_number, row_height);
        TextOut(xpos, ypos,row_number,22,row_height,(char *)_Message_A1[MGetLanguage()][num],4);

    }
    else if(num == 16 || num == 17)
    {
        if (MGetSaveMode()==1 && MGetSaveStatus() == 1 && num == 16)
        {
            num=22;    //DisplayPrompt(22);	//连续记录中
        }
        MEraseWindow(0,ypos,C_CCHAR_HDOT*16,C_CCHAR_VDOT+ypos);
        TextOut(xpos, ypos,row_number,22,row_height,(char *)_Message_A1[MGetLanguage()][num],4);
//		CMenuOut(xpos, ypos, _Message_A1 + num * char_len, char_len, row_number, row_height);
    }
    else if(num == 18)
    {
        char_len = 6;
        row_number = 1;
        row_height = C_CCHAR_VDOT;
        MEraseWindow(0,ypos,C_CCHAR_HDOT*16,C_CCHAR_VDOT+ypos);
        TextOut(xpos, ypos,row_number,22,row_height,(char *)_Message_A1[MGetLanguage()][num],4);
    }
    else if(num == 29)
    {
        char_len = 6;
        row_number = 1;
        row_height = 32;
        MEraseWindow(0,0,C_CCHAR_HDOT*16,C_CCHAR_VDOT);
        TextOut(xpos, ypos,row_number,22,row_height,(char *)_Message_A1[MGetLanguage()][num],4);
        //	CMenuOut(xpos, ypos, _Message_A1 + num * char_len, char_len, row_number, row_height);
    }
    else
    {
        char_len = 6;
        row_number = 1;
        row_height = 32;
        MPushWindow(xpos, ypos, 16 * C_CCHAR_HDOT, row_height+ypos) ;
        MEraseWindow(0,0,C_CCHAR_HDOT*16,C_CCHAR_VDOT);
        TextOut(xpos, ypos,row_number,22,row_height,(char *)_Message_A1[MGetLanguage()][num],4);
        //	CMenuOut(xpos, ypos, _Message_A1 + num * char_len, char_len, row_number, row_height);
        ExpendTime(100);	/* 消耗时间，以10ms为单位 */
        MPopWindow();
    }

    if (MGetAcquisitionEnable())
    {
        if (SDCardGetStatus()==1 && MGetSaveMode())	//显示U盘图标
        {
            PageDisp(504,450,(u_char*)_usbdisk);
        }
        else if (SDCardGetStatus()==0 && MGetSaveMode())
        {
            PageDisp(504,450,(u_char*)_usberror);
        }
        else
        {
            MEraseWindow(504,450,504+18,450+18);
        }

    }
}

int DisplayQuery(int index)
{
    u_int xpos,ypos;
    u_int char_len,row_number,row_height;	/*字符数和字符行数*/
    int keycode;
    int retvalue;

    xpos = 0 ;
    ypos = 0 ;
    char_len = 6;
    row_number = 1;
    row_height = C_CCHAR_VDOT;

    MPushWindow(xpos, ypos, 14 *C_CCHAR_HDOT, row_height+24) ;
    MEraseWindow(xpos,ypos,C_CCHAR_HDOT*13,C_CCHAR_VDOT+4);
//	CMenuOut(xpos, ypos, _Message_B1 + index * char_len, char_len, row_number, row_height);
    TextOut(xpos, ypos, 1, 23,row_height, (u_char*)_Message_B1[MGetLanguage()][index], 4);
    /*	if (MGetLanguage())
    	{
    		EMenuOut(xpos+23*C_ECHAR_HDOT, ypos+1+4,"y/n",3,1,16);
    	}
    	else
    	{
    		CMenuOut(xpos+23*C_ECHAR_HDOT, ypos, _Message_B2, 4, row_number, row_height);
    	}*/
    while(1)
    {
        keycode = MAnyKeyReturn();
        if(keycode == C_KEYCOD_YES)
        {
            retvalue = 1;
            break;
        }
        else if(keycode == C_KEYCOD_NO)
        {
            retvalue = 0;
            break;
        }
    }
    ExpendTime(90);	/* 消耗时间，以10ms为单位 */
    MPopWindow() ;
    return retvalue;
}

void WriteFileDate(u_int xpos, u_int ypos, u_int date)
{
    int year,month;
    year = date/65536 ;
    date = date%65536;
    month = date/256 ;
    date = date%256;

    if(year < 1000)year += 1000;
    Write_Number(xpos , ypos,year,4,0,0);

    Write_Number(xpos +C_ECHAR_HDOT * 4, ypos,month + 100,3,0,0) ;
    Write_Ascii(xpos +C_ECHAR_HDOT * 4, ypos,'-');

    Write_Number(xpos +C_ECHAR_HDOT * 7, ypos,date + 100,3,0,0) ;
    Write_Ascii(xpos +C_ECHAR_HDOT * 7, ypos,'-');
    //	TIME_CLOCK tm;

//	int2date( &tm,date);

//	TimeClock.date = Hex2Bcd(date%256);
//	date = date/256;
//	TimeClock.month = Hex2Bcd(date%256);
//	date = date/256;
//	TimeClock.yearl = Hex2Bcd(date%100);
//	TimeClock.yearh = Hex2Bcd(date/100);

//	WriteDate(xpos, ypos, &tm);
}

void WriteFileDate1(u_int xpos, u_int ypos, u_int date)
{
    int year,month;
    /*
    	year = Hex2Bcd( ( date/65536)%100 );
    	date = date%65536;
    	month = Hex2Bcd( date/256 );
    	date = Hex2Bcd(date%256);
    	date = (year * 100 + month)*100+date;
    /*/
    year = ( date/65536)%100 ;
    date = date%65536;
    month = date/256 ;
    date = date%256;
    date = (year * 100 + month)*100+date;
//*/
    if( date < 100000 )
    {
        Write_Ascii(xpos,ypos,'0');
        Write_Number(xpos + C_ECHAR_HDOT, ypos,date,5,0,0);
    }
    else Write_Number(xpos, ypos,date,6,0,0);
}

void WriteDate(u_int xpos, u_int ypos, TIME_CLOCK *tm)
{
//*
    u_int year = Bcd2Hex( tm->yearh ) * 100 + Bcd2Hex(tm->yearl) ;
    u_int month = Bcd2Hex( (tm->month) & 0x1f );
    u_int date = Bcd2Hex( (tm->date) & 0x3f );	// 日期  BCD码转换成十六进制
    /*/
    	u_int year =  tm->yearh  * 100 + tm->yearl ;
        u_int month =tm->month & 0x1f ;
    	u_int date = tm->date & 0x3f ;	// 日期  BCD码转换成十六进制
    //*/
    /* 2003-05-02 */
    if(year < 1000)year += 1000;
    Write_Number(xpos , ypos,year,4,0,0);

    Write_Number(xpos + C_ECHAR_HDOT * 4, ypos,month + 100,3,0,0) ;
    Write_Ascii(xpos + C_ECHAR_HDOT * 4, ypos,'-');

    Write_Number(xpos + C_ECHAR_HDOT * 7, ypos,date + 100,3,0,0) ;
    Write_Ascii(xpos + C_ECHAR_HDOT * 7, ypos,'-');
}

void WriteWeek(u_int xpos, u_int ypos, TIME_CLOCK *tm)
{
    u_int day = Bcd2Hex( (tm->day) & 0x0f );	// 日期  BCD码转换成十六进制
    Write_Number(xpos , ypos,day,2,0,0);
}
void WriteTime(u_int xpos, u_int ypos, TIME_CLOCK *tm)
{
//*
    u_int hour = Bcd2Hex( tm->hour & 0x3f ) % 24;// 提取有效位 并转换为十六进制
    u_int minute = Bcd2Hex( tm->minute & 0x7f ) % 60;
    u_int second = Bcd2Hex( tm->second & 0x7f ) % 60;
    /*/
    	u_int hour =  (tm->hour & 0x3f ) % 24;// 提取有效位 并转换为十六进制
    	u_int minute =  (tm->minute & 0x7f ) % 60;
    //*/
    if(hour < 10 )
    {
        Write_Space(xpos,ypos,1,0);
        Write_Number(xpos + C_ECHAR_HDOT, ypos,hour * 10 ,3,1,0);
    }
    else
        Write_Number(xpos, ypos,hour * 10 ,4,1,0);
    Write_Number(xpos + C_ECHAR_HDOT * 2, ypos,minute + 100,3,0,0);
    Write_Ascii(xpos + C_ECHAR_HDOT * 2, ypos,':');
   // Write_Number(xpos + C_ECHAR_HDOT * 5, ypos,second+100,3,0,0);
   // Write_Ascii(xpos + C_ECHAR_HDOT * 5, ypos,':');
    /*8:2*/
}
void WriteTime1(u_int xpos, u_int ypos, u_int tm)
{
//*
    u_int hour = ( (tm >> 16) & 0x3f) % 24;// 提取有效位 并转换为十六进制
    u_int minute = ( (tm >> 8 ) & 0x7f ) % 60;
    /*/
    	u_int hour =  (tm->hour & 0x3f ) % 24;// 提取有效位 并转换为十六进制
    	u_int minute =  (tm->minute & 0x7f ) % 60;
    //*/
    if(hour < 10 )
        Write_Number(xpos + C_ECHAR_HDOT, ypos,hour * 10 ,3,1,0);
    else
        Write_Number(xpos, ypos,hour * 10 ,4,1,0);
    Write_Number(xpos + C_ECHAR_HDOT * 2, ypos,minute + 100,3,0,0);
    Write_Ascii(xpos + C_ECHAR_HDOT * 2, ypos,':');
    /*8:2*/
}

u_int InputDate(u_int xpos, u_int ypos, TIME_CLOCK *tm)
{
    u_char input[10] = {0};
    u_int year,month,date;

    int i = 0;/* 光标位置 */

    u_int keycode = C_KEYCODMAX ;		/* 附值为不用的键值 */

    year = Bcd2Hex( tm->yearh ) * 100 + Bcd2Hex(tm->yearl) ;
    month = Bcd2Hex(tm->month & 0x1f );
    date = Bcd2Hex(tm->date & 0x3f );	/* 日期  BCD码转换成十六进制 */

    InvertBytes(xpos + i * C_ECHAR_HDOT, ypos,1) ;
    //InvertBytes(xpos, ypos, 1) ;
    /* 提示输入日期 */
    while(1)
    {
        keycode = MAnyKeyReturn() ;	/* 读输入的键值 */

        switch (keycode)
        {
        case	C_KEYCOD_CONFIRM  :

            ( tm->yearh ) = ( ( (tm->yearh) & (0x0 ) ) + Hex2Bcd(year/100) );
            ( tm->yearl ) = ( ( (tm->yearl) & (0x0 ) ) + Hex2Bcd(year%100)  );
            ( tm->month ) = ( ( (tm->month) & (0xe0) ) + Hex2Bcd(month) );
            ( tm->date  ) = ( ( (tm->date ) & (0xc0) ) + Hex2Bcd(date) );

            Write_Space( xpos, ypos,10,0) ;
            return C_TRUE ;	/* 确认输入的内容 */
        case	C_KEYCOD_RETURN	 :
            Write_Space( xpos, ypos,10,0) ;
            return C_FALSE ;	/* 放弃输入 */
        case	C_KEYCOD_DEL   :/* 2003-10-20，共10位 */
            if( i <= 5)
            {
                i = 0;
                Write_Space( xpos, ypos,10,0) ;
            }
            else if( i <= 8 )
            {
                i = 5;
                Write_Space( xpos+i*C_ECHAR_HDOT, ypos,5,0) ;
            }
            else
            {
                i = 8;
                Write_Space( xpos+i*C_ECHAR_HDOT, ypos,2,0) ;
            }
            InvertBytes(xpos + i * C_ECHAR_HDOT, ypos,1) ;

            break;
        case	C_KEYCOD_MINUS   :/* 2003-10-20，共10位 */
            if(i > 0)	/* i==1的时候可以 */
            {
                if(i == 5 || i == 8)
                {
                    Write_Space( xpos, ypos,10,0) ;
                    i-- ;
                    i-- ;
                    InvertBytes(xpos + i * C_ECHAR_HDOT, ypos,1) ;
                }
                else
                {
                    i-- ;
                    InvertBytes(xpos + i * C_ECHAR_HDOT, ypos,1) ;
                }
            }
            break ;				/* 光标移动 */
        default	:
            if(keycode >= 0 && keycode <= 9)
            {

                Write_Number(xpos + i * C_ECHAR_HDOT, ypos,keycode,1,0,0);
                input[i] = keycode ;

                if(i < 9)
                {
                    if(i == 3)
                    {
                        year = input[0] * 1000 + input[1] * 100 + input[2] * 10 + input[3];
                        i++ ;
                        Write_Ascii( xpos + i * C_ECHAR_HDOT, ypos,'-') ;
                    }
                    else if(i == 6)
                    {
                        month = input[5] * 10 + input[6];
                        //检查年、月、日是否符合
                        //检查年、月、日是否符合规则，返回值0符合，非0不符合，1：year,2:month,4:date
                        //1+2:year+moth,1+4:year+date,2+4:month+date,1+2+4:year+month+date
                        if( ( CheckDate(year,month,date) >> 1 )%2 == 0)
                        {
                            i++ ;
                            Write_Ascii( xpos + i * C_ECHAR_HDOT, ypos,'-') ;
                        }
                        else
                        {
                            i--;
                            Write_Space( xpos + i * C_ECHAR_HDOT, ypos,2,0) ;
                            i--;
                        }
                    }
                    i++ ;
                }
                else if( i == 9)
                {
                    date = input[8] * 10 + input[9];
                    if( ( CheckDate(year,month,date) >> 2 )%2 != 0 )
                    {
                        i--;
                        Write_Space( xpos + i * C_ECHAR_HDOT, ypos,2,0) ;
                    }
                }
                InvertBytes(xpos + i * C_ECHAR_HDOT, ypos,1) ;
            }
            break ;
        }
    }
}

u_int InputTime(u_int xpos, u_int ypos, TIME_CLOCK *tm)
{
    TIME_CLOCK tmp;
    u_int hour,minute;
    u_char input[5] = {0};

    hour = Bcd2Hex( tm->hour & 0x3f ) % 24;// 提取有效位 并转换为十六进制
    minute = Bcd2Hex( tm->minute & 0x7f ) % 60;
    input[0]=hour/10;
    input[1]=hour%10;
    input[3]=minute/10;
    input[4]=minute%10;
    int i = 0;/* 光标位置 */

    u_int keycode = C_KEYCODMAX ;		/* 附值为不用的键值 */

    InvertBytes(xpos, ypos,1) ;
    /* 提示输入时间 */
    while(1)
    {
        keycode = MAnyKeyReturn();	/* 读输入的键值 */

        switch (keycode)
        {
        case	C_KEYCOD_CONFIRM :
            if(i==1)
            {
                input[1]=input[0];
                input[0]=0;
            }
            tmp.hour = input[0] * 10 + input[1] ;
            tmp.minute = input[3] * 10 + input[4] ;

            tm->hour = Hex2Bcd(tmp.hour) ;
            tm->minute = Hex2Bcd(tmp.minute) ;

            Write_Space( xpos, ypos,5,0) ;
            return C_TRUE ;	/* 确认输入的内容 */
        case	C_KEYCOD_RETURN	 :
            Write_Space( xpos, ypos,10,0) ;
            return C_FALSE ;	/* 放弃输入 */
        case	C_KEYCOD_DEL   :/* 2003-10-20，共10位 */
            if( i <= 3)
            {
                i = 0;
                Write_Space( xpos, ypos,10,0) ;
            }
            else if( i <= 5 )
            {
                i = 3;
                Write_Space( xpos+i*C_ECHAR_HDOT, ypos,5,0) ;
            }
            InvertBytes(xpos + i * C_ECHAR_HDOT, ypos,1) ;

            break;
        case	C_KEYCOD_MINUS   :/* 20:10，共5位 */
            if(i > 0)	/* i==1的时候可以 */
            {
                if(i == 3)
                {
                    InvertBytes(xpos + i * C_ECHAR_HDOT, ypos, 1) ;
                    i-- ;
                    i-- ;
                    InvertBytes(xpos + i * C_ECHAR_HDOT, ypos, 1) ;

                }
                else
                {
                    InvertBytes(xpos + i * C_ECHAR_HDOT, ypos, 1) ;
                    i-- ;
                    InvertBytes(xpos + i * C_ECHAR_HDOT, ypos, 1) ;

                }
            }
            break ;				/* 光标移动 */
        default	:
            if(keycode >= 0 && keycode <= 9)
            {

                Write_Number(xpos + i * C_ECHAR_HDOT, ypos,keycode,1,0,0);
                input[i] = keycode ;

                if(i < 4)
                {
                    i++ ;
                    if(i == 2)
                    {
                        hour = input[0] * 10 + input[1];
                        if( hour > 23)
                        {
                            i = 0;
                            Write_Space( xpos + i * C_ECHAR_HDOT, ypos,5-i,0) ;
                        }
                        else
                        {
                            Write_Ascii(xpos + i * C_ECHAR_HDOT, ypos,':');
                            i++ ;
                        }
                    }
                }
                else if(i==4)
                {
                    minute = input[3] * 10 + input[4];
                    if(minute >= 60)
                    {
                        i = 3;
                        Write_Space( xpos + i * C_ECHAR_HDOT, ypos,5-i,0) ;
                    }
                }
                InvertBytes(xpos + i * C_ECHAR_HDOT, ypos, 1) ;
            }
            break ;
        }
    }
}

u_int InputWeek(u_int xpos, u_int ypos, TIME_CLOCK *tm)
{
    TIME_CLOCK tmp;
    u_char day;

    day = Bcd2Hex( (tm->day) & 0x0f );	// 日期  BCD码转换成十六进制

    int i = 0;/* 光标位置 */

    u_int keycode = C_KEYCODMAX ;		/* 附值为不用的键值 */

    InvertBytes(xpos, ypos,1) ;
    /* 提示输入时间 */
    while(1)
    {
        keycode = MGetAnyKey(6) ;	/* 读输入的键值 */

        switch (keycode)
        {
        case	C_KEYCOD_CONFIRM :
            tmp.day = day;

            tm->day =Hex2Bcd(tmp.day); // (tm->day & (0x0 )) | Hex2Bcd(tmp.day) ;

            Write_Space( xpos, ypos,5,0) ;
            return C_TRUE ;	/* 确认输入的内容 */
        case	C_KEYCOD_RETURN	 :
            Write_Space( xpos, ypos,10,0) ;
            return C_FALSE ;	/* 放弃输入 */
        default	:
            if(keycode >= 0 && keycode <= 9)
            {

                Write_Number(xpos, ypos,keycode,1,0,0);
                day = keycode ;
                if( day > 8)
                {
                    i = 0;
                    Write_Space( xpos + i * C_ECHAR_HDOT, ypos,5,0) ;
                }
                InvertBytes(xpos + i * C_ECHAR_HDOT, ypos, 1) ;
            }
            break ;
        }
    }
}
u_int date2int(TIME_CLOCK *tm)
{
    return ( ( ( (Bcd2Hex( tm->yearh & 0x3f) ) * 100 + Bcd2Hex(tm->yearl)) * 256 + (Bcd2Hex(tm->month & 0x1f)) ) *256 + (Bcd2Hex(tm->date &0x3f)) );
}
u_int date2int2(TIME_CLOCK *tm)
{
    unsigned int date =  ( ( ( (Bcd2Hex(tm->yearh & 0x3f) ) * 100 + Bcd2Hex(tm->yearl)) * 256 + (Bcd2Hex(tm->month & 0x1f)) ) *256 + (Bcd2Hex(tm->date &0x3f)) );
    unsigned int year = (Bcd2Hex(tm->yearh & 0x3f) ) * 100 + Bcd2Hex(tm->yearl);
    return ( MGetYearDateCount(2000,year) + MGetCurrentYearTimeCount( date, 0,2) );
}
void int2date(TIME_CLOCK *tm,u_int date)
{
    int year,month;

    year = ( date/65536)%10000 ;
    date = date%65536;
    month = date/256 ;
    date = date%256;

    ( tm->yearh ) = ( ( (tm->yearh) & (0x0 ) ) + Hex2Bcd(year/100) );
    ( tm->yearl ) = ( ( (tm->yearl) & (0x0 ) ) + Hex2Bcd(year%100)  );
    ( tm->month ) = ( ( (tm->month) & (0xe0) ) + Hex2Bcd(month) );
    ( tm->date  ) = ( ( (tm->date ) & (0xc0) ) + Hex2Bcd(date) );
}
void int2date2(TIME_CLOCK *tm,u_int date)//从到2000－1－1的天数－》tm
{
}


int qw2hf(int qw)
{
    switch(qw)
    {
    case (3647):
        return 1;	;//南
    case (4508):
        return 2;	;//通
    }
    return 0;
}

void WriteCharDots24(const u_short* type, int xpos, int ypos)
//写一个32*32的字符，type=区位码，xpos,ypos=坐标
{
    int i,j,k,x,y;
    xpos += HoriOffsetScreen;
    ypos += VertOffsetScreen;
    if(ypos >= C_VERTDOT_VIDEO)ypos = C_VERTDOT_VIDEO-1;
    if( xpos >= C_HORIDOT_VIDEO) xpos = C_HORIDOT_VIDEO-1;

    int numbertype=*type;
    int number=numbertype;

    u_char *char_dots;
    //  GrColor yellow = GrAllocColor(255,255,0);
    //处理区位码
    //区位码从101开始，每过一百会差6个值。比如从194直接到201，预留了6个数值空间。
    if(numbertype<101)number=0;
    else
    {
        number=numbertype-101;
        if(numbertype>200)
            number-=(numbertype/100-1)*6;

    }
    //MEraseWindow(xpos,ypos,xpos+C_CCHAR_HDOT,ypos+C_CCHAR_VDOT);

    x = xpos;
    y = ypos;
    //起始位置
    char_dots = (u_char *)Dots_simkai24 + (number%20) * 24 +(number/20)*480*3; //Dots_stsong24
    //取出点阵
    for(k=0; k<3; k++)
    {
        for( i = 0 ; i < 24; i++)
        {
            for( j = 7; j >= 0; j--)
            {
                if( ((char_dots[i] >> j)&0x01) > 0)DrawPixel(x,y+7-j);
            }
            x++;
        }
        x = xpos;
        y+=8;
        char_dots+=480;
    }

}
void WriteCharDots32(const u_short* type, int xpos, int ypos)
//写一个32*32的字符，type=区位码，xpos,ypos=坐标
{
    int i,j,k,x,y;
    xpos += HoriOffsetScreen;
    ypos += VertOffsetScreen;
    if(ypos >= C_VERTDOT_VIDEO)ypos = C_VERTDOT_VIDEO-1;
    if( xpos >= C_HORIDOT_VIDEO) xpos = C_HORIDOT_VIDEO-1;

    int numbertype=*type;
    int number=numbertype;

    u_char *char_dots;
    //  GrColor yellow = GrAllocColor(255,255,0);
    //处理区位码
    //区位码从101开始，每过一百会差6个值。比如从194直接到201，预留了6个数值空间。
    if(numbertype<101)number=0;
    else
    {
        number=numbertype-101;
        if(numbertype>200)
            number-=(numbertype/100-1)*6;

    }
    MEraseWindow(xpos,ypos,xpos+C_CCHAR_HDOT,ypos+C_CCHAR_VDOT);

    x = xpos;
    y = ypos;
    //起始位置
    //  char_dots = (u_char *)Dots_msyh32 + (number%20) * 32 +(number/20)*640*4;
    //取出点阵
    for(k=0; k<4; k++)
    {
        for( i = 0 ; i < 32; i++)
        {
            for( j = 7; j >= 0; j--)
            {
                if( ((char_dots[i] >> j)&0x01) > 0)DrawPixel(x,y+7-j);
            }
            x++;
        }
        x = xpos;
        y+=8;
        char_dots+=640;
    }

}

void WriteCharDots16(const u_char* type, int xpos, int ypos)
//写一个24*24的字符，type=区位码，xpos,ypos=坐标, mode =16/24字体大小
{
    int i,j,k,x,y;

    xpos += HoriOffsetScreen;
    ypos += VertOffsetScreen;
    if(ypos >= C_VERTDOT_VIDEO)ypos = C_VERTDOT_VIDEO-1;
    if( xpos >= C_HORIDOT_VIDEO) xpos = C_HORIDOT_VIDEO-1;
    u_char numbertype=*type;
    u_char *char_dots;
    int number,number1;
    //  GrColor yellow = GrAllocColor(255,255,0);
    //处理区位码
    //区位码从101开始，每过一百会差6个值。比如从194直接到201，预留了6个数值空间。
    number1=numbertype;
    /*   if(numbertype>=32 && numbertype<127)
       {
           number1=numbertype+268;
       }

       number=number1-101;
       if(number1>200)
           number-=(number1/100-1)*6;
    */

    number=number1-33;
    if(number<=0)number=220;
    MEraseWindow(xpos,ypos,xpos+C_ECHAR_HDOT,ypos+C_ECHAR_VDOT);

    x = xpos;
    y = ypos;
    //起始位置
    char_dots = (u_char *)Dots_times24 + (number%20) * 24 +(number/20)*480*3;
    //取出点阵
    for(k=0; k<3; k++)
    {
        for( i = 0 ; i < 16; i++)
        {
            for( j = 7; j >= 0; j--)
            {
                if( ((char_dots[i] >> j)&0x01) > 0)DrawPixel(x,y+7-j);
            }
            x++;
        }
        x = xpos;
        y+=8;
        char_dots+=480;
    }

}
int TextOut(int x,int y,int row,int len,int height, u_char* szText, int dots)
{

    float len_space;//每行两个字符间的空格点数
    int sx = x;//保存初始水平位置
    u_short textvalue;
    u_short total_len;
    u_short total_row;
    u_short cpzText[2];
    u_char epzText[2];
    u_char *pTemp;	//临时指针
    short i,j;
    u_short len_dots;//每行实际字符占用的点数
    u_char align;//每行的对齐方式
    u_char rh;

    if( dots > 200)rh = 24;
    else rh = 0;
    dots=0;
    ///MEraseWindow(x, y, x+C_ECHAR_HDOT*len,y+row*height) ;	//清显示区
    pTemp = szText ;
    ///if( row > 1 && height > 16)y = y + (height-16)/2;//超过一行的,下移4点


    textvalue = TXAC;
    if( row <= 1)
    {
        if( row < 1 )
        {
            len = GetStrLen(szText);
            row = 1;
        }
        for( i = 0; textvalue != TXEOF && i < len; )
        {
            textvalue = szText[i];
            if( textvalue  <= TXAC && textvalue >= TXAL && textvalue < TXSP)//开始的对齐方式
            {
                i++;//指向下一个字符
            }
            else break;
        }
    }
    else i = 0;
    total_len = row * len;//最多字符数
    total_row = row;
    row = 0;

    for( ; textvalue != TXEOF && i < total_len && row < total_row; )
    {
        //开始显示字符,当有结束符,或者已经超过显示区域可显示的最多字符数,退出
        textvalue = szText[i];
        if( textvalue  <= TXAC && textvalue >= TXAL )//对齐方式
        {
            i++;//指向下一个字符
            align = textvalue;
            total_len++;//存储区中,有一个控制字符
        }
        else align = TXAL;
        for( j = i,len_dots = 0,x = 0; j - i < len ; j++)
        {
            textvalue = szText[j];
            if( textvalue == TXCR || textvalue == TXEOF )break;//看本行有多少字符
            if( textvalue >= HZCDEF )
            {
                len_dots += C_CCHAR_HDOT;
                j++;
                x++;//用X代表有多少个字符
            }
            else if( textvalue >= TXSP )
            {
                len_dots += C_ECHAR_HDOT;
                x++;
            }
        }
        if( x > 1)len_space =  (float)(C_ECHAR_HDOT * len - len_dots) /(x-1.0);//该行两个字符间的空格点数
        else len_space =  (float)(C_ECHAR_HDOT * len - len_dots) ;//
        if( height <= rh)MEraseWindow(sx, y, sx+C_ECHAR_HDOT*len,y+height) ;	//清当前行
        else MEraseWindow(sx, y, sx+C_ECHAR_HDOT*len,y+rh) ;	//清当前行
        if( align == TXAL)
        {
            //左对齐
            x = sx;
            len_space = 0;
        }
        else if( align == TXAR)
        {
            //右对齐
            x = sx + (C_ECHAR_HDOT* len - len_dots);
            len_space = 0;
        }
        else if( align == TXALR)
        {
            //左右对齐
            x = sx;
        }
        else if( align == TXAC)
        {
            //居中对齐
            x = sx + (C_ECHAR_HDOT* len - len_dots)/2;
            len_space = 0;
        }
        for( j = 0; ; j++)
        {
            textvalue = szText[i + j];
            if( j >= len )
            {
                //已显示了一行的最多字符数
                i = i + j ;
                if( szText[i + j]  == TXCR )
                {
                    i++;//忽略掉回车
                    total_len++;//存储区中,有一个控制字符
                }
                y += height;
                row++;//行数增加
                break;
            }
            else if( textvalue == TXCR || textvalue == TXEOF)
            {
                i = i + j + 1;
                total_len++;//存储区中,有一个控制字符
                y += height;
                row++;//行数增加
                break;
            }


            if( textvalue >= HZCODE )//正常中文字库24*24
            {
                j++;
                cpzText[0] = (textvalue - 160) * 100 + ( szText[i + j] - 160);

                WriteCharDots24((u_short *)cpzText,x, y) ;
                x += (C_CCHAR_VDOT + len_space);
            }
            else if( textvalue >= HZCDEF)// ;//自建16*16字库
            {
                j++;
                textvalue = (textvalue - HZCDEF)*(HZCODE - HZCDEF ) + ( szText[i + j] - HZCDEF);
                //	pTemp = (u_char*)CHAR_DOT16M16+ textvalue * 32 ;
                MUserDotsOut(x, y , (u_char*)pTemp,8) ;
                //	pTemp = (u_char*)CHAR_DOT16M16+ textvalue * 32+8 ;
                MUserDotsOut(x, y +8, (u_char*)(pTemp),8) ;
                //	pTemp = (u_char*)CHAR_DOT16M16+ textvalue * 32+16;
                MUserDotsOut(x+8, y , (u_char*)pTemp,8) ;
                //	pTemp = (u_char*)CHAR_DOT16M16 + textvalue * 32+24 ;
                MUserDotsOut(x+8, y +8, (u_char*)(pTemp),8) ;
                x += (C_CCHAR_VDOT + len_space);
            }
            else if( textvalue >= HZCEDEF)//140自建16*8文字库
            {
                j++;
                textvalue = (textvalue - HZCEDEF)*(HZCDEF - HZCEDEF )+( szText[i + j] - HZCEDEF);
                //	pTemp = (u_char*)CHAR_DOT16M8+ textvalue * 16 ;
                MUserDotsOut(x,y,(u_char*)pTemp,8);
                //	pTemp = (u_char*)CHAR_DOT16M8 + (textvalue * 16)+8  ;
                MUserDotsOut(x,y+C_ECHAR_VDOT,(u_char*)pTemp,8);
                x += (C_ECHAR_VDOT + len_space);
            }
            else if( textvalue >= HZECODE)//正常16*16字库
            {
                epzText[0] = textvalue;
                // WriteCharDots24((u_char *)epzText,x, y+ dots*2) ;
                WriteCharDots16((u_char *)epzText,x, y+ dots*2) ;
                //epzText[0] = textvalue;
                //METextOut(x, y + dots , (u_char*)epzText, 1) ;
                x += (C_ECHAR_HDOT + len_space);
            }
            else if( textvalue >= HZEDEF && textvalue < TXSP)//自建8*8字库,< 32
            {
                //		pTemp = (u_char*)CHAR_DOT8M8 +  (textvalue - HZEDEF) * 8 ;
                MUserDotsOut(x,y+dots,(u_char*)pTemp,8);
                x += (C_ECHAR_VDOT + len_space);
            }
        }
    }
    return C_TRUE;
}

/*
mode == 0, 显示一个0-9的8*8字符,0<= code <= 9代表实际的数字
mode == 1, 显示一个8*8字符,32<= code < 128 代表一个可显示的实际字符的ASCII码
mode == 2, 显示一个左下角不带点的16*16字符,0<= code <= 9代表实际的数字
mode == 3, 显示一个左下角带点的16*16字符,0<= code <= 9代表实际的数字
mode == 4, 显示一个16*16字符,code代表字符的的区位码
//*/
void WordOut(int x,int y,int code,int mode)
{
    char buff[4];
    if( mode == 0)
    {
        Write_Number(x,y,code,1,0,0);
    }
    else if( mode == 1)
    {
        buff[0] = code;
        buff[1] = TXEOF;
        TextOut( x,y,1,1,8,buff,0);
    }
    else if( mode == 2)
    {
        buff[0] = GetHZcode(code+315,HZCODE)/256;
        buff[1] = GetHZcode(code+315,HZCODE)%256;
        buff[2] = TXEOF;		//316为0的区位码
        TextOut(x,y,1,2,16,buff,0);
    }
    else if( mode == 3)
    {
        buff[0] = GetHZcode(code+216,HZCODE)/256;
        buff[1] = GetHZcode(code+216,HZCODE)%256;
        buff[2] = TXEOF;		//316为0的区位码
        TextOut(x,y,1,2,16,buff,0);
    }
}
int GetStrLen( u_char * szText)//得到字符串的长度
{
    int len;
    for( len = 0; ; len++)
    {
        if( szText[len] == TXEOF)break;
    }
    return len;
}

/*//code为代码(原始区位码,或处自建字库的位置码),返回值为转换后可以使用的相应代码
mode = HZCODE 正常16*16字库
HZCDEF  自建16*16字库
自建16*8文字库 HZCEDEF
正常8*8字库  HZECODE
自建8*8字库 HZEDEF
//*/
int GetHZcode(int code, int mode)
{
    int retvalue;
    if( (code >= 0 )&& ( mode == HZCODE || mode == HZCDEF || mode == HZCEDEF ) )
    {
        //code是区位码,或自建字库位置码,高度为16点
        retvalue = ( (code)/100  + mode )*256+( (code)%100   + mode);
    }
    else if( (code >= 0 && code < 0x100 ) && ( mode == HZECODE || mode == HZEDEF ) )
    {
        retvalue = code;
    }
    else retvalue = -1;
    return retvalue;
}
void HZ2Code( int code, int mode, u_char *szText, int point)
{
    int retvalue;
    if( (code >= 0 )&& ( mode == HZCODE || mode == HZCDEF || mode == HZCEDEF ) )
    {
        //code是区位码,或自建字库位置码,高度为16点
        retvalue = ( (code)/100  + mode )*256+( (code)%100   + mode);
        szText[point] = (retvalue & 0xff00 )/ 256;
        szText[point+1] = retvalue & 0xff ;
    }
    else if( (code >= 0 && code < 0x100 ) && ( mode == HZECODE || mode == HZEDEF ) )
    {
        retvalue = code;
        szText[point] = retvalue & 0xff ;
    }
    else retvalue = -1;

}



#endif

