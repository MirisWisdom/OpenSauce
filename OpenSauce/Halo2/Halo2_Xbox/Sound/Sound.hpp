/*
	Yelo: Open Sauce SDK
		Halo 2 (Xbox) Edition

	See license\OpenSauce\Halo2_Xbox for specific license information
*/
#pragma once

#include <blamlib/Halo2/memory/data.hpp>

namespace Yelo
{
	namespace GameState
	{
		struct s_sound_class_globals
		{
			struct {
				UNKNOWN_TYPE(real);
				UNKNOWN_TYPE(real);
				UNKNOWN_TYPE(real);

				// bit 0 - enable ducker
				byte_flags Flags;
				PAD24;
			}Classes[52];
		};
		s_sound_class_globals* _SoundClassGlobals();

		struct s_game_looping_sound_datum : TStructImpl(24)
		{
		};
		typedef Memory::DataArray<s_game_looping_sound_datum, 16> t_game_looping_sound_data;
		t_game_looping_sound_data* _GameLoopingSounds();

		struct s_game_sound_globals : TStructImpl(580)
		{
			TStructGetPtrImpl(bool, AmbienceDetailsEnabled, 0x240);
		};
		s_game_sound_globals* _GameSoundGlobals();

		struct s_game_sound_player_effect_globals : TStructImpl(40)
		{
		};
		s_game_sound_player_effect_globals* _GameSoundPlayerEffects();
	};
};