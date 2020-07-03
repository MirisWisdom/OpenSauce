/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

#include <blamlib/Halo1/cache/cache_constants.hpp>
#include <blamlib/Halo1/hs/hs_constants.hpp>
#include <blamlib/Halo1/networking/network_connection.hpp> // for k_maximum_network_machine_count
#include <blamlib/Halo1/game/game_configuration.hpp>
#include <blamlib/Halo1/saved_games/saved_game_constants.hpp>

#include <blamlib/Halo1/effects/effects_constants.hpp>
#include <YeloLib/Halo1/effects/effects_constants_yelo.hpp>

#include <blamlib/Halo1/render/render_constants.hpp>
#include <YeloLib/Halo1/render/render_constants_yelo.hpp>

// How much more memory we're adding.
// 50%
#define K_MEMORY_UPGRADE_INCREASE_AMOUNT (1.5f)

namespace Yelo
{

	//////////////////////////////////////////////////////////////////////////
	// Game state memory
	namespace Enums
	{
		enum {
			// Our upgraded memory allocation size.
			k_physical_memory_map_allocation_size_upgrade = 
				CAST(uint32, k_physical_memory_map_allocation_size * K_MEMORY_UPGRADE_INCREASE_AMOUNT),
		};

		BOOST_STATIC_ASSERT( k_physical_memory_map_allocation_size_upgrade >= k_physical_memory_map_allocation_size );
	};


	//////////////////////////////////////////////////////////////////////////
	// Game memory
	namespace Enums
	{
		enum {
			k_maximum_simultaneous_tag_instances =				0x00001400, // 5120
			k_maximum_simultaneous_tag_instances_upgrade = 
				CAST(uint32, Yelo::Enums::k_maximum_simultaneous_tag_instances * K_MEMORY_UPGRADE_INCREASE_AMOUNT),

			k_tag_base_address =				// 0x40440000
				k_physical_memory_base_address + 
				k_game_state_allocation_size,

			k_tag_allocation_size =				0x01700000,
			k_tag_allocation_size_upgrade = 
				CAST(uint32, Enums::k_tag_allocation_size * K_MEMORY_UPGRADE_INCREASE_AMOUNT),

			// Highest Tag Memory Address
			k_tag_max_address =					// 0x41B40000
				k_tag_base_address + 
				k_tag_allocation_size,
			k_tag_max_address_upgrade = 
				k_tag_base_address + k_tag_allocation_size_upgrade,


			k_multiplayer_maximum_additional_players = 0,

			k_multiplayer_maximum_players_upgrade = 
				k_multiplayer_maximum_players + k_multiplayer_maximum_additional_players,

			k_maximum_network_machine_count_upgrade = 
				k_maximum_network_machine_count + k_multiplayer_maximum_additional_players,

			//////////////////////////////////////////////////////////////////////////
			// effects upgrades
			k_maximum_number_of_particles_per_map_upgrade = 
				k_maximum_number_of_particles_per_map + k_maximum_number_of_particles_per_map_upgrade_amount,

			k_maximum_number_of_effects_per_map_upgrade = 
				k_maximum_number_of_effects_per_map + k_maximum_number_of_effects_per_map_upgrade_amount,
			k_maximum_number_of_effect_locations_per_map_upgrade = 
				k_maximum_number_of_effect_locations_per_map + k_maximum_number_of_effect_locations_per_map_upgrade_amount,

			k_maximum_number_of_particle_system_particles_per_map_upgrade = 
				k_maximum_number_of_particle_system_particles_per_map + k_maximum_number_of_particle_system_particles_per_map_upgrade_amount,
			//////////////////////////////////////////////////////////////////////////
		};
	};


	//////////////////////////////////////////////////////////////////////////
	// Scripting
	namespace Enums
	{
		enum {
			k_hs_functions_count = 543, // the normal hs_function count in the engine under an editor build
			k_hs_external_globals_count = 495, // the normal hs external globals count in the engine under an editor build

			k_maximum_hs_syntax_nodes_per_scenario_upgrade = 
				CAST(uint32, k_maximum_hs_syntax_nodes_per_scenario * K_MEMORY_UPGRADE_INCREASE_AMOUNT),
			k_maximum_hs_string_data_per_scenario_upgrade = 
				CAST(uint32, k_maximum_hs_string_data_per_scenario * K_MEMORY_UPGRADE_INCREASE_AMOUNT),

			k_hs_script_functions_count_upgrade = 1024,
			k_hs_external_globals_count_upgrade = 1024 - k_maximum_hs_globals_per_scenario,
		};

		BOOST_STATIC_ASSERT( k_maximum_hs_syntax_nodes_per_scenario_upgrade >= k_maximum_hs_syntax_nodes_per_scenario );
		BOOST_STATIC_ASSERT( k_maximum_hs_string_data_per_scenario_upgrade >= k_maximum_hs_string_data_per_scenario );
	};


	//////////////////////////////////////////////////////////////////////////
	// Rasterizer
	namespace Enums
	{
		enum {
//			k_rasterizer_maximum_dynamic_triangles_upgrade = 
//				CAST(uint32, k_rasterizer_maximum_dynamic_triangles * K_MEMORY_UPGRADE_INCREASE_AMOUNT),
		};

//		BOOST_STATIC_ASSERT( k_rasterizer_maximum_dynamic_triangles_upgrade >= k_rasterizer_maximum_dynamic_triangles );
	};


	//////////////////////////////////////////////////////////////////////////
	// Render
	namespace Enums
	{
		enum {
			k_maximum_rendered_objects_upgrade = 
				k_maximum_rendered_objects + k_maximum_rendered_objects_upgrade_amount,

			k_maximum_rendered_triangles_upgrade =
				CAST(uint32, k_maximum_rendered_triangles * K_MEMORY_UPGRADE_INCREASE_AMOUNT),
		};
	};


	//////////////////////////////////////////////////////////////////////////
	// Cache
	namespace Enums
	{
		enum {
			// Our upgraded max cache size.
			k_max_cache_size_upgrade = 
				CAST(uint32, k_max_cache_size * K_MEMORY_UPGRADE_INCREASE_AMOUNT),

			k_max_cache_vertex_y_index_buffer_size_upgrade = 
				CAST(uint32, k_max_cache_vertex_y_index_buffer_size * K_MEMORY_UPGRADE_INCREASE_AMOUNT),
		};

		BOOST_STATIC_ASSERT( k_max_cache_size_upgrade >= k_max_cache_size );
		BOOST_STATIC_ASSERT( k_max_cache_vertex_y_index_buffer_size_upgrade >= k_max_cache_vertex_y_index_buffer_size );
	};
};

// TODO: comment this if you wish to reference the amount 
// outside the header
//#undef K_MEMORY_UPGRADE_INCREASE_AMOUNT