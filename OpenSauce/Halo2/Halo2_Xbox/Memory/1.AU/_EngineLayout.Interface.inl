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
	ENGINE_DPTR(s_hud_scripted_globals, _HudScriptedGlobals,NULL);
	ENGINE_DPTR(s_hud_messaging, _HudMessaging,				NULL);
	ENGINE_DPTR(s_hud_nav_points, _HudNavPoints,			NULL);
	ENGINE_DPTR(s_weapon_hud_globals, _WeaponHudGlobals,	NULL);

	ENGINE_DPTR(s_first_person_weapon_globals1, _FpWeapons1,NULL);
	ENGINE_DPTR(s_first_person_weapon_globals2, _FpWeapons2,NULL);

	ENGINE_DPTR(s_objective_globals, _ObjectiveGlobals,		NULL);
	ENGINE_DPTR(s_motion_sensor_globals, _MotionSensor,		NULL);
};


//////////////////////////////////////////////////////////////////////////
// Input.cpp
#elif __EL_INCLUDE_FILE_ID == __EL_INTERFACE_INPUT
namespace Input
{
	FUNC_PTR(INPUT_UPDATE_HOOK,					NULL);

	//////////////////////////////////////////////////////////////////////////
	// HandleYeloInput
	FUNC_PTR(RENDER_FUNC_SET_DRAW_MODE,			NULL);
	//////////////////////////////////////////////////////////////////////////

	ENGINE_PTR(HANDLE, input_gamepad_handles,	NULL); // [4]
	FUNC_PTR(INPUT_GET_GAMEPAD_STATE,			NULL);
};


//////////////////////////////////////////////////////////////////////////
// Interface.cpp
#elif __EL_INCLUDE_FILE_ID == __EL_INTERFACE_INTERFACE
namespace GameState
{
	ENGINE_DPTR(s_user_interface_controller_globals, _UserInterfaceControllerGlobals,	NULL);
};


#else
	#error Undefined engine layout include for: __EL_INCLUDE_INTERFACE
#endif