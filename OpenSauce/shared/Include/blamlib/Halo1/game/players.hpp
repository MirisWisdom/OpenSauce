/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <blamlib/Halo1/game/player_action.hpp>

namespace Yelo
{
	namespace Enums
	{
		enum {
			k_player_name_length = 11,
		};

		enum multiplayer_team
		{
			_multiplayer_team_red,
			_multiplayer_team_blue,

			k_number_of_multiplayer_teams,
		};
	};

	namespace blam
	{
		void PLATFORM_API player_examine_nearby_vehicle(datum_index player_index, datum_index vehicle_index);

		void PLATFORM_API player_set_action_result(datum_index player_index, datum_index action_object_index, int32 action_result, int32 action_seat_index);

		void PLATFORM_API player_set_action_result_to_network(datum_index player_index, datum_index action_object_index, int32 action_result_type, int32 action_result, int32 action_seat_index, datum_index next_weapon_index);


		datum_index PLATFORM_API player_index_from_unit_index(datum_index unit_index);

		bool PLATFORM_API player_teleport(datum_index player_index, datum_index source_unit_index, const real_point3d& position);

		void PLATFORM_API player_over_shield_screen_effect(datum_index player_index);

		void PLATFORM_API player_active_camo_screen_effect(datum_index player_index);

		void PLATFORM_API player_health_pack_screen_effect(datum_index player_index);
	};
};