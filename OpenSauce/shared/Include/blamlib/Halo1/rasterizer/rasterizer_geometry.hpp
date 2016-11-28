/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

namespace Yelo
{
	namespace Enums
	{
		enum {
			k_rasterizer_maximum_dynamic_triangles = 0x8000,

			k_max_dynamic_vertex_buffers = 0x100,
		};

		enum {
			_geometry_detail_level_super_low,
			_geometry_detail_level_low,
			_geometry_detail_level_medium,
			_geometry_detail_level_high,
			_geometry_detail_level_super_high,

			k_number_of_geometry_detail_levels
		};

		enum triangle_buffer_type : _enum
		{
			_triangle_buffer_type_0,
			_triangle_buffer_type_precompiled_strip,

			k_number_of_triangle_buffer_types
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
			_rasterizer_vertex_type_screen_transformed_lit,
			_rasterizer_vertex_type_screen_transformed_lit_specular,

			_rasterizer_vertex_type_environment_single_stream_ff,

			k_number_of_rasterizer_vertex_types,
		};
	};

	namespace Rasterizer
	{
		struct rasterizer_triangle
		{
			int16 vertex0_index;
			int16 vertex1_index;
			int16 vertex2_index;
		}; BOOST_STATIC_ASSERT(sizeof(rasterizer_triangle) == 0x6);

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
};