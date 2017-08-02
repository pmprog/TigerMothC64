@echo off

..\TASM\tasm -65 -b tigermoth.asm
copy tigermoth.obj tigermoth.prg
del tigermoth.obj

pause

..\WinVICE-3.1-x64\x64 tigermoth.prg
