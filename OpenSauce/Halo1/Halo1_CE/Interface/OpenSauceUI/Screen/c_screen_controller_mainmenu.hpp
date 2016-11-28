/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI

#include "Interface/OpenSauceUI/Screen/c_screen_controller_base.hpp"

namespace Yelo
{
	namespace Interface { namespace OpenSauceUI { namespace Screen
	{
		/// <summary>	The main menu screen controller. </summary>
		class c_screen_controller_mainmenu final
			: public c_screen_controller_base
		{
		public:
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Constructor. </summary>
			///
			/// <param name="definition">	[in] The screen definition. </param>
			c_screen_controller_mainmenu(Definitions::c_screen_definition& definition);

		private:
			/// <summary>	Sets a screens static properties. </summary>
			void SetStaticProperties() override;

			/// <summary>	Binds a screens dynamic properties. </summary>
			void BindDynamicProperties() override;

			/// <summary>	Binds a screens events. </summary>
			void BindEvents() override;

			/// <summary>	Unbinds a screens events. </summary>
			void UnbindEvents() override;
		};
	};};};
};
#endif