@echo off
..\bin\ml /nologo /c /coff /Zi /Fo temp.obj temp.asm
..\bin\link /nologo /debug /subsystem:console /entry:start /out:temp.exe temp.obj
.\temp.exe
pause