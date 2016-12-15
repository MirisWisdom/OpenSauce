/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/

#if PLATFORM_TYPE == PLATFORM_GUERILLA
// The following is a hack which fixes a tag paths being truncated when imported from a text file
//
// The stock guerilla code reads the tag path from the txt file into a tag_string, so paths longer
// than 31 characters get truncated. This hack redirects a local variable pointer to a static
// char buffer 255 in length, and changes the destination buffer size to 255 to match.

char c_memory_fixups::tag_import_path_buffer[Enums::k_max_tag_name_length+1];

void API_FUNC_NAKED c_memory_fixups::Hook_TagImportPathBuffer_Copy()
{
	static const uintptr_t RETN_ADDRESS = 0x42034B;

	using namespace Yelo::Enums;
	_asm {
		push	k_max_tag_name_length
		mov		ecx, offset tag_import_path_buffer
		jmp		RETN_ADDRESS
	};
}
void API_FUNC_NAKED c_memory_fixups::Hook_TagImportPathBuffer_Use()
{
	static const uintptr_t RETN_ADDRESS = 0x42035D;

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

	for(auto ptr : K_REAL_PLANE2D_JMP_INDEX_PTRS)
		*ptr = 0x0A;
}
#endif

void c_memory_fixups::FixupsInitializeTagTextIOFixes()
{
#if PLATFORM_TYPE == PLATFORM_GUERILLA
	tag_import_fix_truncated_tag_paths();
	tag_import_fix_real_plane_2d_jmp_indices();
#endif
}