/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#pragma once

namespace Yelo
{
	class c_memory_fixups
	{
		static struct s_override_paths {
			// this HAS to be lower case
			char root[MAX_PATH];

			struct {
				char path[MAX_PATH];
				char path_with_slash[MAX_PATH];
			}data;

			struct {
				char path[MAX_PATH];
				char path_with_slash[MAX_PATH];

				char temp_uncompressed_cache_file[MAX_PATH];
			}maps;

			// these HAVE to be lower case
			struct {
				char path[MAX_PATH];
				char path_with_slash[MAX_PATH];

				// "tags\"
				char folder_name_with_slash_single[64];
				// ".\tags"
				char folder_name_with_slash_single_relative[64];

				// "\tags\"
				char folder_name_with_slash[64];
				// ".\tags\"
				char folder_name_with_slash_relative[64];
#if PLATFORM_TYPE == PLATFORM_SAPIEN
				// "tags\levels"
				char folder_name_with_levels[64];
				// "*root*\tags\levels
				char root_with_tags_levels[MAX_PATH];
#endif
			}tags;
		}_override_paths; // defined in MemoryFixups.Paths.inl

#if PLATFORM_TYPE == PLATFORM_GUERILLA
		static char tag_import_path_buffer[/*Enums::k_max_tag_name_length+1*/]; // defined in MemoryFixups.TagTextIO.inl
#endif

		static DWORD WINAPI GetCurrentDirectoryHack(
			__in DWORD nBufferLength,
			__out_ecount_part_opt(nBufferLength, return + 1) LPSTR lpBuffer);

#if PLATFORM_TYPE != PLATFORM_GUERILLA
		static char* PLATFORM_API tag_file_index_build_strchr_hack(char* str, char val);
		static void tag_file_index_build_strchr_hack_initialize();
#else
		static void Hook_TagImportPathBuffer_Copy();
		static void Hook_TagImportPathBuffer_Use();
		static void PLATFORM_API c_memory_fixups::tag_import_fix_truncated_tag_paths();
		static void PLATFORM_API tag_import_fix_real_plane_2d_jmp_indices();
#endif

		static void FixupsInitializeDataPaths(cstring data_override);
		static void FixupsInitializeMapsPaths(cstring maps_override);

		static void FixupsInitializeTagPaths(cstring tags_override, cstring tags_name_override);
		static void FixupsInitializeFilePaths();

		static void FixupsInitializeTagTextIOFixes();

	public:
		enum fixup_type : _enum
		{
			_fixup_type_none,
			_fixup_type_replace_memory,
			_fixup_type_replace_pointer,
			_fixup_type_replace_tag_field,
			_fixup_type,
		};

		struct s_fixup
		{
			fixup_type type;
			int16 type_data;
			std::array<void*,3> address; // PLATFORM_TYPE - PLATFORM_EDITOR_START_ID
			std::array<void*,3> definition; // PLATFORM_TYPE - PLATFORM_EDITOR_START_ID
			PAD32;
		}; BOOST_STATIC_ASSERT( sizeof(s_fixup) == 0x20 );

		static void ProcessFixup(s_fixup* fixup);
		static void ProcessFixups();

		// The paths have to be initialized before any engine-modifying CheApe systems are 
		// ran so no code (mainly debug logging) is ran before file paths are updated.
		static void InitializePaths();
		static void DisposePaths();

		static void Initialize();
		static void Dispose();

		static void DebugPaths();
	};
};