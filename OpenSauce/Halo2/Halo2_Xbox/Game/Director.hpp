/*
	Yelo: Open Sauce SDK
		Halo 2 (Xbox) Edition

	See license\OpenSauce\Halo2_Xbox for specific license information
*/
#pragma once

namespace Yelo
{
	namespace Enums
	{
		enum {
			k_camera_count = 4,
		};

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


	namespace Camera
	{
		struct camera_data
		{
			UNKNOWN_TYPE(real);					// 0x0
			real_point3d Position;				// 0x4
			real_vector3d Velocity;				// 0x10
			UNKNOWN_TYPE(int32);				// 0x1C
			real Fov;							// 0x20
			UNKNOWN_TYPE(real);					// 0x24
			UNKNOWN_TYPE(real);					// 0x28
			real_vector3d Forward;				// 0x2C
			real_euler_angles3d Up;				// 0x38
			real_vector3d TransitionalVelocity;	// 0x44
			real_vector3d AngularVelocity;		// 0x50

			// float @ 0x88
			// float @ 0xA4
			TAG_PAD(int32, 20);					// 0x5C
		}; BOOST_STATIC_ASSERT( sizeof(camera_data) == 0xAC );

		typedef void (API_FUNC* camera_update_proc)(camera_data* camera, datum_index* player_index, camera_data* result);

		struct s_director_camera
		{
			UNKNOWN_TYPE(int32);
			UNKNOWN_TYPE(int32);
			camera_update_proc UpdateProc;
			TAG_PAD(int32, 20); // 0x5C
			camera_data UpdateResult;
			TAG_PAD(int32, 14); // 0x104
		}; BOOST_STATIC_ASSERT( sizeof(s_director_camera) == 0x140 );


		void Initialize();
		void Dispose();
		void Update();

		camera_update_proc DirectorModeProc(_enum mode);
	};

	namespace GameState
	{
		Camera::s_director_camera* _Directors();


		struct s_game_observer : TStructImpl(856)
		{
			struct s_destination
			{
				real_point3d Position; // 0x0
				real_point3d Offset; // 0xC
				real Depth; // 0x18
				real Fov; // 0x1C
				real_vector3d Forward; // 0x20
				real_euler_angles3d Up; // 0x2C
				//real_vector3d Vector; // not sure if this in halo2...
			};

			struct s_calculated_result
			{
				real_point3d Position; // 0x0
				s_scenario_location Location; // 0xC
				real_vector3d Velocity; // 0x14
				real_vector3d Forward; // 0x20
				real_euler_angles3d Up; // 0x2C
				real Fov; // 0x38
				UNKNOWN_TYPE(int32); // 0x3C, i'm not sure what this field's type is...
				UNKNOWN_TYPE(real); // 0x40
				UNKNOWN_TYPE(real); // 0x44
			}; BOOST_STATIC_ASSERT( sizeof(s_calculated_result) == 0x48 );

			struct s_calculated_origin
			{
				real_point3d Origin; // 0x0
				real_point3d Displacement; // 0xC
				real_point2d LookDisplacement; // 0x18
				real Depth; // 0x20
				real Fov; // 0x24
				real_vector3d Forward; // 0x28 Direction
				real_vector3d Up; // 0x34 Rotation
			}; BOOST_STATIC_ASSERT( sizeof(s_calculated_origin) == 0x40 );

			// 0xAC byte sub-structure at 0x8
			// bool @ 0xB4, 0xB5, 0xB6 (0xB7 must be a PAD8 field)

			//TStructGetPtrImpl(, , 0x);
			TStructGetPtrImpl(s_destination, Destination, 0x2C);

			TStructGetPtrImpl(s_calculated_result, Result, 0xB8);
			// 12 bytes in between
			TStructGetPtrImpl(s_calculated_origin, Origin, 0x10C);

			// maxtrix4x3 @ 0x14C
		};
		s_game_observer* _Observers();

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
			static bool DebugInUseList[Enums::k_camera_count];
			// Hack hook function for debug camera
			static void UpdateAngleHoriz();
			// Hack hook function for debug camera
			static void UpdateAngleVert();
			// Hack hook function for cameras
			static void UpdateObserverPositions();

			// Local machine's player this camera is attached to
			int16 LocalPlayerIndex;
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

			GameState::s_game_observer::s_calculated_origin* ObserverOrigin;
			real_point2d* ControlLookAngle;// should point to the player's control global's look angle
		public:

			// Attach this camera to a player
			void Initialize(int16 local_player_index, _enum mode);
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
			// If we have to, update the movement of this camera
			void UpdateMovement();
			// If we have to, update the look direction of this camera
			void UpdateLookDirection();

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
			static c_yelo_camera Cameras[Enums::k_camera_count];
		public:
			// Determines if the controller index is usable
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