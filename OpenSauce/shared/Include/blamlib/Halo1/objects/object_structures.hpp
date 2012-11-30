/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <blamlib/Halo1/objects/object_definitions.hpp>

namespace Yelo
{
	namespace Enums
	{
		enum networked_datum : long_enum
		{
			_networked_datum_master,
			_networked_datum_puppet,
			_networked_datum_puppet_controlled_by_local_player,
			_networked_datum_3,
		};

		enum object_sizes
		{
			// The base object datum allocates 2 real_orientation3d and 1 real_matrix4x3 FOR EVERY model node
			k_object_size_object =			0x1F4,
			k_object_size_projectile =		0xBC + k_object_size_object,
			k_object_size_scenery =			0x4  + k_object_size_object,
			k_object_size_placeholder =		0x8  + k_object_size_object,
			k_object_size_sound_scenery =	0x4  + k_object_size_object,

			k_object_size_device =			0x20 + k_object_size_object,
			k_object_size_machine =			0x14 + k_object_size_device,
			k_object_size_control =			0x8  + k_object_size_device,
			k_object_size_light_fixture =	0x18 + k_object_size_device,

			k_object_size_item =			0x38 + k_object_size_object,
			k_object_size_weapon =			0x114+ k_object_size_item,
			k_object_size_equipment =		0x68 + k_object_size_item,
			k_object_size_garbage =			0x18 + k_object_size_item,

			k_object_size_unit =			0x2D8+ k_object_size_object,
			k_object_size_biped =			0x84 + k_object_size_unit,
			k_object_size_vehicle =			0xF4 + k_object_size_unit,
		};

		enum attachment_type : sbyte {
			// TODO: remove this pragma warning off in any later migrations newer VS versions
			// http://connect.microsoft.com/VisualStudio/feedback/details/674442/enumeration-with-negative-values-are-popping-c4341-warnings
#pragma warning( push )
#pragma warning( disable : 4341 ) // signed value is out of range for enum constant
			_attachment_type_invalid = NONE,
#pragma warning( pop )

			_attachment_type_light = 0,
			_attachment_type_looping_sound,
			_attachment_type_effect,
			_attachment_type_contrail,
			_attachment_type_particle,
			_attachment_type,
		};
	};

	namespace Flags
	{
		enum object_bits
		{
			// --- 0x10

			_object_unk1_bit = 0,
			_object_unk2_bit,
			_object_unk3_bit,
			_object_is_in_water_bit,
			_object_unk5_bit,
			_object_update_physics_bit,
			_object_unk7_bit,
			_object_unk8_bit,
			_object_unk9_bit,
			_object_unk10_bit,
			_object_unk11_bit,
			_object_connected_to_map_bit,
			_object_unk13_bit,
			_object_unk14_bit,
			_object_unk15_bit,
			_object_unk16_bit,
			_object_unk17_bit,
			_object_garbage_bit,
			_object_unk19_bit,
			_object_unk20_bit,
			_object_unk21_bit,
			_object_unk22_bit,
			_object_beautify_bit,
			_object_unk24_bit,
			_object_collideable_bit,
			_object_unk26_bit,
			_object_unk27_bit,
			_object_unk28_bit,
			_object_unk29_bit,
			_object_unk30_bit,
			_object_unk31_bit,
			_object_unk32_bit,

			_object_yelo_is_being_infected_bit = _object_unk32_bit, // apparently this bit isn't used

			_object_is_in_water_flag = FLAG(_object_is_in_water_bit),
			_object_update_physics_flag = FLAG(_object_update_physics_bit),
			_object_connected_to_map_flag = FLAG(_object_connected_to_map_bit),
			_object_garbage_flag = FLAG(_object_garbage_bit),
			_object_beautify_flag = FLAG(_object_beautify_bit),
			_object_collideable_flag = FLAG(_object_collideable_bit),

			// --- 0x106

			_object_damage_unk0_bit = 0,
			_object_damage_unk1_bit,
			_object_should_be_tracked_bit,
			_object_damage_unk3_bit,
			_object_damage_unk4_bit,
			_object_killed_bit,
			_object_killed_silent_bit,
			_object_damage_unk7_bit,
			_object_damage_unk8_bit,
			_object_damage_unk9_bit,
			_object_damage_unk10_bit,
			_object_cannot_take_damage_bit,
			_object_damage_unk12_bit,
			_object_killed_no_statistics_bit,

			_object_should_be_tracked_flag = FLAG(_object_should_be_tracked_bit),
			_object_killed_flag = FLAG(_object_killed_bit),
			_object_killed_silent_flag = FLAG(_object_killed_silent_bit),
			_object_cannot_take_damage_flag = FLAG(_object_cannot_take_damage_bit),
			_object_killed_no_statistics_flag = FLAG(_object_killed_no_statistics_bit),
		};
	};

	namespace Objects
	{
		// Object datums in the memory pool can have dynamically allocated nested-blocks.
		// Due to the way the game implements this, OS can allocate a nested-block of its own in an object,
		// and when the stock game uses a gamesave with an object like this, it will silently but safely
		// operate on it, even when it comes time to delete the object from the pool.
		// An object gets reallocated when a nested-block is requested (and the additional size is added to
		// the object's overall pool block size)
		struct s_object_header_block_reference
		{
			uint16 size;
			uint16 offset;
		}; BOOST_STATIC_ASSERT( sizeof(s_object_header_block_reference) == 0x4 );

		struct s_object_placement_data
		{
			datum_index definition_index;
			long_flags flags;
			datum_index player_index;
			datum_index owner_object_index;
			UNKNOWN_TYPE(int32);
			int16 owner_team_index;
			int16 region_permutation; // variant id
			real_point3d position;
			UNKNOWN_TYPE(real); // angle?
			real_vector3d transitional_velocity;
			real_vector3d forward;
			real_vector3d up;
			real_vector3d angular_velocity;
			real_rgb_color change_colors[4];
		}; BOOST_STATIC_ASSERT( sizeof(s_object_placement_data) == 0x88 );

		struct s_object_network_datum_data
		{
			datum_index owner_player_index;			// 0x50
			datum_index owner;						// 0x54
			int32 timestamp;						// 0x58
			real_point3d position;					// 0x5C
			real_vector3d transitional_velocity;	// 0x68
			real_vector3d forward;					// 0x74
			real_vector3d up;						// 0x80
			real_vector3d angular_velocity;			// 0x8C

			API_INLINE void CopyToPlacementData(s_object_placement_data& data) const
			{
				data.player_index = owner_player_index;
				data.owner_object_index = owner;
				memcpy(&data.position,				&position, sizeof(position));
				memcpy(&data.transitional_velocity,	&transitional_velocity, sizeof(transitional_velocity));
				memcpy(&data.forward,				&forward, sizeof(forward));
				memcpy(&data.up,					&up, sizeof(up));
				memcpy(&data.angular_velocity,		&angular_velocity, sizeof(angular_velocity));
			}
		}; BOOST_STATIC_ASSERT( sizeof(s_object_network_datum_data) == 0x48 );

		struct s_object_animation_datum_data
		{
			datum_index definition_index;	// 0xCC
			int16 current_index;			// 0xD0
			int16 current_frame_index;		// 0xD2
			UNKNOWN_TYPE(int16);			// 0xD4
			UNKNOWN_TYPE(int16);			// 0xD6
		}; BOOST_STATIC_ASSERT( sizeof(s_object_animation_datum_data) == 0xC );

		struct s_object_damage_datum_data
		{
			real maximum_vitality;				// 0xD8
			real current_vitality;				// 0xDC
			real health;						// 0xE0, health = body
			real shield;						// 0xE4
			real shield_damage_current;			// 0xE8
			real body_damage_current;			// 0xEC
			UNKNOWN_TYPE(datum_index);			// 0xF0, object_index
			real shield_damage_recent;			// 0xF4
			real body_damage_recent;			// 0xF8
			int32 shield_damage_update_tick;	// 0xFC, these update ticks are set to NONE when not active
			int32 body_damage_update_tick;		// 0x100
			int16 stun_ticks;					// 0x104, based on ftol(s_shield_damage_resistance->stun_time * 30f)
			word_flags flags;					// 0x106
		}; BOOST_STATIC_ASSERT( sizeof(s_object_damage_datum_data) == 0x30 );

		struct s_object_attachments_datum_data
		{
			Enums::attachment_type attached_types[Enums::k_maximum_number_of_attachments_per_object];	// 0x144
			// game state datum_index
			// ie, if Attachments[x]'s definition (object_attachment_block[x]) says it is a 'cont'
			// then the datum_index is a contrail_data handle
			datum_index attachment_indices[Enums::k_maximum_number_of_attachments_per_object];			// 0x14C
			datum_index first_widget_index;																// 0x16C
		}; BOOST_STATIC_ASSERT( sizeof(s_object_attachments_datum_data) == 0x2C );

		struct s_object_data
		{
			datum_index definition_index;													// 0x0
			//////////////////////////////////////////////////////////////////////////
			// Added in HaloPC
			Enums::networked_datum datum_role;												// 0x4
			UNKNOWN_TYPE(bool);																// 0x8
			bool should_force_baseline_update;												// 0x9
			PAD16; // haven't verified if this is padding or not
			int32 network_time;																// 0xC
			//////////////////////////////////////////////////////////////////////////
			long_flags flags;																// 0x10
			int32 object_marker_id;															// 0x14
			//////////////////////////////////////////////////////////////////////////
			// Added in HaloPC
			UNKNOWN_TYPE(bool);																// 0x18
			PAD24;
			byte unknown1C[0x34];															// 0x1C
			//////////////////////////////////////////////////////////////////////////
			s_object_network_datum_data network;											// 0x50
			s_scenario_location scenario_location;											// 0x98
			real_point3d center;															// 0xA0
			real radius;																	// 0xBC
			real scale;																		// 0xB0
			_enum type;																		// 0xB4
			PAD16;
			int16 owner_team_index;															// 0xB8
			int16 name_list_index;															// 0xBA
			UNKNOWN_TYPE(int16);															// 0xBC
			int16 region_permutation;														// 0xBE, variant id
			datum_index player_index;														// 0xC0
			// If this were a projectile, I believe this would be the handle to
			// the weapon which spawned it
			datum_index owner_object_index;													// 0xC4
			UNKNOWN_TYPE(int32); // haven't verified if this is padding or not yet
			s_object_animation_datum_data animation;										// 0xCC
			s_object_damage_datum_data damage;												// 0xD8
			PAD32; // 0x108 haven't verified if this is padding or not yet
			datum_index cluster_partition_index;											// 0x10C
			UNKNOWN_TYPE(datum_index);														// 0x110, object_index, garbage collection related

			datum_index next_object_index;													// 0x114
			datum_index first_object_index;													// 0x118
			datum_index parent_object_index;												// 0x11C
			sbyte parent_node_index;														// 0x120
			UNKNOWN_TYPE(byte);																// 0x121 idk if this is an sbyte or bool here
			bool force_shield_update;														// 0x122
			byte_flags valid_outgoing_functions;											// 0x123, 1<<function_index
			real incoming_function_values[Enums::k_number_of_incoming_object_functions];	// 0x124
			real outgoing_function_values[Enums::k_number_of_outgoing_object_functions];	// 0x134

			s_object_attachments_datum_data attachments;									// 0x144
			UNKNOWN_TYPE(int32);	// 0x170, idk what is here, could be part of s_object_attachments_datum_data
			UNKNOWN_TYPE(int16);	// 0x174, idk what is here, could be part of s_object_attachments_datum_data
			int16 shader_permutation;														// 0x176, shader's bitmap block index
			byte region_vitality[Enums::k_maximum_regions_per_model];						// 0x178
			sbyte region_permutation_indices[Enums::k_maximum_regions_per_model];			// 0x180

			real_rgb_color change_colors[Enums::k_number_of_object_change_colors];			// 0x188
			real_rgb_color change_colors2[Enums::k_number_of_object_change_colors];			// 0x1B8

			// one of these are for interpolating
			s_object_header_block_reference node_orientations;								// 0x1E8 real_orientation3d[node_count]
			s_object_header_block_reference node_orientations2;								// 0x1EC real_orientation3d[node_count]
			s_object_header_block_reference node_matrix_block;								// 0x1F0 real_matrix4x3[node_count]

			API_INLINE bool VerifyType(long_flags type_mask)
			{
				return TEST_FLAG( type_mask, type );
			}

			template<typename TBlockData>
			API_INLINE TBlockData* GetBlock(s_object_header_block_reference ref)
			{
				byte* obj = CAST_PTR(byte*, this);

				return ref->offset == 0 ? NULL : CAST_PTR(TBlockData*, &obj[ref->offset]);
			}
			template<typename TBlockData, size_t block_reference_offset>
			API_INLINE TBlockData* GetBlock()
			{
				byte* obj = CAST_PTR(byte*, this);

				s_object_header_block_reference* ref = CAST_PTR(s_object_header_block_reference*, obj+block_reference_offset);

				return GetBlock<TBlockData>(*ref);
			}
		}; BOOST_STATIC_ASSERT( sizeof(s_object_data) == Enums::k_object_size_object );


		//////////////////////////////////////////////////////////////////////////
		// scenery
		struct s_scenery_data
		{
			long_flags flags; // FLAG(0) - playing animation
		}; BOOST_STATIC_ASSERT( sizeof(s_scenery_data) == (Enums::k_object_size_scenery - Enums::k_object_size_object) );

		struct s_scenery_datum
		{
			s_object_data object;
			s_scenery_data scenery;
		}; BOOST_STATIC_ASSERT( sizeof(s_scenery_datum) == Enums::k_object_size_scenery );


		//////////////////////////////////////////////////////////////////////////
		// placeholder
		struct s_placeholder_data
		{
			PAD64;
		}; BOOST_STATIC_ASSERT( sizeof(s_placeholder_data) == (Enums::k_object_size_placeholder - Enums::k_object_size_object) );

		struct s_placeholder_datum
		{
			s_object_data object;
			s_placeholder_data placeholder;
		}; BOOST_STATIC_ASSERT( sizeof(s_placeholder_datum) == Enums::k_object_size_placeholder );


		//////////////////////////////////////////////////////////////////////////
		// sound_scenery
		struct s_sound_scenery_data
		{
			PAD32;
		}; BOOST_STATIC_ASSERT( sizeof(s_sound_scenery_data) == (Enums::k_object_size_sound_scenery - Enums::k_object_size_object) );

		struct s_sound_scenery_datum
		{
			s_object_data object;
			s_sound_scenery_data sound_scenery;
		}; BOOST_STATIC_ASSERT( sizeof(s_sound_scenery_datum) == Enums::k_object_size_sound_scenery );
	};
};