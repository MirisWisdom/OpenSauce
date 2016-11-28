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
		static class AddressOf
		{
			public static PlatformAddress NullString = new PlatformAddress
				(0x76D5CA, 0x7F0B50, 0x7FE3F0);
			public static PlatformAddress BlamString = new PlatformAddress // address of the "BLAM!" string
				(0x7D9C1C, 0x8AFCE4, 0x8F49C0);

			public static PlatformAddress TagGroupDefinitions = new PlatformAddress
				(0x901B90, 0x98BAB0, 0x98DA48);
			public static PlatformAddress TagGroupDefinitions2 = new PlatformAddress
				(0x95FA30, 0x9F0AA0, 0x9ED950); // hs tag reference parse's list
			public static PlatformAddress TagGroupDefinitions_TagDatabase = new PlatformAddress
				(0x901868, 0x0, 0x0); // only in guerilla

			/// <summary>Address of the code in each game tool which checks to see if the OS is Vista</summary>
			public static PlatformAddress VistaCheckCode = new PlatformAddress
				(0x655162, 0x7311D2, 0x6EE142);

			#region Che Ape
			// we offset from one byte since we null-terminate the string memory we're hijacking
			public static PlatformAddress CheApeDllString = new PlatformAddress
				(0x8A9268 - 1, 0x9379C0 - 1, 0x933320 - 1);

			/// <summary>LoadLibraryA call in the game tool</summary>
			public static PlatformAddress CheApeLoadLibrary = new PlatformAddress
				(0x76C3E4, 0x7F0204, 0x7FC29C);
			/// <summary>Address where we'll place our initializer code</summary>
			public static PlatformAddress CheApeFunction1 = new PlatformAddress
				(0x7517C0, 0x7D79D0, 0x7E2C10);

			/// <summary>Address where we'll place our hook code which calls <see cref="CheApeFunction1"/></summary>
			public static PlatformAddress CheApeFunction2 = new PlatformAddress
				(0x6ACF6A, 0x75203F, 0x73A3AB);
			#endregion

			public static PlatformAddress LogHeaderVersion = new PlatformAddress
				(0x77F035, 0x814985, 0x7FD825);

			//public static PlatformAddress LogHeaderDateStamp = new PlatformAddress
			//	(0x77F04F, 0x814999, 0x7FD83B);
		};

		partial class Guerilla
		{
			#region Fix Name Ids
			static int[] FixListNameIds = { 
				// global_geometry_section_info_struct
				0x5558F4,
				// bitmap_block
				0x54BAF4,
				0x54BB04,
				// dont_use_me_scenario_environment_object_block
				0x51633C,
				// scenario_object_names_block
				0x5145DC,
				0x5145EC,
				// scenario_players_block
				0x5154E4,
				0x5154F4,
				// scenario_trigger_volume
				0x515684,
				0x515694,
				// scenario_netpoint
				0x514C9C,
				0x514CAC,
				// scenario_tags_v2
				0x518184,
				0x518194,
				0x5181A4,
				0x5181B4,
				0x5181C4,
				0x5181F4,
				// shader_template_block
				0x59FBDC,
				0x59FBEC,
				// shader_template_runtime_external_light_response_index_block
				0x59FA80,
			};
			#endregion

			#region Fix Names
			static int[] FixListNames = {
				// global_damage_section_block
				 0x56652C
				,0x56653C
				// model_group_tags_v2
				,0x5529DC
				,0x5529EC
				,0x552A7C
				,0x552AAC
				// old_object_function_block
				,0x584078
				// weapon_fields_v4
				,0x5838EC
				// sound_globals_fields_v2
				,0x504CDC
				// globals_block
				,0x548B24
				// interface_tag_references
				,0x5472D4
				// multiplayer_information_block
				,0x546674
				// materials_sweeteners_struct
				,0x5348CC
				,0x53493C
				// looping_sound
				,0x506914
				,0x506934
				// light_volume_runtime_offset_block
				,0x5AA048
				// door_encoded_pas_block
				,0x558D04
				// cluster_door_portal_encoded_pas_block
				,0x558DA8
				// ai_deafening_encoded_pas_block
				,0x558E50
				// encoded_cluster_distances_block
				,0x558EF8
				// ai_conversation_block
				,0x56F4E4
				// hs_unit_seat_block
				,0x52231C
				,0x52232C
				// order_completion_condition
				,0x56D83C
				// weapon_hud_meter_block
				,0x5414C4
				// weapon_hud_interface_block
				,0x541DF4
				// unit_hud_interface_block
				,0x540524
				,0x540834
				// unit_hud_auxilary_panel_block
				,0x53FFEC
				// hud_bitmap_widgets
				,0x53E9E4
				,0x53E9F4
				// hud_text_widgets
				,0x53EC04
				,0x53EC14
				// hud_screen_effect_widgets
				,0x53EF24
				,0x53EF34
				// user_interface_shared_globals_definition
				,0x53BBA4
				,0x53BBB4
				,0x53BBC4
				,0x53BCC4
				// variant_setting_edit_reference_block
				,0x538234
				// multiplayer_variant_settings_interface_definition
				,0x5385E4
				,0x538604
				,0x538624
				,0x538654
				,0x538674
				,0x538694
				,0x5386C4
				,0x5386E4
				,0x538704
				,0x538734
				// g_vocalization_fields_v2_5
				,0x57639C
				// ai_conversation_block
				,0x56F4E4
				// multiplayer_runtime_global
				,0x533CF4
				,0x533D04
				,0x533D14
				,0x533D24
				,0x533D34
				,0x533DE4
				,0x533E04
				,0x533E44
				,0x533E54
				,0x533E64
				,0x533E74
				// game_engine_status_response_block
				,0x533654
				// sound_gestalt_playback_block
				,0x52CF98
				// sound_gestalt_scale_block
				,0x52D040
				// sound_gestalt_runtime_permutation_bit_vector_block
				,0x52D510
				// g_sound_permutation_chunk_fields_v2
				,0x505194
				,0x5051A4
				// sound_gestalt_promotions_block
				,0x52D5B8
				// sound_promotion_runtime_timer_block
				,0x505D98
			};
			#endregion

			#region Fix Hack Names
			static int[] FixListHackNames = {
				// g_sound_effect_template
				0x532AB0, 0x3E063C + 1,
				0x532AC0, 0x3E062C + 1,
// 				// g_sound_effect_template_collection
// 				0x532C80, 0x3E0450 + 1,
// 				// multiplayer_variant_settings_interface_definition_block
//				0x538570, 0x3E4154 + 1,
// 				0x538580, 0x3E4134 + 1,
// 				0x538590, 0x3E410C + 1,
				// create_new_variant_struct
				0x538478, 0x3E4218 + 1,
				0x538488, 0x3E4210 + 1,
				0x5384A8, 0x3E4210 + 1,
// 				// animation_pool
// 				0x52F5F8, 0x3DE5CC + 1,
// 				0x52F608, 0x3DE0BC + 1,
// 				0x52F628, 0x3DE0A8 + 1,
// 				0x52F638, 0x3DE098 + 1,
				// packed_data_sizes_struct
				0x52E4C0, 0x3DEA68 + 1,
				0x52E4D0, 0x3DEA50 + 1,
				0x52E4E0, 0x3DEA40 + 1,
				0x52E4F0, 0x3DEA2C + 1,
				0x52E500, 0x3DEA1C + 1,
				0x52E510, 0x3DEA08 + 1,
				0x52E520, 0x3DE9F4 + 1,
				// looping_sound_detail
				0x506420, 0x3922B4 + 1,
// 				// particle_block
// 				0x549958, 0x3FC5F8 + 1,
// 				// platform_sound_playback_struct
// 				0x532028, 0x3DFCCC + 1,
// 				// sound_effect_struct_definition
// 				0x5327F8, 0x3E08CC + 1,
// 				0x532818, 0x3E08AC + 1,
// 				0x532828, 0x3E0898 + 1,
// 				0x532838, 0x3E0884 + 1,
// 				// simple_platform_sound_playback_struct
// 				0x532140, 0x3DFCCC + 1,
// 				// sound_promotion_parameter
// 				0x505F68, 0x3928D4 + 1,
// 				// user_interface_shared_globals_definition_block
// 				0x53BB70, 0x3E5EF8 + 1,
			};
			#endregion
		};
	};
}