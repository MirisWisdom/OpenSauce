/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

namespace Yelo
{
	namespace Objects
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Get the object_index of the unit in the specified seat_index of the given vehicle (unit). </summary>
		///
		/// <param name="unit_index">	subject vehicle. </param>
		/// <param name="seat_index">   subject vehicle seat. </param>
		///
		/// <returns>	The index of the unit in the seat. </returns>
		datum_index GetUnitInSeat(datum_index unit_index, int32 seat_index);

		// Called on the primary_keyframe_index of the yelo_seat_board animation, 
		// ejects the target unit from their seat
		void UnitSeatBoardingPrimaryKeyframe(datum_index unit_index);

		// Called on the final keyframe of the yelo_seat_board animation,
		// forces the boarding unit into the target seat when the board animation is complete
		void UnitSeatBoardingFinalKeyframe(datum_index unit_index);

		// Called on the final keyframe of the seat_enter animation, 
		// determines if the seat is a boarding seat and whether or not to board the seat or eject the target unit
		void UnitSeatBoardingEnterFinalKeyframe(datum_index unit_index, long_enum* next_animation_state);

		// Replaces the engine's unit_can_enter_seat function with our own to compensate for boarding and multiteam vehicles
		bool UnitCanEnterSeat(datum_index unit_index, datum_index target_unit_index, int16 target_seat_index,
			_Out_opt_ datum_index* return_unit_in_seat);
	};
};