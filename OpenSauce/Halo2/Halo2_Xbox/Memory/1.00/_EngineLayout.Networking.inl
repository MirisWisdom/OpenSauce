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
	FUNC_PTR(TRANSPORT_ENDPOINT_CREATE,				0xB4D50);
	FUNC_PTR(TRANSPORT_ENDPOINT_GET_OPTION,			0xB4E00);
	FUNC_PTR(TRANSPORT_ENDPOINT_SET_OPTION,			0xB4E70);
	FUNC_PTR(TRANSPORT_ENDPOINT_BIND,				0xB4ED0);
	FUNC_PTR(TRANSPORT_ENDPOINT_CLOSE,				0xB4F50);
	FUNC_PTR(TRANSPORT_ENDPOINT_READ,				0xB4FA0);
	FUNC_PTR(TRANSPORT_ENDPOINT_WRITE,				0xB5000);
	FUNC_PTR(TRANSPORT_ENDPOINT_READ_FROM,			0xB5060);
	FUNC_PTR(TRANSPORT_ENDPOINT_WRITE_TO,			0xB5110);
	FUNC_PTR(TRANSPORT_ENDPOINT_CONNECT,			0xB51B0);
	FUNC_PTR(TRANSPORT_ENDPOINT_ASYNC_IS_CONNECTED,	0xB51B0);
};


//////////////////////////////////////////////////////////////////////////
// Simulation.cpp
#elif __EL_INCLUDE_FILE_ID == __EL_NETWORKING_SIMULATION


//////////////////////////////////////////////////////////////////////////
// Statistics.cpp
#elif __EL_INCLUDE_FILE_ID == __EL_NETWORKING_STATISTICS
namespace Statistics
{
	FUNC_PTR(GAME_RESULTS_FINALIZE_HOOK,	0x19641F);
	FUNC_PTR(BUILD_POST_GAME_CARNAGE_REPORT,0x232D77);

	DATA_PTR(GAME_RESULTS_GAME_TYPE,		0x50224C);
};

namespace GameState
{
	ENGINE_DPTR(Statistics::pcr_stat_player, _PcrPlayerStats,	0x55CAF0);
};


#else
	#error Undefined engine layout include for: __EL_INCLUDE_NETWORKING
#endif