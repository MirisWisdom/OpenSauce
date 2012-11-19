/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

namespace Yelo
{
	namespace Enums
	{
		enum {
			k_maximum_buffered_keystrokes = 64, // MAXIMUM_BUFFERED_KEYSTROKES

			k_maximum_gamepads = 4,
			k_maximum_gamepad_axes = 32,
			k_maximum_gamepad_buttons = 32,
			k_maximum_gamepad_povs = 16,
			k_maximum_enumerated_joysticks = 8,

			k_number_of_virtual_codes = 256,
			k_number_of_keys = 109,
		};

		enum {
			_axis_direction_positive,
			_axis_direction_negative,

			k_number_of_axis_directions, // NUMBER_OF_AXIS_DIRECTIONS
		};

		enum {
			_pov_direction_none = NONE,

			_pov_direction_north,
			_pov_direction_north_east,
			_pov_direction_east,
			_pov_direction_south_east,
			_pov_direction_south,
			_pov_direction_south_west,
			_pov_direction_west,
			_pov_direction_north_west,

			k_number_of_pov_directions = 8, // NUMBER_OF_POV_DIRECTIONS
		};

		enum {
			_mouse_button_left,
			_mouse_button_middle,
			_mouse_button_right,
			_mouse_button4,
			_mouse_button5,
			_mouse_button6,
			_mouse_button7,
			_mouse_button8,

			k_number_of_mouse_buttons, // NUMBER_OF_MOUSE_BUTTONS
		};

		enum {
			_mouse_axes_horiz,
			_mouse_axes_vert,
			_mouse_axes_wheel,

			k_number_of_mouse_axes, // NUMBER_OF_MOUSE_AXES
		};

		enum {
			_button_none = NONE,

			_button_jump,
			_button_switch_grenade,
			_button_action,
			_button_switch_weapon,
			_button_melee,
			_button_flashlight,
			_button_throw_grenade,
			_button_fire,
			_button_accept,
			_button_back,
			_button_crouch,
			_button_zoom,
			_button_show_scores,
			_button_reload,
			_button_pickup,
			_button_say,
			_button_say_to_team,
			_button_say_to_vehicle,
			_button_screenshot,

			k_number_of_action_control_buttons, // NUMBER_OF_ACTION_CONTROL_BUTTONS

			_control_button_move_forward = k_number_of_action_control_buttons,
			_control_button_move_backward,
			_control_button_move_left,
			_control_button_move_right,
			_control_button_look_up,
			_control_button_look_down,
			_control_button_look_left,
			_control_button_look_right,

			_new_control_button_show_rules,
			_new_control_button_show_homies,

			k_number_of_total_control_buttons, // NUMBER_OF_TOTAL_CONTROL_BUTTONS
			k_first_new_control_button =		// FIRST_NEW_CONTROL_BUTTON
				_new_control_button_show_rules, 
		};
	};
};