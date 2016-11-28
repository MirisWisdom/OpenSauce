/*
	Yelo: Open Sauce SDK
		Halo 2 (Xbox) Edition

	See license\OpenSauce\Halo2_Xbox for specific license information
*/
#include "Common/Precompile.hpp"
#include "HS/HS.hpp"

namespace Yelo
{
#define __EL_INCLUDE_ID			__EL_INCLUDE_HS
#define __EL_INCLUDE_FILE_ID	__EL_HS_HS
#include "Memory/_EngineLayout.inl"

	namespace GameState
	{
		t_object_list_header_data* _ObjectListHeader()		DPTR_IMP_GET(_ObjectListHeader);
		t_list_object_reference_data* _ListObjectReference()DPTR_IMP_GET(_ListObjectReference);
		t_hs_nodes_data* _HsNodes()							DPTR_IMP_GET(_HsNodes);
		t_hs_thread_data* _HsThread()						DPTR_IMP_GET(_HsThread);
		t_hs_globals_data* _HsGlobals()						DPTR_IMP_GET(_HsGlobals);
		t_recorded_animations_data* _RecordedAnimations()	DPTR_IMP_GET(_RecordedAnimations);
	};
};