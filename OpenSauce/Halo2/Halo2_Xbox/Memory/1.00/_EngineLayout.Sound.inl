/*
    Yelo: Open Sauce SDK
		Halo 2 (Xbox) Edition
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

//////////////////////////////////////////////////////////////////////////
// Sound.cpp
#if __EL_INCLUDE_FILE_ID == __EL_SOUND_SOUND
namespace GameState
{
	ENGINE_DPTR(s_sound_class_globals, _SoundClassGlobals,						0x502118);
	ENGINE_DPTR(t_game_looping_sound_data, _GameLoopingSounds,					0x4ED28C);
	ENGINE_DPTR(s_game_sound_globals, _GameSoundGlobals,						0x4ED288);
	ENGINE_DPTR(s_game_sound_player_effect_globals, _GameSoundPlayerEffects,	0x51EBF4);
};


#else
	#error Undefined engine layout include for: __EL_INCLUDE_SOUND
#endif