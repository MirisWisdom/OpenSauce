/*
	Yelo: Open Sauce SDK
		Halo 2 (Xbox) Edition

	See license\OpenSauce\Halo2_Xbox for specific license information
*/
#include "Common/Precompile.hpp"
#include "Interface/Input.hpp"

#include <blamlib/Halo2/interface/user_interface_controller.hpp>

#include "Memory/Runtime.hpp"
#include "Game/EngineFunctions.hpp"
#include "Memory/Config.hpp"

#include "AI/AI.hpp"
#include "Game/Main.hpp"
#include "Game/Globals.hpp"
#include "Game/Director.hpp"

/*
0x51EA18, sizeof 0x1C, [4]
	real HorizLookSpeed : 0
	real VertLookSpeed : 4
	struct { : 8
		byte;
		byte;
		byte;
		byte;
	}[4];
*/

static Yelo::Memory::Opcode::s_call_ret UNWIND_INPUT_UPDATE;

namespace Yelo
{
#define __EL_INCLUDE_ID			__EL_INCLUDE_INTERFACE
#define __EL_INCLUDE_FILE_ID	__EL_INTERFACE_INPUT
#include "Memory/_EngineLayout.inl"

	namespace Input
	{
		void Initialize()
		{
			YELO_MEM_WRITE(WriteCallRet, &UNWIND_INPUT_UPDATE, GET_FUNC_VPTR(INPUT_UPDATE_HOOK), &Input::Update, 1);
		}

		void Dispose()
		{
			YELO_MEM_WRITE(WriteRetn, UNWIND_INPUT_UPDATE, GET_FUNC_VPTR(INPUT_UPDATE_HOOK));
		}

		void Update()
		{
 			HANDLE handle;
 
			for(int32 x = 0; x < Enums::k_number_of_controllers; x++)
 			{
 				if( handle = GamepadHandles(x) )
				{
 					s_yelopad& pad = YeloPadCreate( CurrentStates(x) );
					HandleYeloInput(pad);
				}
 			}
		}

		void HandleYeloInput(const s_yelopad& pad)
		{
			const float kAdjustment = 0.01f;
			real real_tmp;

			bool camera_is_valid = Camera::c_yelo_camera::IsValid(pad.Index); // is this player's camera currently valid?
			Camera::c_yelo_camera& camera = Camera::c_yelo_camera::Get(pad.Index); // get the yelo camera for the player

			Config::s_definition& config = Config::Current();
			if(!config.Initialized) return;

			word_flags new_state = pad.wButtons;
			if(new_state != pad.wOldButtons) // execute once per press
			{
				if(camera_is_valid && new_state == config.View.InputCamera.FpCamera)
				{
					camera.ChangePerspective(Enums::_director_mode_first_person);
					DebugOnly( Engine::Interface::HudMessagePrint(L"First person") );
				}
				else if(camera_is_valid && new_state == config.View.InputCamera.TpCamera)
				{
					camera.ChangePerspective(Enums::_director_mode_third_person);
					DebugOnly( Engine::Interface::HudMessagePrint(L"Third person") );
				}
				else if(camera_is_valid && new_state == config.View.InputCamera.ChaseCamera)
				{
					camera.ChangePerspective(Enums::_director_mode_following);
					DebugOnly( Engine::Interface::HudMessagePrint(L"Chase camera") );
				}
				else if(camera_is_valid && new_state == config.View.InputCamera.ScriptedCamera)
				{
					camera.ChangePerspective(Enums::_director_mode_scripted);
					DebugOnly( Engine::Interface::HudMessagePrint(L"Scripted camera") );
				}
				else if(camera_is_valid && new_state == config.View.InputCamera.EditorCamera)
				{
					camera.ChangePerspective(Enums::_director_mode_editor);
					DebugOnly( Engine::Interface::HudMessagePrint(L"Editor camera") );
				}
				else if(camera_is_valid && new_state == config.View.InputCamera.AnchoredCamera)
				{
					camera.ChangePerspective(Enums::_director_mode_static);
					DebugOnly( Engine::Interface::HudMessagePrint(L"Static camera") );
				}
				else if(new_state == config.View.Input.Wireframe)
				{
					YELO_MEM_WLIST_BEGIN()
						*CAST_PTR(byte*, GET_FUNC_VPTR(RENDER_FUNC_SET_DRAW_MODE)) ^= 3;
					YELO_MEM_WLIST_END();
				}
				else if(new_state == config.View.Input.Screenshot)
				{
					XboxLib::Screenshot();
				}
				else if(new_state == config.Input.SaveGameState)
				{
					DebugOnly( Engine::Interface::HudMessagePrint(L"Saving...") );
					Engine::Game::Save();
				}
				else if(new_state == config.Input.LoadGameState)
				{
					Engine::Game::Revert();
					DebugOnly( Engine::Interface::HudMessagePrint(L"Loaded") );
				}
				else if(new_state == config.Input.AiToggle)
				{
					*GameState::_AiGlobals()->GetEnabled() ^= true;
				}
				else if(new_state == config.View.Input.Letterbox)
				{
					GameState::s_cinematic_globals* cg = GameState::_CinematicGlobals();
					if( cg->ShowLetterbox ^= true )	cg->Time = XboxLib::Math::RealConstants.Zero;
					else							cg->Time = XboxLib::Math::RealConstants.One;

				}
				else if(new_state == config.View.Input.HudOpacity)
				{
					static bool HudStatus = true;

					HudStatus ^= true;
					// Set the opacity
				}
				else if(new_state == config.Input.GameSpeedToggle)
				{
					static bool GameSpeedStatus = true;

					GameSpeedStatus ^= true;
					GameState::_GameTimeGlobals()->GameSpeed = (real)GameSpeedStatus;
				}
				else if(new_state == config.Cheats.Input.AiEraseAll)
				{
					Engine::AI::Erase();
				}
				else if(camera_is_valid && new_state == config.Input.ThirdPersonOverride)
				{
					// wait, what?
				}
				else if(new_state == config.Input.Cinematic.On)
				{
					Engine::Cinematic::Start();
					Engine::Cinematic::ScreenEffectStart();
					Engine::Cinematic::RenderLightsEnableCinematicShadow();
					Engine::Cinematic::LightmapShadow();
					Engine::Cinematic::AmbienceDetails();

					DebugOnly( Engine::Interface::HudMessagePrint(L"Cinematic on") );
				}
				else if(new_state == config.Input.Cinematic.Off)
				{
					Engine::Cinematic::Stop();
					Engine::Cinematic::ScreenEffectStop();
					Engine::Cinematic::LightmapShadow(false);
					Engine::Cinematic::AmbienceDetails(false);

					DebugOnly( Engine::Interface::HudMessagePrint(L"Cinematic off", pad.Index) );
				}
				else if(new_state == config.Cheats.Input.Invincible)
				{
					config.Cheats.UpdateFlags.Invincible = true;
				}
				else if(new_state == config.Cheats.Input.Cloak)
				{
					config.Cheats.UpdateFlags.Invincible = true;
				}
				else if(new_state == config.Cheats.Input.InfAmmo)
				{
					static bool InfAmmo = false;

					Engine::Cheats::InfiniteAmmo( InfAmmo ^= true );
				}
				else if(new_state == config.Cheats.Input.InfGrenades)
				{
					static bool InfGrens = false;

					Engine::Cheats::InfiniteGrenades( InfGrens ^= true );
				}
				else if(camera_is_valid && new_state == config.Input.TrackMovement)
				{
					camera.ToggleTrackMovement();
				}
				else if(new_state == config.Input.ChangeActiveConfig)
				{
					int32& index = Config::CurrentIndex();
					if(++index == Enums::k_config_definition_count)
						index = 0;
				}
				else if(new_state == config.Input.HighSpeedCapture)
				{
					config.Misc.HighSpeedCapture ^= true;
				}
				else if(new_state == config.View.Input.DisplayPosition)
				{
					config.Misc.CoordinateDisplay ^= true;
				}
				else if(new_state == config.View.Input.ToggleDof)
				{
				}
				else if(new_state == config.View.Input.ToggleBloom)
				{
				}
				else if(new_state == config.View.Input.ToggleCrossFade)
				{
				}
				else if(new_state == config.Cheats.Input.AutoWin)
				{
					Engine::Game::Won();
				}
			}
			else // execute multiple times per press
			{
				GameState::s_observer::s_calculated_origin* origin = 
					&GameState::_Observers()[pad.Index].origin;
				real* real_ptr;

				if(new_state == config.View.Look.Input.IncSpeed)
				{
					const real k_limit = config.View.Look.SpeedBounds.upper;
					real_ptr = &config.View.Look.Speed;
					real_tmp = (*real_ptr -= kAdjustment);
					if(real_tmp > k_limit)
						*real_ptr = k_limit;
				}
				else if(new_state == config.View.Look.Input.DecSpeed)
				{
					const real k_limit = config.View.Look.SpeedBounds.lower;
					real_ptr = &config.View.Look.Speed;
					real_tmp = (*real_ptr -= kAdjustment);
					if(real_tmp < k_limit)
						*real_ptr = k_limit;
				}
				else if(new_state == config.View.Look.Input.IncHorizShift)
				{
					origin->look_offset.x += kAdjustment;
				}
				else if(new_state == config.View.Look.Input.DecHorizShift)
				{
					origin->look_offset.x -= kAdjustment;
				}
				else if(new_state == config.View.Look.Input.IncVertShift)
				{
					origin->look_offset.y += kAdjustment;
				}
				else if(new_state == config.View.Look.Input.DecVertShift)
				{
					origin->look_offset.y -= kAdjustment;
				}

				else if(new_state == config.View.Move.Input.IncSpeed)
				{
					const real k_limit = config.View.Move.SpeedBounds.upper;
					real_ptr = &config.View.Move.Speed;
					real_tmp = (*real_ptr -= kAdjustment);
					if(real_tmp > k_limit)
						*real_ptr = k_limit;
				}
				else if(new_state == config.View.Move.Input.DecSpeed)
				{
					const real k_limit = config.View.Move.SpeedBounds.lower;
					real_ptr = &config.View.Move.Speed;
					real_tmp = (*real_ptr -= kAdjustment);
					if(real_tmp < k_limit)
						*real_ptr = k_limit;
				}
				else if(new_state == config.View.Move.Input.IncHorizShift)
				{
					origin->offset.x += kAdjustment;
				}
				else if(new_state == config.View.Move.Input.DecHorizShift)
				{
					origin->offset.x -= kAdjustment;
				}
				else if(new_state == config.View.Move.Input.IncVertShift)
				{
					origin->offset.y += kAdjustment;
				}
				else if(new_state == config.View.Move.Input.DecVertShift)
				{
					origin->offset.y -= kAdjustment;
				}

				else if(new_state == config.Input.GameSpeed.Inc)
				{
					GameState::_GameTimeGlobals()->GameSpeed += kAdjustment;
				}
				else if(new_state == config.Input.GameSpeed.Dec)
				{
					real_ptr = &GameState::_GameTimeGlobals()->GameSpeed;
					real_tmp = (*real_ptr -= kAdjustment);
					if(real_tmp < XboxLib::Math::RealConstants.Zero)
						*real_ptr = XboxLib::Math::RealConstants.Zero;
				}
				else if(new_state == config.View.InputSliders.Depth.Inc)
				{
					origin->depth += kAdjustment;
				}
				else if(new_state == config.View.InputSliders.Depth.Dec)
				{
					real_ptr = &origin->depth;
					real_tmp = (*real_ptr -= kAdjustment);
					if(real_tmp < XboxLib::Math::RealConstants.Zero)
						*real_ptr = XboxLib::Math::RealConstants.Zero;
				}
				else if(new_state == config.View.InputSliders.Fov.Inc)
				{
					origin->fov += kAdjustment;
				}
				else if(new_state == config.View.InputSliders.Fov.Dec)
				{
					origin->fov -= kAdjustment;
				}
			}
		}

		HANDLE GamepadHandles(int32 gamepad_index)
		{
			return GET_PTR2(input_gamepad_handles)[gamepad_index];
		}

		API_FUNC_NAKED s_gamepad* GamepadStates(int32 gamepad_index)
		{
			static uint32 CALL_ADDR = GET_FUNC_PTR(INPUT_GET_GAMEPAD_STATE);

			API_FUNC_NAKED_START()
				push	ecx
#if PLATFORM_ID == PLATFORM_H2_ALPHA
				push	esi
#else
				push	edx
#endif

#if PLATFORM_ID == PLATFORM_H2_ALPHA
				mov		esi, gamepad_index
#else
				mov		eax, gamepad_index
#endif
				call	CALL_ADDR

#if PLATFORM_ID == PLATFORM_H2_ALPHA
				pop		esi
#else
				pop		edx
#endif
				pop		ecx
			API_FUNC_NAKED_END(1);
		}

		s_yelopad& CurrentStates(int32 controller_index)
		{
			static s_yelopad instances[Enums::k_number_of_controllers] = {
				{0},
				{1},
				{2},
				{3},
			};
			return instances[controller_index];
		}

		s_yelopad& YeloPadCreate(s_yelopad& pad)
		{
			byte* gamepad_handle = CAST_PTR(byte*, Yelo::Input::GamepadHandles(pad.Index));
			if(!gamepad_handle)
			{
				pad.Index = NONE;
				return pad;
			}

			Input::s_gamepad* gamepad = CAST_PTR(
				Input::s_gamepad*,
				gamepad_handle+0x14
				);

			int16 index;

			// builds word_flags of gamepad buttons
			{
				pad.wOldButtons = pad.wButtons;
				pad.wOldThumbDirections = pad.wThumbDirections;

				#pragma region Buttons
				word_flags bitmask = 0;
				word_flags x;
				for(x = Flags::_input_button_dpad_up;
					x < Flags::_input_button_a;
					x <<= 1
					)
				{
					if(gamepad->wButtons & x)
						bitmask |= x;
				}


				index = 0;
				byte* buttons = gamepad->bAnalogButtons;
				for(x = Flags::_input_button_a;
					index < NUMBEROF(gamepad->bAnalogButtons);
					x <<= 1, index++
					)
				{
					if(buttons[index] > 0x3F)
						bitmask |= x;
				}
				#pragma endregion

				pad.wButtons = bitmask;

				#pragma region Thumbsticks
				bitmask = 0;
				index = 0;
				point2d* point = &gamepad->ThumbLeft;
				int16 left_shift = 0;
				for(x = Flags::_input_thumb_direction_left_west;
					index < 2;
					x <<= 4, index++, point++, left_shift = (index*4)
					)
				{
					int16 tmp;
					if( (tmp = point->x) != 0 )
					{
						if(tmp > 0)	bitmask |= Flags::_input_thumb_direction_left_east << left_shift;
						else		bitmask |= Flags::_input_thumb_direction_left_west << left_shift;
					}

					if( (tmp = point->y) != 0 )
					{
						if(tmp > 0)	bitmask |= Flags::_input_thumb_direction_left_north << left_shift;
						else		bitmask |= Flags::_input_thumb_direction_left_south << left_shift;
					}
				}
				#pragma endregion

				pad.wThumbDirections = bitmask;
			}

			// gets trigger input
			{
				pad.LeftTrigger =
					(real)(gamepad->AnalogButtons.LeftTrigger / K_BYTE_MAX);

				pad.RightTrigger =
					(real)(gamepad->AnalogButtons.RightTrigger / K_BYTE_MAX);
			}

			// gets thumbstick input
			{
				real_point2d* point = &pad.ThumbLeft;
				int16* values = &gamepad->ThumbLeft.x;
				for(index = 0; index < 2; index++,
					point++, // goto the next thumb XY value in the yelo pad
					values += 2 // goto the next thumb XY value in the gamepad
					)
				{
					point->x =
						-(real)(values[0] / K_INT16_MAX);

					point->y =
						(real)(values[1] / K_INT16_MAX);
				}
			}

			// debounces analog inputs
			{
				real tolerance = Config::Current().Input.Tolerance;
				real* value = &pad.LeftTrigger;
				real tmp;
				for(index = 0; index < (1 + 1 + 2 + 2); index++,
					value++ // goto the next real value in the yelo pad
					)
				{
					tmp = *value; // store the float, hopefully in a register
					if( XboxLib::Math::fabsf(tmp) < tolerance )
					{
						*value = 0.0f;
						continue;
					}

					if(tmp > 0.0f)	tmp -= tolerance;
					else			tmp += tolerance;

					*value = (1.0f / (1.0f - tolerance)) * tmp;
				}
			}

			return pad;
		}
	};
};