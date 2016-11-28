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
		public static TagGroupCollection Groups;
		static void GroupsInitialize()
		{
			Groups = new TagGroupCollection(false,
				play,
				mphp,
				msst,
				snpl,
				snsc,
				masd_sound,
				sszd,
				MAPP,
				uncs,
				usas,
				uHnd,
				ubms,
				_1234,
				ulYc,
				trcv,
				HVPH,
				blod,
				cbsp,
				csbs,
				aaim,

				spl1,
				//ssfx,

				MAgr,

				MAgc,

				spdf,

				plsn_simple,
				snpr
			);
		}

		/// <summary>
		/// Constant indices to each tag group in <see cref="Groups"/>
		/// </summary>
		public enum Enumerated : int
		{
			/// <summary>cache_file_resource_layout_table_struct</summary>
			play,
			/// <summary>material_physics_properties_struct</summary>
			mphp,
			/// <summary>materials_sweeteners_struct</summary>
			msst,
			/// <summary>sound_playback_parameters_struct</summary>
			snpl,
			/// <summary>sound_scale_modifiers_struct</summary>
			snsc,
			/// <summary>sound_response_extra_sounds_struct</summary>
			masd_sound,
			/// <summary>static_spawn_zone_data_struct</summary>
			sszd,
			/// <summary>mapping_function</summary>
			MAPP,
			/// <summary>unit_camera_struct</summary>
			uncs,
			/// <summary>unit_seat_acceleration_struct</summary>
			usas,
			/// <summary>unit_additional_node_names_struct</summary>
			uHnd,
			/// <summary>unit_boarding_melee_struct</summary>
			ubms,
			/// <summary>unit_boost_struct</summary>
			_1234,
			/// <summary>unit_lipsync_scales_struct</summary>
			ulYc,
			/// <summary>torque_curve_struct</summary>
			trcv,
			/// <summary>havok_vehicle_physics_struct</summary>
			HVPH,
			/// <summary>biped_lock_on_data_struct</summary>
			blod,
			/// <summary>global_collision_bsp_struct</summary>
			cbsp,
			/// <summary>constraint_bodies_struct</summary>
			csbs,
			/// <summary>animation_aiming_screen_struct</summary>
			aaim,

			/// <summary>sound_playback_parameter_definition</summary>
			spl1,
			/// <summary>sound_effect_struct_definition</summary>
//			ssfx,

			/// <summary>animation_graph_resources_struct</summary>
			MAgr,

			/// <summary>animation_graph_contents_struct</summary>
			MAgc,

			/// <summary>global_structure_physics_struct</summary>
			spdf,

			/// <summary>simple_platform_sound_playback_struct</summary>
			plsn_simple,
			/// <summary>sound_promotion_parameters_struct</summary>
			snpr,
		};
	};
};