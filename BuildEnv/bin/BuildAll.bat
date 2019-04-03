@echo OFF
cd ..\xBuildEnv\
call setCmdEnv.cmd && sh -l -c "make all"
pause
