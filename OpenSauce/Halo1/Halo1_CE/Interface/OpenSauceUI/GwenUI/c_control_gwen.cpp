/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Interface/OpenSauceUI/GwenUI/c_control_gwen.hpp"

#if !PLATFORM_IS_DEDI

namespace Yelo
{
	namespace Interface { namespace OpenSauceUI { namespace GwenUI
	{
		c_control_gwen::c_control_gwen(Gwen::Controls::Base* control)
			: c_control_base()
			, m_control(control)
		{ }
			
#pragma region i_control
		void* c_control_gwen::GetControlPtr() const
		{
			return m_control;
		}
#pragma endregion
	};};};
};
#endif