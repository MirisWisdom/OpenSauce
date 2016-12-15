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
		#region Object Group
		/// <summary>
		/// Object Group
		/// </summary>
		public static TagGroupCollection GroupsObjects;
		static void GroupsObjectsInitialize()
		{
			GroupsObjects = new TagGroupCollection(false,
				obje,
				bloc,
				proj,
				scen,
				unit, // unit
				bipd,
				gint,
				vehi,
				devi, // device
				ctrl,
				mach,
				term,
				item, // item
				eqip,
				weap
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
				devi,
				ctrl,
				mach,
				term
			);
		}
		#endregion

		#region Item Group
		/// <summary>
		/// Item Group
		/// </summary>
		public static TagGroupCollection GroupsItems;
		static void GroupsItemsInitialize()
		{
			GroupsItems = new TagGroupCollection(false,
				item,
				eqip,
				weap
			);
		}
		#endregion

		#region Unit Group
		/// <summary>
		/// Unit Group
		/// </summary>
		public static TagGroupCollection GroupsUnits;
		static void GroupsUnitsInitialize()
		{
			GroupsUnits = new TagGroupCollection(false,
				unit,
				bipd,
				gint,
				vehi
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
				rm__,
				_rmc,
				_rmp,
				rmb_,
				rmcs,
				rmct,
				rmd_,
				rmfl,
				rmhg,
				rmlv,
				rmsh,
				rmsk,
				rmtr,
				rmw_
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
			GroupsItemsInitialize();
			GroupsUnitsInitialize();
			GroupsShadersInitialize();
			Groups = new TagGroupCollection(false,
				TagGroup.Null,
				_rmc,
				_rmp,
				shit,
				srscen,
				srweap,
				srvehi,
				srefsc,
				srligh,
				srbipd,
				sreqip,
				srcrea,
				srssce,
				srcmmt,
				_fx_,
				BooM,
				adlg,
				srai,
				ant_,
				beam,
				bink,
				bipd,
				bitm,
				bkey,
				bloc,
				bmp3,
				bsdt,
				cddf,
				cfxs,
				chad,
				char_,
				chdt,
				chgd,
				chmt,
				srcine,
				cine,
				cisc,
				srclut,
				clwd,
				cntl,
				coll,
				colo,
				crea,
				crte,
				ctrl,
				srcube,
				srdcrs,
				dctr,
				srdeca,
				decs,
				devi,
				devo,
				srdgrp,
				dobc,
				draw,
				drdf,
				dsrc,
				effe,
				effg,
				efsc,
				egor,
				eqip,
				flck,
				fldy,
				fog_,
				foot,
				fpch,
				frag,
				gint,
				glps,
				glvs,
				goof,
				grup,
				hlmt,
				hlsl,
				srhscf,
				item,
				itmc,
				jmad,
				jmrq,
				jpt_,
				lens,
				ligh,
				lsnd,
				lst3,
				lswd,
				ltvl,
				mach,
				matg,
				mdl3,
				mdlg,
				metr,
				mffn,
				mode,
				mply,
				mulg,
				nclt,
				obje,
				perf,
				phmo,
				pixl,
				play,
				pmdf,
				pmov,
				pphy,
				proj,
				prt3,
				rasg,
				rm__,
				rmb_,
				rmcs,
				rmct,
				rmd_,
				rmdf,
				rmfl,
				rmhg,
				rmlv,
				rmop,
				rmsh,
				rmsk,
				rmt2,
				rmtr,
				rmw_,
				rwrd,
				sFdT,
				sLdT,
				sbsp,
				scen,
				scn3,
				scnr,
				sddt,
				sefc,
				sfx_,
				sgp_,
				shit2,
				sily,
				skn3,
				srsky_,
				skya,
				smap,
				sncl,
				snd_,
				snde,
				snmx,
				spk_,
				ssce,
				srsslt,
				stli,
				stse,
				styl,
				term,
				trak,
				srtrgr,
				txt3,
				udlg,
				ugh_,
				uise,
				unic,
				unit,
				vehc,
				vehi,
				vtsh,
				wacd,
				wclr,
				weap,
				wezr,
				wfon,
				wgan,
				wgtz,
				whip,
				wigl,
				wind,
				wpos,
				wrot,
				wscl,
				wspr,
				wtuv,
				zone
			);
		}

		/// <summary>
		/// Constant indices to each tag group in <see cref="Groups"/>
		/// </summary>
		public enum Enumerated : int
		{
			/// <summary>any tag</summary>
			null_,
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
			zone
		};
		#endregion

		#region beta only groups
		/*
		 rmc	rm  	ÿÿÿÿ	0	1F12	shader_contrail
		 rmp	rm  	ÿÿÿÿ	0	1F13	shader_particle
		bkey	ÿÿÿÿ	ÿÿÿÿ	0	1F25	gui_button_key_definition
		bmp3	ÿÿÿÿ	ÿÿÿÿ	0	1F27	gui_bitmap_widget_definition
		cddf	ÿÿÿÿ	ÿÿÿÿ	0	423		collision_damage
		cfxs	ÿÿÿÿ	ÿÿÿÿ	0	1F29	camera_fx_settings
		chad	ÿÿÿÿ	ÿÿÿÿ	0	1F2A	chud_animation_definition
		chdt	ÿÿÿÿ	ÿÿÿÿ	0	1F2C	chud_definition
		chgd	ÿÿÿÿ	ÿÿÿÿ	0	1F2D	chud_globals_definition
		cine	ÿÿÿÿ	ÿÿÿÿ	0	1152	cinematic?
		cisc	ÿÿÿÿ	ÿÿÿÿ	0	1F2F	cinematic_scene
		clts	ÿÿÿÿ	ÿÿÿÿ	0	1F30	cinematic_lighting_set
		clu*	ÿÿÿÿ	ÿÿÿÿ	0	1F31	scenario_cluster_data_resource
		cntl	ÿÿÿÿ	ÿÿÿÿ	0	1F33	contrail_system
		cub*	ÿÿÿÿ	ÿÿÿÿ	0	1F39	scenario_cubemap_resource
		decs	ÿÿÿÿ	ÿÿÿÿ	0	1F3D	decal_system
		draw	ÿÿÿÿ	ÿÿÿÿ	0	1F41	rasterizer_cache_file_globals
		drdf	ÿÿÿÿ	ÿÿÿÿ	0	1F42	damage_response_definition
		dsrc	ÿÿÿÿ	ÿÿÿÿ	0	1F43	gui_datasource_definition
		flck	ÿÿÿÿ	ÿÿÿÿ	0	1F46	flock
		fldy	ÿÿÿÿ	ÿÿÿÿ	0	1F47	fluid_dynamics
		frag	ÿÿÿÿ	ÿÿÿÿ	0	1F4B	fragment
		gint	unit	obje	0	1F4C	giant
		glps	ÿÿÿÿ	ÿÿÿÿ	0	1F4E	global_pixel_shader
		glvs	ÿÿÿÿ	ÿÿÿÿ	0	1F4F	global_vertex_shader
		goof	ÿÿÿÿ	ÿÿÿÿ	0	1F50	multiplayer_variant_settings_interface_definition
		grup	ÿÿÿÿ	ÿÿÿÿ	0	1F51	gui_group_widget_definition
		hlsl	ÿÿÿÿ	ÿÿÿÿ	0	1F52	hlsl_include
		jmrq	ÿÿÿÿ	ÿÿÿÿ	0	1F56	sandbox_text_value_pair_definition
		lst3	ÿÿÿÿ	ÿÿÿÿ	0	1F5A	gui_list_widget_definition
		lswd	ÿÿÿÿ	ÿÿÿÿ	0	1F5B	leaf_system
		ltvl	ÿÿÿÿ	ÿÿÿÿ	0	1F5C	light_volume_system
		mdl3	ÿÿÿÿ	ÿÿÿÿ	0	1F5F	gui_model_widget_definition
		metr	ÿÿÿÿ	ÿÿÿÿ	0	631		meter
		mffn	ÿÿÿÿ	ÿÿÿÿ	0	1F61	muffin
		perf	ÿÿÿÿ	ÿÿÿÿ	0	1F66	performance_throttles
		pixl	ÿÿÿÿ	ÿÿÿÿ	0	1F67	pixel_shader
		rasg	ÿÿÿÿ	ÿÿÿÿ	0	1F6C	rasterizer_globals
		rm  	ÿÿÿÿ	ÿÿÿÿ	0	1F6D	render_method
		rmcs	rm  	ÿÿÿÿ	0	1F6E	shader_custom
		rmd 	rm  	ÿÿÿÿ	0	1F6F	shader_decal
		rmdf	ÿÿÿÿ	ÿÿÿÿ	0	1F70	render_method_definition
		rmhg	rm  	ÿÿÿÿ	0	1F71	shader_halogram
		rmlv	rm  	ÿÿÿÿ	0	1F72	shader_light_volume
		rmop	ÿÿÿÿ	ÿÿÿÿ	0	1F73	render_method_option
		rmsh	rm  	ÿÿÿÿ	0	1F74	shader
		rmsk	rm  	ÿÿÿÿ	0	1F75	shader_skin
		rmt2	ÿÿÿÿ	ÿÿÿÿ	0	1F76	render_method_template
		rmtr	rm  	ÿÿÿÿ	0	1F77	shader_terrain
		rmw 	rm  	ÿÿÿÿ	0	1F78	shader_water
		rwrd	ÿÿÿÿ	ÿÿÿÿ	0	1F79	render_water_ripple
		sFdT	ÿÿÿÿ	ÿÿÿÿ	0	1F7A	scenario_faux_data
		sLdT	ÿÿÿÿ	ÿÿÿÿ	0	1F7B	scenario_lightmap
		scn3	ÿÿÿÿ	ÿÿÿÿ	0	1F7E	gui_screen_widget_definition
		sddt	ÿÿÿÿ	ÿÿÿÿ	0	FF80	structure_design
		sgp!	ÿÿÿÿ	ÿÿÿÿ	0	FF82	sound_global_propagation
		shit	ÿÿÿÿ	ÿÿÿÿ	0	FF83	shield_impact
		skn3	ÿÿÿÿ	ÿÿÿÿ	0	FF85	gui_skin_definition
		sky*	ÿÿÿÿ	ÿÿÿÿ	0	FF86	scenario_sky_references_resource
		skya	ÿÿÿÿ	ÿÿÿÿ	0	FF87	sky_atm_parameters
		stse	ÿÿÿÿ	ÿÿÿÿ	0	FF90	structure_seams
		term	devi	obje	0	FF9D	device_terminal?
		txt3	ÿÿÿÿ	ÿÿÿÿ	0	FF94	gui_text_widget_definition
		uise	ÿÿÿÿ	ÿÿÿÿ	0	FF97	user_interface_sounds_definition
		wacd	ÿÿÿÿ	ÿÿÿÿ	0	FF9C	gui_widget_animation_collection_definition
		wclr	ÿÿÿÿ	ÿÿÿÿ	0	FF9D	gui_widget_color_animation_definition
		wezr	ÿÿÿÿ	ÿÿÿÿ	0	FF9E	game_engine_settings_definition
		wfon	ÿÿÿÿ	ÿÿÿÿ	0	FF9F	gui_widget_font_animation_definition
		wgan	ÿÿÿÿ	ÿÿÿÿ	0	FFA0	gui_widget_animation_definition
		wgtz	ÿÿÿÿ	ÿÿÿÿ	0	FFA1	user_interface_globals_definition
		wigl	ÿÿÿÿ	ÿÿÿÿ	0	FFA3	user_interface_shared_globals_definition
		wpos	ÿÿÿÿ	ÿÿÿÿ	0	FFA5	gui_widget_position_animation_definition
		wrot	ÿÿÿÿ	ÿÿÿÿ	0	FFA6	gui_widget_rotation_animation_definition
		wscl	ÿÿÿÿ	ÿÿÿÿ	0	FFA7	gui_widget_scale_animation_definition
		wspr	ÿÿÿÿ	ÿÿÿÿ	0	FFA8	gui_widget_sprite_animation_definition
		wtuv	ÿÿÿÿ	ÿÿÿÿ	0	FFA9	gui_widget_texture_coordinate_animation_definition
		zone	ÿÿÿÿ	ÿÿÿÿ	0	FFAA	cache_file_resource_gestalt
		 */
		#endregion

		/*
		 rmc	rm  	ÿÿÿÿ	0	FFD4	shader_contrail
		 rmp	rm  	ÿÿÿÿ	0	FFD5	shader_particle
		$#!+	ÿÿÿÿ	ÿÿÿÿ	0	FFD6	cache_file_sound
		*cen	ÿÿÿÿ	ÿÿÿÿ	0	FFD7	scenario_scenery_resource
		*eap	ÿÿÿÿ	ÿÿÿÿ	0	FFD8	scenario_weapons_resource
		*ehi	ÿÿÿÿ	ÿÿÿÿ	0	FFD9	scenario_vehicles_resource
		*fsc	ÿÿÿÿ	ÿÿÿÿ	0	FFDA	scenario_effect_scenery_resource
		*igh	ÿÿÿÿ	ÿÿÿÿ	0	FFDB	scenario_lights_resource
		*ipd	ÿÿÿÿ	ÿÿÿÿ	0	FFDC	scenario_bipeds_resource
		*qip	ÿÿÿÿ	ÿÿÿÿ	0	FFDD	scenario_equipment_resource
		*rea	ÿÿÿÿ	ÿÿÿÿ	0	FFDE	scenario_creature_resource
		*sce	ÿÿÿÿ	ÿÿÿÿ	0	FFDF	scenario_sound_scenery_resource
											scenario_comments_resource
		<fx>	ÿÿÿÿ	ÿÿÿÿ	0	FFE1	sound_effect_template
		BooM	ÿÿÿÿ	ÿÿÿÿ	0	FFE2	stereo_system
		adlg	ÿÿÿÿ	ÿÿÿÿ	0	FFE3	ai_dialogue_globals
		ai**	ÿÿÿÿ	ÿÿÿÿ	0	FFE4	scenario_ai_resource
		ant!	ÿÿÿÿ	ÿÿÿÿ	0	FFE5	antenna
		beam	ÿÿÿÿ	ÿÿÿÿ	0	FFE6	beam_system
		bink	ÿÿÿÿ	ÿÿÿÿ	0	FFE7	bink
		bipd	unit	obje	0	FFE8	biped
		bitm	ÿÿÿÿ	ÿÿÿÿ	0	FFE9	bitmap
		bkey	ÿÿÿÿ	ÿÿÿÿ	0	FFEA	gui_button_key_definition
		bloc	obje	ÿÿÿÿ	0	FFEB	crate
		bmp3	ÿÿÿÿ	ÿÿÿÿ	0	FFEC	gui_bitmap_widget_definition
		bsdt	ÿÿÿÿ	ÿÿÿÿ	0	FFED	breakable_surface
		cddf	ÿÿÿÿ	ÿÿÿÿ	0	FFF8	collision_damage
		cfxs	ÿÿÿÿ	ÿÿÿÿ	0	FFEE	camera_fx_settings
		chad	ÿÿÿÿ	ÿÿÿÿ	0	FFEF	chud_animation_definition
		char	ÿÿÿÿ	ÿÿÿÿ	0	FFF0	character
		chdt	ÿÿÿÿ	ÿÿÿÿ	0	FFF1	chud_definition
		chgd	ÿÿÿÿ	ÿÿÿÿ	0	FFF2	chud_globals_definition
		chmt	ÿÿÿÿ	ÿÿÿÿ	0	FFF3	chocolate_mountain_new
		cin*	ÿÿÿÿ	ÿÿÿÿ	0	FFF4	scenario_cinematics_resource
		cine	ÿÿÿÿ	ÿÿÿÿ	0	FFBF	cinematic
		cisc	ÿÿÿÿ	ÿÿÿÿ	0	FFF5	cinematic_scene
		clu*	ÿÿÿÿ	ÿÿÿÿ	0	FFF6	scenario_cluster_data_resource
		clwd	ÿÿÿÿ	ÿÿÿÿ	0	FFF7	cloth
		cntl	ÿÿÿÿ	ÿÿÿÿ	0	FFF8	contrail_system
		coll	ÿÿÿÿ	ÿÿÿÿ	0	FFF9	collision_model
		colo	ÿÿÿÿ	ÿÿÿÿ	0	FFFA	color_table
		crea	obje	ÿÿÿÿ	0	FFFB	creature
		crte	ÿÿÿÿ	ÿÿÿÿ	0	FFFC	cortana_effect_definition
		ctrl	devi	obje	0	FFFD	device_control
		cub*	ÿÿÿÿ	ÿÿÿÿ	0	FFFE	scenario_cubemap_resource
		dc*s	ÿÿÿÿ	ÿÿÿÿ	0	FFFF	scenario_decorators_resource
		dctr	ÿÿÿÿ	ÿÿÿÿ	0	3D00	decorator_set
		dec*	ÿÿÿÿ	ÿÿÿÿ	0	3D01	scenario_decals_resource
		decs	ÿÿÿÿ	ÿÿÿÿ	0	3D02	decal_system
		devi	obje	ÿÿÿÿ	0	FFC3	device
		devo	ÿÿÿÿ	ÿÿÿÿ	0	3D03	cellular_automata
		dgr*	ÿÿÿÿ	ÿÿÿÿ	0	3D04	scenario_devices_resource
		dobc	ÿÿÿÿ	ÿÿÿÿ	0	3D05	detail_object_collection
		draw	ÿÿÿÿ	ÿÿÿÿ	0	3D06	rasterizer_cache_file_globals
		drdf	ÿÿÿÿ	ÿÿÿÿ	0	3D07	damage_response_definition
		dsrc	ÿÿÿÿ	ÿÿÿÿ	0	3D08	gui_datasource_definition
		effe	ÿÿÿÿ	ÿÿÿÿ	0	126B	effect
		effg	ÿÿÿÿ	ÿÿÿÿ	0	3D09	effect_globals
		efsc	obje	ÿÿÿÿ	0	3D0A	effect_scenery
		egor	ÿÿÿÿ	ÿÿÿÿ	0	3D0B	screen_effect
		eqip	item	obje	0	FFAC	equipment
		flck	ÿÿÿÿ	ÿÿÿÿ	0	3D0C	flock
		fldy	ÿÿÿÿ	ÿÿÿÿ	0	3D0D	fluid_dynamics
		fog 	ÿÿÿÿ	ÿÿÿÿ	0	3D0E	planar_fog
		foot	ÿÿÿÿ	ÿÿÿÿ	0	3D0F	material_effects
		fpch	ÿÿÿÿ	ÿÿÿÿ	0	3D10	patchy_fog
		frag	ÿÿÿÿ	ÿÿÿÿ	0	3D11	fragment
		gint	unit	obje	0	3D12	giant
		glps	ÿÿÿÿ	ÿÿÿÿ	0	3D13	global_pixel_shader
		glvs	ÿÿÿÿ	ÿÿÿÿ	0	3D14	global_vertex_shader
		goof	ÿÿÿÿ	ÿÿÿÿ	0	3D15	multiplayer_variant_settings_interface_definition
		grup	ÿÿÿÿ	ÿÿÿÿ	0	3D16	gui_group_widget_definition
		hlmt	ÿÿÿÿ	ÿÿÿÿ	1	133		model
		hlsl	ÿÿÿÿ	ÿÿÿÿ	0	3D17	hlsl_include
		hsc*	ÿÿÿÿ	ÿÿÿÿ	0	3D18	scenario_hs_source_file
		item	obje	ÿÿÿÿ	1	10		item
		itmc	ÿÿÿÿ	ÿÿÿÿ	0	3D19	item_collection
		jmad	ÿÿÿÿ	ÿÿÿÿ	0	3D1A	model_animation_graph
		jmrq	ÿÿÿÿ	ÿÿÿÿ	0	3D1B	sandbox_text_value_pair_definition
		jpt!	ÿÿÿÿ	ÿÿÿÿ	0	3D1C	damage_effect
		lens	ÿÿÿÿ	ÿÿÿÿ	0	FFBA	lens_flare
		ligh	ÿÿÿÿ	ÿÿÿÿ	0	1017	light
		lsnd	ÿÿÿÿ	ÿÿÿÿ	0	3D1D	sound_looping
		lst3	ÿÿÿÿ	ÿÿÿÿ	0	3D1E	gui_list_widget_definition
		lswd	ÿÿÿÿ	ÿÿÿÿ	0	3D1F	leaf_system
		ltvl	ÿÿÿÿ	ÿÿÿÿ	0	3D20	light_volume_system
		mach	devi	obje	0	3D21	device_machine
		matg	ÿÿÿÿ	ÿÿÿÿ	0	3D22	globals
		mdl3	ÿÿÿÿ	ÿÿÿÿ	0	3D23	gui_model_widget_definition
		mdlg	ÿÿÿÿ	ÿÿÿÿ	0	3D24	ai_mission_dialogue
		metr	ÿÿÿÿ	ÿÿÿÿ	0	FFAC	meter
		mffn	ÿÿÿÿ	ÿÿÿÿ	0	3D25	muffin
		mode	ÿÿÿÿ	ÿÿÿÿ	0	3D26	render_model
		mply	ÿÿÿÿ	ÿÿÿÿ	0	3D27	multiplayer_scenario_description
		mulg	ÿÿÿÿ	ÿÿÿÿ	0	3D28	multiplayer_globals
		nclt	ÿÿÿÿ	ÿÿÿÿ	0	3D29	new_cinematic_lighting
		obje	ÿÿÿÿ	ÿÿÿÿ	0	FFC4	object
		perf	ÿÿÿÿ	ÿÿÿÿ	0	3D2A	performance_throttles
		phmo	ÿÿÿÿ	ÿÿÿÿ	0	3D2B	physics_model
		pixl	ÿÿÿÿ	ÿÿÿÿ	0	3D2C	pixel_shader
		play	ÿÿÿÿ	ÿÿÿÿ	0	3D2D	cache_file_resource_layout_table
		pmdf	ÿÿÿÿ	ÿÿÿÿ	0	3D2E	particle_model
		pmov	ÿÿÿÿ	ÿÿÿÿ	0	3D2F	particle_physics
		pphy	ÿÿÿÿ	ÿÿÿÿ	0	3D30	point_physics
		proj	obje	ÿÿÿÿ	0	3D31	projectile
		prt3	ÿÿÿÿ	ÿÿÿÿ	0	3D32	particle
		rasg	ÿÿÿÿ	ÿÿÿÿ	0	3D33	rasterizer_globals
		rm  	ÿÿÿÿ	ÿÿÿÿ	0	3D34	render_method
		rmb 	rm  	ÿÿÿÿ	0	3D35	shader_beam
		rmcs	rm  	ÿÿÿÿ	0	3D36	shader_custom
		rmct	rm  	ÿÿÿÿ	0	3D37	shader_cortana
		rmd 	rm  	ÿÿÿÿ	0	3D38	shader_decal
		rmdf	ÿÿÿÿ	ÿÿÿÿ	0	3D39	render_method_definition
		rmfl	rm  	ÿÿÿÿ	0	3D3A	shader_foliage
		rmhg	rm  	ÿÿÿÿ	0	3D3B	shader_halogram
		rmlv	rm  	ÿÿÿÿ	0	3D3C	shader_light_volume
		rmop	ÿÿÿÿ	ÿÿÿÿ	0	3D3D	render_method_option
		rmsh	rm  	ÿÿÿÿ	0	3D3E	shader
		rmsk	rm  	ÿÿÿÿ	0	3D3F	shader_skin
		rmt2	ÿÿÿÿ	ÿÿÿÿ	0	3D40	render_method_template
		rmtr	rm  	ÿÿÿÿ	0	3D41	shader_terrain
		rmw 	rm  	ÿÿÿÿ	0	3D42	shader_water
		rwrd	ÿÿÿÿ	ÿÿÿÿ	0	3D43	render_water_ripple
		sFdT	ÿÿÿÿ	ÿÿÿÿ	0	3D44	scenario_faux_data
		sLdT	ÿÿÿÿ	ÿÿÿÿ	0	3D45	scenario_lightmap
		sbsp	ÿÿÿÿ	ÿÿÿÿ	0	3D46	scenario_structure_bsp
		scen	obje	ÿÿÿÿ	0	3D47	scenery
		scn3	ÿÿÿÿ	ÿÿÿÿ	0	3D48	gui_screen_widget_definition
		scnr	ÿÿÿÿ	ÿÿÿÿ	0	3D49	scenario
		sddt	ÿÿÿÿ	ÿÿÿÿ	0	3D4A	structure_design
		sefc	ÿÿÿÿ	ÿÿÿÿ	0	3D4B	area_screen_effect
		sfx+	ÿÿÿÿ	ÿÿÿÿ	0	3D4C	sound_effect_collection
		sgp!	ÿÿÿÿ	ÿÿÿÿ	0	3D4D	sound_global_propagation
		shit	ÿÿÿÿ	ÿÿÿÿ	0	3D4E	shield_impact
		sily	ÿÿÿÿ	ÿÿÿÿ	0	3D4F	text_value_pair_definition
		skn3	ÿÿÿÿ	ÿÿÿÿ	0	3D50	gui_skin_definition
		sky*	ÿÿÿÿ	ÿÿÿÿ	0	3D51	scenario_sky_references_resource
		skya	ÿÿÿÿ	ÿÿÿÿ	0	3D52	sky_atm_parameters
		smap	ÿÿÿÿ	ÿÿÿÿ	0	3D53	shared_cache_file_layout
		sncl	ÿÿÿÿ	ÿÿÿÿ	0	3D54	sound_classes
		snd!	ÿÿÿÿ	ÿÿÿÿ	0	FFEB	sound
		snde	ÿÿÿÿ	ÿÿÿÿ	0	3D55	sound_environment
		snmx	ÿÿÿÿ	ÿÿÿÿ	0	3D56	sound_mix
		spk!	ÿÿÿÿ	ÿÿÿÿ	0	3D57	sound_dialogue_constants
		ssce	obje	ÿÿÿÿ	0	3D58	sound_scenery
		sslt	ÿÿÿÿ	ÿÿÿÿ	0	3D59	scenario_structure_lighting_resource
		stli	ÿÿÿÿ	ÿÿÿÿ	0	3D5A	scenario_structure_lighting_info
		stse	ÿÿÿÿ	ÿÿÿÿ	0	3D5B	structure_seams
		styl	ÿÿÿÿ	ÿÿÿÿ	0	3D5C	style
		term	devi	obje	0	FF96	device_terminal
		trak	ÿÿÿÿ	ÿÿÿÿ	0	3D5D	camera_track
		trg*	ÿÿÿÿ	ÿÿÿÿ	0	3D5E	scenario_trigger_volumes_resource
		txt3	ÿÿÿÿ	ÿÿÿÿ	0	3D5F	gui_text_widget_definition
		udlg	ÿÿÿÿ	ÿÿÿÿ	0	3D60	dialogue
		ugh!	ÿÿÿÿ	ÿÿÿÿ	0	3D61	sound_cache_file_gestalt
		uise	ÿÿÿÿ	ÿÿÿÿ	0	3D62	user_interface_sounds_definition
		unic	ÿÿÿÿ	ÿÿÿÿ	0	3D63	multilingual_unicode_string_list
		unit	obje	ÿÿÿÿ	0	3D64	unit
		vehc	ÿÿÿÿ	ÿÿÿÿ	0	3D65	vehicle_collection
		vehi	unit	obje	0	FFC0	vehicle
		vtsh	ÿÿÿÿ	ÿÿÿÿ	0	3D66	vertex_shader
		wacd	ÿÿÿÿ	ÿÿÿÿ	0	3D67	gui_widget_animation_collection_definition
		wclr	ÿÿÿÿ	ÿÿÿÿ	0	3D68	gui_widget_color_animation_definition
		weap	item	obje	0	FFBF	weapon
		wezr	ÿÿÿÿ	ÿÿÿÿ	0	3D69	game_engine_settings_definition
		wfon	ÿÿÿÿ	ÿÿÿÿ	0	3D6A	gui_widget_font_animation_definition
		wgan	ÿÿÿÿ	ÿÿÿÿ	0	3D6B	gui_widget_animation_definition
		wgtz	ÿÿÿÿ	ÿÿÿÿ	0	3D6C	user_interface_globals_definition
		whip	ÿÿÿÿ	ÿÿÿÿ	0	3D6D	cellular_automata2d
		wigl	ÿÿÿÿ	ÿÿÿÿ	0	3D6E	user_interface_shared_globals_definition
		wind	ÿÿÿÿ	ÿÿÿÿ	0	3D6F	wind
		wpos	ÿÿÿÿ	ÿÿÿÿ	0	3D70	gui_widget_position_animation_definition
		wrot	ÿÿÿÿ	ÿÿÿÿ	0	3D71	gui_widget_rotation_animation_definition
		wscl	ÿÿÿÿ	ÿÿÿÿ	0	3D72	gui_widget_scale_animation_definition
		wspr	ÿÿÿÿ	ÿÿÿÿ	0	3D73	gui_widget_sprite_animation_definition
		wtuv	ÿÿÿÿ	ÿÿÿÿ	0	3D74	gui_widget_texture_coordinate_animation_definition
		zone	ÿÿÿÿ	ÿÿÿÿ	0	3D75	cache_file_resource_gestalt
		 */
	};
}