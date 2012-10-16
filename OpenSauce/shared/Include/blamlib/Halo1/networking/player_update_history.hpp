/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <blamlib/Halo1/objects/object_structures.hpp>

namespace Yelo
{
	namespace Networking
	{
		struct s_player_update
		{
			int32 id;
			int32 tick_count;
			byte action_collection[32];
			bool driving_vehicle;
			PAD24;
			datum_index parent_object_index;
			real_point3d position;
			real_vector3d transitional_velocity;
			real_vector3d forward;
			Objects::s_object_animation_datum_data animation;
			byte unit_animation[72]; // s_unit_animation_data
			real_vector3d unit_34C;
			real_vector3d unit_358;
			real_vector3d unit_364;
			real_vector3d unit_370;
			long_flags biped_flags;
			BYTE biped_503;
			BYTE biped_501;
			BYTE biped_502;
			BYTE biped_504;
			int16 biped_508;
			PAD16;
			DWORD biped_50C;
			real_vector3d biped_514;
			PAD32; // unused?
			BYTE biped_4D0;
			BYTE biped_4D1;
			BYTE biped_4D2;
			PAD8;
			DWORD biped_4D8;
			struct {													// 0x124
				real_point3d position;
				real_vector3d transitional_velocity;
				real_vector3d angular_velocity;
				byte object_data[sizeof(Objects::s_object_data) - FIELD_OFFSET(Objects::s_object_data, datum_role)];
				real driver_power;
				real gunner_power;
				UNKNOWN_TYPE(int32);
				byte vehicle_data[Enums::k_object_size_vehicle - Enums::k_object_size_unit];
			}vehicle;
			s_player_update* next;
		}; BOOST_STATIC_ASSERT( sizeof(s_player_update) == 0x418 );
		struct s_player_update_history
		{
			int32 next_update_history_id; // maximum = 64
			s_player_update* updates;
			PAD32;
			int32 number_of_playbacks;
			int32 total_ticks;
			int32 total_updates;
			PAD32; // 0x18
			PAD32; // 0x1C
			PAD32; // 0x20
			real avg_prediction_error;
			real avg_ticks_played_back;
		}; BOOST_STATIC_ASSERT( sizeof(s_player_update_history) == 0x2C );
	};
};