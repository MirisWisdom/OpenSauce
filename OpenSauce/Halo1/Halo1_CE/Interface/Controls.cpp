/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Interface/Controls.hpp"
#if !PLATFORM_IS_DEDI

#include "Memory/MemoryInterface.hpp"
#include "Interface/OpenSauceUI.hpp"

namespace Yelo
{
	namespace Flags
	{
		enum input_state {
			_input_state_enabled_bit,
			_input_state_menu_bit,
			_input_state_chat_bit,
		};
	};

	namespace Input
	{
		struct ControlStates
		{
			// key states (how long its been pressed until 0xFF, 0 if not pressed)
			byte Keys[Enums::k_number_of_keys];

			PAD(0, sizeof(byte)*383);

			int32 MouseAxis[Enums::_MouseAxis];
			byte MouseButton[Enums::_MouseButton];

			PAD(1, sizeof(byte)*4680);

			byte GamepadButton[Enums::_GamepadButton];
			GamepadAxisState GamepadAxis[Enums::_GamepadAxis];

			PAD(2, sizeof(byte)*56);

			int32 GamepadDpad[Enums::_GamepadDpad];
		};
		
		struct PositionState
		{
			//PAD16;
			//bool Moving; // true during mouse movement
			//PAD8;
			PAD32;
			int32 Position[2]; // menu space coordinates (0,0) to (640,480)
		};

#define __EL_INCLUDE_ID			__EL_INCLUDE_INTERFACE
#define __EL_INCLUDE_FILE_ID	__EL_INTERFACE_CONTROLS
#include "Memory/_EngineLayout.inl"

		void Initialize()
		{
			Memory::CreateHookRelativeCall(&Input::Update, GET_FUNC_VPTR(INPUT_UPDATE_HOOK), Enums::_x86_opcode_ret);
		}

		void Dispose()
		{
		}

		void Update()
		{
			Interface::OpenSauceUI::Update();
		}


		bool IsInGame()					{ return TEST_FLAG(GET_PTR(InputStateFlags), Flags::_input_state_enabled_bit); }
		bool IsInMenu()					{ return TEST_FLAG(GET_PTR(InputStateFlags), Flags::_input_state_menu_bit); }
		bool IsInChat()					{ return TEST_FLAG(GET_PTR(InputStateFlags), Flags::_input_state_chat_bit); }
		void AllowMovement(bool allow)	{ SET_FLAG(GET_PTR(InputStateFlags), Flags::_input_state_enabled_bit, allow); }


		int32 GetControlState(int16 device, int16 type, int16 index, int16 direction)
		{
			int32 state = 0;

			if(device == Enums::_ControlDeviceKeyboard)		state = GetKeyState( (Enums::key_code)index );
			else if(device == Enums::_ControlDeviceMouse)
			{
				if(type == Enums::_ControlTypeButton)		state = GetMouseButtonState( (Enums::MouseButton)index );
				else if(type == Enums::_ControlTypeAxis)	state = GetMouseAxisState( (Enums::MouseAxis)index );
			}
			else if(device == Enums::_ControlDeviceGamepad)
			{
				if(type == Enums::_ControlTypeButton)		state = GetGamepadButtonState( (Enums::GamepadButton)index );
				else if(type == Enums::_ControlTypeAxis)	state = GetGamepadAxisState( (Enums::GamepadAxis)index );
				else if(type == Enums::_ControlTypeDpad)	state = GetGamepadDpadState( (Enums::GamepadDpad)index );
			}

			// Check if direction matches setting
			if(	type == Enums::_ControlTypeAxis && 
				direction && 
				state != (direction > 0 ? 1 : 2) )
				state = 0;
			else if(type == Enums::_ControlTypeDpad && 
					state != direction )
				state = 0;

			return state;
		}

		void SetControlState(int16 device, int16 type, int16 index, int32 state)
		{
			if(device == Enums::_ControlDeviceKeyboard)		SetKeyState( (Enums::key_code)index, (byte)state);
			else if(device == Enums::_ControlDeviceMouse)
			{
				if(type == Enums::_ControlTypeButton)		SetMouseButtonState( (Enums::MouseButton)index, (byte)state);
				else if(type == Enums::_ControlTypeAxis)	SetMouseAxisState( (Enums::MouseAxis)index, state);
			}
			else if(device == Enums::_ControlDeviceGamepad)
			{
				if(type == Enums::_ControlTypeButton)		SetGamepadButtonState( (Enums::GamepadButton)index, (byte)state);
				else if(type == Enums::_ControlTypeAxis)	SetGamepadAxisState( (Enums::GamepadAxis)index, (sbyte)state);
				else if(type == Enums::_ControlTypeDpad)	SetGamepadDpadState( (Enums::GamepadDpad)index, state);
			}
		}

		int16 SettingsGetDevice(Enums::PlayerControl control)			{ return GET_PTR2(Settings)[control].Device; }
		int16 SettingsGetType(Enums::PlayerControl control)				{ return GET_PTR2(Settings)[control].Type; }
		int16 SettingsGetIndex(Enums::PlayerControl control)			{ return GET_PTR2(Settings)[control].Index; }
		int16 SettingsGetDirection(Enums::PlayerControl control)		{ return GET_PTR2(Settings)[control].Direction; }

		byte GetKeyState(Enums::key_code key)							{ return GET_PTR2(ControlState)->Keys[key]; }
		void SetKeyState(Enums::key_code key, byte state)				{ GET_PTR2(ControlState)->Keys[key] = state; }


		byte GetMouseButtonState(Enums::MouseButton button)				{ return GET_PTR2(ControlState)->MouseButton[button]; }
		void SetMouseButtonState(Enums::MouseButton button, byte state)	{ GET_PTR2(ControlState)->MouseButton[button] = state; }

		int32 GetMouseAxisState(Enums::MouseAxis axis)					{ return GET_PTR2(ControlState)->MouseAxis[axis]; }
		void SetMouseAxisState(Enums::MouseAxis axis, int32 state)		{ GET_PTR2(ControlState)->MouseAxis[axis] = state; }

		int32 GetMousePositionState(Enums::MouseAxis axis)				{ if(axis > Enums::_MouseAxisY) return 0; return GET_PTR2(MousePositionState)->Position[axis]; }
		void SetMousePositionState(Enums::MouseAxis axis, int32 position) { if(axis > Enums::_MouseAxisY) return; GET_PTR2(MousePositionState)->Position[axis] = position; }


		byte GetGamepadButtonState(Enums::GamepadButton button)				{ return GET_PTR2(ControlState)->GamepadButton[button]; }
		void SetGamepadButtonState(Enums::GamepadButton button, byte state)	{ GET_PTR2(ControlState)->GamepadButton[button] = state; }

		sbyte GetGamepadAxisState(Enums::GamepadAxis axis)				{ return GET_PTR2(ControlState)->GamepadAxis[axis].Direction; }
		void SetGamepadAxisState(Enums::GamepadAxis axis, sbyte state)	{ GET_PTR2(ControlState)->GamepadAxis[axis].Direction = state; GET_PTR2(ControlState)->GamepadAxis[axis].Speed = 0; }

		int32 GetGamepadDpadState(Enums::GamepadDpad dpad)				{ return GET_PTR2(ControlState)->GamepadDpad[dpad]; }
		void SetGamepadDpadState(Enums::GamepadDpad dpad, int32 state)	{ GET_PTR2(ControlState)->GamepadDpad[dpad] = state; }

#ifdef API_DEBUG
		cstring ControlTypeToString(Enums::ControlType value)
		{
			static cstring string_list[] = {
				"button",
				"axis",
				"pov",
			};

			return string_list[value];
		}

		cstring GamepadDpadDirectionToString(Enums::GamepadDpadDirection value)
		{
			static cstring string_list[Enums::k_number_of_pov_directions] = {
				"north",
				"northeast",
				"east",
				"southeast",
				"south",
				"southwest",
				"west",
			};

			return string_list[value];
		}

		cstring PlayerControlToString(Enums::PlayerControl value)
		{
			static cstring string_list[Enums::k_number_of_total_control_buttons] = {
				"jump",
				"switch_grenade",
				"action",
				"switch_weapon",
				"melee",
				"flashlight",
				"throw_grenade",
				"fire",
				"accept",
				"back",
				"crouch",
				"zoom",
				"showscores",
				"reload",
				"exchange_weapon",
				"say",
				"sayteam",
				"sayvehicle",
				"screenshot",
				"forward",
				"backward",
				"left",
				"right",
				"look_up",
				"look_down",
				"look_left",
				"look_right",
				"showrules",
				"show_homies",
			};

			return string_list[value];
		}
#endif
	};
};

#endif