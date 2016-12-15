/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

#include <blamlib/Halo1/models/model_animation_definitions.hpp>

namespace Yelo
{
	namespace blam
	{
		bool PLATFORM_API weapon_prevents_melee_attack(const datum_index weapon_index);

		bool PLATFORM_API weapon_prevents_grenade_throwing(const datum_index weapon_index);

		void PLATFORM_API weapon_stop_reload(const datum_index weapon_index);

		void PLATFORM_API first_person_weapon_message_from_unit(const datum_index unit_index, const int32 weapon_message_type);

		int16 PLATFORM_API weapon_get_first_person_animation_time(const datum_index weapon_index
			, const int16 frame_type
			, Enums::first_person_weapon_animation animation
			, const int32 arg3);
	};
};