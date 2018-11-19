@echo OFF

:: BuildEnv Version number
set BENV_VERSION=2.1.3.RC

:: We want to use a tool that has been installed "natively" on the target machine.  This will always be in "Program Files"
:: when installed to the installer recommended location.
:: On a 64 bit machine running 32 bit process this will be wrong <Program Files (x86)> but we can change below
@set PROGFILES_PATH=%ProgramFiles%

:: If this variable exists then we are on 64 bit machine, point to the 64bit install of Tortoise regardless of process type we are running
@IF "%ProgramW6432%" == "" GOTO NOCHANGE
@set PROGFILES_PATH=%ProgramW6432%
:NOCHANGE

:: A list of common tools to always be in the PATH - be careful here, might be better to use more PROJ specific ones - it's more future proof
set COMMON_TOOLS=^
%BENV_CORE%;^
%BENV_CORE%\bin;^
%TTLD%\7zip;^
%TTLD%\ADTools;^
%TTLD%\CodeBlocks;^
%TTLD%\Doxygen;^
%TTLD%\FlexLM;^
%TTLD%\GraphViz\bin;^
%TTLD%\mcpp-2.7.2\bin;^
%TTLD%\PerlScripts\lcov-1.10\bin;^
%TTLD%\Python27;^
%TTLD%\Python27\Tools\Scripts;^
%TTLD%\Ruby\bin;^
%TTLD%\RubyScripts;^
%TTLD%\SlikSvn\bin;^
%PROGFILES_PATH%\USB Redirector Client;^
%PROGFILES_PATH%\TortoiseSVN\bin;^
%PBCD%\bin;^
%TTLD%\gcc-arm\bin;

:: Git path
set GIT_PATH=%TTLD%\Git_x64\bin;%TTLD%\Git\bin

:: Pick right version of clink
if "%PROCESSOR_ARCHITECTURE%"=="x86" (
    set architecture=86
) else (
    set architecture=64
)

:: Set up a nice lambda prompt and supports git
if defined ConEmuBuild (
    prompt $E[1;32;40m$P$S{git}$S$_$E[1;30;40m{lamb}$S$E[0m

    :: Run clink
	"%TTLD%\clink\clink_x%architecture%.exe" inject --quiet --profile "%BENV_CORE%\config"
)

:: Add aliases
doskey /macrofile="%BENV_CORE%\config\aliases"


:: Set home path
set HOME=%USERPROFILE%

:: Create final PATH
set PATH=.\;%SEL_EXTRA_TOOLS%;%COMMON_TOOLS%;%GIT_PATH%;%SEL_GCC%\bin;%SYSTEMROOT%\system32;%SYSTEMROOT%;%SYSTEMROOT%\System32\Wbem

:: Change to project directory
cd %PTLD%

:: Run the tools check if in GUI mode
if defined ConEmuBuild (
    ruby %BENV_CORE%\bin\svn-tools-check
    cls
)

:: Set up the BENV_USERNAME - Read in from file if it exists in case we are offline and for optimising
set BENVUSER_FILE="%USERPROFILE%\.benv_user"

:: Skip BENV_USERNAME setup if we don't have the ad-tool
if not exist %BENV_CORE%\bin\ad-tool goto END_FILE

if exist "%BENVUSER_FILE%" GOTO LOAD_BENV_USERNAME
@echo INFO: Setting BENV_USERNAME from Active Dirctory Lookup
ruby %BENV_CORE%\bin\ad-tool sel_user >"%BENVUSER_FILE%"

:LOAD_BENV_USERNAME
set /p BENV_USERNAME=<"%BENVUSER_FILE%"
set BENVUSER_FILE=

:END_FILE
