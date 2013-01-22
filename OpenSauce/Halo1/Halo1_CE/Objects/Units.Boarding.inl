/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/

namespace Boarding
{
	// Plays the yelo_ejection animation and ejects the unit from it's seat
	static void EjectUnit(datum_index unit_index)
	{
		// Check if a target_unit is null
		if (unit_index.IsNull())
			return;

		s_unit_datum* unit = (*Objects::ObjectHeader())[unit_index]->_unit;
		TagGroups::model_animation_graph const* animation_graph = GetObjectAnimations(unit_index);
		
		// Check if an animation graph exists for the target unit
		if (animation_graph != NULL)
		{
			sbyte animation_seat_block_index = *unit->unit.animation.GetSeatIndex();
			int32 unit_seat_animation_count = animation_graph->units[animation_seat_block_index].animations.Count;

			// Check if the target unit has an ejection animation to play
			if (Enums::_unit_seat_animation_yelo_ejection < unit_seat_animation_count)
			{
				int32 animation_index = animation_graph->units[animation_seat_block_index].
					animations[Enums::_unit_seat_animation_yelo_ejection];

				// pick a random permutation of the ejection animation
				animation_index = Engine::AnimationPickRandomPermutation(true, 
					unit->object.animation.definition_index, animation_index);

				// set the target unit's animation to yelo_seat_ejection
				Engine::Objects::UnitSetAnimation(unit_index, 
					unit->object.animation.definition_index, animation_index);
				
				// reset  the unit's overlay animations
				*unit->unit.animation.GetOverlayAnimationIndex() = NONE;
				*unit->unit.animation.GetOverlayAnimationState() = NONE;
				*unit->unit.animation.GetReplacementAnimationIndex() = NONE;
				*unit->unit.animation.GetReplacementAnimationState() = NONE;
			}

			// set the target_unit's animation state to seat_exit to force them out of the vehicle
			*unit->unit.animation.GetAnimationState() = Enums::_unit_animation_state_seat_exit;
		}
	}

	// Plays the yelo_board animation
	static void BoardUnitSeatIndex(TagGroups::s_unit_boarding_seat const* boarding_seat_definition, 
		datum_index unit_index, datum_index parent_unit_index)
	{
		s_unit_datum* unit = (*Objects::ObjectHeader())[unit_index]->_unit;

		TagGroups::model_animation_graph const* animation_graph = GetObjectAnimations(unit_index);

		if (animation_graph != NULL)
		{
			sbyte animation_seat_block_index = *unit->unit.animation.GetSeatIndex();
			int32 unit_seat_animation_count = animation_graph->units[animation_seat_block_index].animations.Count;

			// Check if the unit has a board animation to play
			if (Enums::_unit_seat_animation_yelo_board < unit_seat_animation_count)
			{
				int32 animation_index = animation_graph->units[animation_seat_block_index].
					animations[Enums::_unit_seat_animation_yelo_board];

				// pick a random permutation of the boarding animation
				animation_index = Engine::AnimationPickRandomPermutation(true, 
					unit->object.animation.definition_index, animation_index);

				// set the unit's animation to yelo_board
				Engine::Objects::StartInterpolation(unit_index, 6);
				Engine::Objects::UnitSetAnimation(unit_index, 
					unit->object.animation.definition_index, animation_index);
				
				// reset  the unit's overlay animations
				*unit->unit.animation.GetOverlayAnimationIndex() = NONE;
				*unit->unit.animation.GetOverlayAnimationState() = NONE;
				*unit->unit.animation.GetReplacementAnimationIndex() = NONE;
				*unit->unit.animation.GetReplacementAnimationState() = NONE;

				// if boarding enters the target seat, use the seat_board animation state
				if (boarding_seat_definition->flags.boarding_enters_target_seat_bit)
					*unit->unit.animation.GetAnimationState() = Enums::_unit_animation_state_yelo_seat_board;
				// else, use the seat_exit animation state so we exit the vehicle when complete
				else
					*unit->unit.animation.GetAnimationState() = Enums::_unit_animation_state_seat_exit;
			}
		}
	}

	// Called on the primary_keyframe_index of the yelo_seat_board animation, 
	// ejects the target unit from their seat
	static void SeatBoardPrimaryKeyframe(datum_index unit_index)
	{
		s_unit_datum* unit = (*Objects::ObjectHeader())[unit_index]->_unit;

		datum_index parent_unit_index = unit->object.parent_object_index;
		s_unit_datum* parent_unit = (*Objects::ObjectHeader())[parent_unit_index]->_unit;
		int16 seat_index = unit->unit.vehicle_seat_index;

		TagGroups::s_unit_definition const* parent_unit_definition = 
			Objects::GetObjectDefinition<TagGroups::s_unit_definition>(parent_unit_index);
		TagGroups::s_unit_boarding_seat const* boarding_seat_definition = 
			DefinitionFindUnitUpgradesBoardingSeatBlock(parent_unit_index, seat_index);

		// Check if a boarding seat definition exists for the vehicle being exited
		if (boarding_seat_definition != NULL)
		{
			int16 target_seat_index = boarding_seat_definition->target_seat_index;
			datum_index target_unit_index = GetUnitInSeat(parent_unit_index, target_seat_index);

			if (target_unit_index != datum_index::null)
			{
				s_unit_datum* target_unit = (*Objects::ObjectHeader())[target_unit_index]->_unit;

				// Eject the target_unit
				EjectUnit(target_unit_index);

				// If the target seat is a drivers seat and boarding enters the seat, make the boarding unit the driver
				if (parent_unit_definition->unit.seats[target_seat_index].flags.driver_bit && 
					boarding_seat_definition->flags.boarding_enters_target_seat_bit)
				{
					parent_unit->unit.powered_seats_riders[Enums::_powered_seat_driver] = unit_index;
					parent_unit->unit.powered_seats_power[Enums::_powered_seat_driver] = 1.0f;
				}

				// If the boarding seat definition contains a damage effect tag, use it here
				if (boarding_seat_definition->boarding_damage.tag_index != datum_index::null)
				{
					Objects::s_damage_data damage_data;

					// Create a new damage_data struct based on the boarding_seat_definition boarding damage field
					damage_data.effect_definition_index = boarding_seat_definition->boarding_damage.tag_index;
					damage_data.responsible_player_index = unit->unit.controlling_player_index;
					damage_data.responsible_unit_index = unit_index;
					damage_data.responsible_units_team = unit->object.owner_team;
					damage_data.location = target_unit->object.scenario_location;
					damage_data.damage_position = target_unit->object.network.position;
					damage_data.damage_multiplier = 1.0f;

					// Damage the target_unit
					Engine::Objects::ObjectCauseDamage(damage_data, target_unit_index, NONE, NONE, NONE, 0);
				}
			}
		}
	}

	// Called on the final keyframe of the yelo_seat_board animation,
	// forces the boarding unit into the target seat when the board animation is complete
	static void SeatBoardFinalKeyframe(datum_index unit_index)
	{
		s_unit_datum* unit = (*Objects::ObjectHeader())[unit_index]->_unit;

		datum_index parent_unit_index = unit->object.parent_object_index;
		int16 seat_index = unit->unit.vehicle_seat_index;

		TagGroups::s_unit_boarding_seat const* boarding_seat_definition = 
			Objects::Units::DefinitionFindUnitUpgradesBoardingSeatBlock(parent_unit_index, seat_index);

		// Force the unit into the target seat
		if (boarding_seat_definition != NULL)
		{
			int16 target_seat_index = boarding_seat_definition->target_seat_index;
			datum_index target_unit_index = GetUnitInSeat(parent_unit_index, target_seat_index);

			// Eject the target_unit from it's seat if we haven't already
			if (!target_unit_index.IsNull())
				Engine::Objects::UnitExitSeatEnd(target_unit_index, true, true, false);

			// Enter the target seat
			Engine::Objects::UnitEnterSeat(unit_index, parent_unit_index, target_seat_index);
			
			// Close the vehicle if boarding controls open/close
			if (boarding_seat_definition->flags.controls_open_and_close_bit)
				Engine::Objects::UnitClose(parent_unit_index);
		}
	}

	// Called on the final keyframe of the seat_enter animation, 
	// determines if the seat is a boarding seat and whether or not to board the seat or eject the target unit
	static void SeatEnterFinalKeyframe(datum_index unit_index, int32* next_animation_state)
	{
		s_unit_datum* unit = (*Objects::ObjectHeader())[unit_index]->_unit;

		datum_index parent_unit_index = unit->object.parent_object_index;
		int16 seat_index = unit->unit.vehicle_seat_index;

		TagGroups::s_unit_boarding_seat const* boarding_seat_definition = 
			DefinitionFindUnitUpgradesBoardingSeatBlock(parent_unit_index, seat_index);

		// Check if a boarding seat definition exists for the vehicle seat being entered
		if (boarding_seat_definition != NULL)
		{
			// If boarding ejects the target seat, initiate the board sequence (plays board animation)
			if (boarding_seat_definition->flags.boarding_ejects_target_seat_bit)
				BoardUnitSeatIndex(boarding_seat_definition, unit_index, parent_unit_index);

			// if boarding enters the target seat, use the seat_board animation state
			if (boarding_seat_definition->flags.boarding_enters_target_seat_bit)
				*next_animation_state = Enums::_unit_animation_state_yelo_seat_board;
			// else, use the seat_exit animation state so we exit the vehicle when complete
			else
				*next_animation_state = Enums::_unit_animation_state_seat_exit;
			
			// Open the vehicle if boarding controls open/close
			if (boarding_seat_definition->flags.controls_open_and_close_bit)
				Engine::Objects::UnitOpen(parent_unit_index);
		}
	}

	// Replaces the engines unit_can_enter_seat function with our own to compensate for boarding and multiteam vehicles
	static bool UnitCanEnterSeat(datum_index unit_index, datum_index target_unit_index, int16 target_seat_index, datum_index* unit_in_seat)
	{
		bool result = true;

		TagGroups::s_unit_boarding_seat const* boarding_seat_definition = 
			Objects::Units::DefinitionFindUnitUpgradesBoardingSeatBlock(target_unit_index, target_seat_index);

		t_object_header_data object_header = (*Objects::ObjectHeader());
		s_unit_datum* unit = object_header[unit_index]->_unit;
		s_unit_datum* target_unit = object_header[target_unit_index]->_unit;

		if (unit_index == target_unit_index)
			result = false;

		for (datum_index first_object_index = target_unit->object.first_object_index; 
			 first_object_index != datum_index::null; 
			 first_object_index = object_header[first_object_index]->_object->next_object_index)
		{
			s_unit_datum* first_object = object_header[first_object_index]->_unit;

			if (first_object->object.VerifyType(Enums::_object_type_mask_unit))
			{
				// Check if the first_object is in the seat_index that unit_index is trying to enter
				if (first_object->unit.vehicle_seat_index == target_seat_index)
				{
					*unit_in_seat = first_object_index;
					result = false;
				}
				// If multiteam vehicles is prohibited, test unit and target unit teams
				else if (TagGroups::_global_yelo->gameplay.flags.prohibit_multiteam_vehicles_bit)
				{
					if (unit->unit.controlling_player_index == datum_index::null || 
						!Engine::Game::TeamIsEnemy(first_object->object.owner_team, unit->object.owner_team))
						result = true;
					else
						result = false;
				}
			}
		}

		// Check if the target seat is a boarding seat
		if (boarding_seat_definition != NULL)
		{
			datum_index boarded_unit_index = GetUnitInSeat(target_unit_index, boarding_seat_definition->target_seat_index);

			// If there is no unit to board or if they are an ally, disallow entry into the boarding seat
			if (boarded_unit_index == datum_index::null || !ObjectIsEnemy(unit_index, boarded_unit_index))
				result = false;
		}

		return result;
	}

	API_FUNC_NAKED static void PLATFORM_API UnitCanEnterSeatHook()
	{
		__asm {
			push	ebx
			push	ecx
			push	edx

			mov		ecx, [esp+24]
			mov		ebx, [esp+20]
			push	ecx		// datum_index* unit_in_seat
			push	ebx		// int16 target_seat_index
			push	edx		// datum_index target_unit_index
			push	eax		// datum_index unit_index
			call	UnitCanEnterSeat

			pop		edx
			pop		ecx
			pop		ebx

			retn
		}
	}

	void Initialize()
	{
		Memory::CreateHookRelativeCall(&UnitCanEnterSeatHook, GET_FUNC_VPTR(UNIT_CAN_ENTER_SEAT_HOOK), Enums::_x86_opcode_ret);
	}

	void Dispose()
	{
	}
};