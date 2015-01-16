/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

namespace Yelo
{
	namespace Objects
	{
		struct s_damage_data;

		struct s_unit_globals_data
		{
			UNKNOWN_TYPE(int16);
			UNKNOWN_TYPE(int16);
			UNKNOWN_TYPE(bool);
			PAD24;
		};
	};

	namespace blam
	{
		void PLATFORM_API unit_update(const datum_index unit_index);

		void PLATFORM_API unit_set_animation(datum_index unit_index
			, datum_index animation_graph_index
			, int32 animation_index);

		int16 PLATFORM_API unit_find_nearby_seat(datum_index unit_index
			, datum_index target_unit_index
			, __out int16& parent_seat_index);

		bool PLATFORM_API unit_can_enter_seat(datum_index unit_index
			, datum_index target_unit_index
			, int16 seat_index
			, __out datum_index* unit_in_seat_index);

		bool PLATFORM_API unit_enter_seat(datum_index unit_index
			, datum_index target_unit_index
			, int32 seat_index);

		// Calculates the viewing position based on the unit's 'head' marker if it has one. 
		// If not, it will estimate the position based on the object's origin.
		void PLATFORM_API unit_get_camera_position(datum_index unit_index, __out real_point3d* return_position);

		void PLATFORM_API unit_kill(const datum_index unit_index);

		void PLATFORM_API unit_open(datum_index unit_index);

		void PLATFORM_API unit_close(datum_index unit_index);

		void PLATFORM_API unit_scripting_exit_vehicle(datum_index unit_index);

		bool PLATFORM_API unit_try_and_exit_seat(const datum_index unit_index);

		void PLATFORM_API unit_detach_from_parent(const datum_index unit_index);

		void PLATFORM_API unit_exit_seat_end(datum_index unit_index
			, bool no_network_message
			, bool can_run_on_client_side
			, bool );

		void PLATFORM_API unit_can_see_point(datum_index unit_index
			, const real_point3d* point
			, real view_radians);

		// Returns the number of frames remaining in an unit's custom animation
		int16 unit_get_custom_animation_time(datum_index unit_index);

		bool PLATFORM_API unit_start_user_animation(const datum_index unit_index
			, const datum_index animation_definition_index
			, cstring animation_name
			, const bool interpolate);

		void PLATFORM_API unit_drop_current_weapon(const datum_index unit_index, const bool force);

		void PLATFORM_API unit_damage_aftermath(const datum_index unit_index
			, const Objects::s_damage_data* damage_data
			, const _enum damage_flags
			, const real shield_amount
			, const real body_amount
			, void* arg6
			, const int32 damage_part);

		DOC_TODO("Move to unit_script.hpp?")
		void PLATFORM_API unit_scripting_set_current_vitality(const datum_index unit_index
			, const real health
			, const real shield);
	};
};