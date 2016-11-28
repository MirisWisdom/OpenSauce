/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Interface/OpenSauceUI/GwenUI/ControlBuilders/c_control_builder_gwen_pointer.hpp"

#if !PLATFORM_IS_DEDI

#include <Gwen/Controls/Pointer.h>
#include <YeloLib/cseries/value_conversion.hpp>

#include "Interface/OpenSauceUI/Control/control_property_ids.hpp"
#include "Interface/OpenSauceUI/Control/c_control_base.hpp"

namespace Yelo
{
	namespace Interface { namespace OpenSauceUI { namespace GwenUI { namespace ControlBuilders
	{
		Gwen::Controls::Base* c_control_builder_gwen_pointer::CreateInstance(Gwen::Controls::Base* parent) const
		{
			return new Gwen::Controls::Pointer(parent);
		}

		void c_control_builder_gwen_pointer::SetDefaultValues(Gwen::Controls::Base* control) const
		{
			c_control_builder_gwen_base::SetDefaultValues(control);

			auto& gwen_control = GWEN_CTRL_REF(Pointer, control);

			gwen_control.SetSize(30, 30);
		}
	};};};};
};
#endif