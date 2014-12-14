/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Interface/OpenSauceUI/Screen/c_screen_controller_mainmenu.hpp"

#if !PLATFORM_IS_DEDI

#include "Interface/OpenSauceUI/resource_id.hpp"

#include "Rasterizer/GBuffer.hpp"
#include "Rasterizer/ShaderExtension/ShaderExtension.hpp"
#include "Rasterizer/PostProcessing/MotionBlur/c_system_motionblur.hpp"
#include "Rasterizer/PostProcessing/Bloom/c_system_bloom.hpp"
#include "Rasterizer/PostProcessing/FXAA/c_system_fxaa.hpp"
#include "Rasterizer/PostProcessing/Generic/Internal/c_system_internal.hpp"
#include "Rasterizer/PostProcessing/Generic/External/c_system_external.hpp"

namespace Yelo
{
	namespace Interface { namespace OpenSauceUI { namespace Screen
	{
#pragma region Resource ID Defines
// TEMP
#define K_PROPERTY_TEXT_ID RESOURCE_ID_DEBUG("Text")
#define K_PROPERTY_CHECKED_ID RESOURCE_ID_DEBUG("Checked")
#define K_PROPERTY_DISABLED_ID RESOURCE_ID_DEBUG("Disabled")
#define K_PROPERTY_VALUE_ID RESOURCE_ID_DEBUG("Value")

#define K_EVENT_CHECK_CHANGED_ID RESOURCE_ID_DEBUG("CheckChanged")
#define K_EVENT_VALUE_CHANGED_ID RESOURCE_ID_DEBUG("ValueChanged")
// TEMP

#define K_LBL_GRAPHICS_TITLE_ID									RESOURCE_ID_DEBUG("#LBL_graphics_title")

#define K_LBL_OPENSAUCE_SETTINGS_TITLE_ID						RESOURCE_ID_DEBUG("#LBL_opensauce_settings_title")

#define K_LBL_GENERAL_TITLE_ID									RESOURCE_ID_DEBUG("#LBL_general_title")
#define K_CHK_GENERAL_GBUFFER_ENABLED_ID						RESOURCE_ID_DEBUG("#CHK_general_gbuffer_enabled")
#define K_CHK_GENERAL_GBUFFER_TOGGLE_EVENT_ID					RESOURCE_ID_DEBUG("#CHK_general_gbuffer_toggle_event")
#define K_CHK_GENERAL_NVIDIA_CAMO_ENABLED_ID					RESOURCE_ID_DEBUG("#CHK_general_nvidia_camo_enabled")
#define K_CHK_GENERAL_NVIDIA_CAMO_TOGGLE_EVENT_ID				RESOURCE_ID_DEBUG("#CHK_general_nvidia_camo_toggle_event")

#define K_LBL_ENGINE_UPGRADES_TITLE_ID							RESOURCE_ID_DEBUG("#LBL_engine_upgrades_title")
#define K_CHK_UPGRADES_MAX_RENDERED_TRIANGLES_ENABLED_ID		RESOURCE_ID_DEBUG("#CHK_upgrades_max_rendered_triangles_enabled")
#define K_CHK_UPGRADES_MAX_RENDERED_TRIANGLES_TOGGLE_EVENT_ID	RESOURCE_ID_DEBUG("#CHK_upgrades_max_rendered_triangles_toggle_event")
#define K_CHK_UPGRADES_MAX_BONE_NODES_ENABLED_ID				RESOURCE_ID_DEBUG("#CHK_upgrades_max_bone_nodes_enabled")
#define K_CHK_UPGRADES_MAX_BONE_NODES_TOGGLE_EVENT_ID			RESOURCE_ID_DEBUG("#CHK_upgrades_max_bone_nodes_toggle_event")

#define K_LBL_SHADER_EXTENSION_MODEL_TITLE_ID							RESOURCE_ID_DEBUG("#LBL_shader_extension_model_title")
#define K_CHK_SHADER_EXTENSION_MODEL_NORMAL_MAPS_ENABLED_ID				RESOURCE_ID_DEBUG("#CHK_shader_extension_model_normal_maps_enabled")
#define K_CHK_SHADER_EXTENSION_MODEL_NORMAL_MAPS_TOGGLE_EVENT_ID		RESOURCE_ID_DEBUG("#CHK_shader_extension_model_normal_maps_toggle_event")
#define K_CHK_SHADER_EXTENSION_MODEL_SPECULAR_MAPS_ENABLED_ID			RESOURCE_ID_DEBUG("#CHK_shader_extension_model_specular_maps_enabled")
#define K_CHK_SHADER_EXTENSION_MODEL_SPECULAR_MAPS_TOGGLE_EVENT_ID		RESOURCE_ID_DEBUG("#CHK_shader_extension_model_specular_maps_toggle_event")
#define K_CHK_SHADER_EXTENSION_MODEL_SPECULAR_LIGHTING_ENABLED_ID		RESOURCE_ID_DEBUG("#CHK_shader_extension_model_specular_lighting_enabled")
#define K_CHK_SHADER_EXTENSION_MODEL_SPECULAR_LIGHTING_TOGGLE_EVENT_ID	RESOURCE_ID_DEBUG("#CHK_shader_extension_model_specular_lighting_toggle_event")

#define K_LBL_SHADER_EXTENSION_ENVIRONMENT_TITLE_ID						RESOURCE_ID_DEBUG("#LBL_shader_extension_environment_title")
#define K_CHK_SHADER_EXTENSION_ENVIRONMENT_DIFFUSE_DLM_ENABLED_ID		RESOURCE_ID_DEBUG("#CHK_shader_extension_environment_diffuse_dlm_enabled")
#define K_CHK_SHADER_EXTENSION_ENVIRONMENT_DIFFUSE_DLM_TOGGLE_EVENT_ID	RESOURCE_ID_DEBUG("#CHK_shader_extension_environment_diffuse_dlm_toggle_event")
#define K_CHK_SHADER_EXTENSION_ENVIRONMENT_SPECULAR_DLM_ENABLED_ID		RESOURCE_ID_DEBUG("#CHK_shader_extension_environment_specular_dlm_enabled")
#define K_CHK_SHADER_EXTENSION_ENVIRONMENT_SPECULAR_DLM_TOGGLE_EVENT_ID	RESOURCE_ID_DEBUG("#CHK_shader_extension_environment_specular_dlm_toggle_event")

#define K_LBL_SHADER_EXTENSION_EFFECT_TITLE_ID							RESOURCE_ID_DEBUG("#LBL_shader_extension_effect_title")
#define K_CHK_SHADER_EXTENSION_EFFECT_DEPTH_FADE_ENABLED_ID				RESOURCE_ID_DEBUG("#CHK_shader_extension_effect_depth_fade_enabled")
#define K_CHK_SHADER_EXTENSION_EFFECT_DEPTH_FADE_TOGGLE_EVENT_ID		RESOURCE_ID_DEBUG("#CHK_shader_extension_effect_depth_fade_toggle_event")

#define K_LBL_POST_PROCESSING_EFFECT_TITLE_ID						RESOURCE_ID_DEBUG("#LBL_postprocessing_title")
#define K_CHK_POST_PROCESSING_BLOOM_ENABLED_ID						RESOURCE_ID_DEBUG("#CHK_postprocessing_bloom_enabled")
#define K_CHK_POST_PROCESSING_BLOOM_TOGGLE_EVENT_ID					RESOURCE_ID_DEBUG("#CHK_postprocessing_bloom_toggle_event")
#define K_CHK_POST_PROCESSING_ANTI_ALIASING_ENABLED_ID				RESOURCE_ID_DEBUG("#CHK_postprocessing_anti_aliasing_enabled")
#define K_CHK_POST_PROCESSING_ANTI_ALIASING_TOGGLE_EVENT_ID			RESOURCE_ID_DEBUG("#CHK_postprocessing_anti_aliasing_toggle_event")
#define K_CHK_POST_PROCESSING_EXTERNAL_EFFECTS_ENABLED_ID			RESOURCE_ID_DEBUG("#CHK_postprocessing_external_effects_enabled")
#define K_CHK_POST_PROCESSING_EXTERNAL_EFFECTS_TOGGLE_EVENT_ID		RESOURCE_ID_DEBUG("#CHK_postprocessing_external_effects_toggle_event")
#define K_CHK_POST_PROCESSING_MAP_EFFECTS_ENABLED_ID				RESOURCE_ID_DEBUG("#CHK_postprocessing_map_effects_enabled")
#define K_CHK_POST_PROCESSING_MAP_EFFECTS_TOGGLE_EVENT_ID			RESOURCE_ID_DEBUG("#CHK_postprocessing_map_effects_toggle_event")
#define K_CHK_POST_PROCESSING_MOTIONBLUR_ENABLED_ID					RESOURCE_ID_DEBUG("#CHK_postprocessing_motionblur_enabled")
#define K_CHK_POST_PROCESSING_MOTIONBLUR_TOGGLE_EVENT_ID			RESOURCE_ID_DEBUG("#CHK_postprocessing_motionblur_toggle_event")
#define K_LBL_POST_PROCESSING_MOTIONBLUR_AMOUNT_ID					RESOURCE_ID_DEBUG("#LBL_postprocessing_motionblur_amount")
#define K_LBL_POST_PROCESSING_MOTIONBLUR_AMOUNT_TEXT_ID				RESOURCE_ID_DEBUG("#LBL_postprocessing_motionblur_amount_text")
#define K_SLD_POST_PROCESSING_MOTIONBLUR_AMOUNT_ID					RESOURCE_ID_DEBUG("#SLD_postprocessing_motionblur_amount")
#define K_SLD_POST_PROCESSING_MOTIONBLUR_AMOUNT_CHANGED_EVENT_ID	RESOURCE_ID_DEBUG("#SLD_postprocessing_motionblur_amount_changed_event")
#pragma endregion
		
		c_screen_controller_mainmenu::c_screen_controller_mainmenu(Definitions::c_screen_definition& definition)
			: c_screen_controller_base(definition)
		{ }

		void c_screen_controller_mainmenu::SetStaticProperties()
		{
			// Set all static text strings in the screen
			SetControlProperty(K_LBL_OPENSAUCE_SETTINGS_TITLE_ID,							K_PROPERTY_TEXT_ID, "OpenSauce Settings");

			SetControlProperty(K_LBL_GRAPHICS_TITLE_ID,										K_PROPERTY_TEXT_ID, "Graphics");

			SetControlProperty(K_LBL_GENERAL_TITLE_ID,										K_PROPERTY_TEXT_ID, "General");
			SetControlProperty(K_CHK_GENERAL_GBUFFER_ENABLED_ID,							K_PROPERTY_TEXT_ID, "GBuffer Enabled");
			SetControlProperty(K_CHK_GENERAL_NVIDIA_CAMO_ENABLED_ID,						K_PROPERTY_TEXT_ID, "Use nVidia Camo (requires restart)");

			SetControlProperty(K_LBL_ENGINE_UPGRADES_TITLE_ID,								K_PROPERTY_TEXT_ID, "Engine Upgrades");
			SetControlProperty(K_CHK_UPGRADES_MAX_RENDERED_TRIANGLES_ENABLED_ID,			K_PROPERTY_TEXT_ID, "Increase maximum rendered triangles (requires restart)");
			SetControlProperty(K_CHK_UPGRADES_MAX_BONE_NODES_ENABLED_ID,					K_PROPERTY_TEXT_ID, "Increase maximum model bone count (requires restart)");

			SetControlProperty(K_LBL_SHADER_EXTENSION_MODEL_TITLE_ID,						K_PROPERTY_TEXT_ID, "Shader Extension - Models");
			SetControlProperty(K_CHK_SHADER_EXTENSION_MODEL_NORMAL_MAPS_ENABLED_ID,			K_PROPERTY_TEXT_ID, "Normal Maps Enabled");
			SetControlProperty(K_CHK_SHADER_EXTENSION_MODEL_SPECULAR_MAPS_ENABLED_ID,		K_PROPERTY_TEXT_ID, "Specular Maps Enabled");
			SetControlProperty(K_CHK_SHADER_EXTENSION_MODEL_SPECULAR_LIGHTING_ENABLED_ID,	K_PROPERTY_TEXT_ID, "Specular Lighting Enabled");

			SetControlProperty(K_LBL_SHADER_EXTENSION_ENVIRONMENT_TITLE_ID,					K_PROPERTY_TEXT_ID, "Shader Extension - Environment");
			SetControlProperty(K_CHK_SHADER_EXTENSION_ENVIRONMENT_DIFFUSE_DLM_ENABLED_ID,	K_PROPERTY_TEXT_ID, "Diffuse Directional Lightmaps Enabled");
			SetControlProperty(K_CHK_SHADER_EXTENSION_ENVIRONMENT_SPECULAR_DLM_ENABLED_ID,	K_PROPERTY_TEXT_ID, "Specular Directional Lightmaps Enabled");

			SetControlProperty(K_LBL_SHADER_EXTENSION_EFFECT_TITLE_ID,						K_PROPERTY_TEXT_ID, "Shader Extension - Effects");
			SetControlProperty(K_CHK_SHADER_EXTENSION_EFFECT_DEPTH_FADE_ENABLED_ID,			K_PROPERTY_TEXT_ID, "Depth Fade Enabled");

			SetControlProperty(K_LBL_POST_PROCESSING_EFFECT_TITLE_ID,						K_PROPERTY_TEXT_ID, "Post Processing");
			SetControlProperty(K_CHK_POST_PROCESSING_BLOOM_ENABLED_ID,						K_PROPERTY_TEXT_ID, "Bloom");
			SetControlProperty(K_CHK_POST_PROCESSING_ANTI_ALIASING_ENABLED_ID,				K_PROPERTY_TEXT_ID, "Anti Aliasing");
			SetControlProperty(K_CHK_POST_PROCESSING_EXTERNAL_EFFECTS_ENABLED_ID,			K_PROPERTY_TEXT_ID, "External Effects");
			SetControlProperty(K_CHK_POST_PROCESSING_MAP_EFFECTS_ENABLED_ID,				K_PROPERTY_TEXT_ID, "Map Effects");
			SetControlProperty(K_CHK_POST_PROCESSING_MOTIONBLUR_ENABLED_ID,					K_PROPERTY_TEXT_ID, "Motion Blur");
			SetControlProperty(K_LBL_POST_PROCESSING_MOTIONBLUR_AMOUNT_ID,					K_PROPERTY_TEXT_ID, "Motion Blur Amount");
		}

		void c_screen_controller_mainmenu::BindDynamicProperties()
		{
			// Attach all properties that need to be updated each tick
			AddDynamicProperty(K_CHK_GENERAL_GBUFFER_ENABLED_ID, K_PROPERTY_CHECKED_ID,
				[](Control::i_control& control, Control::i_property_interface& property)
				{
					property.Set(control, Control::s_interface_value(DX9::c_gbuffer_system::g_system_enabled));
				});

			AddDynamicProperty(K_CHK_GENERAL_NVIDIA_CAMO_ENABLED_ID, K_PROPERTY_CHECKED_ID,
				[](Control::i_control& control, Control::i_property_interface& property)
				{
					property.Set(control, Control::s_interface_value(Rasterizer::c_settings_rasterizer::Instance().Get().m_use_nvidia_camo));
				});


			AddDynamicProperty(K_CHK_UPGRADES_MAX_RENDERED_TRIANGLES_ENABLED_ID, K_PROPERTY_CHECKED_ID,
				[](Control::i_control& control, Control::i_property_interface& property)
				{
					property.Set(control, Control::s_interface_value(Rasterizer::c_settings_rasterizer::Instance().Get().m_upgrades.m_maximum_rendered_triangles));
				});

			AddDynamicProperty(K_CHK_UPGRADES_MAX_BONE_NODES_ENABLED_ID, K_PROPERTY_CHECKED_ID,
				[](Control::i_control& control, Control::i_property_interface& property)
				{
					property.Set(control, Control::s_interface_value(Rasterizer::c_settings_rasterizer::Instance().Get().m_upgrades.m_model_node_stretching_fix));
				});


			AddDynamicProperty(K_CHK_SHADER_EXTENSION_MODEL_NORMAL_MAPS_ENABLED_ID, K_PROPERTY_CHECKED_ID,
				[](Control::i_control& control, Control::i_property_interface& property)
				{
					property.Set(control, Control::s_interface_value(Rasterizer::ShaderExtension::Model::GetNormalMapsEnabled()));
				});

			AddDynamicProperty(K_CHK_SHADER_EXTENSION_MODEL_SPECULAR_MAPS_ENABLED_ID, K_PROPERTY_CHECKED_ID,
				[](Control::i_control& control, Control::i_property_interface& property)
				{
					property.Set(control, Control::s_interface_value(Rasterizer::ShaderExtension::Model::GetSpecularMapsEnabled()));
				});

			AddDynamicProperty(K_CHK_SHADER_EXTENSION_MODEL_SPECULAR_LIGHTING_ENABLED_ID, K_PROPERTY_CHECKED_ID,
				[](Control::i_control& control, Control::i_property_interface& property)
				{
					property.Set(control, Control::s_interface_value(Rasterizer::ShaderExtension::Model::GetSpecularLightingEnabled()));
				});


			AddDynamicProperty(K_CHK_SHADER_EXTENSION_ENVIRONMENT_DIFFUSE_DLM_ENABLED_ID, K_PROPERTY_CHECKED_ID,
				[](Control::i_control& control, Control::i_property_interface& property)
				{
					property.Set(control, Control::s_interface_value(Rasterizer::ShaderExtension::Environment::GetDiffuseDLMsEnabled()));
				});
			
			AddDynamicProperty(K_CHK_SHADER_EXTENSION_ENVIRONMENT_SPECULAR_DLM_ENABLED_ID, K_PROPERTY_CHECKED_ID,
				[](Control::i_control& control, Control::i_property_interface& property)
				{
					property.Set(control, Control::s_interface_value(Rasterizer::ShaderExtension::Environment::GetSpecularDLMsEnabled()));
				});

			AddDynamicProperty(K_CHK_SHADER_EXTENSION_ENVIRONMENT_SPECULAR_DLM_ENABLED_ID, K_PROPERTY_DISABLED_ID,
				[](Control::i_control& control, Control::i_property_interface& property)
				{
					property.Set(control, Control::s_interface_value(!Rasterizer::ShaderExtension::Environment::GetDiffuseDLMsEnabled()));
				});


			AddDynamicProperty(K_CHK_SHADER_EXTENSION_EFFECT_DEPTH_FADE_ENABLED_ID, K_PROPERTY_CHECKED_ID,
				[](Control::i_control& control, Control::i_property_interface& property)
				{
					property.Set(control, Control::s_interface_value(Rasterizer::ShaderExtension::Effect::GetDepthFadeEnabled()));
				});
			

			AddDynamicProperty(K_CHK_POST_PROCESSING_BLOOM_ENABLED_ID, K_PROPERTY_CHECKED_ID,
				[](Control::i_control& control, Control::i_property_interface& property)
				{
					property.Set(control, Control::s_interface_value(Rasterizer::PostProcessing::Bloom::c_system_bloom::Instance().Enabled()));
				});

			AddDynamicProperty(K_CHK_POST_PROCESSING_ANTI_ALIASING_ENABLED_ID, K_PROPERTY_CHECKED_ID,
				[](Control::i_control& control, Control::i_property_interface& property)
				{
					property.Set(control, Control::s_interface_value(Rasterizer::PostProcessing::FXAA::c_system_fxaa::Instance().Enabled()));
				});

			AddDynamicProperty(K_CHK_POST_PROCESSING_EXTERNAL_EFFECTS_ENABLED_ID, K_PROPERTY_CHECKED_ID,
				[](Control::i_control& control, Control::i_property_interface& property)
				{
					property.Set(control, Control::s_interface_value(Rasterizer::PostProcessing::Generic::External::c_system_external::Instance().Enabled()));
				});

			AddDynamicProperty(K_CHK_POST_PROCESSING_MAP_EFFECTS_ENABLED_ID, K_PROPERTY_CHECKED_ID,
				[](Control::i_control& control, Control::i_property_interface& property)
				{
					property.Set(control, Control::s_interface_value(Rasterizer::PostProcessing::Generic::Internal::c_system_internal::Instance().Enabled()));
				});

			AddDynamicProperty(K_CHK_POST_PROCESSING_MOTIONBLUR_ENABLED_ID, K_PROPERTY_CHECKED_ID,
				[](Control::i_control& control, Control::i_property_interface& property)
				{
					property.Set(control, Control::s_interface_value(Rasterizer::PostProcessing::MotionBlur::c_system_motionblur::Instance().Enabled()));
				});
			
			AddDynamicProperty(K_SLD_POST_PROCESSING_MOTIONBLUR_AMOUNT_ID, K_PROPERTY_VALUE_ID,
				[](Control::i_control& control, Control::i_property_interface& property)
				{
					property.Set(control, Control::s_interface_value(Rasterizer::PostProcessing::MotionBlur::c_system_motionblur::Instance().BlurAmount()));
				});

			AddDynamicProperty(K_LBL_POST_PROCESSING_MOTIONBLUR_AMOUNT_TEXT_ID, K_PROPERTY_TEXT_ID,
				[](Control::i_control& control, Control::i_property_interface& property)
				{
					char buffer[10] = "";
					sprintf_s(buffer, "%.2f", Rasterizer::PostProcessing::MotionBlur::c_system_motionblur::Instance().BlurAmount());

					Control::s_interface_value value;
					value.SetString(buffer);

					property.Set(control, value);
				});
		}

		void c_screen_controller_mainmenu::BindEvents()
		{
			// Attach all events that the screen has to react to
			AttachEvent(K_CHK_GENERAL_GBUFFER_ENABLED_ID, K_EVENT_CHECK_CHANGED_ID, K_CHK_GENERAL_GBUFFER_TOGGLE_EVENT_ID, nullptr,
				[](const Control::s_interface_value& event_data, void* userdata)
				{
					DX9::c_gbuffer_system::g_system_enabled = event_data.m_bool;
				});
			
			AttachEvent(K_CHK_GENERAL_NVIDIA_CAMO_ENABLED_ID, K_EVENT_CHECK_CHANGED_ID, K_CHK_GENERAL_NVIDIA_CAMO_TOGGLE_EVENT_ID, nullptr,
				[](const Control::s_interface_value& event_data, void* userdata)
				{
					Rasterizer::c_settings_rasterizer::Instance().Get().m_use_nvidia_camo = event_data.m_bool;
				});
			

			AttachEvent(K_CHK_UPGRADES_MAX_RENDERED_TRIANGLES_ENABLED_ID, K_EVENT_CHECK_CHANGED_ID, K_CHK_UPGRADES_MAX_RENDERED_TRIANGLES_TOGGLE_EVENT_ID, nullptr,
				[](const Control::s_interface_value& event_data, void* userdata)
				{
					Rasterizer::c_settings_rasterizer::Instance().Get().m_upgrades.m_maximum_rendered_triangles = event_data.m_bool;
				});

			AttachEvent(K_CHK_UPGRADES_MAX_BONE_NODES_ENABLED_ID, K_EVENT_CHECK_CHANGED_ID, K_CHK_UPGRADES_MAX_BONE_NODES_TOGGLE_EVENT_ID, nullptr,
				[](const Control::s_interface_value& event_data, void* userdata)
				{
					Rasterizer::c_settings_rasterizer::Instance().Get().m_upgrades.m_model_node_stretching_fix = event_data.m_bool;
				});
			

			AttachEvent(K_CHK_SHADER_EXTENSION_MODEL_NORMAL_MAPS_ENABLED_ID, K_EVENT_CHECK_CHANGED_ID, K_CHK_SHADER_EXTENSION_MODEL_NORMAL_MAPS_TOGGLE_EVENT_ID, nullptr,
				[](const Control::s_interface_value& event_data, void* userdata)
				{
					Rasterizer::ShaderExtension::Model::SetNormalMapsEnabled(event_data.m_bool);
				});

			AttachEvent(K_CHK_SHADER_EXTENSION_MODEL_SPECULAR_MAPS_ENABLED_ID, K_EVENT_CHECK_CHANGED_ID, K_CHK_SHADER_EXTENSION_MODEL_SPECULAR_MAPS_TOGGLE_EVENT_ID, nullptr,
				[](const Control::s_interface_value& event_data, void* userdata)
				{
					Rasterizer::ShaderExtension::Model::SetSpecularMapsEnabled(event_data.m_bool);
				});

			AttachEvent(K_CHK_SHADER_EXTENSION_MODEL_SPECULAR_LIGHTING_ENABLED_ID, K_EVENT_CHECK_CHANGED_ID, K_CHK_SHADER_EXTENSION_MODEL_SPECULAR_LIGHTING_TOGGLE_EVENT_ID, nullptr,
				[](const Control::s_interface_value& event_data, void* userdata)
				{
					Rasterizer::ShaderExtension::Model::SetSpecularLightingEnabled(event_data.m_bool);
				});


			AttachEvent(K_CHK_SHADER_EXTENSION_ENVIRONMENT_DIFFUSE_DLM_ENABLED_ID, K_EVENT_CHECK_CHANGED_ID, K_CHK_SHADER_EXTENSION_ENVIRONMENT_DIFFUSE_DLM_TOGGLE_EVENT_ID, nullptr,
				[](const Control::s_interface_value& event_data, void* userdata)
				{
					Rasterizer::ShaderExtension::Environment::SetDiffuseDLMsEnabled(event_data.m_bool);
				});
			
			AttachEvent(K_CHK_SHADER_EXTENSION_ENVIRONMENT_SPECULAR_DLM_ENABLED_ID, K_EVENT_CHECK_CHANGED_ID, K_CHK_SHADER_EXTENSION_ENVIRONMENT_SPECULAR_DLM_TOGGLE_EVENT_ID, nullptr,
				[](const Control::s_interface_value& event_data, void* userdata)
				{
					Rasterizer::ShaderExtension::Environment::SetSpecularDLMsEnabled(event_data.m_bool);
				});


			AttachEvent(K_CHK_SHADER_EXTENSION_EFFECT_DEPTH_FADE_ENABLED_ID, K_EVENT_CHECK_CHANGED_ID, K_CHK_SHADER_EXTENSION_EFFECT_DEPTH_FADE_TOGGLE_EVENT_ID, nullptr,
				[](const Control::s_interface_value& event_data, void* userdata)
				{
					Rasterizer::ShaderExtension::Effect::SetDepthFadeEnabled(event_data.m_bool);
				});
			

			AttachEvent(K_CHK_POST_PROCESSING_BLOOM_ENABLED_ID, K_EVENT_CHECK_CHANGED_ID, K_CHK_POST_PROCESSING_BLOOM_TOGGLE_EVENT_ID, nullptr,
				[](const Control::s_interface_value& event_data, void* userdata)
				{
					Rasterizer::PostProcessing::Bloom::c_system_bloom::Instance().Enabled() = event_data.m_bool;
				});

			AttachEvent(K_CHK_POST_PROCESSING_ANTI_ALIASING_ENABLED_ID, K_EVENT_CHECK_CHANGED_ID, K_CHK_POST_PROCESSING_ANTI_ALIASING_TOGGLE_EVENT_ID, nullptr,
				[](const Control::s_interface_value& event_data, void* userdata)
				{
					Rasterizer::PostProcessing::FXAA::c_system_fxaa::Instance().Enabled() = event_data.m_bool;
				});

			AttachEvent(K_CHK_POST_PROCESSING_EXTERNAL_EFFECTS_ENABLED_ID, K_EVENT_CHECK_CHANGED_ID, K_CHK_POST_PROCESSING_EXTERNAL_EFFECTS_TOGGLE_EVENT_ID, nullptr,
				[](const Control::s_interface_value& event_data, void* userdata)
				{
					Rasterizer::PostProcessing::Generic::External::c_system_external::Instance().Enabled() = event_data.m_bool;
				});

			AttachEvent(K_CHK_POST_PROCESSING_MAP_EFFECTS_ENABLED_ID, K_EVENT_CHECK_CHANGED_ID, K_CHK_POST_PROCESSING_MAP_EFFECTS_TOGGLE_EVENT_ID, nullptr,
				[](const Control::s_interface_value& event_data, void* userdata)
				{
					Rasterizer::PostProcessing::Generic::Internal::c_system_internal::Instance().Enabled() = event_data.m_bool;
				});

			AttachEvent(K_CHK_POST_PROCESSING_MOTIONBLUR_ENABLED_ID, K_EVENT_CHECK_CHANGED_ID, K_CHK_POST_PROCESSING_MOTIONBLUR_TOGGLE_EVENT_ID, nullptr,
				[](const Control::s_interface_value& event_data, void* userdata)
				{
					auto& motionblur_instance = Rasterizer::PostProcessing::MotionBlur::c_system_motionblur::Instance();

					motionblur_instance.Enabled() = event_data.m_bool;

					if((event_data.m_bool == true) && (motionblur_instance.BlurAmount() == 0.0f))
					{
						motionblur_instance.BlurAmount() = 1.0f;
					}
				});

			AttachEvent(K_SLD_POST_PROCESSING_MOTIONBLUR_AMOUNT_ID, K_EVENT_VALUE_CHANGED_ID, K_SLD_POST_PROCESSING_MOTIONBLUR_AMOUNT_CHANGED_EVENT_ID, nullptr,
				[](const Control::s_interface_value& event_data, void* userdata)
				{
					auto& motionblur_instance = Rasterizer::PostProcessing::MotionBlur::c_system_motionblur::Instance();

					motionblur_instance.BlurAmount() = event_data.m_real;

					if(motionblur_instance.BlurAmount() == 0.0f)
					{
						motionblur_instance.Enabled() = false;
					}
					else
					{
						motionblur_instance.Enabled() = true;
					}
				});
		}

		void c_screen_controller_mainmenu::UnbindEvents()
		{
			// Remove all bounds events
			DetachEvent(K_CHK_GENERAL_GBUFFER_ENABLED_ID, K_EVENT_CHECK_CHANGED_ID, K_CHK_GENERAL_GBUFFER_TOGGLE_EVENT_ID);
			DetachEvent(K_CHK_GENERAL_NVIDIA_CAMO_ENABLED_ID, K_EVENT_CHECK_CHANGED_ID, K_CHK_GENERAL_NVIDIA_CAMO_TOGGLE_EVENT_ID);

			DetachEvent(K_CHK_UPGRADES_MAX_RENDERED_TRIANGLES_ENABLED_ID, K_EVENT_CHECK_CHANGED_ID, K_CHK_UPGRADES_MAX_RENDERED_TRIANGLES_TOGGLE_EVENT_ID);
			DetachEvent(K_CHK_UPGRADES_MAX_BONE_NODES_ENABLED_ID, K_EVENT_CHECK_CHANGED_ID, K_CHK_UPGRADES_MAX_BONE_NODES_TOGGLE_EVENT_ID);

			DetachEvent(K_CHK_SHADER_EXTENSION_MODEL_NORMAL_MAPS_ENABLED_ID, K_EVENT_CHECK_CHANGED_ID, K_CHK_SHADER_EXTENSION_MODEL_NORMAL_MAPS_TOGGLE_EVENT_ID);
			DetachEvent(K_CHK_SHADER_EXTENSION_MODEL_SPECULAR_MAPS_ENABLED_ID, K_EVENT_CHECK_CHANGED_ID, K_CHK_SHADER_EXTENSION_MODEL_SPECULAR_MAPS_TOGGLE_EVENT_ID);
			DetachEvent(K_CHK_SHADER_EXTENSION_MODEL_SPECULAR_LIGHTING_ENABLED_ID, K_EVENT_CHECK_CHANGED_ID, K_CHK_SHADER_EXTENSION_MODEL_SPECULAR_LIGHTING_TOGGLE_EVENT_ID);

			DetachEvent(K_CHK_SHADER_EXTENSION_ENVIRONMENT_DIFFUSE_DLM_ENABLED_ID, K_EVENT_CHECK_CHANGED_ID, K_CHK_SHADER_EXTENSION_ENVIRONMENT_DIFFUSE_DLM_TOGGLE_EVENT_ID);
			DetachEvent(K_CHK_SHADER_EXTENSION_ENVIRONMENT_SPECULAR_DLM_ENABLED_ID, K_EVENT_CHECK_CHANGED_ID, K_CHK_SHADER_EXTENSION_ENVIRONMENT_SPECULAR_DLM_TOGGLE_EVENT_ID);

			DetachEvent(K_CHK_SHADER_EXTENSION_EFFECT_DEPTH_FADE_ENABLED_ID, K_EVENT_CHECK_CHANGED_ID, K_CHK_SHADER_EXTENSION_EFFECT_DEPTH_FADE_TOGGLE_EVENT_ID);

			DetachEvent(K_CHK_POST_PROCESSING_BLOOM_ENABLED_ID, K_EVENT_CHECK_CHANGED_ID, K_CHK_POST_PROCESSING_BLOOM_TOGGLE_EVENT_ID);
			DetachEvent(K_CHK_POST_PROCESSING_ANTI_ALIASING_ENABLED_ID, K_EVENT_CHECK_CHANGED_ID, K_CHK_POST_PROCESSING_ANTI_ALIASING_TOGGLE_EVENT_ID);
			DetachEvent(K_CHK_POST_PROCESSING_EXTERNAL_EFFECTS_ENABLED_ID, K_EVENT_CHECK_CHANGED_ID, K_CHK_POST_PROCESSING_EXTERNAL_EFFECTS_TOGGLE_EVENT_ID);
			DetachEvent(K_CHK_POST_PROCESSING_MAP_EFFECTS_ENABLED_ID, K_EVENT_CHECK_CHANGED_ID, K_CHK_POST_PROCESSING_MAP_EFFECTS_TOGGLE_EVENT_ID);
			DetachEvent(K_CHK_POST_PROCESSING_MOTIONBLUR_ENABLED_ID, K_EVENT_CHECK_CHANGED_ID, K_CHK_POST_PROCESSING_MOTIONBLUR_TOGGLE_EVENT_ID);
			DetachEvent(K_SLD_POST_PROCESSING_MOTIONBLUR_AMOUNT_ID, K_EVENT_VALUE_CHANGED_ID, K_SLD_POST_PROCESSING_MOTIONBLUR_AMOUNT_CHANGED_EVENT_ID);
		}
	};};};
};
#endif