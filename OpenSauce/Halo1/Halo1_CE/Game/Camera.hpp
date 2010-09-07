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

#if PLATFORM_IS_USER

namespace Yelo
{
	namespace Enums
	{
		enum camera_script_mode : _enum
		{
			_camera_script_mode_none = _enum(NONE),
			_camera_script_mode_unk = 0,
			_camera_script_mode_controlled,
			_camera_script_mode_first_person,
			_camera_script_mode_dead,
		};
	};

	namespace Flags
	{
		enum observer_globals_data_flags : byte_flags
		{
			_observer_update_bit =			BIT32(0),
			_observer_unk1_bit =			BIT32(1),
			_observer_unk2_bit =			BIT32(2),
			_observer_unk3_bit =			BIT32(3),
			_observer_unk4_bit =			BIT32(4),
			_observer_dont_pass_time_bit =	BIT32(5),
		};
	};

	namespace Camera
	{
		struct s_camera_command
		{
			Flags::observer_globals_data_flags flags;
			PAD24;

			real_point3d position;
			real_point3d offset;
			real depth;
			real fov;
			real_vector3d forward;
			real_vector3d up;
			real_vector3d vector;
			real time; // 0 when ever not in transition into 3rdperson

			byte_flags command_flags[5]; // [0] = 3 in fp, 0 default, [4] = 3 in fp, 0 default
			PAD24;
			real commands[5]; // all the same during transitions, 0.0f = command finished
		}; BOOST_STATIC_ASSERT( sizeof(s_camera_command) == 0x68 );

		struct s_player_director_data
		{
			s_camera_command command;
			bool UNKNOWN(0);
			PAD24;
			real_vector3d UNKNOWN(1)[4];
			real UNKNOWN(2);
		}; BOOST_STATIC_ASSERT( sizeof(s_player_director_data) == 0xA0 );

		struct s_observer
		{ 
			tag header_signature;		// 0x0
			s_player_director_data* local_player_director; // 0x4, only one local player so this will always be 0x647420 in HaloCE 1.08

			s_camera_command command;	// 0x8

			bool updated_for_frame;	// 0x70
			UNKNOWN_TYPE(bool);		// 0x71, equal TRUE
			PAD16;					// 0x72

			// calculated / camera
			struct {
				real_point3d position;	// 0x74
				struct { // TODO: s_scenario_location
					int32 leaf_index;	// 0x80, changes when thirdperson rotating (1318,1270)
					int16 cluster_index;// 0x84
					UNKNOWN_TYPE(int16);// 0x86, award
				}location;
				real_vector3d velocity;	// 0x88
				real_vector3d forward;	// 0x94
				real_euler_angles3d up;	// 0xA0
				real fov;				// 0xAC
			}result;

			struct {
				real_point3d position;	// 0xB0
				real_point3d offset;	// 0xBC
				real distance;			// 0xC8
			}focus;

			real field_of_view;			// 0xCC
			real_vector3d forward;		// 0xD0
			real_vector3d up;			// 0xDC

			byte __pad[432]; // 0xE8

			// k_count = 11
			// 0x0E8 real velocities[k_count]

			// 0x114, 0x118, 0x11C

			// 0x120 real ?[k_count]

			// 0x14C, 0x150, 0x154

			// 0x158 real ?[k_count]
			// 0x184 real ?[k_count]
			// 0x1B0 real ?[k_count]
			// 0x1DC real ?[k_count]
			// 0x208 real ?[k_count]
			// 0x234 real ?[k_count]
			// 0x260 real ?[k_count]

			// 0x28C, 0x290, 0x294

			tag trailer_signature; // 0x298

			void GetRightVector(real_vector3d& right) const					{ right = this->forward ^ this->up; }
		}; BOOST_STATIC_ASSERT( sizeof(s_observer) == 0x29C );
		s_observer*		Observer();

		struct s_camera_script_globals_data
		{
			bool initialized; // set this and [enabled] to true to enable scripted camera
			bool enabled;
			Enums::camera_script_mode mode;
			struct {
				int16 next; // next camera point index
				PAD16;
				real time; // multiply by 30 to then ftol to get camera_time
				real_point3d position;
				real_vector3d forward;
				real_euler_angles3d up;
				real fov;
			}camera_point;
			datum_index owner_unit;

			struct {
				datum_index tag_index; // [model_animation]
				int16 animation_index; // index in the animations block
			}animation;
		};
		s_camera_script_globals_data*	CameraScriptGlobals();


		struct s_director_scripting_data
		{
			bool initialized;
			PAD24;
		};
		s_director_scripting_data*	DirectorScripting();

		struct s_cinematic_globals_data
		{
			real unknown1;
			uint32 time;
			bool show_letterbox;
			bool in_progress;
			bool can_be_skipped;
			bool suppress_bsp_object_creation;
			struct {
				int16 title_index;
				int16 title_delay;
			}chapter_titles[4];
		};
		s_cinematic_globals_data*		CinematicGlobals();

		struct s_director_data : TStructImpl(0xF8)
		{
		};
		s_director_data*				GlobalDirector();


		void Initialize();
		void Dispose();
		void Update();

		// Dump the current view state for use in screen-shot point plotting
		void DumpViewState(cstring name = NULL);
		void* DumpViewStateEvaluate(void** arguments);
	};




	namespace Fov
	{
		void Initialize();
		void Dispose();
		void Update();

		bool RequiresZoomFix();

		bool AdjustSettings();
		void LoadSettings(TiXmlElement* fov_element);
		void SaveSettings(TiXmlElement* fov_element);

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