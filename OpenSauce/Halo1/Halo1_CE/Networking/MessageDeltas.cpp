/*
    Yelo: Open Sauce SDK
		Halo 1 (CE) Edition
    Copyright (C) 2005-2010  Kornner Studios (http://kornner.com)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "Common/Precompile.hpp"
#include "Networking/MessageDeltas.hpp"

#include "Game/EngineFunctions.hpp"
#include "Memory/MemoryInterface.hpp"
#include "Networking/MessageDefinitions.hpp"
#include "Networking/Networking.hpp"

namespace Yelo
{
	namespace MessageDeltas
	{
#define __EL_INCLUDE_ID			__EL_INCLUDE_NETWORKING
#define __EL_INCLUDE_FILE_ID	__EL_NETWORKING_MESSAGE_DELTAS
#include "Memory/_EngineLayout.inl"

		s_misc_encoding_globals* MiscEncodingGlobals()			PTR_IMP_GET2(misc_encoding_globals);
		Enums::message_delta_encoding_class* EncodingClass()	PTR_IMP_GET2(g_message_delta_encoding_class);
		byte* PacketBufferReceived()PTR_IMP_GET2(mdp_packet_buffer_received_data);
		byte* PacketBufferSent()	PTR_IMP_GET2(mdp_packet_buffer_sent_data);


#ifndef YELO_NO_NETWORK
		#pragma region message_delta_to_string_table
#ifdef API_DEBUG
		/*static*/ cstring message_delta_to_string_table[] = {
			"object_deletion",
			"projectile_update",
			"equipment_update",
			"weapon_update",
			"biped_update",
			"vehicle_update",
			"hud_add_item",
			"player_create",
			"player_spawn",
			"player_exit_vehicle",
			"player_set_action_result",
			"player_effect_start",
			"unit_kill",
			"client_game_update",
			"player_handle_powerup",
			"hud_chat",
			"slayer_update",
			"ctf_update",
			"oddball_update",
			"king_update",
			"race_update",
			"player_score_update",
			"game_engine_change_mode",
			"game_engine_map_reset",
			"multiplayer_hud_message",
			"multiplayer_sound",
			"team_change",
			"unit_drop_current_weapon",
			"vehicle_new",
			"biped_new",
			"projectile_new",
			"equipment_new",
			"weapon_new",
			"game_settings_update",
			"parameters_protocol",
			"local_player_update",
			"local_player_vehicle_update",
			"remote_player_action_update",
			"super_remote_players_action_update",
			"remote_player_position_update",
			"remote_player_vehicle_update",
			"remote_player_total_update_biped",
			"remote_player_total_update_vehicle",
			"weapon_start_reload",
			"weapon_ammo_pickup_mid_reload",
			"weapon_finish_reload",
			"weapon_cancel_reload",
			"netgame_equipment_new",
			"projectile_detonate",
			"item_accelerate",
			"damage_dealt",
			"projectile_attach",
			"client_to_server_pong",
			"super_ping_update",
			"sv_motd",
			"rcon_request",
			"rcon_response",

			"LOOOOK AT ME MOTHER FUCKER!!!111!!!",
			"update_script_global",
			"update_memory",
			"player_biped_update",
			"poop",
		};
#endif
		#pragma endregion


		void PLATFORM_API NetworkGameClientHandleMessageDeltaMessageBodyEx(); // forward declare

		static message_delta_definition* NewMessageDeltaList[Enums::k_message_deltas_new_count];
		const message_delta_definition* const* NewPackets() { return NewMessageDeltaList; }
#endif

		const message_delta_definition* const* OriginalPackets() { return GET_DPTR2(message_delta_packets); }

		void Initialize()
		{
#ifndef YELO_NO_NETWORK

#pragma region setup the message delta pointers (both old and new)
			message_delta_definition** org_mdptrs = CAST_QUAL(message_delta_definition**, MessageDeltas::OriginalPackets());
			message_delta_definition** new_mdptrs = CAST_QUAL(message_delta_definition**, MessageDeltas::NewPackets());

			// copy the original halopc message delta packets
			size_t x = 0;
			for(; x < Enums::k_message_deltas_count; x++)
				new_mdptrs[x] = org_mdptrs[x];

			for(size_t i = 0; i < Enums::k_message_deltas_new_count; i++, x++)
				new_mdptrs[x] = new_mdp_packets[i];
#pragma endregion

			for(x = 0; x < NUMBEROF(MessageDeltaPointerReferences); x++)
				*MessageDeltaPointerReferences[x] = NewMessageDeltaList;

			for(x = 0; x < NUMBEROF(MessageDeltaPointerEndChecks); x++)
				*MessageDeltaPointerEndChecks[x] = &NewMessageDeltaList[Enums::k_message_deltas_new_count];

			for(x = 0; x < NUMBEROF(MessageDeltaTypeCountChecks8bit); x++)
				*MessageDeltaTypeCountChecks8bit[x] = Enums::k_message_deltas_new_count;

			// need a method to hook network_game_server_handle_message_delta_message so we can intercept client based packets
			Memory::WriteRelativeCall(&NetworkGameClientHandleMessageDeltaMessageBodyEx, GET_FUNC_VPTR(NETWORK_GAME_CLIENT_HANDLE_MESSAGE_DELTA_MESSAGE_BODY_CALL));
#endif
		}

		void Dispose()
		{
		}


		static void EnableMessageDeltaSteroids()
		{
			// By forcing the encoding class to lan, we shouldn't have to worry about doing anything here.
			// However, once a new game starts, the code may re-initialize the encoding class. 
			// If we want permanent changes to the message delta encoding properties, we should do them here.
		}
		bool EnableNetworkingSteroids()
		{
			static bool g_enabled = false;

			// Always force the game to think it's running on a LAN
			*EncodingClass() = Enums::_message_delta_encoding_class_lan;

			if(!g_enabled)
			{
				Networking::network_update_globals* settings = Networking::UpdateSettings();
				settings->remote_player.action_baseline_update_rate = 30;
				settings->remote_player.position_update_rate = 15;
				settings->remote_player.position_baseline_update_rate = 15;

				EnableMessageDeltaSteroids();

				g_enabled = true;
			}

			return g_enabled;
		}



#ifndef YELO_NO_NETWORK

#pragma region Yelo packet handlers
		// Returns true if we handled a Yelo packet
		static bool NetworkGameClientHandleMessageDeltaMessageBodyYelo(Networking::s_network_game_client* client, message_dependant_header* header)
		{
			int32 msg_type = header->decoding_information->definition_type;
			//YELO_DEBUG_FORMAT("type: %X %s", msg_type, message_delta_to_string_table[msg_type]);

			if(msg_type >= Enums::k_message_deltas_count && 
				msg_type < Enums::k_message_deltas_new_count)
			{
				YELO_DEBUG("calling custom packet code...", true);
				const MessageDeltas::mdp_packet_decoder& decoder = new_mdp_packet_decoders[msg_type - Enums::k_message_deltas_count];

				// Validate that the packet can be decoded in our current network state
				if( TEST_BIT32(decoder.Flags, Enums::_message_deltas_new_client_bit) == Networking::IsClient() || 
					TEST_BIT32(decoder.Flags, Enums::_message_deltas_new_server_bit) == Networking::IsServer() )
					decoder.Proc(client, header);

				return true;
			}

			return false;
		}

		// Hooks the network_game_client_handle_message_delta_message_body function to allow us to intercept our own packets
		API_FUNC_NAKED static void PLATFORM_API NetworkGameClientHandleMessageDeltaMessageBodyEx()
		{
			static uint32 TEMP_ASM_ADDR = GET_DATA_PTR(DONT_SEND_OBJECT_NEW_MSG);

			static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(NETWORK_GAME_CLIENT_HANDLE_MESSAGE_DELTA_MESSAGE_BODY);

			// I'm not taking any chances with our network code doing some nasty stuff to the 
			// registers so, we store them temporarily
			static message_dependant_header* local_store_header;
			static Networking::s_network_game_client* local_store_client;

			__asm
			{
				// eax = message_dependant_header*
				// esi = client data

				mov		byte ptr [TEMP_ASM_ADDR], 0
				mov		local_store_header, eax
				mov		local_store_client, ecx

				push	eax
				push	ecx
				call	NetworkGameClientHandleMessageDeltaMessageBodyYelo
				test	al, al
				jnz		the_end	// if not zero then we handled a Yelo message and not a stock game message

				mov		ecx, local_store_client
				mov		eax, local_store_header
				call	TEMP_CALL_ADDR

the_end:
				retn
			}
		}
#pragma endregion


#pragma region SvWrite
		int32 SvWrite(int32 data_size_in_bits, int32 dont_bit_encode, int32 dont_flush)
		{
			static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(NETWORK_GAME_SERVER_WRITE);

			byte header_buffer = 1;
			int32 shit = dont_bit_encode ? 3 : 2;

			__asm {
				push	shit
				push	dont_flush
				push	dont_bit_encode
				push	1
				lea		edi, header_buffer
				push	edi
				push	GET_PTR2(mdp_packet_buffer_sent_data)
				mov		edi, GET_PTR2(global_network_game_server_data)
				mov		edi, [edi] // get the game server's s_network_connection
				mov		ebx, data_size_in_bits
				call	TEMP_CALL_ADDR
				add		esp, 4 * 6
			}
		}
#pragma endregion

#pragma region EncodeStateless
		int32 EncodeStateless(long_enum mode, long_enum def_type, void** headers, void** datas, void** baselines, int32 iterations, int32 unk)
		{
			static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(MDP_ENCODE_STATELESS);

			__asm {
				push	edx
				push	ecx

				mov		edx, 0x7FF8
				mov		eax, GET_PTR2(mdp_packet_buffer_sent_data)
				push	unk
				push	iterations
				push	baselines
				push	datas
				push	headers
				push	def_type
				push	mode
				call	TEMP_CALL_ADDR
				add		esp, 4 * 7

				pop		ecx
				pop		edx
			}
		}
#pragma endregion

#pragma region DecodeStatelessIterated
		bool DecodeStatelessIterated(void* header, void* out_buffer)
		{
			static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(MDP_DECODE_STATELESS_ITERATED);

			__asm {
				push	ecx
				push	edx

				mov		eax, header
				mov		ecx, out_buffer
				call	TEMP_CALL_ADDR

				pop		edx
				pop		ecx
			}
		}
#pragma endregion

#pragma region ClientSendMessageToServer
		void ClientSendMessageToServer(int32 data_size_in_bits)
		{
			static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(NETWORK_CONNECTION_FLUSH_QUEUE);
			static uint32 TEMP_CALL_ADDR2 = GET_FUNC_PTR(BITSTREAM_WRITE_BUFFER);

			if( !data_size_in_bits ) return;

			data_size_in_bits++;
			byte shit = 1;

			__asm {
				//push	edi
				push	ebx // if this is uncommented, the compiler catches it, but if it is commented, it fucking doesn't....
				push	edx // stupid compiler didn't catch this one....
				//push	ecx

				mov		eax, GET_PTR2(global_network_game_client_data)
				mov		edi, [eax+0xADC]
				test	byte ptr [edi+0xA8C], 1
				jnz		_the_exit

				mov		ecx, [edi+0x1C]
				mov		edx, [edi+0x24]
				mov		eax, [edi+0x20]
				lea		esi, [edi+0x10]
				shl		ecx, 3
				sub		edx, ecx
				sub		edx, eax
				inc		edx
				cmp		ebx, edx
				jle		bit_writes
				push	1
				push	edi
				call	TEMP_CALL_ADDR // NETWORK_CONNECTION_FLUSH_QUEUE
				add		esp, 4 * 2
				test	al, al
				jz		_the_exit

bit_writes:
				add		[edi+0xA80], ebx
				push	1
				lea		ecx, shit
				mov		eax, esi
				call	TEMP_CALL_ADDR2 // BITSTREAM_WRITE_BUFFER
				add		esp, 4 * 1

				dec		data_size_in_bits // had to avoid using ebp in this as we need it for using the params
				push	data_size_in_bits
				mov		ecx, GET_PTR2(mdp_packet_buffer_sent_data)
				mov		eax, esi
				mov		byte ptr [esi+0x1C], 0
				call	TEMP_CALL_ADDR2
				add		esp, 4 * 1
				mov		byte ptr [esi+0x1C], 0

_the_exit:
				//pop		ecx
				pop		edx
				pop		ebx
				//pop		edi
			}
		}
#pragma endregion

#pragma region SvSendMessageToAll
		bool SvSendMessageToAll(int32 data_size_in_bits, int32 dont_bit_encode, int32 dont_flush, int32 send_even_after_fail, int32 shit)
		{
			static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(NETWORK_GAME_SERVER_SEND_MESSAGE_TO_ALL_MACHINES); // all machines in-game

			if(shit == -1)
				shit = dont_bit_encode ? 3 : 2;

			__asm {
				push	ecx
				push	edx

				push	shit
				push	send_even_after_fail // false = if fail, don't send
				push	dont_flush
				push	dont_bit_encode
				push	GET_PTR2(mdp_packet_buffer_sent_data)
				push	1 // new packet system, 0 for old packet system
				mov		ecx, GET_PTR2(global_network_game_server_data)
				mov		eax, data_size_in_bits
				call	TEMP_CALL_ADDR
				add		esp, 4 * 6

				pop		edx
				pop		ecx
			}
		}
#pragma endregion

#pragma region SvSendMessageToMachine
		bool SvSendMessageToMachine(int32 machine_index, int32 data_size_in_bits, int32 dont_bit_encode, int32 dont_flush, int32 send_even_after_fail, int32 shit)
		{
			static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(NETWORK_GAME_SERVER_SEND_MESSAGE_TO_MACHINE);

			if(shit == -1)
				shit = dont_bit_encode ? 3 : 2;

			__asm {
				push	edx
				push	esi

				push	shit
				push	send_even_after_fail // false = if fail, don't send
				push	dont_flush
				push	dont_bit_encode
				push	data_size_in_bits
				push	GET_PTR2(mdp_packet_buffer_sent_data)
				push	1
				mov		eax, machine_index
				mov		esi, GET_PTR2(global_network_game_server_data)
				call	TEMP_CALL_ADDR
				add		esp, 4 * 7

				pop		esi
				pop		edx
			}
		}
#pragma endregion

#pragma region DiscardIterationBody
		void DiscardIterationBody(void* header)
		{
			static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(MDP_DISCARD_ITERATION_BODY);

			__asm {
				push	ecx
				push	edx

				mov		eax, header
				call	TEMP_CALL_ADDR

				pop		edx
				pop		ecx
			}
		}
#pragma endregion
#endif
	};
};