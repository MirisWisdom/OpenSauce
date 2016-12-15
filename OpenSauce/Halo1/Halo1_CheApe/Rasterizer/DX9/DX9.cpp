/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#include "Common/Precompile.hpp"
#include "Rasterizer/DX9/DX9.hpp"

#ifndef PLATFORM_NO_DX9
#include "Engine/EngineFunctions.hpp"

namespace Yelo
{
	namespace Rasterizer { namespace DX9 {
		typedef IDirect3D9* (WINAPI *D3DC9)(UINT);

#define __EL_INCLUDE_ID			__EL_INCLUDE_RASTERIZER_DX9
#define __EL_INCLUDE_FILE_ID	__EL_RASTERIZER_DX9_DX9
#include "Memory/_EngineLayout.inl"

		static IDirect3D9* global_d3d;
		static IDirect3DDevice9* global_d3d_device;

#if PLATFORM_TYPE == PLATFORM_TOOL
		#include "Rasterizer/DX9/DX9.Tool.inl"
#else
		void Initialize()
		{
			global_d3d = GET_DPTR(D3D);
			global_d3d_device = GET_DPTR(Device);
		}

		void Dispose()
		{
		}

		D3DPRESENT_PARAMETERS*	D3DPresetParams()		PTR_IMP_GET2(Params);
#endif
		IDirect3D9*				Direct3D9()				{ return global_d3d; }
		IDirect3DDevice9*		Direct3DDevice()		{ return global_d3d_device; }

	}; };
};
#endif