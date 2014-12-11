/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Interface/OpenSauceUI/GwenUI/ControlBuilders/c_control_builder_gwen_base.hpp"

#if !PLATFORM_IS_DEDI

#include <Gwen/Controls/Base.h>
#include <YeloLib/cseries/value_conversion.hpp>

#include "Interface/OpenSauceUI/resource_id.hpp"
#include "Interface/OpenSauceUI/Control/c_control_base.hpp"

namespace Yelo
{
	namespace Interface { namespace OpenSauceUI { namespace GwenUI { namespace ControlBuilders
	{
		DEFINE_PROPERTY_INTERFACE(base_position,
			[](Control::i_control& control)
			{
				auto& gwen_control = GWEN_CTRL_REF(Base, control.GetControlPtr());

				return Control::s_interface_value(point2d { gwen_control.X(), gwen_control.Y() });
			},
			[](Control::i_control& control, const Control::s_interface_value& value)
			{
				auto& gwen_control = GWEN_CTRL_REF(Base, control.GetControlPtr());

				gwen_control.SetPos(value.m_point2d.x, value.m_point2d.y);
			},
			[](Control::i_control& control, cstring value_string)
			{
				auto& gwen_control = GWEN_CTRL_REF(Base, control.GetControlPtr());

				point2d value;
				ValueConversion::FromString(value_string, value);
				gwen_control.SetPos(value.x, value.y);
			}
		);

		DEFINE_PROPERTY_INTERFACE(base_size,
			[](Control::i_control& control)
			{
				auto& gwen_control = GWEN_CTRL_REF(Base, control.GetControlPtr());

				return Control::s_interface_value(point2d { gwen_control.Width(), gwen_control.Height() });
			},
			[](Control::i_control& control, const Control::s_interface_value& value)
			{
				auto& gwen_control = GWEN_CTRL_REF(Base, control.GetControlPtr());

				gwen_control.SetSize(value.m_point2d.x, value.m_point2d.y);
			},
			[](Control::i_control& control, cstring value_string)
			{
				auto& gwen_control = GWEN_CTRL_REF(Base, control.GetControlPtr());

				point2d value;
				ValueConversion::FromString(value_string, value);
				gwen_control.SetSize(value.x, value.y);
			}
		);

		DEFINE_PROPERTY_INTERFACE(base_dock,
			[](Control::i_control& control)
			{
				auto& gwen_control = GWEN_CTRL_REF(Base, control.GetControlPtr());

				return Control::s_interface_value((int32)gwen_control.GetDock());
			},
			[](Control::i_control& control, const Control::s_interface_value& value)
			{
				auto& gwen_control = GWEN_CTRL_REF(Base, control.GetControlPtr());

				gwen_control.Dock(value.m_int32);
			},
			[](Control::i_control& control, cstring value_string)
			{
				auto& gwen_control = GWEN_CTRL_REF(Base, control.GetControlPtr());

				int32 value = 0;

				std::string str_value(value_string);
				if(str_value == "Left")
				{
					value = Gwen::Pos::Left;
				}
				else if(str_value == "Right")
				{
					value = Gwen::Pos::Right;
				}
				else if(str_value == "Top")
				{
					value = Gwen::Pos::Top;
				}
				else if(str_value == "Bottom")
				{
					value = Gwen::Pos::Bottom;
				}
				else if(str_value == "CenterV")
				{
					value = Gwen::Pos::CenterV;
				}
				else if(str_value == "CenterH")
				{
					value = Gwen::Pos::CenterH;
				}
				else if(str_value == "Fill")
				{
					value = Gwen::Pos::Fill;
				}
				else if((str_value == "Center") || (str_value == "Centre"))
				{
					value = Gwen::Pos::Center;
				}

				gwen_control.Dock(value);
			}
		);

		DEFINE_PROPERTY_INTERFACE(base_margin,
			[](Control::i_control& control)
			{
				auto& gwen_control = GWEN_CTRL_REF(Base, control.GetControlPtr());

				auto& margin = gwen_control.GetMargin();
				return Control::s_interface_value(rectangle2d { margin.top, margin.left, margin.bottom, margin.right });
			},
			[](Control::i_control& control, const Control::s_interface_value& value)
			{
				auto& gwen_control = GWEN_CTRL_REF(Base, control.GetControlPtr());

				gwen_control.SetMargin(Gwen::Margin(value.m_rectangle2d.left, value.m_rectangle2d.top, value.m_rectangle2d.right, value.m_rectangle2d.bottom));
			},
			[](Control::i_control& control, cstring value_string)
			{
				auto& gwen_control = GWEN_CTRL_REF(Base, control.GetControlPtr());

				rectangle2d value;
				ValueConversion::FromString(value_string, value);
				gwen_control.SetMargin(Gwen::Margin(value.left, value.top, value.right, value.bottom));
			}
		);

		DEFINE_PROPERTY_INTERFACE(base_padding,
			[](Control::i_control& control)
			{
				auto& gwen_control = GWEN_CTRL_REF(Base, control.GetControlPtr());

				auto& padding = gwen_control.GetPadding();
				return Control::s_interface_value(rectangle2d { padding.top, padding.left, padding.bottom, padding.right });
			},
			[](Control::i_control& control, const Control::s_interface_value& value)
			{
				auto& gwen_control = GWEN_CTRL_REF(Base, control.GetControlPtr());

				gwen_control.SetPadding(Gwen::Margin(value.m_rectangle2d.left, value.m_rectangle2d.top, value.m_rectangle2d.right, value.m_rectangle2d.bottom));
			},
			[](Control::i_control& control, cstring value_string)
			{
				auto& gwen_control = GWEN_CTRL_REF(Base, control.GetControlPtr());

				rectangle2d value;
				ValueConversion::FromString(value_string, value);
				gwen_control.SetPadding(Gwen::Margin(value.left, value.top, value.right, value.bottom));
			}
		);

		DEFINE_PROPERTY_INTERFACE(base_hidden,
			[](Control::i_control& control)
			{
				auto& gwen_control = GWEN_CTRL_REF(Base, control.GetControlPtr());

				return Control::s_interface_value(gwen_control.Hidden());
			},
			[](Control::i_control& control, const Control::s_interface_value& value)
			{
				auto& gwen_control = GWEN_CTRL_REF(Base, control.GetControlPtr());

				gwen_control.SetHidden(value.m_bool);
			},
			[](Control::i_control& control, cstring value_string)
			{
				auto& gwen_control = GWEN_CTRL_REF(Base, control.GetControlPtr());

				bool value;
				ValueConversion::FromString(value_string, value);
				gwen_control.SetHidden(value);
			}
		);

		DEFINE_PROPERTY_INTERFACE(base_disabled,
			[](Control::i_control& control)
			{
				auto& gwen_control = GWEN_CTRL_REF(Base, control.GetControlPtr());

				return Control::s_interface_value(gwen_control.IsDisabled());
			},
			[](Control::i_control& control, const Control::s_interface_value& value)
			{
				auto& gwen_control = GWEN_CTRL_REF(Base, control.GetControlPtr());

				gwen_control.SetDisabled(value.m_bool);
			},
			[](Control::i_control& control, cstring value_string)
			{
				auto& gwen_control = GWEN_CTRL_REF(Base, control.GetControlPtr());

				bool value;
				ValueConversion::FromString(value_string, value);
				gwen_control.SetDisabled(value);
			}
		);

		Gwen::Controls::Base* c_control_builder_gwen_base::CreateInstance(Gwen::Controls::Base* parent) const
		{
			return new Gwen::Controls::Base(parent);
		}

		void c_control_builder_gwen_base::SetDefaultValues(Gwen::Controls::Base* control) const { }

		void c_control_builder_gwen_base::GetPropertyInterfaces(Control::t_property_interface_map& property_interfaces) const
		{
			property_interfaces[RESOURCE_ID_DEBUG("Position")] = GET_PROPERTY_INTERFACE(base_position);
			property_interfaces[RESOURCE_ID_DEBUG("Size")] = GET_PROPERTY_INTERFACE(base_size);
			property_interfaces[RESOURCE_ID_DEBUG("Dock")] = GET_PROPERTY_INTERFACE(base_dock);
			property_interfaces[RESOURCE_ID_DEBUG("Margin")] = GET_PROPERTY_INTERFACE(base_margin);
			property_interfaces[RESOURCE_ID_DEBUG("Padding")] = GET_PROPERTY_INTERFACE(base_padding);
			property_interfaces[RESOURCE_ID_DEBUG("Hidden")] = GET_PROPERTY_INTERFACE(base_hidden);
			property_interfaces[RESOURCE_ID_DEBUG("Disabled")] = GET_PROPERTY_INTERFACE(base_disabled);
		}

		void c_control_builder_gwen_base::GetEventHandlers(Gwen::Controls::Base* control, Control::t_event_handler_map& event_handlers) const
		{
			event_handlers[RESOURCE_ID_DEBUG("HoverEnter")] = std::make_unique<c_event_handler_gwen>(control->onHoverEnter);
			event_handlers[RESOURCE_ID_DEBUG("HoverLeave")] = std::make_unique<c_event_handler_gwen>(control->onHoverLeave);
		}
	};};};};
};
#endif