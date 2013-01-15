/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

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


		struct s_dual_wield_weapon_definition
		{
			TAG_FIELD(tag_reference, primary_weapon, 'weap');
			TAG_FIELD(tag_reference, secondary_weapon, 'weap');
			TAG_FIELD(tag_reference, combined_weapon, 'weap');

			TAG_PAD(int32, 8);
		}; BOOST_STATIC_ASSERT( sizeof(s_dual_wield_weapon_definition) == 0x50 );

		//////////////////////////////////////////////////////////////////////////
		// Currently unused
		struct s_object_damage_region_permutation_extension
		{
			TAG_FIELD(tag_string, name);
			TAG_FIELD(real, damage_threshold);

			TAG_FIELD(tag_string, effect_marker_name);
			TAG_FIELD(tag_reference, transition_effect, 'effe');
			TAG_PAD(int32, 8);

			TAG_FIELD(tag_string, damage_effect_marker_name);
			TAG_FIELD(tag_reference, transition_damage_effect, 'jpt!');
			TAG_PAD(int32, 8);
		}; BOOST_STATIC_ASSERT( sizeof(s_object_damage_region_permutation_extension) == 0xC4 );
		struct s_object_damage_region_extension
		{
			TAG_FIELD(tag_string, name);
			TAG_TBLOCK(permutations, s_object_damage_region_permutation_extension); // 32
		}; BOOST_STATIC_ASSERT( sizeof(s_object_damage_region_extension) == 0x2C );
		struct s_object_damage_extension
		{
			TAG_FIELD(tag_reference, object, 'obje');
			TAG_TBLOCK(regions, s_object_damage_region_extension); // 32
		}; BOOST_STATIC_ASSERT( sizeof(s_object_damage_extension) == 0x1C );
		//////////////////////////////////////////////////////////////////////////

		struct s_unit_infection
		{
			PAD16;
			TAG_FIELD(int16, infection_unit); // block index to a infection_unit
			TAG_FIELD(tag_reference, unit, 'unit', "unit to be infected");
			TAG_FIELD(real, health_threshold, "when our health reaches this threshold after a bj from an infection form, we'll be infected");

			TAG_FIELD(tag_reference, infected_unit, 'unit');
			TAG_FIELD(tag_reference, infected_unit_actor_variant, 'actv');
			TAG_FIELD(tag_reference, transition_effect, 'effe');

			TAG_FIELD(tag_reference, attachment_object, 'obje');
			TAG_FIELD(tag_string, attachment_marker);
			TAG_FIELD(int16, attachment_marker_count);
			PAD16;

			// TODO: it would be cool to implement randomization for multiple different infection forms (like how Halo3 had different pure forms)

			TAG_PAD(int32, 5);
		}; BOOST_STATIC_ASSERT( sizeof(s_unit_infection) == 0x90 );
		struct s_unit_infections_definition
		{
			PAD32;
			TAG_TBLOCK(infection_units, tag_reference); // 8, aka unit_infection_form_block
			TAG_TBLOCK(infectable_units, s_unit_infection); // 64
			TAG_PAD(int32, 6);

		private:
			int32 FindInfectableUnitIndex(int32 infection_unit_index, datum_index infectable_unit_definition_index) const;
			int32 FindInfectionUnitIndex(datum_index unit_definition_index) const;
		public:
			int32 LookupUnitInfectionIndex(datum_index infection_unit_definition_index, datum_index target_unit_definition_index) const;
		};
		
		struct s_unit_boarding_seat
		{
			struct __flags
			{
				TAG_FLAG16(boarding_ejects_target_seat);
				TAG_FLAG16(boarding_enters_target_seat);
				TAG_FLAG16(controls_open_and_close);
			}; BOOST_STATIC_ASSERT( sizeof(__flags) == sizeof(word_flags) );

			TAG_FIELD(__flags, flags);
			PAD16;
			TAG_FIELD(int16, seat_index);
			TAG_FIELD(int16, target_seat_index);
			TAG_FIELD(tag_string, seat_label);
			TAG_FIELD(tag_string, target_seat_label);
			TAG_FIELD(tag_reference, boarding_damage, "jpt!");
			TAG_PAD(int32, 8);
		}; BOOST_STATIC_ASSERT( sizeof(s_unit_boarding_seat) == 0x78 ); // max count: 32

		struct s_unit_external_upgrades
		{
			TAG_FIELD(tag_reference, unit, "unit");
			TAG_TBLOCK(boarding_seats, s_unit_boarding_seat);
			TAG_PAD(int32, 9);
		}; BOOST_STATIC_ASSERT( sizeof(s_unit_external_upgrades) == 0x40 ); // max count: 64


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
				TAG_FLAG16(allow_unit_infections_during_cinematics);
				TAG_FLAG16(allow_grenade_chain_reactions_in_mp);
				TAG_FLAG16(force_game_to_use_stun_jumping_penalty);
			}flags; BOOST_STATIC_ASSERT( sizeof(_flags) == sizeof(word_flags) );
			TAG_FIELD(uint32, base_address);

			TAG_FIELD(tag_string, mod_name, "", "name of the engine 'mod' these globals and, inheriting scenario, are for");

			TAG_FIELD(tag_reference, explicit_references, 'tagc');
			TAG_PAD(int32, 8);

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


			/* !-- Misc --! */
			TAG_TBLOCK(unit_infections, s_unit_infections_definition);	// 1
			TAG_TBLOCK(unit_external_upgrades, s_unit_external_upgrades); // 64
			//TAG_TBLOCK(dual_wielding, s_dual_wield_weapon_definition); // 256
			//TAG_TBLOCK(object_damage_extensions, s_object_damage_extension); // 512
			/* !-- Misc --! */
			
			TAG_PAD(int32, 14);

#if PLATFORM_IS_EDITOR
		private:
			void CullInvalidNetworkPlayerUnits();
		public:
			void Cull();
#endif
		};
	};
};