/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#include "Common/Precompile.hpp"
#include "Engine/AnimationUpgrade.hpp"

#include "TagGroups/TagGroups.hpp"

#include <blamlib/Halo1/models/model_animations.hpp>
#include <blamlib/Halo1/models/model_animation_definitions.hpp>

namespace Yelo
{
	namespace Animation
	{
		ENGINE_PTR(Objects::s_animation_list, UNIT_SEAT_ANIMATION_BLOCK_NAME_TABLE,	0x9D79F0, 0x6BB6D8, 0xA4F1C8);
		ENGINE_PTR(int32, unit_seat_animation_block_name_table_count,				0x9D7AE0, 0x6BB7C8, 0xA4F2B8);
		ENGINE_PTR(void**, unit_seat_animation_block_name_table_ptr,				0x9D7AE4, 0x6BB7CC, 0xA4F2BC);
		ENGINE_PTR(int32, unit_seat_animation_block_maxcount,						0x9D7140, 0x6BAE28, 0xA4E918);

		static Objects::s_animation_list unit_seat_animation_block_name_table[Enums::_unit_seat_animation];

		void AnimationUpgradeInitialize()
		{
			// copy the existing block name table
			memcpy_s(unit_seat_animation_block_name_table, 
				sizeof(Objects::s_animation_list) * NUMBEROF(unit_seat_animation_block_name_table), 
				GET_PTR2(UNIT_SEAT_ANIMATION_BLOCK_NAME_TABLE), 
				sizeof(Objects::s_animation_list) * GET_PTR(unit_seat_animation_block_name_table_count)
			);

			// set the block name table count to our's
			GET_PTR(unit_seat_animation_block_name_table_count) = Enums::_unit_seat_animation;
			// set the block maxcount to our's
			GET_PTR(unit_seat_animation_block_maxcount) = Enums::_unit_seat_animation;
			// set the block name table address to our's
			GET_PTR(unit_seat_animation_block_name_table_ptr) = (void**)unit_seat_animation_block_name_table;

			////////////////////////////////////
			// new animation block names
			unit_seat_animation_block_name_table[Enums::_unit_seat_animation_yelo_board].name = "board";
			unit_seat_animation_block_name_table[Enums::_unit_seat_animation_yelo_board].type = Enums::model_animation_type::_model_animation_type_base;
			unit_seat_animation_block_name_table[Enums::_unit_seat_animation_yelo_ejection].name = "ejection";
			unit_seat_animation_block_name_table[Enums::_unit_seat_animation_yelo_ejection].type = Enums::model_animation_type::_model_animation_type_base;
		}
		
		void AnimationUpgradeDispose()
		{
		}
	};
};