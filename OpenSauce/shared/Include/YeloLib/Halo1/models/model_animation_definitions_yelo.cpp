/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include <YeloLib/Halo1/models/model_animation_definitions_yelo.hpp>

namespace Yelo
{
	namespace Animations
	{
#if PLATFORM_IS_EDITOR
		static s_animation_list_entry equipment_animation_list_entries[] = {
			{},
		};
		s_animation_list equipment_animation_list = {
			Enums::k_number_of_equipment_animations,
			equipment_animation_list_entries
		};

		static s_animation_list_entry projectile_animation_list_entries[] = {
			{},
		};
		s_animation_list projectile_animation_list = {
			Enums::k_number_of_projectile_animations,
			projectile_animation_list_entries,
		};
#endif
	};

	namespace TagGroups
	{
	};
};