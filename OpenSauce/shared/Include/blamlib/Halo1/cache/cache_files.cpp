/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#include "Common/Precompile.hpp"
#include <blamlib/Halo1/cache/cache_files.hpp>

#include <blamlib/Halo1/cache/cache_files_globals.hpp>
#include <blamlib/Halo1/cache/cache_files_structures.hpp>
#include <blamlib/Halo1/cache/data_file_structures.hpp>
#include <blamlib/Halo1/scenario/scenario_definitions.hpp>
#include <blamlib/Halo1/structures/structure_bsp_definitions.hpp>
#include <YeloLib/Halo1/open_sauce/settings/c_settings_cheape.hpp>

namespace Yelo
{
	namespace Cache
	{
		cstring K_MAP_FILE_EXTENSION = ".map";
		cstring K_MAP_FILES_DIRECTORY = R"(maps\)";

		cstring MapsDirectory()
		{
#if PLATFORM_IS_EDITOR
			return Settings::c_settings_cheape::Profile().GetMapsPath();
#else
			return K_MAP_FILES_DIRECTORY;
#endif
		}

		bool s_cache_header::ValidSignatures() const
		{
			return	header_signature == k_header_signature && 
					footer_signature == k_footer_signature;
		}

		bool s_cache_header::ValidFileSize(int32 max) const
		{
			return size >= 0 && size <= max;
		}

		bool s_cache_header::ValidName() const
		{
			return strnlen_s(name, Enums::k_tag_string_length+1) <= Enums::k_tag_string_length;
		}

		bool s_cache_tag_instance::MatchesGroup(tag group_tag) const
		{
			return this->group_tag == group_tag ||
				// test the hierarchy graph
				parent_groups[0] == group_tag ||
				parent_groups[1] == group_tag;
		}

		bool s_cache_header::ValidForStock() const
		{
			if (!ValidSignatures())
				return false;

			if (!ValidFileSize(Enums::k_max_cache_size))
				return false;

			if (!ValidName())
				return false;

			if (version != k_version)
				return false;

			return true;
		}

		bool s_cache_header::ValidForYelo() const
		{
			if (!ValidSignatures())
				return false;

			if (!ValidFileSize(Enums::k_max_cache_size_upgrade))
				return false;

			if (!ValidName())
				return false;

			if (version != k_version)
				return false;

			return true;
		}
	};

#if PLATFORM_USES_CACHE_FILES
	namespace TagGroups
	{
		bool TagIsInstanceOf(datum_index tag_index, tag group_tag)
		{
			if (!tag_index.IsNull() && tag_index.index < Index()->count)
			{
				Cache::s_cache_tag_instance const& instance = Instances()[tag_index.index];

				return instance.MatchesGroup(group_tag);
			}

			return false;
		}
		void* TagTryAndGetImpl(datum_index tag_index, tag group_tag)
		{
			if (!tag_index.IsNull() && tag_index.index < Index()->count)
			{
				Cache::s_cache_tag_instance const& instance = Instances()[tag_index.index];

				if (instance.MatchesGroup(group_tag))
					return instance.base_address;
			}

			return nullptr;
		}
		void* TagGetUnsafeImpl(datum_index tag_index)
		{
			Cache::s_cache_tag_instance const& instance = Instances()[tag_index.index];

			return instance.base_address;
		}
	};

	namespace blam
	{
		using namespace Cache;

		static s_cache_tag_instance* cache_file_tag_get_instance(datum_index tag_index)
		{
			auto& cache_file_globals = *CacheFileGlobals();
			YELO_ASSERT(cache_file_globals.tags_loaded);

			auto* global_tag_instances = TagGroups::Instances();
			YELO_ASSERT(global_tag_instances);

			auto& cache_tag_header = *TagGroups::Index();
			YELO_ASSERT_DISPLAY(tag_index.index > NONE && tag_index.index < cache_tag_header.count,
				"i don't think %08x is a tag index", tag_index);

			auto& tag_instance = global_tag_instances[tag_index.index];
			if (tag_index.salt != NONE)
			{
				YELO_ASSERT_DISPLAY(tag_instance.handle == tag_index,
					"i don't think %08x is a tag index", tag_index);
			}

			return CAST_QUAL(s_cache_tag_instance*, &tag_instance);
		}

		tag PLATFORM_API tag_get_group_tag(datum_index tag_index)
		{
			return cache_file_tag_get_instance(tag_index)->group_tag;
		}

	#if 0 // Disabled, as Halo1_CE's TagGroups.cpp defines a more suitable implementation
		cstring PLATFORM_API tag_get_name(datum_index tag_index)
		{
			cstring name = cache_file_tag_get_instance(tag_index)->name;

			// non-standard behavior, but I believe some cache protectors NULL the tag name
			return name != nullptr
				? name
				: "<unnamed tag>";
		}
	#endif

		const void* PLATFORM_API tag_get(tag group_tag, datum_index tag_index)
		{
			using TagGroups::group_tag_to_string;

			auto* tag_instance = cache_file_tag_get_instance(tag_index);

			YELO_ASSERT_DISPLAY(tag_instance->MatchesGroup(group_tag),
				"expected tag group '%s' but got '%s' for %08x",
				group_tag_to_string { group_tag }.ToString(),
				group_tag_to_string { tag_instance->group_tag }.ToString(),
				tag_index);

			YELO_ASSERT_DISPLAY(tag_instance->base_address != nullptr,
				"can't get() a tag (%08x) with a base address!",
				tag_index);

			return tag_instance->base_address;
		}

		using namespace TagGroups;

		void cache_file_structure_bsp_unload(TagGroups::scenario_structure_bsp_reference* reference)
		{
			auto& cache_file_globals = *CacheFileGlobals();

			YELO_ASSERT_DISPLAY(false, "this isn't implemented yet");

			// TODO: there is some additional DX9-related logic that happens here for structure_bsp_header
			
			auto* tag_instance = cache_file_tag_get_instance(reference->structure_bsp.tag_index);
			YELO_ASSERT(tag_instance->base_address);
			YELO_ASSERT(tag_instance->group_tag==structure_bsp::k_group_tag);

			tag_instance->base_address = nullptr;
			cache_file_globals.structure_bsp_header = nullptr;
		}

		bool cache_file_structure_bsp_load(TagGroups::scenario_structure_bsp_reference* reference)
		{
			auto& cache_file_globals = *CacheFileGlobals();

			YELO_ASSERT_DISPLAY(false, "this isn't implemented yet");

			bool finished_flag = false;
			Cache::s_cache_file_request_params read_params;
			read_params.finished_flag = &finished_flag;

			cache_file_read_request(reference->cache_offset, reference->bsp_data_size, reference->header, read_params);
			while (!finished_flag)
				;

			cache_file_globals.structure_bsp_header = reference->header;
			// actually asserts with CACHE_FILE_STRUCTURE_BSP_HEADER_SIGNATURE
			YELO_ASSERT(cache_file_globals.structure_bsp_header->signature==structure_bsp::k_group_tag);

			// TODO: there is some additional DX9-related logic that happens here for structure_bsp_header
			
			auto* tag_instance = cache_file_tag_get_instance(reference->structure_bsp.tag_index);
			YELO_ASSERT(!tag_instance->base_address);
			YELO_ASSERT(tag_instance->group_tag==structure_bsp::k_group_tag);

			tag_instance->base_address = cache_file_globals.structure_bsp_header->bsp;

			return false;
		}
	};
#endif
};