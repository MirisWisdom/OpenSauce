/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
﻿using System;
using BlamLib.TagInterface;

namespace BlamLib.Blam.Halo3
{
	partial class TagGroups
	{
		static TagGroups()
		{
			GroupsInitialize();

// 			obje.Definition = new Tags.object_group().State;
// 			devi.Definition = new Tags.device_group().State;
// 			item.Definition = new Tags.item_group().State;
// 			unit.Definition = new Tags.unit_group().State;
// 			rm__.Definition = new Tags.render_method_group().State;


// 			_rmc.Definition = new Tags.shader_contrail_group().State;
// 			_rmp.Definition = new Tags.shader_particle_group().State;
 			shit.Definition = new Tags.cache_file_sound_group().State;
// 			srscen.Definition = new Tags.scenario_scenery_resource_group().State;
// 			srweap.Definition = new Tags.scenario_weapons_resource_group().State;
// 			srvehi.Definition = new Tags.scenario_vehicles_resource_group().State;
// 			srefsc.Definition = new Tags.scenario_effect_scenery_resource_group().State;
// 			srligh.Definition = new Tags.scenario_lights_resource_group().State;
// 			srbipd.Definition = new Tags.scenario_bipeds_resource_group().State;
// 			sreqip.Definition = new Tags.scenario_equipment_resource_group().State;
// 			srcrea.Definition = new Tags.scenario_creature_resource_group().State;
// 			srssce.Definition = new Tags.scenario_sound_scenery_resource_group().State;
// 			srcmmt.Definition = new Tags.scenario_comments_resource_group().State;
// 			_fx_.Definition = new Tags.sound_effect_template_group().State;
// 			BooM.Definition = new Tags.stereo_system_group().State;
// 			adlg.Definition = new Tags.ai_dialogue_globals_group().State;
// 			srai.Definition = new Tags.scenario_ai_resource_group().State;
// 			ant_.Definition = new Tags.antenna_group().State;
// 			beam.Definition = new Tags.beam_system_group().State;
// 			bink.Definition = new Tags.Bink_group().State;
// 			bipd.Definition = new Tags.biped_group().State;
// 			bitm.Definition = new Tags.bitmap_group().State;
// 			bkey.Definition = new Tags.gui_button_key_definition_group().State;
// 			bloc.Definition = new Tags.crate_group().State;
// 			bmp3.Definition = new Tags.gui_bitmap_widget_definition_group().State;
// 			bsdt.Definition = new Tags.breakable_surface_group().State;
// 			cddf.Definition = new Tags.collision_damage_group().State;
// 			cfxs.Definition = new Tags.camera_fx_settings_group().State;
// 			chad.Definition = new Tags.chud_animation_definition_group().State;
// 			char_.Definition = new Tags.character_group().State;
// 			chdt.Definition = new Tags.chud_definition_group().State;
// 			chgd.Definition = new Tags.chud_globals_definition_group().State;
// 			chmt.Definition = new Tags.chocolate_mountain_new_group().State;
// 			srcine.Definition = new Tags.scenario_cinematics_resource_group().State;
// 			cine.Definition = new Tags.cinematic_group().State;
// 			cisc.Definition = new Tags.cinematic_scene_group().State;
// 			srclut.Definition = new Tags.scenario_cluster_data_resource_group().State;
// 			clwd.Definition = new Tags.cloth_group().State;
// 			cntl.Definition = new Tags.contrail_system_group().State;
// 			coll.Definition = new Tags.collision_model_group().State;
// 			colo.Definition = new Tags.color_table_group().State;
// 			crea.Definition = new Tags.creature_group().State;
// 			crte.Definition = new Tags.cortana_effect_definition_group().State;
// 			ctrl.Definition = new Tags.device_control_group().State;
// 			srcube.Definition = new Tags.scenario_cubemap_resource_group().State;
// 			srdcrs.Definition = new Tags.scenario_decorators_resource_group().State;
// 			dctr.Definition = new Tags.decorator_set_group().State;
// 			srdeca.Definition = new Tags.scenario_decals_resource_group().State;
// 			decs.Definition = new Tags.decal_system_group().State;

// 			devo.Definition = new Tags.cellular_automata_group().State;
// 			srdgrp.Definition = new Tags.scenario_devices_resource_group().State;
// 			dobc.Definition = new Tags.detail_object_collection_group().State;
// 			draw.Definition = new Tags.rasterizer_cache_file_globals_group().State;
// 			drdf.Definition = new Tags.damage_response_definition_group().State;
// 			dsrc.Definition = new Tags.gui_datasource_definition_group().State;
// 			effe.Definition = new Tags.effect_group().State;
// 			effg.Definition = new Tags.effect_globals_group().State;
// 			efsc.Definition = new Tags.effect_scenery_group().State;
// 			egor.Definition = new Tags.screen_effect_group().State;
// 			eqip.Definition = new Tags.equipment_group().State;
// 			flck.Definition = new Tags.flock_group().State;
// 			fldy.Definition = new Tags.fluid_dynamics_group().State;
// 			fog_.Definition = new Tags.planar_fog_group().State;
// 			foot.Definition = new Tags.material_effects_group().State;
// 			fpch.Definition = new Tags.patchy_fog_group().State;
// 			frag.Definition = new Tags.fragment_group().State;
// 			gint.Definition = new Tags.giant_group().State;
// 			glps.Definition = new Tags.global_pixel_shader_group().State;
// 			glvs.Definition = new Tags.global_vertex_shader_group().State;
// 			goof.Definition = new Tags.multiplayer_variant_settings_interface_definition_group().State;
// 			grup.Definition = new Tags.gui_group_widget_definition_group().State;
// 			hlmt.Definition = new Tags.model_group().State;
// 			hlsl.Definition = new Tags.hlsl_include_group().State;
// 			srhscf.Definition = new Tags.scenario_hs_source_file_group().State;

// 			itmc.Definition = new Tags.item_collection_group().State;
// 			jmad.Definition = new Tags.model_animation_graph_group().State;
 			jmrq.Definition = new Tags.sandbox_text_value_pair_definition_group().State;
// 			jpt_.Definition = new Tags.damage_effect_group().State;
// 			lens.Definition = new Tags.lens_flare_group().State;
// 			ligh.Definition = new Tags.light_group().State;
// 			lsnd.Definition = new Tags.sound_looping_group().State;
// 			lst3.Definition = new Tags.gui_list_widget_definition_group().State;
// 			lswd.Definition = new Tags.leaf_system_group().State;
// 			ltvl.Definition = new Tags.light_volume_system_group().State;
// 			mach.Definition = new Tags.device_machine_group().State;
// 			matg.Definition = new Tags.globals_group().State;
// 			mdl3.Definition = new Tags.gui_model_widget_definition_group().State;
// 			mdlg.Definition = new Tags.ai_mission_dialogue_group().State;
// 			metr.Definition = new Tags.meter_group().State;
// 			mffn.Definition = new Tags.muffin_group().State;
 			mode.Definition = new Tags.render_model_group().State;
// 			mply.Definition = new Tags.multiplayer_scenario_description_group().State;
// 			mulg.Definition = new Tags.multiplayer_globals_group().State;
// 			nclt.Definition = new Tags.new_cinematic_lighting_group().State;

// 			perf.Definition = new Tags.performance_throttles_group().State;
// 			phmo.Definition = new Tags.physics_model_group().State;
// 			pixl.Definition = new Tags.pixel_shader_group().State;
 			play.Definition = new Tags.cache_file_resource_layout_table_group().State;
// 			pmdf.Definition = new Tags.particle_model_group().State;
// 			pmov.Definition = new Tags.particle_physics_group().State;
// 			pphy.Definition = new Tags.point_physics_group().State;
// 			proj.Definition = new Tags.projectile_group().State;
// 			prt3.Definition = new Tags.particle_group().State;
// 			rasg.Definition = new Tags.rasterizer_globals_group().State;

// 			rmb_.Definition = new Tags.shader_beam_group().State;
// 			rmcs.Definition = new Tags.shader_custom_group().State;
// 			rmct.Definition = new Tags.shader_cortana_group().State;
// 			rmd_.Definition = new Tags.shader_decal_group().State;
// 			rmdf.Definition = new Tags.render_method_definition_group().State;
// 			rmfl.Definition = new Tags.shader_foliage_group().State;
// 			rmhg.Definition = new Tags.shader_halogram_group().State;
// 			rmlv.Definition = new Tags.shader_light_volume_group().State;
// 			rmop.Definition = new Tags.render_method_option_group().State;
// 			rmsh.Definition = new Tags.shader_group().State;
// 			rmsk.Definition = new Tags.shader_skin_group().State;
// 			rmt2.Definition = new Tags.render_method_template_group().State;
// 			rmtr.Definition = new Tags.shader_terrain_group().State;
// 			rmw_.Definition = new Tags.shader_water_group().State;
// 			rwrd.Definition = new Tags.render_water_ripple_group().State;
// 			sFdT.Definition = new Tags.scenario_faux_data_group().State;
// 			sLdT.Definition = new Tags.scenario_lightmap_group().State;
 			sbsp.Definition = new Tags.scenario_structure_bsp_group().State;
// 			scen.Definition = new Tags.scenery_group().State;
// 			scn3.Definition = new Tags.gui_screen_widget_definition_group().State;
 			scnr.Definition = new Tags.scenario_group().State;
// 			sddt.Definition = new Tags.structure_design_group().State;
// 			sefc.Definition = new Tags.area_screen_effect_group().State;
// 			sfx_.Definition = new Tags.sound_effect_collection_group().State;
// 			sgp_.Definition = new Tags.sound_global_propagation_group().State;
// 			shit2.Definition = new Tags.shield_impact_group().State;
 			sily.Definition = new Tags.text_value_pair_definition_group().State;
// 			skn3.Definition = new Tags.gui_skin_definition_group().State;
// 			srsky_.Definition = new Tags.scenario_sky_references_resource_group().State;
// 			skya.Definition = new Tags.sky_atm_parameters_group().State;
// 			smap.Definition = new Tags.shared_cache_file_layout_group().State;
// 			sncl.Definition = new Tags.sound_classes_group().State;
// 			snd_.Definition = new Tags.sound_group().State;
// 			snde.Definition = new Tags.sound_environment_group().State;
// 			snmx.Definition = new Tags.sound_mix_group().State;
 			spk_.Definition = new Tags.sound_dialogue_constants_group().State;
// 			ssce.Definition = new Tags.sound_scenery_group().State;
// 			srsslt.Definition = new Tags.scenario_structure_lighting_resource_group().State;
// 			stli.Definition = new Tags.scenario_structure_lighting_info_group().State;
// 			stse.Definition = new Tags.structure_seams_group().State;
 			styl.Definition = new Tags.style_group().State;
// 			term.Definition = new Tags.device_terminal_group().State;
// 			trak.Definition = new Tags.camera_track_group().State;
// 			srtrgr.Definition = new Tags.scenario_trigger_volumes_resource_group().State;
// 			txt3.Definition = new Tags.gui_text_widget_definition_group().State;
// 			udlg.Definition = new Tags.dialogue_group().State;
 			ugh_.Definition = new Tags.sound_cache_file_gestalt_group().State;
// 			uise.Definition = new Tags.user_interface_sounds_definition_group().State;
// 			unic.Definition = new Tags.multilingual_unicode_string_list_group().State;

// 			vehc.Definition = new Tags.vehicle_collection_group().State;
// 			vehi.Definition = new Tags.vehicle_group().State;
// 			vtsh.Definition = new Tags.vertex_shader_group().State;
// 			wacd.Definition = new Tags.gui_widget_animation_collection_definition_group().State;
// 			wclr.Definition = new Tags.gui_widget_color_animation_definition_group().State;
// 			weap.Definition = new Tags.weapon_group().State;
// 			wezr.Definition = new Tags.game_engine_settings_definition_group().State;
// 			wfon.Definition = new Tags.gui_widget_font_animation_definition_group().State;
// 			wgan.Definition = new Tags.gui_widget_animation_definition_group().State;
// 			wgtz.Definition = new Tags.user_interface_globals_definition_group().State;
// 			whip.Definition = new Tags.cellular_automata2d_group().State;
// 			wigl.Definition = new Tags.user_interface_shared_globals_definition_group().State;
// 			wind.Definition = new Tags.Wind_group().State;
// 			wpos.Definition = new Tags.gui_widget_position_animation_definition_group().State;
// 			wrot.Definition = new Tags.gui_widget_rotation_animation_definition_group().State;
// 			wscl.Definition = new Tags.gui_widget_scale_animation_definition_group().State;
// 			wspr.Definition = new Tags.gui_widget_sprite_animation_definition_group().State;
// 			wtuv.Definition = new Tags.gui_widget_texture_coordinate_animation_definition_group().State;
 			zone.Definition = new Tags.cache_file_resource_gestalt_group().State;

			for (int x = 1; x < Groups.Count; x++)
				Groups[x].InitializeHandle(BlamVersion.Halo3, x, false);
		}
	};
}