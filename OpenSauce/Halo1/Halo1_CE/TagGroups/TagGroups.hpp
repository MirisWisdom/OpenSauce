/*
    Yelo: Open Sauce SDK
		Halo 1 (CE) Edition
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

		API_INLINE void tag_iterator_new(tag_iterator& iter, const tag group_tag)
		{
			memset(&iter, 0, sizeof(iter));
			iter.group_tag = group_tag;
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