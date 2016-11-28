/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#include "Common/Precompile.hpp"
#include <blamlib/Halo1/cache/data_file.hpp>

#include <blamlib/Halo1/cache/cache_file_builder.hpp>
#include <blamlib/Halo1/cache/cache_files.hpp>
#include <blamlib/Halo1/cache/cache_files_globals.hpp>
#include <blamlib/Halo1/cache/data_file_structures.hpp>
#include <YeloLib/Halo1/open_sauce/settings/yelo_shared_settings.hpp>

namespace Yelo
{
	namespace Cache
	{
		cstring K_DATA_FILE_EXTENSION = ".map";

		cstring DataFileTypeToString(Enums::data_file_type type)
		{
			switch (type)
			{
			case Enums::_data_file_type_bitmaps:return "bitmaps";
			case Enums::_data_file_type_sounds:	return "sounds";
			case Enums::_data_file_type_locale:	return "loc";

			YELO_ASSERT_CASE_UNREACHABLE();
			}

			return nullptr;
		}
		cstring DataFileTypeToString(Enums::data_file_reference_type type)
		{
			return DataFileTypeToString(CAST(Enums::data_file_type, type - Enums::_data_file_reference_bitmaps));
		}

		s_data_file& s_data_file_globals::Get(Enums::data_file_reference_type data_file)
		{
			switch (data_file)
			{
			case Enums::_data_file_reference_bitmaps:return this->bitmaps;
			case Enums::_data_file_reference_sounds: return this->sounds;
			case Enums::_data_file_reference_locale: return this->locale;

			YELO_ASSERT_CASE_UNREACHABLE();
			}
		}
		s_data_file& DataFileGet(Enums::data_file_reference_type data_file)
		{
#if PLATFORM_TYPE == PLATFORM_TOOL
			auto& globals = BuildCacheFileGlobals()->data_files;

			return globals.Get(data_file);
#elif !PLATFORM_IS_EDITOR
			auto& globals = CacheFileGlobals()->data_files;

			return globals.Get(data_file);
#else
			YELO_ASSERT(!"unsupported platform");
			__assume(false);
#endif
		}

		bool DataFileGetItemDataInfo(Enums::data_file_reference_type data_file, int32 item_index,
			int32& out_data_offset, int32& out_data_size)
		{
			auto& df = DataFileGet(data_file);

			return df.GetItemDataInfo(item_index, out_data_offset, out_data_size);
		}

		bool DataFileReadItemData(Enums::data_file_reference_type data_file,
			uint32 position, void* buffer, size_t buffer_size)
		{
			auto& df = DataFileGet(data_file);

			return df.ReadItemData(position, buffer, buffer_size);
		}

		bool DataFilesOpen(cstring bitmaps_path, cstring sounds_path, cstring locale_path,
			bool store_resources)
		{
#if !PLATFORM_IS_EDITOR || PLATFORM_TYPE == PLATFORM_TOOL
			using namespace Enums;

			if (DataFileGet(_data_file_reference_bitmaps).Open(_data_file_reference_bitmaps,store_resources, bitmaps_path) &&
				DataFileGet(_data_file_reference_sounds ).Open(_data_file_reference_sounds, store_resources, sounds_path) &&
				DataFileGet(_data_file_reference_locale ).Open(_data_file_reference_locale, store_resources, locale_path))
			{
				return true;
			}
#endif
			return false;
		}


		void s_data_file::FreeResources()
		{
			if (file_names.address != nullptr)
			{
				YELO_DELETE_ARRAY(file_names.address);
			}

			if (file_index_table.address != nullptr)
			{
				YELO_DELETE_ARRAY(file_index_table.address);
			}
		}

		bool s_data_file::Read(uint32 position, void* buffer, size_t buffer_size)
		{
			DWORD bytes_read;

#if PLATFORM_IS_EDITOR
			if (SetFilePointer(file_handle, position, nullptr, FILE_BEGIN) != INVALID_SET_FILE_POINTER &&
				ReadFile(file_handle, buffer, buffer_size, &bytes_read, nullptr) != FALSE &&
				bytes_read == buffer_size)
			{
				return true;
			}
#else
			OVERLAPPED overlapped = {};
			overlapped.Offset = position;
			if (ReadFile(file_handle, buffer, buffer_size, &bytes_read, &overlapped) != FALSE)
			{
				return bytes_read == buffer_size;
			}
			else if (GetLastError() == ERROR_IO_PENDING &&
				GetOverlappedResult(file_handle, &overlapped, &bytes_read, TRUE) != FALSE)
			{
				return bytes_read == buffer_size;
			}
#endif

			return false;
		}

		bool s_data_file::ReadHeader(Enums::data_file_reference_type expected_type)
		{
			if (!Read(0, &header, sizeof(header)))
			{
				DWORD last_error = GetLastError();
#if PLATFORM_IS_EDITOR
				printf_s("Failed to read data file header %s\n", name);
#endif
				return false;
			}

			if (header.type != expected_type)
			{
				memset(&header, 0, sizeof(header));
#if PLATFORM_IS_EDITOR
				printf_s("Invalid data file id in data file %s\n", name);
#endif
				return false;
			}

			return true;
		}

		bool s_data_file::ReadFileNames()
		{
			DWORD buffer_size = header.file_index_table_offset - header.file_names_offset;
			file_names.address = YELO_NEW_ARRAY(char, buffer_size);

			if (!Read(header.file_names_offset, file_names.address, buffer_size))
			{
				DWORD last_error = GetLastError();
#if PLATFORM_IS_EDITOR
				printf_s("Invalid format in data file %s\n", name);
#endif
				return false;
			}

			file_names.total_size = file_names.used_size = buffer_size;

			return true;
		}

		bool s_data_file::ReadFileIndexTable()
		{
			DWORD buffer_size = sizeof(s_data_file_item) * header.tag_count;
			file_index_table.address = YELO_NEW_ARRAY(s_data_file_item, header.tag_count);

			if (!Read(header.file_index_table_offset, file_index_table.address, buffer_size))
			{
				DWORD last_error = GetLastError();
#if PLATFORM_IS_EDITOR
				printf_s("Invalid format in data file %s\n", name);
#endif
				return false;
			}

			file_index_table.count = header.tag_count;

			return true;
		}

		bool s_data_file::Open(Enums::data_file_reference_type data_file, bool store_resources,
			cstring full_path)
		{
			memset(this, 0, sizeof(*this));
			this->name = full_path;
			this->writable = store_resources;

			DWORD file_access = GENERIC_READ;
			if (store_resources)
				file_access |= GENERIC_WRITE;

			DWORD file_share_mode = PLATFORM_ENGINE_VALUE(FILE_SHARE_READ, 0);

			DWORD file_flags = FILE_ATTRIBUTE_NORMAL;
#if !PLATFORM_IS_EDITOR
			file_flags |= FILE_FLAG_SEQUENTIAL_SCAN;
			file_flags |= FILE_FLAG_OVERLAPPED;
#endif

			this->file_handle = CreateFileA(full_path, file_access, file_share_mode, nullptr, OPEN_ALWAYS, file_flags, nullptr);

			if (file_handle != INVALID_HANDLE_VALUE &&
				ReadHeader(data_file) && ReadFileNames() && ReadFileIndexTable())
			{
				SetFilePointer(file_handle, header.file_names_offset, nullptr, FILE_BEGIN);
				return true;
			}

			FreeResources();
			if (store_resources)
			{
				header.type = data_file;
				header.file_names_offset = sizeof(header);
				DWORD bytes_written;
				WriteFile(file_handle, &header, sizeof(header), &bytes_written, nullptr);
			}

			DWORD last_error = GetLastError();
			printf_s("### FAILED TO OPEN DATA-CACHE FILE: %s.\n\n", this->name);
			return false;
		}

		bool s_data_file::Close()
		{
			CloseHandle(file_handle);
			FreeResources();

#if PLATFORM_TYPE == PLATFORM_TOOL
			float item_hits_size = CAST(float, item_hits.size);
			if(item_hits.size == 0)
				item_hits_size += 4.2949673e9f;

			printf_s("Cache pack file %s hits: %d for %3.2fM\n", 
				name, item_hits.count, item_hits_size * 9.5367432e-7f);

			float item_adds_or_misses_size = CAST(float, item_hits.size);
			if(item_adds_or_misses.size == 0)
				item_adds_or_misses_size += 4.2949673e9f;

			printf_s("Cache pack file %s adds/misses: %d for %3.2fM\n", 
				name, item_adds_or_misses.count, item_adds_or_misses_size * 9.5367432e-7f);
#endif

			memset(&header, 0, sizeof(header));
			file_handle = INVALID_HANDLE_VALUE; // engine doesn't do this
			return true;
		}

		bool s_data_file::GetItemDataInfo(int32 item_index,
				int32& out_data_offset, int32& out_data_size) const
		{
			if (item_index < 0 || item_index >= file_index_table.count)
				return false;

			s_data_file_item* item = &file_index_table.address[item_index];
			out_data_offset = item->data_offset;
			out_data_size = item->size;

			return true;
		}

		bool s_data_file::ReadItemData(uint32 position, void* buffer, size_t buffer_size)
		{
			YELO_ASSERT(file_handle != INVALID_HANDLE_VALUE);

			return Read(position, buffer, buffer_size);
		}

#if PLATFORM_TYPE == PLATFORM_TOOL
		int32 s_data_file::AddItemName(cstring item_name)
		{
			YELO_ASSERT(item_name);
			YELO_ASSERT(writable);

			size_t item_name_size = strlen(item_name) + 1;

			// name will begin at the end of the current buffer
			int32 name_offset = file_names.used_size;
			int32 new_names_size = file_names.used_size + CAST(int32, item_name_size);
			if (new_names_size >= file_names.total_size)
			{
				// double the names buffer size
				file_names.total_size += file_names.total_size;
				file_names.address = YELO_RENEW_ARRAY(char, file_names.address, file_names.total_size);
			}

			memcpy(file_names.AsByteBuffer() + file_names.used_size,
				item_name, item_name_size);
			file_names.used_size += item_name_size;

			return name_offset;
		}
		int32 s_data_file::AddNewItem(cstring item_name)
		{
			YELO_ASSERT(item_name);
			YELO_ASSERT(writable);

			if (header.tag_count >= file_index_table.count)
			{
				// engine reserves an additional 16 items
				file_index_table.count += 16;
				file_index_table.address = YELO_RENEW_ARRAY(s_data_file_item, 
					file_index_table.address, file_index_table.count);
			}

			int32 item_index = header.tag_count++;
			auto* item = &file_index_table.address[item_index];
			item->name_offset = AddItemName(item_name);

			return item_index;
		}
		int32 s_data_file::GetItemIndex(cstring item_name) const
		{
			YELO_ASSERT(item_name);

			for (int x = 0; x < header.tag_count; x++)
			{
				int32 name_offset = file_index_table.address[x].name_offset;
				cstring name = CAST_PTR(cstring, file_names.AsByteBuffer() + name_offset);

				if (!_stricmp(name, item_name))
					return x;
			}

			return NONE;
		}
		int32 s_data_file::AddItem(cstring item_name, void* item_buffer, int32 item_buffer_size)
		{
			YELO_ASSERT(item_name && item_buffer); // NOTE: engine doesn't verify buffer pointer

			int32 item_index = GetItemIndex(item_name);
			if (item_index == NONE)
			{
				if (writable)
				{
					item_index = AddNewItem(item_name);
					auto* item = &file_index_table.address[item_index];
					item->size = item_buffer_size;
					item->data_offset = header.file_names_offset;

					DWORD num_bytes_written;
					WriteFile(file_handle, item_buffer, item_buffer_size, &num_bytes_written, nullptr);
					header.file_names_offset += item_buffer_size;
				}
			}
			else if (file_index_table.address[item_index].size == item_buffer_size)
			{
				item_hits.count++;
				item_hits.size += item_buffer_size;
			}
			else
			{
				YELO_ERROR_FAILURE("%s: Tried to add item %s with different size (%d) than expected (%d).",
					name, item_name, item_buffer_size, file_index_table.address[item_index].size);

				item_index = NONE;
			}

			item_adds_or_misses.count++;
			item_adds_or_misses.size += item_buffer_size;

			return item_index;
		}
		int32 s_data_file::GetItemDataOffset(int32 item_index)
		{
			if (item_index < 0 || item_index >= header.tag_count)
				return NONE;

			return file_index_table.address[item_index].data_offset;
		}

		void s_data_file::DeleteForCopy(cstring file)
		{
			BOOL delete_result = DeleteFile(file);
			if(delete_result == ERROR_ACCESS_DENIED)
				printf_s("Could not delete %s for copy\n", file);
		}

		void s_data_file::PreprocessForSave()
		{
			SetFilePointer(this->file_handle, this->header.file_names_offset, nullptr, FILE_BEGIN);
			SetEndOfFile(this->file_handle);

			// data_file_open does this before it returns, so do the same
			SetFilePointer(this->file_handle, this->header.file_names_offset, nullptr, FILE_BEGIN);
		}

		void s_data_file::Save()
		{
			SetFilePointer(this->file_handle, 0, nullptr, FILE_END);

			this->header.file_names_offset = SetFilePointer(this->file_handle, 0, nullptr, FILE_CURRENT);
			DWORD bytes_written;
			// Write the file names
			WriteFile(this->file_handle, 
				this->file_names.address, this->file_names.used_size, 
				&bytes_written, nullptr);

			// Write the file index table
			this->header.file_index_table_offset = SetFilePointer(this->file_handle, 0, nullptr, FILE_CURRENT);
			WriteFile(this->file_handle, 
				this->file_index_table.address, this->file_index_table.count * sizeof(s_data_file_item), 
				&bytes_written, nullptr);


			// Go back to the start and write the new header
			SetFilePointer(this->file_handle, 0, nullptr, FILE_BEGIN);

			WriteFile(this->file_handle, 
				&this->header, sizeof(this->header), 
				&bytes_written, nullptr);
		}
#endif
	};

	namespace blam
	{
#if !PLATFORM_IS_EDITOR || PLATFORM_TYPE == PLATFORM_TOOL
		bool data_files_close()
		{
			return 
				data_file_close(Enums::_data_file_reference_bitmaps) &&
				data_file_close(Enums::_data_file_reference_sounds) &&
				data_file_close(Enums::_data_file_reference_locale);
		}

		bool data_file_close(Enums::data_file_reference_type data_file)
		{
			auto& file = Cache::DataFileGet(data_file);

			return file.Close();
		}
#endif
	};
};