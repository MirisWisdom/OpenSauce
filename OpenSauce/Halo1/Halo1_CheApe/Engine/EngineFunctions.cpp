/*
    Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition
    Copyright (C) 2005-2010  Kornner Studios (http://kornner.com)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
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
		API_FUNC_NAKED void display_assert(cstring reason, cstring file, const uint32 line, bool halt)
		{
			static uint32 FUNCTION = GET_FUNC_PTR(DISPLAY_ASSERT);

			NAKED_FUNC_START()
				xor		ecx, ecx
				mov		cl, halt
				push	ecx
				push	line
				push	file
				push	reason
				call	FUNCTION
				add		esp, 4 * 4
			NAKED_FUNC_END(4);
		}

		API_FUNC_NAKED void* debug_malloc(const size_t ptr_size, const bool fill_with_garbage, cstring file, const uint32 line)
		{
			static uint32 FUNCTION = GET_FUNC_PTR(DEBUG_MALLOC);

			NAKED_FUNC_START()
				push	line
				push	file
				xor		ecx, ecx
				mov		cl, fill_with_garbage
				push	ecx
				push	ptr_size
				call	FUNCTION
				add		esp, 4 * 4
			NAKED_FUNC_END(4);
		}

		API_FUNC_NAKED void debug_free(void* pointer, cstring file, const uint32 line)
		{
			static uint32 FUNCTION = GET_FUNC_PTR(DEBUG_FREE);

			NAKED_FUNC_START()
				push	line
				push	file
				push	pointer
				call	FUNCTION
				add		esp, 4 * 3
			NAKED_FUNC_END(3);
		}

		API_FUNC_NAKED void* debug_realloc(void* pointer, const size_t new_size, cstring file, const uint32 line)
		{
			static uint32 FUNCTION = GET_FUNC_PTR(DEBUG_REALLOC);

			NAKED_FUNC_START()
				push	line
				push	file
				push	new_size
				push	pointer
				call	FUNCTION
				add		esp, 4 * 4
			NAKED_FUNC_END(4);
		}


		API_FUNC_NAKED void file_reference_create(s_file_reference& reference, long_enum location)
		{
			static const uint32 FUNCTION = GET_FUNC_PTR(FILE_REFERENCE_CREATE);

			NAKED_FUNC_START()
				push	location
				push	reference
				call	FUNCTION
			NAKED_FUNC_END_CDECL(2);
		}

		API_FUNC_NAKED s_file_reference& file_reference_add_directory(s_file_reference& reference, cstring directory)
		{
			static const uint32 FUNCTION = GET_FUNC_PTR(FILE_REFERENCE_ADD_DIRECTORY);

			NAKED_FUNC_START()
				push	directory
				push	reference
				call	FUNCTION
			NAKED_FUNC_END_CDECL(2);
		}

		API_FUNC_NAKED s_file_reference& file_reference_set_name(s_file_reference& reference, cstring name)
		{
			static const uint32 FUNCTION = GET_FUNC_PTR(FILE_REFERENCE_SET_NAME);

			NAKED_FUNC_START()
				push	name
				push	reference
				call	FUNCTION
			NAKED_FUNC_END_CDECL(2);
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
			static const uint32 FUNCTION = GET_FUNC_PTR(FIND_FILES);

			NAKED_FUNC_START()
				push	references
				push	maximum_count
				push	directory
				push	flags
				call	FUNCTION
			NAKED_FUNC_END_CDECL(4);
		}
	};
};