#pragma once

#include <blam/memory/byte_swapping.hpp>
#include <blam/cseries/cseries_base.hpp>

namespace Yelo
{
	namespace TagGroups
	{
		struct s_tag_field_definition
		{
			size_t size;
			cstring name;
			byte_swap_code_t* byte_swap_codes;
			cstring code_name;
		};
	};
}
