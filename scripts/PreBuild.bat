@echo off

REM handle being called by COIDE and by user manually :)
if "%1"=="COIDE" (
    cd %2\..\BuildEnv\
) ELSE (
    cd ..\BuildEnv\
)

call SetCmdEnv.bat

cd %PTLD%\BuildEnv\bin\
ruby.exe sVersion -a %PTLD%\Src\
