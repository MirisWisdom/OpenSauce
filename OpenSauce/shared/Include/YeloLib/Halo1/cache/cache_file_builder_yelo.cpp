/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#include "Common/Precompile.hpp"
#include <YeloLib/Halo1/cache/cache_file_builder_yelo.hpp>
#if PLATFORM_ID == PLATFORM_TOOL

#include <blamlib/Halo1/tag_files/tag_field_scanner.hpp>
#include <blamlib/Halo1/tag_files/tag_groups.hpp>
#include <YeloLib/memory/memory_interface_base.hpp>
#include <YeloLib/Halo1/cache/cache_file_memory_gestalt_definitions.hpp>
#include <YeloLib/Halo1/tag_files/tag_group_memory.hpp>
#include "Engine/EngineFunctions.hpp"

namespace Yelo
{
	namespace Cache
	{
		c_cache_file_builder_base::c_cache_file_builder_base()
			: m_tag_index_to_cache_tag_index_table( YELO_NEW_ARRAY(datum_index, Enums::k_maximum_simultaneous_tag_instances_upgrade) )
			, m_current_tag_index( datum_index::null )
		{
			YELO_ASSERT_DISPLAY(m_tag_index_to_cache_tag_index_table, "ran out of memory allocating tag index translation table");

			for(size_t x = 0; x < Enums::k_maximum_simultaneous_tag_instances_upgrade; x++)
				m_tag_index_to_cache_tag_index_table[x] = datum_index::null;
		}
		c_cache_file_builder_base::~c_cache_file_builder_base()
		{
			YELO_DELETE_ARRAY(m_tag_index_to_cache_tag_index_table);
		}

		size_t c_cache_file_builder_base::BuildCacheTagIndexTable(_Out_ size_t& predicted_tag_names_buffer_size)
		{
			predicted_tag_names_buffer_size = 0;
			size_t cache_tag_count = 0;
			auto* instances_data = CAST(Memory::s_data_array*, TagGroups::TagInstances());

			datum_index::salt_t salt = instances_data->GetInitialSalt();
			for (auto instance : TagGroups::TagInstances())
			{
				if (TagInstanceNotSuitableForCache(*instance))
					continue;

				predicted_tag_names_buffer_size += strlen(instance->filename) + 1;

				m_tag_index_to_cache_tag_index_table[cache_tag_count++] =
					datum_index::Create(instance.GetAbsoluteIndex(), salt);

				salt = instances_data->GetNextSalt(salt);
			}

			return cache_tag_count;
		}
		bool c_cache_file_builder_base::TagInstanceNotSuitableForCache(const s_tag_instance& instance)
		{
			return instance.is_orphan || instance.is_reload ||
				blam::tag_group_get(instance.group_tag)->IsDebugOnly();
		}
		void c_cache_file_builder_base::IdentifyCacheBoundTags()
		{
			size_t predicted_tag_names_buffer_size;
			size_t cache_tag_count = BuildCacheTagIndexTable(predicted_tag_names_buffer_size);

			size_t tag_header_size = sizeof(s_cache_tag_header);
			tag_header_size += cache_tag_count * sizeof(s_cache_tag_instance);
			tag_header_size += predicted_tag_names_buffer_size;

			auto tag_header_bytes = std::vector<byte>(tag_header_size);
		}


		c_cache_file_builder::s_cache_info::s_cache_info()
			: memory_buffer( YELO_NEW_ARRAY(byte, Enums::k_tag_allocation_size_upgrade) )
		{
			YELO_ASSERT_DISPLAY(memory_buffer, "ran out of memory allocating cache tag memory buffer");
		}
		c_cache_file_builder::s_cache_info::~s_cache_info()
		{
			YELO_DELETE_ARRAY(memory_buffer);
		}

		c_cache_file_builder::c_cache_file_builder()
			: c_cache_file_builder_base()
			, m_cache()
		{
		}
		c_cache_file_builder::~c_cache_file_builder()
		{
		}

		void c_cache_file_builder::StreamTagBlockToBuffer(const tag_block* block)
		{
			auto* info = block->definition->GetRuntimeInfo();
		}

		void c_cache_file_builder::StreamTagToBuffer(datum_index tag_index)
		{
			YELO_ASSERT( !tag_index.IsNull() );

			const tag_group* group = blam::tag_group_get( blam::tag_get_group_tag(tag_index) );
			YELO_ASSERT_DISPLAY(!group->IsDebugOnly(), "tried to stream a debug-only tag to the cache: %s.%s",
				blam::tag_get_name(tag_index), group->name);

			const tag_block* block = blam::tag_get_root_block(tag_index);

			datum_index cache_tag_index = m_tag_index_to_cache_tag_index_table[ tag_index.index ];
			YELO_ASSERT( !cache_tag_index.IsNull() );
		}
	};
};
#endif
/*
memory	\resource_database
		\constant_pool.cpp
		\hashtable.cpp\.h
tool\cache_file_resource_database.cpp
tag_files\tag_import_definitions.cpp

tag_group_uses_shared_tag_blocks

k_tag_database_max_count

cache		\cache_file_memory*
			\cache_file_database*
			\cache_file_7zip_codec*
			\cache_file_tag_resource*?
			\cache_file_tag_resource_control_manager?
			\cache_file_tag_resource_runtime?
tag\ResourceCache	\Controller
					\PageCache
					\PageInstance
					\PageManger
					\Predictor
					\Publisher
					\ResourceManager
					\SegmentCache
tag_files	\tag_group_memory*
			\tag_group_memory_sharing*
*/