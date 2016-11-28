/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <YeloLib/tag_files/tag_groups_base_yelo.hpp>
#include <blamlib/Halo1/game/game.hpp>

namespace Yelo
{
	namespace Enums
	{
		enum game_team : _enum;
		enum actor_default_state : _enum;
		enum actor_action : _enum;
		enum unit_animation_keyframe : _enum;

		enum actor_variant_transform_keyframe_rider_handling : _enum
		{
			_actor_variant_transform_keyframe_rider_handling_none,
			_actor_variant_transform_keyframe_rider_handling_kill,
			_actor_variant_transform_keyframe_rider_handling_eject,

			_actor_variant_transform_keyframe_rider_handling
		};

		enum actor_variant_transform_keyframe_effect_target : _enum
		{
			_actor_variant_transform_keyframe_effect_target_self,
			_actor_variant_transform_keyframe_effect_target_riders,

			_actor_variant_transform_keyframe_effect_target
		};

		enum actor_variant_transform_team_handling : _enum
		{
			_actor_variant_transform_team_handling_inherit_from_old_actor,
			_actor_variant_transform_team_handling_inherit_from_attacker,
			_actor_variant_transform_team_handling_override,

			_actor_variant_transform_team
		};

		enum actor_variant_transform_in_actor_state_handling : _enum
		{
			_actor_variant_transform_in_actor_state_handling_inherit,
			_actor_variant_transform_in_actor_state_handling_override,
			_actor_variant_transform_in_actor_state_handling_actor_default,

			_actor_variant_transform_in_actor_state_handling
		};

		enum actor_variant_transform_in_actor_action_handling : _enum
		{
			_actor_variant_transform_in_actor_action_handling_default,
			_actor_variant_transform_in_actor_action_handling_inherit,
			_actor_variant_transform_in_actor_action_handling_override,

			_actor_variant_transform_in_actor_action_handling
		};

		enum actor_variant_transform_in_encounter_squad_handling : _enum
		{
			_actor_variant_transform_in_encounter_squad_handling_inherit_from_old_actor,
			_actor_variant_transform_in_encounter_squad_handling_inherit_from_attacker,
			_actor_variant_transform_in_encounter_squad_handling_free_actor,

			_actor_variant_transform_in_encounter_squad_handling
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

		enum actor_variant_transform_out_damage_type : _enum
		{
			_actor_variant_transform_out_damage_type_both,
			_actor_variant_transform_out_damage_type_weapon_only,
			_actor_variant_transform_out_damage_type_melee_only,

			_actor_variant_transform_out_damage_type
		};
	};

	namespace Flags
	{
		enum actor_variant_transform_collection_transform_flags
		{
			_actor_variant_transform_collection_transform_flags_scripted_only,

			_actor_variant_transform_collection_transform_flags
		};

		enum actor_variant_transform_in_target_flags
		{
			_actor_variant_transform_in_target_flags_try_to_use_existing_unit,
			_actor_variant_transform_in_target_flags_drop_weapon,
			_actor_variant_transform_in_target_flags_inherit_seated_units,
			_actor_variant_transform_in_target_flags_delete_attached_actors,

			_actor_variant_transform_in_target_flags
		};

		enum actor_variant_transform_out_attachment_flags
		{
			_actor_variant_transform_out_attachment_flags_destroy_attachments_on_death,

			_actor_variant_transform_out_attachment_flags
		};
		
		enum actor_variant_transform_out_flags
		{
			_actor_variant_transform_out_flags_invicible_during_transform_out_bit,

			_actor_variant_transform_out_flags
		};

		enum actor_variant_transform_out_criteria_flags
		{
			_actor_variant_transform_out_criteria_flags_transform_on_damage_only_bit,
			_actor_variant_transform_out_criteria_flags_ignore_friendly_fire_bit,
			_actor_variant_transform_out_criteria_flags_transform_on_actor_action_bit,
			_actor_variant_transform_out_criteria_flags_transform_on_actor_state_bit,
			_actor_variant_transform_out_criteria_flags_transform_on_shield_range_bit,
			_actor_variant_transform_out_criteria_flags_transform_on_health_range_bit,

			_actor_variant_transform_out_criteria_flags
		};
	};

	namespace TagGroups
	{
		struct actor_variant_transform_keyframe_action
		{
			TAG_FIELD(Enums::unit_animation_keyframe, keyframe);
			TAG_FIELD(Enums::actor_variant_transform_keyframe_rider_handling, rider_handling);
			TAG_FIELD(Enums::actor_variant_transform_keyframe_effect_target, target);
			PAD16;
			TAG_FIELD(tag_reference, damage_effect);
			TAG_FIELD(tag_reference, effect);
			TAG_FIELD(tag_string, effect_marker);
		};

#pragma region actor_variant_transform_in_definition
		struct actor_variant_transform_in_target
		{
			TAG_FIELD(tag_string, target_name);
			TAG_FIELD(word_flags, flags);
			PAD16;
			TAG_FIELD(real_fraction, selection_chance)[Enums::k_number_of_game_difficulty_levels];

			TAG_FIELD(tag_reference, actor_variant);
			TAG_FIELD(Enums::actor_variant_transform_in_encounter_squad_handling, encounter_squad_handling);
			PAD16;
			TAG_FIELD(Enums::actor_variant_transform_team_handling, team_handling);
			TAG_FIELD(Enums::game_team, team_override);
			TAG_FIELD(Enums::actor_variant_transform_in_actor_state_handling, initial_state_handling);
			TAG_FIELD(Enums::actor_default_state, initial_state_override);
			TAG_FIELD(Enums::actor_variant_transform_in_actor_state_handling, return_state_handling);
			TAG_FIELD(Enums::actor_default_state, return_state_override);
			PAD16; // TAG_FIELD(Enums::actor_variant_transform_in_actor_action_handling, actor_action_handling);
			PAD16; // TAG_FIELD(Enums::actor_action, actor_action_override);

			TAG_FIELD(tag_string, transform_in_anim);
			TAG_TBLOCK(keyframe_actions, actor_variant_transform_keyframe_action);

			PAD32;
			TAG_FIELD(Enums::actor_variant_transform_in_vitality_handling, vitality_inheritance);
			TAG_FIELD(Enums::actor_variant_transform_in_vitality_handling, vitality_override);
			TAG_FIELD(real, shield_override);
			TAG_FIELD(real, health_override);
			TAG_PAD(tag_block, 2);
		}; BOOST_STATIC_ASSERT(sizeof(actor_variant_transform_in_target) == 0xAC);

		struct actor_variant_transform_in_definition
		{
			enum { k_group_tag = 'avti' };

			TAG_TBLOCK(targets, actor_variant_transform_in_target);
			TAG_PAD(tag_block, 2);
		}; BOOST_STATIC_ASSERT(sizeof(actor_variant_transform_in_definition) == 0x24);
#pragma endregion
		
#pragma region actor_variant_transform_out_definition
		struct actor_variant_transform_out_instigator
		{
			TAG_FIELD(tag_reference, unit);
			TAG_FIELD(Enums::actor_variant_transform_out_damage_type, damage_type);
			PAD16;
			TAG_PAD(tag_block, 1);
		}; BOOST_STATIC_ASSERT(sizeof(actor_variant_transform_out_instigator) == 0x20);

		struct actor_variant_transform_out_attachment
		{
			TAG_FIELD(tag_reference, object_type);
			TAG_FIELD(tag_string, object_marker);
			TAG_FIELD(tag_string, destination_marker);
			TAG_FIELD(int16, destination_marker_count);
			PAD16;
			TAG_FIELD(Enums::actor_variant_transform_team_handling, team_handling);
			TAG_FIELD(Enums::game_team, team_override);
			TAG_FIELD(real_bounds, attachment_scale);
			TAG_PAD(tag_block, 2);
		}; BOOST_STATIC_ASSERT(sizeof(actor_variant_transform_out_attachment) == 0x78);

		struct actor_variant_transform_out_definition
		{
			enum { k_group_tag = 'avto' };

			TAG_FIELD(word_flags, flags);
			PAD16;

			TAG_FIELD(word_flags, criteria_flags);
			PAD16;
			TAG_FIELD(word_flags, actor_action);
			TAG_FIELD(word_flags, actor_state);
			TAG_FIELD(real_bounds, shield_range);
			TAG_FIELD(real_bounds, health_range);
			TAG_PAD(tag_block, 2);

			TAG_TBLOCK(instigators, actor_variant_transform_out_instigator);

			TAG_FIELD(tag_string, transform_out_anim);
			TAG_TBLOCK(keyframe_actions, actor_variant_transform_keyframe_action);

			TAG_FIELD(word_flags, attachment_flags);
			PAD16;
			TAG_TBLOCK(attachments, actor_variant_transform_out_attachment);

			TAG_PAD(tag_block, 2);
		}; BOOST_STATIC_ASSERT(sizeof(actor_variant_transform_out_definition) == 0x94);
#pragma endregion

#pragma region actor_variant_transform_collection_definition

		struct actor_variant_transform_collection_transform
		{
			TAG_FIELD(word_flags, flags);
			PAD16;
			TAG_FIELD(tag_string, transform_name);
			TAG_FIELD(real_fraction, selection_chance)[Enums::k_number_of_game_difficulty_levels];

			TAG_FIELD(tag_reference, transform_out);
			actor_variant_transform_out_definition* transform_out_ptr;
			TAG_FIELD(tag_reference, transform_in);
			actor_variant_transform_in_definition* transform_in_ptr;
			TAG_PAD(tag_block, 2);
		}; BOOST_STATIC_ASSERT(sizeof(actor_variant_transform_collection_transform) == 0x74);

		struct actor_variant_transform_collection_entry
		{
			TAG_FIELD(tag_reference, actor_variant);
			TAG_TBLOCK_(transforms, actor_variant_transform_collection_transform);
			TAG_PAD(tag_block, 2);
		}; BOOST_STATIC_ASSERT(sizeof(actor_variant_transform_collection_entry) == 0x34);

		struct actor_variant_transform_collection_definition
		{
			enum { k_group_tag = 'avtc' };

			TAG_TBLOCK_(actor_variant_transforms, actor_variant_transform_collection_entry);
			TAG_PAD(tag_block, 2);
		}; BOOST_STATIC_ASSERT(sizeof(actor_variant_transform_collection_definition) == 0x24);
#pragma endregion
	};
};