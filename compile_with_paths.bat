@echo off

rem 直接在命令中包含所有必要的路径
cl /EHsc /O2 /D "NDEBUG" test.cpp /I "C:\Program Files\IBM\ILOG\CPLEX_Studio1263\cplex\include" /I "C:\Program Files\IBM\ILOG\CPLEX_Studio1263\concert\include" /link /LIBPATH:"C:\Program Files\IBM\ILOG\CPLEX_Studio1263\cplex\lib\x64_win64\msvc12" /LIBPATH:"C:\Program Files\IBM\ILOG\CPLEX_Studio1263\concert\lib\x64_win64\msvc12" cplex1263.lib concert.lib ilocplex.lib

pause
