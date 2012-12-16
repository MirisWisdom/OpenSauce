/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

namespace Yelo
{
	namespace Enums
	{
		enum data_file_type {
			_data_file_type_bitmaps,
			_data_file_type_sounds,
			_data_file_type_locale,

			k_number_of_data_file_types,
		};
	};

	namespace Cache
	{
		struct s_data_file_header
		{
			int32 type;
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
			}file_names;
			bool writable;
			PAD24;
			struct {
				int32 count;
				int32 size;
			}unreferenced_items, referenced_items;
			cstring name;
			HANDLE file_handle;

			static void DeleteForCopy(cstring file);

			void PreprocessForSave();
			void Save();
		}; BOOST_STATIC_ASSERT( sizeof(s_data_file) == 0x40 );
	};
};