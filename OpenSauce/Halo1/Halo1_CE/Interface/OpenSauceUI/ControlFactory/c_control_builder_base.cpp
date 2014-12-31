/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Interface/OpenSauceUI/ControlFactory/c_control_builder_base.hpp"

#if !PLATFORM_IS_DEDI

namespace Yelo
{
	namespace Interface { namespace OpenSauceUI { namespace ControlFactory
	{
		void c_control_builder_base::BuildChildren(c_control_factory& control_registry
			, std::vector<Definitions::c_control_definition>& control_definitions
			, Control::i_control& parent) const
		{
			// Create controls for all of the child control definitions
			for(auto& child: control_definitions)
			{
				control_registry.BuildControl(parent, child);
			}
		}
	};};};
};
#endif