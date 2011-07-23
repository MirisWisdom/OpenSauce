/*
    Yelo: Open Sauce SDK
		Halo 1 (CE) Edition
    Copyright (C) 2005-2010  Kornner Studios (http://kornner.com)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "Common/Precompile.hpp"
#include "Rasterizer/DX9/DxWrapper.hpp"

#if PLATFORM_IS_USER && defined(DX_WRAPPER)
#include <intrin.h>
#include "Memory/MemoryInterface.hpp"

#include "Common/GameSystems.hpp"
#include "Rasterizer/DX9/DX9.hpp"
#include "Rasterizer/GBuffer.hpp"
#include "Interface/GameUI.hpp"


#pragma region IDirect3DDevice9
void Yelo_IDirect3DDevice9::Initialize(LPDIRECT3DDEVICE9 pDevice, LPDIRECT3DDEVICE9 **ppDevice) 
{
	Yelo_pD3DDevice = pDevice;
	*ppDevice = &Yelo_pD3DDevice;
}

ULONG Yelo_IDirect3DDevice9::AddRef() 
{ return Yelo_pD3DDevice->AddRef(); }

HRESULT Yelo_IDirect3DDevice9::QueryInterface(REFIID riid, LPVOID *ppvObj) 
{ return Yelo_pD3DDevice->QueryInterface(riid, ppvObj); }

ULONG Yelo_IDirect3DDevice9::Release() 
{ return Yelo_pD3DDevice->Release(); }

void Yelo_IDirect3DDevice9::SetCursorPosition(int X, int Y, DWORD Flags) 
{ Yelo_pD3DDevice->SetCursorPosition(X, Y, Flags); }

HRESULT Yelo_IDirect3DDevice9::SetCursorProperties(UINT XHotSpot, UINT YHotSpot, IDirect3DSurface9 *pCursorBitmap) 
{ return Yelo_pD3DDevice->SetCursorProperties(XHotSpot, YHotSpot, pCursorBitmap); }

BOOL Yelo_IDirect3DDevice9::ShowCursor(BOOL bShow) 
{ return Yelo_pD3DDevice->ShowCursor(bShow); }

HRESULT Yelo_IDirect3DDevice9::CreateAdditionalSwapChain(D3DPRESENT_PARAMETERS *pPresentationParameters, IDirect3DSwapChain9 **ppSwapChain) 
{ return Yelo_pD3DDevice->CreateAdditionalSwapChain(pPresentationParameters, ppSwapChain); }

HRESULT Yelo_IDirect3DDevice9::CreateCubeTexture(THIS_ UINT EdgeLength,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DCubeTexture9** ppCubeTexture,HANDLE* pSharedHandle) 
{ return Yelo_pD3DDevice->CreateCubeTexture(EdgeLength, Levels, Usage, Format, Pool, ppCubeTexture,pSharedHandle); }

HRESULT Yelo_IDirect3DDevice9::CreateDepthStencilSurface(THIS_ UINT Width,UINT Height,D3DFORMAT Format,D3DMULTISAMPLE_TYPE MultiSample,DWORD MultisampleQuality,BOOL Discard,IDirect3DSurface9** ppSurface,HANDLE* pSharedHandle) 
{ return Yelo_pD3DDevice->CreateDepthStencilSurface(Width, Height, Format, MultiSample, MultisampleQuality,Discard,ppSurface, pSharedHandle); }

HRESULT Yelo_IDirect3DDevice9::CreateIndexBuffer(THIS_ UINT Length,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DIndexBuffer9** ppIndexBuffer,HANDLE* pSharedHandle) 
{ return Yelo_pD3DDevice->CreateIndexBuffer(Length, Usage, Format, Pool, ppIndexBuffer,pSharedHandle); }

HRESULT Yelo_IDirect3DDevice9::CreateRenderTarget(THIS_ UINT Width,UINT Height,D3DFORMAT Format,D3DMULTISAMPLE_TYPE MultiSample,DWORD MultisampleQuality,BOOL Lockable,IDirect3DSurface9** ppSurface,HANDLE* pSharedHandle) 
{ return Yelo_pD3DDevice->CreateRenderTarget(Width, Height, Format, MultiSample,MultisampleQuality, Lockable, ppSurface,pSharedHandle); }

HRESULT Yelo_IDirect3DDevice9::CreateTexture(THIS_ UINT Width,UINT Height,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DTexture9** ppTexture,HANDLE* pSharedHandle) 
{ return Yelo_pD3DDevice->CreateTexture(Width, Height, Levels, Usage, Format, Pool, ppTexture,pSharedHandle); }

HRESULT Yelo_IDirect3DDevice9::CreateVertexBuffer(THIS_ UINT Length,DWORD Usage,DWORD FVF,D3DPOOL Pool,IDirect3DVertexBuffer9** ppVertexBuffer,HANDLE* pSharedHandle) 
{ return Yelo_pD3DDevice->CreateVertexBuffer(Length, Usage, FVF, Pool, ppVertexBuffer,pSharedHandle); }

HRESULT Yelo_IDirect3DDevice9::CreateVolumeTexture(THIS_ UINT Width,UINT Height,UINT Depth,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DVolumeTexture9** ppVolumeTexture,HANDLE* pSharedHandle) 
{ return Yelo_pD3DDevice->CreateVolumeTexture(Width, Height, Depth, Levels, Usage, Format, Pool, ppVolumeTexture,pSharedHandle); }

HRESULT Yelo_IDirect3DDevice9::BeginStateBlock() 
{ return Yelo_pD3DDevice->BeginStateBlock(); }

HRESULT Yelo_IDirect3DDevice9::CreateStateBlock(THIS_ D3DSTATEBLOCKTYPE Type,IDirect3DStateBlock9** ppSB) 
{ return Yelo_pD3DDevice->CreateStateBlock(Type, ppSB); }

HRESULT Yelo_IDirect3DDevice9::EndStateBlock(THIS_ IDirect3DStateBlock9** ppSB) 
{ return Yelo_pD3DDevice->EndStateBlock(ppSB); }

HRESULT Yelo_IDirect3DDevice9::GetClipStatus(D3DCLIPSTATUS9 *pClipStatus) 
{ return Yelo_pD3DDevice->GetClipStatus(pClipStatus); }

HRESULT Yelo_IDirect3DDevice9::GetDisplayMode(THIS_ UINT iSwapChain,D3DDISPLAYMODE* pMode) 
{ return Yelo_pD3DDevice->GetDisplayMode(iSwapChain,pMode); }

HRESULT Yelo_IDirect3DDevice9::GetRenderState(D3DRENDERSTATETYPE State, DWORD *pValue) 
{ return Yelo_pD3DDevice->GetRenderState(State, pValue); }

HRESULT Yelo_IDirect3DDevice9::GetRenderTarget(THIS_ DWORD RenderTargetIndex,IDirect3DSurface9** ppRenderTarget) 
{ return Yelo_pD3DDevice->GetRenderTarget(RenderTargetIndex,ppRenderTarget); }

HRESULT Yelo_IDirect3DDevice9::GetTransform(D3DTRANSFORMSTATETYPE State, D3DMATRIX *pMatrix) 
{ return Yelo_pD3DDevice->GetTransform(State, pMatrix); }

HRESULT Yelo_IDirect3DDevice9::SetClipStatus(CONST D3DCLIPSTATUS9 *pClipStatus) 
{ return Yelo_pD3DDevice->SetClipStatus(pClipStatus); }

HRESULT Yelo_IDirect3DDevice9::SetRenderState(D3DRENDERSTATETYPE State, DWORD Value) 
{ return Yelo_pD3DDevice->SetRenderState(State, Value); }

HRESULT Yelo_IDirect3DDevice9::SetRenderTarget(THIS_ DWORD RenderTargetIndex,IDirect3DSurface9* pRenderTarget) 
{ return Yelo_pD3DDevice->SetRenderTarget(RenderTargetIndex,pRenderTarget); }

HRESULT Yelo_IDirect3DDevice9::SetTransform(D3DTRANSFORMSTATETYPE State, CONST D3DMATRIX *pMatrix) 
{ return Yelo_pD3DDevice->SetTransform(State, pMatrix); }

void Yelo_IDirect3DDevice9::GetGammaRamp(THIS_ UINT iSwapChain,D3DGAMMARAMP* pRamp) 
{ Yelo_pD3DDevice->GetGammaRamp(iSwapChain,pRamp); }

void Yelo_IDirect3DDevice9::SetGammaRamp(THIS_ UINT iSwapChain,DWORD Flags,CONST D3DGAMMARAMP* pRamp)
{ Yelo_pD3DDevice->SetGammaRamp(iSwapChain,Flags, pRamp); }

HRESULT Yelo_IDirect3DDevice9::DeletePatch(UINT Handle) 
{ return Yelo_pD3DDevice->DeletePatch(Handle); }

HRESULT Yelo_IDirect3DDevice9::DrawRectPatch(UINT Handle, CONST float *pNumSegs, CONST D3DRECTPATCH_INFO *pRectPatchInfo) 
{ return Yelo_pD3DDevice->DrawRectPatch(Handle, pNumSegs, pRectPatchInfo); }

HRESULT Yelo_IDirect3DDevice9::DrawTriPatch(UINT Handle, CONST float *pNumSegs, CONST D3DTRIPATCH_INFO *pTriPatchInfo)
{ return Yelo_pD3DDevice->DrawTriPatch(Handle, pNumSegs, pTriPatchInfo); }

HRESULT Yelo_IDirect3DDevice9::GetIndices(THIS_ IDirect3DIndexBuffer9** ppIndexData) 
{ return Yelo_pD3DDevice->GetIndices(ppIndexData); }

HRESULT Yelo_IDirect3DDevice9::SetIndices(THIS_ IDirect3DIndexBuffer9* pIndexData) 
{ return Yelo_pD3DDevice->SetIndices(pIndexData); }

UINT Yelo_IDirect3DDevice9::GetAvailableTextureMem() 
{ return Yelo_pD3DDevice->GetAvailableTextureMem(); }

HRESULT Yelo_IDirect3DDevice9::GetCreationParameters(D3DDEVICE_CREATION_PARAMETERS *pParameters) 
{ return Yelo_pD3DDevice->GetCreationParameters(pParameters); }

HRESULT Yelo_IDirect3DDevice9::GetDeviceCaps(D3DCAPS9 *pCaps) 
{ return Yelo_pD3DDevice->GetDeviceCaps(pCaps); }

HRESULT Yelo_IDirect3DDevice9::GetDirect3D(IDirect3D9 **ppD3D9) 
{ return Yelo_pD3DDevice->GetDirect3D(ppD3D9); }

HRESULT Yelo_IDirect3DDevice9::GetRasterStatus(THIS_ UINT iSwapChain,D3DRASTER_STATUS* pRasterStatus) 
{ return Yelo_pD3DDevice->GetRasterStatus(iSwapChain,pRasterStatus); }

HRESULT Yelo_IDirect3DDevice9::GetLight(DWORD Index, D3DLIGHT9 *pLight) 
{ return Yelo_pD3DDevice->GetLight(Index, pLight); }

HRESULT Yelo_IDirect3DDevice9::GetLightEnable(DWORD Index, BOOL *pEnable) 
{ return Yelo_pD3DDevice->GetLightEnable(Index, pEnable); }

HRESULT Yelo_IDirect3DDevice9::GetMaterial(D3DMATERIAL9 *pMaterial) 
{ return Yelo_pD3DDevice->GetMaterial(pMaterial); }

HRESULT Yelo_IDirect3DDevice9::LightEnable(DWORD LightIndex, BOOL bEnable) 
{ return Yelo_pD3DDevice->LightEnable(LightIndex, bEnable); }

HRESULT Yelo_IDirect3DDevice9::SetLight(DWORD Index, CONST D3DLIGHT9 *pLight) 
{ return Yelo_pD3DDevice->SetLight(Index, pLight); }

HRESULT Yelo_IDirect3DDevice9::SetMaterial(CONST D3DMATERIAL9 *pMaterial) 
{ return Yelo_pD3DDevice->SetMaterial(pMaterial); }

HRESULT Yelo_IDirect3DDevice9::MultiplyTransform(D3DTRANSFORMSTATETYPE State, CONST D3DMATRIX *pMatrix) 
{ return Yelo_pD3DDevice->MultiplyTransform(State, pMatrix); }

HRESULT Yelo_IDirect3DDevice9::ProcessVertices(THIS_ UINT SrcStartIndex,UINT DestIndex,UINT VertexCount,IDirect3DVertexBuffer9* pDestBuffer,IDirect3DVertexDeclaration9* pVertexDecl,DWORD Flags) 
{ return Yelo_pD3DDevice->ProcessVertices(SrcStartIndex, DestIndex, VertexCount, pDestBuffer,pVertexDecl, Flags); }

HRESULT Yelo_IDirect3DDevice9::TestCooperativeLevel() 
{ return Yelo_pD3DDevice->TestCooperativeLevel(); }

HRESULT Yelo_IDirect3DDevice9::GetCurrentTexturePalette(UINT *pPaletteNumber)
{ return Yelo_pD3DDevice->GetCurrentTexturePalette(pPaletteNumber); }

HRESULT Yelo_IDirect3DDevice9::GetPaletteEntries(UINT PaletteNumber, PALETTEENTRY *pEntries)
{ return Yelo_pD3DDevice->GetPaletteEntries(PaletteNumber, pEntries); }

HRESULT Yelo_IDirect3DDevice9::SetCurrentTexturePalette(UINT PaletteNumber) 
{ return Yelo_pD3DDevice->SetCurrentTexturePalette(PaletteNumber); }

HRESULT Yelo_IDirect3DDevice9::SetPaletteEntries(UINT PaletteNumber, CONST PALETTEENTRY *pEntries) 
{ return Yelo_pD3DDevice->SetPaletteEntries(PaletteNumber, pEntries); }

HRESULT Yelo_IDirect3DDevice9::CreatePixelShader(THIS_ CONST DWORD* pFunction,IDirect3DPixelShader9** ppShader) 
{ return Yelo_pD3DDevice->CreatePixelShader(pFunction, ppShader); }

HRESULT Yelo_IDirect3DDevice9::GetPixelShader(THIS_ IDirect3DPixelShader9** ppShader) 
{ return Yelo_pD3DDevice->GetPixelShader(ppShader); }

HRESULT Yelo_IDirect3DDevice9::SetPixelShader(THIS_ IDirect3DPixelShader9* pShader) 
{ return Yelo_pD3DDevice->SetPixelShader(pShader); }

HRESULT Yelo_IDirect3DDevice9::Present(CONST RECT *pSourceRect, CONST RECT *pDestRect, HWND hDestWindowOverride, CONST RGNDATA *pDirtyRegion) 
{ return Yelo_pD3DDevice->Present(pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion); }

HRESULT Yelo_IDirect3DDevice9::Reset(D3DPRESENT_PARAMETERS *pPresentationParameters) 
{
	if(Yelo::Memory::IsYeloEnabled())
	{
		Yelo::Main::s_dx_component* components;
		const Yelo::int32 component_count = Yelo::Main::GetDXComponents(components);

		for(Yelo::int32 x = 0; x <= component_count; x++)
			components[x].OnLostDevice();
	}

	HRESULT hr = Yelo_pD3DDevice->Reset(pPresentationParameters);

	if(Yelo::Memory::IsYeloEnabled())
	{
		Yelo::Main::s_dx_component* components;
		const Yelo::int32 component_count = Yelo::Main::GetDXComponents(components);

		for(Yelo::int32 x = 0; x <= component_count; x++)
			components[x].OnResetDevice(pPresentationParameters);
	}

	return hr;
}

HRESULT Yelo_IDirect3DDevice9::DrawIndexedPrimitive(THIS_ D3DPRIMITIVETYPE Type,INT BaseVertexIndex,UINT MinVertexIndex,UINT NumVertices,UINT startIndex,UINT primCount) 
{ return Yelo_pD3DDevice->DrawIndexedPrimitive(Type,BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount); }

HRESULT Yelo_IDirect3DDevice9::DrawIndexedPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType, UINT MinIndex, UINT NumVertices, UINT PrimitiveCount, CONST void *pIndexData, D3DFORMAT IndexDataFormat, CONST void *pVertexStreamZeroData, UINT VertexStreamZeroStride) 
{ return Yelo_pD3DDevice->DrawIndexedPrimitiveUP(PrimitiveType, MinIndex, NumVertices, PrimitiveCount, pIndexData, IndexDataFormat, pVertexStreamZeroData, VertexStreamZeroStride); }

HRESULT Yelo_IDirect3DDevice9::DrawPrimitive(D3DPRIMITIVETYPE PrimitiveType, UINT StartVertex, UINT PrimitiveCount) 
{ return Yelo_pD3DDevice->DrawPrimitive(PrimitiveType, StartVertex, PrimitiveCount); }

HRESULT Yelo_IDirect3DDevice9::DrawPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType, UINT PrimitiveCount, CONST void *pVertexStreamZeroData, UINT VertexStreamZeroStride) 
{
	if(Yelo::Memory::IsYeloEnabled())
		Yelo::Hud::Scale(_ReturnAddress(), CAST_QUAL(void*, pVertexStreamZeroData));

	return Yelo_pD3DDevice->DrawPrimitiveUP(PrimitiveType, PrimitiveCount, pVertexStreamZeroData, VertexStreamZeroStride);
}

HRESULT Yelo_IDirect3DDevice9::BeginScene() 
{
	HRESULT hr = Yelo_pD3DDevice->BeginScene();

	if(Yelo::Memory::IsYeloEnabled())
		Yelo::DX9::c_gbuffer_system::ClearGBuffer(Yelo_pD3DDevice);

	return hr;
}

HRESULT Yelo_IDirect3DDevice9::EndScene() 
{
	if(Yelo::Memory::IsYeloEnabled())
	{
		Yelo::Main::s_dx_component* components;
		const Yelo::int32 component_count = Yelo::Main::GetDXComponents(components);

		for(Yelo::int32 x = 0; x <= component_count; x++)
			components[x].Render();
	}

	return Yelo_pD3DDevice->EndScene();
}

HRESULT Yelo_IDirect3DDevice9::GetStreamSource(THIS_ UINT StreamNumber,IDirect3DVertexBuffer9** ppStreamData,UINT* OffsetInBytes,UINT* pStride) 
{ return Yelo_pD3DDevice->GetStreamSource(StreamNumber, ppStreamData,OffsetInBytes, pStride); }

HRESULT Yelo_IDirect3DDevice9::SetStreamSource(THIS_ UINT StreamNumber,IDirect3DVertexBuffer9* pStreamData,UINT OffsetInBytes,UINT Stride) 
{ return Yelo_pD3DDevice->SetStreamSource(StreamNumber, pStreamData,OffsetInBytes, Stride); }

HRESULT Yelo_IDirect3DDevice9::GetBackBuffer(THIS_ UINT iSwapChain,UINT iBackBuffer,D3DBACKBUFFER_TYPE Type,IDirect3DSurface9** ppBackBuffer) 
{ return Yelo_pD3DDevice->GetBackBuffer(iSwapChain,iBackBuffer, Type, ppBackBuffer); }

HRESULT Yelo_IDirect3DDevice9::GetDepthStencilSurface(IDirect3DSurface9 **ppZStencilSurface) 
{ return Yelo_pD3DDevice->GetDepthStencilSurface(ppZStencilSurface); }

HRESULT Yelo_IDirect3DDevice9::GetTexture(DWORD Stage, IDirect3DBaseTexture9 **ppTexture) 
{ return Yelo_pD3DDevice->GetTexture(Stage, ppTexture); }

HRESULT Yelo_IDirect3DDevice9::GetTextureStageState(DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD *pValue) 
{ return Yelo_pD3DDevice->GetTextureStageState(Stage, Type, pValue); }

HRESULT Yelo_IDirect3DDevice9::SetTexture(DWORD Stage, IDirect3DBaseTexture9 *pTexture) 
{ return Yelo_pD3DDevice->SetTexture(Stage, pTexture); }

HRESULT Yelo_IDirect3DDevice9::SetTextureStageState(DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD Value) 
{ return Yelo_pD3DDevice->SetTextureStageState(Stage, Type, Value); }

HRESULT Yelo_IDirect3DDevice9::UpdateTexture(IDirect3DBaseTexture9 *pSourceTexture, IDirect3DBaseTexture9 *pDestinationTexture) 
{ return Yelo_pD3DDevice->UpdateTexture(pSourceTexture, pDestinationTexture); }

HRESULT Yelo_IDirect3DDevice9::ValidateDevice(DWORD *pNumPasses) 
{ return Yelo_pD3DDevice->ValidateDevice(pNumPasses); }

HRESULT Yelo_IDirect3DDevice9::GetClipPlane(DWORD Index, float *pPlane) 
{ return Yelo_pD3DDevice->GetClipPlane(Index, pPlane); }

HRESULT Yelo_IDirect3DDevice9::SetClipPlane(DWORD Index, CONST float *pPlane) 
{ return Yelo_pD3DDevice->SetClipPlane(Index, pPlane); }

HRESULT Yelo_IDirect3DDevice9::Clear(DWORD Count, CONST D3DRECT *pRects, DWORD Flags, D3DCOLOR Color, float Z, DWORD Stencil) 
{ return Yelo_pD3DDevice->Clear(Count, pRects, Flags, Color, Z, Stencil); }

HRESULT Yelo_IDirect3DDevice9::GetViewport(D3DVIEWPORT9 *pViewport) 
{ return Yelo_pD3DDevice->GetViewport(pViewport); }

HRESULT Yelo_IDirect3DDevice9::SetViewport(CONST D3DVIEWPORT9 *pViewport) 
{ return Yelo_pD3DDevice->SetViewport(pViewport); }

HRESULT Yelo_IDirect3DDevice9::CreateVertexShader(THIS_ CONST DWORD* pFunction,IDirect3DVertexShader9** ppShader) 
{ return Yelo_pD3DDevice->CreateVertexShader(pFunction, ppShader); }

HRESULT Yelo_IDirect3DDevice9::GetVertexShader(THIS_ IDirect3DVertexShader9** ppShader) 
{ return Yelo_pD3DDevice->GetVertexShader(ppShader); }

HRESULT Yelo_IDirect3DDevice9::SetVertexShader(THIS_ IDirect3DVertexShader9* pShader) 
{
	HRESULT hr = Yelo_pD3DDevice->SetVertexShader(pShader);

	if(Yelo::Memory::IsYeloEnabled())
		Yelo::DX9::c_gbuffer_system::VertexShaderChanged(pShader);

	return hr;
}

HRESULT Yelo_IDirect3DDevice9::CreateQuery(THIS_ D3DQUERYTYPE Type,IDirect3DQuery9** ppQuery) 
{ return Yelo_pD3DDevice->CreateQuery(Type,ppQuery); }

HRESULT Yelo_IDirect3DDevice9::SetPixelShaderConstantB(THIS_ UINT StartRegister,CONST BOOL* pConstantData,UINT  BoolCount) 
{ return Yelo_pD3DDevice->SetPixelShaderConstantB(StartRegister,pConstantData,BoolCount); }

HRESULT Yelo_IDirect3DDevice9::GetPixelShaderConstantB(THIS_ UINT StartRegister,BOOL* pConstantData,UINT BoolCount) 
{ return Yelo_pD3DDevice->GetPixelShaderConstantB(StartRegister,pConstantData,BoolCount); }

HRESULT Yelo_IDirect3DDevice9::SetPixelShaderConstantI(THIS_ UINT StartRegister,CONST int* pConstantData,UINT Vector4iCount) 
{ return Yelo_pD3DDevice->SetPixelShaderConstantI(StartRegister,pConstantData,Vector4iCount); }

HRESULT Yelo_IDirect3DDevice9::GetPixelShaderConstantI(THIS_ UINT StartRegister,int* pConstantData,UINT Vector4iCount)
{ return Yelo_pD3DDevice->GetPixelShaderConstantI(StartRegister,pConstantData,Vector4iCount); }

HRESULT Yelo_IDirect3DDevice9::SetPixelShaderConstantF(THIS_ UINT StartRegister,CONST float* pConstantData,UINT Vector4fCount) 
{ return Yelo_pD3DDevice->SetPixelShaderConstantF(StartRegister,pConstantData,Vector4fCount); }

HRESULT Yelo_IDirect3DDevice9::GetPixelShaderConstantF(THIS_ UINT StartRegister,float* pConstantData,UINT Vector4fCount) 
{ return Yelo_pD3DDevice->GetPixelShaderConstantF(StartRegister,pConstantData,Vector4fCount); }

HRESULT Yelo_IDirect3DDevice9::SetStreamSourceFreq(THIS_ UINT StreamNumber,UINT Divider)
{ return Yelo_pD3DDevice->SetStreamSourceFreq(StreamNumber,Divider); }

HRESULT Yelo_IDirect3DDevice9::GetStreamSourceFreq(THIS_ UINT StreamNumber,UINT* Divider) 
{ return Yelo_pD3DDevice->GetStreamSourceFreq(StreamNumber,Divider); }

HRESULT Yelo_IDirect3DDevice9::SetVertexShaderConstantB(THIS_ UINT StartRegister,CONST BOOL* pConstantData,UINT  BoolCount) 
{ return Yelo_pD3DDevice->SetVertexShaderConstantB(StartRegister,pConstantData,BoolCount); }

HRESULT Yelo_IDirect3DDevice9::GetVertexShaderConstantB(THIS_ UINT StartRegister,BOOL* pConstantData,UINT BoolCount)
{ return Yelo_pD3DDevice->GetVertexShaderConstantB(StartRegister,pConstantData,BoolCount); }

HRESULT Yelo_IDirect3DDevice9::SetVertexShaderConstantF(THIS_ UINT StartRegister,CONST float* pConstantData,UINT Vector4fCount) 
{ 
	if(Yelo::Memory::IsYeloEnabled())
		return Yelo::DX9::c_gbuffer_system::SetVertexShaderConstantF_All(Yelo_pD3DDevice, StartRegister, pConstantData, Vector4fCount);
	return Yelo_pD3DDevice->SetVertexShaderConstantF(StartRegister,pConstantData,Vector4fCount); 
}


HRESULT Yelo_IDirect3DDevice9::GetVertexShaderConstantF(THIS_ UINT StartRegister,float* pConstantData,UINT Vector4fCount) 
{ return Yelo_pD3DDevice->GetVertexShaderConstantF(StartRegister,pConstantData,Vector4fCount); }

HRESULT Yelo_IDirect3DDevice9::SetVertexShaderConstantI(THIS_ UINT StartRegister,CONST int* pConstantData,UINT Vector4iCount) 
{ return Yelo_pD3DDevice->SetVertexShaderConstantI(StartRegister,pConstantData,Vector4iCount); }

HRESULT Yelo_IDirect3DDevice9::GetVertexShaderConstantI(THIS_ UINT StartRegister,int* pConstantData,UINT Vector4iCount)
{ return Yelo_pD3DDevice->GetVertexShaderConstantI(StartRegister,pConstantData,Vector4iCount); }

HRESULT Yelo_IDirect3DDevice9::SetFVF(THIS_ DWORD FVF) 
{ return Yelo_pD3DDevice->SetFVF(FVF); }

HRESULT Yelo_IDirect3DDevice9::GetFVF(THIS_ DWORD* pFVF) 
{ return Yelo_pD3DDevice->GetFVF(pFVF); }

HRESULT Yelo_IDirect3DDevice9::CreateVertexDeclaration(THIS_ CONST D3DVERTEXELEMENT9* pVertexElements,IDirect3DVertexDeclaration9** ppDecl) 
{ return Yelo_pD3DDevice->CreateVertexDeclaration(pVertexElements,ppDecl); }

HRESULT Yelo_IDirect3DDevice9::SetVertexDeclaration(THIS_ IDirect3DVertexDeclaration9* pDecl) 
{ return Yelo_pD3DDevice->SetVertexDeclaration(pDecl); }

HRESULT Yelo_IDirect3DDevice9::GetVertexDeclaration(THIS_ IDirect3DVertexDeclaration9** ppDecl) 
{ return Yelo_pD3DDevice->GetVertexDeclaration(ppDecl); }

HRESULT Yelo_IDirect3DDevice9::SetNPatchMode(THIS_ float nSegments) 
{ return Yelo_pD3DDevice->SetNPatchMode(nSegments); }

float Yelo_IDirect3DDevice9::GetNPatchMode(THIS) 
{ return Yelo_pD3DDevice->GetNPatchMode(); }

BOOL Yelo_IDirect3DDevice9::GetSoftwareVertexProcessing(THIS) 
{ return Yelo_pD3DDevice->GetSoftwareVertexProcessing(); }

UINT Yelo_IDirect3DDevice9::GetNumberOfSwapChains(THIS) 
{ return Yelo_pD3DDevice->GetNumberOfSwapChains(); }

HRESULT Yelo_IDirect3DDevice9::EvictManagedResources(THIS) 
{ return Yelo_pD3DDevice->EvictManagedResources(); }

HRESULT Yelo_IDirect3DDevice9::SetSoftwareVertexProcessing(THIS_ BOOL bSoftware) 
{ return Yelo_pD3DDevice->SetSoftwareVertexProcessing(bSoftware); }

HRESULT Yelo_IDirect3DDevice9::SetScissorRect(THIS_ CONST RECT* pRect) 
{ return Yelo_pD3DDevice->SetScissorRect(pRect); }

HRESULT Yelo_IDirect3DDevice9::GetScissorRect(THIS_ RECT* pRect) 
{ return Yelo_pD3DDevice->GetScissorRect(pRect); }

HRESULT Yelo_IDirect3DDevice9::GetSamplerState(THIS_ DWORD Sampler,D3DSAMPLERSTATETYPE Type,DWORD* pValue) 
{ return Yelo_pD3DDevice->GetSamplerState(Sampler,Type,pValue); }

HRESULT Yelo_IDirect3DDevice9::SetSamplerState(THIS_ DWORD Sampler,D3DSAMPLERSTATETYPE Type,DWORD Value) 
{ return Yelo_pD3DDevice->SetSamplerState(Sampler,Type,Value); }

HRESULT Yelo_IDirect3DDevice9::SetDepthStencilSurface(THIS_ IDirect3DSurface9* pNewZStencil) 
{ return Yelo_pD3DDevice->SetDepthStencilSurface(pNewZStencil); }

HRESULT Yelo_IDirect3DDevice9::CreateOffscreenPlainSurface(THIS_ UINT Width,UINT Height,D3DFORMAT Format,D3DPOOL Pool,IDirect3DSurface9** ppSurface,HANDLE* pSharedHandle) 
{ return Yelo_pD3DDevice->CreateOffscreenPlainSurface(Width,Height,Format,Pool,ppSurface,pSharedHandle); }

HRESULT Yelo_IDirect3DDevice9::ColorFill(THIS_ IDirect3DSurface9* pSurface,CONST RECT* pRect,D3DCOLOR color) 
{ return Yelo_pD3DDevice->ColorFill(pSurface,pRect,color); }

HRESULT Yelo_IDirect3DDevice9::StretchRect(THIS_ IDirect3DSurface9* pSourceSurface,CONST RECT* pSourceRect,IDirect3DSurface9* pDestSurface,CONST RECT* pDestRect,D3DTEXTUREFILTERTYPE Filter) 
{ return Yelo_pD3DDevice->StretchRect(pSourceSurface,pSourceRect,pDestSurface,pDestRect,Filter); }

HRESULT Yelo_IDirect3DDevice9::GetFrontBufferData(THIS_ UINT iSwapChain,IDirect3DSurface9* pDestSurface) 
{ return Yelo_pD3DDevice->GetFrontBufferData(iSwapChain,pDestSurface); }

HRESULT Yelo_IDirect3DDevice9::GetRenderTargetData(THIS_ IDirect3DSurface9* pRenderTarget,IDirect3DSurface9* pDestSurface) 
{ return Yelo_pD3DDevice->GetRenderTargetData(pRenderTarget,pDestSurface); }

HRESULT Yelo_IDirect3DDevice9::UpdateSurface(THIS_ IDirect3DSurface9* pSourceSurface,CONST RECT* pSourceRect,IDirect3DSurface9* pDestinationSurface,CONST POINT* pDestPoint) 
{ return Yelo_pD3DDevice->UpdateSurface(pSourceSurface,pSourceRect,pDestinationSurface,pDestPoint); }

HRESULT Yelo_IDirect3DDevice9::SetDialogBoxMode(THIS_ BOOL bEnableDialogs) 
{ return Yelo_pD3DDevice->SetDialogBoxMode(bEnableDialogs); }

HRESULT Yelo_IDirect3DDevice9::GetSwapChain(THIS_ UINT iSwapChain,IDirect3DSwapChain9** pSwapChain)
{ return Yelo_pD3DDevice->GetSwapChain(iSwapChain,pSwapChain); }
#pragma endregion

#pragma region IDirect3D9
ULONG Yelo_IDirect3D9::AddRef() 
{ return Yelo_pD3D->AddRef(); }

HRESULT Yelo_IDirect3D9::QueryInterface(REFIID riid, LPVOID *ppvObj) 
{ return Yelo_pD3D->QueryInterface(riid, ppvObj); }

ULONG Yelo_IDirect3D9::Release() 
{ return Yelo_pD3D->Release(); }

HRESULT Yelo_IDirect3D9::CreateDevice(UINT Adapter, D3DDEVTYPE DeviceType, HWND hFocusWindow, DWORD BehaviorFlags, D3DPRESENT_PARAMETERS *pPresentationParameters, IDirect3DDevice9 **ppReturnedDeviceInterface) 
{
	static Yelo_IDirect3DDevice9 g_d3d_device;

	IDirect3DDevice9** alias = ppReturnedDeviceInterface;

	if(g_d3d_device.IsInitialized())
		g_d3d_device.Initialize(*ppReturnedDeviceInterface, &ppReturnedDeviceInterface);

	*alias = &g_d3d_device;

	HRESULT hr =  Yelo_pD3D->CreateDevice(Adapter, DeviceType, hFocusWindow, BehaviorFlags, pPresentationParameters, ppReturnedDeviceInterface);

	
	if(Yelo::Memory::IsYeloEnabled())
	{
		Yelo::Main::s_dx_component* components;
		const Yelo::int32 component_count = Yelo::Main::GetDXComponents(components);

		for(Yelo::int32 x = 0; x <= component_count; x++)
			components[x].Initialize(*ppReturnedDeviceInterface, pPresentationParameters);
	}

	return hr;
}

HRESULT Yelo_IDirect3D9::EnumAdapterModes(THIS_ UINT Adapter,D3DFORMAT Format,UINT Mode,D3DDISPLAYMODE* pMode) 
{ return Yelo_pD3D->EnumAdapterModes(Adapter, Format,Mode, pMode); }

UINT Yelo_IDirect3D9::GetAdapterCount() 
{ return Yelo_pD3D->GetAdapterCount(); }

HRESULT Yelo_IDirect3D9::GetAdapterDisplayMode(UINT Adapter, D3DDISPLAYMODE *pMode) 
{ return Yelo_pD3D->GetAdapterDisplayMode(Adapter, pMode); }

HRESULT Yelo_IDirect3D9::GetAdapterIdentifier(UINT Adapter, DWORD Flags, D3DADAPTER_IDENTIFIER9 *pIdentifier)
{
	// NOTE: "active camo bug" fix
	// http://www.modacity.net/forums/showpost.php?p=444719&postcount=56
	// Thanks FireScythe!
	// "It just returns a different nVidia VendorId, taken from the config.txt file. Tested on my GeForce 9600GT and works fine."
	HRESULT hr = Yelo_pD3D->GetAdapterIdentifier(Adapter, Flags, pIdentifier);
	if(Yelo::Memory::IsYeloEnabled())
		if (SUCCEEDED(hr) && pIdentifier->VendorId == 0x10de)
			pIdentifier->VendorId = 0x12d2;
	return hr;
}

UINT Yelo_IDirect3D9::GetAdapterModeCount(THIS_ UINT Adapter,D3DFORMAT Format) 
{ return Yelo_pD3D->GetAdapterModeCount(Adapter,Format); }

HMONITOR Yelo_IDirect3D9::GetAdapterMonitor(UINT Adapter) 
{ return Yelo_pD3D->GetAdapterMonitor(Adapter); }

HRESULT Yelo_IDirect3D9::GetDeviceCaps(UINT Adapter, D3DDEVTYPE DeviceType, D3DCAPS9 *pCaps)
{ return Yelo_pD3D->GetDeviceCaps(Adapter, DeviceType, pCaps); }

HRESULT Yelo_IDirect3D9::RegisterSoftwareDevice(void *pInitializeFunction) 
{ return Yelo_pD3D->RegisterSoftwareDevice(pInitializeFunction); }

HRESULT Yelo_IDirect3D9::CheckDepthStencilMatch(UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT AdapterFormat, D3DFORMAT RenderTargetFormat, D3DFORMAT DepthStencilFormat) 
{ return Yelo_pD3D->CheckDepthStencilMatch(Adapter, DeviceType, AdapterFormat, RenderTargetFormat, DepthStencilFormat); }

HRESULT Yelo_IDirect3D9::CheckDeviceFormat(UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT AdapterFormat, DWORD Usage, D3DRESOURCETYPE RType, D3DFORMAT CheckFormat) 
{ return Yelo_pD3D->CheckDeviceFormat(Adapter, DeviceType, AdapterFormat, Usage, RType, CheckFormat); }

HRESULT Yelo_IDirect3D9::CheckDeviceMultiSampleType(THIS_ UINT Adapter,D3DDEVTYPE DeviceType,D3DFORMAT SurfaceFormat,BOOL Windowed,D3DMULTISAMPLE_TYPE MultiSampleType,DWORD* pQualityLevels) 
{ return Yelo_pD3D->CheckDeviceMultiSampleType(Adapter, DeviceType, SurfaceFormat, Windowed, MultiSampleType,pQualityLevels); }

HRESULT Yelo_IDirect3D9::CheckDeviceType(UINT Adapter, D3DDEVTYPE CheckType, D3DFORMAT DisplayFormat, D3DFORMAT BackBufferFormat, BOOL Windowed) 
{ return Yelo_pD3D->CheckDeviceType(Adapter, CheckType, DisplayFormat, BackBufferFormat, Windowed); }

HRESULT Yelo_IDirect3D9::CheckDeviceFormatConversion(THIS_ UINT Adapter,D3DDEVTYPE DeviceType,D3DFORMAT SourceFormat,D3DFORMAT TargetFormat) 
{ return Yelo_pD3D->CheckDeviceFormatConversion(Adapter,DeviceType,SourceFormat,TargetFormat); }
#pragma endregion

bool LoadDX9(HMODULE* mod)
{
	static char proc[] = "Direct3DCreate9";

	if( (*mod = LoadLibraryA("d3d9_proxy.dll")) == NULL )
	{
		static char file[] = "\\d3d9.dll";

		char path[MAX_PATH];
		GetSystemDirectory(path, sizeof(path));
		strcat(path, file);

		*mod = LoadLibraryA(path);
	}

	orig_Direct3DCreate9 = (D3DC9)GetProcAddress(*mod, proc);

	if(orig_Direct3DCreate9 == NULL)
		return false;

	return true;
}

void FreeDX9(HMODULE mod)
{
	FreeLibrary(mod);
}

static D3DC9 orig_Direct3DCreate9;

IDirect3D9 *WINAPI Yelo_Direct3DCreate9(UINT SDKVersion)
{
	static Yelo_IDirect3D9 g_d3d;

	if(!g_d3d.IsInitialized())
		g_d3d.Initialize(orig_Direct3DCreate9(SDKVersion));

	return &g_d3d;
}
#else

void* WINAPI Yelo_Direct3DCreate9(UINT SDKVersion) { return NULL; }

#endif