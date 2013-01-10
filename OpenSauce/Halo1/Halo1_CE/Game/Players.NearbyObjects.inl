/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/

namespace NearbyObjects
{
	static void* player_examine_nearby_objects_server_jmp_table_yelo[Yelo::Enums::_object_type_sound_scenery+1];
	static void* player_examine_nearby_objects_client_jmp_table_yelo[Yelo::Enums::_object_type_sound_scenery+1];
	
	// Replaces PlayerExamineNearbyObjectsServer function's switch table with our own
	static void InitializePlayerExamineNearbyObjectsServerJmpTable()
	{
		// copy the game's jmp table to our own
		for(int32 x = 0; x <= Yelo::Enums::_object_type_control; x++)
			player_examine_nearby_objects_server_jmp_table_yelo[x] = (void*)PLAYER_EXAMINE_NEARBY_OBJECTS_SERVER_JMP_TABLE[x];

		// write the game's default jump case into our jump table for cases 9-11
		for(int32 x = Yelo::Enums::_object_type_light_fixture; x <= Yelo::Enums::_object_type_sound_scenery; x++)
			player_examine_nearby_objects_server_jmp_table_yelo[x] = (void*)PLAYER_EXAMINE_NEARBY_OBJECTS_SERVER_JMP_TABLE[0];
		
		// set the game's client jump table count to our's
		*K_NUMBER_OF_PLAYER_EXAMINE_NEARBY_OBJECTS_SERVER_JMP_TABLE_ENTRIES = Yelo::Enums::_object_type_sound_scenery;
		// set the game's client jump table address to our's
		GET_PTR(player_examine_nearby_objects_server_jmp_ptr) = player_examine_nearby_objects_server_jmp_table_yelo;
	}

	// Replaces PlayerExamineNearbyObjectsClient function's switch table with our own
	static void InitializePlayerExamineNearbyObjectsClientJmpTable()
	{
		// copy the game's jmp table to our own
		for(int32 x = 0; x <= Yelo::Enums::_object_type_control; x++)
			player_examine_nearby_objects_client_jmp_table_yelo[x] = (void*)PLAYER_EXAMINE_NEARBY_OBJECTS_CLIENT_JMP_TABLE[x];

		// write the game's default jump case into our jump table for cases 9-11
		for(int32 x = Yelo::Enums::_object_type_light_fixture; x <= Yelo::Enums::_object_type_sound_scenery; x++)
			player_examine_nearby_objects_client_jmp_table_yelo[x] = (void*)PLAYER_EXAMINE_NEARBY_OBJECTS_CLIENT_JMP_TABLE[0];
		
		// set the game's client jump table count to our's
		*K_NUMBER_OF_PLAYER_EXAMINE_NEARBY_OBJECTS_CLIENT_JMP_TABLE_ENTRIES = Yelo::Enums::_object_type_sound_scenery;
		// set the game's client jump table address to our's
		GET_PTR(player_examine_nearby_objects_client_jmp_ptr) = player_examine_nearby_objects_client_jmp_table_yelo;
	}

	// Checks if the player can enter the specified seat and accounts for boarding seats
	static void PlayerCheckNearbyVehicleSeat(datum_index unit_index, datum_index vehicle_index)
	{
		Objects::s_unit_datum* unit = (*Objects::ObjectHeader())[unit_index]->_unit;
		datum_index player_index = unit->object.player_index;
		
		int16 nearby_seat_index = NONE;
		int16 nearby_seat_result = Engine::Objects::UnitFindNearbySeat(unit_index, vehicle_index, &nearby_seat_index) - 1;

		if (nearby_seat_result)
		{
			// nearby_seat_result says we can enter the seat
			if (nearby_seat_result == 1)
			{
				int32 action_result = Enums::_player_action_result_enter_seat;
				TagGroups::s_unit_boarding_seat const* unit_boarding_seat_definition = 
					Objects::Units::DefinitionFindUnitUpgradesBoardingSeatBlock(vehicle_index, nearby_seat_index);

				// Check if a unit_boarding_seat_definition exists for this vehicle
				if (unit_boarding_seat_definition != NULL)
				{
					datum_index target_unit_index = datum_index::null;
					int16 target_seat_index = unit_boarding_seat_definition->target_seat_index;

					// Get the unit in the target_seat_index
					Engine::Objects::UnitCanEnterSeat(unit_index, vehicle_index, target_seat_index, &target_unit_index);

					// If there is an enemy unit in the target_seat_index, allow the player to enter
					if (target_unit_index != datum_index::null && Objects::ObjectIsEnemy(unit_index, target_unit_index))
						Engine::Players::PlayerSetActionResult(player_index, vehicle_index, action_result, nearby_seat_index);

					// return so we avoid setting the player action result again
					return;
				}

				Engine::Players::PlayerSetActionResult(player_index, vehicle_index, action_result, nearby_seat_index);
			}
		}
		// nearby_seat_result says the seat is occupied by an ally ai
		else
		{
			Engine::Players::PlayerSetActionResult
				(player_index, vehicle_index, Enums::_player_action_result_force_ai_to_exit_seat, nearby_seat_index);
		}
	}

	static API_FUNC_NAKED void PLATFORM_API PlayerExamineNearbyVehicleHook()
	{
		__asm {
			push	ebp		// datum_index vehicle_index
			push	ebx		// datum_index unit_index
			call	PlayerCheckNearbyVehicleSeat

			pop     edi
			pop		esi
			pop		ebp
			pop		ebx
			retn
		}
	}

	void Initialize()
	{
		InitializePlayerExamineNearbyObjectsServerJmpTable();
		InitializePlayerExamineNearbyObjectsClientJmpTable();

		Memory::WriteRelativeJmp(PlayerExamineNearbyVehicleHook, GET_FUNC_VPTR(PLAYER_EXAMINE_NEARBY_VEHICLE_HOOK), true);
	}
};