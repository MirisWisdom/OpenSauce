/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

namespace Yelo
{
#if PLATFORM_IS_EDITOR
	struct s_file_reference;
	struct tag_group;

	namespace Memory
	{
		struct s_byte_swap_definition;
	};

	namespace Enums
	{
		enum {
			k_maximum_tag_files_per_index = 24000, // MAXIMUM_TAG_FILES_PER_INDEX
		};
	};

	namespace TagGroups
	{
		struct s_tag_file_globals;

		extern cstring K_TAG_FILES_DIRECTORY; /// "tags\"
		extern cstring K_DATA_FILES_DIRECTORY;/// "data\"

		s_tag_file_globals* TagFileGlobalsThreaded();
		s_tag_file_globals* TagFileGlobals();

		Memory::s_byte_swap_definition* TagHeaderBsDefinition();

		bool TagFileRequiresByteSwap();
	};
#endif
	namespace blam
	{
#if PLATFORM_IS_EDITOR
		void PLATFORM_API tag_files_flush();


		bool PLATFORM_API tag_file_open(tag group_tag, cstring filename, 
			_Out_opt_ bool* is_readonly, _Out_opt_ uint32* crc, bool from_file_system);
		template<typename T> inline
		bool tag_file_open(cstring filename, 
			_Out_opt_ bool* is_readonly, _Out_opt_ uint32* crc, bool from_file_system)
		{
			return tag_file_open(T::k_group_tag, filename, is_readonly, crc, from_file_system);
		}

		bool PLATFORM_API tag_file_read(int32 file_position, size_t buffer_size, void* buffer);

		// Is the tag file read only?
		bool PLATFORM_API tag_file_read_only(tag group_tag, cstring name);
		template<typename T> inline
		bool tag_file_read_only(cstring name)
		{
			return tag_file_read_only(T::k_group_tag, name);
		}

		// Does the tag file exist?
		bool PLATFORM_API tag_file_exists(tag group_tag, cstring name);
		template<typename T> inline
		bool tag_file_exists(cstring name)
		{
			return tag_file_exists(T::k_group_tag, name);
		}

		bool PLATFORM_API tag_file_get_file_reference(_Out_ s_file_reference& reference,
			tag group_tag, _In_opt_ cstring name);
#endif

		cstring tag_name_strip_path(cstring name);
	};
};