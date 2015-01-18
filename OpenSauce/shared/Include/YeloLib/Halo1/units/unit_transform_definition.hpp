/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <YeloLib/tag_files/tag_groups_base_yelo.hpp>

namespace Yelo
{
	namespace Enums
	{
		enum actor_variant_transform_in_team : _enum
		{
			_actor_variant_transform_in_team_unit_default,
			_actor_variant_transform_in_team_player,
			_actor_variant_transform_in_team_human,
			_actor_variant_transform_in_team_covenant,
			_actor_variant_transform_in_team_flood,
			_actor_variant_transform_in_team_sentinel,
			_actor_variant_transform_in_team_unused1,
			_actor_variant_transform_in_team_unused2,
			_actor_variant_transform_in_team_unused3,
			_actor_variant_transform_in_team_unused4,
			_actor_variant_transform_in_team_inherit_from_attacked,
			_actor_variant_transform_in_team_inherit_from_attacker,

			_actor_variant_transform_in_team
		};

		enum actor_variant_transform_in_vitality_handling : _enum
		{
			_actor_variant_transform_in_vitality_handling_neither,
			_actor_variant_transform_in_vitality_handling_both,
			_actor_variant_transform_in_vitality_handling_shield_only,
			_actor_variant_transform_in_vitality_handling_health_only,

			_actor_variant_transform_in_vitality_handling
		};

		enum actor_variant_transform_out_threshold_type : _enum
		{
			_actor_variant_transform_out_threshold_type_both,
			_actor_variant_transform_out_threshold_type_shield_amount_only,
			_actor_variant_transform_out_threshold_type_health_amount_only,

			_actor_variant_transform_out_threshold_type
		};

		enum actor_variant_transform_out_rider_handling : _enum
		{
			_actor_variant_transform_out_rider_handling_eject,
			_actor_variant_transform_out_rider_handling_kill,
			_actor_variant_transform_out_rider_handling_inherit,

			_actor_variant_transform_out_rider_handling
		};

		enum actor_variant_transform_collection_damage_type : _enum
		{
			_actor_variant_transform_collection_damage_type_both,
			_actor_variant_transform_collection_damage_type_weapon_only,
			_actor_variant_transform_collection_damage_type_melee_only,

			_actor_variant_transform_collection_damage_type
		};
	};

	namespace Flags
	{
		enum actor_variant_transform_in_target_flags
		{
			_actor_variant_transform_in_target_flags_try_to_use_existing_unit,
			_actor_variant_transform_in_target_flags_inherit_encounter_squad,
			_actor_variant_transform_in_target_flags_drop_weapon,

			_actor_variant_transform_in_target_flags
		};

		enum actor_variant_transform_in_attachment_flags
		{
			_actor_variant_transform_in_attachment_flags_delete_attachments_on_death,

			_actor_variant_transform_in_attachment_flags
		};

		enum actor_variant_transform_out_flags
		{
			_actor_variant_transform_out_flags_ignore_friendly_fire_bit,
			_actor_variant_transform_out_flags_invicible_during_transform_out_bit,

			_actor_variant_transform_out_flags
		};
	};

	namespace TagGroups
	{
		DOC_TODO("Struct size asserts");
		DOC_TODO("Padding");
#pragma region actor_variant_transform_in_definition
		struct actor_variant_transform_in_target
		{
			TAG_FIELD(tag_string, target_name);
			TAG_FIELD(word_flags, flags);
			TAG_FIELD(Enums::actor_variant_transform_in_team, resulting_team);
			TAG_FIELD(tag_reference, actor_variant);
			TAG_FIELD(Enums::actor_default_state, actor_state);
			PAD16;
			TAG_FIELD(tag_reference, transform_effect);
			TAG_FIELD(tag_string, transform_in_anim);
			TAG_FIELD(Enums::actor_variant_transform_in_vitality_handling, vitality_inheritance);
			TAG_FIELD(Enums::actor_variant_transform_in_vitality_handling, vitality_override);
			TAG_FIELD(real, shield_override);
			TAG_FIELD(real, health_override);
		};

		struct actor_variant_transform_in_attachment
		{
			TAG_FIELD(tag_reference, object_type);
			TAG_FIELD(tag_string, object_marker);
			TAG_FIELD(tag_string, destination_marker);
			TAG_FIELD(int16, destination_marker_count);
			TAG_FIELD(Enums::actor_variant_transform_in_team, attachment_team);
			TAG_FIELD(real_bounds, attachment_scale);
		};

		struct actor_variant_transform_in_definition
		{
			enum { k_group_tag = 'avti' };

			TAG_TBLOCK(targets, actor_variant_transform_in_target);
			TAG_FIELD(word_flags, flags);
			PAD16;
			TAG_TBLOCK(attachments, actor_variant_transform_in_attachment);
		};
#pragma endregion
		
#pragma region actor_variant_transform_out_definition
		struct actor_variant_transform_out_definition
		{
			enum { k_group_tag = 'avto' };

			TAG_FIELD(word_flags, flags);
			PAD16; //TAG_FIELD(Enums::unit_transform_rider_handling, rider_handling);
			TAG_FIELD(Enums::actor_variant_transform_out_threshold_type, threshold_type);
			PAD16;
			TAG_FIELD(real_fraction, shield_threshold);
			TAG_FIELD(real_fraction, health_threshold);
			TAG_FIELD(tag_string, transform_out_anim);
		};
#pragma endregion

#pragma region actor_variant_transform_collection_definition
		struct actor_variant_transform_collection_instigator
		{
			TAG_FIELD(tag_reference, unit);
			TAG_FIELD(Enums::actor_variant_transform_collection_damage_type, damage_type);
			PAD16;
		};

		struct actor_variant_transform_collection_transform
		{
			TAG_FIELD(tag_string, transform_name);
			TAG_FIELD(tag_reference, transform_out);
			actor_variant_transform_out_definition* transform_out_ptr;
			TAG_FIELD(tag_reference, transform_in);
			actor_variant_transform_in_definition* transform_in_ptr;
			TAG_TBLOCK(instigators, actor_variant_transform_collection_instigator);
		};

		struct actor_variant_transform_collection_entry
		{
			TAG_FIELD(tag_reference, actor_variant);
			TAG_TBLOCK_(transforms, actor_variant_transform_collection_transform);
		};

		struct actor_variant_transform_collection_definition
		{
			enum { k_group_tag = 'avtc' };

			TAG_TBLOCK_(actor_variant_transforms, actor_variant_transform_collection_entry);
		};
#pragma endregion
	};
};