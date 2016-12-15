/*
	Yelo: Open Sauce SDK
		Halo 2 (Xbox) Edition

	See license\OpenSauce\Halo2_Xbox for specific license information
*/

//////////////////////////////////////////////////////////////////////////
// Hud.cpp
#if __EL_INCLUDE_FILE_ID == __EL_INTERFACE_HUD
namespace GameState
{
	ENGINE_DPTR(s_hud_scripted_globals, _HudScriptedGlobals,0x510C98);
	ENGINE_DPTR(s_hud_messaging, _HudMessaging,				0x5023F4);
	ENGINE_DPTR(s_hud_nav_points, _HudNavPoints,			0x5023F8);
	ENGINE_DPTR(s_weapon_hud_globals, _WeaponHudGlobals,	0x510C4C);

	ENGINE_DPTR(s_first_person_weapon_globals1, _FpWeapons1,0x4E9BC8);
	ENGINE_DPTR(s_first_person_weapon_globals2, _FpWeapons2,0x4E9BC4);

	ENGINE_DPTR(s_objective_globals, _ObjectiveGlobals,		0x502248);
	ENGINE_DPTR(s_motion_sensor_globals, _MotionSensor,		0x51E994);
};


//////////////////////////////////////////////////////////////////////////
// Input.cpp
#elif __EL_INCLUDE_FILE_ID == __EL_INTERFACE_INPUT
namespace Input
{
	FUNC_PTR(INPUT_UPDATE_HOOK,					0x124ED5);

	//////////////////////////////////////////////////////////////////////////
	// HandleYeloInput
	FUNC_PTR(RENDER_FUNC_SET_DRAW_MODE,			0x133F6);
	//////////////////////////////////////////////////////////////////////////

	ENGINE_PTR(HANDLE, input_gamepad_handles,	0x4E61CC); // [4]
	FUNC_PTR(INPUT_GET_GAMEPAD_STATE,			0x1249A0);
};


//////////////////////////////////////////////////////////////////////////
// Interface.cpp
#elif __EL_INCLUDE_FILE_ID == __EL_INTERFACE_INTERFACE
namespace GameState
{
	ENGINE_DPTR(s_user_interface_controller_globals, _UserInterfaceControllerGlobals,	0x502120);
};


#else
	#error Undefined engine layout include for: __EL_INCLUDE_INTERFACE
#endif