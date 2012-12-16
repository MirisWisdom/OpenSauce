/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#include "Common/Precompile.hpp"
#include <blamlib/Halo1/cache/cache_file_builder.hpp>

#include "Engine/EngineFunctions.hpp"

namespace Yelo
{
	namespace Cache
	{
		cstring s_build_cache_file_globals::k_temp_cache_file_name = "temporary uncompressed cache file.bin";

		void s_build_cache_file_globals::DataFilesSave()
		{
			sounds_data_file.Save();
			locale_data_file.Save();
			bitmaps_data_file.Save();
		}
		void s_build_cache_file_globals::DataFilesPreprocessForSave()
		{
			sounds_data_file.PreprocessForSave();
			locale_data_file.PreprocessForSave();
			bitmaps_data_file.PreprocessForSave();
		}

		void s_build_cache_file_globals::TemporaryFileOpen(cstring filename)
		{
			file_handle = CreateFileA(filename, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		}
		void s_build_cache_file_globals::TemporaryFileClose(cstring filename)
		{
			if(file_handle != INVALID_HANDLE_VALUE)
			{
				CloseHandle(file_handle);
				file_handle = INVALID_HANDLE_VALUE;
			}

			if(file_handle != NULL)
				DeleteFileA(filename);
		}
		void s_build_cache_file_globals::TemporaryFileCopy(cstring new_filename, cstring filename)
		{
			CopyFileA(filename, new_filename, FALSE);
		}
	};

	namespace blam
	{
		static const unsigned k_cache_file_tag_memory_alignment_bit = Flags::_alignment_32_bit;

		template<typename T>
		static T* rebase_pointer(T* pointer, uintptr_t base_address, uintptr_t virtual_base_address)
		{
			T* result = CAST_PTR(T*,
				(CAST_PTR(byte*,pointer) - base_address) + virtual_base_address);

			return result;
		}
		template<typename T>
		static T* align_pointer(T* pointer, unsigned alignment_bit = k_cache_file_tag_memory_alignment_bit)
		{
			uintptr_t aligned_ptr = CAST_PTR(uintptr_t, pointer);
			const uintptr_t alignment_mask = (1<<alignment_bit) - 1;

			if(aligned_ptr & alignment_mask)
				aligned_ptr = (aligned_ptr | alignment_mask) + 1;

			return CAST_PTR(T*, aligned_ptr);
		}
		template<typename T>
		static T* stream_blob_to_buffer(T*& stream, void* blob, size_t blob_size, unsigned alignment_bit = k_cache_file_tag_memory_alignment_bit)
		{
			memcpy(stream, blob, blob_size);
			stream = CAST_PTR(byte*, stream) + blob_size;
			stream = align_pointer(stream);
			return stream;
		}

		void* stream_tag_block_to_buffer(const tag_block* block, void* stream, uintptr_t stream_base_address, uintptr_t virtual_base_address, tag_reference_name_reference* tag_names);

		static void* stream_tag_block_to_buffer_postprocess_tag_data(void*& return_stream, uintptr_t stream_base_address, uintptr_t virtual_base_address, tag_reference_name_reference* tag_names,
			const TagGroups::c_tag_field_scanner& scanner)
		{
			tag_data* data = scanner.GetFieldAs<tag_data>();

			if(data->definition == NULL)
			{	// not exactly what tool's asm does here, but should amount to the same output
				data->address = NULL;
			}
			else if(!TEST_FLAG(data->definition->flags, Flags::_tag_data_not_streamed_to_cache_bit))
			{
				if(data->size > 0 && data->address != NULL)
				{
					// record the current stream position as runtime address for the data blob, then write the blob
					void* data_address = rebase_pointer(return_stream, stream_base_address, virtual_base_address);
					stream_blob_to_buffer(return_stream, data->address, data->size);

					data->definition = NULL;
					data->address = data_address;
				}
				else
				{
					data->address = NULL;
					data->definition = NULL;
				}
			}

			return return_stream;
		}
		static void* stream_tag_block_to_buffer_postprocess_tag_block(void*& return_stream, uintptr_t stream_base_address, uintptr_t virtual_base_address, tag_reference_name_reference* tag_names,
			const TagGroups::c_tag_field_scanner& scanner)
		{
			tag_block* block = scanner.GetFieldAs<tag_block>();

			if(block->count > 0)
			{
				// record the current stream position as runtime address for the block elements, then write the elements
				void* elements_address = rebase_pointer(return_stream, stream_base_address, virtual_base_address);
				return_stream = stream_tag_block_to_buffer(block, return_stream, stream_base_address, virtual_base_address, tag_names);

				block->address = elements_address;
				block->definition = NULL;
			}
			else
			{
				block->address = NULL;
				block->definition = NULL;
			}

			return return_stream;
		}
		static void* stream_tag_block_to_buffer_postprocess_tag_reference(void*& return_stream, uintptr_t stream_base_address, uintptr_t virtual_base_address, tag_reference_name_reference* tag_names,
			const TagGroups::c_tag_field_scanner& scanner)
		{
			tag_reference* reference = scanner.GetFieldAs<tag_reference>();
			int32 absolute_index = reference->tag_index.index;
			YELO_ASSERT( reference->tag_index==datum_index::null || 
						(absolute_index>=0 && 
							(absolute_index<Enums::k_maximum_simultaneous_tag_instances || absolute_index<Enums::k_maximum_simultaneous_tag_instances_upgrade)
						) 
				);

			reference->name = reference->tag_index.IsNull() ? NULL : tag_names[absolute_index];
			reference->name_length = 0;

			if(scanner.TagFieldIsStringId())
			{
				string_id_yelo* sidy = CAST_PTR(string_id_yelo*, reference);
			}

			return return_stream;
		}
		static void* stream_tag_block_to_buffer(const tag_block* block, void* stream, uintptr_t stream_base_address, uintptr_t virtual_base_address, tag_reference_name_reference* tag_names)
		{
			YELO_ASSERT( stream && stream_base_address );
			YELO_ASSERT( virtual_base_address );

			void* return_stream = stream;

			// copy the tag memory, loaded by the tools, verbatim into the cache tag memory stream
			stream_blob_to_buffer(return_stream, block->address, block->count * block->get_element_size());

			for(int x = 0; x < block->count; x++)
			{
				TagGroups::c_tag_field_scanner scanner(block->definition->fields, 
					// get the the element address in the cache tag memory stream
					CAST_PTR(byte*, stream) + (block->get_element_size() * x)
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
			tag_block* block = NULL;

			void* return_stream = stream_tag_block_to_buffer(block, stream, CAST_PTR(uintptr_t, stream), virtual_base_address, tag_names);

			return CAST_PTR(uintptr_t, return_stream) - CAST_PTR(uintptr_t, stream);
		}
	};
};