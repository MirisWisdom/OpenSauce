/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Interface/OpenSauceUI/Screen/c_screen_controller_base.hpp"

#if !PLATFORM_IS_DEDI

#include "Interface/OpenSauceUI/Definitions/c_screen_definition.hpp"
#include "Interface/OpenSauceUI/Screen/c_screen.hpp"

namespace Yelo
{
	namespace Interface { namespace OpenSauceUI { namespace Screen
	{
		c_screen_controller_base::c_screen_controller_base()
			: m_target_screen(nullptr)
		{ }

#pragma region i_screen_controller
		void c_screen_controller_base::BuildScreen(ControlFactory::c_control_factory& control_factory, Control::i_canvas& target_canvas)
		{
			DOC_TODO("PROPER SCREEN LOADING");

			// If the screen has already been built return
			if(m_target_screen)
			{
				return;
			}

			// Build the screen's root control
			auto& config_file = Configuration::c_configuration_file_factory::CreateConfigurationFile("D://test.json");
			config_file->Load();

			auto& root = config_file->Root()->GetChild("OpenSauceUI");
			
			Definitions::c_screen_definition screen;
			auto& control_iterator = root->GetChildIterator("Screen");
			if(control_iterator->MoveNext())
			{
				screen.GetValue(*control_iterator->Current());
			}

			// Build the screen instance
			auto root_ptr = control_factory.BuildControl(target_canvas, screen.m_control_definition);
			m_target_screen = std::make_shared<c_screen>(root_ptr);

			// Set up properties and events
			SetStaticProperties();
			BindDynamicProperties();
			BindEvents();
		}

		void c_screen_controller_base::DestroyScreen(Control::i_canvas& target_canvas)
		{
			// If the screen has already been destroyed return
			if(!m_target_screen)
			{
				return;
			}

			// Unbind all properties and events
			UnbindDynamicProperties();
			UnbindEvents();

			// Remove the screen and delete it
			target_canvas.RemoveControl(m_target_screen->GetRootControl());
			m_target_screen.reset();
		}

		void c_screen_controller_base::Update()
		{
			// Update dynamic properties in the screen
			for(auto& dynamic_property : m_dynamic_properties)
			{
				dynamic_property.m_update(dynamic_property.m_control, dynamic_property.m_property);
			}
		}

		void c_screen_controller_base::UnbindDynamicProperties()
		{
			m_dynamic_properties.clear();
		}
#pragma endregion
		
#pragma region Property Setup
		void c_screen_controller_base::FindControlAndProperty(const uint32 control_id
			, const uint32 property_id
			, Control::t_control_ptr& control_out
			, Control::i_property_interface*& property_out)
		{
			// Get the control from the screen by it's resource id
			auto control = m_target_screen->GetControl(control_id);
			YELO_ASSERT_DISPLAY(control, "Failed to find a required control");
			
			// Get the property from the control by it's resource id
			auto property = control->GetPropertyInterface(property_id);
			YELO_ASSERT_DISPLAY(control, "Failed to find a control property");

			control_out = control;
			property_out = property;
		}

		template<typename Type>
		void c_screen_controller_base::SetControlPropertyImpl(uint32 control_id, uint32 property_id, Type value)
		{
			// Get the property and control
			Control::t_control_ptr control;
			Control::i_property_interface* property;
			FindControlAndProperty(control_id, property_id, control, property);

			// Set the property's value
			property->Set(*control, Control::s_interface_value(value));
		}

		template<>
		void c_screen_controller_base::SetControlPropertyImpl<cstring>(uint32 control_id, uint32 property_id, cstring value)
		{
			// Get the property and control
			Control::t_control_ptr control;
			Control::i_property_interface* property;
			FindControlAndProperty(control_id, property_id, control, property);

			// Send a copy of the input string to the property
			auto property_value = Control::s_interface_value();
			property_value.SetString(value);
			property->Set(*control, property_value);
		}

		template<>
		void c_screen_controller_base::SetControlPropertyImpl<wstring>(uint32 control_id, uint32 property_id, wstring value)
		{
			// Get the property and control
			Control::t_control_ptr control;
			Control::i_property_interface* property;
			FindControlAndProperty(control_id, property_id, control, property);
			
			// Send a copy of the input string to the property
			auto property_value = Control::s_interface_value();
			property_value.SetString(value);
			property->Set(*control, property_value);
		}

		void c_screen_controller_base::SetControlProperty(uint32 control_id, uint32 property_id, bool value)
		{
			SetControlPropertyImpl<bool>(control_id, property_id, value);
		}

		void c_screen_controller_base::SetControlProperty(uint32 control_id, uint32 property_id, int32 value)
		{
			SetControlPropertyImpl<int32>(control_id, property_id, value);
		}

		void c_screen_controller_base::SetControlProperty(uint32 control_id, uint32 property_id, cstring value)
		{
			SetControlPropertyImpl<cstring>(control_id, property_id, value);
		}

		void c_screen_controller_base::AddDynamicProperty(const uint32 control_id, const uint32 property_id, const t_dynamic_property_update& update)
		{
			// Get the property and control
			Control::t_control_ptr control;
			Control::i_property_interface* property;
			FindControlAndProperty(control_id, property_id, control, property);

			// Store the control and property with the update function
			m_dynamic_properties.push_back(s_dynamic_property { *control, *property, update });
		}
#pragma endregion

#pragma region Event Setup
		void c_screen_controller_base::AttachEvent(const uint32 control_id, const uint32 event_id, const uint32 callback_id, void* userdata, Control::t_event_callback function)
		{
			// Find the control by it's resource id then attach a callback function to the referenced event
			auto control = m_target_screen->GetControl(control_id);
			YELO_ASSERT_DISPLAY(control, "Failed to find a required control");

			control->AddEventCallback(event_id, callback_id, function, userdata);
		}

		void c_screen_controller_base::DetachEvent(const uint32 control_id, const uint32 event_id, const uint32 callback_id)
		{
			// Find the control by it's resource id then remove a callback function referenced by it's id
			auto control = m_target_screen->GetControl(control_id);
			YELO_ASSERT_DISPLAY(control, "Failed to find a required control");

			control->RemoveEventCallback(event_id, callback_id);
		}
#pragma endregion

#pragma region i_visibility_toggle
		void c_screen_controller_base::Show()
		{
			m_target_screen->Show();
		}

		void c_screen_controller_base::Hide()
		{
			m_target_screen->Hide();
		}
#pragma endregion
	};};};
};
#endif