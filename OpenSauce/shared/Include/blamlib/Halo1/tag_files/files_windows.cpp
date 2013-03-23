/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#include "Common/Precompile.hpp"
#include <blamlib/Halo1/tag_files/files.hpp>

namespace Yelo
{
	namespace blam
	{
		s_file_reference& file_reference_create(s_file_reference& reference, cstring directory, cstring name, cstring ext, long_enum location)
		{
			char buffer[256];
			_snprintf_s(buffer, NUMBEROF(buffer)-1, "%s%s%s.%s", directory, 
				!is_null_or_empty(directory) ? "\\" : "",
				name, ext);

			file_reference_create(reference, location);
			file_reference_set_name(reference, buffer);

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

		static int __cdecl file_references_sort_proc(void* ctxt, const void* _lhs, const void* _rhs)
		{
			long_flags name_flags = CAST_PTR(long_flags, ctxt);

			const s_file_reference* lhs = CAST_PTR(const s_file_reference*, _lhs);
			const s_file_reference* rhs = CAST_PTR(const s_file_reference*, _rhs);

			char lhs_name[Enums::k_maximum_filename_length+1];
			char rhs_name[Enums::k_maximum_filename_length+1];

			file_reference_get_name(*lhs, name_flags, lhs_name);
			file_reference_get_name(*rhs, name_flags, rhs_name);

			return _stricmp(lhs_name, rhs_name);
		}
		void file_references_sort(long_flags name_flags, size_t count, s_file_reference references[])
		{
			qsort_s(references, count, sizeof(s_file_reference), file_references_sort_proc, CAST_PTR(void*, name_flags));
		}
	};
};