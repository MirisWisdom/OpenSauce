/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI
#include "Memory/1.10/Pointers/HaloCE_110_Runtime.Networking.inl"
#include "Memory/1.10/Pointers/HaloCE_110_Runtime_Manual.Networking.inl"
#else
#include "Memory/1.10/Pointers/HaloCE_110_Dedi.Networking.inl"
#include "Memory/1.10/Pointers/HaloCE_110_Dedi_Manual.Networking.inl"
#endif

#include <blamlib/networking/message_delta_definitions.hpp>
#include <Memory/MemoryInterface.hpp>
#include <yelolib/memory/memory_interface_base.hpp>

namespace Yelo
{
	namespace MessageDeltas
	{
		FUNC_PTR(MDP_FIELD_ENCODE_ARRAY, DUO_PTR(K_MDP_FIELD_ENCODE_ARRAY));
		FUNC_PTR(MDP_FIELD_DECODE_ARRAY, DUO_PTR(K_MDP_FIELD_DECODE_ARRAY));

		ENGINE_PTR(field_properties_definition, message_delta_field_integer_small, DUO_PTR(K_MESSAGE_DELTA_FIELD_INTEGER_SMALL));
		ENGINE_PTR(field_properties_definition, message_delta_field_integer_medium, DUO_PTR(K_MESSAGE_DELTA_FIELD_INTEGER_MEDIUM));
		ENGINE_PTR(field_properties_definition, message_delta_field_integer_large, DUO_PTR(K_MESSAGE_DELTA_FIELD_INTEGER_LARGE));
		ENGINE_PTR(field_properties_definition, message_delta_field_real, DUO_PTR(K_MESSAGE_DELTA_FIELD_REAL));
		ENGINE_PTR(field_properties_definition, message_delta_field_boolean, DUO_PTR(K_MESSAGE_DELTA_FIELD_BOOLEAN));
		ENGINE_PTR(field_properties_definition, message_delta_field_ascii_character, DUO_PTR(K_MESSAGE_DELTA_FIELD_ASCII_CHARACTER));
		ENGINE_PTR(field_properties_definition, message_delta_field_time, DUO_PTR(K_MESSAGE_DELTA_FIELD_TIME));
		ENGINE_PTR(field_properties_definition, message_delta_field_grenade_counts, DUO_PTR(K_MESSAGE_DELTA_FIELD_GRENADE_COUNTS));
		ENGINE_PTR(field_properties_definition, message_delta_field_locality_reference_position, DUO_PTR(K_MESSAGE_DELTA_FIELD_LOCALITY_REFERENCE_POSITION));
		ENGINE_PTR(field_properties_definition, message_delta_field_digital_throttle, DUO_PTR(K_MESSAGE_DELTA_FIELD_DIGITAL_THROTTLE));
		ENGINE_PTR(field_properties_definition, message_delta_field_fixed_width_weapon_index, DUO_PTR(K_MESSAGE_DELTA_FIELD_FIXED_WIDTH_WEAPON_INDEX));
		ENGINE_PTR(field_properties_definition, message_delta_field_fixed_width_grenade_index, DUO_PTR(K_MESSAGE_DELTA_FIELD_FIXED_WIDTH_GRENADE_INDEX));
		ENGINE_PTR(field_properties_definition, message_delta_field_item_placement_position, DUO_PTR(K_MESSAGE_DELTA_FIELD_ITEM_PLACEMENT_POSITION));

		ENGINE_PTR(field_properties_definition, message_delta_field_local_player_update_sequence_number, DUO_PTR(K_MESSAGE_DELTA_FIELD_LOCAL_PLAYER_UPDATE_SEQUENCE_NUMBER));
		ENGINE_PTR(field_properties_definition, message_delta_field_remote_player_update_sequence_number, DUO_PTR(K_MESSAGE_DELTA_FIELD_REMOTE_PLAYER_UPDATE_SEQUENCE_NUMBER));
		ENGINE_PTR(field_properties_definition, message_delta_field_remote_player_action_update_baseline_id, DUO_PTR(K_MESSAGE_DELTA_FIELD_REMOTE_PLAYER_ACTION_UPDATE_BASELINE_ID));
		ENGINE_PTR(field_properties_definition, message_delta_field_update_tick_count, DUO_PTR(K_MESSAGE_DELTA_FIELD_UPDATE_TICK_COUNT));
		ENGINE_PTR(field_properties_definition, message_delta_field_update_id, DUO_PTR(K_MESSAGE_DELTA_FIELD_UPDATE_ID));
		ENGINE_PTR(field_properties_definition, message_delta_field_fixed_width_3bits, DUO_PTR(K_MESSAGE_DELTA_FIELD_FIXED_WIDTH_3BITS));
		ENGINE_PTR(field_properties_definition, message_delta_field_fixed_width_6bits, DUO_PTR(K_MESSAGE_DELTA_FIELD_FIXED_WIDTH_6BITS));
		ENGINE_PTR(field_properties_definition, message_delta_field_fixed_width_1bit, DUO_PTR(K_MESSAGE_DELTA_FIELD_FIXED_WIDTH_1BIT));
		ENGINE_PTR(field_properties_definition, message_delta_field_fixed_width_normal_4bit, DUO_PTR(K_MESSAGE_DELTA_FIELD_FIXED_WIDTH_NORMAL_4BIT));
		ENGINE_PTR(field_properties_definition, message_delta_field_fixed_width_normal_8bit, DUO_PTR(K_MESSAGE_DELTA_FIELD_FIXED_WIDTH_NORMAL_8BIT));
		ENGINE_PTR(field_properties_definition, message_delta_field_fixed_width_normal_16bit, DUO_PTR(K_MESSAGE_DELTA_FIELD_FIXED_WIDTH_NORMAL_16BIT));
		ENGINE_PTR(field_properties_definition, message_delta_field_object_index, DUO_PTR(K_MESSAGE_DELTA_FIELD_OBJECT_INDEX));
		ENGINE_PTR(field_properties_definition, message_delta_field_player_index, DUO_PTR(K_MESSAGE_DELTA_FIELD_PLAYER_INDEX));
		ENGINE_PTR(field_properties_definition, message_delta_field_definition_index, DUO_PTR(K_MESSAGE_DELTA_FIELD_DEFINITION_INDEX));
		ENGINE_PTR(field_properties_definition, message_delta_field_point2d, DUO_PTR(K_MESSAGE_DELTA_FIELD_POINT2D));
		ENGINE_PTR(field_properties_definition, message_delta_field_point3d, DUO_PTR(K_MESSAGE_DELTA_FIELD_POINT3D));
		ENGINE_PTR(field_properties_definition, message_delta_field_vector2d, DUO_PTR(K_MESSAGE_DELTA_FIELD_VECTOR2D));
		ENGINE_PTR(field_properties_definition, message_delta_field_vector3d, DUO_PTR(K_MESSAGE_DELTA_FIELD_VECTOR3D));
		ENGINE_PTR(field_properties_definition, message_delta_field_control_flags, DUO_PTR(K_MESSAGE_DELTA_FIELD_CONTROL_FLAGS));
		ENGINE_PTR(field_properties_definition, message_delta_field_angular_velocity, DUO_PTR(K_MESSAGE_DELTA_FIELD_ANGULAR_VELOCITY));
		ENGINE_PTR(field_properties_definition, message_delta_field_translational_velocity, DUO_PTR(K_MESSAGE_DELTA_FIELD_TRANSLATIONAL_VELOCITY));
		ENGINE_PTR(field_properties_definition, message_delta_field_map_name, DUO_PTR(K_MESSAGE_DELTA_FIELD_MAP_NAME));
		ENGINE_PTR(field_properties_definition, message_delta_field_network_map, DUO_PTR(K_MESSAGE_DELTA_FIELD_NETWORK_MAP));
		ENGINE_PTR(field_properties_definition, message_delta_field_network_player_name, DUO_PTR(K_MESSAGE_DELTA_FIELD_NETWORK_PLAYER_NAME));
		ENGINE_PTR(field_properties_definition, message_delta_field_network_player, DUO_PTR(K_MESSAGE_DELTA_FIELD_NETWORK_PLAYER));
		ENGINE_PTR(field_properties_definition, message_delta_field_network_game_name, DUO_PTR(K_MESSAGE_DELTA_FIELD_NETWORK_GAME_NAME));
		ENGINE_PTR(field_properties_definition, message_delta_field_network_game_players, DUO_PTR(K_MESSAGE_DELTA_FIELD_NETWORK_GAME_PLAYERS));
		ENGINE_PTR(field_properties_definition, message_delta_field_game_variant, DUO_PTR(K_MESSAGE_DELTA_FIELD_GAME_VARIANT));
		ENGINE_PTR(field_properties_definition, message_delta_field_game_variant_human_readable_description, DUO_PTR(K_MESSAGE_DELTA_FIELD_GAME_VARIANT_HUMAN_READABLE_DESCRIPTION));
		ENGINE_PTR(field_properties_definition, message_delta_field_universal_variant, DUO_PTR(K_MESSAGE_DELTA_FIELD_UNIVERSAL_VARIANT));
		ENGINE_PTR(field_properties_definition, message_delta_field_universal_variant_flags, DUO_PTR(K_MESSAGE_DELTA_FIELD_UNIVERSAL_VARIANT_FLAGS));
		ENGINE_PTR(field_properties_definition, message_delta_field_game_engine_variant, DUO_PTR(K_MESSAGE_DELTA_FIELD_GAME_ENGINE_VARIANT));
		ENGINE_PTR(field_properties_definition, message_delta_field_game_variant_flags, DUO_PTR(K_MESSAGE_DELTA_FIELD_GAME_VARIANT_FLAGS));
		ENGINE_PTR(field_properties_definition, message_delta_field_parameters_protocol_array, DUO_PTR(K_MESSAGE_DELTA_FIELD_PARAMETERS_PROTOCOL_ARRAY));
		ENGINE_PTR(field_properties_definition, message_delta_field_hud_chat_message, DUO_PTR(K_MESSAGE_DELTA_FIELD_HUD_CHAT_MESSAGE));
		ENGINE_PTR(field_properties_definition, message_delta_field_hud_chat_message_ptr, DUO_PTR(K_MESSAGE_DELTA_FIELD_HUD_CHAT_MESSAGE_PTR));
		ENGINE_PTR(field_properties_definition, message_delta_field_object_change_colors, DUO_PTR(K_MESSAGE_DELTA_FIELD_OBJECT_CHANGE_COLORS));
		ENGINE_PTR(field_properties_definition, message_delta_field_ctf_score_array, DUO_PTR(K_MESSAGE_DELTA_FIELD_CTF_SCORE_ARRAY));
		ENGINE_PTR(field_properties_definition, message_delta_field_race_score_array, DUO_PTR(K_MESSAGE_DELTA_FIELD_RACE_SCORE_ARRAY));
		ENGINE_PTR(field_properties_definition, message_delta_field_king_score_array, DUO_PTR(K_MESSAGE_DELTA_FIELD_KING_SCORE_ARRAY));
		ENGINE_PTR(field_properties_definition, message_delta_field_oddball_score_array, DUO_PTR(K_MESSAGE_DELTA_FIELD_ODDBALL_SCORE_ARRAY));
		ENGINE_PTR(field_properties_definition, message_delta_field_oddball_owner_array, DUO_PTR(K_MESSAGE_DELTA_FIELD_ODDBALL_OWNER_ARRAY));
		ENGINE_PTR(field_properties_definition, message_delta_field_slayer_score_array, DUO_PTR(K_MESSAGE_DELTA_FIELD_SLAYER_SCORE_ARRAY));
		ENGINE_PTR(field_properties_definition, message_delta_field_damage_data_flags, DUO_PTR(K_MESSAGE_DELTA_FIELD_DAMAGE_DATA_FLAGS));
		ENGINE_PTR(field_properties_definition, message_delta_field_motd_text, DUO_PTR(K_MESSAGE_DELTA_FIELD_MOTD_TEXT));
		ENGINE_PTR(field_properties_definition, message_delta_field_rcon_response, DUO_PTR(K_MESSAGE_DELTA_FIELD_RCON_RESPONSE));
		ENGINE_PTR(field_properties_definition, message_delta_field_rcon_password, DUO_PTR(K_MESSAGE_DELTA_FIELD_RCON_PASSWORD));
		ENGINE_PTR(field_properties_definition, message_delta_field_rcon_command, DUO_PTR(K_MESSAGE_DELTA_FIELD_RCON_COMMAND));


		ENGINE_PTR(field_reference_set, message_delta_empty_field_set, DUO_PTR(K_MESSAGE_DELTA_EMPTY_FIELD_SET));

		//ENGINE_PTR(field_reference_set, message_delta_guaranteed_object_header_field_set,					0x0, 0x0);
		ENGINE_PTR(field_reference_set, message_delta_game_object_header_field_set, DUO_PTR(K_MESSAGE_DELTA_GAME_OBJECT_HEADER_FIELD_SET));
		ENGINE_PTR(field_reference_set, message_delta_game_object_timestamped_header_field_set, DUO_PTR(K_MESSAGE_DELTA_GAME_OBJECT_TIMESTAMPED_HEADER_FIELD_SET));
		ENGINE_PTR(field_reference_set, message_delta_biped_update_header_field_set, DUO_PTR(K_MESSAGE_DELTA_BIPED_UPDATE_HEADER_FIELD_SET));
		ENGINE_PTR(field_reference_set, message_delta_ctf_update_header_field_set, DUO_PTR(K_MESSAGE_DELTA_CTF_UPDATE_HEADER_FIELD_SET));
		ENGINE_PTR(field_reference_set, message_delta_client_game_update_header_field_set, DUO_PTR(K_MESSAGE_DELTA_CLIENT_GAME_UPDATE_HEADER_FIELD_SET));
		ENGINE_PTR(field_reference_set, message_delta_player_score_update_header_field_set, DUO_PTR(K_MESSAGE_DELTA_PLAYER_SCORE_UPDATE_HEADER_FIELD_SET));
		ENGINE_PTR(field_reference_set, message_delta_remote_player_action_update_header_field_set, DUO_PTR(K_MESSAGE_DELTA_REMOTE_PLAYER_ACTION_UPDATE_HEADER_FIELD_SET));
		ENGINE_PTR(field_reference_set, message_delta_remote_player_position_update_header_field_set, DUO_PTR(K_MESSAGE_DELTA_REMOTE_PLAYER_POSITION_UPDATE_HEADER_FIELD_SET));
		ENGINE_PTR(field_reference_set, message_delta_remote_player_total_update_header_field_set, DUO_PTR(K_MESSAGE_DELTA_REMOTE_PLAYER_TOTAL_UPDATE_HEADER_FIELD_SET));
		//ENGINE_PTR(field_reference_set, message_delta__header_field_set, 0, PTR_UNKNOWN);
	}
}
