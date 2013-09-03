/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
//InitializeForNewMap								DisposeFromOldMap








{BuildNumber::InitializeForNewMap,					nullptr},


{TagGroups::InitializeForNewMap,					nullptr},


{GameState::RuntimeData::InitializeForNewMap,		nullptr},

{ScenarioFauxZones::InitializeForNewMap,			nullptr},
#if PLATFORM_IS_USER






{Rasterizer::PostProcessing::InitializeForNewMap,	Rasterizer::PostProcessing::DisposeFromOldMap},

#endif








{Objects::InitializeForNewMap,						Objects::DisposeFromOldMap},

#if PLATFORM_IS_USER



#endif

#if PLATFORM_IS_DEDI



#endif

{Networking::VersionCheck::InitializeForNewMap,		nullptr},
