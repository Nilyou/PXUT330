#include <string.h>
#include <stdio.h>

#include "M10Era.h"
#include "tools.h"

const int FixGain	= 245;
unsigned short GainTable[121] =
{
    150,	251,	301,	352,	400,	444,	492,	541,	589,	633,	//0
    684,	733,	782,	835,	885,	932,	981,	1032,	1084,	1135,	//10
    1179,	1225,	1274,	1326,	1380,	1429,	1476,	1525,	1575,	1620,	//20
    1672,	1722,	1773,	1849,	1917,	1972,	2024,	2076,	2129,	2175,	//30
    2222,	2270,	2318,	2364,	2409,	2460,	2510,	2563,	2611,	2657,	//40
    2705,	2753,	2802,	2854,	2902,	2949,	2996,	3045,	3090,	3141,	//50
    3189,	3237,	3279,	3323,	3371,	3420,	3464,	3514,	3554,	3594,	//60
    3634,	3674,	3716,	3756,	3796,	3836,	3876,	3946,	3986,	4026,	//70
    4066,	4106,	4146,	4186,	4226,	4266,	4306,	4346,	4386,	4426,	//80
    4466,	4506,	4546,	4586,	4626,	4666,	4706,	4746,	//110
    4786,
};

char strPrompt[]="Hello World!";

char strPrompt1[]="Press 'C' key to continue....";


int main()
{
    SystemInit(500,500);
//
// Primilary Test
// Print the prompt string,"Hello World".
//
// notes: there must be dead loop at the "main" function
//

    ETextOut(SCREEN_WIDTH/2,SCREEN_HEIGHT/2,strPrompt1,strlen(strPrompt1));


    SetBackgroundColor(COLOR_PINK);
    SetDisplayColor(COLOR_BLUE);
    ETextOut(SCREEN_WIDTH/2,SCREEN_HEIGHT/2,strPrompt,strlen(strPrompt));

//
// Directly manipulate display memroy
//
    int i,j;

    for (j=0; j<SCREEN_HEIGHT; j++)
        for(i=0; i<256; i++)
            drawpixel(i,j,RGB(i,i,i));

    for (j=0; j<SCREEN_HEIGHT; j++)
        for(i=0; i<256; i++)
            drawpixel(256 + i,j,RGB(0,i,0));

    for (j=0; j<SCREEN_HEIGHT; j++)
        for(i=0; i<640-512; i++)
            drawpixel(512 + i,j,RGB(0,0,255-i));

    for(i=0; i<SCREEN_WIDTH; i++)
    {
        drawpixel(i,0,RGB(255,0,0));
        drawpixel(i,SCREEN_HEIGHT-1,RGB(255,0,0));
    }
    for(i=0; i<SCREEN_HEIGHT; i++)
    {
        drawpixel(0,i,RGB(255,0,0));
        drawpixel(SCREEN_WIDTH-1,i,RGB(255,0,0));
    }


//
// Data acquisition and Echo display test
//
    SetEchoRange(1000);
    SetEchoDisplayMode(1,1);
    SetEchoLayout(10,510,60,0);
    SetEchoColor(0, 499,ECHO_COLOR_GREEN);
    UpdateEchoColor();
    EnableEchoDisplay(1);

    EnableDataAcquisition(1);

// Test debugging function through COM port

    int a=0;
    char buf[100];

    while(1)
    {
        memset(buf,0,100);
        sprintf(buf,"Value of a =%d\n\r",a);
        printf("first: %s\r\n",buf);
        printf("second: %d\r\n",a);
        a++;

        delay(1000);
    }

    //memset(buf,0,100);
    //sprintf(buf,"afasdfa %d",a);
    //char ch;
    u_int keyvalue;
    printf("\r\nKey code test programme.\r\n");
    while(1)
    {
        keyvalue = GetKeyCode();
        if(keyvalue)
            printf("%08x\r\n",keyvalue);
        else
            delay(1000);
        //a++;
        //printf("Value a:%d\n\r",a);
        //ch = DBGU_GetChar();
        //printf("ch=%c",ch);

    }



    while(1);

    return 0;
}


