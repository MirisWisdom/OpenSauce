/*
    BlamLib: .NET SDK for the Blam Engine

    Copyright (C) 2005-2010  Kornner Studios (http://kornner.com)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
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

				MAgr,

				MAgc,

				spdf,

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

			/// <summary>animation_graph_resources_struct</summary>
			MAgr,

			/// <summary>animation_graph_contents_struct</summary>
			MAgc,

			/// <summary>global_structure_physics_struct</summary>
			spdf,

			/// <summary>sound_promotion_parameters_struct</summary>
			snpr,
		};
	};
};