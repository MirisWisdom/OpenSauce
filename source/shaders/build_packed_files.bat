cd "D:\Program Files (x86)\Microsoft Games\Halo Custom Edition"
os_tool build-packed-file "D:\Gamedev\opensauce\OpenSauce\Halo1\Halo1_CE_Shaders" "D:\Gamedev\opensauce\OpenSauce\Halo1\Halo1_CE_Shaders\bin" Halo1_CE_GBufferPackage.xml
os_tool build-packed-file "D:\Gamedev\opensauce\OpenSauce\Halo1\Halo1_CE_Shaders" "D:\Gamedev\opensauce\OpenSauce\Halo1\Halo1_CE_Shaders\bin" Halo1_CE_PostProcessingPackage.xml
copy "D:\Gamedev\opensauce\OpenSauce\Halo1\Halo1_CE_Shaders\bin\gbuffer_shaders.shd" "C:\ProgramData\Kornner Studios\Halo CE\shaders\gbuffer_shaders.shd"
copy "D:\Gamedev\opensauce\OpenSauce\Halo1\Halo1_CE_Shaders\bin\pp_shaders.shd" "C:\ProgramData\Kornner Studios\Halo CE\shaders\pp_shaders.shd"
pause