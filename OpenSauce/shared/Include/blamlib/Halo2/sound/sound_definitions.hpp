/*
    Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

#include <blamlib/Halo2/tag_files/tag_groups.hpp>

namespace Yelo
{
	namespace TagGroups
	{
		struct sound_definition
		{
			enum { k_group_tag = 'snd!' };

		}; //BOOST_STATIC_ASSERT( sizeof(sound_definition) == 0x );


		struct s_sound_effect_template_collection
		{
			enum { k_group_tag = '<fx>' };

			TAG_BLOCK(template_collection, s_sound_effect_template_definition);
			TAG_FIELD(string_id, input_effect_name);
			TAG_BLOCK(additional_sound_inputs, s_sound_effect_template_sound_input_definition);
			TAG_BLOCK(platform_definition, s_platform_sound_effect_template_collection);
		}; BOOST_STATIC_ASSERT( sizeof(s_sound_effect_template_collection) == 0x28 );
	};
};