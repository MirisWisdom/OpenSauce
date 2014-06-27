/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

namespace Yelo
{
	namespace Scripting
	{
		struct s_object_list_header_datum;
		struct s_list_object_reference_datum;
	};

	namespace blam
	{
		void object_lists_garbage_collect();

		datum_index object_list_new();

		void object_list_delete(datum_index list_index);

		// Add [object_index] to [list_index]
		void PLATFORM_API object_list_add(datum_index list_index, datum_index object_index);

		// Get the object_index of the first object in a list of objects
		// [list_reference] - handle to the object_list's first s_list_object_reference_datum
		datum_index object_list_get_first(datum_index list_index, _Out_ datum_index& list_reference);

		// Get the next object_index in a list of objects
		// [list_reference]:
		// in  - handle to the current s_list_object_reference_datum in the object_list we're referencing
		// out - handle to the next s_list_object_reference_datum
		datum_index object_list_get_next(datum_index list_index, _Inout_ datum_index& list_reference);

		void object_list_add_reference(datum_index list_index);

		void object_list_remove_reference(datum_index list_index);

		int16 object_list_count(datum_index list_index);


		Scripting::s_object_list_header_datum* object_list_get(datum_index list_index);

		Scripting::s_list_object_reference_datum* object_list_object_get(datum_index list_reference_index);
	};
};