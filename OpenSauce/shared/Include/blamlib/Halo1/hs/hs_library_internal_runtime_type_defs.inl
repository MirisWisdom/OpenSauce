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

		typedef void (API_FUNC* hs_type_inspector_proc)(Enums::hs_type type, TypeHolder value,
			char buffer[Enums::k_hs_inspect_buffer_size]);

		typedef TypeHolder (API_FUNC* hs_typecast_proc)(TypeHolder value);

		static TypeHolder hs_value_to_void(TypeHolder value)
		{
			value.pointer = nullptr;
			return value;
		}

		//////////////////////////////////////////////////////////////////////////
		// _hs_type_bool
		
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

			AI::s_ai_index_actor_iterator iterator;
			ai_index_actor_iterator_new(*CAST_PTR(AI::s_ai_index*, &value.pointer), iterator);
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

// TODO: THE UGLY MESS BELOW IS JUST A HACK FOR NOW!
// Until we move to a type system which supports hs_type definitions (a la Halo4+).
// The table is defined\laid out just as how it appears in the engine code

#define HS_TYPE_COUNT				49
#define HS_TYPE_COUNT_NONDATA		5
#define HS_TYPE_COUNT_ENUMS			5
#define HS_TYPE_COUNT_OBJECTS		6
#define HS_TYPE_COUNT_DATA			BOOST_PP_SUB(HS_TYPE_COUNT, HS_TYPE_COUNT_NONDATA)
// number of hs_types, relative to 'X', another hs_type
#define HS_TYPE_COUNT_DATA_X(starting_hs_type) BOOST_PP_SUB(HS_TYPE_COUNT, starting_hs_type)

		BOOST_STATIC_ASSERT(HS_TYPE_COUNT == Enums::k_number_of_hs_types);
		BOOST_STATIC_ASSERT(HS_TYPE_COUNT_NONDATA == (Enums::_hs_type_void+1));
		BOOST_STATIC_ASSERT(HS_TYPE_COUNT_ENUMS == (Enums::_hs_type_enum__last-Enums::_hs_type_enum__first)+1);
		BOOST_STATIC_ASSERT(HS_TYPE_COUNT_OBJECTS == (Enums::_hs_type_object__last-Enums::_hs_type_object__first)+1);

#define TYPECAST_PROC_ENUM_MACRO(z, n, proc_name) proc_name

#define TYPECAST_PROCS_FOR_NONDATA()			\
	BOOST_PP_ENUM(HS_TYPE_COUNT_NONDATA, TYPECAST_PROC_ENUM_MACRO, nullptr)
#define TYPECAST_PROCS_FOR_NONDATA_TYPE()		\
	TYPECAST_PROCS_FOR_NONDATA() ,				\
	BOOST_PP_ENUM(HS_TYPE_COUNT_DATA, TYPECAST_PROC_ENUM_MACRO, nullptr)
#define TYPECAST_PROCS_FOR_NONCASTABLE_TYPE()	\
	TYPECAST_PROCS_FOR_NONDATA_TYPE()

static const hs_typecast_proc k_hs_typecasting_procedures[Enums::k_number_of_hs_types][Enums::k_number_of_hs_types] = {
	{	TYPECAST_PROCS_FOR_NONDATA_TYPE() },	// _hs_unparsed
	{	TYPECAST_PROCS_FOR_NONDATA_TYPE() },	// _hs_special_form
	{	TYPECAST_PROCS_FOR_NONDATA_TYPE() },	// _hs_function_name
	{	TYPECAST_PROCS_FOR_NONDATA_TYPE() },	// _hs_passthrough

	{	TYPECAST_PROCS_FOR_NONDATA(),			// _hs_type_void
		BOOST_PP_ENUM(HS_TYPE_COUNT_DATA, TYPECAST_PROC_ENUM_MACRO, nullptr)
		},

	{	TYPECAST_PROCS_FOR_NONDATA(),			// _hs_type_bool
		nullptr,				// from _hs_type_bool
		hs_long_to_boolean,		// from _hs_type_real (this is what the engine does)
		hs_short_to_boolean,	// from _hs_type_short
		hs_long_to_boolean,		// from _hs_type_long
		hs_string_to_boolean,	// from _hs_type_string
		BOOST_PP_ENUM(HS_TYPE_COUNT_DATA_X(10), TYPECAST_PROC_ENUM_MACRO, nullptr) // from _hs_type_script...
		},

	{	TYPECAST_PROCS_FOR_NONDATA(),			// _hs_type_real
		nullptr,				// from _hs_type_bool
		nullptr,				// from _hs_type_real
		hs_short_to_real,		// from _hs_type_short
		hs_long_to_real,		// from _hs_type_long
		BOOST_PP_ENUM(BOOST_PP_SUB(32, 9), TYPECAST_PROC_ENUM_MACRO, nullptr),		// from _hs_type_string..._hs_type_object_definition
		BOOST_PP_ENUM(HS_TYPE_COUNT_ENUMS, TYPECAST_PROC_ENUM_MACRO, hs_enum_to_real),
		BOOST_PP_ENUM(HS_TYPE_COUNT_OBJECTS, TYPECAST_PROC_ENUM_MACRO, nullptr),	// from _hs_type_object...
		BOOST_PP_ENUM(HS_TYPE_COUNT_OBJECTS, TYPECAST_PROC_ENUM_MACRO, nullptr),	// from _hs_type_object_name...
		},

	{	TYPECAST_PROCS_FOR_NONDATA(),			// _hs_type_short
		nullptr,				// from _hs_type_bool
		nullptr,				// from _hs_type_real
		hs_real_to_short,		// from _hs_type_short
		hs_long_to_short,		// from _hs_type_long
		BOOST_PP_ENUM(HS_TYPE_COUNT_DATA_X(9), TYPECAST_PROC_ENUM_MACRO, nullptr) // from _hs_type_string...
		},

	{	TYPECAST_PROCS_FOR_NONDATA(),			// _hs_type_long
		nullptr,				// from _hs_type_bool
		hs_real_to_long,		// from _hs_type_real
		// engine says NULL here then hs_long_to_short for _hs_type_long...pretty sure that's a typo.
		// it will never execute anyway, since actual_type==desired_type in this case
		hs_short_to_long,		// from _hs_type_short
		BOOST_PP_ENUM(HS_TYPE_COUNT_DATA_X(8), TYPECAST_PROC_ENUM_MACRO, nullptr) // from _hs_type_long...
		},

	{	TYPECAST_PROCS_FOR_NONDATA_TYPE() },	// _hs_type_string
	{	TYPECAST_PROCS_FOR_NONDATA_TYPE() },	// _hs_type_script

	{	TYPECAST_PROCS_FOR_NONDATA_TYPE() },	// _hs_type_trigger_volume
	{	TYPECAST_PROCS_FOR_NONDATA_TYPE() },	// _hs_type_cutscene_flag
	{	TYPECAST_PROCS_FOR_NONDATA_TYPE() },	// _hs_type_cutscene_camera_point
	{	TYPECAST_PROCS_FOR_NONDATA_TYPE() },	// _hs_type_cutscene_title
	{	TYPECAST_PROCS_FOR_NONDATA_TYPE() },	// _hs_type_cutscene_recording
	{	TYPECAST_PROCS_FOR_NONDATA_TYPE() },	// _hs_type_device_group
	{	TYPECAST_PROCS_FOR_NONDATA_TYPE() },	// _hs_type_ai
	{	TYPECAST_PROCS_FOR_NONDATA_TYPE() },	// _hs_type_ai_command_list
	{	TYPECAST_PROCS_FOR_NONDATA_TYPE() },	// _hs_type_starting_profile
	{	TYPECAST_PROCS_FOR_NONDATA_TYPE() },	// _hs_type_conversation

	{	TYPECAST_PROCS_FOR_NONDATA_TYPE() },	// _hs_type_navpoint
	{	TYPECAST_PROCS_FOR_NONDATA_TYPE() },	// _hs_type_hud_message

	{	TYPECAST_PROCS_FOR_NONDATA(),			// _hs_type_object_list
		BOOST_PP_ENUM(BOOST_PP_SUB(17,  5), TYPECAST_PROC_ENUM_MACRO, nullptr),							// from _hs_type_bool..._hs_type_device_group
		object_list_from_ai_reference,	// from _hs_type_ai
		BOOST_PP_ENUM(BOOST_PP_SUB(37, 18), TYPECAST_PROC_ENUM_MACRO, nullptr),							// from _hs_type_ai_command_list..._hs_type_hud_corner
		BOOST_PP_ENUM(HS_TYPE_COUNT_OBJECTS, TYPECAST_PROC_ENUM_MACRO, hs_object_to_object_list),		// from _hs_type_object...
		BOOST_PP_ENUM(HS_TYPE_COUNT_OBJECTS, TYPECAST_PROC_ENUM_MACRO, hs_object_name_to_object_list),	// from _hs_type_object_name...
		},
	
	{	TYPECAST_PROCS_FOR_NONDATA_TYPE() },	// _hs_type_sound
	{	TYPECAST_PROCS_FOR_NONDATA_TYPE() },	// _hs_type_effect
	{	TYPECAST_PROCS_FOR_NONDATA_TYPE() },	// _hs_type_damage
	{	TYPECAST_PROCS_FOR_NONDATA_TYPE() },	// _hs_type_looping_sound
	{	TYPECAST_PROCS_FOR_NONDATA_TYPE() },	// _hs_type_animation_graph
	{	TYPECAST_PROCS_FOR_NONDATA_TYPE() },	// _hs_type_actor_variant
	{	TYPECAST_PROCS_FOR_NONDATA_TYPE() },	// _hs_type_damage_effect
	{	TYPECAST_PROCS_FOR_NONDATA_TYPE() },	// _hs_type_object_definition

	{	TYPECAST_PROCS_FOR_NONDATA_TYPE() },	// _hs_type_game_difficulty
	{	TYPECAST_PROCS_FOR_NONDATA_TYPE() },	// _hs_type_team
	{	TYPECAST_PROCS_FOR_NONDATA_TYPE() },	// _hs_type_ai_default_state
	{	TYPECAST_PROCS_FOR_NONDATA_TYPE() },	// _hs_type_actor_type
	{	TYPECAST_PROCS_FOR_NONDATA_TYPE() },	// _hs_type_hud_corner

	{	TYPECAST_PROCS_FOR_NONDATA_TYPE() },	// _hs_type_object
	{	TYPECAST_PROCS_FOR_NONDATA_TYPE() },	// _hs_type_unit
	{	TYPECAST_PROCS_FOR_NONDATA_TYPE() },	// _hs_type_vehicle
	{	TYPECAST_PROCS_FOR_NONDATA_TYPE() },	// _hs_type_weapon
	{	TYPECAST_PROCS_FOR_NONDATA_TYPE() },	// _hs_type_device
	{	TYPECAST_PROCS_FOR_NONDATA_TYPE() },	// _hs_type_scenery

	{	TYPECAST_PROCS_FOR_NONDATA_TYPE() },	// _hs_type_object_name
	{	TYPECAST_PROCS_FOR_NONDATA_TYPE() },	// _hs_type_unit_name
	{	TYPECAST_PROCS_FOR_NONDATA_TYPE() },	// _hs_type_vehicle_name
	{	TYPECAST_PROCS_FOR_NONDATA_TYPE() },	// _hs_type_weapon_name
	{	TYPECAST_PROCS_FOR_NONDATA_TYPE() },	// _hs_type_device_name
	{	TYPECAST_PROCS_FOR_NONDATA_TYPE() },	// _hs_type_scenery_name
};

#undef HS_TYPE_COUNT
#undef HS_TYPE_COUNT_NONDATA
#undef HS_TYPE_COUNT_ENUMS
#undef HS_TYPE_COUNT_OBJECTS
#undef HS_TYPE_COUNT_DATA
#undef HS_TYPE_COUNT_DATA_X

#undef TYPECAST_PROC_ENUM_MACRO
#undef TYPECAST_PROCS_FOR_NONDATA
#undef TYPECAST_PROCS_FOR_NONDATA_TYPE
#undef TYPECAST_PROCS_FOR_NONCASTABLE_TYPE
	};
};