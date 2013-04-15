/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#include "Common/Precompile.hpp"
#include "Engine/EngineFunctions.hpp"

#include <blamlib/Halo1/math/periodic_functions.hpp>
#include <blamlib/Halo1/tag_files/files.hpp>

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
				movzx	ecx, halt
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
				movzx	ecx, fill_with_garbage
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
	};

	//////////////////////////////////////////////////////////////////////////
	// math
	namespace blam
	{
		//////////////////////////////////////////////////////////////////////////
		// periodic_functions.c
		real PLATFORM_API periodic_function_evaluate(Enums::periodic_function function_type, real input)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(PERIODIC_FUNCTION_EVALUATE);

			__asm {
				fld		input
				sub		esp, 4 * 2			// allocate space for the 'input' parameter
				fstp	qword ptr [esp]		// store the input on the stack (as a double)
				push	function_type
				call	FUNCTION
				add		esp, 4 * (1+2)		// deallocate. double type consumes two DWORDs of stack
			}
		}
		real PLATFORM_API transition_function_evaluate(Enums::transition_function function_type, real input)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(TRANSITION_FUNCTION_EVALUATE);

			__asm {
				push	input
				push	function_type
				call	FUNCTION
				add		esp, 4 * 2
			}
		}
	};
	//////////////////////////////////////////////////////////////////////////
	// memory
	namespace blam
	{
		using namespace Yelo::Memory;

		//////////////////////////////////////////////////////////////////////////
		// data.c
		API_FUNC_NAKED void* PLATFORM_API datum_try_and_get(s_data_array* data, datum_index index)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(DATUM_TRY_AND_GET);

			__asm	jmp	FUNCTION
		}
		API_FUNC_NAKED void* PLATFORM_API datum_get(s_data_array* data, datum_index index)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(DATUM_GET);

			__asm	jmp	FUNCTION
		}
		API_FUNC_NAKED void PLATFORM_API data_verify(s_data_array* data)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(DATA_VERIFY);

			__asm	jmp	FUNCTION
		}
		API_FUNC_NAKED s_data_array* PLATFORM_API data_new(cstring name, int32 maximum_count, size_t datum_size)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(DATA_NEW);

			__asm	jmp	FUNCTION
		}
		API_FUNC_NAKED void PLATFORM_API data_dispose(s_data_array* data)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(DATA_DISPOSE);

			__asm	jmp	FUNCTION
		}
		API_FUNC_NAKED datum_index PLATFORM_API datum_new_at_index(s_data_array* data, datum_index index)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(DATUM_NEW_AT_INDEX);

			__asm	jmp	FUNCTION
		}
		API_FUNC_NAKED datum_index PLATFORM_API datum_new(s_data_array* data)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(DATUM_NEW);

			__asm	jmp	FUNCTION
		}
		API_FUNC_NAKED void PLATFORM_API datum_delete(s_data_array* data, datum_index index)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(DATUM_DELETE);

			__asm	jmp	FUNCTION
		}
		API_FUNC_NAKED void PLATFORM_API data_delete_all(s_data_array* data)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(DATA_DELETE_ALL);

			__asm	jmp	FUNCTION
		}
		API_FUNC_NAKED void* PLATFORM_API data_iterator_next(s_data_iterator& iterator)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(DATA_ITERATOR_NEXT);

			__asm	jmp	FUNCTION
		}
		API_FUNC_NAKED datum_index PLATFORM_API data_next_index(s_data_array* data, datum_index cursor)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(DATA_NEXT_INDEX);

			__asm	jmp	FUNCTION
		}
		API_FUNC_NAKED void PLATFORM_API data_make_valid(s_data_array* data)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(DATA_MAKE_VALID);

			__asm	jmp	FUNCTION
		}
	};
	//////////////////////////////////////////////////////////////////////////
	// tag_files
	namespace blam
	{
		//////////////////////////////////////////////////////////////////////////
		// files_windows.c
		API_FUNC_NAKED void PLATFORM_API file_reference_create(s_file_reference& reference, long_enum location)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(FILE_REFERENCE_CREATE);

			__asm	jmp	FUNCTION
		}
		API_FUNC_NAKED s_file_reference& PLATFORM_API file_reference_add_directory(s_file_reference& reference, cstring directory)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(FILE_REFERENCE_ADD_DIRECTORY);

			__asm	jmp	FUNCTION
		}
		API_FUNC_NAKED s_file_reference& PLATFORM_API file_reference_set_name(s_file_reference& reference, cstring name)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(FILE_REFERENCE_SET_NAME);

			__asm	jmp	FUNCTION
		}
		API_FUNC_NAKED cstring PLATFORM_API file_reference_get_name(const s_file_reference& reference, long_flags flags, __out char name[Enums::k_maximum_filename_length+1])
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(FILE_REFERENCE_GET_NAME);

			__asm	jmp	FUNCTION
		}

		API_FUNC_NAKED int16 PLATFORM_API find_files(long_flags flags, const s_file_reference& directory, int32 maximum_count, s_file_reference references[])
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(FIND_FILES);

			__asm	jmp	FUNCTION
		}
		API_FUNC_NAKED bool PLATFORM_API file_exists(const s_file_reference& reference)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(FILE_EXISTS);

			__asm	jmp	FUNCTION
		}
	};
};