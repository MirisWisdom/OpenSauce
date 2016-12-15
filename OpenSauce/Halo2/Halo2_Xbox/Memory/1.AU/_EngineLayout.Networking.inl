/*
	Yelo: Open Sauce SDK
		Halo 2 (Xbox) Edition

	See license\OpenSauce\Halo2_Xbox for specific license information
*/

//////////////////////////////////////////////////////////////////////////
// Networking.cpp
#if __EL_INCLUDE_FILE_ID == __EL_NETWORKING_NETWORKING
namespace Networking
{
	FUNC_PTR(TRANSPORT_ENDPOINT_CREATE,				NULL);
	FUNC_PTR(TRANSPORT_ENDPOINT_GET_OPTION,			NULL);
	FUNC_PTR(TRANSPORT_ENDPOINT_SET_OPTION,			NULL);
	FUNC_PTR(TRANSPORT_ENDPOINT_BIND,				NULL);
	FUNC_PTR(TRANSPORT_ENDPOINT_CLOSE,				NULL);
	FUNC_PTR(TRANSPORT_ENDPOINT_READ,				NULL);
	FUNC_PTR(TRANSPORT_ENDPOINT_WRITE,				NULL);
	FUNC_PTR(TRANSPORT_ENDPOINT_READ_FROM,			NULL);
	// fuck the alpha's function man, fuck it -_-
	FUNC_PTR(TRANSPORT_ENDPOINT_WRITE_TO,			NULL);
	FUNC_PTR(TRANSPORT_ENDPOINT_CONNECT,			NULL);
	FUNC_PTR(TRANSPORT_ENDPOINT_ASYNC_IS_CONNECTED,	NULL);
};


//////////////////////////////////////////////////////////////////////////
// Simulation.cpp
#elif __EL_INCLUDE_FILE_ID == __EL_NETWORKING_SIMULATION


//////////////////////////////////////////////////////////////////////////
// Statistics.cpp
#elif __EL_INCLUDE_FILE_ID == __EL_NETWORKING_STATISTICS
namespace Statistics
{
	FUNC_PTR(GAME_RESULTS_FINALIZE_HOOK,	NULL);
	FUNC_PTR(BUILD_POST_GAME_CARNAGE_REPORT,NULL);

	DATA_PTR(GAME_RESULTS_GAME_TYPE,		NULL);
};

namespace GameState
{
	ENGINE_DPTR(Statistics::pcr_stat_player, _PcrPlayerStats,	NULL);
};


#else
	#error Undefined engine layout include for: __EL_INCLUDE_NETWORKING
#endif