/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Interface/OpenSauceUI/GwenUI/ControlBuilders/c_control_builder_gwen_verticalslider.hpp"

#if !PLATFORM_IS_DEDI

#include <Gwen/Controls/VerticalSlider.h>

namespace Yelo
{
	namespace Interface { namespace OpenSauceUI { namespace GwenUI { namespace ControlBuilders
	{
		Gwen::Controls::Base* c_control_builder_gwen_verticalslider::CreateInstance(Gwen::Controls::Base* parent) const
		{
			return new Gwen::Controls::VerticalSlider(parent);
		}
	};};};};
};
#endif