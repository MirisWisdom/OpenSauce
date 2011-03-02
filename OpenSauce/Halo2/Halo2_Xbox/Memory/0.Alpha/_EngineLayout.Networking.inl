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
// Networking.cpp
#if __EL_INCLUDE_FILE_ID == __EL_NETWORKING_NETWORKING
namespace Networking
{
	FUNC_PTR(TRANSPORT_ENDPOINT_CREATE,				0x2FCFC0);
	FUNC_PTR(TRANSPORT_ENDPOINT_GET_OPTION,			0x2FD180);
	FUNC_PTR(TRANSPORT_ENDPOINT_SET_OPTION,			0x2FD2C0);
	FUNC_PTR(TRANSPORT_ENDPOINT_BIND,				0x2FD5D0);
	FUNC_PTR(TRANSPORT_ENDPOINT_CLOSE,				0x2FD6E0);
	FUNC_PTR(TRANSPORT_ENDPOINT_READ,				0x2FD7F0);
	FUNC_PTR(TRANSPORT_ENDPOINT_WRITE,				0x2FD970);
	FUNC_PTR(TRANSPORT_ENDPOINT_READ_FROM,			0x2FDB00);
	// fuck the alpha's function man, fuck it -_-
	FUNC_PTR(TRANSPORT_ENDPOINT_WRITE_TO,			FUNC_PTR_NULL);
	FUNC_PTR(TRANSPORT_ENDPOINT_CONNECT,			0x2FDF50);
	FUNC_PTR(TRANSPORT_ENDPOINT_ASYNC_IS_CONNECTED,	0x2FDF50);
};


//////////////////////////////////////////////////////////////////////////
// Simulation.cpp
#elif __EL_INCLUDE_FILE_ID == __EL_NETWORKING_SIMULATION


//////////////////////////////////////////////////////////////////////////
// Statistics.cpp
#elif __EL_INCLUDE_FILE_ID == __EL_NETWORKING_STATISTICS
namespace Statistics
{
	FUNC_PTR(GAME_RESULTS_FINALIZE_HOOK,		FUNC_PTR_NULL);
	FUNC_PTR(BUILD_POST_GAME_CARNAGE_REPORT,	FUNC_PTR_NULL);

	DATA_PTR(GAME_RESULTS_GAME_TYPE,			FUNC_PTR_NULL);
};

namespace GameState
{
	ENGINE_DPTR(Statistics::pcr_stat_player, _PcrPlayerStats,	DATA_PTR_NULL);
};


#else
	#error Undefined engine layout include for: __EL_INCLUDE_NETWORKING
#endif