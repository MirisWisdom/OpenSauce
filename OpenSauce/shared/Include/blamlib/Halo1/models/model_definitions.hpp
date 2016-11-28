/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <blamlib/Halo1/rasterizer/rasterizer_geometry.hpp>

#include <YeloLib/tag_files/tag_groups_base_yelo.hpp>

namespace Yelo
{
	namespace Enums
	{
		enum {
			k_maximum_regions_per_model = 8,

			k_maximum_nodes_per_model = 64,
			k_maximum_nodes_per_model_halo_stock = 44, // 'Cause Xbox1 has limits

			k_maximum_nodes_per_model_geometry_part = 22,

			k_number_of_rows_per_node_matrix = 4,
		};
	};

	namespace TagGroups
	{
		struct model_marker_instance
		{
			TAG_FIELD(sbyte, region_index);
			TAG_FIELD(sbyte, permutation_index);
			TAG_FIELD(sbyte, node_index);
			PAD8;
			TAG_FIELD(real_point3d, translation);
			TAG_FIELD(real_quaternion, rotation);
		}; BOOST_STATIC_ASSERT( sizeof(model_marker_instance) == 0x20 ); // max count: 32
		struct model_markers
		{
			TAG_FIELD(tag_string, name);
			TAG_FIELD(int16, magic_identifier);
			PAD16;
			TAG_PAD(int32, 4);
			TAG_TBLOCK(instances, model_marker_instance);
		}; BOOST_STATIC_ASSERT( sizeof(model_markers) == 0x40 ); // max count: 256

		struct model_node
		{
			TAG_FIELD(tag_string, name);
			TAG_FIELD(int16, next_sibling_node_index);
			TAG_FIELD(int16, first_child_node_index);
			TAG_FIELD(int16, parent_node_index);
			PAD16;
			//TAG_FIELD(real_point3d, default_translation);
			//TAG_FIELD(real_quaternion, default_rotation);
			//TAG_FIELD(real, node_distance_from_parent);
			real_orientation3d orientation;
			TAG_PAD(int32, 8);
			real_matrix4x3 inverse_matrix;
		}; BOOST_STATIC_ASSERT( sizeof(model_node) == 0x9C ); // max count: 64

		struct model_region_permutation_marker
		{
			TAG_FIELD(tag_string, name);
			TAG_FIELD(int16, node_index);
			PAD16;
			TAG_FIELD(real_quaternion, rotation);
			TAG_FIELD(real_point3d, translation);
			TAG_PAD(int32, 4);
		}; BOOST_STATIC_ASSERT( sizeof(model_region_permutation_marker) == 0x50 ); // max count: 64
		struct gbxmodel_region_permutation
		{
			struct __flags
			{
				TAG_FLAG(cannot_be_chosen_randomly);
			}; BOOST_STATIC_ASSERT( sizeof(__flags) == sizeof(long_flags) );

			TAG_FIELD(tag_string, name);
			TAG_FIELD(__flags, flags);
			TAG_PAD(int32, 7);
			TAG_FIELD(int16, detail_geometry_index[Enums::k_number_of_geometry_detail_levels]);
			PAD16;
			TAG_TBLOCK(markers, model_region_permutation_marker);
		}; BOOST_STATIC_ASSERT( sizeof(gbxmodel_region_permutation) == 0x58 ); // max count: 32
		struct gbxmodel_region
		{
			TAG_FIELD(tag_string, name);
			TAG_PAD(int32, 8);
			TAG_TBLOCK(permutations, gbxmodel_region_permutation);
		}; BOOST_STATIC_ASSERT( sizeof(gbxmodel_region) == 0x4C ); // max count: 32


		struct model_triangle
		{
			TAG_FIELD(int16, vertex0_index);
			TAG_FIELD(int16, vertex1_index);
			TAG_FIELD(int16, vertex2_index);
		}; BOOST_STATIC_ASSERT( sizeof(model_triangle) == 0x6 ); // max count: 65535
		struct gbxmodel_geometry_part
		{
			struct __flags
			{
				TAG_FLAG(stripped_internal);
				TAG_FLAG(zoner);
			}; BOOST_STATIC_ASSERT( sizeof(__flags) == sizeof(long_flags) );

			TAG_FIELD(__flags, flags);
			TAG_FIELD(int16, shader_index);
			TAG_FIELD(sbyte, prev_filthy_part_index);
			TAG_FIELD(sbyte, next_filthy_part_index);
			TAG_FIELD(int16, centroid_primary_node);
			TAG_FIELD(int16, centroid_secondary_node);
			TAG_FIELD(real_fraction, centroid_primary_weight);
			TAG_FIELD(real_fraction, centroid_secondary_weight);
			TAG_FIELD(real_point3d, centroid);
			TAG_TBLOCK(uncompressed_vertices, Rasterizer::model_vertex_uncompressed); // max count: 65535
			TAG_TBLOCK(compressed_vertices, Rasterizer::model_vertex_compressed); // max count: 65535
			TAG_TBLOCK(triangles, model_triangle);
			Rasterizer::rasterizer_triangle_buffer triangle_buffer;
			Rasterizer::rasterizer_vertex_buffer vertex_buffer;
			TAG_PAD(byte, 1);
			TAG_PAD(byte, 1);
			TAG_PAD(byte, 1);
			sbyte num_nodes;

			sbyte node_table[Enums::k_maximum_nodes_per_model_geometry_part];
			PAD16;
		}; BOOST_STATIC_ASSERT( sizeof(gbxmodel_geometry_part) == 0x84 ); // max count: 32
		struct gbxmodel_geometry
		{
			struct __flags
			{
				TAG_FLAG(unused);
			}; BOOST_STATIC_ASSERT( sizeof(__flags) == sizeof(long_flags) );

			TAG_FIELD(__flags, flags);
			TAG_PAD(int32, 8);
			TAG_TBLOCK(parts, gbxmodel_geometry_part);
		}; BOOST_STATIC_ASSERT( sizeof(gbxmodel_geometry) == 0x30 ); // max count: 256

		struct model_shader_reference
		{
			TAG_FIELD(tag_reference, shader, 'shdr');
			TAG_FIELD(int16, permutation);
			PAD16;
			TAG_PAD(int32, 3);
		}; BOOST_STATIC_ASSERT( sizeof(model_shader_reference) == 0x20 ); // max count: 64
		struct gbxmodel_definition
		{
			enum { k_group_tag = 'mod2' };

			struct __flags
			{
				TAG_FLAG(blend_shared_normals);
				TAG_FLAG(parts_have_local_nodes);
				TAG_FLAG(ignore_skinning);
			}; BOOST_STATIC_ASSERT( sizeof(__flags) == sizeof(long_flags) );

			TAG_FIELD(__flags, flags);
			TAG_FIELD(int32, node_list_checksum);
			TAG_FIELD(real, detail_level_cutoffs[Enums::k_number_of_geometry_detail_levels], "pixels");
			TAG_FIELD(int16, detail_level_node_counts[Enums::k_number_of_geometry_detail_levels], "nodes");
			PAD16;
			TAG_PAD(int32, 2);
			TAG_FIELD(real, base_map_u_scale, "", "0 defaults to 1");
			TAG_FIELD(real, base_map_v_scale, "", "0 defaults to 1");
			TAG_PAD(int32, 29);
			TAG_TBLOCK(markers, model_markers);
			TAG_TBLOCK(nodes, model_node);
			TAG_TBLOCK(regions, gbxmodel_region);
			TAG_TBLOCK(geometries, gbxmodel_geometry);
			TAG_TBLOCK(shaders, model_shader_reference);
		}; BOOST_STATIC_ASSERT( sizeof(gbxmodel_definition) == 0xE8 ); // max count: 1

		struct model_definition
		{
			enum { k_group_tag = 'mode' };
		};
	};
};