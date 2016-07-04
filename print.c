#include "common.h"
#include "display.h"

//int iPrnPosiAbVert;
//int iPrnPosiAbHori;
extern const const u_char _ParaMenu_AB1[C_LANGUAGE][3][10];
extern const u_char _ParaMenu_AB2[C_LANGUAGE][4][10];
extern int HoriOffsetScreen ;		//所有写在屏幕上的内容，水平方向开始点
extern u_char _infinity[C_LANGUAGE][1][6];
extern const u_char _MainMenu_C1A1[C_LANGUAGE][4][16];
extern const u_char _MainMenu_C1A4[C_LANGUAGE][4][16];

const u_short _Report_A1[] =
{
    CH(1912),CH( 100),CH(4189),CH( 100),CH(1808),CH( 100),CH(2876),
    CH( 100),CH(1866),CH( 100),CH(1708),CH( 100),CH(2470)
};
/*超 声 波 检 测 报 告*/
const u_char _Report_A12[] = {"Ultrasonic Flaw Report"};//22

const u_short _Report_A2[] =
{
    CH(2876),CH(4973),CH(2105),CH(4627),
    CH(4615),CH(4548),CH(2105),CH(4627),
    CH(2504),CH(2894),CH(3591),CH(1938),
    CH(2504),CH(2894),CH(1764),CH(2637),
    CH(2504),CH(2894),CH(2570),CH(2481),
    CH(1836),CH(3347),CH(3738),CH(2637)
};
/*	检验单位
	委托单位
	工件名称
	工件编号
	工件规格
	材料牌号
*/
const u_char _Report_A22[] = {"\
Duty DEP. \
          \
Commission\
DEP.      \
Workpiece \
Name      \
Workpiece \
NO.       \
Workpiece \
Specs     \
Material  \
Kind      "
                             };//10

const u_short _Report_A3[] =
{
    CH(2158),CH( 100),CH( 100),CH(5019),
    CH(2518),CH( 100),CH( 100),CH(5019)
};
/*第   页共   页*/
const u_char _Report_A32[] = {"Current Page    Total Page    "};//30

const u_short _Report_A4[] =
{
    CH(2839),CH(3428),CH(1764),CH(2637),
    CH(2876),CH(1866),CH(1831),CH(4627),
    CH(2876),CH(1866),CH(1740),CH(3293),
    CH(2876),CH(1866),CH(4217),CH(2790),
    CH(4973),CH(4253),CH(1774),CH(5528),
    CH(2647),CH(2481),CH(2822),CH(1780)
};
/*	记录编号
	检测部位
	检测比例
	检测时机
	验收标准
	合格级别
*/

const u_char _Report_A42[] = {"\
Report  \
NO.     \
Detect  \
Part    \
Detect  \
Ratio   \
Detect  \
Time    \
Standard\
        \
Grade   \
        "
                             };//8
const u_short _Report_B1[] =
{
    CH(1777),CH(3570),CH(5520),CH(3186),
    CH(4040),CH(2006),CH(3277),CH( 100),
    CH(8178),CH(2647),CH(2833),CH( 100),
    CH(5039),CH(3887),CH(4845),CH(2637),
    CH(4429),CH(4523),CH(1764),CH(2637),
    CH(4429),CH(4523),CH(3821),CH(3442),
    CH(5359),CH(4168),CH(2939),CH( 100),
    CH(3373),CH(3584),CH(2240),CH( 100)
};
/*	表面状况
	热处理
	耦合剂
	仪器型号
	探头编号
	探头频率
	折射角
	灵敏度
*/

const u_char _Report_B12[] = {"\
Surface    \
Status     \
Heat       \
Treatment  \
Coupling   \
           \
Detector   \
NO.        \
Probe      \
Number     \
Probe      \
Frequency  \
Probe      \
Angle      \
Sensitivity\
           "
                             };//11
const u_short _Report_B2[] =
{
    CH(2208),CH(4627),CH(2329),CH(4229),
    CH(4429),CH(4523),CH(3264),CH(4845),
    CH(4429),CH(4523),CH(3916),CH(4956)
};
/*	定位方式
	探头类型
	探头前沿
*/
const u_char _Report_B22[] = {"\
Orientation\
Probe type \
Front size "
                             };//11

const u_short _Report_B3[] =
{
    CH(4017),CH(4761),CH(4627),CH(5435),
    CH(2816),CH(4429),CH(1866),CH(2965),CH(2591)
};
/*缺陷位置及探测结果*/
const u_char _Report_B32[] = {"Detect position  "};

const u_short _Report_B4[] =
{
    CH(4189),CH( 100),CH(1944),
    CH(4314),CH( 100),CH(3829),
    CH(2025),CH( 100),CH(5417),
    CH(2117),CH( 100),CH(3331),
    CH(1904),CH( 100),CH(2240),
    CH(2463),CH( 100),CH(2240)
};
/*
	声程
	水平
	垂直
	当量
	长度
	高度
*/
const u_short _Report_B4A1[] =
{
    'A',CH(2208),CH(4627),
    'A',CH(2117),CH(3331),
    'B',CH(2208),CH(4627),
    'B',CH(2117),CH(3331),
    CH(1904),CH( 100),CH(2240),
    CH(2463),CH( 100),CH(2240)
};
/*
	声程
	水平
	垂直
	当量
	长度
	高度
*/
const u_char _Report_B42[] = {"\
S range   \
X range   \
Y range   \
Equivalent\
Length    \
High      "
                             };//10
const u_char _Report_B43[] = {"\
A range   \
A Equiv   \
B range   \
B Equiv   \
Length    \
High      "
                             };//10
const u_short _Report_B5[] =
{
    CH(2876),CH(4973),CH(1831),CH(4627),CH(2816),
    CH(4017),CH(4761),CH(4627),CH(5435),CH(4230),CH(5066),CH(4528),CH( 326)
};
/*检验部位及缺陷位置示意图:*/
const u_char _Report_B52[] = {"Detect part & Flaw position picture:"};

const u_short _Report_B6[] =
{
    CH(4017),CH(4761),CH(1808),CH(4846),
    CH(2816),CH(3990),CH(4763),CH(4528),CH( 326)
};
/*缺陷波形及曲线图: */
const u_char _Report_B62[] = {"Flaw echo & Curves lines"};//24

const u_short _Report_B7[] = {CH(3832),CH(2208),CH(2822),CH(1780)};
/*评定级别*/
const u_char _Report_B72[] = {"AssessGrade "};//12
const u_short _Report_C1[] = {CH(2876),CH(4973),CH(2965),CH(3459)};
/*检验结论*/
const u_char _Report_C12[] = {"Check Result"};

const u_short _Report_C2[] =
{
    CH(2876),CH(4973),CH(5363),CH( 100),
    CH( 308),CH(5542),CH(2481),CH( 309)
};
/*	检验者
	（资格）
*/
const u_char _Report_C22[] = {"Detector"};

const u_short _Report_C3[] = {CH(4053),CH(3858)};
/*日期*/
const u_char _Report_C32[] = {"Date"};

const u_short _Report_C4[] =
{
    CH(4183),CH(2643),CH(5363),CH( 100),
    CH( 308),CH(5542),CH(2481),CH( 309)
};
/*	审核者
	（资格）
*/
const u_char _Report_C42[] = {"Principal"};

u_int TestPrinterReady()	//检测打印机是否准备好，如没有出现提示，共3次，返回1为准备好
{
    int i;
    int retvalue = 0;

    for(i=0; i< 3; i++)
    {
        /*
        if( IsPrinterReady() == 0)
        	DisplayPrompt(0);		//提示打印机未准备好
        else
        {
        	retvalue = 1;
        	break;
        }
        */
    }
    return retvalue;
}

const u_char Init_LQ[] = {7,		//ESC @ CR LF
                          0x1b,0x40,0x0d,0x0a,
                          27,85,1
                         };		//初始化加回车换行
const u_char Init_HP[] = {18,
                          0x1b,69,13 ,10,13,10,	//初始化，两个回车换行
                          0x1b,42,112,48,78,	//Print mode (Graphics),Graphic default (no break),Ec*p0N,00x1b 042 112 048 078
                          0x1b,42,116,49,48,48,82
                         };	//以上设置成150 Dots Per inch,Ec * t150R
const u_char Init_EPC61[] = {16,
                             0x1b,0x40,10,10,
                             0x1b,0x28,0x47,1,0,1,	//ESC ( G nL nH m, Select graphics mode
                             0x1b,0x28,0x55,1,0,20
                            };	//	;20 * 1/3600 = 180dpi, ESC ( U nL nH m, Set unit(Set the number of 1/3600 inch units per grograming value)
//		0x1b,0x28,0x4b,1,0,0,1};	//0/2:color mode,1:monochrome mode	;ESC ( K nL nH m n,Monochrome Mode/Color Mode Selection
const u_char Init_TP[] = {2,		//ESC @ CR LF
                          0x1b,0x40
                         };		//初始化加回车换行

void PrinterInitialize(void)		//打印机初始化
{
    int i;

    switch ( MGetPrinterMode() )	/*返回值为当前打印机类型1\2\3\4=LQ\HP\EPSON C/P*/
    {
    case 0:
        for(i=1; i<= Init_LQ[0]; i++)
            mSendByteToPrinter(Init_LQ[i]) ;
        break;
    case 1:
        for(i=1; i<= Init_HP[0]; i++)
            mSendByteToPrinter(Init_HP[i]) ;
        break;
    case 2:
        for(i=1; i<= Init_EPC61[0]; i++)
            mSendByteToPrinter(Init_EPC61[i]) ;
        break;
    case 3:
        for(i=1; i<= Init_TP[0]; i++) //TP-UP
            mSendByteToPrinter(Init_TP[i]) ;
        break;
    }
}

void PrintEnd(void)	//打印结束
{
    int i;
    switch ( MGetPrinterMode() )	/*返回值为当前打印机类型1\2\3\4=LQ\HP\EPSON C/P*/
    {
    case 0:
    case 1:
    case 2:
        mSendByteToPrinter(12) ;		//打印机出纸
        PrinterInitialize();
        break;
    case 3:
        for (i=0; i<=6; i++)
            //	PrinterFormFeed();
            mSendByteToPrinter(13) ;
        //	PrinterInitialize();
        break;
    }
}

void PrinterFormFeed()		//打印机进纸
{
    switch ( MGetPrinterMode() )	/*返回值为当前打印机类型1\2\3\4=LQ\HP\EPSON C/P*/
    {
    case 0:
        mSendByteToPrinter(0x1b) ;		//传送 ESE(0x1b) 到打印机
        mSendByteToPrinter(0x4a) ;		//J   打印并走纸 n 点行
        mSendByteToPrinter(24) ;		//以上产生进纸24/180英寸
//mSendByteToPrinter(120) ;		//以上产生进纸24/180英寸
        mSendByteToPrinter(13) ;		//打印机回车
        break;
    case 1:
        break;
    case 2:
        break;
    case 3:
        mSendByteToPrinter(0x1b) ;		//传送 ESE(0x1b) 到打印机
        mSendByteToPrinter(0x4a) ;		//J   打印并走纸 n 点行
        mSendByteToPrinter(24) ;		//以上产生进纸24/180英寸
//mSendByteToPrinter(120) ;		//以上产生进纸24/180英寸
//		mSendByteToPrinter(13) ;		//打印机回车
        break;
    }
}

void SetPrnPosi_AbHori_EpC61(u_char nl,u_char nh)
//Set absolute horizontial print positon,Epson C61 ,	ESC $ nL nH;
{
    mSendByteToPrinter(0x1b) ;		//传送 ESE(0x1b) 到打印机
    mSendByteToPrinter(0x24) ;		//
    mSendByteToPrinter(nl) ;		//
    mSendByteToPrinter(nh) ;		//
}


void SetPrnPosi_ReVert_EpC61(u_char ml,u_char mh)
//设置垂直相对打印位置，  ESC ( v nl nh mL mH,
{
    mSendByteToPrinter(0x1b) ;		//传送 ESE(0x1b) 到打印机
    mSendByteToPrinter(0x28) ;		//'('
    mSendByteToPrinter(0x76) ;		//'v'
    mSendByteToPrinter(2) ;		//
    mSendByteToPrinter(0) ;		//
    mSendByteToPrinter(ml) ;		//
    mSendByteToPrinter(mh) ;		//
}

void SetPrn_Raster_EpC61(u_short PrintHoriBytes)	//Para:Print_HoriBytes,待打印横向字节数，屏拷40报告80
//ESC . c v h m nl nh d1...dk(d1...dk在下段输出）
{
    mSendByteToPrinter(0x1b) ;		//传送 ESE(0x1b) 到打印机
    mSendByteToPrinter(0x2e) ;		//'.'
    mSendByteToPrinter(0) ;		//c
    mSendByteToPrinter(20) ;		//v
    mSendByteToPrinter(10) ;		//h
    mSendByteToPrinter(24) ;		//m
    mSendByteToPrinter( (8*4*PrintHoriBytes) % 256 ) ;		//8*4 水平四倍密度
    mSendByteToPrinter( (8*4*PrintHoriBytes) / 256 ) ;		//nh
}

void LQ12To24(u_int xpos,u_int ypos,u_char PrnBuff[])		//针打垂直12针变为24针
{
    u_char Ax,Bx,Cx;
    int i,j,k;

    for(i=0; i<8; i++)
    {
        //水平每行8个点
        for(j=0; j<3; j++)
        {
            //垂直12个点，分成3组，每组4个扩展为8个即一个字节
            Cx = 0;
            for(k=0; k<4; k++)
            {
                Ax = MGetPixels(xpos+i, ypos + 4*j+k,1 );
                if( Ax > 0)Ax = 1;
//				Ax = ( Ax >> 7);
                Bx = (Ax << 1);
                Cx = ( Cx << 2);
                Cx += ( Ax + Bx );	//一个点扩展为两个，垂直方向两倍密度
            }//垂直4个点为一个字节
            PrnBuff[3*i+j] = Cx;
        }
    }
}


void HPTP(u_int xpos,u_int ypos,u_char PrnBuff[])		//热敏打印机 针打垂直12针变为24针
{
    u_char Ax,Cx;
    int i,j,k;

    for(i=0; i<8; i++)
    {
        //水平每行8个点
        for(j=0; j<3; j++)
        {
            //垂直24个点，分成3组，每组8个即一个字节
            Cx = 0;
            for(k=0; k<8; k++)
            {
                Ax = MGetPixels(xpos+i, ypos + 8*j+k,1 );
//				Ax = ( Ax >> 7);
                Cx = ( Cx << 1);
                Cx += ( Ax  );
            }//垂直8个点为一个字节
            PrnBuff[3*i+j] = Cx;
        }
    }
}
u_short Byte2Word(u_char Byte)		//一个字节变为一个字
{
    u_short Ax = Byte,Bx = 6/*d2d1=11*/,Dx = 1;
    int i;

    Dx = (Dx & Ax);		//保留D0
    for(i=0; i<8; i++)
    {
        Ax = ( Ax << 1);		//D0成双
        Dx = Dx + ( Bx & Ax);	//取D1D2/d3d4/.../d15d16
        Bx = ( Bx << 2 );		//d2d1->d4d3->...->d16d15
    }
    return Dx;
}

u_int Byte2DWord(u_char Byte)		//一个字节变为一个双字8-32
{
    u_int Ax;
    Ax = Byte2Word( Byte2Word(Byte)/256 ) * 256 * 256 + Byte2Word( Byte2Word(Byte)%256 ) ;

    return Ax;
}


void ScrcpyInit(u_short PrintHoriBytes)
{

    PrinterFormFeed();		//打印机进纸(一行）
    switch ( MGetPrinterMode() )	/*返回值为当前打印机类型1\2\3\4=LQ\HP\EPSON C/P*/
    {
    case 0:
        // ESC * m n1 n2 data, 设置图形格式,38式(90DPI * 180DPI 24针)
        //屏拷时40字节共320点，报告时80字节共640点
//		SetPrnPosi_AbHori_EpC61(16,0*256);

//		SetPrnPosi_ReVert_EpC61(24,0*256);//设置垂直相对打印位置，Para:Al=mL,Ah=mH

        mSendByteToPrinter(0x1b) ;		//传送 ESE(0x1b) 到打印机
        mSendByteToPrinter(0x2a) ;		//'*'42
        mSendByteToPrinter(38) ;		//m38
        mSendByteToPrinter( (8*PrintHoriBytes)%256 ) ;		//
        mSendByteToPrinter( (8*PrintHoriBytes)/256 ) ;		//
        break;
    case 1:
        //Start Raster graphics,
        //At left most position,Ec*r0A,00x1b 042 114 048 065

        mSendByteToPrinter(0x1b) ;		//传送 ESE(0x1b) 到打印机
        mSendByteToPrinter(42) ;		//'*'
        mSendByteToPrinter(114) ;		//
        mSendByteToPrinter(48) ;		//
        mSendByteToPrinter(65) ;		//

        break;
    case 2:
        //nL=16,nH=0	设置Epson C61 水平绝对打印位置Para:Ah=nH,Al=nl

        SetPrnPosi_AbHori_EpC61(160,0*256);

        SetPrnPosi_ReVert_EpC61(24,0*256);//设置垂直相对打印位置，Para:Al=mL,Ah=mH

        SetPrn_Raster_EpC61(PrintHoriBytes);	//Para:PrintHoriBytes,待打印横向字节数，屏拷40报告80
        break;
    case 3://TP-UP 热敏
        mSendByteToPrinter(0x1b) ;		//传送 ESE(0x1b) 到打印机
        mSendByteToPrinter(0x2a) ;		//'*'42
        mSendByteToPrinter(33) ;		//m33  点图方式 24点双密度
        mSendByteToPrinter( (8*PrintHoriBytes)%256) ;		//n1
        mSendByteToPrinter( (8*PrintHoriBytes)/256) ;		//n2
        break;
    case 4:
        //nL=16,nH=0	设置Epson C61 水平绝对打印位置Para:Ah=nH,Al=nl

        SetPrnPosi_AbHori_EpC61(16,0*256);

        SetPrnPosi_ReVert_EpC61(24,0*256);//设置垂直相对打印位置，Para:Al=mL,Ah=mH

        mSendByteToPrinter(0x1b) ;		//传送 ESE(0x1b) 到打印机
        mSendByteToPrinter(0x2e) ;		//'.'
        mSendByteToPrinter(0) ;		//c
        mSendByteToPrinter(30) ;		//v
        mSendByteToPrinter(10) ;		//h
        mSendByteToPrinter(15) ;		//m
        mSendByteToPrinter( (8*4*PrintHoriBytes) % 256 ) ;		//8*4 水平四倍密度
        mSendByteToPrinter( (8*4*PrintHoriBytes) / 256 ) ;		//nh
        break;
    }
}

void ScrcpyInvert(u_int xpos,u_int ypos,u_short PrintHoriBytes)
{
    int i,j,k;
    u_short Ax,Bx;
    u_char PrnBuff[24];

    switch ( MGetPrinterMode() )	/*返回值为当前打印机类型1\2\3\4=LQ\HP\EPSON C/P*/
    {
    case 0:
        for(i = 0; i < PrintHoriBytes; i++)
        {
            LQ12To24(xpos,ypos,PrnBuff);
            for(j=0; j<24; j++)mSendByteToPrinter( PrnBuff[j] );
            xpos += 8;
        }
        break;
    case 1:
        //Start Raster graphics,
        //At left most position,Ec*r0A,00x1b 042 114 048 065
        for(i = 0 ; i < 12 ; i++)
        {
            mSendByteToPrinter(0x1b) ;		//传送 ESE(0x1b) 到打印机
            mSendByteToPrinter(42) ;		//'*'
            mSendByteToPrinter(98) ;		//
            mSendByteToPrinter( (PrintHoriBytes/10)+48) ;		//80->30H=8+48
            mSendByteToPrinter( (PrintHoriBytes%10)+48) ;		//80->30H=0+48
            mSendByteToPrinter(87) ;		//
            //Transfer Raster graphics,Number of byes,Ec*b#W[data],00x1b 042 098 # 087 [data]
            //#=40 or 80
            for(j = 0; j < PrintHoriBytes ; j++ )
            {
                mSendByteToPrinter( MGetPixels( xpos + 8 * j, ypos, 8)%256 ) ;
            }
            ypos++;
        }
        mSendByteToPrinter(0x1b) ;		//传送 ESE(0x1b) 到打印机
        mSendByteToPrinter(42) ;		//'*'
        mSendByteToPrinter(114) ;		//
        mSendByteToPrinter(67) ;		//
        //End Raster graphics,End Graphics,Ec*rbC, 00x1b 042 114 098 067
        //HP Deskjet Pcl:End Graphics,Ec*rC

        break;
    case 2:
        for(i = 0; i < 12; i++)
        {
            for(j = 0; j < 2; j++ )
            {
                for(k = 0; k < PrintHoriBytes; k++)
                {
                    Ax = Byte2Word( MGetPixels( xpos + 8 * k, ypos, 8)%256 ) ;
                    Bx = Byte2Word( Ax%256 );
                    Ax = Byte2Word( Ax/256 );
                    mSendByteToPrinter( Ax / 256 ) ;		//
                    mSendByteToPrinter( Ax % 256 ) ;		//
                    mSendByteToPrinter( Bx / 256 ) ;		//
                    mSendByteToPrinter( Bx % 256 ) ;		//
                }
//				ExpendTime(5);
            }
            ypos++;
        }
        break;
    case 3://TP-UP
        for(i = 0; i < PrintHoriBytes; i++)
        {
            HPTP(xpos,ypos,PrnBuff);
            for(j=0; j<24; j++)mSendByteToPrinter( PrnBuff[j] );
            xpos += 8;
        }
        break;
    case 4:
        for(i = 0; i < 15; i++)
        {
            for(j = 0; j < 3; j++ )
            {
                for(k = 0; k < PrintHoriBytes; k++)
                {
                    Ax = Byte2Word( MGetPixels( xpos + 8 * k, ypos, 8)%256 ) ;
                    Bx = Byte2Word( Ax%256 );
                    Ax = Byte2Word( Ax/256 );
                    mSendByteToPrinter( Ax / 256 ) ;		//
                    mSendByteToPrinter( Ax % 256 ) ;		//
                    mSendByteToPrinter( Bx / 256 ) ;		//
                    mSendByteToPrinter( Bx % 256 ) ;		//
                }
            }
            ypos++;
        }
        break;
    }
}

u_int ScreenCopy(void)		//屏拷
{
    u_short PrintHoriBytes = 39;	//每行40个字节
    u_int xpos = 0,ypos = 0;
    int i,j;

    if( TestPrinterReady() == 0) return 0;

    PrinterInitialize();		//打印机初始化

    switch ( MGetPrinterMode() )	//返回值为当前打印机类型1\2\3\4=LQ\HP\EPSON C/P
    {
    case 0:
    case 1:
    case 2:
        for(i = 0; i< 240/12; i++)
        {
            ScrcpyInit(PrintHoriBytes);
            ScrcpyInvert(xpos,ypos,PrintHoriBytes);
            ypos += 12;
        }
        break;
    case 3:
        for(i = 0; i< 240/24; i++)
        {
            ScrcpyInit(PrintHoriBytes+5);
            for(j=0; j<24*5; j++)
            {
                //PrnBuff[i] = 0;
                mSendByteToPrinter( 0 );
            }
            ScrcpyInvert(xpos,ypos,PrintHoriBytes);
            ypos += 24;
        }
        break;
    }
    PrintEnd();

    return 1;
}
u_int ScreenCopy2(int vdots,int hbytes)		//屏拷
{
    //每行40个字节
    u_int xpos = 0,ypos = 0;
    int i,j;

    if( TestPrinterReady() == 0) return 0;

    PrinterInitialize();		//打印机初始化

    switch ( MGetPrinterMode() )	//返回值为当前打印机类型1\2\3\4=LQ\HP\EPSON C/P
    {
    case 0:
    case 1:
    case 2:
        for(i = 0; i< vdots/12; i++)
        {
            ScrcpyInit(hbytes);
            ScrcpyInvert(xpos,ypos,hbytes);
            ypos += 12;
        }
        break;
    case 3:
        for(i = 0; i< vdots/24; i++)
        {
            ScrcpyInit(hbytes+5);
            for(j=0; j<24*5; j++)
            {
                //PrnBuff[i] = 0;
                mSendByteToPrinter( 0 );
            }
            ScrcpyInvert(xpos,ypos,hbytes);
            ypos += 24;
        }
        break;
    }
    PrintEnd();

    return 1;
}

void ReportPrint(void)		//报告打印
{
    u_short PrintHoriBytes = 64;	//每行40个字节
    u_int xpos = 0,ypos = 0;
    int i;

    switch ( MGetPrinterMode() )	//返回值为当前打印机类型1\2\3\4=LQ\HP\EPSON C/P
    {
    case 0:
    case 1:
    case 2:
        PrintHoriBytes = 64;
        for(i = 0; i< 240/12; i++)
        {
            ScrcpyInit(PrintHoriBytes);
            ScrcpyInvert(xpos,ypos,PrintHoriBytes);
            ypos += 12;
        }
        break;

    case 3:
        PrintHoriBytes = 48;
        for(i = 0; i< 240/24; i++)
        {
            ScrcpyInit(PrintHoriBytes);
            ScrcpyInvert(xpos,ypos,PrintHoriBytes);
            ypos += 24;
        }
        break;
    }
}

int DaclineStore(DAC_LINE* Dacline)
{

    int offset = C_OFF_DAC + 25 * C_SIZE_INT;

    MCopyProtectedMemory((void*)offset, Dacline, 2*C_COORWIDTH*C_SIZE_SHORT, PM_COPY_WRITE);

    return 1;

}
int DaclineRestore(DAC_LINE* Dacline)
{
    int offset = C_OFF_DAC + 25 * C_SIZE_INT;

    MCopyProtectedMemory(Dacline, (void*)offset, 2*C_COORWIDTH*C_SIZE_SHORT, PM_COPY_READ);

    return 1;
}

void DispEchoPara(void)
{
    int para1,para2,para3,para4;
    int xpos,ypos,row_height = 24;
    int mode1,mode2;
    int offset;

    u_int testrange[3];		//待测声程值
    u_char pzText[] = "   x         ";

    pzText[0] = PHI;

    offset = C_OFF_TEMP + 30;
    MCopyProtectedMemory( testrange, (void*)offset, 12, PM_COPY_READ);

    mode1 = MGetBGateMode();
    mode2 = MGetEquivalentMode();	//mode1=0B门失波1进波，mode2<0波高.>0dB数

    offset = C_OFF_PEAK;
    MCopyProtectedMemory( &GatePeak, (void*)offset, C_LEN_PEAK, PM_COPY_READ);//门参数

    para1 = ChannelPara.wavepara[0] ;	//距离
    para2 = ChannelPara.wavepara[1] ;	//距离
    para3 = ChannelPara.wavepara[2] ;	//距离
    para4 = ChannelPara.wavepara[3] ;	//距离

    xpos = C_HORIDOT_SCREEN + C_ECHAR_HDOT * 9 + 4;

    if( mode2 != 1)
    {
        pzText[0] = ' ';
        pzText[1] = ' ';
        pzText[2] = ' ';
        pzText[4] = 'L';
        pzText[5] = ' ';
    }
    switch (mode2)
    {
    case 2:
        pzText[3] = 'R';
        break;
    case 3:
        pzText[3] = 'S';
        break;
    case 4:
        pzText[3] = 'E';
        break;
    case 5:
        pzText[3] = '4';
        break;
    case 6:
        pzText[3] = '5';
        break;
    case 7:
        pzText[3] = '6';
        break;
    case 10:
        pzText[0] = ' ';
        pzText[1] = ' ';
        pzText[2] = ' ';
        pzText[3] = ' ';
        pzText[4] = ' ';
        pzText[6] = PHI;
        if( MGetUnitType() == 0)
        {
			pzText[7] = '2';//(int)( powf( 10,( MGetLineGain(2) + 120 ) / 400.0)  + 0.05)+0x30;
        }
        break;
    case 11:
        pzText[0] = PHI;
        pzText[1] = ' ';
        pzText[2] = ' ';
        pzText[3] = ' ';
        pzText[4] = ' ';
        pzText[6] = PHI;
        if( MGetUnitType() == 0)
        {
			pzText[7] = (int)( powf( 10,( MGetLineGain(2) + 120 ) / 400.0)  + 0.05)+0x30;
        }
        break;
    }

    if(mode1 == 0)
    {
        //B门失波
        ypos = row_height + 8 ;
        WriteLongness(xpos ,ypos,para1,5,1);
        //Write_Number(xpos,ypos,para1,6,1,0);//声程
        //EMenuOut(xpos + 6*8,ypos, "mm",2,1,12);
        ypos = row_height*2 + 8 ;
        WriteLongness(xpos ,ypos,para2,5,1);
        //Write_Number(xpos,ypos,para2,6,1,0);//水平
        //EMenuOut(xpos + 6*8,ypos, "mm",2,1,12);
        ypos = row_height*3 + 8 ;
        WriteLongness(xpos ,ypos,para3,5,1);
        //Write_Number(xpos,ypos,para3,6,1,0);//垂直
        //EMenuOut(xpos + 6*8,ypos, "mm",2,1,12);

        ypos = row_height*4 + 8 ;
        if( MGetEchoMode() == C_RF_WAVE)Write_Number(xpos,ypos,para4,4,1,1);
        else if(mode2 > 0)
        {

            ypos = row_height*4 + 4 ;
            EMenuOut(xpos,ypos,pzText,8,1,8);

            if (mode2 == 1)
            {
                if( MGetUnitType() > 0)
                {
                    //Write_Number(xpos + 1*8,ypos,testrange[1]/10,2,0,0);
                    WriteLongness(xpos + 1*8,ypos,(UINT)(Dac.diameter*100/25.4+0.5),5,0);
                }
                else
                {
///					Write_Number(xpos + 1*8,ypos,testrange[1]/10,2,0,0);
///					Write_Number(xpos + 4*8,ypos,testrange[2]/10,2,0,0);
                    Write_Number(xpos + 1*8,ypos, Dac.diameter/10,2,0,0);
                    Write_Number(xpos + 4*8,ypos, Dac.length/10,2,0,0);
                }
            }
            else if (mode2 == 11)
            {
                //avg孔径
                //Write_Number(xpos + 1*8,ypos,(int)( 10*powf( 10,( MGetLineGain(2) + 120 ) / 400.0)  + 0.5)/10, 3,1,0);//显示dB
///				Write_Number(xpos + 1*8,ypos,(int)( 10*powf( 10,( para4 + 120 ) / 400.0)  + 0.5), 3,1,0);
				Write_Number(xpos + 1*8,ypos,(int)( 10*powf( 10,( para4 + MGetLineGain(2) + 120 ) / 400.0)  + 0.5), 3,1,0);
            }


            ypos = row_height*4 + 14 ;
            Write_Number(xpos +  3*8,ypos,para4,5,1,1);//显示当量值
            EMenuOut(xpos + 8*8,ypos, "dB",2,1,12);
        }
        else
        {
            Write_Number(xpos,ypos,para4,4,1,0);
            EMenuOut(xpos + 5*8,ypos, "%",1,1,12);
        }
    }
    else
    {
        //B门进波

        if( mode2 >= 10)
        {
            pzText[4] = PHI;
            if(mode2 == 10 )pzText[5] = '2';
			else pzText[5] = (int)( powf( 10,( MGetLineGain(2) + 120 ) / 400.0)  + 0.05)+0x30;
            pzText[6] = ' ';
            pzText[7] = ' ';
        }

        if(mode2 > 0 )
        {
            if(mode2 == 1)
            {
                pzText[3] = 'D';
                pzText[4] = 'A';
                pzText[5] = 'C';
            }
        }
        else
        {
            //AVG
            pzText[3] = ARROW4;
            pzText[4] = ' ';
            pzText[5] = ' ';
            pzText[6] = ' ';
            pzText[7] = ' ';
            pzText[8] = '%';
        }


        ypos = row_height + 8 ;
        WriteLongness(xpos,ypos,para1,6,1);
        ypos = row_height*2 + 8 ;

        EMenuOut(xpos,ypos, pzText+3,8,1,8);

        if( MGetEchoMode() == C_RF_WAVE)
        {
            Write_Number(xpos+8,ypos,para2,4,1,1);
        }
        else if(mode2 > 0)
        {
            Write_Number(xpos+3*8,ypos,para2,4,1,1);
            EMenuOut(xpos + 7*8,ypos, "dB",2,1,12);
        }
        else
        {
            Write_Number(xpos,ypos,para2,5,1,1);
        }
        ypos = row_height*3 + 8 ;
        WriteLongness(xpos,ypos,para3,6,1);
        //Write_Number(xpos,ypos,para3,6,1,0);
        //EMenuOut(xpos + 6*8,ypos, "mm",2,1,12);
        ypos = row_height*4 + 8 ;
        EMenuOut(xpos,ypos, pzText+3,8,1,8);
        if( MGetEchoMode() == C_RF_WAVE)
        {
            Write_Number(xpos+8,ypos,para4,4,1,1);
        }
        else if(mode2 > 0)
        {
            Write_Number(xpos+3*8,ypos,para4,4,1,1);
            EMenuOut(xpos + 7*8,ypos, "dB",2,1,12);
        }
        else
        {
            Write_Number(xpos+8,ypos,para4,5,1,1);
        }
    }
    if( MGetDepth() != 0)
    {
        ypos = row_height*6+4;
        Write_Number(xpos,ypos,MGetDepth(),5,1,1);
        EMenuOut(xpos + 5*8,ypos, "mm",2,1,12);
    }
}


const u_short _PReport_title[]=
{
    //超声波检测报告
    CH(1912),CH(4189),CH(1808),CH(2876),CH(1866),CH(1708),CH(2470)
};

const u_short _PReport_C1[] =
{
    CH(2158),CH( 100),CH( 100),CH(5019),
    CH(2518),CH( 100),CH( 100),CH(5019)
};
/*第   页共   页*/


const u_short _PReport_C2[]=
{

    //仪器型号:	//记录编号:
    //探头参数:
    //探头前沿:
    //灵敏度:
    CH(5039),CH(3887),CH(4845),CH(2637),CH( 326),//仪器型号
//CH(4429),CH(4523),CH(1764),CH(2637),CH( 326),//探头编号
    CH(4429),CH(4523),CH(1846),CH(4293),CH( 326),//探头参数:
    CH(4429),CH(4523),CH(3916),CH(4956),CH( 326),//探头前沿:
    CH(3373),CH(3584),CH(2240),CH( 100),CH( 326),//灵敏度
    CH(2839),CH(3428),CH(1764),CH(2637),CH( 326),//记录编号

};


const u_short _PReport_C3[]=
{
    //位    置:4627 5435
    //当    量:
    CH(4627),CH( 100),CH( 100),CH(5435),CH( 326),
    CH(2117),CH( 100),CH( 100),CH(3331),CH( 326)
};

const u_short _PReport_C4[] =
{
    CH(4017),CH(4761),CH(1808),CH(4846),
    CH(2816),CH(3990),CH(4763),CH(4528),CH( 326)
};
/*缺陷波形及曲线图: */

const u_short _PReport_C5[]=
{
    //检验结论
    //审 核 者
    //检 验 者
    CH(2876),CH(4973),CH(2965),CH(3459),CH( 326),
    CH(2876),CH(4973),CH(5363),CH( 100),CH( 326),
    CH(4183),CH(2643),CH(5363),CH( 100),CH( 326),
};

extern	const char _version1[];


void DispEchoParat(void)
{
    int para1,para2,para3,para4;
    int xpos,ypos,row_height = 24;
    int mode1,mode2;
    int offset;

    u_int testrange[3];		//待测声程值
    u_char pzText[] = "   x         ";

    pzText[0] = PHI;

    offset = C_OFF_TEMP + 30;
    MCopyProtectedMemory( testrange, (void*)offset, 12, PM_COPY_READ);

    mode1 = MGetBGateMode();
    mode2 = MGetEquivalentMode();	//mode1=0B门失波1进波，mode2<0波高.>0dB数

    offset = C_OFF_PEAK;
    MCopyProtectedMemory( &GatePeak, (void*)offset, C_LEN_PEAK, PM_COPY_READ);//门参数

    para1 = ChannelPara.wavepara[0] ;	//距离
    para2 = ChannelPara.wavepara[1] ;	//距离
    para3 = ChannelPara.wavepara[2] ;	//距离
    para4 = ChannelPara.wavepara[3] ;	//距离

    if( mode2 != 1)
    {
        pzText[0] = ' ';
        pzText[1] = ' ';
        pzText[2] = ' ';
        pzText[4] = 'L';
        pzText[5] = ' ';
    }
    switch (mode2)
    {
    case 2:
        pzText[3] = 'R';
        break;
    case 3:
        pzText[3] = 'S';
        break;
    case 4:
        pzText[3] = 'E';
        break;
    case 5:
        pzText[3] = '4';
        break;
    case 6:
        pzText[3] = '5';
        break;
    case 7:
        pzText[3] = '6';
        break;
    case 10:
        pzText[0] = ' ';
        pzText[1] = ' ';
        pzText[2] = ' ';
        pzText[3] = ' ';
        pzText[4] = ' ';
        pzText[6] = PHI;
        if( MGetUnitType() == 0)
        {
			pzText[7] = '2';//(int)( powf( 10,( MGetLineGain(2) + 120 ) / 400.0)  + 0.05)+0x30;
        }
        break;
    case 11:
        pzText[0] = PHI;
        pzText[1] = ' ';
        pzText[2] = ' ';
        pzText[3] = ' ';
        pzText[4] = ' ';
        pzText[6] = PHI;
        if( MGetUnitType() == 0)
        {
			pzText[7] = (int)( powf( 10,( MGetLineGain(2) + 120 ) / 400.0)  + 0.05)+0x30;
        }
        break;
    }

    ypos = 32+ row_height * 4+8;  //位置 SXY
    xpos= C_CCHAR_HDOT * 8+ 4;

    if(mode1 == 0)
    {
        //B门失波


        Write_Ascii(xpos,ypos,ARROW1);
        Write_Number(xpos+8,ypos,para1,6,1,0);//声程
//		EMenuOut(xpos + 6*8,ypos, "mm",2,1,12);

        xpos= xpos+6*8+2*8+4;
        Write_Ascii(xpos,ypos,ARROW2);
        Write_Number(xpos+8,ypos,para2,6,1,0);//水平
//		EMenuOut(xpos + 6*8,ypos, "mm",2,1,12);

        xpos= xpos+6*8+2*8+4;
        Write_Ascii(xpos,ypos,ARROW3);
        Write_Number(xpos+8,ypos,para3,6,1,0);//垂直
//		EMenuOut(xpos + 6*8,ypos, "mm",2,1,12);

        ypos = 32 + row_height*5+8;
        xpos = C_CCHAR_HDOT * 8 + 4;

        if( MGetEchoMode() == 3)Write_Number(xpos,ypos,para4,4,1,1);
        else if(mode2 > 0)
        {

            //		ypos = row_height*4 + 4 ;
            EMenuOut(xpos,ypos,pzText,8,1,8);

            if (mode2 == 1)
            {
                if( MGetUnitType() > 0)
                {
                    //Write_Number(xpos + 1*8,ypos,testrange[1]/10,2,0,0);
                    WriteLongness(xpos + 1*8,ypos,(UINT)(Dac.diameter*100/25.4+0.5),5,0);
                }
                else
                {
///					Write_Number(xpos + 1*8,ypos,testrange[1]/10,2,0,0);
///					Write_Number(xpos + 4*8,ypos,testrange[2]/10,2,0,0);
                    Write_Number(xpos + 1*8,ypos, Dac.diameter/10,2,0,0);
                    Write_Number(xpos + 4*8,ypos, Dac.length/10,2,0,0);

                }
            }
            else if (mode2 == 11)
            {
                //avg孔径
                //Write_Number(xpos + 1*8,ypos,(int)( 10*powf( 10,( MGetLineGain(2) + 120 ) / 400.0)  + 0.5)/10, 3,1,0);//显示dB
///				Write_Number(xpos + 1*8,ypos,(int)( 10*powf( 10,( para4 + 120 ) / 400.0)  + 0.5), 3,1,0);
				Write_Number(xpos + 1*8,ypos,(int)( 10*powf( 10,( para4 + MGetLineGain(2) + 120 ) / 400.0)  + 0.5), 3,1,0);
            }


            /*	ypos = row_height*4 + 14 ;
            	Write_Number(xpos +  3*8,ypos,para4,5,1,1);//显示当量值
            	EMenuOut(xpos + 8*8,ypos, "dB",2,1,12);*/

            Write_Number(xpos + 8*8,ypos,para4,5,1,1);//显示当量值
            EMenuOut(xpos + 13*8,ypos, "dB",2,1,12);

        }
        else
        {
            Write_Number(xpos,ypos,para4,4,1,0);
            EMenuOut(xpos + 5*8,ypos, "%",1,1,12);
        }
    }
    else
    {
        //B门进波

        if( mode2 >= 10)
        {
            pzText[4] =PHI;
            if(mode2 == 10 )pzText[5] = '2';
			else pzText[5] = (int)( powf( 10,( MGetLineGain(2) + 120 ) / 400.0)  + 0.05)+0x30;
            pzText[6] = ' ';
            pzText[7] = ' ';
        }

        if(mode2 > 0 )
        {
            if(mode2 == 1)
            {
                pzText[3] = 'D';
                pzText[4] = 'A';
                pzText[5] = 'C';
            }
        }
        else
        {
            //AVG
            pzText[3] = ARROW4;
            pzText[4] = ' ';
            pzText[5] = ' ';
            pzText[6] = ' ';
            pzText[7] = ' ';
            pzText[8] = '%';
        }

        Write_Ascii(xpos,ypos,ARROW1);
        //	Write_Number(xpos+8,ypos,para1,6,1,0);
        WriteLongness(xpos,ypos,para1,6,1);

//		EMenuOut(xpos + 6*8,ypos, "mm",2,1,12);

        xpos =xpos + 6*8+8*2+4;
        Write_Ascii(xpos,ypos,ARROW2);


        EMenuOut(xpos,ypos, pzText+3,8,1,8);

        if( MGetEchoMode() == C_RF_WAVE)
        {
            Write_Number(xpos+8,ypos,para2,4,1,1);
        }
        else if(mode2 > 0)
        {
            Write_Number(xpos+8,ypos,para2,4,1,0);
            //		Write_Number(xpos+3*8,ypos,para2,4,1,1);
            EMenuOut(xpos + 7*8,ypos, "dB",2,1,12);
        }
        else
        {
            Write_Number(xpos+8,ypos,para2,5,1,1);
            //		Write_Number(xpos,ypos,para2,5,1,1);
        }




        //	ypos = row_height*3 + 8 ;
        xpos =xpos + 5*8+8*2+4;
        Write_Ascii(xpos,ypos,ARROW2);

        WriteLongness(xpos,ypos,para3,6,1);
        ypos = row_height*4 + 8 ;

        ypos = 32+ row_height * 5+8;  //位置 SXY
        xpos= C_CCHAR_HDOT * 8 + 4;

        EMenuOut(xpos,ypos, pzText+3,8,1,8);
        if( MGetEchoMode() == C_RF_WAVE)
        {
            Write_Number(xpos,ypos,para4,4,1,1);
            //		Write_Number(xpos+8,ypos,para4,4,1,1);
        }
        else if(mode2 > 0)
        {
            Write_Number(xpos,ypos,para4,4,1,0);
            EMenuOut(xpos + 4*8,ypos, "dB",2,1,12);

            //		Write_Number(xpos+3*8,ypos,para4,4,1,1);
            //		EMenuOut(xpos + 7*8,ypos, "dB",2,1,12);
        }
        else
        {
            Write_Number(xpos,ypos,para4,5,1,1);
//			Write_Number(xpos+8,ypos,para4,5,1,1);
        }
    }
    if( MGetDepth() != 0)
    {
        ypos = 32 + row_height*7+8;
        xpos = C_CCHAR_HDOT * 8 + 4;
        Write_Number(xpos,ypos,MGetDepth(),5,1,1);
        EMenuOut(xpos + 5*8,ypos, "mm",2,1,12);
    }

}

int ReportText2(int mode, int file_no)		//报告文本
{
    int xpos,ypos;
    int	row_height = 24;
    int xpos2,ypos2;
    u_short* cpText;
    //	int i;
    u_char* epText;
    int offset;

    if( TestPrinterReady() == 0) return 0;	//打印机准备好测试
    PrinterInitialize();					//打印机初始化


    REPORT_INFO reportinfo;
    offset = C_OFF_REPORT_INFO;
    MCopyProtectedMemory(&reportinfo, (void*)offset, sizeof(REPORT_INFO), PM_COPY_READ);
    DataRestore(file_no);


//1
    MFclearScreen();
    HoriOffsetScreen = 0;//水平起始位置
//	xpos = 0;
    ypos = 0;

    CMenuOut(C_CCHAR_HDOT*8,24, _PReport_title, 7, 1,row_height);	//超声波检测报告
    CMenuOut(C_CCHAR_HDOT*14,24+row_height, _PReport_C1, 8,1,row_height); //第几页,共几页
    CMenuOut(24+24, 24+row_height*2, _PReport_C4, 9,1,row_height);/*缺陷波形及曲线图: */
//	ReportPrint();
    xpos = 0;//ypos = 24;
    u_short PrintHoriBytes = 48;
    while( ypos <= 96 - 24)
    {
        ScrcpyInit(PrintHoriBytes);
        ScrcpyInvert(xpos,ypos,PrintHoriBytes);
        ypos += 24;
    }

//2  画波形
    MFclearScreen();
    HoriOffsetScreen = 16+8+24;
    MCoorDraw(C_COORHPOSI , C_COORVPOSI, C_COORHEIGHT, C_COORWIDTH);/*画指定位置和大小的波形坐标200,280*/
    MDrawLine(C_COORHPOSI + C_COORWIDTH, C_COORVPOSI, C_SCREEN_WIDTH, C_COORVPOSI,C_CR_BACK);
    MDrawLine(C_COORHPOSI + C_COORWIDTH, C_COORVPOSI+C_COORHEIGHT, C_SCREEN_WIDTH, C_COORVPOSI+C_COORHEIGHT,C_CR_BACK);
    WriteScale();	//标度
    MGateDraw();  //画门

//后加:
    switch (MGetScaleMode())
    {
    case 0:
        EMenuOut(SCREEN_WIDTH-50,C_VERTDOT_SCREEN-7, "(Y)",3,1,12);
        break;
    case 1:
        EMenuOut(SCREEN_WIDTH-50,C_VERTDOT_SCREEN-7, "(X)",3,1,12);
        break;
    case 2:
        EMenuOut(SCREEN_WIDTH-50,C_VERTDOT_SCREEN-7, "(S)",3,1,12);
        break;
    }

    DataRestore(file_no);
    u_char echobuffer[C_LEN_SAMP];
    offset = C_OFF_SAMP;
    MCopyProtectedMemory(echobuffer, (void*)offset, C_LEN_SAMP, PM_COPY_READ);	//波形参数
    DrawWave(echobuffer,0,C_LEN_SAMP);

    if( !MGetSaveMode() || !MGetSaveStatus() )
    {
        offset = C_OFF_DAC;
        MCopyProtectedMemory( &Dac, (void*)offset, C_LEN_DAC, PM_COPY_READ);

        DrawDac(0);		//新画DAC
    }
    HoriOffsetScreen = 0;
//	xpos = C_HORIDOT_SCREEN;
    ypos = 0;
//	MDrawLine(xpos, ypos, xpos, C_VERTDOT_SCREEN+5);
//	MDrawLine(501, ypos, 501, 255);

    xpos = 0;//ypos = 24;
//	u_short PrintHoriBytes = 48;
    while( ypos <= 240 - 24)
    {
        ScrcpyInit(PrintHoriBytes);
        ScrcpyInvert(xpos,ypos,PrintHoriBytes);
        ypos += 24;
    }
//2
    MFclearScreen();
    HoriOffsetScreen = 0;

    xpos =0;
    ypos =32;

    CEMenuOut(xpos +C_CCHAR_HDOT*3,ypos, _PReport_C2, 5, 4, row_height);
    //仪器型号:
    //探头编号:
    //探头参数:
    //灵敏度:
    CEMenuOut(xpos +C_CCHAR_HDOT*11+24,ypos, _PReport_C2+5*4, 5, 1, row_height);

    offset = C_OFF_FILEINFO + C_SIZE_INT;
    MCopyProtectedMemory( &fat, (void*)offset, C_LEN_FAT, PM_COPY_READ);

    if( !MGetSaveMode() || !MGetSaveStatus() )  //记录编号
    {
        if( fat.name[0] > 8 )fat.name[0] = 8;
    }



    xpos2= xpos+C_CCHAR_HDOT * 8 + 4;
    ypos2= ypos+row_height*0;

    CEMenuOut2(xpos2+152,ypos2,fat.name + 1,fat.name[0],1,16);//记录编号

    EMenuOut(xpos2,ypos2+8, _version1,8,1,12);//仪器型号


    //	ypos2 = ypos + row_height * 2+8;   //探头编号
//	EMenuOut(xpos2,ypos2, "we",2,1,12);

    ypos2 = ypos + row_height * 1+8;   //探头参数
    Write_Number(xpos2,ypos2,MGetFrequence(),4,2,0);	//频率
    EMenuOut(xpos2+4*8,ypos2, "MHz",3,1,12);
    if (MGetAngle(0)>=C_MAX_ANGLE)
    {
        TextOut(xpos2+8*8,ypos2-6,1,3,10,(char *)_infinity[MGetLanguage()][0],0);
    }
    else
    {
        Write_Number(xpos2+8*8,ypos2,( MGetAngle(1) + 50)/100,4,2,0);	//K value
    }
    Write_Ascii(xpos2+12*8 ,ypos2,'/');

    Write_Number(xpos2+13*8 ,ypos2 ,MGetAngle(0),4,1,0);	//angle
    Write_Ascii(xpos2+17*8 ,ypos2 ,ANGLE);

//	cpText = (u_short*)_ParaMenu_AB2 + ( (MGetProbeMode()-0)*4+1 );	//类型
//	CEMenuOut(xpos2+19*8 ,ypos2-8, cpText ,3,1,row_height);
    epText = (char *)_ParaMenu_AB2[MGetLanguage()][MGetProbeMode()]+2;
    TextOut(xpos2+19*8 ,ypos2-8,1,8,16,epText,4);

    ypos2 = ypos + row_height * 2+8;
    Write_Number(xpos2,ypos2,MGetForward(),4,1,0);	//探头前沿
    EMenuOut(xpos2+ 4*8,ypos2, "mm",2,1,12);


    ypos2 = ypos + row_height * 3+8;   //灵敏度
    WriteBaseGain(xpos2,ypos2);
    WriteCompGain(xpos2 + 4*8,ypos2);
    WriteSurfGain(xpos2 + 9*8,ypos2);
    EMenuOut(xpos2+ 14*8,ypos2, "dB",2,1,12);

    CMenuOut(xpos +C_CCHAR_HDOT*3,ypos+ row_height * 4, _PReport_C3, 5,2,row_height);//位置 当量

    DataRestore(file_no);
//	DispEchoParatt();
    DispEchoParat();

    CEMenuOut(xpos +C_CCHAR_HDOT* 3,ypos+ row_height * 6, _PReport_C5,5, 3,row_height);
    ReportPrint();
    HoriOffsetScreen = 4;
    PrintEnd();
    MFclearScreen();
    return 1;
}





int ReportText(int mode, int file_no)		//报告文本
{
    int xpos,ypos;
//	int row_char_len = 38,row_number = 6;
    int	row_height = 24;
    int xpos2,ypos2;
    u_short* cpText;
    u_char * epText;
    int i;
    int offset;

    if( TestPrinterReady() == 0) return 0;
    PrinterInitialize();		//打印机初始化

    if( MGetPrinterMode() == 3)
    {
        ReportText2(mode,file_no);
        return 1;
    }

    HoriOffsetScreen = 0;

    REPORT_INFO reportinfo;
    offset = C_OFF_REPORT_INFO;
    MCopyProtectedMemory(&reportinfo, (void*)offset, sizeof(REPORT_INFO), PM_COPY_READ);
    DataRestore(file_no);
//1
    MFclearScreen();
    CMenuOut(20*8,34, _Report_A1, 13, 1,row_height);	//标题
    CMenuOut(46 * 8, 70, _Report_A3, 8,1,row_height);

    xpos = 0;
    ypos = 94;
    for(i = 0; i < 7; i++)
    {
        MDrawLine(xpos + 1, ypos + row_height * i, 501, ypos + row_height * i,C_CR_BACK);
    }

    MDrawLine(xpos, ypos, xpos, C_VERTDOT_SCREEN - 2,C_CR_BACK);
    MDrawLine(xpos + C_ECHAR_HDOT * 9, ypos, xpos + C_ECHAR_HDOT * 9, C_VERTDOT_SCREEN - 2,C_CR_BACK);
    CEMenuOut(xpos + 4,ypos + 4, _Report_A2, 4, 6, row_height);

    xpos2 = xpos + C_ECHAR_HDOT * 9 + 4;
//暂不写有关工件的信息
///	CEMenuOut(xpos2, ypos + 4, reportinfo.TestDep+1, 10, 1, 24);
///	CEMenuOut(xpos2, ypos + row_height + 4, reportinfo.EntrustDep+1, 10, 1, 24);
///	CEMenuOut(xpos2, ypos + row_height * 2 +4, reportinfo.WorkpieceName+1, 10, 1, 24);
///	CEMenuOut(xpos2, ypos + row_height * 4 + 4, reportinfo.WorkpieceSpec+1, 10, 1, 24);
///	CEMenuOut(xpos2, ypos + row_height * 5 + 4, reportinfo.MaterialNumber+1, 10, 1, 24);

    xpos = C_HORIDOT_SCREEN;

    MDrawLine(xpos, ypos, xpos, C_VERTDOT_SCREEN - 2,C_CR_BACK);
    MDrawLine(xpos + C_ECHAR_HDOT * 9, ypos, xpos + C_ECHAR_HDOT * 9, C_VERTDOT_SCREEN - 2,C_CR_BACK);
    MDrawLine(501, ypos, 501, C_VERTDOT_SCREEN - 2,C_CR_BACK);
    CEMenuOut2(xpos + 4,ypos + 4, _Report_A4, 4, 6, row_height);

    xpos2 = xpos + C_ECHAR_HDOT * 9 + 4;
    offset = C_OFF_FILEINFO + C_SIZE_INT;
    MCopyProtectedMemory( &fat, (void*)offset, C_LEN_FAT, PM_COPY_READ);

    if( !MGetSaveMode() || !MGetSaveStatus() )
    {
        if( fat.name[0] > 8 )fat.name[0] = 8;
    }
    //if( fat.name[0] < 0 )fat.name[0] = 0;
    CEMenuOut2(xpos2,ypos + 4,fat.name + 1,fat.name[0],1,16);

    ReportPrint();
//2
    MFclearScreen();
    xpos = 0;
    ypos = 0;
    for(i = 1; i < 9; i++)
    {
        MDrawLine(xpos + 1, ypos + row_height * i, 501, ypos + row_height * i,C_CR_BACK);
    }

    MDrawLine(xpos, ypos, xpos, C_VERTDOT_SCREEN - 1,C_CR_BACK);
    MDrawLine(xpos + C_ECHAR_HDOT * 9, ypos, xpos + C_ECHAR_HDOT * 9, ypos + row_height * 8 - 1,C_CR_BACK);

    MDrawLine(xpos + C_ECHAR_HDOT * 22, ypos + row_height * 3 + 1, xpos + C_ECHAR_HDOT * 22, ypos + row_height * 6 - 1,C_CR_BACK);
    MDrawLine(xpos + C_ECHAR_HDOT * 31, ypos + row_height * 3 + 1, xpos + C_ECHAR_HDOT * 31, ypos + row_height * 6 - 1,C_CR_BACK);

    CEMenuOut(xpos + C_ECHAR_HDOT * 22 + 4, ypos + row_height * 3 + 5, _Report_B2, 4, 3, row_height);
    CEMenuOut(xpos + 4, ypos + 4, _Report_B1, 4, 8, row_height);
    CEMenuOut(xpos + 4, ypos + row_height * 8 + 4, _Report_B6, 9, 1, row_height);

    //左
    xpos2 = xpos + C_ECHAR_HDOT * 9 + 4;
    ypos2 = ypos + row_height * 5 + 8 ;
    Write_Number(xpos2,ypos2,MGetFrequence(),4,2,0);	//频率
    EMenuOut(xpos2+4*8,ypos2, "MHz",3,1,12);

    ypos2 = ypos + row_height * 6 + 8 ;
    if (MGetAngle(0)>=C_MAX_ANGLE)
    {
        TextOut(xpos2,ypos2-6,1,3,10,(char *)_infinity[MGetLanguage()][0],0);
    }
    else
    {
        Write_Number(xpos2,ypos2,( MGetAngle(1) + 50)/100,4,2,0);	//K value
    }
    Write_Ascii(xpos2 + 4 * C_ECHAR_VDOT,ypos2,'/');
    Write_Number(xpos2 + 5 * C_ECHAR_VDOT,ypos2 ,MGetAngle(0),4,1,0);	//angle
    Write_Ascii(xpos2 + 9 * C_ECHAR_VDOT,ypos2 ,130);

    ypos2 = ypos + row_height * 7 + 8 ;
    WriteBaseGain(xpos2,ypos2);
    WriteCompGain(xpos2 + 4*8,ypos2);
    WriteSurfGain(xpos2 + 9*8,ypos2);
    EMenuOut(xpos2+ 14*8,ypos2, "dB",2,1,12);

    xpos2 = xpos + C_ECHAR_HDOT * 31 + 4;
    ypos2 = ypos + row_height * 4 + 4 ;
//	cpText = (u_short*)_ParaMenu_AB2 + ( (MGetProbeMode()-0)*4+1 );	//类型
//	CEMenuOut(xpos2,ypos2 , cpText ,3,1,row_height);
//	epText = (char *)_ParaMenu_AB2[MGetLanguage()][MGetProbeMode()]+2;
    TextOut(xpos2 ,ypos2,1,8,16,(u_char *)_ParaMenu_AB2[MGetLanguage()][MGetProbeMode()]+2,4);

    ypos2 = ypos + row_height * 5 + 8 ;
    WriteLongness(xpos2,ypos2,MGetForward(),5,1);
    //Write_Number(xpos2,ypos2,MGetForward(),4,1,0);	//探头前沿
    //EMenuOut(xpos2+ 4*8,ypos2, "mm",2,1,12);

    ypos2 = ypos + row_height * 3 + 4 ;
//	cpText = (u_short*)_ParaMenu_AB1 + (MGetScaleMode()-0) * 3 + 1;//声程标度
//	CEMenuOut(xpos2,ypos2 , cpText ,2,1,row_height);	/*在指定位置根据每行字符数、行数、行高写菜单*/
    TextOut(xpos2,ypos2 ,1,8,row_height,(u_char*)_ParaMenu_AB1[MGetLanguage()][MGetScaleMode()]+2,4);

    //右
    xpos = C_HORIDOT_SCREEN;
    CMenuOut(xpos + 20, ypos + 4, _Report_B3, 9,1,row_height);

    MDrawLine(xpos, ypos, xpos, C_VERTDOT_SCREEN - 1,C_CR_BACK);
    MDrawLine(xpos + C_ECHAR_HDOT * 9, ypos + row_height + 1, xpos + C_ECHAR_HDOT * 9, ypos + row_height * 8 - 1,C_CR_BACK);
    MDrawLine(501, ypos, 501, C_VERTDOT_SCREEN - 1,C_CR_BACK);

    if( MGetBGateMode() == 0)
    {
        CEMenuOut(xpos + 12, ypos + row_height + 4, _Report_B4, 3, 6, row_height);
    }
    else
    {
        CEMenuOut(xpos + 12, ypos + row_height + 4, _Report_B4A1, 3, 6, row_height);
    }
    CEMenuOut(xpos + 4, ypos + row_height * 7 + 4, _Report_B7, 4, 1, row_height);

    xpos = C_HORIDOT_SCREEN + C_ECHAR_HDOT * 9 + 4;
    ypos = row_height * 5+ 8 ;
    //Write_Number(xpos, ypos, detecthigh, 5, 1, 0);
    //EMenuOut(xpos + 5*8,ypos, "mm",2,1,12);

    DataRestore(file_no);
    DispEchoPara();

    ReportPrint();
//3
    MFclearScreen();
    xpos = 0;
    ypos = 0;
    MDrawLine(xpos, ypos, xpos, C_VERTDOT_SCREEN+5,C_CR_BACK);

    HoriOffsetScreen = 16;
    MCoorDraw(C_COORHPOSI , C_COORVPOSI, C_COORHEIGHT, C_COORWIDTH);
    MDrawLine(C_COORHPOSI + C_COORWIDTH, C_COORVPOSI, C_SCREEN_WIDTH, C_COORVPOSI,C_CR_BACK);
    MDrawLine(C_COORHPOSI + C_COORWIDTH, C_COORVPOSI+C_COORHEIGHT, C_SCREEN_WIDTH, C_COORVPOSI+C_COORHEIGHT,C_CR_BACK);
    WriteScale();	//标度
    MGateDraw();

    DataRestore(file_no);
    u_char echobuffer[C_LEN_SAMP];
    offset = C_OFF_SAMP;
    MCopyProtectedMemory(echobuffer, (void*)offset, C_LEN_SAMP, PM_COPY_READ);	//波形参数
    DrawWave(echobuffer,0,C_LEN_SAMP);

    //if( !MGetSaveMode() || !MGetSaveStatus() )
    //{
    offset = C_OFF_DAC;
    MCopyProtectedMemory( &Dac, (void*)offset, C_LEN_DAC, PM_COPY_READ);
    //	CalcuDac();
    DrawDac(0);		//新画DAC
    //}
    HoriOffsetScreen = 0;
    xpos = C_HORIDOT_SCREEN;
    ypos = 0;
    MDrawLine(xpos, ypos, xpos, C_VERTDOT_SCREEN+5,C_CR_BACK);
    MDrawLine(501, ypos, 501, 255,C_CR_BACK);

    if( stWeldPara.type > 0)
    {
        xpos = C_SCREEN_WIDTH+20;
        ypos = C_COORVPOSI;
        WELD_DRAW( &stWeldPara,xpos, 500, ypos+C_COORHEIGHT/2, 50,MGetAngle(0), MGetAmpDist1(0) );
    }
    xpos = 0;
    ypos = 24;
    u_short PrintHoriBytes = 64;
    while( ypos <= 240 - 12)
    {
        ScrcpyInit(PrintHoriBytes);
        ScrcpyInvert(xpos,ypos,PrintHoriBytes);
        ypos += 12;
    }
//4
    MFclearScreen();
    xpos = 0;
    ypos = 0;

    MDrawLine(xpos + 1, ypos, 501, ypos,C_CR_BACK);
    MDrawLine(xpos + 1, ypos + 48, 501, ypos + 48,C_CR_BACK);
    MDrawLine(xpos + 1, ypos + 96, 501, ypos + 96,C_CR_BACK);

    MDrawLine(xpos, ypos, xpos, ypos + 96,C_CR_BACK);
    MDrawLine(xpos + 76, ypos + 49, xpos + 76, ypos + 95,C_CR_BACK);
    MDrawLine(xpos + 148, ypos + 49, xpos + 148, ypos + 95,C_CR_BACK);
    MDrawLine(xpos + 190, ypos + 49, xpos + 190, ypos + 95,C_CR_BACK);
    MDrawLine(xpos + 260, ypos + 49, xpos + 260, ypos + 95,C_CR_BACK);

    CEMenuOut(xpos + 10, ypos + 15, _Report_C1, 4, 1, row_height);
    CEMenuOut(xpos + 6, ypos + 54, _Report_C2, 4, 2, row_height);
    CEMenuOut(xpos + 152, ypos + 64, _Report_C3, 2, 1, row_height);

    xpos = C_HORIDOT_SCREEN + 1;

    MDrawLine(xpos + 16, ypos + 49, xpos + 16, ypos + 95,C_CR_BACK);
    MDrawLine(xpos + 72, ypos + 49, xpos + 72, ypos + 95,C_CR_BACK);
    MDrawLine(xpos + 116, ypos + 49, xpos + 116, ypos + 95,C_CR_BACK);
    MDrawLine(501, ypos, 501, ypos + 95,C_CR_BACK);

    CEMenuOut(264 + 4, ypos + 54, _Report_C4, 4, 2, row_height);
    CEMenuOut(xpos + 76, ypos + 64, _Report_C3, 2, 1, row_height);
    ReportPrint();
    HoriOffsetScreen = 4;
    PrintEnd();
    MFclearScreen();
    return 1;
}
int ReportTextE(int mode, int file_no)		//报告文本
{
    int xpos,ypos;
//	int row_char_len = 38,row_number = 6;
    int	row_height = 24;
    int xpos2,ypos2;
//	u_short* cpText;
    u_char* epText;
    int i;
    int offset;

    if( TestPrinterReady() == 0) return 0;
    PrinterInitialize();		//打印机初始化

    if( MGetPrinterMode() == 3)
    {
        ReportText2(mode,file_no);
        return 1;
    }

    HoriOffsetScreen = 0;

    REPORT_INFO reportinfo;
    offset = C_OFF_REPORT_INFO;
    MCopyProtectedMemory(&reportinfo, (void*)offset, sizeof(REPORT_INFO), PM_COPY_READ);
    DataRestore(file_no);
    MSetLanguage(mode);
//1
    MFclearScreen();
    EMenuOut(20*6,34, _Report_A12, 22, 1,row_height);	//标题
    EMenuOut(35 * 7, 78, _Report_A32, 30,1,row_height);

    xpos = 0;
    ypos = 94;
    for(i = 0; i < 7; i++)
    {
        MDrawLine(xpos + 1, ypos + row_height * i, 502, ypos + row_height * i,C_CR_BACK);
    }

    MDrawLine(xpos, ypos, xpos, C_VERTDOT_SCREEN - 2,C_CR_BACK);
    MDrawLine(xpos + C_ECHAR_HDOT * 12, ypos, xpos + C_ECHAR_HDOT * 12, C_VERTDOT_SCREEN - 2,C_CR_BACK);
    EMenuOut(xpos + 2,ypos + 4, _Report_A22, 10, 12, 12);

    xpos2 = xpos + C_ECHAR_HDOT * 9 + 4;
//暂不写有关工件的信息
///	CEMenuOut(xpos2, ypos + 4, reportinfo.TestDep+1, 10, 1, 24);
///	CEMenuOut(xpos2, ypos + row_height + 4, reportinfo.EntrustDep+1, 10, 1, 24);
///	CEMenuOut(xpos2, ypos + row_height * 2 +4, reportinfo.WorkpieceName+1, 10, 1, 24);
///	CEMenuOut(xpos2, ypos + row_height * 4 + 4, reportinfo.WorkpieceSpec+1, 10, 1, 24);
///	CEMenuOut(xpos2, ypos + row_height * 5 + 4, reportinfo.MaterialNumber+1, 10, 1, 24);

    xpos = C_HORIDOT_SCREEN;

    MDrawLine(xpos, ypos, xpos, C_VERTDOT_SCREEN - 2,C_CR_BACK);
    MDrawLine(xpos + C_ECHAR_HDOT * 10, ypos, xpos + C_ECHAR_HDOT * 10, C_VERTDOT_SCREEN - 2,C_CR_BACK);
    MDrawLine(502, ypos, 502, C_VERTDOT_SCREEN - 2,C_CR_BACK);
    EMenuOut(xpos + 2,ypos + 4, _Report_A42, 8, 12, 12);

    xpos2 = xpos + C_ECHAR_HDOT * 10 + 4;
    offset = C_OFF_FILEINFO + C_SIZE_INT;
    MCopyProtectedMemory( &fat, (void*)offset, C_LEN_FAT, PM_COPY_READ);

    if( !MGetSaveMode() || !MGetSaveStatus() )
    {
        if( fat.name[0] > 8 )fat.name[0] = 8;
    }
    //if( fat.name[0] < 0 )fat.name[0] = 0;
    CEMenuOut2(xpos2,ypos + 4,fat.name + 1,fat.name[0],1,16);

    ReportPrint();
//2
    MFclearScreen();
    xpos = 0;
    ypos = 0;
    for(i = 1; i < 9; i++)
    {
        MDrawLine(xpos + 1, ypos + row_height * i, 502, ypos + row_height * i,C_CR_BACK);
    }

    MDrawLine(xpos, ypos, xpos, C_VERTDOT_SCREEN - 1,C_CR_BACK);
    MDrawLine(xpos + C_ECHAR_HDOT * 12, ypos, xpos + C_ECHAR_HDOT * 12, ypos + row_height * 8 - 1,C_CR_BACK);

    MDrawLine(xpos + C_ECHAR_HDOT * 20, ypos + row_height * 3 + 1, xpos + C_ECHAR_HDOT * 20, ypos + row_height * 6 - 1,C_CR_BACK);
    MDrawLine(xpos + C_ECHAR_HDOT * 31+4, ypos + row_height * 3 + 1, xpos + C_ECHAR_HDOT * 31+4, ypos + row_height * 6 - 1,C_CR_BACK);

    EMenuOut(xpos + C_ECHAR_HDOT * 20 + 4, ypos + row_height * 3 + 5, _Report_B22, 11, 3, row_height);
    EMenuOut(xpos + 2, ypos + 4, _Report_B12, 11, 16, 12);
    EMenuOut(xpos + 4, ypos + row_height * 8 + 4, _Report_B62, 24, 1, row_height);

    //左
    xpos2 = xpos + C_ECHAR_HDOT * 12 + 4;
    ypos2 = ypos + row_height * 5 + 8 ;
    Write_Number(xpos2,ypos2,MGetFrequence(),4,2,0);	//频率
    EMenuOut(xpos2+4*8,ypos2, "MHz",3,1,12);

    ypos2 = ypos + row_height * 6 + 8;
    if (MGetAngle(0)>=C_MAX_ANGLE)
    {
        TextOut(xpos2,ypos2-6,1,3,10,(char *)_infinity[MGetLanguage()][0],0);
    }
    else
    {
        Write_Number(xpos2,ypos2,( MGetAngle(1) + 50)/100,4,2,0);	//K value
    }
    Write_Ascii(xpos2 + 4 * C_ECHAR_VDOT,ypos2,'/');
    Write_Number(xpos2 + 5 * C_ECHAR_VDOT,ypos2 ,MGetAngle(0),4,1,0);	//angle
    Write_Ascii(xpos2 + 9 * C_ECHAR_VDOT,ypos2 ,130);

    ypos2 = ypos + row_height * 7 + 8 ;
    //	WriteBaseGain(xpos2,ypos2);
    Write_Number(xpos2,ypos2,MGetBaseGain(),4,1,0);
    //	WriteCompGain(xpos2 + 5*8,ypos2);
    Write_Number(xpos2+ 5*8,ypos2,MGetCompGain(),5,1,1);
    WriteSurfGain(xpos2 + 10*8,ypos2);
    EMenuOut(xpos2+ 15*8,ypos2, "dB",2,1,12);

    xpos2 = xpos + C_ECHAR_HDOT * 33 - 4;
    ypos2 = ypos + row_height * 4 + 4 ;	//类型
//	epText = (char *)_ParaMenu_AB2[MGetLanguage()][MGetProbeMode()]+2;
//	TextOut(xpos2+19*8 ,ypos2-8,1,8,16,epText,4);
    TextOut(xpos2 ,ypos2,1,8,16,(u_char *)_ParaMenu_AB2[MGetLanguage()][MGetProbeMode()]+2,4);
    ypos2 = ypos + row_height * 5 + 8 ;	//探头前沿
    WriteLongness(xpos2,ypos2,MGetForward(),4,1);

    ypos2 = ypos + row_height * 3 + 4 ;//声程标度
    TextOut(xpos2,ypos2 ,1,8,row_height,(u_char*)_ParaMenu_AB1[MGetLanguage()][MGetScaleMode()]+2,4);

//	epText = (char *)_MainMenu_C1A1[1][MGetSaveMode()];//声程标度
//	TextOut(xpos2+4,ypos2,1,8,16,epText,4);
    //右
    xpos = C_HORIDOT_SCREEN;
    EMenuOut(xpos + 20, ypos + 4, _Report_B32, 17,1,row_height);

    MDrawLine(xpos, ypos, xpos, C_VERTDOT_SCREEN - 1,C_CR_BACK);
    MDrawLine(xpos + C_ECHAR_HDOT * 11-4, ypos + row_height + 1, xpos + C_ECHAR_HDOT * 11-4, ypos + row_height * 8 - 1,C_CR_BACK);
    MDrawLine(502, ypos, 502, C_VERTDOT_SCREEN - 1,C_CR_BACK);

    if( MGetBGateMode() == 0)EMenuOut(xpos + 2, ypos + row_height + 8, _Report_B42, 10, 6, row_height);
    else EMenuOut(xpos + 2, ypos + row_height + 8, _Report_B43, 10, 6, row_height);
    EMenuOut(xpos + 2, ypos + row_height * 7 + 4, _Report_B72, 6, 2, 12);

    xpos = C_HORIDOT_SCREEN + C_ECHAR_HDOT * 10 + 4;
    ypos = row_height * 5+ 8 ;

    DataRestore(file_no);
    MSetLanguage(mode);
    DispEchoPara();

    ReportPrint();

    MFclearScreen();
    xpos = 0;
    ypos = 0;
    MDrawLine(xpos, ypos, xpos, C_VERTDOT_SCREEN+5,C_CR_BACK);

    HoriOffsetScreen = 16;
    MCoorDraw(C_COORHPOSI , C_COORVPOSI, C_COORHEIGHT, C_COORWIDTH);
    MDrawLine(C_COORHPOSI, 24, C_SCREEN_WIDTH, 24,C_CR_BACK);
    WriteScale();	//标度
    MGateDraw();

    DataRestore(file_no);
    MSetLanguage(mode);
    u_char echobuffer[C_LEN_SAMP];
    offset = C_OFF_SAMP;
    MCopyProtectedMemory(echobuffer, (void*)offset, C_LEN_SAMP, PM_COPY_READ);	//波形参数
    DrawWave(echobuffer,0,C_LEN_SAMP);

    offset = C_OFF_DAC;
    MCopyProtectedMemory( &Dac, (void*)offset, C_LEN_DAC, PM_COPY_READ);
    DrawDac(0);		//新画DAC

    HoriOffsetScreen = 0;
    xpos = C_HORIDOT_SCREEN;
    ypos = 0;
    MDrawLine(xpos, ypos, xpos, C_VERTDOT_SCREEN+5,C_CR_BACK);
    MDrawLine(502, ypos, 502, 255,C_CR_BACK);

    xpos = 0;
    ypos = 24;
    u_short PrintHoriBytes = 64;
    while( ypos <= 240 - 12)
    {
        ScrcpyInit(PrintHoriBytes);
        ScrcpyInvert(xpos,ypos,PrintHoriBytes);
        ypos += 12;
    }
//4
    MFclearScreen();
    xpos = 0;
    ypos = 0;

    MDrawLine(xpos + 1, ypos, 502, ypos,C_CR_BACK);
    MDrawLine(xpos + 1, ypos + 48, 502, ypos + 48,C_CR_BACK);
    MDrawLine(xpos + 1, ypos + 96, 502, ypos + 96,C_CR_BACK);

    MDrawLine(xpos, ypos, xpos, ypos + 96,C_CR_BACK);
    MDrawLine(xpos + 76, ypos + 49, xpos + 76, ypos + 95,C_CR_BACK);
    MDrawLine(xpos + 148, ypos + 49, xpos + 148, ypos + 95,C_CR_BACK);
    MDrawLine(xpos + 190, ypos + 49, xpos + 190, ypos + 95,C_CR_BACK);
    MDrawLine(xpos + 260, ypos + 49, xpos + 260, ypos + 95,C_CR_BACK);

    EMenuOut(xpos + 10, ypos + 12, _Report_C12, 12, 1, row_height);
    EMenuOut(xpos + 6, ypos + 64, _Report_C22, 8, 1, row_height);
    EMenuOut(xpos + 152, ypos + 64, _Report_C32, 4, 1, row_height);

    xpos = C_HORIDOT_SCREEN + 1;

    MDrawLine(xpos + 16, ypos + 49, xpos + 16, ypos + 95,C_CR_BACK);
    MDrawLine(xpos + 72, ypos + 49, xpos + 72, ypos + 95,C_CR_BACK);
    MDrawLine(xpos + 116, ypos + 49, xpos + 116, ypos + 95,C_CR_BACK);
    MDrawLine(502, ypos, 502, ypos + 95,C_CR_BACK);

    EMenuOut(264, ypos + 64, _Report_C42, 9, 1, row_height);
    EMenuOut(xpos + 76, ypos + 64, _Report_C32, 4, 1, row_height);
    ReportPrint();
    HoriOffsetScreen = 4;
    PrintEnd();
    MFclearScreen();
    return 1;
}










