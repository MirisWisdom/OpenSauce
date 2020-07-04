/*
    Yelo: Open Sauce SDK
		Halo 2 (Editing Kit) Edition

	See license\OpenSauce\Halo2_CheApe for specific license information
*/


namespace build_cache_file_ex_hacks
{
	// modifies build_cache_file's code flow to where some data could easily be culled 
	// without performing too many cache fix-ups
	namespace compactable_data_code_flow
	{
		void Initialize()
		{
			static void* POST_BUILD_CACHE_FILE_ADD_STRUCTURE_BSPS = CAST_PTR(void*, 0x588FA6);
			static byte _post_build_cache_file_add_structure_bsps[6] = {
				// offset 588FA6
				// jmp 589089
				// nop
				0xE9,0xDE,0x00,0x00,0x00, 0x90 // jump to code that writes language and bitmap shit
			};
			static void* POST_BUILD_CACHE_FILE_ADD_BITMAP_PIXELS = CAST_PTR(void*, 0x58915C);
			static byte _post_build_cache_file_add_bitmap_pixels[6] = {
				// offset 588FA6
				// jmp 589089
				// nop
				0xE9,0x4B,0xFE,0xFF,0xFF, 0x90 // jump to the code that writes debug info shit
			};
			static void* POST_BUILD_CACHE_FILE_ADD_DEBUG_TAG_NAMES = CAST_PTR(void*, 0x589083);
			static byte _post_build_cache_file_add_debug_tag_names[6] = {
				// offset 588FA6
				// jmp 589089
				// nop
				0xE9,0xDA,0x00,0x00,0x00, 0x90 // jump to code that does the rest of the cache io
			};
			Memory::WriteMemory(POST_BUILD_CACHE_FILE_ADD_STRUCTURE_BSPS,
				_post_build_cache_file_add_structure_bsps, sizeof(_post_build_cache_file_add_structure_bsps));
			Memory::WriteMemory(POST_BUILD_CACHE_FILE_ADD_BITMAP_PIXELS,
				_post_build_cache_file_add_bitmap_pixels, sizeof(_post_build_cache_file_add_bitmap_pixels));
			Memory::WriteMemory(POST_BUILD_CACHE_FILE_ADD_DEBUG_TAG_NAMES,
				_post_build_cache_file_add_debug_tag_names, sizeof(_post_build_cache_file_add_debug_tag_names));
		}
	};
};