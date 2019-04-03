@echo off

REM set the title
title=%~n0

REM check for no arguments
if [%~1]==[] goto NO_ARGS


SET MAKEFILE_DIR=%~1
SET OLD_BUILD_DIR=%~2
SET NEW_BUILD_DIR=%~3

IF NOT EXIST %NEW_BUILD_DIR% (
echo Build output folder did not exist
echo Creating Build output folder..
mkdir %NEW_BUILD_DIR%
)

IF NOT EXIST %NEW_BUILD_DIR%\object_files (
echo Object files folder did not exist
echo Creating Object files folder..
mkdir %NEW_BUILD_DIR%\object_files
)

del %NEW_BUILD_DIR%\ *.* /F /Q

echo Copying Build output to new folder location..
XCOPY "%OLD_BUILD_DIR%" "%NEW_BUILD_DIR%" /E /I /Y
echo.

echo Copying Objec files to new folder location..
XCOPY "%OLD_BUILD_DIR%\..\obj" "%NEW_BUILD_DIR%\object_files" /E /I /Y
echo.

REM cd %MAKEFILE_DIR% 
REM call BuildEnv\xBuildEnv\setCmdEnv.cmd && sh -l -c "make generate_memory_stats"
goto END


:NO_ARGS
echo INVALID ARGUMENTS!!, This batchfile cannot be ran without the correct 
echo arguments as it can cause harm
echo.
echo Argument 1.. MAKEFILE_DIR  = The directory of the project makefile.
echo Argument 2.. OLD_BUILD_DIR = The directory of the compiled build output.
echo Argument 3.. NEW_BUILD_DIR = The new directory for the compiled build output to be copied to.

TIMEOUT /t 10


REM EOF
:END






