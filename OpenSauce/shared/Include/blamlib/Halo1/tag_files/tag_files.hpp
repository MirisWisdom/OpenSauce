/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once
#if PLATFORM_IS_EDITOR
#include <blamlib/tag_files/tag_groups_base.hpp>
#include <blamlib/Halo1/tag_files/files.hpp>

namespace Yelo
{
	struct s_file_reference;
	struct tag_group;
	namespace Memory
	{
		struct s_byte_swap_definition;
	};

				k_signature = 'blam',		// signature for files following Bungie's tag file format

				// signature for files following our custom tag file format:
				// * little endian
				k_signature_yelo = 'yelo',
			};

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

			bool IsBungieFormat() const { return signature == k_signature; }
			bool IsYeloFormat() const { return signature == k_signature_yelo; }

		struct s_tag_file_globals
		{
			bool open;
			bool single_file_open;
			bool writing;
			char single_file_name[Enums::k_max_tag_name_length+1];
			s_file_reference single_file_reference;
			byte_flags yelo_flags;
			s_tag_header single_file_header;

		private:
			char* GetSingleFilePath(char buffer[Enums::k_maximum_filename_length+1]) const;

			void SingleFileClose();

			void ReadHeaderFailure(const tag_group* expected_group) const;
			void ReadHeaderInvalidFormat(const tag_group* expected_group, cstring format_type) const;
			void ReadHeaderGroupMismatch(const tag_group* expected_group, const tag_group* found_group) const;
			void ReadHeaderVersionMistmatch(const tag_group* expected_group, cstring reason) const;
		public:
			bool ReadHeader(tag group_tag);
			bool WriteHeader(s_tag_header& header);

			bool SingleFileOpen(tag group_tag, cstring name, bool for_writing, bool from_file_system);
		}; BOOST_STATIC_ASSERT( sizeof(s_tag_file_globals) == 0x250 );

		s_tag_file_globals* TagFileGlobalsThreaded();
		s_tag_file_globals* TagFileGlobals();

		Memory::s_byte_swap_definition* TagHeaderBsDefinition();

		bool TagFileRequiresByteSwap();
	};

	namespace blam
	{
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

		cstring tag_name_strip_name(cstring name);
	};
};

#endif