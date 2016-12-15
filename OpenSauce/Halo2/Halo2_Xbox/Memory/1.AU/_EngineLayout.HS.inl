/*
	Yelo: Open Sauce SDK
		Halo 2 (Xbox) Edition

	See license\OpenSauce\Halo2_Xbox for specific license information
*/

//////////////////////////////////////////////////////////////////////////
// HS.cpp
#if __EL_INCLUDE_FILE_ID == __EL_HS_HS
namespace GameState
{
	ENGINE_DPTR(t_object_list_header_data, _ObjectListHeader,		NULL);
	ENGINE_DPTR(t_list_object_reference_data, _ListObjectReference,	NULL);
	ENGINE_DPTR(t_hs_nodes_data, _HsNodes,							NULL);
	ENGINE_DPTR(t_hs_thread_data, _HsThread,						NULL);
	ENGINE_DPTR(t_hs_globals_data, _HsGlobals,						NULL);
	ENGINE_DPTR(t_recorded_animations_data, _RecordedAnimations,	NULL);
};


#else
	#error Undefined engine layout include for: __EL_INCLUDE_HS
#endif