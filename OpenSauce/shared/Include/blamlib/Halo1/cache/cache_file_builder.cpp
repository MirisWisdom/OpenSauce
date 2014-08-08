/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#include "Common/Precompile.hpp"
#include <blamlib/Halo1/cache/cache_file_builder.hpp>
#if PLATFORM_TYPE == PLATFORM_TOOL

#include <blamlib/Halo1/game/game_globals.hpp>
#include <blamlib/Halo1/game/game_globals_definitions.hpp>
#include <blamlib/Halo1/scenario/scenario_definitions.hpp>
#include <blamlib/Halo1/tag_files/tag_field_scanner.hpp>
#include <blamlib/Halo1/tag_files/tag_groups.hpp>

#include <YeloLib/memory/memory_interface_base.hpp>
#include <YeloLib/Halo1/tag_files/string_id_yelo.hpp>

namespace Yelo
{
	namespace Cache
	{
		cstring s_build_cache_file_globals::k_temp_cache_file_name = "temporary uncompressed cache file.bin";

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

		DWORD s_build_cache_file_globals::GetFileSize() const
		{
			return ::GetFileSize(file_handle, nullptr);
		}

		void s_build_cache_file_globals::TemporaryFileOpen(cstring filename)
		{
			file_handle = CreateFileA(filename, GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
		}
		void s_build_cache_file_globals::TemporaryFileClose(cstring filename)
		{
			if(file_handle != INVALID_HANDLE_VALUE)
			{
				CloseHandle(file_handle);
				file_handle = INVALID_HANDLE_VALUE;
			}

			if(file_handle != nullptr)
				DeleteFileA(filename);
		}
		void s_build_cache_file_globals::TemporaryFileCopy(cstring new_filename, cstring filename)
		{
			CopyFileA(filename, new_filename, FALSE);
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
	};
};
#endif