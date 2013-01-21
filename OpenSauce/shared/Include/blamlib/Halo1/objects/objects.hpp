/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <blamlib/Halo1/memory/data.hpp>
#include <blamlib/Halo1/memory/memory_pool.hpp>
#include <blamlib/Halo1/networking/message_delta_definitions.hpp>
#include <blamlib/Halo1/networking/network_client_manager.hpp>
#include <blamlib/Halo1/scenario/scenario_definitions.hpp>

namespace Yelo
{
	namespace Enums
	{
		enum {
			// Stock game state allocation size for the object memory pool
			// When running in editor tools, this and the max number of objects is increased by 5x
			k_object_memory_pool_allocation_size = 0x200000,
		};
	};

	namespace Flags
	{
		enum object_header_flags : byte_flags
		{
			_object_header_active_bit = 0,
			_object_header_requires_motion_bit = 1,
			// 2
			_object_header_being_deleted_bit = 3,
			// 4
			_object_header_connected_to_map_bit = 5,
			_object_header_child_bit = 6,
			// 7
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


		struct s_object_header_datum : Memory::s_datum_base
		{
			Flags::object_header_flags flags;
			byte_enum object_type;
			int16 cluster_index;
			int16 data_size;
			union {
				void* address;

				struct s_object_data* _object; // Note: since 's_object_data' is the base of all object datums, we can do this

				// Allows us to implicitly treat 'address' as an specific object type ptr
				// in situations where we're programming for something more specific (eg, unit types only)
				union {
					struct s_scenery_datum* _scenery;
					struct s_sound_scenery_datum* _sound_scenery;
					struct s_garbage_datum* _garbage;
					struct s_projectile_datum* _projectile;
					struct s_placeholder_datum* _placeholder;

					struct s_device_datum* _device;
					struct s_device_machine_datum* _machine;
					struct s_device_control_datum* _control;
					struct s_device_lightfixture_datum* _lightfixture;

					struct s_item_datum* _item;
					struct s_weapon_datum* _weapon;
					struct s_equipment_datum* _equipment;

					struct s_unit_datum* _unit;
					struct s_biped_datum* _biped;
					struct s_vehicle_datum* _vehicle;
				};
			};
		}; BOOST_STATIC_ASSERT( sizeof(s_object_header_datum) == 0xC );


		struct s_objects_pool_data
		{
			Memory::s_memory_pool header;

			byte data[Enums::k_object_memory_pool_allocation_size];
		};

		struct s_object_globals_data
		{
			bool object_is_being_placed;
			bool object_marker_initialized;
			bool garbage_collect_now;
			UNKNOWN_TYPE(bool);
			int16 object_type_being_placed; // script value enum
			UNKNOWN_TYPE(int16);
			UNKNOWN_TYPE(datum_index);
			long_flags pvs[BIT_VECTOR_SIZE_IN_DWORDS(512)];
			long_flags pvs2[BIT_VECTOR_SIZE_IN_DWORDS(512)];
			UNKNOWN_TYPE(int32);								// 0x84
			_enum object_pvs_set;								// 0x90
			UNKNOWN_TYPE(int16);
			// none = 0
			// object = 1
			// camera = 2
			_enum object_pvs_object;							// 0x94
			PAD16;
		}; BOOST_STATIC_ASSERT( sizeof(s_object_globals_data) == 0x98 );

		struct s_object_name_list_data
		{
			datum_index object_name_to_datum_table[Enums::k_maximum_object_names_per_scenario];
		};


		struct s_object_iterator
		{
			enum { k_signature = 'ееее' };

			long_flags type_mask;						// object types to iterate
			Flags::object_header_flags ignore_flags;	// When any of these bits are set, the object is skipped
			PAD8;
			int16 absolute_index;
			datum_index object_index;
			tag signature;

			API_INLINE static s_object_iterator& New(s_object_iterator& iter, long_flags type_mask, Flags::object_header_flags ignore_flags)
			{
				iter.signature = k_signature;
				iter.type_mask = type_mask;
				iter.ignore_flags = ignore_flags;
				iter.absolute_index = 0;
				iter.object_index = datum_index::null;
			}
		};
	};
};