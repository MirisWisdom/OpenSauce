/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#pragma once

#include <blamlib/Halo1/tag_files/tag_groups_structures_macros.hpp>
#include <YeloLib/Halo1/tag_files/tag_fieldset_replacement_builder.hpp>
#include <blamlib/Halo1/models/model_definitions.hpp>

namespace Yelo
{
	namespace TagGroups { namespace TagFieldReplacements { namespace GBXModel
	{
		static void GeometryPartBlockFixActualFields(tag_block_definition* model_geometry_part_block)
		{
			assert(0==strcmp("gbxmodel_geometry_part_block", model_geometry_part_block->name));

			// find the last declared field
			int field_index = model_geometry_part_block->FindFieldIndex(Enums::_field_block, "triangles");
			assert(field_index != NONE);

			// move to triangle_buffer
			field_index++;
			assert(model_geometry_part_block->fields[field_index].type==Enums::_field_pad);

			// move to vertex_buffer
			field_index++;
			assert(model_geometry_part_block->fields[field_index].type==Enums::_field_pad);

			// move to num_nodes
			// fucking gbx defines num_nodes as pad, pretending it is a runtime field, but it isn't calculated at runtime
			field_index += 4;
			assert(model_geometry_part_block->fields[field_index].type == Enums::_field_pad);
			{
				tag_field& num_nodes_field = model_geometry_part_block->fields[field_index];
				assert(num_nodes_field.DefinitionCast<int32>()==sizeof(sbyte));

				num_nodes_field.type = Enums::_field_char_integer;
				num_nodes_field.name = "num_nodes"
					TAG_FIELD_MARKUP_IS_READONLY;
				num_nodes_field.definition = nullptr;
			}

			// move to node_table
			field_index++;
			assert(model_geometry_part_block->fields[field_index].type == Enums::_field_pad);
			{
				const int32 k_node_table_field_pad_size =
					(sizeof(sbyte) * Enums::k_maximum_nodes_per_model_geometry_part) +
					sizeof(int16);

				tag_field& node_table_field = model_geometry_part_block->fields[field_index];
				assert(node_table_field.DefinitionCast<int32>() == k_node_table_field_pad_size);

				node_table_field.type = Enums::_field_skip;
			}
		}

		static void Initialize()
		{
			tag_group* gbxmodel = blam::tag_group_get<gbxmodel_definition>();
			assert(gbxmodel);

			auto* block = gbxmodel->header_block_definition;
			// find gbxmodel->geometries
			int field_index = block->FindFieldIndex(Enums::_field_block, "geometries");
			assert(field_index != NONE);

			block = block->fields[field_index].Definition<tag_block_definition>();
			// find gbxmodel_geometry->parts
			field_index = block->FindFieldIndex(Enums::_field_block, "parts");
			assert(field_index != NONE);

			block = block->fields[field_index].Definition<tag_block_definition>();

			GeometryPartBlockFixActualFields(block);
		}
	};};};
};