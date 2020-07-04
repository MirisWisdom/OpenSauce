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
	ENGINE_DPTR(t_object_list_header_data, _ObjectListHeader,		0x4F55D8);
	ENGINE_DPTR(t_list_object_reference_data, _ListObjectReference,	0x4F55D4);
	ENGINE_DPTR(t_hs_nodes_data, _HsNodes,							0x4F9394);
	ENGINE_DPTR(t_hs_thread_data, _HsThread,						0x4F9384);
	ENGINE_DPTR(t_hs_globals_data, _HsGlobals,						0x4F9380);
	ENGINE_DPTR(t_recorded_animations_data, _RecordedAnimations,	0x4F5724);
};


#else
	#error Undefined engine layout include for: __EL_INCLUDE_HS
#endif