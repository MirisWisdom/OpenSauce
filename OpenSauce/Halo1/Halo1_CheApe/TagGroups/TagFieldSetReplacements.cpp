/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#include "Common/Precompile.hpp"
#include "TagGroups/TagGroups.hpp"

#include <blamlib/Halo1/tag_files/tag_groups_structures_macros.hpp>
#include <YeloLib/Halo1/tag_files/tag_fieldset_replacement_builder.hpp>

#include <blamlib/Halo1/bitmaps/bitmap_group.hpp>
#include <blamlib/Halo1/effects/damage_effect_definitions.hpp>
#include <blamlib/Halo1/models/model_definitions.hpp>
#include <blamlib/Halo1/shaders/shader_definitions.hpp>
#include <blamlib/Halo1/sound/sound_definitions.hpp>
#include <blamlib/Halo1/units/unit_definitions.hpp>

namespace Yelo
{
	namespace TagGroups
	{
		static cstring k_reserved_flag_string = "reserved"
			TAG_FIELD_MARKUP_IS_READONLY;
		static cstring k_never_share_resources_flag_string = "never share resources"
			TAG_FIELD_MARKUP_PREFIX_HELP
			"";

		TAG_GROUP_REFERENCE_DEFINE(damage_effect, 0,
			s_damage_effect_definition::k_group_tag
		);

		template<Enums::field_type kFieldType>
		static string_list* FindFlagsField(tag_block_definition* definition, cstring flags_field_name = "flags")
		{
			static_assert(kFieldType == Enums::_field_byte_flags || kFieldType == Enums::_field_word_flags || kFieldType == Enums::_field_long_flags,
				"Expected a flags-based field type");
			assert(definition);

			int flags_field_index = definition->FindFieldIndex(kFieldType, flags_field_name);
			assert(flags_field_index != NONE);

			auto flags_list = definition->fields[flags_field_index].Definition<string_list>();

			return flags_list;
		}
		template<size_t kStartIndex, size_t kEndIndex, rsize_t kStringsCount>
		static void PopulateWithReservedString(cstring (& strings)[kStringsCount])
		{
			BOOST_STATIC_ASSERT(kStartIndex < kStringsCount);
			BOOST_STATIC_ASSERT(kEndIndex < kStringsCount);

			for (size_t x = kStartIndex; x <= kEndIndex; x++)
				strings[x] = k_reserved_flag_string;
		}

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
			auto* flags_list = FindFlagsField<Enums::_field_long_flags>(unit_seat_block);
			assert(flags_list->count==Flags::k_number_of_unit_seat_definition_flags);

			static cstring unit_seat_flags_yelo_strings[Flags::k_number_of_unit_seat_definition_flags_yelo];
			for (int x = 0; x < Flags::k_number_of_unit_seat_definition_flags; x++)
				unit_seat_flags_yelo_strings[x] = flags_list->strings[x];

			unit_seat_flags_yelo_strings[Flags::_unit_seat_boarding_seat_bit] =
				"boarding seat";
			unit_seat_flags_yelo_strings[Flags::_unit_seat_reserved12_bit] =
				k_reserved_flag_string;
			unit_seat_flags_yelo_strings[Flags::_unit_seat_boarding_enters_seat_bit] =
				"boarding enters seat";
			unit_seat_flags_yelo_strings[Flags::_unit_seat_boarding_need_any_passenger_bit] =
				"boarding need any passenger";
			unit_seat_flags_yelo_strings[Flags::_unit_seat_controls_open_and_close_bit] =
				"controls open and close";

			PopulateWithReservedString<Flags::_unit_seat_reserved16_bit, Flags::_unit_seat_reserved19_bit>(
				unit_seat_flags_yelo_strings);

			unit_seat_flags_yelo_strings[Flags::_unit_seat_boarding_ejects_seat_yelo_bit] =
				"boarding ejects seat";

			flags_list->strings = unit_seat_flags_yelo_strings;
			flags_list->count = NUMBEROF(unit_seat_flags_yelo_strings);
		}
		static void InitializeFieldSetReplacementsForUnitSeat(tag_block_definition* unit_seat_block)
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
								&TAG_GROUP_REFERENCE_GET(damage_effect) ),
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
		static void InitializeFieldSetReplacementsForUnit()
		{
			tag_group* unit = blam::tag_group_get<s_unit_definition>();
			assert(unit);

			auto* unit_header_block = unit->header_block_definition;

			// mark the unit's standard postprocess proc, then set it to our overload
			g_unit_header_block_postprocess_proc = unit_header_block->postprocess_proc;
			unit_header_block->postprocess_proc = UnitHeaderBlockPostprocessOverload;

			int unit_seats_field_index = unit_header_block->FindFieldIndex(Enums::_field_block, "seats");
			assert(unit_seats_field_index != NONE);

			InitializeFieldSetReplacementsForUnitSeat(
				unit->header_block_definition->fields[unit_seats_field_index].DefinitionCast<tag_block_definition*>());
		}

		static void BitmapGroupUpdateFlags(tag_block_definition* bitmap_block)
		{
			auto* flags_list = FindFlagsField<Enums::_field_word_flags>(bitmap_block);
			assert(flags_list->count==Flags::k_number_of_bitmap_group_flags);

			static cstring bitmap_group_flags_yelo_strings[Flags::k_number_of_bitmap_group_flags_yelo];
			for (int x = 0; x < Flags::k_number_of_bitmap_group_flags; x++)
				bitmap_group_flags_yelo_strings[x] = flags_list->strings[x];

			PopulateWithReservedString<Flags::_bitmap_group_reserved4_bit, Flags::_bitmap_group_reserved12_bit>(
				bitmap_group_flags_yelo_strings);

			bitmap_group_flags_yelo_strings[Flags::_bitmap_group_never_share_resources_yelo_bit] =
				k_never_share_resources_flag_string;

			flags_list->strings = bitmap_group_flags_yelo_strings;
			flags_list->count = NUMBEROF(bitmap_group_flags_yelo_strings);
		}
		static void InitializeFieldSetReplacementsForBitmap()
		{
			tag_group* bitmap = blam::tag_group_get<s_bitmap_group>();
			assert(bitmap);

			BitmapGroupUpdateFlags(bitmap->header_block_definition);
		}

		static void SoundUpdateFlags(tag_block_definition* sound_block)
		{
			auto* flags_list = FindFlagsField<Enums::_field_long_flags>(sound_block);
			assert(flags_list->count==Flags::k_number_of_sound_definition_flags);

			static cstring sound_definition_flags_yelo_strings[Flags::k_number_of_sound_definition_flags_yelo];
			for (int x = 0; x < Flags::k_number_of_sound_definition_flags; x++)
				sound_definition_flags_yelo_strings[x] = flags_list->strings[x];

			PopulateWithReservedString<Flags::_sound_definition_reserved2_bit, Flags::_sound_definition_reserved9_bit>(
				sound_definition_flags_yelo_strings);

			sound_definition_flags_yelo_strings[Flags::_sound_definition_never_share_resources_yelo_bit] =
				k_never_share_resources_flag_string;

			flags_list->strings = sound_definition_flags_yelo_strings;
			flags_list->count = NUMBEROF(sound_definition_flags_yelo_strings);
		}
		static void InitializeFieldSetReplacementsForSound()
		{
			tag_group* sound = blam::tag_group_get<sound_definition>();
			assert(sound);

			SoundUpdateFlags(sound->header_block_definition);
		}

		static void GbxModelGeometryPartBlockFixActualFields(tag_block_definition* model_geometry_part_block)
		{
			assert(0==strcmp("gbxmodel_geometry_part_block", model_geometry_part_block->name));

			// find the last declared field
			int field_index = model_geometry_part_block->FindFieldIndex(Enums::_field_block, "triangles");
			assert(field_index != NONE);

			// move to triangle_buffer
			field_index++;
			assert(model_geometry_part_block->fields[field_index].type==Enums::_field_pad);

			// move to vertex_buffer
			field_index++;
			assert(model_geometry_part_block->fields[field_index].type==Enums::_field_pad);

			// move to num_nodes
			// fucking gbx defines num_nodes as pad, pretending it is a runtime field, but it isn't calculated at runtime
			field_index += 4;
			assert(model_geometry_part_block->fields[field_index].type == Enums::_field_pad);
			{
				tag_field& num_nodes_field = model_geometry_part_block->fields[field_index];
				assert(num_nodes_field.DefinitionCast<int32>()==sizeof(sbyte));

				num_nodes_field.type = Enums::_field_char_integer;
				num_nodes_field.name = "num_nodes"
					TAG_FIELD_MARKUP_IS_READONLY;
				num_nodes_field.definition = nullptr;
			}

			// move to node_table
			field_index++;
			assert(model_geometry_part_block->fields[field_index].type == Enums::_field_pad);
			{
				const int32 k_node_table_field_pad_size =
					(sizeof(sbyte) * Enums::k_maximum_nodes_per_model_geometry_part) +
					sizeof(int16);

				tag_field& node_table_field = model_geometry_part_block->fields[field_index];
				assert(node_table_field.DefinitionCast<int32>() == k_node_table_field_pad_size);

				node_table_field.type = Enums::_field_skip;
			}
		}
		static void InitializeFieldSetReplacementsForGbxModel()
		{
			tag_group* gbxmodel = blam::tag_group_get<gbxmodel_definition>();
			assert(gbxmodel);

			auto* block = gbxmodel->header_block_definition;
			// find gbxmodel->geometries
			int field_index = block->FindFieldIndex(Enums::_field_block, "geometries");
			assert(field_index != NONE);

			block = block->fields[field_index].Definition<tag_block_definition>();
			// find gbxmodel_geometry->parts
			field_index = block->FindFieldIndex(Enums::_field_block, "parts");
			assert(field_index != NONE);

			block = block->fields[field_index].Definition<tag_block_definition>();

			GbxModelGeometryPartBlockFixActualFields(block);
		}

		static void InitializeFieldSetReplacementsForShaderEnvironment()
		{
			tag_group* senv = blam::tag_group_get<s_shader_environment_definition>();

			TAG_GROUP_BLOCK_FIELDS_DEFINE(shader_environment_extension) =
			{
				// TODO: fields
				TAG_FIELD_ENTRY_END()
			};
			TAG_GROUP_BLOCK_DEFINE(shader_environment_extension, 0, 1,
				0//sizeof(s_shader_environment_extension)
			);

			FIELDSET_REPLACEMENT_BEGIN(_shader_environment_definition, 92+2, senv->header_block_definition)
				FIELDSET_SEEK_AFTER_WITH_COPY("lens flare")
				FIELDSET_INSERT_BEGIN()
					FIELDSET_ADD_EXPLANATION("replacement test", "")
					FIELDSET_INSERT_BLOCK(shader_extension,
						"yelo extension" 
							TAG_FIELD_MARKUP_IS_READONLY, // TODO: readonly until the block actually has fields
						&TAG_GROUP_BLOCK_GET(shader_environment_extension) )
				FIELDSET_INSERT_END()
			FIELDSET_REPLACEMENT_END();
		}
		static void DamageEffectGroupChangeInstantaneouAccelerationTo3d(tag_block_definition* damage_header_block)
		{
			tag_field* fields = damage_header_block->fields;
			int field_index = damage_header_block->FindFieldIndex(Enums::_field_real, "instantaneous acceleration");
			assert(field_index != NONE);

			fields[field_index++].type = Enums::_field_real_vector_3d;

			// "null" the old padding. It's an error to define zero length padding, so we pretend to be custom
			for (int x = 0; x < 2; x++)
			{
				assert(fields[field_index].type == Enums::_field_pad);
				fields[field_index].type = Enums::_field_custom;
				fields[field_index++].definition = 0;
			}
		}
		static void InitializeFieldSetReplacementsForDamageEffects()
		{
			tag_group* jpt_ = blam::tag_group_get<s_damage_effect_definition>();
			tag_group* cdmg = blam::tag_group_get<s_continuous_damage_effect_definition>();
			assert(jpt_ && cdmg);

			DamageEffectGroupChangeInstantaneouAccelerationTo3d(jpt_->header_block_definition);
			DamageEffectGroupChangeInstantaneouAccelerationTo3d(cdmg->header_block_definition);

			// find the damage effect flags, then the actual damage flags (both have the same name)
			int flags_field_index = jpt_->header_block_definition->FindFieldIndex(Enums::_field_long_flags, "flags");
			assert(flags_field_index != NONE);
			flags_field_index = jpt_->header_block_definition->FindFieldIndex(Enums::_field_long_flags, "flags", flags_field_index+1);
			assert(flags_field_index != NONE);

			static cstring damage_flags_yelo_strings[Flags::k_number_of_damage_flags_yelo];
			auto flags_list = jpt_->header_block_definition->fields[flags_field_index].Definition<string_list>();
			assert(flags_list->count==Flags::k_number_of_damage_flags);

			for (int x = 0; x < Flags::k_number_of_damage_flags; x++)
				damage_flags_yelo_strings[x] = flags_list->strings[x];

			damage_flags_yelo_strings[Flags::_damage_use_3d_acceleration_yelo_bit] = 
				"YELO; 3D inst. accel."
					TAG_FIELD_MARKUP_PREFIX_HELP
					"factor in the J and K components instead of only I";

			flags_list->strings = damage_flags_yelo_strings;
			flags_list->count = NUMBEROF(damage_flags_yelo_strings);
		}
		void InitializeFieldSetReplacements()
		{
			// NOTE: call tag_field_set_replacement_builder's here

			InitializeFieldSetReplacementsForUnit();
			InitializeFieldSetReplacementsForBitmap();
			InitializeFieldSetReplacementsForSound();
			InitializeFieldSetReplacementsForGbxModel();
			InitializeFieldSetReplacementsForShaderEnvironment();
			InitializeFieldSetReplacementsForDamageEffects();
		}
	};
};