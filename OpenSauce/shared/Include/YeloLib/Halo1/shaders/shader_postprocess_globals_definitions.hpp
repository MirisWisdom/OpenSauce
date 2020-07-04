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
		struct s_shader_postprocess_bloom_definition
		{
			TAG_FIELD(real,							size);
			TAG_FIELD(real,							exposure);
			TAG_FIELD(real_fraction,				mix_amount);

			TAG_FIELD(real_rgb_color,				minimum_color);
			TAG_FIELD(real_rgb_color,				maximum_color);
		};

		struct s_shader_postprocess_globals_bloom
		{
			struct _flags {
				TAG_FLAG16(is_enabled);
				TAG_FLAG16(apply_after_hud);
			}flags;
			PAD16;

			s_shader_postprocess_bloom_definition bloom;
		}; BOOST_STATIC_ASSERT( sizeof(s_shader_postprocess_globals_bloom) == 0x28 );

		struct s_shader_postprocess_globals
		{
			enum { k_group_tag = 'sppg' };

			PAD16;
			PAD16;
			union {
				TAG_PAD(tag_block, 20); // Up to to 20 blocks for subsystem globals

				struct {
					TAG_TBLOCK_(bloom_globals, s_shader_postprocess_globals_bloom);
				};
			};
		}; BOOST_STATIC_ASSERT( sizeof(s_shader_postprocess_globals) == 0xF4 );
	};
};