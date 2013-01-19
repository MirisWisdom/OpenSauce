/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <YeloLib/Halo1/open_sauce/project_yellow_global_cv_definitions.hpp>

#include <blamlib/Halo1/tag_files/tag_groups.hpp>

namespace Yelo
{
	namespace TagGroups
	{
#if FALSE
		/* !-- Preprocessing --! */
		struct s_yelo_preprocess_maplist_entry
		{
			TAG_FIELD(tag_string, name);
			TAG_FIELD(tag_reference, scenario, "scnr");
			TAG_PAD(int32, 6); // 24
		};
		struct s_yelo_preprocess_definition
		{
			struct _flags {
				TAG_FLAG(unused, "");
			}flags;	BOOST_STATIC_ASSERT( sizeof(_flags) == sizeof(long_flags) );

			struct {
				TAG_TBLOCK(campaign, s_yelo_preprocess_maplist_entry);		// 24
				TAG_TBLOCK(multiplayer, s_yelo_preprocess_maplist_entry);	// 32
			}maplist;

			TAG_PAD(int32, 20); // 80
		};
		/* !-- Preprocessing --! */
#endif


		/* !-- UI --! */
		struct s_project_yellow_scripted_ui_widget
		{
			TAG_FIELD(tag_string, name);
			TAG_FIELD(tag_reference, definition, 'DeLa');

			PAD32; // future flags
			TAG_PAD(tag_block, 2);
		};
		/* !-- UI --! */


		/* !-- Netgame --! */
		struct s_network_game_player_unit
		{
			TAG_FIELD(tag_string, name, "", "name of this block definition");
			TAG_FIELD(tag_reference, definition, "unit", "unit definition for this player's biped");
			TAG_PAD(int32, 8); // 32
		};
		/* !-- Netgame --! */


		/* !-- Scripting --! */
		struct s_script_construct_definition
		{
			TAG_FIELD(tag_string, name[2]); // So we can have at most 62 characters for the name
			TAG_FIELD(int16, index);
		};
		struct s_script_function_definition : public s_script_construct_definition
		{
			TAG_ENUM(return_type, Enums::hs_type);
			TAG_TBLOCK(parameters, _enum); // 32
		};
		struct s_script_global_definition : public s_script_construct_definition
		{
			TAG_ENUM(type, Enums::hs_type);
		};
		struct s_scripting_definitions
		{
			TAG_TBLOCK(new_functions, s_script_function_definition);
			TAG_TBLOCK(new_globals, s_script_global_definition);
		};
		/* !-- Scripting --! */


		// yelo for globals
		struct project_yellow_globals
		{
			// internal name of the cache's Yelo Globals when the user doesn't supply a definition
			static cstring k_default_name;

#if !PLATFORM_IS_EDITOR
			const 
#endif
				int16 version;	enum { k_version = 2, k_group_tag = 'gelo' };
			struct _flags {
				TAG_FLAG16(hide_health_when_zoomed);
				TAG_FLAG16(hide_shield_when_zoomed);
				TAG_FLAG16(hide_motion_sensor_when_zoomed);
				TAG_FLAG16(force_game_to_use_stun_jumping_penalty);
				TAG_FLAG16(allow_grenade_chain_reactions_in_mp);
			}flags; BOOST_STATIC_ASSERT( sizeof(_flags) == sizeof(word_flags) );
			TAG_FIELD(uint32, base_address);

			TAG_FIELD(tag_string, mod_name, "", "name of the engine 'mod' these globals and, inheriting scenario, are for");

			TAG_FIELD(tag_reference, explicit_references, 'tagc');
			TAG_FIELD(tag_reference, cv_globals, 'gelc');
			TAG_PAD(int32, 4);

#if FALSE	// TODO: once we're sure it's safe to remove this, move the padding below into the padding field above
			/* !-- Preprocessing --! */
			// PREPROCESSING DOESN'T EXIST AT RUNTIME. Only for the tools
			TAG_TBLOCK(preprocess, s_yelo_preprocess_definition); // 1
			/* !-- Preprocessing --! */
#else
			TAG_PAD(tag_block, 1);
#endif


			/* !-- UI --! */
			struct {
				TAG_PAD(tag_reference, 3); // 48
				PAD32;
				TAG_TBLOCK(scripted_widgets, s_project_yellow_scripted_ui_widget); // 128
			}ui;
			/* !-- UI --! */


			/* !-- Netgame --! */
			struct {
				TAG_TBLOCK(player_units, s_network_game_player_unit); // 32

				TAG_PAD(int32, 5); // 20
			}networking;
			/* !-- Netgame --! */


			/* !-- Scripting --! */
			TAG_TBLOCK(yelo_scripting, s_scripting_definitions); // 1
			/* !-- Scripting --! */


			TAG_PAD(int32, 20); // 80

#if PLATFORM_IS_EDITOR
		private:
			void CullInvalidNetworkPlayerUnits();
		public:
			void Cull();
#endif
		public:
			// Does the tag definition have a project_yellow_globals_cv reference?
			bool HasCvGlobals() const;
		};
	};
};