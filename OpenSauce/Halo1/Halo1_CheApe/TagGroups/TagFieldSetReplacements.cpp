/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#include "Common/Precompile.hpp"
#include "TagGroups/TagGroups.hpp"

#include "TagGroups/TagFieldReplacements/Shared.hpp"
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
		void InitializeFieldSetReplacements()
		{
			// NOTE: call tag_field_set_replacement_builder's here

			TagFieldReplacements::Unit::Initialize();
			TagFieldReplacements::Bitmap::Initialize();
			TagFieldReplacements::Sound::Initialize();
			TagFieldReplacements::GBXModel::Initialize();
			TagFieldReplacements::Shader::Initialize();
			TagFieldReplacements::DamageEffect::Initialize();
			TagFieldReplacements::Scenario::Initialize();
		}
	};
};