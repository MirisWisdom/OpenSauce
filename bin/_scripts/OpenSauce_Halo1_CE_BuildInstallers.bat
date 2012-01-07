@ECHO OFF

:: Useful references
REM batch parameters - http://www.microsoft.com/resources/documentation/windows/xp/all/proddocs/en-us/percent.mspx?mfr=true
REM querying the reg - http://www.robvanderwoude.com/ntregistry.php
REM strings - http://www.dostips.com/DtTipsStringManipulation.php

:: Setup some common values

REM SET RegProcessorIdQuery="reg query HKLM\Hardware\Description\System\CentralProcessor\0 /v Identifier"
REM SET RegProcessorIdValue=""
REM REM delims is a TAB followed by a space
REM FOR /F "tokens=3 delims=	 " %%A IN ('%RegProcessorIdQuery%') DO SET RegProcessorIdValue=%%A

REM Setup the paths we need to execute external programs
IF "%PROCESSOR_ARCHITECTURE%" == "x86" GOTO PROCESSOR_IS_32BIT
SET "ProgFilesDir=%PROGRAMFILES%"
SET "ProgFilesDir86=%PROGRAMFILES(x86)%"
GOTO SET_PROGFILES_END
:PROCESSOR_IS_32BIT
SET "ProgFilesDir=%PROGRAMFILES%"
SET "ProgFilesDir86=%PROGRAMFILES%"
:SET_PROGFILES_END

REM We're currently using VS2008
REM We have to use this as I've seen VS install using different (default) paths. Plus, someone may have overrided their shit...
SET VS_COMMON_IDE="%VS90COMNTOOLS%"
IF %VS_COMMON_IDE%=="" GOTO INVALID_VS
SET VS_COMMON_IDE="%VS_COMMON_IDE%..\IDE\"

FOR /F "tokens=1,2,3 delims=/ " %%A IN ('DATE /t') DO @(
    SET DateDay=%%A
    SET DateMonth=%%B
    SET DateYear=%%C
    SET DateAll=%%C%%B%%A
)

REM We assume the bat's directory is \bin\_scripts\
SET "BatDir=%~dp0"
REM Thus we have to go up two directories to get root
SET "osRepoDir=%BatDir%..\..\"
SET "osBinPath=%osRepoDir%bin"
SET "osBuildLogPath=%osBinPath%\log"

REM The log directory is not automatically created when building
IF NOT EXIST "%osBuildLogPath%" mkdir "%osBuildLogPath%"

SET "osInstallerSolution=%osRepoDir%OpenSauce\OpenSauce_Installer.sln"

:: Run the meat of this script

ECHO Compiling Debug configuration
SET "BuildLogFile=%osBuildLogPath%\log_debug.log"
ECHO Log: "%BuildLogFile%"
ECHO.

"%VS_COMMON_IDE%devenv.exe" /rebuild Debug %osInstallerSolution% /out %BuildLogFile%

ECHO Compiling Release configuration
SET "BuildLogFile=%osBuildLogPath%\log_release.log"
ECHO Log: "%BuildLogFile%"
ECHO.

"%VS_COMMON_IDE%devenv.exe" /rebuild Release %osInstallerSolution% /out %BuildLogFile%

ECHO Compiling ReleaseSymbols configuration
SET "BuildLogFile=%osBuildLogPath%\log_releasesymbols.log"
ECHO Log: "%BuildLogFile%"
ECHO.

"%VS_COMMON_IDE%devenv.exe" /rebuild ReleaseSymbols %osInstallerSolution% /out %BuildLogFile%

:: Copy the build installers

SET "FolderName=Halo1_CE_%DateAll%"
SET "BuildPath=%BatDir%OpenSauce_Builds\%FolderName%"
IF NOT EXIST "%BuildPath%" mkdir "%BuildPath%"

REM Delete existing installers to be sure we don't archive old builds when building multiple times in a day, and a later build fails
FOR %%A IN (%BuildPath%\*.*) DO DEL %%A

FOR %%A IN ("%osBinPath%\Release\x86\OpenSauce_Installer\*.msi") DO copy %%A "%BuildPath%\%%~nA.msi"
FOR %%A IN ("%osBinPath%\ReleaseSymbols\x86\OpenSauce_Installer\*.msi") DO copy %%A "%BuildPath%\%%~nA_ReleaseSymbols.msi"
FOR %%A IN ("%osBinPath%\Debug\x86\OpenSauce_Installer\*.msi") DO copy %%A "%BuildPath%\%%~nA_Debug.msi"
ECHO.

REM Count the number of files in the build path, if less than 3 something didn't build correctly
SET /A "MSICount=0"
FOR %%A IN ("%BuildPath%\*.msi") DO SET /A "MSICount+=1"
IF "%MSICount%" NEQ "3" GOTO BUILD_FAILED

:: Archive the installers

SET "ArchiveExe=%ProgFilesDir%\7-Zip\7z.exe"

REM If the user does not have 7-Zip installed skip archive creation
IF NOT EXIST "%ArchiveExe%" GOTO ZIP_MISSING

ECHO Creating build archive using 7-Zip
ECHO Output: "%BuildPath%\..\%FolderName%.7z"

FOR /f %%A IN ("%BuildPath%\*.msi") DO "%ArchiveExe%" a -t7z "%BuildPath%\..\%FolderName%.7z" "%%A"
GOTO END

:INVALID_VS
ECHO ERROR: VisualStudio is either not installed or an unsupported version is used
GOTO END

:ZIP_MISSING
ECHO WARNING: 7-Zip is not installed, build archive creation has been skipped
GOTO END

:BUILD_FAILED
ECHO ERROR: An installer is missing, one or more of the project configurations may have failed to build
GOTO END

:END
PAUSE
