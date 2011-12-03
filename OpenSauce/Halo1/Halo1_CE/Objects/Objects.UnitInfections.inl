/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/

namespace UnitInfections
{
	static int32 DefinitionFindInfectableUnitIndex(TagGroups::s_unit_infections_definition const& definition, 
		int32 infection_unit_index, datum_index infectable_unit_definition_index)
	{
		for(int32 x = 0; x < definition.infectable_units.Count; x++)
		{
			TagGroups::s_unit_infection const& unit_infection = definition.infectable_units[x];

			if(	infection_unit_index == unit_infection.infection_unit &&
				infectable_unit_definition_index == unit_infection.unit)
				return x;
		}
		return NONE;
	}
	static int32 DefinitionFindInfectionUnitIndex(TagGroups::s_unit_infections_definition const& definition, 
		datum_index unit_definition_index)
	{
		for(int32 x = 0; x < definition.infection_units.Count; x++)
		{
			if(unit_definition_index == definition.infection_units[x].tag_index)
				return x;
		}
		return NONE;
	}
	static bool AllowInfections(TagGroups::s_unit_infections_definition const& definition)
	{
		// Don't try to infect anything if we're in a cinematic, unless the designers say it's okay
		return	!Camera::CinematicGlobals()->in_progress || 
				TagGroups::_global_yelo_globals->flags.allow_unit_infections_during_cinematics_bit;
	}

	static int32 UnitGetAnimationIndexFromWeaponClass(datum_index unit_index, _enum weapon_class_animation)
	{
		int32 animation_index = NONE;
		TagGroups::model_animation_graph const* animation_graph = GetObjectAnimations(unit_index);

		if(animation_graph != NULL)
		{
			s_unit_datum* unit = (*Objects::ObjectHeader())[unit_index]->Type._unit;
			s_unit_animation_data& unit_animation = *unit->unit.GetAnimation();

			sbyte	seat_block_index = *unit_animation.GetSeatIndex(),
					weapon_block_index = *unit_animation.GetSeatWeaponIndex();

			TagGroups::animation_graph_weapon const& weapon = animation_graph->units[seat_block_index].weapons[weapon_block_index];

			// Make sure the weapon class exists in the seat's weapon animations
			if(weapon_class_animation < weapon.animations.Count)
				animation_index = weapon.animations[weapon_class_animation];
		}

		return animation_index;
	}

	// Attach the infection definition's attachment object to the target unit at the specific marker permutation
	static void InfectionAttachObject(TagGroups::s_unit_infection const& unit_infection, 
		s_unit_datum* target_unit, datum_index target_unit_index, int32 marker_index)
	{
		datum_index attachment_object_index;
		s_object_data* attachment_object;
		{	// Create the attachment based on the target_unit's world data and the infection tag data
			s_object_placement_data attachment_placement_data;
			Engine::Objects::PlacementDataNew(attachment_placement_data, unit_infection.attachment_object.tag_index);
			attachment_placement_data.position = target_unit->object.GetNetworkDatumData()->position;

			attachment_object_index = Engine::Objects::New(attachment_placement_data);
			attachment_object = (*Objects::ObjectHeader())[attachment_object_index]->_object;
		}

		tag_string marker_name;
		sprintf_s(marker_name, "%s%d", unit_infection.attachment_marker, marker_index);
		// TODO: allow a marker name field in the tag definition for the target object?
		Engine::Objects::Attach(target_unit_index, "", attachment_object_index, marker_name);
	}
	// Attach the infection definition's attachment object to the target unit
	static void InfectionAttachObjects(TagGroups::s_unit_infection const& unit_infection, 
		s_unit_datum* target_unit, datum_index target_unit_index)
	{
		for(int32 x = 0; x < unit_infection.attachment_marker_count; x++)
			InfectionAttachObject(unit_infection, target_unit, target_unit_index, x);
	}

	// Starts an infection sequence on the target unit
	static void InfectionStart(TagGroups::s_unit_infection const& unit_infection, 
		s_unit_datum* target_unit, datum_index target_unit_index)
	{
		int32 infect_start_animation_index = UnitGetAnimationIndexFromWeaponClass(target_unit_index, Enums::_weapon_class_animation_yelo_infect_start);
		if(infect_start_animation_index != NONE)
		{
			datum_index actor_index = *target_unit->unit.GetActorIndex();
			// Detach any AI from the target unit so the anim can be played without interruption
			if(!actor_index.IsNull())
				Engine::AI::Delete(actor_index);
			// Set the current animation state to that of a "custom" animation (to avoid the engine doing any weirdness)
			*target_unit->unit.GetAnimation()->GetAnimationState() = Enums::_unit_animation_state_custom_animation;
			// Play the infect-start animation on the target unit
			Engine::Objects::UnitSetAnimation(target_unit_index, *target_unit->object.GetAnimationDefinition(), 
				infect_start_animation_index);
		}

		InfectionAttachObjects(unit_infection, target_unit, target_unit_index);

		SET_FLAG(*target_unit->object.GetFlags(), Flags::_object_yelo_is_being_infected_bit, true);
	}

	// Ends an infection sequence on the target unit
	static void InfectionEnd(TagGroups::s_unit_infection const& unit_infection, 
		s_unit_datum* target_unit, datum_index target_unit_index)
	{
		datum_index infected_unit_index;
		s_unit_datum* infected_unit;
		{	// Create the infected_unit based on the target_unit's world data and the infection tag data
			s_object_placement_data infected_unit_placement_data;
			PlacementDataNewAndCopy(infected_unit_placement_data, target_unit_index, 
				unit_infection.infected_unit.tag_index);
			infected_unit_index = Engine::Objects::New(infected_unit_placement_data);
			infected_unit = (*Objects::ObjectHeader())[infected_unit_index]->Type._unit;
		}

		int32 infect_end_animation_index = UnitGetAnimationIndexFromWeaponClass(infected_unit_index, Enums::_weapon_class_animation_yelo_infect_end);
		if(infect_end_animation_index != NONE)
		{
			// Play the infect-end animation on the infected unit
			Engine::Objects::UnitSetAnimation(infected_unit_index, *infected_unit->object.GetAnimationDefinition(), 
				infect_end_animation_index);
			// Set the current animation state to that of a "custom" animation (to avoid the engine doing any weirdness)
			*infected_unit->unit.GetAnimation()->GetAnimationState() = Enums::_unit_animation_state_custom_animation;
		}

		// Kill the unit being infected
		Engine::Objects::DepleteBody(target_unit_index);
		// Spawn the transition effect
		Engine::Effects::NewOnObjectMarker(unit_infection.transition_effect.tag_index, target_unit_index);
		// Assign the AI definition for the infected unit
		Engine::AI::AttachFree(infected_unit_index, unit_infection.infected_unit_actor_variant.tag_index);
		// Delete the target unit (
		Engine::Objects::Delete(target_unit_index);
	}

	// Check's infection_unit's parent for an entry in the unit_infections definitions
	// If infection_unit can infect parent's object definition (ie, elite), this will handle the magic
	static void TryInfection(s_unit_datum* infection_unit)
	{
		if(TagGroups::_global_yelo_globals->unit_infections.Count == 0)
			return;

		TagGroups::s_unit_infections_definition const& definition = TagGroups::_global_yelo_globals->unit_infections[0];
		if(	!AllowInfections(definition) )
			return;

		datum_index target_unit_index = *infection_unit->object.GetParentObjectIndex();
		s_unit_datum* target_unit = (*Objects::ObjectHeader())[target_unit_index]->Type._unit;

		// Find the s_unit_infection based on the infection form's unit definition and the target unit's definition indexes
		int32 unit_infection_definition_index = DefinitionFindInfectableUnitIndex(definition,
			DefinitionFindInfectionUnitIndex(definition, *infection_unit->object.GetTagDefinition()),
			*target_unit->object.GetTagDefinition());

		// If the target unit can be infected by the infection_unit...
		if(unit_infection_definition_index != NONE)
		{
			TagGroups::s_unit_infection const& unit_infection = definition.infectable_units[unit_infection_definition_index];

			real* target_unit_health = target_unit->object.GetHealth();
			sbyte* target_unit_animation_state = target_unit->unit.GetAnimation()->GetAnimationState();

			// If the target unit's health is below the threshold and hasn't died yet...
			if(*target_unit_health <= unit_infection.health_threshold && *target_unit_health > 0.0f)
			{
				// If the target unit isn't already infected...
				if( !TEST_FLAG(*target_unit->object.GetFlags(), Flags::_object_yelo_is_being_infected_bit) )
					InfectionStart(unit_infection, target_unit, target_unit_index);
				else // They're already infected, wait until the animation is done playing then start the transition
				{
					int16 frames_left = Engine::Objects::UnitGetCustomAnimationTime(target_unit_index);

					if(frames_left == 0)
					{
						// Set the target unit's health to 0 so we don't execute this code again
						*target_unit_health = 0.0f;
						InfectionEnd(unit_infection, target_unit, target_unit_index);
					}
					else if(frames_left > 0)
					{
						// Prevent the target from being killed while in the transition
						// TODO: Expose a flag in the definition to allow a unit being infected to be killed?
						word_flags* dmg_flags = target_unit->object.GetDamageFlags();
						SET_FLAG(*dmg_flags, Flags::_object_cannot_take_damage_bit, true);

						// Detach the infection forms on each update call. This prevents the infection 
						// forms from disappearing when we delete the (uninfected) target unit
						DetachChildrenByType(target_unit_index, Enums::_object_type_mask_unit);
					}
				}
			}
		}
	}

	static API_FUNC_NAKED void PLATFORM_API UnitCauseParentMeleeDamageHook()
	{
		static uint32 RETN_ADDRESS = GET_FUNC_PTR(UNIT_CAUSE_PARENT_MELEE_DAMAGE_HOOK_RETN);

		__asm {
			dec		al
			mov		[esi+0x28A], al
			// we hook the function at the very last instructions, so there's no need to preserve any registers

			push	esi				// esi is a pointer to a unit datum (ebp is the datum_index to it)
			call	TryInfection

			jmp		RETN_ADDRESS
		}
	}

	void Initialize()
	{
		static const byte k_null_bytes[8] = { 
			Enums::_x86_opcode_nop, Enums::_x86_opcode_nop,
			Enums::_x86_opcode_nop, Enums::_x86_opcode_nop,
			Enums::_x86_opcode_nop, Enums::_x86_opcode_nop,
			Enums::_x86_opcode_nop, Enums::_x86_opcode_nop };

			Memory::WriteMemory(GET_FUNC_VPTR(UNIT_CAUSE_PARENT_MELEE_DAMAGE_HOOK), k_null_bytes, sizeof(k_null_bytes));
			Memory::WriteRelativeJmp(UnitCauseParentMeleeDamageHook, GET_FUNC_VPTR(UNIT_CAUSE_PARENT_MELEE_DAMAGE_HOOK), true);
	}

	void Dispose()
	{
	}
};