/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#ifdef API_DEBUG
	#define DX_COMPONENT(name) \
		{ name::Initialize3D, name::OnLostDevice, name::OnResetDevice, name::Render, name::Release }

	#define DX_COMPONENT_DEBUG(name) \
		{ name::Initialize3D, name::OnLostDevice, name::OnResetDevice, name::Render, name::Release }
#else
	#define DX_COMPONENT(name) \
		{ name::Initialize3D, name::OnLostDevice, name::OnResetDevice, name::Render, name::Release }

	#define DX_COMPONENT_DEBUG(name)
#endif


DX_COMPONENT(DX9),
DX_COMPONENT(Rasterizer::ShaderExtension),
DX_COMPONENT(DX9::c_gbuffer_system),
DX_COMPONENT(Rasterizer::PostProcessing),
DX_COMPONENT(Fov),
DX_COMPONENT(Menu),
DX_COMPONENT(Objects::Weapon),
DX_COMPONENT(Hud),
DX_COMPONENT(Objects::Vehicle),
DX_COMPONENT(Networking::VersionCheck),
DX_COMPONENT(Networking::HTTP::Client::MapDownload),


#undef DX_COMPONENT
#undef DX_COMPONENT_DEBUG