/*
    Yelo: Open Sauce SDK
		Halo 1 (CE) Edition
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
#pragma once

#if !PLATFORM_IS_DEDI
#include <TagGroups/Halo1/global_definitions.hpp> // for game teams

namespace Yelo
{
	namespace Enums
	{
		enum {
			MAXIMUM_NUMBER_OF_HUD_SOUNDS = 12,
		};

		enum text_style : _enum
		{
			_text_style_plain = CAST(_enum,NONE),

			_text_style_bold = 0,
			_text_style_italic,
			_text_style_condense,
			_text_style_underline,
		};

		enum text_justification : _enum
		{
			_text_justification_left,
			_text_justification_right,
			_text_justification_center,
		};

		enum rasterize_score : long_enum
		{
			_rasterize_score_welcome,
			_rasterize_score_s_died,
			_rasterize_score_s_was_killed_by_guardians,
			_rasterize_score_s_was_killed_by_vehicle,
			_rasterize_score_s_was_killed_by_s,
			_rasterize_score_s_was_betrayed_bs_s,
			_rasterize_score_s_commited_suicide,
			_rasterize_score_double_kill,
			_rasterize_score_you_killed_s,
			_rasterize_score_triple_kill,
			_rasterize_score_killtacular,
			_rasterize_score_killing_spree,
			_rasterize_score_running_riot,
			_rasterize_score_you_betrayed_s,
			_rasterize_score_killtacular_d,
			_rasterize_score_triple_kill_d,
			_rasterize_score_double_kill_d,
			_rasterize_score_running_riot_d,
			_rasterize_score_started_killing_spree,
			_rasterize_score_you_killed_s_d,
			_rasterize_score_you_are_odd_man_out,
			_rasterize_score_you_are_out_of_lives,
			_rasterize_score_rejoin_in_d,
			_rasterize_score_waiting_for_space,
			_rasterize_score_you_quit,
			_rasterize_score_s_quit,
			_rasterize_score_hold_s_for_score,
			_rasterize_score_d_remaining,
			_rasterize_score_telefragged,
			_rasterize_score,
		};

		enum weapon_hud_state
		{
			_weapon_hud_state_total_ammo,
			_weapon_hud_state_loaded_ammo,
			_weapon_hud_state_heat,
			_weapon_hud_state_age,
			_weapon_hud_state_secondary_weapon_total_ammo,
			_weapon_hud_state_secondary_weapon_loaded_ammo,
			_weapon_hud_state_dist_to_target,
			_weapon_hud_state_elevation_to_target,
			_weapon_hud_state,
		};
	};

	namespace Flags
	{
		enum unit_interface_flags
		{
			_unit_interface_show_health_flag = FLAG(0),
			_unit_interface_blink_health_flag = FLAG(1),
			_unit_interface_show_shield_flag = FLAG(2),
			_unit_interface_blink_shield_flag = FLAG(3),
			_unit_interface_show_motion_sensor_flag = FLAG(4),
			_unit_interface_blink_motion_sensor_flag = FLAG(5),
		};

		enum weapon_interface_show : byte_flags // TODO: verify this flags field size
		{
			_weapon_interface_show_crosshair_flag = FLAG(0),
		};

		enum text_flag : long_flags
		{
			_text_flag1 = FLAG(0),
			_text_flag2 = FLAG(1),
			_text_flag4 = FLAG(2),
			_text_flag8 = FLAG(3),
		};
	};

	namespace GameUI
	{
		struct s_hud_scripted_globals
		{
			bool show_hud;
			bool show_hud_help_text;
			PAD16;
		};
		s_hud_scripted_globals*		HudScriptedGlobals();

		struct s_hud_messaging : TStructImpl(1160)
		{
		};
		s_hud_messaging*			HudMessaging();


		struct s_hud_unit_interface_unit
		{
			real shields, health;
			UNKNOWN_TYPE(real);
			uint32 last_update_time;
			struct {
				uint32 hud_background_element;
				uint32 health_panel_background_element;
				uint32 motion_sensor_elements;
			}last_render_time;
			datum_index unit_index;
			int16 integrated_light_power; // 0 = off 1 = on
			int16 integrated_light_time;
			word_flags active_sound_elements;
			PAD16;
			datum_index sound_elements[Enums::MAXIMUM_NUMBER_OF_HUD_SOUNDS]; // sound cache index
		}; BOOST_STATIC_ASSERT( sizeof(s_hud_unit_interface_unit) == 0x58 );
		struct s_hud_unit_interface
		{
			s_hud_unit_interface_unit units[1]; // 0x0
			long_flags flags; // 0x58, unit_interface_flags
		}; BOOST_STATIC_ASSERT( sizeof(s_hud_unit_interface) == 0x5C );
		s_hud_unit_interface*		HudUnitInterface();


		struct s_hud_weapon_interface_player : TStructImpl(0x50)
		{
			// 0x0, uint32 state_last_render_times[Enums::_weapon_hud_state]; // last render for each weapon hud state type
			// 0x20, datum_index weapon_index
			// 0x24, real

			// 0x4C, long_flags render_crosshair_types_mask
		};
		struct s_hud_weapon_interface : TStructImpl(0x7C)
		{
			TStructGetPtrImpl(s_hud_weapon_interface_player, LocalPlayers, 0x0);
			TStructGetPtrImpl(Flags::weapon_interface_show, ShowHudFlags, 0x78);
		};
		s_hud_weapon_interface*		HudWeaponInterface();

		struct s_hud_nav_points
		{
			struct s_nav_point
			{
				int16 navpoint_index;
				struct {
					// 0 - cutscene flag
					// 1 - object
					// 2 - game goal (objective)
					// 3 - deactivated
					uint16 type : 4;
				};
				// cutscene flag index
				// object = datum_index
				// game goal index
				int32 type_index;
				real vertical_offset;
			};
			struct s_local_player
			{
				s_nav_point nav_points[4];
			};

			s_local_player local_players[1];
		}; BOOST_STATIC_ASSERT( sizeof(s_hud_nav_points) == 0x30 );
		s_hud_nav_points*			HudNavPoints();

		struct s_motion_sensor
		{
			/*
				0 local player // 1, .5, 0
				1 friendly team // 1, 1, 0
				2 enemy team // 1, 0, 0
				3 friendly team vehicle // 1, 1, 0
				4 enemy team vehicle (c_dropship) // 1, 0, 0
				5 objective // .5, .5, 1
				6 = _blip_type_none // 0,0,0

				16 = MAX_CUSTOM_BLIPS
			*/

			struct s_custom_blip
			{
				sbyte x;
				sbyte y;
			};
			struct s_blip : s_custom_blip
			{
				byte_enum type; // [blip_type], set to _blip_type_none when not used
				byte_enum size; // a la object's size (tiny, large, etc)
			}; BOOST_STATIC_ASSERT( sizeof(s_blip) == 0x4 );
			struct s_team_data
			{
				s_blip object_blips[16];	// objects belonging to just this team
				struct {
					s_custom_blip goal_blips[16]; // MAX_CUSTOM_BLIPS
					sbyte goal_indices[16]; // indices to the game engine's global goals
				}game_engine;

				real_point2d world_position;
				UNKNOWN_TYPE(int32); // game time related
				UNKNOWN_TYPE(real); // related to calculations with objects and facing angle of the player
				byte active_object_blips_count;
				PAD24;
			}; BOOST_STATIC_ASSERT( sizeof(s_team_data) == 0x84 );
			struct s_local_player
			{
				s_team_data nearby_team_objects[Enums::_global_game_team];
				
				byte UNKNOWN(0)[0x40];
			}; BOOST_STATIC_ASSERT( sizeof(s_local_player) == 0x568 );


			s_local_player local_players[1];

			struct {
				UNKNOWN_TYPE(int32); // game time related
				int16 current_object_blip_index;
				PAD8; // actually boolean, but you don't need to worry about this value!
				PAD8;
			}update_data;
		}; BOOST_STATIC_ASSERT( sizeof(s_motion_sensor) == 0x570 );
		s_motion_sensor*			MotionSensor();

		struct s_first_person_weapon : TStructImpl(0x1EA0)
		{
			TStructGetPtrImpl(bool, IsVisible, 0x0);
			TStructGetPtrImpl(datum_index, Unit, 0x4);
			TStructGetPtrImpl(datum_index, CurrentWeapon, 0x8);
			// 1 = o-h-enter, 3 = overheated, 4 = charging, 5 = posing, 6 = firing1, 10 = meleeing, 
			// 13 = reload empty, 14 = reload full, 19 = switching/ready, 22 = o-h-exit, 23 = firing2
			TStructGetPtrImpl(int16, AnimationState, 0xC);
			// 0xE = unknown 2 bytes
			TStructGetPtrImpl(int16, IdleTime, 0x10);
			TStructGetPtrImpl(int16, CurrentBaseAnimationFrameIndex, 0x12); // frame index of current base animation (idle or moving)
			TStructGetPtrImpl(int16, CurrentOverlayAnimationIndex, 0x14);
			TStructGetPtrImpl(int16, AnimationCurrentIndex, 0x16);
			TStructGetPtrImpl(int16, AnimationCurrentFrameIndex, 0x18);
			TStructGetPtrImpl(int16, CurrentMovingOverlayAnimationIndex, 0x1A);
			TStructGetPtrImpl(int16, CurrentMovingOverlayFrameIndex, 0x1C);
			// PAD16?
			//TStructGetPtrImpl(int16, , 0x20); // some animation index
			//TStructGetPtrImpl(real, , 0x28); // frame index (real) for the above anim index
			//TStructGetPtrImpl(real, , 0x2C);
			// 0x30?
			//TStructGetPtrImpl(real, , 0x34);
			// 0x38?
			// 0x3C?
			//TStructGetPtrImpl(real, , 0x40);
			//TStructGetPtrImpl(real, , 0x44);
			// 0x48?
			// 0x4C?
			// 0x50 bool
			// 0x54 real_vector3d forward?
			// 0x60 real_euler_angles2d
			// 0x68 real_euler_angles2d
			// 0x70 real_point3d
			// 0x7C real_point3d?
			//////////////////////////////////////////////////////////////////////////
			// 0x88 int16 frame index
			// 0x8A int16 frame count
			// 0x8C real_orientation3d node orientations[128]
			// 0x108C real_matrix4x3 node matrices[64]
			//////////////////////////////////////////////////////////////////////////
			// 0x1D8C bool remapped weapon?
			// PAD8;
			// 0x1D8E int16 node_remappings[64]
			//////////////////////////////////////////////////////////////////////////
			// 0x1E0E bool remapped hands
			// PAD8;
			// 0x1E10 int16 node_remappings[64]
			//////////////////////////////////////////////////////////////////////////
			// 0x1E90 ?
			// 0x1E94 _enum shotgun_reload_type;
			//		_shotgun_reload_type_last_round = 1
			//		_shotgun_reload_type_first_and_last_round = 2
			//TStructGetPtrImpl(int32, , 0x1E98);
			//TStructGetPtrImpl(int16, , 0x1E9C);
		};
		struct s_first_person_weapons : TStructImpl(0x1EA0)
		{
			s_first_person_weapon local_players[1];
		};
		s_first_person_weapons*		FirstPersonWeapons();


		struct s_text_bounds_globals_data
		{
			rectangle2d bounds;
			void* styled_font_tag; // 'font'
		};
		s_text_bounds_globals_data*	TextBoundsGlobals();

		struct s_font_drawing_globals_data
		{
			datum_index internal_string_localization_tag;
			datum_index font_tag;
			Flags::text_flag flags;
			Enums::text_style style;
			Enums::text_justification justification;
			real_argb_color color;
			int16 tab_stop_count;
			wchar_t tab_stops[16];
			int16 highlight_start;
			int16 highlight_end;
			int16 initial_indent;
			int16 paragraph_indent[72];
		};
		s_font_drawing_globals_data*	FontDrawingGlobals();


		void Initialize();
		void Dispose();
		void PLATFORM_API Update();

		void PLATFORM_API FirstPersonWeaponsUpdate();
	};


	namespace Hud
	{
		void Initialize();
		void Dispose();

		bool HudIsScaled();
		void* HudGlobals(); // get the hud_globals tag definition

		bool AdjustSettings();
		void LoadSettings(TiXmlElement* hud_element);
		void SaveSettings(TiXmlElement* hud_element);

#if defined(DX_WRAPPER) // TODO: Need fallback initialization for when we're not using the DX_WRAPPER
		void Scale(void* ref_ptr, void *pVertexStreamZeroData);

		void Initialize3D(IDirect3DDevice9 *pDevice, D3DPRESENT_PARAMETERS *pPP);
		void OnLostDevice();
		void OnResetDevice(D3DPRESENT_PARAMETERS *pPP);
		void Render();
		void Release();
#endif

	};


	namespace Menu
	{
		void Initialize();
		void Dispose();

#if defined(DX_WRAPPER) // TODO: Need fallback initialization for when we're not using the DX_WRAPPER
		void Initialize3D(IDirect3DDevice9 *pDevice, D3DPRESENT_PARAMETERS *pPP);
		void OnLostDevice();
		void OnResetDevice(D3DPRESENT_PARAMETERS *pPP);
		void Render();
		void Release();
#endif
	};
};
#endif