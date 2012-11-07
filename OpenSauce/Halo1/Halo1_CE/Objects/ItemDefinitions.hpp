/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <blamlib/Halo1/items/item_definitions.hpp>
#include <blamlib/Halo1/items/equipment_definitions.hpp>
#include <blamlib/Halo1/items/weapon_definitions.hpp>

#include "Objects/ObjectDefinitions.hpp"

namespace Yelo
{
	namespace Enums
	{
		enum weapon_state : byte_enum
		{
			_weapon_state_idle,

			_weapon_state_fire1,	_weapon_state_fire2,
			_weapon_state_chamber1,	_weapon_state_chamber2,
			_weapon_state_reload1,	_weapon_state_reload2,
			_weapon_state_charged1,	_weapon_state_charged2,

			_weapon_state_ready,
			_weapon_state_put_away,

			_weapon_state,
		};

		enum weapon_trigger_state : byte_enum
		{
			_weapon_trigger_state_idle,
			_weapon_trigger_state_fire1,
			_weapon_trigger_state_fire2,
			_weapon_trigger_state_unknown3, // overcharged related
			_weapon_trigger_state_unknown4, // locked related (tracking)
			_weapon_trigger_state_unknown5, // tracking related
			_weapon_trigger_state_unknown6,
			_weapon_trigger_state_unknown7, // 1
			_weapon_trigger_state_unknown8, // 2

			_weapon_trigger_state,
		};

		enum weapon_magazine_state : _enum
		{
			_weapon_magazine_state_idle,
			_weapon_magazine_state_chambering_start,
			_weapon_magazine_state_chambering_finish,
			_weapon_magazine_state_chambering,
			_weapon_magazine_state,
		};
	};

	namespace Objects
	{
		struct s_item_data : TStructImpl(Enums::k_object_size_item - Enums::k_object_size_object)
		{
			enum { DATA_OFFSET = Enums::k_object_size_object, };

			TStructSubGetPtrImpl(long_flags,			Flags, 0x1F4);
			//TStructSubGetPtrImpl(s_scenario_location,	, 0x1F8);
			// 0x1FC?
			//TStructSubGetPtrImpl(datum_index,					, 0x200); // object index
			TStructSubGetPtrImpl(int32,					LastUpdateTime, 0x204);
			//TStructSubGetPtrImpl(datum_index,					, 0x208); // object index
			//TStructSubGetPtrImpl(real_point3d,				, 0x20C);
			// 0x210?
			// 0x214?
			//TStructSubGetPtrImpl(real_vector3d,				, 0x218);
			//TStructSubGetPtrImpl(real_euler_angles2d,			, 0x224);
		};


		//////////////////////////////////////////////////////////////////////////
		// weapon

		struct s_weapon_datum_network_data
		{
			real_point3d position;
			real_vector3d transitional_velocity;
			PAD32; PAD32; PAD32; // not used in the update...probably a real_vector3d (angular_velocity?)
			int16 magazine_rounds_totals[Enums::k_maximum_number_of_magazines_per_weapon];
			real age;
		}; BOOST_STATIC_ASSERT( sizeof(s_weapon_datum_network_data) == 0x2C );
		struct s_weapon_data : TStructImpl(Enums::k_object_size_weapon - Enums::k_object_size_item)
		{
			enum { DATA_OFFSET = Enums::k_object_size_item, };

			struct s_trigger_state
			{
				PAD8; // sbyte
				Enums::weapon_trigger_state state;
				int16 time;
				UNKNOWN_TYPE(long_flags);			// 0x4
				UNKNOWN_TYPE(int16);				// 0x8 firing effect related
				UNKNOWN_TYPE(int16);				// 0xA firing effect related
				UNKNOWN_TYPE(int16);				// 0xC firing effect related
				int16 rounds_since_last_tracer;
				real rate_of_fire;					// 0x10
				real ejection_port_recovery_time;	// 0x14
				real illumination_recovery_time;	// 0x18
				UNKNOWN_TYPE(real);					// 0x1C used in the calculation of projectile error angle
				datum_index charging_effect_index;	// 0x20
				PAD32; // ?
			}; BOOST_STATIC_ASSERT( sizeof(s_trigger_state) == 0x28 );
			struct s_magazine_state // '?' means IDK if its actually padding or there are values there. If there are, IDK their types (could be a boolean!)
			{
				Enums::weapon_magazine_state state;
				int16 chamber_time;					// 0x2 in ticks
				UNKNOWN_TYPE(int16);				// 0x4 uint16, appears to be another game tick based value (animations?)
				int16 rounds_unloaded;				// 0x6
				int16 rounds_loaded;				// 0x8
				UNKNOWN_TYPE(int16);				// 0xA uint16, appears to be another game tick based value
				UNKNOWN_TYPE(int16);				// 0xC I just know a WORD is here, may be an _enum
				PAD16; // ?
			}; BOOST_STATIC_ASSERT( sizeof(s_magazine_state) == 0x10 );
			struct s_start_reload_data
			{
				int16 starting_total_rounds[Enums::k_maximum_number_of_magazines_per_weapon];
				int16 starting_loaded_rounds[Enums::k_maximum_number_of_magazines_per_weapon];
			}; BOOST_STATIC_ASSERT( sizeof(s_start_reload_data) == 0x8 );

			// FLAG(3) - _weapon_must_be_readied_bit
			TStructSubGetPtrImpl(long_flags,					Flags, 0x22C);
			TStructSubGetPtrImpl(word_flags,					OwnerFlags, 0x230); // owner being the controlling unit
			// 0x232 PAD16
			TStructSubGetPtrImpl(real,							PrimaryTrigger, 0x234);
			TStructSubGetPtrImpl(Enums::weapon_state,			WeaponState, 0x238);
			// 0x23A, uint16, some kind of tick countdown
			TStructSubGetPtrImpl(real,							Heat, 0x23C);
			TStructSubGetPtrImpl(real,							Age, 0x240);
			TStructSubGetPtrImpl(real_fraction,					IlluminationFraction, 0x244);
			TStructSubGetPtrImpl(real,							IntegratedLightPower, 0x248);
			// 0x24C unused
			TStructSubGetPtrImpl(datum_index,					TrackedObject, 0x250);
			// 0x254 unused
			// 0x258 unused
			TStructSubGetPtrImpl(int16,							AltShotsLoaded, 0x25C);
			// 0x25E PAD16
			TStructSubGetPtrImpl(s_trigger_state,				Triggers, 0x260); // [Enums::k_maximum_number_of_magazines_per_weapon]
			TStructSubGetPtrImpl(s_magazine_state,				Magazines, 0x2B0); // [Enums::k_maximum_number_of_magazines_per_weapon]
			TStructSubGetPtrImpl(int32,							LastTriggerFireTime, 0x2D0);
			TStructSubGetPtrImpl(s_start_reload_data,			StartReloadUpdate, 0x2D4);
			// 0x2DC PAD32?
			TStructSubGetPtrImpl(bool,							BaselineValid, 0x2E0);
			TStructSubGetPtrImpl(sbyte,							BaselineIndex, 0x2E1);
			TStructSubGetPtrImpl(sbyte,							MessageIndex, 0x2E2);
			// 0x2E3 PAD8
			TStructSubGetPtrImpl(s_weapon_datum_network_data,	UpdateBaseline, 0x2E4);
			// 0x310, bool
			// 0x311, PAD24
			TStructSubGetPtrImpl(s_weapon_datum_network_data,	UpdateDelta, 0x314);
		};


		//////////////////////////////////////////////////////////////////////////
		// equipment

		struct s_equipment_datum_yelo_network_data
		{
		};
		struct s_equipment_data_yelo
		{
			enum { k_max_size = 0x18 }; // size of the unknown/useless data in s_equipment_data

		}; BOOST_STATIC_ASSERT( sizeof(s_equipment_data_yelo) <= s_equipment_data_yelo::k_max_size );
		struct s_equipment_datum_network_data
		{
			real_point3d position;
			real_vector3d transitional_velocity;
			real_vector3d angular_velocity;
		}; BOOST_STATIC_ASSERT( sizeof(s_equipment_datum_network_data) == 0x24 );
		struct s_equipment_data
		{
			union {												// 0x22C, unknown (and unused?) bytes
				struct s_unknown { PAD128; PAD64; }unknown;

				s_equipment_data_yelo yelo;
			};
			bool baseline_valid;								// 0x244
			sbyte baseline_index;
			sbyte message_index;
			PAD8;
			s_equipment_datum_network_data update_baseline;		// 0x248
			UNKNOWN_TYPE(bool);									// 0x26C probably delta_valid
			PAD24;
			s_equipment_datum_network_data update_delta;		// 0x270
		}; BOOST_STATIC_ASSERT( sizeof(s_equipment_data) == (Enums::k_object_size_equipment - Enums::k_object_size_item) );

		struct s_garbage_data
		{
			int16 ticks_until_gc;
			PAD16;
			int32 _unused[5];
		}; BOOST_STATIC_ASSERT( sizeof(s_garbage_data) == (Enums::k_object_size_garbage - Enums::k_object_size_item) );




		struct s_item_datum
		{
			s_object_data object;
			s_item_data item;
		}; BOOST_STATIC_ASSERT( sizeof(s_item_datum) == Enums::k_object_size_item );

		struct s_weapon_datum : s_item_datum
		{
			s_weapon_data weapon;
		}; BOOST_STATIC_ASSERT( sizeof(s_weapon_datum) == Enums::k_object_size_weapon );

		struct s_equipment_datum : s_item_datum
		{
			s_equipment_data equipment;
		}; BOOST_STATIC_ASSERT( sizeof(s_equipment_datum) == Enums::k_object_size_equipment );

		struct s_garbage_datum : s_item_datum
		{
			s_garbage_data garbage;
		}; BOOST_STATIC_ASSERT( sizeof(s_garbage_datum) == Enums::k_object_size_garbage );
	};
};