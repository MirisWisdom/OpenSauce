/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

namespace Yelo
{
	namespace Enums
	{
		enum unit_seat_animation : _enum;
		enum unit_animation_keyframe: _enum;
	};

	namespace Objects
	{
		struct s_unit_datum;
	};

	namespace TagGroups
	{
		struct unit_seat_keyframe_action;
		struct unit_mounted_state_keyframe_action;
	};

	namespace Objects { namespace Units { namespace SeatBoarding
	{
		/// <summary>	Manager for unit seat boarding. </summary>
		class c_unit_seat_boarding_manager final
		{
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Applies an effect on a unit at the specified marker. </summary>
			///
			/// <param name="unit_index">   	Datum index of the unit. </param>
			/// <param name="effect">			The effect. </param>
			/// <param name="effect_marker">	The effect marker. </param>
			void UnitApplyEffect(const datum_index unit_index
				, const datum_index effect
				, const tag_string& effect_marker) const;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Applies a damage effect on a unit. </summary>
			///
			/// <param name="unit_index">			Datum index of the unit. </param>
			/// <param name="target_unit_index">	Datum index of the target unit. </param>
			/// <param name="damage_effect">		The damage effect. </param>
			void UnitApplyDamage(const datum_index unit_index
				, const datum_index target_unit_index
				, const datum_index damage_effect) const;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Makes a unit exit it's current seat. </summary>
			///
			/// <param name="unit_index">	Zero-based index of the unit. </param>
			void UnitExitFromSeat(const datum_index unit_index) const;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Eject's a unit from a seat. </summary>
			///
			/// <param name="unit_index">	Datum index of the unit. </param>
			void UnitEjectFromSeat(const datum_index unit_index) const;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Makes a unit enter a seat on the target unit. </summary>
			///
			/// <param name="unit_index">			Datum index of the unit. </param>
			/// <param name="target_unit_index">	Datum index of the target unit. </param>
			/// <param name="target_seat">			The target seat index. </param>
			void UnitEnterSeat(const datum_index unit_index
				, const datum_index target_unit_index
				, const int16 target_seat) const;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Overrides the unit controlling a powered seat. </summary>
			///
			/// <param name="unit_index">			Datum index of the unit. </param>
			/// <param name="target_unit_index">	Datum index of the target unit. </param>
			/// <param name="target_seat">			The target seat index. </param>
			void UnitEnterPoweredSeat(const datum_index unit_index
				, const datum_index target_unit_index
				, const int16 target_seat) const;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Starts a unit seat animation on the target unit. </summary>
			///
			/// <param name="unit_index">	Datum index of the unit. </param>
			/// <param name="unit_datum">	The unit datum. </param>
			/// <param name="animation"> 	The animation type. </param>
			///
			/// <returns>	true if it succeeds, false if it fails. </returns>
			bool StartAnimation(const datum_index unit_index
				, const s_unit_datum* unit_datum
				, const Enums::unit_seat_animation animation) const;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Executes the boarding animation's keyframe action. </summary>
			///
			/// <param name="unit_index">	Datum index of the unit. </param>
			/// <param name="action">	 	The action details. </param>
			void DoBoardingKeyframeAction(const datum_index unit_index, const TagGroups::unit_seat_keyframe_action& action) const;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Executes the mounted animation's keyframe action. </summary>
			///
			/// <param name="unit_index">	Datum index of the unit. </param>
			/// <param name="action">	 	The action details. </param>
			void DoMountedKeyframeAction(const datum_index unit_index, const TagGroups::unit_mounted_state_keyframe_action& action) const;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Updates the non seated unit described by unit_index. </summary>
			///
			/// <param name="unit_index">	Datum index of the unit. </param>
			void UpdateNonSeatedUnit(const datum_index unit_index) const;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Updates the seated unit described by unit_index. </summary>
			///
			/// <param name="unit_index">	Datum index of the unit. </param>
			void UpdateSeatedUnit(const datum_index unit_index) const;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Updates the boarding unit described by unit_index. </summary>
			///
			/// <param name="unit_index">	Datum index of the unit. </param>
			void UpdateBoardingUnit(const datum_index unit_index) const;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Updates the ejecting unit described by unit_index. </summary>
			///
			/// <param name="unit_index">	Datum index of the unit. </param>
			void UpdateEjectingUnit(const datum_index unit_index) const;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Updates the mounted unit described by unit_index. </summary>
			///
			/// <param name="unit_index">	Datum index of the unit. </param>
			void UpdateMountedUnit(const datum_index unit_index) const;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Determines whether a unit can enter a seat. </summary>
			///
			/// <param name="unit_index">			Datum index of the unit. </param>
			/// <param name="target_unit_index">	Datum index of the target unit. </param>
			/// <param name="seat_index">			Datum index of the seat. </param>
			/// <param name="is_targeted_seat"> 	Whether the seat is a targeted seat. </param>
			/// <param name="result">				[in,out] The result. </param>
			void CanEnterSeat(const datum_index unit_index
				, const datum_index target_unit_index
				, const int16 seat_index
				, const bool is_targeted_seat
				, bool& result) const;

		public:
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Update's a unit's boarding state. </summary>
			///
			/// <param name="unit_index">	Datum index of the unit. </param>
			void UnitUpdate(const datum_index unit_index) const;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Handles the boarding animations keyframe actions. </summary>
			///
			/// <param name="unit_index">	Datum index of the animating unit. </param>
			/// <param name="keyframe">  	The keyframe. </param>
			void UnitTriggerBoardingKeyframe(const datum_index unit_index, const Enums::unit_animation_keyframe keyframe) const;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Handles the ejection animations keyframe actions. </summary>
			///
			/// <param name="unit_index">	Datum index of the animating unit. </param>
			/// <param name="keyframe">  	The keyframe. </param>
			void UnitTriggerEjectingKeyframe(const datum_index unit_index, const Enums::unit_animation_keyframe keyframe) const;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Handles the mounted animations keyframe actions. </summary>
			///
			/// <param name="unit_index">	Datum index of the animating unit. </param>
			/// <param name="keyframe">  	The keyframe. </param>
			void UnitTriggerMountedKeyframe(const datum_index unit_index, const Enums::unit_animation_keyframe keyframe) const;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Control's whether a unit can enter a boarding seat. </summary>
			///
			/// <param name="unit_index">			Datum index of the unit. </param>
			/// <param name="target_unit_index">	Datum index of the target unit. </param>
			/// <param name="target_seat_index">	Index of the target seat. </param>
			/// <param name="result">				[in,out] The result. </param>
			void UnitCanEnterBoardingSeat(const datum_index unit_index
				, const datum_index target_unit_index
				, const int16 target_seat_index
				, bool& result) const;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Control's whether a unit can enter a boarding target seat. </summary>
			///
			/// <param name="unit_index">			Datum index of the unit. </param>
			/// <param name="target_unit_index">	Datum index of the target unit. </param>
			/// <param name="target_seat_index">	Index of the target seat. </param>
			/// <param name="result">				[in,out] The result. </param>
			void UnitCanEnterTargetSeat(const datum_index unit_index
				, const datum_index target_unit_index
				, const int16 target_seat_index
				, bool& result) const;
		};
	};};};
};