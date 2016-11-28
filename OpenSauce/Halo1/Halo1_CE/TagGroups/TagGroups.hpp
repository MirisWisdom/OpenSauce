/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <blamlib/Halo1/cache/cache_files.hpp>
#include <blamlib/Halo1/tag_files/tag_groups.hpp>

namespace Yelo
{
	namespace TagGroups
	{
		void Initialize();
		void Dispose();
		void InitializeForNewMap();

		// TODO: refactor to TagTryAndGet and move to tag_groups_base.hpp
		template<typename T>
		const T* TagGet(datum_index tag_index)
		{
			extern void* TagTryAndGetImpl(datum_index tag_index, tag group_tag);

			return CAST_PTR(T*, TagTryAndGetImpl(tag_index, T::k_group_tag));
		}
	};
};