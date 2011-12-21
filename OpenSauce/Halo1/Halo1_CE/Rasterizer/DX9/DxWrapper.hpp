/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if PLATFORM_IS_USER && defined(DX_WRAPPER)


#pragma region IDirect3DDevice9
class Yelo_IDirect3DDevice9 : public IDirect3DDevice9 
{
private:
	LPDIRECT3DDEVICE9 Yelo_pD3DDevice;

public:
	void Initialize(LPDIRECT3DDEVICE9 pDevice, LPDIRECT3DDEVICE9 **ppDevice);
	void Initialize() { Yelo_pD3DDevice = NULL; }
	bool IsInitialized() { return Yelo_pD3DDevice != NULL; }

	STDMETHOD_(ULONG, AddRef)(THIS);
	STDMETHOD(BeginStateBlock)(THIS);
	STDMETHOD(CreateAdditionalSwapChain)(THIS_ D3DPRESENT_PARAMETERS *pPresentationParameters, IDirect3DSwapChain9 **pSwapChain);
	STDMETHOD(CreateCubeTexture)(THIS_ UINT EdgeLength,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DCubeTexture9** ppCubeTexture,HANDLE* pSharedHandle);
	STDMETHOD(CreateDepthStencilSurface)(THIS_ UINT Width,UINT Height,D3DFORMAT Format,D3DMULTISAMPLE_TYPE MultiSample,DWORD MultisampleQuality,BOOL Discard,IDirect3DSurface9** ppSurface,HANDLE* pSharedHandle);
	STDMETHOD(CreateIndexBuffer)(THIS_ UINT Length,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DIndexBuffer9** ppIndexBuffer,HANDLE* pSharedHandle);
	STDMETHOD(CreateRenderTarget)(THIS_ UINT Width,UINT Height,D3DFORMAT Format,D3DMULTISAMPLE_TYPE MultiSample,DWORD MultisampleQuality,BOOL Lockable,IDirect3DSurface9** ppSurface,HANDLE* pSharedHandle);
	STDMETHOD(CreateStateBlock)(THIS_ D3DSTATEBLOCKTYPE Type,IDirect3DStateBlock9** ppSB);
	STDMETHOD(CreateTexture)(THIS_ UINT Width,UINT Height,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DTexture9** ppTexture,HANDLE* pSharedHandle);
	STDMETHOD(CreateVertexBuffer)(THIS_ UINT Length,DWORD Usage,DWORD FVF,D3DPOOL Pool,IDirect3DVertexBuffer9** ppVertexBuffer,HANDLE* pSharedHandle);
	STDMETHOD(CreateVolumeTexture)(THIS_ UINT Width,UINT Height,UINT Depth,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DVolumeTexture9** ppVolumeTexture,HANDLE* pSharedHandle);
	STDMETHOD(DeletePatch)(THIS_ UINT Handle);
	STDMETHOD(DrawIndexedPrimitive)(THIS_ D3DPRIMITIVETYPE,INT BaseVertexIndex,UINT MinVertexIndex,UINT NumVertices,UINT startIndex,UINT primCount);
	STDMETHOD(DrawIndexedPrimitiveUP)(THIS_ D3DPRIMITIVETYPE PrimitiveType, UINT MinVertexIndex, UINT NumVertexIndices, UINT PrimitiveCount, CONST void *pIndexData, D3DFORMAT IndexDataFormat, CONST void *pVertexStreamZeroData, UINT VertexStreamZeroStride);
	STDMETHOD(DrawPrimitive)(THIS_ D3DPRIMITIVETYPE PrimitiveType, UINT StartVertex, UINT PrimitiveCount);
	STDMETHOD(DrawPrimitiveUP)(THIS_ D3DPRIMITIVETYPE PrimitiveType, UINT PrimitiveCount, CONST void *pVertexStreamZeroData, UINT VertexStreamZeroStride);
	STDMETHOD(DrawRectPatch)(THIS_ UINT Handle, CONST float *pNumSegs, CONST D3DRECTPATCH_INFO *pRectPatchInfo);
	STDMETHOD(DrawTriPatch)(THIS_ UINT Handle, CONST float *pNumSegs, CONST D3DTRIPATCH_INFO *pTriPatchInfo);
	STDMETHOD(EndStateBlock)(THIS_ IDirect3DStateBlock9** ppSB);
	STDMETHOD_(UINT, GetAvailableTextureMem)(THIS);
	STDMETHOD(GetBackBuffer)(THIS_ UINT iSwapChain,UINT iBackBuffer,D3DBACKBUFFER_TYPE Type,IDirect3DSurface9** ppBackBuffer);
	STDMETHOD(GetClipPlane)(THIS_ DWORD Index, float *pPlane);
	STDMETHOD(GetClipStatus)(THIS_ D3DCLIPSTATUS9 *pClipStatus);
	STDMETHOD(GetCreationParameters)(THIS_ D3DDEVICE_CREATION_PARAMETERS *pParameters);
	STDMETHOD(GetCurrentTexturePalette)(THIS_ UINT *PaletteNumber);
	STDMETHOD(GetDepthStencilSurface)(THIS_ IDirect3DSurface9 **ppZStencilSurface);
	STDMETHOD(GetDeviceCaps)(THIS_ D3DCAPS9 *pCaps);
	STDMETHOD(GetDirect3D)(THIS_ IDirect3D9 **ppD3D9);
	STDMETHOD(GetDisplayMode)(THIS_ UINT iSwapChain,D3DDISPLAYMODE* pMode);
	STDMETHOD(GetFrontBufferData)(THIS_ UINT iSwapChain,IDirect3DSurface9* pDestSurface);
	STDMETHOD(GetFVF)(THIS_ DWORD* pFVF);
	STDMETHOD_(void, GetGammaRamp)(THIS_ UINT iSwapChain,D3DGAMMARAMP* pRamp);
	STDMETHOD(GetIndices)(THIS_ IDirect3DIndexBuffer9** ppIndexData);
	STDMETHOD(GetLight)(THIS_ DWORD Index, D3DLIGHT9 *pLight);
	STDMETHOD(GetLightEnable)(THIS_ DWORD Index, BOOL *pEnable);
	STDMETHOD(GetMaterial)(THIS_ D3DMATERIAL9 *pMaterial);
	STDMETHOD_(float, GetNPatchMode)(THIS);
	STDMETHOD_(UINT, GetNumberOfSwapChains)(THIS);
	STDMETHOD(GetPaletteEntries)(THIS_ UINT PaletteNumber, PALETTEENTRY *pEntries);
	STDMETHOD(GetPixelShader)(THIS_ IDirect3DPixelShader9** ppShader);
	STDMETHOD(GetPixelShaderConstantB)(THIS_ UINT StartRegister,BOOL* pConstantData,UINT BoolCount);
	STDMETHOD(GetPixelShaderConstantF)(THIS_ UINT StartRegister,float* pConstantData,UINT Vector4fCount);
	STDMETHOD(GetPixelShaderConstantI)(THIS_ UINT StartRegister,int* pConstantData,UINT Vector4iCount);
	STDMETHOD(GetRasterStatus)(THIS_ UINT iSwapChain,D3DRASTER_STATUS* pRasterStatus);
	STDMETHOD(GetRenderState)(THIS_ D3DRENDERSTATETYPE State, DWORD *pValue);
	STDMETHOD(GetRenderTarget)(THIS_ DWORD RenderTargetIndex,IDirect3DSurface9** ppRenderTarget);
	STDMETHOD(GetRenderTargetData)(THIS_ IDirect3DSurface9* pRenderTarget,IDirect3DSurface9* pDestSurface);
	STDMETHOD(GetSamplerState)(THIS_ DWORD Sampler,D3DSAMPLERSTATETYPE Type,DWORD* pValue);
	STDMETHOD(GetScissorRect)(THIS_ RECT* pRect);
	STDMETHOD_(BOOL, GetSoftwareVertexProcessing)(THIS);
	STDMETHOD(GetStreamSource)(THIS_ UINT StreamNumber,IDirect3DVertexBuffer9** ppStreamData,UINT* OffsetInBytes,UINT* pStride);
	STDMETHOD(GetStreamSourceFreq)(THIS_ UINT StreamNumber,UINT* Divider);
	STDMETHOD(GetSwapChain)(THIS_ UINT iSwapChain,IDirect3DSwapChain9** pSwapChain);
	STDMETHOD(GetTexture)(THIS_ DWORD Stage, IDirect3DBaseTexture9 **ppTexture);
	STDMETHOD(GetTextureStageState)(THIS_ DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD *pValue);
	STDMETHOD(GetTransform)(THIS_ D3DTRANSFORMSTATETYPE State, D3DMATRIX *pMatrix);
	STDMETHOD(GetVertexDeclaration)(THIS_ IDirect3DVertexDeclaration9** ppDecl);
	STDMETHOD(GetVertexShader)(THIS_ IDirect3DVertexShader9** ppShader);
	STDMETHOD(GetVertexShaderConstantB)(THIS_ UINT StartRegister,BOOL* pConstantData,UINT BoolCount);
	STDMETHOD(GetVertexShaderConstantF)(THIS_ UINT StartRegister,float* pConstantData,UINT Vector4fCount);
	STDMETHOD(GetVertexShaderConstantI)(THIS_ UINT StartRegister,int* pConstantData,UINT Vector4iCount);
	STDMETHOD(GetViewport)(THIS_ D3DVIEWPORT9 *pViewport);

	STDMETHOD_(void, SetCursorPosition)(THIS_ int X, int Y, DWORD Flags);
	STDMETHOD(SetCursorProperties)(THIS_ UINT XHotSpot, UINT YHotSpot, IDirect3DSurface9 *pCursorBitmap);
	STDMETHOD_(BOOL, ShowCursor)(THIS_ BOOL bShow);

	STDMETHOD(SetClipStatus)(THIS_ CONST D3DCLIPSTATUS9 *pClipStatus);
	STDMETHOD(SetRenderState)(THIS_ D3DRENDERSTATETYPE State, DWORD Value);
	STDMETHOD(SetRenderTarget)(THIS_ DWORD RenderTargetIndex,IDirect3DSurface9* pRenderTarget);
	STDMETHOD(SetTransform)(THIS_ D3DTRANSFORMSTATETYPE State, CONST D3DMATRIX *pMatrix);

	STDMETHOD_(void, SetGammaRamp)(THIS_ UINT iSwapChain,DWORD Flags,CONST D3DGAMMARAMP* pRamp);

	STDMETHOD(SetIndices)(THIS_ IDirect3DIndexBuffer9* pIndexData);

	STDMETHOD(LightEnable)(THIS_ DWORD Index, BOOL Enable);
	STDMETHOD(SetLight)(THIS_ DWORD Index, CONST D3DLIGHT9 *pLight);
	STDMETHOD(SetMaterial)(THIS_ CONST D3DMATERIAL9 *pMaterial);

	STDMETHOD(MultiplyTransform)(THIS_ D3DTRANSFORMSTATETYPE, CONST D3DMATRIX*);
	STDMETHOD(ProcessVertices)(THIS_ UINT SrcStartIndex,UINT DestIndex,UINT VertexCount,IDirect3DVertexBuffer9* pDestBuffer,IDirect3DVertexDeclaration9* pVertexDecl,DWORD Flags);

	STDMETHOD(TestCooperativeLevel)(THIS);

	STDMETHOD(SetCurrentTexturePalette)(THIS_ UINT PaletteNumber);
	STDMETHOD(SetPaletteEntries)(THIS_ UINT PaletteNumber, CONST PALETTEENTRY *pEntries);

	STDMETHOD(CreatePixelShader)(THIS_ CONST DWORD* pFunction,IDirect3DPixelShader9** ppShader);

	STDMETHOD(SetPixelShader)(THIS_ IDirect3DPixelShader9* pShader);

	STDMETHOD(Present)(THIS_ CONST RECT *pSourceRect, CONST RECT *pDestRect, HWND hDestWindowOverride, CONST RGNDATA *pDirtyRegion);
	STDMETHOD(Reset)(THIS_ D3DPRESENT_PARAMETERS *pPresentationParameters);

	STDMETHOD(BeginScene)(THIS);
	STDMETHOD(EndScene)(THIS);

	STDMETHOD(SetStreamSource)(THIS_ UINT StreamNumber,IDirect3DVertexBuffer9* pStreamData,UINT OffsetInBytes,UINT Stride);

	STDMETHOD(SetTexture)(THIS_ DWORD Stage, IDirect3DBaseTexture9 *pTexture);
	STDMETHOD(SetTextureStageState)(THIS_ DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD Value);
	STDMETHOD(UpdateTexture)(THIS_ IDirect3DBaseTexture9 *pSourceTexture, IDirect3DBaseTexture9 *pDestinationTexture);
	STDMETHOD(ValidateDevice)(THIS_ DWORD *pNumPasses);

	STDMETHOD(SetClipPlane)(THIS_ DWORD Index, CONST float *pPlane);

	STDMETHOD(Clear)(THIS_ DWORD Count, CONST D3DRECT *pRects, DWORD Flags, D3DCOLOR Color, float Z, DWORD Stencil);

	STDMETHOD(SetViewport)(THIS_ CONST D3DVIEWPORT9 *pViewport);

	STDMETHOD(CreateVertexShader)(THIS_ CONST DWORD* pFunction,IDirect3DVertexShader9** ppShader);

	STDMETHOD(SetVertexShader)(THIS_ IDirect3DVertexShader9* pShader);

	STDMETHOD(CreateQuery)(THIS_ D3DQUERYTYPE Type,IDirect3DQuery9** ppQuery);
	STDMETHOD(SetPixelShaderConstantB)(THIS_ UINT StartRegister,CONST BOOL* pConstantData,UINT  BoolCount);

	STDMETHOD(SetPixelShaderConstantI)(THIS_ UINT StartRegister,CONST int* pConstantData,UINT Vector4iCount);

	STDMETHOD(SetPixelShaderConstantF)(THIS_ UINT StartRegister,CONST float* pConstantData,UINT Vector4fCount);

	STDMETHOD(SetStreamSourceFreq)(THIS_ UINT StreamNumber,UINT Divider);

	STDMETHOD(SetVertexShaderConstantB)(THIS_ UINT StartRegister,CONST BOOL* pConstantData,UINT  BoolCount);

	STDMETHOD(SetVertexShaderConstantF)(THIS_ UINT StartRegister,CONST float* pConstantData,UINT Vector4fCount);

	STDMETHOD(SetVertexShaderConstantI)(THIS_ UINT StartRegister,CONST int* pConstantData,UINT Vector4iCount);

	STDMETHOD(SetFVF)(THIS_ DWORD FVF);

	STDMETHOD(CreateVertexDeclaration)(THIS_ CONST D3DVERTEXELEMENT9* pVertexElements,IDirect3DVertexDeclaration9** ppDecl);
	STDMETHOD(SetVertexDeclaration)(THIS_ IDirect3DVertexDeclaration9* pDecl);

	STDMETHOD(SetNPatchMode)(THIS_ float nSegments);

	STDMETHOD(SetSoftwareVertexProcessing)(THIS_ BOOL bSoftware);

	STDMETHOD(SetScissorRect)(THIS_ CONST RECT* pRect);

	STDMETHOD(SetSamplerState)(THIS_ DWORD Sampler,D3DSAMPLERSTATETYPE Type,DWORD Value);
	STDMETHOD(SetDepthStencilSurface)(THIS_ IDirect3DSurface9* pNewZStencil);
	STDMETHOD(CreateOffscreenPlainSurface)(THIS_ UINT Width,UINT Height,D3DFORMAT Format,D3DPOOL Pool,IDirect3DSurface9** ppSurface,HANDLE* pSharedHandle);
	STDMETHOD(ColorFill)(THIS_ IDirect3DSurface9* pSurface,CONST RECT* pRect,D3DCOLOR color);
	STDMETHOD(StretchRect)(THIS_ IDirect3DSurface9* pSourceSurface,CONST RECT* pSourceRect,IDirect3DSurface9* pDestSurface,CONST RECT* pDestRect,D3DTEXTUREFILTERTYPE Filter);

	STDMETHOD(UpdateSurface)(THIS_ IDirect3DSurface9* pSourceSurface,CONST RECT* pSourceRect,IDirect3DSurface9* pDestinationSurface,CONST POINT* pDestPoint);
	STDMETHOD(SetDialogBoxMode)(THIS_ BOOL bEnableDialogs);

	STDMETHOD(EvictManagedResources)(THIS);

	STDMETHOD(QueryInterface)(THIS_ REFIID riid, void **ppvObj);
	STDMETHOD_(ULONG, Release)(THIS);
};
#pragma endregion

#pragma region IDirect3D9
class Yelo_IDirect3D9: public IDirect3D9 
{
private:
	LPDIRECT3D9 Yelo_pD3D;

public:
	void Initialize(LPDIRECT3D9 pDirect3D) { Yelo_pD3D = pDirect3D; }
	void Initialize() { Yelo_pD3D = NULL; }
	bool IsInitialized() { return Yelo_pD3D != NULL; }

	STDMETHOD_(ULONG, AddRef)(THIS);
	STDMETHOD(CheckDepthStencilMatch)(THIS_ UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT AdapterFormat, D3DFORMAT RenderTargetFormat, D3DFORMAT DepthStencilFormat);
	STDMETHOD(CheckDeviceFormat)(THIS_ UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT AdapterFormat, DWORD Usage, D3DRESOURCETYPE RType, D3DFORMAT CheckFormat);
	STDMETHOD(CheckDeviceFormatConversion)(THIS_ UINT Adapter,D3DDEVTYPE DeviceType,D3DFORMAT SourceFormat,D3DFORMAT TargetFormat);
	STDMETHOD(CheckDeviceMultiSampleType)(THIS_ UINT Adapter,D3DDEVTYPE DeviceType,D3DFORMAT SurfaceFormat,BOOL Windowed,D3DMULTISAMPLE_TYPE MultiSampleType,DWORD* pQualityLevels);
	STDMETHOD(CheckDeviceType)(THIS_ UINT Adapter, D3DDEVTYPE CheckType, D3DFORMAT DisplayFormat, D3DFORMAT BackBufferFormat, BOOL Windowed);
	STDMETHOD(CreateDevice)(THIS_ UINT Adapter, D3DDEVTYPE DeviceType, HWND hFocusWindow, DWORD BehaviorFlags, D3DPRESENT_PARAMETERS *pPresentationParameters, IDirect3DDevice9 **ppReturnedDeviceInterface);
	STDMETHOD(EnumAdapterModes)(THIS_ UINT Adapter,D3DFORMAT Format,UINT Mode,D3DDISPLAYMODE* pMode);
	STDMETHOD_(UINT, GetAdapterCount)(THIS);
	STDMETHOD(GetAdapterDisplayMode)(THIS_ UINT Adapter, D3DDISPLAYMODE *pMode);
	STDMETHOD(GetAdapterIdentifier)(THIS_ UINT Adapter, DWORD Flags, D3DADAPTER_IDENTIFIER9 *pIdentifier);
	STDMETHOD_(UINT, GetAdapterModeCount)(THIS_ UINT Adapter,D3DFORMAT Format);
	STDMETHOD_(HMONITOR, GetAdapterMonitor)(THIS_ UINT Adapter);
	STDMETHOD(GetDeviceCaps)(THIS_ UINT Adapter, D3DDEVTYPE DeviceType, D3DCAPS9 *pCaps);
	STDMETHOD(QueryInterface)(THIS_ REFIID riid,  void **ppvObj);
	STDMETHOD(RegisterSoftwareDevice)(THIS_ void *pInitializeFunction);
	STDMETHOD_(ULONG, Release)(THIS);
};
#pragma endregion


typedef IDirect3D9 *(WINAPI *D3DC9)(UINT);
extern D3DC9 orig_Direct3DCreate9;

IDirect3D9 *WINAPI Yelo_Direct3DCreate9(UINT SDKVersion);

bool LoadDX9(HMODULE* mod);
void FreeDX9(HMODULE mod);
#else

void* __stdcall Yelo_Direct3DCreate9(unsigned int SDKVersion); // declare anyway so we don't get any linker problems

#endif