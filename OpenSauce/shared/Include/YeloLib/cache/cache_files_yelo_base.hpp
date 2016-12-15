/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

namespace Yelo
{
	namespace Cache
	{
		struct s_cache_header_yelo_base {
			enum {
				k_signature = 'yelo',
			};

			tag signature;
			int16 version;
		};
	};
};