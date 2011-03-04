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
		FUNC_PTR(AI_ERASE_ALL, 0x1C84A0);
	};

	namespace Cheats
	{
		FUNC_PTR(INF_AMMO1, 0x1031F0);
		FUNC_PTR(INF_AMMO2, 0x101E44);
		FUNC_PTR(INF_AMMO3, 0x10368E);

		FUNC_PTR(UNIT_THROW_GRENADE_MOVE_TO_HAND, 0xE77CD);
	};

	namespace Cinematic
	{
		FUNC_PTR(LIGHTING_SET_PRIMARY_LIGHT, 0xBBFC0);
		FUNC_PTR(LIGHTING_SET_SECONDARY_LIGHT, 0xBC070);
		FUNC_PTR(FADE_IN, 0x154220);
		FUNC_PTR(FADE_OUT, 0x1542A0);
		FUNC_PTR(CINEMATIC_START, 0x13BFF0);
		FUNC_PTR(CINEMATIC_STOP, 0x13CA80);
		FUNC_PTR(CINEMATIC_SCREEN_EFFECT_SET_FILTER_BITMAP, 0x204B0);
		FUNC_PTR(RENDER_LIGHTS_ENABLE_CINEMATIC_SHADOW, 0x30E30);

		DATA_PTR(ScreenEffectStop_BULLSHIT1, 0x4B5690);
		DATA_PTR(ScreenEffectStop_BULLSHIT2, 0x4B569D);

		DATA_PTR(CINEMATIC_LIGHTMAP_SHADOW_ENABLE, 0x509415);
	};

	namespace Game
	{
		FUNC_PTR(GAME_RATE, 0x146860);

		DATA_PTR(GAME_REVERT1, 0x547F6F);
		DATA_PTR(GAME_REVERT2, 0x547F74);
		DATA_PTR(GAME_REVERT3, 0x547F75);

		DATA_PTR(GAME_SAVE, 0x4701EC);
		FUNC_PTR(GAME_WON, 0x1388E0);
		FUNC_PTR(PLAY_SOUND, 0x1896C0);
	};

	namespace Interface
	{
		FUNC_PTR(XINPUT_GET_STATE, 0x409B6C);
		FUNC_PTR(HUD_CINEMATIC_FADE, 0x13B306);
		FUNC_PTR(HUD_MESSAGE_PRINT, 0x24CBEE);
		FUNC_PTR(HUD_ADD_ITEM_MESSAGE, 0x24CC73);
		FUNC_PTR(SCRIPTED_HUD_SET_TIMER_TIME, 0x24C8E2);

		FUNC_PTR(SCRIPTED_HUD_PAUSE_TIMER, 0x24C93F);
		FUNC_PTR(HUD_SET_STATE_TEXT, 0x24CA1D);
	};

	namespace Players
	{
		FUNC_PTR(LOCAL_PLAYER_GET_PLAYER_INDEX, 0x14DE70);
		FUNC_PTR(PLAYER_INDEX_FROM_UNIT_INDEX, 0x14DE90);
	};

	namespace Objects
	{
		FUNC_PTR(OBJECT_IS_OR_CONTAINS_PLAYER, 0xB9CE0);
		FUNC_PTR(OBJECT_HIDE, 0xB9C60);
		FUNC_PTR(OBJECT_SET_POSITION, 0xB75A0);
		FUNC_PTR(UNIT_SCRIPTING_LOWER_WEAPON, 0x11AB10);
		FUNC_PTR(UNIT_SCRIPTING_RAISE_WEAPON, 0x11AAC0);
	};
};