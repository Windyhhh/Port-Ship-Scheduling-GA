@echo off

rem 设置CPLEX_Studio1263的路径
set CPLEX_DIR=C:\Program Files\IBM\ILOG\CPLEX_Studio1263
set CONCERT_DIR=%CPLEX_DIR%\concert

rem 显示当前设置
echo CPLEX_DIR: %CPLEX_DIR%
echo CONCERT_DIR: %CONCERT_DIR%

rem 提示用户在Visual Studio命令提示符中运行
 echo.  
echo 请在Visual Studio 2022的"x64 Native Tools Command Prompt"中运行以下命令：
echo.  
echo set CPLEX_DIR=C:\Program Files\IBM\ILOG\CPLEX_Studio1263
echo set CONCERT_DIR=%%CPLEX_DIR%%\concert
echo set INCLUDE=%%CPLEX_DIR%%\cplex\include;%%CONCERT_DIR%%\include;%%INCLUDE%%
echo set LIB=%%CPLEX_DIR%%\cplex\lib\x64_win64\msvc12;%%CONCERT_DIR%%\lib\x64_win64\msvc12;%%LIB%%
echo cl /EHsc /O2 /D "NDEBUG" test.cpp /link cplex1263.lib concert.lib ilocplex.lib
echo.  
echo 然后运行：
echo test.exe

pause
