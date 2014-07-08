/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include <YeloLib/Halo1/units/units_yelo.hpp>
#include <YeloLib/Halo1/open_sauce/project_yellow_scenario.hpp>

#include <blamlib/Halo1/game/game_allegiance.hpp>
#include <blamlib/Halo1/models/model_animation_definitions.hpp>
#include <blamlib/Halo1/models/model_animations.hpp>
#include <blamlib/Halo1/objects/damage.hpp>
#include <blamlib/Halo1/objects/objects.hpp>
#include <blamlib/Halo1/units/unit_definitions.hpp>
#include <blamlib/Halo1/units/unit_structures.hpp>
#include <blamlib/Halo1/units/units.hpp>
#include <YeloLib/Halo1/open_sauce/project_yellow_scenario_definitions.hpp>

namespace Yelo { namespace Objects {

	datum_index GetUnitInSeat(datum_index unit_index, int32 seat_index)
	{
		auto unit = blam::object_get_and_verify_type<s_unit_datum>(unit_index);
		datum_index seated_unit_index = datum_index::null;

		for (datum_index next_object_index = unit->object.first_object_index;
			 !next_object_index.IsNull();
			 next_object_index = blam::object_get(next_object_index)->next_object_index)
		{
			auto seated_unit = blam::object_try_and_get_and_verify_type<s_unit_datum>(next_object_index);

			if (seated_unit != nullptr && 
				seated_unit->object.type == Enums::_object_type_biped && 
				seated_unit->unit.vehicle_seat_index == seat_index)
				seated_unit_index = next_object_index;
		}

		return seated_unit_index;
	}

	void s_unit_animation_data::ResetOverlayAnimations()
	{
		replacement_animation = { NONE };
		replacement_animation_state = NONE;	// TODO: pretty sure we need to use _unit_replacement_animation_state_none here
		overlay_animation = { NONE };
		overlay_animation_state = NONE;		// TODO: pretty sure we need to use _unit_overlay_animation_state_none here
	}

	// Plays the yelo_ejection animation and ejects the unit from it's seat
	static void EjectUnit(datum_index unit_index)
	{
		// Check if a target_unit is null
		if (unit_index.IsNull())
			return;

		auto unit = blam::object_get_and_verify_type<s_unit_datum>(unit_index);
		TagGroups::model_animation_graph const* animation_graph = GetObjectAnimations(unit_index);
		
		// Check if an animation graph exists for the target unit
		if (animation_graph != nullptr)
		{
			sbyte animation_seat_block_index = unit->unit.animation.seat_index;
			int32 unit_seat_animation_count = animation_graph->units[animation_seat_block_index].animations.Count;

			// Check if the target unit has an ejection animation to play
			if (Enums::_unit_seat_animation_yelo_ejection < unit_seat_animation_count)
			{
				int32 animation_index = animation_graph->units[animation_seat_block_index].
					animations[Enums::_unit_seat_animation_yelo_ejection];

				// pick a random permutation of the ejection animation
				animation_index = blam::animation_choose_random_permutation_internal(Enums::_animation_update_kind_affects_game_state, 
					unit->object.animation.definition_index, animation_index);

				// set the target unit's animation to yelo_seat_ejection
				blam::unit_set_animation(unit_index, 
					unit->object.animation.definition_index, animation_index);
				
				unit->unit.animation.ResetOverlayAnimations();
			}

			// set the target_unit's animation state to seat_exit to force them out of the vehicle
			unit->unit.animation.state = Enums::_unit_animation_state_seat_exit;
		}
	}

	template<Enums::powered_seat powered_seat_type>
	static void UnitEnterPowerSeat(datum_index unit_index,
		s_unit_datum* target_unit,
		TagGroups::unit_seat const& target_seat)
	{
		if (TEST_FLAG(target_seat.flags, Flags::_unit_seat_boarding_enters_seat_bit))
		{
			target_unit->unit.powered_seats_riders[powered_seat_type] = unit_index;
			target_unit->unit.powered_seats_power[powered_seat_type] = 1.0f;
		}
	}
	void UnitSeatBoardingPrimaryKeyframe(datum_index unit_index)
	{
		auto unit = blam::object_get_and_verify_type<s_unit_datum>(unit_index);

		datum_index parent_unit_index = unit->object.parent_object_index;
		auto parent_unit = blam::object_get_and_verify_type<s_unit_datum>(parent_unit_index);
		int16 seat_index = unit->unit.vehicle_seat_index;

		auto const* parent_unit_definition = 
			Objects::GetObjectDefinition<TagGroups::s_unit_definition>(parent_unit_index);
		if (!TEST_FLAG(parent_unit_definition->unit.flags, Flags::_unit_has_boarding_seats_yelo_bit))
			return;

		TagGroups::unit_seat const& boarding_seat = parent_unit_definition->unit.seats[seat_index];
		if (!boarding_seat.HasBoardingTargetSeat())
			return;

		TagGroups::unit_seat_boarding const& seat_boarding = boarding_seat.GetSeatBoarding();

		int16 target_seat_index = seat_boarding.target_seat_index;
		TagGroups::unit_seat const& target_seat = parent_unit_definition->unit.seats[target_seat_index];
		datum_index target_unit_index = GetUnitInSeat(parent_unit_index, target_seat_index);

		if (!target_unit_index.IsNull())
		{
			auto target_unit = blam::object_get_and_verify_type<s_unit_datum>(target_unit_index);

			// Eject the target_unit
			EjectUnit(target_unit_index);

			// If the target seat is a drivers seat and boarding enters the seat, make the boarding unit the driver
			if (TEST_FLAG(target_seat.flags, Flags::_unit_seat_driver_bit))
				UnitEnterPowerSeat<Enums::_powered_seat_driver>(unit_index, parent_unit, target_seat);
			else if (TEST_FLAG(target_seat.flags, Flags::_unit_seat_gunner_bit))
				UnitEnterPowerSeat<Enums::_powered_seat_gunner>(unit_index, parent_unit, target_seat);

			// If the boarding seat definition contains a damage effect tag, use it here
			if (!seat_boarding.boarding_damage.tag_index.IsNull())
			{
				Objects::s_damage_data damage_data;

				// Create a new damage_data struct based on the boarding_seat_definition boarding damage field
				blam::damage_data_new(damage_data, seat_boarding.boarding_damage.tag_index);
				damage_data.responsible_player_index = unit->unit.controlling_player_index;
				damage_data.responsible_unit_index = unit_index;
				damage_data.responsible_units_team = unit->object.owner_team;
				damage_data.location = target_unit->object.location;
				damage_data.damage_position = target_unit->object.position;
				damage_data.damage_multiplier = 1.0f;

				// Damage the target_unit
				blam::object_cause_damage(damage_data, target_unit_index);
			}
		}
	}

	void UnitSeatBoardingFinalKeyframe(datum_index unit_index)
	{
		auto unit = blam::object_get_and_verify_type<s_unit_datum>(unit_index);

		datum_index parent_unit_index = unit->object.parent_object_index;
		auto parent_unit = blam::object_get_and_verify_type<s_unit_datum>(parent_unit_index);
		int16 seat_index = unit->unit.vehicle_seat_index;

		auto const* parent_unit_definition =
			Objects::GetObjectDefinition<TagGroups::s_unit_definition>(parent_unit_index);
		if (!TEST_FLAG(parent_unit_definition->unit.flags, Flags::_unit_has_boarding_seats_yelo_bit))
			return;

		TagGroups::unit_seat const& boarding_seat = parent_unit_definition->unit.seats[seat_index];
		if (!boarding_seat.HasBoardingTargetSeat())
			return;

		TagGroups::unit_seat_boarding const& seat_boarding = boarding_seat.GetSeatBoarding();

		// Force the unit into the target seat
		int16 target_seat_index = seat_boarding.target_seat_index;
		datum_index target_unit_index = GetUnitInSeat(parent_unit_index, target_seat_index);

		// Eject the target_unit from it's seat if we haven't already
		if (!target_unit_index.IsNull())
			blam::unit_exit_seat_end(target_unit_index, true, true, false);

		// Enter the target seat
		blam::unit_enter_seat(unit_index, parent_unit_index, target_seat_index);
			
		// Close the vehicle if boarding controls open/close
		if (TEST_FLAG(boarding_seat.flags, Flags::_unit_seat_controls_open_and_close_bit))
			blam::unit_close(parent_unit_index);
	}

	// Plays the yelo_board animation
	static void UnitSeatBoardingBeginBoardingAnimation(TagGroups::unit_seat const& boarding_seat,
		datum_index unit_index, datum_index parent_unit_index)
	{
		auto unit = blam::object_get_and_verify_type<s_unit_datum>(unit_index);

		TagGroups::model_animation_graph const* animation_graph = GetObjectAnimations(unit_index);
		if (animation_graph == nullptr)
			return;

		int32 animation_seat_block_index = unit->unit.animation.seat_index;
		int32 unit_seat_animation_count = animation_graph->units[animation_seat_block_index].animations.Count;

		// Check if the unit has a board animation to play
		if (Enums::_unit_seat_animation_yelo_board < unit_seat_animation_count)
		{
			int32 animation_index = animation_graph->units[animation_seat_block_index].
				animations[Enums::_unit_seat_animation_yelo_board];

			// pick a random permutation of the boarding animation
			animation_index = blam::animation_choose_random_permutation_internal(Enums::_animation_update_kind_affects_game_state, 
				unit->object.animation.definition_index, animation_index);

			// set the unit's animation to yelo_board
			blam::object_start_interpolation(unit_index, 6);
			blam::unit_set_animation(unit_index, 
				unit->object.animation.definition_index, animation_index);
				
			unit->unit.animation.ResetOverlayAnimations();

			// if boarding enters the target seat, use the seat_board animation state
			if (TEST_FLAG(boarding_seat.flags, Flags::_unit_seat_boarding_enters_seat_bit))
				unit->unit.animation.state = Enums::_unit_animation_state_yelo_seat_board;
			// else, use the seat_exit animation state so we exit the vehicle when complete
			else
				unit->unit.animation.state = Enums::_unit_animation_state_seat_exit;
		}
	}
	void UnitSeatBoardingEnterFinalKeyframe(datum_index unit_index, long_enum* next_animation_state)
	{
		auto unit = blam::object_get_and_verify_type<s_unit_datum>(unit_index);
		int16 seat_index = unit->unit.vehicle_seat_index;

		datum_index parent_unit_index = unit->object.parent_object_index;
		auto parent_unit = blam::object_get_and_verify_type<s_unit_datum>(parent_unit_index);

		auto const* parent_unit_definition =
			Objects::GetObjectDefinition<TagGroups::s_unit_definition>(parent_unit_index);
		if (!TEST_FLAG(parent_unit_definition->unit.flags, Flags::_unit_has_boarding_seats_yelo_bit))
			return;

		TagGroups::unit_seat const& boarding_seat = parent_unit_definition->unit.seats[seat_index];
		if (!boarding_seat.HasBoardingTargetSeat())
			return;

		TagGroups::unit_seat_boarding const& seat_boarding = boarding_seat.GetSeatBoarding();

		// If boarding ejects the target seat, initiate the board sequence (plays board animation)
		if (TEST_FLAG(boarding_seat.flags, Flags::_unit_seat_boarding_ejects_seat_yelo_bit))
			UnitSeatBoardingBeginBoardingAnimation(boarding_seat, unit_index, parent_unit_index);

		// if boarding enters the target seat, use the seat_board animation state
		if (TEST_FLAG(boarding_seat.flags, Flags::_unit_seat_boarding_enters_seat_bit))
			*next_animation_state = Enums::_unit_animation_state_yelo_seat_board;
		// else, use the seat_exit animation state so we exit the vehicle when complete
		else
			*next_animation_state = Enums::_unit_animation_state_seat_exit;
			
		// Close the vehicle if boarding controls open/close
		if (TEST_FLAG(boarding_seat.flags, Flags::_unit_seat_controls_open_and_close_bit))
			blam::unit_close(parent_unit_index);
	}

	// Checks if any other boarding seats have the same target_seat_index and if anyone else is trying to board from them
	static bool UnitCanEnterBoardingSeat(datum_index parent_unit_index, int16 seat_index)
	{
		auto parent_unit = blam::object_get_and_verify_type<s_unit_datum>(parent_unit_index);
		auto const* parent_unit_definition =
			Objects::GetObjectDefinition<TagGroups::s_unit_definition>(parent_unit_index);
		
		if (!TEST_FLAG(parent_unit_definition->unit.flags, Flags::_unit_has_boarding_seats_yelo_bit))
			return false;

		TagGroups::unit_seat const& boarding_seat = parent_unit_definition->unit.seats[seat_index];
		if (!boarding_seat.HasBoardingTargetSeat())
			return false;

		TagGroups::unit_seat_boarding const& seat_boarding = boarding_seat.GetSeatBoarding();

		int16 target_seat_index = seat_boarding.target_seat_index;

		// Loop through the boarding seats
		int current_seat_index = 0;
		for (const auto& seat : parent_unit_definition->unit.seats)
		{
			if (current_seat_index == seat_index || !seat.HasBoardingTargetSeat())
				continue;

			// Check if the current seat has the same target seat as the seat we're trying to enter
			if (seat.GetSeatBoarding().target_seat_index == target_seat_index)
			{
				datum_index seated_unit_index = GetUnitInSeat(parent_unit_index, current_seat_index);

				// If there is someone sitting in this seat, we can't enter
				if (!seated_unit_index.IsNull())
					return false;
			}

			current_seat_index++;
		}

		return true;
	}

	bool UnitCanEnterSeat(datum_index unit_index, datum_index target_unit_index, int16 target_seat_index,
		_Out_opt_ datum_index* return_unit_in_seat)
	{
		bool result = true;
		datum_index unit_in_seat = datum_index::null;

		auto unit = blam::object_get_and_verify_type<s_unit_datum>(unit_index);
		auto target_unit = blam::object_get_and_verify_type<s_unit_datum>(target_unit_index);

		if (unit_index == target_unit_index)
			result = false;

		for (datum_index child_object_index = target_unit->object.first_object_index;
			 !child_object_index.IsNull();
			 child_object_index = blam::object_get(child_object_index)->next_object_index)
		{
			auto child_object = blam::object_try_and_get_and_verify_type<s_unit_datum>(child_object_index);

			if (child_object)
			{
				// Check if the child_object is in the seat_index that unit_index is trying to enter
				if (child_object->unit.vehicle_seat_index == target_seat_index)
				{
					unit_in_seat = child_object_index;
					result = false;
				}
				// If multiteam vehicles is prohibited, test unit and target unit teams
				else if (Scenario::GetYelo()->gameplay.flags.prohibit_multiteam_vehicles_bit)
				{
					if (unit->unit.controlling_player_index.IsNull() ||
						!blam::game_team_is_enemy(child_object->object.owner_team, unit->object.owner_team))
						result = true;
					else
						result = false;
				}
			}
		}

		auto const* target_unit_definition =
			Objects::GetObjectDefinition<TagGroups::s_unit_definition>(target_unit->object.definition_index);

		if (TEST_FLAG(target_unit_definition->unit.flags, Flags::_unit_has_boarding_seats_yelo_bit))
		{
			TagGroups::unit_seat const& boarding_seat = target_unit_definition->unit.seats[target_seat_index];

			if (boarding_seat.HasBoardingTargetSeat())
			{
				TagGroups::unit_seat_boarding const& seat_boarding = boarding_seat.GetSeatBoarding();

				datum_index boarded_unit_index = GetUnitInSeat(target_unit_index, seat_boarding.target_seat_index);

				// If there is no unit to board, if they are an ally, or if there is already someone boarding, disallow entry into the boarding seat
				if (boarded_unit_index.IsNull() ||
					!ObjectIsEnemy(unit_index, boarded_unit_index) ||
					!UnitCanEnterBoardingSeat(target_unit_index, target_seat_index))
				{
					result = false;
				}
			}
		}

		if (return_unit_in_seat != nullptr)
			*return_unit_in_seat = unit_in_seat;

		return result;
	}
}; };