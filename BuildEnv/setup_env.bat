@set SEL_GCC=%TTLD%\MinGW\TDM32_481_3

:: Setup Lauterbach Trace32 specific paths
@set SEL_T32=%TTLD%\Lauterbach\Trace32\2013_04\bin\windows

:: test\support is added to pick up binlog.dll
@set SEL_EXTRA_TOOLS=%PTLD%\TPMS_SWC\test\support;%SEL_T32%;%SEL_VSX%;%SEL_AB%;%TTLD%\GHS\comp_201355;%TTLD%\TortoiseSVN;%TTLD%\PRQA\8.2\common\bin;%TTLD%\gcc-arm\bin;%TTLD%\Teraterm;

:: Call standard core post_config - No need to edit this
@call %BENV_CORE%\post_config.cmd