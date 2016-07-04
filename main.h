//main.h
#ifndef _MAIN_H_

void MainInitialize(void);	/*软件初始化*/
void AppMain(void);			/*主循环*/
void SystemClear(void);			/*开机需初始化的参数在此函数中进行*/
void TestGainTable();
void TestGainTable1();
void AdjustGainTable();


void DisplayBMP(const u_char * picture,const u_long * Palette, int xpos, int ypos,int length,int width);
//测试函数
int TestMenuOffset(void);
int TestOffset(u_int testrang1,u_int testrange2,u_int mode);
//第1声程，第2声程 mode = 0距离测，=1深度 =2 曲面
int TestMenuAngle(void);
int TestAngle(u_int diameter,u_int depth);//直径、深度

int TestMenuDac(void);
int TestDac(u_int depthmax,u_int diameter,u_int length, short test_num);//最大深度、反射体直径、长度
int CalcuDac(void);	//计算DAC,
int DrawDac(int mode);		//画和或计算DAC曲线,mode=0，计算=1不
int DrawDac2(int mode);		//画和或计算DAC曲线,mode=0，计算=1不
int GetLine_TwoDot(u_short* line,int dist1,int dist2);
int GetMiddleValue(int dist1,int db1,int dist2,int db2,int dist);	//得到中间值

int TestFlawHigh(void);		//测高

int TestMenuStandard(void);
int TestMenuThick(void);
int TestMenuSystem(void);
int TestMenuFft(void);

void BScan(void);

int ManualMonthTest(void);//人工月性能校验

int TestMenuSensitivity(void);//灵敏度校验
int TestSensitivity(u_int depth/*反射体深度*/, u_int stdamp/*基准波高度*/, int mode/*0可调增益，1不可调*/);

int MenuMonthTest(void);//仪器性能校验
//菜单管理
int KeyManage(int keycode,int mode);		//按键管理,mode=1renovate
int MenuChoice(void);
int TestChoice(int Test);
int TestMenu(void);
int ParaMenu1(int page);//出现第几页的参数菜单
int ParaMenu(int page);//出现第几页的参数菜单
int ChannelMenu(void);
int DataMenu(int mode);	//mode > 0 为长按键，直接进入记录菜单
int FuncMenu(void);
int ColorMenu(void);

int STDMenu(void);
int OptionMenu(int page);
void InitPara(void)	;//初始化

//与存储有关的
int WaveSave(int mode);
int DataProcedure();	//数据处理
int DataDisplay(int file_no,int language);	//显示存储得数据
int DataSave();		//返回1表示已存，0未存
int DataStore(int file_no);
int DataRestore(int file_no);
int GetDataPosi(int file_no);	//得到第 x 个数据存储位置
int DataDelete(int file_no);	//删除一个数据，参数为负或正常删除返回剩余数据总数，参数过大返回－1
int FileDelete(int file_no,unsigned int off_total/*文件总数存储位置*/, unsigned off_fat/*文件头开始*/,int len_fat/*文件头长*/, unsigned int off_file/*文件体开始*/, int len_file/*文件体长*/);

int AutoSave(int mode);
int AutoSavePlay(int file_no,int file_total);		//从当前数据开始播放连续存储的波形
int SeekDate(int file_total);
int SeekName(int file_total);

#define _MAIN_H_
#endif

