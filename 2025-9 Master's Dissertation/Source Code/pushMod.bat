@echo off
setlocal

REM === CONFIGURATION ===
set "GAME_DIR=C:\Program Files (x86)\Steam\steamapps\common\Awaria\Awaria_Data\Managed"
set "SRC_ASSEMBLY_CSHARP=vsdecompile\Assembly-CSharp\bin\Debug\Assembly-CSharp.dll"
set "SRC_AWARIA_INTERFACE=DLL_Mod\bin\Debug\AwariaInterface.dll"

REM === Push new files ===
echo Copying new DLLs...
copy /Y "%SRC_ASSEMBLY_CSHARP%" "%GAME_DIR%\Assembly-CSharp.dll"
copy /Y "%SRC_AWARIA_INTERFACE%" "%GAME_DIR%\AwariaInterface.dll"

REM === Done ===
echo Mod files pushed successfully.
pause
endlocal
