/*
    Yelo: Open Sauce SDK
		Halo 2 (Editing Kit) Edition

	See license\OpenSauce\Halo2_CheApe for specific license information
*/


static void PLATFORM_API tool_upgrade_all_tags_proc(wcstring* arguments)
{
// 	char cwd[_MAX_PATH];
// 	_getcwd(cwd, NUMBEROF(cwd));
// 	strcat_s(cwd, "\\tags\\");
// 
// 	s_file_reference tags_directory_reference;
// 	EngineFunctions::file_reference_create(tags_directory_reference, 0);
// 	EngineFunctions::file_reference_set_name(tags_directory_reference, cwd);
// 
// 	enum { k_max_results = 1024 * 15 };
// 	s_file_reference* references = YELO_NEW_ARRAY(s_file_reference, k_max_results);
// 	int16 found_count = EngineFunctions::find_files(0, tags_directory_reference, k_max_results, references);
// 	YELO_DELETE(references);

	static cstring k_file_name = "data\\upgrade_list.txt";

	FILE* file;
	if( fopen_s(&file, k_file_name, "rt") != k_errnone )
	{
		wprintf_s(L"couldn't open the '%S' file!\n", k_file_name);
		return;
	}

	FILE* output;
	if( fopen_s(&output, "data\\upgrade_list_results.txt", "wt") != k_errnone )
	{
		wprintf_s(L"couldn't open the results file!\n");
		return;
	}

	int32 count = 0;
	if(fscanf_s(file, "%d\r\n", &count) == 1)
	{
		wprintf_s(L"loading globals...\n");

		long_flags flags = /*Flags::_tag_loading_flag_dont_call_block_postprocess |*/ Flags::_tag_loading_flag_dont_call_tag_postprocess
			/*| Flags::_tag_loading_flag_ignore_invalid_field_data*/; 

// 		datum_index globals = tag_load('matg', "globals\\globals", flags);
// 		*CAST_PTR(datum_index*, 0x9C7504) = globals;
// 		*CAST_PTR(void**, 0xAA00E0) = tag_get('matg', globals);

		wprintf_s(L"upgrading %d tags...\n", count);

		for(int32 x = 0; x < count; x++)
		{
			char group_tag_str[32];
			char name[_MAX_PATH];
			if(fscanf_s(file, "%4s\t%s\r\n", group_tag_str, NUMBEROF(group_tag_str), name, NUMBEROF(name)) == 2)
			{
				tag group_tag = TagGroups::string_to_group_tag(group_tag_str);
				datum_index tag_index = tag_loaded(group_tag, name);
				if(tag_index.IsNull())
					tag_index = tag_load(group_tag, name, flags);

				if(!tag_index.IsNull())
				{
					tag_save(tag_index);
					tag_unload(tag_index);

					fprintf_s(output, "UPGRADED: '%4s\t%s'\n", group_tag_str/*, NUMBEROF(group_tag_str)*/, name/*, NUMBEROF(name)*/);
				}
				else fprintf_s(output, "FAILED: couldn't load the tag '%4s\t%s' for upgrading!\n", group_tag_str/*, NUMBEROF(group_tag_str)*/, name/*, NUMBEROF(name)*/);

				fflush(output);
			}
		}

		//tag_unload(globals);
	}
	fclose(output);
	fclose(file);
}