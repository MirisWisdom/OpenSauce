/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once // should actually only ever be included by hs_runtime.cpp...

#include <blamlib/Halo1/ai/ai_script.hpp>
#include <blamlib/Halo1/ai/actor_structures.hpp>
#include <blamlib/Halo1/hs/hs_structures.hpp>
#include <blamlib/Halo1/hs/object_lists.hpp>
#include <blamlib/Halo1/objects/objects.hpp>
#include <blamlib/Halo1/units/unit_structures.hpp>

namespace Yelo
{
	namespace blam
	{
		// NOTE: procs are API_FUNC, not PLATFORM_API, as they should only be called by our re-written code, not the engine

		static TypeHolder hs_value_to_void(TypeHolder value)
		{
			return k_null_as_type_holder;
		}

		// TODO: use lexical cast http://www.boost.org/doc/libs/1_55_0/doc/html/boost_lexical_cast.html

		//////////////////////////////////////////////////////////////////////////
		// _hs_type_bool. nonstandard, the engine never defined any of theses.
		static TypeHolder hs_bool_to_real(TypeHolder value)
		{
			value.real = value.boolean ? 1.0f : 0.0f;

			return value;
		}
		static TypeHolder hs_bool_to_short(TypeHolder value)
		{
			value.int16 = value.boolean ? TRUE : FALSE;

			return value;
		}
		static TypeHolder hs_bool_to_long(TypeHolder value)
		{
			value.int32 = value.boolean ? TRUE : FALSE;

			return value;
		}
		// NOTE: non-standard, engine doesn't support this
		static TypeHolder hs_bool_to_string(TypeHolder value)
		{
			// TODO: verify we can safely do this, as strings are usually allocated
			// in the scenario's hs string data
			value.ptr.character = value.boolean ? "true" : "false";

			return value;
		}

		//////////////////////////////////////////////////////////////////////////
		// _hs_type_real
		static TypeHolder hs_real_to_short(TypeHolder value)
		{
			value.int16 = CAST(int16, value.real);

			return value;
		}
		static TypeHolder hs_real_to_long(TypeHolder value)
		{
			value.int32 = CAST(int32, value.real);

			return value;
		}
		// NOTE: non-standard, engine doesn't support this
		static TypeHolder hs_real_to_enum(TypeHolder value)
		{
			// because hs_enum_to_real adds 1 before float conversion
			value.int16 = CAST(int16, value.real) - 1;

			return value;
		}
		// NOTE: non-standard, engine doesn't support this
		static TypeHolder hs_real_to_string(TypeHolder value)
		{
			// TODO: verify we can safely do this, as strings are usually allocated
			// in the scenario's hs string data
			static char text[32];
			value.ptr.character = text;

			if (-1 == sprintf_s(text, "%f", value.real))
				value.ptr.character = BOOST_PP_STRINGIZE(NONE);

			return value;
		}

		//////////////////////////////////////////////////////////////////////////
		// _hs_type_short
		static TypeHolder hs_short_to_boolean(TypeHolder value)
		{
			value.boolean = value.int16 != 0;

			return value;
		}
		static TypeHolder hs_short_to_real(TypeHolder value)
		{
			value.real = CAST(real, value.int16);

			return value;
		}
		static TypeHolder hs_short_to_long(TypeHolder value)
		{
			value.int32 = CAST(int32, value.int16);

			return value;
		}
		// NOTE: non-standard, engine doesn't support this
		static TypeHolder hs_short_to_string(TypeHolder value)
		{
			// TODO: verify we can safely do this, as strings are usually allocated
			// in the scenario's hs string data
			static char text[32];
			value.ptr.character = text;

			if (-1 == sprintf_s(text, "%d", CAST(int32,value.int16)))
				value.ptr.character = BOOST_PP_STRINGIZE(NONE);

			return value;
		}

		//////////////////////////////////////////////////////////////////////////
		// _hs_type_long
		static TypeHolder hs_long_to_boolean(TypeHolder value)
		{
			value.boolean = value.int32 != 0;

			return value;
		}
		static TypeHolder hs_long_to_real(TypeHolder value)
		{
			value.real = CAST(real, value.int32);

			return value;
		}
		static TypeHolder hs_long_to_short(TypeHolder value)
		{
			value.int16 = CAST(int16, value.int32);

			return value;
		}
		// NOTE: non-standard, engine doesn't support this
		static TypeHolder hs_long_to_string(TypeHolder value)
		{
			// TODO: verify we can safely do this, as strings are usually allocated
			// in the scenario's hs string data
			static char text[32];
			value.ptr.character = text;

			if (-1 == sprintf_s(text, "%d", value.int32))
				value.ptr.character = BOOST_PP_STRINGIZE(NONE);

			return value;
		}

		//////////////////////////////////////////////////////////////////////////
		// _hs_type_string
		static TypeHolder hs_string_to_boolean(TypeHolder value)
		{
			value.int32 = strlen(value.ptr.character);

			return hs_long_to_boolean(value);
		}

		//////////////////////////////////////////////////////////////////////////
		// enums
		static TypeHolder hs_enum_to_real(TypeHolder value)
		{
			// yes, the engine adds 1 before converting to a float
			value.real = CAST(real, value.int16 + 1);

			return value;
		}


		// actually defined in ai_script.c
		static TypeHolder object_list_from_ai_reference(TypeHolder value)
		{
			if (value.int32 == NONE)
			{
				value.datum = datum_index::null;
				return value;
			}

			datum_index list_index = object_list_new();
			const auto ai_reference = *CAST_PTR(AI::s_ai_index*, &value.pointer);

			AI::s_ai_index_actor_iterator iterator;
			ai_index_actor_iterator_new(ai_reference, iterator);
			while (auto* actor = ai_index_actor_iterator_next(iterator))
			{
				datum_index unit_index = *actor->GetMetaUnitIndex();
				if (!unit_index.IsNull())
				{
					object_list_add(list_index, unit_index);
				}

				// add all the units that make up this actor's swarm
				for (datum_index swarm_unit_index = *actor->GetMetaSwarmUnitIndex(); !swarm_unit_index.IsNull(); )
				{
					auto* unit = object_get_and_verify_type<s_unit_datum>(swarm_unit_index);
					object_list_add(list_index, swarm_unit_index);

					swarm_unit_index = unit->unit.swarm_next_unit_index;
				}
			}

			value.datum = list_index;
			return value;
		}

		static TypeHolder hs_object_to_object_list(TypeHolder value)
		{
			if (value.datum.IsNull())
			{
				value.datum = datum_index::null;
				return value;
			}

			datum_index list_index = object_list_new();
			object_list_add(list_index, value.datum);

			value.datum = list_index;
			return value;
		}
		static TypeHolder hs_object_name_to_object_list(TypeHolder value)
		{
			datum_index object_index = object_index_from_name_index(value.int16);
			value.datum = object_index;

			return hs_object_to_object_list(value);
		}
	};
};