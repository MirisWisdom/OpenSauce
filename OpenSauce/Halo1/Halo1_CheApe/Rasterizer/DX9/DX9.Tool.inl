/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/

static D3DPRESENT_PARAMETERS	PresentationParameters;

HRESULT CreateDevice(D3DPRESENT_PARAMETERS* pPresentationParameters);

void Initialize()
{
	ZeroMemory(&PresentationParameters, sizeof(PresentationParameters));

	PresentationParameters.BackBufferWidth = 1;
	PresentationParameters.BackBufferHeight = 1;
	PresentationParameters.BackBufferFormat = D3DFMT_X8R8G8B8;
	PresentationParameters.BackBufferCount = 1;

	PresentationParameters.MultiSampleType = D3DMULTISAMPLE_NONE;
	PresentationParameters.MultiSampleQuality = 0;

	PresentationParameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
	PresentationParameters.hDeviceWindow = NULL;
	PresentationParameters.Windowed = true;
	PresentationParameters.EnableAutoDepthStencil = false;
	PresentationParameters.AutoDepthStencilFormat = D3DFMT_UNKNOWN;
	PresentationParameters.Flags = D3DPRESENT_RATE_DEFAULT;

	PresentationParameters.FullScreen_RefreshRateInHz = 0;
	PresentationParameters.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	if(FAILED(CreateDevice(&PresentationParameters)))
		__noop;
}
bool Initialize(D3DPRESENT_PARAMETERS* custom_parameters)
{
	if(FAILED(CreateDevice(custom_parameters)))
		return false;

	return true;
}

void Dispose()
{
	Yelo::safe_release(global_d3d_device);
	Yelo::safe_release(global_d3d);
}

HRESULT CreateDevice(D3DPRESENT_PARAMETERS* pPresentationParameters)
{	
	global_d3d = Direct3DCreate9(D3D_SDK_VERSION);
	if (!global_d3d)
	{
		YELO_WARN("rasterizer: failed to create Direct3D object");

		return E_FAIL;
	}			

	HRESULT hr = global_d3d->CreateDevice(
		D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, NULL, D3DCREATE_HARDWARE_VERTEXPROCESSING, pPresentationParameters, &global_d3d_device);

	if(FAILED(hr))
	{
		YELO_WARN("rasterizer: failed to create Direct3D device");

		return E_FAIL;
	}
	return S_OK;
}

D3DPRESENT_PARAMETERS*	D3DPresetParams()		{ return &PresentationParameters; }