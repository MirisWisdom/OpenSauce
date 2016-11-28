/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Interface/OpenSauceUI/Screen/c_screen_controller_base.hpp"

#if !PLATFORM_IS_DEDI

#include "Interface/OpenSauceUI/Control/control_property_ids.hpp"
#include "Interface/OpenSauceUI/Definitions/c_screen_definition.hpp"
#include "Interface/OpenSauceUI/Screen/c_screen.hpp"

namespace Yelo
{
	namespace Interface { namespace OpenSauceUI { namespace Screen
	{
		c_screen_controller_base::c_screen_controller_base(Definitions::c_screen_definition& definition)
			: m_dynamic_properties()
			, m_target_screen(nullptr)
			, m_screen_definition(definition)
		{ }

		point2d c_screen_controller_base::GetControlAnchorPoint(Control::i_control& control, const screen_anchor anchor)
		{
			// Get the control's position and size
			auto control_bounds = control.GetBounds();
			point2d control_size { control_bounds.right - control_bounds.left, control_bounds.bottom - control_bounds.top };

			point2d anchor_point { 0, 0 };
			if(anchor & _screen_anchor_top)
			{
				anchor_point.y = control_bounds.top;
			}
			else if(anchor & _screen_anchor_center_v)
			{
				anchor_point.y = control_bounds.top + (control_size.y / 2);
			}
			else if(anchor & _screen_anchor_bottom)
			{
				anchor_point.y = control_bounds.bottom;
			}

			if(anchor & _screen_anchor_left)
			{
				anchor_point.x = control_bounds.left;
			}
			else if(anchor & _screen_anchor_center_h)
			{
				anchor_point.x = control_bounds.left + (control_size.x / 2);
			}
			else if(anchor & _screen_anchor_right)
			{
				anchor_point.x = control_bounds.right;
			}

			return anchor_point;
		}

		void c_screen_controller_base::AnchorScreen(const screen_anchor parent_anchor, const screen_anchor child_anchor)
		{
			auto& root_control = *m_target_screen->GetRootControl();
			auto& screen_parent = *root_control.Parent();
			
			point2d parent_anchor_point = GetControlAnchorPoint(screen_parent, parent_anchor);
			point2d child_anchor_point = GetControlAnchorPoint(root_control, child_anchor);

			// Get the screen's position property
			auto& position_prop = *root_control.GetPropertyInterface(K_PROPERTY_POSITION_ID);
			position_prop.Set(root_control, Control::s_interface_value(
				point2d
				{
					parent_anchor_point.x - child_anchor_point.x,
					parent_anchor_point.y - child_anchor_point.y
				}));
		}

#pragma region i_screen_controller
		void c_screen_controller_base::BuildScreen(ControlFactory::c_control_factory& control_factory, Control::i_canvas& target_canvas)
		{
			// If the screen has already been built return
			if(m_target_screen)
			{
				return;
			}

			// Build the screen instance
			auto root_ptr = control_factory.BuildControl(target_canvas, m_screen_definition.m_control_definition);
			m_target_screen = std::make_shared<c_screen>(root_ptr);

			Hide();

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
		void c_screen_controller_base::FindControl(const uint32 control_id, Control::control_ptr_t& control_out)
		{
			// Get the control from the screen by it's resource id
			auto control = m_target_screen->GetControl(control_id);

			YELO_ASSERT_DISPLAY(control, "Failed to find a required control");

			control_out = control;
		}

		void c_screen_controller_base::FindControlAndProperty(const uint32 control_id
			, const uint32 property_id
			, Control::control_ptr_t& control_out
			, Control::i_property_interface*& property_out)
		{
			FindControl(control_id, control_out);

			// Get the property from the control by it's resource id
			auto property = control_out->GetPropertyInterface(property_id);

			YELO_ASSERT_DISPLAY(property, "Failed to find a control property");

			property_out = property;
		}

		template<typename Type>
		void c_screen_controller_base::SetControlPropertyImpl(const uint32 control_id, const uint32 property_id, const Type value)
		{
			// Get the property and control
			Control::control_ptr_t control;
			Control::i_property_interface* property;
			FindControlAndProperty(control_id, property_id, control, property);

			// Set the property's value
			property->Set(*control, Control::s_interface_value(value));
		}

		template<>
		void c_screen_controller_base::SetControlPropertyImpl<cstring>(const uint32 control_id, const uint32 property_id, cstring value)
		{
			// Get the property and control
			Control::control_ptr_t control;
			Control::i_property_interface* property;
			FindControlAndProperty(control_id, property_id, control, property);

			// Set the property's value
			property->Set(*control, value);
		}

		void c_screen_controller_base::SetControlProperty(const uint32 control_id, const uint32 property_id, const bool value)
		{
			SetControlPropertyImpl<bool>(control_id, property_id, value);
		}

		void c_screen_controller_base::SetControlProperty(const uint32 control_id, const uint32 property_id, const int32 value)
		{
			SetControlPropertyImpl<int32>(control_id, property_id, value);
		}

		void c_screen_controller_base::SetControlProperty(const uint32 control_id, const uint32 property_id, const real value)
		{
			SetControlPropertyImpl<real>(control_id, property_id, value);
		}

		void c_screen_controller_base::SetControlProperty(const uint32 control_id, const uint32 property_id, cstring value)
		{
			SetControlPropertyImpl<cstring>(control_id, property_id, value);
		}

		void c_screen_controller_base::AddDynamicProperty(const uint32 control_id, const uint32 property_id, const dynamic_property_update_t& update)
		{
			// Get the property and control
			Control::control_ptr_t control;
			Control::i_property_interface* property;
			FindControlAndProperty(control_id, property_id, control, property);

			// Store the control and property with the update function
			m_dynamic_properties.push_back(s_dynamic_property { *control, *property, update });
		}
#pragma endregion

#pragma region Event Setup
		void c_screen_controller_base::AttachEvent(const uint32 control_id, const uint32 event_id, const uint32 callback_id, void* userdata, Control::event_callback_t function)
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
			if(m_target_screen)
			{
				m_target_screen->Show();
			}
		}

		void c_screen_controller_base::Hide()
		{
			if(m_target_screen)
			{
				m_target_screen->Hide();
			}
		}
#pragma endregion
	};};};
};
#endif