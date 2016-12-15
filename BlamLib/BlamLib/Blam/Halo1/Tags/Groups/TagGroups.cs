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
		#region Object Group
		/// <summary>
		/// Object Group
		/// </summary>
		public static TagGroupCollection GroupsObjects;
		static void GroupsObjectsInitialize()
		{
			GroupsObjects = new TagGroupCollection(false,
				obje,
				plac,
				proj,
				scen,
				ssce,
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

		// ssrc - shader_screen, screen
		// sdec - shader_decal, decal
		#region Shader Group
		/// <summary>
		/// Shader Group
		/// </summary>
		public static TagGroupCollection GroupsShaders;
		public static TagGroupCollection GroupsShadersPc;
		public static TagGroupCollection GroupsShadersXbox;
		static void GroupsShadersInitialize()
		{
			GroupsShaders = new TagGroupCollection(false,
				shdr,
				seff, // effect
				senv, // environment
				soso, // model
				schi, // transparent generic extended
				scex,
				sotr, // transparent generic
				sgla, // transparent glass
				smet, // transparent meter
				spla, // transparent plasma
				swat  // transparent water
			);
			GroupsShadersPc = new TagGroupCollection(false,
				TagGroup.Null,	//ssrc
				seff,			// but listed as -1
				TagGroup.Null,	//sdec
				senv,
				soso,
				sotr,
				schi,
				scex,
				swat,
				sgla,
				smet,
				spla
			);
			GroupsShadersXbox = new TagGroupCollection(false,
				TagGroup.Null,	//ssrc
				seff,			//seff, but listed as -1
				TagGroup.Null,	//sdec
				senv,
				soso,
				sotr,
				swat,
				sgla,
				smet,
				spla
			);
		}

		public static TagGroupCollection GetShaderGroup(BlamVersion engine)
		{
			switch(engine)
			{
				case BlamVersion.Halo1_Xbox:
				case BlamVersion.Stubbs_Xbox:
					return GroupsShadersXbox;

				case BlamVersion.Halo1_PC:
				case BlamVersion.Halo1_Mac:
				case BlamVersion.Halo1_CE:
				case BlamVersion.Stubbs_PC:
				case BlamVersion.Stubbs_Mac:
					return GroupsShadersPc;

				default: throw new Debug.Exceptions.UnreachableException(engine);
			}
		}
		#endregion

		#region Halo 1 Tags Group Collection
		/// <summary>
		/// All tag groups in Halo 1
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
				actr,
				actv,
				ant_,
				bipd,
				bitm,
				trak,
				colo,
				cdmg,
				cont,
				jpt_,
				deca,
				dobc,
				devi,
				ctrl,
				lifi,
				mach,
				udlg,
				effe,
				eqip,
				flag,
				fog_,
				font,
				garb,
				mod2,
				matg,
				glw_,
				grhi,
				hudg,
				hmt_,
				hud_,
				devc,
				item,
				itmc,
				lens,
				ligh,
				mgs2,
				elec,
				foot,
				metr,
				mode,
				antr,
				coll,
				mply,
				obje,
				part,
				pctl,
				phys,
				plac,
				pphy,
				ngpr,
				proj,
				scnr,
				sbsp,
				scen,
				shdr,
				seff,
				senv,
				soso,
				schi,
				scex,
				sotr,
				sgla,
				smet,
				spla,
				swat,
				sky_,
				snd_,
				snde,
				lsnd,
				ssce,
				boom,
				str_,
				tagc,
				Soul,
				DeLa,
				ustr,
				unit,
				unhi,
				vehi,
				vcky,
				weap,
				wphi,
				rain,
				wind,

				tag_,

				srscen,
				srbipd,
				srvehi,
				sreqip,
				srweap,
				srssce,
				srdgrp,
				srdeca,
				srcine,
				srtrgr,
				srclut,
				srhscf,
				srai,
				srcmmt,

				gelo,
				yelo
			);
		}

		/// <summary>
		/// Constant indices to each tag group in <see cref="Groups"/>
		/// </summary>
		public enum Enumerated : int
		{
			/// <summary>any tag</summary>
			null_,
			/// <summary>actor</summary>
			actr,
			/// <summary>actor_variant</summary>
			actv,
			/// <summary>antenna</summary>
			ant_,
			/// <summary>biped</summary>
			bipd,
			/// <summary>bitmap</summary>
			bitm,
			/// <summary>camera_track</summary>
			trak,
			/// <summary>color_table</summary>
			colo,
			/// <summary>continuous_damage_effect</summary>
			cdmg,
			/// <summary>contrail</summary>
			cont,
			/// <summary>damage_effect</summary>
			jpt_,
			/// <summary>decal</summary>
			deca,
			/// <summary>detail_object_collection</summary>
			dobc,
			/// <summary>device</summary>
			devi,
			/// <summary>device_control</summary>
			ctrl,
			/// <summary>device_light_fixture</summary>
			lifi,
			/// <summary>device_machine</summary>
			mach,
			/// <summary>dialogue</summary>
			udlg,
			/// <summary>effect</summary>
			effe,
			/// <summary>equipment</summary>
			eqip,
			/// <summary>flag</summary>
			flag,
			/// <summary>fog</summary>
			fog_,
			/// <summary>font</summary>
			font,
			/// <summary>garbage</summary>
			garb,
			/// <summary>gbxmodel</summary>
			mod2,
			/// <summary>globals</summary>
			matg,
			/// <summary>glow</summary>
			glw_,
			/// <summary>grenade_hud_interface</summary>
			grhi,
			/// <summary>hud_globals</summary>
			hudg,
			/// <summary>hud_message_text</summary>
			hmt_,
			/// <summary>hud_number</summary>
			hud_,
			/// <summary>input_device_defaults</summary>
			devc,
			/// <summary>item</summary>
			item,
			/// <summary>item_collection</summary>
			itmc,
			/// <summary>lens_flare</summary>
			lens,
			/// <summary>light</summary>
			ligh,
			/// <summary>light_volume</summary>
			mgs2,
			/// <summary>lightning</summary>
			elec,
			/// <summary>material_effects</summary>
			foot,
			/// <summary>meter</summary>
			metr,
			/// <summary>model</summary>
			mode,
			/// <summary>model_animations</summary>
			antr,
			/// <summary>model_collision_geometry</summary>
			coll,
			/// <summary>multiplayer_scenario_description</summary>
			mply,
			/// <summary>object</summary>
			obje,
			/// <summary>particle</summary>
			part,
			/// <summary>particle_system</summary>
			pctl,
			/// <summary>physics</summary>
			phys,
			/// <summary>placeholder</summary>
			plac,
			/// <summary>point_physics</summary>
			pphy,
			/// <summary>preferences_network_game</summary>
			ngpr,
			/// <summary>projectile</summary>
			proj,
			/// <summary>scenario</summary>
			scnr,
			/// <summary>scenario_structure_bsp</summary>
			sbsp,
			/// <summary>scenery</summary>
			scen,
			/// <summary>shader</summary>
			shdr,
			/// <summary>shader_effect</summary>
			seff,
			/// <summary>shader_environment</summary>
			senv,
			/// <summary>shader_model</summary>
			soso,
			/// <summary>shader_transparent_chicago</summary>
			schi,
			/// <summary>shader_transparent_chicago_extended</summary>
			scex,
			/// <summary>shader_transparent_generic</summary>
			sotr,
			/// <summary>shader_transparent_glass</summary>
			sgla,
			/// <summary>shader_transparent_meter</summary>
			smet,
			/// <summary>shader_transparent_plasma</summary>
			spla,
			/// <summary>shader_transparent_water</summary>
			swat,
			/// <summary>sky</summary>
			sky_,
			/// <summary>sound</summary>
			snd_,
			/// <summary>sound_environment</summary>
			snde,
			/// <summary>sound_looping</summary>
			lsnd,
			/// <summary>sound_scenery</summary>
			ssce,
			/// <summary>spheroid</summary>
			boom,
			/// <summary>string_list</summary>
			str_,
			/// <summary>tag_collection</summary>
			tagc,
			/// <summary>ui_widget_collection</summary>
			Soul,
			/// <summary>ui_widget_definition</summary>
			DeLa,
			/// <summary>unicode_string_list</summary>
			ustr,
			/// <summary>unit</summary>
			unit,
			/// <summary>unit_hud_interface</summary>
			unhi,
			/// <summary>vehicle</summary>
			vehi,
			/// <summary>virtual_keyboard</summary>
			vcky,
			/// <summary>weapon</summary>
			weap,
			/// <summary>weapon_hud_interface</summary>
			wphi,
			/// <summary>weather_particle_system</summary>
			rain,
			/// <summary>wind</summary>
			wind,

			/// <summary>tag_database</summary>
			tag_,

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
			/// <summary>scenario_hs_source_file</summary>
			srhscf,
			/// <summary>scenario_ai_resource</summary>
			srai,
			/// <summary>scenario_comments_resource</summary>
			srcmmt,

			/// <summary>project_yellow_globals</summary>
			gelo,
			/// <summary>project_yellow</summary>
			yelo,
		};
		#endregion
	};
}