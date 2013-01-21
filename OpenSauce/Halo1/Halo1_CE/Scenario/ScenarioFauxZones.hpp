/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

namespace Yelo
{
	namespace ScenarioFauxZones
	{
		void Initialize();
		void Dispose();
		void Update(real delta_time);

		void InitializeForNewGameState();

		void InitializeForNewMap();

		void Reset();
		bool SwitchCurrentZoneVariant(cstring variant_name);
		bool SwitchZoneVariant(cstring zone_name, cstring variant_name);
		bool SwitchZoneSky(cstring zone_sky_name);
	};
};