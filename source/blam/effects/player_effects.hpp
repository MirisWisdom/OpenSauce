/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <yelo/cseries/cseries_yelo_base.hpp>

namespace Yelo
{
	namespace Players
	{
		struct s_player_screen_flash
		{
			_enum type;
			PAD16;
			real intensity;
			UNKNOWN_TYPE(real);
			UNKNOWN_TYPE(real); UNKNOWN_TYPE(real); UNKNOWN_TYPE(real);
			UNKNOWN_TYPE(int32); // unused?
			UNKNOWN_TYPE(int32); // unused?
		}; ASSERT_SIZE(s_player_screen_flash, 0x20);


		struct s_player_effects_data : TStructImpl(0x128)
		{
		};


		s_player_effects_data*			PlayerEffects();
	};
};