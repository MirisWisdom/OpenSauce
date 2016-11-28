/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#include "Common/Precompile.hpp"
#include <blamlib/Halo1/tag_files/files.hpp>

#include <blamlib/Halo1/cseries/errors.hpp>

namespace Yelo
{
	void s_file_reference::Verify() const
	{
		YELO_ASSERT(signature == k_signature);
		YELO_ASSERT(VALID_FLAGS(flags, Flags::k_number_of_name_flags));
		YELO_ASSERT(location>=NONE && location<Enums::k_number_of_file_reference_locations);
	}

	int __cdecl s_file_reference::CompareProc(long_flags name_flags, const s_file_reference* lhs, const s_file_reference* rhs)
	{
		char lhs_name[Enums::k_maximum_filename_length+1];
		char rhs_name[Enums::k_maximum_filename_length+1];

		blam::file_reference_get_name(*lhs, name_flags, lhs_name);
		blam::file_reference_get_name(*rhs, name_flags, rhs_name);

		return _stricmp(lhs_name, rhs_name);
	}

	namespace blam
	{
		//////////////////////////////////////////////////////////////////////////
		// files.c
		s_file_reference& file_reference_create(s_file_reference& reference, cstring directory, cstring name, cstring ext, long_enum location)
		{
			char buffer[Enums::k_maximum_filename_length+1];
			int result = _snprintf_s(buffer, Enums::k_maximum_filename_length, "%s%s%s.%s", directory,
				!is_null_or_empty(directory) ? "\\" : "",
				name, ext);

			file_reference_create(reference, location);
			if (result > 0)
				file_reference_set_name(reference, buffer);
			else
			{
				YELO_WARN("file_reference_create failed: %d; hint: @%p %s.%s", result, 
					_ReturnAddress(), name, ext);
			}

			return reference;
		}

		s_file_reference& file_reference_create_from_path(s_file_reference& reference, cstring path, bool is_directory)
		{
			file_reference_create(reference, Enums::_file_reference_location_none);

			if(is_directory)
				file_reference_add_directory(reference, path);
			else
				file_reference_set_name(reference, path);

			return reference;
		}

		void file_references_sort(long_flags name_flags, size_t count, s_file_reference references[])
		{
			Qsort(references, count, s_file_reference::CompareProc, name_flags);
		}

		template<size_t k_buffer_size = 1024, bool k_set_eof = true> // defaults are based on engine code
		static void file_printf_internal(s_file_reference& reference, cstring format, va_list argptr)
		{
			char buffer[k_buffer_size];
			int result = vsprintf_s(buffer, format, argptr);

			// NOTE: engine doesn't do this
			if (result < 0)
			{
				file_error(__FUNCTION__, reference);
				return;
			}

			size_t buffer_length = strlen(buffer);
			file_write(reference, buffer_length, buffer);

			if (k_set_eof)
			{
				uint32 position = file_get_position(reference);
				file_set_eof(reference, position);
			}
		}
		void file_printf(s_file_reference& reference, cstring format, ...)
		{
			if (format == nullptr)
				return;

			va_list argptr;
			va_start(argptr, format);
			file_printf_internal(reference, format, argptr);
			va_end(argptr);
		}

		//////////////////////////////////////////////////////////////////////////
		// files_windows.c
		uint32 PLATFORM_API file_get_position(const s_file_reference& reference)
		{
			reference.Verify();
			DWORD position = SetFilePointer(reference.handle, 0, nullptr, FILE_CURRENT);
			// NOTE: engine doesn't check if not NO_ERROR. see MSDN for more details
			if (position == INVALID_SET_FILE_POINTER /*&& GetLastError() != NO_ERROR*/)
				file_error(__FUNCTION__, reference);

			return position;
		}

		bool PLATFORM_API file_set_eof(s_file_reference& reference, uint32 position)
		{
			reference.Verify();
			if (file_set_position(reference, position))
			{
				SetEndOfFile(reference.handle);
				return true;
			}

			file_error(__FUNCTION__, reference);
			return false;
		}
	};
};