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
	ENGINE_DPTR(t_object_list_header_data, _ObjectListHeader,		0x6B0D34);
	ENGINE_DPTR(t_list_object_reference_data, _ListObjectReference,	0x6B0D30);
	ENGINE_DPTR(t_hs_nodes_data, _HsNodes,							0x6B3DD8);
	ENGINE_DPTR(t_hs_thread_data, _HsThread,						0x6B3DB8);
	ENGINE_DPTR(t_hs_globals_data, _HsGlobals,						0x6B3DB4);
	ENGINE_DPTR(t_recorded_animations_data, _RecordedAnimations,	0x6BA2DC);
};


#else
	#error Undefined engine layout include for: __EL_INCLUDE_HS
#endif