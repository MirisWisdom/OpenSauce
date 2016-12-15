/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

namespace Yelo
{
	namespace Cache
	{
		struct s_cache_header_base
		{
			enum {
				k_header_signature = 'head',
				k_footer_signature = 'foot',
			};
		};
	};
};

#include <YeloLib/cache/cache_files_yelo_base.hpp>