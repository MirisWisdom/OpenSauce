/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#include "Common/Precompile.hpp"
#include <blamlib/Halo1/hs/object_lists.hpp>

#include <blamlib/Halo1/hs/object_lists_structures.hpp>

namespace Yelo
{
	namespace blam
	{
		using namespace Scripting;

		void object_lists_garbage_collect()
		{
			// NOTE: engine seems to use a for(;;) using data_next_index() on list headers
			for (auto list : ObjectListHeader())
			{
				if (list->count == 0)
					object_list_delete(list.index);
			}
		}

		datum_index object_list_new()
		{
			datum_index list_index = blam::datum_new( ObjectListHeader() );

			if (!list_index.IsNull())
			{
				s_object_list_header_datum* list = object_list_get(list_index);

				list->count = 0;
				list->first = datum_index::null;
			}

			return list_index;
		}

		void object_list_delete(datum_index list_index)
		{
			if (list_index.IsNull())
				return;

			s_object_list_header_datum* list = object_list_get(list_index);

			YELO_ASSERT(list->reference_count==0);

			// delete all reference datums used in this list
			for (datum_index reference_index = list->first; !reference_index.IsNull(); )
			{
				s_list_object_reference_datum* reference = object_list_object_get(reference_index);
				datum_index next_reference_index = reference->next_reference;

				ListObjectReference().Delete(reference_index);

				// NOTE: engine doesn't make a copy of the next index before calling delete.
				// meaning the engine assumes datum_delete() doesn't change any bits of the datum besides the header.
				// we're not going to run with this assumption, so we make a copy before the delete() above
				reference_index = next_reference_index;
			}

			ObjectListHeader().Delete(list_index);
		}


		datum_index object_list_get_first(datum_index list_index, _Out_ datum_index& list_reference)
		{
			if (!list_index.IsNull())
			{
				s_object_list_header_datum* list = object_list_get(list_index);
				list_reference = list->first;

				return object_list_get_next(list_index, list_reference);
			}

			return datum_index::null;
		}

		datum_index object_list_get_next(datum_index list_index, _Inout_ datum_index& list_reference)
		{
			if (!list_reference.IsNull())
			{
				s_list_object_reference_datum* object_reference = ListObjectReference()[list_reference];
				list_reference = object_reference->next_reference;

				return object_reference->object_index;
			}

			return datum_index::null;
		}

		void object_list_add_reference(datum_index list_index)
		{
			if (list_index.IsNull())
				return;

			s_object_list_header_datum* list = object_list_get(list_index);

			list->reference_count++;
		}

		void object_list_remove_reference(datum_index list_index)
		{
			if (list_index.IsNull())
				return;

			s_object_list_header_datum* list = object_list_get(list_index);

			YELO_ASSERT(list->reference_count>0);
			list->reference_count--;
		}

		int16 object_list_count(datum_index list_index)
		{
			if (list_index.IsNull())
				return 0;

			s_object_list_header_datum* list = object_list_get(list_index);

			return list->count;
		}


		s_object_list_header_datum* object_list_get(datum_index list_index)
		{
			return ObjectListHeader()[list_index];
		}
		s_list_object_reference_datum* object_list_object_get(datum_index list_reference_index)
		{
			return ListObjectReference()[list_reference_index];
		}
	};
};