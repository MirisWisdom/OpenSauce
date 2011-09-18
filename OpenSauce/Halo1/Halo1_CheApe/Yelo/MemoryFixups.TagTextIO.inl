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

#if PLATFORM_ID == PLATFORM_GUERILLA
// The following is a hack which fixes a tag paths being truncated when imported from a text file
//
// The stock guerilla code reads the tag path from the txt file into a tag_string, so paths longer
// than 31 characters get truncated. This hack redirects a local variable pointer to a static
// char buffer 255 in length, and changes the desination buffer size to 255 to match.

char c_memory_fixups::tag_import_path_buffer[0xFF];

void API_FUNC_NAKED c_memory_fixups::Hook_TagImportPathBuffer_Copy()
{
	static uint32 RETN_ADDRESS = 0x42034B;

	_asm {
		push	0xFF
		mov		ecx, offset tag_import_path_buffer
		jmp		RETN_ADDRESS
	};
}
void API_FUNC_NAKED c_memory_fixups::Hook_TagImportPathBuffer_Use()
{
	static uint32 RETN_ADDRESS = 0x42035D;

	_asm {
		mov		edx, offset tag_import_path_buffer
		push	edx
		jmp		RETN_ADDRESS
	};
}

void PLATFORM_API c_memory_fixups::tag_import_fix_truncated_tag_paths()
{
	Memory::WriteRelativeJmp(&Hook_TagImportPathBuffer_Copy, CAST_PTR(void*, 0x420345), true);
	Memory::WriteRelativeJmp(&Hook_TagImportPathBuffer_Use, CAST_PTR(void*, 0x420358), true);
}

void PLATFORM_API c_memory_fixups::tag_import_fix_real_plane_2d_jmp_indices()
{
	static byte* K_REAL_PLANE2D_JMP_INDEX_PTRS[] = {
		CAST_PTR(byte*, 0x41EFA7), CAST_PTR(byte*, 0x41EB4F),
	};

	for(byte i = 0; i < NUMBEROF(K_REAL_PLANE2D_JMP_INDEX_PTRS); i++)
		*K_REAL_PLANE2D_JMP_INDEX_PTRS[i] = 0x0A;
}
#endif

void c_memory_fixups::FixupsInitializeTagTextIOFixes()
{
#if PLATFORM_ID == PLATFORM_GUERILLA
	tag_import_fix_truncated_tag_paths();
	tag_import_fix_real_plane_2d_jmp_indices();
#endif
}