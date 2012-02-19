/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/


//////////////////////////////////////////////////////////////////////////
// Rasterizer.cpp
#if __EL_INCLUDE_FILE_ID == __EL_RASTERIZER_RASTERIZER
	ENGINE_PTR(s_rasterizer_config, rasterizer_config,				0x6BDE18, PTR_NULL);
	ENGINE_PTR(s_render_globals, render_globals,					0x75E100, PTR_NULL);
	ENGINE_PTR(s_render_target, global_render_targets,				0x638A20, PTR_NULL);
	ENGINE_PTR(s_rasterizer_debug_options, rasterizer_debug_data,	0x6249C0, PTR_NULL);
	ENGINE_PTR(s_rasterizer_frame_inputs, rasterizer_frame_inputs,	0x75C210, PTR_NULL);

	FUNC_PTR(RENDER_WINDOW_END_HOOK,								0x51BD98, FUNC_PTR_NULL);

	FUNC_PTR(RASTERIZER_DISPOSE,									0x51C140, FUNC_PTR_NULL);
	FUNC_PTR(RASTERIZER_DISPOSE_CALL_FROM_RASTERIZER,				0x51B110, FUNC_PTR_NULL);
	FUNC_PTR(RASTERIZER_DISPOSE_CALL_FROM_SHELL,					0x544809, FUNC_PTR_NULL);
	

	static void** K_DYNAMIC_TRIANGLE_ARRAY_UPGRADE_ADDRESS_LIST[] = {
		CAST_PTR(void**, 0x50FE3E), CAST_PTR(void**, 0x50FF9C), CAST_PTR(void**, 0x50FFDE), 
		CAST_PTR(void**, 0x510017), CAST_PTR(void**, 0x510049), CAST_PTR(void**, 0x510073), 
		CAST_PTR(void**, 0x5100AE), CAST_PTR(void**, 0x5101BF), CAST_PTR(void**, 0x5101E0), 
		CAST_PTR(void**, 0x556240), CAST_PTR(void**, 0x55637C), CAST_PTR(void**, 0x556466), 
		CAST_PTR(void**, 0x556546),
	};
	static uint16* K_MAXIMUM_DYNAMIC_TRIANGLES_UPGRADE_ADDRESS_LIST[] = {
		CAST_PTR(uint16*, 0x557387), CAST_PTR(uint16*, 0x5573F7),
		CAST_PTR(uint16*, 0x55745A), CAST_PTR(uint16*, 0x557577),
	};

	ENGINE_PTR(DWORD, RASTERIZER_DEVICE_ADAPTER_INDEX,				0x6B8438, PTR_NULL);
	ENGINE_PTR(char, RESOLUTION_LIST_COUNT,							0x4BE38C, PTR_NULL);
	ENGINE_PTR(void**, RESOLUTION_LIST_STRING_NULL_REFERENCE,		0x4BE3CB, PTR_NULL);

	FUNC_PTR(RESOLUTION_LIST_SETUP_RESOLUTIONS_CALL,				0x4BE2EF, FUNC_PTR_NULL);

	static void** K_RESOLUTION_LIST_X_REFERENCES[] = {
		CAST_PTR(void**, 0x4BE2DA), CAST_PTR(void**, 0x4BE302), CAST_PTR(void**, 0x4BE3AC),
		CAST_PTR(void**, 0x4BE57C), CAST_PTR(void**, 0x4BE967)
	};
	static void** K_RESOLUTION_LIST_Y_REFERENCES[] = {
		CAST_PTR(void**, 0x4BE36A), CAST_PTR(void**, 0x4BE3B2), CAST_PTR(void**, 0x4BE4CC),
		CAST_PTR(void**, 0x4BE4FE), CAST_PTR(void**, 0x4BE97B)
	};
	static void** K_RESOLUTION_LIST_STRING_REFERENCES[] = {
		CAST_PTR(void**, 0x4BE3A6), CAST_PTR(void**, 0x4BEC2C)
	};

	static void** K_RESOLUTION_LIST_REFRESH_COUNT_REFERENCES[] = {
		CAST_PTR(void**, 0x4BE3BD), CAST_PTR(void**, 0x4BE3E1), CAST_PTR(void**, 0x4BE404),
		CAST_PTR(void**, 0x4BE425), CAST_PTR(void**, 0x4BE536), CAST_PTR(void**, 0x4BE58E),
		CAST_PTR(void**, 0x4BE9AB), CAST_PTR(void**, 0x4BEC58), CAST_PTR(void**, 0x4BEC5E)
	};
	static void** K_RESOLUTION_LIST_REFRESH_RATE_REFERENCES[] = {
		CAST_PTR(void**, 0x4BE31B), CAST_PTR(void**, 0x4BE3D3), CAST_PTR(void**, 0x4BE3FE),
		CAST_PTR(void**, 0x4BE431), CAST_PTR(void**, 0x4BE546), CAST_PTR(void**, 0x4BE9BC),
		CAST_PTR(void**, 0x4BEC9A),
	};

	ENGINE_PTR(byte, NVIDIA_USE_BASIC_CAMO_TOGGLE,					0x51ABB8, PTR_NULL);

	FUNC_PTR(RENDER_WINDOW_REFLECTION_CALL,							0x50FA10, FUNC_PTR_NULL);
	FUNC_PTR(RENDER_WINDOW,											0x50FC50, FUNC_PTR_NULL);
	FUNC_PTR(RENDER_WINDOW_REFLECTION_CALL_RETN,					0x50FA15, FUNC_PTR_NULL);
#else
	#error Undefined engine layout include for: __EL_INCLUDE_RASTERIZER
#endif