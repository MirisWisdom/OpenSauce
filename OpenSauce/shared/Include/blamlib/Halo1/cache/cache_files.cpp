/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#include "Common/Precompile.hpp"
#include <blamlib/Halo1/cache/cache_files.hpp>

#include <blamlib/Halo1/bitmaps/bitmap_group.hpp>
#include <blamlib/Halo1/cache/cache_files_globals.hpp>
#include <blamlib/Halo1/cache/cache_files_structures.hpp>
#include <blamlib/Halo1/cache/data_file_structures.hpp>
#include <blamlib/Halo1/cache/physical_memory_map.hpp>
#include <blamlib/Halo1/interface/hud_messaging_definitions.hpp>
#include <blamlib/Halo1/memory/data.hpp>
#include <blamlib/Halo1/scenario/scenario_definitions.hpp>
#include <blamlib/Halo1/sound/sound_definitions.hpp>
#include <blamlib/Halo1/structures/structure_bsp_definitions.hpp>
#include <blamlib/Halo1/tag_files/tag_files.hpp>
#include <blamlib/Halo1/tag_files/tag_groups.hpp>
#include <YeloLib/Halo1/cache/cache_files_yelo.hpp>

#if PLATFORM_IS_EDITOR
#include <YeloLib/Halo1/open_sauce/settings/c_settings_cheape.hpp>
#endif

namespace Yelo
{
	namespace Cache
	{
		cstring K_MAP_FILE_EXTENSION = ".map";
		cstring K_MAP_FILES_DIRECTORY = R"(maps\)";

		// this bool is actually defined in the engine, but the only reference is in the cache_file's tags load process
		static bool g_dont_load_external_data;

		cstring MapsDirectory()
		{
#if PLATFORM_IS_EDITOR
			return Settings::c_settings_cheape::Profile().GetMapsPath();
#else
			return K_MAP_FILES_DIRECTORY;
#endif
		}

		bool DontLoadExternalData()
		{
			return g_dont_load_external_data;
		}

		bool s_cache_header::ValidSignatures() const
		{
			return	header_signature == k_header_signature && 
					footer_signature == k_footer_signature;
		}

		bool s_cache_header::ValidFileSize(int32 max) const
		{
			return file_length >= 0 && file_length <= max;
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

		int16 s_cache_file_globals::FindMapFileIndexByName(cstring scenario_name)
		{
			for (int16 map_file_index = 0; map_file_index < NUMBEROF(map_files); map_file_index++)
			{
				if (!_stricmp(scenario_name, map_files[map_file_index].header.name))
					return map_file_index;
			}

			return NONE;
		}

		bool s_cache_file_globals::OpenMapFileOpen(cstring scenario_name, s_cache_header* header)
		{
			YELO_ASSERT(scenario_name);
			YELO_ASSERT(header);
			YELO_ASSERT(open_map_file_index==NONE);

			int16 map_file_index = FindMapFileIndexByName(scenario_name);
			YELO_ASSERT(map_file_index != NONE);

			memset(Requests(), 0, sizeof(*Requests()) * Enums::k_maximum_simultaneous_cache_requests);
			open_map_file_index = map_file_index;
			memcpy(header, &map_files[map_file_index].header, sizeof(*header));

			return true;
		}

		void s_cache_file_globals::OpenMapFileClose()
		{
			auto* open_map_file = OpenMapFile();
			if (open_map_file != nullptr)
			{
				RequestsWaitAll();
				open_map_file->Close();
				open_map_file_index = NONE;
			}
		}

		void s_cache_file_globals::RequestsWaitAll()
		{
			s_cache_file_request* request = Requests();
			for (rsize_t x = Enums::k_maximum_simultaneous_cache_requests; x > 0; x--, request++)
			{
				const bool& request_is_reading = request->reading;

				while(request_is_reading)
					Sleep(0);
			}
		}
	};

#if PLATFORM_USES_CACHE_FILES
	namespace TagGroups
	{
		// defined by the implementing shell
		Cache::s_cache_tag_instance** GlobalTagInstancesReference();
#define blam_global_tag_instances	*(TagGroups::GlobalTagInstancesReference())

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

		void PLATFORM_API cache_file_geometry_cache_for_models_open(Cache::s_cache_tag_header* tag_header)
		{
			YELO_ASSERT_DISPLAY(false, "this isn't implemented yet");
		}
		void PLATFORM_API cache_file_geometry_cache_for_models_close()
		{
			YELO_ASSERT_DISPLAY(false, "this isn't implemented yet");
		}
		void PLATFORM_API cache_file_geometry_cache_for_bsp_open(TagGroups::structure_bsp_header* bsp_header)
		{
			YELO_ASSERT_DISPLAY(false, "this isn't implemented yet");
		}
		void PLATFORM_API cache_file_geometry_cache_for_bsp_close(TagGroups::structure_bsp_header* bsp_header)
		{
			YELO_ASSERT_DISPLAY(false, "this isn't implemented yet");
		}

		bool PLATFORM_API cache_file_header_verify(Cache::s_cache_header* header, cstring scenario_name, bool critical)
		{
			YELO_ASSERT_DISPLAY(false, "this isn't implemented yet");

			return false;
		}

		using namespace TagGroups;

		static void cache_file_data_load_sound_resources(s_cache_tag_instance* tag_instance, s_cache_file_data_load_state& state)
		{
			byte* data_base_address = state.ReadExternalData(tag_instance, Enums::_data_file_reference_sounds);

			YELO_ASSERT_DISPLAY(false, "this isn't implemented yet");
			auto* sound_definition = tag_instance->Definition<TagGroups::sound_definition>();
		}

		static void cache_file_data_load_bitmap_resources(s_cache_tag_instance* tag_instance, s_cache_file_data_load_state& state)
		{
			byte* data_base_address = state.ReadExternalData(tag_instance, Enums::_data_file_reference_bitmaps);

			YELO_ASSERT_DISPLAY(false, "this isn't implemented yet");
			auto* bitmap_group = tag_instance->Definition<TagGroups::s_bitmap_group>();
		}

		static void cache_file_data_load_multilingual_strings_resources(s_cache_tag_instance* tag_instance, byte* data_base_address)
		{
		}
		static void cache_file_data_load_font_resources(s_cache_tag_instance* tag_instance, byte* data_base_address)
		{
		}
		static void cache_file_data_load_hud_message_text_resources(s_cache_tag_instance* tag_instance, byte* data_base_address)
		{
			auto* hud_state_messages = tag_instance->Definition<TagGroups::hud_state_messages>();
		}
		static void cache_file_data_load_ui_resources(s_cache_tag_instance* tag_instance, s_cache_file_data_load_state& state)
		{
			byte* data_base_address = state.ReadExternalData(tag_instance, Enums::_data_file_reference_locale);

			YELO_ASSERT_DISPLAY(false, "this isn't implemented yet");

			switch (tag_instance->group_tag)
			{
			case 'ustr':
				cache_file_data_load_multilingual_strings_resources(tag_instance, data_base_address);
				break;
			case 'font':
				cache_file_data_load_font_resources(tag_instance, data_base_address);
				break;
			case TagGroups::hud_state_messages::k_group_tag:
				cache_file_data_load_hud_message_text_resources(tag_instance, data_base_address);
				break;

			YELO_ASSERT_CASE_UNREACHABLE();
			}
		}

		static void cache_file_data_load(Cache::s_cache_tag_header* tag_header, s_cache_file_data_load_state& state)
		{
			for (datum_index tag_index : TagGroups::c_tag_iterator::all())
			{
				s_cache_tag_instance* tag_instance = cache_file_tag_get_instance(tag_index);
				if (!tag_instance->bool_in_data_file)
					continue;

				switch (tag_instance->group_tag)
				{
				case TagGroups::sound_definition::k_group_tag:
					cache_file_data_load_sound_resources(tag_instance, state);
					break;

				case TagGroups::s_bitmap_group::k_group_tag:
					cache_file_data_load_bitmap_resources(tag_instance, state);
					break;

				case 'ustr':
				case 'font':
				case TagGroups::hud_state_messages::k_group_tag:
					cache_file_data_load_ui_resources(tag_instance, state);
					break;
				}
			}
		}
		datum_index cache_file_tags_load(cstring scenario_name)
		{
			auto& cache_file_globals = *CacheFileGlobals();
			YELO_ASSERT_DISPLAY(false, "this isn't implemented yet");

			scenario_name = tag_name_strip_path(scenario_name);
			auto* cache_header = &cache_file_globals.cache_header;
			if (cache_file_globals.OpenMapFileOpen(scenario_name, cache_header) ||
				!cache_file_header_verify(cache_header, scenario_name, true))
			{
				return datum_index::null;
			}

			auto* tag_header = CAST_PTR(Cache::s_cache_tag_header*, physical_memory_map_get_tag_cache_address());

			bool read_finished = false;
			s_cache_file_request_params read_params = { &read_finished };
			cache_file_read_request(cache_header->offset_to_index, cache_header->tag_memory_size, tag_header,
				read_params);
			while(!read_finished)
				Sleep(0);

			YELO_ASSERT_DISPLAY(tag_header->signature == Cache::s_cache_tag_header::k_signature,
				"signature is '%s', should be '%s'",
				TagGroups::group_tag_to_string{ tag_header->signature }.ToString(),
				TagGroups::group_tag_to_string{ Cache::s_cache_tag_header::k_signature }.ToString());
			cache_file_globals.tag_index = tag_header;

			blam_global_tag_instances = CAST_PTR(s_cache_tag_instance*, tag_header->tags_address);
			cache_file_globals.tags_loaded = true;

			if (!Cache::DontLoadExternalData())
			{
				auto* scenario = tag_get<TagGroups::scenario>(tag_header->scenario_index);
				auto* hs_syntax_data = CAST_PTR(Memory::s_data_array*, scenario->hs_syntax_data.address);
				// the data array's base address pointer isn't updated when a cache is built, so it's updated here
				if (hs_syntax_data != nullptr) // technically, this should never be NULL (engine doesn't perform check)
				{
					hs_syntax_data->base_address = hs_syntax_data + 1;
				}

				s_cache_file_data_load_state load_state(cache_header, tag_header);
				cache_file_data_load(tag_header, load_state);
			}

			cache_file_geometry_cache_for_models_open(tag_header);

			return tag_header->scenario_index;
		}

		void cache_file_tags_unload()
		{
			auto& cache_file_globals = *CacheFileGlobals();

			cache_file_globals.OpenMapFileClose();
			cache_file_geometry_cache_for_bsp_close(cache_file_globals.structure_bsp_header);
			cache_file_geometry_cache_for_models_close();
			cache_file_globals.tags_loaded = false;

			blam_global_tag_instances = nullptr;
		}

		void cache_file_structure_bsp_unload(TagGroups::scenario_structure_bsp_reference* reference)
		{
			auto& cache_file_globals = *CacheFileGlobals();

			cache_file_geometry_cache_for_bsp_close(cache_file_globals.structure_bsp_header);
			
			auto* tag_instance = cache_file_tag_get_instance(reference->structure_bsp.tag_index);
			YELO_ASSERT(tag_instance->base_address);
			YELO_ASSERT(tag_instance->group_tag==structure_bsp::k_group_tag);

			tag_instance->base_address = nullptr;
			cache_file_globals.structure_bsp_header = nullptr;
		}

		bool cache_file_structure_bsp_load(TagGroups::scenario_structure_bsp_reference* reference)
		{
			auto& cache_file_globals = *CacheFileGlobals();

			bool finished_flag = false;
			Cache::s_cache_file_request_params read_params;
			read_params.finished_flag = &finished_flag;

			cache_file_read_request(reference->cache_offset, reference->bsp_data_size, reference->header, read_params);
			while (!finished_flag)
				;

			cache_file_globals.structure_bsp_header = reference->header;
			// actually asserts with CACHE_FILE_STRUCTURE_BSP_HEADER_SIGNATURE
			YELO_ASSERT(cache_file_globals.structure_bsp_header->signature==structure_bsp::k_group_tag);

			cache_file_geometry_cache_for_bsp_open(cache_file_globals.structure_bsp_header);
			
			auto* tag_instance = cache_file_tag_get_instance(reference->structure_bsp.tag_index);
			YELO_ASSERT(!tag_instance->base_address);
			YELO_ASSERT(tag_instance->group_tag==structure_bsp::k_group_tag);

			tag_instance->base_address = cache_file_globals.structure_bsp_header->bsp;

			return false;
		}
	};
#endif
};