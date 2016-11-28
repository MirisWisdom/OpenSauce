/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

namespace Yelo
{
	namespace Enums
	{
		enum {
			k_maximum_hs_dynamic_source_data_bytes = 1024, // HS_MAXIMUM_DYNAMIC_SOURCE_DATA_BYTES

			k_maximum_hs_syntax_nodes_per_scenario = 19001,
			k_maximum_hs_string_data_per_scenario = 256 * 1024,

			k_maximum_hs_scripts_per_scenario = 512,
			k_maximum_hs_globals_per_scenario = 128,
			k_maximum_hs_references_per_scenario = 256,

			k_maximum_hs_source_files_per_scenario = 8,
			k_maximum_hs_source_data_per_file = 0x40000
		};
	};
};