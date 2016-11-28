/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <blamlib/Halo1/networking/message_delta_parameters_protocol.hpp>

namespace Yelo
{
	namespace Enums
	{
		enum {
			k_mdp_max_exposed_parameters = 64,
		};
	};

	namespace MessageDeltas
	{
		struct s_message_delta_parameters
		{
			struct s_locality_reference_position {
				uint32 bits_per_component_full;		// 25, beta used 20
				uint32 bits_per_component_delta;	// 9
				real delta_cutoff_distance;			// 8.0
				real minimum_move_distance;			// 0.0099999998
				uint32 bits_per_component_lan;		// 31, this wasn't in the beta
			}locality;

			struct s_item_placement {
				uint32 bits_x; // 20
				uint32 bits_y; // 20
				uint32 bits_z; // 30
			}item_placement;
		}; BOOST_STATIC_ASSERT( sizeof(s_message_delta_parameters) == 0x20 );

		struct s_parameters_protocol_globals // made-up structure
		{
			bool changeover_enabled;		// g_protocol_changeover_enabled
			PAD24;
			int32 version;					// protocol version
			int32 exposed_params_count;		// g_num_exposed_params
			bool encoding_parameters_set;	// true when a parameters_set is being encoded and queued to be sent across the network
			PAD24;
		}; BOOST_STATIC_ASSERT( sizeof(s_parameters_protocol_globals) == 0x10 );
	};
};