/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI
namespace Yelo
{
	namespace Render { namespace Sky
	{
		void Initialize();
		void Dispose();

		void InitializeForNewGameState();
		void HandleGameStateLifeCycle(_enum life_state);

		void InitializeForRenderSky();

		void InitializeForNewMap();
		void DisposeFromOldMap();

		void ResetSkyOverrides(bool update_sky = true);
		void SetSkyOverride(byte sky_index, byte override_index);
	};};
};
#endif