#include <string.h>
#include "M10Era.h"

#define X_COEF_ENLARGE 1
#define Y_COEF_ENLARGE 1


/* display chinese character string pointed to (pzText) with length (len) at (x,y) */
void 	CTextOut(u_int x, u_int y, u_short* pzText, u_int len)
{
    _CextOut(X_COEF_ENLARGE*x,Y_COEF_ENLARGE*y,pzText,len);

}
/* display english character string pointed to (pzText) with length (len) at (x,y) */
void 	ETextOut(u_int x, u_int y, char* pzText, u_int len)
{
    _EextOut(X_COEF_ENLARGE*x,Y_COEF_ENLARGE*y,pzText,len);
}
/* draw a line from point(x1,y1) to (x2,y2), including both points */
void 	DrawLine(u_int x1, u_int y1, u_int x2, u_int y2)
{
    _rawLine(X_COEF_ENLARGE*x1,Y_COEF_ENLARGE*y1,X_COEF_ENLARGE*x2,Y_COEF_ENLARGE*y2);
}
/* draw a point at (x,y) */
void 	DrawPixel(u_int x, u_int y)
{
        _rawPixel(X_COEF_ENLARGE*x,Y_COEF_ENLARGE*y);
}
/* get one screen pixel color value from (x,y) to (x,y) */
u_int 	GetPixel(u_int x, u_int y)
{
    u_int color;
    color = _etPixel(X_COEF_ENLARGE*x,Y_COEF_ENLARGE*y);
    return color;
}


void 	EraseWindow(u_int left, u_int top, u_int width, u_int height)
{
        _raseWindow(left*X_COEF_ENLARGE,top*Y_COEF_ENLARGE,width*X_COEF_ENLARGE,height*Y_COEF_ENLARGE);
        //_raseWindow(left,top,width,height);
}

void 	InvertWindow(u_int left, u_int top, u_int width, u_int height)
{
        _nvertWindow(left*X_COEF_ENLARGE,top*Y_COEF_ENLARGE,width*X_COEF_ENLARGE,height*Y_COEF_ENLARGE);
        //_nvertWindow(left,top,width,height);

}
void 	PushWindow(u_int left_sd, u_int top_sd, u_int width, u_int height)
{
        _ushWindow(left_sd*X_COEF_ENLARGE,top_sd*Y_COEF_ENLARGE,width*X_COEF_ENLARGE,height*Y_COEF_ENLARGE);
        //_ushWindow(left_sd,top_sd,width,height);
}
void	PopWindow(u_int left_sd, u_int top_sd, u_int width, u_int height)
{
        _opWindow(left_sd*X_COEF_ENLARGE,top_sd*Y_COEF_ENLARGE,width*X_COEF_ENLARGE,height*Y_COEF_ENLARGE);
        //_opWindow(left_sd,top_sd,width,height);

}

//char MEMORY_STORAGE[512*1024];

int	CopyProtectedMemory(void* memdes, void* memsrc, u_int bytesize, u_int copymode)
{
    int rtn;
    rtn = _opyProtectedMemory(memdes,memsrc,bytesize,copymode);

/*    switch(copymode)
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
    }*/
    return rtn;
}

int	CopyProtectedMemoryToFlash(void* memdes, void* memsrc, u_int bytesize, u_int copymode)
{
    int rtn;
    rtn = _opyProtectedMemory(memdes,memsrc,bytesize,copymode);

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
}

u_int 	GetKeyCode(void)
{
    u_int keycode;

    keycode =_etKeyCode();
       char buf[30];
       memset(buf,0,30);
       sprintf(buf,"KeyCode=%08x",keycode);
       ETextOut(340, 460,buf,strlen(buf));

    return keycode;
}


/*
void HexDump(int x,int y,char *buf, int length)
{
    char table[200];
    int i;
    memset(table,0,200);
    for(i=0;i<length;i++)
        sprintf(table + i*3,"%02x ",buf[i]);
    EraseWindow(x,y,strlen(table)*8,8);
    ETextOut(x,y,table,strlen(table));

}


*/
