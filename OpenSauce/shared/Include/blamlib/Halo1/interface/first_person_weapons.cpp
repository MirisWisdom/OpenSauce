/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include <blamlib/Halo1/interface/first_person_weapons.hpp>

namespace Yelo
{
	namespace GameUI
	{
	};

	namespace blam
	{
		void PLATFORM_API first_person_weapons_initialize_for_new_map()
		{
#if /*PLATFORM_IS_CLIENT ||*/ PLATFORM_TYPE == PLATFORM_SAPIEN
			for (auto& player : GameUI::FirstPersonWeapons()->local_players)
			{
				std::memset(&player, 0, sizeof(player));
				*player.GetUnitIndex() = datum_index::null;
				// NOTE: added this to stop needless "local player %d, weapon (0x0), deleted unexpectedly" messages in debug builds
				*player.GetWeaponIndex() = datum_index::null;
				*player.GetAnimationSoundIndex() = datum_index::null;
				*player.GetAnimationStateDuringSound() = NONE;
			}
#endif
		}
	};
};