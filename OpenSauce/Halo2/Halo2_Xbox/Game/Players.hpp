/*
	Yelo: Open Sauce SDK
		Halo 2 (Xbox) Edition

	See license\OpenSauce\Halo2_Xbox for specific license information
*/
#pragma once
#include "Memory/Data.hpp"

namespace Yelo
{
	namespace Flags
	{
		enum player_action
		{
			_player_action_jump_bit,
			_player_action_primary_trigger_bit,
			_player_action_grenade_trigger_bit,
			_player_action_vision_trigger_bit,
			_player_action_zoom_bit,
			_player_action_rotate_weapons_bit,
			_player_action_rotate_grenades_bit,
			_player_action_melee_bit,
			_player_action_action_bit,
			_player_action_accept_bit,
			_player_action_cancel_bit,
			_player_action_look_relative_up_bit,
			_player_action_look_relative_down_bit,
			_player_action_look_relative_left_bit,
			_player_action_look_relative_right_bit,
			_player_action_move_relative_forward_bit,
			_player_action_move_relative_backward_bit,
			_player_action_move_relative_left_bit,
			_player_action_move_relative_right_bit,
			_player_action_start_bit,
			_player_action_back_bit,
			_player_action,

			_player_action_look_relative_directions =
				FLAG(_player_action_look_relative_up_bit) |
				FLAG(_player_action_look_relative_down_bit) |
				FLAG(_player_action_look_relative_left_bit) |
				FLAG(_player_action_look_relative_right_bit),
			_player_action_move_relative_all_directions =
				FLAG(_player_action_move_relative_forward_bit) |
				FLAG(_player_action_move_relative_backward_bit) |
				FLAG(_player_action_move_relative_left_bit) |
				FLAG(_player_action_move_relative_right_bit),
		};
	};

	namespace Players
	{
		void Initialize();
		void Dispose();

		void UpdateBefore(DWORD arg_0);
		void UpdateAfter();
	};

	namespace GameState
	{
		struct s_player_datum : TStructImpl(540)
		{
			TStructGetImpl(int16, Header, 0x0);
			// bit 0 - _player_connecting_to_game_bit
			// bit 1 - _player_left_game_bit
			TStructGetPtrImpl(byte_flags, Flags, 0x2);
			//TStructGetPtrImpl(void*, Identifier, 0x4); // 3 dword structure, simulation_identifier
			//TStructGetPtrImpl(int32, , 0x10);
			TStructGetPtrImpl(int16, MachineIndex, 0x14);
			TStructGetPtrImpl(int16, MachineUserIndex, 0x16);
			//TStructGetPtrImpl(int16, , 0x1A);
			TStructGetPtrImpl(int16, UserIndex,				PLATFORM_VALUE(0x20, 0x20, NULL_HANDLE)); // TODO: I think I have Alpha set to null since this didn't exist in that build?
			TStructGetPtrImpl(int32, ControllerIndex,		PLATFORM_VALUE(0x24, 0x24, 0x1C));
			TStructGetPtrImpl(int16, LocalIndex,			PLATFORM_VALUE(0x28, 0x28, 0x20));
			//TStructGetPtrImpl(int16, , 0x2A);
			TStructGetPtrImpl(datum_index, SlaveUnitIndex,	PLATFORM_VALUE(0x2C, 0x2C, 0x24));
			//TStructGetPtrImpl(int32, , 0x30);
			//TStructGetPtrImpl(int32, , 0x34);
			//TStructGetPtrImpl(byte, , 0x3E);

			//TStructGetPtrImpl(int32, , 0x174);
			//TStructGetPtrImpl(int16, , 0x17C);
			//TStructGetPtrImpl(int16, , 0x17E);
			//TStructGetPtrImpl(int32, , 0x184);
			//TStructGetPtrImpl(int16, , 0x188);
			//TStructGetPtrImpl(int32, , 0x18C);
			//TStructGetPtrImpl(int32, , 0x190);
			//TStructGetPtrImpl(int32, , 0x194);
			//TStructGetPtrImpl(real, , 0x19C); // speed mbmb?
			//TStructGetPtrImpl(int32, , 0x1A0);
			//TStructGetPtrImpl(int16, , 0x1A4);
			//TStructGetPtrImpl(byte, , 0x1A7);
			//TStructGetPtrImpl(byte, , 0x1A8);
			//TStructGetPtrImpl(int32, , 0x1B0);

			//TStructGetPtrImpl(void*, , 0x1D8); // [8] 8 byte structure, first short in the struct gets set to NONE

			//TStructGetPtrImpl(datum_index, , 0x30); // object
			//TStructGetPtrImpl(datum_index, , 0x174); // object
		};
		typedef Memory::DataArray<s_player_datum, 16> t_players_data;
		t_players_data* _Players();

		struct s_player_globals : TStructImpl(304)
		{
			TStructGetPtrImpl(bool, EnableInput, 0x6);
			TStructGetPtrImpl(bool, DisableMovement, 0x7);
			TStructGetPtrImpl(int16, PlayerUserCount, 0x8);
			TStructGetPtrImpl(int16, PlayerControllerCount, 0xA);
			TStructGetPtrImpl(long_flags, CameraFlags, 0x10);
			// int32 player_controller_mapping[]; // 0x1C

			void CameraControl(bool enable)
			{
				*this->GetEnableInput() = true;

				long_flags* flags = this->GetCameraFlags();
				SET_FLAG(*flags, 0, enable);
			}
		};
		s_player_globals* _PlayerGlobals();

		struct s_player_control_globals
		{
			struct s_player : TStructImpl(148)
			{
				TStructGetPtrImpl(datum_index, UnitIndex, 0x0);
				//TStructGetPtrImpl(word_flags, , 0xC);
				//TStructGetPtrImpl(word_flags, , 0xE);
				TStructGetPtrImpl(real_point2d, RightThumb, 0x10); // horiz, vertical
				TStructGetPtrImpl(real_point2d, LeftThumb, 0x18);
				TStructGetPtrImpl(real, RightTrigger, 0x20);
				TStructGetPtrImpl(real, LeftTrigger, 0x24);

				//TStructGetPtrImpl(int16, , 0x28); // same as unit's 0x214
				//TStructGetPtrImpl(byte_flags, , 0x2A);
				//TStructGetPtrImpl(byte_flags, , 0x2B);
				TStructGetPtrImpl(int16, CurrentGrenadeIndex, 0x2C);
				TStructGetPtrImpl(int16, CurrentZoomIndex, 0x2E);

				//TStructGetPtrImpl(real_vector3d, , 0x34);
				//TStructGetPtrImpl(datum_index?, , 0x40); // same as unit's 0x3E4
			};

			UNKNOWN_TYPE(int32);

			long_flags ActionFlags; // player_action
			UNKNOWN_TYPE(int32);
			UNKNOWN_TYPE(int32);

			UNKNOWN_TYPE(long_flags);
			bool CameraControlIsActive; PAD24;

			s_player Players[4]; // 0x14
		};
		s_player_control_globals* _PlayerControlGlobals();

		struct s_player_training_globals
		{
			struct s_player : TStructImpl(432)
			{
				// 1 = flashlight
				// 2 = ?
				// 4 = stealth (camo)
				TStructGetPtrImpl(byte_flags, ActivateFlags, 0x194);

				// 0x80 int32
				// 0x196 byte
				// 0x197 byte
				// 0x19B byte
				// 0x19D byte
				// 0x1A0 byte
			}Players[4];

			string_id TrainingText; // 0x6C0
			bool ShowHudTrainingText; // 0x6C4
			bool EnableTraining; // 0x6C5
			PAD16;
		}; BOOST_STATIC_ASSERT( sizeof(s_player_training_globals) == 1736 );
		s_player_training_globals* _PlayerTrainingGlobals();

		struct s_player_effect_globals : TStructImpl(688)
		{
		};
		s_player_effect_globals* _PlayerEffectGlobals();
	};
};