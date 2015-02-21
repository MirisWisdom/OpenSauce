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

REM We're currently using VS2013
REM We have to use this as I've seen VS install using different (default) paths. Plus, someone may have overrided their shit...
SET VS_COMMON_IDE="%VS120COMNTOOLS%"
IF %VS_COMMON_IDE%=="" GOTO INVALID_VS
SET VS_COMMON_IDE="%VS_COMMON_IDE%..\IDE\"

REM Your machine's globalization settings will determine the values of A,B,C
REM EG, on my machine I have YY/MM/DD, but the default format is DD/MM/YY
REM YY/MM/DD is a better raw sorting format
FOR /F "tokens=1,2,3 delims=/ " %%A IN ('DATE /t') DO @(
    SET DateDay=%%A
    SET DateMonth=%%B
    SET DateYear=%%C
    SET DateAll=%%A%%B%%C
)
FOR /F "tokens=1,2,3 delims=: " %%A IN ('TIME /t') DO @(
    SET TimeHr=%%A
    SET TimeMin=%%B
    SET TimePrd=%%C
)

REM We assume the bat's directory is \bin\_scripts\
SET "BatDir=%~dp0"
REM Thus we have to go up two directories to get root
SET "osRepoDir=%BatDir%..\..\"
SET "osBinPath=%osRepoDir%bin"


SET "osInstallerSolution=%osRepoDir%OpenSauce\OpenSauce_Installer.sln"


:: Parse optional arguments
SET ExpectedMsiCount=3
SET BuildDebug=1
SET BuildRelease=1
SET BuildReleaseSymbols=1
REM We quoting the arguments so unspecified args expand to just ""
IF "%1"=="0" (
	SET /A ExpectedMsiCount-=1
	SET /A BuildDebug=0
)
IF "%2"=="0" (
	SET /A ExpectedMsiCount-=1
	SET /A BuildRelease=0
)
IF "%3"=="0" (
	SET /A ExpectedMsiCount-=1
	SET /A BuildReleaseSymbols=0
)


:: Run the meat of this script
IF %ExpectedMsiCount% NEQ 0 (
	ECHO Starting build at %TimeHr%:%TimeMin% %TimePrd%
	ECHO Building %ExpectedMsiCount% installer[s]
) ELSE @ECHO WARNING: No MSI files are going to be built...
ECHO.

IF %BuildDebug% NEQ 0 (
	ECHO Compiling Debug configuration
	ECHO.

	"%VS_COMMON_IDE%devenv.exe" /rebuild Debug %osInstallerSolution%
)
IF %BuildRelease% NEQ 0 (
	ECHO Compiling Release configuration
	ECHO.

	"%VS_COMMON_IDE%devenv.exe" /rebuild Release %osInstallerSolution%
)
IF %BuildReleaseSymbols% NEQ 0 (
	ECHO Compiling ReleaseSymbols configuration
	ECHO.

	"%VS_COMMON_IDE%devenv.exe" /rebuild ReleaseSymbols %osInstallerSolution%
)


:: Copy the build installers

SET "FolderName=Halo1_CE_%DateAll%"
SET "BuildPath=%BatDir%OpenSauce_Builds\%FolderName%"
IF NOT EXIST "%BuildPath%" mkdir "%BuildPath%"

REM Delete existing installers to be sure we don't archive old builds when building multiple times in a day, and a later build fails
FOR %%A IN (%BuildPath%\*.*) DO DEL %%A

IF %BuildDebug% NEQ 0 (
	FOR %%A IN ("%osBinPath%\Debug\x86\OpenSauce_Installer\*.msi") DO copy %%A "%BuildPath%\%%~nA_Debug.msi"
	mkdir "%BuildPath%\Debug"
	copy "%osBinPath%\Debug\Win32\Halo1_CE\dinput8.dll" "%BuildPath%\Debug\dinput8.dll"
	copy "%osBinPath%\Debug\Win32\Halo1_CE\dinput8.pdb" "%BuildPath%\Debug\dinput8.pdb"
	copy "%osBinPath%\DebugDedi\Win32\Halo1_CE\OpenSauceDedi.dll" "%BuildPath%\Debug\OpenSauceDedi.dll"
	copy "%osBinPath%\DebugDedi\Win32\Halo1_CE\OpenSauceDedi.pdb" "%BuildPath%\Debug\OpenSauceDedi.pdb"
)
IF %BuildRelease% NEQ 0 (
	FOR %%A IN ("%osBinPath%\Release\x86\OpenSauce_Installer\*.msi") DO copy %%A "%BuildPath%\%%~nA.msi"
	mkdir "%BuildPath%\Release"
	copy "%osBinPath%\Release\Win32\Halo1_CE\dinput8.dll" "%BuildPath%\Release\dinput8.dll"
	copy "%osBinPath%\Release\Win32\Halo1_CE\dinput8.pdb" "%BuildPath%\Release\dinput8.pdb"
	copy "%osBinPath%\ReleaseDedi\Win32\Halo1_CE\OpenSauceDedi.dll" "%BuildPath%\Release\OpenSauceDedi.dll"
	copy "%osBinPath%\ReleaseDedi\Win32\Halo1_CE\OpenSauceDedi.pdb" "%BuildPath%\Release\OpenSauceDedi.pdb"
)
IF %BuildReleaseSymbols% NEQ 0 (
	FOR %%A IN ("%osBinPath%\ReleaseSymbols\x86\OpenSauce_Installer\*.msi") DO copy %%A "%BuildPath%\%%~nA_ReleaseSymbols.msi"
)
ECHO.

REM Count the number of files in the build path, if less than 3 something didn't build correctly
SET /A "MsiCount=0"
FOR %%A IN ("%BuildPath%\*.msi") DO SET /A "MsiCount+=1"
IF "%MsiCount%" LSS "%ExpectedMsiCount%" GOTO BUILD_FAILED

:: Archive the installers

SET "ArchiveExe=%ProgFilesDir%\7-Zip\7z.exe"

REM If the user does not have 7-Zip installed skip archive creation
IF NOT EXIST "%ArchiveExe%" GOTO ZIP_MISSING

ECHO Creating build archive using 7-Zip
ECHO Output: "%BuildPath%\..\%FolderName%.7z"

FOR /f %%A IN ("%BuildPath%\*.msi") DO "%ArchiveExe%" a -t7z "%BuildPath%\..\%FolderName%.7z" "%%A"
"%ArchiveExe%" a -t7z "%BuildPath%\..\%FolderName%.7z" "%BuildPath%\Debug\"
"%ArchiveExe%" a -t7z "%BuildPath%\..\%FolderName%.7z" "%BuildPath%\Release\"
GOTO END

:INVALID_VS
ECHO ERROR: VisualStudio is either not installed or an unsupported version is used
GOTO END

:ZIP_MISSING
ECHO WARNING: 7-Zip is not installed, build archive creation has been skipped
GOTO END

:BUILD_FAILED
ECHO ERROR: An installer is missing (expected %ExpectedMsiCount%, got %MsiCount%), one or more of the project configurations may have failed to build
GOTO END

:END
PAUSE
