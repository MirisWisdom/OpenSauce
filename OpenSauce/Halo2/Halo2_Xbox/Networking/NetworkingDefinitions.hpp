/*
	Yelo: Open Sauce SDK
		Halo 2 (Xbox) Edition

	See license\OpenSauce\Halo2_Xbox for specific license information
*/
#pragma once

namespace Yelo
{
	namespace Enums
	{
		enum bitstream_type
		{
			_bitstream_type_none,
			_bitstream_type_identifer,
			_bitstream_type_integer,
			_bitstream_type_signed_integer,
			_bitstream_type_point3d,
			_bitstream_type_boolean,
			_bitstream_type_raw_data,
			_bitstream_type_string,
			_bitstream_type_wchar_string,
			_bitstream_type_secure_address,
			_bitstream_type_quantized_real,
			_bitstream_type_logarithmic_quantized_real,
			_bitstream_type_unit_vector,
			_bitstream_type_vector,
			_bitstream_type_axes,
			_bitstream_type,
		};

		enum packet_mode
		{
			_packet_mode_channel,
			_packet_mode_loopback_client,
			_packet_mode_loopback_server,
			_packet_mode_out_of_band,
			_packet_mode,
		};

		enum network_message_type
		{
			_network_message_type_out_of_band,
			_network_message_type_connected,
			_network_message_type_session_protocol,
			_network_message_type_session_membership,
			_network_message_type_session_parameters,
			_network_message_type_simulation,
			_network_message_type_simulation_synchronous,
			_network_message_type_simulation_distributed,
			_network_message_type_test,
			_network_message_type,
		};

		enum network_message
		{
			_network_message_ping,
			_network_message_pong,
			_network_message_broadcast_search,
			_network_message_broadcast_reply,

			_network_message_connect_request,
			_network_message_connect_refuse,
			_network_message_connect_establish,
			_network_message_connect_closed,

			_network_message_join_request,
			_network_message_join_abort,
			_network_message_join_refuse,
			_network_message_leave_session,
			_network_message_leave_acknowledge,
			_network_message_session_disband,
			_network_message_session_boot,
			_network_message_host_handoff,
			_network_message_peer_handoff,
			_network_message_host_transition,
			_network_message_host_reestablish,
			_network_message_host_decline,
			_network_message_peer_reestablish,
			_network_message_peer_establish,
			_network_message_election,
			_network_message_election_refuse,
			_network_message_time_synchronize,

			_network_message_membership_update,
			_network_message_peer_properties,
			_network_message_delegate_leadership,
			_network_message_boot_machine,
			_network_message_player_add,
			_network_message_player_refuse,
			_network_message_player_remove,
			_network_message_player_properties,

			_network_message_parameters_update,
			_network_message_parameters_request,
			_network_message_countdown_timer,
			_network_message_mode_acknowledge,

			_network_message_view_establistment,
			_network_message_player_acknowledge,

			_network_message_synchronous_update,
			_network_message_synchronous_actions,
			_network_message_synchronous_join,
			_network_message_synchronous_gamestate,

			_network_message_game_results,

			_network_message_test,

			_network_message,
		};
	};

	namespace Networking
	{
		struct s_message_type_definition
		{
			bool initialized; // 0x0
			PAD24;
			cstring name; // 0x4
			PAD32; // 0x8
			uint32 message_size; // 0xC
			uint32 message_size_maximum; // 0x10
			void* encode_function; // 0x14
			void* decode_function; // 0x18
		};

		// s_player_prediction = 64 bytes
		// s_network_storage_file_header = 16 bytes
		// s_network_configuration = 3984 bytes

		struct s_network_storage_item
		{
			//byte_flags m_state_flags; // 0x4, _network_storage_item_state_static = FLAGS(0)
			//long_enum m_type; // 0x8, _network_storage_item_invalid_type = 0
			//_enum m_current_action; // 0xA, _network_storage_item_idle = 0
			//int32 m_queue_id; // 0x20
			//byte* m_buffer; // 0x24, get_file_header
			//uint32 m_total_size; // 0x28
		};
	};
};