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
		struct s_unit_datum;
	};

	namespace TagGroups
	{
		struct unit_seat;
		struct unit_seat_damage;
		struct unit_seat_extensions;
	};

	namespace Objects { namespace Units { namespace SeatDamage
	{
		/// <summary>	Manager for unit seat damage. </summary>
		class c_unit_seat_damage_manager final
		{
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Attaches a grenade projectile to a unit at the specified marker. </summary>
			///
			/// <param name="unit_index">	   	Datum index of the attacking unit. </param>
			/// <param name="unit_datum">	   	[in] If non-null, the unit datum. </param>
			/// <param name="projectile_index">	Datum index of the projectile. </param>
			/// <param name="seat_damage">	   	The seat damage definition. </param>
			void AttachGrenadeToUnit(const datum_index unit_index
				, s_unit_datum* unit_datum
				, const datum_index projectile_index
				, const TagGroups::unit_seat_damage& seat_damage) const;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Applies melee damage to unit. </summary>
			///
			/// <param name="unit_index"> 	Datum index of the attacking unit. </param>
			/// <param name="target_unit">	Datum index of the target unit. </param>
			/// <param name="seat_damage">	The seat damage definition. </param>
			void ApplyMeleeDamageToUnit(const datum_index unit_index
				, const datum_index target_unit
				, const TagGroups::unit_seat_damage& seat_damage
				, const int16 region_index = NONE) const;

		public:
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>
			/// 	Handles seated melee attacks, either against the parent unit or defaulting to a standard
			/// 	melee attack.
			/// </summary>
			///
			/// <param name="unit_index">	Datum index of the unit. </param>
			void UnitCauseSeatedPlayerMelee(const datum_index unit_index) const;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>
			/// 	Handles planting a grenade when attacking the parent unit, otherwise defaults to the
			/// 	standard grenade throw logic.
			/// </summary>
			///
			/// <param name="unit_index">	Datum index of the unit. </param>
			void UnitThrowSeatedPlayerGrenade(const datum_index unit_index) const;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Gets whether the player can throw grenades in their current seat. </summary>
			///
			/// <param name="unit_index">  	Datum index of the unit. </param>
			/// <param name="weapon_index">	Datum index of the weapon. </param>
			///
			/// <returns>	true if prevented, otherwise false. </returns>
			bool PreventSeatedPlayerGrenade(const datum_index unit_index, const datum_index weapon_index) const;
		};
	};};};
};