/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

namespace Yelo
{
	namespace Enums
	{
		enum {
			_ai_index_type_encounter,
			_ai_index_type_platoon,
			_ai_index_type_squad,

			k_number_of_ai_index_types,
		};
	};

	namespace AI
	{
		struct ai_index
		{
			int16 encounter_index;
			union {
				sbyte platoon_index;
				sbyte squad_index;
			};
			byte_enum : BIT_COUNT(byte) - bitfield_enum_size<Enums::k_number_of_ai_index_types>::value;
			byte_enum type : bitfield_enum_size<Enums::k_number_of_ai_index_types>::value;
		}; BOOST_STATIC_ASSERT(sizeof(ai_index) == 4);
	};

	namespace blam
	{
		// Attaches the specified actor_variant to the unit
		void PLATFORM_API ai_scripting_attach_free(datum_index unit_index, datum_index actor_variant_definition_index);
	};
};