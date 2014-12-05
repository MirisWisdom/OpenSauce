/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI

#include <Gwen/Gwen.h>
#include <Gwen/Controls/Base.h>

#include "Interface/OpenSauceUI/c_control_base.hpp"

namespace Yelo
{
	namespace Interface { namespace OpenSauceUI { namespace GwenUI
	{
		/// <summary>	An OpenSauceUI control using Gwen. </summary>
		class c_control_gwen final
			: public c_control_base
		{
			Gwen::Controls::Base* m_control;

		public:
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Constructor. </summary>
			///
			/// <param name="control">	[in] The Gwen control. </param>
			c_control_gwen(Gwen::Controls::Base* control);
			
#pragma region i_control
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Gets the Gwen base control pointer. </summary>
			///
			/// <returns>	The Gwen base control pointer. </returns>
			void* GetControlPtr() const override;
#pragma endregion
		};
	};};};
};
#endif