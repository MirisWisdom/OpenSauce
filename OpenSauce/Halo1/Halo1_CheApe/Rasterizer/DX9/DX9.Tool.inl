/*
    Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition
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
		YELO_ERROR(_error_message_priority_warning, "rasterizer: failed to create Direct3D object");

		return E_FAIL;
	}			

	HRESULT hr = global_d3d->CreateDevice(
		D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, NULL, D3DCREATE_HARDWARE_VERTEXPROCESSING, pPresentationParameters, &global_d3d_device);

	if(FAILED(hr))
	{
		YELO_ERROR(_error_message_priority_warning, "rasterizer: failed to create Direct3D device");

		return E_FAIL;
	}
	return S_OK;
}

D3DPRESENT_PARAMETERS*	D3DPresetParams()		{ return &PresentationParameters; }