/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
namespace Animations
{
	namespace Enums
	{
		// An enumeration for each jmp_table we're replacing
		// Shows each jmp_entry and which animation states use it

		enum {
			unit_update_animation_primary_keyframe_jmp_0,		// 30, 31, 41
			unit_update_animation_primary_keyframe_jmp_1,		// 33
			unit_update_animation_primary_keyframe_jmp_default,	
		};
		
		enum {
			unit_update_animation_final_keyframe_jmp_0,			// 25
			unit_update_animation_final_keyframe_jmp_1,			// 26
			unit_update_animation_final_keyframe_jmp_2,			// 27
			unit_update_animation_final_keyframe_jmp_3,			// 37, 38
			unit_update_animation_final_keyframe_jmp_4,			// 39
			unit_update_animation_final_keyframe_jmp_default,
		};
		
		enum {
			unit_animation_state_interruptable_jmp_0,			// 2, 3, 37, 38
			unit_animation_state_interruptable_jmp_1,			// 23, 26-28
			unit_animation_state_interruptable_jmp_2,			// 24, 25
			unit_animation_state_interruptable_jmp_3,			// 29-31, 33-35, 39, 41
			unit_animation_state_interruptable_jmp_default,
		};
		
		enum {
			unit_animation_busy_jmp_0,			// 23-27, 29-35, 39, 41
			unit_animation_busy_jmp_default,
		};
		
		enum {
			unit_animation_state_loops_jmp_0,		// 1-3, 23, 26-31, 33-35, 39, 41
			unit_animation_state_loops_jmp_default,
		};
		
		enum {
			unit_animation_weapon_ik_jmp_0,			// 16-19, 23-35, 39-41
			unit_animation_weapon_ik_jmp_default,
		};
		
		enum {
			unit_animation_vehicle_ik_jmp_0,		// 23-27, 29, 34, 35
			unit_animation_vehicle_ik_jmp_default,
		};
		
		enum {
			unit_animation_start_overlay_action_jmp_0,			// 23-27, 29-35, 39, 41
			unit_animation_start_overlay_action_jmp_default,
		};
		
		enum {
			unit_is_busy_jmp_0,			// 23-27, 29-35, 39, 41
			unit_is_busy_jmp_default,
		};
	};

	static void* unit_update_animation_primary_keyframe_jmp_table_yelo[Yelo::Enums::_unit_animation_state_yelo];
	static void* unit_update_animation_final_keyframe_jmp_table_yelo[Yelo::Enums::_unit_animation_state_yelo];

	static byte unit_animation_state_interruptable_indirect_table_yelo[Yelo::Enums::_unit_animation_state_yelo];
	static byte unit_animation_busy_indirect_table_yelo[Yelo::Enums::_unit_animation_state_yelo];
	static byte unit_animation_state_loops_indirect_table_yelo[Yelo::Enums::_unit_animation_state_yelo];
	static byte unit_animation_weapon_ik_indirect_table_yelo[Yelo::Enums::_unit_animation_state_yelo];
	static byte unit_animation_vehicle_ik_indirect_table_yelo[Yelo::Enums::_unit_animation_state_yelo];
	static byte unit_animation_start_overlay_action_indirect_table_yelo[Yelo::Enums::_unit_animation_state_yelo];
	static byte unit_is_busy_indirect_table_yelo[Yelo::Enums::_unit_animation_state_yelo];

	// Replaces the UnitUpdateAnimation function's primary keyframe switch tables with our own
	// Called when an animation reaches the primary keyframe index value in the model_animations tag
	static void InitializeUnitUpdateAnimationPrimaryKeyframeJmpTable()
	{
		static const byte opcode_null[] = { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 };
		static const byte jmp_eax[] = { 0xFF, 0x24, 0x85 };

		int32 jmp_default = Enums::unit_update_animation_primary_keyframe_jmp_default;
		
		// write the game's default jump case into our jump table
		for(int32 x = 0; x < Yelo::Enums::_unit_animation_state_yelo; x++)
			unit_update_animation_primary_keyframe_jmp_table_yelo[x] = (void*)UNIT_UPDATE_ANIMATION_PRIMARY_KEYFRAME_JMP_TABLE[jmp_default];
		
		// remap the existing jump entries into our jump table at the correct indices
		unit_update_animation_primary_keyframe_jmp_table_yelo[Yelo::Enums::_unit_animation_state_melee] = 
			(void*)UNIT_UPDATE_ANIMATION_PRIMARY_KEYFRAME_JMP_TABLE[Enums::unit_update_animation_primary_keyframe_jmp_0];
		unit_update_animation_primary_keyframe_jmp_table_yelo[Yelo::Enums::_unit_animation_state_melee_airborne] = 
			(void*)UNIT_UPDATE_ANIMATION_PRIMARY_KEYFRAME_JMP_TABLE[Enums::unit_update_animation_primary_keyframe_jmp_0];
		unit_update_animation_primary_keyframe_jmp_table_yelo[Yelo::Enums::_unit_animation_state_leap_melee] = 
			(void*)UNIT_UPDATE_ANIMATION_PRIMARY_KEYFRAME_JMP_TABLE[Enums::unit_update_animation_primary_keyframe_jmp_0];
		unit_update_animation_primary_keyframe_jmp_table_yelo[Yelo::Enums::_unit_animation_state_throw_grenade] = 
			(void*)UNIT_UPDATE_ANIMATION_PRIMARY_KEYFRAME_JMP_TABLE[Enums::unit_update_animation_primary_keyframe_jmp_1];

		// set the game's jump table count to our's
		GET_PTR(unit_update_animation_primary_keyframe_jmp_tbl_count) = Yelo::Enums::_unit_animation_state_yelo - 1;
		// set the game's jump table address to our's
		GET_PTR(unit_update_animation_primary_keyframe_jmp_ptr) = unit_update_animation_primary_keyframe_jmp_table_yelo;		
		// nullify jmp table size adjustment
		Memory::WriteMemory(GET_FUNC_VPTR(UNIT_UPDATE_ANIMATION_PRIMARY_KEYFRAME_JMP_TABLE_ADJUST_SIZE), opcode_null, 3);
		// nullify indirect table references
		Memory::WriteMemory(GET_FUNC_VPTR(UNIT_UPDATE_ANIMATION_PRIMARY_KEYFRAME_INDIRECT_TABLE_PTR), opcode_null, sizeof(opcode_null));
	}

	// Replaces the UnitUpdateAnimation function's final keyframe switch tables with our own
	// Called when an animation reaches it's final keyframe
	static void InitializeUnitUpdateAnimationFinalKeyframeJmpTable()
	{
		static const byte opcode_null[] = { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 };
		static const byte jmp_eax[] = { 0xFF, 0x24, 0x85 };

		int32 jmp_default = Enums::unit_update_animation_final_keyframe_jmp_default;

		// write the game's default jump case into our jump table
		for(int32 x = 0; x < Yelo::Enums::_unit_animation_state_yelo; x++)
			unit_update_animation_final_keyframe_jmp_table_yelo[x] = (void*)UNIT_UPDATE_ANIMATION_FINAL_KEYFRAME_JMP_TABLE[jmp_default];
		
		// remap the existing jump entries into our jump table at the correct indices
		unit_update_animation_final_keyframe_jmp_table_yelo[Yelo::Enums::_unit_animation_state_landing_dead] = 
			(void*)UNIT_UPDATE_ANIMATION_FINAL_KEYFRAME_JMP_TABLE[Enums::unit_update_animation_final_keyframe_jmp_0];
		unit_update_animation_final_keyframe_jmp_table_yelo[Yelo::Enums::_unit_animation_state_seat_enter] = 
			(void*)UNIT_UPDATE_ANIMATION_FINAL_KEYFRAME_JMP_TABLE[Enums::unit_update_animation_final_keyframe_jmp_1];
		unit_update_animation_final_keyframe_jmp_table_yelo[Yelo::Enums::_unit_animation_state_seat_exit] = 
			(void*)UNIT_UPDATE_ANIMATION_FINAL_KEYFRAME_JMP_TABLE[Enums::unit_update_animation_final_keyframe_jmp_2];
		unit_update_animation_final_keyframe_jmp_table_yelo[Yelo::Enums::_unit_animation_state_surprise_front] = 
			(void*)UNIT_UPDATE_ANIMATION_FINAL_KEYFRAME_JMP_TABLE[Enums::unit_update_animation_final_keyframe_jmp_3];
		unit_update_animation_final_keyframe_jmp_table_yelo[Yelo::Enums::_unit_animation_state_surprise_back] = 
			(void*)UNIT_UPDATE_ANIMATION_FINAL_KEYFRAME_JMP_TABLE[Enums::unit_update_animation_final_keyframe_jmp_3];
		unit_update_animation_final_keyframe_jmp_table_yelo[Yelo::Enums::_unit_animation_state_leap_start] = 
			(void*)UNIT_UPDATE_ANIMATION_FINAL_KEYFRAME_JMP_TABLE[Enums::unit_update_animation_final_keyframe_jmp_4];

		// set the game's jump table count to our's
		GET_PTR(unit_update_animation_final_keyframe_jmp_tbl_count) = Yelo::Enums::_unit_animation_state_yelo - 1;
		// set the game's jump table address to our's
		GET_PTR(unit_update_animation_final_keyframe_jmp_ptr) = unit_update_animation_final_keyframe_jmp_table_yelo;		
		// nullify jmp table size adjustment
		Memory::WriteMemory(GET_FUNC_VPTR(UNIT_UPDATE_ANIMATION_FINAL_KEYFRAME_JMP_TABLE_ADJUST_SIZE), opcode_null, 3);
		// nullify indirect table references
		Memory::WriteMemory(GET_FUNC_VPTR(UNIT_UPDATE_ANIMATION_FINAL_KEYFRAME_INDIRECT_TABLE_PTR), opcode_null, sizeof(opcode_null));
		// fix jmp table index value
		Memory::WriteMemory(GET_FUNC_VPTR(UNIT_UPDATE_ANIMATION_FINAL_KEYFRAME_INDIRECT_TABLE_PTR + sizeof(opcode_null)), jmp_eax, sizeof(jmp_eax));
	}

	static bool PLATFORM_API UnitAnimationStateInterruptableOverride(s_unit_animation_data* unit_animation, int16 next_animation_state)
	{
		bool result = true;

		switch (*unit_animation->GetAnimationState())
		{
			case Yelo::Enums::_unit_animation_state_impulse:
			case Yelo::Enums::_unit_animation_state_melee:
			case Yelo::Enums::_unit_animation_state_melee_airborne:
			case Yelo::Enums::_unit_animation_state_throw_grenade:
			case Yelo::Enums::_unit_animation_state_resurrect_front:
			case Yelo::Enums::_unit_animation_state_resurrect_back:
			case Yelo::Enums::_unit_animation_state_leap_start:
			case Yelo::Enums::_unit_animation_state_leap_melee:
				if (next_animation_state != Yelo::Enums::_unit_animation_state_unknown23)
					result = false;
				break;
			case Yelo::Enums::_unit_animation_state_airborne_dead:
			case Yelo::Enums::_unit_animation_state_landing_dead:
				if (next_animation_state < Yelo::Enums::_unit_animation_state_airborne_dead || 
					next_animation_state > Yelo::Enums::_unit_animation_state_landing_dead)
					result = false;
				result = true;
				break;
			case Yelo::Enums::_unit_animation_state_turn_left:
			case Yelo::Enums::_unit_animation_state_turn_right:
			case Yelo::Enums::_unit_animation_state_surprise_front:
			case Yelo::Enums::_unit_animation_state_surprise_back:
				if (!next_animation_state)
					result = false;
				break;
			case Yelo::Enums::_unit_animation_state_unknown23:
			case Yelo::Enums::_unit_animation_state_seat_enter:
			case Yelo::Enums::_unit_animation_state_seat_exit:
			case Yelo::Enums::_unit_animation_state_custom_animation:
				result = false;
				break;
			default:
				return result;
		}

		return result;
	}

	// Replaces the UnitAnimationStateInterruptable function's switch table with our own
	// Returns whether or not an animation state can be interrupted
	static void InitializeUnitAnimationStateInterruptableJmpTable()
	{
		static const byte opcode_null[] = { 0x90, 0x90, 0x90 };

		// how much the stock indirect table is offset from the animation_state enum
		int32 offset = Yelo::Enums::_unit_animation_state_turn_left;

		// write the game's default jump case into our indirect table for all entries
		for(int32 x = 0; x < Yelo::Enums::_unit_animation_state_yelo; x++)
			unit_animation_state_interruptable_indirect_table_yelo[x] = Enums::unit_animation_state_interruptable_jmp_default;

		// copy the game's indirect table to our own and account for the offset
		for(int32 x = offset; x <= *K_NUMBER_OF_UNIT_ANIMATION_STATE_INTERRUPTABLE_INDIRECT_TBL_ENTRIES + offset; x++)
			unit_animation_state_interruptable_indirect_table_yelo[x] = UNIT_ANIMATION_STATE_INTERRUPTABLE_INDIRECT_TABLE[x-offset];
		
		// set the game's indirect table count to our's
		*K_NUMBER_OF_UNIT_ANIMATION_STATE_INTERRUPTABLE_INDIRECT_TBL_ENTRIES = Yelo::Enums::_unit_animation_state_yelo - 1;
		// set the game's indirect table address to our's
		GET_PTR(unit_animation_state_interruptable_indirect_tbl_ptr) = (void**)unit_animation_state_interruptable_indirect_table_yelo;
		// nullify jmp table size adjustment
		Memory::WriteMemory(GET_FUNC_VPTR(UNIT_ANIMATION_STATE_INTERRUPTABLE_INDIRECT_TABLE_ADJUST_SIZE), opcode_null, sizeof(opcode_null));
	}

	// Replaces the UnitAnimationBusy function's switch table with our own
	// Returns whether or not a unit playing the given animation state is busy
	static void InitializeUnitAnimationBusyJmpTable()
	{
		static const byte opcode_null[] = { 0x90, 0x90, 0x90 };

		// how much the stock indirect table is offset from the animation_state enum
		int32 offset = Yelo::Enums::_unit_animation_state_unknown23;

		// write the game's default jump case into our indirect table for all entries
		for(int32 x = 0; x < Yelo::Enums::_unit_animation_state_yelo; x++)
			unit_animation_busy_indirect_table_yelo[x] = Enums::unit_animation_busy_jmp_default;

		// copy the game's indirect table to our own and account for the offset
		for(int32 x = offset; x <= *K_NUMBER_OF_UNIT_ANIMATION_BUSY_INDIRECT_TBL_ENTRIES + offset; x++)
			unit_animation_busy_indirect_table_yelo[x] = UNIT_ANIMATION_BUSY_INDIRECT_TABLE[x-offset];

		// set the game's indirect table count to our's
		*K_NUMBER_OF_UNIT_ANIMATION_BUSY_INDIRECT_TBL_ENTRIES = Yelo::Enums::_unit_animation_state_yelo - 1;
		// set the game's indirect table address to our's
		GET_PTR(unit_animation_busy_indirect_tbl_ptr) = (void**)unit_animation_busy_indirect_table_yelo;

		// nullify jmp table size adjustment
		Memory::WriteMemory(GET_FUNC_VPTR(UNIT_ANIMATION_BUSY_INDIRECT_TABLE_ADJUST_SIZE), opcode_null, sizeof(opcode_null));
	}

	// Replaces the UnitAnimationStateLoops function's switch table with our own
	// Returns whether or not an animation state loops
	static void InitializeUnitAnimationStateLoopsJmpTable()
	{
		static const byte opcode_null[] = { 0x90 };

		// how much the stock indirect table is offset from the animation_state enum
		int32 offset = Yelo::Enums::_unit_animation_state_gesture;

		// write the game's default jump case into our indirect table for all entries
		for(int32 x = 0; x < Yelo::Enums::_unit_animation_state_yelo; x++)
			unit_animation_state_loops_indirect_table_yelo[x] = Enums::unit_animation_state_loops_jmp_default;

		// copy the game's indirect table to our own and account for the offset
		for(int32 x = offset; x <= *K_NUMBER_OF_UNIT_ANIMATION_STATE_LOOPS_INDIRECT_TBL_ENTRIES + offset; x++)
			unit_animation_state_loops_indirect_table_yelo[x] = UNIT_ANIMATION_STATE_LOOPS_INDIRECT_TABLE[x-offset];

		// set the game's indirect table count to our's
		*K_NUMBER_OF_UNIT_ANIMATION_STATE_LOOPS_INDIRECT_TBL_ENTRIES = Yelo::Enums::_unit_animation_state_yelo - 1;
		// set the game's indirect table address to our's
		GET_PTR(unit_animation_state_loops_indirect_tbl_ptr) = (void**)unit_animation_state_loops_indirect_table_yelo;
		// nullify jmp table size adjustment
		Memory::WriteMemory(GET_FUNC_VPTR(UNIT_ANIMATION_STATE_LOOPS_INDIRECT_TABLE_ADJUST_SIZE), opcode_null, sizeof(opcode_null));
	}

	// Replaces the UnitAnimationWeaponIK function's switch table with our own
	// Returns whether or not an animation state uses weapon IK
	static void InitializeUnitAnimationWeaponIKJmpTable()
	{
		static const byte opcode_null[] = { 0x90, 0x90, 0x90 };

		// how much the stock indirect table is offset from the animation_state enum
		int32 offset = Yelo::Enums::_unit_animation_state_ready;

		// write the game's default jump case into our indirect table for all entries
		for(int32 x = 0; x < Yelo::Enums::_unit_animation_state_yelo; x++)
			unit_animation_weapon_ik_indirect_table_yelo[x] = Enums::unit_animation_weapon_ik_jmp_default;

		// copy the game's indirect table to our own and account for the offset
		for(int32 x = offset; x <= *K_NUMBER_OF_UNIT_ANIMATION_WEAPON_IK_INDIRECT_TBL_ENTRIES + offset; x++)
			unit_animation_weapon_ik_indirect_table_yelo[x] = UNIT_ANIMATION_WEAPON_IK_INDIRECT_TABLE[x-offset];

		// set the game's indirect table count to our's
		*K_NUMBER_OF_UNIT_ANIMATION_WEAPON_IK_INDIRECT_TBL_ENTRIES = Yelo::Enums::_unit_animation_state_yelo - 1;
		// set the game's indirect table address to our's
		GET_PTR(unit_animation_weapon_ik_indirect_tbl_ptr) = (void**)unit_animation_weapon_ik_indirect_table_yelo;
		// nullify jmp table size adjustment
		Memory::WriteMemory(GET_FUNC_VPTR(UNIT_ANIMATION_WEAPON_IK_INDIRECT_TABLE_ADJUST_SIZE), opcode_null, sizeof(opcode_null));
	}

	// Replaces the UnitAnimationVehicleIK function's switch table with our own
	// Returns whether or not an animation state uses vehicle IK
	static void InitializeUnitAnimationVehicleIKJmpTable()
	{
		static const byte opcode_null[] = { 0x90, 0x90, 0x90 };

		// how much the stock indirect table is offset from the animation_state enum
		int32 offset = Yelo::Enums::_unit_animation_state_unknown23;

		// write the game's default jump case into our indirect table for all entries
		for(int32 x = 0; x < Yelo::Enums::_unit_animation_state_yelo; x++)
			unit_animation_vehicle_ik_indirect_table_yelo[x] = Enums::unit_animation_vehicle_ik_jmp_default;

		// copy the game's indirect table to our own and account for the offset
		for(int32 x = offset; x <= *K_NUMBER_OF_UNIT_ANIMATION_VEHICLE_IK_INDIRECT_TBL_ENTRIES + offset; x++)
			unit_animation_vehicle_ik_indirect_table_yelo[x] = UNIT_ANIMATION_VEHICLE_IK_INDIRECT_TABLE[x-offset];

		// set the game's indirect table count to our's
		*K_NUMBER_OF_UNIT_ANIMATION_VEHICLE_IK_INDIRECT_TBL_ENTRIES = Yelo::Enums::_unit_animation_state_yelo - 1;
		// set the game's indirect table address to our's
		GET_PTR(unit_animation_vehicle_ik_indirect_tbl_ptr) = (void**)unit_animation_vehicle_ik_indirect_table_yelo;
		// nullify jmp table size adjustment
		Memory::WriteMemory(GET_FUNC_VPTR(UNIT_ANIMATION_VEHICLE_IK_INDIRECT_TABLE_ADJUST_SIZE), opcode_null, sizeof(opcode_null));
	}

	// Replaces the UnitAnimationStartOverlayAction function's switch table with our own
	// Returns the overlay animation state of a unit and plays overlay animations if allowed
	static void InitializeUnitAnimationStartOverlayActionJmpTable()
	{
		static const byte opcode_null[] = { 0x90, 0x90, 0x90 };

		// how much the stock indirect table is offset from the animation_state enum
		int32 offset = Yelo::Enums::_unit_animation_state_unknown23;

		// write the game's default jump case into our indirect table for all entries
		for(int32 x = 0; x < Yelo::Enums::_unit_animation_state_yelo; x++)
			unit_animation_start_overlay_action_indirect_table_yelo[x] = Enums::unit_animation_start_overlay_action_jmp_default;

		// copy the game's indirect table to our own and account for the offset
		for(int32 x = offset; x <= *K_NUMBER_OF_UNIT_ANIMATION_START_OVERLAY_ACTION_INDIRECT_TBL_ENTRIES + offset; x++)
			unit_animation_start_overlay_action_indirect_table_yelo[x] = UNIT_ANIMATION_START_OVERLAY_ACTION_INDIRECT_TABLE[x-offset];

		// set the game's indirect table count to our's
		*K_NUMBER_OF_UNIT_ANIMATION_START_OVERLAY_ACTION_INDIRECT_TBL_ENTRIES = Yelo::Enums::_unit_animation_state_yelo - 1;
		// set the game's indirect table address to our's
		GET_PTR(unit_animation_start_overlay_action_indirect_tbl_ptr) = (void**)unit_animation_start_overlay_action_indirect_table_yelo;
		// nullify jmp table size adjustment
		Memory::WriteMemory(GET_FUNC_VPTR(UNIT_ANIMATION_START_OVERLAY_ACTION_INDIRECT_TABLE_ADJUST_SIZE), opcode_null, sizeof(opcode_null));
	}

	// Replaces the UnitIsBusy function's switch table with our own
	// Returns whether or not a unit playing the given animation state is busy
	static void InitializeUnitIsBusyJmpTable()
	{
		static const byte opcode_null[] = { 0x90, 0x90, 0x90 };

		// how much the stock indirect table is offset from the animation_state enum
		int32 offset = Yelo::Enums::_unit_animation_state_unknown23;

		// write the game's default jump case into our indirect table for all entries
		for(int32 x = 0; x < Yelo::Enums::_unit_animation_state_yelo; x++)
			unit_is_busy_indirect_table_yelo[x] = Enums::unit_is_busy_jmp_default;

		// copy the game's indirect table to our own and account for the offset
		for(int32 x = offset; x <= *K_NUMBER_OF_UNIT_IS_BUSY_INDIRECT_TBL_ENTRIES + offset; x++)
			unit_is_busy_indirect_table_yelo[x] = UNIT_IS_BUSY_INDIRECT_TABLE[x-offset];

		// set the game's indirect table count to our's
		*K_NUMBER_OF_UNIT_IS_BUSY_INDIRECT_TBL_ENTRIES = Yelo::Enums::_unit_animation_state_yelo - 1;
		// set the game's indirect table address to our's
		GET_PTR(unit_is_busy_indirect_tbl_ptr) = (void**)unit_is_busy_indirect_table_yelo;
		// nullify jmp table size adjustment
		Memory::WriteMemory(GET_FUNC_VPTR(UNIT_IS_BUSY_INDIRECT_TABLE_ADJUST_SIZE), opcode_null, sizeof(opcode_null));
	}

	// Called on the primary_keyframe_index of the yelo_seat_board animation, 
	// ejects the target unit from their seat
	static void SeatBoardPrimaryKeyframe(datum_index unit_index)
	{
		s_unit_datum* unit = (*Objects::ObjectHeader())[unit_index]->_unit;

		datum_index parent_unit_index = unit->object.parent_object_index;
		int16 seat_index = unit->unit.vehicle_seat_index;

		TagGroups::s_unit_boarding_seat const* boarding_seat_definition = 
			DefinitionFindUnitUpgradesBoardingSeatBlock(parent_unit_index, seat_index);

		// Check if a boarding seat definition exists for the vehicle being exited
		if (boarding_seat_definition != NULL)
		{
			int16 target_seat_index = boarding_seat_definition->target_seat_index;
			datum_index target_unit_index = GetUnitInSeat(parent_unit_index, target_seat_index);
			
			// Check if a target_unit exists in the target_seat_index
			if (target_unit_index != datum_index::null)
			{
				Yelo::Objects::s_unit_datum* target_unit = (*Objects::ObjectHeader())[target_unit_index]->_unit;
				Yelo::TagGroups::model_animation_graph const* animation_graph = GetObjectAnimations(target_unit_index);
				
				// Check if an animation graph exists for the target unit
				if (animation_graph != NULL)
				{
					sbyte animation_seat_block_index = *target_unit->unit.animation.GetSeatIndex();
					int32 unit_seat_animation_count = animation_graph->units[animation_seat_block_index].animations.Count;

					// Check if the target unit has an ejection animation to play
					if (Yelo::Enums::_unit_seat_animation_yelo_ejection < unit_seat_animation_count)
					{
						int32 animation_index = animation_graph->units[animation_seat_block_index].
							animations[Yelo::Enums::_unit_seat_animation_yelo_ejection];

						// pick a random permutation of the ejection animation
						animation_index = Yelo::Engine::AnimationPickRandomPermutation(true, 
							target_unit->object.animation.definition_index, animation_index);

						// set the target unit's animation to yelo_seat_ejection
						target_unit->object.animation.current_index = animation_index;
						target_unit->object.animation.current_frame_index = 0;
						*target_unit->unit.animation.GetAnimationState() = Yelo::Enums::_unit_animation_state_seat_exit;
					}
				}
			}
		}
	}

	// Called on the final keyframe of the yelo_seat_board animation,
	// forces the boarding unit into the target seat when the board animation is complete
	static void SeatBoardFinalKeyframe(datum_index unit_index)
	{
		Yelo::Objects::s_unit_datum* unit = (*Yelo::Objects::ObjectHeader())[unit_index]->_unit;

		datum_index parent_unit_index = unit->object.parent_object_index;
		int16 seat_index = unit->unit.vehicle_seat_index;

		Yelo::TagGroups::s_unit_boarding_seat const* boarding_seat_definition = 
			Yelo::Objects::Units::DefinitionFindUnitUpgradesBoardingSeatBlock(parent_unit_index, seat_index);

		// Force the unit into the target seat
		if (boarding_seat_definition != NULL)
		{
			Yelo::Engine::Objects::UnitEnterSeat
				(unit_index, parent_unit_index, boarding_seat_definition->target_seat_index);
			
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

		TagGroups::model_animation_graph const* animation_graph = GetObjectAnimations(unit_index);
		TagGroups::s_unit_boarding_seat const* boarding_seat_definition = 
			DefinitionFindUnitUpgradesBoardingSeatBlock(parent_unit_index, seat_index);

		// Check if a boarding seat definition exists for the vehicle seat being entered
		// and if an animation graph exists for the entering unit
		if (boarding_seat_definition != NULL && animation_graph != NULL)
		{
			// Check if boarding ejects the target seat
			if (boarding_seat_definition->flags.boarding_ejects_target_seat_bit)
			{
				sbyte seat_block_index = *unit->unit.animation.GetSeatIndex();
				int32 unit_seat_animation_count = animation_graph->units[seat_block_index].animations.Count;

				// Check if the unit has a board animation to play
				if (Yelo::Enums::_unit_seat_animation_yelo_board < unit_seat_animation_count)
				{
					int32 animation_index = animation_graph->units[seat_block_index].
						animations[Yelo::Enums::_unit_seat_animation_yelo_board];

					// pick a random permutation of the boarding animation
					animation_index = Yelo::Engine::AnimationPickRandomPermutation(true, 
						unit->object.animation.definition_index, animation_index);

					// set the unit's animation to yelo_board
					Yelo::Engine::Objects::StartInterpolation(unit_index, 6);
					unit->object.animation.current_index = animation_index;
					unit->object.animation.current_frame_index = 0;
					// reset  the unit's overlay animations
					*unit->unit.animation.GetOverlayAnimationIndex() = NONE;
					*unit->unit.animation.GetOverlayAnimationState() = NONE;
					*unit->unit.animation.GetReplacementAnimationIndex() = NONE;
					*unit->unit.animation.GetReplacementAnimationState() = NONE;

					// if boarding enters the target seat, use the seat_board animation state
					if (boarding_seat_definition->flags.boarding_enters_target_seat_bit)
					{
						*unit->unit.animation.GetAnimationState() = Yelo::Enums::_unit_animation_state_yelo_seat_board;
						*next_animation_state = Yelo::Enums::_unit_animation_state_yelo_seat_board;
					}
					// else, use the seat_exit animation state so we exit the vehicle when complete
					else
					{
						*unit->unit.animation.GetAnimationState() = Yelo::Enums::_unit_animation_state_seat_exit;
						*next_animation_state = Yelo::Enums::_unit_animation_state_seat_exit;
					}
				}

				// Open the vehicle if boarding controls open/close
				if (boarding_seat_definition->flags.controls_open_and_close_bit)
					Engine::Objects::UnitOpen(parent_unit_index);
			}
		}
	}

	// Called on the primary_keyframe_index of the yelo_seat_board animation
	static API_FUNC_NAKED void PLATFORM_API UnitUpdateAnimationPrimaryKeyframeSeatBoardJMP()
	{
		static uint32 RETN_ADDRESS = GET_FUNC_PTR(UNIT_UPDATE_ANIMATION_PRIMARY_KEYFRAME_SWITCH_RETN);

		__asm {
			pushad

			push	ebx		// datum_index boarding_unit_index
			call	SeatBoardPrimaryKeyframe

			popad

			jmp		RETN_ADDRESS
		}
	}

	// Called on the final keyframe of the yelo_seat_board animation
	static API_FUNC_NAKED void PLATFORM_API UnitUpdateAnimationFinalKeyframeSeatBoardJMP()
	{
		static uint32 RETN_ADDRESS = GET_FUNC_PTR(UNIT_UPDATE_ANIMATION_FINAL_KEYFRAME_SWITCH_RETN);

		__asm {
			pushad

			push	ebx		// datum_index unit_index
			call	SeatBoardFinalKeyframe

			popad

			jmp		RETN_ADDRESS
		}
	}

	// Called on the final keyframe of the seat enter animation
	static API_FUNC_NAKED void PLATFORM_API UnitUpdateAnimationFinalKeyframeSeatEnterJMP()
	{
		// use the seat_enter animation's final keyframe jmp entry as the return address
		static uint32 RETN_ADDRESS = (uint32)UNIT_UPDATE_ANIMATION_FINAL_KEYFRAME_JMP_TABLE
			[Enums::unit_update_animation_final_keyframe_jmp_1];

		__asm {
			pushad

			mov		eax, ebp
			sub		eax, 16	// address of the next_animation_state variable

			push	eax		// int32* next_animation_state
			push	ebx		// datum_index unit_index
			call	SeatEnterFinalKeyframe

			popad

			jmp		RETN_ADDRESS
		}
	}

	// Initializes custom yelo animation states
	// This is where we replace each switch table entry with our new animation states, 
	// which allows us to change the behavior of each animation state (looping, keyframe events, ending behavior, etc.)
	static void InitializeYeloAnimationStates()
	{
		/* UnitUpdateAnimationPrimaryKeyframe */
		//// Called when an animation reaches the primary keyframe index value in the model_animations tag
		unit_update_animation_primary_keyframe_jmp_table_yelo[Yelo::Enums::_unit_animation_state_seat_exit] = 
			&UnitUpdateAnimationPrimaryKeyframeSeatBoardJMP;
		unit_update_animation_primary_keyframe_jmp_table_yelo[Yelo::Enums::_unit_animation_state_yelo_seat_board] = 
			&UnitUpdateAnimationPrimaryKeyframeSeatBoardJMP;
		/* UnitUpdateAnimationPrimaryKeyframe switch table additions*/

		/* UnitUpdateAnimationFinalKeyframe */
		//// Called on the final keyframe of our animation
		unit_update_animation_final_keyframe_jmp_table_yelo[Yelo::Enums::_unit_animation_state_yelo_seat_board] = 
			&UnitUpdateAnimationFinalKeyframeSeatBoardJMP;
		unit_update_animation_final_keyframe_jmp_table_yelo[Yelo::Enums::_unit_animation_state_seat_enter] = 
			&UnitUpdateAnimationFinalKeyframeSeatEnterJMP;
		/* UnitUpdateAnimationFinalKeyframe */

		/* UnitAnimationStateInterruptable */
		//// Returns whether or not an animation state can be interrupted
		unit_animation_state_interruptable_indirect_table_yelo[Yelo::Enums::_unit_animation_state_yelo_seat_board] = 
			Enums::unit_animation_state_interruptable_jmp_1;
		/* UnitAnimationStateInterruptable */

		/* UnitAnimationBusy */
		//// Returns whether or not a unit playing the given animation state is busy
		unit_animation_busy_indirect_table_yelo[Yelo::Enums::_unit_animation_state_yelo_seat_board] = 
			Enums::unit_animation_busy_jmp_0;
		/* UnitAnimationBusy */

		/* UnitAnimationStateLoops */
		//// Returns whether or not an animation state loops
		unit_animation_state_loops_indirect_table_yelo[Yelo::Enums::_unit_animation_state_yelo_seat_board] = 
			Enums::unit_animation_state_loops_jmp_0;
		/* UnitAnimationStateLoops */

		/* UnitAnimationWeaponIK */
		//// Returns whether or not an animation state uses weapon IK
		unit_animation_weapon_ik_indirect_table_yelo[Yelo::Enums::_unit_animation_state_yelo_seat_board] = 
			Enums::unit_animation_weapon_ik_jmp_0;
		/* UnitAnimationWeaponIK */
		
		/* UnitAnimationVehicleIK */
		//// Returns whether or not an animation state uses vehicle IK
		unit_animation_vehicle_ik_indirect_table_yelo[Yelo::Enums::_unit_animation_state_yelo_seat_board] = 
			Enums::unit_animation_vehicle_ik_jmp_0;
		/* UnitAnimationVehicleIK */
		
		/* UnitAnimationStartOverlayAction */
		//// Returns the overlay animation state of a unit and plays overlay animations if allowed
		unit_animation_start_overlay_action_indirect_table_yelo[Yelo::Enums::_unit_animation_state_yelo_seat_board] = 
			Enums::unit_animation_start_overlay_action_jmp_0;
		/* UnitAnimationStartOverlayAction */
		
		/* UnitIsBusy */
		//// Returns whether or not a unit playing the given animation state is busy
		unit_is_busy_indirect_table_yelo[Yelo::Enums::_unit_animation_state_yelo_seat_board] = 
			Enums::unit_is_busy_jmp_0;
		/* UnitIsBusy */
	}

	void Initialize()
	{
		Animations::InitializeUnitUpdateAnimationPrimaryKeyframeJmpTable();
		Animations::InitializeUnitUpdateAnimationFinalKeyframeJmpTable();
		
		Animations::InitializeUnitAnimationStateInterruptableJmpTable();
		Animations::InitializeUnitAnimationBusyJmpTable();
		Animations::InitializeUnitIsBusyJmpTable();
		Animations::InitializeUnitAnimationStateLoopsJmpTable();
		Animations::InitializeUnitAnimationWeaponIKJmpTable();
		Animations::InitializeUnitAnimationVehicleIKJmpTable();
		Animations::InitializeUnitAnimationStartOverlayActionJmpTable();
		Animations::InitializeYeloAnimationStates();
	}

	void Dispose()
	{
	}
};