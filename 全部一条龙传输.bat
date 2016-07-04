
@echo off 
rem set /P var=请输入当前的端口号:

rem burn authenticating code 内核
echo 内核软件传输
     
M10BurnKern.exe som910_m10ufd_kernel.bin 
echo 内核软件传输完毕，请关闭仪器。    
pause 

rem  内核
echo 仪器软件传输

M10BurnUser userapp.bin 

echo 仪器软件传输完毕。

@echo off 