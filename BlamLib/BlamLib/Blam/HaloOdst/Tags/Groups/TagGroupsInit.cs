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
	partial class TagGroups
	{
		static TagGroups()
		{
			GroupsInitialize();
// 			Lbsp.Definition = new Tags.scenario_lightmap_bsp_data_group().State;
// 			achi.Definition = new Tags.achievements_group().State;
// 			aigl.Definition = new Tags.ai_globals_group().State;
// 			argd.Definition = new Tags.device_arg_device_group().State;
// 			form.Definition = new Tags.formation_group().State;
// 			fwtg.Definition = new Tags.user_interface_fourth_wall_timing_definition_group().State;
// 			gpdt.Definition = new Tags.game_progression_group().State;
// 			pecp.Definition = new Tags.particle_emitter_custom_points_group().State;
// 			rmbk.Definition = new Tags.shader_black_group().State;
// 			rmss.Definition = new Tags.shader_screen_group().State;
// 			smdt.Definition = new Tags.survival_mode_globals_group().State;
// 			spda.Definition = new Tags.scenario_pda_group().State;
// 			sqtm.Definition = new Tags.squad_template_group().State;
// 			ttag.Definition = new Tags.test_tag_group().State;
// 			uttt.Definition = new Tags.tag_template_unit_test_group().State;
// 			vmdx.Definition = new Tags.vision_mode_group().State;

			for (int x = Halo3.TagGroups.Groups.Count; x < Groups.Count; x++)
				Groups[x].InitializeHandle(BlamVersion.HaloOdst, x, false);
		}
	};
}