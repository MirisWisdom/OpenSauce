/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
﻿using System;
using BlamLib.TagInterface;

namespace BlamLib.Blam.Halo2
{
	partial class TagGroups
	{
		static TagGroups()
		{
			GroupsInitialize();
			GroupsInvalidForCacheViewerInitialize();
			GroupsInvalidForExtractionInitialize();

			obje.Definition = new Tags.object_group().State;
			devi.Definition = new Tags.device_group().State;
			item.Definition = new Tags.item_group().State;
			unit.Definition = new Tags.unit_group().State;


			hlmt.Definition = new Tags.model_group().State;
			mode.Definition = new Tags.render_model_group().State;
			coll.Definition = new Tags.collision_model_group().State;
			phmo.Definition = new Tags.physics_model_group().State;
			bitm.Definition = new Tags.bitmap_group().State;
			colo.Definition = new Tags.color_table_group().State;
			unic.Definition = new Tags.multilingual_unicode_string_list_group().State;
			
			bipd.Definition = new Tags.biped_group().State;
			vehi.Definition = new Tags.vehicle_group().State;
			scen.Definition = new Tags.scenery_group().State;
			bloc.Definition = new Tags.crate_group().State;
			crea.Definition = new Tags.creature_group().State;
			phys.Definition = new Tags.physics_group().State;
			
			cont.Definition = new Tags.contrail_group().State;
			weap.Definition = new Tags.weapon_group().State;
			ligh.Definition = new Tags.light_group().State;
			effe.Definition = new Tags.effect_group().State;
			prt3.Definition = new Tags.particle_group().State;
			PRTM.Definition = new Tags.particle_model_group().State;
			pmov.Definition = new Tags.particle_physics_group().State;
			matg.Definition = new Tags.globals_group().State;
			snd_.Definition = new Tags.sound_group().State;
			lsnd.Definition = new Tags.sound_looping_group().State;
			
			eqip.Definition = new Tags.equipment_group().State;
			ant_.Definition = new Tags.antenna_group().State;
			MGS2.Definition = new Tags.light_volume_group().State;
			tdtl.Definition = new Tags.liquid_group().State;
			devo.Definition = new Tags.cellular_automata_group().State;
			whip.Definition = new Tags.cellular_automata2d_group().State;
			BooM.Definition = new Tags.stereo_system_group().State;
			trak.Definition = new Tags.camera_track_group().State;
			proj.Definition = new Tags.projectile_group().State;
			
			mach.Definition = new Tags.device_machine_group().State;
			ctrl.Definition = new Tags.device_control_group().State;
			lifi.Definition = new Tags.device_light_fixture_group().State;
			pphy.Definition = new Tags.point_physics_group().State;
			ltmp.Definition = new Tags.scenario_structure_lightmap_group().State;
			sbsp.Definition = new Tags.scenario_structure_bsp_group().State;
			scnr.Definition = new Tags.scenario_group().State;
			shad.Definition = new Tags.shader_group().State;
			stem.Definition = new Tags.shader_template_group().State;
			slit.Definition = new Tags.shader_light_response_group().State;
			spas.Definition = new Tags.shader_pass_group().State;
			vrtx.Definition = new Tags.vertex_shader_group().State;
			pixl.Definition = new Tags.pixel_shader_group().State;
			DECR.Definition = new Tags.decorator_set_group().State;
			DECP.Definition = new Tags.decorators_group().State;
			sky_.Definition = new Tags.sky_group().State;
			wind.Definition = new Tags.wind_group().State;
			snde.Definition = new Tags.sound_environment_group().State;
			lens.Definition = new Tags.lens_flare_group().State;
			fog.Definition =  new Tags.planar_fog_group().State;
			fpch.Definition = new Tags.patchy_fog_group().State;
			metr.Definition = new Tags.meter_group().State;
			deca.Definition = new Tags.decal_group().State;
			coln.Definition = new Tags.colony_group().State;
			jpt_.Definition = new Tags.damage_effect_group().State;
			udlg.Definition = new Tags.dialogue_group().State;
			itmc.Definition = new Tags.item_collection_group().State;
			vehc.Definition = new Tags.vehicle_collection_group().State;
			wphi.Definition = new Tags.weapon_hud_interface_group().State;
			grhi.Definition = new Tags.grenade_hud_interface_group().State;
			unhi.Definition = new Tags.unit_hud_interface_group().State;
			nhdt.Definition = new Tags.new_hud_definition_group().State;
			hud_.Definition = new Tags.hud_number_group().State;
			hudg.Definition = new Tags.hud_globals_group().State;
			mply.Definition = new Tags.multiplayer_scenario_description_group().State;
			dobc.Definition = new Tags.detail_object_collection_group().State;
			ssce.Definition = new Tags.sound_scenery_group().State;
			hmt_.Definition = new Tags.hud_message_text_group().State;
			wgit.Definition = new Tags.user_interface_screen_widget_definition_group().State;
			skin.Definition = new Tags.user_interface_list_skin_definition_group().State;
			wgtz.Definition = new Tags.user_interface_globals_definition_group().State;
			wigl.Definition = new Tags.user_interface_shared_globals_definition_group().State;
			sily.Definition = new Tags.text_value_pair_definition_group().State;
			goof.Definition = new Tags.multiplayer_variant_settings_interface_definition_group().State;
			foot.Definition = new Tags.material_effects_group().State;
			garb.Definition = new Tags.garbage_group().State;
			styl.Definition = new Tags.style_group().State;
			char_.Definition = new Tags.character_group().State;
			adlg.Definition = new Tags.ai_dialogue_globals_group().State;
			mdlg.Definition = new Tags.ai_mission_dialogue_group().State;
			srscen.Definition = new Tags.scenario_scenery_resource_group().State;
			srbipd.Definition = new Tags.scenario_bipeds_resource_group().State;
			srvehi.Definition = new Tags.scenario_vehicles_resource_group().State;
			sreqip.Definition = new Tags.scenario_equipment_resource_group().State;
			srweap.Definition = new Tags.scenario_weapons_resource_group().State;
			srssce.Definition = new Tags.scenario_sound_scenery_resource_group().State;
			srligh.Definition = new Tags.scenario_lights_resource_group().State;
			srdgrp.Definition = new Tags.scenario_devices_resource_group().State;
			srdeca.Definition = new Tags.scenario_decals_resource_group().State;
			srcine.Definition = new Tags.scenario_cinematics_resource_group().State;
			srtrgr.Definition = new Tags.scenario_trigger_volumes_resource_group().State;
			srclut.Definition = new Tags.scenario_cluster_data_resource_group().State;
			srcrea.Definition = new Tags.scenario_creature_resource_group().State;
			srdcrs.Definition = new Tags.scenario_decorators_resource_group().State;
			srsslt.Definition = new Tags.scenario_structure_lighting_resource_group().State;
			srhscf.Definition = new Tags.scenario_hs_source_file_group().State;
			srai.Definition = new Tags.scenario_ai_resource_group().State;
			srcmmt.Definition = new Tags.scenario_comments_resource_group().State;
			bsdt.Definition = new Tags.breakable_surface_group().State;
			mpdt.Definition = new Tags.material_physics_group().State;
			sncl.Definition = new Tags.sound_classes_group().State;
			mulg.Definition = new Tags.multiplayer_globals_group().State;
			_fx_.Definition = new Tags.sound_effect_template_group().State;
			sfx_.Definition = new Tags.sound_effect_collection_group().State;
			gldf.Definition = new Tags.chocolate_mountain_group().State;
			jmad.Definition = new Tags.model_animation_graph_group().State;
			clwd.Definition = new Tags.cloth_group().State;
			egor.Definition = new Tags.screen_effect_group().State;
			weat.Definition = new Tags.weather_system_group().State;
			snmx.Definition = new Tags.sound_mix_group().State;
			spk_.Definition = new Tags.sound_dialogue_constants_group().State;
			ugh_.Definition = new Tags.sound_cache_file_gestalt_group().State;
			shit.Definition = new Tags.cache_file_sound_group().State;
			mcsr.Definition = new Tags.mouse_cursor_definition_group().State;
			tag_.Definition = new Tags.tag_database_group().State;

			for (int x = 1; x < Groups.Count; x++)
				Groups[x].InitializeHandle(BlamVersion.Halo2, x, false);
		}
	};
}