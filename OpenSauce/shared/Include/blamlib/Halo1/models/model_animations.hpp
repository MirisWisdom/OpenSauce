/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

namespace Yelo
{
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
};