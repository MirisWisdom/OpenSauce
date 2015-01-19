/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include <YeloLib/Halo1/ai/c_actor_variant_transform_manager.hpp>

#include <blamlib/Halo1/tag_files/tag_groups.hpp>
#include <blamlib/Halo1/objects/objects.hpp>
#include <blamlib/Halo1/objects/damage.hpp>
#include <blamlib/Halo1/units/unit_structures.hpp>
#include <blamlib/Halo1/units/units.hpp>
#include <blamlib/Halo1/ai/ai.hpp>
#include <blamlib/Halo1/ai/actors.hpp>
#include <blamlib/Halo1/ai/actor_structures.hpp>
#include <blamlib/Halo1/ai/actor_definitions.hpp>
#include <blamlib/Halo1/ai/ai_script.hpp>
#include <blamlib/Halo1/ai/ai_structures.hpp>
#include <blamlib/Halo1/effects/damage_effect_definitions.hpp>
#include <blamlib/Halo1/hs/hs_library_external.hpp>
#include <blamlib/Halo1/models/models.hpp>
#include <blamlib/Halo1/models/model_animation_definitions.hpp>

#include <YeloLib/Halo1/units/unit_transform_definition.hpp>
#include <YeloLib/Halo1/objects/objects_yelo.hpp>

namespace Yelo
{
	namespace AI { namespace Transform
	{
		c_actor_variant_transform_manager::c_actor_variant_transform_manager()
			: m_transforms_allowed(true)
			, m_transforms_in_progress()
		{ }

		const TagGroups::actor_variant_transform_collection_transform* c_actor_variant_transform_manager::FindTransform(const TagBlock<TagGroups::actor_variant_transform_collection_transform>& transformations
			, const datum_index& unit_index
			, const datum_index& instigator_unit_index
			, const bool damage_is_melee)
		{
			auto* unit_datum = blam::object_get_and_verify_type<Objects::s_unit_datum>(unit_index);
			auto* instigator_unit_datum = (instigator_unit_index.IsNull() ? nullptr : blam::object_try_and_get_and_verify_type<Objects::s_unit_datum>(instigator_unit_index));

			auto test_transform = [&](const TagGroups::actor_variant_transform_collection_transform& transform) -> bool
			{
				auto transform_out_definition = *transform.transform_out_ptr;

				// If we should ignore friendly fire, do so
				if(instigator_unit_datum)
				{
					if(TEST_FLAG(transform_out_definition.flags, Flags::_actor_variant_transform_out_flags_ignore_friendly_fire_bit)
						&& !blam::game_team_is_enemy(unit_datum->object.owner_team, instigator_unit_datum->object.owner_team))
					{
						return false;
					}
				}

				// Test whether the transform is valid based on the unit's damage
				bool health_below_threshold = unit_datum->object.damage.health <= transform_out_definition.health_threshold;
				bool shield_below_threshold = unit_datum->object.damage.shield <= transform_out_definition.shield_threshold;

				bool valid_transform = false;
				switch(transform_out_definition.threshold_type)
				{
				case Enums::_actor_variant_transform_out_threshold_type_both:
					valid_transform = health_below_threshold && shield_below_threshold;
					break;
				case Enums::_actor_variant_transform_out_threshold_type_shield_amount_only:
					valid_transform = shield_below_threshold;
					break;
				case Enums::_actor_variant_transform_out_threshold_type_health_amount_only:
					valid_transform = health_below_threshold;
					break;
				};

				return valid_transform;
			};

			std::function<bool (const TagGroups::actor_variant_transform_collection_transform&)> select_func;
			if(instigator_unit_datum)
			{
				// If the instigator is not null select entries that match the instigator and damage type's
				select_func = [&](const TagGroups::actor_variant_transform_collection_transform& entry) -> bool
					{
						if(test_transform(entry))
						{
							for(auto& instigator : entry.instigators)
							{
								// Skip if the damage type does not match
								if(((instigator.damage_type == Enums::_actor_variant_transform_collection_damage_type_weapon_only) && damage_is_melee)
									|| ((instigator.damage_type == Enums::_actor_variant_transform_collection_damage_type_melee_only) && !damage_is_melee))
								{
									return false;
								}

								if(instigator.unit.tag_index == instigator_unit_datum->object.definition_index)
								{
									return true;
								}
							}
						}
						return false;
					};
			}
			else
			{
				// If the instigator is not null select entries that have no instigators defined
				select_func = [&](const TagGroups::actor_variant_transform_collection_transform& entry) -> bool
					{
						if((entry.instigators.Count == 0) && test_transform(entry))
						{
							return true;
						}

						return false;
					};
			}
			
			// Populate the transforms list
			std::vector<const TagGroups::actor_variant_transform_collection_transform*> transforms;
			for(auto& entry : transformations)
			{
				if(select_func(entry))
				{
					transforms.push_back(&entry);
				}
			}
			
			// Pick a random transform from the transforms list
			if(transforms.size())
			{
				return transforms[rand() % transforms.size()];
			}

			return nullptr;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// 	Gets a team based on whether it is from the attacked, attacker or an override.
		/// </summary>
		///
		/// <param name="option">			The team handling option. </param>
		/// <param name="attacked_team">	The attacked team. </param>
		/// <param name="attacker_team">	The attacker team. </param>
		/// <param name="override_team">	The override team. </param>
		///
		/// <returns>	The chosen team. </returns>
		static Enums::game_team GetTeam(const Enums::actor_variant_transform_in_team_handling option
			, const Enums::game_team attacked_team
			, const Enums::game_team attacker_team
			, const Enums::game_team override_team)
		{
			switch(option)
			{
			case Enums::_actor_variant_transform_in_team_handling_inherit_from_attacked:
				return attacked_team;
			case Enums::_actor_variant_transform_in_team_handling_inherit_from_attacker:
				return attacker_team;
			case Enums::_actor_variant_transform_in_team_handling_override:
				return override_team;
			}

			return Enums::_game_team_none;
		}

#pragma region Attachments
		void c_actor_variant_transform_manager::DeleteAttachments(const datum_index unit_index, const TagGroups::actor_variant_transform_in_definition& transform_definition)
		{
			for(auto& attachment : transform_definition.attachments)
			{
				Objects::DeleteChildrenByDefinition(unit_index, attachment.object_type.tag_index);
			}
		}

		void c_actor_variant_transform_manager::AttachObject(const datum_index unit_index
			, const Objects::s_unit_datum* unit_datum
			, const datum_index object_type
			, const tag_string& object_marker_name
			, const tag_string& destination_marker_name
			, const int32 destination_marker_index
			, const Enums::game_team attachment_team
			, const real attachment_scale)
		{
			// If there is only 1 marker do not append a marker index to the name
			cstring destination_marker = destination_marker_name;

			tag_string destination_marker_buffer;
			if(destination_marker_index != NONE)
			{
				sprintf_s(destination_marker_buffer, "%s%d", destination_marker_name, destination_marker_index);

				destination_marker = destination_marker_buffer;
			}

			// If the target marker does not exist, skip the attachment
			auto* object_definition = blam::tag_get<TagGroups::s_object_definition>(unit_datum->object.definition_index);
			if(object_definition->object.references.render_model.tag_index.IsNull())
			{
				return;
			}

			if(blam::model_find_marker(object_definition->object.references.render_model.tag_index, destination_marker) == NONE)
			{
				return;
			}

			// Set up the placement data to match the target object's
			Objects::s_object_placement_data attachment_placement_data;
			blam::object_placement_data_new(attachment_placement_data, object_type);

			attachment_placement_data.position = unit_datum->object.position;
			attachment_placement_data.owner_team = attachment_team;

			// Create the object
			datum_index attachment_object;
			attachment_object = blam::object_new(attachment_placement_data);

			// Scale the attachment as required
			auto* attachment_datum = blam::object_get(attachment_object);
			attachment_datum->scale = attachment_scale;

			// Attach the object
			blam::object_attach_to_marker(unit_index, destination_marker_name, attachment_object, object_marker_name);
		}

		void c_actor_variant_transform_manager::AttachObjects(const datum_index unit_index
			, const Objects::s_unit_datum* unit_datum
			, const Enums::game_team instigator_team
			, const TagGroups::actor_variant_transform_in_definition& transform_definition)
		{
			for(auto& attachment : transform_definition.attachments)
			{
				Enums::game_team attachment_team = GetTeam(attachment.team_handling
					, unit_datum->object.owner_team
					, instigator_team
					, attachment.team_override);

				real attachment_scale = attachment.attachment_scale.lower + (((real)rand() / (real)RAND_MAX) * (attachment.attachment_scale.upper - attachment.attachment_scale.lower));

				if(attachment.destination_marker_count == 1)
				{
					AttachObject(unit_index
						, unit_datum
						, attachment.object_type.tag_index
						, attachment.object_marker
						, attachment.destination_marker
						, NONE
						, attachment_team
						, attachment_scale);
				}
				else
				{
					for(int32 marker_index = 0; marker_index < attachment.destination_marker_count; marker_index++)
					{
						AttachObject(unit_index
							, unit_datum
							, attachment.object_type.tag_index
							, attachment.object_marker
							, attachment.destination_marker
							, marker_index
							, attachment_team
							, attachment_scale);
					}
				}
			}
		}
#pragma endregion

#pragma region Transform Sequence
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets up the unit's actor. </summary>
		///
		/// <param name="unit_index">			  	Datum index of the unit. </param>
		/// <param name="actor_variant_tag_index">	Datum index of the actor variant tag. </param>
		/// <param name="default_state">		  	The default actor state. </param>
		/// <param name="source_actor_index">	  	Datum index of the source actor. </param>
		/// <param name="inherit_encounter_squad">
		/// 	Flag for whether to inherit the source actor's encounter and squad.
		/// </param>
		static void SetupUnitActor(const datum_index unit_index
			, const datum_index actor_variant_tag_index
			, const Enums::actor_variant_transform_in_actor_state_handling initial_state_handling
			, const Enums::actor_default_state initial_state_override
			, const Enums::actor_variant_transform_in_actor_state_handling return_state_handling
			, const Enums::actor_default_state return_state_override
			, const datum_index source_actor_index
			, const bool inherit_encounter_squad)
		{
			// Set up the unit's inventory
			blam::actor_customize_unit(actor_variant_tag_index, unit_index);

			// Inherit the encounter and squad if desired
			datum_index encounter_index = datum_index::null;
			int32 squad_index = NONE;

			const auto actor_data = AI::Actors()[source_actor_index];

			if(inherit_encounter_squad)
			{
				encounter_index = actor_data->meta.encounter_index;
				squad_index = actor_data->meta.squad_index;
			}

			Enums::actor_default_state initial_state = Enums::_actor_default_state_none;
			switch(initial_state_handling)
			{
			case Enums::_actor_variant_transform_in_actor_state_handling_inherit:
				initial_state = actor_data->initial_state;
				break;
			case Enums::_actor_variant_transform_in_actor_state_handling_override:
				initial_state = initial_state_override;
				break;
			}
			
			// 0xFFFF triggers getting the actor default in actor_create_for_unit
			Enums::actor_default_state return_state = (Enums::actor_default_state)0xFFFF;
			switch(return_state_handling)
			{
			case Enums::_actor_variant_transform_in_actor_state_handling_inherit:
				return_state = actor_data->return_state;
				break;
			case Enums::_actor_variant_transform_in_actor_state_handling_override:
				return_state = return_state_override;
				break;
			}

			blam::actor_create_for_unit(false
				, unit_index
				, actor_variant_tag_index
				, encounter_index
				, squad_index
				, 0
				, NONE
				, false
				, initial_state
				, return_state
				, NONE
				, 0);

			// _encounters_update_dirty_status
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Creates a new unit reusing the old unit. </summary>
		///
		/// <param name="unit_index">	  	Datum index of the old unit. </param>
		/// <param name="unit_datum">	  	[in] If non-null, the old unit's datum. </param>
		/// <param name="resulting_team"> 	The resulting team. </param>
		/// <param name="instigator_team">	The instigator team. </param>
		///
		/// <returns>	The new unit index. </returns>
		static datum_index CreateUnitReuse(const datum_index unit_index
			, Objects::s_unit_datum* unit_datum
			, const Enums::actor_variant_transform_in_team_handling team_option
			, const Enums::game_team instigator_team
			, const Enums::game_team override_team)
		{
			SET_FLAG(unit_datum->object.flags, Flags::_object_yelo_is_transforming_out_bit, false);
			
			// Set the existing unit's team
			unit_datum->object.owner_team = GetTeam(team_option
				, unit_datum->object.owner_team
				, instigator_team
				, override_team);

			return unit_index;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Creates a new unit. </summary>
		///
		/// <param name="unit_index">	  	Datum index of the old unit. </param>
		/// <param name="unit_datum">	  	[in] The old unit's datum. </param>
		/// <param name="new_unit_type">  	Type of the new unit. </param>
		/// <param name="resulting_team"> 	The resulting team. </param>
		/// <param name="instigator_team">	The instigator team. </param>
		///
		/// <returns>	The new unit index. </returns>
		static datum_index CreateUnitNew(const datum_index unit_index
			, Objects::s_unit_datum* unit_datum
			, const datum_index new_unit_type
			, const Enums::actor_variant_transform_in_team_handling team_option
			, const Enums::game_team instigator_team
			, const Enums::game_team override_team)
		{
			// Create the new unit based on the target_unit's data
			Objects::s_object_placement_data unit_placement_data;
			PlacementDataNewAndCopy(unit_placement_data, unit_index, new_unit_type);

			// Set the to-be-created unit's team
			unit_placement_data.owner_team = GetTeam(team_option
				, unit_datum->object.owner_team
				, instigator_team
				, override_team);

			return blam::object_new(unit_placement_data);
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Overrides a units health and shield values. </summary>
		///
		/// <param name="option">	 	The override operation. </param>
		/// <param name="unit_datum">	[in] If non-null, the old unit's datum. </param>
		/// <param name="health">	 	The health value. </param>
		/// <param name="shield">	 	The shield value. </param>
		static void OverrideDamage(const Enums::actor_variant_transform_in_vitality_handling operation
			, Objects::s_unit_datum* unit_datum
			, const real health
			, const real shield)
		{
			switch(operation)
			{
			case Enums::_actor_variant_transform_in_vitality_handling_both:
				unit_datum->object.damage.health = health;
				unit_datum->object.damage.shield = shield;
				break;
			case Enums::_actor_variant_transform_in_vitality_handling_shield_only:
				unit_datum->object.damage.shield = shield;
				break;
			case Enums::_actor_variant_transform_in_vitality_handling_health_only:
				unit_datum->object.damage.health = health;
				break;
			}
		}

		void c_actor_variant_transform_manager::TransformOut(const datum_index unit_index
			, Objects::s_unit_datum* unit_datum
			, const Enums::game_team instigator_team
			, const TagGroups::actor_variant_transform_out_definition& transform_out_definition
			, const TagGroups::actor_variant_transform_in_definition& transform_in_definition)
		{
			// Freeze the actor
			blam::actor_braindead(unit_datum->unit.actor_index, true);

			// Play the transform-out animation
			blam::unit_start_user_animation(unit_index
				, unit_datum->object.animation.definition_index
				, transform_out_definition.transform_out_anim
				, false);

			// Attach the transform objects
			AttachObjects(unit_index, unit_datum, instigator_team, transform_in_definition);

			if(TEST_FLAG(transform_out_definition.flags, Flags::_actor_variant_transform_out_flags_invicible_during_transform_out_bit))
			{
				SET_FLAG(unit_datum->object.damage.flags, Flags::_object_cannot_take_damage_bit, true);
			}

			SET_FLAG(unit_datum->object.flags, Flags::_object_yelo_is_transforming_out_bit, true);
		}

		void c_actor_variant_transform_manager::TransformIn(const datum_index unit_index
			, Objects::s_unit_datum* unit_datum
			, const s_actor_variant_transform_state& transform_state)
		{
			// Get a random transform target
			auto& transform_in_definition = *transform_state.m_transform_entry->transform_in_ptr;
			auto& transform_target = transform_in_definition.targets[rand() % transform_in_definition.targets.Count];

			auto* actor_variant_definition = blam::tag_get<TagGroups::s_actor_variant_definition>(transform_target.actor_variant.tag_index);

			// Spawn the transition effect
			blam::hs_effect_new_from_object_marker(transform_target.transform_effect.tag_index, unit_index);
			
			// Drop weapon if needed
			if(TEST_FLAG(transform_target.flags, Flags::_actor_variant_transform_in_target_flags_drop_weapon))
			{
				blam::unit_drop_current_weapon(unit_index, true);
			}

			// Create the new unit
			bool delete_unit = false;
			auto new_unit_index = datum_index::null;
			if(TEST_FLAG(transform_target.flags, Flags::_actor_variant_transform_in_target_flags_try_to_use_existing_unit)
				&& (unit_datum->object.definition_index == actor_variant_definition->unit.tag_index))
			{
				new_unit_index = CreateUnitReuse(unit_index
					, unit_datum
					, transform_target.team_handling
					, transform_state.m_instigator_team
					, transform_target.team_override);

				DeleteAttachments(unit_index, transform_in_definition);
				Objects::DetachChildActors(unit_index);
			}
			else
			{
				new_unit_index = CreateUnitNew(unit_index
					, unit_datum
					, actor_variant_definition->unit.tag_index
					, transform_target.team_handling
					, transform_state.m_instigator_team
					, transform_target.team_override);

				delete_unit = true;
			}
			auto* new_unit_datum = blam::object_get_and_verify_type<Objects::s_unit_datum>(new_unit_index);

			// Handle seated units
			// TODO: Add ejection and inheritance
			for(datum_index current_child = unit_datum->object.first_object_index;
				!current_child.IsNull();
				current_child = blam::object_get(current_child)->next_object_index)
			{
				auto child_object = blam::object_try_and_get_and_verify_type<Objects::s_unit_datum>(current_child);
				if(!child_object || (child_object->unit.vehicle_seat_index == NONE))
				{
					continue;
				}

				blam::unit_kill(current_child);
			}

			real old_unit_health = unit_datum->object.damage.health;
			real old_unit_shield = unit_datum->object.damage.shield;

			// Set up the unit's actor
			SetupUnitActor(new_unit_index
				, transform_target.actor_variant.tag_index
				, transform_target.initial_state_handling
				, transform_target.initial_state_override
				, transform_target.return_state_handling
				, transform_target.return_state_override
				, unit_datum->unit.actor_index
				, TEST_FLAG(transform_target.flags, Flags::_actor_variant_transform_in_target_flags_inherit_encounter_squad));

			blam::actor_braindead(new_unit_datum->unit.actor_index, true);

			// Inherit unit vitality
			OverrideDamage(transform_target.vitality_inheritance
				, new_unit_datum
				, old_unit_health
				, old_unit_shield);

			// Override unit vitality
			OverrideDamage(transform_target.vitality_override
				, new_unit_datum
				, transform_target.health_override
				, transform_target.shield_override);

			SET_FLAG(new_unit_datum->object.damage.flags, Flags::_object_shield_depleted_bit, new_unit_datum->object.damage.shield <= 0.0f);

			// Delete the existing unit if necessary
			if(delete_unit)
			{
				Objects::DetachChildActors(unit_index);

				blam::actor_delete(unit_datum->unit.actor_index);
				blam::object_delete(unit_index);
			}

			// Play the transform-in animation
			blam::unit_start_user_animation(new_unit_index
				, new_unit_datum->object.animation.definition_index
				, transform_target.transform_in_anim
				, false);

			SET_FLAG(new_unit_datum->object.damage.flags, Flags::_object_cannot_take_damage_bit, true);
			SET_FLAG(new_unit_datum->object.flags, Flags::_object_yelo_is_transforming_in_bit, true);
		}

		void c_actor_variant_transform_manager::TransformEnd(Objects::s_unit_datum* unit_datum)
		{
			blam::actor_braindead(unit_datum->unit.actor_index, false);

			SET_FLAG(unit_datum->object.damage.flags, Flags::_object_cannot_take_damage_bit, false);
			SET_FLAG(unit_datum->object.flags, Flags::_object_yelo_is_transforming_in_bit, false);
		}
#pragma endregion

		bool c_actor_variant_transform_manager::GetTransformsAllowed()
		{
			return m_transforms_allowed;
		}

		void c_actor_variant_transform_manager::SetTransformsAllowed(const bool value)
		{
			m_transforms_allowed = value;
		}

		void c_actor_variant_transform_manager::LoadActorVariantTransforms()
		{
			TagGroups::c_tag_iterator iterator(TagGroups::actor_variant_transform_collection_definition::k_group_tag);

			// Get the first collection tag as there should be only one
			auto tag_index = iterator.Next();
			if(tag_index.IsNull())
			{
				m_transform_collection = nullptr;
				return;
			}

			m_transform_collection = TagGroups::TagGetForModify<TagGroups::actor_variant_transform_collection_definition>(tag_index);

			// Populate the entry pointers
			for(auto& actor_variant_entry : m_transform_collection->actor_variant_transforms)
			{
				for(auto& transform_entry : actor_variant_entry.transforms)
				{
					auto* transform_out_ptr = TagGroups::TagGetForModify<TagGroups::actor_variant_transform_out_definition>(transform_entry.transform_out.tag_index);
					auto* transform_in_ptr = TagGroups::TagGetForModify<TagGroups::actor_variant_transform_in_definition>(transform_entry.transform_in.tag_index);
					
					transform_entry.transform_out_ptr = transform_out_ptr;
					transform_entry.transform_in_ptr = transform_in_ptr;
				}
			}
		}

		void c_actor_variant_transform_manager::UnloadActorVariantTransforms()
		{
			m_transform_collection = nullptr;
		}

		void c_actor_variant_transform_manager::ClearInProgressTransforms()
		{
			m_transforms_in_progress.clear();
		}

		void c_actor_variant_transform_manager::UnitDamaged(const datum_index unit_index, const Objects::s_damage_data* damage_data)
		{
			if(!m_transform_collection || !m_transforms_allowed)
			{
				return;
			}

			// Get the unit datums
			auto* unit_datum = blam::object_try_and_get_and_verify_type<Objects::s_unit_datum>(unit_index);
			if(!unit_datum)
			{
				return;
			}

			// Do not allow transforms whilst in a seat
			if(unit_datum->unit.vehicle_seat_index != NONE)
			{
				return;
			}

			// If the unit is dead or already transforming, return
			if(TEST_FLAG(unit_datum->object.damage.flags, Flags::_object_killed_bit)
				|| TEST_FLAG(unit_datum->object.flags, Flags::_object_yelo_is_transforming_out_bit)
				|| TEST_FLAG(unit_datum->object.flags, Flags::_object_yelo_is_transforming_in_bit))
			{
				return;
			}

			// Transforms only apply to actors
			if(unit_datum->unit.actor_index.IsNull())
			{
				return;
			}

			auto* actor_datum = AI::Actors()[unit_datum->unit.actor_index];
			if(!actor_datum)
			{
				return;
			}
			
			// If the actor variant has no transforms defined, return
			auto& actor_variant_transforms = m_transform_collection->actor_variant_transforms;
			auto found_entry = std::find_if(actor_variant_transforms.begin(), actor_variant_transforms.end(),
				[&](const TagGroups::actor_variant_transform_collection_entry& entry)
				{
					return entry.actor_variant.tag_index == actor_datum->meta.actor_variant_definition_index;
				}
			);

			if(found_entry == actor_variant_transforms.end())
			{
				return;
			}
			auto& transformations = found_entry->transforms;

			// Try to find a transform for the damage instigator
			// If no match is found, look for a default transform
			// Return if no match is found
			bool is_melee = (unit_datum->unit.damage_result.category == Enums::_damage_category_melee);

			auto* actor_variant_transform = FindTransform(transformations
				, unit_index
				, damage_data->responsible_unit_index
				, is_melee);

			if(!actor_variant_transform)
			{
				actor_variant_transform = FindTransform(transformations
					, unit_index
					, datum_index::null
					, is_melee);

				if(!actor_variant_transform)
				{
					return;
				}
			}
			
			auto* instigator_unit_datum = blam::object_try_and_get_and_verify_type<Objects::s_unit_datum>(damage_data->responsible_unit_index);
			auto instigator_team = (instigator_unit_datum ? instigator_unit_datum->object.owner_team : Enums::_game_team_none);

			// Start transforming the unit, using the found transform
			TransformOut(unit_index
				, unit_datum
				, instigator_team
				, *actor_variant_transform->transform_out_ptr
				, *actor_variant_transform->transform_in_ptr);
			
			m_transforms_in_progress[unit_index.index] = s_actor_variant_transform_state
			{
				actor_variant_transform,
				instigator_team
			};
		}

		void c_actor_variant_transform_manager::UnitUpdate(const datum_index unit_index)
		{
			if(!m_transform_collection)
			{
				return;
			}

			auto* unit_datum = blam::object_try_and_get_and_verify_type<Objects::s_unit_datum>(unit_index);
			if(!unit_datum)
			{
				return;
			}

			// If the unit was transforming but is now dead or had it's actor removed, stop the transform
			if(TEST_FLAG(unit_datum->object.flags, Flags::_object_yelo_is_transforming_out_bit)
				|| TEST_FLAG(unit_datum->object.flags, Flags::_object_yelo_is_transforming_in_bit))
			{
				if(TEST_FLAG(unit_datum->object.damage.flags, Flags::_object_killed_bit) || unit_datum->unit.actor_index.IsNull())
				{
					SET_FLAG(unit_datum->object.flags, Flags::_object_yelo_is_transforming_out_bit, false);
					SET_FLAG(unit_datum->object.flags, Flags::_object_yelo_is_transforming_in_bit, false);
					
					auto& transform_state = m_transforms_in_progress[unit_index.index];
					auto& transform_in_definition = *transform_state.m_transform_entry->transform_in_ptr;
					if(TEST_FLAG(transform_in_definition.flags, Flags::_actor_variant_transform_in_attachment_flags_delete_attachments_on_death))
					{
						DeleteAttachments(unit_index, transform_in_definition);
					}

					m_transforms_in_progress.erase(unit_index.index);
					return;
				}
			}

			// If the unit has started a transformation, wait until the animation is done playing
			if(TEST_FLAG(unit_datum->object.flags, Flags::_object_yelo_is_transforming_out_bit))
			{
				// If the transform out animation has finished playing start the transform in stage
				auto frames_left = blam::unit_get_custom_animation_time(unit_index);
				if(frames_left == 0 || (unit_datum->unit.animation.state != Enums::_unit_animation_state_custom_animation))
				{
					// Prevent creating a new actor whilst ai is disabled
					if(AI::AIGlobals()->ai_active)
					{
						TransformIn(unit_index, unit_datum, m_transforms_in_progress[unit_index.index]);
						m_transforms_in_progress.erase(unit_index.index);
					}
				}
			}
			else if(TEST_FLAG(unit_datum->object.flags, Flags::_object_yelo_is_transforming_in_bit))
			{
				// If the transform in animation has finished playing end the transformation
				auto frames_left = blam::unit_get_custom_animation_time(unit_index);
				if(frames_left == 0 || (unit_datum->unit.animation.state != Enums::_unit_animation_state_custom_animation))
				{
					TransformEnd(unit_datum);
				}
			}
		}
	};};
};