/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#include "Common/Precompile.hpp"
#include "TagGroups/TagGroups.hpp"

#include <blamlib/Halo1/interface/hud_messaging_definitions.hpp>

#include "TagGroups/TagFieldReplacements/Shared.hpp"
#include "TagGroups/TagFieldReplacements/Object.hpp"
#include "TagGroups/TagFieldReplacements/Unit.hpp"
#include "TagGroups/TagFieldReplacements/Bitmap.hpp"
#include "TagGroups/TagFieldReplacements/Sound.hpp"
#include "TagGroups/TagFieldReplacements/GBXModel.hpp"
#include "TagGroups/TagFieldReplacements/Shader.hpp"
#include "TagGroups/TagFieldReplacements/DamageEffect.hpp"
#include "TagGroups/TagFieldReplacements/Scenario.hpp"

namespace Yelo
{
	namespace TagGroups
	{
		static void InitializeFieldSetReplacementsForHudMessageText()
		{
			tag_group* hmt_ = blam::tag_group_get<hud_state_messages>();
			assert(hmt_);

			auto* block = hmt_->header_block_definition;
			// find hud_state_messages->messages
			int field_index = block->FindFieldIndex(Enums::_field_block, "messages");
			assert(field_index != NONE);

			block = block->fields[field_index].Definition<tag_block_definition>();
			// find hud_state_message_definition->name
			field_index = block->FindFieldIndex(Enums::_field_string, "name");
			assert(field_index != NONE);

			// update the name field to also act as the block name
			block->fields[field_index].name = "name"
				TAG_FIELD_MARKUP_IS_READONLY
				TAG_FIELD_MARKUP_IS_BLOCK_NAME;
		}

		void InitializeFieldSetReplacements()
		{
			// NOTE: call tag_field_set_replacement_builder's here
			
			TagFieldReplacements::Object::Initialize();
			TagFieldReplacements::Unit::Initialize();
			TagFieldReplacements::Bitmap::Initialize();
			TagFieldReplacements::Sound::Initialize();
			TagFieldReplacements::GBXModel::Initialize();
			TagFieldReplacements::Shader::Initialize();
			TagFieldReplacements::DamageEffect::Initialize();
			TagFieldReplacements::Scenario::Initialize();
			InitializeFieldSetReplacementsForHudMessageText();
		}
	};
};