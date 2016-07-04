//
#include "globe.h"

//#include "user.h"



//常用函数
//工具函数
void InitPara(void);		//初始化

//void MSetAmplifierGain(u_short gain);

void InvertBytes(u_int XPos, u_int YPos, u_int bytes) ;/*反白显示几个字节的区域*/

void InvertWords(u_int XPos, u_int YPos, u_int words) ;/*反白显示几个字的区域*/

int CharStr2Number( char str[],int number);//number：个数
int ShortStr2Number( short str[],int number);

u_int Bcd2Hex(u_int val);
u_int Hex2Bcd(u_int val);

u_int GetNumberLen(u_int number);	/*得到数的位数*/
void Swap(void *x,void *y);		/*两数交换*/
void ExpendTime(int time);	/*消耗时间，以10ms为单位*/
void BuzzerOn(u_int time); 		/*峰鸣0.1 * time 秒*/
void AlarmOn(void);
void MSetAlarmStatus(int on);
int MGetAlarmSatus();

u_int TestSameKeyTime(u_int Prekeycode,u_int PreElapsedTime,u_int MaxTime);
/*测试按键Prekeycode已按时长，PreElapsedTime为调用时间,MaxTime为最长时间，
	此时间到或有不同按键退出,返回值为同一按键持续时长,如Prekeycode = C_KEYCODEMAX 则得到
	前一次调用此函数的返回值*/
u_int GetSameKeyTime(void);
/*得到连续按键次数*/
u_int GetNoKeyTime(void);	//得到到从前次按键到目前止的时长，单位s,四舍五入

u_int CheckDate(u_int year,u_int month,u_int date);
//检查年、月、日是否符合规则，返回值0符合，非0不符合，1：year,2:month,4:date
//1+2:year+moth,1+4:year+date,2+4:month+date,1+2+4:year+month+date

void MPush(int mint);		//压栈
int  MPop(int mode);			//出栈,mode=0栈指针为0

int test_fft(void);
void FFT(short dataI[],short w[],int mode);		//通过转换后dataI中的所有数据转到dataR中
u_int Random();
float Mexpf(float ex, u_int len);//指数函数，得到e^ex,位数不大于len×8
int Mabsi( int x);
float Mabsf( float x);
double Mabsd( double x);
short Mabss( short x);
char Mabsc( char x);

//从包含了待存数据的变量中返回在指定位置的变量
u_int BitLoad(u_int des/*包含数据的变量*/, u_int bits/*字节数*/, u_int cx/*开始的位置*/);
//返回在指定位置包含了待存数据的变量
u_int BitSave(u_int des/*将要包含待存数据的变量*/, u_int bits/*字节数*/, u_int cx/*开始的位置*/,u_int var/*待存的变量*/);

void UGetSystemTime(TIME_CLOCK* tm);
void MSettimetoi2c(TIME_CLOCK TimeClock);
u_int MSetSystemTime(void);
//与键盘有关的函数
int MGetRotary(int rotary/*0第一个旋钮1第二个，－1全部，－2判断是否有旋钮*/);

//得到键值，time表示得到键值返回之前的最小时间
int MGetKey();
int MGetKey2();
int MGetKeyCode(u_int time);
void MKeyRlx(void);		/*只有按键已释放才退出*/
int MAnyKeyReturn(void);		/*直到释放当前按键并有新的按键才退出*/
int MAnyTimeReturn(u_int time);		/*直到有按键或时间到才退出 */
int MGetAnyKey(u_int time);			/*得到任意键，可能是本次按键也可能是前次按键未释放*/
int MenuKeyReturn(int keymax,int mode);	/*出现菜单时，按不大于keymax的键返回，
	mode=0其它键无效，=1确认键有效，=2返回键有效，=3确认返回键有效,keymax为负则可返回0*/

void MFclearScreen(void);
int MCopyProtectedMemory(void* memdes, void* memsrc, u_int bytesize, u_int copymode) ;
int CopyMemoryAllSystem(u_int copymode);
int CopyMemoryToSystem(void* memsrc, u_int bytesize,u_int copymode);
void MCTextOut(u_int x, u_int y, u_short* pzText, u_int len) ;
void METextOut(u_int x, u_int y, char* pzText, u_int len) ;
void MDrawLine(u_int x1, u_int y1, u_int x2, u_int y2,u_int BGColor) ;
void MDrawPixel(int x, int y, int mode) ;
u_int MGetPixels(u_int x, u_int y, u_int len) ;
int MPushWindow(u_int left, u_int top, u_int right, u_int bottom) ;
int MPopWindow(void) ;
void MEraseWindow(u_int left, u_int top, u_int right, u_int bottom) ;
void MInvertWindow(u_int left, u_int top, u_int right, u_int bottom) ;

/* activate/deactivate DAC funtion , bOn != 0 -> activate else deactivate */
void	MActivateDac(int bOn) ;
/* peak amplitude dac adjusting */
void    MDacGatePeak() ;

void	MSetEchoColor(u_int pos_s, u_int pos_e, u_int rgb) ;
void 	MSetDisplayColor(u_int rgb) ;

u_int 	MSetEchoRange(u_int range) ;
void MGetGatePeakBuffer() ;


int DrawCurver( short * buff,int startposi,int dotnumber);			//画曲线
int DrawEnvelope( u_char * buff,int startposi,int dotnumber);			//画包络
int DrawGateCurver( short * buff,int startposi,int dotnumber,int mode);
int DrawWave( u_char* buff,int start,int dots);		//画波形

u_int CheckDrawLine(int x1,int y1,int x2,int y2);
void DrawCircle(u_int HCenter, u_int VCenter, u_int CircleRadius);
void DrawArc1(int x1,int y1,int x2,int y2,int r);//画从点(x1,y1)到点(x2,y2)半径为r的弧线

u_int DrawMultiEdge(int xpos,int ypos,u_int edge,float angle,int len);
/*以指定位置为中心，画边数=edge，倾斜角=angle，顶点到中心距离为len的多边形*/
void DrawCursor(u_int xpos,u_int ypos,u_int mode);	/*画光标，mode=0英文1中文2斜十字*/
void ClearCursor(u_int mode);	/*清光标，mode=0英文1中文*/

/* 画矩形 */
void DrawRectangle(u_int left, u_int top, u_int right, u_int bottom) ;
/* 清除并画矩形 */
void EraseDrawRectangle(u_int left, u_int top, u_int right, u_int bottom) ;


void CMenuOut(u_int xpos,u_int ypos,const u_short pzText[],u_int char_len,u_int row_number,u_int row_height);
/*在指定位置根据每行字符数、行数、行高写菜单*/
void EMenuOut(u_int xpos,u_int ypos,const u_char* pzText,u_int char_len,u_int row_number,u_int row_height);
/*在指定位置根据每行字符数、行数、行高写菜单*/
void CEMenuOut(u_int xpos,u_int ypos,const u_short* pzText,u_int char_len,u_int row_number,u_int row_height);
/*在指定位置根据每行字符数、行数、行高写菜单*/
void CEMenuOut2(u_int xpos,u_int ypos,const u_short* pzText,u_int char_len,u_int row_number,u_int row_height);

void Write_Space(u_int xpos ,u_int ypos,u_int len,u_int mode);
/*写空格，mode=0英文字8x8,=1中文字16x16*/
int CETextOut(u_int xpos,u_int ypos,u_int lpos,u_int rpos,const u_short* pzText,u_int char_max,u_int row_height,u_int type);
/*最左lpos最右rpos，开始(xpos,ypos)，共写字符数char_max,每行点数，文本类型type=0英1中*/
int CETextOut2(int xpos,int ypos,const u_short* pzText,int col_max,int row_number,int row_height,int serial,int align);
/*开始(xpos,ypos)，每行最多字符数col_max(英文),行数row_number,行高row_height,考虑编号serial=0不1有，对齐方式align＝0左1右2中间3两端*/
int Write_UNumber(u_int xpos,u_int ypos,u_int number,u_int total_len,u_int deci_len);
/*在指定位置(xpos,ypos为屏幕上的点）写无符号数，total_len为总长度（包括符号点整数小数），deci_len为小数位长度*/

int Write_Number(u_int xpos,u_int ypos,int number,u_int total_len,u_int deci_len,u_int sign);
/*在指定位置(xpos,ypos为屏幕上的点）写有符号数，total_len为总长度（包括符号点整数小数），deci_len为小数位长度,sign=0无符号，=1有符号*/

void Write_Ascii(u_int xpos,u_int ypos,u_char AscChar);
/*在指定位置写一个ASCII字符*/

void Write_Crystal(u_int xpos,u_int ypos,u_int crystal_l,u_int crystal_w);		/*写晶片尺寸*/

int Input_Crystal(u_int xpos1,u_int ypos1,int *number_l,int *number_w);			/*输入晶片尺寸*/

int Input_Number(u_int xpos,u_int ypos,int *number,u_int int_len,int *deci_len,u_int sign);
/*在指定位置(xpos,ypos为屏幕上的点）输入有符号数，number>0 && <= 9时将作为初始值输入
int_len为整数位最大长度，deci_len为小数位最大长度
sign=0无符号，=1有符号*/
int Input_Float(u_int xpos,u_int ypos,float *number,u_int int_len,u_int deci_len,u_int sign);
/*在指定位置(xpos,ypos为屏幕上的点）输入浮点数，int_len为整数位最大长度，deci_len为小数位最大长度*/
int InputPwd(int x, int y,int total_len/*总长度*/);//返回负值表示自己退出，正值有输入
void WritePwd(int x, int y, int number);

int CMessageBox( int x,int y, u_char * epText, int len, int mode );
int EMessageBox( int x,int y, u_char * epText, int len, int mode );
int EMessageBox2( int x,int y, u_char * epText,int len, int row,int mode );

/* 显示提示信息*/
void DisplayPrompt(u_int num);//显示提示
int DisplayQuery(int index);//显示询问

void WriteFileDate(u_int xpos, u_int ypos, u_int date);//date = year * 65536 + month*256+day
void WriteFileDate1(u_int xpos, u_int ypos, u_int date);

void WriteDate(u_int xpos, u_int ypos, TIME_CLOCK *tm);
void WriteTime(u_int xpos, u_int ypos, TIME_CLOCK *tm);
void WriteTime1(u_int xpos, u_int ypos, u_int tm);

u_int InputDate(u_int xpos, u_int ypos, TIME_CLOCK *tm);
u_int InputTime(u_int xpos, u_int ypos, TIME_CLOCK *tm);
u_int InputWeek(u_int xpos, u_int ypos, TIME_CLOCK *tm);
u_int date2int(TIME_CLOCK *tm);//2004/05/06
u_int date2int2(TIME_CLOCK *tm);//化为到2000－1－1的天数
void int2date(TIME_CLOCK *tm,u_int date);
void int2date2(TIME_CLOCK *tm,u_int date);//从到2000－1－1的天数－》tm

//

//与功能有关的函数
int SearchPeak(u_char * sampbuff,u_int curr_pos,u_int direction);		//返回最高波所在水平点数，如为负数则为无最高波
//寻找sampbuff指向波形存储区的最高波，curr_pos为当前位置,direction为方向:C_KEYCOD_LEFT左C_KEYCOD_RIGHT右
u_int PeakMemory(void);		//峰值记忆
int EchoEnvelope(void);	//回波包络
int DepthCompensate(void);		//深度补偿
void ClearEnvelope(void);		//清包络或清峰值

int GateWaveExpend( int mode);		//门内展宽
//与仪器系统探头有关的函数
//ontime 时间，//时间单位，MILLION：毫秒，SECOND:秒，minute:分钟
u_int MLoadStartTime(void);//分钟
void MSaveStartTime(int time);

u_int MLoadCurrentTime(void);//分钟

u_int MLoadSystemElapsedTime( int mode);//时间单位，MILLION：毫秒，SECOND:秒，minute:分钟
void MSaveSystemElapsedTime( u_int nElapsedTime ,int mode);

//mode = 0年，1月，2，日，3周几，4小时，5分钟，6秒，
//-1年月日,	-2：2000年后第几日，-3第几周，-4今年第几日，-5第几周,－6今天开始后多少分钟,－7今天开始后多少秒
u_int MGetSystemTime(int mode);

u_int MGetCurrentYearTimeCount(int date, int time,int mode);//得到当前时间距当年1月1日0点的差距，mode = 2，日，3周，4小时，5分钟，6秒
u_int MGetYearDateCount( int year1, int year2);//得到年year1到年year2之间的天数，比如2003至2004有365天

u_int MGet2kDays(int mode);//mode=0得到存储的天数，其他实际天数
void MSet2kDays(u_int days);//设置得到到2000年的天数；



u_int MGetDenoise();
void MSetDenoise(u_int denoise,int mode/*mode = 0\1\2*/);
u_int MGetRepeatRate();
void MSetRepeatRate(u_int repeat,int mode/*mode = 0\1\2*/);
u_int MGetPulseWidth();
void MSetPulseWidth(u_int pulsewidth,int mode/*mode = 0\1\2*/);
//与通道状态有关的函数
u_int MLoadChannelStatus( int mode);//取通道状态中的某个变量,mode=0-n,按字母顺序从上到下
void MSaveChannelStatus( int mode, u_int para);


u_int MGetBandMode(void);	/*返回所用频带宽度：2.5\5.0\10\wide	*/
void MSetBandMode(u_int iBand,int mode/*mode = 0\1\2*/);

u_int MGetEchoMode(void);	/*返回所用检波方式：全波\正半波\负半波\射频  	*/
void MSetEchoMode(u_int Wave,int mode/*mode = 0\1\2*/);

u_int MGetDampMode(void);	/*返回所用探头阻尼：50Q\400Q 	*/
void MSetDampMode(u_int iDamp,int mode/*mode = 0\1\2*/);

u_int MGetPulseMode(void);	/*返回所用发射电压：200V\400V  	*/
void MSetPulseMode(u_int Pulse,int mode/*mode = 0\1\2*/);

u_int MGetProbeMode(void);	/*返回值为当前探头模式1\2\3\4=直\斜\双晶*/
void MSetProbeMode(u_int iProbe,int mode/*mode = 0\1\2*/);	/*设置探头模式*/

u_int MGetStdMode(void);	/*返回所用标准	*/
void MSetStdMode(u_int iStd,int mode/*mode = 0*/);

u_int MGetFunctionMode(u_int type);	//得到type所指功能的当前状态,所有功能只有启用与关闭两种状态
void MSetFunctionMode(u_int func,u_int type);	//设置type所指功能的状态

int MSetCurveAmend(CURVE_AMEND *stCurveAmend);//参数正确可计算返回1，否则返回0；
int MGetCurveAmend(int *dist, int *hori, int *vert, CURVE_AMEND *stCurveAmend);//返回经修正后的水平和垂直

u_int MGetDacMode(void);	/*返回所用当量标准：母线\判废\定量\评定	*/
void MSetDacMode(u_int iDac,int mode/*mode = 0*/);

u_int MGetAvgMode(void);	/*返回所用当量显示：dB值\孔径	*/
void MSetAvgMode(u_int iAvg,int mode/*mode = 0*/);
int MGetEquivalentMode();		//当量显示方式，<0波高，1母线2RL3SL4EL,5AVG-db,6avg-孔径

int MGetScaleMode(void);	/*返回所用标度：垂直\水平\距离*/
void MSetScaleMode(int scale,int mode/*mode = 0*/);

int MGetParaLock(int mode,int type);	//返回参量锁定状定0未锁1锁定,mode=1出提示
void MSetParaLock(int status, int type);

int MGetBGateMode();
void MSetBGateMode(int bgate,int mode);	//设置B门，bgate=0失波1进波

int ParaStepChange(u_int paraindex);	//设置待调节参量步长
int MGetParaStep(u_int paraindex);	//待调节参量步长
int MGetParaSort(int index);		//待调节参量名(门A/门B),系统增益/补偿增益
int MSetParaSort(int status,int index);		//待调节参量名(门A/门B),系统增益/补偿增益

//cgain<1000就不变，其他小于0就不变，必须保证bgain+sgain+cgain大于0小于最大增益量，其他参数也必须在范围之内
void MSetPara(int bgain/*基本增益*/,int cgain/*调节增益*/,int sgain/*表面补偿*/,int scale/*标度*/,int range/*声程*/,int offset/*零点*/,int delay/*延时*/,int angle/*折射角*/,int speed/*声速*/);

int MGetCurveNumber(void);	//得到曲线的数目
void MSetCurveNumber(int number);//设置曲线的数目

int MGetBlock(void);//得到试块类型
void MSetBlock(int block);//设置试块类型


int  MGetEquivalentDays(int mode);//测试天数,mode = 0仪器性能，1零点/K/dac/avg,2灵敏度
void MSetEquivalentDays(int mode,int days);//测试天数,mode = 0仪器性能，1零点/K/dac/avg,2灵敏度

int MGetTestStatus(u_int index);	//返回测试状态0=未测1=已测，
void MSetTestStatus(u_int index,int status);	//设置测试状态

int MGetAmpStdMax(void);//0-200
void MSetAmpStdMax(int amp);//设置自动调节时标准波高,AMP=0-100


//通道初始化
int ChannelParaInit(int mode);	/*参数初始化，mode=0当前一个通道，mode=1所有通道,2所有用户*/

//与门有关的函数
void MSetGatePara(u_short posi,u_short wide,u_short high,int gatetype,int mode);
/*设置门参数，gatetype=0A门1B门*/
u_short MGetGatePara(int gatetype, int paratype);   /* 返回值为门参数 */

short MGetGatedB(void);
void MSetGatedB(short gatedB, int mode/*mode = 0*/);

void DACGatedB(void);	//DAC门dB差
//void DACGate(void);
void DacGateInit(void);
int DacGate_dBHigh(int dB,int dac_dB);//门与母线的dB差，DACdB值
int MGetDacDb(void);

//与通道参量有关的函数

u_short MGetThick(void);//得到工件厚度
void MSetThick(u_short thick);//设置工件厚度

u_short MGetDiameter(void);//得到工件直径
void MSetDiameter(u_short Diameter);//设置工件直径

u_short MGetOffset(void);			/*返回值为当前通道	*/
void MSetOffset(u_int Offset,int mode/*mode = 0\1\2*/);	/*设置当前通道	*/

u_short MGetSpeed(void);			/*返回值为当前通道试件声速：	无小数	*/
void MSetSpeed(u_short Speed,int mode/*mode = 0\1\2*/);	/*设置当前通道	*/

int MGetDepth();
void MSetDepth(int depth,int mode);

u_int MGetAngle(u_int mode);			/*返回值为当前通道折射角0角度1正切2正弦3余弦	角度一位小数，其它4位*/
void MSetAngle(u_short Angle,int mode/*mode = 0\1\2*/);	/*设置当前通道	*/
u_short MGetKvalue(void);			/*返回值为当前通道K值 两位小数	*/
void MSetKvalue(u_short Kvalue,int mode/*mode = 0\1\2*/);	/*设置当前通道	*/

u_short MGetForward(void);			/*返回值为当前通道探头前沿长度 一位小数	*/
void MSetForward(u_short Forward,int mode/*mode = 0*/);	/*设置当前通道	*/

u_short MGetFrequence(void);			/*返回值为当前通道探头频率 一位小数	*/
void MSetFrequence(u_short Frequence,int mode/*mode = 0*/);	/*设置当前通道	*/

u_short MGetCrystal_l(void);			/*返回值为当前通道晶片尺寸 a*256+b	*/
u_short MGetCrystal_w(void);			/*返回值为当前通道晶片尺寸 a*256+b	*/
void MSetCrystal_l(u_short Crystal,int mode/*mode = 0*/);	/*设置当前通道	*/
void MSetCrystal_w(u_short Crystal,int mode/*mode = 0*/);	/*设置当前通道	*/

int MGetNearField(double multi);		//得到近场区长度
float MGetAttenuate(int depth);		//材质衰减系数

u_int MGetRange(int mode);			/*返回值为mode=0直接距离，非0为当前标度下的距离	*/
void MSetRange(int Range,int mode/*mode = 0\1\2*/);	/*设置当前通道	*/

u_int MGetDelay(int mode);			/*返回值为mode=0时间，1当前标度下的声程,3延时距离	*/
void MSetDelay(int Delay,int mode/*mode = 0\1\2*/);	/*设置当前通道	*/
void MSetScaleDelay(int Delay,int mode/*mode = 0\1\2*/);	/*设置当前标度下的延时距离	*/

u_short MGetReject(void);			/*返回值为当前通道	*/
void MSetReject(u_short Reject,int mode/*mode = 0\1\2*/);	/*设置当前通道	*/

u_short MGetSysGain(void);	//得到总体增益
u_short MGetSurfGain(void);			/*返回值为当前通道表面补偿增益 一位小数	*/
int MSetSurfGain(u_short SurfGain,int mode/*mode = 0\1\2*/);	/*设置当前通道	*/

u_short MGetBaseGain(void);			/*返回值为当前通道基本增益 一位小数	*/
int MSetBaseGain(short BaseGain,int mode/*mode = 0\1\2*/);	/*设置当前通道	*/

short MGetCompGain(void);			/*返回值为当前通道补偿增益 一位小数	*/
int MSetCompGain(short CompGain,int mode/*mode = 0\1\2*/);	/*设置当前通道	*/

u_short MGetDepthGain(void);
void MSetDepthGain(u_short gain);//设置无DAC/AVG时深度补偿值

short MGetLineGain(int line);//得到曲线的偏移量
void MSetLineGain(int line,short gain);//设置曲线的偏移量

void MSetSystemGain(void);	/*设置当前增益 */
void MSetSystemRange(void);	/*设置当前声程 */
void MSetSystemDelay(void);	/*设置当前延时或零点 */
void MSetSystemBright(void);/*设置当前屏幕亮度 */
int MSetSystemProbe(void);	/*设置当前探头模式*/
void MSetSystem(void);		/*设置当前系统各状态*/


//与用户管理有关的函数
int UserManager(void);
int UserStore(void);
int UserRestore(void);
int GetUserNumber();//得到当前设置的用户总数
void SetUserNumber( int number);//设置当前用户总数
int GetCurrentUser();//得到当前用户的序号，0－总数减1
void SetCurrentUser( int user);//设置当前用户的序号，0－总数减1
void UserMove(int user);

int GetUserName( u_short name[],int nUserNumber);//得到序号用户的姓名,返回1正常取得，0有错
int CheckPwd(int x, int y, int pwd);
int InputPwd(int x, int y,int total_len/*总长度*/);//返回负值表示自己退出，正值有输入
void WritePwd(int x, int y, int number);
int GetPwd(int nUserNumber);//得到序号用户的密码

void SysPwd(int mode);

//与输入法有关的函数
int MInputChar(u_int xpos,u_int ypos,u_int type,u_short* InputChar,u_short len_char_max,u_short char_max);
/*输入字符，type = 0只能英文，1可英可中,len_char_max每行可写的最多英文字符数，char_max总共可写的中英文字符数*/
int  MGetCChar(u_int xpos,u_int ypos,u_short association);		/*得到字符值，英文为ascii码，中文为区位码*/
int MGetQuwei(u_int xpos,u_int ypos,u_short association);
u_short MGetEChar(u_int xpos,u_int ypos,u_short* pzText,u_int mode);		/*得到ASCII字符*/
char MGetSymbol(void);	/*得到8x8标点符号*/

//与回波参数有关的函数
void MGetGatePeakBuffer() ;
int MGetGateParaMax(int gatetype);
//得到门内最高回波的传输时间、水平点、距离、波高,gatetype = 0：A门，=1：B门
//返回C_TRUE表示有新的最高波,如有波与历史最高波在同一点则蜂鸣0.1s
void MSetGateParaInit(void);	//设初值
int MGetAmpMax(int gatetype);	/*得到指定门（0=A or 1=B)内回波高度，*/
int MGetAmpPos(int gatetype);	/*得到指定门（A or B)内回波水平点数，*/
int MGetAmpPos1(int gatetype,int mode);	/*得到指定门内回波mode=0开始/1最高/2结束3/前端门相交/4后端门相交的点数，*/

int MGetAmpTime(int gatetype);	/*得到指定门（A or B)内回波传输时间，*/
int MGetAmpDist(int gatetype);	/*得到指定门（A or B)内回波距离声程，*/
int MGetAmpDist1(int gatetype);	/*得到指定门（A or B)内回波距离声程，*/
int Amp2db(int pos,int amp);		//根据回波位置和波高得到dB值


//各种系统状态参量
u_int MLoadSystemStatus( int mode);//取系统状态中的某个变量,mode=0-n,按字母顺序从上到下
void MSaveSystemStatus( int mode, u_int para);

u_int MGetPrinterMode(void);	/*返回值为当前打印机类型1\2\3\4=LQ\HP\EPSON C/P*/
void MSetPrinterMode(u_int iPrinter,int mode/*mode = 0*/);	/*设置当前打印机类型*/

u_int MGetInputMode(void);	/*返回值为当前输入法类型1\2\3\4=数字\大写\小写*/
void MSetInputMode(u_int iInput,int mode/*mode = 0*/);	/*设置当前输入法类型*/

u_int MGetSystemMode(void);	/*返回值为当前系统状态0\1=正常\调节*/
void MSetSystemMode(u_int iSystem ,int mode/*mode = 0*/);	/*设置当前系统状态*/

u_int MGetAcquisitionEnable(void);	/*是否允许采样d5*/
void MSetAcquisitionEnable(u_int acquisition,int mode/*mode = 0\1\2*/);
void MSetAcquisition(u_int mode);	/*设置采样与否，0不采样1采样*/
u_int MGetFill(void);//显示硬件波形实心还是空心
void MSetFill(u_int fill,int mode);
u_int MGetHardEchoShow(void);//显示硬件波形
void MSetHardEchoShow(u_int show,int mode);

u_int MGetSoftEchoShow(void);//显示软件波形
void MSetSoftEchoShow(u_int show,int mode);

u_int MGetCursorEnable(void);//是否允许有光标
void MSetCursorEnable(u_int cursor,int mode);

u_int MGetCursorShow(void);//是否显示光标
void MSetCursorShow(u_int cursor,int mode);
u_int MGetColorMode(void);//
void MSetColorMode(u_int item,int mode);
u_int MGetChannel(void);	/*返回奉来当前通道号*/
int MSetChannel(int channel,int mode/*mode = 0*/);

u_int MGetSoundAlarm(int mode);//mode = 0按键，1报警
void MSetSoundAlarm( int mode, int alarm );//mode = 0按键，1报警

u_int MGetScreenBrightness(void);	/*返回值为当前屏幕亮度*/
void MSetScreenBrightness(u_int iBrightness,int mode/*mode = 0\1\2*/);		/**/

int MGetInputEnable(int type);
void MSetInputEnable(int bEnable, int type);//设置某种输入法是否可选

int MGetSaveMode(void);		//存储模式0单1连续
int MSetSaveMode(int style,int mode);
int MGetSaveStatus(void);	//存储状态0不在存，1正在存
int MSetSaveStatus(int status,int mode);
int MGetSaveTime(void);		//得到连续存间隔时间
int MSetSaveTime(int time,int mode);
int MGetSavePosition(void);		//得到存储位置
int MSetSavePosition(int time,int mode);

int MGetSaveFrequency();
void MSetSaveFrequency(int bSave,int mode);//是否存储频谱

int MGetCraft();	//得到工艺号
void MSetCraft(int craft,int modee);

int MGetDacShowMode(void);//设置dac显示模式，0波幅，1dB
void MSetDacShowMode(int show,int mode);

int MGetKeyLock();
void MSetKeyLock(int lock,int mode);//键盘锁,1锁,0开

void MSetAmp2Gate( int gatetype,int gate);//门内回波是否高于门高
int MGetAmp2Gate( int gatetype);

int MGetGateEchoColor( int gatetype);
void MSetGateEchoColor( int gatetype,int gate);//门内回波颜色是否要变

void MSetReadForeland( int read);//是否前沿读数
int MGetReadForeland( );

// 得到语言
int MGetLanguage(void);
//设置语言
void MSetLanguage(int language);
// 得到计量单位
void MSetUnitType(int unit);
// 设置计量单位
int MGetUnitType(void);


/*
数据移动、复制、初始化
*/

int ChannelParaStore(void);		/*通道参数存到保护区*/
int ChannelParaRestore(void);		/*通道参数从保护区取*/

int SystemParaStore(void);		/*系统参数存到保护区*/
int SystemParaRestore(void);		/*系统参数从保护区取*/

int AllParaStore(void);
int AllParaRestore(void);//全部参量备份和恢复
//与打印有关的函数
int mSendByteToPrinter(u_int pdata) ;

u_int TestPrinterReady();	//检测打印机是否准备好，如没有出现提示，共3次，返回1为准备好
void PrinterInitialize();		//打印机初始化
void PrintEnd(void);	//打印结束
void PrinterFormFeed();		//打印机进纸
void SetPrnPosi_AbHori_EpC61(u_char nl,u_char nh);
//Set absolute horizontial print positon,Epson C61 Para:Ah=nH,Al=nl	ESC $ nL nH;
void SetPrnPosi_ReVert_EpC61(u_char ml,u_char mh);
//设置垂直相对打印位置，Para:Al=mL,Ah=mH  ESC ( v nl nh mL mH,
void SetPrn_Raster_EpC61(u_short PrintHoriBytes);	//Para:Print_HoriBytes,待打印横向字节数，屏拷40报告80
//ESC . c v h m nl nh d1...dk(d1...dk在下段输出）
void LQ12To24(u_int xpos,u_int ypos,u_char* PrnBuff);	//针打垂直12针变为24针
u_short Byte2Word(u_char Byte);		//一个字节变为一个字8-16
u_int Byte2DWord(u_char Byte);		//一个字节变为一个双字8-32
void ScrcpyInit(u_short PrintHoriBytes);
void ScrcpyInvert(u_int xpos,u_int ypos,u_short PrintHoriBytes);
u_int ScreenCopy(void);		//屏拷
void ReportPrint(void);		//报告打印
int ReportText(int mode, int file_no);		//报告文本


//设置绝对垂直打印位置，以顶部空白为基准
void SetPrnPosi_AbVert(int dots/*移动点数，只能正数*/);
//设置相对垂直打印位置，以当前位置为基准
void SetPrnPosi_ReVert(int dots/*移动点数，可正可负*/);
//设置绝对水平打印位置，以左部空白为基准
void SetPrnPosi_AbHori(int dots/*移动点数，只能正数*/);
//设置相对水平打印位置，以当前位置为基准
void SetPrnPosi_ReHori(int dots/*移动点数，可正可负*/);

//void ScreenStore();//保存屏幕内容
void DebugInfo(int num);//C_DEBUG == 1时，在（10，30）显示数字


#if C_DEVLIB == 4
void	SwitchBuzzerOn(u_int bEnable) ;
u_char* GetDacBuffer() ;
/* activate/deactivate DAC funtion , bOn != 0 -> activate else deactivate */
void	ActivateDac(int bOn) ;
/* peak amplitude dac adjusting */
void    DacGatePeak() ;
void 	UpdateEchoColor() ;
void 	EnableEchoDisplay(u_int ena, u_int mode_fill);
void 	NotExtendEchoDisplay(u_int notext) ;
u_int 	SerialPortByteOut(u_int dout) ;
u_int 	SerialPortByteIn() ;
/* noise reduction , level: 0->no, 1->2 average, 2->4 average, 3->8 average */
//void 	DeNoise(u_int level) ;

#elif C_DEVLIB == 23 || C_DEVLIB == 24

u_int 	SerialPortByteOut(u_int dout) ;
u_int 	SerialPortByteIn() ;
u_char* GetDacBuffer();
/* activate/deactivate DAC funtion , bOn != 0 -> activate else deactivate */
void	ActivateDac(int bOn);
/* peak amplitude dac adjusting */
void    DacGatePeak() ;

#endif


void m_DrawLine(  int x1,int y1,int x2,int y2);
void m_DrawDot(  int x,int y);//在pDC所指在设备上从画点(x,y)
void m_DrawCircle(int x,int y,int radius);
//在pDC所指在设备上以点(x,y)为圆心,radius为半径的圆环,start_angle为起始角度,水平x方向为0度
//angle为从起始值顺时针方向旋转角度值,
void m_DrawArc2(int x,int y,int radius,double start_angle,double angle);
//在pDC所指在设备上从点(x1,y1)/(x2,y2)的圆弧,mode=0直线1向上凸－1向下凹
//返回所画圆弧的半径
int m_DrawArc1( int x1,int y1,int x2,int y2,int mode);

//void WELD_DRAW( CDC *pDC);
void WELD_DRAW( WELD_PARA *stWeld,int wlpos, int wrpos,int wypos,int maxsize,int ProbeBeta, int dist_dire);
void SINGALWELD_DRAW( WELD_PARA *stWeld,WELD_POSI *stWPosi);
void DOUBLEWELD_DRAW( WELD_PARA *stWeld,WELD_POSI *stWPosi);
void TWELD_DRAW( WELD_PARA *stWeld,WELD_POSI *stWPosi);
void WELD_BUGLINE(WELD_PARA *stWeld,WELD_POSI *stWPosi,int ProbeBeta, int dist_dire);

