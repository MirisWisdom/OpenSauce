/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

#include <blamlib/Halo1/tag_files/tag_groups.hpp>

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

	namespace TagGroups
	{
		struct hs_scenario_data_base
		{
			struct _script {
				TAG_ENUM(type, Enums::hs_script_type);
				TAG_ENUM(return_type, Enums::hs_type);
				TAG_FIELD(datum_index, root_expression_index);
			};
			struct _global {
				TAG_ENUM(type, Enums::hs_type);
				PAD16;
				PAD32;
				TAG_FIELD(datum_index, initialization_expression_index);
			};
			TAG_FIELD(tag_string, name);
			union {
				TAG_PAD(int32, 15); // 60
				_script script;
				_global global;
			};
		};
		struct hs_script			: hs_scenario_data_base	{}; BOOST_STATIC_ASSERT( sizeof(hs_script) == 0x5C );
		struct hs_global_internal	: hs_scenario_data_base	{}; BOOST_STATIC_ASSERT( sizeof(hs_global_internal) == 0x5C );
		struct hs_tag_reference
		{
			TAG_PAD(int32, 6); // 24
			TAG_FIELD(tag_reference, reference);
		}; BOOST_STATIC_ASSERT( sizeof(hs_tag_reference) == 0x28 );
		struct hs_source_file
		{
			TAG_FIELD(tag_string, name);
			TAG_FIELD(tag_data, source);
		}; BOOST_STATIC_ASSERT( sizeof(hs_source_file) == 0x34 );
	};
};