/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
﻿using System;
using BlamLib.TagInterface;

namespace BlamLib.Blam.Halo1
{
	partial class TagGroups
	{
		static TagGroups()
		{
			GroupsInitialize();

			obje.Definition = new Tags.object_group().State;
			devi.Definition = new Tags.device_group().State;
			item.Definition = new Tags.item_group().State;
			unit.Definition = new Tags.unit_group().State;
			shdr.Definition = new Tags.shader_group().State;


			actr.Definition = new Tags.actor_group().State;
			actv.Definition = new Tags.actor_variant_group().State;
			ant_.Definition = new Tags.antenna_group().State;
			bipd.Definition = new Tags.biped_group().State;
			bitm.Definition = new Tags.bitmap_group().State;
			trak.Definition = new Tags.camera_track_group().State;
			colo.Definition = new Tags.color_table_group().State;
			cdmg.Definition = new Tags.continuous_damage_effect_group().State;
			cont.Definition = new Tags.contrail_group().State;
			jpt_.Definition = new Tags.damage_effect_group().State;
			deca.Definition = new Tags.decal_group().State;
			dobc.Definition = new Tags.detail_object_collection_group().State;
			
			ctrl.Definition = new Tags.device_control_group().State;
			lifi.Definition = new Tags.device_light_fixture_group().State;
			mach.Definition = new Tags.device_machine_group().State;
			udlg.Definition = new Tags.dialogue_group().State;
			effe.Definition = new Tags.effect_group().State;
			eqip.Definition = new Tags.equipment_group().State;
			flag.Definition = new Tags.flag_group().State;
			fog_.Definition = new Tags.fog_group().State;
			font.Definition = new Tags.font_group().State;
			garb.Definition = new Tags.garbage_group().State;
			mod2.Definition = new Tags.gbxmodel_group().State;
			matg.Definition = new Tags.globals_group().State;
			glw_.Definition = new Tags.glow_group().State;
			grhi.Definition = new Tags.grenade_hud_interface_group().State;
			hudg.Definition = new Tags.hud_globals_group().State;
			hmt_.Definition = new Tags.hud_message_text_group().State;
			hud_.Definition = new Tags.hud_number_group().State;
			devc.Definition = new Tags.input_device_defaults_group().State;
			
			itmc.Definition = new Tags.item_collection_group().State;
			lens.Definition = new Tags.lens_flare_group().State;
			ligh.Definition = new Tags.light_group().State;
			mgs2.Definition = new Tags.light_volume_group().State;
			elec.Definition = new Tags.lightning_group().State;
			foot.Definition = new Tags.material_effects_group().State;
			metr.Definition = new Tags.meter_group().State;
			mode.Definition = new Tags.model_group().State;
			antr.Definition = new Tags.model_animation_group().State;
			coll.Definition = new Tags.model_collision_group().State;
			mply.Definition = new Tags.multiplayer_scenario_description_group().State;
			
			part.Definition = new Tags.particle_group().State;
			pctl.Definition = new Tags.particle_system_group().State;
			phys.Definition = new Tags.physics_group().State;
			plac.Definition = new Tags.placeholder_group().State;
			pphy.Definition = new Tags.point_physics_group().State;
			ngpr.Definition = new Tags.preferences_network_game_group().State;
			proj.Definition = new Tags.projectile_group().State;
			scnr.Definition = new Tags.scenario_group().State;
			sbsp.Definition = new Tags.structure_bsp_group().State;
			scen.Definition = new Tags.scenery_group().State;
			
			seff.Definition = new Tags.shader_effect_group().State;
			senv.Definition = new Tags.shader_environment_group().State;
			soso.Definition = new Tags.shader_model_group().State;
			schi.Definition = new Tags.shader_transparent_chicago_group().State;
			scex.Definition = new Tags.shader_transparent_chicago_extended_group().State;
			sotr.Definition = new Tags.shader_transparent_generic_group().State;
			sgla.Definition = new Tags.shader_transparent_glass_group().State;
			smet.Definition = new Tags.shader_transparent_meter_group().State;
			spla.Definition = new Tags.shader_transparent_plasma_group().State;
			swat.Definition = new Tags.shader_transparent_water_group().State;
			sky_.Definition = new Tags.sky_group().State;
			snd_.Definition = new Tags.sound_group().State;
			snde.Definition = new Tags.sound_environment_group().State;
			lsnd.Definition = new Tags.sound_looping_group().State;
			ssce.Definition = new Tags.sound_scenery_group().State;
			boom.Definition = new Tags.spheroid_group().State;
			str_.Definition = new Tags.string_list_group().State;
			tagc.Definition = new Tags.tag_collection_group().State;
			Soul.Definition = new Tags.ui_widget_collection_group().State;
			DeLa.Definition = new Tags.ui_widget_definition_group().State;
			ustr.Definition = new Tags.unicode_string_list_group().State;
			
			unhi.Definition = new Tags.unit_hud_interface_group().State;
			vehi.Definition = new Tags.vehicle_group().State;
			vcky.Definition = new Tags.virtual_keyboard_group().State;
			weap.Definition = new Tags.weapon_group().State;
			wphi.Definition = new Tags.weapon_hud_interface_group().State;
			rain.Definition = new Tags.weather_particle_system_group().State;
			wind.Definition = new Tags.wind_group().State;

			tag_.Definition = new Tags.tag_database_group().State;

			srscen.Definition = new Tags.scenario_scenery_resource_group().State;
			srbipd.Definition = new Tags.scenario_bipeds_resource_group().State;
			srvehi.Definition = new Tags.scenario_vehicles_resource_group().State;
			sreqip.Definition = new Tags.scenario_equipment_resource_group().State;
			srweap.Definition = new Tags.scenario_weapons_resource_group().State;
			srssce.Definition = new Tags.scenario_sound_scenery_resource_group().State;
			srdgrp.Definition = new Tags.scenario_devices_resource_group().State;
			srdeca.Definition = new Tags.scenario_decals_resource_group().State;
			srcine.Definition = new Tags.scenario_cinematics_resource_group().State;
			srtrgr.Definition = new Tags.scenario_trigger_volumes_resource_group().State;
			srclut.Definition = new Tags.scenario_cluster_data_resource_group().State;
			srhscf.Definition = new Tags.scenario_hs_source_file_group().State;
			srai.Definition = new Tags.scenario_ai_resource_group().State;
			srcmmt.Definition = new Tags.scenario_comments_resource_group().State;

			gelo.Definition = new Tags.project_yellow_globals_group().State;
			yelo.Definition = new Tags.project_yellow_group().State;

			for (int x = 1; x < Groups.Count; x++)
				Groups[x].InitializeHandle(BlamVersion.Halo1, x, false);
		}
	};
}