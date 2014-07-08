/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include <YeloLib/Halo1/units/unit_definitions_yelo.hpp>

#include <blamlib/Halo1/units/unit_definitions.hpp>

namespace Yelo
{
	namespace TagGroups
	{
		bool unit_seat::HasYeloExtensions() const
		{
			return yelo_extensions.Count != 0;
		}
		bool unit_seat::HasBoardingTargetSeat() const
		{
			return	HasYeloExtensions() &&
					yelo_extensions[0].boarding.target_seat_index != NONE;
		}
		const unit_seat_boarding& unit_seat::GetSeatBoarding() const
		{
			YELO_ASSERT(HasYeloExtensions());

			return yelo_extensions[0].boarding;
		}
	};
};