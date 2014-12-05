/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI

#include "Interface/OpenSauceUI/i_control.hpp"

namespace Yelo
{
	namespace Interface { namespace OpenSauceUI
	{
		/// <summary>	The OpenSauceUI base control. </summary>
		class c_control_base
			abstract
			: public i_control
		{
		private:
			uint32 m_resource_id;
			t_control_list m_child_controls;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Searches for a matching control in the child control list. </summary>
			///
			/// <param name="control">	The control to find. </param>
			///
			/// <returns>	The found control. </returns>
			t_control_list::const_iterator FindControl(const t_control_ptr control) const;

		public:
			/// <summary>	Default constructor. </summary>
			c_control_base();
			
#pragma region i_control
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Gets the resource identifier of the control. </summary>
			///
			/// <returns>	The controls resource identifier. </returns>
			uint32 GetResourceID() const final override;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Sets the resource identifier of the control. </summary>
			///
			/// <param name="resource_id">	Identifier for the control resource. </param>
			void SetResourceID(const uint32 resource_id) final override;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Adds a control to the child control list. </summary>
			///
			/// <remarks>	If the specified control has already been added the action is ignored. </remarks>
			///
			/// <param name="control">	The control to add. </param>
			void AddControl(t_control_ptr control) final override;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Removes the specified control from the child control list. </summary>
			///
			/// <remarks>	If the specified control does not exist, the action is ignored. </remarks>
			///
			/// <param name="control">	The control to remove. </param>
			void RemoveControl(t_control_ptr control) final override;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Gets the controls list. </summary>
			///
			/// <returns>	A list of controls. </returns>
			t_control_list& Controls() final override;
#pragma endregion
		};
	};};
};
#endif