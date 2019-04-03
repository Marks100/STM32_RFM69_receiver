@echo OFF

REM handle being called by COIDE and by user manually :)
if "%1"=="COIDE" (
    call %2\..\BuildEnv\xBuildEnv\setCmdEnv.cmd
) ELSE (
    call ..\xBuildEnv\setCmdEnv.cmd
)

cd %PTLD%\BuildEnv\xBuildEnv\bin\
ruby.exe sVersion -a %PTLD%\Src\
