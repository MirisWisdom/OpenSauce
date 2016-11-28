/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#include "Common/Precompile.hpp"
#include <YeloLib/Halo1/cache/cache_file_builder_yelo.hpp>
#if PLATFORM_TYPE == PLATFORM_TOOL

#include <blamlib/Halo1/cache/cache_file_builder.hpp>
#include <blamlib/Halo1/cache/predicted_resources.hpp> // for predicated resources
#include <blamlib/Halo1/models/model_definitions.hpp> // for predicated resources
#include <blamlib/Halo1/objects/object_definitions.hpp> // for predicated resources
#include <blamlib/Halo1/shaders/shader_definitions.hpp> // for predicated resources
#include <blamlib/Halo1/tag_files/tag_field_scanner.hpp>
#include <blamlib/Halo1/tag_files/tag_groups.hpp>
#include <YeloLib/memory/memory_interface_base.hpp>
#include <YeloLib/open_sauce/che_ape/che_ape_api.hpp>
#include <YeloLib/Halo1/cache/cache_file_memory_gestalt_definitions.hpp>
#include <YeloLib/Halo1/open_sauce/project_yellow_global_definitions.hpp> // for cull tags
#include <YeloLib/Halo1/shaders/shader_postprocess_definitions.hpp> // for predicated resources
#include <YeloLib/Halo1/tag_files/tag_group_memory.hpp>

namespace Yelo
{
	namespace TagGroups
	{
		// defined in CheApe code
		// TODO: this setup needs to be changed once we finish the BCF code rewrite
		bool YeloToolCheckTagsForGameStateUpgradeRequirements();
	};
	namespace Tool { namespace BuildCacheFileEx {
		// TODO: this setup needs to be changed once we finish the BCF code rewrite
		namespace TagPreprocess
		{
			void preprocess_tags_for_build();
		};
	}; };

	namespace Cache
	{
		static s_cache_header_yelo g_yelo_cache_header;

		s_cache_header_yelo& BuildCacheFileYeloCacheHeader()
		{
			return g_yelo_cache_header;
		}

		bool BuildCacheFilePreprocessTagsForRuntime()
		{
			Tool::BuildCacheFileEx::TagPreprocess::preprocess_tags_for_build();

			return true;
		}

		bool BuildCacheFileCullTagsYelo()
		{
			// call custom tag's data culling stuff in this loop
			for (auto tag_index : TagGroups::c_tag_iterator::all())
			{
				switch (blam::tag_get_group_tag(tag_index))
				{
				case TagGroups::project_yellow_globals::k_group_tag:
					{
						blam::tag_get<TagGroups::project_yellow_globals>(tag_index)->Cull();
					} break;
				}
			}
			return true;
		}

		static void PredicatedResourcesAddResources(
			TagBlock<TagGroups::predicted_resource>& predicted_resources,
			TagGroups::s_shader_model_extension& extension)
		{
			if (!extension.specular_color.map.tag_index.IsNull())
				blam::predicted_resources_add_resource(predicted_resources,
					Enums::_predicted_resource_bitmap,
					extension.specular_color.map.tag_index,
					0);

			if (!extension.base_normal.map.tag_index.IsNull())
				blam::predicted_resources_add_resource(predicted_resources,
					Enums::_predicted_resource_bitmap,
					extension.base_normal.map.tag_index,
					0);

			for (const auto& detail_normal : extension.detail_normals)
				if (!detail_normal.map.tag_index.IsNull())
					blam::predicted_resources_add_resource(predicted_resources,
						Enums::_predicted_resource_bitmap,
						detail_normal.map.tag_index,
						0);
		}
		static bool BuildObjectPredicatedResourcesYelo(datum_index object_definition_index)
		{
			using namespace TagGroups;

			auto* object_tag = blam::tag_get<s_object_definition>(object_definition_index);

			if (object_tag->object.references.render_model.tag_index.IsNull())
				return true;

			auto* gbxmodel_tag = blam::tag_get<gbxmodel_definition>(object_tag->object.references.render_model.tag_index);

			for (const auto& model_shader : gbxmodel_tag->shaders)
			{
				if (model_shader.shader.group_tag != s_shader_model_definition::k_group_tag ||
					model_shader.shader.tag_index.IsNull())
					continue;

				auto* shader_model_tag = blam::tag_get<s_shader_model_definition>(model_shader.shader.tag_index);

				if (shader_model_tag->model.maps.shader_extension.Count != 1)
					continue;

				s_shader_model_extension& extension = shader_model_tag->model.maps.shader_extension[0];
				PredicatedResourcesAddResources(object_tag->object.predicted_resources, extension);
			}

			return true;
		}
		static bool BuildShaderPostprocessGenericPredictedResources(datum_index tag_index)
		{
			auto* shader_tag = blam::tag_get<TagGroups::s_shader_postprocess_generic>(tag_index);
		
			if (shader_tag->shader_code_binary.size == 0 && shader_tag->base_shader.tag_index.IsNull())
			{
				YELO_WARN(
					"\nCheApe: shader_postprocess_generic validity failed on '%s'\n"
					"error: postprocess shader with no binary data also has no base shader defined",
					blam::tag_get_name(tag_index));

				return false;
			}

			for (auto& parameter : shader_tag->parameters)
			{
				if(parameter.value_type.type != Enums::_shader_variable_base_type_texture)
					continue;

				blam::predicted_resources_add_resource(shader_tag->predicted_resources, 
					Enums::_predicted_resource_bitmap,
					parameter.bitmap_value.bitmap.tag_index,
					parameter.value.bitmap.bitmap_index);
			}

			return true;
		}
		bool BuildCacheFilePredicatedResourcesYelo()
		{
			bool success = true;

			TagGroups::s_tag_iterator tag_iter;
			blam::tag_iterator_new(tag_iter);

			printf_s("building predicted resources for custom tag instances...");
			datum_index tag_index;
			while (success && !(tag_index = blam::tag_iterator_next(tag_iter)).IsNull())
			{
				const s_tag_instance* instance = TagGroups::TagInstances()[tag_index];

				switch (instance->parent_group_tags[1])
				{
				case TagGroups::s_object_definition::k_group_tag:
					success &= BuildObjectPredicatedResourcesYelo(tag_index);
					break;
				}

				switch (instance->parent_group_tags[0])
				{
				case TagGroups::s_object_definition::k_group_tag:
					success &= BuildObjectPredicatedResourcesYelo(tag_index);
					break;
				}

				switch (instance->group_tag)
				{
				case TagGroups::s_shader_postprocess_generic::k_group_tag:
					success &= BuildShaderPostprocessGenericPredictedResources(tag_index);
					break;
				}
			}
			if (success)
				printf_s("done\n");
			else
				printf_s("FAILED!\n");

			return success;
		}

		bool BuildCacheFileWriteHeaderPreprocess(s_cache_header& cache_header)
		{
			bool success = true;

			auto& ych = BuildCacheFileYeloCacheHeader();

			printf_s("Checking tags for OS-modified game-state requirements...\n");
			bool uses_game_state_upgrades = TagGroups::YeloToolCheckTagsForGameStateUpgradeRequirements();
			if (ych.flags.uses_game_state_upgrades = uses_game_state_upgrades)
				printf_s("\tFound tag(s) which require OS-modified game-states to (fully) function\n");

			//////////////////////////////////////////////////////////////////////////
			// Add the definitions for debugging purposes
			void* buffer;
			success = CheApe::GetCacheFileResourceBuffer(buffer,
				ych.cheape_definitions.decompressed_size, ych.cheape_definitions.size);

			printf_s("writing CheApe debug symbols...");
			if (success)
			{
				blam::build_cache_file_add_resource(buffer, ych.cheape_definitions.size,
					CAST_PTR(int32*, &ych.cheape_definitions.offset), false);

				delete buffer;
				printf_s("done\n");
			}
			else
				printf_s("FAILED!\n");
			//////////////////////////////////////////////////////////////////////////

			// Add the build date of the OS HEK tools
			CheApeApi_GetPchBuildDateA(_countof(ych.cheape_definitions.build_string), ych.cheape_definitions.build_string);

			// copy the yelo header data to the header which will be written to file
			memcpy_s(&cache_header.yelo, sizeof(cache_header.yelo), &ych, sizeof(ych));

			return success;
		}

		c_cache_file_builder_base::c_cache_file_builder_base()
			: m_tag_index_to_cache_tag_index_table()
			, m_current_tag_index( datum_index::null )
		{
			m_tag_index_to_cache_tag_index_table.resize(
				Enums::k_maximum_simultaneous_tag_instances_upgrade, datum_index::null);
		}
		c_cache_file_builder_base::~c_cache_file_builder_base()
		{
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

				m_tag_index_to_cache_tag_index_table[instance.GetAbsoluteIndex()] =
					datum_index::Create(cache_tag_count++, salt);

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