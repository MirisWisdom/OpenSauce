/*
    Yelo: Open Sauce SDK

    Copyright (C) 2005-2010  Kornner Studios (http://kornner.com)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#pragma once

#include <TagGroups/Halo1/TagGroupDefinitions.hpp>
#include <TagGroups/Halo1/structure_bsp_definitions.hpp>

namespace Yelo
{
	namespace TagGroups
	{
		struct s_model_marker_instance
		{
			TAG_FIELD(byte, region_index);
			TAG_FIELD(byte, permutation_index);
			TAG_FIELD(byte, node_index);
			PAD8;
			TAG_FIELD(real_point3d, translation);
			TAG_FIELD(real_quaternion, rotation);
		}; BOOST_STATIC_ASSERT( sizeof(s_model_marker_instance) == 0x20 ); // max count: 32

		struct s_model_markers
		{
			TAG_FIELD(tag_string, name);
			TAG_FIELD(int16, magic_identifier);
			PAD16;
			TAG_PAD(int32, 4);
			TAG_TBLOCK(instances, s_model_marker_instance);
		}; BOOST_STATIC_ASSERT( sizeof(s_model_markers) == 0x40 ); // max count: 256

		struct s_model_node
		{
			TAG_FIELD(tag_string, name);
			TAG_FIELD(int16, next_sibling_node_index);
			TAG_FIELD(int16, first_child_node_index);
			TAG_FIELD(int16, parent_node_index);
			PAD16;
			TAG_FIELD(real_point3d, default_translation);
			TAG_FIELD(real_quaternion, default_rotation);
			TAG_FIELD(real, node_distance_from_parent);
			TAG_PAD(int32, 8);
			TAG_PAD(int32, 13);
		}; BOOST_STATIC_ASSERT( sizeof(s_model_node) == 0x9C ); // max count: 64

		struct s_model_region_permutation_marker
		{
			TAG_FIELD(tag_string, name);
			TAG_FIELD(int16, node_index);
			PAD16;
			TAG_FIELD(real_quaternion, rotation);
			TAG_FIELD(real_point3d, translation);
			TAG_PAD(int32, 4);
		}; BOOST_STATIC_ASSERT( sizeof(s_model_region_permutation_marker) == 0x50 ); // max count: 64

		struct s_gbxmodel_region_permutation
		{
			struct __flags
			{
				TAG_FLAG(cannot_be_chosen_randomly);
			}; BOOST_STATIC_ASSERT( sizeof(__flags) == sizeof(long_flags) );

			TAG_FIELD(tag_string, name);
			TAG_FIELD(__flags, flags);
			TAG_PAD(int32, 7);
			TAG_FIELD(int16, super_low);
			TAG_FIELD(int16, low);
			TAG_FIELD(int16, medium);
			TAG_FIELD(int16, high);
			TAG_FIELD(int16, super_high);
			PAD16;
			TAG_TBLOCK(markers, s_model_region_permutation_marker);
		}; BOOST_STATIC_ASSERT( sizeof(s_gbxmodel_region_permutation) == 0x58 ); // max count: 32

		struct s_gbxmodel_region
		{
			TAG_FIELD(tag_string, name);
			TAG_PAD(int32, 8);
			TAG_TBLOCK(permutations, s_gbxmodel_region_permutation);
		}; BOOST_STATIC_ASSERT( sizeof(s_gbxmodel_region) == 0x4C ); // max count: 32


		struct s_model_triangle
		{
			TAG_FIELD(int16, vertex0_index);
			TAG_FIELD(int16, vertex1_index);
			TAG_FIELD(int16, vertex2_index);
		}; BOOST_STATIC_ASSERT( sizeof(s_model_triangle) == 0x6 ); // max count: 65535

		struct s_gbxmodel_geometry_part
		{
			struct __flags
			{
				TAG_FLAG(stripped_internal);
				TAG_FLAG(zoner);
			}; BOOST_STATIC_ASSERT( sizeof(__flags) == sizeof(long_flags) );

			TAG_FIELD(__flags, flags);
			TAG_FIELD(int16, shader_index);
			TAG_FIELD(byte, prev_filthy_part_index);
			TAG_FIELD(byte, next_filthy_part_index);
			TAG_FIELD(int16, centroid_primary_node);
			TAG_FIELD(int16, centroid_secondary_node);
			TAG_FIELD(real_fraction, centroid_primary_weight);
			TAG_FIELD(real_fraction, centroid_secondary_weight);
			TAG_FIELD(real_point3d, centroid);
			TAG_TBLOCK(uncompressed_vertices, Rasterizer::model_vertex_uncompressed); // max count: 65535
			TAG_TBLOCK(compressed_vertices, Rasterizer::model_vertex_compressed); // max count: 65535
			TAG_TBLOCK(triangles, s_model_triangle);
			TAG_PAD(int32, 5);
			TAG_PAD(int32, 4);
			TAG_PAD(byte, 1);
			TAG_PAD(byte, 1);
			TAG_PAD(byte, 1);
			TAG_PAD(byte, 1);
			TAG_PAD(int32, 6);
		}; BOOST_STATIC_ASSERT( sizeof(s_gbxmodel_geometry_part) == 0x84 ); // max count: 32

		struct s_gbxmodel_geometry
		{
			struct __flags
			{
				TAG_FLAG(unused);
			}; BOOST_STATIC_ASSERT( sizeof(__flags) == sizeof(long_flags) );

			TAG_FIELD(__flags, flags);
			TAG_PAD(int32, 8);
			TAG_TBLOCK(parts, s_gbxmodel_geometry_part);
		}; BOOST_STATIC_ASSERT( sizeof(s_gbxmodel_geometry) == 0x30 ); // max count: 256

		struct s_model_shader_reference
		{
			TAG_FIELD(tag_reference, shader, "shdr");
			TAG_FIELD(int16, permutation);
			PAD16;
			TAG_PAD(int32, 3);
		}; BOOST_STATIC_ASSERT( sizeof(s_model_shader_reference) == 0x20 ); // max count: 64

		struct s_gbxmodel_definition
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
			TAG_FIELD(real, super_high_detail_cutoff, "pixels");
			TAG_FIELD(real, high_detail_cutoff, "pixels");
			TAG_FIELD(real, medium_detail_cutoff, "pixels");
			TAG_FIELD(real, low_detail_cutoff, "pixels");
			TAG_FIELD(real, super_low_cutoff, "pixels");
			TAG_FIELD(int16, super_high_detail_node_count, "nodes");
			TAG_FIELD(int16, high_detail_node_count, "nodes");
			TAG_FIELD(int16, medium_detail_node_count, "nodes");
			TAG_FIELD(int16, low_detail_node_count, "nodes");
			TAG_FIELD(int16, super_low_detail_node_count, "nodes");
			PAD16;
			TAG_PAD(int32, 2);
			TAG_FIELD(real, base_map_u_scale, "", "0 defaults to 1");
			TAG_FIELD(real, base_map_v_scale, "", "0 defaults to 1");
			TAG_PAD(int32, 29);
			TAG_TBLOCK(markers, s_model_markers);
			TAG_TBLOCK(nodes, s_model_node);
			TAG_TBLOCK(regions, s_gbxmodel_region);
			TAG_TBLOCK(geometries, s_gbxmodel_geometry);
			TAG_TBLOCK(shaders, s_model_shader_reference);
		}; BOOST_STATIC_ASSERT( sizeof(s_gbxmodel_definition) == 0xE8 ); // max count: 1
	};
};
