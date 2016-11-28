/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
﻿using System;
using BlamLib.TagInterface;

namespace BlamLib.Blam.Halo4
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
				dspn,
				mach,
				term,
				ents, // entity
				spnr,
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
				dspn,
				mach,
				term
			);
		}
		#endregion

		#region Entities Group
		/// <summary>
		/// Entities Group
		/// </summary>
		public static TagGroupCollection GroupsEntities;
		static void GroupsEntitiesInitialize()
		{
			GroupsEntities = new TagGroupCollection(false,
				ents,
				spnr
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
				rmcs,
				rmct,
				rmd_,
				rmfl,
				rmfs,//Reach
				rmfu,//Reach
				rmgl,//Reach
				rmhg,
				rmla,//Halo4
				rmlv,
				rmmm,//Reach
				rmmx,//Reach
				rmsh,
				rmsk,
				rmss,//ODST
				rmtr,
				rmw_,
				rmwf //Halo4
			);
		}
		#endregion


		#region Halo 4 Tags Group Collection
		/// <summary>
		/// All tag groups in Halo 4
		/// </summary>
		public static TagGroupCollection Groups;
		static void GroupsInitialize()
		{
			GroupsObjectsInitialize();
			GroupsDevicesInitialize();
			GroupsEntitiesInitialize();
			GroupsItemsInitialize();
			GroupsUnitsInitialize();
			GroupsShadersInitialize();
			Groups = new TagGroupCollection(false,
				TagGroup.Null,
				_rmc,
				_rmp,
				shit,
				_fx_,
				BooM,
				LMgS,//Reach
				Lbsp,//ODST
				achi,//ODST
				adlg,
				aigl,//ODST
				ant_,
				atgf,//Reach
				bdpd,//Reach
				bink,
				bipd,
				bitm,
				bloc,
				bsdt,
				cddf,
				cfxs,
				char_,
				chdg,//Reach
				cine,
				cisc,
				citr,//Reach
				clwd,
				coll,
				colo,
				comg,//Reach
				coms,//Reach
				cook,//Reach
				coop,//Reach
				cpem,//Reach
				cpgd,//Reach
				cptl,//Reach
				crea,
				csdt,//Reach
				ctrl,
				cusc,//Reach
				cust,//Reach
				dctr,
				decs,
				devi,
				devo,
				dobc,
				draw,
				drdf,
				effe,
				effg,
				efsc,
				eqip,
				flck,
				fldy,
				foot,
				gint,
				glps,
				glvs,
				goof,
				hlmt,
				hlsl,
				hsc_,
				item,
				jmad,
				jmrq,
				jpt_,
				lens,
				ligh,
				lsnd,
				lswd,
				ltvl,
				mach,
				matg,
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
				sLdT,
				sbsp,
				scen,
				scnr,
				sddt,
				sefc,
				sfx_,
				sgp_,
				shit2,
				sily,
				smap,
				sncl,
				snd_,
				snde,
				snmx,
				spk_,
				ssce,
				stli,
				stse,
				styl,
				term,
				trak,
				udlg,
				ugh_,
				uise,
				unic,
				unit,
				vehi,
				vtsh,
				weap,
				wezr,
				wgtz,
				whip,
				wigl,
				wind,
				zone,

				ebhd,//Reach
				fogg,//Reach
				form,//ODST
				frms,//Reach
				fxtt,//Reach
				gcrg,//Reach
				gegl,//Reach
				gldf,//Reach
				gmeg,//Reach
				gpix,//Reach
				gptd,//Reach
				grfr,//Reach
				hcfd,//Reach
				igpd,//Reach
				iimz,//Reach
				impo,//Reach
				ingd,//Reach
				lgtd,//Reach
				locs,//Reach
				mgls,//Reach
				mlib,//Reach
				motl,//Reach
				msit,//Reach
				muxg,//Reach
				pach,//Reach
				pcec,//Reach
				pecp,//ODST
				pfpt,//Reach
				pggd,//Reach
				pmcg,//Reach
				rain,//Reach
				rmbl,//Reach
				rmfs,//Reach
				rmfu,//Reach
				rmgl,//Reach
				rmmm,//Reach
				rmmx,//Reach
				rmss,//ODST
				sadt,//Reach
				scmb,//Reach
				sidt,//Reach
				siin,//Reach
				smdt,//ODST
				sqtm,//ODST
				srad,//Reach
				ssao,//Reach
				ttag,//ODST
				uttt,//ODST
				vmdx,//ODST
				wave,//Reach
				wetn,//Reach
				wpdp,//Reach
				wxcg,//Reach

				airs,//Reach Retail
				avat,//Reach Retail
				bbcr,//Reach Retail
				cmoe,//Reach Retail
				ldsc,//Reach Retail
				pfmc,//Reach Retail
				sdzg,//Reach Retail
				sirp,//Reach Retail
				vtgl,//Reach Retail

				LMMg,
				SDzs,
				armg,
				aulp,
				capg,
				cisd,
				cnmp,
				coag,
				crvs,
				culo,
				dpnd,
				dspn,
				egfd,
				ents,
				ffgd,
				ffgt,
				forg,
				gggl,
				ggol,
				hscn,
				hsdt,
				iuii,
				kccd,
				licn,
				lrig,
				mat_,
				mats,
				mdsv,
				mech,
				meco,
				mgee,
				mmvo,
				mtsb,
				narg,
				paas,
				patg,
				pcaa,
				pdti,
				pegd,
				pfnd,
				pman,
				ppod,
				prfb,
				prog,
				rmla,
				rmwf,
				sbnk,
				scol,
				sgrp,
				shvr,
				sict,
				sigd,
				sirg,
				slag,
				smet,
				sndo,
				sndx,
				spnr,
				ssdf,
				trac,
				uihg,
				uiss,
				vaas,
				vchd
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
			/// <summary>sound_effect_template</summary>
			_fx_,
			/// <summary>stereo_system</summary>
			BooM,
			/// <summary>lightmapper_globals</summary>
			LMgS,
			/// <summary>scenario_lightmap_bsp_data</summary>
			Lbsp,
			/// <summary>achievements</summary>
			achi,
			/// <summary>ai_dialogue_globals</summary>
			adlg,
			/// <summary>ai_globals</summary>
			aigl,
			/// <summary>antenna</summary>
			ant_,
			/// <summary>atmosphere_globals</summary>
			atgf,
			/// <summary>death_program_selector</summary>
			bdpd,
			/// <summary>bink</summary>
			bink,
			/// <summary>biped</summary>
			bipd,
			/// <summary>bitmap</summary>
			bitm,
			/// <summary>crate</summary>
			bloc,
			/// <summary>breakable_surface</summary>
			bsdt,
			/// <summary>collision_damage</summary>
			cddf,
			/// <summary>camera_fx_settings</summary>
			cfxs,
			/// <summary>character</summary>
			char_,
			/// <summary>challenge_globals_definition</summary>
			chdg,
			/// <summary>cinematic</summary>
			cine,
			/// <summary>cinematic_scene</summary>
			cisc,
			/// <summary>cinematic_transition</summary>
			citr,
			/// <summary>cloth</summary>
			clwd,
			/// <summary>collision_model</summary>
			coll,
			/// <summary>color_table</summary>
			colo,
			/// <summary>commendation_globals_definition</summary>
			comg,
			/// <summary>communication_sounds</summary>
			coms,
			/// <summary>cookie_globals_definition</summary>
			cook,
			/// <summary>coop_spawning_globals_definition</summary>
			coop,
			/// <summary>cheap_particle_emitter</summary>
			cpem,
			/// <summary>cookie_purchase_globals</summary>
			cpgd,
			/// <summary>cheap_particle_type_library</summary>
			cptl,
			/// <summary>creature</summary>
			crea,
			/// <summary>camera_shake</summary>
			csdt,
			/// <summary>device_control</summary>
			ctrl,
			/// <summary>cui_screen</summary>
			cusc,
			/// <summary>cui_static_data</summary>
			cust,
			/// <summary>decorator_set</summary>
			dctr,
			/// <summary>decal_system</summary>
			decs,
			/// <summary>device</summary>
			devi,
			/// <summary>cellular_automata</summary>
			devo,
			/// <summary>detail_object_collection</summary>
			dobc,
			/// <summary>rasterizer_cache_file_globals</summary>
			draw,
			/// <summary>damage_response_definition</summary>
			drdf,
			/// <summary>effect</summary>
			effe,
			/// <summary>effect_globals</summary>
			effg,
			/// <summary>effect_scenery</summary>
			efsc,
			/// <summary>equipment</summary>
			eqip,
			/// <summary>flock</summary>
			flck,
			/// <summary>fluid_dynamics</summary>
			fldy,
			/// <summary>material_effects</summary>
			foot,
			/// <summary>giant</summary>
			gint,
			/// <summary>global_pixel_shader</summary>
			glps,
			/// <summary>global_vertex_shader</summary>
			glvs,
			/// <summary>multiplayer_variant_settings_interface_definition</summary>
			goof,
			/// <summary>model</summary>
			hlmt,
			/// <summary>hlsl_include</summary>
			hlsl,
			/// <summary>hsc</summary>
			hsc_,
			/// <summary>item</summary>
			item,
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
			/// <summary>leaf_system</summary>
			lswd,
			/// <summary>light_volume_system</summary>
			ltvl,
			/// <summary>device_machine</summary>
			mach,
			/// <summary>globals</summary>
			matg,
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
			/// <summary>scenario_lightmap</summary>
			sLdT,
			/// <summary>scenario_structure_bsp</summary>
			sbsp,
			/// <summary>scenery</summary>
			scen,
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
			/// <summary>vehicle</summary>
			vehi,
			/// <summary>vertex_shader</summary>
			vtsh,
			/// <summary>weapon</summary>
			weap,
			/// <summary>game_engine_settings_definition</summary>
			wezr,
			/// <summary>user_interface_globals_definition</summary>
			wgtz,
			/// <summary>cellular_automata2d</summary>
			whip,
			/// <summary>user_interface_shared_globals_definition</summary>
			wigl,
			/// <summary>wind</summary>
			wind,
			/// <summary>cache_file_resource_gestalt</summary>
			zone,

			/// <summary>particle_emitter_boat_hull_shape</summary>
			ebhd,
			/// <summary>atmosphere_fog</summary>
			fogg,
			/// <summary>formation</summary>
			form,
			/// <summary>frame_event_list</summary>
			frms,
			/// <summary>fx_test</summary>
			fxtt,
			/// <summary>game_completion_rewards_globals</summary>
			gcrg,
			/// <summary>game_engine_globals</summary>
			gegl,
			/// <summary>cheap_light</summary>
			gldf,
			/// <summary>game_medal_globals</summary>
			gmeg,
			/// <summary>global_cache_file_pixel_shaders</summary>
			gpix,
			/// <summary>game_performance_throttle</summary>
			gptd,
			/// <summary>grounded_friction</summary>
			grfr,
			/// <summary>havok_collision_filter</summary>
			hcfd,
			/// <summary>incident_global_properties_definition</summary>
			igpd,
			/// <summary>instance_imposter_definition</summary>
			iimz,
			/// <summary>imposter_model</summary>
			impo,
			/// <summary>incident_globals_definition</summary>
			ingd,
			/// <summary>loadout_globals_definition</summary>
			lgtd,
			/// <summary>location_name_globals_definition</summary>
			locs,
			/// <summary>megalogamengine_sounds</summary>
			mgls,
			/// <summary>emblem_library</summary>
			mlib,
			/// <summary>multiplayer_object_type_list</summary>
			motl,
			/// <summary>megalo_string_id_table</summary>
			msit,
			/// <summary>mux_generator</summary>
			muxg,
			/// <summary>tag_package_manifest</summary>
			pach,
			/// <summary>pgcr_enemy_to_category_mapping_definition</summary>
			pcec,
			/// <summary>particle_emitter_custom_points</summary>
			pecp,
			/// <summary>planar_fog_parameters</summary>
			pfpt,
			/// <summary>player_grade_globals_definition</summary>
			pggd,
			/// <summary>player_model_customization_globals</summary>
			pmcg,
			/// <summary>rain_definition</summary>
			rain,
			/// <summary>rumble</summary>
			rmbl,
			/// <summary>shader_fur_stencil</summary>
			rmfs,
			/// <summary>shader_fur</summary>
			rmfu,
			/// <summary>shader_glass</summary>
			rmgl,
			/// <summary>shader_mux_material</summary>
			rmmm,
			/// <summary>shader_mux</summary>
			rmmx,
			/// <summary>shader_screen</summary>
			rmss,
			/// <summary>spring_acceleration</summary>
			sadt,
			/// <summary>sound_combiner</summary>
			scmb,
			/// <summary>simulated_input</summary>
			sidt,
			/// <summary>simulation_interpolation</summary>
			siin,
			/// <summary>survival_mode_globals</summary>
			smdt,
			/// <summary>squad_template</summary>
			sqtm,
			/// <summary>sound_radio_settings</summary>
			srad,
			/// <summary>ssao_definition</summary>
			ssao,
			/// <summary>test_tag</summary>
			ttag,
			/// <summary>tag_template_unit_test</summary>
			uttt,
			/// <summary>vision_mode</summary>
			vmdx,
			/// <summary>wave_template</summary>
			wave,
			/// <summary>scenario_wetness_bsp_data</summary>
			wetn,
			/// <summary>water_physics_drag_properties</summary>
			wpdp,
			/// <summary>weather_globals</summary>
			wxcg,

			/// <summary>airstrike</summary>
			airs,
			/// <summary>avatar_awards</summary>
			avat,
			/// <summary>big_battle_creature</summary>
			bbcr,
			/// <summary>camo</summary>
			cmoe,
			/// <summary>load_screen</summary>
			ldsc,
			/// <summary>performance_template</summary>
			pfmc,
			/// <summary>scenario_required_resource</summary>
			sdzg,
			/// <summary>scenario_interpolator</summary>
			sirp,
			/// <summary>variant_globals</summary>
			vtgl,

			LMMg,
			SDzs,
			armg,
			aulp,
			capg,
			cisd,
			cnmp,
			coag,
			crvs,
			culo,
			dpnd,
			dspn,
			egfd,
			ents,
			ffgd,
			ffgt,
			forg,
			gggl,
			ggol,
			hscn,
			hsdt,
			iuii,
			kccd,
			licn,
			lrig,
			mat_,
			mats,
			mdsv,
			mech,
			meco,
			mgee,
			mmvo,
			mtsb,
			narg,
			paas,
			patg,
			pcaa,
			pdti,
			pegd,
			pfnd,
			pman,
			ppod,
			prfb,
			prog,
			rmla,
			rmwf,
			sbnk,
			scol,
			sgrp,
			shvr,
			sict,
			sigd,
			sirg,
			slag,
			smet,
			sndo,
			sndx,
			spnr,
			ssdf,
			trac,
			uihg,
			uiss,
			vaas,
			vchd,
		};
		#endregion
	};
}