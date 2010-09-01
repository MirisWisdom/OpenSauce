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

	IMPORT_CLASS_DEFINITION("build-cache-file-ex", 
			5, "<mod-name> <create-anew> <store-resources> <use-memory-upgrades> <scenario-name>", 
		&build_cache_file_for_scenario_extended),

	IMPORT_CLASS_DEFINITION("compile-scripts", 
			1, "<scenario-name>", 
		&scenario_compile_scripts),

	IMPORT_CLASS_DEFINITION("import-shader-postprocess", 
			1, "<data-file-name>", 
		&tool_import_shader_postprocess),

	IMPORT_CLASS_DEFINITION("help", 
		1, "<yelo-tool-command>", 
		&tool_if_you_dont_get_help_at_charter),

	IMPORT_CLASS_DEFINITION("import-structure-lightmap-uvs", 
		2, "<structure-bsp> <obj-file>", 
		&import_structure_lightmap_uvs),

	// TODO: add new import classes here
