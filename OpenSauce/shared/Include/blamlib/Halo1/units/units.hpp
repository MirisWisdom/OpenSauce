/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <blamlib/Halo1/units/unit_structures.hpp>

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

		bool PLATFORM_API unit_try_and_exit_seat(const datum_index unit_index
#if PLATFORM_IS_EDITOR
			, bool can_run_on_client_side
#endif
			);

		void PLATFORM_API unit_detach_from_parent(const datum_index unit_index);

		void PLATFORM_API unit_exit_seat_end(datum_index unit_index
			, bool no_network_message
			, bool can_run_on_client_side
			, bool );

		bool PLATFORM_API unit_can_see_point(const datum_index unit_index
			, const real_point3d* point
			, const real view_radians);

		// Returns the number of frames remaining in an unit's custom animation
		int16 unit_get_custom_animation_time(datum_index unit_index);

		bool PLATFORM_API unit_start_user_animation(const datum_index unit_index
			, const datum_index animation_definition_index
			, cstring animation_name
			, const bool interpolate);

		void PLATFORM_API unit_animation_start_action(const datum_index unit_index, const Enums::unit_replacement_animation_state action_type);

		void PLATFORM_API unit_animation_set_state(const datum_index unit_index, const Enums::unit_animation_state state);

		void PLATFORM_API unit_drop_current_weapon(const datum_index unit_index, const bool force);

		datum_index PLATFORM_API unit_inventory_get_weapon(const datum_index unit_index, const int16 index);

		void PLATFORM_API unit_ready_desired_weapon(const datum_index unit_index, const bool force);

		void PLATFORM_API unit_throw_grenade_release(const datum_index unit_index, const sbyte keyframe);

		void PLATFORM_API unit_cause_player_melee_damage(const datum_index unit_index);

		void PLATFORM_API unit_damage_aftermath(const datum_index unit_index
			, const Objects::s_damage_data* damage_data
			, const _enum damage_flags
			, const real shield_amount
			, const real body_amount
			, void* arg6
			, const int32 damage_part
#if PLATFORM_TYPE == PLATFORM_SAPIEN
			, const datum_index dead_unit_index
#endif
		);

		void PLATFORM_API unit_set_actively_controlled(const datum_index unit_index, const bool controlled);
	};
};