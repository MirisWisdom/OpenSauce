/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include <YeloLib/Halo1/units/c_unit_seat_boarding_manager.hpp>

#include <blamlib/tag_files/tag_groups_base.hpp>
#include <blamlib/Halo1/ai/actor_structures.hpp>
#include <blamlib/Halo1/objects/objects.hpp>
#include <blamlib/Halo1/objects/damage.hpp>
#include <blamlib/Halo1/units/units.hpp>
#include <blamlib/Halo1/units/unit_definitions.hpp>
#include <blamlib/Halo1/hs/hs_library_external.hpp>
#include <blamlib/Halo1/models/model_animation_definitions.hpp>
#include <blamlib/Halo1/game/players.hpp>
#include <blamlib/Halo1/game/player_structures.hpp>

#include <YeloLib/Halo1/units/unit_definitions_yelo.hpp>
#include <YeloLib/Halo1/units/units_yelo.hpp>

namespace Yelo
{
	namespace Objects { namespace Units { namespace SeatBoarding
	{
		void c_unit_seat_boarding_manager::UnitApplyEffect(const datum_index unit_index
			, const datum_index effect
			, const tag_string& effect_marker) const
		{
			// Spawn the effect
			blam::hs_effect_new_from_object_marker(effect, unit_index, effect_marker);
		}
		
		void c_unit_seat_boarding_manager::UnitApplyDamage(const datum_index unit_index
			, const datum_index target_unit_index
			, const datum_index damage_effect) const
		{
			if(damage_effect.IsNull())
			{
				return;
			}

			auto& unit_datum = *blam::object_get_and_verify_type<s_unit_datum>(unit_index);
			auto& target_unit_datum = *blam::object_get_and_verify_type<s_unit_datum>(target_unit_index);

			s_damage_data damage_data;

			// Create a new damage_data struct and populate it
			blam::damage_data_new(damage_data, damage_effect);
			damage_data.responsible_player_index = unit_datum.unit.controlling_player_index;
			damage_data.responsible_unit_index = unit_index;
			damage_data.responsible_units_team = unit_datum.object.owner_team;
			damage_data.location = target_unit_datum.object.location;
			damage_data.damage_position = target_unit_datum.object.position;
			damage_data.damage_multiplier = 1.0f;

			SET_FLAG(damage_data.flags, Flags::_damage_data_flags_affect_target_only_bit, true);

			// Damage the target unit
			blam::object_cause_damage(damage_data, target_unit_index);
		}
		
		void c_unit_seat_boarding_manager::UnitExitFromSeat(const datum_index unit_index) const
		{
			auto* unit_datum = blam::object_get_and_verify_type<s_unit_datum>(unit_index);

			// Try to get the unit to gracefully exit the seat
			// If that fails, kick it out
			if(!blam::unit_try_and_exit_seat(unit_index))
			{
				blam::unit_exit_seat_end(unit_index, false, true, false);
			}
		}

		void c_unit_seat_boarding_manager::UnitEjectFromSeat(const datum_index unit_index) const
		{
			auto* unit_datum = blam::object_get_and_verify_type<s_unit_datum>(unit_index);

			// Try to play the ejection animation on the target unit
			// If that fails try to get the unit to gracefully exit the seat
			if(StartAnimation(unit_index, unit_datum, Enums::_unit_seat_animation_yelo_ejecting))
			{
				unit_datum->unit.animation.state = Enums::_unit_animation_state_seat_exit;
			}
			else
			{
				UnitExitFromSeat(unit_index);
			}
		}

		void c_unit_seat_boarding_manager::UnitEnterPoweredSeat(const datum_index unit_index
			, const datum_index target_unit_index
			, const int16 target_seat) const
		{
			auto* target_unit_datum = blam::object_get_and_verify_type<s_unit_datum>(unit_index);
			auto& seat_definition = *GetSeatDefinition(target_unit_index, target_seat);
			
			if(TEST_FLAG(seat_definition.flags, Flags::_unit_seat_driver_bit))
			{
				target_unit_datum->unit.powered_seats_riders[Enums::_powered_seat_driver] = unit_index;
				target_unit_datum->unit.powered_seats_power[Enums::_powered_seat_driver] = 1.0f;
			}

			if(TEST_FLAG(seat_definition.flags, Flags::_unit_seat_gunner_bit))
			{
				target_unit_datum->unit.powered_seats_riders[Enums::_powered_seat_gunner] = unit_index;
				target_unit_datum->unit.powered_seats_power[Enums::_powered_seat_gunner] = 1.0f;
			}
		}

		bool c_unit_seat_boarding_manager::StartAnimation(const datum_index unit_index
			, const s_unit_datum* unit_datum
			, const Enums::unit_seat_animation animation) const
		{
			// Get the animations for the unit's seat
			auto* animation_graph = GetObjectAnimations(unit_index);
			if (!animation_graph)
			{
				return false;
			}

			if(unit_datum->unit.animation.seat_index >= animation_graph->units.Count)
			{
				return false;
			}
			auto& animation_seat = animation_graph->units[unit_datum->unit.animation.seat_index];

			// Check if the target unit has a matching animation to play
			if (animation >= animation_seat.animations.Count)
			{
				return false;
			}

			int32 animation_index = animation_seat.animations[animation];
			if(animation_index == NONE)
			{
				return false;
			}

			// Get a random permutation of the animation
			animation_index = blam::animation_choose_random_permutation_internal(Enums::_animation_update_kind_affects_game_state
				, unit_datum->object.animation.definition_index
				, animation_index);

			// Start the animation
			blam::object_start_interpolation(unit_index, 6);
			blam::unit_set_animation(unit_index
				, unit_datum->object.animation.definition_index
				, animation_index);

			return true;
		}

		void c_unit_seat_boarding_manager::DoBoardingKeyframeAction(const datum_index unit_index, const TagGroups::unit_seat_keyframe_action& action) const
		{
			auto& unit_datum = *blam::object_get_and_verify_type<s_unit_datum>(unit_index);
			auto& seat_extension_definition = *GetSeatExtensionDefinition(unit_datum.object.parent_object_index, unit_datum.unit.vehicle_seat_index);
			auto target_seat_unit = GetUnitInSeat(unit_datum.object.parent_object_index, seat_extension_definition.target_seat_index);

			// Open/Close the unit as required
			switch(action.unit_door_action)
			{
			case Enums::_unit_seat_keyframe_action_unit_door_action_open:
				blam::unit_open(unit_datum.object.parent_object_index);
				break;
			case Enums::_unit_seat_keyframe_action_unit_door_action_close:
				blam::unit_close(unit_datum.object.parent_object_index);
				break;
			}

			// Spawn a custom effect on a mounted or seated unit's marker
			switch(action.apply_effect_to)
			{
			case Enums::_unit_seat_keyframe_action_apply_effect_mounted_unit:
				UnitApplyEffect(unit_datum.object.parent_object_index, action.effect.tag_index, action.effect_marker);
				break;
			case Enums::_unit_seat_keyframe_action_apply_effect_seated_unit:
				if(!target_seat_unit.IsNull())
				{
					UnitApplyEffect(target_seat_unit, action.effect.tag_index, action.effect_marker);
				}
				break;
			}
			
			// Apply a damage effect on a mounted or seated unit
			switch(action.apply_damage_to)
			{
			case Enums::_unit_seat_keyframe_action_apply_effect_mounted_unit:
				UnitApplyDamage(unit_index, unit_datum.object.parent_object_index, action.damage_effect.tag_index);
				break;
			case Enums::_unit_seat_keyframe_action_apply_effect_seated_unit:
				if(!target_seat_unit.IsNull())
				{
					UnitApplyDamage(unit_index, target_seat_unit, action.damage_effect.tag_index);
				}
				break;
			}

			// Make the target seat unit exit its seat or be ejected
			if(!target_seat_unit.IsNull())
			{
				switch(action.target_seat_unit_action)
				{
				case Enums::_unit_seat_keyframe_action_target_seat_unit_action_exit_seat:
					UnitExitFromSeat(target_seat_unit);
					break;
				case Enums::_unit_seat_keyframe_action_target_seat_unit_action_eject_from_seat:
					UnitEjectFromSeat(target_seat_unit);
					break;
				}
			}

			// Make the boarding unit exit it's current seat or enter the target seat
			switch(action.self_seat_action)
			{
			case Enums::_unit_seat_keyframe_action_self_seat_action_exit_current:
				UnitExitFromSeat(unit_index);
				break;
			case Enums::_unit_seat_keyframe_action_self_seat_action_enter_target:
				// If the target seat is manned when we need to enter, kick the existing unit
				if(!target_seat_unit.IsNull())
				{
					UnitExitFromSeat(target_seat_unit);
				}
				blam::unit_enter_seat(unit_index, unit_datum.object.parent_object_index, seat_extension_definition.target_seat_index);
				break;
			}

			// If the seat is powered, take over control of the seat
			if(TEST_FLAG(action.flags, Flags::_unit_seat_keyframe_action_flags_control_powered_seat_bit))
			{
				UnitEnterPoweredSeat(unit_index, unit_datum.object.parent_object_index, seat_extension_definition.target_seat_index);
			}
		}

		void c_unit_seat_boarding_manager::DoMountedKeyframeAction(const datum_index unit_index, const TagGroups::unit_mounted_state_keyframe_action& action) const
		{
			auto& unit_datum = *blam::object_get_and_verify_type<s_unit_datum>(unit_index);
			auto& unit_definition = *blam::tag_get<TagGroups::s_unit_definition>(unit_datum.object.definition_index);

			// Apply actions to all mounted units
			for(int16 index = 0; index < unit_definition.unit.seats.Count; index++)
			{
				auto* seat_extension = GetSeatExtensionDefinition(unit_index, index);
				if(!seat_extension)
				{
					continue;
				}

				if(!TEST_FLAG(seat_extension->flags, Flags::_unit_seat_extensions_flags_triggers_mounted_state_bit)
					&& (action.target == Enums::_unit_seat_mounted_keyframe_action_target_mounted_trigger_seats))
				{
					continue;
				}

				auto seated_unit_index = GetUnitInSeat(unit_index, index);
				if(seated_unit_index.IsNull())
				{
					continue;
				}

				// Eject the rider
				if(TEST_FLAG(action.flags, Flags::_unit_mounted_state_keyframe_action_flags_eject_mounted_units))
				{
					UnitEjectFromSeat(seated_unit_index);
				}

				// Apply damage to the mounted unit
				if(!action.damage_effect.tag_index.IsNull())
				{
					UnitApplyDamage(unit_index, seated_unit_index, action.damage_effect.tag_index);
				}

				// Spawn an effect on the mounted unit
				if(!action.effect.tag_index.IsNull())
				{
					UnitApplyEffect(seated_unit_index, action.effect.tag_index, action.effect_marker);
				}
			}
		}

		void c_unit_seat_boarding_manager::UpdateNonSeatedUnit(const datum_index unit_index) const
		{
			auto& unit_datum = *blam::object_get_and_verify_type<s_unit_datum>(unit_index);
			auto& unit_definition = *blam::tag_get<TagGroups::s_unit_definition>(unit_datum.object.definition_index);

			if(TEST_FLAG(unit_datum.object.damage.flags, Flags::_object_body_depleted_bit))
			{
				return;
			}

			// If a seat that triggers the mount seat is occupied, start the mounted state
			for(int16 index = 0; index < unit_definition.unit.seats.Count; index++)
			{
				auto* seat_extension = GetSeatExtensionDefinition(unit_index, index);
				if(!seat_extension || GetUnitInSeat(unit_index, index).IsNull())
				{
					continue;
				}

				if(TEST_FLAG(seat_extension->flags, Flags::_unit_seat_extensions_flags_triggers_mounted_state_bit))
				{
					if(blam::unit_animation_state_interruptable(unit_datum.unit.animation, Enums::_unit_animation_state_yelo_unit_mounted))
					{
						if(StartAnimation(unit_index, &unit_datum, Enums::_unit_seat_animation_yelo_mounted))
						{
							unit_datum.unit.animation.state = Enums::_unit_animation_state_yelo_unit_mounted;
						}
					}
				}
			}
		}

		void c_unit_seat_boarding_manager::UpdateSeatedUnit(const datum_index unit_index) const
		{
			auto& unit_datum = *blam::object_get_and_verify_type<s_unit_datum>(unit_index);
			auto& parent_unit_datum = *blam::object_get_and_verify_type<s_unit_datum>(unit_datum.object.parent_object_index);

			auto* seat_extension_definition = GetSeatExtensionDefinition(unit_datum.object.parent_object_index, unit_datum.unit.vehicle_seat_index);
			if(!seat_extension_definition)
			{
				return;
			}

			if(TEST_FLAG(seat_extension_definition->flags, Flags::_unit_seat_extensions_flags_exit_on_unit_death_bit)
				&& TEST_FLAG(parent_unit_datum.object.damage.flags, Flags::_object_body_depleted_bit))
			{
				unit_datum.unit.animation.state = Enums::_unit_animation_state_idle;
				blam::unit_exit_seat_end(unit_index, false, true, false);
				return;
			}

			if(seat_extension_definition->target_seat_index != NONE)
			{
				if(TEST_FLAG(seat_extension_definition->flags, Flags::_unit_seat_extensions_flags_exit_on_target_seat_empty_bit)
					&& GetUnitInSeat(unit_datum.object.parent_object_index, seat_extension_definition->target_seat_index).IsNull())
				{
					UnitExitFromSeat(unit_index);
					return;
				}
			}

			if(seat_extension_definition->seat_boarding.Count != 1)
			{
				return;
			}

			auto& seat_boarding = seat_extension_definition->seat_boarding[0];
			if(!blam::unit_animation_state_interruptable(unit_datum.unit.animation, Enums::_unit_animation_state_yelo_seat_boarding))
			{
				return;
			}

			bool start_boarding = true;
			if(seat_boarding.boarding_type == Enums::_unit_seat_boarding_type_delayed)
			{
				// If the boarding is delayed, wait for the specified criteria
				// Start boarding if the target seat is empty
				if(TEST_FLAG(seat_boarding.delay_until, Flags::_unit_seat_boarding_delay_until_flags_empty_target_seat_bit))
				{
					start_boarding &= GetUnitInSeat(unit_datum.object.parent_object_index, seat_extension_definition->target_seat_index).IsNull();
				}

				// Start boarding if the unit's shield and health are below the required threshold
				datum_index target_unit_index = unit_datum.object.parent_object_index;
				if(seat_boarding.unit_vitality_source == Enums::_unit_seat_boarding_vitality_threshold_source_seated_unit)
				{
					target_unit_index = GetUnitInSeat(unit_datum.object.parent_object_index, seat_extension_definition->target_seat_index);
				}

				auto* target_unit_datum = blam::object_try_and_get_and_verify_type<s_unit_datum>(target_unit_index);
				if(target_unit_datum)
				{
					if(TEST_FLAG(seat_boarding.delay_until, Flags::_unit_seat_boarding_delay_until_flags_unit_shield_threshold_bit))
					{
						start_boarding &= target_unit_datum->object.damage.shield <= seat_boarding.unit_shield_threshold;
					}
					if(TEST_FLAG(seat_boarding.delay_until, Flags::_unit_seat_boarding_delay_until_flags_unit_health_threshold_bit))
					{
						start_boarding &= target_unit_datum->object.damage.health <= seat_boarding.unit_health_threshold;
					}
				}
			}

			// Start the animation
			if(start_boarding)
			{
				if(StartAnimation(unit_index, &unit_datum, Enums::_unit_seat_animation_yelo_boarding))
				{
					unit_datum.unit.animation.state = Enums::_unit_animation_state_yelo_seat_boarding;
				}
			}
		}

		void c_unit_seat_boarding_manager::UpdateBoardingUnit(const datum_index unit_index) const
		{
			auto& unit_datum = *blam::object_get_and_verify_type<s_unit_datum>(unit_index);
			auto& parent_unit_datum = *blam::object_get_and_verify_type<s_unit_datum>(unit_datum.object.parent_object_index);

			auto& seat_extension_definition = *GetSeatExtensionDefinition(unit_datum.object.parent_object_index, unit_datum.unit.vehicle_seat_index);

			if(TEST_FLAG(parent_unit_datum.object.damage.flags, Flags::_object_body_depleted_bit)
				&& TEST_FLAG(seat_extension_definition.flags, Flags::_unit_seat_extensions_flags_exit_on_unit_death_bit))
			{
				unit_datum.unit.animation.state = Enums::_unit_animation_state_idle;
				blam::unit_exit_seat_end(unit_index, false, true, false);
				return;
			}
		}

		void c_unit_seat_boarding_manager::UpdateEjectingUnit(const datum_index unit_index) const
		{

		}
		
		void c_unit_seat_boarding_manager::UpdateMountedUnit(const datum_index unit_index) const
		{
			auto& unit_datum = *blam::object_get_and_verify_type<s_unit_datum>(unit_index);
			auto& unit_definition = *blam::tag_get<TagGroups::s_unit_definition>(unit_datum.object.definition_index);

			// If no seats that trigger the mounting state are occupied, clear the state
			bool is_mounted = false;
			for(int16 index = 0; index < unit_definition.unit.seats.Count; index++)
			{
				auto* seat_extension = GetSeatExtensionDefinition(unit_index, index);
				if(!seat_extension)
				{
					continue;
				}

				if(TEST_FLAG(seat_extension->flags, Flags::_unit_seat_extensions_flags_triggers_mounted_state_bit)
					&& !GetUnitInSeat(unit_index, index).IsNull())
				{
					is_mounted = true;
					break;
				}
			}

			// If no longer mounted, change animation state
			if(!is_mounted)
			{
				blam::unit_animation_set_state(unit_index, Enums::_unit_animation_state_idle);
				SET_FLAG(unit_datum.unit.flags, Flags::_unit_controllable_bit, true);
			}
			else
			{
				SET_FLAG(unit_datum.unit.flags, Flags::_unit_controllable_bit, false);
			}
		}

		void c_unit_seat_boarding_manager::UnitUpdate(const datum_index unit_index) const
		{
			auto& unit_datum = *blam::object_get_and_verify_type<s_unit_datum>(unit_index);
			switch(unit_datum.unit.animation.state)
			{
			case Enums::_unit_animation_state_yelo_seat_boarding:
				UpdateBoardingUnit(unit_index);
				return;
			case Enums::_unit_animation_state_yelo_seat_ejecting:
				UpdateEjectingUnit(unit_index);
				return;
			case Enums::_unit_animation_state_yelo_unit_mounted:
				UpdateMountedUnit(unit_index);
				return;
			}

			if((unit_datum.object.parent_object_index.IsNull()) || (unit_datum.unit.vehicle_seat_index == NONE))
			{
				UpdateNonSeatedUnit(unit_index);
			}
			else
			{
				UpdateSeatedUnit(unit_index);
			}
		}

		void c_unit_seat_boarding_manager::UnitTriggerBoardingKeyframe(const datum_index unit_index, const Enums::unit_animation_keyframe keyframe) const
		{
			auto& unit_datum = *blam::object_get_and_verify_type<s_unit_datum>(unit_index);
			auto& seat_extension_definition = *GetSeatExtensionDefinition(unit_datum.object.parent_object_index, unit_datum.unit.vehicle_seat_index);

			if(seat_extension_definition.seat_boarding.Count == 1)
			{
				for(auto& keyframe_action : seat_extension_definition.seat_boarding[0].keyframe_actions)
				{
					if(keyframe_action.keyframe == keyframe)
					{
						DoBoardingKeyframeAction(unit_index, keyframe_action);
					}
				}
			}
		}

		void c_unit_seat_boarding_manager::UnitTriggerEjectingKeyframe(const datum_index unit_index, const Enums::unit_animation_keyframe keyframe) const { }

		void c_unit_seat_boarding_manager::UnitTriggerMountedKeyframe(const datum_index unit_index, const Enums::unit_animation_keyframe keyframe) const
		{
			auto& unit_datum = *blam::object_get_and_verify_type<s_unit_datum>(unit_index);
			auto* unit_extension = GetUnitExtensionDefinition(unit_index);

			if(unit_extension && (unit_extension->mounted_state.Count == 1))
			{
				for(auto& keyframe_action : unit_extension->mounted_state[0].keyframe_actions)
				{
					if(keyframe_action.keyframe == keyframe)
					{
						DoMountedKeyframeAction(unit_index, keyframe_action);
					}
				}
			}
		}

		void c_unit_seat_boarding_manager::CanEnterSeat(const datum_index unit_index
			, const datum_index target_unit_index
			, const int16 seat_index
			, const bool is_targeted_seat
			, bool& result) const
		{
			auto& unit_datum = *blam::object_get_and_verify_type<s_unit_datum>(unit_index);

			s_unit_datum* target_unit_datum = nullptr;
			if(is_targeted_seat)
			{
				auto target_index = GetUnitInSeat(target_unit_index, seat_index);
				if(target_index.IsNull())
				{
					return;
				}

				target_unit_datum = blam::object_get_and_verify_type<s_unit_datum>(target_index);
			}
			else
			{
				target_unit_datum = blam::object_get_and_verify_type<s_unit_datum>(target_unit_index);
			}

			auto& seat_extension = *GetSeatExtensionDefinition(target_unit_index, seat_index);
			if(seat_extension.seat_access.Count == 1)
			{
				auto& seat_access_block = seat_extension.seat_access[0];

				// Only allow entry if the target seat unit is your enemy
				if(TEST_FLAG(seat_access_block.flags, Flags::_unit_seat_access_flags_enemy_access_only_bit)
					&& !blam::game_team_is_enemy(unit_datum.object.owner_team, target_unit_datum->object.owner_team))
				{
					result = false;
					return;
				}

				// Return false if the entering unit is within the target unit's cone of vision
				if(TEST_FLAG(seat_access_block.flags, Flags::_unit_seat_access_flags_restrict_by_unit_sight_bit))
				{
					if(blam::unit_can_see_point(target_unit_index, &unit_datum.object.position, seat_access_block.unit_sight_angle))
					{
						result = false;
						return;
					}
				}
			
				//// Return false if the target unit's shield is above the threshold
				if(TEST_FLAG(seat_access_block.flags, Flags::_unit_seat_access_flags_restrict_by_unit_shield_bit))
				{
					if(target_unit_datum->object.damage.shield > seat_access_block.unit_shield_threshold)
					{
						result = false;
						return;
					}
				}
			
				// Return false if the target unit's health is above the threshold
				if(TEST_FLAG(seat_access_block.flags, Flags::_unit_seat_access_flags_restrict_by_unit_health_bit))
				{
					if(target_unit_datum->object.damage.health > seat_access_block.unit_health_threshold)
					{
						result = false;
						return;
					}
				}
			
				// Return false if the target unit's AI is in a non-permitted state
				if(TEST_FLAG(seat_access_block.flags, Flags::_unit_seat_access_flags_restrict_by_ai_state_bit))
				{
					if(!target_unit_datum->unit.actor_index.IsNull())
					{
						auto* actor_datum = AI::Actors()[target_unit_datum->unit.actor_index];
						if(actor_datum && !TEST_FLAG(seat_access_block.permitted_ai_states, actor_datum->current_state))
						{
							result = false;
							return;
						}
					}
				}
			}
		}

		void c_unit_seat_boarding_manager::UnitCanEnterBoardingSeat(const datum_index unit_index
			, const datum_index target_unit_index
			, const int16 target_seat_index
			, bool& result) const
		{
			// Can't enter if there is already a unit in the seat
			if(!GetUnitInSeat(target_unit_index, target_seat_index).IsNull())
			{
				result = false;
				return;
			}

			auto* unit_datum = blam::object_get_and_verify_type<s_unit_datum>(unit_index);

			// Can't enter seats if mounted
			if(unit_datum && (unit_datum->unit.animation.state == Enums::_unit_animation_state_yelo_unit_mounted))
			{
				return;
			}

			auto* unit_definition = blam::tag_get<TagGroups::s_unit_definition>(unit_datum->object.definition_index);
			for(int16 index = 0; index < unit_definition->unit.seats.Count; index++)
			{
				if(!GetUnitInSeat(unit_index, index).IsNull())
				{
					result = false;
					return;
				}
			}

			// If the seat has no extensions, pass through the result
			auto* seat_extension = GetSeatExtensionDefinition(target_unit_index, target_seat_index);
			if(!seat_extension)
			{
				return;
			}

			// Don't allow entry if there is a unit in a seat targeting this one
			for(auto& seat_index : seat_extension->seat_targeting_seats)
			{
				if(!GetUnitInSeat(target_unit_index, seat_index).IsNull())
				{
					result = false;
					return;
				}
			}

			CanEnterSeat(unit_index, target_unit_index, target_seat_index, false, result);

			if(seat_extension->target_seat_index != NONE)
			{
				// If a seat is targeted it must have an extension block
				auto* target_seat_extension = GetSeatExtensionDefinition(target_unit_index, seat_extension->target_seat_index);
				if(!target_seat_extension)
				{
					result = false;
					return;
				}

				// Don't allow entry if there is a unit in a seat targeting the targeted seat
				for(auto& seat_index : target_seat_extension->seat_targeting_seats)
				{
					if(!GetUnitInSeat(target_unit_index, seat_index).IsNull())
					{
						result = false;
						return;
					}
				}
				
				if(seat_extension->seat_access.Count == 1)
				{
					auto& seat_access_block = seat_extension->seat_access[0];
					auto seated_unit = GetUnitInSeat(target_unit_index, seat_extension->target_seat_index);

					// Only allow entry if the seat is occupied
					if(TEST_FLAG(seat_access_block.flags, Flags::_unit_seat_access_flags_requires_target_seat_occupied_bit) && seated_unit.IsNull())
					{
						result = false;
						return;
					}
				}

				CanEnterSeat(unit_index, target_unit_index, seat_extension->target_seat_index, true, result);
			}
		}

		void c_unit_seat_boarding_manager::UnitCanEnterTargetSeat(const datum_index unit_index
			, const datum_index target_unit_index
			, const int16 target_seat_index
			, bool& result) const
		{
			// Handle the seat enter case for when the unit is boarding
			auto* unit_datum = blam::object_get_and_verify_type<s_unit_datum>(unit_index);
			if(unit_datum && (unit_datum->unit.animation.state != Enums::_unit_animation_state_yelo_seat_boarding))
			{
				return;
			}

			// Can only enter the target seat if it is empty
			result = GetUnitInSeat(target_unit_index, target_seat_index).IsNull();
		}
	};};};
};