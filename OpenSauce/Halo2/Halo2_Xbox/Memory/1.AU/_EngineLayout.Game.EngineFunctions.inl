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

namespace Engine
{
	namespace AI
	{
		FUNC_PTR(AI_ERASE_ALL, NULL);
	};

	namespace Cheats
	{
		FUNC_PTR(INF_AMMO1, NULL);
		FUNC_PTR(INF_AMMO2, NULL);
		FUNC_PTR(INF_AMMO3, NULL);

		FUNC_PTR(UNIT_THROW_GRENADE_MOVE_TO_HAND, NULL);
	};

	namespace Cinematic
	{
		FUNC_PTR(LIGHTING_SET_PRIMARY_LIGHT, NULL);
		FUNC_PTR(LIGHTING_SET_SECONDARY_LIGHT, NULL);
		FUNC_PTR(FADE_IN, NULL);
		FUNC_PTR(FADE_OUT, NULL);
		FUNC_PTR(CINEMATIC_START, NULL);
		FUNC_PTR(CINEMATIC_STOP, NULL);
		FUNC_PTR(CINEMATIC_SCREEN_EFFECT_SET_FILTER_BITMAP, NULL);
		FUNC_PTR(RENDER_LIGHTS_ENABLE_CINEMATIC_SHADOW, NULL);

		DATA_PTR(ScreenEffectStop_BULLSHIT1, NULL);
		DATA_PTR(ScreenEffectStop_BULLSHIT2, NULL);

		DATA_PTR(CINEMATIC_LIGHTMAP_SHADOW_ENABLE, NULL);
	};

	namespace Game
	{
		FUNC_PTR(GAME_RATE, NULL);

		DATA_PTR(GAME_REVERT1, NULL);
		DATA_PTR(GAME_REVERT2, NULL);
		DATA_PTR(GAME_REVERT3, NULL);

		DATA_PTR(GAME_SAVE, NULL);
		FUNC_PTR(GAME_WON, NULL);
		FUNC_PTR(PLAY_SOUND, NULL);
	};

	namespace Interface
	{
		FUNC_PTR(XINPUT_GET_STATE, NULL);
		FUNC_PTR(HUD_CINEMATIC_FADE, NULL);
		FUNC_PTR(HUD_MESSAGE_PRINT, NULL);
		FUNC_PTR(HUD_ADD_ITEM_MESSAGE, NULL);
		FUNC_PTR(SCRIPTED_HUD_SET_TIMER_TIME, NULL);

		FUNC_PTR(SCRIPTED_HUD_PAUSE_TIMER, NULL);
		FUNC_PTR(HUD_SET_STATE_TEXT, NULL);
	};

	namespace Players
	{
		FUNC_PTR(LOCAL_PLAYER_GET_PLAYER_INDEX, NULL);
		FUNC_PTR(PLAYER_INDEX_FROM_UNIT_INDEX, NULL);
	};

	namespace Objects
	{
		FUNC_PTR(OBJECT_IS_OR_CONTAINS_PLAYER, NULL);
		FUNC_PTR(OBJECT_HIDE, NULL);
		FUNC_PTR(OBJECT_SET_POSITION, NULL);
		FUNC_PTR(UNIT_SCRIPTING_LOWER_WEAPON, NULL);
		FUNC_PTR(UNIT_SCRIPTING_RAISE_WEAPON, NULL);
	};
};