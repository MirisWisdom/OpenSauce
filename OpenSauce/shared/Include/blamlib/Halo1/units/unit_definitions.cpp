/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include <blamlib/Halo1/units/unit_definitions.hpp>

#include <blamlib/Halo1/tag_files/tag_groups.hpp>

namespace Yelo
{
	namespace TagGroups
	{
#if PLATFORM_IS_EDITOR
		bool unit_seat::Postprocess(Enums::tag_postprocess_mode mode,
			datum_index tag_index)
		{
			using namespace Flags;

			// bits which should only be set when a boarding target seat is specified
			const long_flags k_unit_seat_requires_boarding_flags_mask =
				FLAG(_unit_seat_boarding_seat_bit) |
				FLAG(_unit_seat_boarding_enters_seat_bit) |
				FLAG(_unit_seat_boarding_ejects_seat_yelo_bit);

			if ((flags & k_unit_seat_requires_boarding_flags_mask) != 0 && !HasBoardingTargetSeat())
			{
				YELO_WARN("%s: '%s' seat has flags related to the boarding seat without specifying one. removing flags",
					blam::tag_try_get_name(tag_index), label);

				flags &= ~k_unit_seat_requires_boarding_flags_mask;
			}

			if (HasBoardingTargetSeat())
			{
				SET_FLAG(flags, _unit_seat_boarding_seat_bit, true);
			}

			return true;
		}
		bool PLATFORM_API unit_seat::Postprocess(void* element, Enums::tag_postprocess_mode mode,
			datum_index tag_index)
		{
			auto* seat_definition = CAST_PTR(unit_seat*, element);

			return seat_definition->Postprocess(mode, tag_index);
		}

		bool _unit_definition::Postprocess(Enums::tag_postprocess_mode mode,
			datum_index tag_index)
		{
			return true;
		}
#endif
	};
}