/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Rasterizer/PostProcessing/PostProcessingScripting.hpp"

#if !PLATFORM_IS_DEDI
#include "Rasterizer/PostProcessing/PostProcessing.hpp"
#include "Rasterizer/PostProcessing/MotionBlur/c_system_motionblur.hpp"
#include "Rasterizer/PostProcessing/Bloom/c_system_bloom.hpp"
#include "Rasterizer/PostProcessing/FXAA/c_system_fxaa.hpp"
#include "Rasterizer/PostProcessing/Generic/Internal/c_system_internal.hpp"
#include "Rasterizer/PostProcessing/Generic/External/c_system_external.hpp"

namespace Yelo
{
	namespace Rasterizer { namespace PostProcessing { namespace Scripting
	{
		void*		HS_Load()
		{
			LoadSystem();

			return NULL;
		}

		void*		HS_Unload()
		{
			UnloadSystem();

			return NULL;
		}

		namespace Globals
		{
			bool& Enabled_External() { return Generic::External::c_system_external::Instance().Enabled(); }
			bool& Enabled_Internal() { return Generic::Internal::c_system_internal::Instance().Enabled(); }
			bool& Enabled_MotionBlur() { return MotionBlur::c_system_motionblur::Instance().Enabled(); }
			bool& Enabled_FXAA() { return FXAA::c_system_fxaa::Instance().Enabled(); }

			real& MotionBlur_Amount() { return MotionBlur::c_system_motionblur::Instance().BlurAmount(); }
		};

		namespace Bloom
		{
			void*		HS_BloomSetSize(void** arguments)
			{
				struct s_arguments {
					real size;
					real interp_time;
				}* args = CAST_PTR(s_arguments*, arguments);

				PostProcessing::Bloom::c_system_bloom::Instance().SetBloomSize(args->size, args->interp_time);

				return NULL;
			}

			void*		HS_BloomSetExposure(void** arguments)
			{
				struct s_arguments {
					real exposure;
					real interp_time;
				}* args = CAST_PTR(s_arguments*, arguments);

				PostProcessing::Bloom::c_system_bloom::Instance().SetBloomExposure(args->exposure, args->interp_time);

				return NULL;
			}

			void*		HS_BloomSetMixAmount(void** arguments)
			{
				struct s_arguments {
					real mix_amount;
					real interp_time;
				}* args = CAST_PTR(s_arguments*, arguments);

				PostProcessing::Bloom::c_system_bloom::Instance().SetBloomMixAmount(args->mix_amount, args->interp_time);

				return NULL;
			}

			void*		HS_BloomSetMinimumColor(void** arguments)
			{
				struct s_arguments {
					real_rgb_color minimum_color;
					real interp_time;
				}* args = CAST_PTR(s_arguments*, arguments);

				PostProcessing::Bloom::c_system_bloom::Instance().SetBloomMinimumColor(args->minimum_color, args->interp_time);

				return NULL;
			}

			void*		HS_BloomSetMaximumColor(void** arguments)
			{
				struct s_arguments {
					real_rgb_color maximum_color;
					real interp_time;
				}* args = CAST_PTR(s_arguments*, arguments);

				PostProcessing::Bloom::c_system_bloom::Instance().SetBloomMaximumColor(args->maximum_color, args->interp_time);

				return NULL;
			}
		};
		namespace Internal
		{
			void*		HS_GetEffectInstanceIndexByName(void** arguments)
			{
				struct s_arguments {
					const char* test_string;
				}* args = CAST_PTR(s_arguments*, arguments);
				TypeHolder result;

				result.uint32 = Generic::Internal::c_system_internal::Instance().GetEffectInstanceIndexByName(args->test_string);

				return result.pointer;
			}

			void*		HS_SetEffectInstanceActive(void** arguments)
			{
				struct s_arguments {
					uint16 effect_index;
					PAD16;
					bool active_bool;
					PAD24;
				}* args = CAST_PTR(s_arguments*, arguments);

				Generic::Internal::c_system_internal::Instance().SetEffectInstanceActive(args->effect_index, args->active_bool);

				return NULL;
			}

			void*		HS_SetEffectInstanceFade(void** arguments)
			{
				struct s_arguments {
					uint16 effect_index;
					PAD16;
					real fade_start;
					real fade_end;
					real fade_time;
				}* args = CAST_PTR(s_arguments*, arguments);

				Generic::Internal::c_system_internal::Instance().SetEffectInstanceFade(args->effect_index, args->fade_start, args->fade_end, args->fade_time);

				return NULL;
			}

			void*		HS_GetEffectInstanceCurrentFade(void** arguments)
			{
				struct s_arguments {
					uint16 effect_index;
					PAD16;
				}* args = CAST_PTR(s_arguments*, arguments);
				TypeHolder result;

				result.real = Generic::Internal::c_system_internal::Instance().GetEffectInstanceCurrentFade(args->effect_index);

				return result.pointer;
			}

			void*		HS_GetEffectInstanceFadeDirection(void** arguments)
			{
				struct s_arguments {
					uint16 effect_index;
					PAD16;
				}* args = CAST_PTR(s_arguments*, arguments);
				TypeHolder result;

				result.int16 = Generic::Internal::c_system_internal::Instance().GetEffectInstanceFadeDirection(args->effect_index);

				return result.pointer;
			}

			void*		HS_GetEffectIndexByName(void** arguments)
			{
				struct s_arguments {
					const char* test_string;
				}* args = CAST_PTR(s_arguments*, arguments);
				TypeHolder result;

				result.uint32 = Generic::Internal::c_system_internal::Instance().GetEffectIndexByName(args->test_string);

				return result.pointer;
			}

			void*		HS_GetEffectIsValid(void** arguments)
			{
				struct s_arguments {
					uint16 effect_index;
					PAD16;
				}* args = CAST_PTR(s_arguments*, arguments);
				TypeHolder result;

				result.boolean = Generic::Internal::c_system_internal::Instance().GetEffectIsValid(args->effect_index);

				return result.pointer;
			}

			void*		HS_GetEffectShaderVariableIndexByName(void** arguments)
			{
				struct s_arguments {
					uint16 effect_index;
					PAD16;
					const char* test_string;
				}* args = CAST_PTR(s_arguments*, arguments);
				TypeHolder result;

				result.int16 = Generic::Internal::c_system_internal::Instance().GetEffectShaderVariableIndexByName(args->effect_index, args->test_string);

				return result.pointer;
			}

			void*		HS_SetEffectShaderVariableBoolean(void** arguments)
			{
				struct s_arguments {
					uint16 effect_index;
					PAD16;
					uint16 script_variable_index;
					PAD16;
					bool value;
					PAD24;
					real interp_time;
				}* args = CAST_PTR(s_arguments*, arguments);

				Generic::Internal::c_system_internal::Instance().SetEffectShaderVariableBoolean(
					args->effect_index,		args->script_variable_index,
					args->value,
					args->interp_time);

				return NULL;
			}

			void*		HS_SetEffectShaderVariableInteger(void** arguments)
			{
				struct s_arguments {
					uint16 effect_index;
					PAD16;
					uint16 script_variable_index;
					PAD16;
					uint32 value;
					real interp_time;
				}* args = CAST_PTR(s_arguments*, arguments);

				Generic::Internal::c_system_internal::Instance().SetEffectShaderVariableInteger(
					args->effect_index,		args->script_variable_index,
					args->value,
					args->interp_time);

				return NULL;
			}

			void*		HS_SetEffectShaderVariableReal(void** arguments)
			{
				struct s_arguments {
					uint16 effect_index;
					PAD16;
					uint16 script_variable_index;
					PAD16;
					real values[4];
					real interp_time;
				}* args = CAST_PTR(s_arguments*, arguments);

				Generic::Internal::c_system_internal::Instance().SetEffectShaderVariableReal(
					args->effect_index,		args->script_variable_index,
					args->values[0],
					args->values[1],
					args->values[2],
					args->values[3],
					args->interp_time);

				return NULL;
			}

			void*		HS_SetEffectShaderInstanceActive(void** arguments)
			{
				struct s_arguments {
					uint16 effect_index;
					PAD16;
					uint16 shader_instance_index;
					PAD16;
					bool active;
				}* args = CAST_PTR(s_arguments*, arguments);

				Generic::Internal::c_system_internal::Instance().SetEffectShaderInstanceActive(
					args->effect_index,
					args->shader_instance_index,
					args->active);

				return NULL;
			}
		};
	};};};
};
#endif