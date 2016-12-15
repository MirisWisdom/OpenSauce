/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Networking/MessageDeltas.hpp"

#include <blamlib/Halo1/networking/network_client_manager.hpp>
#include <blamlib/Halo1/networking/network_client_manager_structures.hpp>
#include <blamlib/Halo1/networking/network_game_globals.hpp>
#include <blamlib/Halo1/networking/network_game_manager_structures.hpp>
#include <blamlib/Halo1/networking/network_server_manager.hpp>

#include "Game/EngineFunctions.hpp"
#include "Memory/MemoryInterface.hpp"
#include "Networking/MessageDefinitions.hpp"
#include "Networking/Networking.hpp"

namespace Yelo
{
	namespace Enums
	{
		enum {
			k_mdp_packet_buffer_sent_data_size_in_bits = 0x7FF8,
		};
	};

	namespace MessageDeltas
	{
#define __EL_INCLUDE_ID			__EL_INCLUDE_NETWORKING
#define __EL_INCLUDE_FILE_ID	__EL_NETWORKING_MESSAGE_DELTAS
#include "Memory/_EngineLayout.inl"

		s_message_delta_parameters* Parameters()					PTR_IMP_GET2(message_delta_parameters);
		field_type_definition* GlobalFieldTypeList()				PTR_IMP_GET2(message_delta_global_field_type_list);
		Enums::message_delta_encoding_class* EncodingClass()		PTR_IMP_GET2(g_message_delta_encoding_class);
		exposed_parameters_t* ExposedProtocolParameters()			PTR_IMP_GET2(g_exposed_params);
		s_parameters_protocol_globals* ParametersProtocolGlobals()	PTR_IMP_GET2(mdp_parameters_protocol_globals);
		byte* PacketBufferReceived()								PTR_IMP_GET2(mdp_packet_buffer_received_data);
		byte* PacketBufferSent()									PTR_IMP_GET2(mdp_packet_buffer_sent_data);

		const message_delta_definition* const* OriginalPackets() { return GET_DPTR2(message_delta_packets); }

#ifndef YELO_NO_NETWORK
		void PLATFORM_API NetworkGameClientHandleMessageDeltaMessageBodyEx(); // forward declare

		static message_delta_definition* NewMessageDeltaList[Enums::k_message_deltas_new_count + 1]; // +1 so we can compile when there are no new deltas
		const message_delta_definition* const* NewPackets() { return NewMessageDeltaList; }

		void InitializeNewMessageDeltaList()
		{
			auto** org_mdptrs = CAST_QUAL(message_delta_definition**, MessageDeltas::OriginalPackets());

			// copy the original halopc delta definitions
			size_t x = 0;
			for(; x < Enums::k_message_deltas_count; x++)
				NewMessageDeltaList[x] = org_mdptrs[x];

			// copy the new yelo delta definitions
			for(size_t i = 0; i < Enums::k_message_deltas_yelo_count; i++, x++)
				NewMessageDeltaList[x] = kYeloMessageDeltas[i];
		}
		// Initialize the engine's code to reference our NewMessageDeltaList
		void InitializeEngineWithNewMessageDeltaList()
		{
			for(auto ptr : MessageDeltaPointerReferences)
				*ptr = NewMessageDeltaList;

			for(auto ptr : MessageDeltaPointerEndChecks)
				*ptr = &NewMessageDeltaList[Enums::k_message_deltas_new_count];

			for(auto ptr : MessageDeltaTypeCountChecks8bit)
				*ptr = Enums::k_message_deltas_new_count;
		}
		void InitializeYeloMessageDeltaDefinitions()
		{
			InitializeNewMessageDeltaList();
			InitializeEngineWithNewMessageDeltaList();

			// need a method to hook network_game_server_handle_message_delta_message so we can intercept client based packets
			Memory::WriteRelativeCall(&NetworkGameClientHandleMessageDeltaMessageBodyEx, GET_FUNC_VPTR(NETWORK_GAME_CLIENT_HANDLE_MESSAGE_DELTA_MESSAGE_BODY_CALL));
		}


		static bool NetworkGameClientGameSettingsUpdatedHook(Networking::s_network_game_client* client, const Networking::s_network_game* settings)
		{
			return true;
		}
		static API_FUNC_NAKED bool PLATFORM_API NetworkGameClientGameSettingsUpdatedCallHook(const Networking::s_network_game* settings)
		{
			static const uintptr_t CALL_ADDR = GET_FUNC_PTR(NETWORK_GAME_CLIENT_GAME_SETTINGS_UPDATED);

			__asm {
				push	ebx
				call	CALL_ADDR
				test	al, al
				jz		skip_hook
				push	settings
				push	ebx // client
				call	NetworkGameClientGameSettingsUpdatedHook
skip_hook:
				retn
			}
		}
#endif

		void Initialize()
		{
#ifndef YELO_NO_NETWORK

			if(Enums::k_message_deltas_yelo_count > 0)
			{
				InitializeYeloMessageDeltaDefinitions();

				Memory::WriteRelativeCall(NetworkGameClientGameSettingsUpdatedCallHook, 
					GET_FUNC_VPTR(NETWORK_GAME_CLIENT_GAME_SETTINGS_UPDATED_CALL1)); // network_game_client_handle_message_server_game_settings_update
				Memory::WriteRelativeCall(NetworkGameClientGameSettingsUpdatedCallHook, 
					GET_FUNC_VPTR(NETWORK_GAME_CLIENT_GAME_SETTINGS_UPDATED_CALL2)); // network_game_data_from_network
			}
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
		static bool NetworkGameClientHandleMessageDeltaMessageBodyOverride(Networking::s_network_game_client* client, message_dependant_header* header)
		{
			//int32 msg_type = header->decoding_information->definition_type;

			return false;
		}
		// Returns true if we handled a Yelo packet
		static bool NetworkGameClientHandleMessageDeltaMessageBodyYelo(Networking::s_network_game_client* client, message_dependant_header* header)
		{
			int32 msg_type = header->decoding_information->definition_type;

			if(msg_type >= Enums::k_message_deltas_count && 
				msg_type < Enums::k_message_deltas_new_count)
			{
				const MessageDeltas::packet_decoder& decoder = kYeloMessageDeltaDecoders[msg_type - Enums::k_message_deltas_count];

				// Validate that the packet can be decoded in our current network state
				if( TEST_FLAG(decoder.Flags, Enums::_message_deltas_new_client_bit) == GameState::IsClient() || 
					TEST_FLAG(decoder.Flags, Enums::_message_deltas_new_server_bit) == GameState::IsServer() )
					decoder.Proc(client, header);

				return true;
			}

			return false;
		}

		// Hooks the network_game_client_handle_message_delta_message_body function to allow us to intercept our own packets
		API_FUNC_NAKED static void PLATFORM_API NetworkGameClientHandleMessageDeltaMessageBodyEx()
		{
			static const uintptr_t TEMP_ASM_ADDR = GET_DATA_PTR(DONT_SEND_OBJECT_NEW_MSG);

			static const uintptr_t FUNCTION = GET_FUNC_PTR(NETWORK_GAME_CLIENT_HANDLE_MESSAGE_DELTA_MESSAGE_BODY);

			// I'm not taking any chances with our network code doing some nasty stuff to the 
			// registers so, we store them temporarily
			static message_dependant_header* local_store_header;
			static Networking::s_network_game_client* local_store_client;

			__asm
			{
				// eax = message_dependant_header*
				// esi = client data

				mov		byte ptr [TEMP_ASM_ADDR], TRUE
				mov		local_store_header, eax
				mov		local_store_client, ecx

				push	eax
				push	ecx
				call	NetworkGameClientHandleMessageDeltaMessageBodyYelo
				test	al, al
				jnz		the_end	// if not zero then we handled a Yelo message and not a stock game message

				mov		eax, local_store_header
				mov		ecx, local_store_client
				push	eax
				push	ecx
				call	NetworkGameClientHandleMessageDeltaMessageBodyOverride
				test	al, al
				jnz		the_end	// if not zero then we handled a stock message ourselves

				mov		ecx, local_store_client
				mov		eax, local_store_header
				call	FUNCTION

the_end:
				retn
			}
		}
#pragma endregion


#pragma region MdpiEncode
		API_FUNC_NAKED int32 MdpiEncode(long_enum mode, long_enum definition_type, 
			const void* buffer, size_t buffer_size_in_bits, 
			const void** headers, const void** datas, const void** baselines, 
			int32 iterations, int32 unk)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(MDPI_ENCODE);

			API_FUNC_NAKED_START()
				mov		edx, buffer_size_in_bits
				mov		eax, buffer
				push	unk
				push	iterations
				push	baselines
				push	datas
				push	headers
				push	definition_type
				push	mode
				call	FUNCTION
				add		esp, 4 * 7
			API_FUNC_NAKED_END(9)
		}
#pragma endregion

		int32 EncodeStateless(long_enum definition_type, 
			const void* source_header, const void* source_data, 
			void* buffer, size_t buffer_size_in_bits)
		{
			return MdpiEncode(Enums::_message_delta_mode_stateless, definition_type, 
				buffer, buffer_size_in_bits, 
				&source_header, &source_data);
		}
		int32 EncodeStateless(long_enum definition_type, 
			const void* source_header, const void* source_data)
		{
			return EncodeStateless(definition_type,
				source_header, source_data,
				PacketBufferSent(), Enums::k_mdp_packet_buffer_sent_data_size_in_bits);
		}

#pragma region DecodeStatelessIterated
		API_FUNC_NAKED bool DecodeStatelessIterated(message_dependant_header* header, void* destination_data)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(MDP_DECODE_STATELESS_ITERATED);

			API_FUNC_NAKED_START()
				mov		eax, header
				mov		ecx, destination_data
				call	FUNCTION
			API_FUNC_NAKED_END(2)
		}
#pragma endregion

		API_FUNC_NAKED bool DecodeIncrementalIterated(message_dependant_header* header, void* destination_data, 
			void* baseline_data, bool has_no_iteration_body)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(MDP_DECODE_INCREMENTAL_ITERATED);

			API_FUNC_NAKED_START()
				movzx	edx, has_no_iteration_body
				push	edx
				mov		edx, baseline_data
				mov		eax, header
				mov		ecx, destination_data
				call	FUNCTION
				add		esp, 4 * 1
			API_FUNC_NAKED_END(4)
		}

#pragma region DiscardIterationBody
		API_FUNC_NAKED void DiscardIterationBody(message_dependant_header* header)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(MDP_DISCARD_ITERATION_BODY);

			API_FUNC_NAKED_START()
				mov		eax, header
				call	FUNCTION
			API_FUNC_NAKED_END(1)
		}
#pragma endregion
#endif
	};
};