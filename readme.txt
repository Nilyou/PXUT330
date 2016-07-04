1. main.c文件中下面一行注释掉
   SetUsbMode(1);

2.keyio.c文件
   int TextOut(int x,int y,int row,int len,int height, u_char* szText, u_char dots)
   上面函数申明中u_char dots 引起编译错误，改成int dots

3. user.c文件
  void SysPwd(int mode)函数中把一些数组常量放到函数体外

4.
    It must rebuild the library of libudkfat.a, at the present time ,I temporarily committed all functions calling from the
libudkfat.a in file of 'udisk.c' and 'armtest.c'.

=====================================================

Modified Record of libm10era.a

CopyProtectedMemory   -->   _opyProtectedMemory
CTextOut              -->    _CextOut
ETextOut              -->   _EextOut
DrawLine              -->   _rawLine
DrawPixel             -->   _rawPixel
GetPixel              -->   _etPixel
GetKeyCode            -->   _etKeyCode
EraseWindow           -->   _raseWindow
InvertWindow          -->   _nvertWindow
PushWindow            -->   _ushWindow
PopWindow             -->   _ushWindow

