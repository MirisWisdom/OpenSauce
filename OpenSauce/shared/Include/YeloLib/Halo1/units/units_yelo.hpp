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
		enum unit_animation_state : sbyte;

		enum unit_animation_keyframe : _enum
		{
			_unit_animation_keyframe_primary,
			_unit_animation_keyframe_secondary,
			_unit_animation_keyframe_final,

			_unit_animation_keyframe
		};
	};

	namespace TagGroups
	{
		struct unit_extensions;
		struct unit_seat;
		struct unit_seat_extensions;
	};

	namespace Objects
	{
		struct s_biped_datum;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the unit extension definition for a unit. </summary>
		///
		/// <param name="unit_index">	Datum index of the unit. </param>
		///
		/// <returns>	null if it fails, else the unit extension definition. </returns>
		const TagGroups::unit_extensions* GetUnitExtensionDefinition(const datum_index unit_index);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the specified seat definition from a unit. </summary>
		///
		/// <param name="unit_index">	Datum index of the unit. </param>
		/// <param name="seat_index">	Index of the seat. </param>
		///
		/// <returns>	The seat definition. </returns>
		const TagGroups::unit_seat* GetSeatDefinition(const datum_index unit_index, const int16 seat_index);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the seat extension definition for a seat. </summary>
		///
		/// <param name="unit_index">	Datum index of the unit. </param>
		/// <param name="seat_index">	Datum index of the seat. </param>
		///
		/// <returns>	null if it fails, else the seat extension definition. </returns>
		const TagGroups::unit_seat_extensions* GetSeatExtensionDefinition(const datum_index unit_index, const int16 seat_index);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the unit in an objects seat. </summary>
		///
		/// <param name="unit_index">	Datum index of the unit. </param>
		/// <param name="seat_index">	Datum index of the seat. </param>
		///
		/// <returns>	The unit in seat. </returns>
		datum_index GetUnitInSeat(const datum_index unit_index, const int16 seat_index);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Handler for biped's meleeing whilst seated. </summary>
		///
		/// <param name="unit_index">	Datum index of the unit. </param>
		void PLATFORM_API BipedSeatedMelee(const datum_index unit_index);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Returns whether a unit can enter a seat, handles multi team vehicles. </summary>
		///
		/// <param name="unit_index">		  	Datum index of the unit. </param>
		/// <param name="target_unit_index">  	Datum index of the target unit. </param>
		/// <param name="target_seat_index">  	Datum index of the target seat. </param>
		/// <param name="return_unit_in_seat">	[out] If non-null, the return unit in seat. </param>
		/// <param name="result">			  	[in,out] The result. </param>
		void UnitCanEnterSeatMultiteam(datum_index unit_index
			, datum_index target_unit_index
			, int16 target_seat_index
			, _Out_opt_ datum_index* return_unit_in_seat
			, bool& result);

		namespace Units
		{
			namespace Animations
			{
				typedef void (*animation_state_keyframe_handler_t)(const datum_index, const Enums::unit_animation_keyframe);

				void PLATFORM_API AnimationStateDefinedKeyframe(const datum_index unit_index, const Enums::unit_animation_state state);

				void PLATFORM_API AnimationStateFinalKeyframe(const datum_index unit_index, const Enums::unit_animation_state state);

				void SetAnimationStateKeyframeHandler(const Enums::unit_animation_state state, animation_state_keyframe_handler_t handler);
			}

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>
			/// 	Called by BipedSeatedMelee when melee damage needs to be dealt. Implemented externally.
			/// </summary>
			///
			/// <param name="unit_index">	Datum index of the unit. </param>
			void UnitCausePlayerSeatedMelee(const datum_index unit_index);
		}
	};
};