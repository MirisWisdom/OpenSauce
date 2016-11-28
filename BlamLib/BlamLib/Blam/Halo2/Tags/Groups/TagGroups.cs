/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
﻿using System;
using BlamLib.TagInterface;

namespace BlamLib.Blam.Halo2
{
	public static partial class TagGroups
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
				vehi,
				devi, // device
				ctrl,
				lifi,
				mach,
				item, // item
				eqip,
				garb,
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
				lifi,
				mach
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
				garb,
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
				vehi
			);
		}
		#endregion


		#region Halo 2 Tags Group Collection
		/// <summary>
		/// All tag groups in Halo 2
		/// </summary>
		public static TagGroupCollection Groups;
		static void GroupsInitialize()
		{
			GroupsObjectsInitialize();
			GroupsDevicesInitialize();
			GroupsItemsInitialize();
			GroupsUnitsInitialize();
			Groups = new TagGroupCollection(false,
				TagGroup.Null,
				hlmt,
				mode,
				coll,
				phmo,
				bitm,
				colo,
				unic,
				unit,
				bipd,
				vehi,
				scen,
				bloc,
				crea,
				phys,
				obje,
				cont,
				weap,
				ligh,
				effe,
				prt3,
				PRTM,
				pmov,
				matg,
				snd_,
				lsnd,
				item,
				eqip,
				ant_,
				MGS2,
				tdtl,
				devo,
				whip,
				BooM,
				trak,
				proj,
				devi,
				mach,
				ctrl,
				lifi,
				pphy,
				ltmp,
				sbsp,
				scnr,
				shad,
				stem,
				slit,
				spas,
				vrtx,
				pixl,
				DECR,
				DECP,
				sky_,
				wind,
				snde,
				lens,
				fog,
				fpch,
				metr,
				deca,
				coln,
				jpt_,
				udlg,
				itmc,
				vehc,
				wphi,
				grhi,
				unhi,
				nhdt,
				hud_,
				hudg,
				mply,
				dobc,
				ssce,
				hmt_,
				wgit,
				skin,
				wgtz,
				wigl,
				sily,
				goof,
				foot,
				garb,
				styl,
				char_,
				adlg,
				mdlg,
				srscen,
				srbipd,
				srvehi,
				sreqip,
				srweap,
				srssce,
				srligh,
				srdgrp,
				srdeca,
				srcine,
				srtrgr,
				srclut,
				srcrea,
				srdcrs,
				srsslt,
				srhscf,
				srai,
				srcmmt,
				bsdt,
				mpdt,
				sncl,
				mulg,
				_fx_,
				sfx_,
				gldf,
				jmad,
				clwd,
				egor,
				weat,
				snmx,
				spk_,
				ugh_,
				shit,
				mcsr,
				tag_
			);
		}

		/// <summary>
		/// Constant indices to each tag group in <see cref="Groups"/>
		/// </summary>
		public enum Enumerated : int
		{
			/// <summary>any tag</summary>
			null_,
			/// <summary>model</summary>
			hlmt,
			/// <summary>render_model</summary>
			mode,
			/// <summary>collision_model</summary>
			coll,
			/// <summary>physics_model</summary>
			phmo,
			/// <summary>bitmap</summary>
			bitm,
			/// <summary>color_table</summary>
			colo,
			/// <summary>multilingual_unicode_string_list</summary>
			unic,
			/// <summary>unit</summary>
			unit,
			/// <summary>biped</summary>
			bipd,
			/// <summary>vehicle</summary>
			vehi,
			/// <summary>scenery</summary>
			scen,
			/// <summary>crate</summary>
			bloc,
			/// <summary>creature</summary>
			crea,
			/// <summary>physics</summary>
			phys,
			/// <summary>object</summary>
			obje,
			/// <summary>contrail</summary>
			cont,
			/// <summary>weapon</summary>
			weap,
			/// <summary>light</summary>
			ligh,
			/// <summary>effect</summary>
			effe,
			/// <summary>particle</summary>
			prt3,
			/// <summary>particle_model</summary>
			PRTM,
			/// <summary>particle_physics</summary>
			pmov,
			/// <summary>globals</summary>
			matg,
			/// <summary>sound</summary>
			snd_,
			/// <summary>sound_looping</summary>
			lsnd,
			/// <summary>item</summary>
			item,
			/// <summary>equipment</summary>
			eqip,
			/// <summary>antenna</summary>
			ant_,
			/// <summary>light_volume</summary>
			MGS2,
			/// <summary>liquid</summary>
			tdtl,
			/// <summary>cellular_automata</summary>
			devo,
			/// <summary>cellular_automata2d</summary>
			whip,
			/// <summary>stereo_system</summary>
			BooM,
			/// <summary>camera_track</summary>
			trak,
			/// <summary>projectile</summary>
			proj,
			/// <summary>device</summary>
			devi,
			/// <summary>device_machine</summary>
			mach,
			/// <summary>device_control</summary>
			ctrl,
			/// <summary>device_light_fixture</summary>
			lifi,
			/// <summary>point_physics</summary>
			pphy,
			/// <summary>scenario_structure_lightmap</summary>
			ltmp,
			/// <summary>scenario_structure_bsp</summary>
			sbsp,
			/// <summary>scenario</summary>
			scnr,
			/// <summary>shader</summary>
			shad,
			/// <summary>shader_template</summary>
			stem,
			/// <summary>shader_light_response</summary>
			slit,
			/// <summary>shader_pass</summary>
			spas,
			/// <summary>vertex_shader</summary>
			vrtx,
			/// <summary>pixel_shader</summary>
			pixl,
			/// <summary>decorator_set</summary>
			DECR,
			/// <summary>decorators</summary>
			DECP,
			/// <summary>sky</summary>
			sky_,
			/// <summary>wind</summary>
			wind,
			/// <summary>sound_environment</summary>
			snde,
			/// <summary>lens_flare</summary>
			lens,
			/// <summary>planar_fog</summary>
			fog,
			/// <summary>patchy_fog</summary>
			fpch,
			/// <summary>meter</summary>
			metr,
			/// <summary>decal</summary>
			deca,
			/// <summary>colony</summary>
			coln,
			/// <summary>damage_effect</summary>
			jpt_,
			/// <summary>dialogue</summary>
			udlg,
			/// <summary>item_collection</summary>
			itmc,
			/// <summary>vehicle_collection</summary>
			vehc,
			/// <summary>weapon_hud_interface</summary>
			wphi,
			/// <summary>grenade_hud_interface</summary>
			grhi,
			/// <summary>unit_hud_interface</summary>
			unhi,
			/// <summary>new_hud_definition</summary>
			nhdt,
			/// <summary>hud_number</summary>
			hud_,
			/// <summary>hud_globals</summary>
			hudg,
			/// <summary>multiplayer_scenario_description</summary>
			mply,
			/// <summary>detail_object_collection</summary>
			dobc,
			/// <summary>sound_scenery</summary>
			ssce,
			/// <summary>hud_message_text</summary>
			hmt_,
			/// <summary>user_interface_screen_widget_definition</summary>
			wgit,
			/// <summary>user_interface_list_skin_definition</summary>
			skin,
			/// <summary>user_interface_globals_definition</summary>
			wgtz,
			/// <summary>user_interface_shared_globals_definition</summary>
			wigl,
			/// <summary>text_value_pair_definition</summary>
			sily,
			/// <summary>multiplayer_variant_settings_interface_definition</summary>
			goof,
			/// <summary>material_effects</summary>
			foot,
			/// <summary>garbage</summary>
			garb,
			/// <summary>style</summary>
			styl,
			/// <summary>character</summary>
			char_,
			/// <summary>ai_dialogue_globals</summary>
			adlg,
			/// <summary>ai_mission_dialogue</summary>
			mdlg,
			/// <summary>scenario_scenery_resource</summary>
			srscen,
			/// <summary>scenario_bipeds_resource</summary>
			srbipd,
			/// <summary>scenario_vehicles_resource</summary>
			srvehi,
			/// <summary>scenario_equipment_resource</summary>
			sreqip,
			/// <summary>scenario_weapons_resource</summary>
			srweap,
			/// <summary>scenario_sound_scenery_resource</summary>
			srssce,
			/// <summary>scenario_lights_resource</summary>
			srligh,
			/// <summary>scenario_devices_resource</summary>
			srdgrp,
			/// <summary>scenario_decals_resource</summary>
			srdeca,
			/// <summary>scenario_cinematics_resource</summary>
			srcine,
			/// <summary>scenario_trigger_volumes_resource</summary>
			srtrgr,
			/// <summary>scenario_cluster_data_resource</summary>
			srclut,
			/// <summary>scenario_creature_resource</summary>
			srcrea,
			/// <summary>scenario_decorators_resource</summary>
			srdcrs,
			/// <summary>scenario_structure_lighting_resource</summary>
			srsslt,
			/// <summary>scenario_hs_source_file</summary>
			srhscf,
			/// <summary>scenario_ai_resource</summary>
			srai,
			/// <summary>scenario_comments_resource</summary>
			srcmmt,
			/// <summary>breakable_surface</summary>
			bsdt,
			/// <summary>material_physics</summary>
			mpdt,
			/// <summary>sound_classes</summary>
			sncl,
			/// <summary>multiplayer_globals</summary>
			mulg,
			/// <summary>sound_effect_template</summary>
			_fx_,
			/// <summary>sound_effect_collection</summary>
			sfx_,
			/// <summary>chocolate_mountain</summary>
			gldf,
			/// <summary>model_animation_graph</summary>
			jmad,
			/// <summary>cloth</summary>
			clwd,
			/// <summary>screen_effect</summary>
			egor,
			/// <summary>weather_system</summary>
			weat,
			/// <summary>sound_mix</summary>
			snmx,
			/// <summary>sound_dialogue_constants</summary>
			spk_,
			/// <summary>sound_cache_file_gestalt</summary>
			ugh_,
			/// <summary>cache_file_sound</summary>
			shit,
			/// <summary>mouse_cursor_definition</summary>
			mcsr,
			/// <summary>tag_database</summary>
			tag_,
		};
		#endregion

		#region TagGroupsInvalidForCacheViewer
		internal static TagGroupCollection GroupsInvalidForCacheViewer;
		static void GroupsInvalidForCacheViewerInitialize()
		{
			GroupsInvalidForCacheViewer = new TagGroupCollection(false,
				obje,
				devi,
				item,
				unit,

				srscen,
				srbipd,
				srvehi,
				sreqip,
				srweap,
				srssce,
				srligh,
				srdgrp,
				srdeca,
				srcine,
				srtrgr,
				srclut,
				srcrea,
				srdcrs,
				srsslt,
				srhscf,
				srai,
				srcmmt,

				shit,

				tag_
			);
		}
		#endregion

		#region TagGroupsInvalidForExtraction
		internal static TagGroupCollection GroupsInvalidForExtraction;
		static void GroupsInvalidForExtractionInitialize()
		{
			GroupsInvalidForExtraction = new TagGroupCollection(false,
				ugh_,

				snd_	// TODO: reconstruction
			);
		}
		#endregion
	};
}