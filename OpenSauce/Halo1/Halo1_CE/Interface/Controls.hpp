/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI
#include "Interface/ControlTypes.hpp"

namespace Yelo
{
	namespace Input
	{
		void Initialize();
		void Dispose();
		void Update();

		struct ControlSettings
		{
			bool Initialized;
			PAD8;
			int16 Device; // ControlDevice
			PAD16;
			int16 Type; // ControlType
			int16 Index; // Key / GamepadButton / GamepadAxis / MouseButton / MouseAxis
			int16 Direction; // GamepadAxisDirection / GamepadDpadDirection / MouseAxisDirection
		};

		struct GamepadAxisState
		{
			byte Speed; // not even used
			sbyte Direction; // -16 to 16, screen coords
		};

		bool IsInGame();
		bool IsInMenu();
		bool IsInChat();
		void AllowMovement(bool allow);


		int16 SettingsGetDevice(Enums::PlayerControl control);
		int16 SettingsGetType(Enums::PlayerControl control);
		int16 SettingsGetIndex(Enums::PlayerControl control);
		int16 SettingsGetDirection(Enums::PlayerControl control);

		// device = [Enums::_ControlDeviceKeyboard]
		//	* type = UNUSED
		//	* index = [Enums::Key]
		//	* direction = UNUSED
		//
		// device = [Enums::_ControlDeviceMouse]
		//	* type = [Enums::_ControlTypeButton]
		//		- index = [Enums::MouseButton]
		//		- direction = UNUSED
		//	* type = [Enums::_ControlTypeAxis]
		//		- index = [Enums::MouseAxis]
		//		- direction = [Enums::MouseAxisDirection]
		//
		// device = [Enums::_ControlDeviceGamepad]
		//	* type = Enums::_ControlTypeButton
		//		- index = [Enums::GamepadButton]
		//		- direction = UNUSED
		//	* type = Enums::_ControlTypeAxis
		//		- index = [Enums::GamepadAxis]
		//		- direction = [Enums::GamepadAxisDirection]
		//	* type = Enums::_ControlTypeDpad
		//		- index = [Enums::GamepadDpad]
		//		- direction = [Enums::GamepadDpadDirection]

		int32 GetControlState(int16 device, int16 type, int16 index, int16 direction);
		void SetControlState(int16 device, int16 type, int16 index, int32 state);

		// Returns >0 if [control] is active on any input devices currently being used
		API_INLINE int32 GetControlState(Enums::PlayerControl control) { return GetControlState(SettingsGetDevice(control), SettingsGetType(control), SettingsGetIndex(control), SettingsGetDirection(control)); }
		// Sets the state of [control] on all currently active input devices
		API_INLINE void SetControlState(Enums::PlayerControl control, int32 state) { SetControlState(SettingsGetDevice(control), SettingsGetType(control), SettingsGetIndex(control), state); }

		// Returns true if [control] is assigned to the keyboard
		API_INLINE bool IsKeyboardControl(Enums::PlayerControl control) { return SettingsGetDevice(control) == Enums::_ControlDeviceKeyboard; }
		// Returns true if [control] is assigned to the mouse
		API_INLINE bool IsMouseControl(Enums::PlayerControl control) { return SettingsGetDevice(control) == Enums::_ControlDeviceMouse; }
		// Returns true if [control] is assigned to the gamepad
		API_INLINE bool IsGamepadControl(Enums::PlayerControl control){ return SettingsGetDevice(control) == Enums::_ControlDeviceGamepad; }


		// Gets the current state of [key]
		// The larger the return value, the longer its been held down
		byte GetKeyState(Enums::key_code key);
		// The larger the value of [state], the longer its been held down
		void SetKeyState(Enums::key_code key, byte state);


		// Gets the current state of mouse [button]
		// The larger the return value, the longer its been held down
		byte GetMouseButtonState(Enums::MouseButton button);
		// Sets the current state of mouse [button]
		// The larger the value of [state], the longer its been held down
		void SetMouseButtonState(Enums::MouseButton button, byte state);

		// Gets the current state of mouse [axis]
		// The larger the return value, the longer its been held down
		int32 GetMouseAxisState(Enums::MouseAxis axis);
		// Sets the current state of mouse [axis]
		// The larger the value of [state], the longer its been held down
		void SetMouseAxisState(Enums::MouseAxis axis, int32 state);


		// Gets the current state of gamepad [button]
		// The larger the return value, the longer its been held down
		byte GetGamepadButtonState(Enums::GamepadButton button);
		// Sets the current state of gamepad [button]
		// The larger the value of [state], the longer its been held down
		void SetGamepadButtonState(Enums::GamepadButton button, byte state);

		// Gets the current state of gamepad [axis]
		// The larger the return value, the longer its been held down
		sbyte GetGamepadAxisState(Enums::GamepadAxis axis);
		// Sets the current state of gamepad [axis]
		// The larger the value of [state], the longer its been held down
		void SetGamepadAxisState(Enums::GamepadAxis axis, sbyte state);

		// Gets the current state of [dpad]
		// The larger the return value, the longer its been held down
		int32 GetGamepadDpadState(Enums::GamepadDpad dpad);
		// Sets the current state of [dpad]
		// The larger the value of [state], the longer its been held down
		void SetGamepadDpadState(Enums::GamepadDpad dpad, int32 state);

#ifdef API_DEBUG
		// Returns the string version of [value]
		cstring ControlTypeToString(Enums::ControlType value);
		// Returns the string version of [value]
		cstring GamepadDpadDirectionToString(Enums::GamepadDpadDirection value);
		// Returns the string version of [value]
		cstring PlayerControlToString(Enums::PlayerControl value);
#endif
	};
};
#endif