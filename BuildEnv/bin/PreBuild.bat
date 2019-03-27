@echo OFF
call %1\..\BuildEnv\xBuildEnv\setCmdEnv.cmd
cd %PTLD%\BuildEnv\xBuildEnv\bin\
ruby.exe sVersion -a %PTLD%\Src\
pause