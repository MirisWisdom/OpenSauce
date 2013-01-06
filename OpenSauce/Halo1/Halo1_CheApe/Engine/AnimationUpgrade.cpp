/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#include "Common/Precompile.hpp"
#include "Engine/AnimationUpgrade.hpp"

#include "TagGroups/TagGroups.hpp"

namespace Yelo
{
	namespace Animation
	{
		FUNC_PTR(UNIT_SEAT_ANIMATION_BLOCK_NAME_TABLE,					0x9D79F0, 0x6BB6D8, 0xA4F1C8);
		ENGINE_PTR(int32, unit_seat_animation_block_name_table_count,	0x9D7AE0, 0x6BB7C8, 0xA4F2B8);
		ENGINE_PTR(void**, unit_seat_animation_block_name_table_ptr,	0x9D7AE4, 0x6BB7CC, 0xA4F2BC);

		static struct s_animation_list { cstring name; _enum type; PAD16; };

		static const int32 k_unit_seat_animation_block_count = 30;	// original unit_seat_animation_block maxcount
		static const int32 k_unit_seat_animation_block_count_new = 2;	// number of animation states we've added
		static s_animation_list unit_seat_animation_block_name_table[k_unit_seat_animation_block_count+k_unit_seat_animation_block_count_new];

		// Note: any new animation states added will require an increase in the 
		// unit_seat_animation_block maxcount (check project_yellow_includes.xml)
		void AnimationUpgradeInitialize()
		{
			// copy the existing block name table
			for (int x = 0; x < k_unit_seat_animation_block_count; x++)
			{
				unit_seat_animation_block_name_table[x].name = *(cstring*)(GET_FUNC_PTR(UNIT_SEAT_ANIMATION_BLOCK_NAME_TABLE) + (sizeof(s_animation_list) * x));
				unit_seat_animation_block_name_table[x].type = *(int16*)(GET_FUNC_PTR(UNIT_SEAT_ANIMATION_BLOCK_NAME_TABLE + sizeof(cstring*)) + (sizeof(s_animation_list) * x));
			}
			
			// set the block name table count to our's
			GET_PTR(unit_seat_animation_block_name_table_count) = k_unit_seat_animation_block_count+k_unit_seat_animation_block_count_new;
			// set the block name table address to our's
			GET_PTR(unit_seat_animation_block_name_table_ptr) = (void**)unit_seat_animation_block_name_table;

			////////////////////////////////////
			// new animation block names
			unit_seat_animation_block_name_table[k_unit_seat_animation_block_count].name = (cstring)"board";
			unit_seat_animation_block_name_table[k_unit_seat_animation_block_count].type = 0;
			unit_seat_animation_block_name_table[k_unit_seat_animation_block_count+1].name = (cstring)"eject";
			unit_seat_animation_block_name_table[k_unit_seat_animation_block_count+1].type = 0;
		}
		
		void AnimationUpgradeDispose()
		{
		}
	};
};