:: This does the same thing as SelBuildEnv.exe but for a stand alone Windows command environment
:: This can be used when calling project batch files

:: Where is the core BUILD ENV
@set BENV_CORE=%~dp0

:: set up the Project Top Level Directory (PTLD) and Project Build Config Directory (PBCD)
:: PROJECT_DIR
::   |- BuildEnv
::      |- setup_env.bat
@cd %BENV_CORE%\..
@set PBCD=%cd%
@cd ..
@set PTLD=%cd%

:: Setup the Tools Top Level Directory (TTLD)
@set TTLD=\SVN_source\Tools

:: Grab the PROJECT_DIR name as the project title
@for %%i in ("%cd%") do @set PROJ_TITLE=%%~ni

:: Set up the project specific stuff
@call %PBCD%\setup_env.bat
