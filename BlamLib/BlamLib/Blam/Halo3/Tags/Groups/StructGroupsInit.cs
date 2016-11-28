/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
﻿using System;
using BlamLib.TagInterface;

namespace BlamLib.Blam.Halo3
{
	partial class StructGroups
	{
		static StructGroups()
		{
			GroupsInitialize();
			play.Definition = new Tags.cache_file_resource_layout_table().State;
			mphp.Definition = new Tags.material_physics_properties_struct().State;
			msst.Definition = new Tags.materials_sweeteners_struct().State;
			snpl.Definition = new Tags.sound_playback_parameters_struct().State;
			snsc.Definition = new Tags.sound_scale_modifiers_struct().State;
			masd_sound.Definition = new Tags.sound_response_extra_sounds_struct().State;
			sszd.Definition = new Tags.static_spawn_zone_data_struct().State;
			MAPP.Definition = new Tags.mapping_function().State;
			uncs.Definition = new Tags.unit_camera_struct().State;
			usas.Definition = new Tags.unit_seat_acceleration_struct().State;
			uHnd.Definition = new Tags.unit_additional_node_names_struct().State;
			ubms.Definition = new Tags.unit_boarding_melee_struct().State;
			_1234.Definition = new Tags.unit_boost_struct().State;
			ulYc.Definition = new Tags.unit_lipsync_scales_struct().State;
			HVPH.Definition = new Tags.havok_vehicle_physics_struct().State;
			blod.Definition = new Tags.biped_lock_on_data_struct().State;
			cbsp.Definition = new Tags.global_collision_bsp_struct().State;
			csbs.Definition = new Tags.constraint_bodies_struct().State;
			aaim.Definition = new Tags.animation_aiming_screen_struct().State;

			spl1.Definition = new Tags.sound_playback_parameter_definition().State;
//			ssfx.Definition = new Tags.sound_effect_struct_definition().State;

			MAgr.Definition = new Tags.animation_graph_resources_struct().State;

			MAgc.Definition = new Tags.animation_graph_contents_struct().State;

			spdf.Definition = new Tags.global_structure_physics_struct().State;

			plsn_simple.Definition = new Tags.simple_platform_sound_playback_struct().State;
			snpr.Definition = new Tags.sound_promotion_parameters_struct().State;

			for (int x = 0; x < Groups.Count; x++)
				Groups[x].InitializeHandle(BlamVersion.Halo3, x, true);
		}
	};
};