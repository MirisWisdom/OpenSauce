/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <blam/models/collision_model_definitions.hpp>
#include <blam/physics/collision_bsp.hpp>

namespace Yelo
{
	namespace Physics
	{
		struct s_collision_model_instance
		{
			datum_index object_index;
			TagGroups::collision_model_definition* definition;
			sbyte* region_permutation_indices; // pointer to s_object_data->region_permutation_indices
			real_matrix4x3* node_matrices;
		}; ASSERT_SIZE(s_collision_model_instance, 0x10);

		struct s_collision_model_test_result
		{
			int16 node_index; // collision_model_definition->nodes
			int16 region_index; // collision_model_definition->regions
			int16 collision_bsp_index; // collision_model_node->bsp
			PAD16;
		};
		struct s_collision_model_test_vector_result
		{
			s_collision_model_test_result model_test;
			s_collision_bsp_test_vector_result bsp_test;
		}; ASSERT_SIZE(s_collision_model_test_vector_result, 0x420);
	};
};