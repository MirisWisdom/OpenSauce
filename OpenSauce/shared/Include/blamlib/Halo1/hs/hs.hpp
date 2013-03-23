/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

#include <blamlib/memory/data_base.hpp>
#include <blamlib/Halo1/hs/hs_scenario_definitions.hpp>
#include <blamlib/Halo1/hs/hs_structures.hpp>
#include <YeloLib/Halo1/hs/hs_yelo.hpp>

namespace Yelo
{
	namespace Scripting
	{
		typedef Memory::DataArray<	hs_syntax_node, 
									Enums::k_maximum_hs_syntax_nodes_per_scenario, 
									Enums::k_maximum_hs_syntax_nodes_per_scenario_upgrade> 
			hs_syntax_data_t;
	};

	namespace blam
	{
		Scripting::hs_syntax_node* hs_syntax_get(datum_index expression_index);
	};
};