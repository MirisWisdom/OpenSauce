/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <TagGroups/Halo1/TagGroupDefinitions.hpp>

#include <blamlib/Halo1/models/collision_model_definitions.hpp>

namespace Yelo
{
	namespace TagGroups
	{
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
			TAG_FIELD(tag_reference, shader, 'shdr');
			TAG_FIELD(int16, shader_permutation);
			TAG_FIELD(word_flags, flags);
			TAG_FIELD(int32, surfaces, structure_surface);
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

			TAG_FIELD(tag_reference, lightmap_bitmaps, 'bitm');
			TAG_FIELD(real_bounds, vehicle_heights, "floor", "ceiling");
			TAG_PAD(int32, 5);

			struct {
				TAG_FIELD(real_rgb_color, ambient_color);
				PAD32; // future power value?

				struct {
					TAG_FIELD(real_rgb_color, color);
					TAG_FIELD(real_vector3d, color_direction);
				}distance_light[2];
				TAG_PAD(int32, 3);

				TAG_FIELD(real_argb_color, reflection_tint);
				TAG_FIELD(real_vector3d, shadow_vector);
				TAG_FIELD(real_rgb_color, shadow_color);
				PAD32; // future power value?
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