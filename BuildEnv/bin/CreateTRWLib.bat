@echo OFF
cd ..\..\TPMS_SWC\PROJ_TrwBuildEnv

mkdir TempObj
copy .\MainMicro\Application\CustBsw\Rdci\RdciMain\BUILD\KBMI17_MM_APP\OBJ\*.47R TempObj\.
.\Tools\CodeGen\Bin\armar.exe ru BUILD\KBMI17_MM_APP\Rdci_full .\TempObj\*.47R
.\Tools\CodeGen\Bin\armstrip.exe .\TempObj\*.47R
copy .\MainMicro\Application\CustBsw\Rdci\RdciMain\BUILD\KBMI17_MM_APP\OBJ\SwcRdciComHdlA.47R TempObj\.
copy .\MainMicro\Application\CustBsw\Rdci\RdciMain\BUILD\KBMI17_MM_APP\OBJ\SwcRdciWarn.47R TempObj\.
copy .\MainMicro\Application\CustBsw\Rdci\RdciMain\BUILD\KBMI17_MM_APP\OBJ\SwcRdciAnz.47R TempObj\.
copy .\MainMicro\Application\CustBsw\Rdci\RdciMain\BUILD\KBMI17_MM_APP\OBJ\SwcRdciComHdlQm.47R TempObj\.
copy .\MainMicro\Application\CustBsw\Rdci\RdciMain\BUILD\KBMI17_MM_APP\OBJ\SwcRdciDat.47R TempObj\.
copy .\MainMicro\Application\CustBsw\Rdci\RdciMain\BUILD\KBMI17_MM_APP\OBJ\SwcRdciErfs.47R TempObj\.
copy .\MainMicro\Application\CustBsw\Rdci\RdciMain\BUILD\KBMI17_MM_APP\OBJ\VERSIONS.47R TempObj\.
copy .\MainMicro\Application\CustBsw\Rdci\RdciMain\BUILD\KBMI17_MM_APP\OBJ\CODING_RDCI.47R TempObj\.
.\Tools\CodeGen\Bin\armar.exe ru BUILD\KBMI17_MM_APP\Rdci_stripped .\TempObj\*.47R
del /f /s /q .\TempObj
pause
