/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/


namespace BuildCacheFileEx
{
	namespace CullTags
	{
		static void* BUILD_CACHE_FILE_CULL_TAGS_CALL = CAST_PTR(void*, 0x455541);
		static bool PLATFORM_API cull_custom_tags()
		{
			bool result = build_cache_file_for_scenario_internals.build_cache_file_cull_tags();

			TagGroups::tag_iterator tag_iter;
			tag_iterator_new(tag_iter);

			// call custom tag's data culling stuff in this loop
			datum_index tag_index;
			while( !(tag_index = tag_iterator_next(tag_iter)).IsNull() )
			{
				switch( tag_get_group_tag(tag_index) )
				{
				case TagGroups::project_yellow_globals::k_group_tag:
					{
						tag_get<TagGroups::project_yellow_globals>(tag_iter.current_datum)->Cull();
					} break;
				}
			}

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