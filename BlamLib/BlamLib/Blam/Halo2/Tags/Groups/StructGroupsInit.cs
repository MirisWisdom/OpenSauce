/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
﻿using System;
using BlamLib.TagInterface;

namespace BlamLib.Blam.Halo2
{
	partial class StructGroups
	{
		static StructGroups()
		{
			GroupsInitialize();
			ires.Definition = new Tags.instantaneous_response_damage_effect_struct().State;
			irem.Definition = new Tags.instantaneous_response_damage_effect_marker_struct().State;
			MTLO.Definition = new Tags.model_target_lock_on_data_struct().State;
			SINF.Definition = new Tags.global_geometry_section_info_struct().State;
			SECT.Definition = new Tags.global_geometry_section_struct().State;
			PDAT.Definition = new Tags.global_geometry_point_data_struct().State;
			ISQI.Definition = new Tags.global_geometry_isq_info_struct().State;
			BLOK.Definition = new Tags.geometry_block_info_struct().State;
			cbsp.Definition = new Tags.global_collision_bsp_struct().State;
			csbs.Definition = new Tags.constraint_bodies_struct().State;
			uncs.Definition = new Tags.unit_camera_struct().State;
			usas.Definition = new Tags.unit_seat_acceleration_struct().State;
			uHnd.Definition = new Tags.unit_additional_node_names_struct().State;
			ubms.Definition = new Tags.unit_boarding_melee_struct().State;
			_1234.Definition = new Tags.unit_boost_struct().State;
			ulYc.Definition = new Tags.unit_lipsync_scales_struct().State;
			blod.Definition = new Tags.biped_lock_on_data_struct().State;
			chgr.Definition = new Tags.character_physics_ground_struct().State;
			chfl.Definition = new Tags.character_physics_flying_struct().State;
			chdd.Definition = new Tags.character_physics_dead_struct().State;
			chsn.Definition = new Tags.character_physics_sentinel_struct().State;
			chpy.Definition = new Tags.character_physics_struct().State;
			trcv.Definition = new Tags.torque_curve_struct().State;
			HVPH.Definition = new Tags.havok_vehicle_physics_struct().State;
			MAPP.Definition = new Tags.mapping_function().State;
			masd_melee.Definition = new Tags.melee_aim_assist_struct().State;
			mdps.Definition = new Tags.melee_damage_parameters_struct().State;
			easd.Definition = new Tags.aim_assist_struct().State;
			wtsf.Definition = new Tags.weapon_tracking_struct().State;
			wSiS.Definition = new Tags.weapon_shared_interface_struct().State;
			wItS.Definition = new Tags.weapon_interface_struct().State;
			wtas.Definition = new Tags.weapon_trigger_autofire_struct().State;
			wtcs.Definition = new Tags.weapon_trigger_charging_struct().State;
			wbde.Definition = new Tags.weapon_barrel_damage_effect_struct().State;
			CLFN.Definition = new Tags.color_function_struct().State;
			SCFN.Definition = new Tags.scalar_function_struct().State;
			PRPS.Definition = new Tags.particle_property_scalar_struct_new().State;
			PRPC.Definition = new Tags.particle_property_color_struct_new().State;
			shtb.Definition = new Tags.tag_block_index_struct().State;
			GPUR.Definition = new Tags.shader_gpu_state_reference_struct().State;
			GPUS.Definition = new Tags.shader_gpu_state_struct().State;
			masd_sound.Definition = new Tags.sound_response_extra_sounds_struct().State;
			mphp.Definition = new Tags.material_physics_properties_struct().State;
			msst.Definition = new Tags.materials_sweeteners_struct().State;
			sd2s.Definition = new Tags.super_detonation_damage_struct().State;
			avlb.Definition = new Tags.angular_velocity_lower_bound_struct().State;
			svis.Definition = new Tags.visibility_struct().State;
			igri.Definition = new Tags.structure_instanced_geometry_render_info_struct().State;
			spdf.Definition = new Tags.global_structure_physics_struct().State;
			obj_.Definition = new Tags.scenario_object_id_struct().State;
			rnli.Definition = new Tags.render_lighting_struct().State;
			sobj.Definition = new Tags.scenario_object_datum_struct().State;
			sper.Definition = new Tags.scenario_object_permutation_struct().State;
			sct3.Definition = new Tags.scenario_scenery_datum_struct_v4().State;
			sunt.Definition = new Tags.scenario_unit_struct().State;
			seqt.Definition = new Tags.scenario_equipment_datum_struct().State;
			swpt.Definition = new Tags.scenario_weapon_datum_struct().State;
			sdvt.Definition = new Tags.scenario_device_struct().State;
			smht.Definition = new Tags.scenario_machine_struct_v3().State;
			sctt.Definition = new Tags.scenario_control_struct().State;
			slft.Definition = new Tags.scenario_light_fixture_struct().State;
			_sc_.Definition = new Tags.sound_scenery_datum_struct().State;
			slit.Definition = new Tags.scenario_light_struct().State;
			ntor.Definition = new Tags.scenario_netgame_equipment_orientation_struct().State;
			sszd.Definition = new Tags.static_spawn_zone_data_struct().State;
			masd_damage.Definition = new Tags.damage_outer_cone_angle_struct().State;
			SFDS.Definition = new Tags.screen_flash_definition_struct().State;
			RFDS.Definition = new Tags.vibration_frequency_definition_struct().State;
			RBDS.Definition = new Tags.vibration_definition_struct().State;
			dsfx.Definition = new Tags.damage_effect_sound_effect_definition().State;
			hwis.Definition = new Tags.hud_widget_inputs_struct().State;
			hwsd.Definition = new Tags.hud_widget_state_definition_struct().State;
			hwef.Definition = new Tags.hud_widget_effect_function_struct().State;
			nhd2.Definition = new Tags.new_hud_dashlight_data_struct().State;
			sebs.Definition = new Tags.screen_effect_bonus_struct().State;
			nhgs_const.Definition = new Tags.global_new_hud_globals_constants_struct().State;
			nhgs.Definition = new Tags.global_new_hud_globals_struct().State;
			cnvs.Definition = new Tags.create_new_variant_struct().State;
			gapu.Definition = new Tags.grenade_and_powerup_struct().State;
			spl1.Definition = new Tags.sound_playback_parameter_definition().State;
			ssfx.Definition = new Tags.sound_effect_struct_definition().State;
			plsn.Definition = new Tags.platform_sound_playback_struct().State;
			prli.Definition = new Tags.primary_light_struct().State;
			scli.Definition = new Tags.secondary_light_struct().State;
			amli.Definition = new Tags.ambient_light_struct().State;
			lmsh.Definition = new Tags.lightmap_shadows_struct().State;
			aaim.Definition = new Tags.animation_aiming_screen_struct().State;
			apds.Definition = new Tags.packed_data_sizes_struct().State;
			qoSS.Definition = new Tags.quantized_orientation_struct().State;
			MAgr.Definition = new Tags.animation_graph_resources_struct().State;
			ANII.Definition = new Tags.animation_index_struct().State;
			ATSS_trans.Definition = new Tags.animation_transition_state_struct().State;
			ATSS_dest.Definition = new Tags.animation_destination_state_struct().State;
			MAgc.Definition = new Tags.animation_graph_contents_struct().State;
			MArt.Definition = new Tags.model_animation_runtime_data_struct().State;
			clpr.Definition = new Tags.cloth_properties().State;
			WNDM.Definition = new Tags.global_wind_model_struct().State;
			sngl.Definition = new Tags.sound_global_mix_struct().State;
			snpl.Definition = new Tags.sound_playback_parameters_struct().State;
			snsc.Definition = new Tags.sound_scale_modifiers_struct().State;
			plsn_simple.Definition = new Tags.simple_platform_sound_playback_struct().State;
			snpr.Definition = new Tags.sound_promotion_parameters_struct().State;

			// ISQI is currently the only one that needs this kind of forcing, but just to be 
			// safe I'm going to double check ALL struct groups here in case I add yet another 
			// deprecated tag definition months from now and forgot (how do you forget goatse?) this hack
			for (int x = 0; x < Groups.Count; x++)
				if(!Groups[x].Definition.IsPostProcessed)
					Groups[x].Definition.PostProcess();

			for (int x = 0; x < Groups.Count; x++)
				Groups[x].InitializeHandle(BlamVersion.Halo2, x, true);
		}
	};
};