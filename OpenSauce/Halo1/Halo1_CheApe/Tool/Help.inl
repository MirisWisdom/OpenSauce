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


// Please. GET HELP SOMEWHERE
static void PLATFORM_API tool_if_you_dont_get_help_at_charter(void** arguments)
{
	struct s_arguments {
		cstring tool_command;
	}* args = CAST_PTR(s_arguments*, arguments);

	if( !strcmp(args->tool_command, "argument-types") )
	{
		printf_s(
			"String: a string value.\n"
			"TagName: Path to a specific tag definition (without the group extension).\n"
				"\tExample: globals\\globals\n"
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
				"\t<mod-name>\n"
					"\t\tString: The name of the mod this scenario belongs to. This will\n"
					"\t\t\talso be used when naming the new data files.\n"
				"\t<create-anew>\n"
					"\t\tSwitch: Should new data files be created before building the\n"
					"\t\t\tcache?\n"
				"\t<store-resources>\n"
					"\t\tSwitch: Should the scenario's bitmaps/sounds/locale data be\n"
					"\t\t\tstored in the data files?\n"
				"\t<use-memory-upgrades>\n"
					"\t\tSwitch: Does the scenario require Open Sauce's memory upgrades\n"
					"\t\t\tto run?\n"
				"\t<scenario-name>\n"
					"\t\tTagName: Name of the .scenario to use. Just like the stock\n"
					"\t\t\tbuild-cache-file.\n"
			);
	}
	else if( !strcmp(args->tool_command, "compile-shader-postprocess") )
	{
		printf_s(
			"Creates shader_postprocess_generic tags from HLSL .fx shaders in the data\n"
			"directory\n"
			"Arguments:\n"
				"\t<shader-directory>\n"
					"\t\tDirectory: Path that contains the shaders to compile\n"
			);
	}
	else if( !strcmp(args->tool_command, "import-structure-lightmap-uvs") )
	{
		printf_s(
			"Replaces the lightmap UVs of a bsp with custom ones loaded from\n"
			"an obj file. The obj file must have lightmaps grouped by object\n"
			"and in ascending order (lightmap_0 to lightmap_n). Best way to ensure\n"
			"this is to export a fresh lightmaps obj using Aether, import it into\n"
			"your modelling program then ONLY edit the UV's to ensure the model\n"
			"matches the bsp.\n"
			"Arguments:\n"
				"\t<obj-file>\n"
					"\t\tlocation of the source obj, relative to the data directory\n"
				"\t<structure-bsp>\n"
					"\t\t location of the target bsp, relative to the tags directory\n"
			);
	}
	else if( !strcmp(args->tool_command, "build-cpp-definition") )
	{
		printf_s(
			"Creates a c++ definition header file mapping out the tag structure of a\n"
			"specified tag\n"
			"The resultant header file is saved to the working directory of os_tool\n"
			"Arguments:\n"
				"\t<tag-group>\n"
					"\t\tTag ID: 4 character identifier for the tag type type you want a\n"
					"\t\t\tdefinition of eg. \"bitm\"\n"
				"\t<add-boost-asserts>\n"
					"\t\tSwitch: Controls whether to add \"BOOST_STATIC_ASSERT\" code to\n"
					"\t\t\tthe end of struct to ensure the struct sizes are\n"
					"\t\t\tcorrect\n"
			);
	}
	else
		printf_s(
			"You didn't supply a tool yelo-command name (or had a typo).\n"
			"Help naughtzee says - NO HELP FOR YOU!\n"
			);
}