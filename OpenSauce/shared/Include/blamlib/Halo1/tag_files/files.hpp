/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#pragma once

namespace Yelo
{
	namespace Enums
	{
		enum {
			k_maximum_filename_length = 255,
		};

		enum file_reference_location : _enum
		{
			_file_reference_location_none = CAST(_enum, NONE),
			_file_reference_location_application_relative = 0, // _file_reference_application_relative
			_file_reference_location_tags,
			_file_reference_location_absolute, // _file_reference_absolute

			k_number_of_file_reference_locations
		};
	};

	namespace Flags
	{
		enum file_reference_flags : word_flags
		{
			_name_directory_bit,
			_name_parent_directory_bit,
			_name_file_bit,
			_name_extension_bit,

			k_number_of_name_flags,
		};
	};

	struct s_file_reference {
		tag			signature;
		Flags::file_reference_flags		flags;
		Enums::file_reference_location	location;
		char		file_name[Enums::k_maximum_filename_length+1];
		HANDLE		handle;
	}; BOOST_STATIC_ASSERT( sizeof(s_file_reference) == 0x10C );

	namespace blam
	{
		void PLATFORM_API file_reference_create(s_file_reference& reference, long_enum location = Enums::_file_reference_location_tags);

		s_file_reference& PLATFORM_API file_reference_add_directory(s_file_reference& reference, cstring directory);

		s_file_reference& PLATFORM_API file_reference_set_name(s_file_reference& reference, cstring name);

		cstring PLATFORM_API file_reference_get_name(const s_file_reference& reference, long_flags flags, __out char name[Enums::k_maximum_filename_length+1]);

		s_file_reference& file_reference_create(s_file_reference& reference, cstring directory, cstring name, cstring ext, 
			long_enum location = Enums::_file_reference_location_tags);

		s_file_reference& file_reference_create_from_path(s_file_reference& reference, cstring path, bool is_directory = false);

		int16 PLATFORM_API find_files(long_flags flags, const s_file_reference& directory, int32 maximum_count, s_file_reference references[]);
		template<size_t _SizeOfArray>
		int16 find_files(long_flags flags, const s_file_reference& directory, s_file_reference (&references)[_SizeOfArray])
		{
			return find_files(flags, directory, _SizeOfArray, references);
		}

		void file_references_sort(long_flags name_flags, size_t count, s_file_reference references[]);
		template<size_t _SizeOfArray>
		void file_references_sort(long_flags name_flags, s_file_reference (&references)[_SizeOfArray])
		{
			file_references_sort(name_flags, _SizeOfArray, references);
		}

		bool PLATFORM_API file_exists(const s_file_reference& reference);
	};
};