/*
	Yelo: Open Sauce SDK
		Halo 2 (Xbox) Edition

	See license\OpenSauce\Halo2_Xbox for specific license information
*/
#include "Common/Precompile.hpp"
#include "Interface/Hud.hpp"

namespace Yelo
{
#define __EL_INCLUDE_ID			__EL_INCLUDE_INTERFACE
#define __EL_INCLUDE_FILE_ID	__EL_INTERFACE_HUD
#include "Memory/_EngineLayout.inl"

	namespace Hud
	{
	};

	namespace GameState
	{
		s_hud_scripted_globals* _HudScriptedGlobals()	DPTR_IMP_GET(_HudScriptedGlobals);
		s_hud_messaging* _HudMessaging()				DPTR_IMP_GET(_HudMessaging);
		s_hud_nav_points* _HudNavPoints()				DPTR_IMP_GET(_HudNavPoints);
		s_weapon_hud_globals* _WeaponHudGlobals()		DPTR_IMP_GET(_WeaponHudGlobals);

#if PLATFORM_ID == PLATFORM_H2_ALPHA
		s_unit_hud_globals* _UnitHudGlobals()			DPTR_IMP_GET(_UnitHudGlobals);
		s_first_person_weapon_globals* _FpWeapons()		DPTR_IMP_GET(_FpWeapons);
#else
		s_first_person_weapon_globals1* _FpWeapons1()	DPTR_IMP_GET(_FpWeapons1);
		s_first_person_weapon_globals2* _FpWeapons2()	DPTR_IMP_GET(_FpWeapons2);
#endif

		s_objective_globals* _ObjectiveGlobals()		DPTR_IMP_GET(_ObjectiveGlobals);
		s_motion_sensor_globals* _MotionSensor()		DPTR_IMP_GET(_MotionSensor);
	};
};