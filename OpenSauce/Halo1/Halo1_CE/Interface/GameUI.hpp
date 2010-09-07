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
#include "Objects/ObjectDefinitions.hpp" // for game teams

namespace Yelo
{
	namespace Enums
	{
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

		enum hud_chat_type : long_enum
		{
			_hud_chat_type_none = CAST(long_enum,NONE),

			_hud_chat_type_all = 0,
			_hud_chat_type_team,
			_hud_chat_type_vehicle,
			_hud_chat_type_server,
			_hud_chat_type_game_msg, // used for messages pulled from Strings.dll. Convert 'hud_chat_network_data.message' to a long using 'wtol' to get the resource ID
		};
	};

	namespace Flags
	{
		enum unit_interface_flags
		{
			_unit_interface_show_health_bit = BIT32(0),
			_unit_interface_blink_health_bit = BIT32(1),
			_unit_interface_show_shield_bit = BIT32(2),
			_unit_interface_blink_shield_bit = BIT32(3),
			_unit_interface_show_motion_sensor_bit = BIT32(4),
			_unit_interface_blink_motion_sensor_bit = BIT32(5),
		};

		enum weapon_interface_show : byte_flags // TODO: verify this flags field size
		{
			_weapon_interface_show_crosshair_bit = BIT32(0),
		};

		enum text_flag : long_flags
		{
			_text_flag1 = BIT32(0),
			_text_flag2 = BIT32(1),
			_text_flag4 = BIT32(2),
			_text_flag8 = BIT32(3),
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


		struct s_hud_unit_interface_unit : TStructImpl(0x58)
		{
			TStructGetPtrImpl(real, Shields, 0x0);
			TStructGetPtrImpl(real, Health, 0x4);
			TStructGetPtrImpl(datum_index, Unit, 0x1C);
			TStructGetPtrImpl(int16, IntegratedLightPower, 0x20); // 0x20 0 = off 1 = on
			TStructGetPtrImpl(int16, IntegratedLightTime, 0x22);
		};
		struct s_hud_unit_interface
		{
			s_hud_unit_interface_unit units[1]; // 0x0
			long_flags flags; // 0x58, unit_interface_flags
		}; BOOST_STATIC_ASSERT( sizeof(s_hud_unit_interface) == 0x5C );
		s_hud_unit_interface*		HudUnitInterface();


		struct s_hud_weapon_interface_player : TStructImpl(0x50)
		{
		};
		struct s_hud_weapon_interface : TStructImpl(0x7C)
		{
			// int32 zoom_level 0x24

			TStructGetPtrImpl(datum_index, WeaponIndex, 0x18);
			TStructGetPtrImpl(s_hud_weapon_interface_player, LocalPlayers, 0x28);
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

		struct s_first_person_weapons : TStructImpl(0x1EA0)
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

			// 0x50 bool
			// 0x60 real_euler_angles2d

			// _enum shotgun_reload_type; // 0x1E94
			// _shotgun_reload_type_last_round = 1
			// _shotgun_reload_type_first_and_last_round = 2

			// node matrices start at 0x108C, with each one being sizeof(0x34) of course
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


		struct s_hud_chat_globals_data
		{
			bool active; PAD24;
			Enums::hud_chat_type chat_type;

			PAD(0, sizeof(byte)*132);	// unused...ever
			real_argb_color color;		// unused, except upon initialization
			UNKNOWN_TYPE(tag_string);	// unused, except upon initialization
 
			// PRETTY damn sure this is a file_reference that was never 
			// implemented for saving the hud chat
			PAD(1, sizeof(byte)*0x10C);	// unused, except upon initialization.

			uint32 line_timers[8];
		};
		s_hud_chat_globals_data*		HudChatGlobals();
		int32 HudChatLineCount();


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