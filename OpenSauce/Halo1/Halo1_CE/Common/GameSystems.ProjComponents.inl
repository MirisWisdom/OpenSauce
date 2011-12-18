/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/

// This must come first, many systems use the Report filing system
{Settings::Initialize,								Settings::Dispose},
{FileIO::Initialize,								FileIO::Dispose},
// ...like debugging for example
#ifdef API_DEBUG
	{DebugFile::Initialize,								DebugFile::Dispose},
#endif

{GameState::Initialize,								GameState::Dispose},
{Cache::Initialize,									Cache::Dispose},
{DataFiles::Initialize,								DataFiles::Dispose},
{TagGroups::Initialize,								TagGroups::Dispose,					NULL,	TagGroups::InitializeForNewMap},

{Scripting::Initialize,								Scripting::Dispose},	// No active code in Update game hook currently
#if PLATFORM_IS_USER
	{DX9::Initialize,								DX9::Dispose},
	{DX9::c_gbuffer_system::Initialize,				DX9::c_gbuffer_system::Dispose,		NULL, NULL, NULL, DX9::c_gbuffer_system::Update},
	{Rasterizer::Initialize,						Rasterizer::Dispose},
	{Rasterizer::ShaderExtension::Initialize,		Rasterizer::ShaderExtension::Dispose},
	{
		Rasterizer::PostProcessing::Initialize,
		Rasterizer::PostProcessing::Dispose,
		NULL,
		Rasterizer::PostProcessing::InitializeForNewMap,
		Rasterizer::PostProcessing::DisposeFromOldMap,
		Rasterizer::PostProcessing::Update
	},
	{Camera::Initialize,							Camera::Dispose},
#endif
{Networking::VersionCheck::Initialize,				Networking::VersionCheck::Dispose,	NULL, Networking::VersionCheck::InitializeForNewMap, NULL, Networking::VersionCheck::Update},
//{Console::Initialize,								Console::Dispose},		// No active code currently

{GameEngine::Initialize,							GameEngine::Dispose},
{Networking::Initialize,							Networking::Dispose},
{Players::Initialize,								Players::Dispose},		// No active code in Update game hook currently

//{Effects::Initialize,								Effects::Dispose},		// No active code currently
//{AI::Initialize,									AI::Dispose},			// No active code currently
{Objects::Initialize,								Objects::Dispose,					NULL,		Objects::InitializeForNewMap},		// No active code in Update game hook currently

#if PLATFORM_IS_USER
	{GameUI::Initialize,							GameUI::Dispose},
	//{Input::Initialize,								Input::Dispose},	// No active code currently
	{Keystone::Initialize,							Keystone::Dispose},
#endif

{Memory::c_function_interface_system::Initialize,	Memory::c_function_interface_system::Dispose},