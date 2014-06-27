/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#include "Common/Precompile.hpp"
#include <blamlib/Halo1/hs/hs.hpp>

#include <blamlib/Halo1/hs/hs_structures.hpp>
#include <blamlib/Halo1/objects/object_types.hpp>

namespace Yelo
{
	namespace blam
	{
		using namespace Scripting;

		const std::array<word_flags, Enums::k_number_of_hs_object_types> k_hs_object_type_masks = {
			Enums::_object_type_mask_all, // engine actually uses 0xFFFF here
			Enums::_object_type_mask_unit,
			FLAG(Enums::_object_type_vehicle),
			FLAG(Enums::_object_type_weapon),
			Enums::_object_type_mask_device,
			FLAG(Enums::_object_type_scenery),
		};

		bool hs_type_valid(_enum type)
		{
			return type >= Enums::_hs_type_void && type < Enums::k_number_of_hs_types;
		}
		bool hs_type_is_enum(_enum type)
		{
			return type >= Enums::_hs_type_enum__first && type < Enums::_hs_type_enum__last;
		}
		bool hs_type_is_tag_reference(_enum type)
		{
			return type >= Enums::_hs_type_tag_reference__first && type < Enums::_hs_type_tag_reference__last;
		}
		bool hs_type_is_object(_enum type)
		{
			return type >= Enums::_hs_type_object__first && type < Enums::_hs_type_object__last;
		}
		bool hs_type_is_object_name(_enum type)
		{
			return type >= Enums::_hs_type_object_name__first && type < Enums::_hs_type_object_name__last;
		}

		void hs_nodes_garbage_collect()
		{
			// NOTE: engine seems to use a for(;;) using data_next_index() on syntax nodes
			for (auto node : HsSyntax())
			{
				if (TEST_FLAG(node->flags, Flags::_hs_syntax_node_dont_gc_bit))
					continue;

				HsSyntax().Delete(node.index);
			}
		}


		hs_syntax_node* hs_syntax_get(datum_index expression_index)
		{
			return HsSyntax()[expression_index];
		}
	};
};