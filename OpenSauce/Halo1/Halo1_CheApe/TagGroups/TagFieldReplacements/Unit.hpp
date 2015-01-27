/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#pragma once

#include <blamlib/Halo1/tag_files/tag_groups_structures_macros.hpp>
#include <YeloLib/Halo1/tag_files/tag_fieldset_replacement_builder.hpp>
#include <blamlib/Halo1/units/unit_definitions.hpp>
#include <blamlib/Halo1/effects/damage_effect_definitions.hpp>

#include "TagGroups/TagFieldReplacements/Shared.hpp"

namespace Yelo
{
	namespace TagGroups { namespace TagFieldReplacements { namespace Unit
	{
		enum
		{
			k_unit_extensions_pad_field_index = 0x2E
		};

		static bool PLATFORM_API UnitPostprocess(datum_index tag_index, Enums::tag_postprocess_mode mode)
		{
			if(mode != Enums::_tag_postprocess_mode_for_runtime)
			{
				return true;
			}

			auto& tag_definition = *blam::tag_get<TagGroups::s_unit_definition>(tag_index);
			auto& unit_definition = tag_definition.unit;
			
			// Clear any existing seat target blocks
			for(auto& seat : unit_definition.seats)
			{
				if(seat.seat_extensions.Count != 1)
				{
					continue;
				}

				auto& seat_extension = seat.seat_extensions[0];

				while(seat_extension.seat_targeting_seats.Count)
				{
					blam::tag_block_delete_element(seat_extension.seat_targeting_seats, 0);
				}
			}

			// Link all seat targets
			for(int16 index = 0; index < unit_definition.seats.Count; index++)
			{
				auto& seat = unit_definition.seats[index];
				if(seat.seat_extensions.Count != 1)
				{
					continue;
				}

				auto& seat_extensions = seat.seat_extensions[0];
				if(seat_extensions.target_seat_index == NONE)
				{
					continue;
				}

				// If the target seat has no extensions block, add one
				auto& target_seat = unit_definition.seats[seat_extensions.target_seat_index];
				if(target_seat.seat_extensions.Count == 0)
				{
					blam::tag_block_add_element(target_seat.seat_extensions);
				}
				
				// Add a reference to the current seat to the target seat if not already present
				auto& target_seat_extensions = target_seat.seat_extensions[0];
				auto found_entry = std::find(target_seat_extensions.seat_targeting_seats.begin(), target_seat_extensions.seat_targeting_seats.end(), index);
				if(found_entry == target_seat_extensions.seat_targeting_seats.end())
				{
					auto& targeting_seat = *CAST_PTR(int16*, blam::tag_block_add_and_get_element(target_seat_extensions.seat_targeting_seats));

					targeting_seat = index;
				}
			}

			return true;
		}

		static void UnitSeatBlockUpdateFlags(tag_block_definition* unit_seat_block)
		{
			auto* flags_list = Shared::FindFlagsField<Enums::_field_long_flags>(unit_seat_block);
			assert(flags_list->count==Flags::k_number_of_unit_seat_definition_flags);

			static cstring unit_seat_flags_yelo_strings[Flags::k_number_of_unit_seat_definition_flags_yelo];
			for (int x = 0; x < Flags::k_number_of_unit_seat_definition_flags; x++)
			{
				unit_seat_flags_yelo_strings[x] = flags_list->strings[x];
			}
			
			unit_seat_flags_yelo_strings[Flags::_unit_seat_reserved11_bit] = Shared::k_reserved_flag_string;
			unit_seat_flags_yelo_strings[Flags::_unit_seat_reserved12_bit] = Shared::k_reserved_flag_string;
			unit_seat_flags_yelo_strings[Flags::_unit_seat_reserved13_bit] = Shared::k_reserved_flag_string;
			unit_seat_flags_yelo_strings[Flags::_unit_seat_reserved14_bit] = Shared::k_reserved_flag_string;
			unit_seat_flags_yelo_strings[Flags::_unit_seat_reserved15_bit] = Shared::k_reserved_flag_string;
			unit_seat_flags_yelo_strings[Flags::_unit_seat_reserved16_bit] = Shared::k_reserved_flag_string;
			unit_seat_flags_yelo_strings[Flags::_unit_seat_reserved17_bit] = Shared::k_reserved_flag_string;
			unit_seat_flags_yelo_strings[Flags::_unit_seat_reserved18_bit] = Shared::k_reserved_flag_string;
			unit_seat_flags_yelo_strings[Flags::_unit_seat_reserved19_bit] = Shared::k_reserved_flag_string;
			unit_seat_flags_yelo_strings[Flags::_unit_seat_allows_melee_bit] = "allows melee";

			flags_list->strings = unit_seat_flags_yelo_strings;
			flags_list->count = NUMBEROF(unit_seat_flags_yelo_strings);
		}
		
		static void InitializeUnitSeatExtension(tag_block_definition* unit_block)
		{
			TAG_GROUP_STRING_TABLE_DEFINE(unit_keyframe_index, 3,
				"primary",
				"secondary",
				"final");

			TAG_GROUP_STRING_TABLE_DEFINE(unit_seat_extensions_flags, 10,
				"requires target seat occupied",
				"triggers mounted state",
				"exit on unit death",
				"exit on target seat empty",
				"ignored by seated ai",
				"ignored by mounted ai",
				"restrict by unit sight",
				"restrict by unit shield",
				"restrict by unit health",
				"restrict by ai state");

			TAG_GROUP_STRING_TABLE_DEFINE(unit_seat_extensions_ai_state_flags, 12,
				"none",
				"sleeping",
				"alert",
				"moving repeat same position",
				"moving loop",
				"moving loop back and forth",
				"moving loop randomly",
				"moving randomly",
				"guarding",
				"guarding at guard position",
				"searching",
				"fleeing");


			TAG_GROUP_STRING_TABLE_DEFINE(unit_seat_keyframe_action_flags, 1,
				"control powered seat");

			TAG_GROUP_STRING_TABLE_DEFINE(unit_seat_keyframe_action_self_seat_action, 3,
				"none",
				"exit current seat",
				"enter target seat");

			TAG_GROUP_STRING_TABLE_DEFINE(unit_seat_keyframe_action_target_seat_unit_action, 3,
				"none",
				"exit seat",
				"eject from seat",);

			TAG_GROUP_STRING_TABLE_DEFINE(unit_seat_keyframe_action_apply_effect, 3,
				"none",
				"mounted unit",
				"seated unit");


			TAG_GROUP_STRING_TABLE_DEFINE(unit_seat_boarding_type, 2,
				"immediate",
				"delayed");

			TAG_GROUP_STRING_TABLE_DEFINE(unit_seat_boarding_delay_until, 3,
				"empty target seat",
				"unit shield threshold",
				"unit health threshold");

			TAG_GROUP_STRING_TABLE_DEFINE(unit_seat_boarding_vitality_threshold_source, 2,
				"mounted unit",
				"seated unit");


			TAG_GROUP_STRING_TABLE_DEFINE(unit_seat_damage_melee, 3,
				"normal",
				"mounted unit",
				"target seat unit");

			TAG_GROUP_STRING_TABLE_DEFINE(unit_seat_damage_grenade, 4,
				"normal",
				"disabled",
				"plant on mounted unit",
				"plant on target seat unit");

			TAG_GROUP_STRING_TABLE_DEFINE(unit_seat_damage_flags, 2,
				"use weapon melee damage effect",
				"exit after grenade plant");
			
			TAG_GROUP_BLOCK_FIELDS_DEFINE(unit_seat_keyframe_action) =
			{
				TAG_FIELD_ENTRY(_field_enum, "keyframe", &unit_keyframe_index),
				TAG_FIELD_ENTRY(_field_word_flags, "flags", &unit_seat_keyframe_action_flags),
				TAG_FIELD_ENTRY(_field_enum, "self seat action", &unit_seat_keyframe_action_self_seat_action),
				TAG_FIELD_ENTRY(_field_enum, "target seat unit action", &unit_seat_keyframe_action_target_seat_unit_action),
				TAG_FIELD_ENTRY(_field_enum, "apply damage to", &unit_seat_keyframe_action_apply_effect),
				TAG_FIELD_ENTRY_PAD(2),
				TAG_FIELD_ENTRY(_field_tag_reference, "damage effect", &Shared::TAG_GROUP_REFERENCE_GET(damage_effect)),
				TAG_FIELD_ENTRY(_field_enum, "apply effect to", &unit_seat_keyframe_action_apply_effect),
				TAG_FIELD_ENTRY_PAD(2),
				TAG_FIELD_ENTRY(_field_tag_reference, "effect", &Shared::TAG_GROUP_REFERENCE_GET(effect)),
				TAG_FIELD_ENTRY(_field_string, "effect marker"),

				TAG_FIELD_ENTRY_END()
			};
			TAG_GROUP_BLOCK_DEFINE(unit_seat_keyframe_action, 0, 9,
				sizeof(unit_seat_keyframe_action)
			);

			TAG_GROUP_BLOCK_FIELDS_DEFINE(unit_seat_boarding) =
			{
				TAG_FIELD_ENTRY(_field_enum, "boarding type", &unit_seat_boarding_type),
				TAG_FIELD_ENTRY(_field_word_flags, "delay until", &unit_seat_boarding_delay_until),
				TAG_FIELD_ENTRY(_field_enum, "unit vitality source", &unit_seat_boarding_vitality_threshold_source),
				TAG_FIELD_ENTRY_PAD(2),
				TAG_FIELD_ENTRY(_field_real_fraction, "unit shield threshold"),
				TAG_FIELD_ENTRY(_field_real_fraction, "unit health threshold"),
				TAG_FIELD_ENTRY(_field_block, "keyframe actions", &TAG_GROUP_BLOCK_GET(unit_seat_keyframe_action)),

				TAG_FIELD_ENTRY_END()
			};
			TAG_GROUP_BLOCK_DEFINE(unit_seat_boarding, 0, 1,
				sizeof(unit_seat_boarding)
			);
			
			TAG_GROUP_BLOCK_FIELDS_DEFINE(unit_seat_damage) =
			{
				TAG_FIELD_ENTRY(_field_word_flags, "flags", &unit_seat_damage_flags),
				TAG_FIELD_ENTRY(_field_enum, "melee", &unit_seat_damage_melee),
				TAG_FIELD_ENTRY(_field_tag_reference, "melee damage effect", &Shared::TAG_GROUP_REFERENCE_GET(damage_effect)),
				TAG_FIELD_ENTRY(_field_enum, "grenade", &unit_seat_damage_grenade),
				TAG_FIELD_ENTRY(_field_real, "grenade detonation time"),
				TAG_FIELD_ENTRY(_field_string, "grenade marker"),

				TAG_FIELD_ENTRY_END()
			};
			TAG_GROUP_BLOCK_DEFINE(unit_seat_damage, 0, 1,
				sizeof(unit_seat_damage)
			);
			
			TAG_GROUP_BLOCK_FIELDS_DEFINE(unit_seat_targeting_seat) =
			{
				TAG_FIELD_ENTRY(_field_short_integer, "targeting seat", nullptr),

				TAG_FIELD_ENTRY_END()
			};
			TAG_GROUP_BLOCK_DEFINE(unit_seat_targeting_seat, 0, 16,
				sizeof(int16)
			);

			TAG_GROUP_BLOCK_FIELDS_DEFINE(unit_seat_extensions) =
			{
				TAG_FIELD_ENTRY(_field_word_flags, "flags", &unit_seat_extensions_flags),
				TAG_FIELD_ENTRY(_field_short_block_index, "target seat", nullptr),
				TAG_FIELD_ENTRY(_field_block, "", &TAG_GROUP_BLOCK_GET(unit_seat_targeting_seat)),
				TAG_FIELD_ENTRY(_field_angle, "unit sight angle"),
				TAG_FIELD_ENTRY(_field_real_fraction, "unit shield threshold"),
				TAG_FIELD_ENTRY(_field_real_fraction, "unit health threshold"),
				TAG_FIELD_ENTRY(_field_word_flags, "permitted ai states", &unit_seat_extensions_ai_state_flags),
				TAG_FIELD_ENTRY_PAD(2),
				TAG_FIELD_ENTRY_PAD(sizeof(tag_block)*2),
				TAG_FIELD_ENTRY(_field_block, "seat boarding", &TAG_GROUP_BLOCK_GET(unit_seat_boarding)),
				TAG_FIELD_ENTRY(_field_block, "seat damage", &TAG_GROUP_BLOCK_GET(unit_seat_damage)),
				TAG_FIELD_ENTRY_PAD(sizeof(tag_block)*4),

				TAG_FIELD_ENTRY_END()
			};
			TAG_GROUP_BLOCK_DEFINE(unit_seat_extensions, 0, 1,
				sizeof(unit_seat_extensions)
			);

			Shared::LinkBlockIndex<Enums::_field_short_block_index>(*unit_block, "seats", TAG_GROUP_BLOCK_GET(unit_seat_extensions), "target seat");

			auto& unit_seats_field = Shared::GetTagField(*unit_block, Enums::_field_block, "seats");
			auto* unit_seats_definition = unit_seats_field.DefinitionCast<tag_block_definition*>();

			FIELDSET_REPLACEMENT_BEGIN(unit_seat, 22+1, unit_seats_definition)
				FIELDSET_SEEK_AFTER_WITH_COPY("built-in gunner")
				// Halo2 defines the padding after the above field as useless padding, so we're good
				FIELDSET_INSERT_BEGIN()
					FIELDSET_INSERT_BLOCK(seat_extensions, "seat extensions", &TAG_GROUP_BLOCK_GET(unit_seat_extensions) )
				FIELDSET_INSERT_END()
			FIELDSET_REPLACEMENT_END();

			UnitSeatBlockUpdateFlags(unit_seats_definition);
		}

		static void InitializeUnitExtension(tag_block_definition* unit_block)
		{
			TAG_GROUP_STRING_TABLE_DEFINE(unit_keyframe_index, 3,
				"primary",
				"secondary",
				"final");

			TAG_GROUP_STRING_TABLE_DEFINE(unit_mounted_state_flags,1,
				"third person camera");

			TAG_GROUP_STRING_TABLE_DEFINE(unit_mounted_state_keyframe_action_flags, 1,
				"eject mounted units");

			TAG_GROUP_BLOCK_FIELDS_DEFINE(unit_mounted_state_keyframe_action) =
			{
				TAG_FIELD_ENTRY(_field_enum, "keyframe", &unit_keyframe_index),
				TAG_FIELD_ENTRY(_field_word_flags, "flags", &unit_mounted_state_keyframe_action_flags),
				TAG_FIELD_ENTRY(_field_tag_reference, "damage effect", &Shared::TAG_GROUP_REFERENCE_GET(damage_effect)),
				TAG_FIELD_ENTRY(_field_tag_reference, "effect", &Shared::TAG_GROUP_REFERENCE_GET(effect)),
				TAG_FIELD_ENTRY(_field_string, "effect marker"),

				TAG_FIELD_ENTRY_END()
			};
			TAG_GROUP_BLOCK_DEFINE(unit_mounted_state_keyframe_action, 0, 9,
				sizeof(unit_mounted_state_keyframe_action)
			);

			TAG_GROUP_BLOCK_FIELDS_DEFINE(unit_mounted_state) =
			{
				TAG_FIELD_ENTRY(_field_word_flags, "flags", &unit_mounted_state_flags),
				TAG_FIELD_ENTRY_PAD(2),
				TAG_FIELD_ENTRY(_field_string, "camera marker"),
				TAG_FIELD_ENTRY(_field_tag_reference, "camera track", &Shared::TAG_GROUP_REFERENCE_GET(camera_track)),
				TAG_FIELD_ENTRY(_field_block, "keyframe actions", &TAG_GROUP_BLOCK_GET(unit_mounted_state_keyframe_action)),

				TAG_FIELD_ENTRY_END()
			};
			TAG_GROUP_BLOCK_DEFINE(unit_mounted_state, 0, 1,
				sizeof(unit_mounted_state)
			);

			TAG_GROUP_BLOCK_FIELDS_DEFINE(unit_extensions) =
			{
				TAG_FIELD_ENTRY(_field_block, "mounted state", &TAG_GROUP_BLOCK_GET(unit_mounted_state)),
				TAG_FIELD_ENTRY_PAD(sizeof(tag_block)*4),
				TAG_FIELD_ENTRY_END()
			};
			TAG_GROUP_BLOCK_DEFINE(unit_extensions, 0, 1,
				sizeof(unit_extensions)
			);

			auto& tag_field = unit_block->fields[k_unit_extensions_pad_field_index];
			tag_field.name = "unit extensions";
			tag_field.type = Enums::_field_block;
			tag_field.definition = &TAG_GROUP_BLOCK_GET(unit_extensions);
		}

		static void Initialize()
		{
			tag_group* unit = blam::tag_group_get<s_unit_definition>();
			assert(unit);
			
			unit->postprocess_proc = &UnitPostprocess;

			InitializeUnitExtension(unit->header_block_definition);
			InitializeUnitSeatExtension(unit->header_block_definition);
		}
	};};};
};