/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Interface/OpenSauceUI/Screen/c_screen_controller_mainmenu.hpp"

#if !PLATFORM_IS_DEDI

#include <YeloLib/files/files.hpp>

#include "Interface/OpenSauceUI/Control/control_property_ids.hpp"
#include "Interface/OpenSauceUI/Control/control_event_ids.hpp"

#include "Rasterizer/DX9/DX9.hpp"
#include "Rasterizer/GBuffer.hpp"
#include "Rasterizer/ShaderExtension/ShaderExtension.hpp"
#include "Rasterizer/PostProcessing/MotionBlur/c_system_motionblur.hpp"
#include "Rasterizer/PostProcessing/Bloom/c_system_bloom.hpp"
#include "Rasterizer/PostProcessing/FXAA/c_system_fxaa.hpp"
#include "Rasterizer/PostProcessing/Generic/Internal/c_system_internal.hpp"
#include "Rasterizer/PostProcessing/Generic/External/c_system_external.hpp"
#include "Networking/HTTP/MapDownloadClient.hpp"
#include "Networking/GameSpyApi.hpp"
#include "TagGroups/CacheFiles.hpp"
#include "Game/Camera.hpp"

namespace Yelo
{
	namespace Interface { namespace OpenSauceUI { namespace Screen
	{
#pragma region Resource ID Defines
#define K_LBL_GRAPHICS_TITLE_ID											RESOURCE_ID("#LBL_graphics_title",											0xFC2C7265)

#define K_LBL_OPENSAUCE_SETTINGS_TITLE_ID								RESOURCE_ID("#LBL_opensauce_settings_title",								0x554F0054)

#define K_LBL_GENERAL_TITLE_ID											RESOURCE_ID("#LBL_general_title",											0x27193476)
#define K_CHK_GENERAL_GBUFFER_ENABLED_ID								RESOURCE_ID("#CHK_general_gbuffer_enabled",									0x80E613DB)
#define K_CHK_GENERAL_GBUFFER_TOGGLE_EVENT_ID							RESOURCE_ID("#CHK_general_gbuffer_toggle_event",							0x6ECC34F2)

#define K_LBL_ENGINE_UPGRADES_TITLE_ID									RESOURCE_ID("#LBL_engine_upgrades_title",									0xF20B71EF)
#define K_CHK_UPGRADES_MAX_RENDERED_TRIANGLES_ENABLED_ID				RESOURCE_ID("#CHK_upgrades_max_rendered_triangles_enabled",					0x723FAEB8)
#define K_CHK_UPGRADES_MAX_RENDERED_TRIANGLES_TOGGLE_EVENT_ID			RESOURCE_ID("#CHK_upgrades_max_rendered_triangles_toggle_event",			0xFB9422A8)
		
#define K_LBL_CAMERA_TITLE_ID											RESOURCE_ID("#LBL_camera_title",											0x3542AB46)
#define K_LBL_FIELD_OF_VIEW_LABEL_ID									RESOURCE_ID("#LBL_field_of_view_label",										0x4DBA3CC6)
#define K_LBL_FIELD_OF_VIEW_AMOUNT_TEXT_ID								RESOURCE_ID("#LBL_field_of_view_amount_text",								0xE3D3BC2A)
#define K_SLD_FIELD_OF_VIEW_ID											RESOURCE_ID("#SLD_field_of_view",											0xCB8709CA)
#define K_SLD_FIELD_OF_VIEW_AMOUNT_CHANGED_EVENT_ID						RESOURCE_ID("#SLD_field_of_view_amount_changed_event",						0x1B731E17)
#define K_CHK_IGNORE_FOV_IN_CINEMATICS_ENABLED_ID						RESOURCE_ID("#CHK_ignore_fov_in_cinematics_enabled",						0x873A7F15)
#define K_CHK_IGNORE_FOV_IN_CINEMATICS_TOGGLE_EVENT_ID					RESOURCE_ID("#CHK_ignore_fov_in_cinematics_toggle_event",					0x82CB7060)
#define K_CHK_IGNORE_FOV_IN_MAIN_MENU_ENABLED_ID						RESOURCE_ID("#CHK_ignore_fov_in_main_menu_enabled",							0x1F5A29BD)
#define K_CHK_IGNORE_FOV_IN_MAIN_MENU_TOGGLE_EVENT_ID					RESOURCE_ID("#CHK_ignore_fov_in_main_menu_toggle_event",					0xDDB53F7B)

#define K_LBL_SHADER_EXTENSION_MODEL_TITLE_ID							RESOURCE_ID("#LBL_shader_extension_model_title",							0xA7558801)
#define K_CHK_SHADER_EXTENSION_MODEL_NORMAL_MAPS_ENABLED_ID				RESOURCE_ID("#CHK_shader_extension_model_normal_maps_enabled",				0x4209AEA9)
#define K_CHK_SHADER_EXTENSION_MODEL_NORMAL_MAPS_TOGGLE_EVENT_ID		RESOURCE_ID("#CHK_shader_extension_model_normal_maps_toggle_event",			0x59B4334B)
#define K_CHK_SHADER_EXTENSION_MODEL_DETAIL_NORMAL_MAPS_ENABLED_ID		RESOURCE_ID("#CHK_shader_extension_model_detail_normal_maps_enabled",		0x33CC6A16)
#define K_CHK_SHADER_EXTENSION_MODEL_DETAIL_NORMAL_MAPS_TOGGLE_EVENT_ID	RESOURCE_ID("#CHK_shader_extension_model_detail_normal_maps_toggle_event",	0x934CC9C3)
#define K_CHK_SHADER_EXTENSION_MODEL_SPECULAR_MAPS_ENABLED_ID			RESOURCE_ID("#CHK_shader_extension_model_specular_maps_enabled",			0x4E3180FF)
#define K_CHK_SHADER_EXTENSION_MODEL_SPECULAR_MAPS_TOGGLE_EVENT_ID		RESOURCE_ID("#CHK_shader_extension_model_specular_maps_toggle_event",		0x28C72A09)
#define K_CHK_SHADER_EXTENSION_MODEL_SPECULAR_LIGHTING_ENABLED_ID		RESOURCE_ID("#CHK_shader_extension_model_specular_lighting_enabled",		0xC37EA493)
#define K_CHK_SHADER_EXTENSION_MODEL_SPECULAR_LIGHTING_TOGGLE_EVENT_ID	RESOURCE_ID("#CHK_shader_extension_model_specular_lighting_toggle_event",	0xAEBEA4D7)

#define K_LBL_SHADER_EXTENSION_ENVIRONMENT_TITLE_ID						RESOURCE_ID("#LBL_shader_extension_environment_title",						0x62040FE6)
#define K_CHK_SHADER_EXTENSION_ENVIRONMENT_DIFFUSE_DLM_ENABLED_ID		RESOURCE_ID("#CHK_shader_extension_environment_diffuse_dlm_enabled",		0xDA1A6134)
#define K_CHK_SHADER_EXTENSION_ENVIRONMENT_DIFFUSE_DLM_TOGGLE_EVENT_ID	RESOURCE_ID("#CHK_shader_extension_environment_diffuse_dlm_toggle_event",	0xF9F43381)
#define K_CHK_SHADER_EXTENSION_ENVIRONMENT_SPECULAR_DLM_ENABLED_ID		RESOURCE_ID("#CHK_shader_extension_environment_specular_dlm_enabled",		0xF0989847)
#define K_CHK_SHADER_EXTENSION_ENVIRONMENT_SPECULAR_DLM_TOGGLE_EVENT_ID	RESOURCE_ID("#CHK_shader_extension_environment_specular_dlm_toggle_event",	0x3132F375)

#define K_LBL_SHADER_EXTENSION_EFFECT_TITLE_ID							RESOURCE_ID("#LBL_shader_extension_effect_title",							0x61CB98CD)
#define K_CHK_SHADER_EXTENSION_EFFECT_DEPTH_FADE_ENABLED_ID				RESOURCE_ID("#CHK_shader_extension_effect_depth_fade_enabled",				0x9D3EC452)
#define K_CHK_SHADER_EXTENSION_EFFECT_DEPTH_FADE_TOGGLE_EVENT_ID		RESOURCE_ID("#CHK_shader_extension_effect_depth_fade_toggle_event",			0x154F6FA5)

#define K_LBL_POST_PROCESSING_EFFECT_TITLE_ID							RESOURCE_ID("#LBL_postprocessing_title",									0xAF475376)
#define K_CHK_POST_PROCESSING_BLOOM_ENABLED_ID							RESOURCE_ID("#CHK_postprocessing_bloom_enabled",							0x69B34F07)
#define K_CHK_POST_PROCESSING_BLOOM_TOGGLE_EVENT_ID						RESOURCE_ID("#CHK_postprocessing_bloom_toggle_event",						0x46D9B24E)
#define K_CHK_POST_PROCESSING_ANTI_ALIASING_ENABLED_ID					RESOURCE_ID("#CHK_postprocessing_anti_aliasing_enabled",					0x9A3E351F)
#define K_CHK_POST_PROCESSING_ANTI_ALIASING_TOGGLE_EVENT_ID				RESOURCE_ID("#CHK_postprocessing_anti_aliasing_toggle_event",				0xC4B71458)
#define K_CHK_POST_PROCESSING_EXTERNAL_EFFECTS_ENABLED_ID				RESOURCE_ID("#CHK_postprocessing_external_effects_enabled",					0xEA8FDE98)
#define K_CHK_POST_PROCESSING_EXTERNAL_EFFECTS_TOGGLE_EVENT_ID			RESOURCE_ID("#CHK_postprocessing_external_effects_toggle_event",			0x2DAC5332)
#define K_CHK_POST_PROCESSING_MAP_EFFECTS_ENABLED_ID					RESOURCE_ID("#CHK_postprocessing_map_effects_enabled",						0x0FCFC0F2)
#define K_CHK_POST_PROCESSING_MAP_EFFECTS_TOGGLE_EVENT_ID				RESOURCE_ID("#CHK_postprocessing_map_effects_toggle_event",					0x1D7B961B)
#define K_CHK_POST_PROCESSING_MOTIONBLUR_ENABLED_ID						RESOURCE_ID("#CHK_postprocessing_motionblur_enabled",						0xE67D103C)
#define K_CHK_POST_PROCESSING_MOTIONBLUR_TOGGLE_EVENT_ID				RESOURCE_ID("#CHK_postprocessing_motionblur_toggle_event",					0x246C1BE3)
#define K_LBL_POST_PROCESSING_MOTIONBLUR_AMOUNT_ID						RESOURCE_ID("#LBL_postprocessing_motionblur_amount",						0x31B0586C)
#define K_LBL_POST_PROCESSING_MOTIONBLUR_AMOUNT_TEXT_ID					RESOURCE_ID("#LBL_postprocessing_motionblur_amount_text",					0x37D6937F)
#define K_SLD_POST_PROCESSING_MOTIONBLUR_AMOUNT_ID						RESOURCE_ID("#SLD_postprocessing_motionblur_amount",						0x0510FCF0)
#define K_SLD_POST_PROCESSING_MOTIONBLUR_AMOUNT_CHANGED_EVENT_ID		RESOURCE_ID("#SLD_postprocessing_motionblur_amount_changed_event",			0x28E4475C)

#define K_LBL_INTERNET_TITLE_ID											RESOURCE_ID("#LBL_internet_title",											0x05358140)
		
#define K_CHK_INTERNET_CHECK_FOR_GAME_UPDATES_ENABLED_ID				RESOURCE_ID("#CHK_check_for_game_updates_enabled",							0xCE3A5AD3)
#define K_CHK_INTERNET_CHECK_FOR_GAME_UPDATES_TOGGLE_EVENT_ID			RESOURCE_ID("#CHK_check_for_game_updates_toggle_event",						0x52C9C65B)
#define K_CHK_INTERNET_MAP_DOWNLOADING_ENABLED_ID						RESOURCE_ID("#CHK_map_downloading_enabled",									0x12B0A2A4)
#define K_CHK_INTERNET_MAP_DOWNLOADING_TOGGLE_EVENT_ID					RESOURCE_ID("#CHK_map_downloading_toggle_event",							0x9BC6D089)

#define K_LBL_MAP_FILES_TITLE_ID										RESOURCE_ID("#LBL_map_files_title",											0x078F731E)

#define K_CHK_MAP_FILES_CHECK_FOR_YELO_FIRST_ENABLED_ID					RESOURCE_ID("#CHK_check_for_yelo_first_enabled",							0x5CB50DDF)
#define K_CHK_MAP_FILES_CHECK_FOR_YELO_FIRST_TOGGLE_EVENT_ID			RESOURCE_ID("#CHK_check_for_yelo_first_toggle_event",						0x68FDA864)
#pragma endregion
		
		c_screen_controller_mainmenu::c_screen_controller_mainmenu(Definitions::c_screen_definition& definition)
			: c_screen_controller_base(definition)
		{ }

		void c_screen_controller_mainmenu::SetStaticProperties()
		{
			// Set the screen to be central
			AnchorScreen(
				(screen_anchor)(_screen_anchor_center_v | _screen_anchor_center_h),
				(screen_anchor)(_screen_anchor_center_v | _screen_anchor_center_h));

			// Set all static text strings in the screen
			SetControlProperty(K_LBL_OPENSAUCE_SETTINGS_TITLE_ID,							K_PROPERTY_TEXT_ID, "OpenSauce Settings");

			SetControlProperty(K_LBL_GRAPHICS_TITLE_ID,										K_PROPERTY_TEXT_ID, "Graphics");

			SetControlProperty(K_LBL_GENERAL_TITLE_ID,										K_PROPERTY_TEXT_ID, "General");
			SetControlProperty(K_CHK_GENERAL_GBUFFER_ENABLED_ID,							K_PROPERTY_TEXT_ID, "GBuffer Enabled");

			SetControlProperty(K_LBL_ENGINE_UPGRADES_TITLE_ID,								K_PROPERTY_TEXT_ID, "Engine Upgrades (requires restart)");
			SetControlProperty(K_CHK_UPGRADES_MAX_RENDERED_TRIANGLES_ENABLED_ID,			K_PROPERTY_TEXT_ID, "Increase maximum rendered triangles");

			SetControlProperty(K_LBL_CAMERA_TITLE_ID,										K_PROPERTY_TEXT_ID, "Camera");
			SetControlProperty(K_LBL_FIELD_OF_VIEW_LABEL_ID,								K_PROPERTY_TEXT_ID, "Field Of View");
			SetControlProperty(K_CHK_IGNORE_FOV_IN_CINEMATICS_ENABLED_ID,					K_PROPERTY_TEXT_ID, "Ignore FOV change in cinematics");
			SetControlProperty(K_CHK_IGNORE_FOV_IN_MAIN_MENU_ENABLED_ID,					K_PROPERTY_TEXT_ID, "Ignore FOV change in the main menu");

			SetControlProperty(K_LBL_SHADER_EXTENSION_MODEL_TITLE_ID,						K_PROPERTY_TEXT_ID, "Shader Extension - Models");
			SetControlProperty(K_CHK_SHADER_EXTENSION_MODEL_NORMAL_MAPS_ENABLED_ID,			K_PROPERTY_TEXT_ID, "Normal Maps Enabled");
			SetControlProperty(K_CHK_SHADER_EXTENSION_MODEL_DETAIL_NORMAL_MAPS_ENABLED_ID,	K_PROPERTY_TEXT_ID, "Detail Normal Maps Enabled");
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

			SetControlProperty(K_LBL_INTERNET_TITLE_ID,										K_PROPERTY_TEXT_ID, "Internet");

			SetControlProperty(K_CHK_INTERNET_CHECK_FOR_GAME_UPDATES_ENABLED_ID,			K_PROPERTY_TEXT_ID, "Check for game updates (requires restart)");
			SetControlProperty(K_CHK_INTERNET_MAP_DOWNLOADING_ENABLED_ID,					K_PROPERTY_TEXT_ID, "Map Downloading");

			SetControlProperty(K_LBL_MAP_FILES_TITLE_ID,									K_PROPERTY_TEXT_ID, "Map Files");

			SetControlProperty(K_CHK_MAP_FILES_CHECK_FOR_YELO_FIRST_ENABLED_ID,				K_PROPERTY_TEXT_ID, "Look for .yelo maps first (requires restart)");
		}

		void c_screen_controller_mainmenu::BindDynamicProperties()
		{
			// Attach all properties that need to be updated each tick
			AddDynamicProperty(K_CHK_GENERAL_GBUFFER_ENABLED_ID, K_PROPERTY_CHECKED_ID,
				[](Control::i_control& control, Control::i_property_interface& property)
				{
					property.Set(control, Control::s_interface_value(DX9::c_gbuffer_system::g_system_enabled));
				});


			AddDynamicProperty(K_CHK_UPGRADES_MAX_RENDERED_TRIANGLES_ENABLED_ID, K_PROPERTY_CHECKED_ID,
				[](Control::i_control& control, Control::i_property_interface& property)
				{
					property.Set(control, Control::s_interface_value(Rasterizer::c_settings_rasterizer::Instance().Get().m_upgrades.m_maximum_rendered_triangles));
				});


			AddDynamicProperty(K_LBL_FIELD_OF_VIEW_AMOUNT_TEXT_ID, K_PROPERTY_TEXT_ID,
				[](Control::i_control& control, Control::i_property_interface& property)
				{
					char buffer[10] = "";
					sprintf_s(buffer, "%.0f", Fov::GetFieldOfView());

					property.Set(control, Control::s_interface_value(buffer));
				});

			AddDynamicProperty(K_SLD_FIELD_OF_VIEW_ID, K_PROPERTY_VALUE_ID,
				[](Control::i_control& control, Control::i_property_interface& property)
				{
					property.Set(control, Control::s_interface_value(Fov::GetFieldOfView()));
				});

			AddDynamicProperty(K_CHK_IGNORE_FOV_IN_CINEMATICS_ENABLED_ID, K_PROPERTY_CHECKED_ID,
				[](Control::i_control& control, Control::i_property_interface& property)
				{
					property.Set(control, Control::s_interface_value(Fov::c_settings_fov::Instance()->m_ignore_fov_change_in_cinematics));
				});

			AddDynamicProperty(K_CHK_IGNORE_FOV_IN_MAIN_MENU_ENABLED_ID, K_PROPERTY_CHECKED_ID,
				[](Control::i_control& control, Control::i_property_interface& property)
				{
					property.Set(control, Control::s_interface_value(Fov::c_settings_fov::Instance()->m_ignore_fov_change_in_main_menu));
				});


			AddDynamicProperty(K_CHK_SHADER_EXTENSION_MODEL_NORMAL_MAPS_ENABLED_ID, K_PROPERTY_CHECKED_ID,
				[](Control::i_control& control, Control::i_property_interface& property)
				{
					property.Set(control, Control::s_interface_value(Rasterizer::ShaderExtension::Model::GetNormalMapsEnabled()));
				});

			AddDynamicProperty(K_CHK_SHADER_EXTENSION_MODEL_DETAIL_NORMAL_MAPS_ENABLED_ID, K_PROPERTY_CHECKED_ID,
				[](Control::i_control& control, Control::i_property_interface& property)
				{
					property.Set(control, Control::s_interface_value(Rasterizer::ShaderExtension::Model::GetDetailNormalMapsEnabled()));
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

					property.Set(control, Control::s_interface_value(buffer));
				});


			AddDynamicProperty(K_CHK_INTERNET_CHECK_FOR_GAME_UPDATES_ENABLED_ID, K_PROPERTY_CHECKED_ID,
				[](Control::i_control& control, Control::i_property_interface& property)
				{
					property.Set(control, Control::s_interface_value(!Networking::GameSpy::c_settings_gamespy::Instance()->m_no_update_check));
				});

			AddDynamicProperty(K_CHK_INTERNET_MAP_DOWNLOADING_ENABLED_ID, K_PROPERTY_CHECKED_ID,
				[](Control::i_control& control, Control::i_property_interface& property)
				{
					property.Set(control, Control::s_interface_value(Networking::HTTP::Client::MapDownload::c_settings_mapdownload::Instance()->m_enabled));
				});


			AddDynamicProperty(K_CHK_MAP_FILES_CHECK_FOR_YELO_FIRST_ENABLED_ID, K_PROPERTY_CHECKED_ID,
				[](Control::i_control& control, Control::i_property_interface& property)
				{
					property.Set(control, Control::s_interface_value(Cache::c_settings_cache::Instance()->m_check_yelo_files_first));
				});
		}

		void c_screen_controller_mainmenu::BindEvents()
		{
			// Attach all events that the screen has to react to
			AttachEvent(K_CHK_GENERAL_GBUFFER_ENABLED_ID, K_EVENT_CHECKCHANGED_ID, K_CHK_GENERAL_GBUFFER_TOGGLE_EVENT_ID, nullptr,
				[](const Control::s_interface_value& event_data, void* userdata)
				{
					DX9::c_gbuffer_system::g_system_enabled = event_data.m_bool;

					if(!event_data.m_bool)
					{
						Rasterizer::ShaderExtension::Effect::SetDepthFadeEnabled(false);
						Rasterizer::PostProcessing::MotionBlur::c_system_motionblur::Instance().Enabled() = false;
					}
				});


			AttachEvent(K_CHK_UPGRADES_MAX_RENDERED_TRIANGLES_ENABLED_ID, K_EVENT_CHECKCHANGED_ID, K_CHK_UPGRADES_MAX_RENDERED_TRIANGLES_TOGGLE_EVENT_ID, nullptr,
				[](const Control::s_interface_value& event_data, void* userdata)
				{
					Rasterizer::c_settings_rasterizer::Instance().Get().m_upgrades.m_maximum_rendered_triangles = event_data.m_bool;
				});


			AttachEvent(K_SLD_FIELD_OF_VIEW_ID, K_EVENT_VALUECHANGED_ID, K_SLD_FIELD_OF_VIEW_AMOUNT_CHANGED_EVENT_ID, nullptr,
				[](const Control::s_interface_value& event_data, void* userdata)
				{
					Fov::SetFieldOfView(event_data.m_real);
				});

			AttachEvent(K_CHK_IGNORE_FOV_IN_CINEMATICS_ENABLED_ID, K_EVENT_CHECKCHANGED_ID, K_CHK_IGNORE_FOV_IN_CINEMATICS_TOGGLE_EVENT_ID, nullptr,
				[](const Control::s_interface_value& event_data, void* userdata)
				{
					Fov::c_settings_fov::Instance()->m_ignore_fov_change_in_cinematics = event_data.m_bool;
				});

			AttachEvent(K_CHK_IGNORE_FOV_IN_MAIN_MENU_ENABLED_ID, K_EVENT_CHECKCHANGED_ID, K_CHK_IGNORE_FOV_IN_MAIN_MENU_TOGGLE_EVENT_ID, nullptr,
				[](const Control::s_interface_value& event_data, void* userdata)
				{
					Fov::c_settings_fov::Instance()->m_ignore_fov_change_in_main_menu = event_data.m_bool;
				});


			AttachEvent(K_CHK_SHADER_EXTENSION_MODEL_NORMAL_MAPS_ENABLED_ID, K_EVENT_CHECKCHANGED_ID, K_CHK_SHADER_EXTENSION_MODEL_NORMAL_MAPS_TOGGLE_EVENT_ID, nullptr,
				[](const Control::s_interface_value& event_data, void* userdata)
				{
					Rasterizer::ShaderExtension::Model::SetNormalMapsEnabled(event_data.m_bool);
				});

			AttachEvent(K_CHK_SHADER_EXTENSION_MODEL_DETAIL_NORMAL_MAPS_ENABLED_ID, K_EVENT_CHECKCHANGED_ID, K_CHK_SHADER_EXTENSION_MODEL_DETAIL_NORMAL_MAPS_TOGGLE_EVENT_ID, nullptr,
				[](const Control::s_interface_value& event_data, void* userdata)
				{
					Rasterizer::ShaderExtension::Model::SetDetailNormalMapsEnabled(event_data.m_bool);
				});

			AttachEvent(K_CHK_SHADER_EXTENSION_MODEL_SPECULAR_MAPS_ENABLED_ID, K_EVENT_CHECKCHANGED_ID, K_CHK_SHADER_EXTENSION_MODEL_SPECULAR_MAPS_TOGGLE_EVENT_ID, nullptr,
				[](const Control::s_interface_value& event_data, void* userdata)
				{
					Rasterizer::ShaderExtension::Model::SetSpecularMapsEnabled(event_data.m_bool);
				});

			AttachEvent(K_CHK_SHADER_EXTENSION_MODEL_SPECULAR_LIGHTING_ENABLED_ID, K_EVENT_CHECKCHANGED_ID, K_CHK_SHADER_EXTENSION_MODEL_SPECULAR_LIGHTING_TOGGLE_EVENT_ID, nullptr,
				[](const Control::s_interface_value& event_data, void* userdata)
				{
					Rasterizer::ShaderExtension::Model::SetSpecularLightingEnabled(event_data.m_bool);
				});


			AttachEvent(K_CHK_SHADER_EXTENSION_ENVIRONMENT_DIFFUSE_DLM_ENABLED_ID, K_EVENT_CHECKCHANGED_ID, K_CHK_SHADER_EXTENSION_ENVIRONMENT_DIFFUSE_DLM_TOGGLE_EVENT_ID, nullptr,
				[](const Control::s_interface_value& event_data, void* userdata)
				{
					Rasterizer::ShaderExtension::Environment::SetDiffuseDLMsEnabled(event_data.m_bool);
				});
			
			AttachEvent(K_CHK_SHADER_EXTENSION_ENVIRONMENT_SPECULAR_DLM_ENABLED_ID, K_EVENT_CHECKCHANGED_ID, K_CHK_SHADER_EXTENSION_ENVIRONMENT_SPECULAR_DLM_TOGGLE_EVENT_ID, nullptr,
				[](const Control::s_interface_value& event_data, void* userdata)
				{
					Rasterizer::ShaderExtension::Environment::SetSpecularDLMsEnabled(event_data.m_bool);
				});


			AttachEvent(K_CHK_SHADER_EXTENSION_EFFECT_DEPTH_FADE_ENABLED_ID, K_EVENT_CHECKCHANGED_ID, K_CHK_SHADER_EXTENSION_EFFECT_DEPTH_FADE_TOGGLE_EVENT_ID, nullptr,
				[](const Control::s_interface_value& event_data, void* userdata)
				{
					Rasterizer::ShaderExtension::Effect::SetDepthFadeEnabled(event_data.m_bool);

					if(event_data.m_bool)
					{
						DX9::c_gbuffer_system::g_system_enabled = true;
					}
				});


			AttachEvent(K_CHK_POST_PROCESSING_BLOOM_ENABLED_ID, K_EVENT_CHECKCHANGED_ID, K_CHK_POST_PROCESSING_BLOOM_TOGGLE_EVENT_ID, nullptr,
				[](const Control::s_interface_value& event_data, void* userdata)
				{
					Rasterizer::PostProcessing::Bloom::c_system_bloom::Instance().Enabled() = event_data.m_bool;
				});

			AttachEvent(K_CHK_POST_PROCESSING_ANTI_ALIASING_ENABLED_ID, K_EVENT_CHECKCHANGED_ID, K_CHK_POST_PROCESSING_ANTI_ALIASING_TOGGLE_EVENT_ID, nullptr,
				[](const Control::s_interface_value& event_data, void* userdata)
				{
					Rasterizer::PostProcessing::FXAA::c_system_fxaa::Instance().Enabled() = event_data.m_bool;
				});

			AttachEvent(K_CHK_POST_PROCESSING_EXTERNAL_EFFECTS_ENABLED_ID, K_EVENT_CHECKCHANGED_ID, K_CHK_POST_PROCESSING_EXTERNAL_EFFECTS_TOGGLE_EVENT_ID, nullptr,
				[](const Control::s_interface_value& event_data, void* userdata)
				{
					Rasterizer::PostProcessing::Generic::External::c_system_external::Instance().Enabled() = event_data.m_bool;
				});

			AttachEvent(K_CHK_POST_PROCESSING_MAP_EFFECTS_ENABLED_ID, K_EVENT_CHECKCHANGED_ID, K_CHK_POST_PROCESSING_MAP_EFFECTS_TOGGLE_EVENT_ID, nullptr,
				[](const Control::s_interface_value& event_data, void* userdata)
				{
					Rasterizer::PostProcessing::Generic::Internal::c_system_internal::Instance().Enabled() = event_data.m_bool;
				});

			AttachEvent(K_CHK_POST_PROCESSING_MOTIONBLUR_ENABLED_ID, K_EVENT_CHECKCHANGED_ID, K_CHK_POST_PROCESSING_MOTIONBLUR_TOGGLE_EVENT_ID, nullptr,
				[](const Control::s_interface_value& event_data, void* userdata)
				{
					auto& motionblur_instance = Rasterizer::PostProcessing::MotionBlur::c_system_motionblur::Instance();

					motionblur_instance.Enabled() = event_data.m_bool;
					
					if(event_data.m_bool)
					{
						DX9::c_gbuffer_system::g_system_enabled = true;
					}

					if(event_data.m_bool && (motionblur_instance.BlurAmount() == 0.0f))
					{
						motionblur_instance.BlurAmount() = 1.0f;
					}
				});

			AttachEvent(K_SLD_POST_PROCESSING_MOTIONBLUR_AMOUNT_ID, K_EVENT_VALUECHANGED_ID, K_SLD_POST_PROCESSING_MOTIONBLUR_AMOUNT_CHANGED_EVENT_ID, nullptr,
				[](const Control::s_interface_value& event_data, void* userdata)
				{
					auto& motionblur_instance = Rasterizer::PostProcessing::MotionBlur::c_system_motionblur::Instance();

					if(motionblur_instance.BlurAmount() == event_data.m_real)
					{
						return;
					}

					motionblur_instance.BlurAmount() = event_data.m_real;

					if(motionblur_instance.BlurAmount() == 0.0f)
					{
						motionblur_instance.Enabled() = false;
					}
					else
					{
						motionblur_instance.Enabled() = true;
						DX9::c_gbuffer_system::g_system_enabled = true;
					}
				});


			AttachEvent(K_CHK_INTERNET_CHECK_FOR_GAME_UPDATES_ENABLED_ID, K_EVENT_CHECKCHANGED_ID, K_CHK_INTERNET_CHECK_FOR_GAME_UPDATES_TOGGLE_EVENT_ID, nullptr,
				[](const Control::s_interface_value& event_data, void* userdata)
				{
					Networking::GameSpy::c_settings_gamespy::Instance()->m_no_update_check = !event_data.m_bool;
				});

			AttachEvent(K_CHK_INTERNET_MAP_DOWNLOADING_ENABLED_ID, K_EVENT_CHECKCHANGED_ID, K_CHK_INTERNET_MAP_DOWNLOADING_TOGGLE_EVENT_ID, nullptr,
				[](const Control::s_interface_value& event_data, void* userdata)
				{
					Networking::HTTP::Client::MapDownload::c_settings_mapdownload::Instance()->m_enabled = event_data.m_bool;
				});


			AttachEvent(K_CHK_MAP_FILES_CHECK_FOR_YELO_FIRST_ENABLED_ID, K_EVENT_CHECKCHANGED_ID, K_CHK_MAP_FILES_CHECK_FOR_YELO_FIRST_TOGGLE_EVENT_ID, nullptr,
				[](const Control::s_interface_value& event_data, void* userdata)
				{
					Cache::c_settings_cache::Instance()->m_check_yelo_files_first = event_data.m_bool;
				});
		}

		void c_screen_controller_mainmenu::UnbindEvents()
		{
			// Remove all bound events
			DetachEvent(K_CHK_GENERAL_GBUFFER_ENABLED_ID, K_EVENT_CHECKCHANGED_ID, K_CHK_GENERAL_GBUFFER_TOGGLE_EVENT_ID);
			
			DetachEvent(K_CHK_UPGRADES_MAX_RENDERED_TRIANGLES_ENABLED_ID, K_EVENT_CHECKCHANGED_ID, K_CHK_UPGRADES_MAX_RENDERED_TRIANGLES_TOGGLE_EVENT_ID);

			DetachEvent(K_SLD_FIELD_OF_VIEW_ID, K_EVENT_VALUECHANGED_ID, K_SLD_FIELD_OF_VIEW_AMOUNT_CHANGED_EVENT_ID);
			DetachEvent(K_CHK_IGNORE_FOV_IN_CINEMATICS_ENABLED_ID, K_EVENT_CHECKCHANGED_ID, K_CHK_IGNORE_FOV_IN_CINEMATICS_TOGGLE_EVENT_ID);
			DetachEvent(K_CHK_IGNORE_FOV_IN_MAIN_MENU_ENABLED_ID, K_EVENT_CHECKCHANGED_ID, K_CHK_IGNORE_FOV_IN_MAIN_MENU_TOGGLE_EVENT_ID);
			
			DetachEvent(K_CHK_SHADER_EXTENSION_MODEL_NORMAL_MAPS_ENABLED_ID, K_EVENT_CHECKCHANGED_ID, K_CHK_SHADER_EXTENSION_MODEL_NORMAL_MAPS_TOGGLE_EVENT_ID);
			DetachEvent(K_CHK_SHADER_EXTENSION_MODEL_DETAIL_NORMAL_MAPS_ENABLED_ID, K_EVENT_CHECKCHANGED_ID, K_CHK_SHADER_EXTENSION_MODEL_DETAIL_NORMAL_MAPS_TOGGLE_EVENT_ID);
			DetachEvent(K_CHK_SHADER_EXTENSION_MODEL_SPECULAR_MAPS_ENABLED_ID, K_EVENT_CHECKCHANGED_ID, K_CHK_SHADER_EXTENSION_MODEL_SPECULAR_MAPS_TOGGLE_EVENT_ID);
			DetachEvent(K_CHK_SHADER_EXTENSION_MODEL_SPECULAR_LIGHTING_ENABLED_ID, K_EVENT_CHECKCHANGED_ID, K_CHK_SHADER_EXTENSION_MODEL_SPECULAR_LIGHTING_TOGGLE_EVENT_ID);

			DetachEvent(K_CHK_SHADER_EXTENSION_ENVIRONMENT_DIFFUSE_DLM_ENABLED_ID, K_EVENT_CHECKCHANGED_ID, K_CHK_SHADER_EXTENSION_ENVIRONMENT_DIFFUSE_DLM_TOGGLE_EVENT_ID);
			DetachEvent(K_CHK_SHADER_EXTENSION_ENVIRONMENT_SPECULAR_DLM_ENABLED_ID, K_EVENT_CHECKCHANGED_ID, K_CHK_SHADER_EXTENSION_ENVIRONMENT_SPECULAR_DLM_TOGGLE_EVENT_ID);

			DetachEvent(K_CHK_SHADER_EXTENSION_EFFECT_DEPTH_FADE_ENABLED_ID, K_EVENT_CHECKCHANGED_ID, K_CHK_SHADER_EXTENSION_EFFECT_DEPTH_FADE_TOGGLE_EVENT_ID);

			DetachEvent(K_CHK_POST_PROCESSING_BLOOM_ENABLED_ID, K_EVENT_CHECKCHANGED_ID, K_CHK_POST_PROCESSING_BLOOM_TOGGLE_EVENT_ID);
			DetachEvent(K_CHK_POST_PROCESSING_ANTI_ALIASING_ENABLED_ID, K_EVENT_CHECKCHANGED_ID, K_CHK_POST_PROCESSING_ANTI_ALIASING_TOGGLE_EVENT_ID);
			DetachEvent(K_CHK_POST_PROCESSING_EXTERNAL_EFFECTS_ENABLED_ID, K_EVENT_CHECKCHANGED_ID, K_CHK_POST_PROCESSING_EXTERNAL_EFFECTS_TOGGLE_EVENT_ID);
			DetachEvent(K_CHK_POST_PROCESSING_MAP_EFFECTS_ENABLED_ID, K_EVENT_CHECKCHANGED_ID, K_CHK_POST_PROCESSING_MAP_EFFECTS_TOGGLE_EVENT_ID);
			DetachEvent(K_CHK_POST_PROCESSING_MOTIONBLUR_ENABLED_ID, K_EVENT_CHECKCHANGED_ID, K_CHK_POST_PROCESSING_MOTIONBLUR_TOGGLE_EVENT_ID);
			DetachEvent(K_SLD_POST_PROCESSING_MOTIONBLUR_AMOUNT_ID, K_EVENT_VALUECHANGED_ID, K_SLD_POST_PROCESSING_MOTIONBLUR_AMOUNT_CHANGED_EVENT_ID);

			DetachEvent(K_CHK_INTERNET_CHECK_FOR_GAME_UPDATES_ENABLED_ID, K_EVENT_CHECKCHANGED_ID, K_CHK_INTERNET_CHECK_FOR_GAME_UPDATES_TOGGLE_EVENT_ID);
			DetachEvent(K_CHK_INTERNET_MAP_DOWNLOADING_ENABLED_ID, K_EVENT_CHECKCHANGED_ID, K_CHK_INTERNET_MAP_DOWNLOADING_TOGGLE_EVENT_ID);

			DetachEvent(K_CHK_MAP_FILES_CHECK_FOR_YELO_FIRST_ENABLED_ID, K_EVENT_CHECKCHANGED_ID, K_CHK_MAP_FILES_CHECK_FOR_YELO_FIRST_TOGGLE_EVENT_ID);
		}
	};};};
};
#endif