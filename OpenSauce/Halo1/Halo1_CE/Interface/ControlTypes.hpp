/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <blamlib/Halo1/input/input_abstraction.hpp>
#include <blamlib/Halo1/input/input_windows.hpp>

namespace Yelo
{
	namespace Enums
	{
		enum ControlDevice
		{
			_ControlDeviceUndefined,
			_ControlDeviceKeyboard,
			_ControlDeviceMouse,
			_ControlDeviceGamepad,
			_ControlDevice,
		};

		enum ControlType
		{
			_ControlTypeButton,
			_ControlTypeAxis,
			_ControlTypeDpad,
			_ControlType,
		};

		enum GamepadButton // k_maximum_gamepad_buttons
		{
			_GamepadButton1,
			_GamepadButton2,
			_GamepadButton3,
			_GamepadButton4,
			_GamepadButton5,
			_GamepadButton6,
			_GamepadButton7,
			_GamepadButton8,
			_GamepadButton9,
			_GamepadButton10,
			_GamepadButton11,
			_GamepadButton12,
			_GamepadButton13,
			_GamepadButton14,
			_GamepadButton15,
			_GamepadButton16,
			_GamepadButton17,
			_GamepadButton18,
			_GamepadButton19,
			_GamepadButton20,
			_GamepadButton21,
			_GamepadButton22,
			_GamepadButton23,
			_GamepadButton24,
			_GamepadButton25,
			_GamepadButton26,
			_GamepadButton27,
			_GamepadButton28,
			_GamepadButton29,
			_GamepadButton30,
			_GamepadButton31,
			_GamepadButton32,
			_GamepadButton,
		};

		enum GamepadAxis
		{
			_GamepadAxisRightY,
			_GamepadAxisRightX,
			_GamepadAxisLeftY,
			_GamepadAxisLeftX,
			_GamepadAxis,
		};

		enum GamepadDpad
		{
			_GamepadDpad1,
			_GamepadDpad2,
			_GamepadDpad3,
			_GamepadDpad4,
			_GamepadDpad,
		};

		enum GamepadAxisDirection
		{
			_GamepadAxisDirectionNone,
			_GamepadAxisDirectionDown,
			_GamepadAxisDirectionRight = _GamepadAxisDirectionDown,
			_GamepadAxisDirectionUp,
			_GamepadAxisDirectionLeft = _GamepadAxisDirectionUp,
			_GamepadAxisDirection,
		};

		enum GamepadDpadDirection // TODO: transition to pov_direction (input_windows) and remove
		{
			_GamepadDpadDirectionNone = -1,
			_GamepadDpadDirectionNorth,
			_GamepadDpadDirectionNorthEast,
			_GamepadDpadDirectionEast,
			_GamepadDpadDirectionSouthEast,
			_GamepadDpadDirectionSouth,
			_GamepadDpadDirectionSouthWest,
			_GamepadDpadDirectionWest,
			_GamepadDpadDirectionNorthWest,
			_GamepadDpadDirection,
		};

		enum MouseButton // TODO: transition to mouse_button (input_windows) and remove
		{
			// Left
			_MouseButton1,
			// Middle
			_MouseButton2,
			// Right
			_MouseButton3,
			// Side back
			_MouseButton4,
			// Side forward
			_MouseButton5,
			_MouseButton6,
			_MouseButton7,
			_MouseButton8,
			_MouseButton,
		};

		enum MouseAxis // TODO: transition to mouse_axes (input_windows) and remove
		{
			_MouseAxisX,
			_MouseAxisY,
			_MouseAxisWheel,
			_MouseAxis,
		};

		enum MouseAxisDirection
		{
			_MouseAxisDirectionNone,
			_MouseAxisDirectionUp,
			_MouseAxisDirectionRight = _MouseAxisDirectionUp,
			_MouseAxisDirectionDown,
			_MouseAxisDirectionLeft = _MouseAxisDirectionDown,
			_MouseAxisDirection,
		};

		enum PlayerControl // TODO: transition to control_buttons (input_windows) and remove
		{
			_PlayerControlJump,
			_PlayerControlSwitchGrenade,
			_PlayerControlAction,
			_PlayerControlSwitchWeapon,
			_PlayerControlMelee,
			_PlayerControlFlashlight,
			_PlayerControlThrowGrenade,
			_PlayerControlFire,
			_PlayerControlAccept,
			_PlayerControlBack,
			_PlayerControlCrouch,
			_PlayerControlZoom,
			_PlayerControlShowScores,
			_PlayerControlReload,
			_PlayerControlPickup,
			_PlayerControlSay,
			_PlayerControlSayToTeam,
			_PlayerControlSayToVehicle,
			_PlayerControlScreenshot,
			_PlayerControlMoveForward,
			_PlayerControlMoveBackward,
			_PlayerControlMoveLeft,
			_PlayerControlMoveRight,
			_PlayerControlLookUp,
			_PlayerControlLookDown,
			_PlayerControlLookLeft,
			_PlayerControlLookRight,
			_PlayerControlShowRules,
			_PlayerControlShowHomies,
			_PlayerControl,
		};

		enum KeyModifier
		{
			_KeyModifierNone,
			_KeyModifierShift,
			_KeyModifierCtrl,
			_KeyModifierAlt,
			_KeyModifierShiftCtrl,
			_KeyModifierShiftAlt,
			_KeyModifierCtrlAlt,
			_KeyModifierShiftCtrlAlt,
			_KeyModifier,
		};
	};
};