@echo off

call :DEL_FILE

:DEL_FILE
for /d %%i in (*.*) do (cd %%i
del /s *.ncb
del /s *.opt
del /s *.plg
del /s *.aps
del /s *.fd
del /s *.bak
del /s *.*.bak
del /s *.positions
del /s/a Thumbs.db
del /s *.exe
del /s *.pdb
del /s *.~*
del /s *.obj
del /s *.tds

del /s *.run
del /s *.fnr

rd /s/q debug
rd /s/q debugu
rd /s/q release
rd /s/q releaseu
rd /s/q ReleaseMinDependency
rd /s/q ReleaseMinSize
rd /s/q ReleaseUMinDependency
rd /s/q ReleaseUMinSize
rd /s/q ipch
rd /s/q bin
del *.sdf
del *.user
del *.filters
del Simulation*.log

REM vld files
del dbghelp.dll
del vld.h
del vldapi.h
del vld.lib
del vldmt.lib
del vltmtdll.lib
cd ..)
GOTO :EOF

