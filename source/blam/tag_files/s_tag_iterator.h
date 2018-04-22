#pragma once

#include <blam/cseries/cseries_base.hpp>
#include <blam/memory/data.hpp>

namespace Yelo { namespace TagGroups
{
	struct s_tag_iterator
	{
		Memory::s_data_iterator instances_iterator;
		tag group_tag_filter;
	};

	static_assert(sizeof(s_tag_iterator) == 0x14, "sizeof(s_tag_iterator) == 0x14");
}}
