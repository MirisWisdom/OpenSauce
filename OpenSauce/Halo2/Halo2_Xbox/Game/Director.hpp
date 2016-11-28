/*
	Yelo: Open Sauce SDK
		Halo 2 (Xbox) Edition

	See license\OpenSauce\Halo2_Xbox for specific license information
*/
#pragma once

#include <blamlib/Halo2/interface/user_interface.hpp>

namespace Yelo
{
	namespace Enums
	{
		enum director_mode
		{
			_director_mode_none,
			_director_mode_scripted,
			_director_mode_first_person,
			_director_mode_following,
			_director_mode_editor,
			_director_mode_dead,

			_director_mode,

			_director_mode_third_person, // this is CUSTOM mode.
			_director_mode_debug, // this is a CUSTOM mode
			_director_mode_static, // this is a CUSTOM mode (aka stillcam)
		};

		enum resistance_function
		{
			// resistance is directly proportional to velocity
			_resistance_function_linear,
			// resistance exponentially increases with velocity
			_resistance_function_exponential,
			// resistance increases with velocity but gradually slows the rate at which it increases
			_resistance_function_logarithmic,

			_resistance_function,
		};
	};

	namespace GameState
	{
		struct s_camera_command
		{
			byte_flags flags;
			PAD24;

			real_point3d position;			// 0x4
			real_point3d offset;			// 0x10
			real depth;						// 0x1C
			real fov;						// 0x20
			UNKNOWN_TYPE(real);				// 0x24
			UNKNOWN_TYPE(real);				// 0x28, seems to be another fov value
			real_vector3d forward;			// 0x2C
			real_vector3d up;				// 0x38
			real_vector3d vector;			// 0x44			
			real_matrix4x3 matrix;			// 0x50
			UNKNOWN_TYPE(int32);			// 0x84
			UNKNOWN_TYPE(int32);			// 0x88
			byte_flags command_flags[6];	// 0x8C
			PAD16;
			real commands[6];				// 0x94
		}; BOOST_STATIC_ASSERT( sizeof(s_camera_command) == 0xAC );
	};

	namespace Camera
	{
		typedef void (API_FUNC* camera_update_proc)(GameState::s_camera_command* camera, datum_index* player_index, GameState::s_camera_command* result);

		struct s_director_camera // TODO: Alpha is a bit different from retail (s_camera_command?)
		{
			UNKNOWN_TYPE(int32);
			UNKNOWN_TYPE(real);
			UNKNOWN_TYPE(int32);
			camera_update_proc UpdateProc;
			//////////////////////////////////////////////////////////////////////////
			// union here...data is dependent on the director mode
			TAG_PAD(int32, 19);
			//////////////////////////////////////////////////////////////////////////
			GameState::s_camera_command UpdateResult; // 0x5C
			UNKNOWN_TYPE(bool); // 0x108
			PAD24;
			UNKNOWN_TYPE(real); // 0x10C
			TAG_PAD(int32, 12); // 0x110
		}; BOOST_STATIC_ASSERT( sizeof(s_director_camera) == PLATFORM_VALUE(0x140, 0x140, 0x134) );
		
		struct s_director_globals
		{
			UNKNOWN_TYPE(real);
			UNKNOWN_TYPE(long_enum); // Enums::director_mode
			UNKNOWN_TYPE(bool);
			PAD24;
			s_director_camera Directors[Enums::k_number_of_users]; // director for each local player
		}; BOOST_STATIC_ASSERT( sizeof(s_director_globals) == 0x50C );


		void Initialize();
		void Dispose();
		void Update();

		camera_update_proc DirectorModeProc(_enum mode);
	};

	namespace GameState
	{
		Camera::s_director_globals* _DirectorGlobals();


		struct s_observer
		{
			struct s_calculated_result
			{
				real_point3d position;			// 0x0
				s_scenario_location location;	// 0xC
				real_vector3d velocity;			// 0x14
				real_vector3d forward;			// 0x20
				real_euler_angles3d up;			// 0x2C
				real fov;						// 0x38
				UNKNOWN_TYPE(int32);			// 0x3C, i'm not sure what this field's type is...
				UNKNOWN_TYPE(real);				// 0x40
				UNKNOWN_TYPE(real);				// 0x44
			}; BOOST_STATIC_ASSERT( sizeof(s_calculated_result) == 0x48 );

			// focus. this structure isn't a real structure in the game's code (these fields are part of s_observer)
			struct s_calculated_origin
			{
				real_point3d position;		// 0x0
				real_point3d offset;		// 0xC
				real_point2d look_offset;	// 0x18
				real depth;					// 0x20 distance
				real fov;					// 0x24
				real_vector3d forward;		// 0x28 Direction
				real_vector3d up;			// 0x34 Rotation
			}; BOOST_STATIC_ASSERT( sizeof(s_calculated_origin) == 0x40 );

			tag header_signature;				// 0x0

			s_camera_command* command_update;	// 0x4
			s_camera_command command;			// 0x8
			bool updated_for_frame;				// 0xB4
			UNKNOWN_TYPE(bool);
			UNKNOWN_TYPE(bool);
			PAD8;
			s_calculated_result result;			// 0xB8
			UNKNOWN_TYPE(int32); UNKNOWN_TYPE(int32); UNKNOWN_TYPE(int32); // 12 bytes in between
			s_calculated_origin origin;			// 0x10C
			real_matrix4x3 matrix;				// 0x14C

			real_matrix4x3 matrix0;				// 0x180
			real_matrix4x3 matrix1;				// 0x1B4
			real_matrix4x3 matrix2;				// 0x1E8
			real_matrix4x3 matrix3;				// 0x21C
			real_matrix4x3 matrix4;				// 0x250
			real_matrix4x3 matrix5;				// 0x284
			real_matrix4x3 matrix6;				// 0x2B8
			real_matrix4x3 matrix7;				// 0x2EC
			real_matrix4x3 matrix8;				// 0x320

			tag trailer_signature;				// 0x354
		}; BOOST_STATIC_ASSERT( sizeof(s_observer) == 0x358 );
		s_observer* _Observers();

		struct s_camera_globals
		{
			bool Active;
			PAD24;
		};
		s_camera_globals* _CameraGlobals();

		struct s_scripted_camera_globals
		{
			bool Initialized; // 0x0
			bool Enabled; // 0x1
			_enum Mode; // 0x2
			real Time; // 0x4

			struct {
				uint32 FovTime; // 0x8
				real FovOld; // 0xC
				real Fov; // 0x10
				uint32 Ticks; // 0x14
				real_point3d Position; // 0x18
				real_vector3d Forward; // 0x24
				real_euler_angles3d Up; // 0x30
				datum_index ObjectIndex; // 0x3C
			}Unit;

			struct {
				uint32 StartTick; // 0x40
				datum_index TagDefinition; // 0x44
				string_id Name; // 0x48
				int16 NextCameraPoint; // 0x4C
				PAD16;
			}Animation;

			real_point3d UNKNOWN(0); // 0x50
			TAG_PAD(int32, 7);
		};
		s_scripted_camera_globals* _ScriptedCameraGlobals();

		struct s_observer_globals
		{
			int32 BspIndex; // 0x0
			uint32 UNKNOWN(0)[4]; // index of the block at [structure_bsp+0xAC]
		};
		s_observer_globals* _ObserverGlobals();

		struct s_cinematic_globals
		{
			real Time; // 0x0
			bool ShowLetterbox; // 0x4
			bool InProgress; // 0x5
			bool SkipStart; // 0x6
			PAD8;
			struct _Titles
			{
				int16 BlockIndex;
				int16 DelayTime;
			}Titles[4]; // 0x8
			struct {
				string_id Id; // 0x18
				real Volume; // 0x1C, idk where I got the name 'volume' so feel free to kiss my ass...i think it's 'up time'
			}Subtitle;
			int16 UNKNOWN(0); // 0x20
			bool UNKNOWN(1); // 0x22
			PAD8;
		};
		s_cinematic_globals* _CinematicGlobals();
	};

	namespace Camera
	{
		class c_yelo_camera
		{
		public:
			// hooks the game code to get the camera code to do our debug bidding
			static void DebugInitialize();
			static void DebugDispose();
		private:
			// is there a debug camera in use? holds the count of how many c_yelo_cameras are debug cameras
			static int32 DebugInUse;
			// Table to use to figure out which cameras are the ones in
			// debug mode for easy lookups
			static bool DebugInUseList[Enums::k_number_of_users];
			// Hack hook function for debug camera
			static void UpdateAngleHoriz();
			// Hack hook function for debug camera
			static void UpdateAngleVert();
			// Hack hook function for cameras
			static void UpdateObserverPositions();

			// Local machine's player this camera is attached to
			int16 UserIndex;
			// Are we tracking this camera's movements?
			bool TrackMovement;
			PAD8;
			// Game valid mode state
			_enum DirectorMode;
			// Yelo only mode state
			_enum DirectorModeInternal;

			real_vector3d MoveForce; // external force acting on the camera
			real_vector3d MoveVelocity;
			real_vector3d LookVelocity;

			GameState::s_observer::s_calculated_origin* ObserverOrigin;
			real_point2d* ControlLookAngle;// should point to the player's control global's look angle
		public:

			// Attach this camera to a player
			void Initialize(int16 user_index, _enum mode);
			// Detach and dispose of this camera
			void Dispose();
			// Update this camera for game state changes
			void Update();

		private:
			// Checks to see if the camera has become out of sync with the
			// game state and forces our shit to take charge once again
			// This can happen if the player died or re-spawned
			void UpdateOnOutOfSync();
			// Updates the game state pointers in this object
			void UpdateReferences();
			// Updates the camera look vector based on the current horizontal and vertical look angles of the player
			void UpdateLookVector();
			// Updates the look direction of this camera
			void UpdateLookDirection();
			// Updates the movement of this camera
			void UpdateMovement();

			// Figures out the enumeration for to return based on the current
			// player's perspective (useful when reconnecting to the player and we
			// don't know explicitly what state they are in or were left in)
			_enum GetPerspectiveFromGame() const;
			// Handles setting the director mode
			// Also handles debug camera code enabling and disabling
			// Note: use [_director_mode] to set to current perspective
			// the GAME is using
			void ChangePerspectiveInternal(_enum mode);

		public:
			// Changes the director mode the camera is using
			void ChangePerspective(_enum mode);
			// Get the resistance fraction based on a mode and a velocity
			real GetResistance(_enum resistance, real velocity) const;

			// Invalidates the camera so it can't be used without being
			// initialized
			void Invalidate();

			// Turn the tracking of camera movement on or off
			void ToggleTrackMovement();


		private:
			static c_yelo_camera Cameras[Enums::k_number_of_users];
		public:
			// Determines if the controller index is valid for camera operations
			// remarks: SHOULD return false if the controller was unplugged
			// during game play
			static bool IsValid(int32 controller_index = 0);
			// Determines if the camera attached to this controller
			// index is ready to be used
			static bool IsReady(int32 controller_index = 0);
			// Gets the yelo camera associated to the controller
			static c_yelo_camera& Get(int32 controller_index = 0);
		};
	};
};