/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/

#include <YeloLib/Halo1/units/c_unit_seat_boarding_manager.hpp>

namespace Yelo
{
	namespace Objects { namespace Units { namespace SeatBoarding
	{
		static c_unit_seat_boarding_manager g_unit_seat_transition_manager;

		void UnitUpdate(const datum_index unit_index)
		{
			g_unit_seat_transition_manager.UnitUpdate(unit_index);
		}

		void AnimationStateBoardPrimaryKeyframeHandler(const datum_index unit_index)
		{
			g_unit_seat_transition_manager.UnitTriggerBoardPrimaryKeyframe(unit_index);
		}

		void AnimationStateBoardFinalKeyframeHandler(const datum_index unit_index)
		{
			g_unit_seat_transition_manager.UnitTriggerBoardFinalKeyframe(unit_index);
		}

		void AnimationStateEjectionPrimaryKeyframeHandler(const datum_index unit_index)
		{
			g_unit_seat_transition_manager.UnitTriggerEjectionPrimaryKeyframe(unit_index);
		}

		void AnimationStateEjectionFinalKeyframeHandler(const datum_index unit_index)
		{
			g_unit_seat_transition_manager.UnitTriggerEjectionFinalKeyframe(unit_index);
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
	};};};
};