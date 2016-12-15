/*
	Yelo: Open Sauce SDK
		Halo 2 (Xbox) Edition

	See license\OpenSauce\Halo2_Xbox for specific license information
*/
#pragma once

#include <blamlib/Halo2/hs/hs.hpp>
#include <blamlib/Halo2/memory/data.hpp>

namespace Yelo
{
	namespace GameState
	{
		struct s_object_list_header_datum : Memory::s_datum_base_aligned
		{
			int16 reference_count;	// how many references there are to this list
			int16 count;			// how many objects are in the list
			datum_index first;		// handle to the first list object reference
		};
		typedef Memory::DataArray<s_object_list_header_datum, 48> t_object_list_header_data;
		t_object_list_header_data* _ObjectListHeader();

		struct s_list_object_reference_datum : Memory::s_datum_base_aligned
		{
			datum_index object_index;	// handle to the object datum
			datum_index next_reference;	// handle to the next list object reference in the list
		};
		typedef Memory::DataArray<s_list_object_reference_datum, 128> t_list_object_reference_data;
		t_list_object_reference_data* _ListObjectReference();

		typedef Memory::DataArray<Scripting::hs_syntax_node, 31744> t_hs_nodes_data;
		t_hs_nodes_data* _HsNodes();

		struct s_hs_thread_datum : TStructImpl(1048)
		{
		};
		typedef Memory::DataArray<s_hs_thread_datum, 320> t_hs_thread_data;
		t_hs_thread_data* _HsThread();

		struct s_hs_globals_datum : Memory::s_datum_base
		{
			UNKNOWN_TYPE(int16); // TODO:
			union _Value{
				bool Boolean;
				short Short;
				long Long;
				datum_index Datum;
			}Value;
		};
		typedef Memory::DataArray<s_hs_globals_datum, 3072> t_hs_globals_data;
		t_hs_globals_data* _HsGlobals();

		struct s_recorded_animations_datum : TStructImpl(160)
		{
		};
		typedef Memory::DataArray<s_recorded_animations_datum, 1> t_recorded_animations_data;
		t_recorded_animations_data* _RecordedAnimations();
	};
};