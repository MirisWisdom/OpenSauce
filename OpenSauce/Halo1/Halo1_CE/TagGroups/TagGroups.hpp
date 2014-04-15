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
		Cache::s_cache_tag_header* Index();
		Cache::s_cache_tag_instance const* Instances(); // Don't use this for getting tags, use the TagGet* functions

		void Initialize();
		void Dispose();
		void InitializeForNewMap();

		bool TagIsInstanceOf(datum_index tag_index, tag group_tag);
		template<typename T>
		bool TagIsInstanceOf(datum_index tag_index)
		{
			return TagIsInstanceOf(tag_index, T::k_group_tag);
		}

		template<typename T>
		const T* TagGet(datum_index tag_index)
		{
			extern void* TagGetImpl(datum_index tag_index, tag group_tag);

			return CAST_PTR(T*, TagGetImpl(tag_index, T::k_group_tag));
		}

		// Returns the tag as non-const. Are you sure you want to be writing to tags at runtime?
		template<typename T>
		T* TagGetForModify(datum_index tag_index)
		{
			extern void* TagGetImpl(datum_index tag_index, tag group_tag);

			return CAST_PTR(T*, TagGetImpl(tag_index, T::k_group_tag));
		}

		// 'Unsafe' in that it returns the tag as non-const and doesn't do any bounds checking
		// Useful when you're using tag_iterator and known you're getting some good input
		template<typename T>
		T* TagGetUnsafe(datum_index tag_index)
		{
			extern void* TagGroups::TagGetUnsafeImpl(datum_index tag_index);

			return CAST_PTR(T*, TagGetUnsafeImpl(tag_index));
		}
	};
};