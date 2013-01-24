/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#include "Common/Precompile.hpp"
#include "CheApeCore.hpp"

#include <zlib/zlib.h>

#include "Engine/EngineFunctions.hpp"
#include "CheApeInterface.hpp"
#include "TagGroups/TagGroups.hpp"

namespace Yelo
{
#define __EL_INCLUDE_ID			__EL_INCLUDE_OPEN_SAUCE
#define __EL_INCLUDE_FILE_ID	__EL_MEMORY_CHE_APE_INTERFACE
#include "Memory/_EngineLayout.inl"

	namespace CheApe
	{
		long_enum _InitError = k_error_none;


		static struct {
			struct {
				void* base_address;
				uint32 data_size;
				s_cache_header header;

				DWORD MemoryMapInitialize()
				{
					base_address = VirtualAlloc(CAST_PTR(void*, Enums::k_cheape_physical_memory_map_address), Enums::k_cheape_physical_memory_map_size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

					if(base_address == NULL || base_address != CAST_PTR(void*, Enums::k_cheape_physical_memory_map_address))
						return GetLastError();

					return ERROR_SUCCESS;
				}
				BOOL MemoryMapDispose(DWORD& error_code)
				{
					BOOL result = TRUE;
					error_code = S_OK;

					if(base_address != NULL)
					{
						result = VirtualFree(base_address, 0, MEM_RELEASE);
						if(result == FALSE)
							error_code = GetLastError();

						base_address = NULL;
					}

					return result;
				}

				bool Read(cstring path)
				{
					FILE* file_handle;
					fopen_s(&file_handle, path, "rb");

					if(!file_handle)
					{
						Debug::WriteFormat("CheApe: Failed to load file '%s'!", k_cache_file_name);
						_InitError = k_error_LoadCacheFile;

						return false;
					}

					fseek(file_handle, 0, SEEK_END);
					data_size = ftell(file_handle) - sizeof(s_cache_header);
					fseek(file_handle, 0, SEEK_SET);

					fread_s(&header, sizeof(header), sizeof(header), 1, file_handle);
					cstring invalid_reason_str = header.GetInvalidReasonString(Enums::k_cheape_cache_signature_halo1, this->base_address);
					if(invalid_reason_str != NULL)
					{
						Debug::WriteFormat("CheApe: Bad '%s' file (%s)", k_cache_file_name, invalid_reason_str);
						_InitError = k_error_LoadCacheFile;

						fclose(file_handle);
						return false;
					}

					fseek(file_handle, header.DataOffset, SEEK_SET);
					fread_s(base_address, data_size, data_size, 1, file_handle);

					fclose(file_handle);
					return true;
				}
			}cache;

			struct {
				t_custom_tag_group_data* data_array;

				void** address;
				uint32 count;

				void SetupTagGroupPointers()
				{
					tag_group** old_tag_groups = CAST_PTR(tag_group**, kTagGroupDefinitionsAddress);
					tag_group** address =  CAST_PTR(tag_group**, this->address);

					// first, copy the original tag group pointers
					int32 count;
					for(count = kTagGroupDefinitionsCount; count > 0; count--, address++, old_tag_groups++)
						*address = *old_tag_groups;

					// then, copy our new tag group addresses
					count = 0;
					for(s_custom_tag_group_datum* data = CAST(s_custom_tag_group_datum*, *data_array); 
						count < data_array->Header.next_index; count++, address++)
						*address = data[count].definition;
				}
			}new_tag_groups;

		}_globals = {
			{},
			{CAST_PTR(t_custom_tag_group_data*, Enums::k_cheape_physical_memory_map_address),}
		};

		// declared in CheApeShared.hpp
		s_cache_header& GlobalCacheHeader() { return _globals.cache.header; }

		// declared in CheApeShared.hpp
		bool GetCacheFileResourceBuffer(void*& buffer, uint32& size, uint32& compressed_size)
		{
			struct s_tool_info_header
			{
				tag_string pch_build_date;
				string256 pch_file;
			}* tool_info_header = NULL;

			buffer = NULL;
			size = compressed_size = 0;

			// Read the CheApe cache into the resource buffer
			{
				FILE* file_handle;
				fopen_s(&file_handle, k_cache_file_name, "rb");

				if(!file_handle)
					return false;

				size = sizeof(s_tool_info_header);

				fseek(file_handle, 0, SEEK_END);
				size += ftell(file_handle);
				fseek(file_handle, 0, SEEK_SET);

				buffer = new byte[size];
				tool_info_header = CAST_PTR(s_tool_info_header*, buffer);

				// Read the cache data directly into the memory after the header
				fread_s(tool_info_header+1, size, size, 1, file_handle);
				fclose(file_handle);
			}

			// Build the header information of the resource buffer
			CRT_DEBUG_FILL_OFF_BEGIN()	// so the debug CRT doesn't fill our buffers with magic numbers
				CheApeApi_GetPchBuildDateA(NUMBEROF(tool_info_header->pch_build_date), tool_info_header->pch_build_date);
				CheApeApi_GetPchPathA(NUMBEROF(tool_info_header->pch_file), tool_info_header->pch_file);
			CRT_DEBUG_FILL_OFF_END();

			compressed_size = compressBound(size);
			Bytef* dest = new Bytef[compressed_size];
			int result = compress2(dest, &compressed_size, CAST_PTR(Bytef*,buffer), size, Z_BEST_COMPRESSION);

			buffer = dest;

			return result == Z_OK;
		}

		void UnProtectMemoryRegion()
		{
			if(_InitError > k_error_none) return;

			DWORD old;
			if(!VirtualProtect(CAST_PTR(void*, kExeBaseAddress),kExeSize,PAGE_EXECUTE_READWRITE,&old))
			{
				Debug::Write("CheApe: VirtualProtect wants to give a big FUCK YOU to who ever sees this...");
				_InitError = k_error_UnProtectMemoryRegion;
			}
		}

		void PhysicalMemoryMapInitialize()
		{
			if(_InitError > k_error_none) return;

			DWORD result = _globals.cache.MemoryMapInitialize();
			if(result != ERROR_SUCCESS)
			{
				Debug::WriteFormat("CheApe: Couldn't allocate the memory map! (%X)", result);
				_InitError = k_error_PhysicalMemoryMapInitialize;
			}
		}

		void LoadCacheFile()
		{
			if(_InitError > k_error_none) return;

			if(!_globals.cache.Read(k_cache_file_name))
				return;

			// use the memory following the cache file data for the tag groups address list
			_globals.new_tag_groups.address = CAST_PTR(void**, CAST_PTR(uint32,_globals.cache.base_address) + _globals.cache.data_size );
			_globals.new_tag_groups.count = (kTagGroupDefinitionsCount-1)+_globals.new_tag_groups.data_array->Header.next_index;
			
			// HACK to make it unload the map and not use it since there are no extra tags
			if(_globals.new_tag_groups.count == (kTagGroupDefinitionsCount-1))
			{
				Debug::Write("CheApe: No new tags");
				_InitError = k_error_LoadCacheFile;
			}
		}

		static int __cdecl tag_group_definition_compare(void *, const void* lhs, const void* rhs)
		{
			return strcmp(
				(*CAST_PTR(const tag_group*const*,lhs))->name, 
				(*CAST_PTR(const tag_group*const*,rhs))->name);
		}

		void SetupTagGroupPointers()
		{
			if(_InitError > k_error_none) return;

			_globals.new_tag_groups.SetupTagGroupPointers();

			// ABC the tag groups list
			qsort_s(_globals.new_tag_groups.address, _globals.new_tag_groups.count+1, sizeof(tag_group*), tag_group_definition_compare, NULL);
		}

		void SetupTagGroupCounts()
		{
			if(_InitError > k_error_none) return;

			int32 count = NUMBEROF(TagGroupDefinitionsCountRefs16bit);
			uint16 group_count = CAST(uint16, _globals.new_tag_groups.count);

			while(count--)
				*CAST_PTR(uint16*, TagGroupDefinitionsCountRefs16bit[count]) = group_count;

			count = NUMBEROF(TagGroupDefinitionsCountRefs8bit);
			while(count--)
				*CAST_PTR(byte*, TagGroupDefinitionsCountRefs8bit[count]) = CAST(byte, group_count);
		}

		void UpdateTagGroupReferences()
		{
			if(_InitError > k_error_none) return;

			uint32 address = CAST_PTR(uint32, _globals.new_tag_groups.address);

			int32 count = NUMBEROF(TagGroupDefinitionsRefs0);
			while(count--)
				*CAST_PTR(uint32*, TagGroupDefinitionsRefs0[count]) = address;

			count = NUMBEROF(TagGroupDefinitionsRefs1);
			while(count--)
				*CAST_PTR(uint32*, TagGroupDefinitionsRefs1[count]) = address+4;
		}

		void Dispose()
		{
			if(_InitError == k_error_PhysicalMemoryMapInitialize) return;

			DWORD error_code;
			if(_globals.cache.MemoryMapDispose(error_code) == FALSE)
				Debug::WriteFormat("CheApe: Couldn't free the memory map! (%X)", error_code);
		}
	};
};