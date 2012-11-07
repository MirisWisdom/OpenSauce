/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/

//////////////////////////////////////////////////////////////////////////
// Rasterizer.cpp
#if __EL_INCLUDE_FILE_ID == __EL_RASTERIZER_RASTERIZER
	namespace Rasterizer
	{
	#if PLATFORM_ID == PLATFORM_SAPIEN
		static real_matrix3x4** K_VSH_CONSTANTS__NODEMATRICES_REFERENCES[] = {
			CAST_PTR(real_matrix3x4**, 0x5E8B02), CAST_PTR(real_matrix3x4**, 0x5F47DC), CAST_PTR(real_matrix3x4**, 0x5F4833), 
			CAST_PTR(real_matrix3x4**, 0x5F488B), CAST_PTR(real_matrix3x4**, 0x5F48E9), CAST_PTR(real_matrix3x4**, 0x5F4981)
		};
		static void** K_VSH_CONSTANTS__NODEMATRICES_REFERENCES_PLUS_8[] = {
			CAST_PTR(void**, 0x5E8A79), 
		};
		static uint16* K_RASTERIZER_GLOBALS_MAXIMUM_NODES_PER_MODEL_WRITE_VALUES[] = {
			CAST_PTR(uint16*, 0x5EA43E), 
		};

		ENGINE_PTR(s_rasterizer_config, rasterizer_config,				PTR_NULL, PTR_NULL, 0xDF6600);
		ENGINE_PTR(s_rasterizer_globals, rasterizer_globals,			PTR_NULL, PTR_NULL, 0xA2A6F8);

		ENGINE_PTR(s_rasterizer_debug_options, rasterizer_debug_data,	PTR_NULL, PTR_NULL, 0xA2A758);

		static void** K_DYNAMIC_TRIANGLE_ARRAY_UPGRADE_ADDRESS_LIST[] = {
			CAST_PTR(void**, 0x6B9CB4), CAST_PTR(void**, 0x6B9E23), CAST_PTR(void**, 0x6B9EA8), 
			CAST_PTR(void**, 0x6B9F34), CAST_PTR(void**, 0x6B9F7F), CAST_PTR(void**, 0x6B9FC9), 
			CAST_PTR(void**, 0x6BA028), CAST_PTR(void**, 0x6BA0A8), CAST_PTR(void**, 0x6BA128), 
			CAST_PTR(void**, 0x6BA18B), CAST_PTR(void**, 0x6BA1BD), CAST_PTR(void**, 0x6BA25E), 
			CAST_PTR(void**, 0x6BA39E),
		};
		static uint16* K_MAXIMUM_DYNAMIC_TRIANGLES_UPGRADE_ADDRESS_LIST[] = {
			CAST_PTR(uint16*, 0x6BE4C7), CAST_PTR(uint16*, 0x6BE537),
			CAST_PTR(uint16*, 0x6BE5A5), CAST_PTR(uint16*, 0x6BE717),
		};
	#endif
	};
	namespace Render
	{
	#if PLATFORM_ID == PLATFORM_SAPIEN
		ENGINE_PTR(s_render_globals, render_globals,					PTR_NULL, PTR_NULL, 0xF9A4C0);
	#endif
	};

#else
	#error Undefined engine layout include for: __EL_INCLUDE_RASTERIZER
#endif