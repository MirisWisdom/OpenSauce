/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

namespace Yelo
{
	namespace TagGroups
	{
		struct model_animation_graph;
	};

	namespace Enums
	{
		enum animation_update_kind : long_enum {
			_animation_update_kind_render_only,
			_animation_update_kind_affects_game_state,
		};
	};

	namespace Animations
	{
		struct s_animation_list_entry
		{
			cstring name;
			_enum type;
			PAD16;
		}; BOOST_STATIC_ASSERT( sizeof(s_animation_list_entry) == 0x8 );
		struct s_animation_list
		{
			int16 count;
			PAD16;
			s_animation_list_entry* entries;
		}; BOOST_STATIC_ASSERT( sizeof(s_animation_list) == 0x8 );
	};

	namespace Objects
	{
		struct s_animation_state
		{
			int16 animation_index;
			int16 frame_index;
		}; BOOST_STATIC_ASSERT( sizeof(s_animation_state) == 0x4 );
	};

	namespace blam
	{
		// If the animation at [animation_index] in [animation_graph_index] is permutated, this will randomly pick one of them.
		// Returns the random [animation_index]
		int16 PLATFORM_API animation_choose_random_permutation_internal(long_enum render_or_affects_game_state, datum_index animation_graph_index, int32 animation_index);
	};
};