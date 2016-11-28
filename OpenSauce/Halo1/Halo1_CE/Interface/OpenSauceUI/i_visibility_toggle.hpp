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
		/// <summary>	Interface for toggling a ui objects visibility. </summary>
		class i_visibility_toggle
			abstract
		{
		public:
			/// <summary>	Shows the object. </summary>
			virtual void Show() = 0;

			/// <summary>	Hides the object. </summary>
			virtual void Hide() = 0;
		};
	};};
};
#endif