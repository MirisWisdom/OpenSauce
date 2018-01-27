/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI
#include "Memory/1.10/Pointers/HaloCE_110_Runtime.Rasterizer.DX9.inl"
#include "Memory/1.10/Pointers/HaloCE_110_Runtime_Manual.Rasterizer.DX9.inl"
#endif

#include <dinput.h>
#include <Memory/MemoryInterface.hpp>
#include <yelolib/memory/memory_interface_base.hpp>

namespace Yelo
{
	namespace DX9
	{
		ENGINE_PTR(D3DPRESENT_PARAMETERS, Params, K_PARAMS, PTR_NULL);
		ENGINE_PTR(D3DCAPS9, Caps, K_CAPS, PTR_NULL);
		ENGINE_DPTR(IDirect3D9, D3D, K_D3D, PTR_NULL);
		ENGINE_DPTR(IDirect3DDevice9, Device, K_DEVICE, PTR_NULL);
		ENGINE_DPTR(IDirectInput8, DInput8, K_DINPUT8, PTR_NULL);
		ENGINE_DPTR(IDirectInputDevice8, DInput8DeviceKeyboard, K_DINPUT8DEVICEKEYBOARD, PTR_NULL);
		ENGINE_DPTR(IDirectInputDevice8, DInput8DeviceMouse, K_DINPUT8DEVICEMOUSE, PTR_NULL);
		ENGINE_DPTR(IDirectInputDevice8, DInput8DeviceJoysticks, K_DINPUT8DEVICEJOYSTICKS, PTR_NULL);
	}
}
