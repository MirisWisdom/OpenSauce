/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

#include <blamlib/memory/data_base.hpp>
#include <blamlib/Halo1/hs/hs.hpp>

namespace Yelo
{
	namespace Flags
	{
		enum hs_syntax_node_flags : word_flags
		{
			_hs_syntax_node_primitive_bit,
			_hs_syntax_node_script_index_bit,
			_hs_syntax_node_global_index_bit,
			_hs_syntax_node_dont_gc_bit,

			/// <summary>	node is an index to a script parameter. </summary>
			_hs_syntax_node_script_parameter_index_yelo_bit,
			/// <summary>	node is an index to a script's local variable. </summary>
			_hs_syntax_node_script_local_index_yelo_bit,
			/// <summary>	node is an index to a constant. </summary>
			_hs_syntax_node_const_index_yelo_bit,
		};

		enum hs_access_flag : word_flags
		{
			_hs_access_flag_none,
			_hs_access_flag_enabled =		FLAG(0),
			_hs_access_flag_sent_to_server =FLAG(1), ///< automatically sent to server
			_hs_access_flag_rconable =		FLAG(2), ///< can be executed from an rcon command
			_hs_access_flag_client =		FLAG(3), ///< only a client connection can execute it
			_hs_access_flag_server =		FLAG(4), ///< only a server connection can execute it
		};
	};

	namespace Scripting
	{
		typedef void (PLATFORM_API* hs_parse_proc)(int32 function_index, datum_index expression_index);
		typedef void (PLATFORM_API* hs_evaluate_proc)(int32 function_index, datum_index expression_index, bool* execute);

		// halo script function definition
		struct hs_function_definition
		{
			Enums::hs_type return_type;
			word_flags flags; // padding in halo, special flags in project yellow
			cstring name;
			hs_parse_proc parse;
			hs_evaluate_proc evaluate;
			cstring info;
			cstring param_info;
			Flags::hs_access_flag access;
			int16 paramc;
#pragma warning( push )
#pragma warning( disable : 4200 ) // nonstandard extension used : zero-sized array in struct/union, Cannot generate copy-ctor or copy-assignment operator when UDT contains a zero-sized array
			Enums::hs_type params[];
#pragma warning( pop )
		}; BOOST_STATIC_ASSERT( sizeof(hs_function_definition) == 0x1C ); // size doesn't include [params]

		// halo script accessible value
		struct hs_global_definition
		{
			cstring name;
			Enums::hs_type type;
			word_flags flags; // padding in halo, special flags in project yellow
			union {
				void* address;

				union {
					bool* _bool;
					real* _real;
					int16* _short;
					int32* _long;
					char* _string;
					datum_index* _datum;
				}Value;
			};
			Flags::hs_access_flag access;
			PAD16;
		}; BOOST_STATIC_ASSERT( sizeof(hs_global_definition) == 0x10 );


		struct hs_syntax_node : Memory::s_datum_base
		{
			union {
				Enums::hs_type constant_type;
				int16 function_index;
				int16 script_index;
			};
			Enums::hs_type type;
			union {
				word_flags flags;
				int16 pointer_type;
			};
			datum_index next_expression;
			int32 source_offset;
			union {
				void* address;

				TypeHolder value;
			};
		}; BOOST_STATIC_ASSERT( sizeof(hs_syntax_node) == 0x14 );

		typedef Memory::DataArray<	hs_syntax_node, 
									Enums::k_maximum_hs_syntax_nodes_per_scenario, 
									Enums::k_maximum_hs_syntax_nodes_per_scenario_upgrade> 
			hs_syntax_data_t;

		hs_syntax_data_t&			HsSyntax();
	};
};