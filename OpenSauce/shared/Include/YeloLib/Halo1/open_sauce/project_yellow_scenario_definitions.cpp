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
#include <YeloLib/Halo1/open_sauce/project_yellow_global_definitions.hpp>

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

		bool project_yellow::IsNull() const				{ return TEST_FLAG(flags, Flags::_project_yellow_null_definition_bit); }
		bool project_yellow::IsCacheProtected() const	{ return TEST_FLAG(flags, Flags::_project_yellow_cache_is_protected_bit); }
		//////////////////////////////////////////////////////////////////////////

#if PLATFORM_IS_EDITOR
		datum_index project_yellow::LoadYeloGlobals(const bool for_build_cache)
		{
			datum_index yelo_globals_index = datum_index::null;

			// check if the Yelo definition has a Yelo Globals tag reference and load it
			if (blam::tag_reference_resolve<project_yellow_globals>(yelo_globals))
				yelo_globals_index = yelo_globals.tag_index;
			else if (for_build_cache) // Only use the internal tag for cache-building only, not for editing
			{
				yelo_globals_index = blam::tag_new<project_yellow_globals>(project_yellow_globals::k_default_name);

				if (!yelo_globals_index.IsNull())
					blam::tag_reference_set<project_yellow_globals>(yelo_globals, project_yellow_globals::k_default_name);
			}

			return yelo_globals_index;
		}

		bool project_yellow::LoadGameGlobalsOverride()
		{
			if (game_globals.name_length == 0)
				return false;

			if (!blam::tag_reference_resolve(&game_globals))
				return false;

			// if there is a globals tag override, rename it to K_GAME_GLOBALS_TAG_NAME so
			// we avoid further hacks in the runtime engine code as it also explicitly 
			// looks for K_GAME_GLOBALS_TAG_NAME

			blam::tag_rename(game_globals.tag_index, Scenario::K_GAME_GLOBALS_TAG_NAME);

			// re-set the reference just for good measure
			blam::tag_reference_set(game_globals,
				game_globals.group_tag, Scenario::K_GAME_GLOBALS_TAG_NAME);

			return true;
		}

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
#endif
	};
};