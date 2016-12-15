/*
	Yelo: Open Sauce SDK
		Halo 2 (Xbox) Edition

	See license\OpenSauce\Halo2_Xbox for specific license information
*/

//////////////////////////////////////////////////////////////////////////
// Sound.cpp
#if __EL_INCLUDE_FILE_ID == __EL_SOUND_SOUND
namespace GameState
{
	ENGINE_DPTR(s_sound_class_globals, _SoundClassGlobals,						0x6C2FA8);
	ENGINE_DPTR(t_game_looping_sound_data, _GameLoopingSounds,					0x6B0D2C);
	ENGINE_DPTR(s_game_sound_globals, _GameSoundGlobals,						0x6B0D28);
	ENGINE_DPTR(s_game_sound_player_effect_globals, _GameSoundPlayerEffects,	0x6DA620);
};


#else
	#error Undefined engine layout include for: __EL_INCLUDE_SOUND
#endif