/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI

#include <Gwen/Controls/Label.h>

namespace Yelo
{
	namespace Interface { namespace OpenSauceUI { namespace GwenUI { namespace ControlBuilders
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the text from a label. </summary>
		///
		/// <param name="label">	[in] The label. </param>
		///
		/// <returns>	The text in a std::string. </returns>
		std::string Label_GetText(Gwen::Controls::Label& label);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	sets the text of a label. </summary>
		///
		/// <param name="label">	[in] The label. </param>
		/// <param name="value">	The value to set. </param>
		void Label_SetText(Gwen::Controls::Label& label, const std::string& value);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the label's typeface. </summary>
		///
		/// <param name="label">	[in] The label. </param>
		///
		/// <returns>	A std::string containing the typeface name. </returns>
		std::string Label_GetTextTypeface(Gwen::Controls::Label& label);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Set's the typeface of a label. </summary>
		///
		/// <param name="label">	[in] The label. </param>
		/// <param name="value">	The typeface. </param>
		void Label_SetTextTypeface(Gwen::Controls::Label& label, const std::string& value);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the text size of a label. </summary>
		///
		/// <param name="label">	[in] The label. </param>
		///
		/// <returns>	An int32. </returns>
		int32 Label_GetTextSize(Gwen::Controls::Label& label);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets the text size of a label. </summary>
		///
		/// <param name="label">	[in] The label. </param>
		/// <param name="value">	The value to set. </param>
		void Label_SetTextSize(Gwen::Controls::Label& label, const int32 value);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets whether a labels text is bold. </summary>
		///
		/// <param name="label">	[in] The label. </param>
		///
		/// <returns>	true if bold is enabled, false if not. </returns>
		bool Label_GetTextBold(Gwen::Controls::Label& label);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets whether a labels text is bold. </summary>
		///
		/// <param name="label">	[in] The label. </param>
		/// <param name="value">	The value. </param>
		void Label_SetTextBold(Gwen::Controls::Label& label, const bool value);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets a labels text color. </summary>
		///
		/// <param name="label">	[in] The label. </param>
		///
		/// <returns>	The text color. </returns>
		argb_color Label_GetTextColor(Gwen::Controls::Label& label);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets a labels text color. </summary>
		///
		/// <param name="label">	[in] The label. </param>
		/// <param name="value">	The color to use. </param>
		void Label_SetTextColor(Gwen::Controls::Label& label, const argb_color& value);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets a labels horizontal alignment. </summary>
		///
		/// <param name="label">	[in] The label. </param>
		///
		/// <returns>	The alignment. </returns>
		int32 Label_GetHorizontalAlignment(Gwen::Controls::Label& label);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets a labels horizontal alignment. </summary>
		///
		/// <param name="label">	[in] The label. </param>
		/// <param name="value">	The alignment to use. </param>
		void Label_SetHorizontalAlignment(Gwen::Controls::Label& label, const int32 value);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets a labels horizontal alignment. </summary>
		///
		/// <param name="label">	[in] The label. </param>
		/// <param name="value">	The alignment to use. </param>
		void Label_SetHorizontalAlignment(Gwen::Controls::Label& label, const std::string& value);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets a labels vertical alignment. </summary>
		///
		/// <param name="label">	[in] The label. </param>
		///
		/// <returns>	The alignment. </returns>
		int32 Label_GetVerticalAlignment(Gwen::Controls::Label& label);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets a labels vertical alignment. </summary>
		///
		/// <param name="label">	[in] The label. </param>
		/// <param name="value">	The alignment to use. </param>
		void Label_SetVerticalAlignment(Gwen::Controls::Label& label, const int32 value);
		
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets a labels vertical alignment. </summary>
		///
		/// <param name="label">	[in] The label. </param>
		/// <param name="value">	The alignment to use. </param>
		void Label_SetVerticalAlignment(Gwen::Controls::Label& label, const std::string& value);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets whether a labels text is wrapped. </summary>
		///
		/// <param name="label">	[in] The label. </param>
		///
		/// <returns>	true if it is wrapped, false if not. </returns>
		bool Label_GetWrap(Gwen::Controls::Label& label);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets whether a labels text is wrapped. </summary>
		///
		/// <param name="label">	[in] The label. </param>
		/// <param name="value">	The wrap value. </param>
		void Label_SetWrap(Gwen::Controls::Label& label, bool value);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets a labels text padding. </summary>
		///
		/// <param name="label">	[in] The label. </param>
		///
		/// <returns>	A rectangle2d containing the padding amounts. </returns>
		rectangle2d Label_GetTextPadding(Gwen::Controls::Label& label);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets a labels text padding. </summary>
		///
		/// <param name="label">	[in] The label. </param>
		/// <param name="value">	The padding to use. </param>
		void Label_SetTextPadding(Gwen::Controls::Label& label, const rectangle2d& value);

	};};};};
};
#endif