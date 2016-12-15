/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once // should actually only ever be included by hs_types.cpp...

#include <blamlib/Halo1/ai/ai_script.hpp>
#include <blamlib/Halo1/ai/actor_structures.hpp>
#include <blamlib/Halo1/hs/hs_structures.hpp>
#include <blamlib/Halo1/hs/hs_types.hpp>
#include <blamlib/Halo1/hs/object_lists.hpp>
#include <blamlib/Halo1/objects/objects.hpp>
#include <blamlib/Halo1/scenario/scenario.hpp>
#include <blamlib/Halo1/scenario/scenario_definitions.hpp>
#include <blamlib/Halo1/units/unit_structures.hpp>

#include <type_traits>

namespace Yelo
{
	namespace Scripting
	{
		s_hs_value_union c_hs_type_bool_abi::ReadValue(Enums::hs_type type, const s_hs_value_union* source) const
		{
			if (source != nullptr)
				return GetValue(source->boolean);

			return GetDefaultValue();
		}
		void c_hs_type_bool_abi::WriteValue(Enums::hs_type type, s_hs_value_union source, s_hs_value_union& destination) const
		{
			destination.boolean = source.boolean;
		}
	};

	namespace blam
	{
		using namespace Scripting;

		// NOTE: procs are API_FUNC, not PLATFORM_API, as they should only be called by our re-written code, not the engine

		// TODO: use lexical cast http://www.boost.org/doc/libs/1_55_0/doc/html/boost_lexical_cast.html

		static void hs_inspection_on_none(char* buffer, size_t buffer_size)
		{
			sprintf_s(buffer, buffer_size, "%s",
				"NONE");
		}

		static s_hs_value_union hs_value_to_void(s_hs_value_union value)
		{
			return k_null_value_union;
		}

		//////////////////////////////////////////////////////////////////////////
		// _hs_type_bool. nonstandard, the engine never defined any of these casts.
		static s_hs_value_union hs_bool_to_real(s_hs_value_union value)
		{
			value.real = value.boolean ? 1.0f : 0.0f;

			return value;
		}
		static s_hs_value_union hs_bool_to_short(s_hs_value_union value)
		{
			value.int16 = value.boolean ? TRUE : FALSE;

			return value;
		}
		static s_hs_value_union hs_bool_to_long(s_hs_value_union value)
		{
			value.int32 = value.boolean ? TRUE : FALSE;

			return value;
		}
		static void hs_inspect_boolean(Enums::hs_type type, s_hs_value_union value, char* buffer, size_t buffer_size)
		{
			YELO_ASSERT(type==Enums::_hs_type_bool);

			sprintf_s(buffer, buffer_size, "%s", 
				value.boolean ? "true" : "false");
		}

		//////////////////////////////////////////////////////////////////////////
		// _hs_type_real
		static s_hs_value_union hs_real_to_short(s_hs_value_union value)
		{
			value.int16 = CAST(int16, value.real);

			return value;
		}
		static s_hs_value_union hs_real_to_long(s_hs_value_union value)
		{
			value.int32 = CAST(int32, value.real);

			return value;
		}
		// NOTE: non-standard, engine doesn't support this
		static s_hs_value_union hs_real_to_enum(s_hs_value_union value)
		{
			// because hs_enum_to_real adds 1 before float conversion
			value.int16 = CAST(int16, value.real) - 1;

			return value;
		}
		static void hs_inspect_real(Enums::hs_type type, s_hs_value_union value, char* buffer, size_t buffer_size)
		{
			YELO_ASSERT(type==Enums::_hs_type_real);

			sprintf_s(buffer, buffer_size, "%f", 
				value.real);
		}

		//////////////////////////////////////////////////////////////////////////
		// _hs_type_short
		static s_hs_value_union hs_short_to_boolean(s_hs_value_union value)
		{
			value.boolean = value.int16 != 0;

			return value;
		}
		static s_hs_value_union hs_short_to_real(s_hs_value_union value)
		{
			value.real = CAST(real, value.int16);

			return value;
		}
		static s_hs_value_union hs_short_to_long(s_hs_value_union value)
		{
			value.int32 = CAST(int32, value.int16);

			return value;
		}
		static void hs_inspect_short(Enums::hs_type type, s_hs_value_union value, char* buffer, size_t buffer_size)
		{
			YELO_ASSERT(type==Enums::_hs_type_short);

			sprintf_s(buffer, buffer_size, "%d", 
				value.int16);
		}

		//////////////////////////////////////////////////////////////////////////
		// _hs_type_long
		static s_hs_value_union hs_long_to_boolean(s_hs_value_union value)
		{
			value.boolean = value.int32 != 0;

			return value;
		}
		static s_hs_value_union hs_long_to_real(s_hs_value_union value)
		{
			value.real = CAST(real, value.int32);

			return value;
		}
		static s_hs_value_union hs_long_to_short(s_hs_value_union value)
		{
			value.int16 = CAST(int16, value.int32);

			return value;
		}
		static void hs_inspect_long(Enums::hs_type type, s_hs_value_union value, char* buffer, size_t buffer_size)
		{
			YELO_ASSERT(type==Enums::_hs_type_long);

			sprintf_s(buffer, buffer_size, "%ld", 
				value.int32);
		}

		//////////////////////////////////////////////////////////////////////////
		// _hs_type_string
		static s_hs_value_union hs_string_to_boolean(s_hs_value_union value)
		{
			value.int32 = strlen(value.string);

			return hs_long_to_boolean(value);
		}
		static s_hs_value_union hs_string_to_real(s_hs_value_union value)
		{
			value.real = CAST(real, std::atof(value.string));

			return value;
		}
		static void hs_inspect_string(Enums::hs_type type, s_hs_value_union value, char* buffer, size_t buffer_size)
		{
			YELO_ASSERT(type==Enums::_hs_type_string);

			sprintf_s(buffer, buffer_size, "%s",
				value.string);
		}

		//////////////////////////////////////////////////////////////////////////
		// _hs_type_script
		static void hs_inspect_script(Enums::hs_type type, s_hs_value_union value, char* buffer, size_t buffer_size)
		{
			YELO_ASSERT(type == Enums::_hs_type_script);

			auto script_reference = value.script;
			if (script_reference.IsNull())
				hs_inspection_on_none(buffer, buffer_size);

			const auto& script = global_scenario_get()->scripts[script_reference.script_index];

			sprintf_s(buffer, buffer_size, "%s",
				 script.name);
		}

		//////////////////////////////////////////////////////////////////////////
		// scenario datums
		template<typename T, bool k_supports_none = true>
		static void hs_inspect_scenario_datum(Enums::hs_type type, char* buffer, size_t buffer_size,
			int32 index, const TAG_TBLOCK(& datums, T))
		{
			static_assert(std::is_same<tag_string, decltype(T::name)>::value,
				"expected the scenario datum to have a field called 'name' that is a tag_string");

			if (k_supports_none)
			{
				if (index == NONE)
					hs_inspection_on_none(buffer, buffer_size);
			}
			else
			{
				YELO_ASSERT(index != NONE);
			}

			const auto& datum = datums[index];
			// T's name should be a tag_string
			const byte* name_address = CAST_PTR(const byte*, &datum) + offsetof(T, name);

			cstring name = CAST_PTR(cstring, name_address);

			sprintf_s(buffer, buffer_size, "%s",
				name);
		}
		static void hs_inspect_trigger_volume(Enums::hs_type type, s_hs_value_union value, char* buffer, size_t buffer_size)
		{
			YELO_ASSERT(type == Enums::_hs_type_trigger_volume);

			hs_inspect_scenario_datum<TagGroups::scenario_trigger_volume>(
				type, buffer, buffer_size,
				value.int16, global_scenario_get()->trigger_volumes);
		}
		static void hs_inspect_cutscene_flag(Enums::hs_type type, s_hs_value_union value, char* buffer, size_t buffer_size)
		{
			YELO_ASSERT(type == Enums::_hs_type_cutscene_flag);

			hs_inspect_scenario_datum<TagGroups::scenario_cutscene_flag>(
				type, buffer, buffer_size,
				value.int16, global_scenario_get()->cutscene_flags);
		}
		static void hs_inspect_cutscene_camera_point(Enums::hs_type type, s_hs_value_union value, char* buffer, size_t buffer_size)
		{
			YELO_ASSERT(type == Enums::_hs_type_cutscene_camera_point);

			hs_inspect_scenario_datum<TagGroups::scenario_cutscene_flag>(
				type, buffer, buffer_size,
				value.int16, global_scenario_get()->cutscene_flags);
		}
		static void hs_inspect_cutscene_title(Enums::hs_type type, s_hs_value_union value, char* buffer, size_t buffer_size)
		{
			YELO_ASSERT(type == Enums::_hs_type_cutscene_title);

			hs_inspect_scenario_datum<TagGroups::s_scenario_cutscene_title>(
				type, buffer, buffer_size,
				value.int16, global_scenario_get()->cutscene_titles);
		}
		static void hs_inspect_cutscene_recording(Enums::hs_type type, s_hs_value_union value, char* buffer, size_t buffer_size)
		{
			YELO_ASSERT(type == Enums::_hs_type_cutscene_recording);

			hs_inspect_scenario_datum<TagGroups::recorded_animation_definition>(
				type, buffer, buffer_size,
				value.int16, global_scenario_get()->recorded_animations);
		}
		static void hs_inspect_device_group(Enums::hs_type type, s_hs_value_union value, char* buffer, size_t buffer_size)
		{
			YELO_ASSERT(type == Enums::_hs_type_device_group);

			hs_inspect_scenario_datum<TagGroups::scenario_device_group>(
				type, buffer, buffer_size,
				value.int16, global_scenario_get()->device_groups);
		}
#if 0
		static void hs_inspect_ai_command_list(Enums::hs_type type, s_hs_value_union value, char* buffer, size_t buffer_size)
		{
			YELO_ASSERT(type == Enums::_hs_type_ai_command_list);

			hs_inspect_scenario_datum<TagGroups::ai_command_list_definition>(
				type, buffer, buffer_size,
				value.int16, global_scenario_get()->ai_command_lists);
		}
#endif
		static void hs_inspect_starting_profile(Enums::hs_type type, s_hs_value_union value, char* buffer, size_t buffer_size)
		{
			YELO_ASSERT(type == Enums::_hs_type_starting_profile);

			hs_inspect_scenario_datum<TagGroups::scenario_starting_profile>(
				type, buffer, buffer_size,
				value.int16, global_scenario_get()->player_starting_profiles);
		}
#if 0
		static void hs_inspect_conversation(Enums::hs_type type, s_hs_value_union value, char* buffer, size_t buffer_size)
		{
			YELO_ASSERT(type == Enums::_hs_type_conversation);

			hs_inspect_scenario_datum<TagGroups::ai_conversation>(
				type, buffer, buffer_size,
				value.int16, global_scenario_get()->ai_conversations);
		}
#endif
		static void hs_inspect_navpoint(Enums::hs_type type, s_hs_value_union value, char* buffer, size_t buffer_size)
		{
			YELO_ASSERT(type == Enums::_hs_type_navpoint);

			YELO_ASSERT(false); // TODO
		}
		static void hs_inspect_hud_message(Enums::hs_type type, s_hs_value_union value, char* buffer, size_t buffer_size)
		{
			YELO_ASSERT(type == Enums::_hs_type_hud_message);

			YELO_ASSERT(false); // TODO
		}

		//////////////////////////////////////////////////////////////////////////
		// tag references
		// NOTE: non-standard, engine doesn't support this
		static s_hs_value_union hs_tag_reference_to_long(s_hs_value_union value)
		{
			// can just fall through as long as this is true
			BOOST_STATIC_ASSERT(sizeof(value.int32) == sizeof(value.tag_index));

			return value;
		}
		// NOTE: non-standard, engine doesn't support this
		static s_hs_value_union hs_actor_variant_to_object_definition(s_hs_value_union value)
		{
			YELO_ASSERT(false); // TODO: lookup actor variant def and return its unit tag index

			return k_none_value_union;
		}
		// NOTE: non-standard, engine doesn't support this
		static s_hs_value_union hs_object_definition_to_animation_graph(s_hs_value_union value)
		{
			YELO_ASSERT(false); // TODO: lookup object def and return its animation tag index

			return k_none_value_union;
		}
		static void hs_inspect_tag_reference(Enums::hs_type type, s_hs_value_union value, char* buffer, size_t buffer_size)
		{
			YELO_ASSERT(hs_type_is_tag_reference(type));

			datum_index tag_index = value.ToTagIndex();
			if (tag_index.IsNull())
				hs_inspection_on_none(buffer, buffer_size);

			cstring name = "NOT-YET-IMPLEMENTED"; // TODO

			sprintf_s(buffer, buffer_size, "%s",
				name);
		}

		//////////////////////////////////////////////////////////////////////////
		// enums
		static s_hs_value_union hs_enum_to_real(s_hs_value_union value)
		{
			// yes, the engine adds 1 before converting to a float
			value.real = CAST(real, value.int16 + 1);

			return value;
		}
		static void hs_inspect_enum(Enums::hs_type type, s_hs_value_union value, char* buffer, size_t buffer_size)
		{
			YELO_ASSERT(hs_type_is_enum(type));

			const auto& enum_definition = hs_enum_table[type - Enums::_hs_type_enum__first];
			_enum enum_value = value.int16;

			// NOTE: engine doesn't handle NONE cases, it asserts
			if (enum_value == NONE)
				hs_inspection_on_none(buffer, buffer_size);

			YELO_ASSERT(enum_value>=0 && enum_value<enum_definition.count);

			sprintf_s(buffer, buffer_size, "%s", 
				enum_definition[enum_value]);
		}


		//////////////////////////////////////////////////////////////////////////
		// _hs_type_ai
		// actually defined in ai_script.c
		static s_hs_value_union object_list_from_ai_reference(s_hs_value_union value)
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
				datum_index unit_index = actor->meta.unit_index;
				if (!unit_index.IsNull())
				{
					object_list_add(list_index, unit_index);
				}

				// add all the units that make up this actor's swarm
				for (datum_index swarm_unit_index = actor->meta.swarm_unit_index; !swarm_unit_index.IsNull(); )
				{
					auto* unit = object_get_and_verify_type<s_unit_datum>(swarm_unit_index);
					object_list_add(list_index, swarm_unit_index);

					swarm_unit_index = unit->unit.swarm_next_unit_index;
				}
			}

			value.datum = list_index;
			return value;
		}

		//////////////////////////////////////////////////////////////////////////
		// _hs_type_object
		static s_hs_value_union hs_object_to_object_list(s_hs_value_union value)
		{
			if (value.datum.IsNull())
			{
				value.datum = datum_index::null;
				return value;
			}

			datum_index list_index = object_list_new();
			object_list_add(list_index, value.ToObjectIndex());

			value.datum = list_index;
			return value;
		}
		static void hs_inspect_object(Enums::hs_type type, s_hs_value_union value, char* buffer, size_t buffer_size)
		{
			YELO_ASSERT(hs_type_is_object(type));

			datum_index object_index = value.ToObjectIndex();
			if (object_index.IsNull())
				hs_inspection_on_none(buffer, buffer_size);

			cstring name = "NOT-YET-IMPLEMENTED"; // TODO

			sprintf_s(buffer, buffer_size, "%s",
				name);
		}

		//////////////////////////////////////////////////////////////////////////
		// _hs_type_object_name
		static s_hs_value_union hs_object_name_to_object_list(s_hs_value_union value)
		{
			datum_index object_index = object_index_from_name_index(value.int16);
			value.datum = object_index;

			return hs_object_to_object_list(value);
		}
		static void hs_inspect_object_name(Enums::hs_type type, s_hs_value_union value, char* buffer, size_t buffer_size)
		{
			YELO_ASSERT(hs_type_is_object_name(type));

			int16 name_index = value.int16;
			if (name_index == NONE)
				hs_inspection_on_none(buffer, buffer_size);

			const auto& object_name = global_scenario_get()->object_names[name_index];

			cstring name = object_name.name;

			sprintf_s(buffer, buffer_size, "%s",
				name);
		}
	};
};