/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#include "Common/Precompile.hpp"
#include "Engine/EngineFunctions.hpp"

namespace Yelo
{
#define __EL_INCLUDE_ID			__EL_INCLUDE_GAME
#define __EL_INCLUDE_FILE_ID	__EL_GAME_ENGINE_FUNCTIONS
#include "Memory/_EngineLayout.inl"

	namespace Engine
	{
		API_FUNC_NAKED bool GetCmdLineParameter(cstring parameter, cstring* value_out)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(GET_CMD_LINE_PARAMETER);

			API_FUNC_NAKED_START()
				push	value_out
				push	parameter
				call	FUNCTION
			API_FUNC_NAKED_END_CDECL(2);
		}


		API_FUNC_NAKED void display_assert(cstring reason, cstring file, const uint32 line, bool halt)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(DISPLAY_ASSERT);

			API_FUNC_NAKED_START()
				xor		ecx, ecx
				mov		cl, halt
				push	ecx
				push	line
				push	file
				push	reason
				call	FUNCTION
			API_FUNC_NAKED_END_CDECL(4);
		}

		API_FUNC_NAKED void* debug_malloc(const size_t ptr_size, const bool fill_with_garbage, cstring file, const uint32 line)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(DEBUG_MALLOC);

			API_FUNC_NAKED_START()
				push	line
				push	file
				xor		ecx, ecx
				mov		cl, fill_with_garbage
				push	ecx
				push	ptr_size
				call	FUNCTION
			API_FUNC_NAKED_END_CDECL(4);
		}

		API_FUNC_NAKED void debug_free(void* pointer, cstring file, const uint32 line)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(DEBUG_FREE);

			API_FUNC_NAKED_START()
				push	line
				push	file
				push	pointer
				call	FUNCTION
			API_FUNC_NAKED_END_CDECL(3);
		}

		API_FUNC_NAKED void* debug_realloc(void* pointer, const size_t new_size, cstring file, const uint32 line)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(DEBUG_REALLOC);

			API_FUNC_NAKED_START()
				push	line
				push	file
				push	new_size
				push	pointer
				call	FUNCTION
			API_FUNC_NAKED_END_CDECL(4);
		}


		API_FUNC_NAKED void file_reference_create(s_file_reference& reference, long_enum location)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(FILE_REFERENCE_CREATE);

			API_FUNC_NAKED_START()
				push	location
				push	reference
				call	FUNCTION
			API_FUNC_NAKED_END_CDECL(2);
		}

		API_FUNC_NAKED s_file_reference& file_reference_add_directory(s_file_reference& reference, cstring directory)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(FILE_REFERENCE_ADD_DIRECTORY);

			API_FUNC_NAKED_START()
				push	directory
				push	reference
				call	FUNCTION
			API_FUNC_NAKED_END_CDECL(2);
		}

		API_FUNC_NAKED s_file_reference& file_reference_set_name(s_file_reference& reference, cstring name)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(FILE_REFERENCE_SET_NAME);

			API_FUNC_NAKED_START()
				push	name
				push	reference
				call	FUNCTION
			API_FUNC_NAKED_END_CDECL(2);
		}

		API_FUNC_NAKED cstring file_reference_get_name(const s_file_reference& reference, long_flags flags, __out char name[Enums::k_maximum_filename_length+1])
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(FILE_REFERENCE_GET_NAME);

			API_FUNC_NAKED_START()
				push	name
				push	flags
				push	reference
				call	FUNCTION
			API_FUNC_NAKED_END_CDECL(3);
		}

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

		API_FUNC_NAKED int16 find_files(long_flags flags, const s_file_reference& directory, int32 maximum_count, s_file_reference references[])
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(FIND_FILES);

			API_FUNC_NAKED_START()
				push	references
				push	maximum_count
				push	directory
				push	flags
				call	FUNCTION
			API_FUNC_NAKED_END_CDECL(4);
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

		API_FUNC_NAKED bool file_exists(const s_file_reference& reference)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(FILE_EXISTS);

			API_FUNC_NAKED_START()
				push	reference
				call	FUNCTION
			API_FUNC_NAKED_END_CDECL(1);
		}
	};
};