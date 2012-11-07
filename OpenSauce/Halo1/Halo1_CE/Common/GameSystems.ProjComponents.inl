/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
//Initialize										Dispose									Update
{Settings::Initialize,								Settings::Dispose},
{FileIO::Initialize,								FileIO::Dispose},
#ifdef API_DEBUG
	{Debug::FileInitialize,							Debug::FileDispose},
#endif
{Debug::DumpInitialize,								Debug::DumpDispose},

{GameState::Initialize,								GameState::Dispose},
{BuildNumber::Initialize,							BuildNumber::Dispose},
{Cache::Initialize,									Cache::Dispose},
{DataFiles::Initialize,								DataFiles::Dispose},
{TagGroups::Initialize,								TagGroups::Dispose},

{Scripting::Initialize,								Scripting::Dispose},
{GameState::RuntimeData::Initialize,				GameState::RuntimeData::Dispose},
{ScenarioFauxZones::Initialize,						ScenarioFauxZones::Dispose,				ScenarioFauxZones::Update},
#if PLATFORM_IS_USER
	{Rasterizer::DeviceHooks::Initialize,			Rasterizer::DeviceHooks::Dispose},
	{DX9::Initialize,								DX9::Dispose},
	{DX9::c_gbuffer_system::Initialize,				DX9::c_gbuffer_system::Dispose,			DX9::c_gbuffer_system::Update},
	{Rasterizer::Initialize,						Rasterizer::Dispose},
	{Rasterizer::ShaderDraw::Initialize,			Rasterizer::ShaderDraw::Dispose},
	{Rasterizer::ShaderExtension::Initialize,		Rasterizer::ShaderExtension::Dispose},
	{Rasterizer::PostProcessing::Initialize,		Rasterizer::PostProcessing::Dispose,	Rasterizer::PostProcessing::Update},
	{Camera::Initialize,							Camera::Dispose},
#endif
{Console::Initialize,								Console::Dispose},

{GameEngine::Initialize,							GameEngine::Dispose},
{Networking::Initialize,							Networking::Dispose},
{Players::Initialize,								Players::Dispose},

{Effects::Initialize,								Effects::Dispose},
{AI::Initialize,									AI::Dispose},
{Objects::Initialize,								Objects::Dispose},

#if PLATFORM_IS_USER
	{GameUI::Initialize,							GameUI::Dispose},
	{Input::Initialize,								Input::Dispose},
	{Keystone::Initialize,							Keystone::Dispose},
#endif

#if PLATFORM_IS_DEDI
	{Server::Initialize,							Server::Dispose},
	{Networking::HTTP::Server::Initialize,			Networking::HTTP::Server::Dispose,		Networking::HTTP::Server::Update},
	{Networking::HTTP::Server::MapDownload::Initialize,	Networking::HTTP::Server::MapDownload::Dispose},
#endif
{Networking::HTTP::Client::Initialize,				Networking::HTTP::Client::Dispose,		Networking::HTTP::Client::Update},
{Networking::VersionCheck::Initialize,				Networking::VersionCheck::Dispose,		Networking::VersionCheck::Update},


{Memory::c_function_interface_system::Initialize,	Memory::c_function_interface_system::Dispose},