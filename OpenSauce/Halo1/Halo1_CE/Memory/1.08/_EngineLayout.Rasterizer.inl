/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/


//////////////////////////////////////////////////////////////////////////
// Rasterizer.cpp
#if __EL_INCLUDE_FILE_ID == __EL_RASTERIZER_RASTERIZER
	namespace Rasterizer
	{
		static real_matrix3x4** K_VSH_CONSTANTS__NODEMATRICES_REFERENCES[] = {
			CAST_PTR(real_matrix3x4**, 0x51C703), CAST_PTR(real_matrix3x4**, 0x52A48F), CAST_PTR(real_matrix3x4**, 0x52A4E6), 
			CAST_PTR(real_matrix3x4**, 0x52A53E), CAST_PTR(real_matrix3x4**, 0x52A59C), CAST_PTR(real_matrix3x4**, 0x52A63B)
		};
		static real_matrix3x3** K_VSH_CONSTANTS__NODEMATRICES_REFERENCES_PLUS_8[] = {
			CAST_PTR(void**, 0x51C67A), 
		};
		static uint16* K_RASTERIZER_GLOBALS_MAXIMUM_NODES_PER_MODEL_WRITE_VALUES[] = {
			CAST_PTR(uint16*, 0x51AEC1), 
		};

		ENGINE_PTR(s_rasterizer_config, rasterizer_config,					0x6BDD30, PTR_NULL);
		ENGINE_PTR(s_rasterizer_globals, rasterizer_globals,				0x637CF8, PTR_NULL);
		ENGINE_PTR(s_rasterizer_debug_options, rasterizer_debug_data,		0x6249C0, PTR_NULL);
		ENGINE_PTR(s_rasterizer_frame_parameters, rasterizer_frame_params,	0x75C130, PTR_NULL);

		FUNC_PTR(RASTERIZER_DISPOSE,									0x51C110, FUNC_PTR_NULL);
		FUNC_PTR(RASTERIZER_DISPOSE_CALL_FROM_RASTERIZER,				0x51B0E0, FUNC_PTR_NULL);
		FUNC_PTR(RASTERIZER_DISPOSE_CALL_FROM_SHELL,					0x5447D9, FUNC_PTR_NULL);
		

		ENGINE_PTR(DWORD, RASTERIZER_DEVICE_ADAPTER_INDEX				0x6B8350, PTR_NULL);
		ENGINE_PTR(char, RESOLUTION_LIST_COUNT,							0x4BE35C, PTR_NULL);
		ENGINE_PTR(void**, RESOLUTION_LIST_STRING_NULL_REFERENCE,		0x4BE39B, PTR_NULL);

		FUNC_PTR(RESOLUTION_LIST_SETUP_RESOLUTIONS_CALL,				0x4BE2BF, FUNC_PTR_NULL);

		static void** K_RESOLUTION_LIST_X_REFERENCES[] = {
			CAST_PTR(void**, 0x4BE2AA), CAST_PTR(void**, 0x4BE2D2), CAST_PTR(void**, 0x4BE37C),
			CAST_PTR(void**, 0x4BE54C), CAST_PTR(void**, 0x4BE937)
		};
		static void** K_RESOLUTION_LIST_Y_REFERENCES[] = {
			CAST_PTR(void**, 0x4BE33A), CAST_PTR(void**, 0x4BE382), CAST_PTR(void**, 0x4BE40C),
			CAST_PTR(void**, 0x4BE4CE), CAST_PTR(void**, 0x4BE94B)
		};
		static void** K_RESOLUTION_LIST_STRING_REFERENCES[] = {
			CAST_PTR(void**, 0x4BE376), CAST_PTR(void**, 0x4BEBFC)
		};

		static void** K_RESOLUTION_LIST_REFRESH_COUNT_REFERENCES[] = {
			CAST_PTR(void**, 0x4BE38D), CAST_PTR(void**, 0x4BE3B1), CAST_PTR(void**, 0x4BE3D4),
			CAST_PTR(void**, 0x4BE3F5), CAST_PTR(void**, 0x4BE506), CAST_PTR(void**, 0x4BE55E),
			CAST_PTR(void**, 0x4BE97B), CAST_PTR(void**, 0x4BEC28), CAST_PTR(void**, 0x4BEC2E)
		};
		static void** K_RESOLUTION_LIST_REFRESH_RATE_REFERENCES[] = {
			CAST_PTR(void**, 0x4BE2EB), CAST_PTR(void**, 0x4BE3A3), CAST_PTR(void**, 0x4BE3CE),
			CAST_PTR(void**, 0x4BE401), CAST_PTR(void**, 0x4BE516), CAST_PTR(void**, 0x4BE98C),
			CAST_PTR(void**, 0x4BEC6A),
		};

		ENGINE_PTR(byte, NVIDIA_USE_BASIC_CAMO_TOGGLE,					0x51AB88, PTR_NULL);

		static char** K_SCREENSHOT_FOLDER_REFERENCES[] = {
			CAST_PTR(char**, 0x4CD88C), CAST_PTR(char**, 0x4CD8A3)
		};
	};
	namespace Render
	{
		ENGINE_PTR(s_render_globals, render_globals,					0x75E020, PTR_NULL);
		ENGINE_PTR(s_render_target, global_render_targets,				0x638A20, PTR_NULL);
		ENGINE_PTR(s_structure_render_globals, structure_render_globals,0x67E9D0, PTR_NULL);

		FUNC_PTR(RENDER_WINDOW_REFLECTION_CALL,							0x50F9E0, FUNC_PTR_NULL);
		FUNC_PTR(RENDER_WINDOW_REFLECTION_CALL_RETN,					0x50F9E5, FUNC_PTR_NULL);

		FUNC_PTR(RENDER_WINDOW_END_HOOK,								0x51BD68, FUNC_PTR_NULL);
	};

#elif __EL_INCLUDE_FILE_ID == __EL_RASTERIZER_LIGHTMAPS
	namespace Render { namespace Lightmaps
	{
		FUNC_PTR(BSP_LIGHTMAP_INDEX_HOOK,							0x5568DA - 0x30, FUNC_PTR_NULL);
		FUNC_PTR(BSP_LIGHTMAP_INDEX_RETN,							0x5568E0 - 0x30, FUNC_PTR_NULL);
		FUNC_PTR(SET_LIGHTMAP_SAMPLER_CALL,							0x522F6C - 0x30, FUNC_PTR_NULL);
		FUNC_PTR(OBJECT_RENDER_CACHE_LIGHTMAP_TAG_INDEX_HOOK,		0x4F6304 - 0x30, FUNC_PTR_NULL);
		FUNC_PTR(OBJECT_RENDER_CACHE_LIGHTMAP_TAG_INDEX_RETN,		0x4F630A - 0x30, FUNC_PTR_NULL);
	};};

#elif __EL_INCLUDE_FILE_ID == __EL_RASTERIZER_SKY
	namespace Render { namespace Sky
	{
		FUNC_PTR(FOG_ATMOSPHERIC_SKY_TAG_INDEX_HOOK,				0x54205F - 0x30, FUNC_PTR_NULL);
		FUNC_PTR(FOG_ATMOSPHERIC_SKY_TAG_INDEX_RETN,				0x542066 - 0x30, FUNC_PTR_NULL);
		FUNC_PTR(FOG_PLANAR_SKY_TAG_INDEX_HOOK,						0x558DBC - 0x30, FUNC_PTR_NULL);
		FUNC_PTR(FOG_PLANAR_SKY_TAG_INDEX_RETN,						0x558DC2 - 0x30, FUNC_PTR_NULL);
		FUNC_PTR(RENDER_SKY_SKY_TAG_INDEX_HOOK,						0x51492E - 0x30, FUNC_PTR_NULL);
		FUNC_PTR(RENDER_SKY_SKY_TAG_INDEX_RETN,						0x514935 - 0x30, FUNC_PTR_NULL);
	};};

#else
	#error Undefined engine layout include for: __EL_INCLUDE_RASTERIZER
#endif