/*
    Yelo: Open Sauce SDK

    Copyright (C) 2005-2010  Kornner Studios (http://kornner.com)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#pragma once

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
			k_physical_memory_base_address =		0x40000000,

			// Original allocation size
			k_physical_memory_map_allocation_size = 0x01B40000,
			// Our upgraded memory allocation size.
			k_physical_memory_map_allocation_size_upgrade = 
				CAST(uint32, k_physical_memory_map_allocation_size * K_MEMORY_UPGRADE_INCREASE_AMOUNT),

			k_game_state_allocation_size =			0x00440000,
		};

		BOOST_STATIC_ASSERT( k_physical_memory_map_allocation_size_upgrade >= k_physical_memory_map_allocation_size );
	};


	//////////////////////////////////////////////////////////////////////////
	// Game memory
	namespace Enums
	{
		enum {
			k_maximum_tag_count =				0x00001400, // 5120

			k_tag_base_address =				// 0x40440000
				k_physical_memory_base_address + 
				k_game_state_allocation_size,

			k_tag_allocation_size =				0x01700000,

			// Highest Tag Memory Address
			k_tag_max_address =					// 0x41B40000
				k_tag_base_address + 
				k_tag_allocation_size,
		};
	};


	//////////////////////////////////////////////////////////////////////////
	// Scripting
	namespace Enums
	{
		enum {
			k_maximum_hs_syntax_nodes_per_scenario = 19001,
			k_maximum_hs_string_data_per_scenario = 256 * 1024,
			k_maximum_hs_globals_per_scenario = 128,
			k_hs_functions_count = 543, // the normal hs_function count in the engine under an editor build

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
			k_rastizer_maximum_dynamic_triangles = 0x4000,

			k_rastizer_maximum_dynamic_triangles_upgrade = 
				CAST(uint32, k_rastizer_maximum_dynamic_triangles * K_MEMORY_UPGRADE_INCREASE_AMOUNT),
		};

		BOOST_STATIC_ASSERT( k_rastizer_maximum_dynamic_triangles_upgrade >= k_rastizer_maximum_dynamic_triangles );
	};


	//////////////////////////////////////////////////////////////////////////
	// Cache
	namespace Enums
	{
		enum {
			// Original maximum cache size
			k_max_cache_size = 0x018000000,
			// Our upgraded max cache size.
			k_max_cache_size_upgrade = 
				CAST(uint32, k_max_cache_size * K_MEMORY_UPGRADE_INCREASE_AMOUNT),
		};

		BOOST_STATIC_ASSERT( k_max_cache_size_upgrade >= k_max_cache_size );
	};


	namespace Konstants
	{
#define K_MAXIMUM_TAG_COUNT_UPGRADE					CAST(uint32, Yelo::Enums::k_maximum_tag_count * K_MEMORY_UPGRADE_INCREASE_AMOUNT)

		API_INLINE uint32 MaximumTagCountUpgrade()	{ return K_MAXIMUM_TAG_COUNT_UPGRADE; }

		API_INLINE int32 TagAllocationSizeUpgrade()	{ return CAST(uint32, Enums::k_tag_allocation_size * K_MEMORY_UPGRADE_INCREASE_AMOUNT); }

		API_INLINE uint32 TagMaxAddressUpgrade()	{ return Enums::k_tag_base_address + TagAllocationSizeUpgrade(); }
	};
};

// TODO: comment this if you wish to reference the amount 
// outside the header
//#undef K_MEMORY_UPGRADE_INCREASE_AMOUNT