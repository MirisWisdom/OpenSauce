/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/

//////////////////////////////////////////////////////////////////////////
// DX9.cpp
#if __EL_INCLUDE_FILE_ID == __EL_RASTERIZER_DX9_DX9
	ENGINE_PTR(D3DC9, Direct3DCreate9,						DATA_PTR_NULL, 0x1116734, 0xE38454);

	ENGINE_PTR(D3DPRESENT_PARAMETERS, Params,				DATA_PTR_NULL, DATA_PTR_NULL, 0xF7D6E0);
	ENGINE_DPTR(IDirect3D9, D3D,							DATA_PTR_NULL, DATA_PTR_NULL, 0xDCC270);
	ENGINE_DPTR(IDirect3DDevice9, Device,					DATA_PTR_NULL, DATA_PTR_NULL, 0xDCC26C);


//////////////////////////////////////////////////////////////////////////
// GBuffer.cpp
#elif __EL_INCLUDE_FILE_ID == __EL_RASTERIZER_DX9_GBUFFER

#else
	#error Undefined engine layout include for: __EL_INCLUDE_RASTERIZER_DX9
#endif