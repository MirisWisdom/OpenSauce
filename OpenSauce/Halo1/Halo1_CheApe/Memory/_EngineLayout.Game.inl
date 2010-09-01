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


//////////////////////////////////////////////////////////////////////////
// .cpp
#if __EL_INCLUDE_FILE_ID == __EL_GAME_ENGINE_FUNCTIONS
	namespace EngineFunctions
	{
		proc_error error = CAST_PTR(proc_error, PLATFORM_VALUE(0x4206B0, 0x42CB90, 0x417500));

		FUNC_PTR(DISPLAY_ASSERT,	0x42C840, 0x435F10, 0x4F7000);
		FUNC_PTR(DEBUG_MALLOC,		0x446970, 0x43E6B0, 0x501F50);
		FUNC_PTR(DEBUG_FREE,		0x446A80, 0x43E7C0, 0x502060);
		FUNC_PTR(DEBUG_REALLOC,		0x446B50, 0x43E890, 0x502130);

		FUNC_PTR(FILE_REFERENCE_CREATE,			FUNC_PTR_NULL, 0x4366C0, FUNC_PTR_NULL);
		FUNC_PTR(FILE_REFERENCE_ADD_DIRECTORY,	FUNC_PTR_NULL, 0x436930, FUNC_PTR_NULL);
		FUNC_PTR(FILE_REFERENCE_SET_NAME,		FUNC_PTR_NULL, 0x4369B0, FUNC_PTR_NULL);
		FUNC_PTR(FIND_FILES,					FUNC_PTR_NULL, 0x436740, FUNC_PTR_NULL);
	};

#else
	#error Undefined engine layout include for: __EL_INCLUDE_GAME
#endif