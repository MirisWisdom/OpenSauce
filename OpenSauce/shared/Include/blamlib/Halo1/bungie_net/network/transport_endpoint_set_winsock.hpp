/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <blamlib/Halo1/bungie_net/network/transport_endpoint_winsock.hpp>

namespace Yelo
{
	namespace Networking
	{
		struct s_transport_endpoint_set
		{
			fd_set select_set;

			s_transport_endpoint** ep_array;
			int32 max_endpoints;
			int32 count;
			int32 array_cursor;
		}; BOOST_STATIC_ASSERT( sizeof(s_transport_endpoint_set) == 0x114 );
	};
};