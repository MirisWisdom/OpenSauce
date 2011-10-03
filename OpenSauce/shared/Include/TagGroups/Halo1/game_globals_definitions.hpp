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
	};

	namespace TagGroups
	{
		struct s_game_globals_tag_reference
		{
			tag_reference reference;
		};
		struct s_game_globals
		{
			byte _unused[248];

			TAG_TBLOCK(sounds, tag_reference);
			TAG_TBLOCK(camera, tag_reference);

			TAG_PAD(tag_block,
				1 + // s_game_globals_player_control
				1 + // s_game_globals_difficulty_information
				1 + // s_game_globals_grenade
				1 + // s_game_globals_rasterizer_data
				1); // s_game_globals_interface_tag_references

			TAG_TBLOCK(weapons_list, s_game_globals_tag_reference);
			TAG_TBLOCK(cheat_powerups, s_game_globals_tag_reference);

			TAG_PAD(tag_block,
				1 + // s_game_globals_multiplayer_information
				1 + // s_game_globals_player_information
				1 + // s_game_globals_player_representation
				1 + // s_game_globals_falling_damage
				1 + // material_definition
				1); // playlist_autogenerate_choice
		}; BOOST_STATIC_ASSERT( sizeof(s_game_globals) == 0x1AC );
	};
};