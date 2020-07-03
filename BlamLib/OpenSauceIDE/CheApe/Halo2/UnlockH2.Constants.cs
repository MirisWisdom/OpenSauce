/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
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
			/// <summary>DLL name strings for each game tool</summary>
			public static PlatformData CheApeDllString = new PlatformData(
				// "\0H2CheApeDLLG.dll\0"
				BlamLib.Util.ByteStringToArray("004832436865417065444C4C472E646C6C00"),
				// "\0H2CheApeDLLT.dll\0"
				BlamLib.Util.ByteStringToArray("004832436865417065444C4C542E646C6C00"),
				// "\0H2CheApeDLLS.dll\0"
				BlamLib.Util.ByteStringToArray("004832436865417065444C4C532E646C6C00")
				);

			// As long as no one executes havok_debug_start (only possible in sapien anyway) this crap will work
			// TODO: Null havok_debug_start's evaluate function pointer in CheApe

			/// <summary>Initializer code for each game tool</summary>
			/// <remarks>
			/// offset	<see cref="AddressOf.CheApeFunction1"/>
			/// push	<see cref="AddressOf.CheApeDllString"/>
			/// call	ds:[<see cref="AddressOf.CheApeLoadLibrary"/>]
			/// jmp		___security_init_cookie
			/// </remarks>
			public static PlatformData CheApeFunction1 = new PlatformData(
				// push	0x8A9268		// Push our dll string
				// call	ds:[0x76C3E4]	// call LoadLibrary
				// jmp	0x6BA090		// goto ___security_init_cookie
				BlamLib.Util.ByteStringToArray("686A928A00" + "FF15E4C37600" + "E9C088F6FF"),

				// push	0x9379C0		// Push our dll string
				// call	ds:[0x7F0204]	// call LoadLibrary
				// jmp	0x765007		// goto ___security_init_cookie
				BlamLib.Util.ByteStringToArray("68C0799300" + "FF1504027F00" + "E927D6F8FF"),

				// push	0x933320		// Push our dll string
				// call	ds:[0x7FC29C]	// call LoadLibrary
				// jmp	0x74C05D		// goto ___security_init_cookie
				BlamLib.Util.ByteStringToArray("6820339300" + "FF159CC27F00" + "E93D94F6FF")
				);

			/// <summary>Initializer's hook code for each game tool</summary>
			/// <remarks>
			/// offset	<see cref="AddressOf.CheApeFunction2"/>
			/// call	<see cref="AddressOf.CheApeFunction1"/>
			/// </remarks>
			public static PlatformData CheApeFunction2 = new PlatformData(
				// call	0x7517C0		// Call our setup code listed above
				BlamLib.Util.ByteStringToArray("E851480A00"),

				// call	0x7D79D0		// Call our setup code listed above
				BlamLib.Util.ByteStringToArray("E88C590800"),

				// call	0x7E2C10		// Call our setup code listed above
				BlamLib.Util.ByteStringToArray("E860880A00")
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