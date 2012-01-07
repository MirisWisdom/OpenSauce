:: Checks this machine for the prerequisites needed to build the various parts of the OpenSauce codebase
:: Obviously, this is for developers only...
REM There are some things we just can't (reliably) check for:
REM 	GameSpyOpen
REM 	SlimDX
REM See http://code.google.com/p/open-sauce/wiki/UsingTheCode
@ECHO OFF

REM Setup the paths we need to execute external programs
IF "%PROCESSOR_ARCHITECTURE%" == "x86" GOTO PROCESSOR_IS_32BIT
	SET "ProgFilesDir=%PROGRAMFILES%"
	SET "ProgFilesDir86=%PROGRAMFILES(x86)%"
GOTO SET_PROGFILES_END
:PROCESSOR_IS_32BIT
	SET "ProgFilesDir=%PROGRAMFILES%"
	SET "ProgFilesDir86=%PROGRAMFILES%"
:SET_PROGFILES_END

SET MissingReqs=0
SET MissingOpts=0

REM Required: We're currently using VS2008
SET VsVersionStr=2008
REM Easiest way to check if 2008 is installed
IF "%VS90COMNTOOLS%"=="" (
	ECHO ERROR: VisualStudio [%VsVersionStr%] is either not installed or an unsupported version is used
	SET MissingReqs=1
)
:FINISHED_CHECK_VS


REM Required: Check for the C++ boost libraries
IF NOT EXIST "%ProgFilesDir86%\boost\" (
	ECHO ERROR: Couldn't find the C++ boost framework. Did you install it to a non-default location?
	SET MissingReqs=1
)
:FINISHED_CHECK_BOOST


REM Required: Check for the DirectX SDK
IF "%DXSDK_DIR%"=="" (
	ECHO ERROR: DirectX SDK not installed
	SET MissingReqs=1
)
:FINISHED_CHECK_DXSDK


REM Optional: We're currently using XNA 4.0
SET XnaVersionStr=v4.0
IF "%XNAGSShared%"=="" (
	ECHO WARNING: XNA Framework [%XnaVersionStr%] is either not installed or an unsupported version is used
	SET MissingOpts=1
)
:FINISHED_CHECK_XNA_FRAMEWORK
IF NOT EXIST "%XNAGSShared%Framework\%XnaVersionStr%\XnaNative.dll" (
	ECHO WARNING: Couldn't find the XNA Framework's [%XnaVersionStr%] XnaNative.dll
	SET MissingOpts=1
)
:FINISHED_CHECK_XNA


REM Optional: Check for the Xbox 360 SDK
IF "%XEDK%"=="" (
	ECHO WARNING: Xbox 360 SDK not installed
	SET MissingOpts=1
)
:FINISHED_CHECK_XEDK


:FINISHED_CHECKS
ECHO Finished pre-req checks!
IF NOT %MissingReqs% == 0 (
	ECHO 	Install the missing requirements listed
)
IF NOT %MissingOpts% == 0 (
	ECHO 	Install the missing optional reqs listed for full feature support
)
GOTO END

:END
PAUSE
