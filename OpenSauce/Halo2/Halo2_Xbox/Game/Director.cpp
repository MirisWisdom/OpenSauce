/*
	Yelo: Open Sauce SDK
		Halo 2 (Xbox) Edition

	See license\OpenSauce\Halo2_Xbox for specific license information
*/
#include "Common/Precompile.hpp"
#include "Game/Director.hpp"

#include <blamlib/Halo2/interface/user_interface_controller.hpp>

#include "Memory/Runtime.hpp"
#include "Game/EngineFunctions.hpp"
#include "Memory/Config.hpp"
#include "Interface/Input.hpp"
#include "Game/Players.hpp"

static Yelo::Memory::Opcode::s_call_ret	UNWIND_PLAYER_CONTROL_UPDATE;
static Yelo::Memory::Opcode::s_call		UNWIND_OBSERVER_UPDATE_VELOCITIES_NULLING =	{0x90, 0x90909090};
static Yelo::Memory::Opcode::s_call		UNWIND_OBSERVER_UPDATE_POSITIONS_NULLING1 =	{0x90, 0x90909090};
static Yelo::Memory::Opcode::s_call		UNWIND_OBSERVER_UPDATE_POSITIONS_NULLING2 =	{0x90, 0x90909090};
static Yelo::byte						UNWIND_DIRECTOR_SET_CAMERA_NULLING[] =		{0x90, 0x90, 0x90};
static Yelo::Memory::Opcode::s_call		UNWIND_PLAYER_CONTROL_MODIFY_DESIRED_ANGLES_HORIZ;
static Yelo::Memory::Opcode::s_call		UNWIND_PLAYER_CONTROL_MODIFY_DESIRED_ANGLES_VERT;
static Yelo::Memory::Opcode::s_call		UNWIND_OBSERVER_UPDATE_POSITIONS_START;

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
			for(int16 user_index = 0; false && user_index < Enums::k_number_of_users; user_index++)
			{
				datum_index player_index = Engine::Players::LocalGetPlayerIndex(user_index);
				if(player_index.IsNull()) continue;

				GameState::s_player_datum* player = (*GameState::_Players())[player_index];
				int32 controller_index = *player->GetMachineControllerIndex();
				if(controller_index == NONE) continue;

				c_yelo_camera& yc = c_yelo_camera::Get(controller_index);
				const bool is_ready = c_yelo_camera::IsReady(controller_index);

				if(c_yelo_camera::IsValid(controller_index))
				{
					if(!is_ready)
						yc.Initialize(user_index, 
						Enums::_director_mode); // this causes it to use the existing mode used by the game

					yc.Update();
				}
				else
				{
					if(is_ready) // dispose of cameras that exist for old players
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
		Camera::s_director_globals* _DirectorGlobals()		DPTR_IMP_GET(_DirectorGlobals);
		s_observer* _Observers()							DPTR_IMP_GET(_Observers);
		s_camera_globals* _CameraGlobals()					DPTR_IMP_GET(_CameraGlobals);
		s_scripted_camera_globals* _ScriptedCameraGlobals()	DPTR_IMP_GET(_ScriptedCameraGlobals);
		s_observer_globals* _ObserverGlobals()				DPTR_IMP_GET(_ObserverGlobals);
		s_cinematic_globals* _CinematicGlobals()			DPTR_IMP_GET(_CinematicGlobals);
	};
	#pragma endregion

	namespace Camera
	{
		int32 c_yelo_camera::DebugInUse = FALSE;
		bool c_yelo_camera::DebugInUseList[Enums::k_number_of_users] = {
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
				push	ecx

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
				pop		ecx
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
				jz		dont_use_debug_cam			// we don't have a debug camera in use

				push	edi
				call	CheckPlayerControlPtr		// check to see if EDI is a debug camera player
				test	al, al
				jnz		use_debug_cam				// we found the exact debug camera in use

dont_use_debug_cam:
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
				jz		dont_use_debug_cam			// we don't have a debug camera in use

				push	edi
				call	CheckPlayerControlPtr		// check to see if EDI is a debug camera player
				test	al, al
				jnz		use_debug_cam				// we found the exact debug camera in use

dont_use_debug_cam:
				movss	dword ptr [edi+0x14], xmm0 // player control globals look vert angle
				mov		eax, GET_FUNC_PTR(PLAYER_CONTROL_MODIFY_DESIRED_ANGLES_VERT_END2)
				jmp		the_fucking_end

use_debug_cam:
				mov		eax, GET_FUNC_PTR(PLAYER_CONTROL_MODIFY_DESIRED_ANGLES_VERT_END)
the_fucking_end:
				popfd
				jmp		eax
			}
		}

		API_FUNC_NAKED static c_yelo_camera* CheckObserverPtr(GameState::s_observer* obptr)
		{
			enum {
				_sizeof_of_observer = sizeof(GameState::s_observer)
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

				xor		eax, eax
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

			static size_t g_saved_eax;
			__asm {
				// this is code that we wrote-over in our hook, so re-create it
				//{
				mov		ebx, [eax+ebx]
				inc		ecx
				//}

				push	ebp
				xor		ebp, ebp		// NULL
				mov		g_saved_eax,eax // _continue needs the original eax value (an offset)

				push	edi
				call	CheckObserverPtr
				cmp		eax, ebp		// Did it return NULL?
				jz		_continue
				mov		ebp, eax

				movsx	eax, [ebp]c_yelo_camera.DirectorModeInternal // get the camera's perspective
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
				mov		eax, g_saved_eax
				mov		[eax+edx], ebx		// command that writes to the camera array
				mov		eax, GET_FUNC_PTR(OBSERVER_UPDATE_POSITIONS_END2)

_cleanup:
				pop		ebp
				jmp		eax
			}
		}

		void c_yelo_camera::Initialize(int16 user_index, _enum mode)
		{
			this->UserIndex = user_index;
			this->TrackMovement = false;
			this->DirectorModeInternal = NONE;
			//this->DirectorMode =
			//	this->DirectorModeInternal = Enums::_director_mode;
			ChangePerspectiveInternal(mode);

			this->ObserverOrigin = NULL;
			this->ControlLookAngle = NULL;

			this->UpdateReferences();
		}

		void c_yelo_camera::Dispose()
		{
			ChangePerspectiveInternal(Enums::_director_mode_none); // little 'hack' to turn off debug camera if in use
			this->UserIndex = NONE;
		}

		void c_yelo_camera::Update()
		{
			this->UpdateOnOutOfSync();	// just in case the player has re-spawned in the last tick
			if(c_yelo_camera::DebugInUse)
			{
				this->UpdateLookDirection();// update any needed look changes to this camera
				this->UpdateLookVector();
				this->UpdateMovement();		// update any needed movement changes to this camera
			}
		}

		void c_yelo_camera::UpdateOnOutOfSync()
		{
			// get the player's current director proc
			camera_update_proc curr_proc = 
				GameState::_DirectorGlobals()->Directors[this->UserIndex].UpdateProc;
			// get the current internal mode we're using
			_enum mode = this->DirectorModeInternal;

			if(curr_proc != DirectorModeProc(mode))
				this->ChangePerspective(mode); // they're out of sync, so change back
		}

		void c_yelo_camera::UpdateReferences()
		{
			// Update the origin with the current player's observer state
			this->ObserverOrigin = 
				&GameState::_Observers()[this->UserIndex].origin;

			// Update the look angle with the current player's control state
			const Config::s_definition& k_config = Config::Current();
			GameState::s_player_control_globals::s_player& controls = GameState::_PlayerControlGlobals()->Players[this->UserIndex];
			this->ControlLookAngle = k_config.View.InputCamera.LookControlThumb == Flags::_input_button_left_thumb ?
				controls.GetLeftThumb() : controls.GetRightThumb();
		}

		void c_yelo_camera::UpdateLookVector() // ASM CONVERSION: "UpdateLookVector":
		{
			// get look direction. reminder, these are all radians
			const real h_look_angle = this->ControlLookAngle->x;
			const real v_look_angle = this->ControlLookAngle->y;

			// calculate ijk forward look vectors
			this->ObserverOrigin->forward.i = XboxLib::Math::cosf(v_look_angle) * XboxLib::Math::cosf(h_look_angle);
			this->ObserverOrigin->forward.j = XboxLib::Math::cosf(v_look_angle) * XboxLib::Math::sinf(h_look_angle);
			this->ObserverOrigin->forward.k = XboxLib::Math::sinf(v_look_angle);

			// calculate ijk up vectors; keeps camera upright
			this->ObserverOrigin->up.Set(0.0f, 0.0f, 1.0f);
		}
		void c_yelo_camera::UpdateLookDirection() // ASM CONVERSION: "UpdateCamera", first portion
		{
			const real kAdjustment = 0.01f;

			const Config::s_definition& k_config = Config::Current();

			// get the user input
			const Input::s_yelopad& k_yelopad = Input::CurrentStates(CAST(int32, this->UserIndex));


			// updates horizontal look velocity and position
			//this->LookVelocity.i += 
			//	(k_yelopad.ThumbRight.x * kAdjustment) - (k_config.View.Look.Resistance * this->LookVelocity.i);
			real accel = k_yelopad.ThumbRight.x * kAdjustment * k_config.View.Look.Speed + this->LookVelocity.i;
			real decel = k_config.View.Look.Resistance * accel;
			this->LookVelocity.i = accel - decel;
			this->ControlLookAngle->x += this->LookVelocity.i;

			// updates vertical look velocity and position
			{
				//this->LookVelocity.j += 
				//	(k_yelopad.ThumbRight.y * kAdjustment) - (k_config.View.Look.Resistance * this->LookVelocity.j);
				accel = k_yelopad.ThumbRight.y * kAdjustment * k_config.View.Look.Speed + this->LookVelocity.j;
				decel = k_config.View.Look.Resistance * accel;
				this->LookVelocity.j = accel - decel;
				this->ControlLookAngle->y += this->LookVelocity.j;

				// keeps vertical look in bounds; clamp vertical look angle within the range of [-1.57,1.57] radians
					 if(this->ControlLookAngle->x >  XboxLib::Math::RealConstants.Deg90) this->ControlLookAngle->x =  XboxLib::Math::RealConstants.Deg90;
				else if(this->ControlLookAngle->y < -XboxLib::Math::RealConstants.Deg90) this->ControlLookAngle->y = -XboxLib::Math::RealConstants.Deg90;
			}
		}
		void c_yelo_camera::UpdateMovement() // ASM CONVERSION: "UpdateCamera", second portion
		{
			const real kAdjustment = 0.01f;

			GameState::_PlayerControlGlobals()->CameraControl(false);
			const Config::s_definition& k_config = Config::Current();

			// get the user input
			const Input::s_yelopad& k_yelopad = Input::CurrentStates(CAST(int32, this->UserIndex));


			// calculate forward/backward movement force
			this->MoveForce = this->ObserverOrigin->forward * k_yelopad.ThumbLeft.y;

			// calculates sideways movement force
			this->MoveForce.i += XboxLib::Math::cosf( this->ControlLookAngle->x + XboxLib::Math::RealConstants.Deg90 ) * 
				k_yelopad.ThumbLeft.x;
			this->MoveForce.j += XboxLib::Math::sinf( this->ControlLookAngle->x + XboxLib::Math::RealConstants.Deg90 ) * 
				k_yelopad.ThumbLeft.x;

			// calculates vertical movement force
			this->MoveForce.k += k_yelopad.RightTrigger - k_yelopad.LeftTrigger;

			// updates movement velocity
			ptrdiff_t x = 0;
			for(real* vel = &this->MoveVelocity.i, * force = &this->MoveForce.i, * pos = &this->ObserverOrigin->position.x;
				x < (sizeof(real_vector3d) / sizeof(real));
				x++)
			{
				//this->MoveVelocity += this->MoveForce * kAdjustment * k_config.View.Move.Speed; // after user input
				//this->MoveVelocity -= this->MoveVelocity * k_config.View.Move.Resistance; // after resistance

				// updates movement position
				//this->ObserverOrigin->position += *CAST_PTR(real_point3d*, &this->MoveVelocity);

				real accel = force[x] * kAdjustment * k_config.View.Move.Speed + vel[x];
				real decel = k_config.View.Move.Resistance * accel;

				pos[x] += accel - decel;
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
				DebugInUseList[this->UserIndex] = false;
			}
			
			// change to third person camera
			if(mode == Enums::_director_mode_third_person)
				this->DirectorMode = mode = Enums::_director_mode_following;
			// change to a debug camera
			else if(mode == Enums::_director_mode_debug)
			{
				DebugInUse++; // increase the debug camera count
				DebugInUseList[this->UserIndex] = true;

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

		_enum c_yelo_camera::GetPerspectiveFromGame() const
		{
			uintptr_t ptr = CAST_PTR(uintptr_t, GameState::_DirectorGlobals()->Directors[this->UserIndex].UpdateProc);
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
			int32 user_index = CAST(int32, this->UserIndex);

			this->ChangePerspectiveInternal(mode);

			// for third person camera cases
			if(mode == Enums::_director_mode_third_person)
			{
				this->ObserverOrigin->offset.y = 0.0f;
				this->ObserverOrigin->offset.z = 0.1f;
				this->ObserverOrigin->look_offset.y = 0.0f;
				this->ObserverOrigin->depth = 1.0f;
				this->ObserverOrigin->fov = 1.92f;
			}
			// for debug camera cases
			else if(mode == Enums::_director_mode_debug)
			{
				memset(&this->MoveVelocity, 0, sizeof(this->MoveVelocity));
				memset(&this->LookVelocity, 0, sizeof(this->LookVelocity));
			}

			GameState::_DirectorGlobals()->Directors[user_index].UpdateProc = // update the player's camera update proc
				DirectorModeProc(this->DirectorMode);
		}

		real c_yelo_camera::GetResistance(_enum resistance, real velocity) const
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
			this->UserIndex = NONE;
		}

		void c_yelo_camera::ToggleTrackMovement()
		{
			this->TrackMovement ^= true;
		}

		c_yelo_camera c_yelo_camera::Cameras[Enums::k_number_of_users];

		bool c_yelo_camera::IsValid(int32 controller_index)
		{
			GameState::s_player_datum* player;

#if 0
			GameState::t_players_data::Iterator iter(GameState::_Players());
			while( (player = iter.Next()) != NULL )
#else
			if(controller_index == Enums::k_no_controller) return false;

			datum_index player_index = GameState::_PlayerGlobals()->GetPlayerControllerMapping()[controller_index];
			if(!player_index.IsNull() && (player = (*GameState::_Players())[player_index]) != NULL)
#endif
			{
				if( *player->GetMachineControllerIndex() == controller_index && // only look for the controller we want
					*player->GetSlaveUnitIndex() != datum_index::null)
					return true;
			}
			return false; // nothing was valid
		}

		bool c_yelo_camera::IsReady(int32 controller_index)
		{
			return Cameras[controller_index].UserIndex != NONE;
		}

		c_yelo_camera& c_yelo_camera::Get(int32 controller_index)
		{
			return Cameras[controller_index];
		}
	};
};