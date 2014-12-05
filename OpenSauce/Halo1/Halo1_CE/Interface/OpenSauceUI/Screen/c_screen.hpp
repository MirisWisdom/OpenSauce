/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma region

#if !PLATFORM_IS_DEDI

#include "Interface/OpenSauceUI/i_control.hpp"
#include "Interface/OpenSauceUI/i_screen.hpp"

namespace Yelo
{
	namespace Interface { namespace OpenSauceUI { namespace Screen
	{
		/// <summary>	A screen implementation. </summary>
		class c_screen
			: public i_screen
		{
		private:
			t_control_ptr m_root_control;
			std::map<uint32, t_control_ptr> m_control_resource_list;

		public:
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Constructor. </summary>
			///
			/// <param name="root_control">	The screen's root control. </param>
			c_screen(t_control_ptr root_control);

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Gets the root control of the screen. </summary>
			///
			/// <returns>	The root control of the screen. </returns>
			t_control_ptr GetRootControl() const final override;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Gets a control based on it's resource id. </summary>
			///
			/// <param name="resource_id">	Identifier for the control resource. </param>
			///
			/// <returns>	The found control or nullptr if not found. </returns>
			t_control_ptr GetControl(const uint32 resource_id) final override;

		private:
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Adds a control to the resource list if it has an id defined. </summary>
			///
			/// <param name="control">	The control to add. </param>
			void AddControlResource(t_control_ptr control);
		};
	};};};
};
#endif