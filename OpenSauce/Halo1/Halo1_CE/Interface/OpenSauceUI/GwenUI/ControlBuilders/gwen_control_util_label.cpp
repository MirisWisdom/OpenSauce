/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Interface/OpenSauceUI/GwenUI/ControlBuilders/gwen_control_util_label.hpp"

#if !PLATFORM_IS_DEDI

namespace Yelo
{
	namespace Interface { namespace OpenSauceUI { namespace GwenUI { namespace ControlBuilders
	{
		std::string Label_GetText(Gwen::Controls::Label& label)
		{
			return std::string(label.GetText().c_str());
		}

		void Label_SetText(Gwen::Controls::Label& label, const std::string& value)
		{
			label.SetText(value);
		}

		std::string Label_GetTextTypeface(Gwen::Controls::Label& label)
		{
			auto& font = *label.GetFont();
			return std::string(font.facename.begin(), font.facename.end());
		}

		void Label_SetTextTypeface(Gwen::Controls::Label& label, const std::string& value)
		{
			auto& font = *label.GetFont();

			auto value_wide = std::wstring(value.begin(), value.end());
			if(font.facename != value_wide)
			{
				label.SetFont(value_wide, font.size, font.bold);
			}
		}

		int32 Label_GetTextSize(Gwen::Controls::Label& label)
		{
			auto& font = *label.GetFont();
			return font.size;
		}

		void Label_SetTextSize(Gwen::Controls::Label& label, const int32 value)
		{
			auto& font = *label.GetFont();

			if(font.size != value)
			{
				label.SetFont(font.facename, value, font.bold);
			}
		}

		bool Label_GetTextBold(Gwen::Controls::Label& label)
		{
			auto& font = *label.GetFont();
			return font.bold;
		}

		void Label_SetTextBold(Gwen::Controls::Label& label, const bool value)
		{
			auto& font = *label.GetFont();

			if(font.bold != value)
			{
				label.SetFont(font.facename, font.size, value);
			}
		}

		argb_color Label_GetTextColor(Gwen::Controls::Label& label)
		{
			auto& color = label.TextColor();
			return argb_color { color.a, color.r, color.g, color.b };
		}

		void Label_SetTextColor(Gwen::Controls::Label& label, const argb_color& value)
		{
			label.SetTextColor(Gwen::Color(value.red, value.green, value.blue, value.alpha));
			label.SetTextColorOverride(Gwen::Color(value.red, value.green, value.blue, value.alpha));
		}

		int32 Label_GetHorizontalAlignment(Gwen::Controls::Label& label)
		{
			int32 alignment = label.GetAlignment();

			alignment &= ~Gwen::Pos::Top;
			alignment &= ~Gwen::Pos::CenterV;
			alignment &= ~Gwen::Pos::Bottom;

			return alignment;
		}

		void Label_SetHorizontalAlignment(Gwen::Controls::Label& label, const int32 value)
		{
			int32 alignment = label.GetAlignment();

			alignment &= ~Gwen::Pos::Left;
			alignment &= ~Gwen::Pos::CenterH;
			alignment &= ~Gwen::Pos::Right;

			alignment |= value;

			label.SetAlignment(alignment);
		}

		void Label_SetHorizontalAlignment(Gwen::Controls::Label& label, const std::string& value)
		{
			if(value == "Left")
			{
				Label_SetHorizontalAlignment(label, Gwen::Pos::Left);
			}
			else if((value == "Center") || (value == "Centre"))
			{
				Label_SetHorizontalAlignment(label, Gwen::Pos::CenterH);
			}
			else if(value == "Right")
			{
				Label_SetHorizontalAlignment(label, Gwen::Pos::Right);
			}
		}

		int32 Label_GetVerticalAlignment(Gwen::Controls::Label& label)
		{
			int32 alignment = label.GetAlignment();

			alignment &= ~Gwen::Pos::Left;
			alignment &= ~Gwen::Pos::CenterH;
			alignment &= ~Gwen::Pos::Right;

			return alignment;
		}

		void Label_SetVerticalAlignment(Gwen::Controls::Label& label, const int32 value)
		{
			int32 alignment = label.GetAlignment();

			alignment &= ~Gwen::Pos::Top;
			alignment &= ~Gwen::Pos::CenterV;
			alignment &= ~Gwen::Pos::Bottom;

			alignment |= value;

			label.SetAlignment(alignment);
		}

		void Label_SetVerticalAlignment(Gwen::Controls::Label& label, const std::string& value)
		{
			if(value == "Top")
			{
				Label_SetVerticalAlignment(label, Gwen::Pos::Top);
			}
			else if((value == "Center") || (value == "Centre"))
			{
				Label_SetVerticalAlignment(label, Gwen::Pos::CenterV);
			}
			else if(value == "Bottom")
			{
				Label_SetVerticalAlignment(label, Gwen::Pos::Bottom);
			}
		}

		bool Label_GetWrap(Gwen::Controls::Label& label)
		{
			return label.Wrap();
		}

		void Label_SetWrap(Gwen::Controls::Label& label, bool value)
		{
			label.SetWrap(value);
		}

		rectangle2d Label_GetTextPadding(Gwen::Controls::Label& label)
		{
			auto value = label.GetTextPadding();
			return rectangle2d { value.top, value.left, value.bottom, value.right };
		}

		void Label_SetTextPadding(Gwen::Controls::Label& label, const rectangle2d& value)
		{
			label.SetTextPadding(Gwen::Padding(value.left, value.top, value.right, value.bottom));
		}
	};};};};
};
#endif