/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/


// Please. GET HELP SOMEWHERE
static void PLATFORM_API tool_if_you_dont_get_help_at_charter(char* arguments[])
{
	struct s_arguments {
		cstring tool_command;
	}* args = CAST_PTR(s_arguments*, arguments);

	// NOTE: better to indent help strings with (4) spaces. default console tab consumes 8 characters

	if( !strcmp(args->tool_command, "argument-types") )
	{
		printf_s(
			"String: a string value.\n"
			"TagName: Path to a specific tag definition (without the group extension).\n"
			"	Example: globals\\globals\n"
			"DataName: Path to a specific file in the data sub-folder.\n"
			"Switch: can be a boolean value (true/false) or switch (0/1).\n"
			);
	}
	else if( !strcmp(args->tool_command, "build-cache-file") )
	{
		printf_s(
			"Builds a cache file with no Open Sauce male enhancements whatsoever. This\n"
			"is the stock tool code running here\n"
			);
	}
	else if( !strcmp(args->tool_command, "build-cache-file-ex") )
	{
		printf_s(
			"Builds a cache file with extra Open Sauce arguments.\n"
			"Arguments:\n"
			"	<mod-name>\n"
			"		String: The name of the mod this scenario belongs to. This will\n"
			"			also be used when naming the new data-files.\n"
			"	<create-anew>\n"
			"		Switch: Should new data-files be created before building the\n"
			"			cache?\n"
			"	<store-resources>\n"
			"		Switch: Should the scenario's bitmaps/sounds/locale data be\n"
			"			stored in the data-files?\n"
			"	<use-memory-upgrades>\n"
			"		Switch: Does the scenario require Open Sauce's memory upgrades\n"
			"			to run?\n"
			"	<scenario-name>\n"
			"		TagName: Name of the .scenario to use. Just like the stock\n"
			"			build-cache-file.\n"
			);
	}
	else if( !strcmp(args->tool_command, "compile-shader-postprocess") )
	{
		printf_s(
			"Creates shader_postprocess_generic tags from HLSL .fx shaders in the data\n"
			"directory\n"
			"Arguments:\n"
			"	<shader-directory>\n"
			"		Directory: Path that contains the shaders to compile\n"
			);
	}
	else if( !strcmp(args->tool_command, "import-structure-lightmap-uvs") )
	{
		printf_s(
			"Replaces the lightmap UVs of a bsp with custom ones loaded from\n"
			"an obj file. The obj file must have lightmaps grouped by object\n"
			"and have the group index appending the group name (lightmap_0).\n"
			"Best way to ensure this is to export a fresh lightmaps obj using\n"
			"Aether, import it into your modelling program then ONLY edit the\n"
			"UV's to ensure the model matches the bsp.\n"
			"Arguments:\n"
			"	<obj-file>\n"
			"		location of the source obj, relative to the data directory\n"
			"	<structure-bsp>\n"
			"		location of the target bsp, relative to the tags directory\n"
			);
	}
	else if( !strcmp(args->tool_command, "build-cpp-definition") )
	{
		printf_s(
			"Creates a c++ definition header file mapping out the tag structure of a\n"
			"specified tag\n"
			"The resultant header file is saved to the working directory of os_tool\n"
			"Arguments:\n"
			"	<tag-group>\n"
			"		Tag ID: 4 character identifier for the tag type type you want a\n"
			"			definition of eg. \"bitm\"\n"
			"	<add-boost-asserts>\n"
			"		Switch: Controls whether to add \"BOOST_STATIC_ASSERT\" code to\n"
			"			the end of struct to ensure the struct sizes are\n"
			"			correct\n"
			);
	}
	else if( !strcmp(args->tool_command, "runtime-cache-view") )
	{
		printf_s(
			"This tool allows you to view the contents of the tag cache in an\n"
			"executing instance of Halo CE. See the tools own help for more details\n"
			);
	}
	else if( !strcmp(args->tool_command, "remove-os-tag-data") )
	{
		printf_s(
			"Removes OS tag data that would prevent loading in the stock tools\n"
			"Back up your tags before hand to be safe"
			"Arguments:\n"
			"	<tag-name>\n"
			"		TagName: Name of the tag to clean\n"
			"	<tag-type>\n"
			"		TagType: Type of the tag (file extension)\n"
			"	<recursive>\n"
			"		Boolean: When true, all tags refrenced by the specified tag\n"
			"		will also be processed\n"
			);
	}
	else if( !strcmp(args->tool_command, "tag-load-test") )
	{
		printf_s(
			"Validates a tag and ALL of its children can be loaded without error\n"
			"Optionally prints additional diagnostic information\n"
			"Arguments:\n"
			"    <tag-name>\n"
			"        TagName: Root tag's path\n"
			"    <group>\n"
			"        String: Root tag's group name\n"
			"    <prompt-to-continue>\n"
			"        Switch: Prompt to continue for each child tag\n"
			"    <prompt-to-fix-unresolved>\n"
			"        Switch: NOT YET IMPLEMENTED\n"
			"    <load-non-resolving-refs>\n"
			"        Switch: Load all tag_references, even if they're no-resolving\n"
			"    <print-size>\n"
			"        Switch: print the total size of the tag hierarchy as well as the\n"
			"            tag_group memory\n"
			"    <verbose>\n"
			"        Switch: outputs progress/extra information as it processes the tags\n"
			);
	}
	else
		printf_s(
			"You didn't supply a tool yelo-command name (or had a typo).\n"
			"Help naughtzee says - NO HELP FOR YOU!\n"
			);
}