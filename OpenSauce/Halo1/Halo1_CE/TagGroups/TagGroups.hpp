/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <TagGroups/Halo1/CacheDefinitions.hpp>
#include <TagGroups/Halo1/TagGroupDefinitions.hpp>

namespace Yelo
{
	namespace TagGroups
	{
		s_cache_tag_header* Index();
		s_cache_tag_instance const* Instances(); // Note: if you REALLY have the desire to fuck with the tag instance data, use [Index] to change them

		void Initialize();
		void Dispose();
		void InitializeForNewMap();

		API_INLINE void tag_iterator_new(tag_iterator& iter, const tag group_tag_filter)
		{
			memset(&iter, 0, sizeof(iter));
			iter.group_tag_filter = group_tag_filter;
		}

		datum_index tag_iterator_next(tag_iterator& iter);

		template<typename T> const T* GetTagDefinition(datum_index tag_index)
		{
			if(!tag_index.IsNull() && tag_index.index < Index()->count)
			{
				s_cache_tag_instance const* instance = &Instances()[tag_index.index];

				if(instance->MatchesGroup(T::k_group_tag))
					return instance->Definition<T>();
			}

			return NULL;
		}
	};
};