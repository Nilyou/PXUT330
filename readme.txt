1. main.c�ļ�������һ��ע�͵�
   SetUsbMode(1);

2.keyio.c�ļ�
   int TextOut(int x,int y,int row,int len,int height, u_char* szText, u_char dots)
   ���溯��������u_char dots ���������󣬸ĳ�int dots

3. user.c�ļ�
  void SysPwd(int mode)�����а�һЩ���鳣���ŵ���������

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

