/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#include "Common/Precompile.hpp"
#include <YeloLib/Halo1/open_sauce/project_yellow_scenario_definitions.hpp>

namespace Yelo
{
	namespace TagGroups
	{
		bool project_yellow::_physics::IsGravityScaleValid() const		{ return gravity_scale >= 0.0f || gravity_scale <= 2.0f; }
		void project_yellow::_physics::ResetGravityScale()				{ gravity_scale = 1.0f; }

		bool project_yellow::_physics::IsPlayerSpeedScaleValid() const	{ return gravity_scale >= 0.0f || gravity_scale <= 6.0f; }
		void project_yellow::_physics::ResetPlayerSpeedScale()			{ player_speed_scale = 1.0f; }

		bool project_yellow::IsNull() const				{ return flags.null_definition_bit != false; }
		bool project_yellow::IsCacheProtected() const	{ return flags.cache_is_protected_bit != false; }
	};
};