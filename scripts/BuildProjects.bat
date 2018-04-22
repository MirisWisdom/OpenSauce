cd /d %~dp0
set ProjectRoot=%~dp0..
powershell -ExecutionPolicy Bypass -File cake.ps1 -Script CakeScripts/BuildProjects.cake