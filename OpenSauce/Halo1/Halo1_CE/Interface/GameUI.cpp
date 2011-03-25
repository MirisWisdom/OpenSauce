/*
    Yelo: Open Sauce SDK
		Halo 1 (CE) Edition
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
#include "Common/Precompile.hpp"
#include "Interface/GameUI.hpp"

#if !PLATFORM_IS_DEDI

#include "Memory/MemoryInterface.hpp"
#include "Common/YeloSettings.hpp"
#include "Interface/YeloSettingsInterface.hpp"
#include "Game/Players.hpp"
#include "TagGroups/project_yellow_definitions.hpp"

namespace Yelo
{
#define __EL_INCLUDE_ID			__EL_INCLUDE_INTERFACE
#define __EL_INCLUDE_FILE_ID	__EL_INTERFACE_GAME_UI
#include "Memory/_EngineLayout.inl"

	namespace GameUI
	{
		static void HudHideInterfaceElementsUpdate()
		{
			if (*Players::PlayerControlGlobals()->local_players[0].GetZoomLevel() >= 0)
			{
				if (TagGroups::_global_yelo_globals->flags.hide_health_when_zoomed_bit)
					SET_FLAG(HudUnitInterface()->flags, 0, true);
				if (TagGroups::_global_yelo_globals->flags.hide_shield_when_zoomed_bit)
					SET_FLAG(HudUnitInterface()->flags, 2, true);
				if (TagGroups::_global_yelo_globals->flags.hide_motion_sensor_when_zoomed_bit)
					SET_FLAG(HudUnitInterface()->flags, 4, true);
			}
			else
			{
				if (TagGroups::_global_yelo_globals->flags.hide_health_when_zoomed_bit)
					SET_FLAG(HudUnitInterface()->flags, 0, false);
				if (TagGroups::_global_yelo_globals->flags.hide_shield_when_zoomed_bit)
					SET_FLAG(HudUnitInterface()->flags, 2, false);
				if (TagGroups::_global_yelo_globals->flags.hide_motion_sensor_when_zoomed_bit)
					SET_FLAG(HudUnitInterface()->flags, 4, false);
			}
		}

		s_hud_scripted_globals* HudScriptedGlobals()		DPTR_IMP_GET(hud_scripted_globals);
		s_hud_messaging* HudMessaging()						DPTR_IMP_GET(hud_messaging);
		s_hud_unit_interface* HudUnitInterface()			DPTR_IMP_GET(hud_unit_interface);
		s_hud_weapon_interface* HudWeaponInterface()		DPTR_IMP_GET(hud_weapon_interface);
		s_hud_nav_points* HudNavPoints()					DPTR_IMP_GET(hud_nav_points);
		s_motion_sensor* MotionSensor()						DPTR_IMP_GET(motion_sensor);
		s_first_person_weapons* FirstPersonWeapons()		DPTR_IMP_GET(first_person_weapons);


		s_text_bounds_globals_data* TextBoundsGlobals()		PTR_IMP_GET2(text_bounds_globals);
		s_font_drawing_globals_data* FontDrawingGlobals()	PTR_IMP_GET2(font_drawing_globals);


		void Initialize()
		{
			Menu::Initialize();
			Hud::Initialize();

			Memory::CreateHookRelativeCall(&GameUI::Update, GET_FUNC_VPTR(HUD_UPDATE_HOOK), 0xC3);
			Memory::CreateHookRelativeCall(&GameUI::FirstPersonWeaponsUpdate, GET_FUNC_VPTR(FIRST_PERSON_WEAPONS_UPDATE_HOOK), 0xC3 /*retn*/);
		}

		void Dispose()
		{
			Hud::Dispose();
			Menu::Dispose();
		}

		void PLATFORM_API Update()
		{
			HudHideInterfaceElementsUpdate();
		}

		void PLATFORM_API FirstPersonWeaponsUpdate()
		{
		}
	};
};

#include "Interface/GameUI_Hud.inl"
#include "Interface/GameUI_Menu.inl"

#endif