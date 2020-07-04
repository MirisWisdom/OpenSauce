/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI

#include "Interface/OpenSauceUI/Control/i_control.hpp"
#include "Interface/OpenSauceUI/ControlFactory/i_control_builder.hpp"
#include "Interface/OpenSauceUI/Definitions/c_control_definition.hpp"

namespace Yelo
{
	namespace Interface { namespace OpenSauceUI { namespace ControlFactory
	{
		class i_control_builder;

		/// <summary>	The OpenSauceUI control factory. </summary>
		class c_control_factory final
		{
			struct s_control_type
			{
				const std::string m_type_name;
				const std::shared_ptr<i_control_builder> m_control_builder;
			};

			std::vector<s_control_type> m_controls;

		private:
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Searches for a control type. </summary>
			///
			/// <param name="type_name">	Name of the type. </param>
			///
			/// <returns>	Iterator for the found control type. </returns>
			std::vector<s_control_type>::iterator FindControlType(const std::string& type_name);

		public:
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Adds a control type to the control factory. </summary>
			///
			/// <param name="type_name">	Name of the control type. </param>
			/// <param name="builder">  	The control's builder. </param>
			void AddControl(const std::string& type_name, std::shared_ptr<i_control_builder> builder);

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Builds a control according the the specified control definition. </summary>
			///
			/// <param name="parent">	 	[in] The parent control to add the new control to. </param>
			/// <param name="definition">	[in] The control definition. </param>
			///
			/// <returns>	A shared pointer for the new control. </returns>
			Control::control_ptr_t BuildControl(Control::i_control& parent, Definitions::c_control_definition& definition);
		};
	};};};
};
#endif