/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#include "Common/Precompile.hpp"
#include <blamlib/Halo1/cache/cache_file_builder.hpp>
#if PLATFORM_TYPE == PLATFORM_TOOL

#include <blamlib/Halo1/bitmaps/bitmap_group.hpp>
#include <blamlib/Halo1/game/game_globals.hpp>
#include <blamlib/Halo1/game/game_globals_definitions.hpp>
#include <blamlib/Halo1/hs/hs_scenario_definitions.hpp>
#include <blamlib/Halo1/interface/hud_messaging_definitions.hpp>
#include <blamlib/Halo1/main/levels.hpp>
#include <blamlib/Halo1/scenario/scenario.hpp>
#include <blamlib/Halo1/scenario/scenario_definitions.hpp>
#include <blamlib/Halo1/sound/sound_definitions.hpp>
#include <blamlib/Halo1/structures/structure_bsp_definitions.hpp>
#include <blamlib/Halo1/tag_files/tag_field_scanner.hpp>
#include <blamlib/Halo1/tag_files/tag_files.hpp>
#include <blamlib/Halo1/tag_files/tag_groups.hpp>

#include <YeloLib/memory/memory_interface_base.hpp>
#include <YeloLib/Halo1/cache/cache_file_builder_yelo.hpp>
#include <YeloLib/Halo1/open_sauce/project_yellow_scenario_definitions.hpp>
#include <YeloLib/Halo1/tag_files/string_id_yelo.hpp>
#include <YeloLib/Halo1/tag_files/tag_group_memory.hpp>

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
		static void FixScenarioYelo(datum_index project_yellow_index)
		{
			if (project_yellow_index.IsNull())
				return;

			auto* yelo = blam::tag_get<TagGroups::project_yellow>(project_yellow_index);

			bool has_globals_override = yelo->LoadGameGlobalsOverride();
		}

		static void InitializeYeloCacheHeaderForNewScenario(TagGroups::scenario* scnr, datum_index project_yellow_index)
		{
			if (project_yellow_index.IsNull())
				return;

			auto* yelo = blam::tag_get<TagGroups::project_yellow>(project_yellow_index);

			// If the scenario's yelo specifies build info, update the yelo header with that info, else use the defaults
			if (yelo->build_info.Count > 0)
			{
				const TagGroups::s_project_yellow_scenario_build_info& build_info = yelo->build_info[0];

				BuildCacheFileYeloCacheHeader().InitializeBuildInfo(build_info);
			}
			else
				BuildCacheFileYeloCacheHeader().InitializeBuildInfo();
		}

		bool ScenarioLoadForCacheBuild(cstring scenario_name)
		{
			// NOTE: the Scenario::GetYelo(), etc APIs won't be valid until the call to scenario_load
			
			datum_index scenario_index = blam::tag_load<TagGroups::scenario>(scenario_name, 
				FLAG(Flags::_tag_load_from_file_system_bit));
			// the engine code returns true even if the tags fail to load
			if (scenario_index.IsNull())
				return true;

			auto* scenario = blam::tag_get<TagGroups::scenario>(scenario_index);

			// perform the initial project_yellow fixups and associated changes here
			if (BuildCacheFileForYelo())
			{
				const tag_reference& yelo_reference = scenario->GetYeloReferenceHack();

				FixScenarioYelo(yelo_reference.tag_index);
				InitializeYeloCacheHeaderForNewScenario(scenario, yelo_reference.tag_index);
			}

			// they use non-resolving references as FixGameGlobals clears select tag blocks for certain scenario types,
			// thus any references the blocks have wouldn't need to be used
			datum_index globals_index = blam::tag_load<TagGroups::s_game_globals>(Scenario::K_GAME_GLOBALS_TAG_NAME, 
				FLAG(Flags::_tag_load_from_file_system_bit) | FLAG(Flags::_tag_load_non_resolving_references_bit));
			// the engine code returns true even if the tags fail to load
			if (globals_index.IsNull())
				return true;

			FixGameGlobals(globals_index, scenario->type);
			// the child of the globals tags can now be loaded, as all unnecessary references have been cleared
			blam::tag_load_children(globals_index);

			return blam::scenario_load(scenario_name);
		}
	};

	namespace blam
	{
		typedef std::array<char*, Enums::k_maximum_simultaneous_tag_instances_upgrade> build_cache_file_tag_names_t;

		static bool building_single_player_scenario;
		static const size_t k_build_cache_file_scratch_buffer_size = 40 * Enums::k_mega;
		static const unsigned k_cache_file_tag_memory_alignment_bit = Flags::_alignment_32_bit;
		static const double k_byte_to_megabyte_fraction = 1.0 / Enums::k_mega;

		static const size_t k_cache_file_minimum_fixed_size =
			sizeof(TagGroups::structure_bsp_header) +
			Enums::k_tag_allocation_size_upgrade;
		BOOST_STATIC_ASSERT(k_build_cache_file_scratch_buffer_size >= k_cache_file_minimum_fixed_size);


		// "return_stream" - the new stream pointer, positioned after the data which was written to it/"stream" before the stream operation started

		template<typename T>
		static T* stream_blob_to_buffer(T*& stream, const void* blob, size_t blob_size,
			unsigned alignment_bit = k_cache_file_tag_memory_alignment_bit)
		{
			std::memcpy(stream, blob, blob_size);
			stream = CAST_PTR(byte*, stream) + blob_size;
			stream = Memory::AlignPointer(stream, alignment_bit);
			return stream;
		}

		void* stream_tag_block_to_buffer(tag_block* block,
			void* stream, uintptr_t stream_base_address, uintptr_t virtual_base_address,
			build_cache_file_tag_names_t& tag_names);

		static void* stream_tag_block_to_buffer_postprocess_tag_data(
			void*& return_stream, uintptr_t stream_base_address, uintptr_t virtual_base_address,
			build_cache_file_tag_names_t& tag_names,
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
		static void* stream_tag_block_to_buffer_postprocess_tag_block(
			void*& return_stream, uintptr_t stream_base_address, uintptr_t virtual_base_address,
			build_cache_file_tag_names_t& tag_names,
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
		static void* stream_tag_block_to_buffer_postprocess_tag_reference(
			void*& return_stream, uintptr_t stream_base_address, uintptr_t virtual_base_address,
			build_cache_file_tag_names_t& tag_names,
			const TagGroups::c_tag_field_scanner& scanner)
		{
			auto* reference = scanner.GetFieldAs<tag_reference>();
			int32 absolute_index = reference->tag_index.index;
			YELO_ASSERT( reference->tag_index==datum_index::null || 
						(absolute_index>=0 && 
							(absolute_index<Enums::k_maximum_simultaneous_tag_instances || absolute_index<Enums::k_maximum_simultaneous_tag_instances_upgrade)
						) 
				);

			reference->name = reference->tag_index.IsNull()
				? nullptr
				: tag_names[absolute_index];
			reference->name_length = 0;

			if(scanner.TagFieldIsStringId())
			{
				string_id_yelo* sidy = CAST_PTR(string_id_yelo*, reference);
			}

			return return_stream;
		}
		static void* stream_tag_block_to_buffer(tag_block* block,
			void* stream, uintptr_t stream_base_address, uintptr_t virtual_base_address,
			build_cache_file_tag_names_t& tag_names)
		{
			YELO_ASSERT( stream && stream_base_address );
			YELO_ASSERT( virtual_base_address );

			void* elements_stream = stream;

			// copy the tag memory, loaded by the tools, verbatim into the cache tag memory stream
			// return_stream will equal the starting address of child data (ie, blocks and tag_data)
			void* return_stream = stream_blob_to_buffer(elements_stream, block->address, block->count * block->get_element_size());

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

		void* stream_tag_to_buffer(datum_index tag_index,
			void* stream, size_t& return_tag_size, uintptr_t virtual_base_address,
			build_cache_file_tag_names_t& tag_names)
		{
			tag_block* block = tag_get_root_block(tag_index);

			void* return_stream = stream_tag_block_to_buffer(block,
				stream, CAST_PTR(uintptr_t, stream), virtual_base_address,
				tag_names);

			return_tag_size = CAST_PTR(uintptr_t, return_stream) - CAST_PTR(uintptr_t, stream);
			return return_stream;
		}

		static size_t stream_structure_bsp_to_buffer(TagGroups::scenario_structure_bsp_reference& reference,
			void* stream, build_cache_file_tag_names_t& tag_names, uintptr_t virtual_base_address)
		{
			using namespace TagGroups;

			YELO_ASSERT(!reference.structure_bsp.tag_index.IsNull());

			auto* bsp_header = CAST_PTR(structure_bsp_header*, stream);
			auto* bsp_stream = CAST_PTR(structure_bsp*, AlignPointer(bsp_header+1, k_cache_file_tag_memory_alignment_bit));

			memset(bsp_header, 0, sizeof(*bsp_header));
			bsp_header->signature = structure_bsp::k_group_tag;
			bsp_header->bsp = RebasePointer(bsp_stream, CAST_PTR(uintptr_t, bsp_header), virtual_base_address);

			size_t bsp_tag_size;
			stream_tag_to_buffer(reference.structure_bsp.tag_index,
				bsp_stream, bsp_tag_size, virtual_base_address, tag_names);

			size_t bsp_stream_size;
			bsp_stream_size = AlignValue(CAST_PTR(uintptr_t, bsp_stream) + bsp_tag_size, k_cache_file_tag_memory_alignment_bit);
			bsp_stream_size -= CAST_PTR(uintptr_t, bsp_header);
			bsp_stream_size = AlignValue(bsp_stream_size, Enums::k_cache_file_page_alignment_bit);

			return bsp_stream_size;
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

		static API_FUNC_NAKED void* PLATFORM_API build_cache_file_add_model_vertices_and_triangles(
			s_cache_tag_header* cache_tag_header, void* scratch)
		{
			static const uintptr_t FUNCTION = 0x454360;

			__asm	jmp	FUNCTION
		}
		static void build_cache_file_add_model_vertices_and_triangles_resources(s_cache_tag_header* cache_tag_header)
		{
			printf_s("streaming model vertex and index buffers...");
			void* vertex_y_index_buffer = YELO_MALLOC(Enums::k_max_cache_vertex_y_index_buffer_size_upgrade, false);
			void* vertex_y_index_buffer_end =
				build_cache_file_add_model_vertices_and_triangles(cache_tag_header, vertex_y_index_buffer);
			cache_tag_header->model_data_size =
				CAST_PTR(byte*, vertex_y_index_buffer_end) - CAST_PTR(byte*, vertex_y_index_buffer);
			printf_s("done\n");

			printf_s("writing vertex and index buffers...");
			if (!build_cache_file_add_resource(vertex_y_index_buffer, cache_tag_header->model_data_size,
				&cache_tag_header->vertices.offset))
				printf_s("FAILED!\n");
			else
			{
				printf_s("done %3.2fM\n",
					CAST(float, k_byte_to_megabyte_fraction * cache_tag_header->model_data_size));
			}

			YELO_FREE(vertex_y_index_buffer);
		}
		static char* build_cache_file_add_tag_to_header(datum_index tag_index,
			uintptr_t stream_base_address,
			s_cache_tag_instance* tag_instances, int32 tag_instance_index,
			char* tag_name, build_cache_file_tag_names_t& tag_names)
		{
			char* tag_name_address = RebasePointer(tag_name, stream_base_address, Enums::k_tag_base_address);
			int32 absolute_tag_index = tag_index.index;

			if (stream_base_address != NULL)
			{
				auto* tag_instance = &tag_instances[tag_instance_index];
				tag_group* group = TagGroups::TagGetGroup(tag_index);
				tag_instance->group_tag = group->group_tag;
				tag_instance->parent_groups[0] = group->parent_group_tag;
				tag_instance->parent_groups[1] = group->parent_group_tag == NONE
					? NONE
					: tag_group_get(group->parent_group_tag)->group_tag;

				YELO_ASSERT_DISPLAY(tag_names[absolute_tag_index] != nullptr,
					"the quarter isn't under any of the cups you fuckin cheater i kill you");

				tag_instance->name = tag_name_address;
				strcpy(tag_name, tag_get_name(tag_index));
			}
			else
			{
				const int32 MAXIMUM_SIMULTANEOUS_TAG_INSTANCES = BuildCacheFileForYelo()
					? Enums::k_maximum_simultaneous_tag_instances_upgrade
					: Enums::k_maximum_simultaneous_tag_instances;
				YELO_ASSERT(absolute_tag_index >= 0 && absolute_tag_index < MAXIMUM_SIMULTANEOUS_TAG_INSTANCES);

				tag_names[absolute_tag_index] = tag_name_address;
			}

			return tag_name + strlen(tag_get_name(tag_index)) + 1;
		}
		static void* build_cache_file_add_tag_to_stream(datum_index tag_index,
			uintptr_t stream_base_address,
			s_cache_tag_instance* tag_instances, int32 tag_instance_index,
			void* stream, build_cache_file_tag_names_t& tag_names)
		{
			YELO_ASSERT_DISPLAY(false, "this isn't implemented yet");
			auto* tag_instance = &tag_instances[tag_instance_index];
			tag_instance->bool_in_data_file = false;

			switch (tag_instance->group_tag)
			{
			case TagGroups::sound_definition::k_group_tag:
				goto default_case;
				break;

			case TagGroups::s_bitmap_group::k_group_tag:
				goto default_case;
				break;

			case 'ustr':
			case 'font':
			case TagGroups::hud_state_messages::k_group_tag:
				goto default_case;
				break;

			case TagGroups::scenario::k_group_tag:
				goto default_case;
				break;

			case TagGroups::structure_bsp::k_group_tag:
				tag_instance->base_address = nullptr;
				break;

			default:
default_case:
				size_t tag_stream_size;
				void* tag_base_address = stream_tag_to_buffer(tag_index,
					stream, tag_stream_size, Enums::k_tag_base_address,
					tag_names);
				tag_instance->base_address = tag_base_address;
				stream = tag_base_address;
				break;
			}

			return stream;
		}
		static bool PLATFORM_API build_cache_file_add_tags_impl(
			s_cache_header& cache_header, void* scratch, build_cache_file_tag_names_t& tag_names, int32 largest_structure_bsp_size)
		{
			enum {
				_add_tags_phase_calculate_tag_header_size,
				_add_tags_phase_writing_tags,
			};
			_enum phase = scratch == nullptr
				? _add_tags_phase_calculate_tag_header_size
				: _add_tags_phase_writing_tags;

			bool success = true;

			// the engine populates these pointers, and checks that scratch != NULL whenever it wants to write to them
			auto* cache_tag_header = CAST_PTR(s_cache_tag_header*, scratch);
			auto* cache_tag_instances = CAST_PTR(s_cache_tag_instance*, cache_tag_header + 1);
			cache_tag_instances = AlignPointer(cache_tag_instances, k_cache_file_tag_memory_alignment_bit);

			int32 tag_count = 0;
			// figure out how many tags there are, and zero the instances when writing their headers
			for (datum_index tag_index : TagGroups::c_tag_iterator::all())
			{
				if (phase == _add_tags_phase_writing_tags)
				{
					memset(&cache_tag_instances[tag_count], 0, sizeof(s_cache_tag_instance));
				}

				tag_count++;
			}

			auto* cache_tag_names = CAST_PTR(char*, cache_tag_instances + tag_count);
			cache_tag_names = AlignPointer(cache_tag_names, k_cache_file_tag_memory_alignment_bit);

			int32 index;
			char* next_cache_tag_name = cache_tag_names;
			// figure out the size of the tag names buffer, and populate the instance fields when writing their heads
			for (datum_index tag_index : TagGroups::c_tag_iterator::all())
			{
				YELO_ASSERT(index<tag_count);

				next_cache_tag_name = build_cache_file_add_tag_to_header(tag_index,
					CAST_PTR(uintptr_t, cache_tag_header), cache_tag_instances, index, next_cache_tag_name, tag_names);

				index++;
			}
			YELO_ASSERT(index==tag_count);

			if (phase == _add_tags_phase_calculate_tag_header_size)
				return success;

			// everything after here is _add_tags_phase_writing_tags
			
			// TODO: need to open the data files here
			
			// tags are streamed after the tag names
			void* cache_tags_base_address = AlignPointer(next_cache_tag_name, k_cache_file_tag_memory_alignment_bit);

			memset(cache_tag_header, 0, sizeof(*cache_tag_header));
			cache_tag_header->tags_address = cache_tag_instances;
			cache_tag_header->count = tag_count;
			cache_tag_header->checksum = tag_groups_checksum();
			cache_tag_header->signature = s_cache_tag_header::k_signature;
			cache_tag_header->scenario_index = Scenario::ScenarioIndex();

			ptrdiff_t tag_headers_y_name_size = CAST_PTR(byte*, cache_tags_base_address) - CAST_PTR(byte*, cache_tag_header);
			printf_s("tag headers and names are %3.2fM\n",
				CAST(float, k_byte_to_megabyte_fraction * tag_headers_y_name_size));

			// NOTE: engine actually writes the model resources AFTER it streams the tags to its memory buffer
			// but it writes the tags resources after models anyways, so file data ordering is still maintained
			build_cache_file_add_model_vertices_and_triangles_resources(cache_tag_header);

			printf_s("streaming tags");
			index = 0;
			void* next_cache_tag_base_address = cache_tags_base_address;
			for (datum_index tag_index : TagGroups::c_tag_iterator::all())
			{
				YELO_ASSERT(index<tag_count);

				next_cache_tag_base_address = build_cache_file_add_tag_to_stream(tag_index,
					CAST_PTR(uintptr_t, cache_tag_header), cache_tag_instances, index, next_cache_tag_base_address, tag_names);
			}
			printf_s("done\n");

			printf_s("writing tags...");
			ptrdiff_t cache_tags_size =
				CAST_PTR(byte*, next_cache_tag_base_address) - CAST_PTR(byte*, cache_tag_header);
			if (!build_cache_file_add_resource(cache_tag_header, cache_tags_size,
				&cache_header.offset_to_index))
			{
				success = false;
				printf_s("FAILED!\n");
			}
			else
			{
				printf_s("done (%d tags for %3.2fM)\n",
					tag_count,
					CAST(float, k_byte_to_megabyte_fraction * cache_tags_size));
			}

			// TODO: need to close data files here

			return success;
		}
		static bool build_cache_file_add_structure_bsps_impl(
			void* stream, build_cache_file_tag_names_t& tag_names, _Out_ int32& out_largest_structure_bsp_size)
		{
			out_largest_structure_bsp_size = 0;
			auto* scenario = global_scenario_get();

			if (scenario->structure_bsps.Count == 0)
				return true;

			const uintptr_t k_tag_cache_max_address = BuildCacheFileForYelo()
				? Enums::k_tag_max_address_upgrade
				: Enums::k_tag_max_address;

			bool success = true;
			for (auto& bsp_reference : scenario->structure_bsps)
			{
				// this is how the engine is setup. first call is used to figure out the bsp size
				size_t bsp_stream_size = stream_structure_bsp_to_buffer(bsp_reference, stream, tag_names, NULL);

				// with the size of the bsp as it will appear in the tag cache, we can figure out the virtual address and actually build a valid stream
				uintptr_t bsp_virtual_base_address = k_tag_cache_max_address - bsp_stream_size;
				stream_structure_bsp_to_buffer(bsp_reference, stream, tag_names, bsp_virtual_base_address);

				cstring bsp_tag_name = tag_get_name(bsp_reference.structure_bsp.tag_index);
				printf_s("structure bsp '%s' is %3.2fM\n",
					bsp_tag_name,
					CAST(float, k_byte_to_megabyte_fraction * bsp_stream_size));

				if (!build_cache_file_add_resource(stream,
					bsp_reference.bsp_data_size = CAST(int32, bsp_stream_size),
					&bsp_reference.cache_offset))
				{
					success = false;
					printf_s("failed to write the structure-bsp '%s' to the cache file",
						bsp_tag_name);
					// yes, continue. the engine code continued even when one of the bsps failed to write
					continue;
				}
				bsp_reference.header = CAST_PTR(TagGroups::structure_bsp_header*, bsp_virtual_base_address);

				// if this bsp's size is the biggest so far, update the out parameter
				if (out_largest_structure_bsp_size < bsp_reference.bsp_data_size)
					out_largest_structure_bsp_size = bsp_reference.bsp_data_size;
			}

			return success;
		}

		static API_FUNC_NAKED bool PLATFORM_API build_cache_file_add_tags(
			s_cache_header& cache_header, void* scratch, build_cache_file_tag_names_t& tag_names, int32 largest_structure_bsp_size)
		{
			static const uintptr_t FUNCTION = 0x454D40;

			__asm	jmp	FUNCTION
		}
		static API_FUNC_NAKED bool build_cache_file_add_structure_bsps(
			void* scratch, build_cache_file_tag_names_t& tag_names, int32& largest_structure_bsp_size)
		{
			static const uintptr_t FUNCTION = 0x454B70;

			API_FUNC_NAKED_START()
				push	largest_structure_bsp_size
				push	tag_names
				mov		eax, scratch
				call	FUNCTION
				add		esp, 4 * 2
			API_FUNC_NAKED_END(3)
		}
		static bool build_cache_file_add_tags(build_cache_file_tag_names_t& tag_indexes,
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
			if (!BuildCacheFileForYelo())
			{
				switch (cache_header.cache_type)
				{
				case Enums::_cache_file_type_campaign:		maximum_cache_file_length = Enums::k_max_cache_size; break;
				case Enums::_cache_file_type_multiplayer:	maximum_cache_file_length = 0x8000000; break;
				case Enums::_cache_file_type_main_menu:		maximum_cache_file_length = 0x2300000; break;

				YELO_ASSERT_CASE_UNREACHABLE();
				}
			}

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

		void build_cache_file_for_scenario(cstring scenario_path,
			byte_flags begin_flags)
		{
			cstring scenario_name = tag_name_strip_path(scenario_path);

			void* scratch = YELO_MALLOC(k_build_cache_file_scratch_buffer_size, false);
			YELO_ASSERT(scratch);
			YELO_ASSERT(!((uintptr_t)scratch & 31));

			building_single_player_scenario =
				_stricmp(scenario_name, "ui")==0 || main_get_campaign_level_from_name(scenario_name) != NONE;

			_mkdir(K_MAP_FILES_DIRECTORY);
			memset(scratch, 0x11, 4); // yeah, IDK why they used these parameters. sizeof typo?

			do {
				if (!ScenarioLoadForCacheBuild(scenario_path) ||
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

				build_cache_file_tag_names_t tag_names;
				tag_names.fill(nullptr);

				if (!BuildCacheFilePreprocessTagsForRuntime() ||
					!build_cache_file_begin(scenario_name, begin_flags) ||
					!build_cache_file_cull_tags() ||
					!BuildCacheFileCullTagsYelo() ||
					!build_cache_file_last_minute_changes() ||
					!build_cache_file_predicted_resources() ||
					!BuildCacheFilePredicatedResourcesYelo() ||
					!build_cache_file_add_tags(tag_names, cache_header, scratch) ||
					!BuildCacheFileWriteHeaderPreprocess(cache_header) ||
					!build_cache_file_write_header_and_compress(cache_header))
				{
					build_cache_file_cancel();
					printf_s("### FAILED TO BUILD CACHE FILE.\n\n");
					break;
				}

				printf_s("successfully built cache file.\n");

			} while (false);

			YELO_FREE(scratch);

			if (!BuildCacheFileGlobals()->canceled)
			{
				tag_groups_dump_memory();

				if (TEST_FLAG(begin_flags, Flags::_build_cache_file_begin_dump_tag_group_allocation_stats_bit))
				{
					using namespace TagGroups;

					c_tag_group_allocation_statistics::Initialize();
					// assume all tags that are loaded were used in the cache file
					for (datum_index tag_index : c_tag_iterator::all())
					{
						c_tag_group_allocation_statistics::BuildStatsForTag(tag_index);
					}
					c_tag_group_allocation_statistics::DumpToFile();
					c_tag_group_allocation_statistics::Dispose();
				}
			}
		}
	};
};
#endif