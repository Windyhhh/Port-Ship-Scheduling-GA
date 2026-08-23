@echo off

rem 启动Visual Studio 2022的开发人员命令提示符，并在其中执行编译命令
cmd /k ""C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\Tools\VsDevCmd.bat" && set CPLEX_DIR=C:\Program Files\IBM\ILOG\CPLEX_Studio1263 && set CONCERT_DIR=%CPLEX_DIR%\concert && set INCLUDE=%CPLEX_DIR%\cplex\include;%CONCERT_DIR%\include;%INCLUDE% && set LIB=%CPLEX_DIR%\cplex\lib\x64_win64\msvc12;%CONCERT_DIR%\lib\x64_win64\msvc12;%LIB% && echo 编译test.cpp使用CPLEX_Studio1263... && cl /EHsc /O2 /D "NDEBUG" test.cpp /link cplex1263.lib concert.lib ilocplex.lib && echo 编译完成，运行test.exe... && test.exe"

pause
