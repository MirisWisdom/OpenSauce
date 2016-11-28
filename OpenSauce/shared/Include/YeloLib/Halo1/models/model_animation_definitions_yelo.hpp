/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <blamlib/Halo1/models/model_animations.hpp>
#include <blamlib/Halo1/models/model_animation_definitions.hpp>

namespace Yelo
{
	namespace Enums
	{
		enum {
			k_max_effect_references_per_graph = Enums::k_max_sound_references_per_graph_upgrade,
		};
		enum equipment_animation
		{
			k_number_of_equipment_animations,
		};

		enum projectile_animation
		{
			k_number_of_projectile_animations,
		};
	};

	namespace Animations
	{
#if PLATFORM_IS_EDITOR
		extern s_animation_list equipment_animation_list;
		extern s_animation_list projectile_animation_list;
#endif
	};

	namespace TagGroups
	{
		// The actual definition part of an OS-extended
		struct model_animation_graph_yelo_definition
		{
			TAG_FIELD(tag_reference, stock_animation, 'antr');
			TAG_PAD(tag_block, 13);
		}; BOOST_STATIC_ASSERT( sizeof(model_animation_graph_yelo_definition) == 0xAC );
		struct model_animation_graph_yelo
		{
			enum { k_group_tag = 'magy' };

			model_animation_graph stock_graph;
			model_animation_graph_yelo_definition yelo_graph;
		};
	};
};