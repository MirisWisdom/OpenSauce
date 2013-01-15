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

	void Initialize()
	{
		InitializePlayerExamineNearbyObjectsServerJmpTable();
		InitializePlayerExamineNearbyObjectsClientJmpTable();
	}
};