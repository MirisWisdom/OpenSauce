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
			_file_reference_location_data = 0,
			_file_reference_location_tags,
			_file_reference_location_unused,

			k_number_of_file_reference_locations
		};
	};

	namespace Flags
	{
		enum file_reference_flags : word_flags
		{
			_name_directory_bit = 1,
			_name_parent_directory_bit = 2,
			_name_file_bit = 3,
			_name_extension_bit = 4,
		};
	};

	struct s_file_reference {
		tag			signature;
		Flags::file_reference_flags		flags;
		Enums::file_reference_location	location;
		char		file_name[Enums::k_maximum_filename_length+1];
		HANDLE		handle;
	}; BOOST_STATIC_ASSERT( sizeof(s_file_reference) == 0x10C );
};