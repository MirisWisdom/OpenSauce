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

		//////////////////////////////////////////////////////////////////////////
		// Currently unused
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


		// ChokingVictim's globals
		struct project_yellow_globals_cv
		{
#if !PLATFORM_IS_EDITOR
			const 
#endif
				int16 version;	enum { k_version = 1, k_group_tag = 'gelc' };
			struct _flags {
				TAG_FLAG16(allow_unit_infections_during_cinematics);
			}flags; BOOST_STATIC_ASSERT( sizeof(_flags) == sizeof(word_flags) );

			TAG_TBLOCK(unit_infections, s_unit_infections_definition); // 1
			//TAG_TBLOCK(dual_wielding, s_dual_wield_weapon_definition); // 256
			//TAG_TBLOCK(object_damage_extensions, s_object_damage_extension); // 512

			TAG_PAD(int32, 36);
		};
	};
};