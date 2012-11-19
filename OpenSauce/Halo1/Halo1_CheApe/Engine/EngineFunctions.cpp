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
	};
};