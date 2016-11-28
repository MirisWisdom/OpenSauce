/*
	Yelo: Open Sauce SDK
		Halo 2 (Xbox) Edition

	See license\OpenSauce\Halo2_Xbox for specific license information
*/
#include "Common/Precompile.hpp"
#include "Objects/Widgets.hpp"

namespace Yelo
{
#define __EL_INCLUDE_ID			__EL_INCLUDE_OBJECTS
#define __EL_INCLUDE_FILE_ID	__EL_OBJECTS_WIDGETS
#include "Memory/_EngineLayout.inl"

	namespace GameState
	{
		t_widgets_data* _Widgets()			DPTR_IMP_GET(_Widgets);
		t_antenna_data* _Antenna()			DPTR_IMP_GET(_Antenna);
		t_cloth_data* _Cloth()				DPTR_IMP_GET(_Cloth);
		t_liquid_data* _Liquid()			DPTR_IMP_GET(_Liquid);
		t_lights_data* _Lights()			DPTR_IMP_GET(_Lights);
		s_light_globals* _LightGlobals()	DPTR_IMP_GET(_LightGlobals);
		s_light_clusters* _LightClusters()	DPTR_IMP_GET(_LightClusters);
	};
};