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
			if(StartAnimation(unit_index, unit_datum, Enums::_unit_seat_animation_yelo_ejection))
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

		void c_unit_seat_boarding_manager::DoKeyframeAction(const datum_index unit_index, const TagGroups::unit_seat_keyframe_action& action) const
		{
			auto& unit_datum = *blam::object_get_and_verify_type<s_unit_datum>(unit_index);
			auto& seat_extension_definition = *GetSeatExtensionDefinition(unit_datum.object.parent_object_index, unit_datum.unit.vehicle_seat_index);
			auto target_seat_unit = GetUnitInSeat(unit_datum.object.parent_object_index, seat_extension_definition.target_seat_index);
			
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

		void c_unit_seat_boarding_manager::UnitUpdate(const datum_index unit_index) const
		{
			auto* unit_datum = blam::object_get_and_verify_type<s_unit_datum>(unit_index);
			if((unit_datum->object.parent_object_index.IsNull()) || (unit_datum->unit.vehicle_seat_index == NONE))
			{
				return;
			}
			auto* parent_unit_datum = blam::object_get_and_verify_type<s_unit_datum>(unit_datum->object.parent_object_index);

			auto* seat_extension_definition = GetSeatExtensionDefinition(unit_datum->object.parent_object_index, unit_datum->unit.vehicle_seat_index);
			if(!seat_extension_definition || (seat_extension_definition->seat_boarding.Count != 1))
			{
				return;
			}

			if(TEST_FLAG(parent_unit_datum->object.damage.flags, Flags::_object_body_depleted_bit)
				&& TEST_FLAG(seat_extension_definition->flags, Flags::_unit_seat_extensions_flags_exit_on_unit_death_bit))
			{
				blam::unit_exit_seat_end(unit_index, false, true, false);
				return;
			}

			// Start the boarding state
			auto& seat_boarding = seat_extension_definition->seat_boarding[0];
			if(blam::unit_animation_state_interruptable(unit_datum->unit.animation, Enums::_unit_animation_state_yelo_seat_board))
			{
				if(seat_boarding.boarding_type == Enums::_unit_seat_boarding_type_immediate)
				{
					// If boarding should happen immediately, start the animation
					if(StartAnimation(unit_index, unit_datum, Enums::_unit_seat_animation_yelo_board))
					{
						unit_datum->unit.animation.state = Enums::_unit_animation_state_yelo_seat_board;
					}
				}
				else
				{
					// If the boarding is delayed, wait for the specified criteria
					// Start boarding if the target seat is empty
					bool trigger = true;
					if(TEST_FLAG(seat_boarding.delay_until, Flags::_unit_seat_boarding_delay_until_flags_empty_target_seat_bit))
					{
						trigger &= GetUnitInSeat(unit_datum->object.parent_object_index, seat_extension_definition->target_seat_index).IsNull();
					}

					// Start boarding if the unit's shield and health are below the required threshold
					datum_index target_unit_index = unit_datum->object.parent_object_index;
					if(seat_boarding.unit_vitality_source == Enums::_unit_seat_boarding_vitality_threshold_source_seated_unit)
					{
						target_unit_index = GetUnitInSeat(unit_datum->object.parent_object_index, seat_extension_definition->target_seat_index);
					}

					auto* target_unit_datum = blam::object_try_and_get_and_verify_type<s_unit_datum>(target_unit_index);
					if(target_unit_datum)
					{
						if(TEST_FLAG(seat_boarding.delay_until, Flags::_unit_seat_boarding_delay_until_flags_unit_shield_threshold_bit))
						{
							trigger &= target_unit_datum->object.damage.shield <= seat_boarding.unit_shield_threshold;
						}
						if(TEST_FLAG(seat_boarding.delay_until, Flags::_unit_seat_boarding_delay_until_flags_unit_health_threshold_bit))
						{
							trigger &= target_unit_datum->object.damage.health <= seat_boarding.unit_health_threshold;
						}
					}
					
					// Start the animation
					if(trigger)
					{
						if(StartAnimation(unit_index, unit_datum, Enums::_unit_seat_animation_yelo_board))
						{
							unit_datum->unit.animation.state = Enums::_unit_animation_state_yelo_seat_board;
						}
					}
				}
			}
		}

		void c_unit_seat_boarding_manager::UnitTriggerBoardPrimaryKeyframe(const datum_index unit_index) const
		{
			auto& unit_datum = *blam::object_get_and_verify_type<s_unit_datum>(unit_index);
			auto& seat_extension_definition = *GetSeatExtensionDefinition(unit_datum.object.parent_object_index, unit_datum.unit.vehicle_seat_index);

			if(seat_extension_definition.seat_boarding.Count == 1)
			{
				DoKeyframeAction(unit_index, seat_extension_definition.seat_boarding[0].primary_keyframe_action);
			}
		}

		void c_unit_seat_boarding_manager::UnitTriggerBoardFinalKeyframe(const datum_index unit_index) const
		{
			auto& unit_datum = *blam::object_get_and_verify_type<s_unit_datum>(unit_index);
			auto& seat_extension_definition = *GetSeatExtensionDefinition(unit_datum.object.parent_object_index, unit_datum.unit.vehicle_seat_index);

			if(seat_extension_definition.seat_boarding.Count == 1)
			{
				DoKeyframeAction(unit_index, seat_extension_definition.seat_boarding[0].final_keyframe_action);
			}
		}

		void c_unit_seat_boarding_manager::UnitTriggerEjectionPrimaryKeyframe(const datum_index unit_index) const { }

		void c_unit_seat_boarding_manager::UnitTriggerEjectionFinalKeyframe(const datum_index unit_index) const { }
		
		void c_unit_seat_boarding_manager::UnitCanEnterBoardingSeat(const datum_index unit_index
			, const datum_index target_unit_index
			, const int16 target_seat_index
			, bool& result) const
		{
			// Can't enter if there is already a unit in the seat
			if(!GetUnitInSeat(unit_index, target_seat_index).IsNull())
			{
				result = false;
				return;
			}

			// If the seat has no extensions, pass through the result
			auto* unit_datum = blam::object_get_and_verify_type<s_unit_datum>(unit_index);
			auto* unit_definition = blam::tag_get<TagGroups::s_unit_definition>(unit_datum->object.definition_index);

			auto* seat_extension = GetSeatExtensionDefinition(target_unit_index, target_seat_index);
			if(!seat_extension)
			{
				return;
			}

			if(seat_extension->target_seat_index != NONE)
			{
				// Handle criteria for entering a seat that target's another seat

				// If a seat is targeted it must have an extension block
				auto* target_seat_extension = GetSeatExtensionDefinition(target_unit_index, seat_extension->target_seat_index);
				if(!target_seat_extension)
				{
					result = false;
					return;
				}

				// Only allow entry if the target seat is occupied
				datum_index target_seat_unit = GetUnitInSeat(target_unit_index, seat_extension->target_seat_index);
				if(TEST_FLAG(seat_extension->flags, Flags::_unit_seat_extensions_flags_requires_target_seat_occupied_bit) && target_seat_unit.IsNull())
				{
					result = false;
					return;
				}
			
				// Only allow entry if the target seat unit is your enemy
				if(!target_seat_unit.IsNull())
				{
					auto* target_seat_unit_datum = blam::object_get_and_verify_type<s_unit_datum>(target_seat_unit);
					if(!blam::game_team_is_enemy(unit_datum->object.owner_team, target_seat_unit_datum->object.owner_team))
					{
						result = false;
						return;
					}
				}

				// Only allow entry if no other seat targeting the target seat is occupied
				for(auto& seat_index : target_seat_extension->seat_targeting_seats)
				{
					if(!GetUnitInSeat(target_unit_index, seat_index).IsNull())
					{
						result = false;
						return;
					}
				}
			}

			// Return false the entering unit is within the target unit's cone of vision
			auto* target_unit_datum = blam::object_get_and_verify_type<s_unit_datum>(target_unit_index);
			if(TEST_FLAG(seat_extension->flags, Flags::_unit_seat_extensions_flags_restrict_by_unit_sight_bit))
			{
				if(blam::unit_can_see_point(target_unit_index, &unit_datum->object.position, seat_extension->unit_sight_angle))
				{
					result = false;
					return;
				}
			}
			
			// Return false the target unit's shield is above the threshold
			if(TEST_FLAG(seat_extension->flags, Flags::_unit_seat_extensions_flags_restrict_by_unit_shield_bit))
			{
				if(target_unit_datum->object.damage.shield > seat_extension->unit_shield_threshold)
				{
					result = false;
					return;
				}
			}
			
			// Return false the target unit's health is above the threshold
			if(TEST_FLAG(seat_extension->flags, Flags::_unit_seat_extensions_flags_restrict_by_unit_health_bit))
			{
				if(target_unit_datum->object.damage.health > seat_extension->unit_health_threshold)
				{
					result = false;
					return;
				}
			}
			
			// Return false the target unit's AI is in a non-permitted state
			if(TEST_FLAG(seat_extension->flags, Flags::_unit_seat_extensions_flags_restrict_by_ai_state_bit))
			{
				if(!target_unit_datum->unit.actor_index.IsNull())
				{
					auto* actor_datum = AI::Actors()[target_unit_datum->unit.actor_index];
					if(actor_datum)
					{
						
					}
				}
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

			result &= true;
		}

		void c_unit_seat_boarding_manager::UnitCanEnterTargetSeat(const datum_index unit_index
			, const datum_index target_unit_index
			, const int16 target_seat_index
			, bool& result) const
		{
			// Handle the seat enter case for when the unit is boarding
			auto* unit_datum = blam::object_get_and_verify_type<s_unit_datum>(unit_index);
			if(unit_datum && (unit_datum->unit.animation.state != Enums::_unit_animation_state_yelo_seat_board))
			{
				return;
			}

			// Can only enter the target seat if it is empty
			result = GetUnitInSeat(target_unit_index, target_seat_index).IsNull();
		}
	};};};
};