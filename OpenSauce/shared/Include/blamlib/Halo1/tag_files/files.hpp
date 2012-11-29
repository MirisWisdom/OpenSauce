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
};