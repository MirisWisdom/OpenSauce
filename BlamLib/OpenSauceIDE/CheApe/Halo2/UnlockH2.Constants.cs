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
using System.Collections.Generic;

namespace OpenSauceIDE.CheApeInterface
{
	partial class UnlockH2
	{
		static class Constants
		{
			public const int StockTagGroupCount = 120;

			/// <summary>Byte codes to replace the vista check code with</summary>
			public static byte[] VistaCheckCodeNull = new byte[] {
				0x90, 0x90, 0x90, 0x90,
				0x90, 0x90, 0x90, 0x90,
				0x90
			};

			#region Che Ape
			public static PlatformData CheApeDllString = new PlatformData(
				// "\0H2CheApeDLLG.dll\0"
				BlamLib.Util.ByteStringToArray("004832436865417065444C4C472E646C6C00"),
				// "\0H2CheApeDLLT.dll\0"
				BlamLib.Util.ByteStringToArray("004832436865417065444C4C542E646C6C00"),
				// "\0H2CheApeDLLS.dll\0"
				BlamLib.Util.ByteStringToArray("004832436865417065444C4C532E646C6C00")
				);

			/// <summary></summary>
			/// <remarks>
			/// Our DLL is initialized before any tag related code so we don't have to perform any 
			/// sneaky tricks like we did for Halo1's CheApe
			/// </remarks>
			public static PlatformData CheApeFunction = new PlatformData(
				// push	0x8A9268		// Push our dll string
				// call	ds:[0x76C3E4]	// call LoadLibrary
				// retn
				BlamLib.Util.ByteStringToArray("686A928A00FF15E4C37600C3"),

				// push	0x9379C0		// Push our dll string
				// call	ds:[0x7F0204]	// call LoadLibrary
				// retn
				BlamLib.Util.ByteStringToArray("68C0799300FF1504027F00C3"),

				// push	0x933320		// Push our dll string
				// call	ds:[0x7FC29C]	// call LoadLibrary
				// retn
				BlamLib.Util.ByteStringToArray("6820339300FF159CC27F00C3")
				);
			#endregion
		};

		partial class Guerilla
		{
			#region DisallowTagNames
			/// <summary>Tag groups which were disabled in the beta for the unlocked H2Guerilla</summary>
			static string[] DisallowTagNames = {
				"ai_dialogue_globals",
				"ai_mission_dialogue",
				"cache_file_sound",
				"character",
				"collision_model",
				"grenade_hud_interface",
				"model_animation_graph",
				"physics_model",
				"pixel_shader",
				"render_model",
				"scenario_ai_resource",
				"scenario_bipeds_resource",
				"scenario_cinematics_resource",
				"scenario_cluster_data_resource",
				"scenario_comments_resource",
				"scenario_creature_resource",
				"scenario_decals_resource",
				"scenario_decorators_resource",
				"scenario_devices_resource",
				"scenario_equipment_resource",
				"scenario_hs_source_file",
				"scenario_lights_resource",
				"scenario_scenery_resource",
				"scenario_sound_scenery_resource",
				"scenario_structure_lighting_resource",
				"scenario_trigger_volumes_resource",
				"scenario_vehicles_resource",
				"scenario_weapons_resource",
				"sound",
				"sound_cache_file_gestalt",
				"style",
				"text_value_pair_definition",
				"unit_hud_interface",
				"user_interface_globals_definition",
				"user_interface_list_skin_definition",
				"user_interface_screen_widget_definition",
				"user_interface_shared_globals_definition",
				"vertex_shader",
				"weapon_hud_interface",
				//"",
			};
			#endregion
		};
	};
}