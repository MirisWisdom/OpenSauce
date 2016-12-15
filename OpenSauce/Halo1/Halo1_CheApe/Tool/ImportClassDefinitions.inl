/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/

	IMPORT_CLASS_DEFINITION("build-cache-file-ex", 
			5, "<mod-name> <create-anew> <store-resources> <use-memory-upgrades> <scenario-name>", 
		&build_cache_file_for_scenario_extended),

	IMPORT_CLASS_DEFINITION("build-cache-file-new", 
			4, "<create-anew> <store-resources> <use-memory-upgrades> <scenario-name>", 
		&build_cache_file_for_scenario_new),

	IMPORT_CLASS_DEFINITION("compile-scripts", 
			1, "<scenario-name>", 
		&scenario_compile_scripts),

	IMPORT_CLASS_DEFINITION("compile-shader-postprocess", 
			1, "<shader-directory>", 
		&compile_shader_postprocess),

	IMPORT_CLASS_DEFINITION("help", 
			1, "<os-tool-command>", 
		&tool_if_you_dont_get_help_at_charter),

	IMPORT_CLASS_DEFINITION("import-structure-lightmap-uvs", 
			2, "<structure-bsp> <obj-file>", 
		&import_structure_lightmap_uvs),

	IMPORT_CLASS_DEFINITION("build-cpp-definition", 
			2, "<tag-group> <add-boost-asserts>", 
		&build_cpp_definition),

	IMPORT_CLASS_DEFINITION("runtime-cache-view", 
			0, "", 
		&runtime_cache_view),

	IMPORT_CLASS_DEFINITION("remove-os-tag-data", 
			3, "<tag-name> <tag-type> <recursive>", 
		&remove_os_tag_data),

	IMPORT_CLASS_DEFINITION("tag-load-test", 
			6, "<tag-name> <group> <prompt-to-continue> <load-non-resolving-refs> <print-size> <verbose>", 
		&tag_load_maintenance),

	IMPORT_CLASS_DEFINITION("build-packed-file", 
			3, "<source-directory> <output-directory> <file-definition-xml>", 
		&PackedFile::build_packed_file),

	// TODO: add new import classes here
