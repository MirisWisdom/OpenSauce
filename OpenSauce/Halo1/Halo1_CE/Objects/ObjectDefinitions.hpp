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
#pragma once

namespace Yelo
{
	namespace Enums
	{
		enum global_game_team // I just put this definition here because it seemed best...should be somewhere under the Game folder tho
		{
			_global_game_team_default,
			_global_game_team_player,
			_global_game_team_human,
			_global_game_team_covenant,
			_global_game_team_flood,
			_global_game_team_sentinel,
			_global_game_team_unused6,
			_global_game_team_unused7,
			_global_game_team_unused8,
			_global_game_team_unused9,

			_global_game_team
		};

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

		enum networked_datum
		{
			_networked_datum_master,
			_networked_datum_puppet,
			_networked_datum_puppet_controlled_by_local_player,
		};

		enum object_sizes
		{
			k_object_size_object =			0x1F4,
			k_object_size_garbage =			0x50 + k_object_size_object,
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

			k_object_size_unit =			0x2D8+ k_object_size_object,
			k_object_size_biped =			0x84 + k_object_size_unit,
			k_object_size_vehicle =			0xF4 + k_object_size_unit,
		};
	};

	namespace Flags
	{
		enum object_bits
		{
			// --- 0x2

			_object_activated_bit = BIT32(0),

			// --- 0x10

			_object_unk1_bit = BIT32(0),
			_object_unk2_bit = BIT32(1),
			_object_unk3_bit = BIT32(2),
			_object_connected_to_map_bit = BIT32(3),
			_object_unk5_bit = BIT32(4),
			_object_update_physics_bit = BIT32(5),
			_object_unk7_bit = BIT32(6),
			_object_unk8_bit = BIT32(7),
			_object_unk9_bit = BIT32(8),
			_object_unk10_bit = BIT32(9),
			_object_unk12_bit = BIT32(10),
			_object_unk13_bit = BIT32(11),
			_object_unk14_bit = BIT32(12),
			_object_unk15_bit = BIT32(13),
			_object_unk16_bit = BIT32(14),
			_object_unk17_bit = BIT32(15),
			_object_garbage_bit = BIT32(16),
			_object_unk18_bit = BIT32(17),
			_object_unk19_bit = BIT32(18),
			_object_unk20_bit = BIT32(19),
			_object_unk21_bit = BIT32(20),
			_object_unk22_bit = BIT32(21),
			_object_beautify_bit = BIT32(22),
			_object_unk24_bit = BIT32(23),
			_object_collideable_bit = BIT32(24),
			_object_unk26_bit = BIT32(25),
			_object_unk27_bit = BIT32(26),
			_object_unk28_bit = BIT32(27),
			_object_unk29_bit = BIT32(28),
			_object_unk30_bit = BIT32(29),
			_object_unk31_bit = BIT32(30),
			_object_unk32_bit = BIT32(31),

			// --- 0x106 & 0x107

			_object_should_be_tracked_bit = BIT32(2),
			_object_killed_bit = BIT32(5),
			_object_killed_silent_bit = BIT32(6),
			// 1
			// 2
			// 4
			_object_cannot_take_damage_bit = BIT32(11), // 8
			_object_killed_no_statistics = BIT32(13), // 32
		};
	};

	namespace Objects
	{
		struct s_object_header_block_reference
		{
			uint16 offset;
			uint16 size;
		};

		struct s_object_network_datum_data
		{
			datum_index owner_player_index;
			datum_index owner;
			int16 owner_team_index;
			PAD16;
			real_point3d position;
			real_vector3d transitional_velocity;
			real_vector3d forward;
			real_vector3d up;
			real_vector3d angular_velocity;
		}; BOOST_STATIC_ASSERT( sizeof(s_object_network_datum_data) == 0x48 );

		struct s_object_data : TStructImpl( Enums::k_object_size_object )
		{
			TStructGetPtrImpl(datum_index,				TagDefinition, 0x0);
			// 0x8 - boolean
			// 0x9 - boolean
			TStructGetPtrImpl(uint32,					NetworkTime, 0xC);
			TStructGetPtrImpl(long_flags,				Flags, 0x10);
			TStructGetPtrImpl(s_object_network_datum_data, NetworkDatumData, 0x50);
			TStructGetPtrImpl(s_scenario_location,		ScenarioLocation, 0x98);
			//TStructGetPtrImpl(real_point3d,				, 0xA0); // position, used for stock game code's trigger volume point testing
			TStructGetPtrImpl(real,						Scale, 0xB0);
			TStructGetPtrImpl(byte,						Type, 0xB4);
			TStructGetPtrImpl(int16,					OwnerTeamIndex, 0xB8);
			TStructGetPtrImpl(int16,					NameListIndex, 0xBA);
			TStructGetPtrImpl(int16,					RegionPermutationVariantId, 0xBE);
			TStructGetPtrImpl(datum_index,				PlayerIndex, 0xC0);
			// If this were a projectile, I believe this would be the handle to
			// the weapon which spawned it
			TStructGetPtrImpl(datum_index,				OwnerObjectIndex, 0xC4);
			

			TStructGetPtrImpl(datum_index,				AnimationDefinition, 0xCC);
			TStructGetPtrImpl(int16,					AnimationCurrentIndex, 0xD0);
			TStructGetPtrImpl(int16,					AnimationCurrentFrameIndex, 0xD2);

			TStructGetPtrImpl(real,						MaximumVitality, 0xD8);
			TStructGetPtrImpl(real,						CurrentVitality, 0xDC);
			TStructGetPtrImpl(real,						Health, 0xE0);
			TStructGetPtrImpl(real,						Shields, 0xE4);
			TStructGetPtrImpl(datum_index,				NextObjectIndex, 0x114);
			TStructGetPtrImpl(datum_index,				ParentObjectIndex, 0x11C);
			TStructGetPtrImpl(real,						ExportFunctionValues, 0x124); // [4]
			// game state datum_index
			// ie, if Attachments[x]'s definition (object_attachment_block[x]) says it is a 'cont'
			// then the datum_index is a contrail_data handle
			TStructGetPtrImpl(datum_index,				Attachments, 0x14C); // [k_maximum_number_of_attachments_per_object]
			TStructGetPtrImpl(int16,					CubemapIndex, 0x176); // bitmap block index, found by: "Choking Victim"
			TStructGetPtrImpl(byte,						RegionVitality, 0x178); // [k_maximum_regions_per_model]
			TStructGetPtrImpl(s_object_header_block_reference, NodeMatrixBlock, 0x1F0);

			API_INLINE bool VerifyType(uint32 type_mask)
			{
				return TEST_BIT32( type_mask, *this->GetType() );
			}

		}; BOOST_STATIC_ASSERT( sizeof(s_object_data) == Enums::k_object_size_object );


		struct s_scenery_data : TStructImpl(Enums::k_object_size_scenery - Enums::k_object_size_object)
		{
			enum { DATA_OFFSET = Enums::k_object_size_object, };
		};


		struct s_sound_scenery_data : TStructImpl(Enums::k_object_size_sound_scenery - Enums::k_object_size_object)
		{
			enum { DATA_OFFSET = Enums::k_object_size_object, };
		};


		struct s_garbage_data : TStructImpl(Enums::k_object_size_garbage - Enums::k_object_size_object)
		{
			enum { DATA_OFFSET = Enums::k_object_size_object, };
		};


		struct s_projectile_data : TStructImpl(Enums::k_object_size_projectile - Enums::k_object_size_object)
		{
			enum { DATA_OFFSET = Enums::k_object_size_object, };

			// 0x22C - long_flags, BIT32(1) - tracer
			// 0x230 - _enum
			// 0x232 - int16, looks like maybe some kind of index

			TStructSubGetPtrImpl(datum_index,		SourceUnit, 0x234); // maybe? set to this->object.OwnerObjectIndex's ultimate parent object index
			TStructSubGetPtrImpl(datum_index,		TargetObjectIndex, 0x238);
			TStructSubGetPtrImpl(int32,				ContrailAttachmentIndex, 0x23C); // index for the proj's definition's object_attachment_block
			TStructSubGetPtrImpl(real,				TimeRemaining, 0x240); // to target, or 0.0
			// 0x244 - real
			// 0x248 - real
			// 0x24C - real
			TStructSubGetPtrImpl(real,				RangeTraveled, 0x250); // If the proj definition's "maximum range" is > 0, divide <-this value by "maximum range" to get "range remaining"
			TStructSubGetPtrImpl(real_vector3d,		TransitionalVelocity, 0x254);
			TStructSubGetPtrImpl(real_vector3d,		AngularVelocity, 0x260);
			// 0x26C - real_vector3d
			TStructSubGetPtrImpl(bool,				BaselineValid, 0x279);
			TStructSubGetPtrImpl(byte,				BaselineIndex, 0x27A);
			TStructSubGetPtrImpl(bool,				MessageIndex, 0x27B);
			TStructSubGetPtrImpl(real_vector3d,		Position, 0x27C);
			TStructSubGetPtrImpl(real_vector3d,		TranslationalVelocity, 0x288);
		};

		struct s_placeholder_data : TStructImpl(Enums::k_object_size_placeholder - Enums::k_object_size_object)
		{
			enum { DATA_OFFSET = Enums::k_object_size_object, };
		};




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

		struct s_garbage_datum
		{
			s_object_data object;
			s_garbage_data garbage;
		}; BOOST_STATIC_ASSERT( sizeof(s_garbage_datum) == Enums::k_object_size_garbage );

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