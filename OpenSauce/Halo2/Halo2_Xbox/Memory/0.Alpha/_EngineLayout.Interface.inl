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
// Hud.cpp
#if __EL_INCLUDE_FILE_ID == __EL_INTERFACE_HUD
namespace GameState
{
	ENGINE_DPTR(s_hud_scripted_globals, _HudScriptedGlobals,	0x6D9E58);
	ENGINE_DPTR(s_hud_messaging, _HudMessaging,					0x6C4110);
	ENGINE_DPTR(s_hud_nav_points, _HudNavPoints,				0x6D3F64);
	ENGINE_DPTR(s_weapon_hud_globals, _WeaponHudGlobals,		0x6DA5E4);

	ENGINE_DPTR(s_unit_hud_globals, _UnitHudGlobals,			0x6D9E44);
	ENGINE_DPTR(s_first_person_weapon_globals, _FpWeapons,		0x65E360);

	ENGINE_DPTR(s_objective_globals, _ObjectiveGlobals,			0x6C410C);
	ENGINE_DPTR(s_motion_sensor_globals, _MotionSensor,			0x6DA618);
};


//////////////////////////////////////////////////////////////////////////
// Input.cpp
#elif __EL_INCLUDE_FILE_ID == __EL_INTERFACE_INPUT
namespace Input
{
	FUNC_PTR(INPUT_UPDATE_HOOK,			0xB54D7);

	//////////////////////////////////////////////////////////////////////////
	// HandleYeloInput
	FUNC_PTR(RENDER_FUNC_SET_DRAW_MODE,	NULL);
	//////////////////////////////////////////////////////////////////////////

	FUNC_PTR(INPUT_GET_GAMEPAD_STATE,	0xB4B90);
};


//////////////////////////////////////////////////////////////////////////
// Interface.cpp
#elif __EL_INCLUDE_FILE_ID == __EL_INTERFACE_INTERFACE
namespace GameState
{
	ENGINE_DPTR(s_user_interface_controller_globals, _UserInterfaceControllerGlobals,	0x6C2FD4);
};


#else
	#error Undefined engine layout include for: __EL_INCLUDE_INTERFACE
#endif