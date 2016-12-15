/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <blamlib/Halo1/physics/collision_bsp.hpp>
#include <blamlib/Halo1/physics/collision_models.hpp>

namespace Yelo
{
	namespace Enums
	{
		enum collision_result : _enum
		{
			_collision_result_water,
			_collision_result_unused1, // haven't seen this type used
			_collision_result_bsp,
			_collision_result_object,

			_collision_result_none = NONE,
		};
	};

	namespace Physics
	{
		struct s_collision_result
		{
			Enums::collision_result type;
			PAD16;
			struct {
				int32 first_leaf_index;
				int16 count;
				PAD16;
			}leaves;
			s_scenario_location location;
			UNKNOWN_TYPE(real);
			real_point3d point;
			real_plane3d plane;
			_enum material_type;
			PAD16;
			datum_index object_index;
			s_collision_model_test_result model_test;
			s_collision_bsp_test_surface_result surface;
		}; BOOST_STATIC_ASSERT( sizeof(s_collision_result) == 0x50 );
	};

	namespace blam
	{
		bool PLATFORM_API collision_test_vector(long_flags flags, real_point3d& location, real_vector3d& vector, 
			datum_index object_index, Physics::s_collision_result& collision);
	};
};