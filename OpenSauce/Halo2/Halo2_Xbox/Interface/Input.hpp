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
		enum input_button
		{
			_input_button_a,
			_input_button_b,
			_input_button_x,
			_input_button_y,
			_input_button_black,
			_input_button_white,
			_input_button_left_trigger,
			_input_button_right_trigger,
			_input_button,
		};
	};

	namespace Flags
	{
		enum input_button
		{
			_input_button_dpad_up =					FLAG(0),
			_input_button_dpad_down =				FLAG(1),
			_input_button_dpad_left =				FLAG(2),
			_input_button_dpad_right =				FLAG(3),
			_input_button_start =					FLAG(4),
			_input_button_back =					FLAG(5),
			_input_button_left_thumb =				FLAG(6),
			_input_button_right_thumb =				FLAG(7),

			_input_button_a =						FLAG(8),
			_input_button_b =						FLAG(9),
			_input_button_x =						FLAG(10),
			_input_button_y =						FLAG(11),
			_input_button_black =					FLAG(12),
			_input_button_white =					FLAG(13),

			_input_button_lightgun_onscreen =		FLAG(13),
			_input_button_lightgun_frame_doubler =	FLAG(14),
			_input_button_lightgun_line_doubler =	FLAG(15),


			_input_button_left_trigger =			FLAG(14), // custom
			_input_button_right_trigger =			FLAG(15), // custom

			_input_button_undefined =				NONE, // For unassigned config in-game keys
		};

		enum input_thumb_direction
		{
			_input_thumb_direction_left_west = FLAG(0),
			_input_thumb_direction_left_east = FLAG(1),
			_input_thumb_direction_left_north = FLAG(2),
			_input_thumb_direction_left_south = FLAG(3),

			_input_thumb_direction_right_west = FLAG(4),
			_input_thumb_direction_right_east = FLAG(5),
			_input_thumb_direction_right_north = FLAG(6),
			_input_thumb_direction_right_south = FLAG(7),

			_input_thumb_direction_left_north_west =
				_input_thumb_direction_left_west |
				_input_thumb_direction_left_north,
			_input_thumb_direction_left_north_east =
				_input_thumb_direction_left_east |
				_input_thumb_direction_left_north,
			_input_thumb_direction_left_south_west =
				_input_thumb_direction_left_west |
				_input_thumb_direction_left_south,
			_input_thumb_direction_left_south_east =
				_input_thumb_direction_left_east |
				_input_thumb_direction_left_south,

			_input_thumb_direction_right_north_west =
				_input_thumb_direction_right_west |
				_input_thumb_direction_right_north,
			_input_thumb_direction_right_north_east =
				_input_thumb_direction_right_east |
				_input_thumb_direction_right_north,
			_input_thumb_direction_right_south_west =
				_input_thumb_direction_right_west |
				_input_thumb_direction_right_south,
			_input_thumb_direction_right_south_east =
				_input_thumb_direction_right_east |
				_input_thumb_direction_right_south,
		};
	};

	namespace Input
	{
		struct s_gamepad
		{
			struct _wButtons {
				uint16 DpadUp : 1;
				uint16 DpadDown : 1;
				uint16 DpadLeft : 1;
				uint16 DpadRight : 1;
				uint16 Start : 1;
				uint16 Back : 1;
				uint16 LeftThumb : 1;
				uint16 RightThumb : 1;
			};

			struct _bAnalogButtons {
				byte A;
				byte B;
				byte X;
				byte Y;
				byte Black;
				byte White;
				byte LeftTrigger;
				byte RightTrigger;
			};

			union {
				word_flags wButtons;
				_wButtons Buttons;
			};

			union {
				byte bAnalogButtons[8];
				_bAnalogButtons AnalogButtons;
			};

			point2d ThumbLeft, ThumbRight;

			inline point2d* LightGunPointer() { return &this->ThumbLeft; }
			inline point2d* LightGunThumbstick() { return &this->ThumbRight; }
		}; BOOST_STATIC_ASSERT( sizeof(s_gamepad) == 0x12 );

		struct s_yelopad
		{
			struct ThumbDirectionFlags
			{
				uint16 LeftNorth : 1;
				uint16 LeftSouth : 1;
				uint16 LeftWest : 1;
				uint16 LeftEast : 1;

				uint16 RightNorth : 1;
				uint16 RightSouth : 1;
				uint16 RightWest : 1;
				uint16 RightEast : 1;
			};

			struct ButtonFlags
			{
				uint16 DpadUp : 1;
				uint16 DpadDown : 1;
				uint16 DpadLeft : 1;
				uint16 DpadRight : 1;
				uint16 Start : 1;
				uint16 Back : 1;
				uint16 LeftThumb : 1;
				uint16 RightThumb : 1;

				uint16 A : 1;
				uint16 B : 1;
				uint16 X : 1;
				uint16 Y : 1;
				uint16 Black : 1;
				uint16 White : 1;
				uint16 LeftTrigger : 1;
				uint16 RightTrigger : 1;
			};

			int32 Index; // controller index


			union { // 0x4
				word_flags wOldButtons;
				ButtonFlags OldButtons;
			};
			union { // 0x6
				word_flags wButtons;
				ButtonFlags Buttons;
			};

			union { // 0x8
				word_flags wOldThumbDirections;
				ThumbDirectionFlags OldThumbDirections;
			};
			union { // 0xA
				word_flags wThumbDirections;
				ThumbDirectionFlags ThumbDirections;
			};


			real LeftTrigger;
			real RightTrigger;
			real_point2d ThumbLeft;
			real_point2d ThumbRight;

			inline bool IsNull() { return this->Index == NONE; }
		}; BOOST_STATIC_ASSERT( sizeof(s_yelopad) == 0x24 );

		struct s_state
		{
			DWORD PacketNumber;
			Input::s_gamepad Gamepad;
		};


		void Initialize();
		void Dispose();
		void Update();

		void HandleYeloInput(const s_yelopad& pad);

		HANDLE GamepadHandles(int32 gamepad_index = 0);
		s_gamepad* GamepadStates(int32 gamepad_index = 0);

		s_yelopad& CurrentStates(int32 controller_index);

		s_yelopad& YeloPadCreate(s_yelopad& pad);
	};
};