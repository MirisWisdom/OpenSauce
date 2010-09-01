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
			"Builds a cache file with no Open Sauce male enhancements whatsoever. This is the stock tool code running here\n"
			);
	}
	else if( !strcmp(args->tool_command, "build-cache-file-ex") )
	{
		printf_s(
			"Builds a cache file with extra Open Sauce arguments.\n"
			"Arguments:\n"
				"\t<mod-name>\n"
					"\t\tString: The name of the mod this scenario belongs to. This will also be used when naming the new data files.\n"
				"\t<create-anew>\n"
					"\t\tSwitch: Should new data files be created before building the cache?\n"
				"\t<store-resources>\n"
					"\t\tSwitch: Should the scenario's bitmaps/sounds/locale data be stored in the data files?\n"
				"\t<use-memory-upgrades>\n"
					"\t\tSwitch: Does the scenario require Open Sauce's memory upgrades to run?\n"
				"\t<scenario-name>\n"
					"\t\tTagName: Name of the .scenario to use. Just like the stock build-cache-file.\n"
			);
	}
	else if( !strcmp(args->tool_command, "import-shader-postprocess") )
	{
		printf_s(
			"Imports an intermediate file describing shader-postprocessing definitions."
			"Arguments:\n"
				"\t<data-file-name>\n"
					"\t\tDataName: .\n"
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
	else
		printf_s(
			"You didn't supply a tool yelo-command name (or had a typo).\n"
			"Help naughtzee says - NO HELP FOR YOU!\n"
			);
}