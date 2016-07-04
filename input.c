#include "hzk.h"

int MInputChar(u_int xpos,u_int ypos,u_int type, u_short* InputChar,u_short len_char_max,u_short char_max)
{
    /*输入字符，type = 0只能英文，1可英可中*/
    u_short pzText[4];
    u_short char_len = 0;	/*输入长度*/
    u_short cursor_posi = 0;	/*光标所在位置，一般在最后*/
    u_short insert_sign = 0;	/*中间插入标志，0未插入，1有插入*/
    u_short association;
    xpos = (xpos+16)/16 * 16;
    //u_int s_xpos = xpos,s_ypos = ypos;	//初始位置
    u_int s_xpos = xpos;	//初始位置
    u_int lpos = xpos,rpos = xpos+16*len_char_max;	//两端位置
    u_int retvalue;

    int i;
    if (MGetLanguage())
    {
        type = 0; //强制不输入中文
    }
//;	MPushWindow(C_PINYIN_HPOSI,C_PINYIN_VPOSI,C_PINYIN_HPOSI+20*C_CCHAR_HDOT, C_PINYIN_VPOSI + 2 * 24) ;
    MPushWindow(C_PINYIN_HPOSI,C_PINYIN_VPOSI,C_HORIDOT_SCREEN,480);// C_PINYIN_VPOSI + 2 * 30) ;
  //  MPushWindow(C_HORIDOT_SCREEN/2,C_PINYIN_VPOSI,C_HORIDOT_SCREEN, C_PINYIN_VPOSI + 2 * 24) ;
    MSetColor( C_CR_PARA);
  //  MKeyRlx();

    for(i = 0; i < char_max; i++)    //2012-03-05 此处赋值造成探头类型被赋值为0
        InputChar[i] = 0;

    do
    {
        if( (type == 0 && MGetInputMode() >= C_INPUT_PINYIN) || ( MGetInputMode() > C_INPUT_QUWEI ) )
            MSetInputMode(C_INPUT_DIGITAL,0);
        //	CEMenuOut(C_PINYIN_HPOSI,C_PINYIN_VPOSI + 4,_inputmode+2*(MGetInputMode()-0),2,1,24);
        EraseDrawRectangle(C_PINYIN_HPOSI,C_PINYIN_VPOSI,C_HORIDOT_SCREEN, C_PINYIN_VPOSI + 1 * 40) ;
        EraseDrawRectangle(C_WORD_HPOSI,C_WORD_VPOSI+6,C_HORIDOT_SCREEN, C_WORD_VPOSI + 1 * 30+12) ;
        EraseDrawRectangle(C_WORD_HPOSI,C_WORD_VPOSI+ 1 * 30+12,C_HORIDOT_SCREEN, C_WORD_VPOSI + 2 * 30+1) ;

        TextOut(C_PINYIN_HPOSI,C_PINYIN_VPOSI + 4, 1, 10,24, (u_char*)_inputmode[MGetLanguage()][MGetInputMode()], 4);

        //Write_Number(0,30,char_len,4,0,0);
        pzText[0] = 0;
        if( ( MGetInputMode() == C_INPUT_PINYIN || MGetInputMode() == C_INPUT_QUWEI )&& type > 0 )
        {
            while(1)
            {
                if( xpos > rpos )
                {
                    xpos = lpos;
                    ypos += 24;
                }
                association = InputChar[cursor_posi];
                //association = ( association/256)*100+association%256;
                if(MGetInputMode() == C_INPUT_PINYIN)
                    pzText[0] = MGetCChar(xpos,ypos,association) ;
                else pzText[0] = MGetQuwei(xpos,ypos,association) ;
                if(pzText[0] == 0)break;
                else if ( pzText[0] >= 32 && char_len < char_max)	/*输入有效字符*/
                {
                    char_len++;
                    for(i = char_len; i>cursor_posi; i--)InputChar[i] = InputChar[i-1];
                    cursor_posi ++;
                    InputChar[cursor_posi] = pzText[0];

//					Write_Space(xpos ,ypos,char_len  + 1 - cursor_posi,type);	/*写空格*/
                    //TextOut(xpos,ypos,1,char_len,C_CCHAR_VDOT,InputChar + cursor_posi ,6);
					CMenuOut(xpos ,ypos , InputChar + cursor_posi ,char_len  + 1 - cursor_posi,1,24);/*重写*/
//                    CETextOut(xpos,ypos,lpos,rpos,InputChar + cursor_posi,char_len +1 - cursor_posi,12 * (1+type),type);
                    if( pzText[0] < 256)	/*输入英文字符*/
                        xpos += C_ECHAR_HDOT;
                    else
                        xpos += C_CCHAR_HDOT;
                    MKeyRlx();
                }
                else break;
                ExpendTime(20);
            }
            pzText[1] = 0;
        }
        else if( MGetInputMode() < C_INPUT_PINYIN )
        {
            pzText[0] = 0;
            while(1)
            {
                if( xpos > rpos )
                {
                    xpos = lpos;
                    ypos += 24;
                }
                MGetEChar(xpos,ypos,pzText,char_len - cursor_posi);	/*得到ASCII字符*/
//Write_Number(0,40,pzText[0],4,0,0);
//Write_Number(0,50,pzText[1],4,0,0);
                if ( pzText[0] >= 32  && char_len < char_max)	/*输入有效字符*/
                {
                    if( insert_sign == 0 )
                    {
                        for(i = char_len+1; i>cursor_posi; i--)InputChar[i] = InputChar[i-1];
                        char_len++;
                        cursor_posi ++;
                    }
                    else
                    {
                        if( pzText[1] >= 32 )
                        {
                            InputChar[cursor_posi] = pzText[0];
                            for(i = char_len+1; i>cursor_posi; i--)InputChar[i] = InputChar[i-1];
                            char_len++;
                            cursor_posi ++;
                            pzText[0] = pzText[1];
                            xpos += C_ECHAR_HDOT;
                        }
                        insert_sign = 0 ;
                    }
                    InputChar[cursor_posi] = pzText[0];

//Write_Number(0,50,pzText[0],4,0,0);
//Write_Number(0,70,pzText[1],4,0,0);
                  //  TextOut(xpos,ypos,1,char_len,C_ECHAR_VDOT,InputChar + cursor_posi ,6);
                    CEMenuOut(xpos ,ypos , InputChar + cursor_posi ,char_len  + 1 - cursor_posi,1,24);/*重写*/

                    //CETextOut(xpos,ypos,lpos,rpos,InputChar + cursor_posi,char_len +1 - cursor_posi,16 * (1+type),type);
                    if(pzText[1] >= 32 && insert_sign == 0 && char_len > cursor_posi)
                        insert_sign++;			//表示中间插入一字符
                    else
                        xpos += C_ECHAR_HDOT;

                    if( pzText[1] > 0 && pzText[1] < 32)
                    {
                        pzText[0] = pzText[1];	/*检查输入有效字符后是否输入控制符*/
                        pzText[1] = 0;
                        break;	/*输入控制符*/
                    }
                    MKeyRlx();
                    //ExpendTime(20);
                }
                else
                {
                    if(pzText[0] == 0)
                    {
                        pzText[0] = pzText[1];	/*检查输入有效字符后是否输入控制符*/
                        pzText[1] = 0;
                    }
                    break;	/*输入返回或确认*/
                }
                pzText[0] = pzText[1];	/*检查输入有效字符后是否输入控制符*/
            }


        }
//Write_Number(0,50,pzText[0],4,0,0);
        if(  pzText[0] != pzText[1] && pzText[0] == C_KEYCOD_RETURN)	/*输入返回键*/
        {
            InputChar[0] = 0;	/*设置输入字符数为0*/
            retvalue = C_FALSE;
            break;
        }
        else if(  pzText[0] != pzText[1] && pzText[0] == C_KEYCOD_CONFIRM)	/*输入回车键*/
        {
            InputChar[0] = char_len;	/*设置输入字符数	 */
            retvalue = C_TRUE;
            break;
        }
        else if( pzText[0] == C_KEYCOD_DEL && cursor_posi >0)	/*输入删除键*/
        {
            if( InputChar[cursor_posi] < 256)	/*输入英文字符*/
            {
                if(xpos > s_xpos )xpos -= C_ECHAR_HDOT;
                else ypos -= 24 * (1+type);
            }
            else
            {
                if(xpos > s_xpos )xpos -= C_CCHAR_HDOT;
                else ypos -= 24 * (1+type);
            }

            for(i=cursor_posi; i<char_len; i++)
                InputChar[i] = InputChar[i+1];	/*后续字前移*/
            //if( (char_len - cursor_posi)*(1+type) < len_char_max * 8 )
            //{
//				Write_Space(xpos ,ypos,char_len + 2- cursor_posi,type);	/*写空格*/
//				CEMenuOut(xpos ,ypos , InputChar + cursor_posi ,char_len - cursor_posi,1,24);/*重写*/
            //}
            //else
            //{
            //	MEraseWindow(s_xpos , s_ypos,s_xpos + len_char_max * C_ECHAR_HDOT, ypos + C_ECHAR_VDOT*(1+type) ) ; /* 清除窗体 */
            //	CEMenuOut(s_xpos ,s_ypos , InputChar + 1 ,len_char_max * (1+type), (ypos - s_ypos )/( 12 * (1+type) )+1,12 * (1+type) );/*重写*/
            //}
            for(i = char_max - 1; i >= char_len; i--) InputChar[i] = 0;
            //TextOut(xpos,ypos,1,char_len,C_CCHAR_VDOT,InputChar + cursor_posi ,6);
            CEMenuOut(xpos ,ypos , InputChar + cursor_posi ,char_len  + 1 - cursor_posi,1,24);/*重写*/
            //CETextOut(xpos,ypos,lpos,rpos,InputChar + cursor_posi,char_len + 1 - cursor_posi,12 * (1+type),type);
            char_len--;
            cursor_posi--;
            /*原最后一字及光标用空格抹掉*/
        }
        else if( pzText[0] == C_KEYCOD_LEFT && cursor_posi >0)
        {
            //输入左移键
//			if( InputChar[cursor_posi] < 256)	//输入英文字符
//				xpos -= C_ECHAR_HDOT;
//			else
//				xpos -= C_CCHAR_HDOT;
//			cursor_posi--;
        }
        else if( pzText[0] == C_KEYCOD_RIGHT && cursor_posi <char_len)	//输入右移键
        {
            //输入左移键
//			cursor_posi++;
//			if( InputChar[cursor_posi] < 256)	//输入英文字符
//				xpos += C_ECHAR_HDOT;
//			else
//				xpos += C_CCHAR_HDOT;
        }
        else if( pzText[0] == C_KEYCOD_PLUS && cursor_posi <char_len)
        {

        }
        else if( pzText[0] == C_KEYCOD_MINUS&& cursor_posi >0)
        {

        }

    }
    while(1);

    MPopWindow() ;
    //MPopWindow() ;
    return retvalue;
}

int MGetCChar(u_int xpos,u_int ypos,u_short association)
{

    int i,j,k;
    int t_xpos,t_ypos;
    unsigned short pzText[2];
    unsigned char input[C_MAX_PINYIN_LEN + 1];
    unsigned char input_num[C_MAX_PINYIN_LEN + 1];	/*所有已输入的数字*/
    int keycode;	/*当前输入的数字*/
    unsigned char input_degree;	/*有效按键次数*/
    unsigned char pinyin_num;	/*当前按键具有的拼音数目*/
    unsigned char pinyin_no;	/*第几个拼音*/
    unsigned short pinyin_posi = 0;
    /*根据当前按键可能的拼音位置，按2键则以2开头的拼音*/
    unsigned short wordindex_posi;	/*当前拼音在字库中的位置*/
    unsigned char page_num = 0;			/*当前拼音对应的汉字页总数，每页9个*/
    unsigned char page_no = 0;			/*当前拼音对应的汉字页数,第几页*/
    unsigned char page_word_num = 0;	/*当前拼音对应的汉字页中的字数，最大9*/

    char mode = 0; //不同功能,回车+1,=0方向键选拼音,=1固定拼音方向键选字组(即汉字翻页),=2数字键直接选字
    //
    const unsigned short *pWordIndex[C_PINYIN_TOTAL];
    wordindex_posi = 0;
//a0-5
    pWordIndex[0] = _A;
    pWordIndex[1] = _AI;
    pWordIndex[2] = _AN;
    pWordIndex[3] = _ANG;
    pWordIndex[4] = _AO;
//b5-20
    wordindex_posi = 5;
    pWordIndex[5] = _BA;
    pWordIndex[6] = _BAI;
    pWordIndex[7] = _BAN;
    pWordIndex[8] = _BANG;
    pWordIndex[9] = _BAO;
    pWordIndex[10] = _BEI;
    pWordIndex[11] = _BEN;
    pWordIndex[12] = _BENG;
    pWordIndex[13] = _BI;
    pWordIndex[14] = _BIAN;
    pWordIndex[15] = _BIAO;
    pWordIndex[16] = _BIE;
    pWordIndex[17] = _BIN;
    pWordIndex[18] = _BING;
    pWordIndex[19] = _BO;
    pWordIndex[20] = _BU;
    /*c34=21-54*/
    wordindex_posi = 21;
    pWordIndex[21] = _CA;
    pWordIndex[22] = _CAI;
    pWordIndex[23] = _CAN;
    pWordIndex[24] = _CANG;
    pWordIndex[25] = _CAO;
    pWordIndex[26] = _CE;
    pWordIndex[27] = _CEN;
    pWordIndex[28] = _CENG;
    pWordIndex[29] = _CHA;
    pWordIndex[30] = _CHAI;
    pWordIndex[31] = _CHAN;
    pWordIndex[32] = _CHANG;
    pWordIndex[33] = _CHAO;
    pWordIndex[34] = _CHE;
    pWordIndex[35] = _CHEN;
    pWordIndex[36] = _CHENG;
    pWordIndex[37] = _CHI;
    pWordIndex[38] = _CHONG;
    pWordIndex[39] = _CHOU;
    pWordIndex[40] = _CHU;
    pWordIndex[41] = _CHUA;
    pWordIndex[42] = _CHUAI;
    pWordIndex[43] = _CHUAN;
    pWordIndex[44] = _CHUANG;
    pWordIndex[45] = _CHUI;
    pWordIndex[46] = _CHUN;
    pWordIndex[47] = _CHUO;
    pWordIndex[48] = _CI;
    pWordIndex[49] = _CONG;
    pWordIndex[50] = _COU;
    pWordIndex[51] = _CU;
    pWordIndex[52] = _CUAN;
    pWordIndex[53] = _CUI;
    pWordIndex[54] = _CUN;
    pWordIndex[55] = _CUO;
    /*d21=56-76*/
    wordindex_posi = 56;
    pWordIndex[56] = _DA;
    pWordIndex[57] = _DAI;
    pWordIndex[58] = _DAN;
    pWordIndex[59] = _DANG;
    pWordIndex[60] = _DAO;
    pWordIndex[61] = _DE;
    pWordIndex[62] = _DENG;
    pWordIndex[63] = _DI;
    pWordIndex[64] = _DIA;
    pWordIndex[65] = _DIAN;
    pWordIndex[66] = _DIAO;
    pWordIndex[67] = _DIE;
    pWordIndex[68] = _DING;
    pWordIndex[69] = _DIU;
    pWordIndex[70] = _DONG;
    pWordIndex[71] = _DOU;
    pWordIndex[72] = _DU;
    pWordIndex[73] = _DUAN;
    pWordIndex[74] = _DUI;
    pWordIndex[75] = _DUN;
    pWordIndex[76] = _DUO;
//e4=76-79
    wordindex_posi = 76;
    pWordIndex[ wordindex_posi + 1] = _E;
    pWordIndex[ wordindex_posi + 2] = _EI;
    pWordIndex[ wordindex_posi + 3] = _EN;
    pWordIndex[ wordindex_posi + 4] = _ER;
//f80-87=8
    wordindex_posi += 4;
    pWordIndex[ wordindex_posi + 1] = _FA;
    pWordIndex[ wordindex_posi + 2] = _FAN;
    pWordIndex[ wordindex_posi + 3] = _FANG;
    pWordIndex[ wordindex_posi + 4] = _FEI;
    pWordIndex[ wordindex_posi + 5] = _FEN;
    pWordIndex[ wordindex_posi + 6] = _FENG;
    pWordIndex[ wordindex_posi + 7] = _FO;
    pWordIndex[ wordindex_posi + 8] = _FOU;
    pWordIndex[ wordindex_posi + 9] = _FU;
//g88-105=18,h106-125=20
    wordindex_posi += 9;
    pWordIndex[ wordindex_posi + 1] = _GA;
    pWordIndex[ wordindex_posi + 2] = _GAI;
    pWordIndex[ wordindex_posi + 3] = _GAN;
    pWordIndex[ wordindex_posi + 4] = _GANG;
    pWordIndex[ wordindex_posi + 5] = _GAO;
    pWordIndex[ wordindex_posi + 6] = _GE;
    pWordIndex[ wordindex_posi + 7] = _GEI;
    pWordIndex[ wordindex_posi + 8] = _GEN;
    pWordIndex[ wordindex_posi + 9] = _GENG;
    pWordIndex[ wordindex_posi + 10] = _GONG;
    pWordIndex[ wordindex_posi + 11] = _GOU;
    pWordIndex[ wordindex_posi + 12] = _GU;
    pWordIndex[ wordindex_posi + 13] = _GUA;
    pWordIndex[ wordindex_posi + 14] = _GUAI;
    pWordIndex[ wordindex_posi + 15] = _GUAN;
    pWordIndex[ wordindex_posi + 16] = _GUANG;
    pWordIndex[ wordindex_posi + 17] = _GUI;
    pWordIndex[ wordindex_posi + 18] = _GUN;
    pWordIndex[ wordindex_posi + 19] = _GUO;
    pWordIndex[ wordindex_posi + 20] = _HA;
    pWordIndex[ wordindex_posi + 21] = _HAI;
    pWordIndex[ wordindex_posi + 22] = _HAN;
    pWordIndex[ wordindex_posi + 23] = _HANG;
    pWordIndex[ wordindex_posi + 24] = _HAO;
    pWordIndex[ wordindex_posi + 25] = _HE;
    pWordIndex[ wordindex_posi + 26] = _HEI;
    pWordIndex[ wordindex_posi + 27] = _HEN;
    pWordIndex[ wordindex_posi + 28] = _HENG;
    pWordIndex[ wordindex_posi + 29] = _HONG;
    pWordIndex[ wordindex_posi + 30] = _HOU;
    pWordIndex[ wordindex_posi + 31] = _HU;
    pWordIndex[ wordindex_posi + 32] = _HUA;
    pWordIndex[ wordindex_posi + 33] = _HUAI;
    pWordIndex[ wordindex_posi + 34] = _HUAN;
    pWordIndex[ wordindex_posi + 35] = _HUANG;
    pWordIndex[ wordindex_posi + 36] = _HUI;
    pWordIndex[ wordindex_posi + 37] = _HUN;
    pWordIndex[ wordindex_posi + 38] = _HUO;
//126-139
    wordindex_posi += 38;
    pWordIndex[ wordindex_posi + 1] = _JI;
    pWordIndex[ wordindex_posi + 2] = _JIA;
    pWordIndex[ wordindex_posi + 3] = _JIAN;
    pWordIndex[ wordindex_posi + 4] = _JIANG;
    pWordIndex[ wordindex_posi + 5] = _JIAO;
    pWordIndex[ wordindex_posi + 6] = _JIE;
    pWordIndex[ wordindex_posi + 7] = _JIN;
    pWordIndex[ wordindex_posi + 8] = _JING;
    pWordIndex[ wordindex_posi + 9] = _JIONG;
    pWordIndex[ wordindex_posi + 10] = _JIU;
    pWordIndex[ wordindex_posi + 11] = _JU;
    pWordIndex[ wordindex_posi + 12] = _JUAN;
    pWordIndex[ wordindex_posi + 13] = _JUE;
    pWordIndex[ wordindex_posi + 14] = _JUN;
//140
    wordindex_posi += 14;
    pWordIndex[ wordindex_posi + 1] = _KA;
    pWordIndex[ wordindex_posi + 2] = _KAI;
    pWordIndex[ wordindex_posi + 3] = _KAN;
    pWordIndex[ wordindex_posi + 4] = _KANG;
    pWordIndex[ wordindex_posi + 5] = _KAO;
    pWordIndex[ wordindex_posi + 6] = _KE;
    pWordIndex[ wordindex_posi + 7] = _KEN;
    pWordIndex[ wordindex_posi + 8] = _KENG;
    pWordIndex[ wordindex_posi + 9] = _KONG;
    pWordIndex[ wordindex_posi + 10] = _KOU;
    pWordIndex[ wordindex_posi + 11] = _KU;
    pWordIndex[ wordindex_posi + 12] = _KUA;
    pWordIndex[ wordindex_posi + 13] = _KUAI;
    pWordIndex[ wordindex_posi + 14] = _KUAN;
    pWordIndex[ wordindex_posi + 15] = _KUANG;
    pWordIndex[ wordindex_posi + 16] = _KUI;
    pWordIndex[ wordindex_posi + 17] = _KUN;
    pWordIndex[ wordindex_posi + 18] = _KUO;
//158
    wordindex_posi += 18;
    pWordIndex[ wordindex_posi + 1] = _LA;
    pWordIndex[ wordindex_posi + 2] = _LAI;
    pWordIndex[ wordindex_posi + 3] = _LAN;
    pWordIndex[ wordindex_posi + 4] = _LANG;
    pWordIndex[ wordindex_posi + 5] = _LAO;
    pWordIndex[ wordindex_posi + 6] = _LE;
    pWordIndex[ wordindex_posi + 7] = _LEI;
    pWordIndex[ wordindex_posi + 8] = _LENG;
    pWordIndex[ wordindex_posi + 9] = _LI;
    pWordIndex[ wordindex_posi + 10] = _LIA;
    pWordIndex[ wordindex_posi + 11] = _LIAN;
    pWordIndex[ wordindex_posi + 12] = _LIANG;
    pWordIndex[ wordindex_posi + 13] = _LIAO;
    pWordIndex[ wordindex_posi + 14] = _LIE;
    pWordIndex[ wordindex_posi + 15] = _LIN;
    pWordIndex[ wordindex_posi + 16] = _LING;
    pWordIndex[ wordindex_posi + 17] = _LIU;
    pWordIndex[ wordindex_posi + 18] = _LONG;
    pWordIndex[ wordindex_posi + 19] = _LOU;
    pWordIndex[ wordindex_posi + 20] = _LU;
    pWordIndex[ wordindex_posi + 21] = _LV;
    pWordIndex[ wordindex_posi + 22] = _LUAN;
    pWordIndex[ wordindex_posi + 23] = _LUE;
    pWordIndex[ wordindex_posi + 24] = _LUN;
    pWordIndex[ wordindex_posi + 25] = _LUO;
//182
    wordindex_posi += 25;
    pWordIndex[ wordindex_posi + 1] = _M;
    pWordIndex[ wordindex_posi + 2] = _MA;
    pWordIndex[ wordindex_posi + 3] = _MAI;
    pWordIndex[ wordindex_posi + 4] = _MAN;
    pWordIndex[ wordindex_posi + 5] = _MANG;
    pWordIndex[ wordindex_posi + 6] = _MAO;
    pWordIndex[ wordindex_posi + 7] = _ME;
    pWordIndex[ wordindex_posi + 8] = _MEI;
    pWordIndex[ wordindex_posi + 9] = _MEN;
    pWordIndex[ wordindex_posi + 10] = _MENG;
    pWordIndex[ wordindex_posi + 11] = _MI;
    pWordIndex[ wordindex_posi + 12] = _MIAN;
    pWordIndex[ wordindex_posi + 13] = _MIAO;
    pWordIndex[ wordindex_posi + 14] = _MIE;
    pWordIndex[ wordindex_posi + 15] = _MIN;
    pWordIndex[ wordindex_posi + 16] = _MING;
    pWordIndex[ wordindex_posi + 17] = _MIU;
    pWordIndex[ wordindex_posi + 18] = _MO;
    pWordIndex[ wordindex_posi + 19] = _MOU;
    pWordIndex[ wordindex_posi + 20] = _MU;
//202-
    wordindex_posi += 20;
    pWordIndex[ wordindex_posi + 1] = _NA;
    pWordIndex[ wordindex_posi + 2] = _NAI;
    pWordIndex[ wordindex_posi + 3] = _NAN;
    pWordIndex[ wordindex_posi + 4] = _NANG;
    pWordIndex[ wordindex_posi + 5] = _NAO;
    pWordIndex[ wordindex_posi + 6] = _NE;
    pWordIndex[ wordindex_posi + 7] = _NEI;
    pWordIndex[ wordindex_posi + 8] = _NEN;
    pWordIndex[ wordindex_posi + 9] = _NENG;
    pWordIndex[ wordindex_posi + 10] = _NI;
    pWordIndex[ wordindex_posi + 11] = _NIAN;
    pWordIndex[ wordindex_posi + 12] = _NIANG;
    pWordIndex[ wordindex_posi + 13] = _NIAO;
    pWordIndex[ wordindex_posi + 14] = _NIE;
    pWordIndex[ wordindex_posi + 15] = _NIN;
    pWordIndex[ wordindex_posi + 16] = _NING;
    pWordIndex[ wordindex_posi + 17] = _NIU;
    pWordIndex[ wordindex_posi + 18] = _NONG;
    pWordIndex[ wordindex_posi + 19] = _NOU;
    pWordIndex[ wordindex_posi + 20] = _NU;
    pWordIndex[ wordindex_posi + 21] = _NV;
    pWordIndex[ wordindex_posi + 22] = _NUAN;
    pWordIndex[ wordindex_posi + 23] = _NUE;
    pWordIndex[ wordindex_posi + 24] = _NUO;
//225
    wordindex_posi += 24;
    pWordIndex[ wordindex_posi + 1] = _O;
    pWordIndex[ wordindex_posi + 2] = _OU;
    pWordIndex[ wordindex_posi + 3] = _PA;
    pWordIndex[ wordindex_posi + 4] = _PAI;
    pWordIndex[ wordindex_posi + 5] = _PAN;
    pWordIndex[ wordindex_posi + 6] = _PANG;
    pWordIndex[ wordindex_posi + 7] = _PAO;
    pWordIndex[ wordindex_posi + 8] = _PEI;
    pWordIndex[ wordindex_posi + 9] = _PEN;
    pWordIndex[ wordindex_posi + 10] = _PENG;
    pWordIndex[ wordindex_posi + 11] = _PI;
    pWordIndex[ wordindex_posi + 12] = _PIAN;
    pWordIndex[ wordindex_posi + 13] = _PIAO;
    pWordIndex[ wordindex_posi + 14] = _PIE;
    pWordIndex[ wordindex_posi + 15] = _PIN;
    pWordIndex[ wordindex_posi + 16] = _PING;
    pWordIndex[ wordindex_posi + 17] = _PO;
    pWordIndex[ wordindex_posi + 18] = _POU;
    pWordIndex[ wordindex_posi + 19] = _PU;
//244
    wordindex_posi += 19;
    pWordIndex[ wordindex_posi + 1] = _QI;
    pWordIndex[ wordindex_posi + 2] = _QIA;
    pWordIndex[ wordindex_posi + 3] = _QIAN;
    pWordIndex[ wordindex_posi + 4] = _QIANG;
    pWordIndex[ wordindex_posi + 5] = _QIAO;
    pWordIndex[ wordindex_posi + 6] = _QIE;
    pWordIndex[ wordindex_posi + 7] = _QIN;
    pWordIndex[ wordindex_posi + 8] = _QING;
    pWordIndex[ wordindex_posi + 9] = _QIONG;
    pWordIndex[ wordindex_posi + 10] = _QIU;
    pWordIndex[ wordindex_posi + 11] = _QU;
    pWordIndex[ wordindex_posi + 12] = _QUAN;
    pWordIndex[ wordindex_posi + 13] = _QUE;
    pWordIndex[ wordindex_posi + 14] = _QUN;
//258
    wordindex_posi += 14;
    pWordIndex[ wordindex_posi + 1] = _RAN;
    pWordIndex[ wordindex_posi + 2] = _RANG;
    pWordIndex[ wordindex_posi + 3] = _RAO;
    pWordIndex[ wordindex_posi + 4] = _RE;
    pWordIndex[ wordindex_posi + 5] = _REN;
    pWordIndex[ wordindex_posi + 6] = _RENG;
    pWordIndex[ wordindex_posi + 7] = _RI;
    pWordIndex[ wordindex_posi + 8] = _RONG;
    pWordIndex[ wordindex_posi + 9] = _ROU;
    pWordIndex[ wordindex_posi + 10] = _RU;
    pWordIndex[ wordindex_posi + 11] = _RUAN;
    pWordIndex[ wordindex_posi + 12] = _RUI;
    pWordIndex[ wordindex_posi + 13] = _RUN;
    pWordIndex[ wordindex_posi + 14] = _RUO;
//272
    wordindex_posi += 14;
    pWordIndex[ wordindex_posi + 1] = _SA;
    pWordIndex[ wordindex_posi + 2] = _SAI;
    pWordIndex[ wordindex_posi + 3] = _SAN;
    pWordIndex[ wordindex_posi + 4] = _SANG;
    pWordIndex[ wordindex_posi + 5] = _SAO;
    pWordIndex[ wordindex_posi + 6] = _SE;
    pWordIndex[ wordindex_posi + 7] = _SEN;
    pWordIndex[ wordindex_posi + 8] = _SENG;
    pWordIndex[ wordindex_posi + 9] = _SHA;
    pWordIndex[ wordindex_posi + 10] = _SHAI;
    pWordIndex[ wordindex_posi + 11] = _SHAN;
    pWordIndex[ wordindex_posi + 12] = _SHANG;
    pWordIndex[ wordindex_posi + 13] = _SHAO;
    pWordIndex[ wordindex_posi + 14] = _SHE;
    pWordIndex[ wordindex_posi + 15] = _SHEN;
    pWordIndex[ wordindex_posi + 16] = _SHENG;
    pWordIndex[ wordindex_posi + 17] = _SHI;
    pWordIndex[ wordindex_posi + 18] = _SHOU;
    pWordIndex[ wordindex_posi + 19] = _SHU;
    pWordIndex[ wordindex_posi + 20] = _SHUA;
    pWordIndex[ wordindex_posi + 21] = _SHUAI;
    pWordIndex[ wordindex_posi + 22] = _SHUAN;
    pWordIndex[ wordindex_posi + 23] = _SHUANG;
    pWordIndex[ wordindex_posi + 24] = _SHUI;
    pWordIndex[ wordindex_posi + 25] = _SHUN;
    pWordIndex[ wordindex_posi + 26] = _SHUO;
    pWordIndex[ wordindex_posi + 27] = _SI;
    pWordIndex[ wordindex_posi + 28] = _SONG;
    pWordIndex[ wordindex_posi + 29] = _SOU;
    pWordIndex[ wordindex_posi + 30] = _SU;
    pWordIndex[ wordindex_posi + 31] = _SUAN;
    pWordIndex[ wordindex_posi + 32] = _SUI;
    pWordIndex[ wordindex_posi + 33] = _SUN;
    pWordIndex[ wordindex_posi + 34] = _SUO;
//306
    wordindex_posi += 34;
    pWordIndex[ wordindex_posi + 1] = _TA;
    pWordIndex[ wordindex_posi + 2] = _TAI;
    pWordIndex[ wordindex_posi + 3] = _TAN;
    pWordIndex[ wordindex_posi + 4] = _TANG;
    pWordIndex[ wordindex_posi + 5] = _TAO;
    pWordIndex[ wordindex_posi + 6] = _TE;
    pWordIndex[ wordindex_posi + 7] = _TENG;
    pWordIndex[ wordindex_posi + 8] = _TI;
    pWordIndex[ wordindex_posi + 9] = _TIAN;
    pWordIndex[ wordindex_posi + 10] = _TIAO;
    pWordIndex[ wordindex_posi + 11] = _TIE;
    pWordIndex[ wordindex_posi + 12] = _TING;
    pWordIndex[ wordindex_posi + 13] = _TO;
    pWordIndex[ wordindex_posi + 14] = _TONG;
    pWordIndex[ wordindex_posi + 15] = _TOU;
    pWordIndex[ wordindex_posi + 16] = _TU;
    pWordIndex[ wordindex_posi + 17] = _TUAN;
    pWordIndex[ wordindex_posi + 18] = _TUI;
    pWordIndex[ wordindex_posi + 19] = _TUN;
    pWordIndex[ wordindex_posi + 20] = _TUO;
//325
    wordindex_posi += 20;
    pWordIndex[ wordindex_posi + 1] = _WA;
    pWordIndex[ wordindex_posi + 2] = _WAI;
    pWordIndex[ wordindex_posi + 3] = _WAN;
    pWordIndex[ wordindex_posi + 4] = _WANG;
    pWordIndex[ wordindex_posi + 5] = _WEI;
    pWordIndex[ wordindex_posi + 6] = _WEN;
    pWordIndex[ wordindex_posi + 7] = _WENG;
    pWordIndex[ wordindex_posi + 8] = _WO;
    pWordIndex[ wordindex_posi + 9] = _WU;
//334
    wordindex_posi += 9;
    pWordIndex[ wordindex_posi + 1] = _XI;
    pWordIndex[ wordindex_posi + 2] = _XIA;
    pWordIndex[ wordindex_posi + 3] = _XIAN;
    pWordIndex[ wordindex_posi + 4] = _XIANG;
    pWordIndex[ wordindex_posi + 5] = _XIAO;
    pWordIndex[ wordindex_posi + 6] = _XIE;
    pWordIndex[ wordindex_posi + 7] = _XIN;
    pWordIndex[ wordindex_posi + 8] = _XING;
    pWordIndex[ wordindex_posi + 9] = _XIONG;
    pWordIndex[ wordindex_posi + 10] = _XIU;
    pWordIndex[ wordindex_posi + 11] = _XU;
    pWordIndex[ wordindex_posi + 12] = _XUAN;
    pWordIndex[ wordindex_posi + 13] = _XUE;
    pWordIndex[ wordindex_posi + 14] = _XUN;
//348
    wordindex_posi += 14;
    pWordIndex[ wordindex_posi + 1] = _YA;
    pWordIndex[ wordindex_posi + 2] = _YAN;
    pWordIndex[ wordindex_posi + 3] = _YANG;
    pWordIndex[ wordindex_posi + 4] = _YAO;
    pWordIndex[ wordindex_posi + 5] = _YE;
    pWordIndex[ wordindex_posi + 6] = _YI;
    pWordIndex[ wordindex_posi + 7] = _YIN;
    pWordIndex[ wordindex_posi + 8] = _YING;
    pWordIndex[ wordindex_posi + 9] = _YO;
    pWordIndex[ wordindex_posi + 10] = _YONG;
    pWordIndex[ wordindex_posi + 11] = _YOU;
    pWordIndex[ wordindex_posi + 12] = _YU;
    pWordIndex[ wordindex_posi + 13] = _YUAN;
    pWordIndex[ wordindex_posi + 14] = _YUE;
    pWordIndex[ wordindex_posi + 15] = _YUN;
//363-398
    wordindex_posi += 15;
    pWordIndex[ wordindex_posi + 1] = _ZA;
    pWordIndex[ wordindex_posi + 2] = _ZAI;
    pWordIndex[ wordindex_posi + 3] = _ZAN;
    pWordIndex[ wordindex_posi + 4] = _ZANG;
    pWordIndex[ wordindex_posi + 5] = _ZAO;
    pWordIndex[ wordindex_posi + 6] = _ZE;
    pWordIndex[ wordindex_posi + 7] = _ZEI;
    pWordIndex[ wordindex_posi + 8] = _ZEN;
    pWordIndex[ wordindex_posi + 9] = _ZENG;
    pWordIndex[ wordindex_posi + 10] = _ZHA;
    pWordIndex[ wordindex_posi + 11] = _ZHAI;
    pWordIndex[ wordindex_posi + 12] = _ZHAN;
    pWordIndex[ wordindex_posi + 13] = _ZHANG;
    pWordIndex[ wordindex_posi + 14] = _ZHAO;
    pWordIndex[ wordindex_posi + 15] = _ZHE;
    pWordIndex[ wordindex_posi + 16] = _ZHEN;
    pWordIndex[ wordindex_posi + 17] = _ZHENG;
    pWordIndex[ wordindex_posi + 18] = _ZHI;
    pWordIndex[ wordindex_posi + 19] = _ZHONG;
    pWordIndex[ wordindex_posi + 20] = _ZHOU;
    pWordIndex[ wordindex_posi + 21] = _ZHU;
    pWordIndex[ wordindex_posi + 22] = _ZHUA;
    pWordIndex[ wordindex_posi + 23] = _ZHUAI;
    pWordIndex[ wordindex_posi + 24] = _ZHUAN;
    pWordIndex[ wordindex_posi + 25] = _ZHUANG;
    pWordIndex[ wordindex_posi + 26] = _ZHUI;
    pWordIndex[ wordindex_posi + 27] = _ZHUN;
    pWordIndex[ wordindex_posi + 28] = _ZHUO;
    pWordIndex[ wordindex_posi + 29] = _ZI;
    pWordIndex[ wordindex_posi + 30] = _ZONG;
    pWordIndex[ wordindex_posi + 31] = _ZOU;
    pWordIndex[ wordindex_posi + 32] = _ZU;
    pWordIndex[ wordindex_posi + 33] = _ZUAN;
    pWordIndex[ wordindex_posi + 34] = _ZUI;
    pWordIndex[ wordindex_posi + 35] = _ZUN;
    pWordIndex[ wordindex_posi + 36] = _ZUO;
    //C
#ifdef C_DEBUG
//	Write_Number(0,40,association,4,0,0);
#endif

    mode = 0;
    pinyin_num = 0;
    input_degree = 0;
    pinyin_no = 0;
    page_no = 0;		/*初始化*/
    do
    {
        MEraseWindow(C_PINYIN_HPOSI + C_CCHAR_HDOT*2,C_PINYIN_VPOSI +1,C_PINYIN_HPOSI + C_CCHAR_HDOT*3,C_PINYIN_VPOSI +25);
        CMenuOut(C_PINYIN_HPOSI + C_CCHAR_HDOT*2,C_PINYIN_VPOSI +1, (const u_short*)(_PINYINMODE+mode) ,1,1,24);

        t_ypos = C_PINYIN_VPOSI + 4;
        t_xpos = C_PINYIN_HPOSI + C_CCHAR_HDOT*3 + 8;
      //  Write_Space(t_xpos ,t_ypos,15,1);
        MEraseWindow(t_xpos ,t_ypos,t_xpos+C_CCHAR_HDOT*20 ,t_ypos+C_ECHAR_VDOT+1);
        for(j = 0; j < pinyin_num; j++)
        {
            for(k = 0 ; k < input_degree; k++)
            {
                Write_Ascii(t_xpos ,t_ypos ,num2char[pinyin_posi][k+input_degree*(j+1)+2] +'a' - 'A' );
                t_xpos += C_ECHAR_HDOT;
            }
            Write_Ascii(t_xpos ,t_ypos,' ');
            t_xpos += C_ECHAR_HDOT;
        }		/*显示各种可能的候选拼音*/
        k =  (input_degree +1 ) * pinyin_no;
        t_ypos = C_PINYIN_VPOSI + 4;
        t_xpos = C_PINYIN_HPOSI + C_CCHAR_HDOT*3 + 8 + k * C_ECHAR_HDOT;
        InvertBytes( t_xpos, t_ypos,input_degree ); /*反白显示当前被选中的拼音*/
        k =  input_degree* pinyin_no + input_degree+2;
        for(j = 0 ; j < input_degree; j++)
            input[j] = num2char[pinyin_posi][j+k];/*当前首选拼音存入临时存储区*/
        t_ypos = C_WORD_VPOSI + 12;
        t_xpos = C_WORD_HPOSI + 0;
       // Write_Space(t_xpos ,t_ypos,20,1);	/*写空格*/
        MEraseWindow(t_xpos ,t_ypos,t_xpos+C_CCHAR_HDOT*25 ,t_ypos+C_ECHAR_VDOT+1);

        if(input_degree == 0)
        {
            /*联想前次输入的汉字*/
            if(association > 255)
            {
                for(i = 0; i < C_MAX_ASSOCIATION; i++)
                {
                    if( _Association[i][0] == association )
                    {
                        page_num = 1;
                        page_word_num = _Association[i][1];
                        for(k=0; k<page_word_num; k++)
                        {
                            Write_Ascii(t_xpos, t_ypos, k+48);/*写提示数字0-9*/
                            InvertBytes(t_xpos, t_ypos,1 ); /*反白显示数字*/
                            pzText[0] = CH(_Association[i][k + 2 + page_no*C_MAX_PAGE_WORD] );
                            CMenuOut(t_xpos + C_ECHAR_HDOT,t_ypos, pzText ,1,1,24);
                            t_xpos += 32+16;
                        }	/*根据首选拼音显示相应的汉字*/
                        wordindex_posi = i;
                        break;
                    }
                }
            }
            else
            {
                /*显示高频字*/
                page_num = (_HFWORD[0] + C_MAX_PAGE_WORD- 1 ) / C_MAX_PAGE_WORD ;
                /*当前拼音对应的汉字除以每页能显示的最多字数，得到当前拼音对应的汉字页数*/
                if(page_num == page_no+1)page_word_num = (_HFWORD[0] ) % C_MAX_PAGE_WORD;
                else page_word_num = C_MAX_PAGE_WORD;
                for(k=0; k<page_word_num; k++)
                {
                    Write_Ascii(t_xpos, t_ypos, k+48);/*写提示数字0-9*/
                    InvertBytes(t_xpos, t_ypos,1 ); /*反白显示数字*/
                    pzText[0] = CH(_HFWORD[k + 1 + page_no*C_MAX_PAGE_WORD]) ;
                    CMenuOut(t_xpos + C_ECHAR_HDOT,t_ypos, pzText ,1,1,24);
                    t_xpos += 32+16;
                }	/*根据首选拼音显示相应的汉字*/
                wordindex_posi = 0;
            }
        }
        else
        {
            /*有新的字母输入不联想*/
            for(wordindex_posi = 0; wordindex_posi < C_PINYIN_TOTAL; wordindex_posi++)
            {
                for(k=0; k<input_degree; k++)
                {
                    if(WordIndex[wordindex_posi][k] != input[k])break;
                }
                if(k>=input_degree)break;	//全部比较过说明匹配成功则中断退出
            }
            if(input_degree !=0)
            {
                page_num = (pWordIndex[wordindex_posi][0] + C_MAX_PAGE_WORD- 1 ) / C_MAX_PAGE_WORD ;
                /*当前拼音对应的汉字除以每页能显示的最多字数，得到当前拼音对应的汉字页数*/
                if(page_num == page_no+1)page_word_num = (pWordIndex[wordindex_posi][0] ) % C_MAX_PAGE_WORD;
                else page_word_num = C_MAX_PAGE_WORD;
                if(page_word_num == 0 && page_num != 0)page_word_num = C_MAX_PAGE_WORD;/*当前拼音对应的汉字页中的字数，最大9*/
            }
            else
            {
                page_num = 0;
                page_word_num = 0;
            }
            if(wordindex_posi < C_PINYIN_TOTAL && input_degree>0)
            {
                //匹配成功
                for(k=0; k<page_word_num; k++)
                {
                    Write_Ascii(t_xpos, t_ypos, k+48);/*写提示数字0-9*/
                    InvertBytes(t_xpos, t_ypos,1 ); /*反白显示数字*/
                    pzText[0] = CH(pWordIndex[wordindex_posi][k+1+page_no*C_MAX_PAGE_WORD]);
                    CMenuOut(t_xpos + C_ECHAR_HDOT,t_ypos, pzText ,1,1,24);
                    t_xpos += C_CCHAR_HDOT *2;
                }
            }	/*根据首选拼音显示相应的汉字*/
        }

        while(1)
        {
            if( GetElapsedTime()%400<30)DrawCursor(xpos,ypos,1);/*画光标，mode=0英文1中文*/
            keycode = MGetKey2();//MGetKeyCode(2);
            if(keycode >= 0 && keycode <C_KEYCODMAX)break; /*有新的按键*/
            else
            {
#if C_ROTARY > 0
                keycode = MGetRotary(-1);
                if( keycode > 0 )
                {
                    keycode = C_KEYCOD_PLUS;
                }
                else if( keycode < 0)
                {
                    keycode = C_KEYCOD_MINUS;
                }
                else continue;
                break;
#endif
            }
        }
        for(i = 0; i < C_MAX_SAMEKEYTIME/5; i++)
        {
            ExpendTime(5);
            if(keycode != MGetKey2() )break;//MGetKeyCode(0) )break;
        }/*检查同一按键时间长度，时间较短则是新的输入，如达到预定时间则输出结果，*/

        if(keycode == C_KEYCOD_RETURN && i >= C_MAX_SAMEKEYTIME/5 )
        {
            /*长时间按下返回键*/
            pzText[0] = C_KEYCOD_RETURN;	/*按下键返回键值*/
            break;
        }
        else if(keycode == C_KEYCOD_CONFIRM  && i >= C_MAX_SAMEKEYTIME/5 )
        {
            pzText[0] = C_KEYCOD_CONFIRM;	/*按下确认键返回键*/
            break;
        }
        else if( (keycode >= 0 && keycode <= page_word_num && keycode <= 9) && ( i >= C_MAX_SAMEKEYTIME/5 || mode == 2 ) )
        {
            /*长按0-9键表示选中某一汉字，page_word_num 表示当前页面中的汉字数*/
            //当mode == 2时短按也可
            if(input_degree == 0)	/*有联想*/
            {
                if(association > 255 )
                    pzText[0] = CH(_Association[wordindex_posi][keycode + 2] );
                else pzText[0] = CH(_HFWORD[keycode + 1 + page_no*C_MAX_PAGE_WORD] );
            }
            else /*无联想*/
                pzText[0] = CH(pWordIndex[wordindex_posi][keycode +1 + page_no*C_MAX_PAGE_WORD]);
            break;
        }
        else if(keycode == C_KEYCOD_CONFIRM)//  && i >= C_MAX_SAMEKEYTIME/5 )
        {
            if(input_degree !=0)
            {
                mode++;
                if( mode > 2)mode = 2;
            }
            else
            {
                pzText[0] = C_KEYCOD_CONFIRM;	/*按下确认键返回键*/
                break;
            }
        }
        else if( (keycode > 1 && keycode <= 9) &&  i < C_MAX_SAMEKEYTIME/5 && mode == 0)
        {
            input_num[input_degree] = keycode ;
            input_degree++ ;	/*有效按键次数加1*/

            for(i = 0; i < C_MAX_PINYIN_NUM ; i++)
            {
                /*检查有无与输入相对应的拼音*/
                for(j=0; j < input_degree; j++)
                {
                    if(num2char[i][j] != input_num[j])break ;
                    /*有不同之处则不再比较，跳出比较下一种可能*/
                }
                if(j >= input_degree)
                {
                    /*说明输入与一种拼音相匹配*/
                    if(num2char[i][j] != ' ')
                    {
                        /*部分匹配成功，需自动完成*/
                        for( ; num2char[i][j] != ' '; j++)
                        {
                            /*	*/
                            input_num[input_degree] =  num2char[i][j] ;
                            input_degree++ ;	/*有效按次数加1*/
                        }
                    }
                    pinyin_posi = i;	/*当前输入在num2char数组中的位置*/
                    pinyin_num = num2char[i][input_degree+1];	/*当前输入具有的拼音数*/
                    pinyin_no = 0;
                    page_no = 0;
                    break;
                }
            }
            if(i >= C_MAX_PINYIN_NUM )
            {
                /*全部比较过未发现匹配，输入不合法*/
                if(input_degree>0)input_degree--;/*有效按键次数减1*/
                pinyin_no = 0;
            }
        }
        else if(keycode == C_KEYCOD_DEL )
        {
            /*删除键，按此键后前移*/
            if(input_degree > 0)
            {
                /*有输入时删除拼音，否则返回2表示删除一字*/
                i = input_degree ;
                while( pinyin_posi>0 )
                {
                    pinyin_posi-- ;		/*可能的拼音位置上移1 */
                    for(i=1 ; num2char[pinyin_posi][i] != ' '; i++)	;	/*发现空格，输入结束*/
                    if( i < input_degree)break;		/**/
                }
                if( i < input_degree) input_degree = i;
                else input_degree = 0;
                pinyin_num = num2char[pinyin_posi][input_degree+1];	/*当前输入具有的拼音数改变*/
                pinyin_no = 0;
                page_no = 0;
                mode = 0;
            }
            else
            {
                pzText[0] = C_KEYCOD_DEL;	/*返回表示删除已输入的汉字*/
                break;
            }
        }
        else if(keycode == C_KEYCOD_LEFT)	/*左移候选拼音*/
        {
            if( mode == 0)
            {
                if(input_degree != 0)
                {
                    /*有输入时才能移候选拼音*/
                    if(pinyin_no == 0) pinyin_no = pinyin_num;
                    pinyin_no = (pinyin_no-1) % pinyin_num;
                }
                else
                {
                    pzText[0] = C_KEYCOD_LEFT;	/*返回1表示光标左移*/
                    break;
                }
            }
            else if(mode == 1)
            {
                if( page_no > 0)page_no--;
                else page_no = page_num-1;
            }
        }
        else if(keycode == C_KEYCOD_RIGHT)	/*右移候选拼音*/
        {
            if( mode == 0)
            {
                if(input_degree != 0)
                {
                    /*有输入时才能移候选拼音*/
                    pinyin_no = (pinyin_no+1)%pinyin_num;
                }
                else
                {
                    pzText[0] = C_KEYCOD_RIGHT;	/*返回1表示光标右移*/
                    break;
                }
            }
            else if(mode == 1)
            {
                page_no = (page_no+1) % page_num;
            }
        }
        else if(keycode == C_KEYCOD_PAGEDOWN  &&  i < C_MAX_SAMEKEYTIME/5 )
        {
            /*候选拼音对应的汉字翻页键*/
            page_no = (page_no+1) % page_num;
        }
        else if(keycode == C_KEYCOD_INPUT)	/*切换输入法*/
        {
            MSetInputMode( (MGetInputMode()+1),0);	/*设置当前输入法类型*/
            pzText[0] = 0;		/*返回0表示输入法进行了切换*/
            break;
        }
        else if(keycode == C_KEYCOD_ASCII)	/*各种标点符号*/
        {
            input_degree = 0;
            pzText[0] = MGetSymbol();	/*返回标点符号的ASCII值*/
            break;
        }

    }
    while(1);

    ClearCursor(1);	/*画光标，mode=0英文1中文*/
    return pzText[0];
}
u_short MGetEChar(u_int xpos,u_int ypos,u_short* pzText,u_int mode)		/*得到ASCII字符*/
{
    u_int keycode,Prekeycode = C_KEYCODMAX;
    u_int TimeSlot ;		/*间隔时间*/
    u_int inputmode = MGetInputMode()-0;
    char InputTable[] = {'0','1','2','3','4','5','6','7','8','9','9'+1,
                         '-','_','A','D','G','J','M','P','T','W','Z'+1,
                         '-','_','a','d','g','j','m','p','t','w','z'+1
                        };
    char SymbolTable[] = {'-',':','+',' ','_','/','(',')'};
    u_int KeyInTime = 0;	/*按键次数*/
    int i;
    if(pzText[0] >= 32)
        Prekeycode = pzText[2];	/*前次按下的键值*/
    pzText[1] = 0;
    TimeSlot = GetElapsedTime();
    do
    {
        if(pzText[0] >= 32)Write_Ascii(xpos, ypos, pzText[0]) ;

        while(1)
        {
            if( (GetElapsedTime() - TimeSlot) > 2000 && pzText[0] != 0)
            {
                ClearCursor(0);
                return pzText[0];		/*从上次有效按键到现在有0.5s未按键*/
            }
            if(Prekeycode == C_KEYCODMAX)
                if( GetElapsedTime()%400<5)DrawCursor(xpos,ypos,0);/*画光标，mode=0英文1中文*/

            keycode = MGetKey2();///MGetKeyCode(2);
            if(keycode >=0 && keycode <C_KEYCODMAX)break; /*有新的按键*/
            else
            {
#if C_ROTARY > 0
                keycode = MGetRotary(-1);
                if( keycode > 0 )
                {
                    keycode = C_KEYCOD_PLUS;
                }
                else if( keycode < 0)
                {
                    keycode = C_KEYCOD_MINUS;
                }
                else continue;
                break;
#endif
            }
        }

        for(i = 0; i < C_MAX_SAMEKEYTIME/5; i++)
        {
            ExpendTime(5);
            if(keycode != MGetKey2() )break;//MGetKeyCode(0) )break;
        }/*检查同一按键时间长度，时间较短则是新的输入，如达到预定时间则输出结果，*/

        if(keycode == C_KEYCOD_RETURN && i >= C_MAX_SAMEKEYTIME/5 )
        {
            /*长时间按下返回键*/
            pzText[1] = C_KEYCOD_RETURN;
            break;
        }
        else if(keycode == C_KEYCOD_CONFIRM)//  && i >= C_MAX_SAMEKEYTIME/5 )
        {
            pzText[1] = C_KEYCOD_CONFIRM;	/*按下确认键*/
            break;
        }
        else if(keycode >=0 && keycode <=9 )
        {
            pzText[2] = InputTable[11*inputmode + keycode + 1];
            if(inputmode == 0)
            {
                pzText[0] = InputTable[11*inputmode + keycode];
                break;	/*数字模式*/
            }
            else if (inputmode <= 2)	/*为大写或小写*/
            {
                if (keycode < 2)
                {
                    /*按下0或1,输入符号*/
                    if (KeyInTime == 4 )KeyInTime = 0;
                    pzText[0] = SymbolTable[4 * keycode + KeyInTime];
                }
                else if(Prekeycode == keycode )
                {
                    /*与前次按键相同*/
                    pzText[0]++;
                    if (pzText[0] == pzText[2])
                    {
                        /*已连按同一键多次，回到该键初值*/
                        pzText[0] = InputTable[11*inputmode + keycode];
                        KeyInTime = 0;
                    }
                }
                else if(Prekeycode == C_KEYCODMAX)	/*第一次按键*/
                    pzText[0] = InputTable[11*inputmode + keycode];
            }
            TimeSlot = GetElapsedTime();
        }
        else if(keycode == C_KEYCOD_INPUT)	/*切换输入法*/
        {
            MSetInputMode( (MGetInputMode()+1),0);	/*设置当前输入法类型*/
            pzText[0] = 0;
//			pzText[1] = C_KEYCOD_INPUT;
            break;
        }
        else if(keycode == C_KEYCOD_DEL )
        {
            /*删除键，按此键后前移*/
            pzText[1] = C_KEYCOD_DEL;	/*返回表示删除已输入的字*/
            break;
        }
        else if(keycode == C_KEYCOD_RIGHT)	/*右移*/
        {
//			pzText[1] = C_KEYCOD_RIGHT;	/*返回表示光标右移*/
            break;
        }
        else if(keycode == C_KEYCOD_LEFT)	/*左移*/
        {
//			pzText[1] = C_KEYCOD_LEFT;	/*返回表示光标左移*/
            break;
        }
        //	else if(keycode == C_KEYCOD_PLUS || keycode == C_KEYCOD_MINUS)	//其他按键
        //	{
        //		break;
        //	}
        else if(keycode == C_KEYCOD_ASCII)	/*各种标点符号*/
        {
            pzText[0] = MGetSymbol();	/*返回标点符号的ASCII值*/
            break;
        }
        else
            break;
        if (keycode == Prekeycode)KeyInTime++ ;
        else if ( Prekeycode == C_KEYCODMAX)
        {
            Prekeycode = keycode;	/*如第一次按键则*/
            if( mode > 0 )
            {
                pzText[1] = pzText[0];	/*两个相等说明，插入一个字符*/
                break;
            }
        }
        else	/*第n次按键且与前次按键不同*/
        {
//			if( mode > 0 && KeyInTime == 0 && pzText[0] > 0)
//				pzText[0] = InputTable[11*inputmode + keycode];
            pzText[1] = InputTable[11*inputmode + keycode];
            break;			/*如不是第一次按键则退出*/
        }
    }
    while(1);
    ClearCursor(0);	/*画光标，mode=0英文1中文*/
    pzText[2] = keycode;
    return pzText[0];
}
int MGetQuwei(u_int xpos,u_int ypos,u_short association)
{
    //u_int int_len = 4;
    //u_int deci_len = 0;
    //int retvalue;
    int i,k;
    int t_xpos,t_ypos;
    unsigned short pzText[3];	/*输入的区位码*/
    //unsigned char input[C_MAX_PINYIN_LEN + 1];
    //unsigned char input_num[C_MAX_PINYIN_LEN + 1];	/*所有已输入的数字*/
    unsigned int keycode;	/*当前输入的数字*/
    unsigned char input_degree;	/*有效按键次数*/
    unsigned char page_num = 0;			/*当前拼音对应的汉字页总数，每页9个*/
    unsigned char page_no;			/*当前拼音对应的汉字页数,第几页*/
    unsigned char page_word_num;	/*当前拼音对应的汉字页中的字数，最大9*/
    u_short word_num = 0;	/*当前输入具有的汉字数*/
    u_short wordindex_posi = 0;
    pzText[0] = 0;
    page_no = 0;
    input_degree = 0;
    page_word_num = 0;
    do
    {
        t_ypos = C_PINYIN_VPOSI + 4;
        t_xpos = C_PINYIN_HPOSI + C_CCHAR_HDOT*3 + 8;
        Write_Space(t_xpos ,t_ypos,15,1);
        Write_Number(t_xpos ,t_ypos,pzText[0],4,0,0);

        t_ypos = C_WORD_VPOSI + 12;
        t_xpos = C_WORD_HPOSI + 0;
        Write_Space(t_xpos ,t_ypos,20,1);	/*写空格*/
        if(association > 255 && input_degree == 0)
        {
            /*联想前次输入的汉字*/
            for(i = 0; i < C_MAX_ASSOCIATION; i++)
            {
                if( _Association[i][0] == association )
                {
                    word_num = _Association[i][1];
                    page_num = 1;
                    page_word_num = _Association[i][1];
                    for(k=0; k<page_word_num; k++)
                    {
                        Write_Ascii(t_xpos, t_ypos, k+48);/*写提示数字0-9*/
                        InvertBytes(t_xpos, t_ypos,1 ); /*反白显示数字*/
                        pzText[0] = CH(_Association[i][k + 2] );
                        CMenuOut(t_xpos + C_ECHAR_HDOT,t_ypos, pzText ,1,1,24);
                        t_xpos += C_CCHAR_HDOT *2;
                    }	/*根据首选拼音显示相应的汉字*/
                    wordindex_posi = i;
                    break;
                }
            }
        }
        else
        {
            /*无联想*/
            switch (input_degree)
            {
            case 0:
                word_num = 0;
                break;
            case 1:
                word_num = 1000;
                break;
            case 2:
                word_num = 100;
                break;
            case 3:
                word_num = 10;
                break;
            case 4:
                word_num = 1;
                break;
            }
            page_num = ( word_num + C_MAX_PAGE_WORD -1 )/ C_MAX_PAGE_WORD ;
            /*当前拼音对应的汉字除以每页能显示的最多字数，得到当前拼音对应的汉字页数*/
            if(page_num == page_no+1)page_word_num = word_num % C_MAX_PAGE_WORD;
            else if(page_num != 0)page_word_num = C_MAX_PAGE_WORD;
            if(page_word_num == 0 && page_num != 0)page_word_num = C_MAX_PAGE_WORD;/*当前拼音对应的汉字页中的字数，最大9*/
            for(k=0; k<page_word_num; k++)
            {
                Write_Ascii(t_xpos, t_ypos, k+48);/*写提示数字0-9*/
                InvertBytes(t_xpos, t_ypos,1 ); /*反白显示数字*/
                pzText[1] = CH( pzText[0] * word_num + page_no*C_MAX_PAGE_WORD + k);
                CMenuOut(t_xpos + C_ECHAR_HDOT,t_ypos, pzText + 1 ,1,1,24);
                t_xpos += C_CCHAR_HDOT *2;
            }	/*根据输入区位码显示相应的汉字*/
        }
        if(word_num == 1 && input_degree > 0 )
        {
            pzText[0] = CH(pzText[0]);
            break;
        }
        while(1)
        {
            if( GetElapsedTime()%400<30)DrawCursor(xpos,ypos,1);/*画光标，mode=0英文1中文*/
            keycode = MGetKey2();//MGetKeyCode(2);
            if(keycode >= 0 && keycode <C_KEYCODMAX)break; /*有新的按键*/
            else
            {
#if C_ROTARY > 0
                keycode = MGetRotary(-1);
                if( keycode > 0 )
                {
                    keycode = C_KEYCOD_PLUS;
                }
                else if( keycode < 0)
                {
                    keycode = C_KEYCOD_MINUS;
                }
                else continue;
                break;
#endif
            }
        }

        for(i = 0; i < C_MAX_SAMEKEYTIME/5; i++)
        {
            ExpendTime(5);
            if(keycode != MGetKey2() ) break;//MGetKeyCode(0) )break;
        }/*检查同一按键时间长度，时间较短则是新的输入，如达到预定时间则输出结果，*/

        if( (keycode >= 0 && keycode < page_word_num) &&  i >= C_MAX_SAMEKEYTIME/5 )
        {
            /*长按1-9键表示选中某一汉字，page_word_num 表示当前页面中的汉字数*/
            if(association > 255 && input_degree == 0 && keycode != 0)	/*有联想*/
                pzText[0] = CH(_Association[wordindex_posi][keycode + 1] );
            else /*无联想*/
                pzText[0] = CH( pzText[0] * word_num + page_no*C_MAX_PAGE_WORD + keycode);;
            break;
        }
        else if(keycode == C_KEYCOD_RETURN && i >= C_MAX_SAMEKEYTIME/5 )
        {
            /*长时间按下返回键*/
            pzText[0] = C_KEYCOD_RETURN;	/*按下键返回键值*/
            break;
        }
        else if(keycode == C_KEYCOD_CONFIRM)//  && i >= C_MAX_SAMEKEYTIME/5 )
        {
            pzText[0] = C_KEYCOD_CONFIRM;	/*按下确认键返回键*/
            break;
        }
        else if( (keycode >= 0 && keycode <= 9) &&  i < C_MAX_SAMEKEYTIME/5 )
        {
            pzText[0] = pzText[0] * 10 + keycode ;
            input_degree++;
        }
        else if(keycode == C_KEYCOD_DEL )
        {
            /*删除键，按此键后前移*/
            input_degree--;
            if(pzText[0] > 0)
            {
                /*有输入时删除区位码，否则返回表示删除一字*/
                pzText[0] /= 10;
            }
            else
            {
                pzText[0] = C_KEYCOD_DEL;	/*返回表示删除已输入的汉字*/
                break;
            }
        }
        else if(keycode == C_KEYCOD_LEFT)	/*左移候选拼音*/
        {
//			pzText[0] = C_KEYCOD_LEFT;	/*返回1表示光标左移*/
            break;
        }
        else if(keycode == C_KEYCOD_RIGHT)	/*右移候选拼音*/
        {
//			pzText[0] = C_KEYCOD_RIGHT;	/*返回1表示光标右移*/
            break;
        }
        else if(keycode == C_KEYCOD_PLUS || keycode == C_KEYCOD_MINUS)	/*左移*/
        {
            break;
        }
        else if(keycode == C_KEYCOD_PAGEDOWN  &&  i < C_MAX_SAMEKEYTIME/5 )
        {
            /*候选拼音对应的汉字翻页键*/
            page_no = (page_no+1) % page_num;
        }
        else if(keycode == C_KEYCOD_INPUT)	/*切换输入法*/
        {
            MSetInputMode( (MGetInputMode()+1),0);	/*设置当前输入法类型*/
            pzText[0] = 0;		/*返回0表示输入法进行了切换*/
            break;
        }
        else if(keycode == C_KEYCOD_ASCII)	/*各种标点符号*/
        {
            input_degree = 0;
            pzText[0] = MGetSymbol();	/*返回标点符号的ASCII值*/
            break;
        }

    }
    while(1);

    ClearCursor(1);	/*画光标，mode=0英文1中文*/
    return pzText[0];
}
char MGetSymbol()
{
    char SymbolTable[] =
    {
        '.',',',';','?','!','+','-','x','/','=',
        '_','[',']','(',')','{','}','<','>','\\',
        '~','|','@','#','$','%','^','&','*','`'
    };

    unsigned int keycode;	/*当前输入的数字*/
    unsigned char page_num = 3;			/*当前拼音对应的汉字页总数，每页9个*/
    unsigned char page_no = 0;			/*当前拼音对应的汉字页数,第几页*/
    unsigned char page_word_num = 10;	/*当前拼音对应的汉字页中的字数，最大10*/
    u_int xpos,ypos;
    int i;
    MPushWindow(C_PINYIN_HPOSI,C_PINYIN_VPOSI,C_PINYIN_HPOSI+10*C_CCHAR_HDOT, C_PINYIN_VPOSI + 2 * 30) ;
    MPushWindow(C_PINYIN_HPOSI+10*C_CCHAR_HDOT,C_PINYIN_VPOSI,C_PINYIN_HPOSI+20*C_CCHAR_HDOT, C_PINYIN_VPOSI + 2 * 30) ;
    EraseDrawRectangle(C_WORD_HPOSI,C_WORD_VPOSI,C_WORD_HPOSI+20*C_CCHAR_HDOT, C_WORD_VPOSI + 1 * 30) ;
    do
    {
        ypos = C_WORD_VPOSI + 4;
        xpos = C_WORD_HPOSI + 0;
        Write_Space(xpos ,ypos,20,1);	/*写空格*/
        for(i=0; i<page_word_num && page_no < page_num; i++)
        {
            Write_Ascii(xpos + i*32, ypos, i+48);/*写提示数字0-9*/
            InvertBytes(xpos + i*32, ypos,1 ); /*反白显示数字*/
            Write_Ascii(xpos + i*32+C_ECHAR_HDOT,ypos+C_ECHAR_VDOT, SymbolTable[10*page_no+i]);
        }
        if(page_no >= page_num)
        {
            for(i=0; i<page_word_num; i++)
            {
                Write_Ascii(xpos + i*32, ypos, i+48);/*写提示数字0-9*/
                InvertBytes(xpos + i*32, ypos,1 ); /*反白显示数字*/
                Write_Ascii(xpos + i*32+C_ECHAR_HDOT,ypos+C_ECHAR_VDOT, 'z'+ (page_no - page_num) * page_word_num + i);
            }
        }

        keycode = MAnyKeyReturn();	/*输入一个有效按键*/

        if(keycode == C_KEYCOD_ASCII || keycode == C_KEYCOD_PAGEDOWN)
            page_no = (page_no+1) % page_num;	/*输入标点符号时，标点符号按键即为翻页键*/
        else if( keycode >=0 && keycode <= 9 )
            break;
        else if( keycode == C_KEYCOD_RETURN )
        {
            keycode = -1;
            break;
        }
    }
    while(1);
    MPopWindow() ;
    MPopWindow() ;
    if (keycode == -1)
    {
        return  C_FALSE;
    }
    else
        return  SymbolTable[10*page_no+keycode];
}

