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

		enum GamepadButton
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

		enum GamepadDpadDirection
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

		enum MouseButton
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

		enum MouseAxis
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

		enum PlayerControl
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

		enum Key : _enum
		{
			_KeyEsc,
			_KeyF1,
			_KeyF2,
			_KeyF3,
			_KeyF4,
			_KeyF5,
			_KeyF6,
			_KeyF7,
			_KeyF8,
			_KeyF9,
			_KeyF10,
			_KeyF11,
			_KeyF12,
			_KeyPrntScrn,
			_KeyScrollLock,

			_KeyPause,
			_KeyTilde,

			_Key1,
			_Key2,
			_Key3,
			_Key4,
			_Key5,
			_Key6,
			_Key7,
			_Key8,
			_Key9,
			_Key0,
			_KeyMinus,
			_KeyPlus,
			_KeyBackspace,

			_KeyTab,
			_KeyQ,
			_KeyW,
			_KeyE,
			_KeyR,
			_KeyT,
			_KeyY,
			_KeyU,
			_KeyI,
			_KeyO,
			_KeyP,
			_KeyLBracket,
			_KeyRBracket,
			_KeyPipe,

			_KeyCaps,
			_KeyA,
			_KeyS,
			_KeyD,
			_KeyF,
			_KeyG,
			_KeyH,
			_KeyJ,
			_KeyK,
			_KeyL,
			_KeyColon,
			_KeyQuote,
			_KeyEnter,

			_KeyLShift,
			_KeyZ,
			_KeyX,
			_KeyC,
			_KeyV,
			_KeyB,
			_KeyN,
			_KeyM,
			_KeyComma,
			_KeyPeriod,
			_KeySlash,
			_KeyRShift,

			_KeyLCtrl,
			_KeyLWin,
			_KeyLAlt,
			_KeySpace,
			_KeyRAlt,
			_KeyRWin,
			_KeyAppsMenu,
			_KeyRCtrl,

			_KeyUp,
			_KeyDown,
			_KeyLeft,
			_KeyRight,

			_KeyInsert,
			_KeyHome,
			_KeyPgUp,
			_KeyDelete,
			_KeyEnd,
			_KeyPgDown,

			_KeyNumsLock,
			_KeyNumDivide,
			_KeyNumMultiply,
			_KeyNum0,
			_KeyNum1,
			_KeyNum2,
			_KeyNum3,
			_KeyNum4,
			_KeyNum5,
			_KeyNum6,
			_KeyNum7,
			_KeyNum8,
			_KeyNum9,
			_KeyNumMinus,
			_KeyNumAdd,
			_KeyNumEnter,
			_KeyNumDot,

			_Key,
		};
	};

	namespace Flags
	{
		enum buffered_key_flags : byte_flags
		{
			_buffered_key_shift_flag =	FLAG(0),// SHIFT key pressed
			_buffered_key_control_flag =FLAG(1),// CTRL key pressed
			_buffered_key_alt_flag =	FLAG(2),// ALT key pressed
		};
	};

	namespace Input
	{
		struct s_buffered_key
		{
			Flags::buffered_key_flags flags;
			byte state;			// how long its been pressed until 0xFF, 0 if not pressed
			Enums::Key key_code;
		}; BOOST_STATIC_ASSERT( sizeof(s_buffered_key) == 0x4 );
	};
};