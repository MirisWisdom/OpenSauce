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
#include "Game/EngineFunctions.hpp"
#include "Game/Players.hpp"
#include "Objects/Objects.hpp"

#include <TagGroups/Halo1/item_definitions.hpp>
#include <TagGroups/Halo1/model_definitions.hpp>
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


		static TagGroups::s_weapon_definition const* FirstPersonWeaponGetWeaponDefinition(s_first_person_weapon* fp_weapon)
		{
			datum_index definition_index = *fp_weapon->GetCurrentWeapon();

			if(!definition_index.IsNull())
			{
				s_cache_tag_instance const* tag_instance = Objects::GetObjectDefinition(definition_index);

				return tag_instance->Definition<TagGroups::s_weapon_definition>();
			}

			return NULL;
		}
		static datum_index FirstPersonWeaponGetAnimationGraphIndex(s_first_person_weapon* fp_weapon)
		{
			datum_index anim_graph_index = datum_index::null;

			TagGroups::s_weapon_definition const* weapon = FirstPersonWeaponGetWeaponDefinition(fp_weapon);

			if(weapon != NULL)
				anim_graph_index = weapon->weapon.first_person_animations.tag_index;

			return anim_graph_index;
		}
		static void FirstPersonWeaponSetStatePermutateAnimation(s_first_person_weapon* fp_weapon, _enum animation_state, int16 animation_current_index)
		{
			datum_index animation_graph_index = FirstPersonWeaponGetAnimationGraphIndex(fp_weapon);

			if(!animation_graph_index.IsNull())
			{
				animation_current_index = Engine::AnimationPickRandomPermutation(false, // 1st person stuff won't affect the game state
					animation_graph_index, animation_current_index);
			}

			*fp_weapon->GetAnimationState() = animation_state;
			*fp_weapon->GetAnimationCurrentIndex() = animation_current_index;
		}
		static API_FUNC_NAKED void PLATFORM_API FirstPersonWeaponSetStateHook()
		{
			NAKED_FUNC_START()
				push	edi
				push	ebx
				push	esi
				call	FirstPersonWeaponSetStatePermutateAnimation
			NAKED_FUNC_END(0)
		}
		static void FirstPersonWeaponSetStateHookCreateHook()
		{
			// Our hook only overwrites the AnimationState and AnimationCurrentIndex setting code. 
			// Leaves the AnimationCurrentFrameIndex set alone (ie, frame index will always be zero)
			static const byte k_null_bytes[8] = { Enums::_x86_opcode_nop, Enums::_x86_opcode_nop,
				Enums::_x86_opcode_nop, Enums::_x86_opcode_nop,
				Enums::_x86_opcode_nop, Enums::_x86_opcode_nop,
				Enums::_x86_opcode_nop, Enums::_x86_opcode_nop,};

			Memory::WriteMemory(GET_FUNC_VPTR(FIRST_PERSON_WEAPON_SET_STATE_HOOK), k_null_bytes, sizeof(k_null_bytes));
			Memory::WriteRelativeCall(&FirstPersonWeaponSetStateHook, GET_FUNC_VPTR(FIRST_PERSON_WEAPON_SET_STATE_HOOK), true);
		}

		void Initialize()
		{
			Menu::Initialize();
			Hud::Initialize();

			Memory::CreateHookRelativeCall(&GameUI::Update, GET_FUNC_VPTR(HUD_UPDATE_HOOK), Enums::_x86_opcode_retn);
			Memory::CreateHookRelativeCall(&GameUI::FirstPersonWeaponsUpdate, GET_FUNC_VPTR(FIRST_PERSON_WEAPONS_UPDATE_HOOK), Enums::_x86_opcode_retn);
			FirstPersonWeaponSetStateHookCreateHook();
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