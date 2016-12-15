/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <YeloLib/tag_files/tag_groups_base_yelo.hpp>
#include <YeloLib/Halo1/shaders/shader_postprocess_definitions.hpp>

namespace Yelo
{
	namespace TagGroups
	{
		struct s_effect_postprocess_collection_script_variable
		{
			TAG_FIELD(tag_string, script_variable_name);
			TAG_FIELD(tag_string, exposed_parameter_name);

			struct
			{
				void* exposed_parameter; //void* to avoid a problematic include

				TAG_PAD(byte, 12);
			}runtime;
		}; BOOST_STATIC_ASSERT( sizeof(s_effect_postprocess_collection_script_variable) == 0x50);

		struct s_effect_postprocess_collection_effect
		{
			TAG_FIELD(tag_reference, effect);
			TAG_FIELD(tag_string, name);
			TAG_TBLOCK_(script_variables, s_effect_postprocess_collection_script_variable);
			TAG_PAD(byte, 12);
		}; BOOST_STATIC_ASSERT( sizeof(s_effect_postprocess_collection_effect) == 0x48);

		struct s_effect_postprocess_collection
		{
			enum { k_group_tag = 'efpc' };

			TAG_PAD(byte, 12);
			TAG_TBLOCK_(shaders, tag_reference);
			TAG_TBLOCK_(effects, s_effect_postprocess_collection_effect);
			TAG_TBLOCK_(effect_instances, s_effect_postprocess_generic_effect_instance);
		}; BOOST_STATIC_ASSERT( sizeof(s_effect_postprocess_collection) == 0x30);
	};
};