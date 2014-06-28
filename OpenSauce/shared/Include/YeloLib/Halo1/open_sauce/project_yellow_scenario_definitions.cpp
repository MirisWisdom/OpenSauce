/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#include "Common/Precompile.hpp"
#include <YeloLib/Halo1/open_sauce/project_yellow_scenario_definitions.hpp>

#include <blamlib/Halo1/cseries/errors.hpp>
#include <blamlib/Halo1/tag_files/tag_groups.hpp>
#include <blamlib/Halo1/scenario/scenario_definitions.hpp>
#include <YeloLib/Halo1/hs/hs_yelo.hpp>

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>

namespace Yelo
{
	BOOST_STATIC_ASSERT(sizeof(boost::uuids::uuid) == Enums::k_uuid_buffer_size);

	namespace TagGroups
	{
		bool s_project_yellow_scenario_build_info::HasUuid() const
		{
			return !CAST_PTR(const boost::uuids::uuid&, uuid_buffer).is_nil();
		}

		void s_project_yellow_scenario_build_info::GenerateUuid()
		{
			auto uuid_generator = boost::uuids::random_generator();

			CAST_PTR(boost::uuids::uuid&, uuid_buffer) = uuid_generator();
		}

		//////////////////////////////////////////////////////////////////////////
		// project_yellow
		cstring project_yellow::k_default_name = "i've got a lovely bunch of corncobs";

		bool project_yellow::_physics::IsGravityScaleValid() const
		{
			return gravity_scale >= 0.0f || gravity_scale <= 2.0f;
		}
		void project_yellow::_physics::ResetGravityScale()
		{
			gravity_scale = 1.0f;
		}

		bool project_yellow::_physics::IsPlayerSpeedScaleValid() const
		{
			return gravity_scale >= 0.0f || gravity_scale <= 6.0f;
		}
		void project_yellow::_physics::ResetPlayerSpeedScale()
		{
			player_speed_scale = 1.0f;
		}

		bool project_yellow::IsNull() const				{ return flags.null_definition_bit != false; }
		bool project_yellow::IsCacheProtected() const	{ return flags.cache_is_protected_bit != false; }
		//////////////////////////////////////////////////////////////////////////

#if PLATFORM_IS_EDITOR
		bool PLATFORM_API project_yellow::GroupPostprocess(datum_index tag_index, Enums::tag_postprocess_mode mode)
		{
			auto* def = blam::tag_get<project_yellow>(tag_index);

			def->version = project_yellow::k_version;

			Scripting::ScriptingBlockClear(def->user_scripting);
			if (mode == Enums::_tag_postprocess_mode_for_runtime)
			{
				if (!def->physics.IsGravityScaleValid() || def->physics.gravity_scale == 0.0f)			def->physics.ResetGravityScale();
				if (!def->physics.IsPlayerSpeedScaleValid() || def->physics.player_speed_scale == 0.0f)	def->physics.ResetPlayerSpeedScale();

				Scripting::ScriptingBlockAddDefinitions(def->user_scripting, false);
			}

			return true;
		}

		void YeloCleanseScenario(scenario* scnr)
		{
			YELO_ASSERT(scnr != nullptr);

			// Clear the yelo reference
			tag_reference& yelo_reference = scnr->GetYeloReferenceHack();
			if (yelo_reference.group_tag == project_yellow::k_group_tag)
				blam::tag_reference_clear(yelo_reference);

			// If the scenario is using upgraded script node sizes, clear it
			// Users will need to recompile their scenario's scripts
			tag_data& hs_syntax_data = scnr->hs_syntax_data;
			if ((size_t)hs_syntax_data.size > Scripting::GetTotalScenarioHsSyntaxData())
			{
				tag_data_delete(&hs_syntax_data); // If hs_syntax_data.size != GetTotalScenarioHsSyntaxData, the engine will recompile the scripts
				YELO_WARN("CheApe: '%s' was cleansed but its scripts will need to be recompiled in the stock Sapien before a map can be built");
			}
		}
#endif
	};
};