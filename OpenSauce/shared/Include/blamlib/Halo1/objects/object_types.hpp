/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

namespace Yelo
{
	namespace MessageDeltas
	{
		struct message_dependant_header;
	};
	namespace Networking
	{
		struct s_network_game_client;
	};

	namespace Enums
	{
		enum object_type
		{
			_object_type_object = -4,
			_object_type_device = -3,
			_object_type_item = -2,
			_object_type_unit = -1,

			_object_type_biped = 0,
			_object_type_vehicle,
			_object_type_weapon,
			_object_type_equipment,
			_object_type_garbage,
			_object_type_projectile,
			_object_type_scenery,
			_object_type_machine,
			_object_type_control,
			_object_type_light_fixture,
			_object_type_placeholder,
			_object_type_sound_scenery,

			_object_type_mask_device = 
				FLAG(_object_type_machine) |
				FLAG(_object_type_control) |
				FLAG(_object_type_light_fixture),
			_object_type_mask_item = 
				FLAG(_object_type_weapon) |
				FLAG(_object_type_equipment) |
				FLAG(_object_type_garbage),
			_object_type_mask_unit = 
				FLAG(_object_type_biped) |
				FLAG(_object_type_vehicle),

			_object_type_mask_all =
				FLAG(_object_type_projectile) |
				FLAG(_object_type_scenery) |
				FLAG(_object_type_placeholder) |
				FLAG(_object_type_sound_scenery) |
				_object_type_mask_device |
				_object_type_mask_item | 
				_object_type_mask_unit,
		};
	};

	namespace Objects
	{
		typedef void (PLATFORM_API* proc_object_type)(datum_index object_index);
		typedef bool (PLATFORM_API* proc_object_type_new)(datum_index object_index);
		typedef bool (PLATFORM_API* proc_object_type_update)(datum_index object_index);
		typedef void (PLATFORM_API* proc_object_type_handle_deleted_object)(datum_index object_index, datum_index deleted_object_index);
		typedef void (PLATFORM_API* proc_object_type_handle_region_destroyed)(datum_index object_index, int32 region_index, long_flags damage_region_flags);
		typedef bool (PLATFORM_API* proc_object_type_handle_parent_destroyed)(datum_index object_index);
		typedef void (PLATFORM_API* proc_object_type_preprocess_node_orientations)(datum_index object_index, real_orientation3d* orientations);
		typedef void (PLATFORM_API* proc_object_type_postprocess_node_matrices)(datum_index object_index, real_matrix4x3* matrices);
		typedef void (PLATFORM_API* proc_object_type_notify_impulse_sound)(datum_index object_index, datum_index sound_definition_index, datum_index sound_index);
		typedef bool (PLATFORM_API* proc_object_type_render_message_debug)(datum_index object_index);
		typedef int32 (PLATFORM_API* proc_object_type_create_to_network)(datum_index object_index, void* buffer, int32 buffer_size_in_bits);
		typedef int32 (PLATFORM_API* proc_object_type_build_update_delta)(datum_index object_index, void* buffer, int32 buffer_size_in_bits, _enum message_delta_mode);
		typedef void (PLATFORM_API* proc_object_type_process_update_delta)(datum_index object_index, MessageDeltas::message_dependant_header* header, Networking::s_network_game_client* client);
		typedef bool (PLATFORM_API* proc_object_type_is_network_time_valid)(datum_index object_index);
		typedef bool (PLATFORM_API* proc_object_type_unknown78)(datum_index object_index, void*); // IDK what the 2nd parameter's type is
		struct s_object_type_definition
		{
			cstring name;													// 0x0
			tag group_tag;													// 0x4
			int16 datum_size;												// 0x8
			int16 placement_tag_block_offset;								// 0xA
			int16 palette_tag_block_offset;									// 0xC
			int16 placement_tag_block_size;									// 0xE
			int32 update_message_type;										// 0x10
			proc_initialize				initialize;										// 0x14
			proc_dispose				dispose;										// 0x18
			proc_initialize_for_new_map initialize_for_new_map;							// 0x1C
			proc_dispose_from_old_map	dispose_from_old_map;							// 0x20
			proc_object_type			adjust_placement;								// 0x24
			proc_object_type_new 		new_;											// 0x28
			proc_object_type 			place;											// 0x2C
			proc_object_type 			delete_;										// 0x30
			proc_object_type_update 	update;											// 0x34
			proc_object_type			export_function_values;							// 0x38
			proc_object_type_handle_deleted_object handle_deleted_object;				// 0x3C
			proc_object_type_handle_region_destroyed handle_region_destroyed;			// 0x40
			proc_object_type_handle_parent_destroyed handle_parent_destroyed;			// 0x44
			proc_object_type_preprocess_node_orientations preprocess_node_orientations;	// 0x48
			proc_object_type_postprocess_node_matrices postprocess_node_matrices;		// 0x4C
			proc_object_type 			reset;											// 0x50
			proc_object_type 			disconnect_from_structure_bsp;					// 0x54
			proc_object_type_notify_impulse_sound notify_impulse_sound;					// 0x58
			proc_object_type 			render_debug;									// 0x5C

			proc_object_type_render_message_debug render_message_debug;					// 0x60
			proc_object_type_create_to_network create_to_network;						// 0x64
			proc_object_type update_baseline;											// 0x68
			proc_object_type_build_update_delta build_update_delta;						// 0x6C
			proc_object_type_process_update_delta process_update_delta;					// 0x70
			
			proc_object_type_is_network_time_valid is_network_time_valid;				// 0x74
			proc_object_type_unknown78 unknown78;										// 0x78, unused. Only networked object types implemented this (and they only returned true)
			proc_object_type update_network_time;										// 0x7C

			s_object_type_definition* object_type;
			s_object_type_definition* base_object_types[2];
			int32 _unused1[13]; // s_object_type_definition*.
			s_object_type_definition* next;
			PAD32;
		}; BOOST_STATIC_ASSERT( sizeof(s_object_type_definition) == 0xC8 );
	};
};