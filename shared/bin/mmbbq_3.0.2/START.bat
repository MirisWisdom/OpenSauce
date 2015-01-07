@echo off
set PWD=%~dp0

echo injecting mmBBQ...

cd "%systemroot%\system32"
rundll32 "%PWD%mmbbq.dll",rundll_inject 0,0,0,0
