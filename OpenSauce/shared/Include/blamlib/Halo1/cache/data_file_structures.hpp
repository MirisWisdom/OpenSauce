/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

#include <blamlib/Halo1/cache/data_file.hpp>

namespace Yelo
{
	namespace Cache
	{
		struct s_data_file_header
		{
			long_enum type; // Enums::data_file_reference_type
			// when building or updating a data file, acts as the write cursor for new items
			int32 file_names_offset;
			int32 file_index_table_offset;
			int32 tag_count;
		}; BOOST_STATIC_ASSERT( sizeof(s_data_file_header) == 0x10 );

		struct s_data_file_item
		{
			int32 name_offset;
			int32 size;
			int32 data_offset;
		}; BOOST_STATIC_ASSERT( sizeof(s_data_file_item) == 0xC );

		struct s_data_file
		{
			s_data_file_header header;
			struct {
				s_data_file_item* address;
				int32 count;
			}file_index_table;
			struct {
				int32 total_size;
				int32 used_size;
				char* address;

				byte* AsByteBuffer()				{ return CAST_PTR(byte*, address); }
				const byte* AsByteBuffer() const	{ return CAST_PTR(const byte*, address); }
			}file_names;
			bool writable;
			PAD24;
			struct {
				int32 count;
				int32 size;
			}item_hits, item_adds_or_misses;
			cstring name;
			HANDLE file_handle;

		private:
			void FreeResources();
			bool Read(uint32 position, void* buffer, size_t buffer_size);
			bool ReadHeader(Enums::data_file_reference_type expected_type);
			bool ReadFileNames();
			bool ReadFileIndexTable();

		public:
			bool Open(Enums::data_file_reference_type data_file, bool store_resources,
				cstring full_path); // full_path should NOT be allocated on the stack, as it will become the value of 'name'

			bool Close();

			bool GetItemDataInfo(int32 item_index,
				int32& out_data_offset, int32& out_data_size) const;

			bool ReadItemData(uint32 position, void* buffer, size_t buffer_size);

#if PLATFORM_IS_EDITOR && PLATFORM_TYPE == PLATFORM_TOOL
		private:
			int32 AddItemName(cstring item_name);
			int32 AddNewItem(cstring item_name);
			int32 GetItemIndex(cstring item_name) const;
		public:
			int32 AddItem(cstring item_name, void* item_buffer, int32 item_buffer_size);
			int32 GetItemDataOffset(int32 item_index);

			static void DeleteForCopy(cstring file);

			void PreprocessForSave();
			void Save();
#endif
		}; BOOST_STATIC_ASSERT( sizeof(s_data_file) == 0x40 );

		// build_cache_file_globals (tools) and cache_file_globals (runtime) share the same ordering of the s_data_file instances, so I wrap them in this fake struct
		struct s_data_file_globals
		{
			Cache::s_data_file sounds;
			Cache::s_data_file locale;
			Cache::s_data_file bitmaps;

			s_data_file& Get(Enums::data_file_reference_type data_file);

#if PLATFORM_TYPE == PLATFORM_TOOL
			void Save();
			void PreprocessForSave();
#endif
		}; BOOST_STATIC_ASSERT( sizeof(s_data_file_globals) == 0xC0 );
	};
};