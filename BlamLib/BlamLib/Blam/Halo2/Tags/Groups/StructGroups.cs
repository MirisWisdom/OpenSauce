/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
﻿using System;
using BlamLib.TagInterface;

namespace BlamLib.Blam.Halo2
{
	public static partial class StructGroups
	{
		// NOTE: plsn && plsn_simple are the EXACT SAME fucking structure


		public static TagGroupCollection Groups;
		static void GroupsInitialize()
		{
			Groups = new TagGroupCollection(false,
				ires,
				irem,
				MTLO,
				SINF,
				SECT,
				PDAT,
				ISQI, // old shit
				BLOK,
				cbsp,
				csbs,
				uncs,
				usas,
				uHnd,
				ubms,
				_1234,
				ulYc,
				blod,
				chgr,
				chfl,
				chdd,
				chsn,
				chpy,
				trcv,
				HVPH,
				MAPP,
				masd_melee,
				mdps,
				easd,
				wtsf,
				wSiS,
				wItS,
				wtas,
				wtcs,
				wbde,
				CLFN,
				SCFN,
				PRPS,
				PRPC,
				shtb,
				GPUR,
				GPUS,
				masd_sound,
				mphp,
				msst,
				sd2s,
				avlb,
				svis,
				igri,
				spdf,
				obj_,
				rnli,
				sobj,
				sper,
				sct3,
				sunt,
				seqt,
				swpt,
				sdvt,
				smht,
				sctt,
				slft,
				_sc_,
				slit,
				ntor,
				sszd,
				masd_damage,
				SFDS,
				RFDS,
				RBDS,
				dsfx,
				hwis,
				hwsd,
				hwef,
				nhd2,
				sebs,
				nhgs_const,
				nhgs,
				cnvs,
				gapu,
				spl1,
				ssfx,
				plsn,
				prli,
				scli,
				amli,
				lmsh,
				aaim,
				apds,
				qoSS,
				MAgr,
				ANII,
				ATSS_trans,
				ATSS_dest,
				MAgc,
				MArt,
				clpr,
				WNDM,
				sngl,
				snpl,
				snsc,
				plsn_simple,
				snpr
			);
		}

		/// <summary>
		/// Constant indices to each tag group in <see cref="Groups"/>
		/// </summary>
		public enum Enumerated : int
		{
			/// <summary>instantaneous_response_damage_effect_struct</summary>
			ires,
			/// <summary>instantaneous_response_damage_effect_marker_struct</summary>
			irem,
			/// <summary>model_target_lock_on_data_struct</summary>
			MTLO,
			/// <summary>global_geometry_section_info_struct</summary>
			SINF,
			/// <summary>global_geometry_section_struct</summary>
			SECT,
			/// <summary>global_geometry_point_data_struct</summary>
			PDAT,
			/// <summary>global_geometry_isq_info_struct</summary>
			ISQI, // old shit
			/// <summary>global_geometry_block_info_struct</summary>
			BLOK,
			/// <summary>global_collision_bsp_struct</summary>
			cbsp,
			/// <summary>constraint_bodies_struct</summary>
			csbs,
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
			/// <summary>biped_lock_on_data_struct</summary>
			blod,
			/// <summary>character_physics_ground_struct</summary>
			chgr,
			/// <summary>character_physics_flying_struct</summary>
			chfl,
			/// <summary>character_physics_dead_struct</summary>
			chdd,
			/// <summary>character_physics_sentinel_struct</summary>
			chsn,
			/// <summary>character_physics_struct</summary>
			chpy,
			/// <summary>torque_curve_struct</summary>
			trcv,
			/// <summary>havok_vehicle_physics_struct</summary>
			HVPH,
			/// <summary>mapping_function</summary>
			MAPP,
			/// <summary>melee_aim_assist_struct</summary>
			masd_melee,
			/// <summary>melee_damage_parameters_struct</summary>
			mdps,
			/// <summary>aim_assist_struct</summary>
			easd,
			/// <summary>weapon_tracking_struct</summary>
			wtsf,
			/// <summary>weapon_shared_interface_struct</summary>
			wSiS,
			/// <summary>weapon_interface_struct</summary>
			wItS,
			/// <summary>weapon_trigger_autofire_struct</summary>
			wtas,
			/// <summary>weapon_trigger_charging_struct</summary>
			wtcs,
			/// <summary>weapon_barrel_damage_effect_struct</summary>
			wbde,
			/// <summary>color_function_struct</summary>
			CLFN,
			/// <summary>scalar_function_struct</summary>
			SCFN,
			/// <summary>particle_property_scalar_struct_new</summary>
			PRPS,
			/// <summary>particle_property_color_struct_new</summary>
			PRPC,
			/// <summary>tag_block_index_struct</summary>
			shtb,
			/// <summary>shader_gpu_state_reference_struct</summary>
			GPUR,
			/// <summary>shader_gpu_state_struct</summary>
			GPUS,
			/// <summary>sound_response_extra_sounds_struct</summary>
			masd_sound,
			/// <summary>material_physics_properties_struct</summary>
			mphp,
			/// <summary>materials_sweeteners_struct</summary>
			msst,
			/// <summary>super_detonation_damage_struct</summary>
			sd2s,
			/// <summary>angular_velocity_lower_bound_struct</summary>
			avlb,
			/// <summary>visibility_struct</summary>
			svis,
			/// <summary>structure_instanced_geometry_render_info_struct</summary>
			igri,
			/// <summary>global_structure_physics_struct</summary>
			spdf,
			/// <summary>scenario_object_id_struct</summary>
			obj_,
			/// <summary>render_lighting_struct</summary>
			rnli,
			/// <summary>scenario_object_datum_struct</summary>
			sobj,
			/// <summary>scenario_object_permutation_struct</summary>
			sper,
			/// <summary>scenario_scenery_datum_struct_v4</summary>
			sct3,
			/// <summary>scenario_unit_struct</summary>
			sunt,
			/// <summary>scenario_equipment_datum_struct</summary>
			seqt,
			/// <summary>scenario_weapon_datum_struct</summary>
			swpt,
			/// <summary>scenario_device_struct</summary>
			sdvt,
			/// <summary>scenario_machine_struct_v3</summary>
			smht,
			/// <summary>scenario_control_struct</summary>
			sctt,
			/// <summary>scenario_light_fixture_struct</summary>
			slft,
			/// <summary>sound_scenery_datum_struct</summary>
			_sc_,
			/// <summary>scenario_light_struct</summary>
			slit,
			/// <summary>scenario_netgame_equipment_orientation_struct</summary>
			ntor,
			/// <summary>static_spawn_zone_data_struct</summary>
			sszd,
			/// <summary>damage_outer_cone_angle_struct</summary>
			masd_damage,
			/// <summary>screen_flash_definition_struct</summary>
			SFDS,
			/// <summary>vibration_frequency_definition_struct</summary>
			RFDS,
			/// <summary>vibration_definition_struct</summary>
			RBDS,
			/// <summary>damage_effect_sound_effect_definition</summary>
			dsfx,
			/// <summary>hud_widget_inputs_struct</summary>
			hwis,
			/// <summary>hud_widget_state_definition_struct</summary>
			hwsd,
			/// <summary>hud_widget_effect_function_struct</summary>
			hwef,
			/// <summary>new_hud_dashlight_data_struct</summary>
			nhd2,
			/// <summary>screen_effect_bonus_struct</summary>
			sebs,
			/// <summary>global_new_hud_globals_constants_struct</summary>
			nhgs_const,
			/// <summary>global_new_hud_globals_struct</summary>
			nhgs,
			/// <summary>global_new_hud_globals_struct</summary>
			cnvs,
			/// <summary>grenade_and_powerup_struct</summary>
			gapu,
			/// <summary>sound_playback_parameter_definition</summary>
			spl1,
			/// <summary>sound_effect_struct_definition</summary>
			ssfx,
			/// <summary>platform_sound_playback_struct</summary>
			plsn,
			/// <summary>primary_light_struct</summary>
			prli,
			/// <summary>secondary_light_struct</summary>
			scli,
			/// <summary>ambient_light_struct</summary>
			amli,
			/// <summary>lightmap_shadows_struct</summary>
			lmsh,
			/// <summary>animation_aiming_screen_struct</summary>
			aaim,
			/// <summary>packed_data_sizes_struct</summary>
			apds,
			/// <summary>quantized_orientation_struct</summary>
			qoSS,
			/// <summary>animation_graph_resources_struct</summary>
			MAgr,
			/// <summary>animation_index_struct</summary>
			ANII,
			/// <summary>animation_transition_state_struct</summary>
			ATSS_trans,
			/// <summary>animation_destination_state_struct</summary>
			ATSS_dest,
			/// <summary>animation_graph_contents_struct</summary>
			MAgc,
			/// <summary>model_animation_runtime_data_struct</summary>
			MArt,
			/// <summary>cloth_properties</summary>
			clpr,
			/// <summary>global_wind_model_struct</summary>
			WNDM,
			/// <summary>sound_global_mix_struct</summary>
			sngl,
			/// <summary>sound_playback_parameters_struct</summary>
			snpl,
			/// <summary>sound_scale_modifiers_struct</summary>
			snsc,
			/// <summary>simple_platform_sound_playback_struct</summary>
			plsn_simple,
			/// <summary>sound_promotion_parameters_struct</summary>
			snpr,
		};
	};
};