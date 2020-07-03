/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/

namespace ActionResults
{
	static void* player_handle_action_jmp_table_yelo[Enums::k_number_of_player_action_results_yelo];

	// Replaces PlayerHandleAction function's switch table with our own
	static void InitializePlayerHandleActionJmpTable()
	{
		// how much the stock jmp table is offset from the action_result enum
		const int32 offset = Enums::_player_action_result_swap_equipment;

		// write the game's default jump case into our jump table
		for (auto& ptr : player_handle_action_jmp_table_yelo)
			ptr = GET_PTR2(PLAYER_HANDLE_ACTION_JMP_TABLE)[1];

		// copy the game's jmp table to our own and account for the offset
		int32 handle_action_last_entry_type = GET_PTR(PLAYER_HANDLE_ACTION_LAST_ENTRY_TYPE);
		for (int32 x = offset; x <= handle_action_last_entry_type + offset; x++)
			player_handle_action_jmp_table_yelo[x] = GET_PTR2(PLAYER_HANDLE_ACTION_JMP_TABLE)[x - offset];

		static const byte k_null_opcode[] = { Enums::_x86_opcode_nop, Enums::_x86_opcode_nop, Enums::_x86_opcode_nop };

		// nullify jmp table size adjustment
		Memory::WriteMemory(GET_FUNC_VPTR(PLAYER_HANDLE_ACTION_JMP_TABLE_ADJUST_SIZE), k_null_opcode);
		// set the game's jump table count to our's
		GET_PTR(PLAYER_HANDLE_ACTION_LAST_ENTRY_TYPE) = Enums::k_number_of_player_action_results_yelo - 1;
		// set the game's jump table address to our's
		GET_PTR(player_handle_action_jmp_ptr) = player_handle_action_jmp_table_yelo;
	}
	
	// Initialize custom yelo action results
	static void InitializeYeloActionResults()
	{
		// Example:
		//player_handle_action_jmp_table_yelo[Enums::_player_action_result_yelo] = &JMPFunction;
		static_assert(Enums::k_number_of_player_action_results_yelo == Enums::k_number_of_player_action_results,
			"New player_action_results were defined, but I don't think you've added handlers for them yet");
	}

	void Initialize()
	{
		InitializePlayerHandleActionJmpTable();
		InitializeYeloActionResults();
	}
};