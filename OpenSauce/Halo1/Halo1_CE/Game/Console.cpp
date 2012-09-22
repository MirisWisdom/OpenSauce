/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Game/Console.hpp"

#include "Memory/MemoryInterface.hpp"
#include "Game/Players.hpp"
#include "Networking/MessageDeltas.hpp"
#include "Objects/Objects.hpp"

namespace Yelo
{
	namespace Console
	{
#define __EL_INCLUDE_ID			__EL_INCLUDE_GAME
#define __EL_INCLUDE_FILE_ID	__EL_GAME_CONSOLE
#include "Memory/_EngineLayout.inl"

		s_terminal_globals* TerminalGlobals()				DPTR_IMP_GET(terminal_globals);
		s_console_globals* Globals()						PTR_IMP_GET2(console_globals);
		s_hud_chat_globals_data* HudChatGlobals()			PTR_IMP_GET2(hud_chat_globals);
#if !PLATFORM_IS_DEDI
		int32 HudChatLineCount()							PTR_IMP_GET(hud_chat_line_count);
#endif

		void Initialize()
		{
#if !PLATFORM_DISABLE_UNUSED_CODE
			Memory::WriteRelativeCall(&Console::Update, GET_FUNC_VPTR(CONSOLE_UPDATE_HOOK));
#endif
		}

		void Dispose()
		{
		}

		API_FUNC_NAKED void PLATFORM_API Update(uint32 access_flags)
		{
			static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(CONSOLE_PROCESS_COMMAND);

			__asm {
				mov		eax, access_flags
				push	eax
				//mov		edi, edi // the command string buffer is given to us in edi
				call	TEMP_CALL_ADDR
				add		esp, 4
				retn
			}
		}

#ifndef YELO_NO_NETWORK
		bool SendHudChatToEveryonePredicate(Players::s_player_datum* player, 
			Players::s_player_datum* src_player, datum_index src_player_vehicle_index)
		{
			return true;
		}
		static bool SendHutChatToTeamPredicate(Players::s_player_datum* player, 
			Players::s_player_datum* src_player, datum_index src_player_vehicle_index)
		{
			return player->GetNetworkPlayer()->team_index == src_player->GetNetworkPlayer()->team_index;
		}
		static bool SendHudChatToPlayersInVehiclePredicate(Players::s_player_datum* player, 
			Players::s_player_datum* src_player, datum_index src_player_vehicle_index)
		{
			datum_index player_vehicle_index = player->GetVehicleIndex();
			if(player_vehicle_index.IsNull() || player_vehicle_index != src_player_vehicle_index)
				return false;

			return false;
		}

		static bool SendHudChatToPlayers(byte src_player_number, proc_send_hud_chat_predicate send_predicate,
			int32 message_size)
		{
			if(send_predicate == SendHudChatToEveryonePredicate)
				return MessageDeltas::SvSendMessageToAll(message_size);
			else
			{
				Players::s_player_datum* src_player = Players::GetPlayerFromNumber(src_player_number);
				datum_index src_player_vehicle_index = datum_index::null;
				if(src_player != NULL) src_player_vehicle_index = src_player->GetVehicleIndex();

				Players::t_players_data::Iterator iter(Players::Players());
				Players::s_player_datum* player;
				while( (player = iter.Next()) != NULL )
				{
					int32 player_machine_index = player->GetNetworkPlayer()->machine_index;
					if(	player_machine_index == NONE ||
						!send_predicate(player, src_player, src_player_vehicle_index))
						continue;

					if(!MessageDeltas::SvSendMessageToMachine(player_machine_index, message_size))
					{
						YELO_DEBUG_FORMAT("SendHudChatToPlayers failed for %S", player->GetNetworkPlayer()->name);
					}
				}
			}

			return true;
		}

		static bool SendHudChatServer(byte src_player_number, proc_send_hud_chat_predicate send_predicate,
			int32 message_size, Enums::hud_chat_type msg_type)
		{
			if(send_predicate == NULL)
			{
				switch(msg_type)
				{
				case Enums::_hud_chat_type_team: send_predicate = SendHutChatToTeamPredicate; break;
				case Enums::_hud_chat_type_vehicle: send_predicate = SendHutChatToTeamPredicate; break;

				default: send_predicate = SendHudChatToEveryonePredicate;
				}
			}

			return SendHudChatToPlayers(src_player_number, send_predicate, message_size);
		}

		static bool SendHudChatClient(int32 message_size)
		{
			return MessageDeltas::ClientSendMessageToServer(message_size);
		}
#endif
		void SendHudChat(Enums::hud_chat_type msg_type, wcstring message, byte src_player_number,
			proc_send_hud_chat_predicate send_predicate)
		{
#ifndef YELO_NO_NETWORK
			if(msg_type > Enums::_hud_chat_type_none && msg_type <= Enums::_hud_chat_type_info_msg)
			{
				if(msg_type == Enums::_hud_chat_type_server || msg_type == Enums::_hud_chat_type_info_msg)
					src_player_number = NONE;

				MessageDeltas::hud_chat_network_data network_data;
				network_data.msg_type = msg_type;
				network_data.player_number = src_player_number;
				network_data.message = message;

				void* encode_data = &network_data;
				int32 bits_encoded = MessageDeltas::EncodeStateless(
					Enums::_message_delta_mode_stateless,
					Enums::_message_delta_hud_chat,
					NULL, &encode_data);

				bool result = false;
				if(bits_encoded <= 0)
				{
						 if(Networking::IsServer()) result = SendHudChatServer(src_player_number, send_predicate, bits_encoded, msg_type);
					else if(Networking::IsClient()) result = SendHudChatClient(bits_encoded);
				}

				if(!result)
					YELO_DEBUG("SendHudChat failed", true);
			}
#endif
		}
	};
};