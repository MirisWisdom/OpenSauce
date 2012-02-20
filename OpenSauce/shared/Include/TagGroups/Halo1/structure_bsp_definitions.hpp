/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

#include <TagGroups/Halo1/TagGroupDefinitions.hpp>

namespace Yelo
{
	namespace Enums
	{
		enum triangle_buffer_type : _enum
		{
			_triangle_buffer_type_0,
			_triangle_buffer_type_precompiled_strip,

			NUMBER_OF_TRIANGLE_BUFFER_TYPES
		};

		enum rasterizer_vertex_type : _enum
		{
			_rasterizer_vertex_type_environment_uncompressed,
			_rasterizer_vertex_type_environment_compressed,

			_rasterizer_vertex_type_environment_lightmap_uncompressed,
			_rasterizer_vertex_type_environment_lightmap_compressed,

			_rasterizer_vertex_type_model_uncompressed,
			_rasterizer_vertex_type_model_compressed,

			_rasterizer_vertex_type_unlit,
			_rasterizer_vertex_type_dynamic_unlit,
			_rasterizer_vertex_type_dynamic_screen,
			_rasterizer_vertex_type_debug,
			_rasterizer_vertex_type_decal,
			_rasterizer_vertex_type_detail_object,

			_rasterizer_vertex_type_environment_uncompressed_ff,
			_rasterizer_vertex_type_environment_lightmap_uncompressed_ff,
			_rasterizer_vertex_type_model_uncompressed_ff,
			_rasterizer_vertex_type_model_processed,

			_rasterizer_vertex_type_unlit_zsprite,
			_rasterizer_vertex_type_screen_transformed_lit_specular,

			_rasterizer_vertex_type_environment_single_stream_ff,

			_rasterizer_vertex_type,
		};
	};

	namespace Rasterizer
	{
		struct rasterizer_triangle_buffer
		{
			Enums::triangle_buffer_type type;
			PAD16;
			int32 count;
			void* data;
			void* hardware_format;
		}; BOOST_STATIC_ASSERT( sizeof(rasterizer_triangle_buffer) == 0x10 );

		struct rasterizer_vertex_buffer
		{
			Enums::rasterizer_vertex_type type;
			PAD16;
			int32 vertex_start_index;
			int32 vertex_count;
			void* data;
			void* hardware_format;
		}; BOOST_STATIC_ASSERT( sizeof(rasterizer_vertex_buffer) == 0x14 );

		struct environment_vertex_uncompressed
		{
			real_point3d position;
			real_vector3d normal, binormal;
			real_vector3d tangent;
			real_point2d texcoord;
		};
		struct environment_vertex_compressed
		{
			real_point3d position;
			uint32 normal, binormal;
			uint32 tangent;
			real_point2d texcoord;
		};

		struct environment_lightmap_vertex_uncompressed
		{
			real_vector3d normal;
			real_point2d texcoord;
		};
		struct environment_lightmap_vertex_compressed
		{
			uint32 normal;
			point2d texcoord;
		};

		struct model_vertex_uncompressed
		{
			real_point3d position;
			real_vector3d normal, binormal;
			real_vector3d tangent;
			real_point2d texcoord;

			int16 node_indices[2];
			real node_weights[2];
		};
		struct model_vertex_compressed
		{
			real_point3d position;
			uint32 normal, binormal;
			uint32 tangent;
			point2d texcoord;

			byte node_indices[2];
			uint16 node_weights[2];
		};
	};

	namespace TagGroups
	{
		struct bsp3d_node
		{
			TAG_FIELD(int32, plane);
			TAG_FIELD(int32, back_child);
			TAG_FIELD(int32, front_child);
		};
		struct collision_leaf
		{
			TAG_FIELD(word_flags, flags);
			TAG_FIELD(int16, reference_count, "bsp2d");
			TAG_FIELD(int32, first_reference, "bsp2d");
		};
		struct bsp2d_reference
		{
			TAG_FIELD(int32, plane);
			TAG_FIELD(int32, bsp2d_node);
		};
		struct bsp2d_node
		{
			TAG_FIELD(real_plane2d, plane);
			TAG_FIELD(int32, left_child);
			TAG_FIELD(int32, rigt_child);
		};
		struct collision_surface
		{
			TAG_FIELD(int32, plane);
			TAG_FIELD(int32, first_edge);
			TAG_FIELD(byte_flags, flags);
			TAG_FIELD(sbyte, breakable_surface);
			TAG_FIELD(int16, material);
		};
		struct collision_edge
		{
			TAG_FIELD(int32, start_vertex);
			TAG_FIELD(int32, end_vertex);
			TAG_FIELD(int32, forward_edge);
			TAG_FIELD(int32, reverse_edge);
			TAG_FIELD(int32, left_surface);
			TAG_FIELD(int32, right_surface);
		};
		struct collision_vertex
		{
			TAG_FIELD(real_point3d, point);
			TAG_FIELD(int32, first_edge);
		};
		struct collision_bsp
		{
			TAG_TBLOCK(bsp3d_nodes, bsp3d_node);
			TAG_TBLOCK(planes, real_plane3d);
			TAG_TBLOCK(leaves, collision_leaf);
			struct {
				TAG_TBLOCK(references, bsp2d_reference);
				TAG_TBLOCK(nodes, bsp2d_node);
			}bsp2d;
			TAG_TBLOCK(surfaces, collision_surface);
			TAG_TBLOCK(edges, collision_edge);
			TAG_TBLOCK(vertices, collision_vertex);
		};
		
		struct structure_collision_material
		{
			TAG_FIELD(tag_reference, shader, "shdr");
			TAG_PAD(int32, 1);
		};
		struct structure_node
		{
			int16 skip[3];
		};
		struct structure_leaf
		{
			byte skip[6];
			PAD16;
			TAG_FIELD(int16, cluster);
			TAG_FIELD(int16, reference_count, "surface");
			TAG_FIELD(int32, first_reference, "surface");
		};
		struct structure_surface_reference
		{
			TAG_FIELD(int32, surface);
			TAG_FIELD(int32, index);
		};
		struct structure_surface
		{
			TAG_FIELD(int16, a)[3];
		};
		struct structure_bsp_material
		{
			TAG_FIELD(tag_reference, shader, "shdr");
			TAG_FIELD(int16, shader_permutation);
			TAG_FIELD(word_flags, flags);
			TAG_FIELD(int32, surfaces, "structure_surface");
			TAG_FIELD(int32, surface_count);
			TAG_FIELD(real_point3d, centroid);
			TAG_FIELD(real_rgb_color, ambient_color);
			TAG_FIELD(int16, distant_light_count);
			PAD16;

			struct {
				TAG_FIELD(real_rgb_color, color);
				TAG_FIELD(real_vector3d, direction);
			}distant_lights[2];
			TAG_PAD(int32, 3);
			TAG_FIELD(real_argb_color, reflection_tint);
			TAG_FIELD(real_vector3d, shadow_vector);
			TAG_FIELD(real_rgb_color, shadow_color);
			TAG_FIELD(real_plane3d, plane);
			TAG_FIELD(int16, breakable_surface);
			PAD16;

			Rasterizer::rasterizer_vertex_buffer 
				vertices, lightmap_vertices;
			TAG_FIELD(tag_data, uncompressed_vertices);
			TAG_FIELD(tag_data, compressed_vertices);
		}; BOOST_STATIC_ASSERT( sizeof(structure_bsp_material) == 0x100 );
		struct structure_bsp_lightmap
		{
			TAG_FIELD(int16, bitmap);
			PAD16;
			TAG_PAD(int32, 4);
			TAG_TBLOCK(materials, structure_bsp_material);
		};

		struct structure_subcluster
		{
			struct {
				real_bounds x, y, z;
			}world_bounds;
			TAG_TBLOCK(surface_indices, int32);
		};
		struct structure_mirror
		{
			TAG_FIELD(real_plane3d, plane);
			TAG_PAD(int32, 5);
			TAG_FIELD(tag_reference, shader);
			TAG_TBLOCK(vertices, real_point3d);
		};
		struct structure_cluster
		{
			TAG_FIELD(int16, sky);
			TAG_FIELD(int16, fog);
			TAG_FIELD(int16, background_sound); // block index
			TAG_FIELD(int16, sound_environment); // block index
			TAG_FIELD(int16, weather); // block index
			TAG_FIELD(int16, transition_structure_bsp);
			TAG_PAD(int32, 1 + 6);
			TAG_TBLOCK(predicted_resources, predicted_resource);
			TAG_TBLOCK(subclusters, structure_subcluster);
			TAG_FIELD(int16, first_lens_flare_marker_index);
			TAG_FIELD(int16, lens_flare_marker_count);
			TAG_TBLOCK(surface_indices, int32);
			TAG_BLOCK(mirrors, structure_mirror);
			TAG_BLOCK(portals, int16);
		}; BOOST_STATIC_ASSERT( sizeof(structure_cluster) == 0x68 );

		struct s_structure_fog_region
		{
			TAG_PAD(int32, 9);
			TAG_FIELD(int16, fog_palette); // block index
			TAG_FIELD(int16, weather_palette); // block index
		};
		struct s_structure_fog_palette
		{
			TAG_FIELD(tag_string, name);
			TAG_FIELD(tag_reference, fog, 'fog ');
			TAG_PAD(int32, 1);
			TAG_FIELD(tag_string, fog_scale_function);
			TAG_PAD(int32, 11 + 2);
		};
		struct structure_weather_palette_entry
		{
			TAG_FIELD(tag_string, name);
			TAG_FIELD(tag_reference, particle_system, 'rain');
			TAG_PAD(int32, 1);
			TAG_FIELD(tag_string, particle_system_scale_function);
			TAG_PAD(int32, 11);
			TAG_FIELD(tag_reference, wind, 'wind');
			TAG_FIELD(real_vector3d, wind_direction);
			TAG_FIELD(real, wind_magnitude);
			TAG_PAD(int32, 1);
			TAG_FIELD(tag_string, wind_scale_function);
			TAG_PAD(int32, 11);
		};

		struct structure_background_sound_palette_entry
		{
			TAG_FIELD(tag_string, name);
			TAG_FIELD(tag_reference, background_sound, 'lsnd');
			TAG_PAD(int32, 1);
			TAG_FIELD(tag_string, scale_function);
			TAG_PAD(tag_string, 1);
		};
		struct structure_sound_environment_palette_entry
		{
			TAG_FIELD(tag_string, name);
			TAG_FIELD(tag_reference, sound_environment, 'snde');
			TAG_PAD(tag_string, 1);
		};

		struct structure_bsp
		{
			enum { k_group_tag = 'sbsp' };

			TAG_FIELD(tag_reference, lightmap_bitmaps, "bitm");
			TAG_FIELD(real_bounds, vehicle_heights, "floor", "ceiling");
			TAG_PAD(int32, 5);

			struct {
				TAG_FIELD(real_rgb_color, ambient_color);
				PAD32; // future alpha padding?

				struct {
					TAG_FIELD(real_rgb_color, color);
					TAG_FIELD(real_vector3d, color_direction);
				}distance_light[2];
				TAG_PAD(int32, 3);

				TAG_FIELD(real_argb_color, reflection_tint);
				TAG_FIELD(real_vector3d, shadow_vector);
				TAG_FIELD(real_rgb_color, shadow_color);
				PAD32; // future alpha padding?
			}defaults;

			TAG_TBLOCK(collision_materials, structure_collision_material);
			TAG_TBLOCK(collision_bsp, collision_bsp);
			TAG_TBLOCK(nodes, structure_node);

			struct {
				real_bounds x, y, z;
			}world_bounds;

			TAG_TBLOCK(leafs, structure_leaf);
			TAG_TBLOCK(leaf_surfaces, structure_surface_reference);
			TAG_TBLOCK(surfaces, structure_surface);

			TAG_TBLOCK(lightmaps, structure_bsp_lightmap);
			PAD_TYPE(tag_block);

			TAG_BLOCK(lens_flares, structure_bsp_lens_flare_block);
			TAG_BLOCK(lens_flare_markers, structure_bsp_lens_flare_marker_block);

			TAG_TBLOCK_(clusters, structure_cluster);
			TAG_FIELD(tag_data, cluster_data);
			TAG_BLOCK(cluster_portals, cluster_portal);
			PAD_TYPE(tag_block);

			TAG_BLOCK(breakable_surfaces, structure_breakable_surface);
			TAG_BLOCK(fog_planes, s_structure_fog_plane);
			TAG_TBLOCK(fog_regions, s_structure_fog_region);
			TAG_TBLOCK(fog_palette, s_structure_fog_palette);
			PAD_TYPE(tag_block);
			PAD_TYPE(tag_block);

			TAG_TBLOCK(weather_palette, structure_weather_palette_entry);
			TAG_BLOCK(weather_polyhedra, structure_weather_polyhedron);
			PAD_TYPE(tag_block);
			PAD_TYPE(tag_block);

			TAG_TBLOCK(pathfinding_surfaces, byte);
			TAG_TBLOCK(pathfinding_edges, byte);

			TAG_TBLOCK(background_sound_palette, structure_background_sound_palette_entry);
			TAG_TBLOCK(sound_environment_palette, structure_sound_environment_palette_entry);
			TAG_FIELD(tag_data, sound_pas_data);
			PAD_TYPE(tag_block);
			PAD_TYPE(tag_block);

			TAG_BLOCK(markers, structure_marker);
			TAG_BLOCK(detail_objects, detail_object_data);
			TAG_BLOCK(runtime_decals, structure_runtime_decal);
			PAD64;
			PAD32;
			TAG_BLOCK(leaf_map_leaves, map_leaf);
			TAG_BLOCK(leaf_map_portals, leaf_connection);

		}; BOOST_STATIC_ASSERT( sizeof(structure_bsp) == 0x288 );
	};
};