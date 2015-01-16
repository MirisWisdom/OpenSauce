/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/

#include <blamlib/Halo1/models/model_animation_definitions.hpp>
#include <blamlib/Halo1/models/model_animations.hpp>
#include <blamlib/Halo1/objects/damage.hpp>
#include <blamlib/Halo1/units/unit_definitions.hpp>
#include <blamlib/Halo1/units/unit_structures.hpp>
#include <YeloLib/Halo1/units/units_yelo.hpp>

#include "Objects/Objects.hpp"
#include "Objects/Units.hpp"

namespace Yelo 
{
	namespace Objects { namespace Units { namespace Boarding
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the boarding seat for a specified seat. </summary>
		///
		/// <param name="unit_definition">	The unit definition. </param>
		/// <param name="seat_index">	  	Index of the seat. </param>
		///
		/// <returns>	null if it fails, else the boarding seat. </returns>
		static TagGroups::unit_seat_boarding const* GetBoardingSeat(const TagGroups::s_unit_definition& unit_definition, const int16 seat_index)
		{
			auto& seat = unit_definition.unit.seats[seat_index];
			if(seat.HasBoardingTargetSeat())
			{
				return &seat.yelo_extensions[0].boarding;
			}
		
			return nullptr;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the target and boarding seat for a unit's seat. </summary>
		///
		/// <param name="unit_datum">   	The unit datum. </param>
		/// <param name="seat_index">   	Index of the seat. </param>
		/// <param name="target_seat">  	[out] Target seat definition pointer. </param>
		/// <param name="boarding_seat">	[out] Boarding seat definition pointer. </param>
		static void GetBoardingSeatPair(const datum_index unit_datum
			, const int16 seat_index
			, TagGroups::unit_seat const*& target_seat
			, TagGroups::unit_seat_boarding const*& boarding_seat)
		{
			auto unit = blam::object_get_and_verify_type<s_unit_datum>(unit_datum);

			auto* unit_definition = blam::tag_get<TagGroups::s_unit_definition>(unit->object.definition_index);

			target_seat = &unit_definition->unit.seats.Definitions[seat_index];
			boarding_seat = GetBoardingSeat(*unit_definition, seat_index);
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets a random seat animation. </summary>
		///
		/// <param name="unit_index">	Datum index of the unit. </param>
		/// <param name="animation"> 	The animation type to get a random animation for. </param>
		///
		/// <returns>	The random seat animation index. </returns>
		static int16 GetRandomSeatAnimation(const datum_index unit_index, const Enums::unit_seat_animation animation)
		{
			auto unit = blam::object_get_and_verify_type<s_unit_datum>(unit_index);
			if(!unit)
			{
				return NONE;
			}

			TagGroups::model_animation_graph const* animation_graph = GetObjectAnimations(unit_index);
			if (!animation_graph)
			{
				return NONE;
			}

			auto& seat_animation_graph_animations = animation_graph->units[unit->unit.animation.seat_index].animations;

			// Check if the target unit has an ejection animation to play
			if (animation < seat_animation_graph_animations.Count)
			{
				int32 animation_index = seat_animation_graph_animations[animation];

				// pick a random permutation of the ejection animation
				return blam::animation_choose_random_permutation_internal(Enums::_animation_update_kind_affects_game_state,
					unit->object.animation.definition_index, animation_index);
			}

			return NONE;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Plays the yelo_ejection animation and ejects the unit from it's seat. </summary>
		///
		/// <param name="unit_index">	Datum index of the unit doing the boarding. </param>
		static void EjectUnit(datum_index unit_index)
		{
			// Check if a target_unit is null
			if (unit_index.IsNull())
			{
				return;
			}

			auto unit = blam::object_get_and_verify_type<s_unit_datum>(unit_index);

			auto animation_index = GetRandomSeatAnimation(unit_index, Enums::_unit_seat_animation_yelo_ejection);
			if(animation_index != NONE)
			{
				// Set the target unit's animation to yelo_seat_ejection
				blam::unit_set_animation(unit_index, unit->object.animation.definition_index, animation_index);
				
				unit->unit.animation.ResetOverlayAnimations();

				// set the target_unit's animation state to seat_exit to force them out of the vehicle
				unit->unit.animation.state = Enums::_unit_animation_state_seat_exit;
			}
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Plays the yelo_board animation. </summary>
		///
		/// <param name="target_seat">			[in] The boarding seat definition. </param>
		/// <param name="boarding_seat">		[in] The boarding seat definition. </param>
		/// <param name="unit_index">			Datum index of the unit doing the boarding. </param>
		/// <param name="parent_unit_index">	Datum index of the parent unit. </param>
		static void BoardUnitSeatIndex(const TagGroups::unit_seat& target_seat
			, const TagGroups::unit_seat_boarding& boarding_seat
			, datum_index unit_index
			, datum_index parent_unit_index)
		{
			auto unit = blam::object_get_and_verify_type<s_unit_datum>(unit_index);

			auto animation_index = GetRandomSeatAnimation(unit_index, Enums::_unit_seat_animation_yelo_board);
			if (animation_index != NONE)
			{
				// Set the unit's animation to yelo_board
				blam::object_start_interpolation(unit_index, 6);
				blam::unit_set_animation(unit_index, unit->object.animation.definition_index, animation_index);
				
				unit->unit.animation.ResetOverlayAnimations();

				// If boarding enters the target seat, use the seat_board animation state
				if(TEST_FLAG(target_seat.flags, Flags::_unit_seat_boarding_enters_seat_bit))
				{
					unit->unit.animation.state = Enums::_unit_animation_state_yelo_seat_board;
				}
				// Else, use the seat_exit animation state so we exit the vehicle when complete
				else
				{
					unit->unit.animation.state = Enums::_unit_animation_state_seat_exit;
				}
			}
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// 	Called on the primary_keyframe_index of the yelo_seat_board animation, ejects the target
		/// 	unit from their seat.
		/// </summary>
		///
		/// <param name="unit_index">	Datum index of the unit doing the boarding. </param>
		static void SeatBoardPrimaryKeyframe(datum_index unit_index)
		{
			auto unit = blam::object_get_and_verify_type<s_unit_datum>(unit_index);
		
			// Get target seat and boarding seat
			datum_index parent_unit_index = unit->object.parent_object_index;
			auto parent_unit = blam::object_get_and_verify_type<s_unit_datum>(unit_index);
		
			TagGroups::unit_seat const* target_seat;
			TagGroups::unit_seat_boarding const* boarding_seat;

			GetBoardingSeatPair(parent_unit_index, unit->unit.vehicle_seat_index, target_seat, boarding_seat);

			// Check if a boarding seat definition exists for the vehicle being exited
			if (boarding_seat)
			{
				datum_index target_unit_index = GetUnitInSeat(parent_unit_index, unit->unit.vehicle_seat_index);

				if (!target_unit_index.IsNull())
				{
					auto target_unit = blam::object_get_and_verify_type<s_unit_datum>(target_unit_index);

					// Eject the target_unit
					EjectUnit(target_unit_index);

					// If the target seat is a driver/gunner seat and boarding enters the seat, make the boarding unit the driver/gunner
					if(TEST_FLAG(target_seat->flags, Flags::_unit_seat_boarding_enters_seat_bit))
					{
						if (TEST_FLAG(target_seat->flags, Flags::_unit_seat_driver_bit))
						{
							parent_unit->unit.powered_seats_riders[Enums::_powered_seat_driver] = unit_index;
							parent_unit->unit.powered_seats_power[Enums::_powered_seat_driver] = 1.0f;
						}
						else if (TEST_FLAG(target_seat->flags, Flags::_unit_seat_gunner_bit))
						{
							parent_unit->unit.powered_seats_riders[Enums::_powered_seat_gunner] = unit_index;
							parent_unit->unit.powered_seats_power[Enums::_powered_seat_gunner] = 1.0f;
						}
					}

					// If the boarding seat definition contains a damage effect tag, use it here
					if (!boarding_seat->boarding_damage.tag_index.IsNull())
					{
						Objects::s_damage_data damage_data;

						// Create a new damage_data struct based on the boarding_seat_definition boarding damage field
						blam::damage_data_new(damage_data, boarding_seat->boarding_damage.tag_index);
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
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// 	Called on the final key frame of the yelo_seat_board animation, forces the boarding unit
		/// 	into the target seat when the board animation is complete.
		/// </summary>
		///
		/// <param name="unit_index">	Datum index of the unit doing the boarding. </param>
		static void SeatBoardFinalKeyframe(datum_index unit_index)
		{
			auto unit = blam::object_get_and_verify_type<s_unit_datum>(unit_index);
		
			// Get target seat and boarding seat
			datum_index parent_unit_index = unit->object.parent_object_index;
		
			TagGroups::unit_seat const* target_seat;
			TagGroups::unit_seat_boarding const* boarding_seat;

			GetBoardingSeatPair(parent_unit_index, unit->unit.vehicle_seat_index, target_seat, boarding_seat);

			// Force the unit into the target seat
			if (boarding_seat)
			{
				int16 target_seat_index = boarding_seat->target_seat_index;
				datum_index target_unit_index = GetUnitInSeat(parent_unit_index, target_seat_index);

				// Eject the target_unit from it's seat if we haven't already
				if (!target_unit_index.IsNull())
				{
					blam::unit_exit_seat_end(target_unit_index, true, true, false);
				}

				// Enter the target seat
				blam::unit_enter_seat(unit_index, parent_unit_index, target_seat_index);
			
				// Close the vehicle if boarding controls open/close
				if (TEST_FLAG(target_seat->flags, Flags::_unit_seat_controls_open_and_close_bit))
				{
					blam::unit_close(parent_unit_index);
				}
			}
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// 	Called on the final key frame of the seat_enter animation, determines if the seat is a
		/// 	boarding seat and whether or not to board the seat or eject the target unit.
		/// </summary>
		///
		/// <param name="unit_index">		   	Datum index of the unit doing the boarding. </param>
		/// <param name="next_animation_state">	[out] Output for the next animation state. </param>
		static void SeatEnterFinalKeyframe(datum_index unit_index, int32* next_animation_state)
		{
			auto unit = blam::object_get_and_verify_type<s_unit_datum>(unit_index);
		
			// Get target seat and boarding seat
			datum_index parent_unit_index = unit->object.parent_object_index;
		
			TagGroups::unit_seat const* target_seat;
			TagGroups::unit_seat_boarding const* boarding_seat;

			GetBoardingSeatPair(parent_unit_index, unit->unit.vehicle_seat_index, target_seat, boarding_seat);

			// Check if a boarding seat definition exists for the vehicle seat being entered
			if (boarding_seat)
			{
				// If boarding ejects the target seat, initiate the board sequence (plays board animation)
				if (TEST_FLAG(target_seat->flags, Flags::_unit_seat_boarding_ejects_seat_yelo_bit))
				{
					BoardUnitSeatIndex(*target_seat, *boarding_seat, unit_index, parent_unit_index);
				}

				// If boarding enters the target seat, use the seat_board animation state
				if (TEST_FLAG(target_seat->flags, Flags::_unit_seat_boarding_enters_seat_bit))
				{
					*next_animation_state = Enums::_unit_animation_state_yelo_seat_board;
				}
				// Else, use the seat_exit animation state so we exit the vehicle when complete
				else
				{
					*next_animation_state = Enums::_unit_animation_state_seat_exit;
				}
			
				// Open the vehicle if boarding controls open/close
				if (TEST_FLAG(target_seat->flags, Flags::_unit_seat_controls_open_and_close_bit))
				{
					blam::unit_open(parent_unit_index);
				}
			}
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Determines whether a unit can enter a boarding seat. </summary>
		///
		/// <remarks>
		/// 	Checks if any other boarding seats have the same target_seat_index and if anyone else is
		/// 	trying to board from them.
		/// </remarks>
		///
		/// <param name="parent_unit_index">	Datum index of the parent unit. </param>
		/// <param name="seat_index">			Index of the seat. </param>
		///
		/// <returns>	true if it succeeds, false if it fails. </returns>
		static bool UnitCanEnterBoardingSeat(datum_index parent_unit_index, int16 seat_index)
		{
			bool result = true;

			auto parent_unit = blam::object_get_and_verify_type<s_unit_datum>(parent_unit_index);
			if(parent_unit != nullptr)
			{
				auto* unit_definition = blam::tag_get<TagGroups::s_unit_definition>(parent_unit->object.definition_index);
				auto* boarding_seat = GetBoardingSeat(*unit_definition, seat_index);
				if(!boarding_seat)
				{
					result = false;
				}
				else
				{
					auto seated_unit_index = GetUnitInSeat(parent_unit_index, seat_index);

					// If there is someone sitting in this seat, we can't enter
					if (!seated_unit_index.IsNull())
					{
						result = false;
					}
				}
			}

			return result;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Determines whether a unit can enter a seat. </summary>
		///
		/// <remarks>
		/// 	Replaces the engine's unit_can_enter_seat function to compensate for boarding and
		/// 	multiteam vehicles.
		/// </remarks>
		///
		/// <param name="unit_index">			Datum index of the unit doing the boarding. </param>
		/// <param name="target_unit_index">	Datum index of the unit in the seat. </param>
		/// <param name="target_seat_index">	Zero-based index of the target seat. </param>
		/// <param name="unit_in_seat">			[in,out] If non-null, the unit in seat. </param>
		///
		/// <returns>	true if the unit can enter the seat, false if not. </returns>
		static bool UnitCanEnterSeat(const datum_index unit_index
			, const datum_index target_unit_index
			, const int16 target_seat_index
			, _Out_opt_ datum_index* unit_in_seat)
		{
			bool result = true;

			// Get the unit datums of the boarder and boardee
			auto unit = blam::object_get_and_verify_type<s_unit_datum>(unit_index);
			auto target_unit = blam::object_get_and_verify_type<s_unit_datum>(target_unit_index);

			if (unit_index == target_unit_index)
			{
				result = false;
			}

			// Find the object in the target seat
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
						if(unit_in_seat)
						{
							*unit_in_seat = child_object_index;
						}

						result = false;
					}

					// If multiteam vehicles are prohibited, test unit and target unit teams
					else if (TEST_FLAG(Scenario::GetYelo()->gameplay.flags, Flags::_project_yellow_gameplay_prohibit_multiteam_vehicles_bit))
					{
						if (unit->unit.controlling_player_index.IsNull() ||
							!blam::game_team_is_enemy(child_object->object.owner_team, unit->object.owner_team))
						{
							result = true;
						}
						else
						{
							result = false;
						}
					}
				}
			}

			// Get whether the target seat allows boarding
			auto* unit_definition = blam::tag_get<TagGroups::s_unit_definition>(target_unit->object.definition_index);
			if(unit_definition)
			{
				auto* boarding_seat = GetBoardingSeat(*unit_definition, target_seat_index);
				if(boarding_seat)
				{
					datum_index boarded_unit_index = GetUnitInSeat(target_unit_index, boarding_seat->target_seat_index);

					// If there is no unit to board, if they are an ally, or if there is already someone boarding, disallow entry into the boarding seat
					if (boarded_unit_index.IsNull()
						|| !ObjectIsEnemy(unit_index, boarded_unit_index)
						|| !UnitCanEnterBoardingSeat(target_unit_index, target_seat_index))
					{
						result = false;
					}
				}
			}

			return result;
		}

		API_FUNC_NAKED static void PLATFORM_API UnitCanEnterSeatHook()
		{
			API_FUNC_NAKED_START_()
				push	ebx
				push	ecx
				push	edx

				mov		ecx, [esp+24]
				mov		ebx, [esp+20]
				push	ecx		// datum_index* unit_in_seat_index
				push	ebx		// int16 target_seat_index
				push	edx		// datum_index target_unit_index
				push	eax		// datum_index unit_index
				call	UnitCanEnterSeat

				pop		edx
				pop		ecx
				pop		ebx
			API_FUNC_NAKED_END_()
		}

		void Initialize()
		{
			Memory::CreateHookRelativeCall(&UnitCanEnterSeatHook, GET_FUNC_VPTR(UNIT_CAN_ENTER_SEAT_HOOK), Enums::_x86_opcode_ret);
		}

		void Dispose() { }
	};};};
};