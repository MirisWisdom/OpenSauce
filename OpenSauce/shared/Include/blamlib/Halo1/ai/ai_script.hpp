/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <blamlib/cseries/cseries_base.hpp>
#include <blamlib/Halo1/ai/encounters.hpp>
#include <blamlib/memory/datum_index.hpp>
#include <YeloLib/cseries/cseries_yelo_base.hpp>

namespace Yelo
{
	namespace AI
	{
		namespace e_ai_index_type
		{
			typedef enum : _enum
			{
				encounter,
				platoon,
				squad,

				k_count
			} type_t;
		}

		struct s_actor_datum;

		struct s_ai_index
		{
			int16 encounter_index;

			union
			{
				sbyte platoon_index;
				sbyte squad_index;
			};

			byte_enum : BIT_COUNT(byte) - bitfield_enum_size<e_ai_index_type::k_count>::value;
			byte_enum type : bitfield_enum_size<e_ai_index_type::k_count>::value;
		};

		BOOST_STATIC_ASSERT(sizeof(s_ai_index) == 4);

		struct s_ai_index_actor_iterator
		{
			// these are all absolute, not datum_index
			int32 encounter_index;
			int32 squad_index;
			int32 platoon_index;

			s_encounter_actor_iterator actor_iterator;

			void set_end_hack()
			{
				encounter_index = squad_index = platoon_index = NONE;
				actor_iterator.set_end_hack();
			}

			bool is_end_hack() const
			{
				// This should work because encounter_index is initialized to (ai_index & 0xFFFF),
				// meaning valid iterators will always have zeros in the upper 16-bits.
				// However, ai_index_actor_iterator_new does set encounter_index to NONE if the ai_index.type is invalid
				return encounter_index == NONE;
			}
		};

		BOOST_STATIC_ASSERT( sizeof(s_ai_index_actor_iterator) == 0x18 );
	}

	namespace blam
	{
		AI::s_ai_index_actor_iterator& PLATFORM_API ai_index_actor_iterator_new(
			const AI::s_ai_index index,
			AI::s_ai_index_actor_iterator& iterator);

		AI::s_actor_datum* PLATFORM_API ai_index_actor_iterator_next(
			AI::s_ai_index_actor_iterator& iterator);

		// Attaches the specified actor_variant to the unit
		void PLATFORM_API ai_scripting_attach_free(
			const datum_index unit_index,
			const datum_index actor_variant_definition_index);
	}
}
