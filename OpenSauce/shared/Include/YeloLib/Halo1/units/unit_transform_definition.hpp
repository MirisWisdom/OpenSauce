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
		enum unit_transform_resulting_team : _enum
		{
			_unit_transform_resulting_team_unit_default,
			_unit_transform_resulting_team_player,
			_unit_transform_resulting_team_human,
			_unit_transform_resulting_team_covenant,
			_unit_transform_resulting_team_flood,
			_unit_transform_resulting_team_sentinel,
			_unit_transform_resulting_team_unused1,
			_unit_transform_resulting_team_unused2,
			_unit_transform_resulting_team_unused3,
			_unit_transform_resulting_team_unused4,
			_unit_transform_resulting_team_inherit_from_attacked,
			_unit_transform_resulting_team_inherit_from_attacker,

			_unit_transform_resulting_team
		};

		enum unit_transform_vitality_handling : _enum
		{
			_unit_transform_vitality_handling_neither,
			_unit_transform_vitality_handling_both,
			_unit_transform_vitality_handling_shield_only,
			_unit_transform_vitality_handling_health_only,

			_unit_transform_vitality_handling
		};
	};

	namespace Flags
	{
		enum unit_transform_target_flags
		{
			_unit_transform_target_flags_try_to_use_existing_unit,
			_unit_transform_target_flags_inherit_encounter_squad,
			_unit_transform_target_flags_drop_weapon,

			_unit_transform_target_flags
		};
	};

	namespace TagGroups
	{
		DOC_TODO("Struct size asserts");
		DOC_TODO("Padding");
		struct s_unit_transform_target
		{
			TAG_FIELD(tag_string, target_name);
			TAG_FIELD(word_flags, flags);
			TAG_FIELD(Enums::unit_transform_resulting_team, resulting_team);
			TAG_FIELD(tag_reference, actor_variant);
			TAG_FIELD(Enums::actor_default_state, actor_state);
			PAD16;
			TAG_FIELD(tag_reference, transform_effect);
			TAG_FIELD(tag_string, transform_in_anim);
			TAG_FIELD(Enums::unit_transform_vitality_handling, vitality_inheritance);
			TAG_FIELD(Enums::unit_transform_vitality_handling, vitality_override);
			TAG_FIELD(real, shield_override);
			TAG_FIELD(real, health_override);
		};

		struct s_unit_transform_attachment
		{
			TAG_FIELD(tag_reference, object_type);
			TAG_FIELD(tag_string, object_marker);
			TAG_FIELD(tag_string, destination_marker);
			TAG_FIELD(int16, destination_marker_count);
			PAD16;
		};

		struct s_unit_transform_definition
		{
			enum { k_group_tag = 'utrn' };

			TAG_TBLOCK(targets, s_unit_transform_target);
			TAG_TBLOCK(attachments, s_unit_transform_attachment);
		};
	};
};