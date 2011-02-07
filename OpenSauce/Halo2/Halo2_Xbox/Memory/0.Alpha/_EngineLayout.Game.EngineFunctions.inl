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
		FUNC_PTR(AI_ERASE_ALL,	0x1280B0);
	};

	namespace Cheats
	{
		FUNC_PTR(INF_AMMO1, FUNC_PTR_NULL);
		FUNC_PTR(INF_AMMO2, FUNC_PTR_NULL);
		FUNC_PTR(INF_AMMO3, FUNC_PTR_NULL);

		FUNC_PTR(UNIT_THROW_GRENADE_MOVE_TO_HAND,	NULL);
	};

	namespace Cinematic
	{
		FUNC_PTR(LIGHTING_SET_PRIMARY_LIGHT,				0x51740);
		FUNC_PTR(LIGHTING_SET_SECONDARY_LIGHT,				0x51810);
		FUNC_PTR(FADE_IN,									0xE62B0); // TODO: ALPHA REQUIRES DIFF CODE!!
		FUNC_PTR(FADE_OUT,									0xE6340); // TODO: ALPHA REQUIRES DIFF CODE!!
		FUNC_PTR(CINEMATIC_START,							NULL);
		FUNC_PTR(CINEMATIC_STOP,							NULL);
		FUNC_PTR(CINEMATIC_SCREEN_EFFECT_SET_FILTER_BITMAP,	NULL); // Alpha has more args and resembles the function in halo1
		FUNC_PTR(RENDER_LIGHTS_ENABLE_CINEMATIC_SHADOW,		NULL);

		DATA_PTR(ScreenEffectStop_BULLSHIT1,				NULL);
		DATA_PTR(ScreenEffectStop_BULLSHIT2,				NULL);

		DATA_PTR(CINEMATIC_LIGHTMAP_SHADOW_ENABLE,			DATA_PTR_NULL);
	};

	namespace Game
	{
		FUNC_PTR(GAME_RATE,									0x100470); // TODO: ALPHA REQUIRES DIFF CODE!!

		DATA_PTR(GAME_SAVE,									0x6C5D04);
		FUNC_PTR(GAME_WON,									0x6D9DC0);
		FUNC_PTR(PLAY_SOUND,								NULL);
	};

	namespace Interface
	{
		FUNC_PTR(XINPUT_GET_STATE,							0x468C80);
		FUNC_PTR(HUD_CINEMATIC_FADE,						FUNC_PTR_NULL);
		FUNC_PTR(HUD_MESSAGE_PRINT,							0x1AA6C4);
		FUNC_PTR(HUD_ADD_ITEM_MESSAGE,						NULL);
		FUNC_PTR(SCRIPTED_HUD_SET_TIMER_TIME,				NULL);

		FUNC_PTR(SCRIPTED_HUD_PAUSE_TIMER,					NULL);
		FUNC_PTR(HUD_SET_STATE_TEXT,						NULL);
	};

	namespace Players
	{
		FUNC_PTR(LOCAL_PLAYER_GET_PLAYER_INDEX,				NULL);
		FUNC_PTR(PLAYER_INDEX_FROM_UNIT_INDEX,				NULL);
	};

	namespace Objects
	{
		FUNC_PTR(OBJECT_IS_OR_CONTAINS_PLAYER,				NULL);
		FUNC_PTR(OBJECT_HIDE,								NULL);
		FUNC_PTR(OBJECT_SET_POSITION,						NULL);
		FUNC_PTR(UNIT_SCRIPTING_LOWER_WEAPON,				NULL);
		FUNC_PTR(UNIT_SCRIPTING_RAISE_WEAPON,				NULL);
	};
};