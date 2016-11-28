/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

#include <blamlib/Halo1/hs/hs_constants.hpp>

#include <YeloLib/tag_files/tag_groups_base_yelo.hpp>

namespace Yelo
{
	namespace Enums
	{
		enum hs_type : _enum;
		enum hs_script_type : _enum;
	};

	namespace TagGroups
	{
		struct hs_scenario_data_base
		{
			struct _script {
				Enums::hs_script_type type;
				Enums::hs_type return_type;
				TAG_FIELD(datum_index, root_expression_index);
			};
			struct _global {
				Enums::hs_type type;
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

	namespace blam
	{
		bool PLATFORM_API hs_scenario_postprocess(bool for_runtime);
	};
};