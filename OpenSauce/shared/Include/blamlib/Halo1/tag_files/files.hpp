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
			k_maximum_filename_length = 255, // MAXIMUM_FILENAME_LENGTH
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

			k_number_of_name_flags, // NUMBER_OF_REFERENCE_INFO_FLAGS

			_name_full_path_flags = FLAG(_name_directory_bit) | FLAG(_name_file_bit) | FLAG(_name_extension_bit)
		};

		enum file_permission_flags : long_flags
		{
			_permission_read_bit,
			_permission_write_bit,
			_permission_append_bit,
		};
	};

	struct s_file_reference {
		enum { k_signature = 'filo' };

		tag			signature;
		Flags::file_reference_flags		flags;
		Enums::file_reference_location	location;
		char		file_name[Enums::k_maximum_filename_length+1];
		HANDLE		handle;

		void Verify() const;

		static int __cdecl CompareProc(long_flags name_flags, const s_file_reference* lhs, const s_file_reference* rhs);
	}; BOOST_STATIC_ASSERT( sizeof(s_file_reference) == 0x10C );

	namespace blam
	{
		void PLATFORM_API file_reference_create(s_file_reference& reference, long_enum location = Enums::_file_reference_location_tags);

		s_file_reference& PLATFORM_API file_reference_add_directory(s_file_reference& reference, cstring directory);

		s_file_reference& PLATFORM_API file_reference_set_name(s_file_reference& reference, cstring name);

		char* PLATFORM_API file_reference_get_name(const s_file_reference& reference, long_flags flags, __out char name[Enums::k_maximum_filename_length+1]);

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

		void file_printf(s_file_reference& reference, cstring format, ...);


		void PLATFORM_API file_error(cstring operation, const s_file_reference& reference);

		bool PLATFORM_API file_create(const s_file_reference& reference);

		bool PLATFORM_API file_delete(const s_file_reference& reference);

		bool PLATFORM_API file_exists(const s_file_reference& reference);

		bool PLATFORM_API file_open(s_file_reference& reference, long_flags flags);

		bool PLATFORM_API file_close(s_file_reference& reference);

		uint32 PLATFORM_API file_get_position(const s_file_reference& reference);

		bool PLATFORM_API file_set_position(s_file_reference& reference, uint32 position);

		uint32 PLATFORM_API file_get_eof(const s_file_reference& reference);

		bool PLATFORM_API file_set_eof(s_file_reference& reference, uint32 position);

		bool PLATFORM_API file_read(s_file_reference& reference, size_t buffer_size, void* buffer);

		bool PLATFORM_API file_write(s_file_reference& reference, size_t buffer_size, const void* buffer);

		bool PLATFORM_API file_read_from_position(s_file_reference& reference, uint32 position,
			size_t buffer_size, void* buffer);

		bool PLATFORM_API file_write_to_position(const s_file_reference& reference, uint32 position,
			size_t buffer_size, const void* buffer);

		bool PLATFORM_API file_read_only(const s_file_reference& reference);
	};
};