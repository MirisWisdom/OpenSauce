/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

namespace Yelo
{
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
				(1 << _object_type_machine) |
				(1 << _object_type_control) |
				(1 << _object_type_control),
			_object_type_mask_item = 
				(1 << _object_type_weapon) |
				(1 << _object_type_equipment) |
				(1 << _object_type_garbage),
			_object_type_mask_unit = 
				(1 << _object_type_biped) |
				(1 << _object_type_vehicle),
		};

		enum networked_datum : long_enum
		{
			_networked_datum_master,
			_networked_datum_puppet,
			_networked_datum_puppet_controlled_by_local_player,
			_networked_datum_3,
		};

		enum object_sizes
		{
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

		enum attachment_type {
			_attachment_type_invalid = NONE,

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
			// --- 0x2

			_object_activated_flag = FLAG(0),

			// --- 0x10

			_object_unk1_flag = FLAG(0),
			_object_unk2_flag = FLAG(1),
			_object_unk3_flag = FLAG(2),
			_object_unk4_flag = FLAG(3),
			_object_unk5_flag = FLAG(4),
			_object_update_physics_flag = FLAG(5),
			_object_unk7_flag = FLAG(6),
			_object_unk8_flag = FLAG(7),
			_object_unk9_flag = FLAG(8),
			_object_unk10_flag = FLAG(9),
			_object_unk12_flag = FLAG(10),
			_object_connected_to_map_flag = FLAG(11),
			_object_unk14_flag = FLAG(12),
			_object_unk15_flag = FLAG(13),
			_object_unk16_flag = FLAG(14),
			_object_unk17_flag = FLAG(15),
			_object_garbage_flag = FLAG(16),
			_object_unk18_flag = FLAG(17),
			_object_unk19_flag = FLAG(18),
			_object_unk20_flag = FLAG(19),
			_object_unk21_flag = FLAG(20),
			_object_unk22_flag = FLAG(21),
			_object_beautify_flag = FLAG(22),
			_object_unk24_flag = FLAG(23),
			_object_collideable_flag = FLAG(24),
			_object_unk26_flag = FLAG(25),
			_object_unk27_flag = FLAG(26),
			_object_unk28_flag = FLAG(27),
			_object_unk29_flag = FLAG(28),
			_object_unk30_flag = FLAG(29),
			_object_unk31_flag = FLAG(30),
			_object_unk32_flag = FLAG(31),

			_object_yelo_is_being_infected_bit = 31, // apparently bit 31 isn't used

			// --- 0x106 & 0x107

			_object_cannot_take_damage_bit = 11,

			// 1
			// 2
			_object_should_be_tracked_flag = FLAG(2),
			// 8
			// 16
			_object_killed_flag = FLAG(5),
			_object_killed_silent_flag = FLAG(6),
			// 128
			// 1
			// 2
			// 4
			_object_cannot_take_damage_flag = FLAG(11), // 8
			// 16
			_object_killed_no_statistics = FLAG(13), // 32
		};
	};

	namespace Objects
	{
		struct s_object_header_block_reference
		{
			uint16 offset;
			uint16 size;
		};

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
			datum_index owner_player_index;
			datum_index owner;
			uint32 timestamp;
			real_point3d position;
			real_vector3d transitional_velocity;
			real_vector3d forward;
			real_vector3d up;
			real_vector3d angular_velocity;

			void CopyToPlacementData(s_object_placement_data& data)
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

		struct s_object_data : TStructImpl( Enums::k_object_size_object )
		{
			TStructGetPtrImpl(datum_index,				TagDefinition, 0x0);
			TStructGetPtrImpl(Enums::networked_datum,	DatumnRole, 0x4);
			// 0x8 - boolean
			TStructGetPtrImpl(uint32,					ShouldForceBaselineUpdate, 0x9);
			// 0xA?
			TStructGetPtrImpl(uint32,					NetworkTime, 0xC);
			TStructGetPtrImpl(long_flags,				Flags, 0x10);
			TStructGetPtrImpl(s_object_network_datum_data, NetworkDatumData, 0x50);
			TStructGetPtrImpl(s_scenario_location,		ScenarioLocation, 0x98);
			TStructGetPtrImpl(real_point3d,				Center, 0xA0); // position, used for stock game code's trigger volume point testing
			TStructGetPtrImpl(real,						Radius, 0xAC);
			TStructGetPtrImpl(real,						Scale, 0xB0);
			TStructGetPtrImpl(byte,						Type, 0xB4);
			TStructGetPtrImpl(int16,					OwnerTeamIndex, 0xB8);
			TStructGetPtrImpl(int16,					NameListIndex, 0xBA);
			TStructGetPtrImpl(int16,					RegionPermutationVariantId, 0xBE);
			TStructGetPtrImpl(datum_index,				PlayerIndex, 0xC0);
			// If this were a projectile, I believe this would be the handle to
			// the weapon which spawned it
			TStructGetPtrImpl(datum_index,				OwnerObjectIndex, 0xC4);
			// 0xC8?
			TStructGetPtrImpl(datum_index,				AnimationDefinition, 0xCC);
			TStructGetPtrImpl(int16,					AnimationCurrentIndex, 0xD0);
			TStructGetPtrImpl(int16,					AnimationCurrentFrameIndex, 0xD2);
			// 0xD4?
			TStructGetPtrImpl(real,						MaximumVitality, 0xD8);
			TStructGetPtrImpl(real,						CurrentVitality, 0xDC);
			TStructGetPtrImpl(real,						Health, 0xE0); // health = body
			TStructGetPtrImpl(real,						Shield, 0xE4);
			TStructGetPtrImpl(real,						ShieldDamageCurrent, 0xE8);
			TStructGetPtrImpl(real,						BodyDamageCurrent, 0xEC);
			//TStructGetPtrImpl(datum_index,				, 0xF0); // object index
			TStructGetPtrImpl(real,						ShieldDamageRecent, 0xF4);
			TStructGetPtrImpl(real,						BodyDamageRecent, 0xF8);
			TStructGetPtrImpl(int32,					ShieldDamageUpdateTick, 0xFC); // these update ticks are set to NONE when not active
			TStructGetPtrImpl(int32,					BodyDamageUpdateTick, 0x100);
			TStructGetPtrImpl(uint16,					StunTicks, 0x104); // based on ftol(s_shield_damage_resistance->stun_time * 30f)
			TStructGetPtrImpl(word_flags,				DamageFlags, 0x106);
			// 0x108?
			TStructGetPtrImpl(datum_index,				ClusterPartitionIndex, 0x10C);
			TStructGetPtrImpl(datum_index,				NextObjectIndex, 0x114);
			TStructGetPtrImpl(datum_index,				FirstObjectIndex, 0x118);
			TStructGetPtrImpl(datum_index,				ParentObjectIndex, 0x11C);
			TStructGetPtrImpl(sbyte,					ParentNodeIndex, 0x120);
			// 0x121?
			TStructGetPtrImpl(bool,						ForceShieldUpdate, 0x122);
			TStructGetPtrImpl(byte_flags,				ValidOutgoingFunctions, 0x123); // 1<<function_index
			TStructGetPtrImpl(real,						IncomingFunctionValues, 0x124); // [4]
			TStructGetPtrImpl(real,						OutgoingFunctionValues, 0x134); // [4]

			TStructGetPtrImpl(byte_enum,				AttachmentsTypes, 0x144); // Enums::attachment_type [k_maximum_number_of_attachments_per_object]
			// game state datum_index
			// ie, if Attachments[x]'s definition (object_attachment_block[x]) says it is a 'cont'
			// then the datum_index is a contrail_data handle
			TStructGetPtrImpl(datum_index,				Attachments, 0x14C); // [k_maximum_number_of_attachments_per_object]
			TStructGetPtrImpl(datum_index,				FirstWidget, 0x16C);

			TStructGetPtrImpl(int16,					ShaderPermutation, 0x176); // shader's bitmap block index
			TStructGetPtrImpl(byte,						RegionVitality, 0x178); // [k_maximum_regions_per_model]
			TStructGetPtrImpl(byte,						RegionPermutationIndices, 0x180); 
			TStructGetPtrImpl(real_rgb_color,			ChangeColors, 0x1B8); // [4]
			TStructGetPtrImpl(s_object_header_block_reference, NodeOrientations, 0x1E8); // real_orientation3d[node_count]
			//TStructGetPtrImpl(s_object_header_block_reference, , 0x1EC); // real_orientation3d[node_count]
			TStructGetPtrImpl(s_object_header_block_reference, NodeMatrixBlock, 0x1F0); // real_matrix4x3[node_count]

			API_INLINE bool VerifyType(uint32 type_mask)
			{
				return TEST_FLAG( type_mask, *this->GetType() );
			}

		}; BOOST_STATIC_ASSERT( sizeof(s_object_data) == Enums::k_object_size_object );


		struct s_scenery_data
		{
			long_flags flags; // FLAG(0) - playing animation
		}; BOOST_STATIC_ASSERT( sizeof(s_scenery_data) == (Enums::k_object_size_scenery - Enums::k_object_size_object) );


		struct s_sound_scenery_data
		{
			UNUSED_TYPE(int32); // probably a long_flags field
		}; BOOST_STATIC_ASSERT( sizeof(s_sound_scenery_data) == (Enums::k_object_size_sound_scenery - Enums::k_object_size_object) );


		struct s_projectile_datum_network_data
		{
			real_point3d position;
			real_vector3d transitional_velocity;
		}; BOOST_STATIC_ASSERT( sizeof(s_projectile_datum_network_data) == 0x18 );
		struct s_projectile_data : TStructImpl(Enums::k_object_size_projectile - Enums::k_object_size_object)
		{
			enum { DATA_OFFSET = Enums::k_object_size_object, };

			TStructSubGetPtrImpl(long_flags,						Flags, 0x22C); // FLAG(1) - tracer
			// 0x230, _enum
			// 0x232, int16, looks like maybe some kind of index
			TStructSubGetPtrImpl(datum_index,						SourceUnit, 0x234); // maybe? set to this->object.OwnerObjectIndex's ultimate parent object index
			TStructSubGetPtrImpl(datum_index,						TargetObjectIndex, 0x238);
			TStructSubGetPtrImpl(int32,								ContrailAttachmentIndex, 0x23C); // index for the proj's definition's object_attachment_block
			TStructSubGetPtrImpl(real,								TimeRemaining, 0x240); // to target, or 0.0
			// 0x244, real, related to detonation coundown timer
			// 0x248, real
			// 0x24C, real, related to arming_time
			TStructSubGetPtrImpl(real,								RangeTraveled, 0x250); // If the proj definition's "maximum range" is > 0, divide <-this value by "maximum range" to get "range remaining"
			TStructSubGetPtrImpl(real_vector3d,						TransitionalVelocity, 0x254);
			// 0x260, real, set to water_damage_range's upper bound
			TStructSubGetPtrImpl(real_vector3d,						AngularVelocity, 0x264);
			// 0x270, real_euler_angles2d
			// 0x278 ?
			TStructSubGetPtrImpl(bool,								BaselineValid, 0x279);
			TStructSubGetPtrImpl(byte,								BaselineIndex, 0x27A);
			TStructSubGetPtrImpl(bool,								MessageIndex, 0x27B);
			TStructSubGetPtrImpl(s_projectile_datum_network_data,	UpdateBaseline, 0x27C);
			// 0x294, bool
			// 0x295, PAD24
			TStructSubGetPtrImpl(s_projectile_datum_network_data,	UpdateDelta, 0x298);
		};

		struct s_placeholder_data
		{
			UNUSED_TYPE(int32);
			UNUSED_TYPE(int32);
		}; BOOST_STATIC_ASSERT( sizeof(s_placeholder_data) == (Enums::k_object_size_placeholder - Enums::k_object_size_object) );




		struct s_scenery_datum
		{
			s_object_data object;
			s_scenery_data scenery;
		}; BOOST_STATIC_ASSERT( sizeof(s_scenery_datum) == Enums::k_object_size_scenery );

		struct s_sound_scenery_datum
		{
			s_object_data object;
			s_sound_scenery_data sound_scenery;
		}; BOOST_STATIC_ASSERT( sizeof(s_sound_scenery_datum) == Enums::k_object_size_sound_scenery );

		struct s_projectile_datum
		{
			s_object_data object;
			s_projectile_data projectile;
		}; BOOST_STATIC_ASSERT( sizeof(s_projectile_datum) == Enums::k_object_size_projectile );

		struct s_placeholder_datum
		{
			s_object_data object;
			s_placeholder_data placeholder;
		}; BOOST_STATIC_ASSERT( sizeof(s_placeholder_datum) == Enums::k_object_size_placeholder );
	};
};