#ifndef _TOOLS_H_
#define _TOOLS_H_

#include "M10Era.h"

#define RGB(r, g ,b)  ((u_int) (((u_char) (r) | \
    ((u_short) (g) << 8)) | \
    (((u_int) (u_char) (b)) << 16)))


void delay(u_int interval);
u_int drawpixel(u_int x,u_int y,u_int color);

void   _trace(int x, int y,char   *prompt,   ...);

//#define _DEBUG

#ifdef _DEBUG
#define   TRACE      // _trace
#else
#define   TRACE
#endif

extern int g_ypos;
extern int g_xpos;


#endif
