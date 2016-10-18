/*字符定义*/
#ifndef		_CHAR_H_
#define		_CHAR_H_
/*
#define CH(a)		( (  (a)/100 )*256 + ( (a)%100 ) )
#define ch(a)		( ( (a)/100 )*256 + ( (a)%100 ) )
*/



const char _us[C_LANGUAGE][1][4] =
{
    {
        "μ ",
    },
    {
        "μ ",
    },
};
//---------仪器性能测试---------

const u_char _Power_prompt[C_LANGUAGE][7][40] =
{
    {
        "水平线性                               ",
        "垂直线性                               ",
        "分辨力                                 ",
        "动态范围                               ",
        "灵敏度余量                             ",
        "仪器性能指标                           ",
        "＃重测  ぁ退出                         ",
    },
    {
        "Horizontal Linearity                   ",
        "Vertical Linearity                     ",
        "Resolution                             ",
        "Dynamic Range                          ",
        "Surplus Sensitivity                    ",
        "Detector Performance                   ",
        "＃ Redo ぁ Exit                        ",
    }
};

const u_short _Power_prompt3[] =
{
    '1',' ',
    '#',' ',
    ENTER,' ',
};
const u_short _Power_prompt2[] =
{
    '1',	CH(2082),CH(5101),
    '#',	CH(5456),CH(1866),
    ENTER,CH(4543),CH(1986)
};
/*
水平线性
垂直线性
分辨力
动态范围
灵敏度余量

1.打印
2.重测
3.退出
*/

const char _Menu_Power[C_LANGUAGE][5][90] =
{
    {
        "将直探头放在25mm厚试块上，回波稳定后按确定                        ",  //水平线性
        "请除去探头后按确定                                                ",    //灵敏度余量
        "请将探头置于试块上当深85和91两回波等高且稳定时按确定并稳住探头    ",//分辨力
        "在CS-1-5试块上移动探头当200mm 处Φ2平底孔回波最高时按确定并稳住探头",//垂直线性
        "请除去探头后按确定                                                ",    //灵敏度余量
    },
    {
        "Put the normal probe  on the 25mm thick testblock and keep it stable,press Enter        ", //0水平线性
        "Remove the probe, press Enter    ",   //1灵敏度余量
        "The heights of 85 and 91mm depth echoes are all 50%, Press Enter                        ", //2分辨力
        "The highest echo of   200mm deep,Φ2 flat bottomed hole is 50%, press Enter              ", //3垂直线性
        "Remove the probe, press Enter    ",   //4灵敏度余量

    }
};
const u_char _Menu_Power1E[] = {"\
Put the normal probe on the \
25mm thick test block and   \
keep it stable,press Enter  "
};
const u_short _Menu_Power1[] =
{
    CH(2911),CH(5417),CH(4429),CH(4523),CH(2337),CH(5258),'2','5' ,'m','m' ,CH(2681),CH(4252),CH(3173),
    CH(4147),',', ' ',CH(2756),CH(1808),CH(4640),CH(2208),CH(2683),ENTER, CH(4023),CH(4047),CH( 100)
};
/*
将直探头放在25mm厚试块
上，回波稳定后EN确认
*/

const u_short _Menu_Power2[] = {CH(1620),ENTER,CH(2892),CH(4023),CH(4047)};
/* 按确认键确认*/


const u_short _Menu_Power3[] =
{
    CH(3975),CH(2911),CH(4429),CH(4523),CH(5435),CH(5158),CH(4252),CH(3173),CH(4147),
    CH(2117),CH(4178),'8','5', CH(2645),'9','1', CH(3329),CH(2756),CH(1808),CH(2140),CH(2463),CH(3950),
    CH(4640),CH(2208),CH(4217),ENTER, CH(4023),CH(4047),CH(1802),CH(4640),CH(5501),CH(4429),CH(4523),CH( 100),CH( 100)
};
/*分辨力
请将探头置于试块上
当深85和91两回波等高且
稳定时()确认并稳住探头
*/

const u_short _Menu_Power4[] =
{
    CH(5258),'C','S', '-','1' ,'-','5', CH(4252),CH(3173),CH(4147),CH(5038),CH(2215),CH(4429),CH(4523),
    CH(2117),'2','0', '0','m', 'm',' ', CH(2006),136,'2' ,CH(3829),CH(2155),CH(3155),CH(2756),CH(1808),
    CH(5578),CH(2463),CH(4217),ENTER, CH(4023),CH(4047),CH(1802),CH(4640),CH(5501),CH(4429),CH(4523)
};

const u_short _Menu_Power5[] =
{	CH(3975),CH(1993),CH(4005),CH(4429),CH(4523),CH(2683),ENTER, CH(4023),CH(4047)};
/*垂直线性
在CS-1-5试块上移动探头
当200mm 处()2平底孔回波
最高时()确认并稳住探头

灵敏度余量
请除去探头后EN确认
*/



const u_char _Menu_Power3E[] = {"\
The heights of 85 and 91mm\
depth echoes are all 50%, \
Press Enter               "
                               };
/*分辨力
请将探头置于试块上
当深85和91两回波等高且
稳定时()确认并稳住探头
*/
const u_char _Menu_Power4E[] = {"\
The highest echo of 200mm \
deep, 2 flat-bottomed     \
hole is 50%, press Enter  "
                               };
const u_char _Menu_Power5E[] = {"Remove the probe, press Enter    "};

/*垂直线性
在CS-1-5试块上移动探头
当200mm 处()2平底孔回波
最高时()确认并稳住探头

灵敏度余量
请除去探头后EN确认
*/
const char _ParaName[C_LANGUAGE][21][16] =
{
    {
        "基本增益        ",
        "表面补偿        ",
        "扫查增益        ",
        "垂直声程        ",
        "水平声程        ",
        "距离声程        ",
        "工件声速        ",
        "探头零点        ",
        "垂直延时        ",
        "水平延时        ",
        "距离延时        ",
        "抑制            ",
        "折射角度        ",
        "Ａ门位          ",
        "Ａ门宽          ",
        "Ａ门高          ",
        "Ａ门高          ",
        "Ｂ门位          ",
        "Ｂ门宽          ",
        "Ｂ门高          ",
    },
    {
        "Basic Gain      ",
        "Surface Gain    ",
        "Scanning Gain   ",
        "Vertical Range  ",
        "Horizontal Range",
        "Distance Range  ",
        "Sound Vel.      ",
        "Probe Zero      ",
        "Vertical Delay  ",
        "Horizontal Delay",
        "Distance Delay  ",
        "Reject          ",
        "Angle           ",
        "Gate A Start    ",
        "Gate A Width    ",
        "Gate A Level    ",
        "Gate A Level    ",
        "Gate B Start    ",
        "Gate B Width    ",
        "Gate B Level    ",
    }
};

//记录设置
const u_short _Report_Info1[] =
{
    CH(217),CH(4017),CH(4761),CH(1904),CH(2240),CH( 326),
    CH(218),CH(4017),CH(4761),CH(2463),CH(2240),CH( 326),
    CH(219),CH(2504),CH(2894),CH(4837),CH(4702),CH( 326),
    CH(220),CH(2876),CH(4973),CH(2105),CH(4627),CH( 326),
    CH(221),CH(4615),CH(4548),CH(2105),CH(4627),CH( 326),
};
/*	1.缺陷长度:
	2.缺陷高度:
	3.工件信息:
	4.检验单位:
	5.委托单位:
*/

const u_short _Report_Info2[] =
{
    CH(217),CH(3153),CH(1655),
    CH(218),CH(4268),CH(4075),
    CH(219),CH(1866),CH(3331)
};

/*	1.空白
	2.输入
	3.测量
*/

const u_short _Report_Info3[] =
{
    CH(217),CH(4620),CH(4268),CH(4075),
    CH(218),CH(5049),CH(4268),CH(4075)
};
/*
1.未输入
2.已输入
*/
const u_short _Report_Info32[] =
{
    CH(217),CH(1827),CH(4268),CH(4075),
    CH(218),CH(4268),CH(4075),CH( 100)
};

const u_short _Report_Info31[] =
{
    CH(217),CH(2504),CH(2894),CH(3591),CH(1938),CH( 326),
    CH(218),CH(2504),CH(2894),CH(1764),CH(2637),CH( 326),
    CH(219),CH(2504),CH(2894),CH(2570),CH(2481),CH( 326),
    CH(220),CH(1836),CH(3347),CH(3738),CH(2637),CH( 326),
    CH(221),CH(1777),CH(3570),CH(5520),CH(3186),CH( 326)
};
/*	1.工件名称：
	2.工件编号：
	3.工件规格：
	4.材料牌号：
	5.表面状况：
*/

/*
const u_short _Report_Info4[] = {
CH(217),CH(2876),CH(4973),CH(1831),CH(4627),CH( 326),
CH(218),CH(2876),CH(4973),CH(1740),CH(3293),CH( 326),
CH(219),CH(2876),CH(4973),CH(4217),CH(2790),CH( 326),
CH(220),CH(4973),CH(4253),CH(1774),CH(5528),CH( 326),
CH(221),CH(2647),CH(2481),CH(2822),CH(1780),CH( 326),
};
1.检验部位:
2.检验比例:
3.检验时机:
4.验收标准:
5.合格级别:
*/

const char _Menu_DB[C_LANGUAGE][4][28] =
{
    {
        "请输入试块声程",	//2
        "请输入探头频率",	//3
        "探头频谱测试中",	//4
        "频率：     返回键退出 ",	//5
    },
    {
        "Please input piece range:   ",
        "Please input frequency:     ",
        "Frequency testing           ",
        "Frequency：     ＃exit      ",
    },
};

const char _TestMenu_A1[C_LANGUAGE][7][13] =
{
    {
        "⒈测零点声速 ",
        "⒉测折射角度 ",
        "⒊制作ＤＡＣ ",
        "⒋制作ＡＶＧ ",
        "⒌测仪器性能 ",
        "⒍测缺陷高度 ",
        "⒎测探头频谱 ",
    },
    {
        "1.Zero Test  ",
        "2.Angle Test ",
        "3.DAC Test   ",
        "4.AVG Test   ",
        "5.Performance",
        "6.Flaw High  ",
        "7.Spectrum   ",
    },
};

const u_char _Menu_Choice[C_LANGUAGE][10][22] =
{
    {
        "０数据通讯  ",
        "⒈功能菜单",
        "⒉设置菜单",
        "⒊选项菜单",
        "⒋仪器调校",
        "⒌数据处理",
        "⒍标准设置",
        "⒎色彩设置",
        "⒏波幅dB表",
        "⒐快捷设置",

    },//中文
    {
        "0.Communication       ",
        "1.Function Menu       ",
        "2.Setup Menu          ",
        "3.Option Menu         ",
        "4.Test                ",
        "5.Data Storage        ",
        "6.Standard            ",
        "7.Color Menu          ",
        "8.Amplitude dB        ",
        "9.Hotkey Setup        ",

    },//英文
};
const char _AmplitudedB[C_LANGUAGE][2][30] =
{
    {
        "深度(mm)  增益(dB)     波高(%)",
        "10mm",
    },
    {
        "Deep(mm) Gain(dB)    High(%)  ",
        "10mm  ",
    },
};
const char _TestMenu_Menu[C_LANGUAGE][2][13] =
{
    {
        "⒈自动测试",
        "⒉手动测试",
    },
    {
        "1.Auto    ",
        "2.Manual  ",
    },
};
const u_char _TestMenu_Enter[] = {ENTER};

const u_char _TestMenu_AB1[C_LANGUAGE][4][17] =
{
    {
        "⒈预置工件声速： ",
        "⒉一次回波声程： ",
        "⒊二次回波声程： ",
        "按ぁ键进入测试   ",
    },//中文
    {
        "1.Sound Velocity:",
        "2.1st BeamPath  :",
        "3.2nd BeamPath  :",
        "ぁto start Test  ",
    },//英文
};
/*
1.预置工件声速:
2.一次回波声程：
3.二次回波声程：
按 'z'+ 15 开始测试
*/
const u_char _TestMenu_AB3[C_LANGUAGE][4][85] =
{
    {
        "移动探头使      反射体最高回波  在门内, ぁ确定  ",
        "探头至一次反射体水平距离：                      ",
        "使中心深度      反射体最高回波  在门内, ぁ确定  ",
        "稳住探头使      反射体回波      在门内, ぁ确定  ",
    },
    {
        "Move the probe,  make the highest echo with        in gate.         And then press ぁ",
        "The distance fromprobe to 1st     reflector:                                         ",
        "Make the highest echo with the center depth        in gate.         And then press ぁ",
        "keep the echo with the center depth                in gate.         And then press ぁ",
    }
};

const u_char _TestMenu_AC1[C_LANGUAGE][4][18] =
{
    {
        "⒈目标反射体直径：",
        "⒉反射体中心深度：",
        "⒊标称K 值折射角：",
        "按ぁ键进入测试    ",
    },//中文
    {
        "1.ObjectDiameter: ",
        "2.Object Depth  : ",
        "3.Marking Angle : ",
        "ぁto start test   ",
    },//英文
};
const u_char _TestMenu_DAC[C_LANGUAGE][5][17] =
{
    {
        "⒈  最大深度：   ",
        "⒉反射体直径：   ",
        "⒊反射体长度：   ",
        "按ぁ键进入测试   ",
#if C_DEVTYPE == 14
        "ぁ键测试·键修正 ",
#else
        "ぁ键测试←键修正 ",
#endif
    },//中文
    {
        "1.Maximum Range :",
        "2.ObjectDiameter:",
        "3.Object Length :",
        "ぁto start draw  ",
#if C_DEVTYPE == 14
        "ぁDraw,·Correct ",
#else
        "ぁDraw,←Correct ",
#endif
    },//英文
};

const u_char _TestMenu_AVG[C_LANGUAGE][5][17] =
{
    {
        "⒈  最大深度：   ",
        "⒉反射体直径：   ",
        "⒊  当量显示：   ",
        "按ぁ键进入测试   ",
#if C_DEVTYPE == 14
        "ぁ键测试·键修正 ",
#else
        "ぁ键测试←键修正 ",
#endif
    },//中文
    {
        "1.Maximum Range :",
        "2.ObjectDiameter:",
        "3.Equiv. Display:",
        "ぁto start draw  ",
#if C_DEVTYPE == 14
        "ぁDraw,·Correct ",
#else
        "ぁDraw,←Correct ",
#endif
    },//英文
};
const u_char _TestMenu_set[C_LANGUAGE][3][28] =
{
    {"⒈表面补偿：","⒉工件厚度：","＃返回ぁ完成",},//中文
    {"1.Compensation: ","2.Thickness   : ","＃ReturnぁFinish",},//英文
};


const u_char _TestMenu_AD2[C_LANGUAGE][4][34] =
{
    {
#if C_DEVTYPE == 14
        "左右键选择目标回波＃结束测试",	//2
        "左右键选择回波ぁ确定  ＃返回",	//3
        "左右选波 ·修正 ＃退出 ぁ完成",	//5
        "左右选点上下调整＃清除ぁ完成", //6
#else
        "＋－键选择目标回波＃结束测试",
        "＋－键选择回波ぁ确定  ＃返回",
        "＋－选波 ←修正 ＃退出 ぁ完成",
        "←选点 ＋－调整 ＃清除 ぁ完成",
#endif
    },//中文
    {
#if C_DEVTYPE == 14
        "＜＞Select Echo ＃Exit	           ",
        "＜＞Select Echo ぁConfirm＃Return ",
        "＜＞Select·Correct＃exitぁFinish ",
        "＜＞Select∧∨Adjust＃DelぁFinish ",
#else
        "＋－Select Echo ＃Exit			  ",
        "＋－Select EchoぁConfirm＃Return ",
        "＋－Select←Correct＃ExitぁFinish ",
        "←Select ＋－Adjust ＃DelぁFinish ",
#endif
    },//英文
};

const u_short _TestMenu_AF1[] =
{
    CH(217),CH(2320),CH(4168),CH(4469),CH(4178),CH(2240),CH(326),
    CH(218),CH(2789),CH(5528),CH(1808),CH(2463),CH(2240),CH(326),
    CH(1620),ENTER,CH(2892),CH(3110),CH(4228),CH(1866),CH(4252),
};/*
//1. 校验类型：初校/复校
1.反射体深度：
2.基准波高度：
按'z'+ 15 开始校验

  */
const u_char _TestMenu_AG1[C_LANGUAGE][1][16] =
{
    {"测缺陷高度      ",},
    {"Flaw height test",}
};

const u_char _TestMenu_AG2[C_LANGUAGE][3][73] =
{
    {
        "移探头调参量使缺陷下端 最高波出现在门内",
        "移探头调参量使缺陷上端 最高波出现在门内",
        "测得缺陷高度为：       是否存储波形？Ｙ／Ｎ",
    },
    {
        "Move the probe,make the highest echo of the flawlower tip in gate.       ",
        "Move the probe,make the highest echo of the flawupper tip in gate.       ",
        "The flaw height        save the echo wave? Y/N"
    }
};

const char EpDots[] =
{
    0x00,0x00,0x00,0x03,0x07,0x0E,0x0E,0x1E,
    0x1E,0x1E,0x3E,0x3E,0x3E,0x7E,0x7E,0x7E,
    0x07,0x3F,0xFF,0xCF,0x0F,0x0F,0x0F,0x0F,
    0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,

    0xC0,0xF8,0xFE,0xE7,0xE1,0xE0,0xE0,0xE0,
    0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,
    0x00,0x00,0x00,0x80,0xC0,0xE0,0xE0,0xF0,
    0xF0,0xF0,0xF8,0xF8,0xF8,0xFC,0xFC,0xFC,

    0x7E,0x7E,0x3E,0x3E,0x3E,0x1E,0x1E,0x1E,
    0x0E,0x0E,0x07,0x03,0x00,0x00,0x00,0x00,
    0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x07,0x00,
    0x00,0x00,0xC0,0xF8,0xFF,0x3F,0x07,0x00,

    0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xC0,0x00,
    0x00,0x01,0x07,0x3F,0xFE,0xF8,0xC0,0x00,
    0xFC,0xFC,0xF8,0xF8,0xF8,0xF0,0xF0,0xF0,
    0xE0,0xE0,0xC0,0x80,0x00,0x00,0x00,0x00
};

const u_short company[] = {CH(5149),CH(3310)};//友联

const u_short company2[] = {CH(3647),CH(4508),CH(5149),CH(3310),CH(4293),
                            CH(3475),CH(2828),CH(4285),CH(3110),CH(2302),CH(5148),CH(4762),CH(2511),CH(4330)
                           };
//南通友联数码技术开发有限公司

const u_short product[] = {CH(348),CH(356),CH(353),CH(352),CH(313),CH(345),CH(317)};

const char 	Probe[] =
{
    0x00,0x00,0x07,0x04,0x04,0x04,0x04,0x3f,
    0x00,0x00,0x00,0x01,0x01,0x00,0x00,0x00,
    0x00,0x00,0xf0,0x10,0x10,0x10,0x10,0xfe,
    0x80,0x80,0x80,0xc0,0xc0,0x80,0x00,0x00,

    0x00,0x00,0x07,0x04,0x04,0x04,0x04,0x3f,
    0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0xf0,0x10,0x10,0x10,0x10,0xfe,
    0x00,0x80,0x40,0x20,0x18,0x18,0x00,0x00,

    0x00,0x00,0x3e,0x22,0x22,0x22,0x7f,0x10,
    0x08,0x04,0x02,0x01,0x07,0x00,0x00,0x00,
    0x00,0x00,0x3e,0x22,0x22,0x22,0x7f,0x08,
    0x10,0x20,0x40,0x80,0xe0,0x00,0x00,0x00,

    0x00,0x00,0x03,0x02,0x02,0x0f,0x00,0x00,
    0x00,0x0f,0x02,0x02,0x03,0x00,0x00,0x00,
    0x00,0x00,0xe0,0x20,0x20,0xf8,0x80,0x80,
    0x80,0xf8,0x20,0x20,0xe0,0x00,0x00,0x00
};
const u_char _ParaMenu_H1[C_LANGUAGE][1][40] =
{
    {"          第  页  共  页     "},
    {"Current page   Total page    "}
};

const u_char _ChannelMenu_A1[C_LANGUAGE][9][22] =
{
    {
        "⒈探头类型：",
        "⒉探头频率：",
        "⒊晶片尺寸：",
        "⒋折射角度：",
        "⒌工件声速：",
        "⒍探头前沿：",
        "⒎通道标准：",
        "⒏滤波频带：",
        "⒐检波方式：",
    },//中文
    {
        "1.Probe Type          ",
        "2.Frequency           ",
        "3.Crystal Size        ",
        "4.Angle               ",
        "5.Sound Velocity      ",
        "6.Front Distance      ",
        "7.Standard in Channel ",
        "8.Filter Band         ",
        "9.Echo Rectify        ",
    },//英文
};
const u_char _ChannelMenu_A2[C_LANGUAGE][5][22] =
{
    {
        "⒈探头阻尼：",
        "⒉发射电压：",
        "⒊重复频率：",
        "⒋降噪次数：",
        "⒌脉冲宽度：",
    },//中文
    {
        "1.Damping             ",
        "2.Transmit Voltage    ",
        "3.Repetition Frequency",
        "4.Denoise             ",
        "5.Pulse Breadth       ",
    },//英文
};

const u_char _ChannelMenu_B1[] =
{
    ' ',' ',' ',
    'M','H','z',
    ' ',' ',' ',
    ' ',' ',' ',
    'm','/','s',
    'm','m',' ',
    ' ',' ',' ',
    ' ',' ',' ',
    ' ',' ',' ',
    ' ',' ',' '
};
const u_char _ChannelMenu_B2[] =
{
    ' ',' ',' ',
    ' ',' ',' ',
    ' ',' ',' ',
    ' ',' ',' ',
    ' ',' ',' ',
    ' ',' ',' ',
    ' ',' ',' ',
    ' ',' ',' ',
    ' ',' ',' '
};

const u_char _ParaMenu_B1[] =
{
    ' ',' ',' ',
    'M','H','z',
    ' ',' ',' ',
    ' ',' ',130,
    ' ',' ',' ',
    'm','/','s',
    'm','m',' ',
    'd','B',' ',
    ' ',' ',' '
};

const u_char _ParaMenu_AB1[C_LANGUAGE][3][10] =
{
    {"⒈垂直    ","⒉水平    ","⒊距离    ",},//中文
    {"1.V-Range ","2.H-Range ","3.D-Range ",},//英文
};
const u_char _ParaMenu_AB2[C_LANGUAGE][4][10] =
{
    {"⒈直探头  ","⒉斜探头  ","⒊双晶  ","⒋穿透  ",},//中文
    {"1.Normal  ","2.Angle   ","3.Dual  ","4.Through ",},//英文
};

const u_char _ParaMenu_AB3[] = {"\
               \
1.Epson LQ300K \
2.HP 6L        \
3.Epson C61    \
4.TP UP-NH32P  "
                               };

const u_char _ParaMenu_A2[C_LANGUAGE][9][22] =
{
    {
        "⒈声程标度：",
        "⒉表面补偿：",
        "⒊工件厚度：",
        "⒋工件外径：",
        "⒌当量标准：",
        "⒍当量显示：",
        "⒎判废线RL：",
        "⒏定量线SL：",
        "⒐评定线EL：",
    },//中文
    {
        "1.Scale               ",
        "2.Surface Gain        ",
        "3.Workpiece Thickness ",
        "4.Outer Radius        ",
        "5.Equiv. Reference    ",
        "6.Equiv. Display      ",
        "7.Rejection           ",
        "8.Sizing              ",
        "9.Evalution           ",
    },//英文
};

const u_char _ParaMenu_A22[C_LANGUAGE][9][22] =
{
    {
        "⒈声程标度：",
        "⒉表面补偿：",
        "⒊工件厚度：",
        "⒋工件外径：",
        "⒌当量标准：",
        "⒍当量显示：",
        "⒎ＡＶＧ上：",
        "⒏ＡＶＧ中：",
        "⒐ＡＶＧ下：",
    },//中文
    {
        "1.Scale               ",
        "2.Surface Gain        ",
        "3.Workpiece Thickness ",
        "4.Outer Radius        ",
        "5.Equiv. Reference    ",
        "6.Equiv. Display      ",
        "7.AVG Top             ",
        "8.AVG Middle          ",
        "9.AVG Bottom          ",
    },//英文
};

const u_char _ParaMenu_B2[] =
{
    ' ',' ',' ',
    'd','B',' ',
    ' ',' ',' ',
    ' ',' ',' ',
    ' ',' ',' ',
    ' ',' ',' ',
    'd','B',' ',
    'd','B',' ',
    'd','B',' '
};

const u_char _ParaMenu_B2A1[] =
{
    ' ',' ',' ',
    'd','B',' ',
    ' ',' ',' ',
    ' ',' ',' ',
    ' ',' ',' ',
    ' ',' ',' ',
    'm','m',' ',
    'm','m',' ',
    'm','m',' '
};

const u_char _ParaMenu_AC1[C_LANGUAGE][2][8] =
{
    {"⒈单幅  ","⒉连续  ",},//中文
    {"1.Single","2.Cont. ",},//英文
};
const u_char _ParaMenu_AC7[C_LANGUAGE][2][10] =
{
    {"⒈本机    ","⒉SD卡    ",},//中文
    {"1.Detector","2.SD      ",},//英文
};
const u_char _ParaMenu_AC2[C_LANGUAGE][7][10] =
{
    {
        "⒈母线    ",
        "⒉判废线  ",
        "⒊定量线  ",
        "⒋评定线  ",
        "⒌四号线  ",
        "⒍五号线  ",
        "⒎六号线  ",
    },//中文
    {
        "1.Original",
        "2.RL      ",
        "3.SL      ",
        "4.EL      ",
        "5.4th     ",
        "6.5th     ",
        "7.6th     ",
    },//英文
};

const u_char _infinity[C_LANGUAGE][1][7] =
{
    {
        "∞     ",
    },//中文
    {
        "∞     ",
    },//英文
};
const u_char _ParaMenu_AC3A[C_LANGUAGE][2][8] =
{
    {"⒈ＡＶＧ","⒉当量  ",},//中文
    {"1.AVG   ","2.Equiv.",},//英文
};
const u_char _ParaMenu_AC3[C_LANGUAGE][2][8] =
{
    {"ＤＢ值　","孔径  ",},//中文
    {" DB     ","      ",},//英文
};
const u_char _ParaMenu_AC4[C_LANGUAGE][5][10] =
{
    {"⒈低亮    ","⒉中亮    ","⒊标准    ","⒋高亮    ","⒉高亮    ",},//中文
    {"1.Low     ","2.Middle  ","3.Standard","4.High    ","2.High    ",},//英文
};
const u_char _ParaMenu_AC5[C_LANGUAGE][2][10] =
{
    {"⒈失波门  ","⒉进波门  ",},//中文
    {"1.Negative","2.Positive",},//英文
};
const u_char _ParaMenu_AC6[C_LANGUAGE][2][8] =
{
    {"⒈峰值  ","⒉前沿  ",},//中文
    {"1.Peak  ","2.Front ",},//英文
};
const u_char _MainMenu_C1A11[C_LANGUAGE][2][16] =
{
    {"⒈中文  ","⒉English ",},//中文
    {"⒈中文  ","⒉English ",},//英文
};

const u_char _ParaMenu_A3[C_LANGUAGE][9][22] =
{
    {
//#if C_UDISK == 1
        "⒈记录方式：",
        "⒉记录位置：",
//#else
//		"⒈计量单位：",
//		"⒉语言Lang：",
//#endif
        "⒊位置显示：",
        "⒋语言Lang：",
        "⒌Ｂ门用途：",
        "⒍按键声音：",
        "⒎屏幕亮度：",
        "⒏标准波高：",
        "⒐焊缝图示：",
    },//中文
    {
        "1.Save Mode           ",
        "2.Save Location       ",
        "3.Detection Method    ",
        "4.Language 语言       ",
        "5.Gate B Usage        ",
        "6.Keypad Beep         ",
        "7.Screen Luminance    ",
        "8.Reference Height    ",
        "9.Weld Seam Sketch    ",
    },//英文
};

const u_char _ParaMenu_A5[C_LANGUAGE][2][28] =
{
    {"⒈工件厚度：","⒉工件外径：",},//中文
    {"1.Thickness   : ","2.Outer radius:  ",},//英文
};


const u_char _ParaMenu_A6[C_LANGUAGE][8][22] =
{
    {
        "⒈声程标度：",
        "⒉表面补偿：",
        "⒊工件厚度：",
        "⒋工件外径：",
        "⒌当量标准：",
        "⒍判废线RL：",
        "⒎定量线SL：",
        "⒏评定线EL：",
    },//中文
    {
        "1.Scale               ",
        "2.Surface Gain        ",
        "3.Workpiece Thickness ",
        "4.Outer Radius        ",
        "5.Equiv. Reference    ",
        "6.Rejection           ",
        "7 Sizing              ",
        "8.Evalution           ",
    },//英文
};
const u_char _ParaMenu_A6A1[C_LANGUAGE][8][22] =
{
    {
        "⒈声程标度：",
        "⒉表面补偿：",
        "⒊工件厚度：",
        "⒋工件外径：",
        "⒌当量标准：",
        "⒍ＡＶＧ上：",
        "⒎ＡＶＧ中：",
        "⒏ＡＶＧ下：",
    },//中文
    {
        "1.Scale               ",
        "2.Surface Gain        ",
        "3.Workpiece Thickness ",
        "4.Outer Radius        ",
        "5.Equiv. Reference    ",
        "6.AVG Top             ",
        "7.AVG Middle          ",
        "8.AVG Bottom          ",
    },//英文
};

const u_char _ParaMenu_B6[] =
{
    ' ',' ',' ',
    'd','B',' ',
    ' ',' ',' ',
    ' ',' ',' ',
    ' ',' ',' ',
    'd','B',' ',
    'd','B',' ',
    'd','B',' '
};

const u_char _ParaMenu_B6A1[] =
{
    ' ',' ',' ',
    'd','B',' ',
    ' ',' ',' ',
    ' ',' ',' ',
    ' ',' ',' ',
    'm','m',' ',
    'm','m',' ',
    'm','m',' '
};

const u_char _ParaMenu_B3[] =
{
    ' ',' ',' ',//'m','m',' ',
    ' ',' ',' ',//'m','m',' ',
    ' ',' ',' ',
    ' ',' ',' ',
    ' ',' ',' ',
    ' ',' ',' ',
    ' ',' ',' ',
    '%',' ',' ',
    ' ',' ',' ',
};
#if C_DEVLIB != 3
const u_char _ParaMenu_AD2[C_LANGUAGE][4][18] =
{
    {"⒈全波 ","⒉正半波","⒊负半波","⒋射频 ",},//中文
    {"1.FULL ","2.HALF +","3.HALF -","4.RF   ",},//英文
};
#else
const u_char _ParaMenu_AD2[C_LANGUAGE][2][18] =
{
    {"⒈全波 ","⒉射频 ",},//中文
    {"1.FULL ","2.RF   ",},//英文
};
#endif

const u_char _ParaMenu_AD3_1[] = {"1. LOW   2. MIDDLE3. HIGH  4. WIDE  "};	/*每行9*/

const u_char _ParaMenu_AD3[C_LANGUAGE][4][10] =
{
    {
        "⒈低频 LOW",
        "⒉中频 MID",
        "⒊高频 HIG",
        "⒋宽频 WID",
    },//中文
    {
        "1.Low    ",
        "2.Middle ",
        "3.High   ",
        "4.Wide   ",
    },//英文
};
//const u_char _ParaMenu_AD3[] = {"1. 2.5MHz2. 5.0MHz3. wide  "};	/*每行9*/

const u_char _ParaMenu_AD4[] = {"1. LD/50  2. HD/400  "};		/*每行10*/

#if C_DEVLIB == 23 ||  C_DEVLIB == 24
const u_char _ParaMenu_AD5[C_LANGUAGE][4][18] =
{
    {"⒈低压","⒉中压","⒊标准","⒋高压",},//中文
    {"1.Low ","2.Middle","3.STD","4.High",},//英文
};
//const u_char _ParaMenu_AD5[] = {"1. LV/100V2. ML/200V3. MH/400V4. HV/800V"};		/*每行10*/
#else
const u_char _ParaMenu_AD5[] = {"1. LV/200V2. HV/400V"};		/*每行10*/
#endif

const u_char _ParaMenu_AD8[C_LANGUAGE][2][6] =
{
    {
        "⒈开  ",
        "⒉关  ",
    },//中文
    {
        "1.On  ",
        "2.Off ",
    },//英文
};
const u_char _ParaMenu_AD9[C_LANGUAGE][2][5] =
{
    {
        "⒈否 ",
        "⒉是 ",
    },//中文
    {
        "1.No ",
        "2.Yes",
    },//英文
};
const u_char _ParaMenu_C1[C_LANGUAGE][4][22] =
{
    {
        "⒈选打印机: ",
        "⒉存储频谱：",
        "⒊参量保护：",
        "⒋计量单位：",
    },//中文
    {
        "1.Printer             ",
        "2.Spectrum Save       ",
        "3.Param Protect       ",
        "4.Unit                ",
    },//英文
};

const u_char _ParaMenu_D1[] =
{
    ' ',' ',' ',
    ' ',' ',' ',
    ' ',' ',' ',
    ' ',' ',' ',
    ' ',' ',' ',
};
const u_char _ParaMenu_CA1[] = "1.mm  2.inch";

const u_char _ParaMenu_CC1[] = "0. 0  1. 1  2. 2  3. 3  4. 4  ";
const u_char _ParaMenu_CD1[] = "1. 100  2. 200  3. 300  4. 400  5. 500  6. 600  7. 700  8. 800  9. 900  0. 1000  ";

const u_char _MainMenu_C1A1[C_LANGUAGE][4][16] =
{
    {"垂直","水平","距离","刻度",},//中文
    {"V-Range","H-Range","D-Range","Scale  ",},//英文
};
const u_char _MainMenu_C1A3[C_LANGUAGE][2][16] =
{
    {"窄","宽",},//中文
    {"Narrow ","Wide ",},//英文
};

const u_char _MainMenu_C1A4[C_LANGUAGE][4][16] =
{
    {"直探头","斜探头","双晶 ","穿透 ",},//中文
    {"Normal","Angle ","Dual  ","Through ",},//英文
};
const u_char _Weld_A1[C_LANGUAGE][1][40] =
{
    {"焊缝图示  第  页  共  页     "},
    {"Current page   Total page    "}
};
const u_char _Weld_A2[C_LANGUAGE][1][40] =
{
    {"探头前端到焊缝边的距离： "},
    {"The dis. from probe to seam:"}
};
const u_char _Weld_B1[C_LANGUAGE][9][22] =
{
    {
        "⒈焊缝类型：          ",
        "⒉探测面 ：           ",
        "⒊翼板厚度：          ",
        "⒋腹板厚度：          ",
        "⒌Ａ面焊脚：          ",
        "⒍Ｂ面焊脚：          ",
        "⒎Ａ端余高：          ",
        "⒏Ｂ端余高：          ",
        "⒐Ａ坡口角：          ",
    },
    {
        "1.Weld Seam Type      ",
        "2.Detecting Surface   ",
        "3.Flange Thickness    ",
        "4.Web Thickness       ",
        "5.A Weld Foot         ",
        "6.B Weld Foot         ",
        "7.Reinforcement A     ",
        "8.Reinforcement B     ",
        "9.Groove Angle A      ",
    }
};
const u_char _Weld_B12[C_LANGUAGE][4][22] =
{
    {
        "⒈Ｂ坡口角：          ",
        "⒉Ａ坡口深：          ",
        "⒊钝边高度：          ",
        "⒋根部间隙：          ",
    },
    {
        "1.Bevel Angle B       ",
        "2.Bevel A Depth       ",
        "3.Obtuse Side Height  ",
        "4.Gap Of Root         ",
    }
};

const u_char _Weld_B2[C_LANGUAGE][9][22] =
{
    {
        "⒈焊缝类型：          ",
        "⒉探测面 ：           ",
        "⒊工件厚度：          ",
        "⒋有效厚度：          ",
        "⒌焊缝宽度：          ",
        "⒍根部半径：          ",
        "⒎上端余高：          ",
        "⒏下端余高：          ",
        "⒐坡口角度：          ",
    },
    {
        "1.Weld Seam Type      ",
        "2.Detecting Surface   ",
        "3.Whole Thickness     ",
        "4.Effective Ply       ",
        "5.Weld Width          ",
        "6.Radius Of Root      ",
        "7.Top Reinforcement   ",
        "8.Root Reinforcement  ",
        "9.Groove Angle        ",
    }
};
const u_char _Weld_B22[C_LANGUAGE][7][22] =
{
    {
        "⒈坡口面角：          ",
        "⒉坡口深度：          ",
        "⒊钝边高度：          ",
        "⒋根部间隙：          ",
        "⒌坡内角度：          ",
        "⒍坡内面角：          ",
        "⒎坡内深度：          ",
    },
    {
        "1.Groove Side Angle   ",
        "2.Bevel Depth         ",
        "3.Obtuse Side Height  ",
        "4.Gap Of Root         ",
        "5.Inside Angle        ",
        "6.Inside side Angle   ",
        "7.Inside Depth        ",
    }
};

const u_char _Weld_C1[C_LANGUAGE][4][10] =
{
    {"⒈无      ","⒉单面    ","⒊双面    ","⒋Ｔ型    ",},
    {"1.Null    ","2.Single  ","3.Double  ","4.T-type  ",}
};

const u_char _Weld_D1[C_LANGUAGE][3][13] =
{
    {"⒈Ａ面       ","⒉Ｂ面       ","⒊Ｃ面       ",},
    {"1.Surface A  ","2.Surface B  ","3.Surface C  ",}
};

const u_short _OptionMenu_B1[] =
{
    CH(217),CH(4189),CH(1944),CH(1774),CH(2240),CH(326),
    CH(218),CH(1777),CH(3570),CH(1825),CH(1905),CH(326),
    CH(219),CH(3833),CH(3627),CH(3333),CH(2240),CH(326),
    CH(220),CH(4901),CH(2082),CH(5101),CH(2790),CH(326),
    CH(221),CH(2117),CH(3331),CH(1774),CH(5528),CH(326),
    CH(222),CH(2876),CH(1808),CH(2329),CH(4229),CH(326),
    CH(223),CH(3443),CH(1808),CH(3821),CH(2088),CH(326),
    CH(224),CH(4429),CH(4523),CH(5572),CH(3665),CH(326),
    CH(225),CH(2302),CH(4168),CH(2171),CH(4925),CH(326),
};
/*
1.	声程标度：垂直\\水平\\距离2025 5417 4314 3829 3064 3275
2.	表面补偿：dB
3．	屏幕亮度：低亮\一般\中亮\高亮
4．	选打印机：
5．	当量标准：母线\判废\定量\评定
6．	检波方式：全波\ 正半波\ 负半波\ 射频
7． 滤波频带：2.5\5.0\10\wide
8．	探头阻尼：50Q\400Q
9．	发射电压：200V\400V
*/
const u_char _OptionMenu_C1[] =
{
    ' ',' ',' ',
    'd','B',' ',
    ' ',' ',' ',
    ' ',' ',' ',
    ' ',' ',' ',
    ' ',' ',' ',
    ' ',' ',' ',
    ' ',' ',' ',
    ' ',' ',' ',
};

const u_char _ParaProtect1[C_LANGUAGE][5][24] =
{
    {
        "  参量保护              ",
        "０全部数据：            ",
        "⒈定位参数：            ",
        "⒉定量设置：            ",
        "⒊通道参数：            ",
    },
    {
        "Param Protect           ",
        "0.All Date      :       ",
        "1.Location      :       ",
        "2.Quantification:       ",
        "3.Parameter     :       ",
    }
};
const u_char _ParaProtect2[C_LANGUAGE][2][8] =
{
    {"无　    ","保护    ",},
    {"Null    ","Protect ",}
};
const u_char _ParaMenu_AD6[C_LANGUAGE][4][4] =
{
    {"无  ","部分","部分","全部",},//中文
    {"Null","Part","Part","All ",},//英文
};
const u_char _DataMenu_A1[C_LANGUAGE][2][20] =
{
    {
        "⒈数据通讯",
        "⒉数据处理",
    },//中文
    {
        "1.Print     Screen  ",
        "2.Data      Storage ",
    },//英文
};
const char _DataMenu_A2[C_LANGUAGE][10][10] =
{
    {"查找      ","⒈日期    ","⒉编号    ","⒊隐藏    ","⒋清除    ","⒌通讯    ","⒍调用    ","⒎频谱    ","⒎输出    ","⒏频谱    ",},
    {"REVIEW    ","1.Date    ","2.ID      ","3.Hide    ","4.Delete  ","5.Comm.   ","6.Recall  ","7.Spec.   ","7.Output  ","8.Spec.   ",}
};

#if C_DEVTYPE == 14
const u_short _DataMenu_C1[] =
{
    CH(3312),CH(4888),CH(2839),CH(3428),CH(3110),CH(4228),',',CH(1620),CH(1703),CH(2070),CH(2892),CH(2965),CH(4288)
};//连续记录开始,按保存键结束  1703 2070
#else
const u_short _DataMenu_C1[] =
{
    CH(3312),CH(4888),CH(2839),CH(3428),CH(3110),CH(4228),',',CH(1620),CH(2839),CH(3428),CH(2892),CH(2965),CH(4288)
};//连续记录开始,按记录键结束
#endif
const u_short _DataMenu_C2[] =
{
    CH(1805),CH(2337),CH(3312),CH(4888),CH(2839),CH(3428),CH(1808),CH(4846)
};//播放连续记录波形

const unsigned short _DataMenu_D1[] =
{
    CH(217),CH(1764),CH(2637),CH(326),
    CH(218),CH(2463),CH(2240),CH(326),
    CH(219),CH(1904),CH(2240),CH(326),
};/*
1.编号：
2.高度：
3.长度：
/ 存储
  //*/
const char _DataMenu_E1[C_LANGUAGE][2][22] =
{
    {"覆盖当前通道的数据?Y/N",},
    {"Channel overwrite?Y/N ",}
};
const char _Message_A1[C_LANGUAGE][31][22] =
{
    {
        "打印机未备好",//0
        "电量不足    ",//1
        "无缺陷记录  ",//2
        "参量已达极值",//3
        "数据取值不当",//4
        "存储数据已清",//5
        "内存容量已满",//6
        "数据不能变动",//7
        "未用标准试块",//8
        "请先测试探头",//9
        "数据不存在  ",//10
        "开始自动存储",//11
        "停止自动存储",//12
        "开始数据传送",//13
        "数据已存储  ",//14
        "            ",//15
        "探伤中请操作",//16
        "数据正在清除",//17
        "自动增益    ",//18
        "正在连接SD卡",//19
        "SD卡连接成功",//20
        "SD卡连接失败",//21
        "连续记录中  ",//22
        "SD卡断开连接",//23
        "正在安全断开",//24
        "SD卡容量已满",//25
        "SD卡即将存满",//26
        "SD卡正在待机",//27
        "SD卡正在待机",//28
        "SD正在格式化",//29
        "SD格式化成功",//30
    },
    {
        "No printer            ",//0
        "Battery low           ",//1
        "No record             ",//2
        "Parameter limit       ",//3
        "Value invalid         ",//4
        "Record cleared        ",//5
        "Memory is full        ",//6
        "Data change forbidden ",//7
        "                      ",//8
        "Test probe first      ",//9
        "Record does not exist ",//10
        "Start auto store      ",//11
        "Stop auto store       ",//12
        "                      ",//13
        "Data stored           ",//14
        "                      ",//15
        "Detecting...          ",//16
        "Deleting record       ",//17
        "Automatic Gain        ",//18
        "SD connecting         ",//19
        "Successful connection ",//20
        "Connection failed     ",//21
        "Continuous REC        ",//22
        "SD disconnected       ",//23
        "Disconnecting         ",//24
        "SD capacity full      ",//25
        "SD will be full       ",//26
        "SD will be stand by   ",//27
        "SD will be stand by   ",//28
        "SD formatting         ",//29
        "Formatted successfully",//30
    }
};


const u_char _Message_B1[C_LANGUAGE][15][24] =
{
    {
        "清除存储数据？Ｙ／Ｎ",
        "先测零点声速？Ｙ／Ｎ",
        "先测零点Ｋ值？Ｙ／Ｎ",
        "清除当前数据？Ｙ／Ｎ",
        "退出当前测试？Ｙ／Ｎ",
        "存储测试数据？Ｙ／Ｎ",
        "存储门内包络？Ｙ／Ｎ",
        "存储门内峰值？Ｙ／Ｎ",
        "清除当前通道？Ｙ／Ｎ",
        "清除所有通道？Ｙ／Ｎ",
        "清除探伤数据？Ｙ／Ｎ",
        "清除仪器？Ｙ／Ｎ    ",
        "保存数据？Ｙ／Ｎ    ",
        "清除连续数据？Ｙ／Ｎ",
        "已插入SD卡？Ｙ／Ｎ  ",
    },
    {
        "Delete record?Y/N       ",
        "Test probe zero?Y/N     ",
        "Test zero & angle?Y/N   ",
        "Delete record?Y/N       ",
        "Exit?Y/N                ",
        "Save record?Y/N         ",
        "Save envelope?Y/N       ",
        "Save peak?Y/N           ",
        "Clear current CH?Y/N    ",
        "Clear all channel?Y/N   ",
        "Clear flaw record?Y/N   ",
        "Clear all data?Y/N      ",
        "Save record?Y/N         ",
        "Clear ContinuousData?Y/N",
        "SD inserted?Y/N         ",
    },
};

const u_char _SaveMenu[C_LANGUAGE][4][12] =
{
    {
        "⒈新加标签 ",
        "⒉暂停存储 ",
        "⒊结束存储 ",
        "⒋存储信息 ",
    },
    {
        "1.New label ",
        "2.Pause save",
        "3.Stop save ",
        "4.SD info   ",
    }
};
const u_char _UDiskMenu[C_LANGUAGE][6][30] =
{
    {
        "当前文件           字节       ",
        "已用空间           字节       ",
        "可用空间           字节       ",
        "容量               字节       ",
        "预计可存储时间    小时  分  秒",
        "已用                          ",
    },
    {
        "Current             Byte      ",
        "Used                Byte      ",
        "Free                Byte      ",
        "Capacity            Byte      ",
        "Estimated time     h     m   s",
        "Used                          ",
    }
};

const u_char _PushMenu[C_LANGUAGE][4][12] =
{
    {
        "⒈继续存储 ",
        "⒉移除ＳＤ ",
    },
    {
        "1.Continue  ",
        "2.Remove    ",
    }
};


const u_short _Message_B2[] =
{
    CH(331),CH(389),CH(315),CH(378)
};
//?y/n
const u_char _FuncHotKey_A[C_LANGUAGE][5][22] =
{
    {
        "０默认设置  ",
        "⒈快捷键１：",
        "⒉快捷键２：",
        "⒊快捷键３：",
        "⒋快捷键４：",
    },//中文
    {
        "0.Default  ",
        "1.Hot key 1",
        "2.Hot key 2",
        "3.Hot key 3",
        "4.Hot key 4",
    },//英文
};
const u_char _FuncHotKey_B[C_LANGUAGE][11][22] =
{
    {
        "1.初始化  ",
        "2.回波包络",
        "3.峰值记忆",
        "4.ＤＡＣ门",
        "5.深度补偿",
        "6.实心波形",
        "7.门内报警",
        "8.数据处理",
        "9.波形冻结",
		"0.TOFD",
    },//中文
    {
        "1.Initialization ",
        "2.Echo Envelope  ",
        "3.Peak Memory    ",
        "4.DAC Gate       ",
        "5.TCG            ",
        "6.Solid Wave     ",
        "7.Alarm in Gate  ",
        "8.Data Storage   ",
        "9.Freeze         ",
		"0.TOFD         ",
    },
};
const u_char _FuncMenu_A1[C_LANGUAGE][11][22] =
{
    {
        "确定键生效",
        "０初始化  ",
        "⒈回波包络",
        "⒉峰值记忆",
        "⒊门内报警",
        "⒋ＤＡＣ门",
        "⒌深度补偿",
        "⒍曲面修正",
        "⒎门内展宽",
        "⒏实心波形",
        "⒐TOFD",
    },//中文
    {
        "Press Enter           ",
        "0.Initialization      ",
        "1.Echo Envelope       ",
        "2.Peak Memory         ",
        "3.Alarm in Gate       ",
        "4.DAC Gate            ",
        "5.TCG                 ",
        "6.C S Amend           ",
        "7.Zoom in Wave        ",
        "8.Solid Wave          ",
        "9.TOFD                ",
    },//英文
};
const char _FuncMenu_A2[C_LANGUAGE][20][21] =
{
    {"初始化    ","⒈当前通道","⒉所有通道","⒊缺陷数据","⒋仪器    ","⒌设置时间","⒍SD格式化"},
    {"Initialization","1.Channel","2.All Channel","3.Flaw Record","4.Detector","5.Time setup","6.Format SD",}
};

const u_short _FuncMenu_B1[] =
{
    CH(2504),CH(2894),CH(2681),CH(2240),CH(326),CH(100),CH(100),CH(100),'m','m'
};
/*工件厚度：   mm	/9
*/
const u_short _FuncMenu_B2[] =
{
    CH(5038),CH(4429),CH(4523),CH(2187),CH(1846),CH(3331),CH(4225),CH(4017),CH(4761),CH(4147),CH(2243),
    CH(5578),CH(2463),CH(1808),CH(2006),CH(5258),CH(3537),CH(3658),'E','N', CH(4023),CH(4047),CH( 100),
    CH(5038),CH(4429),CH(4523),CH(2187),CH(1846),CH(3331),CH(4225),CH(4017),CH(4761),CH(4734),CH(2243),
    CH(5578),CH(2463),CH(1808),CH(2006),CH(5258),CH(3537),CH(3658),'E','N', CH(4023),CH(4047),CH( 100)
};
/*移探头调参量使缺陷上端  11*2
  最高波处在门内  EN确认
  移探头调参量使缺陷下端
  最高波处在门内  EN确认
*/

const u_short _FuncMenu_B3[] =
{
    CH(4017),CH(4761),CH(2463),CH(2240),CH( 326),CH( 100),CH( 100),CH( 100),'m','m',
    CH(2070),CH(4017),CH(4761),CH(1808),CH( 331),CH( 389),CH( 315),CH( 378),CH( 100)
};
/*缺陷高度：   mm	9*2
  存缺陷波?Y/N
*/

const u_short _FuncMenu_C1[] =
{
    CH(1825),CH(1905),CH(5421),CH(326),CH(100),CH(100),'d','B',
};//补偿值

const u_char _MenuPrompt_B1[C_LANGUAGE][1][20] =
{
    {"ぁ确定生效＃返回放弃"},
    {"ぁ  Enter ＃  Return"},
};

const char _STDMenu_A1[C_LANGUAGE][11][21] =
{
    {
    "⒈标准类型：",
    "⒉曲线根数：",
    "⒊试块类型：",
    "⒋工件厚度：",
    "⒌第一根线：",
    "⒍第二根线：",
    "⒎第三根线：",
    "⒏第四根线：",
    "⒐第五根线：",
    "０第六根线：",
    "⒊检测等级：",
    },
    {
    "1.Standard Type",
    "2.Curve Number ",
    "3.Block Type   ",
    "4.Thickness    ",
    "5.1st Curve    ",
    "6.2nd Curve    ",
    "7.3rd Curve    ",
    "8.4th Curve    ",
    "9.5th Curve    ",
    "0.6th Curve    ",
    "3.Test Grade   ",
    }
};
const char _STDMenu_A2[C_LANGUAGE][11][21] =
{
    {
    "⒈标准类型：",
    "⒉曲线根数：",
    "⒊缺陷范围：",
    "⒋工件厚度：",
    "⒌验收曲线：",
    "⒍记录曲线：",
    "⒎评定曲线：",
    "⒏第四根线：",
    "⒐第五根线：",
    "０第六根线：",
    "⒊检测等级：",
    },
    {
    "1.Standard Type",
    "2.Curve Number ",
    "3.Defect Size  ",
    "4.Thickness    ",
    "5.Acceptance   ",
    "6.Record Levels",
    "7.Evaluation   ",
    "8.4th Curve    ",
    "9.5th Curve    ",
    "0.6th Curve    ",
    "3.Test Grade   ",
    }
};
const char _STDMenu_A3[C_LANGUAGE][11][21] =
{
    {
    "⒈标准类型：",
    "⒉曲线根数：",
    "⒊缺陷范围：",
    "⒋工件厚度：",
    "⒌验收线一：",
    "⒍验收线二：",
    "⒎评定曲线：",
    "⒏第四根线：",
    "⒐第五根线：",
    "０第六根线：",
    "⒊检测等级：",
    },
    {
    "1.Standard Type",
    "2.Curve Number ",
    "3.Defect Size  ",
    "4.Thickness    ",
    "5.Acceptance  1",
    "6.Acceptance  2",
    "7.Evaluation   ",
    "8.4th Curve    ",
    "9.5th Curve    ",
    "0.6th Curve    ",
    "3.Test Grade   ",
    }
};
const char _STDMenu_A4[C_LANGUAGE][11][21] =
{
    {
    "⒈标准类型：",
    "⒉曲线根数：",
    "⒊缺陷范围：",
    "⒋工件厚度：",
    "⒌验收线一：",
    "⒍验收线二：",
    "⒎验收线三：",
    "⒏评定曲线：",
    "⒐第五根线：",
    "０第六根线：",
    "⒊检测等级：",
    },
    {
    "1.Standard Type",
    "2.Curve Number ",
    "3.Defect Size  ",
    "4.Thickness    ",
    "5.Acceptance  1",
    "6.Acceptance  2",
    "7.Acceptance  3",
    "8.Evaluation   ",
    "9.5th Curve    ",
    "0.6th Curve    ",
    "3.Test Grade   ",
    }
};
const char _STDMenu_A5[C_LANGUAGE][11][21] =
{
    {
    "⒈标准类型：",
    "⒉曲线根数：",
    "⒊缺陷范围：",
    "⒋工件厚度：",
    "⒌母线：    ",
    "⒍评定曲线：",
    "⒎第三根线：",
    "⒏第四根线：",
    "⒐第五根线：",
    "０第六根线：",
    "⒊检测等级：",
    },
    {
    "1.Standard Type",
    "2.Curve Number ",
    "3.Defect Size  ",
    "4.Thickness    ",
    "5.Original Line",
    "6.Evaluation   ",
    "7.3rd Curve    ",
    "8.4th Curve    ",
    "9.5th Curve    ",
    "0.6th Curve    ",
    "3.Test Grade   ",
    }
};

const u_short _KEYLOCK_A1[] =
{
    CH(1620),CH( 104),CH(2892),CH(4388),CH(2892),CH(3744)
};//按点键锁键盘
const u_short _KEYLOCK_A2[] =
{
    CH(1620),CH( 104),CH(2892),CH(2966),CH(4388),CH( 100)
};//按点键解锁
const u_short _KEYLOCK_A3[] =
{
    CH(3975),CH(4740),CH(2966),CH(2892),CH(3744),CH(4388)
};
const u_char _KEYLOCK_A1_E[] =
{
    "Now press . !"
};//按点键锁键盘
const u_char _KEYLOCK_A2_E[] =
{
    "Now press . !"
};//按点键解锁

const u_char _KEYLOCK_A3_E[] =
{
    "Keys are locked!"
};
//请先解键盘锁
#define C_STD_MAX 13//10
#define C_STD_LEN 22
#define C_BLOCK_MAX 6
#define C_BLOCK_LEN 15
#define C_STD_POS_THNUM	0
#define C_STD_POS_LINENUM 1
#define C_STD_POS_THICK	2

const u_char _STDMenu_AB1[] = {"\
1.Custom              \
2.GB/T11345-89        \
3.JB/T4730-2005       \
4.JG/T203-2007        \
5.SY/T4109-2013       \
6.JIS Z3060           \
7.ASME-V              \
8.CB/T3559-2011       \
9.DL/T820-2002        \
1.AWS D1.1/D1.1M      \
2.GB/T11345-2013 T1LV2\
3.GB/T11345-2013 T1LV3\
4.NB/T47013-2015    "
                              };

const u_char _STDMenu_B1[][C_BLOCK_LEN * C_BLOCK_MAX+1] =
{
    {
        "1\
1.Custom       "
    },
    {
        "3\
1.RB-X   A     \
2.RB-X   B     \
3.RB-X   C     "
    },//2.GB/T11345-89
    {
        "5\
1.CSK-IIIA     \
2.CSK-IIA      \
3.CSK-IVA      \
4.GS-X         \
5.AL           "
    },//3.JB4730.3
    {
        "3\
1.A            \
2.B	           \
3.C            "
    },//4.JG/T203-2007
    {
        "1\
1.SGB          "
    },//5.SY 4065-93
    {
        "1\
1.Custom       "
    },//JIS
    {
        "1\
1.Custom       "
    },//ASME-3
    {
        "1\
1.CTRB         "
    },//CB/T3559-2011
    {
        "3\
1.RB-X A       \
2.RB-X B       \
3.RB-X C       "
    },//9.DL/T820 2002
    {
        "1\
1.Custom       "
    },//0.aws
{"5\
1.UNKNOW       \
2.IGNORE       \
3.L <= T/2     \
4.L <= T       \
5.L >  T       "},//GB/T11345-2013
{"5\
1.UNKNOW       \
2.IGNORE       \
3.L <= T/2     \
4.L <= T       \
5.L >  T       "},//GB/T11345-2013
{"5\
1.CSK-IIA      \
2.CSK-IIIA     \
3.CSK-IVA      \
4.GS-X         \
5.AL           "},//47013
} ;

const short c_DACLine[][C_BLOCK_MAX][C_STD_POS_THICK+7*C_BLOCK_MAX] =
{
    {
        {
            2, 3,
            300,	0,		0,		0,	    0,	0,	0,
            1000,	0,		0,		0,		0,	0,	0,
        },//
    },//OTHERS
    {
        {
            3, 3,
            7,		0,		0,	0,	0,	0,	0,
            50,		0,		-100,	-160,	-220,	-280,	-340,
            1000,	0,		0,		0,		0,	0,	0,
        },//	A/C
        {
            3, 3,
            7,		0,		0,	0,	0,	0,	0,
            300,		-40,	-100,	-160,	-220,	-280,	-340,
            1000,	0,		0,		0,		0,	0,	0,
        },//	B/C
        {
            3, 3,
            7,		0,		0,	0,	0,	0,	0,
            300,	-20,	-80,	-140,	-200,	-260,	-320,
            1000,	0,		0,		0,		0,	0,	0,
        },//	B/C
    },//GB/T11345-89
    {
        {
            4, 3,
            15,		20,		-60,	-120,	-180,	-240,	-300,
            46,		50,		-30,	-90,	-150,	-210,	-270,
            120,	100,	0,		-60,	-120,	-180,	-240,
            1000,	0,		0,		0,		0,	0,	0,
        },//CSK-IIIA
        {
            3, 3,
            46,		-40,	-120,	-180,	-240,	-300,	-360,
            120,	20,		-80,	-140,	-200,	-260,	-320,
            1000,	0,		0,		0,		0,	0,	0,
        },//CSK-IIA
        {
            2,3,
            400,	0,		-100,	-160,	-220,	-280,	-340,
            1000,	0,		0,		0,		0,		0,		0,
        },//CSK-IVA
        {
            4, 3,
            8,		-100,	-160,	-160,	-160,	-160,	-160,
            15,		-70,	-130,	-160,	-160,	-160,	-160,
            100,	-40,	-100,	-160,	-160,	-160,	-160,
            1000,	0,		0,		0,		0,		0,		0,
        },//GS-X
        {
            2, 3,
            80,		-40,	-120,	-180,	-240,	-300,	-360,
            1000,	0,		0,		0,		0,		0,		0,
        },//铝－2
    },//JB4730-94
    {

		{	3,3,
			3,		0,		0,	    0,	    0,	    0,	    0,
			50,		0,		-100,	-160,	-220,	-280,	-340,
			1000,	0,		0,		0,		0,	0,	0,
		},
		{	3,3,
			3,		0,		0,	    0,	    0,	    0,	    0,
			300,	-40,	-100,	-160,	-220,	-280,	-340,
			1000,	0,		0,		0,		0,	0,	0,
		},
		{	3,3,
			3,		0,		0,	    0,	    0,	    0,	    0,
			300,	-20,	-80,	-140,	-220,	-280,	-340,
			1000,	0,		0,		0,		0,	    0,	    0,
		},//
    },//JG/T203-2007
    {
        {
            2, 3,
            300,	-20,	-80,	-140,	-60,	-120,	-180,
            1000,	0,		0,		0,		0,	0,	0,
        },//
    },//5.SY 4065-93
    {
        {
            2, 6,
            300,	180,	120,	60,		0,	-60,	-120,
            1000,	0,		0,		0,		0,	0,	0,
        },//
    },//JIS
    {
        {
            2, 3,
            300,	0,		-60,	-140,	-200,	-260,	-320,
            1000,	0,		0,		0,		0,	0,	0,
        },//
    },//ASME-3
    {
        {
            3, 3,
            50,		0,		-100,	-160,	-220,	-280,	-340,
            100,	20,		-60,	-120,	-180,	-240,	-300,
            1000,	20,		-20,	-80,		0,	0,	0,
        },//
    },//CB/T3559-2011
    {
        {
            3,3,
            13,		0,		0,		0,		0,	0,	0,
            50,		0,		-100,	-160,	-220,	-280,	-340,
            1000,	0,		0,		0,		0,	0,	0,
        },
        {
            3,3,
            13,		0,		0,		0,		0,	0,	0,
            160,	-40,	-100,	-160,	-220,	-280,	-340,
            1000,	0,		0,		0,		0,	0,	0,
        },
        {
            3,3,
            13,		0,		0,		0,		0,	0,	0,
            160,	-20,	-80,	-140,	-220,	-280,	-340,
            1000,	0,		0,		0,		0,	0,	0,
        },//
    },//0.DL/T820 2002
    {
        {
            2, 3,
            300,	0,		0,		0,	    0,	0,	0,
            1000,	0,		0,		0,		0,	0,	0,
        },//
    },//AWS
    {
		{
			1, 2,
			1000,0,-140,0,0,0,0,
		},//UNKNOW
		{
			4,4,
			8,0,0,0,0,0,0,
			15,-40,-100,-140,-140,0,0,
			100,0,-60,-100,-140,0,0,
			1000,0,0,0,0,0,0,
		},//IGNORE
		{
			4,3,
			8,0,0,0,0,0,0,
			15,-40,-80,-140,0,0,0,
			100,0,-40,-140,0,0,0,
			1000,0,0,0,0,0,0,
		},//L<=T/2
		{
			4,3,
			8,0,0,0,0,0,0,
			15,-40,-80,-140,0,0,0,
			100,-60,-100,-140,0,0,0,
			1000,0,0,0,0,0,0,
		},//L<=T
		{
			4,3,
			8,0,0,0,0,0,0,
			15,-100,-140,-140,0,0,0,
			100,-100,-140,-140,0,0,0,
			1000,0,0,0,0,0,0,
		},//L>T
	},//验收等级2
	{
		{
			1,2,
			1000,0,-100,0,0,0,0,
		},//UNKNOW只显示评定等级线
		{
			4, 4,
			8,	0,		0,		0,		0,	0,	0,
			15,	        0,    -60,	   -100,     -100,     0,      0,
			100,		40,	  -20,     -60,		  -100,	 0,     0,
			1000,	0,		0,		0,		0,	0,	0,
		},//IGNORE显示所有验收等级线和评定等级线
		{
			4,3,
			8,0,0,0,0,0,0,
			15,0,-40,-100,0,0,0,
			100,40,0,-100,0,0,0,
			1000,0,0,0,0,0,0,
		},//L<=T/2显示相应的验收等级线、记录等级线和评定等级线
		{
			4,3,
			8,0,0,0,0,0,0,
			15,0,-40,-100,0,0,0,
			100,-20,-60,-100,0,0,0,
			1000,0,0,0,0,0,0,
		},//L<=T
		{
			4,3,
			8,0,0,0,0,0,0,
			15,-60,-100,-100,0,0,0,
			100,-60,-100,-100,0,0,0,
			1000,0,0,0,0,0,0,
		},//L>T
	},//验收等级3
	{
        {
            5, 3,
            6,		-40,  -120,     -180,   -240,	-300,	-360,//后三个数为自己添加，在设置偏移量时要求依次递减
            40,		-40,  -120,     -180,   -240,	-300,	-360,
            100,	20,	  -80,      -140,	-200,	-260,	-320,
            200,	60,	  -40,		-100,	-160,	-220,	-300,
            1000,    0,	  0,		0,		0,	    0,	    0,

        },//CSK-IIA
        {
            5, 3,
            8,		 20,	-60,	-120,	-180,	-240,	-300,
            15,	     20,	-60,	-120,	-180,	-240,	-300,
            40,	     50,	-30,	-90,	-150,	-210,	-270,
            120,	 100,	  0,	-60,	-120,    -180,	-240,
            1000,     0,	  0,		0,	   0,	    0,	   0,
        },//CSK-IIIA
        {
            4,3,
            200,    0,	  0,		0,		0,	    0,	    0,
            300,    30,  -70,      -130,   -190,   -250,  -310,
            500,    50,  -50,      -110,   -170,   -230,  -290,
            1000,   0,	  0,		0,		0,	    0,	    0,
        },//CSK-IVA
        {
            4, 3,
            8,		-120,	-180,	-240,	-240,	-240,	-240,
            15,		-80,	-140,	-200,	-200,	-200,	-200,
            100,	-40,	-100,	-160,	-160,	-160,	-160,
            1000,	0,		0,		0,		0,		0,		0,
        },//GS-X
        {
            2, 3,
            80,		-40,	-120,	-180,	-240,	-300,	-360,
            1000,	0,		0,		0,		0,		0,		0,
        },//铝－2

    },//47013
};

const u_short _COMM_B1[] =
{
    CH(4293),CH(3061),CH(5393),CH(5258),CH(2011),CH(4268),CH( 312),CH(3975),CH(2140),CH(2093),'.','.','.',' '
};//数据正在传输，请等待

const u_char _Bright_A1[C_LANGUAGE][2][18] =
{
    {
#if C_DEVTYPE==14
        "左右键调节亮度",
#else
        "＋－键调节亮度",
#endif
    },
    {
#if C_DEVTYPE==14
        "＜＞Set Brightness",
#else
        "＋－Set Brightness",
#endif
    }
};
const u_char _Bscan[C_LANGUAGE][4][32] =
{
    {
        "扫查结束按任意键退出            ",
        "工件底波稳定时确定              ",
        "请在工件上移动探头              ",
        "⒈工件厚度：                    ",
    },
    {
        "Finish scan,press any key exit  ",
        "Keep back wall stable           ",
        "Move the probe                  ",
        "1.Thickness:                    ",
    }
};
const unsigned short _BSCAN_B2[] =
{
    CH(217),CH(2504),CH(2894),CH(2681),CH(2240),CH(326),
};//工件厚度：工件长度：



const short sin_tab[] =
{
    0,  123,  245,  368,  491,  613,  736,  858,  980, 1102, 1224, 1346, 1467, 1589, 1710, 1830,
    1951, 2071, 2191, 2311, 2430, 2549, 2667, 2785, 2903, 3020, 3137, 3253, 3369, 3484, 3599, 3713,
    3827, 3940, 4052, 4164, 4276, 4386, 4496, 4605, 4714, 4822, 4929, 5035, 5141, 5246, 5350, 5453,
    5556, 5657, 5758, 5858, 5957, 6055, 6152, 6249, 6344, 6438, 6532, 6624, 6716, 6806, 6895, 6984,
    7071, 7157, 7242, 7327, 7410, 7491, 7572, 7652, 7730, 7807, 7883, 7958, 8032, 8105, 8176, 8246,
    8315, 8382, 8449, 8514, 8577, 8640, 8701, 8761, 8819, 8876, 8932, 8987, 9040, 9092, 9142, 9191,
    9239, 9285, 9330, 9373, 9415, 9456, 9495, 9533, 9569, 9604, 9638, 9670, 9700, 9729, 9757, 9783,
    9808, 9831, 9853, 9873, 9892, 9909, 9925, 9939, 9952, 9963, 9973, 9981, 9988, 9993, 9997, 9999,
    10000, 9999, 9997, 9993, 9988, 9981, 9973, 9963, 9952, 9939, 9925, 9909, 9892, 9873, 9853, 9831,
    9808, 9783, 9757, 9729, 9700, 9670, 9638, 9604, 9569, 9533, 9495, 9456, 9415, 9373, 9330, 9285,
    9239, 9191, 9142, 9092, 9040, 8987, 8932, 8876, 8819, 8761, 8701, 8640, 8577, 8514, 8449, 8382,
    8315, 8246, 8176, 8105, 8032, 7958, 7883, 7807, 7730, 7652, 7572, 7491, 7410, 7327, 7242, 7157,
    7071, 6984, 6895, 6806, 6716, 6624, 6532, 6438, 6344, 6249, 6152, 6055, 5957, 5858, 5758, 5657,
    5556, 5453, 5350, 5246, 5141, 5035, 4929, 4822, 4714, 4605, 4496, 4386, 4276, 4164, 4052, 3940,
    3827, 3713, 3599, 3484, 3369, 3253, 3137, 3020, 2903, 2785, 2667, 2549, 2430, 2311, 2191, 2071,
    1951, 1830, 1710, 1589, 1467, 1346, 1224, 1102,  980,  858,  736,  613,  491,  368,  245,  123,
    0, -122, -244, -367, -490, -612, -735, -857, -979,-1101,-1223,-1345,-1466,-1588,-1709,-1829,
    -1950,-2070,-2190,-2310,-2429,-2548,-2666,-2784,-2902,-3019,-3136,-3252,-3368,-3483,-3598,-3712,
    -3826,-3939,-4051,-4163,-4275,-4385,-4495,-4604,-4713,-4821,-4928,-5034,-5140,-5245,-5349,-5452,
    -5555,-5656,-5757,-5857,-5956,-6054,-6151,-6248,-6343,-6437,-6531,-6623,-6715,-6805,-6894,-6983,
    -7070,-7156,-7241,-7326,-7409,-7490,-7571,-7651,-7729,-7806,-7882,-7957,-8031,-8104,-8175,-8245,
    -8314,-8381,-8448,-8513,-8576,-8639,-8700,-8760,-8818,-8875,-8931,-8986,-9039,-9091,-9141,-9190,
    -9238,-9284,-9329,-9372,-9414,-9455,-9494,-9532,-9568,-9603,-9637,-9669,-9699,-9728,-9756,-9782,
    -9807,-9830,-9852,-9872,-9891,-9908,-9924,-9938,-9951,-9962,-9972,-9980,-9987,-9992,-9996,-9998,
    -9999,-9998,-9996,-9992,-9987,-9980,-9972,-9962,-9951,-9938,-9924,-9908,-9891,-9872,-9852,-9830,
    -9807,-9782,-9756,-9728,-9699,-9669,-9637,-9603,-9568,-9532,-9494,-9455,-9414,-9372,-9329,-9284,
    -9238,-9190,-9141,-9091,-9039,-8986,-8931,-8875,-8818,-8760,-8700,-8639,-8576,-8513,-8448,-8381,
    -8314,-8245,-8175,-8104,-8031,-7957,-7882,-7806,-7729,-7651,-7571,-7490,-7409,-7326,-7241,-7156,
    -7070,-6983,-6894,-6805,-6715,-6623,-6531,-6437,-6343,-6248,-6151,-6054,-5956,-5857,-5757,-5656,
    -5555,-5452,-5349,-5245,-5140,-5034,-4928,-4821,-4713,-4604,-4495,-4385,-4275,-4163,-4051,-3939,
    -3826,-3712,-3598,-3483,-3368,-3252,-3136,-3019,-2902,-2784,-2666,-2548,-2429,-2310,-2190,-2070,
    -1950,-1829,-1709,-1588,-1466,-1345,-1223,-1101, -979, -857, -735, -612, -490, -367, -244, -122
};
const short cos_tab[] =
{
    10000, 9999, 9997, 9993, 9988, 9981, 9973, 9963, 9952, 9939, 9925, 9909, 9892, 9873, 9853, 9831,
    9808, 9783, 9757, 9729, 9700, 9670, 9638, 9604, 9569, 9533, 9495, 9456, 9415, 9373, 9330, 9285,
    9239, 9191, 9142, 9092, 9040, 8987, 8932, 8876, 8819, 8761, 8701, 8640, 8577, 8514, 8449, 8382,
    8315, 8246, 8176, 8105, 8032, 7958, 7883, 7807, 7730, 7652, 7572, 7491, 7410, 7327, 7242, 7157,
    7071, 6984, 6895, 6806, 6716, 6624, 6532, 6438, 6344, 6249, 6152, 6055, 5957, 5858, 5758, 5657,
    5556, 5453, 5350, 5246, 5141, 5035, 4929, 4822, 4714, 4605, 4496, 4386, 4276, 4164, 4052, 3940,
    3827, 3713, 3599, 3484, 3369, 3253, 3137, 3020, 2903, 2785, 2667, 2549, 2430, 2311, 2191, 2071,
    1951, 1830, 1710, 1589, 1467, 1346, 1224, 1102,  980,  858,  736,  613,  491,  368,  245,  123,
    0, -122, -244, -367, -490, -612, -735, -857, -979,-1101,-1223,-1345,-1466,-1588,-1709,-1829,
    -1950,-2070,-2190,-2310,-2429,-2548,-2666,-2784,-2902,-3019,-3136,-3252,-3368,-3483,-3598,-3712,
    -3826,-3939,-4051,-4163,-4275,-4385,-4495,-4604,-4713,-4821,-4928,-5034,-5140,-5245,-5349,-5452,
    -5555,-5656,-5757,-5857,-5956,-6054,-6151,-6248,-6343,-6437,-6531,-6623,-6715,-6805,-6894,-6983,
    -7070,-7156,-7241,-7326,-7409,-7490,-7571,-7651,-7729,-7806,-7882,-7957,-8031,-8104,-8175,-8245,
    -8314,-8381,-8448,-8513,-8576,-8639,-8700,-8760,-8818,-8875,-8931,-8986,-9039,-9091,-9141,-9190,
    -9238,-9284,-9329,-9372,-9414,-9455,-9494,-9532,-9568,-9603,-9637,-9669,-9699,-9728,-9756,-9782,
    -9807,-9830,-9852,-9872,-9891,-9908,-9924,-9938,-9951,-9962,-9972,-9980,-9987,-9992,-9996,-9998,
    -9999,-9998,-9996,-9992,-9987,-9980,-9972,-9962,-9951,-9938,-9924,-9908,-9891,-9872,-9852,-9830,
    -9807,-9782,-9756,-9728,-9699,-9669,-9637,-9603,-9568,-9532,-9494,-9455,-9414,-9372,-9329,-9284,
    -9238,-9190,-9141,-9091,-9039,-8986,-8931,-8875,-8818,-8760,-8700,-8639,-8576,-8513,-8448,-8381,
    -8314,-8245,-8175,-8104,-8031,-7957,-7882,-7806,-7729,-7651,-7571,-7490,-7409,-7326,-7241,-7156,
    -7070,-6983,-6894,-6805,-6715,-6623,-6531,-6437,-6343,-6248,-6151,-6054,-5956,-5857,-5757,-5656,
    -5555,-5452,-5349,-5245,-5140,-5034,-4928,-4821,-4713,-4604,-4495,-4385,-4275,-4163,-4051,-3939,
    -3826,-3712,-3598,-3483,-3368,-3252,-3136,-3019,-2902,-2784,-2666,-2548,-2429,-2310,-2190,-2070,
    -1950,-1829,-1709,-1588,-1466,-1345,-1223,-1101, -979, -857, -735, -612, -490, -367, -244, -122,
    0,  123,  245,  368,  491,  613,  736,  858,  980, 1102, 1224, 1346, 1467, 1589, 1710, 1830,
    1951, 2071, 2191, 2311, 2430, 2549, 2667, 2785, 2903, 3020, 3137, 3253, 3369, 3484, 3599, 3713,
    3827, 3940, 4052, 4164, 4276, 4386, 4496, 4605, 4714, 4822, 4929, 5035, 5141, 5246, 5350, 5453,
    5556, 5657, 5758, 5858, 5957, 6055, 6152, 6249, 6344, 6438, 6532, 6624, 6716, 6806, 6895, 6984,
    7071, 7157, 7242, 7327, 7410, 7491, 7572, 7652, 7730, 7807, 7883, 7958, 8032, 8105, 8176, 8246,
    8315, 8382, 8449, 8514, 8577, 8640, 8701, 8761, 8819, 8876, 8932, 8987, 9040, 9092, 9142, 9191,
    9239, 9285, 9330, 9373, 9415, 9456, 9495, 9533, 9569, 9604, 9638, 9670, 9700, 9729, 9757, 9783,
    9808, 9831, 9853, 9873, 9892, 9909, 9925, 9939, 9952, 9963, 9973, 9981, 9988, 9993, 9997, 9999
};



const u_short PARA_BB1[] =
{
    //为方便处理，长度应固定
    CH(217),CH(2211),CH(2329),CH(2676),'5',' ',' ',CH(16),
    CH(218),CH(2211),CH(2329),CH(2676),'5','B',' ',CH(16),
    CH(219),CH(2211),CH(2329),CH(2676),'2','1',' ',CH(16),
    CH(220),CH(2211),CH(2371),'3',' ',' ',CH( 100),CH(16),
    CH(221),CH(2211),CH(2371),'4','B',' ',CH( 100),CH(16),
    CH(222),CH(2211),CH(2371),'4','C',' ',CH( 100),CH(16),
    CH(223),CH(2211),CH(2371),'4','D',' ',CH( 100),CH(16),
    CH(224),CH(2211),CH(2371),'4','E',' ',CH( 100),CH(16),
    CH(225),CH(2211),CH(2371),'5',' ',' ',CH( 100),CH(16),
    CH(226),CH(2211),CH(2371),'5','B',' ',CH( 100),CH(16),

    CH(217),CH(2211),CH(2371),'5','D',' ',CH( 100),CH(16),
    CH(218),CH(2211),CH(2371),'6',' ',' ',CH( 100),CH(16),
    CH(219),CH(2211),CH(2371),'7',' ',' ',CH( 100),CH(16),
    CH(220),CH(2211),CH(2371),'7','B',' ',CH( 100),CH(16),
    CH(221),CH(2211),CH(2371),'7','C',' ',CH( 100),CH(16),
    CH(222),CH(2211),CH(2371),'7','D',' ',CH( 100),CH(16),
    CH(223),CH(2211),CH(2371),'8',' ',' ',CH( 100),CH(16),
    CH(224),CH(2211),CH(2371),'8','B',' ',CH( 100),CH(16),
    CH(225),CH(2211),CH(2371),'1','0','F',CH( 100),CH(16),
    CH(226),CH(2211),CH(2371),'1','1',' ',CH( 100),CH(16),

    CH(217),CH(4156),CH(4129),'3','B',' ',CH( 100),CH(16),
    CH(218),CH(4156),CH(4129),'4',' ',' ',CH( 100),CH(16),
    CH(219),CH(4156),CH(4129),'4','B',' ',CH( 100),CH(16),
    CH(220),CH(4156),CH(4129),'4','C',' ',CH( 100),CH(16),
    CH(221),CH(4156),CH(4129),'6','B',' ',CH( 100),CH(16),
    CH(222),CH(4156),CH(4129),'7','B',' ',CH( 100),CH(16),
    CH(223),CH(4156),CH(4129),'8',' ',' ',CH( 100),CH(16),
    CH(224),CH(4156),CH(4129),'9',' ',' ',CH( 100),CH(16),
    CH(225),'N','D', '3',' ',' ',CH( 100),CH(16),
    CH(226),'N','D', '5',' ',' ',CH( 100),CH(16)
};

/*
东方红5
东方红5B
东方红21
东风3
东风4B
东风4C
东风4D
东风4E
东风5
东风5B

东风5D
东风6
东风7
东风7B
东风7C
东风7D
东风8
东风8B
东风10F
东风11

韶山3B
韶山 4
韶山4B
韶山4C
韶山6B
韶山7B
韶山 8
韶山 9
ND3
ND5
*/


#endif


