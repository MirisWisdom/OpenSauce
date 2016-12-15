/*
	Yelo: Open Sauce SDK
		Halo 2 (Xbox) Edition

	See license\OpenSauce\Halo2_Xbox for specific license information
*/
#include "Common/Precompile.hpp"
#include "Sound/Sound.hpp"

namespace Yelo
{
#define __EL_INCLUDE_ID			__EL_INCLUDE_SOUND
#define __EL_INCLUDE_FILE_ID	__EL_SOUND_SOUND
#include "Memory/_EngineLayout.inl"

	namespace GameState
	{
		s_sound_class_globals* _SoundClassGlobals()						DPTR_IMP_GET(_SoundClassGlobals);
		t_game_looping_sound_data* _GameLoopingSounds()					DPTR_IMP_GET(_GameLoopingSounds);
		s_game_sound_globals* _GameSoundGlobals()						DPTR_IMP_GET(_GameSoundGlobals);
		s_game_sound_player_effect_globals* _GameSoundPlayerEffects()	DPTR_IMP_GET(_GameSoundPlayerEffects);
	};
};