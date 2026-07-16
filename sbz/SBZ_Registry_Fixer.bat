@echo off

ECHO This tool was created for ADHentai by YioGames and reworked by VTRinNights
ECHO to fix registry errors in Illusion Games.
ECHO All thanks go to fenris666 for the code.
ECHO.
IF EXIST "%~dp0SexyビーチZERO.exe" GOTO doWork
IF EXIST "%~dp0Sexy Beach Zero English.exe" GOTO doWork


ECHO Error
ECHO.
ECHO Found neither "SexyビーチZERO.exe" nor "Sexy Beach Zero English.exe" (e.g. "SchoolMate.exe" ) in "%~dp0"!
ECHO.
ECHO Please, add the Launcher name on "IF EXIST "%~dp0LAUNCHER NAME" GOTO doWork"

cmd /k

:doWork

REG ADD "HKEY_CURRENT_USER\Software\illusion\SexyBeachZERO" /ve /f
ECHO Koikatu added to registry

REG ADD "HKEY_CURRENT_USER\Software\illusion\SexyBeachZERO" /v "INSTALLDIR" /d "%~dp0\" /f
ECHO INSTALLDIR added to registry

ECHO.
ECHO The path "%~dp0" is now correctly registered.

PAUSE
exit