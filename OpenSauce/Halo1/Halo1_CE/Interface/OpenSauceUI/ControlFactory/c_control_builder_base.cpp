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
		void c_control_builder_base::BuildChildren(c_control_factory& control_registry, Definitions::c_control_definition& control_definition, i_control& parent) const
		{
			for(auto& child: control_definition.m_controls)
			{
				control_registry.BuildControl(parent, child);
			}
		}
	};};};
};
#endif