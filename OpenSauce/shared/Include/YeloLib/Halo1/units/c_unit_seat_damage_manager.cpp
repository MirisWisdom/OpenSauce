/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include <YeloLib/Halo1/units/c_unit_seat_damage_manager.hpp>

#include <blamlib/Halo1/objects/objects.hpp>
#include <blamlib/Halo1/objects/damage.hpp>
#include <blamlib/Halo1/units/units.hpp>
#include <blamlib/Halo1/units/unit_definitions.hpp>
#include <blamlib/Halo1/items/projectiles.hpp>
#include <blamlib/Halo1/items/weapons.hpp>
#include <blamlib/Halo1/items/weapon_definitions.hpp>
#include <blamlib/Halo1/items/weapon_structures.hpp>

#include <YeloLib/Halo1/units/unit_definitions_yelo.hpp>
#include <YeloLib/Halo1/units/units_yelo.hpp>

namespace Yelo
{
	namespace Objects { namespace Units { namespace SeatDamage
	{
		void c_unit_seat_damage_manager::AttachGrenadeToUnit(const datum_index unit_index
			, s_unit_datum* unit_datum
			, const datum_index projectile_index
			, const TagGroups::unit_seat_damage& seat_damage) const
		{
			auto* projectile_datum = blam::object_get_and_verify_type<s_projectile_datum>(projectile_index);

			// Reset the projectiles countdown and set it's new detonation time
			projectile_datum->projectile.time_remaining = 0;
			projectile_datum->projectile.arming_rate = (1.0f / 30.0f) / seat_damage.grenade_detonation_time_scale;

			// Detach from the player's hand and attach to the target unit
			blam::object_detach(projectile_index);
			blam::object_attach_to_marker(unit_index, seat_damage.grenade_marker, projectile_index, nullptr);
			
			SET_FLAG(projectile_datum->projectile.flags, Flags::_projectile_datum_flags_attached_bit, true);

			// Finalise the grenade throw
			unit_datum->unit.throwing_grenade_state = Enums::_unit_throwing_grenade_state_released;
		}

		void c_unit_seat_damage_manager::ApplyMeleeDamageToUnit(const datum_index unit_index
			, const datum_index target_unit
			, const TagGroups::unit_seat_damage& seat_damage
			, const int16 region_index) const
		{
			auto* unit_datum = blam::object_get_and_verify_type<s_unit_datum>(unit_index);
			auto* target_unit_datum = blam::object_get_and_verify_type<s_unit_datum>(target_unit);

			// Get the instigators weapon
			datum_index weapon_index = blam::unit_inventory_get_weapon(unit_index, unit_datum->unit.current_weapon_index);

			bool use_weapon_melee = TEST_FLAG(seat_damage.flags, Flags::_unit_seat_damage_flags_use_weapon_melee_bit);
			if(use_weapon_melee && weapon_index.IsNull())
			{
				return;
			}

			datum_index damage_effect = datum_index::null;
			if((region_index != NULL) && !seat_damage.region_damage_effect.tag_index.IsNull())
			{
				damage_effect = seat_damage.region_damage_effect.tag_index;
			}
			else if(use_weapon_melee)
			{
				// Get the melee damage effect
				auto* weapon_datum = blam::object_get_and_verify_type<s_weapon_datum>(weapon_index);
				auto* weapon_definition = blam::tag_get<TagGroups::s_weapon_definition>(weapon_datum->object.definition_index);

				if(!weapon_definition->weapon.flags.prevents_melee_attack_bit)
				{
					damage_effect = weapon_definition->weapon.player_melee_damage.tag_index;
				}
			}
			else
			{
				damage_effect = seat_damage.melee_damage_effect.tag_index;
			}

			if(damage_effect.IsNull())
			{
				return;
			}

			// Damage the target unit
			s_damage_data damage_data;
			blam::damage_data_new(damage_data, damage_effect);

			damage_data.responsible_player_index = unit_datum->unit.controlling_player_index;
			damage_data.responsible_unit_index = unit_index;
			damage_data.responsible_units_team = unit_datum->object.owner_team;
			damage_data.location = target_unit_datum->object.location;
			damage_data.damage_position = target_unit_datum->object.position;

			SET_FLAG(damage_data.flags, Flags::_damage_data_flags_affect_target_only_bit, true);

			blam::object_cause_damage(damage_data, target_unit, NONE, region_index);
		}

		void c_unit_seat_damage_manager::UnitCauseSeatedPlayerMelee(const datum_index unit_index) const
		{
			auto* unit_datum = blam::object_get_and_verify_type<s_unit_datum>(unit_index);
			auto* parent_unit_datum = blam::object_get_and_verify_type<s_unit_datum>(unit_datum->object.parent_object_index);
			auto* seat_extension_definition = GetSeatExtensionDefinition(unit_datum->object.parent_object_index, unit_datum->unit.vehicle_seat_index);

			if(seat_extension_definition && (seat_extension_definition->seat_damage.Count != 0))
			{
				auto& seat_damage_definition = seat_extension_definition->seat_damage[0];

				// If a region is defined melee should target it first
				if(seat_damage_definition.region_index != NONE)
				{
					if(!TEST_FLAG(parent_unit_datum->object.regions_destroyed_flags, seat_damage_definition.region_index))
					{
						ApplyMeleeDamageToUnit(unit_index, unit_datum->object.parent_object_index, seat_damage_definition, seat_damage_definition.region_index);
						return;
					}
				}

				switch(seat_damage_definition.melee)
				{
				case Enums::_unit_seat_damage_melee_normal:
					// Revert to the stock melee damage function
					blam::unit_cause_player_melee_damage(unit_index);
					break;
				case Enums::_unit_seat_damage_melee_mounted_unit:
					// Melee attack the parent unit
					ApplyMeleeDamageToUnit(unit_index, unit_datum->object.parent_object_index, seat_damage_definition);
					break;
				case Enums::_unit_seat_damage_melee_target_seat_unit:
					{
						// Melee attack the unit in the target seat
						auto seated_unit = GetUnitInSeat(unit_datum->object.parent_object_index, seat_extension_definition->target_seat_index);
						if(!seated_unit.IsNull())
						{
							ApplyMeleeDamageToUnit(unit_index, seated_unit, seat_damage_definition);
						}
					}
					break;
				}
			}
			else
			{
				blam::unit_cause_player_melee_damage(unit_index);
			}
		}

		void c_unit_seat_damage_manager::UnitThrowSeatedPlayerGrenade(const datum_index unit_index) const
		{
			auto* unit_datum = blam::object_get_and_verify_type<s_unit_datum>(unit_index);
			auto* seat_extension_definition = GetSeatExtensionDefinition(unit_datum->object.parent_object_index, unit_datum->unit.vehicle_seat_index);

			// If the grenade state is not "in hand", the projectile index is null or the seat has no extensions revert to the stock function
			if(!seat_extension_definition
				|| (unit_datum->unit.throwing_grenade_state != Enums::_unit_throwing_grenade_state_in_hand)
				|| (unit_datum->unit.throwing_grenade_projectile_index.IsNull())
				|| (seat_extension_definition->seat_damage.Count != 1))
			{
				blam::unit_throw_grenade_release(unit_index, 0);
				return;
			}

			auto& seat_damage_definition = seat_extension_definition->seat_damage[0];

			datum_index target_unit = datum_index::null;
			switch(seat_damage_definition.grenade)
			{
			case Enums::_unit_seat_damage_grenade_normal:
				// Revert to the stock function
				blam::unit_throw_grenade_release(unit_index, 0);
				return;
			case Enums::_unit_seat_damage_grenade_plant_on_mounted_unit:
				// Plant the grenade onto the parent unit
				target_unit = unit_datum->object.parent_object_index;
				break;
			case Enums::_unit_seat_damage_grenade_plant_on_target_seat_unit:
				// Plant the grenade onto the unit in the target seat
				target_unit = GetUnitInSeat(unit_datum->object.parent_object_index, seat_extension_definition->target_seat_index);
				break;
			};

			if(!target_unit.IsNull())
			{
				AttachGrenadeToUnit(target_unit
					, unit_datum
					, unit_datum->unit.throwing_grenade_projectile_index
					, seat_damage_definition);

				// If the player/unit should leave the seat after planting a grenade, do so
				if(TEST_FLAG(seat_damage_definition.flags, Flags::_unit_seat_damage_flags_exit_after_grenade_plant_bit))
				{
					// The player should gracefully exit the seat, but will only do so if it isn't mid animation
					// Force the unit to it's idle animation state so that it can then exit the seat
					blam::unit_animation_set_state(unit_index, Enums::_unit_animation_state_idle);

#if PLATFORM_IS_EDITOR
					blam::unit_try_and_exit_seat(unit_index, true);
#else
					blam::unit_try_and_exit_seat(unit_index);
#endif
				}
			}
		}

		bool c_unit_seat_damage_manager::PreventSeatedPlayerGrenade(const datum_index unit_index, const datum_index weapon_index) const
		{
			// If the current animation is not interruptable, skip the grenade throw
			auto* unit_datum = blam::object_get_and_verify_type<s_unit_datum>(unit_index);
			if(!blam::unit_animation_state_interruptable(unit_datum->unit.animation, Enums::_unit_animation_state_throw_grenade))
			{
				return true;
			}
			
			auto* parent_unit_datum = blam::object_get_and_verify_type<s_unit_datum>(unit_datum->object.parent_object_index);
			auto* seat_extension_definition = GetSeatExtensionDefinition(unit_datum->object.parent_object_index, unit_datum->unit.vehicle_seat_index);
			if(seat_extension_definition && (seat_extension_definition->seat_damage.Count != 0))
			{
				auto& seat_damage_definition = seat_extension_definition->seat_damage[0];

				// If a region is targeted but not destroyed, and grenades are disabled until it is destroyed, return true
				if(seat_damage_definition.region_index != NONE)
				{
					if(!TEST_FLAG(parent_unit_datum->object.regions_destroyed_flags, seat_damage_definition.region_index)
						&& TEST_FLAG(seat_damage_definition.region_flags, Flags::_unit_seat_region_damage_flags_disable_grenades_until_destroyed_bit))
					{
						return true;
					}
				}

				if((unit_datum->unit.current_grenade_index == NONE) || (unit_datum->unit.current_grenade_index >= Enums::k_unit_grenade_types_count_yelo))
				{
					return true;
				}

				if(TEST_FLAG(seat_damage_definition.disabled_grenade_types, unit_datum->unit.current_grenade_index))
				{
					return true;
				}

				switch(seat_damage_definition.grenade)
				{
				case Enums::_unit_seat_damage_grenade_normal:
					break;
				case Enums::_unit_seat_damage_grenade_plant_on_mounted_unit:
				case Enums::_unit_seat_damage_grenade_plant_on_target_seat_unit:
					return false;
				case Enums::_unit_seat_damage_grenade_disabled:
					return true;
				};
			}

			return blam::weapon_prevents_grenade_throwing(weapon_index);
		}
	};};};
};