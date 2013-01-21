/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI
namespace Yelo
{
	namespace Rasterizer { namespace DeviceHooks
	{
		void Initialize();
		void Dispose();

		void CreateD3DHook(void* call_address, void* hook_address);

		HRESULT CreateDevice(IDirect3D9* d3d, UINT Adapter, D3DDEVTYPE DeviceType, HWND hFocusWindow, DWORD BehaviorFlags, D3DPRESENT_PARAMETERS *pPresentationParameters, IDirect3DDevice9 **ppReturnedDeviceInterface);

		HRESULT Reset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS *pPresentationParameters);

		HRESULT BeginScene(IDirect3DDevice9* device);
		HRESULT EndScene(IDirect3DDevice9* device);

		HRESULT SetVertexShaderConstantF_ViewProj(IDirect3DDevice9* device, UINT StartRegister, CONST float* pConstantData, UINT Vector4fCount);
		HRESULT SetVertexShaderConstantF_ModelSpecColor(IDirect3DDevice9* device, UINT StartRegister, CONST float* pConstantData, UINT Vector4fCount);
		HRESULT SetVertexShaderConstantF_ModelVertexLight(IDirect3DDevice9* device, UINT StartRegister, CONST float* pConstantData, UINT Vector4fCount);
	};};
};
#endif