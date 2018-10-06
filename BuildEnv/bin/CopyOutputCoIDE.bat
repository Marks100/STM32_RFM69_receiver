@echo OFF
call ../xBuildEnv/setCmdEnv.cmd
sh -l -c "CopyOutputCoIDE %1"
pause