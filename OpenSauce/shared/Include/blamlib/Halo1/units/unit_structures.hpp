/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

#include <blamlib/Halo1/game/game_globals.hpp>
#include <blamlib/Halo1/objects/object_structures.hpp>
#include <blamlib/Halo1/units/unit_control.hpp>
#include <blamlib/Halo1/units/unit_definitions.hpp>
#include <blamlib/Halo1/units/unit_dialogue.hpp>

namespace Yelo
{
	namespace Enums
	{
		enum unit_animation_state : sbyte
		{
			// TODO: remove this pragma warning off in any later migrations newer VS versions
			// http://connect.microsoft.com/VisualStudio/feedback/details/674442/enumeration-with-negative-values-are-popping-c4341-warnings
#pragma warning( push )
#pragma warning( disable : 4341 ) // signed value is out of range for enum constant
			_unit_animation_state_invalid = NONE,
#pragma warning( pop )

			_unit_animation_state_idle,
			_unit_animation_state_gesture,
			_unit_animation_state_turn_left,
			_unit_animation_state_turn_right,
			_unit_animation_state_move_front,
			_unit_animation_state_move_back,
			_unit_animation_state_move_left,
			_unit_animation_state_move_right,
			_unit_animation_state_stunned_front,
			_unit_animation_state_stunned_back,
			_unit_animation_state_stunned_left,
			_unit_animation_state_stunned_right,
			_unit_animation_state_slide_front,
			_unit_animation_state_slide_back,
			_unit_animation_state_slide_left,
			_unit_animation_state_slide_right,
			_unit_animation_state_ready,
			_unit_animation_state_put_away,
			_unit_animation_state_aim_still,
			_unit_animation_state_aim_move,
			_unit_animation_state_airborne,
			_unit_animation_state_land_soft,
			_unit_animation_state_land_hard,
			_unit_animation_state_unknown23,
			_unit_animation_state_airborne_dead,
			_unit_animation_state_landing_dead,
			_unit_animation_state_seat_enter,
			_unit_animation_state_seat_exit,
			_unit_animation_state_custom_animation,
			_unit_animation_state_impulse,
			_unit_animation_state_melee,
			_unit_animation_state_melee_airborne,
			_unit_animation_state_melee_continuous,
			_unit_animation_state_throw_grenade,
			_unit_animation_state_resurrect_front,
			_unit_animation_state_resurrect_back,
			_unit_animation_state_feeding,
			_unit_animation_state_surprise_front,
			_unit_animation_state_surprise_back,
			_unit_animation_state_leap_start,
			_unit_animation_state_leap_airborne,
			_unit_animation_state_leap_melee,
			_unit_animation_state_unknown42,		// unused AFAICT
			_unit_animation_state_berserk,

			_unit_animation_state
		};

		enum unit_camo_regrowth : _enum
		{
			_unit_camo_regrowth_off,
			_unit_camo_regrowth_on, // they fired their weapon, requiring active_camo_regrowth_rate to be applied
		};
	};

	namespace Flags
	{
		enum unit_flags
		{
			_unit_unk0_bit,
			_unit_unk1_bit,
			_unit_unk2_bit,
			_unit_unk3_bit,
			_unit_powerup_on_bit,
			_unit_powerup_additional_bit,
			_unit_controllable_bit,
			_unit_unk7_bit,
			_unit_unk8_bit,
			_unit_unk9_bit,
			_unit_unk10_bit,
			_unit_unk11_bit,
			_unit_unk12_bit,
			_unit_unk13_bit,
			_unit_unk14_bit,
			_unit_unk15_bit,
			_unit_unk16_bit,
			_unit_unk17_bit,
			_unit_unk18_bit,
			_unit_unk19_bit,
			_unit_doesnt_drop_items_bit,
			_unit_unk21_bit,
			_unit_can_blink_bit,
			_unit_unk23_bit,
			_unit_suspended_bit,
			_unit_unk25_bit,
			_unit_unk26_bit,
			_unit_possessed_bit,
			_unit_desires_flashlight_on_bit,
			_unit_desires_flashlight_off_bit,
			_unit_unk30_bit,
			_unit_unk31_bit,

			_unit_powerup_on_flag = FLAG(_unit_powerup_on_bit),
			_unit_powerup_additional_flag = FLAG(_unit_powerup_additional_bit),
			_unit_controllable_flag = FLAG(_unit_controllable_bit),
			_unit_doesnt_drop_items_flag = FLAG(_unit_doesnt_drop_items_bit),
			_unit_can_blink_flag = FLAG(_unit_can_blink_bit),
			_unit_suspended_flag = FLAG(_unit_suspended_bit),
			_unit_possessed_flag = FLAG(_unit_possessed_bit),
			_unit_desires_flashlight_on_flag = FLAG(_unit_desires_flashlight_on_bit),
			_unit_desires_flashlight_off_flag = FLAG(_unit_desires_flashlight_off_bit),
		};
	};

	namespace Objects
	{
		struct s_unit_animation_data : TStructImpl(0x40 + 0x8)
		{
			TStructGetPtrImpl(word_flags,			Flags, 0x0);			// 0x298
			//TStructGetPtrImpl(int16,				, 0x2);					// 0x29A animation index, weapon type
			//TStructGetPtrImpl(int16,				, 0x4);					// 0x29C animation index
			//TStructGetPtrImpl(int16,				, 0x6);					// 0x29E, appears unused except for getting initialized in unit_new
			//////////////////////////////////////////////////////////////////////////
			// animation graph unit indexes
			TStructGetPtrImpl(sbyte,				SeatIndex, 0x8);		// 0x2A0
			TStructGetPtrImpl(sbyte,				SeatWeaponIndex, 0x9);	// 0x2A1
			TStructGetPtrImpl(sbyte,				WeaponTypeIndex, 0xA);	// 0x2A2
			//////////////////////////////////////////////////////////////////////////
			TStructGetPtrImpl(sbyte,				AnimationState, 0xB);	// 0x2A3 [Enums::unit_animation_state]
			//TStructGetPtrImpl(sbyte,				, 0xC);					// 0x2A4
			//TStructGetPtrImpl(sbyte,				, 0xD);					// 0x2A5
			TStructGetPtrImpl(byte,				DesiredAnimationState, 0xE);// 0x2A6, set from s_unit_control_data's animation_state
			//TStructGetPtrImpl(sbyte,				, 0xF);					// 0x2A7
			//TStructGetPtrImpl(sbyte,				, 0x10);				// 0x2A8
			//PAD8?
			//TStructGetPtrImpl(uint16,				, 0x12);				// 0x2AA animation index
			//TStructGetPtrImpl(uint16,				, 0x14);				// 0x2AC
			//TStructGetPtrImpl(uint16,				, 0x16);				// 0x2AE
			// 0x18?
			//TStructGetPtrImpl(int16,				, 0x1A);				// 0x2B2
			// 0x1C?
			//TStructGetPtrImpl(byte?,				, 0x1E);				// 0x2B6 look related
			//TStructGetPtrImpl(byte?,				, 0x1F);				// 0x2B7 aim related
			TStructGetPtrImpl(real_rectangle2d,		LookingBounds, 0x20);	// 0x2B8
			TStructGetPtrImpl(real_rectangle2d,		AimingBounds, 0x30);	// 0x2C8
			// PAD64 (or unused)											// 0x2D8
		};

		struct s_unit_data
		{
			// These values are also used in determining assists
			// These values would be checked in the killed unit's data.
			struct s_recent_damage
			{
				int32 game_tick;				// the last game tick damage was dealt
				real damage;					// total (read: additive) damage the responsible object has done
				datum_index responsible_unit;
				datum_index responsible_player;	// would be NONE if killed by AI
			};

			datum_index actor_index;										// 0x1F4
			datum_index swarm_actor_index;									// 0x1F8
			UNKNOWN_TYPE(datum_index);										// 0x1FC swarm_next_unit_index?
			datum_index swarm_prev_unit_index;								// 0x200
			long_flags flags;												// 0x204
			long_flags control_flags;										// 0x208
			UNKNOWN_TYPE(int16);											// 0x20C related to the first two int16's in s_unit_globals_data
			sbyte shield_sapping;											// 0x20E
			sbyte base_seat_index;											// 0x20F
			struct {
				int32 ticks_remaining;										// 0x210
				long_flags flags;											// 0x214
			}persistent_control;
			datum_index controlling_player_index;							// 0x218
			_enum ai_effect_type;											// 0x21C ai_unit_effect
			int16 emotion_animation_index;									// 0x21E
			UNKNOWN_TYPE(int32);											// 0x220 time (game ticks) of next update for ai_effect_type related code
			real_vector3d desired_facing_vector;							// 0x224
			real_vector3d desired_aiming_vector;							// 0x230
			real_vector3d aiming_vector;									// 0x23C
			real_vector3d aiming_velocity;									// 0x248
			real_euler_angles3d looking_angles;								// 0x254
			real_vector3d looking_vector;									// 0x260
			real_vector3d looking_velocity;									// 0x26C
			real_vector3d throttle;											// 0x278
			real primary_trigger;											// 0x284
			byte_enum aiming_speed;											// 0x288
			UNKNOWN_TYPE(byte);												// 0x289 melee related (state enum?)
			UNKNOWN_TYPE(byte);												// 0x28A melee related (some kind of counter)
			sbyte ticks_until_flame_to_death;								// 0x28B
			// looks like the amount of frames left for the ping animation
			// also set to the same PersistentControlTicks value when an actor dies and they fire-wildely
			UNKNOWN_TYPE(byte);												// 0x28C sbyte
			byte_enum throwing_grenade_state;								// 0x28D
			UNKNOWN_TYPE(int16);											// 0x28E
			UNKNOWN_TYPE(int16);											// 0x290
			PAD16;															// 0x292
			datum_index thrown_grenade_projectile_index;					// 0x294
			s_unit_animation_data animation;								// 0x298
			real ambient;													// 0x2E0
			real illumination;												// 0x2E4
			real mouth_aperture;											// 0x2E8
			PAD32;															// 0x2EC unused
			int16 vehicle_seat_index;										// 0x2F0
			int16 current_weapon_index;										// 0x2F2
			int16 next_weapon_index;										// 0x2F4
			PAD16;															// 0x2F6 need to verify this is padding
			datum_index weapon_object_indices[Enums::k_maximum_weapons_per_unit];	// 0x2F8
			int32 weapon_ready_times[Enums::k_maximum_weapons_per_unit];	// 0x308
			datum_index equipment_index;									// 0x218
			sbyte current_grenade_index;									// 0x31C
			sbyte next_grenade_index;										// 0x31D
			byte grenade_counts[Enums::k_unit_grenade_types_count];			// 0x21E
		private:
			union {
				struct {
					byte zoom_level;										// 0x320
					byte desired_zoom_level;								// 0x321
				};
				//////////////////////////////////////////////////////////////////////////
				// OS-modified game states only
				byte grenade_counts_yelo[Enums::k_unit_grenade_types_count_yelo - Enums::k_unit_grenade_types_count];
			};
		public:
			sbyte last_vehicle_speech_timer;								// 0x322
			byte aiming_change;												// 0x323
			datum_index powered_seats_riders[Enums::k_number_of_powered_seats];	// 0x324
			//////////////////////////////////////////////////////////////////////////
			// these fields are all related
			UNKNOWN_TYPE(datum_index);										// 0x32C object index
			UNKNOWN_TYPE(int32);											// 0x330 game time
			UNKNOWN_TYPE(datum_index);										// 0x334 object index
			//////////////////////////////////////////////////////////////////////////
			real powered_seats_power[Enums::k_number_of_powered_seats];		// 0x338
			real integrated_light_power;									// 0x340
			real integrated_light_toggle_power;								// 0x344
			real integrated_night_vision_toggle_power;						// 0x348
			//////////////////////////////////////////////////////////////////////////
			// seat related
			UNKNOWN_TYPE(real_vector3d);									// 0x34C
			UNKNOWN_TYPE(real_vector3d);									// 0x358
			UNKNOWN_TYPE(real_vector3d);									// 0x364
			UNKNOWN_TYPE(real_vector3d);									// 0x370
			//////////////////////////////////////////////////////////////////////////
			real camo_power;												// 0x37C
			real full_spectrum_vision_power;								// 0x380 gets updated in unit_update, but nothing actually seems to *use* it...full spectrum vision power?
			datum_index dialogue_definition_index;							// 0x384
			struct {
				s_unit_speech current;										// 0x388
				s_unit_speech next;											// 0x3B8 not *positive* of this field
				UNKNOWN_TYPE(int16);										// 0x3E8
				UNKNOWN_TYPE(int16);										// 0x3EA
				UNKNOWN_TYPE(int16);										// 0x3EC
				UNKNOWN_TYPE(int16);										// 0x3EE
				UNKNOWN_TYPE(int32);										// 0x3F0 time related
				UNKNOWN_TYPE(bool);											// 0x3F4
				UNKNOWN_TYPE(bool);											// 0x3F5
				UNKNOWN_TYPE(bool);											// 0x3F6
				PAD8;														// 0x3F7
				UNKNOWN_TYPE(int16);										// 0x3F8
				UNKNOWN_TYPE(int16);										// 0x3FA
				UNKNOWN_TYPE(int16);										// 0x3FC
				UNKNOWN_TYPE(int16);										// 0x3FE
				UNKNOWN_TYPE(int32);										// 0x400
			}speech;
			//////////////////////////////////////////////////////////////////////////
			// these fields are all related; damage related
			UNKNOWN_TYPE(enum);												// 0x404
			UNKNOWN_TYPE(int16);											// 0x406
			UNKNOWN_TYPE(real);												// 0x408
			datum_index responsible_unit_index;								// 0x40C
			//////////////////////////////////////////////////////////////////////////
			datum_index responsible_flamer_object_index;					// 0x410 object which caused us to start flaming to death
			PAD64;															// 0x414 unused
			int32 death_time;												// 0x41C // game time when this unit died
			int16 feign_death_timer;										// 0x420
			Enums::unit_camo_regrowth camo_regrowth;						// 0x422
			real stun;														// 0x424
			int16 stun_timer;												// 0x428
			int16 killing_spree_count;										// 0x42A
			int32 killing_spree_start_time;									// 0x42C
			s_recent_damage recent_damage[4];								// 0x430
			PAD32;															// 0x470 unused
			//////////////////////////////////////////////////////////////////////////
			// Added in HaloPC
			UNKNOWN_TYPE(bool);												// 0x474 networking related. engine only writes to this, never reads. consider it 'unused'
			UNKNOWN_TYPE(bool);												// 0x475 networking related. engine only writes to this, never reads. consider it 'unused'
		private:
			union {															// 0x476
				PAD16;

				//////////////////////////////////////////////////////////////////////////
				// OS-modified game states only
				struct {
					byte zoom_level_yelo;
					byte desired_zoom_level_yelo;
				};
			};
		public:
			s_unit_control_data control_data;								// 0x478
			bool last_completed_client_update_id_valid;						// 0x4B8
			PAD24;
			int32 last_completed_client_update_id;							// 0x4BC
			PAD32;															// 0x4C0 unused
			PAD32;															// 0x4C4 unused
			PAD32;															// 0x4C8 unused
			//////////////////////////////////////////////////////////////////////////

		public: // see Objects/Units.cpp
			static const size_t k_offset_zoom_level;
			static const size_t k_offset_desired_zoom_level;

			static const size_t k_offset_zoom_level_yelo;
			static const size_t k_offset_desired_zoom_level_yelo;

			byte* GetYeloGrenade2Count();
			byte* GetYeloGrenade3Count();

			byte* GetZoomLevel();
			byte* GetDesiredZoomLevel();
		}; BOOST_STATIC_ASSERT( sizeof(s_unit_data) == (Enums::k_object_size_unit - Enums::k_object_size_object) );


		struct s_unit_datum
		{
			s_object_data object;
			s_unit_data unit;
		}; BOOST_STATIC_ASSERT( sizeof(s_unit_datum) == Enums::k_object_size_unit );
	};
};