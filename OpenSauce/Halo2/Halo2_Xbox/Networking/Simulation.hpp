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
		enum simulation_world_type
		{
			_simulation_world_type_none,
			_simulation_world_type_local,
			_simulation_world_type_synchronous_client,
			_simulation_world_type_synchronous_server,
			_simulation_world_type_film_playback,
			_simulation_world_type_distributed_client,
			_simulation_world_type_distributed_server,
			k_simulation_world_state_count
		};

		enum simulation_object
		{
			_simulation_entity_slayer_engine_globals = 0,
			_simulation_entity_ctf_engine_globals,
			_simulation_entity_oddball_engine_globals,
			_simulation_entity_king_engine_globals,
			_simulation_entity_territories_engine_globals,
			_simulation_entity_juggernaut_engine_globals,
			_simulation_entity_game_engine_player,
			_simulation_entity_game_engine_statborg,
			_simulation_entity_breakable_surface_group,
			_simulation_entity_unit,
			_simulation_entity_item,
			_simulation_entity_generic,
			_simulation_entity_vehicle,
			_simulation_entity_projectile,
			_simulation_entity_weapon,
			_simulation_entity_turret,
			_simulation_entity_device,
			_simulation_entity,

			_simulation_event_damage_aftermath = 0,
			_simulation_event_damage_section_response,
			_simulation_event_breakable_surface_damage,
			_simulation_event_projectile_attach,
			_simulation_event_projectile_effect,
			_simulation_event_projectile_impact_effect,
			_simulation_event_projectile_object_impact_effect,
			_simulation_event_game_engine_event,
			_simulation_event_unit_board_vehicle,
			_simulation_event_unit_pickup,
			_simulation_event_weapon_effect,
			_simulation_event_unit_exit_vehicle,
			_simulation_event_weapon_fire,
			_simulation_event_weapon_reload,
			_simulation_event_unit_grenade_initiate,
			_simulation_event_unit_melee_initiate,
			_simulation_event_weapon_pickup,
			_simulation_event_weapon_put_away,
			_simulation_event_weapon_drop,
			_simulation_event_vehicle_flip,
			_simulation_event_vehicle_trick,
			_simulation_event_device_touch,
			_simulation_event_unit_grenade_release,
			_simulation_event_unit_melee_damage,
			_simulation_event_unit_enter_vehicle,
			_simulation_event_game_engine_request_boot_player_event,
			_simulation_event,
		};
	};

	namespace Simulation
	{
		class c_simulation_distributed_view : TStructImpl(PLATFORM_VALUE(38548, 38548, 1)) // verify retail size
		{
		};

		struct s_network_message_view_establishment
		{
			long_enum establishment_mode; // k_simulation_view_establishment_mode_count = 6
			datum_index establishment_identifier;
		};

		struct s_network_message_synchronous_gamestate
		{
			uint32 gamestate_offset;
			uint32 gamestate_size;
		};

		struct s_network_message_synchronous_join
		{
			uint32 next_update_number;
		};

		struct s_network_message_synchronous_actions : TStructImpl(PLATFORM_VALUE(336, 336, 1)) // verify retail size
		{
		};




		struct entity_definition
		{
			void* entity_type; // 0x0
			void* entity_type_name; // 0x4
			void* state_data_size; // 0x8
			void* creation_data_size; // 0xC
			void* update_flag_count; // 0x10
			void* initial_update_mask; // 0x14
			void* UNKNOWN(18); // 0x18
			void* UNKNOWN(1C); // 0x1C
			void* UNKNOWN(20); // 0x20
			void* UNKNOWN(_0); // 0x24
			void* get_creation_relevance_string; // 0x28
			void* UNKNOWN(_1); // 0x2C // mark_for_deletion or delete?
			void* encode_unk; // 0x30
			void* read_creation_from_packet; // 0x34
			void* write_creation_to_packet; // 0x38
			void* read_update_from_packet; // 0x3C
			void* UNKNOWN(_2); // 0x40
			void* UNKNOWN(_3); // 0x44
			void* UNKNOWN(_4); // 0x48
			void* clear; // 0x4C
			void* process_pending_updates; // 0x50
			void* UNKNOWN(_6); // 0x54
			void* UNKNOWN(_7); // 0x58
			void* UNKNOWN(_8); // 0x5C
			void* UNKNOWN(_9); // 0x60
			void* UNKNOWN(64); // 0x64
			void* update; // 0x68
			void* UNKNOWN(_10); // 0x6C
			void* UNKNOWN(_11); // 0x70
			void* create; // 0x74
			void* UNKNOWN(78); // 0x78
			void* UNKNOWN(_13); // 0x7C
			void* UNKNOWN(_14); // 0x80
			void* UNKNOWN(84); // 0x84
			void* UNKNOWN(_15); // 0x88
			void* UNKNOWN(_16); // 0x8C
		};

		struct event_definition
		{
			void* event_type; // 0x0
			void* event_type_name; // 0x4
			void* payload_size; // 0x8
			void* number_of_entity_references; // 0xC
			void* reference_delays_entity_deletion; // 0x10
			void* UNKNOWN(14); // 0x14
			void* minimum_required_bits; // 0x18
			void* get_relevance; // 0x1C
			void* get_relevance_string; // 0x20
			void* encode; // 0x24
			void* decode; // 0x28
			void* perform; // 0x2C
		};

		struct statistic_definition
		{
			cstring name; // 0x0
			PAD32; // 0x4
			uint16 minimum_value; // 0x8
			uint16 maximum_value; // 0xA
			uint16 encoding_bits; // 0xC
		};
	};

	namespace GameState
	{
		struct s_game_engine_event : TStructImpl(36)
		{
			//TStructGetPtrImpl(int32, , 0x0); // 0, 16
			//TStructGetPtrImpl(int32, EventType?, 0x4); // 0, 64
			TStructGetPtrImpl(datum_index, AudiencePlayerIndex, 0x8);
			TStructGetPtrImpl(datum_index, CausePlayerIndex, 0xC);
			//TStructGetPtrImpl(int32, , 0x10); // 0, 16 gets checked for NONE
			TStructGetPtrImpl(datum_index, EffectPlayerIndex, 0x14);
			//TStructGetPtrImpl(int32, , 0x18); // 0, 16 gets checked for NONE
			//TStructGetPtrImpl(int32, , 0x1C); // gets encoded as a boolean
			// when 0x0 is 7: 0, 127
			// when 0x0 is 9: 0, 7
			//TStructGetPtrImpl(int16, , 0x20);
			//TStructGetPtrImpl(int16, , 0x22); // 0, 127
		};

		struct s_slayer_engine_state_data : TStructImpl(36)
		{
		};

		struct s_simulation_device_state_data : TStructImpl(112)
		{
		};

		struct s_simulation_device_creation_data : TStructImpl(16)
		{
		};
	};
};