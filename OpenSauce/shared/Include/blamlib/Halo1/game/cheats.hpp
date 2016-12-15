/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

namespace Yelo
{
	namespace blam
	{
		// Spawn all weapons referenced in the game globals around the player
		void PLATFORM_API cheat_all_weapons();

		// Spawn a warthog near the player
		void PLATFORM_API cheat_spawn_warthog();

		// Teleport the player to the camera's active position
		void PLATFORM_API cheat_teleport_to_camera();

		// Activates camo on [cheat_local_player()]
		void PLATFORM_API cheat_active_camouflage();

		// Activates camo on the local player
		void PLATFORM_API cheat_active_camouflage_local_player();

		// Gets the player for cheating in this game
		datum_index PLATFORM_API cheat_local_player();
	};
};