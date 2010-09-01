/*
    Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition
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


namespace BuildCacheFileEx
{
	namespace CullTags
	{
		static void* BUILD_CACHE_FILE_CULL_TAGS_CALL = CAST_PTR(void*, 0x455541);
		static bool PLATFORM_API cull_custom_tags()
		{
			static void* BUILD_CACHE_FILE_CULL_TAGS = CAST_PTR(void*, 0x453260);

			typedef bool (PLATFORM_API* call_proc)();
			static const call_proc build_cache_file_cull_tags = CAST_PTR(call_proc, BUILD_CACHE_FILE_CULL_TAGS);

			bool result = build_cache_file_cull_tags();

			TagGroups::tag_iterator tag_iter;
			tag_iterator_new(tag_iter, NULL_HANDLE);

			// call custom tag's data culling stuff in this loop
// 			datum_index tag_index;
// 			while( !(tag_index = tag_iterator_next(tag_iter)).IsNull() )
// 			{
// 			}

			return result;
		}

		void Initialize()
		{
			Memory::WriteRelativeCall(&cull_custom_tags, BUILD_CACHE_FILE_CULL_TAGS_CALL);
		}

		void Dispose()
		{
		}
	};
};