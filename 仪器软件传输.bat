@echo off 

rem set /P var=请输入当前的端口号:7

echo 仪器软件传输
cd C:\cygwin\home\PXUT330
M10BurnUser userapp.bin 

rem %var%

echo 仪器软件传输完毕。

@echo off 
pause