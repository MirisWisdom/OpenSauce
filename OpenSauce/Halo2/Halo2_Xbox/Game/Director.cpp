/*
	Yelo: Open Sauce SDK
		Halo 2 (Xbox) Edition

	See license\OpenSauce\Halo2_Xbox for specific license information
*/
#include "Common/Precompile.hpp"
#include "Game/Director.hpp"

#include "Memory/Runtime.hpp"
#include "Game/EngineFunctions.hpp"
#include "Memory/Config.hpp"
#include "Interface/Input.hpp"
#include "Game/Players.hpp"

static Yelo::Memory::Opcode::CallRet	UNWIND_PLAYER_CONTROL_UPDATE;
static Yelo::Memory::Opcode::Call		UNWIND_OBSERVER_UPDATE_VELOCITIES_NULLING =	{0x90, 0x90909090};
static Yelo::Memory::Opcode::Call		UNWIND_OBSERVER_UPDATE_POSITIONS_NULLING1 =	{0x90, 0x90909090};
static Yelo::Memory::Opcode::Call		UNWIND_OBSERVER_UPDATE_POSITIONS_NULLING2 =	{0x90, 0x90909090};
static Yelo::byte						UNWIND_DIRECTOR_SET_CAMERA_NULLING[] =		{0x90, 0x90, 0x90};
static Yelo::Memory::Opcode::Call		UNWIND_PLAYER_CONTROL_MODIFY_DESIRED_ANGLES_HORIZ;
static Yelo::Memory::Opcode::Call		UNWIND_PLAYER_CONTROL_MODIFY_DESIRED_ANGLES_VERT;
static Yelo::Memory::Opcode::Call		UNWIND_OBSERVER_UPDATE_POSITIONS_START;

namespace Yelo
{
#define __EL_INCLUDE_ID			__EL_INCLUDE_GAME
#define __EL_INCLUDE_FILE_ID	__EL_GAME_DIRECTOR
#include "Memory/_EngineLayout.inl"

	namespace Camera
	{
		void Initialize()
		{
			c_yelo_camera::Get(0).Invalidate();
			c_yelo_camera::Get(1).Invalidate();
			c_yelo_camera::Get(2).Invalidate();
			c_yelo_camera::Get(3).Invalidate();

			YELO_MEM_WLIST_BEGIN();

			YELO_MEM_WRITE(WriteCallRet, 
				&UNWIND_PLAYER_CONTROL_UPDATE, 
				GET_FUNC_VPTR(PLAYER_CONTROL_UPDATE_HOOK), 
				&Camera::Update, 
				2);
			c_yelo_camera::DebugInitialize();

			YELO_MEM_WLIST_END();
		}

		void Dispose()
		{
			YELO_MEM_WLIST_BEGIN();

			YELO_MEM_WRITE(OverwriteMemorySansCopy, 
				GET_FUNC_VPTR(PLAYER_CONTROL_UPDATE_HOOK),
				UNWIND_PLAYER_CONTROL_UPDATE);
			c_yelo_camera::DebugDispose();

			YELO_MEM_WLIST_END();
		}

		void Update()
		{
			for(int16 x = 0; x < Enums::k_camera_count; x++)
			{
				c_yelo_camera& yc = c_yelo_camera::Get(x);

				if(c_yelo_camera::IsValid(x))
				{
					if(!c_yelo_camera::IsReady(x))
						yc.Initialize(x, 
						Enums::_director_mode); // this causes it to use the existing mode used by the game

					yc.Update();
				}
				else
				{
					if(c_yelo_camera::IsReady(x)) // dispose of cameras that exist for old players
						yc.Dispose();
				}
			}
		}

		camera_update_proc DirectorModeProc(_enum mode)
		{
			switch(mode)
			{
			case Enums::_director_mode_scripted:
				return CAST_PTR(camera_update_proc, GET_DATA_VPTR(SCRIPTED_CAMERA_UPDATE));

			case Enums::_director_mode_first_person:
				return CAST_PTR(camera_update_proc, GET_DATA_VPTR(FIRST_PERSON_CAMERA_UPDATE));

			case Enums::_director_mode_following:
				return CAST_PTR(camera_update_proc, GET_DATA_VPTR(FOLLOWING_CAMERA_UPDATE));

			case Enums::_director_mode_editor:
				return CAST_PTR(camera_update_proc, GET_DATA_VPTR(EDITOR_CAMERA_UPDATE));

			case Enums::_director_mode_dead:
				return CAST_PTR(camera_update_proc, GET_DATA_VPTR(DEAD_CAMERA_UPDATE));

			default:
				return NULL;
			}
		}
	};

	#pragma region GameState
	namespace GameState
	{
		Camera::s_director_camera* _Directors()				DPTR_IMP_GET(_Directors);
		s_game_observer* _Observers()						DPTR_IMP_GET(_Observers);
		s_camera_globals* _CameraGlobals()					DPTR_IMP_GET(_CameraGlobals);
		s_scripted_camera_globals* _ScriptedCameraGlobals()	DPTR_IMP_GET(_ScriptedCameraGlobals);
		s_observer_globals* _ObserverGlobals()				DPTR_IMP_GET(_ObserverGlobals);
		s_cinematic_globals* _CinematicGlobals()			DPTR_IMP_GET(_CinematicGlobals);
	};
	#pragma endregion

	namespace Camera
	{
		int32 c_yelo_camera::DebugInUse = false;
		bool c_yelo_camera::DebugInUseList[Enums::k_camera_count] = {
			false,
			false,
			false,
			false,
		};

		void c_yelo_camera::DebugInitialize()
		{
			YELO_MEM_WLIST_ITEM(OverwriteMemory, 
				GET_FUNC_VPTR(OBSERVER_UPDATE_VELOCITIES_NULLING),
				UNWIND_OBSERVER_UPDATE_VELOCITIES_NULLING);
			YELO_MEM_WLIST_ITEM(OverwriteMemory, 
				GET_FUNC_VPTR(OBSERVER_UPDATE_POSITIONS_NULLING1),
				UNWIND_OBSERVER_UPDATE_POSITIONS_NULLING1);
			YELO_MEM_WLIST_ITEM(OverwriteMemory, 
				GET_FUNC_VPTR(OBSERVER_UPDATE_POSITIONS_NULLING2),
				UNWIND_OBSERVER_UPDATE_POSITIONS_NULLING2);
			YELO_MEM_WLIST_ITEM(OverwriteMemory, 
				GET_FUNC_VPTR(DIRECTOR_SET_CAMERA_NULLING),
				UNWIND_DIRECTOR_SET_CAMERA_NULLING);

			YELO_MEM_WLIST_ITEM(WriteJmp, 
				&UNWIND_PLAYER_CONTROL_MODIFY_DESIRED_ANGLES_HORIZ,
				GET_FUNC_VPTR(PLAYER_CONTROL_MODIFY_DESIRED_ANGLES_HORIZ_START),
				&Yelo::Camera::c_yelo_camera::UpdateAngleHoriz);
			YELO_MEM_WLIST_ITEM(WriteJmp, 
				&UNWIND_PLAYER_CONTROL_MODIFY_DESIRED_ANGLES_VERT,
				GET_FUNC_VPTR(PLAYER_CONTROL_MODIFY_DESIRED_ANGLES_VERT_START),
				&Yelo::Camera::c_yelo_camera::UpdateAngleVert);
			YELO_MEM_WLIST_ITEM(WriteJmp, 
				&UNWIND_OBSERVER_UPDATE_POSITIONS_START,
				GET_FUNC_VPTR(OBSERVER_UPDATE_POSITIONS_START),
				&Yelo::Camera::c_yelo_camera::UpdateObserverPositions);
		}

		void c_yelo_camera::DebugDispose()
		{
			YELO_MEM_WLIST_ITEM(OverwriteMemorySansCopy, 
				GET_FUNC_VPTR(OBSERVER_UPDATE_VELOCITIES_NULLING),
				UNWIND_OBSERVER_UPDATE_VELOCITIES_NULLING);
			YELO_MEM_WLIST_ITEM(OverwriteMemorySansCopy, 
				GET_FUNC_VPTR(OBSERVER_UPDATE_POSITIONS_NULLING1),
				UNWIND_OBSERVER_UPDATE_POSITIONS_NULLING1);
			YELO_MEM_WLIST_ITEM(OverwriteMemorySansCopy, 
				GET_FUNC_VPTR(OBSERVER_UPDATE_POSITIONS_NULLING2),
				UNWIND_OBSERVER_UPDATE_POSITIONS_NULLING2);
			YELO_MEM_WLIST_ITEM(OverwriteMemorySansCopy, 
				GET_FUNC_VPTR(DIRECTOR_SET_CAMERA_NULLING),
				UNWIND_DIRECTOR_SET_CAMERA_NULLING);

			YELO_MEM_WLIST_ITEM(OverwriteMemorySansCopy, 
				GET_FUNC_VPTR(PLAYER_CONTROL_MODIFY_DESIRED_ANGLES_HORIZ_START),
				UNWIND_PLAYER_CONTROL_MODIFY_DESIRED_ANGLES_HORIZ);
			YELO_MEM_WLIST_ITEM(OverwriteMemorySansCopy, 
				GET_FUNC_VPTR(PLAYER_CONTROL_MODIFY_DESIRED_ANGLES_VERT_START),
				UNWIND_PLAYER_CONTROL_MODIFY_DESIRED_ANGLES_VERT);
			YELO_MEM_WLIST_ITEM(OverwriteMemorySansCopy, 
				GET_FUNC_VPTR(OBSERVER_UPDATE_POSITIONS_START),
				UNWIND_OBSERVER_UPDATE_POSITIONS_START);
		}

		// Finds out if the player associated to the pointer to the
		// player control globals player is in debug mode right now
		API_FUNC_NAKED static bool CheckPlayerControlPtr(GameState::s_player_control_globals::s_player* pcptr)
		{
			using namespace Yelo::GameState;
			enum {
				_sizeof_of_pcg_player = sizeof(s_player_control_globals::s_player)
			};

			API_FUNC_NAKED_START()
				push	edi
				push	esi

				call	GameState::_PlayerControlGlobals
				mov		edi, eax
				xor		eax, eax		// clear our return value
				xor		ecx, ecx		// clear our indexer
				mov		esi, pcptr		// get the other player control's pointer
										// get the first player control's pointer
				lea		edi, [edi]s_player_control_globals.Players
				cmp		edi, esi		// test player 1
				jz		found_player
				add		edi, _sizeof_of_pcg_player

				cmp		edi, esi		// test player 2
				inc		ecx				// increase our indexer
				jz		found_player
				add		edi, _sizeof_of_pcg_player

				cmp		edi, esi		// test player 3
				inc		ecx				// increase our indexer
				jz		found_player
				add		edi, _sizeof_of_pcg_player

				cmp		edi, esi		// test player 4
				inc		ecx				// increase our indexer
				jz		found_player
				jmp		get_the_fuck_out	// we didn't find a matching player?

found_player:
				mov		al, c_yelo_camera::DebugInUseList[ecx]

get_the_fuck_out:
				pop		esi
				pop		edi
			API_FUNC_NAKED_END(1);
		}

		API_FUNC_NAKED void c_yelo_camera::UpdateAngleHoriz() // ASM CONVERSION: "hangle"
		{
			__asm {
				xorps	xmm1, xmm1					// this is code that we wrote-over in our hook, so re-create it
				pushfd
				cmp		c_yelo_camera::DebugInUse, 0
				jnz		use_debug_cam				// we have a debug camera in use

				push	edi
				call	CheckPlayerControlPtr		// check to see if EDI is a debug camera player
				test	al, al
				jnz		use_debug_cam				// we found the exact debug camera in use

				movss	dword ptr [edi+0x10], xmm0	// player control globals look horiz angle

use_debug_cam:
				popfd
				push	GET_FUNC_PTR(PLAYER_CONTROL_MODIFY_DESIRED_ANGLES_HORIZ_END)
				ret
			};
		}

		API_FUNC_NAKED void c_yelo_camera::UpdateAngleVert() // ASM CONVERSION: "vangle"
		{
			__asm {
				comiss	xmm1, xmm0					// this is code that we wrote-over in our hook, so re-create it
				pushfd
				cmp		c_yelo_camera::DebugInUse, 0
				jnz		use_debug_cam				// we have a debug camera in use

				push	edi
				call	CheckPlayerControlPtr		// check to see if EDI is a debug camera player
				test	al, al
				jnz		use_debug_cam				// we found the exact debug camera in use

				movss	dword ptr [edi+0x14], xmm0 // player control globals look vert angle
				mov		eax, GET_FUNC_PTR(PLAYER_CONTROL_MODIFY_DESIRED_ANGLES_VERT_END2)
				jmp		the_fucking_end

use_debug_cam:
				mov		eax, GET_FUNC_PTR(PLAYER_CONTROL_MODIFY_DESIRED_ANGLES_VERT_END)
the_fucking_end:
				popfd
				push	eax
				ret
			}
		}

		API_FUNC_NAKED static c_yelo_camera* CheckObserverPtr(GameState::s_game_observer* obptr)
		{
			enum {
				_sizeof_of_observer = sizeof(GameState::s_game_observer)
			};

			API_FUNC_NAKED_START()
				push	ecx
				push	edi
				push	esi

				call	GameState::_Observers
				mov		edi, eax
				xor		eax, eax		// clear our return value
				xor		ecx, ecx		// clear our indexer
				mov		esi, obptr		// get the other player observer's pointer
				cmp		edi, esi		// test player 1
				jz		found_player
				add		edi, _sizeof_of_observer

				cmp		edi, esi		// test player 2
				inc		ecx				// increase our indexer
				jz		found_player
				add		edi, _sizeof_of_observer

				cmp		edi, esi		// test player 3
				inc		ecx				// increase our indexer
				jz		found_player
				add		edi, _sizeof_of_observer

				cmp		edi, esi		// test player 4
				inc		ecx				// increase our indexer
				jz		found_player
				jmp		get_the_fuck_out	// we didn't find a matching player?

found_player:
				push	ecx
				call	c_yelo_camera::Get

get_the_fuck_out:
				pop		esi
				pop		edi
				pop		ecx
			API_FUNC_NAKED_END(1);
		}

		API_FUNC_NAKED void c_yelo_camera::UpdateObserverPositions() // ASM CONVERSION: "CameraHook"
		{
			using namespace Yelo::Enums;

			__asm {
				// this is code that we wrote-over in our hook, so re-create it
				//{
				mov		ebx, [eax+ebx]
				inc		ecx
				//}

				push	edx

				push	edi
				call	CheckObserverPtr
				mov		edx, eax

				movsx	eax, [edx]c_yelo_camera.DirectorModeInternal // get the camera's perspective
				cmp		eax, _director_mode_debug
				je		_skip_camera
				cmp		eax, _director_mode_third_person
				je		_third_enable
				cmp		eax, _director_mode_following
				je		_third_enable
				jmp		_continue

_third_enable:	// determines which values can be edited while in third person
				cmp		eax, _director_mode_third_person
				jne		_skip_camera
				cmp		esi, 2
				jge		_skip_camera
				cmp		esi, 8
				jle		_skip_camera
				jmp		_continue

_skip_camera:	// skips writing to camera array which allows for custom input
				cmp		cx, word ptr GET_DATA_PTR(OBSERVER_PARAMETER_REAL_COUNTS)[esi]
				mov		eax, GET_FUNC_PTR(OBSERVER_UPDATE_POSITIONS_END)
				jmp		_cleanup

_continue:		// continues writing to camera array
				mov		[eax+edx], ebx		// command that writes to the camera array
				mov		eax, GET_FUNC_PTR(OBSERVER_UPDATE_POSITIONS_END2)

_cleanup:
				pop		edx
				ret
			}
		}

		void c_yelo_camera::Initialize(int16 local_player_index, _enum mode)
		{
			this->LocalPlayerIndex = local_player_index;
			this->TrackMovement = false;
			this->DirectorMode =
				this->DirectorModeInternal = Enums::_director_mode;
			ChangePerspectiveInternal(mode);

			this->ObserverOrigin = NULL;
			this->ControlLookAngle = NULL;

			this->UpdateReferences();
		}

		void c_yelo_camera::Dispose()
		{
			ChangePerspectiveInternal(Enums::_director_mode_none); // little 'hack' to turn off debug camera if in use
			this->LocalPlayerIndex = NONE;
		}

		void c_yelo_camera::Update()
		{
			this->UpdateOnOutOfSync();	// just in case the player has re-spawned in the last tick
			this->UpdateMovement();		// update any needed movement changes to this camera
			this->UpdateLookDirection();// update any needed look changes to this camera
		}

		void c_yelo_camera::UpdateOnOutOfSync()
		{
			// get the player's current director proc
			camera_update_proc curr_proc = 
				GameState::_Directors()[this->LocalPlayerIndex].UpdateProc;
			// get the current internal mode we're using
			_enum mode = this->DirectorModeInternal;

			if(curr_proc != DirectorModeProc(mode))
				this->ChangePerspective(mode); // they're out of sync, so change back
		}

		void c_yelo_camera::UpdateReferences()
		{
			// Update the origin with the current player's observer state
			this->ObserverOrigin = 
				GameState::_Observers()[this->LocalPlayerIndex].GetOrigin();
			// Update the look angle with the current player's control state
			// TODO: Get a value to indicate which thumbstick to use for the look control
			this->ControlLookAngle = 
				GameState::_PlayerControlGlobals()->Players[this->LocalPlayerIndex].GetRightThumb();
		}

		void c_yelo_camera::UpdateMovement() // ASM CONVERSION: "UpdateCamera", second portion
		{
			if(c_yelo_camera::DebugInUse)
			{
				const real kAdjustment = 0.01f;

				GameState::_PlayerGlobals()->CameraControl(false);
				Config::s_definition& config = Config::Current();

				// get the user input
				Input::s_yelopad& s_yelopad = Input::CurrentStates(CAST(int32, this->LocalPlayerIndex));


				// calculate forward/backward movement force
				this->MoveForce = this->ObserverOrigin->Forward * s_yelopad.ThumbLeft.y;

				// calculates sideways movement force
				this->MoveForce.i += XboxLib::Math::cosf( (this->ControlLookAngle->x + XboxLib::Math::RealConstants.Deg90) * 
					s_yelopad.ThumbLeft.x );
				this->MoveForce.j += XboxLib::Math::sinf( (this->ControlLookAngle->x + XboxLib::Math::RealConstants.Deg90) * 
					s_yelopad.ThumbLeft.x );

				// calculates vertical movement force
				this->MoveForce.k += s_yelopad.RightTrigger - s_yelopad.LeftTrigger;

				// updates movement velocity
				{
					this->MoveVelocity += this->MoveForce * kAdjustment * config.View.Move.Speed; // after user input
					this->MoveVelocity -= this->MoveVelocity * config.View.Look.Resistance; // after resistance

					// updates movement position
					this->ObserverOrigin->Origin += *CAST_PTR(real_point3d*, &this->MoveVelocity);
				}
			}
		}

		void c_yelo_camera::UpdateLookDirection() // ASM CONVERSION: "UpdateCamera", first portion
		{
			if(c_yelo_camera::DebugInUse)
			{
				const real kAdjustment = 0.01f;

				Config::s_definition& config = Config::Current();

				// get the user input
				Input::s_yelopad& s_yelopad = Input::CurrentStates(CAST(int32, this->LocalPlayerIndex));


				// updates horizontal look velocity and position
				this->LookVelocity.i += 
					(s_yelopad.ThumbRight.x * kAdjustment) - (config.View.Look.Resistance * this->LookVelocity.i);
				this->ControlLookAngle->x += this->LookVelocity.i;

				// updates vertical look velocity and position
				{
					this->LookVelocity.j += 
						(s_yelopad.ThumbRight.y * kAdjustment) - (config.View.Look.Resistance * this->LookVelocity.j);
					this->ControlLookAngle->y += this->LookVelocity.j;

					// keeps vertical look in bounds
					if(this->ControlLookAngle->x > XboxLib::Math::RealConstants.Deg90)
						this->ControlLookAngle->x = XboxLib::Math::RealConstants.Deg90;
					else if(this->ControlLookAngle->y < -XboxLib::Math::RealConstants.Deg90)
						this->ControlLookAngle->y = -XboxLib::Math::RealConstants.Deg90;
				}


				// ASM CONVERSION: "UpdateLookVector":

				// get look direction
				real h_look_angle = this->ControlLookAngle->x;
				real v_look_angle = this->ControlLookAngle->y;

				// calculate ijk forward look vectors
				this->ObserverOrigin->Forward.i = XboxLib::Math::cosf(h_look_angle) * XboxLib::Math::cosf(v_look_angle);
				this->ObserverOrigin->Forward.j = XboxLib::Math::sinf(h_look_angle) * XboxLib::Math::cosf(v_look_angle);
				this->ObserverOrigin->Forward.k = XboxLib::Math::sinf(v_look_angle);

				// calculate ijk up vectors; keeps camera upright
				this->ObserverOrigin->Up.Set(0.0f, 0.0f, 1.0f);
			}
		}

		void c_yelo_camera::ChangePerspectiveInternal(_enum mode)
		{
			// sometimes things mess up and we pass the mode we already in
			if(this->DirectorModeInternal == mode) return;

			// if we're about to change FROM a debug camera...
			if(this->DirectorModeInternal == Enums::_director_mode_debug)
			{
				DebugInUse--; // decrease the debug camera count
				DebugInUseList[this->LocalPlayerIndex] = false;
			}
			
			// change to third person camera
			if(mode == Enums::_director_mode_third_person)
				this->DirectorMode = mode = Enums::_director_mode_following;
			// change to a debug camera
			else if(mode == Enums::_director_mode_debug)
			{
				DebugInUse++; // increase the debug camera count
				DebugInUseList[this->LocalPlayerIndex] = true;

				this->DirectorMode = mode = Enums::_director_mode_none;
			}
			// change to a static camera
			else if(mode == Enums::_director_mode_static)
				this->DirectorMode = mode = Enums::_director_mode_none;
			// else, we want something else
			else
			{
				// Adjust for using pre-existing mode
				this->DirectorMode = 
					mode != Enums::_director_mode ? mode : this->GetPerspectiveFromGame();
			}

			// Adjust for using pre-existing mode
			this->DirectorModeInternal =
				mode != Enums::_director_mode ? mode : this->DirectorMode;
		}

		_enum c_yelo_camera::GetPerspectiveFromGame()
		{
			uint32 ptr = CAST_PTR(uint32, GameState::_Directors()[this->LocalPlayerIndex].UpdateProc);
			switch(ptr)
			{
			case GET_DATA_PTR(SCRIPTED_CAMERA_UPDATE):
				return Enums::_director_mode_scripted;

			case GET_DATA_PTR(FIRST_PERSON_CAMERA_UPDATE):
				return Enums::_director_mode_first_person;

			case GET_DATA_PTR(FOLLOWING_CAMERA_UPDATE):
				return Enums::_director_mode_following;

			case GET_DATA_PTR(EDITOR_CAMERA_UPDATE):
				return Enums::_director_mode_editor;

			case GET_DATA_PTR(DEAD_CAMERA_UPDATE):
				return Enums::_director_mode_dead;

			case NULL:
				return Enums::_director_mode_none;
			}

			// we shouldn't get here...
			return Enums::_director_mode;
		}

		void c_yelo_camera::ChangePerspective(_enum mode)
		{
			int32 local_player_index = CAST(int32, this->LocalPlayerIndex);

			this->ChangePerspectiveInternal(mode);

			// for third person camera cases
			if(mode == Enums::_director_mode_third_person)
			{
				this->ObserverOrigin->Displacement.y = 0.0f;
				this->ObserverOrigin->Displacement.z = 0.1f;
				this->ObserverOrigin->LookDisplacement.y = 0.0f;
				this->ObserverOrigin->Depth = 1.0f;
				this->ObserverOrigin->Fov = 1.92f;
			}
			// for debug camera cases
			else if(mode == Enums::_director_mode_debug)
			{
				memset(&this->MoveVelocity, 0, sizeof(this->MoveVelocity));
				memset(&this->LookVelocity, 0, sizeof(this->LookVelocity));
			}

			GameState::_Directors()[local_player_index].UpdateProc = // update the player's camera update proc
				DirectorModeProc(mode);
		}

		real c_yelo_camera::GetResistance(_enum resistance, real velocity)
		{
			switch(resistance)
			{
			case Enums::_resistance_function_linear:
				break;

			case Enums::_resistance_function_exponential:
				break;

			case Enums::_resistance_function_logarithmic:
				break;
			}

			return 0.0f;
		}

		void c_yelo_camera::Invalidate()
		{
			this->LocalPlayerIndex = NONE;
		}

		void c_yelo_camera::ToggleTrackMovement()
		{
			this->TrackMovement ^= true;
		}

		c_yelo_camera c_yelo_camera::Cameras[Enums::k_camera_count];

		bool c_yelo_camera::IsValid(int32 controller_index)
		{
			// TODO: use data iterator
			GameState::s_player_datum* players = *GameState::_Players(); // get players
			for(uint32 x = 0; x < GameState::_Players()->GetDatumCount(); x++) // loop and check for the valid cases
				if(players->GetHeader() != NONE && // the header must be valid first
					*players[x].GetControllerIndex() == controller_index && // only look for the controller we want
					*players[x].GetSlaveUnitIndex() != datum_index::null) // if the player is not existing as a unit in-game right now, they're considered invalid to us
					return true;
			return false; // nothing was valid
		}

		bool c_yelo_camera::IsReady(int32 controller_index)
		{
			return Cameras[controller_index].LocalPlayerIndex != NONE;
		}

		c_yelo_camera& c_yelo_camera::Get(int32 controller_index)
		{
			return Cameras[controller_index];
		}
	};
};