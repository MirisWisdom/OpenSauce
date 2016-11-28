/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/

#include <YeloLib/Halo1/units/c_unit_seat_boarding_manager.hpp>

#if PLATFORM_TYPE == PLATFORM_SAPIEN

#include <YeloLib/Halo1/units/units_yelo.hpp>

namespace Yelo
{
	namespace Objects { namespace Units { namespace SeatBoarding
	{
		static c_unit_seat_boarding_manager g_unit_seat_transition_manager;

		void UnitUpdate(const datum_index unit_index)
		{
			g_unit_seat_transition_manager.UnitUpdate(unit_index);
		}

		void UnitCanEnterBoardingSeat(const datum_index unit_index
			, const datum_index target_unit_index
			, const int16 target_seat_index
			, bool& result)
		{
			g_unit_seat_transition_manager.UnitCanEnterBoardingSeat(unit_index, target_unit_index, target_seat_index, result);
		}

		void UnitCanEnterTargetSeat(const datum_index unit_index
			, const datum_index target_unit_index
			, const int16 target_seat_index
			, bool& result)
		{
			g_unit_seat_transition_manager.UnitCanEnterTargetSeat(unit_index, target_unit_index, target_seat_index, result);
		}

		void Initialize()
		{
			Animations::SetAnimationStateKeyframeHandler(Enums::_unit_animation_state_yelo_seat_boarding,
				[](const datum_index unit_index, const Enums::unit_animation_keyframe keyframe){ g_unit_seat_transition_manager.UnitTriggerBoardingKeyframe(unit_index, keyframe); });
			Animations::SetAnimationStateKeyframeHandler(Enums::_unit_animation_state_yelo_seat_ejecting,
				[](const datum_index unit_index, const Enums::unit_animation_keyframe keyframe){ g_unit_seat_transition_manager.UnitTriggerEjectingKeyframe(unit_index, keyframe); });
			Animations::SetAnimationStateKeyframeHandler(Enums::_unit_animation_state_yelo_unit_mounted,
				[](const datum_index unit_index, const Enums::unit_animation_keyframe keyframe){ g_unit_seat_transition_manager.UnitTriggerMountedKeyframe(unit_index, keyframe); });
		}
	};};};
};
#endif