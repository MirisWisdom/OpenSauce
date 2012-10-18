/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include "Objects/ObjectDefinitions.hpp"
#include <TagGroups/Halo1/game_globals_definitions.hpp>
#include <TagGroups/Halo1/unit_definitions.hpp>

namespace Yelo
{
	namespace Enums
	{
		enum {
			k_maximum_weapons_per_unit = 4,
		};

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
	};

	namespace Flags
	{
		enum unit_bits
		{
			_unit_unk1_flag = FLAG(0),
			_unit_unk2_flag = FLAG(1),
			_unit_unk3_flag = FLAG(2),
			_unit_unk4_flag = FLAG(3),
			_unit_powerup_on_flag = FLAG(4),
			_unit_powerup_additional_flag = FLAG(5),
			_unit_controllable_flag = FLAG(6),
			_unit_unk8_flag = FLAG(7),
			_unit_unk9_flag = FLAG(8),
			_unit_unk10_flag = FLAG(9),
			_unit_unk11_flag = FLAG(10),
			_unit_unk12_flag = FLAG(11),
			_unit_unk13_flag = FLAG(12),
			_unit_unk14_flag = FLAG(13),
			_unit_unk15_flag = FLAG(14),
			_unit_unk16_flag = FLAG(15),
			_unit_unk17_flag = FLAG(16),
			_unit_unk18_flag = FLAG(17),
			_unit_unk19_flag = FLAG(18),
			_unit_unk20_flag = FLAG(19),
			_unit_doesnt_drop_items_flag = FLAG(20),
			_unit_unk22_flag = FLAG(21),
			_unit_can_blink_flag = FLAG(22),
			_unit_unk24_flag = FLAG(23),
			_unit_suspended_flag = FLAG(24),
			_unit_unk26_flag = FLAG(25),
			_unit_unk27_flag = FLAG(26),
			_unit_possessed_flag = FLAG(27),
			_unit_desires_flashlight_on_flag = FLAG(28),
			_unit_desires_flashlight_off_flag = FLAG(29),
			_unit_unk31_flag = FLAG(30),
			_unit_unk32_flag = FLAG(31)
		};

		enum biped_flags
		{
			_biped_airborne_flag = FLAG(0),
			_biped_hovering_flag = FLAG(1),
		};
	};

	namespace Objects
	{
		struct s_unit_animation_data : TStructImpl(0x40)
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
		struct s_unit_data : TStructImpl(Enums::k_object_size_unit - Enums::k_object_size_object)
		{
			enum { DATA_OFFSET = Enums::k_object_size_object, 

				//////////////////////////////////////////////////////////////////////////
				// Stock offsets
				k_offset_ZoomLevel = 0x320,
				k_offset_DesiredZoomLevel = 0x321,
				//////////////////////////////////////////////////////////////////////////
				// Offsets in OS-modified game states
				k_offset_ZoomLevel_Yelo = 0x4BA,
				k_offset_DesiredZoomLevel_Yelo = 0x4BB,
			};

			TStructSubGetPtrImpl(datum_index,			ActorIndex, 0x1F4);
			TStructSubGetPtrImpl(datum_index,			SwarmActorIndex, 0x1F8);
			//TStructSubGetPtrImpl(datum_index,			, 0x1FC);
			TStructSubGetPtrImpl(datum_index,			SwamPrevUnitIndex, 0x200);
			TStructSubGetPtrImpl(long_flags,			Flags, 0x204);
			TStructSubGetPtrImpl(long_flags,			ControlFlags, 0x208); // zero extended unit control flags (which normally use word_flags)
			//TStructSubGetPtrImpl(uint16,				, 0x20C); // related to the first two int16's in s_unit_globals_data
			TStructSubGetPtrImpl(sbyte,					ShieldSapping, 0x20E);
			TStructSubGetPtrImpl(sbyte,					BaseSeatIndex, 0x20F);
			//TStructSubGetPtrImpl(uint32,				ControlTime?, 0x210);
			//TStructSubGetPtrImpl(long_flags,			ControlFlags, 0x214);
			TStructSubGetPtrImpl(datum_index,			ControllingPlayerIndex, 0x218);
			//TStructSubGetPtrImpl(_enum,				EffectType, 0x21C); // ai_unit_effect
			TStructSubGetPtrImpl(int16,					EmotionAnimationIndex, 0x21E);
			//TStructSubGetPtrImpl(uint32,				, 0x220); // time (game ticks) of next update for EffectType related code
			TStructSubGetPtrImpl(real_vector3d,			DesiredFacingVector, 0x224);
			TStructSubGetPtrImpl(real_vector3d,			DesiredAimingVector, 0x230);
			TStructSubGetPtrImpl(real_vector3d,			AimingVector, 0x23C);
			TStructSubGetPtrImpl(real_vector3d,			AimingVelocity, 0x248);
			TStructSubGetPtrImpl(real_euler_angles3d,	LookingAngles, 0x254);
			TStructSubGetPtrImpl(real_vector3d,			LookingVector, 0x260);
			TStructSubGetPtrImpl(real_vector3d,			LookingVelocity, 0x26C);
			TStructSubGetPtrImpl(real_vector3d,			Throttle, 0x278);
			TStructSubGetPtrImpl(real,					PrimaryTrigger, 0x284);
			TStructSubGetPtrImpl(byte,					AimingSpeed, 0x288);
			//TStructSubGetPtrImpl(byte,				, 0x289); // melee related (state enum?)
			//TStructSubGetPtrImpl(byte,				, 0x28A); // melee related (some kind of counter)
			TStructSubGetPtrImpl(sbyte,					TicksUntilFlameToDeath, 0x28B); // related to flame deaths (some kind of counter)
			//TStructSubGetPtrImpl(sbyte,				, 0x28C); // looks like the amount of frames left for the ping animation
			TStructSubGetPtrImpl(byte,					ThrowingGrenadeState, 0x28D);
			//TStructSubGetPtrImpl(int16,				, 0x28E);
			//TStructSubGetPtrImpl(int16,				, 0x290);
			// 0x292 PAD16 (or unused)
			TStructSubGetPtrImpl(datum_index,			ThrownGrenade, 0x294); // object_index to the grenade being thrown
			TStructSubGetPtrImpl(s_unit_animation_data,	Animation, 0x298);
			TStructSubGetPtrImpl(real,					Ambient, 0x2E0);
			TStructSubGetPtrImpl(real,					Illumination, 0x2E4);
			TStructSubGetPtrImpl(real,					MouthAperture, 0x2E8);
			// 0x2EC PAD32 (or unused)
			TStructSubGetPtrImpl(int16,					VehicleSeatIndex, 0x2F0);
			TStructSubGetPtrImpl(int16,					CurrentWeaponIndex, 0x2F2);
			TStructSubGetPtrImpl(int16,					NextWeaponIndex, 0x2F4);
			TStructSubGetPtrImpl(datum_index,			WeaponObjectIndices, 0x2F8); // [k_maximum_weapons_per_unit]
			TStructSubGetPtrImpl(uint32,				WeaponReadyTimes, 0x308); // [k_maximum_weapons_per_unit]
			TStructSubGetPtrImpl(datum_index,			CurrentEquipment, 0x318);
			TStructSubGetPtrImpl(sbyte,					CurrentGrenadeIndex, 0x31C);
			TStructSubGetPtrImpl(sbyte,					NextGrenadeIndex, 0x31D);
			TStructSubGetPtrImpl(byte,					GrenadeCounts, 0x31E); // [k_number_of_unit_grenade_types]
				TStructSubGetPtrImpl(byte,				GrenadeFragCount, 0x31E);
				TStructSubGetPtrImpl(byte,				GrenadePlasmaCount, 0x31F);
		private:
				//////////////////////////////////////////////////////////////////////////
				// OS-modified game states only
				TStructSubGetPtrImpl(byte,				YeloGrenade2Count_, 0x320);
				TStructSubGetPtrImpl(byte,				YeloGrenade3Count_, 0x321);
				//////////////////////////////////////////////////////////////////////////
			TStructSubGetPtrImpl(byte,					ZoomLevel_, k_offset_ZoomLevel);
			TStructSubGetPtrImpl(byte,					DesiredZoomLevel_, k_offset_DesiredZoomLevel);
		public:
			//TStructSubGetPtrImpl(sbyte,					, 0x322);
			TStructSubGetPtrImpl(byte,					AimingChange, 0x323);
			TStructSubGetPtrImpl(datum_index,			DriverObj, 0x324);
			TStructSubGetPtrImpl(datum_index,			GunnerObj, 0x328);
			// 32C?
			// 330?
			// 334?
			TStructSubGetPtrImpl(real,					DriverPower, 0x338);
			TStructSubGetPtrImpl(real,					GunnerPower, 0x33C);
			TStructSubGetPtrImpl(int32,					IntegratedLightPower, 0x340);

			// 0x348, real, related to zoom, night vision, and the player's hud
			//TStructSubGetPtrImpl(real_vector3d,			, 0x34C); // seat related
			//TStructSubGetPtrImpl(real_vector3d,			, 0x358); // seat related
			//TStructSubGetPtrImpl(real_vector3d,			, 0x364); // seat related
			//TStructSubGetPtrImpl(real_vector3d,			, 0x370); // seat related
			TStructSubGetPtrImpl(real,					CamoPower, 0x37C);

			// 0x3B8, int16

			TStructSubGetPtrImpl(datum_index,			ResponsibleFlamerObjectIndex, 0x410); // object which caused us to start flaming to death

			TStructSubGetPtrImpl(int16,					FeignDeathTimer, 0x420);

			// These values are also used in determining assists
			// These values would be checked in the killed unit's data.
			struct s_recent_damage
			{
				uint32 game_tick;				// the last game tick damage was dealt
				real damage;					// total (read: additive) damage the responsible object has done
				datum_index responsible_unit;
				datum_index responsible_player;	// would be NONE if killed by AI
			};
			TStructSubGetPtrImpl(s_recent_damage,		RecentDamage, 0x430); // [4]

			// 0x474, bool, networking related
			// 0x475, bool, networking related

			// 0x4B8 bool, true if LastCompletedClientUpdateId != NONE
			//PAD24;
			TStructSubGetPtrImpl(int32,					LastCompletedClientUpdateId, 0x4BC);

		public: // see Objects/Units.cpp
			byte* GetYeloGrenade2Count();
			byte* GetYeloGrenade3Count();

			byte* GetZoomLevel();
			byte* GetDesiredZoomLevel();
		private:
			//////////////////////////////////////////////////////////////////////////
			// OS-modified game states only
			TStructSubGetPtrImpl(byte,					YeloZoomLevel, k_offset_ZoomLevel_Yelo);
			TStructSubGetPtrImpl(byte,					YeloDesiredZoomLevel, k_offset_DesiredZoomLevel_Yelo);
			//////////////////////////////////////////////////////////////////////////
		};

		struct s_biped_datum_network_data
		{
			byte grenade_counts[Enums::k_unit_grenade_types_count];
			PAD16;
			real body_vitality, shield_vitality;
			bool shield_stun_ticks_greater_than_zero;
			PAD24;
		}; BOOST_STATIC_ASSERT( sizeof(s_biped_datum_network_data) == 0x10 );
		struct s_biped_data : TStructImpl(Enums::k_object_size_biped - Enums::k_object_size_unit)
		{
			enum { DATA_OFFSET = Enums::k_object_size_unit, };

			TStructSubGetPtrImpl(long_flags,					Flags, 0x4CC);

			// 0x4F8, int32
			TStructSubGetPtrImpl(real_point3d,					HoveringPosition, 0x4FC);

			// 0x514, real_plane3d
			// 0x524 ?
			TStructSubGetPtrImpl(bool,							BaselineValid, 0x526);
			TStructSubGetPtrImpl(byte,							BaselineIndex, 0x527);
			TStructSubGetPtrImpl(byte,							MessageIndex, 0x528);
			TStructSubGetPtrImpl(s_biped_datum_network_data,	UpdateBaseline, 0x52C);
			TStructSubGetPtrImpl(bool,							ShieldStunTicksGreaterThanZero, 0x538);
			TStructSubGetPtrImpl(s_biped_datum_network_data,	UpdateDelta, 0x540);
		};

		struct s_vehicle_datum_network_data
		{
			bool at_rest_bit;
			PAD24;
			real_point3d position;
			real_vector3d transitional_velocity;
			real_vector3d angular_velocity;
			real_vector3d forward;
			real_vector3d up;
		}; BOOST_STATIC_ASSERT( sizeof(s_vehicle_datum_network_data) == 0x40 );
		struct s_vehicle_data : TStructImpl(Enums::k_object_size_vehicle - Enums::k_object_size_unit)
		{
			enum { DATA_OFFSET = Enums::k_object_size_unit, };

			TStructSubGetPtrImpl(word_flags,					Flags, 0x4CC);
			// 0x4CE, WORD
			// 0x4D0, BYTE
			// 0x4D1, BYTE
			// 0x4D2, BYTE
			// 0x4D3, BYTE
			// 0x4D4, DWORD
			// 0x4D8, DWORD
			// 0x4DC, DWORD
			// 0x4E0, DWORD
			// 0x4E4, DWORD
			// 0x4E8, DWORD
			// 0x4EC, DWORD
			// 0x4F0, DWORD
			// 0x4F4, sizeof(0x8)
			// 0x4FC, ?, probably a real_vector3d or something here
			// 0x500, ?
			// 0x504, ?
			// 0x508, DWORD
			// 0x50C, DWORD
			// 0x510, DWORD
			// 0x514, DWORD
			// 0x518, DWORD
			// 0x51C, DWORD
			// 0x520, DWORD
			TStructSubGetPtrImpl(bool,							NetworkTimeValid, 0x524);
			TStructSubGetPtrImpl(bool,							BaselineValid, 0x525);
			TStructSubGetPtrImpl(byte,							BaselineIndex, 0x526);
			TStructSubGetPtrImpl(byte,							MessageIndex, 0x527);
			TStructSubGetPtrImpl(s_vehicle_datum_network_data,	UpdateBaseline, 0x528);
			// 0x568 unused?
			TStructSubGetPtrImpl(s_vehicle_datum_network_data,	UpdateDelta, 0x56C);
			// 0x5AC, uint32, game time related
			// block index of the scenario datum used for respawning
			// For all game engines besides race, this will be a scenario vehicle datum
			// For race, it's a scenario_netpoint, aka "scenario_netgame_flags_block"
			TStructSubGetPtrImpl(int16,							ScenarioDatumRespawningIndex, 0x5B0);
			// 0x5B2 PAD16
			// 0x5B4, real_point3d, position based
		};



		struct s_unit_datum
		{
			s_object_data object;
			s_unit_data unit;
		}; BOOST_STATIC_ASSERT( sizeof(s_unit_datum) == Enums::k_object_size_unit );

		struct s_biped_datum : s_unit_datum
		{
			s_biped_data biped;
		}; BOOST_STATIC_ASSERT( sizeof(s_biped_datum) == Enums::k_object_size_biped );

		struct s_vehicle_datum : s_unit_datum
		{
			s_vehicle_data vehicle;
		}; BOOST_STATIC_ASSERT( sizeof(s_vehicle_datum) == Enums::k_object_size_vehicle );
	};
};