/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#pragma once

#include <blamlib/Halo1/tag_files/tag_groups_structures_macros.hpp>
#include <YeloLib/Halo1/tag_files/tag_fieldset_replacement_builder.hpp>
#include <blamlib/Halo1/shaders/shader_definitions.hpp>
#include <blamlib/Halo1/effects/contrail_definitions.hpp>
#include <blamlib/Halo1/effects/particle_definitions.hpp>
#include <blamlib/Halo1/effects/particle_system_definitions.hpp>
#include <blamlib/Halo1/effects/weather_particle_system_definitions.hpp>

#include "TagGroups/TagFieldReplacements/Shared.hpp"

namespace Yelo
{
	namespace TagGroups { namespace TagFieldReplacements { namespace Shader
	{
#pragma region Shader Effect
		template<typename kBlockType, int kFieldCount>
		static void InitialiseShaderEffect(tag_block_definition* block_definition)
		{
			TAG_GROUP_BLOCK_FIELDS_DEFINE(shader_effect_extension) =
			{
				TAG_FIELD_ENTRY_PAD(4),

				// Depth fade
				TAG_FIELD_ENTRY(_field_explanation, "Fade Controls", "Controls the softness of an effect"),

				TAG_FIELD_ENTRY(_field_real, "depth fade distance", "Controls the softness of an effect when intersecting opaque geometry"),
				TAG_FIELD_ENTRY(_field_real, "camera fade distance", "Controls the softness of an effect when nearing the near clip plane"),
				
				TAG_FIELD_ENTRY_PAD(sizeof(tag_block) * 3),
				TAG_FIELD_ENTRY_END()
			};

			TAG_GROUP_BLOCK_DEFINE(shader_effect_extension, 0, 1, sizeof(s_shader_effect_extension));

			FIELDSET_REPLACEMENT_BEGIN(kBlockType, kFieldCount + 1, block_definition)
				FIELDSET_SEEK_AFTER_WITH_COPY("map flags")
				FIELDSET_INSERT_BEGIN()
					fsr_builder.InsertBlock(
						offsetof(kBlockType, shader_effect) +
						offsetof(s_shader_effect, effect) +
						offsetof(_shader_effect_definition, shader_extension),
						"shader extension"
						, &TAG_GROUP_BLOCK_GET(shader_effect_extension)
					);
				FIELDSET_INSERT_END()
			FIELDSET_REPLACEMENT_END();
		}
		
		static void InitialiseShaderEffect_Contrail()
		{
			tag_group* cont = blam::tag_group_get<s_contrail_definition>();

			InitialiseShaderEffect<s_contrail_definition, 50>(cont->header_block_definition);
		}

		static void InitialiseShaderEffect_Particle()
		{
			tag_group* part = blam::tag_group_get<s_particle_definition>();

			InitialiseShaderEffect<s_particle_definition, 56>(part->header_block_definition);
		}

		static void InitialiseShaderEffect_WeatherParticleSystem()
		{
			tag_group* rain = blam::tag_group_get<s_weather_particle_system_definition>();

			auto block_definition = Shared::GetTagField(*rain->header_block_definition, Enums::_field_block, "particle types").Definition<tag_block_definition>();
			InitialiseShaderEffect<s_weather_particle_type, 59>(block_definition);
		}

		static void InitialiseShaderEffect_ParticleSystem()
		{
			tag_group* pctl = blam::tag_group_get<s_particle_system_definition>();

			auto block_definition = Shared::GetTagField(*pctl->header_block_definition, Enums::_field_block, "particle types:particle states").Definition<tag_block_definition>();
			InitialiseShaderEffect<s_particle_system_type_particle_states, 46>(block_definition);
		}
#pragma endregion

#pragma region Shader Environment
		static void InitializeShaderEnvironment()
		{
			tag_group* senv = blam::tag_group_get<s_shader_environment_definition>();
			
			TAG_GROUP_STRING_TABLE_DEFINE(shader_environment_extension_flags, 1, "do not use dlms");

			TAG_GROUP_STRING_TABLE_DEFINE(shader_environment_extension_specular_flags, 1, "alpha as exponent mask");

			TAG_GROUP_BLOCK_FIELDS_DEFINE(shader_environment_extension) =
			{
				TAG_FIELD_ENTRY(_field_explanation, "directional lightmaps", "" ),
				TAG_FIELD_ENTRY(_field_word_flags, "flags", &shader_environment_extension_flags),
				TAG_FIELD_ENTRY_PAD(sizeof(int16)),
				TAG_FIELD_ENTRY(_field_real, "bump amount", nullptr),

				TAG_FIELD_ENTRY(_field_explanation, "specular DLM color map", "" ),
				TAG_FIELD_ENTRY(_field_tag_reference, "specular color map", &Shared::TAG_GROUP_REFERENCE_GET(bitmap)),
				TAG_FIELD_ENTRY(_field_real, "specular color exponent", nullptr),
				TAG_FIELD_ENTRY(_field_real, "specular color coefficient", nullptr),
				TAG_FIELD_ENTRY(_field_word_flags, "specular color flags", &shader_environment_extension_specular_flags),
				TAG_FIELD_ENTRY_PAD(sizeof(int16)),
				
				TAG_FIELD_ENTRY(_field_explanation, "specular DLM tint and brightness", "" ),
				TAG_FIELD_ENTRY(_field_real_fraction, "perpendicular brightness", nullptr),
				TAG_FIELD_ENTRY(_field_real_rgb_color, "perpendicular tint color", nullptr),
				TAG_FIELD_ENTRY(_field_real_fraction, "parallel brightness", nullptr),
				TAG_FIELD_ENTRY(_field_real_rgb_color, "parallel tint color", nullptr),
				
				TAG_FIELD_ENTRY(_field_explanation, "specular DLM intensity", "" ),
				TAG_FIELD_ENTRY(_field_real, "specular lighting exponent", nullptr),
				TAG_FIELD_ENTRY(_field_real, "specular lighting coefficient", nullptr),
				
				TAG_FIELD_ENTRY_PAD(sizeof(tag_block) * 2),
				TAG_FIELD_ENTRY_END()
			};

			TAG_GROUP_BLOCK_DEFINE(shader_environment_extension, 0, 1, sizeof(s_shader_environment_extension));

			FIELDSET_REPLACEMENT_BEGIN(_shader_environment_definition, 92+2, senv->header_block_definition)
				FIELDSET_SEEK_AFTER_WITH_COPY("lens flare")
				FIELDSET_INSERT_BEGIN()
					FIELDSET_INSERT_BLOCK(shader_extension, "shader extension", &TAG_GROUP_BLOCK_GET(shader_environment_extension))
				FIELDSET_INSERT_END()
			FIELDSET_REPLACEMENT_END();
		}
#pragma endregion Shader Environment
		
		static void Initialize()
		{
			InitializeShaderEnvironment();
			InitialiseShaderEffect_Contrail();
			InitialiseShaderEffect_Particle();
			InitialiseShaderEffect_WeatherParticleSystem();
			InitialiseShaderEffect_ParticleSystem();
		}
	};};};
};