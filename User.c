//用户管理UserManager


#ifndef _USER_C_
#define _USER_C_


USER stUser[C_USERMAX];

const u_short _User_A1[] =
{
    CH(5135),CH( 100),CH(2707),CH( 100),CH(2139),CH( 100),CH(3428)
};
/* 用户登录*/

const u_short _User_A2[] =
{
    CH( 100),CH( 100),CH(4853),CH(3591),CH( 100),
    CH( 100),CH( 100),CH( 100),CH(2504),CH(2637)
};
/* 姓名 工号*/
const u_short _User_B1[] =
{
    '+',CH(5286),CH(5217),' ','-',CH(2885),CH(5217),' ','#',CH(2436),CH(3560),' ',215,CH(2988),CH(4075),0,
};//＋增员 － 减员 ＃改密 (回车)进入
const u_short _User_B2[] =
{
    CH(5286),CH(2851),CH(4043),CH(5217),
    CH(217),CH(4853),CH(3591),CH(326),
    CH(218),CH(2504),CH(2637),CH(326),
    CH(219),CH(3560),CH(3475),CH(326),
};
/*
增加人员
姓名：
工号：
密码：
*/
const u_short _User_B3[] =
{
    CH(5213),CH(3560),CH(3475),CH(326),
    CH(4834),CH(3560),CH(3475),CH(326),
};
/*
原密码：
新密码：
*/

u_short _User_C1[] =  /* 请选择用户*/
{
    CH(3975),CH(4901),CH(5281),CH(5135),CH(2707)
};
/* 请选择用户*/

u_short _User_C2[] =  /* 密码错误*/
{
    CH(3560),CH(3475),CH(2077),CH(4683)
};


u_short _User_C3[] =  /* 不存在用户*/
{
    CH(1827),CH(2070),CH(5258),CH(5135),CH(2707)
};


u_short _User_C4[] =  /* 用户数已最大*/
{
    CH(5135),CH(2707),CH(4293),CH(5049),CH(5578),CH(2083)
};

RECT frame;
int xpos,ypos;
int para_xpos,para_ypos;
int i,j;
u_short *cpText;
u_short szText[20];
int keycode;
//int temp;
//int deci = 0,number;
int current_pwd,pwd;
int loop_number;//循环层数
int len,row,height;
//u_short InputChar[20];
int nUserNumber;	//当前员工个数
int nCurrentUser;	//当前选中的员工顺序号

int CheckPwd(int x, int y, int pwd)
{
    int number;
    const u_short szText[] = {CH(3560),CH(3475),CH(326)};//密码
    MPushWindow(x, y, x + 8*C_CCHAR_HDOT, y + 24) ;
    EraseDrawRectangle(x, y, x + 8*C_CCHAR_HDOT, y + 24) ;
    CETextOut2(x+4,y+4,szText,3,1,16,0,0);
    x += 3*C_CCHAR_HDOT;
    number = InputPwd(x,y+8,8) ;
    if( pwd > 0 )
    {
        if( number == pwd)number = 0;
        else number = -1;
    }
    MPopWindow();
    return number;
}

int InputPwd(int x, int y,int total_len/*总长度*/)//返回负值表示自己退出，正值有输入
{
    u_int s_xPos = x;		/*初始水平位置*/
    int keycode;
    bool repeat = true;
    int number = 0;
    int len = 0;
//	int i;

    do
    {
        if( repeat )
        {
            x = s_xPos;		//初始水平位置
            MEraseWindow(x,y,x+C_ECHAR_HDOT*len,y+C_ECHAR_VDOT); //清写数字区域
            len =0;			//代表当前输入的总长度
            number = 0;		/*代表当前输入的数字*/
            repeat = false;
        }

        InvertBytes( x, y, 1); /*反白显示1个字节的区域*/
        keycode = MAnyKeyReturn();
        if (keycode == C_KEYCOD_CONFIRM )
        {
            break;	/*输入确认键*/
        }
        if (keycode == C_KEYCOD_RETURN )
        {
            number = -1;
            break;	/*输入确认键*/
        }
        if ( keycode == 0 && number ==0 )		//第一个不能输入0
        {
            continue;
        }
        else if (keycode >=0 && keycode <= 9)		/*输入数字键,无符号*/
        {
            number = number * 10 +keycode;
        }
        else if (keycode == C_KEYCOD_DEL)
        {
            repeat = true;
            len++;
            continue;
        }
        else continue;

        Write_Ascii(x,y,'*');
        len++ ;
        if (len < total_len) x += C_ECHAR_HDOT;	/*已输入字数小于定义的最大长度*/
        if( len > total_len ) repeat = true;
    }
    while(true);

    if(len > total_len)len = total_len-1;
    MEraseWindow(s_xPos,y,s_xPos+C_ECHAR_HDOT*(len+1),y+C_ECHAR_VDOT); /*清写数字区域*/

    return number;
}

void WritePwd(int x, int y, int number)
{
    while( number > 0)
    {
        Write_Ascii(x, y, '*');
        x += C_ECHAR_HDOT;	/*水平位置加1字符长度=C_ECHAR_HDOT=8*/
        number /= 10;
    }
}
int GetPwd(int nUserNumber)//得到序号用户的密码
{
    if( nUserNumber > GetUserNumber())nUserNumber = 0;
    return stUser[nUserNumber].pwd;
}

int UserStore(void)
{
    int retvalue;
    int offset;

    offset = C_OFF_USER+C_SIZE_INT*2;
    retvalue = MCopyProtectedMemory((void*)offset,&stUser, C_LEN_USER, PM_COPY_WRITE);

    return retvalue;
}

int UserRestore(void)
{
    int retvalue;
    int offset;

    offset = C_OFF_USER+C_SIZE_INT*2;
    retvalue = MCopyProtectedMemory(&stUser, (void*)offset,C_LEN_USER, PM_COPY_READ);

    return retvalue;
}

int GetUserNumber()
{
    //得到目前的用户数
    int retvalue;
    int offset;

    offset = C_OFF_USER;
    MCopyProtectedMemory(&retvalue, (void*)offset,C_SIZE_INT, PM_COPY_READ);
    if( retvalue > C_USERMAX)retvalue = C_USERMAX;
    else if( retvalue < 0 )retvalue = 0;

    //Write_Number( 50,10,retvalue,4,0,0);
    return retvalue;
}
void SetUserNumber( int number)
{
    int retvalue = number;
    int offset;

    if( retvalue > C_USERMAX)retvalue = C_USERMAX;
    else if( retvalue < 0 )retvalue = 0;
    offset = C_OFF_USER;
    MCopyProtectedMemory((void*)offset,&retvalue, C_SIZE_INT, PM_COPY_WRITE);
//	Write_Number( 0,10,retvalue,4,0,0);
}
int GetCurrentUser()
{
    //得到当前的用户序号
    int retvalue;
    int offset;
    int nUserNumber = GetUserNumber();

    offset = C_OFF_USER+C_SIZE_INT;
    MCopyProtectedMemory(&retvalue, (void*)offset,C_SIZE_INT, PM_COPY_READ);
    if( retvalue >= nUserNumber)retvalue = nUserNumber-1;
    else if( retvalue < 0 )retvalue = 0;
//Write_Number( 50,0,retvalue,4,0,0);
    return retvalue;
}

void SetCurrentUser( int user)
{
    int retvalue = user;
    int offset;
    int nUserNumber = GetUserNumber();
//Write_Number( 100,0,nUserNumber,4,0,0);

    if( retvalue > nUserNumber)retvalue = nUserNumber;
    else if( retvalue < 0 )retvalue = -1;
    offset = C_OFF_USER+C_SIZE_INT;
    MCopyProtectedMemory((void*)offset,&retvalue, C_SIZE_INT, PM_COPY_WRITE);
//Write_Number( 0,0,retvalue,4,0,0);
}

int GetUserName( u_short name[],int nUserNumber)
{
//	u_int offset;
    int ret = 0;
    if( nUserNumber >= GetUserNumber() || nUserNumber < 0)return ret;

//	offset = C_OFF_USER+C_SIZE_INT*2;
//	USER user[C_USERMAX];
//	ret = MCopyProtectedMemory(&user, (void*)offset,C_LEN_USER, PM_COPY_READ);
//	for( ret = 0; ret <= C_USERNAMELEN; ret++)name[ret] = user[nUserNumber].name[ret];
    for( ret = 0; ret <= C_USERNAMELEN; ret++)name[ret] = stUser[nUserNumber].name[ret];
    return 1;
}

   /* char _Pwd_A1[] =
    {
        "ID1:ID2:ID3:"
    };*/
    const u_char _Pwd_A1[C_LANGUAGE][11][22] =
    {
        {
            "⒈类型    ：",
            "⒉密码    ：",
            "⒊日期    ：",
        },//中文
        {
            "1.type    ：",
            "2.password：",
            "3.maturity：",
        },
    };
    u_short _Pwd_B1[] =
    {
        CH(4721),CH(4519),CH(2302),CH(4190),CH(2077),CH(4683),',',CH(16),
        CH(3975),CH(5175),CH(3647),CH(4508),CH(5149),CH(3310),CH(3310),CH(4721),'!',CH(16),
        CH(2171),CH(2716),':','0','5','1','3','8','5','2','9','6','9','7','1',CH(16),
    };//系统发生错误，请与南通友联联系！电话：05135296971！

    u_short _Pwd_C1[] =
    {
        CH(4721),CH(4519),CH(2302),CH(4190),CH(5434),CH(3592),CH(2077),CH(4683),',',CH(16),
        CH(3975),CH(5175),CH(3647),CH(4508),CH(5149),CH(3310),CH(3310),CH(4721),'!',CH(16),
        CH(2171),CH(2716),':','0','5','1','3','8','5','2','9','6','9','7','1',CH(16),
    };//系统发生致命错误，请与南通友联联系！电话：05135296971
    u_short _Pwd_D1[] =
    {
        CH(2077),CH(4683),CH(2637),':',
    };//错误号：

void SysPwd(int mode)
{
    int xpos,ypos,para_xpos,para_ypos;
    int i,j,k;
//	u_short *cpText;
//	u_short szText[20];
//	int keycode;
    unsigned int pwd,pwd1,pwd2;
    unsigned int number, deci_len;
    TIME_CLOCK tm;
    int row = 24;
//#if 0
    /*
        	char _Pwd_A1[] = {
        	"ID1:ID2:ID3:"};
        	u_short _Pwd_B1[] = {
        		CH(4721),CH(4519),CH(2302),CH(4190),CH(2077),CH(4683),',',CH(16),
        		CH(3975),CH(5175),CH(3647),CH(4508),CH(5149),CH(3310),CH(3310),CH(4721),'!',CH(16),
        		CH(2171),CH(2716),':','0','5','1','3','8','5','2','9','6','9','7','1',CH(16),
        	};//系统发生错误，请与南通友联联系！电话：05135296971！

        	u_short _Pwd_C1[] = {
        		CH(4721),CH(4519),CH(2302),CH(4190),CH(5434),CH(3592),CH(2077),CH(4683),',',CH(16),
        		CH(3975),CH(5175),CH(3647),CH(4508),CH(5149),CH(3310),CH(3310),CH(4721),'!',CH(16),
        		CH(2171),CH(2716),':','0','5','1','3','8','5','2','9','6','9','7','1',CH(16),
        	};//系统发生致命错误，请与南通友联联系！电话：05135296971
        	u_short _Pwd_D1[] = {
        	CH(2077),CH(4683),CH(2637),':',
        	};//错误号：
    */
    MFclearScreen();
    MKeyRlx();
    if( mode == 0)
    {
        //检查是否到期

        for( i = 0; i < 5; i++)
        {
            number = C_OFF_INIT + i * C_SIZE_INT;
            MCopyProtectedMemory( &pwd, (void*)number, C_SIZE_INT, PM_COPY_READ);
            if( pwd != i)break;
        }
        number = C_OFF_PWD;
        MCopyProtectedMemory(&pwd, (void*)number, C_SIZE_INT, PM_COPY_READ);
        number = C_OFF_PWD + 2*C_SIZE_INT;//存日期
        MCopyProtectedMemory(&pwd1, (void*)number, C_SIZE_INT, PM_COPY_READ);

        if( pwd != 0 && pwd1 > 0 && i >= 5)//i<5说明是数据混乱
        {
            //有密码
            GetSystemTime(&tm);
            pwd2 = date2int2(&tm);
            xpos = 180;
            ypos = 160;
            if( pwd1 > pwd2 && pwd1 <= pwd2 + 7)
            {
                //7天内到期
                CETextOut2(xpos,ypos,_Pwd_B1,15,3,24,1,0);
                CEMenuOut(xpos,ypos + 24*3,_Pwd_D1,4,1,24);
                Write_Number(xpos + 4*C_CCHAR_HDOT,ypos+24*3,pwd,10,0,0);
                MAnyKeyReturn();
            }
            else if( pwd1 <= pwd2)
            {
                //到期
                CETextOut2(xpos,ypos,_Pwd_C1,15,3,24,1,0);
                CEMenuOut(xpos,ypos + 24*3,_Pwd_D1,4,1,24);
                Write_Number(xpos + 4*C_CCHAR_HDOT,ypos+24*3,pwd,10,0,0);
                while(true);
            }
        }
    }
    else
    {
        MFclearScreen();
        para_xpos = xpos = 20;
        para_ypos = ypos = 40;
        //EMenuOut(xpos ,ypos,_Pwd_A1,4,3,row);
        TextOut(xpos ,ypos,1,12,C_CCHAR_VDOT,(u_char*)_Pwd_A1[MGetLanguage()][0],4);
        TextOut(xpos ,ypos+24,1,12,C_CCHAR_VDOT,(u_char*)_Pwd_A1[MGetLanguage()][1],4);
        TextOut(xpos ,ypos+24*2,1,12,C_CCHAR_VDOT,(u_char*)_Pwd_A1[MGetLanguage()][2],4);
        xpos = para_xpos + 10*C_ECHAR_HDOT;
        number = 0;
        deci_len = 0;
        if( Input_Number(xpos,ypos,&number,2,&deci_len,0) > 0)
        {
            switch( number )
            {
            case 1://加密
                ypos += row;
                number = C_OFF_PWD;
                MCopyProtectedMemory(&pwd, (void*)number, C_SIZE_INT, PM_COPY_READ);
                number = C_OFF_PWD + 2*C_SIZE_INT;//存日期
                MCopyProtectedMemory(&pwd1, (void*)number, C_SIZE_INT, PM_COPY_READ);

                if( pwd == 0 && pwd1 == 0 )
                {

                    //pwd = CheckPwd(xpos,ypos,0);
                    //if( pwd == C_SUPERPWD )
                    {
                        number = 0;
                        while( true)
                        {
                            if( Input_Number(xpos,ypos,&number,8,&deci_len,0) > 0)
                            {
                                if( number > 99999999)continue;
                                ypos += row;
                                if( InputDate(xpos,ypos, &tm) == C_TRUE)
                                {
                                    pwd = GetElapsedTime();
                                    pwd = ( ( (pwd%65536+1)*(pwd/65536+1) )%(number+10000000) ) * GetElapsedTime() * date2int2(&tm) ;
                                    pwd = ( pwd % 100 ) + ( pwd / 100 % 100 ) + ( pwd / 10000 % 100 );
                                    pwd = pwd % 40;
                                    j = 10 + pwd/10;
                                    k = 10 + pwd%10;
                                    for( i = 0,pwd1 = 0, pwd2 = 0; i < 7; i++)
                                    {
                                        pwd1 = pwd1 * 10 + (j++)%10;
                                        pwd2 = pwd2 * 10 + (k--)%10;
                                    }
                                    pwd = ( (number ^ pwd1 ) ^ pwd2 ) + pwd * 10000*10000;//最高两位为标志


                                    number = C_OFF_PWD;//存密码
                                    MCopyProtectedMemory((void*)number,&pwd, C_SIZE_INT, PM_COPY_WRITE);
                                    CopyMemoryToSystem(number, C_SIZE_INT,PM_COPY_WRITE);
                                    number = C_OFF_PWD + C_SIZE_INT;//存日期
                                    pwd1 = date2int(&tm);//日期
                                    MCopyProtectedMemory((void*)number,&pwd1, C_SIZE_INT, PM_COPY_WRITE);
                                    CopyMemoryToSystem(number, C_SIZE_INT,PM_COPY_WRITE);
                                    number = C_OFF_PWD + 2*C_SIZE_INT;//存日期
                                    pwd1 = date2int2(&tm);//日期
                                    MCopyProtectedMemory((void*)number,&pwd1, C_SIZE_INT, PM_COPY_WRITE);
                                    CopyMemoryToSystem(number, C_SIZE_INT,PM_COPY_WRITE);
                                }
                                break;
                            }
                        }
                    }
                }
                break;
            case 2://解密
                ypos += row;
                //pwd = CheckPwd(xpos,ypos,0);
                //if( pwd == C_SUPERPWD )
                {
                    number = C_OFF_PWD;
                    MCopyProtectedMemory(&pwd, (void*)number, C_SIZE_INT, PM_COPY_READ);

                    k = ( pwd/100000000)%100;//最高两位
                    pwd = pwd%100000000;
                    j = 10 + k/10;
                    k = 10 + k%10;
                    for( i = 0,pwd1 = 0, pwd2 = 0; i < 7; i++)
                    {
                        pwd1 = pwd1 * 10 + (j++)%10;
                        pwd2 = pwd2 * 10 + (k--)%10;
                    }
                    pwd = (pwd ^ pwd1) ^ pwd2;

                    number = 0;
                    if( Input_Number(xpos,ypos,&number,8,&deci_len,0) > 0)
                    {
                        if( pwd == number)
                        {
                            pwd = 0;
                            number = C_OFF_PWD;//存密码
                            MCopyProtectedMemory((void*)number,&pwd, C_SIZE_INT, PM_COPY_WRITE);
                            CopyMemoryToSystem(number, C_SIZE_INT,PM_COPY_WRITE);
                            number = C_OFF_PWD + C_SIZE_INT;//存日期
                            MCopyProtectedMemory((void*)number,&pwd, C_SIZE_INT, PM_COPY_WRITE);
                            CopyMemoryToSystem(number, C_SIZE_INT,PM_COPY_WRITE);
                            number = C_OFF_PWD + 2*C_SIZE_INT;//存日期
                            MCopyProtectedMemory((void*)number,&pwd, C_SIZE_INT, PM_COPY_WRITE);
                            CopyMemoryToSystem(number, C_SIZE_INT,PM_COPY_WRITE);
                        }
                    }
                }
                break;
            case 3://查询
                ypos += row;
                number = C_OFF_PWD;
                MCopyProtectedMemory(&pwd, (void*)number, C_SIZE_INT, PM_COPY_READ);
                number = C_OFF_PWD + C_SIZE_INT;//取日期
                MCopyProtectedMemory(&pwd1, (void*)number, C_SIZE_INT, PM_COPY_READ);
                int2date(&tm,pwd1);
                Write_Number(xpos,ypos,pwd,10,0,0);
                ypos += row;
                WriteDate(xpos,ypos,&tm);
                MAnyKeyReturn();
                break;
            case 4:	//设置通道数、设置是否带U盘、

                break;

            case 9:
                ypos += row;
                pwd=0;
                number=0;
                while (1)
                {
                    if( Input_Number(xpos,ypos,&number,9,&deci_len,0) > 0)
                    {
                        if( number > 9999999999)continue;
                    }
                    pwd=number;
                    k = ( pwd/100000000)%100;//最高两位
                    pwd = pwd%100000000;
                    j = 10 + k/10;
                    k = 10 + k%10;
                    for( i = 0,pwd1 = 0, pwd2 = 0; i < 7; i++)
                    {
                        pwd1 = pwd1 * 10 + (j++)%10;
                        pwd2 = pwd2 * 10 + (k--)%10;
                    }
                    pwd = (pwd ^ pwd1) ^ pwd2;

                    Write_Number(xpos,ypos+row,pwd,10,0,0);
                }
                break;
            default:
                break;
            }

        }
    }
//#endif
}

//////////////////////////////////////


unsigned short DAC_DB[18];
///unsigned short xpos, ypos;

//unsigned short dist_dire,DIST_HORI,TEMP;
//unsigned short ProbeBeta = 600;
///unsigned short ProbeLvalue = 100;

void WELD_DRAW( WELD_PARA *stWeld,int wlpos, int wrpos,int wypos,int maxsize,int ProbeBeta, int dist_dire)
//void WELD_DRAW( CDC *pDC)
{
    unsigned short ax, bx = 0,cx;
    WELD_POSI stWPosi;

    if( stWeld->type == 0 )return;

    stWPosi.lpos = wlpos;//200;//焊缝左端
    stWPosi.rpos = wrpos;//stWPosi.lpos + 10 * 16;//右端
    stWPosi.ypos = wypos;//400;//焊缝中心垂直位置,xpos为水平位置
    stWPosi.size = maxsize;//焊缝图示的点数

    if( stWeld->type < 3)
    {
        //有焊缝设置
        ax = (stWeld->thick1 + stWeld->high1 + stWeld->high2)/2;//工件厚度、翼板厚度的一半+上余高
        if(  ax < stWeld->space )//根部间隙，
        {
            ax = stWeld->space;
        }
        if(  ax < stWeld->wide )//焊缝宽度
        {
            ax = stWeld->wide;
        }
        bx = ax / 2; //ax取焊缝根部间隙/焊缝宽度/工件厚度一半+上余高的最大值,bx为其一半
    }
    else
    {
        //T
        ax = (stWeld->thick2 + stWeld->high1 + stWeld->high2)/2;//工件厚度、翼板厚度的一半+上余高
        if( stWeld->wide > stWeld->radius )bx = (stWeld->thick1 + stWeld->space + stWeld->wide)/2;
        else bx = (stWeld->thick1 + stWeld->space + stWeld->radius)/2;
        if( ax > bx )bx = ax;
        //else bx /= ;
    }
    while (true)
    {
        if(  stWeld->face == 0 )
        {
            //A面探
            stWPosi.xpos = stWPosi.rpos - stWPosi.size - 10;
        }
        else
        {
            //B面或C面探
            stWPosi.xpos = stWPosi.lpos + stWPosi.size + 10;
        } //xpos为焊缝中心水平位置

        stWPosi.dots = 500 * stWPosi.size / bx;//将长度单位的值转化为点数
        //cx为探头距焊缝长度+前沿长度转化的点数+图示
        cx = (stWeld->probe + 0) * stWPosi.dots / 1000 + stWPosi.size * 2;
        if(  cx < (stWPosi.rpos - stWPosi.lpos) ) break;//能显示下则退出,不能则减小焊缝图示在点数

        stWPosi.size = stWPosi.size - 1;//原设置的图示点数过大,减小再试

    }

    MSetColor( C_CR_PARA );
    if(  stWPosi.dots != 0 )
    {
        //图示点数不为0,可以画图
        if(  stWeld->type == 1 )
        {
            //单面
            SINGALWELD_DRAW(stWeld, &stWPosi);
        }
        if(  stWeld->type == 2 )
        {
            //双面
            SINGALWELD_DRAW(stWeld, &stWPosi);
        }
        if(  stWeld->type == 3 )
        {
            //T型
            TWELD_DRAW(stWeld, &stWPosi);
        }
    }

    WELD_BUGLINE(stWeld, &stWPosi,ProbeBeta, dist_dire);//画声波路线图


}


void SINGALWELD_DRAW( WELD_PARA *stWeld,WELD_POSI *stWPosi)//单面焊
{
    unsigned short ax, bx, cx, dx, bp, si, di ;
    unsigned short ANTAG,ANCOS,ANSIN;

    ax = stWeld->space* stWPosi->dots / 1000  / 2;//根部间隙一半的点数
    bp = stWPosi->xpos - ax;//焊缝中心向左去掉根部间隙
    cx = bp - stWPosi->lpos;//根部间隙左到焊缝左的点数
    ax = stWeld->thick1 * stWPosi->dots/1000 / 2;//工件厚一半的点数
    bx = stWPosi->ypos - ax;//工件上端的位置
    DAC_DB[13] = bx;         //DAC_DB[40)为工件上端垂直位置
    bx = ax * 2 + bx;//为工件下端垂直位置
    ax = stWPosi->lpos;

    if(  stWeld->type != 2 )
    {
        m_DrawLine(  ax, bx, ax + cx, bx);
        ax = stWPosi->xpos * 2 - bp;
        cx = stWPosi->rpos - ax;
        m_DrawLine(  ax, bx, ax + cx, bx);
    }
    else
    {
        m_DrawLine(  ax, bx, ax, bx + 1);
    }
    //下端两横线
    ax = stWeld->deep * stWPosi->dots/1000;
    bx = DAC_DB[13] + ax;
    DAC_DB[10] = bx ;            //DAC_DB[20)钝边上端垂直位置
    ax = stWeld->blunt * stWPosi->dots/1000;
    DAC_DB[5] = ax ;            //DAC_DB[10)钝边长
    cx = ax;
    ax = bp;		//焊缝中心向左去掉根部间隙
    DAC_DB[0] = ax  ;            //DAC_DB[0)钝边上端水平位置
    m_DrawLine(  ax, bx, ax, bx + cx);//画左钝边
    ax = stWPosi->xpos * 2 - bp;
    if(  stWeld->space != 0 )
    {
        //根部间隙不为0,则还有右钝边
        m_DrawLine(  ax, bx, ax, bx + cx);
    }

    ax = bx + cx;
    DAC_DB[1] = ax ; //（2），钝边下端垂直位置
    ax = stWeld->angle1 - stWeld->angle2;//波口左面角度
    ANTAG = (UINT)( tan(ax * PI / 1800) * 100 );
    ANCOS = (UINT)( cos(ax * PI / 1800) * 1000);
    ANSIN = (UINT)( sin(ax * PI / 1800) * 1000);
    if(  stWeld->radius != 0 )
    {
        //根部半径不为0
        ax = stWeld->radius * stWPosi->dots/1000;
        DAC_DB[12] = ax;         //DAC_DB[24),U型圆弧根部半径
        ax = ax * ANCOS / 1000;
        bx = stWPosi->xpos - ax;
        DAC_DB[11] = bx ;        //DAC_DB[22),U型圆弧左端水平位置
        ax = DAC_DB[12];
        di = ax * ax;
        bx = DAC_DB[10] - ax  ;           //DAC_DB[20)钝边上端垂直位置
        DAC_DB[15] = bx ;             //DAC_DB[30),
        //    bx = bx / 1000 - ax * ANSIN
        bx = bx + ax * ANSIN/ 1000 ;
        DAC_DB[16] = bx;             //DAC_DB[32),U型圆弧左端垂直位置
        cx = DAC_DB[0]; //DAC_DB[0)钝边上端水平位置
        bx = DAC_DB[11];
        bp = 0;
        if(  cx > bx ) //钝边上端水平位置， U型圆弧左端水平位置
        {
            int i ;
            cx = cx - bx;
            ax = stWeld->angle2;//坡口面角，坡口右角度
            bx = stWeld->angle1;//坡口角度
            if(  ax != bx || ax == 0 )
            {
                //面角跟整个坡口角度不等或面角不为0，即坡口左边有角度
                for( i = 1 ; i < cx ; i++)
                {
                    ax = stWPosi->xpos - DAC_DB[11] - bp;
                    si = ax * ax;
                    bx = DAC_DB[15] + (UINT)sqrt(di - si);//di：U型圆弧根部半径的平方
                    ax = DAC_DB[11] + bp;
                    m_DrawDot(  ax, bx);
                    bp = bp + 1;
                }
            }
            else
            {
                DAC_DB[11] = DAC_DB[0];
                DAC_DB[16] = DAC_DB[10];
            }
        }
        else
        {
            DAC_DB[11] = DAC_DB[0];
            DAC_DB[16] = DAC_DB[10];
        }
    }
    else
    {
        DAC_DB[11] = DAC_DB[0]  ;//
        DAC_DB[16] = DAC_DB[10];
    }

    ax = DAC_DB[16] - DAC_DB[13];    //DAC_DB[13)为工件上端垂直位置
    si = ax             ;            //DAC_DB[16),U型圆弧左端垂直位置
    di = ax * ANTAG / 100;
    bp = DAC_DB[11];
    bx = bp - di;
    cx = stWPosi->lpos;
    if(  bp < di || bx < cx )
    {
        ax = bp - cx;
        si = ax * si / di;
        bx = cx;
        di = bp - bx;
    }
    dx = DAC_DB[16] ;        //DAC_DB[32),U型圆弧左端垂直位置
    ax = dx - si;
    stWPosi->wxpos = bx;
    //stWPosi->wypos = dx
    m_DrawLine(  bx, ax, bp, dx);//坡口上左斜线
    m_DrawLine(  stWPosi->lpos, ax, bx, ax);//工件上左横线


    ax = stWPosi->xpos * 2 - DAC_DB[0];
    DAC_DB[17] = ax;     //DAC_DB[34)，右钝边水平位置，或U型圆弧右端水平位置

    ax = stWeld->angle2;
    ANTAG = (UINT)(tan(ax * PI / 1800) * 100);
    ANCOS = (UINT)(cos(ax * PI / 1800) * 1000);
    ANSIN = (UINT)(sin(ax * PI / 1800) * 1000);
    ax = stWeld->radius;
    if(  ax != 0 )
    {
        ax = DAC_DB[12] * ANCOS/ 1000;     //DAC_DB[24),U型圆弧根部半径
        bx = ax + stWPosi->xpos;
        DAC_DB[11] = bx ;         //DAC_DB[22),U型圆弧右端水平位置
        ax = DAC_DB[12];
        di = ax * ax;
        bp = 0;
        bx = DAC_DB[15] + ax  * ANSIN/ 1000;
        DAC_DB[16] = bx ;   //DAC_DB[32),U型圆弧右端垂直位置
        bx = DAC_DB[17];
        cx = DAC_DB[11];
        if(  cx > bx )
        {
            cx = DAC_DB[11] - DAC_DB[17];
            for( bp = 0 ; bp < cx ; bp++)
            {
                ax = DAC_DB[11] - stWPosi->xpos - bp;
                si = ax * ax;
                bx = DAC_DB[15] + (UINT)sqrt(di - si);
                ax = DAC_DB[11] - bp;
                m_DrawDot(  ax, bx);
            }
        }
        else
        {
            DAC_DB[11] = DAC_DB[17];
            DAC_DB[16] = DAC_DB[10];
        }
    }
    else
    {
        DAC_DB[11] = DAC_DB[17];
        DAC_DB[16] = DAC_DB[10];
    }

    si = DAC_DB[16] - DAC_DB[13];
    di = si * ANTAG / 100;
    bx = DAC_DB[11];
    bp = bx + di;
    cx = stWPosi->rpos;
    if(  bp > cx )
    {
        ax = cx - bx;
        si = ax * si / di;
        bp = cx;
        di = bp - bx;
    }
    ax = DAC_DB[16];
    dx = ax - si;
    stWPosi->wypos = bp;
    m_DrawLine(  bx, ax, bp, dx);//坡口上右斜线
    m_DrawLine(  bp, dx, stWPosi->rpos, dx);//工件上右横线

    //画工件上端两横线，至此单面坡口完成


    di = stWPosi->ypos - stWeld->thick1 * stWPosi->dots/1000 / 2 ;   //工件上端
    si = stWeld->thick2 * stWPosi->dots/1000;      //有效厚度，焊缝左半部高度
    cx = stWeld->wide * stWPosi->dots/1000;
    bx = (stWPosi->wxpos + stWPosi->wypos - cx) / 2 ;    //焊缝左端位置
    stWPosi->wxpos = bx;
    DAC_DB[14] = bx ;        //DAC_DB[14),上焊缝左端位置
    DAC_DB[7] = bx + cx ;   //DAC_DB[7),上焊缝右端位置
    if(  stWeld->face > 0 )
    {
        stWPosi->wxpos = DAC_DB[7];
    }
    bp = stWPosi->xpos - bx;     //焊缝左半部宽度
    stWPosi->wypos = di;
    ax = (UINT)( stWeld->high1 * stWPosi->dots/1000.0+0.5) ;
    DAC_DB[6] = ax;     //（12）上余高
    ax = (UINT)( stWeld->high2 * stWPosi->dots/1000.0+0.5) ;
    DAC_DB[8] = ax;     //（12）上余高

    if(  cx != 0 && si != 0 )
    {
        //焊缝宽度不为0，或有效厚度不为0
        MSetColor( C_CR_MENU );

        ax =  DAC_DB[14] ;        //DAC_DB[36),上焊缝左端位置,DAC_DB[14),上焊缝右端位置
        bx = stWPosi->wypos;//工件上端
        cx = ( DAC_DB[7] + DAC_DB[14])/2 ;
        dx = bx - DAC_DB[6];    //（12）上余高
        m_DrawArc3( ax, bx, cx, dx, 1);
        ax = DAC_DB[7];
        m_DrawArc3( cx, dx, ax, bx, 1);
        ax =  DAC_DB[14] ;        //DAC_DB[36),上焊缝左端位置,DAC_DB[14),上焊缝右端位置

        if( stWeld->type == 1)dx = bx+si + DAC_DB[8];//单面焊考虑下余高
        else dx = bx+si;

        m_DrawArc3( ax, bx, cx, dx, -1);
        ax = DAC_DB[7];
        m_DrawArc3( cx, dx, ax, bx, -1);

        MSetColor( C_CR_PARA );
    }
    else m_DrawLine(  DAC_DB[14],stWPosi->wypos,DAC_DB[7],stWPosi->wypos);


    if(  stWeld->type == 2 )
    {
        DOUBLEWELD_DRAW(stWeld,stWPosi);
    }


}

void DOUBLEWELD_DRAW( WELD_PARA *stWeld,WELD_POSI *stWPosi)
{
    unsigned short ax, bx, cx, dx, bp, si, di ;
    unsigned short ANTAG,ANCOS,ANSIN;

    cx = stWPosi->xpos;
    ax = cx - DAC_DB[14] ;   //（14）上焊缝左端
    bx = stWeld->thick1 - stWeld->deep - stWeld->blunt;
    cx = cx - ax * bx / stWeld->deep;
    DAC_DB[8] = cx ;    //（8）下焊缝左端
    ax = DAC_DB[7] - stWPosi->xpos;   //DAC_DB[14),上焊缝右端位置
    ax = ax * bx / stWeld->deep + stWPosi->xpos;
    DAC_DB[9] = ax;     //（9），下焊缝右端
    // ax = DAC_DB[12) * bx / stWeld->deep
    ax = stWeld->high2 * stWPosi->dots/1000;
    DAC_DB[6] = ax;     //（6），下焊缝余高


    ax = stWeld->angle1 - stWeld->angle2;
    ANTAG = (UINT)(tan(ax * PI / 1800) * 100);
    ANCOS = (UINT)(cos(ax * PI / 1800) * 1000);
    ANSIN = (UINT)(sin(ax * PI / 1800) * 1000);
    ax = stWeld->thick1 - stWeld->deep - stWeld->blunt;
    si = ax * stWPosi->dots/1000 ;    //焊缝下坡口高度
    DAC_DB[2] = si;
    di = si * ANTAG / 100;
    bp = DAC_DB[0];
    bx = bp - di;
    cx = stWPosi->lpos;
    if(  bp < di || bx < cx )
    {
        si = (bp - cx) * si / di;
        bx = cx;
        di = bp - bx;
    }
    DAC_DB[3] = bx;
    dx = DAC_DB[1] ; //（2），钝边下端垂直位置
    ax = dx + si;
    m_DrawLine(  bx, ax, bp + 1, dx);
    m_DrawLine(  stWPosi->lpos, ax, bx, ax);

    ax = stWeld->angle2;
    ANTAG = (UINT)(tan(ax * PI / 1800) * 100);
    ANCOS = (UINT)(cos(ax * PI / 1800) * 1000);
    ANSIN = (UINT)(sin(ax * PI / 1800) * 1000);
    bx = stWPosi->xpos * 2 - bp;
    si = DAC_DB[2];
    di = si * ANTAG / 100;
    bp = bx + di;
    cx = stWPosi->rpos;
    if(  bp > cx )
    {
        si = (cx - bx) * si / di;
        bp = cx;
        di = bp - bx;
    }
    ax = DAC_DB[1];
    dx = ax + si;
    m_DrawLine(  bx, ax, bp + 1, dx);
    m_DrawLine(  bp, dx, stWPosi->rpos, dx);


    bx = DAC_DB[8] ;    //（16）下焊缝左端
    bp = DAC_DB[9] ;  //（18），下焊缝右端
    ax = DAC_DB[10] ;    //(20),钝边上端垂直位置
    cx = DAC_DB[1] + DAC_DB[2] ;//（2），钝边下端垂直位置 (4)下坡口高度
    dx = cx + DAC_DB[6] ; //（12），下焊缝余高,

    if(  ax != dx && DAC_DB[6] > 0)
    {
        MSetColor( C_CR_MENU );
        ax = DAC_DB[8];
        bx = stWPosi->ypos + stWeld->thick1 * stWPosi->dots/1000 / 2; //工件下端
        cx = ( DAC_DB[9] + DAC_DB[8])/2 ;        //DAC_DB[36),下焊缝左端位置,DAC_DB[14),下焊缝右端位置
        dx = bx +DAC_DB[6] ; //（12），下焊缝余高,
        m_DrawArc3(  ax,bx,cx,dx,-1);
        ax = DAC_DB[9];
        m_DrawArc3(  cx,dx,ax,bx,-1);
        dx = DAC_DB[10];
        m_DrawArc3(  cx,dx,ax,bx,1);
        ax = DAC_DB[8];
        m_DrawArc3(  ax,bx,cx,dx,1);
        MSetColor( C_CR_PARA );

    }
    else m_DrawLine(  DAC_DB[8],DAC_DB[1]+DAC_DB[2],DAC_DB[9],DAC_DB[1]+DAC_DB[2]);

}

void TWELD_DRAW( WELD_PARA *stWeld,WELD_POSI *stWPosi)
{
    unsigned short ax, bx, cx, dx, bp, si, di ;
    unsigned short ANTAG,ANCOS,ANSIN;

    ax = stWeld->high2;
    if(  ax < stWeld->thick2 )
    {
        ax = stWeld->thick2;
    }
    bx = stWeld->high1 + stWeld->radius + stWeld->thick1;

    ax = ax + stWeld->space;
    if(  stWeld->face == 2 )
    {
        ax = ax + stWeld->wide;
    }
    else
    {
        ax = ax + stWeld->thick1;
    }
    if(  ax > bx )
    {
        bx = ax;
    }

    while (true)
    {
        ax = 1600 * stWPosi->size / bx;
        cx = (stWeld->probe + 0) * ax / 1000 + stWPosi->size * 2;
        if(  cx < stWPosi->rpos - stWPosi->lpos )break;
        stWPosi->size = stWPosi->size - 1;

    }



    if(  ax > 0 )
    {
        stWPosi->dots = ax;

        DAC_DB[3] = stWeld->thick1 * stWPosi->dots / 1000; //(6)翼板厚
        DAC_DB[1] = stWeld->thick2 * stWPosi->dots / 1000; //(2)腹板厚
        DAC_DB[2] = stWeld->high1 * stWPosi->dots / 1000 ;//(4)A余高
        DAC_DB[9] = stWeld->high2 * stWPosi->dots / 1000 ;//(18)B右余高
        DAC_DB[8] = stWeld->wide * stWPosi->dots / 1000 ;//(16)A左焊脚
        DAC_DB[6] = stWeld->radius * stWPosi->dots / 1000 ;//(12)B右焊脚
        DAC_DB[5] = stWeld->deep * stWPosi->dots / 1000 ;//(10)A坡口高
        DAC_DB[4] = stWeld->blunt * stWPosi->dots / 1000; //(8)钝边长
        DAC_DB[7] = stWeld->space * stWPosi->dots / 1000 ;//(14)根部间隙

        if(  stWeld->face < 2 )
        {
            bx = stWPosi->ypos + stWPosi->size;
            m_DrawLine(  stWPosi->lpos, bx, stWPosi->rpos, bx);
            bx = bx - DAC_DB[3];
            DAC_DB[10] = bx  ;   //（20）翼板上表面
            m_DrawLine(  stWPosi->lpos, bx, stWPosi->rpos, bx);
            bx = bx - DAC_DB[7]; //（14）根部间隙
            ax = stWPosi->xpos - DAC_DB[1] / 2 ;     //(2)腹板厚
            DAC_DB[0] = ax ; //(0)腹板左表面
            ax = ax + DAC_DB[5]; //（10）A坡口高
            if(  stWeld->space != 0 )
            {
                m_DrawLine(  ax, bx, ax + DAC_DB[4], bx);
            }
            ax = stWeld->angle1;
            ANTAG = (UINT)(tan(ax * PI / 1800) * 100);
            ANCOS = (UINT)(cos(ax * PI / 1800) * 1000);
            ANSIN = (UINT)(sin(ax * PI / 1800) * 1000);

            si = DAC_DB[5] ;    //坡口高
            di = si * ANTAG / 100;
            stWPosi->wypos = DAC_DB[10] ;//（20）翼板上表面
            dx = stWPosi->wypos - DAC_DB[7]; //（14）根部间隙
            ax = dx - di;
            cx = stWPosi->ypos - stWPosi->size  * 8/10;
///			cx = DAC_DB[10] - DAC_DB[8] - 2;//(8)A左焊脚//（10）翼板上表面
            if(  ax < cx )
            {
                ax = cx;
            }
            bx = DAC_DB[0] ; //(0)腹板左表面
            bp = bx + DAC_DB[5]; //(10)A坡口高
            m_DrawLine(  bx, ax, bp, dx);
            m_DrawLine(  bx, cx, bx, ax);

            ax = stWeld->angle2;
            ANTAG = (UINT)(tan(ax * PI / 1800) * 100);
            ANCOS = (UINT)(cos(ax * PI / 1800) * 1000);
            ANSIN = (UINT)(sin(ax * PI / 1800) * 1000);
            bx = DAC_DB[5] + DAC_DB[4]; //(10)A坡口深，(8)钝边长
            si = DAC_DB[1] - bx; //（2）腹板宽
            bx = bx + DAC_DB[0] ;    //(0)腹板左表面
            bp = bx + si ;           //腹板右表面
            ax = DAC_DB[10] - DAC_DB[7]; //钝边垂直位置
            dx = ax - si * ANTAG / 100;
            cx = stWPosi->ypos - stWPosi->size * 8/10;
///			cx = DAC_DB[10] - DAC_DB[8] - 2;//(8)A左焊脚//（10）翼板上表面

            if(  dx < cx )
            {
                dx = cx;
            }
            m_DrawLine(  bx, ax, bp, dx);
            m_DrawLine(  bp, cx, bp, dx);


            ax = DAC_DB[0] - DAC_DB[2];//(0)腹板左表面,(4)A余高
            bx = DAC_DB[10];//（20）翼板上表面
            cx = DAC_DB[0];//(0)腹板左表面
            dx = DAC_DB[10] - DAC_DB[8];//(16)A左焊脚//（20）翼板上表面
            MSetColor( C_CR_MENU );
            if( stWeld->face == 0)
            {
                stWPosi->wxpos = ax ;
                stWPosi->wypos = bx ;
            }
            m_DrawArc3( ax,bx,cx,dx,1);
            ax += DAC_DB[2] + DAC_DB[5] + DAC_DB[4];//(10)A坡口深，(8)钝边长
            m_DrawArc3( ax,bx,cx,dx,1);


            ax = DAC_DB[0] + DAC_DB[5];//(0)腹板左表面,(10)A坡口深
            cx = DAC_DB[0] + DAC_DB[1];//(0)腹板左表面,//(2)腹板厚
            dx = DAC_DB[10] - DAC_DB[6];//(12)B右焊脚
            if( stWeld->face == 1)
            {
                stWPosi->wxpos = ax ;
                stWPosi->wypos = bx ;
            }
            m_DrawArc3( ax,bx,cx,dx,1);
            ax = cx + DAC_DB[9];//(18)B右余高
            m_DrawArc3( ax,bx,cx,dx,1);
            MSetColor( C_CR_PARA );

        }
        else
        {
            // ax = DAC_DB[1];
            // DAC_DB[1] = DAC_DB[3];
            // DAC_DB[3] = ax;      //腹板宽（2）与翼板厚（6）互换
            ax = stWPosi->xpos - stWPosi->size*6/10;
            bx = stWPosi->ypos - stWPosi->size*6/10;
            m_DrawLine(  ax, bx, ax, bx + stWPosi->size * 12/10);
            ax = ax + DAC_DB[3] ;//翼板厚
            DAC_DB[10] = ax  ;   //翼板右表面
            m_DrawLine(  ax, bx, ax, bx + stWPosi->size * 12/10);
            ax = stWeld->angle1;

            ANTAG = (UINT)(tan(ax * PI / 1800) * 100);
            ANCOS = (UINT)(cos(ax * PI / 1800) * 1000);
            ANSIN = (UINT)(sin(ax * PI / 1800) * 1000);
            bx = DAC_DB[10] + DAC_DB[7] ;   //（20）翼板右表面，（14）根部间隙
            bp = DAC_DB[5] * ANTAG / 100 + bx ;//（10）A坡口高
            cx = stWPosi->xpos + stWPosi->size;
            if(  bp > cx )
            {
                bp = cx;
            }
            dx = stWPosi->ypos - DAC_DB[1] / 2 ;//（1）腹板厚
            stWPosi->wypos = dx;
            ax = dx + DAC_DB[5];   //（10）A坡口高
            m_DrawLine(  bx, ax, bp, dx);
            m_DrawLine(  bp, dx, stWPosi->rpos, dx);
            dx = ax + DAC_DB[4];
            m_DrawLine(  bx, ax, bx, dx);

            ax = stWeld->angle2;
            ANTAG = (UINT)(tan(ax * PI / 1800) * 100);
            ANCOS = (UINT)(cos(ax * PI / 1800) * 1000);
            ANSIN = (UINT)(sin(ax * PI / 1800) * 1000);
            si = DAC_DB[1] - DAC_DB[5] - DAC_DB[4]; //（2）腹板厚-（10）A坡口高-（8）钝边
            bp = si * ANTAG / 100 + DAC_DB[10] + DAC_DB[7];
            cx = stWPosi->xpos + stWPosi->size;
            if(  bp > cx )
            {
                bp = cx;
            }
            ax = dx;
            dx = ax + si;
            m_DrawLine(  bx, ax, bp, dx);
            m_DrawLine(  bp, dx, stWPosi->rpos, dx);

            bx = DAC_DB[10]  ;       //（20）翼板右表面
            bp = bx + DAC_DB[8];  //(16)A左焊脚
            stWPosi->wxpos = bp;
            cx = stWPosi->wypos;
            ax = cx - DAC_DB[2];
            dx = cx + DAC_DB[5] + DAC_DB[4];
            if(  bp == bx && ax == dx && ax == cx )
            {
                m_DrawLine(  bx, ax, bx, ax);
            }
            else
            {
                MSetColor( C_CR_MENU );
                m_DrawArc3(  bx,ax, bp,cx,1);
                m_DrawArc3(bp,cx,bx,dx,-1);
                MSetColor( C_CR_PARA );
            }
            ax = stWPosi->wypos + DAC_DB[5];
            cx = stWPosi->wypos + DAC_DB[1];
            dx = cx + DAC_DB[9];
            bp = bx + DAC_DB[6] ; //(12)A焊脚
            if(  bp == bx && ax == dx && ax == cx )
            {
                m_DrawLine(  bx, ax, bx, ax);
            }
            else
            {
                MSetColor( C_CR_MENU );
                m_DrawArc3( bx,ax,bp,cx,1);
                m_DrawArc3( bp,cx,bx,dx,-1);
                MSetColor( C_CR_PARA );
            }

        }


    }


}
void WELD_BUGLINE(WELD_PARA *stWeld,WELD_POSI *stWPosi,int ProbeBeta, int dist_dire)
{
    unsigned short ax, bx, cx, dx, bp ,si, di;
    int i;
    int dist_hori,dist_vert;
///	unsigned short ANTAG,ANCOS,ANSIN;

//	unsigned short GATE_TIME[2],TEST_RANGE[4];
//    ANTAG = (UINT)(tan(MPROBE_BETA * PI / 1800)*100 );
 //   ANCOS = (UINT)(cos(MPROBE_BETA * PI / 1800)*1000);
 //   ANSIN = (UINT)(sin(MPROBE_BETA * PI / 1800)*1000);

	if( dist_dire == 0 )return;
    ax = dist_dire * (UINT)(cos(ProbeBeta * PI / 1800)*1000)/1000;//深度
	bp = 0;
    while( ax > stWeld->thick1)//实际声波深度是否大于工件厚度
    {
        ax = ax - stWeld->thick1;
        bp = bp + 1;//i每加1表示穿过一个工件厚度
    }
    //bp = ax;
//	if( bp % 2 != 0)//奇数
    {
//		si = stWeld->thick1 - ax;//距下表妹变为距上表面
    }
//	else
    si = ax;

	di = si * stWPosi->dots / 1000 ;//缺陷距表面距离化为点数深度
	si = si * (UINT)(tan(ProbeBeta * PI / 1800)*1000)/1000 * stWPosi->dots / 1000 ;//化为点数水平


	dist_hori = stWeld->thick1 * (UINT)(tan(ProbeBeta * PI / 1800)*1000)/1000;//一个深度的水平长度
	dist_hori = dist_hori * stWPosi->dots / 1000 ;//一个跨距化为水平点数
	dist_vert = stWeld->thick1 * stWPosi->dots / 1000 ;//化为垂直点数

    dist_dire = stWeld->probe * stWPosi->dots / 1000;//探头距焊缝边距离化为水平点数
    cx = stWPosi->wxpos;
    dx = stWPosi->wypos;
    if( stWeld->face == 0)
    {
        //A面,向右移
        cx -= dist_dire;
        bx = dx + dist_vert;//下表面位置
        for( i = 0; i < bp; i++)
        {
            ax = bx;
            bx = dx;
            dx = ax;//上表面与下表面交换
            ax = cx;
            cx += dist_hori;
            m_DrawLine( ax,bx,cx,dx);
        }
        ax = cx + si;//一个深度的水平长度
        if( bp % 2 != 0)//奇数
        {
            bx = dx - di;//一个深度的水平长度
        }
        else bx = dx + di;//一个深度的水平长度
        if( ax > stWPosi->wxpos && bp%2 != 0 && 1)
        {
            //di
        }
        m_DrawLine( cx, dx, ax,bx);
    }
    else if( stWeld->face == 1 )
    {
        cx += dist_dire;
        bx = dx + dist_vert;
        for( i = 0; i < bp && cx >= dist_hori; i++)
        {
            ax = bx;
            bx = dx;
            dx = ax;//上表面与下表面交换
            ax = cx;
            cx -= dist_hori;
            m_DrawLine( ax,bx,cx,dx);
        }
        if(  i == bp && cx >= si)
        {
            ax = cx - si;//一个深度的水平长度
            if( bp % 2 != 0)//奇数
            {
                if( dx >= di )
                {
                    bx = dx - di;//一个深度的水平长度
                    m_DrawLine( cx, dx, ax,bx);
                }
            }
            else
            {
                bx = dx + di;//一个深度的水平长度
                m_DrawLine( cx, dx, ax,bx);
            }
        }
    }
    else
    {
        cx += dist_dire;
        dist_hori = stWeld->thick1 * (UINT)(tan(ProbeBeta * PI / 1800)*1000)/1000;//一个深度的水平长度
        dist_hori = dist_hori * stWPosi->dots / 1000 ;//一个跨距化为水平点数
        dist_vert = stWeld->thick1 * stWPosi->dots / 1000 ;//化为垂直点数
        bx = dx + dist_vert;
        for( i = 0; i < bp; i++)
        {
            ax = bx;
            bx = dx;
            dx = ax;//上表面与下表面交换
            ax = cx;
            cx -= dist_hori;
            m_DrawLine( ax,bx,cx,dx);
        }
        ax = cx - si;//一个深度的水平长度
        if( bp % 2 != 0)//奇数
        {
            bx = dx - di;//一个深度的水平长度
        }
        else bx = dx + di;//一个深度的水平长度
        m_DrawLine( cx, dx, ax,bx);
    }

}



#endif




