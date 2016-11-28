/*
    Yelo: Open Sauce SDK
		Halo 2 (Editing Kit) Edition

	See license\OpenSauce\Halo2_CheApe for specific license information
*/
#include "Common/Precompile.hpp"

#if PLATFORM_ID == PLATFORM_TOOL
#include "Engine/EngineFunctions.hpp"
#include "TagGroups/TagGroups.hpp"

#include <blamlib/Halo2/cache/cache_files.hpp>

// http://www.modacity.net/forums/showthread.php?t=4355
// http://www.modacity.net/forums/showthread.php?t=6270
// animations: http://forums.remnantmods.com/viewtopic.php?f=13&t=1574&st=0&sk=t&sd=a

namespace Yelo
{
	namespace Tool
	{
		enum {
			k_number_of_tool_commands = 0xC,
			k_number_of_tool_tag_utility_classes = 0x44,
		};

		enum tool_command_argument_type : long_enum {
			_tool_command_argument_type_0,
			_tool_command_argument_type_1,
			_tool_command_argument_type_data_directory,
			_tool_command_argument_type_3,
			_tool_command_argument_type_tag_name,
			_tool_command_argument_type_5,
			_tool_command_argument_type_data_file,
			_tool_command_argument_type_file,			// file system path, not limited to data or tags
			_tool_command_argument_type_real,
			_tool_command_argument_type_string,
			_tool_command_argument_type_10,
			_tool_command_argument_type_radio,			// definition holds choices, separated by '|'
			_tool_command_argument_type_boolean,
			_tool_command_argument_type,
		};

//////////////////////////////////////////////////////////////////////////
// Tool commands

		typedef void (PLATFORM_API* _tool_command_proc)(wcstring* arguments);

		struct s_tool_command_argument {
			long_enum	type;
			wcstring	name;
			cstring		definition;
			cstring		help;
		};
		struct s_tool_command {
			wcstring						name;
			_tool_command_proc				import_proc;
			const s_tool_command_argument*	arguments;
			int32							argument_count;
			bool							dont_initialize_game;
			PAD24;
		};

		#include "Tool/LostToolCommands.inl"
		#include "Tool/BuildCacheFile.inl"
		#include "Tool/UpgradeTags.inl"

		#include "Tool/ToolCommandDefinitions.inl"
		static const s_tool_command* yelo_tool_commands[] = {
			//&import_model_render,
 			&import_model_collision,
 			&import_model_physics,
			&import_model,
			&import_model_animations,
 			 import_class_monitor_structures,
			 import_class_monitor_bitmaps,
			&import_sound,
			&build_cache_file_ex_definition,
			&import_tool_upgrade_all_tags,
		};

		int __cdecl s_tool_command_compare(void *, const void* lhs, const void* rhs)
		{
			const s_tool_command* _lhs = *CAST_PTR(const s_tool_command* const*, lhs);
			const s_tool_command* _rhs = *CAST_PTR(const s_tool_command* const*, rhs);

			wcstring l = _lhs->name;
			wcstring r = _rhs->name;
			if(l[0] == L'~') l++;
			if(r[0] == L'~') r++;

			return _wcsicmp(l, r);
		}
		void ToolCommandsInitialize()
		{
			// Tool's original tool commands
			static const s_tool_command* const* tool_import_classes = CAST_PTR(s_tool_command**, 0x97B6EC);
			// The new tool commands list which is made up of tool's
			// and [yelo_tool_commands]
			static s_tool_command* tool_commands[k_number_of_tool_commands + NUMBEROF(yelo_tool_commands)];

			// copy official tool commands
			memcpy_s(tool_commands, sizeof(tool_commands),
				tool_import_classes, sizeof(s_tool_command*) * k_number_of_tool_commands);
			// copy yelo tool commands
			memcpy_s(&tool_commands[k_number_of_tool_commands], sizeof(yelo_tool_commands),
				yelo_tool_commands, sizeof(yelo_tool_commands));
			// Now I know my ABCs
			qsort_s(tool_commands, NUMBEROF(tool_commands), sizeof(s_tool_command*), s_tool_command_compare, NULL);


			static s_tool_command*** tool_commands_references[] = {
				CAST_PTR(s_tool_command***, 0x410596),
				CAST_PTR(s_tool_command***, 0x41060E),
				CAST_PTR(s_tool_command***, 0x412D86),
			};
			static byte* tool_commands_count[] = {
				CAST_PTR(byte*, 0x4105E5),
				CAST_PTR(byte*, 0x412D99),
			};

			// update references to the tool command definitions
			for(int32 x = 0; x < NUMBEROF(tool_commands_references); x++)			*tool_commands_references[x] = &tool_commands[0];
			// update code which contain the tool command definitions count
			for(int32 x = 0; x < NUMBEROF(tool_commands_count); x++)				*tool_commands_count[x] = NUMBEROF(tool_commands);
		}


//////////////////////////////////////////////////////////////////////////
// Tag Utility classes

		typedef bool (PLATFORM_API* _tag_utility_class_proc)(cstring tag_name, datum_index tag_index); // cstring tag_name2, datum_index tag_index2
		struct s_tag_utility_class {
			cstring					name;
			cstring					help;
			tag						group_tag;
			tag						group_tag2;
			int32					unknown1;
			int32					unknown2;
			_tag_utility_class_proc utility_proc;
		};
		static const s_tag_utility_class yelo_tag_utility_classes[] = {
			{}
		};

		int __cdecl s_tag_utility_class_compare(void *, const void* lhs, const void* rhs)
		{
			return strcmp(CAST_PTR(const s_tag_utility_class*,lhs)->name, CAST_PTR(const s_tag_utility_class*,rhs)->name);
		}
		void TagUtilityClassesInitialize()
		{
			// Tool's original tag utilities
			static const s_tag_utility_class* tool_tag_utility_classes = CAST_PTR(s_tag_utility_class*, 0x97A910);
			// The new tag utilities list which is made up of tool's
			// and [yelo_tag_utility_classes]
			static s_tag_utility_class utility_classes[k_number_of_tool_tag_utility_classes + NUMBEROF(yelo_tag_utility_classes)];

			// copy official tag utilities
			memcpy_s(&utility_classes[0], sizeof(utility_classes),
				tool_tag_utility_classes, sizeof(s_tool_command) * k_number_of_tool_tag_utility_classes);
			// copy yelo tag utilities
			memcpy_s(&utility_classes[k_number_of_tool_tag_utility_classes], sizeof(yelo_tag_utility_classes),
				&yelo_tag_utility_classes[0], sizeof(yelo_tag_utility_classes));
			// Now I know my ABCs
			qsort_s(utility_classes, NUMBEROF(utility_classes), sizeof(s_tag_utility_class), s_tag_utility_class_compare, NULL);

			const tag_group* _old_light_group = CAST_PTR(tag_group*, 0x97A798); // lxxx
			const tag_group* _model_blank_group = CAST_PTR(tag_group*, 0x97A8A0); // blah
		}

//////////////////////////////////////////////////////////////////////////

		void Initialize()
		{
			ToolCommandsInitialize();
			//TagUtilityClassesInitialize();
		}

		void Dispose()
		{
		}
	};
};
#endif

/* k_number_of_relative_tag_instances checks

.text:0052EDCC  tag_create                            lea     edx, [esi+2710h]
.text:0052EDE5  tag_create                            cmp     esi, 2710h
.text:00531AEF  tags_unload_all_invalid               cmp     word ptr [esp+40h+var_18], 2710h
.text:0053383D  _tag_groups_initialize                push    2710h           ; int
.text:005338C9  _tag_groups_initialize                push    2710h           ; maximum_count
.text:00533901  _tag_groups_initialize                push    2710h           ; maximum_count
.text:0055179E  sub_551760                            cmp     eax, 2710h
.text:0055B1B5  _build_cache_file_sound_gestalt       cmp     ecx, 2710h
.text:0055B628  _build_cache_file_sound_gestalt       cmp     edx, 2710h
.text:0057F5B6  sub_57F570                            cmp     eax, 2710h
.text:0057F609  sub_57F570                            cmp     eax, 2710h
.text:0057FFBA  cache_file_checksum_tag_mopp_code     cmp     edx, 2710h
.text:0058012A  cache_file_checksum_tag_mopp_code     cmp     edx, 2710h
.text:005801AB  cache_file_checksum_tag_mopp_code     cmp     ecx, 2710h
.text:005802EA  cache_file_checksum_tag_mopp_code     cmp     ecx, 2710h
.text:005803CA  cache_file_checksum_tag_mopp_code     cmp     edx, 2710h
.text:00580B58  _build_cache_file_add_debug_tag_names mov     edi, 2710h
.text:00580C0E  _build_cache_file_add_debug_tag_names mov     eax, 2710h
.text:005817C7  tag_sharing_load_multiplayer_shared   cmp     [ebp+tag_index.count], 2710h
.text:0058182F  tag_sharing_load_multiplayer_shared   mov     esi, 2710h
.text:00581DFC  _language_packing                     cmp     edx, 2710h
.text:00582C38  _postprocess_animation_data           cmp     eax, 2710h
.text:00583301  _build_cache_file_add_geometry_blocks cmp     edx, 2710h
.text:005837A1  _build_cache_file_add_geometry_blocks cmp     ecx, 2710h
.text:00583864  _build_cache_file_add_geometry_blocks cmp     eax, 2710h
.text:00583918  _build_cache_file_add_geometry_blocks cmp     ecx, 2710h
.text:00583B80  _build_cache_file_add_bitmap_pixels   cmp     edx, 2710h
.text:00583F08  _build_cache_file_add_sound_samples   cmp     eax, 2710h
.text:0058786F  _build_cache_file_add_tags            mov     edi, 2710h
.text:005879B4  _build_cache_file_add_tags            mov     eax, 2710h
.text:00587AB8  _build_cache_file_add_tags            mov     esi, 2710h
.text:00587C0B  _build_cache_file_add_tags            cmp     eax, 2710h
.text:005CEE94  sub_5CED20                            cmp     eax, 2710h
*/