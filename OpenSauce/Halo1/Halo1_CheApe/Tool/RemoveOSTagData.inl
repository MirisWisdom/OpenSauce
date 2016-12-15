/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/

namespace Yelo
{
	namespace Tool
	{
#pragma region Units
		void RemoveOpenSauceData_Unit(const datum_index tag_index)
		{
			auto& unit_definition = *blam::tag_get<TagGroups::s_unit_definition>(tag_index);

			// Delete unit extensions
			blam::tag_block_resize(unit_definition.unit.extensions, 0);

			// Delete seat extensions
			for(auto& seat : unit_definition.unit.seats)
			{
				blam::tag_block_resize(seat.seat_extensions, 0);
			}
		}

		void RemoveOpenSauceData_Biped(const datum_index tag_index)
		{
			RemoveOpenSauceData_Unit(tag_index);
		}

		void RemoveOpenSauceData_Vehicle(const datum_index tag_index)
		{
			RemoveOpenSauceData_Unit(tag_index);
		}
#pragma endregion

#pragma region Shaders
		void RemoveOpenSauceData_ShaderModel(const datum_index tag_index)
		{
			auto& definition = *blam::tag_get<TagGroups::s_shader_model_definition>(tag_index);

			blam::tag_block_resize(definition.model.maps.shader_extension, 0);
		}

		void RemoveOpenSauceData_ShaderEnvironment(const datum_index tag_index)
		{
			auto& definition = *blam::tag_get<TagGroups::s_shader_environment_definition>(tag_index);

			blam::tag_block_resize(definition.environment.shader_extension, 0);
		}

		void RemoveOpenSauceData_ShaderEffect(TagGroups::s_shader_effect& effect_definition)
		{
			blam::tag_block_resize(effect_definition.effect.shader_extension, 0);
		}
#pragma endregion

#pragma region Particle Effects
		void RemoveOpenSauceData_Contrail(const datum_index tag_index)
		{
			auto& definition = *blam::tag_get<TagGroups::s_contrail_definition>(tag_index);

			RemoveOpenSauceData_ShaderEffect(definition.shader_effect);
		}

		void RemoveOpenSauceData_Particle(const datum_index tag_index)
		{
			auto& definition = *blam::tag_get<TagGroups::s_particle_definition>(tag_index);

			RemoveOpenSauceData_ShaderEffect(definition.shader_effect);
		}

		void RemoveOpenSauceData_WeatherParticleSystem(const datum_index tag_index)
		{
			auto& definition = *blam::tag_get<TagGroups::s_weather_particle_system_definition>(tag_index);

			for(auto& particle : definition.particle_types)
			{
				RemoveOpenSauceData_ShaderEffect(particle.shader_effect);
			}
		}

		void RemoveOpenSauceData_ParticleSystem(const datum_index tag_index)
		{
			auto& definition = *blam::tag_get<TagGroups::s_particle_system_definition>(tag_index);

			for(auto& particle : definition.particle_types)
			{
				for(auto& state : particle.particle_states)
				{
					RemoveOpenSauceData_ShaderEffect(state.shader_effect);
				}
			}
		}
#pragma endregion

#pragma region Animations
		void RemoveOpenSauceData_ModelAnimations(const datum_index tag_index)
		{
			auto& definition = *blam::tag_get<TagGroups::model_animation_graph>(tag_index);

			for(auto& unit_seat_animation : definition.units)
			{
				if(unit_seat_animation.animations.Count > Enums::k_number_of_unit_seat_animations)
				{
					blam::tag_block_resize(unit_seat_animation.animations, Enums::k_number_of_unit_seat_animations);
				}
			}

			if(definition.animations.Count > Enums::k_max_animations_per_graph)
			{
				AddWarning("This animation graph more than 256 animations, you will need to manually remove unwanted animations", tag_index);
			}
		}
#pragma endregion
		
#pragma region Scenario
		void RemoveOpenSauceData_Scenario(const datum_index tag_index)
		{
			auto& definition = *blam::tag_get<TagGroups::scenario>(tag_index);

			// Clear the yelo reference
			tag_reference& yelo_reference = definition.GetYeloReferenceHack();

			if (yelo_reference.group_tag == TagGroups::project_yellow::k_group_tag)
			{
				blam::tag_reference_clear(yelo_reference);
			}

			// If the scenario is using upgraded script node sizes, clear it
			// Users will need to recompile their scenario's scripts
			tag_data& hs_syntax_data = definition.hs_syntax_data;
			if ((size_t)hs_syntax_data.size > Scripting::GetTotalScenarioHsSyntaxData())
			{
				TagGroups::tag_data_delete(&hs_syntax_data); // If hs_syntax_data.size != GetTotalScenarioHsSyntaxData, the engine will recompile the scripts
				AddWarning("This scenario tag will need to have it's scripts recompiled by Sapien", tag_index);
			}

			// Delete the bsp modifiers
			blam::tag_block_resize(definition.bsp_modifiers, 0);
		}
#pragma endregion
	};
};