/*
    BlamLib: .NET SDK for the Blam Engine

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
﻿using System;
using BlamLib.TagInterface;

namespace BlamLib.Blam.HaloOdst
{
	public static partial class TagGroups
	{
		/// <summary>
		/// scenario_lightmap_bsp_data
		/// </summary>
		public static TagGroup Lbsp = new TagGroup("Lbsp", "scenario_lightmap_bsp_data");

		/// <summary>
		/// achievements
		/// </summary>
		public static TagGroup achi = new TagGroup("achi", "achievements");

		/// <summary>
		/// ai_globals
		/// </summary>
		public static TagGroup aigl = new TagGroup("aigl", "ai_globals");

		/// <summary>
		/// device_arg_device
		/// </summary>
		public static TagGroup argd = new TagGroup("argd", Halo3.TagGroups.devi, "device_arg_device");

		/// <summary>
		/// formation
		/// </summary>
		public static TagGroup form = new TagGroup("form", "formation");

		/// <summary>
		/// user_interface_fourth_wall_timing_definition
		/// </summary>
		public static TagGroup fwtg = new TagGroup("fwtg", "user_interface_fourth_wall_timing_definition");

		/// <summary>
		/// game_progression
		/// </summary>
		public static TagGroup gpdt = new TagGroup("gpdt", "game_progression");

		/// <summary>
		/// particle_emitter_custom_points
		/// </summary>
		public static TagGroup pecp = new TagGroup("pecp", "particle_emitter_custom_points");

		/// <summary>
		/// shader_black
		/// </summary>
		public static TagGroup rmbk = new TagGroup("rmbk", Halo3.TagGroups.rm__, "shader_black");

		/// <summary>
		/// shader_screen
		/// </summary>
		public static TagGroup rmss = new TagGroup("rmss", Halo3.TagGroups.rm__, "shader_screen");

		/// <summary>
		/// survival_mode_globals
		/// </summary>
		public static TagGroup smdt = new TagGroup("smdt", "survival_mode_globals");

		/// <summary>
		/// scenario_pda
		/// </summary>
		public static TagGroup spda = new TagGroup("spda", "scenario_pda");

		/// <summary>
		/// squad_template
		/// </summary>
		public static TagGroup sqtm = new TagGroup("sqtm", "squad_template");

		/// <summary>
		/// test_tag
		/// </summary>
		public static TagGroup ttag = new TagGroup("ttag", "test_tag");

		/// <summary>
		/// tag_template_unit_test
		/// </summary>
		public static TagGroup uttt = new TagGroup("uttt", "tag_template_unit_test");

		/// <summary>
		/// vision_mode
		/// </summary>
		public static TagGroup vmdx = new TagGroup("vmdx", "vision_mode");
	};
}