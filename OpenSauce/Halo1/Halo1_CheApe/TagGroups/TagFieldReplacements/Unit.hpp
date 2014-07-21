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
		static proc_tag_block_postprocess_element g_unit_header_block_postprocess_proc;

		static bool PLATFORM_API UnitHeaderBlockPostprocessOverload(void* element, Enums::tag_postprocess_mode mode,
			datum_index tag_index)
		{
			auto* unit_definition = CAST_PTR(s_unit_definition*, element);

			return	g_unit_header_block_postprocess_proc(element, mode, tag_index) &&
					unit_definition->Postprocess(mode, tag_index);
		}

		static void UnitSeatBlockUpdateFlags(tag_block_definition* unit_seat_block)
		{
			auto* flags_list = Shared::FindFlagsField<Enums::_field_long_flags>(unit_seat_block);
			assert(flags_list->count==Flags::k_number_of_unit_seat_definition_flags);

			static cstring unit_seat_flags_yelo_strings[Flags::k_number_of_unit_seat_definition_flags_yelo];
			for (int x = 0; x < Flags::k_number_of_unit_seat_definition_flags; x++)
				unit_seat_flags_yelo_strings[x] = flags_list->strings[x];

			unit_seat_flags_yelo_strings[Flags::_unit_seat_boarding_seat_bit] =
				"boarding seat";
			unit_seat_flags_yelo_strings[Flags::_unit_seat_reserved12_bit] =
				Shared::k_reserved_flag_string;
			unit_seat_flags_yelo_strings[Flags::_unit_seat_boarding_enters_seat_bit] =
				"boarding enters seat";
			unit_seat_flags_yelo_strings[Flags::_unit_seat_boarding_need_any_passenger_bit] =
				"boarding need any passenger";
			unit_seat_flags_yelo_strings[Flags::_unit_seat_controls_open_and_close_bit] =
				"controls open and close";

			Shared::PopulateWithReservedString<Flags::_unit_seat_reserved16_bit, Flags::_unit_seat_reserved19_bit>(
				unit_seat_flags_yelo_strings);

			unit_seat_flags_yelo_strings[Flags::_unit_seat_boarding_ejects_seat_yelo_bit] =
				"boarding ejects seat";

			flags_list->strings = unit_seat_flags_yelo_strings;
			flags_list->count = NUMBEROF(unit_seat_flags_yelo_strings);
		}

		static void InitializeUnitSeat(tag_block_definition* unit_seat_block)
		{
			using namespace Enums;

			const size_t k_unit_seat_boarding_target_seat_field_index = 2;
			TAG_GROUP_BLOCK_FIELDS_DEFINE(unit_seat_yelo_extensions) =
			{
				// unit_seat_boarding fields
				TAG_FIELD_ENTRY(_field_explanation,
								"BOARDING",
								""),
				TAG_FIELD_ENTRY_PAD(sizeof(int16)*1),
				TAG_FIELD_ENTRY(_field_short_block_index,
								"boarding seat"
									TAG_FIELD_MARKUP_PREFIX_HELP
									"the target seat to enter",
								nullptr), // NOTE: initialized later
				TAG_FIELD_ENTRY(_field_tag_reference,
								"boarding damage",
								&Shared::TAG_GROUP_REFERENCE_GET(damage_effect) ),
				TAG_FIELD_ENTRY_PAD(sizeof(int32)*3),

				TAG_FIELD_ENTRY_PAD(sizeof(int32)*20),
				TAG_FIELD_ENTRY_END()
			};
			TAG_GROUP_BLOCK_DEFINE(unit_seat_yelo_extensions, 0, 1,
				sizeof(unit_seat_yelo_extensions)
			);

			// NOTE: engine doesn't define a postprocess for unit_seat, so no need to define an overload
			unit_seat_block->postprocess_proc = unit_seat::Postprocess;

			UnitSeatBlockUpdateFlags(unit_seat_block);

			// Set unit_seat_boarding.target_seat_index's definition
			{
				auto* unit_seat_boarding_target_seat_field =
					&TAG_GROUP_BLOCK_FIELDS_GET(unit_seat_yelo_extensions)[k_unit_seat_boarding_target_seat_field_index];
				assert(unit_seat_boarding_target_seat_field->type == _field_short_block_index);

				unit_seat_boarding_target_seat_field->definition = unit_seat_block;
			};

			FIELDSET_REPLACEMENT_BEGIN(unit_seat, 22+1, unit_seat_block)
				FIELDSET_SEEK_AFTER_WITH_COPY("built-in gunner")
				// Halo2 defines the padding after the above field as useless padding, so we're good
				FIELDSET_INSERT_BEGIN()
					FIELDSET_INSERT_BLOCK(yelo_extensions,
						"yelo extensions",
						&TAG_GROUP_BLOCK_GET(unit_seat_yelo_extensions) )
				FIELDSET_INSERT_END()
			FIELDSET_REPLACEMENT_END();
		}

		static void Initialize()
		{
			tag_group* unit = blam::tag_group_get<s_unit_definition>();
			assert(unit);

			auto* unit_header_block = unit->header_block_definition;

			// mark the unit's standard postprocess proc, then set it to our overload
			g_unit_header_block_postprocess_proc = unit_header_block->postprocess_proc;
			unit_header_block->postprocess_proc = UnitHeaderBlockPostprocessOverload;

			int unit_seats_field_index = unit_header_block->FindFieldIndex(Enums::_field_block, "seats");
			assert(unit_seats_field_index != NONE);

			InitializeUnitSeat(
				unit->header_block_definition->fields[unit_seats_field_index].DefinitionCast<tag_block_definition*>());
		}
	};};};
};