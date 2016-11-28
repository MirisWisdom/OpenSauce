/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Interface/OpenSauceUI/Screen/c_screen_controller_ingame.hpp"

#if !PLATFORM_IS_DEDI

#include "Interface/OpenSauceUI/Control/control_property_ids.hpp"
#include "Interface/OpenSauceUI/Control/control_event_ids.hpp"
#include "Game/Camera.hpp"
#include "Rasterizer/Rasterizer.hpp"
#include "Interface/GameUI.hpp"
#include "Objects/Objects.hpp"

namespace Yelo
{
	namespace Interface { namespace OpenSauceUI { namespace Screen
	{
#pragma region Resource ID Defines
#define K_PDG_INGAME_PAGE_CONTROL_ID									RESOURCE_ID("#PDG_ingame_page_control",						0xD854DD47)

#define K_LBL_CAMERA_TITLE_ID											RESOURCE_ID("#LBL_camera_title",							0x3542AB46)
#define K_LBL_CAMERA_FOV_ID												RESOURCE_ID("#LBL_camera_fov",								0xA3FEC323)
#define K_LBL_CAMERA_FOV_VALUE_ID										RESOURCE_ID("#LBL_camera_fov_value",						0x0545D396)
#define K_SLD_CAMERA_FOV_VALUE_ID										RESOURCE_ID("#SLD_camera_fov_value",						0x1A30B008)
#define K_BTN_CAMERA_FOV_RESET_ID										RESOURCE_ID("#BTN_camera_fov_reset",						0xDA1D2412)
#define K_BTN_CAMERA_FOV_RESET_UP_EVENT_ID								RESOURCE_ID("#BTN_camera_fov_reset_up_event",				0x09C85652)
#define K_SLD_CAMERA_FOV_VALUE_CHANGED_EVENT_ID							RESOURCE_ID("#SLD_camera_fov_value_changed_event",			0x70A9C668)

#define K_LBL_HUD_TITLE_ID												RESOURCE_ID("#LBL_hud_title",								0x63A292FC)
#define K_SLD_HUD_SCALE_X_AMOUNT_ID										RESOURCE_ID("#SLD_hud_scale_x_amount",						0x6DCABA2B)
#define K_SLD_HUD_SCALE_X_AMOUNT_CHANGED_EVENT_ID						RESOURCE_ID("#SLD_hud_scale_x_amount_changed_event",		0x8250BC48)
#define K_SLD_HUD_SCALE_Y_AMOUNT_ID										RESOURCE_ID("#SLD_hud_scale_y_amount",						0xA160BAB5)
#define K_SLD_HUD_SCALE_Y_AMOUNT_CHANGED_EVENT_ID						RESOURCE_ID("#SLD_hud_scale_y_amount_changed_event",		0x9578D888)
#define K_BTN_HUD_SCALE_RESET_ID										RESOURCE_ID("#BTN_hud_scale_reset",							0x0610BE8C)
#define K_BTN_HUD_SCALE_RESET_UP_EVENT_ID								RESOURCE_ID("#BTN_hud_scale_reset_up_event",				0xB1F4DA53)
#define K_CHK_HUD_SHOW_HUD_ENABLED_ID									RESOURCE_ID("#CHK_hud_show_hud_enabled",					0xA8E63B7E)
#define K_CHK_HUD_SHOW_HUD_TOGGLE_EVENT_ID								RESOURCE_ID("#CHK_hud_show_hud_toggle_event",				0xEED00D6F)
#define K_CHK_HUD_SCALE_HUD_ENABLED_ID									RESOURCE_ID("#CHK_hud_scale_hud_enabled",					0xBBFE03A5)
#define K_CHK_HUD_SCALE_HUD_TOGGLE_EVENT_ID								RESOURCE_ID("#CHK_hud_scale_hud_toggle_event",				0x56B02033)
		
#define K_LBL_WEAPON_TITLE_ID											RESOURCE_ID("#LBL_weapon_title",							0x6718BDE4)
#define K_LBL_WEAPON_POSITION_TITLE_ID									RESOURCE_ID("#LBL_weapon_position_title",					0xCB4492AD)
#define K_SLD_WEAPON_POSITION_X_OFFSET_ID								RESOURCE_ID("#SLD_weapon_position_x_offset",				0x91712A9F)
#define K_SLD_WEAPON_POSITION_X_OFFSET_CHANGED_EVENT_ID					RESOURCE_ID("#SLD_weapon_position_x_offset_changed_event",	0x21911389)
#define K_SLD_WEAPON_POSITION_Y_OFFSET_ID								RESOURCE_ID("#SLD_weapon_position_y_offset",				0x5DDB2A01)
#define K_SLD_WEAPON_POSITION_Y_OFFSET_CHANGED_EVENT_ID					RESOURCE_ID("#SLD_weapon_position_y_offset_changed_event",	0x36B97749)
#define K_SLD_WEAPON_POSITION_Z_OFFSET_ID								RESOURCE_ID("#SLD_weapon_position_z_offset",				0xD3542DE2)
#define K_SLD_WEAPON_POSITION_Z_OFFSET_CHANGED_EVENT_ID					RESOURCE_ID("#SLD_weapon_position_z_offset_changed_event",	0x0FC1DA09)
#define K_BTN_WEAPON_POSITION_RESET_ID									RESOURCE_ID("#BTN_weapon_position_reset",					0xD5FF541E)
#define K_BTN_WEAPON_POSITION_RESET_UP_EVENT_ID							RESOURCE_ID("#BTN_weapon_position_reset_up_event",			0x26C5313D)
#pragma endregion
		
		c_screen_controller_ingame::c_screen_controller_ingame(Definitions::c_screen_definition& definition)
			: c_screen_controller_base(definition)
		{ }

		void c_screen_controller_ingame::SetStaticProperties()
		{
			// Set the screen to be central
			AnchorScreen(
				(screen_anchor)(_screen_anchor_center_v | _screen_anchor_center_h),
				(screen_anchor)(_screen_anchor_center_v | _screen_anchor_center_h));
			
			SetControlProperty(K_PDG_INGAME_PAGE_CONTROL_ID, K_PROPERTY_TEXTFORMAT_ID, "Page %i to %i");
			SetControlProperty(K_PDG_INGAME_PAGE_CONTROL_ID, K_PROPERTY_BACKBUTTONTEXT_ID, "< Back");
			SetControlProperty(K_PDG_INGAME_PAGE_CONTROL_ID, K_PROPERTY_NEXTBUTTONTEXT_ID, "Next >");

			SetControlProperty(K_LBL_CAMERA_TITLE_ID, K_PROPERTY_TEXT_ID, "Camera");
			SetControlProperty(K_LBL_CAMERA_FOV_ID, K_PROPERTY_TEXT_ID, "Field Of View");
			SetControlProperty(K_BTN_CAMERA_FOV_RESET_ID, K_PROPERTY_TEXT_ID, "Reset");

			SetControlProperty(K_LBL_HUD_TITLE_ID, K_PROPERTY_TEXT_ID, "HUD");
			SetControlProperty(K_BTN_HUD_SCALE_RESET_ID, K_PROPERTY_TEXT_ID, "Reset");
			SetControlProperty(K_CHK_HUD_SHOW_HUD_ENABLED_ID, K_PROPERTY_TEXT_ID, "Show HUD");
			SetControlProperty(K_CHK_HUD_SCALE_HUD_ENABLED_ID, K_PROPERTY_TEXT_ID, "Scale HUD");
			
			SetControlProperty(K_LBL_WEAPON_TITLE_ID, K_PROPERTY_TEXT_ID, "Weapon");
			SetControlProperty(K_LBL_WEAPON_POSITION_TITLE_ID, K_PROPERTY_TEXT_ID, "Position Offset");
			SetControlProperty(K_BTN_WEAPON_POSITION_RESET_ID, K_PROPERTY_TEXT_ID, "Reset");
		}

		void c_screen_controller_ingame::BindDynamicProperties()
		{
			// Set all static properties in the UI controls
#pragma region Camera
			AddDynamicProperty(K_LBL_CAMERA_FOV_VALUE_ID, K_PROPERTY_TEXT_ID,
				[](Control::i_control& control, Control::i_property_interface& property)
				{
					char buffer[10] = "";
					sprintf_s(buffer, "%.2f", Fov::GetFieldOfView());

					property.Set(control, Control::s_interface_value(buffer));
				});

			AddDynamicProperty(K_SLD_CAMERA_FOV_VALUE_ID, K_PROPERTY_VALUE_ID,
				[](Control::i_control& control, Control::i_property_interface& property)
				{
					property.Set(control, Control::s_interface_value(Fov::GetFieldOfView()));
				});
#pragma endregion

#pragma region HUD
			AddDynamicProperty(K_SLD_HUD_SCALE_X_AMOUNT_ID, K_PROPERTY_VALUE_ID,
				[](Control::i_control& control, Control::i_property_interface& property)
				{
					property.Set(control, Control::s_interface_value(Hud::HUDScaleX()));
				});

			AddDynamicProperty(K_SLD_HUD_SCALE_Y_AMOUNT_ID, K_PROPERTY_VALUE_ID,
				[](Control::i_control& control, Control::i_property_interface& property)
				{
					property.Set(control, Control::s_interface_value(Hud::HUDScaleY()));
				});

			AddDynamicProperty(K_CHK_HUD_SHOW_HUD_ENABLED_ID, K_PROPERTY_CHECKED_ID,
				[](Control::i_control& control, Control::i_property_interface& property)
				{
					property.Set(control, Control::s_interface_value(Hud::ShowHUD()));
				});

			AddDynamicProperty(K_CHK_HUD_SCALE_HUD_ENABLED_ID, K_PROPERTY_CHECKED_ID,
				[](Control::i_control& control, Control::i_property_interface& property)
				{
					property.Set(control, Control::s_interface_value(Hud::ScaleHUD()));
				});
#pragma endregion

#pragma region Weapon
			AddDynamicProperty(K_SLD_WEAPON_POSITION_X_OFFSET_ID, K_PROPERTY_VALUE_ID,
				[](Control::i_control& control, Control::i_property_interface& property)
				{
					auto position = Objects::Weapon::GetWeaponPosition();

					property.Set(control, Control::s_interface_value(position.i));
				});

			AddDynamicProperty(K_SLD_WEAPON_POSITION_Y_OFFSET_ID, K_PROPERTY_VALUE_ID,
				[](Control::i_control& control, Control::i_property_interface& property)
				{
					auto position = Objects::Weapon::GetWeaponPosition();

					property.Set(control, Control::s_interface_value(position.j));
				});

			AddDynamicProperty(K_SLD_WEAPON_POSITION_Z_OFFSET_ID, K_PROPERTY_VALUE_ID,
				[](Control::i_control& control, Control::i_property_interface& property)
				{
					auto position = Objects::Weapon::GetWeaponPosition();

					property.Set(control, Control::s_interface_value(position.k));
				});
#pragma endregion
		}

		void c_screen_controller_ingame::BindEvents()
		{
			// Attach all events that the screen has to react to
#pragma region Camera
			AttachEvent(K_SLD_CAMERA_FOV_VALUE_ID, K_EVENT_VALUECHANGED_ID, K_SLD_CAMERA_FOV_VALUE_CHANGED_EVENT_ID, nullptr,
				[](const Control::s_interface_value& event_data, void* userdata)
				{
					Fov::SetFieldOfView(event_data.m_real);
				});

			AttachEvent(K_BTN_CAMERA_FOV_RESET_ID, K_EVENT_UP_ID, K_BTN_CAMERA_FOV_RESET_UP_EVENT_ID, nullptr,
				[](const Control::s_interface_value& event_data, void* userdata)
				{
					Fov::ResetFieldOfView();
				});
#pragma endregion

#pragma region HUD
			AttachEvent(K_SLD_HUD_SCALE_X_AMOUNT_ID, K_EVENT_VALUECHANGED_ID, K_SLD_HUD_SCALE_X_AMOUNT_CHANGED_EVENT_ID, nullptr,
				[](const Control::s_interface_value& event_data, void* userdata)
				{
					Hud::HUDScaleX() = event_data.m_real;
					Hud::UpdateChanges();
				});

			AttachEvent(K_SLD_HUD_SCALE_Y_AMOUNT_ID, K_EVENT_VALUECHANGED_ID, K_SLD_HUD_SCALE_Y_AMOUNT_CHANGED_EVENT_ID, nullptr,
				[](const Control::s_interface_value& event_data, void* userdata)
				{
					Hud::HUDScaleY() = event_data.m_real;
					Hud::UpdateChanges();
				});

			AttachEvent(K_BTN_HUD_SCALE_RESET_ID, K_EVENT_UP_ID, K_BTN_HUD_SCALE_RESET_UP_EVENT_ID, nullptr,
				[](const Control::s_interface_value& event_data, void* userdata)
				{
					Hud::HUDScaleX() = 1.0f;
					Hud::HUDScaleY() = 1.0f;
				});

			AttachEvent(K_CHK_HUD_SHOW_HUD_ENABLED_ID, K_EVENT_CHECKCHANGED_ID, K_CHK_HUD_SHOW_HUD_TOGGLE_EVENT_ID, nullptr,
				[](const Control::s_interface_value& event_data, void* userdata)
				{
					Hud::ShowHUD() = event_data.m_bool;
				});

			AttachEvent(K_CHK_HUD_SCALE_HUD_ENABLED_ID, K_EVENT_CHECKCHANGED_ID, K_CHK_HUD_SCALE_HUD_TOGGLE_EVENT_ID, nullptr,
				[](const Control::s_interface_value& event_data, void* userdata)
				{
					Hud::ScaleHUD() = event_data.m_bool;
					Hud::UpdateChanges();
				});
#pragma endregion

#pragma region Weapon
			AttachEvent(K_SLD_WEAPON_POSITION_X_OFFSET_ID, K_EVENT_VALUECHANGED_ID, K_SLD_WEAPON_POSITION_X_OFFSET_CHANGED_EVENT_ID, nullptr,
				[](const Control::s_interface_value& event_data, void* userdata)
				{
					auto position = Objects::Weapon::GetWeaponPosition();

					position.i = event_data.m_real;
					Objects::Weapon::SetWeaponPosition(position);
				});

			AttachEvent(K_SLD_WEAPON_POSITION_Y_OFFSET_ID, K_EVENT_VALUECHANGED_ID, K_SLD_WEAPON_POSITION_Y_OFFSET_CHANGED_EVENT_ID, nullptr,
				[](const Control::s_interface_value& event_data, void* userdata)
				{
					auto position = Objects::Weapon::GetWeaponPosition();

					position.j = event_data.m_real;
					Objects::Weapon::SetWeaponPosition(position);
				});

			AttachEvent(K_SLD_WEAPON_POSITION_Z_OFFSET_ID, K_EVENT_VALUECHANGED_ID, K_SLD_WEAPON_POSITION_Z_OFFSET_CHANGED_EVENT_ID, nullptr,
				[](const Control::s_interface_value& event_data, void* userdata)
				{
					auto position = Objects::Weapon::GetWeaponPosition();

					position.k = event_data.m_real;
					Objects::Weapon::SetWeaponPosition(position);
				});

			AttachEvent(K_BTN_WEAPON_POSITION_RESET_ID, K_EVENT_UP_ID, K_BTN_WEAPON_POSITION_RESET_UP_EVENT_ID, nullptr,
				[](const Control::s_interface_value& event_data, void* userdata)
				{
					Objects::Weapon::SetWeaponPosition(real_vector3d { 0.0f, 0.0f, 0.0f });
				});
#pragma endregion
		}

		void c_screen_controller_ingame::UnbindEvents()
		{
			// Attach all events that the screen is attached to
#pragma region Camera
			DetachEvent(K_SLD_CAMERA_FOV_VALUE_ID, K_EVENT_VALUECHANGED_ID, K_SLD_CAMERA_FOV_VALUE_CHANGED_EVENT_ID);
			DetachEvent(K_BTN_CAMERA_FOV_RESET_ID, K_EVENT_UP_ID, K_BTN_CAMERA_FOV_RESET_UP_EVENT_ID);
#pragma endregion

#pragma region HUD
			DetachEvent(K_SLD_HUD_SCALE_X_AMOUNT_ID, K_EVENT_VALUECHANGED_ID, K_SLD_HUD_SCALE_X_AMOUNT_CHANGED_EVENT_ID);
			DetachEvent(K_SLD_HUD_SCALE_Y_AMOUNT_ID, K_EVENT_VALUECHANGED_ID, K_SLD_HUD_SCALE_Y_AMOUNT_CHANGED_EVENT_ID);
			DetachEvent(K_BTN_HUD_SCALE_RESET_ID, K_EVENT_UP_ID, K_BTN_HUD_SCALE_RESET_UP_EVENT_ID);
			DetachEvent(K_CHK_HUD_SHOW_HUD_ENABLED_ID, K_EVENT_CHECKCHANGED_ID, K_CHK_HUD_SHOW_HUD_TOGGLE_EVENT_ID);
			DetachEvent(K_CHK_HUD_SCALE_HUD_ENABLED_ID, K_EVENT_CHECKCHANGED_ID, K_CHK_HUD_SCALE_HUD_TOGGLE_EVENT_ID);
#pragma endregion

#pragma region Weapon
			DetachEvent(K_SLD_WEAPON_POSITION_X_OFFSET_ID, K_EVENT_VALUECHANGED_ID, K_SLD_WEAPON_POSITION_X_OFFSET_CHANGED_EVENT_ID);
			DetachEvent(K_SLD_WEAPON_POSITION_Y_OFFSET_ID, K_EVENT_VALUECHANGED_ID, K_SLD_WEAPON_POSITION_Y_OFFSET_CHANGED_EVENT_ID);
			DetachEvent(K_SLD_WEAPON_POSITION_Z_OFFSET_ID, K_EVENT_VALUECHANGED_ID, K_SLD_WEAPON_POSITION_Z_OFFSET_CHANGED_EVENT_ID);
			DetachEvent(K_BTN_WEAPON_POSITION_RESET_ID, K_EVENT_UP_ID, K_BTN_WEAPON_POSITION_RESET_UP_EVENT_ID);
#pragma endregion
		}

		void c_screen_controller_ingame::Hide()
		{
			c_screen_controller_base::Hide();

			Settings::Manager().Save();
		}
	};};};
};
#endif