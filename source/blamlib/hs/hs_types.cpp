/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#include <Common/Precompile.hpp>
#include <blamlib/hs/hs_types.hpp>

#include <blamlib/ai/actor_definitions.hpp>					// for k_hs_tag_reference_type_group_tags
#include <blamlib/ai/actor_structures.hpp>
#include <blamlib/ai/actor_types.hpp>						// for g_actor_type_names
#include <blamlib/ai/actors.hpp>							// for g_ai_default_state_names
#include <blamlib/effects/damage_effect_definitions.hpp>	// for k_hs_tag_reference_type_group_tags
#include <blamlib/effects/effect_definitions.hpp>			// for k_hs_tag_reference_type_group_tags
#include <blamlib/game/game.hpp>							// for g_game_difficulty_level_names
#include <blamlib/game/game_allegiance.hpp>					// for g_game_team_names
#include <blamlib/hs/hs_types_traits.hpp>
#include <blamlib/hs/object_lists.hpp>
#include <blamlib/interface/hud_definitions.hpp>			// for g_hud_anchor_names
#include <blamlib/models/model_animation_definitions.hpp>	// for k_hs_tag_reference_type_group_tags
#include <blamlib/objects/object_definitions.hpp>			// for k_hs_tag_reference_type_group_tags
#include <blamlib/objects/object_types.hpp>
#include <blamlib/objects/objects.hpp>
#include <blamlib/scenario/scenario_definitions.hpp>
#include <blamlib/sound/sound_definitions.hpp>				// for k_hs_tag_reference_type_group_tags
#include <blamlib/units/unit_structures.hpp>

namespace Yelo { namespace Scripting
{
	proc_hs_typecast k_hs_typecasting_procedures[Enums::k_number_of_hs_types][Enums::k_number_of_hs_types];
	// NOTE: procs are API_FUNC, not PLATFORM_API, as they should only be called by our re-written code, not the engine

	// TODO: use lexical cast http://www.boost.org/doc/libs/1_55_0/doc/html/boost_lexical_cast.html

	static void hs_inspection_on_none(
		char*const buffer,
		const size_t buffer_size)
	{
		sprintf_s(
			buffer,
			buffer_size,
			"%s",
			"NONE");
	}

	static s_hs_value_union hs_value_to_void(
		s_hs_value_union value)
	{
		return k_null_value_union;
	}

	//////////////////////////////////////////////////////////////////////////
	// _hs_type_bool. nonstandard, the engine never defined any of these casts.
	static s_hs_value_union hs_bool_to_real(
		s_hs_value_union value)
	{
		value.real = value.boolean
			? 1.0f
			: 0.0f;

		return value;
	}

	static s_hs_value_union hs_bool_to_short(
		s_hs_value_union value)
	{
		value.int16 = value.boolean
			? TRUE
			: FALSE;

		return value;
	}

	static s_hs_value_union hs_bool_to_long(
		s_hs_value_union value)
	{
		value.int32 = value.boolean
			? TRUE
			: FALSE;

		return value;
	}

	static void hs_inspect_boolean(
		const Enums::hs_type type,
		const s_hs_value_union value,
		char* const buffer,
		const size_t buffer_size)
	{
		YELO_ASSERT(type == Enums::_hs_type_bool);

		sprintf_s(
			buffer,
			buffer_size,
			"%s",
			value.boolean
			? "true"
			: "false");
	}

	//////////////////////////////////////////////////////////////////////////
	// _hs_type_real
	static s_hs_value_union hs_real_to_short(
		s_hs_value_union value)
	{
		value.int16 = CAST(int16, value.real);

		return value;
	}

	static s_hs_value_union hs_real_to_long(
		s_hs_value_union value)
	{
		value.int32 = CAST(int32, value.real);

		return value;
	}

	// NOTE: non-standard, engine doesn't support this
	static s_hs_value_union hs_real_to_enum(
		s_hs_value_union value)
	{
		// because hs_enum_to_real adds 1 before float conversion
		value.int16 = CAST(int16, value.real) - 1;

		return value;
	}

	static void hs_inspect_real(
		const Enums::hs_type type,
		const s_hs_value_union value,
		char* const buffer,
		const size_t buffer_size)
	{
		YELO_ASSERT(type == Enums::_hs_type_real);

		sprintf_s(
			buffer,
			buffer_size,
			"%f",
			value.real);
	}

	//////////////////////////////////////////////////////////////////////////
	// _hs_type_short
	static s_hs_value_union hs_short_to_boolean(
		s_hs_value_union value)
	{
		value.boolean = value.int16 != 0;

		return value;
	}

	static s_hs_value_union hs_short_to_real(
		s_hs_value_union value)
	{
		value.real = CAST(real, value.int16);

		return value;
	}

	static s_hs_value_union hs_short_to_long(
		s_hs_value_union value)
	{
		value.int32 = CAST(int32, value.int16);

		return value;
	}

	static void hs_inspect_short(
		const Enums::hs_type type,
		const s_hs_value_union value,
		char* const buffer,
		const size_t buffer_size)
	{
		YELO_ASSERT(type == Enums::_hs_type_short);

		sprintf_s(
			buffer,
			buffer_size,
			"%d",
			value.int16);
	}

	//////////////////////////////////////////////////////////////////////////
	// _hs_type_long
	static s_hs_value_union hs_long_to_boolean(
		s_hs_value_union value)
	{
		value.boolean = value.int32 != 0;

		return value;
	}

	static s_hs_value_union hs_long_to_real(
		s_hs_value_union value)
	{
		value.real = CAST(real, value.int32);

		return value;
	}

	static s_hs_value_union hs_long_to_short(
		s_hs_value_union value)
	{
		value.int16 = CAST(int16, value.int32);

		return value;
	}

	static void hs_inspect_long(
		const Enums::hs_type type,
		const s_hs_value_union value,
		char* const buffer,
		const size_t buffer_size)
	{
		YELO_ASSERT(type == Enums::_hs_type_long);

		sprintf_s(
			buffer,
			buffer_size,
			"%ld",
			value.int32);
	}

	//////////////////////////////////////////////////////////////////////////
	// _hs_type_string
	static s_hs_value_union hs_string_to_boolean(
		s_hs_value_union value)
	{
		value.int32 = strlen(value.string);

		return hs_long_to_boolean(value);
	}

	static s_hs_value_union hs_string_to_real(
		s_hs_value_union value)
	{
		value.real = CAST(real, std::atof(value.string));

		return value;
	}

	static void hs_inspect_string(
		const Enums::hs_type type,
		const s_hs_value_union value,
		char* const buffer,
		const size_t buffer_size)
	{
		YELO_ASSERT(type == Enums::_hs_type_string);

		sprintf_s(
			buffer,
			buffer_size,
			"%s",
			value.string);
	}

	//////////////////////////////////////////////////////////////////////////
	// _hs_type_script
	static void hs_inspect_script(
		const Enums::hs_type type,
		const s_hs_value_union value,
		char* const buffer,
		const size_t buffer_size)
	{
		YELO_ASSERT(type == Enums::_hs_type_script);

		auto script_reference = value.script;
		if (script_reference.IsNull())
		{
			hs_inspection_on_none(buffer, buffer_size);
		}

		const auto& script = blam::global_scenario_get()->scripts[script_reference.script_index];

		sprintf_s(
			buffer,
			buffer_size,
			"%s",
			script.name);
	}

	//////////////////////////////////////////////////////////////////////////
	// scenario datums
	template <typename T, bool k_supports_none = true>
	static void hs_inspect_scenario_datum(
		const Enums::hs_type type,
		char* const buffer,
		const size_t buffer_size,
		const int32 index,
		const TAG_TBLOCK(&datums, T))
	{
		static_assert(std::is_same<tag_string, decltype(T::name)>::value,
			"expected the scenario datum to have a field called 'name' that is a tag_string");

		if (k_supports_none)
		{
			if (index == NONE)
			{
				hs_inspection_on_none(buffer, buffer_size);
			}
		}
		else
		{
			YELO_ASSERT(index != NONE);
		}

		const auto& datum = datums[index];
		// T's name should be a tag_string
		auto name_address = CAST_PTR(const byte*, &datum) + offsetof(T, name);

		const cstring name = CAST_PTR(cstring, name_address);

		sprintf_s(
			buffer,
			buffer_size,
			"%s",
			name);
	}

	static void hs_inspect_trigger_volume(
		const Enums::hs_type type,
		const s_hs_value_union value,
		char* const buffer,
		const size_t buffer_size)
	{
		YELO_ASSERT(type == Enums::_hs_type_trigger_volume);

		hs_inspect_scenario_datum<TagGroups::scenario_trigger_volume>(
			type,
			buffer,
			buffer_size,
			value.int16,
			blam::global_scenario_get()->trigger_volumes);
	}

	static void hs_inspect_cutscene_flag(
		const Enums::hs_type type,
		const s_hs_value_union value,
		char* const buffer,
		const size_t buffer_size)
	{
		YELO_ASSERT(type == Enums::_hs_type_cutscene_flag);

		hs_inspect_scenario_datum<TagGroups::scenario_cutscene_flag>(
			type,
			buffer,
			buffer_size,
			value.int16,
			blam::global_scenario_get()->cutscene_flags);
	}

	static void hs_inspect_cutscene_camera_point(
		const Enums::hs_type type,
		const s_hs_value_union value,
		char* const buffer,
		const size_t buffer_size)
	{
		YELO_ASSERT(type == Enums::_hs_type_cutscene_camera_point);

		hs_inspect_scenario_datum<TagGroups::scenario_cutscene_flag>(
			type,
			buffer,
			buffer_size,
			value.int16,
			blam::global_scenario_get()->cutscene_flags);
	}

	static void hs_inspect_cutscene_title(
		const Enums::hs_type type,
		const s_hs_value_union value,
		char* const buffer,
		const size_t buffer_size)
	{
		YELO_ASSERT(type == Enums::_hs_type_cutscene_title);

		hs_inspect_scenario_datum<TagGroups::s_scenario_cutscene_title>(
			type,
			buffer,
			buffer_size,
			value.int16,
			blam::global_scenario_get()->cutscene_titles);
	}

	static void hs_inspect_cutscene_recording(
		const Enums::hs_type type,
		const s_hs_value_union value,
		char* const buffer,
		const size_t buffer_size)
	{
		YELO_ASSERT(type == Enums::_hs_type_cutscene_recording);

		hs_inspect_scenario_datum<TagGroups::recorded_animation_definition>(
			type,
			buffer,
			buffer_size,
			value.int16,
			blam::global_scenario_get()->recorded_animations);
	}

	static void hs_inspect_device_group(
		const Enums::hs_type type,
		const s_hs_value_union value,
		char* const buffer,
		const size_t buffer_size)
	{
		YELO_ASSERT(type == Enums::_hs_type_device_group);

		hs_inspect_scenario_datum<TagGroups::scenario_device_group>(
			type,
			buffer,
			buffer_size,
			value.int16,
			blam::global_scenario_get()->device_groups);
	}

	static void hs_inspect_starting_profile(
		const Enums::hs_type type,
		const s_hs_value_union value,
		char* const buffer,
		const size_t buffer_size)
	{
		YELO_ASSERT(type == Enums::_hs_type_starting_profile);

		hs_inspect_scenario_datum<TagGroups::scenario_starting_profile>(
			type,
			buffer,
			buffer_size,
			value.int16,
			blam::global_scenario_get()->player_starting_profiles);
	}

	static void hs_inspect_navpoint(
		const Enums::hs_type type,
		const s_hs_value_union value,
		char* const buffer,
		const size_t buffer_size)
	{
		YELO_ASSERT(type == Enums::_hs_type_navpoint);

		YELO_ASSERT(false); // TODO
	}

	static void hs_inspect_hud_message(
		const Enums::hs_type type,
		const s_hs_value_union value,
		char* const buffer,
		const size_t buffer_size)
	{
		YELO_ASSERT(type == Enums::_hs_type_hud_message);

		YELO_ASSERT(false); // TODO
	}

	//////////////////////////////////////////////////////////////////////////
	// tag references
	// NOTE: non-standard, engine doesn't support this
	static s_hs_value_union hs_tag_reference_to_long(
		s_hs_value_union value)
	{
		// can just fall through as long as this is true
		BOOST_STATIC_ASSERT(sizeof(value.int32) == sizeof(value.tag_index));

		return value;
	}

	// NOTE: non-standard, engine doesn't support this
	static s_hs_value_union hs_actor_variant_to_object_definition(
		s_hs_value_union value)
	{
		YELO_ASSERT(false); // TODO: lookup actor variant def and return its unit tag index

		return k_none_value_union;
	}

	// NOTE: non-standard, engine doesn't support this
	static s_hs_value_union hs_object_definition_to_animation_graph(
		s_hs_value_union value)
	{
		YELO_ASSERT(false); // TODO: lookup object def and return its animation tag index

		return k_none_value_union;
	}

	static void hs_inspect_tag_reference(
		const Enums::hs_type type,
		const s_hs_value_union value,
		char* const buffer,
		const size_t buffer_size)
	{
		YELO_ASSERT(blam::hs_type_is_tag_reference(type));

		auto tag_index = value.ToTagIndex();
		if (tag_index.IsNull())
		{
			hs_inspection_on_none(buffer, buffer_size);
		}

		const auto name = "NOT-YET-IMPLEMENTED"; // TODO

		sprintf_s(
			buffer,
			buffer_size,
			"%s",
			name);
	}

	//////////////////////////////////////////////////////////////////////////
	// enums
	static s_hs_value_union hs_enum_to_real(
		s_hs_value_union value)
	{
		// yes, the engine adds 1 before converting to a float
		value.real = CAST(real, value.int16 + 1);

		return value;
	}

	static void hs_inspect_enum(
		const Enums::hs_type type,
		const s_hs_value_union value,
		char* const buffer,
		const size_t buffer_size)
	{
		YELO_ASSERT(blam::hs_type_is_enum(type));

		const auto& enum_definition = blam::hs_enum_table[type - Enums::_hs_type_enum__first];
		_enum enum_value = value.int16;

		// NOTE: engine doesn't handle NONE cases, it asserts
		if (enum_value == NONE)
		{
			hs_inspection_on_none(buffer, buffer_size);
		}

		YELO_ASSERT(enum_value >= 0 && enum_value<enum_definition.count);

		sprintf_s(
			buffer,
			buffer_size,
			"%s",
			enum_definition[enum_value]);
	}


	//////////////////////////////////////////////////////////////////////////
	// _hs_type_ai
	// actually defined in ai_script.c
	static s_hs_value_union object_list_from_ai_reference(
		s_hs_value_union value)
	{
		if (value.int32 == NONE)
		{
			value.datum = datum_index::null;
			return value;
		}

		const auto list_index = blam::object_list_new();
		const auto ai_reference = *CAST_PTR(AI::s_ai_index*, &value.pointer);

		AI::s_ai_index_actor_iterator iterator;
		blam::ai_index_actor_iterator_new(ai_reference, iterator);
		while (auto* actor = blam::ai_index_actor_iterator_next(iterator))
		{
			auto unit_index = actor->meta.unit_index;
			if (!unit_index.IsNull())
			{
				blam::object_list_add(list_index, unit_index);
			}

			// add all the units that make up this actor's swarm
			for (auto swarm_unit_index = actor->meta.swarm_unit_index; !swarm_unit_index.IsNull();)
			{
				auto* unit = blam::object_get_and_verify_type<Objects::s_unit_datum>(swarm_unit_index);
				blam::object_list_add(list_index, swarm_unit_index);

				swarm_unit_index = unit->unit.swarm_next_unit_index;
			}
		}

		value.datum = list_index;
		return value;
	}

	//////////////////////////////////////////////////////////////////////////
	// _hs_type_object
	static s_hs_value_union hs_object_to_object_list(
		s_hs_value_union value)
	{
		if (value.datum.IsNull())
		{
			value.datum = datum_index::null;
			return value;
		}

		const auto list_index = blam::object_list_new();
		blam::object_list_add(list_index, value.ToObjectIndex());

		value.datum = list_index;
		return value;
	}

	static void hs_inspect_object(
		const Enums::hs_type type,
		const s_hs_value_union value,
		char* const buffer,
		const size_t buffer_size)
	{
		YELO_ASSERT(blam::hs_type_is_object(type));

		const auto object_index = value.ToObjectIndex();
		if (object_index.IsNull())
		{
			hs_inspection_on_none(buffer, buffer_size);
		}

		const auto name = "NOT-YET-IMPLEMENTED"; // TODO

		sprintf_s(
			buffer,
			buffer_size,
			"%s",
			name);
	}

	//////////////////////////////////////////////////////////////////////////
	// _hs_type_object_name
	static s_hs_value_union hs_object_name_to_object_list(
		s_hs_value_union value)
	{
		const auto object_index = blam::object_index_from_name_index(value.int16);
		value.datum = object_index;

		return hs_object_to_object_list(value);
	}

	static void hs_inspect_object_name(
		const Enums::hs_type type,
		const s_hs_value_union value,
		char* const buffer,
		const size_t buffer_size)
	{
		YELO_ASSERT(blam::hs_type_is_object_name(type));

		const auto name_index = value.int16;
		if (name_index == NONE)
		{
			hs_inspection_on_none(buffer, buffer_size);
		}

		const auto& object_name = blam::global_scenario_get()->object_names[name_index];

		const auto name = object_name.name;

		sprintf_s(
			buffer,
			buffer_size,
			"%s",
			name);
	}


	static void hs_types_initialize_type_casting_procedures()
	{
		typedef proc_hs_typecast (&typecasting_procedures_t)[Enums::k_number_of_hs_types];

		auto& void_procedures = k_hs_typecasting_procedures[Enums::_hs_type_void];
		auto& bool_procedures = k_hs_typecasting_procedures[Enums::_hs_type_bool];
		auto& real_procedures = k_hs_typecasting_procedures[Enums::_hs_type_real];
		auto& short_procedures = k_hs_typecasting_procedures[Enums::_hs_type_short];
		auto& long_procedures = k_hs_typecasting_procedures[Enums::_hs_type_long];
		auto& string_procedures = k_hs_typecasting_procedures[Enums::_hs_type_string];
		auto& object_list_procedures = k_hs_typecasting_procedures[Enums::_hs_type_object_list];

		// _hs_type_void
		for (_enum type = Enums::_hs_type_data__first; type < Enums::_hs_type_data__last + 1; type++)
		{
			void_procedures[type] = hs_value_to_void;
		}

		// _hs_type_bool
		bool_procedures[Enums::_hs_type_real] = hs_long_to_boolean; // yes long, (this is what the engine does)
		bool_procedures[Enums::_hs_type_short] = hs_short_to_boolean;
		bool_procedures[Enums::_hs_type_long] = hs_long_to_boolean;
		bool_procedures[Enums::_hs_type_string] = hs_string_to_boolean;

		// _hs_type_real
		real_procedures[Enums::_hs_type_bool] = hs_bool_to_real;
		real_procedures[Enums::_hs_type_short] = hs_short_to_real;
		real_procedures[Enums::_hs_type_long] = hs_long_to_real;
		//			real_procedures[_hs_type_string] =	hs_string_to_real;
		for (_enum type = Enums::_hs_type_enum__first; type < Enums::_hs_type_enum__last + 1; type++)
		{
			real_procedures[type] = hs_enum_to_real;
		}

		// _hs_type_short
		short_procedures[Enums::_hs_type_bool] = hs_bool_to_short;
		short_procedures[Enums::_hs_type_real] = hs_real_to_short;
		short_procedures[Enums::_hs_type_long] = hs_long_to_short;
		//			short_procedures[_hs_type_string] =	hs_string_to_short;

		// _hs_type_long
		long_procedures[Enums::_hs_type_bool] = hs_bool_to_long;
		long_procedures[Enums::_hs_type_real] = hs_real_to_long;
		// NOTE: due to a typo, the engine actually didn't support casting from short to long
		long_procedures[Enums::_hs_type_short] = hs_short_to_long;
		//			long_procedures[_hs_type_string] =	hs_string_to_long;

		// _hs_type_string

		// _hs_type_object_list
		object_list_procedures[Enums::_hs_type_ai] = object_list_from_ai_reference;
		for (_enum type = Enums::_hs_type_object__first; type < Enums::_hs_type_object__last + 1; type++)
		{
			object_list_procedures[type] = hs_object_to_object_list;
		}
		for (_enum type = Enums::_hs_type_object_name__first; type < Enums::_hs_type_object_name__last + 1; type++)
		{
			object_list_procedures[type] = hs_object_name_to_object_list;
		}
	}

	void HsTypesInitialize()
	{
		hs_types_initialize_type_casting_procedures();
	}

	void HsTypesDispose() { }
}

namespace blam
{
	bool hs_type_valid(
		_enum type)
	{
		return type >= Enums::_hs_type_void && type < Enums::k_number_of_hs_types;
	}

	bool hs_type_is_enum(
		_enum type)
	{
		return type >= Enums::_hs_type_enum__first && type < Enums::_hs_type_enum__last;
	}

	bool hs_type_is_tag_reference(
		_enum type)
	{
		return type >= Enums::_hs_type_tag_reference__first && type < Enums::_hs_type_tag_reference__last;
	}

	bool hs_type_is_object(
		_enum type)
	{
		return type >= Enums::_hs_type_object__first && type < Enums::_hs_type_object__last;
	}

	bool hs_type_is_object_name(
		_enum type)
	{
		return type >= Enums::_hs_type_object_name__first && type < Enums::_hs_type_object_name__last;
	}

	const std::array<word_flags, Enums::k_number_of_hs_object_types> k_hs_object_type_masks = {
		Enums::_object_type_mask_all,
		// engine actually uses 0xFFFF here
		Enums::_object_type_mask_unit,
		FLAG(Enums::_object_type_vehicle),
		FLAG(Enums::_object_type_weapon),
		Enums::_object_type_mask_device,
		FLAG(Enums::_object_type_scenery),
	};
	const std::array<tag, Enums::_hs_type_tag_reference__count> k_hs_tag_reference_type_group_tags = {
		TagGroups::sound_definition::k_group_tag,
		TagGroups::effect_definition::k_group_tag,
		TagGroups::s_damage_effect_definition::k_group_tag,
		// damage
		TagGroups::looping_sound_definition::k_group_tag,
		TagGroups::model_animation_graph::k_group_tag,
		TagGroups::s_actor_variant_definition::k_group_tag,
		TagGroups::s_damage_effect_definition::k_group_tag,
		// damage_effect
		TagGroups::s_object_definition::k_group_tag,
	};

	std::array<cstring, Enums::k_number_of_hs_types> hs_type_names = {
		"unparsed",
		"special_form",
		// NOTE: changed spaces to underscores
		"function_name",
		// NOTE: changed spaces to underscores
		"passthrough",
		"void",

		"boolean",
		"real",
		"short",
		"long",
		"string",
		"script",

		"trigger_volume",
		"cutscene_flag",
		"cutscene_camera_point",
		"cutscene_title",
		"cutscene_recording",
		"device_group",
		"ai",
		"ai_command_list",
		"starting_profile",
		"conversation",

		"navpoint",
		"hud_message",

		"object_list",

		"sound",
		"effect",
		"damage",
		"looping_sound",
		"animation_graph",
		"actor_variant",
		"damage_effect",
		"object_definition",

		"game_difficulty",
		"team",
		"ai_default_state",
		"actor_type",
		"hud_corner",

		"object",
		"unit",
		"vehicle",
		"weapon",
		"device",
		"scenery",

		"object_name",
		"unit_name",
		"vehicle_name",
		"weapon_name",
		"device_name",
		"scenery_name",
	};

	std::array<cstring, Enums::k_number_of_hs_script_types> hs_script_type_names = {
		"startup",
		"dormant",
		"continuous",
		"static",
		"stub",
	};

	// should be in game/game.cpp
	cstring g_game_difficulty_level_names[Enums::k_number_of_game_difficulty_levels] = {
		"easy",
		"normal",
		"hard",
		"impossible",
	};
	// should be in game/game_allegiance.cpp
	cstring g_game_team_names[e_game_team::k_count] = {
		"default",
		"player",
		"human",
		"covenant",
		"flood",
		"sentinel",
		"unused6",
		"unused7",
		"unused8",
		"unused9",
	};
	// should be in ai/ai_scenario_definitions.cpp
	cstring g_ai_default_state_names[AI::e_actor_default_state::k_count] = {
		"none",
		"sleep",
		"alert",
		"move_repeat",
		"move_loop",
		"move_loop_back_and_forth",
		"move_loop_random",
		"move_random",
		"guard",
		"guard_at_position",
		"search",
		"flee",
	};
	// should be in ai/actor_types.cpp
	cstring g_actor_type_names[AI::e_actor_type::k_count] = {
		"elite",
		"jackal",
		"grunt",
		"hunter",
		"engineer",
		"assassin",
		"player",
		"marine",
		"crew",
		"combat_form",
		"infection_form",
		"carrier_form",
		"monitor",
		"sentinel",
		"none",
		"mounted_weapon",
	};
	// should be in interface/hud_definitions.cpp
	cstring g_hud_anchor_names[Enums::k_number_of_hud_anchors] = {
		"top_left",
		"top_right",
		"bottom_left",
		"bottom_right",
		"center",
	};
	std::array<const string_list, Enums::_hs_type_enum__count> hs_enum_table = {
		string_list {NUMBEROF(g_game_difficulty_level_names), &g_game_difficulty_level_names[0]},
		string_list {NUMBEROF(g_game_team_names), &g_game_team_names[0]},
		string_list {NUMBEROF(g_ai_default_state_names), &g_ai_default_state_names[0]},
		string_list {NUMBEROF(g_actor_type_names), &g_actor_type_names[0]},
		string_list {NUMBEROF(g_hud_anchor_names), &g_hud_anchor_names[0]},
	};
}};
