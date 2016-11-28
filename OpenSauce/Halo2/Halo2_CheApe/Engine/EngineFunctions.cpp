/*
    Yelo: Open Sauce SDK
		Halo 2 (Editing Kit) Edition

	See license\OpenSauce\Halo2_CheApe for specific license information
*/
#include "Common/Precompile.hpp"
#include "Engine/EngineFunctions.hpp"

namespace Yelo
{
#define __EL_INCLUDE_ID			__EL_INCLUDE_GAME
#define __EL_INCLUDE_FILE_ID	__EL_GAME_ENGINE_FUNCTIONS
#include "Memory/_EngineLayout.inl"

	namespace EngineFunctions
	{
		API_FUNC_NAKED void* debug_malloc(const size_t ptr_size, cstring file, const uint32 line, const uint32 alignment_bit,
			cstring type_name, cstring subtype_name, cstring memory_name)
		{
			static const uint32 FUNCTION = GET_FUNC_PTR(DEBUG_MALLOC);

			API_FUNC_NAKED_START()
				push	memory_name
				push	subtype_name
				push	type_name
				push	line
				push	file
				push	alignment_bit
				push	ptr_size
				call	FUNCTION
			API_FUNC_NAKED_END_CDECL(7);
		}

		API_FUNC_NAKED void debug_free(void* pointer, cstring file, const uint32 line)
		{
			static const uint32 FUNCTION = GET_FUNC_PTR(DEBUG_FREE);

			API_FUNC_NAKED_START()
				push	line
				push	file
				push	pointer
				call	FUNCTION
			API_FUNC_NAKED_END_CDECL(3);
		}

		API_FUNC_NAKED void* debug_realloc(void* pointer, const size_t new_size, cstring file, const uint32 line, 
			const uint32 alignment_bit,
			cstring type_name, cstring subtype_name, cstring memory_name)
		{
			static const uint32 FUNCTION = GET_FUNC_PTR(DEBUG_REALLOC);

			API_FUNC_NAKED_START()
				push	memory_name
				push	subtype_name
				push	type_name
				push	line
				push	file
				push	alignment_bit
				push	new_size
				push	pointer
				call	FUNCTION
			API_FUNC_NAKED_END_CDECL(8);
		}

		API_FUNC_NAKED void file_reference_create(s_file_reference& reference, long_enum location)
		{
			static const uint32 FUNCTION = GET_FUNC_PTR(FILE_REFERENCE_CREATE);

			API_FUNC_NAKED_START()
				push	location
				push	reference
				call	FUNCTION
			API_FUNC_NAKED_END_CDECL(2);
		}

		API_FUNC_NAKED s_file_reference& file_reference_add_directory(s_file_reference& reference, cstring directory)
		{
			static const uint32 FUNCTION = GET_FUNC_PTR(FILE_REFERENCE_ADD_DIRECTORY);

			API_FUNC_NAKED_START()
				push	directory
				push	reference
				call	FUNCTION
			API_FUNC_NAKED_END_CDECL(2);
		}

		API_FUNC_NAKED s_file_reference& file_reference_set_name(s_file_reference& reference, cstring name)
		{
			static const uint32 FUNCTION = GET_FUNC_PTR(FILE_REFERENCE_SET_NAME);

			API_FUNC_NAKED_START()
				push	name
				push	reference
				call	FUNCTION
			API_FUNC_NAKED_END_CDECL(2);
		}

		s_file_reference& file_reference_create(s_file_reference& reference, cstring directory, cstring name, cstring ext, long_enum location)
		{
			char buffer[256];
			_snprintf_s(buffer, NUMBEROF(buffer)-1, "%s\\%s.%s", directory, name, ext);

			file_reference_create(reference, location);
			file_reference_set_name(reference, buffer);

			return reference;
		}

		API_FUNC_NAKED int16 find_files(long_flags flags, const s_file_reference& directory, int32 maximum_count, s_file_reference references[])
		{
#if PLATFORM_ID != PLATFORM_GUERILLA
			static const uint32 FUNCTION = GET_FUNC_PTR(FIND_FILES);

			API_FUNC_NAKED_START()
				push	references
				push	maximum_count
				push	directory
				push	flags
				call	FUNCTION
			API_FUNC_NAKED_END_CDECL(4);
#else
			API_FUNC_NAKED_START()
				xor		eax, eax
			API_FUNC_NAKED_END(4)
#endif
		}
	};
};