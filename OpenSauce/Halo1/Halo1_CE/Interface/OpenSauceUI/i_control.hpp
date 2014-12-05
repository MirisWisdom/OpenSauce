/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI

namespace Yelo
{
	namespace Interface { namespace OpenSauceUI
	{
		class i_control;

		/// <summary>	Defines an alias representing the a shared control pointer. </summary>
		typedef std::shared_ptr<i_control> t_control_ptr;
		/// <summary>	Defines an alias representing a list of shared controls pointers. </summary>
		typedef std::vector<t_control_ptr> t_control_list;

		/// <summary>	Base interface for an OpenSauceUI control. </summary>
		class i_control
			abstract
		{
		public:
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Gets the resource identifier of the control. </summary>
			///
			/// <returns>	The controls resource identifier. </returns>
			virtual uint32 GetResourceID() const = 0;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Sets the resource identifier of the control. </summary>
			///
			/// <param name="resource_id">	Identifier for the control resource. </param>
			virtual void SetResourceID(const uint32 resource_id) = 0;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Gets a raw pointer to an implementation specific control object. </summary>
			///
			/// <returns>	Returns the implementation specific control pointer. </returns>
			virtual void* GetControlPtr() const = 0;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Adds a control to the child control list. </summary>
			///
			/// <remarks>	If the specified control has already been added the action is ignored. </remarks>
			///
			/// <param name="control">	The control to add. </param>
			virtual void AddControl(t_control_ptr control) = 0;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Removes the specified control from the child control list. </summary>
			///
			/// <remarks>	If the specified control does not exist, the action is ignored. </remarks>
			///
			/// <param name="control">	The control to remove. </param>
			virtual void RemoveControl(t_control_ptr control) = 0;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Gets the controls list. </summary>
			///
			/// <returns>	A list of controls. </returns>
			virtual t_control_list& Controls() = 0;
		};
	};};
};
#endif