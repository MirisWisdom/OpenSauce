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

		enum data_file_reference_type : _enum {
			_data_file_reference_none,
			_data_file_reference_bitmaps,
			_data_file_reference_sounds,
			_data_file_reference_locale,
		};
	};

	namespace Cache
	{
		struct s_data_file;

		extern cstring K_DATA_FILE_EXTENSION;

		cstring DataFileTypeToString(Enums::data_file_type type);
		cstring DataFileTypeToString(Enums::data_file_reference_type type);

		s_data_file& DataFileGet(Enums::data_file_reference_type data_file);

		// utility for blam::cache_file_data_load
		bool DataFileGetItemDataInfo(Enums::data_file_reference_type data_file, int32 item_index,
			int32& out_data_offset, int32& out_data_size);
		// utility for blam::cache_file_data_load
		bool DataFileReadItemData(Enums::data_file_reference_type data_file,
			uint32 position, void* buffer, size_t buffer_size);

		bool DataFilesOpen(cstring bitmaps_path, cstring sounds_path, cstring locale_path,
			bool store_resources = false);
	};

	namespace blam
	{
		bool data_files_close();

		bool data_file_close(Enums::data_file_reference_type data_file);
	};
};