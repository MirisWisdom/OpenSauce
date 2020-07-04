/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
﻿using System;
using BlamLib.TagInterface;

namespace BlamLib.Blam.HaloOdst
{
	partial class TagGroups
	{
		#region Object Group
		/// <summary>
		/// Object Group
		/// </summary>
		public static TagGroupCollection GroupsObjects;
		static void GroupsObjectsInitialize()
		{
			GroupsObjects = new TagGroupCollection(false,
				Halo3.TagGroups.obje,
				Halo3.TagGroups.bloc,
				Halo3.TagGroups.proj,
				Halo3.TagGroups.scen,
				Halo3.TagGroups.unit, // unit
				Halo3.TagGroups.bipd,
				Halo3.TagGroups.gint,
				Halo3.TagGroups.vehi,
				Halo3.TagGroups.devi, // device
				argd,
				Halo3.TagGroups.ctrl,
				Halo3.TagGroups.mach,
				Halo3.TagGroups.term,
				Halo3.TagGroups.item, // item
				Halo3.TagGroups.eqip,
				Halo3.TagGroups.weap
			);
		}
		#endregion

		#region Device Group
		/// <summary>
		/// Device Group
		/// </summary>
		public static TagGroupCollection GroupsDevices;
		static void GroupsDevicesInitialize()
		{
			GroupsDevices = new TagGroupCollection(false,
				Halo3.TagGroups.devi,
				argd,
				Halo3.TagGroups.ctrl,
				Halo3.TagGroups.mach,
				Halo3.TagGroups.term
			);
		}
		#endregion

		#region Render Method Group
		/// <summary>
		/// Render Method Group
		/// </summary>
		public static TagGroupCollection GroupsShaders;
		static void GroupsShadersInitialize()
		{
			GroupsShaders = new TagGroupCollection(false,
				Halo3.TagGroups.rm__,
				Halo3.TagGroups._rmc,
				Halo3.TagGroups._rmp,
				Halo3.TagGroups.rmb_,
				rmbk,
				Halo3.TagGroups.rmcs,
				Halo3.TagGroups.rmct,
				Halo3.TagGroups.rmd_,
				Halo3.TagGroups.rmfl,
				Halo3.TagGroups.rmhg,
				Halo3.TagGroups.rmlv,
				Halo3.TagGroups.rmsh,
				Halo3.TagGroups.rmsk,
				rmss,
				Halo3.TagGroups.rmtr,
				Halo3.TagGroups.rmw_
			);
		}
		#endregion

		#region Halo 3 Tags Group Collection
		/// <summary>
		/// All tag groups in Halo 3
		/// </summary>
		public static TagGroupCollection Groups;
		static void GroupsInitialize()
		{
			GroupsObjectsInitialize();
			GroupsDevicesInitialize();
			GroupsShadersInitialize();
			Groups = new TagGroupCollection(false,
				TagGroup.Null,

				#region Halo3
				Halo3.TagGroups._rmc,
				Halo3.TagGroups._rmp,
				Halo3.TagGroups.shit,
				Halo3.TagGroups.srscen,
				Halo3.TagGroups.srweap,
				Halo3.TagGroups.srvehi,
				Halo3.TagGroups.srefsc,
				Halo3.TagGroups.srligh,
				Halo3.TagGroups.srbipd,
				Halo3.TagGroups.sreqip,
				Halo3.TagGroups.srcrea,
				Halo3.TagGroups.srssce,
				Halo3.TagGroups.srcmmt,
				Halo3.TagGroups._fx_,
				Halo3.TagGroups.BooM,
				Halo3.TagGroups.adlg,
				Halo3.TagGroups.srai,
				Halo3.TagGroups.ant_,
				Halo3.TagGroups.beam,
				Halo3.TagGroups.bink,
				Halo3.TagGroups.bipd,
				Halo3.TagGroups.bitm,
				Halo3.TagGroups.bkey,
				Halo3.TagGroups.bloc,
				Halo3.TagGroups.bmp3,
				Halo3.TagGroups.bsdt,
				Halo3.TagGroups.cddf,
				Halo3.TagGroups.cfxs,
				Halo3.TagGroups.chad,
				Halo3.TagGroups.char_,
				Halo3.TagGroups.chdt,
				Halo3.TagGroups.chgd,
				Halo3.TagGroups.chmt,
				Halo3.TagGroups.srcine,
				Halo3.TagGroups.cine,
				Halo3.TagGroups.cisc,
				Halo3.TagGroups.srclut,
				Halo3.TagGroups.clwd,
				Halo3.TagGroups.cntl,
				Halo3.TagGroups.coll,
				Halo3.TagGroups.colo,
				Halo3.TagGroups.crea,
				Halo3.TagGroups.crte,
				Halo3.TagGroups.ctrl,
				Halo3.TagGroups.srcube,
				Halo3.TagGroups.srdcrs,
				Halo3.TagGroups.dctr,
				Halo3.TagGroups.srdeca,
				Halo3.TagGroups.decs,
				Halo3.TagGroups.devi,
				Halo3.TagGroups.devo,
				Halo3.TagGroups.srdgrp,
				Halo3.TagGroups.dobc,
				Halo3.TagGroups.draw,
				Halo3.TagGroups.drdf,
				Halo3.TagGroups.dsrc,
				Halo3.TagGroups.effe,
				Halo3.TagGroups.effg,
				Halo3.TagGroups.efsc,
				Halo3.TagGroups.egor,
				Halo3.TagGroups.eqip,
				Halo3.TagGroups.flck,
				Halo3.TagGroups.fldy,
				Halo3.TagGroups.fog_,
				Halo3.TagGroups.foot,
				Halo3.TagGroups.fpch,
				Halo3.TagGroups.frag,
				Halo3.TagGroups.gint,
				Halo3.TagGroups.glps,
				Halo3.TagGroups.glvs,
				Halo3.TagGroups.goof,
				Halo3.TagGroups.grup,
				Halo3.TagGroups.hlmt,
				Halo3.TagGroups.hlsl,
				Halo3.TagGroups.srhscf,
				Halo3.TagGroups.item,
				Halo3.TagGroups.itmc,
				Halo3.TagGroups.jmad,
				Halo3.TagGroups.jmrq,
				Halo3.TagGroups.jpt_,
				Halo3.TagGroups.lens,
				Halo3.TagGroups.ligh,
				Halo3.TagGroups.lsnd,
				Halo3.TagGroups.lst3,
				Halo3.TagGroups.lswd,
				Halo3.TagGroups.ltvl,
				Halo3.TagGroups.mach,
				Halo3.TagGroups.matg,
				Halo3.TagGroups.mdl3,
				Halo3.TagGroups.mdlg,
				Halo3.TagGroups.metr,
				Halo3.TagGroups.mffn,
				Halo3.TagGroups.mode,
				Halo3.TagGroups.mply,
				Halo3.TagGroups.mulg,
				Halo3.TagGroups.nclt,
				Halo3.TagGroups.obje,
				Halo3.TagGroups.perf,
				Halo3.TagGroups.phmo,
				Halo3.TagGroups.pixl,
				Halo3.TagGroups.play,
				Halo3.TagGroups.pmdf,
				Halo3.TagGroups.pmov,
				Halo3.TagGroups.pphy,
				Halo3.TagGroups.proj,
				Halo3.TagGroups.prt3,
				Halo3.TagGroups.rasg,
				Halo3.TagGroups.rm__,
				Halo3.TagGroups.rmb_,
				Halo3.TagGroups.rmcs,
				Halo3.TagGroups.rmct,
				Halo3.TagGroups.rmd_,
				Halo3.TagGroups.rmdf,
				Halo3.TagGroups.rmfl,
				Halo3.TagGroups.rmhg,
				Halo3.TagGroups.rmlv,
				Halo3.TagGroups.rmop,
				Halo3.TagGroups.rmsh,
				Halo3.TagGroups.rmsk,
				Halo3.TagGroups.rmt2,
				Halo3.TagGroups.rmtr,
				Halo3.TagGroups.rmw_,
				Halo3.TagGroups.rwrd,
				Halo3.TagGroups.sFdT,
				Halo3.TagGroups.sLdT,
				Halo3.TagGroups.sbsp,
				Halo3.TagGroups.scen,
				Halo3.TagGroups.scn3,
				Halo3.TagGroups.scnr,
				Halo3.TagGroups.sddt,
				Halo3.TagGroups.sefc,
				Halo3.TagGroups.sfx_,
				Halo3.TagGroups.sgp_,
				Halo3.TagGroups.shit2,
				Halo3.TagGroups.sily,
				Halo3.TagGroups.skn3,
				Halo3.TagGroups.srsky_,
				Halo3.TagGroups.skya,
				Halo3.TagGroups.smap,
				Halo3.TagGroups.sncl,
				Halo3.TagGroups.snd_,
				Halo3.TagGroups.snde,
				Halo3.TagGroups.snmx,
				Halo3.TagGroups.spk_,
				Halo3.TagGroups.ssce,
				Halo3.TagGroups.srsslt,
				Halo3.TagGroups.stli,
				Halo3.TagGroups.stse,
				Halo3.TagGroups.styl,
				Halo3.TagGroups.term,
				Halo3.TagGroups.trak,
				Halo3.TagGroups.srtrgr,
				Halo3.TagGroups.txt3,
				Halo3.TagGroups.udlg,
				Halo3.TagGroups.ugh_,
				Halo3.TagGroups.uise,
				Halo3.TagGroups.unic,
				Halo3.TagGroups.unit,
				Halo3.TagGroups.vehc,
				Halo3.TagGroups.vehi,
				Halo3.TagGroups.vtsh,
				Halo3.TagGroups.wacd,
				Halo3.TagGroups.wclr,
				Halo3.TagGroups.weap,
				Halo3.TagGroups.wezr,
				Halo3.TagGroups.wfon,
				Halo3.TagGroups.wgan,
				Halo3.TagGroups.wgtz,
				Halo3.TagGroups.whip,
				Halo3.TagGroups.wigl,
				Halo3.TagGroups.wind,
				Halo3.TagGroups.wpos,
				Halo3.TagGroups.wrot,
				Halo3.TagGroups.wscl,
				Halo3.TagGroups.wspr,
				Halo3.TagGroups.wtuv,
				Halo3.TagGroups.zone,
				#endregion

				Lbsp,
				achi,
				aigl,
				argd,
				form,
				fwtg,
				gpdt,
				pecp,
				rmbk,
				rmss,
				smdt,
				spda,
				sqtm,
				ttag,
				uttt,
				vmdx
			);
		}

		/// <summary>
		/// Constant indices to each tag group in <see cref="Groups"/>
		/// </summary>
		public enum Enumerated : int
		{
			/// <summary>any tag</summary>
			null_,

			#region Halo3
			/// <summary>shader_contrail</summary>
			_rmc,
			/// <summary>shader_particle</summary>
			_rmp,
			/// <summary>cache_file_sound</summary>
			shit,
			/// <summary>scenario_scenery_resource</summary>
			srscen,
			/// <summary>scenario_weapons_resource</summary>
			srweap,
			/// <summary>scenario_vehicles_resource</summary>
			srvehi,
			/// <summary>scenario_effect_scenery_resource</summary>
			srefsc,
			/// <summary>scenario_lights_resource</summary>
			srligh,
			/// <summary>scenario_bipeds_resource</summary>
			srbipd,
			/// <summary>scenario_equipment_resource</summary>
			sreqip,
			/// <summary>scenario_creature_resource</summary>
			srcrea,
			/// <summary>scenario_sound_scenery_resource</summary>
			srssce,
			/// <summary>scenario_comments_resource</summary>
			srcmmt,
			/// <summary>sound_effect_template</summary>
			_fx_,
			/// <summary>stereo_system</summary>
			BooM,
			/// <summary>ai_dialogue_globals</summary>
			adlg,
			/// <summary>scenario_ai_resource</summary>
			srai,
			/// <summary>antenna</summary>
			ant_,
			/// <summary>beam_system</summary>
			beam,
			/// <summary>bink</summary>
			bink,
			/// <summary>biped</summary>
			bipd,
			/// <summary>bitmap</summary>
			bitm,
			/// <summary>gui_button_key_definition</summary>
			bkey,
			/// <summary>crate</summary>
			bloc,
			/// <summary>gui_bitmap_widget_definition</summary>
			bmp3,
			/// <summary>breakable_surface</summary>
			bsdt,
			/// <summary>collision_damage</summary>
			cddf,
			/// <summary>camera_fx_settings</summary>
			cfxs,
			/// <summary>chud_animation_definition</summary>
			chad,
			/// <summary>character</summary>
			char_,
			/// <summary>chud_definition</summary>
			chdt,
			/// <summary>chud_globals_definition</summary>
			chgd,
			/// <summary>chocolate_mountain_new</summary>
			chmt,
			/// <summary>scenario_cinematics_resource</summary>
			srcine,
			/// <summary>cinematic</summary>
			cine,
			/// <summary>cinematic_scene</summary>
			cisc,
			/// <summary>scenario_cluster_data_resource</summary>
			srclut,
			/// <summary>cloth</summary>
			clwd,
			/// <summary>contrail_system</summary>
			cntl,
			/// <summary>collision_model</summary>
			coll,
			/// <summary>color_table</summary>
			colo,
			/// <summary>creature</summary>
			crea,
			/// <summary>cortana_effect_definition</summary>
			crte,
			/// <summary>device_control</summary>
			ctrl,
			/// <summary>scenario_cubemap_resource</summary>
			srcube,
			/// <summary>scenario_decorators_resource</summary>
			srdcrs,
			/// <summary>decorator_set</summary>
			dctr,
			/// <summary>scenario_decals_resource</summary>
			srdeca,
			/// <summary>decal_system</summary>
			decs,
			/// <summary>device</summary>
			devi,
			/// <summary>cellular_automata</summary>
			devo,
			/// <summary>scenario_devices_resource</summary>
			srdgrp,
			/// <summary>detail_object_collection</summary>
			dobc,
			/// <summary>rasterizer_cache_file_globals</summary>
			draw,
			/// <summary>damage_response_definition</summary>
			drdf,
			/// <summary>gui_datasource_definition</summary>
			dsrc,
			/// <summary>effect</summary>
			effe,
			/// <summary>effect_globals</summary>
			effg,
			/// <summary>effect_scenery</summary>
			efsc,
			/// <summary>screen_effect</summary>
			egor,
			/// <summary>equipment</summary>
			eqip,
			/// <summary>flock</summary>
			flck,
			/// <summary>fluid_dynamics</summary>
			fldy,
			/// <summary>planar_fog</summary>
			fog_,
			/// <summary>material_effects</summary>
			foot,
			/// <summary>patchy_fog</summary>
			fpch,
			/// <summary>fragment</summary>
			frag,
			/// <summary>giant</summary>
			gint,
			/// <summary>global_pixel_shader</summary>
			glps,
			/// <summary>global_vertex_shader</summary>
			glvs,
			/// <summary>multiplayer_variant_settings_interface_definition</summary>
			goof,
			/// <summary>gui_group_widget_definition</summary>
			grup,
			/// <summary>model</summary>
			hlmt,
			/// <summary>hlsl_include</summary>
			hlsl,
			/// <summary>scenario_hs_source_file</summary>
			srhscf,
			/// <summary>item</summary>
			item,
			/// <summary>item_collection</summary>
			itmc,
			/// <summary>model_animation_graph</summary>
			jmad,
			/// <summary>sandbox_text_value_pair_definition</summary>
			jmrq,
			/// <summary>damage_effect</summary>
			jpt_,
			/// <summary>lens_flare</summary>
			lens,
			/// <summary>light</summary>
			ligh,
			/// <summary>sound_looping</summary>
			lsnd,
			/// <summary>gui_list_widget_definition</summary>
			lst3,
			/// <summary>leaf_system</summary>
			lswd,
			/// <summary>light_volume_system</summary>
			ltvl,
			/// <summary>device_machine</summary>
			mach,
			/// <summary>globals</summary>
			matg,
			/// <summary>gui_model_widget_definition</summary>
			mdl3,
			/// <summary>ai_mission_dialogue</summary>
			mdlg,
			/// <summary>meter</summary>
			metr,
			/// <summary>muffin</summary>
			mffn,
			/// <summary>render_model</summary>
			mode,
			/// <summary>multiplayer_scenario_description</summary>
			mply,
			/// <summary>multiplayer_globals</summary>
			mulg,
			/// <summary>new_cinematic_lighting</summary>
			nclt,
			/// <summary>object</summary>
			obje,
			/// <summary>performance_throttles</summary>
			perf,
			/// <summary>physics_model</summary>
			phmo,
			/// <summary>pixel_shader</summary>
			pixl,
			/// <summary>cache_file_resource_layout_table</summary>
			play,
			/// <summary>particle_model</summary>
			pmdf,
			/// <summary>particle_physics</summary>
			pmov,
			/// <summary>point_physics</summary>
			pphy,
			/// <summary>projectile</summary>
			proj,
			/// <summary>particle</summary>
			prt3,
			/// <summary>rasterizer_globals</summary>
			rasg,
			/// <summary>render_method</summary>
			rm__,
			/// <summary>shader_beam</summary>
			rmb_,
			/// <summary>shader_custom</summary>
			rmcs,
			/// <summary>shader_cortana</summary>
			rmct,
			/// <summary>shader_decal</summary>
			rmd_,
			/// <summary>render_method_definition</summary>
			rmdf,
			/// <summary>shader_foliage</summary>
			rmfl,
			/// <summary>shader_halogram</summary>
			rmhg,
			/// <summary>shader_light_volume</summary>
			rmlv,
			/// <summary>render_method_option</summary>
			rmop,
			/// <summary>shader</summary>
			rmsh,
			/// <summary>shader_skin</summary>
			rmsk,
			/// <summary>render_method_template</summary>
			rmt2,
			/// <summary>shader_terrain</summary>
			rmtr,
			/// <summary>shader_water</summary>
			rmw_,
			/// <summary>render_water_ripple</summary>
			rwrd,
			/// <summary>scenario_faux_data</summary>
			sFdT,
			/// <summary>scenario_lightmap</summary>
			sLdT,
			/// <summary>scenario_structure_bsp</summary>
			sbsp,
			/// <summary>scenery</summary>
			scen,
			/// <summary>gui_screen_widget_definition</summary>
			scn3,
			/// <summary>scenario</summary>
			scnr,
			/// <summary>structure_design</summary>
			sddt,
			/// <summary>area_screen_effect</summary>
			sefc,
			/// <summary>sound_effect_collection</summary>
			sfx_,
			/// <summary>sound_global_propagation</summary>
			sgp_,
			/// <summary>shield_impact</summary>
			shit2,
			/// <summary>text_value_pair_definition</summary>
			sily,
			/// <summary>gui_skin_definition</summary>
			skn3,
			/// <summary>scenario_sky_references_resource</summary>
			srsky_,
			/// <summary>sky_atm_parameters</summary>
			skya,
			/// <summary>shared_cache_file_layout</summary>
			smap,
			/// <summary>sound_classes</summary>
			sncl,
			/// <summary>sound</summary>
			snd_,
			/// <summary>sound_environment</summary>
			snde,
			/// <summary>sound_mix</summary>
			snmx,
			/// <summary>sound_dialogue_constants</summary>
			spk_,
			/// <summary>sound_scenery</summary>
			ssce,
			/// <summary>scenario_structure_lighting_resource</summary>
			srsslt,
			/// <summary>scenario_structure_lighting_info</summary>
			stli,
			/// <summary>structure_seams</summary>
			stse,
			/// <summary>style</summary>
			styl,
			/// <summary>device_terminal</summary>
			term,
			/// <summary>camera_track</summary>
			trak,
			/// <summary>scenario_trigger_volumes_resource</summary>
			srtrgr,
			/// <summary>gui_text_widget_definition</summary>
			txt3,
			/// <summary>dialogue</summary>
			udlg,
			/// <summary>sound_cache_file_gestalt</summary>
			ugh_,
			/// <summary>user_interface_sounds_definition</summary>
			uise,
			/// <summary>multilingual_unicode_string_list</summary>
			unic,
			/// <summary>unit</summary>
			unit,
			/// <summary>vehicle_collection</summary>
			vehc,
			/// <summary>vehicle</summary>
			vehi,
			/// <summary>vertex_shader</summary>
			vtsh,
			/// <summary>gui_widget_animation_collection_definition</summary>
			wacd,
			/// <summary>gui_widget_color_animation_definition</summary>
			wclr,
			/// <summary>weapon</summary>
			weap,
			/// <summary>game_engine_settings_definition</summary>
			wezr,
			/// <summary>gui_widget_font_animation_definition</summary>
			wfon,
			/// <summary>gui_widget_animation_definition</summary>
			wgan,
			/// <summary>user_interface_globals_definition</summary>
			wgtz,
			/// <summary>cellular_automata2d</summary>
			whip,
			/// <summary>user_interface_shared_globals_definition</summary>
			wigl,
			/// <summary>wind</summary>
			wind,
			/// <summary>gui_widget_position_animation_definition</summary>
			wpos,
			/// <summary>gui_widget_rotation_animation_definition</summary>
			wrot,
			/// <summary>gui_widget_scale_animation_definition</summary>
			wscl,
			/// <summary>gui_widget_sprite_animation_definition</summary>
			wspr,
			/// <summary>gui_widget_texture_coordinate_animation_definition</summary>
			wtuv,
			/// <summary>cache_file_resource_gestalt</summary>
			zone,
			#endregion

			/// <summary>scenario_lightmap_bsp_data</summary>
			Lbsp,
			/// <summary>achievements</summary>
			achi,
			/// <summary>ai_globals</summary>
			aigl,
			/// <summary>device_arg_device</summary>
			argd,
			/// <summary>formation</summary>
			form,
			/// <summary>user_interface_fourth_wall_timing_definition</summary>
			fwtg,
			/// <summary>game_progression</summary>
			gpdt,
			/// <summary>particle_emitter_custom_points</summary>
			pecp,
			/// <summary>shader_black</summary>
			rmbk,
			/// <summary>shader_screen</summary>
			rmss,
			/// <summary>survival_mode_globals</summary>
			smdt,
			/// <summary>scenario_pda</summary>
			spda,
			/// <summary>squad_template</summary>
			sqtm,
			/// <summary>test_tag</summary>
			ttag,
			/// <summary>tag_template_unit_test</summary>
			uttt,
			/// <summary>vision_mode</summary>
			vmdx
		};
		#endregion
	};
}