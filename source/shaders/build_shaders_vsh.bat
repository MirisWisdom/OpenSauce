echo off
cd "D:\Program Files (x86)\Microsoft Games\Halo Custom Edition\"
os_tool compile-shaders "D:\Gamedev\opensauce\OpenSauce\Halo1\Halo1_CE_Shaders" "D:\Gamedev\opensauce\OpenSauce\Halo1\Halo1_CE_Shaders\obj" "D:\Gamedev\opensauce\OpenSauce\Halo1\Halo1_CE_Shaders\bin" VS20 ""
copy "D:\Gamedev\opensauce\OpenSauce\Halo1\Halo1_CE_Shaders\bin\vsh_OS.enc" "shaders\vsh_OS.enc"
pause