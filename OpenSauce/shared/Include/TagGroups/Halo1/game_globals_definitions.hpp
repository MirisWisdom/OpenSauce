/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <TagGroups/Halo1/TagGroupDefinitions.hpp>

namespace Yelo
{
	namespace Enums
	{
		enum unit_grenade_type : _enum
		{
			_unit_grenade_type_frag,
			_unit_grenade_type_plasma,

			// Stock number of grenade types
			k_unit_grenade_types_count,

			_unit_grenade_type_yelo2 = k_unit_grenade_types_count,	// ie, "custom 2"
			_unit_grenade_type_yelo3,								// ie, "custom 3"

			// Number of supported grenade types in OS-modified game states
			k_unit_grenade_types_count_yelo
		};
	};

	namespace TagGroups
	{
		struct s_game_globals_grenade
		{
			TAG_FIELD(int16, maximum_count);
			TAG_FIELD(int16, multiplayer_spawn_count); // PC only
			TAG_FIELD(tag_reference, throwing_effect, 'effe');
			TAG_FIELD(tag_reference, hud_interface, 'grhi');
			TAG_FIELD(tag_reference, equipment, 'eqip');
			TAG_FIELD(tag_reference, projectile, 'proj');
		}; BOOST_STATIC_ASSERT( sizeof(s_game_globals_grenade) == 0x44 );

		struct s_game_globals_tag_reference
		{
			tag_reference reference;
		};

		struct s_game_globals_multiplayer_information
		{
			TAG_FIELD(tag_reference, flag, 'item');
			TAG_FIELD(tag_reference, unit, 'unit');
			TAG_TBLOCK(vehicles, s_game_globals_tag_reference); // 20
			TAG_FIELD(tag_reference, hill_shader, 'shdr');
			TAG_FIELD(tag_reference, flag_shader, 'shdr');
			TAG_FIELD(tag_reference, ball, 'item');
			TAG_TBLOCK(sounds, s_game_globals_tag_reference); // 60
			TAG_PAD(int32, 14);
		}; BOOST_STATIC_ASSERT( sizeof(s_game_globals_multiplayer_information) == 0xA0 );

		struct s_game_globals
		{
			enum { k_group_tag = 'matg' };

			TAG_PAD(byte, 248);

			TAG_TBLOCK(sounds, tag_reference);
			TAG_TBLOCK(camera, tag_reference);

			TAG_PAD(tag_block,
				1 + // s_game_globals_player_control
				1); // s_game_globals_difficulty_information

			TAG_TBLOCK(grenades, s_game_globals_grenade);

			TAG_PAD(tag_block,
				1 + // s_game_globals_rasterizer_data
				1); // s_game_globals_interface_tag_references

			TAG_TBLOCK(weapons_list, s_game_globals_tag_reference);
			TAG_TBLOCK(cheat_powerups, s_game_globals_tag_reference);
			TAG_TBLOCK(multiplayer_info, s_game_globals_multiplayer_information);

			TAG_PAD(tag_block,
				1 + // s_game_globals_player_information
				1 + // s_game_globals_player_representation
				1 + // s_game_globals_falling_damage
				1 + // material_definition
				1); // playlist_autogenerate_choice
		}; BOOST_STATIC_ASSERT( sizeof(s_game_globals) == 0x1AC );
	};
};