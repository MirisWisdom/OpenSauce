/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Rasterizer/DX9/DeviceHooks.hpp"

#if !PLATFORM_IS_DEDI
#include "Memory/MemoryInterface.hpp"

#include "Common/GameSystems.hpp"
#include "Rasterizer/DX9/DX9.hpp"
#include "Interface/GameUI.hpp"
#include "Rasterizer/GBuffer.hpp"
#include "Rasterizer/ShaderExtension/ShaderExtension.hpp"

namespace Yelo
{
	namespace Rasterizer { namespace DeviceHooks
	{
#define __EL_INCLUDE_ID			__EL_INCLUDE_RASTERIZER_DX9
#define __EL_INCLUDE_FILE_ID	__EL_RASTERIZER_DX9_DEVICEHOOKS
#include "Memory/_EngineLayout.inl"

		API_FUNC_NAKED static void Hook_D3DCreateDeviceCall()
		{
			static uint32 RETN_ADDRESS = GET_FUNC_PTR(RASTERIZER_D3D_CREATE_DEVICE_RETN);
			static void* CALL_ADDRESS = CreateDevice;

			_asm {
				push	ebp
				push	eax

				call	CALL_ADDRESS
				jmp		RETN_ADDRESS
			}
		}

		API_FUNC_NAKED static void Hook_D3DResetCall()
		{
			static uint32 RETN_ADDRESS = GET_FUNC_PTR(RASTERIZER_D3D_RESET_DEVICE_RETN);
			static void* CALL_ADDRESS = Reset;

			_asm {
				push	esi
				push	eax
				call	CALL_ADDRESS
				jmp		RETN_ADDRESS
			}
		}

		void Initialize()
		{
			Memory::WriteRelativeJmp(Hook_D3DCreateDeviceCall, GET_FUNC_VPTR(RASTERIZER_D3D_CREATE_DEVICE_HOOK), true);
			Memory::WriteRelativeJmp(Hook_D3DResetCall, GET_FUNC_VPTR(RASTERIZER_D3D_RESET_DEVICE_HOOK), true);

			CreateD3DHook(GET_FUNC_VPTR(RASTERIZER_D3D_BEGIN_SCENE_CALL), BeginScene);
			CreateD3DHook(GET_FUNC_VPTR(RASTERIZER_D3D_END_SCENE_CALL), EndScene);

			CreateD3DHook(GET_FUNC_VPTR(RASTERIZER_SET_WORLD_VIEW_PROJECTION_MATRIX_VERTEX_CONSTANT_CALL), SetVertexShaderConstantF_ViewProj);
			CreateD3DHook(GET_FUNC_VPTR(RASTERIZER_SET_MODEL_TEX_SCALE_VERTEX_CONSTANT_CALL), SetVertexShaderConstantF_ModelTexScale);
			
			for(int i = 0; i < NUMBEROF(K_RASTERIZER_SET_MODEL_SPEC_COLOR_VERTEX_CONSTANT_CALLS); i++)
				CreateD3DHook(K_RASTERIZER_SET_MODEL_SPEC_COLOR_VERTEX_CONSTANT_CALLS[i], SetVertexShaderConstantF_ModelSpecColor);
			
			CreateD3DHook(GET_FUNC_VPTR(RASTERIZER_SET_MODEL_VERTEX_LIGHT_VERTEX_CONSTANT_CALL), SetVertexShaderConstantF_ModelVertexLight);
		}

		void Dispose()
		{

		}

		static void CreateD3DHook(void* call_address, void* hook_address)
		{
			Memory::WriteRelativeCall(hook_address, call_address, true);
			byte* nop_address = CAST_PTR(byte*, call_address) + 5;
			*nop_address = Enums::_x86_opcode_nop;
		}

		static HRESULT CreateDevice(IDirect3D9* d3d, UINT Adapter, D3DDEVTYPE DeviceType, HWND hFocusWindow, DWORD BehaviorFlags, D3DPRESENT_PARAMETERS *pPresentationParameters, IDirect3DDevice9 **ppReturnedDeviceInterface)
		{
			HRESULT hr = d3d->CreateDevice(Adapter, DeviceType, hFocusWindow, BehaviorFlags, pPresentationParameters, ppReturnedDeviceInterface);

			if(SUCCEEDED(hr) && Yelo::Memory::IsYeloEnabled())
			{
				Yelo::Main::s_dx_component* components;
				const Yelo::int32 component_count = Yelo::Main::GetDXComponents(components);

				for(Yelo::int32 x = 0; x <= component_count; x++)
					components[x].Initialize(*ppReturnedDeviceInterface, pPresentationParameters);
			}

			return hr;
		}

		static HRESULT Reset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS *pPresentationParameters)
		{
			if(Yelo::Memory::IsYeloEnabled())
			{
				Yelo::Main::s_dx_component* components;
				const Yelo::int32 component_count = Yelo::Main::GetDXComponents(components);

				for(Yelo::int32 x = 0; x <= component_count; x++)
					components[x].OnLostDevice();
			}

			HRESULT hr = device->Reset(pPresentationParameters);

			if(Yelo::Memory::IsYeloEnabled())
			{
				Yelo::Main::s_dx_component* components;
				const Yelo::int32 component_count = Yelo::Main::GetDXComponents(components);

				for(Yelo::int32 x = 0; x <= component_count; x++)
					components[x].OnResetDevice(pPresentationParameters);
			}

			return hr;
		}

		static HRESULT BeginScene(IDirect3DDevice9* device)
		{
			HRESULT hr = device->BeginScene();

			if(Yelo::Memory::IsYeloEnabled())
				Yelo::DX9::c_gbuffer_system::ClearGBuffer(device);

			return hr;
		}

		static HRESULT EndScene(IDirect3DDevice9* device)
		{
			if(Yelo::Memory::IsYeloEnabled())
			{
				Yelo::Main::s_dx_component* components;
				const Yelo::int32 component_count = Yelo::Main::GetDXComponents(components);

				for(Yelo::int32 x = 0; x <= component_count; x++)
					components[x].Render();
			}

			return device->EndScene();
		}

		static HRESULT SetVertexShaderConstantF_ViewProj(IDirect3DDevice9* device, UINT StartRegister, CONST float* pConstantData, UINT Vector4fCount)
		{
			DX9::c_gbuffer_system::SetViewProj(device, pConstantData, Vector4fCount);
			Rasterizer::ShaderExtension::Model::SetViewProj(device, pConstantData, Vector4fCount);

			return device->SetVertexShaderConstantF(StartRegister, pConstantData, Vector4fCount);
		}

		static HRESULT SetVertexShaderConstantF_ModelTexScale(IDirect3DDevice9* device, UINT StartRegister, CONST float* pConstantData, UINT Vector4fCount)
		{
			bool set_original = false;

			set_original |= Rasterizer::ShaderExtension::Model::SetTexScale(device, pConstantData, Vector4fCount);

			if(set_original)
				return device->SetVertexShaderConstantF(StartRegister, pConstantData, Vector4fCount);
			return S_OK;
		}

		static HRESULT SetVertexShaderConstantF_ModelSpecColor(IDirect3DDevice9* device, UINT StartRegister, CONST float* pConstantData, UINT Vector4fCount)
		{
			bool set_original = false;

			set_original |= Rasterizer::ShaderExtension::Model::SetSpecColor(device, pConstantData, Vector4fCount);

			if(set_original)
				return device->SetVertexShaderConstantF(StartRegister, pConstantData, Vector4fCount);
			return S_OK;
		}

		static HRESULT SetVertexShaderConstantF_ModelVertexLight(IDirect3DDevice9* device, UINT StartRegister, CONST float* pConstantData, UINT Vector4fCount)
		{
			bool set_original = false;

			set_original |= Rasterizer::ShaderExtension::Model::SetVertexLight(device, pConstantData, Vector4fCount);

			if(set_original)
				return device->SetVertexShaderConstantF(StartRegister, pConstantData, Vector4fCount);
			return S_OK;
		}
	};};
};
#endif