/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if PLATFORM_IS_USER
#include <blamlib/Halo1/camera/director.hpp>
#include <YeloLib/configuration/c_configuration_value.hpp>
#include <YeloLib/configuration/c_configuration_container.hpp>
#include <YeloLib/open_sauce/settings/c_settings_singleton.hpp>

namespace Yelo
{
	namespace Enums
	{
		enum settings_adjustment_result : long_enum;

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
			_observer_update_flag =			FLAG(0),
			_observer_unk1_flag =			FLAG(1),
			_observer_unk2_flag =			FLAG(2),
			_observer_unk3_flag =			FLAG(3),
			_observer_unk4_flag =			FLAG(4),
			_observer_dont_pass_time_flag =	FLAG(5),
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

			byte_flags command_flags[Enums::k_number_of_director_game_modes]; // 3 in fp, 0 default
			PAD24;
			real commands[Enums::k_number_of_director_game_modes]; // all the same during transitions, 0.0f = command finished
		}; BOOST_STATIC_ASSERT( sizeof(s_camera_command) == 0x68 );

		struct s_observer
		{ 
			tag header_signature;				// 0x0
			s_camera_command* command_update;	// 0x4
			s_camera_command command;			// 0x8
			bool updated_for_frame;
			UNKNOWN_TYPE(bool);
			PAD16;

			// calculated / camera
			struct s_calculated_result
			{
				real_point3d position;			// 0x74
				s_scenario_location location;	// 0x80
				real_vector3d velocity;			// 0x88
				real_vector3d forward;			// 0x94
				real_euler_angles3d up;			// 0xA0
				real fov;						// 0xAC
			}result;	BOOST_STATIC_ASSERT( sizeof(s_calculated_result) == 0x3C );

			// focus. this structure isn't a real structure in the game's code (these fields are part of s_observer)
			struct s_calculated_origin
			{
				real_point3d position;	// 0xB0
				real_point3d offset;	// 0xBC
				real depth;				// 0xC8 distance
				real fov;				// 0xCC
				real_vector3d forward;	// 0xD0
				real_vector3d up;		// 0xDC
			}origin;	BOOST_STATIC_ASSERT( sizeof(s_calculated_origin) == 0x38 );

			byte __pad[432]; // 0xE8

			// I'm thinking these are actually real_matrix3x3
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

			void GetRightVector(real_vector3d& right) const					{ right = this->origin.forward ^ this->origin.up; }
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


		void Initialize();
		void Dispose();
		void Update();

		// Dump the current view state for use in screen-shot point plotting
		void DumpViewState(cstring name = NULL);
		void* DumpViewStateEvaluate(void** arguments);
	};




	namespace Fov
	{
		class c_settings_container
			: public Configuration::c_configuration_container
		{
		public:
			Configuration::c_configuration_value<real> m_field_of_view;
			Configuration::c_configuration_value<bool> m_ignore_fov_change_in_cinematics;
			Configuration::c_configuration_value<bool> m_ignore_fov_change_in_main_menu;

			c_settings_container();
			
		protected:
			const std::vector<i_configuration_value* const> GetMembers() final override;
		};

		class c_settings_fov
			: public Settings::c_settings_singleton<c_settings_container, c_settings_fov>
		{
		public:
			void PostLoad() final override;

			void PreSave() final override;
		};

		void Initialize();
		void Dispose();

		real GetFieldOfView();
		void SetFieldOfView(const real fov);
		void ResetFieldOfView();
	};
};
#endif