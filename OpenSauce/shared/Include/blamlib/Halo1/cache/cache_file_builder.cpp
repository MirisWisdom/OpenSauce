/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#include "Common/Precompile.hpp"
#include <blamlib/Halo1/cache/cache_file_builder.hpp>
#if PLATFORM_TYPE == PLATFORM_TOOL

#include <blamlib/Halo1/game/game_globals.hpp>
#include <blamlib/Halo1/game/game_globals_definitions.hpp>
#include <blamlib/Halo1/hs/hs_scenario_definitions.hpp>
#include <blamlib/Halo1/main/levels.hpp>
#include <blamlib/Halo1/scenario/scenario.hpp>
#include <blamlib/Halo1/scenario/scenario_definitions.hpp>
#include <blamlib/Halo1/tag_files/tag_field_scanner.hpp>
#include <blamlib/Halo1/tag_files/tag_files.hpp>
#include <blamlib/Halo1/tag_files/tag_groups.hpp>

#include <YeloLib/memory/memory_interface_base.hpp>
#include <YeloLib/Halo1/tag_files/string_id_yelo.hpp>

#include <direct.h> // fucking _mkdir

namespace Yelo
{
	namespace Cache
	{
		void s_data_file_globals::Save()
		{
			sounds.Save();
			locale.Save();
			bitmaps.Save();
		}
		void s_data_file_globals::PreprocessForSave()
		{
			sounds.PreprocessForSave();
			locale.PreprocessForSave();
			bitmaps.PreprocessForSave();
		}

		static void FixGameGlobals(datum_index globals_index, Enums::scenario_type scenario_type)
		{
			auto* globals = blam::tag_get<TagGroups::s_game_globals>(globals_index);

			switch(scenario_type)
			{
			case Enums::_scenario_type_main_menu:
				{
					globals->player_info[0].unit.clear();
					globals->materials.resize(0);
					globals->falling_damage.resize(0);
				} // fall through
			case Enums::_scenario_type_campaign:
				{
					globals->cheat_powerups.resize(0);
					globals->weapons_list.resize(0);
					globals->multiplayer_info.resize(0);
				} break;
			}

			// not done in the stock code, but it's unused anyway so fuck it
			globals->playlist_members.resize(0);
		}
		bool ScenarioLoadForCacheBuild(cstring scenario_name, cstring globals_name)
		{
			datum_index scenario_index = blam::tag_load<TagGroups::scenario>(scenario_name, 
				FLAG(Flags::_tag_load_from_file_system_bit));
			datum_index globals_index = blam::tag_load<TagGroups::s_game_globals>(globals_name, 
				FLAG(Flags::_tag_load_from_file_system_bit) | FLAG(Flags::_tag_load_non_resolving_references_bit));

			// the engine code returns true even if the tags fail to load
			if(scenario_index.IsNull() || globals_index.IsNull())
				return true;

			auto* scnr = blam::tag_get<TagGroups::scenario>(scenario_index);
			FixGameGlobals(globals_index, scnr->type);
			blam::tag_load_children(globals_index);

			return true;
		}
	};

	namespace blam
	{
		typedef std::array<datum_index, Enums::k_maximum_simultaneous_tag_instances_upgrade> build_cache_file_tag_handles_t;

		static bool building_single_player_scenario;
		static const unsigned k_cache_file_tag_memory_alignment_bit = Flags::_alignment_32_bit;

		template<typename T>
		static T* stream_blob_to_buffer(T*& stream, const void* blob, size_t blob_size, unsigned alignment_bit = k_cache_file_tag_memory_alignment_bit)
		{
			std::memcpy(stream, blob, blob_size);
			stream = CAST_PTR(byte*, stream) + blob_size;
			stream = Memory::AlignPointer(stream, alignment_bit);
			return stream;
		}

		void* stream_tag_block_to_buffer(tag_block* block, void* stream, uintptr_t stream_base_address, uintptr_t virtual_base_address, tag_reference_name_reference* tag_names);

		static void* stream_tag_block_to_buffer_postprocess_tag_data(void*& return_stream, uintptr_t stream_base_address, uintptr_t virtual_base_address, tag_reference_name_reference* tag_names,
			const TagGroups::c_tag_field_scanner& scanner)
		{
			auto* data = scanner.GetFieldAs<tag_data>();

			if(data->definition == nullptr)
			{	// not exactly what tool's asm does here, but should amount to the same output
				data->address = nullptr;
			}
			else if(!TEST_FLAG(data->definition->flags, Flags::_tag_data_not_streamed_to_cache_bit))
			{
				if(data->size > 0 && data->address != nullptr)
				{
					// record the current stream position as runtime address for the data blob, then write the blob
					void* data_address = Memory::RebasePointer(return_stream, stream_base_address, virtual_base_address);
					stream_blob_to_buffer(return_stream, data->address, data->size);

					data->definition = nullptr;
					data->address = data_address;
				}
				else
				{
					data->address = nullptr;
					data->definition = nullptr;
				}
			}

			return return_stream;
		}
		static void* stream_tag_block_to_buffer_postprocess_tag_block(void*& return_stream, uintptr_t stream_base_address, uintptr_t virtual_base_address, tag_reference_name_reference* tag_names,
			const TagGroups::c_tag_field_scanner& scanner)
		{
			auto* block = scanner.GetFieldAs<tag_block>();

			if(block->count > 0)
			{
				// record the current stream position as runtime address for the block elements, then write the elements
				void* elements_address = Memory::RebasePointer(return_stream, stream_base_address, virtual_base_address);
				return_stream = stream_tag_block_to_buffer(block, return_stream, stream_base_address, virtual_base_address, tag_names);

				block->address = elements_address;
				block->definition = nullptr;
			}
			else
			{
				block->address = nullptr;
				block->definition = nullptr;
			}

			return return_stream;
		}
		static void* stream_tag_block_to_buffer_postprocess_tag_reference(void*& return_stream, uintptr_t stream_base_address, uintptr_t virtual_base_address, tag_reference_name_reference* tag_names,
			const TagGroups::c_tag_field_scanner& scanner)
		{
			auto* reference = scanner.GetFieldAs<tag_reference>();
			int32 absolute_index = reference->tag_index.index;
			YELO_ASSERT( reference->tag_index==datum_index::null || 
						(absolute_index>=0 && 
							(absolute_index<Enums::k_maximum_simultaneous_tag_instances || absolute_index<Enums::k_maximum_simultaneous_tag_instances_upgrade)
						) 
				);

			reference->name = reference->tag_index.IsNull() ? nullptr : tag_names[absolute_index];
			reference->name_length = 0;

			if(scanner.TagFieldIsStringId())
			{
				string_id_yelo* sidy = CAST_PTR(string_id_yelo*, reference);
			}

			return return_stream;
		}
		static void* stream_tag_block_to_buffer(tag_block* block, void* stream, uintptr_t stream_base_address, uintptr_t virtual_base_address, tag_reference_name_reference* tag_names)
		{
			YELO_ASSERT( stream && stream_base_address );
			YELO_ASSERT( virtual_base_address );

			void* return_stream = stream;

			// copy the tag memory, loaded by the tools, verbatim into the cache tag memory stream
			stream_blob_to_buffer(return_stream, block->address, block->count * block->get_element_size());

			for (auto element : *block)
			{
				TagGroups::c_tag_field_scanner scanner(block->definition->fields, 
					// get the the element address in the cache tag memory stream
					CAST_PTR(byte*, stream) + (block->get_element_size() * element.index)
					);

				scanner	.AddFieldType(Enums::_field_data)
						.AddFieldType(Enums::_field_block)
						.AddFieldType(Enums::_field_tag_reference)
						.AddFieldType(Enums::_field_string)			// non-stock code; we scan for tag-strings so we can generate string_ids from them
						;
				while(scanner.Scan())
				{
					switch(scanner.GetTagFieldType())
					{
					case Enums::_field_data:
						stream_tag_block_to_buffer_postprocess_tag_data(return_stream, stream_base_address, virtual_base_address, tag_names, 
							scanner);
						break;

					case Enums::_field_block:
						stream_tag_block_to_buffer_postprocess_tag_block(return_stream, stream_base_address, virtual_base_address, tag_names, 
							scanner);
						break;

					case Enums::_field_tag_reference:
						stream_tag_block_to_buffer_postprocess_tag_reference(return_stream, stream_base_address, virtual_base_address, tag_names, 
							scanner);
						break;

					case Enums::_field_string: // non-stock code
						{
						}
						break;
					}
				}
			}

			return return_stream;
		}

		size_t stream_tag_to_buffer(datum_index tag_index, void* stream, size_t& return_stream_offset, uintptr_t virtual_base_address, tag_reference_name_reference* tag_names)
		{
			tag_block* block = tag_get_root_block(tag_index);

			void* return_stream = stream_tag_block_to_buffer(block, stream, CAST_PTR(uintptr_t, stream), virtual_base_address, tag_names);

			return CAST_PTR(uintptr_t, return_stream) - CAST_PTR(uintptr_t, stream);
		}

		using namespace Cache;

		static API_FUNC_NAKED bool PLATFORM_API build_cache_file_cull_tags()
		{
			static const uintptr_t FUNCTION = 0x453260;

			__asm	jmp	FUNCTION
		}

		static API_FUNC_NAKED bool PLATFORM_API build_cache_file_last_minute_changes()
		{
			static const uintptr_t FUNCTION = 0x454280;

			__asm	jmp	FUNCTION
		}

		static API_FUNC_NAKED bool PLATFORM_API build_object_predicted_resources()
		{
			static const uintptr_t FUNCTION = 0x453680;

			__asm	jmp	FUNCTION
		}
		static API_FUNC_NAKED bool PLATFORM_API build_first_person_weapon_predicted_resources()
		{
			static const uintptr_t FUNCTION = 0x453700;

			__asm	jmp	FUNCTION
		}
		static API_FUNC_NAKED bool PLATFORM_API build_scenario_predicted_resources()
		{
			static const uintptr_t FUNCTION = 0x4537A0;

			__asm	jmp	FUNCTION
		}
		static API_FUNC_NAKED bool PLATFORM_API build_structure_bsp_predicted_resources()
		{
			static const uintptr_t FUNCTION = 0x453860;

			__asm	jmp	FUNCTION
		}
		static bool build_cache_file_predicted_resources()
		{
			return
				build_object_predicted_resources() &&
				build_first_person_weapon_predicted_resources() &&
				build_scenario_predicted_resources() &&
				build_structure_bsp_predicted_resources();
		}

		static API_FUNC_NAKED bool PLATFORM_API build_cache_file_add_tags(
			s_cache_header& cache_header, void* scratch, build_cache_file_tag_handles_t& tag_indexes, int32 largest_structure_bsp_size)
		{
			static const uintptr_t FUNCTION = 0x454D40;

			__asm	jmp	FUNCTION
		}
		static API_FUNC_NAKED bool build_cache_file_add_structure_bsps(
			void* scratch, build_cache_file_tag_handles_t& tag_indexes, int32& largest_structure_bsp_size)
		{
			static const uintptr_t FUNCTION = 0x454B70;

			API_FUNC_NAKED_START()
				push	largest_structure_bsp_size
				push	tag_indexes
				mov		eax, scratch
				call	FUNCTION
				add		esp, 4 * 2
			API_FUNC_NAKED_END(3)
		}
		static bool build_cache_file_add_tags(build_cache_file_tag_handles_t& tag_indexes,
			s_cache_header& cache_header, void* scratch)
		{
			int32 largest_structure_bsp_size = 0;
			return
				build_cache_file_add_tags(cache_header, nullptr, tag_indexes, 0) &&
				build_cache_file_add_structure_bsps(scratch, tag_indexes, largest_structure_bsp_size) &&
				build_cache_file_add_tags(cache_header, scratch, tag_indexes, largest_structure_bsp_size);
		}

		static bool build_cache_file_write_header_and_compress(s_cache_header& cache_header)
		{
			cache_header.file_length = build_cache_file_size();
			cache_header.crc = build_cache_file_checksum();

			int32 maximum_cache_file_length = Enums::k_max_cache_size_upgrade;
			switch (cache_header.cache_type)
			{
			case Enums::_cache_file_type_campaign:		maximum_cache_file_length = Enums::k_max_cache_size; break;
			case Enums::_cache_file_type_multiplayer:	maximum_cache_file_length = 0x8000000; break;
			case Enums::_cache_file_type_main_menu:		maximum_cache_file_length = 0x2300000; break;

			YELO_ASSERT_CASE_UNREACHABLE();
			}

			const double k_byte_to_megabyte_fraction = 1.0 / Enums::k_mega;

			if (cache_header.file_length > maximum_cache_file_length)
			{
				printf_s("cache file %.2fM too large (should be %.2fM but was %.2fM)\n",
					CAST(float, k_byte_to_megabyte_fraction * (cache_header.file_length-maximum_cache_file_length)),
					CAST(float, k_byte_to_megabyte_fraction * maximum_cache_file_length),
					CAST(float, k_byte_to_megabyte_fraction * cache_header.file_length));
				return false;
			}

			printf_s("compressing %.2fM...",
				CAST(float, k_byte_to_megabyte_fraction * cache_header.file_length));
			if (!build_cache_file_end(cache_header))
			{
				printf_s("FAILED!\n");
				return false;
			}
			else
				printf_s("done\n");

			return true;
		}

		void build_cache_file_for_scenario(cstring scenario_path)
		{
			cstring scenario_name = tag_name_strip_path(scenario_path);

			void* scratch = YELO_MALLOC(0x2000000, false);
			YELO_ASSERT(scratch);
			YELO_ASSERT(!((uint32)scratch & 31));

			building_single_player_scenario =
				_stricmp(scenario_name, "ui")==0 || main_get_campaign_level_from_name(scenario_name) != NONE;

			_mkdir(K_MAP_FILES_DIRECTORY);
			memset(scratch, 0x11, 4); // yeah, IDK why they used these parameters. sizeof typo?

			do {
				if (!ScenarioLoadForCacheBuild(scenario_path, Scenario::K_GAME_GLOBALS_TAG_NAME) ||
					!scenario_load_all_structure_bsps() ||
					!errors_handle())
				{
					printf_s("### FAILED TO LOAD SCENARIO.\n\n");
					break;
				}

				if (!hs_scenario_postprocess(true))
				{
					printf_s("### FAILED TO COMPILE SCRIPTS FOR SCENARIO.\n\n");
					break;
				}

				s_cache_header cache_header;
				memset(&cache_header, 0, sizeof(cache_header));
				cache_header.header_signature = s_cache_header::k_header_signature;
				cache_header.footer_signature = s_cache_header::k_footer_signature;
				cache_header.version = s_cache_header::k_version;

				strncpy_s(cache_header.name, scenario_name, Enums::k_tag_string_length);
				strncpy_s(cache_header.build_string, "01.00.00.0609", Enums::k_tag_string_length);
				cache_header.cache_type = CAST(Enums::cache_file_type, global_scenario_get()->type);

				build_cache_file_tag_handles_t tag_indexes;
				tag_indexes.fill(datum_index::null); // NOTE: engine actually fills with 0

				if (!build_cache_file_begin(scenario_name) ||
					!build_cache_file_cull_tags() ||
					!build_cache_file_last_minute_changes() ||
					!build_cache_file_predicted_resources() ||
					!build_cache_file_add_tags(tag_indexes, cache_header, scratch) ||
					!build_cache_file_write_header_and_compress(cache_header))
				{
					build_cache_file_cancel();
					printf_s("### FAILED TO BUILD CACHE FILE.\n\n");
					break;
				}

				printf_s("successfully built cache file.\n");

			} while (false);

			YELO_FREE(scratch);
		}
	};
};
#endif