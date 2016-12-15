/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <blamlib/Halo1/physics/collision_bsp_definitions.hpp>

namespace Yelo
{
	namespace Enums
	{
		enum {
			k_maximum_bsp2d_depth = 64,
			k_maximum_bsp3d_depth = 128,
		};
	};

	namespace Flags
	{
		enum collision_flags : long_flags
		{
			_collision_front_facing_surfaces_bit,
			_collision_back_facing_surfaces_bit,
			_collision_ignore_two_sided_surfaces_bit,
			_collision_ignore_invisible_surfaces_bit,
			_collision_ignore_breakable_surfaces_bit,
			_collision_structure_bit,
			_collision_media_bit,
			_collision_objects_bit,
			_collision_objects_bipeds_bit,
			_collision_objects_vehicles_bit,
			_collision_objects_weapons_bit,
			_collision_objects_equipment_bit,
			_collision_objects_garbage_bit,
			_collision_objects_projectiles_bit,
			_collision_objects_scenery_bit,
			_collision_objects_machines_bit,
			_collision_objects_controls_bit,
			_collision_objects_light_fixtures_bit,
			_collision_objects_placeholders_bit,
			_collision_objects_sound_scenery_bit,
			_collision_try_to_keep_location_valid_bit,
			_collision_skip_passthrough_bipeds_bit,
			_collision_use_vehicle_physics_bit,

			k_number_of_collision_flags,
		};
	};

	namespace Physics
	{
		struct s_collision_surface_result
		{
			int32 index;
			int32 plane_index;
			Flags::collision_surface_flags flags;
			sbyte breakable_surface_index;
			int16 material_index; // collision_model_definition->materials or structure_bsp->collision_materials
		};
		struct s_collision_bsp_test_vector_result
		{
			UNKNOWN_TYPE(real); // set to FLT_MAX most of the time
			real_plane3d* plane; // collision_bsp->planes
			s_collision_surface_result surface;
			int32 stack_depth;
			int32 stack[256]; // leaves indices
		}; BOOST_STATIC_ASSERT( sizeof(s_collision_bsp_test_vector_result) == 0x418 );
	};
};