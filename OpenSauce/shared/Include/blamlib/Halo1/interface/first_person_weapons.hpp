/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <blamlib/Halo1/game/game_configuration.hpp>
#include <blamlib/Halo1/models/model_animations.hpp>

namespace Yelo
{
	namespace GameUI
	{
		struct s_first_person_weapon : TStructImpl(0x1EA0)
		{
			TStructGetPtrImpl(bool, IsVisible, 0x0);
			TStructGetPtrImpl(datum_index, UnitIndex, 0x4);
			TStructGetPtrImpl(datum_index, WeaponIndex, 0x8);

			////////////////////////////////////////////////
			// this enum is used for setting the one below
			// 0  - fire 1
			// 1  - fire 2
			// 2  - misfire 1
			// 3  - misfire 2
			// 4  - melee
			// 5  - light off
			// 6  - light on
			// 7  - (unused ?)
			// 8  - (unused ?)
			// 9  - reload empty
			// 10 - reload full
			// 11 - put away
			// 12 - ready
			// 13 -  (sets weapon index to NONE) ?
			// 14 - overcharged
			// 15 - overheating
			// 16 - overheat exit
			// 17 - throw grenade

			////////////////////////////////////////////////
			// animation state
			// 0  - idle
			// 1  - overheating
			// 2  - overheating again
			// 3  - overheated
			// 4  - overcharged
			// 5  - posing
			// 6  - fire 1
			// 7  - fire 2
			// 8  - misfire 1
			// 9  - misfire 2
			// 10 - melee
			// 11 - light off
			// 12 - light on
			// 13 - reload empty
			// 14 - reload full
			// 15 - enter
			// 16 - exit empty
			// 17 - exit full
			// 18 - put away
			// 19 - ready
			// 20 - throw grenade
			// 21 - throw overheated
			// 22 - overheat enter
			// 23 - overheat exit
			TStructGetPtrImpl(_enum, AnimationState, 0xC);
			// 0xE = unknown 2 bytes
			TStructGetPtrImpl(game_time_t, IdleTime, 0x10);
			// TStructGetPtrImpl(Objects::s_animation_state, CurrentBaseAnimation, 0x12); // frame index of current base animation (idle or moving) --- don't think this is correct
			// 0x14 = overlay animation index
			TStructGetPtrImpl(Objects::s_animation_state, CurrentAnimation, 0x16);
			TStructGetPtrImpl(Objects::s_animation_state, CurrentMovingOverlayAnimation, 0x1A);
			//TStructGetPtrImpl(Objects::s_animation_state, , 0x1E);

			//TStructGetPtrImpl(real, , 0x28); // frame index (real) for the above anim state
			//TStructGetPtrImpl(real, , 0x2C); // adds 0.05f when entering firing state ?
			// 0x30?
			//TStructGetPtrImpl(real, , 0x34);
			// 0x38?
			// 0x3C?
			//TStructGetPtrImpl(real, , 0x40);
			//TStructGetPtrImpl(real, , 0x44);
			// 0x48?
			// 0x4C?
			// 0x50 bool
			// 0x54 real_vector3d forward?
			// 0x60 real_euler_angles2d
			// 0x68 real_euler_angles2d
			// 0x70 real_point3d
			// 0x7C real_point3d?
			//////////////////////////////////////////////////////////////////////////
			// 0x88 int16 frame index
			// 0x8A int16 frame count
			// 0x8C real_orientation3d node orientations[128]
			// 0x108C real_matrix4x3 node matrices[64]
			//////////////////////////////////////////////////////////////////////////
			// 0x1D8C bool remapped weapon?
			// PAD8;
			// 0x1D8E int16 node_remappings[64]
			//////////////////////////////////////////////////////////////////////////
			// 0x1E0E bool remapped hands
			// PAD8;
			// 0x1E10 int16 node_remappings[64]
			//////////////////////////////////////////////////////////////////////////
			// 0x1E90 bool
			// 0x1E94 _enum shotgun_reload_type;
			//		_shotgun_reload_type_last_round = 1
			//		_shotgun_reload_type_first_and_last_round = 2
			TStructGetPtrImpl(datum_index, AnimationSoundIndex, 0x1E98);
			TStructGetPtrImpl(_enum, AnimationStateDuringSound, 0x1E9C);
		};
		struct s_first_person_weapons
		{
			s_first_person_weapon local_players[Enums::k_maximum_number_of_local_players];
		}; BOOST_STATIC_ASSERT( sizeof(s_first_person_weapons) == 0x1EA0 );
		s_first_person_weapons*		FirstPersonWeapons();
	};

	namespace blam
	{
		void PLATFORM_API first_person_weapons_initialize_for_new_map();
	};
};