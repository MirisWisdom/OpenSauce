/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Rasterizer/DX9/DX9.hpp"

#if !PLATFORM_IS_DEDI
#include "Memory/MemoryInterface.hpp"

namespace Yelo
{
	namespace DX9
	{
#define __EL_INCLUDE_ID			__EL_INCLUDE_RASTERIZER_DX9
#define __EL_INCLUDE_FILE_ID	__EL_RASTERIZER_DX9_DX9
#include "Memory/_EngineLayout.inl"

		static IDirect3D9* d3d;
		static IDirect3DDevice9* global_d3d_device;

		void Initialize()
		{
#if !defined(DX_WRAPPER)
			while(!GET_DPTR(D3D) && !GET_DPTR(Device)) Sleep(100);

			d3d = GET_DPTR(D3D);
			global_d3d_device = GET_DPTR(Device);

			//	if(!d3d)
			//		MessageBox(NULL, "Failure: d3d", "debug", MB_OK);
			//	if(!global_d3d_device)
			//		MessageBox(NULL, "Failure: d3d device", "debug", MB_OK);
#endif
		}

		void Dispose() {}

#if defined(DX_WRAPPER)
		void Initialize3D(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* params)
		{
			d3d = GET_DPTR(D3D);
			global_d3d_device = GET_DPTR(Device);
		}

		void OnLostDevice()									{}
		void OnResetDevice(D3DPRESENT_PARAMETERS* params)	{}
		void Render()										{}
		void Release()										{}
#endif

		D3DPRESENT_PARAMETERS*	D3DPresetParams()		PTR_IMP_GET2(Params);
		IDirect3D9*				Direct3D9()				{ return d3d; }
		IDirect3DDevice9*		Direct3DDevice()		{ return global_d3d_device; }
		IDirectInput8*			DirectInput8()			DPTR_IMP_GET(DInput8);
		IDirectInputDevice8*	InputDeviceKeyboard()	DPTR_IMP_GET(DInput8DeviceKeyboard);
		IDirectInputDevice8*	InputDeviceMouse()		DPTR_IMP_GET(DInput8DeviceMouse);
		IDirectInputDevice8*	InputDeviceJoystick(short index)
		{
			if(index >= 8) return NULL;
			return GET_DPTR2(DInput8DeviceJoysticks)[index];
		}
	};
};
#endif