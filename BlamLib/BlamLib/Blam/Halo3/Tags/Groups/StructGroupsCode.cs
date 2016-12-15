/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
﻿using System;
using BlamLib.TagInterface;

namespace BlamLib.Blam.Halo3
{
	public static partial class StructGroups
	{
		/// <summary>
		/// cache_file_resource_layout_table_struct
		/// </summary>
		static TagGroup play = new TagGroup("play", "cache_file_resource_layout_table_struct");

		/// <summary>
		/// material_physics_properties_struct
		/// </summary>
		static TagGroup mphp = new TagGroup("mphp", "material_physics_properties_struct");

		/// <summary>
		/// materials_sweeteners_struct
		/// </summary>
		static TagGroup msst = new TagGroup("msst", "materials_sweeteners_struct");

		/// <summary>
		/// sound_playback_parameters_struct
		/// </summary>
		static TagGroup snpl = new TagGroup("snpl", "sound_playback_parameters_struct");

		/// <summary>
		/// sound_scale_modifiers_struct
		/// </summary>
		static TagGroup snsc = new TagGroup("snsc", "sound_scale_modifiers_struct");

		/// <summary>
		/// sound_response_extra_sounds_struct
		/// </summary>
		static TagGroup masd_sound = new TagGroup("masd", "sound_response_extra_sounds_struct");

		/// <summary>
		/// static_spawn_zone_data_struct
		/// </summary>
		static TagGroup sszd = new TagGroup("sszd", "static_spawn_zone_data_struct");

		/// <summary>
		/// mapping_function
		/// </summary>
		static TagGroup MAPP = new TagGroup("MAPP", "mapping_function");

		/// <summary>
		/// unit_camera_struct
		/// </summary>
		static TagGroup uncs = new TagGroup("uncs", "unit_camera_struct");

		/// <summary>
		/// unit_seat_acceleration_struct
		/// </summary>
		static TagGroup usas = new TagGroup("usas", "unit_seat_acceleration_struct");

		/// <summary>
		/// unit_additional_node_names_struct
		/// </summary>
		static TagGroup uHnd = new TagGroup("uHnd", "unit_additional_node_names_struct");

		/// <summary>
		/// unit_boarding_melee_struct
		/// </summary>
		static TagGroup ubms = new TagGroup("ubms", "unit_boarding_melee_struct");

		/// <summary>
		/// unit_boost_struct
		/// </summary>
		static TagGroup _1234 = new TagGroup("!@#$", "unit_boost_struct");

		/// <summary>
		/// unit_lipsync_scales_struct
		/// </summary>
		static TagGroup ulYc = new TagGroup("ulYc", "unit_lipsync_scales_struct");

		/// <summary>
		/// torque_curve_struct
		/// </summary>
		static TagGroup trcv = new TagGroup("trcv", "torque_curve_struct");

		/// <summary>
		/// havok_vehicle_physics_struct
		/// </summary>
		static TagGroup HVPH = new TagGroup("HVPH", "havok_vehicle_physics_struct");

		/// <summary>
		/// biped_lock_on_data_struct
		/// </summary>
		static TagGroup blod = new TagGroup("blod", "biped_lock_on_data_struct");

		/// <summary>
		/// global_collision_bsp_struct
		/// </summary>
		static TagGroup cbsp = new TagGroup("cbsp", "global_collision_bsp_struct");

		/// <summary>
		/// constraint_bodies_struct
		/// </summary>
		static TagGroup csbs = new TagGroup("csbs", "constraint_bodies_struct");

		/// <summary>
		/// animation_aiming_screen_struct
		/// </summary>
		static TagGroup aaim = new TagGroup("aaim", "animation_aiming_screen_struct");


		/// <summary>
		/// sound_playback_parameter_definition
		/// </summary>
		static TagGroup spl1 = new TagGroup("spl1", "sound_playback_parameter_definition");

		/// <summary>
		/// sound_effect_struct_definition
		/// </summary>
		static TagGroup ssfx = new TagGroup("ssfx", "sound_effect_struct_definition");


		/// <summary>
		/// animation_graph_resources_struct
		/// </summary>
		static TagGroup MAgr = new TagGroup("MAgr", "animation_graph_resources_struct");


		/// <summary>
		/// animation_graph_contents_struct
		/// </summary>
		static TagGroup MAgc = new TagGroup("MAgc", "animation_graph_contents_struct");


		/// <summary>
		/// global_structure_physics_struct
		/// </summary>
		static TagGroup spdf = new TagGroup("spdf", "global_structure_physics_struct");


		/// <summary>
		/// simple_platform_sound_playback_struct
		/// </summary>
		static TagGroup plsn_simple = new TagGroup("plsn", "simple_platform_sound_playback_struct");

		/// <summary>
		/// sound_promotion_parameters_struct
		/// </summary>
		static TagGroup snpr = new TagGroup("snpr", "sound_promotion_parameters_struct");
	};
};