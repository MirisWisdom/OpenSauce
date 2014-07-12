/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#include "Common/Precompile.hpp"
#include "Engine/EngineFunctions.hpp"

#include <blamlib/Halo1/cseries/profile.hpp>
#include <blamlib/Halo1/math/periodic_functions.hpp>
#include <blamlib/Halo1/memory/byte_swapping.hpp>
#include <blamlib/Halo1/memory/memory_pool.hpp>
#include <blamlib/Halo1/saved_games/game_state.hpp>
#include <blamlib/Halo1/tag_files/files.hpp>
#include <blamlib/Halo1/tag_files/tag_files.hpp>
#include <YeloLib/tag_files/tag_groups_base_yelo.hpp>
#include <YeloLib/Halo1/saved_games/game_state_yelo.hpp>

namespace Yelo
{
	namespace GameUI
	{
		struct s_first_person_weapons;
	};
	namespace Scenario
	{
		struct s_scenario_globals;
	};
	namespace TagGroups
	{
		struct predicted_resource;

		struct scenario;

		struct collision_bsp;
		struct structure_bsp;
	};
#define __EL_INCLUDE_ID			__EL_INCLUDE_GAME
#define __EL_INCLUDE_FILE_ID	__EL_GAME_ENGINE_FUNCTIONS
#include "Memory/_EngineLayout.inl"

	namespace GameState
	{
		// TODO: code stolen from Halo1_CE. needs to eventually be setup in YeloLib
		static bool g_yelo_game_state_enabled;
		bool YeloGameStateEnabled()
		{
			return g_yelo_game_state_enabled;
		}
	};

	//////////////////////////////////////////////////////////////////////////
	// cseries
	namespace blam
	{
		//////////////////////////////////////////////////////////////////////////
		// cseries.c
		API_FUNC_NAKED void PLATFORM_API display_assert(cstring reason, cstring file, const uint32 line, bool halt)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(DISPLAY_ASSERT);

			__asm	jmp	FUNCTION
		}
		//////////////////////////////////////////////////////////////////////////
		// debug_memory.c
		API_FUNC_NAKED void PLATFORM_API debug_dump_memory()
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(DEBUG_DUMP_MEMORY);

			__asm	jmp	FUNCTION
		}

		API_FUNC_NAKED void* PLATFORM_API debug_malloc(const size_t ptr_size, const bool fill_with_garbage, cstring file, const uint32 line)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(DEBUG_MALLOC);

			__asm	jmp	FUNCTION
		}

		API_FUNC_NAKED void PLATFORM_API debug_free(void* pointer, cstring file, const uint32 line)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(DEBUG_FREE);

			__asm	jmp	FUNCTION
		}

		API_FUNC_NAKED void* PLATFORM_API debug_realloc(void* pointer, const size_t new_size, cstring file, const uint32 line)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(DEBUG_REALLOC);

			__asm	jmp	FUNCTION
		}
		//////////////////////////////////////////////////////////////////////////
		// cseries/profile
		bool profiling_enabled()	PTR_IMP_GET(g_profiling_enabled);
		API_FUNC_NAKED void PLATFORM_API profile_enter_private(Debug::s_profile_section& section)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(DEBUG_FREE);

			__asm	jmp	FUNCTION
		}
		API_FUNC_NAKED void PLATFORM_API profile_exit_private(Debug::s_profile_section& section)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(DEBUG_FREE);

			__asm	jmp	FUNCTION
		}
	};
	//////////////////////////////////////////////////////////////////////////
	// cache
	namespace blam
	{
		//////////////////////////////////////////////////////////////////////////
		// predicted_resources.c
#if PLATFORM_TYPE == PLATFORM_TOOL
		void predicted_resources_add_resource(TagBlock<TagGroups::predicted_resource>& predicted_resources,
			long_enum resource_type, datum_index tag_index, int32 resource_index = NONE)
		{
			static void* PREDICTED_RESOURCES_ADD_RESOURCE = CAST_PTR(void*, 0x4B94E0);

			typedef void (PLATFORM_API* call_proc)(TagBlock<TagGroups::predicted_resource>&, long_enum, datum_index, int32);
			static call_proc add_predicted_resource = CAST_PTR(call_proc, PREDICTED_RESOURCES_ADD_RESOURCE);

			add_predicted_resource(predicted_resources, resource_type, tag_index, resource_index);
		}
#endif
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
		// byte_swapping.c
		void PLATFORM_API byte_swap_data(Memory::s_byte_swap_definition* definition, void* address,
			int32 data_count)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(BYTE_SWAP_DATA);

			__asm	jmp	FUNCTION
		}
		API_FUNC_NAKED void PLATFORM_API byte_swap_data_explicit(cstring name, int32 size, 
			byte_swap_code_t* codes, int data_count, void *address)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(BYTE_SWAP_DATA_EXPLICIT);

			__asm	jmp	FUNCTION
		}

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
		API_FUNC_NAKED void PLATFORM_API data_verify(const s_data_array* data)
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
		API_FUNC_NAKED datum_index PLATFORM_API data_next_index(const s_data_array* data, datum_index cursor)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(DATA_NEXT_INDEX);

			__asm	jmp	FUNCTION
		}
		API_FUNC_NAKED void PLATFORM_API data_make_valid(s_data_array* data)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(DATA_MAKE_VALID);

			__asm	jmp	FUNCTION
		}

		//////////////////////////////////////////////////////////////////////////
		// memory_pool.c
		API_FUNC_NAKED void PLATFORM_API memory_pool_initialize(Memory::s_memory_pool* pool, cstring name, int32 pool_size)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(MEMORY_POOL_INITIALIZE);

			__asm	jmp	FUNCTION
		}
		API_FUNC_NAKED void PLATFORM_API memory_pool_verify(const Memory::s_memory_pool* pool)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(MEMORY_POOL_VERIFY);

			__asm	jmp	FUNCTION
		}
		API_FUNC_NAKED Memory::s_memory_pool* PLATFORM_API memory_pool_new(cstring name, int32 pool_size)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(MEMORY_POOL_NEW);

			__asm	jmp	FUNCTION
		}
		PLATFORM_VALUE(API_FUNC_NAKED, , API_FUNC_NAKED) void PLATFORM_API memory_pool_delete(Memory::s_memory_pool* pool)
		{
#if PLATFORM_TYPE == PLATFORM_TOOL
			memory_pool_verify(pool);
			memset(pool, 0, sizeof(*pool));
			YELO_FREE(pool); // FREE is preferred here over DELETE since the type is a POD engine type
#else
			static const uintptr_t FUNCTION = GET_FUNC_PTR(MEMORY_POOL_DELETE);

			__asm	jmp	FUNCTION
#endif
		}
		API_FUNC_NAKED bool PLATFORM_API memory_pool_block_allocate(Memory::s_memory_pool* pool, _Inout_ s_memory_pool_block::reference_t reference, int32 size)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(MEMORY_POOL_BLOCK_ALLOCATE);

			__asm	jmp	FUNCTION
		}
		API_FUNC_NAKED void PLATFORM_API memory_pool_block_free(Memory::s_memory_pool* pool, _Inout_ s_memory_pool_block::reference_t reference)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(MEMORY_POOL_BLOCK_FREE);

			__asm	jmp	FUNCTION
		}
		API_FUNC_NAKED void PLATFORM_API memory_pool_defragment(Memory::s_memory_pool* pool)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(MEMORY_POOL_DEFRAGMENT);

			__asm	jmp	FUNCTION
		}
		API_FUNC_NAKED bool PLATFORM_API memory_pool_block_reallocate(Memory::s_memory_pool* pool, _Inout_ s_memory_pool_block::reference_t reference, int32 new_size)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(MEMORY_POOL_BLOCK_REALLOCATE);

			__asm	jmp	FUNCTION
		}
	};
	//////////////////////////////////////////////////////////////////////////
	// shell
	namespace blam
	{
		//////////////////////////////////////////////////////////////////////////
		// shell_windows.c
		API_FUNC_NAKED bool PLATFORM_API shell_get_command_line_argument(cstring param, _Out_opt_ cstring* value = nullptr)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(SHELL_GET_COMMAND_LINE_ARGUMENT);

			__asm	jmp	FUNCTION
		}
	};
	//////////////////////////////////////////////////////////////////////////
	// tag_files
	namespace blam
	{
		//////////////////////////////////////////////////////////////////////////
		// files.c
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
		API_FUNC_NAKED char* PLATFORM_API file_reference_get_name(const s_file_reference& reference, long_flags flags, __out char name[Enums::k_maximum_filename_length+1])
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(FILE_REFERENCE_GET_NAME);

			__asm	jmp	FUNCTION
		}

		API_FUNC_NAKED int16 PLATFORM_API find_files(long_flags flags, const s_file_reference& directory, int32 maximum_count, s_file_reference references[])
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(FIND_FILES);

			__asm	jmp	FUNCTION
		}
		//////////////////////////////////////////////////////////////////////////
		// files_windows.c
		API_FUNC_NAKED void PLATFORM_API file_error(cstring operation, const s_file_reference& reference)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(FILE_ERROR);

			__asm	jmp	FUNCTION
		}
		API_FUNC_NAKED bool PLATFORM_API file_create(const s_file_reference& reference)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(FILE_CREATE);

			__asm	jmp	FUNCTION
		}
		API_FUNC_NAKED bool PLATFORM_API file_delete(const s_file_reference& reference)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(FILE_DELETE);

			__asm	jmp	FUNCTION
		}
		API_FUNC_NAKED bool PLATFORM_API file_exists(const s_file_reference& reference)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(FILE_EXISTS);

			__asm	jmp	FUNCTION
		}
		API_FUNC_NAKED bool PLATFORM_API file_open(s_file_reference& reference, long_flags flags)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(FILE_OPEN);

			__asm	jmp	FUNCTION
		}
		API_FUNC_NAKED bool PLATFORM_API file_close(s_file_reference& reference)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(FILE_CLOSE);

			__asm	jmp	FUNCTION
		}

		API_FUNC_NAKED bool PLATFORM_API file_set_position(s_file_reference& reference, uint32 position)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(FILE_SET_POSITION);

			__asm	jmp	FUNCTION
		}
		API_FUNC_NAKED uint32 PLATFORM_API file_get_eof(const s_file_reference& reference)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(FILE_GET_EOF);

			__asm	jmp	FUNCTION
		}

		API_FUNC_NAKED bool PLATFORM_API file_read(s_file_reference& reference, size_t buffer_size, void* buffer)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(FILE_READ);

			__asm	jmp	FUNCTION
		}
		API_FUNC_NAKED bool PLATFORM_API file_write(s_file_reference& reference, size_t buffer_size, const void* buffer)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(FILE_WRITE);

			__asm	jmp	FUNCTION
		}
		API_FUNC_NAKED bool PLATFORM_API file_read_from_position(s_file_reference& reference, uint32 position,
			size_t buffer_size, void* buffer)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(FILE_READ_FROM_POSITION);

			__asm	jmp	FUNCTION
		}
		API_FUNC_NAKED bool PLATFORM_API file_write_to_position(const s_file_reference& reference, uint32 position,
			size_t buffer_size, const void* buffer)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(FILE_WRITE_TO_POSITION);

			__asm	jmp	FUNCTION
		}
		API_FUNC_NAKED bool PLATFORM_API file_read_only(const s_file_reference& reference)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(FILE_READ_ONLY_);

			__asm	jmp	FUNCTION
		}
	};
};